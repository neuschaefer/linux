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
 * $RCSfile: vdec_vp8psr.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_vp8par.c
 *  This source file include vp8 decode flow and memory contral
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
#include "fbm_drvif.h"
#include "vdec_drvif.h"
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "x_timer.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_common.h"
#include "x_typedef.h"
LINT_EXT_HEADER_END
#include "vdec_drvif.h"
#include "vdec_vp8webp_if.h"
#include "vdec_vp8util.h"
#include "vdec_vp8psr.h"
#include "vdec_vp8dec.h"


#ifdef VP8_TIME_PROFILE
static VOID _VDEC_Vp8TimeProfileInfo(UINT8 uFormat,BOOL fgKeyFrame,VDEC_VP8_TIMEPROFILE *prTP)
{
	UINT64 u8Value1,u8Value2;
	if(uFormat==VP8_DEC_TYPE_WEBP)
	{
	   u8Value1=prTP->u4CurDramCycle*100000*1000;
	   u8Value1=u8Div6432(u8Value1,prTP->u4DecMs,&u8Value2);
       u8Value1=u8Div6432(u8Value1,prTP->u4DelayClock,&u8Value2);
       Printf("Total:%u,HwDec:%us,SwDec=%u,DecCycle=%d,DramUsage=%d\n", \
	   prTP->u4DecMs+prTP->u4SwMicro,prTP->u4DecMs,prTP->u4SwMicro, \
	   prTP->u4CurDecCycle,u8Value1);
	   return;
	} 

	if(fgKeyFrame)
	{
	   prTP->u4IFrameCounter++;
	   prTP->u4TotalIDecTime+=prTP->u4DecMs;
	   prTP->u4TotalISwTime+=prTP->u4SwMicro;
	   prTP->u4TotalIDecCycle+=prTP->u4CurDecCycle;
	   prTP->u4TotalIDramCycle+=prTP->u4CurDramCycle;
	   
	   if(prTP->u4IFrameCounter==1)
	   {
		 prTP->u4MaxIDecTime=prTP->u4DecMs;
		 prTP->u4MinIDecTime=prTP->u4DecMs;
		 prTP->u4MaxIDecCycle=prTP->u4CurDecCycle;
		 prTP->u4MaxIDramCycle=prTP->u4CurDramCycle;
		 prTP->u4MaxISwTime=prTP->u4SwMicro;
	   }
	   else
	   {
		 prTP->u4MaxIDecTime=MAX(prTP->u4MaxIDecTime,prTP->u4DecMs);
		 prTP->u4MinIDecTime=MIN(prTP->u4MinIDecTime,prTP->u4DecMs);
		 prTP->u4MaxIDecCycle=MAX(prTP->u4MaxIDecCycle,prTP->u4CurDecCycle);
		 prTP->u4MaxIDramCycle=MAX(prTP->u4MaxIDramCycle,prTP->u4CurDramCycle);
		 prTP->u4MaxISwTime=MAX(prTP->u4MaxISwTime,prTP->u4SwMicro);
	   }
	}
	else
	{
	   prTP->u4PFrameCounter++;
	   prTP->u4TotalPDecTime+=prTP->u4DecMs;
	   prTP->u4TotalPSwTime+=prTP->u4SwMicro;
	   prTP->u4TotalPDecCycle+=prTP->u4CurDecCycle;
	   prTP->u4TotalPDramCycle+=prTP->u4CurDramCycle;
	   if(prTP->u4PFrameCounter==1)
	   {
		 prTP->u4MaxPDecTime=prTP->u4DecMs;
		 prTP->u4MinPDecTime=prTP->u4DecMs;
		 prTP->u4MaxPDecCycle=prTP->u4CurDecCycle;
		 prTP->u4MaxPDramCycle=prTP->u4CurDramCycle;
		 prTP->u4MaxPSwTime=prTP->u4SwMicro;
	   }
	   else
	   {
		 prTP->u4MaxPDecTime=MAX(prTP->u4MaxPDecTime,prTP->u4DecMs);
		 prTP->u4MinPDecTime=MIN(prTP->u4MinPDecTime,prTP->u4DecMs);
		 prTP->u4MaxPDecCycle=MAX(prTP->u4MaxPDecCycle,prTP->u4CurDecCycle);
		 prTP->u4MaxPDramCycle=MAX(prTP->u4MaxPDramCycle,prTP->u4CurDramCycle);
		 prTP->u4MaxPSwTime=MAX(prTP->u4MaxPSwTime,prTP->u4SwMicro);
	   }
	}
	
	if(prTP->u4IFrameCounter>0 && (prTP->u4IFrameCounter%TIME_PROFILE_I_INTERVAL)==0)
	{
	   u8Value1=prTP->u4MaxIDramCycle*100000*1000;
	   u8Value1=u8Div6432(u8Value1,prTP->u4MaxIDecTime,&u8Value2);
       u8Value1=u8Div6432(u8Value1,prTP->u4DelayClock,&u8Value2);
	   Printf("Vp8 Time Profile:\n\tPicType(Min/MaxHwDec, AvgHwDec, MaxHwDec, Frame/TotalFrm, MaxSwDec/AvgSwDec, MaxDramCycle)");
	   Printf("\n\tI(%u/%u, %u/%d, %u/%d, %d/%d, %u/%u, %d)\n", \
			prTP->u4MinIDecTime,prTP->u4MaxIDecTime, \
			prTP->u4TotalIDecTime/prTP->u4IFrameCounter,prTP->u4TotalIDecCycle/prTP->u4IFrameCounter,\
			prTP->u4MaxIDecTime,prTP->u4MaxIDecCycle, \
			prTP->u4IFrameCounter,prTP->u4IFrameCounter+prTP->u4PFrameCounter, \
			prTP->u4MaxISwTime,prTP->u4TotalISwTime/prTP->u4IFrameCounter, \
			u8Value1);
	   Printf("Total Info (I/P):\n");
	   Printf("HwDec:%u/%u, SwDec:%u/%u, DecCycle:%u/%u,Average=%u \n", \
	   	(prTP->u4TotalIDecTime/prTP->u4IFrameCounter),(prTP->u4TotalPDecTime/prTP->u4PFrameCounter), \
	   	(prTP->u4TotalISwTime/prTP->u4IFrameCounter),(prTP->u4TotalPSwTime/prTP->u4PFrameCounter), \
	    (prTP->u4TotalIDecCycle/prTP->u4IFrameCounter),(prTP->u4TotalPDecCycle/prTP->u4PFrameCounter),\
	    (prTP->u4TotalIDecTime+prTP->u4TotalISwTime+prTP->u4TotalPDecTime+prTP->u4TotalPSwTime)/(prTP->u4IFrameCounter+prTP->u4PFrameCounter));
	}
	else if(prTP->u4PFrameCounter>0 &&(prTP->u4PFrameCounter%TIME_PROFILE_P_INTERVAL)==0)
	{
	    u8Value1=prTP->u4MaxPDramCycle*100000*1000;
	    u8Value1=u8Div6432(u8Value1,prTP->u4MaxPDecTime,&u8Value2);
        u8Value1=u8Div6432(u8Value1,prTP->u4DelayClock,&u8Value2);
		Printf("Vp8 Time Profile:\n\tPicType(Min/MaxHwDec, AvgHwDec, MaxHwDec, Frame/TotalFrm, MaxSwDec/AvgSwDec, MaxDramCycle)");
		Printf("\n\tP(%u/%u, %u/%d, %u/%d, %d/%d, %u/%u, %d)\n", \
			 prTP->u4MinPDecTime,prTP->u4MaxPDecTime, \
			 (prTP->u4TotalPDecTime/prTP->u4PFrameCounter),prTP->u4TotalPDecCycle/prTP->u4PFrameCounter,\
			 prTP->u4MaxPDecTime,prTP->u4MaxPDecCycle, \
			 prTP->u4PFrameCounter,prTP->u4IFrameCounter+prTP->u4PFrameCounter, \
			 prTP->u4MaxPSwTime,prTP->u4TotalPSwTime/prTP->u4PFrameCounter, \
			 u8Value1);
	}
}
#endif

#ifdef CC_WEBP_EMULATION
static VOID _VDEC_Vp8CopyFrameBuffer(VDEC_VP8_INFO_T *prVpDecInfo, UINT32 u4Idx)
{
    WEBP_DEC_INFO_T *prWebPInfo=&prVpDecInfo->rWebPInfo;
    //VDEC_INFO_VP8_FRM_HDR_T *prFrmHdr=&prVpDecInfo->rVp8FrmHdr;
    UINT32 u4MBX = (prVpDecInfo->rDecParam.u2Width+15)/16;
    UINT32 u4MBY = (prVpDecInfo->rDecParam.u2Height+15)/16;
    UINT32 u4Pitch = u4MBX*16;
    UINT32 u4TotalCopyRowY, u4TotalCopyRowC, u4RowNo, u4StartRowY, u4StartRowC, u4CurRow;
    void *pvSrcY, *pvSrcC, *pvDstY, *pvDstC;
    UINT32 x, y, yBlk;

    if ((prWebPInfo->prDecInParam->u4DecOutY > 0) && (prWebPInfo->prDecInParam->u4DecOutC > 0))
    {
        //HalFlushInvalidateDCache();
        HalInvalidateDCacheMultipleLine(prVpDecInfo->rVp8FrmHdr.u4CurYAddr,u4Pitch*32);
        HalInvalidateDCacheMultipleLine(prVpDecInfo->rVp8FrmHdr.u4CurCAddr,u4Pitch*16);

        if (prVpDecInfo->rVp8FrmHdr.rVp8DecParam.iLoopFilterLvl > 0)
        {
            if (u4MBY == 1)
            {
                u4StartRowY = 0;
                u4TotalCopyRowY = prVpDecInfo->rDecParam.u2Height;

                u4StartRowC = 0;
                u4TotalCopyRowC = u4TotalCopyRowY / 2;
            }
            else if (u4Idx == 0)
            {
                u4StartRowY = 0;
                u4TotalCopyRowY = 12;

                u4StartRowC = 0;
                u4TotalCopyRowC = 4;
            }
            else if (u4Idx == u4MBY - 1)
            {
                u4StartRowY = 12 + (u4Idx - 1) * 16;
                u4TotalCopyRowY = 20;

                u4StartRowC = 4 + (u4Idx - 1) * 8;
                u4TotalCopyRowC = 12;
            }
            else
            {
                u4StartRowY = 12 + (u4Idx - 1) * 16;
                u4TotalCopyRowY = 16;

                u4StartRowC = 4 + (u4Idx - 1) * 8;
                u4TotalCopyRowC = 8;
            }

            while (u4TotalCopyRowY > 0)
            {
                u4CurRow = u4StartRowY;            
                y = u4CurRow % 32;
                yBlk = u4CurRow / 32;
                pvDstY = (void*)(prWebPInfo->prDecInParam->u4DecOutY + u4Pitch * yBlk * 32 + y * 16);

                u4CurRow = u4StartRowY % 32;
                y = u4CurRow % 32;
                yBlk = u4CurRow / 32;
                pvSrcY = (void*)(prVpDecInfo->rVp8FrmHdr.u4CurYAddr + u4Pitch * yBlk * 32 + y * 16);

                u4RowNo = ((32 - y) >= u4TotalCopyRowY) ? u4TotalCopyRowY : (32 - y);

                for (x = 0; x < u4MBX; x++)
                {
                    x_memcpy((void*)((UINT32)pvDstY + x * 16 * 32), (void*)((UINT32)pvSrcY + x * 16 * 32), u4RowNo * 16);
                }

                u4TotalCopyRowY -= u4RowNo;
                u4StartRowY += u4RowNo;
            }

            while (u4TotalCopyRowC > 0)
            {       
                u4CurRow = u4StartRowC;
                y = u4CurRow % 16;
                yBlk = u4CurRow / 16;
                pvDstC = (void*)(prWebPInfo->prDecInParam->u4DecOutC + u4Pitch * yBlk * 16 + y * 16);
            
                u4CurRow = u4StartRowC % 16;
                y = u4CurRow % 16;
                yBlk = u4CurRow / 16;
                pvSrcC = (void*)(prVpDecInfo->rVp8FrmHdr.u4CurCAddr + u4Pitch * yBlk * 16 + y * 16);

                u4RowNo = ((16 - y) >= u4TotalCopyRowC) ? u4TotalCopyRowC : (16 - y);

                for (x = 0; x < u4MBX; x++)
                {
                    x_memcpy((void*)((UINT32)pvDstC + x * 16 * 16),  (void*)((UINT32)pvSrcC + x * 16 * 16), u4RowNo * 16);
                }
            
                u4TotalCopyRowC -= u4RowNo;
                u4StartRowC += u4RowNo;
            }
        }
        else
        {
            if (((u4Idx % 2) == 1) ||  (u4Idx == u4MBY - 1))
            {
                u4Idx = u4Idx & (~0x01ul);
                pvSrcY = (void*)(prVpDecInfo->rVp8FrmHdr.u4CurYAddr);
                pvSrcC = (void*)(prVpDecInfo->rVp8FrmHdr.u4CurCAddr);
                pvDstY = (void*)(prWebPInfo->prDecInParam->u4DecOutY + u4Idx * 16 * u4Pitch);
                pvDstC = (void*)(prWebPInfo->prDecInParam->u4DecOutC + u4Idx* 8 * u4Pitch);

                x_memcpy(pvDstY, pvSrcY, u4Pitch * 32);
                x_memcpy(pvDstC, pvSrcC, u4Pitch * 16);
            }
        }
    }
}
#endif

