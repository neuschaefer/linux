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
 * $RCSfile: b2r_drvif.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_if.h
 *  Video plane interface, including init, enable,
 *  mode selection and region desciption.
 */

#ifndef B2R_DRVIF_H
#define B2R_DRVIF_H

#include "x_typedef.h"
#include "stc_drvif.h"
#include "fbm_drvif.h"
#include "b2r_if.h"

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

// 0~3 for MPEG, 4 for Video Input, 5 for DV
#define MAX_ES_NS                    6

#define B2R_MAX_INPORT_NS           1    ///< Maximum number of input port for each video plane.

// Trick Play at 20081001
#define VDP_PLAY_NORMAL 0 ///< Normal Play
#define VDP_PLAY_TRICK 1 ///< Trick Play
#define VDP_PLAY_STEP_FORWARD 2 ///< Step Forward
#define VDP_PLAY_SLOW_FORWARD 3 ///< Slow Forward

// B2R Video plane mode.
#define VDP_MODE_NORMAL             0    ///< normal mode, video out
#define VDP_MODE_BG                 1    ///< background mode, mute
#define VDP_MODE_QV                 2    ///< obsolete
#define VDP_MODE_DEINT              3    ///< obsolete
#define VDP_MODE_UNKNOWN            4

// Maximum number of B2R module
#define B2R_NS                      2
#define B2R_1                       0
#define B2R_2                       1

// Video plane status
#define VDP_STATUS_NOSIGNAL         0
#define VDP_STATUS_NOSUPPORT        1
#define VDP_STATUS_UNKNOWN          2
#define VDP_STATUS_STABLE           3

// Aspect Ratio
#define VDP_UNIT_ASPECT_RATIO       0x10000
#define VDP_4_3_ASPECT_RATIO        ((B2R_UNIT_ASPECT_RATIO * 4) / 3)
#define VDP_16_9_ASPECT_RATIO       ((B2R_UNIT_ASPECT_RATIO * 16) / 9)
#define VDP_221_1_ASPECT_RATIO      ((B2R_UNIT_ASPECT_RATIO * 221) / 100)

// B2R Mode
#define VDP_B2R_MODE_UNKNOWN        0
#define VDP_B2R_MODE_480I_60        1
#define VDP_B2R_MODE_576I_50        2
#define VDP_B2R_MODE_1080I_60       3
#define VDP_B2R_MODE_1080I_50       4
#define VDP_B2R_MODE_480P_24        5
#define VDP_B2R_MODE_480P_30        6
#define VDP_B2R_MODE_480P_48        7
#define VDP_B2R_MODE_480P_60        8
#define VDP_B2R_MODE_480P_120       9
#define VDP_B2R_MODE_576P_25        10
#define VDP_B2R_MODE_576P_50        11
#define VDP_B2R_MODE_576P_100       12
#define VDP_B2R_MODE_720P_24        13
#define VDP_B2R_MODE_720P_25        14
#define VDP_B2R_MODE_720P_30        15
#define VDP_B2R_MODE_720P_48        16
#define VDP_B2R_MODE_720P_50        17
#define VDP_B2R_MODE_720P_60        18
#define VDP_B2R_MODE_720P_100       19
#define VDP_B2R_MODE_720P_120       20
#define VDP_B2R_MODE_1080P_24       21
#define VDP_B2R_MODE_1080P_25       22
#define VDP_B2R_MODE_1080P_30       23
#define VDP_B2R_MODE_1080P_48       24
#define VDP_B2R_MODE_1080P_50       25
#define VDP_B2R_MODE_1080P_60       26
#define VDP_B2R_MODE_1080I_120      27
#define VDP_B2R_MODE_1440p_24       28
#define VDP_B2R_MODE_1440p_60       29
#define VDP_B2R_MODE_21_9_30        30



