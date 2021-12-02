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
#ifndef _DT_DEBUG_H
#define _DT_DEBUG_H
#define PORTING 1
#define USB_RISC_CACHE_ENABLED  1

#if !define(CC_MT5389)
#define CC_MT5389
#endif

#include "mu_arch.h"

//#include <linux/kernel.h>
//#include <linux/list.h>
//#include <linux/slab.h>
//#include <linux/interrupt.h>
//#include <linux/random.h>
//#include <linux/delay.h>

#include "musbdefs.h"
#include "utest_uniusb.h"
#include "utest_transfer.h"
#include "dvt_typedefs.h"
#include "ddr_usbhost.h"
#include "mu_dev.h"

#include "Ddr_qmu.h"

#define GPD_LIST_LEN 10000

extern void tx_handler(int ep_num);
extern void rx_handler(int ep_num);
extern void ep0_handler(void);
#if 0 // mhzhang
struct USB_REQ *usb_get_req(int ep_num, USB_DIR dir);
struct USB_EP_SETTING *usb_get_ep_setting(int ep_num, USB_DIR dir);
#else
extern USB_REQ *usb_get_req(int ep_num, USB_DIR dir);
extern USB_EP_SETTING *usb_get_ep_setting(int ep_num, USB_DIR dir);
#endif

extern void proc_epx_tx(int ep_num);
extern void proc_epx_rx(int ep_num, unsigned int dwCount);
#if 0 // mhzhang
extern struct USB_DMA_REQ *usb_get_dma_req(int channel);
#else
extern USB_DMA_REQ *usb_get_dma_req(int channel);
#endif

extern void resetFIFO(void);
extern void MGC_ResetPort(void);
extern void usb_start_QMU(int Q_num,  USB_DIR dir, unsigned char isMT);
extern void randomTxBuf(int ep_num);
extern unsigned int getFIFOsz(unsigned int FIFOlen, int isDPB);
extern void usb_ep_enable(int ep_num, USB_DIR dir, USB_TRANSFER_TYPE type, int maxp, int dpb, int dev_ep_num);
extern void usb_config_req(int ep_num, USB_DIR dir, int count, char isDma, int channel, int burstmode, int dma_mode, int dma_reqmode);
extern unsigned char checkComplete(int ep_num, USB_DIR dir);
extern void clearRxBuf(int ep_num);
extern void prepareTxBuf(int ep_num);
extern void checkRxBuf(int ep_num);
extern int USB_get_random(void);
extern void usb_start_transfer(int ep_num, USB_DIR dir);
extern void usb_config_ep0_req(int address, USB_EP0_DATASTAGE data_stage, int count, MUSB_DeviceRequest *pDevReq);
extern void usb_start_ep0_transfer(void);
extern unsigned char usb_check_ep0_complete(void);

#if 0 // mhzhang
extern struct USB_EP0_REQ *usb_get_ep0_req(void);
#else
extern USB_EP0_REQ *usb_get_ep0_req(void);
#endif

extern void usb_start_transfer_QMU(int ep_num, int target_ep, USB_DIR dir);
extern void usb_insert_transfer_QMU(int ep_num, int target_ep, USB_DIR dir, unsigned char* buf, unsigned int count, unsigned char isHWO);
extern void usb_start_QMU(int Q_num,  USB_DIR dir, unsigned char isMT);
extern void usb_stop_QMU(int Q_num,  USB_DIR dir);
extern void usb_resume_QMU(int Q_num,  USB_DIR dir);
extern void Flush_QMU(int Q_num,  USB_DIR dir);
extern void usb_restart_QMU(int Q_num,  USB_DIR dir);

//ben
extern void HalFlushInvalidateDCache(void);
extern void vBdpDramBusy(unsigned char fgBool);
//~ben

extern void minusGlobalFIFOadd(unsigned int num);
#endif
