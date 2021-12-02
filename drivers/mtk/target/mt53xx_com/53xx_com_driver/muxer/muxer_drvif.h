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
 * $RCSfile: swdmx_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_if.h
 *  Public interface of software program stream demux driver
 */


#ifndef MUXER_DRVIF_H
#define MUXER_DRVIF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

/*lint -save -e961 */
#include "x_common.h"
#include "x_typedef.h"
#include "x_timer.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_clksrc.h"
#include "x_hal_5381.h"
#include "x_hal_926.h"
#include "x_debug.h"
//#include "../fm/x_fm.h"
#include "x_util.h"

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define MUXER_MAX_INST_NS   2

#define MUXER_CMD_Q_SIZE 10
#define MUXER_VID_PES_Q_SIZE 255
#define MUXER_AUD_PES_Q_SIZE 255
#define MUXER_VBI_PES_Q_SIZE 255
#define MUXER_DATA_Q_SIZE    255

#define MUXER_FPS_ACCURACY 1000

#define CC_MUXER_EVALUATION
#ifdef CC_MUXER_EVALUATION
//#define MUXER_VID_DEST_SIZE 87500
#define MUXER_VID_DEST_SIZE 466 *188
//#define MUXER_VID_DEST_SIZE 567 *188
//#define MUXER_VID_DEST_SIZE 18116 *188
//#define MUXER_AUD_DEST_SIZE 6144
#define MUXER_AUD_DEST_SIZE 33 * 188
#define MAX_PATH 512
#endif

#define MUXER_TMP_BUF_SZ    (512 * 1024)


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/**The video codec format*/
typedef enum
{    
    ENUM_MUXER_VENC_UNKNOWN,    
    ENUM_MUXER_VENC_MPEG1,    
    ENUM_MUXER_VENC_MPEG2,    
    ENUM_MUXER_VENC_H264,    
    ENUM_MUXER_VENC_MAX
}ENUM_MUXER_VENC_T;

/**Audio decoder format.*/
typedef enum
{    
    ENUM_SWDMX_ADEC_UNKNOWN = 0,    
    ENUM_SWDMX_ADEC_MPEG,    
    ENUM_SWDMX_ADEC_AC3,    
    ENUM_SWDMX_ADEC_PCM,    
    ENUM_SWDMX_ADEC_MP3,    
    ENUM_SWDMX_ADEC_AAC,    
}ENUM_MUXER_AENC_T;

typedef struct
{
    ENUM_MUXER_VENC_T eVidFmt;
    ENUM_MUXER_VENC_T eAudFmt;

    UINT32 u4VidFps;
    UINT32 u4AudSamplingRate;

    UINT32 u4CurVidPts;
    UINT32 u4CurAudPts;

    UINT64 u8VPtsRemainder;
    UINT64 u8APtsRemainder;
    
    UINT32 u4AudResidueLen;
    UINT32 u4AudResiduePts;
} MUXER_MPG_INFO_T;

typedef struct
{
    ENUM_MUXER_VENC_T eVidFmt;
    ENUM_MUXER_VENC_T eAudFmt;

    UINT32 u4VidFps;
    UINT32 u4AudSamplingRate;

    UINT32 u4CurVidPts;
    UINT32 u4CurAudPts;
} MUXER_CMPB_INFO_T;

typedef struct
{
    ENUM_MUXER_VENC_T eVidFmt;
    ENUM_MUXER_VENC_T eAudFmt;

    UINT32 u4VidFps;
    UINT32 u4AudSamplingRate;

    UINT32 u4CurVidPts;
    UINT32 u4CurAudPts;
} MUXER_RAW_INFO_T;

typedef enum 
{
    eMUXER_HDR_SUCCEED,                                 // header muxing succeed
    eMUXER_HDR_FAIL,                                        // header muxing fail
    eMUXER_HDR_UNKNOWN_TYPE                      // unknown muxing type
} MUXER_HDR_STATE_T;

