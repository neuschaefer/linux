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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: playmgr.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file playmgr_util.h
 *  This file contains implementation of exported APIs of play manager's utility.
 */
#ifndef PLAYMGR_H
#define PLAYMGR_H

#include "x_assert.h"
#include "u_handle.h"
#include "playmgr_if.h"
#include "dmx_if.h"
#include "feeder_if.h"
#include "swdmx_if.h"
#include "x_common.h"
#include "x_typedef.h"
#include "x_mm_common.h"
#include "x_minfo.h"
#include "x_midxbuld.h"
#include "x_mfmtrecg.h"
#include "u_minfo.h"
#include "u_midxbuld.h"
#include "u_mfmtrecg.h"
#include "x_mfmtrecg_avi.h"
#include "x_mfmtrecg_asf.h"
#include "x_mfmtrecg_mp3.h"
#include "x_mfmtrecg_mp4.h"
#include "x_mfmtrecg_mkv.h"
#include "x_mfmtrecg_ps.h"
#include "x_mfmtrecg_ts.h"
#include "x_mfmtrecg_rm.h"
#include "x_mfmtrecg_vid_es.h"
#include "x_mfmtrecg_aif.h"
#include "x_mfmtrecg_ogg.h"
#include "x_mfmtrecg_aud_es.h"
#include "x_mfmtrecg_flv.h"
#include "x_minfo_avi.h"
#include "x_minfo_ogg.h"
#include "x_minfo_mp4.h"
#include "x_minfo_asf.h"
#include "x_minfo_mp3.h"
#include "x_minfo_ps.h"
#include "x_minfo_mkv.h"
#include "x_minfo_rm.h"
#include "x_minfo_aif.h"
#include "x_minfo_aud_es.h"
#include "x_minfo_flv.h"
#include "x_midxbuld_asf.h"
#include "x_midxbuld_mp4.h"
#include "x_midxbuld_avi.h"
#include "x_midxbuld_mkv.h"
#include "x_midxbuld_rm.h"
#include "x_midxbuld_mp3.h"
#include "x_midxbuld_flv.h"
#include "x_divx_drm.h"
#include "u_divx_drm.h"


/********************************************************************
  MACRO CONSTANT DEFINITIONS
********************************************************************/
#define MAX_PATH 256
#define PLAYMGR_CMD_Q_SIZE 10
#define PLAYMGR_DATA_Q_SIZE 8
#define MAX_RANGE_NUM 2
#define AVI_MAX_AUD_TRACK 8
#define PLAYMGR_MKV_MAX_AUD_TRACK 15
#define RM_MAX_AUD_TRACK 1
#define AVI_MAX_SUB_TRACK 8

#define PLAYMGR_STATUS_UPDATE_TIMER 1000
#define PLAYMGR_NOTIFICATION_TIMEOUT 50000

#define PLAYMGR_SUBTITLE_BUFSIZE (64 * 1024)
#define PLAYMGR_FILENAME_LENGTH (1024)

#define AVI_MAX_KEY_EXT 16
#define MKV_MAX_KEY_EXT 16
/********************************************************************
  MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
  TYPE DEFINITION
********************************************************************/
typedef enum
{
    PLAYMGR_STATE_CLOSE,
    PLAYMGR_STATE_OPEN,
    PLAYMGR_STATE_STOP,
    PLAYMGR_STATE_PLAY,
    PLAYMGR_STATE_PAUSE,
    PLAYMGR_STATE_MAX
} PLAYMGR_STATE_T;

typedef enum
{
    PLAYMGR_SETCMD_INIT,
    PLAYMGR_SETCMD_OPEN,
    PLAYMGR_SETCMD_CLOSE,
    PLAYMGR_SETCMD_START,
    PLAYMGR_SETCMD_STOP,
    PLAYMGR_SETCMD_PAUSE,
    PLAYMGR_SETCMD_RESUME,
    PLAYMGR_SETCMD_TRICK,
    PLAYMGR_SETCMD_SEEKTIME,
    PLAYMGR_SETCMD_SEEKPOSITION,
    PLAYMGR_SETCMD_SEEKPERCENT,
    PLAYMGR_SETCMD_ABORT,
    PLAYMGR_SETCMD_UPDATEPLAYTIME,
    PLAYMGR_SETCMD_UPDATESUBTITLE,
    PLAYMGR_SETCMD_STEP,
    PLAYMGR_SETCMD_ABREPEAT,
    PLAYMGR_SETCMD_MAX
} PLAYMGR_SETCMD_T;

