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
 * $RCSfile: vdp_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_if.h
 *  Video plane interface, including init, enable,
 *  mode selection and region desciption.
 */

#ifndef VDP_IF_H
#define VDP_IF_H

#include "x_typedef.h"
#include "stc_drvif.h"
#include "feature.h"

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

#define VDP_MAX_NS              	2    	///< Maximum number of video plane.
#define VDP_NS                      2


// Value of parameter ucVdpId.
#define VDP_1                       0    	///< VDP1
#define VDP_2                       1    	///< VDP2
#define VDP_3                       2    	///< OSD1, obsolete
#define VDP_4                       3    	///< OSD2, obsolete
#define VDP_5                       4    	///< OSD3, obsolete

#define VDP_MAX_INPORT_NS           1    	///< Maximum number of input port for each video plane.


// Video plane configuration return value.
#define VDP_SET_ERROR               0    	///< return value
#define VDP_SET_OK                  1		///< return value

// Trick Play at 20081001
#define VDP_PLAY_NORMAL 0 ///< Normal Play
#define VDP_PLAY_TRICK 1 ///< Trick Play
#define VDP_PLAY_STEP_FORWARD 2 ///< Step Forward

// Lip Sync OK/NG status change notification
#define VDP_LIPSYNC_REG_NG_TO_OK 0
#define VDP_LIPSYNC_REG_OK_TO_NG 1

#define VERIFY_NULL(ptr)                                    \
          do {                                              \
            if (ptr == NULL) { return VDP_SET_ERROR; }      \
          } while (0)

#define VDP_CHECK_CB_FUNC_VERIFY(func, crc)     ((UINT32)func == ~((UINT32)crc))

#define VDP_MAX_REGION_WIDTH 10000
#define VDP_MAX_REGION_HEIGHT 10000

// Video plane mode.
#define VDP_MODE_NORMAL             0		///< normal mode, video out
#define VDP_MODE_BG                 1		///< background mode, mute
#define VDP_MODE_QV                 2		///< obsolete
#define VDP_MODE_DEINT              3		///< obsolete
#define VDP_MODE_UNKNOWN            4

// Fixed PreScaling Down
#ifndef CC_VDP_FIXED_PRESCALER
#define CC_VDP_FIXED_PRESCALER       1
#endif

/**
 *  Video plane region (source/output).
 */
typedef struct
{
    UINT32 u4X;        	///< start position in x direction (u4X/1000 = start pixel/total pixel)
    UINT32 u4Y;        	///< start position in y direction (u4Y/1000 = start pixel/total pixel)
    UINT32 u4Width;  	///< width in percentage (u4Width/1000 = region width/total width)
    UINT32 u4Height; 	///< height in percentage (u4Width/1000 = region height/total width)
} VDP_REGION_T;

/**
 *  Video plane overscan region.
 */
typedef struct
{
    UINT32 u4Top;      	///<overscan percentage on top side (u4Top%)
    UINT32 u4Bottom;	///<overscan percentage on top side (u4Bottom%)
    UINT32 u4Left;     	///<overscan percentage on top side (u4Left%)
    UINT32 u4Right;   	///<overscan percentage on top side (u4Right%)
} VDP_OVERSCAN_REGION_T;

/**
 *  Video source change information.
 */
typedef struct
{
    UINT32 u4SrcWidth;              ///< new source width
    UINT32 u4SrcHeight;  	        ///< new source height
    UINT32 u4OutWidth; 	        ///< new output width
    UINT32 u4OutHeight; 	        ///< new output height  
    UINT32 u4FrameRate;            ///< new frame rate
    UINT32 u4Interlace;             ///< new scan mode
} VDP_SOURCE_INFO_T;

