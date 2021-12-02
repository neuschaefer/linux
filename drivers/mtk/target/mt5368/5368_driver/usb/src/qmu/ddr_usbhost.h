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
/**
 *	@file	ddr_pcihost.h
 *	@brief 
 *
 *	@author 
 *	@date	08/06/10
 *
 **/

#ifndef	_DDR_USBHOST_H_
#define _DDR_USBHOST_H_

/*==========================================================================
 *
 *      Library Inclusion Segment
 *
 *==========================================================================
 */ 

#if 0
#include "base/cerr.h"
#include "base/ddriver.h"
#include "conf_usbhost.h"
#include "link_usb.h" 
#include "musbdefs.h"
#include "mu_dev.h"
#include <stdio.h>
#include <string.h>
#endif

#include "dt_debug.h"

/*==========================================================================
 *
 *      Constant and Macro Definition Segment
 *
 *==========================================================================
 */

#define USB_HW_IRQ_NO 13
#define QMU_HW_IRQ_NO 10

#define USB_BRIDGE_BASE 0x86000000

#define WDMA_MUSB_FADDR   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_FADDR))
#define WDMA_MUSB_POWER   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_POWER)) 
#define WDMA_MUSB_INTRTXE   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_INTRTXE))
#define WDMA_MUSB_INTRRXE   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_INTRRXE))
#define WDMA_MUSB_INTRUSBE   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_INTRUSBE))
#define WDMA_MUSB_INDEX   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_INDEX))
#define WDMA_MUSB_TESTMODE   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_TESTMODE))
#define WDMA_MUSB_DEVCTL   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_DEVCTL))
#define WDMA_MUSB_CSR0   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_CSR0))
#define WDMA_MUSB_NAKLIMIT0   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_NAKLIMIT0))
#define WDMA_MUSB_TXMAXP   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_TXMAXP))
#define WDMA_MUSB_TXCSR   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_TXCSR))
#define WDMA_MUSB_RXMAXP   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_RXMAXP))
#define WDMA_MUSB_RXCSR   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_RXCSR))
#define WDMA_MUSB_TXTYPE   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_TXTYPE))
#define WDMA_MUSB_TXINTERVAL   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_TXINTERVAL))
#define WDMA_MUSB_RXTYPE   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_RXTYPE))
#define WDMA_MUSB_RXINTERVAL   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_RXINTERVAL))
#define WDMA_MUSB_LINKINFO   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_LINKINFO))
#define WDMA_MUSB_VPLEN   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_VPLEN))
#define WDMA_MUSB_HSEOF1   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_HSEOF1))
#define WDMA_MUSB_FSEOF1   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_FSEOF1))
#define WDMA_MUSB_LSEOF1   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_LSEOF1))
#define WDMA_MUSB_RQPKTCOUNT_1   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_RQPKTCOUNT_1))
#define WDMA_MUSB_RQPKTCOUNT_2   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_RQPKTCOUNT_2))
#define WDMA_MUSB_RQPKTCOUNT_3   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_RQPKTCOUNT_3))
#define WDMA_MUSB_RQPKTCOUNT_4   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_RQPKTCOUNT_4))
#define WDMA_MUSB_RQPKTCOUNT_5   ((volatile unsigned char*)(USB_BRIDGE_BASE+MGC_O_HDRC_RQPKTCOUNT_5))

