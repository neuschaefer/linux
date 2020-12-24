/*
 * mass_storage.c -- USB Mass Storage Function, for USB development
 *
 * Copyright (C) 2006 Avocent Corp.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The names of the above-listed copyright holders may not be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * ALTERNATIVELY, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") as published by the Free Software
 * Foundation, either version 2 of that License or (at your option) any
 * later version.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/*
 * The Mass Storage Function acts as a USB Mass Storage device,
 * appearing to the host as multiple CD/DVD disk drives and or 
 * HD/FD Disk Drives.
 * In addition to providing an example of a genuinely useful function
 * driver for a USB device, it also illustrates a technique of
 * double-buffering for increased throughput. Last but not least, it gives an
 * easy way to probe the behavior of the Mass Storage drivers in a USB host.
 * This driver is heavily based on the File-backed Storage Gadget by Alan Stern.
 * In fact, it is a modified version of it.
 *
 *
 * The function supports Bulk-Only transports.  It supports the
 * following protocols: ATAPI or SFF-8020i (0x02) for CD/DVD, and SFF-8070i
 * (0x05) for the Floppy device.
 *
 * There is support for multiple logical units (LUNs).  The number of LUNs
 * can be set using the optional "luns" module parameter (anywhere from 1 to 16).
 * The default number of LUNs is 1.
 *
 * Requirements are modest; only a bulk-in and a bulk-out endpoint are
 * needed for each device (total of two bulk-in and two bulk-out).  The memory
 * requirement amounts to four 16K buffers, size configurable by a parameter.
 * Support is included for both full-speed and high-speed operation.
 *
 * Module options:
 *
 *				Driver Design
 *
 * The MSD driver is fairly straightforward.  There are up to threee main kernel
 * threads that handle most of the work for each USB interface.
 * Interrupt routines field callbacks from the controller driver: bulk- request
 * completion notifications, endpoint-0 events, and disconnect events.
 * Completion events are passed to the main threads by wakeup calls.  Many
 * ep0 requests are handled at interrupt time, but SetInterface,
 * SetConfiguration, and device reset requests are forwarded to the
 * threads in the form of "exceptions" using SIGUSR1 signals (since they
 * should interrupt any ongoing file I/O operations).
 *
 * Each thread's main routine implements the standard command/data/status
 * parts of a SCSI interaction.  It and its subroutines are full of tests
 * for pending signals/exceptions -- all this polling is necessary since
 * the kernel has no setjmp/longjmp equivalents.  (Maybe this is an
 * indication that the driver really wants to be running in userspace.)
 * The thread catches INT, TERM, and KILL signals and converts them into
 * an EXIT exception.
 *
 * In normal operation the main threads are started during the gadget's
 * msd_bind() callback and stopped during msd_unbind().  But it can also
 * exit when it receives a signal, and there's no point leaving the
 * gadget running when one of threads is dead.  So just before the thread
 * exits, it deregisters the function driver.  This makes things a little
 * tricky: The driver is deregistered at two places, and the exiting
 * thread can indirectly call msd_unbind() which in turn can tell the
 * thread to exit.  The first problem is resolved through the use of the
 * REGISTERED atomic bitflag; the driver will only be deregistered once.
 * The second problem is resolved by having msd_unbind() check
 * msd->state; it won't try to stop the thread if the state is already
 * FSG_STATE_TERMINATED.
 *
 * To provide maximum throughput, the driver uses a circular pipeline of
 * buffer heads (struct msd_buffhd).  In principle the pipeline can be
 * arbitrarily long; in practice the benefits don't justify having more
 * than 2 stages (i.e., double buffering).  But it helps to think of the
 * pipeline as being a long one.  Each buffer head contains a bulk-in and
 * a bulk-out request pointer (since the buffer can be used for both
 * output and input -- directions always are given from the host's
 * point of view) as well as a pointer to the buffer and various state
 * variables.
 *
 * Use of the pipeline follows a simple protocol.  There is a variable
 * (msd->next_buffhd_to_fill) that points to the next buffer head to use.
 * At any time that buffer head may still be in use from an earlier
 * request, so each buffer head has a state variable indicating whether
 * it is EMPTY, FULL, or BUSY.  Typical use involves waiting for the
 * buffer head to be EMPTY, filling the buffer either by file I/O or by
 * USB I/O (during which the buffer head is BUSY), and marking the buffer
 * head FULL when the I/O is complete.  Then the buffer will be emptied
 * (again possibly by USB I/O, during which it is marked BUSY) and
 * finally marked EMPTY again (possibly by a completion routine).
 *
 * One subtle point concerns sending status-stage responses for ep0
 * requests.  Some of these requests, such as device reset, can involve
 * interrupting an ongoing file I/O operation, which might take an
 * arbitrarily long time.  During that delay the host might give up on
 * the original ep0 request and issue a new one.  When that happens the
 * driver should not notify the host about completion of the original
 * request, as the host will no longer be waiting for it.  So the driver
 * assigns to each ep0 request a unique tag, and it keeps track of the
 * tag value of the request associated with a long-running exception
 * (device-reset, interface-change, or configuration-change).  When the
 * exception handler is finished, the status-stage response is submitted
 * only if the current ep0 request tag is equal to the exception request
 * tag.  Thus only the most recently received ep0 request will get a
 * status-stage response.
 *
 * User applications can access the driver thru ioctl calls, and i/o using
 * calls to select(), read(), and write().
 */
 
#undef DEBUG
#undef VERBOSE
#undef DUMP_MSGS

#ifndef LINUX_VERSION_CODE
#include <linux/version.h>
#endif

#include <asm/system.h>
#include <asm/uaccess.h>

#include <linux/bitops.h>
#include <linux/blkdev.h>
#include <linux/compiler.h>
#include <linux/completion.h>
#include <linux/dcache.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fcntl.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/kernel.h>
#include <linux/kref.h>
#include <linux/kthread.h>
#include <linux/limits.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/pagemap.h>
#include <linux/rwsem.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/suspend.h>
#include <linux/utsname.h>
#include <linux/poll.h>
#include <linux/dma-mapping.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21)
   #include <linux/config.h>
   #include <linux/usb_ch9.h>
#else
   #include <linux/usb/ch9.h>
   #include <linux/freezer.h>
#endif

#include <linux/usb_gadget.h>

#include <linux/ctype.h>
#include <asm/unaligned.h>
#include <linux/cdev.h>

#include "composite.h"

#include "gadget_chips.h"

/*-------------------------------------------------------------------------*/

#define DRIVER_DESC		"Mass Storage Function"
#define DRIVER_NAME		"g_mass_storage"
#define DRIVER_VERSION		"20090511"
#define CONTROLLER_NAME         "PCD"
#define VENDOR_NAME             "Linux"
#define PRODUCT_NAME            "Virtual Media"

static const char longname[] = DRIVER_DESC;
static const char shortname[] = DRIVER_NAME;

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_AUTHOR("Tony Scragg");
MODULE_LICENSE("Dual BSD/GPL");

/*
 * This driver assumes self-powered hardware and has no way for users to
 * trigger remote wakeup.  It uses autoconfiguration to select endpoints
 * and endpoint addresses.
 */

/*-------------------------------------------------------------------------*/

/* Encapsulate the module parameter settings */
static struct {
	unsigned int 	num_interfaces;
	unsigned int	num_luns;
	char            *controller;
	char            *vendor;
	char            *product;
	int		can_stall;
	unsigned short	release;
	unsigned int	buflen;
} mod_data = {
	.num_interfaces         = 1,
	.num_luns               = 1,
	.controller             = CONTROLLER_NAME,
	.vendor                 = VENDOR_NAME,
	.product                = PRODUCT_NAME,
	.can_stall		= 1,
	.release		= 0xffff,	// Use controller chip type
	.buflen			= 16384,
};

static char controller[16+1];

module_param_named(interfaces, mod_data.num_interfaces, uint, S_IRUGO);
MODULE_PARM_DESC(interfaces, "number of interfaces");

//module_param_named(luns, mod_data.num_luns, uint, S_IRUGO);
//MODULE_PARM_DESC(luns, "number of LUNs per interface");

module_param_named(controller, mod_data.controller, charp, S_IRUGO);
MODULE_PARM_DESC(controller, "Name of peripheral controller driver");

module_param_named(vendor, mod_data.vendor, charp, S_IRUGO);
MODULE_PARM_DESC(vendor, "Vendor name for inquiry data");

module_param_named(product, mod_data.product, charp, S_IRUGO);
MODULE_PARM_DESC(product, "Product name for device for inquiry");

module_param_named(release, mod_data.release, ushort, S_IRUGO);
MODULE_PARM_DESC(release, "Revision number for inquiry");

module_param_named(buflen, mod_data.buflen, uint, S_IRUGO);
MODULE_PARM_DESC(buflen, "I/O buffer size");

/*-------------------------------------------------------------------------*/
/* USB protocol value = the transport method */
#define USB_PR_CBI	0x00		// Control/Bulk/Interrupt
#define USB_PR_CB	0x01		// Control/Bulk w/o interrupt
#define USB_PR_BULK	0x50		// Bulk-only

/* USB subclass value = the protocol encapsulation */
#define USB_SC_RBC	0x01		// Reduced Block Commands (flash)
#define USB_SC_8020	0x02		// SFF-8020i, MMC-2, ATAPI (CD-ROM)
#define USB_SC_QIC	0x03		// QIC-157 (tape)
#define USB_SC_UFI	0x04		// UFI (floppy)
#define USB_SC_8070	0x05		// SFF-8070i (removable)
#define USB_SC_SCSI	0x06		// Transparent SCSI

/* Bulk-only data structures */

/* Command Block Wrapper */
struct bulk_cb_wrap {
	__le32	Signature;		// Contains 'USBC'
	u32	Tag;			// Unique per command id
	__le32	DataTransferLength;	// Size of the data
	u8	Flags;			// Direction in bit 7
	u8	Lun;			// LUN (normally 0)
	u8	Length;			// Of the CDB, <= MAX_COMMAND_SIZE
	u8	CDB[16];		// Command Data Block
};

#define USB_BULK_CB_WRAP_LEN	31
#define USB_BULK_CB_SIG		0x43425355	// Spells out USBC
#define USB_BULK_IN_FLAG	0x80

/* Command Status Wrapper */
struct bulk_cs_wrap {
	__le32	Signature;		// Should = 'USBS'
	u32	Tag;			// Same as original command
	__le32	Residue;		// Amount not transferred
	u8	Status;			// See below
};

#define USB_BULK_CS_WRAP_LEN	13
#define USB_BULK_CS_SIG		0x53425355	// Spells out 'USBS'
#define USB_STATUS_PASS		0
#define USB_STATUS_FAIL		1
#define USB_STATUS_PHASE_ERROR	2

/* Bulk-only class specific requests */
#define USB_BULK_RESET_REQUEST		0xff
#define USB_BULK_GET_MAX_LUN_REQUEST	0xfe

#define MAX_COMMAND_SIZE	16	// Length of a SCSI Command Data Block

/* SCSI commands that we recognize */
#define SC_FORMAT_UNIT			0x04
#define SC_INQUIRY			0x12
#define SC_MODE_SELECT_6		0x15
#define SC_MODE_SELECT_10		0x55
#define SC_MODE_SENSE_6			0x1a
#define SC_MODE_SENSE_10		0x5a
#define SC_PREVENT_ALLOW_MEDIUM_REMOVAL	0x1e
#define SC_READ_6			0x08
#define SC_READ_10			0x28
#define SC_READ_12			0xa8
#define SC_READ_CAPACITY		0x25
#define SC_READ_FORMAT_CAPACITIES	0x23
#define SC_RELEASE			0x17
#define SC_REQUEST_SENSE		0x03
#define SC_RESERVE			0x16
#define SC_SEND_DIAGNOSTIC		0x1d
#define SC_START_STOP_UNIT		0x1b
#define SC_SYNCHRONIZE_CACHE		0x35
#define SC_TEST_UNIT_READY		0x00
#define SC_VERIFY			0x2f
#define SC_WRITE_6			0x0a
#define SC_WRITE_10			0x2a
#define SC_WRITE_12			0xaa
#define SC_READ_TOC			0x43
#define SC_READ_DVD_STRUCTURE		0xad

/* SCSI Sense Key/Additional Sense Code/ASC Qualifier values */
#define SS_NO_SENSE				0
#define SS_COMMUNICATION_FAILURE		0x040800
#define SS_INVALID_COMMAND			0x052000
#define SS_INVALID_FIELD_IN_CDB			0x052400
#define SS_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE	0x052100
#define SS_LOGICAL_UNIT_NOT_SUPPORTED		0x052500
#define SS_MEDIUM_NOT_PRESENT			0x023a00
#define SS_MEDIUM_REMOVAL_PREVENTED		0x055302
#define SS_NOT_READY_TO_READY_TRANSITION	0x062800
#define SS_RESET_OCCURRED			0x062900
#define SS_SAVING_PARAMETERS_NOT_SUPPORTED	0x053900
#define SS_UNRECOVERED_READ_ERROR		0x031100
#define SS_WRITE_ERROR				0x030c02
#define SS_WRITE_PROTECTED			0x072700

#define SK(x)		((u8) ((x) >> 16))	// Sense Key byte, etc.
#define ASC(x)		((u8) ((x) >> 8))
#define ASCQ(x)		((u8) (x))

/*-------------------------------------------------------------------------*/
// IOCTL types
#define IOCTL_MSD_OPEN              _IOW('i', 190, struct msd_open_data)
#define IOCTL_MSD_EJECT             _IOW('i', 191, unsigned int)
#define IOCTL_MSD_STATUS            _IOWR('i', 192, struct msd_status_data)
#define IOCTL_MSD_RESET             _IOW('i', 193, unsigned int)
#define IOCTL_MSD_SET_LUN_TYPES     _IOWR('i', 194, struct msd_type_data)
#define IOCTL_MSD_GET_LUN_TYPES     _IOWR('i', 195, struct msd_type_data)
#define IOCTL_MSD_SET_INQUIRY_DATA  _IOWR('i', 196, struct msd_inquiry_data)
#define IOCTL_MSD_WRITE_COMPLETE    _IO('i', 197)
#define IOCTL_MSD_READ_COMPLETE     _IO('i', 198)
#define IOCTL_MSD_WRITE_ERROR       _IO('i', 199)
#define IOCTL_MSD_READ_ERROR        _IO('i', 200)

// Request types
#define MSGUSBDATA_FLAG_READ            1
#define MSGUSBDATA_FLAG_WRITE           2
#define MSGUSBDATA_FLAG_ERROR           4
#define MSGUSBDATA_FLAG_OPEN            8
#define MSGUSBDATA_FLAG_GETSTATUS       16
#define MSGUSBDATA_FLAG_EJECT           32

// IOCTL data types
struct msd_status_data
{
	unsigned short flags;
	unsigned int start_block;
	unsigned int block_size;
	unsigned int num_blocks; 
	unsigned int lun;
};

struct msd_open_data
{
	unsigned short flags;
	unsigned int block_size;
	unsigned int num_blocks; 
	unsigned int lun;
	unsigned int cylinders;
	unsigned int heads;
	unsigned int sectors;
	unsigned int toc_size;
	char lba_data[804];
	char msf_data[804];
	unsigned int atr_size;
	char atr_data[255];
};

#define MAX_TOC_SIZE	512

struct cd_toc {
	int size;
	char data_lba[MAX_TOC_SIZE];
	char data_msf[MAX_TOC_SIZE];
};

struct scsi_toc_track_data {
	u8        reserved1;
	u8        adr_ctrl;
	u8        track_number;
	u8        reserved2;
	u32       address;
};

struct scsi_toc_data {
	u16                             length;
	u8                              first_track;
	u8                              last_track;
	struct scsi_toc_track_data      track_data[100];
};

struct msd_type_data {
	u8	types[16];
	u8      preventRemoval[16];
};
struct msd_inquiry_data {
	u8	lun;
	u8	reserved[3];
	u8	productId[16];
};

/*-------------------------------------------------------------------------*/
#define MAX_INTERFACES		3
#define MAX_LUNS		16

#define USB_DRIVE_TYPE_NONE	0x00
#define USB_DRIVE_TYPE_DISABLED	0x01
#define USB_DRIVE_TYPE_CD	0x02
#define USB_DRIVE_TYPE_HD	0x04
#define USB_DRIVE_TYPE_FD	0x08

struct lun {
	u32		num_blocks;
	u32		block_size;
	u32		mounted : 1;
	u32		ro : 1;
	u32		prevent_medium_removal : 1;
	u32		registered : 1;
	u32		sense_data;
	u32		sense_data_info;
	u32		unit_attention_data;
	u8		media_changed;
	u32		amount_left_to_read;
	u8		read_pending;
	u32		amount_left_to_write;
	u8		write_pending;
	u32		curlba;
	u32		type;
	char		vendor[8+1];
	char		product[16+1];
	u32		cylinders;
	u32		heads;
	u32		sectors;
	struct cd_toc	toc_info;
	struct device	dev;
};

#define lun_is_mounted(curlun)	((curlun)->mounted == 1)

static inline struct lun *dev_to_lun(struct device *dev)
{
	return container_of(dev, struct lun, dev);
}

#define get_drive_index(x) (drive_ids[(x)].drive_type)
#define get_lun_num(x) (drive_ids[(x)].lun_num)

/* Big enough to hold our biggest descriptor */
#define EP0_BUFSIZE	256
#define DELAYED_STATUS	(EP0_BUFSIZE + 999)	// An impossibly large value

/* Number of buffers we will use.  2 is enough for double-buffering */
#define NUM_BUFFERS	4

enum msd_buffer_state {
	BUF_STATE_EMPTY = 0,
	BUF_STATE_FULL,
	BUF_STATE_BUSY
};

struct msd_buffhd {
	void				*buf;
	dma_addr_t			dma;
	enum msd_buffer_state		state;
	struct msd_buffhd		*next;

	/* The NetChip 2280 is faster, and handles some protocol faults
	 * better, if we don't submit any short bulk-out read requests.
	 * So we will record the intended request length here. */
	unsigned int			bulk_out_intended_length;

	struct usb_request		*inreq;
	int				inreq_busy;
	struct usb_request		*outreq;
	int				outreq_busy;
};

