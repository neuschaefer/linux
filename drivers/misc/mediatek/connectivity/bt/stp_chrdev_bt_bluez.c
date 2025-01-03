/*
* Copyright (C) 2016 MediaTek Inc.
*
* This program is free software: you can redistribute it and/or modify it under the terms of the
* GNU General Public License version 2 as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.
* If not, see <http://www.gnu.org/licenses/>.
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <linux/uaccess.h>
#include <linux/fcntl.h>
#include <linux/poll.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/printk.h>

#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>

#include "wmt_exp.h"
#include "stp_exp.h"

MODULE_LICENSE("Dual BSD/GPL");

#define BT_DRIVER_NAME "MTK BT"

#define BTMTK_LOG_LEVEL_ERROR       1
#define BTMTK_LOG_LEVEL_WARNING     2
#define BTMTK_LOG_LEVEL_INFO        3
#define BTMTK_LOG_LEVEL_DEBUG       4

unsigned char btmtk_log_lvl = BTMTK_LOG_LEVEL_INFO;

#define BTMTK_ERR(fmt, ...)     \
	do {if (btmtk_log_lvl >= BTMTK_LOG_LEVEL_ERROR) pr_warn("btmtk_err: "fmt"\n", ##__VA_ARGS__); } while (0)
#define BTMTK_WARN(fmt, ...)    \
	do {if (btmtk_log_lvl >= BTMTK_LOG_LEVEL_WARNING) pr_warn("btmtk_warn: "fmt"\n", ##__VA_ARGS__); } while (0)
#define BTMTK_INFO(fmt, ...)    \
	do {if (btmtk_log_lvl >= BTMTK_LOG_LEVEL_INFO) pr_warn("btmtk_info: "fmt"\n", ##__VA_ARGS__); } while (0)
#define BTMTK_DBG(fmt, ...)     \
	do {if (btmtk_log_lvl >= BTMTK_LOG_LEVEL_DEBUG) pr_warn("btmtk_debug: "fmt"\n", ##__VA_ARGS__); } while (0)

struct btmtk_thread {
	struct task_struct *task;
	wait_queue_head_t wait_q;
	void *priv;
};

struct btmtk_device {
	void *card;
	struct hci_dev *hcidev;

	unsigned char dev_type;

	unsigned char tx_dnld_rdy;

	unsigned char psmode;
	unsigned char pscmd;
	unsigned char hsmode;
	unsigned char hscmd;

	/* Low byte is gap, high byte is GPIO */
	unsigned short gpio_gap;

	unsigned char hscfgcmd;
	unsigned char sendcmdflag;
};

struct btmtk_adapter {
	void *hw_regs_buf;
	unsigned char *hw_regs;
	unsigned int int_count;
	struct sk_buff_head tx_queue;
	struct sk_buff_head fops_queue;
	struct sk_buff_head fwlog_fops_queue;
	struct sk_buff_head fwlog_tx_queue;
	unsigned char fops_mode;
	unsigned char psmode;
	unsigned char ps_state;
	unsigned char hs_state;
	unsigned char wakeup_tries;
	wait_queue_head_t cmd_wait_q;
	wait_queue_head_t event_hs_wait_q;
	unsigned char cmd_complete;
	bool is_suspended;
};

struct btmtk_private {
	struct btmtk_device btmtk_dev;
	struct btmtk_adapter *adapter;
	struct btmtk_thread main_thread;
	int (*hw_host_to_card)(struct sk_buff *skb);
	int (*hci_close)(void);
	int (*sdio_download_fw)(void);

	int (*hw_set_own_back)(int owntype);
	//int (*hw_wakeup_firmware)(struct btmtk_private *priv,int owntype);
	int (*hw_process_int_status)(struct btmtk_private *priv);
	void (*firmware_dump)(struct btmtk_private *priv);
	spinlock_t driver_lock;         /* spinlock used by driver */
#ifdef CONFIG_DEBUG_FS
	void *debugfs_data;
#endif
	bool surprise_removed;
};

#define VERSION "1.0"

#define BT_BUFFER_SIZE              2048

static struct btmtk_private *btmtk_priv;
static unsigned char *txbuf = NULL;
static unsigned char *rxbuf = NULL;
static unsigned int rx_length = 0;
static int init_flag = 0;

