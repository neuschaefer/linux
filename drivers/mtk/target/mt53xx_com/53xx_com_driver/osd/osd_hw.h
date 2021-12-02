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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: osd_hw.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_hw.h
 *  This header file declares hardware register interface of OSD.
 */

#ifndef OSD_HW_H
#define OSD_HW_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_io.h"
#include "x_hal_5381.h"
#include "x_hal_926.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

EXTERN void _OSD_AlwaysUpdateReg(BOOL fgEnable);
EXTERN void _OSD_UpdateReg(void);


//-----------------------------------------------------------------------------
// base register relative functions
//-----------------------------------------------------------------------------
EXTERN INT32 _OSD_BASE_GetReg(UINT32 *pOsdBaseReg);
EXTERN INT32 _OSD_BASE_SetReg(const UINT32 *pOsdBaseReg);
EXTERN INT32 _OSD_BASE_UpdateHwReg(void);


EXTERN INT32 _OSD_BASE_SetUpdate(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetAlwaysUpdate(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetResetFsm(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetResetAll(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetHsEdge(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetVsEdge(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetFldPol(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetPrgs(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetChecksumEn(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetAutoPos(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetYuvOut(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetOSD12Ex(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetVsyncSrcSel(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetVsWidth(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetOht(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetOvt(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetScrnHStartOsd2(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetScrnHStartOsd1(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetScrnVStartBot(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetScrnVStartTop(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetScrnVSize(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetScrnHSize(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetOsd1VStart(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetOsd1HStart(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetOsd2VStart(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetOsd2HStart(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetHsWidth(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetDelayAdj(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetOsd12Ex(UINT32 u4Value);
#ifdef CC_MT5363
EXTERN INT32 _OSD_BASE_SetScrnHStartCsr(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetScrnHStartOsd3(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetOsd3VStart(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetOsd3HStart(UINT32 u4Value);
#endif
EXTERN INT32 _OSD_BASE_SetScmemCfg(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetPreMulTarget(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetPreMulVStart(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetPreMulHStart(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetPreMulVEnd(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetPreMulHEnd(UINT32 u4Value);


EXTERN INT32 _OSD_BASE_GetUpdate(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetAlwaysUpdate(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetResetFsm(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetResetAll(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetHsEdge(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetVsEdge(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetFldPol(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetPrgs(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetChecksumEn(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetAutoPos(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetYuvOut(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetOSD12Ex(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetVsyncSrcSel(UINT32* pu4Value);
EXTERN INT32 _OSD_BASE_GetVsWidth(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetOht(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetOvt(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetScrnHStartOsd2(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetScrnHStartOsd1(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetScrnVStartBot(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetScrnVStartTop(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetScrnVSize(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetScrnHSize(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetOsd1VStart(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetOsd1HStart(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetOsd2VStart(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetOsd2HStart(UINT32 *pu4Value);
#ifdef CC_MT5363
EXTERN INT32 _OSD_BASE_GetOsd3VStart(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetOsd3HStart(UINT32 *pu4Value);
#endif

EXTERN INT32 _OSD_BASE_GetHsWidth(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetDelayAdj(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetOsd12Ex(UINT32 *pu4Value);
#ifdef CC_MT5363
EXTERN INT32 _OSD_BASE_GetScrnHStartCsr(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetScrnHStartOsd3(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetOsd3VStart(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetOsd3HStart(UINT32 *pu4Value);
#endif
EXTERN INT32 _OSD_BASE_GetScmemCfg(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetPreMulTarget(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetPreMulVStart(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetPreMulHStart(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetPreMulVEnd(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetPreMulHEnd(UINT32 *pu4Value);


//-----------------------------------------------------------------------------
// plane register relative functions
//-----------------------------------------------------------------------------
// plane regitser get and set function
EXTERN INT32 _OSD_PLA_GetReg(UINT32 u4Plane, UINT32 *pOsdPlaneReg);
EXTERN INT32 _OSD_PLA_SetReg(UINT32 u4Plane, const UINT32 *pOsdPlaneReg);
EXTERN INT32 _OSD_PLA_UpdateHwReg(UINT32 u4Plane);

// plane register set function
EXTERN INT32 _OSD_PLA_SetEnable(UINT32 u4Plane, UINT32 u4Value);
EXTERN INT32 _OSD_PLA_SetFakeHdr(UINT32 u4Plane, UINT32 u4Value);
EXTERN INT32 _OSD_PLA_SetPrngEn(UINT32 u4Plane, UINT32 u4Value);
//EXTERN INT32 _OSD_PLA_SetDecEn(UINT32 u4Plane, UINT32 u4Value);
//EXTERN INT32 _OSD_PLA_SetAlphaMod(UINT32 u4Plane, UINT32 u4Value);
EXTERN INT32 _OSD_PLA_SetHFilter(UINT32 u4Plane, UINT32 u4Value);
//EXTERN INT32 _OSD_PLA_SetVFilter(UINT32 u4Plane, UINT32 u4Value);
EXTERN INT32 _OSD_PLA_SetBlending(UINT32 u4Plane, UINT32 u4Value);
EXTERN INT32 _OSD_PLA_SetHeaderAddr(UINT32 u4Plane, UINT32 u4Value);
EXTERN INT32 _OSD_PLA_SetFading(UINT32 u4Plane, UINT32 u4Value);
EXTERN INT32 _OSD_PLA_SetContReqLmt(UINT32 u4Plane, UINT32 u4Value);
EXTERN INT32 _OSD_PLA_SetFifoSize(UINT32 u4Plane, UINT32 u4Value);
EXTERN INT32 _OSD_PLA_SetVacTh16(UINT32 u4Plane, UINT32 u4Value);
EXTERN INT32 _OSD_PLA_SetVacTh4(UINT32 u4Plane, UINT32 u4Value);


// plane register get function
EXTERN INT32 _OSD_PLA_GetEnable(UINT32 u4Plane, UINT32 *pu4Value);
EXTERN INT32 _OSD_PLA_GetFakeHdr(UINT32 u4Plane, UINT32 *pu4Value);
EXTERN INT32 _OSD_PLA_GetPrngEn(UINT32 u4Plane, UINT32 *pu4Value);
//EXTERN INT32 _OSD_PLA_GetDecEn(UINT32 u4Plane, UINT32 *pu4Value);
//EXTERN INT32 _OSD_PLA_GetAlphaMod(UINT32 u4Plane, UINT32 *pu4Value);
EXTERN INT32 _OSD_PLA_GetHFilter(UINT32 u4Plane, UINT32 *pu4Value);
EXTERN INT32 _OSD_PLA_GetVFilter(UINT32 u4Plane, UINT32 *pu4Value);
EXTERN INT32 _OSD_PLA_GetBlending(UINT32 u4Plane, UINT32 *pu4Value);
EXTERN INT32 _OSD_PLA_GetHeaderAddr(UINT32 u4Plane, UINT32 *pu4Value);
EXTERN INT32 _OSD_PLA_GetFading(UINT32 u4Plane, UINT32 *pu4Value);
EXTERN INT32 _OSD_PLA_GetContReqLmt(UINT32 u4Plane, UINT32 *pu4Value);
EXTERN INT32 _OSD_PLA_GetFifoSize(UINT32 u4Plane, UINT32 *pu4Value);
EXTERN INT32 _OSD_PLA_GetVacTh16(UINT32 u4Plane, UINT32 *pu4Value);
EXTERN INT32 _OSD_PLA_GetVacTh4(UINT32 u4Plane, UINT32 *pu4Value);

//-----------------------------------------------------------------------------
// scaler register relative functions
//-----------------------------------------------------------------------------
// scaler regitser get and set function
EXTERN INT32 _OSD_SC_GetReg(UINT32 u4Scaler, UINT32 *pOsdScalerReg);
EXTERN INT32 _OSD_SC_SetReg(UINT32 u4Scaler, const UINT32 *pOsdScalerReg);
EXTERN INT32 _OSD_SC_UpdateHwReg(UINT32 u4Scaler);

// scaler register set function
EXTERN INT32 _OSD_SC_SetVuscEn(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetVdscEn(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetHuscEn(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetHdscEn(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetScLpfEn(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetScEn(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetSrcVSize(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetSrcHSize(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetDstVSize(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetDstHSize(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetVscHSize(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetHdscStep(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetHdscOfst(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetHuscStep(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetHuscOfst(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetVscOfstTop(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetVscOfstBot(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetVscStep(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetScLpfC1(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetScLpfC2(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_SetScLpfC3(UINT32 u4Scaler, UINT32 u4Value);

// scaler register get function
EXTERN INT32 _OSD_SC_GetVuscEn(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetVdscEn(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetHuscEn(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetHdscEn(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetScLpfEn(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetScEn(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetSrcVSize(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetSrcHSize(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetDstVSize(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetDstHSize(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetVscHSize(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetHdscStep(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetHdscOfst(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetHuscStep(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetHuscOfst(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetVscOfstTop(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetVscOfstBot(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetVscStep(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetScLpfC1(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetScLpfC2(UINT32 u4Scaler, UINT32 *pu4Value);
EXTERN INT32 _OSD_SC_GetScLpfC3(UINT32 u4Scaler, UINT32 *pu4Value);


//-----------------------------------------------------------------------------
// cursor register relative functions
//-----------------------------------------------------------------------------
// cursor
EXTERN INT32 _OSD_CSR_GetReg(UINT32 *pOsdCursorReg);
EXTERN INT32 _OSD_CSR_SetReg(const UINT32 *pOsdCursorReg);
EXTERN INT32 _OSD_CSR_UpdateHwReg(void);

EXTERN INT32 _OSD_CSR_SetVStart(UINT32 u4Value);
EXTERN INT32 _OSD_CSR_SetHStart(UINT32 u4Value);
EXTERN INT32 _OSD_CSR_SetV2x(UINT32 u4Value);
EXTERN INT32 _OSD_CSR_SetH2x(UINT32 u4Value);
EXTERN INT32 _OSD_CSR_SetV4Bit(UINT32 u4Value);
EXTERN INT32 _OSD_CSR_SetH4Bit(UINT32 u4Value);
EXTERN INT32 _OSD_CSR_SetEn(UINT32 u4Value);
EXTERN void _OSD_CSR_SetColor(UINT32 u4Index, UINT32 u4Value);

EXTERN INT32 _OSD_CSR_GetVStart(UINT32 *pu4Value);
EXTERN INT32 _OSD_CSR_GetHStart(UINT32 *pu4Value);
EXTERN INT32 _OSD_CSR_GetV2x(UINT32 *pu4Value);
EXTERN INT32 _OSD_CSR_GetH2x(UINT32 *pu4Value);
EXTERN INT32 _OSD_CSR_GetEn(UINT32 *pu4Value);
EXTERN INT32 _OSD_CSR_GetH4Bit(UINT32 *pu4Value);
EXTERN INT32 _OSD_CSR_GetV4Bit(UINT32 *pu4Value);
EXTERN UINT32 _OSD_CSR_GetColor(UINT32 u4Index);


//-----------------------------------------------------------------------------
// region register relative functions
//-----------------------------------------------------------------------------
EXTERN void _OSD_RGN_InitApi(void);

// region register set function
EXTERN INT32 _OSD_RGN_SetNextRegion(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetFifoEx(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetNextEnable(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetColorMode(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetDataAddr(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetAlpha(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetHClip(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetVClip(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetLineSize9(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetVbSel(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetUgSel(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetYrSel(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetASel(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetPaletteAddr(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetPaletteLen(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetLoadPalette(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetInputWidth(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetInputHeight(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetLineSize(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetHStep(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetVStep(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetOutputHeight(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetOutputPosY(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetOutputWidth(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetOutputPosX(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetColorKeyEnable(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetColorKey(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetFrameMode(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetAutoMode(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetTopField(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetBlendMode(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetHMirror(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetVFlip(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetDecMode(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetDecLine(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetDecEn(UINT32 u4Region, UINT32 u4Value);
// 539x
EXTERN INT32 _OSD_RGN_SetHpxExt(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetVIntpMode(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetEndian(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetHIntMode(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetVStepOffset(UINT32 u4Region, UINT32 u4Value);
EXTERN INT32 _OSD_RGN_SetHStepOffset(UINT32 u4Region, UINT32 u4Value);

// region register get function
EXTERN INT32 _OSD_RGN_GetNextRegion(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetFifoEx(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetNextEnable(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetColorMode(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetDataAddr(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetAlpha(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetHClip(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetVClip(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetLineSize9(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetVbSel(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetUgSel(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetYrSel(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetASel(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetPaletteAddr(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetPaletteLen(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetLoadPalette(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetInputWidth(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetInputHeight(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetLineSize(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetHStep(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetVStep(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetOutputHeight(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetOutputPosY(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetOutputWidth(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetOutputPosX(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetColorKeyEnable(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetColorKey(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetFrameMode(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetAutoMode(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetTopField(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetBlendMode(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetHMirror(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetVFlip(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetDecMode(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetDecLine(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetDecEn(UINT32 u4Region, UINT32 *pu4Value);
// 539x
EXTERN INT32 _OSD_RGN_GetHpxExt(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetVIntpMode(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetEndian(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetHIntMode(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetVStepOffset(UINT32 u4Region, UINT32 *pu4Value);
EXTERN INT32 _OSD_RGN_GetHStepOffset(UINT32 u4Region, UINT32 *pu4Value);


// utility function for region list maintain
EXTERN INT32 _OSD_RGN_GetHandle(UINT32 u4Addr, UINT32 *pu4OsdRegion);
EXTERN INT32 _OSD_RGN_GetAddress(UINT32 u4Region , UINT32 *pu4Addr);
EXTERN INT32 _OSD_RGN_AttachTail(UINT32 u4List, INT32 i4Attachment);
EXTERN INT32 _OSD_RGN_SetAlloc(UINT32 u4Region, UINT32 fgStatus);
EXTERN INT32 _OSD_RGN_GetAlloc(UINT32 u4Region, UINT32 *pfgStatus);
EXTERN INT32 _OSD_RGN_Alloc(UINT32 *pu4OsdRegion);
EXTERN INT32 _OSD_RGN_Free(UINT32 u4Region);
EXTERN INT32 _OSD_RGN_FreeList(UINT32 u4List);

#endif //OSD_HW_H

