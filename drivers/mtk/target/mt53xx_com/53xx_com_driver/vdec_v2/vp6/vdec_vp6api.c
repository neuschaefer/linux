/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright(c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_vp6.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_vp6.c
 *  This file contains implementation of exported APIs of VDEC.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
#include "fbm_drvif.h"
#include "vdec_drvif.h"
#include "x_timer.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_common.h"
#include "x_typedef.h"
LINT_EXT_HEADER_END
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "vdec_vp6com.h"
#include "vdec_vp6util.h"
#ifdef VP6_USE_SW_DECODER
#include "vdec_vp6swpsr.h"
#endif
#ifdef VP6_USE_HW_DECODER
#include "vdec_vp6psr.h"
#include "vdec_vp6dec.h"
#endif
#include "vdec_vp6api.h"

#define MAX_ES_NUM VDEC_MAX_ES
#define ES_ID_0 0
#define ES_ID_1 1

extern BOOL _DMX_GetVideoChannel(UINT8 u1Pidx, UINT8* pu1Channel);
static VDEC_VP6_INFO_T rVp6DecInfo[MAX_ES_NUM];

#ifdef VP6_USE_SW_DECODER
BOOL _VDEC_Vp6GetDecInfo(INT32 i4Id, VDEC_VP6_INFO_T **prVp6DecInfo)
{
    VDEC_VP6_INFO_T *prDecInfo = NULL;
    if(!prVp6DecInfo || (i4Id >= MAX_ES_NUM))
    {
        return FALSE;
    }

    prDecInfo = &rVp6DecInfo[i4Id];
    if(!VP6_ISSET(prDecInfo->rDecParam.u4Flag, VP6_DEC_FLG_INITED))
    {
        return FALSE;
    }

    *prVp6DecInfo=prDecInfo;

    return TRUE;
}

BOOL _VDEC_Vp6GetDecInstance(INT32 i4Id, UINT32 *pInstance)
{
    VDEC_VP6_INFO_T *prDecInfo = NULL;
    if(!pInstance || (i4Id >= MAX_ES_NUM))
    {
        return FALSE;
    }

    prDecInfo = &rVp6DecInfo[i4Id];
    if(!VP6_ISSET(prDecInfo->rDecParam.u4Flag, VP6_DEC_FLG_INITED))
    {
        return FALSE;
    }

    if(!prDecInfo->u4DecInstance)
    {
        return FALSE;
    }

    *pInstance=prDecInfo->u4DecInstance;

    return TRUE;
}
#endif

VDEC_VP6_INFO_T *VDEC_Vp6GetLockedInstance(void)
{
    UINT8 uEsId = 0;
    VDEC_INFO_DEC_PRM_T* prDecParamEx;
    
    for(uEsId = 0; uEsId < MAX_ES_NUM; uEsId++)
    {
        prDecParamEx = &rVp6DecInfo[uEsId].rDecParamEx;
        if(VP6_ISSET(prDecParamEx->u4Flag, VP6_DEC_FLG_LOCKED))
        {
            break;
        }
    }

    ASSERT(uEsId != MAX_ES_NUM);
    return (rVp6DecInfo + uEsId);
}

INT32 _VDEC_Vp6Init(UINT8 uEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_VP6_INFO_T     *prDecInfo;
    VP6_DEC_PARAM_T     *prDecParam;
    VDEC_INFO_DEC_PRM_T *prDecParamEx;    
    INT32 i4Ret = 0;
    UINT32 i;

    if (uEsId >= MAX_ES_NUM)
    {
        return E_VDEC_FAIL;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(uEsId);
    prDecInfo = &rVp6DecInfo[uEsId];
    
    x_memset(&prDecInfo->rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
    x_memset(&prDecInfo->rPpInfo, 0, sizeof(VDEC_INFO_VP6_PP_INFO_T));
    x_memset(&prDecInfo->rDecParamEx, 0, sizeof(VDEC_INFO_DEC_PRM_T));

    for (i = 0; i < 2; i++)
    {
        prDecParam = &prDecInfo->rDecParam[i];
        x_memset(prDecParam, 0, sizeof(VP6_DEC_PARAM_T));
        prDecParam->ucFbgType    = FBM_FBG_TYPE_UNKNOWN;
        prDecParam->ucFbgId      = FBM_FBG_ID_UNKNOWN;
        prDecParam->ucCurFbId    = FBM_FB_ID_UNKNOWN;
        prDecParam->ucLastFbId   = FBM_FB_ID_UNKNOWN;
        prDecParam->ucGoldenFbId = FBM_FB_ID_UNKNOWN;
        prDecParam->ucDbkFbId    = FBM_FB_ID_UNKNOWN;

        x_memset(&prDecInfo->rFrmHdr[i], 0, sizeof(VDEC_INFO_VP6_FRM_HDR_T));
    }

    prDecInfo->uEsId     = uEsId;
    prDecInfo->u1StrmId  = 0;

    prDecParamEx = &prDecInfo->rDecParamEx;
	
#if defined(CC_MT5368)
    #if defined(VDEC_DDR3_SUPPORT) || defined(VDEC_VP6_HWDEBLOCK)
	VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_POLLING);
	VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_FINISH_PARAM);
    #endif
#elif defined(CC_MT5389)
	VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_SUPPORT_MULTI);
	VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_SUPPORT_ALPHA);
    if(BSP_GetIcVersion()==IC_VER_5389_AA)
    {
        #if defined(VDEC_DDR3_SUPPORT) || defined(VDEC_VP6_HWDEBLOCK)
		VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_POLLING);
		VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_FINISH_PARAM);
        #endif
    }
	else
	{
        #ifndef SUPPORT_DEC_ISR
        VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_POLLING);
        #endif
	}
