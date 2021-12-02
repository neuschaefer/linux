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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_if.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_if.h
 *  This header file declares exported APIs of MPV.
 */

#ifndef VDEC_IF_H
#define VDEC_IF_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
#include "vdp_if.h"
#if defined(CC_MT5387) || defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395)
#include "b2r_drvif.h"
#include "b2r_if.h"
#endif
LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "x_os.h"
#include "u_common.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define VDEC_DRV_OK			       ((INT32) 0)
#define VDEC_DRV_SET_FAILED         ((INT32) -262) 
#define VDEC_DRV_INV_GET_INFO       ((INT32) -258)  
#define VDEC_DRV_INV_SET_INFO       ((INT32) -259) /**<the set operation is invalid or cannot be processed*/

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


/** the aspect ratio of video source.
*/
typedef enum
{
	VDEC_ASPECT_RATIO_NONE = 0,
	VDEC_ASPECT_RATIO_1_1, ///< Pixel Aspect ratio 1:1
	VDEC_ASPECT_RATIO_4_3, ///< Display Aspect ratio 4:3
	VDEC_ASPECT_RATIO_16_9, ///< Display Aspect ratio 16:9
	VDEC_ASPECT_RATIO_221_1, ///< Display Aspect ratio 2.21:1
    VDEC_ASPECT_RATIO_10_11, ///< Pixel Aspect ratio 10:11
    VDEC_ASPECT_RATIO_40_33, ///< Pixel Aspect ratio 40:33
    VDEC_ASPECT_RATIO_16_11, ///< Pixel Aspect ratio 16:11
    VDEC_ASPECT_RATIO_12_11, ///< Pixel Aspect ratio 12:11
    VDEC_ASPECT_RATIO_3_2,   ///< Pixel Aspect ratio 3:2
    VDEC_ASPECT_RATIO_24_11,
    VDEC_ASPECT_RATIO_20_11,
    VDEC_ASPECT_RATIO_32_11,
    VDEC_ASPECT_RATIO_80_33,
    VDEC_ASPECT_RATIO_18_11,
    VDEC_ASPECT_RATIO_15_11,
    VDEC_ASPECT_RATIO_64_33,
    VDEC_ASPECT_RATIO_160_99,
    VDEC_ASPECT_RATIO_TRANSMIT,    
	VDEC_ASPECT_RATIO_MAX	
} VDEC_ASPECT_RATIO_T;

/** vdec format.
*/
typedef enum
{
    VDEC_FMT_MPV,
    VDEC_FMT_MP4,
    VDEC_FMT_H264,
    VDEC_FMT_WMV,
    VDEC_FMT_H264VER,
    VDEC_FMT_MJPEG,
    VDEC_FMT_RV,
    VDEC_FMT_AVS,
    VDEC_FMT_MAX
}   ENUM_VDEC_FMT_T;

/** MM parameter
*/
typedef enum
{
    VDEC_MM_PLAY_SPEED = 0,
    VDEC_MM_FRAME_RATE,
    VDEC_MM_CONTAINER_TYPE,
    VDEC_MM_SEQ_INFO,
    VDEC_MM_SRC_TYPE,
	VDEC_MM_TPT_SEQ_INFO,
	VDEC_MM_RV_RPR_INFO
} VDEC_MM_PARAM_T;


/** the notify conditions.
  This structure match to the one in mw. Can't change it's sequence.
*/
typedef enum
{
    VDEC_COND_ERROR = -1,
    VDEC_COND_CTRL_DONE,
    VDEC_COND_RESOLUTION_CHG,
    VDEC_COND_I_FRAME_SET_BUFF_DONE,
    VDEC_COND_I_FRAME_DONE,    
    VDEC_COND_DECODE_STATUS_CHG,
    VDEC_COND_SPEED_CHG,
    VDEC_COND_THUMBNAIL_DONE
}   VDEC_DEC_COND_T;


/** the notify condition status.
  This structure match to the one in mw. Can't change it's sequence.
*/
typedef enum
{
    VDEC_DEC_DECODE_UNKNOWN = -1,
    VDEC_DEC_DECODE_NO_DATA,
    VDEC_DEC_DECODE_NORMAL,
    VDEC_DEC_DECODE_ERROR,
    VDEC_DEC_DECODE_HD_NOT_SUPPORT,
    VDEC_DEC_DECODE_CODEC_NOT_SUPPORT,
    VDEC_DEC_DECODE_CODEC_TYPE_CHG,
    VDEC_DEC_DECODE_RECEIVE_DATA
}   VDEC_DEC_DECODE_STATUS_T;


