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


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//#define MT5360B_WA4
#ifdef MT5360B_WA4
#define MT5360B_WA4_DLY1 4 // decoder + 4 pixels
#endif

#define CC_NEW_WRITE_CTRL 1
#define VDP_NONLINEAR_AUTO      255
#define UINT_NONLINEAR_END_FACTOR	(1024)

// vsync delay constant
#define VDP_VD_DISABLE              0
#define VDP_VD_TURN_DISABLE         1
#define VDP_VD_ENABLE               2
#define VDP_VD_TURN_ENABLE          3
#define VDP_VD_DELAY_COUNT          7

#define getPSCANOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x800)
#define getPIPOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x80)
#define getDSOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x80)
#define getUSOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x80)
#define getPicInfo(bPath)   ((bPath==SV_VP_MAIN)?&_rMPicInfo:&_rPPicInfo)
#define getDispInfo(bPath)   ((bPath==SV_VP_MAIN)?&_rMDispInfo:&_rPDispInfo)
#define getDispPrm(bPath)   ((bPath==SV_VP_MAIN)?&_rMDispPrm:&_rPDispPrm)
#define getChannel(bPath)  ((bPath==SV_VP_MAIN)?&_rMChannel:&_rPChannel)
#define getScalerPath(VDP_ID) ((VDP_ID==VDP_1)?SV_VP_MAIN:SV_VP_PIP)
#define getScalerVDP(bPath)  ((bPath==SV_VP_MAIN)?VDP_1:VDP_2)
#define fireSCPOSModeChange(VDP_ID) ((VDP_ID==VDP_1)?vSetScposFlg(SCALER_MAIN_MODE_CHG):vSetScposFlg(SCALER_PIP_MODE_CHG))
#define getScalerMode(bPath) ((bPath==SV_VP_MAIN)?_arScalePrm[VDP_1].u4DispMode:_arScalePrm[VDP_2].u4DispMode)
#define getMixedHPorch(bPath) ((getPicInfo(bPath)->wXOffset + getPicInfo(bPath)->wUIHPorch >1000)? (getPicInfo(bPath)->wXOffset + getPicInfo(bPath)->wUIHPorch - 1000): 0)
#define getMixedVPorch(bPath) ((getPicInfo(bPath)->wYOffset + getPicInfo(bPath)->wUIVPorch >1000)? (getPicInfo(bPath)->wYOffset + getPicInfo(bPath)->wUIVPorch - 1000): 0)

#define MAIN_ACTIVE_ADJ 0x39
#ifdef CC_MT5363
#define SUB_ACTIVE_ADJ 0xb8
#define SUB_READ_DELAY 0xa1
#else
#define SUB_ACTIVE_ADJ 0xac
#define SUB_READ_DELAY 0x95
//#define SUB_READ_DELAY_E1 0x83
#endif

#define VDP_DS_PTN_OFF 0
#define VDP_DS_PTN_AUTO 1
#define VDP_DS_PTN_MANUAL 2

#define VDP_US_PTN_OFF 0
#define VDP_US_PTN_AUTO 1
#define VDP_US_PTN_MANUAL 2


/** Brief of VDP_SCALER_PRM_T
 */
