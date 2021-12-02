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
 *   $Workfile: ether_drv.h $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *   Ethernet driver header file
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

#ifndef _ETHER_DRV_H_
#define _ETHER_DRV_H_

#include "ether_priv.h"


/****************************************************************************
** Constant definitions
****************************************************************************/
#define ETHER_PKT_BUF_SZ            (PKT_BUFFER_SIZE)

/* added 16 byte is for 128-bit alignment of rx buffer */
#define ETHER_RX_PKT_POOL_SZ        (ETHER_PKT_BUF_SZ * ETHER_RX_PKT_BUF_NUM) + 16
#define ETHER_TX_PKT_POOL_SZ        (ETHER_PKT_BUF_SZ * ETHER_TX_PKT_BUF_NUM) + 16

#define ETHER_PKT_MASK              0x55AA1234

#define NUM_TX_DESC                 48                      /* continuous biggest tx packet will not fill 
                                                               whole descriptors at normal operation */
#define NUM_RX_DESC                 64                      /* continuous smallest rx packet will not fill 
                                                               whole descriptors at normal operation */

#define ETHER_TX_PKT_BUF_NUM        NUM_TX_DESC
#define ETHER_RX_PKT_BUF_NUM        NUM_RX_DESC * 3         /* packet buffer count must be greater than twice of
                                                               descriptor count to ensure every buffer hooked on 
                                                               descriptor is flushed and invalidated */    

#define MAX_RECV_PKT_CNT            (UINT32)(NUM_RX_DESC)
#define MAX_TXMIT_PKT_CNT           (UINT32)(NUM_TX_DESC)
#define MAX_RECV_BYTE_CNT           (UINT32)(15000)
#define MAX_TXMIT_BYTE_CNT          (UINT32)(15000)

#define MAX_INT_LOOP_CNT            (UINT32)(2)

/****************************************************************************
** Structure and Type definitions
****************************************************************************/

/****************************************************************************
** Macro
****************************************************************************/
//#define NONCACHE(addr)				(((addr) & 0x0fffffff) | 0x60000000)
//#define NONCACHE(addr)				(((addr) & 0x0fffffff) | 0x40000000)
#define NONCACHE(addr)				(((addr) & 0x0fffffff) | 0x80000000)

#define UTIL_Printf		Printf
/* The macro set rx own bit to indicate descriptor is owned by DMA */
#define INIT_RX_DESC(prCtrl) \
		{\
			prCtrl->Bits.u2BufSize = PKT_BUFFER_SIZE; \
			prCtrl->Bits.fgOwn = OWNED_BY_DMA; \
		}


/* The macro lock semaphore */
#define ETHER_SEMA_LOCK(hHandle)  VERIFY(OSR_OK == x_sema_lock(hHandle, X_SEMA_OPTION_WAIT))

/* The macro unlock semaphore */
#define ETHER_SEMA_UNLOCK(hHandle)  VERIFY(OSR_OK == x_sema_unlock(hHandle))

/****************************************************************************
** Function Declarations
****************************************************************************/
void  vEtherDrvStart(NET_DRV_IF_T *prDev);
void  vEtherDrvStop(NET_DRV_IF_T *prDev);
void  vEtherRecvPacket(NET_DRV_IF_T *prDev);
void  vEtherDeferTask(NET_DRV_IF_T *prDev);
void  vEtherAdapterRemove(PETHER_ADAPTER_T prMac);
void  vEtherModuleInit(NET_DRV_IF_T *prDev);
void vEtherSendComplete(NET_DRV_IF_T *prDev);
void  vEtherDrvSwReset(NET_DRV_IF_T *prDev);
void  vEtherLinkCheck(NET_DRV_IF_T *prDev);
INT32 i4EtherTxmitFrame(PETHER_ADAPTER_T prMac, PKT_BUFF_T *prPkt);
INT32 i4EtherAdapterCreat(NET_DRV_IF_T *prDev);
void vWriteHashTable(void);
void vStopSoftPollingPhy(UINT8 u1Stop);
void vEnableSupportTxCheckSumOffLoad(BOOL fgEnable);
#if 0
BOOL  GET_ETHER_INFO(PETHER_ADAPTER_T prMac, UINT32 Status);
void  SET_ETHER_INFO(PETHER_ADAPTER_T prMac, UINT32 Status);
void  CLR_ETHER_INFO(PETHER_ADAPTER_T prMac, UINT32 Status);
#endif
#endif // _ETHER_DRV_H_
