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
 * $RCSfile: swdmx_flv.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_flv.c
 *  Software demux for flv - internal interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "dmx_if.h"
#include "dmx_mm_if.h"
#include "swdmx_drvif.h"
#include "vdec_drvif.h"
#include "u_midxbuld.h"
#include "swdmx_debug.h"
#include "aud_drvif.h"
#include "x_util.h"
#include "x_drm_drv.h"
#include "swdmx_flv.h"

#if defined(CC_MT5363)
#include "b2r_drvif.h"
#include "b2r_if.h"
#endif

#ifdef CC_CMPB_PUSH_MODE
static SWDMX_HDR_PRS_STATE_T _SWDMX_FlvHdrParsePushMode(
    UINT8 u1SrcId,
    SWDMX_RANGE_LIST_T* prRangeList,
    SWDMX_CUR_CMD_T *prCmd);
#endif
//#define FLV_USE_DRV_BLD
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define flv_min(x, y) ((((UINT32)x) < ((UINT32)y)) ? ((UINT32)x) : ((UINT32)y))
#define flv_max(x, y) ((((UINT32)x) > ((UINT32)y)) ? ((UINT32)x) : ((UINT32)y))
#define flv_abs(x)    (((x) >= 0) ? (x) : -(x))
//#define SWDMX_SEND_BLOCK_DATA

#define SWDMX_MAYBY
#define SWDMX_FLV_CODEC_JPG 1
#define SWDMX_FLV_CODEC_SORENSON 2
#define SWDMX_FLV_CODEC_SCREEN 3
#define SWDMX_FLV_CODEC_VP6 4
#define SWDMX_FLV_CODEC_VP6A 5
#define SWDMX_FLV_CODEC_SCREEON2 6
#define SWDMX_FLV_CODEC_AVC 7
#define AAC_ALLOC_SIZE   (256)
#define SWDMX_FLV_FILE_HEADER_SIZE (9)
#define SWDMX_FLV_TAG_HEADER_SIZE (11)
#define SWDMX_FLV_TAG_ENDSIZE (4)
#define SWDMX_FLV_PTS_FACTOR (90)
#ifdef LINUX_TURNKEY_SOLUTION
#define SWDMX_FLV_MOVI_MAX (2048*16)
#else
#define SWDMX_FLV_MOVI_MAX (1024*32)
#endif
#define SWDMX_FLV_VIDEO_HEADER_SIZE (8*1024)
#define SWDMX_FLV_AAC_CONFIG_LEN (512)
#define SWDMX_FLV_AAC_FAKE_HDR_SIZE (7)
#define SWDMX_FLV_SEEK_REQ_SIZE (512)
#define SWDMX_FLV_AAC_DEF_FREQ_NUM 16
#define SWDMX_FLV_AAC_CHANNEL_CFG_NUM 7
#define SWDMX_FLV_ERRCOV_IDENTIFY_OFFSET 8
#define SWDMX_FLV_STREAM_ID (0)
#define SWDMX_DMA_ALIGN (4)
#define SWDMX_DMX_FREE_BUF (1024)
#define SWDMX_FLV_INDEX_TRICK_NUM (8)
#define SWDMX_DMX_INVALID_PTS(pts) ((pts%90))
#define SWDMX_FLV_INVALID_PTS 0xffffffff
#define SWDMX_FLV_PTS_INFOR_START 1
#define SWDMX_FLV_PTS_INFOR_PARTION (SWDMX_FLV_PTS_INFOR_START+1)
#define SWDMX_FLV_PTS_INFOR_DROP (SWDMX_FLV_PTS_INFOR_PARTION+1)
#define SWDMX_FLV_PTS_INFOR_SEQHEADER (SWDMX_FLV_PTS_INFOR_DROP+1)
#define SWDMX_FLV_PTS_INFOR_STARTCODE (SWDMX_FLV_PTS_INFOR_SEQHEADER+1)
#define SWDMX_FLV_VIDEO_FIFO_SIZE (1024*1024*2+512*1024)
#define SWDMX_FLV_STARTCODE_LEN (4)
#define SWDMX_FLV_INFORDATA_BEFORE_NAL (5)
#define SWDMX_FLV_INVALID_NALTYPE (0xff)
#define FLV_RPOS_INC(src,len,startaddr,endaddr) (src= (src+(len)>=endaddr) ? (startaddr+((src)+(len)-(endaddr))) : ((src)+(len)))

#define FLV_FLG_SET(flg,index) (flg|=(1<<(index)))
#define FLV_FLG_CLR(flg,index) (flg&=(~(1<<(index))))
#define FLV_FLG_TRUE(flg,index) (flg&(1<<(index)))

#define FLV_TRANS_DTA(src,dst,len,startaddr,endaddr) \
{ \
  UINT32 u4TransLen=0; \
  while(u4TransLen<len) \
  {   \
    *(((UCHAR *)dst)+u4TransLen)=*((UCHAR *)src);  \
    FLV_RPOS_INC(src,1,startaddr,endaddr); \
    u4TransLen++;\
  }  \
}

#define FLV_SET_INTEGER(integer,src,len,stataddr,endaddr) \
{ \
    UCHAR uByte=0,uIntLen=0; \
    uIntLen=len;  \
    while(uIntLen>0) \
    {  \
       uByte=(integer>>((uIntLen-1)*8))&0xff; \
       *((UCHAR *)src)=uByte; \
       uIntLen--;\
       FLV_RPOS_INC(src,1,stataddr,endaddr); \
    } \
}

#define FLV_INTEGER(integer,src,len,startaddr,endaddr) \
{  \
   UCHAR uByte,uIntLen=0; \
   integer=0; \
   while(uIntLen<len) \
   {  \
      uByte=*((UCHAR *)src); \
      integer=(integer<<8)|uByte; \
      uIntLen++;\
      FLV_RPOS_INC(src,1,startaddr,endaddr); \
   } \
}

#define FLV_INTEGER_NEXT(integer,src,len,startaddr,endaddr) \
{  \
  UCHAR uByte,uIntLen=0,*tmpsrc; \
  integer=0; \
  tmpsrc=(UCHAR *)src; \
  while(uIntLen<len) \
  {  \
     uByte=*((UCHAR *)tmpsrc); \
     integer=(integer<<8)|uByte; \
     uIntLen++;\
     FLV_RPOS_INC(tmpsrc,1,startaddr,endaddr); \
  } \
}

#define FLV_INTEGER_BIT(integer,src,sbit,bitlen,saddr,eaddr) \
{  \
   UCHAR uByte,uIntLen=0,uSkip=0,uBits; \
   integer=0; \
   uSkip=sbit/8; \
   uBits=sbit%8; \
   uBits=8-uBits; \
   FLV_RPOS_INC(src,uSkip,saddr,eaddr); \
   uByte=*((UCHAR *)src); \
   if(uBits>bitlen) \
   {  \
     integer= (uByte>>(uBits-bitlen))&((1<<bitlen)-1);    \
   }  \
   else \
   { \
     integer=uByte&((1<<uBits)-1); \
     FLV_RPOS_INC(src,1,saddr,eaddr); \
     uIntLen=(bitlen-uBits)/8; \
     uBits=(bitlen-uBits)%8; \
     while(uIntLen-->0) \
     {  \
        uByte=*((UCHAR *)src); \
        integer=(integer<<8)|uByte; \
        uIntLen--; \
        FLV_RPOS_INC(src,1,saddr,eaddr); \
     } \
     if(uBits) \
     { \
        uByte=*((UCHAR *)src); \
        integer=(integer<<uBits)|((uByte>>(8-uBits))&((1<<uBits)-1)); \
     } \
   } \
   sbit=(sbit+bitlen)%8; \
}

#define SWDMX_FLV_VERIFY_NULL(ptr)                                    \
          do {                                              \
            if (ptr == NULL) { \
                LOG(3, "SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
                return; }      \
          } while (0)

#define SWDMX_FLV_VERIFY_NULL_ERROR(ptr)                                    \
          do {                                              \
            if (ptr == NULL) { \
                LOG(3, "SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
                return FALSE; }      \
          } while (0)
          
//#define INPUT_SRC prSwdmxFlvInfo->prRangeList.prDmxRangeInfo->e_mm_src_type
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static UINT32 _rAacFreqTbl[SWDMX_FLV_AAC_DEF_FREQ_NUM]=
{
  96000,88200,64000,48000,44100,32000,24000,
  22050,16000,12000,11025,8000,0,0,0,0
};
static UINT8 _rAacChCfgTbl[SWDMX_FLV_AAC_CHANNEL_CFG_NUM]=
{
  1,2,3,4,5,6,8
};

static UINT32 u4DebutCounter=0;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static SWDMX_HDR_PRS_STATE_T _SWDMX_ErrRecovery(
    SWDMX_FLV_INFO_T* prSwdmxFlvInfo,
    UCHAR* ErrorData,
    UINT32 u4DataLen,
    UINT32 *u4RecoveryOffset);

static SWDMX_HDR_PRS_STATE_T _SWDMX_FlvTagPrs(
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo,
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange);

static SWDMX_HDR_PRS_STATE_T _SWDMX_FlvDeliverTagData(
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo,
    UCHAR* pucBitstream,
    SWDMX_RANGE_LIST_T* prRangeList,
    UINT32 u4AvailSize,
    UINT32* pu4ConsumeSize,
    INT32 i4PlaySpeed,
    BOOL fgRangeEnd);

static BOOL _SWDMX_FlvGetDataFromFeeder(
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo,
    FEEDER_REQ_DATA_T *prFeederData,
    UINT32 u4WaitTime
);


static BOOL _SWDMX_FlvInitDmx(
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo,
    UINT8 u1HdrType,
    VOID* pfnCbFunc,
    BOOL fgEos);

static BOOL _SWDMX_FlvSendEos(
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo,
    UINT8 u1HdrType);
static INT32 _SWDMX_FlvSeekCurPos(SWDMX_FLV_INFO_T *prSwdmxFlvInfo,UINT8 u4B2RId);
static INT32 _SWDMX_FlvSeek(
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo,
    UINT64 u8SeekTime,INT32 i4PlaySpeed,FLV_SEEK_TYPE eSeekType);

static INT32 _SWDMX_FlvSeekWithoutIndex(
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo,
    UINT64 u8SeekTime, UINT8 u4SeekMode,
    MM_RANGE_INFO_T *prDmxRangeInfo);

static BOOL _SWDMX_FlvResetFeeder(SWDMX_FLV_INFO_T *prSwdmxFlvInfo);
static void _SWDMX_FlvVideoWakeUp(SWDMX_FLV_INFO_T *prSwdmxFlvInfo);

//static BOOL _SWDMX_FlvSeekPos(SWDMX_FLV_INFO_T *prSwdmxFlvInfo, UINT64 u8SeekPos,UINT64 *u8Pos,UINT32 *u4Pts);

static BOOL _SWDMX_FlvInitVar(UCHAR u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo;
    
    SWDMX_FLV_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxFlvInfo= &prSwdmxInst->rFmtInfo.rSwdmxFlvInfo;
   
    x_memset(prSwdmxFlvInfo, 0, sizeof(SWDMX_FLV_INFO_T));

    //*********************************************************
    //FeederSetBufferSize(FEEDER_PROGRAM_SOURCE, 5*10*1024);
    //FeederStart(FEEDER_PROGRAM_SOURCE);
    /*
          FeederInstallCallback(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_DATA_REQ_DONE,
          _SWDMX_FeederCallBack, (UINT32)&prSwdmxFlvInfo->rFeederInfo);
      */
    prSwdmxFlvInfo->uAvcNalLenBytes=4;
    prSwdmxFlvInfo->prFeederInfo=&prSwdmxInst->rFeederInfo;
    prSwdmxFlvInfo->u1SrcId=u1SrcId;
    prSwdmxFlvInfo->u4SwdmxInst=(UINT32)prSwdmxInst;
    u4DebutCounter=0xffffffff;
    prSwdmxFlvInfo->i4PrevSpeed = 1000;
    prSwdmxFlvInfo->fgNewRp = FALSE;
    prSwdmxFlvInfo->fgBeginToPlay = TRUE;
    prSwdmxFlvInfo->u4CurRepeatFrameIdx = 0xFFFFFFFF;
    prSwdmxFlvInfo->fgPreviouIsSeek = FALSE;
	prSwdmxFlvInfo->uLastNalType=SWDMX_FLV_INVALID_NALTYPE;
    return TRUE;
}

INT32 _SWDMX_FlvSetDebugCounter(INT32 i4Counter)
{
   u4DebutCounter=i4Counter;
   return 0;
}

static INT32 _SWDMX_FlvDmxRest(SWDMX_FLV_INFO_T *prSwdmxFlvInfo,UINT32 u4Type)
{
   UINT32 u4Flag=0;
   DMX_MM_T rDmxMMInfo;
   SWDMX_INFO_T* prSwdmxInst = (SWDMX_INFO_T* )prSwdmxFlvInfo->u4SwdmxInst;
   rDmxMMInfo.fgEnable = TRUE;
   rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;
   if(u4Type==eSWDMX_MEDIA_VIDEO)
       {
       switch (prSwdmxFlvInfo->u4VDecType)
       {
         case ENUM_SWDMX_VDEC_MPEG4:
         case ENUM_SWDMX_VDEC_H264:
         case ENUM_SWDMX_VDEC_VP6:
         case ENUM_SWDMX_VDEC_MJPEG:
                  LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = FALSE\n");
                  rDmxMMInfo.fgSearchStartCode = FALSE;
                  break;
         default:

                  u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
                  LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = TRUE\n");
                  rDmxMMInfo.fgSearchStartCode = TRUE;
                  break;
       }

       if (!DMX_MM_Set(prSwdmxFlvInfo->u1VidPid, u4Flag, &rDmxMMInfo))
       {
                    LOG(3, "Fail to reset demuxer\n");
       }
       }
    else if(u4Type==eSWDMX_MEDIA_AUDIO)
       {
       AUD_ClearEOSFlag(prSwdmxInst->u1AdecId);
       }

   return 0;
}




static BOOL _SWDMX_FlvCalculateRepeatFrame(INT32 i4PlaySpeed,SWDMX_FLV_INFO_T *prSwdmxFlvInfo)
{
    UINT32 u4CurKey = prSwdmxFlvInfo->u4CurSentKey;
    MM_RANGE_ELMT_IDXTBL_T *prKeyIdxTbl = prSwdmxFlvInfo->prVidKeyIdxTbl;


    if (prSwdmxFlvInfo->u4VidRepeatFrame == 0)
    {

        if(prSwdmxFlvInfo->fgKeyExist && prKeyIdxTbl->ui4_number_of_entry>0)
        {
            if (prSwdmxFlvInfo->u4CurRepeatFrameIdx != u4CurKey &&(prSwdmxFlvInfo->u4CurRepeatFrameIdx< prKeyIdxTbl->ui4_number_of_entry))
            {
                UINT32 u4PtsDif = 0, u4MaxRepeatFrame;
                u4MaxRepeatFrame = (prSwdmxFlvInfo->u4VidFps * 1000 * 2)/flv_abs(i4PlaySpeed);

                if (i4PlaySpeed > 2000)
                {
                    if(u4CurKey < prKeyIdxTbl->ui4_number_of_entry && prKeyIdxTbl->ui4_number_of_entry>10 )
                    {
                        u4PtsDif = prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts -prKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxFlvInfo->u4CurRepeatFrameIdx].ui4_pts;
                        prSwdmxFlvInfo->u4VidRepeatFrame = (UINT32)((u4PtsDif * prSwdmxFlvInfo->u4VidFps)/(90 * i4PlaySpeed));
                    }
                    else
                    {
                        prSwdmxFlvInfo->u4VidRepeatFrame = (prSwdmxFlvInfo->u4VidFps * 1000)/flv_abs(i4PlaySpeed);
                    }
                }
                else if (i4PlaySpeed < 0)
                {
                    if ((prKeyIdxTbl->ui4_number_of_entry > 10))
                    {
                        u4PtsDif = prKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxFlvInfo->u4CurRepeatFrameIdx].ui4_pts -prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts;
                        prSwdmxFlvInfo->u4VidRepeatFrame = (UINT32)((u4PtsDif * prSwdmxFlvInfo->u4VidFps)/(90 * flv_abs(i4PlaySpeed)));
                    }
                    else
                    {
                        prSwdmxFlvInfo->u4VidRepeatFrame = (prSwdmxFlvInfo->u4VidFps * 1000)/flv_abs(i4PlaySpeed);
                    }
                }

                prSwdmxFlvInfo->u4VidRepeatFrame = flv_min(prSwdmxFlvInfo->u4VidRepeatFrame,u4MaxRepeatFrame);

                if((u4CurKey == prKeyIdxTbl->ui4_number_of_entry - 1) ||(u4CurKey == 0))
                {
                    prSwdmxFlvInfo->u4VidRepeatFrame=32;
                }

                LOG(5, "u4PtsDif = %ld Current Idx = %ld Repeat frame = %ld\n", u4PtsDif, u4CurKey,prSwdmxFlvInfo->u4VidRepeatFrame);

                if(prSwdmxFlvInfo->u4VidRepeatFrame == 0)
                {
                    return FALSE;
                }

                prSwdmxFlvInfo->u4CurRepeatFrameIdx = u4CurKey;
                LOG(5, "Repeat Idx = %ld\n", prSwdmxFlvInfo->u4CurRepeatFrameIdx);
                return TRUE;
            }
            else
            {
               prSwdmxFlvInfo->u4VidRepeatFrame = (prSwdmxFlvInfo->u4VidFps * 1000)/flv_abs(i4PlaySpeed);
               return FALSE;
            }
        }
        else
        {
            prSwdmxFlvInfo->u4VidRepeatFrame = (prSwdmxFlvInfo->u4VidFps * 1000)/flv_abs(i4PlaySpeed);
        }

        return FALSE;
    }

    prSwdmxFlvInfo->u4VidRepeatFrame--;
    return TRUE;
}

static INT32 _SWDMX_FlvChangeSpeed(INT32 i4PlaySpeed,SWDMX_PLAY_MODE_T ePlayMode,
	UINT32 u4CurrentTime,UINT64 u8SeekPos,
	FLV_SEEK_CMD eSeekType,SWDMX_FLV_INFO_T *prSwdmxFlvInfo)
{
   SWDMX_RANGE_LIST_T* prRangeList=prSwdmxFlvInfo->prRangeList;
   MM_RANGE_INFO_T *prCurRangeInfo=prSwdmxFlvInfo->prRangeList->prDmxRangeInfo;
   INT32 i4PreSpeed,i4SeekRet=FLV_SEEK_RET_FIND;
   VDEC_PTS_INFO_T rPTSInfo;
   SWDMX_INFO_T* prSwdmxInst =(SWDMX_INFO_T* )prSwdmxFlvInfo->u4SwdmxInst;
   UINT8 u1EsId=prSwdmxFlvInfo->u1EsId;
   SWDMX_PLAY_MODE_T ePrePlayMode=(SWDMX_PLAY_MODE_T)prSwdmxInst->ePrePlayMode;
   x_memset(&rPTSInfo,0,sizeof(VDEC_PTS_INFO_T));
   LOG(5, "Speed change while VDP Pts = 0x%08x\n", u4CurrentTime);
   i4PreSpeed=prSwdmxFlvInfo->i4PrevSpeed;

   if (prSwdmxFlvInfo->fgDeliverAEos)
   {
      _SWDMX_FlvDmxRest(prSwdmxFlvInfo,eSWDMX_MEDIA_AUDIO);
      prSwdmxFlvInfo->fgDeliverAEos = FALSE;
   }

   prSwdmxFlvInfo->fgAudIsReady = FALSE;
   prSwdmxFlvInfo->fgAudIsLoaded = FALSE;
   prSwdmxFlvInfo->fgBadAudio = FALSE;
   prSwdmxFlvInfo->fgErrorConvery=FALSE;
   prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;

   // 1X -> 2X
   prSwdmxFlvInfo->u4VidRepeatFrame = 0;
   if(prSwdmxFlvInfo->fgVDecIsPause)
   {
      _SWDMX_FlvVideoWakeUp(prSwdmxFlvInfo);
   }

   rPTSInfo.u8Scale = DEMUX_FLV_AV_PERIOD;
   rPTSInfo.u4Rate = prSwdmxFlvInfo->u4VidFps;
        // seek
   
   LOG(3,"FLV speed change: prespeed=0x%x,curspeed=0x%x\n",i4PreSpeed,i4PlaySpeed);
   if(eSeekType!=FLV_SEEK_TYPE_INVALID)
   {
      if(eSeekType==FLV_SEEK_TYPE_PTS)
      {
          LOG(3,"FLV seek pts:0x%x\n",u8SeekPos);
          i4SeekRet=_SWDMX_FlvSeek(prSwdmxFlvInfo,u8SeekPos,i4PlaySpeed,eFLV_SEEK_TYPE_TIME);
      }
      else
      {
          LOG(3,"FLV seek pos:0x%x\n",u8SeekPos);
          i4SeekRet=_SWDMX_FlvSeek(prSwdmxFlvInfo,u8SeekPos,i4PlaySpeed,eFLV_SEEK_TYPE_OFFSET);
      }
      
      if(i4SeekRet!=FLV_SEEK_RET_FILEEND)
      {        
         LOG(5, "u8SeekPos = %lld\n", u8SeekPos);
         if(prSwdmxFlvInfo->fgEnAudio)
         {
            DMX_MM_FlushBuffer(prSwdmxFlvInfo->u1AudPid);
         }
         
         if(prSwdmxFlvInfo->fgEnVideo)
         {
            MPV_FlushEsmQ(u1EsId, TRUE);
            VDEC_ReleaseDispQ(u1EsId);
            DMX_MM_FlushBuffer(prSwdmxFlvInfo->u1VidPid);
            if(prSwdmxFlvInfo->i4PrevSpeed>0)
            { 
              if(eSeekType==FLV_SEEK_TYPE_PTS)
              {
                prSwdmxFlvInfo->u4RecSeekTime=(UINT32)u8SeekPos;
                prSwdmxFlvInfo->u4RecSeekAtime=prSwdmxFlvInfo->u4RecSeekTime;
                prSwdmxFlvInfo->u4AudStartPts=prSwdmxFlvInfo->u4RecSeekTime;
              }
              VDEC_SetRenderFromPts(u1EsId, prSwdmxFlvInfo->u4RecSeekTime);
            }
            else
            {
              VDEC_SetRenderFromPts(u1EsId, 0);
            }
           
           if(prSwdmxFlvInfo->fgDeliverVEos)
           {
              prSwdmxFlvInfo->fgDeliverVEos = FALSE;
           }
		   prSwdmxFlvInfo->uLastNalType=SWDMX_FLV_INVALID_NALTYPE;
           VDEC_StartPTS(u1EsId, TRUE, prSwdmxFlvInfo->u4RecSeekTime, NULL);
         }
 
         //AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxFlvInfo->u4RecSeekAtime);
         STC_StopStc(prSwdmxInst->u1StcId);
         
         if(eSeekType==FLV_SEEK_TYPE_PTS)
         {
             STC_SetStcValue(prSwdmxInst->u1StcId,prSwdmxFlvInfo->u4RecSeekTime);
             if(prSwdmxFlvInfo->fgEnAudio)
             {
                 prSwdmxFlvInfo->fgNeedSetAudStartPts=TRUE;
             }
             else
             {
                 prSwdmxFlvInfo->fgNeedSetAudStartPts=FALSE;
                 prSwdmxFlvInfo->u4TriggerAudPts=prSwdmxFlvInfo->u4RecSeekTime;
                 //STC_StartStc(prSwdmxInst->u1StcId);
             }
             prSwdmxFlvInfo->fgPreviouIsSeek = TRUE;
         }
         else
         {
             prSwdmxFlvInfo->fgNeedSetAudStartPts=FALSE;
             prSwdmxFlvInfo->u4TriggerAudPts=prSwdmxFlvInfo->u4RecSeekTime;
             STC_SetStcValue(prSwdmxInst->u1StcId,prSwdmxFlvInfo->u4RecSeekTime-25000);
             AUD_SetStartPts(prSwdmxInst->u1AdecId,prSwdmxFlvInfo->u4RecSeekAtime);
             if(prSwdmxFlvInfo->u4VDecType==ENUM_SWDMX_VDEC_MPEG4)
             {
                 STC_StartStc(prSwdmxInst->u1StcId);
             }
         }
      }

      return 0;
   }// 2000->1000
   else if(ePlayMode==SWDMX_PLAY_MODE_NORMAL&&ePrePlayMode==SWDMX_PLAY_MODE_NORMAL)
   {
       LOG(5, "Flv Speed 2X -> 1X Cur Vid Pts = 0x%08x, Cur Aud Pts = 0x%08x\n",prSwdmxFlvInfo->u4CurVidPts, prSwdmxFlvInfo->u4AudPts);
       if(i4PlaySpeed == 1000 || (i4PlaySpeed<=2000&&i4PreSpeed>2000&&prSwdmxFlvInfo->fgKeyExist==FALSE))
       {
           if(prSwdmxFlvInfo->fgEnADmx)
           {
			 DMX_MM_FlushBuffer(prSwdmxFlvInfo->u1AudPid);
			 prSwdmxFlvInfo->fgNeedSetAudStartPts=TRUE;
           }
		   MPV_FlushEsmQ(u1EsId, TRUE);
		   VDEC_ReleaseDispQ(u1EsId);
		   DMX_MM_FlushBuffer(prSwdmxFlvInfo->u1VidPid);
		   if(prSwdmxFlvInfo->fgKeyExist)
		   {
			  _SWDMX_FlvSeek(prSwdmxFlvInfo,u4CurrentTime,i4PlaySpeed,eFLV_SEEK_TYPE_TRICK);
			  prSwdmxFlvInfo->u4RecSeekTime=u4CurrentTime;
			  prSwdmxFlvInfo->u4RecSeekAtime=prSwdmxFlvInfo->u4RecSeekTime;
			  prSwdmxFlvInfo->u4AudStartPts=prSwdmxFlvInfo->u4RecSeekTime;
		   }
		   else
		   {
			   if(prSwdmxFlvInfo->fgEnVideo)
			   {
				   _SWDMX_FlvSeekCurPos(prSwdmxFlvInfo,prSwdmxInst->u1B2rId);
			   }
		   }
		   
		   if(prSwdmxFlvInfo->fgEnVideo)
		   {
			 VDEC_SetRenderFromPts(u1EsId, prSwdmxFlvInfo->u4RecSeekTime);
			 VDEC_StartPTS(u1EsId, TRUE, prSwdmxFlvInfo->u4RecSeekTime, NULL);
		   }

            if(prSwdmxFlvInfo->u4VDecType==ENUM_SWDMX_VDEC_H264)
            {   
              LOG(3,"swdmx stc change:(PTS:0x%x,STC:0x%x)\n",prSwdmxFlvInfo->u4RecSeekTime-25000,u4CurrentTime);
              STC_SetStcValue(prSwdmxInst->u1StcId,prSwdmxFlvInfo->u4RecSeekTime-25000);
              STC_StartStc(prSwdmxInst->u1StcId);
			  prSwdmxFlvInfo->uLastNalType=SWDMX_FLV_INVALID_NALTYPE;
            }
              
            if(prSwdmxFlvInfo->fgDeliverVEos)
            {
              prSwdmxFlvInfo->fgDeliverVEos = FALSE;
            }
         }
      }
   else if(i4PreSpeed < 0 && ePlayMode==SWDMX_PLAY_MODE_NORMAL)
   {
	  if(prSwdmxFlvInfo->fgKeyExist)
	  {
		  _SWDMX_FlvSeek(prSwdmxFlvInfo,u4CurrentTime,i4PlaySpeed,eFLV_SEEK_TYPE_TRICK);
	  }
	  else
	  {
	      if(prSwdmxFlvInfo->fgEnVideo)
	      {
			  _SWDMX_FlvSeekCurPos(prSwdmxFlvInfo,prSwdmxInst->u1B2rId);
	      }
	  }
	  
      MPV_FlushEsmQ(u1EsId, TRUE);
      VDEC_ReleaseDispQ(u1EsId);
      DMX_MM_FlushBuffer(prSwdmxFlvInfo->u1VidPid);
      DMX_MM_FlushBuffer(prSwdmxFlvInfo->u1AudPid);
      if(prSwdmxFlvInfo->fgDeliverVEos)
      {
         prSwdmxFlvInfo->fgDeliverVEos = FALSE;
      }
      
      if(prSwdmxFlvInfo->fgEnVideo)
      {
         VDEC_SetRenderFromPts(u1EsId,prSwdmxFlvInfo->u4RecSeekTime);
         VDEC_StartPTS(u1EsId, TRUE,prSwdmxFlvInfo->u4RecSeekTime ,&rPTSInfo);
      }
      STC_SetStcValue(prSwdmxInst->u1StcId,prSwdmxFlvInfo->u4RecSeekTime-15000);
	  prSwdmxFlvInfo->uLastNalType=SWDMX_FLV_INVALID_NALTYPE;
      if(i4PlaySpeed == 1000 && prSwdmxFlvInfo->fgEnAudio)
      {
         prSwdmxFlvInfo->fgNeedSetAudStartPts=TRUE;
		 prSwdmxFlvInfo->u4RecSeekAtime=prSwdmxFlvInfo->u4RecSeekTime;
		 prSwdmxFlvInfo->u4AudStartPts=prSwdmxFlvInfo->u4RecSeekTime;
		 prSwdmxFlvInfo->fgNeedSetAudStartPts=TRUE;
         //prSwdmxFlvInfo->u4TriggerAudPts = prSwdmxFlvInfo->u4RecSeekAtime+45000;
         LOG(5, "VDP trigger audio = 0x%08x\n", prSwdmxFlvInfo->u4TriggerAudPts);
         //AUD_SetStartPts(prSwdmxInst->u1AdecId,prSwdmxFlvInfo->u4RecSeekAtime);
         // set stc
         STC_StopStc(prSwdmxInst->u1StcId);
         STC_SetStcValue(prSwdmxInst->u1StcId,prSwdmxFlvInfo->u4RecSeekTime-15000);
         STC_StartStc(prSwdmxInst->u1StcId);
      }
      else if(prSwdmxFlvInfo->u4VDecType==ENUM_SWDMX_VDEC_MPEG4)
      {
         STC_StartStc(prSwdmxInst->u1StcId);
      }
      LOG(5, "Flv Speed -32X/-16X/-8X/-4X/-2X -> 1X\n");

   }
   if(i4PreSpeed==1000 || ePrePlayMode==SWDMX_PLAY_MODE_IFRAME || ePlayMode==SWDMX_PLAY_MODE_IFRAME)
   {
      //if (!( i4PlaySpeed < 1000 && i4PlaySpeed > 1))
      {
	     if(i4PreSpeed==1000&&ePlayMode==SWDMX_PLAY_MODE_NORMAL)
         {
           DMX_MM_FlushBuffer(prSwdmxFlvInfo->u1AudPid);
           if(i4PlaySpeed==1000 &&i4PreSpeed==1000)
           {
              STC_StopStc(prSwdmxInst->u1StcId);
              STC_SetStcValue(prSwdmxInst->u1StcId,u4CurrentTime-35000);
              STC_StartStc(prSwdmxInst->u1StcId);
           }
         }
		 else if(ePrePlayMode==SWDMX_PLAY_MODE_IFRAME || ePlayMode==SWDMX_PLAY_MODE_IFRAME)
         {
             if(i4PlaySpeed!=1)
             {
				 if(prSwdmxFlvInfo->fgKeyExist)
				 {
					 _SWDMX_FlvSeek(prSwdmxFlvInfo,u4CurrentTime,i4PlaySpeed,eFLV_SEEK_TYPE_TRICK);
				 }
				 else
				 {
					 if(prSwdmxFlvInfo->fgEnVideo)
					 {
						 _SWDMX_FlvSeekCurPos(prSwdmxFlvInfo,prSwdmxInst->u1B2rId);
					 }
				 }
             }
			 
			 prSwdmxFlvInfo->fgRecoveredFromErr=FALSE;
			 prSwdmxFlvInfo->fgErrorConvery=FALSE;
             MPV_FlushEsmQ(u1EsId, TRUE);
             VDEC_ReleaseDispQ(u1EsId);
             DMX_MM_FlushBuffer(prSwdmxFlvInfo->u1VidPid);
             DMX_MM_FlushBuffer(prSwdmxFlvInfo->u1AudPid);
			 prSwdmxFlvInfo->uLastNalType=SWDMX_FLV_INVALID_NALTYPE;
             if(prSwdmxFlvInfo->fgDeliverVEos)
             {
                prSwdmxFlvInfo->fgDeliverVEos = FALSE;
             }
             
             if (i4PlaySpeed>0&&ePlayMode==SWDMX_PLAY_MODE_IFRAME)
             {
                VDEC_SetRenderFromPts(u1EsId, prSwdmxFlvInfo->u4RecSeekTime);
                VDEC_StartPTS(u1EsId, FALSE,  prSwdmxFlvInfo->u4RecSeekTime, &rPTSInfo);
             }
             else if (ePlayMode==SWDMX_PLAY_MODE_NORMAL&&i4PlaySpeed!=1)
             {
                VDEC_SetRenderFromPts(u1EsId,u4CurrentTime);
                STC_SetStcValue(prSwdmxInst->u1StcId,prSwdmxFlvInfo->u4RecSeekTime-35000);
                VDEC_StartPTS(u1EsId, TRUE, prSwdmxFlvInfo->u4RecSeekTime,&rPTSInfo);
                STC_StartStc(prSwdmxInst->u1StcId);
             }
             else if(i4PlaySpeed!=1) // FF should set start pts as 0
             {
                VDEC_SetRenderFromPts(u1EsId,prCurRangeInfo->t_vid_start_pts);
                VDEC_StartPTS(u1EsId, FALSE, prCurRangeInfo->t_vid_start_pts, &rPTSInfo);
             }
             
         }
         else
         {
             STC_StartStc(prSwdmxInst->u1StcId);
         }

         if (ePlayMode==SWDMX_PLAY_MODE_IFRAME)
         {
            prSwdmxFlvInfo->fgTrickMode = TRUE;
         }

        // Things for 32/16/8/4/-2/-4/-8/-16/-32 -> 1X are the same
         if((i4PreSpeed != 1000) &&(i4PlaySpeed == 1000) &&prSwdmxFlvInfo->fgEnAudio)
         {
            //prSwdmxFlvInfo->u4TriggerAudPts = prSwdmxFlvInfo->u4RecSeekAtime;

            //AUD_SetStartPts(prSwdmxInst->u1AdecId,  prSwdmxFlvInfo->u4RecSeekAtime);
            prSwdmxFlvInfo->fgNeedSetAudStartPts=TRUE;
            LOG(2,"u4RecSeekAtime = %x\n", prSwdmxFlvInfo->u4RecSeekAtime);
                  // set stc
            STC_StopStc(prSwdmxInst->u1StcId);
            STC_SetStcValue(prSwdmxInst->u1StcId,prSwdmxFlvInfo->u4RecSeekTime);
         }
         LOG(5, "_rSwdmxFlvInfo.i4PrevSpeed != 1000 i4PlaySpeed != 2000\n");
              // don't need to handle 1X->2X
     }
 }

 if(prSwdmxInst->ePlayMode!=SWDMX_PLAY_MODE_NORMAL)
 {
    prSwdmxFlvInfo->fgTrickMode=TRUE;
    prSwdmxFlvInfo->fgStartTrick=TRUE;
 }
 else
 {
     prSwdmxFlvInfo->fgTrickMode=FALSE;
     prSwdmxFlvInfo->fgStartTrick=FALSE;
 }

 if(prSwdmxFlvInfo->fgKeyExist)
 {
     if (i4PlaySpeed > 2000)
     {
         prSwdmxFlvInfo->u4CurRepeatFrameIdx = prSwdmxFlvInfo->u4CurSentKey + 1;
         prSwdmxFlvInfo->u4CurSentKey++;
     }
     else
     {
         prSwdmxFlvInfo->u4CurRepeatFrameIdx = prSwdmxFlvInfo->u4CurSentKey;
     }
 }


 prSwdmxFlvInfo->i4PrevSpeed = i4PlaySpeed;
 return 0;
}