#elif defined(CC_MT5396)
    if(BSP_GetIcVersion()==IC_VER_5396_AA)
    {
       #if defined(VDEC_DDR3_SUPPORT) || defined(VDEC_VP6_HWDEBLOCK)
	   VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_POLLING);
	   VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_FINISH_PARAM);
	   #endif
    }
	else
	{
		VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_SUPPORT_MULTI);
		VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_SUPPORT_ALPHA);
	    #ifndef SUPPORT_DEC_ISR
	    VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_POLLING);
        #endif
	}
#else
    VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_SUPPORT_MULTI);
    VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_SUPPORT_ALPHA);
    #ifndef SUPPORT_DEC_ISR
    VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_POLLING);
    #endif
#endif

#ifdef VP6_USE_SW_DECODER
    prDecParamEx->fgSwdecoder = FALSE;
#endif
    if (!VP6_ISSET(prDecParamEx->u4Flag, VP6_DEC_FLG_INITED))
    {
        VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_INITED);
#ifdef VP6_USE_SW_DECODER
        if (prDecParamEx->fgSwdecoder)
        {
            i4Ret = _VDEC_Vp6PsrSwInit(prDecInfo, uEsId);
        }
        else
#endif
#ifdef VP6_USE_HW_DECODER
        {
            _VDEC_LockVld(uEsId, prVdecEsInfo->u4VldId);
            i4Ret = _VDEC_Vp6PsrHwInit(prDecInfo, uEsId);
            _VDEC_UnlockVld(uEsId, prVdecEsInfo->u4VldId);
        }
#else
        {
            i4Ret=0;
        }
#endif
        if (i4Ret)
        {
            VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_INITED);
        }
        else
        {
            VP6_CLRFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_INITED);
            return E_VDEC_FAIL;
        }
    }
    else
    {
        return E_VDEC_OK;
    }

    return (INT32)E_VDEC_OK;
}


INT32 _VDEC_Vp6HdrPrs(UCHAR ucVdecId, UCHAR ucEsId)
{   
    VDEC_VP6_INFO_T* prDecInfo;
    VDEC_PES_INFO_T* prPesInfo;
    INT32 i4Ret = E_VDEC_FAIL;
#ifdef VP6_USE_SW_DECODER
    VDEC_INFO_DEC_PRM_T *prDecParamEx;
#endif

    if (ucEsId >= MAX_ES_NUM)
    {
        return E_VDEC_FAIL;
    }
    
    prDecInfo = &rVp6DecInfo[ucEsId];
    prPesInfo = &prDecInfo->rPesInfo;
#ifdef VP6_USE_SW_DECODER
    prDecParamEx = &prDecInfo->rDecParamEx;
#endif
   
#ifdef CC_VP6_EMULATION
    if((prPesInfo->ucPicType == 255) || (prPesInfo->fgEos))
    {
       return E_VDEC_BS_END;
    }
#else
    if (prPesInfo->fgEos)
    {
        return E_VDEC_FAIL;
    }
#endif

#ifdef VP6_USE_SW_DECODER
    if(prDecParamEx->fgSwdecoder)
    {
        i4Ret = _VDEC_Vp6PsrSwHeaderInfo(prDecInfo);
    }
    else
#endif
#ifdef VP6_USE_HW_DECODER
    {
        i4Ret = _VDEC_Vp6PsrHwHeaderInfo(prDecInfo);
    }
#else
    {
        i4Ret = E_VDEC_NOT_SUPPORT;
    }
#endif

/*
    if((!fgRet) && (!pVp6DecInfo->rPesInfo.fgEos))
    {
        LOG(5, "Enter _VDEC_Vp6PrsHdr Fail");
        return E_VDEC_FAIL;
    }
*/   
    return i4Ret;
}

