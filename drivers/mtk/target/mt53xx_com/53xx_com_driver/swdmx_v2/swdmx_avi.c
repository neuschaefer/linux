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
 * $Author: cong.wang $
 * $Date: 2012/11/20 $
 * $RCSfile: swdmx_avi.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_avi.c
 *  Software demux for avi - internal interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "dmx_mm_if.h"
#include "swdmx_avi.h"
#include "vdec_drvif.h"
#include "u_midxbuld.h"
#include "swdmx_debug.h"
#include "aud_drvif.h"
#include "x_util.h"
#include "x_drm_drv.h"
#if defined(CC_MT5387) || defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5396) ||defined(CC_MT5368) ||defined(CC_MT5389)
#include "b2r_drvif.h"
#include "b2r_if.h"
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define LE_LONG32(a) ((a >> 24) | ((a & 0x00FF0000) >> 8) | \
           ((a & 0x0000FF00) << 8) | (a << 24))
#undef min   
#define min(x, y) ((x < y) ? x : y)
#undef max
#define max(x, y) ((x > y) ? x : y)
#undef abs
#define abs(x)                  (((x) >= 0) ? (x) : -(x))
#define IDR_PIC 5
#define IS_IDR_NALU(x) (((x & 0x1f) == IDR_PIC))

#define SWDMX_AVI_AAC_FAKE_HDR_SIZE (7)
#define SWDMX_AVI_AAC_ALLOC_SIZE   (256)


#define SWDMX_AVI_VERIFY_NULL(ptr)                                    \
          do {                                              \
            if (ptr == NULL) { \
                LOG(3, "SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
                return; }      \
          } while (0)

#define SWDMX_AVI_VERIFY_NULL_ERROR(ptr)                                    \
          do {                                              \
            if (ptr == NULL) { \
                LOG(3, "SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
                return FALSE; }      \
          } while (0)


#ifdef LINUX_TURNKEY_SOLUTION
#define SWDMX_AVI_VIDEO_HEADER_ALLOC_SIZE (512)
#endif
#define INPUT_SRC prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->e_mm_src_type
#ifdef SUPPORT_3D_MEDIA
#define ENABLE_NEWTRACK(prSwdmxInst) ((prSwdmxInst->eVideoType==ENUM_SWDMX_VDEC_MJPEG) && (prSwdmxInst->u1VdecId!=prSwdmxInst->u1Vdec2Id))
#define IS_NEWTRACK(prSwdmxInst,u1Id) (ENABLE_NEWTRACK(prSwdmxInst)&&(u1Id==prSwdmxInst->u1Vdec2Id))
#else
#define ENABLE_NEWTRACK(prSwdmxInst) (0)
#define IS_NEWTRACK(prSwdmxInst,u1Id)(0)
#endif

#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
UINT32 _u4Threshold = 90000;
UINT32 _u4Delay = 1;
#define SWDMX_AVI_BAD_INTERLEAVED_PTS_THRD _u4Threshold
#define SWDMX_AVI_BAD_INTERLEAVED_DELAY _u4Delay
#endif

//-----------------------------------------------------------------------------
// Static variables

static BOOL g_fgNotSupport[SWDMX_SOURCE_MAX] = {FALSE};
//-----------------------------------------------------------------------------

//static SWDMX_RANGE_LIST_T *_prRangeList = NULL;

#if 0
static SWDMX_AVI_INFO_T prSwdmxInst->rFmtInfo.rSwdmxAviInfo = {0};

static FEEDER_BUF_INFO_T _rFeederInfo;

static UINT32 _u4LastAlignOffset;

static UCHAR *_pucAviAacBuf = 0;
#ifdef LINUX_TURNKEY_SOLUTION
static UCHAR *_pucAviVidHeaderPtr = 0;
#endif
#endif

#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
static BOOL _fgBiAviAudioReady;
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static SWDMX_HDR_PRS_STATE_T _SWDMX_AviHdrPrsUseIdx(
         UINT8 u1SrcId,
        SWDMX_RANGE_LIST_T* prRangeList, 
        INT32 i4PlaySpeed,
        UINT64 u8SeekPos,
        BOOL fgSpeedChange);

static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverAviDataUseIdx(
    UINT8 u1SrcId,
        INT32 i4PlaySpeed,
        MM_RANGE_ELMT_IDXTBL_T *prIdxTbl, 
        UINT32* pu4CurIdx, 
        SWDMX_RANGE_LIST_T* prRangeList,
       UINT8 u1VdecId);

static SWDMX_HDR_PRS_STATE_T _SWDMX_AviHdrPrsWithoutIdx(
    UINT8 u1SrcId,
        SWDMX_RANGE_LIST_T* prRangeList,
        INT32 i4PlaySpeed,
	 UINT64 u8SeekPos,
	 BOOL fgSpeedChange);

static SWDMX_HDR_PRS_STATE_T _SWDMX_PrsAviChunk(
    UINT8 u1SrcId,
    UCHAR* pucBitstream, 
    SWDMX_RANGE_LIST_T* prRangeList,
    UINT32 u4AvailSize,
    UINT32* pu4ConsumeSize, 
    INT32 i4PlaySpeed,
    BOOL fgRangeEnd);
        
static BOOL _SWDMX_AviGetDataFromFeeder(
    SWDMX_INFO_T *prSwdmxInst,
    FEEDER_REQ_DATA_T *prFeederData,
    UINT8 u1VdecId);

static BOOL _CheckAviChunkIndex(
    UCHAR *pucBuf, 
    UINT32 u4StrmID);

static BOOL _CheckAviChunkType(
    SWDMX_INFO_T *prSwdmxInst,
    UCHAR *pucBuf, 
    UINT8 *pu1HdrType, 
    UINT8 *pu1TrackIdx);

static BOOL _CheckAviOtherChunk(UCHAR *pucBuf);

static BOOL _SWDMX_InitDmx(
    SWDMX_INFO_T *prSwdmxInst,
    UINT8 u1HdrType, 
    UINT8 u1Index,
    VOID* pfnCbFunc, 
    BOOL fgEos);

static VOID AviMoniterVdecSupport(UCHAR u1SrcId, BOOL fgNotSupport);

static BOOL _SWDMX_SendAviEos(
    SWDMX_INFO_T *prSwdmxInst,
    UINT8 u1HdrType,
    UINT8 u1Index);

static BOOL _SWDMX_AviSeekCurPos(SWDMX_INFO_T* prSwdmxInst);
static BOOL _SWDMX_AviSeek(
    SWDMX_INFO_T* prSwdmxInst, 
    UINT64 u8SeekTime, 
    SWDMX_RANGE_LIST_T* prRangeList);

static BOOL _SWDMX_AviResetFeeder(SWDMX_INFO_T* prSwdmxInst);

static BOOL _SWDMX_AviCalculateRepeatFrame(
    SWDMX_INFO_T* prSwdmxInst, 
    INT32 i4PlaySpeed,  
    UINT8 u1VdecId);

static void _SWDMX_AviCalculatePts(
    SWDMX_INFO_T* prSwdmxInst, 
    UINT8 u1HdrType, 
    UINT8 u1TrackIdx, 
    UINT32 u4ChunkSize);

static void _SWDMX_AviPrsDrmChunk(
    SWDMX_INFO_T* prSwdmxInst, 
    UINT8 u1HdrType, 
    UINT32 u4StartAddr);
#if 0
static UINT32 _SWDMX_AviAsci2Decimal(
    UINT8*   aui1_ary, 
    UINT8    ui1_round);

static void _SWDMX_AviPrsSubtitle(
    SWDMX_INFO_T *prSwdmxInst,
    UCHAR* pucBuf,
    UINT32* pt_pts_start,
    UINT32* pt_pts_end);
#endif

static SWDMX_HDR_PRS_STATE_T AviHandAAC(SWDMX_INFO_T *prSwdmxInst, UINT32 u4ChunkSize, UINT32 u4Pts, UINT8 u1TrackIdx);

static void _SWDMX_AviVideoWakeUp(SWDMX_INFO_T *prSwdmxInst);

static SWDMX_HDR_PRS_STATE_T  _SWDMX_AviBeforeDeliverUseIdx(SWDMX_INFO_T *prSwdmxInst, UINT8 u1HdrType);


const UINT32 _pwMpegaSampFreq[3] = {44100, 48000, 32000};

/* bit rate table */
/*  MPEG1 - layer 3 */
const UINT32 _pdwLayer3BitRate1[14] = {
  32000,  40000,  48000,  56000,
  64000,  80000,  96000,  112000,
  128000, 160000, 192000, 224000,
  256000, 320000
};

/*  MPEG2 - layer 3 */
const UINT32 _pdwLayer3BitRate2[14] = {
  8000,   16000,  24000,  32000,
  40000,  48000,  56000,  64000,
  80000,  96000,  112000, 128000,
  144000, 160000
};
  /*  MPEG1 - layer 2 */
const UINT32 _pdwLayer2BitRate1[14] = {
  32000,  48000,  56000,  64000, 
  80000,  96000,  112000, 128000, 
  160000, 192000, 224000, 256000, 
  320000, 384000
};

/*  MPEG1 - layer 1 */
#if 0 
const DWRD _pdwLayer1BitRate1[14] = {
  32000, 64000, 96000, 128000, 
  160000, 192000, 224000, 256000, 
  288000, 320000, 352000, 384000, 
  416000, 448000
};
#endif

/*  MPEG2 - layer 1 */
const UINT32 _pdwLayer1BitRate2[14] = {
  32000, 48000, 56000, 64000,
  80000, 96000, 112000, 128000,
  144000, 160000, 176000, 192000,
  224000,256000
};

BOOL MPEGA_IS_VALID_HDR(const BYTE* hdr) 
{
  return ((hdr[0] == 0xFF) && 
         (((hdr[1] & 0xF0) == 0xF0)||((hdr[1] & 0xF0) == 0xE0)) && 
         ((hdr[1] & 0x06) != 0x0) && ((hdr[2] & 0x0C) != 0x0C) && 
         ((hdr[2] & 0xF0) != 0xF0) && ((hdr[2] & 0xF0) != 0x00));
}

BOOL MPEGA_IS_MPEG25(const BYTE* hdr)
{
   return ((hdr[0] == 0xFF) && ((hdr[1]&0xF0) == 0xE0));
}
UINT32 wMpA_GetSampFreq(BYTE bIdx, BOOL fgMpeg25)
{
  if (bIdx >= 0x3)
    return 0xffff;

  return fgMpeg25 ? _pwMpegaSampFreq[bIdx]/4 : _pwMpegaSampFreq[bIdx];
}

UINT32 dwMpA_GetBitRate(BYTE bIdx, BYTE bLayer, BYTE bID)
{
  UINT32 dwBitRate = 0;
    
  if(!((bIdx>=1)&&(bIdx<=14)))
  {
  	return dwBitRate;
  }

  switch (bLayer)
  {
  case MPEGA_LAYER_3: // - MP3
    if (bID)
      dwBitRate = _pdwLayer3BitRate1[bIdx - 1];  // - MPEG1
    else
      dwBitRate = _pdwLayer3BitRate2[bIdx - 1];  // - MPEG2
    break;
  case MPEGA_LAYER_2: // - MP2
    if (bID)
      dwBitRate = _pdwLayer2BitRate1[bIdx - 1];  // - MPEG1
    else
      dwBitRate = 2 * _pdwLayer3BitRate2[bIdx - 1];  // - MPEG2
    break;
  case MPEGA_LAYER_1: // - MP1
    if (bID)
    {    
      //dwBitRate = 4 * _pdwLayer1BitRate1[bIdx - 1];  // - MPEG1
      dwBitRate = 4 * 32000 * bIdx;
    }  
    else
      dwBitRate = 4 * _pdwLayer1BitRate2[bIdx - 1];  // - MPEG2
    break;
  default:
    // - ASST: unsupported layer
    dwBitRate = 0xffffffff;
    break;
  }

  return dwBitRate;
}

BYTE bMpA_GetBitRateFactor(BYTE bLayer, BYTE bID, BOOL fgMpeg25)
{
  if (bLayer == MPEGA_LAYER_1)
    return bID ? 12 : 24;

  //if (bLayer == MPEGA_LAYER_3)
  //  return bID ? 144 : 72;
  if (fgMpeg25)
    return 72;

  return 144;
}

UINT32 dwMpA_GetFrmSz(const BYTE *pbHdr, UINT32 *pBitRate)
{
  BYTE bID, bLayer, bBitRateIdx, bSampFreqIdx, bPaddingBit;
  UINT32 dwFrmSz;
  BOOL fgMpeg25;

  // - get related value from header
  bID = (pbHdr[1] & 0x08) >> 3;
  bLayer = (pbHdr[1] & 0x06) >> 1;
  bBitRateIdx = (pbHdr[2] & 0xF0) >> 4;
  bSampFreqIdx = (pbHdr[2] & 0x0C) >> 2;
  bPaddingBit = (pbHdr[2] & 0x02) >> 1;
  fgMpeg25 = MPEGA_IS_MPEG25(pbHdr);

  if ((bLayer == 0x0) || (bBitRateIdx == 0x0) || (bBitRateIdx == 0xFF) ||
      (bSampFreqIdx == 0x3))
  {
    // - ASST: given frame header is wrong
  } 

  *pBitRate = (UINT32)dwMpA_GetBitRate(bBitRateIdx, bLayer, bID) ;
  
  dwFrmSz = (((UINT32)bMpA_GetBitRateFactor(bLayer, bID, fgMpeg25) *
              dwMpA_GetBitRate(bBitRateIdx, bLayer, bID)) /
             (UINT32)wMpA_GetSampFreq(bSampFreqIdx, fgMpeg25)) + (UINT32)bPaddingBit;

  return dwFrmSz;
}

BOOL _SWDMX_AVISetDecoderInfo(
    UINT8 u1SrcId,
    UINT8 u1Type, 
    UINT32 u4Para1, 
    UINT32 u4Para2, 
    UINT32 u4Para3)
{
    SWDMX_INFO_T* prSwdmxInst = NULL;
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_AVI_VERIFY_NULL_ERROR(prSwdmxInst);        

    if(u1Type == eSWDMX_SET_VIDEOTYPE) 
    {
        #if defined(CC_AVI_MPEG1_DISABLE) && defined(CC_AVI_MPEG2_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_AVI_MPEG4_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MPEG4)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_AVI_DX311_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_DX311)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_AVI_H263_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_H263)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_AVI_H264_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_H264)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_AVI_VC1_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_VC1)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_AVI_MJPEG_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MJPEG)
        {
            return FALSE;
        }
        else
        #endif
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType =  u4Para1;
            return TRUE;
        }
    }
    else if (u1Type == eSWDMX_SET_AUDIOTYPE)
    {
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ADecType = u4Para1;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudSampleRate = u4Para2;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudChannelNs = u4Para3;
        return TRUE;
    }    
   else if (u1Type == eSWDMX_SET_VIDSEQHEADER)
   {
#ifndef LINUX_TURNKEY_SOLUTION
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidHeaderPtr = u4Para1;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidHdrLength = u4Para2;
 #else
         prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidHdrLength = u4Para2;
         _SWDMX_GetInfo(u1SrcId, eSWDMX_GET_INTERNAL_BUF, &prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidHeaderPtr, 0, 0);
         
         if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidHeaderPtr)
         {
             x_memcpy((VOID*)VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidHeaderPtr), 
                 (VOID*)VIRTUAL(u4Para1), 
                 prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidHdrLength);
         }
         else
         {
             LOG(0,"Header Seq Buffer alloc fail\n");
         }

 #endif
        return TRUE;
    }    

    return FALSE;
}


BOOL _SWDMX_AVIGetInfo(UINT8 u1SrcId,
    UINT8 u1Type,
    UINT32* pu4Para1,
    UINT32* pu4Para2,
    UINT32* pu4Para3)
{
    UINT64 *pu8Tmp;
    SWDMX_INFO_T* prSwdmxInst = NULL;
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_AVI_VERIFY_NULL_ERROR(prSwdmxInst);        
    LOG(5, "_SWDMX_AVIGetInfo u1Type = %d\n", u1Type);

	if(u1Type == eSWDMX_GET_CURRENT_POSITION)
    {
        if(pu4Para1 == NULL)
        {
        	LOG(1, "_SWDMX_AVIGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        pu8Tmp = (UINT64*)pu4Para1;
        *pu8Tmp = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos;
        return TRUE;
    }
    else if (u1Type == eSWDMX_GET_IS_TRICK)
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo)
        {
            if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl != NULL)&&
                (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl->pt_next_partial != NULL))
            {
                if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl->ui4_number_of_entry < AVI_KEY_TABLE_THRESHOLD)&&
                   (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl->pt_next_partial->ui4_number_of_entry < AVI_KEY_TABLE_THRESHOLD))
                {
                    if(pu4Para1)
                    {
                        *pu4Para1 = FALSE;
                    }
                    if(pu4Para2)
                    {
                        *pu4Para2 = FALSE;
                    }
                }
                else
                {
                    if(pu4Para1)
                    {
                        *pu4Para1 = TRUE;
                    }
                    if(pu4Para2)
                    {
                        *pu4Para2 = TRUE;
                    }
                }
            }
            else if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl != NULL)
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl->ui4_number_of_entry <
                    AVI_KEY_TABLE_THRESHOLD)
                {
                    if(pu4Para1)
                    {
                        *pu4Para1 = FALSE;
                    }
                    if(pu4Para2)
                    {
                        *pu4Para2 = FALSE;
                    }
                }
                else
                {
                    if(pu4Para1)
                    {
                        *pu4Para1 = TRUE;
                    }
                    if(pu4Para2)
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
                 if(pu4Para2)
                 {
                     *pu4Para2 = FALSE;
                 }
            }
        }
    }
    else if(u1Type == eSWDMX_GET_AUDIO_ACTIVE_PIDX)
    {
        UINT32 u4AudFifoAddr, u4AudFifoSize,u4DmxAvailSize;
        _SWDMX_GetAudioFifo(prSwdmxInst->u1SwdmxId, &u4AudFifoAddr, &u4AudFifoSize);
        u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, 
                                                                        (DMX_PID_TYPE_T)DMX_PID_TYPE_ES_AUDIO,
                                                                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid);
        
        *pu4Para1 = u4AudFifoSize - u4DmxAvailSize;
        return TRUE;
    }
        
    return FALSE;
}

BOOL _SWDMX_AviVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT32 u4Addr;
    SWDMX_INFO_T *prSwdmxInst = (SWDMX_INFO_T*)prPes->pvInstanceTag;

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    if (!prSwdmxInst)
    {
        LOG(1, "Swdmx Instance is NULL\n");
        return FALSE;
    }
    
    rPesInfo.u4DTS = prPes->u4Dts;
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    rPesInfo.fgDtsValid = prPes->fgPtsDts;
    rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo.fgGop = prPes->fgGop;
    rPesInfo.fgEos = prPes->fgEOS;
    rPesInfo.ucEsId = prPes->u1Channel;     
    #ifdef CC_3D_MM_DS_SUPPORT  
    if(ENABLE_NEWTRACK(prSwdmxInst))  
    {
         if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsLeftVideo)
         {
             rPesInfo.u4PTS = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts;
             rPesInfo.u8Offset = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VidOffset;
         }
         else
         {
             rPesInfo.u4PTS = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSubVidPts;
             rPesInfo.u8Offset = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8SubVidOffset;
         }
    }
    else
    #endif    
    {
         rPesInfo.u4PTS = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts;
         rPesInfo.u8Offset = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VidOffset;
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)
    {
         rPesInfo.rExtra.u8AudPTS =
           prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx];
    }
    #ifdef CC_VDEC_FMT_DETECT
    rPesInfo.u4DmxFrameType = prPes->u4FrameType;
    #endif
    if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == (UINT32)ENUM_SWDMX_VDEC_MJPEG)
    {
        rPesInfo.ucMpvId = VLD1; // video format all use VLD expect jpeg
    }
    else
    {        
        rPesInfo.ucMpvId = VLD0; // video format all use VLD expect jpeg
    }
    if (rPesInfo.fgEos)
    {
         if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed < 0)
     	{
     		rPesInfo.fgBackward = TRUE;
     	}
        rPesInfo.u4FifoStart = 0;
        rPesInfo.u4FifoEnd = 0;
        rPesInfo.u4VldReadPtr = 0;
    }

    if (!rPesInfo.fgEos)
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {  
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsFirstFrame)
            {
                UNUSED(VDEC_SetTotalTime(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TotalTime));
            }
            
            switch (prPes->u4FrameType)
            {
            case PIC_TYPE_I:
                rPesInfo.ucPicType = MPV_I_TYPE;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastIPts = rPesInfo.u4PTS;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8LastIPos = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VidOffset;
                break;

            case PIC_TYPE_P:
                rPesInfo.ucPicType = MPV_P_TYPE;
                break;

            case PIC_TYPE_B:
                rPesInfo.ucPicType = MPV_B_TYPE;
                break;

            default:
                #ifndef CC_VDEC_FMT_DETECT
                ASSERT(0);
                #endif
                break;
            }
        }
        else if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_H264)
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
            rPesInfo.u2SkipVidCount = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u2SkipVidCount;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u2SkipVidCount = 0;
            if(IS_IDR_NALU(rPesInfo.ucPicType))
            {
                rPesInfo.fgDtsValid = TRUE;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastIPts = rPesInfo.u4PTS;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8LastIPos = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VidOffset;
            }
            else
            {
                rPesInfo.fgDtsValid = FALSE;
            }
        }
        else if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_MPEG4)
        {
            UCHAR *pucBuf;

            u4Addr = prPes->u4FrameAddr + 3;
            if(u4Addr >= prPes->u4BufEnd)
            {
                u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
            }
             pucBuf = (UCHAR*)VIRTUAL(u4Addr);
             
            rPesInfo.u2SkipVidCount = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u2SkipVidCount;
            rPesInfo.fgNoVdecCalPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgNoCalPts;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u2SkipVidCount = 0;

            
            if (pucBuf[0] == 0xB6)
            {
                u4Addr = prPes->u4FrameAddr + 4;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgNoCalPts = TRUE;
                if(u4Addr >= prPes->u4BufEnd)
                {
                    u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
                }
                pucBuf = (UCHAR*)VIRTUAL(u4Addr);
                switch (pucBuf[0] >> 6)
                {
                    case 0:
                        rPesInfo.ucPicType  = I_TYPE;
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastIPts = rPesInfo.u4PTS;
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8LastIPos = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VidOffset;
                        rPesInfo.fgDtsValid = TRUE;
                        break;
                    case 1: case 3:
                        rPesInfo.ucPicType  = P_TYPE;
                        rPesInfo.fgDtsValid = FALSE;
                        break;
                    case 2:
                        rPesInfo.ucPicType = B_TYPE;
                        rPesInfo.fgDtsValid = FALSE;
                        break;
                    default:
                      rPesInfo.ucPicType = UNKNOWN_TYPE;
                      rPesInfo.fgDtsValid = FALSE;
                      break;
                }
            }
            else if ((pucBuf[0] & 0x20) == 0x20)            
            {                
                rPesInfo.fgSeqHdr = TRUE;            
            }
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbDmxPos != 0)
            {
                rPesInfo.fgDtsValid = TRUE;
            }
        }
        else if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_DX311)
        {
            UCHAR *pucBuf;
            //UINT32 u4Temp;

            u4Addr = prPes->u4FrameAddr;
            if(u4Addr >= prPes->u4BufEnd)
            {
                u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
            }
            pucBuf = (UCHAR*)VIRTUAL(u4Addr);

            rPesInfo.fgDtsValid = TRUE;

            //u4Temp = pucBuf[0];
            //LOG(5, "u4Temp = 0x%08x\n", u4Temp);
            if ((pucBuf[0] >> 6) & 0x1)
            {
                rPesInfo.ucPicType  = P_TYPE;
            }
            else
            {
                rPesInfo.ucPicType = I_TYPE;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastIPts = rPesInfo.u4PTS;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8LastIPos = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VidOffset;
            }
        }
        else if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_H263)
        {
            UCHAR *pucBuf;
        
            u4Addr = prPes->u4FrameAddr + 4;
            if(u4Addr >= prPes->u4BufEnd)
            {
                u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
            }
            pucBuf = (UCHAR*)VIRTUAL(u4Addr);
        
            switch (pucBuf[0] & 0x02) 
            {
                case 0:
                    rPesInfo.ucPicType = I_TYPE;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastIPts = rPesInfo.u4PTS;
                    break;
                case 2:
                    rPesInfo.ucPicType = P_TYPE;
                    break;
                default:
                    rPesInfo.ucPicType = UNKNOWN_TYPE;
                    break;
            }
        }
        else
        {	    	
            rPesInfo.fgDtsValid = TRUE;
            switch(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType)
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
                default:
                    rPesInfo.u4VideoType = DMX_VIDEO_WMV9;
                break;
            }
        }
    }
    else if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == (UINT32)ENUM_SWDMX_VDEC_MJPEG)
    {
        rPesInfo.ucPicType = I_TYPE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastIPts = rPesInfo.u4PTS;
        #ifdef CC_3D_MM_DS_SUPPORT  
        if(ENABLE_NEWTRACK(prSwdmxInst))  
        {
             if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsLeftVideo)
             {
                 prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8LastIPos = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VidOffset;
             }
             else
             {
                 prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8LastIPos = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8SubVidOffset;
             }
        }
        else
        #endif
        {
             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8LastIPos = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VidOffset;
        }
    }
    
    rPesInfo.u8OffsetI = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8LastIPos;
    rPesInfo.rExtra.u8PTSI = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastIPts;
    
    VDEC_SendEs((void*)&rPesInfo);
    
    return TRUE;
}


BOOL _SWDMX_AviAudioCallback(const DMX_AUDIO_PES_T* prPes)
{
    BOOL fgRet;
    DMX_AUDIO_PES_T rPes;

    SWDMX_INFO_T *prSwdmxInst = NULL;
    

    if(prPes != NULL)
    {
         prSwdmxInst = (SWDMX_INFO_T*)(prPes->pvInstanceTag);
         SWDMX_AVI_VERIFY_NULL_ERROR(prSwdmxInst);
    }
    else
    {
         LOG(1, "_SWDMX_AviAudioCallback prPes NULL\n");
         return FALSE;
    }
    x_memset(&rPes, 0, sizeof(rPes));

    x_memcpy(&rPes, prPes, sizeof(DMX_AUDIO_PES_T));

    fgRet = AUD_SendAudioPes(&rPes);
    return fgRet;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
BOOL _SWDMX_AVIInit(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType)
{
    // Initialize feeder
    //FeederOpen(prSwdmxInst->eFeederSrc);
    INT32 i = 0;

    SWDMX_INFO_T* prSwdmxInst = NULL;
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_AVI_VERIFY_NULL_ERROR(prSwdmxInst);        
    UNUSED(eType);
    x_memset(&prSwdmxInst->rFmtInfo.rSwdmxAviInfo, 0, sizeof(SWDMX_AVI_INFO_T));

    if (!prSwdmxInst->pucAudBuf)
    {
        LOG(3, "_SWDMX_AVIInit audio buffer is NULL, Oops!\n");

       /* prSwdmxInst->pucAudBuf = 
            (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(SWDMX_AVI_AAC_ALLOC_SIZE, 16));*/
            
		{
			UINT32 u4Addr = 0;
        	UINT32 u4Size = SWDMX_AVI_AAC_ALLOC_SIZE;
        	UINT32 u4Align = 16;
			SWDMX_GetInfo(u1SrcId, eSWDMX_GET_INTERNAL_BUF, &u4Addr, &u4Size, &u4Align);
			prSwdmxInst->pucAudBuf = (UCHAR*)u4Addr;
		}

        if(!prSwdmxInst->pucAudBuf)
        {
            LOG(1, "Allocate AAC buffer failed!\n");
            return FALSE;
        }
    }

    //*********************************************************
    //FeederSetBufferSize(prSwdmxInst->eFeederSrc, 5*10*1024); 
    //FeederStart(prSwdmxInst->eFeederSrc);
#if 0
    FeederInstallCallback(prSwdmxInst->eFeederSrc, FEEDER_NOTIFY_DATA_REQ_DONE, 
    _SWDMX_FeederCallBack, (UINT32)&prSwdmxInst->rFeederInfo);
#endif
    //*********************************************************

    // Default Speed
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed = 1000;
    // Default Seek parameter
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSeek = FALSE;

    //prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgPauseVidForAVSync = TRUE;

    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRequestData = TRUE;
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsFirstFrame = TRUE;
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgGetMp3Status = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx = 0xFFFFFFFF;
    //prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ModeChangPts = 0xFFFFFFFF;

    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8RecDmxMoviPos = 0;

    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts = 0xFFFFFFFF;
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts <<= 32;
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts += 0xFFFFFFFF;

    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts = 0xFFFFFFFF;
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts <<= 32;
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts += 0xFFFFFFFF;
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid  = 0xFF;
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid  = 0xFF;
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1SubPid  = 0xFF;

    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PrevSpStrmID = 0xFFFFFFFF;
    
    #ifdef CC_3D_MM_DS_SUPPORT 
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSubVideoRequestData = TRUE;
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatSubFrameIdx = 0xFFFFFFFF;
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1Vid2Pid  = 0xFF;
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsLeftVideo = FALSE;
    #endif
    if (u1SrcId < SWDMX_SOURCE_MAX) 
    {
        g_fgNotSupport[u1SrcId] = FALSE;
    }
    
    for (i=0; i < AVI_DIVX_MAX_AUD_TRACK; i++)
    {
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[i] = 0xFF;
    }
    return TRUE;
}


/**
*/
VOID _SWDMX_AVISetRange(
    UINT8 u1SrcId,
    SWDMX_RANGE_LIST_T *prRangeList, 
    UINT32 u4VidStrmID, 
    UINT32 u4AudStrmID, 
    UINT32 u4SubTitleID)
{
    SWDMX_INFO_T* prSwdmxInst = NULL;
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_AVI_VERIFY_NULL(prSwdmxInst);        

    if (prRangeList != NULL &&
       prRangeList->prDmxRangeInfo != NULL)
    { 
        INT32 i = 0;
        INT32 j = 0;
        
        MM_RANGE_ELMT_IDXTBL_T *pBuldIdxTbl = 
        (MM_RANGE_ELMT_IDXTBL_T*)prRangeList->prDmxRangeInfo->pt_idxtbl_lst;

        prSwdmxInst->prRangeList = prRangeList;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo = prRangeList->prDmxRangeInfo;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsNonSeekable = prRangeList->prDmxRangeInfo->b_non_seekable;
//        _prRangeList = prRangeList;
        while (pBuldIdxTbl != NULL)
        {
            if ((pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_AUDIO )&&
                ( (pBuldIdxTbl->t_strm_id.u.t_stm_avi_id | AVI_AUDIO_FCC) == 
                 AVI_AUDIO_FCC))
            {
                //prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnAudio = TRUE;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prAudIdxTbl[j++] = pBuldIdxTbl;
            }
             else if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_VIDEO )
            {
                #ifdef CC_3D_MM_DS_SUPPORT  
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSupport3DTrick = prRangeList->prDmxRangeInfo->b_support_3d_trick;
                if(ENABLE_NEWTRACK(prSwdmxInst))
                {
                     if(pBuldIdxTbl->t_strm_id.u.t_stm_avi_id == u4VidStrmID)
                    {
                         prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl = pBuldIdxTbl;
                    }  
                    else
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidIdxTbl = pBuldIdxTbl;
                    }
               }
               else 
               #endif
               {
                //prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo = TRUE;
                   if(pBuldIdxTbl->t_strm_id.u.t_stm_avi_id == u4VidStrmID)
                   {
                         prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl = pBuldIdxTbl;
                   }
               }   
             }       
            else if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_KEY)
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgKeyExist = TRUE;
                
                /*if ((pBuldIdxTbl->t_strm_id.u.t_stm_avi_id == u4VidStrmID) &&
                    (((pBuldIdxTbl->t_strm_id.u.t_stm_avi_id | AVI_VIDEO_COMPRE_FCC) == AVI_VIDEO_COMPRE_FCC) ||
                    ((pBuldIdxTbl->t_strm_id.u.t_stm_avi_id | AVI_VIDEO_UNCOMPRE_FCC) == AVI_VIDEO_UNCOMPRE_FCC)))*/
                if (((pBuldIdxTbl->t_strm_id.u.t_stm_avi_id | AVI_VIDEO_COMPRE_FCC) == AVI_VIDEO_COMPRE_FCC) ||
                    ((pBuldIdxTbl->t_strm_id.u.t_stm_avi_id | AVI_VIDEO_UNCOMPRE_FCC) == AVI_VIDEO_UNCOMPRE_FCC))
                {
                    #ifdef CC_3D_MM_DS_SUPPORT  
                     if(ENABLE_NEWTRACK(prSwdmxInst))
                    {
                         if(pBuldIdxTbl->t_strm_id.u.t_stm_avi_id == u4VidStrmID)
                         {
                             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl = pBuldIdxTbl;
                             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl = pBuldIdxTbl;
                             Printf("key size:%d\n",pBuldIdxTbl->u.pt_pb_idx_tbl_entry[(pBuldIdxTbl->ui4_number_of_entry-1)].ui4_size);
                             Printf("last key size:%d\n",pBuldIdxTbl->u.pt_pb_idx_tbl_entry[pBuldIdxTbl->ui4_number_of_entry].ui4_size);
                         }
                         else 
                         {
                             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidKeyIdxTbl = pBuldIdxTbl;
                             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl = pBuldIdxTbl;       
                             Printf("sub key size:%d\n",pBuldIdxTbl->u.pt_pb_idx_tbl_entry[(pBuldIdxTbl->ui4_number_of_entry-1)].ui4_size);
                             Printf("last sub key size:%d\n",pBuldIdxTbl->u.pt_pb_idx_tbl_entry[pBuldIdxTbl->ui4_number_of_entry].ui4_size);
                         }
                     }
                     else
                     #endif
                     {
                         prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl = pBuldIdxTbl;
                         prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl = pBuldIdxTbl;
                     }
                }
                else if ((pBuldIdxTbl->t_strm_id.u.t_stm_avi_id | AVI_AUDIO_FCC) == AVI_AUDIO_FCC)
                {
                    if (i < AVI_DIVX_MAX_AUD_TRACK)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[i++] = pBuldIdxTbl;
                    }
                    else
                    {
                        LOG(3, "Too many audio track key index table\n");
                    }
                }
                else
                {
                    LOG(1, "Unknow Stream ID\n");
                }
            }

            pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)pBuldIdxTbl->pv_next_tbl;
        }

        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl)
        {
            // The upper layer (mw or playmgr should prevent 0 case for rate and scale)
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_scale)
            {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps = 
                (UINT32)(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_rate/prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_scale);
            }
            if (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps ||
            	prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps > 60)
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps = 30;
            }

            LOG(5, "prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps = %ld\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps);
        }

        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgKeyExist)
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack = i;
        }
        else if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prAudIdxTbl)
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack = j;
        }
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TotalTime = prRangeList->prDmxRangeInfo->ui8_pb_duration;
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
        #ifdef CC_3D_MM_DS_SUPPORT 
        if(ENABLE_NEWTRACK(prSwdmxInst))
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSupport3DTrick)
            {
                VDEC_SetMMParam(prSwdmxInst->u1VdecId, VDEC_MM_3D_TRICK_TYPE, VDEC_3D_TRICK_3D_VIEW, 0, 0);
                VDEC_SetMMParam(prSwdmxInst->u1Vdec2Id, VDEC_MM_3D_TRICK_TYPE, VDEC_3D_TRICK_3D_VIEW, 0, 0);
            }
            else
            {
                VDEC_SetMMParam(prSwdmxInst->u1VdecId, VDEC_MM_3D_TRICK_TYPE, VDEC_3D_TRICK_L_VIEW, 0, 0);
                VDEC_SetMMParam(prSwdmxInst->u1Vdec2Id, VDEC_MM_3D_TRICK_TYPE, VDEC_3D_TRICK_L_VIEW, 0, 0);
            }
        }
        #endif
        //prRangeList->u4CurPTS = 
        //prRangeList->prDmxRangeInfo->t_vid_start_render_pts;
  }
  else
  {
     LOG(0, "Demuxer Range is NULL\n");
  }
}


