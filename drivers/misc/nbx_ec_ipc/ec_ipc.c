/*
 * Copyright (C) 2011-2012 Sony Corporation
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
/* EC IPC */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/list.h>
#include <linux/suspend.h>
#include <linux/err.h>
#include <linux/semaphore.h>
#include <linux/suspend.h>

#include <linux/nbx_ec_ipc.h>
#include "ec_ipc_serial.h"

/*#define DBGPRINT*/

#define LOCKED_STATEMENT(mutex, statement)	\
	down(mutex);				\
	do {					\
		statement;			\
	} while(0);				\
	up(mutex);

static DECLARE_WAIT_QUEUE_HEAD(ec_ipc_tx_wait);
static volatile int ec_ipc_tx_active;

static atomic_t ec_ipc_work_alive;

static inline int read_and_set(volatile int* p, int val)
{ int ret = *p; *p = val; return ret; }

#define PACKET_HEAD_SIGN  0xAAAA

#define PACKET_TYPE_REQUEST (1 << 0)
#define PACKET_TYPE_NORESPONSE (1 << 1)
#define PACKET_TYPE_RESPONSE (1 << 2)
#define PACKET_TYPE_EVENT (1 << 3)
#define PACKET_TYPE_ERR_CSUM (1 << 4)
#define PACKET_TYPE_ERR_FRAME (1 << 5)
#define PACKET_TYPE_ERR_UNDEF_COM (1 << 6)

#define PACKET_FOOT_SIGN 0xCC

#define sizeof_PACKET_HEAD (2+1+1+1+1+1) /* head sign, type, size, frame num, pid, cid */
#define sizeof_PACKET_FOOT (1+1) /* checksum, foot sign */

static LIST_HEAD(request_list);
static DEFINE_SEMAPHORE(request_list_mutex);
static atomic_t request_enable;

static LIST_HEAD(event_list);
static DEFINE_SEMAPHORE(event_list_mutex);

static uint8_t ec_ipc_request_frame_num;
static DEFINE_SEMAPHORE(request_frame_num_mutex);

struct request_t {
	struct list_head list;
	uint8_t frame_num;
	uint8_t pid;
	uint8_t cid;
	uint8_t* sendbuf;
	uint8_t sizeof_sendbuf;
	int retry_count;
	void (*res_func)(const uint8_t* buf, int size, void* private_data);
	void* private_data;
	int sending;
	int done;
};

static inline struct request_t* request_list_top(void)
{
	if(list_empty(&request_list)) return NULL;
	return list_entry(request_list.next, struct request_t, list);
}

struct event_t {
	struct list_head list;
	uint8_t cid;
	void (*event_func)(const uint8_t* buf, int size);
};

static struct request_t* construct_request(uint8_t pid, uint8_t cid, uint8_t frame_num,
				const uint8_t* buf, int size,
				void (*res_func)(const uint8_t* buf, int size, void* private_data),
				void* private_data)
{
	int i;
	uint8_t sum = 0;
	struct request_t* new_req;

	new_req = kzalloc(sizeof(struct request_t), GFP_KERNEL);
	if(new_req == NULL) return NULL;

	new_req->sizeof_sendbuf = sizeof_PACKET_HEAD + size + sizeof_PACKET_FOOT;
	new_req->sendbuf = kzalloc(new_req->sizeof_sendbuf, GFP_KERNEL);
	if(new_req->sendbuf == NULL) {
		kfree(new_req);
		return NULL;
	}

	new_req->frame_num = frame_num;
	new_req->pid = pid;
	new_req->cid = cid;
	new_req->res_func = res_func;
	new_req->private_data = private_data;
	new_req->retry_count = 10;
	new_req->sending = 0;
	new_req->done = 0;

	(new_req->sendbuf)[0] = (PACKET_HEAD_SIGN >> 8) & 0xFF;
	(new_req->sendbuf)[1] = PACKET_HEAD_SIGN & 0xFF;
	sum += (new_req->sendbuf)[2] = PACKET_TYPE_REQUEST |
		((new_req->res_func == NULL)? PACKET_TYPE_NORESPONSE: 0);
	sum += (new_req->sendbuf)[3] = new_req->sizeof_sendbuf;
	sum += (new_req->sendbuf)[4] = new_req->frame_num;
	sum += (new_req->sendbuf)[5] = new_req->pid;
	sum += (new_req->sendbuf)[6] = new_req->cid;
	for(i = 0; i < size; i++) {
		sum += (new_req->sendbuf)[sizeof_PACKET_HEAD + i] = buf[i];
	}
	(new_req->sendbuf)[sizeof_PACKET_HEAD + size] = (uint8_t)(0x100 - (uint16_t)sum);
	(new_req->sendbuf)[sizeof_PACKET_HEAD + size + 1] = PACKET_FOOT_SIGN;

	return new_req;
}
static void destruct_request(struct request_t* req)
{
	if(req == NULL) return;

	kfree(req->sendbuf);
	kfree(req);
}