BOOL _VDEC_Vp8PsrGetEmptyFrameBuffer(VP8_DEC_PARAM_T *prDecParam)
{
   BOOL fgGetFram=FALSE;
   while(!fgGetFram)
   {
       prDecParam->ucCurFbId=FBM_GetEmptyFrameBuffer(prDecParam->ucFbgId,VDEC_VP8_WAIT_DISP_TIME);
       if(prDecParam->ucCurFbId==FBM_FB_ID_UNKNOWN)
       {
          ASSERT(0);
          return FALSE;
       }
       else if(prDecParam->ucCurFbId==prDecParam->ucGoldenFbId
               ||prDecParam->ucCurFbId==prDecParam->ucAltFbId
               ||prDecParam->ucCurFbId==prDecParam->ucLastFbId)
       {
          LOG(3,"!!!!!Warning:VP8 fb %d is a reference buffer!!!!\n",prDecParam->ucCurFbId);
#ifdef VDEC_VP8_FBMDEBUG
          if(prDecParam->ucCurFbId!=FBM_FB_ID_UNKNOWN)
          {
             LOG(5,"prDecParam->ucCurFbId=%d,status=%d,RefTime=%d\n",prDecParam->ucCurFbId,FBM_GetFrameBufferStatus(prDecParam->ucFbgId,prDecParam->ucCurFbId),prDecParam->RefTime[prDecParam->ucCurFbId]);
          }
   
          if(prDecParam->ucLastFbId!=FBM_FB_ID_UNKNOWN)
          {
              LOG(5,"prDecParam->ucLastFbId=%d,status=%d,RefTime=%d\n",prDecParam->ucLastFbId,FBM_GetFrameBufferStatus(prDecParam->ucFbgId,prDecParam->ucLastFbId),prDecParam->RefTime[prDecParam->ucLastFbId]);
          }
   
          if(prDecParam->ucGoldenFbId!=FBM_FB_ID_UNKNOWN)
          {
              LOG(5,"prDecParam->ucGoldenFbId=%d,status=%d,RefTime=%d\n",prDecParam->ucGoldenFbId,FBM_GetFrameBufferStatus(prDecParam->ucFbgId,prDecParam->ucGoldenFbId),prDecParam->RefTime[prDecParam->ucGoldenFbId]);
          }
   
          if(prDecParam->ucAltFbId!=FBM_FB_ID_UNKNOWN)
          {
              LOG(5,"prDecParam->ucAltFbId=%d,status=%d,RefTime=%d\n",prDecParam->ucAltFbId,FBM_GetFrameBufferStatus(prDecParam->ucFbgId,prDecParam->ucAltFbId),prDecParam->RefTime[prDecParam->ucAltFbId]);
          }
#endif

#ifdef CC_VP8_EMULATION
          FBM_SetFrameBufferStatus(prDecParam->ucFbgId,prDecParam->ucCurFbId,FBM_FB_STATUS_READY);
#else
          FBM_UpdateReferenceList(prDecParam->ucFbgId,prDecParam->ucCurFbId,TRUE);
#endif    
       }
       else
       {
#ifdef VDEC_VP8_FBMDEBUG
          LOG(5,"Get Frame buffer %d\n",prDecParam->ucCurFbId);
#endif
          fgGetFram=TRUE;
       }
   }

   return TRUE;
}

static BOOL _VDEC_Vp8PsrResolutionChange(VDEC_VP8_INFO_T * prDecInfo, UINT32 u4Widht, UINT32 u4Height)
{
    VP8_DEC_PARAM_T *prDecParam   = NULL;
    VDEC_ES_INFO_T *prVdecEsInfo  = NULL;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep  = NULL;
    FBM_SEQ_HDR_T* prFbmSeqHdr    = NULL;  
    UCHAR ucFbgType = FBM_FBG_TYPE_UNKNOWN, ucFbgId = FBM_FBG_ID_UNKNOWN;
    prDecParam=&prDecInfo->rDecParam;

    if(u4Widht > VDEC_VP8_HWMAX_WIDTH || u4Height > VDEC_VP8_HWMAX_HEIGHT)
    {
        LOG(2,"Frame tool large: Width=%d, Height=%d\n",u4Widht,u4Height);
		prDecParam->i4DecRet=E_VDEC_RES_NOT_SUPPORT;
        return FALSE;
    }

    if(u4Widht<VDEC_VP8_HWMIN_WIDTH  || u4Height< VDEC_VP8_HWMIN_HIGHT)
    {
        LOG(2,"Frame tool small: Width=%d, Height=%d\n",u4Widht,u4Height);
		prDecParam->i4DecRet=E_VDEC_RES_NOT_SUPPORT;
        return FALSE;
    }
#ifdef VP8_TIME_PROFILE
    HAL_GetTime(&prDecInfo->rTimeProfile.rTimeE);
    HAL_GetDeltaTime(&prDecInfo->rTimeProfile.rTimeDif,\
		&prDecInfo->rTimeProfile.rTimeS, \
		 &prDecInfo->rTimeProfile.rTimeE);
	prDecInfo->rTimeProfile.u4SwMicro+=prDecInfo->rTimeProfile.rTimeDif.u4Micros;	 
#endif
    

    //if(prDecParam->u4FrameCounter==0)
    {
        prVdecEsInfo = _VDEC_GetEsInfo(prDecInfo->uEsId);
        prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(prDecInfo->uEsId);
        if(FBM_ChkFrameBufferFlag(prDecParam->ucFbgId, (UINT32)FBM_FLAG_RELEASE_FBG))        
        {
            FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, prDecInfo->uEsId);    
            FBM_WaitUnlockFrameBuffer(prDecParam->ucFbgId, 20);    // block until display unlock                    
            FBM_ReleaseGroup(prDecParam->ucFbgId);
            prDecParam->ucFbgId = FBM_FBG_ID_UNKNOWN;
        }

        if(prDecParam->ucFbgId != FBM_FBG_ID_UNKNOWN)
        {
            if(u4Widht > prDecParam->u2Width || u4Height > prDecParam->u2Height)
            {
                FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, prDecInfo->uEsId);    
                FBM_WaitUnlockFrameBuffer(prDecParam->ucFbgId, 20);    // block until display unlock                    
                FBM_ReleaseGroup(prDecParam->ucFbgId);
                prDecParam->ucFbgId = FBM_FBG_ID_UNKNOWN;
            }
        }

        prDecParam->u2Width = u4Widht;
        prDecParam->u2Height= u4Height;

        if(prDecParam->ucFbgId == FBM_FBG_ID_UNKNOWN)
        {       
            if(prDecParam->u4FrameCounter>0)
            {
				x_memset(prDecParam->RefTime,(UINT8)-1,VDEC_VP8_FBNUM);
				prDecParam->ucCurFbId	= FBM_FB_ID_UNKNOWN;
				prDecParam->ucGoldenFbId= FBM_FB_ID_UNKNOWN;
				prDecParam->ucLastFbId	= FBM_FB_ID_UNKNOWN;
				prDecParam->ucAltFbId	= FBM_FB_ID_UNKNOWN;
            }
			
            ucFbgType= FBM_SelectGroupType(u4Widht, u4Height);
            if(ucFbgType == FBM_FBG_TYPE_UNKNOWN)
            {
                LOG(2,"Select FBM Grouptype return FBM_FGB_TYPE_UNKNOWN\n");
                return FALSE;
            }

            {
                FBM_CREATE_FBG_PAR_T rPar;
                x_memset(&rPar, 0, sizeof(FBM_CREATE_FBG_PAR_T));
                rPar.u1VdecId = (UINT8)prDecInfo->uEsId;
                if((prVdecEsInfoKeep->fgVPush) && (prVdecEsInfoKeep->fgVPushDecodeOnly))
                {
                    rPar.u1AppMode = FBM_FBG_APP_OMX;
                }
                else if(prVdecEsInfoKeep->fgVPush)
                {
                    rPar.u1AppMode = FBM_FBG_APP_OMX_DISP;
                }
                else
                {
                    rPar.u1AppMode = FBM_FBG_APP_NORMAL;
                }
                ucFbgId =
                    FBM_CreateGroupExt(ucFbgType, FBM_VDEC_VP8, 0, 0, &rPar);
            }
            
            if(ucFbgId == FBM_FBG_ID_UNKNOWN)
            {
                LOG(2,"Create FBM Group FBM_FBG_ID_UNKNOWN\n");
				prDecParam->i4DecRet=E_VDEC_RES_NOT_SUPPORT;
                return FALSE;
            }

            FBM_SetPlayMode(ucFbgId, FBM_FBG_MM_MODE);
            if(prVdecEsInfoKeep->fgVPush)
            {
                if(prVdecEsInfoKeep->fgVPushDecodeOnly)
                {
                    FBM_SetPlayMode(ucFbgId,FBM_FBG_NOTDISPLAY_MODE);
                }
                FBM_RegFbgCbFunc(ucFbgId,
                    FBM_CB_FUNC_FB_READY_EX_IND, (UINT32)_VPUSH_DecodeDone, prDecInfo->uEsId);
            }
            prVdecEsInfo->ucFbgId = ucFbgId;
            FBM_SetSyncStc(ucFbgId, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId);
            FBM_FbgChgNotify(ucFbgId, prDecInfo->uEsId);
            prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
			
            prDecParam->ucFbgId = ucFbgId;
			prDecParam->ucFbgType=0xff;
        }

        if(ucFbgType != prDecParam->ucFbgType)
        {
            UINT32 u4WorkingBuffer  = 0;
            UINT32 u4BufferSize     = 0;

            FBM_GetWorkingBuffer(prDecParam->ucFbgId, &u4WorkingBuffer, &u4BufferSize);

            if(u4BufferSize<=VDEC_VP8_HWWORKBUF_SIZE)
            {
                u4BufferSize     = 0;
                u4WorkingBuffer  = 0;
            }
            else
            {
                VDEC_CLRFLG(prDecParam->u4Flag, VP8_DEC_FLG_BSPWKBUF);
                prDecParam->u4WorkBuf = u4WorkingBuffer;
            }

            if(u4BufferSize == 0)
            {
                u4WorkingBuffer = (UINT32)BSP_AllocAlignedDmaMemory(VDEC_VP8_HWWORKBUF_SIZE, 16);
                ASSERT(u4WorkingBuffer);
                if(!u4WorkingBuffer)
                {
                    prDecParam->u4WorkBuf = 0;
                    VDEC_CLRFLG(prDecParam->u4Flag, VP8_DEC_FLG_BSPWKBUF);
                    return FALSE;
                }
                else
                {
                    VDEC_SETFLG(prDecParam->u4Flag, VP8_DEC_FLG_BSPWKBUF);
                    prDecParam->u4WorkBuf = u4WorkingBuffer;
                }
            }

#ifdef VP8_64X32_BLK_MODE
            prDecParam->u4FramePitch=((prDecParam->u2Width+63)>>6)<<6;
#else
            prDecParam->u4FramePitch=((prDecParam->u2Width+15)>>4)<<4;
#endif
            prDecParam->ucFbgType=ucFbgType;
            if (prFbmSeqHdr != NULL)
            {
                if (prVdecEsInfo != 0)
                {
                	UINT16 u2NewHeight=0,u2NewWidht=0,u2LineSize=0;
                    UINT32 u4fps = 0;
					u2NewWidht=prDecParam->u2Width;
					u2NewHeight=prDecParam->u2Height;
					u2LineSize= (UINT16)prDecParam->u4FramePitch;
					prFbmSeqHdr->fgResizeSmallPic=FALSE;
					if(prVdecEsInfo->rMMSeqInfo.e_src_asp!=VDEC_ASPECT_RATIO_NONE 
						&&prVdecEsInfo->rMMSeqInfo.e_src_asp<VDEC_ASPECT_RATIO_MAX)
					{
                        prFbmSeqHdr->ucAspRatInf=prVdecEsInfo->rMMSeqInfo.e_src_asp;
					}
					else
					{
						prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_1_1;		 // 16:9
					}
					
                    switch (prVdecEsInfo->rMMSeqInfo.e_frame_rate)
                    {
                    case VDEC_SRC_FRAME_RATE_23_976:
                        u4fps = 23976;
                        break;
                    case VDEC_SRC_FRAME_RATE_24:
                        u4fps = 24000;
                        break;
                    case VDEC_SRC_FRAME_RATE_25:
                        u4fps = 25000;
                        break;
                    case VDEC_SRC_FRAME_RATE_29_97:
                        u4fps = 29970;
                        break;
                    case VDEC_SRC_FRAME_RATE_30:
                        u4fps = 30000;
                        break;
                    case VDEC_SRC_FRAME_RATE_50:
                        u4fps = 50000;
                        break;
                    case VDEC_SRC_FRAME_RATE_59_94:
                        u4fps = 59940;
                        break;
                    case VDEC_SRC_FRAME_RATE_60:
                        u4fps = 60000;
                        break;
                    default:
                        u4fps = 0;
                        break;
                    }

                    if (u4fps != 0)
                    {
                        prDecParam->u4DeltaPTS = (90000000/u4fps);
                        u4fps /= 1000;
                    }
                    else
                    {
                        prDecParam->u4DeltaPTS = (90000000/VDEC_VP8DEF_FRAME_RATE);
						u4fps=VDEC_VP8DEF_FRAME_RATE/1000;
                    } 

                    switch (u4fps)
                    {
                    case 22:
                    case 23:
                        prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_24_;
                        break;
                    case 24:
                        prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_24;
                        break;
                    case 25:
                        prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_25;
                        break;
                    case 30:
                        prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_30;
                        break;
                    case 29:
                        prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_30_;
                        break;
                    case 50:
                        prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_50;
                        break;
                    case 60:
                        prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_60;
                        break;
                    case 59:
                        prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_60_;
                        break;
                    default:
                        prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_UNKNOWN;
                        break;
                    }
					
#if defined(CC_RESIZE_SMALL_IMAGE) && !defined(CC_VP8_EMULATION)
					if((prDecParam->u2Width < FBM_IMG_RESIZE_LIMITATION ||
					    prDecParam->u2Width > FBM_IMG_RESIZE_LIMITATION_MAX||
					    prDecParam->u2Height< FBM_IMG_RESIZE_LIMITATION||
					    prDecParam->u2Height> FBM_IMG_RESIZE_LIMITATION_MAX) &&
					    (!((prVdecEsInfoKeep->fgVPush) && (prVdecEsInfoKeep->fgVPushDecodeOnly))))
					{
						if (prDecParam->u2Width<FBM_IMG_RESIZE_LIMITATION)
						{
						    u2NewWidht=FBM_IMG_RESIZE_LIMITATION;
							u2LineSize=FBM_IMG_RESIZE_LIMITATION;
						}
						else if (prDecParam->u2Width>FBM_IMG_RESIZE_LIMITATION_MAX)
						{
						    u2NewWidht=FBM_IMG_RESIZE_LIMITATION_MAX;
							u2LineSize=FBM_IMG_RESIZE_LIMITATION_MAX;
						}
						
						if(prDecParam->u2Height < FBM_IMG_RESIZE_LIMITATION)
					    {
					        u2NewHeight=FBM_IMG_RESIZE_LIMITATION;
						}
						else if(prDecParam->u2Height > FBM_IMG_RESIZE_LIMITATION_MAX)
						{
						    u2NewHeight=FBM_IMG_RESIZE_LIMITATION_MAX;
						}
						prFbmSeqHdr->fgResizeSmallPic = TRUE;
					}
#endif
                    prFbmSeqHdr->fgProgressiveSeq = TRUE;
                    prFbmSeqHdr->u2HSize         = u2NewWidht;
                    prFbmSeqHdr->u2VSize         = u2NewHeight;
                    prFbmSeqHdr->u2OrgHSize      = u2NewWidht;
                    prFbmSeqHdr->u2OrgVSize      = u2NewHeight;
                    prFbmSeqHdr->u2LineSize      = u2LineSize;
                    prFbmSeqHdr->u4YAddrOffset   = 0;
                    prFbmSeqHdr->u4CAddrOffset   = 0;
                    prFbmSeqHdr->fgRasterOrder   = 0;					
                }
                else
                {  
                    prFbmSeqHdr->ucAspRatInf         = 1;        // 16:9
                    prFbmSeqHdr->ucFrmRatCod         = 5;        // 30 fps
                    prFbmSeqHdr->fgProgressiveSeq    = 1;
                    prFbmSeqHdr->u2HSize             = prDecParam->u2Width;
                    prFbmSeqHdr->u2VSize             = prDecParam->u2Height;
                    prFbmSeqHdr->u2OrgHSize          = prDecParam->u2Width;
                    prFbmSeqHdr->u2OrgVSize          = prDecParam->u2Height;
                    prFbmSeqHdr->u2LineSize          = prDecParam->u4FramePitch;
                    prFbmSeqHdr->u4YAddrOffset       = 0;
                    prFbmSeqHdr->u4CAddrOffset       = 0;
                    prFbmSeqHdr->fgRasterOrder       = 0;
                }
            }

		    if(_VDEC_Vp8PsrGetEmptyFrameBuffer(prDecParam)==FALSE)
            {
	          LOG(3,"VP8 _VDEC_Vp8GetEmptyFrameBuffer return false1\n");
			  prDecParam->i4DecRet=E_VDEC_FAIL;
			  return FALSE;
            }
        }
    }
