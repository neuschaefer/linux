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
 * $RCSfile: mpv_if.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file mpv_if.c
 *  This file contains implementation of exported APIs of MPV.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "mpv_if.h"
#include "mpv_drvif.h"
#include "vdec_if.h"
#include "mpv_debug.h"
#include "vdec_drvif.h"
#include "mpv_type.h"
#include "mpv_parser.h"
#include "mpv_decode.h"
#include "mpv_hw.h"
#include "mpv_config.h"
#include "dmx_if.h"
#include "fbm_drvif.h"
#include "drvcust_if.h"
#include "drv_common.h"
#include "stc_drvif.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_common.h"
#include "x_typedef.h"
#include "x_stl_lib.h"
#include "x_assert.h"

#ifdef CC_BOOT_VDEC_LOGO
#include "osd_drvif.h"
#include "panel.h"
#include "eepdef.h"
#include "api_eep.h"
#ifdef CC_NAND_ENABLE
#include "nand_if.h"
#endif
#include "nor_if.h"
#endif

#ifdef LINUX_TURNKEY_SOLUTION
#ifdef __KERNEL__
#include <linux/mm.h>
#endif
#endif

#ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
#endif

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define RWPTR_DIFF	144  //16x9
#define MPV_STC_CLOCK (90000*1000)

#define LOGO_DATA_SIZE 0x9

#define MPV_DBK_VALUE_MAX (4+1) // 1 is disable this function
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

UINT32 _au4MpvDbkP1[MPV_DBK_VALUE_MAX][4]
=
{
    {  0,  0,  0,  0}, // disable this function
    {  0,  0,  0,  0}, // disable dbk
    { 16, 19, 25, 28}, // low
    { 12, 15, 23, 25}, // middle
    {  8, 10, 18, 20}  // strong
};

static BOOL _fgMpvInitiated = FALSE;
//static BOOL _afgMpvThreadActive[MPV_MAX_ES];
//static BOOL _afgMpvThreadDestroy[MPV_MAX_ES];
//static UCHAR _aucSyncMode[MPV_MAX_ES];
//static UCHAR _aucStcId[MPV_MAX_ES];
//static HANDLE_T _ahMpvSemaphore[MPV_MAX_VLD];
//static VDEC_PES_INFO_T _rPesInfo;

static MPV_ES_INFO_T _arMpvEsInfo[MPV_MAX_ES];
static MPV_CC_T _arMpvCc[MPV_MAX_ES];
static MPV_EXT_INFO_T _rExtInfo;
static MPV_INFO_T _rMpvInfo;

static VDEC_PES_INFO_T _rMpvPesInfo; // this is for cli


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static MPV_ALL_ASPECT_RATIO_T _DecideAspectRatio(UCHAR ucAspectRatio, UINT32 u4Width, UINT32 u4Height)
{
	if (ucAspectRatio != MPEG_ASPECT_RATIO_1_1)
	{
		switch (ucAspectRatio)
		{
			case MPEG_ASPECT_RATIO_4_3:
				return MPV_ALL_ASPECT_RATIO_4_3;

			case MPEG_ASPECT_RATIO_16_9:
				return MPV_ALL_ASPECT_RATIO_16_9;

			case MPEG_ASPECT_RATIO_221_1:
			default:
				return MPV_ALL_ASPECT_RATIO_221_1;
		}
	}
	else
	{
		UINT32 u4Temp;

		// 4:3 = 1.33
		// 16:9 = 1.78
		// 2.21:1 = 2.21

		// if width / height < (1.78 + 1.33) / 2 >> 4:3
		// else if width / height < (2.21 + 1.78) / 2 >> 16:9
		// else 2.21 : 1
		// (1.78 + 1.33) / 2 = 14 / 9
		// (2.21 + 1.78) / 2 ~= 2

		u4Temp = (((u4Width * 9) / 14) / u4Height);

		if (u4Temp < 1)
		{
			return MPV_ALL_ASPECT_RATIO_4_3;
		}
		else
		{
			u4Temp = ((u4Width / 2) / u4Height);

			if (u4Temp < 1)
			{
				return MPV_ALL_ASPECT_RATIO_16_9;
			}
			else
			{
				return MPV_ALL_ASPECT_RATIO_221_1;
			}
		}
	}
}


void _MPV_StopDecES(UCHAR ucEsId)
{
//    MPV_ES_INFO_T *prEsInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
//    VDEC_INFO_T *prVdecInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

  //  prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
//    prVdecInfo = _VDEC_GetInfo();

    if (prVdecEsInfo->ucFbgId != FBM_FBG_ID_UNKNOWN)
    {
        FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, ucEsId);
        FBM_ReleaseGroup(prVdecEsInfo->ucFbgId);
        prVdecEsInfo->ucFbgId = FBM_FBG_ID_UNKNOWN;
        prVdecEsInfo->ucFbId = FBM_FB_ID_UNKNOWN;
#ifdef CC_MPV_DEBLOCKING
        prVdecEsInfo->ucDbkFbId = FBM_FB_ID_UNKNOWN;
        prVdecEsInfo->ucDbkRefId = FBM_FB_ID_UNKNOWN;
#endif
        LOG(5, "StopDecEs FBM_ReleaseGroup!\n");
    }

    //change to common part
    /*_VDEC_LockFlushMutex(ucEsId);

    if (prVdecEsInfo->fgFlushEsmQ)
    {
        _VDEC_FlushEs(ucEsId);
        prVdecEsInfo->fgFlushEsmQ = FALSE;
        _VDEC_UnlockFlushSema(ucEsId);
    }
    _VDEC_ClearEsInfo(ucEsId);

    _VDEC_UnlockFlushMutex(ucEsId);*/
}




//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

 void _MPV_SetDbk(UCHAR ucEsId)
{
#ifdef CC_MPV_DEBLOCKING
//    MPV_ES_INFO_T *prEsInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
//    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);

//    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
	SET_DECFLAG(prVdecEsInfo, DEC_FLAG_DBK);

    FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_DEBLOCKING);
    {
        UCHAR ucFbNs;
        UCHAR ucIdx;
        FBM_PIC_HDR_T *prFbmPicHdr;

        ucFbNs = FBM_GetFrameBufferNs(prVdecEsInfo->ucFbgId);
        for (ucIdx = 0; ucIdx < ucFbNs; ucIdx++)
        {
            prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucFbNs);
            if(prFbmPicHdr == NULL)
            {
                ASSERT(prFbmPicHdr != NULL);
                continue;
            }
            prFbmPicHdr->u4QpAvg = 0;
        }
    }
#else
    LOG(3, "There is no deblocking function\n");
#endif
}

void MPV_CleanRRDbk(UCHAR ucEsId)
{
    MPV_ES_INFO_T *prEsInfo;
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prEsInfo->fgClearRRDbk = TRUE;
}

void _MPV_CleanRRDbk(UCHAR ucEsId)
{
    MPV_ES_INFO_T *prEsInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);

    if(IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_REDUCE_REF))
    {
    	SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    	prEsInfo->fgChgFbgT = TRUE;  //for Half-Ref test
    }

    if(IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK))
    {
    	if (prVdecEsInfo->ucDbkRefId != FBM_FB_ID_UNKNOWN)
    	{
    		if ((FBM_GetFrameBufferStatus(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucDbkRefId) == FBM_FB_STATUS_DECODE) ||
    			(FBM_GetFrameBufferStatus(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucDbkRefId) == FBM_FB_STATUS_READY))
    		{
    		    LOG_MFBM("MFBM Set Empty d clean, %d\n", prVdecEsInfo->ucDbkRefId);
    			FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId,
    									prVdecEsInfo->ucDbkRefId, FBM_FB_STATUS_EMPTY);
    		}
    		prVdecEsInfo->ucDbkRefId = FBM_FB_ID_UNKNOWN;
    		LOG_MFBM("MFBM ucDbkRefId 0xFF a\n");
    	}
    }
    else
    {
		prVdecEsInfo->ucDbkRefId = FBM_FB_ID_UNKNOWN;
		LOG_MFBM("MFBM ucDbkRefId 0xFF b\n");
    }

	CLR_DECFLAG(prVdecEsInfo, DEC_FLAG_DBK);
	CLR_DECFLAG(prVdecEsInfo, DEC_FLAG_REDUCE_REF);
	FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_DEBLOCKING);
	FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_REDUCE_REF);
	prEsInfo->fgRatioCLI = FALSE;

}
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


/** .
*@param .
 */
void* _MpvGetHdlr(UCHAR ucEsId)
{
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
	return (void*)(&(_arMpvEsInfo[ucEsId]));
}

/** .
*@param .
 */
void* _MPV_GetExtInfo(void)
{
    return (void*)(&_rExtInfo);
}

/** .
*@param .
 */
void* _MPV_GetMpvInfo(void)
{
    return (void*)&_rMpvInfo;
}


void MPV_Init(void)
{
    UCHAR ucMpvId;
    UCHAR ucEsId;
//    HANDLE_T hThread;
    //VDEC_INFO_T *prVdecInfo;

    //prVdecInfo = _VDEC_GetInfo();

    if (!_fgMpvInitiated)
    {
        _fgMpvInitiated = TRUE;
        for (ucEsId = 0; ucEsId < MPV_MAX_ES; ucEsId++)
        {
            x_memset((void *)&_arMpvEsInfo[ucEsId], 0, sizeof(MPV_ES_INFO_T));	//init state = VDEC_ST_STOP
        }



        if (!BSP_IsFPGA())
        {
            _MPV_PllInit();
        }

        _MPV_DecInit();
        //_MPV_IsrInit();

        for (ucMpvId = 0; ucMpvId < MPV_MAX_VLD; ucMpvId++)
        {

            _MPV_CreateDecSema(ucMpvId);
            _MPV_CreateSearchSema(ucMpvId);
            //VERIFY (x_sema_create(&prVdecInfo->arVdecVldInfo[ucMpvId].hVldSemaphore, X_SEMA_TYPE_MUTEX,
            //    X_SEMA_STATE_UNLOCK) == OSR_OK);
        }

/*
        for (ucEsId = 0; ucEsId < MPV_MAX_ES; ucEsId++)
        {
            CHAR szBuf[16];
            sprintf(szBuf, "MPV%d", ucEsId);

            // create thread
            VERIFY(x_thread_create(&hThread, szBuf, MPV_STACK_SIZE,
                MPV_THREAD_PRIORITY,  _DecMainLoop, 4, (void*)&ucEsId) == OSR_OK);

        }*/
    }
    _MPV_IsrInit(); // always init Isr
#if 0
{
UINT32 u4Idx;
UINT32 u4Reg;

        u4Reg = 0;
        IO_WRITE32(0x2000, 0xb0, 0xffffffff);

        for (u4Idx = 0; u4Idx++; u4Idx < 10000)
        {
            u4Reg = (UINT32)IO_READ32(0x2000, 0xb0);
            if (u4Reg != 0xffffffff)
            {
                printf("%d\n", u4Idx);
                ASSERT(0);
            }
        }
        printf("ok\n");
}
#endif
}



void MPV_Termint(void)
{
    VDEC_Termint();
/*
    if (_fgMpvInitiated)
    {
        UCHAR ucEsId;

        for (ucEsId = 0; ucEsId < MPV_MAX_ES; ucEsId++)
        {
            _afgMpvThreadDestroy[ucEsId] = TRUE;
        }
    }*/
}

void MPV_Reset(void)
{
    _MPV_DecInit();
}


void MPV_Play(UCHAR ucEsId)
{

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    UNUSED(VDEC_Play(ucEsId, VDEC_FMT_MPV));
    /*

    _VDEC_SendCmd(ucEsId, VDEC_CMD_PLAY);

    // Virtual Picture Header for Event Trigger
    _rMpvPesInfo.ucEsId = ucEsId;
    _rMpvPesInfo.ucMpvId = MPV_MAX_VLD;

    VERIFY(VDEC_SendEs(&_rMpvPesInfo) == 1);
    */
}

void MPV_Stop(UCHAR ucEsId)
{
//    MPV_ES_INFO_T *prEsInfo;
    //MPV_CC_T *prMpvCc;
    //VDEC_ES_INFO_T *prVdecEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    VDEC_Stop(ucEsId);
}


void MPV_Pause(UCHAR ucEsId)
{
//    MPV_ES_INFO_T *prEsInfo;
    //VDEC_ES_INFO_T *prVdecEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }
    VDEC_Pause(ucEsId);
}

void MPV_PlayCc(UCHAR ucEsId)
{
    MPV_CC_T    *prMpvCc;

    CHAR szBuf[16];
    UINT32 u4CcQSize;

    LOG(3, "Play CC\n");
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    _VDEC_LockCcSema(ucEsId);

    prMpvCc = (MPV_CC_T*)_MPV_GetMpvCc(ucEsId);

    if (prMpvCc->hCcQueue == 0)
    {
        SPrintf(szBuf, "CC-Q%d\n", ucEsId);
        u4CcQSize = (prMpvCc->u4CcBufSize)/3;
        VERIFY(x_msg_q_create(&(prMpvCc->hCcQueue), szBuf, sizeof(CC_INFO_T),
            (UINT16)u4CcQSize) == OSR_OK);
    }

    prMpvCc->fgPlayCc = TRUE;

    _VDEC_UnlockCcSema(ucEsId);

}


void MPV_StopCc(UCHAR ucEsId)
{
    MPV_CC_T    *prMpvCc;

    LOG(3, "Stop CC\n");
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    _VDEC_LockCcSema(ucEsId);

    prMpvCc = (MPV_CC_T*)_MPV_GetMpvCc(ucEsId);
    prMpvCc->fgPlayCc = FALSE;
    //VERIFY(x_msg_q_delete(prMpvCc->hCcQueue) == OSR_OK);

    _VDEC_UnlockCcSema(ucEsId);

}


INT32 MPV_AllocCcBuf(UCHAR ucEsId, UINT32 u4CcBufSize)
{
    MPV_CC_T *prMpvCc;

    LOG(3, "Alloc CC Buf\n");
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    _VDEC_LockCcSema(ucEsId);

    prMpvCc = (MPV_CC_T*)_MPV_GetMpvCc(ucEsId);
    prMpvCc->u4CcBufSize = u4CcBufSize;

    if (prMpvCc->pucCcBuf == NULL)
    {
        prMpvCc->pucCcBuf = (UCHAR*)x_mem_alloc(u4CcBufSize);
        prMpvCc->pucCcTmpBuf = (UCHAR*)x_mem_alloc(u4CcBufSize);
        prMpvCc->pucCcAtscIPTmpBuf = (UCHAR*)x_mem_alloc(u4CcBufSize);
        prMpvCc->pucCcScteIPTmpBuf = (UCHAR*)x_mem_alloc(u4CcBufSize);
    }
    else
    {
        prMpvCc->pucCcBuf =
            (UCHAR*)x_mem_realloc((void*)(prMpvCc->pucCcBuf), u4CcBufSize);
        prMpvCc->pucCcTmpBuf =
            (UCHAR*)x_mem_realloc((void*)(prMpvCc->pucCcTmpBuf), u4CcBufSize);
        prMpvCc->pucCcAtscIPTmpBuf =
            (UCHAR*)x_mem_realloc((void*)(prMpvCc->pucCcAtscIPTmpBuf), u4CcBufSize);
        prMpvCc->pucCcScteIPTmpBuf =
            (UCHAR*)x_mem_realloc((void*)(prMpvCc->pucCcScteIPTmpBuf), u4CcBufSize);
    }

    if ((prMpvCc->pucCcBuf == NULL)||(prMpvCc->pucCcTmpBuf == NULL)
        ||(prMpvCc->pucCcAtscIPTmpBuf == NULL)||(prMpvCc->pucCcScteIPTmpBuf == NULL))
    {
        _VDEC_UnlockCcSema(ucEsId);
        return MPV_DRV_INV_SET_INFO;
    }

    x_memset(prMpvCc->pucCcBuf, 0, u4CcBufSize);
    x_memset(prMpvCc->pucCcTmpBuf, 0, u4CcBufSize);
    x_memset(prMpvCc->pucCcAtscIPTmpBuf, 0, u4CcBufSize);
    x_memset(prMpvCc->pucCcScteIPTmpBuf, 0, u4CcBufSize);

    _VDEC_UnlockCcSema(ucEsId);

    return (MPV_DRV_OK);
}


INT32 MPV_FreeCcBuf(UCHAR ucEsId)
{
    INT32 i4Ret;
    //MPV_CC_T *prMpvCc;

    LOG(3, "Free CC Buf\n");
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    i4Ret = MPV_FlushCcBuf(ucEsId);

    return (i4Ret);

}