static void _SWDMX_FlvVideoWakeUp(SWDMX_FLV_INFO_T *prSwdmxFlvInfo)
{
    SWDMX_INFO_T* prSwdmxInst = (SWDMX_INFO_T* )prSwdmxFlvInfo->u4SwdmxInst;
    LOG(5, "_SWDMX_FlvVideoWakeUp\n");
    VDP_SetPauseMm(prSwdmxInst->u1B2rId, FALSE);
    //VDEC_PlayMM(prSwdmxFlvInfo->u1EsId, TRUE);
    switch(prSwdmxFlvInfo->u4VDecType)
    {
        case ENUM_SWDMX_VDEC_MPEG4:
            VDEC_Play(prSwdmxFlvInfo->u1EsId, VDEC_FMT_MP4);
            break;
        case ENUM_SWDMX_VDEC_H264:
            VDEC_Play(prSwdmxFlvInfo->u1EsId, VDEC_FMT_H264);
            break;
        case ENUM_SWDMX_VDEC_MJPEG:
            VDEC_Play(prSwdmxFlvInfo->u1EsId, VDEC_FMT_MJPEG);
            break;
        case ENUM_SWDMX_VDEC_VP6:
            VDEC_Play(prSwdmxFlvInfo->u1EsId, VDEC_FMT_VP6);

            break;
        default:
            LOG(1, "SWDMX_FlvVideoWakeUp Unknown Video Type\n");
            break;
    }

    prSwdmxFlvInfo->fgVDecIsPause=FALSE;
    return;
}

static BOOL _SWDMX_FlvInitDmx(SWDMX_FLV_INFO_T* prSwdmxFlvInfo, UINT8 u1HdrType, VOID* pfnCbFunc, BOOL fgEos)
{
    BOOL fgRet = TRUE;
    UINT32 u4Flag = 0;
    UINT32 u4VidFifoAddr, u4VidFifoSize;
    DMX_MM_T rDmxMMInfo;
    SWDMX_INFO_T* prSwdmxInst =(SWDMX_INFO_T* )prSwdmxFlvInfo->u4SwdmxInst;
    x_memset(&rDmxMMInfo, 0, sizeof(DMX_MM_T));

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;

        if (!prSwdmxFlvInfo->fgEnVDmx)
        {
#ifdef CC_DMX_PURE_AUDIO
            DMX_DECODER_CALLBACKS_T rCallback;
            //rCallback.pfnSendAudioPes = AUD_SendAudioPes;
            rCallback.pfnSendVideoPes = _SWDMX_FlvVideoCallback;
            rCallback.pfnUpdateVideoWp = VDEC_SetWptr;
            DMX_SetDecoderCallbacksSeparate(DMX_CB_FLAG_VIDEO, (const DMX_DECODER_CALLBACKS_T*)&rCallback);
#endif
            DMX_SetToDecoder(TRUE);
            prSwdmxFlvInfo->u1VidPid = DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);
            // Allocate video buffer from FBM
            _SWDMX_GetVideoFifo(prSwdmxInst->u1SwdmxId, &u4VidFifoAddr, &u4VidFifoSize);
            if((u4VidFifoAddr == 0) || (u4VidFifoSize == 0))
            {
                LOG(1, "_SWDMX_FlvSetVideoStreamID ((u4VidFifoAddr == 0) || (u4VidFifoSize == 0))\n");
                return FALSE;
            }

            u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY |
                DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF |
                DMX_MM_FLAG_INSTANCE_TAG |
                DMX_MM_FLAG_DEVICE_ID);
            rDmxMMInfo.fgAllocBuf = FALSE;
            rDmxMMInfo.u4BufAddr = u4VidFifoAddr;
            rDmxMMInfo.u1ChannelId=prSwdmxInst->u1VdecId;
            if(prSwdmxFlvInfo->u4VDecType ==
                (UINT32)ENUM_SWDMX_VDEC_MJPEG)
            {
                rDmxMMInfo.u1DeviceId = VLD1; // video format all use VLD expect jpeg
            }
            else
            {        
                rDmxMMInfo.u1DeviceId = VLD0; // video format all use VLD expect jpeg
            }
            rDmxMMInfo.u4BufSize=u4VidFifoSize;
            rDmxMMInfo.pvInstanceTag = (void*)prSwdmxInst;
            switch (prSwdmxFlvInfo->u4VDecType)
            {
                case ENUM_SWDMX_VDEC_MPEG4:
                case ENUM_SWDMX_VDEC_MJPEG:
                case ENUM_SWDMX_VDEC_H264:
                case ENUM_SWDMX_VDEC_VP6:
                    LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = FALSE\n");
                    rDmxMMInfo.fgSearchStartCode = FALSE;
                    break;
                default:

                    u4Flag |= DMX_MM_FLAG_SEARCH_START_CODE;
                    LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = TRUE\n");
                    rDmxMMInfo.fgSearchStartCode = TRUE;
                    break;
            }

            fgRet = DMX_MM_Set(prSwdmxFlvInfo->u1VidPid, u4Flag, &rDmxMMInfo);
            prSwdmxFlvInfo->u1EsId=prSwdmxInst->u1VdecId;
            //_DMX_GetVideoChannel(prSwdmxFlvInfo->u1SrcId*3,&prSwdmxFlvInfo->u1EsId);
        }
        else if (fgEos)
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;
            fgRet = DMX_MM_Set(prSwdmxFlvInfo->u1VidPid, u4Flag, &rDmxMMInfo);
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        UINT32 u4AudFifoAddr, u4AudFifoSize;

        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_AUDIO;

        if (!prSwdmxFlvInfo->fgEnADmx)
        {

#ifdef CC_DMX_PURE_AUDIO
            DMX_DECODER_CALLBACKS_T rCallback;
            rCallback.pfnSendAudioPes = AUD_SendAudioPes;
            DMX_SetDecoderCallbacksSeparate(DMX_CB_FLAG_AUDIO, (const DMX_DECODER_CALLBACKS_T*)&rCallback);
#endif
            u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY |
                DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF |
                DMX_MM_FLAG_INSTANCE_TAG |
                DMX_MM_FLAG_DEVICE_ID);
            DMX_SetToDecoder(TRUE);
            prSwdmxFlvInfo->u1AudPid = DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);

            _SWDMX_GetAudioFifo(prSwdmxInst->u1SwdmxId, &u4AudFifoAddr, &u4AudFifoSize);

            rDmxMMInfo.fgAllocBuf = FALSE;
            rDmxMMInfo.u4BufAddr = u4AudFifoAddr;
            rDmxMMInfo.u4BufSize = u4AudFifoSize;
            rDmxMMInfo.pvInstanceTag = (void*)prSwdmxInst;
            rDmxMMInfo.u1ChannelId = prSwdmxInst->u1AdecId;
            rDmxMMInfo.u1DeviceId = prSwdmxInst->u1AdecId;

            // Record the audio fifo size
            prSwdmxFlvInfo->u4AFifoSize = rDmxMMInfo.u4BufSize;
            fgRet = DMX_MM_Set(prSwdmxFlvInfo->u1AudPid, u4Flag, &rDmxMMInfo);
        }
        else if (fgEos)
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;
            fgRet = DMX_MM_Set(prSwdmxFlvInfo->u1AudPid, u4Flag, &rDmxMMInfo);
        }
    }
    return fgRet;
}



static BOOL _SWDMX_FlvResetFeeder(SWDMX_FLV_INFO_T* prSwdmxFlvInfo)
{
    FEEDER_REQ_DATA_T rFeederReqData;

    SWDMX_INFO_T* prSwdmxInst = (SWDMX_INFO_T* )prSwdmxFlvInfo->u4SwdmxInst;
    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, prSwdmxFlvInfo->prFeederInfo) != FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    //FeederSetFlush(prSwdmxInst->eFeederSrc);
    _SWDMX_FlushReqMsgQ(prSwdmxFlvInfo->u1SrcId);
    _SWDMX_FlvGetDataFromFeeder(prSwdmxFlvInfo,&rFeederReqData,1);

    FeederSetRing(prSwdmxInst->eFeederSrc,TRUE);
    prSwdmxFlvInfo->fgFeederInvalid=FALSE;
    prSwdmxFlvInfo->u4CurDmxFeederRPtr = prSwdmxFlvInfo->prFeederInfo->u4StartAddr;
    prSwdmxFlvInfo->fgNewRp=TRUE;
    prSwdmxFlvInfo->u4LeftBytesInFeeder=0;
    prSwdmxFlvInfo->u4FeederReqID++;
    prSwdmxFlvInfo->u4FeederReqReceiveID=prSwdmxFlvInfo->u4FeederReqID;
    //prSwdmxFlvInfo->u4FeederReqID=0;
    prSwdmxFlvInfo->i4ReqReadTimes = 0;
    return TRUE;
}


static INT32 _SWDMX_FlvReqTagData(SWDMX_FLV_INFO_T *prSwdmxFlvInfo,UINT64 u8Offset,UINT32 u4ReadSize,BOOL fgVerify,FEEDER_REQ_DATA_T *prFeederData)
{
    FEEDER_TRANSMIT_COND rFeederCond;
    UCHAR *puReadPos,*puStart,*puEnd;
    UINT32 u4ReqSize=0,u4TagLen=0,u4ErrorIdentify=0;
    INT32 i4Ret;
    UINT8 uTagType,uAudIdy,uVidIdy,uMetIdy=0x2,uIdyLen=4;
    BOOL fgVerifyed=FALSE;
    SWDMX_INFO_T* prSwdmxInst = (SWDMX_INFO_T* )prSwdmxFlvInfo->u4SwdmxInst;
    puStart=(UCHAR *)(prSwdmxFlvInfo->prFeederInfo->u4StartAddr);
    puEnd= (UCHAR *)(prSwdmxFlvInfo->prFeederInfo->u4EndAddr);

    if(u4ReadSize==0)
    {
       u4ReqSize=FLV_DEFAULT_TAT_SIZE;
    }
    else
    {
       u4ReqSize=u4ReadSize;
    }

Retry:

    while(1)
    {
       i4Ret=FeederSetRequest(prSwdmxInst->eFeederSrc, u4ReqSize,u8Offset,&rFeederCond,prSwdmxFlvInfo->u4FeederReqID);
       if(i4Ret!=FEEDER_E_OK)
       {
           _SWDMX_FlvResetFeeder(prSwdmxFlvInfo);
           i4Ret=FeederSetRequest(prSwdmxInst->eFeederSrc, u4ReqSize,u8Offset,&rFeederCond,prSwdmxFlvInfo->u4FeederReqID);
           if(i4Ret!=FEEDER_E_OK)
           {
             return FLV_SEEK_RET_FILEERR;
           }
           prSwdmxFlvInfo->u4FeederReqID++;
           break;
       }
       else
       {
           prSwdmxFlvInfo->u4FeederReqID++;
           break;
       }
    }

    x_thread_delay(100);
    if(!_SWDMX_FlvGetDataFromFeeder(prSwdmxFlvInfo,prFeederData,5))
    {
       if(prSwdmxFlvInfo->fgPlayAbort)
       {
          return FLV_SEEK_RET_PLAYABORT;
       }
       else
       {
           return FLV_SEEK_RET_FILEERR;
       }
    }

    if(prFeederData->u4WriteAddr==0)
    {
       prSwdmxFlvInfo->fgFeederInvalid=TRUE;
       return FLV_SEEK_RET_PLAYABORT;
    }

    puReadPos=(UCHAR *)(prFeederData->u4WriteAddr+prFeederData->u4AlignOffset);

    u4ReqSize=prFeederData->u4ReadSize-prFeederData->u4AlignOffset;
    if((u4ReqSize>=SWDMX_FLV_FILE_HEADER_SIZE+1)&&fgVerify && !fgVerifyed)
    {
       uAudIdy=(prSwdmxFlvInfo->u2TagIdentify&0xff00)>>8;
       uVidIdy=prSwdmxFlvInfo->u2TagIdentify&0xf;
       if(uAudIdy==0 || uVidIdy==0)
       {
          uIdyLen=3;
          uAudIdy=0;
          uVidIdy=0;
          uMetIdy=0;
       }

       puReadPos=(UCHAR *)(prFeederData->u4WriteAddr);
       FLV_RPOS_INC(puReadPos,prFeederData->u4AlignOffset,puStart,puEnd);
       FLV_INTEGER(uTagType,puReadPos,1,puStart,puEnd);
       FLV_INTEGER(u4TagLen,puReadPos,3,puStart,puEnd);
       FLV_RPOS_INC(puReadPos,4,puStart,puEnd);
       FLV_INTEGER(u4ErrorIdentify,puReadPos,uIdyLen,puStart,puEnd);
       if((uTagType==eFLV_TAG_TYPE_AUD && (u4ErrorIdentify==uAudIdy))
           ||(uTagType==eFLV_TAG_TYPE_VID && ((u4ErrorIdentify&0xffffff0f)==uVidIdy))
           ||(uTagType==eFLV_TAG_TYPE_DATA&&(u4ErrorIdentify==uMetIdy)))
       {
           // Verify Pass.
           fgVerifyed=TRUE;
       }
       else
       {
          return FLV_SEEK_RET_FILEERR;
       }
    }

    if(u4ReadSize==0)
    {
       if((prFeederData->u4ReadSize-prFeederData->u4AlignOffset)<u4TagLen+SWDMX_FLV_TAG_HEADER_SIZE)
       {
         if((prSwdmxFlvInfo->u8CurDmxMoviPos+(prFeederData->u4ReadSize-prFeederData->u4AlignOffset))>=prSwdmxFlvInfo->prRangeList->prDmxRangeInfo->z_range_sz)
         {
           return FLV_SEEK_RET_FILEEND;
         }
         u4ReqSize=u4TagLen+SWDMX_FLV_TAG_HEADER_SIZE;
         goto Retry;
       }
    }

    return FLV_SEEK_RET_FIND;
}

static INT32 _SWDMX_FlvSeekWithoutIndex(SWDMX_FLV_INFO_T *prSwdmxFlvInfo, UINT64 u8SeekTime, UINT8 u4SeekMode,MM_RANGE_INFO_T *prDmxRangeInfo)
{
    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;
    UINT32 u4DmxPts=0,u4FindPts=0,u4ErrorIdentify=0,u4AudIdy,u4VidIdy,u4MedIdy;
    UINT32 u4ReadSize=0,u4ReadOffset,u4TagSize=0,u4KeyTagSize=0,u4PreTagSize=0,u4SkipSize=0,u4KeyOffset=0,u4KeyPts=0;
    UCHAR *puReadPos=NULL,*puStart,*puEnd;
    INT32 i4Ret=0;
    UINT8 uData,uTagType,uIdLen=4;
    BOOL fgFindIFrame=FALSE,fgFrwOrBkw=0,fgGetTime=0,fgStreamError=FALSE;
    SWDMX_INFO_T* prSwdmxInst = (SWDMX_INFO_T* )prSwdmxFlvInfo->u4SwdmxInst;
    u4DmxPts=prSwdmxFlvInfo->u4LastPts;
    //u4FilePos=prSwdmxFlvInfo->u4ValidSeekPos;
    u4ReadOffset=(prSwdmxFlvInfo->u8CurMoviPos == 0) ? (SWDMX_FLV_FILE_HEADER_SIZE + SWDMX_FLV_TAG_ENDSIZE) : prSwdmxFlvInfo->u4ValidSeekPos;
    puStart=(UCHAR *)(prSwdmxFlvInfo->prFeederInfo->u4StartAddr);
    puEnd= (UCHAR *)(prSwdmxFlvInfo->prFeederInfo->u4EndAddr);
    _SWDMX_FlvResetFeeder(prSwdmxFlvInfo);
    fgFrwOrBkw= u4SeekMode ? ((u4SeekMode==eFLV_SEEK_MODE_NEXT_AUD ||u4SeekMode==eFLV_SEEK_MODE_NEXT_I) ? TRUE : FALSE) : (u8SeekTime<u4DmxPts ? FALSE : TRUE);
    u4FindPts= u4DmxPts;
    u4SkipSize=0;
    //1-->Forward,0-->BackWard;

    u4AudIdy=((prSwdmxFlvInfo->u2TagIdentify&0XFF00)>>8);
    u4VidIdy=prSwdmxFlvInfo->u2TagIdentify&0xff;
    u4MedIdy=0x2;
    if(u4AudIdy==0 || u4VidIdy==0)
    {
      uIdLen=3;
      u4AudIdy=0;
      u4VidIdy=0;
      u4MedIdy=0;
    }

    if(!fgFrwOrBkw)
    {
       while(1)
       {
          i4Ret=FeederSetRequest(prSwdmxInst->eFeederSrc, SWDMX_FLV_TAG_ENDSIZE+SWDMX_FLV_TAG_HEADER_SIZE+1,u4ReadOffset-SWDMX_FLV_TAG_ENDSIZE,&rFeederCond,prSwdmxFlvInfo->u4FeederReqID);
          if(i4Ret!=FEEDER_E_OK)
          {
             _SWDMX_FlvResetFeeder(prSwdmxFlvInfo);
             i4Ret=FeederSetRequest(prSwdmxInst->eFeederSrc, SWDMX_FLV_TAG_ENDSIZE+SWDMX_FLV_TAG_HEADER_SIZE+1,u4ReadOffset-SWDMX_FLV_TAG_ENDSIZE,&rFeederCond,prSwdmxFlvInfo->u4FeederReqID);
             if(i4Ret==FEEDER_E_OK)
             {
                prSwdmxFlvInfo->u4FeederReqID++;
                break;
             }
             else
             {
                return FLV_SEEK_RET_FILEERR;
             }
          }
          else
          {
             prSwdmxFlvInfo->u4FeederReqID++;
             break;
          }
        }

        if(_SWDMX_FlvGetDataFromFeeder(prSwdmxFlvInfo,&rFeederReqData,5)==FALSE)
        {
           return FLV_SEEK_RET_FILEERR;
        }
        puReadPos =  (UCHAR *)VIRTUAL(rFeederReqData.u4WriteAddr + rFeederReqData.u4AlignOffset);
        FLV_INTEGER(u4TagSize,puReadPos,4,puStart,puEnd);
        FLV_INTEGER(uTagType,puReadPos,1,puStart,puEnd);
        FLV_RPOS_INC(puReadPos,7,puStart,puEnd);
        FLV_INTEGER_NEXT(u4ErrorIdentify,puReadPos,uIdLen,puStart,puEnd);
        u4PreTagSize=u4TagSize;
        if((uTagType==eFLV_TAG_TYPE_AUD && (u4ErrorIdentify==u4AudIdy))
            ||(uTagType==eFLV_TAG_TYPE_VID && ((u4ErrorIdentify&0xffffff0f)==u4VidIdy))
            ||(uTagType==eFLV_TAG_TYPE_DATA&&(u4ErrorIdentify==u4MedIdy)))
        {
            if(u4TagSize+SWDMX_FLV_TAG_ENDSIZE+SWDMX_FLV_TAG_ENDSIZE+SWDMX_FLV_FILE_HEADER_SIZE>u4ReadOffset)
            {
                u4TagSize=0;
                fgStreamError=TRUE;
            }
            else
            {
                u4ReadOffset-=(u4TagSize+SWDMX_FLV_TAG_ENDSIZE+SWDMX_FLV_TAG_ENDSIZE);
            }
        }

        else
        {
           u4TagSize=0;
           fgStreamError=TRUE;
        }
    }

    while(!fgStreamError)
    {
       if(u4KeyOffset && u4SeekMode==eFLV_SEEK_MODE_BACKWARD_AFTER_TIME && u4FindPts<u8SeekTime)
       {
         u4ReadOffset=u4KeyOffset;
         u4FindPts=u4KeyPts;
         u4TagSize=u4KeyTagSize;
         break;
       }

       if(fgFindIFrame)
       {
         fgFindIFrame = FALSE;
         if(u4KeyOffset==0)
         {
            u4KeyOffset=u4ReadOffset;
            u4KeyPts=u4FindPts;
         }

         if(((UINT32)(u8SeekTime>>32) != 0xFFFFFFFF)|| ((UINT32)(u8SeekTime) != 0xFFFFFFFF))
         {
            fgGetTime=FALSE;
            if(fgFrwOrBkw && u4FindPts>=u8SeekTime)
            {
               fgGetTime=TRUE;
            }
            else if((!fgFrwOrBkw) && u4FindPts<=u8SeekTime)
            {
               fgGetTime=TRUE;
            }
          }
          else
          {
             fgGetTime=TRUE;
          }

          if(u4SeekMode==eFLV_SEEK_MODE_NEXT_I || u4SeekMode==eFLV_SEEK_MODE_PREVIOUS_I) // next/previous k frame.
          {
             if(prSwdmxFlvInfo->fgStartTrick)
             {
                prSwdmxFlvInfo->fgStartTrick=FALSE;
                break;
             }
             else if(u4SeekMode==eFLV_SEEK_MODE_NEXT_I && u4ReadOffset!=prSwdmxFlvInfo->u4ValidSeekPos && u8SeekTime&&fgGetTime)
             {
                break;
             }
             else if(u4SeekMode==eFLV_SEEK_MODE_PREVIOUS_I)
             {
                break;
             }
           }
           else if(u4SeekMode==eFLV_SEEK_MODE_BACKWARD_AFTER_TIME && u4FindPts<u8SeekTime)
           {
             u4ReadOffset=u4KeyOffset;
             u4FindPts=u4KeyPts;
             u4TagSize=u4KeyTagSize;
             break;
           }
           else if(u4SeekMode==eFLV_SEEK_MODE_BACKWARD_AFTER_TIME && u4FindPts==u8SeekTime)
           {
             break;
           }
           else if(((!fgFrwOrBkw) && u4FindPts<=u8SeekTime) || (fgFrwOrBkw && u4FindPts>=u8SeekTime))
           {
             break;
           }

           u4KeyOffset=u4ReadOffset;
           u4KeyPts=u4FindPts;
           u4KeyTagSize=u4TagSize;
        }

        if(fgFrwOrBkw)
        {
            if(u4ReadOffset+u4SkipSize+SWDMX_FLV_TAG_ENDSIZE>=prSwdmxFlvInfo->prRangeList->prDmxRangeInfo->z_range_sz)
            {
              return FLV_SEEK_RET_FILEEND;
            }
        }
        else
        {
            if(u4PreTagSize==0 && u4ReadOffset==SWDMX_FLV_FILE_HEADER_SIZE)
            {
               return FLV_SEEK_RET_FILEEND;
            }
            else if(u4PreTagSize>u4ReadOffset-SWDMX_FLV_FILE_HEADER_SIZE)
            {
               u4TagSize=0;
               fgStreamError=TRUE;
               break;
            }

            if(u4ReadOffset<u4SkipSize+SWDMX_FLV_FILE_HEADER_SIZE)
            {
                if(u4PreTagSize==0)
                {
                    return FLV_SEEK_RET_FILEEND;
                }
                else
                {
                    u4TagSize=0;
                    fgStreamError=TRUE;
                    break;
                }

            }
        }

        u4ReadOffset= fgFrwOrBkw ? (u4ReadOffset+u4SkipSize) : (u4ReadOffset-u4SkipSize);
        u4ReadSize= fgFrwOrBkw ? (SWDMX_FLV_TAG_HEADER_SIZE+1) : (SWDMX_FLV_TAG_HEADER_SIZE+SWDMX_FLV_TAG_ENDSIZE+1);
        if(u4ReadOffset>0 && u4ReadOffset<prDmxRangeInfo->z_range_sz)
        {
           while(1)
           {
              i4Ret=FeederSetRequest(prSwdmxInst->eFeederSrc, u4ReadSize,u4ReadOffset,&rFeederCond,prSwdmxFlvInfo->u4FeederReqID);
              if(i4Ret!=FEEDER_E_OK)
              {
                 _SWDMX_FlvResetFeeder(prSwdmxFlvInfo);
                 i4Ret=FeederSetRequest(prSwdmxInst->eFeederSrc, u4ReadSize,u4ReadOffset,&rFeederCond,prSwdmxFlvInfo->u4FeederReqID);
                 if(i4Ret!=FEEDER_E_OK)
                 {
                     return FLV_SEEK_RET_FILEERR;
                 }
                 else
                 {  
                     prSwdmxFlvInfo->u4FeederReqID++;
                     break;
                 }
              }
              else
              {
                 prSwdmxFlvInfo->u4FeederReqID++;
                 break;
              }
           }

           if(!_SWDMX_FlvGetDataFromFeeder(prSwdmxFlvInfo, &rFeederReqData,5))
           {
              fgStreamError=TRUE;
              break;
           }
           
           puReadPos =  (UCHAR *)VIRTUAL(rFeederReqData.u4WriteAddr + rFeederReqData.u4AlignOffset);
           if(!fgFrwOrBkw)
           {
              FLV_INTEGER(u4PreTagSize,puReadPos,4,puStart,puEnd);
           }

           FLV_INTEGER(uTagType,puReadPos,1,puStart,puEnd);
           FLV_INTEGER(u4TagSize,puReadPos,3,puStart,puEnd);
           FLV_INTEGER(u4FindPts,puReadPos,3,puStart,puEnd);
           FLV_INTEGER(uData,puReadPos,1,puStart,puEnd);
           u4FindPts=u4FindPts|(uData<<24);
           u4FindPts=u4FindPts*90;
           FLV_INTEGER_NEXT(u4ErrorIdentify,puReadPos,uIdLen,puStart,puEnd);
           if((uTagType==eFLV_TAG_TYPE_AUD && (u4ErrorIdentify==u4AudIdy))
               ||(uTagType==eFLV_TAG_TYPE_VID && ((u4ErrorIdentify&0xffffff0f)==u4VidIdy))
               ||(uTagType==eFLV_TAG_TYPE_DATA&&(u4ErrorIdentify==u4MedIdy)))
           {

           }
           else
           {
              u4TagSize=0;
              fgStreamError=TRUE;
              break;
           }


           FLV_RPOS_INC(puReadPos,3,puStart,puEnd);

           if(!prSwdmxFlvInfo->fgEnVideo && prSwdmxFlvInfo->fgEnAudio)
           {
              fgFindIFrame=TRUE;
           }
           else if(uTagType==eFLV_TAG_TYPE_VID)
           {
              fgFindIFrame=((((*puReadPos)>>4)&0xf)==1) ? TRUE : FALSE;
              if(u4SeekMode==eFLV_SEEK_MODE_NEXT_AUD)
              {
                fgFindIFrame=FALSE;
              }
           }
           else if(uTagType==eFLV_TAG_TYPE_AUD)
           {
              prSwdmxFlvInfo->u4RecSeekAtime=u4FindPts;
              if(u4SeekMode==eFLV_SEEK_MODE_NEXT_AUD)
              {
                return FLV_SEEK_RET_FIND;
              }
           }

           u4SkipSize=fgFrwOrBkw ? (u4TagSize+SWDMX_FLV_TAG_HEADER_SIZE+SWDMX_FLV_TAG_ENDSIZE) : (u4PreTagSize+SWDMX_FLV_TAG_ENDSIZE);
        }
        else
        {
           break;
        }
   }

   prSwdmxFlvInfo->u4SeekTagLen=u4TagSize+SWDMX_FLV_TAG_HEADER_SIZE;
   u4ReadOffset= fgFrwOrBkw ? u4ReadOffset : (u4ReadOffset+SWDMX_FLV_TAG_ENDSIZE);
   prSwdmxFlvInfo->u4ValidSeekPos=u4ReadOffset;
   prSwdmxFlvInfo->u8CurMoviPos=u4ReadOffset;
   prSwdmxFlvInfo->u8CurDmxMoviPos=u4ReadOffset;
   prSwdmxFlvInfo->u4RecSeekTime=u4FindPts;
   _SWDMX_FlvResetFeeder(prSwdmxFlvInfo);
   if(fgStreamError)
   {
     return FLV_SEEK_RET_FILEERR;
   }
   else
   {
     return FLV_SEEK_RET_FIND;
   }
}

