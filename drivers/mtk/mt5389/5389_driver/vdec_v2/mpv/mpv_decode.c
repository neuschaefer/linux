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
 * $Author: xiaolei.li $
 * $Date: 2012/10/18 $
 * $RCSfile: mpv_decode.c,v $
 * $Revision: #2 $
 *---------------------------------------------------------------------------*/

/** @file mpv_decode.c
 *  This file contains decoding procedure of MPV.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "mpv_if.h"
#include "mpv_drvif.h"
#include "vdec_if.h"
#include "vdec_drvif.h"
#include "mpv_debug.h"
#include "mpv_parser.h"
#include "mpv_decode.h"
#include "mpv_hw.h"
#include "mpv_config.h"
#include "mpv_errcode.h"

#include "x_ckgen.h"
#if defined(VDEC_IS_POST_MT5368)
#include "srm_drvif.h"
#else
#include "../../srm/srm_drvif.h"
#endif

//for emulation
#ifdef MPV_SEMI_HOST
#include "drv_t32.h"
#endif
#include "fbm_drvif.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_assert.h"
#include "x_handle.h"
#include "x_hal_5381.h"
#include "x_hal_926.h"
#include "x_timer.h"
#ifdef MPV_MID
#include "x_mid.h"
#endif
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define MAX_ES_NUM VDEC_MAX_ES

#define MPV_LOG_COUNT     1000
#define MPV_MIN_TIME        1000        // 1 ms

#define DEC_TIMEOUT    0
#define DEC_FINISH     1

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define IS_REFPIC(prFbmPicHdr)         ((prFbmPicHdr->ucPicCdTp == I_TYPE) || \
                                                              (prFbmPicHdr->ucPicCdTp == P_TYPE))

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static BOOL _fgDecTimeout[MPV_MAX_VLD] = {FALSE};
static UINT32 _au4ErrCnt[MPV_MAX_VLD] = {0};

#ifdef MPV_STATISTICS

static UINT32 _u4IAvgTime[MPV_MAX_VLD];
static UINT32 _u4PAvgTime[MPV_MAX_VLD];
static UINT32 _u4BAvgTime[MPV_MAX_VLD];

static UINT32 _u4IMaxTime[MPV_MAX_VLD];
static UINT32 _u4PMaxTime[MPV_MAX_VLD];
static UINT32 _u4BMaxTime[MPV_MAX_VLD];

static UINT32 _u4IMinTime[MPV_MAX_VLD];
static UINT32 _u4PMinTime[MPV_MAX_VLD];
static UINT32 _u4BMinTime[MPV_MAX_VLD];

static UINT32 _u4ITotalAvgTime[MPV_MAX_VLD];
static UINT32 _u4PTotalAvgTime[MPV_MAX_VLD];
static UINT32 _u4BTotalAvgTime[MPV_MAX_VLD];

static UINT32 _u4ITotalMaxTime[MPV_MAX_VLD];
static UINT32 _u4PTotalMaxTime[MPV_MAX_VLD];
static UINT32 _u4BTotalMaxTime[MPV_MAX_VLD];

static UINT32 _u4ITotalMinTime[MPV_MAX_VLD];
static UINT32 _u4PTotalMinTime[MPV_MAX_VLD];
static UINT32 _u4BTotalMinTime[MPV_MAX_VLD];

static UINT32 _u4ICount[MPV_MAX_VLD];
static UINT32 _u4PCount[MPV_MAX_VLD];
static UINT32 _u4BCount[MPV_MAX_VLD];
static UINT32 _u4TotalCount[MPV_MAX_VLD];
 
HAL_TIME_T _rSwStartDecodeTime[MPV_MAX_VLD];

static UINT32 _au4MaxCycle[MPV_MAX_VLD] = {0};
static UINT32 _au4DelayClock[MPV_MAX_VLD];
static UINT32 _au4DramCycle[MPV_MAX_VLD];
#endif

static HAL_TIME_T _rStartDecodeTime[MPV_MAX_VLD];
static HAL_TIME_T _rStopDecodeTime[MPV_MAX_VLD];

#ifdef MPV_SEMI_HOST                                          
CHAR _szSaveFilePath[256] = "d:/dump";
//CHAR _szGoldFilePath[256] = "t:/testdata/mpv/gold";
CHAR _szGoldFilePath[256] = "t:/mpv/gold";
UCHAR *_pYaddr = NULL;
UCHAR *_pCaddr = NULL;
BOOL _fgMpvEnChkSum = FALSE;
BOOL _fgMpvEnCrc = FALSE;
UINT32 _u4PicCount = 0;
#endif

#ifndef LOCK_TIMEOUT // we should not define this
static HANDLE_T _hMpvTimer[MPV_MAX_VLD];
static HANDLE_T _hDecMsgQ[MPV_MAX_VLD];
#else
static HANDLE_T _ahMpvDecFinSema[MPV_MAX_VLD];
#endif

#ifdef VDEC_TIME_PROFILE
static HAL_TIME_T _rTimeHwS, _rTimeHwDt, _rTimeHwE;
static UINT32 u4DelayClock;
static UINT32 u4DramCycle;
static UINT32 u4DecCycle = 0;
#endif



//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


#ifndef LOCK_TIMEOUT
static void _MpvFlushDecMsgQ(UCHAR ucMpvId)
{
    UINT16 u2Cnt;
    UINT16 u2MsgQIdx;
    UINT32 u4DecFin;
    SIZE_T zMsgSize;
        
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
    zMsgSize = sizeof(INT32);   
    
    while (x_msg_q_receive(&u2MsgQIdx, &u4DecFin, &zMsgSize,
        &_hDecMsgQ[ucMpvId], 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
    }
    
    VERIFY(x_msg_q_num_msgs(_hDecMsgQ[ucMpvId], &u2Cnt) == OSR_OK);
    ASSERT(u2Cnt== 0);    
}


void _MpvTimerTimeout(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    UCHAR ucMpvId;
    UINT32 u4DecFin;
    INT32 i4Ret;    
   
    ucMpvId = (UCHAR)(UINT32)pv_tag;
    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }

    if(_hDecMsgQ[ucMpvId] == 0)
    {
        // MM playback detect PS case, i.e. MPV_Init() is not called.
        return;
    }
    
    u4DecFin = DEC_TIMEOUT;
    i4Ret = x_msg_q_send(_hDecMsgQ[ucMpvId], (void *)&u4DecFin, sizeof(UINT32), 255);
  
    if ((i4Ret != OSR_OK)&&(i4Ret != OSR_TOO_MANY))
    {
        ASSERT(0);
    }
    
    UNUSED(pt_tm_handle);
}
#endif


static UINT32 _ChkDecErr(UCHAR ucMpvId, UCHAR ucEsId)
{
    UINT8 ucLastRow;
    UINT32 u4LastErrTp;
    UINT32 u4ErrStatus = 0;
    UINT32 u4ErrMb;
    static UINT32 _u4ErrType = 0;

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    UNUSED(ucEsId);
    _au4ErrCnt[ucMpvId] = (UINT32)(_MPV_VldErrCnt(ucMpvId));

    if (_au4ErrCnt[ucMpvId] > 0)
    {
        ucLastRow = _MPV_VldLastErrRow(ucMpvId);
        u4LastErrTp = _MPV_VldLastErrType(ucMpvId);

        LOG(5, "(%d)E C: %d , T: %d , R: %d\n", ucEsId, _au4ErrCnt[ucMpvId], u4LastErrTp, ucLastRow);
        #ifdef MPV_138184 
        LOG(5, "0xb0: 0x%x, Rptr: 0x%x, Wptr:0x%x\n",
            _MPV_VldRead32(ucMpvId, 0xb0), _MPV_VldRead32(ucMpvId, RO_VLD_VRPTR), _MPV_VldRead32(ucMpvId, RO_VLD_VWPTR));
        #else
        LOG(9, "0xb0: 0x%x, Rptr: 0x%x, Wptr:0x%x\n",
            _MPV_VldRead32(ucMpvId, 0xb0), _MPV_VldRead32(ucMpvId, RO_VLD_VRPTR), _MPV_VldRead32(ucMpvId, RO_VLD_VWPTR));
        #endif

        MPV_EventNotify(MPV_DEC_ERR, ucMpvId, _au4ErrCnt[ucMpvId], u4LastErrTp);

        u4ErrStatus = _MPV_VldErrStatus(ucMpvId);
        u4ErrMb = _MPV_VldErrMb(ucMpvId);
        
        LOG(5, "VLD 0x114: 0x%x  ErrorMB Count %d\n", u4ErrStatus, u4ErrMb);
        
        _u4ErrType = ((u4ErrStatus & 0x0fffff00) >> 8) | _u4ErrType;        
        LOG(5, " Error type 0x%x\n", _u4ErrType);        

        UNUSED(ucLastRow);
        UNUSED(u4ErrMb);        

        #ifdef MPV_138184
        {
            VDEC_INFO_T *prVdecInfo;
            prVdecInfo = _VDEC_GetInfo();
            if(!prVdecInfo->fgIgnoreErr)
            {
                if(!prVdecInfo->fgSave)
                {
                    _MPV_DumpReg(ucMpvId);
                    ASSERT(0);
                }
            }
        }
        #endif        

    }    
    return u4ErrStatus;
}


static BOOL _DecChkDTS(UCHAR ucEsId)
{
//    MPV_ES_INFO_T *prEsInfo;  
    VDEC_ES_INFO_T *prVdecEsInfo;  
    STC_CLOCK rStcClk;
    UINT32 u4Delta;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    
//    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if (prVdecEsInfo->ucSyncMode == VID_SYNC_MODE_SLAVE)
    {
        if (prVdecEsInfo->u4DTS == 0)
        {
            return (TRUE);
        }

        x_memset(&rStcClk, 0, sizeof(STC_CLOCK));
        if (STC_GetSrc(prVdecEsInfo->ucStcId, &rStcClk) == STC_VALID)
        {                
            if (prVdecEsInfo->u4DTS > rStcClk.u4Base)
            {    
                if (((rStcClk.u4Base & STC_DELTA_MSB) == 0) && 
                    ((UINT32)(prVdecEsInfo->u4DTS & STC_DELTA_MSB) == (UINT32)STC_DELTA_MSB))    
                {
                    u4Delta = ((0xFFFFFFFF - prVdecEsInfo->u4DTS) + rStcClk.u4Base);        

                    if (u4Delta > MPV_DTS_INVALID_DRIFT)
                    {
                        return (TRUE);
                    }
                    if (u4Delta > MPV_DTS_DRIFT)
                    {
                        LOG(6, "0x%x 0x%x (%d)(%d)\n", rStcClk.u4Base, prVdecEsInfo->u4DTS, 
                            u4Delta, FBM_CheckFrameBufferDispQ(prVdecEsInfo->ucFbgId) );                        
                        return (FALSE);
                    }
                }
                else
                {
                    return (TRUE);
                }
        
            }
            else
            {
                if (((prVdecEsInfo->u4DTS & STC_DELTA_MSB) == 0) && 
                    ((UINT32)(rStcClk.u4Base & STC_DELTA_MSB) == (UINT32)STC_DELTA_MSB))    
                {
                    return (TRUE);
                }
                else 
                {
                    u4Delta = (rStcClk.u4Base - prVdecEsInfo->u4DTS);

                    if (u4Delta > MPV_DTS_INVALID_DRIFT)
                    {
                        return (TRUE);
                    }
                    if (u4Delta > MPV_DTS_DRIFT)
                    {
                        LOG(6, "0x%x 0x%x (%d)(%d)\n", rStcClk.u4Base, prVdecEsInfo->u4DTS, 
                            u4Delta, FBM_CheckFrameBufferDispQ(prVdecEsInfo->ucFbgId) ); 
                        return (FALSE);
                    }

                }        
            }        
        }
    }
    return (TRUE);
}



static void _SetMcDecBuf(UCHAR ucMpvId, UCHAR ucEsId, const MPV_ES_INFO_T *prEsInfo, 
                                            const FBM_PIC_HDR_T *prFbmPicHdr)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
    if((prEsInfo == NULL) || (prFbmPicHdr == NULL))
    {
        LOG(1, "_SetMcDecBuf null pointer\n");
        return;
    }
    
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if(!prVdecEsInfo)
    {
        LOG(1, "_SetMcDecBuf fail\n");
        return;
    }

    _MPV_McSetOutputBuf(ucMpvId, MC_Prg_B_BUF_1);    

    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_REDUCE_REF))    
    {
        _MPV_McSetBlkMode(ucMpvId, MC_DESIGNER_VIEW);    
    }
    else
    {
        _MPV_McSetBlkMode(ucMpvId, MC_BLK_NORM);
    }

}


static void _WriteVldProc(UCHAR ucMpvId,  UCHAR ucEsId, MPV_ES_INFO_T *prEsInfo, 
            const FBM_SEQ_HDR_T *prFbmSeqHdr, const FBM_PIC_HDR_T *prFbmPicHdr)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
    if ((prEsInfo == NULL) || (prFbmSeqHdr == NULL) ||
        (prFbmPicHdr == NULL))
    {
        LOG(1, "_WriteVldProc null pointer\n");
        return;
    }
   
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if(!prVdecEsInfo)
    {
        LOG(1, "_WriteVldProc fail\n");
        return;
    }
            
    _MPV_VldDecPrmProc(ucMpvId, ucEsId, prEsInfo, prFbmSeqHdr, prFbmPicHdr);

    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_RACING_FULLB|DEC_FLAG_RACING_HALFB|DEC_FLAG_TRACKING_MODE))
    {
        if (prVdecEsInfo->ucPicType == B_TYPE)
        {
#ifdef VDP_RACE_LOG
            VDP_FrcRacingLog('B');
#endif        
            if (IS_FRMPIC(prFbmPicHdr))
            {    
                LOG(6, "Start B Dec (%d)\n", prFbmPicHdr->i4TemporalRef);
                UNUSED(_MPV_CalcPts(ucEsId, prVdecEsInfo->ucFbId));
                FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucFbId, 
                    FBM_FB_STATUS_DISPLAYQ);
#ifdef VDP_RACE_LOG
            VDP_FrcRacingLog('Q');
#endif
            }
            else //field
            {
                if(!IS_DECFLAG_SET(prVdecEsInfo,DEC_FLG_2ND_FLD_PIC)) //first field
                {
                    LOG(6, "Start B Dec (1st Field) (%d)\n", prFbmPicHdr->i4TemporalRef);
                    UNUSED(_MPV_CalcPts(ucEsId, prVdecEsInfo->ucFbId));
                    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucFbId, 
                        FBM_FB_STATUS_DISPLAYQ);            
                }
                else
                {
                    LOG(6, "Start B Dec (2nd Field) (%d)\n", prFbmPicHdr->i4TemporalRef);
                }
            }
        }
        else
        {
#ifdef VDP_RACE_LOG
            VDP_FrcRacingLog('R');
#endif        
        }            
    }
#if defined(MPV_CRCTEST) || defined(MULTIMEDIA_AUTOTEST_SUPPORT) || defined(VDEC_CRC_TEST)
#if defined(VDEC_CRC_TEST) && !defined(MPV_CRCTEST) && !defined(MULTIMEDIA_AUTOTEST_SUPPORT)
        if(VDEC_IsTestCRC(ucEsId))
#endif
        {
            _MPV_EnableCRC(TRUE, ucEsId, ucMpvId);
        }        
#endif 

#ifdef VDEC_TIME_PROFILE
		prVdecEsInfo->u4CurDecMs = 0;
		prVdecEsInfo->u4CurDramCycle = 0;
		prVdecEsInfo->u4CurDecCycle = 0;
		//profile decoding time
		HAL_GetTime(&_rTimeHwS);

#ifdef CC_MT5396
		u4DelayClock = (BSP_GetDomainClock(SRC_MEM_CLK) >> 1) ; //dram cycle /sec
#else
		u4DelayClock = (BSP_GetDomainClock(CAL_SRC_DMPLL) >> 1) ; //dram cycle /sec
#endif

#if 0    
		if(prVdecEsInfo->u1DramMoniter == 1)  //total bandwidth
		{
    #ifdef CC_MT5395
			u4DramCycle = SRM_DramMonitor(0, SRM_DRV_TOTAL, u4DelayClock, 0);
    #else
			u4DramCycle = SRM_DramMonitor(SRM_DRV_TOTAL, u4DelayClock, 0);
    #endif
		}
		else if(prVdecEsInfo->u1DramMoniter == 2)  //mpeg
#endif        
		{
    #if defined(VDEC_IS_POST_MT5363)
    #if defined(CC_MT5395) || defined(CC_MT5396)
			u4DramCycle = SRM_DramMonitor(0, SRM_DRV_VDEC_MC, u4DelayClock, 0);
    #else
			u4DramCycle = SRM_DramMonitor(SRM_DRV_VDEC_MC, u4DelayClock, 0);
    #endif
    #else
    #if defined(CC_MT5395) || defined(CC_MT5396)
			u4DramCycle = SRM_DramMonitor(0, SRM_DRV_MPEG, u4DelayClock, 0);
    #else
			u4DramCycle = SRM_DramMonitor(SRM_DRV_MPEG, u4DelayClock, 0);
    #endif
    #endif
		}
#endif

    _MPV_VldDec(ucMpvId,  ucEsId, prEsInfo);

}


static void _SetDecFld(UCHAR ucMpvId, UCHAR ucEsId, const MPV_ES_INFO_T *prEsInfo, 
                                      const FBM_PIC_HDR_T *prFbmPicHdr)
{
    UCHAR ucDecFld;
    UCHAR uc2ndFld;
    
    VDEC_ES_INFO_T *prVdecEsInfo;
        
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    
    ASSERT(prEsInfo != NULL);
    ASSERT(prFbmPicHdr != NULL);    
    
    if (IS_FRMPIC(prFbmPicHdr))
    {
        ucDecFld = MC_TOP_FLD;
        uc2ndFld = MC_2ND_FLD_OFF;
    }  
    else
    {
        if (IS_REFPIC(prFbmPicHdr))
        {    
            if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC)) 
            {
                ucDecFld =(UCHAR)( prFbmPicHdr->fgTopFldFirst ? 
                                  MC_BOTTOM_FLD : MC_TOP_FLD);
                uc2ndFld = MC_2ND_FLD_ON;
            }
            else
            {
                ucDecFld = (UCHAR)((prFbmPicHdr->fgTopFldFirst) ?
                                  MC_TOP_FLD : MC_BOTTOM_FLD);  
                uc2ndFld = MC_2ND_FLD_OFF;
            }
        }  
        else  // B Picture
        {
            ucDecFld = (UCHAR)((prFbmPicHdr->ucPicStruct == TOP_FLD_PIC) ?
                              MC_TOP_FLD : MC_BOTTOM_FLD);
            uc2ndFld = MC_2ND_FLD_OFF;
        }  
    }    
  
    _MPV_McSetDecFld(ucMpvId, ucDecFld, uc2ndFld); 
}


static void _SetMcBufAddr(UCHAR ucMpvId, UCHAR ucEsId, const FBM_PIC_HDR_T *prFbmPicHdr, 
                                            const MPV_ES_INFO_T *prEsInfo)
{
    UCHAR ucFbgId;
    UCHAR ucFRefFbId;
    UCHAR ucBRefFbId;
    UCHAR ucDecFbId;
    UINT32 u4AddrY;
    UINT32 u4AddrC;
    VDEC_ES_INFO_T *prVdecEsInfo;
  
    #ifdef MPV_SEMI_HOST
    BOOL fgFBlack = FALSE;
    BOOL fgBBlack = FALSE;
    #endif
        
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    
    ASSERT(prFbmPicHdr != NULL);
    ASSERT(prEsInfo != NULL);
    ucFbgId = prVdecEsInfo->ucFbgId;
    ASSERT(ucFbgId != FBM_FBG_ID_UNKNOWN);        
    
    FBM_GetRefFrameBuffer(ucFbgId, &ucFRefFbId, &ucBRefFbId);
    ucDecFbId = prVdecEsInfo->ucFbId;
    //ucDecFbId = FBM_GetDecFrameBuffer(ucFbgId);
    ASSERT(ucDecFbId != FBM_FB_ID_UNKNOWN);

    
#if 0
    if(IS_FRMPIC(prFbmPicHdr))
    {
        FBM_SetFrameBufferColor(ucFbgId, ucDecFbId, 0x9000ff); //blue color
    }
    else
    {
        MPV_ES_INFO_T *prEsInfo;
        
        prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(0);

        if(!IS_DECFLAG_SET(prVdecEsInfo,DEC_FLG_2ND_FLD_PIC)) 
        {
            FBM_SetFrameBufferColor(ucFbgId, ucDecFbId, 0x9000ff); 
        }

    }

    HalFlushInvalidateDCache();
#endif

    switch (prFbmPicHdr->ucPicCdTp)
    {        
        case I_TYPE:               
            
            if (ucFRefFbId == FBM_FB_ID_UNKNOWN)
            {
                ucFRefFbId = FBM_GetReadyFrameBuffer(ucFbgId);
                
                if (ucFRefFbId ==  FBM_FB_ID_UNKNOWN)
                {
                    ucFRefFbId = 0;
                }
                #ifdef MPV_SEMI_HOST
                fgFBlack = TRUE;
                #endif
            }           

            if (ucBRefFbId == FBM_FB_ID_UNKNOWN)
            {
                ucBRefFbId = ucFRefFbId;
                #ifdef MPV_SEMI_HOST
                fgBBlack = TRUE;
                #endif
            }           
            
            FBM_GetFrameBufferAddr(ucFbgId, ucFRefFbId, &u4AddrY, &u4AddrC);           
            _MPV_McSetFrmBufPtr(ucEsId, ucMpvId, MC_REF_BUF_1, u4AddrY, u4AddrC);
            
            FBM_GetFrameBufferAddr(ucFbgId, ucBRefFbId, &u4AddrY, &u4AddrC);
            _MPV_McSetFrmBufPtr(ucEsId, ucMpvId, MC_REF_BUF_2, u4AddrY, u4AddrC);

            FBM_GetFrameBufferAddr(ucFbgId, ucDecFbId, &u4AddrY, &u4AddrC);
            ASSERT((u4AddrY!=0)&&(u4AddrC !=0));                        
            _MPV_McSetFrmBufPtr(ucEsId, ucMpvId, MC_Prg_B_BUF_1, u4AddrY, u4AddrC);   
            
            break;
            
        case P_TYPE:          
        
            if (ucFRefFbId == FBM_FB_ID_UNKNOWN)
            {
                ucFRefFbId = FBM_GetReadyFrameBuffer(ucFbgId);
                
                if (ucFRefFbId ==  FBM_FB_ID_UNKNOWN)
                {
                    ucFRefFbId = 0;
                }
                #ifdef MPV_SEMI_HOST
                fgFBlack = TRUE;
                #endif
            }           

            if (ucBRefFbId == FBM_FB_ID_UNKNOWN)
            {
                ucBRefFbId = ucFRefFbId;
                #ifdef MPV_SEMI_HOST
                fgBBlack = TRUE;
                #endif
            }           

            LOG(7, "ucDecFbId %d ucBRefFbId %d ucFRefFbId %d\n", ucDecFbId, ucBRefFbId, ucFRefFbId );
            FBM_GetFrameBufferAddr(ucFbgId, ucFRefFbId, &u4AddrY, &u4AddrC);            
            _MPV_McSetFrmBufPtr(ucEsId, ucMpvId, MC_REF_BUF_1, u4AddrY, u4AddrC);
            LOG(7, " ucFRefFbId u4AddrY 0x%x, u4AddrC 0x%x\n", u4AddrY, u4AddrC);            

            FBM_GetFrameBufferAddr(ucFbgId, ucBRefFbId, &u4AddrY, &u4AddrC);
            _MPV_McSetFrmBufPtr(ucEsId, ucMpvId, MC_REF_BUF_2, u4AddrY, u4AddrC);
        
            FBM_GetFrameBufferAddr(ucFbgId, ucDecFbId, &u4AddrY, &u4AddrC);
            ASSERT((u4AddrY!=0)&&(u4AddrC !=0));                        
            _MPV_McSetFrmBufPtr(ucEsId, ucMpvId, MC_Prg_B_BUF_1, u4AddrY, u4AddrC);            
            break;
            
        case B_TYPE:
        
            if (ucFRefFbId == FBM_FB_ID_UNKNOWN)
            {
                ucFRefFbId = FBM_GetReadyFrameBuffer(ucFbgId);
                
                if (ucFRefFbId ==  FBM_FB_ID_UNKNOWN)
                {            
                    ucFRefFbId = 0;  
                }
                #ifdef MPV_SEMI_HOST
                fgFBlack = TRUE;
                #endif
            }

            if (ucBRefFbId == FBM_FB_ID_UNKNOWN)
            {
                ucBRefFbId = ucFRefFbId;
                #ifdef MPV_SEMI_HOST
                fgBBlack = TRUE;
                #endif
            }
            
        	            
            FBM_GetFrameBufferAddr(ucFbgId, ucFRefFbId, &u4AddrY, &u4AddrC);
            _MPV_McSetFrmBufPtr(ucEsId, ucMpvId, MC_REF_BUF_1, u4AddrY, u4AddrC);

            FBM_GetFrameBufferAddr(ucFbgId, ucBRefFbId, &u4AddrY, &u4AddrC);
            _MPV_McSetFrmBufPtr(ucEsId, ucMpvId, MC_REF_BUF_2, u4AddrY, u4AddrC);

            FBM_GetFrameBufferAddr(ucFbgId, ucDecFbId, &u4AddrY, &u4AddrC);           
            ASSERT((u4AddrY!=0)&&(u4AddrC !=0));             
            _MPV_McSetFrmBufPtr(ucEsId, ucMpvId, MC_Prg_B_BUF_1, u4AddrY, u4AddrC);            
            break;
            
        default:
            ASSERT(0);            
            break;
                    
    }
    #ifdef MPV_SEMI_HOST
    {
        UINT32 u4ByteSize;
        VDEC_INFO_T *prVdecInfo;
        prVdecInfo = _VDEC_GetInfo();
        // we use 64x32 block
        u4ByteSize = (prVdecEsInfo->u2LineSize) * (((prVdecEsInfo->u2VSize)+0x3F)&(~0x3F));
        if(prVdecInfo->fgIgnoreErr)
        {
            UNUSED(fgFBlack);
            UNUSED(fgBBlack);
            if(fgFBlack)
            {
                x_memset((VOID*)_pYaddr, 0, u4ByteSize);
                x_memset((VOID*)_pCaddr, 0, u4ByteSize/2);
 #if defined(VDEC_IS_POST_MT5365)
                HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)_pYaddr), u4ByteSize);
                HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)_pCaddr), u4ByteSize/2);
 #else
                 HalFlushInvalidateDCache();
 #endif
                _MPV_McSetFrmBufPtr(ucEsId, ucMpvId, MC_REF_BUF_1, (UINT32)_pYaddr, (UINT32)_pCaddr);
            }
            if(fgBBlack)
            {
                x_memset((VOID*)_pYaddr, 0, u4ByteSize);
                x_memset((VOID*)_pCaddr, 0, u4ByteSize/2);
 #if defined(VDEC_IS_POST_MT5365)
                HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)_pYaddr), u4ByteSize);
                HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)_pCaddr), u4ByteSize/2);
 #else
                 HalFlushInvalidateDCache();
 #endif                
                 _MPV_McSetFrmBufPtr(ucEsId, ucMpvId, MC_REF_BUF_2, (UINT32)_pYaddr, (UINT32)_pCaddr);
            }
            // frame data
            {
                x_memset((VOID*)u4AddrY, 0, u4ByteSize);
                x_memset((VOID*)u4AddrC, 0, u4ByteSize/2);
 #if defined(VDEC_IS_POST_MT5365)
                HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)_pYaddr), u4ByteSize);
                HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)_pCaddr), u4ByteSize/2);
 #else
                 HalFlushInvalidateDCache();
 #endif
            }
        }
    }
    #endif

    #ifdef MPV_MID
    {
        FBM_SEQ_HDR_T *prFbmSeqHdr;
        UINT32 u4ByteSize;
        
        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
        u4ByteSize = (prVdecEsInfo->u2LineSize) * (((prVdecEsInfo->u2VSize) + 0x1f) & (~0x1f));
        MID_SetRegion(0, MID_AGENT_MPEG1, u4AddrY, u4AddrY + u4ByteSize);
        MID_SetRegion(1, MID_AGENT_MPEG1, u4AddrC, u4AddrC + u4ByteSize);    
        MID_EnableRegionProtect(0);    
        MID_EnableRegionProtect(1);    
    }
    #endif

}


#ifdef MPV_STATISTICS
static void _CalDecTime(UCHAR ucMpvId, UCHAR ucEsId)
{
    UINT32 u4Delta[MPV_MAX_VLD];
    UINT32 u4TotalDelta[MPV_MAX_VLD];

    HAL_TIME_T rDelta[MPV_MAX_VLD];
    HAL_TIME_T rTotalDelta[MPV_MAX_VLD];

    //MPV_ES_INFO_T *prEsInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
        
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
    //prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);


    HAL_GetDeltaTime(&rDelta[ucMpvId], &_rStartDecodeTime[ucMpvId], &_rStopDecodeTime[ucMpvId]);
    HAL_GetDeltaTime(&rTotalDelta[ucMpvId], &_rSwStartDecodeTime[ucMpvId], &_rStopDecodeTime[ucMpvId]);

    u4Delta[ucMpvId] = rDelta[ucMpvId].u4Micros;
    u4TotalDelta[ucMpvId] = rTotalDelta[ucMpvId].u4Micros;

    if (u4Delta[ucMpvId] > 50000)
    {
        LOG(10, "Big Delta (%d) (%d.%06d) - (%d.%06d)\n", 
            u4Delta[ucMpvId],
            _rStartDecodeTime[ucMpvId].u4Seconds,
            _rStartDecodeTime[ucMpvId].u4Micros,
            _rStopDecodeTime[ucMpvId].u4Seconds,
            _rStopDecodeTime[ucMpvId].u4Micros
            );
    }

    // [LOG] Mpeg decode finish
    //SYSLOG(ucEsId + 20, u4Delta, u4TotalDelta, 0);

    _au4MaxCycle[ucMpvId] = (_au4MaxCycle[ucMpvId] > _au4DramCycle[ucMpvId]) ?_au4MaxCycle[ucMpvId] : _au4DramCycle[ucMpvId];

    #if 1
    // Print decode time, bandwidth
    {
        LOG(1, "Type(%d), Dtime: %d, Bwidth: %d\n",
            prVdecEsInfo->ucPicType,
            u4Delta[ucMpvId],
            _au4DramCycle[ucMpvId]*16);
    }
    #endif

    if (rDelta[ucMpvId].u4Micros > MPV_MIN_TIME)
    {
        switch (prVdecEsInfo->ucPicType )
        {
            case I_TYPE:
                //if (u4ICount[ucMpvId] < 160000)
                {
                    _u4IAvgTime[ucMpvId] = ((_u4IAvgTime[ucMpvId] * _u4ICount[ucMpvId]) + u4Delta[ucMpvId]) / (_u4ICount[ucMpvId] + 1);
                    _u4ITotalAvgTime[ucMpvId] = ((_u4ITotalAvgTime[ucMpvId] * _u4ICount[ucMpvId]) + u4TotalDelta[ucMpvId]) / (_u4ICount[ucMpvId] + 1);
                }
                _u4ICount[ucMpvId] ++;

                _u4IMaxTime[ucMpvId] = (_u4IMaxTime[ucMpvId] > u4Delta[ucMpvId]) ?_u4IMaxTime[ucMpvId] : u4Delta[ucMpvId];
                _u4IMinTime[ucMpvId] = (_u4IMinTime[ucMpvId] < u4Delta[ucMpvId]) ? _u4IMinTime[ucMpvId] : u4Delta[ucMpvId];

                _u4ITotalMaxTime[ucMpvId] = (_u4ITotalMaxTime[ucMpvId] > u4TotalDelta[ucMpvId]) ? _u4ITotalMaxTime[ucMpvId] : u4TotalDelta[ucMpvId];
                _u4ITotalMinTime[ucMpvId] = (_u4ITotalMinTime[ucMpvId] < u4TotalDelta[ucMpvId]) ? _u4ITotalMinTime[ucMpvId] : u4TotalDelta[ucMpvId];
                break;
                    
            case P_TYPE:
                //if (u4PCount[ucMpvId] < 260000)
                {                    
                    _u4PAvgTime[ucMpvId] = ((_u4PAvgTime[ucMpvId] * _u4PCount[ucMpvId] )+ u4Delta[ucMpvId]) / (_u4PCount[ucMpvId] + 1);
                    _u4PTotalAvgTime[ucMpvId] = ((_u4PTotalAvgTime[ucMpvId] * _u4PCount[ucMpvId]) + u4TotalDelta[ucMpvId]) / (_u4PCount[ucMpvId] + 1);
                }
                _u4PCount[ucMpvId] ++;

                _u4PMaxTime[ucMpvId] = (_u4PMaxTime[ucMpvId] > u4Delta[ucMpvId]) ? _u4PMaxTime[ucMpvId] : u4Delta[ucMpvId];
                _u4PMinTime[ucMpvId] = (_u4PMinTime[ucMpvId] < u4Delta[ucMpvId]) ? _u4PMinTime[ucMpvId] : u4Delta[ucMpvId];

                _u4PTotalMaxTime[ucMpvId] = (_u4PTotalMaxTime[ucMpvId] > u4TotalDelta[ucMpvId]) ? _u4PTotalMaxTime[ucMpvId] : u4TotalDelta[ucMpvId];
                _u4PTotalMinTime[ucMpvId] = (_u4PTotalMinTime[ucMpvId] < u4TotalDelta[ucMpvId]) ? _u4PTotalMinTime[ucMpvId] : u4TotalDelta[ucMpvId];
                break;
                
            case B_TYPE:
                //if (u4BCount[ucMpvId] < 390000)
                {                                        
                    _u4BAvgTime[ucMpvId] = ((_u4BAvgTime[ucMpvId] * _u4BCount[ucMpvId]) + u4Delta[ucMpvId]) / (_u4BCount[ucMpvId] + 1);
                    _u4BTotalAvgTime[ucMpvId] = ((_u4BTotalAvgTime[ucMpvId] * _u4BCount[ucMpvId]) + u4TotalDelta[ucMpvId]) / (_u4BCount[ucMpvId] + 1);
                }
                _u4BCount[ucMpvId] ++;

                _u4BMaxTime[ucMpvId] = (_u4BMaxTime[ucMpvId] > u4Delta[ucMpvId]) ? _u4BMaxTime[ucMpvId] : u4Delta[ucMpvId];
                /*                    
                if (u4BMaxTime[ucMpvId] == u4Delta[ucMpvId])
                {
                    LOG(1, "B max count %d\n", u4BCount[ucMpvId]);
                }
                */
                                    
                _u4BMinTime[ucMpvId] = (_u4BMinTime[ucMpvId] < u4Delta[ucMpvId]) ? _u4BMinTime[ucMpvId] : u4Delta[ucMpvId];

                _u4BTotalMaxTime[ucMpvId] = (_u4BTotalMaxTime[ucMpvId] > u4TotalDelta[ucMpvId]) ? _u4BTotalMaxTime[ucMpvId] : u4TotalDelta[ucMpvId];
                _u4BTotalMinTime[ucMpvId] = (_u4BTotalMinTime[ucMpvId] < u4TotalDelta[ucMpvId]) ? _u4BTotalMinTime[ucMpvId] : u4TotalDelta[ucMpvId];
                break;
            
            default:
                ASSERT(0);
                break;                            
        }
        _u4TotalCount[ucMpvId]++;
    }
    else
    {
        LOG(1, "Es(%d) Pic(%d) Time (%d, %d)\n", ucEsId, prVdecEsInfo->ucPicType, rDelta[ucMpvId].u4Seconds, rDelta[ucMpvId].u4Micros);
    }

    LOG(7, "Es(%d) Pic(%d) Time (%d, %d)\n", ucEsId, prVdecEsInfo->ucPicType, rDelta[ucMpvId].u4Seconds, rDelta[ucMpvId].u4Micros);

    if ((_u4TotalCount[ucMpvId] % MPV_LOG_COUNT) == (MPV_LOG_COUNT - 1))
    {
        //_MPV_QueryEsInfo();
        _MPV_PrintTime(ucMpvId);         
    }

}
#endif