#define PRINT_BUF(buf, len) \
do { \
	int i = 0; \
	char str[128]; \
	char *p_str; \
	for (i = 0; i < len; i += 16) { \
		p_str = str; \
		if ((i + 16) <= len) { \
			p_str += sprintf(p_str, \
				"%s: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n", \
				__func__, \
				buf[i], buf[i+1], buf[i+2], buf[i+3], \
				buf[i+4], buf[i+5], buf[i+6], buf[i+7], \
				buf[i+8], buf[i+9], buf[i+10], buf[i+11], \
				buf[i+12], buf[i+13], buf[i+14], buf[i+15]); \
		} else { \
			p_str += sprintf(p_str, "%s:", __func__); \
			for(; i < len; i++) \
				p_str += sprintf(p_str, " %02X", buf[i]); \
			p_str += sprintf(p_str, "\n"); \
		} \
		BTMTK_DBG("%s", str); \
	} \
}while(0)

/*
 * Reset flag for whole chip reset scenario, to indicate reset status:
 *   0 - normal, no whole chip reset occurs
 *   1 - reset start
 *   2 - reset end, have not sent Hardware Error event yet
 *   3 - reset end, already sent Hardware Error event
 */
//static DECLARE_WAIT_QUEUE_HEAD(BT_wq);
static volatile UINT32 rstflag;
static INT32 flag;
//static UINT8 HCI_EVT_HW_ERROR[] = {0x04, 0x10, 0x01, 0x00};

/*******************************************************************
 * WHOLE CHIP RESET message handler
 *******************************************************************
*/
static VOID bt_cdev_rst_cb(ENUM_WMTDRV_TYPE_T src,
				ENUM_WMTDRV_TYPE_T dst,
				ENUM_WMTMSG_TYPE_T type,
				PVOID buf, UINT32 sz)
{
	ENUM_WMTRSTMSG_TYPE_T rst_msg;

	if (sz > sizeof(ENUM_WMTRSTMSG_TYPE_T)) {
		BTMTK_WARN("Invalid message format!\n");
		return;
	}

	memcpy((PINT8)&rst_msg, (PINT8)buf, sz);
	BTMTK_DBG("src = %d, dst = %d, type = %d, buf = 0x%x sz = %d, max = %d\n",
			src, dst, type, rst_msg, sz, WMTRSTMSG_RESET_MAX);
	if ((src == WMTDRV_TYPE_WMT) && (dst == WMTDRV_TYPE_BT) && (type == WMTMSG_TYPE_RESET)) {
		switch (rst_msg) {
		case WMTRSTMSG_RESET_START:
			BTMTK_INFO("Whole chip reset start!\n");
			rstflag = 1;
			break;

		case WMTRSTMSG_RESET_END:
		case WMTRSTMSG_RESET_END_FAIL:
			if (rst_msg == WMTRSTMSG_RESET_END)
				BTMTK_INFO("Whole chip reset end!\n");
			else
				BTMTK_INFO("Whole chip reset fail!\n");
			rstflag = 2;
			flag = 1;
			break;

		default:
			break;
		}
	}
}

#define CFG_SUPPORT_NVRAM 1
#if CFG_SUPPORT_NVRAM
#define BT_NVRAM_FILE_NAME   "/data/nvram/APCFG/APRDEB/BT_Addr"
// the record structure define of bt nvram file
typedef struct
{
	unsigned char addr[6];            // BT address
	unsigned char Voice[2];           // Voice setting for SCO connection
	unsigned char Codec[4];           // PCM codec setting
	unsigned char Radio[6];           // RF configuration
	unsigned char Sleep[7];           // Sleep mode configuration
	unsigned char BtFTR[2];           // Other feature setting
	unsigned char TxPWOffset[3];      // TX power channel offset compensation
	unsigned char CoexAdjust[6];      // BT/WIFI coexistence performance adjustment
}ap_nvram_btradio_struct;