int ec_ipc_send_request_async(uint8_t pid, uint8_t cid, const uint8_t* buf, int size,
			void (*res_func)(const uint8_t* buf, int size, void* private_data),
			void* private_data)
{
	struct request_t* new_req;
	uint8_t frame_num = 0xFF;

	LOCKED_STATEMENT(&request_frame_num_mutex,
			frame_num = ec_ipc_request_frame_num;
			ec_ipc_request_frame_num++;
		);

	new_req = construct_request(pid, cid, frame_num, buf, size, res_func, private_data);
	if(new_req == NULL) return -ENOMEM;

	LOCKED_STATEMENT( &request_list_mutex,
			if(atomic_read(&request_enable)) {
				INIT_LIST_HEAD(&(new_req->list));
				list_add_tail(&(new_req->list), &request_list);
			}
			else {
				destruct_request(new_req);
				new_req = NULL;

				pr_info("ec_ipc : can't request send packet in tx terminated. PID %02x, CID %02x\n", pid, cid);
			}
		);

	if(new_req == NULL) return -EAGAIN;

	ec_ipc_tx_active = 1;
	wake_up_interruptible(&ec_ipc_tx_wait);

	return frame_num;
}
EXPORT_SYMBOL(ec_ipc_send_request_async);

void ec_ipc_cancel_request(uint8_t pid, uint8_t cid, uint8_t frame_num)
{
	int occur = 0;
	struct request_t* cancel_req;

	LOCKED_STATEMENT( &request_list_mutex,
			list_for_each_entry(cancel_req, &request_list, list) {
				if((cancel_req->pid == pid) &&
					(cancel_req->cid == cid) &&
					(cancel_req->frame_num == frame_num) ){

					cancel_req->done = 1;
					occur = 1;
					break;
				}
			}
		);

	if(occur != 0) {
		ec_ipc_tx_active = 1;
		wake_up_interruptible(&ec_ipc_tx_wait);
	}
}
EXPORT_SYMBOL(ec_ipc_cancel_request);

/*********** block request I/F **************/

struct block_request_t {
	struct list_head list;
	uint8_t* buf;
	int buf_size;
	int received_size;
	volatile int blocking;
	wait_queue_head_t wait;
};

static LIST_HEAD(block_request_list);
static DEFINE_SEMAPHORE(block_request_list_mutex);

static void ec_ipc_receive_response(const uint8_t* buf, int size, void* private_data)
{
#define SMALL(a,b) (((a) < (b))?(a):(b))

	struct block_request_t* block_request;

	LOCKED_STATEMENT(&block_request_list_mutex,
			list_for_each_entry(block_request, &block_request_list, list){
				if(block_request == (struct block_request_t*)private_data){
					if(buf != NULL) {
						block_request->received_size = SMALL(size,block_request->buf_size);

						if(block_request->buf != NULL){
							memcpy(block_request->buf, buf, block_request->received_size);
						}
					}
					else {
						block_request->received_size = -1;
					}

					block_request->blocking = 0;
					wake_up_interruptible(&(block_request->wait));
				}
			}
		);

}

