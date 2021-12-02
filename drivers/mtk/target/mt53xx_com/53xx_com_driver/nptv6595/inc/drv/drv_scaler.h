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

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

// TODO: remove
//#define MT5360B_WA4
#ifdef MT5360B_WA4
#define MT5360B_WA4_DLY1 4 // decoder + 4 pixels
#endif

// TODO: used in other files, move?
// vsync delay constant
#define VDP_VD_DISABLE              0
#define VDP_VD_TURN_DISABLE         1
#define VDP_VD_ENABLE               2
#define VDP_VD_TURN_ENABLE          3
#define VDP_VD_DELAY_COUNT          7

#ifdef CC_MT5395
#define MAIN_ACTIVE_ADJ 0x3B
#define SUB_ACTIVE_ADJ 0xCC
#define SUB_READ_DELAY 0xB5
#else
#define MAIN_ACTIVE_ADJ 0x38
#define SUB_ACTIVE_ADJ 0xB2
#define SUB_READ_DELAY 0x9B
#endif

#define VDP_DS_PTN_OFF 0
#define VDP_DS_PTN_AUTO 1
#define VDP_DS_PTN_MANUAL 2

#define VDP_US_PTN_OFF 0
#define VDP_US_PTN_AUTO 1
#define VDP_US_PTN_MANUAL 2

#define getPicInfo(bPath)   ((bPath==SV_VP_MAIN)?&_rMPicInfo:&_rPPicInfo)
#define getDispPrm(bPath)   ((bPath==SV_VP_MAIN)?&_rMDispPrm:&_rPDispPrm)
#define getMixedHPorch(bPath) ((getPicInfo(bPath)->wXOffset + getPicInfo(bPath)->wUIHPorch >1000)? (getPicInfo(bPath)->wXOffset + getPicInfo(bPath)->wUIHPorch - 1000): 0)
#define getMixedVPorch(bPath) ((getPicInfo(bPath)->wYOffset + getPicInfo(bPath)->wUIVPorch >1000)? (getPicInfo(bPath)->wYOffset + getPicInfo(bPath)->wUIVPorch - 1000): 0)
#define vDrvMainUpdateVBound(u2Vline) (vRegWriteFldAlign(SCPIP_US1_14, u2Vline, US1_14_UP_VBOUND_Y1))


// TODO: local only?
typedef struct
{
    UINT32 u4Addr;
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4MddiWidth;
    UINT32 u4FbmSize;
    #ifndef CC_UP8032_ATV
    BOOL fg2FB;
    BOOL fg10bit;
    BOOL fg8bit;
    BOOL fg422;
    #else
    UINT8 fg2FB;
    UINT8 fg10bit;
    UINT8 fg8bit;
    UINT8 fg422;
    #endif
} SCALER_FBM_POOL_T;

typedef struct
{
    UINT16 wX;
    UINT16 wY;
    UINT16 wWidth;
    UINT16 wHeight;
}PSCAN_CLIP_INFO_T;

#if defined(CC_MT5365) || defined(CC_MT5395)
typedef struct
{
    UINT8 dram_from;         // 3 scaler,2 DI , 1 B2R
    UINT8 format;
    UINT16 data_length;
    UINT32 src_width;
    UINT32 src_height;
    UINT32 dramSize;
    UINT32 startAddr;
}Scaler_Inf_T;

typedef union
{
    Scaler_Inf_T scaler_inf;
    B2R_DISP_INFO_T B2R_inf;
    DI_DISP_INFO_T DI_inf;
}DRAM_INFO_T;

#endif
//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//*****************************************************************************
//  drv_scaler.c
//*****************************************************************************

//-----------------------------------------------------------------------------
/** Brief of vScpipWriteCtrlOnOff.
 * Turn On/Off Main Channel Write Control 
 * @param  u1OnOff(SV_ON/SV_OFF)
 * @retval void 
 */
//-----------------------------------------------------------------------------
EXTERN void vScpipWriteCtrlOnOff(UCHAR ucVdpId, UCHAR ucOnOff);

EXTERN UINT8 u1ScpipGetDispMode(UINT8 bPath);

//-----------------------------------------------------------------------------
/** Brief of vScpipSetGameMode.
 * Enable/Disable game mode
 */
//-----------------------------------------------------------------------------
// TODO: obselete?
EXTERN void vScpipSetGameMode(UCHAR ucVdpId, UCHAR ucOnOff);


//-----------------------------------------------------------------------------
/** Brief of vScpipSet121Map.
 */
