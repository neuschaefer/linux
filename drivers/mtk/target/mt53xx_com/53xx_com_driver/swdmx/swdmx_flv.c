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


//#define FLV_USE_DRV_BLD
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define min(x, y) (((x) < (y)) ? (x) : (y))
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define abs(x) (((x) >= 0) ? (x) : -(x))

#define SWDMX_MAYBY
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

#define SWDMX_FLV_INVALID_PTS 0xffffffff
#define SWDMX_FLV_AAC_CONFIG_LEN (512)
#define SWDMX_FLV_AAC_FAKE_HDR_SIZE (7)
#define SWDMX_FLV_SEEK_REQ_SIZE (512)
#define SWDMX_FLV_AAC_DEF_FREQ_NUM 16
#define SWDMX_FLV_AAC_CHANNEL_CFG_NUM 7
#define SWDMX_FLV_ERRCOV_IDENTIFY_OFFSET 8
#define SWDMX_FLV_STREAM_ID (0)
#define SWDMX_FLV_INDEX_TRICK_NUM (8)
#define FLV_RPOS_INC(src,len,startaddr,endaddr) (src= (src+(len)>=endaddr) ? (startaddr+((src)+(len)-(endaddr))) : ((src)+(len)))

#define FLV_SET_INTEGER(integer,src,len,stataddr,endaddr) \
{ \
    UCHAR uByte=0,uIntLen=0; \
    uIntLen=len;  \
    while(uIntLen>0) \
    {  \
       uByte=(integer>>((uIntLen-1)*8))&0xff; \
       *src=uByte; \
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
      uByte=*src; \
      integer=(integer<<8)|uByte; \
      uIntLen++;\
      FLV_RPOS_INC(src,1,startaddr,endaddr); \
   } \
}

#define FLV_INTEGER_NEXT(integer,src,len,startaddr,endaddr) \
{  \
  UCHAR uByte,uIntLen=0,*tmpsrc; \
  integer=0; \
  tmpsrc=src; \
  while(uIntLen<len) \
  {  \
     uByte=*tmpsrc; \
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
   uByte=*src; \
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
        uByte=*src; \
        integer=(integer<<8)|uByte; \
        uIntLen--; \
        FLV_RPOS_INC(src,1,saddr,eaddr); \
     } \
     if(uBits) \
     { \
        uByte=*src; \
        integer=(integer<<uBits)|((uByte>>(8-uBits))&((1<<uBits)-1)); \
     } \
   } \
   sbit=(sbit+bitlen)%8; \
}

//#define INPUT_SRC prSwdmxFlvInfo->prRangeList.prDmxRangeInfo->e_mm_src_type
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//static SWDMX_RANGE_LIST_T *_prRangeList = NULL;


//static UCHAR *_pucFlvAacBuf = NULL;
static UCHAR _aucHeaderBuf[SWDMX_FLV_TAG_HEADER_SIZE+SWDMX_FLV_TAG_ENDSIZE]={0};
static SWDMX_FLV_INFO_T rSwdmxFlvInfo;
static UCHAR *_pucFlvAudBuf = NULL;
static UCHAR *_pucFlvAAcCfg=NULL;
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

INT32 _SWDMX_FlvSetDebugCounter(INT32 i4Counter)
{
   u4DebutCounter=i4Counter;
   return 0;
}

static INT32 _SWDMX_FlvDmxRest(SWDMX_FLV_INFO_T *prSwdmxFlvInfo,UINT32 u4Type)
{
   UINT32 u4Flag=0;
   DMX_MM_T rDmxMMInfo;
   rDmxMMInfo.fgEnable = TRUE;
   rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;
   if(u4Type==eSWDMX_MEDIA_VIDEO)
   	{
	   switch (prSwdmxFlvInfo->u4VDecType)
	   {
		 case ENUM_SWDMX_VDEC_MPEG4:
		 case ENUM_SWDMX_VDEC_MJPEG:
         case ENUM_SWDMX_VDEC_H264:
				  LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = FALSE\n");
				  rDmxMMInfo.fgSearchStartCode = FALSE;
				  break;
		 default:

                  u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
				  LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = TRUE\n");
				  rDmxMMInfo.fgSearchStartCode = TRUE;
				  break;
	   }

	   if (!DMX_MM_Set(eSWDMX_MEDIA_VIDEO, u4Flag, &rDmxMMInfo))
	   {
					LOG(3, "Fail to reset demuxer\n");
	   }
   	}
    else if(u4Type==eSWDMX_MEDIA_AUDIO)
   	{
	   AUD_ClearEOSFlag(0);
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
                u4MaxRepeatFrame = (prSwdmxFlvInfo->u4VidFps * 1000 * 2)/abs(i4PlaySpeed);

                if (i4PlaySpeed > 2000)
                {
                    if(u4CurKey < prKeyIdxTbl->ui4_number_of_entry && prKeyIdxTbl->ui4_number_of_entry>10 )
                    {
                        u4PtsDif = prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts -prKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxFlvInfo->u4CurRepeatFrameIdx].ui4_pts;
                        prSwdmxFlvInfo->u4VidRepeatFrame = (UINT32)((u4PtsDif * prSwdmxFlvInfo->u4VidFps)/(90 * i4PlaySpeed));
                    }
                    else
                    {
                        prSwdmxFlvInfo->u4VidRepeatFrame = (prSwdmxFlvInfo->u4VidFps * 1000)/abs(i4PlaySpeed);
                    }
                }
                else if (i4PlaySpeed < 0)
                {
                    if ((prKeyIdxTbl->ui4_number_of_entry > 10))
                    {
                        u4PtsDif = prKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxFlvInfo->u4CurRepeatFrameIdx].ui4_pts -prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts;
                        prSwdmxFlvInfo->u4VidRepeatFrame = (UINT32)((u4PtsDif * prSwdmxFlvInfo->u4VidFps)/(90 * abs(i4PlaySpeed)));
                    }
                    else
                    {
                        prSwdmxFlvInfo->u4VidRepeatFrame = (prSwdmxFlvInfo->u4VidFps * 1000)/abs(i4PlaySpeed);
                    }
                }

                prSwdmxFlvInfo->u4VidRepeatFrame = min(prSwdmxFlvInfo->u4VidRepeatFrame,u4MaxRepeatFrame);

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
               prSwdmxFlvInfo->u4VidRepeatFrame = (prSwdmxFlvInfo->u4VidFps * 1000)/abs(i4PlaySpeed);
               return FALSE;
            }
        }
        else
        {
            prSwdmxFlvInfo->u4VidRepeatFrame = (prSwdmxFlvInfo->u4VidFps * 1000)/abs(i4PlaySpeed);
        }

        return FALSE;
    }

    prSwdmxFlvInfo->u4VidRepeatFrame--;
    return TRUE;
}

