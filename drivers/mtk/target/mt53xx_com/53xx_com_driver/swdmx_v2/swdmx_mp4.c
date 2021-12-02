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
 * $Author: xin.liao $
 * $Date: 2012/11/26 $
 * $RCSfile: swdmx_mp4.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_mp4.c
 *  Software demux for mp4 - internal interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "swdmx_mp4.h"
#include "vdec_drvif.h"
#include "u_midxbuld.h"
#include "swdmx_debug.h"
#include "aud_drvif.h"

#if defined(CC_MT5387) || defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5396) ||defined(CC_MT5368) ||defined(CC_MT5389)
#include "b2r_drvif.h"
#include "b2r_if.h"
#endif


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define AAC_FAKE_HDR_SIZE 7
#define AAC_ALLOC_SIZE   (256)

#define MIN_READ_SIZE     (512* 1024)
#define MIN_READ_SIZE_NO_VIDEO (192*1024)
#define MP4_BUFFER_SIZE (235 * 7 * 1024)
#define MP4_KEY_TABLE_THRESHOLD (3)
#define MP4_ONE_SEC_DURATION (90000 * 2)
#define FPS_PRECISION (1000)

#ifdef LINUX_TURNKEY_SOLUTION
#define SWDMX_MP4_VIDEO_HEADER_ALLOC_SIZE (512)
#endif

#define H264_STARTCODE_LEN (3)

#define HAN_MVC_HEADER
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#undef min
#define min(x, y) ((x < y) ? x : y)
#undef max
#define max(x, y) ((x > y) ? x : y)
#undef abs
#define abs(x)                  (((x) >= 0) ? (x) : -(x))

#define SWDMX_MP4_VERIFY_NULL(ptr)                                    \
          do {                                              \
            if (ptr == NULL) { \
                LOG(3, "SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
                return; }      \
          } while (0)

#define SWDMX_MP4_VERIFY_NULL_ERROR(ptr)                                    \
          do {                                              \
            if (ptr == NULL) { \
                LOG(3, "SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
                return FALSE; }      \
          } while (0)


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#if 0
static SWDMX_MP4_INFO_T prSwdmxInst->rFmtInfo.rSwdmxMp4Info;

static FEEDER_BUF_INFO_T _rFeederInfo;
static UCHAR *_pucAudBuf = 0;

static MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T _rPrevSample = {0};
#ifdef LINUX_TURNKEY_SOLUTION
static UCHAR* _pucMp4VidHeaderPtr = 0;   
#endif
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static SWDMX_HDR_PRS_STATE_T _SWDMX_Mp4RequestData(SWDMX_INFO_T* prSwdmxInst);

static SWDMX_HDR_PRS_STATE_T  _SWDMX_BeforeDeliverUseIdx(SWDMX_INFO_T* prSwdmxInst,
    UINT8 u1HdrType);

static SWDMX_HDR_PRS_STATE_T _SWDMX_Mp4HdrPrsUseIdx(
    SWDMX_INFO_T* prSwdmxInst,
    INT32 i4Speed,
    SWDMX_RANGE_LIST_T* prRangeList);

static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverMp4DataUseIdx(
    SWDMX_INFO_T* prSwdmxInst,
    INT32 i4Speed,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl, 
    UINT32* pu4CurIdx, 
    UINT8 u1HdrType);

static SWDMX_HDR_PRS_STATE_T _SWDMX_Mp4HdrPrsDataType(
    SWDMX_INFO_T* prSwdmxInst,
    UINT64 u8Offset,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl, 
    UINT32* pu4CurIdx, 
    UINT8 u1HdrType);

static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverMp4Keyframe(
    SWDMX_INFO_T* prSwdmxInst,
    INT32 i4Speed,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl, 
    UINT32* pu4CurIdx, 
    UINT8 u1HdrType,
    SWDMX_RANGE_LIST_T* prRangeList);
static BOOL _SWDMX_InitDmx(
    SWDMX_INFO_T* prSwdmxInst,
    UINT8 u1HdrType, 
    VOID* pfnCbFunc,
    BOOL fgEos);

static BOOL _SWDMX_SendMp4Eos(SWDMX_INFO_T* prSwdmxInst,
    UINT8 u1HdrType);

static UINT32 _SWDMX_Mp4Seek(SWDMX_INFO_T* prSwdmxInst,
    UINT64 u8SeekTime, 
    SWDMX_RANGE_LIST_T* prRangeList, 
    INT32 i4PlaySpeed);
static UINT32 _SWDMX_Mp4SeekCurPos(SWDMX_INFO_T* prSwdmxInst);

static BOOL _SWDMX_Mp4GetDataFromFeeder(SWDMX_INFO_T* prSwdmxInst,
    FEEDER_REQ_DATA_T *prFeederData);

static BOOL _SWDMX_Mp4GetDataFromFeederNoWait(SWDMX_INFO_T* prSwdmxInst,
    FEEDER_REQ_DATA_T *prFeederData);

static BOOL _SWDMX_Mp4CalculateRepeatFrame(SWDMX_INFO_T* prSwdmxInst, INT32 i4PlaySpeed);

static BOOL _SWDMX_Mp4ResetFeeder(SWDMX_INFO_T* prSwdmxInst);

static void _SWDMX_Mp4SetFeederInvalid(SWDMX_INFO_T* prSwdmxInst, BOOL fgFlag);

static void _SWDMX_Mp4VideoWakeUp(SWDMX_INFO_T* prSwdmxInst);

static BOOL _SWDMX_Mp4CheckNextKeyTable(SWDMX_INFO_T* prSwdmxInst,INT32 i4Speed,UINT32* pu4CurIdx);

BOOL _SWDMX_Mp4SetDecoderInfo(UINT8 u1SrcId,
    UINT8 u1Type, 
    UINT32 u4Para1, 
    UINT32 u4Para2, 
    UINT32 u4Para3)
{
    SWDMX_INFO_T* prSwdmxInst = NULL;
    SWDMX_MP4_INFO_T *prSwdmxMp4Info = NULL;
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    prSwdmxMp4Info = (SWDMX_MP4_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMp4Info;
    SWDMX_MP4_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "%s source id = %d\n", __FUNCTION__, u1SrcId);

    if(u1Type == eSWDMX_SET_VIDEOTYPE) 
    {
        #if defined(CC_MP4_MPEG1_DISABLE) && defined(CC_MP4_MPEG2_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MP4_MPEG4_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MPEG4)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MP4_DX311_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_DX311)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MP4_H264_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_H264)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MP4_VC1_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_VC1)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MP4_MJPEG_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MJPEG)
        {
            return FALSE;
        }
        else
        #endif
        {
            prSwdmxMp4Info->u4VDecType =  u4Para1;
            return TRUE;
        }
    }
    else if (u1Type == eSWDMX_SET_VIDSEQHEADER)
    {
#ifndef LINUX_TURNKEY_SOLUTION
       prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHeaderPtr = u4Para1;
       prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHdrLength = u4Para2;
 #else
       UINT32 u4Align = 16;
       if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHeaderPtr == 0)
       {
           prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHdrLength = u4Para2;
           _SWDMX_GetInfo(u1SrcId,eSWDMX_GET_INTERNAL_BUF, &prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHeaderPtr, &u4Para2, &u4Align);
       
		       if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHeaderPtr)
		       {
		           x_memcpy((VOID*)VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHeaderPtr), 
               (VOID*)VIRTUAL(u4Para1), 
		               prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHdrLength);
		       }
		       else
		       {
		           LOG(0,"Header Seq Buffer alloc fail\n");
		       }
		   }
#ifdef HAN_MVC_HEADER
       else if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHeaderSecPtr == 0)  
       {
       	   prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHdrSecLength = u4Para2;
           _SWDMX_GetInfo(u1SrcId,eSWDMX_GET_INTERNAL_BUF, &prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHeaderSecPtr, &u4Para2, &u4Align);
       
		       if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHeaderSecPtr)
		       {
		           x_memcpy((VOID*)VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHeaderSecPtr), 
		               (VOID*)VIRTUAL(u4Para1), 
		               prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHdrSecLength);
		       }
		       else
		       {
		           LOG(0,"Header Seq Buffer alloc fail\n");
		       }
       }  
 #endif      
 #endif
        return TRUE;
    }
    else if (u1Type == eSWDMX_SET_AUDIOTYPE)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4ADecType = u4Para1;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4AudSampleRate = u4Para2;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4AudChannelNs = u4Para3;
        return TRUE;
    }
    
    return FALSE;
}

BOOL _SWDMX_Mp4GetInfo(UINT8 u1SrcId,
    UINT8 u1Type,
    UINT32* pu4Para1,
    UINT32* pu4Para2,
    UINT32* pu4Para3)
{
    UINT64 *pu8Tmp;

    SWDMX_INFO_T* prSwdmxInst = NULL;
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MP4_VERIFY_NULL_ERROR(prSwdmxInst);        
    LOG(5, "_SWDMX_Mp4GetInfo u1Type = %d\n", u1Type);

    LOG(5, "%s source id = %d\n", __FUNCTION__, u1SrcId);

	if(u1Type == eSWDMX_GET_CURRENT_POSITION)
    {
        if(pu4Para1 == NULL)
        {
        	LOG(1, "_SWDMX_Mp4GetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        pu8Tmp = (UINT64*)pu4Para1;
        *pu8Tmp = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8CurFilepos;
        return TRUE;
    }
    else if (u1Type == eSWDMX_GET_IS_TRICK)
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl != NULL)
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl->ui4_number_of_entry < MP4_KEY_TABLE_THRESHOLD)
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
                                                                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1AudPid);
        
        *pu4Para1 = u4AudFifoSize - u4DmxAvailSize;
        return TRUE;
    }
        
    return FALSE;
}

BOOL _SWDMX_Mp4VideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT32 u4Addr;
    
    SWDMX_INFO_T *prSwdmxInst = NULL;
    SWDMX_MP4_INFO_T *prSwdmxMp4Info = NULL;
    prSwdmxInst = (SWDMX_INFO_T*)prPes->pvInstanceTag;
    prSwdmxMp4Info = (SWDMX_MP4_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMp4Info;
    SWDMX_MP4_VERIFY_NULL_ERROR(prSwdmxInst);    
    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
    //rPesInfo.u4PTS = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4LastVidPts;
    rPesInfo.u4PTS = prPes->u4Pts;
    rPesInfo.u4DTS = prPes->u4Dts;
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    rPesInfo.fgDtsValid = prPes->fgPtsDts;
    rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo.fgGop = prPes->fgGop;
    rPesInfo.fgEos = prPes->fgEOS;
    rPesInfo.ucEsId = prSwdmxInst->u1SwdmxId;
    rPesInfo.u8Offset = 
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VidLatestOff;
    
    if(prSwdmxMp4Info->u4VDecType == (UINT32)ENUM_SWDMX_VDEC_MJPEG)
    {
        rPesInfo.ucMpvId = VLD1; // video format all use VLD expect jpeg
    }
    else
    {        
        rPesInfo.ucMpvId = VLD0; // video format all use VLD expect jpeg
    }
    // Pts is valid all the time
    rPesInfo.fgDtsValid = TRUE;
    #ifdef CC_VDEC_FMT_DETECT
    rPesInfo.u4DmxFrameType = prPes->u4FrameType;
    #endif

    LOG(11, " prPes->u4Pts = %x\n",  prPes->u4Pts );
   
    if (rPesInfo.fgEos)
    {
         if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4PrevSpeed < 0)
     	{
     		rPesInfo.fgBackward = TRUE;
     	}    
        rPesInfo.u4FifoStart = 0;
        rPesInfo.u4FifoEnd = 0;
        rPesInfo.u4VldReadPtr = 0;
    }

    if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VDecType == ENUM_SWDMX_VDEC_H264) &&
        !rPesInfo.fgEos)
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
        
        if(( rPesInfo.ucPicType & 0x1f) == 5)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4LastIPts = rPesInfo.u4PTS;  
        }
    }
    else if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VDecType == ENUM_SWDMX_VDEC_MPEG4) &&
        !rPesInfo.fgEos)
    {
        UCHAR *pucBuf;

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
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4LastIPts = rPesInfo.u4PTS;
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
        else if ((pucBuf[0] >= 0x20) &&
            (pucBuf[0] <= 0x2f))            
        {                
            rPesInfo.fgSeqHdr = TRUE;            
        }
    }
    else if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VDecType == (UINT32)ENUM_SWDMX_VDEC_MJPEG)
    {
        rPesInfo.ucPicType = I_TYPE;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4LastIPts = rPesInfo.u4PTS;
    }
    else if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VDecType == (UINT32)ENUM_SWDMX_VDEC_H263)
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
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4LastIPts = rPesInfo.u4PTS;
                break;
            case 2:
                rPesInfo.ucPicType = P_TYPE;
                break;
            default:
                rPesInfo.ucPicType = UNKNOWN_TYPE;
                break;
        }
    }

	rPesInfo.rExtra.fgValid=TRUE;
    rPesInfo.rExtra.u8PTSI = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4LastIPts;
	rPesInfo.rExtra.u8AudPTS=prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts;
    VDEC_SendEs((void*)&rPesInfo);
    
    return TRUE;
}


BOOL _SWDMX_Mp4AudioCallback(const DMX_AUDIO_PES_T* prPes)
{
    BOOL fgRet;
    DMX_AUDIO_PES_T rPes;

    SWDMX_INFO_T *prSwdmxInst = NULL;
    if(prPes != NULL)
    {
        prSwdmxInst = (SWDMX_INFO_T*)(prPes->pvInstanceTag);
        SWDMX_MP4_VERIFY_NULL_ERROR(prSwdmxInst);
    }
    else
    {
        LOG(1, "_SWDMX_Mp4AudioCallback prPes NULL\n");
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
BOOL _SWDMX_Mp4Init(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType)
{
    // Initialize feeder
    //FeederOpen(FEEDER_PROGRAM_SOURCE);
    SWDMX_INFO_T* prSwdmxInst = NULL;
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MP4_VERIFY_NULL_ERROR(prSwdmxInst);        
    UNUSED(eType);

    LOG(5, "%s source id = %d\n", __FUNCTION__, u1SrcId);
    //*********************************************************
    //FeederSetBufferSize(FEEDER_PROGRAM_SOURCE, 5*10*1024); 
    //FeederStart(FEEDER_PROGRAM_SOURCE);
#if 0
    FeederInstallCallback(prSwdmxInst->eFeederSrc, FEEDER_NOTIFY_DATA_REQ_DONE, 
    _SWDMX_FeederCallBack, (UINT32)&prSwdmxInst->rFeederInfo);
#endif
    //*********************************************************

#if 0
    //DMX_MM_DECODER_CALLBACKS_T rCallback; // Isaac: should be DMX_DECODER_CALLBACKS_T, Jack fix me
    DMX_DECODER_CALLBACKS_T rCallback;

    rCallback.pfnSendAudioPes = AUD_SendAudioPes;
    rCallback.pfnSendVideoPes = _SWDMX_Mp4VideoCallback;
    rCallback.pfnUpdateVideoWp = VDEC_SetWptr;
    DMX_SetDecoderCallbacks(&rCallback);
#endif

    // Default some values
    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4PrevAStrmID = 0xFFFFFFFF;
    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4AudSampleRate = 48000;
    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4AudChannelNs = 2;
    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts = 0xFFFFFFFF;
    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts <<= 32;
    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts += 0xFFFFFFFF;
    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8ASeekPts = 0xFFFFFFFF;
    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8ASeekPts <<= 32;
    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8ASeekPts += 0xFFFFFFFF;

    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TriggerAudPts = 0xFFFFFFFF;
    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgWaitNewRange = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurRepeatFrameIdx = 0xFFFFFFFF;
    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1AudPid = 0xFF;

    x_memset(&prSwdmxInst->rFmtInfo.rSwdmxMp4Info.rPrevSample, 0, sizeof(MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T));   
    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8PrevSampleOffset = 0;
    
    //DDI_BUF_ALIGNMENT == 16 and      
    //for _dmx_mm_movedata  Meet the alignment and packet size (188 bytes) requirements.
    if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4ADecType == ENUM_SWDMX_ADEC_AAC &&
        prSwdmxInst->pucAudBuf == 0)
    {
#if 0    
        prSwdmxInst->pucAudBuf = 
            (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(AAC_ALLOC_SIZE,16));
#else
        UINT32 u4RequestSize = AAC_ALLOC_SIZE;
        UINT32 u4Align = 16;
       _SWDMX_GetInfo(u1SrcId,eSWDMX_GET_INTERNAL_BUF, (UINT32*)prSwdmxInst->pucAudBuf, &u4RequestSize, &u4Align);            
#endif
        if(prSwdmxInst->pucAudBuf == 0)
        {
            LOG(1, "Allocate AAC buffer failed!\n");
            return FALSE;
        }
    }               

    DMX_SetToDecoder(TRUE);
    
    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgAudStrmDisable = FALSE;

    _SWDMX_Mp4SetFeederInvalid(prSwdmxInst, TRUE);
    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgRepeatRequest = TRUE;
    return TRUE;
}


/**
*/
VOID _SWDMX_Mp4SetRange(UINT8 u1SrcId,
        SWDMX_RANGE_LIST_T *prRangeList, 
        UINT32 u4VidStrmID, 
        UINT32 u4AudStrmID, 
        UINT32 u4SubTitleID)
{
    UINT8 u1ATrack;
    UINT8 u1SubTrack;
    
    SWDMX_INFO_T* prSwdmxInst = NULL;
    MM_RANGE_ELMT_IDXTBL_T *pBuldIdxTbl = NULL;
    u1ATrack = 0;
    u1SubTrack = 0;
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MP4_VERIFY_NULL(prSwdmxInst);        

    LOG(5, "%s source id = %d\n", __FUNCTION__, u1SrcId);
    if (prRangeList != NULL &&
       prRangeList->prDmxRangeInfo != NULL)
    {
        pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)prRangeList->prDmxRangeInfo->pt_idxtbl_lst;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prDmxRangeInfo = prRangeList->prDmxRangeInfo;
        while (pBuldIdxTbl != NULL)
        {
            if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_AUDIO &&
                u1ATrack < MP4_MAX_AUDIO_TRACK)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[u1ATrack] = pBuldIdxTbl;
                LOG(5, "_SWDMX_Mp4SetRange Audio Index table = 0x%08x\n",
                    (UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[u1ATrack]));
                u1ATrack++;
            }
            else if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_VIDEO)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl = pBuldIdxTbl;
                LOG(5, "_SWDMX_Mp4SetRange Video Index table = 0x%08x\n",
                    (UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl));
            }
            else if ((pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_SP)&&
            	      (u1SubTrack < MP4_MAX_SUB_TRACK))
            {
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[u1SubTrack] = pBuldIdxTbl;
                LOG(5, "_SWDMX_Mp4SetRange subtile Index table = 0x%08x\n",
                    (UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[u1SubTrack]));
                u1SubTrack++;
            }
            else if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_KEY)
            {
                MM_RANGE_ELMT_IDXTBL_T *prTmpVidKeyIdxTbl = pBuldIdxTbl;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurKeyTableIdx = 0;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4NumOfKeyTable = 0;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl = pBuldIdxTbl;

                while(prTmpVidKeyIdxTbl != NULL)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.parVidKeyIdxTblArray[(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4NumOfKeyTable)++] = prTmpVidKeyIdxTbl;
                    prTmpVidKeyIdxTbl = prTmpVidKeyIdxTbl->pt_next_partial;
                }
                
                LOG(5, "_SWDMX_Mp4SetRange Key Index table = 0x%08x\n",
                    (UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl));
            }
            
            pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)pBuldIdxTbl->pv_next_tbl;
            
           prSwdmxInst->rFmtInfo.rSwdmxMp4Info.ui4_data_offset = prRangeList->prDmxRangeInfo->ui8_fileoffset;
           prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8_range_sz  = prRangeList->prDmxRangeInfo->z_range_sz;
        }

        if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl)
        {
            // The upper layer (mw or playmgr should prevent 0 case for rate and scale)
            if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui4_scale)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidFps = 
                    (UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui4_rate/prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui4_scale);
            }

            // Add some error handling
            if (!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidFps ||
            	prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidFps > 60)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidFps = 30;
            }

            LOG(5, "prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidFps = %ld\n", prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidFps);
        }

        // Set the number of total audio tracks
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1TotalATrack = u1ATrack;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1TotalSubTrack = u1SubTrack;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgWaitNewRange = FALSE;
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
        
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TotalTime = prRangeList->prDmxRangeInfo->ui8_pb_duration;
    }
    else
    {
        LOG(1, "Demuxer Range is NULL\n");
    }
}


