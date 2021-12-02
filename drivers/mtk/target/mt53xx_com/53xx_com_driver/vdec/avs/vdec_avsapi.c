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
 * $RCSfile: vdec_avsapi.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_avsapi.c
 *  This file contains implementation of exported APIs of VDEC.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_timer.h"
#include "x_lint.h"
#include "vdec_avspsr.h"
#include "vdec_avsdec.h"
#include "vdec_avsutil.h"
#include "vdec_avsapi.h"
#include "vdec_avshw.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_chip_id.h"
#include "x_common.h"
#include "x_typedef.h"
#include "dmx_if.h"
#ifdef FBM_DPB_CTRL
#include "fbm_drvif.h"
#endif
LINT_EXT_HEADER_END

#include "vdec_drvif.h"
#include "drv_dbase.h"
//for user data
#include "mpv_drvif.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define MAX_ES_NUM 1
#define ES_ID_0 0

#define ATSC_IDENTIFIER_LE  0x34394147
#define EIA_CC    0x03
#define MPEG2_USER_DATA_SC 0x000001b2   // start code

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static VDEC_AVS_INFO_T rAVSDecInfo[MAX_ES_NUM];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Typedef
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

/** .
*@param .
 */
INT32 _VDEC_AVSInit()
{
    VDEC_AVS_INFO_T* prVdec;
    UINT32 i;
    
    prVdec = (VDEC_AVS_INFO_T*)&rAVSDecInfo[ES_ID_0];

    //reset data structure
    x_memset(prVdec, 0, sizeof(VDEC_AVS_INFO_T));
    
    prVdec->rDecParam.bWaitDecLock = TRUE;

    prVdec->rDecParam.ucFbgId = FBM_FBG_ID_UNKNOWN;
    prVdec->rDecParam.ucFbId = FBM_FBG_ID_UNKNOWN;

    prVdec->rDecParam.u4WaitFbTime = AVS_WAIT_DISP_TIME;

    prVdec->rPicHdr.u4PicRefFg = 1;
    prVdec->rPicHdr.u4AdvPredModeDisable = 1;

    for (i = 0; i < MAX_FRM_BUFNUM; i++)
    {
        _VDEC_ClrAvsFBufInfo(&prVdec->rDecParam, i, TRUE);
    }

#ifdef CC_AVS_EMULATION
    prVdec->rDecParam.u4AddrMode = ADDR_MODE_8520; 
    //prVdec->rDecParam.u4AddrMode = ADDR_MODE_5351_NORMAL; 
#else
    prVdec->rDecParam.u4AddrMode = ADDR_MODE_5351_NORMAL; 
#endif

    // Enable PP
    prVdec->rDecParam.fgEnPP = TRUE;

    // Reset bit count
    prVdec->rDecParam.u4BitCnt = 0;

    for (i = 0; i < MAX_REF_PICNUM; i++)
    {
        prVdec->rDecParam.arRefFBufInfo[i].ucFbId = FBM_FB_ID_UNKNOWN;
        prVdec->rDecParam.arRefFBufInfo[i].fgValid = TRUE;
    }

    prVdec->rDecParam.u1LastRefIdx = 0;
    
    //init hw module, fifo address is 0 now
    _VDEC_InitAVSVDecHW(FALSE);

    return (INT32)E_VDEC_OK;
}


/** unit of es packet is a nalu.
*@param .
 */
INT32 _VDEC_AVSHdrPrsHdr(UCHAR ucVdecId, UCHAR ucEsId)
{
    VDEC_AVS_INFO_T* prVdec;
    INT32 i4RetValue = (INT32)E_VDEC_OK;
    #ifdef VDEC_TIME_PROFILE
    HAL_TIME_T rTimeRstS, rTimeRstE, rTimeRstDt;    
    #endif
    DRV_DBASE_T* _prDbase = NULL;
    CRIT_STATE_T _rCritState;
    PARAM_MPV_T* prInfo;
    VDEC_INFO_T* prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    
    _prDbase = DBS_Lock(&_rCritState);
    ASSERT(_prDbase);
    VERIFY(DBS_Unlock(_prDbase, _rCritState));        
    prInfo = &(_prDbase->rMpv[0]);

    UNUSED(ucVdecId);
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_AVS_INFO_T*)&rAVSDecInfo[ucEsId];

    prVdec->rDecParam.u4ByteCnt = 0;
    prVdec->rDecParam.fgSecField = FALSE;

    if ( prVdec->rPesInfo.fgEos)
    {
        return E_VDEC_BS_END;
    }

