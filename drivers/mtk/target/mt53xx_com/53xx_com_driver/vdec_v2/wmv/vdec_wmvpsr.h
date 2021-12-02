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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_wmvpsr.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_wmvpsr.h
 *  This header file declares exported APIs of wmv.
 */

#ifndef VDEC_WMVPSR_H
#define VDEC_WMVPSR_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
#include "vdec_wmvutil.h"
LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
void vVldByteAlign(UCHAR ucEsId);
BOOL fgVldInitBarrelShifter(UCHAR ucEsId,UINT32 dwPtr);
void PostAdjustReconRange(UCHAR ucEsId);
void vInitVParser(UCHAR ucEsId);
void vInitVParserWMV(UCHAR ucEsId);
void before_bp(UCHAR ucEsId);
void after_bp(UCHAR ucEsId);

UINT32 dwVParserWMVA(UCHAR ucEsId);
UINT32 decodeSequenceHead_Advanced(UCHAR ucEsId);
UINT32 DecodeEntryPointHeader(UCHAR ucEsId);
UINT32 decodeVOPHeadProgressiveWMVA(UCHAR ucEsId);
UINT32 decodeVOPHeadInterlaceV2(UCHAR ucEsId);
UINT32 decodeVOPHeadFieldPicture(UCHAR ucEsId);
UINT32 decodeFieldHeadFieldPicture(UCHAR ucEsId);
void SetupFieldPictureMVScaling(UCHAR ucEsId,UINT32 iRefFrameDistance);
void SetupForwardBFieldPictureMVScaling(UCHAR ucEsId,UINT32 iRefFrameDistance);
void SetupBackwardBFieldPictureMVScaling(UCHAR ucEsId,UINT32 iRefFrameDistance);
void decodePFieldMode(UCHAR ucEsId);
void decodeBFieldMode(UCHAR ucEsId);
int iDecodeBitPlane(UCHAR ucEsId,int iBpNum);
void vDecBPNorm2Diff2(UCHAR ucEsId);
BOOL fgDecodeNorm6Diff6(UCHAR ucEsId);
BOOL fgDecNorm6Diff6Tile(UCHAR ucEsId, INT32 *a2On4Off, INT32 *aCount, INT32 *pi6);
void vDecodeVOPDQuant(UCHAR ucEsId);
void vSetDefaultDQuantSetting(UCHAR ucEsId);
UINT32 iGetPMvMode(UCHAR ucEsId,UINT32 iPQuant, BOOL fgRepeat);
UINT32 iGetIFMvMode(UCHAR ucEsId,UINT32 iPQuant, BOOL fgRepeat);
void ComputeDQuantDecParam(UCHAR ucEsId);
void UpdateDCStepSize(UCHAR ucEsId,UINT32 bStepSize);
UINT32 getbits(UCHAR ucEsId,UINT32 dwBits);
UINT32 get16(UCHAR ucEsId,UINT32 dwBits);
UINT32 dwGetHi(UCHAR ucEsId,UINT32 dwN);
//Simple & Main()
UINT32 decodeVOLHead_WMV3(UCHAR ucEsId);
UINT32 decodeVOLHead_WMV12(UCHAR ucEsId);
UINT32 WMVideoDecDecodeFrameHead(UCHAR ucEsId);
UINT32 WMV78DecodePicture(UCHAR ucEsId);
UINT32 WMVideoDecDecodeClipInfo(UCHAR ucEsId);
void decodeVOPHead_WMV2(UCHAR ucEsId);
void DecodeSkipBit(UCHAR ucEsId);
void AdjustReconRange(UCHAR ucEsId);
void PostAdjustReconRange(UCHAR ucEsId);
BOOL fgWaitVldFetchOk(UCHAR ucEsId);
UINT32 dwGetBitStream(UCHAR,UINT8);
void SetupMultiResParams(UCHAR ucEsId);

#endif //VDEC_wmvPSR_H

