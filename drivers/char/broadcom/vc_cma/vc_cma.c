/*****************************************************************************
* Copyright 2012 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/dma-mapping.h>
#include <linux/dma-contiguous.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>


#include "vc_cma.h"

#include "vchiq_util.h"
#include "vchiq_connected.h"

#define DRIVER_NAME  "vc-cma"

#define LOG_DBG(fmt, ...) \
	if (vc_cma_debug) \
		printk(KERN_INFO fmt "\n", ##__VA_ARGS__)
#define LOG_ERR(fmt, ...) \
	printk(KERN_ERR fmt "\n", ##__VA_ARGS__)

#define VC_CMA_FOURCC VCHIQ_MAKE_FOURCC('C', 'M', 'A', ' ')
#define VC_CMA_VERSION 1

#define VC_CMA_CHUNK_ORDER 6  /* 256K */
#define VC_CMA_CHUNK_SIZE (4096 << VC_CMA_CHUNK_ORDER)
#define VC_CMA_MAX_PARAMS_PER_MSG \
	((VCHIQ_MAX_MSG_SIZE - sizeof(unsigned short))/sizeof(unsigned short))

#define PAGES_PER_CHUNK (VC_CMA_CHUNK_SIZE / PAGE_SIZE)

enum {
	VC_CMA_MSG_QUIT,
	VC_CMA_MSG_OPEN,
	VC_CMA_MSG_TICK,
	VC_CMA_MSG_ALLOC,     /* chunk count */
	VC_CMA_MSG_FREE,      /* chunk, chunk, ... */
	VC_CMA_MSG_ALLOCATED, /* chunk, chunk, ... */
	VC_CMA_MSG_REQUEST_ALLOC, /* chunk count */
	VC_CMA_MSG_REQUEST_FREE   /* chunk count */
};

struct cma_msg {
	unsigned short type;
	unsigned short params[VC_CMA_MAX_PARAMS_PER_MSG];
};

/* Device (/dev) related variables */
static dev_t         vc_cma_devnum;
static struct class *vc_cma_class;
static struct cdev   vc_cma_cdev;
static int           vc_cma_inited;
static int           vc_cma_debug;

/* Proc entry */
static struct proc_dir_entry *vc_cma_proc_entry;

phys_addr_t   vc_cma_base;
struct page  *vc_cma_base_page;
unsigned int  vc_cma_size;
EXPORT_SYMBOL(vc_cma_size);
unsigned int  vc_cma_initial;
unsigned int  vc_cma_chunks;
unsigned int  vc_cma_chunks_used;

static u64 vc_cma_dma_mask = DMA_BIT_MASK(32);
static struct platform_device vc_cma_device = {
	.name	= "vc-cma",
	.id	= 0,
	.dev	= {
		.dma_mask		= &vc_cma_dma_mask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	},
};

static VCHIQ_INSTANCE_T cma_instance;
static VCHIQ_SERVICE_HANDLE_T cma_service;
static VCHIU_QUEUE_T cma_msg_queue;
static struct task_struct *cma_worker;
static struct cma_msg cma_reply;

static VCHIQ_STATUS_T cma_service_callback(
	VCHIQ_REASON_T reason,
	VCHIQ_HEADER_T *header,
	VCHIQ_SERVICE_HANDLE_T service,
	void *bulk_userdata);
static void send_msg(
	unsigned short type,
	unsigned short param1,
	unsigned short param2);

static int __init early_vc_cma_mem(char *p)
{
	unsigned int new_size;
	printk(KERN_ERR "early_vc_cma_mem(%s)", p);
	vc_cma_size = memparse(p, &p);
	printk(KERN_ERR "  vc_cma_size = %x", vc_cma_size);
	vc_cma_initial = vc_cma_size;
	if (*p == '/') {
		vc_cma_size = memparse(p + 1, &p);
		printk(KERN_ERR "  vc_cma_size = %x", vc_cma_size);
	}
	if (*p == '@') {
		vc_cma_base = memparse(p + 1, &p);
		printk(KERN_ERR "  vc_cma_base = %x",
			(unsigned int)vc_cma_base);
	}

	new_size = (vc_cma_size - ((-vc_cma_base) & (VC_CMA_CHUNK_SIZE - 1)))
		& ~(VC_CMA_CHUNK_SIZE - 1);
	if (new_size > vc_cma_size)
		vc_cma_size = 0;
	vc_cma_initial = (vc_cma_initial + VC_CMA_CHUNK_SIZE - 1)
		& ~(VC_CMA_CHUNK_SIZE - 1);
	if (vc_cma_initial > vc_cma_size)
		vc_cma_initial = vc_cma_size;
	vc_cma_base = (vc_cma_base + VC_CMA_CHUNK_SIZE - 1)
		& ~(VC_CMA_CHUNK_SIZE - 1);

	printk(KERN_ERR " -> initial %x, size %x, base %x", vc_cma_initial,
		vc_cma_size, (unsigned int)vc_cma_base);

	return 0;
}

