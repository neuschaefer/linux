/*
 * ether.c -- Ethernet gadget driver, with CDC and non-CDC options
 *
 * Copyright (C) 2003-2005 David Brownell
 * Copyright (C) 2003-2004 Robert Schwebel, Benedikt Spranger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/*
Includes Intel Corporation's changes/modifications dated: 2014.
Changed/modified portions - Copyright © 2014, Intel Corporation.
*/


/* Copyright 2008, Texas Instruments Incorporated
 *
 * This program has been modified from its original operation by
 * Texas Instruments to do the following:
 *
 * Explanation of modification:
 *  Puma5 changes/fixes
 *
 *
 * THIS MODIFIED SOFTWARE AND DOCUMENTATION ARE PROVIDED
 * "AS IS," AND TEXAS INSTRUMENTS MAKES NO REPRESENTATIONS
 * OR WARRENTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO, WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
 * PARTICULAR PURPOSE OR THAT THE USE OF THE SOFTWARE OR
 * DOCUMENTATION WILL NOT INFRINGE ANY THIRD PARTY PATENTS,
 * COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS.
 * See The GNU General Public License for more details.
 *
 * These changes are covered under version 2 of the GNU General Public License,
 * dated June 1991.
 */

// #define DEBUG 1
// #define VERBOSE

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ioport.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/utsname.h>
#include <linux/device.h>
#include <linux/moduleparam.h>
#include <linux/ctype.h>

#include <asm/byteorder.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/unaligned.h>

#include <linux/usb/ch9.h>
#include <linux/usb/cdc.h>
#include <linux/usb/gadget.h>

#include <linux/random.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>

#define CONFIG_USB_PPD_SUPPORT
#define USB_PPD_DEBUG

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
#include <asm-arm/arch-avalanche/generic/csl_defs.h>
#include <asm-arm/arch-avalanche/generic/pal_cppi41.h>
#include <asm-arm/arch-avalanche/puma5/puma5_pp.h>
#include <linux/ti_ppm.h>
extern Int32 ti_ppd_create_pid(TI_PP_PID *pid_params);
extern Int32 ti_ppd_set_pid_flags(TI_PP_PID *pid_params, Uint32 new_flags);
extern Int32 ti_ppd_delete_pid(Uint8 pid_handle);
#endif

#ifdef CONFIG_MACH_PUMA5
    #include <asm-arm/arch-avalanche/puma5/usb_vendor.h>
    #define USB_MIB_SUPPORT
    #define USB_PERFCNT_SELECT
#ifdef USB_MIB_SUPPORT
    #include <linux/proc_fs.h>
    #include <linux/proc_fs_macros.h>
    #include "mibIoctl.h"
static int proc_read_usb_stats(struct seq_file *m, void *v);
static int proc_write_usb_stats(struct file *fp, const char __user *buf, size_t count, loff_t *f_pos);
static int init_mib_counters(struct net_device *dev);
static int eth_ioctl(struct net_device *p_dev, struct ifreq *rq, int cmd);
static int proc_read_usb_link(struct seq_file *m, void *v);
static int proc_read_usb_info(struct seq_file *m, void *v);

#ifdef USB_PERFCNT_SELECT
static int proc_read_usb_cppi(struct seq_file *m, void *v);
static int proc_write_usb_cppi(struct file *fp, const char __user *buf, size_t count, loff_t *f_pos);
#endif
static int proc_write_usb_config_mode(struct file *fp, const char __user *buf, size_t count, loff_t *f_pos);
static int proc_read_usb_config_mode(struct seq_file *m, void *v);
/*
echo 0 > /proc/avalanche/usb_config_mode will set
rndis_default_mode = 1 ( default usb configuration mode = 'A'
echo 1 > /proc/avalanche/usb_config_mode will set
rndis_default_mode = 0 ( default usb configuration mode = 'B')
*/
int rndis_default_mode = 1;
#endif
#endif
//#define USBDRV_DEBUG
#ifdef USBDRV_DEBUG
#define dprintk(x,...) printk(x, ## __VA_ARGS__)
#else
#define dprintk(x,...)
#endif

//#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_MACH_PUMA5)
#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
#define MAX_USB_ENDPOINT            4
#define TI_PP_PID_TYPE_USBRNDIS         3
#define TI_PP_PID_TYPE_USBCDC           4

static  TI_PP_PID   pid_usb [MAX_USB_ENDPOINT];
static  TI_PP_VPID  vpid_usb[MAX_USB_ENDPOINT][TI_PP_MAX_VPID];

static int usb_add_pid      (int indx, int pid_type);
static int install_usb_pid  (struct net_device *net, int mode, bool restore_vpids);
static int uninstall_usb_pid(struct net_device *net, int mode, bool save_vpids);

extern Int32 ti_ppd_config_pid_range(TI_PP_PID_RANGE *pid_range);
extern Int32 ti_ppd_remove_pid_range(Uint32 port_num);
int created_usb_pidtype = 0;
#endif
extern int is_rndis_configured(int configNr);

/*-------------------------------------------------------------------------*/

/*
 * Ethernet gadget driver -- with CDC and non-CDC options
 * Builds on hardware support for a full duplex link.
 *
 * CDC Ethernet is the standard USB solution for sending Ethernet frames
 * using USB.  Real hardware tends to use the same framing protocol but look
 * different for control features.  This driver strongly prefers to use
 * this USB-IF standard as its open-systems interoperability solution;
 * most host side USB stacks (except from Microsoft) support it.
 *
 * There's some hardware that can't talk CDC.  We make that hardware
 * implement a "minimalist" vendor-agnostic CDC core:  same framing, but
 * link-level setup only requires activating the configuration.
 * Linux supports it, but other host operating systems may not.
 * (This is a subset of CDC Ethernet.)
 *
 * A third option is also in use.  Rather than CDC Ethernet, or something
 * simpler, Microsoft pushes their own approach: RNDIS.  The published
 * RNDIS specs are ambiguous and appear to be incomplete, and are also
 * needlessly complex.
 */

#define DRIVER_DESC     "Ethernet Gadget"
#ifdef CONFIG_MACH_PUMA5
#define DRIVER_VERSION      "May 2007"
#else
#define DRIVER_VERSION      "May Day 2005"
#endif
static const char shortname [] = "ether";
static const char driver_desc [] = DRIVER_DESC;

#define RX_EXTRA    20      /* guard against rx overflows */

#include "rndis.h"

#ifndef CONFIG_USB_ETH_RNDIS
#define rndis_uninit(x)     do{}while(0)
#define rndis_deregister(c) do{}while(0)
#define rndis_exit()        do{}while(0)
#endif

/* CDC and RNDIS support the same host-chosen outgoing packet filters. */
#define DEFAULT_FILTER  (USB_CDC_PACKET_TYPE_BROADCAST \
            |USB_CDC_PACKET_TYPE_ALL_MULTICAST \
            |USB_CDC_PACKET_TYPE_PROMISCUOUS \
            |USB_CDC_PACKET_TYPE_DIRECTED)

/*-------------------------------------------------------------------------*/

struct eth_dev {
    spinlock_t      lock;
    struct usb_gadget   *gadget;
    struct usb_request  *req;       /* for control responses */
    struct usb_request  *stat_req;  /* for cdc & rndis status */

    u8          config;
    struct usb_ep       *in_ep, *out_ep, *status_ep;
    const struct usb_endpoint_descriptor
                *in, *out, *status;

    spinlock_t      req_lock;
    struct list_head    tx_reqs, rx_reqs;

    struct net_device   *net;
    struct net_device_stats stats;
    atomic_t        tx_qlen;

    struct work_struct  work;
    unsigned        zlp:1;
    unsigned        cdc:1;
    unsigned        rndis:1;
    unsigned        suspended:1;
    u16         cdc_filter;
    unsigned long       todo;
#define WORK_RX_MEMORY      0
    int         rndis_config;
    u8          host_mac [ETH_ALEN];

#ifdef CONFIG_MACH_PUMA5
    unsigned        int rndis_configured;
#ifdef USB_MIB_SUPPORT
    struct proc_dir_entry   *pde;
    struct proc_dir_entry   *usblink_pde;
    struct proc_dir_entry   *usbinfo_pde;
    struct proc_dir_entry   *usbcppi_pde;
    struct proc_dir_entry   *usbixia_pde;
    CDC_RNDIS_STATS     ifstats;
#endif
#endif

};

/* This version autoconfigures as much as possible at run-time.
 *
 * It also ASSUMES a self-powered device, without remote wakeup,
 * although remote wakeup support would make sense.
 */

/*-------------------------------------------------------------------------*/

/* DO NOT REUSE THESE IDs with a protocol-incompatible driver!!  Ever!!
 * Instead:  allocate your own, using normal USB-IF procedures.
 */

/* Thanks to NetChip Technologies for donating this product ID.
 * It's for devices with only CDC Ethernet configurations.
 */
#define CDC_VENDOR_NUM  0x0525      /* NetChip */
#define CDC_PRODUCT_NUM 0xa4a1      /* Linux-USB Ethernet Gadget */

/* For hardware that can't talk CDC, we use the same vendor ID that
 * ARM Linux has used for ethernet-over-usb, both with sa1100 and
 * with pxa250.  We're protocol-compatible, if the host-side drivers
 * use the endpoint descriptors.  bcdDevice (version) is nonzero, so
 * drivers that need to hard-wire endpoint numbers have a hook.
 *
 * The protocol is a minimal subset of CDC Ether, which works on any bulk
 * hardware that's not deeply broken ... even on hardware that can't talk
 * RNDIS (like SA-1100, with no interrupt endpoint, or anything that
 * doesn't handle control-OUT).
 */
#define SIMPLE_VENDOR_NUM   0x049f
#define SIMPLE_PRODUCT_NUM  0x505a

/* For hardware that can talk RNDIS and either of the above protocols,
 * use this ID ... the windows INF files will know it.  Unless it's
 * used with CDC Ethernet, Linux 2.4 hosts will need updates to choose
 * the non-RNDIS configuration.
 */
#define RNDIS_VENDOR_NUM    0x0525  /* NetChip */
#define RNDIS_PRODUCT_NUM   0xa4a2  /* Ethernet/RNDIS Gadget */


/* Some systems will want different product identifers published in the
 * device descriptor, either numbers or strings or both.  These string
 * parameters are in UTF-8 (superset of ASCII's 7 bit characters).
 */

static ushort idVendor;
module_param(idVendor, ushort, S_IRUGO);
MODULE_PARM_DESC(idVendor, "USB Vendor ID");

static ushort idProduct;
module_param(idProduct, ushort, S_IRUGO);
MODULE_PARM_DESC(idProduct, "USB Product ID");

static ushort bcdDevice;
module_param(bcdDevice, ushort, S_IRUGO);
MODULE_PARM_DESC(bcdDevice, "USB Device version (BCD)");

static char *iManufacturer;
module_param(iManufacturer, charp, S_IRUGO);
MODULE_PARM_DESC(iManufacturer, "USB Manufacturer string");

static char *iProduct;
module_param(iProduct, charp, S_IRUGO);
MODULE_PARM_DESC(iProduct, "USB Product string");

static char *iSerialNumber;
module_param(iSerialNumber, charp, S_IRUGO);
MODULE_PARM_DESC(iSerialNumber, "SerialNumber");

/* initial value, changed by "ifconfig usb0 hw ether xx:xx:xx:xx:xx:xx" */
static char *dev_addr;
module_param(dev_addr, charp, S_IRUGO);
MODULE_PARM_DESC(dev_addr, "Device Ethernet Address");

/* this address is invisible to ifconfig */
static char *host_addr;
module_param(host_addr, charp, S_IRUGO);
MODULE_PARM_DESC(host_addr, "Host Ethernet Address");
extern char *puma5_usb_host_address;

/*-------------------------------------------------------------------------*/

/* Include CDC support if we could run on CDC-capable hardware. */

#ifdef CONFIG_USB_GADGET_NET2280
#define DEV_CONFIG_CDC
#endif

#ifdef CONFIG_USB_GADGET_DUMMY_HCD
#define DEV_CONFIG_CDC
#endif

#ifdef CONFIG_USB_GADGET_GOKU
#define DEV_CONFIG_CDC
#endif

#ifdef CONFIG_USB_GADGET_LH7A40X
#define DEV_CONFIG_CDC
#endif

#ifdef CONFIG_USB_GADGET_MQ11XX
#define DEV_CONFIG_CDC
#endif

#ifdef CONFIG_USB_GADGET_OMAP
#define DEV_CONFIG_CDC
#endif

#ifdef CONFIG_USB_GADGET_N9604
#define DEV_CONFIG_CDC
#endif

#ifdef CONFIG_USB_GADGET_PXA27X
#define DEV_CONFIG_CDC
#endif

#ifdef CONFIG_USB_GADGET_AT91
#define DEV_CONFIG_CDC
#endif

#ifdef CONFIG_USB_GADGET_MUSBHSFC
#define DEV_CONFIG_CDC
#endif

#ifdef CONFIG_USB_MUSB_GADGET
#define DEV_CONFIG_CDC
#endif


/* For CDC-incapable hardware, choose the simple cdc subset.
 * Anything that talks bulk (without notable bugs) can do this.
 */
#ifdef CONFIG_USB_GADGET_PXA2XX
#define DEV_CONFIG_SUBSET
#endif

#ifdef CONFIG_USB_GADGET_SH
#define DEV_CONFIG_SUBSET
#endif

#ifdef CONFIG_USB_GADGET_SA1100
/* use non-CDC for backwards compatibility */
#define DEV_CONFIG_SUBSET
#endif

#ifdef CONFIG_USB_GADGET_S3C2410
#define DEV_CONFIG_CDC
#endif

/*-------------------------------------------------------------------------*/

/* "main" config is either CDC, or its simple subset */
static inline int is_cdc(struct eth_dev *dev)
{
#if !defined(DEV_CONFIG_SUBSET)
    return 1;       /* only cdc possible */
#elif   !defined (DEV_CONFIG_CDC)
    return 0;       /* only subset possible */
#else
    return dev->cdc;    /* depends on what hardware we found */
#endif
}

/* "secondary" RNDIS config may sometimes be activated */
static inline int rndis_active(struct eth_dev *dev)
{
#ifdef  CONFIG_USB_ETH_RNDIS
    return dev->rndis;
#else
    return 0;
#endif
}

#define subset_active(dev)  (!is_cdc(dev) && !rndis_active(dev))
#define cdc_active(dev)     ( is_cdc(dev) && !rndis_active(dev))



#define DEFAULT_QLEN    2   /* double buffering by default */

/* peak bulk transfer bits-per-second */
#ifdef CONFIG_MACH_PUMA5
#define HS_BPS      (480000000)
#define FS_BPS      (12000000)
#else
#define HS_BPS      (13 * 512 * 8 * 1000 * 8)
#define FS_BPS      (19 *  64 * 1 * 1000 * 8)
#endif

#ifdef CONFIG_USB_GADGET_DUALSPEED
#define DEVSPEED    USB_SPEED_HIGH

static unsigned qmult = 30;
module_param (qmult, uint, S_IRUGO|S_IWUSR);


/* for dual-speed hardware, use deeper queues at highspeed */
#define qlen(gadget) \
    (DEFAULT_QLEN*((gadget->speed == USB_SPEED_HIGH) ? qmult : 1))

/* also defer IRQs on highspeed TX */
#define TX_DELAY    qmult

static inline int BITRATE(struct usb_gadget *g)
{
    return (g->speed == USB_SPEED_HIGH) ? HS_BPS : FS_BPS;
}

#else   /* full speed (low speed doesn't do bulk) */
#define DEVSPEED    USB_SPEED_FULL

#define qlen(gadget) DEFAULT_QLEN

static inline int BITRATE(struct usb_gadget *g)
{
    return FS_BPS;
}
#endif


/*-------------------------------------------------------------------------*/