INT32 _VDEC_Vp6DecStart(UCHAR ucVdecId, UCHAR ucEsId)
{
    VDEC_VP6_INFO_T* prDecInfo;
    VP6_DEC_PARAM_T* prDecParam;
    BOOL fgRet = FALSE;
#ifdef VP6_USE_SW_DECODER
    VDEC_INFO_DEC_PRM_T *prDecParamEx;
#endif

    if (ucEsId >= MAX_ES_NUM)
    {
        return E_VDEC_FAIL;
    }
    prDecInfo   = &rVp6DecInfo[ucEsId];
    
    if (prDecInfo->u1StrmId >= MAX_STRM_NS)
    {
        return E_VDEC_FAIL;
    }
    prDecParam  = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
#ifdef VP6_USE_SW_DECODER
    prDecParamEx= &prDecInfo->rDecParamEx;
#endif

#ifdef VP6_USE_SW_DECODER
    if(prDecParamEx->fgSwdecoder)
    {
        fgRet = _VDEC_Vp6PsrSwDecode(prDecInfo);
    }
    else
#endif
#ifdef VP6_USE_HW_DECODER
    {
        _VDEC_LockVld(ucEsId, ucVdecId);
        fgRet = _VDEC_Vp6PsrHwDecode(prDecInfo);
        if (!fgRet)
        {
            _VDEC_UnlockVld(ucEsId, ucVdecId);
        }
    }
#else
    {
        fgRet = FALSE;
    }
#endif

    prDecParam->i4DecRet = fgRet ? E_VDEC_OK : E_VDEC_FAIL;

    return prDecParam->i4DecRet;
}

#ifdef LOG_FOR_MC_MEASURE
extern UINT32 _u4LogBufSa;
extern UINT32 _u4LogBufSz;
extern UINT32 _u4LogSz;
static CHAR _acLogTmp[128];

