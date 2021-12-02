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
 * $RCSfile: vdp_drvif.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_drvif.h
 *  Brief of file vdp_drvif.h.
 *  Details of file vdp_drvif.h (optional).
 */

#ifndef VDP_DRVIF_H
#define VDP_DRVIF_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "vdp_if.h"
#include "fbm_drvif.h"
#include "pmx_if.h"
#include "x_os.h"
#include "x_hal_5381.h"


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

// Video plane capability
#define VDP_CAP_OUTPUT_AUX          (1 << 0)
#define VDP_CAP_QV                  (1 << 1)
#define VDP_CAP_DEINT               (1 << 2)
#define VDP_CAP_ENHANCE             (1 << 3)
#define VDP_CAP_420_CM              (1 << 4)
#define VDP_CAP_32BIT_CM            (1 << 5)

/* video cinema mode */
#define VDP_SV_CN_AUTO              0
#define VDP_SV_CN_OFF               1
#define VDP_SV_CN_MOVIE             2
#define VDP_SV_CN_MAX               3

/* video demo mode */
typedef enum 
{
    VDP_PQ_DEMO_OFF = 0,             
    VDP_PQ_DEMO_ON_RIGHT,            
    VDP_PQ_DEMO_ON_LEFT,
    VDP_PQ_DEMO_STILL_REC,
    VDP_PQ_DEMO_MOVING_REC,
    VDP_PQ_DEMO_MODE_NUM
} VDP_DEMO_MODE;

/* video di mode */
#define VDP_DI_AUTO_MODE            0
#define VDP_DI_MODE_1               1
#define VDP_DI_MODE_2               2
#define VDP_DI_MODE_3               3
#define VDP_DI_MODE_4               4
#define VDP_DI_MODE_MAX             5

/* video edge mode */
#define VDP_SV_V_EDGE_OFF           0
#define VDP_SV_V_EDGE_LOW           1
#define VDP_SV_V_EDGE_HIGH          2
#define VDP_SV_V_EDGE_MAX           3

// Video Plane Call Back Error
#define VDP_CB_ERR                  (0x80000000)

// Display Mode
#define VDP_SCPOS_DISPMODE_OFF    0
#define VDP_SCPOS_DISPMODE_AUTO    1
#define VDP_SCPOS_DISPMODE_MANUAL   2
#define VDP_SCPOS_PSCAN_DISPMODE   3
#define VDP_SCPOS_MJC_DISPMODE   4


// 10 Bit mode
#define VDP_SCPOS_10BitMode  10
#define VDP_SCPOS_8BitMode  8


/** Brief of EVDP_VIDEO_INPUT_SOURCE_T
 *  Video Input Source (VIS) type
 */
typedef enum _EVDP_VIDEO_INPUT_SOURCE_T
{
    EVDP_VIS_AVC_COMP_VIDEO = 0,
    EVDP_VIS_AVC_S_VIDEO,
    EVDP_VIS_AVC_Y_PB_PR,
    EVDP_VIS_AVC_VGA,
    EVDP_VIS_AVC_SCART,
    EVDP_VIS_AVC_DVI,
    EVDP_VIS_AVC_HDMI,
    EVDP_VIS_TUNER_DIGITAL,
    EVDP_VIS_TUNER_ANALOG,
    
    EVDP_VIS_MAX
} EVDP_VIDEO_INPUT_SOURCE_T;


/** Brief of VDP_COLOR_GAIN_T
 *  Video plane color gain.
 */
typedef struct
{
    UINT8 ucAuto;
    UINT16 ucRgain;
    UINT16 ucGgain;
    UINT16 ucBgain;
} VDP_COLOR_GAIN_T;

/** Brief of VDP_OUT_INFO_T
 *  Video plane output parameter.
 */
typedef struct
{
    UCHAR ucPmxId;
    UCHAR ucPmxMode;
    UCHAR ucPrg;
    UCHAR ucFrameRate;
    UCHAR ucDoubleClk;
    UCHAR ucInputFrameRate;     // For Clock Configuration
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4TotalWidth;
    UINT32 u4TotalHeight;
    UINT32 u4ClkMode;
} VDP_OUT_INFO_T;

/** Brief of VDP_CONF_T
 *  Video plane configuration.
 */