static void _CalBDecTime(UCHAR ucMpvId, UCHAR ucEsId)
{
    UINT32 u4Delta[MPV_MAX_VLD];
    HAL_TIME_T rDelta[MPV_MAX_VLD];
    MPV_ES_INFO_T *prEsInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
        
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);   
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);    

    HAL_GetDeltaTime(&rDelta[ucMpvId], &_rStartDecodeTime[ucMpvId], &_rStopDecodeTime[ucMpvId]);

    u4Delta[ucMpvId] = rDelta[ucMpvId].u4Micros;

    if (u4Delta[ucMpvId] > prVdecEsInfo->u4BigBLevel1)
    {
        if (u4Delta[ucMpvId] > prVdecEsInfo->u4BigBLevel3)
        {
            prEsInfo->ucBigB = (UCHAR)(3*prVdecEsInfo->u4BigBBaseCnt);
        }
        else
        if (u4Delta[ucMpvId] > prVdecEsInfo->u4BigBLevel2)
        {
            prEsInfo->ucBigB = (UCHAR)(2*prVdecEsInfo->u4BigBBaseCnt);
        }        
        else
        {
            if (prEsInfo->ucBigB < (UCHAR)prVdecEsInfo->u4BigBBaseCnt)
            {
                prEsInfo->ucBigB = (UCHAR)prVdecEsInfo->u4BigBBaseCnt;
            }
        }                    
    }
    else
    {
        if (prEsInfo->ucBigB != 0)
        {
            prEsInfo->ucBigB--;
        }
    }

