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
 * $RCSfile: swdmx_avi.c,v $
 * $Revision: #1 $
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
#if defined(CC_MT5387) || defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395)
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
#define min(x, y) ((x < y) ? x : y)
#define max(x, y) ((x > y) ? x : y)
#define abs(x)                  (((x) >= 0) ? (x) : -(x))
#define IDR_PIC 5
#define IS_IDR_NALU(x) (((x & 0x1f) == IDR_PIC))

#define SWDMX_AVI_AAC_FAKE_HDR_SIZE (7)
#define SWDMX_AVI_AAC_ALLOC_SIZE   (256)

#define INPUT_SRC _rSwdmxAviInfo.prDmxRangeInfo->e_mm_src_type
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//static SWDMX_RANGE_LIST_T *_prRangeList = NULL;

static SWDMX_AVI_INFO_T _rSwdmxAviInfo = {0};

static FEEDER_BUF_INFO_T _rFeederInfo;

static UINT32 _u4LastAlignOffset;

static UCHAR *_pucAviAacBuf = 0;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static SWDMX_HDR_PRS_STATE_T _SWDMX_AviHdrPrsUseIdx(
        SWDMX_RANGE_LIST_T* prRangeList, 
        INT32 i4PlaySpeed,
	 UINT64 u8SeekPos,
	 BOOL fgSpeedChange);

static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverAviDataUseIdx(
        INT32 i4PlaySpeed,
        MM_RANGE_ELMT_IDXTBL_T *prIdxTbl, 
        UINT32* pu4CurIdx, 
        SWDMX_RANGE_LIST_T* prRangeList,
        UINT8 u1HdrType 
        );

static SWDMX_HDR_PRS_STATE_T _SWDMX_AviHdrPrsWithoutIdx(
        SWDMX_RANGE_LIST_T* prRangeList,
        INT32 i4PlaySpeed,
	 UINT64 u8SeekPos,
	 BOOL fgSpeedChange);

static SWDMX_HDR_PRS_STATE_T _SWDMX_PrsAviChunk(
    UCHAR* pucBitstream, 
    SWDMX_RANGE_LIST_T* prRangeList,
    UINT32 u4AvailSize,
    UINT32* pu4ConsumeSize, 
    INT32 i4PlaySpeed,
    BOOL fgRangeEnd);
        

static BOOL _SWDMX_AviGetDataFromFeeder(FEEDER_REQ_DATA_T *prFeederData);
static BOOL _CheckAviChunkIndex(UCHAR *pucBuf, UINT32 u4StrmID);
static BOOL _CheckAviChunkType(UCHAR *pucBuf, UINT8 *pu1HdrType, UINT8 *pu1TrackIdx);
static BOOL _CheckAviOtherChunk(UCHAR *pucBuf);

static BOOL _SWDMX_InitDmx(UINT8 u1HdrType, VOID* pfnCbFunc, BOOL fgEos);

static BOOL _SWDMX_SendAviEos(UINT8 u1HdrType);

static BOOL _SWDMX_AviSeek(UINT64 u8SeekTime, SWDMX_RANGE_LIST_T* prRangeList);
static BOOL _SWDMX_AviResetFeeder(void);
static BOOL _SWDMX_AviCalculateRepeatFrame(INT32 i4PlaySpeed);
static void _SWDMX_AviCalculatePts(UINT8 u1HdrType, UINT8 u1TrackIdx, UINT32 u4ChunkSize);
static void _SWDMX_AviPrsDrmChunk(UINT8 u1HdrType, UINT32 u4StartAddr);
static UINT32 _SWDMX_AviAsci2Decimal(UINT8*   aui1_ary, UINT8    ui1_round);
static void _SWDMX_AviPrsSubtitle(UCHAR*    pucBuf,
                                 UINT32*                 pt_pts_start,
                                 UINT32*                 pt_pts_end);
static void _SWDMX_AviVideoWakeUp(void);
static SWDMX_HDR_PRS_STATE_T  _SWDMX_AviBeforeDeliverUseIdx(UINT8 u1HdrType);


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