ssize_t ec_ipc_send_request_timeout(uint8_t pid, uint8_t cid, const uint8_t* buf, int size,
				uint8_t* response_buf, int response_buf_size, int timeout_ms)
{
	ssize_t ret = -ERESTARTSYS;
	struct block_request_t block_request;
	int frame_num;
	long timeout;

	block_request.buf = response_buf;
	block_request.buf_size = response_buf_size;
	block_request.received_size = -1;

	block_request.blocking = 1;
	init_waitqueue_head(&(block_request.wait));

	LOCKED_STATEMENT(&block_request_list_mutex,
			INIT_LIST_HEAD(&(block_request.list));
			list_add_tail(&(block_request.list), &block_request_list);
		);

	do {
		frame_num = ec_ipc_send_request_async(pid, cid, buf, size,
						&ec_ipc_receive_response, (void*)&block_request);
		if(frame_num < 0){
			ret = -EINVAL;
			break;
		}

		if(timeout_ms <= 0) {
			if(0 != wait_event_interruptible(block_request.wait, (block_request.blocking == 0)) ){
				ret = -ERESTARTSYS;
				break;
			}
		}
		else {
			timeout = wait_event_interruptible_timeout(block_request.wait, (block_request.blocking == 0),
								((((timeout_ms-1)*HZ))/1000)+1);
			if(timeout < 0){
				ret = -ERESTARTSYS;
				break;
			}
			else if(timeout == 0){
				ec_ipc_cancel_request(pid, cid, frame_num);
				/* wait canceled */
				wait_event_interruptible_timeout(block_request.wait, (block_request.blocking == 0), HZ);
			}
		}

		ret = block_request.received_size;
	} while(0);


	LOCKED_STATEMENT(&block_request_list_mutex,
			list_del(&(block_request.list));
		);

	return ret;
}
EXPORT_SYMBOL(ec_ipc_send_request_timeout);
/*********** block request I/F. over ********/

int ec_ipc_register_recv_event(uint8_t cid, void (*event_func)(const uint8_t* buf, int size))
{
	struct event_t* reg_event;

	reg_event = kzalloc(sizeof(struct event_t), GFP_KERNEL);
	if(reg_event == NULL) return -ENOMEM;

	reg_event->cid = cid;
	reg_event->event_func = event_func;

	LOCKED_STATEMENT(&event_list_mutex,
			INIT_LIST_HEAD(&(reg_event->list));
			list_add_tail(&(reg_event->list), &event_list);
		);

	return 0;
}
EXPORT_SYMBOL(ec_ipc_register_recv_event);

int ec_ipc_unregister_recv_event(uint8_t cid)
{
	struct event_t* unreg_event;
	struct event_t* n;

	LOCKED_STATEMENT(&event_list_mutex,
			list_for_each_entry_safe(unreg_event, n, &event_list, list) {
				if(unreg_event->cid == cid) {
					list_del(&(unreg_event->list));
					kfree(unreg_event);
				}
			}
		);

	return 0;
}
EXPORT_SYMBOL(ec_ipc_unregister_recv_event);

/*********** query EC INT I/F. *************/
static DECLARE_WAIT_QUEUE_HEAD(ec_int_wait);
static atomic_t ec_int_updated;
static uint8_t ec_int_factor;

static void ec_ipc_query_ec_int_response(const uint8_t* buf, int size, void* private_data)
{
	enum {
		QUERY_ECINT_RES_WAKEUP = 0,
		QUERY_ECINT_RES_SHUTDOWN,
		NOOF_QUERY_ECINT_RES,
	};

#define QUERY_ECINT_RES_WAKEUP_POWERKEY (1 << 0)
#define QUERY_ECINT_RES_WAKEUP_LID      (1 << 1)
#define QUERY_ECINT_RES_WAKEUP_TOPCOVER (1 << 2)
#define QUERY_ECINT_RES_WAKEUP_SHUTREQ  (1 << 3)
#define QUERY_ECINT_RES_WAKEUP_AC       (1 << 4)

	ec_int_factor = 0;

	if( (buf != NULL) && (NOOF_QUERY_ECINT_RES <= size) ){
		if(buf[QUERY_ECINT_RES_WAKEUP] & QUERY_ECINT_RES_WAKEUP_POWERKEY) {
			ec_int_factor |= EC_INT_POWERKEY;
		}
		if(buf[QUERY_ECINT_RES_WAKEUP] & QUERY_ECINT_RES_WAKEUP_LID) {
			ec_int_factor |= EC_INT_LID;
		}
		if(buf[QUERY_ECINT_RES_WAKEUP] & QUERY_ECINT_RES_WAKEUP_TOPCOVER) {
			ec_int_factor |= EC_INT_TOPCOVER;
		}
		if(buf[QUERY_ECINT_RES_WAKEUP] & QUERY_ECINT_RES_WAKEUP_SHUTREQ) {
			ec_int_factor |= EC_INT_SHUTREQ;
		}
		if(buf[QUERY_ECINT_RES_WAKEUP] & QUERY_ECINT_RES_WAKEUP_AC) {
			ec_int_factor |= EC_INT_AC;
		}
#ifdef DBGPRINT
		pr_info("ec_ipc:EC INT %s%s%s%s%s\n",
			((ec_int_factor & EC_INT_POWERKEY) ? "POWERKEY " : ""),
			((ec_int_factor & EC_INT_LID) ? "LID " : ""),
			((ec_int_factor & EC_INT_TOPCOVER) ? "TOPCOVER " : ""),
			((ec_int_factor & EC_INT_SHUTREQ) ? "SHUTREQ " : ""),
			((ec_int_factor & EC_INT_AC) ? "AC" : ""));
#endif
	}
	else {
		pr_err("nbx_ec_ipc:query EC INT failed.\n");
	}

	atomic_set(&ec_int_updated, 1);
	wake_up_interruptible(&ec_int_wait);
}