enum msd_state {
	MSD_STATE_COMMAND_PHASE = -10,		// This one isn't used anywhere
	MSD_STATE_DATA_PHASE,
	MSD_STATE_STATUS_PHASE,
	
	MSD_STATE_IDLE = 0,
	MSD_STATE_ABORT_BULK_OUT,
	MSD_STATE_RESET,
	MSD_STATE_INTERFACE_CHANGE,
	MSD_STATE_CONFIG_CHANGE,
	MSD_STATE_DISCONNECT,
	MSD_STATE_EXIT,
	MSD_STATE_TERMINATED
};

enum data_direction {
	DATA_DIR_UNKNOWN = 0,
	DATA_DIR_FROM_HOST,
	DATA_DIR_TO_HOST,
	DATA_DIR_NONE
};

struct interface_dev {
 	u32			bulk_out_maxpacket;
	enum msd_state		state;		// For exception handling
	u32			exception_req_tag;
	u32			running : 1;
	u32			bulk_in_enabled : 1;
	u32			bulk_out_enabled : 1;
	u32			phase_error : 1;
	u32			short_packet_received : 1;
	u32			bad_lun_okay : 1;
	unsigned long		atomic_bitflags;
#define REGISTERED		0
#define CLEAR_BULK_HALTS	1
#define SUSPENDED		2
	u8			config;
	struct usb_ep		*bulk_in;
	struct usb_ep		*bulk_out;
	struct msd_buffhd	*next_buffhd_to_fill;
	struct msd_buffhd	*next_buffhd_to_drain;
	struct msd_buffhd	buffhds[NUM_BUFFERS];
	int			thread_wakeup_needed;
	struct completion	thread_notifier;
	struct task_struct	*thread_task;
	sigset_t		thread_signal_mask;
	int			cmnd_size;
	u8			cmnd[MAX_COMMAND_SIZE];
	enum data_direction	data_dir;
	u32			data_size;
	u32			data_size_from_cmnd;
	u32			tag;
	u32			residue;
	u32			usb_amount_left;
	u32			nluns;
	u32			nlunsReset;
	u32			lun;
	struct lun		*luns;
	struct lun		*curlun;
	wait_queue_head_t	status_wait_q;
	wait_queue_head_t	write_wait_q;
	wait_queue_head_t	read_wait_q;
	u8			write_aborted;
	u8			read_aborted;
	struct msd_status_data	status;
	u8			index;
	struct timer_list	read_timer;
};

struct msd_dev {
	/* lock protects: state, all the req_busy's, and cbbuf_cmnd */
	spinlock_t		lock;
	struct usb_gadget	*gadget;
	struct usb_ep		*ep0;		// Handy copy of gadget->ep0
	struct usb_request	*ep0req;	// For control responses
	u32			ep0_req_tag;
	const char		*ep0req_name;
	u8			config;
	u8			new_config;
	u8			is_registered;
	dev_t			chrdev;
	struct cdev		cdev;
	u8			num_interfaces;
	struct interface_dev	interface[MAX_INTERFACES];
	struct usb_composite_dev *c_dev;	
};

int				function_index = 0;
static struct msd_dev		*the_msd;
static struct usb_function	msd_driver;

/*-------------------------------------------------------------------------*/

typedef void (*msd_routine_t)(struct interface_dev *);

static int inline exception_in_progress(struct interface_dev *interface)
{
	return (interface->state > MSD_STATE_IDLE);
}

/* Make bulk-out requests be divisible by the maxpacket size */
static void inline set_bulk_out_req_length(struct interface_dev *interface,
		struct msd_buffhd *bh, unsigned int length)
{
	unsigned int	rem;

	bh->bulk_out_intended_length = length;
	rem = length % interface->bulk_out_maxpacket;
	if (rem > 0)
		length += interface->bulk_out_maxpacket - rem;
	bh->outreq->length = length;
}

/*-------------------------------------------------------------------------*/


static int msd_set_halt(struct interface_dev *interface, struct usb_ep *ep)
{
	return usb_ep_set_halt(ep);
}


/*-------------------------------------------------------------------------*/

/* Routines for unaligned data access */

static u16 inline get_be16(u8 *buf)
{
	return ((u16) buf[0] << 8) | ((u16) buf[1]);
}

static u32 inline get_be32(u8 *buf)
{
	return ((u32) buf[0] << 24) | ((u32) buf[1] << 16) |
			((u32) buf[2] << 8) | ((u32) buf[3]);
}

static void inline put_be16(u8 *buf, u16 val)
{
	buf[0] = val >> 8;
	buf[1] = val;
}

static void inline put_be32(u8 *buf, u32 val)
{
	buf[0] = val >> 24;
	buf[1] = val >> 16;
	buf[2] = val >> 8;
	buf[3] = val & 0xff;
}


/*-------------------------------------------------------------------------*/

/*
 * DESCRIPTORS ... most are static, but string ids and interface
 * descriptors are adjusted during msd_bind().
 */
#define STRING_INTERFACE_0	1
#define STRING_INTERFACE_1	2
#define STRING_INTERFACE_2	3

static struct usb_otg_descriptor
otg_desc = {
	.bLength		= sizeof(otg_desc),
	.bDescriptorType	= USB_DT_OTG,
	.bmAttributes		= USB_OTG_SRP,
};

static struct usb_interface_descriptor intf_desc[MAX_INTERFACES] = {
{
	.bLength		= sizeof (struct usb_interface_descriptor),
	.bDescriptorType	= USB_DT_INTERFACE,
	.bNumEndpoints		= 2,
	.bInterfaceClass	= USB_CLASS_MASS_STORAGE,
	.bInterfaceSubClass	= USB_SC_SCSI,
	.bInterfaceProtocol	= USB_PR_BULK,
	.iInterface		= STRING_INTERFACE_0,
},
{
	.bLength		= sizeof (struct usb_interface_descriptor),
	.bDescriptorType	= USB_DT_INTERFACE,
	.bNumEndpoints		= 2,
	.bInterfaceClass	= USB_CLASS_MASS_STORAGE,
	.bInterfaceSubClass	= USB_SC_SCSI,
	.bInterfaceProtocol	= USB_PR_BULK,
	.iInterface		= STRING_INTERFACE_1,
},
{
	.bLength		= sizeof (struct usb_interface_descriptor),
	.bDescriptorType	= USB_DT_INTERFACE,
	.bNumEndpoints		= 2,
	.bInterfaceClass	= USB_CLASS_MASS_STORAGE,
	.bInterfaceSubClass	= USB_SC_SCSI,
	.bInterfaceProtocol	= USB_PR_BULK,
	.iInterface		= STRING_INTERFACE_2,
}
};

static struct usb_interface_info	intf_info[MAX_INTERFACES] = {
{
	.enabled		= 0,
	.intf_desc		= &intf_desc[0],
},
{
	.enabled		= 0,
	.intf_desc		= &intf_desc[1],
},
{
	.enabled		= 0,
	.intf_desc		= &intf_desc[2],
}
};

static struct usb_interface_info *intf_info_table[] = {
	&intf_info[0],
	&intf_info[1],
	&intf_info[2],	
	NULL,
};


/* Two full-speed endpoint descriptors: bulk-in, bulk-out */
/* for each interface */
static struct usb_endpoint_descriptor fs_bulk_in_desc[MAX_INTERFACES] = {
{
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	.bEndpointAddress	= USB_DIR_IN,
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	/* wMaxPacketSize set by autoconfiguration */
},
{
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	.bEndpointAddress	= USB_DIR_IN,
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	/* wMaxPacketSize set by autoconfiguration */
},
{
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	.bEndpointAddress	= USB_DIR_IN,
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	/* wMaxPacketSize set by autoconfiguration */
}
};

static struct usb_endpoint_descriptor fs_bulk_out_desc[MAX_INTERFACES] = {
{
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	.bEndpointAddress	= USB_DIR_OUT,
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	/* wMaxPacketSize set by autoconfiguration */
},
{
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	.bEndpointAddress	= USB_DIR_OUT,
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	/* wMaxPacketSize set by autoconfiguration */
},
{
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	.bEndpointAddress	= USB_DIR_OUT,
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	/* wMaxPacketSize set by autoconfiguration */
}
};

// This gets built in msd_bind
static struct usb_descriptor_header *fs_function[] = {
	(struct usb_descriptor_header *) &otg_desc,
	NULL,  // &intf_desc[0]
	NULL,  // &fs_bulk_in_desc[0]
	NULL,  // &fs_bulk_out_desc[0]
	NULL,  // &intf_desc[1]
	NULL,  // &fs_bulk_in_desc[1]
	NULL,  // &fs_bulk_out_desc[1]
	NULL,  // &intf_desc[2]
	NULL,  // &fs_bulk_in_desc[2]
	NULL,  // &fs_bulk_out_desc[2]	
	NULL,
};

#ifdef	CONFIG_USB_GADGET_DUALSPEED

/*
 * USB 2.0 devices need to expose both high speed and full speed
 * descriptors, unless they only run at full speed.
 *
 */
static struct usb_endpoint_descriptor hs_bulk_in_desc[MAX_INTERFACES] = {
{
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	/* bEndpointAddress copied from fs_bulk_in_desc during msd_bind() */
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize		= __constant_cpu_to_le16(512),
},
{
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	/* bEndpointAddress copied from fs_bulk_in_desc during msd_bind() */
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize		= __constant_cpu_to_le16(512),
},
{
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	/* bEndpointAddress copied from fs_bulk_in_desc during msd_bind() */
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize		= __constant_cpu_to_le16(512),
}
};

static struct usb_endpoint_descriptor hs_bulk_out_desc[MAX_INTERFACES] = {
{
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	/* bEndpointAddress copied from fs_bulk_out_desc during msd_bind() */
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize		= __constant_cpu_to_le16(512),
	.bInterval		= 1,	// NAK every 1 uframe
},
{
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	/* bEndpointAddress copied from fs_bulk_out_desc during msd_bind() */
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize		= __constant_cpu_to_le16(512),
	.bInterval		= 1,	// NAK every 1 uframe
},
{
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	/* bEndpointAddress copied from fs_bulk_out_desc during msd_bind() */
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize		= __constant_cpu_to_le16(512),
	.bInterval		= 1,	// NAK every 1 uframe
}
};

// This gets built in msd_bind
static struct usb_descriptor_header *hs_function[] = {
	(struct usb_descriptor_header *) &otg_desc,	
	NULL,  // &intf_desc[0]
	NULL,  // &hs_bulk_in_desc[0]
	NULL,  // &hs_bulk_out_desc[0]
	NULL,  // &intf_desc[1]
	NULL,  // &hs_bulk_in_desc[1]
	NULL,  // &hs_bulk_out_desc[1]
	NULL,  // &intf_desc[2]
	NULL,  // &hs_bulk_in_desc[2]
	NULL,  // &hs_bulk_out_desc[2]	
	NULL,
}; 

/* Maxpacket and other transfer characteristics vary by speed. */
#define ep_desc(g,fs,hs)	(((g)->speed==USB_SPEED_HIGH) ? (hs) : (fs))

#else

/* If there's no high speed support, always use the full-speed descriptor. */
#define ep_desc(g,fs,hs)	fs

#endif	/* !CONFIG_USB_GADGET_DUALSPEED */

/* Static strings, in UTF-8 (for simplicity we use only ASCII characters) */
static struct usb_string		strings[] = {	
	{STRING_INTERFACE_0,		"SCSI Transparent Interface 0"},
	{STRING_INTERFACE_1,		"SCSI Transparent Interface 1"},
	{STRING_INTERFACE_2,		"SCSI Transparent Interface 2"},
	{}
};

static struct usb_gadget_strings	stringtab = {
	.language	= 0x0409,		// en-us
	.strings	= strings,
};

static const struct usb_gadget_strings *stringTable[] = {
	(struct usb_gadget_strings *) &stringtab,
	NULL,
};	

/*-------------------------------------------------------------------------*/

/* These routines may be called in process context or in_irq */

/* Caller must hold msd->lock */
static void wakeup_thread(struct interface_dev *interface)
{
	/* Tell the main thread that something has happened */
	interface->thread_wakeup_needed = 1;
	if (interface->thread_task)
		wake_up_process(interface->thread_task);
}


static void raise_exception(struct interface_dev *interface,
					enum msd_state new_state)
{
	struct msd_dev     *msd = the_msd;
	unsigned long      flags;

	/* Do nothing if a higher-priority exception is already in progress.
	 * If a lower-or-equal priority exception is in progress, preempt it
	 * and notify the main thread by sending it a signal. */
	spin_lock_irqsave(&msd->lock, flags);
	if (interface->state <= new_state) {
		interface->exception_req_tag = msd->ep0_req_tag;
		interface->state = new_state;
		if (interface->thread_task)
			send_sig_info(SIGUSR1, SEND_SIG_FORCED, interface->thread_task);
	}
	spin_unlock_irqrestore(&msd->lock, flags);
}


/*-------------------------------------------------------------------------*/

static int do_set_configuration(struct interface_dev *interface, u8 new_config);

/* The disconnect callback and ep0 routines.  These always run in_irq,
 * except that ep0_queue() is called in the main thread to acknowledge
 * completion of various requests: set config, set interface, and
 * Bulk-only device reset. */

static void msd_disconnect(struct usb_composite_dev *cdev)
{
	struct msd_dev *msd = get_composite_data(cdev, function_index);
	struct interface_dev      *interface = NULL;
	int                 i, j;
	struct lun          *curlun;
	
	for (i = 0; i < msd->num_interfaces; i++) {
		interface = &msd->interface[i];
		//raise_exception(interface, MSD_STATE_DISCONNECT);
//		printk(KERN_EMERG "%s() Handling MSD_STATE_DISCONNECT for interface[%d] line %d\n", __FUNCTION__, interface->index, __LINE__);
//		printk(KERN_EMERG "%s() calling do_set_configuration() interface[%d] line %d\n", __FUNCTION__, interface->index, __LINE__);
		do_set_configuration(interface, 0);		// Unconfigured state
				
		/* abort pending read/write operations */
		interface->read_aborted = 1;
		wake_up_interruptible(&interface->read_wait_q);
		
		interface->write_aborted = 1;
		wake_up_interruptible(&interface->write_wait_q);
		
		for (j = 0; j < interface->nluns; j++) {
			curlun = &interface->luns[j];
			curlun->read_pending = 0;
			curlun->amount_left_to_read = 0;
			curlun->write_pending = 0;
			curlun->amount_left_to_write = 0;
		}
		
		wakeup_thread(interface);		
	}
}

static int ep0_queue(struct msd_dev *msd)
{
	int	rc;

	rc = usb_ep_queue(msd->ep0, msd->ep0req, GFP_ATOMIC);
	if (rc != 0 && rc != -ESHUTDOWN) {

		/* We can't do much more than wait for a reset */
		//printk(KERN_DEBUG "%s: error in submission: %s --> %d\n",
		//		__FUNCTION__, msd->ep0->name, rc);
	}
	return rc;
}

static void ep0_complete(struct usb_ep *ep, struct usb_request *req)
{
	if (req->status || req->actual != req->length)
		printk(KERN_ALERT "%s: %d, %u/%u\n", __FUNCTION__,
				req->status, req->actual, req->length);
	if (req->status == -ECONNRESET)		// Request was cancelled
		usb_ep_fifo_flush(ep);
}


/*-------------------------------------------------------------------------*/

/* Bulk and interrupt endpoint completion handlers.
 * These always run in_irq. */

static void bulk_in_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct msd_dev       *msd = the_msd;
	struct interface_dev *interface = (struct interface_dev *)ep->driver_data;
	struct lun           *curlun = interface->curlun;
	struct msd_buffhd    *bh = (struct msd_buffhd *) req->context;

	if (req->status || req->actual != req->length)
		printk(KERN_DEBUG "%s: %d, %u/%u, LUN[%d][%d]\n", __FUNCTION__,
				req->status, req->actual, req->length,
				interface->index, interface->lun);
	if (req->status == -ECONNRESET)		// Request was cancelled
		usb_ep_fifo_flush(ep);

	/* Hold the lock while we update the request and buffer states */
	smp_wmb();
	spin_lock(&msd->lock);
	bh->inreq_busy = 0;
	bh->state = BUF_STATE_EMPTY;

	// Don't wake up main driver thread if read data is pending
	if (curlun && (curlun->read_pending == 0))
		wakeup_thread(interface);
	
	// wake up any user threads waiting on this buffer. 
	wake_up_interruptible(&interface->write_wait_q);	
	spin_unlock(&msd->lock);
}

static void bulk_out_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct msd_dev *msd = the_msd;
	struct interface_dev  *interface = (struct interface_dev *) ep->driver_data;
	struct msd_buffhd   *bh = (struct msd_buffhd *) req->context;

	if (req->status || req->actual != bh->bulk_out_intended_length) {
		printk(KERN_DEBUG "%s: %d, %u/%u, LUN[%d][%d]\n", __FUNCTION__,
				req->status, req->actual,
				bh->bulk_out_intended_length,
				interface->index, interface->lun);
	}
	if (req->status == -ECONNRESET)		// Request was cancelled
		usb_ep_fifo_flush(ep);

	/* Hold the lock while we update the request and buffer states */
	smp_wmb();
	spin_lock(&msd->lock);
	bh->outreq_busy = 0;
	bh->state = BUF_STATE_FULL;
	wakeup_thread(interface);
	// wake up any user threads waiting on this buffer. 
	wake_up_interruptible(&interface->read_wait_q);	
	spin_unlock(&msd->lock);
}

/*-------------------------------------------------------------------------*/