void _VDEC_Vp6McMeasure(UCHAR ucVdecId, UCHAR ucEsId)
{
    extern char _acFileDir[][512];
    UINT32 u4LogSz;
    VDEC_VP6_INFO_T* prDecInfo = NULL;
    VP6_DEC_PARAM_T* prDecParam = NULL;
    CHAR ucPicType;

    if (ucEsId >= MAX_ES_NUM)
    {
        return;
    }

    prDecInfo = &rVp6DecInfo[ucEsId];
    prDecParam = &prDecInfo->rDecParam[prDecInfo->u1StrmId];

    if (prDecParam->fgKeyFrame)
    {
        ucPicType = 'I';
    }
    else
    {
        ucPicType = 'P';
    }

    sprintf(_acLogTmp, "VDEC_perf_measure:seq_name=%s,pic_idx=%d,pic_width=%d,pic_height=%d\n", _acFileDir[2], prDecParam->u4FrameCounter - 1, prDecParam->u2Width, prDecParam->u2Height);
    u4LogSz = x_strlen(_acLogTmp);
    x_memcpy(_u4LogBufSa + _u4LogSz, _acLogTmp, u4LogSz);
    _u4LogSz += u4LogSz;
    ASSERT(_u4LogSz < _u4LogBufSz);
    
    sprintf(_acLogTmp, "VDEC_perf_measure2:pic_type=%c,framepic=%d,CYCLE_DRAM=%ld\n", ucPicType, 1, MC_READ32(RO_MC_DECODING_CYCLE));
    u4LogSz = x_strlen(_acLogTmp);
    x_memcpy(_u4LogBufSa + _u4LogSz, _acLogTmp, u4LogSz);
    _u4LogSz += u4LogSz;
    ASSERT(_u4LogSz < _u4LogBufSz);

    sprintf(_acLogTmp, "VDEC_perf_measure3:MC_DLE_NUM=%ld,MC_MBX =%ld,MC_MBY =%ld\n", u4VDecReadMC(RO_MC_DLE_NUM), u4VDecReadMC(RO_MC_MBX), u4VDecReadMC(RO_MC_MBY));
    u4LogSz = x_strlen(_acLogTmp);
    x_memcpy(_u4LogBufSa + _u4LogSz, _acLogTmp, u4LogSz);
    _u4LogSz += u4LogSz;
    ASSERT(_u4LogSz < _u4LogBufSz);

    sprintf(_acLogTmp, "VDEC_perf_measure4:CYC_SYS=%ld,INTRA_CNT=%ld,Y_BLK_CNT=%ld\n", u4VDecReadMC(RO_MC_CYC_SYS), u4VDecReadMC(RO_MC_INTRA_CNT), u4VDecReadMC(RO_MC_Y_BLK_CNT));
    u4LogSz = x_strlen(_acLogTmp);
    x_memcpy(_u4LogBufSa + _u4LogSz, _acLogTmp, u4LogSz);
    _u4LogSz += u4LogSz;
    ASSERT(_u4LogSz < _u4LogBufSz);

    sprintf(_acLogTmp, "VDEC_perf_measure5:C_BLK_CNT=%ld,WAIT_CNT=%ld,REQ_CNT=%ld\n", u4VDecReadMC(RO_MC_C_BLK_CNT), u4VDecReadMC(RO_MC_WAIT_CNT), u4VDecReadMC(RO_MC_REQ_CNT));
    u4LogSz = x_strlen(_acLogTmp);
    x_memcpy(_u4LogBufSa + _u4LogSz, _acLogTmp, u4LogSz);
    _u4LogSz += u4LogSz;
    ASSERT(_u4LogSz < _u4LogBufSz);

    sprintf(_acLogTmp, "VDEC_perf_measure6:PARA4_BEHAVIOR=%d,PARA8_BEHAVIOR=%d\n\n", ((u4VDecReadMC(RW_MC_PARA_BEHAVIOR)&PARA4_BEHAVIOR)?1:0), ((u4VDecReadMC(RW_MC_PARA_BEHAVIOR)&PARA8_BEHAVIOR)?1:0));            
    u4LogSz = x_strlen(_acLogTmp);
    x_memcpy(_u4LogBufSa + _u4LogSz, _acLogTmp, u4LogSz);
    _u4LogSz += u4LogSz;
    ASSERT(_u4LogSz < _u4LogBufSz);
}
#endif


INT32 _VDEC_Vp6DecFin(UCHAR ucVdecId, UCHAR ucEsId)
{
    VDEC_VP6_INFO_T* prDecInfo;
    BOOL fgDecFin = FALSE;
#ifdef VP6_USE_SW_DECODER
    VDEC_INFO_DEC_PRM_T* prDecParamEx;
#endif

    if (ucEsId >= MAX_ES_NUM)
    {
        return E_VDEC_FAIL;
    }

    prDecInfo   = &rVp6DecInfo[ucEsId];
#ifdef VP6_USE_SW_DECODER
    prDecParamEx  = &prDecInfo->rDecParamEx;
#endif

#ifdef VP6_USE_SW_DECODER
    if(prDecParamEx->fgSwdecoder)
    {
        fgDecFin = _VDEC_Vp6PsrSwFin(prDecInfo);
    }
    else
#endif
#ifdef VP6_USE_HW_DECODER
    {
        fgDecFin = _VDEC_Vp6PsrHwFin(prDecInfo);
        _VDEC_UnlockVld(ucEsId, ucVdecId);
    }
#else
    {
        fgDecFin = FALSE;
    }
#endif

#ifdef LOG_FOR_MC_MEASURE
    _VDEC_Vp6McMeasure(ucVdecId, ucEsId);
#endif

    return fgDecFin ? E_VDEC_OK : E_VDEC_FAIL;
}

void _VDEC_Vp6SWRst(UCHAR ucVdecId, UCHAR ucEsId)
{    
#ifdef VP6_USE_HW_DECODER
    //VDEC_VP6_INFO_T* prDecInfo;

    if (ucEsId >= MAX_ES_NUM)
    {
        return;
    }

    //prDecInfo   = &rVp6DecInfo[ucEsId];

    _VDEC_LockVld(ucEsId, ucVdecId);
    _VDEC_VP6PsrRest(&rVp6DecInfo[ucEsId]);
    _VDEC_UnlockVld(ucEsId, ucVdecId);
    UNUSED(ucVdecId);
#endif
}