static void ec_ipc_query_ec_int(void)
{
#define EC_IPC_CID_QUERY_ECINT_REQUEST 0x94

	atomic_set(&ec_int_updated, 0);
	ec_int_factor = 0;

	ec_ipc_send_request_async(EC_IPC_PID_ECINT, EC_IPC_CID_QUERY_ECINT_REQUEST, NULL, 0,
				&ec_ipc_query_ec_int_response, NULL);
}

int ec_ipc_get_ec_int_factor(uint8_t factor)
{
	int ret;

	ret = wait_event_interruptible_timeout(ec_int_wait, (atomic_read(&ec_int_updated) != 0), (10 * HZ));
	if(0 < ret) {
		return ((ec_int_factor & factor) != 0);
	}
	else {
		pr_err("nbx_ec_ipc:ec_ipc_get_ec_int_factor() timeout or interrupt.\n");
		return 0;
	}
}
EXPORT_SYMBOL(ec_ipc_get_ec_int_factor);
/*********** query EC INT I/F. over ********/

/*########################################################*/

static void ec_ipc_tx_thread(struct work_struct* work)
{
	long timeout_remain = HZ;

	while(atomic_read(&ec_ipc_work_alive)) {
		uint8_t* sendbuf = NULL;
		int sizeof_sendbuf = 0;

		LOCKED_STATEMENT(&request_list_mutex,
				struct request_t* tx_current = request_list_top();

				if(tx_current == NULL) {
					timeout_remain = 60 * HZ;
					break;
				}
				else {
					if(tx_current->done) {
						timeout_remain = 0;
						break;
					}

					if(tx_current->sending) break;

					/* sending new request */
					timeout_remain = HZ;
					sizeof_sendbuf = tx_current->sizeof_sendbuf;
					sendbuf = kzalloc(sizeof_sendbuf, GFP_KERNEL);
					if(sendbuf == NULL) break;

					tx_current->sending = 1;

					memcpy(sendbuf, tx_current->sendbuf, sizeof_sendbuf);

					if(tx_current->res_func == NULL) {
						tx_current->done = 1;
						timeout_remain = 0;
					}
				}
			);

		/* send outside the lock */
		if(sendbuf != NULL) {
			ssize_t result_write;
			ssize_t sizeof_written = 0;

			while(1) {
				if(0 != ec_ipc_serial_open()) {
					msleep(50);
					continue;
				}

				result_write = ec_ipc_serial_write(&(sendbuf[sizeof_written]),
								sizeof_sendbuf - sizeof_written);
				if(result_write < 0) break;

				sizeof_written += result_write;
				if(sizeof_sendbuf <= sizeof_written) break;

				schedule();
			}

#ifdef DBGPRINT
			{
				int i;
				char pstr[256];
				snprintf(pstr, sizeof(pstr), "ec_ipc: %d = write() ", sizeof_sendbuf);
				for(i=0;i<sizeof_sendbuf;i++) snprintf(pstr+strlen(pstr), sizeof(pstr)-strlen(pstr), "%x,",sendbuf[i]);
				pstr[sizeof(pstr)-1] = '\0';

				pr_info("%s\n", pstr);
			}
#endif

			kfree(sendbuf);
			sendbuf = NULL;
		}

		timeout_remain =
			wait_event_interruptible_timeout(ec_ipc_tx_wait,
							!atomic_read(&ec_ipc_work_alive) ||
							read_and_set(&ec_ipc_tx_active, 0),
							timeout_remain);

		if(!atomic_read(&ec_ipc_work_alive)) break;

		{
			void (*res_func)(const uint8_t*, int, void*) = NULL;
			void* private_data = NULL;

			LOCKED_STATEMENT(&request_list_mutex,
					struct request_t* tx_current = request_list_top();
					if(tx_current == NULL) break;

					if(!(tx_current->done)) {
						if(timeout_remain <= 0) {
							tx_current->retry_count--;

							if(tx_current->retry_count <= 0) {
								pr_err("ec_ipc : send packet timeout. give up. PID %02x, CID %02x\n",
									tx_current->pid, tx_current->cid);

								tx_current->done = 1;
							}
							else {
								pr_err("ec_ipc : send packet timeout. retry.\n");

								tx_current->sending = 0;
							}
						}
					}

					if(tx_current->done) {
						/* if res_func remains, it is give up or canceled */
						res_func = tx_current->res_func;
						private_data = tx_current->private_data;

						list_del(&(tx_current->list));
						destruct_request(tx_current);
					}
				);

			if(res_func != NULL) {
				(*res_func)(NULL, 0, private_data);
			}
		}
	}

	/* remove request list */
	{
		struct request_t* del_req;
		void (*res_func)(const uint8_t*, int, void*);
		void* private_data;
		uint8_t pid, cid;

		do {
			del_req = NULL;
			res_func = NULL;
			private_data = NULL;

			LOCKED_STATEMENT(&request_list_mutex,
					del_req = request_list_top();
					if(del_req == NULL) break;

					res_func = del_req->res_func;
					private_data = del_req->private_data;
					pid = del_req->pid;
					cid = del_req->cid;

					list_del(&(del_req->list));
					destruct_request(del_req);
				);

			if(res_func != NULL) {
				(*res_func)(NULL, 0, private_data);
				pr_info("ec_ipc : send packet canceled by tx terminate. PID %02x, CID %02x\n", pid, cid);
			}
		} while(del_req != NULL);
	}

	return;
}