typedef struct
{
    UINT32 u4Enable;
    UINT32 u4HpsFactor;
    #if !CC_NEW_WRITE_CTRL
    UINT32 u4NotReady;
    #endif
    UINT32 u4DynamicScale;
    UINT32 u4ResponseTime;

    UINT32 u4MddiEn;
    UINT32 u4FixedPreScaler;
    UINT32 u4ForcedPreScalingactor;
    UINT32 u4PreScalerNotOptimized;
    UINT32 u4FirstConfig;
    UINT32 u4SrcWidth;
    UINT32 u4SrcHeight;
    UINT32 u4MddiFifoMin;
    UINT32 u4MddiFifoMax;

    UINT32 u410BitMode;
    UINT32 u4Is422;
    UINT32 u4121MapEn;
    UINT32 u4121Map;

    UINT32 u4ForceBypass;
    
    UINT32 u4DispModeEn;
    UINT32 u4DispMode;          //from SRM to tell Scaler what mode should be set
    
    UINT32 u4NonlinearEn;
    UINT32 u4NonlinearCtrl;
    UINT32 u4LastNonlinearCtrl;   
    UINT32 u4NonlinearAuto;
    VDP_AUTO_NONLINEAR_PRM_T rAutoNonlinearPrm;    ///< non-linear scaling setting for auto mode
    VDP_NONLINEAR_PRM_T rNonlinearPrm;
    VDP_NONLINEAR_PRM_T rLastNonlinearPrm;

    UINT32 u4Width;
    UINT32 u4Height;

    UINT32 u4DramWidth;
    UINT32 u4DramHeight;
    UINT32 u4MddiDramWidth;

    UINT32 u4FbAddr1;
    UINT32 u4FbAddr2;
    UINT32 u4FbAddr3;
} VDP_SCALER_PRM_T;

extern VDP_SCALER_PRM_T _arScalePrm[VDP_NS];
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

#if defined(CC_MT5363)
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
}DRAM_INFO_T;

#endif

/*
typedef enum{
    E_IPC_DRAM,
    E_IPC_SCALER_DISP,
    E_IPC_PSCAN_DISP,
    E_IPC_OFF,
    E_IPC_WINDOW_OFF,
    E_IPC_FORCE_CHANGE,
} E_IPC_MODE;
*/

typedef enum{
    E_FIFO_MJC_DIPMODE_UP,
    E_FIFO_PSCAN_DISPMODE_UP,
    E_FIFO_OTHERS,
} E_IPC_FIFO_CONFIG_MODE;

typedef enum {
    E_V_M_UPSCALE   = 0x01,
    E_V_M_DOWNSCALE = 0x02,
    E_V_M_NOSCALE   = 0x04,
    E_V_S_UPSCALE   = 0x10,
    E_V_S_DOWNSCALE = 0x20,
    E_V_S_NOSCALE   = 0x40
} E_V_SCALE;

typedef enum {
    E_DIRECTION_H,
    E_DIRECTION_V,
} E_DIRECTION;

/*typedef enum
{
    PSCAN_OCLK_TYPE_VDOIN = 0x8,
    PSCAN_OCLK_TYPE_OCLK  = 0xC,
}PSCAN_OUTPUT_CLOCK_TYPE;
*/

typedef struct 
{
    UINT32 u4PanelW;
    UINT32 u4PanelH;
    UINT32 u4SrcW;
    UINT32 u4SrcH;
    UINT32 u4USH;
    UINT32 u4Adj;
} Delay_Tbl;

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of vDrvImportOnOff.
 * @brief		Set import protection on/off for specified path
 */
//-----------------------------------------------------------------------------
EXTERN void vDrvImportOnOff(UINT8 bPath, UINT8 bOnOff);

//-----------------------------------------------------------------------------
/** Brief of vDrvVideoSetDispMask.
 * @brief		Set Output Mask according to current Main/Pip window size and position
 */
//-----------------------------------------------------------------------------
//EXTERN void vDrvVideoSetDispMask(UINT32 u4VdpId, UINT32 u4OnOff);

//-----------------------------------------------------------------------------
/** Brief of vDrvScpipWriteCtrl.
 * Turn On/Off Main Channel Write Control 
 * @param  u1OnOff(SV_ON/SV_OFF)
 * @retval void 
 */
//-----------------------------------------------------------------------------
EXTERN void vDrvScpipWriteCtrl(UCHAR ucVdpId, UCHAR ucOnOff);

//-----------------------------------------------------------------------------
/** Brief of vDrvPipWriteCtrl.
 * Turn On/Off PIP Channel Write Control
 * @param  u1OnOff(SV_ON/SV_OFF)
 * @retval void 
 */