void _VDEC_Vp6SetParam(UCHAR ucEsId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3)
{
    VDEC_VP6_INFO_T* prDecInfo;
    VP6_DEC_PARAM_T* prDecParam;
    VDEC_INFO_DEC_PRM_T* prDecParamEx;

    if (ucEsId >= MAX_ES_NUM)
    {
        return;
    }    
    prDecInfo    = &rVp6DecInfo[ucEsId];
    
    if (prDecInfo->u1StrmId >= MAX_STRM_NS)
    {
        return;
    }
    prDecParam   = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
    prDecParamEx = &prDecInfo->rDecParamEx;

    UNUSED(prDecParam);
    
    switch (u4Type)
    {
#ifdef VDEC_TIME_PROFILE
    case VP6_TIME_PROFILE2:
        prDecParam->u1DramMoniter = (UINT8)u4Param1;
        break;
#endif // #ifdef VDEC_TIME_PROFILE

    case VP6_SET_DEC_MODE:
        prDecParamEx->fgAdobeMode = (BOOL)u4Param1;
        break;
    }
}

void _VDEC_Vp6GetParam(UCHAR ucEsId, UINT32 u4Type, UINT32* pu4Param1, UINT32* pu4Param2, UINT32* pu4Param3)
{
    VDEC_VP6_INFO_T* prDecInfo;
    VP6_DEC_PARAM_T* prDecParam;

    if (ucEsId >= MAX_ES_NUM)
    {
        return;
    }
    prDecInfo   = &rVp6DecInfo[ucEsId];
    
    if (prDecInfo->u1StrmId >= MAX_STRM_NS)
    {
        return;
    }
    prDecParam  = &prDecInfo->rDecParam[prDecInfo->u1StrmId];

    switch (u4Type)
    {
    case VP6_DISP_IDX:
        if (pu4Param1 && pu4Param2 && pu4Param3)
        {
            *pu4Param1 = prDecParam->ucFbgId;
            *pu4Param2 = prDecParam->ucCurFbId;
            *pu4Param3 = VDEC_FMT_VP6;
        }
        break;

#ifdef VDEC_TIME_PROFILE
    case VP6_TIME_PROFILE:
        *pu4Param1 = prDecParam->ucFbgId;
        *pu4Param2 = prDecParam->u4CurDecMs;       //hw dec
        break;
        
    case VP6_TIME_PROFILE2:
        if (prDecParam->fgKeyFrame)
        {
            *pu4Param2 = 2;
        }
        else
        {
            *pu4Param2 = 0;
        }

        *pu4Param3 = 1;
        break;

    case VP6_TIME_PROFILE3:
        *pu4Param1 = prDecParam->u4CurSwMicro;
        *pu4Param2 = prDecParam->u4CurDecCycle;
        *pu4Param3 = prDecParam->u4CurDramCycle;
        break;
#endif // VDEC_TIME_PROFILE

    case VP6_PARAM_BLK_TYPE:
        if (pu4Param1)
        {
#ifdef VP6_64X32_BLK_MODE
            *pu4Param1 = 6;     // 2^6 = 64 (64x32 blk)
#else
            *pu4Param1 = 4;     // 2^4 = 16 (16x32 blk)
#endif
        }
        break;

    case VP6_PARAM_DBK_SUPPORT:
        if (pu4Param1)
        {
#ifdef VDEC_VP6_HWDEBLOCK
            *pu4Param1 = TRUE;
#else
            *pu4Param1 = FALSE;
#endif
        }
        break;
    }
}

void _VDEC_Vp6Stop(UCHAR ucEsId)
{
    VDEC_ES_INFO_T*  prVdecEsInfo;
    VDEC_VP6_INFO_T* prDecInfo;
    VP6_DEC_PARAM_T* prDecParam;
    UINT32 i;
#ifdef VP6_USE_SW_DECODER
    VDEC_INFO_DEC_PRM_T *prDecParamEx;
#endif

    if (ucEsId >= MAX_ES_NUM)
    {
        return;
    }

    prVdecEsInfo    = _VDEC_GetEsInfo(ucEsId);
    prDecInfo       = &rVp6DecInfo[ucEsId];
#ifdef VP6_USE_SW_DECODER
    prDecParamEx    = &prDecInfo->rDecParamEx;
#endif

    for (i = 0; i < 2; i++)
    {
        prDecParam = &prDecInfo->rDecParam[i];

        if(prDecParam->ucFbgId != FBM_FBG_ID_UNKNOWN)
        {
            FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, ucEsId);
            FBM_WaitUnlockFrameBuffer(prDecParam->ucFbgId, 20);    // block until display unlock            
            FBM_ReleaseGroup(prDecParam->ucFbgId);
            prDecParam->ucFbgId = FBM_FBG_ID_UNKNOWN;
        }
    }

    _VDEC_LockFlushMutex(ucEsId);
    if(prVdecEsInfo->fgFlushEsmQ)
    {
        _VDEC_FlushEs(ucEsId);
        prVdecEsInfo->fgFlushEsmQ = FALSE;
        _VDEC_UnlockFlushSema(ucEsId);        
    }
    _VDEC_ClearEsInfo(ucEsId);
    _VDEC_UnlockFlushMutex(ucEsId);
    