typedef struct
{
    UCHAR ucVdpEnable;
    UCHAR ucEnable;
    UCHAR ucB2rId;
    UCHAR ucStatus;

    UCHAR ucMode;
    UCHAR ucFreeze;
    UCHAR ucNonlinear;
    UCHAR ucNonlinearScope;
    UINT32 u4NonlinarEndFactor;

 //   UCHAR ucDispMode;
    UCHAR ucBobMode;

    // Input port (frame buffer group ID)
    UCHAR ucInputPort[VDP_MAX_INPORT_NS];
    UCHAR ucMasterPort;

    //UCHAR ucPauseMm;
    UCHAR ucNoOverScan;
    UCHAR ucEarlyDisp;

    UCHAR ucColorDescrip;
    UCHAR ucColourPrimary;                    
    UCHAR ucTransCharacter;                    
    UCHAR ucMatrixCoeff;                    
    
    // (source/output) region
    VDP_REGION_T rSrcRegion;
    VDP_REGION_T rOutRegion;
    VDP_OUT_INFO_T rOutInfo;
    VDP_OVERSCAN_REGION_T rOverScan;

    // output max width/height (MT5371 Limitation)
    UINT32 u4MaxOutWidth;
    UINT32 u4MaxOutHeight;

    // source information
    UINT32 u4SrcLineSize;
    UINT32 u4SrcWidth;
    UINT32 u4SrcHeight;
    UINT32 u4AspectRatio;
    UINT32 u4FrameRate;
    UINT32 u4QpAvg;
    UINT32 u4Afd;

    // hardware debug
    UCHAR ucColorMode;
    UCHAR ucAcsFrame;
    UCHAR ucSrcFrame;
    UCHAR ucAcsAuto;
    UCHAR ucPhyTop;
    UCHAR ucAcsTop;
    UINT32 u4AddrY;
    UINT32 u4AddrC;

    UINT32 u4BgColor;            // YCbCr

    UINT32 u4MpegWidth;
    UINT32 u4MpegHeight;

    UINT32 u4FrmCropTop;
    UINT32 u4FrmCropBtm;


    HANDLE_T hMutex;
    
    UINT32 u4AttachedSrcId; // For playmgr usge while calling back to it
} VDP_CONF_T;

typedef struct _B2R_DISP_INFO_T
{
	UINT8 dram_from;

    UINT8  u1OutMode;          // 0 Progress mode;1 interlace mode
    UINT8  u1BlockMode;        // 0:64x32 ; 1:16x32 ; others: raster mode

	UINT32 u4Pitch;            // Line Size of buffer
    UINT32 u4YStart;           // Y address
    UINT32 u4CStart;           // C address
    UINT32 u4With;             // active  width
    UINT32 u4Height;           // active  height
} B2R_DISP_INFO_T;

typedef struct _VDP_SC_DATA_INFO
{
    UINT32 u4LineStart;
    UINT32 u4LineCount;
    UINT8 bYMax;
    UINT8 bYMin;
    UINT8 bUMax;
    UINT8 bUMin;
    UINT8 bVMax;
    UINT8 bVMin;
} VDP_SC_DATA_INFO;


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

EXTERN UINT32 VDP_SetNonlinear(UCHAR ucVdpId, UCHAR ucNonlinear);

EXTERN UINT32 VDP_SetNonlinearScope(UCHAR ucVdpId, UCHAR ucNonlinear);

EXTERN UINT32 VDP_SetNonlinearFactor(UCHAR ucVdpId, UCHAR ucScpoe, UINT32 u4Factor);

EXTERN UINT32 VDP_GetNonlinearFactor(UCHAR ucVdpId, UCHAR *pucScpoe, UINT32 *pu4Factor);

EXTERN UINT32 VDP_SetManualNonLinear(UCHAR ucVdpId, VDP_NONLINEAR_PRM_T rNonlinearPrm);

EXTERN UINT32 VDP_SetAutoNonLinear(UCHAR ucVdpId, VDP_AUTO_NONLINEAR_PRM_T rAutoNonlinearPrm);
#if defined(CC_MT5365) || defined(CC_MT5395)
EXTERN UINT32 VDP_SetNewNonLinear(UCHAR ucVdpId, VDP_NEW_NONLINEAR_PRM_T rNonlPrm);
#endif
EXTERN UINT32 VDP_Set121Map(UCHAR ucVdpId, UCHAR ucEn);
EXTERN UINT32 VDP_Get121Map(UCHAR ucVdpId);

EXTERN UINT32 VDP_SetDynamiceScaler(UCHAR ucVdpId, UCHAR ucOnOff, UINT32 u4ResponseTime);

EXTERN UINT32 VDP_SetLBoxDetection(UCHAR ucVdpId, UCHAR ucOnOff, UINT8 bFixed);

EXTERN UINT32 VDP_SetSrcSize(UCHAR ucVdpId, UINT32 u4SrcWidth, UINT32 u4SrcHeight);        // for CLI test