typedef struct
{
    PLAYMGR_SETCMD_T eCmd;
    UINT32 u4Param1;
    UINT32 u4Param2;
    UINT32 u4Param3;
    UINT32 u4Param4;
} PLAYMGR_STATUS_CMD_T;

typedef BOOL (*PFN_PLAYMGR_Start)(VOID* pvMediaInfo);

typedef BOOL (*PFN_PLAYMGR_Stop)(VOID* pvMediaInfo);

typedef BOOL (*PFN_PLAYMGR_Close)(VOID* pvMediaInfo);

typedef BOOL (*PFN_PLAYMGR_Pause)(VOID);

typedef BOOL (*PFN_PLAYMGR_Resume)(VOID* pvMediaInfo);

typedef BOOL (*PFN_PLAYMGR_Open)(
    //PLAYMGR_STATUS_T* prMediaStatus,
    VOID* pvMediaInfo);

typedef BOOL (*PFN_PLAYMGR_OpenMediaInfo)(
    PLAYMGR_STATUS_T* prMediaStatus,
    VOID* pvMediaInfo);

typedef BOOL (*PFN_PLAYMGR_UpdateRange)(VOID* pvMediaInfo, UINT32 u4Param1);

typedef BOOL (*PFN_PLAYMGR_FreeRange)(VOID* pvMediaInfo, UINT32 u4RangeTag);

typedef struct
{
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4XPos;
    UINT32 u4YPos;
    UINT32 u4StartPts;
    UINT32 u4EndPts;
    UINT32 au4ARGB[4];
    UINT32 u4WindowWidth;
    UINT32 u4WindowHeight;
} PLAYMGR_SUBTITLE_INFO_T;

typedef struct
{
    UINT32 u4DrmType;
    DIVX_DRM_BASIC_INFO_T* prDivxDrmBasicInfo;
    DRM_OUTPUT_SIGNAL_PROTECTION_INFO_T* prDrmOutputSignalProtectionInfo;
    DIVX_DRM_CRYPTO_INFO_T * prDrmCryptoInfo;
} PLAYMGR_DIVX_DRM_INFO_T;

// Data Structures for AVI
typedef struct
{
    UINT16 u2VidIdx;
    UINT16 u2AudIdx;
    UINT16 u2SubIdx;
    //PLAYMGR_SETTING_T rPlayerSetting;
    MINFO_TYPE_GENERIC_T rPlayerGenericInfo;
    MINFO_TYPE_IDX_INFO_T rPlayerIdxInfo;
    MINFO_TYPE_STM_ATRBT_T rStmAttribute[17];

    HANDLE_T hMediaInfoHandle;
    HANDLE_T hMediaIdxHandle;
    HANDLE_T hDivXDrmHandle;
    MIDXBULD_KEY_TBL_INFO_T rTableInfo;
    MIDXBULD_SOURCE_INFO_T rSrcInfo;
    MIDXBULD_KEY_INFO_T rKeyInfo;
    MIDXBULD_ELMT_TBL_ALLOC_INFO_T rAllocTblInfo[1];

    MIDXBULD_ELMT_TBL_DESCRIB_T rVDescrib[1];
    MIDXBULD_ELMT_TBL_DESCRIB_T rADescrib[1];

    MM_RANGE_ELMT_IDXTBL_T rKeyIdxTBL;
    MM_RANGE_ELMT_IDXTBL_T rKeyIdxTBLExt[AVI_MAX_KEY_EXT];  //need refine the max value
    MM_RANGE_ELMT_IDXTBL_T rAudKeyIdxTBL[AVI_MAX_AUD_TRACK];
    MM_RANGE_ELMT_IDXTBL_T rAudKeyIdxTBLExt[AVI_MAX_AUD_TRACK][AVI_MAX_KEY_EXT];

    MM_RANGE_ELMT_IDXTBL_T rVidIDXTBL[1];
    MM_RANGE_ELMT_IDXTBL_T rAudIDXTBL[1];

    PLAYMGR_DRMTYPE eDRMType;
    UINT32 u4DRMRentalLimit;
    UINT32 u4DRMRentalCount;
    
    PLAYMGR_DIVX_DRM_INFO_T rDivXDrmInfo;

    //meta data information
    MINFO_TYPE_META_DATA_T rAviMetaData;
} PLAYMGR_AVI_INFO_T;