#ifdef VP6_USE_SW_DECODER
    if(prDecParamEx->fgSwdecoder)
    {
        _VDEC_Vp6PsrSwRelease(prDecInfo);
    }
    else
#endif
#ifdef VP6_USE_HW_DECODER
    {
        _VDEC_Vp6PsrHwRelease(prDecInfo);
    }
#else
    {

    }
#endif
}


void _VDEC_Vp6GetPesStruct(UCHAR ucEsId, void **ppvPesInfo)
{
    VDEC_VP6_INFO_T* prDecInfo;

    if ((ucEsId >= MAX_ES_NUM) || (!ppvPesInfo))
    {
        return;
    }

    prDecInfo = &rVp6DecInfo[ucEsId];
    *ppvPesInfo = (void*)(&prDecInfo->rPesInfo);
}


void _VDEC_Vp6DropFrame(UCHAR ucEsId)
{
#ifndef CC_VP6_EMULATION
    VDEC_VP6_INFO_T* prDecInfo;
    VDEC_PES_INFO_T* prPesInfo;
    UINT32 u4CurRptr;
    UINT8 uChannelId;

    if (ucEsId >= MAX_ES_NUM)
    {
        return;
    }

    prDecInfo = &rVp6DecInfo[ucEsId];
    prPesInfo = &prDecInfo->rPesInfo;
    
    u4CurRptr = prPesInfo->u4VldReadPtr;
    _DMX_GetVideoChannel((UINT8)prPesInfo->ucEsId, &uChannelId);
    if(!DMX_UpdateVideoReadPointer(uChannelId, u4CurRptr, u4CurRptr))                
    {   
        LOG(5, "Vp6 ES(%d) DMX_UpdateVideoReadPointer Fail\n", prPesInfo->ucEsId);                
    }
#endif
}


void _VDEC_Vp6GetDispPic(UCHAR ucEsId, void* pvInfo)
{
    VDEC_DISP_PIC_INFO_T*       prDispInfo;
    VDEC_VP6_INFO_T*            prDecInfo;
    VP6_DEC_PARAM_T*            prDecParam;
    VDEC_INFO_VP6_PP_INFO_T*    prPpInfo;
    UINT32 u4YAddr = 0,u4CAddr = 0;
#if defined(VP6_USE_SW_DECODER) || defined(VP6_GEN_CRC)
    VDEC_INFO_DEC_PRM_T*        prDecParamEx;
#endif

    if ((ucEsId >= MAX_ES_NUM) || !pvInfo)
    {
        return;
    }

    prDispInfo  = (VDEC_DISP_PIC_INFO_T*)pvInfo;
    prDecInfo   = &rVp6DecInfo[ucEsId];
    prPpInfo    = &(prDecInfo->rPpInfo);
    
    if (prDecInfo->u1StrmId >= MAX_STRM_NS)
    {
        return;
    }
    prDecParam  = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
#if defined(VP6_USE_SW_DECODER) || defined(VP6_GEN_CRC)
    prDecParamEx= &prDecInfo->rDecParamEx;
#endif

#ifdef VP6_USE_SW_DECODER
    if(prDecParamEx->fgSwdecoder)
    {
        _VDEC_Vp6PsrSwGetPicInfo(prDecInfo, &u4YAddr, &u4CAddr);
    }
    else
#endif
#ifdef VP6_USE_HW_DECODER
    {
        _VDEC_Vp6PsrHwGetPicInfo(prDecInfo, &u4YAddr, &u4CAddr);
    }
#else
    {

    }
#endif
   
    prDispInfo->pvYAddr     = (void*)u4YAddr;
    prDispInfo->pvCAddr     = (void*)u4CAddr;
    prDispInfo->pvYDbkAddr  = (void*)prPpInfo->u4PpYBufSa;
    prDispInfo->pvCDbkAddr  = (void*)prPpInfo->u4PpCBufSa;
    prDispInfo->u4YSize     = prDecParam->u4FbmLineSize * prDecParam->u2Height;
    prDispInfo->u4CSize     = prDispInfo->u4YSize >> 1;
    prDispInfo->u1PicStruct = VP6_FRM_TYPE;
    prDispInfo->u4W         = prDecParam->u2Width;
    prDispInfo->u4H         = prDecParam->u2Height;
    prDispInfo->u4H_Cmp     = prDecParam->u2Height;
    prDispInfo->u4W_Cmp     = prDecParam->u2Width;
    prDispInfo->fgAlphaFrame= (prDecParam->u1AlphaFlag & VP6_ALPHA_FRAME) ? TRUE : FALSE;
    prDispInfo->u4CurrentPicNo++;

#ifdef VP6_GEN_CRC
    prDispInfo->u4Ycrc[0]   = prDecParamEx->u4CrcVal[0][0];
    prDispInfo->u4Ycrc[1]   = prDecParamEx->u4CrcVal[0][1];
    prDispInfo->u4Ycrc[2]   = prDecParamEx->u4CrcVal[0][2];
    prDispInfo->u4Ycrc[3]   = prDecParamEx->u4CrcVal[0][3];
    prDispInfo->u4Ccrc[0]   = prDecParamEx->u4CrcVal[1][0];
    prDispInfo->u4Ccrc[1]   = prDecParamEx->u4CrcVal[1][1];
    prDispInfo->u4Ccrc[2]   = prDecParamEx->u4CrcVal[1][2];
    prDispInfo->u4Ccrc[3]   = prDecParamEx->u4CrcVal[1][3];
#endif
}

