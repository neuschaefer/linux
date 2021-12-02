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
/********************************************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES 
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED 
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS 
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, 
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK 
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION 
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH 
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, 
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE 
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS 
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.  
 ************************************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: ether_hal.h $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *   Ethernet HAL header file
 *
 * Author:
 * -------
 *   HsHuang
 *
 * Last changed:
 * -------------
 * $Author:
 *
 * $Modtime:
 *
 * $Revision:
****************************************************************************/

#ifndef _ETHER_HAL_H_
#define _ETHER_HAL_H_

/****************************************************************************
** Constant definitions
****************************************************************************/
//#define ETH_RMII_PHY
/* Duplex status */
typedef enum 
{
    HALF_DUPLEX,	
    FULL_DUPLEX, 		
    UNKNOWN		
} ETHER_DUPLEX_MODE_T;

/****************************************************************************
** Macro
****************************************************************************/
/* The macro check whether MAC is RX IP checksum mode  */
#define IS_MAC_IP_CHKSUM()       0

/* The following macros are DMA engine interrupt handling */
#define u4DmaGetInt(u4IntMask)   (dReadMAC(RW_DLY_INT_CFG) & u4IntMask)

#define vDmaIntClear(u4IntClr)   vWriteMAC(RW_DLY_INT_CFG, u4IntClr)

#define vDmaIntEnable(u4IntEn)   vWriteMAC(RW_INT_MASK, (dReadMAC(RW_INT_MASK) & (~u4IntEn)))

#define vDmaIntDisable(u4IntDis) vWriteMAC(RW_INT_MASK, (dReadMAC(RW_INT_MASK) | u4IntDis))

#define vDmaIntClearAll()        vWriteMAC(RW_DLY_INT_CFG, DMA_INT_ENABLE_ALL)

/* The macro get current host transmit descriptor address */
#define u4DmaGetCurrTxDesc()     dReadMAC(RW_TX_DPTR)

/* The macro resume tx descriptor, Poll Demand */
#ifdef CC_MT5368
#define vDmaTxStartAndResetTXDesc()   SetBitMAC(RW_TX_DMA_CTRL, TX_START) 
#define vDmaTxResume()                SetBitMAC(RW_TX_DMA_CTRL, TX_RESUME)   
#else
#define vDmaTxResume()           vWriteMAC(RW_TX_DMA_CTRL, TX_EN)   
#endif

/* The macro resume rx descriptor, Poll Demand */
#ifdef CC_MT5368
#define vDmaRxStartAndResetRXDesc()   SetBitMAC(RW_RX_DMA_CTRL, RX_START) 
#define vDmaRxResume()                SetBitMAC(RW_RX_DMA_CTRL, RX_RESUME)  
#else
#define vDmaRxResume()            vWriteMAC(RW_RX_DMA_CTRL, RX_EN)  
#endif


/* The macro check if current transmission is completed  */
#define fgDescIsTxSent(prTxDesc)  ((prTxDesc->rTxCtrl.Bits.fgOwn == OWNED_BY_CPU) && prTxDesc->rTxCtrl.Bits.u2BufSize)

/* The macro check transmission result is OK or not  */
#define fgDescIsTxValid(prCtrl)  1 

/* The macro check received Rx result is OK or not  */
#define fgDescIsRxValid(prCtrl) (!(prCtrl->Bits.fgOversizeErr) && (!(prCtrl->Bits.fgCrcErr)) &&\
                                    (prCtrl->Bits.fgFirstSeg) && \
                                    (prCtrl->Bits.fgLastSeg))


#define fgIsDMATxEnable()	((dReadMAC(RW_DMA_CFG) & TX_SUSPEND) == 0)
/****************************************************************************
** Structure and Type definitions
****************************************************************************/
typedef UINT32 ETHER_WAKEUP_FRAME_T[8];

/****************************************************************************
** Export API
****************************************************************************/
void vHalSetMacAddr(MAC_ADDRESS_T *pMacAddr);
void vHalGetMacAddr(MAC_ADDRESS_T *pMacAddr);
void vHalAddMulticastAddr(MAC_ADDRESS_T *pMacAddr, UINT32 u4AddrIdx);
void vHalDelMulticastAddr(UINT32 u4AddrIdx);
void vHalGetMulticastAddr(MAC_ADDRESS_T *pMacAddr, UINT32 u4AddrIdx);
void vHalGetLinkSpeed(UINT32 *pu4Speed);
void vHalSetLinkSpeed(UINT32 *pu4Speed);
void vHalGetPromiseMode(UINT32 *pu4Promise);
void vHalSetPromiseMode(UINT32 *pu4Promise);
void vHalGetDuplexMode(UINT32 *pu4Duplex);
void vHalSetDuplexMode(UINT32 *pu4Duplex);
void vHalWakeupFrameFilterSet(ETHER_WAKEUP_FRAME_T *prWkFrame);
void vHalSetMacPwrDown(BOOL fgPwrDown);
INT32 i4HalPhyStatusChk(PETHER_ADAPTER_T prMac);
BOOL fgHalIsPwrDownWakeup(void);
INT32 i4HalSetEtherStatInfo(INT32 i4Cmd, VOID *pvParam);
INT32 i4HalGetEtherStatInfo(INT32 i4Cmd, VOID *pvParam);
BOOL fgHalIsMulticastSet(UINT32 u4AddrIdx);
INT32 i4HalSyncDescIdx(PETHER_ADAPTER_T prMac);
void vHalCalThroughput(PETHER_ADAPTER_T prMac);

void vRMiiInit(void);
void vMacInit(PETHER_ADAPTER_T prMac);
void vMmcInit(void);
void vDmaInit(PETHER_ADAPTER_T prMac);
void vDmaRxStart(void);
void vDmaRxStop(PETHER_ADAPTER_T prMac);
void vDmaTxStart(void);
void vDmaTxStop(PETHER_ADAPTER_T prMac);
void vDmaIntEnableAll(void);
void vDmaIntDisableAll(void);
UINT32 u4DmaGetAndClrInt(UINT32 u4IntMask);


/****************************************************************************
** External ethernet API
****************************************************************************/


#endif // _ETHER_HAL_H_