typedef struct
{
    UINT16 u2VidIdx;
    UINT16 u2AudIdx;
    //PLAYMGR_SETTING_T rPlayerSetting;
    BOOL  b_HasKeyframes;
    MINFO_TYPE_GENERIC_T rPlayerGenericInfo;
    MINFO_TYPE_IDX_INFO_T rPlayerIdxInfo;
    MINFO_TYPE_STM_ATRBT_T rStmAttribute[2];
    HANDLE_T hMediaInfoHandle;
    HANDLE_T hMediaIdxHandle;
    //HANDLE_T hDivXDrmHandle;
    MIDXBULD_KEY_TBL_INFO_T rTableInfo;
    MIDXBULD_SOURCE_INFO_T rSrcInfo;
    MIDXBULD_KEY_INFO_T rKeyInfo;
    MIDXBULD_ELMT_TBL_DESCRIB_T rVDescrib[1];
    MIDXBULD_ELMT_TBL_DESCRIB_T rADescrib[1];

    MM_RANGE_ELMT_IDXTBL_T rKeyIdxTBL;
} PLAYMGR_FLV_INFO_T;

typedef struct
{
    UINT16 u2VidIdx;
    UINT16 u2AudIdx;

    MINFO_TYPE_GENERIC_T rPlayerGenericInfo;
    MINFO_TYPE_IDX_INFO_T rPlayerIdxInfo;
    MINFO_TYPE_STM_ATRBT_T rStmAttribute[17];
    HANDLE_T hMediaInfoHandle;
    HANDLE_T hMediaIdxHandle;
    MIDXBULD_SOURCE_INFO_T rSrcInfo;

    MM_RANGE_ELMT_IDXTBL_T rVidIDXTBL[1];
    MM_RANGE_ELMT_IDXTBL_T rAudIDXTBL[1];

    MIDXBULD_ELMT_TBL_DESCRIB_T rVDescrib[1];
    MIDXBULD_ELMT_TBL_DESCRIB_T rADescrib[1];

} PLAYMGR_OGG_INFO_T;


typedef struct
{
    HANDLE_T hWmvMediaInfoHandle;
    HANDLE_T hMediaIdxHandle;

    UINT16 u2VidIdx;
    UINT16 u2AudIdx;
    UINT8 u1VideoStreamIdx;
    UINT8 u1AudioStreamIdx;

    MINFO_TYPE_GENERIC_T rWmvGenericInfo[10];
    MINFO_TYPE_IDX_INFO_T rWmvIdxInfo;
    MINFO_TYPE_STM_ATRBT_T rStmAttribute[10][17];

    //meta data information
    MINFO_TYPE_META_DATA_T rWmvMetaData;

    //index builder
    MIDXBULD_ELMT_TBL_INFO_T rWmvTableInfo;
    MIDXBULD_SOURCE_INFO_T rWmvSrcInfo;
    MIDXBULD_KEY_INFO_T rWmvKeyInfo;

    MM_RANGE_ELMT_IDXTBL_T rWmvKeyIDXTBL;

} PLAYMGR_WMV_INFO_T;