static INT32 _SWDMX_FlvChangeSpeed(INT32 i4PlaySpeed,UINT64 u8SeekPos,SWDMX_FLV_INFO_T *prSwdmxFlvInfo)
{
   SWDMX_RANGE_LIST_T* prRangeList=prSwdmxFlvInfo->prRangeList;
   MM_RANGE_INFO_T *prCurRangeInfo=prSwdmxFlvInfo->prRangeList->prDmxRangeInfo;
   UINT32 u4CurPts,u4AVPtsDiff=0,u4Temp;
   INT32 i4PreSpeed,i4SeekRet=FLV_SEEK_RET_FIND;
   VDEC_PTS_INFO_T rPTSInfo;

   x_memset(&rPTSInfo,0,sizeof(VDEC_PTS_INFO_T));
   u4CurPts = VDP_GetPts(0);
   LOG(5, "Speed change while VDP Pts = 0x%08x\n", u4CurPts);
   i4PreSpeed=prSwdmxFlvInfo->i4PrevSpeed;

   if(prSwdmxFlvInfo->fgDeliverVEos)
   {
      _SWDMX_FlvDmxRest(prSwdmxFlvInfo,eSWDMX_MEDIA_VIDEO);
      prSwdmxFlvInfo->fgDeliverVEos = FALSE;
   }


   if (prSwdmxFlvInfo->fgDeliverAEos)
   {
      _SWDMX_FlvDmxRest(prSwdmxFlvInfo,eSWDMX_MEDIA_AUDIO);
      prSwdmxFlvInfo->fgDeliverAEos = FALSE;
   }

   if(prCurRangeInfo->e_mm_src_type!=MM_SRC_TYPE_HIGH_SPEED_STORAGE)
   {
       FeederSetInfo(FEEDER_SWDMX_EOS, FALSE);
       prSwdmxFlvInfo->fgFeederIsReady=FALSE;
   }


   prSwdmxFlvInfo->fgAudIsReady = FALSE;
   prSwdmxFlvInfo->fgAudIsLoaded = FALSE;
   prSwdmxFlvInfo->fgBadAudio = FALSE;
   prSwdmxFlvInfo->fgErrorConvery=FALSE;
   prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;

   // 1X -> 2X
   if(i4PreSpeed == 1000 && i4PlaySpeed == 2000)
   {
      if(prCurRangeInfo->e_mm_src_type!=MM_SRC_TYPE_HIGH_SPEED_STORAGE)
      {
         // Set active pic to 255
         DMX_AudHandler_SetActivePidx(DMX_AUDIO_HANDLER_NULL_PIDX);
      }
   }

   prSwdmxFlvInfo->u4VidRepeatFrame = 0;
   if(prSwdmxFlvInfo->fgVDecIsPause)
   {
      _SWDMX_FlvVideoWakeUp(prSwdmxFlvInfo);
   }

   rPTSInfo.u8Scale = DEMUX_FLV_AV_PERIOD;
   rPTSInfo.u4Rate = prSwdmxFlvInfo->u4VidFps;
        // seek
   if(((UINT32)(u8SeekPos>>32) != 0xFFFFFFFF)|| ((UINT32)(u8SeekPos) != 0xFFFFFFFF))
   {
      i4SeekRet=_SWDMX_FlvSeek(prSwdmxFlvInfo,u8SeekPos,i4PlaySpeed,eFLV_SEEK_TYPE_TIME);

      if(i4SeekRet!=FLV_SEEK_RET_FILEEND)
      {
         prSwdmxFlvInfo->fgNeedSetAudStartPts=TRUE;
         LOG(5, "u8SeekPos = %lld\n", u8SeekPos);
         prSwdmxFlvInfo->fgPreviouIsSeek = TRUE;

         if(prSwdmxFlvInfo->fgEnVideo)
         {
           MPV_FlushEsmQ(0, TRUE);
           VDEC_ReleaseDispQ(0);

           if(i4PlaySpeed >0)
           {
              VDEC_SetRenderPts(ES0, prSwdmxFlvInfo->u4RecSeekTime);
           }
           else
           {
              VDEC_SetRenderPts(ES0, 0);
           }

           VDEC_StartPTS(ES0, TRUE, prSwdmxFlvInfo->u4RecSeekTime, NULL);
           DMX_MM_FlushBuffer(eSWDMX_MEDIA_VIDEO);
         }

         DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO);
         //AUD_SetStartPts(0, prSwdmxFlvInfo->u4RecSeekAtime);
         STC_StopStc();
         STC_SetStcValue(prSwdmxFlvInfo->u4RecSeekTime);
         if(prCurRangeInfo->e_mm_src_type!=MM_SRC_TYPE_HIGH_SPEED_STORAGE)
         {
           DMX_AudHandler_SeekAndUpdate(7,0xffffffff, &u4Temp);
         }

      }

   }// 2000->1000
   else if(i4PlaySpeed == 1000&&(i4PreSpeed == 2000 ||(i4PreSpeed < 1000 && i4PreSpeed >= 1)))
   {
       u4CurPts = VDP_GetPts(0);
       if (i4PreSpeed<1000 && i4PreSpeed>=1)
	   {
           STC_StartStc();
       }

      if(1)
      {
        LOG(5, "Flv Speed 2X -> 1X Cur Vid Pts = 0x%08x, Cur Aud Pts = 0x%08x\n",prSwdmxFlvInfo->u4CurVidPts, prSwdmxFlvInfo->u4AudPts);
        DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO);

        if(prSwdmxFlvInfo->prRangeList->prDmxRangeInfo->e_mm_src_type == MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        {
           if(prSwdmxFlvInfo->u4LastPts>u4CurPts)
           {
              u4AVPtsDiff = prSwdmxFlvInfo->u4LastPts - u4CurPts;
           }

           if (u4AVPtsDiff >= 45000)
           {
              LOG(5, "u4AVPtsDiff > 45000\n");
              _SWDMX_FlvSeek(prSwdmxFlvInfo,u4CurPts,i4PlaySpeed,eFLV_SEEK_TYPE_TRICK);
              MPV_FlushEsmQ(0, TRUE);
              VDEC_ReleaseDispQ(0);
              DMX_MM_FlushBuffer(eSWDMX_MEDIA_VIDEO);
              prSwdmxFlvInfo->fgNeedSetAudStartPts=TRUE;
           }
           else
           {
               prSwdmxFlvInfo->u4TriggerAudPts = prSwdmxFlvInfo->u4AudPts;
               AUD_SetStartPts(0, prSwdmxFlvInfo->u4AudPts);
           }
        }
        else
        {  //DLNA

        }
        LOG(5, "Cur Dmx Pos = %ld\n",prSwdmxFlvInfo->u8CurDmxMoviPos);
     }

   }
   else if(i4PreSpeed < 0 && (i4PlaySpeed == 2000 || i4PlaySpeed == 1000))
   {
      _SWDMX_FlvSeek(prSwdmxFlvInfo,u4CurPts,i4PlaySpeed,eFLV_SEEK_TYPE_TRICK);
      MPV_FlushEsmQ(0, TRUE);
      VDEC_ReleaseDispQ(0);
      DMX_MM_FlushBuffer(eSWDMX_MEDIA_VIDEO);
      DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO);

      if(prSwdmxFlvInfo->fgEnVideo)
      {
         VDEC_SetRenderFromPts(0,prSwdmxFlvInfo->u4RecSeekTime);
         VDEC_StartPTS(0, TRUE,prSwdmxFlvInfo->u4RecSeekTime ,&rPTSInfo);
      }
      STC_SetStcValue(prSwdmxFlvInfo->u4RecSeekTime-15000);
      if(i4PlaySpeed == 1000 && prSwdmxFlvInfo->fgEnAudio)
      {
         prSwdmxFlvInfo->fgNeedSetAudStartPts=TRUE;
         //prSwdmxFlvInfo->u4TriggerAudPts = prSwdmxFlvInfo->u4RecSeekAtime+45000;
         LOG(5, "VDP trigger audio = 0x%08x\n", prSwdmxFlvInfo->u4TriggerAudPts);
         //AUD_SetStartPts(0,prSwdmxFlvInfo->u4RecSeekAtime);
         // set stc
         STC_StopStc();
         STC_SetStcValue(prSwdmxFlvInfo->u4RecSeekTime);
      }
      LOG(5, "Flv Speed -32X/-16X/-8X/-4X/-2X -> 1X\n");

   }
   else if (!(i4PreSpeed == 1000 && (i4PlaySpeed == 2000 || i4PlaySpeed == 1)))
   {
	  if (!(i4PlaySpeed < 1000 && i4PlaySpeed > 1))
	  {
		 if (!((i4PreSpeed == 1&&i4PlaySpeed == 2000) || (i4PreSpeed==2000 &&i4PlaySpeed==1000)))
		 {
			 _SWDMX_FlvSeek(prSwdmxFlvInfo, u4CurPts,i4PlaySpeed,eFLV_SEEK_TYPE_TRICK);
			 MPV_FlushEsmQ(0, TRUE);
			 VDEC_ReleaseDispQ(0);
			 DMX_MM_FlushBuffer(eSWDMX_MEDIA_VIDEO);
			 DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO);

			 if (i4PlaySpeed > 1000)
			 {
			    VDEC_SetRenderFromPts(0, prSwdmxFlvInfo->u4RecSeekTime);
				VDEC_StartPTS(0, FALSE,  prSwdmxFlvInfo->u4RecSeekTime, &rPTSInfo);
			 }
			 else if (i4PlaySpeed == 1000 || i4PlaySpeed ==1)
			 {
				VDEC_SetRenderFromPts(0,u4CurPts);
				VDEC_StartPTS(0, TRUE, prSwdmxFlvInfo->u4RecSeekTime,&rPTSInfo);
			 }
			 else
			 {
				VDEC_SetRenderFromPts(0,prCurRangeInfo->t_vid_start_pts);
				VDEC_StartPTS(0, FALSE, prCurRangeInfo->t_vid_start_pts, &rPTSInfo);
			 }
		 }
		 else
		 {
		     STC_StartStc();
		 }

	     if (i4PlaySpeed > 2000 || i4PlaySpeed < 0)
	     {
		    prSwdmxFlvInfo->fgTrickMode = TRUE;
	     }

		// Things for 32/16/8/4/-2/-4/-8/-16/-32 -> 1X are the same
	     if((i4PreSpeed > 2000 || i4PreSpeed < 0) &&(i4PlaySpeed == 1000) &&prSwdmxFlvInfo->fgEnAudio)
	     {
		    //prSwdmxFlvInfo->u4TriggerAudPts = prSwdmxFlvInfo->u4RecSeekAtime;

		    //AUD_SetStartPts(0,  prSwdmxFlvInfo->u4RecSeekAtime);
            prSwdmxFlvInfo->fgNeedSetAudStartPts=TRUE;
            LOG(2,"u4RecSeekAtime = %x\n", prSwdmxFlvInfo->u4RecSeekAtime);
				  // set stc
		    STC_StopStc();
		    STC_SetStcValue(prSwdmxFlvInfo->u4RecSeekTime);
	     }
	     LOG(5, "_rSwdmxFlvInfo.i4PrevSpeed != 1000 i4PlaySpeed != 2000\n");
			  // don't need to handle 1X->2X
	 }
 }

 if(i4PlaySpeed<0 || i4PlaySpeed>2000)
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
    LOG(5, "_SWDMX_FlvVideoWakeUp\n");
    VDP_SetPauseMm(0, FALSE);
    switch(prSwdmxFlvInfo->u4VDecType)
    {
        case ENUM_SWDMX_VDEC_MPEG4:
            VDEC_Play(ES0, VDEC_FMT_MP4);
            break;
        case ENUM_SWDMX_VDEC_H264:
            VDEC_Play(ES0, VDEC_FMT_H264);
            break;
        case ENUM_SWDMX_VDEC_MJPEG:
            VDEC_Play(ES0, VDEC_FMT_MJPEG);
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
    FBM_POOL_T* prFbmPool;
    DMX_MM_T rDmxMMInfo;
    x_memset(&rDmxMMInfo, 0, sizeof(DMX_MM_T));

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;

        if (!prSwdmxFlvInfo->fgEnVDmx)
        {
            // Allocate video buffer from FBM
            prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
            if((prFbmPool == NULL) || (prFbmPool->u4Addr == 0))
            {
                LOG(1, "_SWDMX_FlvSetVideoStreamID (prFbmPool == NULL) || (prFbmPool->u4Addr == NULL)\n");
                return FALSE;
            }

            u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY | DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF );
            rDmxMMInfo.fgAllocBuf = FALSE;
            rDmxMMInfo.u4BufAddr = prFbmPool->u4Addr;
            rDmxMMInfo.u4BufSize = prFbmPool->u4Size;

            switch (prSwdmxFlvInfo->u4VDecType)
            {
                case ENUM_SWDMX_VDEC_MPEG4:
                case ENUM_SWDMX_VDEC_MJPEG:
				case ENUM_SWDMX_VDEC_H264:
                    LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = FALSE\n");
                    rDmxMMInfo.fgSearchStartCode = FALSE;
                    break;
                default:

                    u4Flag |= DMX_MM_FLAG_SEARCH_START_CODE;
                    LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = TRUE\n");
                    rDmxMMInfo.fgSearchStartCode = TRUE;
                    break;
            }

            fgRet = DMX_MM_Set(eSWDMX_MEDIA_VIDEO, u4Flag, &rDmxMMInfo);
        }
        else if (fgEos)
        {
            rDmxMMInfo.fgSearchStartCode = FALSE;
            fgRet = DMX_MM_Set(eSWDMX_MEDIA_VIDEO, u4Flag, &rDmxMMInfo);
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        UINT32 au4BufStart[2], au4BufEnd[2];

        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_AUDIO;

        if (!prSwdmxFlvInfo->fgEnADmx)
        {
            u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY | DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF );

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
            prSwdmxFlvInfo->u4AFifoSize = rDmxMMInfo.u4BufSize;
            fgRet = DMX_MM_Set(eSWDMX_MEDIA_AUDIO, u4Flag, &rDmxMMInfo);
        }
        else if (fgEos)
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;
            fgRet = DMX_MM_Set(eSWDMX_MEDIA_AUDIO, u4Flag, &rDmxMMInfo);
        }
    }
    return fgRet;
}