/**
*/
SWDMX_HDR_PRS_STATE_T _SWDMX_Mp4HdrPrs(
    UINT8 u1SrcId,
    SWDMX_RANGE_LIST_T* prRangeList,
    SWDMX_CUR_CMD_T* prCmd)
{
    INT32 i4PlaySpeed = prCmd->i4PlaySpeed;
    UINT64 u8SeekPos = prCmd->u8SeekPos;
    BOOL fgSpeedChange = prCmd->fgChange;
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl = NULL;
    INT32  i;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    if (!prSwdmxInst)
    {
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }

    if ((!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBeginToPlay) &&
        (i4PlaySpeed == 1000))
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prDmxRangeInfo->t_start_pos.ui8_pts_info!=0)
        {
             if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo) && 
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]))
            {
                START_POS_INFO_T *prStartPosInfo=&prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prDmxRangeInfo->t_start_pos;
				fgSpeedChange=TRUE;
                //prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prDmxRangeInfo->t_start_pos.ui8_pts_info;
                //prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgIsSeek = TRUE;
                prCmd->fgSeek=TRUE;
			    prCmd->u4SeekMode = SWDMX_CUR_CMD_SEEK_OFFSET;
				prCmd->rSeekOffset.ui8_i_pts_info=prStartPosInfo->ui8_pts_info;
				prCmd->rSeekOffset.ui8_pts_info=prStartPosInfo->ui8_pts_info;
				prCmd->rSeekOffset.ui8_i_frame_position=prStartPosInfo->ui8_i_frame_position;
				prCmd->rSeekOffset.ui8_frame_position=prStartPosInfo->ui8_frame_position;
		        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_SKIP;
				//return eSWDMX_HDR_PRS_SUCCEED;
				
            }
            // Video only
            else if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo && prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl)
            {
                _SWDMX_BeforeDeliverUseIdx(prSwdmxInst, eSWDMX_MEDIA_VIDEO);
               
                LOG(5, "Video Start Render Pts = %lld\n", 
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prDmxRangeInfo->t_start_pos.ui8_pts_info);
                prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl;
                VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId,prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prDmxRangeInfo->t_start_pos.ui8_pts_info);
                
                // set stc for starting playback
                STC_StopStc(prSwdmxInst->u1StcId);
                STC_SetStcValue(prSwdmxInst->u1StcId, prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prDmxRangeInfo->t_start_pos.ui8_pts_info - 45000);

                VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
                VDEC_Pause(prSwdmxInst->u1VdecId);
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause = TRUE;
				if (1)
                {
                    UINT32 u4AudPts = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prDmxRangeInfo->t_start_pos.ui8_pts_info;
                    AUD_SetStartPts(prSwdmxInst->u1AdecId,u4AudPts);
                    //VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);

                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TriggerAudPts = u4AudPts;
					prRangeList->u4CurPTS = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prDmxRangeInfo->t_start_pos.ui8_i_pts_info;
		            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_SKIP;
					prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBeginToPlay = TRUE;

		            return eSWDMX_HDR_PRS_SUCCEED;
				}
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prDmxRangeInfo->t_start_pos.ui8_pts_info;
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgIsSeek = TRUE;
                }
            }
            // Audio only
            else if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio) && 
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]))
            {
                prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack];
                AUD_SetStartPts(prSwdmxInst->u1AdecId, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts);
            }
            else
            {
                LOG(1, "SWDMX_Mp4 Unknow state for starting playback\n");
                return eSWDMX_HDR_PRS_FAIL;
            }
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBeginToPlay = TRUE;
        }
        else if (prRangeList->fgSetRenderPosition)
        {
             if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo) && 
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]))
            {
                _SWDMX_BeforeDeliverUseIdx(prSwdmxInst, prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid);
                UNUSED(_SWDMX_Mp4Seek(prSwdmxInst, prRangeList->u8StartPts, prRangeList, i4PlaySpeed));
                LOG(5, "Video Start Render Pts = %lld\n", 
                prRangeList->u8StartPts);

                if (prRangeList->fgIsSeekTarget)
                {
                    prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl;
                    VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prRangeList->u8StartPts);
                    
                    // set stc for starting playback
                    STC_StopStc(prSwdmxInst->u1StcId);
                    STC_SetStcValue(prSwdmxInst->u1StcId,prRangeList->u8StartPts - 45000);
                    if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK)
                    {
                        prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack];
                    }
                    AUD_SetStartPts(prSwdmxInst->u1AdecId, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts);
                    //VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);

                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TriggerAudPts = 
                        prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts;
                    VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
                    VDEC_Pause(prSwdmxInst->u1VdecId);
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause = TRUE;
               }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts = prRangeList->u8StartPts;
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgIsSeek = TRUE;
                }
            }
            // Video only
            else if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo && prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl)
            {
                _SWDMX_BeforeDeliverUseIdx(prSwdmxInst, prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid);
                UNUSED(_SWDMX_Mp4Seek(prSwdmxInst, prRangeList->u8StartPts, prRangeList, i4PlaySpeed));
                if(prRangeList->fgIsSeekTarget)
                {
                    prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl;
                    VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prRangeList->u8StartPts);
                    
                    // set stc for starting playback
                    STC_StopStc(prSwdmxInst->u1StcId);
                    STC_SetStcValue(prSwdmxInst->u1StcId, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeVidIdx].ui4_pts - 45000);

                    VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
                    VDEC_Pause(prSwdmxInst->u1VdecId);
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause = TRUE;
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts = prRangeList->u8StartPts;
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgIsSeek = TRUE;  
                }
            }
            // Audio only
            else if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio) && 
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]))
            {
                prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack];
                AUD_SetStartPts(prSwdmxInst->u1AdecId, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts);
            }
            else
            {
                LOG(1, "SWDMX_Mp4 Unknow state for starting playback\n");
                return eSWDMX_HDR_PRS_FAIL;
            }
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBeginToPlay = TRUE;
       }
        else
        {
            if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo) && 
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]))
            {
                prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl;
                VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeVidIdx].ui4_pts);
                
                // set stc for starting playback
                STC_StopStc(prSwdmxInst->u1StcId);
                STC_SetStcValue(prSwdmxInst->u1StcId, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeVidIdx].ui4_pts - 45000);

                prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack];
                AUD_SetStartPts(prSwdmxInst->u1AdecId, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts);
                //VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);

                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TriggerAudPts = 
                    prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts;
                VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
                VDEC_Pause(prSwdmxInst->u1VdecId);
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause = TRUE;
            }
            // Video only
            else if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo && prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl)
            {
                prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl;
                VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeVidIdx].ui4_pts);
                
                // set stc for starting playback
                STC_StopStc(prSwdmxInst->u1StcId);
                STC_SetStcValue(prSwdmxInst->u1StcId, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeVidIdx].ui4_pts - 45000);

                VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
                VDEC_Pause(prSwdmxInst->u1VdecId);
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause = TRUE;
            }
            // Audio only
            else if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio) && 
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]))
            {
                prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack];
                AUD_SetStartPts(prSwdmxInst->u1AdecId, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts);
            }
            else
            {
                LOG(1, "SWDMX_Mp4 Unknow state for starting playback\n");
                return eSWDMX_HDR_PRS_FAIL;
            }
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBeginToPlay = TRUE;
        }
    }     

    if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgChgATrack ||
        fgSpeedChange)
    {
        if ( prCmd->fgSeek || !( fgSpeedChange && ( prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4PrevSpeed > 0 && prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4PrevSpeed <= 2000 ) 
             && ( i4PlaySpeed > 0 && i4PlaySpeed <= 2000 ) ) )
        {
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgFeederInvalid = TRUE;
        }
		
        //prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4NumSentAud = 0;
        //prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4NumSentVid = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgAudIsReady = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockAud = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockVid = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverAEos = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgPartialRangeDone = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgHasAud = FALSE;
        if(!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio)
        {
        	 prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgChgATrack = FALSE;
        }
    }
    
    if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgChgATrack) && 
        (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1TotalATrack > 0) &&
        (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio) &&
        (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1TotalATrack <= MP4_MAX_AUDIO_TRACK))
    {
        BOOL fgFindAudio = FALSE, fgFindATrack = FALSE;
        UINT32 u4CurPts;
        MM_RANGE_ELMT_IDXTBL_T* prIdxTbl = NULL;   //current range

        fgFindATrack = FALSE;
        for (i = 0; (i < prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1TotalATrack) && (i < MP4_MAX_AUDIO_TRACK); i++)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[i] && 
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[i]->t_strm_id.u.t_stm_mp4_id == prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4AStrmID)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack = i;
                fgFindATrack = TRUE;
                break;
            }
        }
        if (!fgFindATrack)
        {
            LOG(3, "_SWDMX_Mp4EnableStrm didn't find audio stream\n");
            return eSWDMX_HDR_PRS_EMPTY_RANGE;
        }

        if (!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
        {
            u4CurPts = STC_GetStcValue(prSwdmxInst->u1StcId);
            // set audio range idx
            prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack];
            if(prIdxTbl)
            {
	            for(i = 1; i < prIdxTbl->ui4_number_of_entry; i++)
	            {
	                if((prIdxTbl->u.pt_pb_idx_tbl_entry[i-1].ui4_pts <= (u4CurPts+45000)) &&
	                    (prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts >= (u4CurPts+45000)))
	                {
	                    prRangeList->u4CurRangeAudIdx = i;
	                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts = prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts;
	                    LOG(5, "Cur Audio Index = %d Audio Track PTS = 0x%08x\n", 
	                        prRangeList->u4CurRangeAudIdx,
	                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts);
	                    fgFindAudio = TRUE;
	                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgChgATrack = FALSE;
				              AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts);
	                    break;
	                }
	            }
	            if(prIdxTbl->ui4_number_of_entry == 0)
                {
                	  prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgChgATrack = FALSE;
                }
	        }
	        else
	        {
	        	  prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgChgATrack = FALSE;
	        }
            if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgWaitNewRange)
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
            if (!fgFindAudio)
            {
                LOG(3, "Didn't find Audio\n");
                prRangeList->u4CurPTS = u4CurPts;
                if(!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
                {	
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgWaitNewRange = TRUE;
		        }
                prRangeList->eRangeStats = eSWDMX_RANGE_STATE_SKIP;
                return eSWDMX_HDR_PRS_SUCCEED;
            }
        }
        else
        {
            u4CurPts = VDP_GetPts(prSwdmxInst->u1B2rId);

            if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK)
            {
                // set audio range idx
                prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack];
                if(prIdxTbl)
                {
	                for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
	                {
	                    if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts >= (u4CurPts+45000))
	                    {
	                        prRangeList->u4CurRangeAudIdx = i;
	                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts = prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts;
	                        LOG(5, "Cur Audio Index = %d Audio Track PTS = 0x%08x\n", 
	                            prRangeList->u4CurRangeAudIdx,
	                            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts);
	                        fgFindAudio = TRUE;
	                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgChgATrack = FALSE;
	                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TriggerAudPts =  prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts;
	                        AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts);
	                        break;
	                    }
	                }
	                if(prIdxTbl->ui4_number_of_entry == 0)
	                {
	                	  prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgChgATrack = FALSE;
	                }
	            }
	            else
	            {
	            	  prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgChgATrack = FALSE;
	            }
            }
        }

        LOG(5, "Chang to Track %d Cur PTS = 0x%08x\n", 
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack,
            u4CurPts);
        //if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio)
        {
            DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1AudPid); 
        }
    }
    if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgChgSubTrack) && 
            (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1TotalSubTrack > 0) &&
            (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnSubTitle) &&
            (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1TotalSubTrack < MP4_MAX_SUB_TRACK))
    {
    
        BOOL fgFindSubTrack = FALSE;
        UINT32 u4CurPts = VDP_GetPts(prSwdmxInst->u1B2rId);
        for (i = 0; (i < prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1TotalSubTrack) && (i < MP4_MAX_SUB_TRACK); i++)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[i] && 
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[i]->t_strm_id.u.t_stm_mp4_id == prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4SubTitleID)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack = i;
                fgFindSubTrack = TRUE;
            }
        }
        if (!fgFindSubTrack)
        {
            LOG(3, "_SWDMX_Mp4EnableStrm didn't find audio stream\n");
            return eSWDMX_HDR_PRS_EMPTY_RANGE;
        }

        if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack < MP4_MAX_SUB_TRACK)
        {
            // set audio range idx
            prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack];
            for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
            {
                if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts >= (u4CurPts+45000))
                {
                    prRangeList->u4CurRangeSubIdx = i;
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgFeederInvalid = TRUE;
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgChgSubTrack = FALSE;
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgInvalidForSub = TRUE;
                    break;
                }
            }
            if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgChgSubTrack)
            {
                prRangeList->u4CurRangeSubIdx = prIdxTbl->ui4_number_of_entry;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgChgSubTrack = FALSE;
            }
        }
    }
    	
    if(fgSpeedChange)
    {
        UINT32 u4CurPts = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts <<= 32;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts += 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgWaitNewRange =  FALSE;  
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8ASeekPts = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8ASeekPts <<= 32;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8ASeekPts += 0xFFFFFFFF;
	    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgInvalidForSub = FALSE;
        if (i4PlaySpeed > 2000 || i4PlaySpeed < 0)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgStartTrick = TRUE;
        }

        if (prRangeList->eRangeStats == eSWDMX_RANGE_STATE_PENDING)
        {
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
        }

        if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgRecSeekFlag)&&(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4PrevSpeed == 1))
        {
             prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgSeekAfterPause = TRUE;
        } 
         
        if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause)
        {
            _SWDMX_Mp4VideoWakeUp(prSwdmxInst);
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause = FALSE;
        }
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame=0;
        u4CurPts = VDP_GetPts(prSwdmxInst->u1B2rId);
        LOG(5, "Mp4 Speed change while VDP Pts = 0x%08x\n", u4CurPts);

        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgSpeedChange = TRUE;

        // Reset some values for state change.
        if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos)
        {
            UINT32 u4Flag;
            DMX_MM_T rDmxMMInfo;

            rDmxMMInfo.fgEnable = TRUE;
            rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;

            switch (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VDecType)
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
            if (!DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid, u4Flag, &rDmxMMInfo))
            {
                LOG(3, "Fail to reset demuxer\n");
            }
            
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos = FALSE;
        }
        if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverAEos)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverAEos = FALSE;
            AUD_ClearEOSFlag(prSwdmxInst->u1AdecId);
        }
        if (i4PlaySpeed == 1000)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame = 0;
        }

        if((prCmd->fgSeek) &&
           (prCmd->u4SeekMode == SWDMX_CUR_CMD_SEEK_OFFSET))
        {
            u8SeekPos = prCmd->rSeekOffset.ui8_i_pts_info;         
        }     
        
        if(((UINT32)(u8SeekPos>>32) != 0xFFFFFFFF) || ((UINT32)(u8SeekPos) != 0xFFFFFFFF))
        {  
            if ((i4PlaySpeed < 4000) && (i4PlaySpeed > 0))
            {
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgIsSeek = TRUE;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgRecSeekFlag = TRUE;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurVidPts = 0;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts = 0;
                if((prCmd->fgSeek) &&
                    (prCmd->u4SeekMode == SWDMX_CUR_CMD_SEEK_OFFSET))
                {
                      prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts = prCmd->rSeekOffset.ui8_i_pts_info;
                      prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8ASeekPts = prCmd->rSeekOffset.ui8_pts_info;
                }
                else
                {
                      prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts = u8SeekPos;
                      prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8ASeekPts = u8SeekPos;
                }
                //should enable audio if parsing to audio entry larger than target PTS
                //if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio)
                {
                    DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1AudPid); 
                }
                if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
                {
                    DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid);
                }
            }
            else
            {
                _SWDMX_Mp4Seek(prSwdmxInst, u8SeekPos, prRangeList, i4PlaySpeed);
            }

            if(i4PlaySpeed >0)
            {    
                if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
                {
                   if((prCmd->fgSeek) &&
                    (prCmd->u4SeekMode == SWDMX_CUR_CMD_SEEK_OFFSET))
                    {
                        VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, (prCmd->rSeekOffset.ui8_pts_info+2000));
                    }
                    else
                    {
                        VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, (u8SeekPos+2000));
                    }
                }
            }
            else
            {    
                VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, 0);
            }
            
            if((prCmd->fgSeek) &&
             (prCmd->u4SeekMode == SWDMX_CUR_CMD_SEEK_OFFSET))
            {
                VDEC_StartPTS(prSwdmxInst->u1VdecId,FALSE,prCmd->rSeekOffset.ui8_pts_info,NULL);
            }
            else
            {
                VDEC_StartPTS(prSwdmxInst->u1VdecId,FALSE,u8SeekPos,NULL);
            }

            // set stc
            STC_StopStc(prSwdmxInst->u1StcId);
        }
        // 2X -> 1X
		else if(prSwdmxInst->ePlayMode==SWDMX_PLAY_MODE_NORMAL 
			&& prSwdmxInst->ePrePlayMode==SWDMX_PLAY_MODE_NORMAL)
        {
            LOG(5, "Mp4 Speed 2X -> 1X\n");
            
            if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4PrevSpeed == 1)
            {
                STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);
            }

            if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4PrevSpeed == 1) || 
            ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4PrevSpeed < 1000) && (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4PrevSpeed > 1)))
            {
                STC_StartStc(prSwdmxInst->u1StcId);
            }

			if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl==NULL
				&& i4PlaySpeed<=2000&&prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4PrevSpeed>2000)
			{
				_SWDMX_Mp4SeekCurPos(prSwdmxInst);
	
				DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1AudPid);
				
				if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
				{
					DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid);
				}
				
				MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
				VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);
				_SWDMX_FlushReqMsgQ(u1SrcId);
				VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u4CurPts);
				// set stc
				STC_StopStc(prSwdmxInst->u1StcId);
				STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);
	            if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgHasAud)
                {
                    AUD_SetStartPts(prSwdmxInst->u1AdecId,prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TriggerAudPts);
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockVid = TRUE;
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TriggerAudPts=u4CurPts;
                }
                else if(prIdxTbl)
                {
                    prRangeList->u4CurRangeAudIdx = prIdxTbl->ui4_number_of_entry;
                }
				
				prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgIsSeek = TRUE;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts = u4CurPts;
				//LOG(5, "_rSwdmxMp4Info.i4PrevSpeed != 1000 i4PlaySpeed != 2000\n");

			}
            else if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]) && 
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl)&&
                (i4PlaySpeed == 1000))
            {
                MM_RANGE_ELMT_IDXTBL_T *prIdxTbl = NULL;
                //if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio)
                {
                    DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1AudPid); 
                }
                // set audio range idx
                prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack];
                for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
                {
                    if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts >= (u4CurPts+45000))
                    {
                        prRangeList->u4CurRangeAudIdx = i;
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgHasAud = TRUE;
                        break;
                    }
                     else
                    {
                    	prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgHasAud = FALSE;
                    }
               }

                if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgHasAud)
                {
                    AUD_SetStartPts(prSwdmxInst->u1AdecId, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts);
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockVid = TRUE;
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TriggerAudPts = 
                        prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts;
                }
                else
                {
                    prRangeList->u4CurRangeAudIdx = prIdxTbl->ui4_number_of_entry;
                }
            }
        }
        else if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4PrevSpeed < 0 &&
            (i4PlaySpeed == 2000 || i4PlaySpeed == 1000))
        {
            LOG(5, "Rewind -> 2X/1X\n");

            _SWDMX_Mp4Seek(prSwdmxInst, u4CurPts, prRangeList, i4PlaySpeed);

            //if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio)
            {
                DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1AudPid); 
            }
            if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
            {
                DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid);
            }


            MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
            VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);

            _SWDMX_FlushReqMsgQ(u1SrcId);

            VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, 
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey].ui4_pts);

            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgIsSeek = TRUE;
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts = u4CurPts;
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurVidPts = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts = 0;
            // set stc
            STC_StopStc(prSwdmxInst->u1StcId);
            STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);
        }
        // else if NOT 1X -> 2X
        else if (!(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4PrevSpeed == 1000 && (i4PlaySpeed == 2000 || i4PlaySpeed == 1)))
        {
        	  if (!(((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4PrevSpeed == 1)&&(i4PlaySpeed == 2000))||
               ( (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4PrevSpeed == 2000 ||
               (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4PrevSpeed == 1) )&&
               ((i4PlaySpeed<1000)&&(i4PlaySpeed>1)))))
            {
                _SWDMX_Mp4Seek(prSwdmxInst, u4CurPts, prRangeList, i4PlaySpeed);

                //if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio)
                {
                    DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1AudPid); 
                }
                if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
                {
                    DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid);
                }

                MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
                VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);

                _SWDMX_FlushReqMsgQ(u1SrcId);

                if (i4PlaySpeed > 1000)
                {
                    if(u4CurPts < prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey].ui4_pts)
                    {
                        u4CurPts = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey].ui4_pts;
                    }
                    VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u4CurPts);
                }
                else if (i4PlaySpeed <= 1000 &&
                    i4PlaySpeed > 0)
                {
                    VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u4CurPts);
                }
                else
                {
                    VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, 
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts);
                }

                // Things for 32/16/8/4/-2/-4/-8/-16/-32 -> 1X are the same
                if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4PrevSpeed > 2000 || prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4PrevSpeed < 0) &&
                    ((i4PlaySpeed == 1000)||(i4PlaySpeed<=1000 && i4PlaySpeed>0)))
                {
                    LOG(5, "Trick -> normal PTS = 0x%08x\n", u4CurPts);

                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgIsSeek = TRUE;
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts = u4CurPts;
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurVidPts = 0;
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts = 0;
                }

                // set stc
                STC_StopStc(prSwdmxInst->u1StcId);
                STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);

                LOG(5, "_rSwdmxMp4Info.i4PrevSpeed != 1000 i4PlaySpeed != 2000\n");
                // don't need to handle 1X->2X
            }
        }

    }
    
    if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgIsSeek)
    {
	    if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl || (!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo))
	    {
            _SWDMX_Mp4Seek(prSwdmxInst, prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts, prRangeList, i4PlaySpeed);
	    }
		else
		{
		    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts=(UINT64)_SWDMX_Mp4SeekCurPos(prSwdmxInst);
		}

        if(prRangeList->fgIsSeekTarget)
        {
            LOG(5, "prRangeList->fgIsSeekTarget\n");

            prRangeList->fgIsSeekTarget = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgIsSeek = FALSE;

            // set stc for starting playback
            STC_StopStc(prSwdmxInst->u1StcId);
            //STC_SetStcValue(prSwdmxInst->u1StcId, prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts);

            //VDEC_SetRenderFromPts(0, prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8SeekPts);

            if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgHasAud))
            {
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockVid = TRUE;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockAud = FALSE;
                
                if(((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8ASeekPts>>32) == 0xFFFFFFFF)&&
                   ((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8ASeekPts) == 0xFFFFFFFF))
                {
                  prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TriggerAudPts = 
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts;                    

                  AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TriggerAudPts);
                }
            }
        }
        else
        {
            //need new range containing target PTS
            if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgWaitNewRange)
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
            LOG(0, "Seek: Need New Range\n");
            prRangeList->u4CurPTS = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts;
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_SKIP;
           // if(!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
            {	
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgWaitNewRange = TRUE;
            }
            return eSWDMX_HDR_PRS_SUCCEED;
        }
    }

    if((prRangeList->prDmxRangeInfo != NULL) &&
       (prRangeList->prDmxRangeInfo->pt_idxtbl_lst != NULL) &&
       (prRangeList->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry != NULL))
    {
        SWDMX_HDR_PRS_STATE_T eHdrPrsState;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4PrevSpeed = i4PlaySpeed;
        eHdrPrsState = _SWDMX_Mp4HdrPrsUseIdx(prSwdmxInst, i4PlaySpeed, prRangeList);
        if( prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos )
        {
            prSwdmxInst->fgRcvEOS = TRUE;
        }
        
        return eHdrPrsState;
    }
    else
    {
        
       if((prRangeList->prDmxRangeInfo != NULL) &&
         (prRangeList->prDmxRangeInfo->pt_idxtbl_lst != NULL)&&
         !prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos)        
       {
           if(prRangeList->prDmxRangeInfo->pt_idxtbl_lst->e_elem_type == MM_ELEMT_TYPE_AUDIO)
           if(_SWDMX_SendMp4Eos(prSwdmxInst, eSWDMX_MEDIA_AUDIO))
           {
               prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos = TRUE;
           }
       }
	    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4PrevSpeed = i4PlaySpeed;
        return eSWDMX_HDR_PRS_SUCCEED;
    }
}