#define xprintk(d,level,fmt,args...) \
    printk(level "%s: " fmt , (d)->net->name , ## args)

#ifdef DEBUG
#undef DEBUG
#define DEBUG(dev,fmt,args...) \
    xprintk(dev , KERN_DEBUG , fmt , ## args)
#else
#define DEBUG(dev,fmt,args...) \
    do { } while (0)
#endif /* DEBUG */

#ifdef VERBOSE
#define VDEBUG  DEBUG
#else
#define VDEBUG(dev,fmt,args...) \
    do { } while (0)
#endif /* DEBUG */

#if 0
#define ERROR(dev,fmt,args...) \
    xprintk(dev , KERN_ERR , fmt , ## args)
#define WARN(dev,fmt,args...) \
    xprintk(dev , KERN_WARNING , fmt , ## args)
#define INFO(dev,fmt,args...) \
    xprintk(dev , KERN_INFO , fmt , ## args)
#endif

/*-------------------------------------------------------------------------*/

/* USB DRIVER HOOKUP (to the hardware driver, below us), mostly
 * ep0 implementation:  descriptors, config management, setup().
 * also optional class-specific notification interrupt transfer.
 */

/*
 * DESCRIPTORS ... most are static, but strings and (full) configuration
 * descriptors are built on demand.  For now we do either full CDC, or
 * our simple subset, with RNDIS as an optional second configuration.
 *
 * RNDIS includes some CDC ACM descriptors ... like CDC Ethernet.  But
 * the class descriptors match a modem (they're ignored; it's really just
 * Ethernet functionality), they don't need the NOP altsetting, and the
 * status transfer endpoint isn't optional.
 */

#define STRING_MANUFACTURER     1
#define STRING_PRODUCT          2
#define STRING_ETHADDR          3
#define STRING_DATA         4
#define STRING_CONTROL          5
#define STRING_RNDIS_CONTROL        6
#define STRING_CDC          7
#define STRING_SUBSET           8
#define STRING_RNDIS            9
#define STRING_SERIALNUMBER     10

/* holds our biggest descriptor (or RNDIS response) */
#define USB_BUFSIZ  256

/*
 * This device advertises one configuration, eth_config, unless RNDIS
 * is enabled (rndis_config) on hardware supporting at least two configs.
 *
 * NOTE:  Controllers like superh_udc should probably be able to use
 * an RNDIS-only configuration.
 *
 * FIXME define some higher-powered configurations to make it easier
 * to recharge batteries ...
 */

#define DEV_CONFIG_VALUE    1   /* cdc or subset */
#define DEV_RNDIS_CONFIG_VALUE  2   /* rndis; optional */

static struct usb_device_descriptor
device_desc = {
    .bLength =      sizeof device_desc,
    .bDescriptorType =  USB_DT_DEVICE,

    .bcdUSB =       __constant_cpu_to_le16 (0x0200),

    .bDeviceClass =     USB_CLASS_COMM,
    .bDeviceSubClass =  0,
    .bDeviceProtocol =  0,

    .idVendor =     __constant_cpu_to_le16 (CDC_VENDOR_NUM),
    .idProduct =        __constant_cpu_to_le16 (CDC_PRODUCT_NUM),
    .iManufacturer =    STRING_MANUFACTURER,
    .iProduct =     STRING_PRODUCT,
    .bNumConfigurations =   1,
};

static struct usb_otg_descriptor
otg_descriptor = {
    .bLength =      sizeof otg_descriptor,
    .bDescriptorType =  USB_DT_OTG,

    .bmAttributes =     USB_OTG_SRP,
};

static struct usb_config_descriptor
eth_config = {
    .bLength =      sizeof eth_config,
    .bDescriptorType =  USB_DT_CONFIG,

    /* compute wTotalLength on the fly */
    .bNumInterfaces =   2,
    .bConfigurationValue =  DEV_CONFIG_VALUE,
    .iConfiguration =   STRING_CDC,
    .bmAttributes =     USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER,
    .bMaxPower =        50,
};

#ifdef  CONFIG_USB_ETH_RNDIS
static struct usb_config_descriptor
rndis_config = {
    .bLength =              sizeof rndis_config,
    .bDescriptorType =      USB_DT_CONFIG,

    /* compute wTotalLength on the fly */
    .bNumInterfaces =       2,
#ifdef CONFIG_MACH_PUMA5
    .bConfigurationValue =  1, //DEV_RNDIS_CONFIG_VALUE,
#else
    .bConfigurationValue =  DEV_RNDIS_CONFIG_VALUE,
#endif
    .iConfiguration =       STRING_RNDIS,
    .bmAttributes =     USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER,
    .bMaxPower =            50,
};
#endif

/*
 * Compared to the simple CDC subset, the full CDC Ethernet model adds
 * three class descriptors, two interface descriptors, optional status
 * endpoint.  Both have a "data" interface and two bulk endpoints.
 * There are also differences in how control requests are handled.
 *
 * RNDIS shares a lot with CDC-Ethernet, since it's a variant of
 * the CDC-ACM (modem) spec.
 */

#ifdef  DEV_CONFIG_CDC
static struct usb_interface_descriptor
control_intf = {
    .bLength =      sizeof control_intf,
    .bDescriptorType =  USB_DT_INTERFACE,

    .bInterfaceNumber = 0,
    /* status endpoint is optional; this may be patched later */
    .bNumEndpoints =    1,
    .bInterfaceClass =  USB_CLASS_COMM,
    .bInterfaceSubClass =   USB_CDC_SUBCLASS_ETHERNET,
    .bInterfaceProtocol =   USB_CDC_PROTO_NONE,
    .iInterface =       STRING_CONTROL,
};
#endif

#ifdef  CONFIG_USB_ETH_RNDIS
static const struct usb_interface_descriptor
rndis_control_intf = {
    .bLength =              sizeof rndis_control_intf,
    .bDescriptorType =      USB_DT_INTERFACE,

    .bInterfaceNumber =     0,
    .bNumEndpoints =        1,
    .bInterfaceClass =      USB_CLASS_COMM,
    .bInterfaceSubClass =   USB_CDC_SUBCLASS_ACM,
    .bInterfaceProtocol =   USB_CDC_ACM_PROTO_VENDOR,
    .iInterface =           STRING_RNDIS_CONTROL,
};
#endif

#if defined(DEV_CONFIG_CDC) || defined(CONFIG_USB_ETH_RNDIS)

static const struct usb_cdc_header_desc header_desc = {
    .bLength =      sizeof header_desc,
    .bDescriptorType =  USB_DT_CS_INTERFACE,
    .bDescriptorSubType =   USB_CDC_HEADER_TYPE,

    .bcdCDC =       __constant_cpu_to_le16 (0x0110),
};

static const struct usb_cdc_union_desc union_desc = {
    .bLength =      sizeof union_desc,
    .bDescriptorType =  USB_DT_CS_INTERFACE,
    .bDescriptorSubType =   USB_CDC_UNION_TYPE,

    .bMasterInterface0 =    0,  /* index of control interface */
    .bSlaveInterface0 = 1,  /* index of DATA interface */
};

#endif  /* CDC || RNDIS */

#ifdef  CONFIG_USB_ETH_RNDIS

static const struct usb_cdc_call_mgmt_descriptor call_mgmt_descriptor = {
    .bLength =      sizeof call_mgmt_descriptor,
    .bDescriptorType =  USB_DT_CS_INTERFACE,
    .bDescriptorSubType =   USB_CDC_CALL_MANAGEMENT_TYPE,

    .bmCapabilities =   0x00,
    .bDataInterface =   0x01,
};

static const struct usb_cdc_acm_descriptor acm_descriptor = {
    .bLength =      sizeof acm_descriptor,
    .bDescriptorType =  USB_DT_CS_INTERFACE,
    .bDescriptorSubType =   USB_CDC_ACM_TYPE,

    .bmCapabilities =   0x00,
};

#endif

#ifdef  DEV_CONFIG_CDC

static const struct usb_cdc_ether_desc ether_desc = {
    .bLength =      sizeof ether_desc,
    .bDescriptorType =  USB_DT_CS_INTERFACE,
    .bDescriptorSubType =   USB_CDC_ETHERNET_TYPE,

    /* this descriptor actually adds value, surprise! */
    .iMACAddress =      STRING_ETHADDR,
    .bmEthernetStatistics = __constant_cpu_to_le32 (0), /* no statistics */
    .wMaxSegmentSize =  __constant_cpu_to_le16 (ETH_FRAME_LEN),
    .wNumberMCFilters = __constant_cpu_to_le16 (0),
    .bNumberPowerFilters =  0,
};

#endif

#if defined(DEV_CONFIG_CDC) || defined(CONFIG_USB_ETH_RNDIS)

/* include the status endpoint if we can, even where it's optional.
 * use wMaxPacketSize big enough to fit CDC_NOTIFY_SPEED_CHANGE in one
 * packet, to simplify cancellation; and a big transfer interval, to
 * waste less bandwidth.
 *
 * some drivers (like Linux 2.4 cdc-ether!) "need" it to exist even
 * if they ignore the connect/disconnect notifications that real aether
 * can provide.  more advanced cdc configurations might want to support
 * encapsulated commands (vendor-specific, using control-OUT).
 *
 * RNDIS requires the status endpoint, since it uses that encapsulation
 * mechanism for its funky RPC scheme.
 */

#define LOG2_STATUS_INTERVAL_MSEC   5   /* 1 << 5 == 32 msec */
#define STATUS_BYTECOUNT        16  /* 8 byte header + data */

static struct usb_endpoint_descriptor
fs_status_desc = {
    .bLength =      USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =  USB_DT_ENDPOINT,

    .bEndpointAddress = USB_DIR_IN,
    .bmAttributes =     USB_ENDPOINT_XFER_INT,
    .wMaxPacketSize =   __constant_cpu_to_le16 (STATUS_BYTECOUNT),
    .bInterval =        1 << LOG2_STATUS_INTERVAL_MSEC,
};
#endif

#ifdef  DEV_CONFIG_CDC

/* the default data interface has no endpoints ... */

static const struct usb_interface_descriptor
data_nop_intf = {
    .bLength =      sizeof data_nop_intf,
    .bDescriptorType =  USB_DT_INTERFACE,

    .bInterfaceNumber = 1,
    .bAlternateSetting =    0,
    .bNumEndpoints =    0,
    .bInterfaceClass =  USB_CLASS_CDC_DATA,
    .bInterfaceSubClass =   0,
    .bInterfaceProtocol =   0,
};

/* ... but the "real" data interface has two bulk endpoints */

static const struct usb_interface_descriptor
data_intf = {
    .bLength =      sizeof data_intf,
    .bDescriptorType =  USB_DT_INTERFACE,

    .bInterfaceNumber = 1,
    .bAlternateSetting =    1,
    .bNumEndpoints =    2,
    .bInterfaceClass =  USB_CLASS_CDC_DATA,
    .bInterfaceSubClass =   0,
    .bInterfaceProtocol =   0,
    .iInterface =       STRING_DATA,
};

#endif

#ifdef  CONFIG_USB_ETH_RNDIS

/* RNDIS doesn't activate by changing to the "real" altsetting */

static const struct usb_interface_descriptor
rndis_data_intf = {
    .bLength =      sizeof rndis_data_intf,
    .bDescriptorType =  USB_DT_INTERFACE,

    .bInterfaceNumber = 1,
    .bAlternateSetting =    0,
    .bNumEndpoints =    2,
    .bInterfaceClass =  USB_CLASS_CDC_DATA,
    .bInterfaceSubClass =   0,
    .bInterfaceProtocol =   0,
    .iInterface =       STRING_DATA,
};

#endif

#ifdef DEV_CONFIG_SUBSET

/*
 * "Simple" CDC-subset option is a simple vendor-neutral model that most
 * full speed controllers can handle:  one interface, two bulk endpoints.
 */

static const struct usb_interface_descriptor
subset_data_intf = {
    .bLength =      sizeof subset_data_intf,
    .bDescriptorType =  USB_DT_INTERFACE,

    .bInterfaceNumber = 0,
    .bAlternateSetting =    0,
    .bNumEndpoints =    2,
    .bInterfaceClass =  USB_CLASS_VENDOR_SPEC,
    .bInterfaceSubClass =   0,
    .bInterfaceProtocol =   0,
    .iInterface =       STRING_DATA,
};

#endif  /* SUBSET */


static struct usb_endpoint_descriptor
fs_source_desc = {
    .bLength =      USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =  USB_DT_ENDPOINT,

    .bEndpointAddress = USB_DIR_IN,
    .bmAttributes =     USB_ENDPOINT_XFER_BULK,
};

static struct usb_endpoint_descriptor
fs_sink_desc = {
    .bLength =      USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =  USB_DT_ENDPOINT,

    .bEndpointAddress = USB_DIR_OUT,
    .bmAttributes =     USB_ENDPOINT_XFER_BULK,
};

static const struct usb_descriptor_header *fs_eth_function [11] = {
    (struct usb_descriptor_header *) &otg_descriptor,
#ifdef DEV_CONFIG_CDC
    /* "cdc" mode descriptors */
    (struct usb_descriptor_header *) &control_intf,
    (struct usb_descriptor_header *) &header_desc,
    (struct usb_descriptor_header *) &union_desc,
    (struct usb_descriptor_header *) &ether_desc,
    /* NOTE: status endpoint may need to be removed */
    (struct usb_descriptor_header *) &fs_status_desc,
    /* data interface, with altsetting */
    (struct usb_descriptor_header *) &data_nop_intf,
    (struct usb_descriptor_header *) &data_intf,
    (struct usb_descriptor_header *) &fs_source_desc,
    (struct usb_descriptor_header *) &fs_sink_desc,
    NULL,
#endif /* DEV_CONFIG_CDC */
};

static inline void __init fs_subset_descriptors(void)
{
#ifdef DEV_CONFIG_SUBSET
    fs_eth_function[1] = (struct usb_descriptor_header *) &subset_data_intf;
    fs_eth_function[2] = (struct usb_descriptor_header *) &fs_source_desc;
    fs_eth_function[3] = (struct usb_descriptor_header *) &fs_sink_desc;
    fs_eth_function[4] = NULL;
#else
    fs_eth_function[1] = NULL;
#endif
}

#ifdef  CONFIG_USB_ETH_RNDIS
static const struct usb_descriptor_header *fs_rndis_function [] = {
    (struct usb_descriptor_header *) &otg_descriptor,
    /* control interface matches ACM, not Ethernet */
    (struct usb_descriptor_header *) &rndis_control_intf,
    (struct usb_descriptor_header *) &header_desc,
    (struct usb_descriptor_header *) &call_mgmt_descriptor,
    (struct usb_descriptor_header *) &acm_descriptor,
    (struct usb_descriptor_header *) &union_desc,
    (struct usb_descriptor_header *) &fs_status_desc,
    /* data interface has no altsetting */
    (struct usb_descriptor_header *) &rndis_data_intf,
    (struct usb_descriptor_header *) &fs_source_desc,
    (struct usb_descriptor_header *) &fs_sink_desc,
    NULL,
};
#endif

#ifdef  CONFIG_USB_GADGET_DUALSPEED

/*
 * usb 2.0 devices need to expose both high speed and full speed
 * descriptors, unless they only run at full speed.
 */

#if defined(DEV_CONFIG_CDC) || defined(CONFIG_USB_ETH_RNDIS)
static struct usb_endpoint_descriptor
hs_status_desc = {
    .bLength =      USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =  USB_DT_ENDPOINT,

    .bmAttributes =     USB_ENDPOINT_XFER_INT,
    .wMaxPacketSize =   __constant_cpu_to_le16 (STATUS_BYTECOUNT),
    .bInterval =        LOG2_STATUS_INTERVAL_MSEC + 4,
};
#endif /* DEV_CONFIG_CDC */

static struct usb_endpoint_descriptor
hs_source_desc = {
    .bLength =      USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =  USB_DT_ENDPOINT,

    .bmAttributes =     USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize =   __constant_cpu_to_le16 (512), /* max usb packet size, 128,256,512 */
};

static struct usb_endpoint_descriptor
hs_sink_desc = {
    .bLength =      USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =  USB_DT_ENDPOINT,

    .bmAttributes =     USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize =   __constant_cpu_to_le16 (512), /* max usb packet size 128,256,512 */
};

static struct usb_qualifier_descriptor
dev_qualifier = {
    .bLength =      sizeof dev_qualifier,
    .bDescriptorType =  USB_DT_DEVICE_QUALIFIER,

    .bcdUSB =       __constant_cpu_to_le16 (0x0200),
    .bDeviceClass =     USB_CLASS_COMM,

    .bNumConfigurations =   1,
};

static const struct usb_descriptor_header *hs_eth_function [11] = {
    (struct usb_descriptor_header *) &otg_descriptor,
#ifdef DEV_CONFIG_CDC
    /* "cdc" mode descriptors */
    (struct usb_descriptor_header *) &control_intf,
    (struct usb_descriptor_header *) &header_desc,
    (struct usb_descriptor_header *) &union_desc,
    (struct usb_descriptor_header *) &ether_desc,
    /* NOTE: status endpoint may need to be removed */
    (struct usb_descriptor_header *) &hs_status_desc,
    /* data interface, with altsetting */
    (struct usb_descriptor_header *) &data_nop_intf,
    (struct usb_descriptor_header *) &data_intf,
    (struct usb_descriptor_header *) &hs_source_desc,
    (struct usb_descriptor_header *) &hs_sink_desc,
    NULL,
#endif /* DEV_CONFIG_CDC */
};

static inline void __init hs_subset_descriptors(void)
{
#ifdef DEV_CONFIG_SUBSET
    hs_eth_function[1] = (struct usb_descriptor_header *) &subset_data_intf;
    hs_eth_function[2] = (struct usb_descriptor_header *) &fs_source_desc;
    hs_eth_function[3] = (struct usb_descriptor_header *) &fs_sink_desc;
    hs_eth_function[4] = NULL;
#else
    hs_eth_function[1] = NULL;
#endif
}

#ifdef  CONFIG_USB_ETH_RNDIS
static const struct usb_descriptor_header *hs_rndis_function [] = {
    (struct usb_descriptor_header *) &otg_descriptor,
    /* control interface matches ACM, not Ethernet */
    (struct usb_descriptor_header *) &rndis_control_intf,
    (struct usb_descriptor_header *) &header_desc,
    (struct usb_descriptor_header *) &call_mgmt_descriptor,
    (struct usb_descriptor_header *) &acm_descriptor,
    (struct usb_descriptor_header *) &union_desc,
    (struct usb_descriptor_header *) &hs_status_desc,
    /* data interface has no altsetting */
    (struct usb_descriptor_header *) &rndis_data_intf,
    (struct usb_descriptor_header *) &hs_source_desc,
    (struct usb_descriptor_header *) &hs_sink_desc,
    NULL,
};
#endif


/* maxpacket and other transfer characteristics vary by speed. */
#define ep_desc(g,hs,fs) (((g)->speed==USB_SPEED_HIGH)?(hs):(fs))

#else

/* if there's no high speed support, maxpacket doesn't change. */
#define ep_desc(g,hs,fs) (((void)(g)), (fs))

static inline void __init hs_subset_descriptors(void)
{
}

#endif  /* !CONFIG_USB_GADGET_DUALSPEED */

/*-------------------------------------------------------------------------*/

/* descriptors that are built on-demand */

static char             manufacturer [50];
static char             product_desc [50] = DRIVER_DESC;
static char             serial_number [20];

#ifdef  DEV_CONFIG_CDC
/* address that the host will use ... usually assigned at random */
static char             ethaddr [2 * ETH_ALEN + 1];
#endif

/* static strings, in UTF-8 */
static struct usb_string        strings [] = {
    { STRING_MANUFACTURER,  manufacturer, },
    { STRING_PRODUCT,   product_desc, },
    { STRING_SERIALNUMBER,  serial_number, },
#if defined(CONFIG_MACH_PUMA5) && defined(DEFAULT_USB_DATA_IF)
    { STRING_DATA,      DEFAULT_USB_DATA_IF, },
#else
    { STRING_DATA,      "Ethernet Data", },
#endif
#ifdef  DEV_CONFIG_CDC
#if defined(CONFIG_MACH_PUMA5) && defined(DEFAULT_USB_CONFIG)
    { STRING_CDC,       DEFAULT_USB_CONFIG, },
#else
    { STRING_CDC,       "CDC Ethernet", },
#endif
    { STRING_ETHADDR,   ethaddr, },
#if defined(CONFIG_MACH_PUMA5) && defined(DEFAULT_USB_COMM_IF)
    { STRING_CONTROL,   DEFAULT_USB_COMM_IF, },
#else
    { STRING_CONTROL,   "CDC Communications Control", },
#endif
#endif
#ifdef  DEV_CONFIG_SUBSET
    { STRING_SUBSET,    "CDC Ethernet Subset", },
#endif
#ifdef  CONFIG_USB_ETH_RNDIS
#if defined(CONFIG_MACH_PUMA5) && defined(DEFAULT_USB_CONFIG)
    { STRING_RNDIS,     DEFAULT_USB_CONFIG, },
#else
    { STRING_RNDIS,     "RNDIS", },
#endif
#if defined(CONFIG_MACH_PUMA5) && defined(DEFAULT_USB_COMM_IF)
    { STRING_RNDIS_CONTROL, DEFAULT_USB_COMM_IF, },
#else
    { STRING_RNDIS_CONTROL, "RNDIS Communications Control", },
#endif
#endif
    {  }        /* end of list */
};

static struct usb_gadget_strings    stringtab = {
    .language   = 0x0409,   /* en-us */
    .strings    = strings,
};

/*
 * one config, two interfaces:  control, data.
 * complications: class descriptors, and an altsetting.
 */
static int
config_buf (enum usb_device_speed speed,
    u8 *buf, u8 type,
    unsigned index, int is_otg)
{
    int                 len;
    const struct usb_config_descriptor  *config;
    const struct usb_descriptor_header  **function;
#ifdef CONFIG_USB_GADGET_DUALSPEED
    int             hs = (speed == USB_SPEED_HIGH);


    if (type == USB_DT_OTHER_SPEED_CONFIG)
        hs = !hs;
#define which_fn(t) (hs ? hs_ ## t ## _function : fs_ ## t ## _function)
#else
#define which_fn(t) (fs_ ## t ## _function)
#endif

#ifdef CONFIG_MACH_PUMA5
      if( !rndis_default_mode )
#endif
      if (index >= device_desc.bNumConfigurations)
        return -EINVAL;

#ifdef  CONFIG_USB_ETH_RNDIS
    /* list the RNDIS config first, to make Microsoft's drivers
     * happy. DOCSIS 1.0 needs this too.
     */
    if (device_desc.bNumConfigurations == 2 && (index == 0)) {
        config = &rndis_config;
        function = which_fn (rndis);
    } else {
#endif

#ifdef CONFIG_MACH_PUMA5
        if( !rndis_default_mode ){
            config = &eth_config;
            function = which_fn (eth);
        }else {
            config = &rndis_config;
            function = which_fn (rndis);
		}
#else
            config = &eth_config;
            function = which_fn (eth);
#endif
    }

    /* for now, don't advertise srp-only devices */
    if (!is_otg)
        function++;

    len = usb_gadget_config_buf (config, buf, USB_BUFSIZ, function);
    if (len < 0)
        return len;
    ((struct usb_config_descriptor *) buf)->bDescriptorType = type;
    return len;
}

/*-------------------------------------------------------------------------*/

static void eth_start (struct eth_dev *dev, gfp_t gfp_flags);
static int alloc_requests (struct eth_dev *dev, unsigned n, gfp_t gfp_flags);

static int
set_ether_config (struct eth_dev *dev, gfp_t gfp_flags,int mode)
{
    int                 result = 0;
    struct usb_gadget           *gadget = dev->gadget;

#if defined(DEV_CONFIG_CDC) || defined(CONFIG_USB_ETH_RNDIS)
    /* status endpoint used for RNDIS and (optionally) CDC */
    if (!subset_active(dev) && dev->status_ep) {
        dev->status = ep_desc (gadget, &hs_status_desc,
                        &fs_status_desc);
        dev->status_ep->driver_data = dev;
        dev->status_ep->private_data = mode;
        dev->status_ep->desc = dev->status;

        result = usb_ep_enable (dev->status_ep);
        if (result != 0) {
            DEBUG (dev, "enable %s --> %d\n",
                dev->status_ep->name, result);
            goto done;
        }
    }
#endif

    dev->in = ep_desc (dev->gadget, &hs_source_desc, &fs_source_desc);
    dev->in_ep->driver_data = dev;
    dev->in_ep->private_data = mode;
	dev->in_ep->desc = dev->in;

    dev->out = ep_desc (dev->gadget, &hs_sink_desc, &fs_sink_desc);
    dev->out_ep->driver_data = dev;
    dev->out_ep->private_data = mode;
	dev->out_ep->desc = dev->out;
    /* With CDC,  the host isn't allowed to use these two data
     * endpoints in the default altsetting for the interface.
     * so we don't activate them yet.  Reset from SET_INTERFACE.
     *
     * Strictly speaking RNDIS should work the same: activation is
     * a side effect of setting a packet filter.  Deactivation is
     * from REMOTE_NDIS_HALT_MSG, reset from REMOTE_NDIS_RESET_MSG.
     */
    if (!cdc_active(dev)) {
        result = usb_ep_enable (dev->in_ep);
        if (result != 0) {
            DEBUG(dev, "enable %s --> %d\n",
                dev->in_ep->name, result);
            goto done;
        }

        result = usb_ep_enable (dev->out_ep);
        if (result != 0) {
            DEBUG (dev, "enable %s --> %d\n",
                dev->out_ep->name, result);
            goto done;
        }
    }

done:
    if (result == 0)
        result = alloc_requests (dev, qlen (gadget), gfp_flags);

    /* on error, disable any endpoints  */
    if (result < 0) {
        if (!subset_active(dev))
            (void) usb_ep_disable (dev->status_ep);
        dev->status = NULL;
        (void) usb_ep_disable (dev->in_ep);
        (void) usb_ep_disable (dev->out_ep);
        dev->in = NULL;
        dev->out = NULL;
    } else

    /* activate non-CDC configs right away
     * this isn't strictly according to the RNDIS spec
     */
    if (!cdc_active (dev)) {
        netif_carrier_on (dev->net);
        if (netif_running (dev->net)) {
            spin_unlock (&dev->lock);
            eth_start (dev, GFP_ATOMIC);
            spin_lock (&dev->lock);
        }
    }

    if (result == 0)
        DEBUG (dev, "qlen %d\n", qlen (gadget));

    /* caller is responsible for cleanup on error */
    return result;
}

static void eth_reset_config (struct eth_dev *dev)
{
    struct usb_request  *req;

    if (dev->config == 0)
        return;

    DEBUG (dev, "%s\n", __FUNCTION__);

    netif_stop_queue (dev->net);
    netif_carrier_off (dev->net);
    rndis_uninit(dev->rndis_config);

    /* disable endpoints, forcing (synchronous) completion of
     * pending i/o.  then free the requests.
     */
    if (dev->in) {
        usb_ep_disable (dev->in_ep);
        spin_lock(&dev->req_lock);
        while (likely (!list_empty (&dev->tx_reqs))) {
            req = container_of (dev->tx_reqs.next,
                        struct usb_request, list);
            list_del (&req->list);

            spin_unlock(&dev->req_lock);
            usb_ep_free_request (dev->in_ep, req);
            spin_lock(&dev->req_lock);
        }
        spin_unlock(&dev->req_lock);
    }

    if (dev->out) {
        usb_ep_disable (dev->out_ep);
        spin_lock(&dev->req_lock);
        while (likely (!list_empty (&dev->rx_reqs))) {
            req = container_of (dev->rx_reqs.next,
                        struct usb_request, list);
            list_del (&req->list);

            spin_unlock(&dev->req_lock);
            usb_ep_free_request (dev->out_ep, req);
            spin_lock(&dev->req_lock);
        }
        spin_unlock(&dev->req_lock);
    }

    if (dev->status) {
        usb_ep_disable (dev->status_ep);
    }
    dev->rndis = 0;
    dev->cdc_filter = 0;
    dev->config = 0;
}

/* change our operational config.  must agree with the code
 * that returns config descriptors, and altsetting code.
 */
static int
eth_set_config (struct eth_dev *dev, unsigned number, gfp_t gfp_flags)
{
    int         result = 0;
    struct usb_gadget   *gadget = dev->gadget;
    int         mode = 0;

#if 0
    if (gadget_is_sa1100 (gadget)
            && dev->config
            && atomic_read (&dev->tx_qlen) != 0) {
        /* tx fifo is full, but we can't clear it...*/
        INFO (dev, "can't change configurations\n");
        return -ESPIPE;
    }
#endif
    eth_reset_config (dev);

    switch (number) {
    case DEV_CONFIG_VALUE:
#ifdef CONFIG_MACH_PUMA5
        if( rndis_default_mode ){
            dev->rndis = 1;
            dev->cdc_filter = 0x2b;
            mode = DEV_RNDIS_CONFIG_VALUE;
        }else
                        mode = DEV_CONFIG_VALUE;
#endif
        result = set_ether_config (dev, gfp_flags,mode);
        break;
#ifdef  CONFIG_USB_ETH_RNDIS
    case DEV_RNDIS_CONFIG_VALUE:
        dev->rndis = 1;
        mode = DEV_RNDIS_CONFIG_VALUE;
        result = set_ether_config (dev, gfp_flags,mode);
        break;
#endif
    default:
        result = -EINVAL;
        /* FALL THROUGH */
    case 0:
        break;
    }

    if (result) {
        if (number)
            eth_reset_config (dev);
        usb_gadget_vbus_draw(dev->gadget,
                gadget_is_otg(dev->gadget) ? 8 : 100);
    } else {
        char *speed;
        unsigned power;

        power = 2 * eth_config.bMaxPower;
        usb_gadget_vbus_draw(dev->gadget, power);

        switch (gadget->speed) {
        case USB_SPEED_FULL:    speed = "full"; break;
#ifdef CONFIG_USB_GADGET_DUALSPEED
        case USB_SPEED_HIGH:    speed = "high"; break;
#endif
        default:        speed = "?"; break;
        }

        dev->config = number;
        INFO (dev, "%s speed config #%d: %d mA, %s, using %s\n",
                speed, number, power, driver_desc,
                rndis_active(dev)
                    ? "RNDIS"
                    : (cdc_active(dev)
                        ? "CDC Ethernet"
                        : "CDC Ethernet Subset"));
    }
    return result;
}

/*-------------------------------------------------------------------------*/

#ifdef  DEV_CONFIG_CDC

/* The interrupt endpoint is used in CDC networking models (Ethernet, ATM)
 * only to notify the host about link status changes (which we support) or
 * report completion of some encapsulated command (as used in RNDIS).  Since
 * we want this CDC Ethernet code to be vendor-neutral, we don't use that
 * command mechanism; and only one status request is ever queued.
 */

static void eth_status_complete (struct usb_ep *ep, struct usb_request *req)
{
    struct usb_cdc_notification *event = req->buf;
    int             value = req->status;
    struct eth_dev          *dev = ep->driver_data;

    /* issue the second notification if host reads the first */
    if (event->bNotificationType == USB_CDC_NOTIFY_NETWORK_CONNECTION
            && value == 0) {
        __le32  *data = req->buf + sizeof *event;

        event->bmRequestType = 0xA1;
        event->bNotificationType = USB_CDC_NOTIFY_SPEED_CHANGE;
        event->wValue = __constant_cpu_to_le16 (0);
        event->wIndex = __constant_cpu_to_le16 (1);
        event->wLength = __constant_cpu_to_le16 (8);

        /* SPEED_CHANGE data is up/down speeds in bits/sec */
        data [0] = data [1] = cpu_to_le32 (BITRATE (dev->gadget));

        req->length = STATUS_BYTECOUNT;
        value = usb_ep_queue (ep, req, GFP_ATOMIC);
        DEBUG (dev, "send SPEED_CHANGE --> %d\n", value);
        if (value == 0)
            return;
    } else if (value != -ECONNRESET)
        DEBUG (dev, "event %02x --> %d\n",
            event->bNotificationType, value);
    req->context = NULL;
}

static void issue_start_status (struct eth_dev *dev)
{
    struct usb_request      *req = dev->stat_req;
    struct usb_cdc_notification *event;
    int             value;

    DEBUG (dev, "%s, flush old status first\n", __FUNCTION__);

    /* flush old status
     *
     * FIXME ugly idiom, maybe we'd be better with just
     * a "cancel the whole queue" primitive since any
     * unlink-one primitive has way too many error modes.
     * here, we "know" toggle is already clear...
     *
     * FIXME iff req->context != null just dequeue it
     */

#ifndef CONFIG_MACH_PUMA5
    usb_ep_disable (dev->status_ep);
    dev->status_ep->desc = dev->status;
    usb_ep_enable (dev->status_ep);
#endif
    /* 3.8.1 says to issue first NETWORK_CONNECTION, then
     * a SPEED_CHANGE.  could be useful in some configs.
     */
    event = req->buf;
    event->bmRequestType = 0xA1;
    event->bNotificationType = USB_CDC_NOTIFY_NETWORK_CONNECTION;
    event->wValue = __constant_cpu_to_le16 (1); /* connected */
    event->wIndex = __constant_cpu_to_le16 (1);
    event->wLength = 0;

    req->length = sizeof *event;
    req->complete = eth_status_complete;
    req->context = dev;
    value = usb_ep_queue (dev->status_ep, req, GFP_ATOMIC);
    if (value < 0)
        DEBUG (dev, "status buf queue --> %d\n", value);
}

#endif

/*-------------------------------------------------------------------------*/

static void eth_setup_complete (struct usb_ep *ep, struct usb_request *req)
{
    if (req->status || req->actual != req->length)
        DEBUG ((struct eth_dev *) ep->driver_data,
                "setup complete --> %d, %d/%d\n",
                req->status, req->actual, req->length);
}

#ifdef CONFIG_USB_ETH_RNDIS

static void rndis_response_complete (struct usb_ep *ep, struct usb_request *req)
{
    if (req->status || req->actual != req->length)
        DEBUG ((struct eth_dev *) ep->driver_data,
            "rndis response complete --> %d, %d/%d\n",
            req->status, req->actual, req->length);

    /* done sending after USB_CDC_GET_ENCAPSULATED_RESPONSE */
}

static void rndis_command_complete (struct usb_ep *ep, struct usb_request *req)
{
    struct eth_dev          *dev = ep->driver_data;
    int         status;
    u32 MsgType;

    /* received RNDIS command from USB_CDC_SEND_ENCAPSULATED_COMMAND */
    spin_lock(&dev->lock);
    status = rndis_msg_parser (dev->rndis_config, (u8 *) req->buf);
    if (status < 0)
        ERROR(dev, "%s: rndis parse error %d\n", __FUNCTION__, status);

    MsgType   = le32_to_cpup((u32 *)req->buf);

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
   /* For USB: responses may take up to 10 seconds */
    switch (MsgType)
    {
        case 2:
        if( is_rndis_configured(dev->rndis_config) == 1 )
        {
            dev->rndis_configured = 1;
        uninstall_usb_pid( dev->net, rndis_default_mode, true );
            install_usb_pid( dev->net, rndis_default_mode, true );
        }
    }
#endif
    spin_unlock(&dev->lock);
}

#endif  /* RNDIS */

/*
 * The setup() callback implements all the ep0 functionality that's not
 * handled lower down.  CDC has a number of less-common features:
 *
 *  - two interfaces:  control, and ethernet data
 *  - Ethernet data interface has two altsettings:  default, and active
 *  - class-specific descriptors for the control interface
 *  - class-specific control requests
 */
static int
eth_setup (struct usb_gadget *gadget, const struct usb_ctrlrequest *ctrl)
{
    struct eth_dev      *dev = get_gadget_data (gadget);
    struct usb_request  *req = dev->req;
    int         value = -EOPNOTSUPP;
    u16         wIndex = le16_to_cpu(ctrl->wIndex);
    u16         wValue = le16_to_cpu(ctrl->wValue);
    u16         wLength = le16_to_cpu(ctrl->wLength);

    /* descriptors just go into the pre-allocated ep0 buffer,
     * while config change events may enable network traffic.
     */
    req->complete = eth_setup_complete;


    switch (ctrl->bRequest) {

    case USB_REQ_GET_DESCRIPTOR:
        if (ctrl->bRequestType != USB_DIR_IN)
            break;
        switch (wValue >> 8) {

        case USB_DT_DEVICE:

#ifdef CONFIG_MACH_PUMA5
            if( rndis_default_mode ){
                device_desc.bNumConfigurations = 1;
            }
            else{
                device_desc.bNumConfigurations = 2;
                rndis_config.bConfigurationValue = DEV_RNDIS_CONFIG_VALUE;
            }
#endif
            value = min (wLength, (u16) sizeof device_desc);
            memcpy (req->buf, &device_desc, value);
            break;
#ifdef CONFIG_USB_GADGET_DUALSPEED
        case USB_DT_DEVICE_QUALIFIER:
            if (!gadget_is_dualspeed(gadget))
                break;
            value = min (wLength, (u16) sizeof dev_qualifier);
            memcpy (req->buf, &dev_qualifier, value);
            break;

        case USB_DT_OTHER_SPEED_CONFIG:
            if (!gadget_is_dualspeed(gadget))
                break;
            // FALLTHROUGH
#endif /* CONFIG_USB_GADGET_DUALSPEED */
        case USB_DT_CONFIG:
            value = config_buf (gadget->speed, req->buf,
                    wValue >> 8,
                    wValue & 0xff,
                    gadget_is_otg(gadget));
            if (value >= 0)
                value = min (wLength, (u16) value);
            break;

        case USB_DT_STRING:
            value = usb_gadget_get_string (&stringtab,
                    wValue & 0xff, req->buf);
            if (value >= 0)
                value = min (wLength, (u16) value);
            break;
        }
        break;

    case USB_REQ_SET_CONFIGURATION:

        if (ctrl->bRequestType != 0)
            break;
        if (gadget->a_hnp_support)
            DEBUG (dev, "HNP available\n");
        else if (gadget->a_alt_hnp_support)
            DEBUG (dev, "HNP needs a different root port\n");
        spin_lock (&dev->lock);
        value = eth_set_config (dev, wValue, GFP_ATOMIC);
        spin_unlock (&dev->lock);
#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
        uninstall_usb_pid( dev->net, rndis_default_mode, true );
          install_usb_pid( dev->net, rndis_default_mode, true );
#endif
        break;
    case USB_REQ_GET_CONFIGURATION:
        if (ctrl->bRequestType != USB_DIR_IN)
            break;
        *(u8 *)req->buf = dev->config;
        value = min (wLength, (u16) 1);
        break;

    case USB_REQ_SET_INTERFACE:
        if (ctrl->bRequestType != USB_RECIP_INTERFACE
                || !dev->config
                || wIndex > 1){
            break;
        }


#ifdef CONFIG_MACH_PUMA5
        if( !rndis_default_mode )
#endif
        if (!cdc_active(dev) && wIndex != 0){
            value = 0;
            break;
        }
        spin_lock (&dev->lock);

        /* PXA hardware partially handles SET_INTERFACE;
         * we need to kluge around that interference.
         */
        if (gadget_is_pxa (gadget)) {
            value = eth_set_config (dev, DEV_CONFIG_VALUE,
                        GFP_ATOMIC);
            goto done_set_intf;
        }
#ifdef DEV_CONFIG_CDC
        switch (wIndex) {
        case 0:     /* control/master intf */
            if (wValue != 0)
                break;


            if (dev->status) {
#ifdef CONFIG_MACH_PUMA5
            if( !rndis_default_mode )
#endif
                usb_ep_disable (dev->status_ep);
                dev->status_ep->desc = dev->status;
                usb_ep_enable (dev->status_ep);
            }

            value = 0;
            break;
        case 1:     /* data intf */
            if (wValue > 1)
                break;

#ifdef CONFIG_MACH_PUMA5
            if( !rndis_default_mode )
#endif
            {
                usb_ep_disable (dev->in_ep);
                usb_ep_disable (dev->out_ep);
            }
            /* CDC requires the data transfers not be done from
             * the default interface setting ... also, setting
             * the non-default interface resets filters etc.
             */
            if (wValue == 1) {
#ifdef CONFIG_MACH_PUMA5
            if( !rndis_default_mode )
#endif
            {
                if (!cdc_active (dev))
                    break;
                dev->in_ep->desc = dev->in;
                usb_ep_enable (dev->in_ep);
                dev->out_ep->desc = dev->out;
                usb_ep_enable (dev->out_ep);
            }

                dev->cdc_filter = DEFAULT_FILTER;
                netif_carrier_on (dev->net);
                if (dev->status){
                    issue_start_status (dev);
                }
#ifdef CONFIG_MACH_PUMA5
                if( !rndis_default_mode )
#endif
                if (netif_running (dev->net)) {
                    spin_unlock (&dev->lock);
                    eth_start (dev, GFP_ATOMIC);
                    spin_lock (&dev->lock);
                }

            } else {
#ifdef CONFIG_MACH_PUMA5
                if( !rndis_default_mode )
#endif
                {
                netif_stop_queue (dev->net);
                netif_carrier_off (dev->net);
                }
            }
            value = 0;
            break;
        }
#else
        /* FIXME this is wrong, as is the assumption that
         * all non-PXA hardware talks real CDC ...
         */
        dev_warn (&gadget->dev, "set_interface ignored!\n");
#endif /* DEV_CONFIG_CDC */

done_set_intf:
        spin_unlock (&dev->lock);
        break;
    case USB_REQ_GET_INTERFACE:
        if (ctrl->bRequestType != (USB_DIR_IN|USB_RECIP_INTERFACE)
                || !dev->config
                || wIndex > 1)
            break;
        if (!(cdc_active(dev) || rndis_active(dev)) && wIndex != 0)
            break;

        /* for CDC, iff carrier is on, data interface is active. */
        if (rndis_active(dev) || wIndex != 1)
            *(u8 *)req->buf = 0;
        else
            *(u8 *)req->buf = netif_carrier_ok (dev->net) ? 1 : 0;
        value = min (wLength, (u16) 1);
        break;

#ifdef DEV_CONFIG_CDC
    case USB_CDC_SET_ETHERNET_PACKET_FILTER:
        /* see 6.2.30: no data, wIndex = interface,
         * wValue = packet filter bitmap
         */
        if (ctrl->bRequestType != (USB_TYPE_CLASS|USB_RECIP_INTERFACE)
                || !cdc_active(dev)
                || wLength != 0
                || wIndex > 1)
            break;
        DEBUG (dev, "packet filter %02x\n", wValue);
        dev->cdc_filter = wValue;
        value = 0;
        break;

    /* and potentially:
     * case USB_CDC_SET_ETHERNET_MULTICAST_FILTERS:
     * case USB_CDC_SET_ETHERNET_PM_PATTERN_FILTER:
     * case USB_CDC_GET_ETHERNET_PM_PATTERN_FILTER:
     * case USB_CDC_GET_ETHERNET_STATISTIC:
     */

#endif /* DEV_CONFIG_CDC */

#ifdef CONFIG_USB_ETH_RNDIS
    /* RNDIS uses the CDC command encapsulation mechanism to implement
     * an RPC scheme, with much getting/setting of attributes by OID.
     */
    case USB_CDC_SEND_ENCAPSULATED_COMMAND:
        if (ctrl->bRequestType != (USB_TYPE_CLASS|USB_RECIP_INTERFACE)
                || !rndis_active(dev)
                || wLength > USB_BUFSIZ
                || wValue
                || rndis_control_intf.bInterfaceNumber
                    != wIndex)
            break;
        /* read the request, then process it */
        value = wLength;
        req->complete = rndis_command_complete;
        /* later, rndis_control_ack () sends a notification */
        break;

    case USB_CDC_GET_ENCAPSULATED_RESPONSE:
        if ((USB_DIR_IN|USB_TYPE_CLASS|USB_RECIP_INTERFACE)
                    == ctrl->bRequestType
                && rndis_active(dev)
                // && wLength >= 0x0400
                && !wValue
                && rndis_control_intf.bInterfaceNumber
                    == wIndex) {
            u8 *buf;

            /* return the result */
            buf = rndis_get_next_response (dev->rndis_config,
                               &value);
            if (buf) {
                memcpy (req->buf, buf, value);
                req->complete = rndis_response_complete;
                rndis_free_response(dev->rndis_config, buf);
            }
            /* else stalls ... spec says to avoid that */
        }
        break;
#endif  /* RNDIS */

    default:
        VDEBUG (dev,
            "unknown control req%02x.%02x v%04x i%04x l%d\n",
            ctrl->bRequestType, ctrl->bRequest,
            wValue, wIndex, wLength);
    }

    /* respond with data transfer before status phase? */
    if (value >= 0) {

        req->length = value;
        req->zero = value < wLength
                && (value % gadget->ep0->maxpacket) == 0;
        value = usb_ep_queue (gadget->ep0, req, GFP_ATOMIC);
        if (value < 0) {
            DEBUG (dev, "ep_queue --> %d\n", value);
            req->status = 0;
            eth_setup_complete (gadget->ep0, req);
        }
    }

    /* host either stalls (value < 0) or reports success */
    return value;
}

static void
eth_disconnect (struct usb_gadget *gadget)
{
    struct eth_dev      *dev = get_gadget_data (gadget);
    unsigned long       flags;

    spin_lock_irqsave (&dev->lock, flags);
#ifdef CONFIG_MACH_PUMA5
#ifdef CONFIG_USB_ETH_RNDIS  /* TAG0002 */
        rndis_signal_disconnect (dev->rndis_config, 0);
    dev->rndis_configured = 0;
#endif
#endif
    netif_stop_queue (dev->net);
    netif_carrier_off (dev->net);
#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
#ifdef CONFIG_TI_PACKET_PROCESSOR
    ti_ppm_set_pid_flags (dev->net->pid_handle, TI_PP_PID_DISCARD_ALL_RX);
    uninstall_usb_pid( dev->net, rndis_default_mode, false );
#else
    ti_ppd_set_pid_flags (&pid_usb[0], TI_PP_PID_DISCARD_ALL_RX);
    uninstall_usb_pid( dev->net, rndis_default_mode, false );
#endif
    /* this delay is to make sure all the packets with the PID successfully egress throgh the respective ports.*/
    mdelay(200);
#endif


    eth_reset_config (dev);
    spin_unlock_irqrestore (&dev->lock, flags);

    /* FIXME RNDIS should enter RNDIS_UNINITIALIZED */

    /* next we may get setup() calls to enumerate new connections;
     * or an unbind() during shutdown (including removing module).
     */
}

/*-------------------------------------------------------------------------*/

/* NETWORK DRIVER HOOKUP (to the layer above this driver) */

static int g_eth_change_mtu (struct net_device *net, int new_mtu)
{
    struct eth_dev  *dev = netdev_priv(net);

    if (dev->rndis)
        return -EBUSY;

    if (new_mtu <= ETH_HLEN || new_mtu > ETH_FRAME_LEN)
        return -ERANGE;
    /* no zero-length packet read wanted after mtu-sized packets */
    if (((new_mtu + sizeof (struct ethhdr)) % dev->in_ep->maxpacket) == 0)
        return -EDOM;
    net->mtu = new_mtu;
    return 0;
}

static struct net_device_stats *eth_get_stats (struct net_device *net)
{
    return &((struct eth_dev *)netdev_priv(net))->stats;
}

static void eth_get_drvinfo(struct net_device *net, struct ethtool_drvinfo *p)
{
    struct eth_dev  *dev = netdev_priv(net);
    strlcpy(p->driver, shortname, sizeof p->driver);
    strlcpy(p->version, DRIVER_VERSION, sizeof p->version);
    strlcpy(p->fw_version, dev->gadget->name, sizeof p->fw_version);
    strlcpy (p->bus_info, dev_name(dev->gadget->dev.parent), sizeof p->bus_info);
}

static u32 eth_get_link(struct net_device *net)
{
    struct eth_dev  *dev = netdev_priv(net);
    return dev->gadget->speed != USB_SPEED_UNKNOWN;
}

static struct ethtool_ops ops = {
    .get_drvinfo = eth_get_drvinfo,
    .get_link = eth_get_link
};

static void defer_kevent (struct eth_dev *dev, int flag)
{
    if (test_and_set_bit (flag, &dev->todo))
        return;
    if (!schedule_work (&dev->work))
        ERROR (dev, "kevent %d may have been dropped\n", flag);
    else
        DEBUG (dev, "kevent %d scheduled\n", flag);
}

static void rx_complete (struct usb_ep *ep, struct usb_request *req);

static int
rx_submit (struct eth_dev *dev, struct usb_request *req, gfp_t gfp_flags)
{
    struct sk_buff      *skb;
    int         retval = -ENOMEM;
    size_t          size;
    unsigned long   flags;

    /* Padding up to RX_EXTRA handles minor disagreements with host.
     * Normally we use the USB "terminate on short read" convention;
     * so allow up to (N*maxpacket), since that memory is normally
     * already allocated.  Some hardware doesn't deal well with short
     * reads (e.g. DMA must be N*maxpacket), so for now don't trim a
     * byte off the end (to force hardware errors on overflow).
     *
     * RNDIS uses internal framing, and explicitly allows senders to
     * pad to end-of-packet.  That's potentially nice for speed,
     * but means receivers can't recover synch on their own.
     */
    size = (sizeof (struct ethhdr) + dev->net->mtu + RX_EXTRA);
    size += dev->out_ep->maxpacket - 1;
    if (rndis_active(dev))
        size += sizeof (struct rndis_packet_msg_type);
    size -= size % dev->out_ep->maxpacket;

    if ((skb = alloc_skb (size + NET_IP_ALIGN, gfp_flags)) == 0) {
        DEBUG (dev, "no rx skb\n");
        goto enomem;
    }
   
    /* Some platforms perform better when IP packets are aligned,
     * but on at least one, checksumming fails otherwise.  Note:
     * RNDIS headers involve variable numbers of LE32 values.
     */
    skb_reserve(skb, NET_IP_ALIGN);
    skb_reset_mac_header(skb);
    skb_reset_network_header(skb);

    req->buf = skb->data;
    req->length = size;
    req->complete = rx_complete;
    req->context = skb;

    retval = usb_ep_queue (dev->out_ep, req, gfp_flags);
    if (retval == -ENOMEM)
enomem:
        defer_kevent (dev, WORK_RX_MEMORY);
    if (retval) {
        DEBUG (dev, "rx submit --> %d\n", retval);
        dev_kfree_skb_any (skb);
        spin_lock_irqsave(&dev->req_lock, flags);
        list_add (&req->list, &dev->rx_reqs);
        spin_unlock_irqrestore(&dev->req_lock, flags);
    }
    return retval;
}

static void rx_complete (struct usb_ep *ep, struct usb_request *req)
{
    struct sk_buff  *skb = req->context;
    struct eth_dev  *dev = ep->driver_data;
    int     status = req->status,len=0;
        u8 src[6];
        struct ethhdr *eth;

    switch (status) {

    /* normal completion */
    case 0:
        skb_put (skb, req->actual);
        /* we know MaxPacketsPerTransfer == 1 here */
        if (rndis_active(dev))
            status = rndis_rm_hdr (skb);
        if (status < 0
                || ETH_HLEN > skb->len
                || skb->len > ETH_FRAME_LEN) {
            dev->stats.rx_errors++;
            dev->stats.rx_length_errors++;
#ifdef CONFIG_MACH_PUMA5
#ifdef USB_MIB_SUPPORT
            dev->ifstats.rx_errors++;
            dev->ifstats.receive_packets_dropped++;
#endif
#endif

            DEBUG (dev, "rx length %d\n", skb->len);
            break;
        }

#ifdef CONFIG_MACH_PUMA5
#ifdef USB_MIB_SUPPORT
            memcpy (src, skb->data, 6);
#endif
#endif
        skb->dev = dev->net;
        len = skb->len;
        skb->protocol = eth_type_trans (skb, dev->net);

#ifdef CONFIG_MACH_PUMA5
            /* drop the unknown protocol packets */
            eth = eth_hdr(skb);

            if(eth->h_proto == 0) {
#ifdef USB_MIB_SUPPORT
            dev->ifstats.unknownProtPkts++;
            dev->ifstats.receive_packets_dropped++;
#endif
                dev_kfree_skb_any(skb);
            return ;
            }
#endif
        dev->stats.rx_packets++;
        dev->stats.rx_bytes += len;
#ifdef CONFIG_MACH_PUMA5
#ifdef USB_MIB_SUPPORT
        {
            //u8 *src = skb->data;
            if( is_multicast_ether_addr(src)){
               if ( is_broadcast_ether_addr(src) ){
                dev->ifstats.broadcast_pkts_rxed++;
                dev->ifstats.broadcast_bytes_rxed += len;
               }else{
                dev->ifstats.multicast_pkts_rxed++;
                dev->ifstats.multicast_bytes_rxed += len;
               }
            }
            else{
                dev->ifstats.unicast_pkts_rxed++;
                dev->ifstats.unicast_bytes_rxed += len;
            }
        }
#endif
#endif
        /* no buffer copies needed, unless hardware can't
         * use skb buffers.
         */
        status = netif_rx (skb);
        skb = NULL;
        break;

    /* software-driven interface shutdown */
    case -ECONNRESET:       // unlink
    case -ESHUTDOWN:        // disconnect etc
        VDEBUG (dev, "rx shutdown, code %d\n", status);
        goto quiesce;

    /* for hardware automagic (such as pxa) */
    case -ECONNABORTED:     // endpoint reset
        DEBUG (dev, "rx %s reset\n", ep->name);
        defer_kevent (dev, WORK_RX_MEMORY);
quiesce:
        dev_kfree_skb_any (skb);
        goto clean;

    /* data overrun */
    case -EOVERFLOW:
        dev->stats.rx_over_errors++;
        // FALLTHROUGH

    default:
        dev->stats.rx_errors++;
#ifdef CONFIG_MACH_PUMA5
#ifdef USB_MIB_SUPPORT
        dev->ifstats.rx_errors++;
#endif
#endif
        DEBUG (dev, "rx status %d\n", status);
        break;
    }

    if (skb)
        dev_kfree_skb_any (skb);
    if (!netif_running (dev->net)) {
clean:
        spin_lock(&dev->req_lock);
        list_add (&req->list, &dev->rx_reqs);
        spin_unlock(&dev->req_lock);
        req = NULL;
    }
    if (req)
        rx_submit (dev, req, GFP_ATOMIC);
}

static int prealloc (struct list_head *list, struct usb_ep *ep,
            unsigned n, gfp_t gfp_flags)
{
    unsigned        i;
    struct usb_request  *req;

    if (!n)
        return -ENOMEM;

    /* queue/recycle up to N requests */
    i = n;
    list_for_each_entry (req, list, list) {
        if (i-- == 0)
            goto extra;
    }
    while (i--) {
        req = usb_ep_alloc_request (ep, gfp_flags);
        if (!req)
            return list_empty (list) ? -ENOMEM : 0;
        list_add (&req->list, list);
    }
    return 0;

extra:
    /* free extras */
    for (;;) {
        struct list_head    *next;

        next = req->list.next;
        list_del (&req->list);
        usb_ep_free_request (ep, req);

        if (next == list)
            break;

        req = container_of (next, struct usb_request, list);
    }
    return 0;
}

static int alloc_requests (struct eth_dev *dev, unsigned n, gfp_t gfp_flags)
{
    int status;

    spin_lock(&dev->req_lock);
    status = prealloc (&dev->tx_reqs, dev->in_ep, n, gfp_flags);
    if (status < 0)
        goto fail;
    status = prealloc (&dev->rx_reqs, dev->out_ep, n, gfp_flags);
    if (status < 0)
        goto fail;
    goto done;
fail:
    DEBUG (dev, "can't alloc requests\n");
done:
    spin_unlock(&dev->req_lock);
    return status;
}

static void rx_fill (struct eth_dev *dev, gfp_t gfp_flags)
{
    struct usb_request  *req;
    unsigned long       flags;

    /* fill unused rxq slots with some skb */
    spin_lock_irqsave(&dev->req_lock, flags);
    while (!list_empty (&dev->rx_reqs)) {
        req = container_of (dev->rx_reqs.next,
                struct usb_request, list);
        list_del_init (&req->list);
        spin_unlock_irqrestore(&dev->req_lock, flags);

        if (rx_submit (dev, req, gfp_flags) < 0) {
            defer_kevent (dev, WORK_RX_MEMORY);
            return;
        }
        spin_lock_irqsave(&dev->req_lock, flags);
    }
    spin_unlock_irqrestore(&dev->req_lock, flags);
}

static void eth_work (struct work_struct *work)
{
    struct eth_dev      *dev = container_of(work, struct eth_dev, work);

    if (test_and_clear_bit (WORK_RX_MEMORY, &dev->todo)) {
        if (netif_running (dev->net))
            rx_fill (dev, GFP_KERNEL);
    }

    if (dev->todo)
        DEBUG (dev, "work done, flags = 0x%lx\n", dev->todo);
}

static void tx_complete (struct usb_ep *ep, struct usb_request *req)
{
    struct sk_buff  *skb = req->context;
    struct eth_dev  *dev = ep->driver_data;

    switch (req->status) {
    default:
        dev->stats.tx_errors++;
#ifdef CONFIG_MACH_PUMA5
#ifdef USB_MIB_SUPPORT
        dev->ifstats.tx_errors++;
#endif
#endif
        VDEBUG (dev, "tx err %d\n", req->status);
        /* FALLTHROUGH */
    case -ECONNRESET:       // unlink
    case -ESHUTDOWN:        // disconnect etc
        break;
    case 0:
        dev->stats.tx_bytes += skb->len;
    }
    dev->stats.tx_packets++;

    spin_lock(&dev->req_lock);
    list_add (&req->list, &dev->tx_reqs);
    spin_unlock(&dev->req_lock);
    dev_kfree_skb_any (skb);

    atomic_dec (&dev->tx_qlen);
    if (netif_carrier_ok (dev->net))
        netif_wake_queue (dev->net);
}

static inline int eth_is_promisc (struct eth_dev *dev)
{
    /* no filters for the CDC subset; always promisc */
    if (subset_active (dev))
        return 1;
    return dev->cdc_filter & USB_CDC_PACKET_TYPE_PROMISCUOUS;
}
#ifdef CONFIG_MACH_PUMA5
char rndis_get_multicast_status (u8 configNr, const u8 *dest);
#endif
static netdev_tx_t eth_start_xmit (struct sk_buff *skb, struct net_device *net)
{
    struct eth_dev      *dev = netdev_priv(net);
    int         length = skb->len;
    int         retval;
    struct usb_request  *req = NULL;
    unsigned long       flags;
#ifdef CONFIG_MACH_PUMA5

#ifdef CONFIG_USB_ETH_RNDIS
    u8                      dst_mac_address[6];
        u32                     filter = rndis_get_param_filter (dev->rndis_config);
    //u32           filter = dev->cdc_filter;
        memcpy (dst_mac_address, skb->data, 6);
    if (!(filter & RNDIS_PACKET_TYPE_PROMISCUOUS) && dev->rndis )
    {
        if( is_multicast_ether_addr( dst_mac_address) )
            {
                  /* Is the packet a BROADCAST packet ? */
                    if ( is_broadcast_ether_addr(dst_mac_address))
                    {
                            if (!(filter & RNDIS_PACKET_TYPE_BROADCAST))
                            {
                                    dev->stats.tx_dropped++;
                    dev->ifstats.transmit_packets_dropped++;
                                    dev_kfree_skb_any (skb);
                                    return 0;
                            }
                    } else {
                            if ((!(filter & RNDIS_PACKET_TYPE_ALL_MULTICAST)) &&
                                    (!((filter & RNDIS_PACKET_TYPE_MULTICAST) &&
                                    rndis_get_multicast_status (dev->rndis_config,
                                    dst_mac_address))))
                            {
                                    dev->stats.tx_dropped++;
                    dev->ifstats.transmit_packets_dropped++;
                                    dev_kfree_skb_any (skb);
                                    return 0;
                            }
                    }
            } else if (filter & RNDIS_PACKET_TYPE_DIRECTED)
            {
                    if (!((dev->host_mac [0] == dst_mac_address[0]) &&
                        (dev->host_mac [1] == dst_mac_address[1]) &&
                        (dev->host_mac [2] == dst_mac_address[2]) &&
                        (dev->host_mac [3] == dst_mac_address[3]) &&
                        (dev->host_mac [4] == dst_mac_address[4]) &&
                        (dev->host_mac [5] == dst_mac_address[5])
                        ))
                    {
                            dev->stats.tx_dropped++;
                dev->ifstats.transmit_packets_dropped++;
                            dev_kfree_skb_any (skb);
                            return 0;
                    }
        } else if (filter & RNDIS_PACKET_TYPE_BROADCAST)
            {
                    /* Is the packet a BROADCAST packet ? */
                    if ( !(is_broadcast_ether_addr(dst_mac_address)))
                    {
                            dev->stats.tx_dropped++;
                dev->ifstats.transmit_packets_dropped++;
                            dev_kfree_skb_any (skb);
                            return 0;
                    }
            } else if ((filter & RNDIS_PACKET_TYPE_ALL_MULTICAST) ||
                       (filter & RNDIS_PACKET_TYPE_MULTICAST))
            {
                            dev->stats.tx_dropped++;
                dev->ifstats.transmit_packets_dropped++;
                            dev_kfree_skb_any (skb);
                            return 0;
        }
    }

    if( is_multicast_ether_addr( dst_mac_address) )
        {
        /* Is the packet a BROADCAST packet ? */
            if ( is_broadcast_ether_addr(dst_mac_address))
                {
                    dev->ifstats.broadcast_pkts_txed++;
                        dev->ifstats.broadcast_bytes_txed += skb->len;
        }else{
            dev->ifstats.multicast_pkts_txed++;
            dev->ifstats.multicast_bytes_txed += skb->len;
        }
    }else{
               dev->ifstats.unicast_pkts_txed++;
               dev->ifstats.unicast_bytes_txed += skb->len;
    }


#endif
#else
       /* apply outgoing CDC or RNDIS filters */
        if (!eth_is_promisc (dev)) {
                u8              *dest = skb->data;

                if (dest [0] & 0x01) {
            u16 type;

                        /* ignores USB_CDC_PACKET_TYPE_MULTICAST and host
                         * SET_ETHERNET_MULTICAST_FILTERS requests
                         */
                        if (memcmp (dest, net->broadcast, ETH_ALEN) == 0)
                                type = USB_CDC_PACKET_TYPE_BROADCAST;
                        else
                                type = USB_CDC_PACKET_TYPE_ALL_MULTICAST;

                        if (!(dev->cdc_filter & type)) {
                                dev_kfree_skb_any (skb);
                                return 0;
                        }
                }
                /* ignores USB_CDC_PACKET_TYPE_DIRECTED */
        }

#endif

    spin_lock_irqsave(&dev->req_lock, flags);
#ifdef CONFIG_MACH_PUMA5
    if (list_empty (&dev->tx_reqs)){
        netif_stop_queue (net);
        spin_unlock_irqrestore(&dev->req_lock, flags);
        dev_kfree_skb_any (skb);
        return 0;
    }
    else {
#endif
        req = container_of (dev->tx_reqs.next, struct usb_request, list);
        list_del (&req->list);
        if (list_empty (&dev->tx_reqs))
            netif_stop_queue (net);
#ifdef CONFIG_MACH_PUMA5
    }
#endif
    spin_unlock_irqrestore(&dev->req_lock, flags);

    /* no buffer copies needed, unless the network stack did it
     * or the hardware can't use skb buffers.
     * or there's not enough space for any RNDIS headers we need
     */
    if (rndis_active(dev)) {
        struct sk_buff  *skb_rndis;

        skb_rndis = skb_realloc_headroom (skb,
                sizeof (struct rndis_packet_msg_type));
        if (!skb_rndis)
            goto drop;

        dev_kfree_skb_any (skb);
        skb = skb_rndis;
        rndis_add_hdr (skb);
        length = skb->len;
    }
    req->buf = skb->data;
    req->context = skb;
    req->complete = tx_complete;

    /* use zlp framing on tx for strict CDC-Ether conformance,
     * though any robust network rx path ignores extra padding.
     * and some hardware doesn't like to write zlps.
     */
    req->zero = 1;
    if (!dev->zlp && (length % dev->in_ep->maxpacket) == 0)
        length++;

    req->length = length;
#ifdef  CONFIG_USB_GADGET_DUALSPEED
    /* throttle highspeed IRQ rate back slightly */
    req->no_interrupt = (dev->gadget->speed == USB_SPEED_HIGH)
        ? ((atomic_read (&dev->tx_qlen) % TX_DELAY) != 0)
        : 0;
#endif
    retval = usb_ep_queue (dev->in_ep, req, GFP_ATOMIC);
    switch (retval) {
    default:
        DEBUG (dev, "tx queue err %d\n", retval);
        break;
    case 0:
        net->trans_start = jiffies;
        atomic_inc (&dev->tx_qlen);
    }

    if (retval) {
drop:
        dev->stats.tx_dropped++;
#ifdef CONFIG_MACH_PUMA5
#ifdef USB_MIB_SUPPORT
        dev->ifstats.transmit_packets_dropped++;
#endif
#endif
        dev_kfree_skb_any (skb);
        spin_lock_irqsave(&dev->req_lock, flags);
        if (list_empty (&dev->tx_reqs))
            netif_start_queue (net);
        list_add (&req->list, &dev->tx_reqs);
        spin_unlock_irqrestore(&dev->req_lock, flags);
    }
    return NETDEV_TX_OK;
}

/*-------------------------------------------------------------------------*/

#ifdef CONFIG_USB_ETH_RNDIS

/* The interrupt endpoint is used in RNDIS to notify the host when messages
 * other than data packets are available ... notably the REMOTE_NDIS_*_CMPLT
 * messages, but also REMOTE_NDIS_INDICATE_STATUS_MSG and potentially even
 * REMOTE_NDIS_KEEPALIVE_MSG.
 *
 * The RNDIS control queue is processed by GET_ENCAPSULATED_RESPONSE, and
 * normally just one notification will be queued.
 */

static struct usb_request *eth_req_alloc (struct usb_ep *, unsigned, gfp_t);
static void eth_req_free (struct usb_ep *ep, struct usb_request *req);

static void
rndis_control_ack_complete (struct usb_ep *ep, struct usb_request *req)
{
    struct eth_dev          *dev = ep->driver_data;

    if (req->status || req->actual != req->length)
        DEBUG (dev,
            "rndis control ack complete --> %d, %d/%d\n",
            req->status, req->actual, req->length);
    req->context = NULL;

    if (req != dev->stat_req)
        eth_req_free(ep, req);
}

static int rndis_control_ack (struct net_device *net)
{
    struct eth_dev          *dev = netdev_priv(net);
    u32                     length;
    struct usb_request      *resp = dev->stat_req;

    /* in case RNDIS calls this after disconnect */
    if (!dev->status) {
        DEBUG (dev, "status ENODEV\n");
        return -ENODEV;
    }

    /* in case queue length > 1 */
    if (resp->context) {
        resp = eth_req_alloc (dev->status_ep, 8, GFP_ATOMIC);
        if (!resp)
            return -ENOMEM;
    }

    /* Send RNDIS RESPONSE_AVAILABLE notification;
     * USB_CDC_NOTIFY_RESPONSE_AVAILABLE should work too
     */
    resp->length = 8;
    resp->complete = rndis_control_ack_complete;
    resp->context = dev;
    *((__le32 *) resp->buf) = __constant_cpu_to_le32 (1);
    *((__le32 *) resp->buf + 1) = __constant_cpu_to_le32 (0);
    length = usb_ep_queue (dev->status_ep, resp, GFP_ATOMIC);
    if (length < 0) {
        resp->status = 0;
        rndis_control_ack_complete (dev->status_ep, resp);
    }

    return 0;
}

#else

#define rndis_control_ack   NULL

#endif  /* RNDIS */

static void eth_start (struct eth_dev *dev, gfp_t gfp_flags)
{
    DEBUG (dev, "%s\n", __FUNCTION__);

    /* fill the rx queue */
    rx_fill (dev, gfp_flags);

    /* and open the tx floodgates */
    atomic_set (&dev->tx_qlen, 0);
    netif_wake_queue (dev->net);

//#ifdef CONFIG_ARM_AVALANCHE_PPD
#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
#ifdef CONFIG_TI_PACKET_PROCESSOR
    ti_ppm_set_pid_flags (dev->net->pid_handle, 0);
#else
    ti_ppd_set_pid_flags (&pid_usb[0], 0);
#endif
#endif

#ifdef CONFIG_MACH_PUMA5
        netif_carrier_on (dev->net);
#endif
    if (rndis_active(dev)) {
        rndis_set_param_medium (dev->rndis_config,
                    RNDIS_MEDIUM_802_3,
                    BITRATE(dev->gadget)/100);
        (void) rndis_signal_connect (dev->rndis_config);
    }
}

static int eth_open (struct net_device *net)
{
    struct eth_dev      *dev = netdev_priv(net);

    DEBUG (dev, "%s\n", __FUNCTION__);
#ifdef CONFIG_MACH_PUMA5
    /* TAG0003 */
        if (dev->config)
               netif_carrier_on (dev->net);
#endif
//#ifdef CONFIG_ARM_AVALANCHE_PPD
#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
#ifdef CONFIG_TI_PACKET_PROCESSOR
    ti_ppm_set_pid_flags (net->pid_handle, 0);
    uninstall_usb_pid( net, rndis_default_mode, true );
      install_usb_pid( net, rndis_default_mode, true );
#else
    ti_ppd_set_pid_flags (&pid_usb[0], 0);
    uninstall_usb_pid( net, rndis_default_mode, true );
      install_usb_pid( net, rndis_default_mode, true );
#endif
#endif


    if (netif_carrier_ok (dev->net))
        eth_start (dev, GFP_KERNEL);
    return 0;
}

static int eth_stop (struct net_device *net)
{
    struct eth_dev      *dev = netdev_priv(net);

    VDEBUG (dev, "%s\n", __FUNCTION__);
    netif_stop_queue (net);
#ifdef CONFIG_MACH_PUMA5
        netif_carrier_off (dev->net);
#endif

//#ifdef CONFIG_ARM_AVALANCHE_PPD
#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
#ifdef CONFIG_TI_PACKET_PROCESSOR
    ti_ppm_set_pid_flags (net->pid_handle, TI_PP_PID_DISCARD_ALL_RX);
    uninstall_usb_pid( net, rndis_default_mode, false );
#else
    ti_ppd_set_pid_flags (&pid_usb[0], TI_PP_PID_DISCARD_ALL_RX);
    uninstall_usb_pid( net, rndis_default_mode, false );
#endif
    /* this delay is to make sure all the packets with the PID successfully egress throgh the respective ports.*/
    mdelay(200);
#endif



    DEBUG (dev, "stop stats: rx/tx %ld/%ld, errs %ld/%ld\n",
        dev->stats.rx_packets, dev->stats.tx_packets,
        dev->stats.rx_errors, dev->stats.tx_errors
        );

    /* ensure there are no more active requests */
    if (dev->config) {

#ifdef CONFIG_MACH_PUMA5
        /* TAG0001 */
        /*
                 * Removed as we allow the requests to drain naturally.
                 * Due to abscense of Flush All requests premitive we need
                 * to allow them to gets flushed through normal IO mechanism.
                 * Using Disable/Enable approach to flush the request is bug
                 * prone as the controller driver can reset the Data Toggle and
                 * this minght lead this EP to completely malfunction.
                 */
#endif
#ifndef CONFIG_MACH_PUMA5
        usb_ep_disable (dev->in_ep);
        usb_ep_disable (dev->out_ep);
        if (netif_carrier_ok (dev->net)) {
            DEBUG (dev, "host still using in/out endpoints\n");
            // FIXME idiom may leave toggle wrong here
            dev->in_ep->desc = dev->in;
            usb_ep_enable (dev->in_ep);
            dev->out_ep->desc = dev->out;
            usb_ep_enable (dev->out_ep);
        }
        if (dev->status_ep) {
            usb_ep_disable (dev->status_ep);
            dev->status_ep->desc = dev->status;
            usb_ep_enable (dev->status_ep);
        }
#endif
    }

    if (rndis_active(dev)) {
        rndis_set_param_medium (dev->rndis_config,
                    RNDIS_MEDIUM_802_3, 0);
#ifdef CONFIG_MACH_PUMA5
        (void) rndis_signal_disconnect (dev->rndis_config, 1);
#else
    (void) rndis_signal_disconnect (dev->rndis_config);
#endif
    }

    return 0;
}

/*-------------------------------------------------------------------------*/

static struct usb_request *
eth_req_alloc (struct usb_ep *ep, unsigned size, gfp_t gfp_flags)
{
    struct usb_request  *req;

    req = usb_ep_alloc_request (ep, gfp_flags);
    if (!req)
        return NULL;

    req->buf = kmalloc (size, gfp_flags);
    if (!req->buf) {
        usb_ep_free_request (ep, req);
        req = NULL;
    }
    return req;
}

static void
eth_req_free (struct usb_ep *ep, struct usb_request *req)
{
    kfree (req->buf);
    usb_ep_free_request (ep, req);
}


static void /* __init_or_exit */
eth_unbind (struct usb_gadget *gadget)
{
    struct eth_dev      *dev = get_gadget_data (gadget);

    DEBUG (dev, "unbind\n");
    rndis_deregister (dev->rndis_config);
    rndis_exit ();

    /* we've already been disconnected ... no i/o is active */
    if (dev->req) {
        eth_req_free (gadget->ep0, dev->req);
        dev->req = NULL;
    }
    if (dev->stat_req) {
        eth_req_free (dev->status_ep, dev->stat_req);
        dev->stat_req = NULL;
    }

//#ifdef CONFIG_ARM_AVALANCHE_PPD
#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
#ifdef CONFIG_TI_PACKET_PROCESSOR
    ti_ppm_delete_pid (dev->net->pid_handle);
#else
    ti_ppd_delete_pid(PP_USB_PID_BASE+0);
#endif
    /* this delay is to make sure all the packets with the PID successfully egress throgh the respective ports.*/
    mdelay(200);
#endif

    /* Temporary Fix USB stability .. This is not the final fix ! */
    dev->net->vpid_handle = -1;

    unregister_netdev (dev->net);
    free_netdev(dev->net);

    /* assuming we used keventd, it must quiesce too */
    flush_scheduled_work ();
    set_gadget_data (gadget, NULL);
}

static u8 nibble (unsigned char c)
{
    if (likely (isdigit (c)))
        return c - '0';
    c = toupper (c);
    if (likely (isxdigit (c)))
        return 10 + c - 'A';
    return 0;
}

#ifndef MODULE

static int __init usb_set_host_macstring(char *str)
{
    if(str[1]) {
        host_addr = &str[1];
    }
    return 1;
}

__setup("usbhostaddr", usb_set_host_macstring);

#endif

static int get_ether_addr(const char *str, u8 *dev_addr)
{
    if (str) {
        unsigned    i;

        for (i = 0; i < 6; i++) {
            unsigned char num;

            if((*str == '.') || (*str == ':'))
                str++;
            num = nibble(*str++) << 4;
            num |= (nibble(*str++));
            dev_addr [i] = num;
        }
        if (is_valid_ether_addr (dev_addr))
            return 0;
    }
    eth_random_addr(dev_addr);
    return 1;
}

static const struct net_device_ops eth_netdev_ops = {
    .ndo_open               = eth_open,
    .ndo_stop               = eth_stop,
    .ndo_change_mtu         = g_eth_change_mtu,
    .ndo_get_stats          = eth_get_stats,
#ifdef CONFIG_MACH_PUMA5
#ifdef USB_MIB_SUPPORT
    .ndo_do_ioctl           = eth_ioctl,
#endif
#endif
    .ndo_start_xmit         = eth_start_xmit,
    .ndo_change_mtu         = g_eth_change_mtu,
    .ndo_set_mac_address    = eth_mac_addr,
    .ndo_validate_addr      = eth_validate_addr,
};

static struct device_type gadget_type = {
        .name   = "gadget",
};

DECLARE_PROCFS_ENTRY(usb_stats, proc_read_usb_stats, proc_write_usb_stats);
DECLARE_PROCFS_ENTRY(usb_link, proc_read_usb_link, NULL);
DECLARE_PROCFS_ENTRY(usb_info, proc_read_usb_info, NULL);
DECLARE_PROCFS_ENTRY(usb_cppi, proc_read_usb_cppi, proc_write_usb_cppi);
DECLARE_PROCFS_ENTRY(usb_config_mode, proc_read_usb_config_mode,  proc_write_usb_config_mode); 

static int 
eth_bind (struct usb_gadget *gadget)
{
    struct eth_dev      *dev;
    struct net_device   *net;
    u8          cdc = 1, zlp = 1, rndis = 1;
    struct usb_ep       *in_ep, *out_ep, *status_ep = NULL;
    int         status = -ENOMEM;
    int         gcnum;
#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    int pid_type;
#endif
    /* these flags are only ever cleared; compiler take note */
#ifndef DEV_CONFIG_CDC
    cdc = 0;
#endif
#ifndef CONFIG_USB_ETH_RNDIS
    rndis = 0;
#endif

    /* Because most host side USB stacks handle CDC Ethernet, that
     * standard protocol is _strongly_ preferred for interop purposes.
     * (By everyone except Microsoft.)
     */
#if 0
    if (gadget_is_pxa (gadget)) {
        /* pxa doesn't support altsettings */
        cdc = 0;
    } else if (gadget_is_sh(gadget)) {
        /* sh doesn't support multiple interfaces or configs */
        cdc = 0;
        rndis = 0;
    } else if (gadget_is_sa1100 (gadget)) {
        /* hardware can't write zlps */
        zlp = 0;
        /* sa1100 CAN do CDC, without status endpoint ... we use
         * non-CDC to be compatible with ARM Linux-2.4 "usb-eth".
         */
        cdc = 0;
    }
#endif
    gcnum = gadget_is_musbhdrc (gadget) ? 0x16 : -ENOENT;
    if (gcnum >= 0)
        device_desc.bcdDevice = cpu_to_le16 (0x0200 + gcnum);
    else {
        /* can't assume CDC works.  don't want to default to
         * anything less functional on CDC-capable hardware,
         * so we fail in this case.
         */
        dev_err (&gadget->dev,
            "controller '%s' not recognized\n",
            gadget->name);
        return -ENODEV;
    }
    snprintf (manufacturer, sizeof manufacturer, "%s %s/%s",
        init_uts_ns.name.sysname, init_uts_ns.name.release,
        gadget->name);

    /* If there's an RNDIS configuration, that's what Windows wants to
     * be using ... so use these product IDs here and in the "linux.inf"
     * needed to install MSFT drivers.  Current Linux kernels will use
     * the second configuration if it's CDC Ethernet, and need some help
     * to choose the right configuration otherwise.
     */
    if (rndis) {
        device_desc.idVendor =
            __constant_cpu_to_le16(RNDIS_VENDOR_NUM);
        device_desc.idProduct =
            __constant_cpu_to_le16(RNDIS_PRODUCT_NUM);
        snprintf (product_desc, sizeof product_desc,
            "RNDIS/%s", driver_desc);
    /* CDC subset ... recognized by Linux since 2.4.10, but Windows
     * drivers aren't widely available.
     */
    } else if (!cdc) {
        device_desc.bDeviceClass = USB_CLASS_VENDOR_SPEC;
        device_desc.idVendor =
            __constant_cpu_to_le16(SIMPLE_VENDOR_NUM);
        device_desc.idProduct =
            __constant_cpu_to_le16(SIMPLE_PRODUCT_NUM);
    }
    /* support optional vendor/distro customization */
    if (idVendor) {
        if (!idProduct) {
            dev_err (&gadget->dev, "idVendor needs idProduct!\n");
            return -ENODEV;
        }
        device_desc.idVendor = cpu_to_le16(idVendor);
        device_desc.idProduct = cpu_to_le16(idProduct);
        if (bcdDevice)
            device_desc.bcdDevice = cpu_to_le16(bcdDevice);
    }
#if defined(CONFIG_MACH_PUMA5) && defined(DEFAULT_USB_VENDOR_ID)
    else {
        device_desc.idVendor = cpu_to_le16(DEFAULT_USB_VENDOR_ID);
        device_desc.idProduct = cpu_to_le16(DEFAULT_USB_PRODUCT_ID);
    }
#endif
    if (iManufacturer)
        strlcpy (manufacturer, iManufacturer, sizeof manufacturer);
#if defined(CONFIG_MACH_PUMA5) && defined(DEFAULT_USB_VENDOR_NAME)
    else
        strlcpy (manufacturer, DEFAULT_USB_VENDOR_NAME, sizeof manufacturer);
#endif

    if (iProduct)
        strlcpy (product_desc, iProduct, sizeof product_desc);
#if defined(CONFIG_MACH_PUMA5) && defined(DEFAULT_USB_VENDOR_DESC)
    else
        strlcpy (product_desc, DEFAULT_USB_VENDOR_DESC, sizeof product_desc);
#endif
    if (iSerialNumber) {
        device_desc.iSerialNumber = STRING_SERIALNUMBER,
        strlcpy(serial_number, iSerialNumber, sizeof serial_number);
    }
#if defined(CONFIG_MACH_PUMA5) && defined(DEFAULT_USB_INSTANCE)
    else{
        device_desc.iSerialNumber = STRING_SERIALNUMBER,
        strlcpy(serial_number, DEFAULT_USB_INSTANCE, sizeof serial_number);
    }
#endif

    /* all we really need is bulk IN/OUT */
    usb_ep_autoconfig_reset (gadget);
    in_ep = usb_ep_autoconfig (gadget, &fs_source_desc);
    if (!in_ep) {
autoconf_fail:
        dev_err (&gadget->dev,
            "can't autoconfigure on %s\n",
            gadget->name);
        return -ENODEV;
    }
    in_ep->driver_data = in_ep; /* claim */

    out_ep = usb_ep_autoconfig (gadget, &fs_sink_desc);
    if (!out_ep){
        goto autoconf_fail;
    }
    out_ep->driver_data = out_ep;   /* claim */

#if defined(DEV_CONFIG_CDC) || defined(CONFIG_USB_ETH_RNDIS)
    /* CDC Ethernet control interface doesn't require a status endpoint.
     * Since some hosts expect one, try to allocate one anyway.
     */
    if (cdc || rndis) {
        status_ep = usb_ep_autoconfig (gadget, &fs_status_desc);
        if (status_ep) {
            status_ep->driver_data = status_ep; /* claim */
        } else if (rndis) {
            dev_err (&gadget->dev,
                "can't run RNDIS on %s\n",
                gadget->name);
            return -ENODEV;
#ifdef DEV_CONFIG_CDC
        /* pxa25x only does CDC subset; often used with RNDIS */
        } else if (cdc) {
            control_intf.bNumEndpoints = 0;
            /* FIXME remove endpoint from descriptor list */
#endif
        }
    }
#endif

    /* one config:  cdc, else minimal subset */
    if (!cdc) {
        eth_config.bNumInterfaces = 1;
        eth_config.iConfiguration = STRING_SUBSET;
        fs_subset_descriptors();
        hs_subset_descriptors();
    }

    device_desc.bMaxPacketSize0 = gadget->ep0->maxpacket;
    usb_gadget_set_selfpowered (gadget);

    /* For now RNDIS is always a second config */
    if (rndis)
        device_desc.bNumConfigurations = 2;

#ifdef  CONFIG_USB_GADGET_DUALSPEED
    if (rndis)
        dev_qualifier.bNumConfigurations = 2;
    else if (!cdc)
        dev_qualifier.bDeviceClass = USB_CLASS_VENDOR_SPEC;

    /* assumes ep0 uses the same value for both speeds ... */
    dev_qualifier.bMaxPacketSize0 = device_desc.bMaxPacketSize0;

    /* and that all endpoints are dual-speed */
    hs_source_desc.bEndpointAddress = fs_source_desc.bEndpointAddress;
    hs_sink_desc.bEndpointAddress = fs_sink_desc.bEndpointAddress;
#if defined(DEV_CONFIG_CDC) || defined(CONFIG_USB_ETH_RNDIS)
    if (status_ep)
        hs_status_desc.bEndpointAddress =
                fs_status_desc.bEndpointAddress;
#endif
#endif  /* DUALSPEED */

    if (gadget_is_otg(gadget)) {
        otg_descriptor.bmAttributes |= USB_OTG_HNP,
        eth_config.bmAttributes |= USB_CONFIG_ATT_WAKEUP;
        eth_config.bMaxPower = 4;
#ifdef  CONFIG_USB_ETH_RNDIS
        rndis_config.bmAttributes |= USB_CONFIG_ATT_WAKEUP;
        rndis_config.bMaxPower = 4;
#endif
    }

    net = alloc_etherdev (sizeof *dev);
    if (!net)
        return status;
    dev = netdev_priv(net);
    spin_lock_init (&dev->lock);
    spin_lock_init (&dev->req_lock);
    INIT_WORK (&dev->work, eth_work);
    INIT_LIST_HEAD (&dev->tx_reqs);
    INIT_LIST_HEAD (&dev->rx_reqs);
#ifdef CONFIG_MACH_PUMA5
#ifdef USB_MIB_SUPPORT
    init_mib_counters(net);
#endif
#endif
    /* network device setup */
    dev->net = net;
    snprintf (net->name, sizeof(net->name), "usb%%d");
    dev->cdc = cdc;
    dev->zlp = zlp;
    dev->in_ep = in_ep;
    dev->out_ep = out_ep;
    dev->status_ep = status_ep;

    dev->rndis_configured = 0;
    /* Module params for these addresses should come from ID proms.
     * The host side address is used with CDC and RNDIS, and commonly
     * ends up in a persistent config database.
     */
#if defined(CONFIG_MACH_PUMA5) && defined(DEFAULT_USB_INSTANCE)
    if( !dev_addr )
    {
        dev_addr = (char *)DEFAULT_USB_DEVICE_MAC_ADDRESS;
    }

    if ( puma5_usb_host_address )
    {
        host_addr = puma5_usb_host_address;
    }

    if( !host_addr )
    {
        host_addr = (char *)DEFAULT_USB_PC_MAC_ADDRESS;
    }
#endif
    if (get_ether_addr(dev_addr, net->dev_addr)){
        dev_warn(&gadget->dev,
            "using random %s ethernet address\n", "self");
            dprintk("using random %s ethernet address\n", "self");
    }
    if (cdc || rndis) {
        if (get_ether_addr(host_addr, dev->host_mac)){
            dev_warn(&gadget->dev,
                "using random %s ethernet address\n", "host");
            dprintk(    "using random %s ethernet address\n", "host");
        }
#ifdef  DEV_CONFIG_CDC
        snprintf (ethaddr, sizeof ethaddr, "%02X%02X%02X%02X%02X%02X",
            dev->host_mac [0], dev->host_mac [1],
            dev->host_mac [2], dev->host_mac [3],
            dev->host_mac [4], dev->host_mac [5]);
#endif
    }

    if (rndis) {
        status = rndis_init();
        if (status < 0) {
            dev_err (&gadget->dev, "can't init RNDIS, %d\n",
                status);
            goto fail;
        }
    }

#if 0
    net->change_mtu = g_eth_change_mtu;
    net->get_stats = eth_get_stats;
    net->hard_start_xmit = eth_start_xmit;
    net->open = eth_open;
#ifdef CONFIG_MACH_PUMA5
#ifdef USB_MIB_SUPPORT
    net->do_ioctl = eth_ioctl;
#endif
#endif
    net->stop = eth_stop;
#endif
     net->netdev_ops = &eth_netdev_ops;


#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
{
    int i;
    if( rndis_default_mode )
        pid_type = TI_PP_PID_TYPE_USBRNDIS;
    else
        pid_type = TI_PP_PID_TYPE_USBCDC;

    printk("%s: create usb pids(",__FUNCTION__);
    for(i=0; i<4; ++i){
#ifdef CONFIG_TI_PACKET_PROCESSOR
        net->pid_handle                     = usb_add_pid (i,pid_type);
        net->vpid_block.type                = TI_PP_ETHERNET;
        net->vpid_block.parent_pid_handle   = net->pid_handle;
        net->vpid_block.egress_mtu          = 0;
        net->vpid_block.priv_tx_data_len    = 0;

#else /* !CONFIG_TI_PACKET_PROCESSOR */
            printk(" %d", usb_add_pid (i,pid_type));
#endif /* CONFIG_ARM_AVALANCHE_PPD */
    }
    printk(" )\n");
}
#endif
    // watchdog_timeo, tx_timeout ...
    // set_multicast_list
    SET_ETHTOOL_OPS(net, &ops);

    /* preallocate control message data and buffer */
    dev->req = eth_req_alloc (gadget->ep0, USB_BUFSIZ, GFP_KERNEL);
    if (!dev->req)
        goto fail;
    dev->req->complete = eth_setup_complete;

    /* ... and maybe likewise for status transfer */
#if defined(DEV_CONFIG_CDC) || defined(CONFIG_USB_ETH_RNDIS)
    if (dev->status_ep) {
        dev->stat_req = eth_req_alloc (dev->status_ep,
                    STATUS_BYTECOUNT, GFP_KERNEL);
        if (!dev->stat_req) {
            eth_req_free (gadget->ep0, dev->req);
            goto fail;
        }
        dev->stat_req->context = NULL;
    }
#endif

    /* finish hookup to lower layer ... */
    dev->gadget = gadget;
    set_gadget_data (gadget, dev);
    gadget->ep0->driver_data = dev;

    /* two kinds of host-initiated state changes:
     *  - iff DATA transfer is active, carrier is "on"
     *  - tx queueing enabled if open *and* carrier is "on"
     */
    netif_stop_queue (dev->net);
    netif_carrier_off (dev->net);

    SET_NETDEV_DEV (dev->net, &gadget->dev);
    SET_NETDEV_DEVTYPE(dev->net, &gadget_type);
    status = register_netdev (dev->net);
    if (status < 0)
        goto fail1;

    INFO (dev, "%s, version: " DRIVER_VERSION "\n", driver_desc);
    INFO (dev, "using %s, OUT %s IN %s%s%s\n", gadget->name,
        out_ep->name, in_ep->name,
        status_ep ? " STATUS " : "",
        status_ep ? status_ep->name : ""
        );
    INFO (dev, "MAC %02x:%02x:%02x:%02x:%02x:%02x\n",
        net->dev_addr [0], net->dev_addr [1],
        net->dev_addr [2], net->dev_addr [3],
        net->dev_addr [4], net->dev_addr [5]);

    if (cdc || rndis){
        INFO (dev, "HOST MAC %02x:%02x:%02x:%02x:%02x:%02x\n",
            dev->host_mac [0], dev->host_mac [1],
            dev->host_mac [2], dev->host_mac [3],
            dev->host_mac [4], dev->host_mac [5]);


                INFO(dev,"VendorID   %04x\n", le16_to_cpu(device_desc.idVendor));
                INFO(dev,"ProductID  %04x\n", le16_to_cpu(device_desc.idProduct));
        }



    if (rndis) {
        u32 vendorID = 0;

        /* FIXME RNDIS vendor id == "vendor NIC code" == ? */

        dev->rndis_config = rndis_register (rndis_control_ack);
        if (dev->rndis_config < 0) {
fail0:
            unregister_netdev (dev->net);
            status = -ENODEV;
            goto fail;
        }

        /* these set up a lot of the OIDs that RNDIS needs */
        rndis_set_host_mac (dev->rndis_config, dev->host_mac);
        if (rndis_set_param_dev (dev->rndis_config, dev->net,
                     &dev->stats, &dev->cdc_filter))
            goto fail0;
        if (rndis_set_param_vendor (dev->rndis_config, vendorID,
                        manufacturer))
            goto fail0;
        if (rndis_set_param_medium (dev->rndis_config,
                        RNDIS_MEDIUM_802_3,
                        0))
            goto fail0;

//                INFO(dev,"VendorID   %04x\n", le16_to_cpu(device_desc.idVendor));
//                INFO(dev,"ProductID  %04x\n", le16_to_cpu(device_desc.idProduct));
                INFO (dev, "RNDIS ready\n");
        }


#ifdef CONFIG_MACH_PUMA5

#ifdef USB_MIB_SUPPORT

        /* mib counter support */

        dev->pde = proc_create_data("avalanche/usb_stats", S_IFREG | S_IRUGO | S_IWUSR, NULL, &usb_stats_proc_fops, net);
        if( !dev->pde ){
            printk("\n could not create avalanche/usb_stats proc entry");
		}

        dev->usblink_pde = proc_create_data("avalanche/usb_link", S_IFREG | S_IRUGO | S_IWUSR, NULL, &usb_link_proc_fops, net);
        if( !dev->pde ){
            printk("\n could not create avalanche/usb_link proc entry");
		}

        dev->usbinfo_pde = proc_create_data("avalanche/usb_info", S_IFREG | S_IRUGO | S_IWUSR, NULL, &usb_info_proc_fops, net);
        if( !dev->pde ){
            printk("\n could not create avalanche/usb_info proc entry");
		}
#ifdef USB_PERFCNT_SELECT
        dev->usbcppi_pde = proc_create_data("avalanche/usb_cppi", S_IFREG | S_IRUGO | S_IWUSR, NULL, &usb_cppi_proc_fops, net);
                if( !dev->pde ){
                        printk("\n could not create avalanche/usb_cppi proc entry");
				}
#endif
        dev->usbixia_pde = proc_create_data("avalanche/usb_config_mode", S_IFREG | S_IRUGO | S_IWUSR, NULL, &usb_config_mode_proc_fops, net);
                if( !dev->pde ){
                        printk("\n could not create avalanche/usb_config_mode proc entry");
				}
#endif
#endif


    return status;

fail1:
    dev_dbg(&gadget->dev, "register_netdev failed, %d\n", status);
fail:
    eth_unbind (gadget);
    return status;
}

/*-------------------------------------------------------------------------*/

static void
eth_suspend (struct usb_gadget *gadget)
{
    struct eth_dev      *dev = get_gadget_data (gadget);

    DEBUG (dev, "suspend\n");
    dev->suspended = 1;
}

static void
eth_resume (struct usb_gadget *gadget)
{
    struct eth_dev      *dev = get_gadget_data (gadget);

    DEBUG (dev, "resume\n");
    dev->suspended = 0;
}

/*-------------------------------------------------------------------------*/

static struct usb_gadget_driver eth_driver = {
    .max_speed      = DEVSPEED,

    .function   = (char *) driver_desc,
    .bind       = eth_bind,
    .unbind     = eth_unbind,

    .setup      = eth_setup,
    .disconnect = eth_disconnect,

    .suspend    = eth_suspend,
    .resume     = eth_resume,

    .driver = {
        .name       = (char *) shortname,
        .owner      = THIS_MODULE,
    },
};

MODULE_DESCRIPTION (DRIVER_DESC);
MODULE_AUTHOR ("David Brownell, Benedikt Spanger");
MODULE_LICENSE ("GPL");


#ifdef CONFIG_MACH_PUMA5
int avalanche_prefetcher_register_callback(void (*cb)(int));

/* will be called from  */
static void usb_gadget_ether_deferred_init(int fail)
{
    if (fail || usb_gadget_probe_driver(&eth_driver)) {
        printk(KERN_EMERG "Failed to init usb ethernet gadget\n");
    }
}

static int __init init (void)
{
    return avalanche_prefetcher_register_callback(usb_gadget_ether_deferred_init);
    /* real init in usb_gadget_ether_deferred_init will be called on firmware download
       or immediately if it's already downloaded */
}
#else
static int __init init (void)
{
    return usb_gadget_probe_driver(&eth_driver);
}
#endif
module_init (init);

static void __exit cleanup (void)
{
    usb_gadget_unregister_driver (&eth_driver);
}
module_exit (cleanup);

#ifdef CONFIG_MACH_PUMA5
#ifdef USB_MIB_SUPPORT
static int init_mib_counters(struct net_device *p_dev)
{
    struct eth_dev          *dev;
    if( !p_dev )
        return -1;

    dev = netdev_priv(p_dev);

        /* clear Transmission counters. */
    dev->ifstats.broadcast_pkts_txed = 0;
    dev->ifstats.broadcast_bytes_txed = 0;
    dev->ifstats.multicast_pkts_txed = 0;
    dev->ifstats.multicast_bytes_txed = 0;
    dev->ifstats.unicast_pkts_txed = 0;
    dev->ifstats.unicast_bytes_txed = 0;
    dev->ifstats.transmit_packets_dropped = 0;
    dev->ifstats.tx_errors = 0;

    /* clear Receive counters */
    dev->ifstats.broadcast_pkts_rxed = 0;
    dev->ifstats.broadcast_bytes_rxed = 0 ;
    dev->ifstats.multicast_pkts_rxed = 0;
    dev->ifstats.multicast_bytes_rxed = 0;
    dev->ifstats.unicast_pkts_rxed = 0;
    dev->ifstats.unicast_bytes_rxed = 0;
    dev->ifstats.receive_packets_dropped = 0;
    dev->ifstats.rx_errors = 0;
    dev->ifstats.unknownProtPkts = 0;
    return 0;

}
int rndis_usb_get_mib_stats(struct net_device *p_dev, int command, void *data)
{
    int error_status = 0;
    struct eth_dev          *dev;
    dev = netdev_priv(p_dev);
    switch(command)
    {
            case TI_SIOCGINTFCOUNTERS:
        {
            USB_MIB2_IF_COUNTERS   *ptr_if_cntr;
                CDC_RNDIS_STATS        *ptr_rndis_stats;

                ptr_rndis_stats = &dev->ifstats;
                ptr_if_cntr = (USB_MIB2_IF_COUNTERS *)data;

            memset((void *)ptr_if_cntr, 0 , sizeof(USB_MIB2_IF_COUNTERS));

                ptr_if_cntr->inBytesLow = ptr_rndis_stats->unicast_bytes_rxed +
                                      ptr_rndis_stats->broadcast_bytes_rxed +
                                      ptr_rndis_stats->multicast_bytes_rxed ;

                ptr_if_cntr->inUnicastPktsLow = ptr_rndis_stats->unicast_pkts_rxed;
                ptr_if_cntr->inBroadcastPktsLow = ptr_rndis_stats->broadcast_pkts_rxed;
                ptr_if_cntr->inMulticastPktsLow = ptr_rndis_stats->multicast_pkts_rxed;
                ptr_if_cntr->inDiscardPkts = ptr_rndis_stats->receive_packets_dropped;

                ptr_if_cntr->outBytesLow = ptr_rndis_stats->unicast_bytes_txed +
                                      ptr_rndis_stats->broadcast_bytes_txed +
                                      ptr_rndis_stats->multicast_bytes_txed ;

                ptr_if_cntr->outUnicastPktsLow = ptr_rndis_stats->unicast_pkts_txed;
                ptr_if_cntr->outBroadcastPktsLow = ptr_rndis_stats->broadcast_pkts_txed;
                ptr_if_cntr->outMulticastPktsLow = ptr_rndis_stats->multicast_pkts_txed;
                ptr_if_cntr->outDiscardPkts = ptr_rndis_stats->transmit_packets_dropped;

        break;
        }

/*      case TI_SIOCGUSBPARAMS:
        {
        break;
        }

            case TI_SIOCGETHERCOUNTERS:
            case TI_SIOCGUSBCOUNTERS:
            {
        break;
        }
*/
        case TI_SIOCSINTFADMINSTATUS:
            {
#if 0
                    USB_MIB2_IF_COMMAND * ptr_if_cmd;
                    ptr_if_cmd = (USB_MIB2_IF_COMMAND *)data;

                    if(ptr_if_cmd->ifAdminStatus == MIB2_STATUS_UP
                                || ptr_if_cmd->ifAdminStatus == MIB2_STATUS_DOWN)
            /* Only these states are supported at present */
                    {
                         ptr_cdc_rndis_mcb->mib2_intf_status = ptr_if_cmd->ifAdminStatus;
                    }
            else
                            error_status=-1;
#endif
                break;
            }

            case TI_SIOCGINTFPARAMS:
            {
                   USB_MIB2_IF_PARAMS * ptr_if_param;
               ptr_if_param = (USB_MIB2_IF_PARAMS *)data;

           memset((void *)ptr_if_param, 0 , sizeof(USB_MIB2_IF_PARAMS));

                switch (dev->gadget->speed) {
                    case USB_SPEED_FULL:
                            ptr_if_param->ifSpeed = 12000000;
                        break;
#ifdef CONFIG_USB_GADGET_DUALSPEED
                    case USB_SPEED_HIGH:
                            ptr_if_param->ifSpeed = 480000000;
                            break;
#endif
                        default:
                        ptr_if_param->ifSpeed = 0;
                        break;
                }

                ptr_if_param->ifHighSpeed = 0; /* TODO */
                ptr_if_param->ifOperStatus = /* TODO down, up, dormant */
                ((p_dev->flags & IFF_UP) ? MIB2_STATUS_UP : MIB2_STATUS_DOWN);

                ptr_if_param->ifPromiscuousMode =
                ((p_dev->flags & IFF_PROMISC) ? 1 : 0 );

                break;
            }

            default:
        {
                break;
            }
    }
    return error_status;
}

typedef struct {
    unsigned int cmd;       /**< Command */
    void *data;             /**< Data provided with the command - depending upon command */
} ethDrvPrivIoctl;
#define USB_SET_HOSTMAC_ADDR 1
/* Ioctl function */
static int eth_ioctl(struct net_device *p_dev, struct ifreq *rq, int cmd)
{
    int ret = 0;
    ethDrvPrivIoctl privIoctl;
    struct eth_dev          *dev = netdev_priv(p_dev);
    char host_mac_addr[18];

    if (cmd == SIOCDEVPRIVATE) {
        if (copy_from_user((char *) &privIoctl, (char *) rq->ifr_data,sizeof(ethDrvPrivIoctl))){
                return -EFAULT;
        }
        switch (privIoctl.cmd) {
                    /* Program Type 2/3 Address Filter */
                case USB_SET_HOSTMAC_ADDR:
            if (copy_from_user
                        ((char *)host_mac_addr, (char *) privIoctl.data,
                         sizeof(host_mac_addr))){
                        return -EFAULT;
            }
            if (get_ether_addr(host_mac_addr, dev->host_mac)){
                            printk( "invalid host mac address\n");
                    }
            printk("host_mac changed to %02x:%02x:%02x:%02x:%02x:%02x\n",dev->host_mac[0],dev->host_mac[1],
                dev->host_mac[2],dev->host_mac[3],dev->host_mac[4],dev->host_mac[5]);

            rndis_set_host_mac (dev->rndis_config, dev->host_mac);
            break;
            default:
                    return -EFAULT;
                    break;
        }

    } else if (cmd == SIOTIMIB2) {
        TI_SNMP_CMD_T ti_snmp_cmd;

    /* copy from user data */
    if( copy_from_user((char *)&ti_snmp_cmd, (char *)rq->ifr_data,
                sizeof(TI_SNMP_CMD_T)))
        return -EFAULT ;

    switch( ti_snmp_cmd.cmd ) {
    case TI_SIOCGINTFCOUNTERS:
        {
              USB_MIB2_IF_COUNTERS localCounters;
                  ret = rndis_usb_get_mib_stats(p_dev, ti_snmp_cmd.cmd, (void *) &localCounters);

          if( !ret ){
                /* copy to user data */
                   if( copy_to_user( (char *)ti_snmp_cmd.data,
                       (char *)&localCounters, sizeof(USB_MIB2_IF_COUNTERS)))
                           return -EFAULT;
          }else
            return -EFAULT;

        break;
        }
    case TI_SIOCGINTFPARAMS:
        {
                   USB_MIB2_IF_PARAMS localParams;
                   ret = rndis_usb_get_mib_stats(p_dev,ti_snmp_cmd.cmd, (void *) &localParams);

           if( !ret ){
                 /* copy to mib counters to user data */
               if( copy_to_user((char *)ti_snmp_cmd.data,
                 (char *)&localParams, sizeof(USB_MIB2_IF_PARAMS)))
                return -EFAULT;
           }else
            return - EFAULT;

        break;
        }
    case TI_SIOCGETHERCOUNTERS:
        {

        break;
        }
    case TI_SIOCGETHERPARAMS:
            {
        break;
        }

    case TI_SIOCSINTFADMINSTATUS:
        {
        USB_MIB2_IF_COMMAND localParams;
                ret = rndis_usb_get_mib_stats(p_dev, ti_snmp_cmd.cmd, (void *) &localParams);

//      localParams.ifAdminStatus = (p_dev->flags & IFF_UP) ? 1 :2;

        if( !ret ){
            /* copy to user data */
            if( copy_to_user( (char *)ti_snmp_cmd.data,
                (char *)&localParams, sizeof(USB_MIB2_IF_COMMAND)))
               return -EFAULT;
        }else
            return -EFAULT;
        break;
        }
    }


    }/* elseif */
    return 0;
}

static int proc_read_usb_stats(struct seq_file *m, void *v)
{
        struct eth_dev          *dev;
        struct net_device       *p_dev;

        p_dev = (struct net_device *)m->private;
        if( !p_dev )
                return -EINVAL;

        dev = netdev_priv(p_dev);

              /* clear Transmission counters. */
        seq_printf(m," USB-MIB Counters - Tx \n");
        seq_printf(m,"\t ifstats.broadcast_pkts_txed      = %u \n", dev->ifstats.broadcast_pkts_txed);
        seq_printf(m,"\t ifstats.broadcast_bytes_txed     = %u \n",dev->ifstats.broadcast_bytes_txed) ;
        seq_printf(m,"\t ifstats.multicast_pkts_txed      = %u \n",dev->ifstats.multicast_pkts_txed);
        seq_printf(m,"\t ifstats.multicast_bytes_txed     = %u\n",dev->ifstats.multicast_bytes_txed);
        seq_printf(m,"\t ifstats.unicast_pkts_txed        = %u\n",dev->ifstats.unicast_pkts_txed );
        seq_printf(m,"\t ifstats.unicast_bytes_txed       = %u\n",dev->ifstats.unicast_bytes_txed );
        seq_printf(m,"\t ifstats.transmit_packets_dropped = %u\n",dev->ifstats.transmit_packets_dropped );
        seq_printf(m,"\t ifstats.tx_errors                = %u\n",dev->ifstats.tx_errors );
        /* clear Receive counters */
        seq_printf(m," USB-MIB Counters - Rx \n");
        seq_printf(m,"\t ifstats.broadcast_pkts_rxed      = %u\n",dev->ifstats.broadcast_pkts_rxed ) ;
        seq_printf(m,"\t ifstats.broadcast_bytes_rxed     = %u\n",dev->ifstats.broadcast_bytes_rxed ) ;
        seq_printf(m,"\t ifstats.multicast_pkts_rxed      = %u\n",dev->ifstats.multicast_pkts_rxed) ;
        seq_printf(m,"\t ifstats.multicast_bytes_rxed     = %u\n",dev->ifstats.multicast_bytes_rxed) ;
        seq_printf(m,"\t ifstats.unicast_pkts_rxed        = %u\n",dev->ifstats.unicast_pkts_rxed );
        seq_printf(m,"\t ifstats.unicast_bytes_rxed       = %u\n",dev->ifstats.unicast_bytes_rxed );

        seq_printf(m,"\t ifstats.receive_packets_dropped  = %u\n",dev->ifstats.receive_packets_dropped);
        seq_printf(m,"\t ifstats.rx_errors                = %u\n",dev->ifstats.rx_errors) ;
        seq_printf(m,"\t ifstats.unknownProtPkts          = %u\n",dev->ifstats.unknownProtPkts);
    	seq_printf(m," mib2_ifParams \n");
    	seq_printf(m,"\t ifSpeed              = %s\n",(dev->gadget->speed == USB_SPEED_FULL)? "12Mbps"
                :(dev->gadget->speed == USB_SPEED_HIGH)? "480Mbps" : "Unknown");
/*
    	seq_printf(m,"\t ifHighSpeed          = \n");
    	seq_printf(m,"\t ifOperStatus         = %s\n",(p_dev->flags & IFF_UP) ? "Up" : "Down");
    	seq_printf(m,"\t ifPromiscuousMode    = %s\n",(p_dev->flags & IFF_PROMISC) ? "On" : "Off" );
*/
        seq_printf(m," mib2_ifCommand \n");
        seq_printf(m,"\t ifAdminStatus = %d\n", (p_dev->flags & IFF_UP) ? 1 :2);

        return 0 ;
}
static int clear_ethdev_net_stats(struct eth_dev  *dev)
{

    dev->stats.rx_packets = 0;             /* total packets received       */
        dev->stats.tx_packets = 0;             /* total packets transmitted    */
        dev->stats.rx_bytes   = 0;               /* total bytes received         */
        dev->stats.tx_bytes   = 0;               /* total bytes transmitted      */
        dev->stats.rx_errors  = 0;              /* bad packets received         */
        dev->stats.tx_errors  = 0;              /* packet transmit problems     */
        dev->stats.rx_dropped = 0;             /* no space in linux buffers    */
        dev->stats.tx_dropped = 0;             /* no space available in linux  */
        dev->stats.multicast  = 0;              /* multicast packets received   */
        dev->stats.collisions = 0;

        /* detailed rx_errors: */
        dev->stats.rx_length_errors = 0;
        dev->stats.rx_over_errors   = 0;         /* receiver ring buff overflow  */
        dev->stats.rx_crc_errors    = 0;          /* recved pkt with crc error    */
        dev->stats.rx_frame_errors  = 0;        /* recv'd frame alignment error */
        dev->stats.rx_fifo_errors   = 0;         /* recv'r fifo overrun          */
        dev->stats.rx_missed_errors = 0;       /* receiver missed packet       */

        /* detailed tx_errors */
        dev->stats.tx_aborted_errors = 0;
        dev->stats.tx_carrier_errors = 0;
        dev->stats.tx_fifo_errors    = 0;
        dev->stats.tx_heartbeat_errors = 0;
        dev->stats.tx_window_errors = 0;

        /* for cslip etc */
        dev->stats.rx_compressed = 0;
        dev->stats.tx_compressed = 0;
    return 0;
}

/* Write stats */
static int proc_write_usb_stats(struct file *fp, const char __user *buf, size_t count, loff_t *f_pos)
{
    char local_buf[31];
    int ret_val = 0;

   struct net_device       *p_dev;
   struct eth_dev          *dev;

    if (count > 30) {
        printk("Use \"echo 0 > usb_stats\" to reset the statistics\n");
        return -EFAULT;
    }

    copy_from_user(local_buf, buf, count);
    local_buf[count - 1] = '\0';        /* Ignoring last \n char */
    ret_val = count;

        p_dev = (struct net_device *)PDE_DATA(file_inode(fp));
        dev = netdev_priv(p_dev);


    if (strcmp("0", local_buf) == 0) {

        /* clear Transmission counters. */
        dev->ifstats.broadcast_pkts_txed = 0;
        dev->ifstats.broadcast_bytes_txed = 0;
        dev->ifstats.multicast_pkts_txed = 0;
        dev->ifstats.multicast_bytes_txed = 0;
        dev->ifstats.unicast_pkts_txed = 0;
        dev->ifstats.unicast_bytes_txed = 0;
        dev->ifstats.transmit_packets_dropped = 0;
        dev->ifstats.tx_errors = 0;

        /* clear Receive counters */
        dev->ifstats.broadcast_pkts_rxed = 0;
        dev->ifstats.broadcast_bytes_rxed = 0 ;
        dev->ifstats.multicast_pkts_rxed = 0;
        dev->ifstats.multicast_bytes_rxed = 0;
        dev->ifstats.unicast_pkts_rxed = 0;
        dev->ifstats.unicast_bytes_rxed = 0;
        dev->ifstats.receive_packets_dropped = 0;
        dev->ifstats.rx_errors = 0;
    dev->ifstats.unknownProtPkts = 0;

    clear_ethdev_net_stats ( dev );
        printk("Resetting statistics for usb interface\n");

    } else{
        printk("Error: Unknown operation on usb_stats statistics\n");
        printk("Use \"echo 0 > usb_stats\" to reset the statistics\n");
        return -EFAULT;
    }
    return ret_val;
}

static int proc_read_usb_link(struct seq_file *m, void *v)
{
    struct eth_dev          *dev;
    struct net_device       *p_dev;

    p_dev = (struct net_device *)m->private;
    if( !p_dev )
		return -EINVAL;

    dev = netdev_priv(p_dev);

    if( netif_carrier_ok(p_dev) && (p_dev->flags & IFF_UP) ) {
		seq_printf(m, " usb0:Link Status: %s, speed = %s, Mode : %s\n","Up" ,
			(dev->gadget->speed == USB_SPEED_FULL)? "Full" : 
				(dev->gadget->speed == USB_SPEED_HIGH)? "High" : "Unknown",
                cdc_active(dev)?"cdc":rndis_active(dev)?"rndis":"Unknown");
    } 
	else {
		seq_printf(m, " usb0:Link Status: %s\n","Down");
    }

	return 0;
}

static int proc_read_usb_info(struct seq_file *m, void *v)
{
    struct eth_dev          *dev;
    struct net_device       *net;

    net = (struct net_device *)m->private;
    if( !net )
		return -EINVAL;

    dev = netdev_priv(net);

	seq_printf(m, "%s, version: " DRIVER_VERSION "\n", driver_desc);
	seq_printf(m, "using %s, OUT %s IN %s%s%s\n", dev->gadget->name,
			dev->out_ep->name, dev->in_ep->name,
			dev->status_ep ? " STATUS " : "",
			dev->status_ep ? dev->status_ep->name : "");
	seq_printf(m, "MAC %02x:%02x:%02x:%02x:%02x:%02x\n",
			net->dev_addr [0], net->dev_addr [1],
			net->dev_addr [2], net->dev_addr [3],
			net->dev_addr [4], net->dev_addr [5]);
   if (cdc_active(dev) || rndis_active(dev)) {
		seq_printf(m, "HOST MAC %02x:%02x:%02x:%02x:%02x:%02x\n",
			dev->host_mac [0], dev->host_mac [1],
			dev->host_mac [2], dev->host_mac [3],
            dev->host_mac [4], dev->host_mac [5]);
	}
	seq_printf(m, "VendorID   %04x\n", le16_to_cpu(device_desc.idVendor));
	seq_printf(m, "ProductID  %04x\n", le16_to_cpu(device_desc.idProduct));
	seq_printf(m, "Configured as %s mode\n", cdc_active(dev)?"cdc":rndis_active(dev)?"rndis":"Unknown");
	seq_printf(m, "USB CONFIGURATION = %s\n", rndis_default_mode?"A":"B");

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
	if( created_usb_pidtype == 3 ) {
		seq_printf(m, "USB PID_TYPE(%d) = %s\n",created_usb_pidtype, "RNDIS");
	}
	else {
		if( created_usb_pidtype == 4 ) {
			seq_printf(m, "USB PID_TYPE(%d) = %s\n",created_usb_pidtype, "CDC");
		}
	}
#endif
	return 0;
}
#ifdef USB_PERFCNT_SELECT

#define MAX_COUNTER 17
struct cppi_counter {
    char name[40];
    u32 addr;
};
u32 gPktDoneCfgvalue =0, gPktDoneCntValue = 0;
struct cppi_counter cppi_cnt[MAX_COUNTER] = {
    { "usb RxDmaCh4 pkt discard due buf/starv",IO_ADDRESS(0x08690204) },
    { "usb RxDmaCh5 pkt discard due buf/starv",IO_ADDRESS(0x08690208) },
    { "usb RxDmaCh6 pkt discard due buf/starv",IO_ADDRESS(0x0869020c) },
    { "usb RxDmaCh7 pkt discard due buf/starv",IO_ADDRESS(0x08690210) },
    { "queue pending register 0..31",IO_ADDRESS(0x0306a090)           },
    { "queue pending register 32..63",IO_ADDRESS(0x0306a094)           },
    { "queue pending register 64..95",IO_ADDRESS(0x0306a098)           },
    { "queue pending register 96..127",IO_ADDRESS(0x0306a09c)           },
    { "queue pending register 128..159",IO_ADDRESS(0x0306a0a0)           },
    { "queue pending register 160..191",IO_ADDRESS(0x0306a0a4)           },
    { "queue pending register 192..223",IO_ADDRESS(0x0306a0a8)           },
    { "queue pending register 224..255",IO_ADDRESS(0x0306a0ac)           },
    { "queue pending register 256..287",IO_ADDRESS(0x0306a0b0)           },
    { "queue pending register 288..319",IO_ADDRESS(0x0306a0b4)           },
    { "queue pending register 320..351",IO_ADDRESS(0x0306a0b8)           },
    { "queue pending register 352..383",IO_ADDRESS(0x0306a0bc)           },
    { "usb pakcet Done counter ",IO_ADDRESS(0x08690248) }
};

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
extern int apdsp_read_statistics(int index_code, u32 *buf, int bufLen);
extern void Debug_monitor(int debug_queue, int dispBD );
#endif
static int proc_read_usb_cppi(struct seq_file *m, void *v)
{
	struct eth_dev          *dev;
	struct net_device       *p_dev;
    int i;
    int value  = gPktDoneCfgvalue;

    p_dev = (struct net_device *)m->private;

    dev = netdev_priv(p_dev);

              /* clear Transmission counters. */
    for(i=0; i<MAX_COUNTER-1; ++i)
                printk("\t %s [%08x]=%x\n",cppi_cnt[i].name,cppi_cnt[i].addr, *(u32 *)cppi_cnt[i].addr);
    if( value != 0 ){
            printk("\t %s for %sDmaCh%d [%08x]=%u\n",
        cppi_cnt[i].name,(value & 1)?"Rx":"Tx",(value>>1),cppi_cnt[i].addr, *(u32 *)cppi_cnt[i].addr);
            printk("\t %s for %sDmaCh%d after new cfg [%08x]=%u\n",
        cppi_cnt[i].name,(value & 1)?"Rx":"Tx",(value>>1),cppi_cnt[i].addr, *(u32 *)cppi_cnt[i].addr - gPktDoneCntValue);
    }

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
#if 0
    {
#define Cppi41QStatus(qnum)   *(volatile Uint32 *)(0xd3020000+(qnum)*16+0x00)
        printk(" Queue(204):%d\n",Cppi41QStatus(204));
        printk(" Queue(206):%d\n",Cppi41QStatus(206));
        printk(" Queue(145):%d\n",Cppi41QStatus(145));
        printk(" Queue(120):%d\n",Cppi41QStatus(120));
        printk(" Queue(57):%d\n",Cppi41QStatus(59));
    //  Debug_monitor(150,0);
/*      {
        TI_PP_GLOBAL_STATS stats;
        printk(" ret = %d=ti_ppd_get_srl_pkt_stats()\n",ti_ppd_get_srl_pkt_stats(&stats));
        printk("packets_rxed = %d\n",stats.packets_rxed);
        printk("packets_searched = %d\n",stats.packets_searched);
        printk("search_matched = %d\n",stats.search_matched);
        printk("sync_delay = %d\n",stats.sync_delay);
        printk("packets_fwd = %d\n",stats.packets_fwd);
        printk("desc_starved = %d\n",stats.desc_starved);
        printk("ipv4_packets_fwd = %d\n",stats.ipv4_packets_fwd);
        printk("buffer_starved = %d\n",stats.buffer_starved);
        }
*/
    }
#endif
#endif
    return 0;
}
u32 cppi_txrx_done_cfg_adr = IO_ADDRESS(0x08690200);
static int proc_write_usb_cppi(struct file *fp, const char __user *buf, size_t count, loff_t *f_pos)
{
    char local_buf[31];
    int ret_val = 0;

   struct net_device       *p_dev;
   struct eth_dev          *dev;
    u32 value =0;

    if (count > 30) {
        printk("Use \"echo 0 > usb_stats\" to reset the statistics\n");
        return -EFAULT;
    }

    copy_from_user(local_buf, buf, count);
    local_buf[count - 1] = '\0';        /* Ignoring last \n char */
    ret_val = count;

        p_dev = (struct net_device *)PDE_DATA(file_inode(fp));
        dev = netdev_priv(p_dev);

    if (strcmp("0", local_buf) == 0) {
    value = 0;
    *(u32 *)cppi_txrx_done_cfg_adr = 0;
    printk("written %x to Adr %x, readback val = %x",value, cppi_txrx_done_cfg_adr,*(u32 *)cppi_txrx_done_cfg_adr);
    } else
    if (strcmp("1", local_buf) == 0) {
    value = (4 <<1) | 1;
    *(u32 *)cppi_txrx_done_cfg_adr = value ;
    printk("Configuring Packet Done counter for RxDmaCh4(ep1out)\n written %x to Adr %x, readback val = %x",
        value, cppi_txrx_done_cfg_adr,*(u32 *)cppi_txrx_done_cfg_adr);
    } else
    if (strcmp("2", local_buf) == 0) {
    value = (5 <<1) | 1;
    *(u32 *)cppi_txrx_done_cfg_adr = value ;
    printk("Configuring Packet Done counter for RxDmaCh5(ep2out)\n written %x to Adr %x, readback val = %x",
        value, cppi_txrx_done_cfg_adr,*(u32 *)cppi_txrx_done_cfg_adr);
    } else
    if (strcmp("3", local_buf) == 0) {
    value = (6 <<1) | 1;
    *(u32 *)cppi_txrx_done_cfg_adr = value ;
    printk("Configuring Packet Done counter for RxDmaCh6(ep3out)\n written %x to Adr %x, readback val = %x",
        value, cppi_txrx_done_cfg_adr,*(u32 *)cppi_txrx_done_cfg_adr);
    } else
    if (strcmp("4", local_buf) == 0) {
    value = (7 <<1) | 1;
    *(u32 *)cppi_txrx_done_cfg_adr = value ;
    printk("Configuring Packet Done counter for RxDmaCh7(ep4out)\n written %x to Adr %x, readback val = %x",
        value, cppi_txrx_done_cfg_adr,*(u32 *)cppi_txrx_done_cfg_adr);
    } else
   if (strcmp("5", local_buf) == 0) {
        value = (4 <<1);
        *(u32 *)cppi_txrx_done_cfg_adr = value ;
    printk("Configuring Packet Done counter for TxDmaCh4(ep1in)\n written %x to Adr %x, readback val = %x",
        value, cppi_txrx_done_cfg_adr,*(u32 *)cppi_txrx_done_cfg_adr);
    } else
    if (strcmp("6", local_buf) == 0) {
        value = (5 <<1);
        *(u32 *)cppi_txrx_done_cfg_adr = value ;
    printk("Configuring Packet Done counter for TxDmaCh5(ep2in)\n written %x to Adr %x, readback val = %x",
        value, cppi_txrx_done_cfg_adr,*(u32 *)cppi_txrx_done_cfg_adr);
    } else
    if (strcmp("7", local_buf) == 0) {
        value = (6 <<1);
        *(u32 *)cppi_txrx_done_cfg_adr = value ;
    printk("Configuring Packet Done counter for TxDmaCh6(ep3in)\n written %x to Adr %x, readback val = %x",
        value, cppi_txrx_done_cfg_adr,*(u32 *)cppi_txrx_done_cfg_adr);
    } else
    if (strcmp("8", local_buf) == 0) {
        value = (7 <<1);
        *(u32 *)cppi_txrx_done_cfg_adr = value ;
    printk("Configuring Packet Done counter for TxDmaCh7(ep4in)\n written %x to Adr %x, readback val = %x",
        value, cppi_txrx_done_cfg_adr,*(u32 *)cppi_txrx_done_cfg_adr);
    } else{

    printk("Usage: echo 1..4 > /proc/avalanche/usb_cppi for configuring Packdone counter RxDmaCh4..7\n");
        printk("       echo 5..8 > /proc/avalanche/usb_cppi for configuring Packdone counter TxDmaCh4..7\n");
        return -EFAULT;
    }
    gPktDoneCntValue = *(u32 *)IO_ADDRESS(0x08690248);
    gPktDoneCfgvalue = value;
    printk("\n");
    return ret_val;
}
#endif
static int proc_write_usb_config_mode(struct file *fp, const char __user *buf, size_t count, loff_t *f_pos)
{
    char local_buf[31];
    int ret_val = 0;

   struct net_device       *p_dev;
   struct eth_dev          *dev;

    if (count > 30) {
        return -EFAULT;
    }

    copy_from_user(local_buf, buf, count);
    local_buf[count - 1] = '\0';        /* Ignoring last \n char */
    ret_val = count;

        p_dev = (struct net_device *)PDE_DATA(file_inode(fp));
        dev = netdev_priv(p_dev);


    if (strcmp("1", local_buf) == 0) {
                rndis_default_mode = 0; /* configuration mode B */
                printk("USB CONFIGURATION = %s\n", rndis_default_mode?"A":"B");
    } else
    if (strcmp("0", local_buf) == 0) {
                rndis_default_mode = 1; /* configuration mode A */
                printk("USB CONFIGURATION = %s\n", rndis_default_mode?"A":"B");
    } else{
/*
        printk("Error: Unknown operation on usb_configuration\n");
        printk("To select configuration A :\n\techo 0 > /proc/avalanche/usb_config_mode");
        printk("To select configuration B :\n\techo 1 > /proc/avalanche/usb_config_mode");
*/
        return -EFAULT;
    }
    return ret_val;
}
static int proc_read_usb_config_mode(struct seq_file *m, void *v)
{
    struct eth_dev          *dev;
    struct net_device       *p_dev;

    p_dev = (struct net_device *)m->private;

    dev = netdev_priv(p_dev);

    seq_printf(m, "%s\n", rndis_default_mode?"A":"B");

    return 0;
}
#endif
#endif


#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)

static int usb_add_pid (int indx,int pid_type)
{
#define  TI_PP_PID_TYPE_USBRNDIS               3
#define  TI_PP_PID_TYPE_USBCDC                 4

    int retVal;

    /* Add USB PIDs */

    pid_usb[indx].type = pid_type;
    pid_usb[indx].ingress_framing   = TI_PP_PID_INGRESS_ETHERNET
                                            | TI_PP_PID_INGRESS_PPPOE
                                            | TI_PP_PID_INGRESS_IPV6
                                            | TI_PP_PID_INGRESS_IPV4
                                            | TI_PP_PID_INGRESS_IPOE;
    pid_usb[indx].pri_mapping     = 0;   /* Num prio Qs for fwd */
    pid_usb[indx].dflt_pri_drp    = 0;
    pid_usb[indx].dflt_dst_tag    = 0x3FFF;
    pid_usb[indx].dflt_fwd_q      = USB_CPPI4x_USB_TO_HOST_PRXY_QNUM(0);/* Queue 226. Fwd to inf0 by default */
    pid_usb[indx].tx_pri_q_map[0] = USB_CPPI4x_EP0_TX_QNUM(0);  /* Default Q used for egress rec */

    pid_usb[indx].pid_handle      = PP_USB_PID_BASE+indx;
    pid_usb[indx].priv_flags      = 0x0;
    pid_usb[indx].tx_hw_data_len  = 0;
    if( pid_type == TI_PP_PID_TYPE_USBRNDIS  )
    {
        pid_usb[indx].tx_hw_data_len  = 44;
        memset (pid_usb[indx].tx_hw_data, 0, 44);
        *(Uint32*)(pid_usb[indx].tx_hw_data) = cpu_to_le32(0x00000001U);
    }

#ifdef CONFIG_TI_PACKET_PROCESSOR
    retVal = ti_ppm_create_pid (&pid_usb[indx], NULL);
#else
    retVal = ti_ppd_create_pid (&pid_usb[indx]);
#endif
    if( retVal < 0 ){
       printk ("usb_add_pid: failed to add PID(%d), retVal  = %d.\n",
                pid_usb[indx].pid_handle, retVal);
    }
    pid_usb[indx].pid_handle = retVal;

    return pid_usb[indx].pid_handle;
}


/************************************************************************/
/*  INSTALL PID                                                         */
/************************************************************************/
static int install_usb_pid(struct net_device *net, int mode, bool restore_vpids)
{
    int pid_type,i,retVal;
    TI_PP_PID_RANGE  pid_range_usb;
    struct eth_dev          *dev = netdev_priv(net);

    if( mode == 1 )
    {
        pid_type = TI_PP_PID_TYPE_USBRNDIS;
    }
    else
    {
        if( dev->rndis_configured == 1)
            pid_type = TI_PP_PID_TYPE_USBRNDIS;
        else
            pid_type = TI_PP_PID_TYPE_USBCDC;
    }

    /* install usb pid ranges */
    pid_range_usb.type        = pid_type;
    pid_range_usb.port_num    = CPPI41_SRCPORT_USBEP0;
    pid_range_usb.count       = PP_USB_PID_COUNT;
    pid_range_usb.base_index  = PP_USB_PID_BASE;

    /* configure the usb pid range */
#ifdef CONFIG_TI_PACKET_PROCESSOR
    retVal = ti_ppm_config_pid_range (&pid_range_usb);
#else
    retVal = ti_ppd_config_pid_range (&pid_range_usb);
#endif
    if( retVal != 0 )
    {
        printk("%s: config_pid_range failed with error code %d.\n",__FUNCTION__, retVal);
    }

    /* create the pid */
    for(i=0; i<MAX_USB_ENDPOINT; ++i)
    {
#ifdef CONFIG_TI_PACKET_PROCESSOR
        net->pid_handle                    = usb_add_pid (i,pid_type);
        net->vpid_block.type               = TI_PP_ETHERNET;
        net->vpid_block.parent_pid_handle  = net->pid_handle;
        net->vpid_block.egress_mtu         = 0;
        net->vpid_block.priv_tx_data_len   = 0;
#else /* !CONFIG_TI_PACKET_PROCESSOR */
        retVal = usb_add_pid (i,pid_type);
#endif
    }

#ifdef CONFIG_TI_PACKET_PROCESSOR
    if (restore_vpids)
    {
        for(i=0; i<MAX_USB_ENDPOINT; ++i)
        {
            int j;
            for (j=0; j<TI_PP_MAX_VPID;j++)
            {
                if (0xFF != vpid_usb[i][j].vpid_handle)
                {
                    net->vpid_handle = ti_ppm_create_vpid (&net->vpid_block);
                }

            }
        }
    }
#endif

    created_usb_pidtype = pid_type;
    return 0;
}


/************************************************************************/
/*  UN-INSTALL PID                                                      */
/************************************************************************/
static int uninstall_usb_pid(struct net_device *net, int mode, bool save_vpids)
{
   int pid_type,i;
    TI_PP_PID_RANGE  pid_range_usb;
    int retval;
    struct eth_dev          *dev = netdev_priv(net);

    if( mode == 1 )
    {
        pid_type = TI_PP_PID_TYPE_USBRNDIS;
    }
    else
    {
        if( dev->rndis_configured == 1)
            pid_type = TI_PP_PID_TYPE_USBRNDIS;
        else
            pid_type = TI_PP_PID_TYPE_USBCDC;
    }

    /* install usb pid ranges */
    pid_range_usb.type        = pid_type;
    pid_range_usb.port_num    = CPPI41_SRCPORT_USBEP0;
    pid_range_usb.count       = PP_USB_PID_COUNT;
    pid_range_usb.base_index  = PP_USB_PID_BASE;

    /* delete all usb pids configured */
    for(i=0; i<MAX_USB_ENDPOINT; ++i)
    {
#ifdef CONFIG_TI_PACKET_PROCESSOR
        if (save_vpids)
        {
            int j;
            for (j=0; j<TI_PP_MAX_VPID;j++)
            {
                vpid_usb[i][j].vpid_handle = 0xFF;
            }

            ti_ppm_get_vpid  ( pid_usb[i].pid_handle, TI_PP_MAX_VPID, &vpid_usb[i][0] );
        }
        retval= ti_ppm_delete_pid( pid_usb[i].pid_handle );
#else
        retval= ti_ppd_delete_pid( pid_usb[i].pid_handle );
#endif
    }

    /* remove pid range for usb */
#ifdef CONFIG_TI_PACKET_PROCESSOR
    retval = ti_ppm_remove_pid_range(pid_range_usb.port_num);
#else
    retval = ti_ppd_remove_pid_range(pid_range_usb.port_num);
#endif

    /* Temporary Fix USB stability .. This is not the final fix ! */
    net->vpid_handle = -1;

    created_usb_pidtype = 0;
    mdelay(100);
    return 0;
}
#endif