//B2R clock mode
#define VDP_B2R_CLK_UNKNOWN         0
#define VDP_B2R_CLK_27_000          1   // 27.000 MHz: for 23.97Hz, 29.97Hz, 59.94Hz, 25Hz, 50Hz
#define VDP_B2R_CLK_27_027          2   // 27.027 MHz: for 24Hz, 30Hz, 60Hz
#define VDP_B2R_CLK_74_176          3   // 75.176 MHz: for 23.97Hz, 29.97Hz, 59.94Hz
#define VDP_B2R_CLK_74_250          4   // 75.250 MHz: for 24Hz, 25Hz, 30Hz, 50Hz, 60Hz
#define VDP_B2R_CLK_148_352         5   // 148.352 MHz: for 23.97Hz, 29.97Hz, 59.94Hz
#define VDP_B2R_CLK_148_500         6   // 148.500 MHz: for 24Hz, 25Hz, 30Hz, 50Hz, 60Hz
#define VDP_B2R_CLK_54_000          7
#define VDP_B2R_CLK_54_054          8

// Video plane status
#define VDP_B2R_SOURCE_CHG          0
#define VDP_B2R_OUTPUT_CHG          1
#define VDP_B2R_START_PLAY          2
#define VDP_B2R_RESOLUTION_NFY      3
#define VDP_B2R_NO_SIGNAL           4

// Video Plane COMMAND (Max 256)
#define VDP_CMD_RST                 0
#define VDP_CMD_SET_ENABLE          1
#define VDP_CMD_SET_MODE            2
#define VDP_CMD_SET_INPUT           3
#define VDP_CMD_SET_OUTPUT          4
#define VDP_CMD_SET_SRCR            5
#define VDP_CMD_SET_OUTR            6
#define VDP_CMD_SET_BG              7
#define VDP_CMD_SET_SRCSIZE         8
#define VDP_CMD_MAX                 9

/*  B2R picture information for MM*/
typedef struct
{
    UCHAR ucHours;
    UCHAR ucMinutes;
    UCHAR ucSeconds;
    UCHAR ucPictures;
    UINT32 u4Pts;
    UINT32 u4TotlaTimeOffset;
    UINT32 u4TickNum;
    UINT64 u8Offset;     // file offset
    UINT64 u8OffsetDisp; // file offset for display

} VDP_PIC_INFO_T;

/*  B2R picture information for MM*/
typedef struct
{
    UINT32 u4TickNum;

} VDP_SET_PIC_INFO_T;

/*  B2R Notify Conditions for MM*/
typedef enum
{
    VDP_COND_ERROR = -1,
    VDP_COND_EOS,
    VDP_COND_SYNC_POINT,
    VDP_COND_RANGE_AB_DONE,
    VDP_COND_CHAP_DONE = 5
}  VDP_COND_T;

/**
 *  PTS Reach Notify Function Type
 */
typedef void (*PTS_CB_FUNC)(UINT32 u4VdpId, UINT32 u4Pts, UINT32 u4Arg);

/**
 *  AFD/Aspect Ratio Change Notify Function Type
 */
typedef void (*AFD_CFG_CB_FUNC)(UINT32 u4VdpId, UINT32 u4Afd, FBM_ASPECT_RATIO_T* rAspectRatio);

/**
 *  Video Play Done Notify Function Type
 */
typedef void (*PLAYDONE_CFG_CB_FUNC)(UINT32 u4VdpId, UINT32 u4Arg1, UINT32 u4Arg2);

/**
 *  UnMute Status Change Notify Function Type
 */
typedef void (*UNMUTE_CFG_CB_FUNC)(UINT32 u4VdpId, BOOL fgUnMute);


/**
 *  Video Plane Notify Function Type for MM
 */
#ifdef CC_53XX_SWDMX_V2
typedef void (*VDP_MM_CB_FUNC)(UINT32 u4VdpId, VDP_COND_T eCond, BOOL fgFastForward, UINT32 u4AttachedSrcId);
#else
typedef void (*VDP_MM_CB_FUNC)(UINT32 u4VdpId, VDP_COND_T eCond, BOOL fgFastForward);
#endif