/** the notify condition status.
  This structure match to some structure, Can't change it's value
*/
typedef enum
{
    VDEC_PIC_TYPE_SEQ = 1,
    VDEC_PIC_TYPE_I   = 2,
    VDEC_PIC_TYPE_P   = 4,
    VDEC_PIC_TYPE_B   = 8,
    VDEC_PIC_TYPE_END = 16
}   VDEC_PIC_TYPE_T;

    
typedef struct {
    UINT16 u2UdfNs;
} VDEC_GNL_INFO_T;

typedef struct {
    UINT32 au4DecTime[5][9]; //[3]P,B,I,SP,SI, [4]maxMs, minMs, totalMs, FrmNum, OverTimeNum, SWTime, DecCycle, MaxDecCycle, MaxDramCycle
    VDEC_ASPECT_RATIO_T eAspectRatio;
    VDEC_DEC_DECODE_STATUS_T eNotifyStatus;
    ENUM_VDEC_FMT_T eCodecType;
    UINT32 u4BitRate;
    UINT32 u4FailCnt;
    //es fifo 
    UINT32 u4DisplayQPicCnt;
    UINT32 u4FirstDispPTS;
    UINT32 u4MaxEsCnt;
    UINT32 u4CurEsCnt;
    UINT32 u4Profile;
    UINT32 u4Level;
    UINT32 u4IsCABAC;
    //mpeg2
    UINT32 u4VbvSizeExt;
    UINT32 u4ChromaFmt;
    //~mpeg2
    UINT16 u2OrgHeight;
    UINT16 u2OrgWidth;
    UINT16 u2Height;
    UINT16 u2Width;
    UINT16 u2DHS;
    UINT16 u2DVS;    
    UINT16 u2FrmRate;
    UINT16 u2TempRef;
    UINT8 u1CurState; 
    UCHAR ucVideoFmt;
    UCHAR ucPicType;
    UCHAR ucPicStruct;
    UCHAR ucAfd;
    UCHAR ucFbNum;
    //mpeg2
    UCHAR ucProfileId;
    UCHAR ucColorPrimaries;
    UCHAR ucTransferCharacteristics;
    UCHAR ucMatrixCoefficients;
    //~mpeg2
    BOOL fgProgressiveSeq;
    BOOL fgTFF;
    BOOL fgRFF;
    BOOL fgProgressiveFrm;
    BOOL fgHdrInvalid;
    BOOL fgNoOverScan;
    //mpeg2
    BOOL fgColorDescrip;
    BOOL fgContrained;
    BOOL fgMPEG2; // mpeg1 or mpeg2
    BOOL fgSeqDispExt;
    BOOL fgLowDelay;
    //~mpeg2

} VDEC_HDR_INFO_T;

typedef void (*PFN_VDEC_EVENT_NOTIFY)(
    UINT32 u4Arg1, 
    UINT32 u4Arg2, 
    UINT32 u4Arg3, 
    UINT32 u4Arg4
);

/**This notification function is called by the video decoder in response to some events.
*/
typedef void (*PFN_VDEC_DEC_NFY_FCT) (
    void*               pvNfyTag,
    VDEC_DEC_COND_T      eNfyCond,
    UINT32              u4Data1,
    UINT32              u4Data2
);

/**This structure contains a video decoders notify function and tag.
*/
typedef struct
{
    void*  pvTag;
    PFN_VDEC_DEC_NFY_FCT  pfDecNfy;
}VDEC_DEC_NFY_INFO_T;


typedef struct {
    UINT64 u8Scale;
    UINT32 u4Rate;
} VDEC_PTS_INFO_T;

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/** Initialize VDEC module.
*/
EXTERN void VDEC_Init(void);

/** Terminate VDEC module.
*/
EXTERN void VDEC_Termint(void);

/** Reset VDEC module.
*/
EXTERN void VDEC_Reset(void);

/** Change to PLAY mode.
*@param u4Fmt.
 */
EXTERN BOOL VDEC_Play(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt);

/** Change to PLAY I frame mode.
*@param u4Fmt.
 */
EXTERN void VDEC_Play_I_Frame(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt);