static int ec_ipc_rx_response(uint8_t frame_num, uint8_t pid, uint8_t cid, uint8_t* data, uint8_t size);
static void ec_ipc_rx_event(uint8_t frame_num, uint8_t pid, uint8_t cid, uint8_t* data, uint8_t size);

static void ec_ipc_rx_thread(struct work_struct* work)
{
	int i;
	uint8_t recv_head_1st,recv_head_2nd;
	uint8_t recv_type, recv_size, recv_frame_num, recv_pid, recv_cid;
	uint8_t recv_data_size;
	uint8_t* recv_data;
	uint8_t recv_csum, recv_foot;
	uint8_t calc_sum;
	int recv_packet_comp;
	uint8_t dummy_size0;

	while(atomic_read(&ec_ipc_work_alive)) {

		if(0 != ec_ipc_serial_open()) {
			msleep(50);
			continue;
		}

		recv_packet_comp = 0;

		/* sync head 1st byte */
		if(sizeof(recv_head_1st) != ec_ipc_serial_read(&recv_head_1st, sizeof(recv_head_1st), 100) ){
			schedule();
			continue;
		}
		if(recv_head_1st != ((PACKET_HEAD_SIGN >> 8) & 0xFF) ){
			/* invalid head received */
			continue;
		}
		/* receive head 2nd byte */
		if(sizeof(recv_head_2nd) != ec_ipc_serial_read(&recv_head_2nd, sizeof(recv_head_2nd), 100) ){
			/* receive packet timeout */
			pr_err("ec_ipc: timeout receive packet HEAD 2nd byte\n");
			continue;
		}
		if(recv_head_2nd != (PACKET_HEAD_SIGN & 0xFF) ){
			/* invalid head received */
			pr_err("ec_ipc: invalid packet HEAD 2nd byte received\n");
			continue;
		}
		/* receive type */
		if(sizeof(recv_type) != ec_ipc_serial_read(&recv_type, sizeof(recv_type), 100) ){
			/* receive packet timeout */
			pr_err("ec_ipc: timeout receive packet TYPE\n");
			continue;
		}
		if((recv_type & (PACKET_TYPE_RESPONSE | PACKET_TYPE_EVENT)) == 0){
			/* invalid type received */
			pr_err("ec_ipc: invalid packet TYPE received\n");
			continue;
		}
		/* receive size */
		if(sizeof(recv_size) != ec_ipc_serial_read(&recv_size, sizeof(recv_size), 100) ){
			/* receive packet timeout */
			pr_err("ec_ipc: timeout receive packet SIZE\n");
			continue;
		}

		recv_data_size = recv_size - (sizeof_PACKET_HEAD + sizeof_PACKET_FOOT);
		if(0 < recv_data_size) {
			recv_data = kzalloc(recv_data_size, GFP_KERNEL);
		}
		else{
			recv_data = &dummy_size0;
		}
		if(recv_data == NULL) break;

		do {
			/* receive frame num */
			if(sizeof(recv_frame_num) != ec_ipc_serial_read(&recv_frame_num, sizeof(recv_frame_num), 100) ){
				/* receive packet timeout */
				pr_err("ec_ipc: timeout receive packet FRAME NO.\n");
				break;
			}

			/* receive pid */
			if(sizeof(recv_pid) != ec_ipc_serial_read(&recv_pid, sizeof(recv_pid), 100) ){
				/* receive packet timeout */
				pr_err("ec_ipc: timeout receive packet PID\n");
				break;
			}

			/* receive cid */
			if(sizeof(recv_cid) != ec_ipc_serial_read(&recv_cid, sizeof(recv_cid), 100) ){
				/* receive packet timeout */
				pr_err("ec_ipc: timeout receive packet CID\n");
				break;
			}

			/* receive data */
			if(recv_data_size != ec_ipc_serial_read(recv_data, recv_data_size, 100) ){
				/* receive packet timeout */
				pr_err("ec_ipc: timeout receive packet PAYLOAD\n");
				break;
			}
			/* receive check sum */
			if(sizeof(recv_csum) != ec_ipc_serial_read(&recv_csum, sizeof(recv_csum), 100) ){
				/* receive packet timeout */
				pr_err("ec_ipc: timeout receive packet CSUM\n");
				break;
			}
			/* receive foot */
			if(sizeof(recv_foot) != ec_ipc_serial_read(&recv_foot, sizeof(recv_foot), 100) ){
				/* receive packet timeout */
				pr_err("ec_ipc: timeout receive packet FOOT\n");
				break;
			}
			if(recv_foot != PACKET_FOOT_SIGN){
				/* invalid foot received */
				pr_err("ec_ipc: invalid packet FOOT received\n");
				break;
			}

			/* check sum */
			calc_sum = recv_type + recv_size + recv_frame_num + recv_pid + recv_cid;
			for(i = 0; i < recv_data_size; i++) calc_sum += recv_data[i];
			calc_sum += recv_csum;

			if(calc_sum != 0) {
				/* invalid check sum */
				pr_err("ec_ipc: invalid packet CSUM received\n");
				break;
			}

			/* error bit */
			if((recv_type & (PACKET_TYPE_ERR_CSUM | PACKET_TYPE_ERR_FRAME | PACKET_TYPE_ERR_UNDEF_COM)) != 0){
				/* occur error bit in type */
				pr_err("ec_ipc: occur error bit in TYPE\n");
				if((recv_type & PACKET_TYPE_ERR_CSUM) != 0) pr_err("PACKET_TYPE_ERR_CSUM\n");
				if((recv_type & PACKET_TYPE_ERR_FRAME) != 0) pr_err("PACKET_TYPE_ERR_FRAME\n");
				if((recv_type & PACKET_TYPE_ERR_UNDEF_COM) != 0) pr_err("PACKET_TYPE_ERR_UNDEF_COM\n");
				break;
			}

			recv_packet_comp = 1;
		}while(0);

#ifdef DBGPRINT
		{
			int i;
			char pstr[256];
			snprintf(pstr, sizeof(pstr), "ec_ipc: %s read() %x,%x,%x,%x,%x,%x,%x,",
				((recv_packet_comp == 0)?"NG":"OK"),
				recv_head_1st,recv_head_2nd,recv_type,recv_size,recv_frame_num,recv_pid,recv_cid);
			for(i=0;i<recv_data_size;i++) snprintf(pstr+strlen(pstr), sizeof(pstr)-strlen(pstr), "%x,",recv_data[i]);
			snprintf(pstr+strlen(pstr), sizeof(pstr)-strlen(pstr), "%x,%x", recv_csum,recv_foot);
			pstr[sizeof(pstr)-1] = '\0';

			pr_info("%s\n", pstr);
		}
#endif

		if((recv_type & PACKET_TYPE_RESPONSE) != 0){
			if((recv_packet_comp == 0) ||
				(0 != ec_ipc_rx_response(recv_frame_num, recv_pid, recv_cid, recv_data, recv_data_size)) ){

				/* recv packet error, retry */
				LOCKED_STATEMENT(&request_list_mutex,
						struct request_t* tx_current = request_list_top();
						if(tx_current == NULL) break;

						tx_current->sending = 0;
					);
			}

			ec_ipc_tx_active = 1;
			wake_up_interruptible(&ec_ipc_tx_wait);
		}
		else if((recv_type & PACKET_TYPE_EVENT) != 0){
			if(recv_packet_comp != 0)
				ec_ipc_rx_event(recv_frame_num, recv_pid, recv_cid, recv_data, recv_data_size);
		}

		if(0 < recv_data_size) {
			kfree(recv_data);
		}
	}

	return;
}