#ifdef VP8_TIME_PROFILE
		HAL_GetTime(&prDecInfo->rTimeProfile.rTimeS);
#endif
    return TRUE;
}

static VOID _VDEC_Vp8PsrUpdateFrame(VDEC_VP8_INFO_T *prVdecInfo,BOOL fgDecResult)
{
   VDEC_INFO_VP8_FRM_HDR_T *prFrmHdr = &prVdecInfo->rVp8FrmHdr;
   VDEC_PARAM_VP8DEC_T* prVp8DecParam=&prFrmHdr->rVp8DecParam;
   VP8_DEC_PARAM_T *prDecParam=&prVdecInfo->rDecParam;
   FBM_PIC_HDR_T *prFbmPicHdr=NULL;
   VDEC_ES_INFO_T* prVdecEsInfo=NULL;
   UINT32 u4FlagParam=prFrmHdr->rVp8DecParam.u4FlagParam;
   UINT32 u4LastCAddr=0;
   UINT8 uFbIndex,uDrop;

   if(fgDecResult==FALSE)
   {
       uDrop=0;
       if(prDecParam->ucLastFbId==FBM_FB_ID_UNKNOWN)
       {
          uDrop=1;
       }
	   else
	   {
	       FBM_GetFrameBufferAddr(prDecParam->ucFbgId,prDecParam->ucCurFbId,&prFrmHdr->u4CurYAddr,&prFrmHdr->u4CurCAddr);
           FBM_GetFrameBufferAddr(prDecParam->ucFbgId,prDecParam->ucLastFbId,&prFrmHdr->u4LstYAddr,&u4LastCAddr);
		   ASSERT(prFrmHdr->u4CurYAddr!=0&&prFrmHdr->u4LstYAddr!=0);
		   if(prFrmHdr->u4CurYAddr!=0&&prFrmHdr->u4LstYAddr!=0)
		   {
			   IMGRZ_DMA(prFrmHdr->u4LstYAddr,prFrmHdr->u4CurYAddr,prDecParam->u4FramePitch,prDecParam->u2Height);
			   IMGRZ_DMA(u4LastCAddr,prFrmHdr->u4CurCAddr,prDecParam->u4FramePitch,(prDecParam->u2Height+1)>>1);
			   FBM_SetFrameBufferStatus(prDecParam->ucFbgId,prDecParam->ucCurFbId, FBM_FB_STATUS_READY);
		   }
		   else
		   {
			   uDrop=1;
		   }
	   }

	   if(uDrop)
	   {
		   FBM_SetFrameBufferStatus(prDecParam->ucFbgId,prDecParam->ucCurFbId, FBM_FB_STATUS_EMPTY);
#ifndef CC_VP8_EMULATION
		   FBM_UpdateReferenceList(prDecParam->ucFbgId,prDecParam->ucCurFbId,FALSE);
#endif
		   if(prDecParam->ucCurFbId!=FBM_FB_ID_UNKNOWN && prDecParam->ucCurFbId<VDEC_VP8_FBNUM)
		   {
			   prDecParam->RefTime[prDecParam->ucCurFbId]-=1;
		   }
		   return;
	   }
   }
   else
   {
       FBM_SetFrameBufferStatus(prDecParam->ucFbgId,prDecParam->ucCurFbId, FBM_FB_STATUS_READY);
   }
   
   if(prVp8DecParam->uCopyBuf2Arf==1)
    {
        if(prDecParam->ucAltFbId!=FBM_FB_ID_UNKNOWN && prDecParam->ucAltFbId<VDEC_VP8_FBNUM)
        {
           prDecParam->RefTime[prDecParam->ucAltFbId]-=1;
        }
        
        if(prDecParam->ucLastFbId!=FBM_FB_ID_UNKNOWN && prDecParam->ucLastFbId<VDEC_VP8_FBNUM)
        {
           prDecParam->ucAltFbId=prDecParam->ucLastFbId;
           prDecParam->RefTime[prDecParam->ucAltFbId]+=1;
        }
    }
    else if(prVp8DecParam->uCopyBuf2Arf==2)
    {
        if(prDecParam->ucAltFbId!=FBM_FB_ID_UNKNOWN && prDecParam->ucAltFbId<VDEC_VP8_FBNUM)
        {
           prDecParam->RefTime[prDecParam->ucAltFbId]-=1;
        }
        
        if(prDecParam->ucGoldenFbId!=FBM_FB_ID_UNKNOWN && prDecParam->ucGoldenFbId<VDEC_VP8_FBNUM)
        {
           prDecParam->ucAltFbId=prDecParam->ucGoldenFbId;
           prDecParam->RefTime[prDecParam->ucAltFbId]+=1;
        }
    }

    if(prVp8DecParam->uCopyBuf2Gf==1) // last to gf
    {
        if(prDecParam->ucGoldenFbId!=FBM_FB_ID_UNKNOWN && prDecParam->ucGoldenFbId<VDEC_VP8_FBNUM)
        {
           prDecParam->RefTime[prDecParam->ucGoldenFbId]-=1;
        }
        
        if(prDecParam->ucLastFbId!=FBM_FB_ID_UNKNOWN && prDecParam->ucLastFbId<VDEC_VP8_FBNUM)
        {
           prDecParam->ucGoldenFbId=prDecParam->ucLastFbId;
           prDecParam->RefTime[prDecParam->ucGoldenFbId]+=1;
        }
    }
    else if(prVp8DecParam->uCopyBuf2Gf==2) // alt to gf
    {   
        if(prDecParam->ucGoldenFbId!=FBM_FB_ID_UNKNOWN && prDecParam->ucGoldenFbId<VDEC_VP8_FBNUM)
        {
            prDecParam->RefTime[prDecParam->ucGoldenFbId]-=1;
        }
        
        if(prDecParam->ucAltFbId!=FBM_FB_ID_UNKNOWN && prDecParam->ucAltFbId<VDEC_VP8_FBNUM)
        {
           prDecParam->ucGoldenFbId=prDecParam->ucAltFbId;
           prDecParam->RefTime[prDecParam->ucGoldenFbId]+=1;
        }
   }

   if(VDEC_FLGSET(u4FlagParam,VP8PARAM_REFRESH_GOLDEN))
   {
       if(prDecParam->ucCurFbId!=FBM_FB_ID_UNKNOWN && prDecParam->ucCurFbId<VDEC_VP8_FBNUM)
       {
           prDecParam->RefTime[prDecParam->ucCurFbId]+=1;
       }
       
       if(prDecParam->ucGoldenFbId!=FBM_FB_ID_UNKNOWN && prDecParam->ucGoldenFbId<VDEC_VP8_FBNUM)
       {
           prDecParam->RefTime[prDecParam->ucGoldenFbId]-=1;
       }
       
       prDecParam->ucGoldenFbId=prDecParam->ucCurFbId;
   }

   if(VDEC_FLGSET(u4FlagParam,VP8PARAM_REFRESH_ALTRF))
   {
       if(prDecParam->ucCurFbId!=FBM_FB_ID_UNKNOWN && prDecParam->ucCurFbId<VDEC_VP8_FBNUM)
       {
           prDecParam->RefTime[prDecParam->ucCurFbId]+=1;
       }
       
       if(prDecParam->ucAltFbId!=FBM_FB_ID_UNKNOWN && prDecParam->ucAltFbId<VDEC_VP8_FBNUM)
       {
           prDecParam->RefTime[prDecParam->ucAltFbId]-=1;
       }
       prDecParam->ucAltFbId=prDecParam->ucCurFbId;
   }
   
   if(VDEC_FLGSET(u4FlagParam,VP8PARAM_REFRESH_LASTFRAME))
   {
       if(prDecParam->ucCurFbId!=FBM_FB_ID_UNKNOWN && prDecParam->ucCurFbId<VDEC_VP8_FBNUM)
       {
           prDecParam->RefTime[prDecParam->ucCurFbId]+=1;
       }
       
       if(prDecParam->ucLastFbId!=FBM_FB_ID_UNKNOWN && prDecParam->ucLastFbId<VDEC_VP8_FBNUM)
       {
           prDecParam->RefTime[prDecParam->ucLastFbId]-=1;
       }
       prDecParam->ucLastFbId=prDecParam->ucCurFbId;
   }
   

   for(uFbIndex=0;uFbIndex<VDEC_VP8_FBNUM;uFbIndex++)
   {
      if(prDecParam->RefTime[uFbIndex]<0)
      {
         continue;
      }

      if(prDecParam->RefTime[uFbIndex]==0)
      {
#ifdef CC_VP8_EMULATION
#ifdef VDEC_VP8_FBMDEBUG
       LOG(5,"Release Frame buffer:%d\n",uFbIndex);
#endif
       FBM_SetFrameBufferStatus(prDecParam->ucFbgId,uFbIndex,FBM_FB_STATUS_EMPTY);
#else
	   FBM_UpdateReferenceList(prDecParam->ucFbgId,uFbIndex,FALSE);
#endif
	   prDecParam->RefTime[uFbIndex]=-1;
      }
      else
      {
         ASSERT(prDecParam->RefTime[uFbIndex]<=3);
		// if in emulation flow, frame needn't to be display, it always is ready status.
#ifndef CC_VP8_EMULATION
         FBM_UpdateReferenceList(prDecParam->ucFbgId,uFbIndex,TRUE);
#endif
      }
   }
   
   if(prFrmHdr->uShowFrame)
   {
#ifndef CC_VP8_EMULATION
        if(!VDEC_IsNotDisplay(prVdecInfo->uEsId,prDecParam->u4DispPts, 0, 0))
        {
            prVdecEsInfo = _VDEC_GetEsInfo(prVdecInfo->uEsId);
            prFbmPicHdr=FBM_GetFrameBufferPicHdr(prDecParam->ucFbgId,prDecParam->ucCurFbId);
            if(!prVdecEsInfo || !prFbmPicHdr)
            {
               return;
            }
            prFbmPicHdr->fgProgressiveFrm=TRUE;
            prFbmPicHdr->ucPicStruct=3;
            prFbmPicHdr->ucTrueZeroPTS = FBM_USE_ZERO_PTS_IF_ZERO;
            prVdecEsInfo->ucFbgId = prDecParam->ucFbgId;
            prVdecEsInfo->ucFbId  = prDecParam->ucCurFbId;
            if(prFbmPicHdr->rExtra.fgValid)
            {
                x_memcpy(&prFbmPicHdr->rExtra,&prVdecEsInfo->rExtra,sizeof(FBM_PIC_EXTRA_INFO_T));
            }

            prFbmPicHdr->u8OffsetI = prVdecEsInfo->u8OffsetI;
            prFbmPicHdr->u4Timestap = prVdecEsInfo->u4Timestap;
            prFbmPicHdr->u4PTS = prDecParam->u4DispPts;
            prFbmPicHdr->u8Offset = prVdecEsInfo->u8Offset;
            prFbmPicHdr->u8OffsetDisp = prVdecEsInfo->u8OffsetLast;
            prFbmPicHdr->u2DecodingOrder = prVdecEsInfo->u2DecodingOrder;
            if(prVdecInfo->rPesInfo.fgEos)
            {
                FBM_SetFrameBufferPicFlag(prDecParam->ucFbgId,prDecParam->ucCurFbId,FBM_MM_EOS_FLAG);
            }
#ifdef CC_RESIZE_SMALL_IMAGE
            prFbmPicHdr->u4PicWidth = prDecParam->u2Width;
            prFbmPicHdr->u4PicHeight = prDecParam->u2Height;
            prFbmPicHdr->u4PicWidthPitch = prDecParam->u4FramePitch;
#endif
            FBM_SetFrameBufferStatus(prDecParam->ucFbgId,prDecParam->ucCurFbId, FBM_FB_STATUS_DISPLAYQ);
		    prVdecEsInfo->u4DisplayQPicCnt++;
        }
        else
        {
            FBM_SetFrameBufferStatus(prDecParam->ucFbgId,prDecParam->ucCurFbId, FBM_FB_STATUS_EMPTY);
            LOG(3,"Vp8 Frame Pts 0x%x is not displayed\n",prDecParam->u4DispPts);
        }
#endif
   }
   else
   {
#ifndef CC_VP8_EMULATION
      FBM_SetFrameBufferStatus(prDecParam->ucFbgId,prDecParam->ucCurFbId, FBM_FB_STATUS_EMPTY);
      prDecParam->i4DecRet=E_VDEC_SKIPFRAME;
#endif
   }
   
#ifdef VDEC_VP8_FBMDEBUG
   if(prDecParam->ucCurFbId!=FBM_FB_ID_UNKNOWN)
   {
       LOG(5,"prDecParam->ucCurFbId=%d,status=%d,RefTime=%d\n",prDecParam->ucCurFbId,FBM_GetFrameBufferStatus(prDecParam->ucFbgId,prDecParam->ucCurFbId),prDecParam->RefTime[prDecParam->ucCurFbId]);
   }
   
   if(prDecParam->ucLastFbId!=FBM_FB_ID_UNKNOWN)
   {
       LOG(5,"prDecParam->ucLastFbId=%d,status=%d,RefTime=%d\n",prDecParam->ucLastFbId,FBM_GetFrameBufferStatus(prDecParam->ucFbgId,prDecParam->ucLastFbId),prDecParam->RefTime[prDecParam->ucLastFbId]);
   }
   
   if(prDecParam->ucGoldenFbId!=FBM_FB_ID_UNKNOWN)
   {
       LOG(5,"prDecParam->ucGoldenFbId=%d,status=%d,RefTime=%d\n",prDecParam->ucGoldenFbId,FBM_GetFrameBufferStatus(prDecParam->ucFbgId,prDecParam->ucGoldenFbId),prDecParam->RefTime[prDecParam->ucGoldenFbId]);
   }
   
   if(prDecParam->ucAltFbId!=FBM_FB_ID_UNKNOWN)
   {
       LOG(5,"prDecParam->ucAltFbId=%d,status=%d,RefTime=%d\n",prDecParam->ucAltFbId,FBM_GetFrameBufferStatus(prDecParam->ucFbgId,prDecParam->ucAltFbId),prDecParam->RefTime[prDecParam->ucAltFbId]);
   }
#endif
   return;
}