#if 0 //use deblocking filter protection instead of dynamicDBK
    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK) && (u4Delta[ucMpvId] > MPV_DISABLE_DBK_TIME))
    {
        MPV_Play(ucEsId);
        LOG(5, "Disable DBK(%d)\n", u4Delta[ucMpvId]);                    
    }
    else if (!IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK) && (prEsInfo->fgDbkFbg))
    {
        if ((rDelta[ucMpvId].u4Micros < MPV_ENABLE_DBK_TIME) && (rDelta[ucMpvId].u4Micros > MPV_MIN_TIME))
        {
            _EnableDbk(ucEsId, 1);
            LOG(5, "Enable DBK(%d)\n", u4Delta[ucMpvId]);        
        }        
    }    
#endif
}


INT32 _MPV_DecodeProc(UCHAR ucMpvId, UCHAR ucEsId)
{
    UCHAR ucFbgId;
    UCHAR ucDecFbId;
    VDEC_DISP_PIC_INFO_T rDispInfo;
    FBM_SEQ_HDR_T *prFbmSeqHdr;
    FBM_PIC_HDR_T *prFbmPicHdr;
    MPV_ES_INFO_T *prEsInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;


    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    
    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    ucFbgId = GET_FBG_ID(prVdecEsInfo);
    ASSERT(ucFbgId != FBM_FBG_ID_UNKNOWN);
    
    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
    if(prFbmSeqHdr == NULL)
    {
        ASSERT(prFbmSeqHdr != NULL);
        return (VDEC_ERR);
    }
      
#ifdef CC_MPV_DEBLOCKING
    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK) && (prVdecEsInfo->ucPicType == B_TYPE))
    {
        ucDecFbId = prVdecEsInfo->ucDbkFbId;             
    }
    else
#endif
    {
        ucDecFbId = prVdecEsInfo->ucFbId; 
    }
    
    ASSERT(ucDecFbId != FBM_FB_ID_UNKNOWN);
    
    if (ucDecFbId == FBM_FB_ID_UNKNOWN)
    {
        LOG(3, "ES(%d) Fbg(%d) ucDecFbId unkonwn\n", ucEsId, ucFbgId);
        return (VDEC_ERR);
        //return (NO_DECODE);
    }

    prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucDecFbId);
    if(prFbmPicHdr == NULL)
    {
        ASSERT(prFbmPicHdr != NULL);
        return (VDEC_ERR);
    }

    if (prVdecEsInfo->fgDtsValid)
    {    
        if (IS_REFPIC(prFbmPicHdr)) //I,P
        {              
            if (!_DecChkDTS(ucEsId))
            {    
                LOG(3, "AV Sync. EsId %d DTS T\n", ucEsId);
                FBM_SetFrameBufferFlag(ucFbgId, FBM_FLAG_SKIP_PIC);
            }
        }    
    }

#ifdef CC_MPV_DEBLOCKING
    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK))
    {
        _MPV_SetMcDbkBufAddr(ucEsId, ucMpvId, ucFbgId, prVdecEsInfo->ucDbkFbId, prFbmPicHdr, prEsInfo);
    }
    else
#endif
    {
        _SetMcBufAddr(ucMpvId, ucEsId, prFbmPicHdr, prEsInfo);  
    }

    if(prVdecEsInfoKeep->fgVdecMidEnable)
    {                
        x_memset(&rDispInfo, 0, sizeof(VDEC_DISP_PIC_INFO_T));
        rDispInfo.pvYAddr = (void*)((UINT32)MC_READ32(ucMpvId, RW_MC_BY1) << 9);
        rDispInfo.pvCAddr = (void*)((UINT32)MC_READ32(ucMpvId, RW_MC_BC1) << 8);
        //rDispInfo.pvYDbkAddr = (void*)((UINT32)MC_READ32(ucMpvId, RW_MC_DBK_Y) << 9);
        //rDispInfo.pvCDbkAddr = (void*)((UINT32)MC_READ32(ucMpvId, RW_MC_DBK_C) << 8);
#ifdef CC_MPV_DEBLOCKING
        if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK))
        {
            rDispInfo.pvYDbkAddr = (void*)((UINT32)MC_READ32(ucMpvId, 0x224) << 9);
            rDispInfo.pvCDbkAddr = (void*)((UINT32)MC_READ32(ucMpvId, 0x228) << 8);
        }
#endif
        rDispInfo.u4YSize = 
            (UINT32)(((UINT32)prVdecEsInfo->u2LineSize) * (((prVdecEsInfo->u2VSize)+0x1f)&(~0x1f)));
        rDispInfo.u4CSize = rDispInfo.u4YSize >> 1;
        if(!_VDEC_MidSetting(ucEsId, &rDispInfo))
        {
            LOG(3, "VdecMid Fail~\n");
        }
    }

    _SetMcDecBuf(ucMpvId, ucEsId, prEsInfo, prFbmPicHdr);
    _SetDecFld(ucMpvId, ucEsId, prEsInfo, prFbmPicHdr);
    _WriteVldProc(ucMpvId, ucEsId, prEsInfo, prFbmSeqHdr, prFbmPicHdr);

    return (VDEC_SUCCESS);
}


static void _WriteVldProcTwice(UCHAR ucMpvId,  UCHAR ucEsId, MPV_ES_INFO_T *prEsInfo, 
    const FBM_SEQ_HDR_T *prFbmSeqHdr, const FBM_PIC_HDR_T *prFbmPicHdr)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    
    if(prEsInfo==NULL)
    {
        LOG(1, "_WriteVldProcTwice null pointer\n");
        return;
    }
        
    _MPV_VldDecPrmProc(ucMpvId, ucEsId, prEsInfo, prFbmSeqHdr, prFbmPicHdr);

    if (prVdecEsInfo->ucPicType == B_TYPE)
    {
        UCHAR ucFbgId;

        ucFbgId = GET_FBG_ID(prVdecEsInfo);
        prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, prVdecEsInfo->ucFbId);
        if(prFbmPicHdr)
        {
            LOG(5, "Start B Dec (%d)\n", prFbmPicHdr->i4TemporalRef);
        }
    }
    
#ifdef MPV_STATISTICS
    HAL_GetTime(&_rStartDecodeTime[ucMpvId]);
#endif
    
}


static INT32 _DecodeProcTwice(UCHAR ucMpvId, UCHAR ucEsId)
{
    UCHAR ucFbgId;
    UCHAR ucDecFbId;
    FBM_SEQ_HDR_T *prFbmSeqHdr;
    FBM_PIC_HDR_T *prFbmPicHdr;
    MPV_ES_INFO_T *prEsInfo;
    
    VDEC_ES_INFO_T *prVdecEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    ucFbgId = GET_FBG_ID(prVdecEsInfo);
    ASSERT(ucFbgId != FBM_FBG_ID_UNKNOWN);
    
    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
    ASSERT(prFbmSeqHdr != NULL);
    
    ucDecFbId = prVdecEsInfo->ucFbId; 
    ASSERT(ucDecFbId != FBM_FB_ID_UNKNOWN);
    
    if (ucDecFbId == FBM_FB_ID_UNKNOWN)
    {
        LOG(3, "ES(%d) Fbg(%d) ucDecFbId unkonwn\n", ucEsId, ucFbgId);
        return (VDEC_ERR);
    }

    prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucDecFbId);
    if(prFbmPicHdr == NULL)
    {
        ASSERT(prFbmPicHdr != NULL);
        return (VDEC_ERR);
    }
    
    if (IS_REFPIC(prFbmPicHdr)) //I,P
    {
        //STC not ready
        #if 0
        if (!_DecChkDTS(ucEsId))
        {    
            LOG(3, "AV Sync. EsId %d DTS T\n", ucEsId);
            FBM_SetFrameBufferFlag(ucFbgId, FBM_FLAG_SKIP_PIC);
        }
        #endif
    }    

    _SetMcBufAddr(ucMpvId, ucEsId, prFbmPicHdr, prEsInfo);  

    _SetMcDecBuf(ucMpvId, ucEsId, prEsInfo, prFbmPicHdr);
    _SetDecFld(ucMpvId, ucEsId, prEsInfo, prFbmPicHdr);
    _WriteVldProcTwice(ucMpvId, ucEsId, prEsInfo, prFbmSeqHdr, prFbmPicHdr);

    return (VDEC_SUCCESS);

}