INT32 MPV_FlushCcBuf(UCHAR ucEsId)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    MPV_CC_T *prMpvCc;
    CC_INFO_T rCcInfo;
    CC_BUF_INFO_T *prCcBufInfo;

    LOG(3, "Flush CC Buf\n");
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    _VDEC_LockCcSema(ucEsId);
    prMpvCc = (MPV_CC_T*)_MPV_GetMpvCc(ucEsId);

    if (prMpvCc->pucCcBuf == NULL)
    {
        _VDEC_UnlockCcSema(ucEsId);
        return (MPV_DRV_INV_SET_INFO);
    }

    zMsgSize = sizeof(CC_INFO_T);
    prCcBufInfo = &(prMpvCc->rCcBufInfo);

    while (x_msg_q_receive(&u2MsgQIdx, &rCcInfo, &zMsgSize,
        &(prMpvCc->hCcQueue), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
    }

    prMpvCc->fgCcPreRef = FALSE;
    prCcBufInfo->u4Rptr = 0;
    prCcBufInfo->u4Wptr = 0;
    _VDEC_UnlockCcSema(ucEsId);

    return (MPV_DRV_OK);

}


INT32 MPV_GetCc(UCHAR ucEsId, MPV_CC_DATA* prCcData)
{
    UCHAR *pucCcData;
    UCHAR *pucCcBuf;
    UINT16 u2MsgQIdx;
    UINT32 u4Idx;
    UINT32 u4Ptr = 0;
    INT32  i4Ret = 0;
    SIZE_T zMsgSize;
    CC_INFO_T rCcInfo;
    CC_BUF_INFO_T *prCcBufInfo;
    MPV_CC_T *prMpvCc;

    if (prCcData == NULL)
    {
        return (MPV_DRV_INV_GET_INFO);
    }

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    _VDEC_LockCcSema(ucEsId);

    prMpvCc = (MPV_CC_T*)_MPV_GetMpvCc(ucEsId);

    if (prMpvCc->fgPlayCc)
    {
        LOG(10, "Get CC \n");

        zMsgSize = sizeof(CC_INFO_T);

        i4Ret = x_msg_q_receive(&u2MsgQIdx, &rCcInfo, &zMsgSize,
                    &(prMpvCc->hCcQueue), 1, X_MSGQ_OPTION_NOWAIT);

        if ( i4Ret != OSR_OK )
        {
            if ( i4Ret == OSR_NO_MSG )
            {
                prCcData->u4Size = 0;
                _VDEC_UnlockCcSema(ucEsId);
                return (MPV_DRV_OK);
            }

            ASSERT(i4Ret == OSR_OK);
        }

        if (prCcData->u4Size < rCcInfo.u4Size)
        {
            VERIFY(x_msg_q_send(prMpvCc->hCcQueue, &rCcInfo,
                sizeof(CC_INFO_T), 254) == OSR_OK);

            _VDEC_UnlockCcSema(ucEsId);

            return (MPV_DRV_INV_GET_INFO);
        }

        pucCcBuf = prMpvCc->pucCcBuf;
        pucCcData = prCcData->pucCcData;
        prCcBufInfo = &(prMpvCc->rCcBufInfo);

        for (u4Idx = 0; u4Idx < rCcInfo.u4Size; u4Idx++)
        {
            u4Ptr = (rCcInfo.u4BufRptr + u4Idx) % prMpvCc->u4CcBufSize;
            pucCcData[u4Idx] = pucCcBuf[u4Ptr];
        }

        prCcBufInfo->u4Rptr = u4Ptr + 1;

        prCcData->u4Size = rCcInfo.u4Size;
        prCcData->u8Pts = (PTS_T)(rCcInfo.u8Pts+VDP_GetDelayTime());
        prCcData->fgIsScte = rCcInfo.fgIsScte;
        prCcData->fgBottomFirst = rCcInfo.fgBottomFirst;
    }

    _VDEC_UnlockCcSema(ucEsId);

    return (MPV_DRV_OK);

}


UCHAR MPV_CallBackEsm(const MPV_PES_INFO_T *prPesInfo)
{
    VDEC_PES_INFO_T rVdecPesInfo;
    x_memset(&rVdecPesInfo, 0, sizeof(rVdecPesInfo));
    x_memcpy(&rVdecPesInfo, prPesInfo, sizeof(MPV_PES_INFO_T));
    return VDEC_SendEs((VDEC_PES_INFO_T*)&rVdecPesInfo);
}

UINT32 MPV_GetDecMode(UCHAR ucEsId)
{
    UINT32 u4DecMode;
    VDEC_INFO_T *prVdecInfo;

    prVdecInfo = _VDEC_GetInfo();
    u4DecMode = (UINT32)prVdecInfo->eCurState;

    UNUSED(ucEsId);
    return (u4DecMode);
}


void MPV_SetDecMode(UCHAR ucEsId, UINT32 u4DecMode)
{
    VDEC_INFO_T *prVdecInfo;

    prVdecInfo = _VDEC_GetInfo();
    prVdecInfo->ePreviousState = prVdecInfo->eCurState;
    prVdecInfo->eCurState = (ENUM_VDEC_STATE_T)u4DecMode;

    UNUSED(ucEsId);
}

INT32 MPV_SetCcNfyFct(UCHAR ucEsId, const MPV_CC_NFY_INFO_T* prCcNfyInfo)
{
    MPV_CC_T *prMpvCc;

    if (prCcNfyInfo == NULL)
    {
        return (MPV_DRV_INV_SET_INFO);
    }

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    LOG(3, "Set Cc Nfy\n");

    prMpvCc = (MPV_CC_T*)_MPV_GetMpvCc(ucEsId);
    ASSERT(prMpvCc != NULL);
    prMpvCc->pvCcNfyTag = prCcNfyInfo->pvTag;
    prMpvCc->pfCcNfyFct = prCcNfyInfo->pfCcNfyFct;

    return (MPV_DRV_OK);
}

// ilii could delete it, it is in vdp now
/*
INT32 MPV_SetPtsNfyFct(UCHAR ucEsId, const MPV_PTS_NFY_INFO_T* prPtsNfyInfo)
{
    MPV_ES_INFO_T *prEsInfo;

    if (prPtsNfyInfo == NULL)
    {
        return (MPV_DRV_INV_SET_INFO);
    }

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    ASSERT(prEsInfo != NULL);
    prEsInfo->pvPtsNfyTag = prPtsNfyInfo->pvTag;
    prEsInfo->pfPtsNfyFct = prPtsNfyInfo->pfPtsNfyFct;
    return (MPV_DRV_OK);
}*/

/*
INT32 MPV_SetInpNfyFct(UCHAR ucEsId, const MPV_INP_NFY_INFO_T* prInpNfyInfo)
{
    MPV_ES_INFO_T *prEsInfo;

    if (prInpNfyInfo == NULL)
    {
        return (MPV_DRV_INV_SET_INFO);
    }

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    ASSERT(prEsInfo != NULL);

    prEsInfo->pvInpNfyTag = prInpNfyInfo->pvTag;
    prEsInfo->pfAcquireNfy = prInpNfyInfo->pfAcquireNfy;
    prEsInfo->pfReleaseNfy = prInpNfyInfo->pfReleaseNfy;
    return (MPV_DRV_OK);
}
*/
/*
INT32 MPV_SetDecNfy(UCHAR ucEsId, const MPV_DEC_NFY_INFO_T* prDecNfyInfo)
{
    // VDEC_DEC_NFY_INFO_T is the same with MPV_DEC_NFY_INFO_T
    return VDEC_SetDecNfy(ucEsId, (VDEC_DEC_NFY_INFO_T*)prDecNfyInfo);
    VDEC_ES_INFO_T *prVdecEsInfo;

    if (prDecNfyInfo == NULL)
    {
        return (MPV_DRV_INV_SET_INFO);
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->pvDecNfyTag = prDecNfyInfo->pvTag;
    prVdecEsInfo->pfDecNfyFct = prDecNfyInfo->pfDecNfy;

    return (MPV_DRV_OK);
}
*/

INT32 MPV_AllocIFrameBuf(UCHAR ucEsId, MPV_DEC_I_FRAME_BUFF_T* prIFrameBuff)
{
    MPV_ES_INFO_T *prEsInfo;

    LOG(3, "Alloc I Frame Buf\n");

    if (prIFrameBuff == NULL)
    {
        return (MPV_DRV_INV_SET_INFO);
    }
    else if (prIFrameBuff->u4Size == 0)
    {
        return (MPV_DRV_INV_SET_INFO);
    }

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);

    if (prEsInfo->pucIFrameBuf == NULL)
    {
        #ifdef LINUX_TURNKEY_SOLUTION
        #ifdef __KERNEL__
        {
        	FBM_POOL_T *prFbmPool;

            prEsInfo->pucIFrameBuf = NULL;
        	prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
        	if((prIFrameBuff->u4Size + PAGE_SIZE + MPV_ESFIFO_ALIGN) <= prFbmPool->u4Size)
    	    {
        	    prEsInfo->pucIFrameBuf = VIRTUAL(PAGE_ALIGN(prFbmPool->u4Addr));
    	    }
        	else
    	    {
    	        LOG(1, "%s, alloc size(%d) > reserved size(%d)\n",__FUNCTION__,
    	            (prIFrameBuff->u4Size + PAGE_SIZE + MPV_ESFIFO_ALIGN),
    	            prFbmPool->u4Size);
    	    }
        }
        #else
        LOG(0, "__KERNEL__ not define\n");
        #endif
        #else
        prEsInfo->pucIFrameBuf = (UCHAR*)x_mem_alloc(prIFrameBuff->u4Size + MPV_ESFIFO_ALIGN);
        #endif

        if (prEsInfo->pucIFrameBuf == NULL)
        {
            return (MPV_DRV_SET_FAILED);
        }

        prIFrameBuff->pucIFrameBufAddr = (UCHAR*)MPV_ALIGN_MASK((UINT32)prEsInfo->pucIFrameBuf, MPV_ESFIFO_ALIGN);
    }

    if(!prIFrameBuff->pucIFrameBufAddr)
    {
        prIFrameBuff->pucIFrameBufAddr =
            (UCHAR*)MPV_ALIGN_MASK((UINT32)prEsInfo->pucIFrameBuf, MPV_ESFIFO_ALIGN);
    }
    x_memset(prEsInfo->pucIFrameBuf, 0, prIFrameBuff->u4Size + MPV_ESFIFO_ALIGN);

    return (MPV_DRV_OK);
}


INT32 MPV_FreeIFrameBuf(UCHAR ucEsId)
{
    MPV_ES_INFO_T *prEsInfo;

    LOG(3, "Free CC Buf\n");
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);

    #ifndef LINUX_TURNKEY_SOLUTION
    if(prEsInfo->pucIFrameBuf)
    {
        x_mem_free(prEsInfo->pucIFrameBuf);
    }
    #endif
    prEsInfo->pucIFrameBuf = NULL;

    return (MPV_DRV_OK);

}


INT32 MPV_SetIFrameBuf(UCHAR ucEsId, const MPV_DEC_I_FRAME_BUFF_T* prIFrameBuff)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;

    if (prIFrameBuff == NULL)
    {
        return (MPV_DRV_INV_SET_INFO);
    }

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    _rMpvPesInfo.fgSeqHdr = TRUE;
    _rMpvPesInfo.fgNonBuffer = TRUE;
    _rMpvPesInfo.ucEsId = ucEsId;
    _rMpvPesInfo.ucMpvId = VLD0;
    _rMpvPesInfo.u4FifoStart = PHYSICAL((UINT32)(prIFrameBuff->pucIFrameBufAddr));
    _rMpvPesInfo.u4FifoEnd = PHYSICAL(MPV_ALIGN_MASK(((UINT32)(prIFrameBuff->pucIFrameBufAddr) +
                              (prIFrameBuff->u4Size - 1)), MPV_ESFIFO_ALIGN));
    #ifdef CC_MT5395
    HalFlushDCacheMultipleLine(_rMpvPesInfo.u4FifoStart, (_rMpvPesInfo.u4FifoEnd - _rMpvPesInfo.u4FifoStart));
    #else
    HalFlushInvalidateDCache();
    #endif
    if(prVdecInfo->eCurFMT == VDEC_FMT_H264)
    {
        _rMpvPesInfo.ucPicType = (*(UCHAR*)VIRTUAL((UINT32)prIFrameBuff->pucIFrameBufAddr + 3));
        _rMpvPesInfo.u4VldReadPtr = PHYSICAL((UINT32)(prIFrameBuff->pucIFrameBufAddr) + 4);
    }
    else
    {
        _rMpvPesInfo.ucPicType= I_TYPE;
        _rMpvPesInfo.u4VldReadPtr = PHYSICAL((UINT32)(prIFrameBuff->pucIFrameBufAddr));
    }
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->fgIFrameDec = FALSE;

    if (VDEC_SendEs(&_rMpvPesInfo) != 1)
    {
        return (MPV_DRV_SET_FAILED);
    }

    return (MPV_DRV_OK);

}


INT32 MPV_SetIFrameDec(UCHAR ucEsId, const MPV_DEC_I_FRAME_BUFF_T* prIFrameBuff)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;

    if (prIFrameBuff == NULL)
    {
        return (MPV_DRV_INV_SET_INFO);
    }

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    _rMpvPesInfo.fgSeqHdr = TRUE;
    _rMpvPesInfo.fgNonBuffer = TRUE;
    _rMpvPesInfo.ucEsId = ucEsId;
    _rMpvPesInfo.ucMpvId = VLD0;
    if(prVdecInfo->eCurFMT == VDEC_FMT_H264)
    {
        _rMpvPesInfo.ucPicType = *(prIFrameBuff->pucIFrameBufAddr + 3);
        _rMpvPesInfo.u4VldReadPtr = PHYSICAL((UINT32)(prIFrameBuff->pucIFrameBufAddr) + 4);
    }
    else
    {
        _rMpvPesInfo.ucPicType= I_TYPE;
        _rMpvPesInfo.u4VldReadPtr = PHYSICAL((UINT32)(prIFrameBuff->pucIFrameBufAddr));
    }
    _rMpvPesInfo.u4FifoStart = PHYSICAL((UINT32)(prIFrameBuff->pucIFrameBufAddr));
    _rMpvPesInfo.u4FifoEnd = PHYSICAL(MPV_ALIGN_MASK(((UINT32)(prIFrameBuff->pucIFrameBufAddr) +
                                                                        (prIFrameBuff->u4Size - 1)), MPV_ESFIFO_ALIGN));

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->fgIFrameDec = TRUE;

    if (VDEC_SendEs(&_rMpvPesInfo) != 1)
    {
        return (MPV_DRV_SET_FAILED);
    }

    return (MPV_DRV_OK);

}


void MPV_FlushEsmQ(UCHAR ucEsId, BOOL fgAbortDecode)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    MPV_ES_INFO_T *prEsInfo;
    VDEC_INFO_T* prVdecInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);

    if((fgAbortDecode) && (prVdecInfo->eCurFMT == VDEC_FMT_MPV))
    {
        prEsInfo->fgAbortDecode = TRUE;
        #if 0
        #ifndef LOCK_TIMEOUT
        {
            UINT8 ucMpvId = 0;
            // make it timeout
            _MpvTimerTimeout(0, (void*)(UINT32)ucMpvId);
        }
        #endif
        #endif
    }

    LOG(6, "Flush Es for changing channel %d\n",ucEsId);
    _VDEC_LockFlushMutex(ucEsId);
    prVdecEsInfo->fgFlushEsmQ = TRUE;

    /*
    // move to container part
    FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);
    FBM_ReleaseDispQ(prVdecEsInfo->ucFbgId);
    FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);
    */

     _VDEC_UnlockFlushMutex(ucEsId);

    _VDEC_SendCmd(ucEsId, ((UINT32)VDEC_CMD_MAX));

    // Virtual Picture Header for Event Trigger
    _rMpvPesInfo.ucEsId = ucEsId;
    _rMpvPesInfo.ucMpvId = MPV_MAX_VLD;

    VERIFY(VDEC_SendEs(&_rMpvPesInfo) == 1);

    FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);
    FBM_ReleaseDispQ(prVdecEsInfo->ucFbgId);
    FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);

#if 0
    // release 1 frame to empty Q to avoid waiting too long at get_frame_buffer
    // Todo: change to free run mode
    if((prVdecInfo->eCurFMT == VDEC_FMT_H264) && (!prVdecEsInfo->fgMMPlayback))
    {
        //only header parse stage has chance to block in get_frm_buf
        //clear currrenc fbId, error handling if no displayQ in header parse stage (check "ClrBuf in AllocFBuf")
        if(prVdecInfo->eDecStatus == VDEC_DEC_HEADER_PARSE)
        {
            FBM_ReleaseOneFB(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucFbId);
            prVdecEsInfo->ucFbId = FBM_FB_ID_UNKNOWN;
        }
    }
    else
    {
        FBM_ReleaseOneFB(prVdecEsInfo->ucFbgId, FBM_FB_ID_UNKNOWN);
    }
