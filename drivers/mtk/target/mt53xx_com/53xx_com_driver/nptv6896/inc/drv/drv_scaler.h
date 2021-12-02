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
 * $RCSfile: drv_scaler.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_scaler.h
 *  Brief of file drv_scaler.h.
 *  Details of file drv_scaler.h (optional).
 */

#ifndef _DRV_SCALER_H_
#define _DRV_SCALER_H_


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "vdp_drvif.h"
#include "debug.h"
#include "feature.h"
#include "video_def.h"
#include "drv_di.h"
#include "hw_scpos.h"

//-----------------------------------------------------------------------------
// Constant define
//-----------------------------------------------------------------------------
#define CAL_HDIRECT_FACTOR               (0)
#define CAL_VDIRECT_FACTOR               (1)
#define VDP_MDDI_RESCALE_TIME               (4)
#define VDP_SCPOS_RESCALE_TIME              (7)
#define VDP_SCPOS_IMPORT_PROTECT            (3)
#define VDP_SCPOS_WEN_WAIT                  (4)
#define VDP_SCPOS_TV3D_MUTE_DELAY           (6)
#define VDP_SCPOS_DELAY_UNMUTE_TIME        (10)    
#define VDP_DYNAMICE_SCALE_IDLE             (0)
#define VDP_DYNAMICE_SCALE_SCALING          (1)

#define SCPOS_BYPASS_PSCANNR    0xffffffff

#define DOWN_SCALING_MODE 0
#define UP_SCALING_MODE 1

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

#define CC_SPEED_UP_DISPMODE_TUNE 1  
#define SCPIP_TURNOFF_IMPORT_AT_MODCHG    //set import_off & set_data_sel when mode changed
#define SCPIP_TOGGLE_IMPORT_AT_INPUTV

// TODO: remove
//#define MT5360B_WA4
#ifdef MT5360B_WA4
#define MT5360B_WA4_DLY1 4 // decoder + 4 pixels
#endif

#if defined(CC_MT5396)
#define SCPIP_LI_TO_SBS_VSYNC_OPT_WA
#endif

// TODO: used in other files, move?
// vsync delay constant
#define VDP_VD_DISABLE              0
#define VDP_VD_TURN_DISABLE         1
#define VDP_VD_ENABLE               2
#define VDP_VD_TURN_ENABLE          3
#define VDP_VD_DELAY_COUNT          7

#define SUB_ACTIVE_ADJ_E4      0x146 //for ECO(E4) later
#define SUB_READ_DELAY_E4      0x128 //for ECO(E4) later

#if defined(CC_MT5396)
#define HSYNC_ADJ           0x9c
#define MAIN_ACTIVE_ADJ     0x9c
#define MAIN_READ_DELAY     0x0
#define SUB_ACTIVE_ADJ      0x143
#define SUB_READ_DELAY      0x125
#elif defined(CC_MT5368)
#define HSYNC_ADJ           0x9c
#define MAIN_ACTIVE_ADJ     0x9c
#define MAIN_READ_DELAY     0x0
#define SUB_ACTIVE_ADJ      0x32c
#define SUB_READ_DELAY      0x30e
#else    //for 5389
#define HSYNC_ADJ           0x32
#define MAIN_ACTIVE_ADJ     0x32
#define MAIN_READ_DELAY     0x0
#define SUB_ACTIVE_ADJ      0x1D6
#define SUB_READ_DELAY      0x1B8    
#endif

#define VDP_DS_PTN_OFF 0
#define VDP_DS_PTN_AUTO 1
#define VDP_DS_PTN_MANUAL 2

#define VDP_US_PTN_OFF 0
#define VDP_US_PTN_AUTO 1
#define VDP_US_PTN_MANUAL 2

#define SCPIP_121MAP_H_POS_LEFT 0
#define SCPIP_121MAP_H_POS_MID 1
#define SCPIP_121MAP_H_POS_RIGHT 2

#define getMixedHPorch(bPath) ((getPicInfo(bPath)->wXOffset + getPicInfo(bPath)->wUIHPorch >1000)? (getPicInfo(bPath)->wXOffset + getPicInfo(bPath)->wUIHPorch - 1000): 0)
#define getMixedVPorch(bPath) ((getPicInfo(bPath)->wYOffset + getPicInfo(bPath)->wUIVPorch >1000)? (getPicInfo(bPath)->wYOffset + getPicInfo(bPath)->wUIVPorch - 1000): 0)
#define vDrvMainUpdateVBound(u2Vline) (vRegWriteFldAlign(SCPIP_US1_14, u2Vline, US1_14_UP_VBOUND_Y1))

