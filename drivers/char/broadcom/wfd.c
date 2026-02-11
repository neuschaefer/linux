/*****************************************************************************
*  Copyright 2012 Broadcom Corporation.  All rights reserved.
*
*  Unless you and Broadcom execute a separate written software license
*  agreement governing use of this software, this software is licensed to you
*  under the terms of the GNU General Public License version 2, available at
*  http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
*  Notwithstanding the above, under no circumstances may you combine this
*  software in any way with any other Broadcom software provided under a
*  license other than the GPL, without Broadcom's express prior written
*  consent.
*
*****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */
#include <linux/version.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/platform_device.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/vmalloc.h>
#include <linux/semaphore.h>
#include <linux/uaccess.h>
#include <linux/list.h>
#include <linux/broadcom/wfd_ioctl.h>
#include <linux/broadcom/hdmi.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/poll.h>

/* ---- Private Constants and Types -------------------------------------- */

#define WFD_DEVICE_NAME       "wfd"
#define WFD_BUFFER_MAX_SIZE   256

/**
* Debug traces
*/
#define WFD_ENABLE_KNLLOG        0
#if WFD_ENABLE_KNLLOG
#include <linux/broadcom/knllog.h>
#define WFD_KNLLOG               KNLLOG
#else
#define WFD_KNLLOG(...)
#endif

/* ---- Private Function Prototypes -------------------------------------- */

static int wfd_probe(struct platform_device *pdev);
static int wfd_remove(struct platform_device *pdev);

static int wfd_open(struct inode *inode, struct file *file);
static int wfd_release(struct inode *inode, struct file *file);
static ssize_t wfd_read(struct file *file, char __user *buffer, size_t count,
			loff_t *ppos);
static ssize_t wfd_write(struct file *file, const char __user *buffer,
			 size_t count, loff_t *ppos);
static long wfd_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static unsigned int wfd_poll(struct file *file,
			     struct poll_table_struct *poll_table);

/* ---- Private Variables ------------------------------------------------- */

static int gDriverMajor;

#ifdef CONFIG_SYSFS
static struct class *wfd_class;
static struct device *wfd_dev;
#endif
static struct proc_dir_entry *wfd_proc_entry;

/* File Operations (these are the device driver entry points) */
static const struct file_operations gfops = {
	.owner = THIS_MODULE,
	.open = wfd_open,
	.release = wfd_release,
	.read = wfd_read,
	.write = wfd_write,
	.unlocked_ioctl = wfd_ioctl,
	.poll = wfd_poll,
};

#define WFD_DATA_SIZE   188
struct wfd_data_block {
	struct list_head wfd_list;

	uint8_t data[WFD_DATA_SIZE];
	uint32_t size;

};

struct wfd_stat_block {
	uint64_t wfd_rd;	/* Read counter */
	uint64_t wfd_rd_bytes;	/* Bytes successfully read */
	uint64_t wfd_rd_fail;	/* Read failure counter */
	uint64_t wfd_wt;	/* Write counter */
	uint64_t wfd_wt_bytes;	/* Bytes successfully written */
	uint64_t wfd_wt_fail;	/* Write failure counter */

};

union wfd_ioctl_params {
	struct wfd_ioctl_metadata metadata;
	struct wfd_ioctl_neg_config neg_config;
	struct wfd_ioctl_con_state con_state;
};

static struct wfd_data_block wfd_data;
static struct wfd_stat_block wfd_stat;
struct mutex wfd_lock;
struct wfd_ioctl_metadata wfd_metadata;
struct wfd_ioctl_neg_config wfd_neg_config;
struct wfd_ioctl_con_state wfd_con_state;

static DECLARE_WAIT_QUEUE_HEAD(read_wq);

/* ---- Public Variables ------------------------------------------------- */

/* ---- Functions -------------------------------------------------------- */

static unsigned int wfd_poll(struct file *file,
			     struct poll_table_struct *poll_table)
{
	unsigned int mask = 0;

	poll_wait(file, &read_wq, poll_table);