#endif
#if 0
    // if empty Q cnt == 0
    FBM_GetEmptyQCount(prVdecEsInfo->ucFbgId, &ucEmptyCnt);
    if(ucEmptyCnt == 0)
    {
        ucDispCnt = FBM_CheckFrameBufferDispQ(prVdecEsInfo->ucFbgId);
        FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);
        if(ucDispCnt > 0)
        {
            ucFbId = FBM_GetFrameBufferFromDispQ(prVdecEsInfo->ucFbgId);
            FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
        }
        else
        {   //clear current buffer, nearest frame from flushed/corrupted data
            FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucFbId, FBM_FB_STATUS_EMPTY);
        }
        FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);
    }
#endif

#if 0
    VDP_SetCHFreeRun();
#endif
    // lock until flush done in main loop
    _VDEC_LockFlushSema(ucEsId);
    LOG(6, "Flush Es finish\n");
#if 0
    VDP_ClrCHFreeRun();
#endif
    //FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, ucEsId);
    //FBM_ResetGroup(prVdecEsInfo->ucFbgId);
    prEsInfo->fgAbortDecode = FALSE;
}


void MPV_SetEC(UCHAR ucEsId, UCHAR ucECMethod, UINT32 u4ErrThrsd,
                                BOOL fgDetectRefLost)
{
    MPV_ES_INFO_T *prEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prEsInfo->ucECMethod = ucECMethod;
    prEsInfo->u4ErrThrsd = u4ErrThrsd;
    prEsInfo->fgDetectRefLost = fgDetectRefLost;
}


void MPV_RegEventNotifyFunc(PFN_MPV_EVENT_NOTIFY pfFunc)
{
    VDEC_INFO_T *prVdecInfo;
    prVdecInfo = _VDEC_GetInfo();
    prVdecInfo->pfErrNotify = pfFunc;
}


void MPV_EventNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    VDEC_INFO_T *prVdecInfo;
    prVdecInfo = _VDEC_GetInfo();
    if (prVdecInfo->pfErrNotify )
    {
        prVdecInfo->pfErrNotify (u4Arg1, u4Arg2, u4Arg3, u4Arg4);
    }
}


BOOL MPV_IsDisplay(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if (FBM_ChkFrameBufferFlagIsr(prVdecEsInfo->ucFbgId, FBM_FLAG_DISPLAYING))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}


void MPV_UpdateVldRptr(UCHAR ucEsId)
{
    VDEC_ES_INFO_T* prVdecEsInfo;
    //UINT32 u4CurRptr;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    LOG(10, "ES(%d) DMX_UpdateVideoReadPointer  0x%x, 0x%x\n",
        ucEsId, prVdecEsInfo->u4VldReadPtr, prVdecEsInfo->u4DecReadPtr);

    // update read pointer address
    if (!DMX_UpdateVideoReadPointer(ucEsId, prVdecEsInfo->u4DecReadPtr,
        prVdecEsInfo->u4VldReadPtr))
    {
        LOG(10, "ES(%d) DMX_UpdateVideoReadPointer  0x%x, 0x%x\n",
            ucEsId, prVdecEsInfo->u4VldReadPtr, prVdecEsInfo->u4DecReadPtr);
    }
}


void MPV_UpdateVldWptr(UCHAR ucEsId, UINT32 u4VldWptr)
{
    //VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    //VDEC_ES_INFO_T *prVdecEsInfo;
    MPV_ES_INFO_T *prEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    //LOG(5,"W0: 0x%x, ", u4VldWptr);
    //prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    //prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);

    // We are picture base esm queue now.
    // just update it with available length
    prEsInfo->u4VldWptr = MPV_ALIGN_MASK(u4VldWptr , 0xf);
    prEsInfo->u4VldWptr += RWPTR_DIFF;
    #if 0
    if(prVdecEsInfo->fgMMPlayback)
    {
        // ToDo: handle EOS. Use below
        // prEsInfo->u4VldWptr = MPV_ALIGN_MASK(u4VldWptr , 0xf);
        // prEsInfo->u4VldWptr += RWPTR_DIFF;
        prEsInfo->u4VldWptr = (u4VldWptr & (~0xf));
    }
    else if (!prVdecEsInfoKeep->fgDmxPartialPes)
    {
        prEsInfo->u4VldWptr = MPV_ALIGN_MASK(u4VldWptr , 0xf);
        prEsInfo->u4VldWptr += RWPTR_DIFF;
        //LOG(5,"W: 0x%x\n", prEsInfo->u4VldWptr);
    }
    else
    {
        prEsInfo->u4VldWptr = (u4VldWptr & (~0xf));
    }
    #endif

    _MPV_VldSetWptr(ucEsId, prEsInfo->u4VldWptr);

    LOG(8, "MPV_UpdateVldWptr 0x%x\n", u4VldWptr);

}


void MPV_SetRR(UCHAR ucEsId, UINT32 u4Ratio)
{
    MPV_ES_INFO_T *prEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);

    prEsInfo->u4RRatio = u4Ratio;
    prEsInfo->fgRatioCLI = TRUE;

    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_RR);

    // Virtual Picture Header for Event Trigger
    _rMpvPesInfo.ucEsId = ucEsId;
    _rMpvPesInfo.ucMpvId = MPV_MAX_VLD;

    VERIFY(VDEC_SendEs(&_rMpvPesInfo));

}


void _MPV_SetRR(UCHAR ucEsId)
{
    MPV_ES_INFO_T *prEsInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

	FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_REDUCE_REF);
	SET_DECFLAG(prVdecEsInfo, DEC_FLAG_REDUCE_REF);
	SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
	prVdecEsInfo->fgCanDisp = FALSE;
	prEsInfo->fgFreeP = FALSE;
	prEsInfo->u4SeqCnt = 0;
	prEsInfo->fgChgFbgT = TRUE;

}

void MPV_SetMcMode(UCHAR ucEsId, BOOL fgMcPipe, BOOL fgMcFastAddr, BOOL fgMcNewFsm)
{
    MPV_ES_INFO_T *prEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);

    prEsInfo->fgMcPipe = fgMcPipe;
    prEsInfo->fgMcFastAddr = fgMcFastAddr;
    prEsInfo->fgMcNewFsm = fgMcNewFsm;

}


// we only have one dbk hardware
void MPV_SetDbk(BOOL fgEnable, BOOL fgEngrMode)
{
    UCHAR ucEsId;
    MPV_INFO_T *prMpvInfo;
    MPV_ES_INFO_T *prEsInfo;
    //VDEC_INFO_T *prVdecInfo;
//    VDEC_ES_INFO_T *prVdecEsInfo;

    //prVdecInfo = _VDEC_GetInfo();
    prMpvInfo = (MPV_INFO_T*)_MPV_GetMpvInfo();
    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ES0);
    //prVdecEsInfo = _VDEC_GetEsInfo(ES0);
    ASSERT(prEsInfo);

#ifdef MPV_SEMI_HOST
    prEsInfo->fgDisableDbkThrsd = TRUE;
#endif

    if (fgEnable)
    {
        prMpvInfo->fgDisableDbk = FALSE;
        //mark out it if you want to test dbk function
        // disable threshold -> always enable deblocking
        prEsInfo->fgDisableDbkThrsd = TRUE;

        if (fgEngrMode)
        {
            prMpvInfo->fgDbkEngrMode = TRUE;
        }
        else
        {
            prMpvInfo->fgDbkEngrMode = FALSE;
        }

        for (ucEsId = 0; ucEsId < MPV_MAX_ES; ucEsId++)
        {
            _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_DEBLOCK);

            // Virtual Picture Header for Event Trigger
            _rMpvPesInfo.ucEsId = ucEsId;
            _rMpvPesInfo.ucMpvId = MPV_MAX_VLD;

            VERIFY(VDEC_SendEs(&_rMpvPesInfo));
        }
    }
    else
    {
        prMpvInfo->fgDisableDbk = TRUE;
        prMpvInfo->fgDbkEngrMode = FALSE;
        //prVdecInfo->eCurFMT == VDEC_FMT_MPV;
        prEsInfo->fgDisableDbkThrsd = FALSE;

        for (ucEsId = 0; ucEsId < MPV_MAX_ES; ucEsId++)
        {
            MPV_CleanRRDbk(ucEsId);

            _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_MAX);

            // Virtual Picture Header for Event Trigger
            _rMpvPesInfo.ucEsId = ucEsId;
            _rMpvPesInfo.ucMpvId = MPV_MAX_VLD;

            VERIFY(VDEC_SendEs(&_rMpvPesInfo));
        }
    }

}


void MPV_SetDbkRegion(UCHAR ucEsId, UINT32 u4XStart, UINT32 u4XEnd,
                                                UINT32 u4YStart, UINT32 u4YEnd)
{
    MPV_ES_INFO_T *prEsInfo;
    MPV_INFO_T *prMpvInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prMpvInfo = (MPV_INFO_T*)_MPV_GetMpvInfo();

    prEsInfo->u4DbkXStart = u4XStart;
    prEsInfo->u4DbkXEnd = u4XEnd;
    prEsInfo->u4DbkYStart = u4YStart;
    prEsInfo->u4DbkYEnd = u4YEnd;
    prMpvInfo->u2DbkWidthT = 0;
    prMpvInfo->u2DbkHeightT = 0;
}


void MPV_SetDBkThrsd(BOOL fgDisable, UINT32 u4QpAvgThrsd, UINT32 u4DbkRatioThrsd,
                          UINT32 u4QpAvgThrsdP, UINT32 u4DbkRatioThrsdP, UINT32 u4MvThrsd)
{
    MPV_ES_INFO_T *prEsInfo;
    //VDEC_INFO_T *prVdecInfo;

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ES0);
    //prVdecInfo = _VDEC_GetInfo();

    prEsInfo->fgDisableDbkThrsd = fgDisable;

    if (!fgDisable)
    {
        prEsInfo->u4QpAvgThrsd = u4QpAvgThrsd;
        prEsInfo->u4DbkRatioThrsd = u4DbkRatioThrsd;
        prEsInfo->u4QpAvgThrsdP = u4QpAvgThrsdP;
        prEsInfo->u4DbkRatioThrsdP = u4DbkRatioThrsdP;
        prEsInfo->u4MvThrsd = u4MvThrsd;
    }
}


void MPV_SetDbkPara1(UCHAR ucEsId, UINT32 u4P1, UINT32 u4P2,UINT32 u4P3,UINT32 u4P4, UINT32 u4P5,
                                      UINT32 u4P6, UINT32 u4P7, UINT32 u4P8,UINT32 u4P9,UINT32 u4P10)
{
    MPV_ES_INFO_T *prEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);

    prEsInfo->u4P1 = u4P1;
    prEsInfo->u4P2 = u4P2;
    prEsInfo->u4P3 = u4P3;
    prEsInfo->u4P4 = u4P4;
    prEsInfo->u4P5 = u4P5;
    prEsInfo->u4P6 = u4P6;
    prEsInfo->u4P7 = u4P7;
    prEsInfo->u4P8 = u4P8;
    prEsInfo->u4P9 = u4P9;
    prEsInfo->u4P10 = u4P10;

}


void MPV_SetDbkPara2(UCHAR ucEsId, UINT32 u4P1, UINT32 u4P2,UINT32 u4P3,UINT32 u4P4, UINT32 u4P5,
                                       UINT32 u4P6, UINT32 u4P7, UINT32 u4P8,UINT32 u4P9,UINT32 u4P10)
{
    MPV_ES_INFO_T *prEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);

    prEsInfo->u4P11 = u4P1;
    prEsInfo->u4P12 = u4P2;
    prEsInfo->u4P13 = u4P3;
    prEsInfo->u4P14 = u4P4;
    prEsInfo->u4P15 = u4P5;
    prEsInfo->u4P16 = u4P6;
    prEsInfo->u4P17 = u4P7;
    prEsInfo->u4P18 = u4P8;
    prEsInfo->u4P19 = u4P9;
    prEsInfo->u4P20 = u4P10;

}


void MPV_SetEcSetting(UCHAR ucEsId, UINT32 u4Mode)
{
    MPV_ES_INFO_T *prEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prEsInfo->u4EcSetting= u4Mode;

}


void MPV_LockVld(UCHAR ucMpvId)
{
    VDEC_INFO_T *prVdecInfo;

    ASSERT(ucMpvId < MPV_MAX_VLD);

    prVdecInfo = _VDEC_GetInfo();

    VERIFY(x_sema_lock(prVdecInfo->arVdecVldInfo[ucMpvId].hVldSemaphore, X_SEMA_OPTION_WAIT) == OSR_OK);
}


void MPV_UnlockVld(UCHAR ucMpvId)
{
    VDEC_INFO_T *prVdecInfo;

    ASSERT(ucMpvId < MPV_MAX_VLD);

    prVdecInfo = _VDEC_GetInfo();

    VERIFY(x_sema_unlock(prVdecInfo->arVdecVldInfo[ucMpvId].hVldSemaphore) == OSR_OK);
}


BOOL MPV_ResetSearch(UCHAR ucMpvId, UINT32 u4FifoStart, UINT32 u4FifoEnd, UINT32 u4SearchStart ,UINT32 u4SearchEnd)
{
    BOOL fgRet;

    ASSERT(ucMpvId < MPV_MAX_VLD);

    fgRet = _MPV_ResetSearch(ucMpvId, u4FifoStart, u4FifoEnd, u4SearchStart, u4SearchEnd);

    return fgRet;
}


UINT32 MPV_SearchStartCode(UCHAR ucMpvId, UCHAR ucEsId, UINT32* pu4Addr)
{
    UINT32 u4Datain;

    ASSERT(ucMpvId < MPV_MAX_VLD);
    ASSERT(ucEsId < MPV_MAX_ES);

    u4Datain = _MPV_SearchStartCode(ucMpvId, ucEsId, pu4Addr);

    return (u4Datain);
}


BOOL MPV_SupportDbk(void)
{
    // ilii todo 5391
    /*if ((DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5382x64MB) ||
        (DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5382pSSx64MB) ||
        (DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5382px64MB))
    {
        return TRUE;
    }
    else*/
    {
        return FALSE;
    }

}

void MPV_GetDbkInfo(UCHAR ucEsId, BOOL* pfgDbkEnable,
    UINT16* pu2XStart, UINT16* pu2YStart, UINT16* pu2Width, UINT16* pu2Height)
{
    //MPV_ES_INFO_T *prEsInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    //VDEC_INFO_T *prVdecInfo;
    MPV_INFO_T *prMpvInfo;

    ASSERT(ucEsId < MPV_MAX_ES);
    ASSERT(pfgDbkEnable);
    ASSERT(pu2XStart);
    ASSERT(pu2YStart);
    ASSERT(pu2Width);
    ASSERT(pu2Height);

    //prVdecInfo = _VDEC_GetInfo();
    prMpvInfo = (MPV_INFO_T*)_MPV_GetMpvInfo();
    //prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    *pfgDbkEnable = (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK) && (!prMpvInfo->fgDisableDbk));
    *pu2XStart = prMpvInfo->u2DbkXStartT;
    *pu2YStart = prMpvInfo->u2DbkYStartT;
    *pu2Width = prMpvInfo->u2DbkWidthT;
    *pu2Height = prMpvInfo->u2DbkHeightT;

}


void MPV_SetDbkInfo(UCHAR ucEsId, BOOL fgDbkEnable,
    UINT16 u2XStart, UINT16 u2YStart, UINT16 u2Width, UINT16 u2Height)

{
    //MPV_ES_INFO_T *prEsInfo;
    //VDEC_INFO_T *prVdecInfo;
    MPV_INFO_T *prMpvInfo;

    ASSERT(ucEsId < MPV_MAX_ES);

    prMpvInfo = (MPV_INFO_T*)_MPV_GetMpvInfo();
    //prVdecInfo = _VDEC_GetInfo();
    //prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);

    if (fgDbkEnable)
    {
        prMpvInfo->fgDisableDbk = FALSE;
        prMpvInfo->u2DbkXStartT = u2XStart;
        prMpvInfo->u2DbkYStartT = u2YStart;
        prMpvInfo->u2DbkWidthT = u2Width;
        prMpvInfo->u2DbkHeightT = u2Height;

        for (ucEsId = 0; ucEsId < MPV_MAX_ES; ucEsId++)
        {

            _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_DEBLOCK);

            // Virtual Picture Header for Event Trigger
            _rMpvPesInfo.ucEsId = ucEsId;
            _rMpvPesInfo.ucMpvId = MPV_MAX_VLD;

            VERIFY(VDEC_SendEs(&_rMpvPesInfo));
        }
    }
    else
    {
        prMpvInfo->fgDisableDbk = TRUE;
        prMpvInfo->u2DbkXStartT = 0;
        prMpvInfo->u2DbkYStartT = 0;
        prMpvInfo->u2DbkWidthT = 0;
        prMpvInfo->u2DbkHeightT = 0;
        //prVdecInfo->eCurFMT == VDEC_FMT_MPV;

        for (ucEsId = 0; ucEsId < MPV_MAX_ES; ucEsId++)
        {
            _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_MAX);

            // Virtual Picture Header for Event Trigger
            _rMpvPesInfo.ucEsId = ucEsId;
            _rMpvPesInfo.ucMpvId = MPV_MAX_VLD;

            VERIFY(VDEC_SendEs(&_rMpvPesInfo));
        }
    }
}