EXTERN void VDP_ScposDispmodeFreeRun(BOOL bEnable);

EXTERN UINT32 VDP_ScposDispModeFIFOOverflow(void);

EXTERN UINT32 VDP_GetNonlinear(UCHAR ucVdpId, UCHAR* pucNonlinear);

EXTERN UINT32 VDP_GetNonlinearScope(UCHAR ucVdpId, UCHAR* pucNonlinear);

EXTERN UINT32 VDP_SetNonLinear_Cust(UCHAR ucVdpId, BOOL bAuto, UINT32 u4StartFactor, UINT32 u4Slope,  UINT32 u4MidPoint,UINT32 u4StopFactor);

EXTERN UINT32 VDP_ForceWriteOff(UINT32 u4VdpId, UINT8 u1OnOff);

EXTERN UINT32 VDP_GetSrcSize(UCHAR ucVdpId, UINT32* pu4SrcLineSize, UINT32* pu4SrcWidth, UINT32* pu4SrcHeight);

EXTERN VDP_CONF_T* VDP_GetConf(UCHAR ucVdpId);
#ifdef CC_B2R_SKYPE_SUPPORT
EXTERN void VDP_HalGetB2rDispInfo(UCHAR ucVdpId,B2R_DISP_INFO_T* ptB2rInfo);
#else
EXTERN void VDP_HalGetB2rDispInfo(B2R_DISP_INFO_T* ptB2rInfo);
#endif

EXTERN UINT32 VDP_DispStatusNotify(UINT8 ucVdpId, UINT8 bStatus);

#if 1 //5387	
EXTERN UINT32 VDP_SourceChangeNotify(UINT8 bPath);
#else
EXTERN UINT32 VDP_SourceChangeNotify(UCHAR ucVdpId, VDP_SOURCE_INFO_T rVdpSrcInfo);
#endif

EXTERN void vDrvDISetPiexelMotionThres(UINT8 u1VdpId);

EXTERN void vDrvDICombMeasure(UINT8 u1VdpId);

EXTERN void _VDP_vDrvDITrickModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff);

EXTERN void _VDP_vDrvDIFilmModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff);

EXTERN UINT32 VDP_SendSCPOSOutsizeSRMEvent(UCHAR ucVdpId);

//EXTERN void VDP_SpecialVGA(UCHAR ucEnable);

EXTERN void VDP_MutexLock(void);

EXTERN void VDP_MutexUnlock(void);

EXTERN HANDLE_T VDP_GetMutex(void);

#ifdef DIVX_PLUS_CER
#ifdef CC_B2R_CHAPTER_CHG
EXTERN BOOL VDP_SetChpterEnable(VDP_CHAPTER_T*    prChapter);
#endif
#endif


//-----------------------------------------------------------------------------
/**
 * @brief u4ScpipForceDispOff , currently for TVE use
 * @param  u4VdpId: video path
 * @param  u4OnOff: SV_TRUE(Display off), SV_FALSE (Display Normal)
 * @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void VDP_SetDisplayOff(UINT32 u4Path, UINT32 u4OnOff);

#if 0 // def CC_MT5392B
EXTERN void vDrvMJCDynOnOff(UINT8 bOnOff);
extern UINT8 bDrvMJCGetFrameDelay(void);
#endif

EXTERN void VDP_XdataInit(void);
EXTERN void VDP_XdataSetSrcRegion(UINT32 u4VdpId, VDP_REGION_T rSrcRegion);
EXTERN void VDP_XdataSetOutRegion(UINT32 u4VdpId, VDP_REGION_T rOutRegion);
EXTERN void VDP_XdataSetOverscan(UINT32 u4VdpId, VDP_OVERSCAN_REGION_T rOverscanRegion);
EXTERN void VDP_XdataSetDispmode(UINT32 u4VdpId, UINT32 u4Mode);
EXTERN void VDP_XdataSetScaler10Bit(UINT32 u4VdpId, UINT32 u4Mode);
EXTERN void VDP_XdataSet121Map(UCHAR ucVdpId, UINT32 ucEn);
EXTERN void VDP_XdataSetNewNonlEn(UCHAR ucVdpId, UINT32 ucEn);
EXTERN void VDP_XdataSetNewNonlPrm(UCHAR ucVdpId, VDP_NEW_NONLINEAR_PRM_T rNonlPrm);

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/** Brief of HAL_5371_MDDI_14_FIELD_T
 */