// Data Structures for Mp4
typedef struct
{
    BOOL fgLastRange;

    UINT16 u2VidIdx;
    UINT16 u2AudIdx;

    MINFO_TYPE_GENERIC_T rPlayerGenericInfo;
    MINFO_TYPE_IDX_INFO_T rPlayerIdxInfo;
    MINFO_TYPE_STM_ATRBT_T rStmAttribute[17];

    HANDLE_T hMediaInfoHandle;
    HANDLE_T hMediaIdxHandle;
    MIDXBULD_ELMT_TBL_INFO_T rTableInfo;
    MIDXBULD_KEY_TBL_INFO_T rKeyTableInfo;
    MIDXBULD_SOURCE_INFO_T rSrcInfo;
    MIDXBULD_KEY_INFO_T rKeyInfo;

    MIDXBULD_ELMT_TBL_ALLOC_INFO_T rAllocTblInfo[MAX_RANGE_NUM];

    MIDXBULD_ELMT_TBL_DESCRIB_T rVDescrib[MAX_RANGE_NUM];
    MIDXBULD_ELMT_TBL_DESCRIB_T rADescrib[MAX_RANGE_NUM];

    MM_RANGE_ELMT_IDXTBL_T rKeyIdxTBL;

    MM_RANGE_ELMT_IDXTBL_T rVidIDXTBL[MAX_RANGE_NUM];
    MM_RANGE_ELMT_IDXTBL_T rAudIDXTBL[MAX_RANGE_NUM];

    //meta data information
    MINFO_TYPE_META_DATA_T rMp4MetaData;
} PLAYMGR_MP4_INFO_T;

typedef struct
{
    MINFO_TYPE_GENERIC_T rPlayerGenericInfo;
    MINFO_TYPE_ID3_TAG_T rMp3Id3Data;
    /*union
    {
        MINFO_MP3_AUDIO_STM_ATRBT_T rMp3AudioAttr;
        MINFO_ES_AUDIO_STM_ATRBT_T rEsAudioAttr;
        MINFO_AIF_AUDIO_STM_ATRBT_T rAifAudioAttr;
    }AudAttr;
    */
    MINFO_TYPE_STM_ATRBT_T rStreamAttr;
    MINFO_TYPE_IDX_INFO_T rIdxInfo;
    MIDXBULD_SOURCE_INFO_T rSrcInfo;
    HANDLE_T hMediaInfoHandle;
    HANDLE_T hMediaIdxHandle;
    MIDXBULD_KEY_TBL_INFO_T rTableInfo;
    MM_RANGE_ELMT_IDXTBL_T rKeyIdxTBL;
    MIDXBULD_KEY_INFO_T rKeyInfo;
    MIDXBULD_ELMT_TBL_ALLOC_INFO_T rAllocTblInfo[MAX_RANGE_NUM];

} PLAYMGR_MP3_INFO_T;

typedef struct _MINFO_PS_INF_T
{
    UINT16 u2VidIdx;
    UINT16 u2AudIdx;
    //PLAYMGR_SETTING_T rPlayerSetting;
    MINFO_TYPE_GENERIC_T rPlayerGenericInfo;
    MINFO_TYPE_META_DATA_T rMetaDat;
    MINFO_TYPE_STM_ATRBT_T rStmAttribute[17];

    HANDLE_T hMediaInfoHandle;
    
    MM_RANGE_ELMT_IDXTBL_T rKeyIdxTBL;
    MM_RANGE_ELEMT_IDXTBL_TS_ENTRY_T rIdxTsEntry;

#ifdef TIME_SHIFT_SUPPORT
    PVR_TICK_CTRL_BLK_T rTickCtl;
    MM_REC_BUF_RANGE_INFO_T rRangeInfo;
#endif

    //HANDLE_T hMediaIdxHandle;
    //MIDXBULD_KEY_TBL_INFO_T rTableInfo;
    //MIDXBULD_SOURCE_INFO_T rSrcInfo;
    //MIDXBULD_KEY_INFO_T rKeyInfo;
    //MIDXBULD_ELMT_TBL_ALLOC_INFO_T rAllocTblInfo[MAX_RANGE_NUM];

    //MIDXBULD_ELMT_TBL_DESCRIB_T rVDescrib[MAX_RANGE_NUM];
    //MIDXBULD_ELMT_TBL_DESCRIB_T rADescrib[MAX_RANGE_NUM];

    //MM_RANGE_ELMT_IDXTBL_T rKeyIdxTBL;
    //MM_RANGE_ELMT_IDXTBL_T rAudKeyIdxTBL[AVI_MAX_AUD_TRACK];

    //MM_RANGE_ELMT_IDXTBL_T rVidIDXTBL[MAX_RANGE_NUM];
    //MM_RANGE_ELMT_IDXTBL_T rAudIDXTBL[MAX_RANGE_NUM];
} PLAYMGR_PS_INFO_T;