VOID MPV_GetDbkLevel(UCHAR ucEsId, UINT16 *pu2Level) // 0 ~ 100
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    if(pu2Level == NULL)
    {
        LOG(3, "MPV_GetDbkLevel pu2Level NULL\n");
        return;
    }

    ASSERT(ucEsId < MPV_MAX_ES);

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if(prVdecEsInfoKeep->i2DBKAdjust == 0)
    {
        *pu2Level = 50;
    }
    else
    {
        *pu2Level = prVdecEsInfoKeep->u2DBKLevel;
    }
}

BOOL MPV_SetDbkLevel(UCHAR ucEsId, UINT16 u2Level) // 0 ~ 100
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    INT32 i4Tmp;

    LOG(3, "MPV_SetDbkLevel %d\n", u2Level);
    if(u2Level > 100)
    {
        return FALSE;
    }

    ASSERT(ucEsId < MPV_MAX_ES);

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    i4Tmp = (INT32)u2Level;
    i4Tmp -= 50;
    i4Tmp = -i4Tmp; // inverse
    i4Tmp = ((i4Tmp * MPV_DBK_LEVEL_MAX) / 50);

    prVdecEsInfoKeep->i2DBKAdjust = i4Tmp;
    prVdecEsInfoKeep->u2DBKLevel = u2Level;
    return TRUE;
}

VOID MPV_GetDbkValue(UCHAR ucEsId, UINT32 *pu4Value) // 0 ~ (MPV_DBK_VALUE_MAX-1)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
	MPV_ES_INFO_T *prMpvEsInfo;

    if(pu4Value == NULL)
    {
        LOG(1, "MPV_GetDbkValue pu4Value NULL\n");
        return;
    }

    ASSERT(ucEsId < MPV_MAX_ES);
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "MPV_GetDbkValue ucEsId %d\n", ucEsId);
        *pu4Value = 0;
        return;
    }

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
	prMpvEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);

    *pu4Value = prVdecEsInfoKeep->u4DBKValue;

    if(prVdecEsInfoKeep->u4DBKValue > 0)
    {
        LOG(1, "DbkValue %d P1~P4 %d %d %d %d\n",
            prVdecEsInfoKeep->u4DBKValue,
            prMpvEsInfo->u4P2,
            prMpvEsInfo->u4P3,
            prMpvEsInfo->u4P4,
            prMpvEsInfo->u4P5);
    }
    return;
}

BOOL MPV_SetDbkValue(UCHAR ucEsId, UINT32 u4Value) // 0 ~ (MPV_DBK_VALUE_MAX-1)
{
    #if 1
    // disable this function, only enable this function in MP branch
    UNUSED(ucEsId);
    UNUSED(u4Value);
    return TRUE;
    #else
    MPV_INFO_T *prMpvInfo;
	MPV_ES_INFO_T *prMpvEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    LOG(3, "MPV_SetDbkValue %d\n", u4Value);
    if(u4Value >= MPV_DBK_VALUE_MAX)
    {
        LOG(1, "MPV_SetDbkValue u4Value %d\n", u4Value);
        return FALSE;
    }

    ASSERT(ucEsId < MPV_MAX_ES);
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "MPV_SetDbkValue ucEsId %d\n", ucEsId);
        return FALSE;
    }

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
	prMpvEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prMpvInfo = (MPV_INFO_T*)_MPV_GetMpvInfo();

    prVdecEsInfoKeep->u4DBKValue = u4Value;
    if((prVdecEsInfoKeep->u4DBKValue == 0) ||
      (prVdecEsInfoKeep->u4DBKValue == 3)) // middle use default setting
    {
        prMpvInfo->fgDisableDbk = FALSE;
        prMpvInfo->u2DbkXStartT = 0;
        prMpvInfo->u2DbkYStartT = 0;
        prMpvInfo->u2DbkWidthT = 1000;
        prMpvInfo->u2DbkHeightT = 1000;
        prMpvEsInfo->u4P1 = 0;
        prMpvEsInfo->u4P2 = 0;
        prMpvEsInfo->u4P3 = 0;
        prMpvEsInfo->u4P4 = 0;
        prMpvEsInfo->u4P5 = 0;
    }
    else if(prVdecEsInfoKeep->u4DBKValue == 1) // disable
    {
        prMpvInfo->fgDisableDbk = TRUE;
        prMpvInfo->u2DbkXStartT = 0;
        prMpvInfo->u2DbkYStartT = 0;
        prMpvInfo->u2DbkWidthT = 0;
        prMpvInfo->u2DbkHeightT = 0;
        prMpvEsInfo->u4P1 = 0;
        prMpvEsInfo->u4P2 = 0;
        prMpvEsInfo->u4P3 = 0;
        prMpvEsInfo->u4P4 = 0;
        prMpvEsInfo->u4P5 = 0;
    }
    else
    {
        prMpvInfo->fgDisableDbk = FALSE;
        prMpvInfo->u2DbkXStartT = 0;
        prMpvInfo->u2DbkYStartT = 0;
        prMpvInfo->u2DbkWidthT = 1000;
        prMpvInfo->u2DbkHeightT = 1000;
        prMpvEsInfo->u4P1 = 1;
        prMpvEsInfo->u4P2 = _au4MpvDbkP1[prVdecEsInfoKeep->u4DBKValue][0];
        prMpvEsInfo->u4P3 = _au4MpvDbkP1[prVdecEsInfoKeep->u4DBKValue][1];
        prMpvEsInfo->u4P4 = _au4MpvDbkP1[prVdecEsInfoKeep->u4DBKValue][2];
        prMpvEsInfo->u4P5 = _au4MpvDbkP1[prVdecEsInfoKeep->u4DBKValue][3];
    }

    for (ucEsId = 0; ucEsId < MPV_MAX_ES; ucEsId++)
    {
        _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_MAX);

        // Virtual Picture Header for Event Trigger
        _rMpvPesInfo.ucEsId = ucEsId;
        _rMpvPesInfo.ucMpvId = MPV_MAX_VLD;

        VERIFY(VDEC_SendEs(&_rMpvPesInfo));
    }

    LOG(1, "DbkValue %d P1~P4 %d %d %d %d\n",
        prVdecEsInfoKeep->u4DBKValue,
        prMpvEsInfo->u4P2,
        prMpvEsInfo->u4P3,
        prMpvEsInfo->u4P4,
        prMpvEsInfo->u4P5);
    return TRUE;
    #endif
}

void MPV_QueryStatus(void)
{
    UCHAR ucEsId;
    MPV_ES_INFO_T *prEsInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    VDEC_INFO_T *prVdecInfo;
    MPV_INFO_T *prMpvInfo;
    PARAM_MPV_T *prMpvCounter;

    prVdecInfo = _VDEC_GetInfo();
    prMpvInfo = (MPV_INFO_T*)_MPV_GetMpvInfo();

    for (ucEsId = 0; ucEsId < MPV_MAX_ES; ucEsId++)
    {
        prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
        prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
        prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
        prMpvCounter = (PARAM_MPV_T*)prVdecEsInfo->prMpvCounter;

        #if 1
        if (prVdecInfo->eCurState != VDEC_ST_IDLE)
    	 {
            LOG(1, "DecMode(0x%x),RR(%d, %d),Racing(FullB:%d,HalfB:%d),Dbk(%d),DbkEng(%d), DBKThrsd(%d), PartialPes(%d), (%d,%d)\n",
                prEsInfo->u4DecMode, IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_REDUCE_REF), prEsInfo->u4RRatio,
                IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_RACING_FULLB),
                IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_RACING_HALFB),
                (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK) && (!prMpvInfo->fgDisableDbk)),
                prMpvInfo->fgDbkEngrMode,
                !prEsInfo->fgDisableDbkThrsd,
                prVdecEsInfoKeep->fgDmxPartialPes,
                prEsInfo->u4NoDbkCnt, prEsInfo->u4DbkCnt);

			if ((prMpvCounter->u4PsrDropNs != 0) ||
				(prMpvCounter->u4RetrieveNs != 0) ||
				(prMpvCounter->u4EsmSkipNs != 0) ||
				(prMpvCounter->u4VpsrDropNs != 0) ||
				(prMpvCounter->u4DecOkNs != 0) ||
				(prMpvCounter->u4TimeOutNs != 0) ||
				(prMpvCounter->u4ErrDropNs != 0)||
				(prMpvCounter->u4EsmQCnt!= 0))
			{
				LOG(0, "MPV(%d) Receive(%d) OK(%d) Skip(%d) Drop(%d) Timeout(%d) Q(%d) ",
				ucEsId,  prMpvCounter->u4RetrieveNs, prMpvCounter->u4DecOkNs,
				prMpvCounter->u4EsmSkipNs, prMpvCounter->u4VpsrDropNs,
				prMpvCounter->u4TimeOutNs, prMpvCounter->u4EsmQCnt);

				if (prMpvCounter->u4DecOkNs != 0)
				{
					if (prMpvCounter->u4HaveI == 0)
					{
						LOG(0, "(%s/", "No_I");
					}
					else if (prMpvCounter->u4NoIAfterSeqH== 1)
					{
						LOG(0, "(%s/", "No_I_AfterSeqHdr");
					}
					else
					{
						LOG(0, "(%d.%06d/", prMpvCounter->rIDeltaTime.u4Seconds, prMpvCounter->rIDeltaTime.u4Micros);
					}

					LOG(0, "%dx%d", prMpvCounter->u2HSize, prMpvCounter->u2VSize);

					if (prMpvCounter->ucPicStruct == 3)
					{
						LOG(0, "/%s", "Frame");
					}
					else
					{
						LOG(0, "/%s", "Field");
					}

					switch (prMpvCounter->ucFrameRate)
					{
						case 1:
							LOG(0, "/%s)\n", "23.976");
							break;
						case 2:
							LOG(0, "/%s)\n", "24");
							break;
						case 3:
							LOG(0, "/%s)\n", "25");
							break;
						case 4:
							LOG(0, "/%s)\n", "29.97");
							break;
						case 5:
							LOG(0, "/%s)\n", "30");
							break;
						case 6:
							LOG(0, "/%s)\n", "50");
							break;
						case 7:
							LOG(0, "/%s)\n", "59.94");
							break;
						case 8:
							LOG(0, "/%s)\n", "60");
							break;
						default:
							LOG(0, ")\n");
							break;
					}
				}
				else
				{
					LOG(0, "\n");
				}
			}
        /*
        #ifdef MPV_TESTCODE
            LOG(1, "Mc(pipe:%d,Fastaddr:%d, NewFsm:%d), EcMode(%d) Half543(%d%d%d%d%d)\n",
                prEsInfo->fgMcPipe, prEsInfo->fgMcFastAddr, prEsInfo->fgMcNewFsm,
                prEsInfo->u4EcSetting, prEsInfo->fgBit5, prEsInfo->fgBit4, prEsInfo->fgBit3, prEsInfo->fgHalfFltC, prEsInfo->fgHalfFltY);
        #endif
        */
        }
        #else // QQ use this
        LOG(1, "I_Interval(%d.%06d)\n", prEsInfo->prMpvCounter->rIDeltaTime.u4Seconds,
            prEsInfo->prMpvCounter->rIDeltaTime.u4Micros);
        #endif
    }
}


MPV_VIDEO_FORMAT_T  MPV_GetVideoFormat(UCHAR ucEsId)
{
    MPV_ES_INFO_T *prEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);

    if (prEsInfo->ucMpegVer == MPEG2)
    {
        return MPV_FORMAT_MPEG2;
    }
    else if (prEsInfo->ucMpegVer == MPEG1)
    {
        return MPV_FORMAT_MPEG1;
    }

    return MPV_FORMAT_MAX;
}


void MPV_GetVideoInfo(UCHAR ucEsId, UINT32* pu4HResolution, UINT32* pu4VResolution,
                            INT32* pi4PrgScan, MPV_ALL_ASPECT_RATIO_T* pe4Ratio, UINT32* pu4FrmRat)
{
    MPV_ES_INFO_T *prEsInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    FBM_SEQ_HDR_T *prFbmSeqHdr;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    if((pu4HResolution == NULL) ||
        (pu4VResolution == NULL) ||
        (pi4PrgScan == NULL) ||
        (pe4Ratio == NULL) ||
        (pu4FrmRat == NULL))
    {
        LOG(1, "MPV_GetVideoInfo null pointer\n");
        return;
    }

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if (!prEsInfo->fgVideoLock)
    {
    	return;
    }

    if (prVdecEsInfo->ucFbgId == FBM_FBG_ID_UNKNOWN)
    {
        return;
    }

    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prVdecEsInfo->ucFbgId);

    if (!prFbmSeqHdr)
    {
        return;
    }

    *pu4HResolution = (UINT32)prFbmSeqHdr->u2HSize;
    *pu4VResolution = (UINT32)prFbmSeqHdr->u2VSize;
    *pi4PrgScan = (INT32)prFbmSeqHdr->fgProgressiveSeq;
    *pe4Ratio = _DecideAspectRatio(prFbmSeqHdr->ucAspRatInf,
                         prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize);
    switch (prFbmSeqHdr->ucFrmRatCod)
    {
        case MPEG_FRAME_RATE_24_:
            *pu4FrmRat= 23970;
            break;
        case MPEG_FRAME_RATE_24:
            *pu4FrmRat = 24000;
            break;
        case MPEG_FRAME_RATE_25:
            *pu4FrmRat = 25000;
            break;
        case MPEG_FRAME_RATE_30_:
            *pu4FrmRat = 29970;
            break;
        case MPEG_FRAME_RATE_30:
            *pu4FrmRat = 30000;
            break;
        case MPEG_FRAME_RATE_50:
            *pu4FrmRat = 50000;
            break;
        case MPEG_FRAME_RATE_60_:
            *pu4FrmRat = 59940;
            break;
        case MPEG_FRAME_RATE_60:
            *pu4FrmRat = 60000;
            break;
        default:
            *pu4FrmRat = 0;
            break;
    }

}


BOOL MPV_IsLock(UCHAR ucEsId)
{
    MPV_ES_INFO_T *prEsInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    PARAM_MPV_T* prMpvCounter;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prMpvCounter = (PARAM_MPV_T*)prVdecEsInfo->prMpvCounter;

//    LOG(1, "MPV_IsLock (%d) (%d)\n", prEsInfo->u4LastGetLockRxPacket, prEsInfo->u4LastGetLockDecOkPacket);

    if ((prMpvCounter->u4DecOkNs == prEsInfo->u4LastGetLockDecOkPacket) &&
        (prMpvCounter->u4RetrieveNs == prEsInfo->u4LastGetLockRxPacket))
    {
        // no packet input and output

        HAL_TIME_T rCurrentTime;
        HAL_TIME_T rDeltaTime;

        HAL_GetTime(&rCurrentTime);
        HAL_GetDeltaTime(&rDeltaTime, &(prEsInfo->rLastGetLockTime), &rCurrentTime);

        if (rDeltaTime.u4Seconds != 0)
        {
            // reset video lock
            prEsInfo->fgVideoLock = FALSE;
        }

//        LOG(1, "MPV_IsLock no packet (%d, %d)\n", rDeltaTime.u4Seconds, rDeltaTime.u4Micros);
    }
    else
    {
        HAL_GetTime(&(prEsInfo->rLastGetLockTime));
    }

    prEsInfo->u4LastGetLockDecOkPacket = prMpvCounter->u4DecOkNs;
    prEsInfo->u4LastGetLockRxPacket = prMpvCounter->u4RetrieveNs;

    if (prEsInfo->fgVideoLock)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


BOOL  MPV_RegAVDecCb(UCHAR ucEsId, PFN_MPV_AVDEC_CB pfAVDecCb)
{
    MPV_EXT_INFO_T *prExtInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        return FALSE;
    }

    if (pfAVDecCb == NULL)
    {
        return FALSE;
    }

    prExtInfo = (MPV_EXT_INFO_T*)_MPV_GetExtInfo();
    ASSERT(prExtInfo != NULL);

    prExtInfo->pfAVDecCb[ucEsId] = pfAVDecCb;

    return TRUE;
}


