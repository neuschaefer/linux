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
 * $RCSfile: swdmx_mkv.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_mkv.c
 *  Software demux for mkv - internal interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "swdmx_mkv.h"
#include "vdec_drvif.h"
#include "u_midxbuld.h"
#include "swdmx_debug.h"
#include "aud_drvif.h"
#include "x_util.h"

#if defined(CC_MT5387) || defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395)
#include "b2r_drvif.h"
#include "b2r_if.h"
#endif
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define AUD_PREBUF_EN

#define MKV_BUFFER_SIZE (235 * 7 * 1024)
#define MIN_READ_SIZE (235 * 1024)
#ifdef LINUX_TURNKEY_SOLUTION
#define FDR_ALIGN_SIZE (3 * 1024)
#else
#define FDR_ALIGN_SIZE (1 * 1024)
#endif
#define RING_BUF_RSV 64
#define MIN_REPEAT_FRM 2    // visible repeat period
#define SKIP_STRM 0xFFFF
#define TRICK_2_NORMAL 1
#define NORMAL_2_TRICK 2
#define MAX_KEY_FRM 5
#define SEND_EOS_THRD 3
#define MKV_KEY_TABLE_THRESHOLD 3
#define TIME_CODE_SCALE 100000
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define LE_LONG32(a) ((a >> 24) | ((a & 0x00FF0000) >> 8) | \
           ((a & 0x0000FF00) << 8) | (a << 24))
#define cal_min(x, y) ((x < y) ? x : y)
#define cal_abs(x)                  (((x) >= 0) ? (x) : -(x))

#define IDR_PIC 5
#define IS_1ST_SLICE(x) (((x & 0x80) > 0))
#define IS_IDR_NALU(x) (((x & 0x1f) == IDR_PIC))
#define IS_FRM_NALU(x) (((x & 0x1f) <= IDR_PIC))

#define AVC_I_FRM   2
#define AVC_B_FRM   1
#define AVC_P_FRM   0
#define AVC_UNKNOWN_FRM   5

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static SWDMX_MKV_INFO_T _rSwdmxMkvInfo = {0};

static UINT8* pAVADmaAddr= NULL;
static BOOL fgActivePidxIsUnKnown = TRUE;
static BOOL fgAudioNnotifyDecodeDone = FALSE;

#ifdef __KERNEL__
static FEEDER_BUF_INFO_T _rFeederInfo;
#endif

static FEEDER_BUF_INFO_T _rFeederMkvInfo;

static UINT32 _u4LastAlignOffset;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static BOOL _SWDMXInitDmx(UINT8 u1HdrType, VOID* pfnCbFunc, BOOL fgEos);

static BOOL _SWDMXResetFeeder(void);

static BOOL _SWDMXMkvPrsDrmInfo(UCHAR* pucBitstream, UINT32 u4BufLength);
    
static BOOL _SWDMXMkvDecDrmData(UINT8 u1HdrType, UCHAR* pucFifo, UINT32 u4BlkSz);

static BOOL _SWDMXMkvSwitchTrack(BOOL fgIsTPT);

static BOOL _vMKVDeliverData(UINT8 u1HdrType, UCHAR* pucFifo, 
    UINT32 u4DataLen, BOOL bInsertSC, BOOL bIsFdrBuf);