//-----------------------------------------------------------------------------
EXTERN void vDrvPipWriteCtrl(UINT8 u1OnOff);

EXTERN UINT8 _VDP_vGetDispModeOnOff(UINT8 bPath);

//-----------------------------------------------------------------------------
/** Brief of _VDP_VideoSetGameMode.
 * Enable/Disable game mode
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_VideoSetGameMode(UCHAR ucVdpId, UCHAR ucOnOff);

//-----------------------------------------------------------------------------
/** Brief of _VDP_VideoSetNonLinearUpScaler.
 * Enable/Disable nonlinear up scaler
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_VideoSetNonLinearUpScaler(UCHAR ucVdpId, UCHAR ucOnOff);

//-----------------------------------------------------------------------------
/** Brief of _VDP_VideoSetNonLinearUpScalerScope.
 * Set nonlinear up scaler scope
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_VideoSetNonLinearUpScalerScope(UCHAR ucVdpId, UCHAR ucScope);

//-----------------------------------------------------------------------------
/** Brief of _VDP_SetAutoNonLinear.
 * Set nonlinear up scaler scope and end point factor
 */
//-----------------------------------------------------------------------------
//EXTERN void _VDP_VideoSetNonLinearUpScalerFactor(UCHAR ucVdpId, UCHAR ucScope, UINT32 u4Factor) ;
EXTERN void _VDP_SetAutoNonLinear(UINT32 u4VdpId, VDP_AUTO_NONLINEAR_PRM_T rAutoNonlinearPrm); 

//-----------------------------------------------------------------------------
/** Brief of _VDP_SetManualNonLinear.
 * Set manual nonlinear scaling factors in register value
 */
//-----------------------------------------------------------------------------
//EXTERN void _VDP_SetManualNonLinear(UCHAR ucVdpId, BOOL bAuto, UINT32 u4StartFactor, UINT32 u4Slope,  UINT32 u4MidPoint,UINT32 u4StopFactor);
EXTERN void _VDP_SetManualNonLinear(UINT32 u4VdpId, VDP_NONLINEAR_PRM_T rNonlinearPrm);

//-----------------------------------------------------------------------------
/** Brief of _VDP_VideoSet121Map.
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_VideoSet121Map(UCHAR ucVdpId, UCHAR ucOnOff);

//-----------------------------------------------------------------------------
/**
* @brief Set dot by dot mode and bypass VDS/VUS
* @param ucVdpId VDP_1/VDP_2
* @param ucOnOff On/Off
*/
//-----------------------------------------------------------------------------
EXTERN void vDrvScpipForceBypass(UCHAR ucVdpId, UCHAR ucOnOff);

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
/** Brief of _VDP_VideoSetDynamiceScaler.
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_VideoSetDynamiceScaler(UCHAR ucVdpId, UCHAR ucOnOff, UINT32 u4ResponseTime);

//-----------------------------------------------------------------------------
/** Brief of _VDP_VideoSetOutputRate.
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_VideoSetOutputRate(UINT32 u4OutputRate);

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
/** Brief of _VDP_ScposDelayEnable.
 *
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_ScposDelayEnable(UCHAR ucVdpId, UCHAR ucEnable,
                                  UINT32 u4DelayCount);
//-----------------------------------------------------------------------------
/** Brief of _VDP_HorizontalPreScaleDownFactor.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 _VDP_GetHorizontalPreScaleDownFactor(UCHAR ucVdpId);

//-----------------------------------------------------------------------------
/** Brief of _VDP_SetHorizontalPreScaleDownFactor.
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_SetHorizontalPreScaleDownFactor(UINT32 u4VdpId, UINT32 u4Factor);

//-----------------------------------------------------------------------------
/** Brief of _VDP_HandleUnMute.
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_HandleUnMute(void);

#if !CC_NEW_WRITE_CTRL
//-----------------------------------------------------------------------------
/** Brief of _VDP_ScposNotReady.
 */