static SWDMX_HDR_PRS_STATE_T _SWDMX_Mp4HdrPrsUseIdx(
        SWDMX_INFO_T* prSwdmxInst,
        INT32 i4Speed, 
        SWDMX_RANGE_LIST_T* prRangeList)
{
    BOOL fgRangeDone = FALSE;
    BOOL fgVidDone = TRUE;
    BOOL fgAudDone = TRUE;
    BOOL fgSubDone = TRUE;

    UINT64  u8VidOffset = 0xFFFFFFFF;
    UINT64  u8AudOffset = 0xFFFFFFFF;
    UINT64  u8SubOffset = 0xFFFFFFFF;
    UINT64  u8MinOffset = 0;
 
    UINT32 *pu4VidCurIdx  = NULL;
    UINT32 *pu4AudCurIdx = NULL;
    UINT32 *pu4SubCurIdx = NULL;

    MM_RANGE_ELMT_IDXTBL_T *pKeyIdxTbl = NULL; 
    u8VidOffset <<= 32;
    u8VidOffset += 0xFFFFFFFF;
    u8AudOffset <<= 32;
    u8AudOffset += 0xFFFFFFFF;
    u8SubOffset <<= 32;
    u8SubOffset += 0xFFFFFFFF;
    
    pKeyIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl; 

    if (!prRangeList)
    {
        LOG(3, "Range list is NULL\n");
        return eSWDMX_HDR_PRS_EMPTY_RANGE; 
    }

    //if ((i4Speed > 2000) || (i4Speed < 0))
     if(prSwdmxInst->ePlayMode != SWDMX_PLAY_MODE_NORMAL)
    {
        fgRangeDone = FALSE;
        if (pKeyIdxTbl != NULL && 
            (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey < pKeyIdxTbl->ui4_number_of_entry))
        {

            LOG(6, "pKeyIdxTbl->ui4_number_of_entry = %x\n", pKeyIdxTbl->ui4_number_of_entry);
            LOG(6, "prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey = %x\n", prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey);
            pu4VidCurIdx = &prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey;

            if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4NumSentVid == 0)
            {
                _SWDMX_BeforeDeliverUseIdx(prSwdmxInst, eSWDMX_MEDIA_VIDEO);
            }

            _SWDMX_DeliverMp4Keyframe(prSwdmxInst,
                i4Speed,
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl, 
                pu4VidCurIdx, 
                eSWDMX_MEDIA_VIDEO,
                prRangeList);
        }
        else if((pKeyIdxTbl != NULL) && (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey >= pKeyIdxTbl->ui4_number_of_entry)&&
                  (!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos))
        {
            if (!_SWDMX_SendMp4Eos(prSwdmxInst, eSWDMX_MEDIA_VIDEO))
            {
                    LOG(3, "Fail to send Video Eos\n");
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos = TRUE;
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        }
    }
    else
    {
        if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo) && (!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl))
        {
            LOG(3, "No video index table now!\n");
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;        
        }

        if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio) && 
            (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
            (!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]))
        {
            LOG(3, "No Audio index table now!\n");
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;        
        }

        if  ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio)&&
            (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
            (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]) &&
            (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry))
        {
            if(prRangeList->u4CurRangeAudIdx < prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->ui4_number_of_entry)
            {   
                fgAudDone = FALSE;    
                pu4AudCurIdx = &prRangeList->u4CurRangeAudIdx;
                u8AudOffset = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->ui8_base_offset +
                            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry[*pu4AudCurIdx].ui4_relative_offset;
            }
            else
            {
                LOG(5, "audio range done\n");
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockVid = FALSE;
            }
        }
        if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)&&
            (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl) &&
            (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry))
        {
            if(prRangeList->u4CurRangeVidIdx < prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui4_number_of_entry)
            {   
                fgVidDone = FALSE;
                pu4VidCurIdx = &prRangeList->u4CurRangeVidIdx;
                u8VidOffset = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui8_base_offset +
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry[*pu4VidCurIdx].ui4_relative_offset;
            }
            else
            {
                LOG(5, "Video range done\n");
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockAud = FALSE;
            }
        }
        if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnSubTitle)&&
           (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack < MP4_MAX_SUB_TRACK) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack]) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack]->u.pt_pb_idx_tbl_entry))
        {
            if(prRangeList->u4CurRangeSubIdx < prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack]->ui4_number_of_entry)
            {   
                fgSubDone = FALSE;
                pu4SubCurIdx = &prRangeList->u4CurRangeSubIdx;
                u8SubOffset = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack]->ui8_base_offset +
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack]->u.pt_pb_idx_tbl_entry[*pu4SubCurIdx].ui4_relative_offset;
            }
            else
            {
                LOG(5, "subtitle range done\n");
            }
        }
        
        if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4NumSentVid == 0)
        {
            _SWDMX_BeforeDeliverUseIdx(prSwdmxInst, eSWDMX_MEDIA_VIDEO);
        }
        
        if(fgVidDone&&fgSubDone&&fgAudDone)
        {
            fgRangeDone = TRUE;
        }
        
        if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo) && (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio))
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgSpeedChange && i4Speed == 1000)
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgHasAud)
                {
                    fgRangeDone = FALSE;
                    LOG(5, "Pause Video for A/V Sync\n");
                    VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
                    VDEC_Pause(prSwdmxInst->u1VdecId);
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause = TRUE;
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgSpeedChange = FALSE;

                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockVid = TRUE;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockAud = FALSE;

                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgHasAud = FALSE;
                }
                else
                {
                    INT32 i;
                    UINT32 u4CurPts = VDP_GetPts(prSwdmxInst->u1B2rId);
                    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl = NULL;

                    if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK)
                    {
                        prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack];
                    }

                    if(prIdxTbl != NULL)
                    {
                        for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
                        {
                            if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts >= (u4CurPts))
                            {
                                prRangeList->u4CurRangeAudIdx = i;
                                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgHasAud = TRUE;
                                LOG(5, "Find Audio PTS = 0x%08x\n", 
                                    prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts);
                                break;
                            }
                            else
                            {
                                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgHasAud = FALSE;
                            }
                        }
                        if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgHasAud)
                        {
                            AUD_SetStartPts(prSwdmxInst->u1AdecId, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts);

                            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TriggerAudPts = 
                                prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts;
                        }
                        else
                        {
                            prRangeList->u4CurRangeAudIdx = prIdxTbl->ui4_number_of_entry;
                        }
                    }
                }
            }
        }
        else if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo && prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgSpeedChange)
        {
             fgRangeDone = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockVid = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockAud = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgSpeedChange = FALSE;
        }

        if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgFeederInvalid)
        {
            INT32 i;
            FEEDER_REQ_DATA_T rFeederReqData;
            
            for (i = 0; i < prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4ReqReadTimes; i++)
            {
                _SWDMX_Mp4GetDataFromFeeder(prSwdmxInst, &rFeederReqData);
            }
           
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4LeftBytesInFeeder = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgRepeatRequest = TRUE;
            
            _SWDMX_Mp4SetFeederInvalid(prSwdmxInst, FALSE);
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4FeederReqID += 1;
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4ReqReadTimes = 0;

            // 2X. 1/2x ~ 1/32x, step 
            if ( i4Speed > 0 && i4Speed <= 2000 && i4Speed != 1000 )
            {
                if ( prRangeList->u4CurRangeVidIdx < prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui4_number_of_entry )
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VidLatestOff = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui8_base_offset +
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeVidIdx].ui4_relative_offset;
                }
                else if ( prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui4_number_of_entry > 0 )
                {
				    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VidLatestOff = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui8_base_offset +
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui4_number_of_entry - 1].ui4_relative_offset;
                }
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8CurFilepos   = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VidLatestOff;
            }
            else if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockVid)
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK)
                {
					if ( prRangeList->u4CurRangeAudIdx < prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->ui4_number_of_entry )
					{
						prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8AudLatestOff = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->ui8_base_offset+
							prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_relative_offset;

					}
					else if ( prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->ui4_number_of_entry > 0 )
					{
						prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8AudLatestOff = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->ui8_base_offset+
							prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->ui4_number_of_entry - 1].ui4_relative_offset;
					}

                    LOG(6, "Block Video Audio offset = %x\n", prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8AudLatestOff);
	
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8CurFilepos = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8AudLatestOff;
                }
                else
                {
                    LOG(1, "Out of Max Audio Tracks\n");
                    return eSWDMX_HDR_PRS_FAIL;
                }
            }
            else if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockAud)
            {
                if ( prRangeList->u4CurRangeVidIdx < prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui4_number_of_entry )
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VidLatestOff = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui8_base_offset +
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeVidIdx].ui4_relative_offset;
                }
                else if ( prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui4_number_of_entry > 0 )
                {
				    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VidLatestOff = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui8_base_offset +
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui4_number_of_entry - 1].ui4_relative_offset;
                }
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8CurFilepos = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VidLatestOff;
            }
            else
            {
                if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                    (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]) &&
                    (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio) &&
                    (prSwdmxInst->i4PlaySpeed == 1000))
                {
                    if ( prRangeList->u4CurRangeAudIdx < prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->ui4_number_of_entry )
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8AudLatestOff = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->ui8_base_offset +
                            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_relative_offset;
                    }
					else if ( prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->ui4_number_of_entry > 0 )
					{
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8AudLatestOff = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->ui8_base_offset +
                            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->ui4_number_of_entry - 1].ui4_relative_offset;
					}
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8AudLatestOff = 0xFFFFFFFF;
					prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8AudLatestOff <<= 32;
					prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8AudLatestOff += 0xFFFFFFFF;
                }

                if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl &&
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
                {
                    if ( prRangeList->u4CurRangeVidIdx < prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui4_number_of_entry )
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VidLatestOff = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui8_base_offset +
                            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeVidIdx].ui4_relative_offset;
                    }
					else if ( prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui4_number_of_entry > 0 )
					{
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VidLatestOff = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui8_base_offset +
                            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui4_number_of_entry - 1].ui4_relative_offset;
					}
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VidLatestOff = 0xFFFFFFFF;
					prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VidLatestOff <<= 32;
					prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VidLatestOff += 0xFFFFFFFF;
                }
        
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8CurFilepos = min(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VidLatestOff, prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8AudLatestOff);
            }

            if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgInvalidForSub)&&
            	(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack < MP4_MAX_SUB_TRACK))
            {
                
                if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack < MP4_MAX_SUB_TRACK) &&
                    (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack]) &&
                    (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnSubTitle) &&
                    (prRangeList->u4CurRangeSubIdx <prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack]->ui4_number_of_entry))
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8CurFilepos = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack]->ui8_base_offset+
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack]->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeSubIdx].ui4_relative_offset;
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8SubLatestOff = 0xFFFFFFFFFFFFFFFF;
                }
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgInvalidForSub = FALSE;
                //prSwdmxInst->rFmtInfo.rSwdmxMp4Info.rPrevSample.ui4_relative_offset 
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8PrevSampleOffset = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack]->ui8_base_offset+
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack]->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeSubIdx].ui4_relative_offset;
            }
            
            //prSwdmxInst->rFmtInfo.rSwdmxMp4Info.rPrevSample.ui4_relative_offset = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8CurFilepos;
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8PrevSampleOffset = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8CurFilepos;
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.rPrevSample.ui4_size = 0;

            _SWDMX_Mp4ResetFeeder(prSwdmxInst);
        }
        if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prDmxRangeInfo->e_mm_src_type ==
            MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        {      
        	  if(fgVidDone&&(!fgAudDone)&&
              (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio)&&
              (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo))
            {
		            if(!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgPartialRangeDone)
		            {
		                LOG(5, "Video Range is done\n");
		                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockVid = TRUE;
		                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockAud = FALSE;
		                _SWDMX_Mp4SetFeederInvalid(prSwdmxInst, TRUE);
		                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgPartialRangeDone = TRUE;
		                return eSWDMX_HDR_PRS_SUCCEED;
		            }
            }
            else if(!fgVidDone&&(fgAudDone)&&
               (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)&&
               (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio))
            {
                if(!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgPartialRangeDone)
                {
		                LOG(5, "Audio Range is done\n");
		                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockAud = TRUE;
		                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockVid = FALSE;
		                _SWDMX_Mp4SetFeederInvalid(prSwdmxInst, TRUE);
		                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgPartialRangeDone = TRUE;
		                return eSWDMX_HDR_PRS_SUCCEED;
                }
            }
        }
   
        if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockVid)&&
            (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio))
        {
            u8MinOffset = (u8SubOffset > u8AudOffset ? u8AudOffset : u8SubOffset);

            if(u8MinOffset == u8AudOffset)
            {   
                if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK)&&
                    pu4AudCurIdx&&
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack])
                {    
                    if (_SWDMX_Mp4HdrPrsDataType(prSwdmxInst,
                        u8AudOffset, 
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack],  
                        pu4AudCurIdx,   
                        eSWDMX_MEDIA_AUDIO) == eSWDMX_HDR_PRS_FAIL)
                    {
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                 }
                else
                {
                    if(!fgRangeDone)
                    {
                        LOG(1,"audio table is NULL\n");
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                }
            }
            else
            {
                if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack < MP4_MAX_SUB_TRACK)&&
                    pu4SubCurIdx&&
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack])
                {    
                    if (_SWDMX_Mp4HdrPrsDataType(prSwdmxInst,
                        u8SubOffset, 
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack],  
                        pu4SubCurIdx,   
                        eSWDMX_MEDIA_SUBTITLE) == eSWDMX_HDR_PRS_FAIL)
                    {
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                 }
                else
                {
                    if(!fgRangeDone)
                    {
                        LOG(1,"audio table is NULL\n");
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                }
            }
        }
        else if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockAud)&&
               (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo))
        {  
            u8MinOffset = (u8SubOffset > u8VidOffset ? u8VidOffset : u8SubOffset);

            if(u8MinOffset == u8VidOffset)
            {   
                if(pu4VidCurIdx&&
                   prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl)
                {    
                    if (_SWDMX_Mp4HdrPrsDataType(prSwdmxInst,
                        u8VidOffset, 
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl,  
                        pu4VidCurIdx,   
                        eSWDMX_MEDIA_VIDEO) == eSWDMX_HDR_PRS_FAIL)
                    {
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                 }
                else
                {
                    if(!fgRangeDone)
                    {
                        LOG(1,"audio table is NULL\n");
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                }
            }
            else
            {
                if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack < MP4_MAX_SUB_TRACK)&&
                    pu4SubCurIdx&&
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack])
                {    
                    if (_SWDMX_Mp4HdrPrsDataType(prSwdmxInst,
                        u8SubOffset,
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack],  
                        pu4SubCurIdx,   
                        eSWDMX_MEDIA_SUBTITLE) == eSWDMX_HDR_PRS_FAIL)
                    {
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                }
                else
                {
                    if(!fgRangeDone)
                    {
                        LOG(1,"audio table is NULL\n");
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                }
            }
        }
        else
        {
            u8MinOffset = (u8AudOffset > u8VidOffset ? u8VidOffset : u8AudOffset);
            u8MinOffset = (u8MinOffset > u8SubOffset ? u8SubOffset : u8MinOffset);

            
            if(u8MinOffset == u8VidOffset)
            {   
                if(pu4VidCurIdx&&
                   prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl)
                {    
                    if (_SWDMX_Mp4HdrPrsDataType(prSwdmxInst,
                        u8VidOffset, 
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl,  
                        pu4VidCurIdx,   
                        eSWDMX_MEDIA_VIDEO) == eSWDMX_HDR_PRS_FAIL)
                    {
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                 }
                else
                {
                    if(!fgRangeDone)
                    {
                        LOG(1,"audio table is NULL\n");
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                }
            }
            else if(u8MinOffset == u8AudOffset)
            {
                if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK)&&
                    pu4AudCurIdx&&prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio&&
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack])
                {    
                    if (_SWDMX_Mp4HdrPrsDataType(prSwdmxInst,
                        u8AudOffset, 
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack],  
                        pu4AudCurIdx,   
                        eSWDMX_MEDIA_AUDIO) == eSWDMX_HDR_PRS_FAIL)
                    {
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                }
                else
                {
                    if(!fgRangeDone)
                    {
                        LOG(1,"audio table is NULL\n");
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                }
            }
            else
            {
               if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack < MP4_MAX_SUB_TRACK)&&
                    pu4SubCurIdx&&
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack])
                {    
                    if (_SWDMX_Mp4HdrPrsDataType(prSwdmxInst,
                        u8SubOffset, 
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack],  
                        pu4SubCurIdx,   
                        eSWDMX_MEDIA_SUBTITLE) == eSWDMX_HDR_PRS_FAIL)
                    {
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                }
                else
                {
                    if(!fgRangeDone)
                    {
                        LOG(1,"audio table is NULL\n");
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                }
            }
        }

        if ((!fgRangeDone) &&
            (prRangeList->eRangeStats == eSWDMX_RANGE_STATE_PENDING))
        {
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
        }

        if (prRangeList->prDmxRangeInfo->b_endofstream)
        {
            if (fgRangeDone)
            {
                //LOG(5, "Last Range is done set it as pending\n");
                fgRangeDone = FALSE;
                prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
            }
            
            if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo) &&
                (prRangeList->u4CurRangeVidIdx == prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl->ui4_number_of_entry) && 
                (!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos))
            {
                LOG(5, "_SWDMX_Mp4HdrPrsUseIdx Last Video Range\n");

                if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause)
                {
                    _SWDMX_Mp4VideoWakeUp(prSwdmxInst);
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause = FALSE;
                }

                if(_SWDMX_SendMp4Eos(prSwdmxInst, eSWDMX_MEDIA_VIDEO))
                {
                     prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos = TRUE;
                }           
            }
            if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio) &&
                (prRangeList->u4CurRangeAudIdx == prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]->ui4_number_of_entry) &&
                (!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverAEos))
            {
                LOG(5, "_SWDMX_Mp4HdrPrsUseIdx Last Audio Range\n");
                
                if(_SWDMX_SendMp4Eos(prSwdmxInst, eSWDMX_MEDIA_AUDIO))
                {        
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverAEos = TRUE;
                }
                AUD_MM_Set_Avsync_event(prSwdmxInst->u1AdecId);
            }
        }

        if (!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgFeederInvalid)
        {
            if (_SWDMX_Mp4RequestData(prSwdmxInst) != eSWDMX_HDR_PRS_SUCCEED)
            {
                LOG(5, "Feeder fails to move data\n");
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
        }

    }
     
    if (fgRangeDone)  
    {
        INT32 i;
        BOOL   fgPending = FALSE;
        
        if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause)
        {                 
           _SWDMX_Mp4VideoWakeUp(prSwdmxInst);
           prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause = FALSE;
        }
        
        if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prDmxRangeInfo->e_mm_src_type ==
            MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
            {
                UINT16 u2MaxQNum = 0; 
                UINT16 u2CurQNum = 0;

                VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
                if (u2CurQNum > (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidFps))
                {
                   LOG(5, "Range is pending\n");
                   fgPending = TRUE;
                }
            }
            else
            {
                UINT32 u4CurPts = 0;
                UINT32 u4CurDmxPts = 0;
            
                u4CurPts = STC_GetStcValue(prSwdmxInst->u1StcId);
                u4CurDmxPts = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts;
     
                if ((u4CurDmxPts >= u4CurPts) &&
                    ((u4CurDmxPts - u4CurPts)> MP4_ONE_SEC_DURATION))
                {
                    LOG(5, "Range is pending\n");
                    x_thread_delay(10);
                    fgPending = TRUE;
                }
            }
        }
        
        if(fgPending) 
        {   
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        }
        else
	    {
            LOG(5, "Range is finished\n");
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl = NULL;
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgPartialRangeDone = FALSE;

            for (i = 0; (i < prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1TotalATrack) && (i < MP4_MAX_AUDIO_TRACK); i++)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[i] = NULL;
            }
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1TotalATrack = 0;

            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockVid = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockAud = FALSE;
        } 
    }
    
    return eSWDMX_HDR_PRS_SUCCEED;
}
        