typedef struct
{
    UINT16 u2VidIdx;
    UINT16 u2AudIdx;
    MINFO_TYPE_GENERIC_T rPlayerGenericInfo;
    MINFO_TYPE_IDX_INFO_T rPlayerIdxInfo;
    MINFO_TYPE_STM_ATRBT_T rStmAttribute[17];
    //meta data information
    MINFO_TYPE_META_DATA_T rMetaData;

    HANDLE_T hMediaInfoHandle;
    HANDLE_T hMediaIdxHandle;
    MIDXBULD_KEY_TBL_INFO_T rTableInfo;
    MIDXBULD_SOURCE_INFO_T rSrcInfo;
    MIDXBULD_KEY_INFO_T rKeyInfo;
    MIDXBULD_ELMT_TBL_ALLOC_INFO_T rAllocTblInfo[MAX_RANGE_NUM];

    MIDXBULD_ELMT_TBL_DESCRIB_T rVDescrib[MAX_RANGE_NUM];
    MIDXBULD_ELMT_TBL_DESCRIB_T rADescrib[MAX_RANGE_NUM];

    MM_RANGE_ELMT_IDXTBL_T rKeyIdxTBL;
    MM_RANGE_ELMT_IDXTBL_T rKeyIdxTBLExt[MKV_MAX_KEY_EXT];
    MM_RANGE_ELMT_IDXTBL_T rAudKeyIdxTBL[PLAYMGR_MKV_MAX_AUD_TRACK];

    //MM_RANGE_ELMT_IDXTBL_T rVidIDXTBL[MAX_RANGE_NUM];
    //MM_RANGE_ELMT_IDXTBL_T rAudIDXTBL[MAX_RANGE_NUM];
} PLAYMGR_MKV_INFO_T;

// Data Structures for RM
typedef struct _XX_RM_SPEC_INFO_T
{
    UINT16          ui2_frm_width;
    UINT16          ui2_frm_height;
    UINT32          ui4_spo_extra_flags;
    UINT32          aui4_specific[25];
    BOOL            b_is_rm8;
} XX_RM_SPEC_INFO_T;

typedef struct
{
    UINT16 u2VidIdx;
    UINT16 u2AudIdx;
    UINT16 u2SubIdx;
    //PLAYMGR_SETTING_T rPlayerSetting;
    MINFO_TYPE_GENERIC_T rPlayerGenericInfo;
    MINFO_TYPE_IDX_INFO_T rPlayerIdxInfo;
    MINFO_TYPE_STM_ATRBT_T rStmAttribute[17];

    HANDLE_T hMediaInfoHandle;	
    HANDLE_T hMediaIdxHandle;	
    MIDXBULD_KEY_TBL_INFO_T rTableInfo;
    MIDXBULD_SOURCE_INFO_T rSrcInfo;
    MIDXBULD_KEY_INFO_T rKeyInfo;
    MIDXBULD_ELMT_TBL_ALLOC_INFO_T rAllocTblInfo[1];

    MIDXBULD_ELMT_TBL_DESCRIB_T rVDescrib[1];
    MIDXBULD_ELMT_TBL_DESCRIB_T rADescrib[1];

    MM_RANGE_ELMT_IDXTBL_T rKeyIdxTBL;
    MM_RANGE_ELMT_IDXTBL_T rAudKeyIdxTBL[RM_MAX_AUD_TRACK]; 
    	
    MM_RANGE_ELMT_IDXTBL_T rVidIDXTBL[1];
    MM_RANGE_ELMT_IDXTBL_T rAudIDXTBL[1];

    //meta data information
    MINFO_TYPE_META_DATA_T rRmMetaData;

    XX_RM_SPEC_INFO_T rRmSpecInfo;
} PLAYMGR_RM_INFO_T;


typedef enum
{
    SPU_NONE = 0,
    SPU_WAIT_DEC,
    SPU_DEC_OK,
    SPU_WAIT_DRAW_ACK,
    SPU_DRAWN
} SPU_STATE_T;