BOOL _SWDMX_AVISetStrmID(
    UINT8 u1SrcId,
    UINT8 u1HdrType,  
    UINT32 u4StrmID, 
    UINT32 u4StrmSubID)
{
    SWDMX_INFO_T* prSwdmxInst = NULL;
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_AVI_VERIFY_NULL_ERROR(prSwdmxInst);        

    UNUSED(u4StrmSubID);
    
    switch(u1HdrType)
    {
        case eSWDMX_STRM_TYPE_VID:
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VStrmID = u4StrmID;
            LOG(5, "_SWDMX_AVISetStrmID Vid = 0x%08x\n", u4StrmID);
            break;
        #ifdef CC_3D_MM_DS_SUPPORT  
        case eSWDMX_STRM_TYPE_VID2:
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VSubStrmID = u4StrmID;
            LOG(5, "_SWDMX_AVISetStrmID Vid = 0x%08x\n", u4StrmID);
            break;
        #endif
        case eSWDMX_STRM_TYPE_AUD:
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AStrmID = u4StrmID;
            LOG(5, "_SWDMX_AVISetStrmID Aud = 0x%08x\n", u4StrmID);
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SpStrmID = u4StrmID;
            LOG(5, "_SWDMX_AVISetStrmID Sub = 0x%08x\n", u4StrmID);
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_AVIEnableStrm(
    UINT8 u1SrcId,
    UINT8 u1StrmType, 
    VOID* pfnCbFunc)
{
    INT32 i;
    SWDMX_INFO_T* prSwdmxInst = NULL;
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_AVI_VERIFY_NULL_ERROR(prSwdmxInst);        

    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            LOG(5, "_SWDMX_AVIEnableStrm Vid\n");
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo = TRUE;
            if (u1SrcId < SWDMX_SOURCE_MAX) 
            {
                g_fgNotSupport[u1SrcId] = FALSE;
            }
            break;
        case eSWDMX_STRM_TYPE_VID2:
            LOG(5, "_SWDMX_AVIEnableStrm Vid\n");
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo = TRUE;
            if (u1SrcId < SWDMX_SOURCE_MAX) 
            {
                g_fgNotSupport[u1SrcId] = FALSE;
            }
            break;
        case eSWDMX_STRM_TYPE_AUD:
            LOG(5, "_SWDMX_AVIEnableStrm Aud\n");
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
            _fgBiAviAudioReady = FALSE;
#endif
            for (i = 0; (i < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack) && (i < AVI_DIVX_MAX_AUD_TRACK); i++)
            {
                if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgKeyExist)&&(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[i]))
                {
                    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[i]->t_strm_id.u.t_stm_avi_id ==
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AStrmID)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx = i;
                        LOG(5, "Cur Track Idx = %ld\n", i);
                    }
                }
                else
                {    
                    if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prAudIdxTbl[i])
                    {
                         if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prAudIdxTbl[i]->t_strm_id.u.t_stm_avi_id ==
                           prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AStrmID)
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx = i;
                            LOG(5, "Cur Track Idx = %ld\n", i);
                        }
                    }
                }
            }
#if 0
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4NumSentAud != 0)
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgChgATrack = TRUE;
            }
#endif
            if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PrevAStrmID != 0xFFFFFFFF)&&(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PrevAStrmID != 0)&&
                 (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PrevAStrmID != prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AStrmID))
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgChgATrack = TRUE;
            }
            else if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAudStrmDisable && 
                (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgPreviouIsSeek) &&
                (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSpeedChange))
            {
                LOG(5, "Audio stream is disabled and seek is not performed\n");
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgChgATrack = TRUE;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAudStrmDisable = FALSE;
            }
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgPreviouIsSeek = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PrevAStrmID = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AStrmID;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnAudio = TRUE;
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
             if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PrevSpStrmID != 0xFFFFFFFF)&&(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PrevSpStrmID != 0)&&
                 (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PrevSpStrmID != prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SpStrmID))
            {
               // prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgChgSbTrack = TRUE;
            }
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnSubDmx == FALSE /*&&*/
                /*(pfnCbFunc)*/)
            {
                UNUSED(_SWDMX_InitDmx(prSwdmxInst, eSWDMX_MEDIA_SUBTITLE, prSwdmxInst->u1SwdmxId, pfnCbFunc, FALSE));
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnSubDmx = TRUE;
            }
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PrevSpStrmID = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SpStrmID;
	    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgChgSbTrack = TRUE;	
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnSubTitle = TRUE;
            LOG(5, "_SWDMX_AVIEnableStrm Sub\n");
           break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_AVIDisableStrm(
    UINT8 u1SrcId,
    UINT8 u1StrmType)
{
#if 0
    DMX_MM_T rPid;

    rPid.fgEnable = FALSE;
#endif 
    SWDMX_INFO_T* prSwdmxInst = NULL;
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_AVI_VERIFY_NULL_ERROR(prSwdmxInst);        

    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
#if 0
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVDmx)
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVDmx = FALSE;
                VERIFY(DMX_MM_Set(eSWDMX_MEDIA_VIDEO, DMX_MM_FLAG_VALID, &rPid));
                DMX_MM_Free(eSWDMX_MEDIA_VIDEO);
            }
#endif
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause)
            {
                _SWDMX_AviVideoWakeUp(prSwdmxInst);
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause = FALSE;
            }

            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo = FALSE;
            LOG(5, "_SWDMX_AVIDisableStrm Vid\n");
            break;
        #ifdef CC_3D_MM_DS_SUPPORT  
        case eSWDMX_STRM_TYPE_VID2:
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause)
            {
                _SWDMX_AviVideoWakeUp(prSwdmxInst);
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause = FALSE;
            }
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo = FALSE;
            LOG(5, "_SWDMX_AVIDisableStrm Vid\n");
            break;
        #endif
        case eSWDMX_STRM_TYPE_AUD:
#if 0
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnADmx)
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnADmx = FALSE;
                VERIFY(DMX_MM_Set(eSWDMX_MEDIA_AUDIO, DMX_MM_FLAG_VALID, &rPid));
                DMX_MM_Free(eSWDMX_MEDIA_AUDIO);
            }
#endif
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnAudio = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAudStrmDisable = TRUE;
            DMX_AudHandler_SetActivePidx(DMX_AUDIO_HANDLER_NULL_PIDX);
			prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSetActivePid = FALSE;
            LOG(5, "_SWDMX_AVIDisableStrm Aud\n");
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
#if 0
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnSubDmx)
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnSubDmx = FALSE;
                VERIFY(DMX_MM_Set(eSWDMX_MEDIA_SUBTITLE, DMX_MM_FLAG_VALID, &rPid));
                DMX_MM_Free(eSWDMX_MEDIA_SUBTITLE);
            }
#endif
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnSubTitle = FALSE;
            LOG(5, "_SWDMX_AVIDisableStrm Sub\n");
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

/**
*/
SWDMX_HDR_PRS_STATE_T _SWDMX_AVIHdrParse(
    UINT8 u1SrcId,
	SWDMX_RANGE_LIST_T* prRangeList,
	 SWDMX_CUR_CMD_T* prCmd)
{
    INT32 i4PlaySpeed = prCmd->i4PlaySpeed;
    UINT64 u8SeekPos = prCmd->u8SeekPos;
    BOOL fgSpeedChange = prCmd->fgChange;
    UINT32 u4CurPts = 0;
    UINT32 u4PrsState = 0;
    UINT32 u4SwdmxInfoAddr = 0;
    VDEC_PTS_INFO_T rPTSInfo = {0};

    SWDMX_INFO_T* prSwdmxInst = NULL;
    SWDMX_AVI_INFO_T *prSwdmxAviInfo = NULL;

    #ifdef CC_3D_MM_DS_SUPPORT  
    VDEC_PTS_INFO_T rSubPTSInfo = {0};
    #endif
    
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    prSwdmxAviInfo = &prSwdmxInst->rFmtInfo.rSwdmxAviInfo;
    x_memset((void*)&rPTSInfo, 0, sizeof(VDEC_PTS_INFO_T));
    if (!prSwdmxInst)
    {
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }
    if (prRangeList->prDmxRangeInfo == NULL)
    {
        LOG(5, "This range is empty\n");
        return eSWDMX_HDR_PRS_EMPTY_RANGE;
    }

    if (!_SWDMX_GetInfo(u1SrcId,eSWDMX_GET_INTERNAL_INFO, (UINT32*)&u4SwdmxInfoAddr, 0, 0))
    {
        LOG(5, "Fail to Get swdmx internal info\n");
        return eSWDMX_HDR_PRS_FAIL;
    }

    if (u4SwdmxInfoAddr != 0)
    {
        if ((prSwdmxInst->eSwDmxPrevousStatus == eSWDMX_STATE_PAUSE) &&
            (i4PlaySpeed == 1000) && 
            (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgPreviouIsSeek))
        {
            LOG(5, "PAUSE -> SEEK -> RESUME set stc value to 0x%08x\n",
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecSeekTime);
            STC_SetStcValue(prSwdmxInst->u1StcId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecSeekTime);
		        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgPreviouIsSeek = FALSE;
        }
        else if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgPreviouIsSeek)&&
            (i4PlaySpeed != 1000)&&(i4PlaySpeed != 1))
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgPreviouIsSeek = FALSE;
        }
    }
    #ifdef CC_3D_MM_DS_SUPPORT  
    if(ENABLE_NEWTRACK(prSwdmxInst))
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo)
        {
            if(!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVDmx)
            {
               UNUSED(_SWDMX_InitDmx(prSwdmxInst, eSWDMX_MEDIA_VIDEO, prSwdmxInst->u1VdecId, NULL, FALSE));
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVDmx = TRUE;
            }
            if(!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnV2Dmx)
            {
                UNUSED(_SWDMX_InitDmx(prSwdmxInst, eSWDMX_MEDIA_VIDEO, prSwdmxInst->u1Vdec2Id, NULL, FALSE));
                 prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnV2Dmx = TRUE;
             }
        }
    }
    else
    #endif
    {
        if((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo) && (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVDmx))
        {
            UNUSED(_SWDMX_InitDmx(prSwdmxInst, eSWDMX_MEDIA_VIDEO, prSwdmxInst->u1VdecId, NULL, FALSE));
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVDmx = TRUE;
        }
    }

    if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnAudio) && 
        (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnADmx))
    {
        UNUSED(_SWDMX_InitDmx(prSwdmxInst, eSWDMX_MEDIA_AUDIO, 0, NULL, FALSE));
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnADmx = TRUE;
    }

    if ((!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgBeginToPlay) &&
        (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts != 0))
    {
 #if 0
#ifdef CC_DLNA_SUPPORT
        if (INPUT_SRC != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        {

            UINT32 u4FeederReadSize;
          
            // For DLNA playback
            if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) != 
              FEEDER_E_OK)
            {
                LOG(1, "Feeder input buffer error.\n");
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
            
            u4FeederReadSize = prSwdmxInst->rFeederInfo.u4Size - MIN_READ_SIZE;
            FeederSetReadyThreshold(u1SrcId, u4FeederReadSize);
            FeederSetUnderFlowThreshold(u1SrcId, u4FeederReadSize/4);
       }
#endif
#endif
        #ifdef CC_3D_MM_DS_SUPPORT 
        VDEC_PTS_INFO_T rSubPTSInfo = {0};
        if(ENABLE_NEWTRACK(prSwdmxInst))
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidIdxTbl)
            {
                rSubPTSInfo.u8Scale = (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidIdxTbl->ui4_scale * DEMUX_AVI_AV_PERIOD;
                rSubPTSInfo.u4Rate = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidIdxTbl->ui4_rate;
            }
        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl)
            {
                rPTSInfo.u8Scale = (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_scale * DEMUX_AVI_AV_PERIOD;
                rPTSInfo.u4Rate = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_rate;
            }
        }        
        else
        #endif    
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl)
            {
                rPTSInfo.u8Scale = (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_scale * DEMUX_AVI_AV_PERIOD;
                rPTSInfo.u4Rate = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_rate;
            }
        }
        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidHeaderPtr != 0)

        {
            BOOL fgSeekReady = FALSE;
            _SWDMX_AviBeforeDeliverUseIdx(prSwdmxInst, eSWDMX_MEDIA_VIDEO);
            fgSeekReady = _SWDMX_AviSeek(prSwdmxInst, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts, prRangeList);
	        LOG(5, "Video Start Render Pts = %lld\n", 
			       prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts);
		 	if(fgSeekReady)
			{
			    prRangeList->fgSetRenderPosition = FALSE; 
			}
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts = 
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts;
            
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSeek = TRUE;
        }
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts = 
                 prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts;

        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo)
        {
            #ifdef CC_3D_MM_DS_SUPPORT
            if(ENABLE_NEWTRACK(prSwdmxInst))
            {
                VDEC_StartPTS(prSwdmxInst->u1Vdec2Id, TRUE, 0, &rSubPTSInfo);
                VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts);
            }
            #endif
            VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE, 0, &rPTSInfo);
            VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts);
        
            VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
            #ifdef CC_3D_MM_DS_SUPPORT
            if(ENABLE_NEWTRACK(prSwdmxInst))
            {
                VDEC_Pause(prSwdmxInst->u1Vdec2Id);
            }
            #endif
            VDEC_Pause(prSwdmxInst->u1VdecId);
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause = TRUE;
        }
        VDEC_SetVdecNotSupportNfy(u1SrcId,AviMoniterVdecSupport);
        STC_StopStc(prSwdmxInst->u1StcId);
        STC_SetStcValue(prSwdmxInst->u1StcId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts - 45000);
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgBeginToPlay = TRUE;
    }
    else if ((!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgBeginToPlay) &&
        (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_aud_start_render_pts != 0))
    {
        BOOL fgSeekReady = FALSE;
#if 0
#ifdef CC_DLNA_SUPPORT
        if (INPUT_SRC != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        {

            UINT32 u4FeederReadSize;
          
            // For DLNA playback
            if (FeederGetBufferInfo(prSwdmxInst->eSrc, &prSwdmxInst->rFeederInfo) != 
              FEEDER_E_OK)
            {
                LOG(1, "Feeder input buffer error.\n");
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
            
            u4FeederReadSize = prSwdmxInst->rFeederInfo.u4Size - MIN_READ_SIZE;
            FeederSetReadyThreshold(u1SrcId, u4FeederReadSize);
            FeederSetUnderFlowThreshold(u1SrcId,u4FeederReadSize/4);
       }
#endif
#endif
        fgSeekReady = _SWDMX_AviSeek(prSwdmxInst, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_aud_start_render_pts, prRangeList);
        LOG(5, "Audio Start Render Pts = %lld\n", 
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_aud_start_render_pts);

		if (fgSeekReady)
		{
		    prRangeList->fgSetRenderPosition = FALSE;
		}
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSeek = TRUE;

        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts = 
                 prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_aud_start_render_pts;
        VDEC_SetVdecNotSupportNfy(u1SrcId,AviMoniterVdecSupport);
        STC_StopStc(prSwdmxInst->u1StcId);
        STC_SetStcValue(prSwdmxInst->u1StcId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_aud_start_render_pts - 45000);
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgBeginToPlay = TRUE;
    }
    else if((!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgBeginToPlay) &&
        (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_start_pos.ui8_pts_info!=0))
    {
        if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) != 
          FEEDER_E_OK)
        {
            LOG(7, "Feeder input buffer error.\n");
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }
		if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo)
        {
            VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE, 0, &rPTSInfo);
            VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_start_pos.ui8_pts_info);
        
            VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
            VDEC_Pause(prSwdmxInst->u1VdecId);
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause = TRUE;
            prRangeList->fgSetRenderPosition = TRUE;
        }
        if ( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl &&
             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl->ui4_number_of_entry != 0)
        {
               BOOL fgSeekReady = FALSE;
               if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidHeaderPtr != 0)
               {
                    _SWDMX_AviBeforeDeliverUseIdx(prSwdmxInst, eSWDMX_MEDIA_VIDEO);
                }
                fgSeekReady = _SWDMX_AviSeek(prSwdmxInst, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_start_pos.ui8_pts_info, prRangeList);
    				    if(fgSeekReady)
    						{
    						    prRangeList->fgSetRenderPosition = FALSE; 
    						}
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts = 
                   prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_start_pos.ui8_pts_info;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts = 
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_start_pos.ui8_pts_info;       
                
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSeek = TRUE;
            
        }
        
        else
        {
	        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnAudio)
	        {
	            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts = 
	                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_start_pos.ui8_pts_info;
	        }
	        
	        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos = 
	            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_start_pos.ui8_i_frame_position;
	        
	        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts =                                                 
	            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_start_pos.ui8_i_pts_info;
	
	        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos = 
	            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos;
	        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr = 
	            VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
	
	        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8AbRepeatVidPts =                                                 
		        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_start_pos.ui8_pts_info;         
		      prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8AbRepeatAudPts =                                                 
		        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_start_pos.ui8_aud_pts_info;
        }
        
        STC_StopStc(prSwdmxInst->u1StcId);
        STC_SetStcValue(prSwdmxInst->u1StcId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_start_pos.ui8_pts_info - 45000);
        if(i4PlaySpeed == 1000)
        {
          prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRecSpeedChangeForAud = TRUE;
          prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4RecPrevSpeedForAud = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed;
        }
    }

    if (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgBeginToPlay &&
        i4PlaySpeed == 1000)
    {
 #if 0 
#ifdef CC_DLNA_SUPPORT
        if (INPUT_SRC != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        {
            UINT32 u4FeederReadSize;

            // For DLNA playback
            if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) != 
              FEEDER_E_OK)
            {
                LOG(1, "Feeder input buffer error.\n");
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
            
            u4FeederReadSize = prSwdmxInst->rFeederInfo.u4Size - MIN_READ_SIZE;
            FeederSetReadyThreshold(u1SrcId,u4FeederReadSize);
            FeederSetUnderFlowThreshold(u1SrcId,u4FeederReadSize/4);
        }
#endif
#endif
        #ifdef CC_3D_MM_DS_SUPPORT
        if(ENABLE_NEWTRACK(prSwdmxInst))
        {
             if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl)
            {
                rPTSInfo.u8Scale = (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_scale * DEMUX_AVI_AV_PERIOD;
                rPTSInfo.u4Rate = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_rate;
            }
               
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidIdxTbl)
            {
                rSubPTSInfo.u8Scale = (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidIdxTbl->ui4_scale * DEMUX_AVI_AV_PERIOD;
                rSubPTSInfo.u4Rate = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidIdxTbl->ui4_rate;
            }
        }
        else
        #endif 
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl)
            {
                rPTSInfo.u8Scale = (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_scale * DEMUX_AVI_AV_PERIOD;
                rPTSInfo.u4Rate = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_rate;
            }
        }
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnAudio && prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo)
        {
            #ifdef CC_3D_MM_DS_SUPPORT 
            if(ENABLE_NEWTRACK(prSwdmxInst))
            {
                VDEC_StartPTS(prSwdmxInst->u1Vdec2Id, TRUE, 0, &rSubPTSInfo);
            }
            #endif
            VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE, 0, &rPTSInfo);

            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_start_pos.ui8_pts_info == 0)
            {
                 #ifdef CC_3D_MM_DS_SUPPORT 
                 if(ENABLE_NEWTRACK(prSwdmxInst))
                 {
                     VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSubVidPts);
                 }
                 #endif
                 VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts);
            }
            else
            {
                 #ifdef CC_3D_MM_DS_SUPPORT 
                 if(ENABLE_NEWTRACK(prSwdmxInst))
                 {
                    VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id, prRangeList->u8StartPts);
                 }
                 #endif
                 VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prRangeList->u8StartPts);
            }
            VDEC_SetVdecNotSupportNfy(u1SrcId,AviMoniterVdecSupport);
            VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
            #ifdef CC_3D_MM_DS_SUPPORT 
             if(ENABLE_NEWTRACK(prSwdmxInst))
             {
                 VDEC_Pause(prSwdmxInst->u1Vdec2Id);
             }
             #endif
            VDEC_Pause(prSwdmxInst->u1VdecId);
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause = TRUE;
       
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)
            {   
                if(!prRangeList->fgSetRenderPosition)
                {
                      AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]);
                      prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = 
                                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx];
                }
                else
                {
                      AUD_SetStartPts(prSwdmxInst->u1AdecId,prRangeList->u8StartAudPts);
                      prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = prRangeList->u8StartAudPts;
                }
                
            }

        }
        // audio only
        else if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnAudio)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)
            {
                if(!prRangeList->fgSetRenderPosition)
                {
                      AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]);
                      prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = 
                                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx];
                }
                else
                {
                      AUD_SetStartPts(prSwdmxInst->u1AdecId,prRangeList->u8StartAudPts);
                      prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = prRangeList->u8StartAudPts;
                }
            }
            else
            {
                LOG(1, "Out of Audio Track Number\n");
                if(!prRangeList->fgSetRenderPosition)
                {
                     AUD_SetStartPts(prSwdmxInst->u1AdecId, 0);
                     prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudStartPts = 0;
                }
                else
                {
                      AUD_SetStartPts(prSwdmxInst->u1AdecId,prRangeList->u8StartAudPts);
                      prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = prRangeList->u8StartAudPts;
                }
            }
        }
        // video only
        else if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo)
        {
            #ifdef CC_3D_MM_DS_SUPPORT 
            if(ENABLE_NEWTRACK(prSwdmxInst))
            {
                VDEC_StartPTS(prSwdmxInst->u1Vdec2Id, TRUE, 0, &rSubPTSInfo);
            }
            #endif
            VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE, 0, &rPTSInfo);
            
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_start_pos.ui8_pts_info == 0)
            {
                #ifdef CC_3D_MM_DS_SUPPORT 
                 if(ENABLE_NEWTRACK(prSwdmxInst))
                 {
                      VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSubVidPts);
                 }
                 #endif
                 VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts);
            }
            else
            {
                 #ifdef CC_3D_MM_DS_SUPPORT 
                 if(ENABLE_NEWTRACK(prSwdmxInst))
                 {
                     VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id, prRangeList->u8StartPts);
                 }
                 #endif
                 VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prRangeList->u8StartPts);
            }

            VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
            #ifdef CC_3D_MM_DS_SUPPORT 
            if(ENABLE_NEWTRACK(prSwdmxInst))
            {
                VDEC_Pause(prSwdmxInst->u1Vdec2Id);
            }
            #endif
            VDEC_Pause(prSwdmxInst->u1VdecId);
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause = TRUE;
        }

        // set stc for starting playback
        STC_StopStc(prSwdmxInst->u1StcId);
        
        if(!prRangeList->fgSetRenderPosition)
        {
            STC_SetStcValue(prSwdmxInst->u1StcId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts - 45000);
        }
        else
        {
            STC_SetStcValue(prSwdmxInst->u1StcId, prRangeList->u8StartPts);
        }
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgBeginToPlay = TRUE;
    }     

    if((prRangeList->fgSetRenderPosition)&&
        (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSendVolInfo))
    {
        UINT8  i = 0;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSendVolInfo = FALSE;
        prRangeList->fgSetRenderPosition =FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid = TRUE;

        for (i =0; (i<prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack)&&(i<AVI_DIVX_MAX_AUD_TRACK); i++)
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[i] = 0;
        }
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos = prRangeList->u8OffsetI;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts = prRangeList->u8StartPtsI;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8AbRepeatVidPts = prRangeList->u8StartPts;         
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8AbRepeatAudPts = prRangeList->u8StartAudPts;  
    }
    
    if(i4PlaySpeed != 1000)
    {
    	  if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)
    	  {
    	    if( (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgKeyExist)&&(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prAudIdxTbl[0] != NULL))
            {
                prRangeList->u4CurRangeAudIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prAudIdxTbl[0]->ui4_number_of_entry;
            }
            else if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prAudIdxTbl [prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]!= NULL)
            {
                prRangeList->u4CurRangeAudIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]->ui4_number_of_entry;
            }
        }
    }

    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVdecNotSupport = g_fgNotSupport[u1SrcId];
    // Reset some values for state change.
    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgChgATrack ||
        fgSpeedChange)
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgDeliverVEos)
        {
            UINT32 u4Flag;
            DMX_MM_T rDmxMMInfo;

            rDmxMMInfo.fgEnable = TRUE;
            rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;

            switch (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType)
            {
                case ENUM_SWDMX_VDEC_DX311: case ENUM_SWDMX_VDEC_WMV7:
                case ENUM_SWDMX_VDEC_WMV8: case ENUM_SWDMX_VDEC_WMV9:
                case ENUM_SWDMX_VDEC_MJPEG:case ENUM_SWDMX_VDEC_H263:
                    LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = FALSE\n");
                    rDmxMMInfo.fgSearchStartCode = FALSE;
                    break;
                default:
                    LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = TRUE\n");
                    rDmxMMInfo.fgSearchStartCode = TRUE;      
                    break;
            }
#if 0
            if (!DMX_MM_Set(eSWDMX_MEDIA_VIDEO, u4Flag, &rDmxMMInfo))
#endif
            #ifdef CC_3D_MM_DS_SUPPORT 
            if(ENABLE_NEWTRACK(prSwdmxInst))
            {
                if (!DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1Vid2Pid, u4Flag, &rDmxMMInfo))
                {
                    LOG(3, "Fail to reset demuxer\n");
                }
            }
            #endif
            if (!DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid, u4Flag, &rDmxMMInfo))
            {
                LOG(3, "Fail to reset demuxer\n");
            }
            
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgDeliverVEos = FALSE;
        }
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgDeliverAEos)
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgDeliverAEos = FALSE;
            AUD_ClearEOSFlag(prSwdmxInst->u1AdecId);
        }
                 
#ifdef CC_DLNA_SUPPORT
        // DLNA
        FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_EOS, FALSE);
#endif
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAudIsReady = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgQAud = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAudIsLoaded = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgBadAudio = FALSE;
        
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }

    if (fgSpeedChange)
    {
        UINT8 i = 0;
        
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSpeedChange = TRUE;

        u4CurPts = VDP_GetPts(prSwdmxInst->u1B2rId);
        LOG(5, "Speed change while VDP Pts = 0x%08x\n", u4CurPts);

        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4NumSentSub = 0;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4NumSentAud = 0;

#ifdef CC_DLNA_SUPPORT
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederIsReady = FALSE;
#endif

        // 1X -> 2X
        if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed == 1000) &&
            (i4PlaySpeed == 2000))
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8RecDmxMoviPos != 0)
            {
                _SWDMX_FlushReqMsgQ(prSwdmxInst->u1SwdmxId);
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes = 0;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8RecDmxMoviPos;
                
            }
        }

        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8RecDmxMoviPos = 0;
        //prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ModeChangPts = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ChangeMode = 0;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRecSpeedChangeForAud = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = 0;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRequestData = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgPreviouIsSeek = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts <<= 32;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts += 0xFFFFFFFF;

        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts <<= 32;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts += 0xFFFFFFFF;
         prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgQAud = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudAddr = 0;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VRemainder = 0;

	    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAbRepeat = FALSE;
	    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbChunkSize = 0;
    	prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbDmxPos = 0;
    	prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbPts = 0;

        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8PrevSubPos = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8PrevSubPos <<= 32;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8PrevSubPos += 0xFFFFFFFF;
        #ifdef CC_3D_MM_DS_SUPPORT 
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame = 0;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSubVideoRequestData = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VRRemainder = 0;
        #endif

         // 1X -> 2X
        if (!(((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed == 1000) && (i4PlaySpeed == 2000)) ||
            ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed == 2000) && (i4PlaySpeed == 1000))))
        {
            for (i = 0; i < AVI_DIVX_MAX_AUD_TRACK; i++)
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ARemainder[i] = 0;
            }
        }

        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause)
        {
            _SWDMX_AviVideoWakeUp(prSwdmxInst);
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause = FALSE;
        }
        #ifdef CC_3D_MM_DS_SUPPORT 
        if(ENABLE_NEWTRACK(prSwdmxInst))
        {
             if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl)
            {
                rPTSInfo.u8Scale = (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_scale * DEMUX_AVI_AV_PERIOD;
                rPTSInfo.u4Rate = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_rate;
            } 
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidIdxTbl)
            {
                 rSubPTSInfo.u8Scale = (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidIdxTbl->ui4_scale * DEMUX_AVI_AV_PERIOD;
                 rSubPTSInfo.u4Rate = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidIdxTbl->ui4_rate;
            }
        }
        else
        #endif   
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl)
            {
                rPTSInfo.u8Scale = (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_scale * DEMUX_AVI_AV_PERIOD;
                rPTSInfo.u4Rate = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_rate;
            }
        } 
        if((prCmd->fgSeek) &&
           (prCmd->u4SeekMode == SWDMX_CUR_CMD_SEEK_OFFSET))
        {
            UINT32 i = 0;
            UINT8 u1Idx = 0;
            LOG(3, "seek offset\n");

            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAbRepeat = TRUE;
            
            if ( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl &&
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl->ui4_number_of_entry != 0)
            {
               u8SeekPos = prCmd->rSeekOffset.ui8_stc;
            }
            else
            {
            	
	            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid = TRUE;
	
	            for (i =0; (i<prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack)&&(i<AVI_DIVX_MAX_AUD_TRACK); i++)
	            {
	                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[i] = 0;
	            }
	
	            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnAudio)
	            {
	                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = prCmd->rSeekOffset.ui8_aud_pts_info;
	                DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid);
	                AUD_SetStartPts(prSwdmxInst->u1AdecId, prCmd->rSeekOffset.ui8_aud_pts_info);
	            }
	            for ( ;u1Idx < (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack)&&(u1Idx < AVI_DIVX_MAX_AUD_TRACK); u1Idx++)
	            {
	                DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[u1Idx]);
	            }
	            
	            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos = 
	                                                              prCmd->rSeekOffset.ui8_i_frame_position;
	            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts =                                                 
	                                                             prCmd->rSeekOffset.ui8_i_pts_info;
	            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8AbRepeatVidPts =                                                 
	                                                             prCmd->rSeekOffset.ui8_pts_info;         
	            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8AbRepeatAudPts =                                                 
	                                                             prCmd->rSeekOffset.ui8_aud_pts_info;
                    #ifdef CC_3D_MM_DS_SUPPORT 
                    if(ENABLE_NEWTRACK(prSwdmxInst))
                    {
                        VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id, prCmd->rSeekOffset.ui8_pts_info);
                    }
                    #endif
	             VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, 
	                     prCmd->rSeekOffset.ui8_pts_info);
	            if(i4PlaySpeed == 1000)
	            {
	              prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRecSpeedChangeForAud = TRUE;
	              prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4RecPrevSpeedForAud = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed;
	            }
	         }
        }
        // seek
         if(((UINT32)(u8SeekPos>>32) != 0xFFFFFFFF)
            || ((UINT32)(u8SeekPos) != 0xFFFFFFFF))
        {
            UINT8 u1Idx = 0;
            
            UNUSED(_SWDMX_AviSeek(prSwdmxInst, u8SeekPos, prRangeList));
            LOG(5, "u8SeekPos = %lld\n", u8SeekPos);

            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts = u8SeekPos;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts = u8SeekPos;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid = TRUE;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgPreviouIsSeek = TRUE;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecSeekTime = u8SeekPos;
            if((prSwdmxInst->fgPreviouIsPause) && (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAbRepeat))
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgPauseToSeek = TRUE;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8PauseToSeekPos = u8SeekPos;
                prSwdmxInst->fgPreviouIsPause = FALSE;
            }
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgMp3Abr)
            {
                LOG(5, "ABR Mp3\n");
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts = 0xFFFFFFFF;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts <<= 32;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts += 0xFFFFFFFF;
                if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)
                {
                    AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]);
                }
            }
            
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo)
            {
                #ifdef CC_3D_MM_DS_SUPPORT 
                if(ENABLE_NEWTRACK(prSwdmxInst))
                {
                    MPV_FlushEsmQ(prSwdmxInst->u1Vdec2Id, TRUE);
                    VDEC_ReleaseDispQ(prSwdmxInst->u1Vdec2Id);
                }
                #endif
                MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
                VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);

                if(i4PlaySpeed >0)
                {    
                    #ifdef CC_3D_MM_DS_SUPPORT 
                    if(ENABLE_NEWTRACK(prSwdmxInst))
                    {
                    	VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id, u8SeekPos); //after, 2011/03/13, from isaac's mail:2011/3/2 pm11:05
//                        VDEC_SetRenderPts(prSwdmxInst->u1Vdec2Id, u8SeekPos); //original, 2011/03/13
                    }
                    #endif
                    VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u8SeekPos); //after, 2011/03/13, from isaac's mail:2011/3/2 pm11:05