early_param("vc-cma-mem", early_vc_cma_mem);

void __init vc_cma_early_init(void)
{
	LOG_DBG("vc_cma_early_init - vc_cma_chunks = %d", vc_cma_chunks);
	if (vc_cma_size) {
		int rc = platform_device_register(&vc_cma_device);
		LOG_DBG("platform_device_register -> %d", rc);
	}
}

void __init vc_cma_reserve(void)
{
	/* if vc_cma_size is set, then declare vc CMA area of the same
	 * size from the end of memory
	 */
	if (vc_cma_size) {
		if (dma_declare_contiguous(&vc_cma_device.dev, vc_cma_size,
			vc_cma_base, 0) == 0) {
			vc_cma_chunks = vc_cma_size / VC_CMA_CHUNK_SIZE;
		} else {
			LOG_ERR("vc_cma: dma_declare_contiguous(%x,%x) failed",
				vc_cma_size, (unsigned int)vc_cma_base);
		}
	}
}

/****************************************************************************
*
*   vc_cma_open
*
***************************************************************************/

static int vc_cma_open(struct inode *inode, struct file *file)
{
	(void)inode;
	(void)file;

	LOG_DBG("%s: called file = 0x%p", __func__, file);

	return 0;
}

/****************************************************************************
*
*   vc_cma_release
*
***************************************************************************/

static int vc_cma_release(struct inode *inode, struct file *file)
{
	(void)inode;
	(void)file;

	LOG_DBG("%s: called file = 0x%p", __func__, file);

	return 0;
}

/****************************************************************************
*
*   vc_cma_ioctl
*
***************************************************************************/

static long vc_cma_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int rc = 0;

	(void)cmd;
	(void)arg;

	LOG_DBG("%s: called file = 0x%p", __func__, file);

	switch (cmd) {
		default: {
			return -ENOTTY;
		}
	}
	LOG_DBG("%s: file = 0x%p returning %d", __func__, file, rc);

	return rc;
}

/****************************************************************************
*
*   File Operations for the driver.
*
***************************************************************************/

static const struct file_operations vc_cma_fops = {
	.owner          = THIS_MODULE,
	.open           = vc_cma_open,
	.release        = vc_cma_release,
	.unlocked_ioctl = vc_cma_ioctl,
};

/****************************************************************************
*
*   vc_cma_proc_read
*
***************************************************************************/

static int vc_cma_proc_read(
	char *buf,
	char **start,
	off_t offset,
	int count,
	int *eof,
	void *data) {
	char *p = buf;

	(void)start;
	(void)count;
	(void)data;

	if (offset > 0) {
		*eof = 1;
		return 0;
	}

	p += sprintf(p, "Videocore CMA:\n");
	p += sprintf(p, "   Base       : %08x\n", (unsigned int)vc_cma_base);
	p += sprintf(p, "   Length     : %08x\n", vc_cma_size);
	p += sprintf(p, "   Initial    : %08x\n", vc_cma_initial);
	p += sprintf(p, "   Chunk size : %08x\n", VC_CMA_CHUNK_SIZE);
	p += sprintf(p, "   Chunks     : %d\n", (unsigned int)vc_cma_chunks);
	p += sprintf(p, "   Used       : %d\n",
		(unsigned int)vc_cma_chunks_used);

	*eof = 1;
	return p - buf;
}

/****************************************************************************
*
*   vc_cma_proc_write
*
***************************************************************************/