static BOOL _MMVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT32 u4Addr;

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    rPesInfo.u4PTS = _rSwdmxMkvInfo.u4CurVidPts;
    rPesInfo.u4DTS = prPes->u4Dts;
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    rPesInfo.fgDtsValid = prPes->fgPtsDts;
    rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo.fgEos = prPes->fgEOS;
    rPesInfo.u8Offset = _rSwdmxMkvInfo.u8VidOffset;
    #ifdef CC_VDEC_FMT_DETECT
    rPesInfo.u4DmxFrameType = prPes->u4FrameType;
    #endif
    
    if(!rPesInfo.fgEos)
    {
        ASSERT((rPesInfo.u4VldReadPtr < rPesInfo.u4FifoEnd) &&
            (rPesInfo.u4VldReadPtr >= rPesInfo.u4FifoStart));
    }
    if (_rSwdmxMkvInfo.u4VDecType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
    {
        rPesInfo.fgGop = prPes->fgGop;
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
            break;
        }
    }
    else if(_rSwdmxMkvInfo.u4VDecType == ENUM_SWDMX_VDEC_H264)
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
        //calculate PTS by decoder if CodecID is V_MS/VFW/FOURCC avi type
        if(_rSwdmxMkvInfo.u4MainVidHeaderPtr == 0)
        {
            if(IS_IDR_NALU(rPesInfo.ucPicType))
           {
                rPesInfo.fgDtsValid = TRUE;
            }
            else
            {
                rPesInfo.fgDtsValid = FALSE;
            }
        }
    }
    else if(_rSwdmxMkvInfo.u4VDecType == ENUM_SWDMX_VDEC_MPEG4)
    {
        UCHAR *pucBuf;

        rPesInfo.fgDtsValid = TRUE;
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
    }
    else if (_rSwdmxMkvInfo.u4VDecType == ENUM_SWDMX_VDEC_DX311)
    {
        UCHAR *pucBuf;

        rPesInfo.fgDtsValid = TRUE;
        u4Addr = prPes->u4FrameAddr;
        if(u4Addr >= prPes->u4BufEnd)
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
    else
    {	    	
        rPesInfo.fgDtsValid = TRUE;
        switch(_rSwdmxMkvInfo.u4VDecType)
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

    if((prPes->u4FrameAddr == 0) && (!prPes->fgEOS))
    {
        LOG(3, "MMCallback Addr 0!\n");
    }
    else
    {    
        VDEC_SendEs((void*)&rPesInfo);    
    }
    return TRUE;
}

static INT64 _VsintSubtr(UINT8 u1Byte)
{
    INT64 i8RetVal = 0;
    
    switch(u1Byte)
    {
        case 1:
          i8RetVal = 0x3F;
        break;
        case 2:
          i8RetVal = 0x1FFF;
        break;
        case 3:
          i8RetVal = 0x0FFFFF;
        break;
        case 4:
          i8RetVal = 0x07FFFFFF;
        break;
        case 5:
          i8RetVal = 0x03FFFFFFF;
        break;
        case 6:
          i8RetVal = 0xFFFFFFFF;//0x01FFFFFFFFFF;
        break;
        case 7:
          i8RetVal = 0xFFFFFFFF;//0x00FFFFFFFFFFFFFF;
        break;
        case 8:
          i8RetVal = 0xFFFFFFFF;//0x007FFFFFFFFFFFFF;
        break;
        default:
          i8RetVal = 0;
        break;
    }
    
    return i8RetVal;
}

UINT32 _EbmlGetSz(UINT32 u4ReadData)
{
    UINT32 i, u4Mask;
  
    for (i = 1, u4Mask = 0x80; u4Mask > 0; i++, u4Mask >>= 1)
    {
        if (u4ReadData & u4Mask)
        {
            return i;
        }
    }
    return 0;
}

UINT32 _EbmlGetValueLess4(UCHAR* pucData)
{
    UINT32 i, u4Mask;
    UINT32 u4ReadData = (UINT32)(*pucData);
    UINT32 u4ReadValue;
    UINT32 u4ReturnValue;
  
    LOADB_1DWRD(pucData, u4ReadValue);
  
    for (i = 1, u4Mask = 0x80; u4Mask > 0; i++, u4Mask >>= 1)
    {
        if (u4ReadData & u4Mask)
        {
            break;
        }
    }
  
    switch (i)
    {
        case 1:
          u4ReturnValue = EBML1BYTE(u4ReadValue);
        break;
        case 2:
          u4ReturnValue = EBML2BYTE(u4ReadValue);
          break;
        case 3:
          u4ReturnValue = EBML3BYTE(u4ReadValue);
          break;
        case 4:
          u4ReturnValue = EBML4BYTE(u4ReadValue);
          break;
     #if 0
         case 5:
         case 6:
         case 7:
         case 8:
         case 0:
     #endif
        default:
          // i = 0, return 0
          u4ReturnValue = 0;// MKV_EBML_INVALID; // Not Handle
          break;
    }
  
    return u4ReturnValue;
}

UINT32 _EbmlGetDataValueLess4(UCHAR* pucData, UINT32 u4Size)
{
    UINT32 u4ReadValue;
    UINT32 u4ReturnValue;
  
    LOADB_1DWRD(pucData, u4ReadValue);
  
    switch (u4Size)
    {
      case 1:
        u4ReturnValue = EBML1BYTE(u4ReadValue);
        break;
      case 2:
        u4ReturnValue = EBML2BYTE(u4ReadValue);
        break;
      case 3:
        u4ReturnValue = EBML3BYTE(u4ReadValue);
        break;
      case 4:
        u4ReturnValue = EBML4BYTE(u4ReadValue);
        break;
      default:
        u4ReturnValue = MKV_EBML_INVALID; // Not Handle
        break;
    }
  
    return u4ReturnValue;
}

UINT32 _EbmlGetValueOver4(UCHAR* pucData)
{
    UINT32 u4ReturnValue;
  
    LOADB_1DWRD(pucData, u4ReturnValue);
  
    return u4ReturnValue;
}

void _MkvSetElementRange(UINT32 u4Level, UINT32 u4Sa, UINT32 u4Sz)
{
    if (u4Level < MKV_MAX_LEVEL)
    {
        if((UINT32)u4Sa >= _rFeederMkvInfo.u4EndAddr)
        {
            u4Sa = (u4Sa - _rFeederMkvInfo.u4EndAddr) + _rFeederMkvInfo.u4StartAddr;
            LOG(3, "Chg SetRangeSa 0x%x\n", u4Sa);
        }
        
        _rSwdmxMkvInfo.au4MkvElementSa[u4Level] = u4Sa;
        _rSwdmxMkvInfo.au4MkvElementSz[u4Level] = u4Sz;
        _rSwdmxMkvInfo.au4MkvElementEa[u4Level] = u4Sa + u4Sz;
        if(_rSwdmxMkvInfo.au4MkvElementEa[u4Level] > _rFeederMkvInfo.u4EndAddr)
        {
            _rSwdmxMkvInfo.au4MkvElementEa[u4Level] = 
                (_rSwdmxMkvInfo.au4MkvElementEa[u4Level] - _rFeederMkvInfo.u4EndAddr) + 
                _rFeederMkvInfo.u4StartAddr;
        }

    }
}

static UINT32 _MkvGetElementEa(UINT32 u4Level)
{
    return (u4Level < MKV_MAX_LEVEL) ? _rSwdmxMkvInfo.au4MkvElementEa[u4Level] : 0;
}

static UINT32 _MkvGetElementSz(UINT32 u4Level)
{
    return (u4Level < MKV_MAX_LEVEL) ? _rSwdmxMkvInfo.au4MkvElementSz[u4Level] : 0;
}

static BOOL _IsOutOfElmtRange(UINT32 u4Level, UINT32 u4Addr)
{
    if((UINT32)u4Addr >= _rFeederMkvInfo.u4EndAddr)
    {
        u4Addr = (u4Addr - _rFeederMkvInfo.u4EndAddr) + _rFeederMkvInfo.u4StartAddr;
        LOG(3, "Chg OutRangeCheck 0x%x\n", u4Addr);
    }
        
    if(_rSwdmxMkvInfo.au4MkvElementEa[u4Level] > _rSwdmxMkvInfo.au4MkvElementSa[u4Level])
    {
        if((u4Addr >= _rSwdmxMkvInfo.au4MkvElementSa[u4Level]) &&
            (u4Addr < _rSwdmxMkvInfo.au4MkvElementEa[u4Level]))
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        if((u4Addr >= _rSwdmxMkvInfo.au4MkvElementSa[u4Level]) &&
            (u4Addr <= _rFeederMkvInfo.u4EndAddr))
        {
            return FALSE;
        }
        else if((u4Addr >= (_rFeederMkvInfo.u4StartAddr - RING_BUF_RSV)) &&
            (u4Addr < _rSwdmxMkvInfo.au4MkvElementEa[u4Level]))
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
}

static UINT32 _CalDataSize(UINT32 u4StartAddr, UINT32 u4EndAddr)
{
    if(u4EndAddr > u4StartAddr)
    {
        return (u4EndAddr - u4StartAddr);
    }
    else
    {
        return ((_rFeederMkvInfo.u4EndAddr - u4StartAddr) +
            (u4EndAddr - _rFeederMkvInfo.u4StartAddr));
    }
}

UINT8 _AACSampFreqIdx(UINT32 u4SampFreq)
{
    switch(u4SampFreq)
    {
      case 96000:
        return(0x0);
      case 88200:
        return(0x1);
      case 64000:
        return(0x2);
      case 48000:
        return(0x3);
      case 44100:
        return(0x4);
      case 32000:
        return(0x5);
      case 24000:
        return(0x6);
      case 22050:
        return(0x7);
      case 16000:
        return(0x8);
      case 12000:
        return(0x9);
      case 11025:
        return(0xa);
      case 8000:
        return(0xb);
      case 7350:
        return(0xc);
      default:
        return(0xFF);
    }
}

UINT32 _ADecTypeCast(ASH_AUDIO_FMT_T  e_fmt)
{
    switch(e_fmt)
    {
        case ASH_AUDIO_FMT_MPEG:
            return ENUM_SWDMX_ADEC_MPEG;
        case ASH_AUDIO_FMT_AC3:
            return ENUM_SWDMX_ADEC_AC3;
        case ASH_AUDIO_FMT_PCM:
            return ENUM_SWDMX_ADEC_PCM;
        case ASH_AUDIO_FMT_MP3:
            return ENUM_SWDMX_ADEC_MP3;
        case ASH_AUDIO_FMT_AAC:
            return ENUM_SWDMX_ADEC_AAC;
        case ASH_AUDIO_FMT_DTS:
            return ENUM_SWDMX_ADEC_DTS;
        case ASH_AUDIO_FMT_WMA:
            return ENUM_SWDMX_ADEC_WMA;
        case ASH_AUDIO_FMT_RA:
            return ENUM_SWDMX_ADEC_RA;
        case ASH_AUDIO_FMT_HDCD:
            return ENUM_SWDMX_ADEC_HDCD;
        case ASH_AUDIO_FMT_MLP:
            return ENUM_SWDMX_ADEC_MLP;
        case ASH_AUDIO_FMT_MTS:
            return ENUM_SWDMX_ADEC_MTS;
        default:
            return ENUM_SWDMX_ADEC_UNKNOWN;
    }
}

void _GenAACHdr(UCHAR *pucBuf, UINT32 u4FrameLen)
{
    /* 8b: syncword */
    pucBuf[0] = 0xFF;
    /* 4b: syncword */
    /* 1b: mpeg id = 1 */
    /* 2b: layer = 0 */
    /* 1b: protection absent */
    pucBuf[1] = 0xF9;
    /* 2b: profile */
    /* 4b: sampling_frequency_index */
    /* 1b: private = 0 */
    /* 1b: channel_configuration */
    pucBuf[2] = (UCHAR)((1 << 6) |
               ((_rSwdmxMkvInfo.u4AudSampleRate[_rSwdmxMkvInfo.u4CurAStrmID] << 2) & 0x3C) |
               ((_rSwdmxMkvInfo.u4AudChannelNs[_rSwdmxMkvInfo.u4CurAStrmID] >> 2) & 0x1));
    /* 2b: channel_configuration */
    /* 1b: original */
    /* 1b: home */
    /* 1b: copyright_id */
    /* 1b: copyright_id_start */
    /* 2b: aac_frame_length */
    pucBuf[3] = (UCHAR)(((_rSwdmxMkvInfo.u4AudChannelNs[_rSwdmxMkvInfo.u4CurAStrmID] & 0x3) << 6) |
               ((u4FrameLen >> 11) & 0x3));
    /* 8b: aac_frame_length */
    pucBuf[4] = (UCHAR)(((u4FrameLen >> 3) & 0xFF));
    /* 3b: aac_frame_length */
    /* 5b: adts_buffer_fullness */
    pucBuf[5] = (UCHAR)(((u4FrameLen << 5) & 0xE0) |
               ((0x7FF >> 6) & 0x1F));
    /* 6b: adts_buffer_fullness */
    /* 2b: number_of_raw_data_blocks_in_frame */
    pucBuf[6] = (UCHAR)(((0x7FF << 2) & 0xFC));

    #ifdef CC_MT5395
    HalFlushInvalidateDCacheMultipleLine((UINT32)pucBuf, AAC_HDR_LEN);
    #else
    HalFlushInvalidateDCache();
    #endif
}


BOOL _ChkWMVSC(const UCHAR *pucBuf)
{
    if(((pucBuf[0] == 0) && (pucBuf[1] == 0)) && (pucBuf[2] == 0x1))
    {
        if((pucBuf[3] >= 0xC) && (pucBuf[3] <= 0xF))
        {
            return TRUE;
        }
    }
    return FALSE;
}


UINT8 _ChkAVCFrmType(const UCHAR *pucBuf)
{
    //bit0  : first_mb_in_slice, ue code 1(1 bit)
    //bit1~: slice_type, ue code decoding
    UINT8 u1Byte = (*pucBuf) & 0x7F;
    UINT8 u1ShiftBit = 6, u1BitCnt = 0, u1SliceType = 1;
    UINT8 u1Info = 0;
    
    if(!IS_1ST_SLICE(*pucBuf))    //not first slice in frame
    {
        return AVC_UNKNOWN_FRM;
    }
    while(((u1Byte >> u1ShiftBit) & 0x1) == 0)
    {
        u1BitCnt++;
        u1SliceType *= 2;
        u1ShiftBit--;
        if(u1ShiftBit == 0)
        {
            break;
        }
    }
    u1Info = (UINT8)(((u1Byte << (u1BitCnt + 2)) & 0xFF) >> (8 - u1BitCnt));
    u1SliceType += u1Info;
    u1SliceType -= 1;

    u1SliceType = (u1SliceType >= 5) ? (u1SliceType - 5) : u1SliceType;

    if(u1SliceType > AVC_I_FRM)
    {
        LOG(3, "AVCFrmType err!%d\n", u1SliceType);
    }
    return u1SliceType;    
}


static BOOL _vMkvInfoInit(void)
{
    UINT32 u4Idx;
    
    // Default Speed
    _rSwdmxMkvInfo.i4PrevSpeed = 1000;
    // Default Seek parameter
    _rSwdmxMkvInfo.fgIsSeek = FALSE;

    _rSwdmxMkvInfo.fgBeginToPlay = TRUE;

    _rSwdmxMkvInfo.u4CurRepeatFrameIdx = 0xFFFFFFFF;
    _rSwdmxMkvInfo.u4ModeChangPts = 0xFFFFFFFF;

    _rSwdmxMkvInfo.u8CurClusterPos = 0;

    _rSwdmxMkvInfo.u8SeekPts = 0xFFFFFFFF;
    _rSwdmxMkvInfo.u8SeekPts <<= 32;
    _rSwdmxMkvInfo.u8SeekPts += 0xFFFFFFFF;
	if(pAVADmaAddr==NULL)
    {
        pAVADmaAddr=VIRTUAL((UINT8 *)BSP_AllocAlignedDmaMemory((AVC_HDR_LEN+VC1_HDR_LEN+AAC_HDR_LEN),1024));
		if(pAVADmaAddr==NULL)
		{
          LOG(3, "Failed to allocate AVC,VC1,AAC buffer!\n");
          return FALSE;     
		}
		else
		{
           _rSwdmxMkvInfo.aucAVCHdr=pAVADmaAddr;
		   _rSwdmxMkvInfo.aucVC1Hdr=pAVADmaAddr+AVC_HDR_LEN;
		   _rSwdmxMkvInfo.aucAACHdr=pAVADmaAddr+AVC_HDR_LEN+VC1_HDR_LEN;
		
		}
	}
	else{

          _rSwdmxMkvInfo.aucAVCHdr=pAVADmaAddr;
		  _rSwdmxMkvInfo.aucVC1Hdr=pAVADmaAddr+AVC_HDR_LEN;
		  _rSwdmxMkvInfo.aucAACHdr=pAVADmaAddr+AVC_HDR_LEN+VC1_HDR_LEN;

	}



    if(_rSwdmxMkvInfo.u8MkvSegmentInfoTimeCodeScale == 0)
    {
        _rSwdmxMkvInfo.u8MkvSegmentInfoTimeCodeScale = 90 * TIME_CODE_SCALE;
    }
    _rSwdmxMkvInfo.aucAVCHdr[0] = 0;
    _rSwdmxMkvInfo.aucAVCHdr[1] = 0;
    _rSwdmxMkvInfo.aucAVCHdr[2] = 0;
    _rSwdmxMkvInfo.aucAVCHdr[3] = 1;
    
    _rSwdmxMkvInfo.aucVC1Hdr[0] = 0;    
    _rSwdmxMkvInfo.aucVC1Hdr[1] = 0;    
    _rSwdmxMkvInfo.aucVC1Hdr[2] = 0x01;    
    _rSwdmxMkvInfo.aucVC1Hdr[3] = 0x0d;    

    _rSwdmxMkvInfo.i4BlkDuration = 0;
    for(u4Idx = 0; u4Idx < MKV_MAX_AUD_TRACK; u4Idx++)
    {
        _rSwdmxMkvInfo.u4AudPts[u4Idx] = 0xFFFFFFFF;
    }
    return TRUE;
}


static void _vMkvVidHdrParsing(UINT32 u4VidHdr, UINT32 u4HdrSz)
{
    UCHAR* pucFifo;
    UINT32 u4MkvPSNum, u4PSLen;
	/*#ifdef __KERNEL__
	UCHAR *pucFdrBuf = NULL;
	UCHAR *pucHdrBuf = NULL;
	#endif
    */if(u4VidHdr == 0)
    {
        return;     // no video header
    }

    /*#ifdef __KERNEL__
	if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
        FEEDER_E_OK)
        {
              LOG(7, "Feeder input buffer error.\n");
             return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }
	pucFdrBuf = (UCHAR*)VIRTUAL(_rFeederInfo.u4StartAddr);
    pucFifo = (UCHAR*)u4VidHdr;
    if(VIRTUAL(_rFeederInfo.u4EndAddr) <= ((UINT32)pucFdrBuf + (UINT32)u4HdrSz))
        {
             LOG(7, "Feeder buffer not enough.\n");
             return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

    x_memcpy((VOID*)pucFdrBuf, (VOID*)pucFifo, u4HdrSz);
    u4VidHdr=(UCHAR*)VIRTUAL(_rFeederInfo.u4StartAddr);
	#endif
    *///video header parsing
    pucFifo = (UCHAR*)u4VidHdr;
    if((ENUM_SWDMX_VDEC_T)_rSwdmxMkvInfo.u4VDecType == ENUM_SWDMX_VDEC_H264)
    {
        _rSwdmxMkvInfo.u1MkvAvcNaluLen = (UINT8)((pucFifo[4] & 0x03) + 1);
        pucFifo += 5;
        u4MkvPSNum = (*pucFifo++) & 0x1F; //SPS
        while(u4MkvPSNum > 0)
        {
            u4PSLen = 0;
            LOADB_1WORD(pucFifo, u4PSLen);
            pucFifo += 2;
			_vMKVDeliverData((UINT8)eSWDMX_MEDIA_VIDEO, pucFifo, u4PSLen, TRUE, FALSE);	
            u4MkvPSNum--;
            pucFifo += u4PSLen;
        }
        u4MkvPSNum = *pucFifo++;  //PPS
        while(u4MkvPSNum > 0)
        {
            u4PSLen = 0;
            LOADB_1WORD(pucFifo, u4PSLen);
            pucFifo += 2;
			_vMKVDeliverData((UINT8)eSWDMX_MEDIA_VIDEO, pucFifo, u4PSLen, TRUE, FALSE);   //insert start code
            u4MkvPSNum--;
            pucFifo += u4PSLen;
        }
    }
    else// mpeg4, vc1
    {
        _vMKVDeliverData((UINT8)eSWDMX_MEDIA_VIDEO, pucFifo, u4HdrSz, FALSE, FALSE); 
    }
}


static BOOL _vMKVDeliverData(UINT8 u1HdrType, UCHAR* pucFifo, 
    UINT32 u4DataLen, BOOL bInsertSC, BOOL bIsFdrBuf)  //p transfer
{
    DMX_MM_DATA_T rDmxMMData;
    UINT16 u2CurQNum, u2MaxQNum;
    UINT32 u4FifoAvailSize, u4SliceTypeAddr, u4NaluTypeAddr;
    UINT32 u4QNumThrd = 30; //general fps

    if(u4DataLen == 0)
    {
        return TRUE;
    }
    //check if A/V fifo full
    u4FifoAvailSize = DMX_MM_GetBufEmptySize(u1HdrType);
    if(u4FifoAvailSize < (u4DataLen + 4))
    {
        //LOG(2, "Type %d Fifo Full\n", u1HdrType);
        if(((u1HdrType == (UINT8)eSWDMX_MEDIA_AUDIO) && 
            _rSwdmxMkvInfo.fgIsAudWaitAout) &&
            (!MPV_IsDisplay(ES0)))
        {
            AUD_MMAoutEnable(0, TRUE);
            _rSwdmxMkvInfo.fgIsAudWaitAout = FALSE;
        }
        //timeout, vfifo full and audio data is not parsed yet
        if((u1HdrType == (UINT8)eSWDMX_MEDIA_VIDEO) &&
            _rSwdmxMkvInfo.fgAudTrigVidDec)
        {
            _SWDMX_MkvAudioCb(ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE);
        }        
        x_thread_delay(10);
        return FALSE;
    }
        
    x_memset((void*)&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
    
    rDmxMMData.u4FrameSize = u4DataLen;
    rDmxMMData.u4Pts = _rSwdmxMkvInfo.u4CurBlockPts;
    rDmxMMData.u4StartAddr = (UINT32)(pucFifo);
    //check vfifo Q full
    if(u1HdrType == (UINT8)eSWDMX_MEDIA_VIDEO)
    {
        VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);
        //mkv test
        UNUSED(u2MaxQNum);
        if(_rSwdmxMkvInfo.u4VidFps > 0)
        {
            u4QNumThrd = _rSwdmxMkvInfo.u4VidFps;
        }
        if(_rSwdmxMkvInfo.u4AStrmID >= MKV_MAX_AUD_TRACK)
        {
            LOG(3, "Audio StrmID error!\n");
            return FALSE;
        }
        if(((_rSwdmxMkvInfo.u4AudPts[_rSwdmxMkvInfo.u4AStrmID] < _rSwdmxMkvInfo.u4CurVidPts) &&
            ((_rSwdmxMkvInfo.u4CurVidPts - _rSwdmxMkvInfo.u4AudPts[_rSwdmxMkvInfo.u4AStrmID]) > 45000)) ||
            (_rSwdmxMkvInfo.fgAudTrigVidDec))
        {
            u4QNumThrd = u2MaxQNum>>1; //audio data behind video, don't block data sending here
        }
        
        if(u2CurQNum > (UINT16)u4QNumThrd)
        {
            x_thread_delay(10);
            //LOG(3, "VFifo Full!\n");
            return FALSE;
        }
        
        if(bIsFdrBuf && 
            (_rSwdmxMkvInfo.u4VDecType == ENUM_SWDMX_VDEC_VC1))
        {
            if(!_ChkWMVSC(pucFifo))
            {
                if(!_vMKVDeliverData(u1HdrType, (UCHAR*)_rSwdmxMkvInfo.aucVC1Hdr, VC1_HDR_LEN, FALSE, FALSE))
                {
                    return FALSE;
                }            
            }
        }
        if(((_rSwdmxMkvInfo.u4VDecType == ENUM_SWDMX_VDEC_WMV7) ||
            (_rSwdmxMkvInfo.u4VDecType == ENUM_SWDMX_VDEC_WMV8)) ||
            (_rSwdmxMkvInfo.u4VDecType == ENUM_SWDMX_VDEC_WMV9))
        {
            rDmxMMData.fgFrameHead = TRUE;
        }
        if(((_rSwdmxMkvInfo.i4PrevSpeed > FF_2X) || IsBackward(_rSwdmxMkvInfo.i4PrevSpeed)) &&
            (rDmxMMData.u4Pts > _rSwdmxMkvInfo.u4KeyPTS) &&
            (!_rSwdmxMkvInfo.fgWaitIFrm))
        {
            _rSwdmxMkvInfo.fgKeyDelivered = TRUE;
            return TRUE;
        }
        if(bIsFdrBuf && _rSwdmxMkvInfo.fgWaitIFrm)
        {
            if(_rSwdmxMkvInfo.u4VDecType == ENUM_SWDMX_VDEC_H264)
            {
                u4NaluTypeAddr = rDmxMMData.u4StartAddr;
                //AVI type remove start code
                if(!bInsertSC)
                {
                    _rSwdmxMkvInfo.fgWaitIFrm = FALSE;
                }
                else
                {
                    u4SliceTypeAddr = u4NaluTypeAddr + 1;
                    if(u4SliceTypeAddr == _rFeederMkvInfo.u4EndAddr)
                    {
                        u4SliceTypeAddr = _rFeederMkvInfo.u4StartAddr;
                    }
                    if(IS_1ST_SLICE(*(UINT8*)VIRTUAL(u4SliceTypeAddr)) &&    //first slice in frame
                        (IS_IDR_NALU(*(UINT8*)VIRTUAL(u4NaluTypeAddr)) ||
                        (IS_FRM_NALU(*(UINT8*)VIRTUAL(u4NaluTypeAddr)) &&
                        (_ChkAVCFrmType((UCHAR*)VIRTUAL(u4SliceTypeAddr)) == AVC_I_FRM))))
                    {
                        LOG(5, "WaitI StartPTS 0x%x\n", rDmxMMData.u4Pts);
                        if(!IS_IDR_NALU(*(UINT8*)VIRTUAL(u4NaluTypeAddr)))
                        {
                            LOG(3, "Key!=IDR\n");
                        }
                        _rSwdmxMkvInfo.fgWaitIFrm = FALSE;
                        if(_rSwdmxMkvInfo.u8AudSeekPts < rDmxMMData.u4Pts)
                        {
                            _rSwdmxMkvInfo.u8AudSeekPts = rDmxMMData.u4Pts;
                        }
                        if(!_rSwdmxMkvInfo.fgKeyDelivered)
                        {
                            if(_rSwdmxMkvInfo.u4KeyPTS < rDmxMMData.u4Pts)
                            {
                                _rSwdmxMkvInfo.u4KeyPTS = rDmxMMData.u4Pts;
                                _rSwdmxMkvInfo.u4KeyDeliveredPTS = rDmxMMData.u4Pts;
                            }
                        }
                    }
                    else
                    {
                        return TRUE;
                    }
                }
            }
            else
            {
                _rSwdmxMkvInfo.fgWaitIFrm = FALSE;
            }
        }
    }
    else if(bIsFdrBuf && 
        (_rSwdmxMkvInfo.u4ADecType[_rSwdmxMkvInfo.u4CurAStrmID] == ENUM_SWDMX_ADEC_AAC))
    {
        //insert header
        if(_rSwdmxMkvInfo.fgPendingAudMoveData[_rSwdmxMkvInfo.u4CurAStrmID])
        {
            _GenAACHdr((UCHAR*)_rSwdmxMkvInfo.aucAACHdr, 
                _rSwdmxMkvInfo.rAudDmxMMData[_rSwdmxMkvInfo.u4CurAStrmID].u4FrameSize + AAC_HDR_LEN);
            if(!_vMKVDeliverData(u1HdrType, (UCHAR*)_rSwdmxMkvInfo.aucAACHdr, AAC_HDR_LEN, FALSE, FALSE))
            {
                return FALSE;
            }
        }
    }

    if(bInsertSC)
    {
        if(!_vMKVDeliverData(u1HdrType, (UCHAR*)_rSwdmxMkvInfo.aucAVCHdr, AVC_HDR_LEN, FALSE, FALSE))
        {
            return FALSE;
        }
    }
    //check if data is in feader buffer
    if(!bIsFdrBuf)
    {
        rDmxMMData.u4BufStart = rDmxMMData.u4StartAddr & 0xFFFFFFF0;
        rDmxMMData.u4BufEnd = (((rDmxMMData.u4StartAddr + rDmxMMData.u4FrameSize) + 0xf) & 0xFFFFFFF0) + 192;
    }
    else
    {
        rDmxMMData.u4BufStart = _rFeederMkvInfo.u4StartAddr;
        rDmxMMData.u4BufEnd = _rFeederMkvInfo.u4EndAddr;
        if(rDmxMMData.u4StartAddr < rDmxMMData.u4BufStart)
        {
            // in feeder RING_BUF_RSV region
            rDmxMMData.u4BufStart -= RING_BUF_RSV;
        }
        else if(rDmxMMData.u4StartAddr >= rDmxMMData.u4BufEnd)
        {
            rDmxMMData.u4StartAddr = 
                (rDmxMMData.u4StartAddr - rDmxMMData.u4BufEnd) + rDmxMMData.u4BufStart;
        }
    }

    ASSERT((rDmxMMData.u4StartAddr < rDmxMMData.u4BufEnd) &&
        (rDmxMMData.u4StartAddr >= rDmxMMData.u4BufStart));

    if(u1HdrType == (UINT8)eSWDMX_MEDIA_SUBTITLE)
    {
        _rSwdmxMkvInfo.fgPendingSbMoveData = TRUE;
        x_memcpy((void*)&_rSwdmxMkvInfo.rSbDmxMMData, (void*)&rDmxMMData, sizeof(rDmxMMData));
    }
    else
    {
        if(u1HdrType == (UINT8)eSWDMX_MEDIA_AUDIO)
        {
            if(_rSwdmxMkvInfo.fgPendingAudMoveData[_rSwdmxMkvInfo.u4CurAStrmID])
            {
                if(_rSwdmxMkvInfo.u1FF2xTo1xMode == AUD_PRE_BUF)
                {
                    u1HdrType = _rSwdmxMkvInfo.u4CurAStrmID + MKV_AUD_PREBUF_IDX;
                }
                if(bIsFdrBuf)   //audio data
                {
                    // set PTS by start pts of sending data only for AAC , end pts for other codec
                    if(_rSwdmxMkvInfo.u4ADecType[_rSwdmxMkvInfo.u4CurAStrmID] != ENUM_SWDMX_ADEC_AAC)
                    {
                        _rSwdmxMkvInfo.rAudDmxMMData[_rSwdmxMkvInfo.u4CurAStrmID].u4Pts = rDmxMMData.u4Pts;
                    }
                    if (DMX_MM_MoveData(u1HdrType, &_rSwdmxMkvInfo.rAudDmxMMData[_rSwdmxMkvInfo.u4CurAStrmID], 500) != TRUE)
                    {
                        LOG(3, "Demuxer fails to move data.\n");
                        return FALSE;
                    }
                }
                else    // aac header part
                {
                    rDmxMMData.u4Pts = 0;//invalid PTS, _rSwdmxMkvInfo.rAudDmxMMData.u4Pts;
                    if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
                    {
                        LOG(3, "Demuxer fails to move data.\n");
                        return FALSE;
                    }
                }
            }
            if(bIsFdrBuf)
            {
                x_memcpy((void*)&_rSwdmxMkvInfo.rAudDmxMMData[_rSwdmxMkvInfo.u4CurAStrmID], (void*)&rDmxMMData, sizeof(rDmxMMData));
                _rSwdmxMkvInfo.fgPendingAudMoveData[_rSwdmxMkvInfo.u4CurAStrmID] = TRUE;
            }
        }
        else if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
        {
            LOG(3, "Demuxer fails to move data.\n");
            return FALSE;
        }
    }
    return TRUE;
}

static BOOL _vMKVBlockParsing(UINT8* pucBuf, UINT32 u4BlockSz, UINT32* pu4ConsumedSz)
{
    UINT8* pucFifo = pucBuf;//(BYTE *) _pbHdrBuf;
    UINT8* pucSz = NULL;
    UINT32 u4VintIDSz = _EbmlGetSz((UINT32)*pucFifo);
    UINT32 u4HeaderSz = u4VintIDSz + 3; // timecode: 2 bytes and flag: 1 byte
    UINT32 u4MkvTrackNo, u4CurDispPts, u4Idx, u4APts;
    INT32 i4BlockTimeCode;
    INT16 i2MkvTimeCode;
    UINT8 u1MkvFlag;
    BOOL fgSkipBlock = TRUE;
    UINT64 u8Remainder;
    INT32 i4Subtraction;
    INT32 i4StcNow;

    u4MkvTrackNo = _EbmlGetValueLess4(pucFifo);

    LOADB_1WORD((pucFifo + u4VintIDSz), i2MkvTimeCode);
    LOAD_1BYTE((pucFifo + u4VintIDSz + 2), u1MkvFlag);

    i4BlockTimeCode = (INT32)i2MkvTimeCode + (INT32)_rSwdmxMkvInfo.u4ClusterTimeCode;

    // get block PTS
    #if 0
    if(_rSwdmxMkvInfo.u4KeyFrmPTS > 0)  //trick mode
    {
        _rSwdmxMkvInfo.u4CurBlockPts = _rSwdmxMkvInfo.u4KeyFrmPTS;
    }
    else
    #endif
    {
        _rSwdmxMkvInfo.u4CurBlockPts =
            (UINT32)u8Div6432(((UINT64)i4BlockTimeCode * 
            _rSwdmxMkvInfo.u8MkvSegmentInfoTimeCodeScale), TIME_CODE_SCALE, &u8Remainder);
    }
   
   if((!_rSwdmxMkvInfo.fgEnVideo)&& (u4MkvTrackNo < MKV_MAX_AUD_TRACK) &&     //is audio track and format support
           _rSwdmxMkvInfo.fgIsAudTrack[u4MkvTrackNo])
       { 
           i4StcNow = STC_GetStcValue();
           i4Subtraction = (INT32)(_rSwdmxMkvInfo.u4CurBlockPts) - i4StcNow;
           if(i4Subtraction > 450000)
           {
               return FALSE;
   
           }
       
       }   
    // ignore block until seek done 
    if(_rSwdmxMkvInfo.fgIsSeek)
    {
        if((_rSwdmxMkvInfo.u4CurBlockPts < (UINT32)_rSwdmxMkvInfo.u8SeekPts) &&
            (u4MkvTrackNo == _rSwdmxMkvInfo.u4VStrmID))
        {
            u4MkvTrackNo = SKIP_STRM;
        }
        else if((u4MkvTrackNo == _rSwdmxMkvInfo.u4VStrmID) || (!_rSwdmxMkvInfo.fgEnVideo))
        {
            LOG(5, "Seek done, BlockPTS 0x%x, Type (%d, A%d, V%d)\n", 
                _rSwdmxMkvInfo.u4CurBlockPts, u4MkvTrackNo, 
                _rSwdmxMkvInfo.u4AStrmID,
                _rSwdmxMkvInfo.u4VStrmID);
            _rSwdmxMkvInfo.u8SeekPts = 0xFFFFFFFF;
            _rSwdmxMkvInfo.u8SeekPts <<= 32;
            _rSwdmxMkvInfo.u8SeekPts += 0xFFFFFFFF;
            _rSwdmxMkvInfo.fgIsSeek = FALSE;
        }
    }
    // set audio/video stream
    if ((u4MkvTrackNo == _rSwdmxMkvInfo.u4VStrmID)/* && _rSwdmxMkvInfo.fgEnVideo*/)
    {
        if(_rSwdmxMkvInfo.fgEnVideo)
        {
            _rSwdmxMkvInfo.u1StrmType = (UINT8)eSWDMX_MEDIA_VIDEO;
            _rSwdmxMkvInfo.u4CurVidPts = _rSwdmxMkvInfo.u4CurBlockPts;
            fgSkipBlock = FALSE;
        }
    }
    else if ((u4MkvTrackNo < MKV_MAX_AUD_TRACK) &&     //is audio track and format support
        _rSwdmxMkvInfo.fgIsAudTrack[u4MkvTrackNo])
    {
        ASSERT(u4MkvTrackNo < MKV_MAX_AUD_TRACK);    
        if((u4MkvTrackNo == _rSwdmxMkvInfo.u4AStrmID) &&
            _rSwdmxMkvInfo.fgEnAudio)                                             //is current audio track
        {
            if((_rSwdmxMkvInfo.u4CurBlockPts >= (UINT32)_rSwdmxMkvInfo.u8AudSeekPts) &&
                (!_rSwdmxMkvInfo.fgWaitIFrm))   //the flag will be true if key!=I frame
            {
                if(_rSwdmxMkvInfo.fgChgATrack
                    || (_rSwdmxMkvInfo.u8AudSeekPts > 0)
#ifdef AUD_PREBUF_EN
                    || ((_rSwdmxMkvInfo.u1FF2xTo1xMode == AUD_PRE_BUF) && fgActivePidxIsUnKnown)
#endif
                )
                {
                    u4APts = _rSwdmxMkvInfo.u4CurBlockPts;
                    if(_rSwdmxMkvInfo.fgEnVideo)
                    {
                        u4CurDispPts = VDP_GetPts(0);   
                    }
                    else
                    {
                        u4CurDispPts =  STC_GetStcValue();   
                    }
                    #ifdef AUD_PREBUF_EN
                    if(_rSwdmxMkvInfo.u1FF2xTo1xMode == AUD_PRE_BUF)
                    {
                        if(_rSwdmxMkvInfo.fgChgATrack)
                        {
                            _rSwdmxMkvInfo.u8AudSeekPts = u4CurDispPts + 45000;
                        }
                        if((_rSwdmxMkvInfo.i4LastSpeed == FF_2X) || _rSwdmxMkvInfo.fgChgATrack)
                        {
                            if(!DMX_AudHandler_SeekAndUpdate(u4MkvTrackNo + MKV_AUD_PREBUF_IDX,
                            _rSwdmxMkvInfo.u8AudSeekPts, &u4APts))
                            {
                                LOG(2, "Mkv DMX_AudHandler_SeekAndUpdate error!\n");
                                u4APts = _rSwdmxMkvInfo.u4CurBlockPts;
                            }
                            _rSwdmxMkvInfo.i4LastSpeed = 0xFF;
                        }
                        AUD_SetStartPts(0, u4APts);
                        DMX_AudHandler_SetActivePidx(u4MkvTrackNo + MKV_AUD_PREBUF_IDX);
                        fgActivePidxIsUnKnown = FALSE;
                    }
                    #endif
                    
                    LOG(3, "ChgAtrk %d, AudStartPts 0x%x, CurDisp 0x%x, Diff %ds\n", u4MkvTrackNo,
                        u4APts, u4CurDispPts, 
                        (u4APts - u4CurDispPts) / 90000);
                    UNUSED(u4CurDispPts);
                    AUD_SetStartPts(0, u4APts);
                    _rSwdmxMkvInfo.fgChgATrack = FALSE;
                    _rSwdmxMkvInfo.u8AudSeekPts = 0;
                    _rSwdmxMkvInfo.u4AudTrigVidDecPTS = u4APts;
                    if(!_rSwdmxMkvInfo.fgAudTrigVidDec)
                    {
                        VDP_TriggerAudReceive(u4APts);
                    }
                }
                else if(_rSwdmxMkvInfo.u4AudPts[u4MkvTrackNo] == 0xFFFFFFFF)    //for begin playing
                {
                    AUD_SetStartPts(0, _rSwdmxMkvInfo.u4CurBlockPts);
                }
                _rSwdmxMkvInfo.u1StrmType = (UINT8)eSWDMX_MEDIA_AUDIO;
                _rSwdmxMkvInfo.u4CurAStrmID = u4MkvTrackNo;
                fgSkipBlock = FALSE;
            }
        }
        else if((_rSwdmxMkvInfo.u1FF2xTo1xMode == AUD_PRE_BUF) &&             //for pre-buffer
            ((_rSwdmxMkvInfo.i4PrevSpeed == FF_2X) ||
            (_rSwdmxMkvInfo.i4PrevSpeed == NORMAL_1X)))
        {
            _rSwdmxMkvInfo.u1StrmType = (UINT8)eSWDMX_MEDIA_AUDIO;
            _rSwdmxMkvInfo.u4CurAStrmID = u4MkvTrackNo;
            fgSkipBlock = FALSE;
        }

        if(_rSwdmxMkvInfo.u4CurAStrmID == _rSwdmxMkvInfo.u4AStrmID)
        {
            _rSwdmxMkvInfo.u4AudPts[u4MkvTrackNo] = _rSwdmxMkvInfo.u4CurBlockPts;
        }
        else
        {
            _rSwdmxMkvInfo.u4AudPts[u4MkvTrackNo] = 0;
        }
    }
    else if ((u4MkvTrackNo == _rSwdmxMkvInfo.u4SpStrmID) && _rSwdmxMkvInfo.fgEnSubTitle)
    {
        if(_rSwdmxMkvInfo.u4CurBlockPts >= (UINT32)_rSwdmxMkvInfo.u8AudSeekPts)
        {
            _rSwdmxMkvInfo.u1StrmType = (UINT8)eSWDMX_MEDIA_SUBTITLE;
            fgSkipBlock = FALSE;
        }
    }
    
    if(fgSkipBlock)
    {
        _rSwdmxMkvInfo.u1StrmType = 0xFF;
        _rSwdmxMkvInfo.ePrsState = MKVPRS_G_SEGMENT_CLUSTER_BG_ELEMENT_ID;
        _rSwdmxMkvInfo.fgDrmEnable = FALSE;
        *pu4ConsumedSz += u4BlockSz;
        return TRUE;
    }
    else if(_rSwdmxMkvInfo.prDmxRangeInfo->b_drm)
    {
        if(!_SWDMXMkvDecDrmData(_rSwdmxMkvInfo.u1StrmType, pucBuf + u4HeaderSz, u4BlockSz))
        {
            LOG(2, "DecDrmData err!\n");
        }
        _rSwdmxMkvInfo.fgDrmEnable = FALSE; //for video 
    }

    _rSwdmxMkvInfo.u4MkvBHeaderSa = (UINT32)pucBuf;//_dMkvFileOfst;
    _rSwdmxMkvInfo.u4MkvBHeaderSz = u4HeaderSz;

    if (fgMkvIsNoLacing(u1MkvFlag))
    {
        //mkv test
        if ((_rSwdmxMkvInfo.u1StrmType == (UINT8)eSWDMX_MEDIA_VIDEO) && 
            ((_rSwdmxMkvInfo.u4VDecType == ENUM_SWDMX_VDEC_H264) && 
            _rSwdmxMkvInfo.u1MkvAvcNaluLen > 0))    // video type V_MPEG4/ISO/AVC
        {
            _rSwdmxMkvInfo.ePrsState = MKVPRS_G_DATA_AVC;
            *pu4ConsumedSz += _rSwdmxMkvInfo.u4MkvBHeaderSz;
        }
        else
        {
            //_rSwdmxMkvInfo.ePrsState = MKVPRS_G_DATA_NO_LACING; // ToDo: Change
            if(_vMKVDeliverData(_rSwdmxMkvInfo.u1StrmType,
                pucBuf + u4HeaderSz, 
                u4BlockSz - u4HeaderSz,
                FALSE, TRUE))
            {
                *pu4ConsumedSz += u4BlockSz;
                _rSwdmxMkvInfo.ePrsState = MKVPRS_G_SEGMENT_CLUSTER_BG_ELEMENT_ID;
            }
            else
            {
                return FALSE;
            }
        }
    }
    else
    {
        LOAD_1BYTE((pucFifo + u4VintIDSz + 3), _rSwdmxMkvInfo.u4MkvFrameCount);
        _rSwdmxMkvInfo.u4MkvFrameCount++; // From Spec, real frame count is one more
        _rSwdmxMkvInfo.u4MkvBHeaderSz++; // size of _dMkvFrameCount

        _rSwdmxMkvInfo.u4MkvFrmCntPrs = 0;
        _rSwdmxMkvInfo.u4MkvFrmAccSz = 0;
        x_memset((void*)_rSwdmxMkvInfo.au4MkvFrmCntPrsSz, 0,
            sizeof(_rSwdmxMkvInfo.au4MkvFrmCntPrsSz));

        if (fgMkvIsXiphLacing(u1MkvFlag))
        {
            _rSwdmxMkvInfo.ePrsState = MKVPRS_G_DATA_XIPH_LACING_HDR;
            *pu4ConsumedSz += _rSwdmxMkvInfo.u4MkvBHeaderSz;
        }
        else if (fgMkvIsFixedSizeLacing(u1MkvFlag))
        {
            //_rSwdmxMkvInfo.ePrsState = MKVPRS_G_DATA_FIXED_LACING;
            _rSwdmxMkvInfo.au4MkvFrmCntPrsSz[0] = 
                (u4BlockSz - _rSwdmxMkvInfo.u4MkvBHeaderSz)/_rSwdmxMkvInfo.u4MkvFrameCount;
            pucFifo += _rSwdmxMkvInfo.u4MkvBHeaderSz;
            if(DMX_MM_GetBufEmptySize(_rSwdmxMkvInfo.u1StrmType) < u4BlockSz)
            {
                  if((_rSwdmxMkvInfo.u1StrmType ==(UINT8)eSWDMX_MEDIA_AUDIO)&&
				  	( _rSwdmxMkvInfo.fgIsAudWaitAout) &&
                       (!MPV_IsDisplay(ES0)))
                   {
                         AUD_MMAoutEnable(0, TRUE);
                       _rSwdmxMkvInfo.fgIsAudWaitAout = FALSE;
                     }
				return FALSE;
            }
            //if AAC, each frame have to insert hdr in DeliverData
            for(u4Idx = 0; u4Idx<_rSwdmxMkvInfo.u4MkvFrameCount; u4Idx++)
            {
                if(!_vMKVDeliverData(_rSwdmxMkvInfo.u1StrmType,
                    pucFifo, 
                    _rSwdmxMkvInfo.au4MkvFrmCntPrsSz[0],
                    FALSE, TRUE))
                {
                    x_thread_delay(1);
                    u4Idx--;    //should send all frames in one block at the same time
                }
                else
                {
                    pucFifo += _rSwdmxMkvInfo.au4MkvFrmCntPrsSz[0];
                }
            }
            *pu4ConsumedSz += u4BlockSz;
            _rSwdmxMkvInfo.ePrsState = MKVPRS_G_SEGMENT_CLUSTER_BG_ELEMENT_ID;
        }
        else if (fgMkvIsEBMLLacing(u1MkvFlag))
        {
            _rSwdmxMkvInfo.ePrsState = MKVPRS_G_DATA_EBML_LACING_HDR;
            *pu4ConsumedSz += _rSwdmxMkvInfo.u4MkvBHeaderSz;

            pucSz = pucFifo + _rSwdmxMkvInfo.u4MkvBHeaderSz;
            _rSwdmxMkvInfo.au4MkvFrmCntPrsSz[0] = 
                _EbmlGetValueLess4(pucSz);
            _rSwdmxMkvInfo.u4MkvFrmAccSz = _rSwdmxMkvInfo.au4MkvFrmCntPrsSz[0];
            *pu4ConsumedSz += _EbmlGetSz((UINT32)*pucSz);
        }
    }
    return TRUE;
}

    
static BOOL _vMKVClusterParsing(
        UCHAR* pucBuf,
        SWDMX_RANGE_LIST_T* prRangeList,
        UINT32 u4AvalSz,
        UINT32* pu4ConsumedSz)
{
    UCHAR* pucFifo = pucBuf;
    UINT32 u4VintIDSz = 0, u4HdrSz = 0, u4VintSzLen = 0, u4VintDataSz = 0;
    INT64 i8Val = 0;
    UINT32 u4ValueLess4 = 0, u4ElmtSz = 0, u4Value = 0;
    UINT32 u4Len = 0, u4NaluLen4, u4Level;
    UINT16 u2NaluLen2, u2Val;
    UINT8 u1RsvLen, u1Val;
    INT8 i1Val;
    INT16 i2Val;
    UINT64 u8Remainder;

    *pu4ConsumedSz = 0;
    while((u4AvalSz > (*pu4ConsumedSz + MKV_UNCERTAIN_SZ)) || 
        (_rSwdmxMkvInfo.fgReadEOS && (u4AvalSz > (*pu4ConsumedSz))))
    {
        //already find key frame in trick mode
        if(((_rSwdmxMkvInfo.i4PrevSpeed > FF_2X) || IsBackward(_rSwdmxMkvInfo.i4PrevSpeed)))
        {
            if(_rSwdmxMkvInfo.fgKeyDelivered && !_rSwdmxMkvInfo.fgIsTPT)
            {
                break;
            }
        }
        pucFifo = pucBuf + *pu4ConsumedSz;
    
        //check ring buffer, last data block transfer
        if((UINT32)pucFifo >= _rFeederMkvInfo.u4EndAddr)
        {
            pucFifo = (pucFifo - _rFeederMkvInfo.u4EndAddr) + _rFeederMkvInfo.u4StartAddr;
        }
        else if((UINT32)pucFifo >= (_rFeederMkvInfo.u4EndAddr - MKV_UNCERTAIN_SZ))
        {
            u1RsvLen = (UINT8)(_rFeederMkvInfo.u4EndAddr - (UINT32)pucFifo);
            x_memcpy((void*)(_rFeederMkvInfo.u4StartAddr - u1RsvLen), (void*)pucFifo, u1RsvLen);
            #ifdef CC_MT5395
            HalFlushInvalidateDCacheMultipleLine((_rFeederMkvInfo.u4StartAddr - (UINT32)u1RsvLen), (UINT32)u1RsvLen);
            #else            
            HalFlushInvalidateDCache();
            #endif
            pucFifo = (UINT8*)(_rFeederMkvInfo.u4StartAddr - u1RsvLen);

            LOG(3, "Use rsv feederBuf len 0x%x\n", u1RsvLen);
            //handle reserve length with level element address 
            for(u4Level = 0; u4Level < MKV_MAX_LEVEL; u4Level++)
            {
                if(_rSwdmxMkvInfo.au4MkvElementEa[u4Level] > 
                    (_rFeederMkvInfo.u4EndAddr - MKV_UNCERTAIN_SZ))
                {
                    _rSwdmxMkvInfo.au4MkvElementEa[u4Level] = 
                        _rFeederMkvInfo.u4StartAddr - 
                        (_rFeederMkvInfo.u4EndAddr - _rSwdmxMkvInfo.au4MkvElementEa[u4Level]);
                }
            }
        }

        //embl parsing
        u4VintIDSz = _EbmlGetSz((UINT32)*pucFifo);
        u4VintSzLen = _EbmlGetSz((UINT32)*(pucFifo + u4VintIDSz));
        u4VintDataSz = (u4VintSzLen <= 4) ? _EbmlGetValueLess4(pucFifo + u4VintIDSz) : 
            _EbmlGetValueOver4(pucFifo + u4VintIDSz + 4);
        u4HdrSz = u4VintIDSz + u4VintSzLen;
        u4ValueLess4 = _EbmlGetDataValueLess4(pucFifo + u4HdrSz, u4VintDataSz);
        u4ElmtSz = u4HdrSz + u4VintDataSz;
        UNUSED(u4ValueLess4);

        //LOG(4, "State 0x%x\n", _rSwdmxMkvInfo.ePrsState);
        int i;
		int j;
		int i1;
        switch(_rSwdmxMkvInfo.ePrsState)
        {
            case MKVPRS_G_SEGMENT_ELEMENT_ID: // P.12~13 Table 2
                if (fgMkvIsSegmentClusterID(pucFifo))
                {
                    _rSwdmxMkvInfo.ePrsState = MKVPRS_G_SEGMENT_CLUSTER_ELEMENT_ID;
                    _rSwdmxMkvInfo.u8CurClusterPos = _rSwdmxMkvInfo.u8CurDmxMoviPos+*pu4ConsumedSz;
					for(i=0;i<10;i++)
					{
                      if(_rSwdmxMkvInfo.au8PostionxTime[i][0]==0)
                      	{_rSwdmxMkvInfo.au8PostionxTime[i][0]=_rSwdmxMkvInfo.u8CurClusterPos;
                        break;
					  }
					}
					if(i==10)
					{  
                       for(j=0;j<9;j++)
                       	{
                       	  _rSwdmxMkvInfo.au8PostionxTime[j][0]=_rSwdmxMkvInfo.au8PostionxTime[j+1][0];
						  _rSwdmxMkvInfo.au8PostionxTime[j][1]=_rSwdmxMkvInfo.au8PostionxTime[j+1][1];
                       	}
					   _rSwdmxMkvInfo.au8PostionxTime[9][0]=_rSwdmxMkvInfo.u8CurClusterPos;
					}
					prRangeList->prDmxRangeInfo->t_start_pos.ui8_frame_position=_rSwdmxMkvInfo.u8CurDmxMoviPos+*pu4ConsumedSz;
                    *pu4ConsumedSz += u4HdrSz;
                }
                else if((UINT32)pucFifo >= _MkvGetElementEa(0))
                {
                    LOG(3, "File end?\n");
                    //error handling
                    if(u4ElmtSz == 0)
                    {
                        *pu4ConsumedSz += u4AvalSz;
                    }
                    else
                    {
                        *pu4ConsumedSz += u4ElmtSz;
                    }
                    return TRUE;
                }
                break;
            // 5.5 Segment.Cluster
            case MKVPRS_G_SEGMENT_CLUSTER_ELEMENT_ID:
                if (fgMkvIsCRC32(pucFifo))
                {
                    *pu4ConsumedSz += u4ElmtSz;
                }
                else if (fgMkvIsClusterTimeCodeID(pucFifo))
                {
                    u4VintSzLen = _EbmlGetSz((UINT32)*(pucFifo + 1));
                    u4VintDataSz = (u4VintSzLen <= 4) ? _EbmlGetValueLess4(pucFifo + 1) : _EbmlGetValueOver4(pucFifo + 1 + 4);
                    switch (u4VintDataSz)
                    {
                        case 1:
                            LOAD_1BYTE(pucFifo + 1 + u4VintSzLen, u1Val);
                            _rSwdmxMkvInfo.u4ClusterTimeCode = u1Val;
                            break;
                        case 2:
                            LOADB_1WORD(pucFifo + 1 + u4VintSzLen, u2Val);
                            _rSwdmxMkvInfo.u4ClusterTimeCode = u2Val;
                            break;
                        case 3:
                            LOADB_1DWRD(pucFifo + 1 + u4VintSzLen, _rSwdmxMkvInfo.u4ClusterTimeCode);
                            _rSwdmxMkvInfo.u4ClusterTimeCode >>= 8;
                            break;
                        case 4:
                            LOADB_1DWRD(pucFifo + 1 + u4VintSzLen, _rSwdmxMkvInfo.u4ClusterTimeCode);
                            break;
                        default:    // length > 4
                            LOG(3, "Cluster timecode length>4\n");
                            _rSwdmxMkvInfo.u4ClusterTimeCode = 0;
                            break;
                    }
					
					for(i1=1;i1<10;i1++)
						{
                              if(_rSwdmxMkvInfo.au8PostionxTime[i1][0]==0)
                              	{
                                  _rSwdmxMkvInfo.au8PostionxTime[i1-1][1]=_rSwdmxMkvInfo.u4ClusterTimeCode * 90;
								  break;
							    }
						}
					if(i1==10)
						{
                          
                           _rSwdmxMkvInfo.au8PostionxTime[9][1]=_rSwdmxMkvInfo.u4ClusterTimeCode * 90;
					    }
                    LOG(3, "cluster time code 0x%x\n", 
                        (UINT32)u8Div6432(
                        ((UINT64)_rSwdmxMkvInfo.u4ClusterTimeCode * _rSwdmxMkvInfo.u8MkvSegmentInfoTimeCodeScale),
                        TIME_CODE_SCALE, &u8Remainder));
                    *pu4ConsumedSz += u4ElmtSz;
                }
                else if (fgMkvIsClusterPositionID(pucFifo))
                {
                    *pu4ConsumedSz += u4ElmtSz;
                }
                else if (fgMkvIsClusterPrevSizeID(pucFifo))
                {
                    *pu4ConsumedSz += u4ElmtSz;
                }
                else if (fgMkvIsClusterBlockGroupID(pucFifo))
                {
                    _rSwdmxMkvInfo.ePrsState = MKVPRS_G_SEGMENT_CLUSTER_BG_ELEMENT_ID;
                    _MkvSetElementRange(2, (UINT32)pucFifo, u4ElmtSz);
                    *pu4ConsumedSz += u4HdrSz;
                }
                else if (fgMkvIsClusterSimpleBlockID(pucFifo))
                {
                    _rSwdmxMkvInfo.ePrsState = MKVPRS_G_BLOCK_DATA;
                    *pu4ConsumedSz += u4HdrSz;
                    _MkvSetElementRange(2, (UINT32)pucFifo + u4HdrSz, u4VintDataSz);
                    _MkvSetElementRange(4, (UINT32)pucFifo + u4HdrSz, u4VintDataSz);
                }
                else if((UINT32)pucFifo >= _MkvGetElementEa(1))
                {
                    _rSwdmxMkvInfo.ePrsState = MKVPRS_G_SEGMENT_ELEMENT_ID;
                }
                else
                {
                    //error handling
                    if(u4ElmtSz == 0)
                    {
                        *pu4ConsumedSz = u4AvalSz;
                        return TRUE;
                    }
                    else
                    {
                        *pu4ConsumedSz += u4ElmtSz;
                    }
                }
                break;
    
            case MKVPRS_G_SEGMENT_CLUSTER_BG_ELEMENT_ID:
                _MkvSetElementRange(3, (UINT32)pucFifo, u4ElmtSz);
                if (fgMkvIsBGBlockID(pucFifo))
                {
                    _rSwdmxMkvInfo.ePrsState = MKVPRS_G_BLOCK_DATA;
                    *pu4ConsumedSz += u4HdrSz;
                    _MkvSetElementRange(4, (UINT32)pucFifo + u4HdrSz, u4VintDataSz);
                    _rSwdmxMkvInfo.u4KeyFrmSz = u4VintDataSz + u4HdrSz;
                }
                else if (fgMkvIsBGReferenceBlockID(pucFifo))
                {
                    *pu4ConsumedSz += u4ElmtSz;
                }
                else if (fgMkvIsBGBlockDurationID(pucFifo))
                {
                    u4VintSzLen = _EbmlGetSz((UINT32)*(pucFifo + 1));
                    u4VintDataSz = (u4VintSzLen <= 4) ? _EbmlGetValueLess4(pucFifo + 1) : _EbmlGetValueOver4(pucFifo + 1 + 4);
                    switch (u4VintDataSz)
                    {
                        case 1:
                            LOAD_1BYTE(pucFifo + 1 + u4VintSzLen, i1Val);
                            _rSwdmxMkvInfo.i4BlkDuration = (INT32)i1Val;
                            break;
                        case 2:
                            LOADB_1WORD(pucFifo + 1 + u4VintSzLen, i2Val);
                            _rSwdmxMkvInfo.i4BlkDuration = (INT32)i2Val;
                            break;
                        case 3:
                            LOADB_1DWRD(pucFifo + 1 + u4VintSzLen, _rSwdmxMkvInfo.i4BlkDuration);
                            _rSwdmxMkvInfo.i4BlkDuration >>= 8;
                            break;
                        case 4:
                            LOADB_1DWRD(pucFifo + 1 + u4VintSzLen, _rSwdmxMkvInfo.i4BlkDuration);
                            break;
                        default:    // length > 4
                            LOG(3, "BlockDuration length>4\n");
                            _rSwdmxMkvInfo.i4BlkDuration = 0;
                            break;
                    }
                    _rSwdmxMkvInfo.i4BlkDuration = 
                        (INT32)u8Div6432(((UINT64)_rSwdmxMkvInfo.i4BlkDuration * _rSwdmxMkvInfo.u8MkvSegmentInfoTimeCodeScale) 
                        , TIME_CODE_SCALE, &u8Remainder);
                    *pu4ConsumedSz += u4ElmtSz;
                }
                else if (fgMkvIsBGDrmBlockID(pucFifo))
                {
                    if (u4ElmtSz > u4HdrSz)
                    {
                        UNUSED(_SWDMXMkvPrsDrmInfo((pucFifo+u4HdrSz), (u4ElmtSz-u4HdrSz)));
                    }

                    *pu4ConsumedSz += u4ElmtSz;
                }
                else if (_IsOutOfElmtRange(2, (UINT32)pucFifo))
                {   // Return to Parent
                    _rSwdmxMkvInfo.ePrsState = MKVPRS_G_SEGMENT_CLUSTER_ELEMENT_ID;

                    // for subtitle, block duration may be parsed after block data
                    if(_rSwdmxMkvInfo.fgPendingSbMoveData)
                    {
                        _rSwdmxMkvInfo.rSbDmxMMData.u4Dts = (UINT32)_rSwdmxMkvInfo.i4BlkDuration;
                        if (DMX_MM_MoveData((UINT8)eSWDMX_MEDIA_SUBTITLE, 
                                &_rSwdmxMkvInfo.rSbDmxMMData, 500) != TRUE)
                        {
                            LOG(3, "Demuxer fails to move data.\n");
                        }
                        _rSwdmxMkvInfo.fgPendingSbMoveData = FALSE;
                    }
                }
                else // Unknow ID, skip this element
                {
                    //error handling
                    if(u4ElmtSz == 0)
                    {
                        *pu4ConsumedSz = u4AvalSz;
                        return TRUE;
                    }
                    else
                    {
                        *pu4ConsumedSz += u4ElmtSz;
                    }
                }
                break;
    
            case MKVPRS_G_BLOCK_DATA:
                if((u4AvalSz < ((*pu4ConsumedSz/* + MKV_UNCERTAIN_SZ*/) + _MkvGetElementSz(4)))/* &&
                    !_rSwdmxMkvInfo.fgReadEOS*/)
                {
                    _rSwdmxMkvInfo.fgIsUnderFlw = TRUE;
                    LOG(6, "Data !enough\n");
                    return TRUE;
                }
                if(!_vMKVBlockParsing(pucFifo, _MkvGetElementSz(4), pu4ConsumedSz))
                {
                    return TRUE;    
                }
                
                break;
    
            case MKVPRS_G_DATA_AVC:
                if (_rSwdmxMkvInfo.u1MkvAvcNaluLen == 4)
                {
                    LOADB_1DWRD(pucFifo, u4NaluLen4);
                    u4Len = u4NaluLen4;
                }
                else if (_rSwdmxMkvInfo.u1MkvAvcNaluLen == 2)
                {
                    LOADB_1WORD(pucFifo, u2NaluLen2);
                    u4Len = u2NaluLen2;
                }
                else
                {
                    LOG(3, "AVCNaluLen %d error\n", _rSwdmxMkvInfo.u1MkvAvcNaluLen);
                }
    
                if (_IsOutOfElmtRange(4, (UINT32)pucFifo))
                { // Return to Parent
                    _rSwdmxMkvInfo.ePrsState = MKVPRS_G_SEGMENT_CLUSTER_BG_ELEMENT_ID;
                }
                else
                //transfer data to A/V fifo by hw dmx
                {
                    if((u4AvalSz < (*pu4ConsumedSz + (_rSwdmxMkvInfo.u1MkvAvcNaluLen + u4Len))) &&
                        !_rSwdmxMkvInfo.fgReadEOS)
                    {
                        _rSwdmxMkvInfo.fgIsUnderFlw = TRUE;
                        LOG(3, "Data !enough\n");
                        return TRUE;
                    }
                    if(_vMKVDeliverData((UINT8)eSWDMX_MEDIA_VIDEO,
                        pucFifo + _rSwdmxMkvInfo.u1MkvAvcNaluLen,
                        u4Len,
                        TRUE,
                        TRUE))  //mkv test
                    {
                        *pu4ConsumedSz += (_rSwdmxMkvInfo.u1MkvAvcNaluLen + u4Len);
                    }
                    else
                    {
                        return TRUE;
                    }
                }
                break;
    
            case MKVPRS_G_DATA_XIPH_LACING_HDR:
                if (_rSwdmxMkvInfo.u4MkvFrmCntPrs < 
                    (_rSwdmxMkvInfo.u4MkvFrameCount - 1))
                {
                    do{
                        LOAD_1BYTE(pucFifo, u4Value);
                        *pu4ConsumedSz += 1;
                        pucFifo += 1;
                        _rSwdmxMkvInfo.au4MkvFrmCntPrsSz[_rSwdmxMkvInfo.u4MkvFrmCntPrs] += u4Value;
                    }while (u4Value == 0xFF);
                    
                    _rSwdmxMkvInfo.u4MkvFrmAccSz += 
                        _rSwdmxMkvInfo.au4MkvFrmCntPrsSz[_rSwdmxMkvInfo.u4MkvFrmCntPrs] ;                    
                    _rSwdmxMkvInfo.u4MkvFrmCntPrs++;
                }
                else if(_rSwdmxMkvInfo.u4MkvFrmCntPrs == 
                    (_rSwdmxMkvInfo.u4MkvFrameCount - 1)) //last frame
                {
                    _rSwdmxMkvInfo.au4MkvFrmCntPrsSz[_rSwdmxMkvInfo.u4MkvFrmCntPrs] =
                        _CalDataSize((UINT32)pucFifo, _MkvGetElementEa(4)) - 
                        _rSwdmxMkvInfo.u4MkvFrmAccSz;

                    _rSwdmxMkvInfo.ePrsState = MKVPRS_G_DATA_LACING;
                    _rSwdmxMkvInfo.u4MkvFrmCntPrs = 0;
                    _rSwdmxMkvInfo.u4MkvFrmAccSz = 0;
                }
                break;
    
            case MKVPRS_G_DATA_EBML_LACING_HDR:
                _rSwdmxMkvInfo.u4MkvFrmCntPrs++;
                if(_rSwdmxMkvInfo.u4MkvFrmCntPrs < 
                    (_rSwdmxMkvInfo.u4MkvFrameCount - 1))
                {
                    i8Val = (INT64)_EbmlGetValueLess4(pucFifo) - _VsintSubtr((UINT8)u4VintIDSz);
                    i8Val += (INT64)_rSwdmxMkvInfo.au4MkvFrmCntPrsSz[_rSwdmxMkvInfo.u4MkvFrmCntPrs - 1];
                    i8Val = (i8Val > 0) ? i8Val : 0;
                    _rSwdmxMkvInfo.au4MkvFrmCntPrsSz[_rSwdmxMkvInfo.u4MkvFrmCntPrs] = (UINT32)i8Val;
                    _rSwdmxMkvInfo.u4MkvFrmAccSz += 
                        _rSwdmxMkvInfo.au4MkvFrmCntPrsSz[_rSwdmxMkvInfo.u4MkvFrmCntPrs] ;
                    *pu4ConsumedSz += u4VintIDSz;
                }
                else if(_rSwdmxMkvInfo.u4MkvFrmCntPrs == 
                    (_rSwdmxMkvInfo.u4MkvFrameCount - 1)) //last frame
                {
                    _rSwdmxMkvInfo.au4MkvFrmCntPrsSz[_rSwdmxMkvInfo.u4MkvFrmCntPrs] =
                        _CalDataSize((UINT32)pucFifo, _MkvGetElementEa(4)) - 
                        _rSwdmxMkvInfo.u4MkvFrmAccSz;

                    _rSwdmxMkvInfo.ePrsState = MKVPRS_G_DATA_LACING;
                    _rSwdmxMkvInfo.u4MkvFrmCntPrs = 0;
                    _rSwdmxMkvInfo.u4MkvFrmAccSz = 0;
                }
                break;
                
            case MKVPRS_G_DATA_LACING:                
                if(_rSwdmxMkvInfo.u4MkvFrmCntPrs < 
                    _rSwdmxMkvInfo.u4MkvFrameCount)
                {
                    if((u4AvalSz < ((*pu4ConsumedSz + MKV_UNCERTAIN_SZ) + 
                        _rSwdmxMkvInfo.au4MkvFrmCntPrsSz[_rSwdmxMkvInfo.u4MkvFrmCntPrs])) &&
                        !_rSwdmxMkvInfo.fgReadEOS)
                    {
                        _rSwdmxMkvInfo.fgIsUnderFlw = TRUE;
                        LOG(3, "Data !enough\n");
                        return TRUE;
                    }
                    if(_vMKVDeliverData(_rSwdmxMkvInfo.u1StrmType,
                            pucFifo, 
                            _rSwdmxMkvInfo.au4MkvFrmCntPrsSz[_rSwdmxMkvInfo.u4MkvFrmCntPrs],
                            FALSE, TRUE))
                    {
                        *pu4ConsumedSz += 
                            _rSwdmxMkvInfo.au4MkvFrmCntPrsSz[_rSwdmxMkvInfo.u4MkvFrmCntPrs];
                        _rSwdmxMkvInfo.u4MkvFrmCntPrs++;
                    }
                    else
                    {
                        return TRUE;
                    }
                }
                else
                {
                    _rSwdmxMkvInfo.ePrsState = MKVPRS_G_SEGMENT_CLUSTER_BG_ELEMENT_ID;
                }
                break;
    
                // 5.6 Segment.Cues.CuePoont
            case MKVPRS_G_SEGMENT_CP_ID:
                _MkvSetElementRange(2, (UINT32)pucFifo, u4ElmtSz);
                if (fgMkvIsCRC32(pucFifo))
                {
                    *pu4ConsumedSz += u4ElmtSz;
                }
                else if (fgMkvIsCueCuePointID(pucFifo))
                {
                    _rSwdmxMkvInfo.ePrsState = MKVPRS_G_SEGMENT_CP_ELEMENT_ID;
                    *pu4ConsumedSz += u4ElmtSz;
                }
                /*else if (_dMkvFileOfst >= dMkvGetElementEa(1))
                { // Return to Parent
                    _rSwdmxMkvInfo.ePrsState = MKVPRS_G_SEGMENT_ELEMENT_ID;
                    vMkvPrsNextG(0);
                }*/
                else // Unknow ID, skip this element
                {
                    //error handling
                    if(u4ElmtSz == 0)
                    {
                        *pu4ConsumedSz = u4AvalSz;
                        return TRUE;
                    }
                    else
                    {
                        *pu4ConsumedSz += u4ElmtSz;
                    }
                }
                break;
    
            case MKVPRS_G_SEGMENT_CP_ELEMENT_ID:
                _MkvSetElementRange(3, (UINT32)pucFifo, u4ElmtSz);
                if (fgMkvIsCPTimeID(pucFifo))
                {
                    *pu4ConsumedSz += u4ElmtSz;
                }
                else if (fgMkvIsCPTrackPositionID(pucFifo))
                {
                    _rSwdmxMkvInfo.ePrsState = MKVPRS_G_SEGMENT_CP_TP_ELEMENT_ID;
                    *pu4ConsumedSz += u4HdrSz;
                }
                /*else if (_dMkvFileOfst >= dMkvGetElementEa(2))
                { // Return to Parent
                    _rSwdmxMkvInfo.ePrsState = MKVPRS_G_SEGMENT_CP_ID;
                    vMkvPrsNextG(0);
                }*/
                else // Unknow ID, skip this element
                {
                    //error handling
                    if(u4ElmtSz == 0)
                    {
                        *pu4ConsumedSz = u4AvalSz;
                        return TRUE;
                    }
                    else
                    {
                        *pu4ConsumedSz += u4ElmtSz;
                    }
                }
                break;
            case MKVPRS_G_SEGMENT_CP_TP_ELEMENT_ID:
                _MkvSetElementRange(4, (UINT32)pucFifo, u4ElmtSz);
                if (fgMkvIsTPTrackID(pucFifo))
                {
                    *pu4ConsumedSz += u4ElmtSz;
                }
                else if (fgMkvIsTPClusterPositionID(pucFifo))
                {
                    *pu4ConsumedSz += u4ElmtSz;
                }
                else if (fgMkvIsTPBlockNumberID(pucFifo))
                {
                    *pu4ConsumedSz += u4ElmtSz;
                }
                /*else if (_dMkvFileOfst >= dMkvGetElementEa(3))
                { // Return to Parent
                    _rSwdmxMkvInfo.ePrsState = MKVPRS_G_SEGMENT_CP_ELEMENT_ID;
                    vMkvPrsNextG(0);
                }*/
                else // Unknow ID, skip this element
                {
                    //error handling
                    if(u4ElmtSz == 0)
                    {
                        *pu4ConsumedSz = u4AvalSz;
                        return TRUE;
                    }
                    else
                    {
                        *pu4ConsumedSz += u4ElmtSz;
                    }
                }
                break;
    
            default:
                //error handling
                if(u4ElmtSz == 0)
                {
                    *pu4ConsumedSz = u4AvalSz;
                    return TRUE;
                }
                else
                {
                    *pu4ConsumedSz += u4ElmtSz;
                }
                break;
        }
    }
    return TRUE;
}


static BOOL _bReqDataFromFeeder(UINT64 u8FileOffset, UINT32 u4ReadSize)
{
    FEEDER_TRANSMIT_COND rFeederCond;

    x_memset((void*)&rFeederCond, 0, sizeof(FEEDER_TRANSMIT_COND));
    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederMkvInfo) != 
        FEEDER_E_OK)
    {
        LOG(2, "Feeder input buffer error.\n");
        return FALSE;
    }

    u4ReadSize = cal_min(u4ReadSize, MIN_READ_SIZE);

    if(FeederSetRequest(FEEDER_PROGRAM_SOURCE, 
        u4ReadSize,
        u8FileOffset,
        &rFeederCond,
        _rSwdmxMkvInfo.u4FeederReqID) != FEEDER_E_BUFFER_FULL)
    {
        _u4LastAlignOffset = 0;
        _rSwdmxMkvInfo.i4ReqReadTimes++;
        _rSwdmxMkvInfo.u4FeederReqID++;
        LOG(6, "Req wptr 0x%x\n", _rFeederMkvInfo.u4WriteAddr);
    }
    else
    {
        LOG(3, "Feeder Buffer Full\n");
        //return FALSE;
    }
    return TRUE;
}