void MPV_AVDecCb(UCHAR ucEsId, MPV_AVDEC_STATUS_T* prStatus,
                        MPV_AVDEC_STATUS_MASK_T eMask)
{
    MPV_EXT_INFO_T *prExtInfo;

    ASSERT(prStatus);
    ASSERT(ucEsId < MPV_MAX_ES);

    prExtInfo = (MPV_EXT_INFO_T*)_MPV_GetExtInfo();

    if (prExtInfo->pfAVDecCb[ucEsId])
    {
        if (eMask == AVDEC_STATUS_VIDEOLOCK)
        {
            prStatus->i4VideoLock = TRUE;
        }
        else if (eMask == AVDEC_STATUS_VIDEOINFO)
        {
            MPV_GetVideoInfo(ucEsId, &(prStatus->rVideoInfo.u4HResolution),
                &(prStatus->rVideoInfo.u4VResolution), &(prStatus->rVideoInfo.i4ProgressScan),
                &(prStatus->rVideoInfo.eRatio), &(prStatus->rVideoInfo.u4frameRate));
        }
        else
        {
            ASSERT(0);
        }

        prExtInfo->pfAVDecCb[ucEsId](prStatus, eMask);
    }

}


BOOL  MPV_RegUserDataCb(PFN_MPV_USERDATA_CB pfUserDataCb)
{
    MPV_EXT_INFO_T *prExtInfo;

    if (pfUserDataCb == NULL)
    {
        return FALSE;
    }

    prExtInfo = (MPV_EXT_INFO_T*)_MPV_GetExtInfo();
    ASSERT(prExtInfo != NULL);
    prExtInfo->pfUserDataCb = pfUserDataCb;

    return TRUE;
}


void MPV_StartUserData(UCHAR ucEsId)
{
    MPV_EXT_INFO_T *prExtInfo;

    ASSERT(ucEsId < MPV_MAX_ES);

    prExtInfo = (MPV_EXT_INFO_T*)_MPV_GetExtInfo();
    ASSERT(prExtInfo != NULL);

    prExtInfo->fgUserData[ucEsId] = TRUE;

}

void MPV_StopUserData(UCHAR ucEsId)
{
    MPV_EXT_INFO_T *prExtInfo;

    ASSERT(ucEsId < MPV_MAX_ES);

    prExtInfo = (MPV_EXT_INFO_T*)_MPV_GetExtInfo();
    ASSERT(prExtInfo != NULL);

    prExtInfo->fgUserData[ucEsId] = FALSE;
}

void MPV_StartPesUserData(UCHAR ucEsId)
{
    MPV_EXT_INFO_T *prExtInfo;

    ASSERT(ucEsId < MPV_MAX_ES);

    prExtInfo = (MPV_EXT_INFO_T*)_MPV_GetExtInfo();
    ASSERT(prExtInfo != NULL);

    prExtInfo->fgPesUserData[ucEsId] = TRUE;

}

void MPV_StopPesUserData(UCHAR ucEsId)
{
    MPV_EXT_INFO_T *prExtInfo;

    ASSERT(ucEsId < MPV_MAX_ES);

    prExtInfo = (MPV_EXT_INFO_T*)_MPV_GetExtInfo();
    ASSERT(prExtInfo != NULL);

    prExtInfo->fgPesUserData[ucEsId] = FALSE;
}

void MPV_EnableAfd(UCHAR ucEsId)
{
    MPV_EXT_INFO_T *prExtInfo;

    ASSERT(ucEsId < MPV_MAX_ES);

    prExtInfo = (MPV_EXT_INFO_T*)_MPV_GetExtInfo();
    ASSERT(prExtInfo != NULL);

    prExtInfo->fgAfd[ucEsId] = TRUE;
}

void MPV_DisableAfd(UCHAR ucEsId)
{
    MPV_EXT_INFO_T *prExtInfo;

    ASSERT(ucEsId < MPV_MAX_ES);

    prExtInfo = (MPV_EXT_INFO_T*)_MPV_GetExtInfo();
    ASSERT(prExtInfo != NULL);

    prExtInfo->fgAfd[ucEsId] = FALSE;
}
#if 0
void MPV_SetSdUserDataFilter(UCHAR ucEsId, BOOL fgFitlerOn, UINT32 u4FilterMask)
{
    MPV_EXT_INFO_T *prExtInfo;

    ASSERT(ucEsId < MPV_MAX_ES);

    prExtInfo = __VDEC_GetSdalInfo(prVdecInfo);
    ASSERT(prExtInfo != NULL);

    prExtInfo->fgSdUserDataFilter[ucEsId] = fgFitlerOn;
    prExtInfo->u4FilterMask[ucEsId] = u4FilterMask;

}


void MPV_GetSdUserDataFilter(UCHAR ucEsId, BOOL* pfgFitlerOn, UINT32* pu4FilterMask)
{
    MPV_EXT_INFO_T *prExtInfo;

    ASSERT(ucEsId < MPV_MAX_ES);
    ASSERT(pfgFitlerOn);
    ASSERT(pu4FilterMask);

    prExtInfo = _VDEC_GetSdalInfo(prVdecInfo);
    ASSERT(prExtInfo != NULL);

    *pfgFitlerOn = prExtInfo->fgSdUserDataFilter[ucEsId];
    *pu4FilterMask = prExtInfo->u4FilterMask[ucEsId];

}
#endif

BOOL MPV_GetUserData(UCHAR* pucUserDataTmpBuf, UINT32 u4Rptr, UINT32 u4DataLen)
{
    MPV_EXT_INFO_T *prExtInfo;

    ASSERT(pucUserDataTmpBuf);

    prExtInfo = (MPV_EXT_INFO_T*)_MPV_GetExtInfo();

    if(prExtInfo == NULL)
    {
        return FALSE;
    }

    if (u4DataLen > 0)
    {
        if ((u4Rptr < MAX_USERDATA_SIZE) &&
            (u4DataLen < MAX_USERDATA_SIZE) &&
            ((u4Rptr + u4DataLen) - 1) < MAX_USERDATA_SIZE)
        {
            x_memcpy(pucUserDataTmpBuf, &(prExtInfo->aucUserDataBuf[u4Rptr]), u4DataLen);
        }
        else
        {
            ASSERT((MAX_USERDATA_SIZE - u4Rptr) > 0);
            x_memcpy(pucUserDataTmpBuf, &(prExtInfo->aucUserDataBuf[u4Rptr]), (MAX_USERDATA_SIZE - u4Rptr));

            x_memcpy(pucUserDataTmpBuf + (MAX_USERDATA_SIZE - u4Rptr) ,
                prExtInfo->aucUserDataBuf, u4DataLen - (MAX_USERDATA_SIZE -u4Rptr));
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}


#ifdef MPV_VIRTUAL_CC
void MPV_PlayVirtualCc(UCHAR ucEsId, BOOL fgEnable)
{
    MPV_ES_INFO_T *prEsInfo;

    ASSERT(ucEsId < MPV_MAX_ES);

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prEsInfo->fgVirtualCc = fgEnable;
}
#endif


#ifdef __MODEL_slt__
void MPV_CountPicNs(UINT32 u4StartPts, UINT32 u4EndPts)
{
    MPV_ES_INFO_T *prEsInfo;

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ES0);
    prEsInfo->u4StartPts = u4StartPts;
    prEsInfo->u4EndPts = u4EndPts;

    // restart
    if (prEsInfo->fgCountPicNs == FALSE)
    {
        prEsInfo->u4PicNs = 0;
        prEsInfo->fgCountPicFinish = FALSE;
    }
}


UINT32 MPV_GetPicNs(void)
{
    MPV_ES_INFO_T *prEsInfo;

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ES0);

    return (prEsInfo->u4PicNs);
}
#endif


#ifdef MPV_SEMI_HOST
void _MPV_SetSaveNo(UINT32 u4StartSaveNo, UINT32 u4EndSaveNo)
{
    MPV_ES_INFO_T *prEsInfo;

    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ES0);
    prEsInfo->u4StartSaveNo = u4StartSaveNo;
    prEsInfo->u4EndSaveNo = u4EndSaveNo;

}
#endif


void _MPV_ClearEsInfo(UCHAR ucEsId)
{
	//VDEC_INFO_T *prVdecInfo;
    MPV_INFO_T *prMpvInfo;
	MPV_ES_INFO_T *prMpvEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

	//prVdecInfo = _VDEC_GetInfo();
	prMpvEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ES0);
    prMpvInfo = (MPV_INFO_T*)_MPV_GetMpvInfo();

	//if(prVdecInfo->eCurFMT == VDEC_FMT_MPV)
	{
	    x_memset((void*)prMpvEsInfo, 0, sizeof(MPV_ES_INFO_T));

	    prMpvEsInfo->pu4IntraMatrix = (UINT32*)(((MPV_READ_ADDR_ALIGN - 1)
	        + (UINT32) prMpvEsInfo->aucMatrix) & (~(MPV_READ_ADDR_ALIGN - 1)));

	    prMpvEsInfo->pu4NonIntraMatrix = (UINT32*)((UINT32)
	        &prMpvEsInfo->pu4IntraMatrix[MPV_MATRIX_SIZE]);

	    prMpvEsInfo->fgDetectIntra = TRUE;

	    //default No AV Sync
	    prMpvEsInfo->ucECMethod = MPV_EC_METHOD;
	    prMpvEsInfo->u4ErrThrsd = MPV_ERR_THRSD;
	    prMpvEsInfo->fgDetectRefLost = MPV_DETECT_REF_MISSING;

	    //DBK threshold
	    prMpvEsInfo->u4QpAvgThrsd = MPV_DBK_QP_THRSD;
	    prMpvEsInfo->u4QpAvgThrsdP = MPV_DBK_QP_THRSD_P;
	    prMpvEsInfo->u4DbkRatioThrsd = MPV_DBK_RATIO_THRSD;
	    prMpvEsInfo->u4DbkRatioThrsdP = MPV_DBK_RATIO_THRSD_P;
	    prMpvEsInfo->u4MvThrsd = MPV_DBK_MV_THRSD;

        if(prVdecEsInfoKeep->u4DBKValue > 0)
        {
            if(prVdecEsInfoKeep->u4DBKValue == 3) // middle use default setting
            {
                prMpvInfo->fgDisableDbk = FALSE;
                prMpvInfo->u2DbkXStartT = 0;
                prMpvInfo->u2DbkYStartT = 0;
                prMpvInfo->u2DbkWidthT = 1000;
                prMpvInfo->u2DbkHeightT = 1000;
                prMpvEsInfo->u4P1 = 0;
                prMpvEsInfo->u4P2 = 0;
                prMpvEsInfo->u4P3 = 0;
                prMpvEsInfo->u4P4 = 0;
                prMpvEsInfo->u4P5 = 0;
            }
            else if(prVdecEsInfoKeep->u4DBKValue == 1) // disable
            {
                prMpvInfo->fgDisableDbk = TRUE;
                prMpvInfo->u2DbkXStartT = 0;
                prMpvInfo->u2DbkYStartT = 0;
                prMpvInfo->u2DbkWidthT = 0;
                prMpvInfo->u2DbkHeightT = 0;
                prMpvEsInfo->u4P1 = 0;
                prMpvEsInfo->u4P2 = 0;
                prMpvEsInfo->u4P3 = 0;
                prMpvEsInfo->u4P4 = 0;
                prMpvEsInfo->u4P5 = 0;
            }
            else
            {
                prMpvInfo->fgDisableDbk = FALSE;
                prMpvInfo->u2DbkXStartT = 0;
                prMpvInfo->u2DbkYStartT = 0;
                prMpvInfo->u2DbkWidthT = 1000;
                prMpvInfo->u2DbkHeightT = 1000;
                prMpvEsInfo->u4P1 = 1;
                prMpvEsInfo->u4P2 = _au4MpvDbkP1[prVdecEsInfoKeep->u4DBKValue][0];
                prMpvEsInfo->u4P3 = _au4MpvDbkP1[prVdecEsInfoKeep->u4DBKValue][1];
                prMpvEsInfo->u4P4 = _au4MpvDbkP1[prVdecEsInfoKeep->u4DBKValue][2];
                prMpvEsInfo->u4P5 = _au4MpvDbkP1[prVdecEsInfoKeep->u4DBKValue][3];
            }
            if(prVdecEsInfoKeep->u4DBKValue > 0)
            {
                LOG(1, "DbkValue %d P1~P4 %d %d %d %d\n",
                    prVdecEsInfoKeep->u4DBKValue,
                    prMpvEsInfo->u4P2,
                    prMpvEsInfo->u4P3,
                    prMpvEsInfo->u4P4,
                    prMpvEsInfo->u4P5);
            }
        }

        _MPV_ResetUserDataIP();
	}
}

void _MPV_ClearMpvCc(UCHAR ucEsId)
{
    x_memset((void*)&_arMpvCc[ucEsId], 0, sizeof(MPV_CC_T));
}

void* _MPV_GetMpvCc(UCHAR ucEsId)
{
    return ((void*)&_arMpvCc[ucEsId]);
}

void _MPV_SkipPic(UCHAR ucMpvId, UCHAR ucEsId)
{
    //MPV_ES_INFO_T *prEsInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    MPV_CC_T    *prMpvCc;
    PARAM_MPV_T *prMpvCounter;

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
    prMpvCc = _MPV_GetMpvCc(ucEsId);
    prMpvCounter = (PARAM_MPV_T *)prVdecEsInfo->prMpvCounter;

    if (prMpvCc->fgPlayCc)
    {
        INT32 i4Ret;

        MPV_LockVld(ucMpvId);
        i4Ret = _MPV_ExtractCc(ucMpvId, ucEsId);
        MPV_UnlockVld(ucMpvId);
        UNUSED(i4Ret);
    }
    prVdecEsInfo->u4DecReadPtr = prVdecEsInfo->u4VldReadPtr;

    prMpvCounter->u4EsmSkipNs++;

    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC))
    {
        UCHAR ucDecFbId;

        ucDecFbId = FBM_GetDecFrameBuffer(prVdecEsInfo->ucFbgId);
        LOG_MFBM("MFBM Set Empty f, %d\n", ucDecFbId);
        UNUSED(_MPV_CalcPts(ucEsId, ucDecFbId));
        FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucDecFbId, FBM_FB_STATUS_EMPTY);

        #ifdef CC_MPV_DEBLOCKING
            if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK))
            {
                LOG_MFBM("MFBM Set Empty g, %d\n", prVdecEsInfo->ucDbkFbId);
                FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId,
                    prVdecEsInfo->ucDbkFbId, FBM_FB_STATUS_EMPTY);
            }
        #endif

        TOGGLE_DECFLAG(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC);
        LOG(9, "Q 2nd Fld S\n");
    }

#ifndef MPV_VIRTUAL_PRS
    if (!DMX_UpdateVideoReadPointer(ucEsId, prVdecEsInfo->u4DecReadPtr,
        prVdecEsInfo->u4VldReadPtr))
    {

        LOG(10, "ES(%d) DMX_UpdateVideoReadPointer F1 0x%x\n", ucEsId,
            prVdecEsInfo->u4DecReadPtr);
    }
#endif
}