//-----------------------------------------------------------------------------
//EXTERN void _VDP_ScposNotReady(UCHAR ucVdpId, UINT32 u4NotReady);

//-----------------------------------------------------------------------------
/** Brief of _VDP_ScposEnable.
 * SCPOS Enable/Disable
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_ScposEnable(UCHAR ucVdpId, UCHAR ucEnable);

//-----------------------------------------------------------------------------
/** Brief of _VDP_ScposChkDelayEnable.
 *
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_ScposChkDelayEnable(UCHAR ucVdpId);
#endif

//-----------------------------------------------------------------------------
/** Brief of _VDP_ScposDispmodeFreeRun.
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_ScposDispmodeFreeRun(BOOL bEnable);

//-----------------------------------------------------------------------------
/** Brief of _VDP_ScposDispModeFIFOOverflow.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 _VDP_ScposDispModeFIFOOverflow(void);

//-----------------------------------------------------------------------------
/** Brief of VDP_PrintCurrentScale.
 */
//-----------------------------------------------------------------------------
EXTERN void VDP_PrintCurrentScale(UCHAR ucVdpId);

//-----------------------------------------------------------------------------
/** Brief of _VDP_vDrvForceWriteOff.
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_vDrvForceWriteOff(UINT32 u4VdpId, UINT8 u1OnOff); 

//-----------------------------------------------------------------------------
/** Brief of _VDP_wGetHPorch.
 */
//-----------------------------------------------------------------------------
//EXTERN UINT16 _VDP_wGetHPorch(UINT8 bPath);

//-----------------------------------------------------------------------------
/** Brief of _VDP_vSetHPorch.
 */
//-----------------------------------------------------------------------------
//EXTERN void _VDP_vSetHPorch(UINT8 bPath, UINT32 porch);

//-----------------------------------------------------------------------------
/** Brief of _VDP_wAdjHPorch.
 */
//-----------------------------------------------------------------------------
//EXTERN UINT16 _VDP_wAdjHPorch(UINT8 bPath, UINT8 dir, UINT16 value);

EXTERN void  _VDP_ScposPorchUpdate(UINT8 bPath,UINT8 bEvent);
EXTERN UINT16 _VDP_ScposGetPorch(UINT8 bPath, UINT8 bPorchType);
EXTERN void _VDP_ScposSetPorch(UINT8 bPath,UINT8 bPorchType,UINT16 wValue);

#ifdef CC_MT5391
//-----------------------------------------------------------------------------
/** Brief of _VDP_wGetVF0Porch.
 */
//-----------------------------------------------------------------------------
//EXTERN UINT16 _VDP_wGetVPorch(UINT8 bPath);

//-----------------------------------------------------------------------------
/** Brief of _VDP_vSetVF0Porch.
 */
//-----------------------------------------------------------------------------
//EXTERN void _VDP_vSetVPorch(UINT8 bPath, UINT32 porch);
#endif

//-----------------------------------------------------------------------------
/** Brief of _VDP_wAdjVPorch.
 */
//-----------------------------------------------------------------------------
//EXTERN UINT16 _VDP_wAdjVPorch(UINT8 bPath, UINT8 dir, UINT16 value);

//-----------------------------------------------------------------------------
#ifdef CC_COPLAT_MT82
#define VDP_POP_WA
#endif
#ifdef VDP_POP_WA
EXTERN UINT32 _VDP_SetTVMode(UINT32 u4TVMode);
EXTERN UINT32 _VDP_HackSubPath(void);
#endif

//-----------------------------------------------------------------------------
/** Brief of _VDP_ScalerReconfig.
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_ScalerReconfig(UCHAR ucVdpId);

//-----------------------------------------------------------------------------
/** Brief of bScalerGetClipInfo.
 *  Used to get overscan info after doing pre-scaling down
 */