//                    VDEC_SetRenderPts(prSwdmxInst->u1VdecId, u8SeekPos); //original, 2011/03/13
                }
                else
                {    
                    #ifdef CC_3D_MM_DS_SUPPORT 
                    if(ENABLE_NEWTRACK(prSwdmxInst))
                    {
                    	VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id, 0); //after, 2011/03/13, from isaac's mail:2011/3/2 pm11:05
//                        VDEC_SetRenderPts(prSwdmxInst->u1Vdec2Id, 0); //original, 2011/03/13
                    }
                    #endif
                    VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, 0); //after, 2011/03/13, from isaac's mail:2011/3/2 pm11:05
//                    VDEC_SetRenderPts(prSwdmxInst->u1VdecId, 0); //original, 2011/03/13
                }
                #ifdef CC_3D_MM_DS_SUPPORT 
                if(ENABLE_NEWTRACK(prSwdmxInst))
                {
                     VDEC_StartPTS(prSwdmxInst->u1Vdec2Id, TRUE, u8SeekPos, NULL);
                }
                #endif
                VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE, u8SeekPos, NULL);
                #ifdef CC_3D_MM_DS_SUPPORT 
                if(ENABLE_NEWTRACK(prSwdmxInst))
                {
                    DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1Vid2Pid);
                }
                #endif
                DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid);
            }

           ///if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnAudio)
           {
               DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid);
           }
            // set stc
            STC_StopStc(prSwdmxInst->u1StcId);
            STC_SetStcValue(prSwdmxInst->u1StcId, u8SeekPos);
                
            for ( ;(u1Idx < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack)&&(u1Idx<AVI_DIVX_MAX_AUD_TRACK); u1Idx++)
            {
                    DMX_MM_FlushBuffer( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[u1Idx]);
            }
            if(i4PlaySpeed == 1000)
            {
              prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRecSpeedChangeForAud = TRUE;
              prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4RecPrevSpeedForAud = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed;
            }
        }
        // 2X -> 1X
		else if(prSwdmxInst->ePlayMode==SWDMX_PLAY_MODE_NORMAL 
			&& prSwdmxInst->ePrePlayMode==SWDMX_PLAY_MODE_NORMAL)
        {
            UINT8 u1TrackIdx = prSwdmxAviInfo->u1CurATrackIdx;
			UINT32 u4Pts=0,u1Idx=0;
            #ifdef CC_3D_MM_DS_SUPPORT 
			UINT32 u4SubPts = 0;
            if(ENABLE_NEWTRACK(prSwdmxInst) && ((i4PlaySpeed == 2000) ||
                ((prSwdmxAviInfo->i4PrevSpeed == 2000) && (i4PlaySpeed == 1000))))
            {
                   if(prSwdmxAviInfo->fgKeyExist)
                   {
					   UNUSED(_SWDMX_AviSeek(prSwdmxInst, u4CurPts, prRangeList));
					   prSwdmxAviInfo->u4PreRepeatFrameIdx = prSwdmxAviInfo->u4CurRepeatFrameIdx;
					   prSwdmxAviInfo->u4PreRepeatSubFrameIdx = prSwdmxAviInfo->u4CurRepeatSubFrameIdx;
                       u4SubPts=prSwdmxAviInfo->prCurSubVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxAviInfo->u4CurSentSubKey].ui4_pts;
					   u4Pts=prSwdmxAviInfo->prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxAviInfo->u4CurSentKey].ui4_pts;
				   }
				   else if(prSwdmxAviInfo->fgEnVDmx)
				   {
                       _SWDMX_AviSeekCurPos(prSwdmxInst);
					   u4Pts=prSwdmxAviInfo->u4CurVidPts;
					   u4SubPts=u4Pts;
				   }
                   
                   MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
                   MPV_FlushEsmQ(prSwdmxInst->u1Vdec2Id, TRUE);
                   VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);                   
                   VDEC_ReleaseDispQ(prSwdmxInst->u1Vdec2Id);
            
                   _SWDMX_FlushReqMsgQ(prSwdmxInst->u1SwdmxId);
                    
                   STC_StopStc(prSwdmxInst->u1StcId);
                   STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);
                   
                   if((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed == 2000) && (i4PlaySpeed == 1000))
                  {
                        UINT8 u1TrackIdx;
                        UINT8 u1Idx = 0;
                        UINT32 u4APts = 0;
                        
                        DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid);
                        DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid);
                        DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1Vid2Pid);
                        
                        VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id, u4CurPts);
                        VDEC_StartPTS(prSwdmxInst->u1Vdec2Id,TRUE,u4SubPts,&rPTSInfo);
                        
                        LOG(0, "######## The current pts is : %x\n", u4CurPts);
                        
                        VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u4CurPts);
                        VDEC_StartPTS(prSwdmxInst->u1VdecId,TRUE,u4Pts,&rPTSInfo);
                        
                        for ( ;(u1Idx < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack)&&(u1Idx<AVI_DIVX_MAX_AUD_TRACK); u1Idx++)
                        {
                              DMX_AudHandler_SeekAndUpdate(
                                      prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[u1Idx],
                                  0xffffffff, &u4APts);
                        }

						
                        u1TrackIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx;
						if(u1TrackIdx<AVI_DIVX_MAX_AUD_TRACK)
						{
							prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx];
						}
                        if(!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgMp3Abr)
                        { 
                            AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]);
                        }
                        else
                        {
                            AUD_SetStartPts(prSwdmxInst->u1AdecId,u4CurPts);
                        }
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudStartPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx];
                 }
            }
            #endif
            u4CurPts = VDP_GetPts(prSwdmxInst->u1B2rId);

            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed == 1)
            {
                STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);
            }

            if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed == 1) || 
                ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed < 1000) && (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed > 1)))
            {
                STC_StartStc(prSwdmxInst->u1StcId);
            }

			if(i4PlaySpeed<=2000&&prSwdmxAviInfo->i4PrevSpeed>2000&&prSwdmxAviInfo->fgKeyExist==FALSE)
			{
                if(prSwdmxAviInfo->fgEnVDmx)
				{
					_SWDMX_AviSeekCurPos(prSwdmxInst);
				}
                #ifdef CC_3D_MM_DS_SUPPORT 
				if(ENABLE_NEWTRACK(prSwdmxInst))
				{
					MPV_FlushEsmQ(prSwdmxInst->u1Vdec2Id, TRUE);
					VDEC_ReleaseDispQ(prSwdmxInst->u1Vdec2Id);
				}
                #endif
				MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
				VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);
                #ifdef CC_3D_MM_DS_SUPPORT 
				if(ENABLE_NEWTRACK(prSwdmxInst))
				{
				     DMX_MM_FlushBuffer(prSwdmxAviInfo->u1Vid2Pid);
				}
                #endif
 			    DMX_MM_FlushBuffer(prSwdmxAviInfo->u1VidPid);
 			    DMX_MM_FlushBuffer(prSwdmxAviInfo->u1AudPid);
				for ( ;(u1Idx < prSwdmxAviInfo->u1TotalATrack)&&(u1Idx<AVI_DIVX_MAX_AUD_TRACK); u1Idx++)
				{
					DMX_AudHandler_SeekAndUpdate(
						prSwdmxAviInfo->u1AudPreBufPid[u1Idx],
					0xffffffff, &u4Pts);
				}
				_SWDMX_FlushReqMsgQ(prSwdmxInst->u1SwdmxId);
				prSwdmxAviInfo->i4ReqReadTimes = 0;
				prSwdmxAviInfo->fgReqDataMiss = FALSE;
				prSwdmxAviInfo->fgRecSpeedChangeForAud = TRUE;
				prSwdmxAviInfo->i4RecPrevSpeedForAud = prSwdmxAviInfo->i4PrevSpeed;
									
                #ifdef CC_3D_MM_DS_SUPPORT
				if(ENABLE_NEWTRACK(prSwdmxInst))
				{
				    VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id, u4CurPts);
					VDEC_StartPTS(prSwdmxInst->u1Vdec2Id, TRUE,prSwdmxAviInfo->u4CurVidPts, &rPTSInfo);
				}
                #endif
				VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u4CurPts);
				VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE, prSwdmxAviInfo->u4CurVidPts,&rPTSInfo);
				if(prSwdmxAviInfo->fgMp3Abr)
				{
					prSwdmxAviInfo->u8VSeekPts = u4CurPts;
				}
				if(u1TrackIdx<AVI_DIVX_MAX_AUD_TRACK)
				{
				    AUD_SetStartPts(prSwdmxInst->u1AdecId,prSwdmxAviInfo->u4AudPts[u1TrackIdx]);
					DMX_AudHandler_SetActivePidx(prSwdmxAviInfo->u1AudPreBufPid[u1TrackIdx]);
					prSwdmxAviInfo->fgSetActivePid = TRUE;
				}
				prSwdmxAviInfo->u4TriggerAudPts = u4CurPts + 27000;
			}
            else if ((u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK)&&(i4PlaySpeed == 1000))
            {
                BOOL fgRet;
                UINT32 u4APts = 0;
                UINT32 u4AVDelta = 0;
                UINT32 u8SeekPos = 0;

                LOG(5, "Avi Speed 2X -> 1X Cur Vid Pts = 0x%08x, Cur Aud Pts = 0x%08x\n",
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx]);

                DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid);

                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = u4CurPts + 27000;
           
                fgRet = DMX_AudHandler_SeekAndUpdate(
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[u1TrackIdx],
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts, &u4APts);

                LOG(5, "Find Audio Track Data for 2X -> 1X\n");
                
                if (fgRet)
                {
                    u4AVDelta = u4APts - u4CurPts;
                    LOG(5, "Find audio in the pre-buffer\n");
                /*add this for some file audio chunk is larger ,lose audio about 2-3s after FF2X or seek after pause */    
                    if((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgPauseToSeek && (u4AVDelta > DEMUX_AVI_AV_PERIOD)) ||
                        (u4AVDelta > DEMUX_AVI_AV_PERIOD))
                    {
                        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgPauseToSeek && (u4AVDelta > DEMUX_AVI_AV_PERIOD) && 
                            (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed == 1))
                        {
                            u8SeekPos = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8PauseToSeekPos;
                        }
                        else if(u4AVDelta > DEMUX_AVI_AV_PERIOD)
                        {
                            u8SeekPos = u4CurPts;
                        }
                        UNUSED(_SWDMX_AviSeek(prSwdmxInst, u8SeekPos, prRangeList));

                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts = u8SeekPos;
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts = u8SeekPos;
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid = TRUE;
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRecSpeedChangeForAud = TRUE;

                        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo)
                        {
                            MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
                            VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);
                         
                            VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u4CurPts);

                            VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE, u4CurPts, NULL);
                        
                            DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid);
                        }                         
                        DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid);
                        STC_StopStc(prSwdmxInst->u1StcId);
                        STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);
                        
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgPauseToSeek = FALSE;
                    }
                    
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = u4APts;
                    
                }
                else
                {
                    LOG(5, "Can't find audio in the pre-buffer\n");
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx];
                }

                if(u4AVDelta < DEMUX_AVI_AV_PERIOD)
                {
                    AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts);

                    DMX_AudHandler_SetActivePidx(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[u1TrackIdx]);
                    
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSetActivePid = TRUE;

                   /* for ( ;(u1Idx < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack)&&(u1Idx<AVI_DIVX_MAX_AUD_TRACK); u1Idx++)
                    {
                        UINT32 u4Pts = 0;
                        DMX_AudHandler_SeekAndUpdate(
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[u1Idx],
                        u4CurPts, &u4Pts);
                   }*/
                  //  prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRecSpeedChangeForAud = TRUE;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4RecPrevSpeedForAud = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed;

                    LOG(5, "Rec Movi Pos = %ld\n",
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8RecDmxMoviPos);
                    LOG(5, "Cur Dmx Pos = %ld, \n",
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos);
                 }
            }
            else
            {
                LOG(1, "Out of Max Audio Track Number\n");
            }
        }
        else if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed < 0 &&
            (i4PlaySpeed == 2000 || i4PlaySpeed == 1000))
        {
            UINT8 u1Idx = 0;
			if(prSwdmxAviInfo->fgKeyExist)
			{
				UNUSED(_SWDMX_AviSeek(prSwdmxInst, u4CurPts, prRangeList));
			}
			else if(prSwdmxAviInfo->fgEnVDmx)
			{
				_SWDMX_AviSeekCurPos(prSwdmxInst);
			}
            #ifdef CC_3D_MM_DS_SUPPORT 
            if(ENABLE_NEWTRACK(prSwdmxInst))
            {
                MPV_FlushEsmQ(prSwdmxInst->u1Vdec2Id, TRUE);
                VDEC_ReleaseDispQ(prSwdmxInst->u1Vdec2Id);
            }
            #endif
            MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
            VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);
            #ifdef CC_3D_MM_DS_SUPPORT 
            if(ENABLE_NEWTRACK(prSwdmxInst))
            {
                DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1Vid2Pid);
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgReqSubDataMiss = FALSE;
            }
            #endif
            DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid);
            DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid);
            
            _SWDMX_FlushReqMsgQ(prSwdmxInst->u1SwdmxId);
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes = 0;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgReqDataMiss = FALSE;
             
            for ( ;(u1Idx < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack)&&(u1Idx<AVI_DIVX_MAX_AUD_TRACK); u1Idx++)
            {
                    DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[u1Idx]);
            }
              
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo)
            {
                #ifdef CC_3D_MM_DS_SUPPORT 
                if(ENABLE_NEWTRACK(prSwdmxInst))
                {
                    VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id, 
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey].ui4_pts);
                }
                #endif
                VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, 
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey].ui4_pts);
                #ifdef CC_3D_MM_DS_SUPPORT 
                if(ENABLE_NEWTRACK(prSwdmxInst))
                {
                    VDEC_StartPTS(prSwdmxInst->u1Vdec2Id, TRUE, 
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey].ui4_pts, 
                        &rPTSInfo);
                }
                #endif
                VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE, 
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey].ui4_pts, 
                    &rPTSInfo);
                if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgMp3Abr)
                {
        	          prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts = u4CurPts;
                } 	      
            }

            if ((i4PlaySpeed == 1000) && 
                (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK))
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx];

                LOG(5, "VDP trigger audio = 0x%08x\n", 
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]);

                if(!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgMp3Abr)
                {	
                    AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]);
                }
                else
                {
                    AUD_SetStartPts(prSwdmxInst->u1AdecId,u4CurPts);
                }
                 prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudStartPts =  prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[ prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx];

                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRecSpeedChangeForAud = TRUE;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4RecPrevSpeedForAud = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed;

                // set stc
            }

            STC_StopStc(prSwdmxInst->u1StcId);
            STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);
			
            if(i4PlaySpeed == 2000)
            {
                STC_StartStc(prSwdmxInst->u1StcId);
            }
            LOG(5, "Avi Speed -32X/-16X/-8X/-4X/-2X -> 1X\n");
        }
        else if (!(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed == 1000 && (i4PlaySpeed == 2000 || i4PlaySpeed == 1)))
        {
            UINT32 u4Pts=0;
#ifdef CC_3D_MM_DS_SUPPORT 
			UINT32 u4SubPts=0;
#endif
            if (!(((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed == 1)&&(i4PlaySpeed == 2000))||
               ( (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed == 2000 ||
               (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed == 1) )&&
               ((i4PlaySpeed<1000)&&(i4PlaySpeed>1)))))
             {
                 UINT8 u1Idx = 0;
				if(prSwdmxAviInfo->fgKeyExist)
				{
					UNUSED(_SWDMX_AviSeek(prSwdmxInst, u4CurPts, prRangeList));
					u4Pts=prSwdmxAviInfo->prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxAviInfo->u4CurSentKey].ui4_pts;
#ifdef CC_3D_MM_DS_SUPPORT 
					if(ENABLE_NEWTRACK(prSwdmxInst))
					{
						u4SubPts=prSwdmxAviInfo->prCurSubVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxAviInfo->u4CurSentSubKey].ui4_pts;
					}
#endif
				}
				else if(prSwdmxAviInfo->fgEnVDmx)
				{
					_SWDMX_AviSeekCurPos(prSwdmxInst);
					u4Pts=prSwdmxAviInfo->u4CurVidPts;
				}
                 #ifdef CC_3D_MM_DS_SUPPORT 
                  if(ENABLE_NEWTRACK(prSwdmxInst))
                  {
                      prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PreRepeatFrameIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx;
                      prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PreRepeatSubFrameIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatSubFrameIdx;
                      MPV_FlushEsmQ(prSwdmxInst->u1Vdec2Id, TRUE);
                      VDEC_ReleaseDispQ(prSwdmxInst->u1Vdec2Id);
                  }
                  #endif
	          MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
	          VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);
                  #ifdef CC_3D_MM_DS_SUPPORT 
                  if(ENABLE_NEWTRACK(prSwdmxInst))
                  {
                      DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1Vid2Pid);
                      prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgReqSubDataMiss = FALSE;
                  }
                  #endif
                  DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid);
                  DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid);
                 for ( ;(u1Idx < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack)&&(u1Idx<AVI_DIVX_MAX_AUD_TRACK); u1Idx++)
                 {
                      DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[u1Idx]);
                 }

                 _SWDMX_FlushReqMsgQ(prSwdmxInst->u1SwdmxId);
                 prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes = 0;
                 prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgReqDataMiss = FALSE;
     
                 prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRecSpeedChangeForAud = TRUE;
                 prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4RecPrevSpeedForAud = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed;
     				
		             if (i4PlaySpeed > 1000)
                 {
                     #ifdef CC_3D_MM_DS_SUPPORT
                     if(ENABLE_NEWTRACK(prSwdmxInst))
                     {
                         VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id, u4CurPts);
                         VDEC_StartPTS(prSwdmxInst->u1Vdec2Id, TRUE, 
                             u4SubPts, 
                             &rPTSInfo);
                     }
                     #endif
                     VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u4CurPts);
                     //avc test
                     VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE, 
                         u4Pts, 
                         &rPTSInfo);
                 }
	               else if (i4PlaySpeed <= 1000 &&
                               i4PlaySpeed > 0)
                 {
                       #ifdef CC_3D_MM_DS_SUPPORT
                       if(ENABLE_NEWTRACK(prSwdmxInst))
                       {
                            VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id, u4CurPts);
                            VDEC_StartPTS(prSwdmxInst->u1Vdec2Id, TRUE, 
								u4SubPts, 
                                 &rPTSInfo);
                       }
                       #endif
	               VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u4CurPts);
	                    //avc test
	                VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE, 
							u4Pts, 
	                        &rPTSInfo);
	                    
	               if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgMp3Abr)
                      {
                	      prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts = u4CurPts;
                      }	   
                }
                else
                {						  
                    #ifdef CC_3D_MM_DS_SUPPORT
                    if(ENABLE_NEWTRACK(prSwdmxInst))
                    {
                        VDEC_SetRenderFromPts(prSwdmxInst->u1Vdec2Id, 
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts);
                        VDEC_StartPTS(prSwdmxInst->u1Vdec2Id, TRUE, 
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts, 
                            &rPTSInfo);  
                    }
                    #endif
                    VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, 
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts);
                    VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE, 
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts, 
                        &rPTSInfo);                
            	  }
          }
          else
          {
  	         STC_StartStc(prSwdmxInst->u1StcId);
          }

          if (i4PlaySpeed > 2000 || i4PlaySpeed < 0)
          {
              prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgStartTrick = TRUE;
          }

                // Things for 32/16/8/4/-2/-4/-8/-16/-32 -> 1X are the same
          if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed > 2000 || prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed < 0) &&
              ((i4PlaySpeed == 1000)||((i4PlaySpeed<1000)&&(i4PlaySpeed>1))) &&
              (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK))
          {
              UINT8 u1TrackIdx;
              UINT8 u1Idx = 0;
              

	      DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid);

              for ( ;(u1Idx < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack)&&(u1Idx<AVI_DIVX_MAX_AUD_TRACK); u1Idx++)
              {
                   DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[u1Idx]);
              }
              u1TrackIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx;
              prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx];
              if(!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgMp3Abr)
              {	
                  AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]);
              }
              else
              {
              	  AUD_SetStartPts(prSwdmxInst->u1AdecId,u4CurPts);
              }
              prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudStartPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx];

              // set stc
              STC_StopStc(prSwdmxInst->u1StcId);
              STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);
          }
        LOG(5, "prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed != 1000 i4PlaySpeed != 2000\n");
          // don't need to handle 1X->2X
        }
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed = i4PlaySpeed;
    }

    if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgChgATrack || prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgChgSbTrack)
        &&(i4PlaySpeed>0)&&(! prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgPreviouIsSeek)&&
        (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSpeedChange))
    {
#ifdef CC_DLNA_SUPPORT
        if (INPUT_SRC == MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        {
#endif
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo)
            {
                u4CurPts = VDP_GetPts(prSwdmxInst->u1B2rId);
            }
            else
            {
                u4CurPts = STC_GetStcValue(prSwdmxInst->u1StcId);
            }

            if(! prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsNonSeekable)
            {
                _SWDMX_FlushReqMsgQ(prSwdmxInst->u1SwdmxId);
      
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos > prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8RecDmxMoviPos)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8RecDmxMoviPos = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos;
                }
                
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgChgSbTrack)
                {
					if(prSwdmxAviInfo->fgKeyExist)
					{
						UNUSED(_SWDMX_AviSeek(prSwdmxInst, u4CurPts, prRangeList));
					}
					else if(prSwdmxAviInfo->fgEnVDmx)
					{
						_SWDMX_AviSeekCurPos(prSwdmxInst);
					}
					
                    if (((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8PrevSubPos >> 32 != 0xFFFFFFFF) ||
                        (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8PrevSubPos != 0xFFFFFFFF)) &&
                        (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8PrevSubPos < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos))
                    {
                        if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos > prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8PrevSubPos) &&
                            ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos - prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8PrevSubPos) < 5 * ONE_MB))
                        {
                            INT32 i = 0;
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8PrevSubPos;
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSeek = TRUE;
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos;
                            //Recover Aud/Vid pts
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecVidPtsForSub;
                            for (i = 0; (i < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack) && (i < AVI_DIVX_MAX_AUD_TRACK); i++)
                            {
                                 prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[i] = 
                                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecAudPtsForSub[i];
                                LOG(5, "Avi Seek Audio pts = 0x%08x\n", 
                                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[i]);
                            }
                        }
                    }
      #if 0
                    else
                    {
                         UNUSED(_SWDMX_AviSeek(prSwdmxInst, u4CurPts, prRangeList));
                    }
       #endif
       
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgChgSbTrack = FALSE;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ChangeMode |= AVI_CHANGE_SUBTITLE_MODE;
                    LOG(5, "Change SubTrack, Rec Movi Pos = %ld\n",
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8RecDmxMoviPos);
                    LOG(5, "Change SubTrack, Cur Dmx Pos = %ld, \n",
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos);
                    LOG(5, "VDP Pts = 0x%08x\n", u4CurPts);
                } 
            }
#ifdef CC_DLNA_SUPPORT
        }
        else
        {
            LOG(5, "Input Source is not MM_SRC_TYPE_HIGH_SPEED_STORAGE\n");

            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgChgSbTrack)
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgChgSbTrack = FALSE;
                LOG(5, "Do nothing for Sub track switch\n");
            }
        }
#endif
        if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgChgATrack) &&
                (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK))
        {   
            BOOL fgRet = FALSE;
            UINT32 u4APts = 0;
       
            DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid);

            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgChgATrack = FALSE;

            if (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRecSpeedChangeForAud)
            {
                u4CurPts = VDP_GetPts(prSwdmxInst->u1B2rId);
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = u4CurPts + 27000;

                if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack)
                {
                    fgRet = DMX_AudHandler_SeekAndUpdate(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx],
                                      prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts, &u4APts);
                }

                LOG(5, "Change Audio Track\n");

                if (fgRet)
                {
                    LOG(5, "Find audio in the pre-buffer\n");
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = u4APts;
                }
                else
                {
                    LOG(5, "Can't find audio in the pre-buffer\n");
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx];
                }

                AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts);
                
                if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack)
                {
                     DMX_AudHandler_SetActivePidx(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]);
                     prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSetActivePid = TRUE;
                }
                LOG(5, "prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = 0x%08x\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts);
            }
        }
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgChgATrack || prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgChgSbTrack)
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgChgATrack)
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgChgATrack = FALSE;
        }
    }
    if (prSwdmxInst->ePlayMode==SWDMX_PLAY_MODE_IFRAME)
    {
        u4PrsState = _SWDMX_AviHdrPrsUseIdx(u1SrcId,
                                                                prRangeList, 
                                                                i4PlaySpeed, 
                                                                u8SeekPos, 
                                                                fgSpeedChange);
    }
    #ifdef CC_3D_MM_DS_SUPPORT
    else if((ENABLE_NEWTRACK(prSwdmxInst)) && (i4PlaySpeed == 2000))
    {
       u4PrsState = _SWDMX_AviHdrPrsUseIdx(u1SrcId,
                                                               prRangeList, 
                                                               i4PlaySpeed, 
                                                               u8SeekPos, 
                                                               fgSpeedChange);
    }
   #endif
    else
    {
        u4PrsState = _SWDMX_AviHdrPrsWithoutIdx(u1SrcId,
                                prRangeList,
                                                                       i4PlaySpeed,
                                                                       u8SeekPos,
                                                                       fgSpeedChange);
    }

	prSwdmxAviInfo->i4PrevSpeed = i4PlaySpeed;
    return (SWDMX_HDR_PRS_STATE_T)u4PrsState;
}

static SWDMX_HDR_PRS_STATE_T _SWDMX_AviHdrPrsUseIdx(
     UINT8 u1SrcId,
        SWDMX_RANGE_LIST_T* prRangeList,
        INT32 i4PlaySpeed,
	 UINT64 u8SeekPos,
	 BOOL fgSpeedChange)
{
    BOOL fgVRangeDone = TRUE;    
    UINT32 *pu4CurIdx = NULL;
    FEEDER_REQ_DATA_T rFeederReqData;
    MM_RANGE_ELMT_IDXTBL_T *prKeyTbl = NULL;
    
    SWDMX_INFO_T* prSwdmxInst = NULL;
    
    #ifdef CC_3D_MM_DS_SUPPORT
    SWDMX_HDR_PRS_STATE_T u4PrsState;
    BOOL fgVSubRangeDone = TRUE;
    UINT32 *pu4CurSubIdx = NULL;
    MM_RANGE_ELMT_IDXTBL_T *prSubKeyTbl = NULL;
    #endif
     
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    if (!prSwdmxInst)
    {
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }
        
    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid)
    {
        INT32 i;
        LOG(3, "Feeder is invalid, Cur Feeder Req ID = %ld,Feeder Rev ID = %ld, Req times = %ld, Cur Dmx Pos = %lld\n",
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID,
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqReceiveID,
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes,
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos);

        for (i = 0; i < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes; i++)
        {
            _SWDMX_AviGetDataFromFeeder(prSwdmxInst, &rFeederReqData, prSwdmxInst->u1VdecId);
            #ifdef CC_3D_MM_DS_SUPPORT
            if(ENABLE_NEWTRACK(prSwdmxInst))
            {
                _SWDMX_AviGetDataFromFeeder(prSwdmxInst, &rFeederReqData, prSwdmxInst->u1Vdec2Id);
            }
            #endif
        }
        
        _SWDMX_AviResetFeeder(prSwdmxInst);

        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSeek = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgReqDataMiss = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes = 0;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID += 1;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqReceiveID = 
             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID;
        #ifdef CC_3D_MM_DS_SUPPORT
        if(ENABLE_NEWTRACK(prSwdmxInst))
        {
             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgReqSubDataMiss = FALSE;
        }
        #endif
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgDeliverVEos)
    {
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        return eSWDMX_HDR_PRS_SUCCEED;
    }
    #ifdef CC_3D_MM_DS_SUPPORT
     prSubKeyTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl;
     
    if(ENABLE_NEWTRACK(prSwdmxInst))
    {
         if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl == 0 ||
             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl ==0)
        {
            LOG(1, "There is no avi key table\n");
            return eSWDMX_HDR_PRS_EMPTY_RANGE;
        }
    }
    else
    #endif   
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl == 0)
        {
            LOG(1, "There is no avi key table\n");
            return eSWDMX_HDR_PRS_EMPTY_RANGE;
        }
    }

    prKeyTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl;

     #ifdef CC_3D_MM_DS_SUPPORT
     if(ENABLE_NEWTRACK(prSwdmxInst) &&(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSupport3DTrick))
    {
        if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->ui4_number_of_entry) ||
             (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl->ui4_number_of_entry))
        {
            _SWDMX_AviResetFeeder(prSwdmxInst);
            
            fgVRangeDone = FALSE;
            pu4CurIdx = &prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey;
            pu4CurSubIdx = &prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSubVidPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurSubIdx].ui4_pts;

            LOG(7, "prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey = %ld\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey);
            LOG(7, "prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey = %ld\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey);
            LOG(7, "Key Pts = %ld\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts);
            LOG(7, "SubKey Pts = %ld\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurSubIdx].ui4_pts);

            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsLeftViewDone == FALSE)
            {
                u4PrsState = _SWDMX_DeliverAviDataUseIdx(u1SrcId,
                    i4PlaySpeed, 
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl, 
                    pu4CurIdx, 
                    prRangeList,
                    prSwdmxInst->u1VdecId);
                    
                if (u4PrsState != eSWDMX_HDR_PRS_SUCCEED)
                {
                    return u4PrsState;
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsLeftViewDone = TRUE;
                }
          	}
          	else
          	{
                u4PrsState = _SWDMX_DeliverAviDataUseIdx(u1SrcId,
                    i4PlaySpeed, 
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl, 
                    pu4CurSubIdx, 
                    prRangeList,
                    prSwdmxInst->u1Vdec2Id);
                    
                if (u4PrsState != eSWDMX_HDR_PRS_SUCCEED)
                {
                    return u4PrsState;
                }
                else
                {
                  prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsLeftViewDone = FALSE;
                }
          	}
            
            if ((i4PlaySpeed < 0) &&
                ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey == 0) ||(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey == 0)))
            {
                LOG(5, "Fast Backward to the first key fram\n");
            }
            LOG(7, "Current key frame movi position = 0x%08x\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos);
        }
        else
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = 0;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame = 0;           
        }    
        
        if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame == 0) &&
             (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame == 0))
        {
            if (i4PlaySpeed >= 2000 && 
            (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey == prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->ui4_number_of_entry) &&
            (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey == prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl->ui4_number_of_entry))
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->pt_next_partial &&
                   prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl->pt_next_partial )
                {
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->pt_next_partial;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey = 0;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx = 0;
                    
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->pt_next_partial;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey = 0;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame = 0;
                }
                else if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->pt_next_partial == NULL) &&
                    (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx == prKeyTbl->ui4_number_of_entry - 1) &&
                    (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->pt_next_partial == NULL) &&
                    (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatSubFrameIdx == prSubKeyTbl->ui4_number_of_entry - 1) &&
                    !prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgDeliverVEos)
                {
                    LOG(5, "Fast Forward _SWDMX_AviHdrPrsUseIdx Deliver Video Eos\n");
                    if (!_SWDMX_SendAviEos(prSwdmxInst, eSWDMX_MEDIA_VIDEO, prSwdmxInst->u1VdecId))
                    {
                        LOG(3, "Fail to send Video Eos\n");
                        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                    }
                     if (!_SWDMX_SendAviEos(prSwdmxInst, eSWDMX_MEDIA_VIDEO, prSwdmxInst->u1Vdec2Id))
                    {
                        LOG(3, "Fail to send Video Eos\n");
                        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgDeliverVEos = TRUE;
                }
            }
            else if (i4PlaySpeed < 0 && (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey == 0)&&
                (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey == 0))
            {
                if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl != prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl != prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidKeyIdxTbl))
                {
                    MM_RANGE_ELMT_IDXTBL_T *prPrevKeyTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl;
                    MM_RANGE_ELMT_IDXTBL_T *prSubPrevKeyTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidKeyIdxTbl;
                    while (prSubPrevKeyTbl->pt_next_partial != prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl)
                    {
                        prPrevKeyTbl = prPrevKeyTbl->pt_next_partial;
                    }
                    while (prPrevKeyTbl->pt_next_partial != prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl)
                    {
                        prPrevKeyTbl = prPrevKeyTbl->pt_next_partial;
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl = prPrevKeyTbl;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey = prPrevKeyTbl->ui4_number_of_entry - 1;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx = prPrevKeyTbl->ui4_number_of_entry - 1;
                    
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl = prPrevKeyTbl;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey = prPrevKeyTbl->ui4_number_of_entry - 1;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatSubFrameIdx = prPrevKeyTbl->ui4_number_of_entry - 1;
                }
                else if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl == prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl) &&
                            (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx == 0) &&
                            (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl == prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidKeyIdxTbl) &&
                            (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatSubFrameIdx == 0) &&
                            !prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgDeliverVEos)
                {
                    LOG(5, "Fast Backward _SWDMX_AviHdrPrsUseIdx Deliver Video Eos\n");
                    if (!_SWDMX_SendAviEos(prSwdmxInst, eSWDMX_MEDIA_VIDEO, prSwdmxInst->u1VdecId))
                    {
                        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                    }
                     if (!_SWDMX_SendAviEos(prSwdmxInst, eSWDMX_MEDIA_VIDEO, prSwdmxInst->u1Vdec2Id))
                    {
                        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgDeliverVEos = TRUE;
                }
            }  
        }
    }
    else
    #endif
    {
         if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->ui4_number_of_entry)
        {
            _SWDMX_AviResetFeeder(prSwdmxInst);

            fgVRangeDone = FALSE;

            pu4CurIdx = &prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;

            LOG(7, "prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey = %ld\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey);
            LOG(7, "Key Pts = %ld\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts);

            _SWDMX_DeliverAviDataUseIdx(u1SrcId,
                i4PlaySpeed, 
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl, 
                pu4CurIdx, 
                prRangeList,
                    prSwdmxInst->u1VdecId);

            if ((i4PlaySpeed < 0) &&
                (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey == 0))
            {
                LOG(5, "Fast Backward to the first key fram\n");
            }

            LOG(7, "Current key frame movi position = 0x%08x\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos);
        }
        else
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = 0;
        }

    // The last key frame in current node
         if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame == 0)
        {
            if (i4PlaySpeed > 2000 && 
            (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey == prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->ui4_number_of_entry))
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->pt_next_partial)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->pt_next_partial;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey = 0;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx = 0;
                }
                else if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl->pt_next_partial == NULL) &&
                    (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx == prKeyTbl->ui4_number_of_entry - 1) &&
                    !prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgDeliverVEos)
                {
                    LOG(5, "Fast Forward _SWDMX_AviHdrPrsUseIdx Deliver Video Eos\n");
                        if (!_SWDMX_SendAviEos(prSwdmxInst, eSWDMX_MEDIA_VIDEO, prSwdmxInst->u1VdecId))
                    {
                        LOG(3, "Fail to send Video Eos\n");
                        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgDeliverVEos = TRUE;
                }
            }
            else if (i4PlaySpeed < 0 && (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey == 0))
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl != prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl)
                {
                    MM_RANGE_ELMT_IDXTBL_T *prPrevKeyTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl;

                    while (prPrevKeyTbl->pt_next_partial != prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl)
                    {
                        prPrevKeyTbl = prPrevKeyTbl->pt_next_partial;
                    }
                    
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl = prPrevKeyTbl;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey = prPrevKeyTbl->ui4_number_of_entry - 1;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx = prPrevKeyTbl->ui4_number_of_entry - 1;
                }
                else if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl == prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl) &&
                            (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx == 0) &&
                            !prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgDeliverVEos)
                {
                        LOG(5, "Fast Backward _SWDMX_AviHdrPrsUseIdx Deliver Video Eos\n");
                        if (!_SWDMX_SendAviEos(prSwdmxInst, eSWDMX_MEDIA_VIDEO, prSwdmxInst->u1VdecId))
                        {
                            return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                        }
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgDeliverVEos = TRUE;
                }
           }
       }
 }
    #ifdef CC_3D_MM_DS_SUPPORT
    if(ENABLE_NEWTRACK(prSwdmxInst))
    {
        if (fgVRangeDone && fgVSubRangeDone)  
        {
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        }        
    }
    else
    #endif
    {
        if (fgVRangeDone)  
        {
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        }
    }
    return eSWDMX_HDR_PRS_SUCCEED;
}        