/* Thumbnail Color mode */
typedef enum
{
    VDP_COLORMODE_AYUV_CLUT2     = 0,
    VDP_COLORMODE_AYUV_CLUT4     = 1,
    VDP_COLORMODE_AYUV_CLUT8     = 2,
    VDP_COLORMODE_UYVY_16        = 3,
    VDP_COLORMODE_YUYV_16        = 4,
    VDP_COLORMODE_AYUV_D8888     = 5,
    VDP_COLORMODE_ARGB_CLUT2     = 6,
    VDP_COLORMODE_ARGB_CLUT4     = 7,
    VDP_COLORMODE_ARGB_CLUT8     = 8,
    VDP_COLORMODE_RGB_D565       = 9,
    VDP_COLORMODE_ARGB_D1555     = 10,
    VDP_COLORMODE_ARGB_D4444     = 11,
    VDP_COLORMODE_ARGB_D8888     = 12,
    VDP_COLORMODE_YUV_420_BLK    = 13,
    VDP_COLORMODE_YUV_420_RS     = 14,
    VDP_COLORMODE_YUV_422_BLK    = 15,
    VDP_COLORMODE_YUV_422_RS     = 16,
    VDP_COLORMODE_YUV_444_BLK    = 17,
    VDP_COLORMODE_YUV_444_RS     = 18
}VDP_COLORMODE_T;


/* Thumbnail Info */
typedef struct
{
    UCHAR*                    pucCanvasBuffer;
    VDP_COLORMODE_T           eCanvasColormode;
    UINT32                    u4CanvasWidth;     /* in pixels */
    UINT32                    u4CanvasHeight;    /* in pixels */
    UINT32                    u4CanvasPitch;     /* in bytes */

    UINT32                    u4ThumbnailX;      /* in pixels */
    UINT32                    u4ThumbnailY;      /* in pixels */
    UINT32                    u4ThumbnailWidth;  /* in pixels */
    UINT32                    u4ThumbnailHeight; /* in pixels */
}VDP_THUMBNAIL_INFO_T;


/**
 *  Source Change Notify Function Type
 */
typedef UINT32 (*PFN_VDP_SRC_CHG_NOTIFY)(UCHAR u4VdpId, VDP_SOURCE_INFO_T rVdpSrcInfo);
/**
 *  Scaler display mode support Notify Function Type
 */
typedef UINT32 (*PFN_VDP_DISP_SUPPORT_NOTIFY)(UCHAR ucVdpId, UCHAR ucStatus);


/**
 *  Video plane call back function type id.
 */
typedef enum
{
    VDP_CB_FUNC_SRC_CHG_IND,        ///< Source Change Notify
    VDP_CB_FUNC_DISP_SUPPORT_IND,   //Scaler display mode not support notify
    VDP_CB_FUNC_PTS_IND,            ///< PTS Reach Notify
    VDP_CB_FUNC_AFD_IND,            ///< AFD/Aspect Ratio Change Notify
    VDP_CB_FUNC_UNMUTE_IND,         ///< UnMute Status Change Notify
    VDP_CB_FUNC_LIPSYNC_IND,        ///< Lip Sync Norify     
    VDP_CB_FUNC_MM_COND_IND,        ///< Vdp Condition Notify for MM
    VDP_CB_FUNC_MM_STEP_FIN_IND,        ///< Vdp Step Forward finish for MM
    VDP_CB_FUNC_MM_SEEK_FIN_IND,        ///< Vdp Seek finish for MM
    VDP_CB_FUNC_REPEAT_DROP_IND,        ///< Vdp repeat or drop
    VDP_CB_FUNC_LIPSYNC_OK_TO_NG_IND,   //< Vdp Lip Sync OK to NG notify
    VDP_CB_FUNC_LIPSYNC_NG_TO_OK_IND,   //< Vdp Lip Sync NG to OK notify        
    VDP_CB_FUNC_SRC_CHG_IND_2,
    VDP_CB_FUNC_AFD_IND_2,
	VDP_CB_FUNC_PLAY_DONE_IND,      ///< Vdp Play Done Notify
    VDP_CB_FUNC_NS
} VDP_CB_FUNC_ENUM_T;

/** Brief of VDP_NONLINEAR_PRM_T
 *  Video plane nonlinear scaler factors.
 */
typedef struct
{
    UINT32 u4Start;
    UINT32 u4End;
    UINT32 u4Slope;
    UINT32 u4MiddlePoint;
} VDP_NONLINEAR_PRM_T;

#if defined(CC_MT5365) || defined(CC_MT5395)
/** Brief of VDP_NONLINEAR_PRM_T
 *  Video plane nonlinear scaler factors.
 */