#ifdef CC_AVS_EMULATION
    if (prVdec->rPesInfo.u4VldReadPtr == prVdec->u4PrevPrsPtr)
    {
        LOG(3, "Duplicate read pointer\n");
        return E_VDEC_NOT_CODED_FRAME;
    }
    
    prVdec->u4PrevPrsPtr = prVdec->rPesInfo.u4VldReadPtr;

    //LOG(3, "Video offset = 0x%08x\n", (prVdec->rPesInfo.u4VldReadPtr - prVdec->rPesInfo.u4FifoStart));

#ifdef CC_DUMP_AVS_RISC_SETTING
    if (//((prVdec->rDecParam.u4PicCnt == 10)) && 
        ((prVdec->rPesInfo.ucPicType == I_PICTURE_SC) || (prVdec->rPesInfo.ucPicType == PB_PICTURE_SC)))
    {
        prVdec->rDecParam.fgDumpSetting = TRUE;
    }
    else
    {
        prVdec->rDecParam.fgDumpSetting = FALSE;
    }
#endif

#endif

    _VDEC_AVSSWRst(ES0, ES0);

    #ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&rTimeRstS);
    #endif
    if(prVdec->rPesInfo.ucPicType <= VIDEO_EDIT_SC)
    {
        _VDEC_SetAvsBsInfo(BARREL1, &prVdec->rDecParam, prVdec->rPesInfo.u4VldReadPtr, prVdec->rDecParam.u4WPtr, 
            prVdec->rPesInfo.u4FifoStart, prVdec->rPesInfo.u4FifoEnd, 
            prVdec->rDecParam.u1ECMode, prVdec->rDecParam.u4ECType);
    }
    else
    {
        // I frame case
        if (prVdecInfo && (prVdecInfo->eCurState == VDEC_ST_PLAY_I_FRAME))
        {
            if (prVdecEsInfoKeep && (prVdecEsInfoKeep->pfDecNfyFct))
            {
                prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag, VDEC_COND_ERROR, 0, 0);                
            }
        }
        return (INT32)E_VDEC_PARSE_NOT_READY;
    }
    
    #ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&rTimeRstE);
    HAL_GetDeltaTime(&rTimeRstDt, &rTimeRstS, &rTimeRstE);
    prVdec->rDecParam.u4CurRstMs += (rTimeRstDt.u4Micros / 1000);
    #endif

    // Turn Off Bit Stuffing Check
    AVS_VLD_WRITE32(RW_AVS_VLD_BITSTUFFING_SWITCH, 0);

    switch(prVdec->rPesInfo.ucPicType)
    {
        case SEQ_HDR_SC:
            _VDEC_AvsSeqHdr(prVdec, &i4RetValue);
            break;
        case I_PICTURE_SC:
        case PB_PICTURE_SC:
            _VDEC_AvsPicHdr(prVdec, &i4RetValue);

            if((i4RetValue != (INT32)E_VDEC_OK) && (i4RetValue != (INT32)E_VDEC_OK_RES_CHG))
            {
                prInfo->u4VpsrDropNs++;
                if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, prVdec->rPesInfo.u4VldReadPtr, prVdec->rPesInfo.u4VldReadPtr))                
                {                    
                    LOG(1, "AVS ES(%d) DMX_UpdVRPtr Fail\n");
                }
            }
            break;
        case SEQ_END_SC:
            LOG(6, "//END_OF_SEQ\n");
            //clean FBM buffer?
            break;
        case USER_DATA_SC:
        case EXTENSION_SC:
        case VIDEO_EDIT_SC:
        default:
            // I frame case
            if (prVdecInfo && (prVdecInfo->eCurState == VDEC_ST_PLAY_I_FRAME))
            {
                if (prVdecEsInfoKeep && (prVdecEsInfoKeep->pfDecNfyFct))
                {
                    prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag, VDEC_COND_ERROR, 0, 0);                
                }
            }
            i4RetValue = (INT32)E_VDEC_PARSE_NOT_READY;
            break;
    }
    
    if(i4RetValue != (INT32)E_VDEC_OK)
    {
        prVdecInfo->arVdecEsInfo[ucEsId].arRcvPesCnt[VDEC_FMT_AVS]--;
    }

    return i4RetValue;
}


/** .
*@param .
 */
INT32 _VDEC_AVSDecStart(UCHAR ucVdecId, UCHAR ucEsId)
{
    VDEC_AVS_INFO_T* prVdec;
    INT32 i4RetValue;
    
    UNUSED(ucVdecId);
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_AVS_INFO_T*)&rAVSDecInfo[ucEsId];

