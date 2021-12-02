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

/** @file fcihw_func.h
 *  FCI module function declaration.
 */

#ifndef FCIHW_FUNC_H
#define FCIHW_FUNC_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "fci_type.h"

#include "fcihw_io.h"
#include "x_typedef.h"
#include "fcihw_reg.h"
#include "fci_if.h"

#ifdef CC_BD_FCI
#include "drv_fci.h"
#endif

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
//typedef VOID (*FCI_CALLBACK)(INT32 fgStatus);

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
EXTERN SDC_T *FCI_GetSDCSlot(VOID);
EXTERN MSP_T *FCI_GetMSPSlot(VOID);

#ifdef CC_FCI_SMXD_SUPPORT
EXTERN SMXD_T *FCI_GetSMXDSlot(VOID);
#endif  // #ifdef CC_FCI_SMXD_SUPPORT

/* Export FCI controller functions */
#if 0 // unused
EXTERN BOOL fgFCIHW_ReadBufFull(VOID);
#endif
EXTERN BOOL fgFCIHW_WriteBufEmpty(VOID);
EXTERN BOOL fgFCIHW_ReadBufEmpty(VOID);
EXTERN BOOL fgFCIHW_WriteBufFull(VOID);
EXTERN INT32 FCIHW_Init(VOID);
EXTERN VOID FCIHW_ChangeSDCClock(INT32 i4ClkMHz);
#ifdef CC_FCI_POWER_CONTROL
EXTERN VOID FCIHW_PowerSwitch(BOOL fgPowerOn, UINT32 u4Delayms);
#endif
EXTERN VOID FCIHW_HWConfigSetup(INT32 i4Slot);
EXTERN BOOL FCIHW_PowerOn(void);
EXTERN BOOL FCIHW_PowerOff(void);
EXTERN VOID FCIHW_SDDrivingCurrent(INT32 i4ClkMHz);
EXTERN VOID FCIHW_MSDrivingCurrent(INT32 i4ClkMHz);
EXTERN VOID FCIHW_SetSRAMSEL(UINT32 u4SRAMSel);
#ifdef CC_FCI_SDIO_SUPPORT
EXTERN VOID FCIHW_SetSDIOINTFct(FCI_CALLBACK pfnCallback);
#endif
#ifdef CC_DTV_FCI
EXTERN VOID FCIHW_SetCardDetectFct(FCI_CALLBACK pfnCallback);
#endif
EXTERN INT32 FCIHW_SetIsr(BOOL fgSet);
EXTERN INT32 FCIHW_WaitIntr(UINT32 u4Nms);
EXTERN VOID FCIHW_ResetSlot(INT32 i4Slot);
#if 0 // unused
EXTERN VOID FCIHW_DumpReg(INT8 *szFile, INT32 i4Line);
#endif

EXTERN VOID FCIHW_SetFifoRead(VOID);
EXTERN VOID FCIHW_SetFifoWrite(VOID);
EXTERN INT32 FCIHW_DetectCard(VOID);

/* Export FCI controller DMA functions */
EXTERN VOID FCIHW_DmaWriteDramEnable(UINT8 *pu1Buf, UINT32 u4BlkSz);
EXTERN VOID FCIHW_DmaReadDramEnable(UINT8 *pu1Buf, UINT32 u4BlkSz);
EXTERN VOID FCIHW_DmaDisable(VOID);

#ifdef CC_DTV_FCI
EXTERN INT32 FCIHW_LockSemaphore(VOID);
EXTERN INT32 FCIHW_ReleaseSemaphore(VOID);
#endif

/* Export SDC slot HW functions */
EXTERN UINT32 SDCHW_CmdIdx(VOID);
EXTERN UINT32 SDCHW_ReadResp(INT32 index);
EXTERN VOID SDCHW_OpenWBFInt(VOID);
EXTERN INT32 SDCHW_WaitDataIdle(VOID);
EXTERN VOID SDCHW_OpenDataRdyInt(VOID);
EXTERN INT32 SDCHW_WaitDataRdyOrTo(VOID);
EXTERN INT32 SDCHW_SendCmd(UINT32 u4Cmd, UINT32 u4Arg);

EXTERN VOID SDCHW_InitSemaphore(VOID);
EXTERN VOID SDCHW_Init(VOID);
EXTERN BOOL SDCHW_IsEmpty(VOID);
EXTERN VOID SDCHW_WaitCardNotBusy(VOID);
EXTERN VOID SDCHW_Set4BitOn(BOOL fgSet);
EXTERN INT32 SDCHW_ReadDataReg(UINT32 *pu4Reg, UINT32 u4Size);
EXTERN VOID SDCHW_SetMultiFlag(BOOL fgMulti);
#ifdef CC_FCI_SDIO_SUPPORT
EXTERN VOID SDCHW_SetSDIOInt(BOOL fgEnable);
#endif

/* Export MSP slot HW functions */
EXTERN VOID MSPHW_Init(VOID);
EXTERN VOID FCIHW_ChangeMSPClock(INT32 i4ClkMHz);
EXTERN BOOL MSPHW_IsEmpty(VOID);
EXTERN INT32 MSPHW_SetTpc(const UINT32 *pu4Data, UINT32 u4DataLen, UINT32 u4TpcCode);
EXTERN UINT32 MSPHW_ReadParallelSif(VOID);
EXTERN INT32 MSPHW_WaitStatusBit(UINT8 u1BitChar);
EXTERN VOID MSPHW_ReadRegData(UINT8 *pu1Reg, UINT32 u4RdSz);

/* Export SMXD slot HW functions */
#ifdef CC_FCI_SMXD_SUPPORT
EXTERN BOOL SMXDHW_IsEmpty(VOID);
EXTERN VOID SMXDHW_Init(VOID);
EXTERN INT32 SMXDHW_SendCmd(UINT32 u4Cmd);
#endif  // #ifdef CC_FCI_SMXD_SUPPORT

#endif // FCIHW_FUNC_H