static BOOL _SWDMX_FlvResetFeeder(SWDMX_FLV_INFO_T* prSwdmxFlvInfo)
{
    FEEDER_REQ_DATA_T rFeederReqData;
    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &prSwdmxFlvInfo->rFeederInfo) != FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    FeederSetFlush();
    _SWDMX_FlushReqMsgQ();
	_SWDMX_FlvGetDataFromFeeder(prSwdmxFlvInfo,&rFeederReqData,1);
	prSwdmxFlvInfo->fgFeederInvalid=FALSE;
	FeederSetRing(FEEDER_PROGRAM_SOURCE,TRUE);
    prSwdmxFlvInfo->u4CurDmxFeederRPtr = prSwdmxFlvInfo->rFeederInfo.u4StartAddr;
    prSwdmxFlvInfo->fgNewRp=TRUE;
    prSwdmxFlvInfo->u4LeftBytesInFeeder=0;
    prSwdmxFlvInfo->u4FeederReqReceiveID=prSwdmxFlvInfo->u4FeederReqID;
    
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
    puStart=(UCHAR *)(prSwdmxFlvInfo->rFeederInfo.u4StartAddr);
    puEnd= (UCHAR *)(prSwdmxFlvInfo->rFeederInfo.u4EndAddr);


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
       i4Ret=FeederSetRequest(FEEDER_PROGRAM_SOURCE, u4ReqSize,u8Offset,&rFeederCond,prSwdmxFlvInfo->u4FeederReqID);
       if(i4Ret!=FEEDER_E_OK)
       {
           _SWDMX_FlvResetFeeder(prSwdmxFlvInfo);
		   i4Ret=FeederSetRequest(FEEDER_PROGRAM_SOURCE, u4ReqSize,u8Offset,&rFeederCond,prSwdmxFlvInfo->u4FeederReqID);
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
       return FLV_SEEK_RET_FILEEND;
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
    u4DmxPts=prSwdmxFlvInfo->u4LastPts;
    //u4FilePos=prSwdmxFlvInfo->u4ValidSeekPos;
    u4ReadOffset=prSwdmxFlvInfo->u4ValidSeekPos;
    puStart=(UCHAR *)(prSwdmxFlvInfo->rFeederInfo.u4StartAddr);
    puEnd= (UCHAR *)(prSwdmxFlvInfo->rFeederInfo.u4EndAddr);
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
          i4Ret=FeederSetRequest(FEEDER_PROGRAM_SOURCE, SWDMX_FLV_TAG_ENDSIZE+SWDMX_FLV_TAG_HEADER_SIZE+1,u4ReadOffset-SWDMX_FLV_TAG_ENDSIZE,&rFeederCond,prSwdmxFlvInfo->u4FeederReqID);
          if(i4Ret!=FEEDER_E_OK)
          {
             _SWDMX_FlvResetFeeder(prSwdmxFlvInfo);
			 i4Ret=FeederSetRequest(FEEDER_PROGRAM_SOURCE, SWDMX_FLV_TAG_ENDSIZE+SWDMX_FLV_TAG_HEADER_SIZE+1,u4ReadOffset-SWDMX_FLV_TAG_ENDSIZE,&rFeederCond,prSwdmxFlvInfo->u4FeederReqID);
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
              i4Ret=FeederSetRequest(FEEDER_PROGRAM_SOURCE, u4ReadSize,u4ReadOffset,&rFeederCond,prSwdmxFlvInfo->u4FeederReqID);
              if(i4Ret!=FEEDER_E_OK)
              {
                 _SWDMX_FlvResetFeeder(prSwdmxFlvInfo);
				 i4Ret=FeederSetRequest(FEEDER_PROGRAM_SOURCE, u4ReadSize,u4ReadOffset,&rFeederCond,prSwdmxFlvInfo->u4FeederReqID);
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
           u4CurKeyIndex=min(u4CurKeyIndex+1,prVidKeyIdxTbl->ui4_number_of_entry-1);
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


static INT32 _SWDMX_FlvSeek(SWDMX_FLV_INFO_T *prSwdmxFlvInfo, UINT64 u8SeekTime,INT32 i4PlaySpeed,FLV_SEEK_TYPE eSeekType)
{
    INT32 i,u4CurKeyIndex=0,i4Ret=FLV_SEEK_RET_FIND;
    MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl = prSwdmxFlvInfo->prVidKeyIdxTbl;
    MM_RANGE_INFO_T *prDmxRangeInfo=prSwdmxFlvInfo->prRangeList->prDmxRangeInfo;
    UINT8 uSeekMode=0;
    LOG(7, "_SWDMX_FlvSeek u8SeekTime = 0x%08x\n", u8SeekTime);

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
          u4VdpPts=VDP_GetPts(0);
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
              VDP_GetFilePosition(0,&i4TempRef,&u2DecodOrder,&u8FileOffset,&u4FileOffsetI);
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
          VDP_GetFilePosition(0,&i4TempRef,&u2DecodOrder,&u8FileOffset,&u4FileOffsetI);
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

        i4Ret=_SWDMX_FlvSeekWithoutIndex(prSwdmxFlvInfo,u8SeekTime,uSeekMode,prDmxRangeInfo);
        LOG(5, "_SWDMX_FlvSeek without index table\n");
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
               prSwdmxFlvInfo->u4CurSentKey= i;
            }
            else
            {
               prSwdmxFlvInfo->u4CurSentKey= i>0 ? (i-1) : 0;
            }
            
            u4CurKeyIndex=prSwdmxFlvInfo->u4CurSentKey;
            prSwdmxFlvInfo->u8CurDmxMoviPos = prVidKeyIdxTbl->ui8_base_offset + prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKeyIndex].ui4_relative_offset;
            prSwdmxFlvInfo->u8CurMoviPos = prSwdmxFlvInfo->u8CurDmxMoviPos;
            prSwdmxFlvInfo->u4ValidSeekPos=prSwdmxFlvInfo->u8CurDmxMoviPos;
            prSwdmxFlvInfo->u4RecSeekTime=prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKeyIndex].ui4_pts;
            prSwdmxFlvInfo->u4RecSeekAtime=prSwdmxFlvInfo->u4RecSeekTime;
            LOG(5, "Flv Seek key frame number = 0x%08x Video offset = %lld\n", prSwdmxFlvInfo->u4CurSentKey,prSwdmxFlvInfo->u8CurDmxMoviPos);
            LOG(5, "Flv Seek Video pts = 0x%08x\n", prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKeyIndex].ui4_pts);
            _SWDMX_FlvResetFeeder(prSwdmxFlvInfo);
            LOG(7, "_SWDMX_FlvSeek Key = %ld\n", prSwdmxFlvInfo->u4CurSentKey);
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
    UINT16 u2MaxQNum, u2CurQNum;
    //UINT32 u4DummyData[4];
    DMX_MM_DATA_T rDmxMMData;

    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &prSwdmxFlvInfo->rFeederInfo) != FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);
        LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
        if (u2CurQNum > (u2MaxQNum - 10))
        {
            x_thread_delay(1);
            return FALSE;
        }
    }

    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if (_SWDMX_FlvInitDmx(prSwdmxFlvInfo, u1HdrType, NULL, TRUE) == FALSE)
    {
        return FALSE;
    }

    rDmxMMData.u4BufStart = prSwdmxFlvInfo->rFeederInfo.u4StartAddr;
    rDmxMMData.u4BufEnd = prSwdmxFlvInfo->rFeederInfo.u4EndAddr;
    rDmxMMData.u4StartAddr = prSwdmxFlvInfo->rFeederInfo.u4StartAddr;
    rDmxMMData.u4FrameSize = 4;
    rDmxMMData.fgEOS = TRUE;
    rDmxMMData.u4Pts=prSwdmxFlvInfo->u4LastPts;
    return DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500);
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
	      prSwdmxFlvInfo->u4FeederReqReceiveID=prSwdmxFlvInfo->u4FeederReqID;
          return FALSE;
	   }
	   x_thread_delay(20);
	   if(!_SWDMX_ReceiveFeederAck((VOID*)prFeederData))
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
        prSwdmxFlvInfo->fgFeederInvalid = TRUE;
        return FALSE;
	}
	
    if( (prFeederData->eDataType != FEEDER_SOURCE_INVALID) && (prFeederData->u4Id) != prSwdmxFlvInfo->u4FeederReqReceiveID)
    {
        LOG(3, "Feeder ID mismatch!!! feeder id = %ld\n", prSwdmxFlvInfo->u4FeederReqReceiveID);
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


static SWDMX_HDR_PRS_STATE_T _SWDMX_DmxTransformData(UINT8 uTagType,UINT32 u4SBufStart,UINT32 u4BufEnd,UINT32 u4DataAddr,UINT32 u4DataLen,UINT32 u4Pts)
{
    UINT32 u4DmxAvailSize=0;
    DMX_MM_DATA_T rDmxMMData;
    UINT32 u4DatType;
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo= &rSwdmxFlvInfo;

    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if(uTagType==eFLV_TAG_TYPE_AUD)
    {
        u4DatType=eSWDMX_MEDIA_AUDIO;
    }
    else if(uTagType==eFLV_TAG_TYPE_VID)
    {
        u4DatType=eSWDMX_MEDIA_VIDEO;
    }
    else
    {
      return eSWDMX_HDR_PRS_SUCCEED;
    }

    if(u4DatType==eSWDMX_MEDIA_VIDEO && (!prSwdmxFlvInfo->fgEnVideo))
      return eSWDMX_HDR_PRS_SUCCEED;
    else if(u4DatType==eSWDMX_MEDIA_AUDIO &&(!prSwdmxFlvInfo->fgEnAudio))
      return eSWDMX_HDR_PRS_SUCCEED;

    if(u4DatType==eSWDMX_MEDIA_VIDEO)
    {
        UINT16 u2MaxQNum, u2CurQNum;
        VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);
        if ((u2CurQNum > (u2MaxQNum - 256)) ||(u2CurQNum > (prSwdmxFlvInfo->u4VidFps * 10)))
        {
           if (prSwdmxFlvInfo->fgVDecIsPause)
           {
        	  _SWDMX_FlvVideoWakeUp(prSwdmxFlvInfo);
           }

           LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
           x_thread_delay(2);
           return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
        }
    }

    u4DmxAvailSize=DMX_MM_GetBufEmptySize(u4DatType);
    if(u4DmxAvailSize>=u4DataLen)
    {
    	rDmxMMData.u4BufStart = u4SBufStart;
	    rDmxMMData.u4BufEnd = u4BufEnd;
	    rDmxMMData.u4StartAddr = u4DataAddr;
	    rDmxMMData.u4FrameSize = u4DataLen;
	    rDmxMMData.u4Pts = u4Pts;
        if(u4Pts==SWDMX_FLV_INVALID_PTS)
        {
            rDmxMMData.u4Dts=u4Pts;
        }
        else
        {
            rDmxMMData.u4Dts=0;
        }

		LOG(5, "Send Q %s Data Size = 0x%08x pts = 0x%08x\n", u4DatType==eSWDMX_MEDIA_AUDIO ? "Audio" : "Video",u4DataLen, u4Pts);

#ifdef CC_MT5395
        HalFlushInvalidateDCacheMultipleLine((UINT32)(u4SBufStart), (UINT32)(u4SBufStart + u4DataLen));
#else
		HalFlushInvalidateDCache();
#endif
        if (DMX_MM_MoveData(u4DatType, &rDmxMMData, 500) != TRUE)
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
	else
	{
        LOG(7, "Dmx Buffer Full.");
        x_thread_delay(10);
		return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
	}

}


static INT32 _SWDMX_FlvParseAacConfigTable(UCHAR *puConfigPos,SWDMX_FLV_INFO_T * prSwdmxFlvInfo)
{

  UCHAR *pucPos=NULL,*pFeederStart=NULL,*pFeederEnd=NULL;
  UINT32 u4Freq=0;
  UINT8 uData=0,uBitShift=0;
  pFeederStart = (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->rFeederInfo.u4StartAddr));
  pFeederEnd =   (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->rFeederInfo.u4EndAddr));
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
  return 0;
}