typedef struct
{
    UINT16 wX;
    UINT16 wY;
    UINT16 wWidth;
    UINT16 wHeight;
}PSCAN_CLIP_INFO_T;

typedef struct
{
    UINT16 wWidth;          //width after up-scaling
    UINT16 wHeight;         //height after up-scaling
    UINT16 wLeft;           //left overscan
    UINT16 wRight;          //right overscan
    UINT16 wTop;            //top overscan
    UINT16 wBottom;         //bottom overscan
    UINT16 wClipX;          //cliping on right for MJC
    UINT16 wClipY;          //cliping on bottom for MJC
    UINT16 wTargetWidth;    //real target width
    UINT16 wTargetHeight;   //read target height
}MJC_CLIP_INFO_T;
    
//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
#ifdef DYNAMIC_RES_VERIFY
void vSetSCDynRes(UINT8 fgOnOff);
void vForcePscanOverscan(UINT8 fgOnOff);
#endif

//*****************************************************************************
//  drv_scaler.c
//*****************************************************************************
EXTERN void vScpipWriteCtrlOnOff(UCHAR ucVdpId, UCHAR ucOnOff);
EXTERN UINT8 u1ScpipGetDispMode(UINT8 bPath);
EXTERN void vScpipSetGameMode(UCHAR ucVdpId, UCHAR ucOnOff);
EXTERN void vScpipSet121Map(UCHAR ucVdpId, UCHAR ucOnOff);
EXTERN UINT32 vScpipGet121Map(UCHAR ucVdpId);
EXTERN void vScpipSet121MapHPos(UCHAR ucVdpId, UINT16 u2HPos);
EXTERN void vDrvVideoFixedPreScaler(UCHAR ucVdpId, UCHAR ucOnOff, UCHAR uc1stConfig);
EXTERN void _VDP_Set10bitMode(UINT32 u4VdpId, UINT32 u4Mode);
EXTERN void vScpipSetDynamicScaler(UCHAR ucVdpId, UCHAR ucOnOff, UINT32 u4ResponseTime);
EXTERN void _VDP_DispModeEnable(UCHAR ucVdpId, UCHAR ucMode);
EXTERN void vScpipDelayEnable(UCHAR ucVdpId, UCHAR ucEnable, UINT32 u4DelayCount);
EXTERN UINT32 u4ScpipGetHorizontalPreScaleDownFactor(UCHAR ucVdpId);
EXTERN void vScpipSetForcedHpsFactor(UINT32 u4VdpId,UINT32 u4Factor);
EXTERN void vScpipPrintCurrentScale(UCHAR ucVdpId);
EXTERN void vScpipDumpResInfo(UINT8 bPath);
EXTERN void vScpipForceWriteOff(UINT32 u4VdpId, UINT8 u1OnOff); 
EXTERN UINT16 _VDP_ScposGetPorch(UINT8 bPath, UINT8 bPorchType);
EXTERN void _VDP_ScposSetPorch(UINT8 bPath,UINT8 bPorchType,UINT16 wValue);
#ifdef CC_COPLAT_MT82
#define VDP_POP_WA
#endif
#ifdef VDP_POP_WA
EXTERN UINT32 _VDP_SetTVMode(UINT32 u4TVMode);
EXTERN UINT32 _VDP_HackSubPath(void);
#endif

EXTERN void vScpipReconfig(UINT32 ucVdpId);
EXTERN UINT8 u1ScpipGetClipInfo(UINT8 bPath,PSCAN_CLIP_INFO_T* clipInfo);
EXTERN UINT8 u1ScpipGetMJCClipInfo(UINT8 bPath,MJC_CLIP_INFO_T* clipInfo);
EXTERN void vScpipSwInit(void);
EXTERN UINT16 u2ScpipCalculateOutputVTotal(void); 
EXTERN UINT32 u4ScpipGetPreScaleWidth(UINT8 bPath);
EXTERN UINT8 u1ScpipIsVideoFreezable(UINT8 bPath);
EXTERN UINT32 u4ScpipGetInOutVDistance(void);
EXTERN UINT32 u4ScpipGetFrameDelay(UINT32 u4VdpId);
EXTERN void vDrvScpipSrmDispCb(UINT32 u4VdpId, UINT32 u4Mode, UINT32 u4PDSize, UINT32 u4SramMode);
EXTERN void vScpipSetFreeze(UINT8 bPath, UINT8 bOnOff) ;
EXTERN UINT32 u4ScpipGetBufNum(UINT32 u4VdpId);
#ifdef CC_SCPOS_3DTV_SUPPORT
EXTERN void vScpipOnTVEInputVSync(void);
#endif