static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverAviDataUseIdx(
    UINT8 u1SrcId,
    INT32 i4PlaySpeed,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl, 
    UINT32* pu4CurIdx, 
    SWDMX_RANGE_LIST_T* prRangeList,
    UINT8 u1VdecId)
{
    BOOL fgRangeEnd = FALSE;

    UINT8 u1PidIdx = 0;
    UINT8 u1HdrType = 0;
    UINT8 u1PidType = DMX_PID_TYPE_NONE;
    UINT8 u1ReadDrmSize = 0;
    UINT32 u4DmxAvailSize = 0;
    UINT32 u4Size = 0, u4ConsumeSize = 0, u4Ret = 0;
    UINT64 u8VidOffset = 0;
    UCHAR *pucBuf = NULL;

    DMX_MM_DATA_T rDmxMMData;
    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;
    
    SWDMX_INFO_T* prSwdmxInst = NULL;
    
    #ifdef CC_3D_MM_DS_SUPPORT
    UINT32 u4CurPts = 0;
    INT32 u4FbCurPts = 0;
    UINT32 u4PtsDif = 0;
    #endif
    
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    if (!prSwdmxInst)
    {
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }
    
    #ifdef CC_3D_MM_DS_SUPPORT
    if(ENABLE_NEWTRACK(prSwdmxInst))
    {
        if(u1VdecId == prSwdmxInst->u1VdecId)
        {
            u1PidIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid;
            u1PidType = DMX_PID_TYPE_ES_VIDEO;
            u1HdrType = eSWDMX_MEDIA_VIDEO;
        }
        else if(u1VdecId == prSwdmxInst->u1Vdec2Id)
        {
            u1PidIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1Vid2Pid;
            u1PidType = DMX_PID_TYPE_ES_VIDEO;
            u1HdrType = eSWDMX_MEDIA_VIDEO;
        }
        else
        {
            u1PidIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid;
            u1PidType = DMX_PID_TYPE_ES_AUDIO;
        }
    }
    else
    #endif
    {
        if(u1VdecId == prSwdmxInst->u1VdecId)
        {
            u1PidIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid;
            u1PidType = DMX_PID_TYPE_ES_VIDEO;
            u1HdrType = eSWDMX_MEDIA_VIDEO;
        }
        else
        {
            u1PidIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid;
            u1PidType = DMX_PID_TYPE_ES_AUDIO;
        }
    }

    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

#if 0
    u4DmxAvailSize = DMX_MM_GetBufEmptySize(u1HdrType);
#endif
    u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, 
                                                                        (DMX_PID_TYPE_T)u1PidType,
                                                                        u1PidIdx);

    if (prRangeList->prDmxRangeInfo->b_drm)
    {
        // Currently for Home Theater Profile only
        u1ReadDrmSize = AVI_DRM_CHUNK_SIZE_HT;
    }
  
    if (prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size > u4DmxAvailSize)
    {
        LOG(7, "SWDMX Demuxer Buffer is not enough.\n");
        x_thread_delay(1);
        return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
    }

    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) != 
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    if ((*pu4CurIdx) < prIdxTbl->ui4_number_of_entry)
    {
        if (u1HdrType == eSWDMX_MEDIA_VIDEO)
        {
            //UCHAR *pBuf;
            UINT16 u2MaxQNum, u2CurQNum;

            VDEC_GetQueueInfo(u1VdecId, &u2CurQNum, &u2MaxQNum);
            LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
            if ((UINT32)u2CurQNum > (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps/2))
            {
                prSwdmxInst->fgFifoFull = TRUE;
                x_thread_delay(1);
                return eSWDMX_HDR_PRS_SUCCEED;
            }
            if(u1VdecId == prSwdmxInst->u1VdecId)
           {
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame != 0)
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRequestData)
                {
                    if ((prIdxTbl->ui8_base_offset + prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset) >
                        (AVI_PARSER_OFFSET + u1ReadDrmSize))
                    {
                        u8VidOffset = 
                            (prIdxTbl->ui8_base_offset + prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset) - 
                            (AVI_PARSER_OFFSET + u1ReadDrmSize);
                    }
                    else
                    {
                        LOG(3, "Video offset is smaller than (AVI_PARSER_OFFSET + u1ReadDrmSize)\n");
                        u8VidOffset = 0;
                    }
                     /*some time key table offset isn't point to four cc ,add the size that subtract in VidOffset 20101228*/
                    u4Size = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size + AVI_PARSER_OFFSET + 
                                  AVI_PARSER_OFFSET + u1ReadDrmSize;  
                    LOG(5, "Offset = %lld Request size = 0x%08x.\n", u8VidOffset,u4Size);
                    
                        if ((u8VidOffset + u4Size) > prSwdmxInst->u8FileSize)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx = *pu4CurIdx;
                        LOG(5, "The information in the index table is wrong\n");
                        if (i4PlaySpeed > 2000)
                        {
                            (*pu4CurIdx)++;
                        }
                        else if ((i4PlaySpeed < 0) &&
                            (*pu4CurIdx > 0))
                        {
                            (*pu4CurIdx)--;
                        }
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRequestData = TRUE;
                            return eSWDMX_HDR_PRS_FAIL;
                        }
                        if (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgReqDataMiss)
                        {
                            if (FeederSetRequest(prSwdmxInst->eFeederSrc, 
                                u4Size, 
                                u8VidOffset,
                                &rFeederCond,
                                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID) != FEEDER_E_BUFFER_FULL)
                            {
                                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID++;
                                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes++;
                            if (!_SWDMX_AviGetDataFromFeeder(prSwdmxInst, &rFeederReqData, prSwdmxInst->u1VdecId))
                                {
                                    return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                                }
                            }
                            else
                            {
                                LOG(3, "Feeder Buffer is not enough\n");
                                return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                            }
                        }
                        else
                        {
                            LOG(5, "wait for missed data request\n");
                            if (!_SWDMX_AviGetDataFromFeeder(prSwdmxInst, &rFeederReqData, prSwdmxInst->u1VdecId))
                            {
                                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                            }
                        }
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes--;
                        if (rFeederReqData.u4ReadSize > rFeederReqData.u4AlignOffset)
                        {
                            u4Size = rFeederReqData.u4ReadSize - rFeederReqData.u4AlignOffset;
                        }
                        else
                        {
                            return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                        }
                        LOG(5, "rFeederReqData.u4ReadSize = 0x%08x.\n", u4Size);
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VidOffset = u8VidOffset + u4Size;
                        pucBuf = (UCHAR*)(VIRTUAL(rFeederReqData.u4WriteAddr + rFeederReqData.u4AlignOffset));
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecFeederWPtr = (UINT32)pucBuf; 
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecFeederSize = u4Size;
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRequestData = FALSE;
                    }
                    else
                    {
                        pucBuf = (UCHAR*)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecFeederWPtr;
                        u4Size = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecFeederSize;
                    }
                    if (u1ReadDrmSize)
                    {
                        if (_SWDMX_PrsAviChunk(u1SrcId,
                            pucBuf, 
                            prRangeList, 
                            u4Size, 
                            &u4ConsumeSize, 
                            i4PlaySpeed, 
                            fgRangeEnd) != eSWDMX_HDR_PRS_SUCCEED)
                        {
                            LOG(3, "_SWDMX_DeliverAviDataUseIdx Parse Drm Fail\n");
                            goto _Exit;
                        }
                        u8VidOffset += (UINT64)(u4ConsumeSize);
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecFeederSize -= u4ConsumeSize;
                    }
                    pucBuf = (UCHAR*)VIRTUAL((UINT32)pucBuf + u4ConsumeSize);
                    u4Size = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecFeederSize;
                    u4Ret = _SWDMX_PrsAviChunk(u1SrcId,
                                    pucBuf, 
                                    prRangeList, 
                                    u4Size, 
                                    &u4ConsumeSize, 
                                    i4PlaySpeed, 
                                    fgRangeEnd);
                    if ((u4ConsumeSize == AVI_PARSER_OFFSET) &&
                        (u4Size > u4ConsumeSize) &&
                        (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsBlock))
                    {
                        pucBuf = (UCHAR*)VIRTUAL((UINT32)pucBuf + u4ConsumeSize);
                        u4Size -= AVI_PARSER_OFFSET;
                        u4Ret = _SWDMX_PrsAviChunk(u1SrcId,
                                    pucBuf, 
                                    prRangeList, 
                                    u4Size, 
                                    &u4ConsumeSize, 
                                    i4PlaySpeed, 
                                    fgRangeEnd);
                    }
                    if (u4Ret == eSWDMX_HDR_FED_BUF_NOT_ENOUGH)
                    {
                        UINT32 u4RequestSize = 0;
                        u8VidOffset = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VidOffset;
                        LOG(5, "Block Size = 0x%08x\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4BlockChunkSize);
                        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsBlock)
                        {
                            u4RequestSize = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4BlockChunkSize - 
                                (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecFeederSize - u4ConsumeSize);
                        }
                        else
                        {
                            LOG(3, "_SWDMX_DeliverAviDataUseIdx Parse Video Chunk Fail\n");
                            goto _Exit;
                        }
                        if (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgReqDataMiss)
                        {
                            if (FeederSetRequest(prSwdmxInst->eFeederSrc, 
                                u4RequestSize, 
                                u8VidOffset,
                                &rFeederCond,
                                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID) != FEEDER_E_BUFFER_FULL)
                            {
                                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID++;
                                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes++;
                                if (!_SWDMX_AviGetDataFromFeeder(prSwdmxInst, &rFeederReqData, prSwdmxInst->u1VdecId))
                                {
                                    return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                                }
                            }
                            else
                            {
                                LOG(3, "Feeder Buffer is not enough\n");
                                return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                            }
                        }
                        else
                        {
                            LOG(5, "wait for missed data request\n");
                            if (!_SWDMX_AviGetDataFromFeeder(prSwdmxInst, &rFeederReqData, prSwdmxInst->u1VdecId))
                            {
                                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                            }
                        }
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes--;
                        if (rFeederReqData.u4AlignOffset != 0)
                        {
                            LOG(1, "Feeder's alignoffset != 0, it's not normal\n");
                        }
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecFeederSize += rFeederReqData.u4ReadSize;
                        UNUSED(_SWDMX_PrsAviChunk(u1SrcId,
                                    pucBuf, 
                                    prRangeList, 
                                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecFeederSize, 
                                    &u4ConsumeSize, 
                                    i4PlaySpeed, 
                                    fgRangeEnd));
                    }
                    else if (u4Ret != eSWDMX_HDR_PRS_SUCCEED)
                    {
                        LOG(3, "_SWDMX_DeliverAviDataUseIdx Parse Video Chunk Fail\n");
                        goto _Exit;
                    }
                }
                if (!_SWDMX_AviCalculateRepeatFrame(prSwdmxInst, i4PlaySpeed, u1VdecId))
                {
                    #ifdef CC_3D_MM_DS_SUPPORT
                    if(ENABLE_NEWTRACK(prSwdmxInst) &&(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSupport3DTrick))
                    {                  
                        if (i4PlaySpeed >= 2000)
                        {
                            (*pu4CurIdx)++;
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey ++;
                            if((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx !=  prIdxTbl->ui4_number_of_entry - 1) &&
                                (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatSubFrameIdx !=  prIdxTbl->ui4_number_of_entry - 1))
                           {
                                u4PtsDif = prIdxTbl->u.pt_pb_idx_tbl_entry[(*pu4CurIdx)].ui4_pts -
                                            prIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx].ui4_pts;
                                if(u4PtsDif < 6000)
                                { 
                                    if(i4PlaySpeed == 2000)
                                    {
                                        u4CurPts = prIdxTbl->u.pt_pb_idx_tbl_entry[(*pu4CurIdx)].ui4_pts + AVI_2X_TRICK_JUMP_PTS;
                                    }
                                    else
                                    {
                                        u4CurPts = prIdxTbl->u.pt_pb_idx_tbl_entry[(*pu4CurIdx)].ui4_pts + AVI_TRICK_JUMP_PTS;
                                    }
                                    if(((*pu4CurIdx) == prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey) &&
                                        (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame == 0))
                                   {
                                         if(u4CurPts < 
                                            prIdxTbl->u.pt_pb_idx_tbl_entry[(prIdxTbl->ui4_number_of_entry - 1)].ui4_pts)
                                        {
                                            UNUSED(_SWDMX_AviSeek(prSwdmxInst, u4CurPts, prRangeList));
                                        }                                  
                                        else
                                        {
                                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey = prIdxTbl->ui4_number_of_entry - 1;
                                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey = prIdxTbl->ui4_number_of_entry -1;
                                        }
                                   }
                                }
                            }
                        }
                        else if ((i4PlaySpeed < 0) &&
                            (*pu4CurIdx > 0))
                        {
                             (*pu4CurIdx)--;
                             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey --;
                             u4PtsDif = prIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx].ui4_pts -
                                        prIdxTbl->u.pt_pb_idx_tbl_entry[(*pu4CurIdx)].ui4_pts;   
                             if(u4PtsDif < 6000)
                             {  
                                u4FbCurPts = (INT32)(prIdxTbl->u.pt_pb_idx_tbl_entry[(*pu4CurIdx)].ui4_pts) - AVI_TRICK_JUMP_PTS;
                                if(((*pu4CurIdx) == prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey) &&
                                    (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame == 0))
                                {
                                     if(u4FbCurPts > 0)
                                    {
                                        u4CurPts = (UINT32)u4FbCurPts;
                                        UNUSED(_SWDMX_AviSeek(prSwdmxInst, u4CurPts, prRangeList));
                                    }                                  
                                    else
                                    {
                                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey = 0;
                                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey = 0;
                                    }
                               }                                
                             }
                        }
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSubVideoRequestData = TRUE;
                    }
                    else
                    #endif
                    {
                        if (i4PlaySpeed > 2000)
                        {
                            (*pu4CurIdx)++;
                        }
                        else if ((i4PlaySpeed < 0) &&
                            (*pu4CurIdx > 0))
                        {
                            (*pu4CurIdx)--;
                        }
                    }     
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRequestData = TRUE;
                    return eSWDMX_HDR_PRS_SUCCEED;
                }        
        }     
        #ifdef CC_3D_MM_DS_SUPPORT
        if((ENABLE_NEWTRACK(prSwdmxInst)) && u1VdecId == prSwdmxInst->u1Vdec2Id)      
       {
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame != 0)
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSubVideoRequestData)
                {
                    if ((prIdxTbl->ui8_base_offset + prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset) >
                        (AVI_PARSER_OFFSET + u1ReadDrmSize))
                    {
                        u8VidOffset = 
                            (prIdxTbl->ui8_base_offset + prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset) - 
                            (AVI_PARSER_OFFSET + u1ReadDrmSize);
                    }
                    else
                    {
                        LOG(3, "Sub Video offset is smaller than (AVI_PARSER_OFFSET + u1ReadDrmSize)\n");
                        u8VidOffset = 0;
                    }
                    u4Size = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size + AVI_PARSER_OFFSET + u1ReadDrmSize;
                    LOG(5, "Offset = %lld Request size = 0x%08x.\n", u8VidOffset,u4Size);
                    if ((u8VidOffset + u4Size) > prSwdmxInst->u8FileSize)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatSubFrameIdx = *pu4CurIdx;
                        LOG(5, "The information in the sub index table is wrong\n");
                        if (i4PlaySpeed > 2000)
                        {
                            (*pu4CurIdx)++;
                        }
                        else if ((i4PlaySpeed < 0) &&
                            (*pu4CurIdx > 0))
                        {
                            (*pu4CurIdx)--;
                        }
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSubVideoRequestData = TRUE;
                        return eSWDMX_HDR_PRS_FAIL;
                    }

                    if (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgReqSubDataMiss)
                    {
                        if (FeederSetRequest(prSwdmxInst->eFeederSrc, 
                            u4Size, 
                            u8VidOffset,
                            &rFeederCond,
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID) != FEEDER_E_BUFFER_FULL)
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID++;
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes++;

                        if (!_SWDMX_AviGetDataFromFeeder(prSwdmxInst, &rFeederReqData, prSwdmxInst->u1Vdec2Id))
                            {
                                //prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid = TRUE;
                                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                            }
                        }
                        else
                        {
                            LOG(3, "Feeder Buffer is not enough\n");
                            return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                        }
                    }
                    else
                    {
                        LOG(5, "wait for missed data request\n");
                        if (!_SWDMX_AviGetDataFromFeeder(prSwdmxInst, &rFeederReqData, prSwdmxInst->u1Vdec2Id))
                        {
                            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                        }
                    }

                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes--;

                    if (rFeederReqData.u4ReadSize > rFeederReqData.u4AlignOffset)
                    {
                        u4Size = rFeederReqData.u4ReadSize - rFeederReqData.u4AlignOffset;
                    }
                    else
                    {
                        return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                    }

                    LOG(5, "rFeederReqData.u4ReadSize = 0x%08x.\n", u4Size);

                   // prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecFeederSize = u4Size;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8SubVidOffset = u8VidOffset + u4Size;

                    pucBuf = (UCHAR*)(VIRTUAL(rFeederReqData.u4WriteAddr + rFeederReqData.u4AlignOffset));
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecSubVFeederWPtr = (UINT32)pucBuf; 
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecSubVFeederSize = u4Size;

                    // we don't have to request for the same data for frame repeating.
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSubVideoRequestData = FALSE;
                }
                else
                {
                    pucBuf = (UCHAR*)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecSubVFeederWPtr;
                    u4Size =prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecSubVFeederSize;
                }

                if (u1ReadDrmSize)
                {
                    if (_SWDMX_PrsAviChunk(u1SrcId,
                        pucBuf, 
                        prRangeList, 
                        u4Size, 
                        &u4ConsumeSize, 
                        i4PlaySpeed, 
                        fgRangeEnd) != eSWDMX_HDR_PRS_SUCCEED)
                    {
                        LOG(3, "_SWDMX_DeliverAviDataUseIdx Parse Drm Fail\n");
                        goto _Exit;
                    }

                    u8VidOffset += (UINT64)(u4ConsumeSize);
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecSubVFeederSize -= u4ConsumeSize;
                }

                pucBuf = (UCHAR*)VIRTUAL((UINT32)pucBuf + u4ConsumeSize);
                u4Size = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecSubVFeederSize;

                u4Ret = _SWDMX_PrsAviChunk(u1SrcId,
                                pucBuf, 
                                prRangeList, 
                                u4Size, 
                                &u4ConsumeSize, 
                                i4PlaySpeed, 
                                fgRangeEnd);

                // Prevent from empty video chunk
                if ((u4ConsumeSize == AVI_PARSER_OFFSET) &&
                    (u4Size > u4ConsumeSize) &&
                    (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsBlock))
                {
                    pucBuf = (UCHAR*)VIRTUAL((UINT32)pucBuf + u4ConsumeSize);
                    u4Size -= AVI_PARSER_OFFSET;

                    u4Ret = _SWDMX_PrsAviChunk(u1SrcId,
                                pucBuf, 
                                prRangeList, 
                                u4Size, 
                                &u4ConsumeSize, 
                                i4PlaySpeed, 
                                fgRangeEnd);
                }

                if (u4Ret == eSWDMX_HDR_FED_BUF_NOT_ENOUGH)
                {
                    UINT32 u4RequestSize = 0;

                    u8VidOffset = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8SubVidOffset;

                    LOG(5, "Block Size = 0x%08x\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4BlockChunkSize);

                    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsBlock)
                    {
                        u4RequestSize = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4BlockChunkSize - 
                            (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecSubVFeederSize - u4ConsumeSize);
                    }
                    else
                    {
                        LOG(3, "_SWDMX_DeliverAviDataUseIdx Parse Video Chunk Fail\n");
                        goto _Exit;
                    }

                    if (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgReqDataMiss)
                    {
                        if (FeederSetRequest(prSwdmxInst->eFeederSrc, 
                            u4RequestSize, 
                            u8VidOffset,
                            &rFeederCond,
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID) != FEEDER_E_BUFFER_FULL)
                        {
                            //LOG(5, "Feeder ID = %d\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID);
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID++;

                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes++;
                            if (!_SWDMX_AviGetDataFromFeeder(prSwdmxInst, &rFeederReqData, prSwdmxInst->u1Vdec2Id))
                            {
                                //prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid = TRUE;
                                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                            }
                        }
                        else
                        {
                            LOG(3, "Feeder Buffer is not enough\n");
                            return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                        }
                    }
                    else
                    {
                        LOG(5, "wait for missed data request\n");
                        if (!_SWDMX_AviGetDataFromFeeder(prSwdmxInst, &rFeederReqData, prSwdmxInst->u1Vdec2Id))
                        {
                            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                        }
                    }

                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes--;

                    if (rFeederReqData.u4AlignOffset != 0)
                    {
                        LOG(1, "Feeder's alignoffset != 0, it's not normal\n");
                    }

                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecSubVFeederSize += rFeederReqData.u4ReadSize;

                    UNUSED(_SWDMX_PrsAviChunk(u1SrcId,
                                pucBuf, 
                                prRangeList, 
                                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecSubVFeederSize, 
                                &u4ConsumeSize, 
                                i4PlaySpeed, 
                                fgRangeEnd));
                }
                else if (u4Ret != eSWDMX_HDR_PRS_SUCCEED)
                {
                    LOG(3, "_SWDMX_DeliverAviDataUseIdx Parse Video Chunk Fail\n");
                    goto _Exit;
                }
            }

            if (!_SWDMX_AviCalculateRepeatFrame(prSwdmxInst, i4PlaySpeed, u1VdecId))
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSubVideoRequestData = TRUE;
                return eSWDMX_HDR_PRS_SUCCEED;
            }
        }
        #endif
    }
        //update feeder RPTR
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr = (UINT32)pucBuf + u4ConsumeSize;
        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr > 
            VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr -= 
                (prSwdmxInst->rFeederInfo.u4EndAddr - prSwdmxInst->rFeederInfo.u4StartAddr);
        }
        FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, 
            VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr));
    }
    return eSWDMX_HDR_PRS_SUCCEED;

_Exit:
    //update feeder RPTR
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr = (UINT32)pucBuf + u4ConsumeSize;
    if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr > VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
    {
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr -= 
            (prSwdmxInst->rFeederInfo.u4EndAddr - prSwdmxInst->rFeederInfo.u4StartAddr);
    }
    FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, 
        PHYSICAL(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr));

    if (i4PlaySpeed > 2000)
    {
        (*pu4CurIdx)++;
    }
    else if ((i4PlaySpeed < 0) &&
        (*pu4CurIdx > 0))
    {
        (*pu4CurIdx)--;
    }
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRequestData = TRUE;
     #ifdef CC_3D_MM_DS_SUPPORT
    if(ENABLE_NEWTRACK(prSwdmxInst) &&(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSupport3DTrick))
    {
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSubVideoRequestData = TRUE;
    }
    #endif
    return eSWDMX_HDR_PRS_FAIL;

}

static SWDMX_HDR_PRS_STATE_T _SWDMX_AviHdrPrsWithoutIdx(
    UINT8 u1SrcId,
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange)
{
    BOOL fgRangeEnd = FALSE;

    UINT32 u4ReadSize = 0;
    UINT32 u4ConsumedSize = 0;
    UINT32 u4PrsState = eSWDMX_HDR_PRS_SUCCEED;
    UINT32 u4MaxDataByte = 0;
    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;

    SWDMX_INFO_T* prSwdmxInst = NULL;
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    if (!prSwdmxInst)
    {
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }

    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid)
    {
        INT32 i;
        LOG(3, "Feeder is invalid, Cur Feeder Req ID = %ld,Feeder Rev Id = %ld  Req times = %ld, Cur Dmx Pos = %lld\n",
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID,
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqReceiveID,
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes,
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos);

        for (i = 0; i < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes; i++)
        {
            _SWDMX_AviGetDataFromFeeder(prSwdmxInst, &rFeederReqData, prSwdmxInst->u1VdecId);
            #ifdef CC_3D_MM_DS_SUPPORT
            if(ENABLE_NEWTRACK(prSwdmxInst))
            {
                _SWDMX_AviGetDataFromFeeder(prSwdmxInst, &rFeederReqData, prSwdmxInst->u1Vdec2Id);
            }
            #endif
        }
        
        _SWDMX_AviResetFeeder(prSwdmxInst);

        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSeek = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgReqDataMiss = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes = 0;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID += 1;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqReceiveID = 
             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID;
        #ifdef CC_3D_MM_DS_SUPPORT
        if(ENABLE_NEWTRACK(prSwdmxInst))
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgReqSubDataMiss = FALSE;
         }
        #endif
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    // Check if demuxing is done
    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos >= prSwdmxInst->u8FileSize
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
        || (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder == 0 && prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos >= prSwdmxInst->u8FileSize)
#endif
#ifdef __MODEL_slt__
        || prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4NumSentVid == 10
#endif
        )
    {
    	  UINT32 u4CurPts = 0;
    	  BOOL fgRet = FALSE; 
    	  UINT32 u4APts = 0;
    	  
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRecSpeedChangeForAud && prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4RecPrevSpeedForAud == 2000 || 
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4RecPrevSpeedForAud == 1||
                ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4RecPrevSpeedForAud > 1)&&
                (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4RecPrevSpeedForAud<1000)))
            {
                u4CurPts = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo) ? (VDP_GetPts(prSwdmxInst->u1B2rId)) : (STC_GetStcValue(prSwdmxInst->u1StcId));
                
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = u4CurPts + 27000;

                fgRet = DMX_AudHandler_SeekAndUpdate(
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx],
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts, &u4APts);

                LOG(5, "Find Audio Track Data for 2X -> 1X\n");

                if (fgRet)
                {
                    LOG(5, "Find audio in the pre-buffer\n");
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = u4APts;
                }
                else
                {
                    LOG(5, "Can't find audio in the pre-buffer\n");
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx];
                }

                AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts);
                LOG(5, "prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = 0x%08x\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts);
            }
            else
            {
                UNUSED(DMX_AudHandler_SeekAndUpdate(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx],
                    0xFFFFFFFF, &u4APts));
            }

            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRecSpeedChangeForAud = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4RecPrevSpeedForAud = i4PlaySpeed;
            DMX_AudHandler_SetActivePidx(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]);
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSetActivePid = TRUE;
        }
        
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo && 
            !prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgDeliverVEos)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause)
            {
                _SWDMX_AviVideoWakeUp(prSwdmxInst);
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause = FALSE;
            }

            LOG(5, "Cur DmxMoviPos = %ld, range size = %ld\n", 
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos,
                prSwdmxInst->u8RangeSize);
            
            LOG(5, "_SWDMX_AviHdrPrsWithoutIdx Deliver Video Eos\n");
            #ifdef CC_3D_MM_DS_SUPPORT
            if(ENABLE_NEWTRACK(prSwdmxInst))
            {
                if (!_SWDMX_SendAviEos(prSwdmxInst, eSWDMX_MEDIA_VIDEO, prSwdmxInst->u1Vdec2Id))
                {
                    LOG(3, "Fail to send VideoR Eos\n");
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                }
                 if (!_SWDMX_SendAviEos(prSwdmxInst, eSWDMX_MEDIA_VIDEO, prSwdmxInst->u1VdecId))
                {
                    LOG(3, "Fail to send Video Eos\n");
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                }
            }
            else
            #endif
            {
                if (!_SWDMX_SendAviEos(prSwdmxInst, eSWDMX_MEDIA_VIDEO, prSwdmxInst->u1VdecId))
                {
                    LOG(3, "Fail to send Video Eos\n");
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                }
            }
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgDeliverVEos = TRUE;
        }

        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnAudio && 
            !prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgDeliverAEos)
        {
            LOG(5, "_SWDMX_AviHdrPrsWithoutIdx Deliver Audio Eos\n");
            if (!_SWDMX_SendAviEos(prSwdmxInst, eSWDMX_MEDIA_AUDIO, 0))
            {
                LOG(3, "Fail to send Audio Eos\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgDeliverAEos = TRUE;
            AUD_MM_Set_Avsync_event(prSwdmxInst->u1AdecId);
        }

        return eSWDMX_HDR_PRS_SUCCEED;
    }

    // Request data for the first time
    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos == 0)
    {
        if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) != 
          FEEDER_E_OK)
        {
            LOG(7, "Feeder input buffer error.\n");
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

        if (prSwdmxInst->u8RangeSize > prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos)
        {
            u4ReadSize = prSwdmxInst->u8RangeSize - 
                                   prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos;
        }

		if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
		{
			u4ReadSize = min(u4ReadSize, (UINT32)MIN_READ_SIZE);
		}
		else
		{
			u4ReadSize = min(u4ReadSize, (UINT32)MIN_READ_SIZE_NO_VIDEO);
		}

        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastAlignOffset = 0;

        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos = 
            prSwdmxInst->u8FileOffset;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos = 
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr = 
            VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);

        if (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgReqDataMiss)
        {
            if (FeederSetRequest(prSwdmxInst->eFeederSrc, 
                    u4ReadSize, /*MIN_READ_SIZE, */
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos,
                    &rFeederCond,
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID) != FEEDER_E_BUFFER_FULL)
            {
                //LOG(5, "Feeder ID = %d\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID);
                 prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes ++;
                 prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID++;
                if (!_SWDMX_AviGetDataFromFeeder(prSwdmxInst, &rFeederReqData, prSwdmxInst->u1VdecId))
                {
                     //prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid = TRUE;

                    return eSWDMX_HDR_PRS_FAIL;
                }
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes --;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastAlignOffset = rFeederReqData.u4AlignOffset;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos += 
                    rFeederReqData.u4ReadSize - prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastAlignOffset;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr += prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastAlignOffset;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
                LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
            }
        }
        else
        {
            LOG(5, "wait for missed data request\n");
            if (!_SWDMX_AviGetDataFromFeeder(prSwdmxInst, &rFeederReqData, prSwdmxInst->u1VdecId))
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }

            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastAlignOffset = rFeederReqData.u4AlignOffset;

            if ((rFeederReqData.u4ReadSize > prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastAlignOffset) &&
                (rFeederReqData.u4ReadSize != 0))
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos += 
                (rFeederReqData.u4ReadSize-prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastAlignOffset);
            }
            else
            {
                LOG(3, "Feeder read size is smaller than align offset!!!\n");
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos = prSwdmxInst->u8FileSize;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
                return eSWDMX_HDR_PRS_FAIL;
            }
                    
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes--;

            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSeek)
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr = 
                VIRTUAL(rFeederReqData.u4WriteAddr + prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastAlignOffset);
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSeek = FALSE;
            }
        }
    }

    if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder <=  prSwdmxInst->rFeederInfo.u4Size) ||
        (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsBlock == TRUE))
    {
        //BOOL fgReadData = TRUE;
        UINT32 u4WriteAddr = 0;

        if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) != 
          FEEDER_E_OK)
        {
            LOG(7, "Feeder input buffer error.\n");
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }
        
		if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
		{
			u4MaxDataByte  = prSwdmxInst->rFeederInfo.u4Size - MIN_READ_SIZE;
		}
		else
		{
			u4MaxDataByte  = prSwdmxInst->rFeederInfo.u4Size - MIN_READ_SIZE_NO_VIDEO;
		}
		
        if (prSwdmxInst->u8FileSize > prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos)
        {
            u4ReadSize = prSwdmxInst->u8FileSize - prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos;
        }
		
		if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
		{
			u4ReadSize = min(u4ReadSize, (UINT32)MIN_READ_SIZE);
		}
		else
		{
			u4ReadSize = min(u4ReadSize, (UINT32)MIN_READ_SIZE_NO_VIDEO);
		}

        if (u4ReadSize == 0)
        {
            fgRangeEnd = TRUE;
        }

        u4WriteAddr = VIRTUAL(prSwdmxInst->rFeederInfo.u4WriteAddr + u4ReadSize);
        if (u4WriteAddr >= VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
        {
            u4WriteAddr =  VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr) + 
                (u4WriteAddr -VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr));
        }
                