#if 0
    if ((prVdec->rPicHdr.u4ProgFrm) ||
        ((!prVdec->rPicHdr.u4ProgFrm) && (!prVdec->rDecParam.fgSecField)))
#endif        
    if ((prVdec->rSeqHdr.u4IsProgSeq) ||
        ((!prVdec->rSeqHdr.u4IsProgSeq) && (!prVdec->rDecParam.fgSecField)))
    {
        _VDEC_AvsPrepareFBufInfo(prVdec);
    }

    if ((!prVdec->rSeqHdr.u4IsProgSeq) && (prVdec->rDecParam.fgSecField))
    {
        // Check wrap around case
        UINT32 u4ReadPtr = prVdec->rPesInfo.u4VldReadPtr + prVdec->rDecParam.u4ByteCnt;

        if (u4ReadPtr >= prVdec->rPesInfo.u4FifoEnd)
        {
            u4ReadPtr = prVdec->rPesInfo.u4FifoStart + (u4ReadPtr - prVdec->rPesInfo.u4FifoEnd);
        }
        
        _VDEC_AVSSWRst(ES0, ES0);
        _VDEC_SetAvsBsInfo(BARREL1, 
            &prVdec->rDecParam,
            u4ReadPtr, 
            prVdec->rDecParam.u4WPtr, 
            prVdec->rPesInfo.u4FifoStart, 
            prVdec->rPesInfo.u4FifoEnd, 
            prVdec->rDecParam.u1ECMode, 
            prVdec->rDecParam.u4ECType);
        //Printf("Decoder's Input Window: 0x%8x\n", AVS_VLD_READ32(0));
    }

    _VDEC_AvsDecProc(prVdec, &i4RetValue);
    return i4RetValue;
}


/** .
*@param .
 */
INT32 _VDEC_AVSDecFin(UCHAR ucVdecId, UCHAR ucEsId)
{
    VDEC_AVS_INFO_T* prVdec;
    INT32 i4RetValue;
    VDEC_INFO_T* prVdecInfo = _VDEC_GetInfo();
    
    UNUSED(ucVdecId);
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_AVS_INFO_T*)&rAVSDecInfo[ucEsId];
    
    _VDEC_OutputAvsDispPic(&prVdec->rDecParam, &i4RetValue);    //vChkOutputFBuf

    if(_VDEC_WaitAvsDecDone(prVdec,
        prVdec->rDecParam.prCurrFBufInfo->u1FBufStatus, 
        prVdec->rDecParam.prCurrFBufInfo->u4W, 
        prVdec->rDecParam.prCurrFBufInfo->u4H,
        prVdec->rDecParam.bWaitDecLock) > 0)
    {
        if (prVdec->rSeqHdr.u4IsProgSeq)
        {
            i4RetValue = (INT32)E_VDEC_OK;
            prVdec->rDecParam.u4PicCnt++;
        }
        else
        {
            // Need to re-trigger video decoder
            if (!prVdec->rDecParam.fgSecField)
            {
                LOG(5, "Re-Trigger HW\n");
                i4RetValue = (INT32)E_VDEC_RETRIGGER;
                prVdec->rDecParam.fgSecField = TRUE;
                prVdec->rDecParam.u4ByteCnt = _VDEC_AvsGetByteCnt();
            }
            else
            {
                LOG(5, "Succeed to re-trigger HW\n");
                i4RetValue = (INT32)E_VDEC_OK;
                prVdec->rDecParam.u4PicCnt++;
            }
        }
    }
    else
    {
        i4RetValue = (INT32)E_VDEC_FAIL; //Timeout, decode error
        LOG(3, "Fail to decode %d\n", prVdec->rDecParam.u4PicCnt);
        prVdec->rDecParam.u4PicCnt++;
    }

    if (i4RetValue != E_VDEC_RETRIGGER)
    {
        _VDEC_AvsUpdateFBList(prVdec);
    }

    _VDEC_AvsFrmFin(&prVdec->rDecParam);

    //I frame case
    if(prVdecInfo && (prVdecInfo->eCurState == VDEC_ST_PLAY_I_FRAME))
    {
    }
    
    if(prVdec->rPesInfo.fgEos)
    {
        _VDEC_FlushAvsDPB(&prVdec->rDecParam);
    }
    return i4RetValue;
}


/** .
*@param .
 */
void _VDEC_AVSSWRst(UCHAR ucVdecId, UCHAR ucEsId)
{    
    #ifdef CC_DRIVER_DEMO
    //VDEC_AVS_INFO_T* prVdec;

    ASSERT(ucEsId < MAX_ES_NUM);
    //prVdec = (VDEC_AVS_INFO_T*)&rAVSDecInfo[ucEsId];

    _VDEC_VLDAvsWaitForSramStable();
    
    _VDEC_VLDAvsHWReset();
#if 0
    _VDEC_LogSimPat(prVdec->rDecParam.fgLogSimPat);
#endif
    #endif

    UNUSED(ucVdecId);
    UNUSED(ucEsId);
}