	if (!list_empty(&(wfd_data.wfd_list)))
		mask |= POLLIN | POLLRDNORM;	/* readable */

	return mask;
}

/***************************************************************************/
/**
*  Driver ioctl method to support user library API.
*
*  @return
*     >= 0           Number of bytes write
*     -ve            Error code
*/
static long wfd_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int rc = -EPERM;
	union wfd_ioctl_params parm;
	unsigned int cmdnr, size;

	cmdnr = _IOC_NR(cmd);
	size = _IOC_SIZE(cmd);

	if (size > sizeof(parm))
		return -ENOMEM;

	switch (cmdnr) {
	case WFD_CMD_SET_METADATA:
		rc = copy_from_user(&parm, (void __user *)arg, size);
		if (rc != 0)
			return rc;

		wfd_metadata.action = parm.metadata.action;
		WFD_KNLLOG("%s: set-metadata to 0x%08X\n", __func__,
			   wfd_metadata.action);

		switch (wfd_metadata.action) {
		case WFDSCRAPER_METADATA_START:
			hdmi_set_wifi_hdmi(1);
			break;

		case WFDSCRAPER_METADATA_STOP:
			hdmi_set_wifi_hdmi(0);
			break;

		default:
			break;
		}
		break;

	case WFD_CMD_GET_METADATA:
		parm.metadata.action = wfd_metadata.action;
		WFD_KNLLOG("%s: get-metadata reports 0x%08X\n",
			   __func__, parm.metadata.action);

		rc = copy_to_user((void __user *)arg, &(parm.metadata), size);
		if (rc != 0)
			return rc;

		break;

	case WFD_CMD_SET_NEG_CONFIG:
		rc = copy_from_user(&(parm.neg_config),
				    (void __user *)arg, size);
		if (rc != 0)
			return rc;

		memcpy(&wfd_neg_config,
		       &(parm.neg_config), sizeof(wfd_neg_config));
		break;

	case WFD_CMD_GET_NEG_CONFIG:
		rc = copy_to_user((void __user *)arg, &wfd_neg_config, size);
		if (rc != 0)
			return rc;
		break;

	case WFD_CMD_SET_CON_STATE:
		rc = copy_from_user(&(parm.con_state),
				    (void __user *)arg, size);

		if (rc != 0)
			return rc;

		memcpy(&wfd_con_state,
		       &(parm.con_state), sizeof(wfd_con_state));
		break;

	case WFD_CMD_GET_CON_STATE:
		rc = copy_to_user((void __user *)arg, &wfd_con_state, size);

		if (rc != 0)
			return rc;

		break;

	default:
		rc = -EINVAL;
		break;
	}

	return rc;
}

/***************************************************************************/
/**
*  Driver open routine
*
*  @return
*     0        Success
*     -ENOMEM  Insufficient memory
*/
static int wfd_open(struct inode *inode, struct file *file)
{
	/*
	 * Should ever been called once per run from the WFD core process, the
	 * media server process uses a dup of the descriptor opened by WFD core
	 * to write to the device...
	 *
	 * TODO: add some logic to ignore multiple open/close.
	 */

	memset(&wfd_stat, 0, sizeof(wfd_stat));

	return 0;
}

/***************************************************************************/
/**
*  Driver release routine
*
*  @return
*     0        Success
*     -ve      Error code
*/
static int wfd_release(struct inode *inode, struct file *file)
{
	/*
	 * Should ever been called once per run from the WFD core process, the
	 * media server process uses a dup of the descriptor opened by WFD core
	 * to write to the device...
	 *
	 * TODO: add some logic to ignore multiple open/close.
	 */

	return 0;
}

