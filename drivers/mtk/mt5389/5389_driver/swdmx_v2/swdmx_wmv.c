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
 * $RCSfile: swdmx_wmv.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_wmv.c
 *  Software demux for wmv - internal interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_if.h"
#include "vdec_drvif.h"
#include "feeder_if.h"
#include "dmx_mm_if.h"
#include "swdmx_drvif.h"
#include "swdmx_wmv.h"
#include "swdmx_debug.h"
#include "x_util.h"
#include "u_midxbuld.h"
#include "aud_drvif.h"

EXTERN void VDEC_SetWptr(UINT8 u1VdecId, UINT32 u4Wptr);
//EXTERN INT32 AUD_GetAudFifo(UINT32 * pu4Fifo1Start, UINT32 * pu4Fifo1SEnd,UINT32 * pu4Fifo2Start,UINT32 * pu4Fifo2End);

EXTERN void AUD_DrvSetWMAErrFunc(UINT8 u1DecId, AUD_WMAErr_NOTIFY pfClipNotify, VOID *prInst);  // megaa 20090828

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define WMV_DATA_OBJ_HEADER (50)
#define WMV_NETFLIX_HEADER_SIZE (4)
#define WMV_MAX_FEEDER_RETRY (10)
#define RETRY_TIMES (3)
#define TRICK_MODE_RETRY_TIMES (1000)
#define FEEDER_OFFSET (64*1024)
#define WMV_KEY_TABLE_THRESHOLD (3)
#define MORE_DEMUX_PACKET_COUNT 11  // ToDo: adjust according to real picture size
#define WMV_VFIFO_SIZE (10*1024*1024)
#define WMV_DMX_FIFO_ALIGNMENT (64)
#define SWMDX_WMV_FEEDER_DATA_Q_SIZE (50)
#define NETFLIX_UNDERFLOW_THRESHOLD (15)
#define NETFLIX_CMMP_BUFFER_SIZE (15*1024)
#define NETFLIX_CMMP_BUFFER_SIZE_EXTRA (20*1024)
#define WMV_INVALID_STREAM_NUM_THREAHOLD (1000)

#ifdef __KERNEL__
#define FM_BLOCK_SIZE (2048)  // tmp solution for DTV00213176, DTV00213223, DTV00213232(?)
#else
#define FM_BLOCK_SIZE (512)
#endif

//option define
#define WMV_FAST_I_FRAME_PARSING
#define WMV_MULTI_REQUEST // always turn on
#define WMV_ERROR_HANDLE_ON
#define WAIT_AUDIO_READY
#define WMV_SUPPORT_AUDIO_SWITCH
//#define WMV_ALLOCATE_VFIFO
//#define DISABLE_AUDIO_IN_BAD_INTERLEAVE
//#define ENABLE_WMV_DRM
#define WMA_TRICK_MODE
#define WMV_SEEK_OFFSET
//#define WMA_SEEK_OFFSET

#ifdef CC_3D_MM_DS_SUPPORT  // tmp solution
//#define WMV_BAD_INTLV_SUPPORT  // bad interleave support (defined in cfg)
#endif
#ifdef WMV_BAD_INTLV_SUPPORT
//#define WMV_BAD_INTLV_DEBUG
#endif

#ifdef WMA_TRICK_MODE
#define WMA_SENDDATA_TIMELEN (2*90000)
#define WMA_MAXSTEND_TIME (6)
#endif

#ifdef WMA_SEEK_OFFSET
#define WMA_SEEK_PACKET_NUM (6)
#define WMA_SEEK_MINCHECK_NUM (3)
#define WMA_MAX_SEEKTIME (4)
#endif

#define WMV_NETFLIX_PRE_BUFFER_SIZE  (2*1024*1024)  // DTV00086562: Netflix requires 2MB pre-buffer size anyway

#ifdef ENABLE_WMV_DRM
  #error "FIXME!!!"
#endif

#define INPUT_SRC  (_prRangeList->prDmxRangeInfo->e_mm_src_type)

#ifdef CC_NETFLIX_CMPB_SUPPORT
static UINT32 _u4RemainPacketDataSize = 0;
static UINT8* _pu1RemainPacketData = NULL;
#endif

#ifdef WMV_BAD_INTLV_SUPPORT
BOOL _fgEnBadIntlv = 1;
UINT32 _u4WmvBadIntlvThresA = 2100;
UINT32 _u4WmvBadIntlvThresV = 1800;
#endif

#ifdef WMV_BAD_INTLV_DEBUG
UINT32 _u4AFifoAccuLen = 0;
UINT32 _u4VFifoAccuLen = 0;
#endif


#if 1  // multi-instance?

#ifdef CC_DLNA_SUPPORT
#define FEEDER_SEGMENT       min(282*1024, min(FEEDER_AUDIO_BUF_SIZE, FEEDER_DEFAULT_BUF_SIZE) >> 3)
#define WMV_PRE_BUFFER_SIZE  min(2*1024*1024, min(FEEDER_AUDIO_BUF_SIZE, FEEDER_DEFAULT_BUF_SIZE) * 5 / 10)
#else
#ifdef CC_DMX_PURE_AUDIO
#define FEEDER_SEGMENT       min(240*1024, min(FEEDER_AUDIO_BUF_SIZE, FEEDER_DEFAULT_BUF_SIZE) >> 3)
#define WMV_PRE_BUFFER_SIZE  min(250*1024, min(FEEDER_AUDIO_BUF_SIZE, FEEDER_DEFAULT_BUF_SIZE) * 5 / 10)
#else
#define FEEDER_SEGMENT       min(282*1024, min(FEEDER_AUDIO_BUF_SIZE, FEEDER_DEFAULT_BUF_SIZE) >> 3)
#define WMV_PRE_BUFFER_SIZE  min(2*1024*1024, min(FEEDER_AUDIO_BUF_SIZE, FEEDER_DEFAULT_BUF_SIZE) * 5 / 10)
#endif
#endif
#define WMA_REQUEST_SIZE (192*1024)
#else  // multi-instance?

#ifdef CC_DLNA_SUPPORT
//#define WMV_ALLOCATE_VFIFO
#define FEEDER_SEGMENT (282*1024)
#define WMV_PRE_BUFFER_SIZE (2*1024*1024)
//#define ENABLE_FEEDER_THRESHOLD //something wrong....
#else
#ifdef CC_DMX_PURE_AUDIO
// feeder buffer only have 512*1024
// wmv will request two feeder request.
// So wmv can't use 288*1024 or feeder will full and drop that request.
#define WMV_PRE_BUFFER_SIZE (250*1024)
#define FEEDER_SEGMENT (240*1024)
#else
// 282*1024 = 288768
#define WMV_PRE_BUFFER_SIZE (2*1024*1024)
#define FEEDER_SEGMENT (282*1024)
#endif
#endif

#endif  // multi-instance?

//#ifdef CC_DLNA_SUPPORT
//#define WMV_2X_TO_1X_PREBUFFER
//#endif


#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
  #ifdef FEEDER_SEGMENT
  #undef FEEDER_SEGMENT
  #endif

  #define FEEDER_SEGMENT  (512*1024)  // to ensure the data input performance under Linux!
#endif


#define NETFLIX_MUX (0)
#define NETFLIX_AUDIO (1)
#define NETFLIX_VIDEO (2)

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
//for wmv parsing
#undef TypeTransform
#define TypeTransform(x) (SIZE_T)((x&0x2)&&(x&0x1)?4:x)
#undef min
#define min(x, y) ((x < y) ? x : y)
#undef max
#define max(x, y) ((x > y) ? x : y)
#undef abs
#define abs(x) (((x) >= 0) ? (x) : -(x))
#define IDR_PIC 5
#define IS_IDR_NALU(x) (((x & 0x1f) == IDR_PIC))

#define _rSwdmxWmvInfo  (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo)  // multi-instance