static int vc_cma_proc_write(
	struct file *file,
	const char __user *buffer,
	unsigned long count,
	void *data)
{
	int rc = -EFAULT;
	char input_str[20];

	memset(input_str, 0, sizeof(input_str));

	if (count > sizeof(input_str)) {
		LOG_ERR("%s: input string length too long", __func__);
		goto out;
	}

	if (copy_from_user(input_str, buffer, count - 1)) {
		LOG_ERR("%s: failed to get input string", __func__);
		goto out;
	}

#define ALLOC_STR "alloc"
#define FREE_STR "free"
#define DEBUG_STR "debug"
	if (strncmp(input_str, ALLOC_STR, strlen(ALLOC_STR)) == 0) {
		int size;
		char *p = input_str + strlen(ALLOC_STR);

		while (*p == ' ')
			p++;
		size = memparse(p, NULL);
		LOG_ERR("/proc/vc-cma: alloc %d", size);
		if (size)
			send_msg(VC_CMA_MSG_REQUEST_FREE,
				size / VC_CMA_CHUNK_SIZE, 0);
		else
			LOG_ERR("invalid size '%s'", p);
		rc = count;
	} else if (strncmp(input_str, FREE_STR,
			strlen(FREE_STR)) == 0) {
		int size;
		char *p = input_str + strlen(FREE_STR);

		while (*p == ' ')
			p++;
		size = memparse(p, NULL);
		LOG_ERR("/proc/vc-cma: free %d", size);
		if (size)
			send_msg(VC_CMA_MSG_REQUEST_ALLOC,
				size / VC_CMA_CHUNK_SIZE, 0);
		else
			LOG_ERR("invalid size '%s'", p);
		rc = count;
	} else if (strncmp(input_str, DEBUG_STR, strlen(DEBUG_STR)) == 0) {
		char *p = input_str + strlen(DEBUG_STR);
		while (*p == ' ')
			p++;
		if ((strcmp(p, "on") == 0) || (strcmp(p, "1") == 0))
			vc_cma_debug = 1;
		else if ((strcmp(p, "off") == 0) || (strcmp(p, "0") == 0))
			vc_cma_debug = 0;
		LOG_ERR("/proc/vc-cma: debug %s", vc_cma_debug ? "on" : "off");
		rc = count;
	}

out:
	return rc;
}

static VCHIQ_STATUS_T cma_service_callback(
	VCHIQ_REASON_T reason,
	VCHIQ_HEADER_T *header,
	VCHIQ_SERVICE_HANDLE_T service,
	void *bulk_userdata)
{
	switch (reason) {
	case VCHIQ_MESSAGE_AVAILABLE:
		vchiu_queue_push(&cma_msg_queue, header);
		break;
	case VCHIQ_SERVICE_CLOSED:
	LOG_DBG("CMA service closed");
		break;
	default:
		LOG_ERR("Unexpected CMA callback reason %d", reason);
		break;
	}
	return VCHIQ_SUCCESS;
}

static void send_msg(
	unsigned short type,
	unsigned short param1,
	unsigned short param2)
{
	unsigned short msg[] = { type, param1, param2 };
	VCHIQ_ELEMENT_T elem = { &msg, sizeof(msg) };
	VCHIQ_STATUS_T ret;
	vchiq_use_service(cma_service);
	ret = vchiq_queue_message(cma_service, &elem, 1);
	vchiq_release_service(cma_service);
	if (ret != VCHIQ_SUCCESS)
		LOG_ERR("vchiq_queue_message returned %x", ret);
}