/* Ep0 class-specific handlers.  These always run in_irq. */
static int class_setup_req(struct msd_dev *msd,
		const struct usb_ctrlrequest *ctrl)
{
	struct usb_request	*req;
	int			value = -EOPNOTSUPP;
	u16			w_index;
	u16			w_length;
	u16                     w_value;
	u8			i;
	
	req = msd->ep0req;
	w_index  = le16_to_cpu(ctrl->wIndex);
	w_length = le16_to_cpu(ctrl->wLength);
	w_value  = le16_to_cpu(ctrl->wValue);

	if (!msd->config) {
//		printk(KERN_EMERG "%s() returning EOPNOTSUPP because config is not set %d\n", __FUNCTION__, msd->config);
		return value;
	}

	/* Handle Bulk-only class-specific requests */
	switch (ctrl->bRequest) {

	case 0x00:
	case USB_BULK_RESET_REQUEST:
		if (ctrl->bRequestType != (USB_DIR_OUT |
				USB_TYPE_CLASS | USB_RECIP_INTERFACE))
			break;

		if (ctrl->bRequest == 0x00) {
			if (w_value == 0x00FF) {
				// Kludge for AMI BIOS
				// A 0x00 bRequest with 0x00FF w_value is really
				// a USB_BULK_RESET_REQUEST
				w_value = 0x00;
			} else {
				value = -EDOM;
				break;
			}
		}

		if (w_value != 0 || w_length != 0) {
			value = -EDOM;
			break;
		}

		/* Raise an exception to stop the current operation
		 * and reinitialize our state. */
		//printk(KERN_DEBUG "%s: bulk reset request\n");
		value = -EDOM;
		for (i = 0; i < msd->num_interfaces; i++) {
			if (w_index == intf_desc[i].bInterfaceNumber) {
				
				printk(KERN_DEBUG "%s: USB_BULK_RESET_REQUEST: interface[%d] w_index=%d\n",
				__FUNCTION__, msd->interface[i].index, w_index);
				
				raise_exception(&msd->interface[i],
							MSD_STATE_RESET);
				value = DELAYED_STATUS;
				break;
			}
		}

		break;

	case USB_BULK_GET_MAX_LUN_REQUEST:
		if (ctrl->bRequestType != (USB_DIR_IN |
				USB_TYPE_CLASS | USB_RECIP_INTERFACE)){
//                   printk(KERN_ERR "%s(): GETMAXLUN t = 0x%02X\n", __FUNCTION__, ctrl->bRequestType);
			break;
		}
		if (w_value != 0 || w_length != 1) {
			value = -EDOM;
			break;
		}
		value = -EDOM;
		for (i = 0; i < msd->num_interfaces; i++) {
			if (w_index == intf_desc[i].bInterfaceNumber) {
				*(u8 *)req->buf = msd->interface[i].nluns - 1;
				value = 1;
				break;
			}
		}
		
		printk(KERN_DEBUG "%s: get max LUN: %d for interface[%d], w_index=%d\n",
				__FUNCTION__, msd->interface[i].nluns - 1, i, w_index);
		break;
	}

	if (value == -EOPNOTSUPP)
		printk(KERN_DEBUG "%s: unknown class-specific control req "
			"%02x.%02x v%04x i%04x l%u\n", __FUNCTION__,
			ctrl->bRequestType, ctrl->bRequest,
			le16_to_cpu(ctrl->wValue), w_index, w_length); 

	return value;
}


/*-------------------------------------------------------------------------*/
static int do_set_configuration(struct interface_dev *interface, u8 new_config);

/* Ep0 standard request handlers.  These always run in_irq. */
static int standard_setup_req(struct msd_dev *msd,
		const struct usb_ctrlrequest *ctrl)
{
	struct usb_request	*req = msd->ep0req;
	int			value = -EOPNOTSUPP;
	int			rc;
	u16			w_index;
	u16			w_value;
	u8                      i;
	
	w_index = le16_to_cpu(ctrl->wIndex);
	w_value = le16_to_cpu(ctrl->wValue);

	/* Usually this just stores reply data in the pre-allocated ep0 buffer,
	 * but config change events will also reconfigure hardware. */
	switch (ctrl->bRequest) {
	/* One config, two speeds */
	case USB_REQ_SET_CONFIGURATION:
		if (ctrl->bRequestType != (USB_DIR_OUT | USB_TYPE_STANDARD |
				USB_RECIP_DEVICE))
			break;
			
		printk(KERN_DEBUG "%s(): MSD_STATE_CONFIG_CHANGE config=%d\n",
						__FUNCTION__, w_value);
			
		msd->new_config = w_value;

		// Raise an exception to wipe out previous transaction
		// state (queued bufs, etc) and set the new config.
		for (i = 0; i < msd->num_interfaces; i++) {
			//raise_exception(&msd->interface[i],
			//		MSD_STATE_CONFIG_CHANGE);
	
			msd->interface[i].nluns = msd->interface[i].nlunsReset;
					
			rc = do_set_configuration(&msd->interface[i],
							msd->new_config);
			
			if (rc != 0) {			// STALL on errors
				msd_set_halt(&msd->interface[i], the_msd->ep0);
			}
		}
		value = DELAYED_STATUS;
		break;
	case USB_REQ_GET_CONFIGURATION:
		if (ctrl->bRequestType != (USB_DIR_IN | USB_TYPE_STANDARD |
				USB_RECIP_DEVICE))
			break;
		//printk(KERN_DEBUG "%s: get configuration\n", __FUNCTION__);
		*(u8 *) req->buf = msd->config;
		value = 1;
		break;

	case USB_REQ_SET_INTERFACE:
		if (ctrl->bRequestType != (USB_DIR_OUT| USB_TYPE_STANDARD |
				USB_RECIP_INTERFACE))
			break;

		/* Raise an exception to wipe out previous transaction
		 * state (queued bufs, etc) and install the new
		 * interface altsetting. */
		for (i = 0; i < msd->num_interfaces; i++) {
			if (msd->config && w_index ==
						intf_desc[i].bInterfaceNumber) {
				raise_exception(&msd->interface[i],
						MSD_STATE_INTERFACE_CHANGE);
				value = DELAYED_STATUS;
			}
		}
		break;
	case USB_REQ_GET_INTERFACE:
		if (ctrl->bRequestType != (USB_DIR_IN | USB_TYPE_STANDARD |
				USB_RECIP_INTERFACE))
			break;
		if (!msd->config)
			break;

		//printk(KERN_DEBUG "%s: get interface\n", __FUNCTION__);
		value = -EDOM;
		for (i = 0; i < msd->num_interfaces; i++) {
			if (w_index == intf_desc[i].bInterfaceNumber) {
				*(u8 *) req->buf = intf_desc[i].bInterfaceNumber;
				value = 1;
				break;
			}	
		}
		break;

	default:
		printk(KERN_DEBUG "%s: unknown control req %02x.%02x v%04x"
				" i%04x l%u\n", __FUNCTION__,
			ctrl->bRequestType, ctrl->bRequest,
			w_value, w_index, le16_to_cpu(ctrl->wLength));
	}

	return value;
}


static int msd_setup(struct usb_composite_dev *cdev,
		     const struct usb_ctrlrequest *ctrl)
{
	struct msd_dev *msd = get_composite_data(cdev, function_index);		
	int             rc;
	int             w_length = le16_to_cpu(ctrl->wLength);
	
	++msd->ep0_req_tag;	// Record arrival of a new request
	msd->ep0req->context = NULL;
	msd->ep0req->length = 0;

	if ((ctrl->bRequestType & USB_TYPE_MASK) == USB_TYPE_CLASS) {
//             printk(KERN_ERR "%s(): class_setup_req: bRequestType = 0x%02X ", __FUNCTION__, ctrl->bRequestType);
		rc = class_setup_req(msd, ctrl);	
	} else {
//             printk(KERN_ERR "%s(): standard_setup_req: bRequestType = 0x%02X ", __FUNCTION__, ctrl->bRequestType);
		rc = standard_setup_req(msd, ctrl);
	}

	/* Respond with data/status or defer until later? */
	if (rc >= 0 && rc != DELAYED_STATUS) {
		rc = min(rc, w_length);
		msd->ep0req->length = rc;
		msd->ep0req->zero = rc < w_length;
		msd->ep0req_name = (ctrl->bRequestType & USB_DIR_IN ?
				"ep0-in" : "ep0-out");
		rc = ep0_queue(msd);
	}

	/* Device either stalls (rc < 0) or reports success */
	return rc;
}


/*-------------------------------------------------------------------------*/

/* All the following routines run in process context */


/* Use this for bulk or interrupt transfers, not ep0 */
static void start_transfer(struct interface_dev *interface, struct usb_ep *ep,
		struct usb_request *req, int *pbusy,
		enum msd_buffer_state *state)
{
	struct msd_dev *msd = the_msd;
	int	rc;

	spin_lock_irq(&msd->lock);
	*pbusy = 1;
	*state = BUF_STATE_BUSY;
	spin_unlock_irq(&msd->lock);

	rc = usb_ep_queue(ep, req, GFP_KERNEL);
	if (rc != 0) {
		*pbusy = 0;
		*state = BUF_STATE_EMPTY;

		/* We can't do much more than wait for a reset */

		/* Note: currently the net2280 driver fails zero-length
		 * submissions if DMA is enabled. */
		if (rc != -ESHUTDOWN && !(rc == -EOPNOTSUPP &&
						req->length == 0))
			printk(KERN_DEBUG "%s: error in submission: %s --> %d\n",
				__FUNCTION__, ep->name, rc);
	}
}


static int sleep_thread(struct interface_dev *interface)
{
	int	rc = 0;

	/* Wait until a signal arrives or we are woken up */
	for (;;) {
		try_to_freeze();
		set_current_state(TASK_INTERRUPTIBLE);
		if (signal_pending(current)) {
			rc = -EINTR;
			break;
		}
		if (interface->thread_wakeup_needed)
			break;
		schedule();
	}
	__set_current_state(TASK_RUNNING);
	interface->thread_wakeup_needed = 0;
	return rc;
}


/*-------------------------------------------------------------------------*/
static int halt_bulk_in_endpoint(struct interface_dev *interface);

static void read_timeout(unsigned long _interface)
{
	struct interface_dev *interface = (struct interface_dev *)_interface;
	struct lun             *curlun;
	struct msd_buffhd      *bh;
	int                    i;
	int                    rc;
	

	curlun = interface->curlun;
	// read timed out, stall bulk IN endpoint
	printk(KERN_ALERT "%s: READ10 timed out LUN[%d][%d], stalling %s\n",
			__FUNCTION__, interface->index, interface->lun,
			interface->bulk_in->name);
	curlun->sense_data = SS_UNRECOVERED_READ_ERROR;
	curlun->sense_data_info = curlun->curlba;
	curlun->amount_left_to_read = 0;
	curlun->read_pending = 0;

	/* Cancel all the pending transfers */
	for (i = 0; i < NUM_BUFFERS; ++i) {
		bh = &interface->buffhds[i];
		if (bh->inreq_busy)
			usb_ep_dequeue(interface->bulk_in, bh->inreq);
	}
	
	/* Clear out the controller's fifos */
	if (interface->bulk_in_enabled)
		usb_ep_fifo_flush(interface->bulk_in);
		
	// Stall the endpoint
	rc = usb_ep_set_halt(interface->bulk_in);
	
	if (rc != 0) {
		printk(KERN_ALERT "%s: Could not halt %s\n",
				__FUNCTION__, interface->bulk_in->name);		
	}

	wakeup_thread(interface);
}

static int do_read(struct interface_dev *interface)
{
	struct lun		*curlun = interface->curlun;
	u32			lba;
	int                     rc;

	/* Get the starting Logical Block Address and check that it's
	 * not too big */
	if (interface->cmnd[0] == SC_READ_6)
		lba = (interface->cmnd[1] << 16) | get_be16(&interface->cmnd[2]);
	else {
		lba = get_be32(&interface->cmnd[2]);

		/* We allow DPO (Disable Page Out = don't save data in the
		 * cache) and FUA (Force Unit Access = don't read from the
		 * cache), but we don't implement them. */
		if ((interface->cmnd[1] & ~0x18) != 0) {
			curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
			return -EINVAL;
		}
	}
	if (lba >= curlun->num_blocks) {
		curlun->sense_data = SS_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE;
		return -EINVAL;
	}

	interface->status.start_block     = lba;
	interface->status.num_blocks      = interface->data_size_from_cmnd;
	interface->status.lun             = interface->lun;
	curlun->curlba                    = lba;
	curlun->amount_left_to_read = (interface->status.num_blocks * 
							curlun->block_size);
	curlun->read_pending        = 1;
	interface->write_aborted    = 0;
	
	// set data direction to NONE to skip finish_reply()
	interface->data_dir = DATA_DIR_NONE;

#if 0							
	printk(KERN_ALERT "%s: Start!!! drive[%d][%d], start_block=0x%08X,"
		" num_blocks=%d, pending=%d\n",
		 __FUNCTION__, interface->index,
		 interface->status.lun, interface->status.start_block,
		 interface->status.num_blocks, curlun->read_pending);
#endif
		 
	mod_timer(&interface->read_timer, jiffies + msecs_to_jiffies(5000));
	
	interface->status.flags = MSGUSBDATA_FLAG_READ;

	// Wake up pending thread
	wake_up_interruptible(&interface->status_wait_q);		
	
	// Go to sleep until read is not pending
	while (curlun->read_pending == 1) {
		if ((rc = sleep_thread(interface)) != 0) {
			return rc;
		}
	}
	
#if 0
	printk(KERN_ALERT "%s: Done!!! drive[%d][%d], pending=%d\n",
			__FUNCTION__, interface->index, interface->lun,
			curlun->read_pending);
#endif
	return -EIO;		// No default reply
}


/*-------------------------------------------------------------------------*/
static int do_write(struct interface_dev *interface)
{
	struct lun        *curlun = interface->curlun;
	u32               lba;
	int               rc;	

	if (curlun->ro) {
		//printk(KERN_INFO "%s: Write Protect Error drive[%d][%d]\n",
		//		__FUNCTION__, interface->id, interface->lun);
		curlun->sense_data = SS_WRITE_PROTECTED;
		return -EINVAL;
	}

	/* Get the starting Logical Block Address and check that it's
	 * not too big */
	if (interface->cmnd[0] == SC_WRITE_6)
		lba = (interface->cmnd[1] << 16) | get_be16(&interface->cmnd[2]);
	else {
		lba = get_be32(&interface->cmnd[2]);

		if ((interface->cmnd[1] & ~0x18) != 0) {
			curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
			return -EINVAL;
		}
	}
	if (lba >= curlun->num_blocks) {
		curlun->sense_data = SS_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE;
		return -EINVAL;
	}
			
	// Set up status and notify user thread
	interface->status.start_block      = lba;
	interface->status.num_blocks       = interface->data_size_from_cmnd;
	interface->status.lun              = interface->lun;
	interface->status.block_size       = curlun->block_size;
	curlun->curlba               = lba;
	curlun->amount_left_to_write = (interface->status.num_blocks * 
							curlun->block_size);		
	curlun->write_pending        = 1;
	interface->read_aborted      = 0;
	
#if 0	
	printk(KERN_DEBUG "%s: Start!!! lba = %d, blocks = %d, drive[%d][%d]\n",
				__FUNCTION__, interface->status.start_block,
				interface->status.num_blocks, interface->index,
				interface->lun);
#endif
	
	interface->status.flags = MSGUSBDATA_FLAG_WRITE;

	// Wake up pending thread
	wake_up_interruptible(&interface->status_wait_q);			
		
	// Go to sleep until user thread finishes writing data
	// curlun->write_pending will be set to 0 when the user thread
	// calls ioctl indicating the write is complete
	while (curlun->write_pending == 1) {
		//printk(KERN_DEBUG "%s: sleeping main thread, drive[%d][%d]\n",
		//		__FUNCTION__, interface->id, interface->lun);
		if ((rc = sleep_thread(interface)) != 0)
			return rc;
	}
	
#if 0
	printk(KERN_DEBUG "%s: done!!! drive[%d][%d]\n",
			__FUNCTION__, interface->index, interface->lun);
#endif
	
	return -EIO;		// No default reply
}

/*-------------------------------------------------------------------------*/

static int do_synchronize_cache(struct interface_dev *interface)
{
	struct lun	*curlun = interface->curlun;

	/* We don't support SC_SYNCHRONIZE_CACHE */
	curlun->sense_data = SS_INVALID_COMMAND;
	return -EINVAL;
}

static int do_verify(struct interface_dev *interface)
{
	struct lun	*curlun = interface->curlun;

#if 0
	/* We don't support SC_VERIFY */
	curlun->sense_data = SS_INVALID_COMMAND;
	return -EINVAL;
#else
	// Allow DPO (Disable Page Out = don't save data in the
	// cache) but don't implement it.
        if ((interface->cmnd[1] & ~0x10) != 0) {
                curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
                return -EINVAL;
        }

        return 0;

#endif
}

static int do_inquiry(struct interface_dev *interface, struct msd_buffhd *bh)
{
	u8	     *buf = (u8 *) bh->buf;
	struct lun   *curlun = interface->curlun;
	
	printk(KERN_DEBUG "%s: for LUN[%d][%d]\n", __FUNCTION__, interface->index, interface->lun);

	if (!curlun) {		// Unsupported LUNs are okay
		//printk(KERN_ALERT "%s: Unsupported LUN[%d][%d]\n",
		//	__FUNCTION__, interface->index, interface->lun);
		interface->bad_lun_okay = 1;
		memset(buf, 0, 36);
		buf[0] = 0x7f;		// Unsupported, no device-type
		return 36;
	}
	
	memset(buf, 0, 8);	// Non-removable, direct-access device
	if (curlun->type == USB_DRIVE_TYPE_CD) {
		buf[0] = 0x05;
		buf[1] = 0x80;	// removable device
		buf[2] = 0x00;		// ANSI SCSI level 2
		buf[3] = 0x32; // SCSI-2 INQUIRY data format
		buf[4] = 0x1F;		// Additional length
					// No special options
		sprintf(buf + 8, "%-8s%-16s%04x", curlun->vendor,
					curlun->product, mod_data.release);
	} else {
		buf[0] = 0x00;
		buf[1] = 0x80;  // removable device
		buf[2] = 0x00;		// ANSI SCSI level 2
		buf[3] = 0x01; // SCSI-2 INQUIRY data format
		buf[4] = 0x1F;		// Additional length
					// No special options
		sprintf(buf + 8, "%-8s%-16s%04x", curlun->vendor,
					curlun->product, mod_data.release);
	}
	return 36;
}