#define SWDMX_WMV_VERIFY_NULL(ptr)                                                          \
          do {                                                                              \
            if (ptr == NULL) {                                                              \
                LOG(3, "SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
                return; }                                                                   \
          } while (0)

#define SWDMX_WMV_VERIFY_NULL_ERROR(ptr)                                                    \
          do {                                                                              \
            if (ptr == NULL) {                                                              \
                LOG(3, "SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
                return FALSE; }                                                             \
          } while (0)

#ifdef SUPPORT_3D_MEDIA
#define ENABLE_NEWTRACK(prSwdmxInst) (prSwdmxInst->eSubVideoType==ENUM_SWDMX_VDEC_MULTITRACK&&(prSwdmxInst->u1VdecId!=prSwdmxInst->u1Vdec2Id))
#define IS_NEWTRACK(prSwdmxInst,u1Id) (ENABLE_NEWTRACK(prSwdmxInst)&&(u1Id==prSwdmxInst->u1Vdec2Id))
#define IS_NEWTRACKDATA(prSwdmxInst,u1StreamId) (prSwdmxInst->eSubVideoType==ENUM_SWDMX_VDEC_MULTITRACK&&u1StreamId==prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u4VSubStreamId)
#define IS_SIMP_IDX(p) ((p) && (p)->ui4_size > 3)
#else
#define ENABLE_NEWTRACK(prSwdmxInst) (0)
#define IS_NEWTRACK(prSwdmxInst,u1Id)(0)
#define IS_NEWTRACKDATA(prSwdmxInst,u1StreamId) (0)
#define IS_SIMP_IDX(p) (1)
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#if 1  // multi-instance

#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
UINT32 _u4BIVideoDelay = 500;
UINT32 _u4BIVideoDelay2 = 5;
UINT32 _u4BIAThreshold = 60;
UINT32 _u4BIAThreshold2 = 25;
//UINT32 _u4BIAudioDelay = 0;

UINT32 _u4CommVFifoSize = 0;
UINT32 _u4CommAFifoSize = 0;
UINT8 _u1CommAudDmxId = 0;
UINT8 _u1CommAudPid = 0;
HANDLE_T _hCommBIQ = 0;
#endif

#define _prRangeList             (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.prRangeList)
#define _prRangeInfo             (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.prRangeInfo)

#define _rFeederCond             (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.rFeederCond)
#define _rFeederReqData          (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.rFeederReqData)

#define _u1AudioStreamId         (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudioStreamId)
#define _rFeederInfo             (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.rFeederInfo)

#if 1  // because of complicated build error  ToDo: fix it and use instance variable
static VDEC_PES_INFO_T _rPreviousPesInfo[SWDMX_SOURCE_MAX] = {{{0}}};
static DMX_AUDIO_PES_T _rPreviousAudioPes = {0};
static BOOL _fgReceiveAudioPes = FALSE;
#else
#define _rPreviousPesInfo        (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.rPreviousPesInfo)
#define _rPreviousAudioPes       (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.rPreviousAudioPes)
#define _fgReceiveAudioPes       (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.fgReceiveAudioPes)
#endif

#define _fgReceiveVideoPes       (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.fgReceiveVideoPes)
//#define _u4DataWithStartCode     (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u4DataWithStartCode)
#define _pu1VideoFrameData       (prSwdmxInst->pu1VideoFrameData)  //(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.pu1VideoFrameData)
#define _u4PrevFramePTS          (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u4PrevFramePTS)
#define _u4DefalutReqSize        (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u4DefalutReqSize)

//for netflix application
#define _prWmvGenericInfo        (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.prWmvGenericInfo)
#define _u4NetflixVideoStreamId  (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u4NetflixVideoStreamId)
#define _u4NetflixAudioStreamId  (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u4NetflixAudioStreamId)
#define _fgNetflixEos            (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.fgNetflixEos)
#define _fgDataExhausted         (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.fgDataExhausted)
#define _eCurFMT                 (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.eCurFMT)
#define _u4NumberOfBitstream     (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u4NumberOfBitstream)
//for netflix application

#define rPayLoadInfo             (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.rPayLoadInfo)
#define rMultiPayloadInfo        (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.rMultiPayloadInfo)
#define rPayLoadHeader           (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.rPayLoadHeader)

#ifdef ENABLE_WMV_DRM
static UINT8 _u1VideoFrameData[100*1024];
static UINT64 _u8SampleId = 0;

//1.wmv drm file
static UINT8 _au1ContentKey[16] = {0xfd, 0x2b, 0xb7, 0x8c, 0x88, 0xb8 ,0xbe ,0x2f,0x75,
                                   0x15, 0xe9, 0xf6, 0x2f, 0x1a, 0xa4 ,0x3a
                                  };


//2.wmv drm file
/*static UINT8 _au1ContentKey[16] = {0xd3, 0x16, 0x1f, 0xea, 0x9e, 0x12 ,0xaf ,0x1,0x12,
                                   0x19, 0xd1, 0x29, 0x9, 0x8a, 0xe3 ,0xc5
                                  };
*/

#endif  // ENABLE_WMV_DRM

#ifdef WMV_FAST_I_FRAME_PARSING
#define _u8PreKeyFrameAddr     (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u8PreKeyFrameAddr)
#endif


//#ifdef WMV_MULTI_REQUEST
#define _hWMVFeederDataQue     (prSwdmxInst->hWMVFeederDataQue) //(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.hWMVFeederDataQue)
#define _u4RequestDataSize     (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u4RequestDataSize)
//#endif

#define _u4VFifoStartAddr      (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u4VFifoStartAddr)
#define _u4VFifoSize           (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u4VFifoSize)
#define _u4AFifoStartAddr      (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u4AFifoStartAddr)
#define _u4AFifoSize           (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u4AFifoSize)

//static UINT32 _u4WmvStpRsmThres = 1;  // for stop resume

#ifdef WMV_2X_TO_1X_PREBUFFER
static INT32 _i4WmvStcAdj = -1500;
#endif

#define _u4WmaBytesPerSec      (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u4WmaBytesPerSec)
#define _rBadIntlv             (prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.rBadIntlv)

#else  // single-instance

static SWDMX_RANGE_LIST_T *_prRangeList = NULL;
static MM_RANGE_INFO_T* _prRangeInfo = NULL;

static FEEDER_TRANSMIT_COND _rFeederCond;
static FEEDER_REQ_DATA_T _rFeederReqData;

static UINT8 _u1AudioStreamId = 0xFF;
static FEEDER_BUF_INFO_T _rFeederInfo;

static SWDMX_WMV_INFO_T _rSwdmxWmvInfo;
static VDEC_PES_INFO_T _rPreviousPesInfo;
static DMX_AUDIO_PES_T _rPreviousAudioPes;
static BOOL _fgReceiveAudioPes;
static BOOL _fgReceiveVideoPes;
//static UINT32 _u4DataWithStartCode[10*1024];
static UINT8* _pu1VideoFrameData = NULL;
static UINT32 _u4PrevFramePTS = 0;
static UINT32 _u4DefalutReqSize = 0;

//for netflix application
static MINFO_TYPE_GENERIC_T* _prWmvGenericInfo = NULL;
static UINT32 _u4NetflixVideoStreamId;
static UINT32 _u4NetflixAudioStreamId;
static BOOL _fgNetflixEos = FALSE;
static BOOL _fgDataExhausted = TRUE;
static ENUM_SWDMX_FMT_T _eCurFMT;
static UINT32 _u4NumberOfBitstream = 1;
//for netflix application

static PayloadParseInfo rPayLoadInfo;
static MultiPayloadInfo rMultiPayloadInfo;
static PayloadHeader rPayLoadHeader;


#ifdef ENABLE_WMV_DRM
static UINT8 _u1VideoFrameData[100*1024];
static UINT64 _u8SampleId = 0;

//1.wmv drm file
static UINT8 _au1ContentKey[16] = {0xfd, 0x2b, 0xb7, 0x8c, 0x88, 0xb8 ,0xbe ,0x2f,0x75,
                                   0x15, 0xe9, 0xf6, 0x2f, 0x1a, 0xa4 ,0x3a
                                  };


//2.wmv drm file
/*static UINT8 _au1ContentKey[16] = {0xd3, 0x16, 0x1f, 0xea, 0x9e, 0x12 ,0xaf ,0x1,0x12,
                                   0x19, 0xd1, 0x29, 0x9, 0x8a, 0xe3 ,0xc5
                                  };
*/

#endif  // ENABLE_WMV_DRM

#ifdef WMV_FAST_I_FRAME_PARSING
static UINT64 _u8PreKeyFrameAddr =0;
#endif


//#ifdef WMV_MULTI_REQUEST
static HANDLE_T _hWMVFeederDataQue = 0;
static UINT32 _u4RequestDataSize = 0;
static HAL_TIME_T _rFeederReqTime;
static HAL_TIME_T _rStartTime,_rEndTime;
//#endif

#ifdef WMV_ALLOCATE_VFIFO
static UINT32 _u4VFifoStartAddr = NULL;
#endif

//static UINT32 _u4WmvStpRsmThres = 1;  // for stop resume

#ifdef WMV_2X_TO_1X_PREBUFFER
static INT32 _i4WmvStcAdj = -1500;
#endif

#endif  // multi/single-instance


#ifdef __MODEL_slt__
static UINT8 _au1VC1SltSeqHdr[] =
{
    0x21, 0x00, 0x00, 0x01, 0x0F, 0xD3, 0xEE, 0x27,
    0xF1, 0x67, 0x0A, 0x27, 0xF8, 0x59, 0xE2, 0xE9,
    0x40, 0x00, 0x40, 0x01, 0xC0, 0x00, 0x80, 0x01,
    0xA0, 0x00, 0xC0, 0x01, 0x80, 0x01, 0x00, 0x01,
    0x60, 0x01, 0x40, 0x01, 0x40, 0x01, 0x80, 0x01,
    0x20, 0x01, 0xC0, 0x01, 0x00, 0x02, 0x00, 0x00,
    0xE0, 0x02, 0x40, 0x00, 0xC0, 0x02, 0x80, 0x00,
    0xA0, 0x02, 0xC0, 0x00, 0x80, 0x03, 0x00, 0x00,
    0x60, 0x03, 0x40, 0x00, 0x40, 0x03, 0x80, 0x00,
    0x28, 0x00, 0x00, 0x01, 0x0E, 0x16, 0x93, 0xBB,
    0xD3, 0xEB, 0xF4, 0x04, 0x14, 0x24, 0x3C, 0x44,
    0x54, 0x5C, 0x64, 0x7C, 0x94, 0x9F, 0xC5, 0x9E,
    0x40
};

UINT8 _u1VC1SltLastPicType = 0;
UINT32 _u4VC1SltLastPicPts = 0;
#endif


#if 0  // moved to swdmx_drvif.h for multi-instance
typedef struct
{
    FEEDER_REQ_DATA_T rFeederReqData;
    UINT32 u4RequestFeederSize;
} WMV_FEEDER_REQ_DATA_T;
#endif

UINT32 u4AudDspPts[SWDMX_SOURCE_MAX]={1};
static BOOL fgTrickTrigger[SWDMX_SOURCE_MAX]={FALSE};
//static UINT32 u4StartIndex=0;
extern AUD_DRV_STATE_T AUD_DRVGetAudioState(UINT8 u1DecId);
BOOL _SWDMX_WMVRequestFeederData(SWDMX_INFO_T *prSwdmxInst, UINT32 u4QrySize,UINT64 u8Offset,BOOL fgTrickMode);


static VOID _SWDMX_WMVFlushFeederDataQ(SWDMX_INFO_T *prSwdmxInst)
{
    UINT16 u2Cnt;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    WMV_FEEDER_REQ_DATA_T rData;

    zMsgSize = sizeof(WMV_FEEDER_REQ_DATA_T);
    while (x_msg_q_receive(&u2MsgQIdx, &rData, &zMsgSize,
                           &(_hWMVFeederDataQue), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
    }

    VERIFY(x_msg_q_num_msgs(_hWMVFeederDataQue, &u2Cnt) == OSR_OK);
    ASSERT(u2Cnt== 0);
}


#ifdef WMV_BAD_INTLV_SUPPORT
static BOOL _WMVBeginReparse(SWDMX_INFO_T* prSwdmxInst)
{
    if (_rBadIntlv.eState == BAD_INTLV_STT_TO_REPRS_V)
    {
        _rBadIntlv.eState = BAD_INTLV_STT_REPARSE_V;
    }
    else if (_rBadIntlv.eState == BAD_INTLV_STT_TO_REPRS_A)
    {
        _rBadIntlv.eState = BAD_INTLV_STT_REPARSE_A;
    }
    else
    {
        return FALSE;
    }
    
    //_SWDMX_FlushReqMsgQ(u1SrcId);  !!!
    _SWDMX_WMVFlushFeederDataQ(prSwdmxInst);
    _rSwdmxWmvInfo.fgFirstReqFeederData = TRUE;
    _rSwdmxWmvInfo.u4ConsumedSize = 0;
    _SWDMX_WMVComsumeFeeder(prSwdmxInst);
    
    _rSwdmxWmvInfo.u4CurParsingPacketNo = _rBadIntlv.u4BeginSkipPktNo;
    _rSwdmxWmvInfo.u8LastFileParsingPosition = _prRangeInfo->ui8_fileoffset + WMV_DATA_OBJ_HEADER +
            _rSwdmxWmvInfo.u4CurParsingPacketNo*_rSwdmxWmvInfo.u4PacketSize;

    return TRUE;
}

static BOOL _WMVBeginRePatch(SWDMX_INFO_T* prSwdmxInst)
{
    if (_rBadIntlv.u4EndPatchPktNo == 0)  // !!!1027
        return FALSE;
    
    if (_rBadIntlv.eState == BAD_INTLV_STT_TO_PATCH_V)
    {
        _rBadIntlv.eState = BAD_INTLV_STT_PATCH_V;
    }
    else if (_rBadIntlv.eState == BAD_INTLV_STT_TO_PATCH_A)
    {
        _rBadIntlv.eState = BAD_INTLV_STT_PATCH_A;
    }
    else
    {
        return FALSE;
    }
    
    //_SWDMX_FlushReqMsgQ(u1SrcId);  !!!
    _SWDMX_WMVFlushFeederDataQ(prSwdmxInst);
    _rSwdmxWmvInfo.fgFirstReqFeederData = TRUE;
    _rSwdmxWmvInfo.u4ConsumedSize = 0;
    _SWDMX_WMVComsumeFeeder(prSwdmxInst);
    
    _rSwdmxWmvInfo.u4CurParsingPacketNo = _rBadIntlv.u4EndPatchPktNo;
    _rSwdmxWmvInfo.u8LastFileParsingPosition = _prRangeInfo->ui8_fileoffset + WMV_DATA_OBJ_HEADER +
            _rSwdmxWmvInfo.u4CurParsingPacketNo*_rSwdmxWmvInfo.u4PacketSize;

    return TRUE;
}

static VOID _WMVEndReparse(SWDMX_INFO_T* prSwdmxInst)
{
    _rBadIntlv.eState = BAD_INTLV_STT_NONE;
    _rBadIntlv.u4BeginSkipPktNo = 0;
    _rBadIntlv.u4BeginSkipPldNo = 0;
    _rBadIntlv.u8BeginSkipPktOfst = 0;
    _rBadIntlv.u4EndPatchPktNo = 0;
    _rBadIntlv.u4EndPatchPldNo = 0;
}

static VOID _WMVResetBadIntlv(SWDMX_INFO_T *prSwdmxInst)
{
    _WMVEndReparse(prSwdmxInst);
    _rBadIntlv.fgAudioEnd = FALSE;
    _rBadIntlv.fgVideoEnd = FALSE;
}
#endif  // WMV_BAD_INTLV_SUPPORT


static VOID _WMVMsgQInit(UCHAR u1SrcId)
{
    char szMsgQName[] = { "WMVFQn" };
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    if(!prSwdmxInst)
    {
        LOG(0,"instance is NULL");
        return;
    }
    
    if (_hWMVFeederDataQue == 0)
    {
        szMsgQName[5] = '0' + u1SrcId;
        //if (_u4SwdmxWmvMsgQID >= 10)
        //    _u4SwdmxWmvMsgQID = 0;
        // create receive feeder data queue
        VERIFY(x_msg_q_create(&_hWMVFeederDataQue, szMsgQName, sizeof(WMV_FEEDER_REQ_DATA_T),
                              SWMDX_WMV_FEEDER_DATA_Q_SIZE) == OSR_OK);
    }
        
    _SWDMX_WMVFlushFeederDataQ(prSwdmxInst);
}


VOID _WMVAudioErrorCallback(VOID *prInst, UINT32 u4ErrorCode)  // megaa 20090828
{
    SWDMX_INFO_T *prSwdmxInst = (SWDMX_INFO_T *)prInst;
    _rSwdmxWmvInfo.u4WmaErrorCode = u4ErrorCode;
}


static VOID _WMVInternalInit(UCHAR u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    
    if(!prSwdmxInst)
    {
        LOG(0,"instance is NULL");
        return;
    }
    
    //set audio call back function
    AUD_DrvSetWMAErrFunc(prSwdmxInst->u1AdecId,_WMVAudioErrorCallback, (VOID *)prSwdmxInst);  // megaa 20090828

#if 1  // sometimes &_eCurFMT is not 4 byte aligned so that accessing it as UINT32* will cause data abort
    {
        UINT32 u4CurFMT;

        _SWDMX_GetInfo(u1SrcId, eSWDMX_CONTAINER_TYPE,(UINT32*)(&u4CurFMT),0,0);
        _eCurFMT = (ENUM_SWDMX_FMT_T)u4CurFMT;
    }
#else
    _SWDMX_GetInfo(eSWDMX_CONTAINER_TYPE,(UINT32*)(&_eCurFMT),0,0);
#endif

    //x_memset(&(_rSwdmxWmvInfo),0,sizeof(SWDMX_WMV_INFO_T));  this has been done in common part
    
    _rSwdmxWmvInfo.u1AudioStreamId = 0xFF;
    _rSwdmxWmvInfo.fgDataExhausted = TRUE;
    _rSwdmxWmvInfo.u4NumberOfBitstream = 1;
    _rSwdmxWmvInfo.fgDemuxFinish = FALSE;
    _rSwdmxWmvInfo.fgFirstFrame = TRUE;
    _rSwdmxWmvInfo.fgFirstReqFeederData = TRUE;
    _rSwdmxWmvInfo.fgBeginPlay = TRUE;
    _rSwdmxWmvInfo.u8SeekPTS = 0xFFFFFFFF;
    _rSwdmxWmvInfo.u8SeekPTS <<= 32;
    _rSwdmxWmvInfo.u8SeekPTS += 0xFFFFFFFF;
    //_rSwdmxWmvInfo.u4AStreamId = 0xff;
    //_rSwdmxWmvInfo.u4VStreamId = 0xff;
    _rSwdmxWmvInfo.fgBlock = FALSE;
    _rSwdmxWmvInfo.u4CurParsingPacketNo = 0;
    _fgNetflixEos = FALSE;
    _fgDataExhausted = TRUE;
    _u1AudioStreamId = 0xFF;
    _u4PrevFramePTS = 0;
    _u4DefalutReqSize = 0;
    _fgReceiveAudioPes = FALSE;
    _fgReceiveVideoPes = FALSE;
#ifdef ENABLE_WMV_DRM
    _u8SampleId =0;
    _rSwdmxWmvInfo.fgIsDRM = TRUE;
#endif

    _rSwdmxWmvInfo.u2PreviousSendPayloadNo = 0;  // !!! megaa 20100321 ToDo: check if this affects stop resume

    _rSwdmxWmvInfo.fgInternalInited = TRUE;

    if (_pu1VideoFrameData == NULL)
    {
#if 1
        _pu1VideoFrameData = (UINT8 *)BSP_AllocAlignedDmaMemory(1*1024,16);
#else /* need mentor's double check, follow David's mail: "Please review swdmx internal buffer use" */
		{
			UINT32 u4Addr = 0;
			UINT32 u4Size = 1024;
			UINT32 u4Align = 16;
			SWDMX_GetInfo(u1SrcId, eSWDMX_GET_INTERNAL_BUF, &u4Addr, &u4Size, &u4Align);
			_pu1VideoFrameData = (UCHAR*)u4Addr;
		}
#endif
        if (_pu1VideoFrameData == NULL)
        {
            ASSERT(0);
        }
    }
    
#ifdef CC_NETFLIX_CMPB_SUPPORT
    if(_pu1RemainPacketData == NULL)
    {
#if 1
        _pu1RemainPacketData = (UINT8 *)BSP_AllocAlignedDmaMemory(NETFLIX_CMMP_BUFFER_SIZE_EXTRA,16);
#else /* need mentor's double check, follow David's mail: "Please review swdmx internal buffer use" */
		{
			UINT32 u4Addr = 0;
			UINT32 u4Size = NETFLIX_CMMP_BUFFER_SIZE_EXTRA;
			UINT32 u4Align = 16;
			SWDMX_GetInfo(u1SrcId, eSWDMX_GET_INTERNAL_BUF, &u4Addr, &u4Size, &u4Align);
			_pu1RemainPacketData = (UCHAR*)u4Addr;
		}
#endif
        if(_pu1RemainPacketData == NULL)
        {
            ASSERT(0);
        }
    }
#endif     
}


#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
static BOOL _WMV_AFifo_Fullness(SWDMX_INFO_T *prSwdmxInst, UINT32 u4Percentage)
{
    UINT32 u4AFullness;
    
    u4AFullness = _u4CommAFifoSize - DMX_MUL_GetEmptySize(_u1CommAudDmxId, DMX_PID_TYPE_ES_AUDIO, _u1CommAudPid);
    
    if (_u4CommAFifoSize * u4Percentage <= u4AFullness * 100)
        return TRUE;
    else
        return FALSE;
}

static VOID vWmvBIGetMsg(BOOL fgWait)
{
    UINT16 u2QIdx;
    UINT32 u4Msg;
    SIZE_T zMsgSize;
    
    // flush the BI message Q
    x_msg_q_receive(&u2QIdx, &u4Msg, &zMsgSize, &_hCommBIQ, 1, fgWait ? X_MSGQ_OPTION_WAIT : X_MSGQ_OPTION_NOWAIT);
}
#endif


#ifdef WMV_BAD_INTLV_SUPPORT
static BOOL _WMV_AFifo_Low(SWDMX_INFO_T *prSwdmxInst, UINT32 *pu4ADuration)
{
    UINT32 u4AFullness;
    
    u4AFullness = _u4AFifoSize - DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, DMX_PID_TYPE_ES_AUDIO, _rSwdmxWmvInfo.u1AudPid);
    *pu4ADuration = 1000 * u4AFullness / _u4WmaBytesPerSec;
    
    if (*pu4ADuration < _u4WmvBadIntlvThresA)
        return TRUE;
    else
        return FALSE;
}

static BOOL _WMV_VFifo_Low(SWDMX_INFO_T *prSwdmxInst, UINT32 *pu4VDuration)
{
    UINT16 u2CurQNum, u2CurQNum2, u2MaxQNum;

    VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
    if (ENABLE_NEWTRACK(prSwdmxInst))
    {
        VDEC_GetQueueInfo(prSwdmxInst->u1Vdec2Id, &u2CurQNum2, &u2MaxQNum);
    }
    else
    {
        u2CurQNum2 = u2CurQNum;
    }
    *pu4VDuration = 1000 * min(u2CurQNum, u2CurQNum2) / _rSwdmxWmvInfo.u4FrameRate;
    
    if (*pu4VDuration < _u4WmvBadIntlvThresV)
        return TRUE;
    else
        return FALSE;
}

static VOID _WMVCheckBadIntlv(SWDMX_INFO_T *prSwdmxInst, BOOL fgVideoFull)
{
    UINT32 u4ADuration, u4VDuration;
    
    if (fgVideoFull)  // video full, check audio status
    {
        if (!_rSwdmxWmvInfo.fgEnAudio)
            return;
            
        if (!_rBadIntlv.fgEverEntered && _rSwdmxWmvInfo.u4AudioCount != 0)  // tmp solution to allow only G39, G40, G41 to enter bad interleave flow
            return;
            
        if (_rBadIntlv.eState == BAD_INTLV_STT_NONE || _rBadIntlv.eState == BAD_INTLV_STT_REPARSE_V)
        {
            if (_WMV_AFifo_Low(prSwdmxInst, &u4ADuration) && !_rBadIntlv.fgAudioEnd)
            {
                LOG(3, "Audio underflow (%dms) due to bad interleave!!!\n", u4ADuration);
                _rBadIntlv.fgEverEntered = TRUE;
                _rBadIntlv.eState = BAD_INTLV_STT_TO_PATCH_A;
                _rBadIntlv.fgRecord = TRUE;
            }
        }
        else if (_rBadIntlv.eState == BAD_INTLV_STT_PATCH_V)
        {
#if defined(WAIT_AUDIO_READY)
            if (_rSwdmxWmvInfo.fgVideoPause)
            {
                _WMVStartToPlay(prSwdmxInst);
            }
#endif
            _rBadIntlv.eState = BAD_INTLV_STT_TO_REPRS_A;
            _rBadIntlv.fgRecord = TRUE;
        }
    }
    else  // audio full, check video status
    {
        if (!_rSwdmxWmvInfo.fgEnVideo)
            return;
            
        if (_rBadIntlv.eState == BAD_INTLV_STT_NONE || _rBadIntlv.eState == BAD_INTLV_STT_REPARSE_A)
        {
            if (_WMV_VFifo_Low(prSwdmxInst, &u4VDuration) && !_rBadIntlv.fgVideoEnd)
            {
                LOG(3, "Video underflow (%dms) due to bad interleave!!!\n", u4VDuration);
                _rBadIntlv.fgEverEntered = TRUE;
                _rBadIntlv.eState = BAD_INTLV_STT_TO_PATCH_V;
                _rBadIntlv.fgRecord = TRUE;
            }
        }
        else if (_rBadIntlv.eState == BAD_INTLV_STT_PATCH_A)
        {
#if defined(WAIT_AUDIO_READY)
            if (_rSwdmxWmvInfo.fgVideoPause)
            {
                _WMVStartToPlay(prSwdmxInst);
            }
#endif
            _rBadIntlv.eState = BAD_INTLV_STT_TO_REPRS_V;
            _rBadIntlv.fgRecord = TRUE;
        }
    }
}
#endif  // WMV_BAD_INTLV_SUPPORT


VOID _WMVFindMaxPacketSize(SWDMX_INFO_T *prSwdmxInst)
{
#ifdef __KERNEL__
    UINT32 u4Idx;
    UINT32 u4MaxPacketSize = 0;
    for (u4Idx = 0 ; u4Idx < _u4NumberOfBitstream ; u4Idx++)
    {
        if (_prWmvGenericInfo[u4Idx].ui4_sz_max_acces_unt > u4MaxPacketSize)
        {
            u4MaxPacketSize = _prWmvGenericInfo[u4Idx].ui4_sz_max_acces_unt;
        }
    }
    FeederNotifyHandle(prSwdmxInst->eFeederSrc, FEEDER_NOTIFY_PACKET_SIZE, (u4MaxPacketSize+WMV_NETFLIX_HEADER_SIZE));
#else
    UNUSED(_u4NumberOfBitstream);
#endif
#ifdef WMA_SEEK_OFFSET
    prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u4SampleRate=_prWmvGenericInfo[0].ui4_sz_max_bitrate;
#endif
}

BOOL _WMVDmxMoveData(SWDMX_INFO_T *prSwdmxInst, UINT8 u1InsId,UINT8 u1Idx, DMX_MM_DATA_T *prData, UINT32 u4TimeOut)
{
    BOOL fgRet = TRUE;
    prData->u1Idx = u1Idx;
#ifdef ENABLE_WMV_DRM
    if (_rSwdmxWmvInfo.fgIsDRM)
    {
        DMX_MM_DRM_AES_T rMMDrmAes;
        UINT32 u4Flag =0;
        x_memset((VOID*)&rMMDrmAes,0,sizeof(DMX_MM_DRM_AES_T));
        if (INPUT_SRC == MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            u4Flag = (UINT32)(DMX_MM_DRM_FLAG_CHUNK_LEN);
            rMMDrmAes.u4ChunkLen = prData->u4FrameSize;
            prData->fgTrigger = TRUE;
            DMX_MM_SetPDDRM(u1Idx,u4Flag,&rMMDrmAes);  // megaa 20100314 ToDo: check if DMX has new DRM APIs
            fgRet = DMX_PDDRM_MoveData(u1Idx,prData,u4TimeOut);  // megaa 20100314 ToDo: check if DMX has new DRM APIs
        }
        else if (INPUT_SRC == MM_SRC_TYPE_NETWORK_DLNA)
        {
            u4Flag = (UINT32)(DMX_MM_DRM_FLAG_CHUNK_LEN | DMX_MM_DRM_FLAG_ENCRYLEN | DMX_MM_DRM_FLAG_SAMPLE_ID);
            rMMDrmAes.u4ChunkLen = prData->u4FrameSize;
            rMMDrmAes.u4EncryLen = prData->u4FrameSize;
            x_memcpy((VOID*)&rMMDrmAes.au1SampleID[0],&_u8SampleId,8);
            prData->fgTrigger = TRUE;
            DMX_MM_SetNDDRM(u1Idx,u4Flag,&rMMDrmAes);  // megaa 20100314 ToDo: check if DMX has new DRM APIs
            fgRet = DMX_NDDRM_MoveData(u1Idx,prData,u4TimeOut);  // megaa 20100314 ToDo: check if DMX has new DRM APIs
        }
    }
    else
    {
        fgRet = _SWDMX_MoveData(u1InsId, prData);
    }
#else
    if(prData->u4Pts==0)
    {
      LOG(2,"+++++++++++++++++++++++++PTS IS ZERO++++++++++++++++++\n"); 
    }
    LOG(5, "prData->u4Pts  = %x\n",prData->u4Pts);
    fgRet = _SWDMX_MoveData(u1InsId, prData);
#endif
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
    if (fgRet && prSwdmxInst->fgIsBadInterleaved)
    {
        if (prSwdmxInst->fgEnVideoDmx)
        {
            if (!_rSwdmxWmvInfo.fgVideoPause && !_WMV_AFifo_Fullness(prSwdmxInst, _u4BIAThreshold2))
            {
                LOG(1, "A fullness under %d%, V thread sleep for a while...\n", _u4BIAThreshold2);
                x_thread_delay(_u4BIVideoDelay2);
            }
        }
        /*
        else
        {
            x_thread_delay(_u4BIAudioDelay);
        }
        */
    }
#endif
    return fgRet;
}

UINT32 _WMVParsingStartCode(UINT32 u4FrameAddr,UINT32 u4FifoStart,UINT32 u4FifoEnd)
{
    UINT32 u4StartCode = 0;

    UINT8 u1Byte1,u1Byte2,u1Byte3,u1Byte4;
#if 0
    if (!(u4FrameAddr + 4 > u4FifoEnd))
    {
        x_memcpy(&u4StartCode, (VOID*)VIRTUAL(u4FrameAddr), 4);
        u4StartCode = ((u4StartCode&0x000000FF)<<24)
                      | ((u4StartCode&0x0000FF00)<<8) | ((u4StartCode&0x00FF0000)>>8)
                      | ((u4StartCode&0xFF000000)>>24);
    }
    else
    {
        //ASSERT(0);
    }
#else
    x_memcpy(&u1Byte1,(VOID*)VIRTUAL(u4FrameAddr),1);
    x_memcpy(&u1Byte2,(VOID*)VIRTUAL((u4FrameAddr+1)%u4FifoEnd),1);
    x_memcpy(&u1Byte3,(VOID*)VIRTUAL((u4FrameAddr+2)%u4FifoEnd),1);
    x_memcpy(&u1Byte4,(VOID*)VIRTUAL((u4FrameAddr+3)%u4FifoEnd),1);
    u4StartCode = u1Byte1<<24 |u1Byte2<<16 |u1Byte3<<8 |u1Byte4;
#endif
    return u4StartCode;
}


#ifdef WMV_BAD_INTLV_DEBUG
UINT32 _u4PrintPer = 10;
void vShowMoveStatus(SWDMX_INFO_T *prSwdmxInst, UINT32 u4FileSize, UINT32 u4Sa, BOOL fgVideo)
{
    UINT8 u1Data = *(UINT8 *)u4Sa;
    
    if ((_rSwdmxWmvInfo.u4CurParsingPacketNo % _u4PrintPer == 0)/* || _rSwdmxWmvInfo.u4CurParsingPacketNo >= 1100*/)
        printf("[swmv]%02X l=%d %ca=%08X p#%d s=%d\n",
            (UINT32)u1Data,
            u4FileSize,
            fgVideo ? 'v' : 'a', 
            fgVideo ? _u4VFifoAccuLen : _u4AFifoAccuLen,
            _rSwdmxWmvInfo.u4CurParsingPacketNo,
            (int)_rBadIntlv.eState);
}
#endif


BOOL _WMVMoveData(SWDMX_INFO_T *prSwdmxInst, UINT32 u4StartAddress,UINT32 u4CurPosition ,UINT32 u4FileSize,PayloadHeader* prPayLoadHeader,BOOL fgHeadSegment)
{
    //UINT32 static u4AudioCount = 0;
    //UINT32 u4VideoCount =0;
    UINT32 u4FrameCount=0;
    UINT32 u4Data;
    //UINT32 static u4Count =0;
    DMX_MM_DATA_T rDmxMMData;
    DMX_MM_DATA_T rDmxMMData_StartCode;
    DMX_MM_DATA_T rDmxMMData_SequenceHeader;
    UINT32 u4DmxAvailSize;
    BOOL fgSendToDMX = FALSE;
    //UINT32 u4StartCode[10];
    UINT32 u4RetryTimes =0;
    UINT16 u2VdecQueueSize =0;
    UINT16 u2VdecMaxQueueSize=0;
    UINT16 u2WaitTimes=50;
    BOOL fgVideoSegment = FALSE;
    BOOL fgAudioSegment = FALSE;
    UINT8 u1Idx;
    
    if((prPayLoadHeader->u1StreamNum==_rSwdmxWmvInfo.u4VStreamId)||IS_NEWTRACKDATA(prSwdmxInst,prPayLoadHeader->u1StreamNum))
    {
       fgVideoSegment=TRUE;
       if(IS_NEWTRACKDATA(prSwdmxInst,prPayLoadHeader->u1StreamNum))
       {
         if (prSwdmxInst->ePlayMode==SWDMX_PLAY_MODE_IFRAME || _rSwdmxWmvInfo.i4WMVPlaySpeed==2000)
            return TRUE;
         
         u1Idx=_rSwdmxWmvInfo.u1Vid2Pid;
       }
       else
       {
         u1Idx=_rSwdmxWmvInfo.u1VidPid;
       }
    }
    else
    {
#ifdef WMV_2X_TO_1X_PREBUFFER
        u1Idx = _rSwdmxWmvInfo.u1AudPreBufPid[0]; 
#else
        u1Idx=_rSwdmxWmvInfo.u1AudPid;
#endif
        if (prPayLoadHeader->u1StreamNum == (UINT8)_rSwdmxWmvInfo.u4AStreamId)
        {
            fgAudioSegment = TRUE;
        }
    }
    
#ifdef WMV_BAD_INTLV_SUPPORT
    if (_rBadIntlv.eState == BAD_INTLV_STT_TO_PATCH_A || _rBadIntlv.eState == BAD_INTLV_STT_PATCH_A || _rBadIntlv.eState == BAD_INTLV_STT_REPARSE_A)
    {
        if (fgVideoSegment)
            return TRUE;  // skip video
    }
    else if (_rBadIntlv.eState == BAD_INTLV_STT_TO_PATCH_V || _rBadIntlv.eState == BAD_INTLV_STT_PATCH_V || _rBadIntlv.eState == BAD_INTLV_STT_REPARSE_V)
    {
        if (fgAudioSegment)
            return TRUE;  // skip audio
    }
    
    if (_rBadIntlv.eState == BAD_INTLV_STT_TO_PATCH_A || _rBadIntlv.eState == BAD_INTLV_STT_PATCH_A ||
        _rBadIntlv.eState == BAD_INTLV_STT_TO_PATCH_V || _rBadIntlv.eState == BAD_INTLV_STT_PATCH_V)
    {
        if (_rSwdmxWmvInfo.u4CurParsingPacketNo < _rBadIntlv.u4EndPatchPktNo)
            return TRUE;
        if (_rSwdmxWmvInfo.u4CurParsingPacketNo == _rBadIntlv.u4EndPatchPktNo && rMultiPayloadInfo.u1CurPayloadIdx < _rBadIntlv.u4EndPatchPldNo)
            return TRUE;
    }
    
    if (_rBadIntlv.fgAudioEnd && fgAudioSegment)
        return TRUE;
    if (_rBadIntlv.fgVideoEnd && fgVideoSegment)
        return TRUE;
#else
    UNUSED(fgAudioSegment);
#endif

    x_memset(&rDmxMMData,0,sizeof(DMX_MM_DATA_T));
    x_memset(&rDmxMMData_StartCode,0,sizeof(DMX_MM_DATA_T));
    x_memset(&rDmxMMData_SequenceHeader,0,sizeof(DMX_MM_DATA_T));


    if ((_eCurFMT != SWDMX_FMT_WMA_AUDIO_ES)&&(prSwdmxInst->ePlayMode==SWDMX_PLAY_MODE_IFRAME))
    {
        u2WaitTimes = TRICK_MODE_RETRY_TIMES;
    }
    else
    {
        u2WaitTimes = RETRY_TIMES;
    }

    //u4StartCode[1] = 0x0D010000;

    _rSwdmxWmvInfo.u2SendPayloadNo++;

    //VDEC_GetQueueInfo(prSwdmxInst->u1VdecId,&u2VdecQueueSize,&u2VdecMaxQueueSize);
    //LOG(0,"status : vdec queue size : %d/%d \n",u2VdecQueueSize,u2VdecMaxQueueSize);

    if (prPayLoadHeader->fgKeyFrame && fgHeadSegment && fgVideoSegment && prPayLoadHeader->u1StreamNum==(UINT8)_rSwdmxWmvInfo.u4VStreamId)
    {
        _rSwdmxWmvInfo.fgGetKeyHead = TRUE;
        _rSwdmxWmvInfo.u4IFrameOffset = _rSwdmxWmvInfo.u4CurParsingPacketNo;
        _rSwdmxWmvInfo.u4CurPacketPTS = (prPayLoadHeader->u4PTS-_rSwdmxWmvInfo.u4PreRoll)*90;

    }
    else if (!prPayLoadHeader->fgKeyFrame && fgHeadSegment && fgVideoSegment)
    {
        _rSwdmxWmvInfo.u4FrameOffset = _rSwdmxWmvInfo.u4CurParsingPacketNo;
    }

    else
    {
        //null operation
    }

    if (_rSwdmxWmvInfo.u2SendPayloadNo  < _rSwdmxWmvInfo.u2PreviousSendPayloadNo)
    {
        return TRUE;
    }

    if (fgVideoSegment&& (prSwdmxInst->ePlayMode==SWDMX_PLAY_MODE_IFRAME))
    {

		if(!_rSwdmxWmvInfo.fgGetKeyHead)
		{
           return TRUE;
		}

        if(fgHeadSegment&&prPayLoadHeader->u1StreamNum==(UINT8)_rSwdmxWmvInfo.u4VStreamId)
        {
           if(prPayLoadHeader->fgKeyFrame&&(_rSwdmxWmvInfo.u1FWKeyFrameNo==0))
           {
			   _rSwdmxWmvInfo.u1FWKeyFrameNo=1;
           }
		   else if(_rSwdmxWmvInfo.u1FWKeyFrameNo==1)
		   {
			   _rSwdmxWmvInfo.fgFinishDemuxIFrame = TRUE;
			   return TRUE;
		   }
        }
    }
    //LOG(0,"packet distance : %d\n",u4CurPosition - u4Address);
    //u4Address = u4CurPosition;

    rDmxMMData.u4BufStart = _rFeederInfo.u4StartAddr;
    rDmxMMData.u4BufEnd = _rFeederInfo.u4EndAddr;
    rDmxMMData.u4StartAddr = (u4StartAddress + u4CurPosition);

    if (rDmxMMData.u4StartAddr < rDmxMMData.u4BufStart)
    {
        rDmxMMData.u4BufStart -= FEEDER_OFFSET;
    }

    if (rDmxMMData.u4StartAddr >= rDmxMMData.u4BufEnd || rDmxMMData.u4StartAddr < rDmxMMData.u4BufStart)
    {
        LOG(1, "!!!!!!!!!!!!!!!!! wrong parm @ 660 !!!!!!!!!!!!!!!\n");
        return FALSE;
    }
    
    //if (_rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_VC1 && _rSwdmxWmvInfo.fgFileWithStartCode)
    //    rDmxMMData.u4FrameSize = (UINT32)max(6,u4FileSize);
    //else
    rDmxMMData.u4FrameSize = u4FileSize;
    rDmxMMData.u4Pts = ((prPayLoadHeader->u4PTS-_rSwdmxWmvInfo.u4PreRoll)*90);

    //rDmxMMData.fgFrameTail  = fgEndFrame;
    rDmxMMData.fgFrameHead  = fgHeadSegment;

    if (u4FileSize ==0)
        return TRUE;

    if (_rSwdmxWmvInfo.fgEnVideo &&
       ((prPayLoadHeader->u1StreamNum == (UINT8)_rSwdmxWmvInfo.u4VStreamId)||IS_NEWTRACKDATA(prSwdmxInst,prPayLoadHeader->u1StreamNum)))
    {
        if (_rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_VC1 ||
			_rSwdmxWmvInfo.eVideoType==ENUM_SWDMX_VDEC_MPEG4)
        {
            if (_rSwdmxWmvInfo.u4VideoCount == 0
#ifndef __MODEL_slt__
                &&_rSwdmxWmvInfo.u4SequenceHeaderLength>0
#endif
                )
            {
                UINT8 u1BindingByte = 0;
                // DTV00213176
                //x_memcpy((VOID*)(VIRTUAL(&_pu1VideoFrameData[0])), (VOID*)VIRTUAL(_rSwdmxWmvInfo.u4SequenceHeader), (_rSwdmxWmvInfo.u4SequenceHeaderLength));
                //_rSwdmxWmvInfo.u4SequenceHeader = (UINT32)VIRTUAL(&_pu1VideoFrameData[0]);
#ifdef __MODEL_slt__
                x_memcpy((VOID *)(VIRTUAL(&_pu1VideoFrameData[0])), (VOID*)_au1VC1SltSeqHdr, 97);
                _rSwdmxWmvInfo.u4SequenceHeader = (UINT32)VIRTUAL(&_pu1VideoFrameData[0]);
                _rSwdmxWmvInfo.u4SequenceHeaderLength = 97;
#endif

                rDmxMMData_SequenceHeader.u4BufStart = (UINT32)(_rSwdmxWmvInfo.u4SequenceHeader)&0xFFFFFFF0;
                rDmxMMData_SequenceHeader.u4BufEnd = ((UINT32)(_rSwdmxWmvInfo.u4SequenceHeader) + 0x10+188)&0xFFFFFFF0;
                rDmxMMData_SequenceHeader.u4StartAddr = _rSwdmxWmvInfo.u4SequenceHeader;//(UINT32)&_rSwdmxWmvInfo.au1SequenceHeader[0];
                rDmxMMData_SequenceHeader.u4FrameSize = (UINT32)(_rSwdmxWmvInfo.u4SequenceHeaderLength);
/*#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
                if (rDmxMMData_SequenceHeader.u4FrameSize < 128 + 16)
                {
                    rDmxMMData_SequenceHeader.u4FrameSize = 128 + 16;
                }
#endif*/
                rDmxMMData_SequenceHeader.u4Pts = 0;
                rDmxMMData_SequenceHeader.fgFrameHead  = TRUE;
                x_memcpy(&u1BindingByte,(VOID*)rDmxMMData_SequenceHeader.u4BufStart,1);
                
                 LOG(3, "%s(%d): Setup binding byte to WMV decoder1 : %02x\n", __FUNCTION__, __LINE__, u1BindingByte);
                 VDEC_SetParam(prSwdmxInst->u1VdecId, 18, (UINT32)(&u1BindingByte), 0, 0);
                if (_WMVDmxMoveData(prSwdmxInst, prSwdmxInst->u1SwdmxId,_rSwdmxWmvInfo.u1VidPid, &rDmxMMData_SequenceHeader, 500) != TRUE)
                {
                    LOG(0, "Demuxer fails to move VC-1 sequence header.\n");
                    ASSERT(0);
                    return FALSE;
                }
                
                if(ENABLE_NEWTRACK(prSwdmxInst))
                {
                    LOG(3, "%s(%d): Setup binding byte to WMV decoder2 : %02x\n", __FUNCTION__, __LINE__, u1BindingByte);
                    VDEC_SetParam(prSwdmxInst->u1Vdec2Id, 18, (UINT32)(&u1BindingByte), 0, 0);
                    if (_WMVDmxMoveData(prSwdmxInst, prSwdmxInst->u1SwdmxId,_rSwdmxWmvInfo.u1Vid2Pid, &rDmxMMData_SequenceHeader, 500) != TRUE)
                    {
                        LOG(0, "Demuxer fails to move VC-1 sequence header.\n");
                        ASSERT(0);
                        return FALSE;
                    }
                }
            }
            _rSwdmxWmvInfo.u4VideoCount++;
        }
        do
        {
#if 1
            u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, DMX_PID_TYPE_ES_VIDEO, u1Idx);
#else
            u4DmxAvailSize = DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_VIDEO); //indicate video fifo
#endif
            if(IS_NEWTRACKDATA(prSwdmxInst,prPayLoadHeader->u1StreamNum))
            {
               VDEC_GetQueueInfo(prSwdmxInst->u1Vdec2Id,&u2VdecQueueSize,&u2VdecMaxQueueSize);
            }
            else
            {
               VDEC_GetQueueInfo(prSwdmxInst->u1VdecId,&u2VdecQueueSize,&u2VdecMaxQueueSize);
            }
#if 1
            if (_rSwdmxWmvInfo.fgVideoNormalPlay && fgHeadSegment && prPayLoadHeader->fgKeyFrame)
            {
                _rSwdmxWmvInfo.u4TriggerAudioPTS = rDmxMMData.u4Pts;
                _rSwdmxWmvInfo.u4DisplayPTS = rDmxMMData.u4Pts;
                STC_StopStc(prSwdmxInst->u1StcId);
                STC_SetStcValue(prSwdmxInst->u1StcId, (UINT32)(rDmxMMData.u4Pts-3000));
                if (_rSwdmxWmvInfo.fgEnVideo)//protect flag
                {
                    _rSwdmxWmvInfo.fgVideoNormalPlay = FALSE;
                }
            }
            else if (_rSwdmxWmvInfo.fgVideoNormalPlay)
            {
                return TRUE;
            }
#endif

            //if((u4FileSize <= u4DmxAvailSize) && (u2VdecQueueSize <= 3*30))
            if ((u4FileSize + 4 <= u4DmxAvailSize) && ((u2VdecMaxQueueSize - u2VdecQueueSize) >256))  // megaa: add 4 for start code's space!
            {
                if (prPayLoadHeader->u4OffsetIntoMediaObj == 0 && _rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_VC1
                        && !_rSwdmxWmvInfo.fgFileWithStartCode)  // megaa 20100331 ToDo: check who added the "&!_rSwdmxWmvInfo.fgFileWithStartCode"
                {
                    UINT32 u4PreStartCode = _WMVParsingStartCode(rDmxMMData.u4StartAddr,rDmxMMData.u4BufStart,rDmxMMData.u4BufEnd);
                    if (!(u4PreStartCode ==0x10D || u4PreStartCode==0x10E || u4PreStartCode==0x10C || u4PreStartCode==0x10F))
                    {
                        UINT32 u4Tmp = 0x0D010000;
                        x_memcpy((VOID*)(VIRTUAL((UINT32)&_pu1VideoFrameData[0])),(VOID*)&u4Tmp,4);

                        //x_memcpy((VOID*)(VIRTUAL(&_pu1VideoFrameData[4])),(VOID*)rDmxMMData.u4StartAddr,rDmxMMData.u4FrameSize);
                        rDmxMMData_StartCode.u4BufStart = VIRTUAL((UINT32)(_pu1VideoFrameData));
                        rDmxMMData_StartCode.u4BufEnd = ((UINT32)( rDmxMMData_StartCode.u4BufStart+1*1024) + 0x10+188)&0xFFFFFFF0;
                        rDmxMMData_StartCode.u4StartAddr = VIRTUAL((UINT32)(_pu1VideoFrameData));
                        rDmxMMData_StartCode.u4FrameSize = 4;
                        rDmxMMData_StartCode.u4Pts = rDmxMMData.u4Pts;
                        rDmxMMData_StartCode.fgFrameHead= TRUE;

                        //if((_rSwdmxWmvInfo.i4WMVPlaySpeed > 2000 || _rSwdmxWmvInfo.i4WMVPlaySpeed <= -2000))
                        //    LOG(0,"send addition start code : %x\n",rDmxMMData.u4Pts);

                        if (_WMVDmxMoveData(prSwdmxInst,prSwdmxInst->u1SwdmxId,u1Idx,&rDmxMMData_StartCode, 500) != TRUE)
                        {
                            LOG(0, "Demuxer fails to move frame start_code data.\n");
                            return FALSE;
                        }
                    }
                    else
                    {
                        _rSwdmxWmvInfo.fgFileWithStartCode = TRUE;
                        //LOG(0,"!!!!!!! wmv error bug\n");
                    }

                }
                //else
                {
#ifdef ENABLE_WMV_DRM
                    x_memcpy((VOID*)&_u1VideoFrameData[prPayLoadHeader->u4OffsetIntoMediaObj],
                             (VOID*)rDmxMMData.u4StartAddr,u4FileSize);

                    if ((prPayLoadHeader->u4OffsetIntoMediaObj + u4FileSize == prPayLoadHeader->u4MediaObjSize))
                    {
                        rDmxMMData.u4BufStart = (UINT32)(&(_u1VideoFrameData[0]))&0xFFFFFFF0;
                        rDmxMMData.u4BufEnd = ((UINT32)(&(_u1VideoFrameData[100*1024])) + 0x10+188)&0xFFFFFFF0;
                        rDmxMMData.u4StartAddr = (UINT32)&(_u1VideoFrameData[0]);
                        rDmxMMData.u4FrameSize = prPayLoadHeader->u4MediaObjSize;// + rDmxMMData.u4FrameSize;
                        //rDmxMMData.u4Pts = rDmxMMData.u4Pts;
                        rDmxMMData.fgFrameHead= TRUE;
                        if (_WMVDmxMoveData(prSwdmxInst,prSwdmxInst->u1SwdmxId,u1Idx,&rDmxMMData, 500) != TRUE)
                        {
                            LOG(0, "Demuxer fails to move video data.\n");
                            return FALSE;
                        }
                    }
#else
                    if (_WMVDmxMoveData(prSwdmxInst,prSwdmxInst->u1SwdmxId,u1Idx, &rDmxMMData, 500) != TRUE)
                    {
                        LOG(0, "Demuxer fails to move video data.\n");
                        return FALSE;
                    }
#ifdef WMV_BAD_INTLV_DEBUG
                    _u4VFifoAccuLen += u4FileSize;
                    vShowMoveStatus(prSwdmxInst, u4FileSize, rDmxMMData.u4StartAddr, TRUE);
#endif
#endif
                }
                if (_rSwdmxWmvInfo.u2PreviousSendPayloadNo >0)
                {
                    LOG(6,"video start at packet %d , payload %d\n",_rSwdmxWmvInfo.u4CurParsingPacketNo,_rSwdmxWmvInfo.u2SendPayloadNo);
                    x_memcpy(&u4Data,(void*)(VIRTUAL(u4CurPosition+u4StartAddress)),4);
                    LOG(6,"video start again , first 4 bytes : %x\n",u4Data);
                }
                u4RetryTimes = 0;
                //_rSwdmxWmvInfo.u2SendPayloadNo++;
                _rSwdmxWmvInfo.fgBlock = FALSE;
                _rSwdmxWmvInfo.u2PreviousSendPayloadNo =0;
                fgSendToDMX = TRUE;
            }
            else
            {
                //LOG(0,"dmx doesn't have available size to send payload data\n");
                if (u4RetryTimes >= u2WaitTimes || 
					(_rSwdmxWmvInfo.fgPlayAbort&&u2WaitTimes==TRICK_MODE_RETRY_TIMES))
                {
#ifdef DISABLE_AUDIO_IN_BAD_INTERLEAVE
 #if 1
                    //UINT8 u1PidIdx = prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid;
                    UINT32 u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, DMX_PID_TYPE_ES_AUDIO, u1Idx);
 #else
                    UINT32 u4DmxAvailSize = DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_AUDIO); //indicate video fifo
 #endif
                    UINT32 u4QueueSize = (288*1024 - u4DmxAvailSize);
                    UINT32 u4Ratio = (100*u4QueueSize)/(288*1024);
                    if (u4Ratio <10 && _rSwdmxWmvInfo.fgEnAudio)
                        _rSwdmxWmvInfo.fgEnAudio = FALSE;
                    //LOG(0,"audio FIFO size : %d %d\n",u4QueueSize,((100*u4QueueSize)/(288*1024)));
#endif
                    _rSwdmxWmvInfo.fgBlock = TRUE;
                    _rSwdmxWmvInfo.u2PreviousSendPayloadNo = _rSwdmxWmvInfo.u2SendPayloadNo;
                    x_memcpy(&u4Data,(void*)(VIRTUAL(u4CurPosition+u4StartAddress)),4);
                    LOG(6,"video payload size : %d/%d \n",u4FileSize,u4DmxAvailSize);
                    LOG(6,"vdec queue size : %d/%d \n",u2VdecQueueSize,u2VdecMaxQueueSize);
                    LOG(6,"video stop at packet %d , payload %d\n",_rSwdmxWmvInfo.u4CurParsingPacketNo,_rSwdmxWmvInfo.u2SendPayloadNo);
                    LOG(6,"video stop at first 4 bytes : %x\n",u4Data);
                    u4RetryTimes = 0;
#if defined(WAIT_AUDIO_READY) && !defined(WMV_BAD_INTLV_SUPPORT)
                    if (_rSwdmxWmvInfo.fgVideoPause)
                    {
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
                        if (prSwdmxInst->fgIsBadInterleaved)
                        {
                            if (_WMV_AFifo_Fullness(prSwdmxInst, _u4BIAThreshold))
                            {
                                LOG(1, "V full with A %d%, start to play!!!\n", _u4BIAThreshold);
                                _WMVStartToPlay(prSwdmxInst);
                            }
                            else
                            {
                                LOG(1, "V full but A under %d%, suspend!!!\n", _u4BIAThreshold);
                                vWmvBIGetMsg(TRUE);
                                LOG(1, "V full and A over %d%, start to play!!!\n", _u4BIAThreshold);
                                _WMVStartToPlay(prSwdmxInst);
                            }
                        }
#else
                        _WMVStartToPlay(prSwdmxInst);
#endif
                    }
#endif
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
                    if (prSwdmxInst->fgIsBadInterleaved && !_rSwdmxWmvInfo.fgVideoPause)
                    {
                        LOG(3, "V full, sleep for a while......................\n");
                        x_thread_delay(_u4BIVideoDelay);
                    }
#endif
#ifdef WMV_BAD_INTLV_SUPPORT
                    if (_fgEnBadIntlv)
                    {
                        _WMVCheckBadIntlv(prSwdmxInst, TRUE);
                    }
                    else
                    {
                        if (_rSwdmxWmvInfo.fgVideoPause)
                        {
                            _WMVStartToPlay(prSwdmxInst);
                        }
                    }
#endif
					prSwdmxInst->fgFifoFull=TRUE;
                    return FALSE;
                }
                u4RetryTimes++;
                x_thread_delay(1);
                fgSendToDMX = FALSE;
            }
        } while (!fgSendToDMX);
        if (fgHeadSegment)
            u4FrameCount++;
        _rSwdmxWmvInfo.u4VideoCount++;
        _rSwdmxWmvInfo.u4InvalidStreamNumCount = 0;
        //_rSwdmxWmvInfo.u4LastVideoPTS = rDmxMMData.u4Pts;
    }
    else if ((prPayLoadHeader->u1StreamNum == (UINT8)_rSwdmxWmvInfo.u4AStreamId) &&
#ifdef WMV_2X_TO_1X_PREBUFFER
             (_rSwdmxWmvInfo.fgEnAudio || _rSwdmxWmvInfo.fg2XAudParsing) &&
#else
             _rSwdmxWmvInfo.fgEnAudio &&
#endif
             !_rSwdmxWmvInfo.fgChangeAudio &&

#ifdef WMV_SEEK_OFFSET
             !_prRangeList->fgSetRenderPosition &&
#endif

#ifndef WMA_TRICK_MODE
#ifdef WMV_2X_TO_1X_PREBUFFER
            (_rSwdmxWmvInfo.i4WMVPlaySpeed == 1000 || _rSwdmxWmvInfo.fg2XAudParsing)
#else
            (_rSwdmxWmvInfo.i4WMVPlaySpeed == 1000)
#endif
#else
            (
#ifdef WMV_2X_TO_1X_PREBUFFER
            (_rSwdmxWmvInfo.i4WMVPlaySpeed == 1000 || _rSwdmxWmvInfo.fg2XAudParsing)
#else
            (_rSwdmxWmvInfo.i4WMVPlaySpeed == 1000)
#endif

            ||(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
            )
#endif
            )
    {
#if 1
        UINT32 u4RetryAudio =0;
        do {
            if (_rSwdmxWmvInfo.u4WmaErrorCode)
            {
                if (_rSwdmxWmvInfo.fgFirstAudioError)
                    return TRUE;
                LOG(0,"wma error code %x\n",_rSwdmxWmvInfo.u4WmaErrorCode);

                //enable video to play
#ifdef WAIT_AUDIO_READY
                if (_rSwdmxWmvInfo.fgVideoPause)
                {
                    _WMVStartToPlay(prSwdmxInst);
                }
#endif
                _rSwdmxWmvInfo.fgFirstAudioError = TRUE;
                return TRUE;
            }
#ifdef WMV_2X_TO_1X_PREBUFFER
            if (_rSwdmxWmvInfo.fg2XAudParsing)
                u4DmxAvailSize = 0xFFFFFFFF;
            else
 #if 1
            {
                u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, DMX_PID_TYPE_ES_AUDIO, u1Idx);
            }
 #else
                u4DmxAvailSize = DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_AUDIO); //indicate audio fifo
 #endif
#else
 #if 1
            u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, DMX_PID_TYPE_ES_AUDIO,u1Idx);
 #else
            u4DmxAvailSize = DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_AUDIO); //indicate audio fifo
 #endif
#endif

            if (_rSwdmxWmvInfo.fgAudioNormalPlay && fgHeadSegment
                    && (rDmxMMData.u4Pts >= (_rSwdmxWmvInfo.u4DisplayPTS+3000)
                        && (rDmxMMData.u4Pts>=(prSwdmxInst->rCurRangeReq.u8StartAudPts+3000) || (_rSwdmxWmvInfo.u4DisplayPTS==0))))
            {
                LOG(0,"\nWMV start PTS : %x\n",rDmxMMData.u4Pts);
                //VDP_TriggerAudReceive(rDmxMMData.u4Pts);
                _rSwdmxWmvInfo.u4TriggerAudioPTS = rDmxMMData.u4Pts;
                _rSwdmxWmvInfo.u4CurPTS = rDmxMMData.u4Pts;//DTV00213648
                prSwdmxInst->rCurRangeReq.u8StartAudPts=0;
                AUD_SetStartPts(prSwdmxInst->u1AdecId,rDmxMMData.u4Pts);
#ifdef WMV_2X_TO_1X_PREBUFFER
                DMX_AudHandler_SetActivePidx(_rSwdmxWmvInfo.u1AudPreBufPid[0]);  // !!!
#endif
                if (_rSwdmxWmvInfo.fgEnAudio)//protect flag
                {
                    _rSwdmxWmvInfo.fgAudioNormalPlay = FALSE;
                }
            }
            else if (_rSwdmxWmvInfo.fgAudioNormalPlay)
                //else if(_rSwdmxWmvInfo.fgAudioNormalPlay && !fgHeadSegment )
            {
                return TRUE;
            }

            if ((u4FileSize <= u4DmxAvailSize))
            {
                if (_WMVDmxMoveData(prSwdmxInst,prSwdmxInst->u1SwdmxId,u1Idx, &rDmxMMData, 500) != TRUE)
                {
                    LOG(0, "Demuxer fails to move audio data.\n");
                    return FALSE;
                }
                //printf("Audio pkt pts = %d (%d sec)\n", rDmxMMData.u4Pts, rDmxMMData.u4Pts / 90000);
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
                if (prSwdmxInst->fgIsBadInterleaved)
                {
                    if (_WMV_AFifo_Fullness(prSwdmxInst, _u4BIAThreshold))
                    {
                        UINT32 u4Msg = 0;
                        x_msg_q_send(_hCommBIQ, &u4Msg, sizeof(UINT32), 1);
                    }
                }
#endif
#ifdef WMV_BAD_INTLV_DEBUG
                _u4AFifoAccuLen += u4FileSize;
                vShowMoveStatus(prSwdmxInst, u4FileSize, rDmxMMData.u4StartAddr, FALSE);
#endif
                if (_rSwdmxWmvInfo.u2PreviousSendPayloadNo >0) {
                    LOG(6,"audio start at packet %d , payload %d\n",_rSwdmxWmvInfo.u4CurParsingPacketNo,_rSwdmxWmvInfo.u2SendPayloadNo);
                    x_memcpy(&u4Data,(void*)(VIRTUAL(u4CurPosition+u4StartAddress)),4);
                    LOG(6,"audio start again , first 4 bytes : %x\n",u4Data);
                }
                _rSwdmxWmvInfo.u4CurPTS = rDmxMMData.u4Pts;//DTV00213648
                u4RetryAudio =0;
                fgSendToDMX = TRUE;
				if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
				{
					_rSwdmxWmvInfo.fgBlock = FALSE;
					_rSwdmxWmvInfo.u2PreviousSendPayloadNo = 0;
				}
                //LOG(0,"swdmx send audio pes : %x\n",rDmxMMData.u4Pts);
            }
            else
            {
                if (u4RetryAudio >= u2WaitTimes)
                {
                    _rSwdmxWmvInfo.fgBlock = TRUE;
                    _rSwdmxWmvInfo.u2PreviousSendPayloadNo = _rSwdmxWmvInfo.u2SendPayloadNo;
					LOG(5,"Audio fifo full,pts=0x%x\n",rDmxMMData.u4Pts);
					x_thread_delay(20);
                    //x_memcpy(&u4Data,(void*)(VIRTUAL(u4CurPosition+u4StartAddress)),4);
                    //LOG(6,"audio payload size : %d/%d \n",u4FileSize,u4DmxAvailSize);
                    //LOG(6,"audio stop at packet %d , payload %d\n",_rSwdmxWmvInfo.u4CurParsingPacketNo,_rSwdmxWmvInfo.u2SendPayloadNo);
                    //LOG(6,"audio stop at first 4 bytes : %x\n",u4Data);
#ifdef WMV_BAD_INTLV_SUPPORT
                    if (_fgEnBadIntlv)
                    {
                        _WMVCheckBadIntlv(prSwdmxInst, FALSE);
                    }
#endif
					prSwdmxInst->fgFifoFull=TRUE;
                    return FALSE;
                }
                u4RetryAudio++;
                x_thread_delay(1);
                fgSendToDMX = FALSE;
            }
        } while (!fgSendToDMX);
        _rSwdmxWmvInfo.u4AudioCount++;
        _rSwdmxWmvInfo.u4InvalidStreamNumCount = 0;
        //u4AudioCount++;