#ifdef CC_DLNA_SUPPORT
            // DLNA
        if ((!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederIsReady) ||
                (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder < u4MaxDataByte))
#else
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder < u4MaxDataByte)
#endif
        {
            if (u4ReadSize > 0)
            {
                LOG(6, "Left bytes = %ld offset = %lld id = %ld.\n", 
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder,
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos,
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID);

                if (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgReqDataMiss)
                {
                    if (FeederSetRequest(prSwdmxInst->eFeederSrc, 
                            u4ReadSize, 
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos,
                            &rFeederCond,
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID) != FEEDER_E_BUFFER_FULL)
                    {
                        //LOG(5, "Feeder ID = %d\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID);

                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastAlignOffset = 0;
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes++;
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID++;
                    }
                    else
                    {
                        LOG(3, "Feeder Buffer Full\n");
                       // prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid = TRUE;
                        if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) != 
                            FEEDER_E_OK)
                        {
                            LOG(1, "Feeder input buffer error.\n");
                            return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                        }
                        LOG(3, "Cur Fed's write ptr = 0x%08x\n", prSwdmxInst->rFeederInfo.u4WriteAddr);
                        //return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                    }
                }
                else
                {
                    LOG(5, "wait for missed data request\n");
                    if (!_SWDMX_AviGetDataFromFeeder(prSwdmxInst, &rFeederReqData , prSwdmxInst->u1VdecId))
                    {
                        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                    }

                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastAlignOffset = rFeederReqData.u4AlignOffset;

                    if ((rFeederReqData.u4ReadSize > prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastAlignOffset) &&
                        (rFeederReqData.u4ReadSize != 0))
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos += rFeederReqData.u4ReadSize - prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastAlignOffset;
                    }
                    else
                    {
                        LOG(3, "Feeder read size is smaller than align offset!!!\n");
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos = prSwdmxInst->u8FileSize;
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                            
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes--;

                    if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSeek)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr = VIRTUAL(rFeederReqData.u4WriteAddr + prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastAlignOffset);
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSeek = FALSE;
                    }
                }
            }
        }
    }

#ifdef CC_DLNA_SUPPORT

	if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
	{
	   if (u4ReadSize != MIN_READ_SIZE)
	   {
		   // DLNA
		   FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_EOS, TRUE);
	   }
	}
	else
	{
		if (u4ReadSize != MIN_READ_SIZE_NO_VIDEO)
	   {
		   // DLNA
		   FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_EOS, TRUE);
	   }
	}

   

    // DLNA
    if (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederIsReady && FeederIsReady(prSwdmxInst->eFeederSrc))
    /*if ((!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederIsReady) && 
        ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder >= AVI_PREBUFFER_SIZE) || (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder >= prSwdmxInst->u8RangeSize)))*/
    {
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederIsReady = TRUE;
    }
#endif

#ifdef SWDMX_TIME_PROFILE
    HAL_TIME_T rTimeRstS, rTimeRstE, rTimeRstDt; 
    HAL_GetTime(&rTimeRstS);
#endif

#ifdef CC_DLNA_SUPPORT
    if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder > 0) && prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederIsReady)
#else
    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder)
#endif
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSpeedChange && i4PlaySpeed == 1000)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnAudio && prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo)
            {
                if(((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts>>32) != 0xFFFFFFFF)
                    || ((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts) != 0xFFFFFFFF))
                {
                    if (prRangeList->prDmxRangeInfo->ui8_vid_duration > 
                        (UINT32)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts)
                    {
                        LOG(5, "Pause Video for A/V Sync\n");
                        VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
                        #ifdef CC_3D_MM_DS_SUPPORT
                        if(ENABLE_NEWTRACK(prSwdmxInst)) 
                        {
                            VDEC_Pause(prSwdmxInst->u1Vdec2Id);
                        }
                        #endif
                        VDEC_Pause(prSwdmxInst->u1VdecId);
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause = TRUE;
                    }
                }
#if 0
                else
                {
                    LOG(5, "Pause Video for A/V Sync\n");
                    VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
                    VDEC_Pause(prSwdmxInst->u1VdecId);
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause = TRUE;
                }
#endif
            }
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSpeedChange = FALSE;
        }
    
        // Start to parse data
        u4PrsState = _SWDMX_PrsAviChunk(u1SrcId,
            (UCHAR*)(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr),
            prRangeList,
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder,
            &u4ConsumedSize,
            i4PlaySpeed,
            fgRangeEnd);
    }

#ifdef SWDMX_TIME_PROFILE
    HAL_GetTime(&rTimeRstE);
    HAL_GetDeltaTime(&rTimeRstDt, &rTimeRstS, &rTimeRstE);
    LOG(0, "Swdmx parsing and deliver time = (%ld) \n", rTimeRstDt.u4Micros);
#endif

    switch (u4PrsState)
    {
        case eSWDMX_HDR_PRS_FAIL:
            // Prevent unknow chunk in the first one mega
            // If the file size is smaller than one mega, avi demuxer
            // will check if demuxing is done in the beginning of _SWDMX_AviHdrPrsWithoutIdx
            if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos >=
                 prSwdmxInst->u8FileSize) ||
                (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos > prSwdmxInst->u8FileOffset + 4*ONE_MB))
            {
                LOG(3, "No avi chunks was discovered\n");
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos = prSwdmxInst->u8FileSize;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
                return eSWDMX_HDR_PRS_FAIL;
            }
            break;
        default:
            break;
    }

    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder >= u4ConsumedSize)
    {
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos += u4ConsumedSize;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder -= u4ConsumedSize;
    }
    else
    {
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
    }

    if (u4ConsumedSize != 0)
    {
        LOG(6, "u4ConsumedSize = %ld u4LeftBytesInFeeder = 0x%08x.\n", 
            u4ConsumedSize, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder);
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSkipIdxData &&
        (u4ConsumedSize > prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder))
    {
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos += u4ConsumedSize;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSkipIdxData = FALSE;
    }
    else if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr + u4ConsumedSize) >= 
        VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
    {
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr =  VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr) + 
            ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr + u4ConsumedSize) -VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr));
    }
    else
    {
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr += u4ConsumedSize;
    }
    
    // Update read ptr to feeder
    if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbDmxPos != 0 && 
         prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudAddr != 0)
    {
        UINT32 u4DmxPos = MIN(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbDmxPos ,prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudAddr );
        FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, VIRTUAL(u4DmxPos));
    }  
    else if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbDmxPos != 0 )
    {
        FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbDmxPos));
    }
    else if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudAddr)
    {
        FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudAddr));
    }
    else
    {
        FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, 
           VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr));
    }

    LOG(9, "prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxMoviPos = %ld.\n", 
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos);

    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes > 0)
    {
        if (!_SWDMX_AviGetDataFromFeeder(prSwdmxInst, &rFeederReqData, prSwdmxInst->u1VdecId))
        {
            //prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid = TRUE;
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

        LOG(6, "rFeederReqData.u4WriteAddr = 0x%08x. ReadSize = 0x%08x id = %ld\n", 
            rFeederReqData.u4WriteAddr,
            rFeederReqData.u4ReadSize,
            rFeederReqData.u4Id);

        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastAlignOffset = rFeederReqData.u4AlignOffset;

        if ((rFeederReqData.u4ReadSize > prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastAlignOffset) &&
            (rFeederReqData.u4ReadSize != 0))
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos += 
                rFeederReqData.u4ReadSize-prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastAlignOffset;
        }
        else
        {
            LOG(3, "Feeder read size is smaller than align offset!!!\n");
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos = prSwdmxInst->u8FileSize;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
            return eSWDMX_HDR_PRS_FAIL;
        }
                
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4ReqReadTimes--;

        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSeek)
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr = 
                VIRTUAL(rFeederReqData.u4WriteAddr + prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LastAlignOffset);
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSeek = FALSE;
        }
        LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
    }

    return eSWDMX_HDR_PRS_SUCCEED;
}


static SWDMX_HDR_PRS_STATE_T _SWDMX_PrsAviChunk(
    UINT8 u1SrcId,
    UCHAR* pucBitstream, 
    SWDMX_RANGE_LIST_T* prRangeList,
    UINT32 u4AvailSize,
    UINT32* pu4ConsumeSize, 
    INT32 i4PlaySpeed,
    BOOL fgRangeEnd)
{
    BOOL fgRingBuf = FALSE;
    BOOL fgFindInRing = FALSE;
    BOOL fgFindFcc = FALSE;

    UINT8 u1HdrType;
    UINT8 u1TrackIdx = 0;
    UINT8 u1AudPid = 0;

    UINT32 i = 0, j = 0;
    UINT32 u4RingSize = 0, u4RingStart = 0, u4OrgRingSize = 0;
    //UINT32 u4TotalConsumeSize = 0;
    UINT32 u4ChunkSize, u4DmxAvailSize,u4DmxAudAvailSize;
    UINT32 u4Pts = 0;
    UINT32 u4OrgAvailSize = u4AvailSize;

    //UINT64 u8Numerator, u8Denominator;
    //UINT64 u8Remainder;

    UCHAR ucRingBuf[AVI_PARSER_OFFSET];
    UCHAR ucTempBuf[AVI_CHUNK_ID_OFFSET];
    UCHAR* pucTemp;
    UCHAR* pucOriginalPtr = pucBitstream;

    DMX_MM_DATA_T rDmxMMData;
    SWDMX_INFO_T* prSwdmxInst = NULL;
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    

    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
    if (!prSwdmxInst)
    {
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }

    if (u4AvailSize < AVI_PARSER_OFFSET)
    {
        *pu4ConsumeSize = 0;
    }

    while (i < u4AvailSize)
    {
        if (((pucBitstream + i) >= (UCHAR*)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr - AVI_PARSER_OFFSET))) &&
            ((u4AvailSize - i) >= AVI_PARSER_OFFSET) && (!fgRingBuf))
        {
            fgRingBuf = TRUE;
            u4RingSize = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr)) - (UINT32)(pucBitstream + i);
            u4OrgRingSize = u4RingSize;

            LOG(5, "Ring Buffer start from 0x%08x to 0x%08x size = 0x%08x\n", pucBitstream, 
                VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr), u4RingSize);

            x_memcpy(ucRingBuf, (pucBitstream + i), u4RingSize);

            u4AvailSize -= (u4RingSize + i);
            pucBitstream = (UCHAR*)(VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
            i = 0;
        }

        if ((fgRingBuf) && (u4RingSize))
        {
            // Handle the ring buffer case
            if (u4RingSize > AVI_CHUNK_ID_OFFSET)
            {
                for (j = 0; j < AVI_CHUNK_ID_OFFSET; j++)
                {
                    ucTempBuf[j] = ucRingBuf[u4RingStart + j];
                }
            }
            else
            {
                for (j = 0; j < u4RingSize; j++)
                {
                    ucTempBuf[j] = ucRingBuf[u4RingStart + j];
                }
                for (; j < AVI_CHUNK_ID_OFFSET; j++)
                {
                    ucTempBuf[j] = pucBitstream[j - u4RingSize];
                }
            }
            
            if (_CheckAviChunkType(prSwdmxInst, ucTempBuf, &u1HdrType, &u1TrackIdx) == TRUE)
            {
                fgFindFcc = TRUE;
                fgFindInRing = TRUE;
                break;
            }
            u4RingStart++;
            u4RingSize--;
            continue;
        }

        if (_CheckAviChunkType(prSwdmxInst, pucBitstream + i, &u1HdrType, &u1TrackIdx) == TRUE)
        {
            fgFindFcc = TRUE;
            break;
        }
        i++;
        //u4TotalConsumeSize++;
    }

    // Didn't find the chunk
    if (!fgFindFcc)
    {
        *pu4ConsumeSize = u4OrgAvailSize;
        return eSWDMX_HDR_PRS_FAIL;
    }

    if ((!fgRingBuf) || (!fgFindInRing))
    {
        pucTemp = pucBitstream + AVI_CHUNK_ID_OFFSET + i;
        rDmxMMData.u4StartAddr = (UINT32)(pucBitstream + AVI_PARSER_OFFSET + i);
    }
    else
    {
        if (u4RingSize == 0)
        {
            pucTemp = pucBitstream;
            rDmxMMData.u4StartAddr = (UINT32)(pucBitstream + AVI_CHUNK_ID_OFFSET);
        }
        else
        {
            LOG(5, "fgFindInRing == TRUE u4RingSize = 0x%08x\n", u4RingSize);
            if (u4RingSize <= AVI_CHUNK_ID_OFFSET)
            {
                pucTemp = (UCHAR*)(pucBitstream + (AVI_CHUNK_ID_OFFSET - u4RingSize));
            }
            else
            {
                u4RingStart += AVI_CHUNK_ID_OFFSET;
                
                for (j = 0; j < (u4OrgRingSize - u4RingStart); j++)
                {
                    ucTempBuf[j] = ucRingBuf[u4RingStart + j];
                }
                for (j = 0; j < (AVI_CHUNK_ID_OFFSET - (u4RingSize - u4RingStart)); j++)
                {
                    ucTempBuf[j + (u4OrgRingSize - u4RingStart)] = pucBitstream[j];
                }
                pucTemp = ucTempBuf;
            }

            rDmxMMData.u4StartAddr = (UINT32)(pucBitstream + (AVI_PARSER_OFFSET - u4RingSize));
        }
    }

    u4ChunkSize = (pucTemp[0] | (pucTemp[1] << 8) |
                             (pucTemp[2] <<16) | (pucTemp[3] << 24));

    LOG(7, "u4ChunkSize = %ld.\n", u4ChunkSize);

    
    // Error Handling
    if (u4ChunkSize > prSwdmxInst->rFeederInfo.u4Size)
    {
        LOG(3, "u4ChunkSize > Feeder size = 0x%08x\n", u4ChunkSize);
        //add for mutil riff avi file
        if ((u1HdrType == eSWDMX_MEDIA_SKIPPED) && (!fgRangeEnd))
        {
            *pu4ConsumeSize = u4ChunkSize + AVI_PARSER_OFFSET + i;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSkipIdxData = TRUE;
            if (fgRingBuf)
            {
                *pu4ConsumeSize += ((UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr)) - 
                                                                   (UINT32)(pucOriginalPtr) - 
                                                                   u4RingSize);
            }
            return eSWDMX_HDR_PRS_SUCCEED;
        }
        if((i + prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ErrorData)> MIN_READ_SIZE*2)
        {
            return eSWDMX_HDR_PRS_FAIL;  
        }
        else
        {
            *pu4ConsumeSize = i + AVI_PARSER_OFFSET;  
             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ErrorData = i;  
            return eSWDMX_HDR_PRS_SUCCEED;
        }
    } 
    else if ((u4ChunkSize + AVI_PARSER_OFFSET + i) > u4AvailSize)
    {
        LOG(3, "Chunk size is 0x%08x larger than AvailSize, the size is 0x%08x\n", u4ChunkSize, u4AvailSize);
        u4DmxAudAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, (DMX_PID_TYPE_T)(DMX_PID_TYPE_ES_AUDIO),prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid);   
        
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgQAud)
        {
		    
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudSize < u4DmxAudAvailSize)
            {
		        if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ADecType == ENUM_SWDMX_ADEC_AAC)&&
          	  	(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack < AVI_DIVX_MAX_AUD_TRACK))
	            {
	                  AviHandAAC(prSwdmxInst, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudSize,prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudPts, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack);
	               
	              }
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack < AVI_DIVX_MAX_AUD_TRACK)
                {
                    u1AudPid= prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack] ;
                }
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack < AVI_DIVX_MAX_AUD_TRACK)
                {
                    u4Pts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack];
                }
                else
                {
                    u4Pts = 0;
                }
                
                rDmxMMData.u1Idx = u1AudPid;
                rDmxMMData.u4BufStart = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                rDmxMMData.u4BufEnd = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr));
                rDmxMMData.u4StartAddr = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudAddr;
                rDmxMMData.u4FrameSize = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudSize;
                rDmxMMData.u4Pts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudPts;
                
                LOG(5, "Send Q Audio Data Size = 0x%08x pts = 0x%08x\n", u4ChunkSize, rDmxMMData.u4Pts);
    
                //if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
                if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
                {
                    LOG(0, "Demuxer fails to move data.\n");
                    *pu4ConsumeSize = 0;
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                }
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsBlock = TRUE;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4BlockChunkSize = u4ChunkSize + AVI_PARSER_OFFSET;
                *pu4ConsumeSize = 0;
    
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgQAud = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudAddr = 0;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudSize = 0;
                
                return eSWDMX_HDR_PRS_SUCCEED;
            }
        }
         //add for mutil riff avi file
        if ((u1HdrType == eSWDMX_MEDIA_SKIPPED) && (!fgRangeEnd))
        {
            *pu4ConsumeSize = u4ChunkSize + AVI_PARSER_OFFSET + i;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSkipIdxData = TRUE;
            if (fgRingBuf)
            {
                *pu4ConsumeSize += ((UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr)) - 
                                                                   (UINT32)(pucOriginalPtr) - 
                                                                   u4RingSize);
            }
            return eSWDMX_HDR_PRS_SUCCEED;
        }
        if (!fgRangeEnd)
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsBlock = TRUE;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4BlockChunkSize = u4ChunkSize + AVI_PARSER_OFFSET;
            *pu4ConsumeSize = i;
        }
        else
        {
            *pu4ConsumeSize = u4AvailSize;
            LOG(5, "Range End!!!\n");
        }
        
        return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
    }
    else if (!u4ChunkSize)
    {
        // dummy video chunk
        // still have to calculate the pts
        if (u1HdrType == eSWDMX_MEDIA_VIDEO)
        {
            _SWDMX_AviCalculatePts(prSwdmxInst, u1HdrType, u1TrackIdx, u4ChunkSize);
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u2SkipVidCount++;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgNoCalPts = FALSE;        
        }
        
        *pu4ConsumeSize = AVI_PARSER_OFFSET + i;
        return eSWDMX_HDR_PRS_SUCCEED;
    }
    
    if(u1HdrType != eSWDMX_MEDIA_SKIPPED)
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8RecDmxMoviPos)
        {
            LOG(7, "CurDmxMoviPos when mode Change = %ld\n", 
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos);
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8RecDmxMoviPos > prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos)
            {
                // Find the first audio chunk since 2X -> 1X
                if (u1HdrType == eSWDMX_MEDIA_AUDIO)
                {
                    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ChangeMode & AVI_CHANGE_AUDIO_MODE)
                    {
                        UINT8 u1ATrack = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx;
                        UINT32 u4ModeChangePts = 0;
                        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo)
                        {
                            u4ModeChangePts = VDP_GetPts(prSwdmxInst->u1B2rId);
                        }
                        else
                        {
                            u4ModeChangePts = STC_GetStcValue(prSwdmxInst->u1StcId);
                        }
                        //prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ModeChangPts = VDP_GetPts(0);
                        
                        if (u1ATrack < AVI_DIVX_MAX_AUD_TRACK)
                        {
                            //if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1ATrack] >= (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ModeChangPts + 27000))
                            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1ATrack] >= (u4ModeChangePts + 18000))
                            {
                                LOG(5, "Audio Pts (0x%08x) > Mode ChangePts (0x%08x)\n", 
                                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1ATrack], 
                                    u4ModeChangePts);

                                //prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ModeChangPts = 0xFFFFFFFF;
                                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ChangeMode = 0;
                                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1ATrack];

                                AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1ATrack]);
                                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudStartPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1ATrack];
                            }
                            else
                            {
                                LOG(5, "Audio Pts (0x%08x) < Mode ChangePts (0x%08x)\n", 
                                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1ATrack], 
                                    u4ModeChangePts);

                                _SWDMX_AviCalculatePts(prSwdmxInst, u1HdrType, u1ATrack, u4ChunkSize);
                                u1HdrType = eSWDMX_MEDIA_SKIPPED;
                            }
                        }
                    }
                    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ChangeMode & AVI_CHANGE_SUBTITLE_MODE)
                    {
                         UINT8 u1ATrack = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx;
                         _SWDMX_AviCalculatePts(prSwdmxInst, u1HdrType, u1ATrack, u4ChunkSize);
                         u1HdrType = eSWDMX_MEDIA_SKIPPED;
                    }
                }
                else if (u1HdrType == eSWDMX_MEDIA_SUBTITLE)
                {
#if 0
                    UINT32 u4StartPts = 0, u4EndPts = 0, u4CurPts = 0;

                    u4CurPts = VDP_GetPts(prSwdmxInst->u1B2rId);
                    _SWDMX_AviPrsSubtitle(prSwdmxInst, (UCHAR*)rDmxMMData.u4StartAddr, &u4StartPts, &u4EndPts);
                    LOG(5, "SubTitle start pts = 0x%08x end pts = 0x%08x\n", u4StartPts, u4EndPts);
                    if (u4EndPts < (u4CurPts))
                    {
                        LOG(5, "Skip SubTitle Mode change PTS = 0x%08x\n", 
                            u4CurPts);
                        u1HdrType = eSWDMX_MEDIA_SKIPPED;
                    }  
#endif
                }
                else if (u1HdrType == eSWDMX_MEDIA_VIDEO)
                {
                     _SWDMX_AviCalculatePts(prSwdmxInst, u1HdrType, u1TrackIdx, u4ChunkSize);
                     //Skip other chunks
                     u1HdrType = eSWDMX_MEDIA_SKIPPED;
                }
                else
                {
                    if ((u1HdrType == eSWDMX_MEDIA_DRM_VIDEO) ||
                    	(u1HdrType == eSWDMX_MEDIA_DRM_AUDIO))
                    {
                        _SWDMX_AviPrsDrmChunk(prSwdmxInst, u1HdrType, rDmxMMData.u4StartAddr);
                    }

                    _SWDMX_AviCalculatePts(prSwdmxInst, u1HdrType, u1TrackIdx, u4ChunkSize);

                    // Skip other chunks
                    u1HdrType = eSWDMX_MEDIA_SKIPPED;
                }
            }
            else
            {   
                LOG(5, "prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8RecDmxMoviPos == prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos\n");
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8RecDmxMoviPos = 0;

                if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ChangeMode&AVI_CHANGE_AUDIO_MODE)
                {
                     if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)
                     {
                         AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]);
                     }
                }
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ChangeMode = 0;
            }
        }

        // Disable streams here
        if (u1HdrType == eSWDMX_MEDIA_AUDIO)
        {
            if((((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts>>32) != 0xFFFFFFFF) || 
                ((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts) != 0xFFFFFFFF))&&(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgMp3Abr))
            {
            	  if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts < (UINT32)(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts))
            	  {
            	  	   u1HdrType = eSWDMX_MEDIA_SKIPPED_AUDIO;
            	  }
            }
            u1TrackIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx;
            if (i4PlaySpeed != SWDMX_PLAY_SPEED_1X)
            {
                _SWDMX_AviCalculatePts(prSwdmxInst, u1HdrType, u1TrackIdx, u4ChunkSize);
            }
            else if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ChangeMode & AVI_CHANGE_AUDIO_MODE)
            {
                UINT32 u4ModeChangePts = VDP_GetPts(prSwdmxInst->u1B2rId);
                //prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ModeChangPts = VDP_GetPts(prSwdmxInst->u1B2rId);
                
                if (u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK)
                {
                    //if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx] >= (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ModeChangPts + 27000))
                    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx] >= (u4ModeChangePts + 18000))
                    {
                        LOG(5, "Audio Pts (0x%08x) > Mode ChangePts (0x%08x)\n", 
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx], 
                            u4ModeChangePts);

                        //prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ModeChangPts = 0xFFFFFFFF;
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ChangeMode = 0;
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx];
                        AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx]);
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudStartPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx];
                    }
                    else
                    {
                        _SWDMX_AviCalculatePts(prSwdmxInst, u1HdrType, u1TrackIdx, u4ChunkSize);
                        u1HdrType = eSWDMX_MEDIA_SKIPPED;
                    }
                }
            }
            else if((((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts>>32) != 0xFFFFFFFF) || 
                ((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts) != 0xFFFFFFFF) )||
                (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8AbRepeatAudPts != 0))
            {
                if((u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK) &&
                    ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx] < (UINT32)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts)||
                    ( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8AbRepeatAudPts != 0)))
                {
                    _SWDMX_AviCalculatePts(prSwdmxInst, u1HdrType, u1TrackIdx, u4ChunkSize);
                    u1HdrType = eSWDMX_MEDIA_SKIPPED;
                }
            }
            
            if(prRangeList->fgSetRenderPosition)
            {
                u1HdrType = eSWDMX_MEDIA_SKIPPED;
            }
        }
        else if ((u1HdrType == eSWDMX_MEDIA_DRM_VIDEO) ||
        	(u1HdrType == eSWDMX_MEDIA_DRM_AUDIO))
        {
            _SWDMX_AviPrsDrmChunk(prSwdmxInst, u1HdrType, rDmxMMData.u4StartAddr);
            u1HdrType = eSWDMX_MEDIA_SKIPPED;
        }

        if(((u1HdrType == eSWDMX_MEDIA_AUDIO) || 
          (u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO))&&
          (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->b_thumbnail_pb))
        {
            u1HdrType = eSWDMX_MEDIA_SKIPPED;
        }
    }
    
    if (u1HdrType == eSWDMX_MEDIA_SKIPPED)
    {
        *pu4ConsumeSize = u4ChunkSize + AVI_PARSER_OFFSET + i;
        if (fgRingBuf)
        {
            *pu4ConsumeSize += ((UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr)) - 
                                                               (UINT32)(pucOriginalPtr) - 
                                                               u4RingSize);
        }
        return eSWDMX_HDR_PRS_SUCCEED;
    }

  if (u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO)
    {
        if ((!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnAudio)&&
             (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnADmx))
        {
            *pu4ConsumeSize = u4ChunkSize + AVI_PARSER_OFFSET + i;
            if (fgRingBuf)
            {
                *pu4ConsumeSize += ((UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr)) - 
                                                                   (UINT32)(pucOriginalPtr) - 
                                                                   u4RingSize);
            }
            _SWDMX_AviCalculatePts(prSwdmxInst, u1HdrType, u1TrackIdx, u4ChunkSize);
            return eSWDMX_HDR_PRS_SUCCEED;
        }
    }

    // Get available size here (we don't have to query subtitle available size here)
    if ((u1HdrType == eSWDMX_MEDIA_VIDEO) ||
        ((u1HdrType == eSWDMX_MEDIA_AUDIO) && (i4PlaySpeed == SWDMX_PLAY_SPEED_1X)))
    {
        UINT8 u1PidIdx;
        UINT8 u1PidType = 0;

        if(u1HdrType == eSWDMX_MEDIA_VIDEO)
        {
            #ifdef CC_3D_MM_DS_SUPPORT
            if(ENABLE_NEWTRACK(prSwdmxInst))
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsLeftVideo)
                {
                    u1PidIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid;
                }
                else
                {
                    u1PidIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1Vid2Pid;
                }
            }
            else
            #endif
            {
                 u1PidIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid;
            }
        }
        else
        {
            u1PidIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid;
        }
        
        u1PidType = (u1HdrType == eSWDMX_MEDIA_VIDEO) ? (DMX_PID_TYPE_ES_VIDEO) : (DMX_PID_TYPE_ES_AUDIO);

        u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, 
                                                                                (DMX_PID_TYPE_T)u1PidType,
                                                                                u1PidIdx);
        if ((u1HdrType == eSWDMX_MEDIA_AUDIO) &&
            (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo)&&
            (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgQAud) &&
            (u4ChunkSize > u4DmxAvailSize) &&
            (u4ChunkSize > (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AFifoSize/2)) &&
            (u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK))
        {
            _SWDMX_AviCalculatePts(prSwdmxInst, u1HdrType, u1TrackIdx, u4ChunkSize);

            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgQAud = TRUE;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudAddr = rDmxMMData.u4StartAddr;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudSize = u4ChunkSize;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx];
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack = u1TrackIdx;
            
            *pu4ConsumeSize = u4ChunkSize + AVI_PARSER_OFFSET + i;
            if (fgRingBuf)
            {
                *pu4ConsumeSize += ((UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr)) - 
                                                               (UINT32)(pucOriginalPtr) - 
                                                               u4RingSize);
            }

            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsBlock = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4BlockChunkSize = 0;

            return eSWDMX_HDR_PRS_SUCCEED;
        }

        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgQAud)
        {
            u4DmxAudAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, (DMX_PID_TYPE_T)(DMX_PID_TYPE_ES_AUDIO),prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid);
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudSize < u4DmxAudAvailSize)
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ADecType == ENUM_SWDMX_ADEC_AAC)
	            {
	                if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack < AVI_DIVX_MAX_AUD_TRACK)
	                {
	               	    AviHandAAC(prSwdmxInst, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudSize,prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudPts,prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack);
	                }
	            }
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack < AVI_DIVX_MAX_AUD_TRACK)
                {
                    u1AudPid= 
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack] ;
                }
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack < AVI_DIVX_MAX_AUD_TRACK)
                {
                    u4Pts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack];
                }
                else
                {
                    u4Pts = 0;
                }

                rDmxMMData.u1Idx = u1AudPid;
                rDmxMMData.u4BufStart = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                rDmxMMData.u4BufEnd = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr));
                rDmxMMData.u4StartAddr = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudAddr;
                rDmxMMData.u4FrameSize = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudSize;
                rDmxMMData.u4Pts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudPts;
                
                LOG(5, "Send Q Audio Data Size = 0x%08x pts = 0x%08x\n", u4ChunkSize, rDmxMMData.u4Pts);

                //if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
                if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
                {
                    LOG(0, "Demuxer fails to move data.\n");
                    *pu4ConsumeSize = 0;
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                }

                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsBlock = TRUE;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4BlockChunkSize = u4ChunkSize + AVI_PARSER_OFFSET;
                *pu4ConsumeSize = 0;

                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgQAud = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudAddr = 0;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudSize = 0;
                
                return eSWDMX_HDR_PRS_SUCCEED;
            }
           else if(u1HdrType == eSWDMX_MEDIA_AUDIO)
            {
                *pu4ConsumeSize = 0;
                x_thread_delay(1);
                return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
            }
        }
                
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause
            && _fgBiAviAudioReady)
        {
            _SWDMX_AviVideoWakeUp(prSwdmxInst);
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause = FALSE;
        }