static SWDMX_HDR_PRS_STATE_T _SWDMX_FlvSendAacHeader(SWDMX_FLV_INFO_T *prSwdmxFlvInfo)
{
    DMX_MM_DATA_T rDmxMMData;
    UINT32 u4DmxAvailSize,u4AudRawDataSize=0;
    UINT8 u1SampleRateIdx = 3;

    u4DmxAvailSize=DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_AUDIO);
    u4AudRawDataSize=prSwdmxFlvInfo->u4LastDataLen-2+SWDMX_FLV_AAC_FAKE_HDR_SIZE;
    if(u4DmxAvailSize<7)
      return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
    x_memset(&rDmxMMData,0,sizeof(rDmxMMData));

    /* 8b: syncword */
    _pucFlvAudBuf[0] = 0xFF;
    /* 4b: syncword */
    /* 1b: mpeg id = 1 */
    /* 2b: layer = 0 */
    /* 1b: protection absent */
    _pucFlvAudBuf[1] = 0xF9;
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

    _pucFlvAudBuf[2] = (1 << 6) | ((u1SampleRateIdx<< 2) & 0x3C) | ((prSwdmxFlvInfo->uAudChannelNs>> 2) & 0x1);
    /* 2b: channel_configuration */
    /* 1b: original */
    /* 1b: home */
    /* 1b: copyright_id */
    /* 1b: copyright_id_start */
    /* 2b: aac_frame_length */
    _pucFlvAudBuf[3] = ((prSwdmxFlvInfo->uAudChannelNs & 0x3) << 6) | ((u4AudRawDataSize >> 11) & 0x3);
    /* 8b: aac_frame_length */
    _pucFlvAudBuf[4] = ((u4AudRawDataSize>> 3) & 0xFF);
    /* 3b: aac_frame_length */
    /* 5b: adts_buffer_fullness */
    _pucFlvAudBuf[5] = ((u4AudRawDataSize << 5) & 0xE0) |((0x7FF >> 6) & 0x1F);
    /* 6b: adts_buffer_fullness */
    /* 2b: number_of_raw_data_blocks_in_frame */
    _pucFlvAudBuf[6] = ((0x7FF << 2) & 0xFC);
    rDmxMMData.u4BufStart = (UINT32)_pucFlvAudBuf;
    rDmxMMData.u4BufEnd = rDmxMMData.u4BufStart + AAC_ALLOC_SIZE;
    rDmxMMData.u4StartAddr = rDmxMMData.u4BufStart;
    rDmxMMData.u4FrameSize = SWDMX_FLV_AAC_FAKE_HDR_SIZE;
    rDmxMMData.u4Pts = prSwdmxFlvInfo->u4LastPts;

    if(DMX_MM_MoveData(eSWDMX_MEDIA_AUDIO, &rDmxMMData, 500) != TRUE)
    {
       LOG(0, "Demuxer fails to move data.\n");
       return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
    }

    return eSWDMX_HDR_PRS_SUCCEED;
}