#endif
    }
    else
    {
        // for DTV00367006, if continuous invalid stream number, the file might be broken.
        if (prPayLoadHeader->u1StreamNum == 0)
        {
            _rSwdmxWmvInfo.u4InvalidStreamNumCount++;
            if (_rSwdmxWmvInfo.u4InvalidStreamNumCount > WMV_INVALID_STREAM_NUM_THREAHOLD)
            {
                LOG(0, "File is broken: invalid stream number: %d\n", prPayLoadHeader->u1StreamNum);
                _rSwdmxWmvInfo.fgBrokenFile = TRUE;
                return FALSE;
            }
        }
        else
        {
            _rSwdmxWmvInfo.u4InvalidStreamNumCount = 0;
        }
    }

    return TRUE;
}


static BOOL _WMVParrsingMultiPayload(SWDMX_INFO_T *prSwdmxInst, UINT32 u4ParsingCount,PayloadParseInfo* prPayLoadInfo,UINT32 u4PacketStartAddr,UINT32 u4CurrentPos)
{
    //UINT32 u4PayloadSize=0;
    //MultiPayloadInfo rMultiPayloadInfo;

    UINT8 u1Tmp=0;
    UINT8 index=0;
    BOOL fgRet = TRUE;

    x_memset(&rMultiPayloadInfo,0,sizeof(MultiPayloadInfo));

    //u4PayloadSize = (prPayLoadInfo->u4PacketLength) - u4ParsingCount - (prPayLoadInfo->u4PaddingLength);
    x_memcpy(&u1Tmp,((VOID*)VIRTUAL(u4PacketStartAddr+u4CurrentPos)),1);
    u4CurrentPos += 1;
    rMultiPayloadInfo.u1PayloadsNumber = u1Tmp &0x3F;
    rMultiPayloadInfo.u1PayloadLengthType = (u1Tmp>>6) & 0x3;
    rMultiPayloadInfo.u4FilePos = u4CurrentPos;
    //LOG(0,"number of payloads : %d\n",rMultiPayloadInfo.u1PayloadsNumber);
    //do {
    for (index=0 ; index<rMultiPayloadInfo.u1PayloadsNumber ; index++) {
        rMultiPayloadInfo.u1CurPayloadIdx = index;
        fgRet = _WMVParsingPayload(prSwdmxInst, u4ParsingCount,prPayLoadInfo,u4PacketStartAddr,rMultiPayloadInfo.u4FilePos,&rMultiPayloadInfo);
        //fgRet = WMVParsingPayload(rMultiPayloadInfo.u4FilePos,prPayLoadInfo,u4PacketStartAddr,rMultiPayloadInfo.u4FilePos,&rMultiPayloadInfo);
        
        if (fgRet == FALSE)
        {
#ifdef WMV_BAD_INTLV_SUPPORT
            if (_rBadIntlv.eState == BAD_INTLV_STT_NONE || _rBadIntlv.eState == BAD_INTLV_STT_REPARSE_A || _rBadIntlv.eState == BAD_INTLV_STT_REPARSE_V)
            {
                return FALSE;  // return due to FIFO being blocked
            }
            else if (_rBadIntlv.fgRecord)
            {
                FEEDER_REQ_DATA_T *prFdr = &_rFeederReqData;
                
                _rBadIntlv.fgRecord = FALSE;
                if (_rBadIntlv.eState == BAD_INTLV_STT_TO_PATCH_A || _rBadIntlv.eState == BAD_INTLV_STT_TO_PATCH_V)
                {
                    _rBadIntlv.u4BeginSkipPktNo = _rSwdmxWmvInfo.u4CurParsingPacketNo;
                    _rBadIntlv.u4BeginSkipPldNo = index;
                    _rBadIntlv.u8BeginSkipPktOfst = prFdr->u8FilePos - prFdr->u4ReadSize + prFdr->u4AlignOffset;
                    // do not return because the latter payloads may be what we want to patch!
                }
                else if (_rBadIntlv.eState == BAD_INTLV_STT_TO_REPRS_V || _rBadIntlv.eState == BAD_INTLV_STT_TO_REPRS_A)
                {
                    _rBadIntlv.u4EndPatchPktNo = _rSwdmxWmvInfo.u4CurParsingPacketNo;
                    _rBadIntlv.u4EndPatchPldNo = index;
                    return FALSE;  // return due to FIFO being blocked and we have to reparse another FIFO immediately
                }
            }
#else
            return FALSE;
#endif
        }
    }
    return TRUE;
    //}while(rMultiPayloadInfo.u4FilePos < u4PayloadSize);
}

BOOL _WMVParsingPayload(SWDMX_INFO_T *prSwdmxInst, UINT32 u4ParsingCount,PayloadParseInfo* prPayLoadInfo,UINT32 u4PacketStartAddr,UINT32 u4CurrentPos,MultiPayloadInfo* prMultiPayloadInfo)
{
    //PayloadHeader rPayLoadHeader;

    UINT8 u1Tmp =0;
    UINT32 u4PayloadLength =0;
    //UINT32 u4ParsingCount =0;
    x_memset(&rPayLoadHeader,0,sizeof(PayloadHeader));

    //ASSERT(((u4PacketStartAddr+u4CurrentPos)&(0xF0000000)) ==0 );


    //ASSERT((u4PacketStartAddr+u4CurrentPos) < _rFeederInfo.u4EndAddr);

    //LOG(0,"address : %x\n",u4PacketStartAddr+u4CurrentPos);
#ifdef WMV_ERROR_HANDLE_ON
    if ((u4CurrentPos& 0xFF000000) != 0)
    {
        _rSwdmxWmvInfo.fgBrokenFile = TRUE;
        LOG(0,"\n\n!!!!!!!!!!!!!!!! Inst#%d: The file is broken1\n\n\n", prSwdmxInst->u1SwdmxId);
        return FALSE;
    }
#endif
    //media stream number , there are multiple stream in packet , it represents for multi video stream or audio stream
    x_memcpy(&u1Tmp,((VOID*)VIRTUAL(u4PacketStartAddr+u4CurrentPos)),1);
    //LOG(0,"u1Tmp : %x\n",u1Tmp);
    //LOG(0,"payload index : %x\n",u4CurrentPos);
    rPayLoadHeader.u1StreamNum = u1Tmp &0x7F;
    rPayLoadHeader.fgKeyFrame = (u1Tmp>>7) & 0x1;
    u4CurrentPos += 1;
    u4ParsingCount += 1;

    //LOG(0,"u1StreamNum : %d\n",rPayLoadHeader.u1StreamNum);

    //media object number , it represent vidoe/audio frame no.
    if (prPayLoadInfo->u1MediaObjectNumType != 0) {
        x_memcpy(&rPayLoadHeader.u4MediaObjNum,(VOID*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),TypeTransform((prPayLoadInfo->u1MediaObjectNumType)));
        u4CurrentPos += TypeTransform((prPayLoadInfo->u1MediaObjectNumType));
        u4ParsingCount += TypeTransform((prPayLoadInfo->u1MediaObjectNumType));
        //LOG(0,"u4MediaObjNum : %d\n",rPayLoadHeader.u4MediaObjNum);
    }
    else 
    {
        //ASSERT((prPayLoadInfo->u1MediaObjectNumType));
    }
    //OffsetInto , it represent the offset of corresponding position in that video/audio object
    if (prPayLoadInfo->u1OffsetIntoType != 0) {
        x_memcpy(&rPayLoadHeader.u4OffsetIntoMediaObj,(VOID*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),TypeTransform((prPayLoadInfo->u1OffsetIntoType)));
        u4CurrentPos += TypeTransform((prPayLoadInfo->u1OffsetIntoType));
        u4ParsingCount += TypeTransform((prPayLoadInfo->u1OffsetIntoType));
        //LOG(0,"u4OffsetIntoMediaObj : %d\n",rPayLoadHeader.u4OffsetIntoMediaObj);
    }
    else 
    {
        //ASSERT((prPayLoadInfo->u1OffsetIntoType));
    }
    // , it represent the offset of corresponding position in that video/audio object
    if (prPayLoadInfo->u1ReplicatedDataType!= 0) {
        x_memcpy(&rPayLoadHeader.u4ReplicatedDataLen,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),TypeTransform((prPayLoadInfo->u1ReplicatedDataType)));
        u4CurrentPos += TypeTransform((prPayLoadInfo->u1ReplicatedDataType));
        u4ParsingCount += TypeTransform((prPayLoadInfo->u1ReplicatedDataType));
        //LOG(0,"u4ReplicatedDataLen : %d\n",rPayLoadHeader.u4ReplicatedDataLen);
    }
    else 
    {
        //ASSERT((prPayLoadInfo->u1ReplicatedDataType));
    }
    //parsing single payload
    if (rPayLoadHeader.u4ReplicatedDataLen != 1) {
        //parsing replicated data (PTS and media object size)
        x_memcpy(&rPayLoadHeader.u4MediaObjSize,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),4);
        u4CurrentPos += 4;
        u4ParsingCount += 4;
        x_memcpy(&rPayLoadHeader.u4PTS,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),4);
        u4CurrentPos += 4;
        u4ParsingCount +=4;
        //adding sample id info
#ifdef ENABLE_WMV_DRM
        if (rPayLoadHeader.u4ReplicatedDataLen >= 16)
        {
            UINT32 u4Offset = rPayLoadHeader.u4ReplicatedDataLen -16;
            x_memcpy(&_u8SampleId,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos+u4Offset),8);
        }
#endif
        u4CurrentPos += rPayLoadHeader.u4ReplicatedDataLen - 8;
        u4ParsingCount += rPayLoadHeader.u4ReplicatedDataLen - 8;
        //parsing payload data
        if (prMultiPayloadInfo != NULL) {
            //spork , error handling
#ifdef WMV_ERROR_HANDLE_ON
            if ( (u4CurrentPos& 0xFF000000) != 0)
            {
                _rSwdmxWmvInfo.fgBrokenFile = TRUE;
                LOG(0,"\n\n!!!!!!!!!!!!!!!! Inst#%d: The file is broken2\n\n\n", prSwdmxInst->u1SwdmxId);
                return FALSE;
            }
#endif
            x_memcpy(&u4PayloadLength,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),TypeTransform((prMultiPayloadInfo->u1PayloadLengthType)));
            u4CurrentPos += TypeTransform((prMultiPayloadInfo->u1PayloadLengthType));
            prMultiPayloadInfo ->u4FilePos = u4CurrentPos+u4PayloadLength;
            /*if(((prMultiPayloadInfo ->u4FilePos)&(0xF0000000)) !=0 ) {
                _rSwdmxWmvInfo.u4FilePos = prMultiPayloadInfo ->u4FilePos;
                return FALSE;
            }*/
            //LOG(0,"filepos : %x ,multi payload length : %d %d\n",prMultiPayloadInfo ->u4FilePos,u4PayloadLength,(prMultiPayloadInfo->u1PayloadLengthType));
#ifdef WMV_BAD_INTLV_SUPPORT
            if (_rBadIntlv.eState == BAD_INTLV_STT_REPARSE_V || _rBadIntlv.eState == BAD_INTLV_STT_REPARSE_A)
            {
                if ((_rSwdmxWmvInfo.u4CurParsingPacketNo < _rBadIntlv.u4BeginSkipPktNo) ||
                    (_rSwdmxWmvInfo.u4CurParsingPacketNo == _rBadIntlv.u4BeginSkipPktNo && prMultiPayloadInfo->u1CurPayloadIdx < _rBadIntlv.u4BeginSkipPldNo))
                {
                    return TRUE;
                }
                if (_rSwdmxWmvInfo.u4CurParsingPacketNo == _rBadIntlv.u4EndPatchPktNo &&
                    prMultiPayloadInfo->u1CurPayloadIdx == _rBadIntlv.u4EndPatchPldNo)
                {
                    _WMVEndReparse(prSwdmxInst);
                }
            }
#endif
        }
        else 
        {
            u4PayloadLength = prPayLoadInfo->u4PacketLength - u4ParsingCount - (prPayLoadInfo->u4PaddingLength);
            //LOG(0,"payload size : %x %x %x %x %x\n",u4PayloadLength,prPayLoadInfo->u4PacketLength,u4CurrentPos,u4ParsingCount,prPayLoadInfo->u4PaddingLength);
#ifdef WMV_BAD_INTLV_SUPPORT
            if (_rBadIntlv.eState == BAD_INTLV_STT_REPARSE_V || _rBadIntlv.eState == BAD_INTLV_STT_REPARSE_A)
            {
                if (_rSwdmxWmvInfo.u4CurParsingPacketNo < _rBadIntlv.u4BeginSkipPktNo)
                {
                    return TRUE;
                }
                if (_rSwdmxWmvInfo.u4CurParsingPacketNo == _rBadIntlv.u4EndPatchPktNo)
                {
                    _WMVEndReparse(prSwdmxInst);
                }
            }
#endif
        }
        //u4PayloadLength = prPayLoadInfo->u4PacketLength - u4ParsingCount - (prPayLoadInfo->u4PaddingLength);
        //printf("payload size : %x %x %x %x %x\n",u4PayloadLength,prPayLoadInfo->u4PacketLength,u4CurrentPos,u4ParsingCount,prPayLoadInfo->u4PaddingLength);
        /*
        if((rPayLoadHeader.u4OffsetIntoMediaObj + u4PayloadLength) == rPayLoadHeader.u4MediaObjSize) {
            DMAMove(u4PacketStartAddr,u4CurrentPos,u4PayloadLength,&rPayLoadHeader,TRUE);
        }
        else {
            DMAMove(u4PacketStartAddr,u4CurrentPos,u4PayloadLength,&rPayLoadHeader,FALSE);
        }
        */
        if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
        {
           if(_rSwdmxWmvInfo.i4WMVPlaySpeed<0&&(!_rSwdmxWmvInfo.fgAudioNormalPlay))
           {
               if(_rSwdmxWmvInfo.u4IndexPts==0)
               {
                 _rSwdmxWmvInfo.u4IndexPts=rPayLoadHeader.u4PTS;
               }
               else
               {
                   if(_rSwdmxWmvInfo.u4IndexPts>_rSwdmxWmvInfo.u4PreRoll)
                   {
				     _rSwdmxWmvInfo.u4IndexPts--;
                   }
               }
               rPayLoadHeader.u4PTS=_rSwdmxWmvInfo.u4IndexPts;
           }
        }
        
        if (rPayLoadHeader.u4OffsetIntoMediaObj == 0)
        {
            _WMVMoveData(prSwdmxInst, u4PacketStartAddr,u4CurrentPos,u4PayloadLength,&rPayLoadHeader,TRUE);
        }
        else
        {
            _WMVMoveData(prSwdmxInst, u4PacketStartAddr,u4CurrentPos,u4PayloadLength,&rPayLoadHeader,FALSE);
        }
        if (_rSwdmxWmvInfo.fgBlock)
        {
            if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
            {
              if(_rSwdmxWmvInfo.u4IndexPts && _rSwdmxWmvInfo.i4WMVPlaySpeed<0)
              {
                 _rSwdmxWmvInfo.u4IndexPts++;
              }
            }
            return FALSE;
        }
        /*if((rPayLoadHeader.u4OffsetIntoMediaObj + u4PayloadLength) == rPayLoadHeader.u4MediaObjSize)
            SendES();
        else
            ;//for lint free
        */
    }
    //parsing single payload with compressed payload data
    else 
    {
        UINT32 u4ParsingSize =0;
        rPayLoadHeader.u4PTS = rPayLoadHeader.u4OffsetIntoMediaObj; //in compressed payload , offset into media object field represent PTS
        x_memcpy(&rPayLoadHeader.u4PTSDelta,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),1);
        u4CurrentPos += 1;
        u4ParsingCount += 1;
        if (prMultiPayloadInfo != NULL)
        {
            x_memcpy(&u4PayloadLength,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),TypeTransform((prMultiPayloadInfo->u1PayloadLengthType)));
            u4CurrentPos += TypeTransform((prMultiPayloadInfo->u1PayloadLengthType));
            prMultiPayloadInfo ->u4FilePos = u4CurrentPos+u4PayloadLength;
#ifdef WMV_BAD_INTLV_SUPPORT
            if (_rBadIntlv.eState == BAD_INTLV_STT_REPARSE_V || _rBadIntlv.eState == BAD_INTLV_STT_REPARSE_A)
            {
                if ((_rSwdmxWmvInfo.u4CurParsingPacketNo < _rBadIntlv.u4BeginSkipPktNo) ||
                    (_rSwdmxWmvInfo.u4CurParsingPacketNo == _rBadIntlv.u4BeginSkipPktNo && prMultiPayloadInfo->u1CurPayloadIdx < _rBadIntlv.u4BeginSkipPldNo))
                {
                    return TRUE;
                }
                if (_rSwdmxWmvInfo.u4CurParsingPacketNo == _rBadIntlv.u4EndPatchPktNo &&
                    prMultiPayloadInfo->u1CurPayloadIdx == _rBadIntlv.u4EndPatchPldNo)
                {
                    _WMVEndReparse(prSwdmxInst);
                }
            }
#endif
        }
        else
        {
            u4PayloadLength = prPayLoadInfo->u4PacketLength - u4ParsingCount - (prPayLoadInfo->u4PaddingLength);
#ifdef WMV_BAD_INTLV_SUPPORT
            if (_rBadIntlv.eState == BAD_INTLV_STT_REPARSE_V || _rBadIntlv.eState == BAD_INTLV_STT_REPARSE_A)
            {
                if (_rSwdmxWmvInfo.u4CurParsingPacketNo < _rBadIntlv.u4BeginSkipPktNo)
                {
                    return TRUE;
                }
                if (_rSwdmxWmvInfo.u4CurParsingPacketNo == _rBadIntlv.u4EndPatchPktNo)
                {
                    _WMVEndReparse(prSwdmxInst);
                }
            }
#endif
        }
        do {
            UINT8 u1SubPayloadLen =0;
            x_memcpy(&u1SubPayloadLen,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),1);
            u4CurrentPos += 1;
            _WMVMoveData(prSwdmxInst, u4PacketStartAddr,u4CurrentPos,u1SubPayloadLen,&rPayLoadHeader,TRUE);
            if (_rSwdmxWmvInfo.fgBlock)
            {
#ifdef WMV_BAD_INTLV_SUPPORT
                ASSERT(0);  // !!! ToDo: find a real case to trace and figure out the solution
#endif
                return FALSE;
            }
            //SendES(); //for compressed payload data, every payload represent one media object
            u4CurrentPos += u1SubPayloadLen;
            u4ParsingSize += u1SubPayloadLen + 1;
        } while (u4ParsingSize < u4PayloadLength);
    }
    return TRUE;
}

BOOL _WMVParsingPacket(
    SWDMX_INFO_T *prSwdmxInst, 
    UINT32 u4PacketStartAddr, 
    UINT32 u4CurrentPos, 
    UINT32 u4PacketSize)
{
    //UINT32 u4CurrentPos = 0;
    //UINT64 u8TotalDataPackets=0;
    //UINT32 index=0;
    UINT32 u4ParsingCount = 0;
    BOOL fgRet;

    //x_memcpy(&u8TotalDataPackets,u4PacketStartAddr+u4CurrentPos+40,8);
    //LOG(0,"total data packet : %d \n",u8TotalDataPackets);
    //u4CurrentPos += WMV_DATA_OBJ_HEADER; //packet header size is 50 bytes

    //LOG(0,"packet start pos : %x\n",u4CurrentPos);

    //u4Address = u4CurrentPos;

    //handle other than payload paring
    //do {
    UINT8 u1Tmp =0;
    if (_prRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK_NETFLIX)
    {
#ifndef CC_NETFLIX_CMPB_SUPPORT    
        UINT8 u1IdxOfPraingBitstream=0;
        UINT8 u1PacketType =0;
        UINT8 u1EOS =0;
        UINT64 u8Remainder =0;
        x_memcpy(&u1IdxOfPraingBitstream,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),1);
        u4CurrentPos +=1;
        x_memcpy(&u1PacketType,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),1);
        u4CurrentPos +=1;
        x_memcpy(&u1EOS,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),1);
        u4CurrentPos +=1;

        //reserved bytes
        u4CurrentPos +=1;

        if (u1EOS == TRUE)
        {   
            LOG(2,"Send Eos 1\n");
            _SWDMX_WMVSendEOS(prSwdmxInst);
            return TRUE;
        }
        _rSwdmxWmvInfo.u4VStreamId = _u4NetflixVideoStreamId;
        _rSwdmxWmvInfo.u4AStreamId = _u4NetflixAudioStreamId;
        _rSwdmxWmvInfo.u4PreRoll = (UINT32)u8Div6432(_prWmvGenericInfo[u1IdxOfPraingBitstream].ui8_preroll, 90, &u8Remainder);
        _rSwdmxWmvInfo.u4PacketSize = (UINT32)_prWmvGenericInfo[u1IdxOfPraingBitstream].ui4_sz_max_acces_unt;
        //_prRangeInfo->ui4_sz_min_acces_unt = _rSwdmxWmvInfo.u4PacketSize;

        if (u1PacketType == NETFLIX_VIDEO) //video
        {
            _rSwdmxWmvInfo.u4AStreamId = 0xFF;
        }
        else if (u1PacketType == NETFLIX_AUDIO) //audio
        {
            _rSwdmxWmvInfo.u4VStreamId = 0xFF;
        }
#else //netflix cmpb mode

        static NetflixEmbeddedCmd_T _rNetflixEmbeddedCmd;
        x_memset(&_rNetflixEmbeddedCmd,0,sizeof(NetflixEmbeddedCmd_T));
        x_memcpy(&_rNetflixEmbeddedCmd,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),sizeof(NetflixEmbeddedCmd_T));

        u4CurrentPos += sizeof(NetflixEmbeddedCmd_T);

        if (_rNetflixEmbeddedCmd.fgEos )
        {
            _SWDMX_WMVSendEOS(prSwdmxInst);
            return TRUE;
        }
        

        _rSwdmxWmvInfo.u4VStreamId = _u4NetflixVideoStreamId;
        _rSwdmxWmvInfo.u4AStreamId = _u4NetflixAudioStreamId;
        
        _rSwdmxWmvInfo.u4PacketSize = (UINT32)_rNetflixEmbeddedCmd.u2PacketSize;
        //_prRangeInfo->ui4_sz_min_acces_unt = _rSwdmxWmvInfo.u4PacketSize;
        
        _rSwdmxWmvInfo.u4PreRoll = 0; //netflix spec , pts preroll always = 0

        if (_rNetflixEmbeddedCmd.u1PacketType == NETFLIX_VIDEO) //video
        {
            _rSwdmxWmvInfo.u4AStreamId = 0xFF;
        }
        else if (_rNetflixEmbeddedCmd.u1PacketType == NETFLIX_AUDIO) //audio
        {
            _rSwdmxWmvInfo.u4VStreamId = 0xFF;
        }          
#endif
    }

    x_memcpy(&u1Tmp,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),1);
    //LOG(0,"u1Tmp : %x\n",u1Tmp);
    //LOG(0,"index : %x\n",u4CurrentPos);
    //u4CurrentPos += 1;
    u4ParsingCount = 0;
#ifdef WMA_SEEK_OFFSET
    {
      x_memcpy((void *)(&_rSwdmxWmvInfo.u8PacketMarker),(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),5);
    }
#endif

    if (u1Tmp&0x80) //parsing error correction data
    {
        
#ifdef WMA_SEEK_OFFSET
      _rSwdmxWmvInfo.u4PacketMarkerLen=5;
#endif
        //UINT8 u1ErrorCorrectFlag;
        //UINT8 u1ErrorCorrectionDataLen=0;
        //UINT8 u1ErrorCorrectionLenType=0;
        //UINT8 u1ErrorCorrectionPresent=0;
        //parsing Error Correction Data
        //u1ErrorCorrectionDataLen = (u1Tmp&0xF);
        //u1ErrorCorrectionLenType = (u1Tmp>>5)&0x3;
        //u1ErrorCorrectionPresent = (u1Tmp>>7)&0x1;
        u4CurrentPos += 1;
        u4CurrentPos += 2;
        u4ParsingCount += 3;
    }
    else
    {
#ifdef WMA_SEEK_OFFSET
        _rSwdmxWmvInfo.u4PacketMarkerLen=2;
#endif

    }
    //else //parsing payload information
    {
        //UINT8 u1Length=0;

        //PayloadParseInfo rPayLoadInfo;

        x_memset(&rPayLoadInfo,0,sizeof(PayloadParseInfo));
        //LOG(0,"index : %x\n",u4CurrentPos);
        x_memcpy(&u1Tmp,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),1);
        //LOG(0,"u1Tmp : %x\n",u1Tmp);
        u4CurrentPos += 1;
        u4ParsingCount += 1;
        rPayLoadInfo.fgMultiplePayload = u1Tmp & 0x1;
        rPayLoadInfo.u1SequenceType= (u1Tmp>>1) & 0x3;
        rPayLoadInfo.u1PaddingLenghtType = (u1Tmp>>3) & 0x3;
        rPayLoadInfo.u1PacketLengthType = (u1Tmp>>5) & 0x3;
        rPayLoadInfo.fgErrorCorrectPresent = (u1Tmp>>7) & 0x1;

        x_memcpy(&u1Tmp,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),1);
        //LOG(0,"u1Tmp : %x\n",u1Tmp);
        u4CurrentPos += 1;
        u4ParsingCount += 1;
        rPayLoadInfo.u1ReplicatedDataType = (u1Tmp)&0x3;
        rPayLoadInfo.u1OffsetIntoType = (u1Tmp>>2)&0x3;
        rPayLoadInfo.u1MediaObjectNumType = (u1Tmp>>4)&0x3;
        rPayLoadInfo.u1StreamNumberType = (u1Tmp>>6)&0x3;

        if (rPayLoadInfo.u1PacketLengthType != 0)
        { //packet length
            x_memcpy(&rPayLoadInfo.u4PacketLength,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),TypeTransform((rPayLoadInfo.u1PacketLengthType)));
            u4CurrentPos += TypeTransform((rPayLoadInfo.u1PacketLengthType));
            u4ParsingCount += TypeTransform((rPayLoadInfo.u1PacketLengthType));
            //LOG(0,"packet length : %d %d\n",rPayLoadInfo.u4PacketLength,TypeTransform((rPayLoadInfo.u1PacketLengthType)));
        }
        else 
        {
            rPayLoadInfo.u4PacketLength = _rSwdmxWmvInfo.u4PacketSize; //get form upper middleware
        }
        if (rPayLoadInfo.u1SequenceType != 0) 
        { //sequence length
            x_memcpy(&rPayLoadInfo.u4Sequence,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),TypeTransform((rPayLoadInfo.u1SequenceType)));
            u4CurrentPos += TypeTransform((rPayLoadInfo.u1SequenceType));
            u4ParsingCount += TypeTransform((rPayLoadInfo.u1SequenceType));
            //LOG(0,"sequence length : %d %d\n",rPayLoadInfo.u4Sequence,TypeTransform((rPayLoadInfo.u1SequenceType)));
        }
        else 
        {
            rPayLoadInfo.u4Sequence = 0;
        }
        if (rPayLoadInfo.u1PaddingLenghtType != 0) 
        { //padding length
            x_memcpy(&rPayLoadInfo.u4PaddingLength,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),TypeTransform((rPayLoadInfo.u1PaddingLenghtType)));
            u4CurrentPos += TypeTransform((rPayLoadInfo.u1PaddingLenghtType));
            u4ParsingCount += TypeTransform((rPayLoadInfo.u1PaddingLenghtType));
            //LOG(0,"padding length : %d %d\n",rPayLoadInfo.u4PaddingLength,TypeTransform((rPayLoadInfo.u1PaddingLenghtType)));
        }
        else 
        {
            rPayLoadInfo.u4PaddingLength = 0;
        }
        x_memcpy(&rPayLoadInfo.u4SendTime,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),4);

        u4CurrentPos += 4;
        u4ParsingCount += 4;
        x_memcpy(&rPayLoadInfo.u2Duration,(void*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),2);
        u4CurrentPos += 2;
        u4ParsingCount += 2;
        //LOG(0,"index : %x\n",u4CurrentPos);
        //LOG(0,"u4ParsingCount : %x\n",u4ParsingCount);
        if (rPayLoadInfo.fgMultiplePayload)
        {
            fgRet = _WMVParrsingMultiPayload(prSwdmxInst, u4ParsingCount,&rPayLoadInfo,u4PacketStartAddr,u4CurrentPos);
        }
        else {
            rMultiPayloadInfo.u1CurPayloadIdx = 0;
            fgRet = _WMVParsingPayload(prSwdmxInst, u4ParsingCount,&rPayLoadInfo,u4PacketStartAddr,u4CurrentPos,NULL);
#ifdef WMV_BAD_INTLV_SUPPORT
            if (!fgRet && _rBadIntlv.eState != BAD_INTLV_STT_NONE)
            {
                if (_rBadIntlv.fgRecord)
                {
                    FEEDER_REQ_DATA_T *prFdr = &_rFeederReqData;
                                        
                    _rBadIntlv.fgRecord = FALSE;
                    if (_rBadIntlv.eState == BAD_INTLV_STT_TO_PATCH_A || _rBadIntlv.eState == BAD_INTLV_STT_TO_PATCH_V)
                    {
                        _rBadIntlv.u4BeginSkipPktNo = _rSwdmxWmvInfo.u4CurParsingPacketNo;
                        _rBadIntlv.u4BeginSkipPldNo = 0;
                        _rBadIntlv.u8BeginSkipPktOfst = prFdr->u8FilePos - prFdr->u4ReadSize + prFdr->u4AlignOffset;
                    }
                    else if (_rBadIntlv.eState == BAD_INTLV_STT_TO_REPRS_V || _rBadIntlv.eState == BAD_INTLV_STT_TO_REPRS_A)
                    {
                        _rBadIntlv.u4EndPatchPktNo = _rSwdmxWmvInfo.u4CurParsingPacketNo;
                        _rBadIntlv.u4EndPatchPldNo = 0;
                    }
                }
            }
#endif
        }
    }

    return fgRet;
}


VOID _WMVStartToPlay(SWDMX_INFO_T *prSwdmxInst)
{
    if (_rSwdmxWmvInfo.fgEnVideo)
    {
        VDP_SetPauseMm(prSwdmxInst->u1B2rId, FALSE);
    }

    switch (_rSwdmxWmvInfo.eVideoType)
    {
    case ENUM_SWDMX_VDEC_VC1:
#if 0  // disable WMV7/8
    case ENUM_SWDMX_VDEC_WMV7:
    case ENUM_SWDMX_VDEC_WMV8:
#endif
    case ENUM_SWDMX_VDEC_WMV9:
        VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_WMV);
        if(ENABLE_NEWTRACK(prSwdmxInst))
        {
           VDEC_Play(prSwdmxInst->u1Vdec2Id, VDEC_FMT_WMV);
        }
        break;
    case ENUM_SWDMX_VDEC_MPEG4:
    case ENUM_SWDMX_VDEC_DX311:
        VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MP4);
        if(ENABLE_NEWTRACK(prSwdmxInst))
        {
           VDEC_Play(prSwdmxInst->u1Vdec2Id, VDEC_FMT_MP4);
        }       
        break;
    case ENUM_SWDMX_VDEC_H264:
        VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_H264);
        if(ENABLE_NEWTRACK(prSwdmxInst))
        {
           VDEC_Play(prSwdmxInst->u1Vdec2Id, VDEC_FMT_H264);
        }
        break;
    default :
        //do nothing
        break;
    }
    _rSwdmxWmvInfo.fgVideoPause = FALSE;
    LOG(0,"_WMVStartToPlay start to play video\n");
}

BOOL _SWDMX_WmvAudioCallback(const DMX_AUDIO_PES_T* prPes)
{
    BOOL fgRet = TRUE;
    SWDMX_INFO_T *prSwdmxInst = (SWDMX_INFO_T*)prPes->pvInstanceTag;
    DMX_AUDIO_PES_T rDmxPes;
    x_memcpy(&rDmxPes,prPes,sizeof(DMX_AUDIO_PES_T));
    
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)|| defined(CC_MT5368)|| defined(CC_MT5389) 
    if(_rSwdmxWmvInfo.i4WMVPlaySpeed>0)
    {
        rDmxPes.fgForward=TRUE;
    }
    else
    {
       rDmxPes.fgForward=FALSE;
    }
#endif

    if (_fgReceiveAudioPes)
    {
        _rPreviousAudioPes.u4Pts = rDmxPes.u4Pts;
        fgRet =  AUD_SendAudioPes(&_rPreviousAudioPes);
        if (rDmxPes.fgEOS)
        {
            fgRet =  AUD_SendAudioPes(&rDmxPes);
            AUD_MM_Set_Avsync_event(prSwdmxInst->u1AdecId);
        }
    }
    else
    {
        
        if (rDmxPes.fgEOS)
        {
            fgRet =  AUD_SendAudioPes(&rDmxPes);
            AUD_MM_Set_Avsync_event(prSwdmxInst->u1AdecId);
        }
    }
    _fgReceiveAudioPes = TRUE;
    x_memcpy((VOID*)&_rPreviousAudioPes,(VOID*)(&rDmxPes),sizeof(DMX_AUDIO_PES_T));
    return fgRet;
}

VOID _WMVCheckDataExhausted(SWDMX_INFO_T *prSwdmxInst)
{
    UINT16 u2VdecQueueSize,u2VdecMaxQueueSize;
    u2VdecMaxQueueSize =0;
    u2VdecQueueSize =0;

    if(!MPV_IsDisplay(prSwdmxInst->u1VdecId))
    {
        return; //wait sequence header
    }

    VDEC_GetQueueInfo(prSwdmxInst->u1VdecId,&u2VdecQueueSize,&u2VdecMaxQueueSize);//maiyou
    if (u2VdecQueueSize < NETFLIX_UNDERFLOW_THRESHOLD && !_fgNetflixEos && !_fgDataExhausted &&
            _prRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK_NETFLIX)
    {
        LOG(0,"swdmx wmv netflix data exhausted 3\n");
        _fgDataExhausted = TRUE;
        FeederNotifyHandle(prSwdmxInst->eFeederSrc, FEEDER_NOTIFY_DATA_EXHAUSTED, 1);  // megaa 20090828
        _SWDMX_RANGE_CALLBACK(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->pv_tag,
                                MM_RANGE_BUFF_UNDERFLOW,
                                prSwdmxInst->rCurRangeReq.prDmxRangeInfo->ui4_range_id,
                                0xFFFFFFFF,
                                0);        
    }
    else if (u2VdecQueueSize > (NETFLIX_UNDERFLOW_THRESHOLD<<1) && _fgDataExhausted)
    {
        LOG(0,"swdmx wmv netflix data exhausted 4\n");
        _SWDMX_RANGE_CALLBACK(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->pv_tag,
                                MM_RANGE_BUFF_READY,
                                prSwdmxInst->rCurRangeReq.prDmxRangeInfo->ui4_range_id,
                                0xFFFFFFFF,
                                0);        
        _fgDataExhausted = FALSE;
    }
}