#endif

        if (u4ChunkSize > u4DmxAvailSize)
        {    
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause)
            {
                _SWDMX_AviVideoWakeUp(prSwdmxInst);
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause = FALSE;
            }
            
            if ((u1HdrType == eSWDMX_MEDIA_AUDIO) &&
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
                !_fgBiAviAudioReady &&
#endif
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAudIsReady)
            {
                AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                STC_StartStc(prSwdmxInst->u1StcId);
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAudIsReady = FALSE;
            }

            prSwdmxInst->fgFifoFull = TRUE;
                        
            LOG(7, "Chunk is blocked u1HdrType = %ld, u4ChunkSize = %ld, u4DmxAvailSize = %ld\n", 
                u1HdrType, 
                u4ChunkSize,
                u4DmxAvailSize);
            *pu4ConsumeSize = 0;
            x_thread_delay(1);
            return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
        }

            _SWDMX_InitDmx(prSwdmxInst, u1HdrType, prSwdmxInst->u1SwdmxId, NULL, FALSE);
    }
    if (u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO)
    {
        _SWDMX_AviCalculatePts(prSwdmxInst, u1HdrType, u1TrackIdx, u4ChunkSize);
    }
    else if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        u1TrackIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx;

        if (u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK)
        {
            BOOL fgRet;
            UINT32 u4CurPts, u4APts;
            
            UCHAR *pBuf = (UCHAR*)(rDmxMMData.u4StartAddr);
            UINT32 u4TimeSeekAudPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx];

            if (((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ADecType == ENUM_SWDMX_ADEC_MP3) || (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ADecType == ENUM_SWDMX_ADEC_MPEG)) &&
                (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAudIsLoaded) &&
                (pBuf[0] != 0xFF) &&
                ((pBuf[1] & 0xF0) != 0xF0))
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgBadAudio = TRUE;
            }
                
            _SWDMX_AviCalculatePts(prSwdmxInst, u1HdrType, u1TrackIdx, u4ChunkSize);
	          prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4NumSentAud++;
            u4Pts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx];
            
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRecSpeedChangeForAud)
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4RecPrevSpeedForAud == 2000 || 
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4RecPrevSpeedForAud == 1||
                    ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4RecPrevSpeedForAud > 1)&&
                    (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4RecPrevSpeedForAud<1000)))
                {
                    u4CurPts = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo) ? (VDP_GetPts(prSwdmxInst->u1B2rId)) : (STC_GetStcValue(prSwdmxInst->u1StcId));
                    
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = u4CurPts + 27000;

                    fgRet = DMX_AudHandler_SeekAndUpdate(
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[u1TrackIdx],
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts, &u4APts);

                    LOG(5, "Find Audio Track Data for 2X -> 1X\n");

                    if (fgRet)
                    {
                        LOG(5, "Find audio in the pre-buffer\n");
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = u4APts;
                    }
                    else
                    {
                        LOG(5, "Can't find audio in the pre-buffer\n");
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx];
                    }

                    AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts);
                    LOG(5, "prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = 0x%08x\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts);
                }
                else
                {
                    UNUSED(DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[u1TrackIdx]));
                }

                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgRecSpeedChangeForAud = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4RecPrevSpeedForAud = i4PlaySpeed;
                DMX_AudHandler_SetActivePidx(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[u1TrackIdx]);
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSetActivePid = TRUE;
            }

            if (((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ADecType == ENUM_SWDMX_ADEC_MP3)||
                (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ADecType == ENUM_SWDMX_ADEC_MPEG))&&
                (! prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgGetMp3Status)&&!(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgMp3Abr))
            {
                 UCHAR *puMp3Hdr =  (UCHAR*)rDmxMMData.u4StartAddr;
                 UCHAR uParHdr[4]= {0};
                 UINT32  u4HeaderSize = 0;
                 UINT8 i = 0;
                 
                 prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudNotParseSize =
                     prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudNotParseSize +u4ChunkSize;

                 if((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudNotParseSize > prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudFrameSize)&&
                    (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudFrameSize != 0))
                 {
                     prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudNotParseSize -= prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudFrameSize;
                     puMp3Hdr = (UCHAR*) ((UINT32)puMp3Hdr + u4ChunkSize -prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudNotParseSize);
                     prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudFrameSize = 0;
                 }
                 while(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudNotParseSize > prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudFrameSize)
                 {
                     prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudNotParseSize -= prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudFrameSize;
                     
                      puMp3Hdr = (UCHAR*) ((UINT32)puMp3Hdr- u4HeaderSize + prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudFrameSize);
                      u4HeaderSize = 0;
                      
                      while(*puMp3Hdr == 0)
                      {
                          puMp3Hdr++;
                          
                           if((UINT32)puMp3Hdr >= VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                          {
                               puMp3Hdr = 
                                 (UCHAR*)((UINT32)puMp3Hdr - VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) + 
                                VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                          }
                          prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudNotParseSize --;
                      }

                     if((UINT32)puMp3Hdr >= VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                      {
                           puMp3Hdr = 
                                (UCHAR*)((UINT32)puMp3Hdr - VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) + 
                                VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                      }
                      for (i = 0; i<4; i++)
                      {
                      	  uParHdr[i] = *puMp3Hdr;
                      	  puMp3Hdr++;
                          u4HeaderSize ++;
                          
                          if((UINT32)puMp3Hdr >= VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                          {
                               puMp3Hdr = 
                                 (UCHAR*)((UINT32)puMp3Hdr - VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) + 
                                VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                          }
                      	
                      }
                      
                      if(!(MPEGA_IS_VALID_HDR(uParHdr)))
                      {
                          LOG(3, "illegal audio header \n");
                          break;
                      }

                      prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudFrameSize = dwMpA_GetFrmSz(uParHdr, &prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4Mp3Rate);

                      if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudFrameSize  == 0)
                      {
                           LOG(3, "illegal u4AudFrameSize\n");
                           prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgGetMp3Status = TRUE;
                           break;
                      }
                      
                     if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4Mp3LastRate !=0)&&
                      (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4Mp3LastRate != prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4Mp3Rate))
                     {
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgMp3Vbr = TRUE;
                     }    
                     prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4Mp3LastRate = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4Mp3Rate;
                 }

                 if((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgMp3Vbr) && (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[u1TrackIdx]))
                 {
                      if (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[u1TrackIdx]->b_is_VBR)
                      {
                           prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgMp3Abr = TRUE;
                      }      
                 }
            }

            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ADecType == ENUM_SWDMX_ADEC_AAC)
            {
            	  if(u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK)
            	  {
                      AviHandAAC(prSwdmxInst, u4ChunkSize,prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudPts,u1TrackIdx);
                }
            }

            //enable audio according to pts, need fix by aud index table
            if(((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts>>32) != 0xFFFFFFFF)
                || ((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts) != 0xFFFFFFFF))
            {
                if((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx] >= (UINT32)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts))
                {

                    AUD_SetStartPts(prSwdmxInst->u1AdecId, u4TimeSeekAudPts);
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudStartPts = u4TimeSeekAudPts;

                    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo)
                    {
                        VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = (UINT32)u4TimeSeekAudPts;
                
                        VDEC_Pause(prSwdmxInst->u1VdecId);
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause = TRUE;
                    }
                    
                    LOG(7, "Got Aud T PTS 0x%x, Seek 0x%x\n", 
                        u4Pts, (UINT32)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts);            
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts = 0xFFFFFFFF;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts <<= 32;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts += 0xFFFFFFFF;
                }
                else
                {
                    //skip audio data in seek process
                    *pu4ConsumeSize = u4ChunkSize + AVI_PARSER_OFFSET + i;
                    if (fgRingBuf)
                    {
                        *pu4ConsumeSize += ((UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr)) - 
                                                                   (UINT32)(pucOriginalPtr) - 
                                                                   u4RingSize);
                    }
                    return eSWDMX_HDR_PRS_SUCCEED;
                }
            }
            if((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVdecNotSupport)&&
                (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAudIsReady))
            {
                 AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                 STC_StartStc(prSwdmxInst->u1StcId);
                 prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAudIsReady = FALSE;
            }
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        UINT16 u2MaxQNum, u2CurQNum;
        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgQAud)
        {
        	u4DmxAudAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, (DMX_PID_TYPE_T)(DMX_PID_TYPE_ES_AUDIO),prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid);   
          if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudSize < u4DmxAudAvailSize)
          {
          	  if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ADecType == ENUM_SWDMX_ADEC_AAC)
	            {
	           	    if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack < AVI_DIVX_MAX_AUD_TRACK)
	           	    {
	              	   AviHandAAC(prSwdmxInst, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudSize,prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudPts, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack);
	                }
	            }

              if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack < AVI_DIVX_MAX_AUD_TRACK)
              {
                  u1AudPid= 
                      prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack] ;
              }
              if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack < AVI_DIVX_MAX_AUD_TRACK)
              {
                  u4Pts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudTrack];
              }
              else
              {
                  u4Pts = 0;
              }


              rDmxMMData.u1Idx = u1AudPid;
              rDmxMMData.u4BufStart = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
              rDmxMMData.u4BufEnd = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr));
              rDmxMMData.u4StartAddr = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudAddr;
              rDmxMMData.u4FrameSize = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudSize;
              rDmxMMData.u4Pts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudPts;
              
              LOG(5, "Send Q Audio Data Size = 0x%08x pts = 0x%08x\n", u4ChunkSize, rDmxMMData.u4Pts);

              //if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
              if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
              {
                  LOG(0, "Demuxer fails to move data.\n");
                  *pu4ConsumeSize = 0;
                  return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
              }

              prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsBlock = TRUE;
              prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4BlockChunkSize = u4ChunkSize + AVI_PARSER_OFFSET;
              *pu4ConsumeSize = 0;

              prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgQAud = FALSE;
              prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudAddr = 0;
              prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4QAudSize = 0;
              
              return eSWDMX_HDR_PRS_SUCCEED;
          }
        }
    
        
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo == FALSE)
        {
            *pu4ConsumeSize = u4ChunkSize + AVI_PARSER_OFFSET + i;
            if (fgRingBuf)
            {
                *pu4ConsumeSize += ((UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr)) - 
                                                                   (UINT32)(pucOriginalPtr) - 
                                                                   u4RingSize);
            }
             x_thread_delay(33);
            return eSWDMX_HDR_PRS_SUCCEED;
        }

#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
        if (_fgBiAviAudioReady)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause)
            {
                _SWDMX_AviVideoWakeUp(prSwdmxInst);
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause = FALSE;
            }
        }
#endif

        #ifdef CC_3D_MM_DS_SUPPORT
        if(ENABLE_NEWTRACK(prSwdmxInst))
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsLeftVideo)
            {
                VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
            }
            else
            {
                VDEC_GetQueueInfo(prSwdmxInst->u1Vdec2Id, &u2CurQNum, &u2MaxQNum);
            }
        }
        else
        #endif
        {
            VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
        }
        if (u2CurQNum > (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps * 3))
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause)
            {
                _SWDMX_AviVideoWakeUp(prSwdmxInst);
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause = FALSE;
            }
            LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
            *pu4ConsumeSize = 0;            
            prSwdmxInst->fgFifoFull = TRUE;
            
            x_thread_delay(1);
            
            return eSWDMX_HDR_PRS_SUCCEED;
        }
        #ifdef CC_3D_MM_DS_SUPPORT
        if(ENABLE_NEWTRACK(prSwdmxInst))
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsLeftVideo)
           {
                u4Pts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts;
            }
            else
            {
                u4Pts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSubVidPts;
            }
         }
         else
         #endif
         {
             u4Pts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts;
         }
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgNoCalPts = FALSE;

        // Record every video's chunk position.
        #ifdef CC_3D_MM_DS_SUPPORT
        if(ENABLE_NEWTRACK(prSwdmxInst))
        {
                if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsLeftVideo)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VidOffset = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos;
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8SubVidOffset = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos;
                }
         }
        else
        #endif
        {
             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VidOffset = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos;
        }
        if(((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts>>32) != 0xFFFFFFFF)
            || ((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts) != 0xFFFFFFFF))
        {
            if(u4Pts > (UINT32)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts)
            {
                if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgMp3Abr) &&
                    (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK))
                {
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx] = 
                        (UINT32)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts;
                }
                LOG(5, "Got Vid T PTS 0x%x, Seek 0x%x\n", 
                    u4Pts, (UINT32)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts); 
                
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAbRepeat = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts = 0xFFFFFFFF;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts <<= 32;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts += 0xFFFFFFFF;
            }

        }
        if((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8AbRepeatVidPts!=0)&&
        (u4Pts >prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8AbRepeatVidPts))
        {
            UINT32 i =0;
            for (i =0; (i<prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack)&&(i<AVI_DIVX_MAX_AUD_TRACK); i++)
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[i] = 
                       prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8AbRepeatAudPts;
            }
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8AbRepeatVidPts = 0;
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8AbRepeatAudPts = 0;
        }
        
        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAbRepeat || 
          (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbDmxPos != 0))
        {
            UINT32 u4StartAddr = 0;
            if((u4ChunkSize < AVI_MPEG4_NOT_CODEC_SIZE)&&
              (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAbRepeat))
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbPts = u4Pts;
            }
            else
            {
                if((u4ChunkSize < AVI_MPEG4_NOT_CODEC_SIZE)&&
                   (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAbOneMore))
                {
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbPts = u4Pts;
                    if(!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAbRepeat)
                    {
                        rDmxMMData.u4BufStart = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                	    rDmxMMData.u4BufEnd = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr));
                	    rDmxMMData.u4FrameSize = u4ChunkSize;
                	    rDmxMMData.u4Pts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbPts;
                	    rDmxMMData.u4Dts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbPts;
                        _SWDMX_AviCalculatePts(prSwdmxInst, u1HdrType, u1TrackIdx, u4ChunkSize);
                	    LOG(11, "Header Type = %ld u4FrameSize = 0x%08x pts = 0x%08x\n", 
                	        u1HdrType, u4ChunkSize, u4Pts);  
                	    //if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
                	    if(((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_WMV7) ||
					     (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_WMV8)) ||
					     (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_WMV9))
					    {
					         rDmxMMData.fgFrameHead = TRUE;
					    }
									    
                	    if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
                	    {
                	        LOG(0, "Demuxer fails to move data.\n");
                	        *pu4ConsumeSize = 0;
                	        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                	    }
                    }
                    *pu4ConsumeSize = u4ChunkSize + AVI_PARSER_OFFSET + i;
                    if (fgRingBuf)
                    {
                        *pu4ConsumeSize += ((UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr)) - 
                                                                           (UINT32)(pucOriginalPtr) - 
                                                                           u4RingSize);
                    }
                    _SWDMX_AviCalculatePts(prSwdmxInst, u1HdrType, u1TrackIdx, u4ChunkSize);
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAbOneMore = TRUE;
                    return eSWDMX_HDR_PRS_SUCCEED;
                }
                u4StartAddr = rDmxMMData.u4StartAddr;
                if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbDmxPos != 0)
                {
                    #ifdef CC_3D_MM_DS_SUPPORT
                    if(ENABLE_NEWTRACK(prSwdmxInst))
                    {
                        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsLeftVideo)    
                        {
                            rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid;
                        }
                        else
                        {
                             rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1Vid2Pid;
                        }
                    }
                    else
                    #endif
                    {
                        rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid;
                    }
                    
                    if(((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_WMV7) ||
				     (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_WMV8)) ||
				     (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_WMV9))
				    {
				         rDmxMMData.fgFrameHead = TRUE;
				    }
                    rDmxMMData.u4BufStart = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                    rDmxMMData.u4BufEnd = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr));
                    rDmxMMData.u4StartAddr = (UINT32)(VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbDmxPos));
                    rDmxMMData.u4FrameSize = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbChunkSize;
                    rDmxMMData.u4Pts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbPts;
                    rDmxMMData.u4Dts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbPts;

                    if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
                    {
                        LOG(0, "Demuxer fails to move data.\n");
                        *pu4ConsumeSize = 0;
                        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                    } 
                }
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbChunkSize = u4ChunkSize;
        	    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbDmxPos = u4StartAddr;
        	    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbPts = u4Pts;
                rDmxMMData.u4StartAddr = u4StartAddr;

                if(!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAbRepeat)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbChunkSize = 0;
        	        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbDmxPos = 0;
        	        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AbPts = 0;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAbOneMore = FALSE;
                }
            }
        }
        
    }
    else if (u1HdrType == eSWDMX_MEDIA_SUBTITLE)
    {
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4NumSentSub++;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8PrevSubPos = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecVidPtsForSub = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts;
        for (j = 0; (j < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack) && (j < AVI_DIVX_MAX_AUD_TRACK); j++)
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4RecAudPtsForSub[j] = 
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[j] ;
            LOG(5, "Avi Seek Audio pts = 0x%08x\n", 
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[j]);

        }
    }

    if ((u1HdrType == eSWDMX_MEDIA_AUDIO) ||
        (u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO))
    {
        if (u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK)
        {
            u1AudPid = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[u1TrackIdx];
        }
        if (u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK)
        {
            u4Pts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx];
        }
        else
        {
            u4Pts = 0;
        }
    }

    if (prRangeList->prDmxRangeInfo->b_drm)
    {
        UINT32 u4Flag;
        DMX_MM_DRM_AES_T rDRM = {0};
        DIVX_DRM_CRYPTO_INFO_T *prDrmInfo;

        UINT8 u1VidPidIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid;
        
        rDRM.fgCBC = FALSE;
        rDRM.u4KeyBitLen = AVI_DRM_AES_SIZE_128;

        prDrmInfo = 
            prRangeList->prDmxRangeInfo->t_drm_info_drv.u.t_divx_drm_info_drv.pt_divx_drm_crypto_info;
        
        LOG(6, "Drm Type = %ld, DRM key idx = %ld\n",  
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1DrmType, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u2FrameKeyIdx);
        
        if ((u1HdrType == eSWDMX_MEDIA_VIDEO) &&
            (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1DrmType == eSWDMX_MEDIA_DRM_VIDEO) &&
            (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4EncryptLength != 0) &&
            (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u2FrameKeyIdx < 128))
        {
            rDRM.u4Offset = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4EncryptOffset;
            rDRM.u4EncryLen = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4EncryptLength;
            
            x_memcpy(rDRM.au1Key, 
                prDrmInfo->aui1_frame_keys[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u2FrameKeyIdx],
                (AVI_DRM_AES_SIZE_128/8));

            u4Flag = DMX_MM_DRM_FLAG_ALL;
            DMX_MM_SetDRMMode(u1VidPidIdx, DMX_DRM_MODE_AES);
            DMX_MM_SetAES(u1VidPidIdx, u4Flag , &rDRM);
        }
        else if (((u1HdrType == eSWDMX_MEDIA_AUDIO) ||(u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO)) &&
            (prDrmInfo->ui1_flag & DIVX_DRM_AUDIO_PROTECTION))
        {
            if (u4ChunkSize < prDrmInfo->ui1_protected_audio_offset)
            {
                LOG(1, "Audio DRM offset is larger than Audio Chunk Size Impossible\n");
            }
            else
            {

                if (u4ChunkSize < prDrmInfo->ui1_protected_audio_offset + prDrmInfo->ui1_protected_audio_crypto_size)
                {
                    rDRM.u4EncryLen = u4ChunkSize - prDrmInfo->ui1_protected_audio_offset;
                }
                else
                {
                    rDRM.u4EncryLen = prDrmInfo->ui1_protected_audio_crypto_size;
                }
                rDRM.u4Offset = prDrmInfo->ui1_protected_audio_offset;
                
                x_memcpy(rDRM.au1Key, 
                    prDrmInfo->aui1_frame_keys[0],
                    (AVI_DRM_AES_SIZE_128/8));

                u4Flag = DMX_MM_DRM_FLAG_ALL;
                DMX_MM_SetDRMMode(u1AudPid, DMX_DRM_MODE_AES);
                DMX_MM_SetAES(u1AudPid, u4Flag , &rDRM);
            }
        }
        LOG(6, "Drm u4Offset = %ld, DRM u4EncryLen = %ld\n",  
            rDRM.u4Offset, rDRM.u4EncryLen);

        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1DrmType = eSWDMX_MEDIA_NONE;
    }

    if ((u1HdrType == eSWDMX_MEDIA_AUDIO) ||
        (u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO))
    {
    
        if (u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK)
        {
            u1AudPid = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[u1TrackIdx];
        }
        if (u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK)
        {
            u4Pts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx];
        }
        else
        {
            u4Pts = 0;
        }
    }

    switch (u1HdrType)
    {
        case eSWDMX_MEDIA_VIDEO:
            #ifdef CC_3D_MM_DS_SUPPORT
            if(ENABLE_NEWTRACK(prSwdmxInst))
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsLeftVideo)
                {
                    rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid;
                }
                else
                {
                    rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1Vid2Pid;
                }
            }
            else
            #endif
            {
                rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid;
            }
            break;
        case eSWDMX_MEDIA_AUDIO:
        case eSWDMX_MEDIA_SKIPPED_AUDIO:
            rDmxMMData.u1Idx = u1AudPid;
            break;
        // To Do
        case eSWDMX_MEDIA_SUBTITLE:
             rDmxMMData.u1Idx =prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1SubPid;
             break;
        default:
            LOG(3, "Wrong Pid\n");
            return eSWDMX_HDR_PRS_FAIL;
    }
    if(((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_WMV7) ||
     (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_WMV8)) ||
     (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_WMV9))
    {
         rDmxMMData.fgFrameHead = TRUE;
    }
   
    if(!((u1HdrType == eSWDMX_MEDIA_VIDEO)&&
        (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAbRepeat)))
    { 
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgNoCalPts = FALSE;
	    rDmxMMData.u4BufStart = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
	    rDmxMMData.u4BufEnd = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr));
	    rDmxMMData.u4FrameSize = u4ChunkSize;
	    rDmxMMData.u4Pts = u4Pts;
	    rDmxMMData.u4Dts = u4Pts;
	    LOG(11, "Header Type = %ld u4FrameSize = 0x%08x pts = 0x%08x\n", 
	        u1HdrType, u4ChunkSize, u4Pts);  
	    //if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
	    if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
	    {
	        LOG(0, "Demuxer fails to move data.\n");
	        *pu4ConsumeSize = 0;
	        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
	    }
	}

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSendVolInfo = TRUE;
         _SWDMX_AviCalculatePts(prSwdmxInst, u1HdrType, u1TrackIdx, u4ChunkSize);
        if( (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts != 0)&&
            (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidHeaderPtr == 0)&&
            (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4NumSentVid == 0))
        {
             UNUSED(_SWDMX_AviSeek(prSwdmxInst, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts, prRangeList));
             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VSeekPts = 
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts;
             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ASeekPts = 
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts;
             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid = TRUE;
        }
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4NumSentVid++;
    }

    *pu4ConsumeSize = u4ChunkSize + AVI_PARSER_OFFSET + i;
    if (fgRingBuf)
    {
        *pu4ConsumeSize += ((UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr)) - 
                                                           (UINT32)(pucOriginalPtr) - 
                                                           u4RingSize);
    }

    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4ErrorData = 0;
    
    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsBlock == TRUE)
    {
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsBlock = FALSE;
    }
    
    return eSWDMX_HDR_PRS_SUCCEED;
}

static BOOL _SWDMX_AviGetDataFromFeeder(
    SWDMX_INFO_T* prSwdmxInst, 
    FEEDER_REQ_DATA_T *prFeederData,
     UINT8 u1VdecId)
{
    SWDMX_AVI_VERIFY_NULL_ERROR(prSwdmxInst);
        
    if (!_SWDMX_ReceiveFeederAck(prSwdmxInst->u1SwdmxId, (VOID*)prFeederData))
    {
        LOG(3, "_SWDMX_AviGetDataFromFeeder Ack Fail!!!\n");

        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid = TRUE;
        //ASSERT(0);
        return FALSE;        
    }

    if (prFeederData->eDataType == FEEDER_SOURCE_INVALID)
    {
        LOG(3, "_SWDMX_AviGetDataFromFeeder Timeout!!!\n");
        if ((prFeederData->u4Id + 1) == prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqID)
        {
        	  prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid = TRUE;
        }
        else
        {
                  if(u1VdecId == prSwdmxInst->u1VdecId)
                  {
        	        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgReqDataMiss = TRUE;
                  }
                  #ifdef CC_3D_MM_DS_SUPPORT
                  if(ENABLE_NEWTRACK(prSwdmxInst))
                  {
                      if( u1VdecId == prSwdmxInst->u1Vdec2Id)
                      {
                          prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgReqSubDataMiss = TRUE;
                      }
                  }
                  #endif
        }
        return FALSE;
    }

    if (prFeederData->u4Id != prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqReceiveID)
    {
        LOG(3, "Feeder ID mismatch!!! feeder id = %ld\n", prFeederData->u4Id);
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid = TRUE;
        return FALSE;
    }
     if(u1VdecId == prSwdmxInst->u1VdecId)
    {
         prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgReqDataMiss = FALSE;
     }
     #ifdef CC_3D_MM_DS_SUPPORT
     if(ENABLE_NEWTRACK(prSwdmxInst))
     {
         if( u1VdecId == prSwdmxInst->u1Vdec2Id)
          {
              prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgReqSubDataMiss = FALSE;
          }
      }
     #endif
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4FeederReqReceiveID++;
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgFeederInvalid = FALSE;
    return TRUE;
}

static BOOL _CheckAviChunkType(SWDMX_INFO_T *prSwdmxInst,
    UCHAR *pucBuf, 
    UINT8 *pu1HdrType, 
    UINT8 *pu1TrackIdx)
{
    INT32 i;

    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo)
    {
        if(_CheckAviChunkIndex(pucBuf, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VStrmID) == TRUE)
        {
            *pu1HdrType = eSWDMX_MEDIA_VIDEO;
             #ifdef CC_3D_MM_DS_SUPPORT
             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsLeftVideo = TRUE;
             #endif
             return TRUE;
        }
        #ifdef CC_3D_MM_DS_SUPPORT
        if(ENABLE_NEWTRACK(prSwdmxInst))
        {
            if(_CheckAviChunkIndex(pucBuf, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VSubStrmID) == TRUE)
            {
                *pu1HdrType = eSWDMX_MEDIA_VIDEO;
                 prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsLeftVideo = FALSE;
                 return TRUE;
             }
         }
        #endif
    }
    
    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnSubTitle)
    {
        if (_CheckAviChunkIndex(pucBuf, 
             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SpStrmID) == TRUE)
        {
            *pu1HdrType = eSWDMX_MEDIA_SUBTITLE;
            return TRUE;
        }
    }
    
    if ((_CheckAviChunkIndex(pucBuf, AVI_CHUNK_ID_JUNK) == TRUE) /*||
        (_CheckAviChunkIndex(pucBuf, ((AVI_IDX_CHUNK_ID) | (pucBuf[2] << 16) | (pucBuf[3] << 24)))== TRUE)*/)
    {
        *pu1HdrType = eSWDMX_MEDIA_SKIPPED;
        return TRUE;
    }
    else if (_CheckAviChunkIndex(pucBuf, ((AVI_UNCOMPRE_DATA) | (pucBuf[0]) | (pucBuf[1] << 8)))== TRUE)
    {
        UCHAR ucStreamID[2];
  
        ucStreamID[1] = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VStrmID >> 8) & 0xff;
        ucStreamID[0] = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VStrmID >> 0) & 0xff;

        if (pucBuf[0] == ucStreamID[0] && pucBuf[1] == ucStreamID[1])
        {
            *pu1HdrType = eSWDMX_MEDIA_VIDEO;
            return TRUE;
        }

        ucStreamID[1] = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AStrmID >> 8) & 0xff;
        ucStreamID[0] = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AStrmID >> 0) & 0xff;

        if (pucBuf[0] == ucStreamID[0] && pucBuf[1] == ucStreamID[1])
        {
            *pu1HdrType = eSWDMX_MEDIA_AUDIO;
            return TRUE;
        }
    }
    else if (_CheckAviChunkIndex(pucBuf, ((AVI_VID_COMPRE_DATA) | (pucBuf[0]) | (pucBuf[1] << 8))) == TRUE)
    {
        UCHAR ucStreamID[2];

	ucStreamID[1] = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VStrmID >> 8) & 0xff;
	ucStreamID[0] = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VStrmID >> 0) & 0xff;
	
	if ((pucBuf[0] == ucStreamID[0] && pucBuf[1] == ucStreamID[1]) &&
	    (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo))
	{
           *pu1HdrType = eSWDMX_MEDIA_VIDEO;
	   return TRUE;
	}
    }
    else if (_CheckAviChunkIndex(pucBuf, ((AVI_DRM_VID_CHUNK_ID) | (pucBuf[0]) | (pucBuf[1] << 8)))== TRUE)
    {
        UCHAR ucStreamID[2];
  
        ucStreamID[1] = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VStrmID >> 8) & 0xff;
        ucStreamID[0] = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VStrmID >> 0) & 0xff;

        if (pucBuf[0] == ucStreamID[0] && pucBuf[1] == ucStreamID[1])
        {
            *pu1HdrType = eSWDMX_MEDIA_DRM_VIDEO;
            return TRUE;
        }
        
        ucStreamID[1] = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AStrmID >> 8) & 0xff;
        ucStreamID[0] = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AStrmID >> 0) & 0xff;

        if (pucBuf[0] == ucStreamID[0] && pucBuf[1] == ucStreamID[1])
        {
            *pu1HdrType = eSWDMX_MEDIA_DRM_AUDIO;
            return TRUE;
        }
    }

    for (i = 0; (i < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack) && (i < AVI_DIVX_MAX_AUD_TRACK); i++)
    {
        if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgKeyExist)&&(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[i]))
        {
            if (_CheckAviChunkIndex(pucBuf, 
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[i]->t_strm_id.u.t_stm_avi_id) == TRUE)
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnAudio && prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx == i)
                {
                    *pu1HdrType = eSWDMX_MEDIA_AUDIO;
                }
                else 
                {
                    *pu1HdrType = eSWDMX_MEDIA_SKIPPED_AUDIO;
                }

                if (pu1TrackIdx)
                {
                    *pu1TrackIdx = i;
                }

                return TRUE;
           }
        }
        else
        {   
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prAudIdxTbl[i])
                {
                    if (_CheckAviChunkIndex(pucBuf, 
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prAudIdxTbl[i]->t_strm_id.u.t_stm_avi_id) == TRUE)
                    {
                        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnAudio &&(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx == i))
                        {
                            *pu1HdrType = eSWDMX_MEDIA_AUDIO;
                        }
                        else
                        {
                            *pu1HdrType = eSWDMX_MEDIA_SKIPPED_AUDIO;
                        }
                        
                        if (pu1TrackIdx)
                        {
                            *pu1TrackIdx = i;
                        }
                        return TRUE;
                    }
                }
            }
        }
    }

    /* skip all other video & Audio ... */ 
    if (_CheckAviOtherChunk(pucBuf) == TRUE)
    {
        *pu1HdrType = eSWDMX_MEDIA_SKIPPED;
        return TRUE;
    }
    /*==============================*/

    return FALSE;
}

static BOOL _CheckAviOtherChunk(UCHAR *pucBuf)
{
    INT32 i4StreamNum = 0;
    
    // Skip Chunks here
    if (((pucBuf[2] == 'd') && (pucBuf[3] == 'c')) || 
        ((pucBuf[2] == 'w') && (pucBuf[3] == 'b')) ||
        ((pucBuf[2] == 's') && (pucBuf[3] == 'b'))
        )
    {
        if ((pucBuf[0] >= '0' && pucBuf[0] <= '1') && 
            (pucBuf[1] >= '0' && pucBuf[1] <= '9'))
        {
            i4StreamNum = (INT32)((pucBuf[0] - '0') * 10 + (pucBuf[1] - '0'));
            // Max 17 streams
            if (i4StreamNum < 17)
            {
                return TRUE;
            }
        }
    }
    else if ((pucBuf[0] == 'i') && (pucBuf[1] == 'x'))
    {
        if ((pucBuf[2] >= '0' && pucBuf[2] <= '1') && 
            (pucBuf[3] >= '0' && pucBuf[3] <= '9'))
        {
            i4StreamNum = (INT32)((pucBuf[2] - '0') * 10 + (pucBuf[3] - '0'));
            // Max 17 streams
            if (i4StreamNum < 17)
            {
                return TRUE;
            }
        }
    }
    else if ((pucBuf[0] == 'i') && (pucBuf[1] == 'd') &&
                (pucBuf[2] == 'x') && (pucBuf[3] == '1'))
    {
        return TRUE;
    }

    return FALSE;
}

static BOOL _CheckAviChunkIndex(UCHAR *pucBuf, UINT32 u4StrmID)
{
    UINT32 u4CmpID = 0;

    u4CmpID = ((UINT32)(pucBuf[0])) |
                       ((UINT32)(pucBuf[1]) << 8) |
                       ((UINT32)(pucBuf[2]) << 16) |
                       ((UINT32)(pucBuf[3]) << 24);
    if (u4CmpID != u4StrmID)
    {
        return FALSE;
    }
    
    return TRUE;
}

#if 0
static UINT32 _SWDMX_AviAsci2Decimal(UINT8*   aui1_ary,
                                       UINT8    ui1_round)
{
    UINT32  ui4_result = 0;
    UINT32  ui4_multiply = 1;
    while(ui1_round)
    {
        ui1_round -= 1;
        ui4_result += (aui1_ary[ui1_round]-'0')*ui4_multiply;
        ui4_multiply *= 10;
    }

    return ui4_result;
}