static INT32 _RacingProc(UCHAR ucMpvId, UCHAR ucEsId, const FBM_PIC_HDR_T* prFbmPicHdr)
{
#ifndef LOCK_TIMEOUT
    UINT16 u2MsgQIdx;
    UINT32 u4DecFin;
    SIZE_T zMsgSize; 
#else
    INT32 i4ReturnValue = 0;                        
#endif
    MPV_ES_INFO_T *prEsInfo;  

    VDEC_ES_INFO_T *prVdecEsInfo;
    
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    
    if(prFbmPicHdr==NULL)
    {
        LOG(1, "_RacingProc null pointer\n");
        return NONE_ERROR;
    }

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);

    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_RACING_HALFB))
    {
        if (prVdecEsInfo->ucPicType == B_TYPE)
        {
            //FBM_PIC_HDR_T *prFbmPicHdr;
            //prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, prVdecEsInfo->ucFbId);

            if (!IS_FRMPIC(prFbmPicHdr) && 
                (!IS_DECFLAG_SET(prVdecEsInfo,DEC_FLG_2ND_FLD_PIC)))
            {
                // 1 st field of Field Picture
                UCHAR ucChangeFieldNs;
                ucChangeFieldNs = FBM_GetFrameBufferChangeFieldNs(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucFbId);    
                LOG(5, "Dec Fin, MBL = %x, PlayBack(%d, %d)\n", _MPV_McRead32(ucMpvId, RO_MC_MBY), ucChangeFieldNs, prFbmPicHdr->i4TemporalRef);

                while(ucChangeFieldNs > 2)
                {
                    // _InitHw(ucMpvId, ucEsId);                 
                    VERIFY(_MPV_VldInitBarrelShifter(ucMpvId, prVdecEsInfo->u4DecReadPtr));
                    
                    if (!_MPV_NextSliceHdr(ucMpvId, ucEsId))
                    {
                        return (NO_START_C_ERR12);
                    }

                   // _DecodeProcTwice(ucMpvId, ucEsId);    //decode again

                    {
                        UINT32 u4RegVal = 0;

                        u4RegVal = _MPV_VldRead32(ucMpvId, RW_VLD_MBROWPRM);                        
                        u4RegVal |= (RACING_EN|WAIT_VDO_PULSE);                                    
                        VLD0_WRITE32(RW_VLD_MBROWPRM, u4RegVal);
                    }

            #ifdef RACING_COMPACT
            {
                    UINT32 u4Compact = 0;

                    if (prEsInfo->fgNextBtm)
                    {
                        u4Compact |= (Y_BTM |C_BTM);
                    }
                    
                    u4Compact |= (Y_EN |C_EN);
                    
                    MC_WRITE32(ucMpvId, RW_MC_COMPACT_FLD, u4Compact);                
                    prEsInfo->fgNextBtm = !prEsInfo->fgNextBtm;
            }
            #endif
            
                    _MPV_VldDec(ucMpvId,  ucEsId, prEsInfo);

                #ifndef LOCK_TIMEOUT
                    zMsgSize = sizeof(INT32);
                    VERIFY(x_msg_q_receive(&u2MsgQIdx, &u4DecFin, &zMsgSize,
                            &(_hDecMsgQ[ucMpvId]), 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
                #else
                    VERIFY (x_sema_lock(_ahMpvDecFinSema[ucMpvId], X_SEMA_OPTION_WAIT) == OSR_OK);
                #endif

                    ucChangeFieldNs = FBM_GetFrameBufferChangeFieldNs(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucFbId);    
                
                    LOG(5, "Dec Fin, MBL = %x, PlayBack(%d, %d) (while)\n", _MPV_McRead32(ucMpvId, RO_MC_MBY), ucChangeFieldNs, prFbmPicHdr->i4TemporalRef);            
                }

                if (ucChangeFieldNs > 1)
                {
                    // _InitHw(ucMpvId, ucEsId);   //reset HW        
                    VERIFY(_MPV_VldInitBarrelShifter(ucMpvId, prVdecEsInfo->u4DecReadPtr));
                    
                    if (!_MPV_NextSliceHdr(ucMpvId, ucEsId))
                    {
                        return (NO_START_C_ERR15);
                    }
                    
                    //_DecodeProcTwice(ucMpvId, ucEsId);    //decode again

                    {
                        UINT32 u4RegVal = 0;

                        u4RegVal = _MPV_VldRead32(ucMpvId, RW_VLD_MBROWPRM);                        
                        u4RegVal |= (RACING_EN|WAIT_VDO_PULSE);   
                        VLD0_WRITE32(RW_VLD_MBROWPRM, u4RegVal);
                    }


            #ifdef RACING_COMPACT
                {
                    UINT32 u4Compact = 0;

                    if (prEsInfo->fgNextBtm)
                    {
                        u4Compact |= (Y_BTM |C_BTM);
                    }
                    
                    u4Compact |= (Y_EN |C_EN);
                    
                    MC_WRITE32(ucMpvId, RW_MC_COMPACT_FLD, u4Compact);                

                    prEsInfo->fgNextBtm = !prEsInfo->fgNextBtm;

                }
            #endif
                    
                    _MPV_VldDec(ucMpvId,  ucEsId, prEsInfo);

                #ifndef LOCK_TIMEOUT
                    zMsgSize = sizeof(INT32);
                    VERIFY(x_msg_q_receive(&u2MsgQIdx, &u4DecFin, &zMsgSize,
                            &(_hDecMsgQ[ucMpvId]), 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
                #else
                    VERIFY (x_sema_lock(_ahMpvDecFinSema[ucMpvId], X_SEMA_OPTION_WAIT) == OSR_OK);
                #endif
                
                    LOG(5, "Dec Fin, MBL = %x, PlayBack(%d, %d) (Last)\n", _MPV_McRead32(ucMpvId, RO_MC_MBY), ucChangeFieldNs, prFbmPicHdr->i4TemporalRef);            
                }

                LOG(1, "Dec Leave 1st Field (%d)\n", prFbmPicHdr->i4TemporalRef);
            
            }             
            else            	
            {
				UCHAR ucNextFieldPlayBackNs;
                UINT32 u4Compact = 0;                
                UINT32 u4RegVal = 0;                
                INT32 i4Ret;               

				i4Ret = FBM_WaitFbChangeField(prVdecEsInfo->ucFbgId, MPV_WAIT_CHGFLD_TIME);

				if (i4Ret == OSR_OK)
				{				
					ucNextFieldPlayBackNs = FBM_GetFbNextFieldPlayBackNs(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucFbId);

        		#ifdef VDP_RACE_LOG
		            VDP_FrcRacingLog('P');
		            VDP_FrcRacingLog('0'+ucNextFieldPlayBackNs);		
	        	#endif	
	             			
					if (ucNextFieldPlayBackNs)
                    {
                        _MPV_DecReset(ucMpvId, ucEsId);                      //reset HW        
                        VERIFY(_MPV_VldInitBarrelShifter(ucMpvId, prVdecEsInfo->u4DecReadPtr));

                        if (!_MPV_NextSliceHdr(ucMpvId, ucEsId))
                        {
                            return (NO_START_C_ERR16);
                        }

                        VERIFY(_DecodeProcTwice(ucMpvId, ucEsId) == VDEC_SUCCESS);    //decode again

                        u4RegVal = _MPV_VldRead32(ucMpvId, RW_VLD_MBROWPRM);                        
                        u4RegVal |= (RACING_EN|WAIT_VDO_PULSE);                                    
                        VLD0_WRITE32(RW_VLD_MBROWPRM, u4RegVal);
					
		                if (prEsInfo->fgNextBtm)
		                {		                
		                    u4Compact |= (Y_BTM |C_BTM);
                		#ifdef VDP_RACE_LOG
		                    VDP_FrcRacingLog('M');
                		#endif	                    
		                    
		                }
		                else
		                {
                		#ifdef VDP_RACE_LOG
		                  VDP_FrcRacingLog('T');
                		#endif	                    		                
		                }
		  
		                u4Compact |= (Y_EN |C_EN);
		                
		                MC_WRITE32(ucMpvId, RW_MC_COMPACT_FLD, u4Compact);                

		                prEsInfo->fgNextBtm = !prEsInfo->fgNextBtm;			
		                		                
		                _MPV_VldDec(ucMpvId,  ucEsId, prEsInfo);

                    #ifndef LOCK_TIMEOUT                        
                        zMsgSize = sizeof(INT32);

                        VERIFY(x_timer_start(_hMpvTimer[ucMpvId], 500, X_TIMER_FLAG_ONCE, 
                                _MpvTimerTimeout, (void*)(UINT32)ucMpvId) == OSR_OK);        

                        VERIFY(x_msg_q_receive(&u2MsgQIdx, &u4DecFin, &zMsgSize,
                            &(_hDecMsgQ[ucMpvId]), 1, X_MSGQ_OPTION_WAIT) == OSR_OK);

                        if (u4DecFin != DEC_FINISH)
                    #else
					    i4ReturnValue = x_sema_lock_timeout(_ahMpvDecFinSema[ucMpvId], MPV_WAIT_DEC_TIME);

		                if (i4ReturnValue != OSR_OK)
                    #endif
		                {
		                    LOG(1, "0x%08x, 0x%08x, 0x%08x\n", _MPV_VldRead32(ucMpvId, RO_VLD_FSBI), 
					            _MPV_McRead32(ucMpvId, RO_MC_STATUS), _MPV_McRead32(ucMpvId, RO_MC_IDCTBUF_STATUS));
		                }                        
		         
		            #ifdef VDP_RACE_LOG
		                VDP_FrcRacingLog('F');
                		#endif	
					}
				}	
				else
				{
           		#ifdef VDP_RACE_LOG
	                VDP_FrcRacingLog('U');
        		#endif	
        		
                    return VDEC_ERR;        		

				}
            }

        }
    }
    return VDEC_SUCCESS;
}

// for A1
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
//#define MPV_REPLACE_0x1F4
#endif

// workaround for 5396 IC
// prevent h/w concealment whole frame by change 0x1F4 to 0x1AF
#ifndef MPV_REPLACE_0x1F4
#define MPV_RETRY_TIMES       (1)
#else
#define MPV_RETRY_TIMES       (2)
#define MPV_RETRY_SEARCH_BYTE (128)
#define MPV_1_MB              (1024*1024)
UCHAR *_pucCurMinus5;
UCHAR *_pucCur;
UINT32 _u4Cur;
UINT32 _u4Cnt;
UINT32 _u4FifoStart;
UINT32 _u4FifoEnd;
UINT32 _u4PicStart;
UINT32 _u4PicEnd;
UINT32 _u4PicBytes;

VOID _advance(VOID)
{
    _u4Cur <<= 8;
    _u4Cur += (UINT32)*_pucCur;
    _u4Cnt++;
    _pucCur++;
    if((UINT32)_pucCur >= _u4FifoEnd)
    {
        _pucCur = (UCHAR*)_u4FifoStart;
    }
    _pucCurMinus5++;
    if((UINT32)_pucCurMinus5 >= _u4FifoEnd)
    {
        _pucCurMinus5 = (UCHAR*)_u4FifoStart;
    }
}
UCHAR _getByte(VOID)
{
    return *_pucCur;
}
UINT32 _get4bytes(VOID)
{
    return _u4Cur;
}
BOOL _findinit(UCHAR ucMpvId, UCHAR ucEsId)
{
    UINT32 i;
    //UINT32 u4PicOffset;
    UINT32 u4PicSize;
    VDEC_ES_INFO_T *prVdecEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId); 
    if(prVdecEsInfo == NULL)
    {
        LOG(1, "%s(L:%d) prVdecEsInfo null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    
    _u4PicStart = VIRTUAL(prVdecEsInfo->u4DecReadPtr);
    _u4PicEnd = VIRTUAL(prVdecEsInfo->u4VldReadPtrEnd);
    _u4FifoStart = VIRTUAL(prVdecEsInfo->u4FifoStart);
    _u4FifoEnd = VIRTUAL(prVdecEsInfo->u4FifoEnd);
    if((_u4FifoStart >= _u4FifoEnd) || 
       (_u4PicStart < _u4FifoStart) ||
       (_u4PicEnd < _u4FifoStart) ||
       (_u4PicStart >= _u4FifoEnd) ||
       (_u4PicEnd >= _u4FifoEnd))
    {
        LOG(1, "%s(L:%d) pic out of fifo range\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    if(_u4PicEnd >= _u4PicStart)
    {
        u4PicSize = (_u4PicEnd - _u4PicStart);
    }
    else
    {
        u4PicSize = (_u4FifoEnd - _u4PicStart) + (_u4PicEnd - _u4FifoStart);
    }
    _pucCur = (UCHAR*)VIRTUAL(_MPV_SaveReadPtr(ucMpvId));
    if(_u4PicEnd >= (UINT32)_pucCur)
    {
        _u4PicBytes = (_u4PicEnd - (UINT32)_pucCur);
    }
    else
    {
        _u4PicBytes = (_u4FifoEnd - (UINT32)_pucCur) + (_u4PicEnd - _u4FifoStart);
    }
    if((u4PicSize > MPV_1_MB) ||
       (_u4PicBytes > MPV_1_MB))
    {
        LOG(1, "%s(L:%d) size > 1MB\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    // 32 is comment by SJ Wang, start to search from readptr - 32
    if(u4PicSize <= (_u4PicBytes + 32)) 
    {
        _pucCur = (UCHAR*)_u4PicStart;
    }
    else
    {
        _pucCur = (UCHAR*)(_u4PicStart + (u4PicSize - (_u4PicBytes + 32)));
        if(_pucCur >= (UCHAR*)_u4FifoEnd)
        {
            _pucCur = _pucCur - (_u4FifoEnd - _u4FifoStart);
        }
    }
    if((UINT32)_pucCur < 5)
    {
        LOG(1, "%s(L:%d) _pucCur < 5\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    _u4Cnt = 0;
    _pucCurMinus5 = _pucCur - 5;
    for(i=0;i<4;i++)
    {
        _advance();
    }
    return TRUE;
}
BOOL _findend(VOID)
{
    // search range 
    if((_u4Cnt >= _u4PicBytes) || (_u4Cnt > 128))
    {
        return TRUE;
    }
    return FALSE;
}
UCHAR *_find16zero(VOID)
{
    //BOOL fgRet;
    UINT32 u4Temp;
    u4Temp = _get4bytes();
    // pattern is 0x0000xxxx
    if(((u4Temp & 0xFFFF0000) == 0) &&
       ((u4Temp & 0xFFFF) != 0))
    {
        return _pucCurMinus5;
    }
    return 0;
}
BOOL _findSysStartCode(UCHAR *pucPtr)
{
    UINT32 i, j;
    UINT32 u4Tmp;
    UINT32 u4Pattern; // 0x000001AF
    UCHAR ucTmp;
    UCHAR *pucPtrOrg;

    u4Pattern = 0x1AF; // max slice start code
    pucPtrOrg = pucPtr;
    // get first 4 bytes
    u4Tmp = 0;
    for(i=0;i<4;i++)
    {
        u4Tmp <<= 8;
        u4Tmp += (UINT32)*pucPtr;
        pucPtr++;
        if((UINT32)pucPtr >= _u4FifoEnd)
        {
            pucPtr = (UCHAR*)_u4FifoStart;
        }
    }
    ucTmp = *pucPtr;
    pucPtr++;
    if((UINT32)pucPtr >= _u4FifoEnd)
    {
        pucPtr = (UCHAR*)_u4FifoStart;
    }
    // check the later 2 bytes
    for(i=0;i<2;i++)
    {
        for(j=0;j<8;j++)
        {
            if((u4Tmp & 0xFFFFFF00) == 0x100)
            {
                if(((u4Tmp & 0xFF) >= 0xB0) &&
                   ((u4Tmp & 0xFF) <= 0xFF))
                {
                    // found, replace
                    // Replance System start code with Max Slice start code
                    LOG(1, "R S w MS\n");
                    pucPtrOrg += 4;
                    if((UINT32)pucPtrOrg >= _u4FifoEnd)
                    {
                        pucPtrOrg -= (_u4FifoEnd - _u4FifoStart);
                    }
                    *(pucPtrOrg) = (UCHAR)(((u4Pattern << (8-j)) >> 8) & 0xFF);
                    HalFlushInvalidateDCacheMultipleLine((UINT32)(pucPtrOrg), 1);
                    if(j)
                    {
                        pucPtrOrg ++;
                        if((UINT32)pucPtrOrg >= _u4FifoEnd)
                        {
                            pucPtrOrg = (UCHAR*)_u4FifoStart;
                        }
                        *(pucPtrOrg) = (UCHAR)(((u4Pattern << (8-j))) & 0xFF);
                        HalFlushInvalidateDCacheMultipleLine((UINT32)(pucPtrOrg), 1);
                    }
                    return TRUE;
                }
            }
            if(i == 1)
            {
                ucTmp = *pucPtr;
                pucPtr++;
                if((UINT32)pucPtr >= _u4FifoEnd)
                {
                    pucPtr = (UCHAR*)_u4FifoStart;
                }
            }
            u4Tmp <<= 1;
            u4Tmp += (UINT32)((ucTmp & 0x80) >> 7);
            ucTmp <<= 1;
        }
    }
    return FALSE;
}

INT32 _MPV_RemoveSysStartCode(UCHAR ucMpvId, UCHAR ucEsId)
{
    UCHAR *pucPtr;
    VDEC_ES_INFO_T *prVdecEsInfo;
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId); 
    if(prVdecEsInfo == NULL)
    {
        LOG(1, "%s(L:%d) prVdecEsInfo null\n", __FUNCTION__, __LINE__);
        return VDEC_ERR;
    }
    if(!_findinit(ucMpvId, ucEsId))
    {
        return VDEC_ERR;
    }
    do
    {
        pucPtr = _find16zero();
        if(pucPtr)
        {
            if(_findSysStartCode(pucPtr))
            {
                // found
                LOG(5, "%s(L:%d) _findSysStartCode found\n", __FUNCTION__, __LINE__);
                return VDEC_ERR_SYSTEM;
            }
        }
        _advance();
    }
    while(!_findend());
    return VDEC_ERR;
}

INT32 _MPV_Prepare2ndDecode(UCHAR ucMpvId, UCHAR ucEsId)
{
    MPV_ES_INFO_T* prEsInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId); 
    if(prVdecEsInfo == NULL)
    {
        LOG(1, "%s(L:%d) prVdecEsInfo null\n", __FUNCTION__, __LINE__);
        return VDEC_ERR;
    }
    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);   
    if(prEsInfo == NULL)
    {
        LOG(1, "%s(L:%d) prEsInfo null\n", __FUNCTION__, __LINE__);
        return VDEC_ERR;
    }
    
    _MPV_VldHWReset(ucEsId, ucMpvId);
    _MPV_VldSetVFifo(ucMpvId, prVdecEsInfo->u4FifoStart, prVdecEsInfo->u4FifoEnd);

#ifndef MPV_VIRTUAL_PRS
    _MPV_VldRisCtrlWptr(ucMpvId);
    _MPV_VldSetWptr(ucMpvId, prEsInfo->u4VldWptr);
    LOG(9, "%s(L:%d) _MPV_VldSetWptr 0x%x\n", prEsInfo->u4VldWptr, __FUNCTION__, __LINE__);
#endif

    if (!_MPV_VldInitBarrelShifter(ucMpvId, prVdecEsInfo->u4DecReadPtr))
    {
        return(VDEC_ERR);
    }

    if ((prEsInfo->fgLoadIntraMatrix) ||(prEsInfo->fgLoadNonIntraMatrix))
    {
        {
            if (prEsInfo->fgLoadIntraMatrix)
            {
                _MPV_VldLoadQMat(ucMpvId, prEsInfo->pu4IntraMatrix, TRUE);
            }

            if (prEsInfo->fgLoadNonIntraMatrix)
            {
                _MPV_VldLoadQMat(ucMpvId, prEsInfo->pu4NonIntraMatrix, FALSE);
            }
        }

        if (prEsInfo->fgLoadIntraMatrix)
        {
            _MPV_VldRldQMat(ucMpvId, TRUE);
        }

        if (prEsInfo->fgLoadNonIntraMatrix)
        {
            _MPV_VldRldQMat(ucMpvId, FALSE);
        }
    }

    if (!_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)SLICE_START_CODE))
    {
        return(VDEC_ERR);
    }

    return VDEC_SUCCESS;
}
#endif


INT32 _MPV_WaitDecodeProc(UCHAR ucMpvId, UCHAR ucEsId)
{
    BOOL fgNotDisplay;
    BOOL fgRet;
    UCHAR ucFbgId;
    UCHAR ucDecFbId;
    INT32 i4TempRef;
    UINT64 u8Offset;
    //UCHAR ucFRefFbId;
    //UCHAR ucBRefFbId;
    INT32 i4ReturnValue;
#ifndef LOCK_TIMEOUT
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    UINT32 u4WaitTime;
#endif
    UINT32 u4ErrStatus;
    FBM_PIC_HDR_T *prFbmPicHdr;
    //FBM_SEQ_HDR_T *prFbmSeqHdr;
    MPV_ES_INFO_T *prEsInfo;   
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;   
    VDEC_INFO_T *prVdecInfo;   
    UINT32 u4DecFin;
    UINT32 u4PTS;
    static UINT32 u4Count = 0;
    static UINT32 u4TotalCount = 0;
    PARAM_MPV_T *prMpvCounter;
    VDEC_STATUS_CB_T rStatusCB;
    UINT32 u4AddrY, u4AddrC;
    
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecInfo = _VDEC_GetInfo();
    ucFbgId = GET_FBG_ID(prVdecEsInfo);
    prMpvCounter = (PARAM_MPV_T *)prVdecEsInfo->prMpvCounter;
    prFbmPicHdr = NULL;

#ifndef LOCK_TIMEOUT
    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    ucFbgId = GET_FBG_ID(prVdecEsInfo);    
    //prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
    //ASSERT(prFbmSeqHdr != NULL);  

    u4WaitTime = 500;
    if(prEsInfo->fgAbortDecode)
    {
        u4WaitTime = 100;
    }
        
    zMsgSize = sizeof(INT32);
    UNUSED(u4Count);

    VERIFY(x_timer_start(_hMpvTimer[ucMpvId], u4WaitTime, X_TIMER_FLAG_ONCE, 
            _MpvTimerTimeout, (void*)(UINT32)ucMpvId) == OSR_OK);        
    VERIFY(x_msg_q_receive(&u2MsgQIdx, &u4DecFin, &zMsgSize, &(_hDecMsgQ[ucMpvId]), 1,
        X_MSGQ_OPTION_WAIT) == OSR_OK);
    VERIFY(x_timer_stop(_hMpvTimer[ucMpvId]) == OSR_OK);    

#else
    //VERIFY (x_sema_lock(_ahMpvDecFinSema[ucMpvId], X_SEMA_OPTION_WAIT) == OSR_OK);
    i4ReturnValue = x_sema_lock_timeout(_ahMpvDecFinSema[ucMpvId], MPV_WAIT_DEC_TIME);
#endif

   #ifdef MPV_DUMP_REG
    {
        static BOOL fgMPVDumpAfterTrigger = FALSE;
        if (fgMPVDumpAfterTrigger)
        {
            LOG(3, "fgMPVDumpAfterTrigger\n");
            _MPV_DumpReg(ucMpvId);
            LOG(3, "~fgMPVDumpAfterTrigger\n");
        }
    }
    if (0)
    {
       _MPV_DumpReg1(ucMpvId);
    }
   #endif

#ifdef CC_MPV_DEBLOCKING
    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK) && (prVdecEsInfo->ucPicType == B_TYPE))
    {
        ucDecFbId = prVdecEsInfo->ucDbkFbId;
    }
    else
#endif
    {    
        ucDecFbId = prVdecEsInfo->ucFbId;
    }
    
    prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucDecFbId);
    if(prFbmPicHdr == NULL)
    {
        ASSERT(prFbmPicHdr != NULL);
        return (VDEC_ERR);
    }

    u4ErrStatus = _ChkDecErr(ucMpvId, ucEsId);
    UNUSED(u4ErrStatus);
#ifdef MPV_REPLACE_0x1F4
    // 0x114, bit8
    if((!prEsInfo->fgNotFirstPass) &&
       (u4ErrStatus & 0x100) &&
       ((prVdecEsInfo->ucPicType == I_TYPE) || (prVdecEsInfo->ucPicType == P_TYPE))) 
    {
        LOG(6, "Es %d E S\n", ucEsId); // error type, system start code
        if(_MPV_RemoveSysStartCode(ucMpvId, ucEsId) == VDEC_ERR_SYSTEM)
        {
            return VDEC_ERR_SYSTEM;
        }
    }
#endif
           
#ifndef LOCK_TIMEOUT
    if (u4DecFin != DEC_FINISH)
#else
    //if (_fgDecTimeout[ucMpvId])    
    if (i4ReturnValue != OSR_OK)                //decode timeout
#endif
    {

        prVdecEsInfo->u4DecReadPtr = prVdecEsInfo->u4VldReadPtr;        

        MPV_EventNotify(MPV_DEC_TIMEOUT, ucEsId, ucMpvId, prFbmPicHdr->ucPicCdTp);
        prMpvCounter->u4TimeOutNs++;        

        if(prVdecInfo->pfnStatusCb)
        {
            rStatusCB.u1EsId = ucEsId;
            rStatusCB.u4Status = (UINT32)VDEC_PIC_DECODING_ERROR;
            prVdecInfo->pfnStatusCb(&rStatusCB);
        }
        LOG(1, "Es %d Decode T, PicTp: %d, Cnt: %d, \n", ucEsId, prFbmPicHdr->ucPicCdTp, prMpvCounter->u4TimeOutNs);    
        
        _MPV_DecodeTimeoutLog(ucMpvId);
        
    #if defined(MPV_SEMI_HOST) && defined(MPV_DUMP_REG)
        if(!prVdecInfo->fgIgnoreErr)
        {
            _MPV_DumpReg(ucMpvId);
        }
    #endif

        if (prVdecEsInfo->ucPicType == I_TYPE)
        {
             LOG(1, "I frame\n");
        }
        
        return (VDEC_TIMEOUT);
    }

    #ifdef MPV_DUMP_REG
    _MPV_DumpMCData(ucEsId, ucMpvId);
    #endif

#ifdef VDEC_TIME_PROFILE

    _rTimeHwDt.u4Micros = 0;

    HAL_GetTime(&_rTimeHwE);
    HAL_GetDeltaTime(&_rTimeHwDt, &_rTimeHwS, &_rTimeHwE);

		//save decode time
		prVdecEsInfo->u4CurDecMs += (_rTimeHwDt.u4Micros / 1000);
		LOG(5, "dec time = %d\n", _rTimeHwDt.u4Micros);
		u4DramCycle = IO_REG32(DRAM_BASE, 0x190);
    u4DecCycle = MC_READ32(0, 0x778);
		prVdecEsInfo->u4CurDecCycle += u4DecCycle;
		prVdecEsInfo->u4CurDramCycle +=  u4DramCycle;
		
		if ((IS_FRMPIC(prFbmPicHdr))||(IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC)))
		{
			prVdecEsInfo->u1FBufStatus = 1;
		}
		else
		{   
	         prVdecEsInfo->u1FBufStatus = 0;
		}
			
#endif

    // This is end of a video frame decode
    u4AddrY = 0;
    u4AddrC = 0;
    FBM_GetFrameBufferAddr(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucFbId
        , &u4AddrY, &u4AddrC);
    UNUSED(u4AddrY);
    UNUSED(u4AddrC);
#ifdef MPV_SEMI_HOST
    if((u4AddrY > 0) && (u4AddrC > 0) && (prVdecEsInfo->u2LineSize > 0) && (prVdecEsInfo->u2OrgVSize > 0))
    {
        HalFlushInvalidateDCacheMultipleLine(VIRTUAL(u4AddrY), prVdecEsInfo->u2LineSize*prVdecEsInfo->u2OrgVSize);
        HalFlushInvalidateDCacheMultipleLine(VIRTUAL(u4AddrC), prVdecEsInfo->u2LineSize*prVdecEsInfo->u2OrgVSize/2);
    }    
#ifdef CC_MPV_DEBLOCKING
    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK) && (prVdecEsInfo->ucPicType == B_TYPE))
    {
        FBM_GetFrameBufferAddr(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucDbkFbId
            , &u4AddrY, &u4AddrC);
        if((u4AddrY > 0) && (u4AddrC > 0) && (prVdecEsInfo->u2LineSize > 0) && (prVdecEsInfo->u2OrgVSize > 0))
        {
            HalFlushInvalidateDCacheMultipleLine(VIRTUAL(u4AddrY), prVdecEsInfo->u2LineSize*prVdecEsInfo->u2OrgVSize);
            HalFlushInvalidateDCacheMultipleLine(VIRTUAL(u4AddrC), prVdecEsInfo->u2LineSize*prVdecEsInfo->u2OrgVSize/2);
        }
    }