VOID _VDEC_Vp8PsrIsrInit(VDEC_VP8_INFO_T *prVpDecInfo)
{
   return _Vdec_VP8IsrInit(prVpDecInfo);
}

BOOL _VDEC_Vp8PsrInit(VDEC_VP8_INFO_T *prVpDecInfo,INT32 i4DecId)
{
   return _Vdec_VP8HwInit(prVpDecInfo);
}

BOOL _VDEC_Vp8PsrInstanceChange(VDEC_VP8_INFO_T *prVpDecInfo,BOOL fgChg2Multi)
{
   
   return TRUE;
}

BOOL _VDEC_Vp8PsrHeaderInfo(VDEC_VP8_INFO_T *prVpDecInfo)
{
   VP8_DEC_PARAM_T *prDecParam=&prVpDecInfo->rDecParam;
   BOOL fgRet=TRUE;

   fgRet=_Vdec_VP8HwParseHeader(prVpDecInfo);   
   
   if(!fgRet)
   {
     LOG(3,"VP8 Header parse return false\n");
	 prDecParam->i4DecRet=E_VDEC_FAIL;
	 if(prDecParam->ucCurFbId!=FBM_FB_ID_UNKNOWN)
	 {
        _VDEC_Vp8PsrUpdateFrame(prVpDecInfo,FALSE);
	 }
     return FALSE;
   }
   
   if(!VDEC_FLGSET(prDecParam->u4Flag,VP8_DEC_FLG_STARTED))
   {
      if(!prDecParam->fgKeyFrame)
      {
          LOG(3,"VP8 Header parse not started from key frame\n");
          return FALSE;
      }
      else
      {
         VDEC_SETFLG(prDecParam->u4Flag,VP8_DEC_FLG_STARTED);
      }
   }

   if(prDecParam->fgKeyFrame)
   {
      prDecParam->u4KekFrameIndex=prDecParam->u4FrameCounter;
   }

   
   if(prDecParam->u4FrameCounter==0 || prDecParam->u2NewWidth!=prDecParam->u2Width ||prDecParam->u2NewHeight!=prDecParam->u2Height)
   {
      LOG(5,"vp8 resolution change\n");
      if(_VDEC_Vp8PsrResolutionChange(prVpDecInfo,prDecParam->u2NewWidth,prDecParam->u2NewHeight)==FALSE)
      {
        return FALSE;
      }
   }
   
   if(prDecParam->ucFbgId!=FBM_FBG_ID_UNKNOWN)
   {
      if(FBM_ChkFrameBufferFlag(prDecParam->ucFbgId,(UINT32)FBM_FLAG_RESET))
      {
         LOG(3,"vp8 fbm reset1\n");
         FBM_ResetGroup(prDecParam->ucFbgId);
         FBM_ClrFrameBufferFlag(prDecParam->ucFbgId, (UINT32)FBM_FLAG_RESET);
         FBM_ClrFrameBufferFlag(prDecParam->ucFbgId, (UINT32)FBM_FLAG_NOT_READY);  
      }
   }
    
   prDecParam->u4DispPts=prVpDecInfo->rPesInfo.u4PTS;
   if(prDecParam->ucCurFbId >= VDEC_VP8_FBNUM)
   {
      prDecParam->i4DecRet=E_VDEC_FAIL;
      return FALSE;
   }

   if(prDecParam->RefTime[prDecParam->ucCurFbId]<0)
   {
      prDecParam->RefTime[prDecParam->ucCurFbId]=0;
   }

   prDecParam->u4FrameCounter++;
   return TRUE;
}

BOOL _VDEC_Vp8PsrDecode(VDEC_VP8_INFO_T *prVpDecInfo)
{
  BOOL fgRet=TRUE;
  //Printf("Frame %d,FrameType=%d\n",prVpDecInfo->rDecParam.u4FrameCounter-1,prVpDecInfo->rVp8FrmHdr.uShowFrame);
  fgRet=_Vdec_VP8HwStartDecode(prVpDecInfo);
  return fgRet;
}

BOOL _VDEC_Vp8PsrFin(VDEC_VP8_INFO_T *prVpDecInfo)
{ 
   BOOL fgRet=TRUE;
   prVpDecInfo->rDecParam.i4DecRet=E_VDEC_OK;
   fgRet=_Vdec_VP8HwWaitDecodeDone(prVpDecInfo);
#ifdef __MODEL_slt__
   if(prVpDecInfo->rDecParam.u4FrameCounter == 7)
   {
      VP8_DEC_PARAM_T *prDecParam=&prVpDecInfo->rDecParam;
      if(fgRet)
      {
           if(prDecParam->pu4CrcVal[0][0]==0xE42C1F7C
           &&prDecParam->pu4CrcVal[0][1]==0x6CDF4134
           &&prDecParam->pu4CrcVal[0][2]==0x824B17D9
           &&prDecParam->pu4CrcVal[0][3]==0x835AF4B9
           &&prDecParam->pu4CrcVal[1][0]==0x6B04D9DC
           &&prDecParam->pu4CrcVal[1][1]==0xF80A29B6
           &&prDecParam->pu4CrcVal[1][2]==0x96AEAE34
           &&prDecParam->pu4CrcVal[1][3]==0x11DEE582)
           {
                   Printf("\n\nMMOK!!\n\n\n");
           }
           else
           {
                   Printf("\n\nMMNG!!\n\n\n");
           }
       }
       else
       {
           Printf("\n\nMMNG!!\n\n\n");
       }
   }
#endif

   if(!fgRet)
   {
      prVpDecInfo->rDecParam.i4DecRet=E_VDEC_FAIL;
      ASSERT(0);
   }

   _VDEC_Vp8PsrUpdateFrame(prVpDecInfo,fgRet);

#ifdef VP8_TIME_PROFILE
   HAL_GetTime(&prVpDecInfo->rTimeProfile.rTimeE);
   HAL_GetDeltaTime(&prVpDecInfo->rTimeProfile.rTimeDif,\
	  	&prVpDecInfo->rTimeProfile.rTimeS,\
	  	&prVpDecInfo->rTimeProfile.rTimeE);
   prVpDecInfo->rTimeProfile.u4SwMicro+=prVpDecInfo->rTimeProfile.rTimeDif.u4Micros;	
   if(prVpDecInfo->rTimeProfile.u4SwMicro+prVpDecInfo->rTimeProfile.u4DecMs>16000)
   { 
     Printf("Frame (%d,%d) Decode time %d=(%d,%d+%d)\n", \
	 	prVpDecInfo->rDecParam.u4FrameCounter,prVpDecInfo->rDecParam.fgKeyFrame,\
	 	prVpDecInfo->rTimeProfile.u4DecMs+prVpDecInfo->rTimeProfile.u4SwMicro, \
	 	prVpDecInfo->rTimeProfile.u4DecMs,\
	    prVpDecInfo->rTimeProfile.u4SwMicro-prVpDecInfo->rTimeProfile.rTimeDif.u4Micros, \
	    prVpDecInfo->rTimeProfile.rTimeDif.u4Micros);
   }
   //_VDEC_Vp8TimeProfileInfo(prVpDecInfo->eDecType,prVpDecInfo->rDecParam.fgKeyFrame,&prVpDecInfo->rTimeProfile);
#endif

#ifdef VP8_EMUCOVERAGE_STATISTIC
   prVpDecInfo->rDecParam.i4DecRet=E_VDEC_SKIPFRAME;
#endif

#ifndef CC_VP8_EMULATION
   UNUSED(VDEC_CheckStopStatus(prVpDecInfo->uEsId, prVpDecInfo->rDecParam.u4DispPts, 0));
#endif
   _Vdec_VP8HwPowerOff();
   return fgRet;
}

BOOL _VDEC_Vp8PsrGetPicInfo(VDEC_VP8_INFO_T *prVpDecInfo,UINT32 *pu4YAddr,UINT32 *pu4CAddr)
{
   FBM_GetFrameBufferAddr(prVpDecInfo->rDecParam.ucFbgId,prVpDecInfo->rDecParam.ucCurFbId,pu4YAddr,pu4CAddr);
   return TRUE;
}