BOOL MPV_EsFlwCtrl(UCHAR ucEsId, UCHAR* pucMpvId, INT8 i1EsBufStatus, UCHAR ucSkipMode)
{
    BOOL fgRet;
    MPV_ES_INFO_T* prEsInfo;
    VDEC_ES_INFO_T* prVdecEsInfo;
    //INT32 i4Count;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        ucEsId = (MPV_MAX_ES-1);
    }

    if(!pucMpvId)
    {
        LOG(1, "MPV_EsFlwCtrl pucMpvId NULL\n");
        return FALSE;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);

    fgRet = _MPV_EsFlwCtrl(ucEsId, pucMpvId, i1EsBufStatus, ucSkipMode);

    #ifndef MPV_SEMI_HOST
    if(prEsInfo->fgClearRRDbk)
    {
        _MPV_CleanRRDbk(ucEsId);
        prEsInfo->fgClearRRDbk = FALSE;
    }
    #endif

    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I))
    {
        FBM_InvalidateRefFrameBuffer(prVdecEsInfo->ucFbgId);
        CLR_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);

        if((prVdecEsInfo->fgMMPlayback) &&
           (prVdecEsInfo->fgDetectIntra))
        {
            // because we invalidate reference frame, we should restart to get a sync.
            prEsInfo->fgDetectIntra = TRUE;
            prVdecEsInfo->fgDetectIntra = FALSE;
        }

        if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC))
        {
            TOGGLE_DECFLAG(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC);
        }
    }

    return fgRet;
}
//skip ES if return TRUE
BOOL _MPV_EsFlwCtrl(UCHAR ucEsId, UCHAR* pucMpvId, INT8 i1EsBufStatus, UCHAR ucSkipMode)
{
    UCHAR ucPicType;
    INT32 i4TempRef;
    UINT64 u8Offset;
    //UCHAR ucFbId;
    UINT32 u4PTS;
    BOOL fgNotDisplay;
    BOOL fgRet;
    VDEC_INFO_T* prVdecInfo;
    VDEC_ES_INFO_T* prVdecEsInfo;
    FBM_PIC_HDR_T *prFbmPicHdr;
    //MPV_ES_INFO_T* prEsInfo;

    fgNotDisplay = FALSE;
    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    //prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    ucPicType = prVdecEsInfo->ucPicType;
    u4PTS = 0;

    if ((!prVdecInfo) || (!prVdecEsInfo) || (!pucMpvId))
    {
        LOG(1, "_MPV_EsFlwCtrl prVdecInfo prVdecEsInfo pucMpvId NULL\n");
        return FALSE;
    }

    #ifndef MPV_VIRTUAL_PRS
    if ((prVdecInfo->eCurState == VDEC_ST_STOP) ||
        (prVdecInfo->eCurState  == VDEC_ST_IDLE))
    {
        LOG(10, "STOP/IDLE mode\n");
        _MPV_SkipPic(*pucMpvId, (UCHAR)prVdecEsInfo->u4EsId);
        *pucMpvId = VDEC_MAX_VLD;
        return TRUE;
    }

    if(ucSkipMode == IMode)
    {
        //if (is I frame) or (is second field and previous field is I), can't skip
        //return FALSE
        //else
        //return TRUE
    }

    if (prVdecEsInfo->fgSendedEos)
    {
        LOG(6, "SendedEos already\n");
        *pucMpvId = VDEC_MAX_VLD;
        return TRUE;
    }

    // ask mpeg decode to skip one picture due to repeat
    // if ESM-Q Ns > Underflow Threshold, otherwise, ignore
    if (ucPicType == B_TYPE)
    {
        if (prVdecEsInfo->ucFbgId != FBM_FBG_ID_UNKNOWN)
        {
            //when freeze mode with racing mode, decode reference picture only.
            if (FBM_ChkFrameBufferFlag(prVdecEsInfo->ucFbgId , FBM_FLAG_FREEZE))
            {
                if (FBM_ChkFrameBufferFlag(prVdecEsInfo->ucFbgId , FBM_FLAG_RACING_FULL_B))
                {
                    _MPV_SkipPic(*pucMpvId, (UCHAR)prVdecEsInfo->u4EsId);
                    *pucMpvId = VDEC_MAX_VLD;
                    return TRUE;
                }
                else if (FBM_ChkFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_RACING_HALF_B))
                {
                    ASSERT(0);
                }
            }

            if (FBM_ChkFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SKIP_PIC))
            {
                if(!prVdecEsInfo->fgMMPlayback)
                {
                    // DTV skip picture in _MPV_EsFlwCtrl.
                    // MM, we have to calculate PTS,
                    // check this after picture header parsed (_VParser).
                    FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SKIP_PIC);

                    if (prVdecEsInfo->u2Count > prVdecEsInfo->u2UdfNs)
                    {
                        LOG(3, "Q S VLD EsFlwCtrl(%d) Es(%d) G(%d) Q(%d) RP(0x%08x)\n",
                            *pucMpvId, prVdecEsInfo->u4EsId, prVdecEsInfo->ucFbgId, prVdecEsInfo->u2Count,
                            prVdecEsInfo->u4VldReadPtr);

                        _MPV_SkipPic(*pucMpvId, (UCHAR)prVdecEsInfo->u4EsId);

                        // [LOG] Video plane ask mpeg to drop 1 picture
                        //SYSLOG(98, prVdecEsInfo->u4EsId, ucPicType, prVdecEsInfo->u2Count);
                        *pucMpvId = VDEC_MAX_VLD;
                        return TRUE;
                    }
                }

                // [LOG] Video plane ask mpeg to drop 1 picture, mpeg don't drop since Q is near empty
                //SYSLOG(97, prVdecEsInfo->u4EsId, ucPicType, prVdecEsInfo->u2Count);
            }
            else if (FBM_ChkFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SKIP_PIC_NO_PTS))
            {
                if (prVdecEsInfo->u4PTS == 0)
                {
                    FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SKIP_PIC_NO_PTS);

                    if (prVdecEsInfo->u2Count > prVdecEsInfo->u2UdfNs)
                    {
                        LOG(3, "Q S P VLD(%d) Es(%d) G(%d) Q(%d) RP(0x%08x)\n",
                        *pucMpvId, prVdecEsInfo->u4EsId, prVdecEsInfo->ucFbgId, prVdecEsInfo->u2Count,
                        prVdecEsInfo->u4VldReadPtr);

                        _MPV_SkipPic(*pucMpvId, (UCHAR)prVdecEsInfo->u4EsId);

                        // [LOG] Video plane ask mpeg to drop 1 picture
                        //SYSLOG(98, prVdecEsInfo->u4EsId, ucPicType, prVdecEsInfo->u2Count);
                        *pucMpvId = VDEC_MAX_VLD;
                        return TRUE;
                    }
                }

                // [LOG] Video plane ask mpeg to drop 1 picture, mpeg don't drop since Q is near empty
                //SYSLOG(97, prVdecEsInfo->u4EsId, ucPicType, prVdecEsInfo->u2Count);
            }
        }

        if (prVdecEsInfo->ucDropBNs > 0)
        {
            //prVdecEsInfo->ucDropBNs--;

            LOG(3, "Q %d R (%d)\n", prVdecEsInfo->u4EsId, prVdecEsInfo->ucDropBNs);

            _MPV_SkipPic(*pucMpvId, (UCHAR)prVdecEsInfo->u4EsId);

            // [LOG] Drop 1 picture due to ESM Q full
            *pucMpvId = VDEC_MAX_VLD;
            return TRUE;
        }

    }
    #else
    LOG(8, "ES(%d) R(%x) --\n",
        prVdecEsInfo->u4EsId,
        prVdecEsInfo->u4VldReadPtr);
    #endif

    if (prVdecEsInfo->ucFbgId != FBM_FBG_ID_UNKNOWN)
    {
        // Check if reset is required
        if (FBM_ChkFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_RESET))
        {
            FBM_ResetGroup(prVdecEsInfo->ucFbgId);
            FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_RESET);
            FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_NOT_READY);
            if(prVdecInfo->eCurState != VDEC_ST_PLAY_I_FRAME)
            {
                FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_CLEAR_FRC_PRM);
            }

            if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC))
            {
                _MPV_SkipPic(*pucMpvId, (UCHAR)prVdecEsInfo->u4EsId);
                *pucMpvId = VDEC_MAX_VLD;
            }
        }
        #ifndef FBM_VIRTUAL_DISPLAY
        // Check if display is ready, skip picture if NOT
        if ((!prVdecEsInfo->fgMMPlayback) &&
            (!FBM_ChkFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_DISP_READY)))
        {
            LOG(3, "Disp Not Ready\n");
            _MPV_SkipPic(*pucMpvId, (UCHAR)prVdecEsInfo->u4EsId);

            *pucMpvId = VDEC_MAX_VLD;
        }
        else
        #endif
        if ((!IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC)) &&
            (prVdecInfo->eCurState != VDEC_ST_PLAY_I_FRAME))
        {
            if (ucPicType != B_TYPE)
            {
                UCHAR ucBRefFbId;
                UCHAR ucFRefFbId;
                //FBM_PIC_HDR_T *prFbmPicHdr;

                FBM_GetRefFrameBuffer(prVdecEsInfo->ucFbgId, &ucFRefFbId, &ucBRefFbId);

                if (ucBRefFbId != FBM_FB_ID_UNKNOWN)
                {
                    UCHAR ucFbStatus;

                    ucFbStatus = FBM_GetFrameBufferStatus(prVdecEsInfo->ucFbgId,
                                                                            ucBRefFbId);

                    if(ucFbStatus == FBM_FB_STATUS_READY)
                    {
                        if ((prVdecEsInfo->fgAllIntraSlice) && (!prVdecEsInfo->fgCanDisp))
                        {
                            if (prVdecEsInfo->u4WaitP == MPV_WAIT_P)
                            {
                                prVdecEsInfo->fgCanDisp = TRUE;
                                prVdecEsInfo->fgBisOk = TRUE;
                                LOG(5, "First Ref CanDisp, B is OK\n");
                            }
                            else
                            {
                                prVdecEsInfo->u4WaitP++;
                            }
                        }

                        if (!prVdecEsInfo->fgCanDisp)
                        {
                            LOG_MFBM("MFBM Set Empty h, %d\n", ucBRefFbId);
                            FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId,
                                FBM_FB_STATUS_EMPTY);
                        #ifdef CC_MPV_DEBLOCKING
                            if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK))
                            {
                                LOG_MFBM("MFBM Set Empty i, %d\n", prVdecEsInfo->ucDbkRefId);
                                FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId,
                                    prVdecEsInfo->ucDbkRefId, FBM_FB_STATUS_EMPTY);
                                prVdecEsInfo->ucDbkRefId = FBM_FB_ID_UNKNOWN;
                                LOG_MFBM("MFBM ucDbkRefId 0xFF c\n");
                            }
                        #endif

                        }
                        else
                        {
                            if ((!prVdecEsInfo->fgSetStartPts) && (prVdecEsInfo->ucSyncMode != (UCHAR)VID_SYNC_MODE_NONE))
                            {
                                prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucBRefFbId);
                                if (!prVdecEsInfo->fgMMPlayback)
                                {
                                    if(prFbmPicHdr)
                                    {
                                        if (STC_SetStartPts(AV_SYNC_TARGET_VDO, prVdecEsInfo->ucStcId, prFbmPicHdr->u4PTS) != STC_VALID)
                                        {
                                            LOG(3, "STC_SetStartPts Invalid\n");
                                        }
                                        LOG(5, "STC_SetStartPts 0x%x\n", prFbmPicHdr->u4PTS);
                                    }
                                    prVdecEsInfo->fgSetStartPts = TRUE;
                                }
                            }

                            #ifdef MPV_REPEAT_TEST
                            if(!prVdecEsInfo->fgParseOnRepeat)
                            {
                            #endif

                            #ifdef CC_MPV_DEBLOCKING
                            if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLAG_DBK) && (!prVdecEsInfo->fgRefNoDbk))
                            {
                                if (prVdecEsInfo->ucDbkRefId == FBM_FB_ID_UNKNOWN) //change normal mode to DBK mode
                                {
                                    u4PTS = _MPV_CalcPts(ucEsId, ucBRefFbId);
                                    prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucBRefFbId);
                                    if(prFbmPicHdr)
                                    {
                                        i4TempRef = prFbmPicHdr->i4TemporalRef;
                                        u8Offset = prFbmPicHdr->u8Offset;
                                    }
                                    else
                                    {
                                        i4TempRef = 0;
                                        u8Offset = 0;
                                        LOG(3, "_MPV_EsFlwCtrl 1 prFbmPicHdr NULL");
                                    }
                                    fgNotDisplay = VDEC_IsNotDisplay(ucEsId, u4PTS, i4TempRef, u8Offset);

                                    LOG_MFBM("MFBM Set R Disp a, %d, %d\n", ucBRefFbId,
                                    	(FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucBRefFbId))->i4TemporalRef);

                                    if ((IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I)) ||
                                        (fgNotDisplay))
                                    {
                                        FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId,
                                            ucBRefFbId, FBM_FB_STATUS_EMPTY);
                                    }
                                    else
                                    {
                                        FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId,
                                            ucBRefFbId, FBM_FB_STATUS_DISPLAYQ);
                                        prVdecEsInfo->u4DisplayQPicCnt++;
                                    }
                                }
                                else
                                {
                                    u4PTS = _MPV_CalcPts(ucEsId, prVdecEsInfo->ucDbkRefId);
                                    prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucDbkRefId);
                                    if(prFbmPicHdr)
                                    {
                                        i4TempRef = prFbmPicHdr->i4TemporalRef;
                                        u8Offset = prFbmPicHdr->u8Offset;
                                    }
                                    else
                                    {
                                        i4TempRef = 0;
                                        u8Offset = 0;
                                        LOG(3, "_MPV_EsFlwCtrl 3 fgNotDisplay prFbmPicHdr NULL");
                                    }
                                    fgNotDisplay = VDEC_IsNotDisplay(ucEsId, u4PTS, i4TempRef, u8Offset);

                                    LOG_MFBM("MFBM Set R Disp b, %d, %d\n", prVdecEsInfo->ucDbkRefId,
                                    	(FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucDbkRefId))->i4TemporalRef);

                                    if ((IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I)) ||
                                        (fgNotDisplay))
                                    {
                                        FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId,
                                            prVdecEsInfo->ucDbkRefId, FBM_FB_STATUS_EMPTY);
                                    }
                                    else
                                    {
                                        FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId,
                                            prVdecEsInfo->ucDbkRefId, FBM_FB_STATUS_DISPLAYQ);
                                        prVdecEsInfo->u4DisplayQPicCnt++;
                                    }
                                    prVdecEsInfo->ucDbkRefId = FBM_FB_ID_UNKNOWN;
                                    LOG_MFBM("MFBM ucDbkRefId 0xFF d\n");
                                }
                            }
                            else
                            #endif
                            {
                                u4PTS = _MPV_CalcPts(ucEsId, ucBRefFbId);
                                prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucBRefFbId);
                                if(prFbmPicHdr)
                                {
                                    i4TempRef = prFbmPicHdr->i4TemporalRef;
                                    u8Offset = prFbmPicHdr->u8Offset;
                                }
                                else
                                {
                                    i4TempRef = 0;
                                    u8Offset = 0;
                                    LOG(3, "_MPV_EsFlwCtrl 3 fgNotDisplay prFbmPicHdr NULL");
                                }
                                fgNotDisplay = VDEC_IsNotDisplay(ucEsId, u4PTS, i4TempRef, u8Offset);

                                LOG_MFBM("MFBM Set R Disp c, %d, %d\n", ucBRefFbId,
                                	(FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucBRefFbId))->i4TemporalRef);

                                if ((IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I)) ||
                                    (fgNotDisplay))
                                {
                                    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId,
                                        ucBRefFbId, FBM_FB_STATUS_EMPTY);
                                }
                                else
                                {
                                    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId,
                                        ucBRefFbId, FBM_FB_STATUS_DISPLAYQ);
                                    prVdecEsInfo->u4DisplayQPicCnt++;
                                }
                                #ifdef VDP_RACE_LOG
                                VDP_FrcRacingLog('Q');
                                #endif
                            }

                            fgRet = VDEC_CheckStopStatus(ucEsId, u4PTS, i4TempRef);
                            if(fgRet)
                            {
                                *pucMpvId = VDEC_MAX_VLD;
                                return TRUE;
                            }

                            #ifdef MPV_REPEAT_TEST
                            }
                            #endif

                        }
                    }
                    else
                    if ((ucFbStatus != FBM_FB_STATUS_LOCK) &&
                        (ucFbStatus != FBM_FB_STATUS_DISPLAYQ) &&
                        (ucFbStatus != FBM_FB_STATUS_EMPTY))
                    {
                        // [Special Case]
                        // if last R frame decode fail, current R frame will use
                        // previous R frame as forward reference.

                        LOG(1, "\nF Reference is NOT ready !\n");
                        ASSERT(0);

                    }

                }

            }

        }
    }

    UNUSED(i1EsBufStatus);
    return FALSE;
}