static void _SWDMX_AviPrsSubtitle(SWDMX_INFO_T *prSwdmxInst,
                                UCHAR*    pucBuf,
                                 UINT32*                 pt_pts_start,
                                 UINT32*                 pt_pts_end)
{
    UINT32  ui4_hour;
    UINT32  ui4_min;
    UINT32  ui4_sec;
    UINT32  ui4_ms;

    UCHAR ucSubBuf[26];

    if ((UINT32)pucBuf <= (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) - 26))
    {
        x_memcpy(ucSubBuf, (void*)pucBuf, 26);
    }
    else
    {
        UINT32 u4Temp = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) -(UINT32)pucBuf);
        
        x_memcpy(ucSubBuf, (void*)pucBuf, u4Temp);
        x_memcpy(&ucSubBuf[u4Temp], 
                           (void*)VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr), 
                           (26 - u4Temp)); 
    }

    ui4_hour = _SWDMX_AviAsci2Decimal(&ucSubBuf[1], 2);
    ui4_min = _SWDMX_AviAsci2Decimal(&ucSubBuf[4], 2);
    ui4_sec = _SWDMX_AviAsci2Decimal(&ucSubBuf[7], 2);
    ui4_ms = _SWDMX_AviAsci2Decimal(&ucSubBuf[10], 3);

    *pt_pts_start = ((90)*ui4_ms)
                    + ((90000)*ui4_sec)
                    + ((90000)*ui4_min*60)
                    + ((90000)*ui4_hour*60);

    ui4_hour = _SWDMX_AviAsci2Decimal(&ucSubBuf[14], 2);
    ui4_min = _SWDMX_AviAsci2Decimal(&ucSubBuf[17], 2);
    ui4_sec = _SWDMX_AviAsci2Decimal(&ucSubBuf[20], 2);
    ui4_ms = _SWDMX_AviAsci2Decimal(&ucSubBuf[23], 3);

    *pt_pts_end = ((90)*ui4_ms)
                    + ((90000)*ui4_sec)
                    + ((90000)*ui4_min*60)
                    + ((90000)*ui4_hour*60);
    /*
    *pt_pts_end = (UINT64)((90000)*(UINT64)ui4_ms/1000)
                    + (UINT64)((90000)*(UINT64)ui4_sec)
                    + (UINT64)((90000)*(UINT64)ui4_min*60)
                    + (UINT64)((90000)*(UINT64)ui4_hour*60);*/
}
#endif

static void _SWDMX_AviPrsDrmChunk(SWDMX_INFO_T* prSwdmxInst, UINT8 u1HdrType, UINT32 u4StartAddr)
{
    UCHAR ucDrmBuf[AVI_DRM_SIZE];

    if (u4StartAddr <= (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) - AVI_DRM_SIZE))
    {
        x_memcpy(ucDrmBuf, (void*)u4StartAddr, AVI_DRM_SIZE);
    }
    else
    {
        UINT32 u4Temp = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) -u4StartAddr);
        
        x_memcpy(ucDrmBuf, (void*)u4StartAddr, u4Temp);
        x_memcpy(&ucDrmBuf[u4Temp], 
                           (void*)VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr), 
                           (AVI_DRM_SIZE - u4Temp)); 
    }

    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1DrmType = u1HdrType;
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u2FrameKeyIdx = (UINT16)((ucDrmBuf[0]) |
                                                                         (ucDrmBuf[1] << 8));

    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4EncryptOffset = (ucDrmBuf[2] |
    								(ucDrmBuf[3] << 8) | 
    								(ucDrmBuf[4] << 16) | 
    								(ucDrmBuf[5] << 24));


    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4EncryptLength = (ucDrmBuf[6] | 
    								 (ucDrmBuf[7] << 8) |
    								 (ucDrmBuf[8] << 16) | 
    								 (ucDrmBuf[9] << 24));

    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u2FrameKeyIdx >= AVI_DRM_MAX_NUM_KEY)
    {
        LOG(3, "DRM Error Key Index is too large\n");
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1DrmType = eSWDMX_MEDIA_NONE;
    }

    LOG(9, "Drm Type = %ld\n", u1HdrType);
    LOG(9, "Drm Frame Key Index = %ld\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u2FrameKeyIdx);
    LOG(9, "Drm Frame Encrypt Offset = %ld\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4EncryptOffset);
    LOG(9, "Drm Frame Encrypt Length = %ld\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4EncryptLength);
}


static BOOL _SWDMX_InitDmx(SWDMX_INFO_T *prSwdmxInst, UINT8 u1HdrType, UINT8 u1Index,VOID* pfnCbFunc, BOOL fgEos)
{
    BOOL fgRet = TRUE;
    UINT32 u4Flag = 0; 
    DMX_MM_T rDmxMMInfo;
    UINT8 u1VidPid=0;
    BOOL fgVDmxEnabled=FALSE;
    x_memset(&rDmxMMInfo, 0, sizeof(DMX_MM_T));

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        UINT8 u1DmxPid = 0xFF;
        UINT32 u4VidFifoAddr, u4VidFifoSize;

        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;
        #ifdef CC_3D_MM_DS_SUPPORT
        if(IS_NEWTRACK(prSwdmxInst,u1Index))
        {
           fgVDmxEnabled = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnV2Dmx;
        }
        else
        #endif
        {
           fgVDmxEnabled = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVDmx;
        }
        if (!fgVDmxEnabled)
        {

            DMX_SetToDecoder(TRUE);

            u1VidPid = DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);

            if( u1VidPid != 0xFF)
            {
                u1DmxPid = u1VidPid;
            }
            else
            {
                LOG( 3,"get  video pid fail\n");
            }


            #ifdef CC_3D_MM_DS_SUPPORT
            if(ENABLE_NEWTRACK(prSwdmxInst))
            {
                if(IS_NEWTRACK(prSwdmxInst,u1Index))
                {
                    _SWDMX_GetVideo2Fifo(prSwdmxInst->u1SwdmxId, &u4VidFifoAddr, &u4VidFifoSize);
                   prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1Vid2Pid = u1VidPid;
                   rDmxMMInfo.u1ChannelId = prSwdmxInst->u1Vdec2Id;
                }
                else
                {
                    _SWDMX_GetVideoFifo(prSwdmxInst->u1SwdmxId, &u4VidFifoAddr, &u4VidFifoSize);
                   prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid = u1VidPid;
                   rDmxMMInfo.u1ChannelId = prSwdmxInst->u1VdecId;
                }
            }
            else
            #endif
            {
                _SWDMX_GetVideoFifo(prSwdmxInst->u1SwdmxId, &u4VidFifoAddr, &u4VidFifoSize);
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid = u1VidPid;
                rDmxMMInfo.u1ChannelId = prSwdmxInst->u1VdecId;
            }

            u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_VALID |
                DMX_MM_FLAG_BUF | DMX_MM_FLAG_INSTANCE_TAG |
                DMX_MM_FLAG_DEVICE_ID);

            rDmxMMInfo.fgAllocBuf = FALSE;
            //rDmxMMInfo.u4BufAddr = prFbmPool->u4Addr;
            //rDmxMMInfo.u4BufSize = prFbmPool->u4Size;
            rDmxMMInfo.u4BufAddr          = u4VidFifoAddr ;
            rDmxMMInfo.u4BufSize          = u4VidFifoSize;
            rDmxMMInfo.pvInstanceTag = (void*)prSwdmxInst;
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType ==
                (UINT32)ENUM_SWDMX_VDEC_MJPEG)
            {
                rDmxMMInfo.u1DeviceId = VLD1; // video format all use VLD expect jpeg
            }
            else
            {        
                rDmxMMInfo.u1DeviceId = VLD0; // video format all use VLD expect jpeg
            }
            u4Flag |= DMX_MM_FLAG_SEARCH_START_CODE;

            switch (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType)
            {
                case ENUM_SWDMX_VDEC_DX311: case ENUM_SWDMX_VDEC_WMV7:
                case ENUM_SWDMX_VDEC_WMV8: case ENUM_SWDMX_VDEC_WMV9:
                case ENUM_SWDMX_VDEC_MJPEG: case ENUM_SWDMX_VDEC_H263:
                    LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = FALSE\n");
                    rDmxMMInfo.fgSearchStartCode = FALSE;
                    break;
                default:
                    LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = TRUE\n");
                    rDmxMMInfo.fgSearchStartCode = TRUE;      
                    break;
            }
            fgRet = DMX_MM_Set(u1DmxPid, u4Flag, &rDmxMMInfo);
        }
        else if (fgEos)
        {
            #ifdef CC_3D_MM_DS_SUPPORT
             if(IS_NEWTRACK(prSwdmxInst,u1Index))
            {
                u1VidPid = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1Vid2Pid;
            }
            else
            #endif
            {
                u1VidPid = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid;
            }
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;
            u1DmxPid = u1VidPid;
            fgRet = DMX_MM_Set(u1DmxPid, u4Flag, &rDmxMMInfo);
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        UINT8 u1DmxPid = 0xFF;
        INT32 i;
        UINT32 u4AudFifoAddr, u4AudFifoSize;
        UINT32 u4AudPreFifoAddr = 0;
        UINT32 u4AudPreFifoSize = 0;
        UINT32 u4AudTrickSize = 0;

        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_AUDIO;

        if (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnADmx)
        {
            DMX_SetToDecoder(TRUE);
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid = 
                DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);

            _SWDMX_GetAudioFifo(prSwdmxInst->u1SwdmxId, &u4AudFifoAddr, &u4AudFifoSize);

            if( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid != 0xFF)
            {
                u1DmxPid = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid;
            }
            else
            {
                LOG(3, "get  audio pid fail\n");
            }

            u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_VALID |
                DMX_MM_FLAG_BUF | DMX_MM_FLAG_INSTANCE_TAG |
                DMX_MM_FLAG_DEVICE_ID);
        
            rDmxMMInfo.fgAllocBuf = FALSE;
            
            rDmxMMInfo.u4BufAddr = u4AudFifoAddr;
            rDmxMMInfo.u4BufSize = u4AudFifoSize;
            rDmxMMInfo.pvInstanceTag = (void*)prSwdmxInst;
            rDmxMMInfo.u1ChannelId = prSwdmxInst->u1AdecId;
            rDmxMMInfo.u1DeviceId = prSwdmxInst->u1AdecId;

            // Record the audio fifo size
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AFifoSize = rDmxMMInfo.u4BufSize;

            
            DMX_AudHandler_SetEnable(TRUE);
            
            fgRet = DMX_MM_Set(u1DmxPid, u4Flag, &rDmxMMInfo);
            if(!DMX_AudHandler_Set(u1DmxPid, TRUE))
            {
                LOG(1, "%s:%d, DMX_AudHandler_Set fail\n", __FUNCTION__, __LINE__);
            }
            
            if(_SWDMX_GetAudioPreFifo(prSwdmxInst->u1SwdmxId, &u4AudPreFifoAddr, &u4AudPreFifoSize))
            {
                u4AudTrickSize = u4AudPreFifoSize/AVI_DIVX_MAX_AUD_TRACK;
            }
            else
            {
                LOG(1, "%s:%d, _SWDMX_GetAudioPreFifo fail\n", __FUNCTION__, __LINE__);
            }
            for (i = 0; i < AVI_DIVX_MAX_AUD_TRACK; i++)
            {
                rDmxMMInfo.fgAllocBuf = FALSE;            
                rDmxMMInfo.u4BufSize = u4AudTrickSize; 
                rDmxMMInfo.u4BufAddr = u4AudPreFifoAddr;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[i] = 
                              DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);
                if( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[i]  == 0xFF)
                {
                    LOG(3, "get  audio pre  pid fail\n");
                }
                
                fgRet = DMX_MM_Set( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[i], u4Flag, &rDmxMMInfo);
                if(!DMX_AudHandler_Set( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[i], FALSE))
                {
                    LOG(1, "%s:%d, DMX_AudHandler_Set fail\n", __FUNCTION__, __LINE__);
                }
                
                u4AudPreFifoAddr = u4AudPreFifoAddr + u4AudTrickSize;
            }
            fgRet = DMX_AudHandler_SetActivePidx( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[0]);
             prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSetActivePid = TRUE;
        }
        else if (fgEos)
        {
            INT32 i;

            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;
            for (i = 0; i < AVI_DIVX_MAX_AUD_TRACK; i++)
            { 
                if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[i]!= 0xFF)
                {
                    fgRet = DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[i], u4Flag, &rDmxMMInfo);
                }
            }
        }

    }
    else if (u1HdrType == eSWDMX_MEDIA_SUBTITLE)
    {
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_PES;
        
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1SubPid = 
            DMX_MUL_GetSubtitlePidx(prSwdmxInst->u1DmxId);
        if (pfnCbFunc)
        {
            u4Flag = (DMX_MM_FLAG_TYPE | 
                DMX_MM_FLAG_SEARCH_START_CODE | 
                DMX_MM_FLAG_BUF | 
              DMX_MM_FLAG_VALID);
        
            u4Flag |= DMX_MM_FLAG_NOTIFY;
            rDmxMMInfo.pfnCallback = (PFN_DMX_NOTIFY)pfnCbFunc;
            
            rDmxMMInfo.fgSearchStartCode = FALSE;
            rDmxMMInfo.fgAllocBuf = TRUE;
            rDmxMMInfo.u4BufSize = AVI_SUBTITLE_BUFSIZE;

            fgRet = DMX_MM_Set( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1SubPid , u4Flag, &rDmxMMInfo);
        }
    }

    return fgRet;
}


static BOOL _SWDMX_SendAviEos(SWDMX_INFO_T* prSwdmxInst, UINT8 u1HdrType, UINT8 u1Index)
{
    UINT16 u2MaxQNum, u2CurQNum;
    //UINT32 u4DummyData[4];
    DMX_MM_DATA_T rDmxMMData;
    SWDMX_AVI_VERIFY_NULL_ERROR(prSwdmxInst);

    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) != 
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }
    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    rDmxMMData.u4BufStart = prSwdmxInst->rFeederInfo.u4StartAddr;
    rDmxMMData.u4BufEnd = prSwdmxInst->rFeederInfo.u4EndAddr;
    rDmxMMData.u4StartAddr = prSwdmxInst->rFeederInfo.u4StartAddr;
    rDmxMMData.u4FrameSize = 4;
    rDmxMMData.fgEOS = TRUE;

    if(((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_WMV7) ||
     (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_WMV8)) ||
     (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_WMV9))
    {
         rDmxMMData.fgFrameHead = TRUE;
    }
    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {            
        #ifdef CC_3D_MM_DS_SUPPORT
        if(ENABLE_NEWTRACK(prSwdmxInst))
        {
            if(IS_NEWTRACK(prSwdmxInst,u1Index))
            {
                rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1Vid2Pid;
                VDEC_GetQueueInfo(prSwdmxInst->u1Vdec2Id, &u2CurQNum, &u2MaxQNum);
                if (u2CurQNum > (u2MaxQNum - 10))
                {
                    x_thread_delay(1);
                    return FALSE;
                }
                if (_SWDMX_InitDmx(prSwdmxInst, u1HdrType, prSwdmxInst->u1Vdec2Id, NULL, TRUE) == FALSE)
                {
                    return FALSE;
                }
                return  _SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData);    
            }
            else
            {
                rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid;
                VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
                if (u2CurQNum > (u2MaxQNum - 10))
                {
                    x_thread_delay(1);
                    return FALSE;
                }
                if (_SWDMX_InitDmx(prSwdmxInst, u1HdrType, prSwdmxInst->u1VdecId, NULL, TRUE) == FALSE)
                {
                    return FALSE;
                }            
                return  _SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData);  
            }
        }
        else
        #endif
        {
            rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid;
            VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
            LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
            if (u2CurQNum > (u2MaxQNum - 10))
            {
                x_thread_delay(1);
                return FALSE;
            }
            if (_SWDMX_InitDmx(prSwdmxInst, u1HdrType, prSwdmxInst->u1VdecId, NULL, TRUE) == FALSE)
            {
                return FALSE;
            }
            return _SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData); 
        }           
    }
    else
    {
        INT32 i;

        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnAudio)
        {
            if((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)&&
                (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgSetActivePid == FALSE))
            {
                 DMX_AudHandler_SetActivePidx(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]);
            }
        }
        for (i = 0; i < AVI_DIVX_MAX_AUD_TRACK; i++)
        {
            rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[i];
            if (!_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData))
            {
                LOG(3, "Fail to send audio eos\n");
                return FALSE;
            }
        }
        return TRUE;
    }
}

static BOOL _SWDMX_AviSeekCurPos(SWDMX_INFO_T* prSwdmxInst)
{
        VDP_POS_INTO_T rVdpPosInfo;
	SWDMX_AVI_INFO_T *prSwdmxAviInfo=&prSwdmxInst->rFmtInfo.rSwdmxAviInfo;
        SWDMX_AVI_VERIFY_NULL_ERROR(prSwdmxInst);
	if(!prSwdmxAviInfo->fgEnVDmx)
	{
       return FALSE;
	}
	
	if(VDP_GetPosInfo(prSwdmxInst->u1B2rId,&rVdpPosInfo))
	{
        prSwdmxAviInfo->u8CurDmxMoviPos=rVdpPosInfo.u8OffsetI;
		prSwdmxAviInfo->u4CurVidPts=rVdpPosInfo.u8Pts;
		if(prSwdmxAviInfo->u1CurATrackIdx<AVI_DIVX_MAX_AUD_TRACK)
		{
			prSwdmxAviInfo->u4AudPts[prSwdmxAviInfo->u1CurATrackIdx]=(UINT32)rVdpPosInfo.u8AudPts;
		}
		_SWDMX_AviResetFeeder(prSwdmxInst);
		prSwdmxAviInfo->fgIsSeek = TRUE;
		prSwdmxAviInfo->u4LeftBytesInFeeder = 0;
		prSwdmxAviInfo->u8CurMoviPos=rVdpPosInfo.u8OffsetI;
		return TRUE;
	}
	return FALSE;
}

static BOOL _SWDMX_AviSeek(SWDMX_INFO_T* prSwdmxInst, 
    UINT64 u8SeekTime, 
    SWDMX_RANGE_LIST_T* prRangeList)
{
    BOOL fgFindKey = FALSE;
    UINT8 u1ReadDrmSize = 0;
    INT32 i;
    MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl = NULL;
    MM_RANGE_ELMT_IDXTBL_T *prAudKeyIdxTbl = 0;
    MM_RANGE_ELMT_IDXTBL_T *prVidTmpKeyIdxTbl = 0;
    MM_RANGE_ELMT_IDXTBL_T *prAudBaseKeyIdxTbl = 0;
    MM_RANGE_ELMT_IDXTBL_T *prAudTmpKeyIdxTbl = 0;
    MM_RANGE_ELMT_IDXTBL_T *prVidIdxTbl = 0;
    MM_RANGE_ELMT_IDXTBL_T *prAudIdxTbl = 0;
    UINT32* pu4AudPts;
    
    #ifdef CC_3D_MM_DS_SUPPORT
    INT32 j;
    UINT64 u8TmpCurDmxPos = 0;
    MM_RANGE_ELMT_IDXTBL_T *prSubVidKeyIdxTbl = NULL;
    #endif
    
    SWDMX_AVI_VERIFY_NULL_ERROR(prSwdmxInst);
    prVidKeyIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl;
    LOG(7, "_SWDMX_AviSeek u8SeekTime = 0x%08x\n", u8SeekTime);
    
    #ifdef CC_3D_MM_DS_SUPPORT
    prSubVidKeyIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidKeyIdxTbl;
   // MM_RANGE_ELMT_IDXTBL_T *prVidRTmpKeyIdxTbl = 0;
    if(ENABLE_NEWTRACK(prSwdmxInst))
   {
        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSupport3DTrick)
        {
            UINT8 u1ReadDrmSize = 0;

    LOG(7, "_SWDMX_AviSeek u8SeekTime = 0x%08x\n", u8SeekTime);

            if (prSubVidKeyIdxTbl && prSubVidKeyIdxTbl->ui4_number_of_entry != 0)
            {
                if (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgKeyExist)
                {
                    LOG(5, "_SWDMX_Avi3DSeek doesn't have the key table\n");
                    return FALSE;
                }
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl = prSubVidKeyIdxTbl;
             	for (j = 0; j < prSubVidKeyIdxTbl->ui4_number_of_entry; j++)
             	{
             	    if (prSubVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[j].ui4_pts > u8SeekTime)
             	    {
             	        break;
             	    }
             	}
                if (prSubVidKeyIdxTbl->pt_next_partial)
                {
                    INT32 i4NumRiffs = 2;
                    fgFindKey = FALSE;
                    LOG(5, "There are multiple riffs\n");
                    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidKeyIdxTbl->pt_next_partial->u.pt_pb_idx_tbl_entry[0].ui4_pts < u8SeekTime)
                    {
                        prSubVidKeyIdxTbl = prSubVidKeyIdxTbl->pt_next_partial;
                        while (prSubVidKeyIdxTbl)
                        {
                            prVidTmpKeyIdxTbl = prSubVidKeyIdxTbl;
                            for (j = 0; j < prSubVidKeyIdxTbl->ui4_number_of_entry; j++)
                            {
                                if (prSubVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[j].ui4_pts > u8SeekTime)
                                {
                                    LOG(5, " Find key frame in the %ldth riff\n", i4NumRiffs);
                                    fgFindKey = TRUE;
                                    break;
                                }
                            }
                            if (fgFindKey)
                            {
                                break;
                            }
                            prSubVidKeyIdxTbl = prSubVidKeyIdxTbl->pt_next_partial;
                            i4NumRiffs++;
                        }
                        prSubVidKeyIdxTbl = prVidTmpKeyIdxTbl;
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurSubVidKeyIdxTbl = prSubVidKeyIdxTbl;
                    }
                }

                if (prRangeList->prDmxRangeInfo->b_drm)
                {
                    // Currently for Home Theater Profile only
                    u1ReadDrmSize = AVI_DRM_CHUNK_SIZE_HT;
                }
                 if (j > 0)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey = j - 1;
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey = 0;
                }
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatSubFrameIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey;
                if (prSubVidKeyIdxTbl != 0)
                {
                     u8TmpCurDmxPos = prSubVidKeyIdxTbl->ui8_base_offset + 
                        prSubVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey].ui4_relative_offset -
                        (AVI_PARSER_OFFSET + u1ReadDrmSize);
                    if(u8TmpCurDmxPos < prSwdmxInst->u8FileOffset)
                    {
                        u8TmpCurDmxPos = prSwdmxInst->u8FileOffset;
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSubVidPts = 
                        prSubVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey].ui4_pts;
                    LOG(5, "Avi Seek Subkey frame number = 0x%08x Video offset = %lld\n", 
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey,
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos);
                    LOG(5, "Avi Seek SubVideo pts = 0x%08x\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSubVidPts);
                }      
            }
        }
    }
    #endif
    if ( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl &&
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl->ui4_number_of_entry != 0)
    {
        if (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgKeyExist)
        {
            LOG(5, "_SWDMX_AviSeek doesn't have the key table\n");
            return FALSE;
        }

        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl = prVidKeyIdxTbl;
     	for (i = 0; i < prVidKeyIdxTbl->ui4_number_of_entry; i++)
     	{
     	    if (prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts > u8SeekTime)
     	    {
     	        break;
     	    }
     	}

    // There r extra riffs
        if (prVidKeyIdxTbl->pt_next_partial)
        {
            INT32 i4NumRiffs = 2;
        
            LOG(5, "There are multiple riffs\n");
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl->pt_next_partial->u.pt_pb_idx_tbl_entry[0].ui4_pts < u8SeekTime)
            {
                prVidKeyIdxTbl = prVidKeyIdxTbl->pt_next_partial;
                while (prVidKeyIdxTbl)
                {
                    prVidTmpKeyIdxTbl = prVidKeyIdxTbl;
                    for (i = 0; i < prVidKeyIdxTbl->ui4_number_of_entry; i++)
                    {
                        if (prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts > u8SeekTime)
                        {
                            LOG(5, " Find key frame in the %ldth riff\n", i4NumRiffs);
                            fgFindKey = TRUE;
                            break;
                        }
                    }
                    if (fgFindKey)
                    {
                        break;
                    }
                    prVidKeyIdxTbl = prVidKeyIdxTbl->pt_next_partial;
                    i4NumRiffs++;
                }
                prVidKeyIdxTbl = prVidTmpKeyIdxTbl;
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl = prVidKeyIdxTbl;
            }
        }

        if (prRangeList->prDmxRangeInfo->b_drm)
        {
            // Currently for Home Theater Profile only
            u1ReadDrmSize = AVI_DRM_CHUNK_SIZE_HT;
        }


        // Error Handling 
        // i = 0 is impossible
         if ((i > 0) /* && 
            (i < (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl->ui4_number_of_entry - 1))*/)
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey = i - 1;
        }
        /*else if (i == (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl->ui4_number_of_entry - 1))
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey = i;
        }*/
        else
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey= 0;
        }

            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey;

        if (prVidKeyIdxTbl != 0)
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos = 
                prVidKeyIdxTbl->ui8_base_offset + 
                prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey].ui4_relative_offset -
                (AVI_PARSER_OFFSET + u1ReadDrmSize);

            if( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos < prSwdmxInst->u8FileOffset)
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos = prSwdmxInst->u8FileOffset;
            }
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts = 
                prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey].ui4_pts;

            LOG(5, "Avi Seek key frame number = 0x%08x Video offset = %lld\n", 
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey,
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos);

            LOG(5, "Avi Seek Video pts = 0x%08x\n", 
                prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey].ui4_pts);
        }
        #ifdef CC_3D_MM_DS_SUPPORT
        if(ENABLE_NEWTRACK(prSwdmxInst))
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSupport3DTrick)
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos = min(u8TmpCurDmxPos, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos);
                if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey != prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatSubFrameIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey;
                }
            }
        }
        #endif
        for (i = 0; (i < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack) && (i < AVI_DIVX_MAX_AUD_TRACK); i++)
        {
            if (prVidKeyIdxTbl == prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl)
            {
                prAudKeyIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[i];
                pu4AudPts = (UINT32*)prAudKeyIdxTbl->u.pv_idx_tbl_entry;

                LOG(5, "prAudKeyIdxTbl = 0x%08x\n", prAudKeyIdxTbl);
            }
            else
            {
                prVidIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl->pt_next_partial;
                prAudIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[i]->pt_next_partial;

                while (prVidIdxTbl != prVidKeyIdxTbl)
                {
                    prVidIdxTbl = prVidIdxTbl->pt_next_partial;
                    prAudIdxTbl = prAudIdxTbl->pt_next_partial;
                }
                
                prAudKeyIdxTbl = prAudIdxTbl;
                pu4AudPts = (UINT32*)prAudIdxTbl->u.pv_idx_tbl_entry;
                LOG(5, "prAudKeyIdxTbl = 0x%08x\n", prAudKeyIdxTbl);
            }

            if(pu4AudPts)
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[i] = 
                    pu4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey];
                LOG(5, "Avi Seek Audio pts = 0x%08x\n", 
                    pu4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey]);
            }
            else
            {
                LOG(0,"audio pts tabel is NULL\n");
            }
            
        }
    }
    else if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK) &&
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx] &&
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]->ui4_number_of_entry != 0)
    {
        prAudKeyIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx];
        pu4AudPts = (UINT32*)prAudKeyIdxTbl->u.pv_idx_tbl_entry;
        for (i = 0; i < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]->ui4_number_of_entry; i++)
        {
            if (pu4AudPts[i]> u8SeekTime)
            {
                break;
            }
        }

        prAudBaseKeyIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx];
        prAudTmpKeyIdxTbl = prAudKeyIdxTbl;

         // There r extra riffs
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]->pt_next_partial)
        {
            INT32 i4NumRiffs = 2;
            
            LOG(5, "There are multiple riffs\n");
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]->pt_next_partial->u.pt_pb_idx_tbl_entry[0].ui4_pts < u8SeekTime)
            {
                prAudKeyIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]->pt_next_partial;
                while (prAudKeyIdxTbl)
                {
                    prAudTmpKeyIdxTbl = prAudKeyIdxTbl;
                    pu4AudPts = (UINT32*)prAudKeyIdxTbl->u.pv_idx_tbl_entry;
                    for (i = 0; i < prAudKeyIdxTbl->ui4_number_of_entry; i++)
                    {
                        if (pu4AudPts[i]> u8SeekTime)
                        {
                           
                             fgFindKey = TRUE;
                             break;
                         }
                    }
                    if (fgFindKey)
                    {
                        break;
                    }
                    prAudKeyIdxTbl = prAudKeyIdxTbl->pt_next_partial;
                    i4NumRiffs++;
                }
                prAudBaseKeyIdxTbl = prAudTmpKeyIdxTbl;
            }
        }

        // Error Handling 
        // i = 0 is impossible
         if ((i > 0) /* && 
            (i < (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl->ui4_number_of_entry - 1))*/)
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey = i - 1;
        }
        /*else if (i == (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl->ui4_number_of_entry - 1))
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey = i;
        }*/
        else
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey= 0;
        }

        for (i = 0; (i < prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1TotalATrack) && (i < AVI_DIVX_MAX_AUD_TRACK); i++)
        {
            if (prAudBaseKeyIdxTbl == prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx])
            {
                prAudKeyIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[i];
                pu4AudPts = (UINT32*)prAudKeyIdxTbl->u.pv_idx_tbl_entry;

                LOG(5, "prAudKeyIdxTbl = 0x%08x\n", prAudKeyIdxTbl);
            }
            else
            {
                prAudIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[i]->pt_next_partial;
                prAudTmpKeyIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx];
                
                while (prAudTmpKeyIdxTbl != prAudBaseKeyIdxTbl)
                {
                    prAudIdxTbl = prAudIdxTbl->pt_next_partial;
                    prAudTmpKeyIdxTbl = prAudTmpKeyIdxTbl->pt_next_partial;
                }

                prAudKeyIdxTbl = prAudIdxTbl;
                pu4AudPts = (UINT32*)prAudIdxTbl->u.pv_idx_tbl_entry;
                LOG(5, "prAudKeyIdxTbl = 0x%08x\n", prAudKeyIdxTbl);
            }
            
            if(pu4AudPts)
            {
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[i] = 
                    pu4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey];
                LOG(5, "Avi Seek Audio pts = 0x%08x\n", 
                    pu4AudPts[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey]);
            }
            else
            {
                LOG(1, "2audio table is NULL\n");
            }

        }

        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl)
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos = 
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl->ui8_base_offset + 
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey].ui4_relative_offset -
                    (AVI_PARSER_OFFSET + u1ReadDrmSize); 
         }
    }
    else
    {
        LOG(5, "There is no audio and video index table for Avi seek\n");
    }

    _SWDMX_AviResetFeeder(prSwdmxInst);

    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSeek = TRUE;
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurMoviPos = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8CurDmxMoviPos;

    // Set STC, temp, should update by audio (audio master)
    //STC_SetStcValue(prSwdmxInst->u1StcId, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts);
    
    LOG(7, "_SWDMX_AviSeek Key = %ld\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey);
    return TRUE;
}


static BOOL _SWDMX_AviResetFeeder(SWDMX_INFO_T* prSwdmxInst)
{
    SWDMX_AVI_VERIFY_NULL_ERROR(prSwdmxInst);
    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) != 
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    //LOG(5, "_SWDMX_AviResetFeeder Cur Fed's WPtr = 0x%08x\n", _rFeederInfo.u4WriteAddr);
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr = 
        prSwdmxInst->rFeederInfo.u4WriteAddr;

    // Update read ptr to feeder
    FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurDmxFeederRPtr));
    return TRUE;
}


