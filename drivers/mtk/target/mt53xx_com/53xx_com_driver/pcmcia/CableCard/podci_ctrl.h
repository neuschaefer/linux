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
/*-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: podci_ctrl.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

#ifndef PODCI_CTRL_H
#define PODCI_CTRL_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define POD_CLK_SELECT              CLK_APLL1_3 ///< CLK_DMPLL_3
#define POD_CLK_RESOLUTION          20000000    ///< 20MHz
#define DATA_CH_BUF_SIZE            256         ///< Max data channel buffer size
#define EXT_CH_BUF_SIZE             256         ///< Max extended channel buffer size
#define SEMA_TIMEOUT                200

// Memory and I/O timing configuration under 20MHz
#define MEM_TIMING0_VAL1_20M        1   // Start driving OE/WE time
#define MEM_TIMING0_VAL2_20M        2   // Start driving data time
#define MEM_TIMING0_VAL3_20M        4   // Start sampling data time
#define MEM_TIMING0_VAL4_20M        7   // Stop driving OE/WE time
#define MEM_TIMING1_VAL1_20M        8   // Stop driving data time
#define MEM_TIMING1_VAL2_20M        8   // Stop driving CE time
#define IO_TIMING0_VAL1_20M         2   // Start driving IORD/IOWR time
#define IO_TIMING0_VAL2_20M         2   // Start driving data time
#define IO_TIMING0_VAL3_20M         5   // Start sampling data time
#define IO_TIMING0_VAL4_20M         5   // Stop driving IORD/IOWR time
#define IO_TIMING1_VAL1_20M         6   // Stop driving data time
// mntsou #define IO_TIMING1_VAL2_20M         5   // Stop driving CE time
#define IO_TIMING1_VAL2_20M         6   // Stop driving CE time
#define IO_TIMING1_VAL3_20M         1   // Start sampling IOIS16 time
#define IO_TIMING1_VAL4_20M         5   // Start driving CE time

#define WAIT_FOR_MODULE_STATUS_COUNT    100 /* Count for module status register check */


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
// Macro for timing calculation
#define TIMING_CAL(clk, time, val)                                                  \
    {                                                                               \
        if ((((clk) * (time)) % POD_CLK_RESOLUTION) >= (POD_CLK_RESOLUTION / 2))    \
        {                                                                           \
            val = (UINT8)((((clk) * (time)) / POD_CLK_RESOLUTION) + 1);             \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            val = (UINT8)(((clk) * (time)) / POD_CLK_RESOLUTION);                   \
        }                                                                           \
    }


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
EXTERN INT32    PODCI_GetDaFrIntStatus(POD_ACCESS_MODE_T eAccessMode, UINT8* pu1DaFrSts);
EXTERN INT32    PODCI_LowLevelInit(void);
EXTERN void     PODCI_Dump(CHAR* pszTitle, UINT8* pu1Msg, UINT32 u4MsgLen);
EXTERN void     PODCI_Init(void);
EXTERN void     PODCI_SetCISInfo(UINT16 addrcor, UINT8 cor);
EXTERN INT32    PODCI_GetPodCiAccessMode(POD_ACCESS_MODE_T* peAccessMode);
EXTERN INT32    PODCI_SetPodCiAccessMode(POD_ACCESS_MODE_T eAccessMode);
EXTERN void     PODCI_SetBypass(BOOL fgBypass);
EXTERN INT32    PODCI_WaitForModuleStatus(UINT8 u1StatusBit, UINT16 u2TimeOut);
EXTERN INT32    PODCI_GetNegotiatedBufSize(UINT16* pu2BufSize);
EXTERN BOOL     PODCI_NegotiateBufSize(UINT16* pu2Buf);
EXTERN BOOL     PODCI_ResetChannel(void);
EXTERN INT32    PODCI_ReadCardReg(POD_IO_STATE_T eIoState,UINT32 u4RegAddr,UINT8* pu1Data,UINT16 pu2DataLen);
EXTERN INT32    PODCI_WriteCardReg(POD_IO_STATE_T eIoState,UINT32 u4RegAddr,const UINT8* pu1Data,UINT16 u2DataLen);
EXTERN INT32    PODCI_ReadCard(POD_IO_STATE_T eIoState,UINT8* pu1Data,UINT16* pu2DataLen);
EXTERN INT32    PODCI_WriteCard(POD_IO_STATE_T eIoState,const UINT8* pu1Data,UINT16 u2DataLen);
EXTERN void     PODCI_AllocDmaBuf(void);

#endif //PODCI_CTRL_H