BOOL _SWDMX_WmvVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UCHAR *pucBuf;
    UINT32 u4Addr;
    UINT32 u4StartCode = 0;
    UINT32 u4RepeatTimes =0;
    SWDMX_INFO_T *prSwdmxInst;
    BOOL fgTrickMode;
    
    prSwdmxInst = (SWDMX_INFO_T*)prPes->pvInstanceTag;

    
    if (!prSwdmxInst)
    {
        LOG(1, "Swdmx Instance is NULL\n");
        return FALSE;
    }
    
    fgTrickMode = (prSwdmxInst->ePlayMode==SWDMX_PLAY_MODE_IFRAME) ? TRUE : FALSE;
    
    //UINT16 u2VdecQueueSize,u2VdecMaxQueueSize;
    //VDEC_GetQueueInfo((UCHAR)prSwdmxInst->u1VdecId,&u2VdecQueueSize,&u2VdecMaxQueueSize);

    //LOG(0,"vdec queue %d/%d\n",u2VdecQueueSize,u2VdecMaxQueueSize);

    //pu1Temp = (UINT8*)prPes->u4FrameAddr;

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    //LOG(6,"wmv call back PTS : %d\n",prPes->u4Pts);
    if(ENABLE_NEWTRACK(prSwdmxInst))
    {
      rPesInfo.ucEsId = (UCHAR)prPes->u1Channel;
    }
    else
    {
      rPesInfo.ucEsId = (UCHAR)prSwdmxInst->u1VdecId;
    }
    if(_rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_MJPEG)
    {
        rPesInfo.ucMpvId = VLD1; // video format all use VLD expect jpeg
    }
    else
    {        
        rPesInfo.ucMpvId = VLD0; // video format all use VLD expect jpeg
    }
    rPesInfo.fgDtsValid = TRUE;//prPes->fgPtsDts;
    rPesInfo.u4PTS = (prPes->u4Pts);
    rPesInfo.u4DTS = prPes->u4Dts;
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;//(prPes->u4FrameAddr - prPes->u4BufStart);
    rPesInfo.fgEos = prPes->fgEOS;

    rPesInfo.u8OffsetI = (UINT64)_rSwdmxWmvInfo.u4IFrameOffset;
    rPesInfo.u8Offset = (UINT64)_rSwdmxWmvInfo.u4FrameOffset;
	
    if (rPesInfo.fgEos)
    {
        rPesInfo.fgBackward= (_rSwdmxWmvInfo.i4WMVPlaySpeed<0) ? TRUE : FALSE;
        rPesInfo.u4VldReadPtr =0;
        rPesInfo.u4FifoStart =0;
        rPesInfo.u4FifoEnd =0;
        if (_rSwdmxWmvInfo.fgVideoPause)
        {
            _WMVStartToPlay(prSwdmxInst);
        }
    }


    _rSwdmxWmvInfo.u4LastVideoWptr = prPes->u4FrameAddr;

    switch (_rSwdmxWmvInfo.eVideoType)
    {
    case ENUM_SWDMX_VDEC_VC1:
        rPesInfo.u4VideoType = DMX_VIDEO_VC1;
        break;
    case ENUM_SWDMX_VDEC_WMV7:
        rPesInfo.u4VideoType = DMX_VIDEO_WMV7;
        break;
    case ENUM_SWDMX_VDEC_WMV8:
        rPesInfo.u4VideoType = DMX_VIDEO_WMV8;
        break;
    case ENUM_SWDMX_VDEC_WMV9:
        rPesInfo.u4VideoType = DMX_VIDEO_WMV9;
        break;
    case ENUM_SWDMX_VDEC_MPEG4:
        rPesInfo.u4VideoType = DMX_VIDEO_MPEG4;
        break;
    case ENUM_SWDMX_VDEC_H264:
        rPesInfo.u4VideoType = DMX_VIDEO_H264;
        break;
    default:
        rPesInfo.u4VideoType = DMX_VIDEO_WMV9;
        break;
    }

    if ( _rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_DX311)
    {
        u4Addr = prPes->u4FrameAddr;
        if (u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        pucBuf = (UCHAR*)VIRTUAL(u4Addr);

        if ((pucBuf[0] >> 6) & 0x1)
        {
            rPesInfo.ucPicType  = P_TYPE;
        }
        else
        {
            rPesInfo.ucPicType = I_TYPE;
        }
    }
    else if (_rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_MPEG4)
    {
        u4Addr = prPes->u4FrameAddr + 3;
        if(u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        pucBuf = (UCHAR*)VIRTUAL(u4Addr);

        if (pucBuf[0] == 0xB6)
        {
            u4Addr = prPes->u4FrameAddr + 4;
            if(u4Addr >= prPes->u4BufEnd)
            {
                u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
            }
            pucBuf = (UCHAR*)VIRTUAL(u4Addr);
            switch (pucBuf[0] >> 6)
            {
                case 0:
                    rPesInfo.ucPicType  = I_TYPE;
                    break;
                case 1: case 3:
                    rPesInfo.ucPicType  = P_TYPE;
                    break;
                case 2:
                    rPesInfo.ucPicType = B_TYPE;
                    break;
                default:
                    rPesInfo.ucPicType = UNKNOWN_TYPE;
                    break;
            }
        }
        if(fgTrickMode)
        {
            if(rPesInfo.u4PTS)
            {
                _u4PrevFramePTS = rPesInfo.u4PTS;
            }
            else
            {
                rPesInfo.u4PTS = _u4PrevFramePTS;
            }
            if(rPesInfo.ucPicType!=I_TYPE)
            {
                fgTrickMode=FALSE;
            }
        }
    }
    else if (_rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_VC1)
    {
        u4Addr = prPes->u4FrameAddr+3;
        if (u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        pucBuf = (UCHAR*)VIRTUAL(u4Addr);
        u4StartCode = pucBuf[0];
        u4StartCode |= 0x100;
        if ((u4StartCode == 0x10E || u4StartCode ==0x10F))
        {
            VDEC_SendEs((void*)&rPesInfo);
            if (rPesInfo.u4PTS!=0 || u4StartCode ==0x10F)
                _u4PrevFramePTS = (rPesInfo.u4PTS);
            return TRUE;
        }
        else if ((u4StartCode == 0x10C || u4StartCode ==0x10D))
        {
            if (rPesInfo.u4PTS ==0)
                rPesInfo.u4PTS = _u4PrevFramePTS;
            if (fgTrickMode)
                LOG(0,"frame pts : %x %d\n",rPesInfo.u4PTS,rPesInfo.u4PTS/90000);
        }
        else
        {
            //ASSERT(0);
        }

    }
    else if(_rSwdmxWmvInfo.eVideoType==ENUM_SWDMX_VDEC_WMV7 
		|| _rSwdmxWmvInfo.eVideoType==ENUM_SWDMX_VDEC_WMV8
		|| _rSwdmxWmvInfo.eVideoType==ENUM_SWDMX_VDEC_WMV9)
    {
		if(rPayLoadHeader.u4OffsetIntoMediaObj==0 && rPayLoadHeader.u4MediaObjSize==1)
		{
           rPesInfo.u2SkipVidCount=1;
		}
    }
    else if (_rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_H264)
    {
        u4Addr = prPes->u4FrameAddr + 3;
        if(u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        rPesInfo.ucPicType = *(UINT8*)VIRTUAL(u4Addr);

        u4Addr = prPes->u4FrameAddr + 4;
        if(u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        rPesInfo.u4VldReadPtr = u4Addr;
        if(IS_IDR_NALU(rPesInfo.ucPicType))
        {
            rPesInfo.fgDtsValid = TRUE;
        }
        else
        {
            rPesInfo.fgDtsValid = FALSE;
        }
    }
	
    if (fgTrickMode)
    {
        if (!rPesInfo.fgEos && rPesInfo.u4PTS!=0)
        {
            if ((rPesInfo.u4PTS < (UINT32)_rSwdmxWmvInfo.u4DisplayPTS))
            {
                if (_rSwdmxWmvInfo.i4WMVPlaySpeed >0)
                {
                    LOG(3,"wmv video pts is too small for FF %x %x\n",rPesInfo.u4PTS,_rSwdmxWmvInfo.u4DisplayPTS);
                    return TRUE;
                }
            }
            else
            {
                if (_rSwdmxWmvInfo.i4WMVPlaySpeed <0)
                {
                    LOG(3,"wmv video pts is too big for FR %x %x \n",rPesInfo.u4PTS,_rSwdmxWmvInfo.u4DisplayPTS);
                    return TRUE;
                }
            }
        }

        if ( _fgReceiveVideoPes && (u4StartCode != 0x10C) && (rPesInfo.ucEsId == prSwdmxInst->u1VdecId))
        {
            UINT32 u4PtsDelta = 0;
            INT32 i4PtsDelta = 0;
            UINT32 i;
            
            if(rPesInfo.ucEsId < SWDMX_SOURCE_MAX)
            {
                i4PtsDelta = (INT32)((INT32)rPesInfo.u4PTS - (INT32)_rPreviousPesInfo[rPesInfo.ucEsId].u4PTS);
                u4PtsDelta = (UINT32)abs(i4PtsDelta);
                u4RepeatTimes = (UINT32)(_rSwdmxWmvInfo.u4FrameRate*u4PtsDelta)/(90*(abs(_rSwdmxWmvInfo.i4WMVPlaySpeed)));
                LOG(5,"repeat times : %d %d %d\n",rPesInfo.u4PTS/90000,_rPreviousPesInfo[rPesInfo.ucEsId].u4PTS/90000,u4RepeatTimes);
                _rPreviousPesInfo[rPesInfo.ucEsId].u1SliceCount = TRUE;
            }
            
            if (rPesInfo.fgEos)
                u4RepeatTimes = 5;

            //x_thread_delay(100); // to avoid dmx not finish send data to vfifo

            for (i =0 ; i<u4RepeatTimes ; i++)
            {
                if(rPesInfo.ucEsId < SWDMX_SOURCE_MAX)
                {
                	  if (i == u4RepeatTimes-1)
                	  {
                        _rPreviousPesInfo[rPesInfo.ucEsId].u1SliceCount = FALSE;
                    }
                    VDEC_SendEs((void*)&_rPreviousPesInfo[rPesInfo.ucEsId]);
                }
            }
            if (rPesInfo.fgEos)
                VDEC_SendEs((void*)&rPesInfo);
        }
        else if ( _fgReceiveVideoPes && u4StartCode == 0x10C)
        {
            UINT32 u4idx =0;
            for (u4idx =0 ; u4idx <_rSwdmxWmvInfo.u4RepeatTimes ; u4idx++)
            {
            	  if(rPesInfo.ucEsId < SWDMX_SOURCE_MAX)
            	  {
                    VDEC_SendEs((void*)&_rPreviousPesInfo[rPesInfo.ucEsId]);
                }
                VDEC_SendEs((void*)&rPesInfo);
            }
        }
        else
        {
            if (rPesInfo.fgEos)
            {
                VDEC_SendEs((void*)&rPesInfo);
            }
        }
    }
    else
    {
#ifdef __MODEL_slt__
        if (rPesInfo.u4PTS > _u4VC1SltLastPicPts)
        {
            _u1VC1SltLastPicType = rPesInfo.ucPicType;
            _u4VC1SltLastPicPts = rPesInfo.u4PTS;
        }
#endif
        VDEC_SendEs((void*)&rPesInfo);
    }

    if (fgTrickMode&&(u4RepeatTimes!=0 ||_fgReceiveVideoPes==FALSE))
    {
        if (_rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_VC1)
        {
            if ((u4StartCode == 0x10D || u4StartCode==0x10C))
            {
                if(rPesInfo.ucEsId < SWDMX_SOURCE_MAX)
                {
                    x_memcpy((VOID*)&_rPreviousPesInfo[rPesInfo.ucEsId],(VOID*)&rPesInfo,sizeof(VDEC_PES_INFO_T));
                }
                _fgReceiveVideoPes = TRUE;
            }
            else
            {
                VDEC_SendEs((void*)&rPesInfo);
            }
        }
        else
        {
            if(rPesInfo.ucEsId < SWDMX_SOURCE_MAX)
            {
                x_memcpy((VOID*)&_rPreviousPesInfo,(VOID*)&rPesInfo,sizeof(VDEC_PES_INFO_T));
            }
            _fgReceiveVideoPes = TRUE;
        }
    }
    return TRUE;

}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
/**
*/
BOOL _SWDMX_WMVFeederIsReady(SWDMX_INFO_T *prSwdmxInst)
{


    if(MPV_IsDisplay(prSwdmxInst->u1VdecId))
    {
        return TRUE; //wait sequence header
    }

    if ( (_prRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK_NETFLIX)
            || (_prRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK_DLNA))
    {
#ifdef ENABLE_FEEDER_THRESHOLD
        //with feeder threshold
#else
        //without feeder threshold
#if 1
        if (_fgNetflixEos && _prRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            return TRUE;
        }
#endif
        if (!_SWDMX_WMVIsFeederBufferFull(prSwdmxInst))
        {
            UINT32 u4FeederSize;
            FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &_rFeederInfo);
            u4FeederSize = _rFeederInfo.u4BytesInBuf;
            if (_prRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK_NETFLIX)
            {
                if (u4FeederSize < WMV_NETFLIX_PRE_BUFFER_SIZE && _u4RequestDataSize !=0/*!_rSwdmxWmvInfo.fgReqFeederDataEOS*/)
                {
                    return FALSE;
                }
            }
            else
            {
                if (u4FeederSize < WMV_PRE_BUFFER_SIZE && _u4RequestDataSize !=0/*!_rSwdmxWmvInfo.fgReqFeederDataEOS*/)
                {
                    return FALSE;
                }
            }
            return TRUE;
        }
        else
        {
            return TRUE;
        }
#endif
    }
    else
    {
        return TRUE;
    }
}


BOOL _SWDMX_WMVIsFeederBufferFull(SWDMX_INFO_T *prSwdmxInst)
{
    UINT32 u4BlockSize;
    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &_rFeederInfo) != FEEDER_E_OK)
    {
        LOG(7, "Feeder input buffer error.\n");
        return FALSE;
    }
    u4BlockSize=FeederGetAlignSize(prSwdmxInst->eFeederSrc);

    if (_rFeederInfo.u4FreeSize >=(_u4DefalutReqSize+u4BlockSize+u4BlockSize))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BOOL _SWDMX_WMVGetFeederData(SWDMX_INFO_T *prSwdmxInst, BOOL fgNoWait)
{
    BOOL fgRet = FALSE;
    INT32 i4Ret;
    WMV_FEEDER_REQ_DATA_T rWmvFeederReqData;

    //if(_rSwdmxWmvInfo.fgReqFeederDataEOS)
    if ( _u4RequestDataSize == 0)
    {
        return FALSE;
    }

    x_memset(&_rFeederReqData,0,sizeof(FEEDER_REQ_DATA_T));

    if (!fgNoWait)
    {
        do
        {
            if(_rSwdmxWmvInfo.fgPlayAbort)
            {
                _rSwdmxWmvInfo.fgFeederInvalid = TRUE;
                LOG(3,"Play abort\n");
                return FALSE;
            }
            
            fgRet = _SWDMX_ReceiveFeederAck(prSwdmxInst->u1SwdmxId, (VOID*)&_rFeederReqData);
            if (_rFeederReqData.eDataType == FEEDER_SOURCE_INVALID)
            {
                _rSwdmxWmvInfo.fgFeederInvalid = TRUE;
                LOG(3,"feeder invalid 1\n");
                return FALSE;
            }
            
#if 1  // megaa 20100422: tmp solution for 1st playback error of WMA on Linux Turnkey
            if (_rFeederReqData.u4WriteAddr == 0)  // it is error case!!!
            {
                _SWDMX_WMVRequestFeederData(prSwdmxInst, _u4RequestDataSize, _rSwdmxWmvInfo.u8LastFileParsingPosition, FALSE);
                _rFeederReqData.u4Id = 0xFFFF1234;
            }
#endif
        } while (_rFeederReqData.u4Id+1 != _rSwdmxWmvInfo.u4FeederReqID);

        if (!fgRet)
        {
            LOG(3,"feeder timeout1\n");
            
            LOG(2,"Send Eos 2\n");
            _SWDMX_WMVSendEOS(prSwdmxInst);
            return FALSE;
        }
    }
    else
    {
        do
        {
            if(_rSwdmxWmvInfo.fgPlayAbort)
            {
                _rSwdmxWmvInfo.fgFeederInvalid = TRUE;
                LOG(3,"Play abort\n");
                return FALSE;
            }
            i4Ret = _SWDMX_ReceiveFeederAckNoWait(prSwdmxInst->u1SwdmxId, (VOID*)&_rFeederReqData);
            if (i4Ret == OSR_NO_MSG)
            {
                return FALSE;
            }
            else if (i4Ret != OSR_OK)
            {
                LOG(2,"Send Eos 3\n");
                _SWDMX_WMVSendEOS(prSwdmxInst);
                return FALSE;
            }
            else
            {
                if (_rFeederReqData.eDataType == FEEDER_SOURCE_INVALID)
                {
                    _rSwdmxWmvInfo.fgFeederInvalid = TRUE;
                    LOG(3,"feeder invalid 1\n");
                    return FALSE;
                }
            }
        } while (_rFeederReqData.u4Id+1 != _rSwdmxWmvInfo.u4FeederReqID);
    }

    LOG(6,"feeder ack %d\n",_rFeederReqData.u4Id);

    x_memcpy((VOID*)&(rWmvFeederReqData.rFeederReqData),(VOID*)&_rFeederReqData,sizeof(FEEDER_REQ_DATA_T));
    rWmvFeederReqData.u4RequestFeederSize = _u4RequestDataSize;

    i4Ret = x_msg_q_send(_hWMVFeederDataQue, &rWmvFeederReqData, sizeof(WMV_FEEDER_REQ_DATA_T), 255);
    if (_rSwdmxWmvInfo.fgFirstFrame)
    {
        if (_rSwdmxWmvInfo.u8BeginParseOfst == 0)
        {
            _rSwdmxWmvInfo.u8LastFileParsingPosition = _prRangeInfo->ui8_fileoffset + WMV_DATA_OBJ_HEADER+
                    ((_u4RequestDataSize-WMV_DATA_OBJ_HEADER)/_prRangeInfo->ui4_sz_min_acces_unt)*_prRangeInfo->ui4_sz_min_acces_unt;
        }
        else
        {
            _rSwdmxWmvInfo.u8LastFileParsingPosition = _rSwdmxWmvInfo.u8BeginParseOfst + 
                    (_u4RequestDataSize/_prRangeInfo->ui4_sz_min_acces_unt)*_prRangeInfo->ui4_sz_min_acces_unt;
        }
    }
    else
    {
        _rSwdmxWmvInfo.u8LastFileParsingPosition += (_u4RequestDataSize/_prRangeInfo->ui4_sz_min_acces_unt)*_prRangeInfo->ui4_sz_min_acces_unt;
    }

    return TRUE;
}

BOOL _SWDMX_WMVRequestFeederData(SWDMX_INFO_T *prSwdmxInst, UINT32 u4QrySize,UINT64 u8Offset,BOOL fgTrickMode)
{
    //HAL_TIME_T rStartTime,rEndTime;
    UINT32 u4RemainedDataSize,u4RequestDataSize;
    UINT32 u4DefalutReqSize,u4DefaultSize=0;
    //UINT32 u4QrySize;
    //UINT64 u8Offset;
    x_memset(&_rFeederCond,0,sizeof(FEEDER_TRANSMIT_COND));

    if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
    {
       u4DefaultSize=WMA_REQUEST_SIZE;
    }
	else
	{
       u4DefaultSize=FEEDER_SEGMENT;
	}
	
    if (_rSwdmxWmvInfo.fgFirstFrame)
    {
        if (_rSwdmxWmvInfo.u8BeginParseOfst == 0)
            u4RemainedDataSize = (UINT32)_rSwdmxWmvInfo.u8FileSize;
        else
            u4RemainedDataSize = (UINT32)_rSwdmxWmvInfo.u8FileSize + _prRangeInfo->ui8_fileoffset - _rSwdmxWmvInfo.u8BeginParseOfst;
        if (_prRangeInfo->e_mm_src_type != MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            u4DefalutReqSize = (u4DefaultSize/_prRangeInfo->ui4_sz_min_acces_unt)*_prRangeInfo->ui4_sz_min_acces_unt;
            if (_rSwdmxWmvInfo.u8BeginParseOfst == 0)
                u4DefalutReqSize += WMV_DATA_OBJ_HEADER;
        }
        else
        {
            u4DefalutReqSize = (282*1024);//min(282*1024, FEEDER_DEFAULT_BUF_SIZE >> 3);
        }
    }
    else
    {
        u4RemainedDataSize = (UINT32)_rSwdmxWmvInfo.u8FileSize -
                             ((_rSwdmxWmvInfo.u8LastFileParsingPosition)-(_prRangeList->prDmxRangeInfo->ui8_fileoffset));
        if (_prRangeInfo->e_mm_src_type != MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            u4DefalutReqSize = (u4DefaultSize/_prRangeInfo->ui4_sz_min_acces_unt)*_prRangeInfo->ui4_sz_min_acces_unt;
        }
        else
        {
            u4DefalutReqSize = (282*1024);//min(282*1024, FEEDER_DEFAULT_BUF_SIZE >> 3);
        }
    }

    if (_prRangeInfo->e_mm_src_type != MM_SRC_TYPE_NETWORK_NETFLIX)
    {
        u4RequestDataSize = min(u4DefalutReqSize,u4RemainedDataSize);
    }
    else
    {
        u4RequestDataSize = u4DefalutReqSize;
    }
    
    if (!fgTrickMode)
    {
        u4QrySize = u4RequestDataSize;
        u8Offset = _rSwdmxWmvInfo.u8LastFileParsingPosition;
    }

    _u4RequestDataSize = u4QrySize;

    if (u4QrySize != 0)
    {
        INT32 i4Ret;
        LOG(6,"feeder request id: %d\n",_rSwdmxWmvInfo.u4FeederReqID);
        i4Ret = FeederSetRequest(prSwdmxInst->eFeederSrc,u4QrySize,u8Offset,&_rFeederCond,_rSwdmxWmvInfo.u4FeederReqID++);

        //HAL_GetTime(&rStartTime);
        //_SWDMX_WMVGetFeederData(prSwdmxInst);
        //HAL_GetTime(&rEndTime);
        //HAL_GetDeltaTime(&_rFeederReqTime, &rStartTime, &rEndTime);
        //_rSwdmxWmvInfo.fgTureReqFeederData = TRUE;
        if (i4Ret != FEEDER_E_OK)
        {
            _u4RequestDataSize = 0;
        }
        return TRUE;
    }
    else
    {
        _rSwdmxWmvInfo.fgReqFeederDataEOS = TRUE;
        FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_EOS,TRUE);
        return FALSE;
    }
}

BOOL _SWDMX_WMVSetStrmID(UINT8 u1SrcId, UINT8 u1HdrType,UINT32 u4StrmID, UINT32 u4StrmSubID)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_WMV_VERIFY_NULL_ERROR(prSwdmxInst);        

    switch (u1HdrType)
    {
    case eSWDMX_STRM_TYPE_VID:
#if defined(CC_3D_MM_DS_SUPPORT)  // hard code to inverse L/R
        if (ENABLE_NEWTRACK(prSwdmxInst))
        {
            u4StrmID = 2;
        }
#endif
        _rSwdmxWmvInfo.u4VStreamId= u4StrmID;
#ifdef CC_NETFLIX_CMPB_SUPPORT
        _u4NetflixVideoStreamId = u4StrmID;
#endif
        break;
    case eSWDMX_STRM_TYPE_VID2:
#if defined(CC_3D_MM_DS_SUPPORT)  // hard code to inverse L/R
        if (ENABLE_NEWTRACK(prSwdmxInst))
        {
            u4StrmID = 3;
        }
#endif
        _rSwdmxWmvInfo.u4VSubStreamId=u4StrmID;
        break;
    case eSWDMX_STRM_TYPE_AUD:
        _u1AudioStreamId = (UINT8)_rSwdmxWmvInfo.u4AStreamId;
        _rSwdmxWmvInfo.u4AStreamId = u4StrmID;
#ifdef CC_NETFLIX_CMPB_SUPPORT
        _u4NetflixAudioStreamId = u4StrmID;
#endif
#ifdef WMV_SUPPORT_AUDIO_SWITCH
        if ((_u1AudioStreamId != (UINT8)_rSwdmxWmvInfo.u4AStreamId) &&_u1AudioStreamId !=0xff)
        {
            _rSwdmxWmvInfo.fgChangeAudio = TRUE;
            LOG(0,"WMV file change audio channel\n");
        }
        else
        {
            _rSwdmxWmvInfo.u4WmaErrorCode = 0; //DTV215322
            _rSwdmxWmvInfo.fgChangeAudio = FALSE;
        }
#endif
        break;
    case eSWDMX_STRM_TYPE_SUBTITLE:
        _rSwdmxWmvInfo.u4SpStreamId = u4StrmID;
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

BOOL _SWDMX_WMVEnableStrm(UINT8 u1SrcId, UINT8 u1StrmType, VOID* pfnCbFunc)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_WMV_VERIFY_NULL_ERROR(prSwdmxInst);        

    switch (u1StrmType)
    {
    case eSWDMX_STRM_TYPE_VID:
        //prSwdmxInst->u1SwdmxId = u1SrcId;//maiyou  megaa: no need
        _rSwdmxWmvInfo.fgEnVideo = TRUE;
        if (!_rSwdmxWmvInfo.fgEnVDmx)
        {
            _SWDMX_WMVInitDmx(prSwdmxInst, eSWDMX_MEDIA_VIDEO,prSwdmxInst->u1VdecId,TRUE,FALSE);
        }
        _rSwdmxWmvInfo.fgEnVDmx = TRUE;

        break;
    
    case eSWDMX_STRM_TYPE_VID2:
        //prSwdmxInst->u1SwdmxId = u1SrcId;//maiyou  megaa: no need
        _rSwdmxWmvInfo.fgEnVideo = TRUE;
        if (!_rSwdmxWmvInfo.fgEnV2Dmx)
        {
            _SWDMX_WMVInitDmx(prSwdmxInst, eSWDMX_MEDIA_VIDEO,prSwdmxInst->u1Vdec2Id,TRUE,FALSE);
        }
        _rSwdmxWmvInfo.fgEnV2Dmx = TRUE;
        break;
    case eSWDMX_STRM_TYPE_AUD:
    case eSWDMX_STRM_TYPE_AUD2:
        _rSwdmxWmvInfo.fgEnAudio = TRUE;
        if (!_rSwdmxWmvInfo.fgEnADmx)
        {
            _SWDMX_WMVInitDmx(prSwdmxInst, eSWDMX_MEDIA_AUDIO,prSwdmxInst->u1SwdmxId,TRUE,FALSE);
        }
        _rSwdmxWmvInfo.fgEnADmx = TRUE;

#if 0  //def WMV_2X_TO_1X_PREBUFFER
        _rSwdmxWmvInfo.fg2XAudParsing = FALSE;
#endif
        break;
    case eSWDMX_STRM_TYPE_SUBTITLE:
        _rSwdmxWmvInfo.fgEnSubTitle = TRUE;
        break;
    default:
        return FALSE;
    }
    return TRUE;
}


BOOL _SWDMX_WMVDisableStrm(UINT8 u1SrcId, UINT8 u1StrmType)
{
    DMX_MM_T rPid;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_WMV_VERIFY_NULL_ERROR(prSwdmxInst);        

    rPid.fgEnable = FALSE;

    switch (u1StrmType)
    {
    case eSWDMX_STRM_TYPE_VID:
        _rSwdmxWmvInfo.fgEnVideo = FALSE;
        if (_rSwdmxWmvInfo.fgEnVDmx)
        {
            VERIFY(DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1VidPid, DMX_MM_FLAG_VALID, &rPid));
            VERIFY(DMX_MM_Free(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1VidPid));
            VERIFY(DMX_MUL_FreePidx(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1VidPid));
        }
        _rSwdmxWmvInfo.fgEnVDmx = FALSE;
        break;
    case eSWDMX_STRM_TYPE_VID2:
        _rSwdmxWmvInfo.fgEnVideo = FALSE;
        if (_rSwdmxWmvInfo.fgEnV2Dmx)
        {
            VERIFY(DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1Vid2Pid, DMX_MM_FLAG_VALID, &rPid));
            VERIFY(DMX_MM_Free(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1Vid2Pid));
            VERIFY(DMX_MUL_FreePidx(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1Vid2Pid));
            _rSwdmxWmvInfo.fgEnV2Dmx = FALSE;
        }
        break;
    case eSWDMX_STRM_TYPE_AUD:
    case eSWDMX_STRM_TYPE_AUD2:
        _rSwdmxWmvInfo.fgEnAudio = FALSE;
        if (_rSwdmxWmvInfo.fgVideoPause&&_rSwdmxWmvInfo.fgEnVideo)
        {
            _WMVStartToPlay(prSwdmxInst);
        }
        if (_rSwdmxWmvInfo.fgEnADmx)
        {
//#ifndef WMV_2X_TO_1X_PREBUFFER
            VERIFY(DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid, DMX_MM_FLAG_VALID, &rPid));
            VERIFY(DMX_MM_Free(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid));
            VERIFY(DMX_MUL_FreePidx(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid));
//#endif
            _rSwdmxWmvInfo.fgEnADmx = FALSE;
        }
#ifdef WMV_2X_TO_1X_PREBUFFER
        //_rSwdmxWmvInfo.fg2XAudParsing = TRUE;
#endif

        break;
    case eSWDMX_STRM_TYPE_SUBTITLE:
        _rSwdmxWmvInfo.fgEnSubTitle = FALSE;
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