static int ec_ipc_rx_response(uint8_t frame_num, uint8_t pid, uint8_t cid, uint8_t* data, uint8_t size)
{
	int ret = -1;
	void (*res_func)(const uint8_t*, int, void*) = NULL;
	void* private_data = NULL;

#ifdef DBGPRINT
	{
		int i;
		char pstr[256];
		snprintf(pstr, sizeof(pstr), "%s(len:%x, frm:%x, pid:%x, cid:%x): ", __func__, size, frame_num, pid, cid);
		for(i=0;i<size;i++) snprintf(pstr+strlen(pstr), sizeof(pstr)-strlen(pstr), "%x,",data[i]);
		pstr[sizeof(pstr)-1] = '\0';

		pr_info("%s\n", pstr);
	}
#endif

	if(!atomic_read(&ec_ipc_work_alive)) return ret;

	LOCKED_STATEMENT(&request_list_mutex,
			struct request_t* tx_current = request_list_top();
			if(tx_current == NULL) break;

			if((tx_current->frame_num == frame_num) &&
				(tx_current->pid == pid) &&
				(tx_current->cid == cid)){

				if(!(tx_current->done)) {
					res_func = tx_current->res_func;
					private_data = tx_current->private_data;

					/* call res_func only ones. */
					tx_current->res_func = NULL;
					tx_current->private_data = NULL;
				}

				tx_current->done = 1;
				ret = 0;
			}
			else {
				pr_warning("ec_ipc:missed response. f_num %x pid %x cid %x",
					frame_num, pid, cid);
			}
		);

	if(res_func != NULL) {
		(*res_func)(data, size, private_data);
	}

	return ret;
}
static void ec_ipc_rx_event(uint8_t frame_num, uint8_t pid, uint8_t cid, uint8_t* data, uint8_t size)
{
	struct event_t* event;
	void (*event_func)(const uint8_t*, int) = NULL;

#ifdef DBGPRINT
	{
		int i;
		char pstr[256];
		snprintf(pstr, sizeof(pstr), "%s(len:%x, frm:%x, pid:%x, cid:%x): ", __func__, size, frame_num, pid, cid);
		for(i=0;i<size;i++) snprintf(pstr+strlen(pstr), sizeof(pstr)-strlen(pstr), "%x,",data[i]);
		pstr[sizeof(pstr)-1] = '\0';

		pr_info("%s\n", pstr);
	}
#endif

	if(!atomic_read(&ec_ipc_work_alive)) return;

	LOCKED_STATEMENT(&event_list_mutex,
			list_for_each_entry(event, &event_list, list) {
				if(event->cid == cid) {
					event_func = event->event_func;
					break;
				}
			}
		);

	if(event_func != NULL) {
		(*event_func)(data, size);
	}
}