/** .
*@param .
 */
void _VDEC_AVSStop(UCHAR ucEsId)
{
    VDEC_AVS_INFO_T* prVdec;
    
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_AVS_INFO_T*)&rAVSDecInfo[ucEsId];

    //clean data structure
    //clean FBM buffer

    if(prVdec->rDecParam.ucFbgId != FBM_FBG_ID_UNKNOWN)
    {
        FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, ucEsId);
        FBM_ReleaseGroup(prVdec->rDecParam.ucFbgId);
        prVdec->rDecParam.ucFbgId = FBM_FBG_ID_UNKNOWN;
    }

    LOG(5, "Total Frm Cnt = %d, Timeout Cnt = %d\n", 
        prVdec->rDecParam.u4PicCnt,
        prVdec->rDecParam.u4TimeOutNum);
}


/** .
*@param .
 */
void _VDEC_AVSGetPesStruct(UCHAR ucEsId, void **ppvPesInfo)
{
    VDEC_AVS_INFO_T* prVdec;
    
    ASSERT(ppvPesInfo !=NULL);    
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_AVS_INFO_T*)&rAVSDecInfo[ucEsId];

    if(prVdec->rDecParam.u1RptFrm == 2)
    {
        *ppvPesInfo = NULL;//(void*)(&prVdec->rPesInfoSkip);    //emulation repeat flw, skip pes info        
    }
    else
    {
        *ppvPesInfo = (void*)(&prVdec->rPesInfo);
    }
}


/** for emulation
*@param .
 */
void _VDEC_AVSGetDispPic(UCHAR ucEsId, void* pvInfo)
{
    VDEC_AVS_INFO_T* prVdec;
    VDEC_DISP_PIC_INFO_T* prDispInfo;
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;
    
    ASSERT(pvInfo !=NULL);
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_AVS_INFO_T*)&rAVSDecInfo[ucEsId];
    prDispInfo = (VDEC_DISP_PIC_INFO_T*)pvInfo;

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if(prVdecEsInfoKeep == NULL)
    {
        return;
    }

    prDispInfo->u4H = prVdec->rDecParam.u4LastVSize;
    prDispInfo->u4W = prVdec->rDecParam.u4LastHSize; 
    prDispInfo->pvYAddr = (void*)(prVdec->rDecParam.pucDecWorkBufY);
    prDispInfo->u4YSize = prVdec->rDecParam.u4LastHSize * prVdec->rDecParam.u4LastVSize;
    prDispInfo->pvCAddr = (void*)prVdec->rDecParam.pucDecWorkBufC;
    prDispInfo->u4CSize = prDispInfo->u4YSize >> 1;
    prDispInfo->u1PicStruct = FRAME;

#if 0
    UINT8 u1BufIdx = 0;
    if(prVdec->rDecParam.fgOutDispOrder && (!prVdecEsInfoKeep->fgVdecMidEnable))
    {
        u1BufIdx = prVdec->rDecParam.u1MinPOCBufIdx;
    }
    else
    {
        u1BufIdx = prVdec->rDecParam.u1DecFBufIdx;
    }
    if(u1BufIdx >= 18)
    {
        ASSERT(u1BufIdx < 18);
        return;
    }
    
    prDispInfo->pvYAddr = (void*)prVdec->rDecParam.arFBufInfo[u1BufIdx].u4YAddr;
    prDispInfo->u4YSize = prVdec->rDecParam.arFBufInfo[u1BufIdx].u4DramPicSize;
    prDispInfo->pvCAddr = (void*)prVdec->rDecParam.arFBufInfo[u1BufIdx].u4CAddr;
    prDispInfo->u4CSize = prDispInfo->u4YSize >> 1;
    prDispInfo->pvMVAddr = (void*)prVdec->rDecParam.arFBufInfo[u1BufIdx].u4MVBufAddr;
    
    prDispInfo->u1PicStruct = prVdec->rDecParam.u1PicStruct;
    prDispInfo->u4W = prVdec->rDecParam.prCurrFBufInfo->u4W;
    prDispInfo->u4H = prVdec->rDecParam.prCurrFBufInfo->u4H;
    prDispInfo->u4ChromaFormatIdc = prVdec->rDecParam.prSPS->u4ChromaFormatIdc;
#endif
    
}