static SWDMX_HDR_PRS_STATE_T _SWDMX_FlvSendH264SeqHeader(VDEC_PES_INFO_T *prPesInfo)
{
   UINT8 u1NumOfPS = 0, u1PSNum = 0;
   UCHAR *pucPos=NULL,*puStart,*puEnd;
   UINT16 u2PSLength = 0;
   UINT8  i=0;

   if(!prPesInfo)
     return eSWDMX_HDR_PRS_SUCCEED;
   x_thread_delay(200);
   
 /*
   if(rSwdmxFlvInfo.u4VidHdrPtr)
   {
       puStart=(UCHAR *)rSwdmxFlvInfo.u4VidHdrPtr;
       puEnd=(UCHAR *)(rSwdmxFlvInfo.u4VidHdrPtr+rSwdmxFlvInfo.u4VidHdrLen);
       pucPos=(UCHAR *)rSwdmxFlvInfo.u4VidHdrPtr;

   }
   else
  */
   {
       puStart=(UCHAR *)VIRTUAL(prPesInfo->u4FifoStart);
       puEnd=(UCHAR *)VIRTUAL(prPesInfo->u4FifoEnd);
       pucPos=(UCHAR *)VIRTUAL(prPesInfo->u4VldReadPtr);
   }

   FLV_RPOS_INC(pucPos,5,puStart,puEnd);

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

BOOL _SWDMX_FlvAudioCb(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{

    SWDMX_FLV_INFO_T *prSwdmxFlvInfo =&rSwdmxFlvInfo;
    if(eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE)
    {
        AUD_DEC_INFO_T rADecInfo;
        AUD_DspGetMpegAudInfo(0, &rADecInfo);
        //AUD_DspSetMp3PreparsingMode(TRUE);
#if 0
        BOOL fgMp3Vbr = FALSE;
        fgMp3Vbr = AUD_DspIsMp3Vbr();
        if (rADecInfo.e_aud_fmt == AUD_FMT_MP3 && !fgMp3Vbr)
        {
            if (!_SWDMX_FlvValidHdrBps(prAudIdxTbl->ui4_avg_bytes_per_sec, rADecInfo.ui4_data_rate/8))
            {
                prSwdmxFlvInfo->fgMp3Abr = TRUE;
                LOG(5, "SWDMX_Flv Mp3 Abr!!!!!\n");
            }
        }
#endif

        LOG(5, "SWDMX_FlvAudioCb Audio is ready\n");
        if (prSwdmxFlvInfo->fgEnVideo &&prSwdmxFlvInfo->prRangeList->prDmxRangeInfo->ui8_vid_duration >prSwdmxFlvInfo->u4TriggerAudPts)
        {
            if ((prSwdmxFlvInfo->fgBadAudio) &&(rADecInfo.e_aud_fmt == AUD_FMT_MP3))
            {
                UINT32 u4DspPts;
                UINT32 u4PrsPos;

                LOG(5, "Bad Audio, Lookup audio pts from audio driver\n");
                AUD_GetDspPtsInfo(0, &u4DspPts, &u4PrsPos);
                prSwdmxFlvInfo->u4TriggerAudPts = u4DspPts;
                AUD_SetStartPts(0, u4DspPts);
                prSwdmxFlvInfo->fgBadAudio = FALSE;
            }

            LOG(5, "Triggerred by video\n");
            VDP_TriggerAudReceive(prSwdmxFlvInfo->u4TriggerAudPts);
            LOG(5, "SWDMX_FlvAudioCb VDP Trigger PTS = 0x%08x\n", prSwdmxFlvInfo->u4TriggerAudPts);
            if(prSwdmxFlvInfo->fgVDecIsPause)
            {
                _SWDMX_FlvVideoWakeUp(prSwdmxFlvInfo);
            }

            if (prSwdmxFlvInfo->fgMp3Abr)
            {
                STC_SetSyncMode(AV_SYNC_MODE_NONE);
            }
        }
        else
        {
            // Audio Only case
            LOG(5, "Trigger audio by _SWDMX_FlvAudioCb\n");
            //VDP_TriggerAudReceive(prSwdmxFlvInfo->u4TriggerAudPts);
            AUD_MMAoutEnable(0, TRUE);
            STC_StartStc();
        }
        prSwdmxFlvInfo->fgAudIsReady = TRUE;
        prSwdmxFlvInfo->fgAudIsLoaded = TRUE;
    }
    else
    {
        // impossible
        LOG(4, "_SWDMX_FlvAudioCb eAudioNotifyType != ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE\n");
    }
    return TRUE;
}

static BOOL _MM_Flv_VideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT8 uByte=0,uPicType=0;
    UINT32 u4StartCode=0x00000001,u4Temp,u4NALLen=0,u4TotalNalLen=0;
    INT32 i4PtsOffset=0;
    UCHAR *puReadPtr=NULL,*puStartCode=NULL,*puBufStart=NULL,*puBufEnd=NULL;
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo =&rSwdmxFlvInfo;

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    rPesInfo.u4PTS = prPes->u4Pts;
    rPesInfo.u4DTS = prPes->u4Dts;
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.fgDtsValid = TRUE;
    rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo.fgGop = prPes->fgGop;
    rPesInfo.fgEos = prPes->fgEOS;
    rPesInfo.u8Offset=prSwdmxFlvInfo->u4ValidSeekPos;

    if (rPesInfo.fgEos)
    {
        rPesInfo.u4FifoStart = NULL;
        rPesInfo.u4FifoEnd = NULL;
        rPesInfo.u4VldReadPtr = NULL;
    }
    else if(rPesInfo.u4PTS==SWDMX_FLV_INVALID_PTS && rPesInfo.u4DTS==SWDMX_FLV_INVALID_PTS && prSwdmxFlvInfo->u4FrameAddrToVdec==0)
    {
        prSwdmxFlvInfo->u4FrameAddrToVdec=prPes->u4FrameAddr;
        return TRUE;
    }
    else if(rPesInfo.u4PTS==SWDMX_FLV_INVALID_PTS && rPesInfo.u4DTS==SWDMX_FLV_INVALID_PTS)
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
             if(prSwdmxFlvInfo->fgH264SendSeqHeader)
             {
                return TRUE;
             }
             
             rPesInfo.fgSeqHdr=TRUE;
             rPesInfo.u4VldReadPtr=(UINT32)puReadPtr;
             //FLV_RPOS_INC(rPesInfo.u4VldReadPtr,5,rPesInfo.u4FifoStart,rPesInfo.u4FifoEnd);
             _SWDMX_FlvSendH264SeqHeader(&rPesInfo);
             prSwdmxFlvInfo->fgH264SendSeqHeader=TRUE;
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
                 FLV_INTEGER(u4NALLen,puReadPtr,4,puBufStart,puBufEnd);
                 FLV_SET_INTEGER(u4StartCode,puStartCode,4,puBufStart,puBufEnd);//fill start code
                 u4TotalNalLen+=4;
                 rPesInfo.ucPicType= *puReadPtr;
                 FLV_RPOS_INC(puReadPtr,1,puBufStart,puBufEnd);
                 rPesInfo.u4VldReadPtr=(UINT32)PHYSICAL(((UINT32)puReadPtr));
                 rPesInfo.fgDtsValid = TRUE;
                 if((u4TotalNalLen+u4NALLen)>prSwdmxFlvInfo->u4LastDataLen)
                 {
                    LOG(0,"Send NAL error\n");
                 }
#ifdef CC_MT5395
                 HalFlushInvalidateDCacheMultipleLine((UINT32)(rPesInfo.u4VldReadPtr), (UINT32)(rPesInfo.u4VldReadPtr + u4NALLen+4));
#else
                 HalFlushInvalidateDCache();
#endif
                 VDEC_SendEs((void*)&rPesInfo);
                 u4TotalNalLen+=u4NALLen;
                 FLV_RPOS_INC(puReadPtr,(u4NALLen-1),puBufStart,puBufEnd);
             }
          }

       }
       else  // Sorenson/mp4
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