#endif
#endif

#ifdef VDP_RACE_LOG
    VDP_FrcRacingLog('F');
#endif

    if (prVdecEsInfoKeep->eCurState == VDEC_ST_PLAY_I_FRAME)
    {
        UNUSED(_MPV_CalcPts(ucEsId, ucDecFbId));
        
        _VDEC_JobBeforePutDispQ(ucEsId, ucDecFbId);

        FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_DISPLAYQ);        

        if (prVdecEsInfoKeep->pfDecNfyFct)
        {
            prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag, VDEC_COND_I_FRAME_DONE, 0, 0);
        }

        /*
        UNUSED(_MPV_CalcPts(ucEsId, ucDecFbId));
        FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_DISPLAYQ);        
        */

        return (VDEC_SUCCESS);        
    }

    #if 0 // move down
    if((prVdecEsInfo->ucPicType == I_TYPE) &&
      (prVdecEsInfo->fgMMPlayback) &&
      (prVdecEsInfoKeep->rPesInfo.fgSeqEnd))        //DTV00139473
    {
        UNUSED(_MPV_CalcPts(ucEsId, ucDecFbId));
        FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_DISPLAYQ);
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
        LOG(5, "fgSeqEnd with I\n");	
        return (VDEC_SUCCESS);
    }
    #endif
	
    //HalfB Racing
    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_RACING_HALFB))
    {
        i4ReturnValue = _RacingProc(ucMpvId, ucEsId, prFbmPicHdr);

        if  (i4ReturnValue != VDEC_SUCCESS)
        {
            LOG(5, "_RacingProc fail %d\n", i4ReturnValue);
            return i4ReturnValue;
        }    
    }    
    
    LOG(1, "DecFin Rptr: 0x%x  Wptr:0x%x\n", 
        _MPV_VldRead32(ucMpvId, RO_VLD_VRPTR), _MPV_VldRead32(ucMpvId, RO_VLD_VWPTR));

    if (_au4ErrCnt[ucMpvId] == 0)
    {
        prVdecEsInfo->u4DecReadPtr = _MPV_SaveReadPtr(ucMpvId);
    }
    else
    {
        prVdecEsInfo->u4DecReadPtr = prVdecEsInfo->u4VldReadPtr;
    }

    //If Error count > error thrsd, drop the picture. Not drop now.
    if (_au4ErrCnt[ucMpvId] > prEsInfo->u4ErrThrsd)
    {    
        prMpvCounter->u4ErrDropNs++;
        return (VDEC_ERR);
    }

    #ifdef CC_MPV_DEBLOCKING
    _MPV_HandleDbk(ucMpvId, ucFbgId, ucDecFbId, ucEsId, prFbmPicHdr, _au4ErrCnt[ucMpvId]);
    #endif

    //Detect all Intra slices
    if ((prEsInfo->fgDetectIntra) && (!prVdecEsInfo->fgAllIntraSlice))
    {
        i4ReturnValue = _MPV_DetectIntra(ucMpvId, ucEsId, prEsInfo);

        if  (i4ReturnValue != VDEC_SUCCESS)
        {
            LOG(5, "_MPV_DetectIntra fail %d\n", i4ReturnValue);        
            return i4ReturnValue;
        }            
    }

#if defined(MULTIMEDIA_AUTOTEST_SUPPORT) || defined(VDEC_CRC_TEST)
#if defined(VDEC_CRC_TEST) && !defined(MULTIMEDIA_AUTOTEST_SUPPORT)
    if(VDEC_IsTestCRC(ucEsId))
#endif
    {
        if (IS_FRMPIC(prFbmPicHdr))
        {
        UINT32 CurYcrc[4] = {0};
        UINT32 CurCcrc[4] = {0};
        UINT8 i;
        _MPV_Read_Crc(ucEsId, ucMpvId, (UINT32*)CurYcrc, (UINT32*)CurCcrc);

        for(i=0; i<4; i++)
        {
          prVdecEsInfo->u4CrcResult ^=CurYcrc[i];
          prVdecEsInfo->u4CrcResult ^=CurCcrc[i];
        }

#if 0
        if(prVdecEsInfo->fgAutoCrcCk)
        {   
            rVdecEsInfo->fgAutoCrcCk = FALSE;
            if((CurYcrc[0]== prVdecEsInfo->u4YCrcGolden)&&(CurCcrc[0]== prVdecEsInfo->u4CCrcGolden))
            {   
                u4Result =1;
            }
            else 
            {
                LOG(0,"crc check fail, Ycrc(%d), Ccrc(%d), Ygold(%d), Cgold(%d)\n",
                CurYcrc[0],CurCcrc[0],prVdecEsInfo->u4YCrcGolden,prVdecEsInfo->u4CCrcGolden);
            }
            //callback, non this format data
            if (prVdecEsInfoKeep->pfDecAutoTestNfyFct)
            {
                LOG(3, "Notify Auto test Crc result\n");
                prVdecEsInfoKeep->pfDecAutoTestNfyFct(prVdecEsInfoKeep->pvDecAutoTestNfyTag,
                VDEC_COND_CRC_CHECK_DONE, 2, u4Result);
            }
        }
#endif 
        }
        else
        {
            if(IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC)) 
            {
                UINT32 CurYcrc[4] = {0};
                UINT32 CurCcrc[4] = {0};
                UINT8 i;

                _MPV_Read_Crc(ucEsId, ucMpvId, (UINT32*)CurYcrc, (UINT32*)CurCcrc);

                for(i=0; i<4; i++)
                {
                   prVdecEsInfo->u4CrcResult ^=CurYcrc[i];
                   prVdecEsInfo->u4CrcResult ^=CurCcrc[i];
                }
#if 0	
                if(prVdecEsInfo->fgAutoCrcCk)
                {   
                    prVdecEsInfo->fgAutoCrcCk = FALSE;
                    if((CurYcrc[0]== prVdecEsInfo->u4YCrcGolden)&&(CurCcrc[0]== prVdecEsInfo->u4CCrcGolden))
                    {
                        u4Result =1;
                    }
                    else 
                    {
                        LOG(0,"crc check fail, Ycrc(%d), Ccrc(%d), Ygold(%d), Cgold(%d)\n",
                        CurYcrc[0],CurCcrc[0],prVdecEsInfo->u4YCrcGolden,prVdecEsInfo->u4CCrcGolden);
                    }
                     //callback, non this format data
                     if (prVdecEsInfoKeep->pfDecAutoTestNfyFct)
                     {
                        LOG(3, "Notify Auto test Crc result\n");
                        prVdecEsInfoKeep->pfDecAutoTestNfyFct(prVdecEsInfoKeep->pvDecAutoTestNfyTag,
                        VDEC_COND_CRC_CHECK_DONE, 2, u4Result);
                     }

                }
#endif
            }
        }
    }
#endif 

#ifdef MPV_SEMI_HOST
    #ifdef CC_MPV_DEBLOCKING
     if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK))
     {
         ucDecFbId = prVdecEsInfo->ucDbkFbId;
     }
    #endif    

    if (IS_FRMPIC(prFbmPicHdr))
    {
        if (prVdecInfo->fgSave)
        {
            _MPV_SavePic(ucFbgId, ucDecFbId);
        }
        else if(prVdecInfo->fgCompare)
        {
            _MPV_CompareGold(ucFbgId, ucDecFbId);  
        }
        LOG(5, "Pic %d, %d Decoded\n", u4Count, u4TotalCount);
        u4TotalCount++;
        if (++u4Count == prVdecEsInfo->u4CompareNs)
        {
            u4Count = 0;
        }
    }
    else
    {
        if(IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC)) 
        {
            if (prVdecInfo->fgSave)
            {        
                _MPV_SavePic(ucFbgId, ucDecFbId);        
            }
            else if(prVdecInfo->fgCompare)
            {
                _MPV_CompareGold(ucFbgId, ucDecFbId);        
            }
            LOG(5, "Pic/Fld %d, %d Decoded\n", u4Count, u4TotalCount);
            u4TotalCount++;
            if (++u4Count == prVdecEsInfo->u4CompareNs)
            {
                u4Count = 0;
            }
        }
        else
        {
            // CRC have top and bottem field data
            if(_fgMpvEnCrc)
            {
                if (prVdecInfo->fgSave)
                {        
                    _MPV_SavePic(ucFbgId, ucDecFbId);        
                }
                else if(prVdecInfo->fgCompare)
                {
                    _MPV_CompareGold(ucFbgId, ucDecFbId);        
                }
                LOG(5, "Fld %d, %d Decoded\n", u4Count, u4TotalCount);
            }
        }
    }

    #ifdef CC_MPV_DEBLOCKING
     if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK) && (prVdecEsInfo->ucPicType != B_TYPE))
     {
         ucDecFbId = prVdecEsInfo->ucFbId;
     }
    #endif
#endif

    
    //Change FB status
    if (IS_FRMPIC(prFbmPicHdr))
    {
        if (IS_REFPIC(prFbmPicHdr)) 
        {
            // for rendering thumbanil, we have to call CheckStopStatus
            // to check deliver eos.
            if((prVdecEsInfo->ucPicType == I_TYPE) &&
              (prVdecEsInfo->fgMMPlayback) &&
              (prVdecEsInfoKeep->rPesInfo.fgSeqEnd))        //DTV00139473
            {
                UCHAR ucDispFbId;
                #ifdef CC_MPV_DEBLOCKING
                //if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK))                       //DTV00215561  DEC_FLAG_DBK is set, but _MPV_HandleDbk() judge it no
                if ((IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK))&&(!prVdecEsInfo->fgRefNoDbk))   //needed to dbk, so 
                {
                    FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_READY);    
                    ucDispFbId = prVdecEsInfo->ucDbkFbId;
                }
                else            
                #endif
                {
                    ucDispFbId = ucDecFbId;
                }
                u4PTS = _MPV_CalcPts(ucEsId, ucDecFbId);
                prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucDispFbId);
                if(prFbmPicHdr)
                {
                    i4TempRef = prFbmPicHdr->i4TemporalRef;
                    u8Offset = prFbmPicHdr->u8Offset;
                }
                else
                {
                    i4TempRef = 0;
                    u8Offset = 0;
                    LOG(3, "_MPV_WaitDecodeProc 1 prFbmPicHdr NULL");
                }
                fgNotDisplay = VDEC_IsNotDisplay(ucEsId, u4PTS, i4TempRef, u8Offset);

                if ((IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I)) ||
                    (fgNotDisplay))
                {
                    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId,
                        ucDispFbId, FBM_FB_STATUS_EMPTY);
                }
                else
                {
                    //DTV00215347 for some specail ts file, for unkown reason can't set frame status to display
                    #if 0
                    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId,
                        ucDispFbId, FBM_FB_STATUS_DISPLAYQ);
                    prVdecEsInfo->u4DisplayQPicCnt++;
					#endif
					
				    UCHAR ucFbStatus;
                    ucFbStatus = FBM_GetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucDispFbId);
                    if ((ucFbStatus != FBM_FB_STATUS_DISPLAYQ) ||
                        (ucFbStatus != FBM_FB_STATUS_LOCK))
                    {
                        #ifdef CC_REALD_3D_SUPPORT
                        _VDEC_Check2D3DSwitch(ucEsId, ucDispFbId);
                        #endif
                        
                        _VDEC_JobBeforePutDispQ(ucEsId, ucDispFbId);
                        
                        _MPV_CheckVenc(ucEsId, ucDispFbId);

						FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucDispFbId, FBM_FB_STATUS_DISPLAYQ);
						prVdecEsInfo->u4DisplayQPicCnt++;

                    }
					else
					{
                        LOG(5,"can't set frame status to display current status is %d",ucFbStatus);
                    }
                }
                fgRet = VDEC_CheckStopStatus(ucEsId, u4PTS, i4TempRef);
                // now remove wait next I when seq end
                #if 0
                SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
                #endif
                LOG(5, "fgSeqEnd with I\n");
                return (VDEC_SUCCESS);
            }
            // this is MHP case.
            else if((prVdecEsInfo->fgMMPlayback) &&
               (prVdecEsInfoKeep->rPesInfo.fgNonBuffer))
            {
                UCHAR ucDispFbId;
                #ifdef CC_MPV_DEBLOCKING
                if ((IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK))&&(!prVdecEsInfo->fgRefNoDbk))
                {
                    FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_READY);    
                    ucDispFbId = prVdecEsInfo->ucDbkFbId;
                }
                else            
                #endif
                {
                    ucDispFbId = ucDecFbId;
                }
                u4PTS = _MPV_CalcPts(ucEsId, ucDecFbId);
                prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucDispFbId);
                if(prFbmPicHdr)
                {
                    i4TempRef = prFbmPicHdr->i4TemporalRef;
                    u8Offset = prFbmPicHdr->u8Offset;
                }
                else
                {
                    i4TempRef = 0;
                    u8Offset = 0;
                    LOG(3, "_MPV_WaitDecodeProc 1 prFbmPicHdr NULL");
                }
                fgNotDisplay = VDEC_IsNotDisplay(ucEsId, u4PTS, i4TempRef, u8Offset);

                if (fgNotDisplay)
                {
                    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId,
                        ucDispFbId, FBM_FB_STATUS_EMPTY);
                }
                else
                {					
				    UCHAR ucFbStatus;
                    ucFbStatus = FBM_GetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucDispFbId);
                    if ((ucFbStatus != FBM_FB_STATUS_DISPLAYQ) ||
                        (ucFbStatus != FBM_FB_STATUS_LOCK))
                    {
                        #ifdef CC_REALD_3D_SUPPORT
                        _VDEC_Check2D3DSwitch(ucEsId, ucDispFbId);
                        #endif
                        
                        _VDEC_JobBeforePutDispQ(ucEsId, ucDispFbId);
                        
                        _MPV_CheckVenc(ucEsId, ucDispFbId);

						FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucDispFbId, FBM_FB_STATUS_DISPLAYQ);
						prVdecEsInfo->u4DisplayQPicCnt++;

                    }
					else
					{
                        LOG(5,"can't set frame status to display current status is %d",ucFbStatus);
					}
                }
                fgRet = VDEC_CheckStopStatus(ucEsId, u4PTS, i4TempRef);
                
                return (VDEC_SUCCESS);
            }
            else
            {
            #ifdef CC_MPV_DEBLOCKING
                if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK))
                {
                    FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_READY);    
                    FBM_SetFrameBufferStatus(ucFbgId, prVdecEsInfo->ucDbkFbId, FBM_FB_STATUS_READY);                    
                }
                else            
            #endif
                {            
                    //I,P
                    FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_READY);
                }
            }
        }
        else 
        {            
            //B
            if (!IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_RACING_FULLB|DEC_FLAG_RACING_HALFB|DEC_FLAG_TRACKING_MODE))
            {
                u4PTS = _MPV_CalcPts(ucEsId, ucDecFbId);
                prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucDecFbId);                
                if(prFbmPicHdr)
                {
                    i4TempRef = prFbmPicHdr->i4TemporalRef;
                    u8Offset = prFbmPicHdr->u8Offset;
                }
                else
                {
                    i4TempRef = 0;
                    u8Offset = 0;
                    LOG(6, "_MPV_WaitDecodeProc 1 fgNotDisplay prFbmPicHdr NULL");
                }
                fgNotDisplay = VDEC_IsNotDisplay(ucEsId, u4PTS, i4TempRef, u8Offset);
                
                LOG_MFBM("MFBM Set B Disp a, %d, %d\n", ucDecFbId,
                    (FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucDecFbId))->i4TemporalRef);
                
                if (fgNotDisplay)
                {
                    FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_EMPTY);
					LOG(1,"B Frame Not To Display\n");
                }
                else
                {
                    #if 0
                    FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_DISPLAYQ);
                    prVdecEsInfo->u4DisplayQPicCnt++;
					#endif 
				    UCHAR ucFbStatus;
					ucFbStatus = FBM_GetFrameBufferStatus(ucFbgId, ucDecFbId);
                    if ((ucFbStatus != FBM_FB_STATUS_DISPLAYQ) ||
                        (ucFbStatus != FBM_FB_STATUS_LOCK))
                    {
                        #ifdef CC_REALD_3D_SUPPORT
                        _VDEC_Check2D3DSwitch(ucEsId, ucDecFbId);
                        #endif
                        
                        _VDEC_JobBeforePutDispQ(ucEsId, ucDecFbId);
                        
                        _MPV_CheckVenc(ucEsId, ucDecFbId);

						 FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_DISPLAYQ);
						 prVdecEsInfo->u4DisplayQPicCnt++;

						 LOG(1,"B Frame To Q, PTS = %x\n",u4PTS);

                    }
					else
					{
                         LOG(5,"can't set frame status to display current status is %d",ucFbStatus);
					}
				}
                
                fgRet = VDEC_CheckStopStatus(ucEsId, u4PTS, i4TempRef);
                UNUSED(fgRet);
            }                
        }
    }
    //field
    else 
    {
        if (IS_REFPIC(prFbmPicHdr))
        {        
            if (IS_DECFLAG_SET(prVdecEsInfo,DEC_FLG_2ND_FLD_PIC)) 
            {
        #ifdef CC_MPV_DEBLOCKING
                if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK))
                {
                    FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_READY);    
                    FBM_SetFrameBufferStatus(ucFbgId, prVdecEsInfo->ucDbkFbId, FBM_FB_STATUS_READY);                        
                }
                else            
        #endif
                {
                    FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_READY);
                }
            }
            else
            {
        #ifdef CC_MPV_DEBLOCKING
                if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK))
                {
                    FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_FIELD_READY);    
                    FBM_SetFrameBufferStatus(ucFbgId, prVdecEsInfo->ucDbkFbId, FBM_FB_STATUS_FIELD_READY);                        
                }
                else            
        #endif
                {
                    FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_FIELD_READY); 
                }
            }  
        }  
        else
        {
            if (!IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_RACING_FULLB|DEC_FLAG_RACING_HALFB|DEC_FLAG_TRACKING_MODE))
            {
                //B
                if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC)) 
                {
                    u4PTS = _MPV_CalcPts(ucEsId, ucDecFbId);
                    prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucDecFbId);
                    if(prFbmPicHdr)
                    {
                        i4TempRef = prFbmPicHdr->i4TemporalRef;
                        u8Offset = prFbmPicHdr->u8Offset;
                    }
                    else
                    {
                        i4TempRef = 0;
                        u8Offset = 0;
                        LOG(6, "_MPV_WaitDecodeProc 2 fgNotDisplay prFbmPicHdr NULL");
                    }
                    fgNotDisplay = VDEC_IsNotDisplay(ucEsId, u4PTS, i4TempRef, u8Offset);
                    
                    LOG_MFBM("MFBM Set B Disp b, %d, %d\n", ucDecFbId,
                    	(FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucDecFbId))->i4TemporalRef);
                    
                    if (fgNotDisplay)
                    {
                        FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_EMPTY);
						LOG(1,"B Frame Not To Display\n");
                    }
                    else
                    {
                        #if 0
                        FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_DISPLAYQ);
                        prVdecEsInfo->u4DisplayQPicCnt++;
						#endif 
					    UCHAR ucFbStatus;
						ucFbStatus = FBM_GetFrameBufferStatus(ucFbgId, ucDecFbId);
                        if ((ucFbStatus != FBM_FB_STATUS_DISPLAYQ) ||
                        (ucFbStatus != FBM_FB_STATUS_LOCK))
                        {
                            #ifdef CC_REALD_3D_SUPPORT
                            _VDEC_Check2D3DSwitch(ucEsId, ucDecFbId);
                            #endif
                        
                            _VDEC_JobBeforePutDispQ(ucEsId, ucDecFbId);
                        
                            _MPV_CheckVenc(ucEsId, ucDecFbId); 
                            
						   FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_DISPLAYQ);
						   prVdecEsInfo->u4DisplayQPicCnt++;
						   LOG(1,"B Frame To Q, PTS = %x\n",u4PTS);

                        }
					    else
					    {
                            LOG(5,"can't set frame status to display current status is %d",ucFbStatus);
					    }
				    }
                    fgRet = VDEC_CheckStopStatus(ucEsId, u4PTS, i4TempRef);
                    UNUSED(fgRet);    
                }            
                else
                {        
                    FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_FIELD_READY);                            
                }
            }
        }  
        TOGGLE_DECFLAG(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC);
    }    

    if (prMpvCounter->u4DecOkNs == 0)
    {    
        HAL_TIME_T rTime;
        x_memset(&rTime, 0, sizeof(HAL_TIME_T));
        HAL_GetTime(&rTime);
        prMpvCounter->rDecFirstTime = rTime;
        LOG(3, "Vld(%d) Es(%d) Decode First Pic. Time(%d.%06d) PTS(0x%x)\n",
            ucMpvId, ucEsId, rTime.u4Seconds, rTime.u4Micros, prVdecEsInfo->u4PTS);

    #ifdef TIME_MEASURE
        {
            HAL_TIME_T rResult;
            CRIT_STATE_T _rCritState;
            static DRV_DBASE_T* _prDbase = NULL;

            _prDbase = DBS_Lock(&_rCritState);
            ASSERT(_prDbase);
            VERIFY(DBS_Unlock(_prDbase, _rCritState));

            HAL_GetDeltaTime(&rResult, &(_prDbase->rDmx.rEnableTime), &(prMpvCounter->rDecFirstTime));

            LOG(7, "Mpv Time Measure (%d.%06d)\n", rResult.u4Seconds, rResult.u4Micros);
        }
    #endif
        
    }

    prMpvCounter->u4DecOkNs++;