/*########################################################*/

static struct workqueue_struct* ec_ipc_tx_workqueue;
static struct work_struct ec_ipc_tx_work;
static struct workqueue_struct* ec_ipc_rx_workqueue;
static struct work_struct ec_ipc_rx_work;

static int __init ec_ipc_probe(struct nbx_ec_ipc_device* edev)
{
	int ret = 0;

	ec_ipc_tx_workqueue = NULL;
	ec_ipc_rx_workqueue = NULL;

	ec_ipc_serial_init();

	LOCKED_STATEMENT(&request_frame_num_mutex,
			ec_ipc_request_frame_num = 0;
		);

	ec_ipc_tx_active = 0;

	ec_ipc_tx_workqueue = create_singlethread_workqueue("ec_ipc_tx_thread");
	if(ec_ipc_tx_workqueue == NULL) {
		pr_err("ec_ipc:create_singlethread_workqueue() failed.\n");
		ret = -ENOMEM;
		goto error_exit;
	}

	ec_ipc_rx_workqueue = create_singlethread_workqueue("ec_ipc_rx_thread");
	if(ec_ipc_rx_workqueue == NULL) {
		pr_err("ec_ipc:create_singlethread_workqueue() failed.\n");
		ret = -ENOMEM;
		goto error_exit;
	}

	INIT_WORK(&ec_ipc_tx_work, ec_ipc_tx_thread);
	INIT_WORK(&ec_ipc_rx_work, ec_ipc_rx_thread);

	atomic_set(&request_enable, 1);
	atomic_set(&ec_ipc_work_alive, 1);
	queue_work(ec_ipc_tx_workqueue, &ec_ipc_tx_work);
	queue_work(ec_ipc_rx_workqueue, &ec_ipc_rx_work);

	atomic_set(&ec_int_updated, 1);
	ec_int_factor = 0;

	return 0;

error_exit:
	destroy_workqueue(ec_ipc_tx_workqueue);
	destroy_workqueue(ec_ipc_rx_workqueue);
	ec_ipc_tx_workqueue = NULL;
	ec_ipc_rx_workqueue = NULL;

	ec_ipc_serial_close();

	return ret;
}
static int ec_ipc_remove(struct nbx_ec_ipc_device* edev)
{
	struct event_t* del_event;
	struct event_t* n_event;

	atomic_set(&request_enable, 0);
	atomic_set(&ec_ipc_work_alive, 0);
	wake_up_interruptible(&ec_ipc_tx_wait);

	flush_work(&ec_ipc_tx_work);
	flush_work(&ec_ipc_rx_work);

	ec_ipc_serial_close();

	destroy_workqueue(ec_ipc_tx_workqueue);
	destroy_workqueue(ec_ipc_rx_workqueue);
	ec_ipc_tx_workqueue = NULL;
	ec_ipc_rx_workqueue = NULL;

	LOCKED_STATEMENT(&event_list_mutex,
			list_for_each_entry_safe(del_event, n_event, &event_list, list) {
				list_del(&(del_event->list));
				kfree(del_event);
			}
		);

	return 0;
}