typedef MUXER_HDR_STATE_T (*PFN_MULTIPLEX_HDR)(UCHAR ucMxId);

typedef struct 
{
    BOOL	fgMuxerInited;
    BOOL    fgOccupied;
    
    UINT32 u4VidCnt;
    UINT32 u4AudCnt;
    UINT32 u4VBICnt;

    UINT32 u4RecTotalTime;

    HANDLE_T hMuxerMainThread;
#if 0
    HANDLE_T hMuxerTestThread;
#endif
    HANDLE_T hMuxerCmdQ;
    HANDLE_T hMuxerVidDataQ;
    HANDLE_T hMuxerAudDataQ;
    HANDLE_T hMuxerVBIDataQ;
    HANDLE_T hMuxerDataQ;
    HANDLE_T hMuxerSema;

    VENC_PES_INFO_T rVEncPesInfo;
    AENC_PES_INFO_T rAEncPesInfo;
    VBIENC_PES_INFO_T rVBIEncPesInfo;
    MUXER_PES_INFO_T rMuxerPesInfo;

    ENUM_MUXER_STATE_T eMuxerPrevousStatus;
    ENUM_MUXER_STATE_T eMuxerStatus;
    ENUM_MUXER_FMT_T eCurFMT;

    PFN_MULTIPLEX_HDR pfnMuxHdr;

#ifdef CC_MUXER_EVALUATION
    HANDLE_T hFileHandle;

    UCHAR *pucVidSrcBuf;
    UCHAR *pucVidDestBuf;

    UCHAR *pucAudSrcBuf;
    UCHAR *pucAudDestBuf;
#endif

    union
    {
        MUXER_MPG_INFO_T  rMuxerMpgInfo;
        MUXER_CMPB_INFO_T rMuxerCmpbInfo;
        MUXER_RAW_INFO_T  rMuxerRawInfo;
    } rFmtInfo;
} MUXER_INFO_T;


//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototype  of public functions
//-----------------------------------------------------------------------------
VOID _MUXER_Init(VOID);

MX_RET _MUXER_GetInst(UCHAR *pucMxId);

MX_RET _MUXER_ReleaseInst(UCHAR ucMxId);

MUXER_INFO_T *_MUXER_QueryInst(UCHAR ucMxId);

BOOL _MUXER_Run(UCHAR ucMxId);

BOOL _MUXER_Pause(UCHAR ucMxId);

BOOL _MUXER_Stop(UCHAR ucMxId);

BOOL _MUXER_Flush(UCHAR ucMxId);

BOOL _MUXER_SendVideoPes(UCHAR ucMxId, VENC_PES_INFO_T *prVEncPes);

BOOL _MUXER_QueryVideoPesCnt(UCHAR ucMxId);

BOOL _MUXER_SendAudioPes(UCHAR ucMxId, AENC_PES_INFO_T *prAEncPes);

BOOL _MUXER_SendVBIPes(UCHAR ucMxId, VBIENC_PES_INFO_T *prVBIEncPes);

BOOL _MUXER_SendPes(UCHAR ucMxId, MUXER_PES_INFO_T *prPes);

BOOL _MUXER_ReceiveVideoPes(UCHAR ucMxId);

BOOL _MUXER_ReceiveAudioPes(UCHAR ucMxId);

BOOL _MUXER_ReceiveVBIPes(UCHAR ucMxId);

BOOL _MUXER_ReceivePes(UCHAR ucMxId);

BOOL _MUXER_QueryAudioPesCnt(UCHAR ucMxId);

BOOL _MUXER_SetMuxFmt(UCHAR ucMxId, ENUM_MUXER_FMT_T eMuxFmt);

VOID vMxPutBits(UINT32 u4Pattern, UINT32 u4BitLen);

VOID _MUXER_CLI_TEST_Init(UINT32 u4Fmt);

#endif  // MUXER_DRVIF_H