#ifdef SCPIP_LI_TO_SBS_VSYNC_OPT_WA
void vScpipDoLIToSBSVsyncOptWA(UINT8 bPath, UINT8 bReset);
#endif

EXTERN void vScpipOnMJCInputVSync(void);
EXTERN void vScpipOnMJCOutputVSync(void);
EXTERN void vScpipOnDIOutputISR(void);
EXTERN void vScpipOnInputVSync(UINT8 bPath);
EXTERN UINT16 u2ScpipGetDispWidth(UINT8 bPath);
EXTERN UINT16 u2ScpipGetDispHeight(UINT8 bPath);
EXTERN void vScpipForceDispOff(UINT32 u4Path, UINT32 u4OnOff);
EXTERN void vScpipNormProc(void);
EXTERN void vScpipSetHNegInitPhaseOnOff(const BOOL fgOnOff);
EXTERN void vScpipSetVNegInitPhaseOnOff(const BOOL fgOnOff);
EXTERN void vScpipHwInitOnPanelInit(UINT8 bPath);

#if defined(CC_MT5396)
EXTERN void vScpipSetFSCImportToggle(void);
EXTERN void vScpipSetPSCImportToggle(void);
#endif

//*****************************************************************************
//  drv_scaler_ptgen.c
//*****************************************************************************
EXTERN void vScpipSetDSPattern(UINT32 u4VdpId, UINT32 u4OnOff, UINT32 u4HTotal, UINT32 u4Width, UINT32 u4VTotal, UINT32 u4Height);
EXTERN void vScpipSetUSPattern(UINT32 u4VdpId, UINT32 u4OnOff, UINT32 u4Width, UINT32 u4Height);
#if SUPPORT_DOT_TO_DOT_PATTERN
EXTERN void vScpipDotToDotPattern(UINT8 ucOnOff,UINT32 u4RGBColor);
#endif

//*****************************************************************************
//  drv_scaler_nonlinear.c
//*****************************************************************************
EXTERN void vScpipSetNonLinearUpScaler(UCHAR ucVdpId, UCHAR ucOnOff);
EXTERN void vScpipSetNewNonLinearOnOff(UCHAR ucVdpId, UCHAR ucOnOff);
EXTERN void vScpipSetNewNonLinear(const UINT32 u4VdpId, VDP_NEW_NONLINEAR_PRM_T rNonlPrm);
EXTERN void vScpipGetNewNonLinear(const UINT32 u4VdpId, VDP_NEW_NONLINEAR_PRM_T *prNonlPrm);
EXTERN void vScpipSetAutoNonLinear(UINT32 u4VdpId, VDP_AUTO_NONLINEAR_PRM_T rAutoNonlinearPrm); 
EXTERN void vScpipSetManualNonLinear(UINT32 u4VdpId, VDP_NONLINEAR_PRM_T rNonlinearPrm);

EXTERN BOOL fgForceScalerDramDump(UINT32 u4VdpId, BOOL fgOnOff);
EXTERN void vScpipSetDumpCtrl(UINT8 bOnOff);
EXTERN UINT8 vScpipGetDumpCtrl(void);

EXTERN void vScpipGetDramInf(UINT8 bPath,SCALER_INFO_T* inf);

EXTERN UINT32 u4ScpipGet10BitMode(UINT32 u4VdpId);
EXTERN UINT32 u4ScpipGetForced10BitMode(UINT32 u4VdpId);

EXTERN void vScpipSetTveEnable(UINT8 bOnOff);
EXTERN void vScpipSetTveUpEnable(UINT8 bOnOff);
EXTERN void vScpipSetTvePal(UINT8 bOnOff);
EXTERN void vScpipSetTveOsdPrelen(UINT8 prelen);

EXTERN void vScpipSetPanelVOfstVar(UINT32 panelVofst);
EXTERN void  vScpipSetFrameTrackTargetVar(UINT32 target);
EXTERN UINT32 u4ScpipGetFrameTrackTargetVar(void);
EXTERN void vScpipFrameTrackConfig(UINT32 u4Step);
EXTERN void vScpipOnVdoModeChange(UINT8 bPath);
EXTERN void vScpipOnVdoModeChangeDone(UINT8 bPath);
EXTERN void vScpipSetPscanPrelen(UINT8 bPath,UINT32 u4PreLen);
EXTERN UINT32 u4ScpipGetPscanPrelen(UINT8 bPath);
#endif