static SWDMX_HDR_PRS_STATE_T _SWDMX_Mp4HdrPrsDataType(
    SWDMX_INFO_T* prSwdmxInst,
    UINT64 u8Offset,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl, 
    UINT32* pu4CurIdx, 
    UINT8 u1HdrType)
{
	      
    if (!prSwdmxInst || (prIdxTbl == NULL) || (pu4CurIdx == NULL))
    {
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }
    
    if ((u8Offset + prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size) >
       (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8_range_sz))         
    {
        LOG(5, "Video offset or Audio offset is larger than range size\n");
        
        if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl) && (!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos))
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause)
            {
                _SWDMX_Mp4VideoWakeUp(prSwdmxInst);
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause = FALSE;
            }

            if (_SWDMX_SendMp4Eos(prSwdmxInst, eSWDMX_MEDIA_VIDEO))
            {
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos = TRUE;
            }            
        }
        if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
            (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]) &&
            (!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverAEos))
        {
            if(_SWDMX_SendMp4Eos(prSwdmxInst, eSWDMX_MEDIA_AUDIO))
            {
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverAEos = TRUE;
            } 
            AUD_MM_Set_Avsync_event(prSwdmxInst->u1AdecId);
        }
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }
    
    if (_SWDMX_DeliverMp4DataUseIdx(prSwdmxInst,
                                    1000, prIdxTbl,  
                                    pu4CurIdx,u1HdrType)== eSWDMX_HDR_PRS_FAIL)
    {
        return eSWDMX_HDR_PRS_FAIL;
    }
    return eSWDMX_HDR_PRS_SUCCEED;
}

static SWDMX_HDR_PRS_STATE_T _SWDMX_Mp4RequestData(SWDMX_INFO_T* prSwdmxInst)
{
    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;   
    UINT32 u4ReadSize = 0;
    UINT32 u4MaxDataByte = 0;
    
    if (!prSwdmxInst)
    {
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }
    
    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) != 
        FEEDER_E_OK)
    {
        LOG(7, "Feeder input buffer error.\n");
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }
    
    if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
    {
    	  u4ReadSize = MIN_READ_SIZE;
    }
    else
    {
    	  u4ReadSize =  MIN_READ_SIZE_NO_VIDEO;
    }
    u4MaxDataByte  = prSwdmxInst->rFeederInfo.u4Size - u4ReadSize;
      
    if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4LeftBytesInFeeder < u4MaxDataByte) && 
        ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8CurFilepos) < prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8_range_sz))
    {
        u4ReadSize = min((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8_range_sz - prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8CurFilepos), (UINT64)u4ReadSize);
        if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4ReqReadTimes == 0) && (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4FeederInvalidCount <=4))
        {
	        if (FeederSetRequest(prSwdmxInst->eFeederSrc,
	            u4ReadSize, 
	            (UINT64)(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8CurFilepos),
	            &rFeederCond,
	            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4FeederReqID++) == FEEDER_E_BUFFER_FULL)
	        {
	            LOG(3, "Feeder buffer is not enough\n");
	            return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
	        }
	        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4ReqReadTimes++;
	    }
   
                    
        if (!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgFeederLeftNotEnough)
        {
            if (!_SWDMX_Mp4GetDataFromFeederNoWait(prSwdmxInst, &rFeederReqData))
            {
                return eSWDMX_HDR_PRS_SUCCEED;
            }
        }
        else
        {
            if (!_SWDMX_Mp4GetDataFromFeeder(prSwdmxInst, &rFeederReqData))
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }    

        }
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4ReqReadTimes--;
    
        if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgRepeatRequest)  
        {
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurDmxFeederRPtr = rFeederReqData.u4WriteAddr+ rFeederReqData.u4AlignOffset;
            
            if(rFeederReqData.u4WriteAddr < prSwdmxInst->rFeederInfo.u4StartAddr)
            {
            	  _SWDMX_Mp4SetFeederInvalid(prSwdmxInst, TRUE);
                  prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4FeederInvalidCount++;
                  return eSWDMX_HDR_PRS_FAIL;
            }
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4FeederInvalidCount = 0;
            
            if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurDmxFeederRPtr > VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
            {
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurDmxFeederRPtr = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurDmxFeederRPtr  + 
                VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr)   - VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr);
            }
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgRepeatRequest = FALSE;
        }

        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8CurFilepos += rFeederReqData.u4ReadSize - rFeederReqData.u4AlignOffset; 
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4LeftBytesInFeeder+= rFeederReqData.u4ReadSize -rFeederReqData.u4AlignOffset ;
    }
    else if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8CurFilepos) >= prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8_range_sz)
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo && prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos && prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverAEos)
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
            else
            {
                if (((!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgPartialRangeDone)||
                    (!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos) ||( !prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverAEos))&&
                    (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgFeederLeftNotEnough))
                {
                    _SWDMX_Mp4SetFeederInvalid(prSwdmxInst, TRUE);
                }
                return eSWDMX_HDR_PRS_SUCCEED;
            }
        }
        else if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo && 
            (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos||prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgFeederLeftNotEnough))
        {
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }
        else if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio && prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverAEos)
        {
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }
    }
    
    return eSWDMX_HDR_PRS_SUCCEED;
}


static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverMp4Keyframe(
    SWDMX_INFO_T* prSwdmxInst,
    INT32 i4Speed,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl, 
    UINT32* pu4CurIdx, 
    UINT8 u1HdrType,
    SWDMX_RANGE_LIST_T* prRangeList)
{
    UINT8 u1PidIdx, u1PidType;
    UINT32 u4DmxAvailSize = 0;

    DMX_MM_DATA_T rDmxMMData;
    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;

    UINT32 u4WriteAddr = 0,u4NaluSize = 0, u4FrmSize = 0;
    UINT8  i = 0;

    UCHAR *pucFdrBuf = NULL;

    if (!prSwdmxInst)
    {
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgFeederInvalid)
    {
        INT32 i;
        FEEDER_REQ_DATA_T rFeederReqData;
        
        for (i = 0; i < prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4ReqReadTimes ; i++)
        {
            _SWDMX_Mp4GetDataFromFeeder(prSwdmxInst, &rFeederReqData);
        }
        
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4LeftBytesInFeeder = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgRepeatRequest = TRUE;
        _SWDMX_Mp4SetFeederInvalid(prSwdmxInst, FALSE);
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4FeederReqID += 1;
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4ReqReadTimes = 0;

        _SWDMX_Mp4ResetFeeder(prSwdmxInst);
         
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
     }

    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    u1PidIdx = (u1HdrType == eSWDMX_MEDIA_VIDEO) ? (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid) :
        (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1AudPid);

    u1PidType = (u1HdrType == eSWDMX_MEDIA_VIDEO) ? (DMX_PID_TYPE_ES_VIDEO) : (DMX_PID_TYPE_ES_AUDIO);

#if 0
    u4DmxAvailSize = DMX_MM_GetBufEmptySize(u1HdrType);
#endif
    u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, 
                                                                        (DMX_PID_TYPE_T)u1PidType,
                                                                        u1PidIdx);

    LOG(6, "prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey = %ld Key Pts = 0x%08x Dmx available size = 0x%08x\n", 
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey,
        prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts,
        u4DmxAvailSize);

    if (prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size > u4DmxAvailSize)
    {
        LOG(7, "Demuxer Buffer is not enough Hdr type = %x u4DmxAvailSize = %ld  frame size = %ld.\n", 
            u1HdrType,
            u4DmxAvailSize,
            prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size);
        
        x_thread_delay(1);
		prSwdmxInst->fgFifoFull = TRUE;
        return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
    }

    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) != 
      FEEDER_E_OK)
    {
        LOG(7, "Feeder input buffer error.\n");
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    // Send sequence header for decoder's initialization?

    if (((*pu4CurIdx) < prIdxTbl->ui4_number_of_entry)&& !prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos)
    {
        if (u1HdrType == eSWDMX_MEDIA_VIDEO)
        {
            //UCHAR *pBuf;
            UINT16 u2MaxQNum, u2CurQNum;
            VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
            LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);

            // Temporally use 30 for fps
            // Should use fps in the fucture.
            if (u2CurQNum > prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidFps)
            {
                x_thread_delay(1);
                return eSWDMX_HDR_PRS_SUCCEED;
            }

	      if (((UINT64)prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset + prIdxTbl->ui8_base_offset
		  	+ prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size)> (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8_range_sz))
            {
                LOG(5, "Video Offset is larger than range size\n");
                    
                if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl) && (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos == FALSE))
                {
                    if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause)
                    {
                        _SWDMX_Mp4VideoWakeUp(prSwdmxInst);
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause = FALSE;					
                    }

                    if (_SWDMX_SendMp4Eos(prSwdmxInst, eSWDMX_MEDIA_VIDEO))
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos = TRUE;
                    }    
					
                    prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
                }
               return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
            LOG(7, "prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4NumSentVid %ld  ui4_size = 0x%08x\n", prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4NumSentVid, prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size);
            LOG(7, "ui8_base_offset + ui4_relative_offset = %08x.\n", prIdxTbl->ui8_base_offset + prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset);

			if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VDecType == ENUM_SWDMX_VDEC_H264
				 || *pu4CurIdx!=prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4LastSentKey)
			{
				if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4ReqReadTimes==0)
				{
					if (FeederSetRequest(prSwdmxInst->eFeederSrc, 
						prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size, 
						(UINT64)((prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset)+(prIdxTbl->ui8_base_offset)),
						&rFeederCond,
						prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4FeederReqID) == FEEDER_E_BUFFER_FULL)
					{
						LOG(3, "Feeder's buffer is full\n");
						FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, VIRTUAL(prSwdmxInst->rFeederInfo.u4WriteAddr));
						return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
					}
					else
					{
						prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4FeederReqID++;
						prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4ReqReadTimes++;
					}
				}
				
                if (!_SWDMX_Mp4GetDataFromFeeder(prSwdmxInst, &rFeederReqData))
                {
                   return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                } 

                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4ReqReadTimes--;

			 if (rFeederReqData.u4ReadSize <= rFeederReqData.u4AlignOffset 
			 	   ||prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size > (rFeederReqData.u4ReadSize - rFeederReqData.u4AlignOffset))
                {
                  LOG(3, "_SWDMX_DeliverMp4Keyframe rFeederReqData.u4ReadSize <= rFeederReqData.u4AlignOffset\n");
                  return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                }

		        if(rFeederReqData.u4WriteAddr+rFeederReqData.u4AlignOffset>=prSwdmxInst->rFeederInfo.u4EndAddr)
		        {
				   u4WriteAddr = VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr+(rFeederReqData.u4WriteAddr + rFeederReqData.u4AlignOffset-prSwdmxInst->rFeederInfo.u4EndAddr));
			    }
			    else
			    {
				   u4WriteAddr = VIRTUAL(rFeederReqData.u4WriteAddr + rFeederReqData.u4AlignOffset);
			    }
				prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4LastSentKey=*pu4CurIdx;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4RecFeederWPtr = u4WriteAddr;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgRequestData = FALSE;
			}
			else
			{
                u4WriteAddr=prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4RecFeederWPtr;
			}

            
            //replace length with start code            
            if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VDecType == ENUM_SWDMX_VDEC_H264)
            {
                UCHAR* pucBuf = NULL;
                
                pucFdrBuf = (UCHAR*)u4WriteAddr;
                pucBuf       = pucFdrBuf;
                u4FrmSize = 0;
                
                while(u4FrmSize < prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size)
                {
                    u4NaluSize = 0;
                    //insert start code
                    for(i = 0; i < prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1StartCodeLenMinus1; i++)
                    {
                        u4NaluSize += (UINT32)*pucFdrBuf;
                        u4NaluSize <<= 8;
                        *pucFdrBuf++ = 0;
                        //check ring buffer
                        if((UINT32)pucFdrBuf == VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                        {
                            pucFdrBuf = (UCHAR*)VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
                        }
                    }
                    u4NaluSize += (UINT32)*pucFdrBuf;
                    if (u4NaluSize == 0)
                    {
                	    (*pu4CurIdx)++;
                	    return eSWDMX_HDR_PRS_SUCCEED;
                    }

                    *pucFdrBuf++ = 1;
                    if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1StartCodeLenMinus1 < H264_STARTCODE_LEN) &&
                        (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1StartCodeLenMinus1 > 0))
                    {
                         UINT32   u4Loop = 0;
                         UINT32   u4DataSize = 0;

#if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5396) ||defined(CC_MT5368) ||defined(CC_MT5389)
                        HalFlushInvalidateDCacheMultipleLine(prSwdmxInst->rFeederInfo.u4StartAddr, 
                            (UINT32)(prSwdmxInst->rFeederInfo.u4EndAddr - prSwdmxInst->rFeederInfo.u4StartAddr));
#else
                        HalFlushInvalidateDCache();
#endif
                         
                        for(u4Loop=0; u4Loop<(H264_STARTCODE_LEN-
                                                     prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1StartCodeLenMinus1); u4Loop++)
                        {
                            if((UINT32)pucBuf == VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr))
                            {
                                pucBuf = (UCHAR*)VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr-1);
                            }
                            else
                            {
                                pucBuf--;
                            }
                            *pucBuf = 0;
                        }
                        u4DataSize = u4NaluSize + (H264_STARTCODE_LEN-
                                                                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1StartCodeLenMinus1)+
                                                                        (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1StartCodeLenMinus1 + 1);
                        
                        rDmxMMData.u4BufStart = VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
                        rDmxMMData.u4BufEnd = VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr);
                        rDmxMMData.u4StartAddr = (UINT32)pucBuf;
                        rDmxMMData.u4FrameSize = u4DataSize;
                        rDmxMMData.u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
                        rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid;
                        u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, 
                                                                                            DMX_PID_TYPE_ES_VIDEO,
                                                                                            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid);
                        if(u4DataSize >= u4DmxAvailSize)
                        {
                            _SWDMX_Mp4SetFeederInvalid(prSwdmxInst, TRUE);
							prSwdmxInst->fgFifoFull = TRUE;
                            return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
                        }
                        //if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
                        if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
                        {
                            LOG(0, "Demuxer fails to move data.\n");
                            return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                        }
                    }
                    //check ring buffer
                    if((UINT32)pucFdrBuf == VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                    {
                        pucFdrBuf = (UCHAR*)VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
                    }
                    // nalu size
                    u4FrmSize += ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1StartCodeLenMinus1 + 1) + u4NaluSize);
                    pucFdrBuf += u4NaluSize;
                    //check ring buffer
                    if((UINT32)pucFdrBuf >= VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                    {
                        pucFdrBuf = 
                            (UCHAR*)((UINT32)pucFdrBuf - 
                            VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) + 
                            VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                    }
                    pucBuf = pucFdrBuf;
                }
