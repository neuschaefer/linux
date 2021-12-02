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
 * $RCSfile: vdp_hal.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_hal.h
 *  Brief of file vdp_hal.h.
 *  Details of file vdp_hal.h (optional).
 */

#ifndef VDP_HAL_H
#define VDP_HAL_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

#define VDP_PD_VIDEO_MODE                   0
#define VDP_PD_32_MODE                      1
#define VDP_PD_22_MODE                      2
#define VDP_PD_STOP_MODE                    3

#define VDP_FRC_NORMAL                      0
#define VDP_FRC_NONE                        1   // no FRC
#define VDP_FRC_STOP_AUTO                   2   // stop auto at last field
#define VDP_FRC_TRICK_PLAY                  3   // Trick Play at 20081001
#define VDP_FRC_CANCEL                      4   // vdp cancel for 4 frame buffer at 20090630

#define VDP_ISR_VEI                         0x00000080
#define VDP_ISR_ADI                         0x00000100
#define VDP_ISR_FI                          0x00000200

#define VDP_CLK_148_5_MHZ                   1
#define VDP_CLK_148_3515_MHZ                2   // 148.5/1.001

#ifdef __MODEL_slt__
#define VDP_B2R_PSCAN_CRC_FLOW
#endif

/** Brief of VDP_RACING_MODE_ENUM_T
 */
typedef enum
{
    VDP_RACING_MODE_ONOFF,
    VDP_RACING_MODE_B,
    VDP_RACING_MODE_MBL,
    VDP_RACING_MODE_PIC_STRUCTURE,
    VDP_RACING_MODE_VEI,
    VDP_RACING_MODE_COMPACT,
    VDP_RACING_MODE_NS
} VDP_RACING_MODE_ENUM_T;

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

EXTERN UCHAR _B2R_TO_VDP(UCHAR ucB2RId);
EXTERN void _VDP_HalInit(void);
EXTERN void _VDP_HalIsrFin(UCHAR ucVdpId);
EXTERN void _VDP_HalReset(void);
EXTERN void _VDP_HalSetEnable(UCHAR ucVdpId, UCHAR ucEnable);
EXTERN void _VDP_HalSetQVEnable(UCHAR ucVdpId, UCHAR ucEnable);
EXTERN void _VDP_HalSetDeintEnable(UCHAR ucVdpId, UCHAR ucEnable);
EXTERN void _VDP_HalSetColorMode(UCHAR ucVdpId, UCHAR ucColorMode);
EXTERN void _VDP_HalSetChromaMode(UCHAR ucVdpId, UCHAR ucFrameEncode, UCHAR ucTff);
EXTERN void _VDP_HalSetSrcSize(UCHAR ucVdpId, UINT32 u4Width, UINT32 u4Height, UINT32 u4Pitch);
EXTERN void _VDP_HalSetPswWidth(UCHAR ucVdpId, UINT32 u4PswEn, UINT32 u4Width, UINT32 u4Pitch);
EXTERN void _VDP_HalSetOutClock(UCHAR ucVdpId, UINT32 u4Clock);
EXTERN void _VDP_HalSetOutSize(UCHAR ucVdpId, UINT32 u4Width, UINT32 u4Height, UINT32 u4Interlace, UINT32 u4DoubleClk, UINT32 u4ClkMode);
EXTERN void _VDP_HalSetBg(UCHAR ucVdpId, UINT32 u4BgColor);
EXTERN void _VDP_HalForceBg(UCHAR ucVdpId, UCHAR ucForce);
EXTERN void _VDP_HalSetAddr(UCHAR ucVdpId, UINT32 u4AddrY, UINT32 u4AddrC);
EXTERN void _VDP_HalSetRViewAddr(UCHAR ucVdpId, UINT32 u4AddrY, UINT32 u4AddrC);
EXTERN void _VDP_HalSetQVdoLT(UCHAR ucVdpId, UINT32 u4AddrY, UINT32 u4AddrC);
EXTERN void _VDP_HalSetQVdoRT(UCHAR ucVdpId, UINT32 u4AddrY, UINT32 u4AddrC);
EXTERN void _VDP_HalSetQVdoLB(UCHAR ucVdpId, UINT32 u4AddrY, UINT32 u4AddrC);
EXTERN void _VDP_HalSetQVdoRB(UCHAR ucVdpId, UINT32 u4AddrY, UINT32 u4AddrC);
EXTERN void _VDP_HalSetRacingMode(UCHAR ucVdpId, VDP_RACING_MODE_ENUM_T eMode, UINT32 u4Arg);
EXTERN void _VDP_HalConfig(UCHAR ucVdpId);
EXTERN void _VDP_HalSetTopFieldFirst(UCHAR ucVdpId, UCHAR ucTff);
EXTERN void _VDP_HalSetFrc(UCHAR ucVdpId, UCHAR ucTargetNs, UCHAR ucChangeFieldNs, UCHAR ucMode);
EXTERN void _VDP_HalSetFormat(UCHAR ucVdpId, UINT32 u4Mode422, UINT32 u4ModeLinear);
EXTERN UINT32 _VDP_HalInsideBlankRegion(UCHAR ucVdpId);
EXTERN void _VDP_HalB2rSwitch(UCHAR ucVdpId, UCHAR ucB2rId);
EXTERN void _VDP_HalStcSpeed(UINT8 u1B2rId,UINT32 u4Speed, BOOL fgDiv10);
EXTERN void _VDP_HalStcPeriod(UINT8 u1B2rId,UINT32 u4VSyncPeriod);
EXTERN void _VDP_HalUpdateStc(UINT8 u1B2rId,BOOL fgEnable);
EXTERN BOOL _VDP_HalGetUpdateStc(UINT8 u1B2rId);
EXTERN void _VDP_HalSetB2rStc(UINT8 u1B2rId,UINT32 u4B2rStc);
EXTERN UINT32 _VDP_HalGetB2rStc(UINT8 u1B2rId);