VOID _VDEC_Vp8PsrDumpReg(VDEC_VP8_INFO_T *prVpDecInfo)
{
   return _Vdec_VP8HwPrintRegInfo(prVpDecInfo);
}



static VOID _VDEC_Vp8PsrWebpBufInfo(WEBP_BUF_INFO *prBufInfo,UINT32 u4Index,UINT32 u4CurIndex,UINT32 u4PartData,UCHAR *prPrex)
{
  UINT32 u4DataSize=0;
  Printf("************%s:Start**************\n",prPrex);
  Printf("InfoIndex=%d,CurIndex=%d,PartSize=0x%x\n",u4Index,u4CurIndex,u4PartData);
  Printf("Start=0x%x,End=0x%x,Size=0x%x\n",prBufInfo->u4Start,prBufInfo->u4End,prBufInfo->u4End-prBufInfo->u4Start);
  Printf("ReadPos=0x%x,WritePos=0x%x,RealWritePos=0x%x,UsedByte=0x%x\n",prBufInfo->u4ReadPos,prBufInfo->u4WritePos,prBufInfo->u4RealWritePos,prBufInfo->u4UsedBitCount/8);
  Printf("ReadOffset=0x%x,RequestedSize=0x%x,RemainSize=0x%x\n",prBufInfo->u4ReadOffset,prBufInfo->u4RequestedSize,u4PartData-prBufInfo->u4RequestedSize);
  u4DataSize=prBufInfo->u4ReadPos<=prBufInfo->u4WritePos ? (prBufInfo->u4WritePos-prBufInfo->u4ReadPos) : (prBufInfo->u4End-prBufInfo->u4ReadPos+prBufInfo->u4WritePos-prBufInfo->u4Start);
  Printf("DataSize=0x%x,EmptySize=0x%x\n",u4DataSize,prBufInfo->u4End-prBufInfo->u4Start-u4DataSize);
  Printf("************%s:End****************\n",prPrex);
}


static VOID _VDEC_Vp8PsrWebPResizeCfg(RZ_VDO_SCL_PARAM_SET_T *prSclParam,VDEC_VP8_INFO_T *prVpDecInfo)
{
    WEBP_DEC_INFO_T *prWebPInfo=&prVpDecInfo->rWebPInfo;
	WEBP_DECIN_PARAM_T *prDecInParam=prWebPInfo->prDecInParam;
	WEBP_DECOUT_PARAM_T *prDecOutParam=prWebPInfo->prDecOutParam;
	x_memset(prSclParam,0,sizeof(RZ_VDO_SCL_PARAM_SET_T));
	if(VDEC_FLGSET(prDecInParam->u4Flag,WEBP_DEC_FLAG_KEEPRATIO))
	{
		UINT32 u4Scale=0;
		u4Scale=(prDecInParam->u4Height*prVpDecInfo->rDecParam.u2Width)/prVpDecInfo->rDecParam.u2Height;
		if(u4Scale<=prDecInParam->u4Width)
		{
		   prDecInParam->u4StartX+=(prDecInParam->u4Width-u4Scale)>>1;
           prDecInParam->u4Width=u4Scale;
		}
		else
		{
		   u4Scale=(prDecInParam->u4Width*prVpDecInfo->rDecParam.u2Height)/prVpDecInfo->rDecParam.u2Width;
           if(u4Scale<=prDecInParam->u4Height)
           {
			 prDecInParam->u4StartY+=(prDecInParam->u4Height-u4Scale)>>1;
           }
		   prDecInParam->u4Height=u4Scale;
		}
	}
	
    prSclParam->u4IsRsIn  = FALSE;
    prSclParam->u4YSrcBase    = prWebPInfo->rBufLayOut.u4ColorBufY;
    prSclParam->u4YSrcBufLen  = ((prVpDecInfo->rDecParam.u2Width+15)>>4)*16;
    prSclParam->u4CSrcBufLen  = prSclParam->u4YSrcBufLen;
    prSclParam->u4YSrcHOffset = 0;
    prSclParam->u4YSrcVOffset = 0;
#ifdef CC_WEBP_EMULATION
    prSclParam->u4YSrcW       = prVpDecInfo->rDecParam.u2Width-(prVpDecInfo->rDecParam.u2Width&0x1);
    prSclParam->u4YSrcH       = prVpDecInfo->rDecParam.u2Height-(prVpDecInfo->rDecParam.u2Height&0x1);
#else
    prSclParam->u4YSrcW 	  = prVpDecInfo->rDecParam.u2Width;
    prSclParam->u4YSrcH 	  = prVpDecInfo->rDecParam.u2Height;
#endif
	prSclParam->u4CSrcBase    = prWebPInfo->rBufLayOut.u4ColorBufC;
    prSclParam->u4CSrcHOffset = prSclParam->u4YSrcHOffset>>1;
    prSclParam->u4CSrcVOffset = prSclParam->u4YSrcVOffset>>1;
    prSclParam->u4CSrcW       = prSclParam->u4YSrcW>>1;
    prSclParam->u4CSrcH       = prSclParam->u4YSrcH>>1;
    prSclParam->u4InMode      =E_RZ_VDO_OUTMD_420;
    prSclParam->u4IsVdoPartialModeEnable=1;
    prSclParam->u4Venc_skip=0;
    prSclParam->u4VdoCbCrSwap=0;
    prSclParam->u4RzTmpBuf=PHYSICAL(prWebPInfo->rBufLayOut.u4RzTempBuf);
    prSclParam->u4ABlend=0x80;
    prSclParam->u4IsFstBl=1;
	prSclParam->u4IsLstBl=0;
	if(prVpDecInfo->rVp8FrmHdr.rVp8DecParam.iLoopFilterLvl>0 && prVpDecInfo->rDecParam.u2Height>=16)
	{
		prSclParam->u4LoopFilterEnable=1;
	}
	else
	{
		prSclParam->u4LoopFilterEnable=0;
	}
    
	if(VDEC_FLGSET(prWebPInfo->prDecInParam->u4NotifyInfo,WEBP_NOTIFY_TYPE_ROWNOTIFY))
	{
       prSclParam->u4IsSameaddr=1;
	}
	else
	{
		prSclParam->u4IsSameaddr=0;
	}

    //special case.
    if(prVpDecInfo->rDecParam.u2Width<=16/* || prVpDecInfo->rDecParam.u2Height<=16*/)
    {
        prSclParam->u4IsVdoPartialModeEnable=0;
        prSclParam->u4IsWebPRzOn=0;
		prSclParam->u4LoopFilterEnable=0;
		prSclParam->u4IsSameaddr=0;
        prSclParam->u4IsFstBl=0;
    }
    else if(prWebPInfo->u1DecMode==VDEC_VP8_DEC_PIC_MODE)
    {
       prSclParam->u4IsWebPRzOn=1;
    }
	else
	{
	   prSclParam->u4IsWebPRzOn=0;
	}
	
	if(prDecInParam->eColorMode==WEBP_COLOR_AYCBCR8888 ||prDecInParam->eColorMode==WEBP_COLOR_ARGB8888)
	{
       //method 1: Y/C --> AYCbCr8888, 1 pass
       prSclParam->u4OutMode     = (UINT32) E_RZ_VDO_OUTMD_444;
       prSclParam->u4IsVdo2Osd   = 1;
       prSclParam->u4IsRsOut     = 1;
       prSclParam->u4YTgBase     = (UINT32) PHYSICAL(prDecInParam->u4DestBuffer);
       prSclParam->u4YTgCM       = (UINT32) E_RZ_OSD_DIR_CM_AYCbCr8888;
       prSclParam->u4YTgBufLen   = prDecInParam->u4Pitch;
       prSclParam->u4CTgBufLen   = prDecInParam->u4Pitch;
       prSclParam->u4CTgBase     = prDecInParam->u4DestBuffer;
       prSclParam->u4CTgW        = prDecInParam->u4Width;
       prSclParam->u4CTgH        = prDecInParam->u4Height;       
	}
	else
	{
       //method 2: Y/C --> RGB, 2 pass
       // 1st pass: VDO scaling
       if(prDecInParam->eColorMode==WEBP_COLOR_YCSEP420)
       {
		   prSclParam->u4OutMode	 = (UINT32) E_RZ_VDO_OUTMD_420;
		   prSclParam->u4YTgCM		 = (UINT32) E_RZ_VDO_OUTMD_420;
		   prSclParam->u4CTgW		 = prDecInParam->u4Width>> 1;
		   prSclParam->u4CTgH		 = prDecInParam->u4Height>>1;
       }
	   else if(prDecInParam->eColorMode==WEBP_COLOR_YCSEP422)
	   {
		   prSclParam->u4OutMode	 = (UINT32) E_RZ_VDO_OUTMD_422;
		   prSclParam->u4YTgCM		 = (UINT32) E_RZ_VDO_OUTMD_422;
		   prSclParam->u4CTgW		 = prDecInParam->u4Width>> 1;
		   prSclParam->u4CTgH		 = prDecInParam->u4Height;
	   }
	   else
	   {
          ASSERT(0);
		  return;
	   }
       prSclParam->u4IsVdo2Osd   = 0;
#ifdef CC_WEBP_EMULATION
       prSclParam->u4IsRsOut     = 0;
#else
       prSclParam->u4IsRsOut	 = 1;
#endif
       prSclParam->u4YTgBase     = PHYSICAL(prDecInParam->u4DestBuffer);
       prSclParam->u4YTgBufLen   = prDecInParam->u4Pitch;
       prSclParam->u4CTgBufLen   = prDecInParam->u4Pitch;
       if(VDEC_FLGSET(prWebPInfo->prDecInParam->u4NotifyInfo,WEBP_NOTIFY_TYPE_ROWNOTIFY))
       {
           prSclParam->u4CTgBase = prDecInParam->u4DestBuffer + (prSclParam->u4YTgBufLen * (prWebPInfo->u4ScalFactor>>1)*16);  
       }
       else
       {
           prSclParam->u4CTgBase = prDecInParam->u4DestBuffer + prDecInParam->u4CbCrOffset;  
       }
	   
	   prSclParam->u4CTgBase=PHYSICAL(prSclParam->u4CTgBase);

	}
	
	prSclParam->u4YTgHOffset  = prDecInParam->u4StartX;
	prSclParam->u4YTgVOffset  = prDecInParam->u4StartY;
	prSclParam->u4YTgW		= prDecInParam->u4Width;
	prSclParam->u4YTgH		= prDecInParam->u4Height;  
    prSclParam->u4SrcSwap     = 0;
	prSclParam->u4OutSwap     = 0;
	
 	prDecOutParam->u4CbCrOffset=prSclParam->u4CTgBase-prSclParam->u4YTgBase;
	prDecOutParam->u4Height=prSclParam->u4YTgH;
	prDecOutParam->u4Width=prSclParam->u4YTgW;
	prDecOutParam->u4StartX=prSclParam->u4YTgHOffset;
	prDecOutParam->u4StartY=prSclParam->u4YTgVOffset;
	
}
UINT32 _VDEC_Vp8PsrWebPRequestData(VDEC_VP8_INFO_T *prVpDecInfo)
{
   WEBP_DEC_INFO_T *prWebPInfo=&prVpDecInfo->rWebPInfo;
   UINT32 u4ReqSize=0, u4RtnVal=0;
   WEBP_DATA_REQ_INFO rReqInfo;
   WEBP_BUF_INFO *prBufInfo;

   prBufInfo=prWebPInfo->rSrcBufInfo+prWebPInfo->u4RingBufIndex;

   if(prWebPInfo->u4RingBufIndex==0)
   {
      if(prBufInfo->u4RequestedSize==0)
      {
         u4ReqSize=VP8_KEYHDR_PREPARSE_LEN;
      }
	  else
	  {
         if(prWebPInfo->fgBufEnough)
         {
             u4ReqSize=prWebPInfo->prDecInParam->u4FileSize-prWebPInfo->prDecInParam->u4FileOffset;
         }
         else
         {
             u4ReqSize=VP8_KEYHDR_PREPARSE_LEN+prVpDecInfo->rVp8FrmHdr.u4rPartitionSize[0]+(MAX_VP8_DATAPARTITION-1)*3;
         }
	  }
      
	  rReqInfo.u4Size=u4ReqSize;
	  rReqInfo.u4WritePosition=prBufInfo->u4WritePos;
	  rReqInfo.u4FileOffset=prBufInfo->u4ReadOffset;
	  u4ReqSize=(UINT32)prWebPInfo->pfnWebpCb(prWebPInfo->u4Tag,(void *)&rReqInfo,WEBP_NOTIFY_TYPE_REQDATA);
      prBufInfo->u4RequestedSize+=u4ReqSize;
	  if(u4ReqSize<rReqInfo.u4Size)
      {
         ASSERT(0);
      }
	  VDEC_RPOS_INC(prBufInfo->u4WritePos,u4ReqSize,prBufInfo->u4Start,prBufInfo->u4End);
	  prBufInfo->u4ReadOffset+=u4ReqSize;

      u4RtnVal = u4ReqSize;
   }
   else
   {
      UINT32 u4EmptySize1=0,u4EmptySize2=0,u4RemaindSize=0,u4NextPtr=0;
      UINT32 u4UpdateBufIndex=prWebPInfo->u4RingBufIndex,u4ReadSize=0,u4NewWritePos=0;
      
      if(VDEC_FLGSET(prVpDecInfo->rDecParam.u4Flag,VP8_DEC_FLG_STARTED))
      {
         ASSERT((prBufInfo->u4UsedBitCount/8)<=prVpDecInfo->rVp8FrmHdr.u4rPartitionSize[u4UpdateBufIndex]);
         prBufInfo->u4ReadPos=prBufInfo->u4Start+(prBufInfo->u4UsedBitCount/8);
         while(prBufInfo->u4ReadPos>prBufInfo->u4End)
         {
            prBufInfo->u4ReadPos-=(prBufInfo->u4End-prBufInfo->u4Start);
         }
      }

      if(prWebPInfo->fgPartitionRing)
      {
          //_VDEC_Vp8PsrWebpBufInfo(prBufInfo,u4UpdateBufIndex,prWebPInfo->u4RingBufIndex,prVpDecInfo->rVp8FrmHdr.u4rPartitionSize[u4UpdateBufIndex],"BeforeGetData");
          u4RemaindSize=prVpDecInfo->rVp8FrmHdr.u4rPartitionSize[u4UpdateBufIndex]-prBufInfo->u4RequestedSize;
      }
      else
      {
          u4RemaindSize=prWebPInfo->prDecInParam->u4FileSize-prBufInfo->u4ReadOffset;
      }
      
	  if(prBufInfo->u4ReadPos<=prBufInfo->u4WritePos)
	  {
        u4EmptySize1=prBufInfo->u4End-prBufInfo->u4WritePos;
        u4EmptySize2=prBufInfo->u4ReadPos-prBufInfo->u4Start;
	  }
	  else
	  {
        u4EmptySize1=prBufInfo->u4ReadPos-prBufInfo->u4WritePos;
	  }

	  if(u4RemaindSize>0 && ((u4EmptySize1+u4EmptySize2>=u4RemaindSize)
	  	 ||(u4EmptySize1+u4EmptySize2)>=((prBufInfo->u4End-prBufInfo->u4Start)>>1)))
	  {
	     u4ReqSize=MIN(u4EmptySize1,u4RemaindSize);
         
		 if(u4ReqSize)
		 {
             u4NextPtr=prBufInfo->u4WritePos;
			 VDEC_RPOS_INC(u4NextPtr,u4ReqSize,prBufInfo->u4Start,prBufInfo->u4End);
             if(u4NextPtr==prBufInfo->u4ReadPos)
             {
               u4ReqSize--;
             }
			 rReqInfo.u4FileOffset=prBufInfo->u4ReadOffset;
			 rReqInfo.u4Size=u4ReqSize;
			 rReqInfo.u4WritePosition=prBufInfo->u4WritePos;  
			 u4ReqSize=(UINT32)prWebPInfo->pfnWebpCb(prWebPInfo->u4Tag,(void *)&rReqInfo,WEBP_NOTIFY_TYPE_REQDATA);
			 VDEC_RPOS_INC(prBufInfo->u4WritePos,u4ReqSize,prBufInfo->u4Start,prBufInfo->u4End);
			 prBufInfo->u4ReadOffset+=u4ReqSize;
			 prBufInfo->u4RequestedSize+=u4ReqSize;
			 u4RemaindSize-=u4ReqSize;
             u4ReadSize=u4ReqSize;
		 }
		 
	     u4ReqSize=MIN(u4EmptySize2,u4RemaindSize);
		 if(u4ReqSize)
		 {
             u4NextPtr=prBufInfo->u4WritePos;
			 VDEC_RPOS_INC(u4NextPtr,u4ReqSize,prBufInfo->u4Start,prBufInfo->u4End);
             if(u4NextPtr==prBufInfo->u4ReadPos)
             {
               u4ReqSize--;
             }
             
			 rReqInfo.u4FileOffset=prBufInfo->u4ReadOffset;
			 rReqInfo.u4Size=u4ReqSize;
			 rReqInfo.u4WritePosition=prBufInfo->u4WritePos;  
			 u4ReqSize=(UINT32)prWebPInfo->pfnWebpCb(prWebPInfo->u4Tag,(void *)&rReqInfo,WEBP_NOTIFY_TYPE_REQDATA);
			 VDEC_RPOS_INC(prBufInfo->u4WritePos,u4ReqSize,prBufInfo->u4Start,prBufInfo->u4End);
			 prBufInfo->u4ReadOffset+=u4ReqSize;
			 prBufInfo->u4RequestedSize+=u4ReqSize;
			 u4RemaindSize-=u4ReqSize;
             u4ReadSize+=u4ReqSize;
		 }
	  }

      if(u4RemaindSize==0)
      {   
         u4NewWritePos=prBufInfo->u4ReadPos-(prBufInfo->u4ReadPos&WPTR_ALIGN_MARK)-WPTR_ALIGN;
      
         if(u4NewWritePos<prBufInfo->u4Start)
         {
            u4NewWritePos=prBufInfo->u4End-WPTR_ALIGN;
         }
      }
      else if(u4ReadSize>0)
      {
         u4NewWritePos=prBufInfo->u4WritePos-(prBufInfo->u4WritePos&WPTR_ALIGN_MARK)-WPTR_ALIGN;
         if(u4NewWritePos<prBufInfo->u4Start)
         {
            u4NewWritePos=prBufInfo->u4End-WPTR_ALIGN;
         }
      }
      else
      {
         u4NewWritePos=prBufInfo->u4RealWritePos;
      }
      
      if((prWebPInfo->fgPartitionRing)&&VDEC_FLGSET(prVpDecInfo->rDecParam.u4Flag,VP8_DEC_FLG_STARTED))
      {
         if((!VDEC_FLGSET(prVpDecInfo->rDecParam.u4Flag,VP8_DEC_FLG_PARTCHG))&&VDEC_FLGSET(prVpDecInfo->rDecParam.u4Flag,VP8_DEC_FLG_DATAEMPTY)&&(prBufInfo->u4RealWritePos!=u4NewWritePos))
         {
            _Vdec_VP8SetWritePtr(u4NewWritePos);
         }
         //_VDEC_Vp8PsrWebpBufInfo(prBufInfo,u4UpdateBufIndex,prWebPInfo->u4RingBufIndex,prVpDecInfo->rVp8FrmHdr.u4rPartitionSize[u4UpdateBufIndex],"AfterGetData");
      }
      
      prBufInfo->u4RealWritePos=u4NewWritePos;
      u4RtnVal = u4ReadSize;
   }
    
   return u4RtnVal;
}