static INT32 _SWDMX_FlvNextKeyFrame(SWDMX_FLV_INFO_T *prSwdmxFlvInfo,BOOL fgFWard)
{
    UINT64 u8SeekTime;
    INT32 u4CurKeyIndex=0,i4Ret=FLV_SEEK_RET_FIND;
    UINT8 uForBward=eFLV_SEEK_MODE_NEXT_I;
    MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl = prSwdmxFlvInfo->prVidKeyIdxTbl;
    MM_RANGE_INFO_T *prDmxRangeInfo=prSwdmxFlvInfo->prRangeList->prDmxRangeInfo;
    FEEDER_REQ_DATA_T rFeederData;

    if (!prSwdmxFlvInfo->fgKeyExist||prSwdmxFlvInfo->prVidKeyIdxTbl->ui4_number_of_entry == 0)
    {
        {
           uForBward=fgFWard ? eFLV_SEEK_MODE_NEXT_I : eFLV_SEEK_MODE_PREVIOUS_I;
        }
        u8SeekTime = 0xFFFFFFFF;
        u8SeekTime <<= 32;
        u8SeekTime += 0xFFFFFFFF;
        i4Ret=_SWDMX_FlvSeekWithoutIndex(prSwdmxFlvInfo,u8SeekTime,uForBward,prDmxRangeInfo);
        if(i4Ret==FLV_SEEK_RET_FIND)
        {
            i4Ret=_SWDMX_FlvReqTagData(prSwdmxFlvInfo,prSwdmxFlvInfo->u8CurDmxMoviPos,prSwdmxFlvInfo->u4SeekTagLen,TRUE,&rFeederData);
        }
    }
    else
    {
        u4CurKeyIndex=prSwdmxFlvInfo->u4CurSentKey;
        if(fgFWard)
        {
           u4CurKeyIndex=flv_min(u4CurKeyIndex+1,prVidKeyIdxTbl->ui4_number_of_entry-1);
        }
        else
        {
           if(u4CurKeyIndex>0)
           {
             u4CurKeyIndex--;
           }
           else
           {
             u4CurKeyIndex=0;
           }
        }
        if(u4CurKeyIndex==prSwdmxFlvInfo->u4CurSentKey || (prVidKeyIdxTbl->ui8_base_offset + prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKeyIndex].ui4_relative_offset)>=prDmxRangeInfo->z_range_sz)
        {
          i4Ret=FLV_SEEK_RET_FILEEND;
        }
        else
        {
          prSwdmxFlvInfo->u4CurSentKey=u4CurKeyIndex;
          if(prSwdmxFlvInfo->fgRecoveredFromErr)
          {

              uForBward=fgFWard ? eFLV_SEEK_MODE_NEXT_I : eFLV_SEEK_MODE_PREVIOUS_I;
              //uForBward=eFLV_SEEK_MODE_NEXT_I;
              u8SeekTime = 0xFFFFFFFF;
              u8SeekTime <<= 32;
              u8SeekTime += 0xFFFFFFFF;
              i4Ret=_SWDMX_FlvSeekWithoutIndex(prSwdmxFlvInfo,u8SeekTime,uForBward,prDmxRangeInfo);
              if(i4Ret==FLV_SEEK_RET_FIND)
              {
                  i4Ret=_SWDMX_FlvReqTagData(prSwdmxFlvInfo,prSwdmxFlvInfo->u8CurDmxMoviPos,prSwdmxFlvInfo->u4SeekTagLen,TRUE,&rFeederData);
              }
              prSwdmxFlvInfo->fgRecoveredFromErr=FALSE;
          }
          else
          {
              prSwdmxFlvInfo->u8CurDmxMoviPos = prVidKeyIdxTbl->ui8_base_offset + prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKeyIndex].ui4_relative_offset;
              prSwdmxFlvInfo->u8CurMoviPos = prSwdmxFlvInfo->u8CurDmxMoviPos;
              prSwdmxFlvInfo->u4ValidSeekPos=prSwdmxFlvInfo->u8CurDmxMoviPos;
              prSwdmxFlvInfo->u4RecSeekTime=prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKeyIndex].ui4_pts;
              LOG(5, "Flv Seek key frame number = 0x%08x Video offset = %lld\n", prSwdmxFlvInfo->u4CurSentKey,prSwdmxFlvInfo->u8CurDmxMoviPos);
              LOG(5, "Flv Seek Video pts = 0x%08x\n", prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKeyIndex].ui4_pts);
              _SWDMX_FlvResetFeeder(prSwdmxFlvInfo);
              i4Ret=_SWDMX_FlvReqTagData(prSwdmxFlvInfo,prSwdmxFlvInfo->u8CurDmxMoviPos,0,TRUE,&rFeederData);
          }

          LOG(7, "_SWDMX_FlvSeek Key = %ld\n", prSwdmxFlvInfo->u4CurSentKey);
        }
    }

    if(i4Ret==FLV_SEEK_RET_FIND)
    {
      prSwdmxFlvInfo->u4LeftBytesInFeeder = rFeederData.u4ReadSize-rFeederData.u4AlignOffset;
      prSwdmxFlvInfo->u4CurDmxFeederRPtr =  VIRTUAL(rFeederData.u4WriteAddr + rFeederData.u4AlignOffset);
    }
    else if(i4Ret==FLV_SEEK_RET_FILEEND)
    {
        prSwdmxFlvInfo->u4LeftBytesInFeeder = 0;
        prSwdmxFlvInfo->u8CurDmxMoviPos=prDmxRangeInfo->z_range_sz;
        prSwdmxFlvInfo->u4LastPts=(UINT32)prSwdmxFlvInfo->prRangeList->prDmxRangeInfo->ui8_vid_duration;
    }
    else if(i4Ret==FLV_SEEK_RET_FILEERR)
    {
       if(uForBward==eFLV_SEEK_MODE_PREVIOUS_I)
       {
          if(prSwdmxFlvInfo->u8CurDmxMoviPos>(SWDMX_FLV_FILE_HEADER_SIZE+FLV_ERROR_RECOVERY_LEN))
          {
             prSwdmxFlvInfo->u8CurDmxMoviPos= prSwdmxFlvInfo->u8CurDmxMoviPos-FLV_ERROR_RECOVERY_LEN;
          }
          else
          {
             prSwdmxFlvInfo->u4LeftBytesInFeeder = 0;
             prSwdmxFlvInfo->u8CurDmxMoviPos=prDmxRangeInfo->z_range_sz;
             prSwdmxFlvInfo->u4LastPts=(UINT32)prSwdmxFlvInfo->prRangeList->prDmxRangeInfo->ui8_vid_duration;
             i4Ret= FLV_SEEK_RET_FILEEND;
          }
       }
    }
    return i4Ret;
}

#if 0
static BOOL _SWDMX_FlvSeekPos(SWDMX_FLV_INFO_T *prSwdmxFlvInfo, UINT64 u8SeekPos,UINT64 *u8Pos,UINT32 *u4Pts)
{
   INT32 i=0;
   MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl = prSwdmxFlvInfo->prVidKeyIdxTbl;
   MM_RANGE_INFO_T *prDmxRangeInfo=prSwdmxFlvInfo->prRangeList->prDmxRangeInfo;
   if(!prSwdmxFlvInfo->fgKeyExist)
   {
     return FALSE;
   }

   if(u8SeekPos>prDmxRangeInfo->z_range_sz)
   {
     return FALSE;
   }

   for (i = 0; i < prVidKeyIdxTbl->ui4_number_of_entry; i++)
   {
      if((prVidKeyIdxTbl->ui8_base_offset+prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_relative_offset)>=u8SeekPos)
      {
        break;
      }
   }

   if(i==prVidKeyIdxTbl->ui4_number_of_entry)
   {
     return FALSE;
   }

   *u8Pos=prVidKeyIdxTbl->ui8_base_offset+prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_relative_offset;
   *u4Pts=prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts;
   return TRUE;
}
#endif

static INT32 _SWDMX_FlvSeekCurPos(SWDMX_FLV_INFO_T *prSwdmxFlvInfo,UINT8 u1B2RId)
{
   VDP_POS_INTO_T rVdpPosInfo;
   if(VDP_GetPosInfo(u1B2RId,&rVdpPosInfo))
   {
	   prSwdmxFlvInfo->u4RecSeekTime=rVdpPosInfo.u8Pts;
	   prSwdmxFlvInfo->u4RecSeekAtime=rVdpPosInfo.u8AudPts;
	   prSwdmxFlvInfo->u4AudStartPts=prSwdmxFlvInfo->u4RecSeekAtime;
	   prSwdmxFlvInfo->u8CurDmxMoviPos=rVdpPosInfo.u8OffsetI;
	   prSwdmxFlvInfo->u8CurMoviPos = prSwdmxFlvInfo->u8CurDmxMoviPos;
	   prSwdmxFlvInfo->fgTagHeaderBroken=FALSE;
	   prSwdmxFlvInfo->fgTagDataBroken=FALSE;
	   prSwdmxFlvInfo->fgTagHeaderParsed=FALSE;
	   prSwdmxFlvInfo->u4SkipDatLen=0;
	   prSwdmxFlvInfo->u4FrameAddrToVdec=0;
	   _SWDMX_FlvResetFeeder(prSwdmxFlvInfo);
	   return FLV_SEEK_RET_FIND;
   }
   return FLV_SEEK_RET_FILEEND;
}

static INT32 _SWDMX_FlvSeek(SWDMX_FLV_INFO_T *prSwdmxFlvInfo, UINT64 u8SeekTime,INT32 i4PlaySpeed,FLV_SEEK_TYPE eSeekType)
{
    INT32 i,u4CurKeyIndex=0,i4Ret=FLV_SEEK_RET_FIND;
    MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl = prSwdmxFlvInfo->prVidKeyIdxTbl;
    MM_RANGE_INFO_T *prDmxRangeInfo=prSwdmxFlvInfo->prRangeList->prDmxRangeInfo;
    SWDMX_INFO_T* prSwdmxInst = (SWDMX_INFO_T* )prSwdmxFlvInfo->u4SwdmxInst;
    UINT8 uSeekMode=0;
    LOG(7, "_SWDMX_FlvSeek SeekType=%d,u8SeekTime = 0x%08x\n", eSeekType,u8SeekTime);

    //uSeekMode: 0,seek as u8SeekTime and lastpts, 1,seek forward lastpts,firstkeyframe, 2: backware firstkeyframe, 3 backward find keyframe, until u8SeekTime

    prSwdmxFlvInfo->fgTagHeaderBroken=FALSE;
    prSwdmxFlvInfo->fgTagDataBroken=FALSE;
    prSwdmxFlvInfo->fgTagHeaderParsed=FALSE;
    prSwdmxFlvInfo->u4SkipDatLen=0;
    prSwdmxFlvInfo->u4FrameAddrToVdec=0;

    if (!prSwdmxFlvInfo->fgKeyExist || prSwdmxFlvInfo->prVidKeyIdxTbl->ui4_number_of_entry == 0)
    {
       UINT64 u8FileOffset=0,u4FileOffsetI=0;
       INT32 i4TempRef=0;
       UINT32 u4VdpPts=0;
       UINT16 u2DecodOrder=0;

       if(eSeekType==eFLV_SEEK_TYPE_TIME)
       {
          u4VdpPts=VDP_GetPts(prSwdmxInst->u1B2rId);
          if(u8SeekTime>=prSwdmxFlvInfo->u4LastPts)
          {
             uSeekMode=eFLV_SEEK_MODE_NEXT_I;
          }
          else if(u8SeekTime<prSwdmxFlvInfo->u4LastPts && u8SeekTime>u4VdpPts)
          {
             uSeekMode=eFLV_SEEK_MODE_BACKWARD_AFTER_TIME;
          }
          else if(u8SeekTime<=u4VdpPts)
          {
              VDP_GetFilePosition(prSwdmxInst->u1B2rId,&i4TempRef,&u2DecodOrder,&u8FileOffset,&u4FileOffsetI);
              if(u8FileOffset!=0)
              {
                prSwdmxFlvInfo->u4ValidSeekPos=(u8FileOffset&0xffffffff);
                prSwdmxFlvInfo->u4LastPts=u4VdpPts;
                if(u8SeekTime==u4VdpPts)
                {
                    uSeekMode=eFLV_SEEK_MODE_NEXT_I;
                }
                else
                {
                    uSeekMode=eFLV_SEEK_MODE_BACKWARD_BEFORE_TIME;
                }
              }
              else
              {
                  uSeekMode=eFLV_SEEK_MODE_BACKWARD_BEFORE_TIME;
              }
          }
       }
       else if(eSeekType==eFLV_SEEK_TYPE_TRICK)
       {
          VDP_GetFilePosition(prSwdmxInst->u1B2rId,&i4TempRef,&u2DecodOrder,&u8FileOffset,&u4FileOffsetI);
          if(u8FileOffset!=0)
          {
              prSwdmxFlvInfo->u4ValidSeekPos=(u8FileOffset&0xffffffff);
              prSwdmxFlvInfo->u4LastPts=u8SeekTime;

              if(i4PlaySpeed<0)
              {
                  uSeekMode=eFLV_SEEK_MODE_BACKWARD_AFTER_TIME;
              }
              else
              {
                  uSeekMode=eFLV_SEEK_MODE_NEXT_I;
              }
          }
          else
          {
              if(i4PlaySpeed>0)
              {
                 if(prSwdmxFlvInfo->i4PrevSpeed<0)
                 {
                    uSeekMode=eFLV_SEEK_MODE_BACKWARD_BEFORE_TIME;
                 }
                 else if((u8SeekTime>prSwdmxFlvInfo->u4LastKeyPts)||(u8SeekTime==prSwdmxFlvInfo->u4LastKeyPts && prSwdmxFlvInfo->u4LastKeyPts==prSwdmxFlvInfo->u4LastPts))
                 {
                   uSeekMode=eFLV_SEEK_MODE_NEXT_I; // next i frame.
                 }
                 else if((u8SeekTime==prSwdmxFlvInfo->u4LastKeyPts) && prSwdmxFlvInfo->i4PrevSpeed<0)
                 {
                   uSeekMode=eFLV_SEEK_MODE_NEXT_I; //next i frame.
                 }
                 else
                 {
                   uSeekMode=eFLV_SEEK_MODE_BACKWARD_AFTER_TIME; // seek back ward, but can't less than u8SeekTime;
                 }
              }
              else if(i4PlaySpeed<0)
              {
                 uSeekMode=eFLV_SEEK_MODE_BACKWARD_BEFORE_TIME; // seek backware,  target time will  less than u8SeekTime;
              }
          }
       }
       else if(eSeekType==eFLV_SEEK_TYPE_OFFSET)
       {
          prSwdmxFlvInfo->u4ValidSeekPos=u8SeekTime;
          prSwdmxFlvInfo->u8CurMoviPos=u8SeekTime;
          prSwdmxFlvInfo->u8CurDmxMoviPos=u8SeekTime;
          if(u8SeekTime>=prDmxRangeInfo->z_range_sz)
          {
             i4Ret=FLV_SEEK_RET_FILEEND;
          }
          _SWDMX_FlvResetFeeder(prSwdmxFlvInfo);
       }

       if(eSeekType!=eFLV_SEEK_TYPE_OFFSET)
       {
           i4Ret=_SWDMX_FlvSeekWithoutIndex(prSwdmxFlvInfo,u8SeekTime,uSeekMode,prDmxRangeInfo);
       }
        LOG(5, "_SWDMX_FlvSeek without index table\n");
    }
    else
    {
        if(eSeekType==eFLV_SEEK_TYPE_OFFSET)
        {
           UINT32 u4IndexOffset=0;
           prSwdmxFlvInfo->u4ValidSeekPos=u8SeekTime;
           prSwdmxFlvInfo->u8CurMoviPos=u8SeekTime;
           prSwdmxFlvInfo->u8CurDmxMoviPos=u8SeekTime;
           if(u8SeekTime>=prDmxRangeInfo->z_range_sz)
           {
              i4Ret=FLV_SEEK_RET_FILEEND;
           }
           
           for (i = 0; i < prVidKeyIdxTbl->ui4_number_of_entry; i++)
           {
               u4IndexOffset=prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_relative_offset;
               if (u4IndexOffset+prVidKeyIdxTbl->ui8_base_offset> u8SeekTime)
               {
                   break;
               }
           }

           if(eSeekType==eFLV_SEEK_TYPE_TRICK && prSwdmxFlvInfo->i4PrevSpeed>0)
           {
              if(prSwdmxFlvInfo->i4PrevSpeed<2000&&i4PlaySpeed==1000)
              {
                  prSwdmxFlvInfo->u4CurSentKey= i>0 ? (i-1) : 0;
              }
              else
              {
                  prSwdmxFlvInfo->u4CurSentKey= i;
              }
           }
           else
           {
              prSwdmxFlvInfo->u4CurSentKey= i>0 ? (i-1) : 0;
           }
           _SWDMX_FlvResetFeeder(prSwdmxFlvInfo);           
        }
        else
        {
            for (i = 0; i < prVidKeyIdxTbl->ui4_number_of_entry; i++)
            {
                if (prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts > u8SeekTime)
                {
                    break;
                }
            }
            
            if(i==prVidKeyIdxTbl->ui4_number_of_entry ||(prVidKeyIdxTbl->ui8_base_offset + prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_relative_offset)>=prDmxRangeInfo->z_range_sz)
            {
              if(i4PlaySpeed<0)
              {
                 i=prVidKeyIdxTbl->ui4_number_of_entry-1;
                 i4Ret=FLV_SEEK_RET_FIND;
              }
              else
              {
                  i4Ret=FLV_SEEK_RET_FILEEND;
              }
            }
            
            if(i4Ret==FLV_SEEK_RET_FIND)
            {
                if(eSeekType==eFLV_SEEK_TYPE_TRICK && prSwdmxFlvInfo->i4PrevSpeed>0)
                {
                   if(prSwdmxFlvInfo->i4PrevSpeed<2000&&i4PlaySpeed==1000)
                   {
                       prSwdmxFlvInfo->u4CurSentKey= i>0 ? (i-1) : 0;
                   }
                   else
                   {
                       prSwdmxFlvInfo->u4CurSentKey= i;
                   }
                }
                else
                {
                   prSwdmxFlvInfo->u4CurSentKey= i>0 ? (i-1) : 0;
                }
            
                u4CurKeyIndex=prSwdmxFlvInfo->u4CurSentKey;
                prSwdmxFlvInfo->u8CurDmxMoviPos = prVidKeyIdxTbl->ui8_base_offset + prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKeyIndex].ui4_relative_offset;
                prSwdmxFlvInfo->u8CurMoviPos = prSwdmxFlvInfo->u8CurDmxMoviPos;
                prSwdmxFlvInfo->u4ValidSeekPos=prSwdmxFlvInfo->u8CurDmxMoviPos;
                prSwdmxFlvInfo->u4RecSeekTime=(UINT32)prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKeyIndex].ui4_pts;;
                prSwdmxFlvInfo->u4RecSeekAtime=prSwdmxFlvInfo->u4RecSeekTime;
                
                LOG(5, "Flv Seek key frame number = 0x%08x Video offset = %lld\n", prSwdmxFlvInfo->u4CurSentKey,prSwdmxFlvInfo->u8CurDmxMoviPos);
                LOG(5, "Flv Seek Video pts = 0x%08x\n", prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKeyIndex].ui4_pts);
                _SWDMX_FlvResetFeeder(prSwdmxFlvInfo);
                LOG(7, "_SWDMX_FlvSeek Key = %ld\n", prSwdmxFlvInfo->u4CurSentKey);
            }

        }
    }

    if(i4Ret==FLV_SEEK_RET_FILEEND)
    {
      prSwdmxFlvInfo->u4LeftBytesInFeeder = 0;
      prSwdmxFlvInfo->u8CurDmxMoviPos=prDmxRangeInfo->z_range_sz;
      prSwdmxFlvInfo->u4LastPts=(UINT32)prSwdmxFlvInfo->prRangeList->prDmxRangeInfo->ui8_vid_duration;
    }
    return i4Ret;
}


static BOOL _SWDMX_FlvSendEos(SWDMX_FLV_INFO_T *prSwdmxFlvInfo, UINT8 u1HdrType)
{
    UINT8 u1Pidx;
    UINT16 u2MaxQNum, u2CurQNum;
    DMX_MM_DATA_T rDmxMMData;
    SWDMX_INFO_T* prSwdmxInst = (SWDMX_INFO_T* )prSwdmxFlvInfo->u4SwdmxInst;

    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, prSwdmxFlvInfo->prFeederInfo) != FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        u1Pidx = prSwdmxFlvInfo->u1VidPid;

        VDEC_GetQueueInfo(prSwdmxFlvInfo->u1EsId, &u2CurQNum, &u2MaxQNum);
        LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
        if (u2CurQNum > (u2MaxQNum - 10))
        {
            x_thread_delay(1);
            return FALSE;
        }

    }
    else
    {
        u1Pidx = prSwdmxFlvInfo->u1AudPid;
    }

    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if (_SWDMX_FlvInitDmx(prSwdmxFlvInfo, u1HdrType, NULL, TRUE) == FALSE)
    {
        return FALSE;
    }

    rDmxMMData.u4BufStart = prSwdmxFlvInfo->prFeederInfo->u4StartAddr;
    rDmxMMData.u4BufEnd = prSwdmxFlvInfo->prFeederInfo->u4EndAddr;
    rDmxMMData.u4StartAddr = prSwdmxFlvInfo->prFeederInfo->u4StartAddr;
    rDmxMMData.u4FrameSize = 4;
    rDmxMMData.fgEOS = TRUE;
    rDmxMMData.u4Pts=prSwdmxFlvInfo->u4LastPts;
    rDmxMMData.u1Idx=u1Pidx;
    LOG(3,"FLV Send %s Eos\n", u1HdrType==eSWDMX_MEDIA_VIDEO ? "Video" : "Audio");
	return _SWDMX_MoveData(prSwdmxFlvInfo->u1SrcId, &rDmxMMData);
}


static BOOL _SWDMX_FlvGetDataFromFeeder(SWDMX_FLV_INFO_T *prSwdmxFlvInfo, FEEDER_REQ_DATA_T *prFeederData,UINT32 u4WaitTime)
{

    UINT32 u4Time=0;
Retry:
    for(u4Time=0;u4Time<u4WaitTime;u4Time++)
    {
       prFeederData->eDataType=FEEDER_SOURCE_TOTAL;
       if(prSwdmxFlvInfo->fgPlayAbort)
       {
          //prSwdmxFlvInfo->u4FeederReqReceiveID=prSwdmxFlvInfo->u4FeederReqID;
          LOG(3,"Abrot,Return from get data %d\n",u4WaitTime);
          return FALSE;
       }
       x_thread_delay(20);
       if(!_SWDMX_ReceiveFeederAck(prSwdmxFlvInfo->u1SrcId,(VOID*)prFeederData))
       {
           LOG(3, "_SWDMX_FlvGetDataFromFeeder FeederFail!!!\n");
           continue;
       }
       
       if(prFeederData->eDataType == FEEDER_SOURCE_INVALID)
       {
           LOG(3, "_SWDMX_FlvGetDataFromFeeder Feeder invlad!!!\n");
           if(u4WaitTime==1)
           {
             break;
           }
       }
       else
       {
           break;
       }
    }

    if(u4Time==u4WaitTime)
    {
        LOG(3, "_SWDMX_FlvGetDataFromFeeder Fail Fail(%d)!!!\n",u4WaitTime);
        //prSwdmxFlvInfo->fgFeederInvalid = TRUE;
        return FALSE;
    }
    
    if( (prFeederData->eDataType != FEEDER_SOURCE_INVALID) && (prFeederData->u4Id) != prSwdmxFlvInfo->u4FeederReqReceiveID)
    {
        if(prSwdmxFlvInfo->i4ReqReadTimes>0 && prFeederData->u4Id>prSwdmxFlvInfo->u4FeederReqReceiveID)
        {
            prSwdmxFlvInfo->i4ReqReadTimes--;
        }
        LOG(3, "Feeder ID mismatch!!! AckId=%d,WaitDd = %d\n", prFeederData->u4Id,prSwdmxFlvInfo->u4FeederReqReceiveID);
        goto Retry;
        //prSwdmxFlvInfo->fgFeederInvalid = TRUE;
        //return FALSE;
    }

    if(prFeederData->eDataType != FEEDER_SOURCE_INVALID)
    {
        prSwdmxFlvInfo->u4FeederReqReceiveID++;
    }
    prSwdmxFlvInfo->fgFeederInvalid = FALSE;
    return TRUE;
}



static SWDMX_HDR_PRS_STATE_T _SWDMX_DmxTransformAvcData
  (SWDMX_FLV_INFO_T *prSwdmxFlvInfo,DMX_MM_DATA_T *prDmxMMData,
   UINT32 u4DataAddr,UINT32 u4DataLen)
{
   UINT32 u4RemainLen,u4SendLen,u4Value;
   UCHAR *puBufStart,*puBufEnd,*puSendPos;
   if(!prSwdmxFlvInfo || !prDmxMMData)
   {
      return eSWDMX_HDR_PRS_FAIL;
   }
   
   puBufStart=(UCHAR *)prDmxMMData->u4BufStart;
   puBufEnd  =(UCHAR *)prDmxMMData->u4BufEnd;
   puSendPos =(UCHAR *)u4DataAddr;
  
   if(!prSwdmxFlvInfo->fgTagDataBroken)// first nal in a video tag
   {
	  prSwdmxFlvInfo->u4AvcHeaderParseInfo=0;
	  prSwdmxFlvInfo->u4NalLen=0;
	  prSwdmxFlvInfo->u4NalSkipLen=prSwdmxFlvInfo->u4LastDataLen;
   }
   
   u4RemainLen=u4DataLen;
   u4Value=(prSwdmxFlvInfo->u4AvcHeaderParseInfo&0xf0)>>4;
   if(u4Value!=3 && u4RemainLen)
   {
      UINT32 u4DataValue;
	  if((prSwdmxFlvInfo->u4AvcHeaderParseInfo&0xf000)!=0x1000)
	  {
		 u4RemainLen--;
		 prSwdmxFlvInfo->u4AvcHeaderParseInfo|=0x1000;
		 FLV_INTEGER(u4DataValue,puSendPos,1,puBufStart,puBufEnd);
		 prSwdmxFlvInfo->u2TagIdentify=(u4DataValue&0xf)|(prSwdmxFlvInfo->u2TagIdentify&0xff00);
		 if(u4RemainLen==0) 
		 {
			return eSWDMX_HDR_PRS_SUCCEED;
		 }
	  }
   
	  if((prSwdmxFlvInfo->u4AvcHeaderParseInfo&0xf00)!=0x100)
	  {
		  u4RemainLen--;
		  prSwdmxFlvInfo->u4AvcHeaderParseInfo|=0x100;
		  FLV_INTEGER(prSwdmxFlvInfo->uCurNalType,puSendPos,1,puBufStart,puBufEnd);
		  if(u4RemainLen==0) 
		  {
			return eSWDMX_HDR_PRS_SUCCEED;
		  }
	  }
	  
	  u4SendLen=flv_min(u4RemainLen,(3-u4Value));
	  FLV_INTEGER(u4DataValue,puSendPos,u4SendLen,puBufStart,puBufEnd);
	  prSwdmxFlvInfo->u4AvcPtsOffset=(prSwdmxFlvInfo->u4AvcPtsOffset<<(u4SendLen*8))|u4DataValue;
	  prSwdmxFlvInfo->u4AvcPtsOffset&=0xffffff;
	  u4RemainLen-=u4SendLen;
      u4Value+=u4SendLen;
	  prSwdmxFlvInfo->u4AvcHeaderParseInfo=(prSwdmxFlvInfo->u4AvcHeaderParseInfo&0xffffff0f)|(u4Value<<4);
	}
	
	while(u4RemainLen)
	{	
	    UINT32 u4SendPts=prSwdmxFlvInfo->u4LastPts;
	    INT32 i4PtsOffset=0;
	    i4PtsOffset=(prSwdmxFlvInfo->u4AvcPtsOffset+0xff800000)^0xff800000;
	    if(i4PtsOffset>0)
	    {
			u4SendPts+=(i4PtsOffset*90);
	    }
		
		if(prSwdmxFlvInfo->uCurNalType==0) // sequence header
		{
		   UINT32 u4PsNum,u4PsIndex=0,u4StartCode=0x00000001;

		   if(u4DataLen<prSwdmxFlvInfo->u4LastDataLen || u4DataLen>SWDMX_FLV_VIDEO_HEADER_SIZE)
		   {
              ASSERT(0);
		   }
		   // first start code
		   prDmxMMData->u4StartAddr = (UINT32)puSendPos;
		   FLV_SET_INTEGER(u4StartCode,puSendPos,SWDMX_FLV_STARTCODE_LEN,puBufStart,puBufEnd);
		   prDmxMMData->u4FrameSize = SWDMX_FLV_STARTCODE_LEN;
		   prDmxMMData->u4Pts=SWDMX_FLV_PTS_INFOR_STARTCODE;
		   if(_SWDMX_MoveData(prSwdmxFlvInfo->u1SrcId,prDmxMMData) != TRUE)
		   {
			 ASSERT(0);
		   }
		   
		   FLV_INTEGER(prSwdmxFlvInfo->uAvcNalLenBytes,puSendPos,1,puBufStart,puBufEnd);
		   prSwdmxFlvInfo->uAvcNalLenBytes=(prSwdmxFlvInfo->uAvcNalLenBytes&0x3)+1;		   
		   for(u4Value=0;u4Value<2;u4Value++)
		   {
		      FLV_INTEGER(u4PsNum,puSendPos,1,puBufStart,puBufEnd);
			  u4PsNum= u4Value==0 ? u4PsNum&0x1f : u4PsNum;
			  for(u4PsIndex=0;u4PsIndex<u4PsNum;u4PsIndex++)
			  {
				  FLV_INTEGER(prSwdmxFlvInfo->u4NalLen,puSendPos,2,puBufStart,puBufEnd);
				  prDmxMMData->u4FrameSize = prSwdmxFlvInfo->u4NalLen+SWDMX_FLV_STARTCODE_LEN;
				  prDmxMMData->u4StartAddr = (UINT32)puSendPos;
				  prDmxMMData->u4Pts=SWDMX_FLV_PTS_INFOR_SEQHEADER;
				  prDmxMMData->u4Dts=prSwdmxFlvInfo->u4NalLen; // use dts to transform nalen.
				                                               //because move data return and video callback is not sync in dmx.
				  if(_SWDMX_MoveData(prSwdmxFlvInfo->u1SrcId,prDmxMMData) != TRUE)
				  {
					ASSERT(0);
				  }
				  FLV_RPOS_INC(puSendPos,prSwdmxFlvInfo->u4NalLen,puBufStart,puBufEnd);
			  }
		   }
		   break;
		}
		else if(prSwdmxFlvInfo->uCurNalType==1) //NAL
		{
		   u4Value=prSwdmxFlvInfo->u4AvcHeaderParseInfo&0xf;
		   if(u4Value<prSwdmxFlvInfo->uAvcNalLenBytes)
		   {
			  UINT32 u4NalLen=0;
			  u4SendLen=flv_min(u4RemainLen,(prSwdmxFlvInfo->uAvcNalLenBytes-u4Value));
			  FLV_INTEGER(u4NalLen,puSendPos,u4SendLen,puBufStart,puBufEnd);
			  prSwdmxFlvInfo->u4NalLen=(prSwdmxFlvInfo->u4NalLen<<(u4SendLen*8))|u4NalLen;
			  u4RemainLen-=u4SendLen;
			  u4Value+=u4SendLen;
			  prSwdmxFlvInfo->u4AvcHeaderParseInfo=(prSwdmxFlvInfo->u4AvcHeaderParseInfo&0xfffffff0)|u4Value;
			  if(u4Value==prSwdmxFlvInfo->uAvcNalLenBytes)
			  {
			     if(prSwdmxFlvInfo->uAvcNalLenBytes<SWDMX_FLV_STARTCODE_LEN)
			     {
                    prSwdmxFlvInfo->u4NalLen&=((1<<(prSwdmxFlvInfo->uAvcNalLenBytes*8))-1);
			     }
				 prSwdmxFlvInfo->u4NalSkipLen=prSwdmxFlvInfo->u4NalLen;
			  }
			  else
			  {
				  prSwdmxFlvInfo->u4NalSkipLen=0;
			  }
		   }
		   
		   if(prSwdmxFlvInfo->u4NalSkipLen && u4RemainLen) 
		   {
			  u4SendLen=flv_min(u4RemainLen,prSwdmxFlvInfo->u4NalSkipLen);
			  u4RemainLen-=u4SendLen;
			  prSwdmxFlvInfo->u4NalSkipLen-=u4SendLen;
			  prDmxMMData->u4StartAddr = (UINT32)puSendPos;
			  FLV_RPOS_INC(puSendPos,u4SendLen,puBufStart,puBufEnd);
			  if(prSwdmxFlvInfo->u4NalSkipLen==0) // Nal end
			  {
				 u4SendLen+=SWDMX_FLV_STARTCODE_LEN;
				 prSwdmxFlvInfo->u4AvcHeaderParseInfo&=0xfffffff0;
				 prDmxMMData->u4Pts=u4SendPts;
				 // use DTS to transform NalLen.
				 //because move data return and video callback is not sync in dmx.
				 prDmxMMData->u4Dts=prSwdmxFlvInfo->u4NalLen; 
			  }
			  else
			  {
                 prDmxMMData->u4Pts=SWDMX_FLV_PTS_INFOR_PARTION;
			  }
			  
			  prDmxMMData->u4FrameSize = u4SendLen;
			  if(_SWDMX_MoveData(prSwdmxFlvInfo->u1SrcId,prDmxMMData) != TRUE)
			  {
				 ASSERT(0);
			  }
		   }
		}
		else
		{
		   break;
		}
	}

	return eSWDMX_HDR_PRS_SUCCEED;

}