void _VDEC_Vp6UpdDmxRptr(UCHAR ucEsId)
{
#ifndef CC_VP6_EMULATION
    VDEC_VP6_INFO_T* prDecInfo;
    VDEC_PES_INFO_T* prPesInfo;
    UINT8 uChannelId;

    if (ucEsId >= MAX_ES_NUM)
    {
        return;
    }

    prDecInfo = &rVp6DecInfo[ucEsId];
    prPesInfo = &prDecInfo->rPesInfo;
    
    #ifdef VP6_FPGA_TEST
    uChannelId = 0;
    #else
    _DMX_GetVideoChannel((UINT8)prPesInfo->ucEsId, &uChannelId);
    #endif
    
    if (!DMX_UpdateVideoReadPointer(uChannelId, prPesInfo->u4VldReadPtr, prPesInfo->u4VldReadPtr))                
    {                    
        LOG(5, "Vp6 ES(%d) DMX_UpdateVideoReadPointer Fail\n", prPesInfo->ucEsId);                
    }
#endif
}

void _VDEC_VP6SetDmxWptr(UCHAR ucEsId, UINT32 u4Wptr)
{
    VDEC_VP6_INFO_T* prDecInfo;
    VP6_DEC_PARAM_T* prDecParam;
    
    if (ucEsId >= MAX_ES_NUM)
    {
        return;
    }
    prDecInfo   = (VDEC_VP6_INFO_T*) & rVp6DecInfo[ucEsId];
    
    if (prDecInfo->u1StrmId >= MAX_STRM_NS)
    {
        return;
    }
    prDecParam  = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
    
    prDecParam->u4WPtr = u4Wptr;
    //_VDEC_LockVld(ucEsId, prDecInfo->rPesInfo.ucMpvId);
    //_VDEC_SetVP6Wptr(u4Wptr);
    //_VDEC_UnlockVld(ucEsId, prDecInfo->rPesInfo.ucMpvId);
}

//skip ES if return FALSE
BOOL _VDEC_Vp6EsFlwCtrl(UCHAR ucEsId, UCHAR* pucVldId, INT8 i1EsBufStatus, UCHAR ucSkipMode)
{
    VDEC_ES_INFO_T*  prVdecEsInfo;
    VDEC_VP6_INFO_T* prDecInfo;
    VDEC_PES_INFO_T* prPesInfo;
#ifdef VP6_USE_SW_DECODER
    VDEC_INFO_DEC_PRM_T *prDecParamEx;
#endif

    if ((ucEsId >= MAX_ES_NUM) || (!pucVldId))
    {
        return FALSE;
    }

    prDecInfo       = &rVp6DecInfo[ucEsId];
    prPesInfo       = &prDecInfo->rPesInfo;
    prVdecEsInfo    = _VDEC_GetEsInfo(ucEsId);
#ifdef VP6_USE_SW_DECODER
    prDecParamEx    = &prDecInfo->rDecParamEx;
#endif
    
    LOG(9, "Es flw control\n");
    LOG(9, "Skip mode = %ld\n", ucSkipMode);

    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I))
    {
        _VDEC_VP6Flush(ucEsId, TRUE);
        //FBM_InvalidateRefFrameBuffer(pVp6DecInfo->rDecParam.ucFbgId);
        CLR_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    }  