static int do_request_sense(struct interface_dev *interface, struct msd_buffhd *bh)
{
	struct lun	*curlun = interface->curlun;
	u8		*buf = (u8 *) bh->buf;
	u32		sd, sdinfo;

	if (curlun && (curlun->media_changed == 1)) {
		curlun->sense_data = SS_NOT_READY_TO_READY_TRANSITION;
		curlun->mounted = 1;
		curlun->media_changed = 0;
	}

	if (!curlun) {		// Unsupported LUNs are okay
		printk(KERN_ALERT "%s: Unsupported LUN[%d][%d]\n",
			__FUNCTION__, interface->index, interface->lun);
		interface->bad_lun_okay = 1;
		sd = SS_LOGICAL_UNIT_NOT_SUPPORTED;
		sdinfo = 0;
	} else {
		sd = curlun->sense_data;
		sdinfo = curlun->sense_data_info;
		curlun->sense_data = SS_NO_SENSE;
		curlun->sense_data_info = 0;
	}

	memset(buf, 0, 18);
	buf[0] = 0x80 | 0x70;			// Valid, current error
	buf[2] = SK(sd);
	put_be32(&buf[3], sdinfo);		// Sense information
	buf[7] = 18 - 8;			// Additional sense length
	buf[12] = ASC(sd);
	buf[13] = ASCQ(sd);
	return 18;
}


static int do_read_capacity(struct interface_dev *interface, struct msd_buffhd *bh)
{
	struct lun	*curlun = interface->curlun;
	u32		lba = get_be32(&interface->cmnd[2]);
	int		pmi = interface->cmnd[8];
	u8		*buf = (u8 *) bh->buf;

	/* Check the PMI and LBA fields */
	if (pmi > 1 || (pmi == 0 && lba != 0)) {
		curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
		return -EINVAL;
	}
	
	put_be32(&buf[0], curlun->num_blocks - 1); // Max logical block
	put_be32(&buf[4], curlun->block_size);      // Block length
	return 8;
}


static int do_mode_sense(struct interface_dev *interface, struct msd_buffhd *bh)
{
	struct lun	*curlun = interface->curlun;
	int		mscmnd = interface->cmnd[0];
	u8		*buf = (u8 *) bh->buf;
	u8		*buf0 = buf;
	int		pc, page_code;
	int		changeable_values, all_pages;
	int		valid_page = 0;
	int		len, limit;

	if ((interface->cmnd[1] & ~0x08) != 0) {		// Mask away DBD
		curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
		return -EINVAL;
	}
	pc = interface->cmnd[2] >> 6;
	page_code = interface->cmnd[2] & 0x3f;
	if (pc == 3) {
		curlun->sense_data = SS_SAVING_PARAMETERS_NOT_SUPPORTED;
		return -EINVAL;
	}
	changeable_values = (pc == 1);
	all_pages = (page_code == 0x3f);

	/*
	 * Write the mode parameter header.  We will fill in
	 * the mode data length later.
	 */
	memset(buf, 0, 8);
	
	if (curlun->type == USB_DRIVE_TYPE_CD) {
		buf[2] = 0x70; /* medium type */
		buf += 8;
		limit = mod_data.buflen;
	
		/* fill in the mode page */
		if (page_code == 0x2A || all_pages) {
			valid_page = 1;
			buf[0] = 0x2A;		// Page code
			buf[1] = 0x1E;		// Page length
			memset(buf+2, 0, 0x1E);	// None of the fields are changeable
			if (!changeable_values) {
				buf[2] = 0x3F;
			}
			
			buf += (0x1E + 2);
		}	
		
		/* Check that a valid page was requested and the mode data length
		 * isn't too long. */
		len = buf - buf0;
		if (!valid_page || len > limit) {
			curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
			return -EINVAL;
		}
	
		/*  Store the mode data length */
		if (mscmnd == SC_MODE_SENSE_6)
			buf0[0] = len - 1;
		else
			put_be16(buf0, len - 2);
	} else {
		if (mscmnd == SC_MODE_SENSE_6) {
			buf[2] = (curlun->ro ? 0x80 : 0x00);	// WP, DPOFUA
			buf += 4;
			limit = 255;
		} else { // SC_MODE_SENSE_10
			buf[3] = (curlun->ro ? 0x80 : 0x00);	// WP, DPOFUA
			buf += 8;
			limit = mod_data.buflen;
		}	
	
		/* fill in the mode page. we only support 0x05*/
		if (page_code == 0x05 || all_pages) {
			valid_page = 1;
			buf[0] = 0x05;		    // Page code
			buf[1] = 0x1E;		    // Page length
			memset(buf+2, 0, 0x1E);	// None of the fields are changeable
			if (!changeable_values) {
				buf[4] = curlun->heads;     // Number or heads
				buf[5] = curlun->sectors;   // Number or heads
				put_be16(&buf[6], 512);      // Bytes per sector
				put_be16(&buf[8], curlun->cylinders); // Number of cylinders
			}
			buf += (0x1E + 2);
		}
		
		/* Check that a valid page was requested and the mode data length
		 * isn't too long. */
		len = buf - buf0;
		if (!valid_page || len > limit) {
			curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
			return -EINVAL;
		}
	
		/*  Store the mode data length */
		if (mscmnd == SC_MODE_SENSE_6)
			buf0[0] = len - 1;
		else
			put_be16(buf0, len - 2);
	}
		
	return len;
}