#if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5396) ||defined(CC_MT5368) ||defined(CC_MT5389)
                HalFlushInvalidateDCacheMultipleLine(prSwdmxInst->rFeederInfo.u4StartAddr, 
                    (UINT32)(prSwdmxInst->rFeederInfo.u4EndAddr - prSwdmxInst->rFeederInfo.u4StartAddr));
#else
                HalFlushInvalidateDCache();
#endif
            }

            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4NumSentVid++;

            if(((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts>>32) != 0xFFFFFFFF)
                || ((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts) != 0xFFFFFFFF))
            {
                UINT32 u4Pts, u4CurSTC;
                
                u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
                if(u4Pts > (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts)
                {
                    STC_SetStcValue(prSwdmxInst->u1StcId, u4Pts - 18000);
                    u4CurSTC = STC_GetStcValue(prSwdmxInst->u1StcId);                
                    LOG(1, "Got Vid T PTS 0x%x, STC 0x%x, Seek 0x%x\n", 
                        u4Pts, u4CurSTC, (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts);            
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts = 0xFFFFFFFF;
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts <<= 32;
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VSeekPts += 0xFFFFFFFF;
                }
            }
        }

	  prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4LastVidPts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
      
          prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VidLatestOff = prIdxTbl->ui8_base_offset +
                 prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset+
                 prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
          
        if((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VDecType != ENUM_SWDMX_VDEC_H264)||
            ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VDecType == ENUM_SWDMX_VDEC_H264) && 
            (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1StartCodeLenMinus1 == H264_STARTCODE_LEN)) )                        
        {
            rDmxMMData.u4BufStart = VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
            rDmxMMData.u4BufEnd = VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr);
            rDmxMMData.u4StartAddr = u4WriteAddr;
            rDmxMMData.u4FrameSize = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
            rDmxMMData.u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
            rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid;
            
            //if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
            if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
            {
                LOG(1, "Demuxer fails to move data.\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
        }


        FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, VIRTUAL(u4WriteAddr));
        
        while (!_SWDMX_Mp4CalculateRepeatFrame(prSwdmxInst, i4Speed))
        {
            BOOL fgNextKeyTable = FALSE;
            
            fgNextKeyTable = _SWDMX_Mp4CheckNextKeyTable(prSwdmxInst,i4Speed,pu4CurIdx);

            if(!fgNextKeyTable)
            {
               if (i4Speed > 2000)
               {
                  (*pu4CurIdx)++;
               }
               else if ((i4Speed < 0) &&(*pu4CurIdx > 0))
               {
                   (*pu4CurIdx)--;
               }
             }

             // The last key frame
             if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame == 0)
             {
                 if ((i4Speed > 2000) &&
                 (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey == prIdxTbl->ui4_number_of_entry) &&
                 (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurRepeatFrameIdx == prIdxTbl->ui4_number_of_entry - 1) &&
                 !prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos)
                 {
                     LOG(5, "Fast Forward _SWDMX_Mp4HdrPrsUseIdx Deliver Video Eos\n");
                     if (!_SWDMX_SendMp4Eos(prSwdmxInst, eSWDMX_MEDIA_VIDEO))
                     {
                         LOG(3, "Fail to send Video Eos\n");
                         return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                     }
                     prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos = TRUE;
                     prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
                     return eSWDMX_HDR_PRS_SUCCEED;
                 }
                 else if ((i4Speed < 0) &&
                             (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey == 0) &&
                             (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurRepeatFrameIdx == 0) &&
                             !prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos)
                 {
                     if (!_SWDMX_SendMp4Eos(prSwdmxInst, eSWDMX_MEDIA_VIDEO))
                     {
                         return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                     }
                     prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgDeliverVEos = TRUE;
                     prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
                     return eSWDMX_HDR_PRS_SUCCEED;
                 }
             }
         }
    }
    
    return eSWDMX_HDR_PRS_SUCCEED;
}

static BOOL _SWDMX_Mp4ResetFeeder(SWDMX_INFO_T* prSwdmxInst)
{
    SWDMX_MP4_INFO_T *prSwdmxMp4Info = NULL;
    prSwdmxMp4Info = (SWDMX_MP4_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMp4Info;
    SWDMX_MP4_VERIFY_NULL_ERROR(prSwdmxInst);  

    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) != 
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    LOG(5, "_SWDMX_Mp4ResetFeeder Cur Fed's WPtr = 0x%08x\n", prSwdmxInst->rFeederInfo.u4WriteAddr);
    prSwdmxMp4Info->u4CurDmxFeederRPtr = prSwdmxInst->rFeederInfo.u4WriteAddr ;

    // Update read ptr to feeder
    FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurDmxFeederRPtr));
    return TRUE;
}

#ifdef HAN_MVC_HEADER //MVC Handle

SWDMX_HDR_PRS_STATE_T  _SWDMX_H264HeaderHandle(SWDMX_INFO_T *prSwdmxInst, UCHAR *pucHdrBuf, UCHAR *pucFdrBuf, UINT8 u1HdrType) 
{
	 UINT8 u1NumOfPS = 0;
	 UINT8 u1PSNum = 0;
	 UINT8 i = 0;
	 UINT16 u2PSLength = 0;
	 DMX_MM_DATA_T rDmxMMData;
	 if(pucHdrBuf == NULL || pucFdrBuf == NULL || prSwdmxInst == NULL)
	 {
	 	    LOG(0,"header pointer is NULL\n");
	 	    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
	 }
	 
	 x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
	 
	 pucHdrBuf += 4; //skip unused info
   prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1StartCodeLenMinus1 = (*pucHdrBuf++) & 0x3;
   for(u1PSNum = 0; u1PSNum < 2; u1PSNum++)    // 0 for SPS, 1 for PPS
   {
      if(u1PSNum == 0)
      {
          u1NumOfPS = (*pucHdrBuf++) & 0x1f;
      }
      else
      {
          u1NumOfPS = *pucHdrBuf++;
      }
      for (i = 0; i < u1NumOfPS; i++)
      {
          u2PSLength = (UINT16)(*pucHdrBuf++);
          u2PSLength <<= 8;
          u2PSLength += (UINT16)(*pucHdrBuf++);

          //handle ring buffer
          if(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) <= (((UINT32)pucFdrBuf + (UINT32)u2PSLength) + 4))
          {
              pucFdrBuf = (UCHAR*)VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
          }
          //set move data parameter
          rDmxMMData.u4BufStart = VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
          rDmxMMData.u4BufEnd = VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr);
          rDmxMMData.u4StartAddr = (UINT32)pucFdrBuf;
          rDmxMMData.u4FrameSize = (UINT32)u2PSLength + 4;    // 4 for start code
          rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid;

          //insert start code
          *pucFdrBuf++ = 0;
          *pucFdrBuf++ = 0;
          *pucFdrBuf++ = 0;
          *pucFdrBuf++ = 1;      
          //copy SPS or PPS
          x_memcpy((VOID*)pucFdrBuf, (VOID*)pucHdrBuf, u2PSLength);
          pucFdrBuf += u2PSLength;
          pucHdrBuf += u2PSLength;
#if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5396) ||defined(CC_MT5368) ||defined(CC_MT5389)
          HalFlushInvalidateDCacheMultipleLine(prSwdmxInst->rFeederInfo.u4StartAddr, 
              (UINT32)(prSwdmxInst->rFeederInfo.u4EndAddr - prSwdmxInst->rFeederInfo.u4StartAddr));
#else
          HalFlushInvalidateDCache();
#endif
          
          //if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
          if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
          {
              LOG(1, "Demuxer fails to move sequence header.\n");
              return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
          }
      }
   }
   return eSWDMX_HDR_PRS_SUCCEED;
}

#endif 
static SWDMX_HDR_PRS_STATE_T  _SWDMX_BeforeDeliverUseIdx(SWDMX_INFO_T *prSwdmxInst, UINT8 u1HdrType)
{
    UCHAR *pucHdrBuf = NULL;
    UCHAR *pucFdrBuf = NULL;
#ifndef HAN_MVC_HEADER     
    UINT8 u1NumOfPS = 0, u1PSNum = 0;
    UINT8 i = 0;
    UINT16 u2PSLength = 0;
#endif
    DMX_MM_DATA_T rDmxMMData;

    if (!prSwdmxInst)
    {
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }

    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) != 
        FEEDER_E_OK)
    {
        LOG(7, "Feeder input buffer error.\n");
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

        if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VDecType == ENUM_SWDMX_VDEC_H264)
        {  
        	  #ifdef HAN_MVC_HEADER
        	  if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHeaderPtr != (UINT32)NULL)
        	  {
        	  	  pucHdrBuf = (UCHAR*)VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHeaderPtr);
                pucFdrBuf = (UCHAR*)VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
        	  	  if(_SWDMX_H264HeaderHandle(prSwdmxInst, pucHdrBuf, pucFdrBuf,u1HdrType)!= eSWDMX_HDR_PRS_SUCCEED)
        	  	  {
        	  	  	  return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
        	  	  }
        	  }
        	  if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHeaderSecPtr != (UINT32)NULL)
        	  {
        	  	  pucHdrBuf = (UCHAR*)VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHeaderSecPtr);
                pucFdrBuf = (UCHAR*)VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
        	  	  if(_SWDMX_H264HeaderHandle(prSwdmxInst, pucHdrBuf, pucFdrBuf,u1HdrType)!= eSWDMX_HDR_PRS_SUCCEED)
        	  	  {
        	  	  	  return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
        	  	  }
        	  }
        	  #else
            pucHdrBuf = (UCHAR*)VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHeaderPtr);
            pucFdrBuf = (UCHAR*)VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
            pucHdrBuf += 4; //skip unused info
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1StartCodeLenMinus1 = (*pucHdrBuf++) & 0x3;
            for(u1PSNum = 0; u1PSNum < 2; u1PSNum++)    // 0 for SPS, 1 for PPS
            {
                if(u1PSNum == 0)
                {
                    u1NumOfPS = (*pucHdrBuf++) & 0x1f;
                }
                else
                {
                    u1NumOfPS = *pucHdrBuf++;
                }
                for (i = 0; i < u1NumOfPS; i++)
                {
                    u2PSLength = (UINT16)(*pucHdrBuf++);
                    u2PSLength <<= 8;
                    u2PSLength += (UINT16)(*pucHdrBuf++);

                    //handle ring buffer
                    if(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) <= (((UINT32)pucFdrBuf + (UINT32)u2PSLength) + 4))
                    {
                        pucFdrBuf = (UCHAR*)VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
                    }
                    //set move data parameter
                    rDmxMMData.u4BufStart = VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
                    rDmxMMData.u4BufEnd = VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr);
                    rDmxMMData.u4StartAddr = (UINT32)pucFdrBuf;
                    rDmxMMData.u4FrameSize = (UINT32)u2PSLength + 4;    // 4 for start code
                    rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid;

                    //insert start code
                    *pucFdrBuf++ = 0;
                    *pucFdrBuf++ = 0;
                    *pucFdrBuf++ = 0;
                    *pucFdrBuf++ = 1;      
                    //copy SPS or PPS
                    x_memcpy((VOID*)pucFdrBuf, (VOID*)pucHdrBuf, u2PSLength);
                    pucFdrBuf += u2PSLength;
                    pucHdrBuf += u2PSLength;
#if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5396) ||defined(CC_MT5368) ||defined(CC_MT5389)
                    HalFlushInvalidateDCacheMultipleLine(prSwdmxInst->rFeederInfo.u4StartAddr, 
                        (UINT32)(prSwdmxInst->rFeederInfo.u4EndAddr - prSwdmxInst->rFeederInfo.u4StartAddr));
#else
                    HalFlushInvalidateDCache();
#endif
                    
                    //if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
                    if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
                    {
                        LOG(1, "Demuxer fails to move sequence header.\n");
                        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                    }
                }
            }
            #endif
        }
        else if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VDecType == ENUM_SWDMX_VDEC_MPEG4)
        {
            x_memcpy((VOID*)VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr), 
                (VOID*)VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHeaderPtr), 
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHdrLength);

#if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5396) ||defined(CC_MT5368) ||defined(CC_MT5389)
            HalFlushInvalidateDCacheMultipleLine(prSwdmxInst->rFeederInfo.u4StartAddr, 
                (UINT32)(prSwdmxInst->rFeederInfo.u4EndAddr - prSwdmxInst->rFeederInfo.u4StartAddr));
#else
            HalFlushInvalidateDCache();
#endif
            
            rDmxMMData.u4BufStart = VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
            rDmxMMData.u4BufEnd = VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr);
            rDmxMMData.u4StartAddr = VIRTUAL(prSwdmxInst->rFeederInfo.u4WriteAddr);
            rDmxMMData.u4FrameSize = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidHdrLength;
            rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid;
            
            //if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
            if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
            {
                LOG(1, "Demuxer fails to move sequence header.\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
        }
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4NumSentVid++;
        
       return eSWDMX_HDR_PRS_SUCCEED;
}