static ap_nvram_btradio_struct stBtDefault =
{
	{0x00, 0x00, 0x46, 0x81, 0x67, 0x01},
	{0x60, 0x00}, //not used
#if defined(MTK_MERGE_INTERFACE_SUPPORT)
	{0x63, 0x10, 0x00, 0x00},
#elif defined(__MTK_BT_I2S_SUPPORT__)
	{0x03, 0x10, 0x00, 0x02},
#else
	{0x23, 0x10, 0x00, 0x00},
#endif
	{0x07, 0x80, 0x00, 0x06, 0x05, 0x07},
	{0x03, 0x40, 0x1F, 0x40, 0x1F, 0x00, 0x04},
	{0x80, 0x00}, //not used
	{0xFF, 0xFF, 0xFF},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

static bool cfg_flag;
static bool rcv_flag;
static char opcode[2];
static unsigned char rx_evt[16];

typedef int (*HCI_CMD_FUNC_T)(ap_nvram_btradio_struct *bt_nvm);
typedef struct {
	HCI_CMD_FUNC_T command_func;
}HCI_SEQ_T;

static int btmtk_fw_cfg_set_check(char *cmd, int cmd_len,
						char *event, int event_len)
{
	int retrytime = 60;
	char str[128];
	char *p_str;
	int i;

	PRINT_BUF(cmd, (cmd_len < 16 ? cmd_len : 16));

	if (mtk_wcn_stp_send_data(cmd, cmd_len, BT_TASK_INDX) < 0) {
		memset(str, 0, sizeof(str));
		p_str = str;
		p_str += sprintf(p_str, "%02X", cmd[0]);
		for (i = 1; i < cmd_len; i++)
			p_str += sprintf(p_str, " %02X", cmd[i]);
		BTMTK_ERR("%s: send fail(%s)\n", __func__, str);
		return -EIO;
	}

	if (event && (event_len != 0)) {
		rcv_flag = false;
		opcode[0] = cmd[1];
		opcode[1] = cmd[2];

		do {
			if (rcv_flag == true)
				break;

			if (retrytime == 0) {
				BTMTK_ERR("%s: recv %02X%02X fail\n", __func__,
						cmd[2], cmd[1]);
				return -EIO;
			}

			if (retrytime < 40)
				BTMTK_WARN("%s: retry over 2s, retrytime %d\n",
					__func__, retrytime);
			retrytime--;
			msleep(100);
		} while(1);

		if (memcmp(rx_evt, event, event_len) != 0) {
	 		memset(str, 0, sizeof(str));
			p_str = str;
			p_str += sprintf(p_str, "%02X", rx_evt[0]);
			for (i = 1; i < event_len; i++)
				p_str += sprintf(p_str, " %02X", rx_evt[i]);
			BTMTK_ERR("%s: check %02X%02X fail(%s)\n", __func__,
					cmd[2], cmd[1],
					str);
			return -EIO;
		}
	}
	return 0;
}

#ifdef BD_ADDR_AUTOGEN
static void GetRandomValue(u8 string[6])
{
	int iRandom = 0;

	BTMTK_INFO("Enable random generation\n");

	/* first random */
	get_random_bytes(&iRandom, sizeof(int));
	BTMTK_INFO("iRandom = [%d]", iRandom);
	string[0] = (((iRandom>>24|iRandom>>16) & (0xFE)) | (0x02)); /* Must use private bit(1) and no BCMC bit(0) */

	/* second random */
	get_random_bytes(&iRandom, sizeof(int));
	BTMTK_INFO("iRandom = [%d]", iRandom);
	string[1] = ((iRandom>>8) & 0xFF);

	/* third random */
	get_random_bytes(&iRandom, sizeof(int));
	BTMTK_INFO("iRandom = [%d]", iRandom);
	string[5] = (iRandom & 0xFF);

	return;
}
#endif

static int nvram_write(char *filename, char *buf, ssize_t len, int offset)
{
	struct file *fd;
	int retLen = -1;

	mm_segment_t old_fs = get_fs();

	set_fs(KERNEL_DS);

	fd = filp_open(filename, O_WRONLY, 0644);

	if (IS_ERR(fd)) {
		BTMTK_INFO("[nvram_write] : failed to open!!");
		return -1;
	}

	fd->f_pos = offset;
	retLen = vfs_write(fd, buf, len, &fd->f_pos);
	filp_close(fd, NULL);

	set_fs(old_fs);

	return retLen;
}

int btmtk_set_cfg_to_nvram(UINT8 *buf, ssize_t len)
{
	return nvram_write(BT_NVRAM_FILE_NAME, buf, len, 0);
}

static int btmtk_set_local_bd_addr(ap_nvram_btradio_struct *bt_nvm)
{
	char cmd[] = {0x01, 0x1A, 0xFC, 0x06,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	char event[] = {0x04, 0x0E, 0x04,
			0x01, 0x1A, 0xFC, 0x00};

	cmd[4] = bt_nvm->addr[5];
	cmd[5] = bt_nvm->addr[4];
	cmd[6] = bt_nvm->addr[3];
	cmd[7] = bt_nvm->addr[2];
	cmd[8] = bt_nvm->addr[1];
	cmd[9] = bt_nvm->addr[0];

	return btmtk_fw_cfg_set_check(cmd, sizeof(cmd), event, sizeof(event));
}

static int btmtk_set_radio(ap_nvram_btradio_struct *bt_nvm)
{
	char cmd[] = {0x01, 0x79, 0xFC, 0x06,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	char event[] = {0x04, 0x0E, 0x04,
			0x01, 0x79, 0xFC, 0x00};

	cmd[4] = bt_nvm->Radio[0];
	cmd[5] = bt_nvm->Radio[1];
	cmd[6] = bt_nvm->Radio[2];
	cmd[7] = bt_nvm->Radio[3];
	cmd[8] = bt_nvm->Radio[4];
	cmd[9] = bt_nvm->Radio[5];

	return btmtk_fw_cfg_set_check(cmd, sizeof(cmd), event, sizeof(event));
}

static int btmtk_set_tx_power_offset(ap_nvram_btradio_struct *bt_nvm)
{
	char cmd[] = {0x01, 0x93, 0xFC, 0x03,
			0x00, 0x00, 0x00};
	char event[] = {0x04, 0x0E, 0x04,
			0x01, 0x93, 0xFC, 0x00};

	cmd[4] = bt_nvm->TxPWOffset[0];
	cmd[5] = bt_nvm->TxPWOffset[1];
	cmd[6] = bt_nvm->TxPWOffset[2];

	return btmtk_fw_cfg_set_check(cmd, sizeof(cmd), event, sizeof(event));
}

static int btmtk_set_sleep_timeout(ap_nvram_btradio_struct *bt_nvm)
{
	char cmd[] = {0x01, 0x7A, 0xFC, 0x07,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	char event[] = {0x04, 0x0E, 0x04,
			0x01, 0x7A, 0xFC, 0x00};

	cmd[4] = bt_nvm->Sleep[0];
	cmd[5] = bt_nvm->Sleep[1];
	cmd[6] = bt_nvm->Sleep[2];
	cmd[7] = bt_nvm->Sleep[3];
	cmd[8] = bt_nvm->Sleep[4];
	cmd[9] = bt_nvm->Sleep[5];
	cmd[10] = bt_nvm->Sleep[6];

	return btmtk_fw_cfg_set_check(cmd, sizeof(cmd), event, sizeof(event));
}

static int btmtk_reset(ap_nvram_btradio_struct *bt_nvm)
{
	char cmd[] = {0x01, 0x03, 0x0C, 0x00};
	char event[] = {0x04, 0x0E, 0x04,
			0x01, 0x03, 0x0C, 0x00};

	return btmtk_fw_cfg_set_check(cmd, sizeof(cmd), event, sizeof(event));
}

HCI_SEQ_T bt_init_preload_script[] =
{
    {  btmtk_set_local_bd_addr       }, /*0xFC1A*/
    {  btmtk_set_radio               }, /*0xFC79*/
    {  btmtk_set_tx_power_offset     }, /*0xFC93*/
    {  btmtk_set_sleep_timeout       }, /*0xFC7A*/
    {  btmtk_reset                   }, /*0x0C03*/
    {  0  },
};

/*----------------------------------------------------------------------------*/
/*!
* \brief Utility function for reading data from files on NVRAM-FS
*
* \param[in]
*           filename
*           len
*           offset
* \param[out]
*           buf
* \return
*           actual length of data being read
*/
/*----------------------------------------------------------------------------*/
static int nvram_read(char *filename, char *buf, ssize_t len, int offset)
{
	struct file *fd;
	int retLen = -1;

	mm_segment_t old_fs = get_fs();

	set_fs(KERNEL_DS);

	fd = filp_open(filename, O_RDONLY, 0644);

	if (IS_ERR(fd)) {
		BTMTK_INFO("[nvram_read] : failed to open!!");
		return -1;
	}
#if 0
	do {
		if ((fd->f_op == NULL) || (fd->f_op->read == NULL)) {
			BTMTK_INFO("[nvram_read] : file can not be read!!\n");
			break;
		}

		if (fd->f_pos != offset) {
			if (fd->f_op->llseek) {
				if (fd->f_op->llseek(fd, offset, 0) != offset) {
					BTMTK_INFO("[nvram_read] : failed to seek!!\n");
					break;
				}
			} else {
				fd->f_pos = offset;
			}
		}

		retLen = fd->f_op->read(fd, buf, len, &fd->f_pos);

	} while (false);
#else
	fd->f_pos = offset;
	retLen = vfs_read(fd, buf, len, &fd->f_pos);
#endif
	filp_close(fd, NULL);

	set_fs(old_fs);

	return retLen;
}

int btmtk_get_cfg_from_nvram(UINT8 *buf, ssize_t len)
{
	return nvram_read(BT_NVRAM_FILE_NAME, buf, len, 0);
}

void btmtk_fw_cfg(void)
{
	ap_nvram_btradio_struct bt_nvm;
	int i = 0;
#ifdef BD_ADDR_AUTOGEN
	UINT8 ucZeroAddr[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif

	if (btmtk_get_cfg_from_nvram((UINT8 *)&bt_nvm, sizeof(bt_nvm)) <= 0) {
		BTMTK_ERR("%s: get configuration failed!\n", __func__);
		return;
	}
	BTMTK_INFO("[BDAddr %02x-%02x-%02x-%02x-%02x-%02x]",
		bt_nvm.addr[0], bt_nvm.addr[1], bt_nvm.addr[2], bt_nvm.addr[3], bt_nvm.addr[4], bt_nvm.addr[5]);
	BTMTK_INFO("[Voice %02x %02x]",
		bt_nvm.Voice[0], bt_nvm.Voice[1]);
	BTMTK_INFO("[Codec %02x %02x %02x %02x]",
		bt_nvm.Codec[0], bt_nvm.Codec[1], bt_nvm.Codec[2], bt_nvm.Codec[3]);
	BTMTK_INFO("[Radio %02x %02x %02x %02x %02x %02x]",
		bt_nvm.Radio[0], bt_nvm.Radio[1], bt_nvm.Radio[2], bt_nvm.Radio[3], bt_nvm.Radio[4], bt_nvm.Radio[5]);
	BTMTK_INFO("[Sleep %02x %02x %02x %02x %02x %02x %02x]",
		bt_nvm.Sleep[0], bt_nvm.Sleep[1], bt_nvm.Sleep[2], bt_nvm.Sleep[3], bt_nvm.Sleep[4], bt_nvm.Sleep[5], bt_nvm.Sleep[6]);
	BTMTK_INFO("[BtFTR %02x %02x]",
		bt_nvm.BtFTR[0], bt_nvm.BtFTR[1]);
	BTMTK_INFO("[TxPWOffset %02x %02x %02x]",
		bt_nvm.TxPWOffset[0], bt_nvm.TxPWOffset[1], bt_nvm.TxPWOffset[2]);
	BTMTK_INFO("[CoexAdjust %02x %02x %02x %02x %02x %02x]",
		bt_nvm.CoexAdjust[0], bt_nvm.CoexAdjust[1], bt_nvm.CoexAdjust[2], bt_nvm.CoexAdjust[3], bt_nvm.CoexAdjust[4], bt_nvm.CoexAdjust[5]);

	cfg_flag = true;

#ifdef BD_ADDR_AUTOGEN
	if ((0 == memcmp(bt_nvm.addr, stBtDefault.addr, 6)) ||
		(0 == memcmp(bt_nvm.addr, ucZeroAddr, 6))) {
		GetRandomValue(bt_nvm.addr);
		if (btmtk_set_cfg_to_nvram((UINT8 *)&bt_nvm, sizeof(bt_nvm)) <= 0) {
	                BTMTK_ERR("%s: set configuration failed!\n", __func__);
	                return;
		}
#if 0
		if (btmtk_get_cfg_from_nvram((UINT8 *)&bt_nvm, sizeof(bt_nvm)) <= 0) {
			BTMTK_ERR("%s: get configuration failed!\n", __func__);
			return;
		}
		BTMTK_DBG("after read");
		BTMTK_DBG("[BDAddr %02x-%02x-%02x-%02x-%02x-%02x]",
				bt_nvm.addr[0], bt_nvm.addr[1], bt_nvm.addr[2], bt_nvm.addr[3], bt_nvm.addr[4], bt_nvm.addr[5]);
		BTMTK_DBG("[Voice %02x %02x]",
				bt_nvm.Voice[0], bt_nvm.Voice[1]);
		BTMTK_DBG("[Codec %02x %02x %02x %02x]",
				bt_nvm.Codec[0], bt_nvm.Codec[1], bt_nvm.Codec[2], bt_nvm.Codec[3]);
		BTMTK_DBG("[Radio %02x %02x %02x %02x %02x %02x]",
				bt_nvm.Radio[0], bt_nvm.Radio[1], bt_nvm.Radio[2], bt_nvm.Radio[3], bt_nvm.Radio[4], bt_nvm.Radio[5]);
		BTMTK_DBG("[Sleep %02x %02x %02x %02x %02x %02x %02x]",
				bt_nvm.Sleep[0], bt_nvm.Sleep[1], bt_nvm.Sleep[2], bt_nvm.Sleep[3], bt_nvm.Sleep[4], bt_nvm.Sleep[5], bt_nvm.Sleep[6]);
		BTMTK_DBG("[BtFTR %02x %02x]",
				bt_nvm.BtFTR[0], bt_nvm.BtFTR[1]);
		BTMTK_DBG("[TxPWOffset %02x %02x %02x]",
				bt_nvm.TxPWOffset[0], bt_nvm.TxPWOffset[1], bt_nvm.TxPWOffset[2]);
		BTMTK_DBG("[CoexAdjust %02x %02x %02x %02x %02x %02x]",
				bt_nvm.CoexAdjust[0], bt_nvm.CoexAdjust[1], bt_nvm.CoexAdjust[2], bt_nvm.CoexAdjust[3], bt_nvm.CoexAdjust[4], bt_nvm.CoexAdjust[5]);
#endif
	}
#endif /* BD_ADDR_AUTOGEN */

	while(bt_init_preload_script[i].command_func) {
		if (bt_init_preload_script[i].command_func(&bt_nvm) < 0)
			BTMTK_ERR("%s: set fw failed(%d)!", __func__, i);
		i++;
	}

	cfg_flag = false;
}
#endif

static int btmtk_sdio_host_to_card(struct sk_buff *skb)
{
	int ret = 0;
	int len = 0;

	if (!skb) {
		BTMTK_WARN("%s skb is NULL return -EINVAL", __func__);
		return -EINVAL;
	}

	if (!skb->data) {
		BTMTK_WARN("%s skb->data is NULL return -EINVAL", __func__);
		return -EINVAL;
	}

	if (!skb->len || (skb->len > BT_BUFFER_SIZE)) {
		BTMTK_WARN("%s Tx Error: Bad skb length %d : %d", __func__,
			skb->len, BT_BUFFER_SIZE);
		return -EINVAL;
	}

	txbuf[0] = bt_cb(skb)->pkt_type;
	memcpy(&txbuf[1], &skb->data[0], skb->len);
	len = skb->len + 1;
	kfree_skb(skb);
	PRINT_BUF(txbuf, (len < 16 ? len : 16));

	ret = mtk_wcn_stp_send_data(txbuf, len, BT_TASK_INDX);

	return ret;
}

static int btmtk_sdio_card_to_host(struct btmtk_private *priv)
{
	struct sk_buff *skb = NULL;
	struct hci_dev *hdev;
	unsigned char *buf;
	int type;
	int buf_len, copy_len, hdr_len, payload_len;
	int err = 0;

	PRINT_BUF(rxbuf, (rx_length < 16 ? rx_length : 16));

	hdev = priv->btmtk_dev.hcidev;

	type = rxbuf[0];
	buf = rxbuf + 1;
	buf_len = rx_length - 1;

	switch (type) {
	case HCI_ACLDATA_PKT:
		hdr_len = HCI_ACL_HDR_SIZE;
		payload_len = rxbuf[3] | (rxbuf[4] << 8);
		break;
	case HCI_SCODATA_PKT:
		hdr_len = HCI_SCO_HDR_SIZE;
		payload_len = rxbuf[3];
		break;
	case HCI_EVENT_PKT:
		hdr_len = HCI_EVENT_HDR_SIZE;
		payload_len = rxbuf[2];
		if (rxbuf[4] == 0x04 && rxbuf[5] == 0x10 && rxbuf[6] != 0) {
			BTMTK_INFO("%s opcode 0x1004, status 0x%x -> 0", __func__, rxbuf[6]);
			rxbuf[6] = 0;
		}
		break;
	default:
		BTMTK_WARN("%s Unknown packet type:%d", __func__, type);
		err = -1;
		goto exit;
	}

	while (buf_len) {
		if (!skb) {
			skb = bt_skb_alloc(HCI_MAX_FRAME_SIZE, GFP_ATOMIC);
			if (!skb) {
				err = -ENOMEM;
				break;
			}
			bt_cb(skb)->pkt_type = type;
			bt_cb(skb)->expect = hdr_len;
		}

		copy_len = min_t(int, bt_cb(skb)->expect, buf_len);
		memcpy(skb_put(skb, copy_len), buf, copy_len);

		buf += copy_len;
		buf_len -= copy_len;
		bt_cb(skb)->expect -= copy_len;

		if (skb->len == hdr_len) {
			/* Complete header */
			bt_cb(skb)->expect = payload_len;

			if (skb_tailroom(skb) < bt_cb(skb)->expect) {
				kfree_skb(skb);
				skb = NULL;
				err = -EILSEQ;
				break;
			}
		}

		if (bt_cb(skb)->expect == 0) {
			/* Complete frame */
			hci_recv_frame(hdev, skb);
			skb = NULL;
		}
	}

exit:
	if (err) {
		BTMTK_DBG("%s fail free skb\n", __func__);
		hdev->stat.err_rx++;
		if (skb)
			kfree_skb(skb);
	}

	return err;
}

static VOID btmtk_recv_cb(const PUINT8 data, INT32 size)
{
        if (size) {
		memcpy(rxbuf, data, size);
		rx_length = size;

#if CFG_SUPPORT_NVRAM
		if (rcv_flag == false && opcode[0] == rxbuf[4] && opcode[1] == rxbuf[5]) {
			PRINT_BUF(rxbuf, (rx_length < 16 ? rx_length : 16));
			memcpy(rx_evt, rxbuf, (rx_length < 16 ? rx_length : 16));
			rcv_flag = true;
		} else {
#endif
	        btmtk_sdio_card_to_host(btmtk_priv);
#if CFG_SUPPORT_NVRAM
		}
#endif
	}

	return;
}

static int btmtk_open(struct hci_dev *hdev)
{
	/* Turn on BT */
#if 0
	if (mtk_wcn_wmt_func_on(WMTDRV_TYPE_BT) == MTK_WCN_BOOL_FALSE) {
		BTMTK_WARN("WMT turn on BT fail!\n");
		return -EIO;
	}
#else
	while (mtk_wcn_wmt_func_on(WMTDRV_TYPE_BT) == MTK_WCN_BOOL_FALSE) {
		int cnt = 0;
		BTMTK_WARN("WMT turn on BT fail!, retry %d\n", cnt);
		cnt++;
		msleep(1000);
	}
#endif

	BTMTK_INFO("WMT turn on BT OK!\n");
	rstflag = 0;

	if (mtk_wcn_stp_is_ready()) {

		mtk_wcn_stp_set_bluez(1);
		mtk_wcn_stp_register_if_rx(btmtk_recv_cb);
		BTMTK_INFO("Now it's in MTK Bluetooth Mode\n");
		BTMTK_INFO("STP is ready!\n");

		BTMTK_DBG("Register BT event callback!\n");
		mtk_wcn_stp_register_event_cb(BT_TASK_INDX, NULL);
	} else {
		BTMTK_ERR("STP is not ready!\n");
		mtk_wcn_wmt_func_off(WMTDRV_TYPE_BT);
		return -EIO;
	}

	BTMTK_DBG("Register BT reset callback!\n");
	mtk_wcn_wmt_msgcb_reg(WMTDRV_TYPE_BT, bt_cdev_rst_cb);

        BTMTK_INFO("%s set HCI_RUNNIN %08x\n",__func__, HCI_RUNNING);
        set_bit(HCI_RUNNING, &hdev->flags);

#if CFG_SUPPORT_NVRAM
	btmtk_fw_cfg();
#endif
	return 0;
}

static int btmtk_close(struct hci_dev *hdev)
{
	BTMTK_INFO("%s \n",__func__);
	if (!test_and_clear_bit(HCI_RUNNING, &hdev->flags)) {
		BTMTK_WARN("%s is not hci running\n",__func__);
		//return 0;
	}

	rstflag = 0;
	mtk_wcn_wmt_msgcb_unreg(WMTDRV_TYPE_BT);
	mtk_wcn_stp_register_event_cb(BT_TASK_INDX, NULL);

	if (mtk_wcn_wmt_func_off(WMTDRV_TYPE_BT) == MTK_WCN_BOOL_FALSE) {
		BTMTK_ERR("WMT turn off BT fail!\n");
		/* Mostly, native program will not check this return value. */
		return -EIO;
	}

	BTMTK_INFO("WMT turn off BT OK!\n");
	return 0;
}
static int btmtk_flush(struct hci_dev *hdev)
{
        //struct btmtk_private *priv = hci_get_drvdata(hdev);

        //skb_queue_purge(&priv->adapter->tx_queue);

        return 0;
}

static int btmtk_send_frame(struct hci_dev *hdev, struct sk_buff *skb)
{
	if (!hdev) {
		BTMTK_INFO("%s hdev=NULL return\n",__func__);
		return -ENODEV;
	}

	if (!test_bit(HCI_RUNNING, &hdev->flags)) {
		BTMTK_ERR("%s Failed testing HCI_RUNING, flags=%lx\n",
			__func__, hdev->flags);
		BTMTK_INFO("%s return -EBUSY\n",__func__);
		return -EBUSY;
	}

	BTMTK_DBG("%s type=%d, len=%d\n",__func__, bt_cb(skb)->pkt_type, skb->len);

	switch (bt_cb(skb)->pkt_type) {
	case HCI_COMMAND_PKT:
		hdev->stat.cmd_tx++;
		break;
	case HCI_ACLDATA_PKT:
		hdev->stat.acl_tx++;
		break;
	case HCI_SCODATA_PKT:
		hdev->stat.sco_tx++;
		break;
	}

	btmtk_sdio_host_to_card(skb);
	return 0;
}

static int btmtk_setup(struct hci_dev *hdev)
{
	BTMTK_INFO("%s \n",__func__);

	BTMTK_INFO("%s set HCI_RUNNIN %08x\n", __func__, HCI_RUNNING);
	set_bit(HCI_RUNNING, &hdev->flags);

	if (!test_bit(HCI_RUNNING, &hdev->flags))
		BTMTK_ERR("%s Failed testing HCI_RUNING, flags=%lx",
				__func__, hdev->flags);
	return 0;
}

static int btmtk_set_bdaddr(struct hci_dev *hdev, const bdaddr_t *bdaddr)
{
	unsigned char set_bdaddr[] = {0x01, 0x1A, 0xFC, 0x06,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	int ret = 0;

	memcpy(&set_bdaddr[4], &bdaddr->b[0], 6);

	PRINT_BUF(set_bdaddr, (sizeof(set_bdaddr) < 16 ? sizeof(set_bdaddr) : 16));

	ret = mtk_wcn_stp_send_data(set_bdaddr, sizeof(set_bdaddr), BT_TASK_INDX);
	if (ret < 0)
		BTMTK_ERR("Set BD addr failed!");
	return ret;
}

struct btmtk_private *btmtk_add_card(void)
{
	struct btmtk_private *priv;
	BTMTK_INFO("%s -->", __func__);
	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		BTMTK_ERR("Can not allocate priv");

	return priv;
}

int btmtk_remove_card(struct btmtk_private *priv)
{
	struct hci_dev *hdev;

	BTMTK_INFO("%s \n",__func__);
	hdev = priv->btmtk_dev.hcidev;

	hci_unregister_dev(hdev);

	hci_free_dev(hdev);

	priv->btmtk_dev.hcidev = NULL;

	kfree(priv);

	return 0;
}

int btmtk_register_hdev(struct btmtk_private *priv)
{
	struct hci_dev *hdev = NULL;
	int ret;

	BTMTK_INFO("%s \n",__func__);
	hdev = hci_alloc_dev();
	if (!hdev) {
		BTMTK_ERR("Can not allocate HCI device");
		goto err_hdev;
	}

	priv->btmtk_dev.hcidev = hdev;
	hci_set_drvdata(hdev, priv);

	hdev->bus   = HCI_SDIO;
	hdev->open  = btmtk_open;
	hdev->close = btmtk_close;
	hdev->flush = btmtk_flush;
	hdev->send  = btmtk_send_frame;
	hdev->setup = btmtk_setup;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,18,0))
	hdev->set_bdaddr = btmtk_set_bdaddr;
#endif
	hdev->dev_type = priv->btmtk_dev.dev_type;

	ret = hci_register_dev(hdev);
	if (ret < 0) {
		BTMTK_ERR("Can not register HCI device");
		goto err_hci_register_dev;
	}

	return 0;

err_hci_register_dev:
	hci_free_dev(hdev);

err_hdev:
	kfree(priv);

	return -ENOMEM;
}

static int BT_init(void)
{
	if (init_flag == 1) {
		BTMTK_INFO("Already initialized");
		return 0;
	}

	BTMTK_INFO();
#if CFG_SUPPORT_NVRAM
	cfg_flag = false;
#endif
	if (txbuf == NULL) {
		txbuf = kmalloc(BT_BUFFER_SIZE, GFP_ATOMIC);
		memset(txbuf, 0, BT_BUFFER_SIZE);
	}

	if (rxbuf == NULL) {
		rxbuf = kmalloc(BT_BUFFER_SIZE, GFP_ATOMIC);
		memset(rxbuf, 0, BT_BUFFER_SIZE);
	}

	btmtk_priv = btmtk_add_card();
	if (!btmtk_priv) {
		BTMTK_ERR("Initializing card failed!");
		kfree(txbuf);
		kfree(rxbuf);
		return -ENODEV;
	}

	btmtk_register_hdev(btmtk_priv);

	init_flag = 1;

	return 0;
}

static void BT_exit(void)
{
	if (init_flag != 1) {
		BTMTK_INFO("No initialize");
		return;
	}

	btmtk_remove_card(btmtk_priv);
	kfree(txbuf);
	kfree(rxbuf);
	init_flag = 0;
	BTMTK_INFO("%s driver removed\n", BT_DRIVER_NAME);
}

#ifdef MTK_WCN_REMOVE_KERNEL_MODULE
int mtk_wcn_stpbt_drv_init(void)
{
	return BT_init();
}
EXPORT_SYMBOL(mtk_wcn_stpbt_drv_init);

void mtk_wcn_stpbt_drv_exit(void)
{
	return BT_exit();
}
EXPORT_SYMBOL(mtk_wcn_stpbt_drv_exit);

#else

module_init(BT_init);
module_exit(BT_exit);

#endif