typedef struct
{
    //UCHAR ucUpDown;             // Linear region up scale or down scale   
    UINT32 u4SrcMiddleRatio;         // Source linear scaling region width in ratio from 0~10000
    UINT32 u4OutMiddleRatio;         // Output linear scaling region width in ratio from 0~10000
} VDP_NEW_NONLINEAR_PRM_T;

/** Brief of VDP_NEW_NONLINEAR_REG_T
 *  Video plane nonlinear scaler factors.
 */
typedef struct
{
    UINT32 u4StartFactor;   // Non-linear up scaling start factor
    UINT32 u4StepFactor;    // Non-linear up scaling step factor
    UINT32 u4MiddlePoint;   // Non-linear up scaling middle point
    //UINT8  u1Neg;           // Negative non-linear up scaling 
    UINT8  u1Step;          // Non-linear up scaling step width for each increment
    UINT32 u4StopPoint;     // Non-linear up scaling stop point
} VDP_NEW_NONLINEAR_REG_T;
#else
typedef struct
{
  UINT32 u4EndFactor;
  UINT32 u4StopPoint;
  UINT32 u4StartFactor;
  UINT32 u4IncreaseFactor;
  UINT32 u4SigWdith;
  UINT32 u4MiddleWidth;
} VDP_NEW_NONLINEAR_PRM_T;
#endif

/** Brief of VDP_AUTO_NONLINEAR_PRM_T
 *  Video plane nonlinear scaler factors in auto mode.
 */
typedef struct
{
    UINT32 u4Adaptive;
    UINT32 u4Slope;
    UINT32 u4EndFactor;
    UINT8 u1Mode;  
} VDP_AUTO_NONLINEAR_PRM_T;

/**
 * Video plane call back function pool
 */
typedef struct
{
    UINT32 au4CbFunc[VDP_CB_FUNC_NS];
    UINT32 au4CbFuncCRC[VDP_CB_FUNC_NS];
} VDP_CB_FUNC_T;


// Video Image Module Color Mode.
#define VDP_IMAGE_420_MODE          (1 << 0)		///< 420 mode
#define VDP_IMAGE_422_MODE          (1 << 1)		///< 422 mode
#define VDP_IMAGE_444_MODE          (1 << 2)		///< 424 mode

///-----------------------------------------------------------------------------
/// Public functions
///-----------------------------------------------------------------------------

/**
 * VDP init function.
 *
 * @param void
 * @return void
 */
EXTERN void VDP_Init(void);

/**
 * VDP stop function.
 *
 * @param void
 * @return void
 */
EXTERN void VDP_Stop(void);

/**
 * VDP reset function.
 *
 * @param ucVdpId specify the video plane id.
 * @return void
 */
EXTERN void VDP_Reset(UCHAR ucVdpId);

/**
 * VDP query function, show status in CLI with debug level 1.
 *
 * @param void
 * @return void
 */
EXTERN void VDP_QueryStatus(void);

/**
 * VDP query function, show DTV status in CLI with debug level 1.
 *
 * @param void
 * @return void
 */
EXTERN void VDP_PrintDtvInfo(void);

/**
 * VDP set video plane enable.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucEnable value 0:disable, 1:enable.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_SetEnable(UCHAR ucVdpId, UCHAR ucEnable);

/**
 * VDP set video plane mode.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucMode VDP_MODE_NORMAL or VDP_MODE_BG.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_SetMode(UCHAR ucVdpId, UCHAR ucMode);

/**
 * VDP set video plane with freeze feature.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucFreeze value 0:disable, 1:freeze feature enable.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_SetFreeze(UCHAR ucVdpId, UCHAR ucFreeze);

/**
 * VDP query if nonlinear is supported.
 *
 * @param ucVdpId specify the video plane id.
 * @return TRUE/FALSE
 */
EXTERN BOOL VDP_IsSupportNonlinear(UCHAR ucVdpId);

/**
 * VDP query if dot-by-dot is supported.
 *
 * @param ucVdpId specify the video plane id.
 * @return TRUE/FALSE
 */
EXTERN UINT32 VDP_IsSupportDotByDot(void);


/**
 * VDP query if overscan is supported.
 *
 * @param ucVdpId specify the video plane id.
 * @return TRUE/FALSE
 */
EXTERN BOOL VDP_IsSupportOverscan(UCHAR ucVdpId);