static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverMp4DataUseIdx(SWDMX_INFO_T* prSwdmxInst,
    INT32 i4Speed,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl, 
    UINT32* pu4CurIdx, 
    UINT8 u1HdrType)
{
    UINT32 u4DmxAvailSize          = 0;
    UINT32 u4DmxVdecAvailSize = 0;
    UINT32 u4DmxAdecAvailSize = 0;

    DMX_MM_DATA_T rDmxMMData;

    UINT32 u4NaluSize = 0, u4FrmSize = 0;

    UCHAR *pucFdrBuf = NULL;

    UINT64 u8WriteOffset = 0;

    int i = 0;
    
    MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T *prTmpSample;
    SWDMX_MP4_INFO_T *prSwdmxMp4Info = NULL;
    UINT64 u8TmpSampleOffset = 0;

    if (!prSwdmxInst)
    {
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }

    prSwdmxMp4Info = (SWDMX_MP4_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMp4Info;

    prTmpSample = &prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx];
    u8TmpSampleOffset = prIdxTbl->ui8_base_offset + prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset;

    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    u4DmxVdecAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, 
                                                                        DMX_PID_TYPE_ES_VIDEO,
                                                                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid);
    u4DmxAdecAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, 
                                                                        DMX_PID_TYPE_ES_AUDIO,
                                                                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1AudPid);

    if (prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size == 0)
    {
        (*pu4CurIdx)++;
        return eSWDMX_HDR_PRS_FAIL;
    }

    if(u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
          u4DmxAvailSize = u4DmxVdecAvailSize;
    }
    else
    {
          u4DmxAvailSize = u4DmxAdecAvailSize;
          
        if ((prSwdmxMp4Info->u4ADecType == ENUM_SWDMX_ADEC_AAC) &&
            (u4DmxAvailSize > AAC_FAKE_HDR_SIZE))
          {
            u4DmxAvailSize -= AAC_FAKE_HDR_SIZE;
          }
    }
   
    if (prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size > u4DmxAvailSize)
    {
        if (prSwdmxMp4Info->fgVDecIsPause)
        {
            _SWDMX_Mp4VideoWakeUp(prSwdmxInst);
            prSwdmxMp4Info->fgVDecIsPause = FALSE;
        }
        if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prDmxRangeInfo->e_mm_src_type ==
            MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        {
		        if (u1HdrType == eSWDMX_MEDIA_AUDIO && 
		            prSwdmxMp4Info->fgEnVideo && 
		            !prSwdmxMp4Info->fgPartialRangeDone)
		        {
		            prSwdmxMp4Info->fgBlockAud = TRUE;
		            prSwdmxMp4Info->fgBlockVid = FALSE;
		            _SWDMX_Mp4SetFeederInvalid(prSwdmxInst, TRUE);
		        }
		        if (u1HdrType == eSWDMX_MEDIA_VIDEO && 
		            prSwdmxMp4Info->fgEnAudio &&
		            !prSwdmxMp4Info->fgPartialRangeDone)
		        {
		            prSwdmxMp4Info->fgBlockAud = FALSE;
		            prSwdmxMp4Info->fgBlockVid = TRUE;
		            _SWDMX_Mp4SetFeederInvalid(prSwdmxInst, TRUE);
		        }
		    }

        if (u1HdrType == eSWDMX_MEDIA_AUDIO &&
            !prSwdmxMp4Info->fgEnVideo)
        {
            LOG(5, "Trigger audio by _SWDMX_DeliverMp4DataUseIdx\n");
            AUD_MMAoutEnable(0, TRUE);
            prSwdmxMp4Info->fgAudIsReady = FALSE;
        }

        LOG(6, "Dmx Buffer is not enough. Header Type = %x\n", u1HdrType);
        x_thread_delay(1);
		prSwdmxInst->fgFifoFull = TRUE;
        return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
    }

    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) != 
        FEEDER_E_OK)
    {
        LOG(7, "Feeder input buffer error.\n");
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        UINT16 u2MaxQNum, u2CurQNum;

        VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
        if ((u2CurQNum > (u2MaxQNum - 256)) ||
            (u2CurQNum > (prSwdmxMp4Info->u4VidFps * 10)))
        {
                prSwdmxInst->fgFifoFull = TRUE;
            	if (prSwdmxMp4Info->fgVDecIsPause)
            	{
        		      _SWDMX_Mp4VideoWakeUp(prSwdmxInst);
                	prSwdmxMp4Info->fgVDecIsPause = FALSE;
            	}
            
            	LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
            	x_thread_delay(10);
            	return eSWDMX_HDR_PRS_SUCCEED;
        }    
        if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prDmxRangeInfo->e_mm_src_type ==
            MM_SRC_TYPE_HIGH_SPEED_STORAGE) 
        {
        	  if (prSwdmxMp4Info->fgEnAudio &&
            (!prSwdmxMp4Info->fgPartialRangeDone) &&
            (prSwdmxMp4Info->u4CurVidPts > (prSwdmxMp4Info->u4CurAudPts + MP4_ONE_SEC_DURATION)))
		        {
		            LOG(5, "Video is enough, send audio data in the same timeline\n");
		            prSwdmxMp4Info->fgBlockVid = TRUE;
		            prSwdmxMp4Info->fgBlockAud = FALSE;
		            _SWDMX_Mp4SetFeederInvalid(prSwdmxInst, TRUE);
		        }
		        else if(prSwdmxMp4Info->fgEnAudio &&
		            (!prSwdmxMp4Info->fgPartialRangeDone) &&
		            (prSwdmxMp4Info->u4CurVidPts > prSwdmxMp4Info->u4CurAudPts))
		        {
		            prSwdmxMp4Info->fgBlockVid = FALSE;
		            prSwdmxMp4Info->fgBlockAud = FALSE;
		        }
		    }
    }

    if(prTmpSample->ui4_size > prSwdmxMp4Info->u4LeftBytesInFeeder)
    {
        prSwdmxMp4Info->fgFeederLeftNotEnough = TRUE;
        LOG(3, "left in feeder is not enough, hdr type = %d, sample size = %d, left bytes = %d\n", 
            u1HdrType, prTmpSample->ui4_size, prSwdmxMp4Info->u4LeftBytesInFeeder);       
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;         
    }
    else
    {
        prSwdmxMp4Info->fgFeederLeftNotEnough = FALSE;
    }
    
    if (/*prTmpSample->ui4_relative_offset*/ u8TmpSampleOffset >= 
        (/*prSwdmxMp4Info->rPrevSample.ui4_relative_offset*/prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8PrevSampleOffset + prSwdmxMp4Info->rPrevSample.ui4_size))
    {
        u8WriteOffset =  
            /*prTmpSample->ui4_relative_offset*/ u8TmpSampleOffset - 
            (/*prSwdmxMp4Info->rPrevSample.ui4_relative_offset*/prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8PrevSampleOffset + prSwdmxMp4Info->rPrevSample.ui4_size);
    }
    else
    {
        prSwdmxMp4Info->u8CurFilepos = u8TmpSampleOffset;/*prTmpSample->ui4_relative_offset*/;
        _SWDMX_Mp4SetFeederInvalid(prSwdmxInst, TRUE);
        if(u1HdrType == eSWDMX_MEDIA_SUBTITLE)
        {
            prSwdmxMp4Info->fgInvalidForSub = TRUE;
        }
        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
    }

    if((u8WriteOffset + prTmpSample->ui4_size) > (prSwdmxMp4Info->u4LeftBytesInFeeder))
    {
        //LOG(3, "fgFeederInvalid , left byte too small\n");
        _SWDMX_Mp4SetFeederInvalid(prSwdmxInst, TRUE);
        if(u1HdrType == eSWDMX_MEDIA_SUBTITLE)
        {
            prSwdmxMp4Info->fgInvalidForSub = TRUE;
        }
        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
    }

    prSwdmxMp4Info->u4CurDmxFeederRPtr += u8WriteOffset;
    if(prSwdmxMp4Info->u4CurDmxFeederRPtr >= VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
    {
         prSwdmxMp4Info->u4CurDmxFeederRPtr = 
            prSwdmxMp4Info->u4CurDmxFeederRPtr - 
            VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) + 
            VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
    }

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        //replace length with start code            
        if (prSwdmxMp4Info->u4VDecType == ENUM_SWDMX_VDEC_H264)
        {
            UCHAR* pucBuf = NULL;
            
            pucFdrBuf = (UCHAR*)prSwdmxMp4Info->u4CurDmxFeederRPtr ;
            pucBuf = pucFdrBuf;
            u4FrmSize = 0;
            
            if (prSwdmxMp4Info->u1StartCodeLenMinus1 <= H264_STARTCODE_LEN)
            {
                while(u4FrmSize < prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size)
                {
                    u4NaluSize = 0;
                    //insert start code
                    for(i = 0; i < prSwdmxMp4Info->u1StartCodeLenMinus1; i++)
                    {
                        u4NaluSize += (UINT32)*pucFdrBuf;
                        u4NaluSize <<= 8;
                        *pucFdrBuf++ = 0;
                        //check ring buffer
                        if((UINT32)pucFdrBuf == VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                        {
                            pucFdrBuf = (UCHAR*)VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
                        }
                    }
                    u4NaluSize += (UINT32)*pucFdrBuf;
                    if (u4NaluSize == 0)
                    {
                        prSwdmxMp4Info->u8VidLatestOff = prIdxTbl->ui8_base_offset +
                                 prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset + 
                                                         prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
                        prSwdmxMp4Info->u4NumSentVid++;
                        prSwdmxMp4Info->u4CurVidPts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
                        prSwdmxMp4Info->u4LeftBytesInFeeder -= 
                                            (u8WriteOffset + prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size);
                        prSwdmxMp4Info->u4CurDmxFeederRPtr += prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
                        if(prSwdmxMp4Info->u4CurDmxFeederRPtr >= VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                       {
                             prSwdmxMp4Info->u4CurDmxFeederRPtr = 
                                         prSwdmxMp4Info->u4CurDmxFeederRPtr - 
                                         VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) + 
                                         VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
                       }
   
                        FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, 
                            VIRTUAL(prSwdmxMp4Info->u4CurDmxFeederRPtr));

                        x_memcpy(&prSwdmxMp4Info->rPrevSample, 
                            prTmpSample, sizeof(MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T));
                        prSwdmxMp4Info->u8PrevSampleOffset = u8TmpSampleOffset;
                        
                       (*pu4CurIdx)++;
                	      return eSWDMX_HDR_PRS_SUCCEED;
                    }
                    
                    *pucFdrBuf++ = 1;

                    if((prSwdmxMp4Info->u1StartCodeLenMinus1 < H264_STARTCODE_LEN) &&
                        (prSwdmxMp4Info->u1StartCodeLenMinus1 > 0))
                    {
                        UINT32   u4Loop = 0;
                        UINT32   u4DataSize = 0;
                        
#if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5396) ||defined(CC_MT5368) ||defined(CC_MT5389)
                        HalFlushInvalidateDCacheMultipleLine(prSwdmxInst->rFeederInfo.u4StartAddr, 
                            (UINT32)(prSwdmxInst->rFeederInfo.u4EndAddr - prSwdmxInst->rFeederInfo.u4StartAddr));
#else
                        HalFlushInvalidateDCache();
#endif
                        for(u4Loop=0; u4Loop<(H264_STARTCODE_LEN - prSwdmxMp4Info->u1StartCodeLenMinus1); u4Loop++)
                        {
                            if((UINT32)pucBuf == VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr))
                            {
                                pucBuf = (UCHAR*)VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr - 1);
                            }
                            else
                            {
                                pucBuf--;
                            }

                            *pucBuf = 0;
                        }

                        u4DataSize = u4NaluSize + (H264_STARTCODE_LEN-
                                            prSwdmxMp4Info->u1StartCodeLenMinus1)+
                                            (prSwdmxMp4Info->u1StartCodeLenMinus1 + 1);
                        
                        rDmxMMData.u4BufStart = VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
                        rDmxMMData.u4BufEnd = VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr);
                        rDmxMMData.u4StartAddr = (UINT32)pucBuf;
                        rDmxMMData.u4FrameSize = u4DataSize;
                        rDmxMMData.u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
                        rDmxMMData.u1Idx = prSwdmxMp4Info->u1VidPid;

                        u4DmxVdecAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, 
                                                                                            DMX_PID_TYPE_ES_VIDEO,
                                                                                            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid);
                        if(u4DataSize >= u4DmxVdecAvailSize)
                        {
                            _SWDMX_Mp4SetFeederInvalid(prSwdmxInst, TRUE);
                            return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
                        }
                        
                        if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
                        {
                            LOG(1, "Demuxer fails to move data.\n");
                            return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                        }
                    }

                    //check ring buffer
                    if((UINT32)pucFdrBuf == VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                    {
                        pucFdrBuf = (UCHAR*)VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
                    }
                    // nalu size
                    u4FrmSize += ((prSwdmxMp4Info->u1StartCodeLenMinus1 + 1) + u4NaluSize);
                    pucFdrBuf += u4NaluSize;
                    //check ring buffer
                    if((UINT32)pucFdrBuf >= VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                    {
                        pucFdrBuf = 
                        (UCHAR*)((UINT32)pucFdrBuf - 
                        VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) + 
                        VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                    }
                    pucBuf = pucFdrBuf;
                }
            }
#if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5396) ||defined(CC_MT5368) ||defined(CC_MT5389)
            HalFlushInvalidateDCacheMultipleLine(prSwdmxInst->rFeederInfo.u4StartAddr, 
                (UINT32)(prSwdmxInst->rFeederInfo.u4EndAddr - prSwdmxInst->rFeederInfo.u4StartAddr));
#else
            HalFlushInvalidateDCache();
#endif
        }

        if(((UINT32)(prSwdmxMp4Info->u8SeekPts>>32) != 0xFFFFFFFF)
            || ((UINT32)(prSwdmxMp4Info->u8SeekPts) != 0xFFFFFFFF))
        {
            UINT32 u4Pts, u4CurSTC;
            
            u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
            if(u4Pts > (UINT32)prSwdmxMp4Info->u8SeekPts)
            {
                STC_SetStcValue(prSwdmxInst->u1StcId, u4Pts - 18000);
                u4CurSTC = STC_GetStcValue(prSwdmxInst->u1StcId);                
                LOG(1, "Got Vid T PTS 0x%x, STC 0x%x, Seek 0x%x\n", 
                    u4Pts, u4CurSTC, (UINT32)prSwdmxMp4Info->u8SeekPts);            
                prSwdmxMp4Info->u8SeekPts = 0xFFFFFFFF;
                prSwdmxMp4Info->u8SeekPts <<= 32;
                prSwdmxMp4Info->u8SeekPts += 0xFFFFFFFF;
            }
        }
    }
        
    if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {  
        if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prDmxRangeInfo->e_mm_src_type ==
            MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        {
		        if (prSwdmxMp4Info->fgEnVideo &&
		            (!prSwdmxMp4Info->fgPartialRangeDone) &&
		            (prSwdmxMp4Info->u4CurAudPts > (prSwdmxMp4Info->u4CurVidPts + MP4_ONE_SEC_DURATION)))
		        {
		            LOG(5, "Audio is enough, send video data in the same timeline\n");
		            prSwdmxMp4Info->fgBlockVid = FALSE;
		            prSwdmxMp4Info->fgBlockAud = TRUE;
		            _SWDMX_Mp4SetFeederInvalid(prSwdmxInst, TRUE);
		        }
		        else if (prSwdmxMp4Info->fgEnVideo &&
		            (!prSwdmxMp4Info->fgPartialRangeDone) && 
		            (prSwdmxMp4Info->u4CurAudPts > prSwdmxMp4Info->u4CurVidPts))
		        {
		             prSwdmxMp4Info->fgBlockVid = FALSE;
		             prSwdmxMp4Info->fgBlockAud = FALSE;
		        }
		    }

        if(((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8ASeekPts>>32) != 0xFFFFFFFF)
            || ((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8ASeekPts) != 0xFFFFFFFF))
        {
            UINT32 u4Pts;

            u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
            if(u4Pts > (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8ASeekPts)
            {
                AUD_SetStartPts(0, u4Pts);         

                if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TriggerAudPts = (UINT32)u4Pts;
                }	  

                LOG(7, "Got Aud T PTS 0x%x, Seek 0x%x\n", 
                u4Pts, (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8ASeekPts);            
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8ASeekPts = 0xFFFFFFFF;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8ASeekPts <<= 32;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8ASeekPts += 0xFFFFFFFF;
            }
            else
            {
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8AudLatestOff = prIdxTbl->ui8_base_offset +
                prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset +
                prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4LeftBytesInFeeder -= 
                (u8WriteOffset + prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size);

                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurDmxFeederRPtr += prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
                if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurDmxFeederRPtr >= VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurDmxFeederRPtr = 
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurDmxFeederRPtr - 
                    VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) + 
                    VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
                } 
                FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurDmxFeederRPtr));		
                //record the previous sample data
                x_memcpy(&prSwdmxInst->rFmtInfo.rSwdmxMp4Info.rPrevSample, prTmpSample, sizeof(MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T));
                prSwdmxMp4Info->u8PrevSampleOffset = u8TmpSampleOffset;
                
                (*pu4CurIdx)++;

                return eSWDMX_HDR_PRS_SUCCEED;
            }
        }	
        if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgSeekAfterPause)
        {
            UINT32 u4Pts = 0;
            UINT32 u4PlayPts = VDP_GetPts(prSwdmxInst->u1B2rId);

            u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
            if(u4Pts > u4PlayPts)
            {
                AUD_SetStartPts(0, u4Pts);         

                if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TriggerAudPts = (UINT32)u4Pts;
                }
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgSeekAfterPause = FALSE;
            }
            else
            {
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8AudLatestOff = prIdxTbl->ui8_base_offset +
                prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset +
                prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4LeftBytesInFeeder -= 
                (u8WriteOffset + prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size);

                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurDmxFeederRPtr += prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
                if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurDmxFeederRPtr >= VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurDmxFeederRPtr = 
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurDmxFeederRPtr - 
                    VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) + 
                    VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
                } 
                FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurDmxFeederRPtr));		
                //record the previous sample data
                x_memcpy(&prSwdmxInst->rFmtInfo.rSwdmxMp4Info.rPrevSample, prTmpSample, sizeof(MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T));
                prSwdmxMp4Info->u8PrevSampleOffset = u8TmpSampleOffset;

                (*pu4CurIdx)++;

                return eSWDMX_HDR_PRS_SUCCEED;
            }
        }
        if (prSwdmxMp4Info->u4ADecType == ENUM_SWDMX_ADEC_AAC)
        {
            UINT8 u1SampleRateIdx = 3;
            UINT32 u4RequestSize = AAC_ALLOC_SIZE;
            UINT32 u4Align = 16;
            if(prSwdmxInst->pucAudBuf == 0)
            {
                LOG(1, "pucAudBuf should be allocated in init function");
#if 0           
                prSwdmxInst->pucAudBuf = 
                    (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(AAC_ALLOC_SIZE,16));                           
#else
                _SWDMX_GetInfo(prSwdmxInst->u1SwdmxId,eSWDMX_GET_INTERNAL_BUF, (UINT32*)prSwdmxInst->pucAudBuf, &u4RequestSize, &u4Align); 
#endif
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
                switch (prSwdmxMp4Info->u4AudSampleRate)
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
                     ((prSwdmxMp4Info->u4AudChannelNs >> 2) & 0x1);
                /* 2b: channel_configuration */
                /* 1b: original */
                /* 1b: home */
                /* 1b: copyright_id */
                /* 1b: copyright_id_start */
                /* 2b: aac_frame_length */
                prSwdmxInst->pucAudBuf[3] = ((prSwdmxMp4Info->u4AudChannelNs & 0x3) << 6) |
                         (((prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size + AAC_FAKE_HDR_SIZE) >> 11) & 0x3);
                /* 8b: aac_frame_length */
                prSwdmxInst->pucAudBuf[4] = (((prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size + AAC_FAKE_HDR_SIZE) >> 3) & 0xFF);
                /* 3b: aac_frame_length */
                /* 5b: adts_buffer_fullness */
                prSwdmxInst->pucAudBuf[5] = (((prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size + AAC_FAKE_HDR_SIZE) << 5) & 0xE0) |
                         ((0x7FF >> 6) & 0x1F);
                /* 6b: adts_buffer_fullness */
                /* 2b: number_of_raw_data_blocks_in_frame */
                prSwdmxInst->pucAudBuf[6] = ((0x7FF << 2) & 0xFC);
                rDmxMMData.u4BufStart = (UINT32)prSwdmxInst->pucAudBuf;
                rDmxMMData.u4BufEnd = rDmxMMData.u4BufStart + AAC_ALLOC_SIZE;
                rDmxMMData.u4StartAddr = rDmxMMData.u4BufStart;
                rDmxMMData.u4FrameSize = AAC_FAKE_HDR_SIZE;
                rDmxMMData.u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
                rDmxMMData.u1Idx =prSwdmxMp4Info->u1AudPid;
     //if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
                if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
                {
                    LOG(1, "Demuxer fails to move data. SrcID = %d, Header Type = %d\n",
                        prSwdmxInst->u1SwdmxId,
                        u1HdrType);
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                } 
            }
        }
    }
    if(u1HdrType == eSWDMX_MEDIA_SUBTITLE) 
    {
        UINT32 u4CurPts = VDP_GetPts(prSwdmxInst->u1B2rId);

        if(prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts < u4CurPts)
        {
             prSwdmxMp4Info->u4LeftBytesInFeeder -= 
                 (u8WriteOffset + prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size);
            
             prSwdmxMp4Info->u4CurDmxFeederRPtr += prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
             if(prSwdmxMp4Info->u4CurDmxFeederRPtr >= VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
             {
                 prSwdmxMp4Info->u4CurDmxFeederRPtr = 
                     prSwdmxMp4Info->u4CurDmxFeederRPtr - 
                     VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) + 
                     VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
             }
            
             FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, VIRTUAL(prSwdmxMp4Info->u4CurDmxFeederRPtr));
            
             //record the previous sample data
             x_memcpy(&prSwdmxMp4Info->rPrevSample, 
                 prTmpSample,
                 sizeof(MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T));

             prSwdmxMp4Info->u8PrevSampleOffset = u8TmpSampleOffset;
             
             (*pu4CurIdx)++;
            
            return eSWDMX_HDR_PRS_SUCCEED;
        }
    }
    
    if(u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        prSwdmxMp4Info->u4LastVidPts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
    }

    if(!((u1HdrType == eSWDMX_MEDIA_VIDEO) &&
        (prSwdmxMp4Info->u1StartCodeLenMinus1 < H264_STARTCODE_LEN) &&
        (prSwdmxMp4Info->u1StartCodeLenMinus1 > 0)))
    {
       
        switch (u1HdrType)
        {
            case eSWDMX_MEDIA_VIDEO:
                
                 rDmxMMData.u1Idx = prSwdmxMp4Info->u1VidPid;
                break;
            case eSWDMX_MEDIA_AUDIO:
                 rDmxMMData.u1Idx = prSwdmxMp4Info->u1AudPid;
                break;
            // To Do
            case eSWDMX_MEDIA_SUBTITLE:
                rDmxMMData.u1Idx = prSwdmxMp4Info->u1SubPid;
				break;
            default:
                LOG(3, "Wrong Pid\n");
                return eSWDMX_HDR_PRS_FAIL;
        }
        rDmxMMData.u4BufStart = VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
        rDmxMMData.u4BufEnd = VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr);
        rDmxMMData.u4StartAddr = prSwdmxMp4Info->u4CurDmxFeederRPtr;
        rDmxMMData.u4FrameSize = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
        rDmxMMData.u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;

        //if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
        if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
        {
            LOG(1, "Demuxer fails to move data. SrcID = %d, Header Type = %d\n",
                prSwdmxInst->u1SwdmxId,
                u1HdrType);
            return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
        }
    }

    if((u1HdrType == eSWDMX_MEDIA_VIDEO) && 
        (prSwdmxMp4Info->u8VidLatestOff <= prIdxTbl->ui8_base_offset + prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset))
    {
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u8VidLatestOff = prIdxTbl->ui8_base_offset +
            prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset + 
            prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
        prSwdmxMp4Info->u4NumSentVid++;
		
	      if (prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts > 
            prSwdmxMp4Info->u4CurVidPts)
        {
            prSwdmxMp4Info->u4CurVidPts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
	      }  
        LOG(6, "prSwdmxMp4Info->u4CurVidPts = 0x%08x\n", prSwdmxMp4Info->u4CurVidPts);
    }
    else if(u1HdrType == eSWDMX_MEDIA_AUDIO && 
        (prSwdmxMp4Info->u8AudLatestOff  <= prIdxTbl->ui8_base_offset + prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset) )
    {
        prSwdmxMp4Info->u8AudLatestOff = prIdxTbl->ui8_base_offset +
            prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset +
            prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
        prSwdmxMp4Info->u4CurAudPts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
        //prSwdmxMp4Info->u4NumSentAud++;
        LOG(7, "prSwdmxMp4Info->u4CurAudPts = 0x%08x\n", prSwdmxMp4Info->u4CurAudPts);
    }
                                                                    
    prSwdmxMp4Info->u4LeftBytesInFeeder -= 
        (u8WriteOffset + prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size);

    prSwdmxMp4Info->u4CurDmxFeederRPtr += prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
    if(prSwdmxMp4Info->u4CurDmxFeederRPtr >= VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
    {
        prSwdmxMp4Info->u4CurDmxFeederRPtr = 
            prSwdmxMp4Info->u4CurDmxFeederRPtr - 
            VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) + 
            VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
    }
   
    FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, VIRTUAL(prSwdmxMp4Info->u4CurDmxFeederRPtr));

    //record the previous sample data
    x_memcpy(&prSwdmxMp4Info->rPrevSample, 
        prTmpSample,
        sizeof(MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T));

    prSwdmxMp4Info->u8PrevSampleOffset = u8TmpSampleOffset;
    
    (*pu4CurIdx)++;

   return eSWDMX_HDR_PRS_SUCCEED;
}