BOOL _MPV_WaitRandomAP(const void* pvPesInfo)
{
    UCHAR ucEsId;
    //UCHAR ucMpvId;

    VDEC_INFO_T* prVdecInfo;
    VDEC_ES_INFO_T* prVdecEsInfo;
    PARAM_MPV_T *prMpvCounter;
    VDEC_PES_INFO_T* prPesInfo = (VDEC_PES_INFO_T*)pvPesInfo;

    ASSERT(prPesInfo != NULL);
    //ucMpvId = prPesInfo->ucMpvId;
    ucEsId = prPesInfo->ucEsId;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecInfo = _VDEC_GetInfo();
    prMpvCounter = (PARAM_MPV_T *)prVdecEsInfo->prMpvCounter;

        // Drop Picture Before I Picture is found
        // In file playback, even in pause stage, we have to check drop case
        // For DTV, we keep original behavior.
        // (Actually, there is no VDEC_ST_IDLE in DTV)
        if (!((prVdecInfo->eCurState == VDEC_ST_STOP) ||
              ((prVdecInfo->eCurState == VDEC_ST_IDLE) &&
               (!prVdecEsInfo->fgMMPlayback))))
        {
            if (prVdecEsInfo->fgMMPlayback && prPesInfo->fgEos)
            {
                // must send EOS to EsmQ
                return FALSE;
            }
            if (!prVdecEsInfo->ucSequenceHdrFound)
            {
                if (!prPesInfo->fgSeqHdr)
                {
                    //if((!prVdecEsInfo->fgMMPlayback) ||
                    //    ((prVdecEsInfo->u4MMSpeed >= 1000) &&
                    //    (prVdecEsInfo->u4MMSpeed < 4000)))
                    //{
                        LOG(6, "Q D VLD(%d) Es(%d) Pic(%d) RP(0x%08x)\n",
                               prPesInfo->ucMpvId, ucEsId, prPesInfo->ucPicType,
                               prPesInfo->u4VldReadPtr);
                    //}

                    if (!DMX_UpdateVideoReadPointer(prPesInfo->ucEsId,
                        prPesInfo->u4VldReadPtr, prPesInfo->u4VldReadPtr))
                    {
                        LOG(10, "ES(%d) DMX_UpdateVideoReadPointer F0\n", prPesInfo->ucEsId);
                    }
                    
                    #ifdef TIME_MEASUREMENT
                    if(prMpvCounter->u4PsrDropNs == 0)
                    {
                        TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, "REC_1st_Frame");
                    }
                    #endif
                    prMpvCounter->u4PsrDropNs++;

                    #ifdef CC_VDEC_FMT_DETECT
                    if((prPesInfo->u4DmxFrameType == 0) ||
                        MPV_PIC_VALID_TYPE(prPesInfo->u4DmxFrameType))
                    {
                        return TRUE;
                    }
                    else
                    {
                        return FALSE;   // for format detection
                    }

                    #else
                    return TRUE;
                    #endif
                }
                else
                {
                    {
                        HAL_TIME_T rTime;
                        HAL_GetTime(&rTime);
                        LOG(3, "Es(%d) Receive First Pic. Time(%d.%06d)\n", ucEsId,
                            rTime.u4Seconds, rTime.u4Micros);
                        #ifdef CHANNEL_CHANGE_LOG
                        LOG(0," %u.%06u s [AV SYNC] 6 Es Receive First I pic.PTS=(0x%x)\n", rTime.u4Seconds, rTime.u4Micros, prPesInfo->u4PTS);
                        #endif
                        #ifdef TIME_MEASUREMENT
                        if(prMpvCounter->u4PsrDropNs == 0)
                        {
                            TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, "REC_1st_Frame");
                        }
                        {
                            CHAR szBuf[15];
                            SPrintf(szBuf, "REC_1st_I(%3d)", prMpvCounter->u4PsrDropNs);
                            TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, szBuf);
                        }
                        #endif
                    }

                    prVdecEsInfo->ucSequenceHdrFound = 1;
                    prVdecEsInfo->fgChgThrsd = TRUE;
                    prVdecEsInfo->u4TotalPicSize = 0;
                    prVdecEsInfo->u4PicCnt = 0;
                    prVdecEsInfo->u4LastVldRptr = 0;
                }
            }
        }
        return FALSE;
}

#ifndef MPV_VIRTUAL_PRS
UCHAR _MPV_HandleOverFlow(UCHAR ucMpvId, UCHAR ucEsmQId, HANDLE_T hMutex,
     HANDLE_T hMsgQueue, const void* pvPesInfo)
{
    BOOL fgFirstPic;
    UCHAR ucValid = 0;
    UINT16 u2MsgQIdx;
    UINT32 u4Idx;
    UINT32 u4Finish;
    SIZE_T zMsgSize;
    VDEC_PES_INFO_T* prPesInQ;
    VDEC_PES_INFO_T* prPesInfo = (VDEC_PES_INFO_T*)pvPesInfo;

    ASSERT(prPesInfo != NULL);

    if (x_sema_lock(hMutex, X_SEMA_OPTION_WAIT) != OSR_OK)
    {
        return 0;
    }

    if (prPesInfo->ucPicType == B_TYPE)
    {
        LOG(3, "Q %d (CB)\n", prPesInfo->ucEsId);
        VERIFY(x_sema_unlock(hMutex) == OSR_OK);
        return 0;
    }

    prPesInQ = (VDEC_PES_INFO_T *) x_mem_alloc (
                          (VDEC_MSG_Q_SIZE + 1) * sizeof(VDEC_PES_INFO_T));

    if (prPesInQ == NULL)
    {
        ASSERT(0);
        LINT_SUPPRESS_BRACE(527);
        VERIFY(x_sema_unlock(hMutex) == OSR_OK);
        return 0;
    }

    u4Idx = 0;
    u4Finish = 0;
    zMsgSize = sizeof(VDEC_PES_INFO_T);
    fgFirstPic = TRUE;

    while (u4Finish == 0)
    {
        if (x_msg_q_receive(&u2MsgQIdx, (void *)&(prPesInQ[u4Idx]), &zMsgSize,
                &hMsgQueue, 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
        {
            if (prPesInQ[u4Idx].ucPicType == B_TYPE)
            {
                LOG(3, "Q %d (QB)\n", prPesInfo->ucEsId);
                ucValid = 1;
                u4Finish = 1;
                if (fgFirstPic)
                {
                    _MPV_SkipPic(ucMpvId, ucEsmQId);
                }
            }

            if (fgFirstPic)
            {
                fgFirstPic = FALSE;
            }
        }
        else
        {
            LOG(3, "Q %d (F)\n", prPesInfo->ucEsId);
            ucValid = 0;
            u4Finish = 1;
        }

        if (u4Finish)
        {
            UINT32 u4SendIdx;

            // put back all previous message (0, 1, ..., u4Idx - 1)
            // the last receive message (which is B) will drop (u4Idx)
            for (u4SendIdx = 0; u4SendIdx < u4Idx; u4SendIdx++)
            {
                if (x_msg_q_send(hMsgQueue, (void *)&(prPesInQ[u4SendIdx]),
                    sizeof(VDEC_PES_INFO_T), 254) != OSR_OK)
                {
                    LOG(5, "EsmQ Full 0\n");
                }
            }

            // send the current message
            if (ucValid)
            {
                if (x_msg_q_send(hMsgQueue, (void *)prPesInfo,
                    sizeof(VDEC_PES_INFO_T), 255) != OSR_OK)
                {
                    LOG(5, "EsmQ Full 1\n");
                }
            }
        }

        u4Idx++;
    }

    x_mem_free((void *)prPesInQ);

    VERIFY(x_sema_unlock(hMutex) == OSR_OK);
    return ucValid;
}
#endif

UINT32 _MPV_CalcPts(UCHAR ucEsId, UCHAR ucFbId)
{
    UINT32 u4PTSOffset;
    VDEC_ES_INFO_T* prVdecEsInfo = 0;
    FBM_SEQ_HDR_T *prFbmSeqHdr = 0;
    FBM_PIC_HDR_T *prFbmPicHdr = 0;

    ASSERT(ucEsId < MPV_MAX_ES);

    if(ucFbId == FBM_FB_ID_UNKNOWN)
    {
        LOG(3, "_MPV_CalcPts FBM_FB_ID_UNKNOWN\n");
        return 0;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    if(prVdecEsInfo == NULL)
    {
        LOG(3, "_MPV_CalcPts prVdecEsInfo null\n");
        return 0;
    }

    if(!prVdecEsInfo->fgMMPlayback)
    {
        return 0;
    }

    if(prVdecEsInfo->u4FrameRate == 0)
    {
        LOG(3, "_MPV_CalcPts u4FrameRate 0\n");
        //ASSERT(0);
        return 0;
    }
    u4PTSOffset = (MPV_STC_CLOCK / prVdecEsInfo->u4FrameRate);

    prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucFbId);
    if(prFbmPicHdr == NULL)
    {
        LOG(3, "_MPV_CalcPts prFbmPicHdr null\n");
        return 0;
    }

    if(prVdecEsInfo->ucFbgId == FBM_FBG_ID_UNKNOWN)
    {
        LOG(3, "_MPV_CalcPts FBM_FBG_ID_UNKNOWN null\n");
        return 0;
    }
    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prVdecEsInfo->ucFbgId);
    if(prFbmSeqHdr == NULL)
    {
        LOG(3, "_MPV_CalcPts prFbmSeqHdr null\n");
        return 0;
    }

    // 20081120 Isaac: copy from vdp_frc.c _VdpBaseVSyncNs(...)
    // 20080209 William: SGP, StickyVideo2.trp
    // Handle RFF, we should not consider frame rate
    if (prFbmSeqHdr->fgProgressiveSeq)        // Progressive Sequence
    {
        if (!prFbmPicHdr->fgProgressiveFrm)  // Progressive Frame
        {
            // Progressive Sequence with Interlace Frame ?! (Bad Edit?)
            if (prFbmPicHdr->fgRepFirstFld)
            {
                // According to 13818-2, If Progressive Frame = 0, RFF shall be 0.
                LOG(3, "Warning! Progressive Frame = 0 and RFF = 1\n");
            }
        }

        // Acoording to 13818-2, top_field_first section

        if ((prFbmPicHdr->fgRepFirstFld) && (prFbmPicHdr->fgTopFldFirst))
        {
            // 3 Frame for PRGS_SEQ + REPEAT_1ST_Fld + TOP_FLS_FIRST
            u4PTSOffset *= 3;

            /*
            if (!prFrcPrm->ucProgressiveOut)
            {
                // although according to spec, it should play 6 vsync
                // but we only encounter streams that will error if we play 6 vsync, but will correct if we play 3 vsync,
                // this is the stream error, but we modify our code to cover the stream error

                u4PTSOffset /= 2;
            }
            */
        }
        else
        if (prFbmPicHdr->fgRepFirstFld)
        {
            // 2 Frame for PRGS_SEQ + REPEAT_1ST_Fld
            u4PTSOffset *= 2;

            /*
            if (!prFrcPrm->ucProgressiveOut)
            {
                // although according to spec, it should play 4 vsync
                // but we only encounter streams that will error if we play 4 vsync, but will correct if we play 2 vsync,
                // this is the stream error, but we modify our code to cover the stream error

                u4PTSOffset /= 2;
            }
            */
        }
        else
        {
            // 1 Frame for PRGS_SEQ
        }
    }
    else                                // Interlace Sequence
    {
        // According to 13818-2, repeat_first_field section
        if (prFbmPicHdr->fgRepFirstFld)
        {
            if (prFbmPicHdr->fgProgressiveFrm == 0)  // Progressive Frame
            {
                // According to 13818-2, If Progressive Frame = 0, RFF shall be 0.
                LOG(3, "Warning! Progressive Frame = 0 and RFF = 1\n");
            }

            u4PTSOffset *= 3;
            u4PTSOffset /= 2;
        }
    }

    if(prFbmPicHdr->u4PTS == 0)
    {
        if(prVdecEsInfo->u4PTSLast != 0)
        {
            prFbmPicHdr->u4PTS = prVdecEsInfo->u4PTSLast;
            prVdecEsInfo->u4PTSLast = prVdecEsInfo->u4PTSLast + u4PTSOffset;

            LOG(6, "PTS-Cal, Es%d, Pic Type = %d , TemporalRef = %d , PTS = 0x%x\n",
                        ucEsId, prFbmPicHdr->ucPicCdTp, prFbmPicHdr->i4TemporalRef,
                        prFbmPicHdr->u4PTS);
        }
        else
        {
            LOG(5, "Wait for first PTS, ucPicCdTp %d\n", prFbmPicHdr->ucPicCdTp);
        }
    }
    else
    {
        if(prVdecEsInfo->u4PTSLast > prFbmPicHdr->u4PTS)
        {
            LOG(3, "PTS-Overlap, PTS = 0x%x -> 0x%x \n",
                        prVdecEsInfo->u4PTSLast, prFbmPicHdr->u4PTS);
        }
        LOG(6, "PTS-Org, Es%d, Pic Type = %d , TmpRef = %d , PTS = 0x%x\n",
                    ucEsId, prFbmPicHdr->ucPicCdTp, prFbmPicHdr->i4TemporalRef,
                    prFbmPicHdr->u4PTS);
        prVdecEsInfo->u4PTSLast = prFbmPicHdr->u4PTS + u4PTSOffset;
    }

    if(prFbmPicHdr->u4PTS > (prVdecEsInfo->u4TotlaTimeOffset +
        prVdecEsInfo->u4TotalTime))
    {
        prFbmPicHdr->u4TotlaTimeOffset =
            (prFbmPicHdr->u4PTS - prVdecEsInfo->u4TotalTime);
        prVdecEsInfo->u4TotlaTimeOffset = prFbmPicHdr->u4TotlaTimeOffset;
    }
    prFbmPicHdr->u8OffsetDisp = prVdecEsInfo->u8OffsetLast;
    return prFbmPicHdr->u4PTS;
}


#if !defined(CC_MTK_LOADER) && defined(CC_BOOT_VDEC_LOGO)

#define LOGOVDEC_IFRAME_TIMEOUT              5000

#define LOGOVDEC_TIMEOUT    0
#define LOGOVDEC_FINISH     1
#define LOGOVDEC_ERROR      2


static BOOL     _bPlayingIframe = FALSE;
static UINT32   _u4IFrameResult = 0;
static HANDLE_T _hIframeSema = 0;

void _LOGOVDEC_VDEC_Nfy(
    void*               pvNfyTag,
    VDEC_DEC_COND_T      eNfyCond,
    UINT32              u4Data1,
    UINT32              u4Data2)
{
    LOG(5, "_LOGOVDEC_VDEC_Nfy\n");

    if((eNfyCond == VDEC_COND_I_FRAME_SET_BUFF_DONE) ||
        (eNfyCond == VDEC_COND_I_FRAME_DONE))
    {
        LOG(3, " - Vdec job done\n");
        _u4IFrameResult = LOGOVDEC_FINISH;
        VERIFY(x_sema_unlock(_hIframeSema) == OSR_OK);
    }
    else if(eNfyCond == VDEC_COND_ERROR)
    {
        LOG(1, " - Vdec job error\n");
        _u4IFrameResult = LOGOVDEC_ERROR;
        VERIFY(x_sema_unlock(_hIframeSema) == OSR_OK);
    }
}

void _LOGOVDEC_VDEC_EVENT_NOTIFY(
    UINT32 u4Arg1,
    UINT32 u4Arg2,
    UINT32 u4Arg3,
    UINT32 u4Arg4)
{
    if(u4Arg2 == VDEC_COND_THUMBNAIL_DONE)
    {
        //Printf(" - Vdec thumbnail done\n");
        _u4IFrameResult = LOGOVDEC_FINISH;
        VERIFY(x_sema_unlock(_hIframeSema) == OSR_OK);
    }
    else
    {
        //Printf(" - Vdec thumbnail error\n");
        _u4IFrameResult = LOGOVDEC_ERROR;
        VERIFY(x_sema_unlock(_hIframeSema) == OSR_OK);
    }
}

//-----------------------------------------------------------------------------
/** MPV_OsdDisplay(): Setup OSD driver.
 *  This function is going to setup osd driver with bmp info.
 *
 *  @param  u4BmpAddr       The bmp data pointer.
 *  @param  u4Width         The width info of the bmp data.
 *  @param  u4Height        The height info of the bmp data.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
UINT32 MPV_OsdDisplay(UINT32 u4OsdPlane, UINT32 u4Scaler, UINT32 u4ColorMode, UINT32 u4BmpAddr, UINT32 u4Width, UINT32 u4Height)
{
    static BOOL fgLogoInit = FALSE;
    static UINT32 u4RegionList = (UINT32)-1;
    static UINT32 u4Region;
    UINT32 u4BmpPitch;
    UINT32 u4PanelWidth, u4PanelHeight;
    INT32 ret;
    INT32 i;

    //Printf("Color:%d BmpAddr:0x%08x Width:%d Height:%d\n", u4ColorMode, u4BmpAddr, u4Width, u4Height);
    
    //OSD_Init();

    u4PanelWidth = PANEL_GetPanelWidth();
    u4PanelHeight = PANEL_GetPanelHeight();
    UNUSED(u4PanelWidth);
    UNUSED(u4PanelHeight);
    
    if(fgLogoInit == TRUE)
    {
    	ret = OSD_PLA_FlipTo(u4OsdPlane, u4RegionList);
    }
    else
    {
        //Printf("Panel %d x %d \n", u4PanelWidth, u4PanelHeight);

        ret = OSD_RGN_LIST_Create(&u4RegionList);
        if (ret != OSD_RET_OK) return 1;
        u4BmpPitch = 0;
        OSD_GET_PITCH_SIZE(u4ColorMode, u4Width, u4BmpPitch); // to set u4BmpPitch by u4ColorMode and u4Width.
        ret = OSD_RGN_Create(&u4Region, u4Width, u4Height, (void *)u4BmpAddr,
                                u4ColorMode, u4BmpPitch, 0, 0, u4Width, u4Height);
        if (ret != OSD_RET_OK) return 2;
        ret = OSD_RGN_Insert(u4Region, u4RegionList);
        if (ret != OSD_RET_OK) return 3;
        ret = OSD_PLA_FlipTo(u4OsdPlane, u4RegionList);
        if (ret != OSD_RET_OK) return 4;

#if 0   // original bmp size.
        if (u4PanelWidth > u4Width)
        {
            ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_X, (u4PanelWidth - u4Width) >> 1);
        }
        else
        {
            ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_X, 0);
        }
        if (ret != OSD_RET_OK) return 5;

        if (u4PanelHeight > u4Height)
        {
            ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_Y, (u4PanelHeight - u4Height) >> 1);
        }
        else
        {
            ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_Y, 0);
        }
        if (ret != OSD_RET_OK) return 6;
#endif
    	fgLogoInit=TRUE;
    }

    // scale size.
    ret = OSD_SC_Scale(u4Scaler, 1, u4Width, u4Height, u4PanelWidth, u4PanelHeight);
    if (ret != OSD_RET_OK) return 7;
        
    OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, 0);
    OSD_RGN_Set(u4Region, OSD_RGN_MIX_SEL, (UINT32)OSD_BM_REGION);   

    ret = OSD_PLA_Enable(u4OsdPlane, TRUE);
    if (ret != OSD_RET_OK) return 8;    
 
    for(i=0;i<16;i++)
    {
        x_thread_delay(62);
        OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, ((i+1)*16-1));   
    }
    x_thread_delay(1000);
    for(i=16;i>0;i--)
    {
        OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, (i*16-1));   
        x_thread_delay(62);
    }
    OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, 0);

    return 0;
}

BOOL MPV_GetIFrameBufferAddr(UCHAR ucEsId, VDP_CAPTURE_INTO_T* prCapInfo)
{
    FBM_SEQ_HDR_T* prFbmSeqHdr;
    FBM_PIC_HDR_T* prFbmPicHdr;

    VDEC_ES_INFO_T *prVdecEsInfo;
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if(!prVdecEsInfo)
    {
        Printf("prVdecEsInfo = 0!\n");
        return FALSE;
    }

    prCapInfo->u4AddrY = 0;
    prCapInfo->u4AddrC = 0;
    FBM_GetFrameBufferAddr(prVdecEsInfo->ucFbgId,
        prVdecEsInfo->ucFbId, &(prCapInfo->u4AddrY), &(prCapInfo->u4AddrC));

    FBM_GetFrameBufferSize(prVdecEsInfo->ucFbgId,
        &(prCapInfo->u4MemHSize), &(prCapInfo->u4MemVSize));

    if((prCapInfo->u4AddrY == 0) ||
       (prCapInfo->u4AddrC == 0) ||
       (prCapInfo->u4MemHSize == 0) ||
       (prCapInfo->u4MemVSize == 0))
    {
        Printf("size = 0!\n");
        return FALSE;
    }

    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prVdecEsInfo->ucFbgId);
    prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucFbId);

    if (prFbmSeqHdr == NULL)
    {
        Printf("prFbmSeqHdr = 0!\n");
        return FALSE;
    }
    else
    {
        /* Thumbnail usage: Image resizer will use this as pitch. */
        // because we do not enter b2r, we don't need to check resize case
        // but because we already assign origina information in picture header
        // and target information to sequence header, we have to use picture header in this case
        if (prFbmPicHdr && prFbmSeqHdr->fgResizeSmallPic)
        {
            prCapInfo->u4MemHSize = (UINT32)prFbmPicHdr->u4PicWidthPitch;

            prCapInfo->u4HSize = (UINT32)prFbmPicHdr->u4PicWidth;
            prCapInfo->u4VSize = (UINT32)prFbmPicHdr->u4PicHeight;
        }
        else
        {
            prCapInfo->u4MemHSize = (UINT32)prFbmSeqHdr->u2LineSize;

            prCapInfo->u4HSize = (UINT32)prFbmSeqHdr->u2OrgHSize;
            prCapInfo->u4VSize = (UINT32)prFbmSeqHdr->u2OrgVSize;
        }

        prCapInfo->u4RRMode = (UINT32)prFbmSeqHdr->fgRRYEn;
    }

    return TRUE;
}