#ifdef MPV_STATISTICS
    _CalDecTime(ucMpvId, ucEsId); 
#endif

    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK) && (prVdecEsInfo->ucPicType == B_TYPE))
    {
        _CalBDecTime(ucMpvId, ucEsId); 
    }
    
    UNUSED(u4TotalCount);
    

    return (VDEC_SUCCESS);
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

void _MPV_CreateDecSema(UCHAR ucMpvId)
{    
#ifndef LOCK_TIMEOUT
    CHAR szBuf[16];
#endif
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }

#ifndef LOCK_TIMEOUT
    x_snprintf(szBuf, sizeof(szBuf), "Dec-Q%d", ucMpvId);
    VERIFY(x_msg_q_create(&_hDecMsgQ[ucMpvId], szBuf, sizeof(UINT32), 2) == OSR_OK);        
    VERIFY(x_timer_create(&_hMpvTimer[ucMpvId]) == OSR_OK);
#else
    VERIFY (x_sema_create(&_ahMpvDecFinSema[ucMpvId], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
#endif
    
}


void _MPV_DeleteDecSema(UCHAR ucMpvId)
{    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
#ifndef LOCK_TIMEOUT
    VERIFY(x_msg_q_delete(_hDecMsgQ[ucMpvId]) == OSR_OK);        
    VERIFY(x_timer_delete(_hMpvTimer[ucMpvId]) == OSR_OK);
#else
    VERIFY (x_sema_delete(_ahMpvDecFinSema[ucMpvId]) == OSR_OK);
#endif
    
}


void _MPV_DecInit(void)
{
    UCHAR ucIdx;

    for (ucIdx = 0; ucIdx < MPV_MAX_VLD; ucIdx++)
    {
        _MPV_VParserInit(ucIdx);
        //_MPV_VldHWReset(ucIdx);                
    }

    if (BSP_IsFPGA())
    {
        _MPV_GetHwVer();
    }
    
    for (ucIdx = 0; ucIdx < MPV_MAX_ES; ucIdx++)
    {
        _MPV_ClearEsInfo(ucIdx);             
    }

#ifdef MPV_SEMI_HOST 
    _pYaddr = x_mem_alloc(1920*1088*2);
    _pCaddr = x_mem_alloc((1920*1088*2) >> 1);
    ASSERT(_pYaddr != NULL);
    ASSERT(_pCaddr != NULL);    
#endif

#ifdef MPV_STATISTICS
    _MPV_ResetTime();
    VERIFY(HAL_InitTimer());
#endif
}


void _MPV_DecReset(UCHAR ucMpvId, UCHAR ucEsId)
{
    MPV_ES_INFO_T *prEsInfo;

    VDEC_ES_INFO_T *prVdecEsInfo;
    
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);        

#ifdef TEST_ONOFF_RUNTIME
    _VDEC_LockVld(ucEsId, ucMpvId);
#endif

    _MPV_VldHWReset(ucEsId, ucMpvId);
    _MPV_VldSetVFifo(ucMpvId, prVdecEsInfo->u4FifoStart, prVdecEsInfo->u4FifoEnd);

    #ifndef MPV_VIRTUAL_PRS
        _MPV_VldRisCtrlWptr(ucMpvId);
        _MPV_VldSetWptr(ucMpvId, prEsInfo->u4VldWptr);
        LOG(9, "_InitHw _MPV_VLDSetWptr 0x%x 0x%x\n", prEsInfo->u4VldWptr, 
            _MPV_VldRead32(ucMpvId, RO_VLD_VWPTR));
    #endif
    
    // Restore Intra Quantization Matrix
    if(prEsInfo->fgLoadIntraMatrix)
    {
        LOG(10, "fgLoadIntraMatrix\n");
        _MPV_VldRldQMat(ucMpvId, TRUE);
    }

    // Restore NonIntra Quantization Matrix
    if(prEsInfo->fgLoadNonIntraMatrix)
    {
        LOG(10, "fgLoadNonIntraMatrix\n");
        _MPV_VldRldQMat(ucMpvId, FALSE);
    }

#ifndef LOCK_TIMEOUT
    _MpvFlushDecMsgQ  (ucMpvId);    
#endif

#ifdef TEST_ONOFF_RUNTIME
    _VDEC_UnlockVld(ucEsId, ucMpvId);
#endif
}


INT32 _MPV_Decode(UCHAR ucMpvId, UCHAR ucEsId)
{    
    BOOL fgStatus = TRUE;
    BOOL fgRetry = FALSE;
    UINT32 u4Times = 0;
	INT32 i4Ret;
    MPV_ES_INFO_T *prEsInfo;
//    MPV_CC_T    *prMpvCc;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
    _MPV_DecReset(ucMpvId, ucEsId);

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);

    //if (fgStatus)
    {
        i4Ret = _MPV_VParser(ucMpvId, ucEsId);
        if(i4Ret!= VDEC_SUCCESS)
        {
            LOG(1, "_MPV_VParser fail\n");
            fgStatus = FALSE;

			if((i4Ret== P_REFERNCE_INVALID)||(i4Ret== B_REFERNCE_INVALID))    // DTV00214416  invalid reference frame don't needed notify error
			{
			    LOG(1, "fail: reference frame invalid call _MPV_DropPic \n");
				_MPV_DropPic(ucEsId);
				return (0);

			}
        }
    }

    prEsInfo->fgNotFirstPass = FALSE;
    do
    {
        fgRetry = FALSE;
#ifdef MPV_REPLACE_0x1F4
        if(u4Times) // not first pass
        {
            LOG(6, "%d pass\n", u4Times);
            prEsInfo->fgNotFirstPass = TRUE;
            if(_MPV_Prepare2ndDecode(ucMpvId, ucEsId) != VDEC_SUCCESS)
            {
                LOG(7, "_MPV_Prepare2ndDecode fail\n");
                fgStatus = FALSE;
            }
        }
#endif
    if (fgStatus)
    {
        if(_MPV_DecodeProc(ucMpvId, ucEsId) != VDEC_SUCCESS)
        {
            LOG(1, "_MPV_DecodeProc fail\n");
            fgStatus = FALSE;
        }
    }
    
    if (fgStatus)
    {
            i4Ret = _MPV_WaitDecodeProc(ucMpvId, ucEsId);
            if(i4Ret != VDEC_SUCCESS)
            {
#ifdef MPV_REPLACE_0x1F4
                if(i4Ret == VDEC_ERR_SYSTEM)
                {
                    LOG(6, "SysStartCode in slice picture\n");
                    fgRetry = TRUE;
                }
                else
#endif
        {
            LOG(1, "_MPV_WaitDecodeProc fail\n");
            fgStatus = FALSE;
        }
    }
        }
        u4Times++;
    }
    while((u4Times < MPV_RETRY_TIMES) && (fgRetry));
    prEsInfo->fgNotFirstPass = FALSE;

    if(!fgStatus) // drop pic
    {
        LOG(1, "Call _MPV_DropPic fail\n");
        _MPV_DropPic(ucEsId);
        _VdecCheckStatus(ucEsId, FALSE);
        return (0);
    }
    else
    {
        _VdecCheckStatus(ucEsId, TRUE);
    }

    return (1);
}


void _MPV_FinDec(UCHAR ucMpvId)
{
#ifndef LOCK_TIMEOUT
    UINT32 u4DecFin = 0;
    INT32 i4Ret;
#endif
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
#ifndef LOCK_TIMEOUT    
    u4DecFin = DEC_FINISH;
    i4Ret = x_msg_q_send(_hDecMsgQ[ucMpvId], (void *)&u4DecFin, sizeof(UINT32), 255);
  
    if ((i4Ret != OSR_OK)&&(i4Ret != OSR_TOO_MANY))
    {
        ASSERT(0);
    }
#else
    VERIFY (x_sema_unlock(_ahMpvDecFinSema[ucMpvId]) == OSR_OK);
#endif
}


void _MPV_DropPic(UCHAR ucEsId)
{
    //MPV_ES_INFO_T *prEsInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    UCHAR ucFbgId, ucDecFbId;
	
    INT32 i4TempRef;
    UINT32 u4PTS=0;
    FBM_PIC_HDR_T *prFbmPicHdr;
	
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    
    //prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    ucFbgId = GET_FBG_ID(prVdecEsInfo);
    ucDecFbId = FBM_GetDecFrameBuffer(ucFbgId);
    prFbmPicHdr = NULL;
    if((prVdecEsInfo->ucFbgId != FBM_FBG_ID_UNKNOWN) &&
       (ucDecFbId != FBM_FB_ID_UNKNOWN))
    {
        prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucDecFbId);
    }

    if (ucDecFbId != FBM_FB_ID_UNKNOWN)
    {
        if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK))
        {
            //ilii
            //if(prVdecEsInfo->ucPicType == I_TYPE || prVdecEsInfo->ucPicType == P_TYPE)
            {
                LOG_MFBM("MFBM Set Empty a (Drop IP:%d)\n", ucDecFbId);   
                FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_EMPTY);
            }
            LOG_MFBM("MFBM Set Empty b (Drop:%d)\n", prVdecEsInfo->ucDbkFbId);
            u4PTS=_MPV_CalcPts(ucEsId, prVdecEsInfo->ucDbkFbId);
            FBM_SetFrameBufferStatus(ucFbgId, prVdecEsInfo->ucDbkFbId, FBM_FB_STATUS_EMPTY);
        }
        else
        {
            LOG_MFBM("MFBM Set Empty c (Drop:%d)\n", ucDecFbId);  
            u4PTS=_MPV_CalcPts(ucEsId, ucDecFbId);
            FBM_SetFrameBufferStatus(ucFbgId, ucDecFbId, FBM_FB_STATUS_EMPTY);
        }  
        //DTV00211808  for droped B frame also needed  to check wether meet the render conditions or not.
        // threre is a case do 2X after step, if the last display frame is a B frame, when change to 2X.
        // the render from position condition can't meet.
        if(prVdecEsInfo->ucPicType == B_TYPE)
        {
            if((prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_FORWARD_1X) ||
               ((prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_FORWARD_2X)
#ifdef VDEC_TIME_SHIFT_2X_I_MODE
                &&
                (prVdecEsInfo->eContainerType != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
#endif
               )
               ||
               ((prVdecEsInfo->u4MMSpeed >= (UINT32)STC_SPEED_TYPE_FORWARD_1_DIV_2X)
                &&
                (prVdecEsInfo->u4MMSpeed <= (UINT32)STC_SPEED_TYPE_FORWARD_1_DOT_5X)))
            {
                if(prFbmPicHdr!=NULL)
                {
                    i4TempRef=prFbmPicHdr->i4TemporalRef;
                    UNUSED(VDEC_CheckStopStatus(ucEsId, u4PTS, i4TempRef));
                }
            }
        }
    }

 
	
    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC))
    {
        TOGGLE_DECFLAG(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC);
    }


    LOG(1, "Es%d Dec D Pic\n", ucEsId);
    if (prVdecEsInfo->fgIFrameDec)
    {    
        if (prVdecEsInfoKeep->pfDecNfyFct)
        {
            prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag, VDEC_COND_ERROR, 0, 0);                
        }
    }

#ifdef MPV_138184
    {
        VDEC_INFO_T *prVdecInfo;
        prVdecInfo = _VDEC_GetInfo();
        if(!prVdecInfo->fgIgnoreErr)
        {
            ASSERT(0);
        }
    }
#endif

}

void _MPV_DecTimeout(UCHAR ucMpvId, BOOL fgTimeout)
{
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
    _fgDecTimeout[ucMpvId] = fgTimeout;
    UNUSED(_fgDecTimeout[ucMpvId]);
}


#ifdef MPV_SEMI_HOST    
void _MPV_SetFilePath(UINT32 u4Flag, char *szFilePath)
{
//    MPV_ES_INFO_T *prEsInfo;
    VDEC_INFO_T *prVdecInfo;

    //prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ES0);
    prVdecInfo = _VDEC_GetInfo();

    if (u4Flag & MPV_SEMI_FLAG_IGNORE_ERR)
    {
        prVdecInfo->fgIgnoreErr = TRUE;
    }
    if (u4Flag & MPV_SEMI_FLAG_SAVE)
    {
        prVdecInfo->fgSave = TRUE;
        x_strcpy(_szSaveFilePath, szFilePath);
    }
    else
    {
        prVdecInfo->fgCompare = TRUE;    
        x_strcpy(_szGoldFilePath, szFilePath);        
    }
}


#define BLOCK_W 64
#define BLOCK_H 32

#define GOLD_BLOCK_W 16
#define GOLD_BLOCK_H 32