void _VDEC_AVSSetParam(UCHAR ucEsId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3)
{
    VDEC_AVS_INFO_T* prVdec;
    
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_AVS_INFO_T*)&rAVSDecInfo[ucEsId];

    if(u4Type == (UINT32)AVS_OUTPUT_DISP_ORDER)
    {
        if(u4Param1 > 0)            //compare enable, decoding order
        {
            prVdec->rDecParam.fgOutDispOrder = FALSE;
        }
        else
        {
            prVdec->rDecParam.fgOutDispOrder = TRUE;
        }
    }
    else if(u4Type == (UINT32)AVS_REPEAT_FRM)
    {
        //repeat frame
        if(u4Param1 > 0)
        {
            if(prVdec->rDecParam.u1RptFrm==0)
            {
                prVdec->rDecParam.u1RptFrm = (UINT8)u4Param1;
            }
        }
        else
        {
            prVdec->rDecParam.u1RptFrm = 0;
        }
    }
    else if(u4Type == (UINT32)AVS_ECC_MODE)
    {
        //u4Param1: Enable error re-concealment, u4Param2: Mode, u4Param3: Dedicate error
        prVdec->rDecParam.u1ECMode = (UINT8)((u4Param1 | (u4Param2 << 2)) | (u4Param3 << 1));
    }
    else if(u4Type == (UINT32)AVS_ECC_TYPE)
    {
        //u4Param1: Enable error re-concealment, u4Param2: Mode, u4Param3: Dedicate error
        prVdec->rDecParam.u4ECType = u4Param1;
    }
    else if(u4Type == (UINT32)AVS_LOG_SIMU_PAT)
    {
        prVdec->rDecParam.fgLogSimPat = ((u4Param1>0)?TRUE:FALSE);
    }
    else if(u4Type == (UINT32)AVS_WAIT_FB_TIME)
    {
        prVdec->rDecParam.u4WaitFbTime = u4Param1;
    }
    else if(u4Type == (UINT32)AVS_AV_SYNC_INFO)
    {
        prVdec->rDecParam.ucSyncMode = (UCHAR)u4Param1;
        prVdec->rDecParam.ucStcId = (UCHAR)u4Param2;
    }
    else if(u4Type == (UINT32)AVS_TIME_PROFILE2)
    {
        prVdec->rDecParam.u1DramMoniter = (UINT8)u4Param1;
    }
    else if(u4Type==(UINT32)AVS_MM_PARAM)
    {
        prVdec->rDecParam.fgIsMMPlay = (u4Param1>0)?TRUE:FALSE;
        if(prVdec->rDecParam.fgIsMMPlay)
        {
            prVdec->rDecParam.u4WaitFbTime = VDEC_AVSYNC_WAIT_DISP_TIME;
        }
    }
}