static int cma_worker_proc(void *param)
{
	(void)param;

	while (1) {
		VCHIQ_HEADER_T *msg;
		struct cma_msg *cma_msg;
		msg = vchiu_queue_pop(&cma_msg_queue);
		cma_msg = (struct cma_msg *)msg->data;
		if (!msg)
			break;
		switch (cma_msg->type) {
		case VC_CMA_MSG_ALLOC: {
			int num_chunks, free_chunks;
			int i;
			num_chunks = cma_msg->params[0];
			free_chunks = vc_cma_chunks - vc_cma_chunks_used;
			LOG_DBG("CMA_MSG_ALLOC(%d chunks)", num_chunks);
			if (num_chunks > VC_CMA_MAX_PARAMS_PER_MSG) {
				LOG_ERR("CMA_MSG_ALLOC - chunk count (%d) "
					"exceeds VC_CMA_MAX_PARAMS_PER_MSG (%d)",
					num_chunks, VC_CMA_MAX_PARAMS_PER_MSG);
				num_chunks = VC_CMA_MAX_PARAMS_PER_MSG;
			}
			if (num_chunks > free_chunks) {
				LOG_ERR("CMA_MSG_ALLOC - chunk count (%d) "
					"exceeds free chunks (%d)",
					num_chunks, free_chunks);
				num_chunks = free_chunks;
			}

			for (i = 0; i < num_chunks; i++) {
				struct page *chunk;
				unsigned int chunk_num;
				chunk = dma_alloc_from_contiguous(
					&vc_cma_device.dev,
					PAGES_PER_CHUNK,
					VC_CMA_CHUNK_ORDER);
				if (!chunk)
					break;
				BUG_ON(((page_to_phys(chunk) - vc_cma_base) %
					VC_CMA_CHUNK_SIZE) != 0);
				chunk_num =
					(page_to_phys(chunk) - vc_cma_base) /
					VC_CMA_CHUNK_SIZE;
				BUG_ON(chunk_num >= vc_cma_chunks);
				cma_reply.params[i] = chunk_num;
				vc_cma_chunks_used++;
			}

			if (i < num_chunks) {
				LOG_ERR("%s: dma_alloc_from_contiguous failed "
					"for %x bytes (alloc %d of %d, %d free)",
					__func__, VC_CMA_CHUNK_SIZE, i,
					num_chunks,
					vc_cma_chunks - vc_cma_chunks_used);
				num_chunks = i;
			}

			LOG_DBG("CMA allocated %d chunks -> %d used",
				num_chunks, vc_cma_chunks_used);
			cma_reply.type = VC_CMA_MSG_ALLOCATED;
			{
				VCHIQ_ELEMENT_T elem = {
					&cma_reply,
					offsetof(struct cma_msg,
						params[num_chunks])
				};
				VCHIQ_STATUS_T ret;
				vchiq_use_service(cma_service);
				ret = vchiq_queue_message(cma_service,
					&elem, 1);
				vchiq_release_service(cma_service);
				if (ret != VCHIQ_SUCCESS)
					LOG_ERR("vchiq_queue_message return "
						"%x", ret);
			}
		}
		break;
		case VC_CMA_MSG_FREE: {
			int chunk_count =
				(msg->size - offsetof(struct cma_msg, params))/
				sizeof(cma_msg->params[0]);
			int i;
			BUG_ON(chunk_count <= 0);

			LOG_DBG("CMA_MSG_FREE(%d chunks - %x, ...)",
				chunk_count, cma_msg->params[0]);
			for (i = 0; i < chunk_count; i++) {
				int chunk_num = cma_msg->params[i];
				struct page *page = vc_cma_base_page +
					chunk_num * PAGES_PER_CHUNK;
				if (chunk_num >= vc_cma_chunks) {
					LOG_ERR("CMA_MSG_FREE - chunk %d of %d"
						" (value %x) exceeds maximum "
						"(%x)",
						i, chunk_count, chunk_num,
						vc_cma_chunks - 1);
					break;
				}

				if (!dma_release_from_contiguous(
					&vc_cma_device.dev,
					page, PAGES_PER_CHUNK)) {
					LOG_ERR("CMA_MSG_FREE - failed to "
						"release chunk %d (phys %x, "
						"page %x)",
						chunk_num, page_to_phys(page),
						(unsigned int)page);
				}
				vc_cma_chunks_used--;
			}
			LOG_DBG("CMA released %d chunks -> %d used",
				i, vc_cma_chunks_used);
			vchiq_release_message(cma_service, msg);
			msg = NULL;
		}
		break;
		default:
			LOG_ERR("unexpected msg type %d", cma_msg->type);
			break;
		}
		if (msg)
			vchiq_release_message(cma_service, msg);
	}

	LOG_DBG("quitting...");
	return 0;
}

static void vc_cma_connected_init(void)
{
	VCHIQ_SERVICE_PARAMS_T service_params;

	LOG_DBG("vc_cma_connected_init");

	if (!vchiu_queue_init(&cma_msg_queue, 16)) {
		LOG_ERR("could not create CMA msg queue");
		goto fail_queue;
	}

	if (vchiq_initialise(&cma_instance) != VCHIQ_SUCCESS)
		goto fail_vchiq_init;

	vchiq_connect(cma_instance);

	service_params.fourcc = VC_CMA_FOURCC;
	service_params.callback = cma_service_callback;
	service_params.userdata = NULL;
	service_params.version = VC_CMA_VERSION;
	service_params.version_min = VC_CMA_VERSION;

	if (vchiq_open_service(cma_instance, &service_params,
		&cma_service) != VCHIQ_SUCCESS) {
		LOG_ERR("failed to open service - already in use?");
		goto fail_vchiq_open;
	}

	vchiq_release_service(cma_service);

	cma_worker = kthread_create(cma_worker_proc, NULL, "cma_worker");
	if (!cma_worker) {
		LOG_ERR("could not create CMA worker thread");
		goto fail_worker;
	}
	set_user_nice(cma_worker, -20);
	wake_up_process(cma_worker);

	return;

fail_worker:
	vchiq_close_service(cma_service);
fail_vchiq_open:
	vchiq_shutdown(cma_instance);
fail_vchiq_init:
	vchiu_queue_delete(&cma_msg_queue);
fail_queue:
	return;
}


/****************************************************************************
*
*   vc_cma_connected_init
*
*   This function is called once the videocore has been connected.
*
***************************************************************************/