/**
 * VDP set overscan region.
 *
 * @param ucVdpId specify the video plane id.
 * @param rRegion overscan percentage setting.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_SetOverScan(UCHAR ucVdpId, VDP_OVERSCAN_REGION_T rRegion);

/**
 * VDP set source region.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucSrcFullRegion value 1:full region and ignore parameter rSrcRegion.
 * @param rSrcRegion source region description.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_SetSrcRegion(UCHAR ucVdpId, UCHAR ucSrcFullRegion, VDP_REGION_T rSrcRegion);

/**
 * VDP set output region.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucOutFullRegion value 1:full region and ignore parameter rOutRegion.
 * @param rOutRegion output region description.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_SetOutRegion(UCHAR ucVdpId, UCHAR ucOutFullRegion, VDP_REGION_T rOutRegion);

/**
 * VDP set background color.
 *
 * @param ucVdpId specify the video plane id.
 * @param u4BgColor specify the color in RGB color mode, 1 byte per color component.(e.g. Red: 0xFF0000).
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_SetBg(UCHAR ucVdpId, UINT32 u4BgColor);

/**
 * VDP query video plane enable/disable status.
 *
 * @param ucVdpId specify the video plane id.
 * @param pucEnable value 0:disable, 1:enable.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_GetEnable(UCHAR ucVdpId, UCHAR* pucEnable);

/**
 * VDP query video plane mode.
 *
 * @param ucVdpId specify the video plane id.
 * @param pucMode VDP_MODE_NORMAL or VDP_MODE_BG.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_GetMode(UCHAR ucVdpId, UCHAR* pucMode);

/**
 * VDP query video plane with freeze feature.
 *
 * @param ucVdpId specify the video plane id.
 * @param pucFreeze value 0:disable, 1:freeze feature enable.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_GetFreeze(UCHAR ucVdpId, UCHAR* pucFreeze);

/**
 * VDP query video plane with freeze feature.
 *
 * @param ucVdpId specify the video plane id.
 * @param pucFreezable value 0: can't freeze, 1:freeze feature is available.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_GetFreezable(UCHAR ucVdpId, UCHAR* pucFreezable);

/**
 * VDP query overscan region.
 *
 * @param ucVdpId specify the video plane id.
 * @param prRegion overscan percentage setting.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_GetOverScan(UCHAR ucVdpId, VDP_OVERSCAN_REGION_T* prRegion);

/**
 * VDP query source region.
 *
 * @param ucVdpId specify the video plane id.
 * @param pucSrcFullRegion value 1:full region and ignore parameter rSrcRegion.
 * @param prSrcRegion source region description.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_GetSrcRegion(UCHAR ucVdpId, UCHAR* pucSrcFullRegion, VDP_REGION_T* prSrcRegion);

/**
 * VDP query output region.
 *
 * @param ucVdpId specify the video plane id.
 * @param pucOutFullRegion value 1:full region and ignore parameter rOutRegion.
 * @param prOutRegion output region description.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_GetOutRegion(UCHAR ucVdpId, UCHAR* pucOutFullRegion, VDP_REGION_T* prOutRegion);

/**
 * VDP query background color.
 *
 * @param ucVdpId specify the video plane id.
 * @param pu4BgColor the color in RGB color mode, 1 byte per color component.(e.g. Red: 0xFF0000).
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_GetBg(UCHAR ucVdpId, UINT32* pu4BgColor);

/**
 * Video plane call back function register
 *
 * @param eFuncType specify the call back function type.
 * @param u4FuncPtr specify the call back function pointer.
 * @param u4Arg1 specify argument #1.
 * @param u4Arg2 specify argument #2.
 * @return void
 */
EXTERN void VDP_RegCbFunc(VDP_CB_FUNC_ENUM_T eFuncType, UINT32 u4FuncPtr, UINT32 u4Arg1, UINT32 u4Arg2);

/**
 * VDP set scaler to pixel based or percentage based.
 *
 * @param u4OnOff = 0 refer to scaler API change to percentage based. (Default)
 * @param u4OnOff = 1 refer to scaler API change to pixel based.
 * @return void
 */
EXTERN void VDP_SetScalerPixelBased(UINT32 u4OnOff);


/**
 * VDP Get scaler to pixel based or percentage based.
 *
 * @param void
 * @return 1 refer to scaler API change to pixel based.
 */