static BOOL _bGetDataFromFeederAck(FEEDER_REQ_DATA_T *prFeederData)
{
    if (!_SWDMX_ReceiveFeederAck((VOID*)prFeederData))
    {
        _rSwdmxMkvInfo.fgFeederInvalid = TRUE;
        LOG(3, "GetDataFromFeeder FeederTimeOut!!!\n");
        return FALSE;        
    }
    if(prFeederData->eDataType == FEEDER_SOURCE_INVALID)
    {
        LOG(3, "GetDataType=FEEDER_SOURCE_INVALID, Q\n"/*, _SWDMX_GetFeederQueueSize()*/);
        return FALSE;
    }

    if((_rSwdmxMkvInfo.i4ReqReadTimes == 0) || 
        (prFeederData->u4Id != (_rSwdmxMkvInfo.u4FeederReqID - _rSwdmxMkvInfo.i4ReqReadTimes)))
    {
        LOG(3, "ReqTimesErr\n");
        return FALSE;
    }
     _u4LastAlignOffset = prFeederData->u4AlignOffset;
    _rSwdmxMkvInfo.u8CurMoviPos += prFeederData->u4ReadSize -_u4LastAlignOffset;
    _rSwdmxMkvInfo.u4LeftBytesInFeeder += prFeederData->u4ReadSize -_u4LastAlignOffset;
    _rSwdmxMkvInfo.i4ReqReadTimes--;
    if(prFeederData->u4ReadSize > (MIN_READ_SIZE + FDR_ALIGN_SIZE))    //error handling
    {
        LOG(3, "ReqDataSzErr?\n");
        //return FALSE;
    }
    return TRUE;
}