void _VDEC_AVSGetParam(UCHAR ucEsId, UINT32 u4Type, UINT32* pu4Param1, UINT32* pu4Param2, UINT32* pu4Param3)
{
    VDEC_AVS_INFO_T* prVdec;
    
    ASSERT(ucEsId < MAX_ES_NUM);
    ASSERT(pu4Param1 !=NULL);
    ASSERT(pu4Param2 !=NULL);
    ASSERT(pu4Param3 !=NULL);

    prVdec = (VDEC_AVS_INFO_T*)&rAVSDecInfo[ucEsId];

    if(u4Type == (UINT32)AVS_DISP_IDX)
    {
        *pu4Param1 = (UINT32)prVdec->rDecParam.ucFbgId;
        if(prVdec->rDecParam.prCurrFBufInfo)
        {
            *pu4Param2 = (UINT32)prVdec->rDecParam.prCurrFBufInfo->ucFbId;
        }
    }
    else if(u4Type == (UINT32)AVS_OUTPUT_DISP_ORDER)
    {
        if(prVdec->rDecParam.fgOutDispOrder)            //compare enable, decoding order
        {
            *pu4Param1 = 0;
        }
        else
        {
            *pu4Param1 = 1;
        }
    }
    else if(u4Type == (UINT32)AVS_REPEAT_FRM)
    {
        *pu4Param1 = prVdec->rDecParam.u1RptFrm;
    }
    else if(u4Type == (UINT32)AVS_ECC_MODE)
    {
        //u4Param1: Enable error re-concealment, u4Param2: Mode, u4Param3: Dedicate error
        *pu4Param1 = (UINT32)(prVdec->rDecParam.u1ECMode & 0x1);
        *pu4Param2 = (UINT32)(prVdec->rDecParam.u1ECMode & 0xFC) >> 2;
        *pu4Param3 = (UINT32)(prVdec->rDecParam.u1ECMode & 0x2) >> 1;
    }
    else if(u4Type == (UINT32)AVS_ECC_TYPE)
    {
        *pu4Param1 = AVS_VLD_READ32(RW_AVLD_ERC_DED_ERR_TYPE);
    }    
    else if(u4Type == (UINT32)AVS_TIME_PROFILE)
    {
        *pu4Param1 = prVdec->rDecParam.ucFbgId;
        *pu4Param2 = prVdec->rDecParam.u4CurDecMs;       //hw dec
        *pu4Param3 = prVdec->rDecParam.u4CurWaitFbMicro/1000;   //wait fb time
    }
    else if(u4Type == (UINT32)AVS_TIME_PROFILE2)
    {
#if 1
         *pu4Param1 = prVdec->rDecParam.u4CurRstMs;   //wait fb time
         prVdec->rDecParam.u4CurRstMs = 0;
         switch (prVdec->rPicHdr.u4PicCodingType)
         {
            case I_IMG:
                *pu4Param2 = 2;
                break;
            case P_IMG:
                *pu4Param2 = 0;
                break;
            case B_IMG:
                *pu4Param2 = 1;
                break;
         }
         *pu4Param3 = 1;
#if 0
         *pu4Param3 = (UINT32)((prVdec->rDecParam.prCurrFBufInfo->u1FBufStatus == FRAME)?1:0);
#endif
#endif         
    }
    else if(u4Type == (UINT32)AVS_TIME_PROFILE3)
    {
        *pu4Param1 = prVdec->rDecParam.u4CurSwMicro;
        *pu4Param2 = prVdec->rDecParam.u4CurDecCycle;
        *pu4Param3 = prVdec->rDecParam.u4CurDramCycle;
    }
    else if(u4Type == (UINT32)AVS_BS_TYPE)
    {
#if 0
        *pu4Param1 = prVdec->rDecParam.prSPS->u4ProfileIdc;
        *pu4Param2 = prVdec->rDecParam.prSPS->u4LevelIdc;
        *pu4Param3 = (UINT32)prVdec->rDecParam.prPPS->fgEntropyCodingModeFlag;        
#endif        
    }
    else if(u4Type == (UINT32)AVS_SEQ_HDR_INFO)
    {    
#if 0
        *pu4Param1 = (prVdec->rDecParam.prSPS->u4PicWidthInMbsMinus1 + 1) << 4;
        *pu4Param2 = ((2 -prVdec->rDecParam.prSPS->fgFrameMbsOnlyFlag)*
            (prVdec->rDecParam.prSPS->u4PicHeightInMapUnitsMinus1 + 1)) << 4; //32
#endif        
    }
}


void _VDEC_AVSDropFrame(UCHAR ucEsId)
{
    VDEC_AVS_INFO_T* prVdec;
    UINT32 u4CurRpt;
    
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_AVS_INFO_T*)&rAVSDecInfo[ucEsId];
    
    u4CurRpt = prVdec->rPesInfo.u4VldReadPtr;
    
    if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, u4CurRpt, u4CurRpt))                
    {                    
        LOG(3, "AVS ES(%d) DMX_UpdateVideoReadPointer Fail\n", prVdec->rPesInfo.ucEsId);                
    }
}


void _VDEC_AVSUpdDmxRptr(UCHAR ucEsId)
{
    VDEC_AVS_INFO_T* prVdec;
    ASSERT(ucEsId < MAX_ES_NUM);

    prVdec = (VDEC_AVS_INFO_T*)&rAVSDecInfo[ucEsId];
#if 1
    UINT32 u4CurRptr, u4UsedSz = 0;
    
    prVdec = (VDEC_AVS_INFO_T*)&rAVSDecInfo[ucEsId];

    _VDEC_GetAvsRptr(&u4CurRptr);

    u4UsedSz = (u4CurRptr > prVdec->rPesInfo.u4VldReadPtr)?(u4CurRptr - prVdec->rPesInfo.u4VldReadPtr):
        ((prVdec->rPesInfo.u4FifoEnd - prVdec->rPesInfo.u4VldReadPtr) + (u4CurRptr - prVdec->rPesInfo.u4FifoStart));

    if(u4UsedSz > (WPTR_OFFSET + RPTR_ALIGN))
    {
        u4CurRptr -= (WPTR_OFFSET + RPTR_ALIGN);
    }
    if(u4CurRptr < prVdec->rPesInfo.u4FifoStart)
    {
        u4CurRptr += (prVdec->rPesInfo.u4FifoEnd - prVdec->rPesInfo.u4FifoStart);
    }
#endif

    //update dmx pointer, use current pes start addr, hw rptr may go beyond frame end
    //if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, u4CurRptr, u4CurRptr))
    if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, prVdec->rPesInfo.u4VldReadPtr, prVdec->rPesInfo.u4VldReadPtr))
    {                    
        LOG(2, "AVS ES(%d) DMX_UpdVRPtr Fail VS 0x%x, VE 0x%x, PicS 0x%x, DmxWptr 0x%x\n", 
            prVdec->rPesInfo.ucEsId,  
            prVdec->rPesInfo.u4FifoStart, prVdec->rPesInfo.u4FifoEnd, 
            prVdec->rPesInfo.u4VldReadPtr, prVdec->rDecParam.u4WPtr);
    }
}