static void _SWDMX_AviCalculatePts(SWDMX_INFO_T *prSwdmxInst,
    UINT8 u1HdrType, 
    UINT8 u1TrackIdx, 
    UINT32 u4ChunkSize)
{
    UINT64 u8Numerator = 0, u8Denominator = 0;

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {   
        #ifdef CC_3D_MM_DS_SUPPORT
        if(ENABLE_NEWTRACK(prSwdmxInst))
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsLeftVideo)
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl)
                {
                    u8Numerator = 
                        (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_scale * DEMUX_AVI_AV_PERIOD 
                        + prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VRemainder;
                    u8Denominator = 
                        (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_rate;

                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts += u8Div6432(u8Numerator, u8Denominator, &prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VRemainder);
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
                    {
                        UINT32 u4Pts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts;
                    
                        if (prSwdmxInst->fgIsBadInterleaved && u4Pts > SWDMX_AVI_BAD_INTERLEAVED_PTS_THRD)
                        {
                            UINT32 u4CurStc = STC_GetStcValue(prSwdmxInst->u1StcId);
                            if ((u4CurStc + SWDMX_AVI_BAD_INTERLEAVED_PTS_THRD) < u4Pts)
                            {
                                LOG(1, "id,stc,pts=(%d,%d,%d)\n", prSwdmxInst->u1SwdmxId, u4CurStc, u4Pts);
                                x_thread_delay(SWDMX_AVI_BAD_INTERLEAVED_DELAY);
                            }
                        }
                    }
#endif                    
                }
               else
                {
                    LOG(3, "Video table is NULL\n");
                    return ;
                }            
            }
            else 
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidIdxTbl)
                {
                    u8Numerator = 
                        (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidIdxTbl->ui4_scale * DEMUX_AVI_AV_PERIOD 
                        + prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VRRemainder;
                    u8Denominator = 
                        (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidIdxTbl->ui4_rate;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSubVidPts += u8Div6432(u8Numerator, u8Denominator, &prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VRRemainder);
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
                    {
                        UINT32 u4Pts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSubVidPts;
        
                        if (prSwdmxInst->fgIsBadInterleaved && u4Pts > SWDMX_AVI_BAD_INTERLEAVED_PTS_THRD)
                        {
                            UINT32 u4CurStc = STC_GetStcValue(prSwdmxInst->u1StcId);
                            if ((u4CurStc + SWDMX_AVI_BAD_INTERLEAVED_PTS_THRD) < u4Pts)
                            {
                                LOG(1, "id,stc,pts=(%d,%d,%d)\n", prSwdmxInst->u1SwdmxId, u4CurStc, u4Pts);
                                x_thread_delay(SWDMX_AVI_BAD_INTERLEAVED_DELAY);
                            }
                        }
                    }
#endif                    
                }
               else
                {
                    LOG(3, "Sub Video table is NULL\n");
                    return ;
                }              
            }
        }
        else
        #endif
        {
             if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl)
             {
                 u8Numerator = 
                     (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_scale * DEMUX_AVI_AV_PERIOD 
                     + prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VRemainder;
                 u8Denominator = 
                     (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prVidIdxTbl->ui4_rate;
                 prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts += u8Div6432(u8Numerator, u8Denominator, &prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8VRemainder);
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
                 {
                     UINT32 u4Pts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurVidPts;
    
                     if (prSwdmxInst->fgIsBadInterleaved && u4Pts > SWDMX_AVI_BAD_INTERLEAVED_PTS_THRD)
                     {
                         UINT32 u4CurStc = STC_GetStcValue(prSwdmxInst->u1StcId);
                         if ((u4CurStc + SWDMX_AVI_BAD_INTERLEAVED_PTS_THRD) < u4Pts)
                         {
                             LOG(1, "id,stc,pts=(%d,%d,%d)\n", prSwdmxInst->u1SwdmxId, u4CurStc, u4Pts);
                             x_thread_delay(SWDMX_AVI_BAD_INTERLEAVED_DELAY);
                         }
                     }
                 }
#endif                    
             }
            else
            {
                 LOG(3, "Video table is NULL\n");
                 return ;
             }            
        }
    }
    else if ((u1HdrType == eSWDMX_MEDIA_AUDIO ||u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO) &&
        (u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK))
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgKeyExist)
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[u1TrackIdx])
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[u1TrackIdx]->b_is_VBR)
                {
                    u8Numerator = 
                        (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[u1TrackIdx]->ui4_scale * DEMUX_AVI_AV_PERIOD
                        + prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ARemainder[u1TrackIdx];
                    u8Denominator = 
                        (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[u1TrackIdx]->ui4_rate;
                }
                else
                {
                    u8Numerator = (UINT64)u4ChunkSize * DEMUX_AVI_AV_PERIOD
                        + prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ARemainder[u1TrackIdx];
                    u8Denominator = 
                        (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[u1TrackIdx]->ui4_avg_bytes_per_sec;
                }
            }
            else
            {
                LOG(3, "audio key table is NULL\n");
                return ;
            }
        }
        else
        {   
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prAudIdxTbl[u1TrackIdx])
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prAudIdxTbl[u1TrackIdx]->b_is_VBR)
                {
                    u8Numerator = 
                        (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prAudIdxTbl[u1TrackIdx]->ui4_scale * DEMUX_AVI_AV_PERIOD
                        + prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ARemainder[u1TrackIdx];
                    u8Denominator = 
                        (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prAudIdxTbl[u1TrackIdx]->ui4_rate;
                }
                else
                {
                    u8Numerator = (UINT64)u4ChunkSize * DEMUX_AVI_AV_PERIOD
                        + prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ARemainder[u1TrackIdx];
                    u8Denominator = 
                        (UINT64)prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prAudIdxTbl[u1TrackIdx]->ui4_avg_bytes_per_sec;
                }
            }
            else
            {
                LOG(3, "Audio table is NULL\n");
                return ;
            }
        }
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx]  += u8Div6432(u8Numerator, u8Denominator, &prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u8ARemainder[u1TrackIdx]);
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
        {
            UINT32 u4Pts = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudPts[u1TrackIdx];

            if (prSwdmxInst->fgIsBadInterleaved && u4Pts > SWDMX_AVI_BAD_INTERLEAVED_PTS_THRD)
            {
                UINT32 u4CurStc = STC_GetStcValue(prSwdmxInst->u1StcId);
                if ((u4CurStc + SWDMX_AVI_BAD_INTERLEAVED_PTS_THRD) < u4Pts)
                {
                    LOG(1, "id,stc,pts=(%d,%d,%d)\n", prSwdmxInst->u1SwdmxId, u4CurStc, u4Pts);
                    x_thread_delay(SWDMX_AVI_BAD_INTERLEAVED_DELAY);
                }
            }
        }
#endif
    }
}


static BOOL _SWDMX_AviCalculateRepeatFrame(SWDMX_INFO_T* prSwdmxInst, INT32 i4PlaySpeed, UINT8 u1VdecId)
{
    UINT32 u4CurKey = 0;
    MM_RANGE_ELMT_IDXTBL_T *prKeyIdxTbl = NULL;
    
    #ifdef CC_3D_MM_DS_SUPPORT
    MM_RANGE_ELMT_IDXTBL_T *prSubKeyIdxTbl = NULL;
    UINT32 u4CurSubKey = 0;
    #endif
    
    SWDMX_AVI_VERIFY_NULL_ERROR(prSwdmxInst);
    u4CurKey = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentKey;
    prKeyIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prCurVidKeyIdxTbl;
    
    #ifdef CC_3D_MM_DS_SUPPORT
    prSubKeyIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prSubVidKeyIdxTbl;
    u4CurSubKey = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurSentSubKey;
    if(ENABLE_NEWTRACK(prSwdmxInst))
    {
        if(u1VdecId == prSwdmxInst->u1Vdec2Id)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame == 0)
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PreRepeatSubFrameIdx != u4CurSubKey &&
                    (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatSubFrameIdx < prSubKeyIdxTbl->ui4_number_of_entry))
                {
                    UINT32 u4SubPtsDif = 0, u4SubMaxRepeatFrame;
                    u4SubMaxRepeatFrame = AVI_MAX_REPEAT_FRAME;//(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps * 1000 * 2)/abs(i4PlaySpeed);
                    if(i4PlaySpeed == 2000)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame = 7;
                    }
                    else if (i4PlaySpeed > 2000)
                    {
                        if ((u4CurSubKey < (prSubKeyIdxTbl->ui4_number_of_entry)) &&
                            (prSubKeyIdxTbl->ui4_number_of_entry > AVI_MINIMUM_KEY_NUM))
                        {
                            u4SubPtsDif = prSubKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurSubKey].ui4_pts -
                                prSubKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PreRepeatSubFrameIdx].ui4_pts;
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame = 
                                (UINT32)((u4SubPtsDif * (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps))/(90 * i4PlaySpeed));
                        }
                        else
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps * 1000)/abs(i4PlaySpeed);
                        }
                    }
                    else if (i4PlaySpeed < 0)
                    {
                        if ((prSubKeyIdxTbl->ui4_number_of_entry > AVI_MINIMUM_KEY_NUM))
                        {
                            u4SubPtsDif = prSubKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PreRepeatSubFrameIdx ].ui4_pts -
                                prSubKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurSubKey].ui4_pts;
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame = 
                                (UINT32)((u4SubPtsDif * (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps))/(90 * abs(i4PlaySpeed)));
                        }
                        else
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps * 1000)/abs(i4PlaySpeed);
                        }
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame = (u4SubMaxRepeatFrame > prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame) ?
                        (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame) : u4SubMaxRepeatFrame;

                    if ((u4CurSubKey == prSubKeyIdxTbl->ui4_number_of_entry - 1) ||
                        (u4CurSubKey == 0))
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame = 
                            (AVI_FIX_REPEAT_FRAME/(abs(i4PlaySpeed)/1000));                       
                    }
                    LOG(5, "u4SubPtsDif = %ld Current Sub Idx = %ld Sub Repeat frame = %ld\n", 
                        u4SubPtsDif, u4CurSubKey, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame);
                    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame == 0)
                    {
                        return FALSE;
                    }
                    
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatSubFrameIdx = u4CurSubKey;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PreRepeatSubFrameIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatSubFrameIdx;
                    
                    LOG(5, "Sub Repeat Idx = %ld\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatSubFrameIdx);
                    return TRUE;
                }
                else
                {
                    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgStartTrick)
                    {
                         if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSupport3DTrick)
                        {
                            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame != 0)
                            {
                                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgStartTrick = FALSE;
                            }
                        }
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps * 1000)/abs(i4PlaySpeed);
                        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame == 0)
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame = 1;
                        }
                        return TRUE;
                    }
                    return FALSE;
                }
            }
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame--;
        }
        else if(u1VdecId == prSwdmxInst->u1VdecId)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame == 0)
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PreRepeatFrameIdx != u4CurKey &&
                    (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx < prKeyIdxTbl->ui4_number_of_entry))
                {
                    UINT32 u4PtsDif = 0, u4MaxRepeatFrame;
                    u4MaxRepeatFrame = AVI_MAX_REPEAT_FRAME;//(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps * 1000 * 2)/abs(i4PlaySpeed);
                    if(i4PlaySpeed == 2000)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = 7;
                    }
                    else if (i4PlaySpeed > 2000)
                    {
                        if ((u4CurKey < (prKeyIdxTbl->ui4_number_of_entry)) &&
                            (prKeyIdxTbl->ui4_number_of_entry > AVI_MINIMUM_KEY_NUM))
                        {
                            if(ENABLE_NEWTRACK(prSwdmxInst) &&(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSupport3DTrick))
                            {
                                u4PtsDif = prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts -
                                    prKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PreRepeatFrameIdx].ui4_pts;
                            }
                            else
                            {
                                u4PtsDif = prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts -
                                    prKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx].ui4_pts;
                             }
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = 
                                (UINT32)((u4PtsDif * (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps))/(90 * i4PlaySpeed));
                        }
                        else
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps * 1000)/abs(i4PlaySpeed);
                        }
                    }
                    else if (i4PlaySpeed < 0)
                    {
                        if ((prKeyIdxTbl->ui4_number_of_entry > AVI_MINIMUM_KEY_NUM))
                        {
                            if(ENABLE_NEWTRACK(prSwdmxInst) &&(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSupport3DTrick))    
                            {
                                u4PtsDif = prKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PreRepeatFrameIdx].ui4_pts -
                                    prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts;
                            }
                            else
                            {
                                u4PtsDif = prKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx].ui4_pts -
                                    prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts;
                            }
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = 
                                (UINT32)((u4PtsDif * (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps))/(90 * abs(i4PlaySpeed)));
                        }
                        else
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps * 1000)/abs(i4PlaySpeed);
                        }
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = (u4MaxRepeatFrame > prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame) ?
                        (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame) : u4MaxRepeatFrame;
                    if ((u4CurKey == prKeyIdxTbl->ui4_number_of_entry - 1) ||
                        (u4CurKey == 0))
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = 
                            (AVI_FIX_REPEAT_FRAME/(abs(i4PlaySpeed)/1000));
                    }
                    LOG(5, "u4PtsDif = %ld Current Idx = %ld Repeat frame = %ld\n", 
                        u4PtsDif, u4CurKey, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame);
                    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame == 0)
                    {
                        return FALSE;
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx = u4CurKey;
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4PreRepeatFrameIdx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx;
                    LOG(5, "Repeat Idx = %ld\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx);
                    return TRUE;
                }
                else
                {
                    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgStartTrick)
                    {
                        if(ENABLE_NEWTRACK(prSwdmxInst) &&(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSupport3DTrick))
                        {
                            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame != 0)
                            {
                                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgStartTrick = FALSE;
                            }
                        }
                        else
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgStartTrick = FALSE;
                        }
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps * 1000)/abs(i4PlaySpeed);
                        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame == 0)
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = 1;
                        }                   
                        return TRUE;
                    }                
                    return FALSE;
                }
            }
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame--;
        }
    }
    else
    #endif
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame == 0)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx != u4CurKey &&
                (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx < prKeyIdxTbl->ui4_number_of_entry))
            {
                UINT32 u4PtsDif = 0, u4MaxRepeatFrame;
        
                u4MaxRepeatFrame = AVI_MAX_REPEAT_FRAME;//(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps * 1000 * 2)/abs(i4PlaySpeed);

                if (i4PlaySpeed > 2000)
                {
                    if ((u4CurKey < (prKeyIdxTbl->ui4_number_of_entry)) &&
                        (prKeyIdxTbl->ui4_number_of_entry > AVI_MINIMUM_KEY_NUM))
                    {
                        u4PtsDif = prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts -
                            prKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx].ui4_pts;

                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = 
                            (UINT32)((u4PtsDif * prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps * 2)/(90 * i4PlaySpeed));
                    }
                    else
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps * 1000)/abs(i4PlaySpeed);
                    }
                }
                else if (i4PlaySpeed < 0)
                {
                    if ((prKeyIdxTbl->ui4_number_of_entry > AVI_MINIMUM_KEY_NUM))
                    {
                        u4PtsDif = prKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx].ui4_pts -
                            prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts;

                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = 
                            (UINT32)((u4PtsDif * prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps * 2)/(90 * abs(i4PlaySpeed)));;
                    }
                    else
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps * 1000)/abs(i4PlaySpeed);
                    }
                }

                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = (u4MaxRepeatFrame > prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame) ?
                    (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame) : u4MaxRepeatFrame;

                if ((u4CurKey == prKeyIdxTbl->ui4_number_of_entry - 1) ||
                    (u4CurKey == 0))
                {
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = 
                        (AVI_FIX_REPEAT_FRAME/(abs(i4PlaySpeed)/1000));

                    /*if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame > AVI_MAX_REPEAT_FRAME)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = AVI_MAX_REPEAT_FRAME;
                    }
                    else if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame < AVI_MIN_REPEAT_FRAME)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = AVI_MIN_REPEAT_FRAME;
                    }*/                        
                }

                LOG(5, "u4PtsDif = %ld Current Idx = %ld Repeat frame = %ld\n", 
                    u4PtsDif, u4CurKey, prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame);

                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame == 0)
                {
                    return FALSE;
                }

                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx = u4CurKey;
                LOG(5, "Repeat Idx = %ld\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4CurRepeatFrameIdx);

                return TRUE;
            }
            else
            {
                // Force to play the first key frame
                if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgStartTrick)
                {
                    #ifdef CC_3D_MM_DS_SUPPORT
                    if(ENABLE_NEWTRACK(prSwdmxInst) &&(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgIsSupport3DTrick))
                    {
                        if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4SubVidRepeatFrame != 0)
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgStartTrick = FALSE;
                        }
                    }
                    else
                    #endif
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgStartTrick = FALSE;
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame = (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidFps * 1000)/abs(i4PlaySpeed);
                    return TRUE;
                }
                
                return FALSE;
            }
        }

        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidRepeatFrame--;
   }
    return TRUE;
}



BOOL _SWDMX_AviReadBuffer(UINT8 u1SrcId, ENUM_SWDMX_STRM_TYPE_T eStrmType, 
    SWDMX_READ_INFO_T *prReadInfo)
{
    BOOL  fgRet = FALSE;
    SWDMX_INFO_T* prSwdmxInst = NULL;
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_AVI_VERIFY_NULL_ERROR(prSwdmxInst);
    switch (eStrmType)
    {
        case eSWDMX_STRM_TYPE_SUBTITLE:
            LOG(3, "Avi Get Subtitle Buffer serial number = 0x%x Frame Address = 0x%08x",
                prReadInfo->u1SerialNumber,
                prReadInfo->u4FrameAddr);
                
            fgRet = DMX_MM_GetBuffer( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1SubPid , 
                prReadInfo->u1SerialNumber, 
                (UINT32)prReadInfo->u4FrameAddr,
            0, (UINT32)prReadInfo->u4CopyLen,
            prReadInfo->pucDest);

            if(!fgRet)
            {
                LOG(3, "_SWDMX_AviReadBuffer, get buffer error\n");
            }
            break;
        default:
            return FALSE;
    }
    
    return fgRet;
}


BOOL _SWDMX_AviReleaseBuffer(UINT8 u1SrcId, ENUM_SWDMX_STRM_TYPE_T eStrmType, 
    SWDMX_READ_INFO_T *prReadInfo)
{
    BOOL  fgRet = FALSE;
    SWDMX_INFO_T* prSwdmxInst = NULL;
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_AVI_VERIFY_NULL_ERROR(prSwdmxInst);
    switch (eStrmType)
    {
        case eSWDMX_STRM_TYPE_SUBTITLE:
            fgRet = DMX_MM_UnlockBuffer( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1SubPid , 
                prReadInfo->u1SerialNumber, 
                (UINT32)prReadInfo->u4CopyLen,
                (UINT32)prReadInfo->u4FrameAddr);

            if(!fgRet)
            {
                LOG(3, "_SWDMX_AviReleaseBuffer, unlock buffer error\n");
            }
            break;
        default:
            return FALSE;
    }
    
    return fgRet;
}


/**
*/
BOOL _SWDMX_AVIStop(UINT8 u1SrcId)
{
    //FeederStop(prSwdmxInst->eFeederSrc);
    //FeederUnInstallCallback(prSwdmxInst->eFeederSrc, 
    //  FEEDER_NOTIFY_DATA_REQ_DONE, _FeederCallBack);
    //FeederClose(prSwdmxInst->eFeederSrc);

    //BOOL fgEnVDmx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVDmx;
    //BOOL fgEnADmx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnADmx;
    //BOOL fgEnSPDmx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnSubDmx;
    SWDMX_INFO_T* prSwdmxInst = NULL;
    DMX_MM_T rPid;

    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    rPid.fgEnable = FALSE;

    SWDMX_AVI_VERIFY_NULL_ERROR(prSwdmxInst);
   
    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVDmx)
    {
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVDmx = FALSE;

        VERIFY(DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid);
        DMX_MUL_FreePidx(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid);
    }
    #ifdef CC_3D_MM_DS_SUPPORT
    if(ENABLE_NEWTRACK(prSwdmxInst))
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnV2Dmx)
        {
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnV2Dmx = FALSE;
            VERIFY(DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1Vid2Pid, DMX_MM_FLAG_VALID, &rPid));
            DMX_MM_Free(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1Vid2Pid);
            DMX_MUL_FreePidx(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1Vid2Pid);
        }
    }
    #endif
    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnADmx)
    {
        INT32 i;

        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnADmx = FALSE;
        VERIFY(DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid);
        DMX_MUL_FreePidx( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid );
        
        for (i = 0; i < AVI_DIVX_MAX_AUD_TRACK; i++)
        {
            VERIFY(DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[i],
                  DMX_MM_FLAG_VALID, &rPid));
            DMX_MM_Free(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[i]);
            DMX_MUL_FreePidx( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[i]);
            DMX_AudHandler_Free(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[i]);
        }
        DMX_AudHandler_SetEnable(FALSE);
    }
    if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnSubDmx)
    {
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnSubDmx = FALSE;
        VERIFY(DMX_MM_Set( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1SubPid , DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1SubPid );
        DMX_MUL_FreePidx( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1SubPid );
    }
    
    if (u1SrcId < SWDMX_SOURCE_MAX) 
    {
         g_fgNotSupport[u1SrcId] = FALSE;
    }
    x_memset(&prSwdmxInst->rFmtInfo.rSwdmxAviInfo, 0, sizeof(SWDMX_AVI_INFO_T));
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.i4PrevSpeed = 1000;
#endif

    return TRUE;
}


static BOOL _SWDMX_AviValidHdrBps(UINT32 u4HdrBps, UINT32 u4DspBps)
{
    // These values are defined in 11172-3 page 21.
    const UINT16 au2NormalBps[16] = {16,  24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320};
    const UINT16 u2Precision = 1;
    UINT16 u2HdrBps = (UINT16)(u4HdrBps * 8 / 1000);
    UINT16 u2DspBps = (UINT16)(u4DspBps * 8 / 1000);
    UINT16 u2HdrPtn = 16;
    UINT16 u2DspPtn = 16;
    UINT32 i;

    LOG(5, "u4HdrBps %d u4DspBps %x\n", u4HdrBps,u4DspBps);

    for(i = 0; i < 16; i++)
    {
        if ((u2HdrBps >= au2NormalBps[i] - u2Precision) && (u2HdrBps <= au2NormalBps[i] + u2Precision))
        {
            u2HdrPtn = i;
        }
        if ((u2DspBps >= au2NormalBps[i] - u2Precision) && (u2DspBps <= au2NormalBps[i] + u2Precision))
        {
            u2DspPtn = i;
        }
    }
    if ((u2HdrPtn < 16) && (u2HdrPtn == u2DspPtn))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


BOOL _SWDMX_AviAudioDrvCb(UINT8 u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
    if(eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE)
    {
        //BOOL fgMp3Vbr = FALSE;
        AUD_DEC_INFO_T rADecInfo;
        MM_RANGE_ELMT_IDXTBL_T *prAudIdxTbl = NULL;
        
        SWDMX_INFO_T* prSwdmxInst = NULL;
        prSwdmxInst = _SWDMX_GetInst(u1SrcId);
        SWDMX_AVI_VERIFY_NULL_ERROR(prSwdmxInst);
        
        x_memset(&rADecInfo, 0, sizeof(AUD_DEC_INFO_T));

        //AUD_DspSetMp3PreparsingMode(TRUE);
        //fgMp3Vbr = AUD_DspIsMp3Vbr();
        AUD_DspGetMpegAudInfo(prSwdmxInst->u1AdecId, &rADecInfo);

        if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgKeyExist) &&
            (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)&&
            (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx]))
        {
            prAudIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.parAudKeyIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx];
        }
        else if((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)&&
           ( prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx] !=NULL))
        {
            prAudIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx];
        }
        else
        {
            LOG(3,"error happen: no audio table\n");
        }

        if (prAudIdxTbl)
        {
            if ((rADecInfo.e_aud_fmt == AUD_FMT_MP3) &&
                (!prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgMp3Vbr) &&
                (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK) &&
                (!prAudIdxTbl->b_is_VBR))
            {
                if (!_SWDMX_AviValidHdrBps(prAudIdxTbl->ui4_avg_bytes_per_sec, rADecInfo.ui4_data_rate/8))
                {
                    prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgMp3Abr = TRUE;
                    LOG(5, "SWDMX_AVI Mp3 Abr!!!!!\n");
                }
            }
        }

        LOG(5, "SWDMX_AviAudioCb Audio is ready\n");
        if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgEnVideo &&
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.prDmxRangeInfo->ui8_vid_duration >prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts)
        {
#if 1
            if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgBadAudio) &&
                (rADecInfo.e_aud_fmt == AUD_FMT_MP3))
            {
                UINT32 u4DspPts;
                UINT32 u4PrsPos;

                LOG(5, "Bad Audio, Lookup audio pts from audio driver\n");
                AUD_GetDspPtsInfo(prSwdmxInst->u1AdecId, &u4DspPts, &u4PrsPos);
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = u4DspPts;
                AUD_SetStartPts(prSwdmxInst->u1AdecId, u4DspPts);
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgBadAudio = FALSE;
            }
#endif            

            LOG(5, "Triggerred by video\n");
            VDP_TriggerAudReceive(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts);
            LOG(5, "SWDMX_AviAudioCb VDP Trigger PTS = 0x%08x\n", 
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts);
            if(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause)
            {
                _SWDMX_AviVideoWakeUp(prSwdmxInst);
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgVDecIsPause = FALSE;
            }

            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgMp3Abr)
            {
                STC_SetSyncMode(prSwdmxInst->u1StcId,
                    prSwdmxInst->u1AdecId,
                    prSwdmxInst->u1VdecId, AV_SYNC_MODE_NONE);
                AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                STC_StartStc(prSwdmxInst->u1StcId);
                LOG(5, "mp3 abr case, reset sync mode to none!\n");
            }
        }
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT  // this is copied from the above if block with some modification
        else if (prSwdmxInst->fgIsBadInterleaved)
        {
#if 1
            if ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgBadAudio) &&
                (rADecInfo.e_aud_fmt == AUD_FMT_MP3))
            {
                UINT32 u4DspPts;
                UINT32 u4PrsPos;

                LOG(5, "Bad Audio, Lookup audio pts from audio driver\n");
                AUD_GetDspPtsInfo(prSwdmxInst->u1AdecId, &u4DspPts, &u4PrsPos);
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts = u4DspPts;
                AUD_SetStartPts(prSwdmxInst->u1AdecId, u4DspPts);
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgBadAudio = FALSE;
            }
#endif            

            LOG(5, "Triggerred by video\n");
            VDP_TriggerAudReceive(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts);
            LOG(5, "SWDMX_AviAudioCb VDP Trigger PTS = 0x%08x\n", 
                prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts);

            _fgBiAviAudioReady = TRUE;  // instead of original "_SWDMX_AviVideoWakeUp" if block

            if (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgMp3Abr)
            {
                STC_SetSyncMode(prSwdmxInst->u1StcId,
                    prSwdmxInst->u1AdecId,
                    prSwdmxInst->u1VdecId, AV_SYNC_MODE_NONE);
                AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                STC_StartStc(prSwdmxInst->u1StcId);
                LOG(5, "mp3 abr case, reset sync mode to none!\n");
            }
        }
#endif
        else
        {
            // Audio Only case
            LOG(5, "Trigger audio by _SWDMX_AviAudioDrvCb\n");
            AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
            STC_StartStc(prSwdmxInst->u1StcId);
        }
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAudIsReady = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgAudIsLoaded = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.fgGetMp3Status = TRUE;
    }
    else if (eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_NO_DATA)
    {
        LOG(5, "_SWDMX_AviAudioDrvCb: no data\n");
    }
    else
    {
        // impossible
        LOG(1, "_SWDMX_AviAudioDrvCb eAudioNotifyType != ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE\n");
    }

    return TRUE;
}

static void _SWDMX_AviVideoWakeUp(SWDMX_INFO_T *prSwdmxInst)
{
    LOG(5, "_SWDMX_AviVideoWakeUp\n");
    VDP_SetPauseMm(prSwdmxInst->u1B2rId, FALSE);
    switch(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VDecType)
    {
        case ENUM_SWDMX_VDEC_VC1:
        case ENUM_SWDMX_VDEC_WMV7:
        case ENUM_SWDMX_VDEC_WMV8:
        case ENUM_SWDMX_VDEC_WMV9:
            VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_WMV);
            break;
        case ENUM_SWDMX_VDEC_DX311:
        case ENUM_SWDMX_VDEC_MPEG4:
        case ENUM_SWDMX_VDEC_H263:
            VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MP4);
            break;
        case ENUM_SWDMX_VDEC_MJPEG:
            VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MJPEG);
             #ifdef CC_3D_MM_DS_SUPPORT
             if(ENABLE_NEWTRACK(prSwdmxInst))
            {
               VDEC_Play(prSwdmxInst->u1Vdec2Id, VDEC_FMT_MJPEG);
            }
            #endif
            break;
        case ENUM_SWDMX_VDEC_MPEG1_MPEG2:
            VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MPV);
            break;
        case ENUM_SWDMX_VDEC_H264:
            VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_H264);
            break;
        default:
            LOG(1, "SWDMX_AviAudioCb Unknown Video Type\n");
            break;
    }
}

static SWDMX_HDR_PRS_STATE_T  _SWDMX_AviBeforeDeliverUseIdx(
    SWDMX_INFO_T *prSwdmxInst,
    UINT8 u1HdrType)
{
       DMX_MM_DATA_T rDmxMMData = {0};
       
       x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

       if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) != 
        FEEDER_E_OK)
        {
              LOG(7, "Feeder input buffer error.\n");
             return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

        x_memcpy((VOID*)VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr), 
            (VOID*)VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidHeaderPtr), 
            prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidHdrLength);

        if(u1HdrType == eSWDMX_MEDIA_VIDEO)
        {
             #ifdef CC_3D_MM_DS_SUPPORT
             if(ENABLE_NEWTRACK(prSwdmxInst))
             {
                 rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1Vid2Pid;
             }
             else
             #endif
             {
                 rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1VidPid;
             }
        }
        else
        {
            rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPid;
        }
        rDmxMMData.u4BufStart = VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
        rDmxMMData.u4BufEnd = VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr);
        rDmxMMData.u4StartAddr = VIRTUAL(prSwdmxInst->rFeederInfo.u4WriteAddr);
        rDmxMMData.u4FrameSize = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4VidHdrLength;
        //if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
        if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
        {
            LOG(3, "Demuxer fails to move sequence header.\n");
            return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
        }
 
        prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4NumSentVid++;
       return eSWDMX_HDR_PRS_FED_MOVE_FAIL;

}

static VOID AviMoniterVdecSupport(UCHAR u1SrcId, BOOL fgNotSupport)
{
     if (u1SrcId < SWDMX_SOURCE_MAX) 
     {
         g_fgNotSupport[u1SrcId] = fgNotSupport;
     }
}
static SWDMX_HDR_PRS_STATE_T AviHandAAC(SWDMX_INFO_T *prSwdmxInst, UINT32 u4ChunkSize, UINT32 u4Pts, UINT8 u1TrackIdx)
{
  DMX_MM_DATA_T rTmpDmxMMData;
  UINT8 u1SampleRateIdx = 3;
  
  x_memset(&rTmpDmxMMData, 0, sizeof(DMX_MM_DATA_T));
  
  if(prSwdmxInst->pucAudBuf == 0)
  {
      LOG(1, "pucAudBuf should be allocated in init function");
      prSwdmxInst->pucAudBuf = 
          (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(SWDMX_AVI_AAC_ALLOC_SIZE,16));                           
  }                               

  if (prSwdmxInst->pucAudBuf)
  {
      /* 8b: syncword */
      prSwdmxInst->pucAudBuf[0] = 0xFF;
      /* 4b: syncword */
      /* 1b: mpeg id = 1 */
      /* 2b: layer = 0 */
      /* 1b: protection absent */
      prSwdmxInst->pucAudBuf[1] = 0xF9;
      /* 2b: profile */
      /* 4b: sampling_frequency_index */
      /* 1b: private = 0 */
      /* 1b: channel_configuration */
      switch (prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudSampleRate)
      {
          case 96000:
              u1SampleRateIdx = 0;
              break;
          case 88200:
              u1SampleRateIdx = 1;
              break;
          case 64000:
              u1SampleRateIdx = 2;
              break;
          case 48000:
              u1SampleRateIdx = 3;
              break;
          case 44100:
              u1SampleRateIdx = 4;
              break;
          case 32000:
              u1SampleRateIdx = 5;
              break;
          case 24000:
              u1SampleRateIdx = 6;
              break;
          case 22050:
              u1SampleRateIdx = 7;
              break;
          case 16000:
              u1SampleRateIdx = 8;
              break;
          case 12000:  
              u1SampleRateIdx = 9;
              break;
          case 11025:  
              u1SampleRateIdx = 10;
              break;
          case 8000:  
              u1SampleRateIdx = 11;
              break;
          default:
              u1SampleRateIdx = 4;
              break;
      }

      prSwdmxInst->pucAudBuf[2] = (1 << 6) |
               ((u1SampleRateIdx<< 2) & 0x3C) |
               ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudChannelNs >> 2) & 0x1);
      /* 2b: channel_configuration */
      /* 1b: original */
      /* 1b: home */
      /* 1b: copyright_id */
      /* 1b: copyright_id_start */
      /* 2b: aac_frame_length */
      prSwdmxInst->pucAudBuf[3] = ((prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4AudChannelNs & 0x3) << 6) |
               (((u4ChunkSize + SWDMX_AVI_AAC_FAKE_HDR_SIZE) >> 11) & 0x3);
      /* 8b: aac_frame_length */
      prSwdmxInst->pucAudBuf[4] = (((u4ChunkSize + SWDMX_AVI_AAC_FAKE_HDR_SIZE) >> 3) & 0xFF);
      /* 3b: aac_frame_length */
      /* 5b: adts_buffer_fullness */
      prSwdmxInst->pucAudBuf[5] = (((u4ChunkSize + SWDMX_AVI_AAC_FAKE_HDR_SIZE) << 5) & 0xE0) |
               ((0x7FF >> 6) & 0x1F);
      /* 6b: adts_buffer_fullness */
      /* 2b: number_of_raw_data_blocks_in_frame */
      prSwdmxInst->pucAudBuf[6] = ((0x7FF << 2) & 0xFC);
      rTmpDmxMMData.u4BufStart = (UINT32)prSwdmxInst->pucAudBuf;
      rTmpDmxMMData.u4BufEnd = rTmpDmxMMData.u4BufStart + SWDMX_AVI_AAC_ALLOC_SIZE;
      rTmpDmxMMData.u4StartAddr = rTmpDmxMMData.u4BufStart;
      rTmpDmxMMData.u4FrameSize = 7;
      rTmpDmxMMData.u4Pts = u4Pts;
      rTmpDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u1AudPreBufPid[u1TrackIdx];
          
      //if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
      if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rTmpDmxMMData)  != TRUE)
      {
          LOG(0, "Demuxer fails to move data.\n");
          return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
      } 
  }
  return eSWDMX_HDR_PRS_SUCCEED;
}