typedef struct
{
    SPU_STATE_T eState;
    BOOL fgOutBufEmpty;
    UCHAR *pucSpuBuf;
    UINT32 u4FrameAddr[5];
    UINT32 u4CopyLen[5];
    UINT8 u1SerialNumber[5];
    UINT32 u4StartPts[5];
    UINT32 u4EndPts[5];
    UINT32 u4SpuRp;
    UINT32 u4SpuWp;
    BOOL fgXsubPlus;
    UINT32 u4WindowWidth;
    UINT32 u4WindowHeight;
} SPU_STATUS_T;

typedef struct _PLAYMGR_AB_POS_INFO_T
{
    UINT64  ui8_stc;
    UINT64  ui8_aud_pts_info;
    UINT64  ui8_aud_frame_position;
    UINT64  ui8_i_pts_info;
    UINT64  ui8_pts_info;
    UINT64  ui8_i_frame_position;
    UINT64  ui8_frame_position;
    UINT64  ui8_frame_position_disp;
    INT32   i4_temporal_reference;
    UINT16  ui2_decoding_order;
}PLAYMGR_AB_POS_INFO_T;

typedef struct _PLAYMGR_RENDER_INFO_T
{
    PLAYMGR_AB_POS_INFO_T rA;
    PLAYMGR_AB_POS_INFO_T rB;
}PLAYMGR_AB_INFO_T;

/**Mplayer parameters
*/
typedef struct
{
    //BOOL fgInit;

    UINT8 u1SrcId;
    UINT8 u1StcId;
    UINT8 u1AdecId;
    UINT8 u1VdecId;
    UINT8 u1Vdec2Id;
    //UINT8 u1VdpId;
    UINT8 u1B2rId;

    BOOL fgOpen;
    
    UINT64 u8FileSize; // change to int64 later

    MEDIA_FORMAT_T rMediaFmt;

    UCHAR *pucSubBuf;

    // Sub-Title
    SPU_STATUS_T rSpuStatus;
    PLAYMGR_SUBTITLE_INFO_T rTmpSubtInfo;
    UCHAR *pucSubOutBuf;


    // Common information
    FeederSourceType eFeederSrc;

    //CHAR crFileName[MAX_PATH];
    PLAYMGR_STATUS_T rPlayerStatus;

    //for Mpg only
    //FEEDER_BUF_INFO_T rFeederInfo;
    HANDLE_T hPlaymgrFeederReqDataQ;
    SWDMX_PROGRAM_INFO_T rPlayerMpgInfo;

    UINT32 u4Tag;
    UINT32 u4StartPts;
    UINT32 u4SeekPts;
    //UINT32 u4TotalTime;

    UINT32 u4RangeID;
    UINT32 u4CurRangePTS;

    UINT8 u1CurEmptyRange;
    UINT8 u1LeftRangeNum;
    MM_RANGE_INFO_T rRangeInfo[MAX_RANGE_NUM];

    PFN_PLAYMGR_Start pfnPlayMgrStart;
    PFN_PLAYMGR_Stop pfnPlayMgrStop;
    PFN_PLAYMGR_Close pfnPlayMgrClose;
    PFN_PLAYMGR_Resume pfnPlayMgrResume;
    PFN_PLAYMGR_Pause pfnPlayMgrPause;
    PFN_PLAYMGR_OpenMediaInfo pfnPlayMgrOpenMediaInfo;
    PFN_PLAYMGR_UpdateRange pfnPlaymgrUpdateRange;
    PFN_PLAYMGR_FreeRange pfnPlaymgrFreeRange;

    MEDIA_FEEDER_T rMediaFeeder;
    UINT16 u2VidStmNum;
    UINT16 u2AudStmNum;
    UINT16 u2SubStmNum;
    UINT16 u2PcrStmNum;

    union
    {
        PLAYMGR_FLV_INFO_T rFlvInfo;
        PLAYMGR_AVI_INFO_T rAviInfo;
        PLAYMGR_WMV_INFO_T rWmvInfo;
        PLAYMGR_MP4_INFO_T rMp4Info;
        PLAYMGR_MP3_INFO_T rMp3Info;
        PLAYMGR_PS_INFO_T  rPsInfo;
        PLAYMGR_MKV_INFO_T rMkvInfo;
        PLAYMGR_RM_INFO_T  rRmInfo;
        PLAYMGR_OGG_INFO_T rOggInfo;
    } FmtInfo;

    HANDLE_T hFileHandle, hHdrHandle;
    HANDLE_T hPlayMgrCmdQ;
    HANDLE_T hPlayMgrStatusQ;
    HANDLE_T hPlayMgrDmxCtrlSema;
    HANDLE_T hPlayMgrVdecCtrlSema;
    HANDLE_T hPlayMgrVdec2CtrlSema;
    HANDLE_T hPlayMgrAdecCtrlSema;
    HANDLE_T hPlayMgrVdpCtrlSema;
    HANDLE_T hPlayMgrEosMutex;

    HANDLE_T hPlayMgrRangeCmdQ;
    HANDLE_T hPlayMgrPlaybackStatusCmdQ;

    HANDLE_T hPlaymgrUpdateRangeThread;
    HANDLE_T hPlayMgrUpdateStatusThread;

    HANDLE_T hPlaymgrStatusNotifyTimer;
    BOOL fgPlaymgrStatusNotifyTimerStarted;

    HANDLE_T hPlayMgrSema;
    HANDLE_T hPlayMgrSemaFinSeek;
    UINT32 u4AudHandle;

    HANDLE_T hPlaymgrABDoneSema;
    HANDLE_T hPlaymgrABDoneThread;
    
#ifdef PLAYMGR_ASYNC_MODE
    HANDLE_T hPlayMgrThread;
#endif

#ifdef __KERNEL__
    HANDLE_T hPlayMgrDataQ;
#endif
    CHAR szFileName[PLAYMGR_FILENAME_LENGTH];

    PLAYMGR_STATE_T ePlayMgrCurStatus;
    PLAYMGR_SourceType_T ePlayMgrSrcType;
    UINT32 u4NumberOfBitstream;

    COOK_INFO_T rCookInfo;
    PLAYMGR_AB_INFO_T rABInfo;
} PLAYMGR_INFO_T;


