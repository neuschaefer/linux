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

/** @file msp_func.h
 *  This header file declares the MS basic hardware protocol functions.
 */

#ifndef MSP_FUNC_H
#define MSP_FUNC_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "fci_type.h"

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_typedef.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------
/* CLI easy command interface based on fci_msp.c physical access functions */
EXTERN INT32 _MSP_Detect(VOID);
EXTERN INT32 _MSP_SetRWReg(VOID);
EXTERN INT32 _MSP_ReadReg(VOID);
EXTERN INT32 _MSP_WriteReg(VOID);
EXTERN INT32 _MSP_MSSetCmd(const CHAR *szStr);
EXTERN INT32 _MSP_MSPSetCmd(const CHAR *szStr);
EXTERN INT32 _MSP_GetInt(VOID);
EXTERN INT32 _MSP_ReadPage(UINT32 u4Block, UINT32 u4PgNo, UINT32 *pu4Result, UINT32 u4BufPtr);
EXTERN INT32 _MSP_WritePage(UINT32 u4Block, UINT32 u4PgNo, UINT32 u4Extra, UINT32 u4BufPtr);
EXTERN INT32 _MSP_MsproReadAttribute(UINT8 *pu1Buf);
EXTERN INT32 _MSP_ReadBlock(UINT32 u4Block, UINT32 u4ExtraMemBuf, UINT32 u4DataMemBuf);
EXTERN INT32 _MSP_WriteBlock(UINT32 u4Block, UINT32 u4ExtraMemBuf, UINT32 u4DataMemBuf);
EXTERN INT32 _MSP_EraseBlock(UINT32 u4Block);
/* Debug functions. */
EXTERN INT32 _MSP_BootWrite(VOID);
EXTERN INT32 _MSP_SetParallel(VOID);
EXTERN INT32 _MSP_SetSerial(VOID);
EXTERN VOID _MSP_ChangeClock(INT32 i4Clock);

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
/* MSP Slot interface */
EXTERN INT32 MSP_Init(MSP_T *prMSP);
EXTERN INT32 MSP_Stop(VOID);

/* MS command interface for L2P module. */
EXTERN INT32 MSP_MsReadSinglePage(UINT32 u4BlkNo, UINT32 u4PgNo, UINT8 *pu1Buf, UINT8 *pu1Extra,
                                  UINT32 u4Mode);
EXTERN INT32 MSP_MsWriteSinglePage(UINT32 u4BlkNo, UINT32 u4PgNo, UINT8 *pu1Buf,
                                   const UINT8 *pu1Extra, UINT32 u4Mode);
EXTERN INT32 MSP_MsEraseBlock(UINT32 u4BlkNo);
EXTERN INT32 MSP_MsReadBlock(UINT32 u4BlkNo, UINT32 u4PgNo, UINT32 u4PgNum, UINT8 *pu1Buf,
                             UINT8 *pu1Extra);
EXTERN INT32 MSP_MsWriteBlock(UINT32 u4BlkNo, UINT32 u4PgNo, UINT32 u4PgNum, UINT8 *pu1Buf,
                              const UINT8 *pu1Extra);

/* MS command interface for ms_if.c based on ms_l2p.c logic access functions */
EXTERN INT32 MSP_MsL2PRead(UINT32 u4Lba, UINT8 *pu1MemPtr, UINT32 u4SecLen);
EXTERN INT32 MSP_MsL2PWrite(UINT32 u4Lba, const UINT8 *pu1MemPtr, UINT32 u4SecLen);

/* MSPro command interface for ms_if.c */
EXTERN INT32 MSP_MsproReadBlks(UINT32 u4Lba, UINT8 *pu1Buf, UINT32 u4BlkNs);
EXTERN INT32 MSP_MsproWriteBlks(UINT32 u4Lba, UINT8 *pu1Buf, UINT32 u4BlkNs);
EXTERN INT32 MSP_ErrorHandling(INT32  i4Retry);
EXTERN INT32 MSP_ResetClockSetting(VOID);
EXTERN UINT32 MSP_SetHighClock(UINT32 u4MspHighClock);
EXTERN UINT32 MSP_SetNormClock(UINT32 u4MspNormClock);

EXTERN INT32 MSP_BootWrite(VOID);

#endif // MSP_FUNC_H