BOOL _SWDMX_WMVInitDmx(SWDMX_INFO_T* prSwdmxInst, UINT8 u1HdrType,UINT8 u1Index,BOOL fgEnable,BOOL fgEos)
{
    UINT32 u4Flag;
    DMX_MM_T rDmxMMInfo;
    UINT8 u1VidPid=0;
    BOOL fgDmxEnabled=FALSE;
    x_memset(&rDmxMMInfo, 0, sizeof(DMX_MM_T));
    
    if (u1HdrType == eSWDMX_MEDIA_VIDEO && _rSwdmxWmvInfo.fgEnVideo)
    {
        rDmxMMInfo.fgEnable = fgEnable;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;
        if(IS_NEWTRACK(prSwdmxInst,u1Index))
        {
           fgDmxEnabled=_rSwdmxWmvInfo.fgEnV2Dmx;
        }
        else
        {
           fgDmxEnabled=_rSwdmxWmvInfo.fgEnVDmx;
        }
#if 1  // for multi-instance
 #if 1  // v2
        DMX_SetToDecoder(TRUE);
 #else
        {
            DMX_DECODER_CALLBACKS_T rDecoderCallbacks;
            rDecoderCallbacks.pfnSendVideoPes = _WMVVideoCallback;//_VideoCallback;
            rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;
            //rDecoderCallbacks.pfnSendAudioPes = AUD_SendAudioPes;
            DMX_SetDecoderCallbacksSeparate(DMX_CB_FLAG_VIDEO, (const DMX_DECODER_CALLBACKS_T*)&rDecoderCallbacks);
            DMX_SetToDecoder(TRUE);
        }
 #endif
#endif
        if (!fgDmxEnabled)
        {
           // prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1VidPid = DMX_MUL_GetAvailablePidx(u1Index);
           
            u1VidPid=DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);
#ifndef WMV_ALLOCATE_VFIFO
            if (!ENABLE_NEWTRACK(prSwdmxInst))
            {
                _SWDMX_GetVideoFifo(prSwdmxInst->u1SwdmxId, &rDmxMMInfo.u4BufAddr, &rDmxMMInfo.u4BufSize);
            }
#else

#if 1
            _u4VFifoStartAddr = (UINT32)BSP_AllocAlignedDmaMemory(_u4WmvFifoSize,WMV_DMX_FIFO_ALIGNMENT);
#else /* need mentor's double check, follow David's mail: "Please review swdmx internal buffer use" */
			{
				UINT32 u4Addr = 0;
				UINT32 u4Size = _u4WmvFifoSize;
				UINT32 u4Align = WMV_DMX_FIFO_ALIGNMENT;
				SWDMX_GetInfo(u1SrcId, eSWDMX_GET_INTERNAL_BUF, &u4Addr, &u4Size, &u4Align);
				_u4VFifoStartAddr = (UINT32)u4Addr;
			}
#endif

            if (_u4VFifoStartAddr != NULL)
            {
                rDmxMMInfo.u4BufAddr = _u4VFifoStartAddr;
                rDmxMMInfo.u4BufSize = _u4WmvFifoSize;
            }
            else
            {
                ASSERT(0);
                _SWDMX_GetVideoFifo(prSwdmxInst->u1SwdmxId, &rDmxMMInfo.u4BufAddr, &rDmxMMInfo.u4BufSize);
            }
#endif
            if(ENABLE_NEWTRACK(prSwdmxInst))
            {
                if(IS_NEWTRACK(prSwdmxInst,u1Index))
                {
                    _SWDMX_GetVideo2Fifo(prSwdmxInst->u1SwdmxId, &rDmxMMInfo.u4BufAddr, &rDmxMMInfo.u4BufSize);
                   _rSwdmxWmvInfo.u1Vid2Pid=u1VidPid;
                   rDmxMMInfo.u1ChannelId = prSwdmxInst->u1Vdec2Id;
                    //rDmxMMInfo.u4BufSize=rDmxMMInfo.u4BufSize>>1;
                    //rDmxMMInfo.u4BufAddr+=rDmxMMInfo.u4BufSize;
                }
                else
                {
                    _SWDMX_GetVideoFifo(prSwdmxInst->u1SwdmxId, &rDmxMMInfo.u4BufAddr, &rDmxMMInfo.u4BufSize);
                   _rSwdmxWmvInfo.u1VidPid=u1VidPid;
                   rDmxMMInfo.u1ChannelId = prSwdmxInst->u1VdecId;
                    //rDmxMMInfo.u4BufSize=rDmxMMInfo.u4BufSize>>1;
                }
            }
            else
            {
                _rSwdmxWmvInfo.u1VidPid=u1VidPid;
                rDmxMMInfo.u1ChannelId = prSwdmxInst->u1VdecId;
            }
            
            u4Flag = (UINT32)(DMX_MM_FLAG_TYPE | DMX_MM_FLAG_INSTANCE_TAG |
                              DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF |
                              DMX_MM_FLAG_SEARCH_START_CODE |
                              DMX_MM_FLAG_DEVICE_ID);

            rDmxMMInfo.pvInstanceTag = (void*)prSwdmxInst;
            rDmxMMInfo.fgAllocBuf = FALSE;
            if(_rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_MJPEG)
            {
                rDmxMMInfo.u1DeviceId = VLD1; // video format all use VLD expect jpeg
            }
            else
            {        
                rDmxMMInfo.u1DeviceId = VLD0; // video format all use VLD expect jpeg
            }

            switch (_rSwdmxWmvInfo.eVideoType)
            {
            case ENUM_SWDMX_VDEC_WMV7:
            case ENUM_SWDMX_VDEC_WMV8:
            case ENUM_SWDMX_VDEC_WMV9:
            case ENUM_SWDMX_VDEC_DX311:
                LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = FALSE\n");
                rDmxMMInfo.fgSearchStartCode = FALSE;
                break;
            case ENUM_SWDMX_VDEC_MPEG4:
            case ENUM_SWDMX_VDEC_VC1:
            case ENUM_SWDMX_VDEC_H264:
                LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = TRUE\n");
                rDmxMMInfo.fgSearchStartCode = TRUE;
                break;
            default:
                //ASSERT(0); //unknow wmv type
                //LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = TRUE\n");
                //rDmxMMInfo.fgSearchStartCode = TRUE;
                break;
            }
            _u4VFifoStartAddr = rDmxMMInfo.u4BufAddr;
            _u4VFifoSize = rDmxMMInfo.u4BufSize;
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
            _u4CommVFifoSize = rDmxMMInfo.u4BufSize;
#endif
            DMX_MM_Set(u1VidPid, u4Flag, &rDmxMMInfo);
#ifdef ENABLE_WMV_DRM
            if (_rSwdmxWmvInfo.fgIsDRM)
            {
                DMX_MM_DRM_AES_T rMMDrmAes;
                UINT32 u4Flag =0;

                x_memset((VOID*)&rMMDrmAes,0,sizeof(DMX_MM_DRM_AES_T));
                if (INPUT_SRC == MM_SRC_TYPE_NETWORK_NETFLIX)
                {
                    u4Flag = (UINT32)(DMX_MM_DRM_FLAG_KEY);
                    x_memset((VOID*)&rMMDrmAes.au1Key[0],0,sizeof(rMMDrmAes.au1Key));
                    x_memcpy((VOID*)&rMMDrmAes.au1Key[0],(VOID*)&_au1ContentKey[0],sizeof(_au1ContentKey));
                    DMX_MM_SetDRMMode(u1VidPid,DMX_DRM_MODE_PD);  // megaa 20100314 ToDo: check if DMX has new DRM APIs
                    DMX_MM_SetNDDRM(u1VidPid,u4Flag,&rMMDrmAes);  // megaa 20100314 ToDo: check if DMX has new DRM APIs
                }
                else if (INPUT_SRC == MM_SRC_TYPE_NETWORK_DLNA)
                {
                    u4Flag = (UINT32)(DMX_MM_DRM_FLAG_KEY | DMX_MM_DRM_FLAG_SAMPLE_ID);
                    x_memset((VOID*)&rMMDrmAes.au1Key[0],0,sizeof(rMMDrmAes.au1Key));
                    x_memcpy((VOID*)&rMMDrmAes.au1Key[0],(VOID*)&_au1ContentKey[0],sizeof(_au1ContentKey));
                    x_memset((VOID*)&rMMDrmAes.au1SampleID[0],0,sizeof(rMMDrmAes.au1SampleID));

                    DMX_MM_SetDRMMode(u1VidPid,DMX_DRM_MODE_ND);  // megaa 20100314 ToDo: check if DMX has new DRM APIs
                    DMX_MM_SetNDDRM(u1VidPid,u4Flag,&rMMDrmAes);  // megaa 20100314 ToDo: check if DMX has new DRM APIs
                }

            }
#endif
        }
        else
        {
        
            if(IS_NEWTRACK(prSwdmxInst,u1Index))
            {
                u1VidPid=_rSwdmxWmvInfo.u1Vid2Pid;
            }
            else
            {
               u1VidPid=_rSwdmxWmvInfo.u1VidPid;
            }
            
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            if (fgEos)
            {
                rDmxMMInfo.fgSearchStartCode = FALSE;
            }
            else
            {
                switch (_rSwdmxWmvInfo.eVideoType)
                {
                case ENUM_SWDMX_VDEC_WMV7:
                case ENUM_SWDMX_VDEC_WMV8:
                case ENUM_SWDMX_VDEC_WMV9:
                case ENUM_SWDMX_VDEC_DX311:
                    LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = FALSE\n");
                    rDmxMMInfo.fgSearchStartCode = FALSE;
                    break;
                case ENUM_SWDMX_VDEC_MPEG4:
                case ENUM_SWDMX_VDEC_VC1:
                case ENUM_SWDMX_VDEC_H264:
                    LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = TRUE\n");
                    rDmxMMInfo.fgSearchStartCode = TRUE;
                    break;
                default:
                    //ASSERT(0); //unknow wmv type
                    //LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = TRUE\n");
                    //rDmxMMInfo.fgSearchStartCode = TRUE;
                    break;
                }
            }
            DMX_MM_Set(u1VidPid, u4Flag, &rDmxMMInfo);
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_AUDIO && _rSwdmxWmvInfo.fgEnAudio)
    {
        UINT32 au4BufStart[2], au4BufEnd[2];

#if 1  // for multi-instance
 #if 1  // v2
        DMX_SetToDecoder(TRUE);
 #else
        {
            DMX_DECODER_CALLBACKS_T rDecoderCallbacks;
            //rDecoderCallbacks.pfnSendVideoPes = _WMVVideoCallback;//_VideoCallback;
            //rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;
            rDecoderCallbacks.pfnSendAudioPes = AUD_SendAudioPes;
            DMX_SetDecoderCallbacksSeparate(DMX_CB_FLAG_AUDIO, (const DMX_DECODER_CALLBACKS_T*)&rDecoderCallbacks);
            DMX_SetToDecoder(TRUE);
        }
 #endif
#endif

#if 0  //def WMV_2X_TO_1X_PREBUFFER
        if (_rSwdmxWmvInfo.fgAlreadySet)
            return TRUE;
        else
            _rSwdmxWmvInfo.fgAlreadySet = TRUE;
#endif

#if 1  // v2
        prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid = DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);
        _SWDMX_GetAudioFifo(prSwdmxInst->u1SwdmxId, &rDmxMMInfo.u4BufAddr, &rDmxMMInfo.u4BufSize);

        u4Flag = (UINT32)(DMX_MM_FLAG_TYPE | DMX_MM_FLAG_VALID | DMX_MM_FLAG_INSTANCE_TAG
                          | DMX_MM_FLAG_BUF | DMX_MM_FLAG_SEARCH_START_CODE |
                          DMX_MM_FLAG_DEVICE_ID);
#else
        u4Flag = (UINT32)(DMX_MM_FLAG_TYPE | DMX_MM_FLAG_VALID | DMX_MM_FLAG_NOTIFY
                          | DMX_MM_FLAG_BUF | DMX_MM_FLAG_SEARCH_START_CODE);
#endif

        rDmxMMInfo.fgEnable = fgEnable;
#ifdef CC_DMX_PURE_AUDIO   // multi-instance
        if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
        {
            rDmxMMInfo.ePidType = DMX_PID_TYPE_PURE_AUDIO;
        }
        else
#endif
        {
            rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_AUDIO;
        }

        rDmxMMInfo.fgSearchStartCode = FALSE;

#ifdef AUD_OLDFIFO_INTF
            if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
                 &au4BufEnd[1]) != AUD_OK)
#else
            if (AUD_GetAudFifo(prSwdmxInst->u1AdecId, &au4BufStart[0], &au4BufEnd[0]) != AUD_OK)
#endif
        {
            LOG(1, "_SWDMX_SetAudioStreamID Can't get audio buffers!\n");
            return FALSE;
        }

        rDmxMMInfo.pvInstanceTag = (void*)prSwdmxInst;
        rDmxMMInfo.fgAllocBuf = FALSE;
        rDmxMMInfo.u1ChannelId = prSwdmxInst->u1AdecId;
        rDmxMMInfo.u1DeviceId = prSwdmxInst->u1AdecId;

        _u4AFifoStartAddr = rDmxMMInfo.u4BufAddr;
        _u4AFifoSize = rDmxMMInfo.u4BufSize;
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
        _u4CommAFifoSize = rDmxMMInfo.u4BufSize;
        _u1CommAudDmxId = prSwdmxInst->u1DmxId;
        _u1CommAudPid = _rSwdmxWmvInfo.u1AudPid;
        if (prSwdmxInst->fgIsBadInterleaved)
        {
            if (_hCommBIQ == 0)
            {
                // create BI message Q
                VERIFY(x_msg_q_create(&_hCommBIQ, "WMVBIQ", sizeof(UINT32), 1) == OSR_OK);
            }
            else
            {
                // clear BI message Q
                vWmvBIGetMsg(FALSE);
            }
        }
#endif
#ifdef WMV_2X_TO_1X_PREBUFFER
        DMX_AudHandler_SetEnable(TRUE);
#endif
        DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid, u4Flag, &rDmxMMInfo);

#ifdef WMV_2X_TO_1X_PREBUFFER
        int i;
        UINT32 u4AudPreFifoAddr = 0, u4AudPreFifoSize = 0, u4AudTrackSize = 0;

        if (_rSwdmxWmvInfo.fgAlreadySet)
            return TRUE;
        else
            _rSwdmxWmvInfo.fgAlreadySet = TRUE;

        if (!DMX_AudHandler_Set(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid, TRUE))
        {
            LOG(1, "%s:%d, DMX_AudHandler_Set fail\n", __FUNCTION__, __LINE__);
        }

        if (_SWDMX_GetAudioPreFifo(prSwdmxInst->u1SwdmxId, &u4AudPreFifoAddr, &u4AudPreFifoSize))
        {
            u4AudTrackSize = u4AudPreFifoSize / WMV_MAX_AUDIO_TRACK;
        }
        else
        {
            LOG(1, "%s:%d, _SWDMX_GetAudioPreFifo fail\n", __FUNCTION__, __LINE__);
        }

        for (i = 0; i < WMV_MAX_AUDIO_TRACK; i++)  // !!!
        {
            rDmxMMInfo.fgAllocBuf = FALSE;
            rDmxMMInfo.u4BufSize = u4AudTrackSize;
            rDmxMMInfo.u4BufAddr = u4AudPreFifoAddr;
            _rSwdmxWmvInfo.u1AudPreBufPid[i] = DMX_MUL_GetAvailablePidx(prSwdmxInst->u1SwdmxId);
            if (_rSwdmxWmvInfo.u1AudPreBufPid[i] == 0xFF)
            {
                LOG(3, "get  audio pre  pid fail\n");
                ASSERT(0);
            }
            VERIFY(DMX_MM_Set(_rSwdmxWmvInfo.u1AudPreBufPid[i], u4Flag, &rDmxMMInfo));
            if (!DMX_AudHandler_Set(_rSwdmxWmvInfo.u1AudPreBufPid[i], FALSE))
            {
                LOG(1, "%s:%d, DMX_AudHandler_Set fail\n", __FUNCTION__, __LINE__);
            }
            u4AudPreFifoAddr += u4AudTrackSize;
        }
        VERIFY(DMX_AudHandler_SetActivePidx(_rSwdmxWmvInfo.u1AudPreBufPid[0]));
#else
#ifdef CC_DLNA_SUPPORT
        DMX_AudHandler_SetEnable(FALSE);
#endif
#endif

#ifdef ENABLE_WMV_DRM
        if (_rSwdmxWmvInfo.fgIsDRM)
        {
            DMX_MM_DRM_AES_T rMMDrmAes;
            UINT32 u4Flag =0;

            if (INPUT_SRC == MM_SRC_TYPE_NETWORK_NETFLIX)
            {
                x_memset((VOID*)&rMMDrmAes,0,sizeof(DMX_MM_DRM_AES_T));
                u4Flag = (UINT32)(DMX_MM_DRM_FLAG_KEY);
                x_memset((VOID*)&rMMDrmAes.au1Key[0],0,sizeof(rMMDrmAes.au1Key));
                x_memcpy((VOID*)&rMMDrmAes.au1Key[0],(VOID*)&_au1ContentKey[0],sizeof(_au1ContentKey));

                DMX_MM_SetDRMMode(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid,DMX_DRM_MODE_PD);  // megaa 20100314 ToDo: check if DMX has new DRM APIs
                DMX_MM_SetNDDRM(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid,u4Flag,&rMMDrmAes);  // megaa 20100314 ToDo: check if DMX has new DRM APIs
            }
            else if (INPUT_SRC == MM_SRC_TYPE_NETWORK_DLNA)
            {
                x_memset((VOID*)&rMMDrmAes,0,sizeof(DMX_MM_DRM_AES_T));
                u4Flag = (UINT32)(DMX_MM_DRM_FLAG_KEY | DMX_MM_DRM_FLAG_SAMPLE_ID);
                x_memset((VOID*)&rMMDrmAes.au1Key[0],0,sizeof(rMMDrmAes.au1Key));
                x_memcpy((VOID*)&rMMDrmAes.au1Key[0],(VOID*)&_au1ContentKey[0],sizeof(_au1ContentKey));
                x_memset((VOID*)&rMMDrmAes.au1SampleID[0],0,sizeof(rMMDrmAes.au1SampleID));

                DMX_MM_SetDRMMode(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid,DMX_DRM_MODE_ND);  // megaa 20100314 ToDo: check if DMX has new DRM APIs
                DMX_MM_SetNDDRM(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid,u4Flag,&rMMDrmAes);  // megaa 20100314 ToDo: check if DMX has new DRM APIs
            }

        }
#endif
    }
    return TRUE;
}

VOID _SWDMX_WMVComsumeFeeder(SWDMX_INFO_T *prSwdmxInst)
{
    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &_rFeederInfo) !=
            FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        //return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, _rFeederInfo.u4WriteAddr);
    _rSwdmxWmvInfo.u4FeederReqID++;
}

#ifdef WMA_SEEK_OFFSET
INT32 _SWDMX_WMASeekOffset(SWDMX_INFO_T *prSwdmxInst)
{
   MM_RANGE_INFO_T* prRangeInfo=_prRangeInfo;
   SWDMX_WMV_INFO_T *prSwdmxwmvInfo=&_rSwdmxWmvInfo;
   FEEDER_TRANSMIT_COND rFeederReqcond;
   FEEDER_REQ_DATA_T rFeederReqdata;
   UINT64 u8StartSeekPos;
   UINT32 u4CheckPoint=0,u4MarkerPoint=0,u4RequestSize=0,u4CheckTime=0,u4CheckIndex=0;
   UCHAR *puData,*puMarker;
   BOOL fgFindPacket=FALSE;
   //prSwdmxwmvInfo->u4SampleRate=97353;
   
   if(prSwdmxwmvInfo->u4WMASeekTimes==0 && prSwdmxwmvInfo->fgSeekingNow==FALSE)
   {
     u8StartSeekPos=u8Div6464(prSwdmxwmvInfo->u8SeekPTS,90000,NULL)*prSwdmxwmvInfo->u4SampleRate;
     u8StartSeekPos=u8Div6464(u8StartSeekPos,8,NULL);
     u8StartSeekPos+=prRangeInfo->ui8_fileoffset+WMV_DATA_OBJ_HEADER;
   }
   else
   {
     u8StartSeekPos=prSwdmxwmvInfo->u8LastSeekPos;
   }
   u4RequestSize=prSwdmxwmvInfo->u4PacketSize*WMA_SEEK_PACKET_NUM;
   u4RequestSize=MIN(u4RequestSize,prSwdmxwmvInfo->u8FileSize-u8StartSeekPos);
   u4CheckTime=(u4RequestSize/prSwdmxwmvInfo->u4PacketSize);
   if(u4CheckTime<WMA_SEEK_MINCHECK_NUM)
   {
     prSwdmxwmvInfo->u4WMASeekTimes=0;
     prSwdmxwmvInfo->fgSeekingNow=FALSE;
     prSwdmxwmvInfo->u8LastSeekPos=0;
     return -1;
   }
   u4CheckTime--;
   if(!(prSwdmxwmvInfo->u8LastSeekPos && prSwdmxwmvInfo->u4WMASeekTimes==0))
   {
       _SWDMX_FlushReqMsgQ(prSwdmxInst->u1StcId);
       _SWDMX_ReceiveFeederAck(prSwdmxInst->u1StcId,&rFeederReqdata);
       FeederSetRing(prSwdmxInst->eFeederSrc,TRUE);
       FeederSetRequest(prSwdmxInst->eFeederSrc,u4RequestSize,u8StartSeekPos,&rFeederReqcond,prSwdmxwmvInfo->u4FeederReqID++);
   }
   _SWDMX_ReceiveFeederAck(prSwdmxInst->u1StcId,&rFeederReqdata);
   prSwdmxwmvInfo->fgSeekingNow=TRUE;
   prSwdmxwmvInfo->u8LastSeekPos=u8StartSeekPos;
   if(rFeederReqdata.eDataType==FEEDER_SOURCE_INVALID)
   {
     LOG(3,"WMASeek,Invalid data type\n");
     prSwdmxwmvInfo->fgFeederInvalid=TRUE;
     return -1;
   }
   
   if(rFeederReqdata.u4WriteAddr==0)
   {
     LOG(3,"WMASeek,Get data error\n");
     prSwdmxwmvInfo->fgSeekingNow=FALSE;
     prSwdmxwmvInfo->fgFeederInvalid=TRUE;
     return -1;
   }
   
   puData=(UCHAR *)(rFeederReqdata.u4WriteAddr+rFeederReqdata.u4AlignOffset);
   puMarker=(UCHAR *)(&prSwdmxwmvInfo->u8PacketMarker);
   u4RequestSize=rFeederReqdata.u4ReadSize-rFeederReqdata.u4AlignOffset;
   for(u4CheckPoint=0;u4CheckPoint<u4RequestSize-8;u4CheckPoint++)
   {
      u4MarkerPoint=0;
      if(!(u4CheckPoint&(1023)))
      {
         if(_SWDMX_ReceiveFeederAckNoWait(prSwdmxInst->u1StcId,&rFeederReqdata)==OSR_OK)
         {
            if(rFeederReqdata.eDataType==FEEDER_SOURCE_INVALID)
            {
                prSwdmxwmvInfo->fgFeederInvalid=TRUE;
                break;
            }
         }
         
      }
      while(puData[u4CheckPoint+u4MarkerPoint]==puMarker[u4MarkerPoint]&&u4MarkerPoint<8)u4MarkerPoint++;
      
      if(u4MarkerPoint>=prSwdmxwmvInfo->u4PacketMarkerLen)
      {  
         UINT32 u4CheckOffset;
         for(u4CheckIndex=1;u4CheckIndex<u4CheckTime;u4CheckIndex++)
         {
            u4MarkerPoint=0;
             u4CheckOffset=u4CheckPoint+u4CheckIndex*prSwdmxwmvInfo->u4PacketSize;
            while(puData[u4CheckOffset+u4MarkerPoint]==puMarker[u4MarkerPoint]&&u4MarkerPoint<prSwdmxwmvInfo->u4PacketMarkerLen) u4MarkerPoint++;
            if(u4MarkerPoint<prSwdmxwmvInfo->u4PacketMarkerLen)
            {
              break;
            }
         }

         if(u4CheckIndex==u4CheckTime)
         {
             LOG(3,"SWA Seek Find\n");
             prSwdmxwmvInfo->u8LastFileParsingPosition=u8StartSeekPos+u4CheckPoint;
             prSwdmxwmvInfo->u4CurParsingPacketNo = (UINT32)u8Div6464(prSwdmxwmvInfo->u8LastFileParsingPosition-(prRangeInfo->ui8_fileoffset+WMV_DATA_OBJ_HEADER),(UINT64)prSwdmxwmvInfo->u4PacketSize,NULL);
             prSwdmxwmvInfo->fgSeekingNow=FALSE;
             prSwdmxwmvInfo->u4WMASeekTimes=0;
             fgFindPacket=TRUE;
             break;
         }
      }
   }
   
   if(!fgFindPacket)
   {
   
      prSwdmxwmvInfo->u4WMASeekTimes++;
      if(prSwdmxwmvInfo->u4WMASeekTimes>WMA_MAX_SEEKTIME)
      {
          prSwdmxwmvInfo->fgSeekingNow=FALSE;
          prSwdmxwmvInfo->u8LastSeekPos=0; 
          prSwdmxwmvInfo->u4WMASeekTimes=0;
      }
      else
      {
          prSwdmxwmvInfo->fgSeekingNow=TRUE;
          prSwdmxwmvInfo->u8LastSeekPos=u8StartSeekPos+u4CheckPoint;
      }
      return -1;
   }
   else
   {
      return 0;
   }
}
#endif

#ifdef WMV_SEEK_OFFSET
VOID _SWDMX_WMVSeekOffset(SWDMX_INFO_T *prSwdmxInst,BOOL fgCurPos)
{

    _SWDMX_WMVComsumeFeeder(prSwdmxInst);

    if (_rSwdmxWmvInfo.u4PacketSize == 0 && _prRangeInfo !=0)
    {
        _rSwdmxWmvInfo.u4PacketSize = _prRangeInfo->ui4_sz_min_acces_unt;
    }

    if (_rSwdmxWmvInfo.u4PacketSize != 0)
    {
        if(fgCurPos)
        {
			VDP_POS_INTO_T rVdpPosInfo;
			if(VDP_GetPosInfo(prSwdmxInst->u1B2rId,&rVdpPosInfo))
			{
               _rSwdmxWmvInfo.u4CurParsingPacketNo=(UINT32)rVdpPosInfo.u8OffsetI;
			   _rSwdmxWmvInfo.u8SeekPTS=rVdpPosInfo.u8Pts;
			}
        }
		else
		{
			if(_prRangeList->fgSetRenderPosition)
			{
				UINT64 u8Remainder=0;
				_prRangeList->fgSetRenderPosition = FALSE;
				_rSwdmxWmvInfo.u4CurParsingPacketNo = _prRangeList->u8OffsetI;//(UINT32)u8Div6464(_prRangeList->u8OffsetI,(UINT64)_rSwdmxWmvInfo.u4PacketSize,&u8Remainder);
				_rSwdmxWmvInfo.u4TotalPacketNo = (UINT32)u8Div6432((_rSwdmxWmvInfo.u8FileSize-WMV_DATA_OBJ_HEADER),
					   _prRangeInfo->ui4_sz_min_acces_unt, &u8Remainder);
				if (_rSwdmxWmvInfo.u4TotalPacketNo == 0) 
				{
				   _rSwdmxWmvInfo.u4TotalPacketNo = 1;	 // this will cause _SWDMX_WMVSendEOS immediately
				}
				_rSwdmxWmvInfo.u4PacketSize = _prRangeInfo->ui4_sz_min_acces_unt;
				_rSwdmxWmvInfo.fgFirstFrame=FALSE;
			}
			else
			{
				_rSwdmxWmvInfo.u4CurParsingPacketNo = prSwdmxInst->rSeekOffset.ui8_i_frame_position;//(UINT32)u8Div6464(prSwdmxInst->rSeekOffset.ui8_i_frame_position,(UINT64)_rSwdmxWmvInfo.u4PacketSize,&u8Remainder);
			}
		}

        _rSwdmxWmvInfo.u8LastFileParsingPosition = _prRangeInfo->ui8_fileoffset + WMV_DATA_OBJ_HEADER +
                _rSwdmxWmvInfo.u4CurParsingPacketNo*_rSwdmxWmvInfo.u4PacketSize;

    }
    else
    {
        LOG(2,"Send Eos 4\n");
        _SWDMX_WMVSendEOS(prSwdmxInst);
    }
}
#endif

VOID _SWDMX_WMVSeek(SWDMX_INFO_T *prSwdmxInst, UINT64 u8SeekTime, SWDMX_RANGE_LIST_T* prRangeList)
{
    UINT32 i;

    UINT32 u4NumberOfEntry = 0;

    if (_rSwdmxWmvInfo.prWmvKeyIdxTbl != NULL)
    {
        u4NumberOfEntry = _rSwdmxWmvInfo.prWmvKeyIdxTbl->ui4_number_of_entry;
    }
    else
    {
        LOG(3,"_SWDMX_WMVSeek : _rSwdmxWmvInfo.prWmvKeyIdxTbl == NULL\n");
        ASSERT(_rSwdmxWmvInfo.prWmvKeyIdxTbl != NULL);
        return;
    }


    LOG(5, "_SWDMX_WMVSeek u8SeekTime = 0x%08x\n", u8SeekTime);

    for (i = 0; i < u4NumberOfEntry ; i++)
    {
        if (_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts >= (UINT32)u8SeekTime)
        {
            break;
        }
    }

    i= i>=u4NumberOfEntry ? u4NumberOfEntry-1 : i;
    if (i > 0)
    {
        if (_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts == (UINT32)u8SeekTime || _rSwdmxWmvInfo.i4WMVPlaySpeed>0)
        {
            _rSwdmxWmvInfo.u4CurKeyIdx = i;
        }
        else
        {
            _rSwdmxWmvInfo.u4CurKeyIdx = i - 1;
        }

    }
    else
    {
        _rSwdmxWmvInfo.u4CurKeyIdx = 0;
    }

    _SWDMX_WMVComsumeFeeder(prSwdmxInst);

    if (_rSwdmxWmvInfo.u4PacketSize == 0 && _prRangeInfo !=0)
    {
        _rSwdmxWmvInfo.u4PacketSize = _prRangeInfo->ui4_sz_min_acces_unt;
    }

    if (_rSwdmxWmvInfo.u4PacketSize != 0)
    {
        _rSwdmxWmvInfo.u4CurParsingPacketNo = (_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxWmvInfo.u4CurKeyIdx].ui4_relative_offset)/(_rSwdmxWmvInfo.u4PacketSize);
        _rSwdmxWmvInfo.u8LastFileParsingPosition = _prRangeInfo->ui8_fileoffset + WMV_DATA_OBJ_HEADER +
                _rSwdmxWmvInfo.u4CurParsingPacketNo*_rSwdmxWmvInfo.u4PacketSize;
        _rSwdmxWmvInfo.u8SeekPTS=_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts;
		if(_rSwdmxWmvInfo.fgFirstFrame)
		{
		    _rSwdmxWmvInfo.u8BeginParseOfst=_rSwdmxWmvInfo.u8LastFileParsingPosition;
			_rSwdmxWmvInfo.fgVideoNormalPlay=FALSE;
			
		}
    }
    else
    {
    
        LOG(2,"Send Eos 5\n");
        _SWDMX_WMVSendEOS(prSwdmxInst);
    }
    _rSwdmxWmvInfo.fgVideoNormalPlay = FALSE;
    LOG(5, "_SWDMX_WmvSeek Key = %ld\n", _rSwdmxWmvInfo.u4CurKeyIdx);
    
}



BOOL _SWDMX_WMVTrickModeParsing(SWDMX_INFO_T *prSwdmxInst)
{
    //parsing one key frame in trick mode
    UINT32 u4NumberOfEntry;
    UINT64 u8address;
    UINT32 u4RetryTimes =0;
    MIDXBULD_ELMT_STRM_ENTRY_T* prKeyidxEntry = NULL;
    INT32 i4Ret;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    WMV_FEEDER_REQ_DATA_T rWmvFeederReqData;
	BOOL fgReqedData=FALSE;
    if (_rSwdmxWmvInfo.prWmvKeyIdxTbl != NULL)
    {
        u4NumberOfEntry = _rSwdmxWmvInfo.prWmvKeyIdxTbl->ui4_number_of_entry;
    }
    else
    {
        LOG(3,"_SWDMX_WMVTrickModeParsing : _rSwdmxWmvInfo.prWmvKeyIdxTbl == NULL\n");
        ASSERT(_rSwdmxWmvInfo.prWmvKeyIdxTbl == NULL);
        return TRUE;
    }
    
#ifdef WMA_TRICK_MODE
    if((!_rSwdmxWmvInfo.fgDemuxFinish)&&(_rSwdmxWmvInfo.fgFinishKeyTable) && (_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES) && (_rSwdmxWmvInfo.i4WMVPlaySpeed < 0))
    {
        LOG(0,"DSP finished, Send EOS\n");
        LOG(2,"Send Eos 6\n");
        _SWDMX_WMVSendEOS(prSwdmxInst);
    }
#endif
    if (_rSwdmxWmvInfo.prWmvKeyIdxTbl != NULL && !_rSwdmxWmvInfo.fgFinishKeyTable)
    {
        UINT32 i=0;
        UINT32 u8Baseoffset = (UINT32)_rSwdmxWmvInfo.prWmvKeyIdxTbl->ui8_base_offset;
        UINT32 u4DemuxSize;
        UINT32 u4DemuxPacketCount;
        UINT32 u4ParsingPacketNo;
        UINT32 u4MoreParsingPacket;
        
        prKeyidxEntry = (MIDXBULD_ELMT_STRM_ENTRY_T*)(_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry+_rSwdmxWmvInfo.u4CurKeyIdx);
        if (prKeyidxEntry == NULL)
        {
            ASSERT(0);
            return FALSE;
        }
        
        u8address = u8Baseoffset + prKeyidxEntry->ui4_relative_offset;
        
#ifdef WMA_TRICK_MODE
        if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
        {   
            u4DemuxSize = prKeyidxEntry->ui4_size;
            u4DemuxPacketCount = prKeyidxEntry->ui4_size/_rSwdmxWmvInfo.u4PacketSize;
			
			if(_rSwdmxWmvInfo.u2PreviousSendPayloadNo==0)
			{
				if(!_SWDMX_WMVGetFeederData(prSwdmxInst,TRUE))
				{
				   if(_rSwdmxWmvInfo.fgFeederInvalid)
				   {
					  return FALSE;
				   }
				   else
				   {
					  _SWDMX_WMVRequestFeederData(prSwdmxInst, u4DemuxSize,u8address,TRUE);
					  _SWDMX_WMVGetFeederData(prSwdmxInst, FALSE);
					  if(_rSwdmxWmvInfo.fgFeederInvalid ||_rSwdmxWmvInfo.fgDemuxFinish)
					  {
						 return FALSE;
					  }
				   }
				}
				fgReqedData=TRUE;
			}
			else
			{
                 u4DemuxPacketCount=u4DemuxPacketCount-(_rSwdmxWmvInfo.u2PreviousSendPayloadNo-1);
				 _rSwdmxWmvInfo.u2SendPayloadNo = _rSwdmxWmvInfo.u2PreviousSendPayloadNo-1;
			}
            
        }
        else
#endif
        {
            u4ParsingPacketNo = (prKeyidxEntry->ui4_relative_offset/_rSwdmxWmvInfo.u4PacketSize);
            u4MoreParsingPacket = (UINT32)min(MORE_DEMUX_PACKET_COUNT,_rSwdmxWmvInfo.u4TotalPacketNo-u4ParsingPacketNo
                                              -(prKeyidxEntry->ui4_size/_rSwdmxWmvInfo.u4PacketSize));
            u4DemuxSize = prKeyidxEntry->ui4_size + (u4MoreParsingPacket*_rSwdmxWmvInfo.u4PacketSize);
            u4DemuxPacketCount = (prKeyidxEntry->ui4_size/_rSwdmxWmvInfo.u4PacketSize) + u4MoreParsingPacket;

            if (u4DemuxSize != 0)
            {
               _SWDMX_WMVRequestFeederData(prSwdmxInst, u4DemuxSize,u8address,TRUE);
               _SWDMX_WMVGetFeederData(prSwdmxInst, FALSE);
               if (_rSwdmxWmvInfo.fgFeederInvalid || _rSwdmxWmvInfo.fgDemuxFinish)
               {
                   return FALSE;
               } 
            }
			fgReqedData=TRUE;
        }

		if(fgReqedData)
		{
        x_memset(&rWmvFeederReqData,0,sizeof(rWmvFeederReqData));

        zMsgSize = sizeof(WMV_FEEDER_REQ_DATA_T);
        i4Ret = x_msg_q_receive(&u2MsgQIdx, &rWmvFeederReqData, &zMsgSize,
                                &(_hWMVFeederDataQue), 1, X_MSGQ_OPTION_WAIT);

        if (i4Ret != OSR_OK)
        {
            ASSERT(0);
        }

        x_memcpy((VOID*)&_rFeederReqData,(VOID*)&rWmvFeederReqData.rFeederReqData,sizeof(FEEDER_REQ_DATA_T));
        _u4DefalutReqSize = rWmvFeederReqData.u4RequestFeederSize;

        _rSwdmxWmvInfo.u4PacketStartAddr = _rFeederReqData.u4WriteAddr + _rFeederReqData.u4AlignOffset;
        _rSwdmxWmvInfo.u4FeederReadSize = _rFeederReqData.u4ReadSize;

        _rSwdmxWmvInfo.u1FWKeyFrameNo =0;
			_rSwdmxWmvInfo.fgGetKeyHead = FALSE;
			
			_rSwdmxWmvInfo.u2PreviousSendPayloadNo =0;
			_rSwdmxWmvInfo.u2SendPayloadNo = 0;
		}
		
		_rSwdmxWmvInfo.fgBlock = FALSE;
		_rSwdmxWmvInfo.u4CurParsingPacketNo = prKeyidxEntry->ui4_relative_offset/_rSwdmxWmvInfo.u4PacketSize;

        if(prSwdmxInst->u1SwdmxId<SWDMX_SOURCE_MAX)
        {
            fgTrickTrigger[prSwdmxInst->u1SwdmxId]=FALSE;
        }
        for (i=0 ; i<u4DemuxPacketCount ; i++)
        {
            UINT32 u4AddrDelta = _rFeederInfo.u4EndAddr - _rSwdmxWmvInfo.u4PacketStartAddr;
            if (u4AddrDelta < _rSwdmxWmvInfo.u4PacketSize)
            {
                x_memcpy((VOID*)(_rFeederInfo.u4StartAddr-u4AddrDelta),(VOID*)(_rSwdmxWmvInfo.u4PacketStartAddr),u4AddrDelta);
                _rSwdmxWmvInfo.u4PacketStartAddr = _rFeederInfo.u4StartAddr-u4AddrDelta;
#if defined(CC_MT5395) || defined(CC_MT5365)
				HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)(_rSwdmxWmvInfo.u4PacketStartAddr)), (UINT32)u4AddrDelta);
#elif (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
				HalFlushDCacheMultipleLine(VIRTUAL((UINT32)(_rSwdmxWmvInfo.u4PacketStartAddr)), (UINT32)u4AddrDelta);
#else
			    HalFlushInvalidateDCache();
#endif 
            }
            _WMVParsingPacket(prSwdmxInst, _rSwdmxWmvInfo.u4PacketStartAddr,0,_rSwdmxWmvInfo.u4PacketSize);
			if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES &&_rSwdmxWmvInfo.fgBlock==TRUE)
			{
               return TRUE;
			}
			
            _rSwdmxWmvInfo.u4PacketStartAddr += _rSwdmxWmvInfo.u4PacketSize;
            if (_rSwdmxWmvInfo.fgFinishDemuxIFrame)
            {
                _rSwdmxWmvInfo.fgFinishDemuxIFrame = FALSE;
                break;
            }
        }

#ifdef CC_3D_MM_DS_SUPPORT  // megaa 20110113: tmp solution for 3D multi-track trick mode
		if(i==u4DemuxPacketCount&&_rSwdmxWmvInfo.u1FWKeyFrameNo==1)
#else
		if(i==u4DemuxPacketCount&&_rSwdmxWmvInfo.u1FWKeyFrameNo==1)
#endif
		{
           _fgReceiveVideoPes=FALSE;
		}

        FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, _rSwdmxWmvInfo.u4PacketStartAddr);
#ifdef WMV_FAST_I_FRAME_PARSING
        _u8PreKeyFrameAddr = u8address;
#endif
    }