//-----------------------------------------------------------------------------
EXTERN void vScpipSet121Map(UCHAR ucVdpId, UCHAR ucOnOff);
EXTERN UINT32 vScpipGet121Map(UCHAR ucVdpId);

//-----------------------------------------------------------------------------
/**
* @brief Set dot by dot mode and bypass VDS/VUS
* @param ucVdpId VDP_1/VDP_2
* @param ucOnOff On/Off
*/
//-----------------------------------------------------------------------------
//EXTERN void vScpipForceBypassOnOff(UCHAR ucVdpId, UCHAR ucOnOff);

//-----------------------------------------------------------------------------
/** Brief of vDrvVideoFixedPreScaler.
 */
//-----------------------------------------------------------------------------
EXTERN void vDrvVideoFixedPreScaler(UCHAR ucVdpId, UCHAR ucOnOff, UCHAR uc1stConfig);

//-----------------------------------------------------------------------------
/** Brief of _VDP_Set10bitMode.
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_Set10bitMode(UINT32 u4VdpId, UINT32 u4Mode);

//-----------------------------------------------------------------------------
/** Brief of vScpipSetDynamicScaler.
 */
//-----------------------------------------------------------------------------
EXTERN void vScpipSetDynamicScaler(UCHAR ucVdpId, UCHAR ucOnOff, UINT32 u4ResponseTime);


//-----------------------------------------------------------------------------
/** Brief of _VDP_DispModeEnable.
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_DispModeEnable(UCHAR ucVdpId, UCHAR ucOnOff);

//-----------------------------------------------------------------------------
/** Brief of _VDP_SetVspTune.
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_SetVspTune(UINT32 u4Vsp, UINT32 u4Tune);

//-----------------------------------------------------------------------------
/** Brief of _VDP_DispModeTune.
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_DispModeTune(UINT32 u4Auto, UINT32 u4Up, UINT32 u4Print);

//-----------------------------------------------------------------------------
/** Brief of vScpipDelayEnable.
 *
 */
//-----------------------------------------------------------------------------
EXTERN void vScpipDelayEnable(UCHAR ucVdpId, UCHAR ucEnable,
                                  UINT32 u4DelayCount);
//-----------------------------------------------------------------------------
/** Brief of _VDP_HorizontalPreScaleDownFactor.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 u4ScpipGetHorizontalPreScaleDownFactor(UCHAR ucVdpId);

//-----------------------------------------------------------------------------
/** Brief of _VDP_SetHorizontalPreScaleDownFactor.
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_SetHorizontalPreScaleDownFactor(UINT32 u4VdpId, UINT32 u4Factor);

//-----------------------------------------------------------------------------
/** Brief of vScpipDispmodeFreeRun.
 */
//-----------------------------------------------------------------------------
EXTERN void vScpipDispmodeFreeRun(BOOL bEnable);

//-----------------------------------------------------------------------------
/** Brief of u4ScpipDispmodeFifoOverflow.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 u4ScpipDispmodeFifoOverflow(void);

//-----------------------------------------------------------------------------
/** Brief of vScpipPrintCurrentScale.
 */
//-----------------------------------------------------------------------------
EXTERN void vScpipPrintCurrentScale(UCHAR ucVdpId);

//-----------------------------------------------------------------------------
/** Brief of vScpipForceWriteOff.
 */
//-----------------------------------------------------------------------------
EXTERN void vScpipForceWriteOff(UINT32 u4VdpId, UINT8 u1OnOff); 

EXTERN UINT16 _VDP_ScposGetPorch(UINT8 bPath, UINT8 bPorchType);

EXTERN void _VDP_ScposSetPorch(UINT8 bPath,UINT8 bPorchType,UINT16 wValue);

// TODO: Where are these functions?
#ifdef CC_COPLAT_MT82
#define VDP_POP_WA
#endif
#ifdef VDP_POP_WA
EXTERN UINT32 _VDP_SetTVMode(UINT32 u4TVMode);
EXTERN UINT32 _VDP_HackSubPath(void);
#endif

//-----------------------------------------------------------------------------
/** Brief of vScpipReconfig.
 */
//-----------------------------------------------------------------------------
EXTERN void vScpipReconfig(UCHAR ucVdpId);

//-----------------------------------------------------------------------------
/** Brief of u1ScpipGetClipInfo.
 *  Used to get overscan info after doing pre-scaling down
 */
//-----------------------------------------------------------------------------
EXTERN UINT8 u1ScpipGetClipInfo(UINT8 bPath,PSCAN_CLIP_INFO_T* clipInfo);