/**
 *  Video Plane Lip Sync Notify
 */
typedef void (*VDP_LIPSYNC_CB_FUNC)(void);

#define VDP_SEEK_STEP_OK         0
#define VDP_SEEK_STEP_NO_DATA    1
/**
 *  Video Plane Step Forward Finish Notify
 */
#ifdef CC_53XX_SWDMX_V2
typedef void (*VDP_STEP_FIN_CB_FUNC)(UINT32 u4VdpId, UINT32 u4Pts, UINT32 u4Ret, UINT32 u4AttachedSrcId);
#else
typedef void (*VDP_STEP_FIN_CB_FUNC)(UINT32 u4VdpId, UINT32 u4Pts, UINT32 u4Ret);
#endif

/**
 *  Video Plane Seek Finish Notify
 */
#ifdef CC_53XX_SWDMX_V2
typedef void (*VDP_SEEK_FIN_CB_FUNC)(UINT32 u4AttachedSrcId, UINT32 u4VdpId, UINT32 u4Pts, UINT32 u4Ret);
#else
typedef void (*VDP_SEEK_FIN_CB_FUNC)(UINT32 u4VdpId, UINT32 u4Pts, UINT32 u4Ret);
#endif

/**
 *  Video Plane Repeat/Drop Notify
 */
typedef void (*VDP_REPEAT_DROP_CB_FUNC)(UINT32 u4VdpId, UINT32 u4RepeatDrop);

/**
 *  B2R Video plane image module, Capability infomation
 */
typedef struct
{
    UINT32 u4Width;             ///< width of maximum supported image width
    UINT32 u4Height;            ///< height of maximum supported image height
    UINT32 u4ColorMode;         ///< supported color mode
    UINT32 u4FrameBufferNs;     ///< supported number for Frame Buffer
    UINT32 u4PitchAlignement;   ///< supported alignement step for Pitch
} VDP_IMAGE_INFO_T;

/**
 *  Video plane image module, Image Argument
 */
typedef struct
{
    UINT32 u4FrameBufferId;     ///< frame buffer id of image
    UINT32 u4Width;             ///< width of imge
    UINT32 u4Height;            ///< height of image
    UINT32 u4ColorMode;         ///< color mode of image
    UINT32 u4Pitch;             ///< Pitch of image
} VDP_IMAGE_ARG_T;

// B2R Video Image Module Color Mode.
#define VDP_IMAGE_420_MODE          (1 << 0)    ///< 420 mode
#define VDP_IMAGE_422_MODE          (1 << 1)    ///< 422 mode
#define VDP_IMAGE_444_MODE          (1 << 2)    ///< 424 mode

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

#ifdef CC_53XX_SWDMX_V2
EXTERN void _VdpCheckFbgReady(UCHAR ucFbgId, UCHAR ucEsId);
#else
EXTERN void _VdpCheckFbgReady(void);
#endif

EXTERN void _VdpFbgChgNotify(UCHAR ucFbgId, UCHAR ucEsId);

EXTERN void _B2rSetEnable(UCHAR ucVdpId);

EXTERN void B2R_SetEs2Fbg(UCHAR ucEsId, UCHAR ucFbgId);

EXTERN UCHAR B2R_GetEs2Fbg(UCHAR ucEsId);

EXTERN void _VDP_StatusNofify(UCHAR ucVdpId, UINT32 u4Status);

EXTERN void _VDP_EventNofify(UCHAR ucVdpId, UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3);

EXTERN void VDP_ResetPts(void);

EXTERN void VDP_FrcSetFreeze(UCHAR ucVdpId, UINT32 u4OnOff);

EXTERN void VDP_B2rSwitch(UCHAR ucVdpId, UCHAR ucB2rId);