void _MPV_CompareGold(UCHAR ucFbgId, UCHAR ucFbId)
{
    char szFileName[256];
    //static UINT32 u4Count = 0;
    UINT32 u4AddrY, u4AddrC;
    UINT32 u4ByteSize = 0;
    UINT32 u4Idx;
    UINT32 u4BlockSize, u4BlockNum, u4BlockPerLine;
    UINT32 u4BlockStartX, u4BlockStartY, u4BlockCoordX, u4BlockCoordY;
    FBM_SEQ_HDR_T *prFbmSeqHdr;
    MPV_ES_INFO_T *prEsInfo;
    UINT32* pu4CS;
    UINT32 u4CS1 = 0, u4CS2 = 0;
    UINT32 u4CS3 = 0, u4CS4 = 0;
    UINT32 u4GoldenCSY1 = 0, u4GoldenCSY2 = 0;
    UINT32 u4GoldenCSY3 = 0, u4GoldenCSY4 = 0;
    UINT32 u4GoldenCSC1 = 0, u4GoldenCSC2 = 0;
    UINT32 u4GoldenCSC3 = 0, u4GoldenCSC4 = 0;
    BOOL fg16x32 = FALSE;
    UINT32 u4Path = 0;
    
    FBM_PIC_HDR_T *prFbmPicHdr;
    UINT32 u4FrmFld = 0;
    
    VDEC_ES_INFO_T *prVdecEsInfo;
    prVdecEsInfo = _VDEC_GetEsInfo(ES0);
    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ES0);
        
    u4Path = x_strlen(_szGoldFilePath);
    fg16x32 = !x_strcmp(_szGoldFilePath+u4Path-6, "16x32_");

    prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucFbId);
    if(prFbmPicHdr)
    {
        if (IS_FRMPIC(prFbmPicHdr))
        {
            u4FrmFld = 0;
        }
        else
        {
            // top field u4FrmFld = 1
            // bottom field u4FrmFld = 2
            if(IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC))
            {
                if(prFbmPicHdr->fgTopFldFirst) 
                {
                    u4FrmFld = 2;
                }
                else
                {
                    u4FrmFld = 1;
                }
            }
            else
            {
                if(prFbmPicHdr->fgTopFldFirst) 
                {
                    u4FrmFld = 1;
                }
                else
                {
                    u4FrmFld = 2;
                }
            }
        }
    }
    
    if (((_u4PicCount >= prEsInfo->u4StartSaveNo) && (_u4PicCount <= prEsInfo->u4EndSaveNo)) ||
        ((prEsInfo->u4StartSaveNo == 0) && (prEsInfo->u4EndSaveNo == 0)))
    {
        FBM_GetFrameBufferAddr(ucFbgId, ucFbId, &u4AddrY, &u4AddrC);
        LOG(7, "u4AddrY = 0x%x,  u4AddrC = 0x%x\n", u4AddrY, u4AddrC);

        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
        ASSERT(prFbmSeqHdr != NULL);

        if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_REDUCE_REF))        
        {
            u4ByteSize = (prVdecEsInfo->u2OrgHSize * prVdecEsInfo->u2OrgVSize) >> 1;

            #ifdef MPV_REPEAT_LOAD_GOLDEN
            if(prEsInfo->fgDecodeOnRepeat == 1)
            {
            	// do nothing
            }
            else
            #endif
            {
                x_sprintf((CHAR *)szFileName, "%s%03d.bitY", _szGoldFilePath, _u4PicCount);            
                UNUSED(T32_HostLoadData(szFileName, _pYaddr));
                x_sprintf((CHAR *)szFileName, "%s%03d.bitC", _szGoldFilePath, _u4PicCount);            
                UNUSED(T32_HostLoadData(szFileName, _pCaddr));
                //#if defined(CC_MT5365) || defined(CC_MT5395)
                #if defined(CC_MT5365) || defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)
                HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)_pYaddr), u4ByteSize);
                HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)_pCaddr), u4ByteSize>>1);
                #else
                HalFlushInvalidateDCache();
                #endif
            }

            for (u4Idx = 0; u4Idx < u4ByteSize; u4Idx++)
            {
                if (*(UCHAR*)(u4AddrY + u4Idx) != _pYaddr[u4Idx])
                {
                    LOG(1, "BYTE 0x%x (%d, %d), Dec: %02x, Ref: %02x, Diff: %3d\n", 
                        u4Idx, (u4BlockStartX + u4BlockCoordX), (u4BlockStartY + u4BlockCoordY), 
                      *(UCHAR*)(u4AddrY + u4Idx), _pYaddr[u4Idx], *(UCHAR*)(u4AddrY + u4Idx) - _pYaddr[u4Idx]); 

                    #ifdef MPV_DUMP_REG
                    _MPV_DumpReg(ES0);
                    #endif
                    ASSERT(0);
                    //break;   
                }
            }
            LOG(1, "Y Ok!\n");

            for (u4Idx = 0; u4Idx < (u4ByteSize >> 1); u4Idx++)
            {

                if (*(UCHAR*)(u4AddrC + u4Idx) != _pCaddr[u4Idx])
                {
                    LOG(1, "BYTE 0x%x, Dec: %02x, Ref: %02x, Diff: %3d\n", 
                    u4Idx, *(UCHAR*)(u4AddrC + u4Idx), _pCaddr[u4Idx], *(UCHAR*)(u4AddrC + u4Idx) - _pCaddr[u4Idx]);
                    
                    #ifdef MPV_DUMP_REG
                    _MPV_DumpReg(ES0);
                    #endif
                    ASSERT(0);
                    //break;
                }
            }
        }
        else
        {
            u4ByteSize = (prVdecEsInfo->u2LineSize) * (((prVdecEsInfo->u2VSize)+0x1f)&(~0x1f));      
	    
            #ifdef MPV_REPEAT_LOAD_GOLDEN
            if(prEsInfo->fgDecodeOnRepeat == 1)
 	        {
                // do nothing
            }
            else
            #endif
            {
                if(_fgMpvEnChkSum)
                {
                    x_sprintf((CHAR *)szFileName, "%s%d.cs", _szGoldFilePath, _u4PicCount); 
                    UNUSED(T32_HostLoadData(szFileName, _pYaddr));
                    #if defined(CC_MT5365) || defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)
                    HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)_pYaddr), 16);
                    #else
                    HalFlushInvalidateDCache();
                    #endif
                    pu4CS = (UINT32*)_pYaddr;
                    u4GoldenCSY1 = (*(pu4CS  ));
                    u4GoldenCSY2 = (*(pu4CS+1));
                    u4GoldenCSC1 = (*(pu4CS+2));
                    u4GoldenCSC2 = (*(pu4CS+3));
                }
                else if(_fgMpvEnCrc)
                {
                    x_sprintf((CHAR *)szFileName, "%s%d_%d.crc", _szGoldFilePath, _u4PicCount, u4FrmFld); 
                    UNUSED(T32_HostLoadData(szFileName, _pYaddr));
                    #if defined(CC_MT5365) || defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)
                    HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)_pYaddr), 64);
                    #else
                    HalFlushInvalidateDCache();
                    #endif
                    pu4CS = (UINT32*)_pYaddr;
                    u4GoldenCSY1 = (*(pu4CS  ));
                    u4GoldenCSY2 = (*(pu4CS+1));
                    u4GoldenCSY3 = (*(pu4CS+2));
                    u4GoldenCSY4 = (*(pu4CS+3));
                    u4GoldenCSC1 = (*(pu4CS+4));
                    u4GoldenCSC2 = (*(pu4CS+5));
                    u4GoldenCSC3 = (*(pu4CS+6));
                    u4GoldenCSC4 = (*(pu4CS+7));
                }
                else
                {
                    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK))              
                    {
                        x_sprintf((CHAR *)szFileName, "%s%d.y", _szGoldFilePath, _u4PicCount); 
                        UNUSED(T32_HostLoadData(szFileName, _pYaddr));
                        x_sprintf((CHAR *)szFileName, "%s%d.c", _szGoldFilePath, _u4PicCount);            
                        UNUSED(T32_HostLoadData(szFileName, _pCaddr));
                    }
                    else
                    {
                        x_sprintf((CHAR *)szFileName, "%s%d.ramY", _szGoldFilePath, _u4PicCount);            
                        UNUSED(T32_HostLoadData(szFileName, _pYaddr));
                        x_sprintf((CHAR *)szFileName, "%s%d.ramCbCr", _szGoldFilePath, _u4PicCount);            
                        UNUSED(T32_HostLoadData(szFileName, _pCaddr));
                    }
                    #if defined(CC_MT5365) || defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)
                    HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)_pYaddr), u4ByteSize);
                    HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)_pCaddr), u4ByteSize>>1);
                    #else
                    HalFlushInvalidateDCache();
                    #endif
                }
            }

            u4BlockSize = BLOCK_W*BLOCK_H; 
            u4BlockPerLine = (prVdecEsInfo->u2LineSize / BLOCK_W);
            u4CS1 = 0;
            u4CS2 = 0;

            if(_fgMpvEnCrc)
            {
                u4CS1 = IO_READ32(IO_VIRT + 0x3000,(2+0)*4);
                u4CS2 = IO_READ32(IO_VIRT + 0x3000,(2+1)*4);
                u4CS3 = IO_READ32(IO_VIRT + 0x3000,(2+2)*4);
                u4CS4 = IO_READ32(IO_VIRT + 0x3000,(2+3)*4);

                if((u4CS1 != u4GoldenCSY1) || (u4CS2 != u4GoldenCSY2) ||
                   (u4CS3 != u4GoldenCSY3) || (u4CS4 != u4GoldenCSY4))
                {
                    LOG(1, "(%d)Y CRC fail\n", _u4PicCount); 

                    #ifdef MPV_DUMP_REG
                    _MPV_DumpReg(ES0);
                    #endif
                    
                    ASSERT(0);      
                    //break;
                }
            }
            else
            {
                for (u4Idx = 0; u4Idx < u4ByteSize; u4Idx++)
                {
                    if(fg16x32)
                    {
                        UINT32 u4X, u4Y;
                        UINT32 u4GAddr16x32;
                        UINT32 u4Addr64x32;
                        UINT32 u4GoldBlockH;
                        UINT32 u4GBlockSize, u4GBlockPerLine;
                        
                        u4GoldBlockH = GOLD_BLOCK_H;
                        if(prVdecEsInfo->u2HSize < 32)
                        {
                            u4GoldBlockH = 16;
                        }
                        u4X = (u4Idx % prVdecEsInfo->u2LineSize);
                        u4Y = (u4Idx / prVdecEsInfo->u2LineSize);
                        u4GBlockPerLine = (prVdecEsInfo->u2HSize / GOLD_BLOCK_W);
                        u4GBlockSize = GOLD_BLOCK_W*u4GoldBlockH;
                        u4GAddr16x32 = (u4Y / u4GoldBlockH) * u4GBlockSize * u4GBlockPerLine;
                        u4GAddr16x32 += (u4X / GOLD_BLOCK_W) * u4GBlockSize;
                        u4GAddr16x32 += ((u4Y % u4GoldBlockH) * GOLD_BLOCK_W);
                        u4GAddr16x32 += ((u4X % GOLD_BLOCK_W));
                        
                        u4Addr64x32 = (u4Y / BLOCK_H) * u4BlockSize * u4BlockPerLine;
                        u4Addr64x32 += (u4X / BLOCK_W) * u4BlockSize;
                        u4Addr64x32 += ((u4Y % BLOCK_H) * BLOCK_W);
                        u4Addr64x32 += ((u4X % BLOCK_W));       

                        if ((u4X >= prVdecEsInfo->u2HSize) 
                            || (u4Y >= prVdecEsInfo->u2VSize))
                        {
                        }
                        else
                        {
                            if (*(UCHAR*)(u4AddrY + u4Addr64x32) != _pYaddr[u4GAddr16x32])
                            {
                                LOG(1, "BYTE 0x%x (%d, %d), Dec: %02x, Ref: %02x, Diff: %3d\n", 
                                    u4Idx, u4X, u4Y, 
                                  *(UCHAR*)(u4AddrY + u4Addr64x32), _pYaddr[u4GAddr16x32],
                                  *(UCHAR*)(u4AddrY + u4Addr64x32) - _pYaddr[u4GAddr16x32]); 

                                #ifdef MPV_DUMP_REG
                                _MPV_DumpReg(ES0);
                                #endif
                                
                                ASSERT(0);      
                                //break;
                            }
                        }		
                    }
                    else
                    {
	                    u4BlockNum = u4Idx /u4BlockSize;
	                    u4BlockStartX = (u4BlockNum % u4BlockPerLine) * BLOCK_W;
	                    u4BlockStartY = (u4BlockNum / u4BlockPerLine) * BLOCK_H;
	                    u4BlockCoordX = (u4Idx % u4BlockSize) % BLOCK_W;
	                    u4BlockCoordY = (u4Idx % u4BlockSize) / BLOCK_W;            
	
	                    if (((u4BlockStartX + u4BlockCoordX) >= prVdecEsInfo->u2HSize) 
	                        || ((u4BlockStartY + u4BlockCoordY) >= prVdecEsInfo->u2VSize))
	                    {
	                    }
	                    else
	                    {
	                        if(_fgMpvEnChkSum)
	                        {
	                            u4CS1 += (*(((UCHAR*)(u4AddrY)) + u4Idx));
	                            u4CS2 += (2*(*(((UCHAR*)(u4AddrY)) + u4Idx)));
	                            //u4CS1 <<= 1;
	                            //u4CS2 <<= 2;
	                        }
	                        else
	                        {
	                            if (*(UCHAR*)(u4AddrY + u4Idx) != _pYaddr[u4Idx])
	                            {
	                                LOG(1, "BYTE 0x%x (%d, %d), Dec: %02x, Ref: %02x, Diff: %3d\n", 
	                                    u4Idx, (u4BlockStartX + u4BlockCoordX), (u4BlockStartY + u4BlockCoordY), 
	                                  *(UCHAR*)(u4AddrY + u4Idx), _pYaddr[u4Idx], *(UCHAR*)(u4AddrY + u4Idx) - _pYaddr[u4Idx]); 
	
	                                #ifdef MPV_DUMP_REG
	                                _MPV_DumpReg(ES0);
	                                #endif
	                                
	                                ASSERT(0);      
	                                //break;
	                            }
	                        }
	                    }		
	                }
	            }
            }

            if(_fgMpvEnChkSum)
            {
                if((u4CS1 != u4GoldenCSY1) || (u4CS2 != u4GoldenCSY2))
                {
                    LOG(1, "Y CheckSum fail"); 

                    #ifdef MPV_DUMP_REG
                    _MPV_DumpReg(ES0);
                    #endif
                    
                    ASSERT(0);      
                    //break;
                }
            }

            LOG(1, "Y Ok!\n");
            
            u4BlockSize = BLOCK_W*(BLOCK_H >> 1);     
            u4BlockPerLine = (prVdecEsInfo->u2LineSize / BLOCK_W);
            u4CS1 = 0;
            u4CS2 = 0;

            if(_fgMpvEnCrc)
            {
                u4CS1 = IO_READ32(IO_VIRT + 0x3000,(6+0)*4);
                u4CS2 = IO_READ32(IO_VIRT + 0x3000,(6+1)*4);
                u4CS3 = IO_READ32(IO_VIRT + 0x3000,(6+2)*4);
                u4CS4 = IO_READ32(IO_VIRT + 0x3000,(6+3)*4);
                
                if((u4CS1 != u4GoldenCSC1) || (u4CS2 != u4GoldenCSC2) ||
                   (u4CS3 != u4GoldenCSC3) || (u4CS4 != u4GoldenCSC4))
                {
                    LOG(1, "(%d)C CRC fail\n", _u4PicCount); 

                    #ifdef MPV_DUMP_REG
                    _MPV_DumpReg(ES0);
                    #endif
                    
                    ASSERT(0);      
                    //break;
                }
            }
            else
            {
                for (u4Idx = 0; u4Idx < (u4ByteSize >> 1); u4Idx++)
                {
                    if(fg16x32)
                    {
                        UINT32 u4X, u4Y;
                        UINT32 u4GAddr16x32;
                        UINT32 u4Addr64x32;
                        UINT32 u4GoldBlockH;
                        UINT32 u4GBlockSize, u4GBlockPerLine;
                        
                        u4GoldBlockH = GOLD_BLOCK_H;
                        if(prVdecEsInfo->u2HSize < 32)
                        {
                            u4GoldBlockH = 16;
                        }
                        u4X = (u4Idx % prVdecEsInfo->u2LineSize);
                        u4Y = (u4Idx / prVdecEsInfo->u2LineSize);
                        u4GBlockPerLine = (prVdecEsInfo->u2HSize / GOLD_BLOCK_W);
                        u4GBlockSize = GOLD_BLOCK_W*(u4GoldBlockH >> 1);
                        u4GAddr16x32 = (u4Y / (u4GoldBlockH >> 1)) * u4GBlockSize * u4GBlockPerLine;
                        u4GAddr16x32 += (u4X / GOLD_BLOCK_W) * u4GBlockSize;
                        u4GAddr16x32 += ((u4Y % (u4GoldBlockH >> 1)) * GOLD_BLOCK_W);
                        u4GAddr16x32 += ((u4X % GOLD_BLOCK_W));
                        
                        u4Addr64x32 = (u4Y / (BLOCK_H >> 1)) * u4BlockSize * u4BlockPerLine;
                        u4Addr64x32 += (u4X / BLOCK_W) * u4BlockSize;
                        u4Addr64x32 += ((u4Y % (BLOCK_H >> 1)) * BLOCK_W);
                        u4Addr64x32 += ((u4X % BLOCK_W));       

                        if ((u4X >= prVdecEsInfo->u2HSize) 
                                || (u4Y >= (prVdecEsInfo->u2VSize >> 1)))
                        {
                        }
                        else
                        {
                            if (*(UCHAR*)(u4AddrC + u4Addr64x32) != _pCaddr[u4GAddr16x32])
                            {
                                LOG(1, "BYTE 0x%x, Dec: %02x, Ref: %02x, Diff: %3d\n", 
                                    u4Idx, *(UCHAR*)(u4AddrC + u4Addr64x32),
                                    _pCaddr[u4GAddr16x32],
                                    *(UCHAR*)(u4AddrC + u4Addr64x32) - _pCaddr[u4GAddr16x32]);

                                #ifdef MPV_DUMP_REG
                                _MPV_DumpReg(ES0);
                                #endif
                                
                                ASSERT(0);      
                                //break;
                            }
                        }
                    }
                    else
                    {
	                    u4BlockNum = u4Idx /u4BlockSize;
	                    u4BlockStartX = (u4BlockNum % u4BlockPerLine) * BLOCK_W;
	                    u4BlockStartY = (u4BlockNum / u4BlockPerLine) * (BLOCK_H >> 1);
	                    u4BlockCoordX = (u4Idx % u4BlockSize) % BLOCK_W;
	                    u4BlockCoordY = (u4Idx % u4BlockSize) / BLOCK_W;            
	
	                    if (((u4BlockStartX + u4BlockCoordX) >= prVdecEsInfo->u2HSize) 
	                            || ((u4BlockStartY + u4BlockCoordY) >= (prVdecEsInfo->u2VSize >> 1)))
	                    {
	                    }
	                    else
	                    {
	                        if(_fgMpvEnChkSum)
	                        {
	                            u4CS1 += (*(((UCHAR*)(u4AddrC)) + u4Idx));
	                            u4CS2 += (2*(*(((UCHAR*)(u4AddrC)) + u4Idx)));
	                            //u4CS1 <<= 1;
	                            //u4CS2 <<= 2;
	                        }
	                        else
	                        {
	                            if (*(UCHAR*)(u4AddrC + u4Idx) != _pCaddr[u4Idx])
	                            {
	                                LOG(1, "BYTE 0x%x, Dec: %02x, Ref: %02x, Diff: %3d\n", 
	                                u4Idx, *(UCHAR*)(u4AddrC + u4Idx), _pCaddr[u4Idx], *(UCHAR*)(u4AddrC + u4Idx) - _pCaddr[u4Idx]);
	
	                                #ifdef MPV_DUMP_REG
	                                _MPV_DumpReg(ES0);
	                                #endif
	                                
	                                ASSERT(0);      
	                                //break;
	                            }
	                        }
	                    }		
	                }
	            }
            }
            
            if(_fgMpvEnChkSum)
            {
                if((u4CS1 != u4GoldenCSC1) || (u4CS2 != u4GoldenCSC2))
                {
                    LOG(1, "C CheckSum fail"); 

                    #ifdef MPV_DUMP_REG
                    _MPV_DumpReg(ES0);
                    #endif
                    
                    ASSERT(0);      
                    //break;
                }
            }
        }

        LOG(5, "Pic %d OK\n", _u4PicCount);        
    }    

    if (IS_FRMPIC(prFbmPicHdr))
    {
        if (++_u4PicCount == prVdecEsInfo->u4CompareNs)
        {
            _u4PicCount = 0;
        }
    }
    else
    {
        if(IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC)) 
        {
            if (++_u4PicCount == prVdecEsInfo->u4CompareNs)
            {
                _u4PicCount = 0;
            }
        }
    }
    
    #ifdef MPV_REPEAT_TEST
    if(_u4PicCount > MPV_DECODE_REPEAT_END)
    {
    	_u4PicCount = MPV_DECODE_REPEAT_START;
    	prEsInfo->fgDecodeOnRepeat = 1;
    }
    #endif
}