static SWDMX_HDR_PRS_STATE_T _SWDMX_DmxTransformData
	(SWDMX_FLV_INFO_T *prSwdmxFlvInfo,
	UINT8 uTagType,UINT32 u4SBufStart,UINT32 u4BufEnd,
	UINT32 u4DataAddr,UINT32 u4DataLen,UINT32 u4Pts)
{
    UINT8 u1Pid;
    UINT32 u4DmxAvailSize=0;
    DMX_MM_DATA_T rDmxMMData;
    UINT32 u4DatType=0;
    SWDMX_INFO_T* prSwdmxInst = (SWDMX_INFO_T* )prSwdmxFlvInfo->u4SwdmxInst;
    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if(uTagType==eFLV_TAG_TYPE_AUD)
    {
        u1Pid = prSwdmxFlvInfo->u1AudPid;
        u4DatType=eSWDMX_MEDIA_AUDIO;
    }
    else if(uTagType==eFLV_TAG_TYPE_VID)
    {
        u1Pid = prSwdmxFlvInfo->u1VidPid;
        u4DatType=eSWDMX_MEDIA_VIDEO;
        LOG(8,"swdmx Senddat SrcId=%d\n",u4DatType);
    }
    else
    {
       return eSWDMX_HDR_PRS_SUCCEED;
    }
    
    if(prSwdmxFlvInfo->fgBeginToPlay)
    {
      if(prSwdmxFlvInfo->u4VDecType==ENUM_SWDMX_VDEC_H264
          &&u4DatType==eSWDMX_MEDIA_VIDEO
          &&(!prSwdmxFlvInfo->fgH264SeqHeaderParsed))//H264
      {  
         UINT32 u4DstBuf,u4SrcBuf;
         u4SrcBuf=u4DataAddr;
         FLV_RPOS_INC(u4DataAddr,1,u4SBufStart,u4BufEnd);
         FLV_INTEGER(u4DatType,u4DataAddr,1,u4SBufStart,u4SBufStart);
         if(u4DatType==0)
         {
           if(u4DataLen<10)
           {
             LOG(3,"FLV error h264 header\n");
           } 
           else
           {
                if (u4DataLen > SWDMX_FLV_VIDEO_HEADER_SIZE)
                {
                    LOG(0, "SWDMX FLV video header buffer too small\n");
                    ASSERT(0);
                    return eSWDMX_HDR_PRS_FAIL;
                }
                prSwdmxFlvInfo->u4VidHdrLen=u4DataLen;
                u4DstBuf=prSwdmxFlvInfo->u4VidHdrPtr;
                FLV_TRANS_DTA(u4SrcBuf,u4DstBuf,u4DataLen,u4SBufStart,u4BufEnd);
                prSwdmxFlvInfo->fgH264SeqHeaderParsed = TRUE;
           }
        }
      }
      return eSWDMX_HDR_PRS_SUCCEED;
    }

    //rDmxMMData.u4Dts=prSwdmxFlvInfo->u4SwdmxInst;
    //u4DmxAvailSize=DMX_MM_GetBufEmptySize(u4DatType);
    u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId,
                                    (DMX_PID_TYPE_T)u4DatType,
                                    u1Pid);
    if(u4DatType!=eSWDMX_MEDIA_AUDIO)
    {
      UINT16 u2MaxQNum, u2CurQNum;

      if(!prSwdmxFlvInfo->fgEnVideo)
      {
          return eSWDMX_HDR_PRS_SUCCEED;
      }

      VDEC_GetQueueInfo(prSwdmxFlvInfo->u1EsId, &u2CurQNum, &u2MaxQNum);
      if((u2CurQNum > (u2MaxQNum - 256)) ||(u2CurQNum > (prSwdmxFlvInfo->u4VidFps * 10)))
      {
         if (prSwdmxFlvInfo->fgVDecIsPause)
         {
            _SWDMX_FlvVideoWakeUp(prSwdmxFlvInfo);
         }

         LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
         x_thread_delay(50);
         return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
      }

      if(u4DmxAvailSize<(u4DataLen+SWDMX_DMX_FREE_BUF))
      {
          x_thread_delay(50);
          return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
      }
    }
    else if(u4DatType==eSWDMX_MEDIA_AUDIO)
    {
      if((!prSwdmxFlvInfo->fgEnAudio)|| u4Pts<prSwdmxFlvInfo->u4AudStartPts)
      {
          return eSWDMX_HDR_PRS_SUCCEED;
      }
       prSwdmxFlvInfo->u4AudStartPts=0;
      if(u4DmxAvailSize<u4DataLen)
      {
          x_thread_delay(50);
          return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
      }
    }

    rDmxMMData.u4BufStart = u4SBufStart;
	rDmxMMData.u4BufEnd = u4BufEnd;
	rDmxMMData.u1Idx=u1Pid;
#if 0
    if(prSwdmxFlvInfo->u4VDecType==ENUM_SWDMX_VDEC_VP6 && u4DatType!=eSWDMX_MEDIA_AUDIO)
    {
      DMX_MM_T rDMXInfo;
      x_memset(&rDMXInfo, 0, sizeof(DMX_MM_T));
      DMX_MM_Get(u4DatType,DMX_MM_FLAG_BUF,&rDMXInfo);
      if(rDMXInfo.u4Wp+u4DataLen+SWDMX_DMA_ALIGN>=rDMXInfo.u4BufAddr+rDMXInfo.u4BufSize)
      {
        rDmxMMData.u4BufStart = u4SBufStart;
        rDmxMMData.u4BufEnd = u4BufEnd;
        rDmxMMData.u4StartAddr = u4DataAddr;
        rDmxMMData.u4FrameSize = u4DataLen+SWDMX_DMA_ALIGN;
        rDmxMMData.u4Pts=SWDMX_FLV_PTS_INFOR_DROP;
        rDmxMMData.u1Idx=u1Pid;
        LOG(5, "FLV Send data,Swap dmx WP to start\n");

        if(_SWDMX_MoveData(prSwdmxFlvInfo->u1SrcId, &rDmxMMData) != TRUE)
        //if(DMX_MM_MoveData(u4DatType, &rDmxMMData, 500) != TRUE)
        {
            LOG(0, "Demuxer fails to move data.\n");
            return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
        }
        else
        {
            prSwdmxFlvInfo->i4DebugerCounter++;
        }
      }
    }
#endif
    if (u4DatType==eSWDMX_MEDIA_AUDIO)
    {
        LOG(9, "[A] 0x%x\n", u4Pts);
    }
    else if (u4DatType==eSWDMX_MEDIA_VIDEO)
    {
        LOG(9, "[V] 0x%x\n", u4Pts);
    }

    if(prSwdmxFlvInfo->u4VDecType==ENUM_SWDMX_VDEC_H264
		&& u4DatType==eSWDMX_MEDIA_VIDEO)
    {
	  return _SWDMX_DmxTransformAvcData(prSwdmxFlvInfo,&rDmxMMData,u4DataAddr,u4DataLen);
    }
	else
	{
		rDmxMMData.u4StartAddr = u4DataAddr;
		rDmxMMData.u4FrameSize = u4DataLen;
		rDmxMMData.u4Pts=u4Pts;		
		if(_SWDMX_MoveData(prSwdmxFlvInfo->u1SrcId, &rDmxMMData) != TRUE)
		//if(DMX_MM_MoveData(u4DatType, &rDmxMMData, 500) != TRUE)
		{
			LOG(0, "Demuxer fails to move data.\n");
			return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
		}
		else
		{
			prSwdmxFlvInfo->i4DebugerCounter++;
			return eSWDMX_HDR_PRS_SUCCEED;
		}
	}
}


static INT32 _SWDMX_FlvParseAacConfigTable(UCHAR *puConfigPos,SWDMX_FLV_INFO_T * prSwdmxFlvInfo)
{

  UCHAR *pucPos=NULL,*pFeederStart=NULL,*pFeederEnd=NULL;
  UINT32 u4Freq=0;
  UINT8 uData=0,uBitShift=0;
  pFeederStart = (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->prFeederInfo->u4StartAddr));
  pFeederEnd =   (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->prFeederInfo->u4EndAddr));
  pucPos=  (UCHAR *)(VIRTUAL(((UINT32)puConfigPos)));
  uBitShift=5;
  FLV_INTEGER_BIT(uData,pucPos,uBitShift,4,pFeederStart,pFeederEnd);
  if(uData==0xf)
  {
      FLV_INTEGER_BIT(u4Freq,pucPos,uBitShift,24,pFeederStart,pFeederEnd);
  }
  else
  {
     if(uData<SWDMX_FLV_AAC_DEF_FREQ_NUM)
     {
         u4Freq=_rAacFreqTbl[uData];
     }
     else
     {
        u4Freq=44100;
     }
  }

  if(u4Freq!=0)
  {
     prSwdmxFlvInfo->u4AudSampleRate=u4Freq;
  }
  else if(prSwdmxFlvInfo->u4AudSampleRate==0)
  {
     prSwdmxFlvInfo->u4AudSampleRate=441000;
  }

  FLV_INTEGER_BIT(uData,pucPos,uBitShift,4,pFeederStart,pFeederEnd);
  if(uData>0 && uData<=SWDMX_FLV_AAC_CHANNEL_CFG_NUM)
  {
     prSwdmxFlvInfo->uAudChannelNs=_rAacChCfgTbl[uData-1];
  }
  else
  {
     prSwdmxFlvInfo->uAudChannelNs=2;
  }
  prSwdmxFlvInfo->fgGetAacHeader=TRUE;
  return 0;
}


static SWDMX_HDR_PRS_STATE_T _SWDMX_FlvSendAacHeader(SWDMX_FLV_INFO_T *prSwdmxFlvInfo)
{
    SWDMX_INFO_T* prSwdmxInst = (SWDMX_INFO_T* )prSwdmxFlvInfo->u4SwdmxInst;
    DMX_MM_DATA_T rDmxMMData;
    UINT32 u4DmxAvailSize,u4AudRawDataSize=0;
    UINT8 u1SampleRateIdx = 3;

    if(!prSwdmxFlvInfo->fgEnAudio|| prSwdmxFlvInfo->u4LastPts<prSwdmxFlvInfo->u4AudStartPts)
    {
      return eSWDMX_HDR_PRS_SUCCEED;
    }
    //u4DmxAvailSize=DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_AUDIO);
    u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId,
                                    (DMX_PID_TYPE_T)eSWDMX_MEDIA_AUDIO,
                                    prSwdmxFlvInfo->u1AudPid);
    u4AudRawDataSize=prSwdmxFlvInfo->u4LastDataLen-2+SWDMX_FLV_AAC_FAKE_HDR_SIZE;
    if(u4DmxAvailSize<7)
    {
        x_thread_delay(50);
		return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
    }
    x_memset(&rDmxMMData,0,sizeof(rDmxMMData));

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
    switch (prSwdmxFlvInfo->u4AudSampleRate)
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

    prSwdmxInst->pucAudBuf[2] = (1 << 6) | ((u1SampleRateIdx<< 2) & 0x3C) | ((prSwdmxFlvInfo->uAudChannelNs>> 2) & 0x1);
    /* 2b: channel_configuration */
    /* 1b: original */
    /* 1b: home */
    /* 1b: copyright_id */
    /* 1b: copyright_id_start */
    /* 2b: aac_frame_length */
    prSwdmxInst->pucAudBuf[3] = ((prSwdmxFlvInfo->uAudChannelNs & 0x3) << 6) | ((u4AudRawDataSize >> 11) & 0x3);
    /* 8b: aac_frame_length */
    prSwdmxInst->pucAudBuf[4] = ((u4AudRawDataSize>> 3) & 0xFF);
    /* 3b: aac_frame_length */
    /* 5b: adts_buffer_fullness */
    prSwdmxInst->pucAudBuf[5] = ((u4AudRawDataSize << 5) & 0xE0) |((0x7FF >> 6) & 0x1F);
    /* 6b: adts_buffer_fullness */
    /* 2b: number_of_raw_data_blocks_in_frame */
    prSwdmxInst->pucAudBuf[6] = ((0x7FF << 2) & 0xFC);
    rDmxMMData.u4BufStart = (UINT32)prSwdmxInst->pucAudBuf;
    rDmxMMData.u4BufEnd = rDmxMMData.u4BufStart + AAC_ALLOC_SIZE;
    rDmxMMData.u4StartAddr = rDmxMMData.u4BufStart;
    rDmxMMData.u4FrameSize = SWDMX_FLV_AAC_FAKE_HDR_SIZE;
    rDmxMMData.u4Pts = prSwdmxFlvInfo->u4LastPts;
    rDmxMMData.u1Idx = prSwdmxFlvInfo->u1AudPid;

    //if(DMX_MM_MoveData(eSWDMX_MEDIA_AUDIO, &rDmxMMData, 500) != TRUE)
    if(_SWDMX_MoveData(prSwdmxFlvInfo->u1SrcId, &rDmxMMData) != TRUE)
    {
       LOG(0, "Demuxer fails to move data.\n");
       return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
    }

    return eSWDMX_HDR_PRS_SUCCEED;
}



#if 0
static BOOL _SWDMX_FlvValidHdrBps(UINT32 u4HdrBps, UINT32 u4DspBps)
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

#endif

BOOL _SWDMX_FlvAudioCb(UINT8 u1SrcId,ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo;
    
    SWDMX_FLV_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxFlvInfo= &prSwdmxInst->rFmtInfo.rSwdmxFlvInfo;
    
    if(eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE)
    {
        AUD_DEC_INFO_T rADecInfo;
        
        x_memset(&rADecInfo, 0, sizeof(AUD_DEC_INFO_T));
        AUD_DspGetMpegAudInfo(prSwdmxInst->u1AdecId, &rADecInfo);
        //AUD_DspSetMp3PreparsingMode(TRUE);

        LOG(5, "SWDMX_FlvAudioCb Audio is ready\n");
        if (prSwdmxFlvInfo->fgEnVideo &&prSwdmxFlvInfo->prRangeList->prDmxRangeInfo->ui8_vid_duration >prSwdmxFlvInfo->u4TriggerAudPts)
        {
            if ((prSwdmxFlvInfo->fgBadAudio) &&(rADecInfo.e_aud_fmt == AUD_FMT_MP3))
            {
                UINT32 u4DspPts;
                UINT32 u4PrsPos;

                LOG(5, "Bad Audio, Lookup audio pts from audio driver\n");
                AUD_GetDspPtsInfo(prSwdmxInst->u1AdecId, &u4DspPts, &u4PrsPos);
                prSwdmxFlvInfo->u4TriggerAudPts = u4DspPts;
                AUD_SetStartPts(prSwdmxInst->u1AdecId, u4DspPts);
                prSwdmxFlvInfo->fgBadAudio = FALSE;
            }

            LOG(5, "Triggerred by video\n");
            VDP_TriggerAudReceive(prSwdmxFlvInfo->u4TriggerAudPts);
            LOG(5, "SWDMX_FlvAudioCb VDP Trigger PTS = 0x%08x\n", prSwdmxFlvInfo->u4TriggerAudPts);
            if(prSwdmxFlvInfo->fgVDecIsPause)
            {
                _SWDMX_FlvVideoWakeUp(prSwdmxFlvInfo);
            }
        }
        else
        {
            // Audio Only case
            LOG(5, "Trigger audio by _SWDMX_FlvAudioCb\n");
            //VDP_TriggerAudReceive(prSwdmxFlvInfo->u4TriggerAudPts);
            AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
            STC_StartStc(prSwdmxInst->u1StcId);
        }
        prSwdmxFlvInfo->fgAudIsReady = TRUE;
        prSwdmxFlvInfo->fgAudIsLoaded = TRUE;
    }
    else
    {
        // impossible
        LOG(8, "_SWDMX_FlvAudioCb eAudioNotifyType != ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE\n");
    }
    return TRUE;
}

#if 1
BOOL _SWDMX_FlvVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT8 uByte=0,uPicType=0,uCodecType=0,uSend=0;
    UINT32 u4StartCode=0x00000001,u4NalLen=0;
    UCHAR *puReadPtr=NULL,*puBufStart=NULL,*puBufEnd=NULL;
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo =NULL;
    SWDMX_INFO_T* prSwdmxInst =0;
	
    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
    if(!prPes->pvInstanceTag)
    {
      LOG(0,"Video CallBack prPes->pvInstanceTag==NULL\n");
      return TRUE;
    }
    else
    {
        prSwdmxInst = (SWDMX_INFO_T*)prPes->pvInstanceTag;
        if(prSwdmxInst)
        {
            prSwdmxFlvInfo=(SWDMX_FLV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxFlvInfo;
        }
    }
    if(!prSwdmxFlvInfo)
    {
      LOG(0,"Video CallBack prSwdmxFlvInfo==NULL\n");
      return FALSE;
    }

    rPesInfo.ucEsId=prSwdmxFlvInfo->u1EsId;
    rPesInfo.ucMpvId=0;
    rPesInfo.u4PTS = prPes->u4Pts;
    rPesInfo.u4DTS = prPes->u4Dts;
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.fgDtsValid = TRUE;
    rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo.fgGop = prPes->fgGop;
    rPesInfo.fgEos = prPes->fgEOS;
    rPesInfo.u8Offset=prSwdmxFlvInfo->u4ValidSeekPos;
    rPesInfo.u8OffsetI=prSwdmxFlvInfo->u4LastKeyPos;
    rPesInfo.rExtra.fgValid=TRUE;
    rPesInfo.rExtra.u8PTSI=prSwdmxFlvInfo->u4LastKeyPts;
    rPesInfo.rExtra.u8AudPTS=prSwdmxFlvInfo->u4AudPts;
    if (rPesInfo.fgEos)
    {
        rPesInfo.u4FifoStart = (UINT32)NULL;
        rPesInfo.u4FifoEnd = (UINT32)NULL;
        rPesInfo.u4VldReadPtr = (UINT32)NULL;
		rPesInfo.fgBackward=(prSwdmxFlvInfo->i4PrevSpeed<0) ? TRUE : FALSE;
    }
    else if(rPesInfo.u4PTS==SWDMX_FLV_PTS_INFOR_PARTION&&prSwdmxFlvInfo->u4FrameAddrToVdec==0)
    {
        prSwdmxFlvInfo->u4FrameAddrToVdec=prPes->u4FrameAddr;
        return TRUE;
    }
    else if(rPesInfo.u4PTS==SWDMX_FLV_PTS_INFOR_PARTION)
    {
        return TRUE;
    }
	else if(rPesInfo.u4PTS==SWDMX_FLV_PTS_INFOR_STARTCODE)
	{
        return TRUE;
	}

    if(prSwdmxFlvInfo->u4FrameAddrToVdec)
    {
       rPesInfo.u4VldReadPtr=prSwdmxFlvInfo->u4FrameAddrToVdec;
       prSwdmxFlvInfo->u4FrameAddrToVdec=0;
    }
    else
    {
        rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    }

    rPesInfo.u4DTS=rPesInfo.u4PTS;
	u4NalLen=prPes->u4Dts; // use u4Dts field to transform NAL Len.
    if (!rPesInfo.fgEos)
    {
       puBufStart=(UCHAR *)VIRTUAL(rPesInfo.u4FifoStart);
       puBufEnd=(UCHAR *)VIRTUAL(rPesInfo.u4FifoEnd);
       puReadPtr=(UCHAR *)VIRTUAL(rPesInfo.u4VldReadPtr);
	   if(prSwdmxFlvInfo->u4VDecType==ENUM_SWDMX_VDEC_H264)
	   {
          if(rPesInfo.u4PTS==SWDMX_FLV_PTS_INFOR_SEQHEADER)
          {
		      rPesInfo.ucPicType=*puReadPtr;
		      FLV_RPOS_INC(puReadPtr,1,puBufStart,puBufEnd);
			  rPesInfo.u4VldReadPtr=(UINT32)PHYSICAL(((UINT32)puReadPtr));
			  rPesInfo.fgSeqHdr=TRUE;
			  rPesInfo.u4PTS=0;
		      FLV_RPOS_INC(puReadPtr,(u4NalLen-1),puBufStart,puBufEnd);
			  uSend=1;
          }
		  else // NAL
		  {
              UINT32 u4CurType;
			  u4CurType=*puReadPtr;
			  FLV_RPOS_INC(puReadPtr,1,puBufStart,puBufEnd);
			  if(prSwdmxFlvInfo->uLastNalType==SWDMX_FLV_INVALID_NALTYPE || (((u4CurType&0x1f)!=1)&&((u4CurType&0x1f)!=5)) || (((*puReadPtr)&0x80)!=0))
			  {
				  if(prSwdmxFlvInfo->uLastNalType!=SWDMX_FLV_INVALID_NALTYPE)
				  {
					rPesInfo.ucPicType=prSwdmxFlvInfo->uLastNalType;
					rPesInfo.u4VldReadPtr=prSwdmxFlvInfo->u4LastNalPtr;
					rPesInfo.u4PTS=prSwdmxFlvInfo->u4LastNalPts;
					uSend=1;
				  }
				  else
				  {
					uSend=0;
				  }
				  
				  prSwdmxFlvInfo->uLastNalType=u4CurType;
				  prSwdmxFlvInfo->u4LastNalPts=prPes->u4Pts;
				  prSwdmxFlvInfo->u4LastNalPtr=(UINT32)PHYSICAL(((UINT32)puReadPtr));
			  }
			  
			  FLV_RPOS_INC(puReadPtr,(u4NalLen-1),puBufStart,puBufEnd);
		  }
		  
		  FLV_SET_INTEGER(u4StartCode,puReadPtr,SWDMX_FLV_STARTCODE_LEN,puBufStart,puBufEnd);//fill start code
		  HalFlushInvalidateDCacheMultipleLine((UINT32)puReadPtr-8,32);
		  if(puReadPtr>puBufEnd)
		  {
			HalFlushInvalidateDCacheMultipleLine((UINT32)puBufStart,32);
		  } 
		  
		  if(uSend)
		  {
			 VDEC_SendEs((void*)&rPesInfo);
		  }
	   }
	   else
	   {
		   uByte= *puReadPtr;
		   prSwdmxFlvInfo->u2TagIdentify=(uByte&0xf)|(prSwdmxFlvInfo->u2TagIdentify&0xff00);
		   uPicType=(uByte&0xf0)>>4;
		   uCodecType=uByte&0xf;
		   rPesInfo.fgSeqHdr=FALSE;
		   FLV_RPOS_INC(puReadPtr,1,puBufStart,puBufEnd);
           if(uPicType==1)
           {
              rPesInfo.ucPicType = I_TYPE;
           }
           else if(uPicType==2)
           {
              rPesInfo.ucPicType = P_TYPE;
           }
           else if(uPicType==3)
           {
              rPesInfo.ucPicType = I_TYPE;
           }
           else
           {
              LOG(1, "_SWDMX_FlvVidCb UnknowPicType:%d\n",uByte&0xf0);
              rPesInfo.ucPicType = UNKNOWN_TYPE;
           }

            if(prSwdmxFlvInfo->u4VDecType==ENUM_SWDMX_VDEC_VP6)
            {
               //prSwdmxFlvInfo->uVp6Adjustment=*puReadPtr;
               if(rPesInfo.u4PTS==SWDMX_FLV_PTS_INFOR_DROP)
               {
                   rPesInfo.fgDtsValid = FALSE;
               }

               if(uCodecType==SWDMX_FLV_CODEC_VP6)
               {
                  FLV_RPOS_INC(puReadPtr,1,puBufStart,puBufEnd);
                  //do nothing
               }
               else if(uCodecType==SWDMX_FLV_CODEC_VP6A)
               {
                  UINT32 u4Temp;
				  // skip dummy
				  FLV_RPOS_INC(puReadPtr,1,puBufStart,puBufEnd);
				  // get alpha offset
				  FLV_INTEGER(u4Temp,puReadPtr,3,puBufStart,puBufEnd);
				  // send YCbCr
				  rPesInfo.u1AlphaFlag = VP6_ALPHA_ENABLE;
				  rPesInfo.u4VldReadPtr=(UINT32)PHYSICAL(((UINT32)puReadPtr));
				  VDEC_SendEs((void*)&rPesInfo);
				  // advance to alpha
				  rPesInfo.u1AlphaFlag = VP6_ALPHA_ENABLE | VP6_ALPHA_FRAME;
				  FLV_RPOS_INC(puReadPtr,u4Temp,puBufStart,puBufEnd);
               }
               else
               {
                 LOG(3,"Error Vpt codec type\n");
                 return TRUE;
               }
            }

           rPesInfo.u4VldReadPtr=(UINT32)PHYSICAL(((UINT32)puReadPtr));
           VDEC_SendEs((void*)&rPesInfo);
	   }
    }
    else
    {
       VDEC_SendEs((void*)&rPesInfo);
    }
    return TRUE;
}

#else
static UCHAR * _SWDMX_FlvSendH264SeqHeader(VDEC_PES_INFO_T *prPesInfo,SWDMX_FLV_INFO_T *prSwdmxFlvInfo)
{
   UCHAR *pucPos=NULL,*puStart,*puEnd;
   UINT16 u2PSLength = 0;
   UINT8 u1NumOfPS=0,u1PSNum=0,u1NalLenLen=0;
   UINT8  i=0;
   if(!prPesInfo)
   {
       return NULL;
   }

   //x_thread_delay(200);
   {
       puStart=(UCHAR *)VIRTUAL(prPesInfo->u4FifoStart);
       puEnd=(UCHAR *)VIRTUAL(prPesInfo->u4FifoEnd);
       pucPos=(UCHAR *)VIRTUAL(prPesInfo->u4VldReadPtr);
   }

   FLV_RPOS_INC(pucPos,4,puStart,puEnd);
   FLV_INTEGER(u1NalLenLen,pucPos,1,puStart,puEnd);
   prSwdmxFlvInfo->uAvcNalLenBytes=(u1NalLenLen&0X3)+1;
   for(u1PSNum = 0; u1PSNum < 2; u1PSNum++)    // 0 for SPS, 1 for PPS
   {
      FLV_INTEGER(u1NumOfPS,pucPos,1,puStart,puEnd);
      u1NumOfPS= (u1PSNum==0) ? (u1NumOfPS&0x1f) : u1NumOfPS;
      for(i=0;i<u1NumOfPS;i++)
      {
        FLV_INTEGER(u2PSLength,pucPos,2,puStart,puEnd);
        prPesInfo->ucPicType=*pucPos;
        FLV_RPOS_INC(pucPos,1,puStart,puEnd);
        prPesInfo->u4VldReadPtr=(UINT32)PHYSICAL(((UINT32)pucPos));
        VDEC_SendEs((void*)prPesInfo);
        FLV_RPOS_INC(pucPos,u2PSLength-1,puStart,puEnd);
      }
   }
   return pucPos;
}