EXTERN UCHAR VDP_Vdp2B2r(UCHAR ucVdpId);

EXTERN UCHAR VDP_B2r2Vdp(UCHAR ucB2rId);

EXTERN UINT32 VDP_Es2Vdp(UCHAR ucEsId);

EXTERN void VDP_Pattern(UCHAR ucVdpId, UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3);

EXTERN UCHAR _VDP_GetOutFrameRate(UCHAR ucVdpId);

EXTERN void VDP_FrcSetPtsCb(UCHAR ucVdpId, UCHAR ucPort, UINT32 u4CbPts, UINT32 u4CbArg);

EXTERN void VDP_FrcSetLipSyncCb(UCHAR ucVdpId, UCHAR ucPort, UINT32 u4Thrsd, UINT32 u4FrmCnt);

EXTERN void VDP_FrcSetLipSyncNGOKCb(UCHAR ucNgOk, UCHAR ucRemove, UINT32 u4Thrsd, UINT32 u4FrmCnt);

EXTERN void _VDP_VsyncReset(UCHAR ucVdpId);


///-----------------------------------------------------------------------------
/// Public functions
///-----------------------------------------------------------------------------

EXTERN void VDP_CheckFbgReady(void);

EXTERN void VDP_FrcResetPort(UCHAR ucVdpId, UCHAR ucPort);

/**
 * VDP query function, show DTV status in CLI with debug level 1.
 *
 * @param void
 * @return void
 */
EXTERN void VDP_PrintDtvInfo(void);

/**
 * Get mute status of DTV playback
 *
 * @param ucVdpId specify the video plane id.
 * @return TRUE for mute, FALSE for not mute
 */
EXTERN BOOL VDP_GetUnMuteStatus(UCHAR ucVdpId);

/**
 * Video Image Module : Get information
 *
 * @param void
 * @return pointer of Video Image Module Capability Infomation
 */
EXTERN VDP_IMAGE_INFO_T* VDP_Image_GetInfo(void);