/** Change to PLAY mode.
*@param u4Fmt.
 */
EXTERN void VDEC_PlayVideoClip(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt);

/** Change to STOP mode.
*@param.
 */
EXTERN void VDEC_Stop(UCHAR ucEsId);

/** Change to PAUSE mode.
*@param.
 */
EXTERN void VDEC_Pause(UCHAR ucEsId);

/** Flush decoded picture buffer.
*@param.
 */
EXTERN void VDEC_Flush_DPB(UCHAR ucEsId);

/** Set Sync mode.
*@param.
 */
EXTERN void VDEC_SyncStc(UCHAR ucEsId, UCHAR ucMode, UCHAR ucStcId);

/** Register notify function.
*@param.
 */
EXTERN void VDEC_RegEventNotifyFunc(PFN_VDEC_EVENT_NOTIFY pfFunc);

/** Set decode notify function.
*@param ucEsId 					ES id.
*@param prDecNfyInfo 			Video decoders notify function and tag.
*@retval VDEC_DRV_OK  			Success.
*@retval VDEC_DRV_INV_SET_INFO  Fail.
*/
EXTERN INT32 VDEC_SetDecNfy(UCHAR ucEsId, const VDEC_DEC_NFY_INFO_T* prDecNfyInfo);

/** Query status.
*@param .
 */
EXTERN BOOL VDEC_QueryInfo(UCHAR ucEsId, VDEC_HDR_INFO_T* prHdrInfo);

/** Query status.
*@param .
 */
EXTERN BOOL VDEC_GetGnlInfo(UCHAR ucEsId, VDEC_GNL_INFO_T* prGnlInfo);

/** Set parameter.
*@param .
 */
EXTERN void VDEC_SetParam(UCHAR ucEsId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3);

/** Get parameter.
*@param .
 */
EXTERN void VDEC_GetParam(UCHAR ucEsId, UINT32 u4Type, UINT32* pu4Param1, UINT32* pu4Param2, UINT32* pu4Param3);

/** Get parameter.
*@param .
 */
EXTERN BOOL VDEC_IsLock(UCHAR ucEsId);

/** Get parameter.
*@param .
 */
EXTERN void VDEC_PlayMM(UCHAR ucEsId, BOOL fgPlayMM);

/** Get parameter.
*@param .
 */
EXTERN BOOL VDEC_IsPlayMM(UCHAR ucEsId);

/** Set MM related parameter.
*@param .
 */
EXTERN void VDEC_SetMMParam(UCHAR ucEsId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3);
/** Clr MM related parameter.
*@param .
 */
EXTERN void VDEC_ClrMMParam(UCHAR ucEsId, UINT32 u4Type);

/** Set  Pvr  parameter.
*@param .
 */
EXTERN BOOL VDEC_SetWaitPvrSps(UCHAR ucEsId, UINT8 u1PVRWaitSps);

/** Set  Pvr  parameter.
*@param .
 */
EXTERN void VDEC_SetTPTFlag(UCHAR ucEsId, BOOL fgIsTPT);

/** Set BigB related parameter.
*@param .
 */
EXTERN void VDEC_SetBigBParameter(UCHAR ucEsId, BOOL fgForceEnable, UINT32 u4BigBLevel1,
    UINT32 u4BigBLevel2, UINT32 u4BigBLevel3, UINT32 u4BigBBaseCnt);

/** Set MM related parameter.
*@param .
 */
EXTERN VOID VDEC_GetQueueInfo(UCHAR ucEsId,UINT16* pu2QueueSize,UINT16* pu2MaxQueueSize);

/** Get video thumbnail.
*@param .
 */
EXTERN BOOL VDEC_GetVideoThumbnail(UCHAR ucEsId, UINT16 u2CompId, BOOL bIsOnePass, 
    VDP_THUMBNAIL_INFO_T* prDstInfo, VDP_CAPTURE_INTO_T* prSrcInfo);

/** Check video type capability.
*@param .
 */
EXTERN BOOL VDEC_ChkCodecCap(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt);

/** Check DivX bonding.
*@param .
 */
EXTERN BOOL VDEC_ChkDivXBonding(VOID);

/** VDEC notify status to MW.
*@param .
 */
EXTERN BOOL VDEC_Notify(UCHAR ucEsId,VDEC_DEC_DECODE_STATUS_T notifyStatus);
#endif //VDEC_IF_H