static BOOL _bCalculateRepeatFrame(INT32 i4PlaySpeed)
{
    UINT32 u4CurKey = _rSwdmxMkvInfo.u4CurSentKey;
    MM_RANGE_ELMT_IDXTBL_T *prKeyIdxTbl;
    UINT32 u4CurRptPTS = 0, u4CurSentPTS = 0, u4EntryCnt;
    UINT32 u4PtsDif = 0, u4MaxRepeatFrame;
    
    u4EntryCnt = 0;
    prKeyIdxTbl = _rSwdmxMkvInfo.prVidKeyIdxTbl;
    while(prKeyIdxTbl->pt_next_partial && 
        (u4CurKey >= (u4EntryCnt + prKeyIdxTbl->ui4_number_of_entry)))
    {
        prKeyIdxTbl = prKeyIdxTbl->pt_next_partial;
        u4EntryCnt += prKeyIdxTbl->ui4_number_of_entry;
    }
    u4CurSentPTS = prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey - u4EntryCnt].ui4_pts;

    u4EntryCnt = 0;
    prKeyIdxTbl = _rSwdmxMkvInfo.prVidKeyIdxTbl;
    while(prKeyIdxTbl->pt_next_partial && 
        (_rSwdmxMkvInfo.u4CurRepeatFrameIdx >= 
        (u4EntryCnt + prKeyIdxTbl->ui4_number_of_entry)))
    {
        prKeyIdxTbl = prKeyIdxTbl->pt_next_partial;
        u4EntryCnt += prKeyIdxTbl->ui4_number_of_entry;
    }

    u4MaxRepeatFrame = (_rSwdmxMkvInfo.u4VidFps > 10) ? _rSwdmxMkvInfo.u4VidFps : 30; 
    //(_rSwdmxMkvInfo.u4VidFps * 1000)/abs(i4PlaySpeed);

    if (_rSwdmxMkvInfo.u4VidRepeatFrame == 0)
    {
        if (_rSwdmxMkvInfo.u4CurRepeatFrameIdx != u4CurKey &&
            (_rSwdmxMkvInfo.u4CurRepeatFrameIdx < _rSwdmxMkvInfo.u4NumOfKeyEntry))
        {
            //if key frame is not I picture, key table will be updated in _MKVCuePointParsing
            u4CurRptPTS = _rSwdmxMkvInfo.u4CurRptPTS;
            
            if (i4PlaySpeed > FF_2X)
            {
                if (u4CurKey < _rSwdmxMkvInfo.u4NumOfKeyEntry)
                {
                    u4PtsDif = (u4CurSentPTS > u4CurRptPTS) ? (u4CurSentPTS - u4CurRptPTS) : 0;

                    _rSwdmxMkvInfo.u4VidRepeatFrame = 
                        (UINT32)((u4PtsDif * u4MaxRepeatFrame)/((90 * i4PlaySpeed) << 1));   // << 1, factor
                }
                else
                {
                    _rSwdmxMkvInfo.u4VidRepeatFrame = (u4MaxRepeatFrame * 1000)/cal_abs(i4PlaySpeed);
                }
            }
            else if (IsBackward(i4PlaySpeed))
            {
                u4PtsDif = (u4CurRptPTS > u4CurSentPTS) ? (u4CurRptPTS - u4CurSentPTS) : 0;

                _rSwdmxMkvInfo.u4VidRepeatFrame = 
                    (UINT32)((u4PtsDif * u4MaxRepeatFrame)/((90 * cal_abs(i4PlaySpeed)) << 1));;
            }

            _rSwdmxMkvInfo.u4VidRepeatFrame = (u4MaxRepeatFrame > _rSwdmxMkvInfo.u4VidRepeatFrame) ?
                (_rSwdmxMkvInfo.u4VidRepeatFrame) : u4MaxRepeatFrame;

            LOG(3, "u4PtsDif = %ld CurKeyIdx = %ld, RepeatIdx = %d, Repeat frame = %ld\n", 
                u4PtsDif, u4CurKey, _rSwdmxMkvInfo.u4CurRepeatFrameIdx, _rSwdmxMkvInfo.u4VidRepeatFrame);

            if ((u4CurKey == _rSwdmxMkvInfo.u4NumOfKeyEntry - 1) ||
                (u4CurKey == 0))    // must display
            {
                _rSwdmxMkvInfo.u4VidRepeatFrame = u4MaxRepeatFrame << 1;
                    /*(UINT32)((_rSwdmxMkvInfo.u4VidRepeatFrame < MIN_REPEAT_FRM) ? 
                    MIN_REPEAT_FRM : _rSwdmxMkvInfo.u4VidRepeatFrame);*/
            }
            else if (_rSwdmxMkvInfo.u4VidRepeatFrame < MIN_REPEAT_FRM)
            {
                _rSwdmxMkvInfo.u4VidRepeatFrame = 0;
                return FALSE;
            }

            _rSwdmxMkvInfo.u4CurRepeatFrameIdx = u4CurKey;
            _rSwdmxMkvInfo.u4CurRptPTS = prKeyIdxTbl->
                u.pt_pb_idx_tbl_entry[_rSwdmxMkvInfo.u4CurRepeatFrameIdx - u4EntryCnt].ui4_pts;

            return TRUE;
        }
        else
        {
            if(_rSwdmxMkvInfo.u4CurRepeatFrameIdx == (UINT32)0xFFFFFFFF)    // first key frame
            {
                _rSwdmxMkvInfo.u4VidRepeatFrame = (u4MaxRepeatFrame * 1000)/cal_abs(i4PlaySpeed);
                _rSwdmxMkvInfo.u4CurRepeatFrameIdx = u4CurKey;
                _rSwdmxMkvInfo.u4CurRptPTS = prKeyIdxTbl->
                    u.pt_pb_idx_tbl_entry[_rSwdmxMkvInfo.u4CurRepeatFrameIdx - u4EntryCnt].ui4_pts;
                
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
    }

    _rSwdmxMkvInfo.u4VidRepeatFrame--;
    return TRUE;
}


static BOOL _bSeekTime(UINT32 u4SeekTime, SWDMX_RANGE_LIST_T* prRangeList)
{
    INT32 i;
    UINT32 u4PrevIdx = 0;
    BOOL fgFindKey = FALSE;
    MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl = _rSwdmxMkvInfo.prVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prPrevKeyIdxTbl = prVidKeyIdxTbl;
    
    LOG(7, "MkvSeek u4SeekTime = 0x%x\n", u4SeekTime);

    _SWDMXResetFeeder();
    _rSwdmxMkvInfo.u4LeftBytesInFeeder = 0;

    if (prVidKeyIdxTbl)
    {
        if (prVidKeyIdxTbl->ui4_number_of_entry == 0)
        {
            LOG(5, "MkvSeek doesn't have the key table\n");
            return FALSE;
        }
        _rSwdmxMkvInfo.u4CurSentKey = 0;

        //check if target pts is less than first key entry
        if(u4SeekTime < prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts)
        {
            LOG(3, "TargetPTS < key entry0 0x%x, set to file begin\n",
                prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts);
            _rSwdmxMkvInfo.u8CurDmxMoviPos = 0;
            _rSwdmxMkvInfo.u4CurVidPts = 0;
        }
        else
        {        
            do
            {
                //check if there's next key table
                for (i = 0; i < (INT32)prVidKeyIdxTbl->ui4_number_of_entry; i++)
                {
                    if (prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts > u4SeekTime)
                    {
                        fgFindKey = TRUE;
                        break;
                    }
                }
                _rSwdmxMkvInfo.u4CurSentKey += (UINT32)((i > 0) ? (i - 1) : i);
                if(!fgFindKey)
                {  
                    if(prVidKeyIdxTbl->pt_next_partial)
                    {
                        u4PrevIdx = _rSwdmxMkvInfo.u4CurSentKey;
                        prPrevKeyIdxTbl = prVidKeyIdxTbl;                
                        prVidKeyIdxTbl = prVidKeyIdxTbl->pt_next_partial;
                    }
                    else
                    {
                        LOG(3, "keyTbl missing\n");
                        break;
                    }
                }
                else if(i == 0) //prev table
                {
                    i = u4PrevIdx;
                    prVidKeyIdxTbl = prPrevKeyIdxTbl;
                }
            }while(!fgFindKey);

            /*if(IsBackward(_rSwdmxMkvInfo.i4PrevSpeed) &&
                (_rSwdmxMkvInfo.u4CurSentKey < (prVidKeyIdxTbl->ui4_number_of_entry - 1)))
            {
                _rSwdmxMkvInfo.u4CurSentKey++;  // for render PTS
            }*/

            _rSwdmxMkvInfo.u8CurDmxMoviPos = 
                (prVidKeyIdxTbl->ui8_base_offset +
                (UINT64)prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[((i > 0) ? (i - 1) : i)].ui4_size) - 
                _rSwdmxMkvInfo.u8FileOffset;
            _rSwdmxMkvInfo.u4CurVidPts = 
                prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[((i > 0) ? (i - 1) : i)].ui4_pts;
        }
        
        _rSwdmxMkvInfo.u4CurRepeatFrameIdx = _rSwdmxMkvInfo.u4CurSentKey;
        _rSwdmxMkvInfo.u4CurRptPTS = _rSwdmxMkvInfo.u4CurVidPts;
        
        LOG(5, "Mkv Seek key idx = %d Cluster offset = %lld\n", 
            _rSwdmxMkvInfo.u4CurSentKey,
            _rSwdmxMkvInfo.u8CurDmxMoviPos);

        LOG(5, "Mkv Seek key pts = 0x%08x, diff target %ds\n", 
            _rSwdmxMkvInfo.u4CurVidPts,
            (u4SeekTime - _rSwdmxMkvInfo.u4CurVidPts) / 90000);

        _rSwdmxMkvInfo.u8CurMoviPos = _rSwdmxMkvInfo.u8CurDmxMoviPos;
        
        return TRUE;
    }
    else
    {
        _rSwdmxMkvInfo.u8CurDmxMoviPos = 0;
        _rSwdmxMkvInfo.u8CurMoviPos = 0;
        return FALSE;
    }
}


static SWDMX_HDR_PRS_STATE_T _MKVCuePointParsing
    (SWDMX_RANGE_LIST_T* prRangeList, INT32 i4PlaySpeed)
{
    MM_RANGE_ELMT_IDXTBL_T* prKeyTbl = _rSwdmxMkvInfo.prVidKeyIdxTbl;
    FEEDER_REQ_DATA_T rFeederReqData;
    UINT32 u4CurIdx = _rSwdmxMkvInfo.u4CurSentKey;
    UINT32 u4ConsumedSize = 0, u4ConsumedSzAcc = 0, u4AvalSz = 0;
    #if 0
    UINT32 u4ReqSz = 0;
    #endif
    UINT64 u8ReqOffst = 0, u8KeyClusterOffst = 0;
    UINT8* pucBuf = NULL;
    UINT16 u2CurQNum, u2MaxQNum;
    
    x_memset((void*)&rFeederReqData, 0, sizeof(FEEDER_REQ_DATA_T));
    if (prKeyTbl == NULL)
    {
        LOG(3, "No key table\n");
        x_thread_delay(10);
        return eSWDMX_HDR_PRS_EMPTY_RANGE;
    }
    //=== Key table parsing
    if(u4CurIdx < _rSwdmxMkvInfo.u4NumOfKeyEntry)
    {
        // check Q size, flow control
        VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);
        if(u2CurQNum > MAX_KEY_FRM)
        {
            x_thread_delay(10);
            return eSWDMX_HDR_PRS_SUCCEED;
        }
        
        // calculate display period (repeat frame)
        if(!_bCalculateRepeatFrame(i4PlaySpeed))
        {
            if (i4PlaySpeed > FF_2X)
            {
                _rSwdmxMkvInfo.u4CurSentKey++;
            }
            else if (IsBackward(i4PlaySpeed))
            {
                if(_rSwdmxMkvInfo.u4CurSentKey > 0)
                {
                    _rSwdmxMkvInfo.u4CurSentKey--;
                }
                else if(_rSwdmxMkvInfo.u4CurSentKey == 0)
                {
                    _rSwdmxMkvInfo.u8CurDmxMoviPos = _rSwdmxMkvInfo.u8RangeSz;
                    prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;  
                }
            }
            return eSWDMX_HDR_PRS_SUCCEED;
        }
        
        //find key position
        while(u4CurIdx >= prKeyTbl->ui4_number_of_entry)
        {
            u4CurIdx -= prKeyTbl->ui4_number_of_entry;
            if(prKeyTbl->pt_next_partial)
            {
                prKeyTbl = prKeyTbl->pt_next_partial;
            }
            else
            {
                LOG(3, "keyTbl missing\n");
                break;
            }
        }

        u8KeyClusterOffst = (prKeyTbl->ui8_base_offset +
        (UINT64)prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_size) - 
        _rSwdmxMkvInfo.u8FileOffset;
        
        LOG(3, "Idx %d - Blk Ofst, Cluster Ofst %lld, Pts 0x%x\n", u4CurIdx,
        /*prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_relative_offset,*/
        prKeyTbl->ui8_base_offset +
        (UINT64)prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_size,
        prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_pts);

        _rSwdmxMkvInfo.u8CurDmxMoviPos = u8KeyClusterOffst;
        _rSwdmxMkvInfo.u8CurMoviPos = u8KeyClusterOffst;

        //=== Prepare data
        _rSwdmxMkvInfo.ePrsState = MKVPRS_G_SEGMENT_ELEMENT_ID;
        u8ReqOffst = prKeyTbl->ui8_base_offset + (UINT64)prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_size;
        _u4LastAlignOffset = 0;
        _rSwdmxMkvInfo.fgIsSeek = TRUE;
        _rSwdmxMkvInfo.u8SeekPts = prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_pts;
        _rSwdmxMkvInfo.u4KeyDeliveredPTS = (UINT32)_rSwdmxMkvInfo.u8SeekPts;
        _rSwdmxMkvInfo.fgKeyDelivered = FALSE;
        if(_rSwdmxMkvInfo.u4VDecType == ENUM_SWDMX_VDEC_H264)
        {
            _rSwdmxMkvInfo.fgWaitIFrm = TRUE;
        }
        _rSwdmxMkvInfo.u4KeyPTS = prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_pts;
        if((_rSwdmxMkvInfo.u1DispStruct == DISP_INTERLACE) &&
            (_rSwdmxMkvInfo.u4VidFps >= 50))
        {
            _rSwdmxMkvInfo.u4KeyPTS += ((90000/_rSwdmxMkvInfo.u4VidFps) + 100);
        }
        
        //=== Deliver data
        do
        {
            if((u4AvalSz - u4ConsumedSzAcc) < (MKV_BUFFER_SIZE - MIN_READ_SIZE))
            {
                //=== Request data
                if(_rSwdmxMkvInfo.i4ReqReadTimes == 0)
                {
                    if(!_bReqDataFromFeeder(u8ReqOffst, 
                        _rSwdmxMkvInfo.u8RangeSz - _rSwdmxMkvInfo.u8CurMoviPos))
                    {
                        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                    }
                }            
                //=== Wait feeder ack
                if (_rSwdmxMkvInfo.i4ReqReadTimes > 0)
                {
                    if(!_bGetDataFromFeederAck(&rFeederReqData))
                    {
                        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                    }
                    else
                    {
                        LOG(6, "CueAck ptr 0x%x--0x%x, ofst 0x%x\n", 
                            rFeederReqData.u4WriteAddr, 
                            rFeederReqData.u4WriteAddr + rFeederReqData.u4ReadSize, 
                            _u4LastAlignOffset);
                        u4AvalSz += (rFeederReqData.u4ReadSize - _u4LastAlignOffset);
                        u8ReqOffst += (UINT64)(rFeederReqData.u4ReadSize - _u4LastAlignOffset);
                    }
                }
            }        
            if(pucBuf == NULL)
            {
                pucBuf = (UCHAR*)(VIRTUAL(rFeederReqData.u4WriteAddr + rFeederReqData.u4AlignOffset));
                _rSwdmxMkvInfo.u4CurDmxFeederRPtr = (UINT32)pucBuf;
                //error handling
                if((pucBuf == 0) ||
                    (((UINT32)pucBuf > VIRTUAL(_rFeederMkvInfo.u4EndAddr)) ||
                    ((UINT32)pucBuf < VIRTUAL(_rFeederMkvInfo.u4StartAddr))))
                {
                    LOG(3, "CuePt DataReq Err!\n");
                    return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                }                
            }

            //set cluster end addr           
            _vMKVClusterParsing((UCHAR*)_rSwdmxMkvInfo.u4CurDmxFeederRPtr,
                prRangeList,
                u4AvalSz - u4ConsumedSzAcc,
                &u4ConsumedSize);
            LOG(6, "CuePrs 0x%x--0x%x\n", _rSwdmxMkvInfo.u4CurDmxFeederRPtr, 
                _rSwdmxMkvInfo.u4CurDmxFeederRPtr + u4ConsumedSize);                
            u4ConsumedSzAcc += u4ConsumedSize;
            u4ConsumedSzAcc = (u4ConsumedSzAcc < u4AvalSz) ? u4ConsumedSzAcc : u4AvalSz;
            _rSwdmxMkvInfo.u8CurDmxMoviPos += u4ConsumedSize;

            if ((_rSwdmxMkvInfo.u4CurDmxFeederRPtr + u4ConsumedSize) >= 
                _rFeederMkvInfo.u4EndAddr)
            {
                _rSwdmxMkvInfo.u4CurDmxFeederRPtr =  _rFeederMkvInfo.u4StartAddr + 
                    ((_rSwdmxMkvInfo.u4CurDmxFeederRPtr + u4ConsumedSize) -_rFeederMkvInfo.u4EndAddr);
            }
            else
            {
                _rSwdmxMkvInfo.u4CurDmxFeederRPtr += u4ConsumedSize;
            }
            
            FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, _rSwdmxMkvInfo.u4CurDmxFeederRPtr);
        }while((!_rSwdmxMkvInfo.fgKeyDelivered) &&
        (_rSwdmxMkvInfo.u8CurDmxMoviPos < _rSwdmxMkvInfo.u8RangeSz));

        if(_rSwdmxMkvInfo.fgKeyDelivered &&
            (_rSwdmxMkvInfo.u4KeyDeliveredPTS != 
            prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_pts))
        {
            prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_pts = _rSwdmxMkvInfo.u4KeyDeliveredPTS;
            LOG(5, "Upd Key(Idx,PTS)=(%d, 0x%x)\n", u4CurIdx, _rSwdmxMkvInfo.u4KeyDeliveredPTS);
            if(_rSwdmxMkvInfo.u8CurClusterPos != u8KeyClusterOffst)
            {
                prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_size = 
                    (u8KeyClusterOffst + _rSwdmxMkvInfo.u8FileOffset) - 
                    prKeyTbl->ui8_base_offset;
                LOG(5, "Upd Key(Idx,Ofs)=(%d, 0x%x)\n", u4CurIdx, 
                    prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_size);
            }
        }
    }
    else
    {
        _rSwdmxMkvInfo.u8CurDmxMoviPos = _rSwdmxMkvInfo.u8RangeSz;
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;  
    }



    //=== Reset feeder
    /*if(!_SWDMXResetFeeder())
    {
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }*/
    
    return eSWDMX_HDR_PRS_SUCCEED;
}