#ifdef CC_FAST_INIT 
EXTERN void vScpipInit(UINT8 bResume);
#else
EXTERN void vScpipInit(void);
#endif

//-----------------------------------------------------------------------------
/** Brief of u2ScpipCalculateOutputVTotal.
 * Calculate Output Total Lines in the Vertical Direction
 * @param  void
 * @retval VTotal Lines 
 */
//-----------------------------------------------------------------------------
EXTERN UINT16 u2ScpipCalculateOutputVTotal(void); 

EXTERN UINT32 u4ScpipGetPreScaleWidth(UINT8 bPath);
//-----------------------------------------------------------------------------
/** Brief of u1ScpipIsVideoFreezable.
 * Check if video can be freezed or not.
 * @param  bPath
 * @retval SV_TRUE if video can be freezed.
 *         SV_FALSE if video can't be freezed.
 */
//-----------------------------------------------------------------------------
EXTERN UINT8 u1ScpipIsVideoFreezable(UINT8 bPath);

//-----------------------------------------------------------------------------
/** Brief of u4ScpipGetInOutVDistance.
 * Get distance between in and out V sync.
 * @param  void
 * @retval in/out v distance
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 u4ScpipGetInOutVDistance(void);

//-----------------------------------------------------------------------------
/** u1ScpipModeChangeDone
 *  For pscan to get mode change done information.
 *  bPath Path information.
 */
//-----------------------------------------------------------------------------
//EXTERN UINT8 u1ScpipModeChangeDone(UINT8 bPath);

//-----------------------------------------------------------------------------
/** Brief of u4ScpipGetFrameDelay.
 * Get Scalar Frame Delay.
 * @param u4VdpId video plane ID 0~1
 * @retval Frame Delay
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 u4ScpipGetFrameDelay(UINT32 u4VdpId);

//-----------------------------------------------------------------------------
/** VDP scaler control call back
 *
 * @return void
 */
//-----------------------------------------------------------------------------
EXTERN void vDrvScpipSrmDispCb(UINT32 u4VdpId, UINT32 u4Mode, UINT32 u4PDSize, UINT32 u4SramMode);


//-----------------------------------------------------------------------------
/**
 * @brief vScpipSetFreeze 
 * Freeze Video in scalar
 * @param  bPath: SV_VP_MAIN/SV_VP_PIP
 *         bOnOff: SV_ON/SV_OFF
 * @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void vScpipSetFreeze(UINT8 bPath, UINT8 bOnOff) ;

//-----------------------------------------------------------------------------
/**
 * @brief u4ScpipGetHPS
 * Get pre-scaling down factor
 * @param  bPath: SV_VP_MAIN/SV_VP_PIP
 * @retval void
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 u4ScpipGetHPS(UINT8 bPath);
// TODO: local function?
EXTERN void vScpipSrmSetBuf(UINT32 u4VdpId, const SCALER_FBM_POOL_T * prFbmPool); 
// TODO: local function?
EXTERN UINT32 u4ScpipGetBufNum(UINT32 u4VdpId);
EXTERN void vScpipToggleForceOff(UINT32 u4VdpId);
#ifdef CC_SCPOS_3DTV_SUPPORT
EXTERN void vScpipOnMJCInputVSync(void);
EXTERN void vScpipOnDIOutputISR(void);
EXTERN void vScpipOnTVEInputVSync(void);
#endif
EXTERN void vScpipOnInputVSync(UINT8 bPath);
EXTERN UINT16 u2ScpipGetDispWidth(UINT8 bPath);
EXTERN UINT16 u2ScpipGetDispHeight(UINT8 bPath);

//-----------------------------------------------------------------------------
/**
 * @brief vScpipForceDispOff , currently for TVE use
 * @param  u4VdpId: video path
 * @param  u4OnOff: SV_TRUE(Display off), SV_FALSE (Display Normal)
 * @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void vScpipForceDispOff(UINT32 u4Path, UINT32 u4OnOff);

#if PSCAN_DISPMODE_MIRROR_WA
EXTERN UINT8 u1ScpipPScanDispMirrorWA(UINT8 bPath, UINT8 bOnOff);
#endif
// For pscan display mode hlen update
EXTERN void vScpipSetPscanDispmodeHLen(UINT8 bPath);

EXTERN void vScpipNormProc(void);

EXTERN void vScpipSetHNegInitPhaseOnOff(const BOOL fgOnOff);
EXTERN void vScpipSetVNegInitPhaseOnOff(const BOOL fgOnOff);

//*****************************************************************************
//  drv_scaler_ptgen.c
//*****************************************************************************

//-----------------------------------------------------------------------------
/**
 * @brief vScpipSetDSPattern
 * Set DS inner pattern generator
 * @param  u4VdpId: video path
 * @param  u4OnOff: VDP_DS_PTN_OFF, VDP_DS_PTN_AUTO, and VDP_DS_PTN_MANUAL
 * @param  u4HTotal: htotal
 * @param  u4Width: video active width
 * @param  u4VTotal: vtotal
 * @param  u4Height: video active height
 * @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void vScpipSetDSPattern(UINT32 u4VdpId, UINT32 u4OnOff, UINT32 u4HTotal, UINT32 u4Width, UINT32 u4VTotal, UINT32 u4Height);

//-----------------------------------------------------------------------------
/**
 * @brief vScpipSetUSPattern
 * Set US inner pattern generator
 * @param  u4VdpId: video path
 * @param  u4OnOff: VDP_US_PTN_OFF, VDP_US_PTN_AUTO, and VDP_US_PTN_MANUAL
 * @param  u4Width: video active width
 * @param  u4Height: video active height
 * @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void vScpipSetUSPattern(UINT32 u4VdpId, UINT32 u4OnOff, UINT32 u4Width, UINT32 u4Height);

#if SUPPORT_DOT_TO_DOT_PATTERN
EXTERN void vScpipDotToDotPattern(UINT8 ucOnOff,UINT32 u4RGBColor);
#endif
#if SUPPORT_SBS_CONVERT
EXTERN void vScpipSBSConVert(UINT8 ucOnOff);
#endif

#if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
EXTERN void vScpipLRInverseWA(UINT8 bInverse);
EXTERN void vScpipSetDepthWA(UINT8 bDepth);
EXTERN void vScpipSetOverscanWA(UINT8 bOverscan);
EXTERN void vScpip3DPRWA(void);
#endif

//*****************************************************************************
//  drv_scaler_nonlinear.c
//*****************************************************************************

//-----------------------------------------------------------------------------
/** Brief of vScpipSetNonLinearUpScaler.
 * Enable/Disable nonlinear up scaler
 */