static ssize_t wfd_dequeue_and_copy(char __user *buffer, size_t count)
{
	struct wfd_data_block *data = NULL;
	int read = 0;
	int valid = 0;

	mutex_lock(&wfd_lock);
	if (!list_empty(&(wfd_data.wfd_list))) {
		data =
		    list_first_entry(&(wfd_data.wfd_list),
				     struct wfd_data_block, wfd_list);
		if (data) {
			valid = 1;
			list_del(&(data->wfd_list));
		}
	}
	mutex_unlock(&wfd_lock);

	if (valid) {
		if (copy_to_user((void *)buffer,
				 &(data->data),
				 (data->size <= count) ? data->size : count) ==
		    0) {
			read = ((data->size <= count) ? data->size : count);
			wfd_stat.wfd_rd_bytes += read;
		} else
			wfd_stat.wfd_rd_fail++;

		kfree(data);
		data = NULL;
	}

	return read;
}

/***************************************************************************/
/**
*  Driver read method
*
*  @return
*     >= 0           Number of bytes read
*     -ve            Error code
*/
static ssize_t wfd_read(struct file *file,
			char __user *buffer, size_t count, loff_t *ppos)
{
	int read = 0;

	wfd_stat.wfd_rd++;

	if (!list_empty(&(wfd_data.wfd_list))) {
		read = wfd_dequeue_and_copy(buffer, count);
	} else {
		if (file->f_flags & O_NONBLOCK)
			return -EAGAIN;

		while (list_empty(&(wfd_data.wfd_list))) {
			if (wait_event_interruptible(read_wq,
						     (!list_empty
						      (&(wfd_data.wfd_list)))))
				return -ERESTARTSYS;
		}

		read = wfd_dequeue_and_copy(buffer, count);
	}

	return read;
}

/***************************************************************************/
/**
*  Driver write method.
*
*  @return
*     >= 0           Number of bytes write
*     -ve            Error code
*/
static ssize_t wfd_write(struct file *file,
			 const char __user *buffer,
			 size_t count, loff_t *ppos)
{
	struct wfd_data_block *data = NULL;
	int write = 0;

	wfd_stat.wfd_wt++;

	data = kzalloc(sizeof(struct wfd_data_block), GFP_KERNEL);
	if (data == NULL) {
		wfd_stat.wfd_wt_fail++;
	} else {
		if (copy_from_user(&(data->data),
				   buffer,
				   (count <=
				    WFD_DATA_SIZE) ? count : WFD_DATA_SIZE) ==
		    0) {
			data->size =
			    ((count <= WFD_DATA_SIZE) ? count : WFD_DATA_SIZE);
			write = data->size;

			mutex_lock(&wfd_lock);
			list_add_tail(&(data->wfd_list), &(wfd_data.wfd_list));
			mutex_unlock(&wfd_lock);

			wfd_stat.wfd_wt_bytes += write;

			wake_up_interruptible(&read_wq);
		} else {
			wfd_stat.wfd_wt_fail++;

			kfree(data);
			data = NULL;
		}
	}

	return write;
}

/***************************************************************************/
/**
*  Driver proc entry read method.
*
*/
static int wfd_proc_read(char *buf,
			 char **start,
			 off_t offset, int count, int *eof, void *data)
{
	char *p = buf;

	(void)start;
	(void)count;
	(void)data;

	if (offset > 0) {
		*eof = 1;
		return 0;
	}

	p += sprintf(p, "[wfd-meta]    \'%c%c%c%c\'\n",
		     (wfd_metadata.action >> 24) & 0xFF,
		     (wfd_metadata.action >> 16) & 0xFF,
		     (wfd_metadata.action >> 8) & 0xFF,
		     (wfd_metadata.action) & 0xFF);

	p += sprintf(p, "[wfd-config]  video: %dx%d @ %d fps - \'%c\' scan\n",
		     wfd_neg_config.width,
		     wfd_neg_config.height,
		     wfd_neg_config.fps, wfd_neg_config.scan_mode);

	p += sprintf(p, "[wfd-config]  audio: %d @ %d Hz - %d ch\n",
		     (int)wfd_neg_config.audio_fmt,
		     wfd_neg_config.audio_hz, wfd_neg_config.audio_ch);

	p += sprintf(p, "[wfd-state]   %s\n",
		     wfd_con_state.status ? "CONNECTED" : "idle");

	p += sprintf(p, "[wfd-read]    %llu calls, %llu bytes, %llu failure\n",
		     wfd_stat.wfd_rd,
		     wfd_stat.wfd_rd_bytes, wfd_stat.wfd_rd_fail);

	p += sprintf(p,
		     "[wfd-write]   %llu calls, %llu bytes, %llu failure\n\n",
		     wfd_stat.wfd_wt, wfd_stat.wfd_wt_bytes,
		     wfd_stat.wfd_wt_fail);

	*eof = 1;
	return p - buf;
}