static BOOL _VDEC_Vp8PsrWebPMemCfg(VDEC_VP8_INFO_T *prVpDecInfo)
{
   WEBP_DEC_INFO_T *prWebPInfo=&prVpDecInfo->rWebPInfo;
   UINT32 u4WorkPtr=prWebPInfo->u4BitStreamStartPos;
   UINT32 u4PartitionNum=prVpDecInfo->rVp8FrmHdr.rVp8DecParam.uDataPartitionNum;

   if(prWebPInfo->u4BufStartAddr+prWebPInfo->u4BufSize<=u4WorkPtr)
   {
	   LOG(2,"WebP Dec Buffer not enough 2\n");
	   return FALSE;
   }
   else if((prWebPInfo->u4BufStartAddr+prWebPInfo->u4BufSize-u4WorkPtr)
   	>=(prWebPInfo->prDecInParam->u4FileSize-prWebPInfo->prDecInParam->u4FileOffset-prWebPInfo->u4HeaderLen))
   {
	   WEBP_BUF_INFO *prBufInfo=&prWebPInfo->rSrcBufInfo[1];
       prWebPInfo->fgPartitionRing=FALSE;
	   prBufInfo->u4Start=u4WorkPtr;
	   prBufInfo->u4End=prWebPInfo->u4BufStartAddr+prWebPInfo->u4BufSize;
	   prBufInfo->u4WritePos=u4WorkPtr;
	   prBufInfo->u4ReadPos=u4WorkPtr;
	   prBufInfo->u4UsedBitCount=0;
	   prBufInfo->u4RequestedSize=0;
	   prBufInfo->u4ReadOffset=prWebPInfo->prDecInParam->u4FileOffset+prWebPInfo->u4HeaderLen;
	   //prVpDecInfo->rVp8FrmHdr.u4rPartitionSize[1]=prWebPInfo->prDecInParam->u4FileSize-prBufInfo->u4ReadOffset;
	   return TRUE;
   }
   else if((u4WorkPtr+u4PartitionNum*VP8_PARTITION_BUF_MINSIZE)>(prWebPInfo->u4BufStartAddr+prWebPInfo->u4BufSize))
   {
	  LOG(2,"WebP Dec Buffer not enough 3\n");
	  return FALSE;
   }
   else
   {
      UINT32 u4RemainSize=prWebPInfo->u4BufStartAddr+prWebPInfo->u4BufSize-u4WorkPtr;
	  UINT32 u4PartionBufLen=((u4RemainSize/VP8_PARTITION_BUF_MINSIZE)/u4PartitionNum)*VP8_PARTITION_BUF_MINSIZE;
      UINT32 u4FileReadOffset=prWebPInfo->u4FileOffset+prWebPInfo->u4HeaderLen;
	  WEBP_BUF_INFO *prBufInfo=prWebPInfo->rSrcBufInfo+1;
	  UINT32 u4Index;
	  for(u4Index=1;u4Index<u4PartitionNum+1;u4Index++)
	  {
         prBufInfo->u4Start=u4WorkPtr;
		 prBufInfo->u4End=u4WorkPtr+u4PartionBufLen;
		 prBufInfo->u4ReadPos=u4WorkPtr;
		 prBufInfo->u4WritePos=u4WorkPtr;
		 prBufInfo->u4UsedBitCount=0;
		 prBufInfo->u4RequestedSize=0;
		 prBufInfo->u4ReadOffset=u4FileReadOffset;
		 u4WorkPtr+=u4PartionBufLen;
		 if(u4Index<MAX_VP8_DATAPARTITION)
		 {
	       u4FileReadOffset+=prVpDecInfo->rVp8FrmHdr.u4rPartitionSize[u4Index];
		 }
		 prBufInfo++;
	  }
	  prWebPInfo->fgPartitionRing=TRUE;
   }
   return TRUE;
}


static VOID _VDEC_Vp8PsrWebpMemoryInfo(VDEC_VP8_INFO_T *prVpDecInfo)
{
   WEBP_DEC_INFO_T *prWebPInfo=&prVpDecInfo->rWebPInfo;
 
   LOG(5,"Width=%d,Height=%d,FirstPartLen=0x%x\n",prVpDecInfo->rDecParam.u2Width,prVpDecInfo->rDecParam.u2Height, \
    prVpDecInfo->rVp8FrmHdr.u4rPartitionSize[0]);

   LOG(5,"WapperOff=%d,PartitionRing=%d,BufEnough=%d,DecMode=%d\n",
      prWebPInfo->fgWrapperOff,prWebPInfo->fgPartitionRing,prWebPInfo->fgBufEnough,prWebPInfo->u1DecMode);
   LOG(5,"ColorY=0x%x,ColorC=0x%x\n",prWebPInfo->rBufLayOut.u4ColorBufY,prWebPInfo->rBufLayOut.u4ColorBufC);

   if(prWebPInfo->fgWrapperOff)
   {
      LOG(5,"WapperOff Vld=0x%x,SegId=0x%x,PPY=0x%x,PPC=0x%x\n",prWebPInfo->rBufLayOut.u4VLDWrapper, \
        prWebPInfo->rBufLayOut.u4SegIdWapper,prWebPInfo->rBufLayOut.u4PPWrapperY,prWebPInfo->rBufLayOut.u4PPWrapperC);
   }

   LOG(5,"ResizeWrokBuf=0x%x\n",prWebPInfo->rBufLayOut.u4RzTempBuf);

   if(prWebPInfo->fgPartitionRing)
   {
      UINT32 u4Id=0;
      WEBP_BUF_INFO *prBufInfo=NULL;
      LOG(5,"Partition ring buffer on\n");
      for(u4Id=1;u4Id<prVpDecInfo->rVp8FrmHdr.rVp8DecParam.uDataPartitionNum+1;u4Id++)
      { 
          if(u4Id<MAX_VP8_DATAPARTITION)
          {
			  prBufInfo=prWebPInfo->rSrcBufInfo+u4Id;
			  LOG(5,"Buf[%d] Start:0x%x,Eend:0x%x,Size:0x%x,Part:0x%x\n",u4Id, \
			  prBufInfo->u4Start,prBufInfo->u4End,prBufInfo->u4End-prBufInfo->u4Start, \
			  prVpDecInfo->rVp8FrmHdr.u4rPartitionSize[u4Id]);
          }
      }
	  UNUSED(prBufInfo);
	  UNUSED(u4Id);
   }
   else
   {
       WEBP_BUF_INFO *prBufInfo=prWebPInfo->rSrcBufInfo+prWebPInfo->u4RingBufIndex;
       LOG(5,"Buf[%d] Start:0x%x,Eend:0x%x,Size:0x%x,Pos:0x%x\n", \
                 prWebPInfo->u4RingBufIndex,prBufInfo->u4Start,prBufInfo->u4End,prBufInfo->u4End-prBufInfo->u4Start,
                 prWebPInfo->prDecInParam->u4FileOffset+prWebPInfo->u4HeaderLen);
       LOG(5,"Buf[%d] ReadPos:0x%x, WritePos:0x%x, RealWritePos:0x%x\n",prWebPInfo->u4RingBufIndex,prBufInfo->u4ReadPos, \
        prBufInfo->u4WritePos,prBufInfo->u4RealWritePos);
	   UNUSED(prBufInfo);
   }

   UNUSED(prWebPInfo);
   return;
}