BOOL _SWDMX_Mp4SetStrmID(
    UINT8 u1SrcId,
    UINT8 u1HdrType,  
    UINT32 u4StrmID, 
    UINT32 u4StrmSubID)
{
    SWDMX_INFO_T* prSwdmxInst = NULL;
    SWDMX_MP4_INFO_T *prSwdmxMp4Info = NULL;
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    prSwdmxMp4Info = (SWDMX_MP4_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMp4Info;
    UNUSED(u4StrmSubID);
    SWDMX_MP4_VERIFY_NULL_ERROR(prSwdmxInst);  

    LOG(5, "%s source id = %d\n", __FUNCTION__, u1SrcId);

    switch(u1HdrType)
    {
        case eSWDMX_STRM_TYPE_VID:
            prSwdmxMp4Info->u4VStrmID = u4StrmID;
            break;
        case eSWDMX_STRM_TYPE_AUD:
            prSwdmxMp4Info->u4AStrmID = u4StrmID;
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            prSwdmxMp4Info->u4SubTitleID = u4StrmID;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_Mp4EnableStrm(
    UINT8 u1SrcId,
    UINT8 u1StrmType, 
    VOID* pfnCbFunc)
{
    //BOOL fgFindATrack;
    //INT32 i;

    SWDMX_INFO_T* prSwdmxInst = NULL;
    SWDMX_MP4_INFO_T *prSwdmxMp4Info = NULL;
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    prSwdmxMp4Info = (SWDMX_MP4_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMp4Info;
    SWDMX_MP4_VERIFY_NULL_ERROR(prSwdmxInst); 

    LOG(5, "%s source id = %d\n", __FUNCTION__, u1SrcId);

    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            prSwdmxMp4Info->fgEnVideo = TRUE;
            if (prSwdmxMp4Info->fgEnVDmx == FALSE)
            {
                _SWDMX_InitDmx(prSwdmxInst, eSWDMX_MEDIA_VIDEO,NULL, FALSE);
                prSwdmxMp4Info->fgEnVDmx = TRUE;
            }
            break;
        case eSWDMX_STRM_TYPE_AUD:
            if (prSwdmxMp4Info->fgEnADmx == FALSE)
            {
                _SWDMX_InitDmx(prSwdmxInst, eSWDMX_MEDIA_AUDIO, NULL, FALSE);
                prSwdmxMp4Info->fgEnADmx = TRUE;
            }
            /*fgFindATrack = FALSE;
            for (i = 0; i < prSwdmxMp4Info->u1TotalATrack; i++)
            {
                if (prSwdmxMp4Info->prAudIdxTbl[i] && 
                    prSwdmxMp4Info->prAudIdxTbl[i]->t_strm_id.u.t_stm_mp4_id == prSwdmxMp4Info->u4AStrmID)
                {
                    prSwdmxMp4Info->u1CurATrack = i;
                    fgFindATrack = TRUE;
                }
            }
            if (!fgFindATrack)
            {
                LOG(3, "_SWDMX_Mp4EnableStrm didn't find audio stream\n");
                return FALSE;
            }
            if (prSwdmxMp4Info->u4NumSentAud != 0)
            {
                prSwdmxMp4Info->fgChgATrack = TRUE;
            }*/
            if (/*(prSwdmxMp4Info->u4PrevAStrmID != 0xFFFFFFFF)&&*/
                 (prSwdmxMp4Info->u4PrevAStrmID != prSwdmxMp4Info->u4AStrmID)&&
                 prSwdmxMp4Info->fgBeginToPlay)
            {
                prSwdmxMp4Info->fgChgATrack = TRUE;
            }
            else if (prSwdmxMp4Info->fgAudStrmDisable && !prSwdmxMp4Info->fgRecSeekFlag &&
            	!prSwdmxMp4Info->fgBeginToPlay)
            {
                LOG(5, "Audio stream is disabled and seek is not performed\n");
                prSwdmxMp4Info->fgChgATrack = TRUE;
                prSwdmxMp4Info->fgAudStrmDisable = FALSE;
            }
            if(!prSwdmxMp4Info->fgRecSeekFlag)
            {	
                prSwdmxMp4Info->fgWaitNewRange =  FALSE;
            }  
            prSwdmxMp4Info->fgRecSeekFlag = FALSE;
            prSwdmxMp4Info->u4PrevAStrmID = prSwdmxMp4Info->u4AStrmID;
            prSwdmxMp4Info->fgEnAudio = TRUE;
            break;
         case eSWDMX_STRM_TYPE_SUBTITLE:
            
            if (prSwdmxMp4Info->fgEnSubDmx == FALSE)
            {
                UNUSED(_SWDMX_InitDmx(prSwdmxInst, eSWDMX_MEDIA_SUBTITLE, pfnCbFunc, FALSE));
                prSwdmxMp4Info->fgEnSubDmx = TRUE;
            }
            prSwdmxMp4Info->fgChgSubTrack = TRUE;
            prSwdmxMp4Info->fgEnSubTitle = TRUE;
            break;
			
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_Mp4DisableStrm(
    UINT8 u1SrcId,
    UINT8 u1StrmType)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MP4_VERIFY_NULL_ERROR(prSwdmxInst);        

    LOG(5, "%s source id = %d\n", __FUNCTION__, u1SrcId);
    
    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:   
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo = FALSE;
            if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause)
            {
                _SWDMX_Mp4VideoWakeUp(prSwdmxInst);
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause = FALSE;
            }
            break;
        case eSWDMX_STRM_TYPE_AUD:
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnAudio = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgAudStrmDisable = TRUE;
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnSubTitle = FALSE;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

static BOOL _SWDMX_InitDmx(
    SWDMX_INFO_T* prSwdmxInst,
    UINT8 u1HdrType, 
    VOID* pfnCbFunc,
    BOOL fgEos)
{
    BOOL fgRet = FALSE;
    UINT32 u4Flag = 0; 
    DMX_MM_T rDmxMMInfo;

    SWDMX_MP4_VERIFY_NULL_ERROR(prSwdmxInst);        

    LOG(5, "%s source id = %d\n", __FUNCTION__, prSwdmxInst->u1SwdmxId);

    x_memset((void*)&rDmxMMInfo, 0, sizeof(DMX_MM_T));
    
    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        if (!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVDmx)
        {
            UINT32 u4VidFifoAddr, u4VidFifoSize;

            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid = 
                DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);

#if 0
            // Allocate video buffer from FBM
            prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
            if((prFbmPool == NULL) || (prFbmPool->u4Addr == 0))
            {
                LOG(1, "_SWDMX_SetVideoStreamID (prFbmPool == NULL) || (prFbmPool->u4Addr == NULL)\n");
                return FALSE;
            }
#endif
            _SWDMX_GetVideoFifo(prSwdmxInst->u1SwdmxId, &u4VidFifoAddr, &u4VidFifoSize);

            u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_VALID |
                DMX_MM_FLAG_BUF | DMX_MM_FLAG_INSTANCE_TAG |
                DMX_MM_FLAG_DEVICE_ID);

            rDmxMMInfo.fgAllocBuf = FALSE;
            rDmxMMInfo.u4BufAddr = u4VidFifoAddr;
            rDmxMMInfo.u4BufSize = u4VidFifoSize;
            rDmxMMInfo.u1ChannelId = prSwdmxInst->u1VdecId;
            rDmxMMInfo.pvInstanceTag = (void*)prSwdmxInst;
            if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VDecType ==
                (UINT32)ENUM_SWDMX_VDEC_MJPEG)
            {
                rDmxMMInfo.u1DeviceId = VLD1; // video format all use VLD expect jpeg
            }
            else
            {        
                rDmxMMInfo.u1DeviceId = VLD0; // video format all use VLD expect jpeg
            }
        }

        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;

        u4Flag |= DMX_MM_FLAG_SEARCH_START_CODE;

        switch (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VDecType)
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

        if (fgEos)
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;
        }

        fgRet = DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid, u4Flag, &rDmxMMInfo);
    }
    else if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        //UINT32 au4BufStart[2], au4BufEnd[2];
        UINT32 u4AudFifoAddr, u4AudFifoSize;
    
        u4Flag = (DMX_MM_FLAG_TYPE | 
          DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF |
          DMX_MM_FLAG_INSTANCE_TAG |
          DMX_MM_FLAG_DEVICE_ID);

        if (!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnADmx)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1AudPid = 
                    DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);
        }

        _SWDMX_GetAudioFifo(prSwdmxInst->u1SwdmxId, &u4AudFifoAddr, &u4AudFifoSize);
        
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_AUDIO;
        rDmxMMInfo.pvInstanceTag = (void*)prSwdmxInst;
        rDmxMMInfo.u1ChannelId = prSwdmxInst->u1AdecId;
        rDmxMMInfo.u1DeviceId = prSwdmxInst->u1AdecId;

#if 0        
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
#endif

        rDmxMMInfo.fgAllocBuf = FALSE;
        
        rDmxMMInfo.u4BufAddr = u4AudFifoAddr;
        rDmxMMInfo.u4BufSize = u4AudFifoSize;

        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4AudBufSize = rDmxMMInfo.u4BufSize;

        if (fgEos)
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;
        }

        fgRet = DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1AudPid, u4Flag, &rDmxMMInfo);
    }
    else if (u1HdrType == eSWDMX_MEDIA_SUBTITLE)
    {
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_PES;
        
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1SubPid = 
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
            rDmxMMInfo.u4BufSize = MP4_SUBTITLE_BUFSIZE;

            fgRet = DMX_MM_Set( prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1SubPid , u4Flag, &rDmxMMInfo);
        }
    }

    return fgRet;
}


static BOOL _SWDMX_SendMp4Eos(
    SWDMX_INFO_T* prSwdmxInst,
    UINT8 u1HdrType)
{
    UINT8 u1PidIdx;
    UINT16 u2MaxQNum, u2CurQNum;
    //UINT32 u4DummyData[4];
    DMX_MM_DATA_T rDmxMMData;

    SWDMX_MP4_VERIFY_NULL_ERROR(prSwdmxInst);        

    LOG(5, "%s source id = %d\n", __FUNCTION__, prSwdmxInst->u1SwdmxId);
    LOG(5, "SWDMX Send Mp4 Eos Hdr Type = %d\n", u1HdrType);

    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) != 
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
        VDEC_SetTotalTime(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TotalTime);
        LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
        //if (u2CurQNum > (_rSwdmxMp4Info.u4VidFps/2))
        if (u2CurQNum > (u2MaxQNum - 10))
        {
            x_thread_delay(1);
            return FALSE;
        }
        u1PidIdx = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid;
    }
    else
    {
        u1PidIdx = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1AudPid;
    }
    
    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if (_SWDMX_InitDmx(prSwdmxInst, u1HdrType, NULL, TRUE) == FALSE)
    {
        return FALSE;
    }

    rDmxMMData.u4BufStart = prSwdmxInst->rFeederInfo.u4StartAddr;
    rDmxMMData.u4BufEnd = prSwdmxInst->rFeederInfo.u4EndAddr;
    rDmxMMData.u4StartAddr = prSwdmxInst->rFeederInfo.u4StartAddr;
    rDmxMMData.u4FrameSize = 4;
    rDmxMMData.fgEOS = TRUE;
    rDmxMMData.u1Idx = u1PidIdx;
    
    //return DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500);
    return _SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData);
}


/**
*/
BOOL _SWDMX_Mp4Stop(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = NULL;
    DMX_MM_T rPid;
    rPid.fgEnable = FALSE;
    prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    SWDMX_MP4_VERIFY_NULL_ERROR(prSwdmxInst);      

    if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVDmx)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVDmx = FALSE;

        VERIFY(DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid);
        DMX_MUL_FreePidx(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1VidPid);
    }
    if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnADmx)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnADmx = FALSE;
        VERIFY(DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1AudPid, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1AudPid);
        DMX_MUL_FreePidx( prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1AudPid );
    }
    if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnSubDmx)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnSubDmx = FALSE;
        VERIFY(DMX_MM_Set( prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1SubPid , DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free( prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1SubPid );
        DMX_MUL_FreePidx( prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1SubPid );
    }
    x_memset(&prSwdmxInst->rFmtInfo.rSwdmxMp4Info, 0, sizeof(SWDMX_MP4_INFO_T));

    return TRUE;
}

static UINT32 _SWDMX_Mp4SeekCurPos(SWDMX_INFO_T* prSwdmxInst)
{
	VDP_POS_INTO_T rVdpPosInfo;
    SWDMX_MP4_INFO_T *prSwdmxMp4Info = (SWDMX_MP4_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMp4Info;
    MM_RANGE_ELMT_IDXTBL_T* prIdxTbl = NULL;   //current range
    UINT32 u4StartPTS=0xffffffff,u4EndPTS,u4TargetPTS=0xffffffff,i=0;
	
    prSwdmxInst->rCurRangeReq.fgIsSeekTarget = FALSE;
	if(!prSwdmxMp4Info->fgEnVDmx)
	{
       return FALSE;
	}
	
	if(VDP_GetPosInfo(prSwdmxInst->u1B2rId,&rVdpPosInfo))
	{
        //u4TargetPTS=rVdpPosInfo.u8PtsI;
        prIdxTbl = prSwdmxMp4Info->prVidIdxTbl;
        prSwdmxMp4Info->u4TriggerAudPts=rVdpPosInfo.u8AudPts;
        prSwdmxMp4Info->u4LastVidPts = rVdpPosInfo.u8Pts;
		if(prIdxTbl)
		{
			u4StartPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts;
			u4EndPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[prIdxTbl->ui4_number_of_entry - 1].ui4_pts;
			if(rVdpPosInfo.u8PtsI<u4StartPTS || rVdpPosInfo.u8PtsI>u4EndPTS)
			{
               return (UINT32)rVdpPosInfo.u8PtsI;
			}
			else
			{
               u4TargetPTS=(UINT32)rVdpPosInfo.u8PtsI;
			}
			
			prSwdmxMp4Info->u8CurFilepos=rVdpPosInfo.u8Offset;
			prSwdmxInst->rCurRangeReq.fgIsSeekTarget=TRUE;
            //set video range idx to the entry with target PTS
            for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
            {
                if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts == u4TargetPTS)
                {
                    prSwdmxInst->rCurRangeReq.u4CurRangeVidIdx = i;
                    prSwdmxMp4Info->u4CurVidPts = u4TargetPTS;
                    break;
                }
            }
		}
		else
		{
           prSwdmxMp4Info->u4CurVidPts=(UINT32)rVdpPosInfo.u8Pts;
		}

		if(prSwdmxMp4Info->u1CurATrack<MP4_MAX_AUDIO_TRACK)
		{
			prIdxTbl=prSwdmxMp4Info->prAudIdxTbl[prSwdmxMp4Info->u1CurATrack];
		}
		if(prIdxTbl&&(prIdxTbl->ui4_number_of_entry != 0))
		{			 
			for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
			{
				if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts >=u4TargetPTS)
				{
					prSwdmxInst->rCurRangeReq.u4CurRangeAudIdx = i;
					prSwdmxMp4Info->fgHasAud = TRUE;
					prSwdmxMp4Info->u4CurAudPts = prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts;
					LOG(5, "Audio PTS = 0x%08x indx = %x\n",prSwdmxMp4Info->u4CurAudPts,prSwdmxInst->rCurRangeReq.u4CurRangeAudIdx);
					break;
				}
			}
			if (!prSwdmxMp4Info->fgHasAud)
			{
				prSwdmxInst->rCurRangeReq.u4CurRangeAudIdx = prIdxTbl->ui4_number_of_entry;
			}
		}
		else
		{
            prSwdmxMp4Info->fgHasAud=FALSE;
		}
		
        if(prSwdmxMp4Info->u1CurSubTrack < MP4_MAX_SUB_TRACK) 
        {
            prIdxTbl = prSwdmxMp4Info->prSubIdxTbl[prSwdmxMp4Info->u1CurSubTrack];
        }

		if(prIdxTbl&&(prIdxTbl->ui4_number_of_entry != 0))
        {            
            for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
            {
                if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts >= u4TargetPTS)
                {
                    prSwdmxInst->rCurRangeReq.u4CurRangeSubIdx = i;
                    LOG(5, "Subtitle PTS = 0x%08x\n", prSwdmxMp4Info->u4CurAudPts);
                    break;
                }
            }

            if (i >= prIdxTbl->ui4_number_of_entry)
            {
                prSwdmxInst->rCurRangeReq.u4CurRangeSubIdx = prIdxTbl->ui4_number_of_entry;
            }
           
            LOG(2, "_SWDMX_Mp4Seek Key = %ld, Key PTS = %ld\n", prSwdmxMp4Info->u4CurSentKey, u4TargetPTS);
        }	  
		return TRUE;
	}
	return FALSE;
}