//-----------------------------------------------------------------------------

EXTERN void vScpipSetNonLinearUpScaler(UCHAR ucVdpId, UCHAR ucOnOff);
/**
* @brief Set new nonlinear (MT5365/MT5395)
* @param u4VdpId VDP_1/VDP_2
* @param rNonlPrm nonlinear paramters
*/
EXTERN void vScpipSetNewNonLinear(const UINT32 u4VdpId, VDP_NEW_NONLINEAR_PRM_T rNonlPrm);
/**
* @brief Get new nonlinear
* @param u4VdpId VDP_1/VDP_2
* @param prNonlPrm nonlinear paramters
*/
EXTERN void vScpipGetNewNonLinear(const UINT32 u4VdpId, VDP_NEW_NONLINEAR_PRM_T *prNonlPrm);

//-----------------------------------------------------------------------------
/** Brief of vScpipSetAutoNonLinear.
 * Set nonlinear up scaler scope and end point factor
 */
//-----------------------------------------------------------------------------
EXTERN void vScpipSetAutoNonLinear(UINT32 u4VdpId, VDP_AUTO_NONLINEAR_PRM_T rAutoNonlinearPrm); 

//-----------------------------------------------------------------------------
/** Brief of vScpipSetManualNonLinear.
 * Set manual nonlinear scaling factors in register value
 */
//-----------------------------------------------------------------------------
EXTERN void vScpipSetManualNonLinear(UINT32 u4VdpId, VDP_NONLINEAR_PRM_T rNonlinearPrm);
EXTERN BOOL fgForceScalerDramDump(UINT32 u4VdpId, BOOL fgOnOff);
EXTERN void vScpipSetDumpCtrl(UINT8 bOnOff);
EXTERN UINT8 vScpipGetDumpCtrl(void);

#if defined(CC_MT5365) || defined(CC_MT5395)
EXTERN void  vScpipGetDramInf(UINT8 bPath,DRAM_INFO_T* inf);
#endif
EXTERN UINT32 u4ScpipGet10BitMode(UINT32 u4VdpId);
EXTERN UINT32 u4ScpipGetForced10BitMode(UINT32 u4VdpId);
EXTERN UINT8 u1ScpipGet444Mode(UINT8 bPath);
#endif