EXTERN UINT32 VDP_GetScalerPixelBased(void);


/**
 * VDP set SCPOS porch.
 *
 * @param u4VdpId specify the video plane id.
 * @param bPorchType SV_VPORCH_CURRENT/SV_HPORCH_CURRENT
 * @param wValue porch value of 1000-based.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_SetScposPorch(UINT32 u4VdpId, UINT8 bPorchType, UINT16 wValue);

/**
 * VDP set game mode on/off
 *
 * @param u4VdpId specify the video plane id.
 * @param u1OnOff enable/disable game mode.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_SetGameMode(UINT32 u4VdpId, UINT8 u1OnOff);

//mtk01140 20071010 3D game mode
/**
 * VDP set 3D game mode on/off
 *
 * @param u4VdpId specify the video plane id.
 * @param u1OnOff enable/disable 3D game mode.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_Set3DGameMode(UINT32 u4VdpId, UINT8 u1OnOff);


/**
 * Get delay time of PQ
 *
 * @return delay time in 90000
 */
EXTERN UINT32 VDP_GetDelayTime(void);

/**
 * VDP set scaler DRAM 
 *
 * @param u4VdpId specify the video plane id.
 * @param u4X x offset to fill DRAM data
 * @param u4Y y offset to fill DRAM data
 * @param u4Width pixel width to fill DRAM data
 * @param u4Height pixel height to fill DRAM data
 * @param u4GBRYCbCr pixel data to fill DRAM
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_SetScalerDRAM(UINT32 u4VdpId, UINT32 u4X, UINT32 u4Y, UINT32 u4Width, UINT32 u4Height, UINT32 u4GBRYCbCr);
EXTERN UINT32 VDP_SetScalerPattern(UINT32 u4VdpId, UINT32 u4OnOff);


EXTERN UINT8 VDP_GetDIFilm32Enable(UCHAR ucVdpId);

EXTERN UINT32 VDP_SetNoOverScan(UCHAR ucVdpId, UCHAR ucNoOverScan);

EXTERN UINT32 VDP_LBoxDetectMode(UCHAR ucVdpId, UINT32 u4DetectPeriod, UINT32 Ratio);

EXTERN void VDP_GetActiveImageArea(UINT16 *x, UINT16 *y, UINT16 *w, UINT16 *h);


#define BLANK_FRAMERATE_CHANGED  (1)
#define BLANK_RESOLUTION_CHANGED (1<<1)

EXTERN void VDP_SetBlankedReason(UINT32 u4BlankedReason, BOOL fgOnOff);
EXTERN void VDP_SetPlanesBlank(UINT32 u4BlankedReason);
EXTERN void VDP_SetPlanesUnblank(void);

EXTERN UINT32 VDP_GetMinSrcRegionWidth(void);
#if SUPPORT_DOT_TO_DOT_PATTERN
EXTERN void VDP_DotToDotPattern(UINT8 ucOnOff,UINT32 u4RGBColor);
#endif

/**
 * VDP set force 2D mode
 * @param  ucVdpId    
 * @param  ucMode    0: DRAM mode, 1: Scaler display mode, 2: n/a, 3: pscan display mode, 4: mjc display mode
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_SetDispmode(UCHAR ucVdpId, UCHAR ucMode);
EXTERN UINT32 VDP_GetDispmode(UCHAR ucVdpId);
EXTERN UINT32 VDP_SetScaler10BitMode(UINT32 u4VdpId, UINT32 u4Mode);
EXTERN UINT32 VDP_GetScaler10BitMode(UINT32 u4VdpId);

EXTERN UINT32 VDP_UpdateCmdDelay(UINT32 u4VdpId);
EXTERN UINT32 VDP_GetCmdDelay(void);
EXTERN UINT32 VDP_GetPanelType(void);
EXTERN void VDP_FrcQueryStatus(void);

// freez in 8283 when main case
#ifdef DRV_SUPPORT_EXTMJC

EXTERN INT32 d_custom_extmjc_set_mjc_frz(
    UINT8                                   ui1_fgfrz,
    UINT8*                                  pui1_msg_error
    );

#define vMJCSetMT8283FRZOnOff(A)        d_custom_extmjc_set_mjc_frz(A, NULL)
#endif



#endif    // VDP_IF_H