static UINT32 _SWDMX_Mp4Seek(SWDMX_INFO_T* prSwdmxInst,
    UINT64 u8SeekTime, 
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed)
{
    //find previous key's PTS
    //if within cur range, 
    // 1. prRangeList->fgIsSeekTarget will be TRUE, 
    // 2. range idx will set to the entry with target PTS
    //else, update new range according to target PTS
    INT32 i;
    UINT32 u4TargetPTS = 0xFFFFFFFF, u4StartPTS, u4EndPTS;

    SWDMX_MP4_INFO_T *prSwdmxMp4Info = NULL;
    MM_RANGE_ELMT_IDXTBL_T* prIdxTbl = NULL;   //current range
    MM_RANGE_ELMT_IDXTBL_T* prVidKeyIdxTbl = NULL; 
    prSwdmxMp4Info = (SWDMX_MP4_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMp4Info;
    prVidKeyIdxTbl = prSwdmxMp4Info->prVidKeyIdxTbl;

    prRangeList->fgIsSeekTarget = FALSE;
    if (!prSwdmxInst)
    {
        return 0;
    }

    if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl) &&
        (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl) &&
        (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
        (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]))
    {
        BOOL fgTarget = FALSE;
        BOOL fgNextKeyTable = FALSE;
        do
        {
              fgTarget = FALSE;
            fgNextKeyTable = FALSE;
            
            for (i = 0; i < prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl->ui4_number_of_entry; i++)
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts > u8SeekTime)
                {
                    fgTarget = TRUE;
                    break;
                }
            }
            if(prVidKeyIdxTbl->pt_next_partial != NULL)
            {
                fgNextKeyTable = TRUE;
                if(!fgTarget)
                {
                    prVidKeyIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)(prVidKeyIdxTbl->pt_next_partial);
                    prSwdmxMp4Info->prVidKeyIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)(prSwdmxMp4Info->prVidKeyIdxTbl->pt_next_partial);
                }
            }
        }while(!fgTarget && fgNextKeyTable);
        
        //get current idx of key table array
        _SWDMX_Mp4SetKeyTableIdx(prSwdmxInst,prVidKeyIdxTbl);
        
        // Error Handling 
        // i = 0 is impossible
        if (i > 0)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey = i - 1;
        }
        else
        {
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey= 0;
        }

        if (i4PlaySpeed > 2000&&(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey+1<prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl->ui4_number_of_entry))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurRepeatFrameIdx = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey + 1;
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey++;
        }
        else
        {
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurRepeatFrameIdx = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey;
        }
		prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4LastSentKey=prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey+1;

        // check if target key frame PTS is in current range
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4LastVidPts = u8SeekTime;
        u4TargetPTS = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey].ui4_pts;
        prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl;
        u4StartPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts;
        u4EndPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[prIdxTbl->ui4_number_of_entry - 1].ui4_pts;
        if((u4TargetPTS + MP4_MAX_CTTS_PTS >= u4StartPTS) && (u4TargetPTS <= u4EndPTS))
        {
            LOG(5, "u4StartPTS = 0x%08x u4EndPts = 0x%08x u4TargetPTS = 0x%08x\n", 
                u4StartPTS, 
                u4EndPTS,
                u4TargetPTS);

            prRangeList->fgIsSeekTarget = TRUE;
	    if(u4TargetPTS <= u4StartPTS)
	    {
		prRangeList->u4CurRangeVidIdx = 0;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurVidPts = u4TargetPTS;
	    }
	    else
	    {
                //set video range idx to the entry with target PTS
                for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
                {
                    if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts == u4TargetPTS)
                    {
                        prRangeList->u4CurRangeVidIdx = i;
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurVidPts = u4TargetPTS;
                        break;
                    }
                }
            }
           //seamless playback ,when target key frame PTS is not in current range,return the seek time to mw
          //  prRangeList->u4CurRangeVidIdx = 0;


            // set audio range idx
            prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack];
            
            if(prIdxTbl&&
               (prIdxTbl->ui4_number_of_entry != 0))
            {            
		            for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
		            {
		                if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts >= u8SeekTime)
		                {
		                    prRangeList->u4CurRangeAudIdx = i;
		                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgHasAud = TRUE;
		                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts = prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts;
		                    LOG(5, "Audio PTS = 0x%08x indx = %x\n", prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts,  prRangeList->u4CurRangeAudIdx);
		                    break;
		                }
		            }
		
		            if (!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgHasAud)
		            {
		                prRangeList->u4CurRangeAudIdx = prIdxTbl->ui4_number_of_entry;
		            }
		           
		            LOG(2, "_SWDMX_Mp4Seek Key = %ld, Key PTS = %ld\n", 
		                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey, u4TargetPTS);
	          }
            
	          if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack < MP4_MAX_SUB_TRACK) 
	          {
	              prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack];
	          }
            if(prIdxTbl&&
             (prIdxTbl->ui4_number_of_entry != 0))
            {            
		            for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
		            {
		                if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts >= u8SeekTime)
		                {
		                    prRangeList->u4CurRangeSubIdx = i;
		                    LOG(5, "Subtitle PTS = 0x%08x\n", prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts);
		                    break;
		                }
		            }
		
		            if (i >= prIdxTbl->ui4_number_of_entry)
		            {
		                prRangeList->u4CurRangeSubIdx = prIdxTbl->ui4_number_of_entry;
		            }
		           
		            LOG(2, "_SWDMX_Mp4Seek Key = %ld, Key PTS = %ld\n", 
		                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey, u4TargetPTS);
            }	      
        }
    }
    else if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl &&
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl)
    {
        for (i = 0; i < prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl->ui4_number_of_entry; i++)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts > u8SeekTime)
            {
                break;
            }
        }

        // Error Handling 
        // i = 0 is impossible
        if (i > 0)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey = i - 1;
        }
        else
        {
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey= 0;
        }

        if (i4PlaySpeed > 2000)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurRepeatFrameIdx = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey + 1;
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey++;
        }
        else
        {
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurRepeatFrameIdx = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey;
        }

        // check if target key frame PTS is in current range
        u4TargetPTS = prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey].ui4_pts;
        prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prVidIdxTbl;
        u4StartPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts;
        u4EndPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[prIdxTbl->ui4_number_of_entry - 1].ui4_pts;
        if((u4TargetPTS + MP4_MAX_CTTS_PTS >= u4StartPTS) && (u4TargetPTS <= u4EndPTS))
        {
            LOG(5, "u4StartPTS = 0x%08x u4EndPts = 0x%08x u4TargetPTS = 0x%08x\n", 
                u4StartPTS, 
                u4EndPTS,
                u4TargetPTS);

            prRangeList->fgIsSeekTarget = TRUE;
            if(u4TargetPTS <= u4StartPTS)
            {
                prRangeList->u4CurRangeVidIdx = 0;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurVidPts = u4TargetPTS;
            }
            else
            {
                //set video range idx to the entry with target PTS
                for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
                {
                    if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts == u4TargetPTS)
                    {
                        prRangeList->u4CurRangeVidIdx = i;
                        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurVidPts = u4TargetPTS;
                        break;
                    }
                }
            }

            LOG(2, "_SWDMX_Mp4Seek Key = %ld, Key PTS = %ld\n", 
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey, u4TargetPTS);
        }
        if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack < MP4_MAX_SUB_TRACK) 
        {
            prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prSubIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurSubTrack];
        } 
         if(prIdxTbl&&
            (prIdxTbl->ui4_number_of_entry != 0))
         {            
             for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
             {
                 if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts >= u8SeekTime)
                 {
                     prRangeList->u4CurRangeAudIdx = i;
                     LOG(5, "Subtitle PTS = 0x%08x\n", prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts);
                     break;
                 }
             }
 
             if (i >= prIdxTbl->ui4_number_of_entry)
             {
                 prRangeList->u4CurRangeAudIdx = prIdxTbl->ui4_number_of_entry;
             }
            
             LOG(2, "_SWDMX_Mp4Seek Key = %ld, Key PTS = %ld\n", 
                 prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurSentKey, u4TargetPTS);
         }	

    }
    else if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack]))
    {
        u4TargetPTS = (UINT32)u8SeekTime;
        // set audio range idx
        prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMp4Info.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u1CurATrack];
        if(prIdxTbl->u.pt_pb_idx_tbl_entry!=NULL)
        {   
	        u4StartPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts;
	        
	        if(prIdxTbl->ui4_number_of_entry != 0)
	        {
	            u4EndPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[prIdxTbl->ui4_number_of_entry - 1].ui4_pts;
	        }
	        else
	        {
	            u4EndPTS = u4StartPTS;
	        }
	        
	        //u4EndPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[prIdxTbl->ui4_number_of_entry - 1].ui4_pts;
	        if(prIdxTbl->ui4_number_of_entry != 0)
	        {
		        for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
		        {
		            if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts >= u8SeekTime)
		            {
		                prRangeList->u4CurRangeAudIdx = i;
		                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgHasAud = TRUE;
		                prRangeList->fgIsSeekTarget = TRUE;
		                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurAudPts = prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts;
		                break;
		            }
		        }
		        
		        if((u8SeekTime < u4StartPTS)&&(!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo))
		        {
		                if(u4StartPTS > MP4_MAX_CTTS_PTS)
                            {
                                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgHasAud = FALSE;
                                prRangeList->fgIsSeekTarget = FALSE;
                            }
		        }
		        
		        if (!prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgHasAud)
		        {
		            prRangeList->u4CurRangeAudIdx = prIdxTbl->ui4_number_of_entry;
		        }
		    }
		    else
		    {
		       	prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgHasAud = FALSE;
		        prRangeList->fgIsSeekTarget = TRUE;
		    }
	    }
	    else
	    { 	       
	        prRangeList->fgIsSeekTarget = TRUE;
	        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgHasAud = FALSE;
	    }
    }

    return u4TargetPTS;
}


static BOOL _SWDMX_Mp4GetDataFromFeeder(SWDMX_INFO_T* prSwdmxInst,
    FEEDER_REQ_DATA_T *prFeederData)
{
    SWDMX_MP4_VERIFY_NULL_ERROR(prSwdmxInst);        

    if (!_SWDMX_ReceiveFeederAck(prSwdmxInst->u1SwdmxId, (VOID*)prFeederData))
    {
        LOG(3, "_SWDMX_Mp4GetDataFromFeeder FeederTimeOut!!!\n");
        return FALSE;        
    }

    if (prFeederData->eDataType == FEEDER_SOURCE_INVALID)
    {
         LOG(3, "_SWDMX_Mp4GetDataFromFeeder invalid. Current req times: %d\n", prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4ReqReadTimes);
         return FALSE;
    }
    
    //if ((prFeederData->u4Id + 1) != prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4FeederReqID)
    if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4ReqReadTimes == 0) || 
		(prFeederData->u4Id != (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4FeederReqID - prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4ReqReadTimes)))
    {
        LOG(3, "Feeder ID mismatch!!! feeder id = %ld\n", prFeederData->u4Id);
        _SWDMX_Mp4SetFeederInvalid(prSwdmxInst, TRUE);
        return FALSE;
    }


    return TRUE;
}


static BOOL _SWDMX_Mp4GetDataFromFeederNoWait(SWDMX_INFO_T* prSwdmxInst,
    FEEDER_REQ_DATA_T *prFeederData)
{
     
    INT32 i4Ret = 0;
    SWDMX_MP4_VERIFY_NULL_ERROR(prSwdmxInst);
    i4Ret = _SWDMX_ReceiveFeederAckNoWait(prSwdmxInst->u1SwdmxId, (VOID*)prFeederData);
   
    if (i4Ret != OSR_OK)
    {
        //LOG(3, "_SWDMX_Mp4GetDataFromFeederNoWait No Msg!!!\n");
        //_rSwdmxMp4Info.fgFeederInvalid = TRUE;
        return FALSE;        
    }

    if (prFeederData->eDataType == FEEDER_SOURCE_INVALID)
    {
         LOG(3, "_SWDMX_Mp4GetDataFromFeederNoWait invalid\n");
         //_rSwdmxMp4Info.fgFeederInvalid = TRUE;
         return FALSE;
    }
    
    //if ((prFeederData->u4Id + 1) != prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4FeederReqID)
    
    if ((prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4ReqReadTimes == 0) || 
		(prFeederData->u4Id != (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4FeederReqID - prSwdmxInst->rFmtInfo.rSwdmxMp4Info.i4ReqReadTimes)))
    {
        LOG(3, "_SWDMX_Mp4GetDataFromFeederNoWait Feeder ID mismatch!!! feeder id = %ld\n", prFeederData->u4Id);
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgFeederInvalid = TRUE;
        return FALSE;
    }


    return TRUE;
}


static BOOL _SWDMX_Mp4CalculateRepeatFrame(SWDMX_INFO_T* prSwdmxInst,
    INT32 i4PlaySpeed)
{
    SWDMX_MP4_INFO_T *prSwdmxMp4Info = NULL;
    MM_RANGE_ELMT_IDXTBL_T *prKeyIdxTbl = NULL;
    UINT32 u4CurKey = 0;
    prSwdmxMp4Info = (SWDMX_MP4_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMp4Info;
    u4CurKey = prSwdmxMp4Info->u4CurSentKey;
    prKeyIdxTbl = prSwdmxMp4Info->prVidKeyIdxTbl;
    SWDMX_MP4_VERIFY_NULL_ERROR(prSwdmxInst);   

    if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame > 0)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame--;
    }

    if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame == 0)
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurRepeatFrameIdx != u4CurKey &&
            (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurRepeatFrameIdx < prKeyIdxTbl->ui4_number_of_entry))
        {
            UINT32 u4PtsDif = 0, u4MaxRepeatFrame;
    
            u4MaxRepeatFrame = (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidFps * 1000 * 2)/abs(i4PlaySpeed);

            if (i4PlaySpeed > 2000)
            {
                if ((u4CurKey < (prKeyIdxTbl->ui4_number_of_entry)) &&
                    (prKeyIdxTbl->ui4_number_of_entry > MP4_MINIMUM_KEY_NUM))
                {
                    u4PtsDif = prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts -
                        prKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurRepeatFrameIdx].ui4_pts;

                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame = 
                        (UINT32)((u4PtsDif * prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidFps)/(90 * i4PlaySpeed));
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame = (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidFps * 1000)/abs(i4PlaySpeed);
                }
            }
            else if (i4PlaySpeed < 0)
            {
                if ((prKeyIdxTbl->ui4_number_of_entry > MP4_MINIMUM_KEY_NUM))
                {
                    u4PtsDif = prKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurRepeatFrameIdx].ui4_pts -
                        prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts;

                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame = 
                        (UINT32)((u4PtsDif * prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidFps)/(90 * abs(i4PlaySpeed)));
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame = (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidFps * 1000)/abs(i4PlaySpeed);
                }
            }

            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame = (u4MaxRepeatFrame > prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame) ?
                (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame) : u4MaxRepeatFrame;

            if ((u4CurKey == prKeyIdxTbl->ui4_number_of_entry - 1) ||
                (u4CurKey == 0))
            {
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame = 
                    (MP4_FIX_REPEAT_FRAME/(abs(i4PlaySpeed)/1000));

                if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame > MP4_MAX_REPEAT_FRAME)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame = MP4_MAX_REPEAT_FRAME;
                }
                else if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame < MP4_MIN_REPEAT_FRAME)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame = MP4_MIN_REPEAT_FRAME;
                }                        
            }

            LOG(5, "u4PtsDif = %ld Current Idx = %ld Repeat frame = %ld\n", 
                u4PtsDif, u4CurKey, prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame);

            if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame == 0)
            {
                return FALSE;
            }

            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurRepeatFrameIdx = u4CurKey;
            LOG(5, "Repeat Idx = %ld\n", prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4CurRepeatFrameIdx);

            return TRUE;
        }
        else
        {
            // Force to play the first key frame
            if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgStartTrick)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgStartTrick = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidRepeatFrame = (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4VidFps * 1000)/abs(i4PlaySpeed);
                return TRUE;
            }
            
            return FALSE;
        }
    }

    return TRUE;
}


BOOL _SWDMX_Mp4AudioCb(UINT8 u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MP4_VERIFY_NULL_ERROR(prSwdmxInst);        

    if(eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE)
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
        {
            LOG(5, "SWDMX_Mp4AudioCb Audio is ready\n");
            if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TriggerAudPts != 0xFFFFFFFF)
            {
            	VDP_TriggerAudReceive(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TriggerAudPts);
            	LOG(5, "SWDMX_Mp4AudioCb VDP Trigger PTS = 0x%08x\n", 
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TriggerAudPts);
            	prSwdmxInst->rFmtInfo.rSwdmxMp4Info.u4TriggerAudPts = 0xFFFFFFFF;
            }

            if(prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause)
            {
                _SWDMX_Mp4VideoWakeUp(prSwdmxInst);
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgVDecIsPause = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgBlockVid = FALSE;
            }
            prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgAudIsReady = TRUE;
        }
        else 
        {
            // Audio Only case
            LOG(5, "Trigger audio by _SWDMX_Mp4AudioCb\n");
            AUD_MMAoutEnable(0, TRUE);
            STC_StartStc(prSwdmxInst->u1StcId);
        }
    }
    else
    {
        // impossible
        LOG(11, "_SWDMX_Mp4AudioCb eAudioNotifyType != ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE\n");
    }

    return TRUE;
}

static void _SWDMX_Mp4VideoWakeUp(SWDMX_INFO_T* prSwdmxInst)
{
    SWDMX_MP4_INFO_T *prSwdmxMp4Info = NULL;
    prSwdmxMp4Info = (SWDMX_MP4_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMp4Info;
    SWDMX_MP4_VERIFY_NULL(prSwdmxInst);    
    VDP_SetPauseMm(prSwdmxInst->u1B2rId, FALSE);
    LOG(5, "_SWDMX_Mp4VideoWakeUp\n");
    switch(prSwdmxMp4Info->u4VDecType)
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
            break;
        case ENUM_SWDMX_VDEC_MPEG1_MPEG2:
            VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MPV);
            break;
        case ENUM_SWDMX_VDEC_H264:
            VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_H264);
            break;
        default:
            LOG(1, "SWDMX_Mp4AudioCb Unknown Video Type\n");
            break;
    }
}

static void _SWDMX_Mp4SetFeederInvalid(SWDMX_INFO_T* prSwdmxInst, BOOL fgFlag)
{
    prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgFeederInvalid = fgFlag;
}

static BOOL _SWDMX_Mp4CheckNextKeyTable(SWDMX_INFO_T* prSwdmxInst,INT32 i4Speed,UINT32* pu4CurIdx)
{
    SWDMX_MP4_INFO_T* prMp4Info = &prSwdmxInst->rFmtInfo.rSwdmxMp4Info;

    if(prMp4Info->u4NumOfKeyTable >= 2)
    {
        if((*pu4CurIdx) == prMp4Info->parVidKeyIdxTblArray[prMp4Info->u4CurKeyTableIdx]->ui4_number_of_entry -2)
        {
            if(i4Speed > 2000)
            {
                if((prMp4Info->u4CurKeyTableIdx+1) < prMp4Info->u4NumOfKeyTable)
                {
                    prMp4Info->u4CurKeyTableIdx++;
                    prMp4Info->prVidKeyIdxTbl = prMp4Info->parVidKeyIdxTblArray[prMp4Info->u4CurKeyTableIdx];
                    *pu4CurIdx = 0;
                    return TRUE;
                }
            }
            return FALSE;
        }
        else if((*pu4CurIdx) ==1)
        {
            if(i4Speed < 0)
            {
                if(prMp4Info->u4CurKeyTableIdx >= 1)
                {
                    prMp4Info->u4CurKeyTableIdx--;
                    prMp4Info->prVidKeyIdxTbl = prMp4Info->parVidKeyIdxTblArray[prMp4Info->u4CurKeyTableIdx];
                    *pu4CurIdx = ((prMp4Info->prVidKeyIdxTbl)->ui4_number_of_entry -2);
                    return TRUE;
                }
            }
            return FALSE;
        }
        return FALSE;
    }
    else
    {
        return FALSE;
    }
}

void _SWDMX_Mp4SetKeyTableIdx(SWDMX_INFO_T* prSwdmxInst,MM_RANGE_ELMT_IDXTBL_T* prTargetKeyTable)
{
    UINT32 i=0;
    SWDMX_MP4_INFO_T* prMp4Info = &prSwdmxInst->rFmtInfo.rSwdmxMp4Info;
    
    for(i=0 ; i< prMp4Info->u4NumOfKeyTable ; i++)
    {
        if(prMp4Info->parVidKeyIdxTblArray[i] == prTargetKeyTable)
        {
            prMp4Info->u4CurKeyTableIdx = i;
            break;
        }
    }
}