#ifdef WMV_FAST_I_FRAME_PARSING
NEXT_I_FRAME:
#endif

    if ((_rSwdmxWmvInfo.i4WMVPlaySpeed < 0 && (_rSwdmxWmvInfo.u4CurKeyIdx == 0 || _rSwdmxWmvInfo.u4CurParsingPacketNo==0 ))
            || (_rSwdmxWmvInfo.i4WMVPlaySpeed >=4000 && _rSwdmxWmvInfo.u4CurKeyIdx >= u4NumberOfEntry-1)
       )
    {
        if (!_rSwdmxWmvInfo.fgFinishKeyTable)
        {
#ifdef WMA_TRICK_MODE
            if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
            {
                _rSwdmxWmvInfo.fgFinishKeyTable = TRUE;
            }
            else
#endif
            {
                LOG(2,"Send Eos 7\n");
                _SWDMX_WMVSendEOS(prSwdmxInst);
                _rSwdmxWmvInfo.fgFinishKeyTable = TRUE;
                return TRUE;
            }
        }
    }
    else
    {
        if (_rSwdmxWmvInfo.i4WMVPlaySpeed<0)
        {
#ifdef WMA_TRICK_MODE
            if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
            {
                UINT32 u4JumpSec,u4SeekTime,u4PrePts;

                u4JumpSec = abs(_rSwdmxWmvInfo.i4WMVPlaySpeed)/1000+1;
                if (prKeyidxEntry == NULL)
                {
                    ASSERT(0);
                    return FALSE;
                }
                u4SeekTime = prKeyidxEntry->ui4_pts;
                if(_rSwdmxWmvInfo.u4CurKeyIdx>0)
                {
                    _rSwdmxWmvInfo.u4CurKeyIdx--;
                }
                else
                {
                   _rSwdmxWmvInfo.fgFinishKeyTable=TRUE;
                   return TRUE;
                }
                prKeyidxEntry = (MIDXBULD_ELMT_STRM_ENTRY_T*)(_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry+_rSwdmxWmvInfo.u4CurKeyIdx);
                if (prKeyidxEntry == NULL)
                {
                    ASSERT(0);
                    return FALSE;
                }
                u4PrePts = prKeyidxEntry->ui4_pts;
                for(;u4JumpSec>0;u4JumpSec--)
                {
                    while((u4SeekTime == u4PrePts)&&(_rSwdmxWmvInfo.u4CurKeyIdx > 0))
                    {   
                        if(_rSwdmxWmvInfo.u4CurKeyIdx>0)
                        {
                           _rSwdmxWmvInfo.u4CurKeyIdx--;
                        }

                        prKeyidxEntry = (MIDXBULD_ELMT_STRM_ENTRY_T*)(_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry+_rSwdmxWmvInfo.u4CurKeyIdx);
                        if (prKeyidxEntry == NULL)
                        {
                            ASSERT(0);
                            return FALSE;
                        }
                        u4PrePts = prKeyidxEntry->ui4_pts;
                    }
                    u4SeekTime = u4PrePts;
                }
                prKeyidxEntry = (MIDXBULD_ELMT_STRM_ENTRY_T*)(_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry+_rSwdmxWmvInfo.u4CurKeyIdx);
                if (prKeyidxEntry == NULL)
                {
                    ASSERT(0);
                    return FALSE;
                }
                u4SeekTime = prKeyidxEntry->ui4_pts;
                _rSwdmxWmvInfo.u4IndexPts=0;
				_rSwdmxWmvInfo.u2PreviousSendPayloadNo=0;
                _SWDMX_WMVSeek(prSwdmxInst,u4SeekTime,NULL);
            }
            else if(_rSwdmxWmvInfo.u4CurKeyIdx>0)
#endif
            {
				if(_fgReceiveVideoPes)
				{
				   UINT32 u4DeltaTime=0;
				   UINT32 u4PrePts=0;
				   prKeyidxEntry = (MIDXBULD_ELMT_STRM_ENTRY_T*)(_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry+_rSwdmxWmvInfo.u4CurKeyIdx);
				   if (prKeyidxEntry == NULL)
				   {
					   ASSERT(0);
					   return FALSE;
				   }

				   u4PrePts=prKeyidxEntry->ui4_pts;
				   do
				   {
				   	 if(_rSwdmxWmvInfo.u4CurKeyIdx==0)
					 {
                        break;
					 }
				     _rSwdmxWmvInfo.u4CurKeyIdx--;

				     prKeyidxEntry = (MIDXBULD_ELMT_STRM_ENTRY_T*)(_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry+_rSwdmxWmvInfo.u4CurKeyIdx);
					 if (prKeyidxEntry == NULL)
					 {
						 ASSERT(0);
						 return FALSE;
					 }

					 u4DeltaTime=u4PrePts-prKeyidxEntry->ui4_pts;
				   }while(u4DeltaTime<90*abs(_rSwdmxWmvInfo.i4WMVPlaySpeed));
				}
				else
				{
					_rSwdmxWmvInfo.u4CurKeyIdx--;
				}
            }
        }
        else if (_rSwdmxWmvInfo.i4WMVPlaySpeed>0)
        {
			if(_fgReceiveVideoPes)
			{
				UINT32 u4DeltaTime=0;
				UINT32 u4PrePts=0;
				prKeyidxEntry = (MIDXBULD_ELMT_STRM_ENTRY_T*)(_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry+_rSwdmxWmvInfo.u4CurKeyIdx);
				u4PrePts=prKeyidxEntry->ui4_pts;
				do
				{
				  if(_rSwdmxWmvInfo.u4CurKeyIdx>=_rSwdmxWmvInfo.prWmvKeyIdxTbl->ui4_number_of_entry-1)
				  {
                     break;
				  }
				  
				  _rSwdmxWmvInfo.u4CurKeyIdx++;
				  prKeyidxEntry = (MIDXBULD_ELMT_STRM_ENTRY_T*)(_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry+_rSwdmxWmvInfo.u4CurKeyIdx);
				  u4DeltaTime=prKeyidxEntry->ui4_pts-u4PrePts;
				}while(u4DeltaTime<90*abs(_rSwdmxWmvInfo.i4WMVPlaySpeed));
			}
			else
			{
				_rSwdmxWmvInfo.u4CurKeyIdx++;
			}
        }
    }
#ifdef WMA_TRICK_MODE
    if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
    {
        return TRUE;
    }
#endif
#ifdef WMV_FAST_I_FRAME_PARSING
    prKeyidxEntry = (MIDXBULD_ELMT_STRM_ENTRY_T*)(_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry+_rSwdmxWmvInfo.u4CurKeyIdx);
    if (prKeyidxEntry == NULL)
    {
        ASSERT(0);
        return FALSE;
    }
    u8address = _rSwdmxWmvInfo.prWmvKeyIdxTbl->ui8_base_offset + prKeyidxEntry->ui4_relative_offset;
    if (u8address == _u8PreKeyFrameAddr)
    {
        u4RetryTimes++;
        if (u4RetryTimes < 100)
            goto NEXT_I_FRAME;
    }
#endif
    return TRUE;
}

BOOL _SWDMX_WMVDeliverData(SWDMX_INFO_T *prSwdmxInst, SWDMX_RANGE_LIST_T* prRangeList)
{
    INT32 i4Ret;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    BOOL fgRet = FALSE;
    WMV_FEEDER_REQ_DATA_T rWmvFeederReqData;

    BOOL fgTrickMode = prSwdmxInst->ePlayMode==SWDMX_PLAY_MODE_NORMAL ? FALSE : TRUE;
#ifdef WMA_TRICK_MODE
    if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
    {
      if(_rSwdmxWmvInfo.i4WMVPlaySpeed>=2000)
      {
        fgTrickMode=FALSE;
      }
    }
#endif

    if (_rSwdmxWmvInfo.fgDemuxFinish)
    {
        if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES && _rSwdmxWmvInfo.i4WMVPlaySpeed<0)
		{
		   if(prSwdmxInst->u1SwdmxId<SWDMX_SOURCE_MAX)
		   {
			 if((!fgTrickTrigger[prSwdmxInst->u1SwdmxId])&&(AUD_IsDecoderRealPlay(prSwdmxInst->u1AdecId)))
			 {
			   AUD_GetAudioTrickPts(prSwdmxInst->u1AdecId, &u4AudDspPts[prSwdmxInst->u1SwdmxId], FALSE);
			 }
		   }
		}
		x_thread_delay(20);
        return FALSE;
    }
    if (_rSwdmxWmvInfo.fgFeederInvalid)
    {
        _rSwdmxWmvInfo.fgFeederInvalid = FALSE;
    }

#if 0     // move to swdmx_drvif.c common part
    if(_prRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK_NETFLIX)
    {
        _WMVCheckDataExhausted(prSwdmxInst);
    }        
#endif

    if ( !_rSwdmxWmvInfo.fgBlock && _rSwdmxWmvInfo.fgFirstFrame && !fgTrickMode )
    {
        //VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(prSwdmxInst->u1VdecId);

        //_SWDMX_WMVComsumeFeeder(prSwdmxInst);

        if (_rSwdmxWmvInfo.fgFirstReqFeederData)
        {
            _SWDMX_WMVRequestFeederData(prSwdmxInst, 0,0,FALSE);
            _rSwdmxWmvInfo.fgFirstReqFeederData = FALSE;
        }

        fgRet = _SWDMX_WMVGetFeederData(prSwdmxInst, FALSE);
        if (!fgRet)
        {
            if (_rSwdmxWmvInfo.fgFeederInvalid || _rSwdmxWmvInfo.fgDemuxFinish)
            {
                return FALSE;
            }
        }
        else
        {
            _rSwdmxWmvInfo.fgFirstFrame = FALSE;
            _SWDMX_WMVRequestFeederData(prSwdmxInst, 0,0,FALSE);
            _rSwdmxWmvInfo.fgFirstReqFeederData = FALSE;
        }

        if(_prRangeInfo->e_mm_src_type != MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            zMsgSize = sizeof(WMV_FEEDER_REQ_DATA_T);
            i4Ret = x_msg_q_receive(&u2MsgQIdx, &rWmvFeederReqData, &zMsgSize,
                                &(_hWMVFeederDataQue), 1, X_MSGQ_OPTION_WAIT);

            if (i4Ret != OSR_OK)
            {
                ASSERT(0);
            }

            x_memcpy((VOID*)&_rFeederReqData,(VOID*)&rWmvFeederReqData.rFeederReqData,sizeof(FEEDER_REQ_DATA_T));
            _u4DefalutReqSize = rWmvFeederReqData.u4RequestFeederSize;

            _rSwdmxWmvInfo.u4PacketStartAddr = _rFeederReqData.u4WriteAddr + _rFeederReqData.u4AlignOffset; 

            _rSwdmxWmvInfo.u4FeederReadSize = _rFeederReqData.u4ReadSize-_rFeederReqData.u4AlignOffset;
#ifdef CC_NETFLIX_CMPB_SUPPORT
            if(_prRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK_NETFLIX)
            {
                _rSwdmxWmvInfo.u4PacketStartAddr -= _u4RemainPacketDataSize;
                _rSwdmxWmvInfo.u4FeederReadSize += _u4RemainPacketDataSize;
            }
#endif
            //get feeder buffer information
            if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &_rFeederInfo) != FEEDER_E_OK)
            {
                LOG(3, "_SWDMX_WMVDeliverData FeederGetBufferInfo() error.\n");
                return FALSE;
            }

            FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, _rSwdmxWmvInfo.u4PacketStartAddr);
        }
#ifndef ENABLE_WMV_DRM
        if (_prRangeInfo->e_mm_src_type != MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            if (_rSwdmxWmvInfo.u8BeginParseOfst == 0)
            {
                UINT64 u8Field;
                x_memcpy(&(u8Field),(VOID*)VIRTUAL(_rSwdmxWmvInfo.u4PacketStartAddr+16),8);
                _rSwdmxWmvInfo.u4DataObjSize = (UINT32)u8Field;
                x_memcpy(&(u8Field),(VOID*)VIRTUAL(_rSwdmxWmvInfo.u4PacketStartAddr+40),8);
                _rSwdmxWmvInfo.u4TotalPacketNo = (UINT32)u8Field;
            }
            else
            {
                _rSwdmxWmvInfo.u4DataObjSize = 0;
                _rSwdmxWmvInfo.u4TotalPacketNo = 0;
            }

            if (_rSwdmxWmvInfo.u4TotalPacketNo ==0 || _rSwdmxWmvInfo.u4DataObjSize==0 || _prRangeInfo->ui4_sz_min_acces_unt ==0)
            {
                UINT64 u8Remainder;
                if (_prRangeInfo->ui4_sz_min_acces_unt ==0)
                {
                    LOG(2,"Bad file, unable to be played!!! Send Eos 8\n");
                    _SWDMX_WMVSendEOS(prSwdmxInst);
                    return FALSE;
                }
                
                _rSwdmxWmvInfo.u4DataObjSize = (UINT32)_rSwdmxWmvInfo.u8FileSize;
                _rSwdmxWmvInfo.u4TotalPacketNo = (UINT32)u8Div6432((_rSwdmxWmvInfo.u8FileSize-WMV_DATA_OBJ_HEADER),
                                                 _prRangeInfo->ui4_sz_min_acces_unt, &u8Remainder);

                if (_rSwdmxWmvInfo.u4TotalPacketNo == 0)  // tmp solution for DTV00212590
                    _rSwdmxWmvInfo.u4TotalPacketNo = 1;   // this will cause _SWDMX_WMVSendEOS immediately

                //_SWDMX_WMVSendEOS(prSwdmxInst);
                //return FALSE;
            }
            _rSwdmxWmvInfo.u4PacketSize = (_rSwdmxWmvInfo.u4DataObjSize-WMV_DATA_OBJ_HEADER)/_rSwdmxWmvInfo.u4TotalPacketNo;

            if (_rSwdmxWmvInfo.u4PacketSize != _prRangeInfo->ui4_sz_min_acces_unt && _prRangeInfo->ui4_sz_min_acces_unt!=0)
            {
                _rSwdmxWmvInfo.u4PacketSize = _prRangeInfo->ui4_sz_min_acces_unt;
            }
        }
        else
        {
            _rSwdmxWmvInfo.u4DataObjSize = 0xFFFFFFF0;
            _rSwdmxWmvInfo.u4TotalPacketNo = 0xFFFFFFF0;
        }
#else
        _rSwdmxWmvInfo.u4TotalPacketNo = 1416;
        _rSwdmxWmvInfo.u4PacketSize = 5175;
#endif
        LOG(5,"total data packet : %d \n",_rSwdmxWmvInfo.u4TotalPacketNo);
        LOG(5,"packet size : %d \n",_rSwdmxWmvInfo.u4PacketSize);

        //_rSwdmxWmvInfo.u4CurPos = WMV_DATA_OBJ_HEADER; //packet header size is 50 bytes
        if (_prRangeInfo->e_mm_src_type != MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            if (_rSwdmxWmvInfo.u8BeginParseOfst == 0)
            {
                _rSwdmxWmvInfo.u4PacketStartAddr += WMV_DATA_OBJ_HEADER;
                _rSwdmxWmvInfo.u4ConsumedSize = WMV_DATA_OBJ_HEADER;
            }
        }
        else
        {
            _rSwdmxWmvInfo.u4ConsumedSize = 0;
        }

        _rSwdmxWmvInfo.fgFirstFrame = FALSE;

        if (_rSwdmxWmvInfo.fgEnVideo)
        {
            VDEC_StartPTS(prSwdmxInst->u1VdecId,TRUE,0,NULL); //maiyou
            if(ENABLE_NEWTRACK(prSwdmxInst))
            {
                VDEC_StartPTS(prSwdmxInst->u1Vdec2Id,TRUE,0,NULL); 
            }
        }

#if 1
        if (!_SWDMX_WMVFeederIsReady(prSwdmxInst))
        {
            return TRUE;
        }
#endif
    }

    else if (!_rSwdmxWmvInfo.fgBlock && (_rSwdmxWmvInfo.u4CurParsingPacketNo < _rSwdmxWmvInfo.u4TotalPacketNo)
             && !fgTrickMode )
    {
        if (!_SWDMX_WMVIsFeederBufferFull(prSwdmxInst))
        {
            if ( _rSwdmxWmvInfo.fgFirstReqFeederData )
            {
                _SWDMX_WMVRequestFeederData(prSwdmxInst, 0,0,FALSE);
                _rSwdmxWmvInfo.fgFirstReqFeederData = FALSE;
            }

            if(_prRangeInfo->e_mm_src_type != MM_SRC_TYPE_NETWORK_NETFLIX)
            {
				if(_prRangeInfo->e_mm_src_type!=MM_SRC_TYPE_HIGH_SPEED_STORAGE)
				{
				    UINT64 u8UnParsedPacket,u8UnParsedData,u8Remainder;
					u8UnParsedData=_rSwdmxWmvInfo.u8LastFileParsingPosition-_prRangeInfo->ui8_fileoffset-WMV_DATA_OBJ_HEADER-_rSwdmxWmvInfo.u4CurParsingPacketNo*_rSwdmxWmvInfo.u4PacketSize;
					u8UnParsedPacket=u8Div6432(u8UnParsedData,_rSwdmxWmvInfo.u4PacketSize,&u8Remainder);
					UNUSED(u8Remainder);
					if(u8UnParsedPacket>=2)
					{
						fgRet = _SWDMX_WMVGetFeederData(prSwdmxInst, TRUE); 
					}
					else
					{
						fgRet = _SWDMX_WMVGetFeederData(prSwdmxInst, FALSE);		 
					}
					
					if (!fgRet)
					{
                         if ( _u4RequestDataSize ==0)
						 {
							 //need to request data again
							 _SWDMX_WMVRequestFeederData(prSwdmxInst,0,0,FALSE);
							 _rSwdmxWmvInfo.fgFirstReqFeederData = FALSE;
						 }
						 else if (_rSwdmxWmvInfo.u4ConsumedSize ==0 && (u8UnParsedPacket<2 || _rSwdmxWmvInfo.fgDemuxFinish))
						 {
							 return FALSE;
						 }
						 
					}
					else
					{
						 //need to request data again
						 _SWDMX_WMVRequestFeederData(prSwdmxInst, 0,0,FALSE);
						 _rSwdmxWmvInfo.fgFirstReqFeederData = FALSE;
					}
				}
				else
				{
					fgRet = _SWDMX_WMVGetFeederData(prSwdmxInst, FALSE);
					if (!fgRet)
					{
						if (_rSwdmxWmvInfo.fgFeederInvalid || _rSwdmxWmvInfo.fgDemuxFinish)
						{
							return FALSE;
						}
						else if ( _u4RequestDataSize ==0 )
						{
							//need to request data again
							_SWDMX_WMVRequestFeederData(prSwdmxInst,0,0,FALSE);
							_rSwdmxWmvInfo.fgFirstReqFeederData = FALSE;
						}
					}
					else
					{
						//need to request data again
						_SWDMX_WMVRequestFeederData(prSwdmxInst, 0,0,FALSE);
						_rSwdmxWmvInfo.fgFirstReqFeederData = FALSE;
					}
				}

            }
            else //netflix mode
            {
                UINT16 u2Cnt = 0;
                VERIFY(x_msg_q_num_msgs(_hWMVFeederDataQue, &u2Cnt) == OSR_OK);
                
                if((MPV_IsDisplay(prSwdmxInst->u1VdecId)) && u2Cnt>=1)
                {
                    // don't receive feeder ack when data is >=1
                }
                else
                {
                    fgRet = _SWDMX_WMVGetFeederData(prSwdmxInst, FALSE);
                    if (!fgRet)
                    {
                        if (_rSwdmxWmvInfo.fgFeederInvalid || _rSwdmxWmvInfo.fgDemuxFinish)
                        {
                            return FALSE;
                        }
                        else if ( _u4RequestDataSize ==0 )
                        {
                            //need to request data again
                            _SWDMX_WMVRequestFeederData(prSwdmxInst,0,0,FALSE);
                            _rSwdmxWmvInfo.fgFirstReqFeederData = FALSE;
                        }
                    }
                    else
                    {
                        //need to request data again
                        _SWDMX_WMVRequestFeederData(prSwdmxInst, 0,0,FALSE);
                        _rSwdmxWmvInfo.fgFirstReqFeederData = FALSE;
                    }                
                }
            }


            if (!_SWDMX_WMVFeederIsReady(prSwdmxInst))
            {
                x_thread_delay(50);
                return TRUE;
            }

        }

        if ( _rSwdmxWmvInfo.u4ConsumedSize ==0 )
        {
            WMV_FEEDER_REQ_DATA_T rWmvFeederReqData;
            UINT16 u2Cnt = 0;
            zMsgSize = sizeof(WMV_FEEDER_REQ_DATA_T);

            // tmp solution for DTV00085618
            VERIFY(x_msg_q_num_msgs(_hWMVFeederDataQue, &u2Cnt) == OSR_OK);
            if (u2Cnt == 0)  // which will cause deadlock at the x_msg_q_receive below
            {
                LOG(2,"Send Eos 9\n");
                _SWDMX_WMVSendEOS(prSwdmxInst);
                return TRUE;
            }
            
            i4Ret = x_msg_q_receive(&u2MsgQIdx, &rWmvFeederReqData, &zMsgSize,
                                    &(_hWMVFeederDataQue), 1, X_MSGQ_OPTION_WAIT);
            if (i4Ret != OSR_OK)
            {
                ASSERT(0);
            }

            x_memcpy((VOID*)&_rFeederReqData,(VOID*)&rWmvFeederReqData.rFeederReqData,sizeof(FEEDER_REQ_DATA_T));
            _u4DefalutReqSize = rWmvFeederReqData.u4RequestFeederSize;

            _rSwdmxWmvInfo.u4FeederReadSize = _rFeederReqData.u4ReadSize-_rFeederReqData.u4AlignOffset;
            _rSwdmxWmvInfo.u4PacketStartAddr = _rFeederReqData.u4WriteAddr + _rFeederReqData.u4AlignOffset;
#ifdef CC_NETFLIX_CMPB_SUPPORT
            if(_prRangeInfo->e_mm_src_type==MM_SRC_TYPE_NETWORK_NETFLIX)
            {
                _rSwdmxWmvInfo.u4FeederReadSize += _u4RemainPacketDataSize;
                _rSwdmxWmvInfo.u4PacketStartAddr -= _u4RemainPacketDataSize;
                x_memcpy((VOID*)(_rSwdmxWmvInfo.u4PacketStartAddr),(VOID*)VIRTUAL((UINT32)_pu1RemainPacketData),_u4RemainPacketDataSize);
#if defined(CC_MT5395) || defined(CC_MT5365) || defined(CC_MT5396) ||defined(CC_MT5389)
                //HalFlushInvalidateDCache();
                HalFlushDCacheMultipleLine((UINT32)(_rSwdmxWmvInfo.u4PacketStartAddr), (UINT32)_u4RemainPacketDataSize);
#else
                HalFlushInvalidateDCache();
#endif				

            }
#endif
                
            FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, _rSwdmxWmvInfo.u4PacketStartAddr);
        }
    }
    else if ((_rSwdmxWmvInfo.u4CurParsingPacketNo >= _rSwdmxWmvInfo.u4TotalPacketNo)
             && (_prRangeInfo->e_mm_src_type != MM_SRC_TYPE_NETWORK_NETFLIX))
    {
        LOG(2,"Send Eos 10\n");
        _SWDMX_WMVSendEOS(prSwdmxInst);
        return TRUE;
    }

    else if (fgTrickMode)
    {
       BOOL fgRetValue=TRUE;
	   
	   fgRetValue= _SWDMX_WMVTrickModeParsing(prSwdmxInst);
	   if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
       {
          if(prSwdmxInst->u1SwdmxId<SWDMX_SOURCE_MAX)
          {
            if((!fgTrickTrigger[prSwdmxInst->u1SwdmxId])&&(AUD_IsDecoderRealPlay(prSwdmxInst->u1AdecId)))
            {
              AUD_GetAudioTrickPts(prSwdmxInst->u1AdecId, &u4AudDspPts[prSwdmxInst->u1SwdmxId], FALSE);
            }
          }
       }
	   
	   return fgRetValue;
    }

    //pasrsing packet
    //do
    {
        UINT32 u4AddrDelta;

        _rSwdmxWmvInfo.u2SendPayloadNo = 0;
        _rSwdmxWmvInfo.fgBlock = FALSE;

        u4AddrDelta = _rFeederInfo.u4EndAddr - _rSwdmxWmvInfo.u4PacketStartAddr;

        if (u4AddrDelta < _rSwdmxWmvInfo.u4PacketSize)
        {
            LOG(1,"swdmx wmv handle ring buffer\n");
            x_memcpy((VOID*)VIRTUAL(_rFeederInfo.u4StartAddr-u4AddrDelta),(VOID*)VIRTUAL(_rSwdmxWmvInfo.u4PacketStartAddr),u4AddrDelta);
            _rSwdmxWmvInfo.u4PacketStartAddr = _rFeederInfo.u4StartAddr-u4AddrDelta;
#if defined(CC_MT5395) || defined(CC_MT5365)
            HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)(_rSwdmxWmvInfo.u4PacketStartAddr)), (UINT32)u4AddrDelta);
#elif (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
            HalFlushDCacheMultipleLine(VIRTUAL((UINT32)(_rSwdmxWmvInfo.u4PacketStartAddr)), (UINT32)u4AddrDelta);
#else
            HalFlushInvalidateDCache();
#endif                        
        }

        _WMVParsingPacket(prSwdmxInst, _rSwdmxWmvInfo.u4PacketStartAddr,0,_rSwdmxWmvInfo.u4PacketSize);
		if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
		{
		   if(prSwdmxInst->u1SwdmxId<SWDMX_SOURCE_MAX)
		   {
		     u4AudDspPts[prSwdmxInst->u1SwdmxId]=1;
		   }
		}

        if (_rSwdmxWmvInfo.fgBrokenFile || _rSwdmxWmvInfo.fgDemuxFinish)
        {
            LOG(2,"Send Eos 11\n");
            _SWDMX_WMVSendEOS(prSwdmxInst);
            return TRUE;
        }

#ifdef WMV_BAD_INTLV_SUPPORT
        if (_rBadIntlv.eState == BAD_INTLV_STT_TO_PATCH_A || _rBadIntlv.eState == BAD_INTLV_STT_TO_PATCH_V)
        {
            _rBadIntlv.eState = (_rBadIntlv.eState == BAD_INTLV_STT_TO_PATCH_A ? BAD_INTLV_STT_PATCH_A : BAD_INTLV_STT_PATCH_V);
            _rSwdmxWmvInfo.fgBlock = FALSE;
        }
        else if (_rBadIntlv.eState == BAD_INTLV_STT_TO_REPRS_A || _rBadIntlv.eState == BAD_INTLV_STT_TO_REPRS_V)
        {
            _rSwdmxWmvInfo.fgBlock = FALSE;
        }
#endif            
        if (_rSwdmxWmvInfo.fgBlock)
        {
            //LOG(0,"fgBlock : TRUE , leaving _SWDMX_WMVSendData\n");
            return FALSE;
        }
        else
        {
            _rSwdmxWmvInfo.u2PreviousSendPayloadNo = 0;
        }

        _rSwdmxWmvInfo.u4PacketStartAddr += _rSwdmxWmvInfo.u4PacketSize;

        if (_prRangeInfo->e_mm_src_type != MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            _rSwdmxWmvInfo.u4ConsumedSize += _rSwdmxWmvInfo.u4PacketSize;
        }
        else
        {
#ifndef CC_NETFLIX_CMPB_SUPPORT
            _rSwdmxWmvInfo.u4PacketStartAddr += WMV_NETFLIX_HEADER_SIZE;
            _rSwdmxWmvInfo.u4ConsumedSize += 1;
#else
            _rSwdmxWmvInfo.u4PacketStartAddr += sizeof(NetflixEmbeddedCmd_T); //embedded cmd
            _rSwdmxWmvInfo.u4ConsumedSize += _rSwdmxWmvInfo.u4PacketSize;
            _rSwdmxWmvInfo.u4ConsumedSize += sizeof(NetflixEmbeddedCmd_T);
#endif
        }

        _rSwdmxWmvInfo.u4CurParsingPacketNo++;

        if (_rSwdmxWmvInfo.u4CurParsingPacketNo == _rSwdmxWmvInfo.u4TotalPacketNo
#ifdef __MODEL_slt__
                || _u4VC1SltLastPicPts > 33030
#endif
           )
        {
#ifdef __MODEL_slt__
            if (_rSwdmxWmvInfo.fgVideoPause)
            {
                _WMVStartToPlay(prSwdmxInst);
            }
#endif
#ifdef WMV_BAD_INTLV_SUPPORT
            if (_rBadIntlv.eState != BAD_INTLV_STT_NONE)
            {
                if (_rBadIntlv.eState == BAD_INTLV_STT_PATCH_A)
                {
                    _rBadIntlv.fgAudioEnd = TRUE;
                    _rBadIntlv.eState = BAD_INTLV_STT_TO_REPRS_V;
                }
                else if (_rBadIntlv.eState == BAD_INTLV_STT_PATCH_V)
                {
                    _rBadIntlv.fgVideoEnd = TRUE;
                    _rBadIntlv.eState = BAD_INTLV_STT_TO_REPRS_A;
                }
                else if (_rBadIntlv.eState == BAD_INTLV_STT_REPARSE_A)
                {
                    _rBadIntlv.fgAudioEnd = TRUE;
                    _WMVEndReparse(prSwdmxInst);
                    if (_rBadIntlv.fgAudioEnd && _rBadIntlv.fgVideoEnd)
                    {
                        LOG(2, "Send Eos 13a\n");
                        _SWDMX_WMVSendEOS(prSwdmxInst);
                        return TRUE;
                    }
                }
                else if (_rBadIntlv.eState == BAD_INTLV_STT_REPARSE_V)
                {
                    _rBadIntlv.fgVideoEnd = TRUE;
                    _WMVEndReparse(prSwdmxInst);
                    if (_rBadIntlv.fgAudioEnd && _rBadIntlv.fgVideoEnd)
                    {
                        LOG(2, "Send Eos 13b\n");
                        _SWDMX_WMVSendEOS(prSwdmxInst);
                        return TRUE;
                    }
                }
            }
            else
#endif  // WMV_BAD_INTLV_SUPPORT
            {
                LOG(2,"Send Eos 13\n");
                _SWDMX_WMVSendEOS(prSwdmxInst);
                return TRUE;
                //break;
            }
        }
#ifdef WMV_BAD_INTLV_SUPPORT
        else
        {
            UINT32 u4ADuration, u4VDuration;
            
            if ((_rBadIntlv.eState == BAD_INTLV_STT_PATCH_A || _rBadIntlv.eState == BAD_INTLV_STT_REPARSE_A) && !_WMV_AFifo_Low(prSwdmxInst, &u4ADuration) && _WMV_VFifo_Low(prSwdmxInst, &u4VDuration))
            {
                LOG(3, "Video underflow (%dms) due to bad interleave (%d)!!!\n", u4VDuration, __LINE__);
                _rBadIntlv.eState = (_rBadIntlv.eState == BAD_INTLV_STT_PATCH_A ? BAD_INTLV_STT_TO_REPRS_V : BAD_INTLV_STT_TO_PATCH_V);
            }
            else if ((_rBadIntlv.eState == BAD_INTLV_STT_PATCH_V || _rBadIntlv.eState == BAD_INTLV_STT_REPARSE_V) && !_WMV_VFifo_Low(prSwdmxInst, &u4VDuration) && _WMV_AFifo_Low(prSwdmxInst, &u4ADuration))
            {
                LOG(3, "Audio underflow (%dms) due to bad interleave (%d)!!!\n", u4ADuration, __LINE__);
                _rBadIntlv.eState = (_rBadIntlv.eState == BAD_INTLV_STT_PATCH_V ? BAD_INTLV_STT_TO_REPRS_A : BAD_INTLV_STT_TO_PATCH_A);
            }
            
            if (_rBadIntlv.eState == BAD_INTLV_STT_TO_REPRS_V || _rBadIntlv.eState == BAD_INTLV_STT_TO_REPRS_A)
            {
                _rBadIntlv.u4EndPatchPktNo = _rSwdmxWmvInfo.u4CurParsingPacketNo;
                _rBadIntlv.u4EndPatchPldNo = 0;
            }
            else if (_rBadIntlv.eState == BAD_INTLV_STT_TO_PATCH_V || _rBadIntlv.eState == BAD_INTLV_STT_TO_PATCH_A)
            {
                _rBadIntlv.u4BeginSkipPktNo = _rSwdmxWmvInfo.u4CurParsingPacketNo;
                _rBadIntlv.u4BeginSkipPldNo = 0;
            }
        }
#endif  // WMV_BAD_INTLV_SUPPORT

        FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, _rSwdmxWmvInfo.u4PacketStartAddr);

#ifdef WMV_BAD_INTLV_SUPPORT
        if (_WMVBeginReparse(prSwdmxInst))
        {
            return TRUE;
        }
        if (_WMVBeginRePatch(prSwdmxInst))
        {
            return TRUE;
        }
#endif

#ifndef CC_NETFLIX_CMPB_SUPPORT
        if (_prRangeInfo->e_mm_src_type != MM_SRC_TYPE_NETWORK_NETFLIX)
#endif         
        {
           UINT32 u4WMVPacketSize = 0;
#ifdef CC_NETFLIX_CMPB_SUPPORT            
           if (_prRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK_NETFLIX)
           {
               u4WMVPacketSize = NETFLIX_CMMP_BUFFER_SIZE;
           }
           else
#endif            
           {
               u4WMVPacketSize = _rSwdmxWmvInfo.u4PacketSize;
           }
       
           if (_rSwdmxWmvInfo.u4ConsumedSize + u4WMVPacketSize > _rSwdmxWmvInfo.u4FeederReadSize)
            {
                if (_rSwdmxWmvInfo.fgFirstFrame)
                {
                    _rSwdmxWmvInfo.fgFirstFrame = FALSE;
                }
#ifdef CC_NETFLIX_CMPB_SUPPORT
                
                if(_prRangeInfo->e_mm_src_type==MM_SRC_TYPE_NETWORK_NETFLIX)
                {
                    _u4RemainPacketDataSize = _rSwdmxWmvInfo.u4FeederReadSize - _rSwdmxWmvInfo.u4ConsumedSize;

                   HalInvalidateDCacheMultipleLine((UINT32)(_rSwdmxWmvInfo.u4PacketStartAddr), (UINT32)_u4RemainPacketDataSize);
                   
                   x_memcpy((VOID*)VIRTUAL((UINT32)_pu1RemainPacketData),(VOID*)_rSwdmxWmvInfo.u4PacketStartAddr,_u4RemainPacketDataSize);
#if defined(CC_MT5395) || defined(CC_MT5365) || defined(CC_MT5396) ||defined(CC_MT5389)
               //HalFlushInvalidateDCache();
               //HalInvalidateDCacheMultipleLine((UINT32)(_rSwdmxWmvInfo.u4PacketStartAddr), (UINT32)_u4RemainPacketDataSize);
#else
               HalFlushInvalidateDCache();
#endif					
                }
#endif
                _rSwdmxWmvInfo.u4ConsumedSize = 0;
                return TRUE;
            }

            if (_rSwdmxWmvInfo.u4ConsumedSize + _rSwdmxWmvInfo.u4PacketSize > _rSwdmxWmvInfo.u4FeederReadSize)
            {
                LOG(0,"feeder read size is smaller than request data size\n");
                LOG(2,"Send Eos 14\n");
                _SWDMX_WMVSendEOS(prSwdmxInst);
                _rSwdmxWmvInfo.u4ConsumedSize = 0;
                return TRUE;
            }
        }
