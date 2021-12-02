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

/** @file vdec_vp6psr.c
 *  This file contains implementation of exported APIs of VDEC.
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
#include "vdec_vp6com.h"
#include "vdec_vp6util.h"
#include "vdec_vp6psr.h"
#include "vdec_vp6dec.h"
#include "vdec_vp6if.h"

static BOOL _VDEC_BackupSram(VP6_DEC_PARAM_T *prDecParam)
{
    UINT32 i, j, u4Cnt, u4Addr, *pBuf;

    if (!prDecParam)
    {
        return FALSE;
    }

    // backup VLD wrapper
    pBuf = &(prDecParam->au4VldWrapper[0]);

    u4Cnt = 0;
    for (i = 0; i < (VLD_WRAPPER_AC_BASE_END - VLD_WRAPPER_UPD_MP_TYPE + 1); i++)
    {
        for (j = 0; j < 4; j++)
        {
            u4Addr = MC_VLD_WRAPPER_READ | ((VLD_WRAPPER_UPD_MP_TYPE + i) << 2) | (3 - j);
            vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, u4Addr);
            pBuf[u4Cnt] = u4VDecReadMC(RW_MC_VLD_WRAPPER_DATA);
            u4Cnt++;
        }
    }
   
    // backup reorder coeff
    pBuf = &(prDecParam->au4Reorder[0]);

    for (i = 0; i < 16; i++)
    {
       vVDecWriteVLD(RW_VLD_SCL_ADDR, (SCL_READ | (i * 4)));
       pBuf[i] = u4VDecReadVLD(RW_VLD_SCL_DATA);
    }

    return TRUE;
}

static BOOL _VDEC_RestoreSram(VP6_DEC_PARAM_T *prDecParam)
{
    UINT32 i, j, u4Cnt, u4Addr, *pBuf;

    if (!prDecParam)
    {
        return FALSE;
    }

    // restore VLD wrapper
    pBuf = &(prDecParam->au4VldWrapper[0]);

    u4Cnt = 0;
    for (i = 0; i < (VLD_WRAPPER_AC_BASE_END - VLD_WRAPPER_UPD_MP_TYPE + 1); i++)
    {
        for (j = 0; j < 4; j++)
        {
            u4Addr = MC_VLD_WRAPPER_WRITE | ((VLD_WRAPPER_UPD_MP_TYPE + i) << 2) | (3 - j);
            vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, u4Addr);
            vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, pBuf[u4Cnt]);
            u4Cnt++;
        }
    }
    
    // restore reorder coeff
    vVDecWriteVP6VLD(RW_VP6_PCI_PAR, RW_VP6_FLAG);

    pBuf = &(prDecParam->au4Reorder[0]);

    for (i = 0; i < 16; i++) 
    {
       vVDecWriteVLD(RW_VLD_SCL_ADDR, (SCL_WRITE | (i * 4)));
       vVDecWriteVLD(RW_VLD_SCL_DATA, pBuf[i]);
    }

    return TRUE;
}

static BOOL _VDEC_Vp6PsrHwResolutionChange(VDEC_VP6_INFO_T * prDecInfo, UINT32 u4Widht, UINT32 u4Height)
{
    VP6_DEC_PARAM_T* prDecParam         = NULL;
    VDEC_ES_INFO_T*  prVdecEsInfo       = NULL;
    VDEC_ES_INFO_KEEP_T*  prVdecEsInfoKeep   = NULL;
    FBM_SEQ_HDR_T*   prFbmSeqHdr        = NULL; 
    VDEC_INFO_DEC_PRM_T* prDecParamEx   = NULL;
    BOOL fgRet = TRUE;
	UINT8 u4FbNum=4;
    UCHAR ucFbgType = FBM_FBG_TYPE_UNKNOWN, ucFbgId = FBM_FBG_ID_UNKNOWN;

    if (!prDecInfo || (prDecInfo->u1StrmId >= MAX_STRM_NS) || (u4Widht == 0) || (u4Height == 0) || (u4Widht > VDEC_VP6_HWMAX_WIDTH))
    {
        return FALSE;
    }

    prDecParam      = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
    prDecParamEx    = &prDecInfo->rDecParamEx;
    
    //if (pDecParam->u4FrameCounter==0)
    {
        prVdecEsInfo = _VDEC_GetEsInfo(prDecInfo->uEsId);
        prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(prDecInfo->uEsId);
        if (FBM_ChkFrameBufferFlag(prDecParam->ucFbgId, (UINT32)FBM_FLAG_RELEASE_FBG))        
        {
            FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, prDecInfo->uEsId);    
            FBM_WaitUnlockFrameBuffer(prDecParam->ucFbgId, 20);    // block until display unlock                    
            FBM_ReleaseGroup(prDecParam->ucFbgId);
            prDecParam->ucFbgId = FBM_FBG_ID_UNKNOWN;
        }

        if (prDecParam->ucFbgId != FBM_FBG_ID_UNKNOWN)
        {
            //if ((u4Widht > prDecParam->u2Width) || (u4Height > prDecParam->u2Height))
            {
                FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, prDecInfo->uEsId);    
                FBM_WaitUnlockFrameBuffer(prDecParam->ucFbgId, 20);    // block until display unlock                    
                FBM_ReleaseGroup(prDecParam->ucFbgId);
                prDecParam->ucFbgId = FBM_FBG_ID_UNKNOWN;
            }
        }

        prDecParam->u2Width  = u4Widht;
        prDecParam->u2Height = u4Height;

        if (prDecParam->ucFbgId == FBM_FBG_ID_UNKNOWN)
        {       
            ucFbgType= FBM_SelectGroupType(u4Widht, u4Height);
            if (ucFbgType == FBM_FBG_TYPE_UNKNOWN)
            {
                LOG(3,  "prDecPrm->u1FbgId == FBM_FGB_TYPE_UNKNOWN\n");
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
                else if((prDecParam->u1AlphaFlag&(VP6_ALPHA_ENABLE|VP6_ALPHA_FRAME)) == (VP6_ALPHA_ENABLE|VP6_ALPHA_FRAME))
                {
                    rPar.u1AppMode = FBM_FBG_APP_VP6_ALPHA;
				    FBM_SetDecoderFB(FBM_VDEC_VP6,3);
				    u4FbNum=3;
                }
                else
                {
                    rPar.u1AppMode = FBM_FBG_APP_NORMAL;
                }
                ucFbgId =
                    FBM_CreateGroupExt(ucFbgType, FBM_VDEC_VP6, 0, 0, &rPar);
            }
			
            if (ucFbgId == FBM_FBG_ID_UNKNOWN)
            {
                LOG(3,  "Stream(%d):prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN\n",prDecInfo->u1StrmId);
                return FALSE;
            }

            if (FBM_GetFrameBufferNs(ucFbgId) < u4FbNum)
            {
                LOG(0, "VP6 can't get enough FBM for normal decode(%d < 4)\n", FBM_GetFrameBufferNs(ucFbgId));
                ASSERT(0);
            }
			if ((prDecParam->u1AlphaFlag&(VP6_ALPHA_ENABLE|VP6_ALPHA_FRAME)) != (VP6_ALPHA_ENABLE|VP6_ALPHA_FRAME))
			{
                FBM_SetPlayMode(ucFbgId, FBM_FBG_MM_MODE);
                if(prVdecEsInfoKeep->fgVPush)
                {
                   if(prVdecEsInfoKeep->fgVPushDecodeOnly)
                   {
                       FBM_SetPlayMode(ucFbgId,FBM_FBG_NOTDISPLAY_MODE);
                   }
                   FBM_RegFbgCbFunc(ucFbgId,FBM_CB_FUNC_FB_READY_EX_IND, (UINT32)_VPUSH_DecodeDone, prDecInfo->uEsId);
                }

                prVdecEsInfo->ucFbgId = ucFbgId;
                FBM_SetSyncStc(ucFbgId, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId);
                FBM_FbgChgNotify(ucFbgId, prDecInfo->uEsId);
			}
			

            prFbmSeqHdr=FBM_GetFrameBufferSeqHdr(ucFbgId);
            prDecParam->ucFbgId = ucFbgId;
#ifdef VP6_64X32_BLK_MODE
            prDecParam->u4FbmLineSize = ((u4Widht + 63) >> 6) << 6;
#else
            prDecParam->u4FbmLineSize = ((u4Widht + 15) >> 4) << 4;
#endif

#ifdef VDEC_VP6_HWDEBLOCK
            if (FBM_GetFrameBufferNs(ucFbgId) < 5)
            {
                LOG(0, "VP6 can't get enough FBM for deblocking (%d < 5)\n", FBM_GetFrameBufferNs(ucFbgId));
                ASSERT(0);
            }
            FBM_SetFrameBufferFlag(prDecParam->ucFbgId, FBM_FLAG_DEBLOCKING);
            LOG(5, "DBK Enable\n");
#endif
        }

        if (ucFbgType != prDecParam->ucFbgType)
        {
            UINT32 u4WorkingBuffer = 0;
            UINT32 u4BufferSize = 0;
            
            FBM_GetWorkingBuffer(prDecParam->ucFbgId, &u4WorkingBuffer, &u4BufferSize);
            if (VDEC_VP6_HWWORKBUF_SIZE > u4BufferSize)
            {
                u4BufferSize    = 0;
                u4WorkingBuffer = 0;
            }
            else
            {
                VP6_CLRFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_BSPWKBUF);
                prDecParam->u4WorkBuf = u4WorkingBuffer;
            }

            if (u4BufferSize == 0)
            {
                u4WorkingBuffer =(UINT32)BSP_AllocAlignedDmaMemory(VDEC_VP6_HWWORKBUF_SIZE, 16);
                ASSERT(u4WorkingBuffer);
                if (!u4WorkingBuffer)
                {
                    prDecParam->u4WorkBuf = 0;
                    VP6_CLRFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_BSPWKBUF);
                    return FALSE;
                }
                else
                {
                    VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_BSPWKBUF);
                    prDecParam->u4WorkBuf = u4WorkingBuffer;
                }
            }

            prDecParam->fgInitedDec = TRUE;
            if (prFbmSeqHdr != NULL)
            {
                if (prVdecEsInfo != 0)
                {
                    UINT32 u4fps = 0;
                    prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_1_1;        // 16:9

#ifdef CC_VP6_EMULATION
                    prVdecEsInfo->rMMSeqInfo.e_frame_rate = VDEC_SRC_FRAME_RATE_30;
#endif

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
                        prDecParam->u4DeltaPTS = (90000000/VDEC_VP6DEF_FRAME_RATE);
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
#ifdef VP6_FPGA_TEST
                    prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_60;
#endif
                    prFbmSeqHdr->fgProgressiveSeq = TRUE;
                    prFbmSeqHdr->u2HSize         = prDecParam->u2Width;
                    prFbmSeqHdr->u2VSize         = prDecParam->u2Height;
                    prFbmSeqHdr->u2OrgHSize      = prDecParam->u2Width;
                    prFbmSeqHdr->u2OrgVSize      = prDecParam->u2Height;
                    prFbmSeqHdr->u2LineSize      = (UINT16)prDecParam->u4FbmLineSize;
                    prFbmSeqHdr->u4YAddrOffset   = 0;
                    prFbmSeqHdr->u4CAddrOffset   = 0;
                    prFbmSeqHdr->fgRasterOrder   = 0;
                }
                else
                {  
                    prFbmSeqHdr->fgProgressiveSeq = 1;
                    prFbmSeqHdr->ucAspRatInf     = 1;        // 16:9
                    prFbmSeqHdr->ucFrmRatCod     = 5;        // 30 fps
                    prFbmSeqHdr->u2HSize         = prDecParam->u2Width;
                    prFbmSeqHdr->u2VSize         = prDecParam->u2Height;
                    prFbmSeqHdr->u2OrgHSize      = prDecParam->u2Width;
                    prFbmSeqHdr->u2OrgVSize      = prDecParam->u2Height;
                    prFbmSeqHdr->u2LineSize      = (UINT16)prDecParam->u4FbmLineSize;
                    prFbmSeqHdr->u4YAddrOffset   = 0;
                    prFbmSeqHdr->u4CAddrOffset   = 0;
                    prFbmSeqHdr->fgRasterOrder   = 0;
                }
            }
        }
    }
    
    return fgRet;
}

BOOL _VDEC_Vp6PsrHwInit(VDEC_VP6_INFO_T *prDecInfo, INT32 i4DecId)
{
    if (!prDecInfo)
    {
        return FALSE;
    }

    return _Vdec_VP6Hw_init(prDecInfo);
}

INT32 _VDEC_Vp6PsrHwHeaderInfo(VDEC_VP6_INFO_T *prDecInfo)
{
    VP6_DEC_PARAM_T* prDecParam;
    VDEC_PES_INFO_T* prPesInfo;
    VDEC_INFO_DEC_PRM_T* prDecParamEx;
#ifdef VDEC_TIME_PROFILE
    HAL_TIME_T rTimeFbS, rTimeFbE, rTimeFbDt;
#endif // VDEC_TIME_PROFILE

    if (!prDecInfo)
    {
        return E_VDEC_FAIL;
    }

    prPesInfo  = &prDecInfo->rPesInfo;
    if (prPesInfo->fgEos)
    {
        return E_VDEC_FAIL;
    }
    
    if(!VP6_ISSET(prDecInfo->rDecParamEx.u4Flag,VP6_DEC_FLG_SUPPORT_ALPHA))
    {
		if ((prPesInfo->u1AlphaFlag & (VP6_ALPHA_ENABLE | VP6_ALPHA_FRAME)) == (VP6_ALPHA_ENABLE | VP6_ALPHA_FRAME))
		{
			return E_VDEC_SKIPFRAME;
		}
    }

    _VDEC_LockVld(prDecInfo->rPesInfo.ucEsId, prDecInfo->rPesInfo.ucMpvId);

    prDecInfo->u1StrmId = (prPesInfo->u1AlphaFlag & VP6_ALPHA_ENABLE) ? ((prPesInfo->u1AlphaFlag & VP6_ALPHA_FRAME) ? 1 : 0) : 0;
    prDecParam = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
    prDecParamEx = &prDecInfo->rDecParamEx;
#if 0
	if(prDecParam->u4FrameCounter==498)
	{
		_VDEC_VP6_LogSimPat(TRUE);
	}
	else
	{
		_VDEC_VP6_LogSimPat(FALSE);
	}
#endif	
    prDecParam->u1AlphaFlag = prPesInfo->u1AlphaFlag;
    if (prDecParam->u1AlphaFlag & VP6_ALPHA_ENABLE)
    {
        _VDEC_RestoreSram(prDecParam);
    }

    if (!_Vdec_VP6HwGetHeaderInfo(prDecInfo))
    {
        _VDEC_UnlockVld(prDecInfo->rPesInfo.ucEsId, prDecInfo->rPesInfo.ucMpvId);
        return E_VDEC_NOT_SUPPORT;
    }
   
    _VDEC_UnlockVld(prDecInfo->rPesInfo.ucEsId, prDecInfo->rPesInfo.ucMpvId);
    
    if ((prDecParam->u4FrameCounter == 0) || (prDecParam->u2NewWidth != prDecParam->u2Width) || (prDecParam->u2NewHeight != prDecParam->u2Height))
    {
        LOG(2, "VP6 Resolution Change, old (%d, %d), new (%d %d)\n", prDecParam->u2Width, prDecParam->u2Height, prDecParam->u2NewWidth, prDecParam->u2NewHeight);
        if(_VDEC_Vp6PsrHwResolutionChange(prDecInfo, prDecParam->u2NewWidth, prDecParam->u2NewHeight))
        {
			prDecParam->ucGoldenFbId  = FBM_FB_ID_UNKNOWN;
			prDecParam->ucCurFbId	  = FBM_FB_ID_UNKNOWN;
			prDecParam->ucLastFbId	  = FBM_FB_ID_UNKNOWN;
        }
		else if((prPesInfo->u1AlphaFlag & (VP6_ALPHA_ENABLE | VP6_ALPHA_FRAME)) == (VP6_ALPHA_ENABLE | VP6_ALPHA_FRAME))
		{
		    VP6_CLRFLG(prDecInfo->rDecParamEx.u4Flag,VP6_DEC_FLG_SUPPORT_ALPHA);
		    LOG(2,"Resolution Change for alpha fail\n");
            return E_VDEC_SKIPFRAME;
		}
		else
		{
            LOG(2,"Resolution Change fail\n");
            return E_VDEC_RES_NOT_SUPPORT;
		}
    }

    if (prDecParam->ucFbgId != FBM_FBG_ID_UNKNOWN)
    {
        if(FBM_ChkFrameBufferFlag(prDecParam->ucFbgId, (UINT32)FBM_FLAG_RESET))
        {
            FBM_ResetGroup(prDecParam->ucFbgId);
            FBM_ClrFrameBufferFlag(prDecParam->ucFbgId, (UINT32)FBM_FLAG_RESET);
            FBM_ClrFrameBufferFlag(prDecParam->ucFbgId, (UINT32)FBM_FLAG_NOT_READY);
        }
    }
	else if((prPesInfo->u1AlphaFlag & (VP6_ALPHA_ENABLE | VP6_ALPHA_FRAME)) != (VP6_ALPHA_ENABLE | VP6_ALPHA_FRAME))
	{
        return E_VDEC_SKIPFRAME;
	}

#ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&rTimeFbS);
#endif // VDEC_TIME_PROFILE

    prDecParam->u4DispPts = prPesInfo->u4PTS;
    prDecParam->ucCurFbId = FBM_GetEmptyFrameBuffer(prDecParam->ucFbgId, VDEC_VP6_WAIT_DISP_TIME);
    if(prDecParam->ucCurFbId == FBM_FB_ID_UNKNOWN)
    {
        ASSERT(0);
        return E_VDEC_FAIL;
    }

    if(prDecParam->ucCurFbId == prDecParam->ucGoldenFbId)
    {
        ASSERT(0);
        return E_VDEC_FAIL;
    }

    FBM_SetFrameBufferStatus(prDecParam->ucFbgId, prDecParam->ucCurFbId, FBM_FB_STATUS_READY);

#ifdef VDEC_VP6_HWDEBLOCK
    prDecParam->ucDbkFbId = FBM_GetEmptyFrameBuffer(prDecParam->ucFbgId, VDEC_VP6_WAIT_DISP_TIME);
    if(prDecParam->ucDbkFbId == FBM_FB_ID_UNKNOWN)
    {
        ASSERT(0);
        return E_VDEC_FAIL;
    }

    if(prDecParam->ucDbkFbId == prDecParam->ucGoldenFbId)
    {
        ASSERT(0);
        return E_VDEC_FAIL;
    } 

    FBM_SetFrameBufferStatus(prDecParam->ucFbgId, prDecParam->ucDbkFbId, FBM_FB_STATUS_READY);
#endif

#ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&rTimeFbE);
    HAL_GetDeltaTime(&rTimeFbDt, &rTimeFbS, &rTimeFbE);
    prDecParam->u4CurWaitFbMicro += rTimeFbDt.u4Micros;///1000;
#endif // VDEC_TIME_PROFILE

    prPesInfo->fgDtsValid ? VP6_SETFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_VALIDPTS) : VP6_CLRFLG(prDecParamEx->u4Flag, VP6_DEC_FLG_VALIDPTS);

    prDecParam->u4FrameCounter++;

    return E_VDEC_OK;
}

BOOL _VDEC_Vp6PsrHwGetPicInfo(VDEC_VP6_INFO_T *prDecInfo, UINT32 *pu4YAddr,UINT32 *pu4CAddr)
{
    VP6_DEC_PARAM_T *prDecParam;
    
    if (!prDecInfo || (prDecInfo->u1StrmId >= MAX_STRM_NS) || !pu4YAddr || !pu4CAddr)
    {
        return FALSE;
    }

    prDecParam = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
    FBM_GetFrameBufferAddr(prDecParam->ucFbgId, prDecParam->ucCurFbId, pu4YAddr, pu4CAddr);
    return TRUE;
}

static void _VDEC_Vp6RestartHw(VDEC_VP6_INFO_T *prDecInfo)
{
    UINT32 i;
    VP6_DEC_PARAM_T* prDecParam;
    VDEC_PES_INFO_T* prPesInfo;
    VDEC_INFO_VP6_FRM_HDR_T *prFrmHdr;
    
    if ((!prDecInfo) || (prDecInfo->u1StrmId >= MAX_STRM_NS))
    {
        return;
    }
    
    prDecParam  = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
    prPesInfo   = &prDecInfo->rPesInfo;
    prFrmHdr    = &prDecInfo->rFrmHdr[prDecInfo->u1StrmId];
    
    _VDEC_VP6PsrRest(prDecInfo);
    _VDEC_SetVP6BsInfo(prDecInfo->uEsId, prPesInfo->u4VldReadPtr, prDecParam->u4WPtr, prPesInfo->u4FifoStart, prPesInfo->u4FifoEnd, NULL);
    
    for (i = 0; i < prDecParam->u4DecReadBytes; i++)
    {
        u4VDEC_HAL_VP6_GetBitStreamShift(8);
    }
    
    u4VDEC_HAL_VP6_InitBoolCoder(0);
    
    if (prFrmHdr->u2Buff2Offset)
    {
        _Vdec_VP6HwSetByteCount(0, prFrmHdr->u2Buff2Offset - ((prFrmHdr->ucFrameType == VP6_I_FRM) ? 6 : 1));
    }
    else
    {
        _Vdec_VP6HwSetByteCount(0, prFrmHdr->u4FrameSize - ((prFrmHdr->ucFrameType == VP6_I_FRM) ? 6 : 1));
    }
    
    for (i = 0; i < prDecParam->u4DecReadBits; i++)
    {
        u4VDEC_HAL_VP6_GetBoolCoderShift(1);
    }

    if (prFrmHdr->u2Buff2Offset && VP6_ISSET(prDecInfo->rDecParamEx.u4Flag,VP6_DEC_FLG_SUPPORT_MULTI)) 
    {
        _VDEC_SetVP6BsInfo(prDecInfo->uEsId + 1, prPesInfo->u4VldReadPtr + prFrmHdr->u2Buff2Offset, prDecParam->u4WPtr, prPesInfo->u4FifoStart, prPesInfo->u4FifoEnd, prFrmHdr);
        u4VDEC_HAL_VP6_InitBoolCoder(1);
        _Vdec_VP6HwSetByteCount(1, prFrmHdr->u4FrameSize - prFrmHdr->u2Buff2Offset);
    }
}

BOOL _VDEC_Vp6PsrHwDecode(VDEC_VP6_INFO_T *prDecInfo)
{
    VP6_DEC_PARAM_T* prDecParam;
    VDEC_PES_INFO_T* prPesInfo;
    VDEC_INFO_DEC_PRM_T* prDecParamEx;

    if ((!prDecInfo) || (prDecInfo->u1StrmId >= MAX_STRM_NS))
    {
        return FALSE;
    }

    prDecParam  = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
    prPesInfo   = &prDecInfo->rPesInfo;
    prDecParamEx= &prDecInfo->rDecParamEx;
    
    _VDEC_Vp6RestartHw(prDecInfo);

    VP6_SETFLG(prDecParamEx->u4Flag,VP6_DEC_FLG_LOCKED);

    if (_Vdec_Vp6HwDecode(prDecInfo) >= 0)
    {
        FBM_SetFrameBufferStatus(prDecParam->ucFbgId, prDecParam->ucCurFbId, FBM_FB_STATUS_READY);
#ifdef VDEC_VP6_HWDEBLOCK
        FBM_SetFrameBufferStatus(prDecParam->ucFbgId, prDecParam->ucDbkFbId, FBM_FB_STATUS_READY);
#endif
        if(prPesInfo->fgEos)
        {
            prDecParam->u4LastReadPtr   = prDecParam->u4ReadPtr;
            prDecParam->u4RefPts        = prDecParam->u4DispPts;
            prDecParam->fgLastKeyFrame  = prDecParam->fgKeyFrame;
        }

        return TRUE;
    }

    return FALSE;
}

static VOID _VDEC_Vp6GetCRC(VDEC_INFO_DEC_PRM_T* prDecParamEx)
{
#ifdef VP6_GEN_CRC
    prDecParamEx->u4CrcVal[0][0] = u4VDecReadVDECMisc(RW_VDEC_MISC_CRC_YDATA0);
    prDecParamEx->u4CrcVal[0][1] = u4VDecReadVDECMisc(RW_VDEC_MISC_CRC_YDATA1);
    prDecParamEx->u4CrcVal[0][2] = u4VDecReadVDECMisc(RW_VDEC_MISC_CRC_YDATA2);
    prDecParamEx->u4CrcVal[0][3] = u4VDecReadVDECMisc(RW_VDEC_MISC_CRC_YDATA3);
    //LOG(5, "*** CRC Y: 0x%x 0x%x 0x%x 0x%x\n", 
    //    u4VDecReadVDECTop(0x8), u4VDecReadVDECTop(0xC), u4VDecReadVDECTop(0x10), u4VDecReadVDECTop(0x14));

    prDecParamEx->u4CrcVal[1][0] = u4VDecReadVDECMisc(RW_VDEC_MISC_CRC_CDATA0);
    prDecParamEx->u4CrcVal[1][1] = u4VDecReadVDECMisc(RW_VDEC_MISC_CRC_CDATA1);
    prDecParamEx->u4CrcVal[1][2] = u4VDecReadVDECMisc(RW_VDEC_MISC_CRC_CDATA2);
    prDecParamEx->u4CrcVal[1][3] = u4VDecReadVDECMisc(RW_VDEC_MISC_CRC_CDATA3);
    //LOG(5, "*** CRC CbCr: 0x%x 0x%x 0x%x 0x%x\n", 
    //    u4VDecReadVDECTop(0x18), u4VDecReadVDECTop(0x1C), u4VDecReadVDECTop(0x20), u4VDecReadVDECTop(0x24));
#endif
}

#ifdef __MODEL_slt__
static VOID _VDEC_Vp6SltCompare(VDEC_VP6_INFO_T* prDecInfo)
{
    VP6_DEC_PARAM_T*        prDecParam;
    VDEC_INFO_DEC_PRM_T*    prDecParamEx;
    
    if (!prDecInfo)
    {
        return;
    }
    
    prDecParam      = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
    prDecParamEx    = &prDecInfo->rDecParamEx;
    
    if (prDecParam->u4FrameCounter == 10)
    {
        if ((prDecParamEx->u4CrcVal[0][0] == 0xFBD2524A) &&
            (prDecParamEx->u4CrcVal[0][1] == 0x7279D411) &&
            (prDecParamEx->u4CrcVal[0][2] == 0xE054EB41) &&
            (prDecParamEx->u4CrcVal[0][3] == 0x0F6643D6) &&
            (prDecParamEx->u4CrcVal[1][0] == 0x3A6F1B22) &&
            (prDecParamEx->u4CrcVal[1][1] == 0x742A9634) &&
            (prDecParamEx->u4CrcVal[1][2] == 0x82369620) &&
            (prDecParamEx->u4CrcVal[1][3] == 0xB21236CE))
        {
            Printf("\n\nMMOK!!\n\n\n");
        }
        else
        {
            Printf("\n\nMMNG!!\n\n\n");
        }
    }
}
#endif

BOOL _VDEC_Vp6PsrHwFin(VDEC_VP6_INFO_T *prDecInfo)
{
    VP6_DEC_PARAM_T*        prDecParam;
    VDEC_INFO_DEC_PRM_T*    prDecParamEx;
    FBM_PIC_HDR_T*          prFbmPicHdr;
    VDEC_PES_INFO_T*        prPesInfo;
    VDEC_ES_INFO_T*         prVdecEsInfo;
    UCHAR ucFbgId;
    BOOL fgNotDisplay = FALSE, fgDecOk;

    if ((!prDecInfo) || (prDecInfo->u1StrmId >= MAX_STRM_NS))
    {
        return FALSE;
    }

    prDecParamEx    = &prDecInfo->rDecParamEx;
    prPesInfo       = &prDecInfo->rPesInfo;
    prDecParam      = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
    ucFbgId         = prDecParam->ucFbgId;
    prVdecEsInfo    = _VDEC_GetEsInfo((UCHAR)prDecInfo->uEsId);

    fgDecOk         = _VDEC_WaitVP6DecDone(prDecInfo);
    
    _VDEC_Vp6GetCRC(prDecParamEx);

#ifdef __MODEL_slt__
    _VDEC_Vp6SltCompare(prDecInfo);
#endif

    prDecParam->u4LastReadPtr = prDecParam->u4ReadPtr;
    
    if (fgDecOk)
    {
        if (prDecParam->u1AlphaFlag & VP6_ALPHA_ENABLE)
        {
            _VDEC_BackupSram(prDecParam);    
        }

        if ((prDecParam->ucLastFbId != FBM_FB_ID_UNKNOWN) && (prDecParam->ucLastFbId != prDecParam->ucGoldenFbId))
        {
#ifndef CC_VP6_EMULATION
            FBM_UpdateReferenceList(prDecParam->ucFbgId, prDecParam->ucLastFbId, FALSE);
#else
            FBM_SetFrameBufferStatus(ucFbgId, prDecParam->ucLastFbId, FBM_FB_STATUS_EMPTY);
#endif
        }

        prDecParam->ucLastFbId = prDecParam->ucCurFbId;
        
        if (prDecParam->fgRefreshGd || prDecParam->fgKeyFrame)
        {
            if (prDecParam->ucGoldenFbId != FBM_FB_ID_UNKNOWN)
            {
#ifndef CC_VP6_EMULATION
                FBM_UpdateReferenceList(prDecParam->ucFbgId, prDecParam->ucGoldenFbId,FALSE);
#else
                FBM_SetFrameBufferStatus(ucFbgId, prDecParam->ucGoldenFbId, FBM_FB_STATUS_EMPTY);
#endif
            }
            prDecParam->ucGoldenFbId = prDecParam->ucCurFbId;
        }
        
#ifndef CC_VP6_EMULATION
        FBM_UpdateReferenceList(prDecParam->ucFbgId, prDecParam->ucCurFbId, TRUE);
        #ifdef VDEC_VP6_HWDEBLOCK
        FBM_UpdateReferenceList(prDecParam->ucFbgId, prDecParam->ucDbkFbId, FALSE);
        #endif
#endif
        fgNotDisplay = VDEC_IsNotDisplay(prDecInfo->uEsId, prDecParam->u4DispPts, 0, 0);
        
        if (fgNotDisplay)
        {
            FBM_SetFrameBufferStatus(ucFbgId, prDecParam->ucCurFbId, FBM_FB_STATUS_EMPTY);
#ifdef VDEC_VP6_HWDEBLOCK
            FBM_SetFrameBufferStatus(ucFbgId, prDecParam->ucDbkFbId, FBM_FB_STATUS_EMPTY);
#endif    
        }
        else
        {
            prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, prDecParam->ucCurFbId);
            if (!prFbmPicHdr)
            {
                return FALSE;
            }

            prFbmPicHdr->fgProgressiveFrm = TRUE; 
            prFbmPicHdr->ucPicCdTp      = prDecParam->fgKeyFrame ? (UCHAR)I_TYPE : (UCHAR)P_TYPE;
            prFbmPicHdr->ucPicStruct    = (UCHAR)3;
            prFbmPicHdr->fgRepFirstFld  = FALSE;
            prFbmPicHdr->fgTopFldFirst  = FALSE;
            prFbmPicHdr->u4PTS          = prPesInfo->u4PTS;
            prFbmPicHdr->u8OffsetI      = prVdecEsInfo->u8OffsetI;
            prFbmPicHdr->u4Timestap     = prVdecEsInfo->u4Timestap;
            prFbmPicHdr->u4PTS          = prDecParam->u4DispPts;
            prFbmPicHdr->u8Offset       = prVdecEsInfo->u8Offset;
            prFbmPicHdr->u8OffsetDisp   = prVdecEsInfo->u8OffsetLast;
            prFbmPicHdr->u2DecodingOrder= prVdecEsInfo->u2DecodingOrder;
            prFbmPicHdr->u4TotlaTimeOffset = prVdecEsInfo->u4TotlaTimeOffset;
            prFbmPicHdr->ucTrueZeroPTS  = FBM_USE_ZERO_PTS_IF_ZERO;
            prFbmPicHdr->u4PicWidth     = prDecParam->u2Width;
            prFbmPicHdr->u4PicHeight    = prDecParam->u2Height;
            prFbmPicHdr->u4PicWidthPitch= prDecParam->u4FbmLineSize;
            prVdecEsInfo->ucFbgId       = prDecParam->ucFbgId;
            prVdecEsInfo->ucFbId        = prDecParam->ucCurFbId;
            
            if(prDecInfo->rPesInfo.rExtra.fgValid)
            {
                x_memcpy(&prFbmPicHdr->rExtra, &prDecInfo->rPesInfo.rExtra, sizeof(FBM_PIC_EXTRA_INFO_T));
            }
            
            if(prDecInfo->rPesInfo.fgEos)
            {
                FBM_SetFrameBufferPicFlag(prDecParam->ucFbgId, prDecParam->ucCurFbId, FBM_MM_EOS_FLAG);
            }
            
            if (fgDecOk)
            {
#ifdef VDEC_VP6_HWDEBLOCK
        #ifndef CC_VP6_EMULATION
                FBM_SetFrameBufferStatus(ucFbgId, prDecParam->ucDbkFbId, FBM_FB_STATUS_DISPLAYQ);
                FBM_SetFrameBufferStatus(ucFbgId, prDecParam->ucCurFbId, FBM_FB_STATUS_EMPTY);
        #else
                FBM_SetFrameBufferStatus(ucFbgId, prDecParam->ucDbkFbId, FBM_FB_STATUS_EMPTY);
        #endif
#else
        #ifndef CC_VP6_EMULATION
                if ((prDecParam->u1AlphaFlag & (VP6_ALPHA_ENABLE | VP6_ALPHA_FRAME)) != (VP6_ALPHA_ENABLE | VP6_ALPHA_FRAME))
                {
                    FBM_SetFrameBufferStatus(ucFbgId, prDecParam->ucCurFbId, FBM_FB_STATUS_DISPLAYQ);
					prVdecEsInfo->u4DisplayQPicCnt++;
                }
				else
				{
                    FBM_SetFrameBufferStatus(ucFbgId, prDecParam->ucCurFbId, FBM_FB_STATUS_EMPTY);
				}
        #endif
#endif
            }
            else
            {
#ifdef VDEC_VP6_HWDEBLOCK
        #ifndef CC_VP6_EMULATION
                FBM_UpdateReferenceList(prDecParam->ucFbgId, prDecParam->ucDbkFbId, FALSE);
                FBM_SetFrameBufferStatus(ucFbgId, prDecParam->ucDbkFbId, FBM_FB_STATUS_EMPTY);
                
                FBM_UpdateReferenceList(prDecParam->ucFbgId, prDecParam->ucCurFbId, FALSE);
                FBM_SetFrameBufferStatus(ucFbgId, prDecParam->ucCurFbId, FBM_FB_STATUS_EMPTY);
        #else
                FBM_UpdateReferenceList(prDecParam->ucFbgId, prDecParam->ucDbkFbId, FALSE);
                FBM_SetFrameBufferStatus(ucFbgId, prDecParam->ucDbkFbId, FBM_FB_STATUS_EMPTY);
        #endif
#else
        #ifndef CC_VP6_EMULATION
                FBM_UpdateReferenceList(prDecParam->ucFbgId, prDecParam->ucCurFbId, FALSE);
                FBM_SetFrameBufferStatus(ucFbgId, prDecParam->ucCurFbId, FBM_FB_STATUS_EMPTY);
        #endif
#endif
            }
        }
    }
    else
    {
#ifdef VDEC_VP6_HWDEBLOCK
    #ifndef CC_VP6_EMULATION
        FBM_UpdateReferenceList(prDecParam->ucFbgId, prDecParam->ucDbkFbId, FALSE);
        FBM_SetFrameBufferStatus(ucFbgId, prDecParam->ucDbkFbId, FBM_FB_STATUS_EMPTY);
        
        FBM_UpdateReferenceList(prDecParam->ucFbgId, prDecParam->ucCurFbId, FALSE);
        FBM_SetFrameBufferStatus(ucFbgId, prDecParam->ucCurFbId, FBM_FB_STATUS_EMPTY);
    #else
        FBM_UpdateReferenceList(prDecParam->ucFbgId, prDecParam->ucDbkFbId, FALSE);
        FBM_SetFrameBufferStatus(ucFbgId, prDecParam->ucDbkFbId, FBM_FB_STATUS_EMPTY);
    #endif
#else
        FBM_UpdateReferenceList(prDecParam->ucFbgId, prDecParam->ucCurFbId, FALSE);
        FBM_SetFrameBufferStatus(ucFbgId, prDecParam->ucCurFbId, FBM_FB_STATUS_EMPTY);
#endif
        _Vdec_VP6Hw_Reset((UCHAR)prDecInfo->uEsId);
    }
    
    UNUSED(VDEC_CheckStopStatus(prDecInfo->uEsId, prDecParam->u4DispPts, 0));
    VP6_CLRFLG(prDecParamEx->u4Flag,VP6_DEC_FLG_LOCKED);

#if defined(ERROR_TYPE_TEST) || defined(ERROR_STRESS_TEST) || (defined(CC_VP6_EMULATION) && defined(VDEC_TIME_PROFILE))
    fgDecOk = FALSE;
#endif

    return fgDecOk;
}

BOOL _VDEC_Vp6PsrHwRelease(VDEC_VP6_INFO_T *prDecInfo)
{
    if (!prDecInfo)
    {
        return FALSE;
    }
    
    return _Vdec_VP6Hw_Release(prDecInfo->uEsId);
}

BOOL _VDEC_VP6PsrRest(VDEC_VP6_INFO_T *prDecInfo)
{
    if (!prDecInfo)
    {
        return FALSE;
    }
    
    return _Vdec_VP6Hw_Reset(prDecInfo->uEsId);
}