EXTERN void VDP_HalSetTrickMode(UINT8 u1B2rId,UCHAR ucTrick);
EXTERN UCHAR VDP_HalGetTrickMode(UINT8 u1B2rId);
EXTERN void VDP_HalPowerOffB2R(void);
EXTERN void VDP_HalTestB2RSdPath(void);
EXTERN void VDP_HalSetStepForward(UINT8 u1B2rId,UCHAR ucSet);
EXTERN UCHAR VDP_HalGetStepForward(UINT8 u1B2rId);
EXTERN void VDP_HalSetSlowForward(UINT8 u1B2rId,UCHAR ucSet);
EXTERN UCHAR VDP_HalGetSlowForward(UINT8 u1B2rId);
EXTERN UINT8 VDP_HalGetUnderFlowFlag(UCHAR ucVdpId);
EXTERN VOID VDP_HalResetUnderFlowFlag(UCHAR ucVdpId);
EXTERN VOID VDP_HalSetRepeatForever(UCHAR ucVdpId,BOOL fgRepeatForever,BOOL fgTopField);
EXTERN VOID VDP_HalSetB2RMirrorFlip(UCHAR ucVdpId);
EXTERN UINT32 _B2rHdGetCrc(void);
EXTERN VOID _VDP_HalSetTrackingMode(UCHAR ucVdpId,BOOL fgTrackingMode);
#ifdef CC_B2R_SKYPE_SUPPORT
EXTERN void _VDP_HalStatusTest(UCHAR ucVdpId);
EXTERN void _VDP_HalDisablePattern (UCHAR ucVdpId);
EXTERN UINT32 VDP_HalGetB2RFifoStatus(UCHAR ucVdpId);
EXTERN void VDP_HalResetB2RFifoStatus(UCHAR ucVdpId);
EXTERN void VDP_HalGetYCStart(UCHAR ucVdpId,UINT32 *pu4YStart, UINT32 *pu4CStart, UINT32 *pu4LineSize, UINT32 *pu4VActive);
EXTERN VOID VDP_HalSetB2RPower(UCHAR ucVdpId,BOOL fgPowerOn);
EXTERN VOID VDP_HalSet3DMode(UCHAR ucVdpId,UINT8 u1B2R3DMode);
EXTERN VOID VDP_HalSetLRState(UCHAR ucVdpId,BOOL fgLView);
EXTERN VOID VDP_HalSetTBState(UCHAR ucVdpId,BOOL fgTopField);
EXTERN void VDP_DumpB2RRegister(UCHAR ucVdpId);
#else
EXTERN void _VDP_HalStatusTest(void);
EXTERN void _VDP_HalDisablePattern (void);
EXTERN UINT32 VDP_HalGetB2RFifoStatus(void);
EXTERN void VDP_HalResetB2RFifoStatus(void);
EXTERN void VDP_HalGetYCStart(UINT32 *pu4YStart, UINT32 *pu4CStart, UINT32 *pu4LineSize, UINT32 *pu4VActive);
EXTERN VOID VDP_HalSetB2RPower(BOOL fgPowerOn);
EXTERN VOID VDP_HalSet3DMode(UINT8 u1B2R3DMode);
EXTERN VOID VDP_HalSetLRState(BOOL fgLView);
EXTERN VOID VDP_HalSetTBState(BOOL fgTopField);
EXTERN void VDP_DumpB2RRegister(void);
#endif
EXTERN VOID VDP_EnableDumpRegister(BOOL fgOn);
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


#endif    // VDP_HAL_H