BOOL _SWDMX_FlvVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT8 uByte=0,uPicType=0,uCodecType=0,uSend=0;
    UINT32 u4StartCode=0x00000001,u4Temp,u4NALLen=0,u4TotalNalLen=0,u4LastPtr=0;
    INT32 i4PtsOffset=0;
    UCHAR *puReadPtr=NULL,*puStartCode=NULL,*puBufStart=NULL,*puBufEnd=NULL,ucLastType=0xff,u4CurType=0xff;
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo =NULL;
    SWDMX_INFO_T* prSwdmxInst =0;
    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
    if(!prPes->pvInstanceTag)
    {
      LOG(0,"Video CallBack prPes->pvInstanceTag==NULL\n");
      return TRUE;
    }
    else
    {
        prSwdmxInst = (SWDMX_INFO_T*)prPes->pvInstanceTag;
        if(prSwdmxInst)
        {
            prSwdmxFlvInfo=(SWDMX_FLV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxFlvInfo;
        }
    }
    if(!prSwdmxFlvInfo)
    {
      LOG(0,"Video CallBack prSwdmxFlvInfo==NULL\n");
      return FALSE;
    }

    rPesInfo.ucEsId=prSwdmxFlvInfo->u1EsId;
    rPesInfo.ucMpvId=0;
    rPesInfo.u4PTS = prPes->u4Pts;
    rPesInfo.u4DTS = prPes->u4Dts;
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.fgDtsValid = TRUE;
    rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo.fgGop = prPes->fgGop;
    rPesInfo.fgEos = prPes->fgEOS;
    rPesInfo.u8Offset=prSwdmxFlvInfo->u4ValidSeekPos;
    rPesInfo.u8OffsetI=prSwdmxFlvInfo->u4LastKeyPos;
    rPesInfo.rExtra.fgValid=TRUE;
    rPesInfo.rExtra.u8PTSI=prSwdmxFlvInfo->u4LastKeyPts;
    rPesInfo.rExtra.u8AudPTS=prSwdmxFlvInfo->u4AudPts;
    if (rPesInfo.fgEos)
    {
        rPesInfo.u4FifoStart = NULL;
        rPesInfo.u4FifoEnd = NULL;
        rPesInfo.u4VldReadPtr = NULL;
		rPesInfo.fgBackward=(prSwdmxFlvInfo->i4PrevSpeed<0) ? TRUE : FALSE;
    }
    else if(rPesInfo.u4PTS==SWDMX_FLV_PTS_INFOR_PARTION&&prSwdmxFlvInfo->u4FrameAddrToVdec==0)
    {
        prSwdmxFlvInfo->u4FrameAddrToVdec=prPes->u4FrameAddr;
        return TRUE;
    }
    else if(rPesInfo.u4PTS==SWDMX_FLV_PTS_INFOR_PARTION)
    {
        return TRUE;
    }

    if(prSwdmxFlvInfo->u4FrameAddrToVdec)
    {
       rPesInfo.u4VldReadPtr=prSwdmxFlvInfo->u4FrameAddrToVdec;
       prSwdmxFlvInfo->u4FrameAddrToVdec=0;
    }
    else
    {
        rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    }

    rPesInfo.u4DTS=rPesInfo.u4PTS;
    if (!rPesInfo.fgEos)
    {
       puBufStart=(UCHAR *)VIRTUAL(rPesInfo.u4FifoStart);
       puBufEnd=(UCHAR *)VIRTUAL(rPesInfo.u4FifoEnd);
       puReadPtr=(UCHAR *)VIRTUAL(rPesInfo.u4VldReadPtr);
       puStartCode=puReadPtr;
       uByte= *puReadPtr;
       prSwdmxFlvInfo->u2TagIdentify=(uByte&0xf)|(prSwdmxFlvInfo->u2TagIdentify&0xff00);
       uPicType=(uByte&0xf0)>>4;
       uCodecType=uByte&0xf;
       FLV_RPOS_INC(puReadPtr,1,puBufStart,puBufEnd);

       if(prSwdmxFlvInfo->u4VDecType==ENUM_SWDMX_VDEC_H264)
       {
          uByte= *puReadPtr;
          FLV_RPOS_INC(puReadPtr,1,puBufStart,puBufEnd);
          FLV_INTEGER(u4Temp,puReadPtr,3,puBufStart,puBufEnd);
          u4StartCode=0x00000001;
          FLV_SET_INTEGER(u4StartCode,puStartCode,4,puBufStart,puBufEnd);//fill start code
          if(uByte==0)
          {
             LOG(1, "_SWDMX_FlvVidCb Get H264 Sequence Header\n");

             rPesInfo.fgSeqHdr=TRUE;
             rPesInfo.u4VldReadPtr=(UINT32)puReadPtr;
             //FLV_RPOS_INC(rPesInfo.u4VldReadPtr,5,rPesInfo.u4FifoStart,rPesInfo.u4FifoEnd);
             _SWDMX_FlvSendH264SeqHeader(&rPesInfo,prSwdmxFlvInfo);
             return TRUE;
          }
          else if(uByte==2)
          {
             rPesInfo.fgSeqEnd=TRUE;
          }
          else if(uByte==1)
          {
             i4PtsOffset=(u4Temp+0xff800000)^0xff800000;
             if(i4PtsOffset>0)
             {
               rPesInfo.u4PTS=rPesInfo.u4DTS+i4PtsOffset*90;
             }

             u4TotalNalLen=5;
             while(u4TotalNalLen<prSwdmxFlvInfo->u4LastDataLen)
             {
                 puStartCode=puReadPtr;
                 FLV_INTEGER(u4NALLen,puReadPtr,prSwdmxFlvInfo->uAvcNalLenBytes,puBufStart,puBufEnd);
                 FLV_SET_INTEGER(u4StartCode,puStartCode,prSwdmxFlvInfo->uAvcNalLenBytes,puBufStart,puBufEnd);//fill start code
                 u4TotalNalLen+=prSwdmxFlvInfo->uAvcNalLenBytes;
				 u4CurType=*puReadPtr;
				 FLV_RPOS_INC(puReadPtr,1,puBufStart,puBufEnd);
                 if(ucLastType==0xff || (((u4CurType&0x1f)!=1)&&((u4CurType&0x1f)!=5)) || (((*puReadPtr)&0x80)!=0))
                 {
                     if(ucLastType!=0xff)
                     {
                       rPesInfo.ucPicType=ucLastType;
                       rPesInfo.u4VldReadPtr=u4LastPtr;
                       ucLastType=u4CurType;
                       uSend=1;
                     }
                     else
                     {
                       ucLastType=u4CurType;
                       rPesInfo.ucPicType=ucLastType;
                       uSend=0;
                     }
                     u4LastPtr=(UINT32)PHYSICAL(((UINT32)puReadPtr));
                 }
                 
                 u4TotalNalLen+=u4NALLen;
                 if(u4TotalNalLen==prSwdmxFlvInfo->u4LastDataLen)
                 {
                    rPesInfo.ucPicType=ucLastType;
                    rPesInfo.u4VldReadPtr=u4LastPtr;
                    uSend=1;
                 }
                 else if(u4TotalNalLen > prSwdmxFlvInfo->u4LastDataLen)
                 {
                    LOG(0,"Send NAL error\n");
                    return FALSE;
                 }

                 #if defined(CC_MT5365) || defined(CC_MT5395) || defined(SWDMX_IS_POST_MT5368)
                 HalFlushInvalidateDCacheMultipleLine((UINT32)puStartCode-8,32);
                 if(puStartCode>puBufEnd)
                 {
                    HalFlushInvalidateDCacheMultipleLine((UINT32)puBufStart,32);
                 }
                 #else
                 HalFlushInvalidateDCache();
                 #endif
                 
                 if(uSend)
                 {
                     rPesInfo.fgDtsValid = TRUE;
                     VDEC_SendEs((void*)&rPesInfo);
                     uSend=0;
                 }                 
                 FLV_RPOS_INC(puReadPtr,(u4NALLen-1),puBufStart,puBufEnd);
             }
          }

       }
       else  // Sorenson/mp4/vp6
       {
           if(uPicType==1)
           {
              rPesInfo.ucPicType = I_TYPE;
           }
           else if(uPicType==2)
           {
              rPesInfo.ucPicType = P_TYPE;
           }
           else if(uPicType==3)
           {
              rPesInfo.ucPicType = I_TYPE;
           }
           else
           {
              LOG(1, "_SWDMX_FlvVidCb UnknowPicType:%d\n",uByte&0xf0);
              rPesInfo.ucPicType = UNKNOWN_TYPE;
           }

            if(prSwdmxFlvInfo->u4VDecType==ENUM_SWDMX_VDEC_VP6)
            {
               //prSwdmxFlvInfo->uVp6Adjustment=*puReadPtr;
               if(rPesInfo.u4PTS==SWDMX_FLV_PTS_INFOR_DROP)
               {
                   rPesInfo.fgDtsValid = FALSE;
               }

               if(uCodecType==SWDMX_FLV_CODEC_VP6)
               {
                  FLV_RPOS_INC(puReadPtr,1,puBufStart,puBufEnd);
                  //do nothing
               }
               else if(uCodecType==SWDMX_FLV_CODEC_VP6A)
               {
#ifdef FLV_ALPHA_SUPPORT
                    // skip dummy
                    FLV_RPOS_INC(puReadPtr,1,puBufStart,puBufEnd);

                    // get alpha offset
                    FLV_INTEGER(u4Temp,puReadPtr,3,puBufStart,puBufEnd);

                    // send YCbCr
                    rPesInfo.u1AlphaFlag = VP6_ALPHA_ENABLE;
                    rPesInfo.u4VldReadPtr=(UINT32)PHYSICAL(((UINT32)puReadPtr));
                    VDEC_SendEs((void*)&rPesInfo);

                    // advance to alpha
                    rPesInfo.u1AlphaFlag = VP6_ALPHA_ENABLE | VP6_ALPHA_FRAME;
                    FLV_RPOS_INC(puReadPtr,u4Temp,puBufStart,puBufEnd);
#else
                  FLV_RPOS_INC(puReadPtr,4,puBufStart,puBufEnd); // alpha data offset
#endif
               }
               else
               {
                 LOG(3,"Error Vpt codec type\n");
                 return TRUE;
               }
            }

           rPesInfo.u4VldReadPtr=(UINT32)PHYSICAL(((UINT32)puReadPtr));
           VDEC_SendEs((void*)&rPesInfo);
       }

    }
    else
    {
       VDEC_SendEs((void*)&rPesInfo);
    }

    return TRUE;
}

#endif



static SWDMX_HDR_PRS_STATE_T _SWDMX_ErrRecovery(SWDMX_FLV_INFO_T* prSwdmxFlvInfo, UCHAR* ErrorData,UINT32 u4DataLen,UINT32 *u4RecoveryOffset)
{
    UCHAR *pucPos=NULL,*pFeederStart=NULL,*pFeederEnd=NULL,*pIdentifyPos=NULL;
    UINT32 u4AudIdentify,u4VidIdentify,u4MetaIdentify=0,u4MovieByte=0,u4FramePos=0,u4Identify=0,u4Pts=0;
    UINT8  uIdentifyOffset=4;
    BOOL fgFindFrame=FALSE;
    pFeederStart = (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->prFeederInfo->u4StartAddr));
    pFeederEnd =   (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->prFeederInfo->u4EndAddr));
    u4AudIdentify=((prSwdmxFlvInfo->u2TagIdentify&0xff00)>>8);
    u4VidIdentify=prSwdmxFlvInfo->u2TagIdentify&0xff;
    u4MetaIdentify=0x02;
    if(u4AudIdentify==0 || u4VidIdentify==0)
    {
       uIdentifyOffset=3;
       u4MetaIdentify=0;
       u4AudIdentify=0;
       u4VidIdentify=0;
    }

    if(u4DataLen==0)
    {
       return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
    }

    if(!prSwdmxFlvInfo->fgErrorConvery)
    {
      LOG(2,"SWDMX_FLV:Start Error Recovery\n");
      prSwdmxFlvInfo->PRecoverPos=ErrorData;
      prSwdmxFlvInfo->fgErrorConvery=TRUE;
      prSwdmxFlvInfo->fgAacCfgBroken=FALSE;
      prSwdmxFlvInfo->fgTagHeaderBroken=FALSE;
      prSwdmxFlvInfo->fgTagDataBroken=FALSE;
      prSwdmxFlvInfo->fgTagHeaderParsed=FALSE;
      prSwdmxFlvInfo->u4SkipDatLen=0;
      prSwdmxFlvInfo->u4ErrorLen=0;
      prSwdmxFlvInfo->u2FindCounter=0;
    }

    pIdentifyPos=pucPos=prSwdmxFlvInfo->PRecoverPos;
    if(prSwdmxFlvInfo->u4ErrorLen<FLV_ERROR_RECOVERY_LEN)
    {
        while(1)
        {
          if((u4DataLen-u4MovieByte)<SWDMX_FLV_TAG_HEADER_SIZE+1)
          {
             if(u4FramePos)
             {
               u4MovieByte=u4FramePos;
               break;
             }
             else
             {
                 goto DataNotEnought;
             }
          }

          if(prSwdmxFlvInfo->u4ErrorLen==FLV_ERROR_RECOVERY_LEN)
          {
              prSwdmxFlvInfo->u2FindCounter++;
              prSwdmxFlvInfo->u4ErrorLen=0;

              if(!prSwdmxFlvInfo->fgTrickMode)
              {
                if(prSwdmxFlvInfo->u2FindCounter>FLV_MAX_ERRORRCV_TIME)
                {
                   goto ErrorCannotRecorved;
                }
                u4MovieByte+=prSwdmxFlvInfo->u2FindCounter*FLV_ERROR_RECOVERY_LEN;
                u4MovieByte=flv_min(u4MovieByte,u4DataLen);
                if((u4DataLen-u4MovieByte)<SWDMX_FLV_TAG_HEADER_SIZE+1)
                {
                   goto DataNotEnought;
                }
              }
              else
              {
                  if(u4FramePos)
                  {
                    u4MovieByte= u4FramePos;
                    break;
                  }
                  else
                  {
                     if(prSwdmxFlvInfo->u2FindCounter>FLV_MAX_ERRORRCV_TIME)
                     {
                        goto ErrorCannotRecorved;
                     }
                     else
                     {
                         goto DataNotEnought;
                     }
                  }
              }


           }

          pucPos=prSwdmxFlvInfo->PRecoverPos;
          FLV_RPOS_INC(pucPos,u4MovieByte,pFeederStart,pFeederEnd);
          pIdentifyPos=pucPos;
          u4Identify=0;
          fgFindFrame=FALSE;

          if(*pucPos==eFLV_TAG_TYPE_AUD || *pucPos==eFLV_TAG_TYPE_VID || *pucPos==eFLV_TAG_TYPE_DATA)
          {
            FLV_RPOS_INC(pIdentifyPos,SWDMX_FLV_ERRCOV_IDENTIFY_OFFSET,pFeederStart,pFeederEnd);
            FLV_INTEGER(u4Identify,pIdentifyPos,uIdentifyOffset,pFeederStart,pFeederEnd);

            if((*pucPos==eFLV_TAG_TYPE_AUD&&u4Identify==u4AudIdentify)
                ||(*pucPos==eFLV_TAG_TYPE_VID&&((u4Identify&0xffffff0f)==u4VidIdentify))
                ||(*pucPos==eFLV_TAG_TYPE_VID && u4Identify==u4MetaIdentify))
            {
                   FLV_INTEGER(prSwdmxFlvInfo->uLastTagType,pucPos,1,pFeederStart,pFeederEnd);
                   FLV_INTEGER(prSwdmxFlvInfo->u4LastDataLen,pucPos,3,pFeederStart,pFeederEnd);
                   prSwdmxFlvInfo->u4LastPts=0;
                   FLV_INTEGER(prSwdmxFlvInfo->u4LastPts,pucPos,3,pFeederStart,pFeederEnd);
                   FLV_INTEGER(u4Pts,pucPos,1,pFeederStart,pFeederEnd);
                   prSwdmxFlvInfo->u4LastPts=prSwdmxFlvInfo->u4LastPts|(u4Pts<<24);
                   prSwdmxFlvInfo->u4LastPts=prSwdmxFlvInfo->u4LastPts*SWDMX_FLV_PTS_FACTOR;
                   prSwdmxFlvInfo->fgErrorConvery=FALSE;
                   prSwdmxFlvInfo->u4VidRepeatFrame=0;
                   prSwdmxFlvInfo->u4ErrorLen=0;
                   prSwdmxFlvInfo->u4SeekTagLen=prSwdmxFlvInfo->u4LastDataLen;
                   prSwdmxFlvInfo->u4RecSeekTime=prSwdmxFlvInfo->u4LastPts;
                   u4FramePos=u4MovieByte;
                   fgFindFrame=TRUE;
            }
          }

          if(fgFindFrame)
          {
             if(prSwdmxFlvInfo->i4PrevSpeed>0)
             {
                 LOG(2,"SWDMX_FLV:Error Recovery: Success!\n");
                 break;
             }
             else
             {
                 if((u4FramePos+prSwdmxFlvInfo->u4LastDataLen+SWDMX_FLV_FILE_HEADER_SIZE+SWDMX_FLV_TAG_ENDSIZE+SWDMX_FLV_ERRCOV_IDENTIFY_OFFSET)>u4DataLen)
                 {
                    LOG(2,"SWDMX_FLV:Error Recovery: Success!\n");
                    u4MovieByte=u4FramePos;
                    break;
                 }
                 else
                 {
                    u4MovieByte+=(prSwdmxFlvInfo->u4LastDataLen+SWDMX_FLV_FILE_HEADER_SIZE);
                    continue;
                 }
             }

          }

          u4MovieByte++;
          prSwdmxFlvInfo->u4ErrorLen++;
        }
    }
/*
    else if(prSwdmxFlvInfo->fgKeyExist && !prSwdmxFlvInfo->fgTrickMode)
    {
       if(_SWDMX_FlvSeekPos(prSwdmxFlvInfo,prSwdmxFlvInfo->u8CurDmxMoviPos+(prSwdmxFlvInfo->u2FindCounter*FLV_ERROR_RECOVERY_LEN),&u8KeyPos,&u4Pts))
       {
         prSwdmxFlvInfo->u8CurDmxMoviPos =u8KeyPos ;
         prSwdmxFlvInfo->u8CurMoviPos = prSwdmxFlvInfo->u8CurDmxMoviPos;
         prSwdmxFlvInfo->u4ValidSeekPos=prSwdmxFlvInfo->u8CurDmxMoviPos;
         prSwdmxFlvInfo->u4RecSeekTime=u4Pts;
         prSwdmxFlvInfo->u4RecSeekAtime=prSwdmxFlvInfo->u4RecSeekTime;
         _SWDMX_FlvResetFeeder(prSwdmxFlvInfo);
         prSwdmxFlvInfo->u4ErrorLen=0;
         prSwdmxFlvInfo->u2FindCounter++;
         prSwdmxFlvInfo->fgErrorConvery=FALSE;
         if(prSwdmxFlvInfo->u2FindCounter>FLV_MAX_ERRORRCV_TIME)
         {
             goto ErrorCannotRecorved;
         }
         LOG(2,"SWDMX_FLV:Error Recovery: Try NextIndexTag!\n");
       }
       else
       {
           goto ErrorCannotRecorved;
       }
    }
    else
    {
        goto ErrorCannotRecorved;
    }
*/
    *u4RecoveryOffset=u4MovieByte;
    return eSWDMX_HDR_PRS_SUCCEED;

ErrorCannotRecorved:
    LOG(2,"SWDMX_FLV:Error Recovery: Fail!\n");
    return eSWDMX_HDR_PRS_FAIL;

DataNotEnought:
    *u4RecoveryOffset=u4MovieByte;
    prSwdmxFlvInfo->PRecoverPos=pucPos;
    LOG(2,"SWDMX_FLV:Error Recovery: Continue!\n");
    return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
}


static SWDMX_HDR_PRS_STATE_T _SWDMX_FlvDeliverTagData(
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo,
    UCHAR* pucBitstream,
    SWDMX_RANGE_LIST_T* prRangeList,
    UINT32 u4AvailSize,
    UINT32* pu4ConsumeSize,
    INT32 i4PlaySpeed,
    BOOL fgRangeEnd)
{
    UINT8 uData,uTagType,uRetState=eSWDMX_HDR_PRS_SUCCEED;
    UINT32 u4MoviedByte=0,u4DataParam=0;
    UINT32 u4Pts,u4Index=0,u4DataLen,u4TransLen=0,u4TagEndLen=0;
    UINT32 u4TagDataSzie=0,u4TransPts=0;
    UCHAR *pucPos=pucBitstream,*pFeederStart=NULL,*pFeederEnd=NULL;
    BOOL fgUseHeadBuf=FALSE,fgDoTransData=TRUE;
    SWDMX_INFO_T* prSwdmxInst = (SWDMX_INFO_T* )prSwdmxFlvInfo->u4SwdmxInst;
    pFeederStart = (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->prFeederInfo->u4StartAddr));
    pFeederEnd =   (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->prFeederInfo->u4EndAddr));
    pucBitstream=  (UCHAR *)(VIRTUAL(((UINT32)(pucBitstream))));

    if(prSwdmxFlvInfo->i4DebugerCounter==u4DebutCounter)
    {
      LOG(10,"Stop Here\n");
    }

    if (prSwdmxFlvInfo->u1SrcId >= SWDMX_SOURCE_MAX)
    {
        goto HdrParsFail;
    }
    
    //Skip FileHeader
    if(prSwdmxFlvInfo->u8CurDmxMoviPos==0 && u4AvailSize>=(SWDMX_FLV_FILE_HEADER_SIZE+SWDMX_FLV_TAG_ENDSIZE))
    {
       u4MoviedByte+=5;
       FLV_RPOS_INC(pucPos,u4MoviedByte,pFeederStart,pFeederEnd);
       FLV_INTEGER(u4DataParam,pucPos,4,pFeederStart,pFeederEnd);
       u4MoviedByte+=4;
       u4DataParam-=9;
       prSwdmxFlvInfo->u4SkipDatLen=u4DataParam+SWDMX_FLV_TAG_ENDSIZE;
    }
    else if(prSwdmxFlvInfo->u8CurDmxMoviPos==0)
    {
       u4MoviedByte=0;
       goto DataNotEnough;
    }


    pucPos=pucBitstream;
    if((u4AvailSize-u4MoviedByte) && prSwdmxFlvInfo->u4SkipDatLen)
    {
      u4DataLen=flv_min(prSwdmxFlvInfo->u4SkipDatLen,(u4AvailSize-u4MoviedByte));
      u4MoviedByte+=u4DataLen;
      prSwdmxFlvInfo->u4SkipDatLen-=u4DataLen;
      if(u4DataLen<prSwdmxFlvInfo->u4SkipDatLen)
      {
         u4MoviedByte=u4AvailSize;
         goto DataNotEnough;
      }
    }

    pucPos=pucBitstream;
    if(prSwdmxFlvInfo->fgTagHeaderBroken)
    {
       FLV_RPOS_INC(pucPos,u4MoviedByte,pFeederStart,pFeederEnd);
       u4DataLen=flv_min((SWDMX_FLV_TAG_HEADER_SIZE-prSwdmxFlvInfo->uTagHeaderBrokenIndex),(u4AvailSize-u4MoviedByte));
       for(u4Index=0;u4Index<u4DataLen;u4Index++)
       {
          if((prSwdmxFlvInfo->uTagHeaderBrokenIndex+u4Index)<SWDMX_FLV_TAG_HEADER_SIZE && (prSwdmxFlvInfo->u1EsId<2))
          {
              prSwdmxFlvInfo->aucHeaderBuf[prSwdmxFlvInfo->uTagHeaderBrokenIndex+u4Index]=*pucPos;
          }
          FLV_RPOS_INC(pucPos,1,pFeederStart,pFeederEnd);
       }
       prSwdmxFlvInfo->uTagHeaderBrokenIndex+=u4DataLen;
       if(prSwdmxFlvInfo->uTagHeaderBrokenIndex==SWDMX_FLV_TAG_HEADER_SIZE)
       {
          fgUseHeadBuf=TRUE;
          prSwdmxFlvInfo->fgTagHeaderBroken=FALSE;
          u4MoviedByte+=u4DataLen;
       }
       else
       {
          u4MoviedByte=u4AvailSize;
          goto DataNotEnough;
       }
    }

    //Parse Tag

    pucPos=pucBitstream;
    if(prSwdmxFlvInfo->fgTagDataBroken)
    {
       uTagType=prSwdmxFlvInfo->uLastTagType;
       fgDoTransData=TRUE;
       FLV_RPOS_INC(pucPos,u4MoviedByte,pFeederStart,pFeederEnd);
       u4DataLen=flv_min((prSwdmxFlvInfo->u4LastDataLen-prSwdmxFlvInfo->u4LastDataBrokenNum),(u4AvailSize-u4MoviedByte));

       if(u4DataLen &&(uTagType==eFLV_TAG_TYPE_AUD || uTagType==eFLV_TAG_TYPE_VID))
       {
           if(u4DataLen+prSwdmxFlvInfo->u4LastDataBrokenNum<prSwdmxFlvInfo->u4LastDataLen)
           {
              if(uTagType==eFLV_TAG_TYPE_VID)
              {
                  u4TransPts=SWDMX_FLV_PTS_INFOR_PARTION;
              }
              else
              {
                  u4TransPts=prSwdmxFlvInfo->u4LastPts;
              }
           }
           else
           {
              u4TransPts=prSwdmxFlvInfo->u4LastPts;
           }
           u4TransLen=u4DataLen;

           if(uTagType==eFLV_TAG_TYPE_AUD && prSwdmxFlvInfo->u4LastDataBrokenNum==0)
           {
              FLV_RPOS_INC(pucPos,1,pFeederStart,pFeederEnd);
              u4MoviedByte++;
              u4TransLen--;
              if(u4TransLen==0 &&prSwdmxFlvInfo->u4LastDataLen!=1)
              {
                u4MoviedByte=u4AvailSize;
                prSwdmxFlvInfo->u4LastDataBrokenNum++;
                goto DataNotEnough;
              }
           }


           if(u4TransLen && uTagType==eFLV_TAG_TYPE_AUD && prSwdmxFlvInfo->u4ADecType==ENUM_SWDMX_ADEC_AAC && prSwdmxFlvInfo->u4LastDataBrokenNum==1)
           {
              UINT8 uPackType;
              FLV_INTEGER(uPackType,pucPos,1,pFeederStart,pFeederEnd);
              u4TransLen--;
              u4MoviedByte++;
              if(uPackType==0)
              {
                 if(u4DataLen+prSwdmxFlvInfo->u4LastDataBrokenNum==prSwdmxFlvInfo->u4LastDataLen)
                 {
                    _SWDMX_FlvParseAacConfigTable(pucPos,prSwdmxFlvInfo);
                    prSwdmxFlvInfo->fgAacCfgBroken=FALSE;
                 }
                 else
                 {
                    for(u4Index=0;u4Index<u4TransLen;u4Index++)
                    {
                       FLV_INTEGER(uData,pucPos,1,pFeederStart,pFeederEnd);
                      prSwdmxFlvInfo->pucAacCfg[u4Index]=uData;
                    }
                    prSwdmxFlvInfo->fgAacCfgBroken=TRUE;
                 }
                 fgDoTransData=FALSE;
              }
              else
              {
                 if(prSwdmxFlvInfo->fgAacSendHeader==FALSE)
                 {
				     uRetState=_SWDMX_FlvSendAacHeader(prSwdmxFlvInfo);
	                 if(uRetState==eSWDMX_HDR_PRS_DMX_MOVE_FAIL)
	                 {
	                   LOG(0, "Demuxer fails to move data.\n");
	                   goto DmxMovFail;
	                 }
	                 else if(uRetState==eSWDMX_HDR_DMX_BUF_NOT_ENOUGH)
	                 {
	                   LOG(7, "Demuxer audio Buffer full,When send aac header.\n");
	                   u4MoviedByte--;
	                   goto DmxBufNotEnough;
	                 }

					 prSwdmxFlvInfo->fgAacSendHeader=TRUE;
                  }
              }

              if(u4TransLen==0 && prSwdmxFlvInfo->u4LastDataLen!=2)
              {
                 LOG(3,"SWDMX_FLV:Parse End,Need Header Data\n");
                 prSwdmxFlvInfo->fgTagDataBroken=TRUE;
                 prSwdmxFlvInfo->u4LastDataBrokenNum=2;
                 u4MoviedByte=u4AvailSize;
                 goto DataNotEnough;
              }
           }
           else if(prSwdmxFlvInfo->fgAacCfgBroken)
           {
               for(u4Index=0;u4Index<u4TransLen;u4Index++)
               {
                  FLV_INTEGER(uData,pucPos,1,pFeederStart,pFeederEnd);
                  prSwdmxFlvInfo->pucAacCfg[prSwdmxFlvInfo->u4LastDataBrokenNum-2+u4Index]=uData;
               }
               fgDoTransData=FALSE;
           }

           if(fgDoTransData)
           {
               uRetState=_SWDMX_DmxTransformData(prSwdmxFlvInfo,uTagType,(UINT32)pFeederStart,(UINT32)pFeederEnd,(UINT32)pucPos,u4TransLen,u4TransPts);
               if(uRetState==eSWDMX_HDR_PRS_DMX_MOVE_FAIL)
               {
                   LOG(0, "Demuxer fails to move data.\n");
                   goto DmxMovFail;
               }
               else if(uRetState==eSWDMX_HDR_DMX_BUF_NOT_ENOUGH)
               {
                   LOG(7, "Demuxer %s Buffer full.\n",uTagType==8 ? "Audio" : "Video");
                   goto DmxBufNotEnough;
               }
           }

       }

       if(uTagType==eFLV_TAG_TYPE_DATA)
       {
         u4TransLen=u4DataLen;
       }

       if(prSwdmxFlvInfo->u4LastDataLen==prSwdmxFlvInfo->u4LastDataBrokenNum+u4DataLen)
       {
          prSwdmxFlvInfo->fgTagDataBroken=FALSE;
          u4MoviedByte+=u4TransLen;
          prSwdmxFlvInfo->fgTagHeaderParsed=FALSE;
          prSwdmxFlvInfo->u4LastDataBrokenNum=0;
          if(prSwdmxFlvInfo->fgAacCfgBroken)
          {
             prSwdmxFlvInfo->fgAacCfgBroken=FALSE;
             if (prSwdmxFlvInfo->u1SrcId < SWDMX_SOURCE_MAX)
             {
                _SWDMX_FlvParseAacConfigTable(prSwdmxFlvInfo->pucAacCfg,prSwdmxFlvInfo);
             }
          }

          if(u4AvailSize-u4MoviedByte>=SWDMX_FLV_TAG_ENDSIZE)
          {
              pucPos=pucBitstream;
              FLV_RPOS_INC(pucPos,u4MoviedByte,pFeederStart,pFeederEnd);
              FLV_INTEGER_NEXT(u4TagEndLen,pucPos,4,pFeederStart,pFeederEnd);
              if(u4TagEndLen != (SWDMX_FLV_TAG_HEADER_SIZE+prSwdmxFlvInfo->u4LastDataLen))
              {
                 LOG(2,"ErrorHappen1,Counter=%d\n",prSwdmxFlvInfo->i4DebugerCounter);
                 goto HdrParsFail;
              }
              else
              {
                  u4MoviedByte+=SWDMX_FLV_TAG_ENDSIZE;
                  prSwdmxFlvInfo->fgErrorConvery=FALSE;
              }
          }
          else
          {
             prSwdmxFlvInfo->u4SkipDatLen=SWDMX_FLV_TAG_ENDSIZE-(u4AvailSize-u4MoviedByte);
             u4MoviedByte=u4AvailSize;
             goto DataNotEnough;
          }
       }
       else
       {
          prSwdmxFlvInfo->u4LastDataBrokenNum+=u4DataLen;
          u4MoviedByte=u4AvailSize;
          goto DataNotEnough;
       }
    }

    while(u4MoviedByte<=SWDMX_FLV_MOVI_MAX)
    {
      pucPos=pucBitstream;
      if(prSwdmxFlvInfo->i4DebugerCounter==u4DebutCounter)
      {
        LOG(10,"Stop Here\n");
      }

      if(!prSwdmxFlvInfo->fgTagHeaderParsed)
      {
         if(fgUseHeadBuf)
         {
            UCHAR *puHeaBuf=NULL;
            if(prSwdmxFlvInfo->u1EsId<2)
            {
               puHeaBuf=prSwdmxFlvInfo->aucHeaderBuf;
            }
            else
            {
               goto HdrParsFail;
            }
            pucPos=puHeaBuf;
            pFeederStart=puHeaBuf;
            pFeederEnd=puHeaBuf+SWDMX_FLV_TAG_HEADER_SIZE+SWDMX_FLV_TAG_ENDSIZE;
            fgUseHeadBuf=FALSE;
            LOG(3,"Use HeaderBuffer to Parse Header\n");

         }
         else
         {
            pucPos=pucBitstream;
            pFeederStart = (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->prFeederInfo->u4StartAddr));
            pFeederEnd =     (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->prFeederInfo->u4EndAddr));
            FLV_RPOS_INC(pucPos,u4MoviedByte,pFeederStart,pFeederEnd);
            prSwdmxFlvInfo->u4ValidSeekPos=prSwdmxFlvInfo->u8CurDmxMoviPos+u4MoviedByte;

            if(u4AvailSize-u4MoviedByte<SWDMX_FLV_TAG_HEADER_SIZE)
            {
#ifdef SWDMX_SEND_BLOCK_DATA
              goto DataNotEnough;
#endif
               u4DataLen=u4AvailSize-u4MoviedByte;
               for(u4Index=0;u4Index<u4DataLen;u4Index++)
               {
                 if(u4Index<SWDMX_FLV_TAG_HEADER_SIZE && prSwdmxFlvInfo->u1EsId<2)
                 {
                    prSwdmxFlvInfo->aucHeaderBuf[u4Index]=*pucPos;
                 }
                 FLV_RPOS_INC(pucPos,1,pFeederStart,pFeederEnd);
               }

               LOG(3,"SWDMX_FLV:Parse End,Need Header Data\n");
               prSwdmxFlvInfo->fgTagHeaderBroken=TRUE;
               prSwdmxFlvInfo->uTagHeaderBrokenIndex=u4DataLen;
               u4MoviedByte+=u4DataLen;
               prSwdmxFlvInfo->fgTagHeaderParsed=FALSE;
               goto DataNotEnough;
            }
            else
            {
               u4MoviedByte+=SWDMX_FLV_TAG_HEADER_SIZE;
            }
          }

          u4Pts=0;
          u4TagDataSzie=0;
          uTagType=0;
          FLV_INTEGER(uTagType,pucPos,1,pFeederStart,pFeederEnd);
          FLV_INTEGER(u4TagDataSzie,pucPos,3,pFeederStart,pFeederEnd);
          FLV_INTEGER(u4Pts,pucPos,3,pFeederStart,pFeederEnd);
          FLV_INTEGER(uData,pucPos,1,pFeederStart,pFeederEnd);
          u4Pts|=(uData<<24);
          u4Pts=u4Pts*SWDMX_FLV_PTS_FACTOR;
          FLV_INTEGER(u4DataParam,pucPos,3,pFeederStart,pFeederEnd);

          if((uTagType!=eFLV_TAG_TYPE_AUD)&&(uTagType!=eFLV_TAG_TYPE_VID)&&(uTagType!=eFLV_TAG_TYPE_DATA))
          {
             LOG(2,"WarningHappen, ReservedTag found = %d, change to TYPE_DATA\n", uTagType);
             uTagType = eFLV_TAG_TYPE_DATA;
          }

          if(u4DataParam!=SWDMX_FLV_STREAM_ID)
          {
              LOG(2,"ErrorHappen4,Counter=%d\n",prSwdmxFlvInfo->i4DebugerCounter);
              goto HdrParsFail;
          }
          prSwdmxFlvInfo->fgAacSendHeader=FALSE;
          prSwdmxFlvInfo->fgTagHeaderParsed=TRUE;
          prSwdmxFlvInfo->u4LastDataLen=u4TagDataSzie;
          prSwdmxFlvInfo->u4LastPts=u4Pts;
          prSwdmxFlvInfo->uLastTagType=uTagType;

      }
      else
      {
          uTagType=prSwdmxFlvInfo->uLastTagType;
          u4TagDataSzie=prSwdmxFlvInfo->u4LastDataLen;
          u4Pts=prSwdmxFlvInfo->u4LastPts;
      }

      u4DataLen=flv_min((u4AvailSize-u4MoviedByte),u4TagDataSzie);