BOOL _SWDMX_AVISetDecoderInfo(UINT8 u1Type, 
    UINT32 u4Para1, 
    UINT32 u4Para2, 
    UINT32 u4Para3)
{
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
            _rSwdmxAviInfo.u4VDecType =  u4Para1;
            return TRUE;
        }
    }
    else if (u1Type == eSWDMX_SET_AUDIOTYPE)
    {
        _rSwdmxAviInfo.u4ADecType = u4Para1;
        _rSwdmxAviInfo.u4AudSampleRate = u4Para2;
        _rSwdmxAviInfo.u4AudChannelNs = u4Para3;
        return TRUE;
    }    
   else if (u1Type == eSWDMX_SET_VIDSEQHEADER)
   {
#ifndef LINUX_TURNKEY_SOLUTION
        _rSwdmxAviInfo.u4VidHeaderPtr = u4Para1;
        _rSwdmxAviInfo.u4VidHdrLength = u4Para2;
 #else
        _rSwdmxAviInfo.u4VidHdrLength = u4Para2;
        _SWDMX_GetInfo(eSWDMX_GET_INTERNAL_BUF, &_rSwdmxAviInfo.u4VidHeaderPtr, 0, 0);
 
        if(_rSwdmxAviInfo.u4VidHeaderPtr)
        {
            x_memcpy((VOID*)VIRTUAL(_rSwdmxAviInfo.u4VidHeaderPtr), 
                (VOID*)VIRTUAL(u4Para1), 
                _rSwdmxAviInfo.u4VidHdrLength);
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

BOOL _SWDMX_AVIGetInfo(UINT8 u1Type,
    UINT32* pu4Para1,
    UINT32* pu4Para2,
    UINT32* pu4Para3)
{
    UINT64 *pu8Tmp;
	LOG(5, "_SWDMX_AVIGetInfo u1Type = %d\n", u1Type);

	if(u1Type == eSWDMX_GET_CURRENT_POSITION)
    {
        if(pu4Para1 == NULL)
        {
        	LOG(1, "_SWDMX_AVIGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        pu8Tmp = (UINT64*)pu4Para1;
        *pu8Tmp = _rSwdmxAviInfo.u8CurDmxMoviPos;
        return TRUE;
    }
    else if (u1Type == eSWDMX_GET_IS_TRICK)
    {
        if(_rSwdmxAviInfo.fgEnVideo)
        {
            if ((_rSwdmxAviInfo.prVidKeyIdxTbl != NULL)&&
                (_rSwdmxAviInfo.prVidKeyIdxTbl->pt_next_partial != NULL))
            {
                if ((_rSwdmxAviInfo.prVidKeyIdxTbl->ui4_number_of_entry < AVI_KEY_TABLE_THRESHOLD)&&
                   (_rSwdmxAviInfo.prVidKeyIdxTbl->pt_next_partial->ui4_number_of_entry < AVI_KEY_TABLE_THRESHOLD))
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
            else if(_rSwdmxAviInfo.prVidKeyIdxTbl != NULL)
            {
                if (_rSwdmxAviInfo.prVidKeyIdxTbl->ui4_number_of_entry <
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
        
    return FALSE;
}

static BOOL _MM_VideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT32 u4Addr;

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    rPesInfo.u4PTS = _rSwdmxAviInfo.u4CurVidPts;
    rPesInfo.u4DTS = prPes->u4Dts;
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    rPesInfo.fgDtsValid = prPes->fgPtsDts;
    rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo.fgGop = prPes->fgGop;
    rPesInfo.fgEos = prPes->fgEOS;
    rPesInfo.u8Offset = _rSwdmxAviInfo.u8VidOffset;
    #ifdef CC_VDEC_FMT_DETECT
    rPesInfo.u4DmxFrameType = prPes->u4FrameType;
    #endif

    if (rPesInfo.fgEos)
    {
        rPesInfo.u4FifoStart = NULL;
        rPesInfo.u4FifoEnd = NULL;
        rPesInfo.u4VldReadPtr = NULL;
    }

    if (!rPesInfo.fgEos)
    {
        if (_rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {  
            if(_rSwdmxAviInfo.fgIsFirstFrame)
            {
                UNUSED(VDEC_SetTotalTime(ES0, _rSwdmxAviInfo.u4TotalTime));
            }
            
            switch (prPes->u4FrameType)
            {
            case PIC_TYPE_I:
                rPesInfo.ucPicType = MPV_I_TYPE;
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
        else if(_rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_H264)
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
            rPesInfo.u2SkipVidCount = _rSwdmxAviInfo.u2SkipVidCount;
            _rSwdmxAviInfo.u2SkipVidCount = 0;
            if(IS_IDR_NALU(rPesInfo.ucPicType))
            {
                rPesInfo.fgDtsValid = TRUE;
            }
            else
            {
                rPesInfo.fgDtsValid = FALSE;
            }
        }
        else if(_rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_MPEG4)
        {
            UCHAR *pucBuf;

            u4Addr = prPes->u4FrameAddr + 3;
            if(u4Addr >= prPes->u4BufEnd)
            {
                u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
            }
             pucBuf = (UCHAR*)VIRTUAL(u4Addr);
             
            rPesInfo.u2SkipVidCount = _rSwdmxAviInfo.u2SkipVidCount;
            rPesInfo.fgNoVdecCalPts = _rSwdmxAviInfo.fgNoCalPts;
            _rSwdmxAviInfo.u2SkipVidCount = 0;
            _rSwdmxAviInfo.fgNoCalPts = TRUE;
     
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
        }
        else if (_rSwdmxAviInfo.u4VDecType == ENUM_SWDMX_VDEC_DX311)
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
            }
        }
    }
    else if (_rSwdmxAviInfo.u4VDecType == (UINT32)ENUM_SWDMX_VDEC_MJPEG)
    {
        rPesInfo.ucPicType = I_TYPE;
    }

    VDEC_SendEs((void*)&rPesInfo);
    
    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
BOOL _SWDMX_AVIInit(ENUM_SWDMX_FMT_T eType)
{
    UNUSED(eType);
    // Initialize feeder
    //FeederOpen(FEEDER_PROGRAM_SOURCE);

    x_memset(&_rSwdmxAviInfo, 0, sizeof(SWDMX_AVI_INFO_T));

    if (!_pucAviAacBuf)
    {
        _pucAviAacBuf = 
            (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(SWDMX_AVI_AAC_ALLOC_SIZE, 16));
        if(!_pucAviAacBuf)
        {
            LOG(1, "Allocate AAC buffer failed!\n");
            return FALSE;
        }
    }

    //*********************************************************
    //FeederSetBufferSize(FEEDER_PROGRAM_SOURCE, 5*10*1024); 
    //FeederStart(FEEDER_PROGRAM_SOURCE);
    FeederInstallCallback(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_DATA_REQ_DONE, 
    _SWDMX_FeederCallBack, (UINT32)&_rFeederInfo);
    //*********************************************************

    DMX_DECODER_CALLBACKS_T rCallback; // Isaac: should be DMX_DECODER_CALLBACKS_T, Jack fix me

    rCallback.pfnSendAudioPes = AUD_SendAudioPes;
    rCallback.pfnSendVideoPes = _MM_VideoCallback;
    rCallback.pfnUpdateVideoWp = VDEC_SetWptr;
    
    DMX_SetDecoderCallbacks((const DMX_DECODER_CALLBACKS_T*)&rCallback);
    DMX_SetToDecoder(TRUE);

    // Default Speed
    _rSwdmxAviInfo.i4PrevSpeed = 1000;
    // Default Seek parameter
    _rSwdmxAviInfo.fgIsSeek = FALSE;

    //_rSwdmxAviInfo.fgPauseVidForAVSync = TRUE;

    _rSwdmxAviInfo.fgRequestData = TRUE;
    _rSwdmxAviInfo.fgIsFirstFrame = TRUE;
     _rSwdmxAviInfo.fgGetMp3Status = FALSE;
    _rSwdmxAviInfo.u4CurRepeatFrameIdx = 0xFFFFFFFF;
    //_rSwdmxAviInfo.u4ModeChangPts = 0xFFFFFFFF;

    _rSwdmxAviInfo.u8RecDmxMoviPos = 0;

    _rSwdmxAviInfo.u8VSeekPts = 0xFFFFFFFF;
    _rSwdmxAviInfo.u8VSeekPts <<= 32;
    _rSwdmxAviInfo.u8VSeekPts += 0xFFFFFFFF;

     _rSwdmxAviInfo.u8ASeekPts = 0xFFFFFFFF;
    _rSwdmxAviInfo.u8ASeekPts <<= 32;
    _rSwdmxAviInfo.u8ASeekPts += 0xFFFFFFFF;
               
    return TRUE;
}


/**
*/
VOID _SWDMX_AVISetRange(SWDMX_RANGE_LIST_T *prRangeList, UINT32 u4VidStrmID, 
                      UINT32 u4AudStrmID, UINT32 u4SubTitleID)
{
  if (prRangeList != NULL &&
       prRangeList->prDmxRangeInfo != NULL)
  {
        INT32 i = 0;
        INT32 j = 0;
        
        MM_RANGE_ELMT_IDXTBL_T *pBuldIdxTbl = 
        (MM_RANGE_ELMT_IDXTBL_T*)prRangeList->prDmxRangeInfo->pt_idxtbl_lst;

        _rSwdmxAviInfo.prDmxRangeInfo = prRangeList->prDmxRangeInfo;

//        _prRangeList = prRangeList;
        while (pBuldIdxTbl != NULL)
        {
            if ((pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_AUDIO )&&
                ( (pBuldIdxTbl->t_strm_id.u.t_stm_avi_id | AVI_AUDIO_FCC) == 
                 AVI_AUDIO_FCC))
            {
                //_rSwdmxAviInfo.fgEnAudio = TRUE;
                _rSwdmxAviInfo.prAudIdxTbl[j++] = pBuldIdxTbl;
            }
            else if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_VIDEO &&
                pBuldIdxTbl->t_strm_id.u.t_stm_avi_id == u4VidStrmID)
            {
                //_rSwdmxAviInfo.fgEnVideo = TRUE;
                _rSwdmxAviInfo.prVidIdxTbl = pBuldIdxTbl;
            }
            else if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_KEY)
            {
                _rSwdmxAviInfo.fgKeyExist = TRUE;
                
                /*if ((pBuldIdxTbl->t_strm_id.u.t_stm_avi_id == u4VidStrmID) &&
                    (((pBuldIdxTbl->t_strm_id.u.t_stm_avi_id | AVI_VIDEO_COMPRE_FCC) == AVI_VIDEO_COMPRE_FCC) ||
                    ((pBuldIdxTbl->t_strm_id.u.t_stm_avi_id | AVI_VIDEO_UNCOMPRE_FCC) == AVI_VIDEO_UNCOMPRE_FCC)))*/
                if (((pBuldIdxTbl->t_strm_id.u.t_stm_avi_id | AVI_VIDEO_COMPRE_FCC) == AVI_VIDEO_COMPRE_FCC) ||
                    ((pBuldIdxTbl->t_strm_id.u.t_stm_avi_id | AVI_VIDEO_UNCOMPRE_FCC) == AVI_VIDEO_UNCOMPRE_FCC))
                {
                    _rSwdmxAviInfo.prVidKeyIdxTbl = pBuldIdxTbl;
                    _rSwdmxAviInfo.prCurVidKeyIdxTbl = pBuldIdxTbl;
                }
                else if ((pBuldIdxTbl->t_strm_id.u.t_stm_avi_id | AVI_AUDIO_FCC) == AVI_AUDIO_FCC)
                {
                    if (i < AVI_DIVX_MAX_AUD_TRACK)
                    {
                        _rSwdmxAviInfo.parAudKeyIdxTbl[i++] = pBuldIdxTbl;
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

        if (_rSwdmxAviInfo.prVidIdxTbl)
        {
            // The upper layer (mw or playmgr should prevent 0 case for rate and scale)
            if (_rSwdmxAviInfo.prVidIdxTbl->ui4_scale)
            {
            _rSwdmxAviInfo.u4VidFps = 
                (UINT32)(_rSwdmxAviInfo.prVidIdxTbl->ui4_rate/_rSwdmxAviInfo.prVidIdxTbl->ui4_scale);
            }
            if (!_rSwdmxAviInfo.u4VidFps ||
            	_rSwdmxAviInfo.u4VidFps > 60)
            {
                _rSwdmxAviInfo.u4VidFps = 30;
            }

            LOG(5, "_rSwdmxAviInfo.u4VidFps = %ld\n", _rSwdmxAviInfo.u4VidFps);
        }

        if (_rSwdmxAviInfo.fgKeyExist)
        {
            _rSwdmxAviInfo.u1TotalATrack = i;
        }
        else if (_rSwdmxAviInfo.prAudIdxTbl)
        {
            _rSwdmxAviInfo.u1TotalATrack = j;
        }
        _rSwdmxAviInfo.u4TotalTime = prRangeList->prDmxRangeInfo->ui8_pb_duration;
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
        //prRangeList->u4CurPTS = 
        //prRangeList->prDmxRangeInfo->t_vid_start_render_pts;
  }
  else
  {
     LOG(0, "Demuxer Range is NULL\n");
  }
}


BOOL _SWDMX_AVISetStrmID(UINT8 u1HdrType,  
    UINT32 u4StrmID, UINT32 u4StrmSubID)
{
    UNUSED(u4StrmSubID);
    
    switch(u1HdrType)
    {
        case eSWDMX_STRM_TYPE_VID:
            _rSwdmxAviInfo.u4VStrmID = u4StrmID;
            LOG(5, "_SWDMX_AVISetStrmID Vid = 0x%08x\n", u4StrmID);
            break;
        case eSWDMX_STRM_TYPE_AUD:
            _rSwdmxAviInfo.u4AStrmID = u4StrmID;
            LOG(5, "_SWDMX_AVISetStrmID Aud = 0x%08x\n", u4StrmID);
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            _rSwdmxAviInfo.u4SpStrmID = u4StrmID;
            LOG(5, "_SWDMX_AVISetStrmID Sub = 0x%08x\n", u4StrmID);
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_AVIEnableStrm(UINT8 u1StrmType, VOID* pfnCbFunc)
{
    INT32 i;
    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            LOG(5, "_SWDMX_AVIEnableStrm Vid\n");
            _rSwdmxAviInfo.fgEnVideo = TRUE;
            break;
        case eSWDMX_STRM_TYPE_AUD:
            LOG(5, "_SWDMX_AVIEnableStrm Aud\n");
            for (i = 0; (i < _rSwdmxAviInfo.u1TotalATrack) && (i < AVI_DIVX_MAX_AUD_TRACK); i++)
            {
                if ((_rSwdmxAviInfo.fgKeyExist)&&(_rSwdmxAviInfo.parAudKeyIdxTbl[i]))
                {
                    if (_rSwdmxAviInfo.parAudKeyIdxTbl[i]->t_strm_id.u.t_stm_avi_id ==
                        _rSwdmxAviInfo.u4AStrmID)
                    {
                        _rSwdmxAviInfo.u1CurATrackIdx = i;
                        LOG(5, "Cur Track Idx = %ld\n", i);
                    }
                }
                else
                {    
                    if(_rSwdmxAviInfo.prAudIdxTbl[i])
                    {
                         if (_rSwdmxAviInfo.prAudIdxTbl[i]->t_strm_id.u.t_stm_avi_id ==
                           _rSwdmxAviInfo.u4AStrmID)
                        {
                            _rSwdmxAviInfo.u1CurATrackIdx = i;
                            LOG(5, "Cur Track Idx = %ld\n", i);
                        }
                    }
                }
            }
#if 0
            if (_rSwdmxAviInfo.u4NumSentAud != 0)
            {
                _rSwdmxAviInfo.fgChgATrack = TRUE;
            }
#endif
            if ((_rSwdmxAviInfo.u4PrevAStrmID != 0xFFFFFFFF)&&(_rSwdmxAviInfo.u4PrevAStrmID != 0)&&
                 (_rSwdmxAviInfo.u4PrevAStrmID != _rSwdmxAviInfo.u4AStrmID))
            {
                _rSwdmxAviInfo.fgChgATrack = TRUE;
            }
            else if (_rSwdmxAviInfo.fgAudStrmDisable && 
                (!_rSwdmxAviInfo.fgPreviouIsSeek) &&
                (!_rSwdmxAviInfo.fgSpeedChange))
            {
                LOG(5, "Audio stream is disabled and seek is not performed\n");
                _rSwdmxAviInfo.fgChgATrack = TRUE;
                _rSwdmxAviInfo.fgAudStrmDisable = FALSE;
            }
            _rSwdmxAviInfo.fgPreviouIsSeek = FALSE;
            _rSwdmxAviInfo.u4PrevAStrmID = _rSwdmxAviInfo.u4AStrmID;
            _rSwdmxAviInfo.fgEnAudio = TRUE;
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
             if ((_rSwdmxAviInfo.u4PrevSpStrmID != 0xFFFFFFFF)&&(_rSwdmxAviInfo.u4PrevSpStrmID != 0)&&
                 (_rSwdmxAviInfo.u4PrevSpStrmID != _rSwdmxAviInfo.u4SpStrmID))
            {
                _rSwdmxAviInfo.fgChgSbTrack = TRUE;
            }
            if (_rSwdmxAviInfo.fgEnSubDmx == FALSE &&
                (pfnCbFunc))
            {
                UNUSED(_SWDMX_InitDmx(eSWDMX_MEDIA_SUBTITLE, pfnCbFunc, FALSE));
                _rSwdmxAviInfo.fgEnSubDmx = TRUE;
            }
            _rSwdmxAviInfo.u4PrevSpStrmID = _rSwdmxAviInfo.u4SpStrmID;
		
            _rSwdmxAviInfo.fgEnSubTitle = TRUE;
            LOG(5, "_SWDMX_AVIEnableStrm Sub\n");
           break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_AVIDisableStrm(UINT8 u1StrmType)
{
#if 0
    DMX_MM_T rPid;

    rPid.fgEnable = FALSE;
#endif 

    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
#if 0
            if (_rSwdmxAviInfo.fgEnVDmx)
            {
                _rSwdmxAviInfo.fgEnVDmx = FALSE;
                VERIFY(DMX_MM_Set(eSWDMX_MEDIA_VIDEO, DMX_MM_FLAG_VALID, &rPid));
                DMX_MM_Free(eSWDMX_MEDIA_VIDEO);
            }
#endif
            if(_rSwdmxAviInfo.fgVDecIsPause)
            {
                _SWDMX_AviVideoWakeUp();
                _rSwdmxAviInfo.fgVDecIsPause = FALSE;
            }

            _rSwdmxAviInfo.fgEnVideo = FALSE;
            LOG(5, "_SWDMX_AVIDisableStrm Vid\n");
            break;
        case eSWDMX_STRM_TYPE_AUD:
#if 0
            if (_rSwdmxAviInfo.fgEnADmx)
            {
                _rSwdmxAviInfo.fgEnADmx = FALSE;
                VERIFY(DMX_MM_Set(eSWDMX_MEDIA_AUDIO, DMX_MM_FLAG_VALID, &rPid));
                DMX_MM_Free(eSWDMX_MEDIA_AUDIO);
            }
#endif
            _rSwdmxAviInfo.fgEnAudio = FALSE;
            _rSwdmxAviInfo.fgAudStrmDisable = TRUE;
            LOG(5, "_SWDMX_AVIDisableStrm Aud\n");
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
#if 0
            if (_rSwdmxAviInfo.fgEnSubDmx)
            {
                _rSwdmxAviInfo.fgEnSubDmx = FALSE;
                VERIFY(DMX_MM_Set(eSWDMX_MEDIA_SUBTITLE, DMX_MM_FLAG_VALID, &rPid));
                DMX_MM_Free(eSWDMX_MEDIA_SUBTITLE);
            }
#endif
            _rSwdmxAviInfo.fgEnSubTitle = FALSE;
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
	SWDMX_RANGE_LIST_T* prRangeList,
	INT32 i4PlaySpeed,
	UINT64 u8SeekPos,
	BOOL fgSpeedChange)
{
    UNUSED(i4PlaySpeed);
    UNUSED(fgSpeedChange);

    UINT32 u4CurPts = 0;
    UINT32 u4PrsState;
    UINT32 u4SwdmxInfoAddr;
    SWDMX_INFO_T *prSwdmxInfo = NULL;
    VDEC_PTS_INFO_T rPTSInfo;

    x_memset((void*)&rPTSInfo, 0, sizeof(VDEC_PTS_INFO_T));
    if (prRangeList->prDmxRangeInfo == NULL)
    {
        LOG(5, "This range is empty\n");
        return eSWDMX_HDR_PRS_EMPTY_RANGE;
    }

    if (!_SWDMX_GetInfo(eSWDMX_GET_INTERNAL_INFO, (UINT32*)&u4SwdmxInfoAddr, 0, 0))
    {
        LOG(5, "Fail to Get swdmx internal info\n");
        return eSWDMX_HDR_PRS_FAIL;
    }

    if (u4SwdmxInfoAddr != 0)
    {
        prSwdmxInfo = (SWDMX_INFO_T *)u4SwdmxInfoAddr;
        if ((prSwdmxInfo->eSwDmxPrevousStatus == eSWDMX_STATE_PAUSE) &&
            (i4PlaySpeed == 1000) && 
            (_rSwdmxAviInfo.fgPreviouIsSeek))
        {
            LOG(5, "PAUSE -> SEEK -> RESUME set stc value to 0x%08x\n",
                _rSwdmxAviInfo.u4RecSeekTime);
            STC_SetStcValue(_rSwdmxAviInfo.u4RecSeekTime);
		    _rSwdmxAviInfo.fgPreviouIsSeek = FALSE;
        }
        else if ((_rSwdmxAviInfo.fgPreviouIsSeek)&&
            (i4PlaySpeed != 1000)&&(i4PlaySpeed != 1))
        {
            _rSwdmxAviInfo.fgPreviouIsSeek = FALSE;
        }
    }

    if ((_rSwdmxAviInfo.fgEnVideo) && (!_rSwdmxAviInfo.fgEnVDmx))
    {
        UNUSED(_SWDMX_InitDmx(eSWDMX_MEDIA_VIDEO, NULL, FALSE));
        _rSwdmxAviInfo.fgEnVDmx = TRUE;
    }

    if ((_rSwdmxAviInfo.fgEnAudio) && (!_rSwdmxAviInfo.fgEnADmx))
    {
        UNUSED(_SWDMX_InitDmx(eSWDMX_MEDIA_AUDIO, NULL, FALSE));
        _rSwdmxAviInfo.fgEnADmx = TRUE;
    }

    if ((!_rSwdmxAviInfo.fgBeginToPlay) &&
        (_rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts != 0))
    {
#ifdef CC_DLNA_SUPPORT
        if (INPUT_SRC != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        {

            UINT32 u4FeederReadSize;
          
            // For DLNA playback
            if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
              FEEDER_E_OK)
            {
                LOG(1, "Feeder input buffer error.\n");
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
            
            u4FeederReadSize = _rFeederInfo.u4Size - MIN_READ_SIZE;
            FeederSetReadyThreshold(u4FeederReadSize);
            FeederSetUnderFlowThreshold(u4FeederReadSize/4);
       }
#endif

        if(_rSwdmxAviInfo.prVidIdxTbl)
        {
            rPTSInfo.u8Scale = (UINT64)_rSwdmxAviInfo.prVidIdxTbl->ui4_scale * DEMUX_AVI_AV_PERIOD;
            rPTSInfo.u4Rate = _rSwdmxAviInfo.prVidIdxTbl->ui4_rate;
        }
        if(_rSwdmxAviInfo.u4VidHeaderPtr != NULL)
        {
            _SWDMX_AviBeforeDeliverUseIdx(eSWDMX_MEDIA_VIDEO);
            UNUSED(_SWDMX_AviSeek(_rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts, prRangeList));
            LOG(5, "Video Start Render Pts = %lld\n", 
                _rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts);

            _rSwdmxAviInfo.u8VSeekPts = 
                _rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts;
            
            _rSwdmxAviInfo.fgIsSeek = TRUE;
        }
        _rSwdmxAviInfo.u8ASeekPts = 
                 _rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts;

        if (_rSwdmxAviInfo.fgEnVideo)
        {
            VDEC_StartPTS(0, TRUE, 0, &rPTSInfo);
            VDEC_SetRenderFromPts(ES0, _rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts);
        
            VDP_SetPauseMm(0, TRUE);
            VDEC_Pause(ES0);
            _rSwdmxAviInfo.fgVDecIsPause = TRUE;
        }
        
        STC_StopStc();
        STC_SetStcValue(_rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts - 45000);
        _rSwdmxAviInfo.fgBeginToPlay = TRUE;
    }
    else if ((!_rSwdmxAviInfo.fgBeginToPlay) &&
        (_rSwdmxAviInfo.prDmxRangeInfo->t_aud_start_render_pts != 0))
    {
#if 0
#ifdef CC_DLNA_SUPPORT
        if (INPUT_SRC != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        {

            UINT32 u4FeederReadSize;
          
            // For DLNA playback
            if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
              FEEDER_E_OK)
            {
                LOG(1, "Feeder input buffer error.\n");
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
            
            u4FeederReadSize = _rFeederInfo.u4Size - MIN_READ_SIZE;
            FeederSetReadyThreshold(u4FeederReadSize);
            FeederSetUnderFlowThreshold(u4FeederReadSize/4);
       }
#endif
#endif
        UNUSED(_SWDMX_AviSeek(_rSwdmxAviInfo.prDmxRangeInfo->t_aud_start_render_pts, prRangeList));
        LOG(5, "Audio Start Render Pts = %lld\n", 
            _rSwdmxAviInfo.prDmxRangeInfo->t_aud_start_render_pts);

        _rSwdmxAviInfo.fgIsSeek = TRUE;

        _rSwdmxAviInfo.u8ASeekPts = 
                 _rSwdmxAviInfo.prDmxRangeInfo->t_aud_start_render_pts;

        STC_StopStc();
        STC_SetStcValue(_rSwdmxAviInfo.prDmxRangeInfo->t_aud_start_render_pts - 45000);
        _rSwdmxAviInfo.fgBeginToPlay = TRUE;
    }

    if (!_rSwdmxAviInfo.fgBeginToPlay &&
        i4PlaySpeed == 1000)
    {
#ifdef CC_DLNA_SUPPORT
        if (INPUT_SRC != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        {
            UINT32 u4FeederReadSize;

            // For DLNA playback
            if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
              FEEDER_E_OK)
            {
                LOG(1, "Feeder input buffer error.\n");
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
            
            u4FeederReadSize = _rFeederInfo.u4Size - MIN_READ_SIZE;
            FeederSetReadyThreshold(u4FeederReadSize);
            FeederSetUnderFlowThreshold(u4FeederReadSize/4);
        }
#endif
        if(_rSwdmxAviInfo.prVidIdxTbl)
        {
            rPTSInfo.u8Scale = (UINT64)_rSwdmxAviInfo.prVidIdxTbl->ui4_scale * DEMUX_AVI_AV_PERIOD;
            rPTSInfo.u4Rate = _rSwdmxAviInfo.prVidIdxTbl->ui4_rate;
        }
        
        if (_rSwdmxAviInfo.fgEnAudio && _rSwdmxAviInfo.fgEnVideo)
        {
            VDEC_StartPTS(0, TRUE, 0, &rPTSInfo);
            VDEC_SetRenderFromPts(ES0, _rSwdmxAviInfo.u4CurVidPts);
        
            VDP_SetPauseMm(0, TRUE);
            VDEC_Pause(ES0);
            _rSwdmxAviInfo.fgVDecIsPause = TRUE;
       
            if (_rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)
            {
                if (prRangeList->prDmxRangeInfo->t_aud_start_pts != 0)
                {
                    _rSwdmxAviInfo.u4AudPts[_rSwdmxAviInfo.u1CurATrackIdx] = 
                        prRangeList->prDmxRangeInfo->t_aud_start_pts;
                }
                
                AUD_SetStartPts(0, _rSwdmxAviInfo.u4AudPts[_rSwdmxAviInfo.u1CurATrackIdx]);
                _rSwdmxAviInfo.u4TriggerAudPts = 
                    _rSwdmxAviInfo.u4AudPts[_rSwdmxAviInfo.u1CurATrackIdx];
            }

        }
        // audio only
        else if (_rSwdmxAviInfo.fgEnAudio)
        {
            if (_rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)
            {
                AUD_SetStartPts(0, _rSwdmxAviInfo.u4AudPts[_rSwdmxAviInfo.u1CurATrackIdx]);
                _rSwdmxAviInfo.u4AudStartPts = _rSwdmxAviInfo.u4AudPts[_rSwdmxAviInfo.u1CurATrackIdx];
            }
            else
            {
                LOG(1, "Out of Audio Track Number\n");
                AUD_SetStartPts(0, 0);
                _rSwdmxAviInfo.u4AudStartPts = 0;
            }
        }
        // video only
        else if (_rSwdmxAviInfo.fgEnVideo)
        {
            VDEC_StartPTS(0, TRUE, 0, &rPTSInfo);
            VDEC_SetRenderFromPts(ES0, _rSwdmxAviInfo.u4CurVidPts);

            VDP_SetPauseMm(0, TRUE);
            VDEC_Pause(ES0);
            _rSwdmxAviInfo.fgVDecIsPause = TRUE;
        }

        // set stc for starting playback
        STC_StopStc();
        STC_SetStcValue(_rSwdmxAviInfo.u4CurVidPts - 45000);

        _rSwdmxAviInfo.fgBeginToPlay = TRUE;
    }     

    if(i4PlaySpeed != 1000)
    {
    	  if(_rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)
    	  {
    	    if( (_rSwdmxAviInfo.fgKeyExist)&&(_rSwdmxAviInfo.prAudIdxTbl[0] != NULL))
            {
                prRangeList->u4CurRangeAudIdx = _rSwdmxAviInfo.prAudIdxTbl[0]->ui4_number_of_entry;
            }
            else if (_rSwdmxAviInfo.prAudIdxTbl [_rSwdmxAviInfo.u1CurATrackIdx]!= NULL)
            {
                prRangeList->u4CurRangeAudIdx = _rSwdmxAviInfo.prAudIdxTbl[_rSwdmxAviInfo.u1CurATrackIdx]->ui4_number_of_entry;
            }
        }
    }

    // Reset some values for state change.
    if (_rSwdmxAviInfo.fgChgATrack ||
        fgSpeedChange)
    {
        if (_rSwdmxAviInfo.fgDeliverVEos)
        {
            UINT32 u4Flag;
            DMX_MM_T rDmxMMInfo;

            rDmxMMInfo.fgEnable = TRUE;
            rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;

            switch (_rSwdmxAviInfo.u4VDecType)
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
            if (!DMX_MM_Set(eSWDMX_MEDIA_VIDEO, u4Flag, &rDmxMMInfo))
            {
                LOG(3, "Fail to reset demuxer\n");
            }
            
            _rSwdmxAviInfo.fgDeliverVEos = FALSE;
        }
        if (_rSwdmxAviInfo.fgDeliverAEos)
        {
            _rSwdmxAviInfo.fgDeliverAEos = FALSE;
            AUD_ClearEOSFlag(0);
        }
                 
#ifdef CC_DLNA_SUPPORT
        // DLNA
        FeederSetInfo(FEEDER_SWDMX_EOS, FALSE);
#endif
        _rSwdmxAviInfo.fgAudIsReady = FALSE;
        _rSwdmxAviInfo.fgQAud = FALSE;
        _rSwdmxAviInfo.fgAudIsLoaded = FALSE;
        _rSwdmxAviInfo.fgBadAudio = FALSE;
        
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }

    if (fgSpeedChange)
    {
        _rSwdmxAviInfo.fgSpeedChange = TRUE;

        u4CurPts = VDP_GetPts(0);
        LOG(5, "Speed change while VDP Pts = 0x%08x\n", u4CurPts);

        _rSwdmxAviInfo.u4NumSentSub = 0;
        _rSwdmxAviInfo.u4NumSentAud = 0;

#ifdef CC_DLNA_SUPPORT
        _rSwdmxAviInfo.fgFeederIsReady = FALSE;
#endif

        // 1X -> 2X
        if ((_rSwdmxAviInfo.i4PrevSpeed == 1000) &&
            (i4PlaySpeed == 2000))
        {
            if (_rSwdmxAviInfo.u8RecDmxMoviPos != 0)
            {
                _SWDMX_FlushReqMsgQ();
                _rSwdmxAviInfo.i4ReqReadTimes = 0;
                _rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
                _rSwdmxAviInfo.u8CurDmxMoviPos = _rSwdmxAviInfo.u8RecDmxMoviPos;
                
            }

#ifdef CC_DLNA_SUPPORT
            if (INPUT_SRC != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
            {
                // Set active pic to 255
                DMX_AudHandler_SetActivePidx(DMX_AUDIO_HANDLER_NULL_PIDX);
            }
#endif
        }

        _rSwdmxAviInfo.u8RecDmxMoviPos = 0;
        //_rSwdmxAviInfo.u4ModeChangPts = 0xFFFFFFFF;
        _rSwdmxAviInfo.u4ChangeMode = 0;
        _rSwdmxAviInfo.fgRecSpeedChangeForAud = FALSE;
        _rSwdmxAviInfo.u4VidRepeatFrame = 0;
        _rSwdmxAviInfo.fgRequestData = TRUE;
        _rSwdmxAviInfo.fgPreviouIsSeek = FALSE;
        _rSwdmxAviInfo.u8VSeekPts = 0xFFFFFFFF;
        _rSwdmxAviInfo.u8VSeekPts <<= 32;
        _rSwdmxAviInfo.u8VSeekPts += 0xFFFFFFFF;

        _rSwdmxAviInfo.u8ASeekPts = 0xFFFFFFFF;
        _rSwdmxAviInfo.u8ASeekPts <<= 32;
        _rSwdmxAviInfo.u8ASeekPts += 0xFFFFFFFF;

        if(_rSwdmxAviInfo.fgVDecIsPause)
        {
            _SWDMX_AviVideoWakeUp();
            _rSwdmxAviInfo.fgVDecIsPause = FALSE;
        }

        if(_rSwdmxAviInfo.prVidIdxTbl)
        {
            rPTSInfo.u8Scale = (UINT64)_rSwdmxAviInfo.prVidIdxTbl->ui4_scale * DEMUX_AVI_AV_PERIOD;
            rPTSInfo.u4Rate = _rSwdmxAviInfo.prVidIdxTbl->ui4_rate;
        }   
        // seek
        if(((UINT32)(u8SeekPos>>32) != 0xFFFFFFFF)
            || ((UINT32)(u8SeekPos) != 0xFFFFFFFF))
        {
            UNUSED(_SWDMX_AviSeek(u8SeekPos, prRangeList));
            LOG(5, "u8SeekPos = %lld\n", u8SeekPos);

            _rSwdmxAviInfo.u8VSeekPts = u8SeekPos;
            _rSwdmxAviInfo.u8ASeekPts = u8SeekPos;
            _rSwdmxAviInfo.fgFeederInvalid = TRUE;
            _rSwdmxAviInfo.fgPreviouIsSeek = TRUE;
            _rSwdmxAviInfo.u4RecSeekTime = u8SeekPos;

            if (_rSwdmxAviInfo.fgMp3Abr)
            {
                LOG(5, "ABR Mp3\n");
                _rSwdmxAviInfo.u8ASeekPts = 0xFFFFFFFF;
                _rSwdmxAviInfo.u8ASeekPts <<= 32;
                _rSwdmxAviInfo.u8ASeekPts += 0xFFFFFFFF;
                if(_rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)
                {
                    AUD_SetStartPts(0, _rSwdmxAviInfo.u4AudPts[_rSwdmxAviInfo.u1CurATrackIdx]);
                }
            }
            
            if (_rSwdmxAviInfo.fgEnVideo)
            {
                MPV_FlushEsmQ(0, TRUE);
                VDEC_ReleaseDispQ(0);

                if(i4PlaySpeed >0)
                {    
                    VDEC_SetRenderPts(ES0, u8SeekPos);
                }
                else
                {    
                    VDEC_SetRenderPts(ES0, 0);
                }

                VDEC_StartPTS(ES0, TRUE, u8SeekPos, NULL);

                DMX_MM_FlushBuffer(eSWDMX_MEDIA_VIDEO);
            }

            DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO);

            // set stc
            STC_StopStc();
            STC_SetStcValue(u8SeekPos);
          
           #ifdef CC_DLNA_SUPPORT
            {
                UINT8 u1Idx = 0;
                UINT32 u4APts = 0;
                
                for ( ;u1Idx < _rSwdmxAviInfo.u1TotalATrack; u1Idx++)
                {
                        DMX_AudHandler_SeekAndUpdate(u1Idx + AVI_AUD_PREBUF_IDX,
                        0xffffffff, &u4APts);
                }
            }
            #endif
        }
        // 2X -> 1X
        else if (((_rSwdmxAviInfo.i4PrevSpeed == 2000) || 
                     (_rSwdmxAviInfo.i4PrevSpeed == 1) || 
                     ((_rSwdmxAviInfo.i4PrevSpeed < 1000) && (_rSwdmxAviInfo.i4PrevSpeed > 1))) &&
                    ((i4PlaySpeed == 1000)||(i4PlaySpeed == 2000)))
        {
            UINT8 u1TrackIdx = _rSwdmxAviInfo.u1CurATrackIdx;
            UINT32 u4AVPtsDiff  = 0;

            u4CurPts = VDP_GetPts(0);
            //STC_SetStcValue(u4CurPts);

            if ((_rSwdmxAviInfo.i4PrevSpeed == 1) || 
                ((_rSwdmxAviInfo.i4PrevSpeed < 1000) && (_rSwdmxAviInfo.i4PrevSpeed > 1)))
            {
                STC_StartStc();
            }

            if ((u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK)&&(i4PlaySpeed == 1000))
            {
                LOG(5, "Avi Speed 2X -> 1X Cur Vid Pts = 0x%08x, Cur Aud Pts = 0x%08x\n",
                _rSwdmxAviInfo.u4CurVidPts, _rSwdmxAviInfo.u4AudPts[u1TrackIdx]);

                DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO); 

#ifdef CC_DLNA_SUPPORT
                if (INPUT_SRC == MM_SRC_TYPE_HIGH_SPEED_STORAGE)
#endif
                {
                    if(_rSwdmxAviInfo.u4AudPts[u1TrackIdx] > u4CurPts)
                    {
                        u4AVPtsDiff = _rSwdmxAviInfo.u4AudPts[u1TrackIdx]  - u4CurPts;
                    }
                    
                    if (u4AVPtsDiff >= 45000)
                    {
                        LOG(5, "u4AVPtsDiff > 45000\n");

                        if (_rSwdmxAviInfo.u8CurDmxMoviPos > _rSwdmxAviInfo.u8RecDmxMoviPos)
                        {
                            _rSwdmxAviInfo.u8RecDmxMoviPos = _rSwdmxAviInfo.u8CurDmxMoviPos;
                        }

                        if (_SWDMX_AviSeek(u4CurPts, prRangeList))
                        {
                            //_rSwdmxAviInfo.u4ModeChangPts = u4CurPts + 90000; 
                            _rSwdmxAviInfo.u4ChangeMode |= AVI_CHANGE_AUDIO_MODE;
                        }
                        else
                        {
                            _rSwdmxAviInfo.u8RecDmxMoviPos = 0;
                            _rSwdmxAviInfo.u4TriggerAudPts = _rSwdmxAviInfo.u4AudPts[u1TrackIdx];
                            AUD_SetStartPts(0, _rSwdmxAviInfo.u4AudPts[u1TrackIdx]);
                            _rSwdmxAviInfo.u4AudStartPts = _rSwdmxAviInfo.u4AudPts[u1TrackIdx];
                        }
                    }
                    else
                    {
                        _rSwdmxAviInfo.u4TriggerAudPts = _rSwdmxAviInfo.u4AudPts[u1TrackIdx];
                        AUD_SetStartPts(0, _rSwdmxAviInfo.u4AudPts[u1TrackIdx]);
                        _rSwdmxAviInfo.u4AudStartPts = _rSwdmxAviInfo.u4AudPts[u1TrackIdx];
                    }
                }
#ifdef CC_DLNA_SUPPORT
                else
                {
                    _rSwdmxAviInfo.fgRecSpeedChangeForAud = TRUE;
                    _rSwdmxAviInfo.i4RecPrevSpeedForAud = _rSwdmxAviInfo.i4PrevSpeed;
                }
#endif

                LOG(5, "Rec Movi Pos = %ld\n",
                    _rSwdmxAviInfo.u8RecDmxMoviPos);
                LOG(5, "Cur Dmx Pos = %ld, \n",
                    _rSwdmxAviInfo.u8CurDmxMoviPos);
            }
            else
            {
                LOG(1, "Out of Max Audio Track Number\n");
            }
        }
        else if (_rSwdmxAviInfo.i4PrevSpeed < 0 &&
            (i4PlaySpeed == 2000 || i4PlaySpeed == 1000))
        {
            UNUSED(_SWDMX_AviSeek(u4CurPts, prRangeList));

            MPV_FlushEsmQ(0, TRUE);
            VDEC_ReleaseDispQ(0);

            DMX_MM_FlushBuffer(eSWDMX_MEDIA_VIDEO); 
            DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO); 

            _SWDMX_FlushReqMsgQ();
            _rSwdmxAviInfo.i4ReqReadTimes = 0;
            _rSwdmxAviInfo.fgReqDataMiss = FALSE;

            if (_rSwdmxAviInfo.fgEnVideo)
            {
                VDEC_SetRenderFromPts(0, 
                    _rSwdmxAviInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxAviInfo.u4CurSentKey].ui4_pts);

                VDEC_StartPTS(0, TRUE, 
                    _rSwdmxAviInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxAviInfo.u4CurSentKey].ui4_pts, 
                    &rPTSInfo);
                if(_rSwdmxAviInfo.fgMp3Abr)
                {
        	          _rSwdmxAviInfo.u8VSeekPts = u4CurPts;
                } 	      
            }

            if ((i4PlaySpeed == 1000) && 
                (_rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK))
            {
                _rSwdmxAviInfo.u4TriggerAudPts = _rSwdmxAviInfo.u4AudPts[_rSwdmxAviInfo.u1CurATrackIdx];

                LOG(5, "VDP trigger audio = 0x%08x\n", 
                    _rSwdmxAviInfo.u4AudPts[_rSwdmxAviInfo.u1CurATrackIdx]);

                if(!_rSwdmxAviInfo.fgMp3Abr)
                {	
                		AUD_SetStartPts(0, _rSwdmxAviInfo.u4AudPts[_rSwdmxAviInfo.u1CurATrackIdx]);
                }
                else
                {
                	  AUD_SetStartPts(0,u4CurPts);
                }
                _rSwdmxAviInfo.u4AudStartPts = _rSwdmxAviInfo.u4AudPts[_rSwdmxAviInfo.u1CurATrackIdx];

                _rSwdmxAviInfo.fgRecSpeedChangeForAud = TRUE;
                _rSwdmxAviInfo.i4RecPrevSpeedForAud = _rSwdmxAviInfo.i4PrevSpeed;

                // set stc
            }

            STC_StopStc();
            STC_SetStcValue(u4CurPts);
			
            if(i4PlaySpeed == 2000)
            {
                STC_StartStc();
            }
            LOG(5, "Avi Speed -32X/-16X/-8X/-4X/-2X -> 1X\n");
        }
        else if (!(_rSwdmxAviInfo.i4PrevSpeed == 1000 && (i4PlaySpeed == 2000 || i4PlaySpeed == 1)))
        {
            if (!(((_rSwdmxAviInfo.i4PrevSpeed == 1)&&(i4PlaySpeed == 2000))||
               ( (_rSwdmxAviInfo.i4PrevSpeed == 2000 ||(_rSwdmxAviInfo.i4PrevSpeed == 1) )&&
               ((i4PlaySpeed<1000)&&(i4PlaySpeed>1)))))
            {
                UNUSED(_SWDMX_AviSeek(u4CurPts, prRangeList));

                MPV_FlushEsmQ(0, TRUE);
                VDEC_ReleaseDispQ(0);

                DMX_MM_FlushBuffer(eSWDMX_MEDIA_VIDEO); 
                DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO); 

                _SWDMX_FlushReqMsgQ();
                _rSwdmxAviInfo.i4ReqReadTimes = 0;
                _rSwdmxAviInfo.fgReqDataMiss = FALSE;

                _rSwdmxAviInfo.fgRecSpeedChangeForAud = TRUE;
                _rSwdmxAviInfo.i4RecPrevSpeedForAud = _rSwdmxAviInfo.i4PrevSpeed;
				
		if (i4PlaySpeed > 1000)
                {
                    VDEC_SetRenderFromPts(0, u4CurPts);
                    //avc test
                    VDEC_StartPTS(0, FALSE, 
                        _rSwdmxAviInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxAviInfo.u4CurSentKey].ui4_pts, 
                        &rPTSInfo);
                }
               else if (i4PlaySpeed <= 1000 &&
                           i4PlaySpeed > 0)
               {
                    VDEC_SetRenderFromPts(0, u4CurPts);
                    //avc test
                    VDEC_StartPTS(0, TRUE, 
                        _rSwdmxAviInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxAviInfo.u4CurSentKey].ui4_pts, 
                        &rPTSInfo);
                    
                    if(_rSwdmxAviInfo.fgMp3Abr)
                    {
            	        _rSwdmxAviInfo.u8VSeekPts = u4CurPts;
                    }	   
               }
               else
               {						  
                   VDEC_SetRenderFromPts(0, 
                        _rSwdmxAviInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts);
                   VDEC_StartPTS(0, FALSE, 
                       _rSwdmxAviInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts, 
                    &rPTSInfo);                
               }
            }
            else
            {
    	         STC_StartStc();
            }

                if (i4PlaySpeed > 2000 || i4PlaySpeed < 0)
                {
                    _rSwdmxAviInfo.fgStartTrick = TRUE;
                }

                // Things for 32/16/8/4/-2/-4/-8/-16/-32 -> 1X are the same
                if ((_rSwdmxAviInfo.i4PrevSpeed > 2000 || _rSwdmxAviInfo.i4PrevSpeed < 0) &&
                    (i4PlaySpeed == 1000) &&
                    (_rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK))
                {
                    UINT8 u1TrackIdx;

                    u1TrackIdx = _rSwdmxAviInfo.u1CurATrackIdx;
                    _rSwdmxAviInfo.u4TriggerAudPts = _rSwdmxAviInfo.u4AudPts[u1TrackIdx];
                    if(!_rSwdmxAviInfo.fgMp3Abr)
                    {	
                    		AUD_SetStartPts(0, _rSwdmxAviInfo.u4AudPts[_rSwdmxAviInfo.u1CurATrackIdx]);
                    }
                    else
                    {
                    	  AUD_SetStartPts(0,u4CurPts);
                    }
                    _rSwdmxAviInfo.u4AudStartPts = _rSwdmxAviInfo.u4AudPts[_rSwdmxAviInfo.u1CurATrackIdx];

                    // set stc
                    STC_StopStc();
                    STC_SetStcValue(u4CurPts);
                }
                LOG(5, "_rSwdmxAviInfo.i4PrevSpeed != 1000 i4PlaySpeed != 2000\n");
                // don't need to handle 1X->2X
        }
        _rSwdmxAviInfo.i4PrevSpeed = i4PlaySpeed;
    }

    if ((_rSwdmxAviInfo.fgChgATrack || _rSwdmxAviInfo.fgChgSbTrack)
        &&(i4PlaySpeed>0)&&(! _rSwdmxAviInfo.fgPreviouIsSeek)&&
        (!_rSwdmxAviInfo.fgSpeedChange))
    {
#ifdef CC_DLNA_SUPPORT
        if (INPUT_SRC == MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        {
#endif
            if(_rSwdmxAviInfo.fgEnVideo)
            {
                u4CurPts = VDP_GetPts(0);
            }
            else
            {
                u4CurPts = STC_GetStcValue();
            }

            _SWDMX_FlushReqMsgQ();
  
            if (_rSwdmxAviInfo.u8CurDmxMoviPos > _rSwdmxAviInfo.u8RecDmxMoviPos)
            {
                _rSwdmxAviInfo.u8RecDmxMoviPos = _rSwdmxAviInfo.u8CurDmxMoviPos;
            }
            
            UNUSED(_SWDMX_AviSeek(u4CurPts, prRangeList));

            if (_rSwdmxAviInfo.fgChgSbTrack)
            {
                _rSwdmxAviInfo.fgChgSbTrack = FALSE;
                _rSwdmxAviInfo.u4ChangeMode |= AVI_CHANGE_SUBTITLE_MODE;
                LOG(5, "Change SubTrack, Rec Movi Pos = %ld\n",
                    _rSwdmxAviInfo.u8RecDmxMoviPos);
                LOG(5, "Change SubTrack, Cur Dmx Pos = %ld, \n",
                    _rSwdmxAviInfo.u8CurDmxMoviPos);
                LOG(5, "VDP Pts = 0x%08x\n", u4CurPts);
            }

            if (_rSwdmxAviInfo.fgChgATrack)
            {   
                //_rSwdmxAviInfo.u4ModeChangPts = u4CurPts + 27000; // 0.3 sec
                DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO); 
                _rSwdmxAviInfo.fgChgATrack = FALSE;
                _rSwdmxAviInfo.u4ChangeMode |= AVI_CHANGE_AUDIO_MODE;
                
                LOG(5, "Change ATrack, Rec Movi Pos = %ld\n",
                    _rSwdmxAviInfo.u8RecDmxMoviPos);
                LOG(5, "Change ATrack, Cur Dmx Pos = %ld, \n",
                    _rSwdmxAviInfo.u8CurDmxMoviPos);
                /*LOG(5, "VDP Pts = 0x%08x, Change Pts = 0x%08x\n",
                    u4CurPts,
                    _rSwdmxAviInfo.u4ModeChangPts);*/
            }
#ifdef CC_DLNA_SUPPORT
        }
        else
        {
            LOG(5, "Input Source is not MM_SRC_TYPE_HIGH_SPEED_STORAGE\n");

            if (_rSwdmxAviInfo.fgChgSbTrack)
            {
                _rSwdmxAviInfo.fgChgSbTrack = FALSE;
                LOG(5, "Do nothing for Sub track switch\n");
            }

            if ((_rSwdmxAviInfo.fgChgATrack) &&
                (_rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK))
            {   
                BOOL fgRet = FALSE;
                UINT32 u4APts = 0;
                
                DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO); 
                _rSwdmxAviInfo.fgChgATrack = FALSE;

                if (!_rSwdmxAviInfo.fgRecSpeedChangeForAud)
                {
                    u4CurPts = VDP_GetPts(0);
                    _rSwdmxAviInfo.u4TriggerAudPts = u4CurPts + 27000;

                    fgRet = DMX_AudHandler_SeekAndUpdate(_rSwdmxAviInfo.u1CurATrackIdx + AVI_AUD_PREBUF_IDX,
                        _rSwdmxAviInfo.u4TriggerAudPts, &u4APts);

                    LOG(5, "Change Audio Track\n");

                    if (fgRet)
                    {
                        LOG(5, "Find audio in the pre-buffer\n");
                        _rSwdmxAviInfo.u4TriggerAudPts = u4APts;
                    }
                    else
                    {
                        LOG(5, "Can't find audio in the pre-buffer\n");
                        _rSwdmxAviInfo.u4TriggerAudPts = _rSwdmxAviInfo.u4AudPts[_rSwdmxAviInfo.u1CurATrackIdx];
                    }

                    AUD_SetStartPts(0, _rSwdmxAviInfo.u4TriggerAudPts);
                    DMX_AudHandler_SetActivePidx(_rSwdmxAviInfo.u1CurATrackIdx + AVI_AUD_PREBUF_IDX);

                    LOG(5, "_rSwdmxAviInfo.u4TriggerAudPts = 0x%08x\n", _rSwdmxAviInfo.u4TriggerAudPts);
                }
            }
        }
#endif
    }

    if(_rSwdmxAviInfo.fgChgATrack || _rSwdmxAviInfo.fgChgSbTrack)
    {
        if(_rSwdmxAviInfo.fgChgATrack)
        {
            _rSwdmxAviInfo.fgChgATrack = FALSE;
        }
    }
    
    if ((_rSwdmxAviInfo.prCurVidKeyIdxTbl != 0) &&
        ((i4PlaySpeed > 2000) || (i4PlaySpeed < 0)))
    {
        u4PrsState = _SWDMX_AviHdrPrsUseIdx(prRangeList, 
                                                                i4PlaySpeed, 
                                                                u8SeekPos, 
                                                                fgSpeedChange);
    }
    else if ((_rSwdmxAviInfo.prCurVidKeyIdxTbl == 0) &&
        ((i4PlaySpeed > 2000) || (i4PlaySpeed < 0)))
    {
        return eSWDMX_HDR_PRS_FAIL;
    }
    else
    {
        u4PrsState = _SWDMX_AviHdrPrsWithoutIdx(prRangeList,
                                                                       i4PlaySpeed,
                                                                       u8SeekPos,
                                                                       fgSpeedChange);
    }

    return (SWDMX_HDR_PRS_STATE_T)u4PrsState;
}

static SWDMX_HDR_PRS_STATE_T _SWDMX_AviHdrPrsUseIdx(
        SWDMX_RANGE_LIST_T* prRangeList,
        INT32 i4PlaySpeed,
	 UINT64 u8SeekPos,
	 BOOL fgSpeedChange)
{
    BOOL fgVRangeDone = TRUE;    
    UINT32 *pu4CurIdx = NULL;
    FEEDER_REQ_DATA_T rFeederReqData;
    MM_RANGE_ELMT_IDXTBL_T *prKeyTbl = NULL;

    if (_rSwdmxAviInfo.fgFeederInvalid)
    {
        INT32 i;
        LOG(3, "Feeder is invalid, Cur Feeder ID = %ld, req times = %ld, Cur Dmx Pos = %lld\n",
            _rSwdmxAviInfo.u4FeederReqID,
            _rSwdmxAviInfo.i4ReqReadTimes,
            _rSwdmxAviInfo.u8CurDmxMoviPos);

        for (i = 0; i < _rSwdmxAviInfo.i4ReqReadTimes; i++)
        {
            _SWDMX_AviGetDataFromFeeder(&rFeederReqData);
        }
        
        _SWDMX_AviResetFeeder();

        _rSwdmxAviInfo.fgIsSeek = TRUE;
        _rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
        _rSwdmxAviInfo.u8CurMoviPos = _rSwdmxAviInfo.u8CurDmxMoviPos;
        _rSwdmxAviInfo.fgFeederInvalid = FALSE;
        _rSwdmxAviInfo.fgReqDataMiss = FALSE;
        _rSwdmxAviInfo.i4ReqReadTimes = 0;
        _rSwdmxAviInfo.u4FeederReqID = 0;
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    if (_rSwdmxAviInfo.fgDeliverVEos)
    {
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        return eSWDMX_HDR_PRS_SUCCEED;
    }
    
    if (_rSwdmxAviInfo.prCurVidKeyIdxTbl == 0)
    {
        LOG(1, "There is no avi key table\n");
        return eSWDMX_HDR_PRS_EMPTY_RANGE;
    }

    prKeyTbl = _rSwdmxAviInfo.prCurVidKeyIdxTbl;

    if (_rSwdmxAviInfo.u4CurSentKey < _rSwdmxAviInfo.prCurVidKeyIdxTbl->ui4_number_of_entry)
    {
        _SWDMX_AviResetFeeder();

        fgVRangeDone = FALSE;

        pu4CurIdx = &_rSwdmxAviInfo.u4CurSentKey;
        _rSwdmxAviInfo.u4CurVidPts = _rSwdmxAviInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;

        LOG(7, "_rSwdmxAviInfo.u4CurSentKey = %ld\n", _rSwdmxAviInfo.u4CurSentKey);
        LOG(7, "Key Pts = %ld\n", _rSwdmxAviInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts);

        _SWDMX_DeliverAviDataUseIdx(i4PlaySpeed, 
            _rSwdmxAviInfo.prCurVidKeyIdxTbl, 
            pu4CurIdx, 
            prRangeList,
            eSWDMX_MEDIA_VIDEO);

        if ((i4PlaySpeed < 0) &&
            (_rSwdmxAviInfo.u4CurSentKey == 0))
        {
            LOG(5, "Fast Backward to the first key fram\n");
        }


        LOG(7, "Current key frame movi position = 0x%08x\n", _rSwdmxAviInfo.u8CurMoviPos);
    }
    else
    {
        _rSwdmxAviInfo.u4VidRepeatFrame = 0;
    }

    // The last key frame in current node
    if (_rSwdmxAviInfo.u4VidRepeatFrame == 0)
    {
        if (i4PlaySpeed > 2000 && 
        (_rSwdmxAviInfo.u4CurSentKey == _rSwdmxAviInfo.prCurVidKeyIdxTbl->ui4_number_of_entry))
        {
            if (_rSwdmxAviInfo.prCurVidKeyIdxTbl->pt_next_partial)
            {
                _rSwdmxAviInfo.prCurVidKeyIdxTbl = _rSwdmxAviInfo.prCurVidKeyIdxTbl->pt_next_partial;
                _rSwdmxAviInfo.u4CurSentKey = 0;
                _rSwdmxAviInfo.u4CurRepeatFrameIdx = 0;
            }
            else if ((_rSwdmxAviInfo.prCurVidKeyIdxTbl->pt_next_partial == NULL) &&
                (_rSwdmxAviInfo.u4CurRepeatFrameIdx == prKeyTbl->ui4_number_of_entry - 1) &&
                !_rSwdmxAviInfo.fgDeliverVEos)
            {
                LOG(5, "Fast Forward _SWDMX_AviHdrPrsUseIdx Deliver Video Eos\n");
                if (!_SWDMX_SendAviEos(eSWDMX_MEDIA_VIDEO))
                {
                    LOG(3, "Fail to send Video Eos\n");
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                }
                _rSwdmxAviInfo.fgDeliverVEos = TRUE;
            }
        }
        else if (i4PlaySpeed < 0 && (_rSwdmxAviInfo.u4CurSentKey == 0))
        {
            if (_rSwdmxAviInfo.prCurVidKeyIdxTbl != _rSwdmxAviInfo.prVidKeyIdxTbl)
            {
                MM_RANGE_ELMT_IDXTBL_T *prPrevKeyTbl = _rSwdmxAviInfo.prVidKeyIdxTbl;

                while (prPrevKeyTbl->pt_next_partial != _rSwdmxAviInfo.prCurVidKeyIdxTbl)
                {
                    prPrevKeyTbl = prPrevKeyTbl->pt_next_partial;
                }
                
                _rSwdmxAviInfo.prCurVidKeyIdxTbl = prPrevKeyTbl;
                _rSwdmxAviInfo.u4CurSentKey = prPrevKeyTbl->ui4_number_of_entry - 1;
                _rSwdmxAviInfo.u4CurRepeatFrameIdx = prPrevKeyTbl->ui4_number_of_entry - 1;
            }
            else if ((_rSwdmxAviInfo.prCurVidKeyIdxTbl == _rSwdmxAviInfo.prVidKeyIdxTbl) &&
                        (_rSwdmxAviInfo.u4CurRepeatFrameIdx == 0) &&
                        !_rSwdmxAviInfo.fgDeliverVEos)
            {
                LOG(5, "Fast Backward _SWDMX_AviHdrPrsUseIdx Deliver Video Eos\n");
                if (!_SWDMX_SendAviEos(eSWDMX_MEDIA_VIDEO))
                {
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                }
                _rSwdmxAviInfo.fgDeliverVEos = TRUE;
            }
        }
    }

    if (fgVRangeDone)  
    {
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
    }
    
    return eSWDMX_HDR_PRS_SUCCEED;
}        


static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverAviDataUseIdx(
    INT32 i4PlaySpeed,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl, 
    UINT32* pu4CurIdx, 
    SWDMX_RANGE_LIST_T* prRangeList,
    UINT8 u1HdrType)
{
    BOOL fgRangeEnd = FALSE;
    UINT8 u1ReadDrmSize = 0;
    UINT32 u4DmxAvailSize = 0;
    UINT32 u4Size = 0, u4ConsumeSize = 0, u4Ret = 0;
    UINT64 u8VidOffset = 0;

    DMX_MM_DATA_T rDmxMMData;
    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;

    UCHAR *pucBuf = NULL;

    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
    u4DmxAvailSize = DMX_MM_GetBufEmptySize(u1HdrType);
  
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

    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
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

            VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);
            LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
            if (u2CurQNum > (_rSwdmxAviInfo.u4VidFps/2))
            {
                x_thread_delay(1);
                return eSWDMX_HDR_PRS_SUCCEED;
            }

            if (_rSwdmxAviInfo.u4VidRepeatFrame != 0)
            {
                if (_rSwdmxAviInfo.fgRequestData)
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

                    u4Size = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size + AVI_PARSER_OFFSET + u1ReadDrmSize;
                    LOG(5, "Offset = %lld Request size = 0x%08x.\n", u8VidOffset,u4Size);
                    
                    if ((u8VidOffset + u4Size) > prRangeList->prDmxRangeInfo->z_range_sz)
                    {
                        _rSwdmxAviInfo.u4CurRepeatFrameIdx = *pu4CurIdx;
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
                        _rSwdmxAviInfo.fgRequestData = TRUE;
                        return eSWDMX_HDR_PRS_FAIL;
                    }

                    if (!_rSwdmxAviInfo.fgReqDataMiss)
                    {
                        if (FeederSetRequest(FEEDER_PROGRAM_SOURCE, 
                            u4Size, 
                            u8VidOffset,
                            &rFeederCond,
                            _rSwdmxAviInfo.u4FeederReqID++) != FEEDER_E_BUFFER_FULL)
                        {
                            _rSwdmxAviInfo.i4ReqReadTimes++;

                            if (!_SWDMX_AviGetDataFromFeeder(&rFeederReqData))
                            {
                                //_rSwdmxAviInfo.fgFeederInvalid = TRUE;
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
                        if (!_SWDMX_AviGetDataFromFeeder(&rFeederReqData))
                        {
                            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                        }
                    }

                    _rSwdmxAviInfo.i4ReqReadTimes--;

                    if (rFeederReqData.u4ReadSize > rFeederReqData.u4AlignOffset)
                    {
                        u4Size = rFeederReqData.u4ReadSize - rFeederReqData.u4AlignOffset;
                    }
                    else
                    {
                        return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                    }

                    LOG(5, "rFeederReqData.u4ReadSize = 0x%08x.\n", u4Size);

                    _rSwdmxAviInfo.u4RecFeederSize = u4Size;
                    _rSwdmxAviInfo.u8VidOffset = u8VidOffset + u4Size;

                    pucBuf = (UCHAR*)(VIRTUAL(rFeederReqData.u4WriteAddr + rFeederReqData.u4AlignOffset));
                    _rSwdmxAviInfo.u4RecFeederWPtr = (UINT32)pucBuf;

                    // we don't have to request for the same data for frame repeating.
                    _rSwdmxAviInfo.fgRequestData = FALSE;
                }
                else
                {
                    pucBuf = (UCHAR*)_rSwdmxAviInfo.u4RecFeederWPtr;
                    u4Size = _rSwdmxAviInfo.u4RecFeederSize;
                }

                if (u1ReadDrmSize)
                {
                    if (_SWDMX_PrsAviChunk(pucBuf, 
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
                    _rSwdmxAviInfo.u4RecFeederSize -= u4ConsumeSize;
                }

                pucBuf = (UCHAR*)VIRTUAL(pucBuf + u4ConsumeSize);
                u4Size = _rSwdmxAviInfo.u4RecFeederSize;

                u4Ret = _SWDMX_PrsAviChunk(pucBuf, 
                                prRangeList, 
                                u4Size, 
                                &u4ConsumeSize, 
                                i4PlaySpeed, 
                                fgRangeEnd);

                // Prevent from empty video chunk
                if ((u4ConsumeSize == AVI_PARSER_OFFSET) &&
                    (u4Size > u4ConsumeSize) &&
                    (!_rSwdmxAviInfo.fgIsBlock))
                {
                    pucBuf = (UCHAR*)VIRTUAL(pucBuf + u4ConsumeSize);
                    u4Size -= AVI_PARSER_OFFSET;

                    u4Ret = _SWDMX_PrsAviChunk(pucBuf, 
                                prRangeList, 
                                u4Size, 
                                &u4ConsumeSize, 
                                i4PlaySpeed, 
                                fgRangeEnd);
                }

                if (u4Ret == eSWDMX_HDR_FED_BUF_NOT_ENOUGH)
                {
                    UINT32 u4RequestSize = 0;

                    u8VidOffset = _rSwdmxAviInfo.u8VidOffset;

                    LOG(5, "Block Size = 0x%08x\n", _rSwdmxAviInfo.u4BlockChunkSize);

                    if (_rSwdmxAviInfo.fgIsBlock)
                    {
                        u4RequestSize = _rSwdmxAviInfo.u4BlockChunkSize - 
                            (_rSwdmxAviInfo.u4RecFeederSize - u4ConsumeSize);
                    }
                    else
                    {
                        LOG(3, "_SWDMX_DeliverAviDataUseIdx Parse Video Chunk Fail\n");
                        goto _Exit;
                    }

                    if (!_rSwdmxAviInfo.fgReqDataMiss)
                    {
                        if (FeederSetRequest(FEEDER_PROGRAM_SOURCE, 
                            u4RequestSize, 
                            u8VidOffset,
                            &rFeederCond,
                            _rSwdmxAviInfo.u4FeederReqID++) != FEEDER_E_BUFFER_FULL)
                        {
                            //LOG(5, "Feeder ID = %d\n", _rSwdmxAviInfo.u4FeederReqID);

                            _rSwdmxAviInfo.i4ReqReadTimes++;
                            if (!_SWDMX_AviGetDataFromFeeder(&rFeederReqData))
                            {
                                //_rSwdmxAviInfo.fgFeederInvalid = TRUE;
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
                        if (!_SWDMX_AviGetDataFromFeeder(&rFeederReqData))
                        {
                            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                        }
                    }

                    _rSwdmxAviInfo.i4ReqReadTimes--;

                    if (rFeederReqData.u4AlignOffset != 0)
                    {
                        LOG(1, "Feeder's alignoffset != 0, it's not normal\n");
                    }

                    _rSwdmxAviInfo.u4RecFeederSize += rFeederReqData.u4ReadSize;

                    UNUSED(_SWDMX_PrsAviChunk(pucBuf, 
                                prRangeList, 
                                _rSwdmxAviInfo.u4RecFeederSize, 
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

            if (!_SWDMX_AviCalculateRepeatFrame(i4PlaySpeed))
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
                _rSwdmxAviInfo.fgRequestData = TRUE;
                return eSWDMX_HDR_PRS_SUCCEED;
            }
        }

        //update feeder RPTR
        _rSwdmxAviInfo.u4CurDmxFeederRPtr = (UINT32)pucBuf + u4ConsumeSize;
        if(_rSwdmxAviInfo.u4CurDmxFeederRPtr > VIRTUAL(_rFeederInfo.u4EndAddr))
        {
            _rSwdmxAviInfo.u4CurDmxFeederRPtr -= (_rFeederInfo.u4EndAddr - _rFeederInfo.u4StartAddr);
        }
        FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(_rSwdmxAviInfo.u4CurDmxFeederRPtr));
    }
    return eSWDMX_HDR_PRS_SUCCEED;

_Exit:
    //update feeder RPTR
    _rSwdmxAviInfo.u4CurDmxFeederRPtr = (UINT32)pucBuf + u4ConsumeSize;
    if(_rSwdmxAviInfo.u4CurDmxFeederRPtr > VIRTUAL(_rFeederInfo.u4EndAddr))
    {
        _rSwdmxAviInfo.u4CurDmxFeederRPtr -= (_rFeederInfo.u4EndAddr - _rFeederInfo.u4StartAddr);
    }
    FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, PHYSICAL(_rSwdmxAviInfo.u4CurDmxFeederRPtr));

    if (i4PlaySpeed > 2000)
    {
        (*pu4CurIdx)++;
    }
    else if ((i4PlaySpeed < 0) &&
        (*pu4CurIdx > 0))
    {
        (*pu4CurIdx)--;
    }
    _rSwdmxAviInfo.fgRequestData = TRUE;

    return eSWDMX_HDR_PRS_FAIL;

}

static SWDMX_HDR_PRS_STATE_T _SWDMX_AviHdrPrsWithoutIdx(
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange)
{
    BOOL fgRangeEnd = FALSE;

    UINT32 u4ReadSize = 0;
    UINT32 u4ConsumedSize = 0;
    UINT32 u4PrsState = eSWDMX_HDR_PRS_SUCCEED;

    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;

    if (_rSwdmxAviInfo.fgFeederInvalid)
    {
        INT32 i;
        LOG(3, "Feeder is invalid, Cur Feeder ID = %ld, req times = %ld, Cur Dmx Pos = %lld\n",
            _rSwdmxAviInfo.u4FeederReqID,
            _rSwdmxAviInfo.i4ReqReadTimes,
            _rSwdmxAviInfo.u8CurDmxMoviPos);

        for (i = 0; i < _rSwdmxAviInfo.i4ReqReadTimes; i++)
        {
            _SWDMX_AviGetDataFromFeeder(&rFeederReqData);
        }
        
        _SWDMX_AviResetFeeder();

        _rSwdmxAviInfo.fgIsSeek = TRUE;
        _rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
        _rSwdmxAviInfo.u8CurMoviPos = _rSwdmxAviInfo.u8CurDmxMoviPos;
        _rSwdmxAviInfo.fgFeederInvalid = FALSE;
        _rSwdmxAviInfo.fgReqDataMiss = FALSE;
        _rSwdmxAviInfo.i4ReqReadTimes = 0;
        _rSwdmxAviInfo.u4FeederReqID = 0;
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    // Check if demuxing is done
    if (_rSwdmxAviInfo.u8CurDmxMoviPos >= 
        (prRangeList->prDmxRangeInfo->z_range_sz + prRangeList->prDmxRangeInfo->ui8_fileoffset)
#ifdef __MODEL_slt__
        || _rSwdmxAviInfo.u4NumSentVid == 10
#endif
        )
    {
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        if (_rSwdmxAviInfo.fgEnVideo && !_rSwdmxAviInfo.fgDeliverVEos)
        {
            if (_rSwdmxAviInfo.fgVDecIsPause)
            {
                _SWDMX_AviVideoWakeUp();
                _rSwdmxAviInfo.fgVDecIsPause = FALSE;
            }

            LOG(5, "Cur DmxMoviPos = %ld, range size = %ld\n", 
                _rSwdmxAviInfo.u8CurDmxMoviPos,
                prRangeList->prDmxRangeInfo->z_range_sz);
            
            LOG(5, "_SWDMX_AviHdrPrsWithoutIdx Deliver Video Eos\n");
            if (!_SWDMX_SendAviEos(eSWDMX_MEDIA_VIDEO))
            {
                LOG(3, "Fail to send Video Eos\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            _rSwdmxAviInfo.fgDeliverVEos = TRUE;
        }

        if (_rSwdmxAviInfo.fgEnAudio && !_rSwdmxAviInfo.fgDeliverAEos)
        {
            LOG(5, "_SWDMX_AviHdrPrsWithoutIdx Deliver Audio Eos\n");
            if (!_SWDMX_SendAviEos(eSWDMX_MEDIA_AUDIO))
            {
                LOG(3, "Fail to send Audio Eos\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            _rSwdmxAviInfo.fgDeliverAEos = TRUE;
            AUD_MM_Set_Avsync_event(AUD_DEC_MAIN);
        }

        return eSWDMX_HDR_PRS_SUCCEED;
    }

    // Request data for the first time
    if (_rSwdmxAviInfo.u8CurMoviPos == 0)
    {
        if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
          FEEDER_E_OK)
        {
            LOG(7, "Feeder input buffer error.\n");
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

        if (prRangeList->prDmxRangeInfo->z_range_sz > _rSwdmxAviInfo.u8CurMoviPos)
        {
            u4ReadSize = prRangeList->prDmxRangeInfo->z_range_sz - 
                                   _rSwdmxAviInfo.u8CurMoviPos;
        }
        u4ReadSize = min(u4ReadSize, MIN_READ_SIZE);
        _u4LastAlignOffset = 0;

        _rSwdmxAviInfo.u8CurMoviPos = prRangeList->prDmxRangeInfo->ui8_fileoffset;
        _rSwdmxAviInfo.u8CurDmxMoviPos = _rSwdmxAviInfo.u8CurMoviPos;
        _rSwdmxAviInfo.u4CurDmxFeederRPtr = VIRTUAL(_rFeederInfo.u4StartAddr);

        if (!_rSwdmxAviInfo.fgReqDataMiss)
        {
            if (FeederSetRequest(FEEDER_PROGRAM_SOURCE, 
                    u4ReadSize, /*MIN_READ_SIZE, */
                    _rSwdmxAviInfo.u8CurMoviPos,
                    &rFeederCond,
                    _rSwdmxAviInfo.u4FeederReqID++) != FEEDER_E_BUFFER_FULL)
            {
                //LOG(5, "Feeder ID = %d\n", _rSwdmxAviInfo.u4FeederReqID);
                 _rSwdmxAviInfo.i4ReqReadTimes ++;
                if (!_SWDMX_AviGetDataFromFeeder(&rFeederReqData))
                {
                     //_rSwdmxAviInfo.fgFeederInvalid = TRUE;

                    return eSWDMX_HDR_PRS_FAIL;
                }
                _rSwdmxAviInfo.i4ReqReadTimes --;
                _u4LastAlignOffset = rFeederReqData.u4AlignOffset;
                _rSwdmxAviInfo.u8CurMoviPos += rFeederReqData.u4ReadSize - _u4LastAlignOffset;
                _rSwdmxAviInfo.u4CurDmxFeederRPtr += _u4LastAlignOffset;
                 _rSwdmxAviInfo.u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
                LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
            }
        }
        else
        {
            LOG(5, "wait for missed data request\n");
            if (!_SWDMX_AviGetDataFromFeeder(&rFeederReqData))
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }

            _u4LastAlignOffset = rFeederReqData.u4AlignOffset;

            if ((rFeederReqData.u4ReadSize > _u4LastAlignOffset) &&
                (rFeederReqData.u4ReadSize != 0))
            {
                _rSwdmxAviInfo.u8CurMoviPos += rFeederReqData.u4ReadSize-_u4LastAlignOffset;
            }
            else
            {
                LOG(3, "Feeder read size is smaller than align offset!!!\n");
                _rSwdmxAviInfo.u8CurDmxMoviPos = 
                    prRangeList->prDmxRangeInfo->z_range_sz + prRangeList->prDmxRangeInfo->ui8_fileoffset;
                _rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
                return eSWDMX_HDR_PRS_FAIL;
            }
                    
            _rSwdmxAviInfo.u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
            _rSwdmxAviInfo.i4ReqReadTimes--;

            if(_rSwdmxAviInfo.fgIsSeek)
            {
                _rSwdmxAviInfo.u4CurDmxFeederRPtr = VIRTUAL(rFeederReqData.u4WriteAddr + _u4LastAlignOffset);
                _rSwdmxAviInfo.fgIsSeek = FALSE;
            }
        }
    }

    if ((_rSwdmxAviInfo.u4LeftBytesInFeeder <=  AVI_BUFFER_SIZE) ||
        (_rSwdmxAviInfo.fgIsBlock == TRUE))
    {
        BOOL fgRing = FALSE;
        BOOL fgReadData = TRUE;
        UINT32 u4WriteAddr = NULL;

        if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
          FEEDER_E_OK)
        {
            LOG(7, "Feeder input buffer error.\n");
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

        if (prRangeList->prDmxRangeInfo->z_range_sz > 
            (_rSwdmxAviInfo.u8CurMoviPos - prRangeList->prDmxRangeInfo->ui8_fileoffset))
        {
            u4ReadSize = prRangeList->prDmxRangeInfo->z_range_sz - 
                                   (_rSwdmxAviInfo.u8CurMoviPos - prRangeList->prDmxRangeInfo->ui8_fileoffset);
        }
        u4ReadSize = min(u4ReadSize, MIN_READ_SIZE);

        if (u4ReadSize == 0)
        {
            fgRangeEnd = TRUE;
        }

        u4WriteAddr = VIRTUAL(_rFeederInfo.u4WriteAddr + u4ReadSize);
        if (u4WriteAddr >= VIRTUAL(_rFeederInfo.u4EndAddr))
        {
            fgRing = TRUE;
            u4WriteAddr =  VIRTUAL(_rFeederInfo.u4StartAddr) + 
                (u4WriteAddr -VIRTUAL(_rFeederInfo.u4EndAddr));
        }

        if (_rSwdmxAviInfo.fgQAud)
        {
            if (_rFeederInfo.u4WriteAddr <= _rSwdmxAviInfo.u4QAudAddr)
            {
                fgReadData = (u4WriteAddr >= _rSwdmxAviInfo.u4QAudAddr) ? (FALSE) : (TRUE);
            }
            else
            {
                if (fgRing)
                {
                    fgReadData = (u4WriteAddr >= _rSwdmxAviInfo.u4QAudAddr) ? (FALSE) : (TRUE);
                }
            }
        }
                
        if (fgReadData)
        {
#ifdef CC_DLNA_SUPPORT
            // DLNA
            if ((!_rSwdmxAviInfo.fgFeederIsReady) ||
                (_rSwdmxAviInfo.u4LeftBytesInFeeder + u4ReadSize < AVI_BUFFER_SIZE))
#else
            if (_rSwdmxAviInfo.u4LeftBytesInFeeder + u4ReadSize < AVI_BUFFER_SIZE)
#endif
            {
                if (u4ReadSize > 0)
                {
                    LOG(6, "Left bytes = %ld offset = %lld id = %ld.\n", 
                            _rSwdmxAviInfo.u4LeftBytesInFeeder,
                            _rSwdmxAviInfo.u8CurMoviPos,
                            _rSwdmxAviInfo.u4FeederReqID);

                    if (!_rSwdmxAviInfo.fgReqDataMiss)
                    {
                        if (FeederSetRequest(FEEDER_PROGRAM_SOURCE, 
                                u4ReadSize, 
                                _rSwdmxAviInfo.u8CurMoviPos,
                                &rFeederCond,
                                _rSwdmxAviInfo.u4FeederReqID++) != FEEDER_E_BUFFER_FULL)
                        {
                            //LOG(5, "Feeder ID = %d\n", _rSwdmxAviInfo.u4FeederReqID);

                            _u4LastAlignOffset = 0;
                            _rSwdmxAviInfo.i4ReqReadTimes++;
                        }
                        else
                        {
                            LOG(3, "Feeder Buffer Full\n");
                            _rSwdmxAviInfo.fgFeederInvalid = TRUE;
                            if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
                                FEEDER_E_OK)
                            {
                                LOG(1, "Feeder input buffer error.\n");
                                return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                            }
                            LOG(3, "Cur Fed's write ptr = 0x%08x\n", _rFeederInfo.u4WriteAddr);
                            //return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                        }
                    }
                    else
                    {
                        LOG(5, "wait for missed data request\n");
                        if (!_SWDMX_AviGetDataFromFeeder(&rFeederReqData))
                        {
                            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                        }

                        _u4LastAlignOffset = rFeederReqData.u4AlignOffset;

                        if ((rFeederReqData.u4ReadSize > _u4LastAlignOffset) &&
                            (rFeederReqData.u4ReadSize != 0))
                        {
                            _rSwdmxAviInfo.u8CurMoviPos += rFeederReqData.u4ReadSize-_u4LastAlignOffset;
                        }
                        else
                        {
                            LOG(3, "Feeder read size is smaller than align offset!!!\n");
                            _rSwdmxAviInfo.u8CurDmxMoviPos = 
                                prRangeList->prDmxRangeInfo->z_range_sz+prRangeList->prDmxRangeInfo->ui8_fileoffset;
                            _rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
                            return eSWDMX_HDR_PRS_FAIL;
                        }
                                
                        _rSwdmxAviInfo.u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
                        _rSwdmxAviInfo.i4ReqReadTimes--;

                        if(_rSwdmxAviInfo.fgIsSeek)
                        {
                            _rSwdmxAviInfo.u4CurDmxFeederRPtr = VIRTUAL(rFeederReqData.u4WriteAddr + _u4LastAlignOffset);
                            _rSwdmxAviInfo.fgIsSeek = FALSE;
                        }
                    }
                }
            }
        }
    }

#ifdef CC_DLNA_SUPPORT
    if (u4ReadSize != MIN_READ_SIZE)
    {
        // DLNA
        FeederSetInfo(FEEDER_SWDMX_EOS, TRUE);
    }

    // DLNA
    if (!_rSwdmxAviInfo.fgFeederIsReady && FeederIsReady(FEEDER_PROGRAM_SOURCE))
    /*if ((!_rSwdmxAviInfo.fgFeederIsReady) && 
        ((_rSwdmxAviInfo.u4LeftBytesInFeeder >= AVI_PREBUFFER_SIZE) || (_rSwdmxAviInfo.u4LeftBytesInFeeder >= prRangeList->prDmxRangeInfo->z_range_sz)))*/
    {
        _rSwdmxAviInfo.fgFeederIsReady = TRUE;
    }
#endif

#ifdef SWDMX_TIME_PROFILE
    HAL_TIME_T rTimeRstS, rTimeRstE, rTimeRstDt; 
    HAL_GetTime(&rTimeRstS);
#endif

#ifdef CC_DLNA_SUPPORT
    if ((_rSwdmxAviInfo.u4LeftBytesInFeeder > 0) && _rSwdmxAviInfo.fgFeederIsReady)
#else
    if (_rSwdmxAviInfo.u4LeftBytesInFeeder)
#endif
    {
        if (_rSwdmxAviInfo.fgSpeedChange && i4PlaySpeed == 1000)
        {
            if (_rSwdmxAviInfo.fgEnAudio && _rSwdmxAviInfo.fgEnVideo)
            {
                if(((UINT32)(_rSwdmxAviInfo.u8VSeekPts>>32) != 0xFFFFFFFF)
                    || ((UINT32)(_rSwdmxAviInfo.u8VSeekPts) != 0xFFFFFFFF))
                {
                    if (prRangeList->prDmxRangeInfo->ui8_vid_duration > 
                        (UINT32)_rSwdmxAviInfo.u8VSeekPts)
                    {
                        LOG(5, "Pause Video for A/V Sync\n");
                        VDP_SetPauseMm(0, TRUE);
                        VDEC_Pause(ES0);
                        _rSwdmxAviInfo.fgVDecIsPause = TRUE;
                    }
                }
#if 0
                else
                {
                    LOG(5, "Pause Video for A/V Sync\n");
                    VDP_SetPauseMm(0, TRUE);
                    VDEC_Pause(ES0);
                    _rSwdmxAviInfo.fgVDecIsPause = TRUE;
                }
#endif
            }
            _rSwdmxAviInfo.fgSpeedChange = FALSE;
        }
    
        // Start to parse data
        u4PrsState = _SWDMX_PrsAviChunk((UCHAR*)(_rSwdmxAviInfo.u4CurDmxFeederRPtr),
            prRangeList,
            _rSwdmxAviInfo.u4LeftBytesInFeeder,
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
            if ((_rSwdmxAviInfo.u8CurDmxMoviPos >=
                (prRangeList->prDmxRangeInfo->z_range_sz+prRangeList->prDmxRangeInfo->ui8_fileoffset)) ||
                (_rSwdmxAviInfo.u8CurDmxMoviPos > prRangeList->prDmxRangeInfo->ui8_fileoffset + ONE_MB))
            {
                LOG(3, "No avi chunks was discovered\n");
                _rSwdmxAviInfo.u8CurDmxMoviPos = 
                    prRangeList->prDmxRangeInfo->z_range_sz + prRangeList->prDmxRangeInfo->ui8_fileoffset;
                _rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
                return eSWDMX_HDR_PRS_FAIL;
            }
            break;
        default:
            break;
    }

    if (_rSwdmxAviInfo.u4LeftBytesInFeeder >= u4ConsumedSize)
    {
        _rSwdmxAviInfo.u8CurDmxMoviPos += u4ConsumedSize;
        _rSwdmxAviInfo.u4LeftBytesInFeeder -= u4ConsumedSize;
    }
    else
    {
        _rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
    }

    if (u4ConsumedSize != 0)
    {
        LOG(6, "u4ConsumedSize = %ld u4LeftBytesInFeeder = 0x%08x.\n", 
            u4ConsumedSize, _rSwdmxAviInfo.u4LeftBytesInFeeder);
    }

    if ((_rSwdmxAviInfo.u4CurDmxFeederRPtr + u4ConsumedSize) >= 
        VIRTUAL(_rFeederInfo.u4EndAddr))
    {
        _rSwdmxAviInfo.u4CurDmxFeederRPtr =  VIRTUAL(_rFeederInfo.u4StartAddr) + 
            ((_rSwdmxAviInfo.u4CurDmxFeederRPtr + u4ConsumedSize) -VIRTUAL(_rFeederInfo.u4EndAddr));
    }
    else
    {
        _rSwdmxAviInfo.u4CurDmxFeederRPtr += u4ConsumedSize;
    }
    
    // Update read ptr to feeder
    FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(_rSwdmxAviInfo.u4CurDmxFeederRPtr));

    LOG(9, "_rSwdmxAviInfo.u4CurDmxMoviPos = %ld.\n", _rSwdmxAviInfo.u8CurDmxMoviPos);

    if (_rSwdmxAviInfo.i4ReqReadTimes > 0)
    {
        if (!_SWDMX_AviGetDataFromFeeder(&rFeederReqData))
        {
            //_rSwdmxAviInfo.fgFeederInvalid = TRUE;
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

        LOG(6, "rFeederReqData.u4WriteAddr = 0x%08x. ReadSize = 0x%08x id = %ld\n", 
            rFeederReqData.u4WriteAddr,
            rFeederReqData.u4ReadSize,
            rFeederReqData.u4Id);

        _u4LastAlignOffset = rFeederReqData.u4AlignOffset;

        if ((rFeederReqData.u4ReadSize > _u4LastAlignOffset) &&
            (rFeederReqData.u4ReadSize != 0))
        {
            _rSwdmxAviInfo.u8CurMoviPos += rFeederReqData.u4ReadSize-_u4LastAlignOffset;
        }
        else
        {
            LOG(3, "Feeder read size is smaller than align offset!!!\n");
            _rSwdmxAviInfo.u8CurDmxMoviPos = 
                prRangeList->prDmxRangeInfo->z_range_sz + prRangeList->prDmxRangeInfo->ui8_fileoffset;
            _rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
            return eSWDMX_HDR_PRS_FAIL;
        }
                
        _rSwdmxAviInfo.u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
        _rSwdmxAviInfo.i4ReqReadTimes--;

        if(_rSwdmxAviInfo.fgIsSeek)
        {
            _rSwdmxAviInfo.u4CurDmxFeederRPtr = VIRTUAL(rFeederReqData.u4WriteAddr + _u4LastAlignOffset);
            _rSwdmxAviInfo.fgIsSeek = FALSE;
        }
        LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
    }

    return eSWDMX_HDR_PRS_SUCCEED;
}


static SWDMX_HDR_PRS_STATE_T _SWDMX_PrsAviChunk(
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

    UINT32 i = 0, j = 0;
    UINT32 u4RingSize = 0, u4RingStart = 0, u4OrgRingSize = 0;
    //UINT32 u4TotalConsumeSize = 0;
    UINT32 u4ChunkSize, u4DmxAvailSize;
    UINT32 u4Pts = 0;
    UINT32 u4OrgAvailSize = u4AvailSize;

    //UINT64 u8Numerator, u8Denominator;
    //UINT64 u8Remainder;

    UCHAR ucRingBuf[AVI_PARSER_OFFSET];
    UCHAR ucTempBuf[AVI_CHUNK_ID_OFFSET];
    UCHAR* pucTemp;
    UCHAR* pucOriginalPtr = pucBitstream;

    DMX_MM_DATA_T rDmxMMData;

    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if (u4AvailSize < AVI_PARSER_OFFSET)
    {
        *pu4ConsumeSize = 0;
    }

    while (i < u4AvailSize)
    {
        if (((pucBitstream + i) >= (UCHAR*)(VIRTUAL(_rFeederInfo.u4EndAddr - AVI_PARSER_OFFSET))) &&
            ((u4AvailSize - i) >= AVI_PARSER_OFFSET) && (!fgRingBuf))
        {
            fgRingBuf = TRUE;
            u4RingSize = (UINT32)(VIRTUAL(_rFeederInfo.u4EndAddr)) - (UINT32)(pucBitstream + i);
            u4OrgRingSize = u4RingSize;

            LOG(5, "Ring Buffer start from 0x%08x to 0x%08x size = 0x%08x\n", pucBitstream, 
                VIRTUAL(_rFeederInfo.u4EndAddr), u4RingSize);

            x_memcpy(ucRingBuf, (pucBitstream + i), u4RingSize);

            u4AvailSize -= (u4RingSize + i);
            pucBitstream = (UCHAR*)(VIRTUAL(_rFeederInfo.u4StartAddr));
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
            
            if (_CheckAviChunkType(ucTempBuf, &u1HdrType, &u1TrackIdx) == TRUE)
            {
                fgFindFcc = TRUE;
                fgFindInRing = TRUE;
                break;
            }
            u4RingStart++;
            u4RingSize--;
            continue;
        }

        if (_CheckAviChunkType(pucBitstream + i, &u1HdrType, &u1TrackIdx) == TRUE)
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
    
    if (_rSwdmxAviInfo.u8RecDmxMoviPos)
    {
        LOG(7, "CurDmxMoviPos when mode Change = %ld\n", 
            _rSwdmxAviInfo.u8CurDmxMoviPos);
        if (_rSwdmxAviInfo.u8RecDmxMoviPos > _rSwdmxAviInfo.u8CurDmxMoviPos)
        {
            // Find the first audio chunk since 2X -> 1X
            if (u1HdrType == eSWDMX_MEDIA_AUDIO)
            {
                if (_rSwdmxAviInfo.u4ChangeMode & AVI_CHANGE_AUDIO_MODE)
                {
                    UINT8 u1ATrack = _rSwdmxAviInfo.u1CurATrackIdx;
                    UINT32 u4ModeChangePts = 0;
                    if(_rSwdmxAviInfo.fgEnVideo)
                    {
                        u4ModeChangePts = VDP_GetPts(0);
                    }
                    else
                    {
                        u4ModeChangePts = STC_GetStcValue();
                    }
                    //_rSwdmxAviInfo.u4ModeChangPts = VDP_GetPts(0);
                    
                    if (u1ATrack < AVI_DIVX_MAX_AUD_TRACK)
                    {
                        //if (_rSwdmxAviInfo.u4AudPts[u1ATrack] >= (_rSwdmxAviInfo.u4ModeChangPts + 27000))
                        if (_rSwdmxAviInfo.u4AudPts[u1ATrack] >= (u4ModeChangePts + 18000))
                        {
                            LOG(5, "Audio Pts (0x%08x) > Mode ChangePts (0x%08x)\n", 
                                _rSwdmxAviInfo.u4AudPts[u1ATrack], 
                                u4ModeChangePts);

                            //_rSwdmxAviInfo.u4ModeChangPts = 0xFFFFFFFF;
                            _rSwdmxAviInfo.u4ChangeMode = 0;
                            _rSwdmxAviInfo.u4TriggerAudPts = _rSwdmxAviInfo.u4AudPts[u1ATrack];

                            AUD_SetStartPts(0, _rSwdmxAviInfo.u4AudPts[u1ATrack]);
                            _rSwdmxAviInfo.u4AudStartPts = _rSwdmxAviInfo.u4AudPts[u1ATrack];
                        }
                        else
                        {
                            LOG(5, "Audio Pts (0x%08x) < Mode ChangePts (0x%08x)\n", 
                                _rSwdmxAviInfo.u4AudPts[u1ATrack], 
                                u4ModeChangePts);

                            _SWDMX_AviCalculatePts(u1HdrType, u1ATrack, u4ChunkSize);
                            u1HdrType = eSWDMX_MEDIA_SKIPPED;
                        }
                    }
                }
                if (_rSwdmxAviInfo.u4ChangeMode & AVI_CHANGE_SUBTITLE_MODE)
                {
                     UINT8 u1ATrack = _rSwdmxAviInfo.u1CurATrackIdx;
                     _SWDMX_AviCalculatePts(u1HdrType, u1ATrack, u4ChunkSize);
                            u1HdrType = eSWDMX_MEDIA_SKIPPED;
                }
            }
            else if (u1HdrType == eSWDMX_MEDIA_SUBTITLE)
            {
                UINT32 u4StartPts = 0, u4EndPts = 0, u4CurPts = 0;

                u4CurPts = VDP_GetPts(0);
                _SWDMX_AviPrsSubtitle((UCHAR*)rDmxMMData.u4StartAddr, &u4StartPts, &u4EndPts);
                LOG(5, "SubTitle start pts = 0x%08x end pts = 0x%08x\n", u4StartPts, u4EndPts);
                if (u4EndPts < (u4CurPts))
                {
                    LOG(5, "Skip SubTitle Mode change PTS = 0x%08x\n", 
                        u4CurPts);
                    u1HdrType = eSWDMX_MEDIA_SKIPPED;
                }  
            }
            else if (u1HdrType == eSWDMX_MEDIA_VIDEO)
            {
                 _SWDMX_AviCalculatePts(u1HdrType, u1TrackIdx, u4ChunkSize);
                 //Skip other chunks
                 u1HdrType = eSWDMX_MEDIA_SKIPPED;
            }
            else
            {
                if ((u1HdrType == eSWDMX_MEDIA_DRM_VIDEO) ||
                	(u1HdrType == eSWDMX_MEDIA_DRM_AUDIO))
                {
                    _SWDMX_AviPrsDrmChunk(u1HdrType, rDmxMMData.u4StartAddr);
                }

                _SWDMX_AviCalculatePts(u1HdrType, u1TrackIdx, u4ChunkSize);

                // Skip other chunks
                u1HdrType = eSWDMX_MEDIA_SKIPPED;
            }
        }
        else
        {   
            LOG(5, "_rSwdmxAviInfo.u8RecDmxMoviPos == _rSwdmxAviInfo.u8CurDmxMoviPos\n");
            _rSwdmxAviInfo.u8RecDmxMoviPos = 0;

            if(_rSwdmxAviInfo.u4ChangeMode&AVI_CHANGE_AUDIO_MODE)
            {
                 if(_rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)
                 {
                     AUD_SetStartPts(0, _rSwdmxAviInfo.u4AudPts[_rSwdmxAviInfo.u1CurATrackIdx]);
                 }
            }
            _rSwdmxAviInfo.u4ChangeMode = 0;
        }
    }

    // Error Handling
    if (u4ChunkSize > AVI_BUFFER_SIZE)
    {
        LOG(3, "u4ChunkSize > AVI_BUFFER_SIZE Chunk size = 0x%08x\n", u4ChunkSize);
        if((i + _rSwdmxAviInfo.u4ErrorData)> MIN_READ_SIZE*2)
        {
            return eSWDMX_HDR_PRS_FAIL;
        }
        else
        {
            *pu4ConsumeSize = i + AVI_PARSER_OFFSET;
             _rSwdmxAviInfo.u4ErrorData = i;
            return eSWDMX_HDR_PRS_SUCCEED;
        }
    } 
    else if ((u4ChunkSize + AVI_PARSER_OFFSET + i) > u4AvailSize)
    {
        LOG(3, "Chunk size is 0x%08x larger than feeder's, the size is 0x%08x\n", u4ChunkSize, u4AvailSize);

        if (!fgRangeEnd)
        {
            _rSwdmxAviInfo.fgIsBlock = TRUE;
            _rSwdmxAviInfo.u4BlockChunkSize = u4ChunkSize + AVI_PARSER_OFFSET;
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
             _SWDMX_AviCalculatePts(u1HdrType, u1TrackIdx, u4ChunkSize);
             _rSwdmxAviInfo.u2SkipVidCount++;
             _rSwdmxAviInfo.fgNoCalPts = FALSE;
        }
        
        *pu4ConsumeSize = AVI_PARSER_OFFSET + i;
        return eSWDMX_HDR_PRS_SUCCEED;
    }

    // Disable streams here
    if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
    	  if((((UINT32)(_rSwdmxAviInfo.u8VSeekPts>>32) != 0xFFFFFFFF) || 
            ((UINT32)(_rSwdmxAviInfo.u8VSeekPts) != 0xFFFFFFFF))&&(_rSwdmxAviInfo.fgMp3Abr))
        {
        	  if(_rSwdmxAviInfo.u4CurVidPts < (UINT32)(_rSwdmxAviInfo.u8VSeekPts))
        	  {
        	  	   u1HdrType = eSWDMX_MEDIA_SKIPPED_AUDIO;
        	  }
        }
        u1TrackIdx = _rSwdmxAviInfo.u1CurATrackIdx;
        if (i4PlaySpeed != SWDMX_PLAY_SPEED_1X)
        {
            _SWDMX_AviCalculatePts(u1HdrType, u1TrackIdx, u4ChunkSize);
#ifdef CC_DLNA_SUPPORT
            if (INPUT_SRC == MM_SRC_TYPE_HIGH_SPEED_STORAGE)
#endif
            {
                u1HdrType = eSWDMX_MEDIA_SKIPPED;
            }
        }
        else if (_rSwdmxAviInfo.u4ChangeMode & AVI_CHANGE_AUDIO_MODE)
        {
            UINT32 u4ModeChangePts = VDP_GetPts(0);
            //_rSwdmxAviInfo.u4ModeChangPts = VDP_GetPts(0);
            
            if (u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK)
            {
                //if (_rSwdmxAviInfo.u4AudPts[u1TrackIdx] >= (_rSwdmxAviInfo.u4ModeChangPts + 27000))
                if (_rSwdmxAviInfo.u4AudPts[u1TrackIdx] >= (u4ModeChangePts + 18000))
                {
                    LOG(5, "Audio Pts (0x%08x) > Mode ChangePts (0x%08x)\n", 
                        _rSwdmxAviInfo.u4AudPts[u1TrackIdx], 
                        u4ModeChangePts);

                    //_rSwdmxAviInfo.u4ModeChangPts = 0xFFFFFFFF;
                    _rSwdmxAviInfo.u4ChangeMode = 0;
                    _rSwdmxAviInfo.u4TriggerAudPts = _rSwdmxAviInfo.u4AudPts[u1TrackIdx];
                    AUD_SetStartPts(0, _rSwdmxAviInfo.u4AudPts[u1TrackIdx]);
                    _rSwdmxAviInfo.u4AudStartPts = _rSwdmxAviInfo.u4AudPts[u1TrackIdx];
                }
                else
                {
                    _SWDMX_AviCalculatePts(u1HdrType, u1TrackIdx, u4ChunkSize);
                    u1HdrType = eSWDMX_MEDIA_SKIPPED;
                }
            }
        }
        else if(((UINT32)(_rSwdmxAviInfo.u8ASeekPts>>32) != 0xFFFFFFFF) || 
            ((UINT32)(_rSwdmxAviInfo.u8ASeekPts) != 0xFFFFFFFF))
        {
            if((u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK) &&
                (_rSwdmxAviInfo.u4AudPts[u1TrackIdx] < (UINT32)_rSwdmxAviInfo.u8ASeekPts))
            {
                _SWDMX_AviCalculatePts(u1HdrType, u1TrackIdx, u4ChunkSize);
                u1HdrType = eSWDMX_MEDIA_SKIPPED;
            }
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO)
    {
        _SWDMX_AviCalculatePts(u1HdrType, u1TrackIdx, u4ChunkSize);
    }
    else if ((u1HdrType == eSWDMX_MEDIA_DRM_VIDEO) ||
    	(u1HdrType == eSWDMX_MEDIA_DRM_AUDIO))
    {
        _SWDMX_AviPrsDrmChunk(u1HdrType, rDmxMMData.u4StartAddr);
        u1HdrType = eSWDMX_MEDIA_SKIPPED;
    }

    if (u1HdrType == eSWDMX_MEDIA_SKIPPED)
    {
        *pu4ConsumeSize = u4ChunkSize + AVI_PARSER_OFFSET + i;
        if (fgRingBuf)
        {
            *pu4ConsumeSize += ((UINT32)(VIRTUAL(_rFeederInfo.u4EndAddr)) - 
                                                               (UINT32)(pucOriginalPtr) - 
                                                               u4RingSize);
        }
        return eSWDMX_HDR_PRS_SUCCEED;
    }

    if (u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO)
    {
#ifdef CC_DLNA_SUPPORT
        if ((INPUT_SRC == MM_SRC_TYPE_HIGH_SPEED_STORAGE) || 
            ((_rSwdmxAviInfo.i4PrevSpeed == 1000)&&
            (_rSwdmxAviInfo.u1CurATrackIdx == u1TrackIdx)))
#endif
        {
            *pu4ConsumeSize = u4ChunkSize + AVI_PARSER_OFFSET + i;
            if (fgRingBuf)
            {
                *pu4ConsumeSize += ((UINT32)(VIRTUAL(_rFeederInfo.u4EndAddr)) - 
                                                                   (UINT32)(pucOriginalPtr) - 
                                                                   u4RingSize);
            }
            return eSWDMX_HDR_PRS_SUCCEED;
        }
    }

    // Get available size here (we don't have to query subtitle available size here)
    if ((u1HdrType == eSWDMX_MEDIA_VIDEO) ||
        (((u1HdrType == eSWDMX_MEDIA_AUDIO) || (u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO)) && 
        (i4PlaySpeed == SWDMX_PLAY_SPEED_1X)))
    {
#ifdef CC_DLNA_SUPPORT
        // DLNA
        if (INPUT_SRC == MM_SRC_TYPE_HIGH_SPEED_STORAGE)
#endif
        {
            u4DmxAvailSize = DMX_MM_GetBufEmptySize(u1HdrType);
        }
#ifdef CC_DLNA_SUPPORT
        else
        {
            if((u1HdrType == eSWDMX_MEDIA_AUDIO) ||
                (u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO))
            {
                u4DmxAvailSize = 
                    //DMX_MM_GetBufEmptySize(AVI_AUD_PREBUF_IDX + u1TrackIdx);
                    DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_AUDIO);
            }
            else
            {
                u4DmxAvailSize = DMX_MM_GetBufEmptySize(u1HdrType);
            }
        }
#endif

        if ((u1HdrType == eSWDMX_MEDIA_AUDIO) &&
            (!_rSwdmxAviInfo.fgQAud) &&
            (u4ChunkSize > u4DmxAvailSize) &&
            (u4ChunkSize > (_rSwdmxAviInfo.u4AFifoSize/2)) &&
            (u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK))
        {
            _SWDMX_AviCalculatePts(u1HdrType, u1TrackIdx, u4ChunkSize);

            _rSwdmxAviInfo.fgQAud = TRUE;
            _rSwdmxAviInfo.u4QAudAddr = rDmxMMData.u4StartAddr;
            _rSwdmxAviInfo.u4QAudSize = u4ChunkSize;
            _rSwdmxAviInfo.u4QAudPts = _rSwdmxAviInfo.u4AudPts[u1TrackIdx];

            *pu4ConsumeSize = u4ChunkSize + AVI_PARSER_OFFSET + i;
            if (fgRingBuf)
            {
                *pu4ConsumeSize += ((UINT32)(VIRTUAL(_rFeederInfo.u4EndAddr)) - 
                                                               (UINT32)(pucOriginalPtr) - 
                                                               u4RingSize);
            }

            _rSwdmxAviInfo.fgIsBlock = FALSE;
            _rSwdmxAviInfo.u4BlockChunkSize = 0;

            return eSWDMX_HDR_PRS_SUCCEED;
        }

        if ((u1HdrType == eSWDMX_MEDIA_AUDIO) &&
            (_rSwdmxAviInfo.fgQAud))
        {
            if (_rSwdmxAviInfo.u4QAudSize < u4DmxAvailSize)
            {
                rDmxMMData.u4BufStart = (UINT32)(VIRTUAL(_rFeederInfo.u4StartAddr));
                rDmxMMData.u4BufEnd = (UINT32)(VIRTUAL(_rFeederInfo.u4EndAddr));
                rDmxMMData.u4StartAddr = _rSwdmxAviInfo.u4QAudAddr;
                rDmxMMData.u4FrameSize = _rSwdmxAviInfo.u4QAudSize;
                rDmxMMData.u4Pts = _rSwdmxAviInfo.u4QAudPts;

#ifdef CC_DLNA_SUPPORT
                    // DLNA
                if (INPUT_SRC != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
                {
                    if ((u1HdrType == eSWDMX_MEDIA_AUDIO) ||
                        (u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO))
                    {
                        u1HdrType = AVI_AUD_PREBUF_IDX + u1TrackIdx;
            
                        if (u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK)
                        {
                            u4Pts = _rSwdmxAviInfo.u4AudPts[u1TrackIdx];
                        }
                        else
                        {
                            u4Pts = 0;
                        }
                    }
                }
#endif

                LOG(5, "Send Q Audio Data Size = 0x%08x pts = 0x%08x\n", u4ChunkSize, rDmxMMData.u4Pts);

                if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
                {
                    LOG(0, "Demuxer fails to move data.\n");
                    *pu4ConsumeSize = 0;
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                }

                _rSwdmxAviInfo.fgIsBlock = TRUE;
                _rSwdmxAviInfo.u4BlockChunkSize = u4ChunkSize + AVI_PARSER_OFFSET;
                *pu4ConsumeSize = 0;

                _rSwdmxAviInfo.fgQAud = FALSE;
                _rSwdmxAviInfo.u4QAudAddr = NULL;
                _rSwdmxAviInfo.u4QAudSize = 0;
                
                return eSWDMX_HDR_PRS_SUCCEED;
            }
            else
            {
                *pu4ConsumeSize = 0;
                x_thread_delay(1);
                return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
            }
        }
                
        if (u4ChunkSize > u4DmxAvailSize)
        {    
            if (_rSwdmxAviInfo.fgVDecIsPause)
            {
                _SWDMX_AviVideoWakeUp();
                _rSwdmxAviInfo.fgVDecIsPause = FALSE;
            }
            
            if (u1HdrType == eSWDMX_MEDIA_AUDIO &&
                _rSwdmxAviInfo.fgAudIsReady)
            {
                AUD_MMAoutEnable(0, TRUE);
                STC_StartStc();
                _rSwdmxAviInfo.fgAudIsReady = FALSE;
            }
            
            LOG(7, "Chunk is blocked u1HdrType = %ld, u4ChunkSize = %ld, u4DmxAvailSize = %ld\n", 
                u1HdrType, 
                u4ChunkSize,
                u4DmxAvailSize);
            *pu4ConsumeSize = 0;
            x_thread_delay(1);
            return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
        }

        _SWDMX_InitDmx(u1HdrType, NULL, FALSE);
    }

    if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        u1TrackIdx = _rSwdmxAviInfo.u1CurATrackIdx;

        if (u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK)
        {
            UCHAR *pBuf = (UCHAR*)(rDmxMMData.u4StartAddr);
            UINT32 u4TimeSeekAudPts = _rSwdmxAviInfo.u4AudPts[u1TrackIdx];

            if (((_rSwdmxAviInfo.u4ADecType == ENUM_SWDMX_ADEC_MP3) || (_rSwdmxAviInfo.u4ADecType == ENUM_SWDMX_ADEC_MPEG)) &&
                (!_rSwdmxAviInfo.fgAudIsLoaded) &&
                (pBuf[0] != 0xFF) &&
                ((pBuf[1] & 0xF0) != 0xF0))
            {
                _rSwdmxAviInfo.fgBadAudio = TRUE;
            }
                
            _SWDMX_AviCalculatePts(u1HdrType, u1TrackIdx, u4ChunkSize);
	    _rSwdmxAviInfo.u4NumSentAud++;
            u4Pts = _rSwdmxAviInfo.u4AudPts[u1TrackIdx];

#ifdef CC_DLNA_SUPPORT
            BOOL fgRet;
            UINT32 u4CurPts, u4APts;
            
            if ((INPUT_SRC != MM_SRC_TYPE_HIGH_SPEED_STORAGE) &&
                (_rSwdmxAviInfo.fgRecSpeedChangeForAud))
            {
                if (_rSwdmxAviInfo.i4RecPrevSpeedForAud == 2000 || 
                    _rSwdmxAviInfo.i4RecPrevSpeedForAud == 1 )
                {
                    u4CurPts = (_rSwdmxAviInfo.fgEnVideo) ? (VDP_GetPts(0)) : (STC_GetStcValue());
                    
                    _rSwdmxAviInfo.u4TriggerAudPts = u4CurPts + 27000;

                    fgRet = DMX_AudHandler_SeekAndUpdate(u1TrackIdx + AVI_AUD_PREBUF_IDX,
                        _rSwdmxAviInfo.u4TriggerAudPts, &u4APts);

                    LOG(5, "Find Audio Track Data for 2X -> 1X\n");

                    if (fgRet)
                    {
                        LOG(5, "Find audio in the pre-buffer\n");
                        _rSwdmxAviInfo.u4TriggerAudPts = u4APts;
                    }
                    else
                    {
                        LOG(5, "Can't find audio in the pre-buffer\n");
                        _rSwdmxAviInfo.u4TriggerAudPts = _rSwdmxAviInfo.u4AudPts[u1TrackIdx];
                    }

                    AUD_SetStartPts(0, _rSwdmxAviInfo.u4TriggerAudPts);
                    LOG(5, "_rSwdmxAviInfo.u4TriggerAudPts = 0x%08x\n", _rSwdmxAviInfo.u4TriggerAudPts);
                }
                else
                {
                    UNUSED(DMX_AudHandler_SeekAndUpdate(u1TrackIdx + AVI_AUD_PREBUF_IDX,
                        0xFFFFFFFF, &u4APts));
                }

                _rSwdmxAviInfo.fgRecSpeedChangeForAud = FALSE;
                _rSwdmxAviInfo.i4RecPrevSpeedForAud = i4PlaySpeed;
                DMX_AudHandler_SetActivePidx(u1TrackIdx + AVI_AUD_PREBUF_IDX);
            }
#endif
            if (((_rSwdmxAviInfo.u4ADecType == ENUM_SWDMX_ADEC_MP3)||
                (_rSwdmxAviInfo.u4ADecType == ENUM_SWDMX_ADEC_MPEG))&&
                (! _rSwdmxAviInfo.fgGetMp3Status)&&!(_rSwdmxAviInfo.fgMp3Abr))
            {
                 UCHAR *puMp3Hdr =  (UCHAR*)rDmxMMData.u4StartAddr;
                 UCHAR uParHdr[4]= {0};
                 UINT32  u4HeaderSize = 0;
                 UINT8 i = 0;
                 
                 _rSwdmxAviInfo.u4AudNotParseSize =_rSwdmxAviInfo.u4AudNotParseSize +u4ChunkSize;

                 if((_rSwdmxAviInfo.u4AudNotParseSize > _rSwdmxAviInfo.u4AudFrameSize)&&
                    (_rSwdmxAviInfo.u4AudFrameSize != 0))
                 {
                     _rSwdmxAviInfo.u4AudNotParseSize -= _rSwdmxAviInfo.u4AudFrameSize;
                     puMp3Hdr = (UCHAR*) ((UINT32)puMp3Hdr + u4ChunkSize -_rSwdmxAviInfo.u4AudNotParseSize);
                     _rSwdmxAviInfo.u4AudFrameSize = 0;
                 }
                 while(_rSwdmxAviInfo.u4AudNotParseSize > _rSwdmxAviInfo.u4AudFrameSize)
                 {
                     _rSwdmxAviInfo.u4AudNotParseSize -= _rSwdmxAviInfo.u4AudFrameSize;
                     
                      puMp3Hdr = (UCHAR*) ((UINT32)puMp3Hdr- u4HeaderSize + _rSwdmxAviInfo.u4AudFrameSize);
                      u4HeaderSize = 0;
                      
                      while(*puMp3Hdr == 0)
                      {
                          puMp3Hdr++;
                          
                           if((UINT32)puMp3Hdr >= VIRTUAL(_rFeederInfo.u4EndAddr))
                          {
                               puMp3Hdr = 
                                 (UCHAR*)((UINT32)puMp3Hdr - VIRTUAL(_rFeederInfo.u4EndAddr) + 
                                VIRTUAL(_rFeederInfo.u4StartAddr));
                          }
                          _rSwdmxAviInfo.u4AudNotParseSize --;
                      }

                     if((UINT32)puMp3Hdr >= VIRTUAL(_rFeederInfo.u4EndAddr))
                      {
                           puMp3Hdr = 
                                (UCHAR*)((UINT32)puMp3Hdr - VIRTUAL(_rFeederInfo.u4EndAddr) + 
                                VIRTUAL(_rFeederInfo.u4StartAddr));
                      }
                      for (i = 0; i<4; i++)
                      {
                      	  uParHdr[i] = *puMp3Hdr;
                      	  puMp3Hdr++;
                          u4HeaderSize ++;
                          
                          if((UINT32)puMp3Hdr >= VIRTUAL(_rFeederInfo.u4EndAddr))
                          {
                               puMp3Hdr = 
                                 (UCHAR*)((UINT32)puMp3Hdr - VIRTUAL(_rFeederInfo.u4EndAddr) + 
                                VIRTUAL(_rFeederInfo.u4StartAddr));
                          }
                      	
                      }
                      
                      if(!(MPEGA_IS_VALID_HDR(uParHdr)))
                      {
                          LOG(3, "illegal audio header \n");
                          break;
                      }

                      _rSwdmxAviInfo.u4AudFrameSize = dwMpA_GetFrmSz(uParHdr, &_rSwdmxAviInfo.u4Mp3Rate);

                      if(_rSwdmxAviInfo.u4AudFrameSize  == 0)
                      {
                           LOG(3, "illegal u4AudFrameSize\n");
                           _rSwdmxAviInfo.fgGetMp3Status = TRUE;
                           break;
                      }
                      
                     if ((_rSwdmxAviInfo.u4Mp3LastRate !=0)&&
                      (_rSwdmxAviInfo.u4Mp3LastRate != _rSwdmxAviInfo.u4Mp3Rate))
                     {
                        _rSwdmxAviInfo.fgMp3Vbr = TRUE;
                     }    
                     _rSwdmxAviInfo.u4Mp3LastRate = _rSwdmxAviInfo.u4Mp3Rate;
                 }

                 if((_rSwdmxAviInfo.fgMp3Vbr) && (_rSwdmxAviInfo.parAudKeyIdxTbl[u1TrackIdx]))
                 {
                      if (!_rSwdmxAviInfo.parAudKeyIdxTbl[u1TrackIdx]->b_is_VBR)
                      {
                           _rSwdmxAviInfo.fgMp3Abr = TRUE;
                      }      
                 }
            }

            if (_rSwdmxAviInfo.u4ADecType == ENUM_SWDMX_ADEC_AAC)
            {
                DMX_MM_DATA_T rTmpDmxMMData;

                UINT8 u1SampleRateIdx = 3;
                if(_pucAviAacBuf == 0)
                {
                    LOG(1, "pucAudBuf should be allocated in init function");
                    _pucAviAacBuf = 
                        (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(SWDMX_AVI_AAC_ALLOC_SIZE,16));                           
                }                               

                if (_pucAviAacBuf)
                {
                    /* 8b: syncword */
                    _pucAviAacBuf[0] = 0xFF;
                    /* 4b: syncword */
                    /* 1b: mpeg id = 1 */
                    /* 2b: layer = 0 */
                    /* 1b: protection absent */
                    _pucAviAacBuf[1] = 0xF9;
                    /* 2b: profile */
                    /* 4b: sampling_frequency_index */
                    /* 1b: private = 0 */
                    /* 1b: channel_configuration */
                    switch (_rSwdmxAviInfo.u4AudSampleRate)
                    {
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

                    _pucAviAacBuf[2] = (1 << 6) |
                             ((u1SampleRateIdx<< 2) & 0x3C) |
                             ((_rSwdmxAviInfo.u4AudChannelNs >> 2) & 0x1);
                    /* 2b: channel_configuration */
                    /* 1b: original */
                    /* 1b: home */
                    /* 1b: copyright_id */
                    /* 1b: copyright_id_start */
                    /* 2b: aac_frame_length */
                    _pucAviAacBuf[3] = ((_rSwdmxAviInfo.u4AudChannelNs & 0x3) << 6) |
                             (((u4ChunkSize + SWDMX_AVI_AAC_FAKE_HDR_SIZE) >> 11) & 0x3);
                    /* 8b: aac_frame_length */
                    _pucAviAacBuf[4] = (((u4ChunkSize + SWDMX_AVI_AAC_FAKE_HDR_SIZE) >> 3) & 0xFF);
                    /* 3b: aac_frame_length */
                    /* 5b: adts_buffer_fullness */
                    _pucAviAacBuf[5] = (((u4ChunkSize + SWDMX_AVI_AAC_FAKE_HDR_SIZE) << 5) & 0xE0) |
                             ((0x7FF >> 6) & 0x1F);
                    /* 6b: adts_buffer_fullness */
                    /* 2b: number_of_raw_data_blocks_in_frame */
                    _pucAviAacBuf[6] = ((0x7FF << 2) & 0xFC);
                    rTmpDmxMMData.u4BufStart = (UINT32)_pucAviAacBuf;
                    rTmpDmxMMData.u4BufEnd = rTmpDmxMMData.u4BufStart + SWDMX_AVI_AAC_ALLOC_SIZE;
                    rTmpDmxMMData.u4StartAddr = rTmpDmxMMData.u4BufStart;
                    rTmpDmxMMData.u4FrameSize = 7;
                    rTmpDmxMMData.u4Pts = u4Pts;
                    
                    if (DMX_MM_MoveData(u1HdrType, &rTmpDmxMMData, 500) != TRUE)
                    {
                        LOG(0, "Demuxer fails to move data.\n");
                        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                    } 
                }
            }

            //enable audio according to pts, need fix by aud index table
            if(((UINT32)(_rSwdmxAviInfo.u8ASeekPts>>32) != 0xFFFFFFFF)
                || ((UINT32)(_rSwdmxAviInfo.u8ASeekPts) != 0xFFFFFFFF))
            {
                if((_rSwdmxAviInfo.u4AudPts[u1TrackIdx] >= (UINT32)_rSwdmxAviInfo.u8ASeekPts))
                {

                    AUD_SetStartPts(0, u4TimeSeekAudPts);
                    _rSwdmxAviInfo.u4AudStartPts = u4TimeSeekAudPts;

                    if (_rSwdmxAviInfo.fgEnVideo)
                    {
                        VDP_SetPauseMm(0, TRUE);
                        _rSwdmxAviInfo.u4TriggerAudPts = (UINT32)u4TimeSeekAudPts;
                
                        VDEC_Pause(ES0);
                        _rSwdmxAviInfo.fgVDecIsPause = TRUE;
                    }
                    
                    LOG(7, "Got Aud T PTS 0x%x, Seek 0x%x\n", 
                        u4Pts, (UINT32)_rSwdmxAviInfo.u8ASeekPts);            
                    _rSwdmxAviInfo.u8ASeekPts = 0xFFFFFFFF;
                    _rSwdmxAviInfo.u8ASeekPts <<= 32;
                    _rSwdmxAviInfo.u8ASeekPts += 0xFFFFFFFF;
                }
                else
                {
                    //skip audio data in seek process
                    *pu4ConsumeSize = u4ChunkSize + AVI_PARSER_OFFSET + i;
                    if (fgRingBuf)
                    {
                        *pu4ConsumeSize += ((UINT32)(VIRTUAL(_rFeederInfo.u4EndAddr)) - 
                                                                   (UINT32)(pucOriginalPtr) - 
                                                                   u4RingSize);
                    }
                    return eSWDMX_HDR_PRS_SUCCEED;
                }
            }
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        UINT16 u2MaxQNum, u2CurQNum;
        
        if (_rSwdmxAviInfo.fgEnVideo == FALSE)
        {
            u1HdrType = eSWDMX_MEDIA_SKIPPED;
        }
        
        VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);
        if (u2CurQNum > (_rSwdmxAviInfo.u4VidFps * 3))
        {
            if (_rSwdmxAviInfo.fgVDecIsPause)
            {
                _SWDMX_AviVideoWakeUp();
                _rSwdmxAviInfo.fgVDecIsPause = FALSE;
            }
            LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
            *pu4ConsumeSize = 0;            
            x_thread_delay(1);
            return eSWDMX_HDR_PRS_SUCCEED;
        }

        u4Pts = _rSwdmxAviInfo.u4CurVidPts;
        _rSwdmxAviInfo.fgNoCalPts = FALSE;
        // Record every video's chunk position.
        _rSwdmxAviInfo.u8VidOffset = _rSwdmxAviInfo.u8CurDmxMoviPos;
        
        if(((UINT32)(_rSwdmxAviInfo.u8VSeekPts>>32) != 0xFFFFFFFF)
            || ((UINT32)(_rSwdmxAviInfo.u8VSeekPts) != 0xFFFFFFFF))
        {
            if(u4Pts > (UINT32)_rSwdmxAviInfo.u8VSeekPts)
            {
                if ((_rSwdmxAviInfo.fgMp3Abr) &&
                    (_rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK))
                {
                    _rSwdmxAviInfo.u4AudPts[_rSwdmxAviInfo.u1CurATrackIdx] = 
                        (UINT32)_rSwdmxAviInfo.u8VSeekPts;
                    _rSwdmxAviInfo.u8ASeekPts = _rSwdmxAviInfo.u8VSeekPts;
                }
                
                LOG(5, "Got Vid T PTS 0x%x, Seek 0x%x\n", 
                    u4Pts, (UINT32)_rSwdmxAviInfo.u8VSeekPts);            
                _rSwdmxAviInfo.u8VSeekPts = 0xFFFFFFFF;
                _rSwdmxAviInfo.u8VSeekPts <<= 32;
                _rSwdmxAviInfo.u8VSeekPts += 0xFFFFFFFF;
            }
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_SUBTITLE)
    {
        _rSwdmxAviInfo.u4NumSentSub++;
    }

    if (prRangeList->prDmxRangeInfo->b_drm)
    {
        UINT32 u4Flag;
        DMX_MM_DRM_AES_T rDRM = {0};
        DIVX_DRM_CRYPTO_INFO_T *prDrmInfo;

        rDRM.fgCBC = FALSE;
        rDRM.u4KeyBitLen = AVI_DRM_AES_SIZE_128;

        prDrmInfo = 
            prRangeList->prDmxRangeInfo->t_drm_info_drv.u.t_divx_drm_info_drv.pt_divx_drm_crypto_info;
        
        LOG(6, "Drm Type = %ld, DRM key idx = %ld\n",  
            _rSwdmxAviInfo.u1DrmType, _rSwdmxAviInfo.u2FrameKeyIdx);
        
        if ((u1HdrType == eSWDMX_MEDIA_VIDEO) &&
            (_rSwdmxAviInfo.u1DrmType == eSWDMX_MEDIA_DRM_VIDEO) &&
            (_rSwdmxAviInfo.u4EncryptLength != 0) &&
            (_rSwdmxAviInfo.u2FrameKeyIdx < 128))
        {
            rDRM.u4Offset = _rSwdmxAviInfo.u4EncryptOffset;
            rDRM.u4EncryLen = _rSwdmxAviInfo.u4EncryptLength;
            
            x_memcpy(rDRM.au1Key, 
                prDrmInfo->aui1_frame_keys[_rSwdmxAviInfo.u2FrameKeyIdx],
                (AVI_DRM_AES_SIZE_128/8));

            u4Flag = DMX_MM_DRM_FLAG_ALL;
            DMX_MM_SetDRMMode(u1HdrType, DMX_DRM_MODE_AES);
            DMX_MM_SetAES(u1HdrType, u4Flag , &rDRM);
        }
        else if (u1HdrType == eSWDMX_MEDIA_AUDIO &&
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
                DMX_MM_SetDRMMode(u1HdrType, DMX_DRM_MODE_AES);
                DMX_MM_SetAES(u1HdrType, u4Flag , &rDRM);
            }
        }
        LOG(6, "Drm u4Offset = %ld, DRM u4EncryLen = %ld\n",  
            rDRM.u4Offset, rDRM.u4EncryLen);

        _rSwdmxAviInfo.u1DrmType = eSWDMX_MEDIA_NONE;
    }

#ifdef CC_DLNA_SUPPORT
    // DLNA
    if (INPUT_SRC != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
    {
        if ((u1HdrType == eSWDMX_MEDIA_AUDIO) ||
            (u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO))
        {
            u1HdrType = AVI_AUD_PREBUF_IDX + u1TrackIdx;

            if (u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK)
            {
                u4Pts = _rSwdmxAviInfo.u4AudPts[u1TrackIdx];
            }
            else
            {
                u4Pts = 0;
            }
        }
    }
#endif

    rDmxMMData.u4BufStart = (UINT32)(VIRTUAL(_rFeederInfo.u4StartAddr));
    rDmxMMData.u4BufEnd = (UINT32)(VIRTUAL(_rFeederInfo.u4EndAddr));
    rDmxMMData.u4FrameSize = u4ChunkSize;
    rDmxMMData.u4Pts = u4Pts;
    rDmxMMData.u4Dts = u4Pts;
    LOG(11, "Header Type = %ld u4FrameSize = 0x%08x pts = 0x%08x\n", 
        u1HdrType, u4ChunkSize, u4Pts);
    
    if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
    {
        LOG(0, "Demuxer fails to move data.\n");
        *pu4ConsumeSize = 0;
        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
    }

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        _SWDMX_AviCalculatePts(u1HdrType, u1TrackIdx, u4ChunkSize);
        if( (_rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts != 0)&&
            (_rSwdmxAviInfo.u4VidHeaderPtr == NULL)&&
            (_rSwdmxAviInfo.u4NumSentVid == 0))
        {
             UNUSED(_SWDMX_AviSeek(_rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts, prRangeList));
             _rSwdmxAviInfo.u8VSeekPts = 
                _rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts;
             _rSwdmxAviInfo.u8ASeekPts = 
                _rSwdmxAviInfo.prDmxRangeInfo->t_vid_start_render_pts;
             _rSwdmxAviInfo.fgFeederInvalid = TRUE;
        }
        _rSwdmxAviInfo.u4NumSentVid++;
    }

    *pu4ConsumeSize = u4ChunkSize + AVI_PARSER_OFFSET + i;
    if (fgRingBuf)
    {
        *pu4ConsumeSize += ((UINT32)(VIRTUAL(_rFeederInfo.u4EndAddr)) - 
                                                           (UINT32)(pucOriginalPtr) - 
                                                           u4RingSize);
    }

    _rSwdmxAviInfo.u4ErrorData = 0;
    
    if (_rSwdmxAviInfo.fgIsBlock == TRUE)
    {
        _rSwdmxAviInfo.fgIsBlock = FALSE;
    }
    
    return eSWDMX_HDR_PRS_SUCCEED;
}

static BOOL _SWDMX_AviGetDataFromFeeder(FEEDER_REQ_DATA_T *prFeederData)
{
    if (!_SWDMX_ReceiveFeederAck((VOID*)prFeederData))
    {
        LOG(3, "_SWDMX_AviGetDataFromFeeder Ack Fail!!!\n");

        _rSwdmxAviInfo.fgFeederInvalid = TRUE;
        //ASSERT(0);
        return FALSE;        
    }

    if (prFeederData->eDataType == FEEDER_SOURCE_INVALID)
    {
        LOG(3, "_SWDMX_AviGetDataFromFeeder Timeout!!!\n");
#ifdef CC_DLNA_SUPPORT
        if (INPUT_SRC != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        {
            _rSwdmxAviInfo.fgReqDataMiss = TRUE;
        }
        else
#endif
        {
            _rSwdmxAviInfo.fgFeederInvalid = TRUE;
        }
        return FALSE;
    }

    if ((prFeederData->u4Id + 1) != _rSwdmxAviInfo.u4FeederReqID)
    {
        LOG(3, "Feeder ID mismatch!!! feeder id = %ld\n", prFeederData->u4Id);
        _rSwdmxAviInfo.fgFeederInvalid = TRUE;
        return FALSE;
    }

    _rSwdmxAviInfo.fgReqDataMiss = FALSE;

    return TRUE;
}

static BOOL _CheckAviChunkType(UCHAR *pucBuf, UINT8 *pu1HdrType, UINT8 *pu1TrackIdx)
{
    INT32 i;

    if (_rSwdmxAviInfo.fgEnVideo)
    {
        if (_CheckAviChunkIndex(pucBuf, 
             _rSwdmxAviInfo.u4VStrmID) == TRUE)
        {
            *pu1HdrType = eSWDMX_MEDIA_VIDEO;
            return TRUE;
        }
    }
    
    if (_rSwdmxAviInfo.fgEnSubTitle)
    {
        if (_CheckAviChunkIndex(pucBuf, 
             _rSwdmxAviInfo.u4SpStrmID) == TRUE)
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
  
        ucStreamID[1] = (_rSwdmxAviInfo.u4VStrmID >> 8) & 0xff;
        ucStreamID[0] = (_rSwdmxAviInfo.u4VStrmID >> 0) & 0xff;

        if (pucBuf[0] == ucStreamID[0] && pucBuf[1] == ucStreamID[1])
        {
            *pu1HdrType = eSWDMX_MEDIA_VIDEO;
            return TRUE;
        }

        ucStreamID[1] = (_rSwdmxAviInfo.u4AStrmID >> 8) & 0xff;
        ucStreamID[0] = (_rSwdmxAviInfo.u4AStrmID >> 0) & 0xff;

        if (pucBuf[0] == ucStreamID[0] && pucBuf[1] == ucStreamID[1])
        {
            *pu1HdrType = eSWDMX_MEDIA_AUDIO;
            return TRUE;
        }
    }
    else if (_CheckAviChunkIndex(pucBuf, ((AVI_VID_COMPRE_DATA) | (pucBuf[0]) | (pucBuf[1] << 8))) == TRUE)
    {
        UCHAR ucStreamID[2];

	ucStreamID[1] = (_rSwdmxAviInfo.u4VStrmID >> 8) & 0xff;
	ucStreamID[0] = (_rSwdmxAviInfo.u4VStrmID >> 0) & 0xff;
	
	if ((pucBuf[0] == ucStreamID[0] && pucBuf[1] == ucStreamID[1]) &&
	    (_rSwdmxAviInfo.fgEnVideo))
	{
           *pu1HdrType = eSWDMX_MEDIA_VIDEO;
	   return TRUE;
	}
    }
    else if (_CheckAviChunkIndex(pucBuf, ((AVI_DRM_VID_CHUNK_ID) | (pucBuf[0]) | (pucBuf[1] << 8)))== TRUE)
    {
        UCHAR ucStreamID[2];
  
        ucStreamID[1] = (_rSwdmxAviInfo.u4VStrmID >> 8) & 0xff;
        ucStreamID[0] = (_rSwdmxAviInfo.u4VStrmID >> 0) & 0xff;

        if (pucBuf[0] == ucStreamID[0] && pucBuf[1] == ucStreamID[1])
        {
            *pu1HdrType = eSWDMX_MEDIA_DRM_VIDEO;
            return TRUE;
        }
        
        ucStreamID[1] = (_rSwdmxAviInfo.u4AStrmID >> 8) & 0xff;
        ucStreamID[0] = (_rSwdmxAviInfo.u4AStrmID >> 0) & 0xff;

        if (pucBuf[0] == ucStreamID[0] && pucBuf[1] == ucStreamID[1])
        {
            *pu1HdrType = eSWDMX_MEDIA_DRM_AUDIO;
            return TRUE;
        }
    }

    for (i = 0; (i < _rSwdmxAviInfo.u1TotalATrack) && (i < AVI_DIVX_MAX_AUD_TRACK); i++)
    {
        if ((_rSwdmxAviInfo.fgKeyExist)&&(_rSwdmxAviInfo.parAudKeyIdxTbl[i]))
        {
            if (_CheckAviChunkIndex(pucBuf, 
                _rSwdmxAviInfo.parAudKeyIdxTbl[i]->t_strm_id.u.t_stm_avi_id) == TRUE)
            {
                if (_rSwdmxAviInfo.fgEnAudio && _rSwdmxAviInfo.u1CurATrackIdx == i)
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
            if(_rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)
            {
                if(_rSwdmxAviInfo.prAudIdxTbl[i])
                {
                    if (_CheckAviChunkIndex(pucBuf, 
                        _rSwdmxAviInfo.prAudIdxTbl[i]->t_strm_id.u.t_stm_avi_id) == TRUE)
                    {
                        if(_rSwdmxAviInfo.fgEnAudio &&(_rSwdmxAviInfo.u1CurATrackIdx == i))
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


static void _SWDMX_AviPrsSubtitle(UCHAR*    pucBuf,
                                 UINT32*                 pt_pts_start,
                                 UINT32*                 pt_pts_end)
{
    UINT32  ui4_hour;
    UINT32  ui4_min;
    UINT32  ui4_sec;
    UINT32  ui4_ms;

    UCHAR ucSubBuf[26];

    if ((UINT32)pucBuf <= (UINT32)(VIRTUAL(_rFeederInfo.u4EndAddr) - 26))
    {
        x_memcpy(ucSubBuf, (void*)pucBuf, 26);
    }
    else
    {
        UINT32 u4Temp = (UINT32)(VIRTUAL(_rFeederInfo.u4EndAddr) -(UINT32)pucBuf);
        
        x_memcpy(ucSubBuf, (void*)pucBuf, u4Temp);
        x_memcpy(&ucSubBuf[u4Temp], 
                           (void*)VIRTUAL(_rFeederInfo.u4StartAddr), 
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


static void _SWDMX_AviPrsDrmChunk(UINT8 u1HdrType, UINT32 u4StartAddr)
{
    UCHAR ucDrmBuf[AVI_DRM_SIZE];

    if (u4StartAddr <= (UINT32)(VIRTUAL(_rFeederInfo.u4EndAddr) - AVI_DRM_SIZE))
    {
        x_memcpy(ucDrmBuf, (void*)u4StartAddr, AVI_DRM_SIZE);
    }
    else
    {
        UINT32 u4Temp = (UINT32)(VIRTUAL(_rFeederInfo.u4EndAddr) -u4StartAddr);
        
        x_memcpy(ucDrmBuf, (void*)u4StartAddr, u4Temp);
        x_memcpy(&ucDrmBuf[u4Temp], 
                           (void*)VIRTUAL(_rFeederInfo.u4StartAddr), 
                           (AVI_DRM_SIZE - u4Temp)); 
    }

    _rSwdmxAviInfo.u1DrmType = u1HdrType;
    _rSwdmxAviInfo.u2FrameKeyIdx = (UINT16)((ucDrmBuf[0]) |
                                                                         (ucDrmBuf[1] << 8));

    _rSwdmxAviInfo.u4EncryptOffset = (ucDrmBuf[2] |
    								(ucDrmBuf[3] << 8) | 
    								(ucDrmBuf[4] << 16) | 
    								(ucDrmBuf[5] << 24));


    _rSwdmxAviInfo.u4EncryptLength = (ucDrmBuf[6] | 
    								 (ucDrmBuf[7] << 8) |
    								 (ucDrmBuf[8] << 16) | 
    								 (ucDrmBuf[9] << 24));

    if (_rSwdmxAviInfo.u2FrameKeyIdx >= AVI_DRM_MAX_NUM_KEY)
    {
        LOG(3, "DRM Error Key Index is too large\n");
        _rSwdmxAviInfo.u1DrmType = eSWDMX_MEDIA_NONE;
    }

    LOG(9, "Drm Type = %ld\n", u1HdrType);
    LOG(9, "Drm Frame Key Index = %ld\n", _rSwdmxAviInfo.u2FrameKeyIdx);
    LOG(9, "Drm Frame Encrypt Offset = %ld\n", _rSwdmxAviInfo.u4EncryptOffset);
    LOG(9, "Drm Frame Encrypt Length = %ld\n", _rSwdmxAviInfo.u4EncryptLength);
}


static BOOL _SWDMX_InitDmx(UINT8 u1HdrType, VOID* pfnCbFunc, BOOL fgEos)
{
    BOOL fgRet = TRUE;
    UINT32 u4Flag = 0; 
    FBM_POOL_T* prFbmPool;
    DMX_MM_T rDmxMMInfo;

    x_memset(&rDmxMMInfo, 0, sizeof(DMX_MM_T));

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;

        if (!_rSwdmxAviInfo.fgEnVDmx)
        {
            // Allocate video buffer from FBM
            prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
            if((prFbmPool == NULL) || (prFbmPool->u4Addr == 0))
            {
                LOG(1, "_SWDMX_SetVideoStreamID (prFbmPool == NULL) || (prFbmPool->u4Addr == NULL)\n");
                return FALSE;
            }

            u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY | 
              DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF);

            rDmxMMInfo.fgAllocBuf = FALSE;
            rDmxMMInfo.u4BufAddr = prFbmPool->u4Addr;
            rDmxMMInfo.u4BufSize = prFbmPool->u4Size;

            u4Flag |= DMX_MM_FLAG_SEARCH_START_CODE;

            switch (_rSwdmxAviInfo.u4VDecType)
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
            fgRet = DMX_MM_Set(eSWDMX_MEDIA_VIDEO, u4Flag, &rDmxMMInfo);
        }
        else if (fgEos)
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;
            fgRet = DMX_MM_Set(eSWDMX_MEDIA_VIDEO, u4Flag, &rDmxMMInfo);
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        UINT32 au4BufStart[2], au4BufEnd[2];

        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_AUDIO;
    
        if (!_rSwdmxAviInfo.fgEnADmx)
        {
            u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY | 
              DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF);
        
#ifdef AUD_OLDFIFO_INTF
            if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
                 &au4BufEnd[1]) != AUD_OK)
#else
            if (AUD_GetAudFifo(AUD_DEC_MAIN, &au4BufStart[0], &au4BufEnd[0]) != AUD_OK)
#endif
            {
                    LOG(1, "_SWDMX_SetAudioStreamID Can't get audio buffers!\n");
                    return FALSE;
            }
            rDmxMMInfo.fgAllocBuf = FALSE;
            
            rDmxMMInfo.u4BufAddr = au4BufStart[0];
            rDmxMMInfo.u4BufSize = au4BufEnd[0] - au4BufStart[0];

            // Record the audio fifo size
            _rSwdmxAviInfo.u4AFifoSize = rDmxMMInfo.u4BufSize;

#ifdef CC_DLNA_SUPPORT
            if (INPUT_SRC == MM_SRC_TYPE_HIGH_SPEED_STORAGE)
#endif
            {
#ifdef CC_DLNA_SUPPORT
                DMX_AudHandler_SetEnable(FALSE);
#endif
                fgRet = DMX_MM_Set(eSWDMX_MEDIA_AUDIO, u4Flag, &rDmxMMInfo);
            }
#ifdef CC_DLNA_SUPPORT
            else
            {
                INT32 i;

                DMX_AudHandler_SetEnable(TRUE);
                
                fgRet = DMX_MM_Set(eSWDMX_MEDIA_AUDIO, u4Flag, &rDmxMMInfo);
                if(!DMX_AudHandler_Set(eSWDMX_MEDIA_AUDIO, TRUE))
                {
                    LOG(1, "%s:%d, DMX_AudHandler_Set fail\n", __FUNCTION__, __LINE__);
                }

                for (i = 0; i < AVI_DIVX_MAX_AUD_TRACK; i++)
                {
                    rDmxMMInfo.fgAllocBuf = TRUE;            
                    rDmxMMInfo.u4BufSize = au4BufEnd[0] - au4BufStart[0];            
                    fgRet = DMX_MM_Set(AVI_AUD_PREBUF_IDX + i, u4Flag, &rDmxMMInfo);
                    if(!DMX_AudHandler_Set(AVI_AUD_PREBUF_IDX + i, FALSE))
                    {
                        LOG(1, "%s:%d, DMX_AudHandler_Set fail\n", __FUNCTION__, __LINE__);
                    }
                }
                fgRet = DMX_AudHandler_SetActivePidx(AVI_AUD_PREBUF_IDX);
            }
#endif
        }
        else if (fgEos)
        {
#ifdef CC_DLNA_SUPPORT
            if (INPUT_SRC == MM_SRC_TYPE_HIGH_SPEED_STORAGE)
#endif
            {
                u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
                rDmxMMInfo.fgSearchStartCode = FALSE;
                fgRet = DMX_MM_Set(eSWDMX_MEDIA_AUDIO, u4Flag, &rDmxMMInfo);
            }
#ifdef CC_DLNA_SUPPORT
            else
            {
                INT32 i;

                u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
                rDmxMMInfo.fgSearchStartCode = FALSE;
                for (i = 0; i < AVI_DIVX_MAX_AUD_TRACK; i++)
                {
                    fgRet = DMX_MM_Set(AVI_AUD_PREBUF_IDX + i, u4Flag, &rDmxMMInfo);
                }
            }
#endif
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_SUBTITLE)
    {
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_PES;

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

            fgRet = DMX_MM_Set(eSWDMX_MEDIA_SUBTITLE, u4Flag, &rDmxMMInfo);
        }
    }

    return fgRet;
}


static BOOL _SWDMX_SendAviEos(UINT8 u1HdrType)
{
    UINT16 u2MaxQNum, u2CurQNum;
    //UINT32 u4DummyData[4];
    DMX_MM_DATA_T rDmxMMData;

    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);
        LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
        //if (u2CurQNum > (_rSwdmxAviInfo.u4VidFps/2))
        if (u2CurQNum > (u2MaxQNum - 10))
        {
            x_thread_delay(1);
            return FALSE;
        }
    }
    
    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if (_SWDMX_InitDmx(u1HdrType, NULL, TRUE) == FALSE)
    {
        return FALSE;
    }

    rDmxMMData.u4BufStart = _rFeederInfo.u4StartAddr;
    rDmxMMData.u4BufEnd = _rFeederInfo.u4EndAddr;
    rDmxMMData.u4StartAddr = _rFeederInfo.u4StartAddr;
    rDmxMMData.u4FrameSize = 4;
    rDmxMMData.fgEOS = TRUE;

#ifdef CC_DLNA_SUPPORT
    if (INPUT_SRC == MM_SRC_TYPE_HIGH_SPEED_STORAGE)
#endif
    {
        return DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500);
    }
#ifdef CC_DLNA_SUPPORT
    else
    {
        if (u1HdrType != eSWDMX_MEDIA_AUDIO)
        {
            return DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500);
        }
        else
        {
            INT32 i;

            for (i = 0; i < AVI_DIVX_MAX_AUD_TRACK; i++)
            {
                if (!DMX_MM_MoveData(AVI_AUD_PREBUF_IDX + i, &rDmxMMData, 500))
                {
                    LOG(3, "Fail to send audio eos\n");
                    return FALSE;
                }
            }
            return TRUE;
        }
    }
#endif
}


static BOOL _SWDMX_AviSeek(UINT64 u8SeekTime, SWDMX_RANGE_LIST_T* prRangeList)
{
    BOOL fgFindKey = FALSE;
    UINT8 u1ReadDrmSize = 0;
    INT32 i;
    MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl = _rSwdmxAviInfo.prVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prAudKeyIdxTbl = 0;
    MM_RANGE_ELMT_IDXTBL_T *prVidTmpKeyIdxTbl = 0;
    MM_RANGE_ELMT_IDXTBL_T *prAudBaseKeyIdxTbl = 0;
    MM_RANGE_ELMT_IDXTBL_T *prAudTmpKeyIdxTbl = 0;
    MM_RANGE_ELMT_IDXTBL_T *prVidIdxTbl = 0;
    MM_RANGE_ELMT_IDXTBL_T *prAudIdxTbl = 0;
    UINT32* pu4AudPts;

    LOG(7, "_SWDMX_AviSeek u8SeekTime = 0x%08x\n", u8SeekTime);

    if ( _rSwdmxAviInfo.prVidKeyIdxTbl &&
        _rSwdmxAviInfo.prVidKeyIdxTbl->ui4_number_of_entry != 0)
    {
        if (!_rSwdmxAviInfo.fgKeyExist)
        {
            LOG(5, "_SWDMX_AviSeek doesn't have the key table\n");
            return FALSE;
        }

        _rSwdmxAviInfo.prCurVidKeyIdxTbl = prVidKeyIdxTbl;
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
            if (_rSwdmxAviInfo.prVidKeyIdxTbl->pt_next_partial->u.pt_pb_idx_tbl_entry[0].ui4_pts < u8SeekTime)
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
                _rSwdmxAviInfo.prCurVidKeyIdxTbl = prVidKeyIdxTbl;
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
            (i < (_rSwdmxAviInfo.prVidKeyIdxTbl->ui4_number_of_entry - 1))*/)
        {
            _rSwdmxAviInfo.u4CurSentKey = i - 1;
        }
        /*else if (i == (_rSwdmxAviInfo.prVidKeyIdxTbl->ui4_number_of_entry - 1))
        {
            _rSwdmxAviInfo.u4CurSentKey = i;
        }*/
        else
        {
            _rSwdmxAviInfo.u4CurSentKey= 0;
        }

            _rSwdmxAviInfo.u4CurRepeatFrameIdx = _rSwdmxAviInfo.u4CurSentKey;

        if (prVidKeyIdxTbl != 0)
        {
            _rSwdmxAviInfo.u8CurDmxMoviPos = 
                prVidKeyIdxTbl->ui8_base_offset + 
                prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxAviInfo.u4CurSentKey].ui4_relative_offset -
                (AVI_PARSER_OFFSET + u1ReadDrmSize);

            if( _rSwdmxAviInfo.u8CurDmxMoviPos < prRangeList->prDmxRangeInfo->ui8_fileoffset)
            {
                _rSwdmxAviInfo.u8CurDmxMoviPos = prRangeList->prDmxRangeInfo->ui8_fileoffset;
            }
            _rSwdmxAviInfo.u4CurVidPts = 
                prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxAviInfo.u4CurSentKey].ui4_pts;

            LOG(5, "Avi Seek key frame number = 0x%08x Video offset = %lld\n", 
                _rSwdmxAviInfo.u4CurSentKey,
                _rSwdmxAviInfo.u8CurDmxMoviPos);

            LOG(5, "Avi Seek Video pts = 0x%08x\n", 
                prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxAviInfo.u4CurSentKey].ui4_pts);
        }

        for (i = 0; (i < _rSwdmxAviInfo.u1TotalATrack) && (i < AVI_DIVX_MAX_AUD_TRACK); i++)
        {
            if (prVidKeyIdxTbl == _rSwdmxAviInfo.prVidKeyIdxTbl)
            {
                prAudKeyIdxTbl = _rSwdmxAviInfo.parAudKeyIdxTbl[i];
                pu4AudPts = (UINT32*)prAudKeyIdxTbl->u.pv_idx_tbl_entry;

                LOG(5, "prAudKeyIdxTbl = 0x%08x\n", prAudKeyIdxTbl);
            }
            else
            {
                prVidIdxTbl = _rSwdmxAviInfo.prVidKeyIdxTbl->pt_next_partial;
                prAudIdxTbl = _rSwdmxAviInfo.parAudKeyIdxTbl[i]->pt_next_partial;

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
                _rSwdmxAviInfo.u4AudPts[i] = 
                    pu4AudPts[_rSwdmxAviInfo.u4CurSentKey];
                LOG(5, "Avi Seek Audio pts = 0x%08x\n", 
                    pu4AudPts[_rSwdmxAviInfo.u4CurSentKey]);
            }
            else
            {
                LOG(0,"audio pts tabel is NULL\n");
            }
            
        }
    }
    else if ((_rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK) &&
        _rSwdmxAviInfo.parAudKeyIdxTbl[_rSwdmxAviInfo.u1CurATrackIdx] &&
        _rSwdmxAviInfo.parAudKeyIdxTbl[_rSwdmxAviInfo.u1CurATrackIdx]->ui4_number_of_entry != 0)
    {
        prAudKeyIdxTbl = _rSwdmxAviInfo.parAudKeyIdxTbl[_rSwdmxAviInfo.u1CurATrackIdx];
        pu4AudPts = (UINT32*)prAudKeyIdxTbl->u.pv_idx_tbl_entry;
        for (i = 0; i < _rSwdmxAviInfo.parAudKeyIdxTbl[_rSwdmxAviInfo.u1CurATrackIdx]->ui4_number_of_entry; i++)
        {
            if (pu4AudPts[i]> u8SeekTime)
            {
                break;
            }
        }

        prAudBaseKeyIdxTbl = _rSwdmxAviInfo.parAudKeyIdxTbl[_rSwdmxAviInfo.u1CurATrackIdx];
        prAudTmpKeyIdxTbl = prAudKeyIdxTbl;

         // There r extra riffs
        if (_rSwdmxAviInfo.parAudKeyIdxTbl[_rSwdmxAviInfo.u1CurATrackIdx]->pt_next_partial)
        {
            INT32 i4NumRiffs = 2;
            
            LOG(5, "There are multiple riffs\n");
            if (_rSwdmxAviInfo.parAudKeyIdxTbl[_rSwdmxAviInfo.u1CurATrackIdx]->pt_next_partial->u.pt_pb_idx_tbl_entry[0].ui4_pts < u8SeekTime)
            {
                prAudKeyIdxTbl = _rSwdmxAviInfo.parAudKeyIdxTbl[_rSwdmxAviInfo.u1CurATrackIdx]->pt_next_partial;
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
            (i < (_rSwdmxAviInfo.prVidKeyIdxTbl->ui4_number_of_entry - 1))*/)
        {
            _rSwdmxAviInfo.u4CurSentKey = i - 1;
        }
        /*else if (i == (_rSwdmxAviInfo.prVidKeyIdxTbl->ui4_number_of_entry - 1))
        {
            _rSwdmxAviInfo.u4CurSentKey = i;
        }*/
        else
        {
            _rSwdmxAviInfo.u4CurSentKey= 0;
        }

        for (i = 0; (i < _rSwdmxAviInfo.u1TotalATrack) && (i < AVI_DIVX_MAX_AUD_TRACK); i++)
        {
            if (prAudBaseKeyIdxTbl == _rSwdmxAviInfo.parAudKeyIdxTbl[_rSwdmxAviInfo.u1CurATrackIdx])
            {
                prAudKeyIdxTbl = _rSwdmxAviInfo.parAudKeyIdxTbl[i];
                pu4AudPts = (UINT32*)prAudKeyIdxTbl->u.pv_idx_tbl_entry;

                LOG(5, "prAudKeyIdxTbl = 0x%08x\n", prAudKeyIdxTbl);
            }
            else
            {
                prAudIdxTbl = _rSwdmxAviInfo.parAudKeyIdxTbl[i]->pt_next_partial;
                prAudTmpKeyIdxTbl = _rSwdmxAviInfo.parAudKeyIdxTbl[_rSwdmxAviInfo.u1CurATrackIdx];
                
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
                _rSwdmxAviInfo.u4AudPts[i] = 
                    pu4AudPts[_rSwdmxAviInfo.u4CurSentKey];
                LOG(5, "Avi Seek Audio pts = 0x%08x\n", 
                    pu4AudPts[_rSwdmxAviInfo.u4CurSentKey]);
            }
            else
            {
                LOG(1, "2audio table is NULL\n");
            }

        }

        if(_rSwdmxAviInfo.prVidKeyIdxTbl)
        {
            _rSwdmxAviInfo.u8CurDmxMoviPos = 
                    _rSwdmxAviInfo.prVidKeyIdxTbl->ui8_base_offset + 
                    _rSwdmxAviInfo.prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxAviInfo.u4CurSentKey].ui4_relative_offset -
                    (AVI_PARSER_OFFSET + u1ReadDrmSize); 
         }
    }
    else
    {
        LOG(5, "There is no audio and video index table for Avi seek\n");
    }

    _SWDMX_AviResetFeeder();

    _rSwdmxAviInfo.fgIsSeek = TRUE;
    _rSwdmxAviInfo.u4LeftBytesInFeeder = 0;
    _rSwdmxAviInfo.u8CurMoviPos = _rSwdmxAviInfo.u8CurDmxMoviPos;

    // Set STC, temp, should update by audio (audio master)
    //STC_SetStcValue(_rSwdmxAviInfo.u4CurVidPts);
    
    LOG(7, "_SWDMX_AviSeek Key = %ld\n", _rSwdmxAviInfo.u4CurSentKey);
    return TRUE;
}


static BOOL _SWDMX_AviResetFeeder(void)
{
    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    //LOG(5, "_SWDMX_AviResetFeeder Cur Fed's WPtr = 0x%08x\n", _rFeederInfo.u4WriteAddr);
    _rSwdmxAviInfo.u4CurDmxFeederRPtr = _rFeederInfo.u4WriteAddr;

    // Update read ptr to feeder
    FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(_rSwdmxAviInfo.u4CurDmxFeederRPtr));
    return TRUE;
}


static void _SWDMX_AviCalculatePts(UINT8 u1HdrType, UINT8 u1TrackIdx, UINT32 u4ChunkSize)
{
    UINT64 u8Numerator = 0, u8Denominator = 0;

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {   
        if(_rSwdmxAviInfo.prVidIdxTbl)
        {
            u8Numerator = 
                (UINT64)_rSwdmxAviInfo.prVidIdxTbl->ui4_scale * DEMUX_AVI_AV_PERIOD 
                + _rSwdmxAviInfo.u8VRemainder;
            u8Denominator = 
                (UINT64)_rSwdmxAviInfo.prVidIdxTbl->ui4_rate;

            _rSwdmxAviInfo.u4CurVidPts += u8Div6432(u8Numerator, u8Denominator, &_rSwdmxAviInfo.u8VRemainder);
        }
        else
        {
            LOG(1, "Video table is NULL\n");
            return ;
        }
    }
    else if ((u1HdrType == eSWDMX_MEDIA_AUDIO ||u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO) &&
        (u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK))
    {
        if (_rSwdmxAviInfo.fgKeyExist)
        {
            if(_rSwdmxAviInfo.parAudKeyIdxTbl[u1TrackIdx])
            {
                if (_rSwdmxAviInfo.parAudKeyIdxTbl[u1TrackIdx]->b_is_VBR)
                {
                    u8Numerator = 
                        (UINT64)_rSwdmxAviInfo.parAudKeyIdxTbl[u1TrackIdx]->ui4_scale * DEMUX_AVI_AV_PERIOD
                        + _rSwdmxAviInfo.u8ARemainder;
                    u8Denominator = 
                        (UINT64)_rSwdmxAviInfo.parAudKeyIdxTbl[u1TrackIdx]->ui4_rate;
                }
                else
                {
                    u8Numerator = (UINT64)u4ChunkSize * DEMUX_AVI_AV_PERIOD
                        + _rSwdmxAviInfo.u8ARemainder;
                    u8Denominator = 
                        (UINT64)_rSwdmxAviInfo.parAudKeyIdxTbl[u1TrackIdx]->ui4_avg_bytes_per_sec;
                }
            }
            else
            {
                LOG(1, "audio key table is NULL\n");
                return ;
            }
        }
        else
        {   
            if(_rSwdmxAviInfo.prAudIdxTbl[u1TrackIdx])
            {
                if (_rSwdmxAviInfo.prAudIdxTbl[u1TrackIdx]->b_is_VBR)
                {
                    u8Numerator = 
                        (UINT64)_rSwdmxAviInfo.prAudIdxTbl[u1TrackIdx]->ui4_scale * DEMUX_AVI_AV_PERIOD
                        + _rSwdmxAviInfo.u8ARemainder;
                    u8Denominator = 
                        (UINT64)_rSwdmxAviInfo.prAudIdxTbl[u1TrackIdx]->ui4_rate;
                }
                else
                {
                    u8Numerator = (UINT64)u4ChunkSize * DEMUX_AVI_AV_PERIOD
                        + _rSwdmxAviInfo.u8ARemainder;
                    u8Denominator = 
                        (UINT64)_rSwdmxAviInfo.prAudIdxTbl[u1TrackIdx]->ui4_avg_bytes_per_sec;
                }
            }
            else
            {
                LOG(1, "Audio table is NULL\n");
                return ;
            }
        }
        _rSwdmxAviInfo.u4AudPts[u1TrackIdx]  += u8Div6432(u8Numerator, u8Denominator, &_rSwdmxAviInfo.u8ARemainder);
    }
}


static BOOL _SWDMX_AviCalculateRepeatFrame(INT32 i4PlaySpeed)
{
    UINT32 u4CurKey = _rSwdmxAviInfo.u4CurSentKey;
    MM_RANGE_ELMT_IDXTBL_T *prKeyIdxTbl = _rSwdmxAviInfo.prCurVidKeyIdxTbl;

    if (_rSwdmxAviInfo.u4VidRepeatFrame == 0)
    {
        if (_rSwdmxAviInfo.u4CurRepeatFrameIdx != u4CurKey &&
            (_rSwdmxAviInfo.u4CurRepeatFrameIdx < prKeyIdxTbl->ui4_number_of_entry))
        {
            UINT32 u4PtsDif = 0, u4MaxRepeatFrame;
    
            u4MaxRepeatFrame = AVI_MAX_REPEAT_FRAME;//(_rSwdmxAviInfo.u4VidFps * 1000 * 2)/abs(i4PlaySpeed);

            if (i4PlaySpeed > 2000)
            {
                if ((u4CurKey < (prKeyIdxTbl->ui4_number_of_entry)) &&
                    (prKeyIdxTbl->ui4_number_of_entry > AVI_MINIMUM_KEY_NUM))
                {
                    u4PtsDif = prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts -
                        prKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxAviInfo.u4CurRepeatFrameIdx].ui4_pts;

                    _rSwdmxAviInfo.u4VidRepeatFrame = 
                        (UINT32)((u4PtsDif * _rSwdmxAviInfo.u4VidFps * 2)/(90 * i4PlaySpeed));
                }
                else
                {
                    _rSwdmxAviInfo.u4VidRepeatFrame = (_rSwdmxAviInfo.u4VidFps * 1000)/abs(i4PlaySpeed);
                }
            }
            else if (i4PlaySpeed < 0)
            {
                if ((prKeyIdxTbl->ui4_number_of_entry > AVI_MINIMUM_KEY_NUM))
                {
                    u4PtsDif = prKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxAviInfo.u4CurRepeatFrameIdx].ui4_pts -
                        prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts;

                    _rSwdmxAviInfo.u4VidRepeatFrame = 
                        (UINT32)((u4PtsDif * _rSwdmxAviInfo.u4VidFps * 2)/(90 * abs(i4PlaySpeed)));;
                }
                else
                {
                    _rSwdmxAviInfo.u4VidRepeatFrame = (_rSwdmxAviInfo.u4VidFps * 1000)/abs(i4PlaySpeed);
                }
            }

            _rSwdmxAviInfo.u4VidRepeatFrame = (u4MaxRepeatFrame > _rSwdmxAviInfo.u4VidRepeatFrame) ?
                (_rSwdmxAviInfo.u4VidRepeatFrame) : u4MaxRepeatFrame;

            if ((u4CurKey == prKeyIdxTbl->ui4_number_of_entry - 1) ||
                (u4CurKey == 0))
            {
                _rSwdmxAviInfo.u4VidRepeatFrame = 
                    (AVI_FIX_REPEAT_FRAME/(abs(i4PlaySpeed)/1000));

                /*if (_rSwdmxAviInfo.u4VidRepeatFrame > AVI_MAX_REPEAT_FRAME)
                {
                    _rSwdmxAviInfo.u4VidRepeatFrame = AVI_MAX_REPEAT_FRAME;
                }
                else if (_rSwdmxAviInfo.u4VidRepeatFrame < AVI_MIN_REPEAT_FRAME)
                {
                    _rSwdmxAviInfo.u4VidRepeatFrame = AVI_MIN_REPEAT_FRAME;
                }*/                        
            }

            LOG(5, "u4PtsDif = %ld Current Idx = %ld Repeat frame = %ld\n", 
                u4PtsDif, u4CurKey, _rSwdmxAviInfo.u4VidRepeatFrame);

            if (_rSwdmxAviInfo.u4VidRepeatFrame == 0)
            {
                return FALSE;
            }

            _rSwdmxAviInfo.u4CurRepeatFrameIdx = u4CurKey;
            LOG(5, "Repeat Idx = %ld\n", _rSwdmxAviInfo.u4CurRepeatFrameIdx);

            return TRUE;
        }
        else
        {
            // Force to play the first key frame
            if (_rSwdmxAviInfo.fgStartTrick)
            {
                _rSwdmxAviInfo.fgStartTrick = FALSE;
                _rSwdmxAviInfo.u4VidRepeatFrame = (_rSwdmxAviInfo.u4VidFps * 1000)/abs(i4PlaySpeed);
                return TRUE;
            }
            
            return FALSE;
        }
    }

    _rSwdmxAviInfo.u4VidRepeatFrame--;
    return TRUE;
}



BOOL _SWDMX_AviReadBuffer(ENUM_SWDMX_STRM_TYPE_T eStrmType, 
    SWDMX_READ_INFO_T *prReadInfo)
{
    BOOL  fgRet = FALSE;
    
    switch (eStrmType)
    {
        case eSWDMX_STRM_TYPE_SUBTITLE:
            LOG(3, "Avi Get Subtitle Buffer serial number = 0x%x Frame Address = 0x%08x",
                prReadInfo->u1SerialNumber,
                prReadInfo->u4FrameAddr);
                
            fgRet = DMX_MM_GetBuffer(eSWDMX_MEDIA_SUBTITLE, 
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


BOOL _SWDMX_AviReleaseBuffer(ENUM_SWDMX_STRM_TYPE_T eStrmType, 
    SWDMX_READ_INFO_T *prReadInfo)
{
    BOOL  fgRet = FALSE;
    
    switch (eStrmType)
    {
        case eSWDMX_STRM_TYPE_SUBTITLE:
            fgRet = DMX_MM_UnlockBuffer(eSWDMX_MEDIA_SUBTITLE, 
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
BOOL _SWDMX_AVIStop(VOID)
{
    //FeederStop(FEEDER_PROGRAM_SOURCE);
    //FeederUnInstallCallback(FEEDER_PROGRAM_SOURCE, 
    //  FEEDER_NOTIFY_DATA_REQ_DONE, _FeederCallBack);
    //FeederClose(FEEDER_PROGRAM_SOURCE);

    //BOOL fgEnVDmx = _rSwdmxAviInfo.fgEnVDmx;
    //BOOL fgEnADmx = _rSwdmxAviInfo.fgEnADmx;
    //BOOL fgEnSPDmx = _rSwdmxAviInfo.fgEnSubDmx;
    DMX_MM_T rPid;

    rPid.fgEnable = FALSE;
   
    if (_rSwdmxAviInfo.fgEnVDmx)
    {
        _rSwdmxAviInfo.fgEnVDmx = FALSE;
        VERIFY(DMX_MM_Set(eSWDMX_MEDIA_VIDEO, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(eSWDMX_MEDIA_VIDEO);
    }
    if (_rSwdmxAviInfo.fgEnADmx)
    {
#ifdef CC_DLNA_SUPPORT
        if (INPUT_SRC == MM_SRC_TYPE_HIGH_SPEED_STORAGE)
#endif
        {
            _rSwdmxAviInfo.fgEnADmx = FALSE;
            VERIFY(DMX_MM_Set(eSWDMX_MEDIA_AUDIO, DMX_MM_FLAG_VALID, &rPid));
            DMX_MM_Free(eSWDMX_MEDIA_AUDIO);
        }
#ifdef CC_DLNA_SUPPORT
        else
        {
            INT32 i;

            _rSwdmxAviInfo.fgEnADmx = FALSE;
            VERIFY(DMX_MM_Set(eSWDMX_MEDIA_AUDIO, DMX_MM_FLAG_VALID, &rPid));
            DMX_MM_Free(eSWDMX_MEDIA_AUDIO);

            for (i = 0; i < AVI_DIVX_MAX_AUD_TRACK; i++)
            {
                VERIFY(DMX_MM_Set(AVI_AUD_PREBUF_IDX + i, DMX_MM_FLAG_VALID, &rPid));
                DMX_MM_Free(AVI_AUD_PREBUF_IDX + i);
                DMX_AudHandler_Free(AVI_AUD_PREBUF_IDX + i);
            }
        }

        DMX_AudHandler_SetEnable(FALSE);
#endif
    }
    if (_rSwdmxAviInfo.fgEnSubDmx)
    {
        _rSwdmxAviInfo.fgEnSubDmx = FALSE;
        VERIFY(DMX_MM_Set(eSWDMX_MEDIA_SUBTITLE, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(eSWDMX_MEDIA_SUBTITLE);
    }

    x_memset(&_rSwdmxAviInfo, 0, sizeof(SWDMX_AVI_INFO_T));

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


BOOL _SWDMX_AviAudioCb(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
    if(eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE)
    {
        //BOOL fgMp3Vbr = FALSE;
        AUD_DEC_INFO_T rADecInfo;
        MM_RANGE_ELMT_IDXTBL_T *prAudIdxTbl = NULL;

        //AUD_DspSetMp3PreparsingMode(TRUE);
        //fgMp3Vbr = AUD_DspIsMp3Vbr();
        AUD_DspGetMpegAudInfo(0, &rADecInfo);

        if ((_rSwdmxAviInfo.fgKeyExist) &&
            (_rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)&&
            (_rSwdmxAviInfo.parAudKeyIdxTbl[_rSwdmxAviInfo.u1CurATrackIdx]))
        {
            prAudIdxTbl = _rSwdmxAviInfo.parAudKeyIdxTbl[_rSwdmxAviInfo.u1CurATrackIdx];
        }
        else if((_rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK)&&
           ( _rSwdmxAviInfo.prAudIdxTbl[_rSwdmxAviInfo.u1CurATrackIdx] !=NULL))
        {
            prAudIdxTbl = _rSwdmxAviInfo.prAudIdxTbl[_rSwdmxAviInfo.u1CurATrackIdx];
        }
        else
        {
            LOG(0,"error happen: no audio table\n");
        }

        if (prAudIdxTbl)
        {
            if ((rADecInfo.e_aud_fmt == AUD_FMT_MP3) &&
                (!_rSwdmxAviInfo.fgMp3Vbr) &&
                (_rSwdmxAviInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK) &&
                (!prAudIdxTbl->b_is_VBR))
            {
                if (!_SWDMX_AviValidHdrBps(prAudIdxTbl->ui4_avg_bytes_per_sec, rADecInfo.ui4_data_rate/8))
                {
                    _rSwdmxAviInfo.fgMp3Abr = TRUE;
                    LOG(5, "SWDMX_AVI Mp3 Abr!!!!!\n");
                }
            }
        }

        LOG(5, "SWDMX_AviAudioCb Audio is ready\n");
        if (_rSwdmxAviInfo.fgEnVideo &&
            _rSwdmxAviInfo.prDmxRangeInfo->ui8_vid_duration >_rSwdmxAviInfo.u4TriggerAudPts)
        {
#if 0
            if ((_rSwdmxAviInfo.fgBadAudio) &&
                (rADecInfo.e_aud_fmt == AUD_FMT_MP3))
            {
                UINT32 u4DspPts;
                UINT32 u4PrsPos;

                LOG(5, "Bad Audio, Lookup audio pts from audio driver\n");
                AUD_GetDspPtsInfo(0, &u4DspPts, &u4PrsPos);
                _rSwdmxAviInfo.u4TriggerAudPts = u4DspPts;
                AUD_SetStartPts(0, u4DspPts);
                _rSwdmxAviInfo.fgBadAudio = FALSE;
            }
#endif            

            LOG(5, "Triggerred by video\n");
            VDP_TriggerAudReceive(_rSwdmxAviInfo.u4TriggerAudPts);
            LOG(5, "SWDMX_AviAudioCb VDP Trigger PTS = 0x%08x\n", 
                _rSwdmxAviInfo.u4TriggerAudPts);
            if(_rSwdmxAviInfo.fgVDecIsPause)
            {
                _SWDMX_AviVideoWakeUp();
                _rSwdmxAviInfo.fgVDecIsPause = FALSE;
            }

            if (_rSwdmxAviInfo.fgMp3Abr)
            {
                STC_SetSyncMode(AV_SYNC_MODE_NONE);
                STC_StartStc();
            }
        }
        else
        {
            // Audio Only case
            LOG(5, "Trigger audio by _SWDMX_AviAudioCb\n");
            AUD_MMAoutEnable(0, TRUE);
            STC_StartStc();
        }
        _rSwdmxAviInfo.fgAudIsReady = TRUE;
        _rSwdmxAviInfo.fgAudIsLoaded = TRUE;
        _rSwdmxAviInfo.fgGetMp3Status = TRUE;
    }
    else
    {
        // impossible
        LOG(1, "_SWDMX_AviAudioCb eAudioNotifyType != ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE\n");
    }

    return TRUE;
}

static void _SWDMX_AviVideoWakeUp(void)
{
    LOG(5, "_SWDMX_AviVideoWakeUp\n");
    VDP_SetPauseMm(0, FALSE);
    switch(_rSwdmxAviInfo.u4VDecType)
    {
        case ENUM_SWDMX_VDEC_VC1:
        case ENUM_SWDMX_VDEC_WMV7:
        case ENUM_SWDMX_VDEC_WMV8:
        case ENUM_SWDMX_VDEC_WMV9:
            VDEC_Play(ES0, VDEC_FMT_WMV);
            break;
        case ENUM_SWDMX_VDEC_DX311:
        case ENUM_SWDMX_VDEC_MPEG4:    
            VDEC_Play(ES0, VDEC_FMT_MP4);
            break;
        case ENUM_SWDMX_VDEC_MJPEG:
            VDEC_Play(ES0, VDEC_FMT_MJPEG);
            break;
        case ENUM_SWDMX_VDEC_MPEG1_MPEG2:
            VDEC_Play(ES0, VDEC_FMT_MPV);
            break;
        case ENUM_SWDMX_VDEC_H264:
            VDEC_Play(ES0, VDEC_FMT_H264);
            break;
        default:
            LOG(1, "SWDMX_AviAudioCb Unknown Video Type\n");
            break;
    }
}

static SWDMX_HDR_PRS_STATE_T  _SWDMX_AviBeforeDeliverUseIdx(UINT8 u1HdrType)
{
       DMX_MM_DATA_T rDmxMMData;
       
       x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

       if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
        FEEDER_E_OK)
        {
              LOG(7, "Feeder input buffer error.\n");
             return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }
        x_memcpy((VOID*)VIRTUAL(_rFeederInfo.u4StartAddr), 
            (VOID*)VIRTUAL(_rSwdmxAviInfo.u4VidHeaderPtr), 
            _rSwdmxAviInfo.u4VidHdrLength);

        rDmxMMData.u4BufStart = VIRTUAL(_rFeederInfo.u4StartAddr);
        rDmxMMData.u4BufEnd = VIRTUAL(_rFeederInfo.u4EndAddr);
        rDmxMMData.u4StartAddr = VIRTUAL(_rFeederInfo.u4WriteAddr);
        rDmxMMData.u4FrameSize = _rSwdmxAviInfo.u4VidHdrLength;

        if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
        {
            LOG(1, "Demuxer fails to move sequence header.\n");
            return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
        }
 
        _rSwdmxAviInfo.u4NumSentVid++;

       return eSWDMX_HDR_PRS_FED_MOVE_FAIL;

}