static void ec_ipc_shutdown(struct nbx_ec_ipc_device* edev)
{
	atomic_set(&request_enable, 0);
	atomic_set(&ec_ipc_work_alive, 0);
	wake_up_interruptible(&ec_ipc_tx_wait);

	flush_work(&ec_ipc_tx_work);
	flush_work(&ec_ipc_rx_work);

	ec_ipc_serial_close();
}

#ifdef CONFIG_SUSPEND

extern int nbx_ec_suspend_suspend(void);
extern int nbx_ec_suspend_resume(void);

static int ec_ipc_suspend(struct nbx_ec_ipc_device* edev)
{
	/* send power state change (must send last) -> stop */
	nbx_ec_suspend_suspend();

	atomic_set(&request_enable, 0);
	atomic_set(&ec_ipc_work_alive, 0);
	wake_up_interruptible(&ec_ipc_tx_wait);

	flush_work(&ec_ipc_tx_work);
	flush_work(&ec_ipc_rx_work);

	ec_ipc_serial_close();

	return 0;
}

static int ec_ipc_resume(struct nbx_ec_ipc_device* edev)
{
	atomic_set(&request_enable, 1);
	atomic_set(&ec_ipc_work_alive, 1);
	queue_work(ec_ipc_tx_workqueue, &ec_ipc_tx_work);
	queue_work(ec_ipc_rx_workqueue, &ec_ipc_rx_work);

	/* async send power state change (must send first) */
	nbx_ec_suspend_resume();

	/* async start query EC INT factor. */
	ec_ipc_query_ec_int();

	return 0;
}

#else /* !CONFIG_SUSPEND */

#define ec_ipc_suspend NULL
#define ec_ipc_resume NULL

#endif /* CONFIG_SUSPEND */

static struct nbx_ec_ipc_driver ec_ipc_driver = {
	.probe    = ec_ipc_probe,
	.remove   = ec_ipc_remove,
	.shutdown = ec_ipc_shutdown,
	.suspend  = ec_ipc_suspend,
	.resume   = ec_ipc_resume,
	.drv   = {
		.name = "nbx_ec_ipc",
	},
};

static int __init ec_ipc_init(void)
{
	int ret;

	ret = nbx_ec_ipc_driver_register(&ec_ipc_driver);
	if(ret < 0) {
		pr_err("%s:nbx_ec_ipc_driver_register() failed, %d\n", __func__, ret);
		return ret;
	}

	return ret;
}
static void __exit ec_ipc_exit(void)
{
	nbx_ec_ipc_driver_unregister(&ec_ipc_driver);
}

module_init(ec_ipc_init);
module_exit(ec_ipc_exit);

MODULE_LICENSE("GPL");