static int do_start_stop(struct interface_dev *interface)
{
	struct lun	*curlun = interface->curlun;
	u8              immed;
	u8              power_condition;
	u8              load_eject_start;
	
	immed = interface->cmnd[1] & 0x01;
	power_condition = (interface->cmnd[4] & 0xF0) >> 4;
	load_eject_start = interface->cmnd[4] & 0x03;

	switch (power_condition)
	{
	case 0: // No change
		switch (load_eject_start)
		{
		case 2:	// Eject
			if (curlun->prevent_medium_removal) {
				printk(KERN_ERR "%s() Eject attempt"
						" prevented\n", __FUNCTION__);
				curlun->sense_data = SS_MEDIUM_REMOVAL_PREVENTED;
				return -EINVAL;
			} else {
				interface->status.lun      = interface->lun;
				interface->status.flags    = MSGUSBDATA_FLAG_EJECT;
				// Wake up pending thread
				wake_up_interruptible(&interface->status_wait_q);
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return 0;
}


static int do_prevent_allow(struct interface_dev *interface)
{
#if 0
	struct lun	*curlun = interface->curlun;
	int		prevent;

	prevent = interface->cmnd[4] & 0x01;
	if ((interface->cmnd[4] & ~0x01) != 0) {	// Mask away Prevent
		curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
		return -EINVAL;
	}

	curlun->prevent_medium_removal = prevent;
#endif
	// Just return success to send a successfull CSW
	return 0;
}


static int do_read_format_capacities(struct interface_dev *interface,
						struct msd_buffhd *bh)
{
	struct lun	*curlun = interface->curlun;
	u8		*buf = (u8 *) bh->buf;

	buf[0] = buf[1] = buf[2] = 0;
	buf[3] = 8;		// Only the Current/Maximum Capacity Descriptor
	buf += 4;
	
	//put_be32(&buf[0], curlun->num_blocks);	// Number of blocks
	put_be32(&buf[4], 512);	// Block length

	if (curlun->type == USB_DRIVE_TYPE_FD) {
		// Set to 2880 so the drive shows up as a floppy drive in Windows
		put_be32(&buf[0], 2880);	// Number of blocks
		if (lun_is_mounted(curlun)) {
			buf[4] = 0x01;	// Unformatted, Maximum Capacity
		} else {
			buf[4] = 0x03;	// No Cartridge, Maximum Capacity
		}
	} else {
		// Set to 0 so the driver shows up as a removable drive in Windows
		put_be32(&buf[0], 0);	// Number of blocks
		buf[4] = 0x02;		// Formatted, Current capacity
	}

	return 12;
}


static int do_mode_select(struct interface_dev *interface, struct msd_buffhd *bh)
{
	struct lun	*curlun = interface->curlun;

	/* We don't support MODE SELECT */
	curlun->sense_data = SS_INVALID_COMMAND;
	return -EINVAL;
}

static void build_toc_data(struct interface_dev *interface,
				struct scsi_toc_data* toc_data)
{
	struct lun           *curlun = interface->curlun;
	
	struct scsi_toc_data *toc_data_lba = (struct scsi_toc_data*)
				curlun->toc_info.data_lba;

	struct scsi_toc_data *toc_data_msf = (struct scsi_toc_data*)
				curlun->toc_info.data_msf;				
	
	int last_session        = toc_data_lba->last_track;
	int track_session       = interface->cmnd[6];
	u8  format2             = (interface->cmnd[2] & 0x07);
	u8  format9             = (interface->cmnd[9] & 0xC0) >> 6;
	u8  format              = format2;
	u8  msf                 = (interface->cmnd[1] & 0x02) >> 1;
	u16 length              = 0;
	int number_of_sessions;
	struct scsi_toc_track_data* toc_track_data_ptr;
	struct scsi_toc_track_data* vdisk_toc_track_data_ptr;
	int i;

	if(track_session == 0)
		track_session = 1;
	
	if(format == 0)
		format = format9;
	
	if(format == 0) {
		// Calculate length
		number_of_sessions = (last_session - track_session) + 1 + 1;
		length             = (2 + (number_of_sessions * 8));
		
		// Fill out header info
		toc_data->length      = length;
		toc_data->first_track = track_session;
		toc_data->last_track  = last_session;
		
		if(msf == 0)
			vdisk_toc_track_data_ptr =
				&(toc_data_lba->track_data[track_session - 1]);
		else
			vdisk_toc_track_data_ptr =
				&(toc_data_msf->track_data[track_session - 1]);
	} else {  // Multi-session
		// Last track info
		number_of_sessions    = 1;
		
		// Fill out header info
		toc_data->length      = 10;
		toc_data->first_track = track_session;
		toc_data->last_track  = last_session;
		
		if(msf == 0)
			vdisk_toc_track_data_ptr =
				&(toc_data_lba->track_data[last_session - 1]);
		else
			vdisk_toc_track_data_ptr =
				&(toc_data_msf->track_data[last_session - 1]);				
	}
		
	toc_track_data_ptr = toc_data->track_data;
	
	for(i = 0; i < number_of_sessions; i++) {
		memcpy(toc_track_data_ptr, vdisk_toc_track_data_ptr,
					sizeof(struct scsi_toc_track_data));
		toc_track_data_ptr++;
		vdisk_toc_track_data_ptr++;
	}	
}

static int do_read_toc(struct interface_dev *interface, struct msd_buffhd *bh)
{	
	struct lun           *curlun = interface->curlun;
	u8                   *buf = (u8 *) bh->buf;
	struct scsi_toc_data toc_data;
	u32                  length;
	
	// Set up TOC data to be sent to the host.
	memset(&toc_data, 0, sizeof(struct scsi_toc_data));
	build_toc_data(interface, &toc_data);
	
	memcpy(buf, &toc_data, sizeof(struct scsi_toc_data));

	length = min((u32)curlun->toc_info.size, interface->data_size_from_cmnd);
	
#if 0
	{
		u8  format2             = (msd->cmnd[2] & 0x07);
		u8  format9             = (msd->cmnd[9] & 0xC0) >> 6;
		u8  msf                 = (msd->cmnd[1] & 0x02) >> 1;	
		int i;
		u8 *pTocData = (u8*)&toc_data;
		
		printk(KERN_ALERT "%s Toc Data: length = %d\n", __FUNCTION__,
							toc_data.length);
		printk(KERN_ALERT "msf = %d, format9 = %d format2 = %d\n",
					__FUNCTION__, msf, format9, format2);
		
		for (i = 0; i < (2 + toc_data.length); ++i)
		{
			printk(KERN_ALERT "%02X ", pTocData[i]);
			if (((i - 3) % 8) == 0)
				printk(KERN_ALERT "\n");
		}
		printk(KERN_ALERT "\n");
	}
#endif	

	return length;	
}

/*-------------------------------------------------------------------------*/

static int halt_bulk_in_endpoint(struct interface_dev *interface)
{
	int	rc;

	//printk(KERN_DEBUG "%s: \n", __FUNCTION__);

	rc = msd_set_halt(interface, interface->bulk_in);
	//if (rc == -EAGAIN)
		//printk(KERN_DEBUG "%s delayed bulk-in endpoint halt\n",
		//						__FUNCTION__);
	while (rc != 0) {
		if (rc != -EAGAIN) {
			printk(KERN_WARNING "%s: usb_ep_set_halt -> %d\n",
							 __FUNCTION__, rc);
			rc = 0;
			break;
		}

		/* Wait for a short time and then try again */
		if (msleep_interruptible(100) != 0)
			return -EINTR;
		rc = usb_ep_set_halt(interface->bulk_in);
	}
	return rc;
}

static int pad_with_zeros(struct interface_dev *interface)
{
	struct msd_buffhd	*bh = interface->next_buffhd_to_fill;
	u32			nkeep = bh->inreq->length;
	u32			nsend;
	int			rc;

	bh->state = BUF_STATE_EMPTY;		// For the first iteration
	interface->usb_amount_left = nkeep + interface->residue;
	while (interface->usb_amount_left > 0) {

		/* Wait for the next buffer to be free */
		while (bh->state != BUF_STATE_EMPTY) {
			if ((rc = sleep_thread(interface)) != 0)
				return rc;
		}

		nsend = min(interface->usb_amount_left, (u32) mod_data.buflen);
		memset(bh->buf + nkeep, 0, nsend - nkeep);
		bh->inreq->length = nsend;
		bh->inreq->zero = 0;
		start_transfer(interface, interface->bulk_in, bh->inreq,
				&bh->inreq_busy, &bh->state);
		bh = interface->next_buffhd_to_fill = bh->next;
		interface->usb_amount_left -= nsend;
		nkeep = 0;
	}
	return 0;
}

static int throw_away_data(struct interface_dev *interface)
{
	struct msd_buffhd	*bh;
	u32			amount;
	int			rc;

	while ((bh = interface->next_buffhd_to_drain)->state != BUF_STATE_EMPTY
			|| interface->usb_amount_left > 0) {

		/* Throw away the data in a filled buffer */
		if (bh->state == BUF_STATE_FULL) {
			smp_rmb();
			bh->state = BUF_STATE_EMPTY;
			interface->next_buffhd_to_drain = bh->next;

			/* A short packet or an error ends everything */
			if (bh->outreq->actual != bh->outreq->length ||
					bh->outreq->status != 0) {
				raise_exception(interface,
						MSD_STATE_ABORT_BULK_OUT);
				return -EINTR;
			}
			continue;
		}

		/* Try to submit another request if we need one */
		bh = interface->next_buffhd_to_fill;
		if (bh->state == BUF_STATE_EMPTY
					&& interface->usb_amount_left > 0) {
			amount = min(interface->usb_amount_left,
					(u32) mod_data.buflen);

			/* amount is always divisible by 512, hence by
			 * the bulk-out maxpacket size */
			bh->outreq->length = bh->bulk_out_intended_length =
					amount;
			start_transfer(interface, interface->bulk_out,
				bh->outreq, &bh->outreq_busy, &bh->state);
			interface->next_buffhd_to_fill = bh->next;
			interface->usb_amount_left -= amount;
			continue;
		}

		/* Otherwise wait for something to happen */
		if ((rc = sleep_thread(interface)) != 0)
			return rc;
	}
	return 0;
}


static int finish_reply(struct interface_dev *interface)
{
	struct msd_buffhd	*bh = interface->next_buffhd_to_fill;
	int			rc = 0;

	switch (interface->data_dir) {
	case DATA_DIR_NONE:
		break;			// Nothing to send

	/* If we don't know whether the host wants to read or write,
	 * this must be CB or CBI with an unknown command.  We mustn't
	 * try to send or receive any data.  So stall both bulk pipes
	 * if we can and wait for a reset. */
	case DATA_DIR_UNKNOWN:
		if (mod_data.can_stall) {
			//printk(KERN_DEBUG "%s: stalling endpoints\n",
			//			__FUNCTION__);
			
			msd_set_halt(interface, interface->bulk_out);
			rc = halt_bulk_in_endpoint(interface);
		}
		break;

	/* All but the last buffer of data must have already been sent */
	case DATA_DIR_TO_HOST:
		if (interface->data_size == 0)
			;		// Nothing to send

		/* If there's no residue, simply send the last buffer */
		else if (interface->residue == 0) {
			bh->inreq->zero = 0;
			start_transfer(interface, interface->bulk_in, bh->inreq,
					&bh->inreq_busy, &bh->state);
			interface->next_buffhd_to_fill = bh->next;
		}

		/* For Bulk-only, if we're allowed to stall then send the
		 * short packet and halt the bulk-in endpoint.  If we can't
		 * stall, pad out the remaining data with 0's. */
		else {
			if (mod_data.can_stall) {
				bh->inreq->zero = 1;
				start_transfer(interface, interface->bulk_in,
					bh->inreq, &bh->inreq_busy, &bh->state);
				interface->next_buffhd_to_fill = bh->next;
				
				//printk(KERN_DEBUG "%s: finish_reply()" 
				//		" stalling IN endpoints\n");

				rc = halt_bulk_in_endpoint(interface);
			} else
				rc = pad_with_zeros(interface);
		}
		break;

	/* We have processed all we want from the data the host has sent.
	 * There may still be outstanding bulk-out requests. */
	case DATA_DIR_FROM_HOST:
		if (interface->residue == 0)
			;		// Nothing to receive

		/* Did the host stop sending unexpectedly early? */
		else if (interface->short_packet_received) {
			raise_exception(interface, MSD_STATE_ABORT_BULK_OUT);
			rc = -EINTR;
		}

		/* We haven't processed all the incoming data.  Even though
		 * we may be allowed to stall, doing so would cause a race.
		 * The controller may already have ACK'ed all the remaining
		 * bulk-out packets, in which case the host wouldn't see a
		 * STALL.  Not realizing the endpoint was halted, it wouldn't
		 * clear the halt -- leading to problems later on. */
#if 0
		else if (mod_data.can_stall) {
			msd_set_halt(interface, interface->bulk_out);
			raise_exception(interface, MSD_STATE_ABORT_BULK_OUT);
			rc = -EINTR;
		}
#endif

		/* We can't stall.  Read in the excess data and throw it
		 * all away. */
		else
			rc = throw_away_data(interface);
		break;
	}
	return rc;
}


static int send_status(struct interface_dev *interface)
{
	struct lun		*curlun = interface->curlun;
	struct msd_buffhd	*bh;
	int			rc;
	u8			status = USB_STATUS_PASS;
	u32			sd, sdinfo = 0;

	/* Wait for the next buffer to become available */
	bh = interface->next_buffhd_to_fill;
	while (bh->state != BUF_STATE_EMPTY) {
		if ((rc = sleep_thread(interface)) != 0)
			return rc;
	}

	if (curlun) {
		sd = curlun->sense_data;
		sdinfo = curlun->sense_data_info;
	} else if (interface->bad_lun_okay)
		sd = SS_NO_SENSE;
	else
		sd = SS_LOGICAL_UNIT_NOT_SUPPORTED;

	if (interface->phase_error) {
		//printk(KERN_DEBUG "%s: sending phase-error status\n",
		//					__FUNCTION__);
		status = USB_STATUS_PHASE_ERROR;
		sd = SS_INVALID_COMMAND;
	} else if (sd != SS_NO_SENSE) {
		//printk(KERN_DEBUG "%s: sending command-failure status\n",
							//__FUNCTION__);
		status = USB_STATUS_FAIL;
		//printk(KERN_DEBUG "%s: sense data: SK 0x%02X, ASC 0x%02X,"
		//		" ASCQ 0x%02X; info 0x%02X\n", __FUNCTION__,
		//		SK(sd), ASC(sd), ASCQ(sd), sdinfo); 
	}

	{
		struct bulk_cs_wrap	*csw = (struct bulk_cs_wrap *) bh->buf;
		
		/* Store and send the Bulk-only CSW */
		csw->Signature = __constant_cpu_to_le32(USB_BULK_CS_SIG);
		csw->Tag = interface->tag;
		csw->Residue = cpu_to_le32(interface->residue);
		csw->Status = status;
		
		bh->inreq->length = USB_BULK_CS_WRAP_LEN;
		bh->inreq->zero = 0;
		start_transfer(interface, interface->bulk_in, bh->inreq,
				&bh->inreq_busy, &bh->state);
	}
	
	interface->next_buffhd_to_fill = bh->next;
	return 0;
}


/*-------------------------------------------------------------------------*/

/* Check whether the command is properly formed and whether its data size
 * and direction agree with the values we already have. */
static int check_command(struct interface_dev *interface, int cmnd_size,
		enum data_direction data_dir, unsigned int mask,
		int needs_medium, const char *name)
{
	int			i;
	//int			lun = interface->cmnd[1] >> 5;
	static const char	dirletter[4] = {'u', 'o', 'i', 'n'};
	char			hdlen[20];
	struct lun		*curlun;
	
	cmnd_size = interface->cmnd_size;	

	hdlen[0] = 0;
	if (interface->data_dir != DATA_DIR_UNKNOWN)
		sprintf(hdlen, ", H%c=%u", dirletter[(int) interface->data_dir],
				interface->data_size);
				
	//printk(KERN_DEBUG "%s: SCSI command: %s;  Dc=%d, D%c=%u;  Hc=%d%s\n",
	//		__FUNCTION__, name, cmnd_size, 
	//		dirletter[(int) interface->data_dir],
	//		interface->data_size_from_cmnd,
	//              interface->cmnd_size, hdlen);

	/* We can't reply at all until we know the correct data direction
	 * and size. */
	if (interface->data_size_from_cmnd == 0)
		data_dir = DATA_DIR_NONE;
	if (interface->data_dir == DATA_DIR_UNKNOWN) {	// CB or CBI
		interface->data_dir = data_dir;
		interface->data_size = interface->data_size_from_cmnd;

	} else {					// Bulk-only
		if (interface->data_size < interface->data_size_from_cmnd) {

			/* Host data size < Device data size is a phase error.
			 * Carry out the command, but only transfer as much
			 * as we are allowed. */
			interface->data_size_from_cmnd = interface->data_size;
			interface->phase_error = 1;
		}
	}
	
	interface->residue = interface->usb_amount_left = interface->data_size;

	/* Conflicting data directions is a phase error */
	if (interface->data_dir != data_dir
				&& interface->data_size_from_cmnd > 0) {
		interface->phase_error = 1;
		return -EINVAL;
	}

	/* Verify the length of the command itself */
	if (cmnd_size != interface->cmnd_size) {

		/* Special case workaround: MS-Windows issues REQUEST SENSE
		 * with cbw->Length == 12 (it should be 6). */
		if (interface->cmnd[0] == SC_REQUEST_SENSE
					&& interface->cmnd_size == 12)
			cmnd_size = interface->cmnd_size;
		else {
			interface->phase_error = 1;
			return -EINVAL;
		}
	}

	/* Check that the LUN values are consistent */
	//if (interface->lun != lun) 
		//printk(KERN_DEBUG "%s: using LUN %d from CBW, "
		//		"not LUN %d from CDB\n", __FUNCTION__,
		//		interface->lun, lun);

	/* Check the LUN */
	if ((interface->lun >= 0) && (interface->lun < interface->nluns)) {
		interface->curlun = curlun = &interface->luns[interface->lun];
		if (curlun->type == USB_DRIVE_TYPE_NONE ||
				curlun->type == USB_DRIVE_TYPE_DISABLED) {					
			interface->curlun = curlun = NULL;
			interface->bad_lun_okay = 0;
	
			// INQUIRY and REQUEST SENSE commands are explicitly
			// allowed to use disabled LUNs; all others may not.
			if (interface->cmnd[0] != SC_INQUIRY &&
				interface->cmnd[0] != SC_REQUEST_SENSE) {
				//printk(KERN_ALERT "%s: LUN[%d][%d]"
				//		" not enabled cmd=0x%02X\n",
				//		__FUNCTION__, interface->index,
				//	interface->lun, interface->cmnd[0]);
				return -EINVAL;
			}
		} else {
			if (interface->cmnd[0] != SC_REQUEST_SENSE) {
				curlun->sense_data = SS_NO_SENSE;
				curlun->sense_data_info = 0;
			}
		}
	} else {
		interface->curlun = curlun = NULL;
		interface->bad_lun_okay = 0;

		// INQUIRY and REQUEST SENSE commands are explicitly allowed
		// to use unsupported LUNs; all others may not.
		if (interface->cmnd[0] != SC_INQUIRY &&
				interface->cmnd[0] != SC_REQUEST_SENSE) {
			printk(KERN_ALERT "%s: unsupported LUN[%d][%d]\n",
					__FUNCTION__, interface->index,
					interface->lun);
			return -EINVAL;
		}
	}

	/* If a unit attention condition exists, only INQUIRY and
	 * REQUEST SENSE commands are allowed; anything else must fail. */
	if (curlun && curlun->unit_attention_data != SS_NO_SENSE &&
			interface->cmnd[0] != SC_INQUIRY &&
			interface->cmnd[0] != SC_REQUEST_SENSE) {
		curlun->sense_data = curlun->unit_attention_data;
		curlun->unit_attention_data = SS_NO_SENSE;
		return -EINVAL;
	}

	/* Check that only command bytes listed in the mask are non-zero */
	interface->cmnd[1] &= 0x1f;			// Mask away the LUN
	for (i = 1; i < cmnd_size; ++i) {
		if (interface->cmnd[i] && !(mask & (1 << i))) {
			if (curlun)
				curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
			return -EINVAL;
		}
	}

	/* If the medium isn't mounted and the command needs to access
	 * it, return an error. */
	if (curlun && !lun_is_mounted(curlun) && needs_medium) {
		curlun->sense_data = SS_MEDIUM_NOT_PRESENT;
		return -EINVAL;
	}
	
	return 0;
}


static int do_scsi_command(struct interface_dev *interface)
{
	struct msd_buffhd	*bh;
	int			rc;
	int			reply = -EINVAL;
	int			i;
	static char		unknown[16];

	/* Wait for the next buffer to become available for data or status */
	bh = interface->next_buffhd_to_drain = interface->next_buffhd_to_fill;
	while (bh->state != BUF_STATE_EMPTY) {
		if ((rc = sleep_thread(interface)) != 0)
			return rc;
	}
	
	interface->phase_error           = 0;
	interface->short_packet_received = 0;

	switch (interface->cmnd[0]) {

	case SC_INQUIRY:
		interface->data_size_from_cmnd = interface->cmnd[4];
		if ((reply = check_command(interface, 6, DATA_DIR_TO_HOST,
				(1<<4), 0,
				"INQUIRY")) == 0)
			reply = do_inquiry(interface, bh);	
		break;

	case SC_MODE_SELECT_6:
		interface->data_size_from_cmnd = interface->cmnd[4];
		if ((reply = check_command(interface, 6, DATA_DIR_FROM_HOST,
				(1<<1) | (1<<4), 0,
				"MODE SELECT(6)")) == 0)
			reply = do_mode_select(interface, bh);
		break;

	case SC_MODE_SELECT_10:
		interface->data_size_from_cmnd = get_be16(&interface->cmnd[7]);
		if ((reply = check_command(interface, 10, DATA_DIR_FROM_HOST,
				(1<<1) | (3<<7), 0,
				"MODE SELECT(10)")) == 0)
			reply = do_mode_select(interface, bh);
		break;

	case SC_MODE_SENSE_6:
		interface->data_size_from_cmnd = interface->cmnd[4];
		if ((reply = check_command(interface, 6, DATA_DIR_TO_HOST,
				(1<<1) | (1<<2) | (1<<4), 0,
				"MODE SENSE(6)")) == 0) {
			reply = do_mode_sense(interface, bh);
		}
		break;

	case SC_MODE_SENSE_10:
		interface->data_size_from_cmnd = get_be16(&interface->cmnd[7]);
		if ((reply = check_command(interface, 10, DATA_DIR_TO_HOST,
				(1<<1) | (1<<2) | (3<<7), 0,
				"MODE SENSE(10)")) == 0) {
			reply = do_mode_sense(interface, bh);
		}
		break;

	case SC_PREVENT_ALLOW_MEDIUM_REMOVAL:
		interface->data_size_from_cmnd = 0;
		if ((reply = check_command(interface, 6, DATA_DIR_NONE,
				(1<<4), 0,
				"PREVENT-ALLOW MEDIUM REMOVAL")) == 0)
			reply = do_prevent_allow(interface);
		break;

	case SC_READ_6:
		i = interface->cmnd[4];
		interface->data_size_from_cmnd = (i == 0 ? 256 : i);
		if ((reply = check_command(interface, 6, DATA_DIR_TO_HOST,
				(7<<1) | (1<<4), 1,
				"READ(6)")) == 0)
			reply = do_read(interface);			
		break;

	case SC_READ_10:
		interface->data_size_from_cmnd = get_be16(&interface->cmnd[7]);
		if ((reply = check_command(interface, 10, DATA_DIR_TO_HOST,
				(1<<1) | (0xf<<2) | (3<<7), 1,
				"READ(10)")) == 0)
			reply = do_read(interface);			
		break;

	case SC_READ_12:
		interface->data_size_from_cmnd = get_be32(&interface->cmnd[6]);
		if ((reply = check_command(interface, 12, DATA_DIR_TO_HOST,
				(1<<1) | (0xf<<2) | (0xf<<6), 1,
				"READ(12)")) == 0)
			reply = do_read(interface);			
		break;

	case SC_READ_CAPACITY:
		interface->data_size_from_cmnd = 8;
		if ((reply = check_command(interface, 10, DATA_DIR_TO_HOST,
				(0xf<<2) | (1<<8), 1,
				"READ CAPACITY")) == 0)
			reply = do_read_capacity(interface, bh);
		break;

	case SC_READ_FORMAT_CAPACITIES:
		interface->data_size_from_cmnd = get_be16(&interface->cmnd[7]);
		if ((reply = check_command(interface, 10, DATA_DIR_TO_HOST,
				(3<<7), 0, "READ FORMAT CAPACITIES")) == 0)
			reply = do_read_format_capacities(interface, bh);
		break;

	case SC_REQUEST_SENSE:
		interface->data_size_from_cmnd = interface->cmnd[4];
		if ((reply = check_command(interface, 6, DATA_DIR_TO_HOST,
				(1<<4), 0,
				"REQUEST SENSE")) == 0)
			reply = do_request_sense(interface, bh);
		break;

	case SC_START_STOP_UNIT:
		interface->data_size_from_cmnd = 0;
		if ((reply = check_command(interface, 6, DATA_DIR_NONE,
				(1<<1) | (1<<4), 0,
				"START-STOP UNIT")) == 0)
			reply = do_start_stop(interface);
		break;

	case SC_SYNCHRONIZE_CACHE:
		interface->data_size_from_cmnd = 0;
		if ((reply = check_command(interface, 10, DATA_DIR_NONE,
				(0xf<<2) | (3<<7), 1,
				"SYNCHRONIZE CACHE")) == 0)
			reply = do_synchronize_cache(interface);
		break;

	case SC_TEST_UNIT_READY:
		interface->data_size_from_cmnd = 0;
		reply = check_command(interface, 6, DATA_DIR_NONE,
				0, 1,
				"TEST UNIT READY");
		break;

	/* Although optional, this command is used by MS-Windows.  We
	 * support a minimal version: BytChk must be 0. */
	case SC_VERIFY:
		interface->data_size_from_cmnd = 0;
		if ((reply = check_command(interface, 10, DATA_DIR_NONE,
				(1<<1) | (0xf<<2) | (3<<7), 1,
				"VERIFY")) == 0)
			reply = do_verify(interface);
		break;

	case SC_WRITE_6:
		i = interface->cmnd[4];
		interface->data_size_from_cmnd = (i == 0 ? 256 : i);
		if ((reply = check_command(interface, 6, DATA_DIR_FROM_HOST,
				(7<<1) | (1<<4), 1,
				"WRITE(6)")) == 0)
			reply = do_write(interface);
		break;

	case SC_WRITE_10:
		interface->data_size_from_cmnd = get_be16(&interface->cmnd[7]);
		if ((reply = check_command(interface, 10, DATA_DIR_FROM_HOST,
				(1<<1) | (0xf<<2) | (3<<7), 1,
				"WRITE(10)")) == 0)
			reply = do_write(interface);
		break;

	case SC_WRITE_12:
		interface->data_size_from_cmnd = get_be32(&interface->cmnd[6]);
		if ((reply = check_command(interface, 12, DATA_DIR_FROM_HOST,
				(1<<1) | (0xf<<2) | (0xf<<6), 1,
				"WRITE(12)")) == 0)
			reply = do_write(interface);
		break;
		
	case SC_READ_TOC:
		interface->data_size_from_cmnd = get_be16(&interface->cmnd[7]);
		if ((reply = check_command(interface, 12, DATA_DIR_TO_HOST,
				(3<<1) | (0xf<<6), 0,
				"READ TOC")) == 0)
			reply = do_read_toc(interface, bh);		
		break;		

	case SC_READ_DVD_STRUCTURE:
		interface->data_size_from_cmnd = get_be16(&interface->cmnd[8]);
		if ((reply = check_command(interface, 12, DATA_DIR_TO_HOST,
				(1<<1) | (0xf<<2) | (3<<6) | (3<<8) | (3<<10),
				0, "READ DVD")) == 0)
			reply = do_read_toc(interface, bh);		
		break;	
	/* Some mandatory commands that we recognize but don't implement.
	 * They don't mean much in this setting.  It's left as an exercise
	 * for anyone interested to implement RESERVE and RELEASE in terms
	 * of Posix locks. */
	case SC_FORMAT_UNIT:
	case SC_RELEASE:
	case SC_RESERVE:
	case SC_SEND_DIAGNOSTIC:
		// Fall through

	default:
		interface->data_size_from_cmnd = 0;
		sprintf(unknown, "Unknown x%02x", interface->cmnd[0]);
		if ((reply = check_command(interface, interface->cmnd_size,
				DATA_DIR_UNKNOWN, 0xffff, 0, unknown)) == 0) {
			interface->curlun->sense_data = SS_INVALID_COMMAND;
			reply = -EINVAL;
		}
		break;
	}

	if (reply == -EINTR || signal_pending(current))
		return -EINTR;

	/* Set up the single reply buffer for finish_reply() */
	if (reply == -EINVAL)
		reply = 0;		// Error reply length
	if (reply >= 0 && interface->data_dir == DATA_DIR_TO_HOST) {
		reply = min((u32) reply, interface->data_size_from_cmnd);
		bh->inreq->length = reply;
		bh->state = BUF_STATE_FULL;
		interface->residue -= reply;
	}				// Otherwise it's already set

	return 0;
}


/*-------------------------------------------------------------------------*/

static int received_cbw(struct interface_dev *interface, struct msd_buffhd *bh)
{
	struct usb_request	*req = bh->outreq;
	struct bulk_cb_wrap	*cbw = (struct bulk_cb_wrap *) req->buf;

	/* Was this a real packet? */
	if (req->status)
		return -EINVAL;

	/* Is the CBW valid? */
	if (req->actual != USB_BULK_CB_WRAP_LEN ||
			cbw->Signature != __constant_cpu_to_le32(
				USB_BULK_CB_SIG)) {
		//printk(KERN_INFO "%s received_cbw() invalid CBW: len %d,"
		//	" sig 0x%08X should be 0x%08X\n", __FUNCTION__,
		//		req->actual, cbw->Signature,
		//		__constant_cpu_to_le32(USB_BULK_CB_SIG));

		/* The Bulk-only spec says we MUST stall the bulk pipes!
		 * If we want to avoid stalls, set a flag so that we will
		 * clear the endpoint halts at the next reset. */
		if (!mod_data.can_stall)
			set_bit(CLEAR_BULK_HALTS, &interface->atomic_bitflags);
			
		//printk(KERN_INFO "%s: received_cbw()"
		//		" stalling endpoints\n", __FUNCTION__);			
		//msd_set_halt(interface, interface->bulk_out);
		halt_bulk_in_endpoint(interface);
		return -EINVAL;
	}

	/* Is the CBW meaningful? */
	if (cbw->Lun >= MAX_LUNS || cbw->Flags & ~USB_BULK_IN_FLAG ||
			cbw->Length < 6 || cbw->Length > MAX_COMMAND_SIZE) {
		//printk(KERN_DEBUG "%s: non-meaningful CBW: lun = %u,"
		//		" flags = 0x%x, cmdlen %u\n", __FUNCTION__,
		//		cbw->Lun, cbw->Flags, cbw->Length);
				
		/* We can do anything we want here, so let's stall the
		 * bulk pipes if we are allowed to. */
		if (mod_data.can_stall) {
			//printk(KERN_INFO "%s: received_cbw() stalling"
			//			" endpoints\n", __FUNCTION__);
			msd_set_halt(interface, interface->bulk_out);
			halt_bulk_in_endpoint(interface);
		}
		return -EINVAL;
	}

	/* Save the command for later */
	interface->cmnd_size = cbw->Length;
	memcpy(interface->cmnd, cbw->CDB, interface->cmnd_size);
	if (cbw->Flags & USB_BULK_IN_FLAG)
		interface->data_dir = DATA_DIR_TO_HOST;
	else
		interface->data_dir = DATA_DIR_FROM_HOST;
	interface->data_size = le32_to_cpu(cbw->DataTransferLength);
	if (interface->data_size == 0)
		interface->data_dir = DATA_DIR_NONE;
	interface->lun = cbw->Lun;
	interface->tag = cbw->Tag;
	return 0;
}


static int get_next_command(struct interface_dev *interface)
{
	struct msd_buffhd	*bh;
	int			rc = 0;

	/* Wait for the next buffer to become available */
	bh = interface->next_buffhd_to_fill;
	while (bh->state != BUF_STATE_EMPTY) {
		if ((rc = sleep_thread(interface)) != 0)
			return rc;
		}

	/* Queue a request to read a Bulk-only CBW */
	set_bulk_out_req_length(interface, bh, USB_BULK_CB_WRAP_LEN);
	start_transfer(interface, interface->bulk_out, bh->outreq,
			&bh->outreq_busy, &bh->state);

	/* We will drain the buffer in software, which means we
	 * can reuse it for the next filling.  No need to advance
	 * next_buffhd_to_fill. */

	/* Wait for the CBW to arrive or termination */
	while ((bh->state != BUF_STATE_FULL) &&
	       (bh->outreq_busy)) {
		if ((rc = sleep_thread(interface)) != 0)
			return rc;
		}
	smp_rmb();
	rc = received_cbw(interface, bh);
	bh->state = BUF_STATE_EMPTY;

	return rc;
}

/*-------------------------------------------------------------------------*/

static int enable_endpoint(struct interface_dev *interface, struct usb_ep *ep,
		const struct usb_endpoint_descriptor *d)
{
	int	rc;

	ep->driver_data = interface;
	rc = usb_ep_enable(ep, d);
	if (rc)
		printk(KERN_ERR "%s: can't enable %s, result %d\n",
						__FUNCTION__, ep->name, rc);
	return rc;
}

static int alloc_request(struct interface_dev *interface, struct usb_ep *ep,
		struct usb_request **preq)
{
	*preq = usb_ep_alloc_request(ep, GFP_ATOMIC);
	if (*preq)
		return 0;
	printk(KERN_ERR "%s: can't allocate request for %s\n",
							__FUNCTION__, ep->name);
	return -ENOMEM;
}

/*
 * Reset interface setting and re-init endpoint state (toggle etc).
 * Call with altsetting < 0 to disable the interface.  The only other
 * available altsetting is 0, which enables the interface.
 */
static int do_set_interface(struct interface_dev *interface, int altsetting)
{
	int	rc = 0;
	int	i;
	const struct usb_endpoint_descriptor *d = NULL;

	if (interface->running)
		printk(KERN_DEBUG "%s(): reset_interface\n", __FUNCTION__);
reset:
	/* Disable the endpoints */
	if (interface->bulk_in_enabled) {
		usb_ep_disable(interface->bulk_in);
		interface->bulk_in_enabled = 0;
	}
	if (interface->bulk_out_enabled) {
		usb_ep_disable(interface->bulk_out);
		interface->bulk_out_enabled = 0;
	}

	interface->running = 0;
	if (altsetting < 0 || rc != 0)
		return rc;

	//printk(KERN_DEBUG "%s: set interface %d\n", altsetting);
	
	if (intf_info[interface->index].enabled) {
//		printk(KERN_EMERG "%s(): set interface[%d] %d\n", __FUNCTION__, interface->index, altsetting);
		/* Enable the endpoints */
		d = ep_desc(the_msd->gadget, &fs_bulk_in_desc[interface->index],
						&hs_bulk_in_desc[interface->index]);
		if ((rc = enable_endpoint(interface, interface->bulk_in, d)) != 0)
			goto reset;
		interface->bulk_in_enabled = 1;
		printk(KERN_DEBUG "%s: bulk IN ep for interface[%d] enabled\n", __FUNCTION__, interface->index);
	
		d = ep_desc(the_msd->gadget, &fs_bulk_out_desc[interface->index],
						&hs_bulk_out_desc[interface->index]);
		if ((rc = enable_endpoint(interface, interface->bulk_out, d)) != 0)
			goto reset;
		interface->bulk_out_enabled = 1;
		printk(KERN_DEBUG "%s: bulk OUT ep for interface[%d] enabled\n", __FUNCTION__, interface->index);
		
		interface->bulk_out_maxpacket = le16_to_cpu(d->wMaxPacketSize);
	
		interface->running = 1;
	}
	
	for (i = 0; i < interface->nluns; ++i)
		interface->luns[i].unit_attention_data = SS_RESET_OCCURRED;
	return rc;
}


/*
 * Change our operational configuration.  This code must agree with the code
 * that returns config descriptors, and with interface altsetting code.
 *
 * It's also responsible for power management interactions.  Some
 * configurations might not work with our current power sources.
 * For now we just assume the gadget is always self-powered.
 */
static int do_set_config(struct interface_dev *interface, u8 new_config)
{
	int	rc = 0;

	/* Disable the single interface */
	if (interface->config != 0) {
		interface->config = 0;
		the_msd->config   = 0;
//		printk(KERN_EMERG "%s() calling do_set_interface() line %d\n", __FUNCTION__, __LINE__);
		rc = do_set_interface(interface, -1);
	}

	/* Enable the interface */
	if (new_config != 0) {
		interface->config = new_config;
		the_msd->config   = new_config;
//		printk(KERN_EMERG "%s() calling do_set_interface() line %d\n", __FUNCTION__, __LINE__);
		if ((rc = do_set_interface(interface, 0)) != 0) {
			interface->config = 0;	// Reset on errors
			the_msd->config   = 0;	// Reset on errors
		} else {
			char *speed;

			switch (the_msd->gadget->speed) {
			case USB_SPEED_LOW:	speed = "low";	break;
			case USB_SPEED_FULL:	speed = "full";	break;
			case USB_SPEED_HIGH:	speed = "high";	break;
			default: 		speed = "?";	break;
			}
			printk(KERN_DEBUG DRIVER_NAME ": %s speed config #%d"
					" for interface[%d]\n",
					speed, interface->config,
					interface->index);
		}
	}
	
	return rc;
}

static int do_set_configuration(struct interface_dev *interface, u8 new_config)
{
	int			i;
	int			num_active;
	struct msd_buffhd	*bh;
	int			rc;
		
	/* Cancel all the pending transfers */
	for (i = 0; i < NUM_BUFFERS; ++i) {
		bh = &interface->buffhds[i];
		if (bh->inreq_busy) {
			usb_ep_dequeue(interface->bulk_in, bh->inreq);
		}
		if (bh->outreq_busy) {
			usb_ep_dequeue(interface->bulk_out, bh->outreq);
		}
	}

	/* Wait until everything is idle */
	for (;;) {
		num_active = 0;
		for (i = 0; i < NUM_BUFFERS; ++i) {
			bh = &interface->buffhds[i];
			num_active += bh->inreq_busy + bh->outreq_busy;
		}
		
		if (num_active == 0)
			break;
	}

	/* Clear out the controller's fifos */
	if (interface->bulk_in_enabled)
		usb_ep_fifo_flush(interface->bulk_in);
	if (interface->bulk_out_enabled)
		usb_ep_fifo_flush(interface->bulk_out);

	/* Reset the I/O buffer states and pointers, the SCSI
	 * state, and the exception.  Then invoke the handler. */
	spin_lock_irq(&the_msd->lock);

	for (i = 0; i < NUM_BUFFERS; ++i) {
		bh = &interface->buffhds[i];
		bh->state = BUF_STATE_EMPTY;
	}
	
	interface->next_buffhd_to_fill = interface->next_buffhd_to_drain =
			&interface->buffhds[0];
			
	interface->state = MSD_STATE_IDLE;
			
	spin_unlock_irq(&the_msd->lock);
			
//	printk(KERN_EMERG "%s() calling do_set_config() line %d\n", __FUNCTION__, __LINE__);
	rc = do_set_config(interface, new_config);
	
	wakeup_thread(interface);
	
	// status stage handled by composite driver	
	return rc;
}


/*-------------------------------------------------------------------------*/

static void handle_exception(struct interface_dev *interface)
{
	siginfo_t		info;
	int			sig;
	int			i;
	int			num_active;
	struct msd_buffhd	*bh;
	enum msd_state		old_state;
	u8			new_config;
	struct lun		*curlun;
	unsigned int		exception_req_tag;
	int			rc;
	
	/* Clear the existing signals.  Anything but SIGUSR1 is converted
	 * into a high-priority EXIT exception. */
	for (;;) {
		sig = dequeue_signal_lock(current, &interface->thread_signal_mask,
				&info);
		if (!sig)
			break;
		if (sig != SIGUSR1) {
			if (interface->state < MSD_STATE_EXIT)
				//printk(KERN_DEBUG "%s: Main thread"
				//	" for interface[%d]"
				//	" exiting on signal\n", __FUNCTION__,
				//			interface->id);
			raise_exception(interface, MSD_STATE_EXIT);
		}
	}
	
	/* Cancel all the pending transfers */
	for (i = 0; i < NUM_BUFFERS; ++i) {
		bh = &interface->buffhds[i];
		if (bh->inreq_busy) {
			usb_ep_dequeue(interface->bulk_in, bh->inreq);
		}
		if (bh->outreq_busy) {
			usb_ep_dequeue(interface->bulk_out, bh->outreq);
		}
	}

	/* Wait until everything is idle */
	for (;;) {
		num_active = 0;
		for (i = 0; i < NUM_BUFFERS; ++i) {
			bh = &interface->buffhds[i];
			num_active += bh->inreq_busy + bh->outreq_busy;
		}
		if (num_active == 0)
			break;
		if (sleep_thread(interface))
			return;
	}

	/* Clear out the controller's fifos */
	if (interface->bulk_in_enabled)
		usb_ep_fifo_flush(interface->bulk_in);
	if (interface->bulk_out_enabled)
		usb_ep_fifo_flush(interface->bulk_out);

	/* Reset the I/O buffer states and pointers, the SCSI
	 * state, and the exception.  Then invoke the handler. */
	spin_lock_irq(&the_msd->lock);

	for (i = 0; i < NUM_BUFFERS; ++i) {
		bh = &interface->buffhds[i];
		bh->state = BUF_STATE_EMPTY;
	}
	interface->next_buffhd_to_fill = interface->next_buffhd_to_drain =
			&interface->buffhds[0];

	exception_req_tag = interface->exception_req_tag;
	new_config = the_msd->new_config;
	old_state = interface->state;

	if (old_state == MSD_STATE_ABORT_BULK_OUT)
		interface->state = MSD_STATE_STATUS_PHASE;
	else {
		for (i = 0; i < interface->nluns; ++i) {
			curlun = &interface->luns[i];
			curlun->sense_data = curlun->unit_attention_data =
					SS_NO_SENSE;
			curlun->sense_data_info = 0;
		}
		interface->state = MSD_STATE_IDLE;
	}
	spin_unlock_irq(&the_msd->lock);

	/* Carry out any extra actions required for the exception */
	switch (old_state) {
	default:
		break;

	case MSD_STATE_ABORT_BULK_OUT:
		send_status(interface);
		spin_lock_irq(&the_msd->lock);
		if (interface->state == MSD_STATE_STATUS_PHASE)
			interface->state = MSD_STATE_IDLE;
		spin_unlock_irq(&the_msd->lock);
		break;

	case MSD_STATE_RESET:
		/* In case we were forced against our will to halt a
		 * bulk endpoint, clear the halt now.  (The SuperH UDC
		 * requires this.) */
		printk(KERN_ALERT "%s: Handling MSD_STATE_RESET for"
				" interface[%d]\n",
				__FUNCTION__, interface->index);
		if (test_and_clear_bit(CLEAR_BULK_HALTS,
				&interface->atomic_bitflags)) {
			usb_ep_clear_halt(interface->bulk_in);
			usb_ep_clear_halt(interface->bulk_out);
		}

		if (the_msd->ep0_req_tag == exception_req_tag)
			ep0_queue(the_msd); // Complete the status stage

		/* Technically this should go here, but it would only be
		 * a waste of time.  Ditto for the INTERFACE_CHANGE and
		 * CONFIG_CHANGE cases. */
		// for (i = 0; i < msd->nluns; ++i)
		//	msd->luns[i].unit_attention_data = SS_RESET_OCCURRED;
		break;

	case MSD_STATE_INTERFACE_CHANGE:
//		printk(KERN_ALERT "%s: Handling MSD_STATE_INTERFACE_CHANGE for"
//				" interface[%d]\n",
//				__FUNCTION__, interface->index);
//		printk(KERN_EMERG "%s() calling do_set_interface() line %d\n", __FUNCTION__, __LINE__);
		rc = do_set_interface(interface, 0);
		if (the_msd->ep0_req_tag != exception_req_tag)
			break;
		if (rc != 0)			// STALL on errors
			msd_set_halt(interface, the_msd->ep0);
		else				// Complete the status stage
			ep0_queue(the_msd);
		break;

	case MSD_STATE_CONFIG_CHANGE:
		printk(KERN_ALERT "%s: Handling MSD_STATE_CONFIG_CHANGE for"
				" interface[%d]\n",
				__FUNCTION__, interface->index);
//		printk(KERN_EMERG "%s() calling do_set_config() line %d\n", __FUNCTION__, __LINE__);
		rc = do_set_config(interface, new_config);
		if (the_msd->ep0_req_tag != exception_req_tag)
			break;
		if (rc != 0) {			// STALL on errors
			msd_set_halt(interface, the_msd->ep0);
		}
		// status stage handled by composite driver
		break;

	case MSD_STATE_DISCONNECT:
		printk(KERN_ALERT "%s: Handling MSD_STATE_DISCONNECT for"
				" interface[%d]\n",
				__FUNCTION__, interface->index);
//		printk(KERN_EMERG "%s() calling do_set_config() line %d\n", __FUNCTION__, __LINE__);
		do_set_config(interface, 0);		// Unconfigured state
		break;

	case MSD_STATE_EXIT:
	case MSD_STATE_TERMINATED:
//		printk(KERN_EMERG "%s() calling do_set_config() line %d\n", __FUNCTION__, __LINE__);
		do_set_config(interface, 0);		// Free resources
		spin_lock_irq(&the_msd->lock);
		interface->state = MSD_STATE_TERMINATED; // Stop the thread
		spin_unlock_irq(&the_msd->lock);
		break;
	}
}


/*-------------------------------------------------------------------------*/

static int msd_main_thread(void *interface_)
{
	struct interface_dev  *interface = (struct interface_dev *) interface_;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
	/* Allow the thread to be killed by a signal, but set the signal mask
	 * to block everything but INT, TERM, KILL, and USR1. */
	siginitsetinv(&interface->thread_signal_mask, sigmask(SIGINT) |
			sigmask(SIGTERM) | sigmask(SIGKILL) |
			sigmask(SIGUSR1));
	sigprocmask(SIG_SETMASK, &interface->thread_signal_mask, NULL);
#else
	allow_signal(SIGINT);
	allow_signal(SIGTERM);
	allow_signal(SIGKILL);
	allow_signal(SIGUSR1);
	
	/* Allow the thread to be frozen */
	set_freezable();
#endif

	/* Arrange for userspace references to be interpreted as kernel
	 * pointers.  That way we can pass a kernel pointer to a routine
	 * that expects a __user pointer and it will work okay. */
	set_fs(get_ds());
	
	//printk(KERN_DEBUG "%s: Starting thread for interface[%d]\n", 
	//			__FUNCTION__, interface->id);

	/* The main loop */
	while (interface->state != MSD_STATE_TERMINATED) {
		if (exception_in_progress(interface)
					|| signal_pending(current)) {
			handle_exception(interface);
			continue;
		}

		if (!interface->running) {
			sleep_thread(interface);
			continue;
		}
		
		if (get_next_command(interface))
			continue;

		spin_lock_irq(&the_msd->lock);
		if (!exception_in_progress(interface))
			interface->state = MSD_STATE_DATA_PHASE;
		spin_unlock_irq(&the_msd->lock);

		if (do_scsi_command(interface) || finish_reply(interface))
			continue;

		spin_lock_irq(&the_msd->lock);
		if (!exception_in_progress(interface))
			interface->state = MSD_STATE_STATUS_PHASE;
		spin_unlock_irq(&the_msd->lock);

		if (send_status(interface))
			continue;

		spin_lock_irq(&the_msd->lock);
		if (!exception_in_progress(interface))
			interface->state = MSD_STATE_IDLE;
		spin_unlock_irq(&the_msd->lock);
	}

	spin_lock_irq(&the_msd->lock);
	interface->thread_task = NULL;
	spin_unlock_irq(&the_msd->lock);

	/* In case we are exiting because of a signal, unregister the
	 * gadget driver*/
	if (test_and_clear_bit(REGISTERED, &interface->atomic_bitflags) &&
					the_msd->is_registered) {
		the_msd->is_registered = 0;						
		usb_composite_unregister(&msd_driver);
	}	

	/* Let the unbind and cleanup routines know the thread has exited */
	complete_and_exit(&interface->thread_notifier, 0);
}


/*-------------------------------------------------------------------------*/

u8 isReadable(struct interface_dev *interface)
{
	struct msd_buffhd *bh;
	int               rc = 0;
	
	bh = interface->next_buffhd_to_drain;
	
	if (bh->state == BUF_STATE_FULL)
		return 1;
		
	return (rc);
}

ssize_t msdUsbRead(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	u8                      minor_num;
	struct msd_buffhd       *bh;
	struct interface_dev    *interface;
	struct lun              *curlun;
	u32                     num_bytes = 0;
	u32                     bytes_left;
	u32                     byte_count;
	u32                     rVal;

	// Determine which endpoint this call relates to
	minor_num = MINOR(filp->f_dentry->d_inode->i_rdev);
	
	if (minor_num >= the_msd->num_interfaces) {
		printk(KERN_ERR "%s: invalid minor_num (%d)\n", __FUNCTION__,
						minor_num);
		return (-EINVAL);
	}

	interface = &the_msd->interface[minor_num];
	
	curlun = interface->curlun;
	
	bytes_left = count;
	
	while ((bytes_left > 0) && (interface->read_aborted == 0)) {	
	
		// Calculate number of bytes to deal with this iteration
		byte_count = min(bytes_left, mod_data.buflen);
		
		bytes_left -= byte_count;
		
		/* Queue a request for more data from the host */
		bh = interface->next_buffhd_to_fill;
		interface->next_buffhd_to_drain = bh;
		
		bh->outreq->length = bh->bulk_out_intended_length = byte_count;
		start_transfer(interface, interface->bulk_out, bh->outreq,
					&bh->outreq_busy, &bh->state);
		interface->next_buffhd_to_fill = bh->next;	
	
		// wait for transfer to complete
		rVal = wait_event_interruptible(interface->read_wait_q,
					isReadable(interface) == 1);
		if (rVal != 0)
			return (-ERESTARTSYS);
			
		// Return an error if USB is resetting
		if (interface->read_aborted) {
			printk(KERN_ERR "%s: LUN[%d][%d] read aborted for"
					" device reset\n", __FUNCTION__,
					interface->index, interface->lun);				
			return (-1);
		}
			
		bh = interface->next_buffhd_to_drain;
		bh->state = BUF_STATE_EMPTY;
		interface->residue -= bh->outreq->actual;

		/* copy to user */
		rVal = __copy_to_user(&buf[num_bytes], bh->buf,
							bh->outreq->actual);
		
		num_bytes += bh->outreq->actual;
		curlun->amount_left_to_write -= bh->outreq->actual;
	}
	
	//printk(KERN_DEBUG "%s: received %d bytes, %d left\n", __FUNCTION__,
	//			num_bytes, curlun->amount_left_to_write);	
	
	return num_bytes;
}

u8 isWritable(struct interface_dev *interface)
{
	struct msd_buffhd *bh;
	int               rc = 0;
	
	bh = interface->next_buffhd_to_fill;	
	
	if (bh->state == BUF_STATE_EMPTY)
		return 1;
		
	return (rc);
}

ssize_t msdUsbWrite(struct file *filp,
                    const char *buf,
                    size_t count,
                    loff_t *f_pos)
{
	u8 minor_num;
	struct msd_buffhd     *bh;
	struct interface_dev *interface;
	struct lun           *curlun;
	u32                  bytes_left;
	u32                  byte_count;
	u8                   *buf_ptr;
	u32                  rVal;
	
	// Determine which endpoint this call relates to
	minor_num = MINOR(filp->f_dentry->d_inode->i_rdev);
	
	if (minor_num >= the_msd->num_interfaces) {
		printk(KERN_ERR "%s: invalid minor_num (%d)\n",
						__FUNCTION__, minor_num);
		return (-EINVAL);
	}	
	
	interface = &the_msd->interface[minor_num];
	
	curlun = interface->curlun;
			
	bytes_left = count;
	buf_ptr = (u8*)buf;

	//printk(KERN_ALERT "%s: %d bytes on LUN[%d][%d]\n", __FUNCTION__, 
	//				count, interface->index, interface->lun);
	
	if (curlun->amount_left_to_read <= 0) {
		printk(KERN_ERR "%s: LUN[%d][%d] Unexpected write, amount_left"
					" = %d\n", __FUNCTION__,
					interface->index, interface->lun,
					curlun->amount_left_to_read);
		return (-1);
	}
	
	while(bytes_left > 0) {
		// pend until buffer is available
		rVal = wait_event_interruptible(interface->write_wait_q,
				isWritable(interface) == 1);
		if (rVal != 0)
			return (-ERESTARTSYS);
			
		// Return an error if USB is resetting
		if (interface->write_aborted) {
			printk(KERN_ERR "%s: LUN[%d][%d] write aborted for device"
					" reset.\n", __FUNCTION__,
					interface->index, interface->lun);				
			return (-1);
		}
		
		// Calculate number of bytes to deal with this iteration
		byte_count = min(bytes_left, mod_data.buflen);
		
		bh = interface->next_buffhd_to_fill;
				
		/* copy from user */
		rVal = __copy_from_user(bh->buf, buf_ptr, byte_count);
		
		curlun->amount_left_to_read -= byte_count;
		
		interface->residue -= byte_count;
			
		bh->inreq->length = byte_count;
	
		//printk(KERN_ALERT "     sending %d bytes\n", bh->inreq->length);
		/* Send this buffer and go read some more */
		bh->inreq->zero = 0;
		start_transfer(interface, interface->bulk_in, bh->inreq,
				&bh->inreq_busy, &bh->state);
		interface->next_buffhd_to_fill = bh->next;
		
		// Update counters
		bytes_left -= byte_count;
		buf_ptr += byte_count;
		
		//printk(KERN_ALERT "     %d left\n", bytes_left);
	}

	return (count - bytes_left);
}

unsigned int msdUsbPoll(struct file *filp, struct poll_table_struct *wait)
{
	struct msd_dev       *msd = the_msd;
	struct interface_dev *interface;
	u8 minor_num;
	u32 mask = 0;
	
	// Determine which minor number this call relates to
	minor_num = MINOR(filp->f_dentry->d_inode->i_rdev);
	
	if (minor_num >= the_msd->num_interfaces) {
		printk(KERN_ERR "%s: invalid minor_num (%d)\n", __FUNCTION__,
							minor_num);
		return (-EINVAL);
	}
	
	//printk(KERN_DEBUG "%s: called for minor_num %d\n", __FUNCTION__,
	//							minor_num);
	
	interface = &msd->interface[minor_num];
	// Add read and write queues to poll wait table
	poll_wait(filp, &interface->read_wait_q, wait);
	poll_wait(filp, &interface->write_wait_q, wait);
	
	// Check for readability
	if (isReadable(interface))
		mask |= (POLLIN | POLLRDNORM);

	// Check for writeability
	if (isWritable(interface))
		mask |= (POLLOUT | POLLWRNORM);

	//printk(KERN_DEBUG "%s: returned %d\n", __FUNCTION__, mask);
	return (mask);	
}
 
static void build_function_tables(void);

int msdUsbIoctl(struct inode *inode,
                struct file *filp,
                unsigned int cmd,
                unsigned long arg)
{
	u8                     minor_num;
	struct msd_status_data *status_data;
	struct msd_open_data   *open_data;
	struct msd_dev         *msd = the_msd;
	struct interface_dev   *interface;
	struct lun             *curlun;
	u8                     lun;
	u32                    rVal;
	
	// Determine which minor number this call relates to
	minor_num = MINOR(filp->f_dentry->d_inode->i_rdev);
	
	if (minor_num >= msd->num_interfaces) {
		printk(KERN_ERR "%s: invalid minor_num (%d)\n", __FUNCTION__,
								minor_num);
		return (-EINVAL);
	}
	
	switch (cmd)
	{
	case IOCTL_MSD_OPEN:
		open_data = (struct msd_open_data *)arg;
		if (open_data != 0) {
			lun = open_data->lun;			
			interface = &msd->interface[minor_num];
			//printk(KERN_DEBUG "%s: - IOCTL_MSD_OPEN "
			//			"LUN[%d][%d]\n",
			//		__FUNCTION__, interface->index, lun);
			interface->luns[lun].ro = open_data->flags & 0x01;
			interface->luns[lun].num_blocks = open_data->num_blocks; 
			interface->luns[lun].block_size = open_data->block_size;
			interface->luns[lun].cylinders  = open_data->cylinders;
			interface->luns[lun].heads      = open_data->heads;
			interface->luns[lun].sectors    = open_data->sectors;
			
			if (interface->luns[lun].type == USB_DRIVE_TYPE_CD) { 
				interface->luns[lun].toc_info.size =
							open_data->toc_size;
							
				memcpy(interface->luns[lun].toc_info.data_lba,
					open_data->lba_data,
					min(open_data->toc_size,
							(u32)MAX_TOC_SIZE));
			
				memcpy(interface->luns[lun].toc_info.data_msf,
					open_data->msf_data,
					min((u32) open_data->toc_size,
							(u32)MAX_TOC_SIZE));
			}
			
			interface->luns[lun].media_changed = 1;
		}	
		break;	
	case IOCTL_MSD_EJECT:
		lun = arg;
		interface = &msd->interface[minor_num];
		//printk(KERN_DEBUG "%s: - IOCTL_MSD_EJECT "
		//				"LUN[%d][%d]\n",
		//			__FUNCTION__, interface->index, lun);
		interface->luns[lun].mounted       = 0;
		interface->luns[lun].media_changed = 0;	
		break;
	case IOCTL_MSD_STATUS:
		status_data = (struct msd_status_data *)arg;		
		interface     = &msd->interface[minor_num];
		//printk(KERN_DEBUG "%s: - IOCTL_MSD_STATUS interface[%d]\n",
		//				__FUNCTION__, interface->index);
		if(interface->status.flags == 0) {
			// pend on status event
			rVal = wait_event_interruptible(interface->status_wait_q,
						interface->status.flags);
			if (rVal != 0)
				return (-ERESTARTSYS);
		}
		status_data->flags       = interface->status.flags;
		interface->status.flags  = 0;
		status_data->start_block = interface->status.start_block;
		status_data->num_blocks  = interface->status.num_blocks;
		status_data->block_size  = interface->status.block_size;
		status_data->lun         = interface->status.lun;	
		break;
	case IOCTL_MSD_RESET:
		//printk(KERN_DEBUG "%s- IOCTL_MSD_RESET\n", __FUNCTION__);
		usb_gadget_disconnect(msd->gadget);
		build_function_tables();
		if (usb_composite_reconfigure_interface_ids(the_msd->c_dev)
									 > 0) {
			usb_gadget_connect(msd->gadget);
		}
		break;
	case IOCTL_MSD_SET_LUN_TYPES:
	{
		struct msd_type_data *type_data = (struct msd_type_data*)arg;
		u8 i,j;
		
		interface = &msd->interface[minor_num];
		interface->nlunsReset = 0;
#if 0		
		printk(KERN_DEBUG "%s: IOCTL_MSD_SET_LUN_TYPES for"
			" interface[%d]\n", __FUNCTION__, interface->index);
		for (i = 1; i <= MAX_LUNS; i++) {
			printk(KERN_DEBUG "0x%02X[%d] ", type_data->types[i-1],
						type_data->preventRemoval[i-1]);
			if (i%8 == 0)
				printk(KERN_DEBUG "\n");
		}
		printk(KERN_DEBUG "\n");
#endif
		
		for (i = 0; i < MAX_LUNS; i++) {
			if(type_data->types[i] == USB_DRIVE_TYPE_NONE) {
				if(interface->luns[i].type ==
						USB_DRIVE_TYPE_DISABLED) {
					for (j = i; j <= MAX_LUNS; j++) {
						interface->luns[j].type =
							USB_DRIVE_TYPE_DISABLED;
					}
					break;
				}
			} else if (type_data->types[i]
						== USB_DRIVE_TYPE_DISABLED) {
				for (j = i; j <= MAX_LUNS; j++) {
					interface->luns[j].type =
							USB_DRIVE_TYPE_DISABLED;
				}
				break;
			} else {			
				interface->luns[i].type = type_data->types[i];
			}
			
			interface->luns[i].prevent_medium_removal =
					type_data->preventRemoval[i];
		}
		
		interface->nlunsReset = i;
		
		if (interface->nlunsReset == 0)
			intf_info[interface->index].enabled = 0;  // disabled
		else
			intf_info[interface->index].enabled = 1;  // enabled
#if 0
		printk(KERN_DEBUG "%s: %d LUNs for interface[%d]\n",
			__FUNCTION__,interface->nlunsReset, interface->index);
		for (i = 1; i <= MAX_LUNS; i++) {
			printk(KERN_DEBUG "0x%02X[%d] ", interface->luns[i-1].type,
					interface->luns[i-1].prevent_medium_removal);
			if (i%8 == 0)
				printk(KERN_DEBUG "\n");
		}
		printk(KERN_DEBUG "\n");
		printk(KERN_DEBUG "\n");
#endif
		break;
	}		
	case IOCTL_MSD_GET_LUN_TYPES:
	{
		struct msd_type_data *type_data = (struct msd_type_data*)arg;
		u8 i;
		
		interface = &msd->interface[minor_num];

		//printk(KERN_DEBUG "%s: - IOCTL_MSD_GET_LUN_TYPES interface[%d]\n",
		//				__FUNCTION__, interface->index);
								
		for (i = 0; i < MAX_LUNS; i++) {
			type_data->types[i] = interface->luns[i].type;
			type_data->preventRemoval[i] =
				interface->luns[i].prevent_medium_removal;
		}
		
		break;
	}
	case IOCTL_MSD_SET_INQUIRY_DATA:
	{
		u32 rVal;
		struct msd_inquiry_data *inquiry_data =
					(struct msd_inquiry_data*)arg;

		interface = &msd->interface[minor_num];
		
		//printk(KERN_DEBUG "%s: - IOCTL_MSD_SET_INQUIRY_DATA"
		//		" interface[%d][%d]\n",
		//		__FUNCTION__, interface->index,
		//		inquiry_data->lun);
		if (inquiry_data->lun < MAX_LUNS) {
			rVal = __copy_from_user(
				interface->luns[inquiry_data->lun].product,
				inquiry_data->productId, 16);
		}
		
		break;
	}	
	case IOCTL_MSD_WRITE_COMPLETE:
		interface = &msd->interface[minor_num];
		curlun = interface->curlun;
		//printk(KERN_ALERT "%s: IOCTL_MSD_WRITE_COMPLETE LUN[%d][%d]\n",
		//	__FUNCTION__, interface->index, interface->lun);
		curlun->write_pending = 0;
		if (interface->read_aborted == 0) {
			wakeup_thread(interface);
		} else {

			printk(KERN_ERR "%s: WRITE_COMPLETE LUN[%d][%d]"
					" aborted for device reset.\n",
					__FUNCTION__, interface->index,
					interface->lun);
		}
		break;
	case IOCTL_MSD_READ_COMPLETE:
		interface = &msd->interface[minor_num];
		curlun = interface->curlun;
		del_timer(&interface->read_timer);
		if (curlun->amount_left_to_read == 0) {
			//printk(KERN_ALERT "%s: READ_COMPLETE LUN[%d][%d]\n",
			//	__FUNCTION__, interface->index, interface->lun);
			curlun->read_pending = 0;
			if (interface->write_aborted == 0) {
				wakeup_thread(interface);
			} else {
				printk(KERN_ERR "%s: READ_COMPLETE LUN[%d][%d]"
					" aborted for device reset.\n",
					__FUNCTION__, interface->index,
					interface->lun);
			}
		}
		else {
			printk(KERN_ALERT "%s:"
				" - Unexpected READ_COMPLETE LUN[%d][%d]"
				" amount left to read = %d\n",
					__FUNCTION__,
					interface->index, interface->lun,
					curlun->amount_left_to_read);
			curlun->sense_data = SS_UNRECOVERED_READ_ERROR;
			curlun->sense_data_info = curlun->curlba;
			curlun->amount_left_to_read = 0;
			curlun->read_pending = 0;
			halt_bulk_in_endpoint(interface);
			wakeup_thread(interface);				
		}
		break;
	case IOCTL_MSD_WRITE_ERROR:
		interface = &msd->interface[minor_num];
		printk(KERN_ALERT "%s: - IOCTL_MSD_WRITE_ERROR LUN[%d][%d]\n",
					__FUNCTION__,
					interface->index, interface->lun);		
		curlun = interface->curlun;
		curlun->sense_data = SS_WRITE_ERROR;
		curlun->sense_data_info = curlun->curlba;
		curlun->amount_left_to_write = 0;
		curlun->write_pending = 0;
		msd_set_halt(interface, interface->bulk_out);
		wakeup_thread(interface);
		break;
	case IOCTL_MSD_READ_ERROR:
		interface = &msd->interface[minor_num];
		printk(KERN_ALERT "%s: - IOCTL_MSD_READ_ERROR LUN[%d][%d]\n",
				__FUNCTION__, interface->index, interface->lun);
		curlun = interface->curlun;
		curlun->sense_data = SS_UNRECOVERED_READ_ERROR;
		curlun->sense_data_info = curlun->curlba;
		curlun->amount_left_to_read = 0;
		curlun->read_pending = 0;
		halt_bulk_in_endpoint(interface);
		wakeup_thread(interface);
		break;
	default:
		printk(KERN_ERR "%s: Bad Command %d\n", __FUNCTION__, cmd);
		return (-ENOTTY);
	}
	return (0);
}

int msdUsbOpen(struct inode *inode, struct file *filp)
{
	u8 minor_num;
	
	// Determine which minor number this call relates to
	minor_num = MINOR(filp->f_dentry->d_inode->i_rdev);

	// Check for invalid interface ID
	if (minor_num >= the_msd->num_interfaces) {
		printk(KERN_ERR "open() called for invalid minor_num %d\n",
						minor_num);
		return (-EINVAL);
	}
	
	//printk(KERN_DEBUG "open() called for minor_num %d...\n",
	//			minor_num);
	
	return (0);
}

int msdUsbRelease(struct inode *inode, struct file *filp)
{
	u8                     minor_num;
	u8                     lun;
	struct msd_dev         *msd = the_msd;
	struct interface_dev   *interface;
	struct lun             *curlun;
	
	// Determine which minor number this call relates to
	minor_num = MINOR(filp->f_dentry->d_inode->i_rdev);
	
	printk(KERN_DEBUG "%s() for interface[%d]\n", __FUNCTION__, minor_num);

	// Check for invalid endpoint ID
	if (minor_num >= the_msd->num_interfaces) {
		printk(KERN_ERR "%s: called for invalid minor_num %d\n",
					__FUNCTION__, minor_num);
		return (-EINVAL);
	}
	
	// Make sure everything is Unmounted
	interface = &msd->interface[minor_num];
	
	for (lun = 0; lun < MAX_LUNS; lun++) {
		curlun = &interface->luns[lun];
		
		// Make sure luns are unmounted
		curlun->mounted       = 0;
		curlun->media_changed = 0;
		
		// Disable luns
		curlun->ro            = 1;
		curlun->read_pending  = 0;
		curlun->type 	      = USB_DRIVE_TYPE_DISABLED;		
	}
	
#if 1
	// Disable interface
	interface->nlunsReset = 0;
	intf_info[interface->index].enabled = 0;  // disabled
	
	// Reset the USB
	usb_gadget_disconnect(msd->gadget);
	build_function_tables();
	if (usb_composite_reconfigure_interface_ids(the_msd->c_dev) > 0) {
		usb_gadget_connect(msd->gadget);
	}
#endif
	return (0);
}

struct file_operations msd_fops =
{
	llseek:		NULL,
	read:		msdUsbRead,
	write:		msdUsbWrite,
	poll:		msdUsbPoll,
	ioctl:		msdUsbIoctl,
	open:		msdUsbOpen,
	release:	msdUsbRelease,
};

/*-------------------------------------------------------------------------*/

static void msd_unbind(struct usb_composite_dev *cdev)
{
	struct msd_dev *msd = get_composite_data(cdev, function_index);
	struct interface_dev      *interface;		
	int                       i, j;
	struct usb_request        *req;
	
	if (!msd)
		return;


	for (i = 0; i < msd->num_interfaces; i++) {
		interface = &msd->interface[i];
		clear_bit(REGISTERED, &interface->atomic_bitflags);
		
		// If the thread isn't already dead, tell it to exit now
		if (interface->state != MSD_STATE_TERMINATED) {
			raise_exception(interface, MSD_STATE_EXIT);
			//wait_for_completion(&interface->thread_notifier);
			// The cleanup routine waits for this completion also
			complete(&interface->thread_notifier);
		}
		
		// Deallocate the requests
		for (j = 0; j < NUM_BUFFERS; ++j) {
			struct msd_buffhd *bh = &interface->buffhds[j];
	
			if (bh->inreq) {
				usb_ep_free_request(interface->bulk_in,
								bh->inreq);
				bh->inreq = NULL;
			}
			if (bh->outreq) {
				usb_ep_free_request(interface->bulk_out,
								bh->outreq);
				bh->outreq = NULL;
			}
		}
		
		/* Free the data buffers */
		for (j = 0; j < NUM_BUFFERS; ++j) {
			struct msd_buffhd	*bh = &interface->buffhds[j];
	
			if (bh->buf) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
				usb_ep_free_buffer(interface->bulk_in, bh->buf,
						bh->dma, mod_data.buflen);
#else
				_usb_ep_free_buffer(msd->gadget,
						interface->bulk_in, bh->buf,
						bh->dma, mod_data.buflen);
#endif
			}
		}
		
		del_timer_sync(&interface->read_timer);
		
		/* Free the luns */
		if (interface->luns)
			kfree(interface->luns);
	}

	
	req = msd->ep0req;

	// Free the request and buffer for endpoint 0
	if (req) {
		if (req->buf) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
			usb_ep_free_buffer(msd->ep0, req->buf,
					req->dma, EP0_BUFSIZE);
#else
			_usb_ep_free_buffer(msd->gadget, msd->ep0,
					 req->buf, req->dma, EP0_BUFSIZE);
#endif
		}
		usb_ep_free_request(msd->ep0, req);
	}
	set_composite_data(cdev, function_index, NULL);
	
	if (msd)
		kfree(msd);
}


static int __init check_parameters(struct msd_dev *msd)
{
	int	gcnum;

	if (gadget_is_sh(msd->gadget))
		mod_data.can_stall = 0;

	if (mod_data.release == 0xffff) {	// Parameter wasn't set
		/* The sa1100 controller is not supported */
		if (gadget_is_sa1100(msd->gadget))
			gcnum = -1;
		else
			gcnum = usb_gadget_controller_number(msd->gadget);
		if (gcnum >= 0)
			mod_data.release = 0x0300 + gcnum;
		else {
			printk(KERN_WARNING "%s: controller '%s' not recognized\n",
					__FUNCTION__, msd->gadget->name);
			mod_data.release = 0x0399;
		}
	}

	return 0;
}

static void build_function_tables(void)
{
	u8     i;
	struct usb_descriptor_header **fs_ptr;
	struct usb_descriptor_header **hs_ptr;
	
	fs_ptr = &fs_function[0];
	hs_ptr = &hs_function[0];
	
	for (i = 0; i < the_msd->num_interfaces; i++) {
		
		if (intf_info[i].enabled) {
			// Start after the OTG descriptor
			fs_ptr++;
			hs_ptr++;

			// Add interface desctiptors
			*fs_ptr = (struct usb_descriptor_header *) &intf_desc[i];
			*hs_ptr = (struct usb_descriptor_header *) &intf_desc[i];
		
			// Add IN endpoint descriptors
			fs_ptr++;
			hs_ptr++;
			*fs_ptr = (struct usb_descriptor_header *)
							&fs_bulk_in_desc[i];
			*hs_ptr = (struct usb_descriptor_header *)
							&hs_bulk_in_desc[i];
			
			// Add OUT endpoint descriptors
			fs_ptr++;
			hs_ptr++;
			*fs_ptr = (struct usb_descriptor_header *)
							&fs_bulk_out_desc[i];
			*hs_ptr = (struct usb_descriptor_header *)
							&hs_bulk_out_desc[i];
		}
	}
	
	fs_ptr++;
	hs_ptr++;
	*fs_ptr = NULL;
	*hs_ptr = NULL;
}

static int set_string_ids(struct usb_composite_dev *cdev)
{
	struct usb_string *s;
	int               string_id = -EINVAL;
	
	for (s = stringtab.strings; (s && (s->id > 0)); s++)
	{
		string_id = usb_composite_string_id(cdev);
		
		if (string_id < 0)
			break;
		
		switch (s->id)
		{
			case STRING_INTERFACE_0:
				s->id = string_id;
				intf_desc[0].iInterface = string_id;
				break;
			case STRING_INTERFACE_1:
				s->id = string_id;
				intf_desc[1].iInterface = string_id;
				break;
			case STRING_INTERFACE_2:
				s->id = string_id;
				intf_desc[2].iInterface = string_id;
				break;
			default:
				return -EINVAL;
		}
	}
	
	return string_id;
}

static int create_luns(struct usb_composite_dev *cdev,
				struct interface_dev *interface, int num_luns)
{
	int			i;
	int                     rc = 0;
	struct lun		*curlun;
	struct usb_gadget	*gadget = cdev->gadget;	

	
	/* Create the LUNs,and register the
	 * LUN devices in sysfs. */
	interface->luns = kzalloc(MAX_LUNS * sizeof(struct lun), GFP_KERNEL);
	if (!interface->luns) {
		return -ENOMEM;
	}

	interface->nluns = interface->nlunsReset = num_luns;

	for (i = 0; i < MAX_LUNS; ++i) {
		curlun = &interface->luns[i];
		
		curlun->ro = 1;
		curlun->read_pending = 0;
		curlun->dev.parent = &gadget->dev;
		curlun->dev.driver = &msd_driver.driver;
		curlun->type 	   = USB_DRIVE_TYPE_DISABLED;
		curlun->num_blocks = 0;
		curlun->block_size = 0;
		curlun->cylinders  = 0;
		curlun->heads      = 0;
		curlun->sectors    = 0;
		
		// Initialize vendor and product ids for inquiry
		strlcpy(curlun->vendor, mod_data.vendor,
					min(sizeof(curlun->vendor),
					strlen(mod_data.vendor)+1));
		strlcpy(curlun->product, mod_data.product,
					min(sizeof(curlun->product),
					strlen(mod_data.product)+1));		
	}
	
	return rc;	
}

static int allocate_data_buffs(struct interface_dev *interface)
{
	int i;
	
	/* Allocate the data buffers */
	for (i = 0; i < NUM_BUFFERS; ++i) {
		struct msd_buffhd *bh = &interface->buffhds[i];

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
		bh->buf = usb_ep_alloc_buffer(interface->bulk_in,
						mod_data.buflen,
						&bh->dma, GFP_KERNEL);
#else
		bh->buf = _usb_ep_alloc_buffer(the_msd->gadget,
						interface->bulk_in,
						mod_data.buflen,
						&bh->dma, GFP_KERNEL);
#endif
		if (!bh->buf)
			return -ENOMEM;
			
		bh->next = bh + 1;
	}
	
	interface->buffhds[NUM_BUFFERS - 1].next = &interface->buffhds[0];
	
	return 0;	
}

static int msd_start_thread(struct interface_dev *interface)
{
	int rc = 0;
	
	interface->thread_task = kthread_create(msd_main_thread, interface,
			"mass-storage-function");
	if (IS_ERR(interface->thread_task)) {
		rc = PTR_ERR(interface->thread_task);
		return rc;
	}

	set_bit(REGISTERED, &interface->atomic_bitflags);

	/* Tell the thread to start working */
	wake_up_process(interface->thread_task);
	
	return rc;
}

static int __init msd_bind(struct usb_composite_dev *cdev)
{
	struct msd_dev	        *msd = the_msd;
	struct interface_dev	*interface;
	int			rc;
	struct usb_ep		*ep;
	struct usb_request	*req;
	struct usb_gadget	*gadget = cdev->gadget;
	int			interface_id;
	int                     i, j;

	// Get function index
	function_index = usb_composite_function_index(cdev);
	if (function_index < 0) {
		printk(KERN_ERR "%s: failed getting function index\n", 
							__FUNCTION__);
		rc = -EINVAL;
		goto out;		
	}
	
	msd->c_dev   = cdev;
	msd->gadget = gadget;
	set_composite_data(cdev, function_index, msd);
	msd->ep0 = gadget->ep0;
	msd->ep0->driver_data = msd;

	if ((rc = check_parameters(msd)) != 0)
		goto out;
			
	for (i = 0; i < msd->num_interfaces; i++) {
		interface = &msd->interface[i];
		
		interface->running = 0;

		rc = create_luns(cdev, interface, mod_data.num_luns);
		if (rc != 0)
			goto out;
			
		// init wait queues
		init_waitqueue_head(&interface->status_wait_q);
		init_waitqueue_head(&interface->write_wait_q);
		init_waitqueue_head(&interface->read_wait_q);
		
		// init read timeout timer
		init_timer(&interface->read_timer);
		interface->read_timer.function = read_timeout;
		interface->read_timer.data = (unsigned long)interface;
		
		// Find all the endpoints we will use
		// Bulk IN
		ep = usb_ep_autoconfig(gadget, &fs_bulk_in_desc[i]);
	
		if (!ep)
			goto autoconf_fail;
			
		// claim the endpoint
		ep->driver_data = interface;
		interface->bulk_in = ep;
		
		// Bulk OUT
		ep = usb_ep_autoconfig(gadget, &fs_bulk_out_desc[i]);
	
		if (!ep)
			goto autoconf_fail;

		// claim the endpoint
		ep->driver_data = interface;
		interface->bulk_out = ep;
		
		// Get Interface Number from the composite driver
		// and set it in the interface descriptor for this interface
		interface_id = usb_composite_interface_id(cdev, &intf_info[i]);
		if (interface_id >= 0)
			intf_desc[i].bInterfaceNumber = interface_id;
		else {
			printk(KERN_ERR "%s: invalid interface number %d\n",
						__FUNCTION__, interface_id);
			rc = -EINVAL;
			goto out;
		}
		
#ifdef CONFIG_USB_GADGET_DUALSPEED
		// Assume that all endpoint addresses are 
		// the same for both speeds
		hs_bulk_in_desc[i].bEndpointAddress =
					fs_bulk_in_desc[i].bEndpointAddress;
		hs_bulk_out_desc[i].bEndpointAddress =
					fs_bulk_out_desc[i].bEndpointAddress;
#endif

		// Allocate data buffers
		rc = allocate_data_buffs(interface);
		if (rc != 0)
			goto out;
			
		/* Allocate the requests */
		for (j = 0; j < NUM_BUFFERS; ++j) {
			struct msd_buffhd	*bh = &interface->buffhds[j];
	
			if ((rc = alloc_request(interface,
					interface->bulk_in, &bh->inreq)) != 0)
				goto out;
				
			if ((rc = alloc_request(interface,
					interface->bulk_out, &bh->outreq)) != 0)
				goto out;
				
			bh->inreq->buf = bh->outreq->buf = bh->buf;
			bh->inreq->dma = bh->outreq->dma = bh->dma;
			bh->inreq->context = bh->outreq->context = bh;
			bh->inreq->complete = bulk_in_complete;
			bh->outreq->complete = bulk_out_complete;
		}	
	}

	// Build function tables
	build_function_tables();
	
	// Set String IDs
	if (set_string_ids(cdev) < 0) {
		printk(KERN_ERR "%s: failed setting string IDs\n", __FUNCTION__);
		rc = -EINVAL;
		goto out;		
	}	
		
	rc = -ENOMEM;
	/* Allocate the request and buffer for endpoint 0 */
	msd->ep0req = req = usb_ep_alloc_request(msd->ep0, GFP_KERNEL);
	
	if (!req)
		goto out;
		
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
	req->buf = usb_ep_alloc_buffer(msd->ep0, EP0_BUFSIZE,
						&req->dma, GFP_KERNEL);
#else
	req->buf = _usb_ep_alloc_buffer(msd->gadget, msd->ep0,
			 		EP0_BUFSIZE, &req->dma, GFP_KERNEL);
#endif
	if (!req->buf)
		goto out;
		
	req->complete = ep0_complete;

	/* This should reflect the actual gadget power source */
	usb_gadget_set_selfpowered(gadget);

	// Get major number
	rc = alloc_chrdev_region(&msd->chrdev, 0, MAX_INTERFACES, DRIVER_NAME); 
    	if (rc < 0) {
    		printk(KERN_ERR "%s: Failed to obtain major number\n",
    							__FUNCTION__);
		goto out;
	}
	
	printk(KERN_ALERT "%s: Using Major Number %d\n", __FUNCTION__,
						MAJOR(msd->chrdev));
	
	// Register char device					
	cdev_init(&msd->cdev, &msd_fops);
	msd->cdev.owner = THIS_MODULE;
	msd->cdev.ops   = &msd_fops;
	rc = cdev_add(&msd->cdev, msd->chrdev, MAX_INTERFACES); 
	if (rc < 0) {
		printk(KERN_ERR "%s: Failed to register char device\n",
								__FUNCTION__); 
		goto out;
	}
	
	// Start threads
	for (i = 0; i < msd->num_interfaces; i++) {
		interface = &msd->interface[i];
		rc = msd_start_thread(interface);
		if (rc != 0)
			goto out;
	}

	return 0;

autoconf_fail:
	printk(KERN_ERR "%s:unable to autoconfigure all endpoints\n",
								__FUNCTION__); 
	rc = -ENOTSUPP;

out:

	for (i = 0; i < msd->num_interfaces; i++) {
		interface = &msd->interface[i];
		interface->state = MSD_STATE_TERMINATED; // The thread is dead
	}

	msd_unbind(cdev);
	return rc;
}


/*-------------------------------------------------------------------------*/

static void msd_suspend(struct usb_composite_dev *cdev)
{
	struct msd_dev *msd = get_composite_data(cdev, function_index);		
	u8 i;
	
	for (i = 0; i < msd->num_interfaces; i++) {
		set_bit(SUSPENDED, &msd->interface[i].atomic_bitflags);
	}
}

static void msd_resume(struct usb_composite_dev *cdev)
{
	struct msd_dev *msd = get_composite_data(cdev, function_index);
	u8 i;
	
	for (i = 0; i < msd->num_interfaces; i++) {
		clear_bit(SUSPENDED, &msd->interface[i].atomic_bitflags);
	}
}


/*-------------------------------------------------------------------------*/

static struct usb_function msd_driver = {
	.name             = (char *) shortname,
	.controller_name  = controller,
	.strings          = stringTable,
	.descriptors      = (const struct usb_descriptor_header **)fs_function,
	.hs_descriptors   = (const struct usb_descriptor_header **)hs_function,
	.intf_info_table  = intf_info_table,
	.bind             = msd_bind,
	.unbind           = msd_unbind,
	.disconnect       = msd_disconnect,
	.setup            = msd_setup,
	.suspend          = msd_suspend,
	.resume           = msd_resume,
	
	.driver           = {
		.name      = (char *) shortname,
		.owner     = THIS_MODULE,
		// .release = ...
		// .suspend = ...
		// .resume = ...
	},		
};

static int __init msd_alloc(void)
{
	struct msd_dev            *msd;
	struct interface_dev      *interface;
	u8                        i;

	msd = kzalloc(sizeof *msd, GFP_KERNEL);
	if (!msd)
		return -ENOMEM;
		
	spin_lock_init(&msd->lock);
	
	if (mod_data.num_interfaces < 1)
		msd->num_interfaces = 1;
	else if (mod_data.num_interfaces > MAX_INTERFACES)
		msd->num_interfaces = MAX_INTERFACES;
	else
		msd->num_interfaces = mod_data.num_interfaces;
	
	for (i = 0; i < msd->num_interfaces; i++) {
		interface = &msd->interface[i];
		interface->index = i;
		init_completion(&interface->thread_notifier);
	}

	the_msd = msd;
	
	return 0;
}


static int __init msd_init(void)
{
	int                 rc;
	struct msd_dev      *msd;
	
	printk(KERN_ALERT "%s: Desc: %s\n", __FUNCTION__, DRIVER_DESC);
	printk(KERN_ALERT "%s: Name: %s\n", __FUNCTION__, DRIVER_NAME);
	printk(KERN_ALERT "%s: Version: %s\n", __FUNCTION__, DRIVER_VERSION);

	if ((rc = msd_alloc()) != 0)
		return rc;
	msd = the_msd;
	
	strlcpy(controller, mod_data.controller, min(sizeof(controller),
				strlen(mod_data.controller)+1));
	
	if ((rc = usb_composite_register(&msd_driver)) == 0)
		msd->is_registered = 1;
		
	return rc;
}
module_init(msd_init);


static void __exit msd_cleanup(void)
{
	struct msd_dev *msd = the_msd;

	cdev_del(&msd->cdev);
	unregister_chrdev_region(msd->chrdev, MAX_INTERFACES);

	if (msd->is_registered)
		usb_composite_unregister(&msd_driver);

	/* Wait for the thread to finish up */
	//wait_for_completion(&msd->thread_notifier);

}
module_exit(msd_cleanup);