/**
 * Video Image Module : Connect
 *
 * @param u4VdpId: Video plane connected to
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
EXTERN UINT32 VDP_Image_Connect(UINT32 u4VdpId);

/**
 * Video Image Module : Disconnect
 *
 * @param void
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
EXTERN UINT32 VDP_Image_Disconnect(void);

/**
 * Video Image Module : Mute on/off
 *
 * @param u4OnOff: Mute on/off
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
EXTERN UINT32 VDP_Image_Mute(UINT32 u4OnOff);

/**
 * Video Image Module : Show Frame Buffer
 *
 * @param prInfo: Frame Buffer Info
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
EXTERN UINT32 VDP_Image_Show(VDP_IMAGE_ARG_T* prInfo);

/**
 * Video Image Module : Get Frame Buffer Address
 *
 * @param u4FrameBufferId: Frame Buffer id
 * @param u4YAddr: pointer to store Y address
 * @param u4CAddr: pointer to store C address
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
EXTERN UINT32 VDP_Image_GetFrameBuffer(UINT32 u4FrameBufferId, UINT32* u4YAddr, UINT32* u4CAddr);

EXTERN void VDP_SetB2rUpdateStc(UINT8 u1B2rId,BOOL fgEnable);
EXTERN BOOL VDP_GetB2rUpdateStc(UINT8 u1B2rId);
EXTERN BOOL VDP_Is4To1024XSpeed(UINT32 u4Speed);

EXTERN void VDP_SetB2rStc(UINT8 u1B2rId,UINT32 u4B2rStc);
EXTERN UINT32 VDP_GetB2rStc(UINT8 u1B2rId);
EXTERN INT64 VDP_GetAvsyncDelta(void);

EXTERN UINT32 VDP_SetFrmCrop(UCHAR ucVdpId, UINT32 u4FrmCropTop, UINT32 u4FrmCropBtm);

EXTERN UINT32 VDP_GetPicInfo(UCHAR ucVdpId, VDP_PIC_INFO_T* prPicInfo);
EXTERN UINT32 VDP_SetPicInfo(UCHAR ucVdpId, VDP_SET_PIC_INFO_T rPicInfo);

EXTERN BOOL VDP_GetFilePosition(UCHAR ucVdpId,
                                INT32 *pi4TempRef, UINT16 *pu2DecodingOrder,
                                UINT64 *pu8Offset, UINT64 *pu8OffsetI);

#define B2R_GETPOSINFO_AVAILABLE // for different version code complier OK.
EXTERN BOOL VDP_GetPosInfo(UCHAR ucVdpId, VDP_POS_INTO_T *prPosInfo);

EXTERN BOOL VDP_SetABInfo(UCHAR ucVdpId, VDP_AB_INTO_T *prAB);

#ifdef CC_B2R_SKYPE_SUPPORT
EXTERN UCHAR VDP_GetPendingFB(UCHAR ucVdpId);
EXTERN void VDP_CleanPendingFB(UCHAR ucVdpId);
EXTERN void VDP_SetReleaseDispQ(UCHAR ucVdpId);
EXTERN UINT32 VDP_GetB2RFifoStatus(UCHAR ucVdpId);
EXTERN void VDP_ResetB2RFifoStatus(UCHAR ucVdpId);
EXTERN void VDP_GetYCStart(UCHAR ucVdpId,UINT32 *pu4YStart, UINT32 *pu4CStart, UINT32 *pu4LineSize, UINT32 *pu4VActive);
#else
EXTERN UCHAR VDP_GetPendingFB(void);
EXTERN void VDP_CleanPendingFB(void);
EXTERN void VDP_SetReleaseDispQ(void);
EXTERN UINT32 VDP_GetB2RFifoStatus(void);
EXTERN void VDP_ResetB2RFifoStatus(void);
EXTERN void VDP_GetYCStart(UINT32 *pu4YStart, UINT32 *pu4CStart, UINT32 *pu4LineSize, UINT32 *pu4VActive);
#endif

EXTERN void VDP_VsyncMutexLock(void);

EXTERN void VDP_VsyncMutexUnlock(void);

EXTERN void VDP_SetWaitSTC(UCHAR ucVdpId, UCHAR ucSet);

EXTERN UCHAR VDP_GetWaitSTC(UCHAR ucVdpId);

EXTERN void VDP_SetChkPTS(UINT32 u4Pts);
EXTERN void VDP_SetEncTrigger(BOOL fgEnable);

EXTERN void VDP_PowerOffB2R(void);

EXTERN void VDP_TestB2RSdPath(void);

EXTERN UCHAR VDP_GetPlayMode(UCHAR ucVdpId);


/* VDP trigger mode + audio master to do trigger mode */
EXTERN void VDP_TriggerAudReceive(UINT32 u4Pts);

EXTERN void B2R_InitVdpConf(void);

EXTERN void B2R_SetImgConnect(UCHAR ucVdpId, UCHAR ucConnect);

EXTERN UCHAR B2R_GetImgConnect(UCHAR ucVdpId);

EXTERN void VDP_VsyncInit(void);

EXTERN void VDP_Image_Init(void);

EXTERN void VDP_VsyncSendCmd(UCHAR ucVdpId, UCHAR ucCmdId);

EXTERN void VDP_VsyncReset(UCHAR ucVdpId);

EXTERN void VDP_HalB2rSwitch(UCHAR ucVdpId, UCHAR ucB2rId);

EXTERN UINT32 VDP_Set3DInfo(UCHAR u1B2rId,UINT8 u13DType,BOOL fg3DEnable);

EXTERN void VDP_B2RDumpRegister(BOOL fgOn);

EXTERN UINT32 VDP_SetPauseMm(UCHAR ucVdpId, UCHAR ucPause);

#endif    // B2R_DRVIF_H