//-----------------------------------------------------------------------------
EXTERN UINT8 bScalerGetClipInfo(UINT8 bPath,PSCAN_CLIP_INFO_T* clipInfo);

//-----------------------------------------------------------------------------
/** Brief of bDrvScalerGetVscale.
 *  For SRM to get Vertical scaling info
 */
//-----------------------------------------------------------------------------
EXTERN UINT8 bDrvScalerGetVscale(void);

EXTERN void vScpipTriggerFlipImport(UINT32 u4VdpId);

#ifdef CC_SUPPORT_TVE
EXTERN UINT32 _SCPIP_SetTVEEnable(UINT32 u4VdpId, UINT32 u4OnOff, UINT32 u4Width, UINT32 u4Height);
#endif
EXTERN void vDrvScpipInit(void);

//-----------------------------------------------------------------------------
/** Brief of wCalculateOutputVTotal.
 * Calculate Output Total Lines in the Vertical Direction
 * @param  void
 * @retval VTotal Lines 
 */
//-----------------------------------------------------------------------------
EXTERN UINT16 wCalculateOutputVTotal(void); 

EXTERN UINT32 wDrvVideoGetPreScaleWidth(UINT8 bPath);
//-----------------------------------------------------------------------------
/** Brief of fgScposIsVideoFreezable.
 * Check if video can be freezed or not.
 * @param  bPath
 * @retval SV_TRUE if video can be freezed.
 *         SV_FALSE if video can't be freezed.
 */
//-----------------------------------------------------------------------------
EXTERN UINT8 fgScposIsVideoFreezable(UINT8 bPath);

//-----------------------------------------------------------------------------
/** Brief of u4ScposGetInOutVDistance.
 * Get distance between in and out V sync.
 * @param  void
 * @retval in/out v distance
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 u4ScposGetInOutVDistance(void);

//-----------------------------------------------------------------------------
/** bScalerModeChangeDone
 *  For pscan to get mode change done information.
 *  bPath Path information.
 */
//-----------------------------------------------------------------------------
EXTERN UINT8 bScalerModeChangeDone(UINT8 bPath);

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

EXTERN void vScpipConfigScalingMode(UINT32 u4VdpId);

//-----------------------------------------------------------------------------
/** Brief of _VDP_NotifyPlaneOrderChanged.
 * Notify VDP that plane order changed.
 * @param u4VdpId video plane ID 0~1
 * @param u4PlaneOrder plane order 0~3
 * @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void _VDP_NotifyPlaneOrderChanged(UINT32 u4VdpId, UINT32 u4PlaneOrder);

//-----------------------------------------------------------------------------
/** Brief of fgScposIsUpscalerEnabled.
 * API to query if Up scaler is enabled by direction (H or V)
 * @param bPath video path
 * @param direction H or V direction
 * @retval SV_TRUE means upscaler is enabled
           SV_FALSE mean upscaler is disabled
 */
//-----------------------------------------------------------------------------
EXTERN UINT8 fgScposIsUpscalerEnabled(UINT8 bPath,E_DIRECTION direction);

//-----------------------------------------------------------------------------
/** Brief of u4DrvGetScalarFrameDelay.
 * Get Scalar Frame Delay.
 * @param u4VdpId video plane ID 0~1
 * @retval Frame Delay
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 u4DrvGetScalarFrameDelay(UINT32 u4VdpId);

//-----------------------------------------------------------------------------
/** Brief of _VDP_GetMinSrcRegionWidth.
 * Get Scalar minimun source region width.
 * @param u4VdpId video plane ID 0~1
 * @retval Frame Delay
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 _VDP_GetMinSrcRegionWidth(UINT8 u4VdpId);


EXTERN UINT8 bGetScalerFrameNum(UINT8 bPath);


#ifdef CC_TV_MEMO_MODE
EXTERN void _VDP_SetMemoMode(BOOL fgEnable);
EXTERN BOOL VDP_GetMemoMode(void);
#endif

EXTERN UINT32 wDrvVideoGetPreScaleDEWidth(UINT8 bPath); 


//-----------------------------------------------------------------------------
/** VDP scaler control call back
 *
 * @return void
 */