void _MPV_SavePic(UCHAR ucFbgId, UCHAR ucFbId)
{
    char szFileName[256];
    //static UINT32 u4Count = 0;
    UINT32 au4CS[8];
    UINT32 u4AddrY, u4AddrC;
    UINT32 u4ByteSize = 0;
    FBM_SEQ_HDR_T *prFbmSeqHdr;
    MPV_ES_INFO_T *prEsInfo;
    UINT32 u4CSY1 = 0, u4CSY2 = 0;
    UINT32 u4CSC1 = 0, u4CSC2 = 0;
    UINT32 u4Idx;
    UINT32 u4BlockSize, u4BlockNum, u4BlockPerLine;
    UINT32 u4BlockStartX, u4BlockStartY, u4BlockCoordX, u4BlockCoordY;

    VDEC_ES_INFO_T *prVdecEsInfo;
    prVdecEsInfo = _VDEC_GetEsInfo(ES0);
    
    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ES0);

    if (((_u4PicCount >= prEsInfo->u4StartSaveNo) && (_u4PicCount <= prEsInfo->u4EndSaveNo)) ||
        ((prEsInfo->u4StartSaveNo == 0) && (prEsInfo->u4EndSaveNo == 0)))
    {        
        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
        ASSERT(prFbmSeqHdr != NULL);

        u4ByteSize = (prVdecEsInfo->u2LineSize) * (((prVdecEsInfo->u2VSize)+0x1f)&(~0x1f));

        if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_REDUCE_REF))
        {
            //u4ByteSize = u4ByteSize >> 1;
        }

        FBM_GetFrameBufferAddr(ucFbgId, ucFbId, &u4AddrY, &u4AddrC);
        LOG(5, "ucFbId (%d) u4AddrY = 0x%x, u4AddrC = 0x%x\n", ucFbId, u4AddrY, u4AddrC);

        if(_fgMpvEnChkSum)
        {
            u4BlockSize = BLOCK_W*BLOCK_H; 
            u4BlockPerLine = (prVdecEsInfo->u2LineSize / BLOCK_W);
            u4CSY1 = 0;
            u4CSY2 = 0;
            
            for (u4Idx = 0; u4Idx < u4ByteSize; u4Idx++)
            {
                u4BlockNum = u4Idx /u4BlockSize;
                u4BlockStartX = (u4BlockNum % u4BlockPerLine) * BLOCK_W;
                u4BlockStartY = (u4BlockNum / u4BlockPerLine) * BLOCK_H;
                u4BlockCoordX = (u4Idx % u4BlockSize) % BLOCK_W;
                u4BlockCoordY = (u4Idx % u4BlockSize) / BLOCK_W;            

                if (((u4BlockStartX + u4BlockCoordX) >= prVdecEsInfo->u2HSize) 
                    || ((u4BlockStartY + u4BlockCoordY) >= prVdecEsInfo->u2VSize))
                {
                }
                else
                {
                    u4CSY1 += (*(UCHAR*)(u4AddrY + u4Idx));
                    u4CSY2 += (2*(*(UCHAR*)(u4AddrY + u4Idx)));
                    //u4CSY1 <<= 1;
                    //u4CSY2 <<= 2;
                }		
            }
            
            u4BlockSize = BLOCK_W*(BLOCK_H >> 1);     
            u4BlockPerLine = (prVdecEsInfo->u2LineSize / BLOCK_W);
            u4CSC1 = 0;
            u4CSC2 = 0;

            for (u4Idx = 0; u4Idx < (u4ByteSize >> 1); u4Idx++)
            {
                u4BlockNum = u4Idx /u4BlockSize;
                u4BlockStartX = (u4BlockNum % u4BlockPerLine) * BLOCK_W;
                u4BlockStartY = (u4BlockNum / u4BlockPerLine) * (BLOCK_H >> 1);
                u4BlockCoordX = (u4Idx % u4BlockSize) % BLOCK_W;
                u4BlockCoordY = (u4Idx % u4BlockSize) / BLOCK_W;            

                if (((u4BlockStartX + u4BlockCoordX) >= prVdecEsInfo->u2HSize) 
                        || ((u4BlockStartY + u4BlockCoordY) >= (prVdecEsInfo->u2VSize >> 1)))
                {
                }
                else
                {
                    u4CSC1 += (*(((UCHAR*)(u4AddrC)) + u4Idx));
                    u4CSC2 += (2*(*(((UCHAR*)(u4AddrC)) + u4Idx)));
                    //u4CSC1 <<= 1;
                    //u4CSC2 <<= 2;
                }		
            }
            x_sprintf((CHAR *)szFileName, "%s%d.cs", _szSaveFilePath, _u4PicCount);
            au4CS[0]  = u4CSY1;
            au4CS[1]  = u4CSY2;
            au4CS[2]  = u4CSC1;
            au4CS[3]  = u4CSC2;
            #if defined(CC_MT5365) || defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)
            HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)au4CS), 16);
            #else
            HalFlushInvalidateDCache();
            #endif
            T32_HostSaveData(szFileName, (unsigned char*)(&(au4CS[0])), 16);
        }
        else if(_fgMpvEnCrc)
        {
            INT32 i, j;
            x_sprintf((CHAR *)szFileName, "%s%d.crc", _szSaveFilePath, _u4PicCount);
            
            for(i=0;i<4;i++)
            {
                au4CS[i] = IO_READ32(IO_VIRT + 0x3000,(2+i)*4);
            }

            for(j=0;j<4;j++)
            {
                au4CS[4+j] = IO_READ32(IO_VIRT + 0x3000,(6+j)*4);
            }
            #if defined(CC_MT5365) || defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)
            HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)au4CS), 64);
            #else
            HalFlushInvalidateDCache();
            #endif
            T32_HostSaveData(szFileName, (unsigned char*)(&(au4CS[0])), 64);
        }
        else
        {
            #if defined(CC_MT5365) || defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)
            HalFlushInvalidateDCacheMultipleLine(VIRTUAL(u4AddrY), u4ByteSize);
            HalFlushInvalidateDCacheMultipleLine(VIRTUAL(u4AddrC), u4ByteSize>>1);
            #else
            HalFlushInvalidateDCache();
            #endif

            {
                static UINT32 u4SaveTo = 0;
                if(u4SaveTo == 0) // save to same folder
                {
                    x_sprintf((CHAR *)szFileName, "%s%d_Y.raw", _szSaveFilePath, _u4PicCount);
                    T32_HostSaveData(szFileName, (unsigned char*)(u4AddrY), u4ByteSize);
                    x_sprintf((CHAR *)szFileName, "%s%d_C.raw", _szSaveFilePath, _u4PicCount);
                    T32_HostSaveData(szFileName, (unsigned char*)(u4AddrC), u4ByteSize>>1);
                }
                else if(u4SaveTo == 1) // save as golden
                {
                    x_sprintf((CHAR *)szFileName, "%s%d.ramY", _szSaveFilePath, _u4PicCount);
                    T32_HostSaveData(szFileName, (unsigned char*)(u4AddrY), u4ByteSize);
                    x_sprintf((CHAR *)szFileName, "%s%d.ramCbCr", _szSaveFilePath, _u4PicCount);
                    T32_HostSaveData(szFileName, (unsigned char*)(u4AddrC), u4ByteSize>>1);
                }
                else // save to temp
                {
                    x_sprintf((CHAR *)szFileName, "d:/tmp/dec%d_Y.raw", _u4PicCount);
                    T32_HostSaveData(szFileName, (unsigned char*)(u4AddrY), u4ByteSize);
                    x_sprintf((CHAR *)szFileName, "d:/tmp/dec%d_C.raw", _u4PicCount);
                    T32_HostSaveData(szFileName, (unsigned char*)(u4AddrC), u4ByteSize>>1);
                }
            }
        }

        #if 0
        if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK))
        {
            if (prVdecEsInfo->ucPicType != B_TYPE)
            {
                FBM_GetFrameBufferAddr(ucFbgId, prVdecEsInfo->ucFbId, &u4AddrY, &u4AddrC);
                LOG(5, "ucFbId (%d) u4AddrY = 0x%x, u4AddrC = 0x%x\n", prVdecEsInfo->ucFbId, u4AddrY, u4AddrC);

                x_sprintf((CHAR *)szFileName, "%s%dn_Y.raw", _szSaveFilePath, _u4PicCount);            
                T32_HostSaveData(szFileName, (unsigned char*)(u4AddrY), u4ByteSize);       
           
                //x_sprintf((CHAR *)szFileName, "d:/tmp/data/dec%d_C.raw", _u4PicCount);       
                x_sprintf((CHAR *)szFileName, "%s%dn_C.raw", _szSaveFilePath, _u4PicCount);         
                T32_HostSaveData(szFileName, (unsigned char*)(u4AddrC), u4ByteSize>>1);       
            }
        }
        #endif
        LOG(5, "Pic %d Saved\n", _u4PicCount);
    }
    
    _u4PicCount++;   
}
#endif


void _MPV_GetStartTime(UCHAR ucMpvId)
{
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    HAL_GetTime(&_rStartDecodeTime[ucMpvId]);
    
    #ifdef MPV_STATISTICS

#ifdef CC_MT5396
    _au4DelayClock[ucMpvId] = (BSP_GetDomainClock(SRC_MEM_CLK) >> 1) ; //dram cycle /sec
#else
    _au4DelayClock[ucMpvId] = (BSP_GetDomainClock(CAL_SRC_DMPLL) >> 1) ; //dram cycle /sec
#endif
	
    UNUSED(_au4DelayClock);
    // we use av.dram to monitor dram.
    // remove this code to make measure decoding time correctly.
    #if 0  //total bandwidth
    {
        _au4DramCycle[ucMpvId] = SRM_DramMonitor(SRM_DRV_TOTAL, _au4DelayClock, 0);
    }
    #else  //mpeg
    {
        #if defined(VDEC_IS_POST_MT5368)
        _au4DramCycle[ucMpvId] = SRM_DramMonitor(0,SRM_DRV_VDEC_MC, _au4DelayClock[ucMpvId], 0);
        #elif defined(CC_MT5395)
        _au4DramCycle[ucMpvId] = SRM_DramMonitor(SRM_DRV_MPEG, _au4DelayClock[ucMpvId], 0);
        #elif defined(CC_MT5363) || defined(CC_MT5365)
        _au4DramCycle[ucMpvId] = SRM_DramMonitor(SRM_DRV_VDEC_MC, _au4DelayClock[ucMpvId], 0);
        #else
        _au4DramCycle[ucMpvId] = SRM_DramMonitor(SRM_DRV_MPEG, _au4DelayClock[ucMpvId], 0);
        #endif
    }
    #endif
    #endif
}


void _MPV_GetStopTime(UCHAR ucMpvId)
{
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
    HAL_GetTime(&_rStopDecodeTime[ucMpvId]);
    
    #ifdef MPV_STATISTICS
    #if defined(CC_MT5365) || defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)
    _au4DramCycle[ucMpvId] = IO_REG32(DRAM_BASE, 0x190);
    #else    
    _au4DramCycle[ucMpvId] = IO_REG32(DRAM_BASE, 0x3C8);
    #endif
    #endif
}


#ifdef MPV_STATISTICS 
void _MPV_ResetTime(void)
{
    UCHAR ucIdx;

    for (ucIdx = 0; ucIdx < MPV_MAX_VLD; ucIdx++)
    {
        _u4ICount[ucIdx] = 0;
        _u4PCount[ucIdx] = 0;
        _u4BCount[ucIdx] = 0;
        _u4TotalCount[ucIdx] = 0;

        _u4IAvgTime[ucIdx] = 0;
        _u4PAvgTime[ucIdx] = 0;
        _u4BAvgTime[ucIdx] = 0;

        _u4IMaxTime[ucIdx] = 0;
        _u4PMaxTime[ucIdx] = 0;
        _u4BMaxTime[ucIdx] = 0;

        _u4IMinTime[ucIdx] = 0xFFFFFFFF;
        _u4PMinTime[ucIdx] = 0xFFFFFFFF;
        _u4BMinTime[ucIdx] = 0xFFFFFFFF;

        _u4ITotalAvgTime[ucIdx] = 0;
        _u4PTotalAvgTime[ucIdx] = 0;
        _u4BTotalAvgTime[ucIdx] = 0;

        _u4ITotalMaxTime[ucIdx] = 0;
        _u4PTotalMaxTime[ucIdx] = 0;
        _u4BTotalMaxTime[ucIdx] = 0;

        _u4ITotalMinTime[ucIdx] = 0xFFFFFFFF;
        _u4PTotalMinTime[ucIdx] = 0xFFFFFFFF;
        _u4BTotalMinTime[ucIdx] = 0xFFFFFFFF;

        _au4MaxCycle[ucIdx] = 0;
    }

}


void _MPV_PrintTime(UCHAR ucMpvId)
{    
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }
    
    LOG(0, "VLD %d Cycle(%d)\n",ucMpvId, _au4MaxCycle[ucMpvId]);
    
    LOG(0, "VLD %d I(%d) HW(%d, %d, %d) SW(%d, %d, %d)\n",
        ucMpvId,
        _u4ICount[ucMpvId],
        _u4IMaxTime[ucMpvId], _u4IAvgTime[ucMpvId], _u4IMinTime[ucMpvId], 
        _u4ITotalMaxTime[ucMpvId], _u4ITotalAvgTime[ucMpvId], _u4ITotalMinTime[ucMpvId]);

    LOG(0, "VLD %d P(%d) HW(%d, %d, %d) SW(%d, %d, %d)\n",
        ucMpvId,
        _u4PCount[ucMpvId],
        _u4PMaxTime[ucMpvId], _u4PAvgTime[ucMpvId], _u4PMinTime[ucMpvId], 
        _u4PTotalMaxTime[ucMpvId], _u4PTotalAvgTime[ucMpvId], _u4PTotalMinTime[ucMpvId]);

    LOG(0, "VLD %d B(%d) HW(%d, %d, %d) SW(%d, %d, %d)\n",
        ucMpvId,
        _u4BCount[ucMpvId],
        _u4BMaxTime[ucMpvId], _u4BAvgTime[ucMpvId], _u4BMinTime[ucMpvId], 
        _u4BTotalMaxTime[ucMpvId], _u4BTotalAvgTime[ucMpvId], _u4BTotalMinTime[ucMpvId]);

}
#endif

#ifdef VDEC_TIME_PROFILE
void _MPV_GetParam(UCHAR ucEsId, UINT32 u4Type, UINT32* pu4Param1, UINT32* pu4Param2, UINT32* pu4Param3)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    ASSERT(ucEsId < MAX_ES_NUM);
    ASSERT(pu4Param1 !=NULL);
    ASSERT(pu4Param2 !=NULL);
    ASSERT(pu4Param3 !=NULL);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

  #if 0
    if(u4Type == (UINT32)DRAM_BANDWITH_SIMU)
    {
        *pu4Param1 = prVdec->rDecParam.u4DramMaxLen;
        *pu4Param2 = prVdec->rDecParam.u4DramReqConnDis;
        *pu4Param3 = prVdec->rDecParam.u4DramFifoBank;
    }
  #endif 	
    if (u4Type == (UINT32)MPV_TIME_PROFILE)
    {
        if(prVdecEsInfo->u1FBufStatus)
        {
            *pu4Param1 = prVdecEsInfo->ucFbgId;
            *pu4Param2 = prVdecEsInfo->u4CurDecMs;       //hw dec
            *pu4Param3 = 0;
            //*pu4Param3 = prVdecEsInfo->u4CurWaitFbMicro/1000;   //wait fb time
            
        }
        else
        {
            *pu4Param1 = 0;
			*pu4Param2 = 0;
			*pu4Param3 = 0;
        }
    }
    else if (u4Type == (UINT32)MPV_TIME_PROFILE2)
    {
          //*pu4Param1 = prVdecEsInfo->u4CurRstMs;   //wait fb time
          // prVdecEsInfo->u4CurRstMs = 0;
         
          *pu4Param1 = 0;   //wait fb time
          *pu4Param2 =  prVdecEsInfo->ucPicType;
          *pu4Param3 = (UINT32)prVdecEsInfo->u1FBufStatus;
    }
    else if (u4Type == (UINT32)MPV_TIME_PROFILE3)
    {
        *pu4Param1 = prVdecEsInfo->u4CurSwMicro;
        *pu4Param2 = prVdecEsInfo->u4CurDecCycle;
        *pu4Param3 = prVdecEsInfo->u4CurDramCycle;
    }

}


BOOL MPV_MpegIsPic(UCHAR ucEsId, VDEC_PES_INFO_T *prPes)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    //VDEC_INFO_T *prVdecInfo;

	static BOOL fgToggle;

	prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

	fgToggle = ~fgToggle;
	UNUSED(prPes);
	if( prVdecEsInfo->prMpvCounter->ucPicStruct==3)
	{
      return TRUE;
	}
	else
	{
       if(fgToggle)
       {
       	   return TRUE;
       }
	   else
	   {
           return FALSE;
	   }
	}
 
}
#endif 