#ifndef CC_NETFLIX_CMPB_SUPPORT        
        else
        {
            if (_rSwdmxWmvInfo.u4ConsumedSize + 1 > _rSwdmxWmvInfo.u4FeederReadSize)
            {
                if (_rSwdmxWmvInfo.fgFirstFrame)
                {
                    _rSwdmxWmvInfo.fgFirstFrame = FALSE;
                }
                _rSwdmxWmvInfo.u4ConsumedSize = 0;
                //LOG(0,"swdmx finish consume feeder data size = %d\n",_rSwdmxWmvInfo.u4FeederReadSize);
                return TRUE;
            }
        }
#endif        
        //return to main loop when finishing parsing a packet
        //LOG(0,"parsing packet number : %d/%d\n",_rSwdmxWmvInfo.u4CurParsingPacketNo,_rSwdmxWmvInfo.u4TotalPacketNo);
        return TRUE;
    }
    //} while (_rSwdmxWmvInfo.u4CurParsingPacketNo < _rSwdmxWmvInfo.u4TotalPacketNo);

    //return TRUE;
}

VOID _SWDMX_WMVSendEOS(SWDMX_INFO_T *prSwdmxInst)
{
    DMX_MM_DATA_T rDmxMMData_EOS;
    //UINT32 u4DummyData[3]={0xFFFFFFFF};

#ifdef __MODEL_slt__
    LOG(5, "VC1 SLT last pic type = 0x%X, PTS = %d\n", _u1VC1SltLastPicType, _u4VC1SltLastPicPts);
#endif

    x_memset((VOID*)&rDmxMMData_EOS,0,sizeof(DMX_MM_DATA_T));
    rDmxMMData_EOS.u4BufStart = _rFeederInfo.u4StartAddr;
    rDmxMMData_EOS.u4BufEnd = _rFeederInfo.u4EndAddr;
    rDmxMMData_EOS.u4StartAddr = _rFeederInfo.u4StartAddr;
    rDmxMMData_EOS.u4FrameSize = 4;
    rDmxMMData_EOS.fgEOS = TRUE;
    rDmxMMData_EOS.fgFrameHead= TRUE;
    LOG(0,"!!!!!!!! deliever EOS flag\n");
    LOG(0,"PlaySpeed=%d,Index=%d\n",_rSwdmxWmvInfo.i4WMVPlaySpeed,_rSwdmxWmvInfo.u4CurKeyIdx);
    //update variable to know we have sent EOS flag
    _rSwdmxWmvInfo.fgDemuxFinish = TRUE;
    _prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;

    if (_rSwdmxWmvInfo.fgEnVideo)//protect func
    {
        if (_SWDMX_WMVInitDmx(prSwdmxInst, eSWDMX_MEDIA_VIDEO,prSwdmxInst->u1VdecId,TRUE, TRUE) == FALSE)
        {
            LOG(0,"set dmx error when sending EOS flag\n");
            return;
        }
        if (_WMVDmxMoveData(prSwdmxInst,prSwdmxInst->u1SwdmxId,prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1VidPid, &rDmxMMData_EOS, 500) != TRUE)
        {
            LOG(0, "Demuxer fails to move frame start_code data.\n");
        }
        
        if(ENABLE_NEWTRACK(prSwdmxInst))
        {
            if (_SWDMX_WMVInitDmx(prSwdmxInst, eSWDMX_MEDIA_VIDEO,prSwdmxInst->u1Vdec2Id,TRUE, TRUE) == FALSE)
            {
                LOG(0,"set dmx error when sending EOS flag\n");
                return;
            }
            
            if (_WMVDmxMoveData(prSwdmxInst,prSwdmxInst->u1SwdmxId,prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1Vid2Pid, &rDmxMMData_EOS, 500) != TRUE)
            {
                LOG(0, "Demuxer fails to move frame start_code data.\n");
            }
        }
    }
    if (_rSwdmxWmvInfo.fgEnAudio)
    {
        if (_WMVDmxMoveData(prSwdmxInst,prSwdmxInst->u1SwdmxId,prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid, &rDmxMMData_EOS, 500) != TRUE)
        {
            LOG(0, "Demuxer fails to move frame start_code data.\n");
        }
    }

}


BOOL _SWDMX_WMVInit(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType)

{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_WMV_VERIFY_NULL_ERROR(prSwdmxInst);        
    
#if 0  // megaa 20100321
    static UINT32 _u4SwdmxWmvMsgQID = 0;
    char szMsgQName[] = { "WMVFQn" };
#endif

    LOG(5, "_SWDMX_WMVInit \n");

#ifdef WMV_BAD_INTLV_DEBUG
    _u4AFifoAccuLen = 0;
    _u4VFifoAccuLen = 0;
#endif
    
    //set audio call back function
    AUD_DrvSetWMAErrFunc(prSwdmxInst->u1AdecId, _WMVAudioErrorCallback, (VOID *)prSwdmxInst);  // megaa 20090828

#if 0  // for multi-instance, something has been done in common part
    FeederInstallCallback(prSwdmxInst->eFeederSrc, FEEDER_NOTIFY_DATA_REQ_DONE,
                          _SWDMX_FeederCallBack, (UINT32)&_rFeederInfo);
    // Initialize dmx
    fgRet = DMX_SelectInputType(DMX_IN_PLAYBACK_MM);
    if (fgRet == FALSE)
    {
        LOG(3, "DMX_SelectInputType DMX_IN_PLAYBACK_MM\n");
        return fgRet;
    }

    // moved to _SWDMX_WMVInitDmx
    DMX_SetToDecoder(TRUE);
    {
        DMX_DECODER_CALLBACKS_T rDecoderCallbacks;
        rDecoderCallbacks.pfnSendVideoPes = _WMVVideoCallback;//_VideoCallback;
        rDecoderCallbacks.pfnSendAudioPes = _WMVAudioCallback;//AUD_SendAudioPes;
        rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;
        //rDecoderCallbacks.pfnSendAudioPes = AUD_SendAudioPes;
        DMX_SetDecoderCallbacks(&rDecoderCallbacks);
    }
#endif

#if 1  // sometimes &_eCurFMT is not 4 byte aligned so that accessing it as UINT32* will cause data abort
    {
        UINT32 u4CurFMT;

        _SWDMX_GetInfo(u1SrcId, eSWDMX_CONTAINER_TYPE,(UINT32*)(&u4CurFMT),0,0);
        _eCurFMT = (ENUM_SWDMX_FMT_T)u4CurFMT;
        _rSwdmxWmvInfo.u4AudioCount = 0;
    }
#else
    _SWDMX_GetInfo(eSWDMX_CONTAINER_TYPE,(UINT32*)(&_eCurFMT),0,0);
#endif

    //x_memset(&(_rSwdmxWmvInfo),0,sizeof(SWDMX_WMV_INFO_T));  megaa 20100321: this has been done in common part
    
    _rSwdmxWmvInfo.u1AudioStreamId = 0xFF;
    _rSwdmxWmvInfo.fgDataExhausted = TRUE;
    _rSwdmxWmvInfo.u4NumberOfBitstream = 1;

#if 1  // megaa 20100321
    _WMVMsgQInit(u1SrcId);
#else
    if ( _hWMVFeederDataQue == NULL )
    {
        szMsgQName[5] = '0' + u1SrcId;  //_u4SwdmxWmvMsgQID++;
        //if (_u4SwdmxWmvMsgQID >= 10)
        //    _u4SwdmxWmvMsgQID = 0;
        // create receive feeder data queue
        VERIFY(x_msg_q_create(&_hWMVFeederDataQue, szMsgQName, sizeof(WMV_FEEDER_REQ_DATA_T),
                              SWMDX_WMV_FEEDER_DATA_Q_SIZE) == OSR_OK);
    }
    _SWDMX_WMVFlushFeederDataQ(prSwdmxInst);
#endif
#ifdef CC_NETFLIX_CMPB_SUPPORT
    _u4RemainPacketDataSize = 0;
#endif
    _rSwdmxWmvInfo.fgFirstFrame = TRUE;
    _rSwdmxWmvInfo.fgFirstReqFeederData = TRUE;
    _rSwdmxWmvInfo.fgBeginPlay = TRUE;
    _rSwdmxWmvInfo.u8SeekPTS = 0xFFFFFFFF;
    _rSwdmxWmvInfo.u8SeekPTS <<= 32;
    _rSwdmxWmvInfo.u8SeekPTS += 0xFFFFFFFF;
    _rSwdmxWmvInfo.u4AStreamId = 0xff;
    _rSwdmxWmvInfo.u4VStreamId = 0xff;
    _fgNetflixEos = FALSE;
    _fgDataExhausted = TRUE;
    _u1AudioStreamId = 0xFF;
    _u4PrevFramePTS = 0;
    _u4DefalutReqSize = 0;
    _fgReceiveAudioPes = FALSE;
    _fgReceiveVideoPes = FALSE;
#ifdef WMV_2X_TO_1X_PREBUFFER
    _rSwdmxWmvInfo.fgAlreadySet = FALSE;
#endif
#ifdef ENABLE_WMV_DRM
    _u8SampleId =0;
    _rSwdmxWmvInfo.fgIsDRM = TRUE;
#endif

    if (_pu1VideoFrameData == NULL)
    {
#if 1
        _pu1VideoFrameData = (UINT8 *)BSP_AllocAlignedDmaMemory(1*1024,16);
#else /* need mentor's double check, follow David's mail: "Please review swdmx internal buffer use" */
		{
			UINT32 u4Addr = 0;
			UINT32 u4Size = 1024;
			UINT32 u4Align = 16;
			SWDMX_GetInfo(u1SrcId, eSWDMX_GET_INTERNAL_BUF, &u4Addr, &u4Size, &u4Align);
			_pu1VideoFrameData = (UCHAR*)u4Addr;
		}
#endif

        if (_pu1VideoFrameData == NULL)
        {
            ASSERT(0);
            return FALSE;
        }
    }
    
#ifdef __MODEL_slt__
    _u4VC1SltLastPicPts = 0;
#endif

    return TRUE;
}

VOID _SWDMX_WMVSetRange(UINT8 u1SrcId, SWDMX_RANGE_LIST_T *prRangeList, UINT32 u4VidStrmID,
                        UINT32 u4AudStrmID, UINT32 u4SubTitleID)
{
    UINT64 u8Remainder;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_WMV_VERIFY_NULL(prSwdmxInst);  
    //UINT16 u2idx =0;
    //MIDXBULD_ELMT_STRM_ENTRY_T* prIdxEntry;

#if defined(CC_3D_MM_DS_SUPPORT)  // hard code to inverse L/R
    if (ENABLE_NEWTRACK(prSwdmxInst))
    {
        u4VidStrmID = 2;
    }
#endif
    
    LOG(5, "_SWDMX_WMV_SET_RANGE\n");

    if (prRangeList != NULL &&
            prRangeList->prDmxRangeInfo != NULL)
    {
        MM_RANGE_ELMT_IDXTBL_T* pBuldIdxTbl =
            (MM_RANGE_ELMT_IDXTBL_T*) prRangeList->prDmxRangeInfo->pt_idxtbl_lst;

        MM_RANGE_INFO_T* rRangeInfo = (prRangeList->prDmxRangeInfo);
        ASSERT(rRangeInfo != NULL);

        _prRangeInfo = rRangeInfo;
        //x_memcpy(&_rRangeInfo,prRangeList->prDmxRangeInfo,sizeof(MM_RANGE_INFO_T));
        //_prRangeInfo = &_rRangeInfo;
        _prRangeList = prRangeList;

        _rSwdmxWmvInfo.u4PreRoll = (UINT32)u8Div6432(prRangeList->prDmxRangeInfo->t_pts_offset, 90, &u8Remainder);
        _rSwdmxWmvInfo.u4FrameRate = 24;  // !!!1015
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
        if (_prRangeInfo->ui4_1st_pkgnum)
        {
            _rSwdmxWmvInfo.u8BeginParseOfst = _prRangeInfo->ui8_fileoffset + WMV_DATA_OBJ_HEADER +
                                       _prRangeInfo->ui4_sz_min_acces_unt * _prRangeInfo->ui4_1st_pkgnum;
            _rSwdmxWmvInfo.u8LastFileParsingPosition = _rSwdmxWmvInfo.u8BeginParseOfst;
        }
        else
#endif
        {
            _rSwdmxWmvInfo.u8BeginParseOfst = 0;
            _rSwdmxWmvInfo.u8LastFileParsingPosition = prRangeList->prDmxRangeInfo->ui8_fileoffset;
        }
        //_rSwdmxWmvInfo.u4PreRoll = (prRangeList->prDmxRangeInfo->t_pts_offset/90);
        _rSwdmxWmvInfo.u8FileSize = prRangeList->prDmxRangeInfo->z_range_sz;

        while (pBuldIdxTbl != NULL)
        {
#ifdef CC_3D_MM_DS_SUPPORT
            if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_KEY && pBuldIdxTbl->t_strm_id.u.t_stm_asf_id == u4VidStrmID && IS_SIMP_IDX(pBuldIdxTbl->u.pt_pb_idx_tbl_entry))
#else
            if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_KEY && pBuldIdxTbl->t_strm_id.u.t_stm_asf_id ==u4VidStrmID)
#endif
            {
                if (_rSwdmxWmvInfo.prWmvKeyIdxTbl ==NULL)
                    _rSwdmxWmvInfo.prWmvKeyIdxTbl = pBuldIdxTbl;
            }
#ifdef CC_3D_MM_DS_SUPPORT
            if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_KEY && IS_NEWTRACKDATA(prSwdmxInst, pBuldIdxTbl->t_strm_id.u.t_stm_asf_id))
            {
                if (_rSwdmxWmvInfo.prWmvKeyIdxTbl2 == NULL)
                    _rSwdmxWmvInfo.prWmvKeyIdxTbl2 = pBuldIdxTbl;
            }
#endif
            pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)pBuldIdxTbl->pv_next_tbl;
        }
#ifdef WMA_TRICK_MODE
        pBuldIdxTbl=prRangeList->prDmxRangeInfo->pt_idxtbl_lst;
        if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES&&pBuldIdxTbl&&(!_rSwdmxWmvInfo.prWmvKeyIdxTbl))
        {
            if(pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_KEY&&(pBuldIdxTbl->t_strm_id.u.t_stm_asf_id==0 || pBuldIdxTbl->t_strm_id.u.t_stm_asf_id==u4AudStrmID))
            {
               _rSwdmxWmvInfo.prWmvKeyIdxTbl = pBuldIdxTbl;
            }
        }
#endif

        
        LOG(0,"pts offset : %d\n",_rSwdmxWmvInfo.u4PreRoll);

        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }
    else
    {
        LOG(0, "Demuxer Range is NULL\n");
    }
}


#ifdef WMV_BAD_INTLV_DEBUG
BOOL _fgShowFifoStatus = 1;
UINT32 _u4LastVFullness = 0;
UINT32 _u4LastAFullness = 0;
UINT32 _u4Last1_10Sec = 0;
void vShowFifoStatus(SWDMX_INFO_T* prSwdmxInst)
{
    UINT32 u4AFullness, u4VFullness;
    UINT32 u4ADuration, u4VDuration;
    UINT16 u2CurQNum, u2MaxQNum;
    UINT32 u41_10Sec;
    
    u4VFullness = _u4VFifoSize - DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, DMX_PID_TYPE_ES_VIDEO, 0);
    u4AFullness = _u4AFifoSize - DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, DMX_PID_TYPE_ES_AUDIO, 1);
    u41_10Sec = STC_GetStcValue(prSwdmxInst->u1StcId) / 9000;
    
    if (_fgShowFifoStatus &&
        /*(u4VFullness != _u4LastVFullness || u4AFullness != _u4LastAFullness)*/
        u41_10Sec != _u4Last1_10Sec)
    {
        VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
        u4VDuration = 1000 * u2CurQNum / 25;
        if (_u4WmaBytesPerSec > 0)
            u4ADuration = 1000 * u4AFullness / _u4WmaBytesPerSec;
        else
            u4ADuration = 0;
        printf("[swmv] %3ds A:V = %d : %d -%df (%d ms : %d ms)\n", u41_10Sec, u4AFullness, u4VFullness, u2CurQNum, u4ADuration, u4VDuration);
    }
    _u4LastVFullness = u4VFullness;
    _u4LastAFullness = u4AFullness;
    _u4Last1_10Sec = u41_10Sec;
}
#endif  // WMV_BAD_INTLV_DEBUG


#ifdef CC_3D_MM_DS_SUPPORT
static VOID _WMVChkResume1XPos(SWDMX_INFO_T* prSwdmxInst)
{
    UINT32 i;
    UINT32 u4NumberOfEntry;
    MIDXBULD_ELMT_STRM_ENTRY_T *prKeyidxEntry = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T *prKeyidxEntry2 = NULL;
    
    if (_rSwdmxWmvInfo.prWmvKeyIdxTbl != NULL)
    {
        u4NumberOfEntry = _rSwdmxWmvInfo.prWmvKeyIdxTbl->ui4_number_of_entry;
    }
    else
    {
        LOG(0, "No key table for L track!!!\n");
        ASSERT(0);
        return;
    }

    if (_rSwdmxWmvInfo.prWmvKeyIdxTbl2 == NULL)
    {
        LOG(0, "No key table for R track!!!\n");
        ASSERT(0);
        return;
    }

#if 0
    i = _rSwdmxWmvInfo.u4CurKeyIdx;
    //if (i >= 1)
    //    i--;
#else
    i = 0;
#endif
    for ( ; i < u4NumberOfEntry; i++)
    {
        prKeyidxEntry = (MIDXBULD_ELMT_STRM_ENTRY_T*)(_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry + i);
        if (prKeyidxEntry->ui4_relative_offset == _rSwdmxWmvInfo.u4CurParsingPacketNo * _rSwdmxWmvInfo.u4PacketSize)
            break;
    }
    
    if (i < u4NumberOfEntry)
    {
        LOG(3, "!!!!!!!!!!!!!!!!!!!!!!!!!!! Old resume pkt #: %d\n", _rSwdmxWmvInfo.u4CurParsingPacketNo);
        prKeyidxEntry2 = (MIDXBULD_ELMT_STRM_ENTRY_T*)(_rSwdmxWmvInfo.prWmvKeyIdxTbl2->u.pt_pb_idx_tbl_entry + i);
        _rSwdmxWmvInfo.u4CurParsingPacketNo = min(prKeyidxEntry->ui4_relative_offset / _rSwdmxWmvInfo.u4PacketSize, prKeyidxEntry2->ui4_relative_offset / _rSwdmxWmvInfo.u4PacketSize);
        LOG(3, "!!!!!!!!!!!!!!!!!!!!!!!!!!! New resume pkt #: %d\n", _rSwdmxWmvInfo.u4CurParsingPacketNo);
    }
}
#endif  // CC_3D_MM_DS_SUPPORT


#if 1//def WMV_SEEK_OFFSET
SWDMX_HDR_PRS_STATE_T _SWDMX_WMVHeaderParse(
    UINT8 u1SrcId,
    SWDMX_RANGE_LIST_T* prRangeList,
    SWDMX_CUR_CMD_T* prCmd)
#else
SWDMX_HDR_PRS_STATE_T _SWDMX_WMVHeaderParse(
    UINT8 u1SrcId,
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange)
#endif
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    UINT64 u8SeekPos = prCmd->u8SeekPos;
    INT32 i4PlaySpeed = prCmd->i4PlaySpeed;
    SWDMX_PLAY_MODE_T ePlayMode=prCmd->ePlayMode;
    BOOL fgSpeedChange = prCmd->fgChange;

    LOG(9, "_SWDMX_WMV_HEADER_PARSE\n");

#ifdef WMV_BAD_INTLV_DEBUG
    vShowFifoStatus(prSwdmxInst);
#endif

    if (!prSwdmxInst)
    {
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }

    // megaa 20100321 tmp solution
    if (!_rSwdmxWmvInfo.fgInternalInited)
    {
        _WMVMsgQInit(u1SrcId);
        _WMVInternalInit(u1SrcId);
    }
    
    prVdecEsInfo = _VDEC_GetEsInfo(prSwdmxInst->u1VdecId);
    _rSwdmxWmvInfo.fgPlayAbort=FALSE;
    
    if (_rSwdmxWmvInfo.fgBeginPlay && i4PlaySpeed == 1000)
    {
        if(u1SrcId<SWDMX_SOURCE_MAX)
        {
            u4AudDspPts[u1SrcId]=1;
        }
        //VDP_TriggerAudReceive(0);
        if (_rSwdmxWmvInfo.fgEnAudio)//protect func
        {
            AUD_SetStartPts(prSwdmxInst->u1AdecId,0);
        }

        if (_rSwdmxWmvInfo.fgEnVideo)//protect func
        {
            VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId,0); //maiyou
            if(ENABLE_NEWTRACK(prSwdmxInst))
            {
                VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id,0); 
            }
        }

        if (_rSwdmxWmvInfo.fgEnVideo  // make sure that video is prior than audio
            && _rSwdmxWmvInfo.eVideoType != ENUM_SWDMX_VDEC_UNKNOWN)  // DTV00079781
        {
            _rSwdmxWmvInfo.u4TriggerAudioPTS = 0xF0000000;
            _rSwdmxWmvInfo.u4DisplayPTS = 0xF0000000;
        }
        else
        {
            _rSwdmxWmvInfo.u4TriggerAudioPTS = 0x00000000;
            _rSwdmxWmvInfo.u4DisplayPTS = 0x00000000;
        }

#ifdef WMV_BAD_INTLV_SUPPORT
        _rBadIntlv.fgEverEntered = FALSE;
#endif

        _rSwdmxWmvInfo.fgBeginPlay = FALSE;

        if (_rSwdmxWmvInfo.fgEnAudio)//protect flag
        {
            _rSwdmxWmvInfo.fgAudioNormalPlay = TRUE;
        }
        if (_rSwdmxWmvInfo.fgEnVideo)//protect flag
        {
            _rSwdmxWmvInfo.fgVideoNormalPlay = TRUE;
        }
        //_fgGotVideoFrame = FALSE;
        //_fgVideoPause = FALSE;
#ifdef WAIT_AUDIO_READY
        if (!_rSwdmxWmvInfo.fgVideoPause && _rSwdmxWmvInfo.fgEnVideo && (
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
            prSwdmxInst->fgIsBadInterleaved ||
#endif
            _rSwdmxWmvInfo.fgEnAudio))
        {
            VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
            VDEC_Pause(prSwdmxInst->u1VdecId);//maiyou
            if(ENABLE_NEWTRACK(prSwdmxInst))
            {
                VDEC_Pause(prSwdmxInst->u1Vdec2Id);
            }
            _rSwdmxWmvInfo.fgVideoPause = TRUE;
        }
#endif
#ifdef WMA_SEEK_OFFSET
        _rSwdmxWmvInfo.fgSeekingNow=FALSE;
        _rSwdmxWmvInfo.u4WMASeekTimes=0;
        _rSwdmxWmvInfo.u8LastSeekPos=0;
#endif
        STC_StopStc(prSwdmxInst->u1StcId);
        if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
        {
            //UINT32 u4CurPts=0;
            STC_SetStcValue(prSwdmxInst->u1StcId, (UINT32)(0));
            //AUD_MMQueueSyncInfo(prSwdmxInst->u1AdecId,0,0,TRUE);
            AUD_ClearEOSFlag(prSwdmxInst->u1AdecId);
            //AUD_GetAudioTrickPts(255,&u4CurPts,TRUE);
        }
        else
        {
            STC_SetStcValue(prSwdmxInst->u1StcId, (UINT32)(0-3000));
        }
		
        if(prSwdmxInst->rCurRangeReq.fgSetRenderPosition == TRUE 
           ||_prRangeInfo->t_vid_start_render_pts>90000)
        {
            if(_prRangeInfo->t_vid_start_render_pts>90000)
            {
                if(_rSwdmxWmvInfo.prWmvKeyIdxTbl)
                {
                    UINT64 u8Remainder=0;
                    fgSpeedChange = TRUE;
                    u8SeekPos = _prRangeInfo->t_vid_start_render_pts;
                    _prRangeInfo->t_vid_start_render_pts = 0;
                    _rSwdmxWmvInfo.fgFirstFrame = FALSE;
                    _rSwdmxWmvInfo.u4TotalPacketNo = (UINT32)u8Div6432((_rSwdmxWmvInfo.u8FileSize-WMV_DATA_OBJ_HEADER),
					   _prRangeInfo->ui4_sz_min_acces_unt, &u8Remainder);
                    if (_rSwdmxWmvInfo.u4TotalPacketNo == 0)  
                    {
                        _rSwdmxWmvInfo.u4TotalPacketNo = 1; 
                    }
                    _rSwdmxWmvInfo.u4PacketSize = _prRangeInfo->ui4_sz_min_acces_unt;
                }
                else
                {
                   LOG(0,"No index table,can't start from 0x%x\n",_prRangeInfo->t_vid_start_render_pts);
                }
            }
            else
            {
			    fgSpeedChange = TRUE;
                _rSwdmxWmvInfo.fgSeekingNow=TRUE;
				u8SeekPos=prSwdmxInst->rCurRangeReq.u8StartPts;
				prSwdmxInst->rCurRangeReq.u8StartAudPts=u8SeekPos;
            }
        }
    }

    if (i4PlaySpeed == 1000 && (prSwdmxInst->ePrePlayMode==SWDMX_PLAY_MODE_IFRAME))
    {
        _rSwdmxWmvInfo.fgAudioNormalPlay = TRUE;
        //_rSwdmxWmvInfo.fgVideoNormalPlay = TRUE;
#ifdef WMV_2X_TO_1X_PREBUFFER
        UINT8 u1Idx = 0;
        UINT32 u4APts = 0;
        for ( ;(u1Idx < 1; u1Idx++)
        {
            DMX_MM_FlushBuffer(_rSwdmxWmvInfo.u1AudPreBufPid[u1Idx]);
        }
#endif
    }
    if (i4PlaySpeed == 1000&& (_rSwdmxWmvInfo.i4WMVPlaySpeed != 1000 && prSwdmxInst->ePrePlayMode==SWDMX_PLAY_MODE_NORMAL))
    {
#ifndef WMV_2X_TO_1X_PREBUFFER
        _rSwdmxWmvInfo.fgAudioNormalPlay = TRUE;
        //_rSwdmxWmvInfo.fgVideoNormalPlay = TRUE;
#endif
    }

#if 0  // obsolete code (stop resume is now implemented by pause/resume)
    // stop resume handling
    if (prVdecEsInfo->fgFmtDectFinish&& _prRangeInfo->t_vid_start_render_pts >= 90000)
    {
        if(_rSwdmxWmvInfo.prWmvKeyIdxTbl)
        {
            fgSpeedChange = TRUE;
            u8SeekPos = _prRangeInfo->t_vid_start_render_pts;
            _prRangeInfo->t_vid_start_render_pts = 0;
        }
        else
        {
           LOG(0,"No index table,can't start from 0x%x\n",_prRangeInfo->t_vid_start_render_pts);
        }
    }
#else
    UNUSED(prVdecEsInfo);
#endif

    //handle speed change
    if (   fgSpeedChange
        || _rSwdmxWmvInfo.fgChangeAudio
#ifdef WMA_SEEK_OFFSET
        || _rSwdmxWmvInfo.fgSeekingNow
#endif
        )
    {
        UINT16 u2DecodingOrder;
        INT32 i4TempRef;
        UINT64 u8FrameOffset =0;
        UINT64 u8IFrameOffset =0;
        VDEC_ES_INFO_T *prVdecEsInfo;
        
        
        LOG(0, "******************i4PlaySpeed = %d\n",i4PlaySpeed);
        
        prVdecEsInfo = _VDEC_GetEsInfo((UCHAR)prSwdmxInst->u1VdecId); //maiyou

        if (_rSwdmxWmvInfo.fgEnVideo)//protect func
        {
            VDP_GetFilePosition(prSwdmxInst->u1B2rId, &i4TempRef, &u2DecodingOrder,
                                &u8FrameOffset, &u8IFrameOffset);
        }
        _rSwdmxWmvInfo.fgFinishKeyTable = FALSE;

        LOG(6,"speed %d: %d \n",_rSwdmxWmvInfo.i4WMVPlaySpeed,i4PlaySpeed);
#ifdef WMA_SEEK_OFFSET
        if(fgSpeedChange || _rSwdmxWmvInfo.fgChangeAudio)
        {
            _rSwdmxWmvInfo.fgSeekingNow=FALSE;
            _rSwdmxWmvInfo.u4WMASeekTimes=0;
            _rSwdmxWmvInfo.u8LastSeekPos=0;
        }
#endif
        if (_rSwdmxWmvInfo.fgChangeAudio)
        {
            _rSwdmxWmvInfo.fgChangeAudio = FALSE;
            _rSwdmxWmvInfo.u4AudioCount = 0;
            _rSwdmxWmvInfo.u4WmaErrorCode = 0;
            _rSwdmxWmvInfo.fgFirstAudioError = FALSE;
            if (_rSwdmxWmvInfo.fgEnAudio)//DTV00213648
            {
                AUD_SetStartPts(prSwdmxInst->u1AdecId,_rSwdmxWmvInfo.u4CurPTS);
            }
        }

        if (((UINT32)(u8SeekPos>>32) != (UINT32)0xFFFFFFFF)
                || ((UINT32)(u8SeekPos) != (UINT32)0xFFFFFFFF)
#ifdef WMV_SEEK_OFFSET
            ||(prCmd->u4SeekMode == SWDMX_CUR_CMD_SEEK_OFFSET)
#endif
#ifdef WMA_SEEK_OFFSET
            ||(_rSwdmxWmvInfo.fgSeekingNow)
#endif
        )
        {
            #ifdef WMV_SEEK_OFFSET
            if(prCmd->u4SeekMode == SWDMX_CUR_CMD_SEEK_OFFSET)
            {
                if (_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
                {
                    u8SeekPos = prSwdmxInst->rSeekOffset.ui8_aud_pts_info;
                }
                else
                {
                    u8SeekPos = prSwdmxInst->rSeekOffset.ui8_pts_info;
                }
            }
            #endif
#ifdef WMA_SEEK_OFFSET
            if(!_rSwdmxWmvInfo.fgSeekingNow)
            {
                _rSwdmxWmvInfo.u8SeekPTS = u8SeekPos;
                _rSwdmxWmvInfo.u4DisplayPTS = (UINT32)u8SeekPos;
				if(i4PlaySpeed==1000)
				{
					_rSwdmxWmvInfo.fgAudioNormalPlay = TRUE;
				}
                //_rSwdmxWmvInfo.fgVideoNormalPlay = TRUE;
                _rSwdmxWmvInfo.u2SendPayloadNo = 0;
                _rSwdmxWmvInfo.u2PreviousSendPayloadNo =0;
                _rSwdmxWmvInfo.fgBlock = FALSE;
            }
#else
            _rSwdmxWmvInfo.u8SeekPTS = u8SeekPos;
            _rSwdmxWmvInfo.u4DisplayPTS = (UINT32)u8SeekPos;
			if(_eCurFMT != SWDMX_FMT_WMA_AUDIO_ES || i4PlaySpeed>0)
			{
				_rSwdmxWmvInfo.fgAudioNormalPlay = TRUE;
			}
            //_rSwdmxWmvInfo.fgVideoNormalPlay = TRUE;
            _rSwdmxWmvInfo.u2SendPayloadNo = 0;
            _rSwdmxWmvInfo.u2PreviousSendPayloadNo =0;
            _rSwdmxWmvInfo.fgBlock = FALSE;
#endif
            if (_rSwdmxWmvInfo.fgEnVideo)  // megaa: remove the unnecessary "_rSwdmxWmvInfo.i4WMVPlaySpeed != 1000" condition for DTV00095857
            {
                if (_SWDMX_WMVInitDmx(prSwdmxInst, eSWDMX_MEDIA_VIDEO,prSwdmxInst->u1VdecId,TRUE, FALSE) == FALSE)
                {
                    LOG(0,"trick mode set search start code fail\n");
                }

                if(ENABLE_NEWTRACK(prSwdmxInst))
                {
                    if (_SWDMX_WMVInitDmx(prSwdmxInst, eSWDMX_MEDIA_VIDEO,prSwdmxInst->u1Vdec2Id,TRUE, FALSE) == FALSE)
                    {
                        LOG(0,"trick mode set search start code fail\n");
                    }
                }
            }
			
            if (_rSwdmxWmvInfo.fgEnVideo)//protect func
            {
                VDP_GetFilePosition(prSwdmxInst->u1B2rId, &i4TempRef, &u2DecodingOrder,
                                    &u8FrameOffset, &u8IFrameOffset);
            }
            
            if (_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
            {
#ifdef WMA_SEEK_OFFSET
                if(_rSwdmxWmvInfo.u4WMASeekTimes<WMA_MAX_SEEKTIME)
                {
                   if(_SWDMX_WMASeekOffset(prSwdmxInst)<0)
                   {
                     return eSWDMX_HDR_PRS_SUCCEED;
                   }
                }
                else
                {
                   _rSwdmxWmvInfo.fgSeekingNow=FALSE;
                   return eSWDMX_HDR_PRS_SUCCEED;
                }
#else
                _SWDMX_WMVComsumeFeeder(prSwdmxInst);
                if(_rSwdmxWmvInfo.prWmvKeyIdxTbl)
                {
					_SWDMX_WMVSeek(prSwdmxInst, u8SeekPos, prRangeList);
					 if(i4PlaySpeed<0)
					 {
						 fgTrickTrigger[u1SrcId]=TRUE;
					 }
                }
				else
				{
					_rSwdmxWmvInfo.u8LastFileParsingPosition = _prRangeInfo->ui8_fileoffset + WMV_DATA_OBJ_HEADER;
					_rSwdmxWmvInfo.u4CurParsingPacketNo = 0;
				}
#endif
            }
#ifdef WMV_SEEK_OFFSET
            else if((prCmd->u4SeekMode == SWDMX_CUR_CMD_SEEK_OFFSET)||_prRangeList->fgSetRenderPosition)
            {
                _SWDMX_WMVSeekOffset(prSwdmxInst,FALSE);
            }
#endif
            else
            {
                _SWDMX_WMVSeek(prSwdmxInst, u8SeekPos, prRangeList);
            }
            //to be done , speed up searching time

            if ((i4PlaySpeed >0)&&(_rSwdmxWmvInfo.fgEnVideo))
            {
            	VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId,(UINT32)(_rSwdmxWmvInfo.u8SeekPTS+3000)); //after, 2011/03/13, from isaac's mail:2011/3/2 pm11:05
//              VDEC_SetRenderPts(prSwdmxInst->u1VdecId,(UINT32)(_rSwdmxWmvInfo.u8SeekPTS+3000)); //maiyou //original, 2011/03/13
                if(ENABLE_NEWTRACK(prSwdmxInst))
                {
                	VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id,(UINT32)(_rSwdmxWmvInfo.u8SeekPTS+3000)); //after, 2011/03/13, from isaac's mail:2011/3/2 pm11:05
//                    VDEC_SetRenderPts(prSwdmxInst->u1Vdec2Id,(UINT32)(_rSwdmxWmvInfo.u8SeekPTS+3000)); //original, 2011/03/13
                }
            }
            else if (_rSwdmxWmvInfo.fgEnVideo)
            {
            	VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId,0); //after, 2011/03/13, from isaac's mail:2011/3/2 pm11:05
//                VDEC_SetRenderPts(prSwdmxInst->u1VdecId,0); //original, 2011/03/13
                if(ENABLE_NEWTRACK(prSwdmxInst))
                {
                	VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id,0); //after, 2011/03/13, from isaac's mail:2011/3/2 pm11:05
//                    VDEC_SetRenderPts(prSwdmxInst->u1Vdec2Id,0); //original, 2011/03/13
                }
            }

            if (_rSwdmxWmvInfo.fgEnVideo)
            {
                MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
                VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);
                DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1VidPid);
                if(ENABLE_NEWTRACK(prSwdmxInst))
                {
                    MPV_FlushEsmQ(prSwdmxInst->u1Vdec2Id, TRUE);
                    VDEC_ReleaseDispQ(prSwdmxInst->u1Vdec2Id);
                    DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1Vid2Pid);
                }
            }
            if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
            {
                DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid);
                AUD_ClearEOSFlag(prSwdmxInst->u1AdecId);
				u4AudDspPts[u1SrcId]=_rSwdmxWmvInfo.u8SeekPTS;
            }
            else if(_rSwdmxWmvInfo.fgEnAudio)
            {
            
               DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid);
            }
            _SWDMX_WMVFlushFeederDataQ(prSwdmxInst);
            _rSwdmxWmvInfo.fgFirstReqFeederData = TRUE;
            _rSwdmxWmvInfo.fgDemuxFinish = FALSE;
            _rSwdmxWmvInfo.u4ConsumedSize = 0;
            _rSwdmxWmvInfo.fgBrokenFile = FALSE;

            _fgReceiveVideoPes = FALSE;
            _fgReceiveAudioPes = FALSE;