#endif

BOOL MPV_ShowLogo(VOID)
{
    BOOL fgRet = TRUE;
#if !defined(CC_MTK_LOADER) && defined(CC_BOOT_VDEC_LOGO)
    UCHAR ucEsId = ES0;
    VDEC_ES_INFO_T *prVdecEsInfo;
    FBM_POOL_T* prFbmPool;
    INT32 i4Ret;
    VDEC_DEC_NFY_INFO_T rNfyInfo;
    MPV_DEC_I_FRAME_BUFF_T rMpvBuf;
    VDP_THUMBNAIL_INFO_T rCapDstInfo;
    VDP_CAPTURE_INTO_T rSrcInfo;
    MPV_VDEC_LOGO_CTRL_UNION_T rVdecBootLogo;
    UINT8   ucDataType = 0;
    UINT32  u4DataSize = 0;
    UINT32  u4DataAddr = 0;

    x_memset(&rCapDstInfo, 0, sizeof(rCapDstInfo));
    x_memset(&rSrcInfo, 0, sizeof(rSrcInfo));

    if((DRVCUST_OptGet(eDrvLogoEnable)==0) ||
       (DRVCUST_OptGet(eDrvLogoVdecEnable)==0))
    {
        //LOG(1, "Driver Logo Disabled!\n");
        return TRUE;
    }

    rVdecBootLogo.aucReg[0] = bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_CTRL);

    if ((rVdecBootLogo.rField.ucDefault) &&
        (DRVCUST_OptGet(eDrvLogoVdecDefault)))
    {
        ucDataType = (DRVCUST_OptGet(eDrvLogoVdecMode));
        u4DataAddr = (DRVCUST_OptGet(eDrvLogoVdecDataAddr));
        u4DataSize = (DRVCUST_OptGet(eDrvLogoVdecDataSize));

        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
        if(!prFbmPool)
        {
            LOG(1, "prFbmPool null\n", prFbmPool);
            return FALSE;
        }
        if(u4DataSize > prFbmPool->u4Size)
        {
            LOG(1, "u4DataSize(%d) too big\n", u4DataSize);
            u4DataSize = prFbmPool->u4Size;
        }
        if(u4DataSize == 0)
        {
            LOG(1, "u4DataSize(0)\n", u4DataSize);
            return FALSE;
        }
        x_memcpy((VOID*)VIRTUAL(prFbmPool->u4Addr), (VOID*)u4DataAddr, u4DataSize);
        u4DataAddr = prFbmPool->u4Addr;
    }
    else
    {
        if(!rVdecBootLogo.rField.ucEnable)
        {
            //LOG(1, "Driver Logo Disabled by AP!\n");
            return TRUE;
        }

        ucDataType =
            bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_DATA_TYPE);

        u4DataSize =
            bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_DATA_SIZE);
        u4DataSize <<= 8;
        u4DataSize +=
            bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_DATA_SIZE + 1);
        u4DataSize <<= 8;
        u4DataSize +=
            bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_DATA_SIZE + 2);
        u4DataSize <<= 8;
        u4DataSize +=
            bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_DATA_SIZE + 3);

        u4DataAddr =
            bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_DATA_ADDR);
        u4DataAddr <<= 8;
        u4DataAddr +=
            bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_DATA_ADDR + 1);
        u4DataAddr <<= 8;
        u4DataAddr +=
            bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_DATA_ADDR + 2);
        u4DataAddr <<= 8;
        u4DataAddr +=
            bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_DATA_ADDR + 3);

        // NAND access unit is 2K.
        if((u4DataSize % (2048)) > 0)
        {
            u4DataSize += (2048 - (u4DataSize % (2048)));
        }
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
        if(!prFbmPool)
        {
            LOG(1, "prFbmPool null\n", prFbmPool);
            return FALSE;
        }
        if(u4DataSize > prFbmPool->u4Size)
        {
            LOG(1, "u4DataSize(%d) too big\n", u4DataSize);
            u4DataSize = prFbmPool->u4Size;
        }
        if(u4DataSize == 0)
        {
            LOG(1, "u4DataSize(0)\n", u4DataSize);
            return FALSE;
        }
#ifdef CC_NAND_ENABLE
        STORG_SyncNandRead(DRVCUST_OptGet(eDrvLogoVdecFlashDevId),
            u4DataAddr, (VOID*)VIRTUAL(prFbmPool->u4Addr), u4DataSize);
#else
        NORSTG_Read (DRVCUST_OptGet(eDrvLogoVdecFlashDevId),
            u4DataAddr, (UINT32)VIRTUAL(prFbmPool->u4Addr), u4DataSize);
#endif
        u4DataAddr = prFbmPool->u4Addr;
    }
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    if(!prVdecEsInfo)
    {
        LOG(1, "prVdecEsInfo = 0!\n");
        return FALSE;
    }

    if(_hIframeSema == 0)
    {
        VERIFY(x_sema_create(&_hIframeSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK) == OSR_OK);
    }

    _bPlayingIframe = TRUE;

    rNfyInfo.pvTag = &ucEsId;
    rNfyInfo.pfDecNfy = _LOGOVDEC_VDEC_Nfy;
    VDEC_SetDecNfy(ucEsId, &rNfyInfo);

    VDEC_Play_I_Frame(ucEsId, (ENUM_VDEC_FMT_T)ucDataType);

    // decoder eat virtual address
    rMpvBuf.pucIFrameBufAddr = (UCHAR*)VIRTUAL(u4DataAddr);
    rMpvBuf.u4Size = u4DataSize;

    // set write pointer
    VDEC_SetWptr(ucEsId, (UINT32)(rMpvBuf.pucIFrameBufAddr + rMpvBuf.u4Size));

    // clear lock
    _u4IFrameResult = LOGOVDEC_TIMEOUT;
    x_sema_lock(_hIframeSema, X_SEMA_OPTION_NOWAIT);

    // decoding
    //Printf(" - MPV_SetIFrameDec\n");
    i4Ret = MPV_SetIFrameDec(ucEsId, &rMpvBuf);
    if(i4Ret != MPV_DRV_OK)
    {
        LOG(1, " - ucEsId = %d, MPV_SetIFrameDec fail\n",(int)ucEsId);
        fgRet = FALSE;
        goto DECODE_ERROR;
    }

    //Printf(" - Wait Vdec decoding\n");
    if(x_sema_lock_timeout(_hIframeSema, LOGOVDEC_IFRAME_TIMEOUT) != OSR_OK)
    {
        LOG(1, " - Wait Vdec decoding timeout\n");
        fgRet = FALSE;
        goto DECODE_ERROR;
    }
    if(_u4IFrameResult != LOGOVDEC_FINISH)
    {
        LOG(1, " - Wait Vdec decoding error %d\n", (int)_u4IFrameResult);
        fgRet = FALSE;
        goto DECODE_ERROR;
    }

    if(!_bPlayingIframe)
    {
        LOG(1, " - _bPlayingIframe 2 FALSE\n");
        fgRet = FALSE;
        goto DECODE_ERROR;
    }

    if(!MPV_GetIFrameBufferAddr(ucEsId, &rSrcInfo))
    {
        LOG(1, " MPV_GetIFrameBufferAddr fail\n");
        fgRet = FALSE;
        goto DECODE_ERROR;
    }
    if((rSrcInfo.u4AddrY == 0) || (rSrcInfo.u4AddrC== 0) || (rSrcInfo.u4RRMode > 1))
    {
        LOG(1, " addr fail\n");
        fgRet = FALSE;
        goto DECODE_ERROR;
    }
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG_OSD1);
    if(!prFbmPool)
    {
        LOG(1, " - prFbmPool null\n");
        fgRet = FALSE;
        goto DECODE_ERROR;
    }
    if( (rSrcInfo.u4VSize * rSrcInfo.u4HSize * 4) > prFbmPool->u4Size)
    {
        LOG(1, " - Buffer Size not enough %d x %d x 4 > %d\n",
            rSrcInfo.u4HSize, rSrcInfo.u4VSize, prFbmPool->u4Size);
        fgRet = FALSE;
        goto DECODE_ERROR;
    }
    u4DataAddr = prFbmPool->u4Addr;
    if(!u4DataAddr)
    {
        LOG(1, " - Buffer null\n");
        fgRet = FALSE;
        goto DECODE_ERROR;
    }

    rCapDstInfo.u4ThumbnailWidth = (((rSrcInfo.u4HSize+7)>>3)<<3);
    rCapDstInfo.u4ThumbnailHeight = rSrcInfo.u4VSize;
    rCapDstInfo.pucCanvasBuffer = (UCHAR*)u4DataAddr;
    rCapDstInfo.eCanvasColormode = VDP_COLORMODE_AYUV_D8888;
    rCapDstInfo.u4CanvasWidth = (rCapDstInfo.u4ThumbnailWidth*4);
    rCapDstInfo.u4CanvasHeight = rCapDstInfo.u4ThumbnailHeight;
    rCapDstInfo.u4CanvasPitch = (rCapDstInfo.u4ThumbnailWidth*4);

    VDEC_RegEventNotifyFunc(_LOGOVDEC_VDEC_EVENT_NOTIFY);

    VDEC_GetVideoThumbnail(ucEsId, 0/*u2CompId*/, TRUE,
        &rCapDstInfo, &rSrcInfo);

    //Printf(" - Wait Vdec thumbnail\n");
    if(x_sema_lock_timeout(_hIframeSema, LOGOVDEC_IFRAME_TIMEOUT) != OSR_OK)
    {
        LOG(1, " - Wait Vdec thumbnail timeout\n");
        fgRet = FALSE;
    }
    if(_u4IFrameResult != LOGOVDEC_FINISH)
    {
        LOG(1, " - Wait Vdec thumbnail error %d\n", (int)_u4IFrameResult);
        fgRet = FALSE;
    }
    if(!_bPlayingIframe)
    {
        LOG(1, " - _bPlayingIframe 3 FALSE\n");
        fgRet = FALSE;
    }

    VDEC_RegEventNotifyFunc(NULL);

    if(fgRet)
    {
        BOOL fgEnable = FALSE;
        UINT32 u4OsdPlane = OSD_PLANE_2;
        INT32 i4OrgRegionList = -1;
        UINT32 u4Scaler = OSD_SCALER_2;
        UINT32 u4Enable = 0;
        UINT32 u4SrcW = 0;
        UINT32 u4SrcH = 0;
        UINT32 u4DstW = 0;
        UINT32 u4DstH = 0;
        UINT32 u4Is16Bpp = 0;
    
#ifdef CC_MT5365
        i4OrgRegionList = OSD_PLA_GetFirstRegionList(u4OsdPlane);
        if(i4OrgRegionList < 0)
        {
            i4OrgRegionList = OSD_PLA_GetFirstRegionList(OSD_PLANE_3);
            if(i4OrgRegionList >= 0)
            {
                u4OsdPlane = OSD_PLANE_3;
                u4Scaler = OSD_SCALER_3;
            }
        }
        i4OrgRegionList = OSD_PLA_GetFirstRegionList(u4OsdPlane);
        if(OSD_PLA_GetEnable(u4OsdPlane, &fgEnable) < 0)
        {
            Printf("OSD_PLA_GetEnable fail\n");
        }
#endif
        UNUSED(OSD_SC_GetScalerInfo(u4Scaler, &u4Enable,
                       &u4SrcW, &u4SrcH,
                       &u4DstW, &u4DstH,
                       &u4Is16Bpp));
        UNUSED(u4Is16Bpp);
            
        // osd eat PHYSICAL address
        MPV_OsdDisplay(u4OsdPlane, u4Scaler, OSD_CM_AYCBCR8888_DIRECT32, u4DataAddr, rCapDstInfo.u4ThumbnailWidth, rCapDstInfo.u4ThumbnailHeight);
        
        OSD_PLA_Enable(u4OsdPlane, FALSE);
        if(u4Enable != 0)
        {
            UNUSED(OSD_SC_Scale(u4Scaler, u4Enable,
                           u4SrcW, u4SrcH,
                           u4DstW, u4DstH));
        }
        if(i4OrgRegionList >= 0)
        {
            OSD_PLA_FlipTo(u4OsdPlane, (UINT32)i4OrgRegionList);
        }
        else
        {
            OSD_PLA_FlipToNone(u4OsdPlane);
        }
        if(fgEnable)
        {
            OSD_PLA_Enable(u4OsdPlane, TRUE);
        }
            
        LOG(1, "MPV_OsdDisplay finish\n");
    }
DECODE_ERROR:
    VDEC_Stop(ucEsId);
    if(_bPlayingIframe)
    {
        _bPlayingIframe = FALSE;
        x_sema_unlock(_hIframeSema);
    }

#endif
    return fgRet;
}