//-----------------------------------------------------------------------------
EXTERN void vDrvScpipSrmDispCb(UINT32 u4VdpId, UINT32 u4Mode, UINT32 u4PDSize, UINT32 u4SramMode);


//-----------------------------------------------------------------------------
/**
 * @brief vDrvSetScalarFreeze 
 * Freeze Video in scalar
 * @param  bPath: SV_VP_MAIN/SV_VP_PIP
 *         bOnOff: SV_ON/SV_OFF
 * @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void vDrvSetScalarFreeze(UINT8 bPath, UINT8 bOnOff) ;

//-----------------------------------------------------------------------------
/**
 * @brief u4ScalerGetHPS
 * Get pre-scaling down factor
 * @param  bPath: SV_VP_MAIN/SV_VP_PIP
 * @retval void
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 u4ScalerGetHPS(UINT8 bPath);


//-----------------------------------------------------------------------------
/**
 * @brief vDrvScpipForce2PSDivClock
 * Force scpip input clock to PS_DIV_CLOCK
 * @param  u4VdpId: video plane id
 * @param  u4OnOff: turn on/off
 * @param  u4N: denominator of PS_DIV_CLOCK
 * @param  u4M: numerator of PS_DIV_CLOCK
 * @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void vDrvScpipForce2PSDivClock(UINT32 u4VdpId, UINT32 u4OnOff, UINT32 u4N, UINT32 u4M);

//-----------------------------------------------------------------------------
/**
 * @brief vScpipSrmSetBuf
 */
//-----------------------------------------------------------------------------
EXTERN void vScpipSrmSetBuf(UINT32 u4VdpId, const SCALER_FBM_POOL_T * prFbmPool); 


//-----------------------------------------------------------------------------
/**
 * @brief vDrvSetDSPattern
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
EXTERN void vDrvSetDSPattern(UINT32 u4VdpId, UINT32 u4OnOff, UINT32 u4HTotal, UINT32 u4Width, UINT32 u4VTotal, UINT32 u4Height);

//-----------------------------------------------------------------------------
/**
 * @brief vDrvSetUSPattern
 * Set US inner pattern generator
 * @param  u4VdpId: video path
 * @param  u4OnOff: VDP_US_PTN_OFF, VDP_US_PTN_AUTO, and VDP_US_PTN_MANUAL
 * @param  u4Width: video active width
 * @param  u4Height: video active height
 * @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void vDrvSetUSPattern(UINT32 u4VdpId, UINT32 u4OnOff, UINT32 u4Width, UINT32 u4Height);


EXTERN UINT32 u4ScpipGetBufNum(UINT32 u4VdpId);

EXTERN void vDrvScpipToggleForceOff(UINT32 u4VdpId);

EXTERN void vScpipOnInputVSync(UINT8 bPath);

EXTERN UINT32 u4ScpipSRMGetFrameDelay(UINT32 u4VdpId);

EXTERN UINT16 wScpipGetDispWidth(UINT8 bPath);
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
EXTERN UINT8 bScpipPScanDispMirrorWA(UINT8 bPath, UINT8 bOnOff);
#endif

#if SUPPORT_DOT_TO_DOT_PATTERN
EXTERN void VDP_DotToDotPattern(UINT8 ucOnOff,UINT32 u4RGBColor);
#endif

// For pscan display mode hlen update
EXTERN void vScpipSetPscanDispmodeHLen(UINT8 bPath);
#if defined(CC_MT5363)
EXTERN void vScpipGetDramInf(UINT8 bPath,DRAM_INFO_T* inf);
#endif

#endif