#ifdef WMV_BAD_INTLV_SUPPORT
            _WMVResetBadIntlv(prSwdmxInst);
#endif

            if (_rSwdmxWmvInfo.fgEnVideo)//protect func
            {
                VDEC_StartPTS(prSwdmxInst->u1VdecId,TRUE,(UINT32)_rSwdmxWmvInfo.u8SeekPTS,NULL);
                if(ENABLE_NEWTRACK(prSwdmxInst))
                {
                    VDEC_StartPTS(prSwdmxInst->u1Vdec2Id,TRUE,(UINT32)_rSwdmxWmvInfo.u8SeekPTS,NULL);
                }
            }

#ifdef WAIT_AUDIO_READY
            if (!_rSwdmxWmvInfo.fgVideoPause && i4PlaySpeed !=1 && _rSwdmxWmvInfo.fgEnVideo && _rSwdmxWmvInfo.fgEnAudio)
            {
                VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
                VDEC_Pause(prSwdmxInst->u1VdecId);
                if(ENABLE_NEWTRACK(prSwdmxInst))
                {
                    VDEC_Pause(prSwdmxInst->u1Vdec2Id);
                }
                _rSwdmxWmvInfo.fgVideoPause = TRUE;
            }
#endif
            STC_StopStc(prSwdmxInst->u1StcId);
            STC_SetStcValue(prSwdmxInst->u1StcId, (UINT32)_rSwdmxWmvInfo.u8SeekPTS);
#ifdef WMV_2X_TO_1X_PREBUFFER
            UINT8 u1Idx = 0;
            UINT32 u4APts = 0;
            
            for ( ;(u1Idx < 1; u1Idx++)
		        {
		            DMX_MM_FlushBuffer(_rSwdmxWmvInfo.u1AudPreBufPid[u1Idx]);
		        }
#endif
            _rSwdmxWmvInfo.u4DisplayPTS=_rSwdmxWmvInfo.u8SeekPTS;
            _rSwdmxWmvInfo.u8SeekPTS = 0xFFFFFFFF;
            _rSwdmxWmvInfo.u8SeekPTS <<= 32;
            _rSwdmxWmvInfo.u8SeekPTS += 0xFFFFFFFF;

        }
        // 1x->2x
        else if ((_rSwdmxWmvInfo.i4WMVPlaySpeed>1 && _rSwdmxWmvInfo.i4WMVPlaySpeed <= 1000)&&
                 ((i4PlaySpeed!=1000)&&(ePlayMode==SWDMX_PLAY_MODE_NORMAL)))
        {
            //do nothing
            if (_rSwdmxWmvInfo.fgVideoPause && i4PlaySpeed==1)
            {
                _WMVStartToPlay(prSwdmxInst);
            }

#ifdef WMV_2X_TO_1X_PREBUFFER
            //if (INPUT_SRC != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
            {
                // Set active pid to 255
                DMX_AudHandler_SetActivePidx(DMX_AUDIO_HANDLER_NULL_PIDX);
            }
            DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid);
            _rSwdmxWmvInfo.fg2XAudParsing = TRUE;
#endif
        }
#ifdef WMV_2X_TO_1X_PREBUFFER
        // 2x->1x (step? slow forward?)
        //else if (_rSwdmxWmvInfo.i4WMVPlaySpeed >= 2000 && _rSwdmxWmvInfo.i4WMVPlaySpeed <= 2000 && i4PlaySpeed == 1000)
        else if (_rSwdmxWmvInfo.fg2XAudParsing && i4PlaySpeed == 1000)
        {
            //_fgMegaaTest = TRUE;
#if 1
            UINT32 u4CurPts;

            u4CurPts = VDP_GetPts(prSwdmxInst->u1B2rId);
            _rSwdmxWmvInfo.u4DisplayPTS= u4CurPts;
            _rSwdmxWmvInfo.u4TriggerAudioPTS = u4CurPts;

            if (prVdecEsInfo != NULL)
            {
                //reset seek operation flag when speed change
                if (prVdecEsInfo->fgRenderPts)
                {
                    prVdecEsInfo->fgRenderPts = FALSE;
                }
            }
#endif
            //_fgReceiveVideoPes = FALSE;  !!!
            //_fgReceiveAudioPes = FALSE;  !!!

            if (_rSwdmxWmvInfo.i4WMVPlaySpeed != 2000)  // step => 1X
            {
                STC_StopStc(prSwdmxInst->u1StcId);
                STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts + _i4WmvStcAdj);
                STC_StartStc(prSwdmxInst->u1StcId);
            }

            UINT32 u4APts;
            BOOL fgRet;
            fgRet = DMX_AudHandler_SeekAndUpdate(_rSwdmxWmvInfo.u1AudPreBufPid[0],
                    _rSwdmxWmvInfo.u4TriggerAudioPTS, &u4APts);

            if (fgRet)
            {
                LOG(5, "Find audio in the pre-buffer\n");
                _rSwdmxWmvInfo.u4TriggerAudioPTS = u4APts;
            }
            else
            {
                LOG(5, "Can't find audio in the pre-buffer\n");
                ASSERT(0);  // !!!
                //_rSwdmxWmvInfo.u4TriggerAudPts = _rSwdmxAviInfo.u4AudPts[_rSwdmxAviInfo.u1CurATrackIdx];
            }

            AUD_SetStartPts(prSwdmxInst->u1AdecId, _rSwdmxWmvInfo.u4TriggerAudioPTS);
            DMX_AudHandler_SetActivePidx(_rSwdmxWmvInfo.u1AudPreBufPid[0]);  // !!!

            LOG(5, "_rSwdmxWmvInfo.u4TriggerAudioPTS = 0x%08x\n", _rSwdmxWmvInfo.u4TriggerAudioPTS);
            _rSwdmxWmvInfo.fg2XAudParsing = FALSE;
        }
#endif  // WMV_2X_TO_1X_PREBUFFER
#ifdef WMA_TRICK_MODE
        else if((_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)&&(i4PlaySpeed >= 2000)&&(_rSwdmxWmvInfo.i4WMVPlaySpeed>0))
        {
            //nothing to do
        }
#endif
        else if(fgSpeedChange)
        {
            UINT32 u4CurPts=0;

            if (_eCurFMT==SWDMX_FMT_WMA_AUDIO_ES && _rSwdmxWmvInfo.prWmvKeyIdxTbl == NULL)
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
#ifdef WMA_TRICK_MODE
            if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
            {
                if((_rSwdmxWmvInfo.i4WMVPlaySpeed!=1000&&i4PlaySpeed==1000)||(i4PlaySpeed<0))
                {
                    UINT32 u4DelayTime=0;
                    while(u4DelayTime<20&&AUD_IsDecoderStop(prSwdmxInst->u1AdecId))
                    {
                       x_thread_delay(50);
                       u4DelayTime++;
                    }
                    if(u4DelayTime!=20)
                    {
                        //AUD_MMQueueSyncInfo(prSwdmxInst->u1AdecId,0,0,TRUE);
                    }
                    else
                    {
                        LOG(0,"Speed change, But audio is stop now\n");
                    }
                    AUD_GetAudioTrickPts(prSwdmxInst->u1AdecId, &u4CurPts, TRUE);
                    if(u4CurPts==0 &&AUD_IsDecoderRealPlay(prSwdmxInst->u1AdecId))
                    {
                        AUD_GetAudioTrickPts(prSwdmxInst->u1AdecId, &u4CurPts, FALSE);
                    }
                }
                else if(AUD_IsDecoderRealPlay(prSwdmxInst->u1AdecId))
                {
                    AUD_GetAudioTrickPts(prSwdmxInst->u1AdecId, &u4CurPts, FALSE);
                }
                
                if(u4CurPts==0)
                {
                
                    LOG(0,"First CurPts iz 0\n");
                    u4CurPts=STC_GetStcValue(prSwdmxInst->u1StcId);
                    LOG(0,"STC Pts iz (%d,%d)\n",u4CurPts,u4CurPts/90000);
                }

                if(u1SrcId<SWDMX_SOURCE_MAX)
                {
                    if(i4PlaySpeed<0)
                    { 
                        if(u4CurPts==0||u4CurPts>_rSwdmxWmvInfo.prRangeInfo->ui8_pb_duration)
                        {
                            u4CurPts=u4AudDspPts[u1SrcId];
                        }
                        STC_StopStc(prSwdmxInst->u1StcId);
                        STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);
                        u4AudDspPts[u1SrcId]=u4CurPts;
                        fgTrickTrigger[u1SrcId]=TRUE;
                    }
                    else
                    {  
                       u4AudDspPts[u1SrcId]=1;                    
                    }
                }

                //AUD_GetDspPtsInfo(prSwdmxInst->u1AdecId, &u4CurPts, &u4PrsPos);
                LOG(2,"First CurPts: (%d,%d)\n",u4CurPts,u4CurPts/90000);
            }
            else
#endif
            {
                u4CurPts = VDP_GetPts(prSwdmxInst->u1B2rId);
            }
            _rSwdmxWmvInfo.u4DisplayPTS= u4CurPts;

            //MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
            //VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);
            if (prVdecEsInfo != NULL)
            {
                //reset seek operation flag when speed change
                if (prVdecEsInfo->fgRenderPts)
                {
                    prVdecEsInfo->fgRenderPts = FALSE;
                }
            }
#ifdef WMA_TRICK_MODE
            if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
            {
                _rSwdmxWmvInfo.fgFinishKeyTable=FALSE;
                _rSwdmxWmvInfo.u4IndexPts=0;
				_rSwdmxWmvInfo.u2SendPayloadNo = 0;
                _rSwdmxWmvInfo.u2PreviousSendPayloadNo =0;
                _rSwdmxWmvInfo.fgBlock = FALSE;
                DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid);
            }
            else
#endif
            {
                DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1VidPid);
                if(ENABLE_NEWTRACK(prSwdmxInst))
                 {
                  DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1Vid2Pid);
                 }
                DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid);
            }
#ifdef WMV_2X_TO_1X_PREBUFFER
            _rSwdmxWmvInfo.fg2XAudParsing = FALSE;
#endif

            _SWDMX_FlushReqMsgQ(u1SrcId);
            _SWDMX_WMVFlushFeederDataQ(prSwdmxInst);
            _rSwdmxWmvInfo.fgFirstReqFeederData = TRUE;
            _rSwdmxWmvInfo.u4ConsumedSize = 0;
            _fgReceiveVideoPes = FALSE;
            _fgReceiveAudioPes = FALSE;
            _u4PrevFramePTS=0;;
            //VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u4CurPts);
            LOG(0,"VDEC_SetRenderFromPts : %x\n",u4CurPts);

#ifdef WMV_BAD_INTLV_SUPPORT
            _WMVResetBadIntlv(prSwdmxInst);
#endif

            // Things for 32/16/8/4 -> 1X are the same
            if ((_rSwdmxWmvInfo.i4WMVPlaySpeed >= 2000 || _rSwdmxWmvInfo.i4WMVPlaySpeed < 0
                    || (_rSwdmxWmvInfo.i4WMVPlaySpeed >=1 && _rSwdmxWmvInfo.i4WMVPlaySpeed<1000))
                    && (i4PlaySpeed == 1000 || i4PlaySpeed==2000 || i4PlaySpeed==500))
            {
                //UINT32 u4RemainedDataSize;
#ifdef WMA_TRICK_MODE
                if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES &&( (_rSwdmxWmvInfo.i4WMVPlaySpeed<0 || _rSwdmxWmvInfo.i4WMVPlaySpeed>=2000)&&(i4PlaySpeed==1000 || i4PlaySpeed==2000)))
                {
                    _SWDMX_WMVSeek(prSwdmxInst, u4CurPts, prRangeList);
                    LOG(2,"FilePos=%d,PacketNumber=%d\n",_rSwdmxWmvInfo.u8LastFileParsingPosition ,_rSwdmxWmvInfo.u4CurParsingPacketNo);
                    _rSwdmxWmvInfo.fgFinishKeyTable=FALSE;
                    _rSwdmxWmvInfo.u2SendPayloadNo = 0;
                    _rSwdmxWmvInfo.u2PreviousSendPayloadNo =0;
                    _rSwdmxWmvInfo.fgBlock = FALSE;
                    if(u1SrcId<SWDMX_SOURCE_MAX)
                    {
                        u4AudDspPts[u1SrcId]=1;
                    }
                    _SWDMX_WMVComsumeFeeder(prSwdmxInst);
                    STC_StopStc(prSwdmxInst->u1StcId);
                    STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);
                }
                else
#endif
                {
                    VDP_GetFilePosition(prSwdmxInst->u1B2rId, &i4TempRef, &u2DecodingOrder,
                                    &u8FrameOffset, &u8IFrameOffset);
                    _rSwdmxWmvInfo.u2SendPayloadNo = 0;
                    _rSwdmxWmvInfo.u2PreviousSendPayloadNo =0;
                    _rSwdmxWmvInfo.fgBlock = FALSE;
                    _rSwdmxWmvInfo.u4CurParsingPacketNo = (UINT32)u8IFrameOffset;
#ifdef CC_3D_MM_DS_SUPPORT
                    if (ENABLE_NEWTRACK(prSwdmxInst))
                    {
                        _WMVChkResume1XPos(prSwdmxInst);
                    }
#endif
                    _rSwdmxWmvInfo.u8LastFileParsingPosition = _prRangeInfo->ui8_fileoffset + WMV_DATA_OBJ_HEADER +
                    _rSwdmxWmvInfo.u4CurParsingPacketNo*_rSwdmxWmvInfo.u4PacketSize;

                    //u4RemainedDataSize = _rSwdmxWmvInfo.u4DataObjSize-
                    //(_rSwdmxWmvInfo.u8LastFileParsingPosition-prRangeList->prDmxRangeInfo->ui8_fileoffset);
                    _SWDMX_WMVComsumeFeeder(prSwdmxInst);
 
                    //since pts is not precious, jump to next 2 frame to avoid frame flicking
                    if (_rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_VC1 && _rSwdmxWmvInfo.i4WMVPlaySpeed ==2000)
                    {
                        u4CurPts += 6000;
                    }
                
                    if((_rSwdmxWmvInfo.i4WMVPlaySpeed >=1 && _rSwdmxWmvInfo.i4WMVPlaySpeed<1000)&&(i4PlaySpeed == 1000))
                    {
                        UINT32 u4CurSTC;
                        u4CurSTC = STC_GetStcValue(prSwdmxInst->u1StcId);
                        u4CurPts = max(u4CurPts,u4CurSTC);
                    }
                
                    VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u4CurPts); //maiyou
                    VDEC_StartPTS(prSwdmxInst->u1VdecId,TRUE,u4CurPts,NULL); 
                    if(ENABLE_NEWTRACK(prSwdmxInst))
                    {
                        VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id, u4CurPts); 
                        VDEC_StartPTS(prSwdmxInst->u1Vdec2Id,TRUE,u4CurPts,NULL); 
                    }
                    LOG(3,"vdp get PTS : %d\n",(u4CurPts/90000));
#ifdef WAIT_AUDIO_READY
                    if (!_rSwdmxWmvInfo.fgVideoPause && _rSwdmxWmvInfo.fgEnVideo && _rSwdmxWmvInfo.fgEnAudio)
                    {
                        VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
                        VDEC_Pause(prSwdmxInst->u1VdecId);
                        if(ENABLE_NEWTRACK(prSwdmxInst))
                        {
                            VDEC_Pause(prSwdmxInst->u1Vdec2Id);
                        }
                        _rSwdmxWmvInfo.fgVideoPause = TRUE;
                    }
#endif 
                    STC_StopStc(prSwdmxInst->u1StcId);
                    STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);
                }
            }
            // Things for 32/16/8/4/2X/1X -> -2X/-4X/-8X/-16X/-32X are the same
            else if (_rSwdmxWmvInfo.i4WMVPlaySpeed > 0 && i4PlaySpeed < 0 )
            {
                if(_rSwdmxWmvInfo.prWmvKeyIdxTbl)
                {
					_SWDMX_WMVSeek(prSwdmxInst, u4CurPts, prRangeList);
                }
				else
				{
				   _SWDMX_WMVSeekOffset(prSwdmxInst,TRUE);
				}
				
                #ifdef WMA_TRICK_MODE
                if(_eCurFMT != SWDMX_FMT_WMA_AUDIO_ES)
                #endif
                VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId,0);//maiyou
                if(ENABLE_NEWTRACK(prSwdmxInst))
                {
                    VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id,0);
                }
            }
            //else if( _rSwdmxWmvInfo.i4WMVPlaySpeed >=2000 && i4PlaySpeed>2000)
            else if (i4PlaySpeed>=2000)
            {
                if(_rSwdmxWmvInfo.prWmvKeyIdxTbl)
                {
					_SWDMX_WMVSeek(prSwdmxInst, u4CurPts, prRangeList);
                }
				else
				{
				   _SWDMX_WMVSeekOffset(prSwdmxInst,TRUE);
				}
#ifdef WMA_TRICK_MODE
                if(_eCurFMT != SWDMX_FMT_WMA_AUDIO_ES)
#endif
                {
                    if (_rSwdmxWmvInfo.u4CurParsingPacketNo < u8IFrameOffset)
                    {
                        _rSwdmxWmvInfo.u4CurParsingPacketNo = (UINT32)u8IFrameOffset;
                    }
                    
                    VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u4CurPts);
                    if(ENABLE_NEWTRACK(prSwdmxInst))
                    {
                        VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id, u4CurPts);
                    }
                }
            }
            else if (i4PlaySpeed <=-2000)
            {
            
#ifdef WMA_TRICK_MODE
                if(_rSwdmxWmvInfo.prWmvKeyIdxTbl)
                {
					_SWDMX_WMVSeek(prSwdmxInst, u4CurPts, prRangeList);
                }
				else
				{
				   _SWDMX_WMVSeekOffset(prSwdmxInst,TRUE);
				}
                if(_eCurFMT != SWDMX_FMT_WMA_AUDIO_ES)
#endif
                { 
                  VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, 0);
                  if(ENABLE_NEWTRACK(prSwdmxInst))
                  {
                      VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id, 0);
                  }
                }
            }
            else
            {
                //null operation
            }

        }
        if (!((_rSwdmxWmvInfo.i4WMVPlaySpeed==1000 && (i4PlaySpeed==2000 || i4PlaySpeed==1 ||i4PlaySpeed == 500))
            ||(_rSwdmxWmvInfo.i4WMVPlaySpeed > 1 && _rSwdmxWmvInfo.i4WMVPlaySpeed < 1000 && i4PlaySpeed >1 && i4PlaySpeed < 1000)))
        {
            
            if (_rSwdmxWmvInfo.fgDemuxFinish && _rSwdmxWmvInfo.u4PacketSize != 0)
            {
                if (prVdecEsInfo != NULL)
                {
                    SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
                }
                AUD_ClearEOSFlag(prSwdmxInst->u1AdecId);
                LOG(0,"Clear Eos Flag\n");
                if (_SWDMX_WMVInitDmx(prSwdmxInst, eSWDMX_MEDIA_VIDEO,prSwdmxInst->u1VdecId,TRUE, FALSE) == FALSE)
                {
                    LOG(0,"set dmx error when sending EOS flag\n");
                }
                
                if(ENABLE_NEWTRACK(prSwdmxInst))
                {
                    if (_SWDMX_WMVInitDmx(prSwdmxInst, eSWDMX_MEDIA_VIDEO,prSwdmxInst->u1Vdec2Id,TRUE, FALSE) == FALSE)
                    {
                        LOG(0,"set dmx error when sending EOS flag\n");
                    }
                }
                
                _u4PrevFramePTS = 0;//DTV00085165
                _rSwdmxWmvInfo.fgDemuxFinish = FALSE;
            }
            if (prRangeList->eRangeStats == eSWDMX_RANGE_STATE_PENDING && _rSwdmxWmvInfo.u4PacketSize != 0)
            {
                prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
            }
        }
    }


    if (_rSwdmxWmvInfo.i4WMVPlaySpeed != i4PlaySpeed)
    {
        _rSwdmxWmvInfo.i4WMVPlaySpeed = i4PlaySpeed;
    }
    
    if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
    {
      if((!AUD_IsDecoderPlay(prSwdmxInst->u1AdecId))||AUD_IsDecoderStop(prSwdmxInst->u1AdecId))
      {
         LOG(3,"WMA Wait audio ready\n");
         AUD_ClearEOSFlag(prSwdmxInst->u1AdecId);
         x_thread_delay(50);
         return eSWDMX_HDR_PRS_SUCCEED;
      }

      if((_rSwdmxWmvInfo.i4WMVPlaySpeed!=1000 || _rSwdmxWmvInfo.u4AudioCount==0)
        &&(AUD_DRVGetAudioState(prSwdmxInst->u1AdecId)==AUD_WAIT_PLAY))
      {
         AUD_MM_Set_Avsync_event(prSwdmxInst->u1AdecId);
         x_thread_delay(50);
      }
    }

    
    if (_SWDMX_WMVDeliverData(prSwdmxInst, prRangeList) == FALSE)
    {
        LOG(9, "_SWDMX_WMV_HEADER_PARSE FALSE\n");
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }
    return eSWDMX_HDR_PRS_SUCCEED;
}


/**
*/

BOOL _SWDMX_WMVAbort(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_WMV_VERIFY_NULL_ERROR(prSwdmxInst);
    _rSwdmxWmvInfo.fgPlayAbort=TRUE;
    return TRUE;
}

BOOL _SWDMX_WMVStop(UINT8 u1SrcId)
{
    BOOL fgEnVDmx;
    BOOL fgEnADmx;
    //DMX_MM_T rMM;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_WMV_VERIFY_NULL_ERROR(prSwdmxInst);

    fgEnVDmx = _rSwdmxWmvInfo.fgEnVDmx;
    fgEnADmx = _rSwdmxWmvInfo.fgEnADmx;

    LOG(5, "_SWDMX_WMV_STOP\n");

#if 0  // megaa 20100321
    _prRangeList = NULL;
    _prRangeInfo = NULL;
    //_prWmvGenericInfo = NULL;
#endif
    //_pprStmAttribute = NULL;

#ifdef WMV_ALLOCATE_VFIFO
    if (_u4VFifoStartAddr!= NULL)
    {
        BSP_FreeAlignedDmaMemory((VOID*)_u4VFifoStartAddr);
        _u4VFifoStartAddr = NULL;
    }
#endif
    if (_rSwdmxWmvInfo.fgEnVideo)//protect func
    {
        DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1VidPid);
        if(ENABLE_NEWTRACK(prSwdmxInst))
        {
           DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1Vid2Pid);
        }
    }
    if (_rSwdmxWmvInfo.fgEnAudio)//protect func
    {
        DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid);
    }
    //_SWDMX_WMVFlushFeederDataQ(prSwdmxInst);

    //x_memset(&(_rSwdmxWmvInfo),0,sizeof(SWDMX_WMV_INFO_T));  for multi-instance

#ifdef CC_NETFLIX_CMPB_SUPPORT
    _u4RemainPacketDataSize = 0;
    UNUSED(_pu1RemainPacketData);
#endif
    
    _rSwdmxWmvInfo.fgFirstFrame = TRUE;
    _rSwdmxWmvInfo.fgFirstReqFeederData = TRUE;
    _rSwdmxWmvInfo.fgBeginPlay = TRUE;
    _rSwdmxWmvInfo.u8SeekPTS = 0xFFFFFFFF;
    _rSwdmxWmvInfo.u8SeekPTS <<= 32;
    _rSwdmxWmvInfo.u8SeekPTS += 0xFFFFFFFF;
#if 0  // megaa 20100321
    _rSwdmxWmvInfo.u4AStreamId = 0xff;
    _rSwdmxWmvInfo.u4VStreamId = 0xff;
#endif
    _fgNetflixEos = FALSE;
    _fgDataExhausted = TRUE;
    _u1AudioStreamId = 0xFF;
    _u4PrevFramePTS = 0;
    _u4DefalutReqSize = 0;
    _fgReceiveAudioPes = FALSE;
    _fgReceiveVideoPes = FALSE;

    _SWDMX_WMVFlushFeederDataQ(prSwdmxInst);

#if 0  // for multi-instance, because _pu1VideoFrameData will be cleared in _SWDMX_WMVInit, we have
       // to free it here and let it be re-allocated again otherwise there will be memory leakage
       // ToDo: use a more clever method to avoid allocate/free repeatedly
    if (_pu1VideoFrameData != NULL)
    {
        BSP_FreeAlignedDmaMemory(_pu1VideoFrameData);
        _pu1VideoFrameData = NULL;
    }
#endif

#ifdef ENABLE_WMV_DRM
    _u8SampleId =0;
    _rSwdmxWmvInfo.fgIsDRM = TRUE;
#endif
    if(u1SrcId<SWDMX_SOURCE_MAX)
    {
        u4AudDspPts[u1SrcId]=1;
    }
    _rSwdmxWmvInfo.fgEnVDmx = fgEnVDmx;
    _rSwdmxWmvInfo.fgEnADmx = fgEnADmx;

#ifdef WMV_2X_TO_1X_PREBUFFER
    DMX_MM_T rPid;
    rPid.fgEnable = FALSE;
    int i;
    for (i = 0; i < WMV_MAX_AUDIO_TRACK; i++)  // !!!
    {
        VERIFY(DMX_MM_Set(_rSwdmxWmvInfo.u1AudPreBufPid[i], DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(_rSwdmxWmvInfo.u1AudPreBufPid[i]);
        DMX_MUL_FreePidx(_rSwdmxWmvInfo.u1AudPreBufPid[i]);
        DMX_AudHandler_Free(_rSwdmxWmvInfo.u1AudPreBufPid[i]);
    }
    DMX_AudHandler_SetEnable(FALSE);
    _rSwdmxWmvInfo.fgAlreadySet = FALSE;
#endif

#if 1
    _rSwdmxWmvInfo.fgInternalInited = FALSE;
#else
    VERIFY(x_msg_q_delete(_hWMVFeederDataQue) == OSR_OK);
    _hWMVFeederDataQue = 0;
#endif
    AUD_DrvSetWMAErrFunc(prSwdmxInst->u1AdecId,NULL, NULL);  // megaa 20090828
 
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
    if (prSwdmxInst->fgIsBadInterleaved)
    {
        // send dummy message to unlock the instance waiting if any
        UINT32 u4Msg = 0;
        x_msg_q_send(_hCommBIQ, &u4Msg, sizeof(UINT32), 1);
    }
#endif

    return TRUE;
}

BOOL _SWDMX_WMVSetInfo(UINT8 u1SrcId, UINT8 u1Type,UINT32 u4Para1,UINT32 u4Para2,UINT32 u4Para3)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_WMV_VERIFY_NULL_ERROR(prSwdmxInst);        

    if (u1Type == eSWDMX_SET_VIDEOTYPE)
    {
#if 1  // disable WMV7/8
        if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_WMV7 || u4Para1 == (UINT32)ENUM_SWDMX_VDEC_WMV8)
        {
            return FALSE;
        }
        else
#endif
#if defined(CC_WMV_MPEG1_DISABLE) && defined(CC_WMV_MPEG2_DISABLE)
        if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            return FALSE;
        }
        else
#endif
#if defined(CC_WMV_MPEG4_DISABLE)
            if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MPEG4)
            {
                return FALSE;
            }
            else
#endif
#if defined(CC_WMV_DX311_DISABLE)
                if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_DX311)
                {
                    return FALSE;
                }
                else
#endif
#if defined(CC_WMV_H264_DISABLE)
                    if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_H264)
                    {
                        return FALSE;
                    }
                    else
#endif
#if defined(CC_WMV_VC1_DISABLE)
                        if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_VC1)
                        {
                            return FALSE;
                        }
                        else
#endif
#if defined(CC_WMV_MJPEG_DISABLE)
                            if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MJPEG)
                            {
                                return FALSE;
                            }
                            else
#endif
                            {
                                _rSwdmxWmvInfo.eVideoType =  (ENUM_SWDMX_VDEC_T)u4Para1;
                            }
    }
    else if (u1Type == eSWDMX_SET_AUDIOTYPE)
    {
        _rSwdmxWmvInfo.eAudioType = (ENUM_SWDMX_ADEC_T)u4Para1;
        _u4WmaBytesPerSec = u4Para2;
        //printf("WMV Bytes Per Sec = %d\n", _u4WmaBytesPerSec);
    }
    else if (u1Type == eSWDMX_SET_VIDSEQHEADER)
    {
#if 0
        _rSwdmxWmvInfo.u4SequenceHeader = u4Para1;
        _rSwdmxWmvInfo.u4SequenceHeaderLength = u4Para2;
#else
        x_memcpy((VOID*)(VIRTUAL((UINT32)&_pu1VideoFrameData[0])), (VOID*)VIRTUAL(u4Para1), (u4Para2));
        _rSwdmxWmvInfo.u4SequenceHeader = (UINT32)VIRTUAL((UINT32)&_pu1VideoFrameData[0]);
        _rSwdmxWmvInfo.u4SequenceHeaderLength = u4Para2;
#endif
    }
    else if (u1Type == eSWDMX_SET_NETFLIX_STREAM_ID)
    {
        _u4NetflixVideoStreamId = (UINT32)(u4Para1);
        _u4NetflixAudioStreamId = (UINT32)(u4Para2);
    }
    else if (u1Type == eSWDMX_SET_NETFLIX_EOS)
    {
        _fgNetflixEos = (BOOL)(u4Para1);
    }
    else if (u1Type == eSWDMX_SET_SYSTEMINFO)
    {
        _prWmvGenericInfo = (MINFO_TYPE_GENERIC_T*)(u4Para1);
        _u4NumberOfBitstream = (UINT32)(u4Para2);
        _WMVFindMaxPacketSize(prSwdmxInst);
    }

    return TRUE;
}

BOOL _SWDMX_WMVGetInfo(UINT8 u1SrcId, UINT8 u1Type,UINT32* pu4Para1,UINT32* pu4Para2,UINT32* pu4Para3)
{
    UINT64 *pu8Tmp;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_WMV_VERIFY_NULL_ERROR(prSwdmxInst);        

    LOG(5, "_SWDMX_WMVGetInfo u1Type = %d\n", u1Type);

    if (u1Type == eSWDMX_GET_CURRENT_POSITION)
    {
        if (pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_WMVGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        pu8Tmp = (UINT64*)pu4Para1;
        *pu8Tmp = 0;
        if (_prRangeInfo != NULL)
        {
            *pu8Tmp = (_prRangeInfo->ui8_fileoffset+(_rSwdmxWmvInfo.u4CurParsingPacketNo*_rSwdmxWmvInfo.u4PacketSize)+WMV_DATA_OBJ_HEADER);
        }
        return TRUE;
    }
    else if (u1Type == eSWDMX_GET_IS_TRICK)
    {
        if (_rSwdmxWmvInfo.prWmvKeyIdxTbl != NULL)
        {
            if (_rSwdmxWmvInfo.prWmvKeyIdxTbl->ui4_number_of_entry < WMV_KEY_TABLE_THRESHOLD)
            {
                *pu4Para1 = FALSE;
                if (pu4Para2 != NULL)
                {
                    *pu4Para2 = FALSE;
                }
            }
            else
            {
                *pu4Para1 = TRUE;
                if (pu4Para2 != NULL)
                {
                    *pu4Para2 = TRUE;
                }
            }
        }
        else
        {
            if(pu4Para1)
            {
                *pu4Para1 = FALSE;
            }
            
            if (pu4Para2 != NULL)
            {
                *pu4Para2 = FALSE;
            }
        }
    }
    else if (u1Type == eSWDMX_GET_CURRENT_TIME)
    {
        if (pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_WMVGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }

        *pu4Para1 = STC_GetStcValue(prSwdmxInst->u1StcId);
        
        return TRUE;
    }
    else if(u1Type == eSWDMX_GET_AUDIO_ACTIVE_PIDX)
    {
        UINT32 u4AudFifoAddr, u4AudFifoSize,u4DmxAvailSize;
        _SWDMX_GetAudioFifo(prSwdmxInst->u1SwdmxId, &u4AudFifoAddr, &u4AudFifoSize);
        u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, 
                                                                        (DMX_PID_TYPE_T)DMX_PID_TYPE_ES_AUDIO,
                                                                        prSwdmxInst->rFmtInfo.rSwdmxWmvInfo.u1AudPid);
        
        *pu4Para1 = u4AudFifoSize - u4DmxAvailSize;
        return TRUE;
    }
    return FALSE;
}

BOOL _SWDMX_WMVAudioCb(UINT8 u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
#if 0
    SWDMX_INFO_T *prSwdmxInst = (SWDMX_INFO_T*)pvSwdmxInst;
    SWDMX_WMV_VERIFY_NULL_ERROR(prSwdmxInst);
#endif
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_WMV_VERIFY_NULL_ERROR(prSwdmxInst);

#ifdef WAIT_AUDIO_READY
    if (eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE)
    {
        if (_rSwdmxWmvInfo.fgEnVideo)
        {
            VDP_TriggerAudReceive(_rSwdmxWmvInfo.u4TriggerAudioPTS);
            LOG(0,"!!!!!! audio is ready %d\n",_rSwdmxWmvInfo.fgVideoPause);
            if (_rSwdmxWmvInfo.fgVideoPause)
            {
                _WMVStartToPlay(prSwdmxInst);
            }
        }
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
        else if (prSwdmxInst->fgIsBadInterleaved)
        {
            VDP_TriggerAudReceive(_rSwdmxWmvInfo.u4TriggerAudioPTS);
            LOG(0,"!!!!!! audio is ready %d\n",_rSwdmxWmvInfo.fgVideoPause);
            if (_rSwdmxWmvInfo.fgVideoPause)
            {
                _WMVStartToPlay(prSwdmxInst);
            }
        }
#endif
        else
        {
            STC_StartStc(prSwdmxInst->u1StcId);
            AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
        }
    }
    else
    {
        //impossible
        //ASSERT(0);
    }
#endif
    return TRUE;
}