void _VDEC_AVSSetDmxWptr(UCHAR ucEsId, UINT32 u4Wptr)
{
    VDEC_AVS_INFO_T* prVdec;    

    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_AVS_INFO_T*)&rAVSDecInfo[ucEsId];
    
    prVdec->rDecParam.u4WPtr = u4Wptr;
    _VDEC_SetAvsWptr(u4Wptr);
}


//skip ES if return FALSE
BOOL _VDEC_AVSEsFlwCtrl(UCHAR ucEsId, UCHAR* pucVldId, INT8 i1EsBufStatus, UCHAR ucSkipMode)
{
    VDEC_AVS_INFO_T* prVdec;

    ASSERT(ucEsId < MAX_ES_NUM);
    ASSERT(pucVldId !=NULL);
    prVdec = (VDEC_AVS_INFO_T*)&rAVSDecInfo[ucEsId];

    prVdec->rDecParam.ucSkipMode = ucSkipMode;

    // ask mpeg decode to skip one picture due to repeat
    // if ESM-Q Ns > Underflow Threshold, otherwise, ignore
    {
        //don't skip, for 2x->1x, decode but not display
        /*if(ucSkipMode == IPMode)
        {
            if(!_VDEC_SkipFrm(&prVdec->rDecParam))
            {
                return FALSE;
            }
            else
            {
                return TRUE;    //skip current packet
            }
        }*/

#if 0
        UCHAR ucFbgId;

        VDEC_ES_INFO_T* prVdecEsInfo;

        ucFbgId = prVdec->rDecParam.ucFbgId;
        if (ucFbgId != FBM_FBG_ID_UNKNOWN)
        {
            if (FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_SKIP_PIC))
            {
                //FBM_ClrFrameBufferFlag(ucFbgId, FBM_FLAG_SKIP_PIC);

                if (i1EsBufStatus >= 0)
                {
                    //update dmx
                    if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, 
                        prVdec->rPesInfo.u4VldReadPtr, prVdec->rPesInfo.u4VldReadPtr))                
                    {                    
                        LOG(2, "EsFlwCtrl AVS ES(%d) DMX_UpdateVideoReadPointer Fail\n", prVdec->rPesInfo.ucEsId);                
                    }

                      // [LOG] Video plane ask mpeg to drop 1 picture, set fbm empty if decode partial frame
                    if(!_VDEC_AvsSkipFrm(&prVdec->rDecParam))
                    {
                        return FALSE;
                    }
                    else
                    {
                        FBM_ClrFrameBufferFlag(ucFbgId, FBM_FLAG_SKIP_PIC);
                        return TRUE;    //skip current packet
                    }
                }
                // [LOG] Video plane ask mpeg to drop 1 picture, mpeg don't drop since Q is near empty
            }
            else
            if (FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_SKIP_PIC_NO_PTS))
            {
                if (prVdec->rPesInfo.u4PTS == 0)
                {
                    FBM_ClrFrameBufferFlag(ucFbgId, FBM_FLAG_SKIP_PIC_NO_PTS);

                    if (i1EsBufStatus >= 0)
                    {
                        LOG(3, "Q S P VLD(%d) Es(%d) G(%d) RP(0x%08x)\n",
                        *pucVldId, ucEsId, ucFbgId, prVdec->rPesInfo.u4VldReadPtr);

                        //update dmx
                        if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, 
                            prVdec->rPesInfo.u4VldReadPtr, prVdec->rPesInfo.u4VldReadPtr))                
                        {                    
                            LOG(2, "EsFlwCtrl AVS ES(%d) DMX_UpdateVideoReadPointer Fail\n", prVdec->rPesInfo.ucEsId);                
                        }
                        // [LOG] Video plane ask mpeg to drop 1 picture, set fbm empty if decode partial frame
                        if(!_VDEC_AvsSkipFrm(&prVdec->rDecParam))
                        {
                            return FALSE;
                        }

                        *pucVldId = VDEC_MAX_VLD;
                        return FALSE;
                    }
                }
                // [LOG] Video plane ask mpeg to drop 1 picture, mpeg don't drop since Q is near empty
            }
        }