static BOOL _SWDMXInitDmx(UINT8 u1HdrType, VOID* pfnCbFunc, BOOL fgEos)
{
    BOOL fgRet = TRUE;
    UINT32 u4Flag = 0;
    #ifdef AUD_PREBUF_EN
    UINT32 u4Idx; 
    #endif
    FBM_POOL_T* prFbmPool;
    DMX_MM_T rDmxMMInfo;

    x_memset((void*)&rDmxMMInfo, 0, sizeof(DMX_MM_T));
    
    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;

        if (!_rSwdmxMkvInfo.fgEnVDmx)
        {
            // Allocate video buffer from FBM
            prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
            if((prFbmPool == NULL) || (prFbmPool->u4Addr == 0))
            {
                LOG(3, "_SWDMX_SetVideoStreamID (prFbmPool == NULL) || (prFbmPool->u4Addr == NULL)\n");
                return FALSE;
            }

            u4Flag = (UINT32)(DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY | 
              DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF);

            rDmxMMInfo.fgAllocBuf = FALSE;
            rDmxMMInfo.u4BufAddr = prFbmPool->u4Addr;
            rDmxMMInfo.u4BufSize = prFbmPool->u4Size;

            u4Flag |= DMX_MM_FLAG_SEARCH_START_CODE;

            switch (_rSwdmxMkvInfo.u4VDecType)
            {
                case ENUM_SWDMX_VDEC_DX311: case ENUM_SWDMX_VDEC_WMV7:
                case ENUM_SWDMX_VDEC_WMV8: case ENUM_SWDMX_VDEC_WMV9:
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
    
        if (!_rSwdmxMkvInfo.fgEnADmx)
        {
            u4Flag = (UINT32)(DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY | 
              DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF);
        
#ifdef AUD_OLDFIFO_INTF
            if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
                 &au4BufEnd[1]) != AUD_OK)
#else
            if (AUD_GetAudFifo(AUD_DEC_MAIN, &au4BufStart[0], &au4BufEnd[0]) != AUD_OK)
#endif
            {
                    LOG(3, "_SWDMX_SetAudioStreamID Can't get audio buffers!\n");
                    return FALSE;
            }
            rDmxMMInfo.fgAllocBuf = FALSE;
            
            rDmxMMInfo.u4BufAddr = au4BufStart[0];
            rDmxMMInfo.u4BufSize = au4BufEnd[0] - au4BufStart[0];
            fgRet = DMX_MM_Set(eSWDMX_MEDIA_AUDIO, u4Flag, &rDmxMMInfo);

            #ifdef AUD_PREBUF_EN
            //audio prebuffer setting
            if(_rSwdmxMkvInfo.prDmxRangeInfo)   //execute after range added
            {
                if(!DMX_AudHandler_Set(eSWDMX_MEDIA_AUDIO, TRUE))
                {
                    LOG(1, "MKV DMX_AudHandler_Set fail\n");
                }
                else
                {
                    for (u4Idx = 0; u4Idx < MKV_MAX_AUD_TRACK; u4Idx++)
                    {
                        rDmxMMInfo.fgAllocBuf = TRUE;            
                        rDmxMMInfo.u4BufSize = au4BufEnd[0] - au4BufStart[0];
                        if(_rSwdmxMkvInfo.fgIsAudTrack[u4Idx])
                        {
                            if(DMX_MM_Set(MKV_AUD_PREBUF_IDX + u4Idx, u4Flag, &rDmxMMInfo))
                            {
                                if(!DMX_AudHandler_Set(MKV_AUD_PREBUF_IDX + u4Idx, FALSE))
                                {
                                    LOG(1, "MKV DMX_AudHandler_Set fail\n");
                                    break;
                                }
                                _rSwdmxMkvInfo.fgAudPreBuf[u4Idx] = TRUE;
                            }
                            else
                            {
                                LOG(1, "MKV DMX_MM_Set pre-buffer fail\n");
                                break;
                            }
                        }
                    }
                    if(u4Idx == MKV_MAX_AUD_TRACK)  // all track set successfully
                    {
                        fgRet = DMX_AudHandler_SetActivePidx(MKV_AUD_UNKNOWN_IDX);
                        fgActivePidxIsUnKnown = TRUE;
                        DMX_AudHandler_SetEnable(TRUE);
                        _rSwdmxMkvInfo.u1FF2xTo1xMode = AUD_PRE_BUF;
                    }
                }
            }
            #endif
        }
        else if (fgEos)
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;
            fgRet = DMX_MM_Set(eSWDMX_MEDIA_AUDIO, u4Flag, &rDmxMMInfo);

            #ifdef AUD_PREBUF_EN
            //audio prebuffer setting
            if(_rSwdmxMkvInfo.u1FF2xTo1xMode == AUD_PRE_BUF)
            {
                for (u4Idx = 0; u4Idx < MKV_MAX_AUD_TRACK; u4Idx++)
                {
                    if(_rSwdmxMkvInfo.fgIsAudTrack[u4Idx])
                    {
                        fgRet = DMX_MM_Set(MKV_AUD_PREBUF_IDX + u4Idx, u4Flag, &rDmxMMInfo);
                    }
                }
            }
            #endif
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_SUBTITLE)
    {
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_PES;//DMX_PID_TYPE_PES_TIME;

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
            rDmxMMInfo.u4BufSize = MKV_SUBTITLE_BUFSIZE;

            fgRet = DMX_MM_Set(eSWDMX_MEDIA_SUBTITLE, u4Flag, &rDmxMMInfo);
        }
    }
    return fgRet;
}