// USB PHY register
#define MUSB_PHY_BASE                       0x86000500
#define MUSB_PHY_USBUTMI                 ((volatile unsigned int*)(MUSB_PHY_BASE+0x00))
#define MUSB_PHY_PHYDCTRL0             ((volatile unsigned int*)(MUSB_PHY_BASE+0x04))
#define MUSB_PHY_PHYDCTRL1             ((volatile unsigned int*)(MUSB_PHY_BASE+0x08))
#define MUSB_PHY_PHYACTL0             ((volatile unsigned int*)(MUSB_PHY_BASE+0x0C))
#define MUSB_PHY_PHYACTL1             ((volatile unsigned int*)(MUSB_PHY_BASE+0x10))
#define MUSB_PHY_PHYACTL3             ((volatile unsigned int*)(MUSB_PHY_BASE+0x14))
#define MUSB_PHY_PHYACTL4             ((volatile unsigned int*)(MUSB_PHY_BASE+0x18))
#define MUSB_PHY_PHYACTL5             ((volatile unsigned int*)(MUSB_PHY_BASE+0x1C))
#define MUSB_PHY_PHYACTL15           ((volatile unsigned int*)(MUSB_PHY_BASE+0x20))
#define MUSB_PHY_PHYACTL16           ((volatile unsigned int*)(MUSB_PHY_BASE+0x24))
#define MUSB_PHY_PHYACTL17           ((volatile unsigned int*)(MUSB_PHY_BASE+0x28))
#define MUSB_PHY_PHYACTL18           ((volatile unsigned int*)(MUSB_PHY_BASE+0x2C))
#define MUSB_PHY_PHYACTL19           ((volatile unsigned int*)(MUSB_PHY_BASE+0x30))
#define MUSB_PHY_PHYACTL20           ((volatile unsigned int*)(MUSB_PHY_BASE+0x34))
#define MUSB_PHY_PHYACTL21           ((volatile unsigned int*)(MUSB_PHY_BASE+0x38))
#define MUSB_PHY_PHYACTL22           ((volatile unsigned int*)(MUSB_PHY_BASE+0x3C))
#define MUSB_PHY_PHYACTL23           ((volatile unsigned int*)(MUSB_PHY_BASE+0x40))
#define MUSB_PHY_PHYACTL24           ((volatile unsigned int*)(MUSB_PHY_BASE+0x44))
#define MUSB_PHY_PHYACTL25           ((volatile unsigned int*)(MUSB_PHY_BASE+0x48))
#define MUSB_PHY_PHYACTL26           ((volatile unsigned int*)(MUSB_PHY_BASE+0x4C))
#define MUSB_PHY_PHYACTL27           ((volatile unsigned int*)(MUSB_PHY_BASE+0x50))
#define MUSB_PHY_PHYACTL28           ((volatile unsigned int*)(MUSB_PHY_BASE+0x54))
#define MUSB_PHY_PHYACTL29           ((volatile unsigned int*)(MUSB_PHY_BASE+0x58))
#define MUSB_PHY_MACEXTRA              ((volatile unsigned int*)(MUSB_PHY_BASE+0x5C))

typedef enum usbhost_ioctl_t
{
    USBHOST_IOCTL_GETREADY,
    USBHOST_IOCTL_GETHDRC
} usbhost_ioctl;

#if 0 //!PORTING
ddriver_t* USBHost_GetInstance();
#endif
void QMU_HdrcUnloadFifo (unsigned char bEnd, unsigned int dwCount, unsigned char* pDest);
void QMU_HdrcLoadFifo (unsigned char bEnd, unsigned int dwCount, const unsigned char* pSource);

/*==========================================================================
 *
 *      External Variable Segment
 *
 *==========================================================================
 */

#if 0 //!PORTING
extern void MGC_HdrcStartTx (hdrc_t* pCtrl, unsigned char bEnd);
extern void HdrcServiceETHEnd (hdrc_t* pCtrl);
extern void HdrcServiceMSEnd (hdrc_t* pCtrl);
extern void HdrcService7108End (hdrc_t* pCtrl);
extern void HdrcServiceDefaultEnd (hdrc_t* pCtrl);
extern void MGC_HdrcServiceTxEnd (hdrc_t* pCtrl, unsigned char bEnd);
extern void MGC_HdrcServiceRxEnd (hdrc_t* pCtrl, unsigned char bEnd);
extern unsigned char MGC_SetPortReset (hdrc_t* pCtrl, unsigned char bIsIPReset);
#endif
//extern int EP0ControlRequest (hdrc_t *pCtrl, MUSB_DeviceRequest *pDevReq);
extern unsigned char MGC_DmaChannelStatusChanged (void* pPrivateData, unsigned char bLocalEnd, unsigned char bTransmit);
extern void GPIO_WriteIO(unsigned int data, unsigned int pin);

#endif