#ifdef SWDMX_SEND_BLOCK_DATA
      if(u4TagDataSzie>(u4AvailSize-u4MoviedByte))
      {
        goto DataNotEnough;
      }
#endif
      if(u4DataLen &&(uTagType==eFLV_TAG_TYPE_AUD || uTagType==eFLV_TAG_TYPE_VID))
      {
         pFeederStart = (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->prFeederInfo->u4StartAddr));
         pFeederEnd =    (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->prFeederInfo->u4EndAddr));
         pucPos=pucBitstream;
         fgDoTransData=TRUE;
         FLV_RPOS_INC(pucPos,u4MoviedByte,pFeederStart,pFeederEnd);
         if(u4DataLen==u4TagDataSzie|| uTagType==eFLV_TAG_TYPE_AUD)
         {
            u4TransPts=u4Pts;
         }
         else
         {
            u4TransPts=SWDMX_FLV_PTS_INFOR_PARTION;
         }

         if(uTagType==eFLV_TAG_TYPE_AUD)
         {
            prSwdmxFlvInfo->u2TagIdentify=((*pucPos)<<8)|(0xff&prSwdmxFlvInfo->u2TagIdentify);
            FLV_RPOS_INC(pucPos,1,pFeederStart,pFeederEnd);
            u4MoviedByte++;
            u4TransLen=u4DataLen-1;
            if(prSwdmxFlvInfo->fgNeedSetAudStartPts && u4Pts>=prSwdmxFlvInfo->u4AudStartPts)
            {
              prSwdmxFlvInfo->fgNeedSetAudStartPts=FALSE;
              prSwdmxFlvInfo->u4TriggerAudPts=u4Pts;
              
              LOG(3,"swdmx stc change2:(PTS:0x%x,STC:0x%x)\n",u4Pts,VDP_GetPts(prSwdmxInst->u1B2rId));
              STC_SetStcValue(prSwdmxInst->u1StcId,u4Pts-9000);
              AUD_SetStartPts(prSwdmxInst->u1AdecId,u4Pts);
              if(prSwdmxFlvInfo->u4VDecType==ENUM_SWDMX_VDEC_MPEG4)
              {
                  STC_StartStc(prSwdmxInst->u1StcId);
              }
            }

            if(u4TransLen==0 && u4TagDataSzie!=1)
            {
              LOG(3,"SWDMX_FLV:Parse End,Need Header Data\n");
              prSwdmxFlvInfo->fgTagDataBroken=TRUE;
              prSwdmxFlvInfo->u4LastDataBrokenNum=1;
              u4MoviedByte=u4AvailSize;
              goto DataNotEnough;
            }
         }
         else
         {
            u4TransLen=u4DataLen;
         }

         if(uTagType==eFLV_TAG_TYPE_AUD && prSwdmxFlvInfo->u4ADecType==ENUM_SWDMX_ADEC_AAC)
         {
            UINT8 uPackType;
            FLV_INTEGER(uPackType,pucPos,1,pFeederStart,pFeederEnd);
            u4TransLen--;
            u4MoviedByte++;
            if(uPackType==0)
            {
               if(u4DataLen==u4TagDataSzie)
               {
                  _SWDMX_FlvParseAacConfigTable(pucPos,prSwdmxFlvInfo);
                  prSwdmxFlvInfo->fgAacCfgBroken=FALSE;
               }
               else
               {
                  for(u4Index=0;u4Index<u4TransLen;u4Index++)
                  {
                     FLV_INTEGER(uData,pucPos,1,pFeederStart,pFeederEnd);
                     if (prSwdmxFlvInfo->u1SrcId < SWDMX_SOURCE_MAX)
                     {
                        prSwdmxFlvInfo->pucAacCfg[u4Index]=uData;
                     }
                  }
                  prSwdmxFlvInfo->fgAacCfgBroken=TRUE;
               }

               fgDoTransData=FALSE;
            }
            else
            {
               if(prSwdmxFlvInfo->fgAacSendHeader==FALSE)
               {
				   uRetState=_SWDMX_FlvSendAacHeader(prSwdmxFlvInfo);
				   if(uRetState==eSWDMX_HDR_PRS_DMX_MOVE_FAIL)
				   {
					   LOG(0, "Demuxer fails to move data.\n");
					   goto DmxMovFail;
				   }
				   else if(uRetState==eSWDMX_HDR_DMX_BUF_NOT_ENOUGH)
				   {
					   LOG(7, "Demuxer audio Buffer full,When send aac header.\n");
					   if(uTagType==eFLV_TAG_TYPE_AUD)
					   {
						   u4MoviedByte-=2;
					   }
					   goto DmxBufNotEnough;
				   }
				   prSwdmxFlvInfo->fgAacSendHeader=TRUE;
               }
            }

            if(u4TransLen==0 && u4TagDataSzie!=2)
            {
               LOG(3,"SWDMX_FLV:Parse End,Need Header Data\n");
               prSwdmxFlvInfo->fgTagDataBroken=TRUE;
               prSwdmxFlvInfo->u4LastDataBrokenNum=2;
               u4MoviedByte=u4AvailSize;
               goto DataNotEnough;
            }
         }

         if(fgDoTransData)
         {
             if(uTagType== eFLV_TAG_TYPE_VID)
             {
                if((((*pucPos)>>4)&0xf)==1)
                {
                    prSwdmxFlvInfo->u4LastKeyPos=prSwdmxFlvInfo->u4ValidSeekPos;
                    prSwdmxFlvInfo->u4LastKeyPts=prSwdmxFlvInfo->u4LastPts;
                }
             }

             uRetState=_SWDMX_DmxTransformData(prSwdmxFlvInfo,uTagType,(UINT32)pFeederStart,(UINT32)pFeederEnd,(UINT32)pucPos,u4TransLen,u4TransPts);
             if(uRetState==eSWDMX_HDR_PRS_DMX_MOVE_FAIL)
             {
                 LOG(0, "Demuxer fails to move data.\n");
                 goto DmxMovFail;
             }
             else if(uRetState==eSWDMX_HDR_DMX_BUF_NOT_ENOUGH)
             {
                 LOG(7, "Demuxer %s Buffer full.\n",uTagType==8 ? "Audio" : "Video");
                 if(uTagType==eFLV_TAG_TYPE_AUD && prSwdmxFlvInfo->u4ADecType==ENUM_SWDMX_ADEC_AAC)
                 {
                     u4MoviedByte-=2;
                 }
                 else if(uTagType==eFLV_TAG_TYPE_AUD)
                 {
                     u4MoviedByte-=1;
                 }
                 goto DmxBufNotEnough;
             }
         }

         if(uTagType==eFLV_TAG_TYPE_VID)
             prSwdmxFlvInfo->u4CurVidPts=u4Pts;
         else
            prSwdmxFlvInfo->u4AudPts=u4Pts;
      }

      if(uTagType==eFLV_TAG_TYPE_DATA)
      {
        u4TransLen=u4DataLen;
      }

      if(u4DataLen<u4TagDataSzie)
      {
        prSwdmxFlvInfo->fgTagDataBroken=TRUE;
        prSwdmxFlvInfo->u4LastDataBrokenNum=u4DataLen;
        u4MoviedByte=u4AvailSize;
        LOG(4,"SWDMX_FLV:Data End,Need Tag Data\n");
        goto DataNotEnough;
      }
      else
      {
         prSwdmxFlvInfo->fgTagHeaderParsed=FALSE;
         u4MoviedByte+=u4TransLen;
         if(prSwdmxFlvInfo->fgTrickMode)
         {
           prSwdmxFlvInfo->u4AudPts=u4Pts;
           break;
         }

         if(u4AvailSize-u4MoviedByte>=SWDMX_FLV_TAG_ENDSIZE)
         {
            pucPos=pucBitstream;
            FLV_RPOS_INC(pucPos,u4MoviedByte,pFeederStart,pFeederEnd);
            FLV_INTEGER_NEXT(u4TagEndLen,pucPos,4,pFeederStart,pFeederEnd);

            if(u4TagEndLen != (SWDMX_FLV_TAG_HEADER_SIZE+u4TagDataSzie))
            {
               LOG(2,"ErrorHappen4,Counter=%d\n",prSwdmxFlvInfo->i4DebugerCounter);
               goto HdrParsFail;
            }
            else
            {
                u4MoviedByte+=SWDMX_FLV_TAG_ENDSIZE;
                prSwdmxFlvInfo->fgErrorConvery=FALSE;
				if(prSwdmxFlvInfo->fgBeginToPlay==TRUE)
				{
				   LOG(5,"prSwdmxFlvInfo->fgBeginToPlay=TRUE, return\n");
                   break;
				}
            }
         }
         else
         {
            prSwdmxFlvInfo->u4SkipDatLen=SWDMX_FLV_TAG_ENDSIZE-(u4AvailSize-u4MoviedByte);
            u4MoviedByte=u4AvailSize;
            goto DataNotEnough;
         }
      }
    }

  *pu4ConsumeSize=u4MoviedByte;
  return eSWDMX_HDR_PRS_SUCCEED;

DataNotEnough:
   prSwdmxFlvInfo->fgNewRp=TRUE;
  *pu4ConsumeSize=u4MoviedByte;
  return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;

DmxBufNotEnough:
  prSwdmxInst->fgFifoFull=TRUE;
  *pu4ConsumeSize=u4MoviedByte;
  return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;

DmxMovFail:
  *pu4ConsumeSize=u4MoviedByte;
  return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
HdrParsFail:
  *pu4ConsumeSize=u4MoviedByte;
  return eSWDMX_HDR_PRS_FAIL;
}



