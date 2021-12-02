/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/

#ifndef __WLAN_KAL_H
#define __WLAN_KAL_H

#ifdef CC_BD_FCI
#include "x_debug.h"
#endif

/************************************************************************
*                     C O M P I L E R   F L A G S
*************************************************************************
*/
#define __KAL_PACK_BEGIN__      /* _Pragma("pack(1)") */
#define __KAL_PACK_END__        /* _Pragma("pack()") */
#define __KAL_ATTRIB_PACKED__   __attribute__ ((__packed__))
#define __KAL_INLINE__          static __inline__

#define IN
#define OUT

/************************************************************************
*                E X T E R N A L   R E F E R E N C E S
*************************************************************************
*/
extern int g_malloc_flag;

/*=======================================================*/
/* System Includes */

// Max Liao, wlan card porting, 2006/03/02.
/*
#include <linux/config.h>
#include <linux/version.h>
#include <linux/init.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/rtnetlink.h>
#include <linux/timer.h>
#include <linux/list.h>
#include <linux/if_arp.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/vmalloc.h>
#include <linux/proc_fs.h>
#include <asm/io.h>
#include <asm/byteorder.h>
#include <asm/uaccess.h>
#include <asm/semaphore.h>

#include <linux/ioport.h>
*/

/************************************************************************
*                          C O N S T A N T S
*************************************************************************
*/

#define MAX_RX_PKT_LEN          1600 /* including encryption needed */
                                     /* the Ethernet backbone is considered */
// Jiun add (0423)
#define MAX_MGT_PKT_LEN     1024

#ifndef IFNAMSIZ
#define IFNAMSIZ                16
#endif

/* HZ must be larger than 2 (500ms) */
#ifndef HZ
#define HZ                      100  /* 10ms, basic time unit */
#endif

/* kmalloc allocation flag. Please note this flag is used in both
 * interrupt routine and general routine. In Linux, use GFP_ATOMIC.
 */
// Max Liao, wlan card porting, 2006/03/02.
#define KAL_MALLOC_FLAG     NULL

#ifdef CONFIG_ARCH_S3C2440A
#include <asm/arch/irq.h> // set_external_irq
#define WLAN_STA_IRQ    IRQ_EINT8 // steal IRQ_CF_RDY
    #ifdef WLAN_HIF_HPI
#define MEM_MAPPED_ADDR   0x28000000UL /* Memory bank5 base address before remap */
    #endif

    #ifdef WLAN_HIF_SDIO
#include "mmcsd.h"
    #endif
#endif

#ifdef CONFIG_M5249
#define HOSEA2      1
#define MEM_MAPPED_ADDR   0xE0400000UL /* System Space Starting Address  */
#define MT5901IRQ 164 // GPI4 H -> L
#endif // CONFIG_M5249

// Max Liao, wlan card porting, 2006/03/02.
#define printk	        UTIL_Printf
#define memset      x_memset
#define memcpy      x_memcpy
/************************************************************************
*                         D A T A   T Y P E S
*************************************************************************
*/
//typedef struct pci_dev          kal_pci_dev_t;
typedef struct net_device       kal_net_device_t;
typedef struct net_device_stats kal_net_device_stats_t;
typedef struct sk_buff          kal_skb_t;
typedef struct dma_addr_t              kal_dma_addr_t;

/************************************************************************
*                        P U B L I C   D A T A
*************************************************************************
*/
#define kal_jiffies     jiffies

/************************************************************************
*                             M A C R O S
*************************************************************************
*/
#define kal_MOD_INC_USE_COUNT           MOD_INC_USE_COUNT
#define kal_MOD_DEC_USE_COUNT           MOD_DEC_USE_COUNT
#define kal_netif_start_queue(dev)      netif_start_queue(dev)
#define kal_netif_stop_queue(dev)       netif_stop_queue(dev)
#define kal_netif_wake_queue(dev)       netif_wake_queue(dev)
#define kal_netif_queue_stopped(dev)    netif_queue_stopped(dev)
#define kal_netif_running(dev)          netif_running(dev)
#define kal_tasklet_schedule(t)         tasklet_schedule(t)