static int __init vc_cma_init(void)
{
	int rc = -EFAULT;
	struct device *dev;
	struct page *initial_pages = NULL;

	printk(KERN_INFO "vc-cma: Videocore CMA driver\n");
	printk(KERN_INFO "vc-cma: vc_cma_base      = 0x%08x\n",
			vc_cma_base);
	printk(KERN_INFO "vc-cma: vc_cma_size      = 0x%08x (%u MiB)\n",
			vc_cma_size, vc_cma_size / (1024 * 1024));
	printk(KERN_INFO "vc-cma: vc_cma_initial   = 0x%08x (%u MiB)\n",
			vc_cma_initial, vc_cma_initial / (1024 * 1024));

	vc_cma_base_page = phys_to_page(vc_cma_base);

	if (vc_cma_initial) {
		int i;
		vc_cma_chunks_used = vc_cma_initial / VC_CMA_CHUNK_SIZE;

		for (i = 0; i < vc_cma_chunks_used; i++) {
			struct page *chunk;
			chunk = dma_alloc_from_contiguous(&vc_cma_device.dev,
				PAGES_PER_CHUNK, VC_CMA_CHUNK_ORDER);
			if (!chunk)
				break;
			BUG_ON(((page_to_phys(chunk) - vc_cma_base) %
				 VC_CMA_CHUNK_SIZE) != 0);
		}
		if (i != vc_cma_chunks_used) {
			/* Don't clean up - the system is in a bad shape, but
			   at least this way there is a chance VideoCore may
			   not crash when its relocatable heap pages are
			   simultaneously used by the ARM. */
			LOG_ERR("%s: dma_alloc_from_contiguous failed (%d "
				"bytes, allocation %d of %d)",
				__func__, VC_CMA_CHUNK_SIZE, i,
				vc_cma_chunks_used);
			goto out_err;
		}

		vchiq_add_connected_callback(vc_cma_connected_init);
	}

	rc = alloc_chrdev_region(&vc_cma_devnum, 0, 1, DRIVER_NAME);
	if (rc < 0) {
		LOG_ERR("%s: alloc_chrdev_region failed (rc=%d)", __func__,
			rc);
		goto out_release;
	}

	cdev_init(&vc_cma_cdev, &vc_cma_fops);
	rc = cdev_add(&vc_cma_cdev, vc_cma_devnum, 1);
	if (rc != 0) {
		LOG_ERR("%s: cdev_add failed (rc=%d)", __func__, rc);
		goto out_unregister;
	}

	vc_cma_class = class_create(THIS_MODULE, DRIVER_NAME);
	if (IS_ERR(vc_cma_class)) {
		rc = PTR_ERR(vc_cma_class);
		LOG_ERR("%s: class_create failed (rc=%d)", __func__, rc);
		goto out_cdev_del;
	}

	dev = device_create(vc_cma_class, NULL, vc_cma_devnum, NULL,
		DRIVER_NAME);
	if (IS_ERR(dev)) {
		rc = PTR_ERR(dev);
		LOG_ERR("%s: device_create failed (rc=%d)", __func__, rc);
		goto out_class_destroy;
	}

	vc_cma_proc_entry = create_proc_entry(DRIVER_NAME, 0444, NULL);
	if (vc_cma_proc_entry == NULL) {
		rc = -EFAULT;
		LOG_ERR("%s: create_proc_entry failed", __func__);
		goto out_device_destroy;
	}
	vc_cma_proc_entry->read_proc = vc_cma_proc_read;
	vc_cma_proc_entry->write_proc = vc_cma_proc_write;

	vc_cma_inited = 1;
	return 0;

out_device_destroy:
	device_destroy(vc_cma_class, vc_cma_devnum);

out_class_destroy:
	class_destroy(vc_cma_class);
	vc_cma_class = NULL;

out_cdev_del:
	cdev_del(&vc_cma_cdev);

out_unregister:
	unregister_chrdev_region(vc_cma_devnum, 1);

out_release:
	if (initial_pages)
		dma_release_from_contiguous(&vc_cma_device.dev, initial_pages,
			vc_cma_initial >> PAGE_SHIFT);

out_err:
	return -1;
}

/****************************************************************************
*
*   vc_cma_exit
*
***************************************************************************/

static void __exit vc_cma_exit(void)
{
	LOG_DBG("%s: called", __func__);

	if (vc_cma_inited) {
		remove_proc_entry(vc_cma_proc_entry->name, NULL);
		device_destroy(vc_cma_class, vc_cma_devnum);
		class_destroy(vc_cma_class);
		cdev_del(&vc_cma_cdev);
		unregister_chrdev_region(vc_cma_devnum, 1);
	}
}

module_init(vc_cma_init);
module_exit(vc_cma_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Broadcom Corporation");