static BOOL _SWDMXResetFeeder(void)
{
    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederMkvInfo) != 
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    LOG(6, "RstFdr Cur Fed's WPtr = 0x%08x\n", _rFeederMkvInfo.u4WriteAddr);
    _rSwdmxMkvInfo.u4CurDmxFeederRPtr = _rFeederMkvInfo.u4WriteAddr;

    // Update read ptr to feeder
    FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, _rSwdmxMkvInfo.u4CurDmxFeederRPtr);

    _SWDMX_FlushReqMsgQ();
    _rSwdmxMkvInfo.i4ReqReadTimes = 0;  //invalid flush msg
    
    return TRUE;
}


static BOOL _SWDMX_SendMkvEos(UINT8 u1HdrType)
{
    UINT16 u2MaxQNum, u2CurQNum;
    UINT32 u4DummyData[4];
    #ifdef AUD_PREBUF_EN
    UINT32 u4Idx;
    #endif
    BOOL fgRet = FALSE;
    DMX_MM_DATA_T rDmxMMData;

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);
        LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
        if (u2CurQNum > SEND_EOS_THRD)
        {  
            x_thread_delay(10);
            return FALSE;
        }
    }
    
    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if (_SWDMXInitDmx(u1HdrType, NULL, TRUE) == FALSE)
    {
        return FALSE;
    }

    rDmxMMData.u4BufStart = (UINT32)(&(u4DummyData[0]))&0xFFFFFFF0;
    rDmxMMData.u4BufEnd = ((UINT32)(&(u4DummyData[2])) + 0x10+188)&0xFFFFFFF0;
    rDmxMMData.u4StartAddr = (UINT32)&(u4DummyData[1]);
    rDmxMMData.u4FrameSize = 4;
    rDmxMMData.fgEOS = TRUE;

    #ifdef AUD_PREBUF_EN
    //audio prebuffer setting
    if((u1HdrType == eSWDMX_MEDIA_AUDIO) &&
        (_rSwdmxMkvInfo.u1FF2xTo1xMode == AUD_PRE_BUF))
    {
        for (u4Idx = 0; u4Idx < MKV_MAX_AUD_TRACK; u4Idx++)
        {   if(_rSwdmxMkvInfo.fgIsAudTrack[u4Idx])
            {
            fgRet |= DMX_MM_MoveData(MKV_AUD_PREBUF_IDX + u4Idx, &rDmxMMData, 500);
        	  }
        }
    }
    else
    {
        fgRet = DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500);
    }
    #else    
    fgRet = DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500);
    #endif
    
    return fgRet;
}


//DivX Plus Trick play track feature
static BOOL _SWDMXMkvSwitchTrack(BOOL fgIsTPT)
{
    if(fgIsTPT)
    {
        #ifdef DIVX_PLUS_CER
        if(_rSwdmxMkvInfo.prTPTVidKeyIdxTbl &&
            (_rSwdmxMkvInfo.u4TPTNumOfKeyEntry >= MKV_KEY_TABLE_THRESHOLD))
        {
            _rSwdmxMkvInfo.u8FileOffset = _rSwdmxMkvInfo.u8TPTFileOffset;
            _rSwdmxMkvInfo.u8RangeSz = _rSwdmxMkvInfo.u8TPTRangeSz;
            _rSwdmxMkvInfo.prVidKeyIdxTbl = _rSwdmxMkvInfo.prTPTVidKeyIdxTbl;
            _rSwdmxMkvInfo.u4NumOfKeyEntry = _rSwdmxMkvInfo.u4TPTNumOfKeyEntry;
            _rSwdmxMkvInfo.u4KeyPTS = 0xFFFFFFFF;
            return TRUE;
        }
        #endif
    }
    else
    {
        _rSwdmxMkvInfo.u8FileOffset = _rSwdmxMkvInfo.u8MainFileOffset;
        _rSwdmxMkvInfo.u8RangeSz = _rSwdmxMkvInfo.u8MainRangeSz;        
        _rSwdmxMkvInfo.prVidKeyIdxTbl = _rSwdmxMkvInfo.prMainVidKeyIdxTbl;
        _rSwdmxMkvInfo.u4NumOfKeyEntry = _rSwdmxMkvInfo.u4MainNumOfKeyEntry;
    }
    return FALSE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

BOOL _SWDMX_MKVInit(ENUM_SWDMX_FMT_T eType)
{
    DMX_DECODER_CALLBACKS_T rCallback;
    
    UNUSED(eType);
    // Initialize feeder
    //FeederOpen(FEEDER_PROGRAM_SOURCE);
    //*********************************************************
    //FeederSetBufferSize(FEEDER_PROGRAM_SOURCE, 5*10*1024); 
    //FeederStart(FEEDER_PROGRAM_SOURCE);
    x_memset((void*)&_rSwdmxMkvInfo, 0, sizeof(SWDMX_MKV_INFO_T));
    x_memset((void*)&_rFeederMkvInfo, 0, sizeof(FEEDER_BUF_INFO_T));
    FeederInstallCallback(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_DATA_REQ_DONE, 
    _SWDMX_FeederCallBack, (UINT32)&_rFeederMkvInfo);
    //*********************************************************

    rCallback.pfnSendAudioPes = AUD_SendAudioPes;
    rCallback.pfnSendVideoPes = _MMVideoCallback;
    rCallback.pfnUpdateVideoWp = VDEC_SetWptr;
    
    DMX_SetDecoderCallbacks(&rCallback);
    DMX_SetToDecoder(TRUE);

    if(!_vMkvInfoInit())
    	{return FALSE;}
    _rSwdmxMkvInfo.u1FF2xTo1xMode = KEEP_VFIFO;
    
    return TRUE;

}


void _SWDMX_MKVSetRange(SWDMX_RANGE_LIST_T *prRangeList, 
    UINT32 u4VidStrmID, 
    UINT32 u4AudStrmID, 
    UINT32 u4SubTitleID)
{
    MM_RANGE_ELMT_IDXTBL_T *prKeyTbl;
    MM_RANGE_ELMT_IDXTBL_T *pBuldIdxTbl;
    UINT32 u4StrmID;
    
    if (prRangeList != NULL &&
         prRangeList->prDmxRangeInfo != NULL)
    {
        pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)prRangeList->prDmxRangeInfo->pt_idxtbl_lst;
        _rSwdmxMkvInfo.prDmxRangeInfo = prRangeList->prDmxRangeInfo;
        UINT32 u4TotalTime = 
            (UINT32)u8Div6432((UINT64)((UINT64)(_rSwdmxMkvInfo.prDmxRangeInfo->ui8_timecode_scale)*
            (UINT64)(_rSwdmxMkvInfo.prDmxRangeInfo->ui8_pb_duration)), 1000000, NULL);
        VDEC_SetTotalTime(ES0,u4TotalTime );
        
        while (pBuldIdxTbl != NULL)
        {
            u4StrmID = pBuldIdxTbl->t_strm_id.u.t_stm_mkv_id;
            if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_KEY)
            {
                if (pBuldIdxTbl->t_strm_id.u.t_stm_mkv_id == u4VidStrmID)
                {
                    _rSwdmxMkvInfo.prMainVidKeyIdxTbl = pBuldIdxTbl;
                    if(_rSwdmxMkvInfo.prMainVidKeyIdxTbl->ui4_scale > 0)
                    {
                        _rSwdmxMkvInfo.u4VidFps = 
                            (UINT32)(_rSwdmxMkvInfo.prMainVidKeyIdxTbl->ui4_rate/
                            _rSwdmxMkvInfo.prMainVidKeyIdxTbl->ui4_scale);
                    }
                    prKeyTbl = pBuldIdxTbl;
                    _rSwdmxMkvInfo.u4MainNumOfKeyEntry = prKeyTbl->ui4_number_of_entry;
                    while(prKeyTbl->pt_next_partial)
                    {
                        prKeyTbl = prKeyTbl->pt_next_partial;
                        _rSwdmxMkvInfo.u4MainNumOfKeyEntry += prKeyTbl->ui4_number_of_entry;
                    }
                }
            }
            else if((pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_AUDIO) && 
                (u4StrmID < MKV_MAX_AUD_TRACK))
            {
                _rSwdmxMkvInfo.fgIsAudTrack[u4StrmID] = TRUE;
                if(pBuldIdxTbl->t_aud_spec_info.e_fmt < 0xFF)
                {
                    _rSwdmxMkvInfo.u4ADecType[u4StrmID] = 
                        _ADecTypeCast(pBuldIdxTbl->t_aud_spec_info.e_fmt);
                }
                if(_rSwdmxMkvInfo.u4ADecType[u4StrmID] == ENUM_SWDMX_ADEC_AAC)
                {
                    _rSwdmxMkvInfo.u4AudSampleRate[u4StrmID] = 
                        _AACSampFreqIdx(pBuldIdxTbl->t_aud_spec_info.u.t_aac_info.ui4_sample_rate);
                    _rSwdmxMkvInfo.u4AudChannelNs[u4StrmID] = 
                        pBuldIdxTbl->t_aud_spec_info.u.t_aac_info.ui4_num_ch;
                }
            }
            pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)pBuldIdxTbl->pv_next_tbl;
            //set segment time scale
            _rSwdmxMkvInfo.u8MkvSegmentInfoTimeCodeScale = (UINT64)prRangeList->prDmxRangeInfo->ui8_timecode_scale * 9;

            LOG(3, "SetSegScale %d\n", (UINT32)_rSwdmxMkvInfo.u8MkvSegmentInfoTimeCodeScale);

        }

        _rSwdmxMkvInfo.u8MainFileOffset = prRangeList->prDmxRangeInfo->ui8_fileoffset;
        _rSwdmxMkvInfo.u8MainRangeSz = prRangeList->prDmxRangeInfo->z_range_sz;
        _rSwdmxMkvInfo.fgIsTPT = _SWDMXMkvSwitchTrack(FALSE);
        if(_rSwdmxMkvInfo.fgIsTPT)
            {
          VDEC_SetTPTFlag(0,_rSwdmxMkvInfo.fgIsTPT);
        }
      
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
        //prRangeList->u4CurPTS = 
        //prRangeList->prDmxRangeInfo->t_vid_start_render_pts;

        //for trick play track
        #ifdef DIVX_PLUS_CER
        if(prRangeList->prDmxRangeInfo->pt_next_scale)
        {
            pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)prRangeList->prDmxRangeInfo->pt_next_scale->pt_idxtbl_lst;
            while (pBuldIdxTbl != NULL)
            {
                if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_KEY)
                {
                    _rSwdmxMkvInfo.prTPTVidKeyIdxTbl = pBuldIdxTbl;
                    prKeyTbl = pBuldIdxTbl;
                    _rSwdmxMkvInfo.u4TPTNumOfKeyEntry = prKeyTbl->ui4_number_of_entry;
                    while(prKeyTbl->pt_next_partial)
                    {
                        prKeyTbl = prKeyTbl->pt_next_partial;
                        _rSwdmxMkvInfo.u4TPTNumOfKeyEntry += prKeyTbl->ui4_number_of_entry;
                    }
                    break;
                }
                pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)pBuldIdxTbl->pv_next_tbl;
            }
            _rSwdmxMkvInfo.u8TPTFileOffset = 
                prRangeList->prDmxRangeInfo->pt_next_scale->ui8_fileoffset;
            _rSwdmxMkvInfo.u8TPTRangeSz = 
                prRangeList->prDmxRangeInfo->pt_next_scale->z_range_sz;
        }
        #endif
    }
    else
    {
        LOG(0, "Demuxer Range is NULL\n");
    }
}