static BOOL _VDEC_Vp8PsrWebPHeaderPreParse(VDEC_VP8_INFO_T *prVpDecInfo)
{
   WEBP_DEC_INFO_T *prWebPInfo=&prVpDecInfo->rWebPInfo;
   WEBP_BUF_INFO *prBufInfo=&prWebPInfo->rSrcBufInfo[0];
   WEBP_DECIN_PARAM_T *prDecInParam=prWebPInfo->prDecInParam;
   WEBP_DEC_BUFINFO_T *prBufLayout=&prWebPInfo->rBufLayOut;
   UINT32 u4MBWidth=0,u4MBHeight=0;
   UINT32 u4WorkPtr=prWebPInfo->u4BufStartAddr;
   UCHAR *puData=NULL;
   prBufInfo->u4Start=prWebPInfo->u4BufStartAddr;
   prBufInfo->u4End=prWebPInfo->u4BufStartAddr+prWebPInfo->u4BufSize;
   prBufInfo->u4ReadPos=prBufInfo->u4Start;
   prBufInfo->u4WritePos=prBufInfo->u4Start;
   prBufInfo->u4ReadOffset=prWebPInfo->u4FileOffset;
   prBufInfo->u4RequestedSize=0;
   prBufInfo->u4UsedBitCount=0;
   prWebPInfo->u4RingBufIndex=0;
   _VDEC_Vp8PsrWebPRequestData(prVpDecInfo);
   puData=(UCHAR *)VIRTUAL(prBufInfo->u4ReadPos);

   if((puData[0]&0x1)!=VP68_KEY_FRAME)
   {
      LOG(0,"Vp8 PreParse not key frame \n");
	  return FALSE;
   }

   prVpDecInfo->rVp8FrmHdr.u4rPartitionSize[0]=(puData[0]|(puData[1]<<8)|(puData[2]<<16))>>5;
   if(puData[3]!=0x9d || puData[4]!=0x01 || puData[5]!=0x2a)
   {
     LOG(0,"Vp8 PreParse identify tag not right \n");
	 return FALSE;
   }
   
   prVpDecInfo->rDecParam.u2Width=(puData[6]|(puData[7]<<8))&0x3fff;
   prVpDecInfo->rDecParam.u2Height=(puData[8]|(puData[9]<<8))&0x3fff;
   if(prDecInParam->u4Width==0 || prDecInParam->u4Height==0)
   {
      prDecInParam->u4Width=prVpDecInfo->rDecParam.u2Width;
	  prDecInParam->u4Height=prVpDecInfo->rDecParam.u2Height;
   }
   if(prVpDecInfo->rDecParam.u2Width>VDEC_WEBP_MAX_WIDTH || prVpDecInfo->rDecParam.u2Height>VDEC_WEBP_MAX_HEIGHT)
   {
	   LOG(0,"Vp8 WxH=%dx%d is no support\n",prVpDecInfo->rDecParam.u2Width,prVpDecInfo->rDecParam.u2Height);
       return FALSE;
   }
   
   if(prVpDecInfo->rDecParam.u2Width>VDEC_VP8_WRAPPER_LIMIT_W || prVpDecInfo->rDecParam.u2Height>VDEC_VP8_WRAPPER_LIMIT_H)
   {
	  prWebPInfo->fgWrapperOff=TRUE;
   }
#ifdef VP8_DISABLE_WRAPPER
    prWebPInfo->fgWrapperOff=TRUE;
#endif

   u4MBWidth= (prVpDecInfo->rDecParam.u2Width+15)/16;
   u4MBHeight=(prVpDecInfo->rDecParam.u2Height+15)/16;
   VDEC_VALUE_ALIGN(u4WorkPtr,512);
   prBufLayout->u4ColorBufY=u4WorkPtr;
   // 16 pix width
   if(u4MBWidth==1)
   {
       u4WorkPtr+=(u4MBHeight*512);
   }
   else
   {
       u4WorkPtr+=(u4MBWidth*512);
   }
   
   VDEC_VALUE_ALIGN(u4WorkPtr,512);
   prBufLayout->u4ColorBufC=u4WorkPtr;
   if(u4MBWidth==1)
   {
       u4WorkPtr+=(u4MBHeight*256);
   }
   else
   {
       u4WorkPtr+=(u4MBWidth*256);
   }

   if(prWebPInfo->fgWrapperOff)
   {
	  VDEC_VALUE_ALIGN(u4WorkPtr,64);
	  prBufLayout->u4VLDWrapper=u4WorkPtr;
	  u4WorkPtr+=(u4MBWidth*64);
	  VDEC_VALUE_ALIGN(u4WorkPtr,64);
	  prBufLayout->u4SegIdWapper=u4WorkPtr;
	  u4WorkPtr+=u4MBHeight*64;
	  VDEC_VALUE_ALIGN(u4WorkPtr,512);
	  prBufLayout->u4PPWrapperY=u4WorkPtr;
	  u4WorkPtr+=(u4MBWidth*64);
	  VDEC_VALUE_ALIGN(u4WorkPtr,512);
	  prBufLayout->u4PPWrapperC=u4WorkPtr;
	  u4WorkPtr+=(u4MBWidth*64);
	  VDEC_VALUE_ALIGN(u4WorkPtr,64);
   }
   
   VDEC_VALUE_ALIGN(u4WorkPtr,1024);
   prBufLayout->u4RzTempBuf=u4WorkPtr;
   u4WorkPtr+=IMGRZ_WORKING_BUFSIZE;
   VDEC_VALUE_ALIGN(u4WorkPtr,128);
   if(VDEC_FLGSET(prWebPInfo->prDecInParam->u4NotifyInfo,WEBP_NOTIFY_TYPE_ROWNOTIFY))
   {
      UINT32 u4Factor=0;
      if(prWebPInfo->prDecInParam->eColorMode==WEBP_COLOR_AYCBCR8888 || prWebPInfo->prDecInParam->eColorMode==WEBP_COLOR_ARGB8888)
      {
         u4Factor=1;
      }
      else
      {
         u4Factor=2;
      }
      
      if(prVpDecInfo->rDecParam.u2Height<prWebPInfo->prDecInParam->u4Height)
      {
         u4Factor=u4Factor*((prWebPInfo->prDecInParam->u4Height+prVpDecInfo->rDecParam.u2Height-1)/prVpDecInfo->rDecParam.u2Height);
      }
      prWebPInfo->u4ScalFactor=u4Factor;
	  prWebPInfo->prDecInParam->u4DestBuffer=u4WorkPtr;
	  u4WorkPtr+=prWebPInfo->prDecInParam->u4Pitch*u4Factor*16;
	  prWebPInfo->prDecOutParam->eDecMode=WEBP_DEC_MODE_MCUROW;
      u4WorkPtr+=1024;
	  VDEC_VALUE_ALIGN(u4WorkPtr,64);
   }
   
   // ic problem, when width is less or equal 16 :
   // mcu row mode, pic finish interrupt is send after penult mcu row done.
   // pic mode, imgrz can't get command to resize last mcu row and time out.
   // solution: do decode use pic mode, and then do resize.
   if(prVpDecInfo->rDecParam.u2Width<=16/*|| prVpDecInfo->rDecParam.u2Height<16*/)
   {
	  prWebPInfo->prDecOutParam->eDecMode=WEBP_DEC_MODE_PIC;
   }
   VDEC_VALUE_ALIGN(u4WorkPtr,1024);
   
   if(u4WorkPtr>=prWebPInfo->u4BufStartAddr+prWebPInfo->u4BufSize)
   {
      LOG(2,"WebP Dec Buffer not enough1 (%d,%d)\n",prWebPInfo->u4BufSize,u4WorkPtr-prWebPInfo->u4BufStartAddr);
	  return FALSE;
   }

   if((prWebPInfo->u4BufStartAddr+prWebPInfo->u4BufSize-u4WorkPtr) \
   	<(VP8_KEYHDR_PREPARSE_LEN+prVpDecInfo->rVp8FrmHdr.u4rPartitionSize[0]+(MAX_VP8_DATAPARTITION-1)*3))
   	{
       UINT32 u4WorkingSz = VP8_KEYHDR_PREPARSE_LEN+prVpDecInfo->rVp8FrmHdr.u4rPartitionSize[0]+  \
	   	(MAX_VP8_DATAPARTITION-1)*3 + (u4WorkPtr-prWebPInfo->u4BufStartAddr);
	   LOG(2,"Vp8 PreParse Working buffer not enough to parse header (%d < %d)\n",prWebPInfo->u4BufSize, u4WorkingSz);
	   return FALSE;
   	}

   if((prWebPInfo->u4BufStartAddr+prWebPInfo->u4BufSize-u4WorkPtr)>(prWebPInfo->prDecInParam->u4FileSize-prWebPInfo->prDecInParam->u4FileOffset))
   {
	   prWebPInfo->fgBufEnough=TRUE;
   }
   
   if(!prWebPInfo->fgBufEnough)
   {
       prBufInfo->u4Start=prWebPInfo->u4BufStartAddr+prWebPInfo->u4BufSize-prVpDecInfo->rVp8FrmHdr.u4rPartitionSize[0]-(MAX_VP8_DATAPARTITION-1)*3-VP8_KEYHDR_PREPARSE_LEN;
       prBufInfo->u4Start-=128;
       VDEC_VALUE_ALIGN(prBufInfo->u4Start,64);
   }
   else
   {
       prBufInfo->u4Start=u4WorkPtr;
   }
   prWebPInfo->u4BitStreamStartPos=u4WorkPtr;
   
   prBufInfo->u4End=prWebPInfo->u4BufStartAddr+prWebPInfo->u4BufSize+1024;
   prBufInfo->u4ReadPos=prBufInfo->u4Start;
   prBufInfo->u4WritePos=prBufInfo->u4Start;
   prBufInfo->u4ReadOffset=prWebPInfo->u4FileOffset;
   prBufInfo->u4UsedBitCount=0;
   _VDEC_Vp8PsrWebPRequestData(prVpDecInfo);
   return TRUE;
}