#define kal_netif_start_queue_all(priv_p) { \
            kal_netif_start_queue(priv_p->netdev); \
    }

#define kal_netif_stop_queue_all(priv_p) do { \
            if (priv_p->netdev) { \
                    kal_netif_stop_queue(priv_p->netdev); \
            } \
    } while (0)

#define kal_netif_wake_queue_all(priv_p) do { \
            if (priv_p->netdev) { \
                    kal_netif_wake_queue(priv_p->netdev); \
            } \
    } while (0)

#define kal_vmalloc(sz)         vmalloc(sz)
#define kal_vfree(a)            vfree(a)

#define kal_netif_tx_confirm(skb)
#define kal_netif_rx(skb)       netif_rx(skb)

/* Endian conversion of MAC Hardware intergers to/from HOST */
#define chip2host_16(n)         __le16_to_cpu(n)
#define chip2host_32(n)         __le32_to_cpu(n)
#define host2chip_16(n)         __cpu_to_le16(n)
#define host2chip_32(n)         __cpu_to_le32(n)

/* Endian conversion of network fields to/from HOST */
#define kal_ntohs(n)            ntohs(n)
#define kal_htons(n)            htons(n)


/* Pointer to private data in kal_net_device_t */
#define kal_dev_priv(dev)               (dev)->priv
#define kal_set_dev_macAddr(dev,mac)    memcpy((dev)->dev_addr,mac,6)

#define kal_request_mem_region(a,b,c)		      request_mem_region(a,b,c)
#define kal_release_mem_region(a,b)		      release_mem_region(a,b)

#define kal_malloc(typ,flg)     (typ*)kmalloc(sizeof(typ),flg)

// Max Liao, wlan card porting, 2006/03/02.
#define kal_malloc_sz(sz,flg)   x_mem_alloc(sz)
#define kal_free(m)                 x_mem_free(m)

#define kal_alloc_skb(sz)       dev_alloc_skb(sz)
#define kal_free_skb(skb)       dev_kfree_skb(skb)

/* Functions acting on socket buffers */
#define kal_skb_put(skb,len)            skb_put(skb,len)
#define kal_skb_push(skb,len)           skb_push(skb,len)
#define kal_skb_pull(skb,len)           skb_pull(skb,len)
#define kal_skb_reserve(skb,len)        skb_reserve(skb,len)
#define kal_skb_data(skb)               (skb)->data
#define kal_skb_len(skb)                (skb)->len

/* Timer related functions */
// Max Liao, wlan card porting, 2006/03/02.
#define kal_udelay(t)
#define kal_mdelay(t)                   mdelay(t)

#define INIT_KDBG_VARS(...)
#define KDBG_ENTER  do{}while(0)
#define KDBG_EXIT do{}while(0)
#define KDBG_LOG_INIT(...)
#define KDBG_LOG_RX(...)
#define KDBG_LOG_MGT(...)
#define DbgPrintZo(...)
#define DbgPrint UTIL_Printf

#define CON_MSG(fmt, args...) do{ \
    UTIL_Printf("<1>[MT%x] ", 0x5912); \
    UTIL_Printf(fmt, ##args); \
    x_thread_delay(10); \
} while(0)

#define HEX_DUMP_BODY(s,p,n) do { \
    int __i__; \
    UTIL_Printf("%s:: " #s ": length %d", __FUNCTION__, (int)n); \
    for( __i__=0; __i__ < (n); __i__++) { \
        if ((__i__%8) == 0) DbgPrint("\n%s:: (%08lx) ", __FUNCTION__, (unsigned long)((UINT_8*)(p)+__i__));\
        UTIL_Printf(" %02x", ((UINT_8*)(p))[__i__]); \
    } \
    UTIL_Printf("\n=======================\n"); \
} while(0)

/************************************************************************
*              F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*           I N L I N E   F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/

#endif  /* __WLAN_KAL_H */