static SWDMX_HDR_PRS_STATE_T _SWDMX_FlvTagPrs(
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo,
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange)
{
    BOOL fgRangeEnd = FALSE;
    UINT32 u4ReadSize = 0,u4AlignSize=0;
    UINT32 u4ConsumedSize = 0,u4ErrorSkipSize=0;
    UINT32 u4PrsState = eSWDMX_HDR_PRS_SUCCEED;

    SWDMX_INFO_T* prSwdmxInst =(SWDMX_INFO_T* )prSwdmxFlvInfo->u4SwdmxInst;
    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;

    if (prSwdmxFlvInfo->fgFeederInvalid)
    {
        INT32 i;
        LOG(3, "Feeder is invalid,FeederReqID = %ld, FeederReceiveID = %ld,req times = %ld, Cur Dmx Pos = %lld\n",
            prSwdmxFlvInfo->u4FeederReqID,
            prSwdmxFlvInfo->u4FeederReqReceiveID,
            prSwdmxFlvInfo->i4ReqReadTimes,
            prSwdmxFlvInfo->u8CurDmxMoviPos);

        for (i = 0; i < prSwdmxFlvInfo->i4ReqReadTimes; i++)
        {
            _SWDMX_FlvGetDataFromFeeder(prSwdmxFlvInfo,&rFeederReqData,2);
        }

        _SWDMX_FlvResetFeeder(prSwdmxFlvInfo);
        LOG(3,"Flv invalid, reset to dmx movipos\n");
        prSwdmxFlvInfo->fgNewRp = TRUE;
        prSwdmxFlvInfo->u8CurMoviPos = prSwdmxFlvInfo->u8CurDmxMoviPos;
        prSwdmxFlvInfo->fgFeederInvalid = FALSE;
        prSwdmxFlvInfo->fgTagHeaderBroken=FALSE;
        prSwdmxFlvInfo->fgTagDataBroken=FALSE;
        prSwdmxFlvInfo->fgTagHeaderParsed=FALSE;
        prSwdmxFlvInfo->u4SkipDatLen=0;
        prSwdmxFlvInfo->u4FrameAddrToVdec=0;
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    // Check if demuxing is done
    if (prSwdmxFlvInfo->u8CurDmxMoviPos >= prRangeList->prDmxRangeInfo->z_range_sz)
    {
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        if (prSwdmxFlvInfo->fgEnVideo && !prSwdmxFlvInfo->fgDeliverVEos)
        {
            if (prSwdmxFlvInfo->fgVDecIsPause)
            {
                _SWDMX_FlvVideoWakeUp(prSwdmxFlvInfo);
                prSwdmxFlvInfo->fgVDecIsPause = FALSE;
            }

            LOG(5, "Cur DmxMoviPos = %ld, range size = %ld\n", prSwdmxFlvInfo->u8CurDmxMoviPos,prRangeList->prDmxRangeInfo->z_range_sz);
            LOG(5, "_SWDMX_FlvHdrPrsWithoutIdx Deliver Video Eos\n");
            if (!_SWDMX_FlvSendEos(prSwdmxFlvInfo, eSWDMX_MEDIA_VIDEO))
            {
                //LOG(3, "Fail to send Video Eos\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            prSwdmxFlvInfo->fgDeliverVEos = TRUE;
        }

        if (prSwdmxFlvInfo->fgEnAudio && !prSwdmxFlvInfo->fgDeliverAEos)
        {
            LOG(5, "_SWDMX_FlvHdrPrsWithoutIdx Deliver Audio Eos\n");
            if (!_SWDMX_FlvSendEos(prSwdmxFlvInfo, eSWDMX_MEDIA_AUDIO))
            {
                LOG(3, "Fail to send Audio Eos\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            prSwdmxFlvInfo->fgDeliverAEos = TRUE;
        }

        x_thread_delay(20);
        return eSWDMX_HDR_PRS_SUCCEED;
    }


    // Request data for the first time
    if (prSwdmxFlvInfo->u8CurMoviPos == 0)
    {
         FeederSetFlush(prSwdmxInst->eFeederSrc);
        _SWDMX_FlushReqMsgQ(prSwdmxFlvInfo->u1SrcId);
        if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, prSwdmxFlvInfo->prFeederInfo) != FEEDER_E_OK)
        {
            LOG(7, "Feeder input buffer error.\n");
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

        if(prRangeList->prDmxRangeInfo->z_range_sz > prSwdmxFlvInfo->u8CurMoviPos)
        {
            u4ReadSize = prRangeList->prDmxRangeInfo->z_range_sz - prSwdmxFlvInfo->u8CurMoviPos;
        }

        u4ReadSize = flv_min(u4ReadSize, FLV_MIN_READ_SIZE);
        //prSwdmxFlvInfo->u8CurMoviPos = prRangeList->prDmxRangeInfo->ui8_fileoffset;
        prSwdmxFlvInfo->u8CurDmxMoviPos = prSwdmxFlvInfo->u8CurMoviPos;

        if (FeederSetRequest(prSwdmxInst->eFeederSrc, u4ReadSize,prSwdmxFlvInfo->u8CurMoviPos,&rFeederCond,prSwdmxFlvInfo->u4FeederReqID) != FEEDER_E_BUFFER_FULL)
        {
            x_thread_delay(100);
            if(!_SWDMX_FlvGetDataFromFeeder(prSwdmxFlvInfo, &rFeederReqData,8))
            {
                if(prSwdmxFlvInfo->fgPlayAbort)
                {
                    return eSWDMX_HDR_PRS_SUCCEED;
                }
                else
                {
                    return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                }
            }
            prSwdmxFlvInfo->u4FeederReqID++;
            prSwdmxFlvInfo->fgFeederInvalid = FALSE;
            prSwdmxFlvInfo->u8CurMoviPos +=(rFeederReqData.u4ReadSize - rFeederReqData.u4AlignOffset);
            prSwdmxFlvInfo->u4CurDmxFeederRPtr = VIRTUAL(prSwdmxFlvInfo->prFeederInfo->u4StartAddr + rFeederReqData.u4AlignOffset);
            prSwdmxFlvInfo->u4LeftBytesInFeeder = rFeederReqData.u4ReadSize-rFeederReqData.u4AlignOffset;
            LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
        }
    }


    if(FeederGetBufferInfo(prSwdmxInst->eFeederSrc, prSwdmxFlvInfo->prFeederInfo) != FEEDER_E_OK)
    {
       LOG(7, "Feeder input buffer error.\n");
       return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    if(!prSwdmxFlvInfo->fgTrickMode &&(prSwdmxFlvInfo->prFeederInfo->u4BytesInBuf+FLV_MIN_READ_SIZE <prSwdmxFlvInfo->prFeederInfo->u4Size))
    {
        u4ReadSize=0;
        if (prRangeList->prDmxRangeInfo->z_range_sz > prSwdmxFlvInfo->u8CurMoviPos)
        {
            u4ReadSize = prRangeList->prDmxRangeInfo->z_range_sz - prSwdmxFlvInfo->u8CurMoviPos;
        }

        u4ReadSize = flv_min(u4ReadSize, FLV_MIN_READ_SIZE);
        
        if (u4ReadSize == 0)
        {
            fgRangeEnd = TRUE;
        }
        if(prRangeList->prDmxRangeInfo->b_thumbnail_pb && prSwdmxFlvInfo->i4ReqReadTimes>0)
        {
           u4ReadSize=0;
        }
        
        if(u4ReadSize>0 && prSwdmxFlvInfo->i4ReqReadTimes<=(SWDMX_DATA_Q_SIZE-2))
        {
            if((prSwdmxFlvInfo->prFeederInfo->u4BytesInBuf + u4ReadSize < prSwdmxFlvInfo->prFeederInfo->u4Size))
            {
               LOG(6, "Left bytes = %ld offset = %lld id = %ld.\n", prSwdmxFlvInfo->prFeederInfo->u4BytesInBuf,prSwdmxFlvInfo->u8CurMoviPos,prSwdmxFlvInfo->u4FeederReqID);
               if(FeederSetRequest(prSwdmxInst->eFeederSrc,u4ReadSize, prSwdmxFlvInfo->u8CurMoviPos,&rFeederCond,prSwdmxFlvInfo->u4FeederReqID) != FEEDER_E_BUFFER_FULL)
               {
                  UINT64 u8Remainder;
                  u4AlignSize=FeederGetAlignSize(prSwdmxInst->eFeederSrc);
                  u8Div6432(prSwdmxFlvInfo->u8CurMoviPos,(UINT64)u4AlignSize, &u8Remainder);
                  prSwdmxFlvInfo->u8CurMoviPos+=((UINT64)rFeederCond.u4TransmitSize-u8Remainder);
                  prSwdmxFlvInfo->i4ReqReadTimes++;
                  prSwdmxFlvInfo->u4FeederReqID++;
               }
               else
               {
                  LOG(3, "Feeder Buffer Full\n");
                  if(FeederGetBufferInfo(prSwdmxInst->eFeederSrc, prSwdmxFlvInfo->prFeederInfo) != FEEDER_E_OK)
                  {
                     LOG(1, "Feeder input buffer error.\n");
                     return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                  }
                  LOG(3, "Cur Fed's write ptr = 0x%08x\n", prSwdmxFlvInfo->prFeederInfo->u4WriteAddr);
               }


            }
        }
    }
    else if(prSwdmxFlvInfo->fgTrickMode)
    {
       INT32 i4TrickRet=FLV_SEEK_RET_FIND;
       UINT16 u2CurQNum=0,u2MaxQNum=0;
       VDEC_GetQueueInfo(prSwdmxFlvInfo->u1EsId, &u2CurQNum, &u2MaxQNum);

       if(u2CurQNum > prSwdmxFlvInfo->u4VidFps *3)
       {
          x_thread_delay(5);
          return eSWDMX_HDR_PRS_SUCCEED;
       }


       if((!prSwdmxFlvInfo->fgTrickRcv) && (!_SWDMX_FlvCalculateRepeatFrame(i4PlaySpeed,prSwdmxFlvInfo)))
       {   
           if(i4PlaySpeed>2000)
           {
               i4TrickRet=_SWDMX_FlvNextKeyFrame(prSwdmxFlvInfo,TRUE);
           }
           else if(i4PlaySpeed<0)
           {
               i4TrickRet=_SWDMX_FlvNextKeyFrame(prSwdmxFlvInfo,FALSE);
           }
           else
           {
               LOG(3,"Trick mode,but speed is %d\n",i4PlaySpeed);
           }

           if(i4TrickRet==FLV_SEEK_RET_FILEEND)
           {
             prSwdmxFlvInfo->u8CurDmxMoviPos=prRangeList->prDmxRangeInfo->z_range_sz;
             prSwdmxFlvInfo->u4LeftBytesInFeeder=0;
             prSwdmxFlvInfo->u4LastPts=(UINT32)prSwdmxFlvInfo->prRangeList->prDmxRangeInfo->ui8_vid_duration;
           }
           else if(i4TrickRet==FLV_SEEK_RET_FILEERR)
           {
              prSwdmxFlvInfo->fgTrickRcv=TRUE;
           }
           else if(i4TrickRet==FLV_SEEK_RET_PLAYABORT)
           {
              return eSWDMX_HDR_PRS_SUCCEED;
           }
       }

       prSwdmxFlvInfo->fgTagHeaderBroken=FALSE;
       prSwdmxFlvInfo->fgTagDataBroken=FALSE;
       prSwdmxFlvInfo->fgTagHeaderParsed=FALSE;
       prSwdmxFlvInfo->u4SkipDatLen=0;
       prSwdmxFlvInfo->u4FrameAddrToVdec=0;
    }

#ifdef SWDMX_TIME_PROFILE
    HAL_TIME_T rTimeRstS, rTimeRstE, rTimeRstDt;
    HAL_GetTime(&rTimeRstS);
#endif

    u4ConsumedSize=0;
    prSwdmxFlvInfo->fgRecoveredFromErr=FALSE;
    if(!prSwdmxFlvInfo->fgTrickMode)
    {
       prSwdmxFlvInfo->fgTrickRcv=FALSE;
    }
    if((!prSwdmxFlvInfo->fgTrickRcv)&&(!prSwdmxFlvInfo->fgErrorConvery)&&prSwdmxFlvInfo->u4LeftBytesInFeeder)
    {
        // Start to parse data
        u4PrsState = _SWDMX_FlvDeliverTagData(prSwdmxFlvInfo,(UCHAR*)(prSwdmxFlvInfo->u4CurDmxFeederRPtr),prRangeList,prSwdmxFlvInfo->u4LeftBytesInFeeder,&u4ConsumedSize,i4PlaySpeed,fgRangeEnd);
    }
    else if(prSwdmxFlvInfo->fgErrorConvery || prSwdmxFlvInfo->fgTrickRcv)
    {
        u4PrsState=eSWDMX_HDR_PRS_FAIL;
    }
    else
    {
        u4PrsState=eSWDMX_HDR_PRS_SUCCEED;
    }

#ifdef SWDMX_TIME_PROFILE
    HAL_GetTime(&rTimeRstE);
    HAL_GetDeltaTime(&rTimeRstDt, &rTimeRstS, &rTimeRstE);
    LOG(0, "Swdmx parsing and deliver time = (%ld) \n", rTimeRstDt.u4Micros);
#endif

    switch (u4PrsState)
    {
        case eSWDMX_HDR_PRS_DMX_MOVE_FAIL:
            LOG(3, "Flv Parse Fail\n");
            prSwdmxFlvInfo->u8CurDmxMoviPos = prRangeList->prDmxRangeInfo->z_range_sz;
            prSwdmxFlvInfo->u4LeftBytesInFeeder = 0;
            return eSWDMX_HDR_PRS_FAIL;
        case eSWDMX_HDR_PRS_FAIL:
        {
            //if(!prSwdmxFlvInfo->fgTrickMode)
            {
                u4ErrorSkipSize=0;
                u4PrsState=_SWDMX_ErrRecovery(prSwdmxFlvInfo,(UCHAR *)(prSwdmxFlvInfo->u4CurDmxFeederRPtr+u4ConsumedSize),prSwdmxFlvInfo->u4LeftBytesInFeeder-u4ConsumedSize,&u4ErrorSkipSize);
                if(u4PrsState==eSWDMX_HDR_PRS_SUCCEED)
                {
                  prSwdmxFlvInfo->fgRecoveredFromErr=TRUE;
                  u4ConsumedSize+=u4ErrorSkipSize;
                  prSwdmxFlvInfo->u4ValidSeekPos=prSwdmxFlvInfo->u8CurDmxMoviPos+u4ConsumedSize;
                }
                else if(u4PrsState==eSWDMX_HDR_PRS_FAIL)
                {
                    prSwdmxFlvInfo->u8CurDmxMoviPos = prRangeList->prDmxRangeInfo->z_range_sz;
                    prSwdmxFlvInfo->u4LeftBytesInFeeder = 0;
                    return eSWDMX_HDR_PRS_FAIL;
                }
                else if(u4PrsState==eSWDMX_HDR_FED_BUF_NOT_ENOUGH)
                {
                    if(prSwdmxFlvInfo->fgTrickMode)
                    {
                       FEEDER_REQ_DATA_T rReqData;
                       UINT32 u4FeederAlignSize=0;
                       _SWDMX_FlvResetFeeder(prSwdmxFlvInfo);
					   prSwdmxFlvInfo->u4FrameAddrToVdec=0;
                       u4FeederAlignSize=FeederGetAlignSize(prSwdmxInst->eFeederSrc);
                       if(prSwdmxFlvInfo->i4PrevSpeed>0)
                       {
                           prSwdmxFlvInfo->u8CurDmxMoviPos=prSwdmxFlvInfo->u8CurDmxMoviPos+prSwdmxFlvInfo->u2FindCounter*FLV_ERROR_RECOVERY_LEN;
                       }
                       else
                       {
                           if(prSwdmxFlvInfo->u8CurDmxMoviPos>(SWDMX_FLV_FILE_HEADER_SIZE+u4FeederAlignSize+prSwdmxFlvInfo->u2FindCounter*FLV_ERROR_RECOVERY_LEN))
                           {
                               prSwdmxFlvInfo->u8CurDmxMoviPos=prSwdmxFlvInfo->u8CurDmxMoviPos-u4FeederAlignSize-prSwdmxFlvInfo->u2FindCounter*FLV_ERROR_RECOVERY_LEN;

                           }
                           else
                           {
                               prSwdmxFlvInfo->u8CurDmxMoviPos = prRangeList->prDmxRangeInfo->z_range_sz;
                               return eSWDMX_HDR_PRS_FAIL;
                           }
                       }
                       u4PrsState=_SWDMX_FlvReqTagData(prSwdmxFlvInfo,prSwdmxFlvInfo->u8CurDmxMoviPos,FLV_ERROR_RECOVERY_LEN,FALSE,&rReqData);
                       if(u4PrsState==FLV_SEEK_RET_FIND)
                       {
                          prSwdmxFlvInfo->u4CurDmxFeederRPtr=rReqData.u4WriteAddr+rReqData.u4AlignOffset;
                          prSwdmxFlvInfo->u4LeftBytesInFeeder=rReqData.u4ReadSize-rReqData.u4AlignOffset;
                          prSwdmxFlvInfo->PRecoverPos=(UCHAR *)prSwdmxFlvInfo->u4CurDmxFeederRPtr;
                       }
					   else if(u4PrsState==FLV_SEEK_RET_PLAYABORT)
					   {
                          return eSWDMX_HDR_PRS_SUCCEED;
					   }
                       else
                       {
                          prSwdmxFlvInfo->u8CurDmxMoviPos = prRangeList->prDmxRangeInfo->z_range_sz;
                          return eSWDMX_HDR_PRS_FAIL;
                       }
                    }
                    else
                    {
                        u4ConsumedSize+=u4ErrorSkipSize;
                        prSwdmxFlvInfo->fgNewRp=TRUE;
                    }
                }
            }
            break;
        }
        default:
          break;
    }

    if(prSwdmxFlvInfo->fgTrickMode)
    {
       if(prSwdmxFlvInfo->u8CurDmxMoviPos+u4ConsumedSize>=prRangeList->prDmxRangeInfo->z_range_sz)
       {
         prSwdmxFlvInfo->u8CurDmxMoviPos=prRangeList->prDmxRangeInfo->z_range_sz;
       }

       if(u4PrsState==eSWDMX_HDR_PRS_SUCCEED)
       {
           if(prSwdmxFlvInfo->fgRecoveredFromErr)
           {
              prSwdmxFlvInfo->u8CurDmxMoviPos+=u4ConsumedSize;
              prSwdmxFlvInfo->u8CurMoviPos=prSwdmxFlvInfo->u8CurDmxMoviPos;
           }
           prSwdmxFlvInfo->fgTrickRcv=FALSE;
       }
       else if(!prSwdmxFlvInfo->fgTrickRcv)
       {
          prSwdmxFlvInfo->u4VidRepeatFrame++;
       }

       if(prSwdmxFlvInfo->fgKeyExist && prSwdmxFlvInfo->fgRecoveredFromErr && prSwdmxFlvInfo->fgKeyExist &&prSwdmxFlvInfo->prVidKeyIdxTbl)
       {
          UINT32 u4CurKeyIndex=0;
          while(1)
          {
             u4CurKeyIndex= prSwdmxFlvInfo->u4CurSentKey;
             //u4CurKeyIndex= ((prSwdmxFlvInfo->i4PrevSpeed>0) ? (min(u4CurKeyIndex+1,prSwdmxFlvInfo->prVidKeyIdxTbl->ui4_number_of_entry-1)) : (max(0,u4CurKeyIndex-1)));
             if(prSwdmxFlvInfo->i4PrevSpeed>0)
             {
                u4CurKeyIndex=flv_min(u4CurKeyIndex+1,prSwdmxFlvInfo->prVidKeyIdxTbl->ui4_number_of_entry-1);
             }
             else
             {
                if(u4CurKeyIndex>0)
                {
                   u4CurKeyIndex=u4CurKeyIndex-1;
                }
                else
                {
                   u4CurKeyIndex=0;
                }
             }

             if(u4CurKeyIndex==prSwdmxFlvInfo->u4CurSentKey)
             {
                prSwdmxFlvInfo->u8CurDmxMoviPos = prRangeList->prDmxRangeInfo->z_range_sz;
                return eSWDMX_HDR_PRS_FAIL;
             }

             if(prSwdmxFlvInfo->i4PrevSpeed>0)
             {
                if(prSwdmxFlvInfo->prVidKeyIdxTbl->ui8_base_offset+prSwdmxFlvInfo->prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKeyIndex].ui4_relative_offset<prSwdmxFlvInfo->u8CurDmxMoviPos)
                {
                   prSwdmxFlvInfo->u4CurSentKey++;
                }
                else
                {
                   break;
                }
             }
             else
             {
                if(prSwdmxFlvInfo->prVidKeyIdxTbl->ui8_base_offset+prSwdmxFlvInfo->prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKeyIndex].ui4_relative_offset>prSwdmxFlvInfo->u8CurDmxMoviPos)
                {
                   prSwdmxFlvInfo->u4CurSentKey--;
                }
                else
                {
                   break;
                }
             }
          }
          prSwdmxFlvInfo->u4CurRepeatFrameIdx=prSwdmxFlvInfo->u4CurSentKey;
       }

       return eSWDMX_HDR_PRS_SUCCEED;
    }

    if (prSwdmxFlvInfo->u4LeftBytesInFeeder >= u4ConsumedSize)
    {
        prSwdmxFlvInfo->u8CurDmxMoviPos += u4ConsumedSize;
        prSwdmxFlvInfo->u4LeftBytesInFeeder -= u4ConsumedSize;
    }
    else
    {
        prSwdmxFlvInfo->fgNewRp=TRUE;
        prSwdmxFlvInfo->u4LeftBytesInFeeder = 0;
    }

    if (u4ConsumedSize != 0)
    {
        LOG(6, "u4ConsumedSize = %ld u4LeftBytesInFeeder = 0x%08x.\n", u4ConsumedSize, prSwdmxFlvInfo->u4LeftBytesInFeeder);
    }

    if(prSwdmxFlvInfo->u4LeftBytesInFeeder==0 && prSwdmxFlvInfo->i4ReqReadTimes>0)
    {
      prSwdmxFlvInfo->fgNewRp=TRUE;
    }

    if ((prSwdmxFlvInfo->u4CurDmxFeederRPtr + u4ConsumedSize) >= VIRTUAL(prSwdmxFlvInfo->prFeederInfo->u4EndAddr))
    {
        prSwdmxFlvInfo->u4CurDmxFeederRPtr =  VIRTUAL(prSwdmxFlvInfo->prFeederInfo->u4StartAddr) +
            ((prSwdmxFlvInfo->u4CurDmxFeederRPtr + u4ConsumedSize) -VIRTUAL(prSwdmxFlvInfo->prFeederInfo->u4EndAddr));
    }
    else
    {
        prSwdmxFlvInfo->u4CurDmxFeederRPtr += u4ConsumedSize;
    }

    // Update read ptr to feeder
    if(u4ConsumedSize>0)
    {
        FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, VIRTUAL(prSwdmxFlvInfo->u4CurDmxFeederRPtr));
    }

    LOG(9, "_rSwdmxFlvInfo.u4CurDmxMoviPos = %ld.\n", prSwdmxFlvInfo->u8CurDmxMoviPos);

    if (prSwdmxFlvInfo->i4ReqReadTimes > 0 && prSwdmxFlvInfo->fgNewRp)
    {

        if(!_SWDMX_FlvGetDataFromFeeder(prSwdmxFlvInfo,&rFeederReqData,2))
        {
            if(prSwdmxFlvInfo->fgPlayAbort)
            {
                return eSWDMX_HDR_PRS_SUCCEED;
            }
            else
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
        }

        if(((rFeederReqData.u4ReadSize-rFeederReqData.u4AlignOffset)<FLV_MIN_READ_SIZE)
            &&((prSwdmxFlvInfo->u8CurDmxMoviPos+rFeederReqData.u4ReadSize-rFeederReqData.u4AlignOffset)<prRangeList->prDmxRangeInfo->z_range_sz))
        {
            prSwdmxFlvInfo->fgFeederInvalid=TRUE;
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

        if(rFeederReqData.u4WriteAddr==0)
        {
            prSwdmxFlvInfo->fgFeederInvalid=TRUE;
			return eSWDMX_HDR_PRS_SUCCEED;
        }
        /*
              if (!_SWDMX_FlvGetDataFromFeeder(prSwdmxFlvInfo, &rFeederReqData))
              {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
              }
            */
        
        prSwdmxFlvInfo->i4ReqReadTimes--;
        LOG(6, "rFeederReqData.u4WriteAddr = 0x%08x. ReadSize = 0x%08x id = %ld\n", rFeederReqData.u4WriteAddr,rFeederReqData.u4ReadSize,rFeederReqData.u4Id);
        if((rFeederReqData.u4ReadSize <rFeederReqData.u4AlignOffset) || (rFeederReqData.u4ReadSize == 0))
        {
            LOG(3, "Feeder read size is smaller than align offset!!!\n");
            prSwdmxFlvInfo->u8CurDmxMoviPos = prRangeList->prDmxRangeInfo->z_range_sz;
            prSwdmxFlvInfo->u4LeftBytesInFeeder = 0;
            return eSWDMX_HDR_PRS_FAIL;
        }

        prSwdmxFlvInfo->u4LeftBytesInFeeder += (rFeederReqData.u4ReadSize-rFeederReqData.u4AlignOffset);
        FLV_RPOS_INC(rFeederReqData.u4WriteAddr,rFeederReqData.u4AlignOffset,prSwdmxFlvInfo->prFeederInfo->u4StartAddr,prSwdmxFlvInfo->prFeederInfo->u4EndAddr);
#ifndef SWDMX_SEND_BLOCK_DATA
        prSwdmxFlvInfo->u4CurDmxFeederRPtr =  VIRTUAL(rFeederReqData.u4WriteAddr);
#endif
        prSwdmxFlvInfo->fgNewRp = FALSE;
        LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
    }

    return eSWDMX_HDR_PRS_SUCCEED;
}


static INT32 _SWDMX_FlvBeginPlay(SWDMX_FLV_INFO_T *prSwdmxFlvInfo)
{
  VDEC_PTS_INFO_T rPTSInfo;
  MM_RANGE_INFO_T *prCurRangeInfo=prSwdmxFlvInfo->prRangeList->prDmxRangeInfo;
  UINT32 u4VidStartPts=0;
  SWDMX_INFO_T* prSwdmxInst = (SWDMX_INFO_T* )prSwdmxFlvInfo->u4SwdmxInst;
  BOOL fgSentSeqHdr=FALSE;
  //FeederResetFlushId(prSwdmxInst->eFeederSrc);
  rPTSInfo.u8Scale = DEMUX_FLV_AV_PERIOD;
  rPTSInfo.u4Rate = prSwdmxFlvInfo->u4VidFps;
  x_memset(&rPTSInfo,0,sizeof(VDEC_PTS_INFO_T));

  if(prSwdmxInst->rCurRangeReq.fgSetRenderPosition == TRUE ||(prCurRangeInfo->t_vid_start_render_pts&& prSwdmxFlvInfo->fgKeyExist))
  {
    if((prSwdmxFlvInfo->u4VDecType==ENUM_SWDMX_VDEC_H264 &&(!prSwdmxFlvInfo->fgH264SeqHeaderParsed))
       ||(prSwdmxFlvInfo->u4ADecType==ENUM_SWDMX_ADEC_AAC &&(!prSwdmxFlvInfo->fgGetAacHeader)))
    {
      return 0;
    }

    if(prCurRangeInfo->t_vid_start_render_pts)
    {
       _SWDMX_FlvSeek(prSwdmxFlvInfo,prCurRangeInfo->t_vid_start_render_pts,prSwdmxFlvInfo->i4PrevSpeed,eFLV_SEEK_TYPE_TIME);
       u4VidStartPts=prCurRangeInfo->t_vid_start_render_pts;
       prSwdmxFlvInfo->u4AudStartPts=u4VidStartPts;
       prSwdmxFlvInfo->fgNeedSetAudStartPts=TRUE;
    }
    else
    {
       _SWDMX_FlvSeek(prSwdmxFlvInfo,prSwdmxInst->rCurRangeReq.u8OffsetI,prSwdmxFlvInfo->i4PrevSpeed,eFLV_SEEK_TYPE_OFFSET);
       u4VidStartPts=prSwdmxInst->rCurRangeReq.u8StartPts;
       prSwdmxFlvInfo->u4AudStartPts=prSwdmxInst->rCurRangeReq.u8StartAudPts;
       if(prSwdmxFlvInfo->u4AudStartPts==0)
       {
         prSwdmxFlvInfo->fgNeedSetAudStartPts=TRUE;
         prSwdmxFlvInfo->u4AudStartPts=u4VidStartPts;
       }
	   else
	   {
		   prSwdmxFlvInfo->fgNeedSetAudStartPts=TRUE;
	   }
    }
	
    prSwdmxFlvInfo->u4RecSeekTime=u4VidStartPts;
    prSwdmxFlvInfo->u4AudStartPts=u4VidStartPts;
    prSwdmxFlvInfo->u4RecSeekAtime=u4VidStartPts;
    prSwdmxFlvInfo->u4CurVidPts=u4VidStartPts;
    
    if(prSwdmxFlvInfo->u4VDecType==ENUM_SWDMX_VDEC_H264)
    {
       fgSentSeqHdr=TRUE;
    }
    LOG(5,"Video Start Render Pts = %lld,Pos=%11d\n",u4VidStartPts,prSwdmxInst->rCurRangeReq.u8OffsetI);
  }
  
  prSwdmxFlvInfo->fgBeginToPlay=FALSE;
  if(prSwdmxFlvInfo->i4PrevSpeed==1000)
  {
      if(prSwdmxFlvInfo->fgEnAudio && prSwdmxFlvInfo->fgEnVideo)
      {
         VDEC_SetRenderFromPts(prSwdmxFlvInfo->u1EsId, prSwdmxFlvInfo->u4CurVidPts);
         VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
         VDEC_Pause(prSwdmxFlvInfo->u1EsId);
         prSwdmxFlvInfo->fgVDecIsPause = TRUE;
         AUD_SetStartPts(prSwdmxInst->u1AdecId,prSwdmxFlvInfo->u4AudStartPts);
         prSwdmxFlvInfo->u4TriggerAudPts = prSwdmxFlvInfo->u4AudStartPts;
      }
      else if(prSwdmxFlvInfo->fgEnAudio)
      {
         AUD_SetStartPts(prSwdmxInst->u1AdecId,prSwdmxFlvInfo->u4AudStartPts);
         prSwdmxFlvInfo->u4AudPts=prSwdmxFlvInfo->u4AudStartPts;
      }
      else if(prSwdmxFlvInfo->fgEnVideo)
      {
         VDEC_StartPTS(prSwdmxFlvInfo->u1EsId, TRUE, 0, &rPTSInfo);
         VDEC_SetRenderFromPts(prSwdmxFlvInfo->u1EsId, prSwdmxFlvInfo->u4CurVidPts);
         VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
         VDEC_Pause(prSwdmxFlvInfo->u1EsId);
         prSwdmxFlvInfo->fgVDecIsPause = TRUE;
      }

      STC_StopStc(prSwdmxInst->u1StcId);
      STC_SetStcValue(prSwdmxInst->u1StcId,flv_max((prSwdmxFlvInfo->u4CurVidPts - 45000),0));
      STC_StartStc(prSwdmxInst->u1StcId);
  }

  if(fgSentSeqHdr)
  {
     x_memcpy((void *)prSwdmxFlvInfo->prFeederInfo->u4StartAddr,(void *)prSwdmxFlvInfo->u4VidHdrPtr,prSwdmxFlvInfo->u4VidHdrLen);
     prSwdmxFlvInfo->u4LastDataLen=prSwdmxFlvInfo->u4VidHdrLen;
	 _SWDMX_DmxTransformData(prSwdmxFlvInfo,eFLV_TAG_TYPE_VID,prSwdmxFlvInfo->prFeederInfo->u4StartAddr, \
     prSwdmxFlvInfo->prFeederInfo->u4EndAddr,prSwdmxFlvInfo->prFeederInfo->u4StartAddr,prSwdmxFlvInfo->u4VidHdrLen,u4VidStartPts);        
  }
  return 0;
}

SWDMX_HDR_PRS_STATE_T _SWDMX_FlvHdrParse(
    UINT8 u1SrcId,
    SWDMX_RANGE_LIST_T* prRangeList,
    SWDMX_CUR_CMD_T *prCmd)
{
    UINT32 ui4Addr, ui4Size, ui4Align;
    UINT32 u4PrsState;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo;
    VDEC_PTS_INFO_T rPTSInfo;
    UINT64 u8SeekPos=0;

    if ((!prSwdmxInst) || (!prCmd))
    {
        return eSWDMX_HDR_PRS_FAIL;
    }
    
    
    prSwdmxFlvInfo= &prSwdmxInst->rFmtInfo.rSwdmxFlvInfo;
    
    if (prSwdmxFlvInfo->u1SrcId >= SWDMX_SOURCE_MAX)
    {
        return eSWDMX_HDR_PRS_FAIL;
    }
    
    prSwdmxFlvInfo->prRangeList=prRangeList;
    prSwdmxFlvInfo->fgPlayAbort=FALSE;

    x_memset((void*)&rPTSInfo, 0, sizeof(VDEC_PTS_INFO_T));
    if (prRangeList->prDmxRangeInfo == NULL)
    {
        LOG(5, "This range is empty\n");
        return eSWDMX_HDR_PRS_EMPTY_RANGE;
    }

    if (!_SWDMX_GetInfo(u1SrcId,eSWDMX_GET_INTERNAL_INFO, (UINT32*)&prSwdmxInst, 0, 0))
    {
        LOG(5, "Fail to Get swdmx internal info\n");
        return eSWDMX_HDR_PRS_FAIL;
    }
    
    if ((prSwdmxInst->eSwDmxPrevousStatus == eSWDMX_STATE_PAUSE) &&
        (prCmd->i4PlaySpeed == 1000) &&
        (prSwdmxFlvInfo->fgPreviouIsSeek))
    {
        LOG(5, "PAUSE -> SEEK -> RESUME set stc value to 0x%08x\n",
            prSwdmxFlvInfo->u4RecSeekTime);
        STC_SetStcValue(prSwdmxInst->u1StcId,prSwdmxFlvInfo->u4RecSeekTime-15000);
        prSwdmxFlvInfo->fgPreviouIsSeek = FALSE;
    }

    if ((prSwdmxFlvInfo->fgEnVideo) && (!prSwdmxFlvInfo->fgEnVDmx))
    {
        UNUSED(_SWDMX_FlvInitDmx(prSwdmxFlvInfo,eSWDMX_MEDIA_VIDEO, NULL, FALSE));
        prSwdmxFlvInfo->fgEnVDmx = TRUE;
    }

    if(prSwdmxFlvInfo->fgEnVideo &&(prSwdmxFlvInfo->u4VidHdrPtr==(UINT32)NULL))
    {
        ui4Addr  = 0;
        ui4Size  = SWDMX_FLV_VIDEO_HEADER_SIZE;
        ui4Align = 16;                    
        
        SWDMX_GetInfo(prSwdmxInst->u1SwdmxId, eSWDMX_GET_INTERNAL_BUF, &ui4Addr, &ui4Size, &ui4Align); 
        prSwdmxFlvInfo->u4VidHdrPtr = ui4Addr;
        
        if(!prSwdmxFlvInfo->u4VidHdrPtr)
        {
            LOG(1, "Allocate Video Header buffer failed!\n");
            ASSERT(0);
            return eSWDMX_HDR_PRS_FAIL;
        }
    }
        
    if ((prSwdmxFlvInfo->fgEnAudio) && (!prSwdmxFlvInfo->fgEnADmx))
    {
        UNUSED(_SWDMX_FlvInitDmx(prSwdmxFlvInfo, eSWDMX_MEDIA_AUDIO, NULL, FALSE));
        prSwdmxFlvInfo->fgEnADmx = TRUE;
    }
#ifdef CC_CMPB_PUSH_MODE
    if(prSwdmxInst->u1PushMode)
    {
       return _SWDMX_FlvHdrParsePushMode(u1SrcId,prRangeList,prCmd);
    }
#endif

    if(prSwdmxFlvInfo->fgEnAudio &&(prSwdmxFlvInfo->u4ADecType==ENUM_SWDMX_ADEC_AAC))
    {
       if(!prSwdmxFlvInfo->pucAacCfg)
       {
            ui4Addr  = 0;
            ui4Size  = SWDMX_FLV_AAC_CONFIG_LEN;        
            ui4Align = 16;
            
            SWDMX_GetInfo(prSwdmxInst->u1SwdmxId, eSWDMX_GET_INTERNAL_BUF, &ui4Addr, &ui4Size, &ui4Align);        
            prSwdmxFlvInfo->pucAacCfg=(UCHAR*)ui4Addr;
            
           if(!prSwdmxFlvInfo->pucAacCfg)
           {
              LOG(1, "Allocate AAC Config buffer failed!\n");
              ASSERT(0);
              return eSWDMX_HDR_PRS_FAIL;
           }
       }
    }

    if (prSwdmxFlvInfo->fgBeginToPlay)
    {
        prSwdmxFlvInfo->i4PrevSpeed=prCmd->i4PlaySpeed;
        _SWDMX_FlvBeginPlay(prSwdmxFlvInfo);
    }

    u8SeekPos=0xffffffff;
    u8SeekPos<<=32;
    u8SeekPos+=0xffffffff;
    if ((prCmd->fgChange) ||
        ((prCmd->fgSeek) &&
         (prCmd->u4SeekMode == SWDMX_CUR_CMD_SEEK_OFFSET)) ||
        (((prSwdmxInst->u8SeekPos>>32)!=0xFFFFFFFF)|| (prSwdmxInst->u8SeekPos&0xffffffff)!=0xFFFFFFFF))
    {   
        FLV_SEEK_CMD eSeekType=FLV_SEEK_TYPE_INVALID;
		UINT32 u4CurrentTime=VDP_GetPts(prSwdmxInst->u1B2rId);
        if((prCmd->fgSeek) &&
           (prCmd->u4SeekMode == SWDMX_CUR_CMD_SEEK_OFFSET))
        {
            eSeekType=FLV_SEEK_TYPE_OFFSET;
            u8SeekPos=prCmd->rSeekOffset.ui8_i_frame_position;
            prSwdmxFlvInfo->u4RecSeekTime=prCmd->rSeekOffset.ui8_pts_info;
            prSwdmxFlvInfo->u4RecSeekAtime=prCmd->rSeekOffset.ui8_aud_pts_info;
            prSwdmxFlvInfo->u4AudStartPts=prSwdmxFlvInfo->u4RecSeekAtime;
            LOG(3,"FLV SeekPos=0x%x,Time=%u\n",(UINT32)u8SeekPos,prSwdmxFlvInfo->u4RecSeekTime);
        }
        else if(((prSwdmxInst->u8SeekPos>>32)!=0xFFFFFFFF)|| (prSwdmxInst->u8SeekPos&0xffffffff)!=0xFFFFFFFF)
        {
            eSeekType=FLV_SEEK_TYPE_PTS;
            u8SeekPos=prSwdmxInst->u8SeekPos;
            prCmd->fgSeek=TRUE;
        }
        if(prCmd->fgSeek)
        {
            _SWDMX_FlvChangeSpeed(prCmd->i4PlaySpeed,prCmd->ePlayMode,u4CurrentTime,u8SeekPos,eSeekType,prSwdmxFlvInfo);
        }

        
        if((prCmd->fgChange&&(!prCmd->fgSeek))||(prCmd->fgSeek&&prCmd->i4PlaySpeed!=prSwdmxFlvInfo->i4PrevSpeed))
        {
           if(prCmd->fgSeek)
           {
             LOG(3,"seek and speed at same time\n");
             u4CurrentTime=prSwdmxFlvInfo->u4RecSeekTime;
           }
           //spead change
           _SWDMX_FlvChangeSpeed(prCmd->i4PlaySpeed,prCmd->ePlayMode,u4CurrentTime,u8SeekPos,FLV_SEEK_TYPE_INVALID,prSwdmxFlvInfo);
        }
        
    }

    if(prSwdmxFlvInfo->fgVDecIsPause)
    {
       _SWDMX_FlvVideoWakeUp(prSwdmxFlvInfo);
    }


#if 0
    if (prSwdmxFlvInfo->prCurVidKeyIdxTbl != 0&&((i4PlaySpeed > 2000) || (i4PlaySpeed < 0)))
    {
        u4PrsState = _SWDMX_FlvHdrPrsUseIdx(prSwdmxInst,prRangeList, i4PlaySpeed, u8SeekPos, fgSpeedChange);
    }
    else
#endif
    {
        u4PrsState = _SWDMX_FlvTagPrs(prSwdmxFlvInfo,prRangeList,prSwdmxFlvInfo->i4PrevSpeed,u8SeekPos,prCmd->fgChange);
    }

    prSwdmxFlvInfo->i4PrevSpeed=prCmd->i4PlaySpeed;
	if(prSwdmxInst->ePlayMode==SWDMX_PLAY_MODE_NORMAL&&prSwdmxFlvInfo->i4PrevSpeed>2000)
	{
        x_thread_delay(10);
	}
    return (SWDMX_HDR_PRS_STATE_T)u4PrsState;
}

VOID _SWDMX_FlvSetRange(
    UINT8 u1SrcId,
    SWDMX_RANGE_LIST_T *prRangeList,
    UINT32 u4VidStrmID,
    UINT32 u4AudStrmID,
    UINT32 u4SubTitleID)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo;
    MM_RANGE_ELMT_IDXTBL_T *pBuldIdxTbl = NULL;
    MM_RANGE_INFO_T * prDmxRangeInfo = NULL;

    SWDMX_FLV_VERIFY_NULL(prSwdmxInst);
    prSwdmxFlvInfo= &prSwdmxInst->rFmtInfo.rSwdmxFlvInfo;
    
    prSwdmxFlvInfo->prRangeList=prRangeList;
    prDmxRangeInfo=prSwdmxFlvInfo->prRangeList->prDmxRangeInfo;
    prSwdmxFlvInfo->u4AudStartPts=(UINT32)prDmxRangeInfo->t_aud_start_pts;
    prSwdmxFlvInfo->u4CurVidPts=(UINT32)prDmxRangeInfo->t_vid_start_pts;
    prSwdmxFlvInfo->u4AudPts=prSwdmxFlvInfo->u4AudStartPts;
    prSwdmxFlvInfo->prRangeList->prDmxRangeInfo->ui8_pb_duration=27000000;
    pBuldIdxTbl=(MM_RANGE_ELMT_IDXTBL_T*)prDmxRangeInfo->pt_idxtbl_lst;
    prSwdmxFlvInfo->prVidKeyIdxTbl = pBuldIdxTbl;
    prSwdmxFlvInfo->u4VidFps=pBuldIdxTbl->ui4_rate;
    if(prDmxRangeInfo->b_thumbnail_pb)
    {
      prDmxRangeInfo->e_mm_src_type=MM_SRC_TYPE_HIGH_SPEED_STORAGE;
	  prDmxRangeInfo->t_vid_start_render_pts=0;
	  prDmxRangeInfo->t_aud_start_pts=0;
	  prSwdmxInst->rCurRangeReq.fgSetRenderPosition=FALSE;
    }

	if(prDmxRangeInfo->t_vid_start_render_pts<90000)
	{
		prDmxRangeInfo->t_vid_start_render_pts=0;
	}

    if(prSwdmxFlvInfo->u4VidFps==0 || prSwdmxFlvInfo->u4VidFps>60)
    {
      prSwdmxFlvInfo->u4VidFps=30;
    }

    LOG(3,"Flv RangeInfo:Size=%d,Time=%d,FrameRate=%d\n",prDmxRangeInfo->z_range_sz,prDmxRangeInfo->ui8_pb_duration,prSwdmxFlvInfo->u4VidFps);
	VDEC_SetTotalTime(prSwdmxFlvInfo->u1EsId, (UINT32)prSwdmxFlvInfo->prRangeList->prDmxRangeInfo->ui8_vid_duration);

    if (prRangeList != NULL && prRangeList->prDmxRangeInfo != NULL)
    {
        if(prDmxRangeInfo->b_key_tbl_exist)
        {
           prSwdmxFlvInfo->fgKeyExist = TRUE;
        }
        else
        {
            prSwdmxFlvInfo->fgKeyExist = FALSE;
        }

        prSwdmxFlvInfo->u4AudStartPts=prDmxRangeInfo->t_aud_start_pts;
        prSwdmxFlvInfo->u4CurVidPts=prDmxRangeInfo->t_vid_start_pts;
        prSwdmxFlvInfo->u4AudPts=prSwdmxFlvInfo->u4AudStartPts;
        UNUSED(VDEC_SetTotalTime(prSwdmxFlvInfo->u1EsId, (UINT32)prSwdmxFlvInfo->prRangeList->prDmxRangeInfo->ui8_vid_duration));
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }
    else
    {
       LOG(0, "Demuxer Range is NULL\n");
    }

  return;
}


BOOL _SWDMX_FlvStop(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo;
    DMX_MM_T rPid;

    SWDMX_FLV_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxFlvInfo= &prSwdmxInst->rFmtInfo.rSwdmxFlvInfo;
    
    rPid.fgEnable = FALSE;
    prSwdmxFlvInfo->fgPlayAbort=TRUE;

    if (prSwdmxFlvInfo->fgEnVDmx)
    {
        prSwdmxFlvInfo->fgEnVDmx = FALSE;
        VERIFY(DMX_MM_Set(prSwdmxFlvInfo->u1VidPid, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(prSwdmxFlvInfo->u1VidPid);
        DMX_MUL_FreePidx(prSwdmxFlvInfo->u1VidPid);
    }

    if (prSwdmxFlvInfo->fgEnADmx)
    {
       prSwdmxFlvInfo->fgEnADmx = FALSE;
       VERIFY(DMX_MM_Set(prSwdmxFlvInfo->u1AudPid, DMX_MM_FLAG_VALID, &rPid));
       DMX_MM_Free(prSwdmxFlvInfo->u1AudPid);
       DMX_MUL_FreePidx(prSwdmxFlvInfo->u1AudPid);
    }

    UNUSED(_SWDMX_FlvInitVar(prSwdmxFlvInfo->u1SrcId));      
    return TRUE;
}


BOOL _SWDMX_FlvDisableStrm(
    UINT8 u1SrcId,
    UINT8 u1StrmType)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo;
    
    SWDMX_FLV_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxFlvInfo= &prSwdmxInst->rFmtInfo.rSwdmxFlvInfo;
    
    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            if(prSwdmxFlvInfo->fgVDecIsPause)
            {
                _SWDMX_FlvVideoWakeUp(prSwdmxFlvInfo);
                prSwdmxFlvInfo->fgVDecIsPause = FALSE;
            }

            prSwdmxFlvInfo->fgEnVideo = FALSE;

            LOG(5, "_SWDMX_FlvDisableStrm Vid\n");
            break;
        case eSWDMX_STRM_TYPE_AUD:
            prSwdmxFlvInfo->fgEnAudio = FALSE;
            LOG(5, "_SWDMX_FlvDisableStrm Aud\n");
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_FlvEnableStrm(
    UINT8 u1SrcId,
    UINT8 u1StrmType,
    VOID* pfnCbFunc)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo;
    
    SWDMX_FLV_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxFlvInfo= &prSwdmxInst->rFmtInfo.rSwdmxFlvInfo;
    
    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            LOG(5, "_SWDMX_FlvEnableStrm Vid\n");
            //Move to _SWDMX_FlvSetRange
            if(prSwdmxFlvInfo->prRangeList && prSwdmxFlvInfo->prRangeList->prDmxRangeInfo)
            {
			  VDEC_SetTotalTime(prSwdmxFlvInfo->u1EsId, (UINT32)prSwdmxFlvInfo->prRangeList->prDmxRangeInfo->ui8_vid_duration);
            }
            prSwdmxFlvInfo->fgEnVideo = TRUE;
            break;
        case eSWDMX_STRM_TYPE_AUD:
            LOG(5, "_SWDMX_FlvEnableStrm Aud\n");
            prSwdmxFlvInfo->fgEnAudio = TRUE;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_FlvGetInfo(
    UINT8 u1SrcId,
    UINT8 u1Type,
    UINT32* pu4Para1,
    UINT32* pu4Para2,
    UINT32* pu4Para3)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo;
    UINT64 *pu8Temp=NULL;
    
    LOG(5, "_SWDMX_FlvGetInfo u1Type = %d\n", u1Type);
    
    SWDMX_FLV_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxFlvInfo= &prSwdmxInst->rFmtInfo.rSwdmxFlvInfo;
    
    if(u1Type == eSWDMX_GET_CURRENT_POSITION)
    {

        if(pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_FlvGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        pu8Temp = (UINT64*)pu4Para1;
        *pu8Temp = prSwdmxFlvInfo->u8CurDmxMoviPos;
        return TRUE;
    }
    else if(u1Type==eSWDMX_GET_IS_TRICK)
    {
        if(pu4Para1==NULL)
        {
          return FALSE;
        }

        if(prSwdmxFlvInfo->fgKeyExist&&prSwdmxFlvInfo->prVidKeyIdxTbl->ui4_number_of_entry>=SWDMX_FLV_INDEX_TRICK_NUM)
        {
            *pu4Para1 = (UINT32)TRUE;
        }
        else
        {
            *pu4Para1 = (UINT32)FALSE;
        }
    }
    else if(u1Type == eSWDMX_GET_AUDIO_ACTIVE_PIDX)
    {
        UINT32 u4AudFifoAddr, u4AudFifoSize,u4DmxAvailSize;
        _SWDMX_GetAudioFifo(prSwdmxInst->u1SwdmxId, &u4AudFifoAddr, &u4AudFifoSize);
        u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, 
                                                                        (DMX_PID_TYPE_T)DMX_PID_TYPE_ES_AUDIO,
                                                                        prSwdmxFlvInfo->u1AudPid);
        
        *pu4Para1 = u4AudFifoSize - u4DmxAvailSize;
        return TRUE;
    }
    return FALSE;
}




BOOL _SWDMX_FlvSetStrmID(UINT8 u1SrcId,UINT8 u1HdrType,
    UINT32 u4StrmID, UINT32 u4StrmSubID)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo;
    UNUSED(u4StrmSubID);
    
    SWDMX_FLV_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxFlvInfo= &prSwdmxInst->rFmtInfo.rSwdmxFlvInfo;
    
    switch(u1HdrType)
    {
        case eSWDMX_STRM_TYPE_VID:
            prSwdmxFlvInfo->u4VStrmID = u4StrmID;
            LOG(5, "_SWDMX_FlvSetStrmID Vid = 0x%08x\n", u4StrmID);
            break;
        case eSWDMX_STRM_TYPE_AUD:
            prSwdmxFlvInfo->u4AStrmID = u4StrmID;
            LOG(5, "_SWDMX_FlvSetStrmID Aud = 0x%08x\n", u4StrmID);
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_FlvSetDecoderInfo(
    UINT8 u1SrcId,
    UINT8 u1Type,
    UINT32 u4Para1,
    UINT32 u4Para2,
    UINT32 u4Para3)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo;
    
    SWDMX_FLV_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxFlvInfo= &prSwdmxInst->rFmtInfo.rSwdmxFlvInfo;
    
    if(u1Type == eSWDMX_SET_VIDEOTYPE)
    {

        #if defined(CC_FLV_H264_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_H264)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_FLV_MJPEG_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MJPEG)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_FLV_SORENSON_DISABLE)
        if(u4Para1==ENUM_SWDMX_VDEC_MPEG4)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_FLV_VP6_DISABLE)
        if(u4Para1==ENUM_SWDMX_VDEC_VP6)
        {
            return FALSE;
        }
        else
        #endif
        {
            prSwdmxFlvInfo->u4VDecType =u4Para1;
            LOG(3,"Flv VdecType=%d\n",prSwdmxFlvInfo->u4VDecType);
            return TRUE;
        }
    }
    else if(u1Type ==eSWDMX_SET_SYSTEMINFO)
    {
        prSwdmxFlvInfo->u4VidFps=u4Para1;
        return TRUE;
    }
    else if (u1Type == eSWDMX_SET_VIDSEQHEADER)
    {
        if (u4Para2 > SWDMX_FLV_VIDEO_HEADER_SIZE)
        {
            LOG(1, "Not enough memory for VIDSEQHEADER\n");
            return FALSE;
        }
        
        if (prSwdmxFlvInfo->u4VidHdrPtr && u4Para1 && u4Para2)
        {
             prSwdmxFlvInfo->u4VidHdrLen = u4Para2;
             x_memcpy((VOID*)VIRTUAL(prSwdmxFlvInfo->u4VidHdrPtr), 
                 (VOID*)VIRTUAL(u4Para1), 
                 prSwdmxFlvInfo->u4VidHdrLen);
             prSwdmxFlvInfo->fgH264SeqHeaderParsed = TRUE;
        }
        
        return TRUE;
    }
    else if(u1Type == eSWDMX_SET_AUDIOTYPE)
    {

        prSwdmxFlvInfo->u4ADecType= (u4Para1==ENUM_SWDMX_ADEC_MPEG) ? ENUM_SWDMX_ADEC_MP3 : u4Para1;
        LOG(3,"Flv AdecType=%d\n",prSwdmxFlvInfo->u4ADecType);
        if(prSwdmxFlvInfo->u4ADecType!=ENUM_SWDMX_ADEC_AAC)
        {
           prSwdmxFlvInfo->u4AudSampleRate=u4Para2;
           prSwdmxFlvInfo->uAudChannelNs=u4Para3;
        }
        return TRUE;
    }
    return FALSE;
}


BOOL _SWDMX_FlvFlush(UINT8 u1SrcId)
{
   SWDMX_INFO_T* prSwdmxInst = NULL;
   SWDMX_FLV_INFO_T *prSwdmxFlvInfo= NULL;

   prSwdmxInst=_SWDMX_GetInst(u1SrcId);

   if(prSwdmxInst)
   {
     prSwdmxFlvInfo=&prSwdmxInst->rFmtInfo.rSwdmxFlvInfo;
   }
   else
   {
     return FALSE;
   }
   
   if(prSwdmxFlvInfo->fgEnAudio&&prSwdmxFlvInfo->fgDeliverAEos)
   {
     prSwdmxFlvInfo->fgDeliverAEos=FALSE;
   }

   if(prSwdmxFlvInfo->fgEnVideo&&prSwdmxFlvInfo->fgDeliverVEos)
   {
	   prSwdmxFlvInfo->fgDeliverVEos=FALSE;
   }
   return TRUE;
}

BOOL _SWDMX_FlvAbort(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo= &prSwdmxInst->rFmtInfo.rSwdmxFlvInfo;
    if(prSwdmxFlvInfo)
    {
       LOG(3,"FLV Play abort\n");
       prSwdmxFlvInfo->fgPlayAbort=TRUE;
    }
    return TRUE;
}
BOOL _SWDMX_FlvInit(UCHAR u1SrcId, ENUM_SWDMX_FMT_T eType)
{
    return _SWDMX_FlvInitVar(u1SrcId);
}

#ifdef CC_CMPB_PUSH_MODE
#define SWDMX_FLV_PARSE_BUFLEN 32
#define FLV_POS_DEC(src,len,startaddr,endaddr) \
{ \
   UINT32 u4DecLen=len; \
   while(u4DecLen>0) \
   { \
     if((UINT32)src==(UINT32)startaddr) \
	 {   \
       src=(UINT32)endaddr;  \
	 }        \
     src--; \
   }  \
}
extern BOOL AUD_DRVGetDecodeType(UINT8 u1DecId,  
	AUD_DEC_STREAM_FROM_T * peStreamFrom,AUD_FMT_T * peDecType);

static UCHAR _aucHeaderBuf[2][SWDMX_FLV_PARSE_BUFLEN];
static UINT32 flv_add_video_frame(sd_demuxer_t *demuxer,UINT32 u4FrameAddr,UINT32 u4Size,UINT32 u4Pts)
{
    UCHAR *puBufStart,*puBufEnd,*puReadPtr,*puStartCode;
	UINT32 u4Temp,u4StartCode;
	UINT8 uByte,uPicType,uCodecType;
    LOG(6, "send video: ns:%05x, pts(ms):%08x\n", demuxer->video->pack_no, u4Pts);
	puBufStart=(UCHAR *)VIRTUAL(demuxer->video->start_pos);
    puBufEnd=(UCHAR *)VIRTUAL(demuxer->video->end_pos);
    puReadPtr=(UCHAR *)VIRTUAL(u4FrameAddr);
	puStartCode=puReadPtr;
	uByte= *puReadPtr;
	uPicType=(uByte&0xf0)>>4;
	uCodecType=uByte&0xf;

	FLV_RPOS_INC(puReadPtr,1,puBufStart,puBufEnd);
	
	if(demuxer->video->dec_type==ENUM_SWDMX_VDEC_H264)
	{
	
	   dmx_es_comm comm_es;
       comm_es.pts       = u4Pts;
		uByte= *puReadPtr;
	    FLV_RPOS_INC(puReadPtr,1,puBufStart,puBufEnd);
		FLV_INTEGER(u4Temp,puReadPtr,3,puBufStart,puBufEnd);
		u4StartCode=0x00000001;
		FLV_SET_INTEGER(u4StartCode,puStartCode,4,puBufStart,puBufEnd);//fill start code
		if(uByte==0)
		{
			UINT16 u2PSLength = 0;
			UINT8 u1NumOfPS = 0, u1PSNum = 0;
			UINT8  i=0;
			LOG(1, "_SWDMX_FlvVidCb Get H264 Sequence Header\n");
			FLV_RPOS_INC(puReadPtr,5,puBufStart,puBufEnd);
			for(u1PSNum = 0; u1PSNum < 2; u1PSNum++)	// 0 for SPS, 1 for PPS
			{
			   FLV_INTEGER(u1NumOfPS,puReadPtr,1,puBufStart,puBufEnd);
			   u1NumOfPS= (u1PSNum==0) ? (u1NumOfPS&0x1f) : u1NumOfPS;
			   for(i=0;i<u1NumOfPS;i++)
			   {
				 FLV_INTEGER(u2PSLength,puReadPtr,2,puBufStart,puBufEnd);
				 FLV_RPOS_INC(puReadPtr,1,puBufStart,puBufEnd);
				 comm_es.addr=(UINT32)puReadPtr;
				 FLV_POS_DEC(comm_es.addr,4,puBufStart,puBufEnd);
				 decoder_add_vframe(demuxer, (void*)&comm_es);
				 FLV_RPOS_INC(puReadPtr,u2PSLength-1,puBufStart,puBufEnd);
			   }
			}
			return TRUE;
		 }
		 else if(uByte==1)
		 {
		    UINT32 i4PtsOffset=0,u4TotalNalLen=0,u4NALLen=0;
			i4PtsOffset=(u4Temp+0xff800000)^0xff800000;
		    if(i4PtsOffset>0)
			{
			   comm_es.pts =comm_es.pts +i4PtsOffset*90;
			}
	
			u4TotalNalLen=5;
			while(u4TotalNalLen<u4Size)
			{
			   puStartCode=puReadPtr;
			   FLV_INTEGER(u4NALLen,puReadPtr,4,puBufStart,puBufEnd);
			   FLV_SET_INTEGER(u4StartCode,puStartCode,4,puBufStart,puBufEnd);//fill start code
			   u4TotalNalLen+=4;
			   FLV_RPOS_INC(puReadPtr,1,puBufStart,puBufEnd);
			   comm_es.addr=(UINT32)puReadPtr;
			   if((u4TotalNalLen+u4NALLen)>u4Size)
			   {
				  LOG(0,"Send NAL error\n");
				  return FALSE;
			   }
			    HalFlushInvalidateDCache();
				decoder_add_vframe(demuxer, (void*)&comm_es);
				u4TotalNalLen+=u4NALLen;
				FLV_RPOS_INC(puReadPtr,(u4NALLen-1),puBufStart,puBufEnd);
			}
		 }
	
	  }
	  else  // Sorenson/mp4/vp6
      {
      	   dmx_es_mpeg4 comm_es;
           comm_es.pts       = u4Pts;
		   comm_es.no_vdec_cal_pts=TRUE;
		  if(uPicType==1)
		  {
			 comm_es.type = I_TYPE;
		  }
		  else if(uPicType==2)
		  {
			 comm_es.type = P_TYPE;
		  }
		  else if(uPicType==3)
		  {
			  comm_es.type = I_TYPE;
		  }
		  else
		  {
			  LOG(1, "_SWDMX_FlvVidCb UnknowPicType:%d\n",uByte&0xf0);
  		      comm_es.type = UNKNOWN_TYPE;
		  }
	
		  if(demuxer->video->dec_type==ENUM_SWDMX_VDEC_VP6)
		  {
	
			   if(uCodecType==SWDMX_FLV_CODEC_VP6)
			   {
				  FLV_RPOS_INC(puReadPtr,1,puBufStart,puBufEnd);
				  //do nothing
			   }
			   else if(uCodecType==SWDMX_FLV_CODEC_VP6A)
			   {
				  FLV_RPOS_INC(puReadPtr,4,puBufStart,puBufEnd); // alpha data offset
			   }
			   else
			   {
				 LOG(3,"Error Vpt codec type\n");
				 return TRUE;
			   }
		 }
		 comm_es.addr=(UINT32)puReadPtr;
		 decoder_add_vframe(demuxer, (void*)&comm_es);
	}

    return 0;
}

static UINT32 _SWDMX_FlvErrRecover(sd_demuxer_t *demuxer, dmx_stream_t * dmx_stream)
{
   SWDMX_FLV_INFO_T *prSwdmxFlvInfo=NULL;
   UCHAR *puParseBuf=NULL;
   UINT32 u4DataAvaiable,u4ReadeLen=0,u4TotalParaeLen=0,u4Index=0;
   UINT32 u4ParseLen;
   sd_stream_t *prStream = NULL;
   ASSERT(demuxer);
   prSwdmxFlvInfo=(SWDMX_FLV_INFO_T*)demuxer->priv;
   puParseBuf=_aucHeaderBuf[prSwdmxFlvInfo->u1EsId];
   prStream= demuxer->stream;
   
   u4DataAvaiable=sd_stream_capacity(prStream);
   if(u4DataAvaiable<SWDMX_FLV_TAG_HEADER_SIZE)
   {
       if(prStream->eof)
       {
           prSwdmxFlvInfo->fgErrorConvery=FALSE;
           sd_stream_seek(prStream,dmx_stream->end_pos);
           LOG(3,"Error Recover File end0\n");
           return 0;
       }
   }
   
   while(u4TotalParaeLen<64*1024 && u4DataAvaiable>SWDMX_FLV_TAG_HEADER_SIZE) // error
   {
     u4ReadeLen=MIN(u4DataAvaiable,SWDMX_FLV_PARSE_BUFLEN);
     if(u4ReadeLen>=SWDMX_FLV_TAG_HEADER_SIZE)
     {
         sd_stream_peek_xx(prStream,puParseBuf,u4ReadeLen);
         u4ParseLen=u4ReadeLen-SWDMX_FLV_TAG_HEADER_SIZE+1;
         u4Index=0;
         while(u4Index<u4ParseLen)
         {
            if(puParseBuf[u4Index]==eFLV_TAG_TYPE_AUD
               ||puParseBuf[u4Index]==eFLV_TAG_TYPE_VID
               ||puParseBuf[u4Index]==eFLV_TAG_TYPE_DATA)
            {
               if(puParseBuf[u4Index+7]==0&&puParseBuf[u4Index+8]==0 
                &&puParseBuf[u4Index+9]==0 && puParseBuf[u4Index+10]==0)
                {
                   break;
                }
            }
            else
            {
               u4Index++;
            }
         }
         
         if(u4Index!=u4ParseLen)
         {
            sd_stream_skip(prStream,u4Index);
            prSwdmxFlvInfo->fgErrorConvery=FALSE;
            LOG(3,"Error Recover success \n");
            break;
         }
         else
         {
            u4TotalParaeLen+=u4ParseLen;
            u4DataAvaiable-=u4ParseLen;
            sd_stream_skip(prStream,u4ParseLen);
         }         
     }
     else
     {
       if(prStream->eof)
       {
           prSwdmxFlvInfo->fgErrorConvery=FALSE;
           sd_stream_seek(prStream,dmx_stream->end_pos);
           LOG(3,"Error Recover File end\n");
       }
       break;
     }
   }

   return 0;
}

static UINT32 _SWDMX_FlvAVDmx(sd_demuxer_t *demuxer, dmx_stream_t * dmx_stream)
{
  SWDMX_FLV_INFO_T *prSwdmxFlvInfo=NULL;
  UINT32 u4DataValue=0,u4SendDataLen=0,u4TotalSendData=0;
  sd_stream_t *prStream = NULL;
  UCHAR *puParseBuf=NULL;
  DMX_MM_DATA_T rDmxMMData;
  UINT32 u4DmxAvaiableData;
  ASSERT(demuxer);
  prSwdmxFlvInfo=(SWDMX_FLV_INFO_T*)demuxer->priv;
  prStream= demuxer->stream;
  puParseBuf=_aucHeaderBuf[prSwdmxFlvInfo->u1EsId];
  if(dmx_stream)
  {
      sd_stream_seek(prStream, dmx_stream->curr_pos);
  }
  
  do
  {
     // check data enough or not.
     if(sd_stream_capacity(prStream)<SWDMX_FLV_TAG_HEADER_SIZE)
     {  
        goto DataNotEnough;
     }
	 
     if(prSwdmxFlvInfo->uLastTagType==eFLV_TAG_TYPE_UNKNOW)
     {
		 sd_stream_skip(prStream,SWDMX_FLV_TAG_ENDSIZE);
     }
	 
     sd_stream_peek_xx(prStream,puParseBuf,SWDMX_FLV_TAG_HEADER_SIZE);
	 
     prSwdmxFlvInfo->uLastTagType=puParseBuf[0];
	 if((prSwdmxFlvInfo->uLastTagType!=eFLV_TAG_TYPE_AUD)
        &&(prSwdmxFlvInfo->uLastTagType!=eFLV_TAG_TYPE_VID)
        &&(prSwdmxFlvInfo->uLastTagType!=eFLV_TAG_TYPE_DATA))
     {
         LOG(3,"FLV push data error1\n");
         goto DataError;
     }
     
     u4DataValue=(puParseBuf[1]<<16)+(puParseBuf[2]<<8)+puParseBuf[3]; // tag size
     if(sd_stream_capacity(prStream)<u4DataValue+SWDMX_FLV_TAG_HEADER_SIZE+SWDMX_FLV_TAG_ENDSIZE)
     {
        goto DataNotEnough;
     }
     
     x_memset(&rDmxMMData,0,sizeof(DMX_MM_DATA_T));
     // start parse
     //sd_stream_read_xx(prStream,puParseBuf,SWDMX_FLV_TAG_HEADER_SIZE);
    
	 prSwdmxFlvInfo->u4LastDataLen=(puParseBuf[1]<<16)+(puParseBuf[2]<<8)+puParseBuf[3];
	 prSwdmxFlvInfo->u4LastPts=(puParseBuf[7]<<24)+(puParseBuf[4]<<16)+(puParseBuf[5]<<8)+puParseBuf[6];
     prSwdmxFlvInfo->u4LastPts*=SWDMX_FLV_PTS_FACTOR;
     u4DataValue=(puParseBuf[8]<<16)+(puParseBuf[9]<<8)+puParseBuf[10];

     if(u4DataValue!=0)
     {
        LOG(3,"FLV push data error2\n");
        goto DataError;
     }

     u4SendDataLen=prSwdmxFlvInfo->u4LastDataLen;

     if(prSwdmxFlvInfo->uLastTagType==eFLV_TAG_TYPE_AUD)
     {
        prSwdmxFlvInfo->u4AudPts=prSwdmxFlvInfo->u4LastPts;
	    u4DmxAvaiableData = DMX_MUL_GetEmptySize(prSwdmxFlvInfo->u1SrcId, DMX_PID_TYPE_ES_AUDIO,demuxer->audio->dmx_mul_pid);
        if(prSwdmxFlvInfo->u4ADecType==ENUM_SWDMX_ADEC_AAC)
        { 
            if(u4DmxAvaiableData<SWDMX_FLV_AAC_FAKE_HDR_SIZE)
            {
				goto MoveDataFail;
            }
			else
			{
				u4DmxAvaiableData-=SWDMX_FLV_AAC_FAKE_HDR_SIZE;
			}
        }
	 }
     else if(prSwdmxFlvInfo->uLastTagType==eFLV_TAG_TYPE_VID)
     {
        prSwdmxFlvInfo->u4CurVidPts=prSwdmxFlvInfo->u4LastPts;
	    u4DmxAvaiableData = DMX_MUL_GetEmptySize(prSwdmxFlvInfo->u1SrcId, DMX_PID_TYPE_ES_AUDIO,demuxer->video->dmx_mul_pid);
     } 
     else
     {
		sd_stream_skip(prStream,SWDMX_FLV_TAG_HEADER_SIZE);
        sd_stream_skip(prStream,prSwdmxFlvInfo->u4LastDataLen+SWDMX_FLV_TAG_ENDSIZE);
        u4SendDataLen=0;
        continue;
     }
	 if(u4DmxAvaiableData<u4SendDataLen)
     {
	   goto MoveDataFail;
     }
	 else
	 {
		 sd_stream_skip(prStream,SWDMX_FLV_TAG_HEADER_SIZE);
	 }
	 
     if(prSwdmxFlvInfo->uLastTagType==eFLV_TAG_TYPE_AUD)
     {
        sd_stream_skip(prStream,1);
        u4SendDataLen--;
        if(prSwdmxFlvInfo->u4ADecType==ENUM_SWDMX_ADEC_AAC)
        {
           sd_stream_read_xx(prStream,puParseBuf,1);
           u4SendDataLen--;
           if(puParseBuf[0]==0)  // aac config table
           {
              UCHAR *pucPos=NULL,*puStart=NULL,*puEnd=NULL;
              UINT32 u4Freq=0,u4ReadLen=0;
              UINT8 uData=0,uBitShift=0;
              u4ReadLen=MIN(u4SendDataLen,5);
              sd_stream_read_xx(prStream,puParseBuf,u4ReadLen);
              u4SendDataLen-=u4ReadLen;
              pucPos=puParseBuf;
              puStart=puParseBuf;
              puEnd=puParseBuf+u4ReadLen;
              uBitShift=5;
              FLV_INTEGER_BIT(uData,pucPos,uBitShift,4,puStart,puEnd);
              if(uData==0xf)
              {
                 FLV_INTEGER_BIT(u4Freq,pucPos,uBitShift,24,puStart,puEnd);
              }
              else
              {
                if(uData<SWDMX_FLV_AAC_DEF_FREQ_NUM)
                {
                  u4Freq=_rAacFreqTbl[uData];
                }
                else
                {
                  u4Freq=44100;
                }
              }
               
              if(u4Freq!=0)
              {
                prSwdmxFlvInfo->u4AudSampleRate=u4Freq;
              }
              else if(prSwdmxFlvInfo->u4AudSampleRate==0)
              {
                prSwdmxFlvInfo->u4AudSampleRate=441000;
              }
                
              FLV_INTEGER_BIT(uData,pucPos,uBitShift,4,puStart,puEnd);
              if(uData>0 && uData<=SWDMX_FLV_AAC_CHANNEL_CFG_NUM)
              {
                prSwdmxFlvInfo->uAudChannelNs=_rAacChCfgTbl[uData-1];
              }
              else
              {
                prSwdmxFlvInfo->uAudChannelNs=2;
              }
              
              prSwdmxFlvInfo->fgAacSendHeader=TRUE;
              if(u4SendDataLen>0)
              {
                sd_stream_skip(prStream,u4SendDataLen);
              }
              u4SendDataLen=0;
           }
           else
           {
              if(_SWDMX_FlvSendAacHeader(prSwdmxFlvInfo)!=eSWDMX_HDR_PRS_SUCCEED)
              {
                goto MoveDataFail;
              }
           }

        }
     }

     if(u4SendDataLen)
     {
         rDmxMMData.u4FrameSize = u4SendDataLen;
         rDmxMMData.u4Pts = prSwdmxFlvInfo->u4LastPts;
         if(prSwdmxFlvInfo->uLastTagType==eFLV_TAG_TYPE_AUD)
         {
            rDmxMMData.u1Idx = demuxer->audio->dmx_mul_pid;
            u4DmxAvaiableData = DMX_MUL_GetEmptySize(prSwdmxFlvInfo->u1SrcId, DMX_PID_TYPE_ES_AUDIO, rDmxMMData.u1Idx);
            if(u4DmxAvaiableData<u4SendDataLen)
            {
               goto MoveDataFail;
            }
            
            sd_stream_move_data(demuxer->stream, SD_STREAM_MOVE_AUDIO, u4SendDataLen,&rDmxMMData);
            demuxer->audio->pack_no++;
         }
         else if(prSwdmxFlvInfo->uLastTagType==eFLV_TAG_TYPE_VID)
         {
            UINT32 u4WriteAddr;
            rDmxMMData.u1Idx = demuxer->video->dmx_mul_pid;
			u4WriteAddr=new_dmx_frame(demuxer->video, u4SendDataLen, NULL);
            u4DmxAvaiableData = DMX_MUL_GetEmptySize(prSwdmxFlvInfo->u1SrcId, DMX_PID_TYPE_ES_VIDEO, rDmxMMData.u1Idx);
            if(u4DmxAvaiableData<u4SendDataLen)
            {
               goto MoveDataFail;
            }
            sd_stream_move_data(demuxer->stream, SD_STREAM_MOVE_VIDEO, u4SendDataLen,&rDmxMMData);
			flv_add_video_frame(demuxer,u4WriteAddr,u4SendDataLen,rDmxMMData.u4Pts);
            demuxer->video->pack_no++;
         }
         u4TotalSendData+=u4SendDataLen;
         u4SendDataLen=0;
     }
     
     sd_stream_read_xx(prStream,puParseBuf,4);
     u4DataValue=(puParseBuf[0]<<24)+(puParseBuf[1]<<16)+(puParseBuf[2]<<8)+puParseBuf[3];
     if(u4DataValue!=(prSwdmxFlvInfo->u4LastDataLen+SWDMX_FLV_TAG_HEADER_SIZE))
     {
        LOG(3,"FLV push data error3\n");
        goto DataError;
     }
     //ASSERT(u4DataValue==prSwdmxFlvInfo->u4LastDataLen+SWDMX_FLV_TAG_ENDSIZE);
     if(u4DataValue>=SWDMX_FLV_MOVI_MAX)
     {
        break;
     }
     continue;
     
MoveDataFail:
     break;
DataNotEnough:
     if(sd_stream_eof(prStream))
     {
        sd_stream_skip(prStream,sd_stream_capacity(prStream));
     }
     break;
DataError:
    prSwdmxFlvInfo->fgErrorConvery=TRUE;
    break;
  }
  while(1);
  
  return 0;
}

static SWDMX_HDR_PRS_STATE_T _SWDMX_FlvHdrParsePushMode(
    UINT8 u1SrcId,
    SWDMX_RANGE_LIST_T* prRangeList,
    SWDMX_CUR_CMD_T *prCmd)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo;
    prSwdmxFlvInfo= &prSwdmxInst->rFmtInfo.rSwdmxFlvInfo;

	if(prSwdmxInst==NULL)
	{    	 
	   ASSERT(0);
	   return eSWDMX_HDR_PRS_FAIL;
    }
	
    if(prSwdmxFlvInfo->fgBeginToPlay)
    {
       if(prSwdmxFlvInfo->fgEnAudio && prSwdmxFlvInfo->fgEnVideo)
       {
           VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
           VDEC_Pause(prSwdmxInst->u1VdecId);
           prSwdmxFlvInfo->fgVDecIsPause=TRUE;
           prSwdmxFlvInfo->u4TriggerAudPts=prRangeList->u8StartPts;
           AUD_SetStartPts(prSwdmxInst->u1AdecId, prRangeList->u8StartAudPts);
           VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId,prRangeList->u8StartPts);
       }
       else if(prSwdmxFlvInfo->fgEnAudio)  // audio only
       {
           AUD_SetStartPts(prSwdmxInst->u1AdecId, prRangeList->u8StartPts);
       }
       else if(prSwdmxFlvInfo->fgEnVideo)
       {

          VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
          VDEC_Pause(prSwdmxInst->u1VdecId);
          prSwdmxFlvInfo->fgVDecIsPause=TRUE;
          VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId,prRangeList->u8StartPts);
       }
       
       //connect to  stream and demxuer module begin +++++++++++++++++++++++++++++++++++
	   if (prSwdmxFlvInfo->prRangeList->prDmxRangeInfo->e_mm_src_type == MM_SRC_TYPE_PUSH)
	   {
		   sd_stream_set_input_mode(prSwdmxInst->demuxer.stream, SD_STREAM_PUSH);
	   }
       sd_stream_reset(prSwdmxInst->demuxer.stream);
       prSwdmxInst->demuxer.stream->start_fpos = prRangeList->prDmxRangeInfo->ui8_fileoffset;    
       prSwdmxInst->demuxer.stream->end_fpos   = prRangeList->prDmxRangeInfo->ui8_fileoffset + prRangeList->prDmxRangeInfo->z_range_sz;
       prSwdmxInst->demuxer.stream->fpos       = prSwdmxInst->demuxer.stream->start_fpos;
       prSwdmxInst->demuxer.stream->state = SD_STREAM_ST_RUN;
       
       if (prSwdmxInst->demuxer.stream->stream_type  == SD_STREAM_TYPE_RING)
       {
           FEEDER_BUF_INFO_T type;
           FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &type);
           prSwdmxInst->demuxer.stream->priv.u.ring.start_addr = VIRTUAL(type.u4StartAddr);
           prSwdmxInst->demuxer.stream->priv.u.ring.end_addr   = VIRTUAL(type.u4EndAddr);
       }
       
       //-demxuer stream video
       if (prSwdmxFlvInfo->fgEnVDmx)
       {
           UINT32 u4Addr, u4Size;
           prSwdmxInst->demuxer.video = new_dmx_stream();
           prSwdmxInst->demuxer.video->curr_pos  = prSwdmxInst->demuxer.stream->fpos;
           prSwdmxInst->demuxer.video->start_pos = prSwdmxInst->demuxer.stream->start_fpos;
           prSwdmxInst->demuxer.video->end_pos  = prSwdmxInst->demuxer.stream->end_fpos;
           prSwdmxInst->demuxer.video->pack_no  = 0;
           prSwdmxInst->demuxer.video->last_pts = 0;
           prSwdmxInst->demuxer.video->prs_on = TRUE;
           prSwdmxInst->demuxer.video->dmx_mul_pid = prSwdmxFlvInfo->u1VidPid;
           _SWDMX_GetVideoFifo(prSwdmxInst->u1SwdmxId, &u4Addr, &u4Size);
           prSwdmxInst->demuxer.video->fifo_size = u4Size;
		   prSwdmxInst->demuxer.video->start_pos=u4Addr;
		   prSwdmxInst->demuxer.video->end_pos=u4Addr+u4Size;
           prSwdmxInst->demuxer.video->dec_type = (UINT8)prSwdmxFlvInfo->u4VDecType;
		   prSwdmxInst->demuxer.vdec_info.fifo_start=u4Addr;
		   prSwdmxInst->demuxer.vdec_info.fifo_end=u4Addr+u4Size;
		   prSwdmxInst->demuxer.srch_sc_mode=SD_DMX_SRCH_SC_NONE;
       }
       //-demxuer stream audio
       if (prSwdmxFlvInfo->fgEnADmx)
       {
           UINT32 u4Addr, u4Size;
           prSwdmxInst->demuxer.audio = new_dmx_stream();
           prSwdmxInst->demuxer.audio->curr_pos  = prSwdmxInst->demuxer.stream->fpos;
           prSwdmxInst->demuxer.audio->start_pos = prSwdmxInst->demuxer.stream->start_fpos;
           prSwdmxInst->demuxer.audio->end_pos  = prSwdmxInst->demuxer.stream->end_fpos;
           prSwdmxInst->demuxer.audio->pack_no  = 0;
           prSwdmxInst->demuxer.audio->last_pts = 0;
           prSwdmxInst->demuxer.audio->prs_on = TRUE;
           prSwdmxInst->demuxer.audio->dmx_mul_pid = prSwdmxFlvInfo->u1AudPid;
           _SWDMX_GetAudioFifo(prSwdmxInst->u1SwdmxId, &u4Addr, &u4Size);
           prSwdmxInst->demuxer.audio->fifo_size = u4Size;
		   prSwdmxInst->demuxer.audio->start_pos=u4Addr;
		   prSwdmxInst->demuxer.audio->end_pos=u4Addr+u4Size;
		   if(prSwdmxFlvInfo->u4ADecType==ENUM_SWDMX_ADEC_UNKNOWN)
		   {
		      AUD_DEC_STREAM_FROM_T eStreamFrom;
			  AUD_FMT_T eDecType;
		      AUD_DRVGetDecodeType(prSwdmxInst->u1AdecId,&eStreamFrom,&eDecType);
              if(eDecType==AUD_FMT_AAC)
              {
                 prSwdmxFlvInfo->u4ADecType=ENUM_SWDMX_ADEC_AAC;
              }
		   }
           prSwdmxInst->demuxer.audio->dec_type = (UINT8)prSwdmxFlvInfo->u4ADecType;

       }

       prSwdmxInst->pfnSwdmxSendVideoPes = NULL; 
       STC_StopStc(prSwdmxInst->u1StcId);
       STC_SetStcValue(prSwdmxInst->u1StcId, (UINT32)(0-3000));
       prSwdmxFlvInfo->u8CurDmxMoviPos=prRangeList->prDmxRangeInfo->ui8_fileoffset;
       prSwdmxFlvInfo->u8CurMoviPos = prSwdmxFlvInfo->u8CurDmxMoviPos;
       prSwdmxInst->demuxer.priv=(VOID *)prSwdmxFlvInfo;
       prSwdmxFlvInfo->fgBeginToPlay=FALSE;
	   prSwdmxFlvInfo->uLastTagType=eFLV_TAG_TYPE_UNKNOW;
    }
    
    if(prSwdmxFlvInfo->fgEnVideo&&prSwdmxFlvInfo->fgVDecIsPause)
    {
       _SWDMX_FlvVideoWakeUp(prSwdmxFlvInfo);
    }

    /*
    if(prSwdmxInst->demuxer.stream->fpos >= prSwdmxInst->demuxer.stream->end_fpos)
    {
       prSwdmxInst->demuxer.audio->prs_on = FALSE;
       prSwdmxInst->demuxer.video->prs_on = FALSE;    
       return (SWDMX_HDR_PRS_STATE_T)_SWDMX_FlvCheckEos(prSwdmxFlvInfo);
    }
    */
    sd_stream_state(prSwdmxInst->demuxer.stream);
    if(prSwdmxFlvInfo->fgErrorConvery)
    {
        swdmx_demuxer(&prSwdmxInst->demuxer,_SWDMX_FlvErrRecover);
    }
    else
    {
        swdmx_demuxer(&prSwdmxInst->demuxer,_SWDMX_FlvAVDmx);
    }


    // if demuxing is done
    if (sd_stream_eof(prSwdmxInst->demuxer.stream) && sd_stream_capacity(prSwdmxInst->demuxer.stream) == 0)
    {
    
        sd_demuxer_t *pt_demuxer = &prSwdmxInst->demuxer;
        prSwdmxInst->rCurRangeReq.eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        if (pt_demuxer->video->prs_on)
        {
            VDEC_PES_INFO_T rPesInfo;
            rPesInfo.fgEos   = TRUE;
            rPesInfo.ucEsId  = prSwdmxInst->u1VdecId;
            rPesInfo.ucMpvId = VLD0;
            VDEC_SendEs((void*)&rPesInfo);
            pt_demuxer->video->prs_on = FALSE;
        }

        if (pt_demuxer->audio->prs_on)
        {
		    DMX_MM_DATA_T rDmxMMData;
            FEEDER_BUF_INFO_T *prFeederInfo = &prSwdmxInst->rFeederInfo;
            FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo);
            
            rDmxMMData.u4BufStart = prFeederInfo->u4StartAddr;
            rDmxMMData.u4BufEnd = prFeederInfo->u4EndAddr;
            rDmxMMData.u4StartAddr = prFeederInfo->u4StartAddr;
            rDmxMMData.u4FrameSize = 4;
            rDmxMMData.fgEOS = TRUE;
            rDmxMMData.u1Idx = pt_demuxer->audio->dmx_mul_pid;
            _SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData);
            AUD_MM_Set_Avsync_event(prSwdmxInst->u1AdecId);
            pt_demuxer->audio->prs_on = FALSE;
        }
    }
    
    return eSWDMX_HDR_PRS_SUCCEED;
}
#endif