BOOL _SWDMX_MKVSetDecoderInfo(UINT8 u1Type, 
    UINT32 u4Para1, 
    UINT32 u4Para2, 
    UINT32 u4Para3)
{    
    #ifdef LINUX_TURNKEY_SOLUTION
	UCHAR *pucFdrBuf = NULL;
	UCHAR *pucHdrBuf = NULL;
	#endif
    if(u1Type == eSWDMX_SET_VIDEOTYPE) 
    {
        #if defined(CC_MKV_MPEG1_DISABLE) && defined(CC_MKV_MPEG2_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MKV_MPEG4_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MPEG4)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MKV_DX311_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_DX311)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MKV_H264_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_H264)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MKV_VC1_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_VC1)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MKV_MJPEG_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MJPEG)
        {
            return FALSE;
        }
        else
        #endif
        {
            _rSwdmxMkvInfo.u4VDecType =  u4Para1;
            return TRUE;
        }
    }
    else if (u1Type == eSWDMX_SET_VIDSEQHEADER)
    {
        _rSwdmxMkvInfo.u4MainVidHdrLength = u4Para2;
        #ifndef LINUX_TURNKEY_SOLUTION
        _rSwdmxMkvInfo.u4MainVidHeaderPtr = u4Para1;
        #else
        if(u4Para1 != 0 && u4Para2 != 0)
	    {   if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
                FEEDER_E_OK)
                {
                  LOG(7, "Feeder input buffer error.\n");
                  return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                }
	        pucFdrBuf = (UCHAR*)VIRTUAL(_rFeederInfo.u4StartAddr);
            if(VIRTUAL(_rFeederInfo.u4EndAddr) <= ((UINT32)pucFdrBuf + (UINT32)u4Para2))
            {
                LOG(7, "Feeder buffer not enough.\n");
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
            pucHdrBuf = (UCHAR*)u4Para1;
             x_memcpy((VOID*)pucFdrBuf, (VOID*)pucHdrBuf, (UINT32)u4Para2);
             _rSwdmxMkvInfo.u4MainVidHeaderPtr=(UCHAR*)VIRTUAL(_rFeederInfo.u4StartAddr);
        }
        else
        {
            _rSwdmxMkvInfo.u4MainVidHeaderPtr = 0;
        }
	    #endif
        //_rSwdmxMkvInfo.u8MkvSegmentInfoTimeCodeScale = (u4Para3 * 9) / 100000;
        //LOG(1, "SetSegScale %d\n", _rSwdmxMkvInfo.u8MkvSegmentInfoTimeCodeScale);
        return TRUE;
    }
    else if (u1Type == eSWDMX_SET_TPT_VIDSEQHEADER)
    {
        _rSwdmxMkvInfo.u4TPTVidHeaderPtr = u4Para1;
        _rSwdmxMkvInfo.u4TPTVidHdrLength = u4Para2;
        return TRUE;
    }    
    else if (u1Type == eSWDMX_SET_AUDIOTYPE)
    {
        //mkv test
        if(u4Para1 < 0xFF)
        {
            _rSwdmxMkvInfo.u4ADecType[_rSwdmxMkvInfo.u4AStrmID] = u4Para1;
        }
        if(u4Para2 > 0)
        {
            _rSwdmxMkvInfo.u4AudSampleRate[_rSwdmxMkvInfo.u4AStrmID] = 
                _AACSampFreqIdx(u4Para2);
        }
        if(u4Para3 > 0)
        {
            _rSwdmxMkvInfo.u4AudChannelNs[_rSwdmxMkvInfo.u4AStrmID] = u4Para3;
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL _SWDMX_MKVGetInfo(UINT8 u1Type,
    UINT32* pu4Para1,
    UINT32* pu4Para2,
    UINT32* pu4Para3)
{    
    UINT64 u8StcValue=0;
	UINT64 *pu8Tmp;
	INT8 i;
    if(u1Type == eSWDMX_GET_CURRENT_POSITION)
    {
        if(pu4Para1 == NULL)
        {
        	LOG(1, "_SWDMX_MKVGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
		u8StcValue=STC_GetStcValue();
		pu8Tmp = (UINT64*)pu4Para1;
		*pu8Tmp=0;
		for(i=9;i>=0;i--)
		{
          if(_rSwdmxMkvInfo.au8PostionxTime[i][0]!=0 && _rSwdmxMkvInfo.au8PostionxTime[i][1]!=0 )
          	{
          	  if(_rSwdmxMkvInfo.au8PostionxTime[i][1]<=u8StcValue)
          	  	{
                 *pu8Tmp= _rSwdmxMkvInfo.au8PostionxTime[i][0];
				  break;
			    }
          	}
		}
        
        return TRUE;
    }
else if (u1Type == eSWDMX_GET_IS_TRICK)
    {
        if (_rSwdmxMkvInfo.prVidKeyIdxTbl)
        {
            if (_rSwdmxMkvInfo.prVidKeyIdxTbl->ui4_number_of_entry < MKV_KEY_TABLE_THRESHOLD)
            {
                *pu4Para1 = FALSE;
            }
            else
                *pu4Para1 = TRUE;
        }
        else
        {
            *pu4Para1 = FALSE;
        }
    }
    return TRUE;	
}

BOOL _SWDMX_MKVSetStrmID(UINT8 u1HdrType,  
    UINT32 u4StrmID, UINT32 u4StrmSubID)
{
    UNUSED(u4StrmSubID);
    
    switch(u1HdrType)
    {
        case eSWDMX_STRM_TYPE_VID:
            _rSwdmxMkvInfo.u4VStrmID = u4StrmID;
            LOG(5, "_SWDMX_MKVSetStrmID Vid = 0x%08x\n", u4StrmID);
            break;
        case eSWDMX_STRM_TYPE_AUD:
            if(u4StrmID < MKV_MAX_AUD_TRACK)
            {
                _rSwdmxMkvInfo.u4AStrmID = u4StrmID;
                LOG(5, "_SWDMX_MKVSetStrmID Aud = 0x%08x\n", u4StrmID);
            }
            else
            {
                LOG(5, "_SWDMX_MKVSetStrmID Aud = 0x%08x Fail!\n", u4StrmID);
            }
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            _rSwdmxMkvInfo.u4SpStrmID = u4StrmID;
            LOG(5, "_SWDMX_MKVSetStrmID Sub = 0x%08x\n", u4StrmID);
            break;
        default:
            return FALSE;
    }

    return TRUE;

}


SWDMX_HDR_PRS_STATE_T _SWDMX_MKVHdrParse(
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange)
{
    UINT32 u4ConsumedSize = 0, u4TargetPTS = 0;
    #ifdef AUD_PREBUF_EN
    UINT32 u4Idx, u4APts;
    #endif
    FEEDER_REQ_DATA_T rFeederReqData;
    VDEC_HDR_INFO_T rHdrInfo;
    BOOL fgSeek = FALSE, fgAudTrigVidDec = FALSE;
    static BOOL fgWaitStartAddr = FALSE;
    UINT8 u1TrickNomalSwitch = 0;
    
    x_memset((void*)&rFeederReqData, 0, sizeof(FEEDER_REQ_DATA_T));
    if (prRangeList->prDmxRangeInfo == NULL)
    {
        LOG(5, "This range is empty\n");
        return eSWDMX_HDR_PRS_EMPTY_RANGE;
    }

    //check if init after stop
    if(!_rSwdmxMkvInfo.fgSeqHdrParsed)
    {
        if(!_vMkvInfoInit())
        	{
              return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
		    }
        //===  Request data for the first time, parsing private data
        if ((_rSwdmxMkvInfo.u8CurDmxMoviPos == 0) 
            && ((_rSwdmxMkvInfo.u4MainVidHeaderPtr > 0) && (_rSwdmxMkvInfo.i4PrevSpeed == NORMAL_1X)))
        {
            if(_rSwdmxMkvInfo.fgEnVideo && !_rSwdmxMkvInfo.fgEnVDmx)
            {
                UNUSED(_SWDMXInitDmx(eSWDMX_MEDIA_VIDEO, NULL, FALSE));
                _rSwdmxMkvInfo.fgEnVDmx = TRUE;
            }
            _vMkvVidHdrParsing(_rSwdmxMkvInfo.u4MainVidHeaderPtr, _rSwdmxMkvInfo.u4MainVidHdrLength);
        }
        _rSwdmxMkvInfo.fgSeqHdrParsed = TRUE;
    }

    if (_rSwdmxMkvInfo.fgBeginToPlay &&
        i4PlaySpeed == NORMAL_1X)
    {   
    	   if(_rSwdmxMkvInfo.fgEnAudio && !_rSwdmxMkvInfo.fgEnADmx)
       {
            //audio pre-buffer setting, in case "SetRange" is called after EnableStrm
            UNUSED(_SWDMXInitDmx(eSWDMX_MEDIA_AUDIO, NULL, FALSE));
            _rSwdmxMkvInfo.fgEnADmx = TRUE;      
         }
        if(_rSwdmxMkvInfo.fgEnVideo && !_rSwdmxMkvInfo.fgEnVDmx)
        {
            UNUSED(_SWDMXInitDmx(eSWDMX_MEDIA_VIDEO, NULL, FALSE));
            _rSwdmxMkvInfo.fgEnVDmx = TRUE;
        }
        STC_StopStc();
        STC_SetStcValue(0);
        u4TargetPTS = 0;
        VDEC_SetRenderFromPts(0, _rSwdmxMkvInfo.u4CurVidPts);
        _rSwdmxMkvInfo.fgBeginToPlay = FALSE;
        fgAudTrigVidDec = TRUE;

        //resume stop
        if(prRangeList->prDmxRangeInfo->t_vid_start_render_pts != 0)
        {
            fgSpeedChange = TRUE;
            u8SeekPos = (UINT64)prRangeList->prDmxRangeInfo->t_vid_start_render_pts;
        }
		else if(prRangeList->prDmxRangeInfo->t_aud_start_render_pts!= 0)
			{
               _rSwdmxMkvInfo.u8CurDmxMoviPos=(UINT64)prRangeList->prDmxRangeInfo->t_start_pos.ui8_frame_position;
			   _rSwdmxMkvInfo.u8CurMoviPos=(UINT64)prRangeList->prDmxRangeInfo->t_start_pos.ui8_frame_position;
			   AUD_SetStartPts(0, (UINT64)prRangeList->prDmxRangeInfo->t_aud_start_render_pts);
		}
        #ifdef AUD_PREBUF_EN
        if((_rSwdmxMkvInfo.u1FF2xTo1xMode == AUD_PRE_BUF) && 
            _rSwdmxMkvInfo.fgEnAudio)
        {
            DMX_AudHandler_SetActivePidx(_rSwdmxMkvInfo.u4AStrmID + MKV_AUD_PREBUF_IDX);
            fgActivePidxIsUnKnown = FALSE;
        }
        #endif
        #if 0//CC_DLNA_SUPPORT
        FeederSetReadyThreshold(8*1024*1024);
        FeederSetUnderFlowThreshold(1*1024*1024);
        while(!FeederIsReady(FEEDER_PROGRAM_SOURCE))
        {
            if(!_bReqDataFromFeeder((UINT64)(_rSwdmxMkvInfo.u8CurMoviPos + 
                _rSwdmxMkvInfo.u8MainFileOffset),
                _rSwdmxMkvInfo.u8RangeSz - 
                _rSwdmxMkvInfo.u8CurMoviPos))
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
            //=== Wait feeder ack
            if (_rSwdmxMkvInfo.i4ReqReadTimes > 0)
            {
                if(!_bGetDataFromFeederAck(&rFeederReqData))
                {
                    return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                }
            }
            if(_rSwdmxMkvInfo.fgReadEOS)
            {
                FeederSetInfo(FEEDER_SWDMX_EOS,TRUE);
                break;
            }
        }
        #endif
    }

    //=== Check if end of range
    if ((!fgSpeedChange) && 
        ((_rSwdmxMkvInfo.u8CurDmxMoviPos >= _rSwdmxMkvInfo.u8RangeSz) ||
        ((_rSwdmxMkvInfo.u8CurMoviPos >= _rSwdmxMkvInfo.u8RangeSz) && (_rSwdmxMkvInfo.fgIsUnderFlw))))
    {
        if (_rSwdmxMkvInfo.fgEnVideo && !_rSwdmxMkvInfo.fgDeliverVEos)
        {
            LOG(6, "Cur DmxMoviPos = %ld, range size = %ld\n", 
                _rSwdmxMkvInfo.u8CurDmxMoviPos,
                _rSwdmxMkvInfo.u8RangeSz);
            
            LOG(5, "Deliver Video Eos...\n");
            if(_rSwdmxMkvInfo.fgAudTrigVidDec)
            {
                _SWDMX_MkvAudioCb(ENUM_SWDMX_AUDIO_NOTIFY_NO_DATA);  
            }
            if (!_SWDMX_SendMkvEos(eSWDMX_MEDIA_VIDEO))
            {
                LOG(5, "Fail\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            _rSwdmxMkvInfo.fgDeliverVEos = TRUE;
        }

        if (_rSwdmxMkvInfo.fgEnAudio && !_rSwdmxMkvInfo.fgDeliverAEos)
        {   

       #ifdef AUD_PREBUF_EN
        if (_rSwdmxMkvInfo.u1FF2xTo1xMode == AUD_PRE_BUF)
        {
            if(fgActivePidxIsUnKnown == TRUE)
            {
                DMX_AudHandler_SetActivePidx(_rSwdmxMkvInfo.u4AStrmID + MKV_AUD_PREBUF_IDX);
            }   
        }
       #endif 
            LOG(5, "Deliver Audio Eos...\n");
            if (!_SWDMX_SendMkvEos(eSWDMX_MEDIA_AUDIO))
            {
                LOG(5, "Fail\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            _rSwdmxMkvInfo.fgDeliverAEos = TRUE;
		    AUD_MM_Set_Avsync_event(AUD_DEC_MAIN);
        }
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        return eSWDMX_HDR_PRS_SUCCEED;
    }

    //reset range status
    prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    //=== Check feeder status
    if (_rSwdmxMkvInfo.fgFeederInvalid)
    {
        LOG(3, "Feeder is invalid now\n");
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }
        
    //=== speed change handler
    if(fgSpeedChange)
    {    
        if(_rSwdmxMkvInfo.fgAudTrigVidDec)
        {
            _SWDMX_MkvAudioCb(ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE);
        }
        //_rSwdmxMkvInfo.fgPendingAudMoveData = FALSE;
        x_memset(&_rSwdmxMkvInfo.fgPendingAudMoveData, 0, sizeof(_rSwdmxMkvInfo.fgPendingAudMoveData));
        // Reset some values for state change.
        if (_rSwdmxMkvInfo.fgDeliverVEos)
        {
            UINT32 u4Flag;
            DMX_MM_T rDmxMMInfo;

            rDmxMMInfo.fgEnable = TRUE;
            rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;

            switch (_rSwdmxMkvInfo.u4VDecType)
            {
                case ENUM_SWDMX_VDEC_DX311: case ENUM_SWDMX_VDEC_WMV7:
                case ENUM_SWDMX_VDEC_WMV8: case ENUM_SWDMX_VDEC_WMV9:
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
            
            _rSwdmxMkvInfo.fgDeliverVEos = FALSE;
        }
        if (_rSwdmxMkvInfo.fgDeliverAEos)
        {
            _rSwdmxMkvInfo.fgDeliverAEos = FALSE;
            AUD_ClearEOSFlag(0);
        }
        
        #ifdef AUD_PREBUF_EN
        DMX_AudHandler_SetActivePidx(MKV_AUD_UNKNOWN_IDX);
        fgActivePidxIsUnKnown = TRUE;
        #endif
        
        // seek
        u4TargetPTS = VDP_GetPts(0);    //default value
        if(((UINT32)(u8SeekPos>>32) != (UINT32)0xFFFFFFFF)
            || ((UINT32)(u8SeekPos) != (UINT32)0xFFFFFFFF))
        {
            u4TargetPTS = (UINT32)u8SeekPos;
            _rSwdmxMkvInfo.fgIsSeek = TRUE;
            _rSwdmxMkvInfo.u8AudSeekPts = u8SeekPos;
            if(_rSwdmxMkvInfo.fgEnVideo)
            {
                _rSwdmxMkvInfo.fgWaitIFrm = TRUE;
            }
            fgSeek = TRUE;
            fgAudTrigVidDec = TRUE;
            // set stc
            STC_StopStc();
            if((u8SeekPos == 0) || (!_rSwdmxMkvInfo.fgEnVideo))
            {
                STC_SetStcValue(u8SeekPos);
            }//else will set in vdec
        }
        //normal (1x, 2x) to trick
        else if(((i4PlaySpeed > FF_2X) || IsBackward(i4PlaySpeed)) 
            && 
            (cal_abs(_rSwdmxMkvInfo.i4PrevSpeed) <= FF_2X))
        {
            u4TargetPTS = VDP_GetPts(0);
            u1TrickNomalSwitch = NORMAL_2_TRICK;
            //query display struct
            if(_rSwdmxMkvInfo.u1DispStruct == 0)
            {
                x_memset((void*)&rHdrInfo, 0, sizeof(rHdrInfo));
                if(VDEC_QueryInfo(ES0, &rHdrInfo))
                {
                    if(rHdrInfo.fgProgressiveSeq)
                    {
                        _rSwdmxMkvInfo.u1DispStruct = DISP_PROG;
                    }
                    else
                    {
                        _rSwdmxMkvInfo.u1DispStruct = DISP_INTERLACE;
                    }
                }
            }
        }
        //trick to normal (1x)
        else if((((_rSwdmxMkvInfo.i4PrevSpeed >= FF_2X) || 
            IsBackward(_rSwdmxMkvInfo.i4PrevSpeed)) ||
            (_rSwdmxMkvInfo.i4PrevSpeed == STEP_FW)) 
            && 
            (cal_abs(i4PlaySpeed) <= FF_2X))
        {
            if((_rSwdmxMkvInfo.i4PrevSpeed == FF_2X) ||
                (_rSwdmxMkvInfo.i4PrevSpeed == STEP_FW))
            {
                if(_rSwdmxMkvInfo.u1FF2xTo1xMode == FLUSH_VFIFO)
                {
                    VDEC_SetRenderFromPts(0, 0);  //set DEC_FLG_WAIT_NEXT_I, stop put frame to dispQ
                    VDEC_ReleaseDispQ(0);              //clr dispQ
                    if((_rSwdmxMkvInfo.i4PrevSpeed == FF_2X) && _rSwdmxMkvInfo.fgEnVideo)
                    {
                        STC_StopStc();  //continue in VDEC_IsNotDisplay, in case large target/key diff 
                    }                    
                }
                u4TargetPTS = VDP_GetPts(0) + 45000;    //pts for vdp trig audio
            }
            else
            {
                STC_SetStcValue(u4TargetPTS);   //current display PTS
                _rSwdmxMkvInfo.u8SeekPts = (UINT64)u4TargetPTS;
            }
            if(i4PlaySpeed == NORMAL_1X)
            {
                _rSwdmxMkvInfo.u8AudSeekPts = (UINT64)u4TargetPTS;
                fgAudTrigVidDec = TRUE;
            }

            u1TrickNomalSwitch = TRICK_2_NORMAL;
            if((_rSwdmxMkvInfo.i4PrevSpeed == FF_2X) && (i4PlaySpeed == NORMAL_1X))
            {
                _rSwdmxMkvInfo.i4LastSpeed = FF_2X;
                if((_rSwdmxMkvInfo.u1FF2xTo1xMode == KEEP_VFIFO) ||
                    (_rSwdmxMkvInfo.u1FF2xTo1xMode == AUD_PRE_BUF))
                {
                    u1TrickNomalSwitch = 0;
                    fgAudTrigVidDec = FALSE;
                }
            }
        }
        else if(((_rSwdmxMkvInfo.i4PrevSpeed > STEP_FW) && 
            (_rSwdmxMkvInfo.i4PrevSpeed < NORMAL_1X)) &&
            (i4PlaySpeed == NORMAL_1X))   //SF to 1X
        {
            VDEC_SetRenderFromPts(0, 0);  //set DEC_FLG_WAIT_NEXT_I, stop put frame to dispQ
            VDEC_ReleaseDispQ(0);              //clr dispQ
            u4TargetPTS = VDP_GetPts(0) + 10000;    //pts for vdp trig audio
            _rSwdmxMkvInfo.u8AudSeekPts = (UINT64)u4TargetPTS;
            fgAudTrigVidDec = TRUE;
            u1TrickNomalSwitch = TRICK_2_NORMAL;
        }

        _rSwdmxMkvInfo.i4PrevSpeed = i4PlaySpeed;                              //current speed
        
        //TPT
        #ifdef DIVX_PLUS_CER
        if((i4PlaySpeed >= FF_2X) || 
            IsBackward(i4PlaySpeed))
        {
            _rSwdmxMkvInfo.fgIsTPT = _SWDMXMkvSwitchTrack(TRUE);
            if(_rSwdmxMkvInfo.fgIsTPT)
            {
                fgSeek = TRUE;
                if(i4PlaySpeed <= FF_8X)
                {
                    STC_SetSyncMode(AV_SYNC_MODE_VIDEO_MASTER);
                }
            }
        }
        else
        {
            if(_rSwdmxMkvInfo.fgIsTPT)
            {
                fgSeek = TRUE;
            }
            _rSwdmxMkvInfo.fgIsTPT = _SWDMXMkvSwitchTrack(FALSE);
        }
        #endif
        
        //seek and reset feeder
        if((u1TrickNomalSwitch > 0) || fgSeek)  //except 1x to 2x
        {
            //flush buffer before "set vdp trigger aud pts" and "set render pts"
            if(_rSwdmxMkvInfo.u8CurDmxMoviPos > 0)  //in case of resume stop, will seek at beginning, sps/pps is still in Q
            {
                MPV_FlushEsmQ(0, TRUE);
                DMX_MM_FlushBuffer(eSWDMX_MEDIA_VIDEO);
            }
            DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO);

            #ifdef AUD_PREBUF_EN
            //clear pre-buffered audio
            if(_rSwdmxMkvInfo.u1FF2xTo1xMode == AUD_PRE_BUF)
            {
                for (u4Idx = 0; u4Idx < MKV_MAX_AUD_TRACK; u4Idx++)
                {
                    if(!DMX_AudHandler_SeekAndUpdate(u4Idx + MKV_AUD_PREBUF_IDX,
                    0xFFFFFFFF, &u4APts))
                    {
                        LOG(2, "Mkv DMX_AudHandler Flush %d\n", u4Idx);
                    }
                }
            }
            #endif
            
            //TPT
            #ifdef DIVX_PLUS_CER
            if(_rSwdmxMkvInfo.fgIsTPT)
            {
                _vMkvVidHdrParsing(_rSwdmxMkvInfo.u4TPTVidHeaderPtr, _rSwdmxMkvInfo.u4TPTVidHdrLength);
            }
            else
            {
                _vMkvVidHdrParsing(_rSwdmxMkvInfo.u4MainVidHeaderPtr, _rSwdmxMkvInfo.u4MainVidHdrLength);
            }
            #endif
            
            //seek
            if(_bSeekTime(u4TargetPTS, prRangeList))
            {
                LOG(5, "TargetPTS 0x%x, SeekPos 0x%x\n", u4TargetPTS, (UINT32)u8SeekPos);

                if(_rSwdmxMkvInfo.fgEnVideo)
                {
                    if(!IsBackward(i4PlaySpeed))
                    {
                        VDEC_SetRenderFromPts(0, u4TargetPTS);
                    }
                    else
                    {   //common part will wet as u4TargetPTS
                        VDEC_SetRenderPts(0, 0);
                    }
                }                
                //will clr all frames in DPB @ EsFlwCtrl by setting DEC_FLG_WAIT_NEXT_I
                //will not display frm by checking Vdec_IsNotDisplay
                VDEC_ReleaseDispQ(0);
                //if(fgSeek)
                {  _rSwdmxMkvInfo.fgIsSeek = TRUE;  // start parsing pos is cluster offset, not key
                    _rSwdmxMkvInfo.u8SeekPts = _rSwdmxMkvInfo.u4CurVidPts; //start send vid data
                }
            }
            else    //no key table
            {
                _rSwdmxMkvInfo.fgIsSeek = FALSE;
                
                //will clr all frames in DPB @ EsFlwCtrl by setting DEC_FLG_WAIT_NEXT_I
                //will not display frm by checking Vdec_IsNotDisplay
                VDEC_ReleaseDispQ(0);
                STC_SetStcValue(0);
                _rSwdmxMkvInfo.u8AudSeekPts = 0;
                return eSWDMX_HDR_PRS_SUCCEED;
            }
        }         
    }

    // trigger video dec when audio start to decode(by callback function)
    // avoid VDP_trigger audio aout before aud start to decode
    if((fgAudTrigVidDec && 
        _rSwdmxMkvInfo.fgEnVideo) &&
        (_rSwdmxMkvInfo.fgEnAudio &&
        (_rSwdmxMkvInfo.u4ADecType[_rSwdmxMkvInfo.u4AStrmID] > ENUM_SWDMX_ADEC_UNKNOWN)))
    {
        LOG(1, "VidPause by Swdmx\n");
        VDEC_Pause(ES0);
        _rSwdmxMkvInfo.fgAudTrigVidDec = TRUE;
        //_rSwdmxMkvInfo.u4AudTrigVidDecPTS = u4TargetPTS;
    }

    //===  Request data for the first time, pre-request
    if ((((_rSwdmxMkvInfo.u8CurMoviPos == 0) || fgSeek) ||(u1TrickNomalSwitch == TRICK_2_NORMAL)) && 
        ((((i4PlaySpeed <= NORMAL_1X)&&(i4PlaySpeed >= STEP_FW)) || (i4PlaySpeed == FF_2X) || 
        (_rSwdmxMkvInfo.fgIsTPT)) && (_rSwdmxMkvInfo.i4ReqReadTimes == 0)))
    {
        _rSwdmxMkvInfo.ePrsState = MKVPRS_G_SEGMENT_ELEMENT_ID;
        _u4LastAlignOffset = 0;
        if(!_bReqDataFromFeeder((UINT64)(_rSwdmxMkvInfo.u8CurMoviPos + 
            _rSwdmxMkvInfo.u8FileOffset),
            (UINT32)(_rSwdmxMkvInfo.u8RangeSz - 
            _rSwdmxMkvInfo.u8CurMoviPos)))
        {
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }
        fgWaitStartAddr = TRUE;
        _rSwdmxMkvInfo.u4CurDmxFeederRPtr = _rFeederMkvInfo.u4WriteAddr;
    }
    //=== Wait feeder ack
    if (_rSwdmxMkvInfo.i4ReqReadTimes > 0)
    {
        if(!_bGetDataFromFeederAck(&rFeederReqData))
        {
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }
        else
        {
            LOG(6, "Ack 0x%x--0x%x, ofst 0x%x\n", 
                rFeederReqData.u4WriteAddr, 
                rFeederReqData.u4WriteAddr + rFeederReqData.u4ReadSize, 
                _u4LastAlignOffset);
            _rSwdmxMkvInfo.fgIsUnderFlw = FALSE;
            if(fgWaitStartAddr)
            {
                _rSwdmxMkvInfo.u4CurDmxFeederRPtr = rFeederReqData.u4WriteAddr + _u4LastAlignOffset;
                fgWaitStartAddr = FALSE;
            }
            if(_rSwdmxMkvInfo.u8CurMoviPos >= _rSwdmxMkvInfo.u8RangeSz)
            {
                _rSwdmxMkvInfo.fgReadEOS = TRUE;
            }
            else
            {
                _rSwdmxMkvInfo.fgReadEOS = FALSE;
            }
        }        
    }
    
    //=== trick mode, 4X/8X/...
    if(IsBackward(i4PlaySpeed) || 
        ((_rSwdmxMkvInfo.fgIsTPT && (i4PlaySpeed > FF_8X)) ||
        ((!_rSwdmxMkvInfo.fgIsTPT) && (i4PlaySpeed > FF_2X))))
    {
        return _MKVCuePointParsing(prRangeList, i4PlaySpeed);
    }
    else
    {
        //=== Normal play, FF_2X, SF
        //=== Request data
        _rSwdmxMkvInfo.u4KeyFrmPTS = 0;
        if((_rSwdmxMkvInfo.u4LeftBytesInFeeder < (MKV_BUFFER_SIZE - MIN_READ_SIZE)) &&
            (_rSwdmxMkvInfo.u8RangeSz > _rSwdmxMkvInfo.u8CurMoviPos))
        {
            if(!_bReqDataFromFeeder((UINT64)(_rSwdmxMkvInfo.u8CurMoviPos + 
                _rSwdmxMkvInfo.u8FileOffset),
                (UINT32)(_rSwdmxMkvInfo.u8RangeSz - 
                _rSwdmxMkvInfo.u8CurMoviPos)))
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
        }

        if(_rSwdmxMkvInfo.u4LeftBytesInFeeder > 0)
        {
            //=== Sequential parsing from cluster
            _vMKVClusterParsing((UCHAR*)(VIRTUAL(_rSwdmxMkvInfo.u4CurDmxFeederRPtr)),
                prRangeList,
                _rSwdmxMkvInfo.u4LeftBytesInFeeder,
                &u4ConsumedSize);
            LOG(6, "Prs 0x%x--0x%x\n", _rSwdmxMkvInfo.u4CurDmxFeederRPtr, 
                _rSwdmxMkvInfo.u4CurDmxFeederRPtr + u4ConsumedSize);
                    
            //mkv test
            x_thread_delay(10);
            
            //=== Update rptr
            _rSwdmxMkvInfo.u8CurDmxMoviPos += u4ConsumedSize;
            if(_rSwdmxMkvInfo.u4LeftBytesInFeeder < u4ConsumedSize)
            {
                LOG(1, "Parse over available size!, %d\n", 
                    (u4ConsumedSize - _rSwdmxMkvInfo.u4LeftBytesInFeeder));
                u4ConsumedSize = _rSwdmxMkvInfo.u4LeftBytesInFeeder;
                _rSwdmxMkvInfo.u4LeftBytesInFeeder = 0;
            }
            else
            {
                _rSwdmxMkvInfo.u4LeftBytesInFeeder -= u4ConsumedSize;
            }
            
            if ((_rSwdmxMkvInfo.u4CurDmxFeederRPtr + u4ConsumedSize) >= 
                _rFeederMkvInfo.u4EndAddr)
            {
                _rSwdmxMkvInfo.u4CurDmxFeederRPtr =  _rFeederMkvInfo.u4StartAddr + 
                    ((_rSwdmxMkvInfo.u4CurDmxFeederRPtr + u4ConsumedSize) -_rFeederMkvInfo.u4EndAddr);
            }
            else
            {
                _rSwdmxMkvInfo.u4CurDmxFeederRPtr += u4ConsumedSize;
            }
            
            FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, _rSwdmxMkvInfo.u4CurDmxFeederRPtr);
        }
        else
        {
            _rSwdmxMkvInfo.u8CurDmxMoviPos = _rSwdmxMkvInfo.u8RangeSz;
        }
    }

    return eSWDMX_HDR_PRS_SUCCEED;
}


BOOL _SWDMX_MKVEnableStrm(UINT8 u1StrmType, VOID* pfnCbFunc)
{    
    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            LOG(5, "_SWDMX_MKVEnableStrm Vid\n");
            _rSwdmxMkvInfo.fgEnVideo = TRUE;
            //if (_rSwdmxMkvInfo.fgEnVDmx == FALSE)
            //{
            //    UNUSED(_SWDMXInitDmx(eSWDMX_MEDIA_VIDEO, NULL, FALSE));
             //   _rSwdmxMkvInfo.fgEnVDmx = TRUE;
            //
            //}
            break;
        case eSWDMX_STRM_TYPE_AUD:
            LOG(5, "_SWDMX_MKVEnableStrm Aud\n");
            //change to set range for audio-prebuffer parameter
            /*if (_rSwdmxMkvInfo.fgEnADmx == FALSE)
            {
                UNUSED(_SWDMXInitDmx(eSWDMX_MEDIA_AUDIO, NULL, FALSE));
                _rSwdmxMkvInfo.fgEnADmx = TRUE;
            }*/

            if ((!_rSwdmxMkvInfo.fgBeginToPlay) &&
                _rSwdmxMkvInfo.i4PrevSpeed == 1000)
            {
                _rSwdmxMkvInfo.fgChgATrack = TRUE;
            }
            _rSwdmxMkvInfo.fgEnAudio = TRUE;
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            if (_rSwdmxMkvInfo.fgEnSubTitle)
            {
                _rSwdmxMkvInfo.fgChgSbTrack = TRUE;
            }
            if (_rSwdmxMkvInfo.fgEnSubDmx == FALSE) //&&
               // (pfnCbFunc))
            {
                UNUSED(_SWDMXInitDmx(eSWDMX_MEDIA_SUBTITLE, pfnCbFunc, FALSE));
                _rSwdmxMkvInfo.fgEnSubDmx = TRUE;
            }
            _rSwdmxMkvInfo.fgEnSubTitle = TRUE;
            LOG(5, "EnableStrm Sub\n");
           break;
        default:
            return FALSE;
    }

    return TRUE;

}


BOOL _SWDMX_MKVDisableStrm(UINT8 u1StrmType)
{
    DMX_MM_T rPid;

    x_memset((void*)&rPid, 0, sizeof(DMX_MM_T));
    
    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            _rSwdmxMkvInfo.fgEnVideo = FALSE;
            if(_rSwdmxMkvInfo.fgIsAudWaitAout)
            {  
                if(fgAudioNnotifyDecodeDone)
                {
                    AUD_MMAoutEnable(0, TRUE);
                    _rSwdmxMkvInfo.fgIsAudWaitAout = FALSE;
                    STC_StartStc();
                }
            }
            LOG(5, "_SWDMX_MKVDisableStrm Vid\n");
            break;
        case eSWDMX_STRM_TYPE_AUD:
            _rSwdmxMkvInfo.fgEnAudio = FALSE;
            DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO);
            _rSwdmxMkvInfo.fgPendingAudMoveData[_rSwdmxMkvInfo.u4CurAStrmID] = FALSE;
            _rSwdmxMkvInfo.fgDeliverAEos= FALSE;
            #ifdef AUD_PREBUF_EN
            if(_rSwdmxMkvInfo.u1FF2xTo1xMode == AUD_PRE_BUF)
            {
                DMX_AudHandler_SetActivePidx(MKV_AUD_UNKNOWN_IDX);
                fgActivePidxIsUnKnown = TRUE;
            }
            #endif            
            LOG(5, "_SWDMX_MKVDisableStrm Aud, flush\n");
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            if (_rSwdmxMkvInfo.fgEnSubDmx)
            {
                _rSwdmxMkvInfo.fgEnSubDmx = FALSE;
                VERIFY(DMX_MM_Set(eSWDMX_MEDIA_SUBTITLE, DMX_MM_FLAG_VALID, &rPid));
                DMX_MM_Free(eSWDMX_MEDIA_SUBTITLE);
            }
            _rSwdmxMkvInfo.fgEnSubTitle = FALSE;
            LOG(5, "DisableStrm Sub\n");
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_MKVStop(void)
{
    DMX_MM_T rPid;
    FEEDER_REQ_DATA_T rFeederReqData;
    #ifdef AUD_PREBUF_EN
    UINT32 u4Idx;
    #endif
    
    x_memset((void*)&rFeederReqData, 0, sizeof(FEEDER_REQ_DATA_T));
    //rPid.fgEnable = FALSE;

    // wait feeder ack
    /*while(_rSwdmxMkvInfo.i4ReqReadTimes > 0)
    {
        //_rSwdmxMkvInfo.i4ReqReadTimes--;
        if(!_bGetDataFromFeederAck(&rFeederReqData))
        {
            return FALSE;
        }
    }*/
    
    if (_rSwdmxMkvInfo.fgEnVideo && !_rSwdmxMkvInfo.fgDeliverVEos)
    {
        if (!_SWDMX_SendMkvEos(eSWDMX_MEDIA_VIDEO))
        {
            LOG(3, "Fail to send Video Eos\n");
            //return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
        }
        _rSwdmxMkvInfo.fgDeliverVEos = TRUE;
    }

    if (_rSwdmxMkvInfo.fgEnAudio && !_rSwdmxMkvInfo.fgDeliverAEos)
    {
        if (!_SWDMX_SendMkvEos(eSWDMX_MEDIA_AUDIO))
        {
            LOG(3, "Fail to send Audio Eos\n");
            //return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
        }
        _rSwdmxMkvInfo.fgDeliverAEos = TRUE;
    }
    
    if(_rSwdmxMkvInfo.fgEnADmx)
    {
        VERIFY(DMX_MM_Set(eSWDMX_MEDIA_AUDIO, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(eSWDMX_MEDIA_AUDIO);

        #ifdef AUD_PREBUF_EN
        for (u4Idx = 0; u4Idx < MKV_MAX_AUD_TRACK; u4Idx++)
        {
            if(_rSwdmxMkvInfo.fgAudPreBuf[u4Idx])
            {
                VERIFY(DMX_MM_Set(MKV_AUD_PREBUF_IDX + u4Idx, DMX_MM_FLAG_VALID, &rPid));
                DMX_MM_Free(MKV_AUD_PREBUF_IDX + u4Idx);
                DMX_AudHandler_Free(MKV_AUD_PREBUF_IDX + u4Idx);
            }
        }
        DMX_AudHandler_SetEnable(FALSE);
        #endif
    }
    
    if (_rSwdmxMkvInfo.fgEnVideo == TRUE)
    {
        _rSwdmxMkvInfo.fgEnVideo = FALSE;
        VERIFY(DMX_MM_Set(eSWDMX_MEDIA_VIDEO, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(eSWDMX_MEDIA_VIDEO);

    }
    
    x_memset((void*)&_rSwdmxMkvInfo, 0, sizeof(SWDMX_MKV_INFO_T));
    x_memset((void*)&_rFeederMkvInfo, 0, sizeof(FEEDER_BUF_INFO_T));
    
    return TRUE;

}


BOOL _SWDMX_MKVFlush(void)
{
    //_SWDMX_FlushReqMsgQ();
    //_rSwdmxMkvInfo.i4ReqReadTimes = 0;
    
    return TRUE;
}

BOOL _SWDMX_MkvAudioCb(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{  
    if(eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE)
    {
        fgAudioNnotifyDecodeDone = TRUE;
        if(_rSwdmxMkvInfo.fgEnVideo)
        {
            if(_rSwdmxMkvInfo.fgAudTrigVidDec)
            {
                switch((ENUM_SWDMX_VDEC_T)_rSwdmxMkvInfo.u4VDecType)
                {
                    case ENUM_SWDMX_VDEC_MPEG1_MPEG2:
                        VDEC_Play(ES0, VDEC_FMT_MPV);
                    break;
                    case ENUM_SWDMX_VDEC_DX311:
                    case ENUM_SWDMX_VDEC_MPEG4:
                        VDEC_Play(ES0, VDEC_FMT_MP4);
                    break;
                    case ENUM_SWDMX_VDEC_H264:
                        VDEC_Play(ES0, VDEC_FMT_H264);
                    break;
                    case ENUM_SWDMX_VDEC_VC1:
                    case ENUM_SWDMX_VDEC_WMV7:
                    case ENUM_SWDMX_VDEC_WMV8:
                    case ENUM_SWDMX_VDEC_WMV9:
                        VDEC_Play(ES0, VDEC_FMT_WMV);                    
                    break;
                    default:
                        LOG(1, "VidPlay by AudCb fail, Unknow VidType %d!\n", _rSwdmxMkvInfo.u4VDecType);
                    return TRUE;
                }
                if(_rSwdmxMkvInfo.u8AudSeekPts == 0)
                {
                    LOG(3,"VidPlay by AudCb, vdp trig aud PTS 0x%x\n", _rSwdmxMkvInfo.u4AudTrigVidDecPTS);
                    VDP_TriggerAudReceive(_rSwdmxMkvInfo.u4AudTrigVidDecPTS);
                }
                else
                {
                    LOG(1, "AudCb before feeding data!\n");
                    AUD_MMAoutEnable(0, TRUE);  // for EOS, aud will not notify EOS if Aout Disable 
                }
                _rSwdmxMkvInfo.fgAudTrigVidDec = FALSE;
            }
            _rSwdmxMkvInfo.fgIsAudWaitAout = TRUE;
            if(_rSwdmxMkvInfo.u8AudSeekPts == 0)
             {
                 VDP_TriggerAudReceive(_rSwdmxMkvInfo.u4AudTrigVidDecPTS);
             }
           
        }
        else
        {
            STC_StartStc();
            AUD_MMAoutEnable(0, TRUE);
            _rSwdmxMkvInfo.fgIsAudWaitAout = FALSE;
        }
    }
    else if(eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_NO_DATA)
    {
        if(_rSwdmxMkvInfo.fgEnVideo)
        {
            if(_rSwdmxMkvInfo.fgAudTrigVidDec)
            {
                switch((ENUM_SWDMX_VDEC_T)_rSwdmxMkvInfo.u4VDecType)
                {
                    case ENUM_SWDMX_VDEC_MPEG1_MPEG2:
                        VDEC_Play(ES0, VDEC_FMT_MPV);
                    break;
                    case ENUM_SWDMX_VDEC_DX311:
                    case ENUM_SWDMX_VDEC_MPEG4:
                        VDEC_Play(ES0, VDEC_FMT_MP4);
                    break;
                    case ENUM_SWDMX_VDEC_H264:
                        VDEC_Play(ES0, VDEC_FMT_H264);
                    break;
                    case ENUM_SWDMX_VDEC_VC1:
                    case ENUM_SWDMX_VDEC_WMV7:
                    case ENUM_SWDMX_VDEC_WMV8:
                    case ENUM_SWDMX_VDEC_WMV9:
                        VDEC_Play(ES0, VDEC_FMT_WMV);                    
                    break;
                    default:
                        LOG(1, "VidPlay by AudCb fail, Unknow VidType %d!\n", _rSwdmxMkvInfo.u4VDecType);
                    return TRUE;
                }
                _rSwdmxMkvInfo.fgAudTrigVidDec = FALSE;
            }
        }
    }
    else
    {
        LOG(1, "AudCb Type Unknow %d\n", (UINT32)eAudioNotifyType);
    }
    return TRUE;
}

BOOL _SWDMX_MkvReadBuffer(ENUM_SWDMX_STRM_TYPE_T eStrmType, 
    SWDMX_READ_INFO_T *prReadInfo)
{
    BOOL  fgRet = FALSE;
    
    switch (eStrmType)
    {
        case eSWDMX_STRM_TYPE_SUBTITLE:
            LOG(3, "Mkv Get Subtitle Buffer serial number = 0x%x Frame Address = 0x%08x",
                prReadInfo->u1SerialNumber,
                prReadInfo->u4FrameAddr);
                
            fgRet = DMX_MM_GetBuffer(eSWDMX_MEDIA_SUBTITLE, 
                prReadInfo->u1SerialNumber, 
                (UINT32)prReadInfo->u4FrameAddr,
            0, (UINT32)prReadInfo->u4CopyLen,
            prReadInfo->pucDest);

            if(!fgRet)
            {
                LOG(3, "_SWDMX_MkvReadBuffer, get buffer error\n");
            }
            break;
        default:
            return FALSE;
    }

    return fgRet;
}

BOOL _SWDMX_MkvReleaseBuffer(ENUM_SWDMX_STRM_TYPE_T eStrmType, 
    SWDMX_READ_INFO_T *prReadInfo)
{
    BOOL  fgRet = FALSE;
    switch (eStrmType)
    {
        case eSWDMX_STRM_TYPE_SUBTITLE:
            fgRet = DMX_MM_UnlockBuffer( eSWDMX_MEDIA_SUBTITLE , 
                prReadInfo->u1SerialNumber, 
                (UINT32)prReadInfo->u4CopyLen,
                (UINT32)prReadInfo->u4FrameAddr);

            if(!fgRet)
            {
                LOG(3, "_SWDMX_MkvReadBuffer, unlock buffer error\n");
            }
            
            break;
        default:
            return FALSE;
    }
    
    return fgRet;
}

static BOOL _SWDMXMkvPrsDrmInfo(UCHAR* pucBitstream, UINT32 u4BufLength)
{
    UCHAR *pucBuf = (UCHAR*)VIRTUAL((UINT32)pucBitstream);
    
    if (u4BufLength < DIVX_DRM_LENGTH)
    {
        LOG(5, "Wrong Length for DivX DRM\n");
        return FALSE;
    }

    _rSwdmxMkvInfo.fgDrmEnable = TRUE;
    _rSwdmxMkvInfo.u2FrameKeyIdx = (UINT16)((pucBuf[0]) |
                                                                         (pucBuf[1] << 8));

    _rSwdmxMkvInfo.u4EncryptOffset = (pucBuf[2] |
    								(pucBuf[3] << 8) | 
    								(pucBuf[4] << 16) | 
    								(pucBuf[5] << 24));


    _rSwdmxMkvInfo.u4EncryptLength = (pucBuf[6] | 
    								 (pucBuf[7] << 8) |
    								 (pucBuf[8] << 16) | 
    								 (pucBuf[9] << 24));

    return TRUE;
}


static BOOL _SWDMXMkvDecDrmData(UINT8 u1HdrType, UCHAR* pucFifo, UINT32 u4BlkSz)
{
    DMX_MM_DRM_AES_T rDRM = {0};
    DMX_AES_DATA_T rAESData = {0};
    DIVX_DRM_CRYPTO_INFO_T *prDrmInfo;
    UINT32 u4Offset;
    
    if (_rSwdmxMkvInfo.prDmxRangeInfo->b_drm)
    {        
        rDRM.fgCBC = FALSE;
        rDRM.u4KeyBitLen = MKV_DIVX_DRM_AES_SIZE_128;

        prDrmInfo = 
            _rSwdmxMkvInfo.prDmxRangeInfo->t_drm_info_drv.u.t_divx_drm_info_drv.pt_divx_drm_crypto_info;

        if ((_rSwdmxMkvInfo.fgDrmEnable) && (u1HdrType == eSWDMX_MEDIA_VIDEO) &&
            (_rSwdmxMkvInfo.u4EncryptLength != 0) &&
            (_rSwdmxMkvInfo.u2FrameKeyIdx < 128) &&
            ((_rSwdmxMkvInfo.u4EncryptOffset + _rSwdmxMkvInfo.u4EncryptLength) < u4BlkSz))
        {
            u4Offset = _rSwdmxMkvInfo.u4EncryptOffset;
            rDRM.u4EncryLen = _rSwdmxMkvInfo.u4EncryptLength;
            
            x_memcpy(rDRM.au1Key, 
                prDrmInfo->aui1_frame_keys[_rSwdmxMkvInfo.u2FrameKeyIdx],
                (MKV_DIVX_DRM_AES_SIZE_128/8));
        }
        else if (u1HdrType == eSWDMX_MEDIA_AUDIO &&
            (prDrmInfo->ui1_flag & DIVX_DRM_AUDIO_PROTECTION))
        {
            if (u4BlkSz < prDrmInfo->ui1_protected_audio_offset)
            {
                LOG(2, "Audio DRM offset is larger than Audio Chunk Size Impossible\n");
                return FALSE;
            }
            else
            {
                if (u4BlkSz < prDrmInfo->ui1_protected_audio_offset + prDrmInfo->ui1_protected_audio_crypto_size)
                {
                    rDRM.u4EncryLen = u4BlkSz - prDrmInfo->ui1_protected_audio_offset;
                }
                else
                {
                    rDRM.u4EncryLen = prDrmInfo->ui1_protected_audio_crypto_size;
                }
                u4Offset = prDrmInfo->ui1_protected_audio_offset;
                
                x_memcpy(rDRM.au1Key, 
                    prDrmInfo->aui1_frame_keys[0],
                    (MKV_DIVX_DRM_AES_SIZE_128/8));
            }
        }
        else
        {
            LOG(2, "DRM type/param err!\n");
            return FALSE;
        }

        if(!DMX_AES_SetDecryptInfo(DMX_MM_DRM_FLAG_ALL, &rDRM))
        {
            LOG(2, "DMX_AES_SetDecryptInfo fail\n");
        }

        rAESData.u4Size = rDRM.u4EncryLen;

        rAESData.u4SrcBufStart = _rFeederMkvInfo.u4StartAddr;
        rAESData.u4SrcBufEnd = _rFeederMkvInfo.u4EndAddr;
        rAESData.u4SrcStart = (UINT32)pucFifo + u4Offset;

        if(rAESData.u4SrcStart >= _rFeederMkvInfo.u4EndAddr)
        {
            rAESData.u4SrcStart = 
                (rAESData.u4SrcStart - _rFeederMkvInfo.u4EndAddr) + _rFeederMkvInfo.u4StartAddr;
        }
        rAESData.u4DstBufStart = rAESData.u4SrcBufStart;
        rAESData.u4DstBufEnd = rAESData.u4SrcBufEnd;
        rAESData.u4DstStart = rAESData.u4SrcStart;
        if(!DMX_AES_MoveData(&rAESData))
        {
            LOG(2, "DMX_AES_MoveData fail\n");
        }
        return TRUE;
    }
    return FALSE;
}