#endif
        
        //dmx input speed faster than consumption
        if (i1EsBufStatus > 0)
        {
            LOG(3, "Q %d ucDropBNs\n", ucEsId);

            //update dmx
            if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, 
                prVdec->rPesInfo.u4VldReadPtr, prVdec->rPesInfo.u4VldReadPtr))                
            {                    
                LOG(2, "EsFlwCtrl AVS ES(%d) DMX_UpdateVideoReadPointer Fail\n", prVdec->rPesInfo.ucEsId);                
            }  
            // [LOG] Video plane ask mpeg to drop 1 picture, set fbm empty if decode partial frame
            if(!_VDEC_AvsSkipFrm(prVdec))
            {
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }
    }

#if 0
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I))
    {
        LOG(3, "DEC_FLG_WAIT_NEXT_I FlushDPB\n");
        _VDEC_FlushDPB(&prVdec->rDecParam);
        CLR_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    }
    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_WAIT_AVC_RAP))
    {
        LOG(3, "DEC_FLG_WAIT_AVC_RAP\n");
        prVdec->rDecParam.rSEI.u1ExtMatchFlg = FALSE;
        prVdec->rDecParam.rSEI.u1WaitRAPCnt = 0;
        prVdec->rDecParam.rSEI.u1WaitRAPRefCnt = 0;
        prVdec->rDecParam.rSEI.u1WaitRAPICnt = 0;
        prVdec->rDecParam.fgIsFirstFrm = FALSE;
        #ifdef ENABLE_MULTIMEDIA
        if(((prVdecEsInfo->eContainerType >= SWDMX_FMT_MPEG2_TS) &&
            (prVdecEsInfo->eContainerType <= SWDMX_FMT_MPEG2_TS_TIME_SHIFT)) ||
            (prVdecEsInfo->eContainerType == SWDMX_FMT_AVS_VIDEO_ES))
        {
            prVdec->rDecParam.fgWaitPPS = TRUE;
        }
        #endif
        CLR_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_AVC_RAP);
    }
    if (ucFbgId != FBM_FBG_ID_UNKNOWN)
    {
        // Check if reset is required
        if (FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_RESET))
        {
            LOG(3, "Disp rst flag on~\n");

            _VDEC_FlushDPB(&prVdec->rDecParam);
            prVdec->rDecParam.rLastInfo.u4LastFrameNum = 0xffffffff;
            prVdec->rDecParam.fgStartPlay = FALSE;
            
            prVdec->rDecParam.rSEI.u1ExtMatchFlg = FALSE;
            prVdec->rDecParam.rSEI.u1WaitRAPCnt = 0;
            prVdec->rDecParam.rSEI.u1WaitRAPRefCnt = 0;
            prVdec->rDecParam.rSEI.u1WaitRAPICnt = 0;            
            prVdec->rDecParam.fgIsFirstFrm = FALSE;

            FBM_ResetGroup(ucFbgId);
            FBM_ClrFrameBufferFlag(ucFbgId, FBM_FLAG_RESET);
            FBM_ClrFrameBufferFlag(ucFbgId, FBM_FLAG_NOT_READY);
            FBM_SetFrameBufferFlag(ucFbgId, FBM_FLAG_CLEAR_FRC_PRM);

            *pucVldId = VDEC_MAX_VLD;
            return TRUE;    //skip current packet
        }
        // Check if display is ready, skip picture if NOT
        if (!FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_DISP_READY))
        {
            LOG(3, "Disp Not Ready\n");
            *pucVldId = VDEC_MAX_VLD;
            return TRUE;    //skip current packet
        }
    }    
#endif
    
    return FALSE;
}


BOOL _VDEC_IsSupportAVS()
{
    //return IS_SupportAVS();
    return FALSE;
}


UCHAR _VDEC_AVSHandleOverFlw(UCHAR ucVldId, UCHAR ucEsmQId, HANDLE_T hMutex,
    HANDLE_T hMsgQueue, const void* pvPesInfo)
{
    ASSERT(pvPesInfo!=NULL);
    //temp code
    do{
        x_thread_delay(1);
    }while(x_msg_q_send(hMsgQueue, pvPesInfo, sizeof(VDEC_PES_INFO_T), 255) != OSR_OK);
    
    UNUSED(ucVldId);
    UNUSED(ucEsmQId);
    UNUSED(hMutex);
    return 1;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------