typedef struct
{
    UINT32 fgBgYUV420                      :    1;
    UINT32 fgBg420Frame                    :    1;
    UINT32 u4BgDramSegment                 :    2;
    UINT32 fgBgOldInterp                   :    1;
    UINT32 fgBgOldStore                    :    1;
    UINT32 fgBgNo420Set                    :    1;
    UINT32 fgBgUseB2rVsync                 :    1;

    UINT32 fgBgSelB2rVsyncEnd              :    1;
    UINT32 u4BgDelayB2rVsync               :    3;
    UINT32 u4BgRotateB2Rptr                :    2;
    UINT32 u4BgShift32Dnt                  :    2;

    UINT32 u4PSkip                         :    11;
    UINT32 fgNone1                         :    1;
    UINT32 fgBgInv420Backup                :    1;
    UINT32 fgBgInv22Det                    :    1;
    UINT32 fgBgInv22Sel                    :    1;
    UINT32 fgBgShift32Dir                  :    1;
} HAL_5371_MDDI_14_FIELD_T;

/** Brief of HAL_5371_MDDI_14_UNION_T
 */
typedef union
{
    UINT32 u4Reg;
    HAL_5371_MDDI_14_FIELD_T rField;
} HAL_5371_MDDI_14_UNION_T;


/** Brief of HAL_5371_MDDI_1C_FIELD_T
 */
typedef struct
{
    UINT32 fgVDOEN                          :    1;
    UINT32 fgAPTR                           :    1;
    UINT32 fgHDEN                           :    1;
    UINT32 fgSUBX                           :    1;
    UINT32 fgNone1                          :    1;
    UINT32 fgVSYNC_P                        :    1;
    UINT32 fgHSYNC_P                        :    1;
    UINT32 fgRUND_80                        :    1;

    UINT32 fgFS_RST                         :    1;
    UINT32 fgACTP                           :    1;
    UINT32 fgMA3F                           :    1;
    UINT32 fgAPD                            :    1;
    UINT32 fgERRD                           :    1;
    UINT32 fgAPF                            :    1;
    UINT32 fgF1EDGE                         :    1;
    UINT32 fgF1FLD                          :    1;

    UINT32 fgPROT                           :    1;
    UINT32 fgWRST                           :    1;
    UINT32 fgSTILL                          :    1;
    UINT32 fgWREND                          :    1;
    UINT32 fgPROTH1                         :    1;
    UINT32 fgDISP3                          :    1;
    UINT32 fgPROTH2                         :    1;
    UINT32 fgPROTCOORD                      :    1;

    UINT32 fgTST0                           :    1;
    UINT32 fgTST1                           :    1;
    UINT32 fgTST2                           :    1;
    UINT32 fgTST3                           :    1;
    UINT32 fgTST4                           :    1;
    UINT32 fgTST5                           :    1;
    UINT32 fgTST6                           :    1;
    UINT32 fgTST7                           :    1;
} HAL_5371_MDDI_1C_FIELD_T;

/** Brief of HAL_5371_MDDI_1C_UNION_T
 */
typedef union
{
    UINT32 u4Reg;
    HAL_5371_MDDI_1C_FIELD_T rField;
} HAL_5371_MDDI_1C_UNION_T;


/** Brief of HAL_5371_MDDI_3C_FIELD_T
 */
typedef struct
{
    UINT32 u4Reserved1                     :    15;
    UINT32 fg32Seq                         :    1;
    UINT32 fgSceneChg                      :    1;
    UINT32 fgBadEdit                       :    1;
    UINT32 fg22Seq                         :    1;
    UINT32 u4Reserved2                     :    13;
} HAL_5371_MDDI_3C_FIELD_T;

/** Brief of HAL_5371_MDDI_3C_UNION_T
 */
typedef union
{
    UINT32 u4Reg;
    HAL_5371_MDDI_3C_FIELD_T rField;
} HAL_5371_MDDI_3C_UNION_T;

//-----------------------------------------------------------------------------
// Other constants
//-----------------------------------------------------------------------------

#define VDP_MDDI_BOB_CHANGE_TIME            (8)

#define MDDI_1080_WIDTH                     1920
#define MDDI_1080_HEIGHT                    1080

#define MDDI_720P_WIDTH                     1280
#define MDDI_720P_HEIGHT                    720

#define MDDI_NTSC_WIDTH                     720
#define MDDI_NTSC_HEIGHT                    480

#define MDDI_PAL_WIDTH                      720
#define MDDI_PAL_HEIGHT                     576

#define MDDI_HPSD_UNIT                      (0x800)

#define MDDI_PSCAN_LIMITATION               (95)



#define MDDI_MDDI_HPDS		0
#define MDDI_VDOIN_HPDS		1

#define MDDI_DRAM_32MB		0
#define MDDI_DRAM_64MB		1

#endif    // VDP_DRVIF_H