static BOOL _VDEC_Vp8PsrWebpDecodeStart(VDEC_VP8_INFO_T *prVpDecInfo,RZ_VDO_SCL_PARAM_SET_T *prSclParam)
{
    WEBP_DEC_INFO_T *prWebPInfo=&prVpDecInfo->rWebPInfo;
    UINT32 u4WaiteMode=VDEC_VP8_WAITEMODE_ISR;
	WEBP_DECIN_PARAM_T *prDecInParam=prWebPInfo->prDecInParam;
	
    prWebPInfo->u4RingBufIndex=1;

	if(prWebPInfo->fgPartitionRing)
	{
       u4WaiteMode=VDEC_VP8_WAITEMODE_POLLING;
	}
	
   	if(prWebPInfo->u1DecMode==VDEC_VP8_DEC_PIC_MODE)
	{   
		IMGRZ_Lock();
		IMGRZ_ReInit();
		IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
        if(prVpDecInfo->rDecParam.u2Width>16 /*&& prVpDecInfo->rDecParam.u2Height>=16*/)
        {
            IMGRZ_Scale(prSclParam);
            IMGRZ_Flush();
        }
        
        _Vdec_VP8HwWebPDecode(prVpDecInfo);
		if(_Vdec_VP8HwWebPWaitDecodeDone(prVpDecInfo,u4WaiteMode)==FALSE)
		{
			IMGRZ_Unlock();
			return FALSE;			   
		}

        if(prVpDecInfo->rDecParam.u2Width<=16 /*||  prVpDecInfo->rDecParam.u2Height<16*/)
        {
            IMGRZ_Scale(prSclParam);
            IMGRZ_Flush();
        }
        IMGRZ_Wait();
		IMGRZ_Unlock();
        if(!VDEC_FLGSET(prVpDecInfo->rDecParam.u4Flag,VP8_DEC_FLG_FINISHED))
		{
		   LOG(0,"Webp decode error4 \n");
           return FALSE;
		}
	}
	else
	{
	    UINT32 u4TotalRow=0,u4ResizeRow=0,u4Index=0;
		u4TotalRow=(prVpDecInfo->rDecParam.u2Height+15)/16;
        prWebPInfo->u4MbRowIndex=0;
        
		// first mcu row
		_Vdec_VP8HwWebPDecode(prVpDecInfo);
		_Vdec_VP8HwWebPMcuRowTrigger(FALSE);
		if(_Vdec_VP8HwWebPWaitDecodeDone(prVpDecInfo,u4WaiteMode)==FALSE)
		{
			return FALSE;			   
		}
		
#ifdef CC_WEBP_EMULATION
        _VDEC_Vp8CopyFrameBuffer(prVpDecInfo, 0);
#endif
		for(u4Index=0;u4Index<u4TotalRow-1;u4Index++)
		{
           if(prWebPInfo->fgPartitionRing)
           {
			   _VDEC_Vp8PsrWebPRequestData(prVpDecInfo);
           }
           
		   if(u4Index==0)
		   {
			  prSclParam->u4IsFstBl=1;
			  prSclParam->u4IsLstBl=0;
		   }
		   else
		   {
			  prSclParam->u4IsFstBl=0;
			  prSclParam->u4IsLstBl=0;
		   }
		   IMGRZ_Lock(); // for previous mcu row
		   if(u4Index==0)
		   {
			 IMGRZ_ReInit();
		   }
		   IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
		   IMGRZ_Scale(prSclParam);
		   IMGRZ_Flush();

           if(u4Index==u4TotalRow-2)
           {
               IMGRZ_Wait();
               _Vdec_VP8HwWebPMcuRowTrigger(TRUE);
           }
           else
           {
               _Vdec_VP8HwWebPMcuRowTrigger(TRUE);
               IMGRZ_Wait();
           }
		   
		   if(_Vdec_VP8HwWebPWaitDecodeDone(prVpDecInfo,u4WaiteMode)==FALSE)
		   {
			   IMGRZ_Unlock();
			   return FALSE;			  
		   }
           
		   u4ResizeRow=_IMGRZ_GetScaleLine();
		   IMGRZ_Unlock();
#ifdef CC_WEBP_EMULATION
           /*
                 if(prWebPInfo->prDecInParam->u4DecOutY && prWebPInfo->prDecInParam->u4DecOutC)
                 {
                       if((u4Index&0x1)==0)
                       {
                            UINT32 u4Pitch=((prVpDecInfo->rDecParam.u2Width+15)/16)*16;
                            HalFlushInvalidateDCache();
                            x_memcpy((void *)(prWebPInfo->prDecInParam->u4DecOutY+u4Index*16*u4Pitch),(void *)prVpDecInfo->rVp8FrmHdr.u4CurYAddr,u4Pitch*32);
                            x_memcpy((void *)(prWebPInfo->prDecInParam->u4DecOutC+u4Index*8*u4Pitch),(void *)prVpDecInfo->rVp8FrmHdr.u4CurCAddr,u4Pitch*16);
                       }
                  }
                  */
           _VDEC_Vp8CopyFrameBuffer(prVpDecInfo, u4Index + 1);
#endif
           prWebPInfo->u4MbRowIndex=u4Index+1;
           LOG(8,"Row %d Done \n",prWebPInfo->u4MbRowIndex);

           if(VDEC_FLGSET(prWebPInfo->prDecInParam->u4NotifyInfo,WEBP_NOTIFY_TYPE_ROWNOTIFY))
           {
              WEBP_RZROW_INFO rRowInfo;
              rRowInfo.u4YAddr=prSclParam->u4YTgBase;
              rRowInfo.u4CAddr=prSclParam->u4CTgBase;
              rRowInfo.u4Height=u4ResizeRow;
              rRowInfo.u4Pitch=prSclParam->u4YTgBufLen;
              prWebPInfo->pfnWebpCb(prWebPInfo->u4Tag,(void *)&rRowInfo,WEBP_NOTIFY_TYPE_ROWNOTIFY);
           }
        }

		IMGRZ_Lock(); // for last mcu row
		
        if(u4TotalRow==1)
        {
            prSclParam->u4IsFstBl=1;
            prSclParam->u4IsLstBl=1;
            IMGRZ_ReInit();
        }
        else
        {
            prSclParam->u4IsFstBl=0;
            prSclParam->u4IsLstBl=1;
        }

		IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
		IMGRZ_Scale(prSclParam);
		IMGRZ_Flush();
		IMGRZ_Wait();
		
		if(!VDEC_FLGSET(prVpDecInfo->rDecParam.u4Flag,VP8_DEC_FLG_FINISHED))
		{
           IMGRZ_Unlock();
		   LOG(0,"Webp decode error4 \n");
           return FALSE;
		}
        IMGRZ_Unlock();

#if 0//def CC_WEBP_EMULATION
       if(prWebPInfo->prDecInParam->u4DecOutY && prWebPInfo->prDecInParam->u4DecOutC)
       {
           if(u4TotalRow==0x1)
           {
              UINT32 u4Pitch=((prVpDecInfo->rDecParam.u2Width+15)/16)*16;
              HalInvalidateDCacheMultipleLine(prVpDecInfo->rVp8FrmHdr.u4CurYAddr,u4Pitch*32);
              HalInvalidateDCacheMultipleLine(prVpDecInfo->rVp8FrmHdr.u4CurCAddr,u4Pitch*16);
              //HalFlushInvalidateDCache();
              x_memcpy((void *)(prWebPInfo->prDecInParam->u4DecOutY+u4Index*16*u4Pitch),(void *)prVpDecInfo->rVp8FrmHdr.u4CurYAddr,u4Pitch*32);
              x_memcpy((void *)(prWebPInfo->prDecInParam->u4DecOutC+u4Index*8*u4Pitch),(void *)prVpDecInfo->rVp8FrmHdr.u4CurCAddr,u4Pitch*16);
           }
       }
#endif

	}

	if(prDecInParam->eColorMode==WEBP_COLOR_ARGB8888)
	{
       IMGRZ_AYUV_ARGB_Conv(prSclParam->u4YTgBase,prSclParam->u4YTgBufLen,prSclParam->u4YTgW,prSclParam->u4YTgH,
	   	 prSclParam->u4YTgHOffset,prSclParam->u4YTgVOffset,1);
	}
	prWebPInfo->u4MbRowIndex=(prVpDecInfo->rDecParam.u2Height+15)/16;
#ifdef WEBP_ENABLE_CRC
    prVpDecInfo->rDecParam.u4RzCrc=IO_READ32(0xf0020000,0xe4); // resize output crc.
    //Printf("Rz Reg:0xE4=0x%x,0x1E4=0x%x,0x1F0=0x%x\n",IO_READ32(0xf0020000,0xe4),IO_READ32(0xf0020000,0x1e4),IO_READ32(0xf0020000,0x1f0));
#endif
	return TRUE;
}


BOOL _VDEC_Vp8PsrWebPDecode(VDEC_VP8_INFO_T *prVpDecInfo,UINT32 u4PicMode)
{ 
	WEBP_DEC_INFO_T *prWebPInfo=&prVpDecInfo->rWebPInfo;
	VDEC_INFO_VP8_FRM_HDR_T *prFrmHdr=&prVpDecInfo->rVp8FrmHdr;
 	RZ_VDO_SCL_PARAM_SET_T rSclParam;
    BOOL fgRetValue;
	// request first 10 byte for pre parse, and request data of the header partition.
     prWebPInfo->u4RingBufIndex=0;
#ifdef VP8_TIME_PROFILE
	x_memset(&prVpDecInfo->rTimeProfile,0,sizeof(VDEC_VP8_TIMEPROFILE));
	HAL_GetTime(&prVpDecInfo->rTimeProfile.rTimeS);
#endif

    prWebPInfo->prDecOutParam->eDecMode = (WEBP_DEC_MODE)u4PicMode;
	if(_VDEC_Vp8PsrWebPHeaderPreParse(prVpDecInfo)==FALSE)
	{  
	   LOG(0,"Header Preparse Faile\n");
       return FALSE;
	}
	
    if( prVpDecInfo->rDecParam.u2Width<WEBP_MIN_HW
        ||prVpDecInfo->rDecParam.u2Height<WEBP_MIN_HW
        || prVpDecInfo->rDecParam.u2Height>WEBP_MAX_HW
        ||prVpDecInfo->rDecParam.u2Width>WEBP_MAX_HW)
    {
		LOG(0,"File resolution is out of range:(H=0%x,V=0x%x)\n", \
            prVpDecInfo->rDecParam.u2Width,prVpDecInfo->rDecParam.u2Height);
        return FALSE;
    }

	_Vdec_VP8IsrInit(prVpDecInfo);
	if(_Vdec_VP8HwWebPParseHeader(prVpDecInfo)==FALSE)
	{
		LOG(0,"Header Parse Faile\n");
		_Vdec_VP8HwPowerOff();
		return FALSE;
	}
    if(!prWebPInfo->fgBufEnough)
    {
        WEBP_BUF_INFO *prBufInfo=&prWebPInfo->rSrcBufInfo[0];
        prWebPInfo->u4BufSize-=(prBufInfo->u4End-prBufInfo->u4Start-(prBufInfo->u4UsedBitCount/8)-1);
        prWebPInfo->u4BufSize-=1024;
		prWebPInfo->u4BufSize-= prWebPInfo->u4BufSize&127;
        _VDEC_Vp8PsrWebpBufInfo(prBufInfo,0,0,prWebPInfo->u4HeaderLen,"HeaderBuf");
	    if(_VDEC_Vp8PsrWebPMemCfg(prVpDecInfo)==FALSE)
	    {
		   _Vdec_VP8HwPowerOff();
		   LOG(0,"MemCfg Faile\n");
		   return FALSE;
	     }
    }
	
	if(prWebPInfo->prDecOutParam->eDecMode!=WEBP_DEC_MODE_MCUROW)
    {
	   prWebPInfo->u1DecMode=VDEC_VP8_DEC_PIC_MODE;
	   prWebPInfo->prDecOutParam->eDecMode=WEBP_DEC_MODE_PIC;
    }
	else
	{
       prWebPInfo->u1DecMode=VDEC_VP8_DEC_MCUROW_MODE;
	}
	
    if(prWebPInfo->fgPartitionRing 
       &&(prVpDecInfo->rDecParam.u2Width<VP8WEBP_RINGBUF_MINWIDTH)
       &&(prFrmHdr->rVp8DecParam.uDataPartitionNum>1))
    {
        LOG(0,"Use multi ring buffer, but width is %d\n",prVpDecInfo->rDecParam.u2Width);
		_Vdec_VP8HwPowerOff();
        return FALSE;
    }
    
	_VDEC_Vp8PsrWebPResizeCfg(&rSclParam,prVpDecInfo);
	if((!prWebPInfo->fgPartitionRing) && !prWebPInfo->fgBufEnough)
	{
        prWebPInfo->u4RingBufIndex=1;
        _VDEC_Vp8PsrWebPRequestData(prVpDecInfo);
        _Vdec_VP8HwWebPReset(&prWebPInfo->rSrcBufInfo[1],FALSE);
	}
	else if(prWebPInfo->fgPartitionRing)
	{
       UINT32 u4Index=0;
       for(u4Index=1;u4Index<prFrmHdr->rVp8DecParam.uDataPartitionNum+1;u4Index++)
       {
		  prWebPInfo->u4RingBufIndex=u4Index;
          _VDEC_Vp8PsrWebPRequestData(prVpDecInfo);
       }
       prWebPInfo->u4RingBufIndex=1;
       _Vdec_VP8HwWebPReset(&prWebPInfo->rSrcBufInfo[1],FALSE);
	}
	
    _VDEC_Vp8PsrWebpMemoryInfo(prVpDecInfo);
	VDEC_SETFLG(prVpDecInfo->rDecParam.u4Flag,VP8_DEC_FLG_STARTED);

	fgRetValue=_VDEC_Vp8PsrWebpDecodeStart(prVpDecInfo,&rSclParam);
	
	if(prWebPInfo->fgWrapperOff)
	{
		_Vdec_VP8HwResetWapper();
	}

#ifdef VP8_TIME_PROFILE
	prVpDecInfo->rTimeProfile.u4CurDramCycle=IO_REG32(DRAM_BASE, 0x190);
	prVpDecInfo->rTimeProfile.u4CurDecCycle=IO_REG32(MC0_BASE, 0x778);
	HAL_GetTime(&prVpDecInfo->rTimeProfile.rTimeE);
	HAL_GetDeltaTime(&prVpDecInfo->rTimeProfile.rTimeDif, \
		&prVpDecInfo->rTimeProfile.rTimeS,&prVpDecInfo->rTimeProfile.rTimeE);
	prVpDecInfo->rTimeProfile.u4DecMs=prVpDecInfo->rTimeProfile.rTimeDif.u4Micros;
	_VDEC_Vp8TimeProfileInfo(prVpDecInfo->eDecType,prVpDecInfo->rDecParam.fgKeyFrame,&prVpDecInfo->rTimeProfile);
#endif
	if(fgRetValue==FALSE)
    {
       Printf("Decode Faile\n");
	   _Vdec_VP8HwPowerOff();
	   return FALSE;
    }
	else
	{
		_Vdec_VP8HwPowerOff();
		return TRUE;
	}    
}

UINT32 _VDEC_Vp8PsrWebPProgress(VDEC_VP8_INFO_T *prVpDecInfo)
{
   WEBP_DEC_INFO_T *prWebPInfo=&prVpDecInfo->rWebPInfo;
   UINT32 u4CurY,u4TotalRow;

   if(VDEC_FLGSET(prVpDecInfo->rDecParam.u4Flag,VP8_DEC_FLG_STARTED))
   {
     u4TotalRow=(prVpDecInfo->rDecParam.u2Height+15)/16;
     if(prWebPInfo->u4MbRowIndex==u4TotalRow)
     {
        return 100;
     }
	 else
	 {
        _Vdec_Vp8DecProgress(NULL,&u4CurY);
		return (u4CurY*100)/u4TotalRow;
	 }
   }
   else
   {
     return 0;
   }
}
