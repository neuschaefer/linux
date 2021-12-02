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
 * $RCSfile: x_drv_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef X_DRV_IF_H
#define X_DRV_IF_H

/** @file x_drv_if.h
 *  Driver interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_tuner.h"

#include "source_table.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DRV_VDO_QTY_BLENDING                    0
#define DRV_VDO_QTY_BRIGHTNESS                  1
#define DRV_VDO_QTY_CONTRAST                    2
#define DRV_VDO_QTY_HUE                         3
#define DRV_VDO_QTY_SATURATION                  4
#define DRV_VDO_QTY_BLE                         5
#define DRV_VDO_QTY_CTI                         6
#define DRV_VDO_QTY_LTI                         7
//#define DRV_VDO_QTY_SHARPNESS_ON_OFF            8
#define DRV_VDO_QTY_SHARPNESS                   9
#define DRV_VDO_QTY_NR                          10
#define DRV_VDO_QTY_COLOR_SUPPRESS              11
//#define DRV_VDO_QTY_DEINT_FILM                  12
//#define DRV_VDO_QTY_DEINT_DEINT                 13
//#define DRV_VDO_QTY_DEINT_EDGE                  14
#define DRV_VDO_QTY_ADAPTIVE_LUMA               15
#define DRV_VDO_QTY_SCE                         16
#define DRV_VDO_QTY_R_GAIN                      17
#define DRV_VDO_QTY_G_GAIN                      18
#define DRV_VDO_QTY_B_GAIN                      19
//#define DRV_VDO_QTY_R_OFFSET                    20
//#define DRV_VDO_QTY_G_OFFSET                    21
//#define DRV_VDO_QTY_B_OFFSET                    22
#define DRV_VDO_QTY_GAMMA                       23
#define DRV_VDO_QTY_WHITE_PEAK_LMT              24
#define DRV_VDO_QTY_BACKLIGHT_LVL               25
#define DRV_VDO_QTY_ADAPTIVE_BACKLIGHT          26
#define DRV_VDO_QTY_3D_NR                       27
//#define DRV_VDO_QTY_NS                          28
//#define DRV_VDO_QTY_SOFT_BWS                    29    //SoftBWS

#define DRV_VDO_QTYMSK_BLENDING                 (1<<DRV_VDO_QTY_BLENDING)
#define DRV_VDO_QTYMSK_BRIGHTNESS               (1<<DRV_VDO_QTY_BRIGHTNESS)
#define DRV_VDO_QTYMSK_CONTRAST                 (1<<DRV_VDO_QTY_CONTRAST)
#define DRV_VDO_QTYMSK_HUE                      (1<<DRV_VDO_QTY_HUE)
#define DRV_VDO_QTYMSK_SATURATION               (1<<DRV_VDO_QTY_SATURATION)
#define DRV_VDO_QTYMSK_BLE                      (1<<DRV_VDO_QTY_BLE)
#define DRV_VDO_QTYMSK_CTI                      (1<<DRV_VDO_QTY_CTI)
#define DRV_VDO_QTYMSK_LTI                      (1<<DRV_VDO_QTY_LTI)
//#define DRV_VDO_QTYMSK_SHARPNESS_ON_OFF         (1<<DRV_VDO_QTY_SHARPNESS_ON_OFF)
#define DRV_VDO_QTYMSK_SHARPNESS                (1<<DRV_VDO_QTY_SHARPNESS)
#define DRV_VDO_QTYMSK_NR                       (1<<DRV_VDO_QTY_NR)
#define DRV_VDO_QTYMSK_COLOR_SUPPRESS			(1<<DRV_VDO_QTY_COLOR_SUPPRESS)
//#define DRV_VDO_QTYMSK_DEINT_FILM				  (1<<DRV_VDO_QTY_DEINT_FILM)
//#define DRV_VDO_QTYMSK_DEINT_DEINT              (1<<DRV_VDO_QTY_DEINT_DEINT)
//#define DRV_VDO_QTYMSK_DEINT_EDGE               (1<<DRV_VDO_QTY_DEINT_EDGE)
#define DRV_VDO_QTYMSK_ADAPTIVE_LUMA            (1<<DRV_VDO_QTY_ADAPTIVE_LUMA)
#define DRV_VDO_QTYMSK_SCE                      (1<<DRV_VDO_QTY_SCE)
#define DRV_VDO_QTYMSK_R_GAIN                   (1<<DRV_VDO_QTY_R_GAIN)
#define DRV_VDO_QTYMSK_G_GAIN                   (1<<DRV_VDO_QTY_G_GAIN)
#define DRV_VDO_QTYMSK_B_GAIN                   (1<<DRV_VDO_QTY_B_GAIN)
//#define DRV_VDO_QTYMSK_R_OFFSET                 (1<<DRV_VDO_QTY_R_OFFSET)
//#define DRV_VDO_QTYMSK_G_OFFSET                 (1<<DRV_VDO_QTY_G_OFFSET)
//#define DRV_VDO_QTYMSK_B_OFFSET                 (1<<DRV_VDO_QTY_B_OFFSET)
#define DRV_VDO_QTYMSK_GAMMA                    (1<<DRV_VDO_QTY_GAMMA)
#define DRV_VDO_QTYMSK_WHITE_PEAK_LMT           (1<<DRV_VDO_QTY_WHITE_PEAK_LMT)
#define DRV_VDO_QTYMSK_BACKLIGHT_LVL            (1<<DRV_VDO_QTY_BACKLIGHT_LVL)
#define DRV_VDO_QTYMSK_ADAPTIVE_BACKLIGHT       (1<<DRV_VDO_QTY_ADAPTIVE_BACKLIGHT)
#define DRV_VDO_QTYMSK_3D_NR                    (1<<DRV_VDO_QTY_3D_NR)
//#define DRV_VDO_QTYMSK_NS                       (1<<DRV_VDO_QTY_NS)
//#define DRV_VDO_QTYMSK_SOFT_BWS                 (1<<DRV_VDO_QTY_SOFT_BWS)

#define DRV_VDO_SCART_BYPASS                    0
#define DRV_VDO_SCART_OUTPUT                    1
#define DRV_VDO_SCART_MODE                      2

#define DRV_VDO_SCARTMSK_BYPASS                 (1<<DRV_VDO_SCART_BYPASS)
#define DRV_VDO_SCARTMSK_OUTPUT                 (1<<DRV_VDO_SCART_OUTPUT)
#define DRV_VDO_SCARTMSK_MODE                   (1<<DRV_VDO_SCART_MODE)

#define DRV_VDO_VGA_CLOCK                       0
#define DRV_VDO_VGA_PHASE                       1
#define DRV_VDO_VGA_AUTO                        2
#define DRV_VDO_VGA_AUTOCOLOR                   3

#define DRV_VDO_VGAMSK_CLOCK                    (1<<DRV_VDO_VGA_CLOCK)
#define DRV_VDO_VGAMSK_PHASE                    (1<<DRV_VDO_VGA_PHASE)
#define DRV_VDO_VGAMSK_AUTO                     (1<<DRV_VDO_VGA_AUTO)
#define DRV_VDO_VGAMSK_AUTOCOLOR                (1<<DRV_VDO_VGA_AUTOCOLOR)

#define DRV_AUD_QTY_VOLUME                      0
#define DRV_AUD_QTY_BASS                        1
#define DRV_AUD_QTY_TREBLE                      2
#define DRV_AUD_QTY_BALANCE                     3
#define DRV_AUD_QTY_REVERB                      4
#define DRV_AUD_QTY_EQUALIZER                   5
#define DRV_AUD_QTY_SURROUND                    6
#define DRV_AUD_QTY_AUTO_VOL_CTRL               7
#define DRV_AUD_QTY_SPEAKER                     8
#define DRV_AUD_QTY_SPDIF                       9

#define DRV_AUD_QTYMSK_VOLUME                   (1<<DRV_AUD_QTY_VOLUME)
#define DRV_AUD_QTYMSK_BASS                     (1<<DRV_AUD_QTY_BASS)
#define DRV_AUD_QTYMSK_TREBLE                   (1<<DRV_AUD_QTY_TREBLE)
#define DRV_AUD_QTYMSK_BALANCE                  (1<<DRV_AUD_QTY_BALANCE)
#define DRV_AUD_QTYMSK_REVERB                   (1<<DRV_AUD_QTY_REVERB)
#define DRV_AUD_QTYMSK_EQUALIZER                (1<<DRV_AUD_QTY_EQUALIZER)
#define DRV_AUD_QTYMSK_SURROUND                 (1<<DRV_AUD_QTY_SURROUND)
#define DRV_AUD_QTYMSK_AUTO_VOL_CTRL            (1<<DRV_AUD_QTY_AUTO_VOL_CTRL)
#define DRV_AUD_QTYMSK_SPEAKER                  (1<<DRV_AUD_QTY_SPEAKER)
#define DRV_AUD_QTYMSK_SPDIF                    (1<<DRV_AUD_QTY_SPDIF)

#define DRV_OSD_PLANE_1 0
#define DRV_OSD_PLANE_2 1
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef void (*PFN_DRV_CALLBACK)(INT32 i4Tag);

typedef enum
{
    DRV_E_OK = 0,
    DRV_E_FAIL,
    DRV_E_BUFFER_OVERFLOW,
    DRV_E_BUFFER_UNDERFLOW,
    DRV_E_OUT_OF_MEMORY,
    DRV_E_INV_ARG,
    DRV_E_NOT_EXIST
} DRV_ERR_CODE_T;

//
// Video
//

// Analogous to source_table.h's video input source definition
typedef enum
{
    DRV_IS_ATUNER_1                             = VSS_MKVSRC(VSS_TUNER,0),
    DRV_IS_CVBS_1                               = VSS_MKVSRC(VSS_CVBS,0),
    DRV_IS_CVBS_2,
    DRV_IS_CVBS_3,
    DRV_IS_SVIDEO_1                             = VSS_MKVSRC(VSS_SV,0),
    DRV_IS_SVIDEO_2,
    DRV_IS_SVIDEO_3,
    DRV_IS_YPBPR_1                              = VSS_MKVSRC(VSS_YPBPR,0),
    DRV_IS_YPBPR_2,
    DRV_IS_YPBPR_3,
    DRV_IS_YPBPR_4,
    DRV_IS_VGA_1                                = VSS_MKVSRC(VSS_VGA,0),
    DRV_IS_HDMI_1                               = VSS_MKVSRC(VSS_HDMI,0),
    DRV_IS_HDMI_2,
    DRV_IS_HDMI_3,
    DRV_IS_HDMI_4,
    DRV_IS_DVI_1                                = VSS_MKVSRC(VSS_DVI,0),
    DRV_IS_DVI_2,
    DRV_IS_DVI_3,
    DRV_IS_MPEG_1                               = VSS_MKVSRC(VSS_DTV,0),
    DRV_IS_MPEG_2,
    DRV_IS_SCART_1                              = VSS_MKVSRC(VSS_SCART,0),
    DRV_IS_SCART_2,
} DRV_INPUT_SRC_T;

typedef enum
{
    DRV_VDO_SCART_FS_UNKNOWN,
    DRV_VDO_SCART_FS_NO_SIGNAL,
    DRV_VDO_SCART_FS_ASPECT_16_9,
    DRV_VDO_SCART_FS_ASPECT_4_3
} DRV_VDO_SCART_FS_T;

typedef enum
{
    DRV_VDO_SCART_FB_UNKNOWN = 0,
    DRV_VDO_SCART_FB_COMPOSITE,
    DRV_VDO_SCART_FB_RGB,
    DRV_VDO_SCART_FB_TOGGLE
}   DRV_VDO_SCART_FB_T;

typedef enum
{
    DRV_VDO_SCART_BYPASS_TRUE = 0,
    DRV_VDO_SCART_BYPASS_FALSE
}   DRV_VDO_SCART_BYPASS_T;

typedef enum
{
    DRV_VDO_SCART_OUTPUT_OFF = 0,
    DRV_VDO_SCART_OUTPUT_COMPOSITE,
    DRV_VDO_SCART_OUTPUT_S_VIDEO
}  DRV_VDO_SCART_OUTPUT_T;

typedef enum
{
    DRV_VDO_SCART_MODE_AUTO,
    DRV_VDO_SCART_MODE_MIX,
    DRV_VDO_SCART_MODE_CVBS,
    DRV_VDO_SCART_MODE_RGB,
    DRV_VDO_SCART_MODE_SVIDEO
} DRV_VDO_SCART_MODE_T;

typedef struct
{
    DRV_VDO_SCART_FS_T      rFs;
    DRV_VDO_SCART_FB_T      rFb;
    DRV_VDO_SCART_BYPASS_T  rBypass;
    DRV_VDO_SCART_OUTPUT_T  rOutput;
} DRV_VDO_SCART_STATUS_T;

typedef struct
{
    DRV_VDO_SCART_BYPASS_T  rBypass;
    DRV_VDO_SCART_OUTPUT_T  rOutput;
    DRV_VDO_SCART_MODE_T    rMode;
} DRV_VDO_SCART_PARAM_T;

typedef struct
{
    UINT8 u1Clock;
    UINT8 u1Phase;
} DRV_VDO_VGA_PARAM_T;

// Notification callback list for various VDO event
typedef struct
{
    PFN_DRV_CALLBACK        pfnResChange;
    PFN_DRV_CALLBACK        pfnScartStatus;
    PFN_DRV_CALLBACK        pfnMpegCcData;
    PFN_DRV_CALLBACK        pfnVbiTtxData;
    PFN_DRV_CALLBACK        pfnVbiCcData;
    PFN_DRV_CALLBACK        pfnVbiWssData;
    PFN_DRV_CALLBACK        pfnVbiVpsData;
    PFN_DRV_CALLBACK        pfnVgaAuto;
    PFN_DRV_CALLBACK        pfnVgaAutocolor;
    PFN_DRV_CALLBACK        pfnPtsNfy;
} DRV_VDO_CALLBACK_LIST_T;

// Video plane display area
typedef struct
{
    UINT32                  u4Left;
    UINT32                  u4Top;
    UINT32                  u4Width;
    UINT32                  u4Height;
} DRV_VDO_WINDOW_T;

typedef enum
{
    DRV_VDO_DISP_FMT_NORMAL,
    DRV_VDO_DISP_FMT_LETTERBOX,
    DRV_VDO_DISP_FMT_ZOOM,
    DRV_VDO_DISP_FMT_PANORAMA,
    DRV_VDO_DISP_FMT_DOTBYDOT,
    DRV_VDO_DISP_FMT_AUTO
} DRV_VDO_DISP_FMT_T;

// The structure for DRV_VDO_SetParameter
typedef struct
{
    UINT16                  u2Pid;
    UINT16                  u2PcrPid;
    DRV_INPUT_SRC_T         rInputSrc;
    DRV_VDO_DISP_FMT_T      rDispFmt;
    UINT32                  u4BackgroundColor;
    DRV_VDO_WINDOW_T        rVideoWindow;
    DRV_VDO_CALLBACK_LIST_T rCallbackList;
    void*                   pvNotifyTag;
} DRV_VDO_PARAM_T;

// The structure for DRV_VDO_SetQuality
typedef struct
{
    UINT8                   u1Blending;
    UINT8                   u1Brightness;
    UINT8                   u1Contrast;
    INT8                    i1Hue;
    UINT8                   u1Saturation;
    UINT8                   u1Ble;
    UINT8                   u1Cti;
    UINT8                   u1Lti;
    //UINT8                   u1SharpnessOnOff;
    UINT8                   u1Sharpness;
    UINT8                   u1Nr;
    UINT8                   u1ColorSuppress;
    //UINT8                   u1DeintFilm;
    //UINT8                   u1DeintDeint;
    //UINT8                   u1DeintEdge;
    UINT8                   u1AdaptiveLuma;
    UINT8                   u1Sce;
    UINT8                   u1RGain;
    UINT8                   u1BGain;
    UINT8                   u1GGain;
    //UINT8                   u1ROffset;
    //UINT8                   u1GOffset;
    //UINT8                   u1BOffset;
    UINT8                   u1Gamma;
    UINT8                   u1WhitePeakLmt;
    UINT8                   u1BackLightLvl;
    UINT8                   u1AdaptiveBackLight;
    UINT8                   u13dNr;
    //UINT8                   u1Ns;
    //UINT8                   u1SoftBws;
} DRV_VDO_QUALITY_T;

typedef enum
{
    DRV_VDO_SIGNAL_DETECTED,
    DRV_VDO_SIGNAL_NOT_DETECTED
} DRV_VDO_SIGNAL_STATUS_T;

// The structure for DRV_VDO_GetInfo
typedef struct
{
    BOOL                    fgProgressive;
    BOOL                    fg4_3;
    BOOL	                fgVgaTiming;
    UINT16                  u2Width;
    UINT16                  u2Height;
    UINT16                  u2FramePerSecond;
    DRV_VDO_SIGNAL_STATUS_T rSigStatus;
    DRV_INPUT_SRC_T         rInputSrc;

} DRV_VDO_INFO_T;

typedef struct
{
    BOOL                    fgEvenField;
    UINT8                   u1Byte1;
    UINT8                   u1Byte2;
} DRV_VDO_VBICC_DATA_T;

typedef struct
{
    UINT32                  u4Padding;
    UINT8                   au1Buf[42];
} DRV_VDO_VBITTX_PACKET_T;

typedef struct
{
    UINT16                  u2NumPacket;
    DRV_VDO_VBITTX_PACKET_T *prPacket;
} DRV_VDO_VBITTX_DATA_T;

typedef enum
{
    DRV_VDO_VBITTX_DATA_ARRIVAL,
    DRV_VDO_VBITTX_OVERFLOW,
    DRV_VDO_VBITTX_UNDERFLOW,
    DRV_VDO_VBITTX_ERROR
} DRV_VDO_VBITTX_COND_T;

typedef enum
{
    DRV_VDO_MPEGCC_DATA_ARRIVAL,
    DRV_VDO_MPEGCC_ERROR
} DRV_VDO_MPEGCC_COND_T;

typedef struct
{
    UINT16                  u2Data;
} DRV_VDO_VBIWSS_DATA_T;

typedef struct
{
    UINT16                  u2Data;
} DRV_VDO_VBIVPS_DATA_T;

typedef struct
{
    BOOL                    fgScteCc;
    UINT32                  u4Size;
    UINT8*                  pu1Buf;
    UINT64                  u8Pts;
} DRV_VDO_MPEGCC_DATA_T;

typedef enum
{
    DRV_CM_ARGB_8888,
    DRV_CM_ARGB_4444,
    DRV_CM_ARGB_1555,
    DRV_CM_RGB_565,
    DRV_CM_RGB_8,
    DRV_CM_RGB_4,
    DRV_CM_RGB_2,
    DRV_CM_AYBR_8888,
    DRV_CM_YBR_422,
    DRV_CM_BRY_422,
    DRV_CM_YBR_8,
    DRV_CM_YBR_4,
    DRV_CM_YBR_2,
} DRV_COLOR_MODE_T;

typedef enum
{
    DRV_OSD_BM_PIXEL,
    DRV_OSD_BM_PLANE,
} DRV_OSD_BLEND_MODE_T;

//
// Audio
//

// The structure for DRV_VDO_SetParameter
typedef struct
{
    UINT8                   u1Volume;
    UINT8                   u1Bass;
    UINT8                   u1Treble;
    UINT8                   u1Balance;
    UINT8                   u1Reverb;
    UINT8                   u1Equalizer;
    UINT8                   u1Surround;
    UINT8                   u1AutoVolumeCtrl;
    UINT8                   u1SpeakerOn;
    UINT8                   u1SpdifType;
} DRV_AUD_QUALITY_T;

// Audio coding type
typedef enum
{
    DRV_AUD_CT_UNKNOWN = 0,
    DRV_AUD_CT_MPEG,
    DRV_AUD_CT_AC3,
    DRV_AUD_CT_PCM,
    DRV_AUD_CT_MP3,
    DRV_AUD_CT_AAC,
    DRV_AUD_CT_DTS,
    DRV_AUD_CT_WMA,
    DRV_AUD_CT_RA,
    DRV_AUD_CT_HDCD,
    DRV_AUD_CT_MLP,
    DRV_AUD_CT_MTS,
    DRV_AUD_CT_A2,
    DRV_AUD_CT_PAL,
    DRV_AUD_CT_FMFM,
    DRV_AUD_CT_NICAM,
    DRV_AUD_CT_TTXAAC,
    DRV_AUD_CT_DETECTOR,
    DRV_AUD_CT_MINER,
    DRV_AUD_CT_AUTO
} DRV_AUD_CODING_TYPE_T;

// Analog TV system. i.e. the position sound carrier frequency
typedef enum
{
    DRV_TV_SYS_B_G,
    DRV_TV_SYS_D_K,
    DRV_TV_SYS_I,
    DRV_TV_SYS_L,
    DRV_TV_SYS_L1,
    DRV_TV_SYS_M_N,
    DRV_TV_SYS_FM_Radio,
    DRV_TV_SYS_Satelite,
    DRV_TV_SYS_UNKNOWN,
}   DRV_TV_SYS_T;

// Analog sound system. i.e. the modulation method
typedef enum
{
    DRV_SOUND_SYS_AM,
    DRV_SOUND_SYS_FM_MONO,
    DRV_SOUND_SYS_FM_EIA_J,  // Japan
    DRV_SOUND_SYS_FM_A2,  // Korea
    DRV_SOUND_SYS_FM_A2_DK1,
    DRV_SOUND_SYS_FM_A2_DK2,
    DRV_SOUND_SYS_FM_RADIO,
    DRV_SOUND_SYS_FM_MONO_NICAM,
    DRV_SOUND_SYS_BTSC
}   DRV_SOUND_SYS_T;

// Spdif type
typedef enum
{
    DRV_SPDIF_OFF = 0,
    DRV_SPDIF_PCM,
    DRV_SPDIF_DOLBY_DIGITAL
} DRV_AUD_SPDIF_TYPE_T;
typedef struct
{
    UINT16                  u2Pid;
    UINT16                  u2PcrPid;
    DRV_INPUT_SRC_T         rInputSrc;
    DRV_AUD_CODING_TYPE_T   rCodingType;
    // Notification callback for analog TV/sound system detection
    PFN_DRV_CALLBACK        pfnDetectDone;
    PFN_DRV_CALLBACK        pfnPlayDone;
    PFN_DRV_CALLBACK        pfnStopDone;
    void*                   pvNotifyTag;    
} DRV_AUD_PARAM_T;

typedef struct
{
    DRV_INPUT_SRC_T         rInputSrc;
    DRV_AUD_CODING_TYPE_T   rCodingType;
    // Analog TV system
    DRV_TV_SYS_T            rTvSys;
    // Analog sound system
    DRV_SOUND_SYS_T         rSoundSys;
} DRV_AUD_INFO_T;

//
// OSD & Graphics
//

typedef struct
{
    UINT8                   u1PlaID;
    DRV_COLOR_MODE_T        rColorMode;
    UINT32                  u4Pitch; // u4Pitch MUST align with 16 bytes
    UINT32                  u4Left;
    UINT32                  u4Top;
    // u4Width * bytes-per-pixel MUST align with 16 bytes
    UINT32                  u4Width;
    UINT32                  u4Height;
    UINT32                  u4ScaleWidth;
    UINT32                  u4ScaleHeight;
    // pu1FrameBaseAddr MUST align with 16 bytes
    UINT8*                  pu1FrameBaseAddr;
    UINT32*                 pu4Palette;
} DRV_OSD_RGN_T;

typedef struct
{
    UINT16                  u2DstX;
    UINT16                  u2DstY;
    UINT16                  u2DstPitch;
    // Rectangular's width and height
    UINT16                  u2Width;
    UINT16                  u2Height;
    // Destiniation paper
    UINT8*                  pu1DstBaseAddr;
    // Color
    UINT32                  u4Color;
} DRV_GFX_FILL_T;

typedef struct
{
    UINT16                  u2SrcX;
    UINT16                  u2SrcY;
    UINT16                  u2SrcPitch;
    UINT16                  u2DstX;
    UINT16                  u2DstY;
    UINT16                  u2DstPitch;
    // Blitting area's width and height
    UINT16                  u2Width;
    UINT16                  u2Height;
    // Source paper
    UINT8*                  pu1SrcBaseAddr;
    // Destination paper
    UINT8*                  pu1DstBaseAddr;
} DRV_GFX_BLIT_T;



typedef struct
{
    UINT16                  u2SrcX;
    UINT16                  u2SrcY;
    UINT16                  u2SrcPitch;
    UINT16                  u2DstX;
    UINT16                  u2DstY;
    UINT16                  u2DstPitch;
    // Blitting area's width and height
    UINT16                  u2Width;
    UINT16                  u2Height;
    // Source paper
    UINT8*                  pu1SrcBaseAddr;
    // Destination paper
    UINT8*                  pu1DstBaseAddr;
    UINT32                  u4SrcColorKeyMin;
    UINT32                  u4SrcColorKeyMax;
    UINT32                  u4InverseMask;
} DRV_GFX_TRANSBLIT_T;


typedef struct
{
    UINT16                  u2SrcX;
    UINT16                  u2SrcY;
    UINT16                  u2SrcPitch;
    UINT16                  u2DstX;
    UINT16                  u2DstY;
    UINT16                  u2DstPitch;
    // Blending area's width, height
    UINT16                  u2Width;
    UINT16                  u2Height;
    // Source paper
    UINT8*                  pu1SrcBaseAddr;
    // Destination paper
    UINT8*                  pu1DstBaseAddr;
    UINT32                  u4Alpha;


} DRV_GFX_BLEND_T;

//
// Demux
//

/// Demux filter sturcture
typedef struct
{
    UINT8                   u1Offset;           ///< Byte offset, 1 - 31
    UINT8                   u1PatternLength;    ///< Length of pattern
    BOOL                    fgCheckCrc;         ///< If check CRC
    UINT16                  u2Pid;              ///< PID value
    UINT32                  u4BufferSize;       ///< Filter buffer size
    UINT8*                  pu1Pattern;         ///< The pattern array
    UINT8*                  pu1PositiveMask;    ///< Positive mask array
    UINT8*                  pu1NegativeMask;    ///< Negative mask array
    PFN_DRV_CALLBACK		pfnHandler;         ///< Section callback handler
    void*                   pvNotifyTag;        ///< Tag value to handler
} DRV_DMX_FILTER_T;

/// Demux frame data (section or PES) structure
typedef struct
{
    UINT8                   u1SerialNumber;     ///< Serial number
    UINT32                  u4FrameAddr;        ///< Frame address
    UINT32                  u4Size;             ///< Frame size
} DRV_DMX_DATA_T;

/// Demux message type
typedef enum
{
    DMX_MSG_TYPE_PSI,                           ///< PSI message
    DMX_MSG_TYPE_PES                            ///< PES message
} DMX_MSG_TYPE_T;

/// Demux message structure
typedef struct
{
    DMX_MSG_TYPE_T      eType;                  ///< Message type
    UINT8               u1Id;                   ///< Filter id or PID id
    DRV_DMX_DATA_T      rData;                  ///< Type specific data
    void*               pvNotifyTag;            ///< User tag value
} DRV_DMX_MSG_T;

/// Demux PES structure
typedef struct
{
    UINT16                  u2Pid;              ///< PID value
    UINT32                  u4BufferSize;       ///< PES buffer size
    PFN_DRV_CALLBACK        pfnHandler;         ///< PES callback handler
    void*                   pvNotifyTag;        ///< User tag value
} DRV_DMX_PES_T;

//
// Tuner
//

typedef enum
{
    DRV_TUNER_MOD_UNKNOWN					    = MOD_UNKNOWN,
    DRV_TUNER_MOD_PSK_8                         = MOD_PSK_8,
    DRV_TUNER_MOD_VSB_8                         = MOD_VSB_8,
    DRV_TUNER_MOD_VSB_16                        = MOD_VSB_16,
    DRV_TUNER_MOD_QAM_16                        = MOD_QAM_16,
    DRV_TUNER_MOD_QAM_32                        = MOD_QAM_32,
    DRV_TUNER_MOD_QAM_64                        = MOD_QAM_64,
    DRV_TUNER_MOD_QAM_80                        = MOD_QAM_80,
    DRV_TUNER_MOD_QAM_96                        = MOD_QAM_96,
    DRV_TUNER_MOD_QAM_112                       = MOD_QAM_112,
    DRV_TUNER_MOD_QAM_128                       = MOD_QAM_128,
    DRV_TUNER_MOD_QAM_160                       = MOD_QAM_160,
    DRV_TUNER_MOD_QAM_192                       = MOD_QAM_192,
    DRV_TUNER_MOD_QAM_224                       = MOD_QAM_224,
    DRV_TUNER_MOD_QAM_256                       = MOD_QAM_256,
    DRV_TUNER_MOD_QAM_320                       = MOD_QAM_320,
    DRV_TUNER_MOD_QAM_384                       = MOD_QAM_384,
    DRV_TUNER_MOD_QAM_448                       = MOD_QAM_448,
    DRV_TUNER_MOD_QAM_512                       = MOD_QAM_512,
    DRV_TUNER_MOD_QAM_640                       = MOD_QAM_640,
    DRV_TUNER_MOD_QAM_768                       = MOD_QAM_768,
    DRV_TUNER_MOD_QAM_896                       = MOD_QAM_896,
    DRV_TUNER_MOD_QAM_1024                      = MOD_QAM_1024,
    DRV_TUNER_MOD_QPSK                          = MOD_QPSK,
    DRV_TUNER_MOD_OQPSK                         = MOD_OQPSK,
    DRV_TUNER_MOD_BPSK                          = MOD_BPSK,
    DRV_TUNER_MOD_VSB_AM                        = MOD_VSB_AM
} DRV_TUNER_MOD_T;

typedef enum
{
    DRV_TUNER_BW_UNKNOWN                        = BW_UNKNOWN,
    DRV_TUNER_BW_6_MHz                          = BW_6_MHz,
    DRV_TUNER_BW_7_MHz                          = BW_7_MHz,
    DRV_TUNER_BW_8_MHz                          = BW_8_MHz
}   DRV_TUNER_BANDWIDTH_T;

typedef struct
{
    BOOL                    fgSpectrumInversion;
    DRV_TUNER_MOD_T         rMod;
    DRV_TUNER_BANDWIDTH_T   rBandwidth;
    PFN_DRV_CALLBACK        pfnTuningDone;
    PFN_DRV_CALLBACK        pfnLockStatusChange;
    UINT32                  u4SymbolRate;
    UINT32                  u4QamSize;
} DRV_TUNER_DIG_PARAM_T;

typedef struct
{
    BOOL                    fgAfcEnabled;
    PFN_DRV_CALLBACK        pfnTuningDone;
    DRV_TV_SYS_T            rTvSys;
} DRV_TUNER_ANA_PARAM_T;

typedef struct
{
    DRV_TUNER_DIG_PARAM_T   rDigParam;
    DRV_TUNER_ANA_PARAM_T   rAnaParam;
} DRV_TUNER_PARAM_T;

typedef enum
{
    DRV_TUNER_LOCKED,
    DRV_TUNER_NOT_LOCKED,
} DRV_TUNER_LOCK_STATUS_T;

typedef enum
{
    DRV_TUNER_CT_DIGS       = TUNER_CONN_TYPE_SAT_DIG,
    DRV_TUNER_CT_DIGC       = TUNER_CONN_TYPE_CAB_DIG,
    DRV_TUNER_CT_DIGT       = TUNER_CONN_TYPE_TER_DIG,
    DRV_TUNER_CT_ANAS       = TUNER_CONN_TYPE_SAT_ANA,
    DRV_TUNER_CT_ANAC       = TUNER_CONN_TYPE_CAB_ANA,
    DRV_TUNER_CT_ANAT       = TUNER_CONN_TYPE_TER_ANA,
    DRV_TUNER_CT_MAX
} DRV_TUNER_CONN_TYPE_T;

typedef struct
{
    DRV_TUNER_CONN_TYPE_T rConnType;
} DRV_TUNER_INFO_T;

//
// IR RX
//

typedef enum
{
    DRV_IR_TYPE_NEC,
    DRV_IR_TYPE_RC56,
    DRV_IR_TYPE_USER,
    DRV_IR_TYPE_AUTO
} DRV_IR_TYPE_T;

typedef enum 
{
    DRV_IR_REPEAT_FREQ_NONE,
    DRV_IR_REPEAT_FREQ_LOW,
    DRV_IR_REPEAT_FREQ_HIGH	
} DRV_IR_REPEAT_FREQ_T;

typedef enum
{
    DRV_IR_EVENT_UP,
    DRV_IR_EVENT_DOWN,        
    DRV_IR_EVENT_REPEAT,  
    DRV_IR_EVENT_EXT,
    DRV_IR_EVENT_DUMMY
} DRV_IR_EVENT_TYPE_T;

typedef struct
{
    DRV_IR_EVENT_TYPE_T     rType;
    UINT32                  u4Key;
} DRV_IR_EVENT_T;

typedef void (*PFN_DRV_IR_CALLBACK)(DRV_IR_EVENT_TYPE_T rType, UINT32 u4Key);

typedef struct
{
    DRV_IR_REPEAT_FREQ_T rRepeatFreq;
    UINT32           rIrType;
    PFN_DRV_IR_CALLBACK        pfnBtnEvent;
} DRV_IR_PARAM_T;

//
// USB
//

typedef enum
{
    // device/medium status.
    DRV_USB_DEVICE_CHANGE,
    DRV_USB_MEDIUM_CHANGE
} DRV_USB_EVENT_TYPE_T;

typedef struct
{
    UINT32                  u4Aux;
    PFN_DRV_CALLBACK        pfnUSBNotify;
} DRV_USB_INIT_PARAM_T;

typedef enum
{
    // Get information cmd.
    DRV_USB_IOCTL_GET_BLOCK_SIZE,
    DRV_USB_IOCTL_GET_BLOCK_NO,
    DRV_USB_IOCTL_GET_WP_STATUS,
    DRV_USB_IOCTL_GET_DEVICE_STATUS,
    DRV_USB_IOCTL_GET_MEDIUM_NUM,
    DRV_USB_IOCTL_GET_MEDIUM_STATUS,

    // Set information cmd.
    DRV_USB_IOCTL_SET_NOTIFY_FUNC
} DRV_USB_IOCTL_TYPE_T;

typedef struct
{
    UINT32 u4Unit;
    // ioctl command type.
    DRV_USB_IOCTL_TYPE_T    rCmdType;
    UINT32                  u4Aux;
    void*                   pBuffer;
} DRV_USB_IOCTL_PARAM_T;

typedef struct
{
    UINT32                  u4Unit;
    UINT64                  u8StartSector;
    UINT32                  u4NumSectors;
    void*                   pBuffer;
    // if pfnUSBTransferDone == NULL, it is sync read/write operation.
    PFN_DRV_CALLBACK        pfnUSBTransferDone;
} DRV_USB_TRANS_PARAM_T;

typedef struct
{
    BOOL fgIRWakeup;
    BOOL fgVGAWakeup;
    BOOL fgRTCWakeup;
    BOOL fgRS232Wakeup;
} DRV_SYS_WAKEUP_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Exported functionss
//-----------------------------------------------------------------------------

//
// Driver initialization
//
extern DRV_ERR_CODE_T DRV_Init(void);

//
// Video and display
//

extern DRV_ERR_CODE_T DRV_VDO_SetParameter(UINT8 u1VideoIndex,
    DRV_VDO_PARAM_T* prParam);

extern DRV_ERR_CODE_T DRV_VDO_Start(UINT8 u1VideoIndex);

extern DRV_ERR_CODE_T DRV_VDO_Stop(UINT8 u1VideoIndex);

extern DRV_ERR_CODE_T DRV_VDO_Freeze(UINT8 u1VideoIndex);

extern DRV_ERR_CODE_T DRV_VDO_Resume(UINT8 u1VideoIndex);

extern DRV_ERR_CODE_T DRV_VDO_Mute(UINT8 u1VideoIndex);

extern DRV_ERR_CODE_T DRV_VDO_Unmute(UINT8 u1VideoIndex);

extern DRV_ERR_CODE_T DRV_VDO_SetOutputWindow(UINT8 u1VideoIndex,
    DRV_VDO_WINDOW_T* prWindow);

extern DRV_ERR_CODE_T DRV_VDO_SetOverScanRatio(UINT8 u1VideoIndex,
    UINT32 x, UINT32 y);

extern DRV_ERR_CODE_T DRV_VDO_SetScart(UINT32 u4Mask,
    DRV_VDO_SCART_PARAM_T* prParam);

extern DRV_ERR_CODE_T DRV_VDO_SetVga(UINT32 u4Mask,
    DRV_VDO_VGA_PARAM_T* prParam);

extern DRV_ERR_CODE_T DRV_VDO_SetQuality(UINT8 u1VideoIndex,
	UINT32 u4Mask, DRV_VDO_QUALITY_T* prQuality);

extern DRV_ERR_CODE_T DRV_VDO_SetDispFmt(UINT8 u1VideoIndex,
	DRV_VDO_DISP_FMT_T rFmt);

extern DRV_ERR_CODE_T DRV_VDO_GetInfo(UINT8 u1VideoIndex,
    DRV_VDO_INFO_T* prInfo);

extern DRV_ERR_CODE_T DRV_VDO_GetVbiTtx(UINT8 u1VideoIndex,
	DRV_VDO_VBITTX_DATA_T* prVbiTtxData, INT32 i4Tag);

extern DRV_ERR_CODE_T DRV_VDO_UnlockVbiTtx(UINT8 u1VideoIndex,
	DRV_VDO_VBITTX_DATA_T* prVbiTtxData, INT32 i4Tag);

extern DRV_ERR_CODE_T DRV_VDO_GetVbiCc(UINT8 u1VideoIndex,
	DRV_VDO_VBICC_DATA_T* prVbiCcData, INT32 i4Tag);

extern DRV_ERR_CODE_T DRV_VDO_GetVbiVps(UINT8 u1VideoIndex,
	DRV_VDO_VBIVPS_DATA_T* prVbiVpsData, INT32 i4Tag);

extern DRV_ERR_CODE_T DRV_VDO_GetVbiWss(UINT8 u1VideoIndex,
	DRV_VDO_VBIWSS_DATA_T* prVbiWssData, INT32 i4Tag);

extern DRV_ERR_CODE_T DRV_VDO_GetMpegCc(UINT8 u1VideoIndex,
	DRV_VDO_MPEGCC_DATA_T* prMpegCcData, INT32 i4Tag);

extern DRV_ERR_CODE_T DRV_VDO_GetScartStatus(UINT8 u1VideoIndex,
	DRV_VDO_SCART_STATUS_T* prScartStatus);

extern DRV_ERR_CODE_T DRV_VDO_SetPtsNfy(UINT8 u1VideoIndex, 
	UINT64 u8Pts);

extern DRV_ERR_CODE_T DRV_VDO_GetStc(UINT8 u1VideoIndex, 
	UINT64* pu8Stc);

//
// Audio
//

extern DRV_ERR_CODE_T DRV_AUD_SetParameter(DRV_AUD_PARAM_T* prParam);

extern DRV_ERR_CODE_T DRV_AUD_GetInfo(DRV_AUD_INFO_T* prInfo);

extern DRV_ERR_CODE_T DRV_AUD_Start(void);

extern DRV_ERR_CODE_T DRV_AUD_Stop(void);

extern DRV_ERR_CODE_T DRV_AUD_Mute(void);

extern DRV_ERR_CODE_T DRV_AUD_Unmute(void);

extern DRV_ERR_CODE_T DRV_AUD_DetectTvSys(void);

extern DRV_ERR_CODE_T DRV_AUD_SetTvSys(DRV_TV_SYS_T rTvSys,
	DRV_SOUND_SYS_T rSoundSys);

extern DRV_ERR_CODE_T DRV_AUD_GetTvSys(DRV_TV_SYS_T* prTvSys,
	DRV_SOUND_SYS_T* prSoundSys);

extern DRV_ERR_CODE_T DRV_AUD_SetQuality(UINT32 u4Mask,
    DRV_AUD_QUALITY_T* prQuality);

//
// OSD
//

extern DRV_ERR_CODE_T DRV_OSD_Enable(UINT8 u1PlaID, BOOL fgEnable);

extern DRV_ERR_CODE_T DRV_OSD_RGN_Create(DRV_OSD_RGN_T* prOsd,
                                         UINT32* pu4RgnID);

extern DRV_ERR_CODE_T DRV_OSD_SetPlaneAlpha(UINT8 u1PlaID, UINT8 u1Alpha);

extern DRV_ERR_CODE_T DRV_OSD_SetRegionAlpha(UINT32 u4RgnID, UINT8 u1Alpha);

extern DRV_ERR_CODE_T DRV_OSD_SetColorkey(UINT32 u4RgnID, BOOL fgEnable,
                                          UINT32 u4Color);

extern DRV_ERR_CODE_T DRV_OSD_RGN_Delete(UINT8 u1PlaID, UINT32 u4RgnID);
//u1OrderType = 1, OSD Plane1 above OSD Plane2
//u1OrderType = 2, OSD Plane2 above OSD Plane1
extern DRV_ERR_CODE_T DRV_OSD_SetPlaneOrder(UINT8 u1OrderType);

//
// Graphics
//

extern DRV_ERR_CODE_T DRV_GFX_BitBlt(DRV_COLOR_MODE_T rColorMode,
    DRV_GFX_BLIT_T* prBlit);

extern DRV_ERR_CODE_T DRV_GFX_Fill(DRV_COLOR_MODE_T rColorMode,
    DRV_GFX_FILL_T* prFill);

extern DRV_ERR_CODE_T DRV_GFX_AlphaBlend(DRV_COLOR_MODE_T rColorMode,
    DRV_GFX_BLEND_T* prBlend);

extern DRV_ERR_CODE_T DRV_GFX_Flush(void);

extern DRV_ERR_CODE_T DRV_GFX_Wait(void);

extern DRV_ERR_CODE_T DRV_GFX_TransBitBlt(DRV_COLOR_MODE_T rColorMode,
    DRV_GFX_TRANSBLIT_T* prBlit);

//
// Demux
//

extern DRV_ERR_CODE_T DRV_DMX_PSI_AllocateFilter(DRV_DMX_FILTER_T* prFilter,
    UINT8* pu1FilterId);

extern DRV_ERR_CODE_T DRV_DMX_PSI_ReleaseFilter(UINT8 u1FilterId);

extern DRV_ERR_CODE_T DRV_DMX_PSI_StartFilter(UINT8 u1FilterId);

extern DRV_ERR_CODE_T DRV_DMX_PSI_StopFilter(UINT8 u1FilterId);

extern DRV_ERR_CODE_T DRV_DMX_PSI_GetMessage(UINT8 u1MsgId,
    DRV_DMX_MSG_T* prMsg);

extern DRV_ERR_CODE_T DRV_DMX_PSI_CopyData(UINT8 u1FilterId,
    DRV_DMX_DATA_T* prData, UINT8* pu1Buf);

extern DRV_ERR_CODE_T DRV_DMX_PSI_UnlockData(UINT8 u1FilterId,
    DRV_DMX_DATA_T* prData);



extern DRV_ERR_CODE_T DRV_DMX_PES_AllocateBuffer(DRV_DMX_PES_T* prPes,
    UINT8* pu1Pidx);

extern DRV_ERR_CODE_T DRV_DMX_PES_ReleaseBuffer(UINT8 u1Pidx);

extern DRV_ERR_CODE_T DRV_DMX_PES_Start(UINT8 u1Pidx);

extern DRV_ERR_CODE_T DRV_DMX_PES_Stop(UINT8 u1Pidx);

extern DRV_ERR_CODE_T DRV_DMX_PES_GetMessage(UINT32 u4MsgId,
    DRV_DMX_MSG_T* prMsg);

extern DRV_ERR_CODE_T DRV_DMX_PES_CopyData(UINT8 u1Pidx,
    DRV_DMX_DATA_T* prData, UINT8* pu1Buf);

extern DRV_ERR_CODE_T DRV_DMX_PES_UnlockData(UINT8 u1Pidx,
    DRV_DMX_DATA_T* prData);


//
// Tuner
//

extern DRV_ERR_CODE_T DRV_TUNER_SetParameter(DRV_TUNER_PARAM_T* prParam);

extern DRV_ERR_CODE_T DRV_TUNER_SetFreq(UINT32* pu4Freq,
    DRV_TUNER_CONN_TYPE_T rType, DRV_TV_SYS_T rTvSys, BOOL fgAutoFineTune, void* pvTag);

extern DRV_ERR_CODE_T DRV_TUNER_GetFreq(UINT32* pu4Freq,
    INT32 i4Tag);

extern DRV_ERR_CODE_T DRV_TUNER_GetLockStatus(DRV_TUNER_LOCK_STATUS_T* prStatus,
	INT32 i4Tag);

extern DRV_ERR_CODE_T DRV_TUNER_GetInfo(DRV_TUNER_INFO_T* prInfo);

//
// IR RX
//

extern DRV_ERR_CODE_T DRV_IR_SetParameter(DRV_IR_PARAM_T* prParam);

extern DRV_ERR_CODE_T DRV_IR_Start(void);

extern DRV_ERR_CODE_T DRV_IR_Stop(void);

extern DRV_ERR_CODE_T DRV_IR_GetKey(DRV_IR_EVENT_T* prEvent,
    UINT32 u4Tag);

//
// USB Mass storage function.
//

extern DRV_ERR_CODE_T DRV_USB_Init(DRV_USB_INIT_PARAM_T* prParam);

extern DRV_ERR_CODE_T DRV_USB_IoCtl(DRV_USB_IOCTL_PARAM_T* prParam);

extern DRV_ERR_CODE_T DRV_USB_Write(DRV_USB_TRANS_PARAM_T* prParam);

extern DRV_ERR_CODE_T DRV_USB_Read(DRV_USB_TRANS_PARAM_T* prParam);

//
// System related
//
extern DRV_ERR_CODE_T DRV_SYS_ReadEep(UINT64 u8Offset,
    UINT32 u4MemPtr, UINT32 u4MemLen);

extern DRV_ERR_CODE_T DRV_SYS_WriteEep(UINT64 u8Offset,
    UINT32 u4MemPtr, UINT32 u4MemLen);

extern DRV_ERR_CODE_T DRV_SYS_ReadNor(UINT32 u4Offset,
    UINT32 u4MemPtr, UINT32 u4MemLen);

extern DRV_ERR_CODE_T DRV_SYS_WriteNor(UINT32 u4Offset,
    UINT32 u4MemPtr, UINT32 u4MemLen);

extern DRV_ERR_CODE_T DRV_SYS_EraseNor(void);

extern DRV_ERR_CODE_T DRV_SYS_SetWakeup(DRV_SYS_WAKEUP_T* prWakeupSetting);

extern DRV_ERR_CODE_T DRV_SYS_SetTime(const UINT64 u8Utc);

extern DRV_ERR_CODE_T DRV_SYS_GetTime(const UINT64 *pu8Utc);

extern DRV_ERR_CODE_T DRV_SYS_SetAlarm(const UINT64 u8Utc);

extern void DRV_SYS_Reboot(void);

extern void DRV_SYS_Down(void);

extern void* DRV_SYS_AllocDmaMemory(UINT32 u4Size);

extern void* DRV_SYS_AllocAlignedDmaMemory(UINT32 u4Size,
    UINT32 u4Alignment);

extern BOOL DRV_SYS_FreeDmaMemory(void* pvBuf);

extern BOOL DRV_SYS_FreeAlignedDmaMemory(void* pvBuf);

extern void DRV_SYS_FlushInvalidateCache(void);

#endif  // X_DRV_IF_H