/********************************************************************
  EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
EXTERN BOOL _PLAYMGR_ReceiveNotify(UINT8 u1SrcId, VOID *pvData);
EXTERN BOOL _PLAYMGR_SendNotify(UINT8 u1SrcId, UINT32 u4State);
EXTERN BOOL _PLAYMGR_Open(UINT8 u1SrcId, CHAR* pcFileName);
EXTERN BOOL _PLAYMGR_Init(VOID);
EXTERN BOOL _PLAYMGR_Cfg(UINT8 u1SrcId);
EXTERN BOOL _PLAYMGR_Close(UINT8 u1SrcId);
EXTERN BOOL _PLAYMGR_GetMetaData(UINT8 u1SrcId, PLAYMGR_AVMETA_T *prMeta);
EXTERN BOOL _PLAYMGR_ReceiveData(UINT8 u1SrcId, UINT32 u4Size,  UINT64 u8Pos);
EXTERN BOOL _PLAYMGR_Flush(UINT8 u1SrcId);
EXTERN BOOL _PLAYMGR_SeekTime(UINT8 u1SrcId, UINT32 u4SeekTime);
EXTERN BOOL _PLAYMGR_SeekPercent(UINT8 u1SrcId, UINT32 u4SeekPercent);

EXTERN BOOL _PLAYMGR_Seek(UINT8 u1SrcId, UINT32 u4SeekTime, UINT64* pu4FilePos);
EXTERN BOOL _PLAYMGR_Set(UINT8 u1SrcId, PLAYMGR_SETTING_T* prSettings);
EXTERN BOOL _PLAYMGR_Get(UINT8 u1SrcId, PLAYMGR_SETTING_T* prSettings);
EXTERN BOOL _PLAYMGR_GetInfo(UINT8 u1SrcId, PLAYMGR_STATUS_T *prStatus);
EXTERN BOOL _PLAYMGR_GetInfoByPgm(UINT8 u1SrcId, PLAYMGR_STATUS_T *prStatus, UINT32 u4Pgm);
EXTERN BOOL _PLAYMGR_Resume(UINT8 u1Src);
EXTERN BOOL _PLAYMGR_ABRepeat(UINT8 u1SrcId, PLAYMGR_ABREPEAT_TYPE_T eMode);
EXTERN BOOL _PLAYMGR_Pause(UINT8 u1Src);
EXTERN BOOL _PLAYMGR_Stop(UINT8 u1SrcId);
EXTERN BOOL _PLAYMGR_Start(UINT8 u1SrcId);
EXTERN BOOL _PLAYMGR_Speed(UINT8 u1SrcId, INT32 i4Speed);
EXTERN INT32 _PLAYMGR_GetSpeed(UINT8 u1SrcId);
EXTERN BOOL _PLAYMGR_SetCMD(UINT8 u1SrcId, PLAYMGR_STATUS_CMD_T rStatus);
EXTERN BOOL _PLAYMGR_SetFifo(UINT8 u1SrcId, PLAYMGR_FIFO_T *prFifo);
EXTERN BOOL _PLAYMGR_Step(UINT8 u1SrcId, INT32 i4StepAmount);
EXTERN BOOL _PLAYMGR_SubtitleCb(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
                                UINT32 u4Data, const void* pvTag);
EXTERN BOOL _PLAYMGR_Abort(VOID);
EXTERN UINT64 _PLAYMGR_GetBytePos(UINT8 u1SrcId);
EXTERN UINT32 _PLAYMGR_GetPos(UINT8 u1SrcId);
EXTERN UINT32 _PLAYMGR_GetTick(UINT8 u1SrcId);
EXTERN BOOL _PLAYMGR_CtlStm(UINT8 u1SrcId, UINT32 u4InfoValue, BOOL fgEnable);
EXTERN BOOL _PLAYMGR_SetSyncMode(UINT8 u1SrcId);
EXTERN BOOL _PLAYMGR_RegCb(UINT8 u1SrcId, PLAYMGR_CALLBACK_TYPE_T eCallbackType,
                           PLAYMGR_CALLBACK pfnCallback,
                           UINT32 u4Param);
EXTERN BOOL _PLAYMGR_GetDivxDRMStatus(UINT8 u1SrcId, PLAYMGR_DRMTYPE *peDRMType, UINT32 *pu4DRMRentalLimit, UINT32 *pu4DRMRentalCount);
EXTERN BOOL _PLAYMGR_GetDivxDRMRegCode(UINT8 u1SrcId, char *pu1Code);
EXTERN BOOL _PLAYMGR_GetDivxDRMUIHelpInfo(UINT8 u1SrcId, UINT32 *pu4Info);
EXTERN BOOL _PLAYMGR_DivxDRMDeactivation(UINT8 u1SrcId, char *pu1Code);
EXTERN VOID _PLAYMGR_SetSrcType(UINT8 u1SrcId, PLAYMGR_SourceType_T eSrcType);
EXTERN PLAYMGR_SourceType_T _PLAYMGR_GetSrcType(UINT8 u1SrcId);
EXTERN VOID _PLAYMGR_SetNumberOfBitstream(UINT8 u1SrcId, UINT32 u4Size);
EXTERN UINT32 _PLAYMGR_GetNumberOfBitstream(UINT8 u1SrcId);
EXTERN BOOL _PLAYMGR_GetDataExhausted(UINT8 u1SrcId);

EXTERN BOOL _PLAYMGR_SetFeederSrcType(UINT8 u1SrcId);
EXTERN PLAYMGR_INFO_T* _PLAYMGR_GetInst(UINT8 u1SrcId);
EXTERN BOOL _PLAYMGR_GetCap(UINT8 u1SrcId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2);
EXTERN BOOL _PLAYMGR_SetLPCMInfo(UINT8 u1SrcId, PLAYMGR_LPCM_INFO_T rPlayLPCMInfo);
EXTERN VOID _PLAYMGR_FeederReset(UINT8 u1SrcId);

#ifdef __MODEL_slt__
EXTERN VOID _PLAYMGR_SetContainerType(UINT32 u4ContainerType);
EXTERN VOID _PLAYMGR_SetSubType(UINT32 u4SubType);
EXTERN VOID _PLAYMGR_SLTVdecFlushCtrl(VOID);
EXTERN VOID _PLAYMGR_SLTVdecWaitCtrlDone(VOID);
#endif
#endif