static SWDMX_HDR_PRS_STATE_T _SWDMX_ErrRecovery(SWDMX_FLV_INFO_T* prSwdmxFlvInfo, UCHAR* ErrorData,UINT32 u4DataLen,UINT32 *u4RecoveryOffset)
{
	UCHAR *pucPos=NULL,*pFeederStart=NULL,*pFeederEnd=NULL,*pIdentifyPos=NULL;
    UINT32 u4AudIdentify,u4VidIdentify,u4MetaIdentify=0,u4MovieByte=0,u4FramePos=0,u4Identify=0,u4Pts=0;
    UINT8  uIdentifyOffset=4;
    BOOL fgFindFrame=FALSE;
    pFeederStart = (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->rFeederInfo.u4StartAddr));
    pFeederEnd =   (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->rFeederInfo.u4EndAddr));
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
	  prSwdmxFlvInfo->u4FrameAddrToVdec=0;
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
                u4MovieByte=min(u4MovieByte,u4DataLen);
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
	pFeederStart = (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->rFeederInfo.u4StartAddr));
	pFeederEnd =   (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->rFeederInfo.u4EndAddr));
	pucBitstream=  (UCHAR *)(VIRTUAL(((UINT32)(pucBitstream))));

    if(prSwdmxFlvInfo->i4DebugerCounter==u4DebutCounter)
    {
      LOG(10,"Stop Here\n");
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
      u4DataLen=min(prSwdmxFlvInfo->u4SkipDatLen,(u4AvailSize-u4MoviedByte));
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
       u4DataLen=min((SWDMX_FLV_TAG_HEADER_SIZE-prSwdmxFlvInfo->uTagHeaderBrokenIndex),(u4AvailSize-u4MoviedByte));
       for(u4Index=0;u4Index<u4DataLen;u4Index++)
       {
          if((prSwdmxFlvInfo->uTagHeaderBrokenIndex+u4Index)<SWDMX_FLV_TAG_HEADER_SIZE)
          {
              _aucHeaderBuf[prSwdmxFlvInfo->uTagHeaderBrokenIndex+u4Index]=*pucPos;
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
       u4DataLen=min((prSwdmxFlvInfo->u4LastDataLen-prSwdmxFlvInfo->u4LastDataBrokenNum),(u4AvailSize-u4MoviedByte));

	   if(u4DataLen &&(uTagType==eFLV_TAG_TYPE_AUD || uTagType==eFLV_TAG_TYPE_VID))
       {
           if(u4DataLen+prSwdmxFlvInfo->u4LastDataBrokenNum<prSwdmxFlvInfo->u4LastDataLen)
           {
              u4TransPts=SWDMX_FLV_INVALID_PTS;
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
                      _pucFlvAAcCfg[u4Index]=uData;
                    }
                    prSwdmxFlvInfo->fgAacCfgBroken=TRUE;
                 }
                 fgDoTransData=FALSE;
              }
              else
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
                  _pucFlvAAcCfg[prSwdmxFlvInfo->u4LastDataBrokenNum-2+u4Index]=uData;
               }
               fgDoTransData=FALSE;
           }

           if(fgDoTransData)
           {
               uRetState=_SWDMX_DmxTransformData(uTagType,(UINT32)pFeederStart,(UINT32)pFeederEnd,(UINT32)pucPos,u4TransLen,u4TransPts);
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
             _SWDMX_FlvParseAacConfigTable(_pucFlvAAcCfg,prSwdmxFlvInfo);
          }

          if(u4AvailSize-u4MoviedByte>=SWDMX_FLV_TAG_ENDSIZE)
		  {
              pucPos=pucBitstream;
              FLV_RPOS_INC(pucPos,u4MoviedByte,pFeederStart,pFeederEnd);
              FLV_INTEGER_NEXT(u4TagEndLen,pucPos,4,pFeederStart,pFeederEnd);
              if(u4TagEndLen && (u4TagEndLen != (SWDMX_FLV_TAG_HEADER_SIZE+prSwdmxFlvInfo->u4LastDataLen)))
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
		    pucPos=_aucHeaderBuf;
		    pFeederStart=_aucHeaderBuf;
		    pFeederEnd=_aucHeaderBuf+SWDMX_FLV_TAG_HEADER_SIZE+SWDMX_FLV_TAG_ENDSIZE;
		    fgUseHeadBuf=FALSE;
            LOG(3,"Use HeaderBuffer to Parse Header\n");

	     }
	     else
	     {
		    pucPos=pucBitstream;
		    pFeederStart = (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->rFeederInfo.u4StartAddr));
		    pFeederEnd =	 (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->rFeederInfo.u4EndAddr));
		    FLV_RPOS_INC(pucPos,u4MoviedByte,pFeederStart,pFeederEnd);
            prSwdmxFlvInfo->u4ValidSeekPos=prSwdmxFlvInfo->u8CurDmxMoviPos+u4MoviedByte;

            if(u4AvailSize-u4MoviedByte<SWDMX_FLV_TAG_HEADER_SIZE)
		    {
               u4DataLen=u4AvailSize-u4MoviedByte;
			   for(u4Index=0;u4Index<u4DataLen;u4Index++)
			   {
			     if(u4Index<SWDMX_FLV_TAG_HEADER_SIZE)
                 {
                    _aucHeaderBuf[u4Index]=*pucPos;
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
             LOG(2,"ErrorHappen2,Counter=%d\n",prSwdmxFlvInfo->i4DebugerCounter);
             goto HdrParsFail;
          }

          if(u4DataParam!=SWDMX_FLV_STREAM_ID)
          {
              LOG(2,"ErrorHappen4,Counter=%d\n",prSwdmxFlvInfo->i4DebugerCounter);
              goto HdrParsFail;
          }

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

      u4DataLen=min((u4AvailSize-u4MoviedByte),u4TagDataSzie);

	  if(u4DataLen &&(uTagType==eFLV_TAG_TYPE_AUD || uTagType==eFLV_TAG_TYPE_VID))
	  {
		 pFeederStart = (UCHAR *)(VIRTUAL(prSwdmxFlvInfo->rFeederInfo.u4StartAddr));
		 pFeederEnd =	(UCHAR *)(VIRTUAL(prSwdmxFlvInfo->rFeederInfo.u4EndAddr));
         pucPos=pucBitstream;
         fgDoTransData=TRUE;
         FLV_RPOS_INC(pucPos,u4MoviedByte,pFeederStart,pFeederEnd);
         if(u4DataLen==u4TagDataSzie)
         {
            u4TransPts=u4Pts;
         }
         else
         {
            u4TransPts=SWDMX_FLV_INVALID_PTS;
         }

         if(uTagType==eFLV_TAG_TYPE_AUD)
         {
            prSwdmxFlvInfo->u2TagIdentify=((*pucPos)<<8)|(0xff&prSwdmxFlvInfo->u2TagIdentify);
            FLV_RPOS_INC(pucPos,1,pFeederStart,pFeederEnd);
            u4MoviedByte++;
            u4TransLen=u4DataLen-1;
            if(prSwdmxFlvInfo->fgNeedSetAudStartPts)
            {
              prSwdmxFlvInfo->fgNeedSetAudStartPts=FALSE;
              prSwdmxFlvInfo->u4TriggerAudPts=u4Pts;
              STC_SetStcValue(u4Pts-25000);
              AUD_SetStartPts(0,u4Pts);
              if(prSwdmxFlvInfo->u4VDecType==ENUM_SWDMX_VDEC_MPEG4)
              {
                  STC_StartStc();
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
                    _pucFlvAAcCfg[u4Index]=uData;
                  }
                  prSwdmxFlvInfo->fgAacCfgBroken=TRUE;
               }

               fgDoTransData=FALSE;
            }
            else
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
                   prSwdmxFlvInfo->u4LastKeyPts=prSwdmxFlvInfo->u4LastPts;
             }

             uRetState=_SWDMX_DmxTransformData(uTagType,(UINT32)pFeederStart,(UINT32)pFeederEnd,(UINT32)pucPos,u4TransLen,u4TransPts);
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
            break;

	     if(u4AvailSize-u4MoviedByte>=SWDMX_FLV_TAG_ENDSIZE)
	     {
            pucPos=pucBitstream;
		    FLV_RPOS_INC(pucPos,u4MoviedByte,pFeederStart,pFeederEnd);
            FLV_INTEGER_NEXT(u4TagEndLen,pucPos,4,pFeederStart,pFeederEnd);

            if(u4TagEndLen && (u4TagEndLen != (SWDMX_FLV_TAG_HEADER_SIZE+u4TagDataSzie)))
            {
               LOG(2,"ErrorHappen4,Counter=%d\n",prSwdmxFlvInfo->i4DebugerCounter);
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
	}

  *pu4ConsumeSize=u4MoviedByte;
  return eSWDMX_HDR_PRS_SUCCEED;

DataNotEnough:
   prSwdmxFlvInfo->fgNewRp=TRUE;
  *pu4ConsumeSize=u4MoviedByte;
  return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;

DmxBufNotEnough:
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

        prSwdmxFlvInfo->fgNewRp = TRUE;
        prSwdmxFlvInfo->u4LeftBytesInFeeder = 0;
		prSwdmxFlvInfo->u8CurMoviPos = prSwdmxFlvInfo->u8CurDmxMoviPos;
        prSwdmxFlvInfo->fgFeederInvalid = FALSE;
        prSwdmxFlvInfo->i4ReqReadTimes = 0;
        prSwdmxFlvInfo->u4FeederReqID = 0;
        prSwdmxFlvInfo->u4FeederReqReceiveID=0;
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

        if(prRangeList->prDmxRangeInfo->e_mm_src_type!=MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        {
            FeederSetInfo(FEEDER_SWDMX_EOS, TRUE);
        }
        x_thread_delay(1);
        return eSWDMX_HDR_PRS_SUCCEED;
    }


    // Request data for the first time
    if (prSwdmxFlvInfo->u8CurMoviPos == 0)
    {
        if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &prSwdmxFlvInfo->rFeederInfo) != FEEDER_E_OK)
        {
            LOG(7, "Feeder input buffer error.\n");
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

        if(prRangeList->prDmxRangeInfo->z_range_sz > prSwdmxFlvInfo->u8CurMoviPos)
        {
            u4ReadSize = prRangeList->prDmxRangeInfo->z_range_sz - prSwdmxFlvInfo->u8CurMoviPos;
        }

        u4ReadSize = min(u4ReadSize, FLV_MIN_READ_SIZE);
        //prSwdmxFlvInfo->u8CurMoviPos = prRangeList->prDmxRangeInfo->ui8_fileoffset;
        prSwdmxFlvInfo->u8CurDmxMoviPos = prSwdmxFlvInfo->u8CurMoviPos;

        if (FeederSetRequest(FEEDER_PROGRAM_SOURCE, u4ReadSize,prSwdmxFlvInfo->u8CurMoviPos,&rFeederCond,prSwdmxFlvInfo->u4FeederReqID) != FEEDER_E_BUFFER_FULL)
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
            prSwdmxFlvInfo->u4CurDmxFeederRPtr = VIRTUAL(prSwdmxFlvInfo->rFeederInfo.u4StartAddr + rFeederReqData.u4AlignOffset);
            prSwdmxFlvInfo->u4LeftBytesInFeeder = rFeederReqData.u4ReadSize-rFeederReqData.u4AlignOffset;
            LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
        }
    }


    if(FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &prSwdmxFlvInfo->rFeederInfo) != FEEDER_E_OK)
    {
       LOG(7, "Feeder input buffer error.\n");
       return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    if(prRangeList->prDmxRangeInfo->e_mm_src_type != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
    {
        if(FeederIsReady(FEEDER_PROGRAM_SOURCE))
        {
            prSwdmxFlvInfo->fgFeederIsReady = TRUE;
        }
        else
        {
            prSwdmxFlvInfo->fgFeederIsReady = FALSE;
        }
    }

    if(!prSwdmxFlvInfo->fgTrickMode &&(prSwdmxFlvInfo->rFeederInfo.u4BytesInBuf <=FLV_BUFFER_SIZE))
    {
        u4ReadSize=0;
        if (prRangeList->prDmxRangeInfo->z_range_sz > prSwdmxFlvInfo->u8CurMoviPos)
        {
            u4ReadSize = prRangeList->prDmxRangeInfo->z_range_sz - prSwdmxFlvInfo->u8CurMoviPos;
        }

        u4ReadSize = min(u4ReadSize, FLV_MIN_READ_SIZE);

        if (u4ReadSize == 0)
        {
            fgRangeEnd = TRUE;
        }

		if(prRangeList->prDmxRangeInfo->b_thumbnail_pb && prSwdmxFlvInfo->i4ReqReadTimes>0)
		{
           u4ReadSize=0;
		}

        if (u4ReadSize>0)
        {
            if((prSwdmxFlvInfo->rFeederInfo.u4BytesInBuf + u4ReadSize < FLV_BUFFER_SIZE))
            {
               LOG(6, "Left bytes = %ld offset = %lld id = %ld.\n", prSwdmxFlvInfo->rFeederInfo.u4BytesInBuf,prSwdmxFlvInfo->u8CurMoviPos,prSwdmxFlvInfo->u4FeederReqID);
               if(FeederSetRequest(FEEDER_PROGRAM_SOURCE,u4ReadSize, prSwdmxFlvInfo->u8CurMoviPos,&rFeederCond,prSwdmxFlvInfo->u4FeederReqID) != FEEDER_E_BUFFER_FULL)
               {
               	    UINT64 u8Remainder;
                     u4AlignSize=FeederGetAlignSize();
               	     u8Div6432(prSwdmxFlvInfo->u8CurMoviPos,(UINT64)u4AlignSize, &u8Remainder);
                     prSwdmxFlvInfo->u8CurMoviPos+=((UINT64)rFeederCond.u4TransmitSize-u8Remainder);
                     prSwdmxFlvInfo->i4ReqReadTimes++;
					 prSwdmxFlvInfo->u4FeederReqID++;
               }
               else
               {
                  LOG(3, "Feeder Buffer Full\n");
                  if(FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &prSwdmxFlvInfo->rFeederInfo) != FEEDER_E_OK)
                  {
                     LOG(1, "Feeder input buffer error.\n");
                     return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                  }
                  LOG(3, "Cur Fed's write ptr = 0x%08x\n", prSwdmxFlvInfo->rFeederInfo.u4WriteAddr);
               }

               if(prRangeList->prDmxRangeInfo->e_mm_src_type != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
               {
                   if(FeederIsReady(FEEDER_PROGRAM_SOURCE))
                   {
                       prSwdmxFlvInfo->fgFeederIsReady = TRUE;
                   }
                   else
                   {
                       prSwdmxFlvInfo->fgFeederIsReady = FALSE;
                   }
               }

            }
        }
    }
    else if(prSwdmxFlvInfo->fgTrickMode)
    {
       INT32 i4TrickRet=FLV_SEEK_RET_FIND;
       UINT16 u2CurQNum=0,u2MaxQNum=0;
       VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);

       if(u2CurQNum > prSwdmxFlvInfo->u4VidFps *3)
       {
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
	
    if((!prSwdmxFlvInfo->fgTrickRcv)&&(!prSwdmxFlvInfo->fgErrorConvery)&&prSwdmxFlvInfo->u4LeftBytesInFeeder &&((prRangeList->prDmxRangeInfo->e_mm_src_type==MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        ||(prRangeList->prDmxRangeInfo->e_mm_src_type!=MM_SRC_TYPE_HIGH_SPEED_STORAGE && (prSwdmxFlvInfo->fgFeederIsReady || prSwdmxFlvInfo->fgTrickMode))))
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
                       _SWDMX_FlvResetFeeder(prSwdmxFlvInfo);
                       UINT32 u4FeederAlignSize=0;
                       u4FeederAlignSize=FeederGetAlignSize();
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

                       if(_SWDMX_FlvReqTagData(prSwdmxFlvInfo,prSwdmxFlvInfo->u8CurDmxMoviPos,FLV_ERROR_RECOVERY_LEN,FALSE,&rReqData)==FLV_SEEK_RET_FIND)
                       {
                          prSwdmxFlvInfo->u4CurDmxFeederRPtr=rReqData.u4WriteAddr+rReqData.u4AlignOffset;
                          prSwdmxFlvInfo->u4LeftBytesInFeeder=rReqData.u4ReadSize-rReqData.u4AlignOffset;
                          prSwdmxFlvInfo->PRecoverPos=(UCHAR *)prSwdmxFlvInfo->u4CurDmxFeederRPtr;
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
                u4CurKeyIndex=min(u4CurKeyIndex+1,prSwdmxFlvInfo->prVidKeyIdxTbl->ui4_number_of_entry-1);
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

    if ((prSwdmxFlvInfo->u4CurDmxFeederRPtr + u4ConsumedSize) >= VIRTUAL(prSwdmxFlvInfo->rFeederInfo.u4EndAddr))
    {
        prSwdmxFlvInfo->u4CurDmxFeederRPtr =  VIRTUAL(prSwdmxFlvInfo->rFeederInfo.u4StartAddr) +
            ((prSwdmxFlvInfo->u4CurDmxFeederRPtr + u4ConsumedSize) -VIRTUAL(prSwdmxFlvInfo->rFeederInfo.u4EndAddr));
    }
    else
    {
        prSwdmxFlvInfo->u4CurDmxFeederRPtr += u4ConsumedSize;
    }

    // Update read ptr to feeder
    if(u4ConsumedSize>0)
    {
        FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(prSwdmxFlvInfo->u4CurDmxFeederRPtr));
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

        /*
              if (!_SWDMX_FlvGetDataFromFeeder(prSwdmxFlvInfo, &rFeederReqData))
              {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
              }
            */

        LOG(6, "rFeederReqData.u4WriteAddr = 0x%08x. ReadSize = 0x%08x id = %ld\n", rFeederReqData.u4WriteAddr,rFeederReqData.u4ReadSize,rFeederReqData.u4Id);
        if((rFeederReqData.u4ReadSize <rFeederReqData.u4AlignOffset) || (rFeederReqData.u4ReadSize == 0))
        {
            LOG(3, "Feeder read size is smaller than align offset!!!\n");
            prSwdmxFlvInfo->u8CurDmxMoviPos = prRangeList->prDmxRangeInfo->z_range_sz;
            prSwdmxFlvInfo->u4LeftBytesInFeeder = 0;
            return eSWDMX_HDR_PRS_FAIL;
        }

        prSwdmxFlvInfo->u4LeftBytesInFeeder += (rFeederReqData.u4ReadSize-rFeederReqData.u4AlignOffset);
        prSwdmxFlvInfo->i4ReqReadTimes--;
        FLV_RPOS_INC(rFeederReqData.u4WriteAddr,rFeederReqData.u4AlignOffset,prSwdmxFlvInfo->rFeederInfo.u4StartAddr,prSwdmxFlvInfo->rFeederInfo.u4EndAddr);
        prSwdmxFlvInfo->u4CurDmxFeederRPtr =  VIRTUAL(rFeederReqData.u4WriteAddr);
        prSwdmxFlvInfo->fgNewRp = FALSE;
        LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
    }

    return eSWDMX_HDR_PRS_SUCCEED;
}


static INT32 _SWDMX_FlvBeginPlay(SWDMX_FLV_INFO_T *prSwdmxFlvInfo)
{
  VDEC_PTS_INFO_T rPTSInfo;
  MM_RANGE_INFO_T *prCurRangeInfo=prSwdmxFlvInfo->prRangeList->prDmxRangeInfo;
  UINT32 u4VidStartPts;
  FeederSetFlush();
  _SWDMX_FlushReqMsgQ();
  rPTSInfo.u8Scale = DEMUX_FLV_AV_PERIOD;
  rPTSInfo.u4Rate = prSwdmxFlvInfo->u4VidFps;
  x_memset(&rPTSInfo,0,sizeof(VDEC_PTS_INFO_T));

  if ((prCurRangeInfo->e_mm_src_type!= MM_SRC_TYPE_HIGH_SPEED_STORAGE) && (prSwdmxFlvInfo->i4PrevSpeed==1000 || prCurRangeInfo->t_vid_start_render_pts!=0))
  {
     FeederSetReadyThreshold(FLV_MIN_READ_SIZE*2);
     FeederSetUnderFlowThreshold(FLV_MIN_READ_SIZE/2);
  }

  if(prCurRangeInfo->t_vid_start_render_pts)
  {
    if(prSwdmxFlvInfo->u4VidHdrPtr != NULL)
    {
        _SWDMX_FlvSendH264SeqHeader(NULL);
    }

    _SWDMX_FlvSeek(prSwdmxFlvInfo, prCurRangeInfo->t_vid_start_render_pts,1000,eFLV_SEEK_TYPE_TIME);
    LOG(5, "Video Start Render Pts = %lld\n", prCurRangeInfo->t_vid_start_render_pts);
    u4VidStartPts=prCurRangeInfo->t_vid_start_render_pts;;

    if (prSwdmxFlvInfo->fgEnVideo)
    {
       VDEC_StartPTS(0, TRUE, 0, &rPTSInfo);
       VDEC_SetRenderFromPts(ES0, u4VidStartPts);
       VDP_SetPauseMm(0, TRUE);
       VDEC_Pause(ES0);
       prSwdmxFlvInfo->fgVDecIsPause = TRUE;
    }

    STC_StopStc();
    STC_SetStcValue(max((u4VidStartPts - 45000),0));

  }
  else if(prSwdmxFlvInfo->i4PrevSpeed==1000)
  {
      if(prSwdmxFlvInfo->fgEnAudio && prSwdmxFlvInfo->fgEnVideo)
      {
         //VDEC_StartPTS(0, TRUE, 0, &rPTSInfo);
         VDEC_SetRenderFromPts(ES0, prSwdmxFlvInfo->u4CurVidPts);
         VDP_SetPauseMm(0, TRUE);
         VDEC_Pause(ES0);
         prSwdmxFlvInfo->fgVDecIsPause = TRUE;
         if(prCurRangeInfo->t_aud_start_pts!=0)
         {
           prSwdmxFlvInfo->u4AudStartPts=prCurRangeInfo->t_aud_start_pts;
         }

         AUD_SetStartPts(0,prSwdmxFlvInfo->u4AudStartPts);
         prSwdmxFlvInfo->u4TriggerAudPts = prSwdmxFlvInfo->u4AudStartPts;
      }
      else if(prSwdmxFlvInfo->fgEnAudio)
      {
         AUD_SetStartPts(0,prSwdmxFlvInfo->u4AudStartPts);
         prSwdmxFlvInfo->u4AudPts=prSwdmxFlvInfo->u4AudStartPts;
      }
      else if(prSwdmxFlvInfo->fgEnVideo)
      {
         VDEC_StartPTS(0, TRUE, 0, &rPTSInfo);
         VDEC_SetRenderFromPts(ES0, prSwdmxFlvInfo->u4CurVidPts);
         VDP_SetPauseMm(0, TRUE);
         VDEC_Pause(ES0);
         prSwdmxFlvInfo->fgVDecIsPause = TRUE;
      }

      STC_StopStc();
      STC_SetStcValue(max((prSwdmxFlvInfo->u4CurVidPts - 45000),0));
      STC_StartStc();
  }

  return 0;
}

SWDMX_HDR_PRS_STATE_T _SWDMX_FlvHdrParse(
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange)
{
    UNUSED(i4PlaySpeed);
    UNUSED(fgSpeedChange);

    UINT32 u4PrsState;
	SWDMX_FLV_INFO_T * prSwdmxFlvInfo=&rSwdmxFlvInfo;
    VDEC_PTS_INFO_T rPTSInfo;
    SWDMX_INFO_T* prSwdmxInst = NULL;

    prSwdmxFlvInfo->prRangeList=prRangeList;
	prSwdmxFlvInfo->fgPlayAbort=FALSE;

    x_memset((void*)&rPTSInfo, 0, sizeof(VDEC_PTS_INFO_T));
    if (prRangeList->prDmxRangeInfo == NULL)
    {
        LOG(5, "This range is empty\n");
        return eSWDMX_HDR_PRS_EMPTY_RANGE;
    }

    if (!_SWDMX_GetInfo(eSWDMX_GET_INTERNAL_INFO, (UINT32*)&prSwdmxInst, 0, 0))
    {
        LOG(5, "Fail to Get swdmx internal info\n");
        return eSWDMX_HDR_PRS_FAIL;
    }

    if ((prSwdmxInst->eSwDmxPrevousStatus == eSWDMX_STATE_PAUSE) &&
        (i4PlaySpeed == 1000) &&
        (prSwdmxFlvInfo->fgPreviouIsSeek))
    {
        LOG(5, "PAUSE -> SEEK -> RESUME set stc value to 0x%08x\n",
            prSwdmxFlvInfo->u4RecSeekTime);
        STC_SetStcValue(prSwdmxFlvInfo->u4RecSeekTime);
        prSwdmxFlvInfo->fgPreviouIsSeek = FALSE;
    }

    if ((prSwdmxFlvInfo->fgEnVideo) && (!prSwdmxFlvInfo->fgEnVDmx))
    {
        UNUSED(_SWDMX_FlvInitDmx(prSwdmxFlvInfo,eSWDMX_MEDIA_VIDEO, NULL, FALSE));
        prSwdmxFlvInfo->fgEnVDmx = TRUE;
    }

    if ((prSwdmxFlvInfo->fgEnAudio) && (!prSwdmxFlvInfo->fgEnADmx))
    {
        UNUSED(_SWDMX_FlvInitDmx(prSwdmxFlvInfo, eSWDMX_MEDIA_AUDIO, NULL, FALSE));
        prSwdmxFlvInfo->fgEnADmx = TRUE;
    }


    if(prSwdmxFlvInfo->fgEnAudio &&(prSwdmxFlvInfo->u4ADecType==ENUM_SWDMX_ADEC_AAC))
    {
       if(!_pucFlvAudBuf)
       {
           _pucFlvAudBuf = (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(AAC_ALLOC_SIZE,16));
           if(!_pucFlvAudBuf)
           {
              LOG(1, "Allocate AAC Header buffer failed!\n");
              return eSWDMX_HDR_PRS_FAIL;
           }
       }

       if(!_pucFlvAAcCfg)
       {
           _pucFlvAAcCfg=(UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(SWDMX_FLV_AAC_CONFIG_LEN,16));
           if(!_pucFlvAAcCfg)
           {
              LOG(1, "Allocate AAC Config buffer failed!\n");
              return eSWDMX_HDR_PRS_FAIL;
           }
       }
    }
    
    if (prSwdmxFlvInfo->fgBeginToPlay)
    {
        prSwdmxFlvInfo->i4PrevSpeed=i4PlaySpeed;
        _SWDMX_FlvBeginPlay(prSwdmxFlvInfo);
        prSwdmxFlvInfo->fgBeginToPlay = FALSE;
    }

    if (fgSpeedChange)
    {
        _SWDMX_FlvChangeSpeed(i4PlaySpeed,u8SeekPos,prSwdmxFlvInfo);

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
        u4PrsState = _SWDMX_FlvTagPrs(prSwdmxFlvInfo,prRangeList,i4PlaySpeed,u8SeekPos,fgSpeedChange);

    }

    return (SWDMX_HDR_PRS_STATE_T)u4PrsState;
}

VOID _SWDMX_FlvSetRange(
    SWDMX_RANGE_LIST_T *prRangeList,
    UINT32 u4VidStrmID,
    UINT32 u4AudStrmID,
    UINT32 u4SubTitleID)
{
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo= &rSwdmxFlvInfo;
	MM_RANGE_ELMT_IDXTBL_T *pBuldIdxTbl =NULL;
	MM_RANGE_INFO_T * prDmxRangeInfo=NULL;;
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
    }

    if(prSwdmxFlvInfo->u4VidFps==0 || prSwdmxFlvInfo->u4VidFps>60)
    {
      prSwdmxFlvInfo->u4VidFps=30;
    }

    LOG(3,"Flv RangeInfo:Size=%d,Time=%d,FrameRate=%d\n",prDmxRangeInfo->z_range_sz,prDmxRangeInfo->ui8_pb_duration,prSwdmxFlvInfo->u4VidFps);

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
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }
    else
    {
       LOG(0, "Demuxer Range is NULL\n");
    }

  return;
}


BOOL _SWDMX_FlvStop(VOID)
{
    DMX_MM_T rPid;
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo= &rSwdmxFlvInfo;

    rPid.fgEnable = FALSE;
	prSwdmxFlvInfo->fgPlayAbort=TRUE;

    if (prSwdmxFlvInfo->fgEnVDmx)
    {
        prSwdmxFlvInfo->fgEnVDmx = FALSE;
        VERIFY(DMX_MM_Set(eSWDMX_MEDIA_VIDEO, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(eSWDMX_MEDIA_VIDEO);
    }

    if (prSwdmxFlvInfo->fgEnADmx)
    {
       prSwdmxFlvInfo->fgEnADmx = FALSE;
       VERIFY(DMX_MM_Set(eSWDMX_MEDIA_AUDIO, DMX_MM_FLAG_VALID, &rPid));
       DMX_MM_Free(eSWDMX_MEDIA_AUDIO);
       /*  dlna
           if(prSwdmxFlvInfo->prRangeList->prDmxRangeInfo->e_mm_src_type != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
           {
             DMX_AudHandler_SetEnable(FALSE);
           }
           */
    }
    return TRUE;
}


BOOL _SWDMX_FlvDisableStrm(
    UINT8 u1StrmType)
{
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo= &rSwdmxFlvInfo;

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
    UINT8 u1StrmType,
    VOID* pfnCbFunc)
{
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo= &rSwdmxFlvInfo;

    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            LOG(5, "_SWDMX_FlvEnableStrm Vid\n");
			if(prSwdmxFlvInfo->prRangeList && prSwdmxFlvInfo->prRangeList->prDmxRangeInfo)
			{
				UNUSED(VDEC_SetTotalTime(ES0, (UINT32)prSwdmxFlvInfo->prRangeList->prDmxRangeInfo->ui8_vid_duration));
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
    UINT8 u1Type,
    UINT32* pu4Para1,
    UINT32* pu4Para2,
    UINT32* pu4Para3)
{
    UINT64 *pu8Temp=NULL;
    LOG(5, "_SWDMX_FlvGetInfo u1Type = %d\n", u1Type);
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo= &rSwdmxFlvInfo;
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
    return FALSE;
}




BOOL _SWDMX_FlvSetStrmID(UINT8 u1HdrType,
    UINT32 u4StrmID, UINT32 u4StrmSubID)
{
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo= &rSwdmxFlvInfo;
    UNUSED(u4StrmSubID);
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
    UINT8 u1Type,
    UINT32 u4Para1,
    UINT32 u4Para2,
    UINT32 u4Para3)
{
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo= &rSwdmxFlvInfo;
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
        prSwdmxFlvInfo->u4VidHdrPtr = u4Para1;
        prSwdmxFlvInfo->u4VidHdrLen = u4Para2;
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

BOOL _SWDMX_FlvAbort(VOID)
{
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo= &rSwdmxFlvInfo;
	if(prSwdmxFlvInfo)
	{
       prSwdmxFlvInfo->fgPlayAbort=TRUE;
	}
    return TRUE;
}

BOOL _SWDMX_FlvInit(ENUM_SWDMX_FMT_T eType)
{
    UNUSED(eType);

    // Initialize feeder
    //FeederOpen(FEEDER_PROGRAM_SOURCE);
    DMX_DECODER_CALLBACKS_T rCallback; // Isaac: should be DMX_DECODER_CALLBACKS_T, Jack fix me
    SWDMX_FLV_INFO_T *prSwdmxFlvInfo= &rSwdmxFlvInfo;
    x_memset(prSwdmxFlvInfo, 0, sizeof(SWDMX_FLV_INFO_T));

    //*********************************************************
    //FeederSetBufferSize(FEEDER_PROGRAM_SOURCE, 5*10*1024);
    //FeederStart(FEEDER_PROGRAM_SOURCE);

    FeederInstallCallback(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_DATA_REQ_DONE,
    _SWDMX_FeederCallBack, (UINT32)&prSwdmxFlvInfo->rFeederInfo);
    //*********************************************************


    rCallback.pfnSendAudioPes = AUD_SendAudioPes;
    rCallback.pfnSendVideoPes = _MM_Flv_VideoCallback;
    rCallback.pfnUpdateVideoWp = VDEC_SetWptr;

    DMX_SetDecoderCallbacks((const DMX_DECODER_CALLBACKS_T*)&rCallback);
    DMX_SetToDecoder(TRUE);
    u4DebutCounter=0xffffffff;
    // Default Speed
    prSwdmxFlvInfo->i4PrevSpeed = 1000;
    // Default Seek parameter
    prSwdmxFlvInfo->fgNewRp = FALSE;
    prSwdmxFlvInfo->fgBeginToPlay = TRUE;

    prSwdmxFlvInfo->u4CurRepeatFrameIdx = 0xFFFFFFFF;
    //prSwdmxFlvInfo->u4ModeChangPts = 0xFFFFFFFF;
    prSwdmxFlvInfo->fgPreviouIsSeek = FALSE;
    //prSwdmxFlvInfo->u8RecDmxMoviPos = 0;
    return TRUE;
}