#ifdef VP6_USE_SW_DECODER
    if (prDecParamEx->fgSwdecoder)
    {
        if (prPesInfo->fgDtsValid == FALSE)
        {
           return TRUE;
        }
    }
#endif
    
    if (ucSkipMode == IPMode)
    {
       //
    }
    else if (ucSkipMode == IMode)
    {
        if (prPesInfo->ucPicType != I_TYPE)
        {
            return TRUE;
        }
    }

    return FALSE;
}

VOID _VDEC_Vp6SetIsr(UCHAR ucEsId)
{
    if (ucEsId >= MAX_ES_NUM)
    {
        return;
    }

    Vp6IsrInit(&rVp6DecInfo[ucEsId]);
}

BOOL _VDEC_IsSupportVP6(void)
{
    return TRUE;
}

void _VDEC_VP6Flush(UCHAR ucEsId, BOOL bIsDisp)
{
    UINT32 i;
    UCHAR ucStatus;
    VDEC_VP6_INFO_T *prDecInfo;
    VP6_DEC_PARAM_T *prDecParam;

    if (ucEsId >= MAX_ES_NUM)
    {
        return;
    }

    prDecInfo = &rVp6DecInfo[ucEsId];

    for (i = 0 ; i < 2; i++)
    {
        prDecParam = &prDecInfo->rDecParam[i];

        if (prDecParam->ucFbgId != FBM_FBG_ID_UNKNOWN)
        {
            if (prDecParam->ucGoldenFbId != FBM_FB_ID_UNKNOWN)
            {
                ucStatus = FBM_GetFrameBufferStatus(prDecParam->ucFbgId, prDecParam->ucGoldenFbId);
                FBM_UpdateReferenceList(prDecParam->ucFbgId, prDecParam->ucGoldenFbId, FALSE);
                if ((ucStatus == FBM_FB_STATUS_READY) || (ucStatus == FBM_FB_STATUS_DECODE))
                {
                    FBM_SetFrameBufferStatus(prDecParam->ucFbgId, prDecParam->ucGoldenFbId, FBM_FB_STATUS_EMPTY);
                }
                prDecParam->ucGoldenFbId = FBM_FB_ID_UNKNOWN;
            }

            if (prDecParam->ucLastFbId != FBM_FB_ID_UNKNOWN)
            {
                ucStatus = FBM_GetFrameBufferStatus(prDecParam->ucFbgId, prDecParam->ucLastFbId);
                FBM_UpdateReferenceList(prDecParam->ucFbgId, prDecParam->ucLastFbId, FALSE);
                if ((ucStatus == FBM_FB_STATUS_READY) || (ucStatus == FBM_FB_STATUS_DECODE))
                {
                    FBM_SetFrameBufferStatus(prDecParam->ucFbgId, prDecParam->ucLastFbId, FBM_FB_STATUS_EMPTY);
                }
                prDecParam->ucLastFbId = FBM_FB_ID_UNKNOWN;
            }

            if (prDecParam->ucCurFbId != FBM_FB_ID_UNKNOWN)
            {
                ucStatus = FBM_GetFrameBufferStatus(prDecParam->ucFbgId, prDecParam->ucCurFbId);
                FBM_UpdateReferenceList(prDecParam->ucFbgId, prDecParam->ucCurFbId, FALSE);
                if ((ucStatus == FBM_FB_STATUS_READY) || (ucStatus == FBM_FB_STATUS_DECODE))
                {
                    FBM_SetFrameBufferStatus(prDecParam->ucFbgId, prDecParam->ucCurFbId, FBM_FB_STATUS_EMPTY);
                }                
                prDecParam->ucCurFbId = FBM_FB_ID_UNKNOWN;
            }
        }
    }
}

#ifdef VDEC_TIME_PROFILE
BOOL _VDEC_VP6IsPic(UCHAR ucEsId, VDEC_PES_INFO_T *prPes)
{
    UNUSED(ucEsId);
    UNUSED(prPes);
    return TRUE;
}
#endif

