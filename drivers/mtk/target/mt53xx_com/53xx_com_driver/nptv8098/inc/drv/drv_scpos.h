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

/*************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: drv_scpos.h $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *
 *
 * Author:
 * -------
 *
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Modtime: 04/05/28 9:33a $
 *
 * $Revision: #1 $
*************************************************************************/

#ifndef _DRV_SCPOS_H_
#define _DRV_SCPOS_H_

#include "hw_scpos.h"
#include "hw_ospe.h"
#include "debug.h"
#include "feature.h"

#ifdef CC_COPLAT_MT82
EXTERN BOOL _fgPIP,_fgLRPOP;
#endif

#define fgMain444Mode() ((RegReadFld(SCPIP_DS1_02, DS1_02_SEL444_1) > 0)? TRUE: FALSE)
#define fgPIP444Mode() ((RegReadFld(SCPIP_DS2_02, DS2_02_SEL444_2) > 0)? TRUE: FALSE)
#define vDrvMainMute(bOnOff) (vRegWriteFldAlign(MUTE_00, bOnOff, R_M_MUTE_FRONT_EN))
#define vDrvPIPMute(bOnOff) (vRegWriteFldAlign(MUTE_00, bOnOff, R_P_MUTE_FRONT_EN))
#define fgMainMuteOn() (RegReadFld(MUTE_00, R_M_MUTE_FRONT_EN))
#define fgPIPMuteOn() (RegReadFld(MUTE_00, R_P_MUTE_FRONT_EN))

typedef enum
{
    QUICK_BLANK_NO_FUNCTION = 0,
    QUICK_BLANK_BY_MAIN = 2,
    QUICK_BLANK_BY_PIP = 3,
}E_QUICK_BLANK_MODE;

//----------------------------------------------------------------------------------
// public functions
//----------------------------------------------------------------------------------
// drv_scpos.c
EXTERN void vScpipHwInit(UINT8 bPath);

#if SUPPORT_HDTV_HARDWARE_MUTE
EXTERN void vScpipConfigHardwareMute(void);
EXTERN void vOSTGVideoPlaneHWMute(UINT8 bPath,UINT8 mode);
#endif

//EXTERN void vScpipSetMuteMask(UINT8 bPath, UINT8 bOnOff, UINT32 u4Mask);
//EXTERN void vScpipMuteMaskOnOff(UINT8 bPath, UINT32 u4OnOff);

#if CC_SCPOS_MUTE_RELATCH_WA
EXTERN UINT32 u4ScpipGetMuteMask(UINT8 bMode);
EXTERN UINT32 u4ScpipGetHwMuteStatus(UINT8 bPath);
EXTERN void vScpipSetHwMuteStatus(UINT8 bPath, UINT8 u1OnOff);
#endif

EXTERN void vScpipOutRGBColorOnOff(UINT8 bOnOff);
EXTERN void vScpipOutRGBColor(UINT8 RValue,UINT8 GValue,UINT8 BValue);

EXTERN UINT32 u4ScpipDirectSetDRAM(UINT32 u4VdpId, UINT32 u4X, UINT32 u4Y, UINT32 u4Width, UINT32 u4Height, UINT32 u4GBRYCbCr);

EXTERN void vDrvSetOSTGInPattern(UINT32 u4VdpId, UINT32 u4OnOff);
EXTERN void vDrvSetOSTGOutPattern(UINT32 u4OnOff);
EXTERN void vDrvSetOSTGOutPatternChg(UINT32 u4OnOff,UINT32 u4PatType,UINT32 u4PatSpd);

//-----------------------------------------------------------------------------
/** Brief of u4ScpipGetMinSrcRegionWidth.
 * Get Scalar minimun source region width.
 * @param u4VdpId video plane ID 0~1
 * @retval Frame Delay
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 u4ScpipGetMinSrcRegionWidth(UINT8 u4VdpId);

#if defined(CC_MT5396)
EXTERN UINT32 u4ScpipGetMinSrcRegionHeight(UINT8 u4VdpId);
#endif

/* @6896 remove*/
#if 0
EXTERN void vScpipSetDramBurst(UINT8 bPath, UINT8 bOnOff);
EXTERN UINT8 u1ScpipGetDramBurst(UINT8 bPath);
#endif

EXTERN void vScpipSetMirror(UINT8 bPath, UINT8 bOnOff);
EXTERN void vScpipSetFlip(UINT8 bPath, UINT8 bOnOff);
EXTERN void vScpipSetMirrorFlipAll(UINT8 bOnOff);
EXTERN UINT8 u1ScpipGetTv3dOutIndicator(void);

// added by peggy 20071004
#ifdef SUPPORT_DRAM_DUMP
EXTERN UINT8 bInitScalerDump(void);
EXTERN UINT8 bGetScalerDRAM(UINT32 u2X, UINT32 u2Y, UINT32 *pu2VR, UINT32 *pu2UB, UINT32 *pu2YG);
EXTERN UINT8 bGetScalerDRAMAverage(UINT32 u4X, UINT32 u4Y, UINT32 u4Width, UINT32 u4Height, UINT32 *pu4VR, UINT32 *pu4UB, UINT32 *pu4YG);
EXTERN UINT8 bReleaseScalerDump(void);

EXTERN UINT8 bInitLastStageDump(void);
EXTERN UINT8 bGetLastStageDRAM(UINT32 u2X, UINT32 u2Y, UINT32 *pu2R, UINT32 *pu2B, UINT32 *pu2G);
EXTERN UINT8 bReleaseLastStageDump(void);
#endif

typedef struct
{
    UINT32 dwAddr;
    UINT32 dwSize;
    UINT32 dwWidth;    
    UINT32 dwHeight;        
    UINT32 dwWINC;    
    UINT16 u2W_POS;        
    UINT16 u2R_POS;        
    UINT32 dwMHL;
    UINT32 dwMVL;
    UINT8 bIs444;
    UINT8 bIs10bit;
}tagDramDumpInfo_T;

EXTERN void vGetScalerDRAMInfo(tagDramDumpInfo_T *rDramDumpInfo);
EXTERN UINT32 u4ScpipCheckFifoOverflow(UINT32 u4VdpId);
EXTERN void vScpipResetFifoOverflow(UINT32 u4VdpId);
EXTERN UINT32 u4ScpipGetDSInputHTotal(const UINT32 u4VdpId);
EXTERN UINT32 u4ScpipGetDSInputVTotal(const UINT32 u4VdpId);
EXTERN UINT32 u4ScpipGetDSInputHActive(const UINT32 u4VdpId);
EXTERN UINT32 u4ScpipGetDSInputVActive(const UINT32 u4VdpId);

//----------------------------------------------------------------------------------
// inter-module functions
//----------------------------------------------------------------------------------
EXTERN void vOSTGSetBorderOnOff(UINT32 u4VdpId, UINT32 u4OnOff);
EXTERN void vOSTGSetBorderColor(UINT32 u4VdpId, UINT32 u4RGB);
EXTERN void vOSTGSetBorderParam(UINT32 u4VdpId, UINT32 u4X, UINT32 u4Y, UINT32 u4Width, UINT32 u4Height, UINT32 u4Thickness);

#endif