/***************************************************************************/
/**
*  Platform support constructor
*/
static int wfd_probe(struct platform_device *pdev)
{
	int err = 0;

	gDriverMajor = register_chrdev(0, WFD_DEVICE_NAME, &gfops);
	if (gDriverMajor < 0) {
		printk(KERN_ERR
		       "WFD: Failed to register character device major\n");
		err = -EFAULT;
		goto error_cleanup;
	}
#ifdef CONFIG_SYSFS
	wfd_class = class_create(THIS_MODULE, "wfd-class");
	if (IS_ERR(wfd_class)) {
		printk(KERN_ERR "WFD: Class create failed\n");
		err = -EFAULT;
		goto err_unregister_chrdev;
	}

	wfd_dev = device_create(wfd_class,
				NULL,
				MKDEV(gDriverMajor, 0), NULL, WFD_DEVICE_NAME);
	if (IS_ERR(wfd_dev)) {
		printk(KERN_ERR "WFD: Device create failed\n");
		err = -EFAULT;
		goto err_class_destroy;
	}
#endif

	wfd_proc_entry = create_proc_entry(WFD_DEVICE_NAME, 0660, NULL);
	if (wfd_proc_entry == NULL) {
		err = -EFAULT;
		printk(KERN_ERR "WFD: Proc-entry create failed\n");
		goto err_device_destroy;
	}
	wfd_proc_entry->read_proc = wfd_proc_read;
	wfd_proc_entry->write_proc = NULL;

	memset(&wfd_stat, 0, sizeof(wfd_stat));
	memset(&wfd_neg_config, 0, sizeof(wfd_neg_config));
	memset(&wfd_con_state, 0, sizeof(wfd_con_state));

	INIT_LIST_HEAD(&(wfd_data.wfd_list));
	mutex_init(&wfd_lock);

	printk(KERN_INFO "WiFi Display - \'WFD\' driver...\n");
	return 0;

err_device_destroy:
#ifdef CONFIG_SYSFS
	device_destroy(wfd_class, MKDEV(gDriverMajor, 0));
err_class_destroy:
	class_destroy(wfd_class);
err_unregister_chrdev:
	unregister_chrdev(gDriverMajor, WFD_DEVICE_NAME);
#endif
error_cleanup:
	wfd_remove(pdev);
	return err;
}

/***************************************************************************/
/**
*  Platform support destructor
*/
static int wfd_remove(struct platform_device *pdev)
{
	remove_proc_entry(wfd_proc_entry->name, NULL);
#ifdef CONFIG_SYSFS
	device_destroy(wfd_class, MKDEV(gDriverMajor, 0));
	class_destroy(wfd_class);
#endif

	unregister_chrdev(gDriverMajor, "wfd");

	mutex_destroy(&wfd_lock);

	return 0;
}

/* Platform driver */
static struct platform_driver wfd_driver = {
	.driver = {
		   .name = "bcm-wfd",
		   .owner = THIS_MODULE,
		   },
	.probe = wfd_probe,
	.remove = wfd_remove,
};

static int __init wfd_init(void)
{
	WFD_KNLLOG("%s: called...\n", __func__);

	return platform_driver_register(&wfd_driver);
}

static void __exit wfd_exit(void)
{
	WFD_KNLLOG("%s: called...\n", __func__);

	platform_driver_unregister(&wfd_driver);
}

module_init(wfd_init);
module_exit(wfd_exit);
MODULE_AUTHOR("Broadcom");
MODULE_DESCRIPTION("WiFi Display Datapath.");
MODULE_LICENSE("GPL");
