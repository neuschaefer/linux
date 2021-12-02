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
 * $RCSfile: vdec_h264dec.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_h264api.c
 *  This file contains implementation of exported APIs of VDEC.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
#include "x_ckgen.h"
#include "srm_drvif.h"
#include "vdec_h264hw.h"
#include "vdec_h264util.h"
#include "vdec_h264dec.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_common.h"
#include "x_typedef.h"
#include "x_timer.h"
#include "stc_drvif.h"
#include "x_util.h"
#ifdef FBM_DPB_CTRL
#include "fbm_drvif.h"
#endif
#include "vdec_drvif.h"
#include "drv_dbase.h"
#include "drvcust_if.h"
LINT_EXT_HEADER_END


//for emulation
//EXTERN void _VdecNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);

//-----------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define POC_abs(x)                  (((x) >= 0) ? (x) : -(x))
#define FB_ALIGN  0xF
#define FRAME_RATE_MAX 9
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
//static BOOL _fgVDecComplete = FALSE;
static HAL_TIME_T rTimeHwS, rTimeHwDt,rTimeHwE;
static UINT32 u4DecCycle = 0, u4MaxDecCycle = 0;
//follow FBM defined MPEG_FRAME_RATE_24_...

static UINT64 au8Scale[FRAME_RATE_MAX] = {
    0,
    375374700,
    3750,
    3600,
    300300300,
    3000,
    1800,
    150075037,
    1500
};
static UINT32 au4Rate[9] = {
    0,
    100000,
    1,
    1,
    100000,
    1,
    1,
    100000,
    1
};

#ifndef AVC_IRQ_DISABLE
static HANDLE_T _hVdecFinSema = (HANDLE_T)NULL;
static BOOL _bTimeOutFlag = FALSE;
#endif

CHAR quant_intra_default[16] = {
 6,13,20,28,
13,20,28,32,
20,28,32,37,
28,32,37,42
};

CHAR quant_inter_default[16] = {
10,14,20,24,
14,20,24,27,
20,24,27,30,
24,27,30,34
};

CHAR quant8_intra_default[64] = {
 6,10,13,16,18,23,25,27,
10,11,16,18,23,25,27,29,
13,16,18,23,25,27,29,31,
16,18,23,25,27,29,31,33,
18,23,25,27,29,31,33,36,
23,25,27,29,31,33,36,38,
25,27,29,31,33,36,38,40,
27,29,31,33,36,38,40,42
};

CHAR quant8_inter_default[64] = {
 9,13,15,17,19,21,22,24,
13,13,17,19,21,22,24,25,
15,17,19,21,22,24,25,27,
17,19,21,22,24,25,27,28,
19,21,22,24,25,27,28,30,
21,22,24,25,27,28,30,32,
22,24,25,27,28,30,32,33,
24,25,27,28,30,32,33,35
};

CHAR quant_org[16] = { //to be use if no q matrix is chosen
16,16,16,16,
16,16,16,16,
16,16,16,16,
16,16,16,16
};

CHAR quant8_org[64] = { //to be use if no q matrix is chosen
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16
}; 

#ifdef VDEC_TIME_PROFILE
static UINT32 u4DelayClock;
static UINT32 u4DramCycle;
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static void _vAssignQuantParam(VDEC_AVC_DecPrm *prDecPrm);
static void _vPartitionDPB(VDEC_AVC_DecPrm *prDecPrm);
static void _vAllocateFBuf(VDEC_AVC_DecPrm *prDecPrm);

//static void _vFillFrameNumGap(VDEC_AVC_DecPrm *prDecPrm);
static void _vPrepareFBufInfo(VDEC_AVC_DecPrm *prDecPrm);

static BOOL _fgChkRefInfo(const VDEC_AVC_DecPrm* prDecPrm, UINT32 u4FBufIdx, UINT32 u4RefType);
static void _vInsertRefPicList(const VDEC_AVC_DecPrm* prDecPrm, VDEC_REF_PIC_LIST_T* prRefPicList, 
    INT32 i4CurrPOC, UINT32 u4RefPicListInfo);

static void _vSetupPRefPicList(const VDEC_REF_PIC_LIST_T* prRefPicList, const VDEC_FBUF_INFO_T* prFBufInfo, 
    UINT8 u1PicStruct, UINT32 *pu4RefIdx, UINT32 u4TFldListIdx, UINT32 u4BFldListIdx);

static void _vSetPRefPicList(VDEC_AVC_DecPrm* prDecPrm);

static void _vSetupBRefPicList(const VDEC_REF_PIC_LIST_T* prRefPicList, const VDEC_FBUF_INFO_T* prFBufInfo, 
    UINT8 u1PicStruct, UINT32 *pu4RefIdx, UINT32 u4TFldListIdx, UINT32 u4BFldListIdx, BOOL *pfgDiff);

static void _vSetBRefPicList(VDEC_AVC_DecPrm* prDecPrm);

static void _vSetPicRefType(VDEC_FBUF_INFO_T* prFBufInfo, UINT8 u1PicStruct, UINT8 u1RefType);
static UINT8 _bGetPicRefType(VDEC_FBUF_INFO_T rFBufInfo, UINT8 u1PicStruct);
static void _vClrPicRefInfo(VDEC_AVC_DecPrm* prDecPrm, UINT8 u1PicType, UINT8 u1FBufIdx);
static void _vAdapRefPicmarkingProce(VDEC_AVC_DecPrm* prDecPrm);
static BOOL _bNotifyAudStartPlay(UCHAR ucStcId, UINT32 u4PTS);
static void _vECBufSetting(const VDEC_AVC_DecPrm* prDecPrm, UINT32 u4RefCnt, UINT32 u4RefEntryCnt);

//static void _vSaveChkSum(void);

#ifdef FBM_DPB_CTRL
extern void FBM_UpdateReferenceList(UCHAR ucFbgId, UCHAR ucFbId, BOOL fgReference);
#endif

#ifndef AVC_IRQ_DISABLE
static void _AVLDIsr(UINT16 u2Vector);

// *********************************************************************
// Function : void _AVLDIsr
// Description : 
// Parameter : 
// Return    : 
// *********************************************************************

static void _AVLDIsr(UINT16 u2Vector)
{
    #ifdef VDEC_TIME_PROFILE
    rTimeHwDt.u4Micros = 0;
    #endif
    
    if (u2Vector == AVLD_VECTOR)
    {
        if (AVC_VLD_READ32(RO_AVLD_COMPLETE))
        {
            AVC_VLD_WRITE32(RW_AVLD_CTRL, AVC_VLD_READ32(RW_AVLD_CTRL) & AVC_DEC_CYCLE_DISABLE);
            AVC_VLD_WRITE32(RW_AVLD_COMPLETE_CLR, 1);
            AVC_VLD_WRITE32(RW_AVLD_COMPLETE_CLR, 0);
            //#ifdef VDEC_TIME_PROFILE
            HAL_GetTime(&rTimeHwE);
            HAL_GetDeltaTime(&rTimeHwDt, &rTimeHwS, &rTimeHwE);
            //#endif            
        }
        else if(AVC_VLD_READ32(RO_AVLD_ERR_MESSAGE) & AVLD_ERR_MESSAGE_MSK)
        {
            _bTimeOutFlag = TRUE;
            AVC_VLD_WRITE32(RW_AVLD_TIMEOUT_CLR, 1);
            AVC_VLD_WRITE32(RW_AVLD_TIMEOUT_CLR, 0);
        }
        else
        {
            _bTimeOutFlag = TRUE;
        }
        //unlock semaphore
        VERIFY(x_sema_unlock(_hVdecFinSema) == OSR_OK);
    }
}

#endif

// *********************************************************************
// Function    : BOOL _bNotifyAudStartPlay(void)
// Description : notify audio when first video frame ready to play
// Parameter   : None
// Return      : None
// *********************************************************************
static BOOL _bNotifyAudStartPlay(UCHAR ucStcId, UINT32 u4PTS)
{
    if (STC_SetStartPts(AV_SYNC_TARGET_VDO, ucStcId, u4PTS) != STC_VALID)
    {
        LOG(3, "STC_SetStartPts Invalid\n");
        return FALSE;
    }
    LOG(5, "STC_SetStartPts 0x%x\n", u4PTS);
    
    return TRUE;
}             

// *********************************************************************
// Function : void AssignQuantParam(void)
// Description : 
// Parameter : 
// Return    : 
// *********************************************************************
static void _vAssignQuantParam(VDEC_AVC_DecPrm *prDecPrm)
{
    UINT32 i;
    CHAR *pactList[8];

    ASSERT(prDecPrm!=NULL);

    for(i=0; i<8; i++)
    {
        pactList[i] = quant_intra_default;        //initial
        prDecPrm->afgUserScalingListPresentFlag[i] = FALSE;
    }
   
    if((!prDecPrm->prPPS->fgPicScalingMatrixPresentFlag) && (!prDecPrm->prSPS->fgSeqScalingMatrixPresentFlag))
    {
        prDecPrm->fgUserScalingMatrixPresentFlag = FALSE;
        // do nothing
    }
    else
    {
        prDecPrm->fgUserScalingMatrixPresentFlag = TRUE;
        if(prDecPrm->prSPS->fgSeqScalingMatrixPresentFlag) // check sps first
        {
            for(i=0; i<8; i++)
            {
                prDecPrm->afgUserScalingListPresentFlag[i] = TRUE;    
                //need to have the flag for pps info setting, for hw to reload data from sram
                //_VDEC_WriteScalingList only update the sram data
                if(i<6)
                {
                    if(!prDecPrm->prSPS->fgSeqScalingListPresentFlag[i]) // fall-back rule A
                    {
                        if((i==0) || (i==3))
                        {
                            pactList[i] =  (i==0) ? quant_intra_default:quant_inter_default;
                            _VDEC_WriteScalingList(i, pactList[i]);  
                        }
                        else
                        {
                            pactList[i] =  pactList[i-1];
                            prDecPrm->afgUserScalingListPresentFlag[i] = prDecPrm->afgUserScalingListPresentFlag[i-1];
                            _VDEC_WriteScalingList(i, pactList[i]);  
                        }
                    }
                    else // fall-back rule B
                    {
                        if(prDecPrm->prSPS->fgUseDefaultScalingMatrix4x4Flag[i])
                        {
                            pactList[i] = (i<3) ? quant_intra_default:quant_inter_default;
                            _VDEC_WriteScalingList(i, pactList[i]);  
                        }
                        else
                        {
                            prDecPrm->afgUserScalingListPresentFlag[i] = TRUE;
                            pactList[i] = prDecPrm->prSPS->ppcScalingList4x4[i];
                            _VDEC_WriteScalingList(i, pactList[i]);                
                        }
                    }
                }
                else
                {
                    if(!prDecPrm->prSPS->fgSeqScalingListPresentFlag[i] || prDecPrm->prSPS->fgUseDefaultScalingMatrix8x8Flag[i-6]) // fall-back rule A
                    {
                        pactList[i] = (i==6) ? quant8_intra_default:quant8_inter_default;
                        _VDEC_WriteScalingList(i, pactList[i]);  
                    }
                    else
                    {
                        prDecPrm->afgUserScalingListPresentFlag[i] = TRUE;
                        pactList[i] = prDecPrm->prSPS->ppcScalingList8x8[i-6];
                        _VDEC_WriteScalingList(i, pactList[i]);
                    }
                }
            }
        }
    
        if(prDecPrm->prPPS->fgPicScalingMatrixPresentFlag) // then check pps
        {
            for(i=0; i<8; i++)
            {
                if(i<6)
                {
                    if(!prDecPrm->prPPS->fgPicScalingListPresentFlag[i]) // fall-back rule A
                    {
                        if((i==0) || (i==3))
                        {              
                            if(!prDecPrm->prSPS->fgSeqScalingMatrixPresentFlag)
                            {
                                pactList[i] = (i==0) ? quant_intra_default:quant_inter_default;
                                _VDEC_WriteScalingList(i, pactList[i]);  
                            }
                        }
                        else
                        {
                            pactList[i] = pactList[i-1];
                            prDecPrm->afgUserScalingListPresentFlag[i] = prDecPrm->afgUserScalingListPresentFlag[i-1];
                            _VDEC_WriteScalingList(i, pactList[i]);  
                        }
                    }
                    else
                    {
                        if(prDecPrm->prPPS->fgUseDefaultScalingMatrix4x4Flag[i])
                        {
                            pactList[i] = (i<3) ? quant_intra_default:quant_inter_default;
                            _VDEC_WriteScalingList(i, pactList[i]);  
                        }
                        else
                        {
                            pactList[i] = prDecPrm->prPPS->ppcScalingList4x4[i];
                            prDecPrm->afgUserScalingListPresentFlag[i] = TRUE;
                            _VDEC_WriteScalingList(i, pactList[i]);
                        }
                    }
                }
                else
                {
                    if(!prDecPrm->prPPS->fgPicScalingListPresentFlag[i]) // fall-back rule B
                    {
                        if(!prDecPrm->prSPS->fgSeqScalingMatrixPresentFlag)
                        {
                            pactList[i] = (i==6) ? quant8_intra_default:quant8_inter_default;
                            _VDEC_WriteScalingList(i, pactList[i]);  
                        }
                    }
                    else if(prDecPrm->prPPS->fgUseDefaultScalingMatrix8x8Flag[i-6])
                    {
                        pactList[i] = (i==6) ? quant8_intra_default:quant8_inter_default;
                        _VDEC_WriteScalingList(i, pactList[i]);  
                    }
                    else
                    {
                        pactList[i] = prDecPrm->prPPS->ppcScalingList8x8[i-6];
                        prDecPrm->afgUserScalingListPresentFlag[i] = TRUE;
                        _VDEC_WriteScalingList(i, pactList[i]);            
                    }
                }
            }
        }
    }
}

// *********************************************************************
// Function    : void vPartitionDPB(void)
// Description : Set VDec related parameters
// Parameter   : None
// Return      : None
// *********************************************************************
static void _vPartitionDPB(VDEC_AVC_DecPrm *prDecPrm)
{
    UINT32 i, u4FbCnt = 0, u4DPBExpCnt = 0;
    UINT32 u4DramPicSize;
    UINT32 u4DramPicArea;
    UINT32 u4YAddr, u4CAddr, u4MVAddr;

    ASSERT(prDecPrm!=NULL);

    // Real pic size w=16x, h=32x  
    u4DramPicSize = ((((prDecPrm->u4PicW + 15) >> 4) * ((prDecPrm->u4PicH + 31) >> 5)) << 9);
    // 1 pic area = Y + CbCr + restored MV
    u4DramPicArea = ((((u4DramPicSize * 7) >> 2) + 511) >> 9)<< 9;

    #ifdef FBM_DPB_CTRL
    prDecPrm->u4MaxFBufNum = //prDecPrm->prSPS->u4NumRefFrames + 1;
    (prDecPrm->u4DPBSize / (((prDecPrm->u4PicW * prDecPrm->u4PicH)*3)>>1)) + 1;

    //check if ref frame num over spec
    u4DPBExpCnt = prDecPrm->prSPS->u4NumRefFrames + 1;
    if(u4DPBExpCnt > prDecPrm->u4MaxFBufNum)
    {
        u4FbCnt = (UINT32)(FBM_GetFrameBufferNs(prDecPrm->ucFbgId) - 2);  // 2 lock frame
        if(u4FbCnt >= u4DPBExpCnt)
        {
            prDecPrm->u4MaxFBufNum = u4DPBExpCnt;
            LOG(2, "!!NumRef %d > Level Limit, Enlarge MaxFBuf\n", prDecPrm->prSPS->u4NumRefFrames);
        }
        else
        {
            prDecPrm->fgNotSupport = TRUE;
            LOG(2, "!!NumRef %d > Level Limit, Not support\n", prDecPrm->prSPS->u4NumRefFrames);
        }
    }
    prDecPrm->u4MaxFBufNum = (prDecPrm->u4MaxFBufNum > 17)?((UINT32)17):prDecPrm->u4MaxFBufNum;
    prDecPrm->ucEmpMVBufMaxNum = (UCHAR)prDecPrm->u4MaxFBufNum;
    //MV buffer array
    for(i=0; i<prDecPrm->ucEmpMVBufMaxNum; i++)
    {
        prDecPrm->aucEmpMVBufIdxMap[i] = (UCHAR)i;
    }
    FBM_GetFrameBufferAddr(prDecPrm->ucFbgId, 0, &u4YAddr, &u4CAddr);
    FBM_GetFrameBufferAddrMv(prDecPrm->ucFbgId, 0, &u4MVAddr);
    //MC_WRITE32(RW_MC_P_LIST0, PHYSICAL(u4YAddr));
    #else
    prDecPrm->u4MaxFBufNum = (prDecPrm->u4DPBSize / (((prDecPrm->u4PicW * prDecPrm->u4PicH)*3)>>1)) + 2;
    if(prDecPrm->u4MaxFBufNum > 18)
    {
        prDecPrm->u4MaxFBufNum = 18;    //16 reference frame + 1 cur dec + 1 cur disp
    }
    #endif
    ASSERT(prDecPrm->u4MaxFBufNum <= 18);
    for(i = 0; i < prDecPrm->u4MaxFBufNum; i++)
    {
        prDecPrm->arFBufInfo[i].u4W = prDecPrm->u4PicW;
        prDecPrm->arFBufInfo[i].u4H = prDecPrm->u4PicH;    
        prDecPrm->arFBufInfo[i].u4DramPicSize = u4DramPicSize;
        prDecPrm->arFBufInfo[i].u4DramPicArea = u4DramPicArea;
        #ifdef FBM_DPB_CTRL
        prDecPrm->arFBufInfo[i].u4YAddr = u4YAddr;
        prDecPrm->arFBufInfo[i].u4CAddr = u4CAddr;
        prDecPrm->arFBufInfo[i].u4MVBufAddr = u4MVAddr;        
        #else
        prDecPrm->arFBufInfo[i].u4YAddr = prDecPrm->u4DPBAddr + (i * u4DramPicArea);
        prDecPrm->arFBufInfo[i].u4CAddr = prDecPrm->arFBufInfo[i].u4YAddr + u4DramPicSize;
        prDecPrm->arFBufInfo[i].u4MVBufAddr = prDecPrm->arFBufInfo[i].u4CAddr + (u4DramPicSize>>1);
        #endif
    }
    // current reset to 0 when DPB partition.
    prDecPrm->prCurrFBufInfo = &prDecPrm->arFBufInfo[0];
}


// *********************************************************************
// Function    : void vAllocateFBuf()
// Description : Allocate decoding frm buff in DPB
// Parameter   : 
// Return      : None
// *********************************************************************
static void _vAllocateFBuf(VDEC_AVC_DecPrm *prDecPrm)
{
    UINT32 i;
    INT32 i4MinPOC;
    UINT32 u4MinPOCFBufIdx = 0xFF, u4ClrPOCFBufIdx = 0xFF, u4RefPOCFBufIdx = 0;
    #ifdef VDEC_TIME_PROFILE
    HAL_TIME_T rTimeFbS, rTimeFbE, rTimeFbDt;
    #endif
    #ifdef FBM_DPB_CTRL
    FBM_SEQ_HDR_T *prFbmSeqHdr;
    FBM_PIC_HDR_T * prFbmPicHdr;
    UCHAR ucFbId, ucFbStatus;
    VDEC_ES_INFO_T* prVdecEsInfo;
    VDEC_INFO_T *prVdecInfo;   
    
    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(0);
    #endif
    
    ASSERT(prDecPrm!=NULL);

    #ifdef FBM_DPB_CTRL
    //check duplicate frame_num
    prDecPrm->u4DupFrmNumId = 0xFF;
    ASSERT(prDecPrm->u4MaxFBufNum <= 18);
    for(i=0; (i<prDecPrm->u4MaxFBufNum) && (i<18); i++)
    {
        if(!fgIsNonRefFBuf(prDecPrm->arFBufInfo[i].u1FBufRefType, 
            prDecPrm->arFBufInfo[i].u1TFldRefType, 
            prDecPrm->arFBufInfo[i].u1BFldRefType))
        {
            if(!_fgChkRefInfo(prDecPrm, i, LREF_PIC) &&
                (prDecPrm->arFBufInfo[i].u4FrameNum == prDecPrm->prSliceHdr->u4FrameNum) &&
                fgIsRefPic(prDecPrm->u1NalRefIdc))
            {
                //can't clr buffer here, may still need to be reference, clr in _VDEC_OutputDispPic
                prDecPrm->u4DupFrmNumId = i;
                LOG(3, "Dup Frm_Num\n");
            }
        }
    }
    #endif
    
    // Check if DPB full
    i4MinPOC = 0x7fffffff;
    for(i=0; i<prDecPrm->u4MaxFBufNum; i++)
    {
        ucFbStatus = FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[i].ucFbId);
        if(prDecPrm->arFBufInfo[i].u1FBufStatus == NO_PIC)
        {
            i4MinPOC = 0x7fffffff;
            u4MinPOCFBufIdx = i;        
            break;
        }
        // miew: need to take care of field empty
        else if(fgIsNonRefFBuf(prDecPrm->arFBufInfo[i].u1FBufRefType, prDecPrm->arFBufInfo[i].u1TFldRefType, prDecPrm->arFBufInfo[i].u1BFldRefType))
        {
            if((ucFbStatus != FBM_FB_STATUS_DISPLAYQ) && (ucFbStatus != FBM_FB_STATUS_LOCK))
            {
                if(i4MinPOC > prDecPrm->arFBufInfo[i].i4POC)
                {
                    i4MinPOC = prDecPrm->arFBufInfo[i].i4POC;
                    u4MinPOCFBufIdx = i;
                }
                else
                {
                    u4ClrPOCFBufIdx = i;
                }
            }
        }
        else if((ucFbStatus != FBM_FB_STATUS_DISPLAYQ) && (ucFbStatus != FBM_FB_STATUS_LOCK))
        {
            u4RefPOCFBufIdx = i;
        }
    }
    
    //clr current frame status if still in the middle of decoding process        
    if(FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId) 
    == FBM_FB_STATUS_DECODE)
    {
        FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId, 
        FBM_FB_STATUS_READY);
    }    
    // error handling if can't find non-ref pic with minimun poc and not in displayQ
    if(u4MinPOCFBufIdx == 0xFF)
    {
        if(u4ClrPOCFBufIdx == 0xFF)
        {
            u4MinPOCFBufIdx = u4RefPOCFBufIdx;
        }
        else
        {
            u4MinPOCFBufIdx = u4ClrPOCFBufIdx;
        }
    }
    // No empty DPB, 1 FBuf output
    if(prDecPrm->arFBufInfo[u4MinPOCFBufIdx].u1FBufStatus != NO_PIC)
    {
        LOG(2, "///// ClrBuf in AllocFBuf, Idx %d , Status %d(>0)\n", 
            u4MinPOCFBufIdx, 
            (UINT32)FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId));
        //find an empty fb entry, put non-ref buf to empty
        FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId, 
        FBM_FB_STATUS_EMPTY);
        _VDEC_ClrFBufInfo(prDecPrm, u4MinPOCFBufIdx);
    }
    prDecPrm->u1DecFBufIdx = (UINT8)u4MinPOCFBufIdx;
    // Only new alloc needs to update current fbuf idx
    // set current buf index
    if(prDecPrm->u1DecFBufIdx > 17)
    {
        ASSERT(prDecPrm->u1DecFBufIdx <= 17);
        return;
    }
    prDecPrm->prCurrFBufInfo = &prDecPrm->arFBufInfo[prDecPrm->u1DecFBufIdx];
    prDecPrm->prCurrFBufInfo->u4SliceType = prDecPrm->prSliceHdr->u4SliceType;
    
    #ifdef FBM_DPB_CTRL
    //if freeze, still decoding normally, shorten delay time when unfreeze
    //but get fb from dispQ(set the attribute from disp to empty)
    if (FBM_ChkFrameBufferFlag(prDecPrm->ucFbgId , FBM_FLAG_FREEZE))
    {
        ucFbId = FBM_GetFrameBufferFromDispQ(prDecPrm->ucFbgId);
        FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
    }
    
    #ifdef VDP_RACE_LOG
    VDP_FrcRacingLog('G');
    #endif
    #ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&rTimeFbS);
    #endif
    prDecPrm->prCurrFBufInfo->ucFbId = FBM_GetEmptyFrameBuffer(prDecPrm->ucFbgId, prDecPrm->u4WaitFbTime);
    prVdecEsInfo->ucFbId = prDecPrm->prCurrFBufInfo->ucFbId;
    if(!prDecPrm->fgIsFirstFrm)
    {   //!display until find suitable RAP
        prDecPrm->prCurrFBufInfo->fgIsNotDisp = TRUE;
    }
    else if(prDecPrm->fgFirstFrmTag)
    {
        prDecPrm->prCurrFBufInfo->fgFirstFrmTag = TRUE;
    }

    if(prVdecEsInfo->fgMMPlayback)
    {
        if((prVdecEsInfo->u4MMSpeed == STC_SPEED_TYPE_FORWARD_2X) &&
            ((!fgIsRefPic(prDecPrm->u1NalRefIdc))))
        {
            prDecPrm->prCurrFBufInfo->fgIsNotDisp = TRUE;
        }
    }
    
    prDecPrm->u4CurDecMs = 0;
    #ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&rTimeFbE);
    HAL_GetDeltaTime(&rTimeFbDt, &rTimeFbS, &rTimeFbE);
    prDecPrm->u4CurWaitFbMicro += rTimeFbDt.u4Micros;///1000;
    if(prDecPrm->u4CurWaitFbMicro > 
        (33000 * (FBM_GetFrameBufferNs(prDecPrm->ucFbgId) - prDecPrm->u4MaxFBufNum))) //33 ms
    {
        LOG(6, "WFb %dus!!\n", prDecPrm->u4CurWaitFbMicro);
    }
    prDecPrm->u4CurDecCycle = 0;
    prDecPrm->u4CurDramCycle = 0;
    #endif
    
    if(prDecPrm->prCurrFBufInfo->ucFbId == FBM_FB_ID_UNKNOWN)
    {
        prDecPrm->fgAllocFbFail = TRUE;
        LOG(2, "AllocFB Fail!!\n");
        return;
    }
    else
    {
        prDecPrm->fgAllocFbFail = FALSE;
    }
    
    #ifdef VDP_RACE_LOG
    if(fgIsRefPic(prDecPrm->u1NalRefIdc))
    {
        VDP_FrcRacingLog('R');
    }
    else
    {
        VDP_FrcRacingLog('B');
    }        
    VDP_FrcRacingLog('0' + prDecPrm->prCurrFBufInfo->ucFbId);

    prVdecEsInfo = _VDEC_GetEsInfo(0);

    if (prVdecEsInfo->u2Count > 9)
    {
        VDP_FrcRacingLog('0' + 9);
    }
    else
    {
        VDP_FrcRacingLog('0' + prVdecEsInfo->u2Count);
    }
    #endif

    #ifdef VDEC_TIME_PROFILE
    if((prDecPrm->prSPS->rVUI.u4TimeScale > 0) &&
        ((rTimeFbDt.u4Micros / 1000) > 
        ((prDecPrm->prSPS->rVUI.u4NumUnitsInTick * 1000)/prDecPrm->prSPS->rVUI.u4TimeScale)))
    {
        #ifdef VDP_RACE_LOG
        VDP_FrcRacingLog('T');
        #endif
        prDecPrm->u4WaitFbCnt++;
    }
    #endif
    
    FBM_UpdateReferenceList(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId, fgIsRefPic(prDecPrm->u1NalRefIdc));
    FBM_GetFrameBufferAddr(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId,
        &prDecPrm->prCurrFBufInfo->u4YAddr, &prDecPrm->prCurrFBufInfo->u4CAddr);

    if((prDecPrm->prCurrFBufInfo->u4YAddr == (UINT32)NULL) || 
    	(prDecPrm->prCurrFBufInfo->u4CAddr == (UINT32)NULL))
    {
        prDecPrm->fgAllocFbFail = TRUE;
        LOG(2, "FbAddr NULL!!\n");
        //ASSERT(0);
        return;
    }
    
    if(fgIsRefPic(prDecPrm->u1NalRefIdc))
    {
        ASSERT(prDecPrm->u4MaxFBufNum <= 18);
        ASSERT((prDecPrm->ucEmpMVBufMaxNum > 0) && (prDecPrm->ucEmpMVBufMaxNum <= prDecPrm->u4MaxFBufNum));
        if((prDecPrm->ucEmpMVBufMaxNum > 0) && (prDecPrm->ucEmpMVBufMaxNum <=18))
        {
            prDecPrm->prCurrFBufInfo->ucMvBufIdx = prDecPrm->aucEmpMVBufIdxMap[prDecPrm->ucEmpMVBufMaxNum-1];
            //LOG(2, "malloc mvbufIdx %d, maxNum %d\n", prDecPrm->prCurrFBufInfo->ucMvBufIdx, prDecPrm->ucEmpMVBufMaxNum);
        }
        FBM_GetFrameBufferAddrMv(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucMvBufIdx,
            &prDecPrm->prCurrFBufInfo->u4MVBufAddr);

        if(prDecPrm->prCurrFBufInfo->u4MVBufAddr == (UINT32)NULL)
        {
            prDecPrm->fgAllocFbFail = TRUE;
            LOG(2, "FbMvAddr NULL!!\n");
            //ASSERT(0);
            return;
        }
        prDecPrm->ucEmpMVBufMaxNum--;
    }
    else
    {
        prDecPrm->prCurrFBufInfo->ucMvBufIdx = 0xFF;
        //prDecPrm->prCurrFBufInfo->u4MVBufAddr = 0;
    }
    LOG(9, "//AllocFBuf, FbId=%d, Ref=%d, Y=0x%x, C=0x%x, MV=0x%x, W=%d, H=%d, arFBufId %d\n", 
        prDecPrm->prCurrFBufInfo->ucFbId, fgIsRefPic(prDecPrm->u1NalRefIdc),
        prDecPrm->prCurrFBufInfo->u4YAddr, prDecPrm->prCurrFBufInfo->u4CAddr,
        prDecPrm->prCurrFBufInfo->u4MVBufAddr, prDecPrm->u4PicW, prDecPrm->u4PicH,
        prDecPrm->u1DecFBufIdx);

    //set display parameter, Progressive flag, TFF, RFF
    prFbmPicHdr = FBM_GetFrameBufferPicHdr(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId);
    if(prFbmPicHdr == NULL)
    {
        prDecPrm->fgAllocFbFail = TRUE;
        LOG(2, "GetFbgPicHdr Fail!!\n");
        //ASSERT(0);
        return;
    }
    #ifdef CC_RESIZE_SMALL_IMAGE
    prFbmPicHdr->u4PicWidth = prVdecEsInfo->u2OrgHSize;
    prFbmPicHdr->u4PicWidthPitch = prVdecEsInfo->u2LineSize;
    prFbmPicHdr->u4PicHeight = prVdecEsInfo->u2OrgVSize;
    #endif
    //DivX Plus TPT seemless resolution change
    if(prDecPrm->fgSeamlessDisp)
    {
        //default value
        prFbmPicHdr->fgSeamlessDisp = TRUE;
        prFbmPicHdr->u4PicWidth = prDecPrm->u4PicW;
        prFbmPicHdr->u4PicHeight = prDecPrm->u4PicH;
        prFbmPicHdr->u4PicWidthPitch = prDecPrm->u4PicW;

        //modified for MM
        if(prDecPrm->fgIsMMPlay)
        {
            if(prVdecEsInfo->rMMTPTSeqInfo.u2_width > 0)
            {
                prFbmPicHdr->u4PicWidth = prVdecEsInfo->rMMTPTSeqInfo.u2_width;
                prFbmPicHdr->u4PicHeight = prVdecEsInfo->rMMTPTSeqInfo.u2_height;
            }
            else if(prVdecEsInfo->rMMSeqInfo.u2_width > 0)
            {
                prFbmPicHdr->u4PicWidth = prVdecEsInfo->rMMSeqInfo.u2_width;
                prFbmPicHdr->u4PicHeight = prVdecEsInfo->rMMSeqInfo.u2_height;
            }
        }
    }
    else
    {
        prFbmPicHdr->fgSeamlessDisp = FALSE;
    }
    if(prDecPrm->prSPS->rVUI.fgPicStructPresentFlag &&
        (prDecPrm->rSEI.u1PicStruct != (UINT8)SEI_FRAME_UNKNOWN))
    {
        prFbmPicHdr->fgProgressiveFrm= 
            ((prDecPrm->rSEI.u1PicStruct == (UINT8)SEI_FRAME) || 
            (prDecPrm->rSEI.u1PicStruct == (UINT8)SEI_FRAME_DOUBLING) ||
            (prDecPrm->rSEI.u1PicStruct == (UINT8)SEI_FRAME_TRIPLING))?TRUE:FALSE;
    }
    else    // progressive/interlace display detect
    {
        if(prDecPrm->prSliceHdr->fgFieldPicFlag)
        {
            prFbmPicHdr->fgProgressiveFrm= FALSE;
            prDecPrm->rSEI.u1PicStruct = (UINT8)SEI_TOP_FIELD;
        }
        else
        {
            prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prDecPrm->ucFbgId);
            if(prFbmSeqHdr != NULL)
            {
                if(prVdecEsInfo->fgMMPlayback)
                {
                    prFbmPicHdr->fgProgressiveFrm = prFbmSeqHdr->fgProgressiveSeq;
                    if(prFbmPicHdr->fgProgressiveFrm)
                    {
                        prDecPrm->rSEI.u1PicStruct = (UINT8)SEI_FRAME;
                    }
                    else
                    {
                        prDecPrm->rSEI.u1PicStruct = (UINT8)SEI_TOP_FIELD;
                    }
                }
                else
                {
                    if((prFbmSeqHdr->ucFrmRatCod == (UCHAR)MPEG_FRAME_RATE_24) ||
                        (prFbmSeqHdr->ucFrmRatCod == (UCHAR)MPEG_FRAME_RATE_24_) ||
                        (prDecPrm->u4PicW == 1280))
                    {
                        prFbmPicHdr->fgProgressiveFrm= TRUE;
                        prDecPrm->rSEI.u1PicStruct = (UINT8)SEI_FRAME;
                    }
                    else
                    {
                        prFbmPicHdr->fgProgressiveFrm= FALSE;
                        prDecPrm->rSEI.u1PicStruct = (UINT8)SEI_TOP_BOTTOM;
                    }              
                }
            }
            else
            {
                prFbmPicHdr->fgProgressiveFrm= FALSE;
                prDecPrm->rSEI.u1PicStruct = (UINT8)SEI_TOP_BOTTOM;
            }
        }
    }
    prFbmPicHdr->fgRepFirstFld = 
        ((prDecPrm->rSEI.u1PicStruct == (UINT8)SEI_TOP_BOTTOM_TOP) || 
        (prDecPrm->rSEI.u1PicStruct == (UINT8)SEI_BOTTOM_TOP_BOTTOM) ||
        (prDecPrm->rSEI.u1PicStruct == (UINT8)SEI_FRAME_DOUBLING) ||
        (prDecPrm->rSEI.u1PicStruct == (UINT8)SEI_FRAME_TRIPLING))?TRUE:FALSE;
    
    prFbmPicHdr->fgTopFldFirst = 
        ((prDecPrm->rSEI.u1PicStruct == (UINT8)SEI_TOP_BOTTOM_TOP) || 
         (prDecPrm->rSEI.u1PicStruct == (UINT8)SEI_TOP_BOTTOM) ||
         (prDecPrm->rSEI.u1PicStruct == (UINT8)SEI_FRAME_TRIPLING) ||
         ((prDecPrm->rSEI.u1PicStruct == (UINT8)SEI_TOP_FIELD) && (prDecPrm->u1PicStruct & TOP_FIELD)))?TRUE:FALSE;

    prFbmPicHdr->ucPicStruct = prDecPrm->u1PicStruct;

    prFbmPicHdr->u4PTS = prVdecInfo->rPesInfo.u4PTS;
    prDecPrm->prCurrFBufInfo->fgPTSValid = prVdecInfo->rPesInfo.fgDtsValid;
    if(prVdecInfo->rPesInfo.u4PTS == 0)
    {
        LOG(3, "Rcv PTS=0, Valid %d\n", prVdecInfo->rPesInfo.fgDtsValid);
    }
    
    if(prDecPrm->fgIsMMPlay)
    {
        prFbmPicHdr->u8Offset = prDecPrm->u8Offset;
        prFbmPicHdr->u8OffsetDisp = prVdecEsInfo->u8OffsetLast;
        prFbmPicHdr->u8OffsetI = prDecPrm->u8OffsetI;
        prFbmPicHdr->u4TickNum = prDecPrm->u4TickNum;
        prFbmPicHdr->u2DecodingOrder = prDecPrm->u2DecodingOrder;
    }
    #endif
    
    prDecPrm->pu1DecWorkBufY = (UINT8*)(prDecPrm->prCurrFBufInfo->u4YAddr);
    prDecPrm->pu1DecWorkBufC = (UINT8*)(prDecPrm->prCurrFBufInfo->u4CAddr);
    prDecPrm->pu1DecWorkBufMV = (UINT8*)(prDecPrm->prCurrFBufInfo->u4MVBufAddr);
}


// *********************************************************************
// Function    : void   vFillFrameNumGap(TMpvDecPrm *tMpvDecPrm)
// Description : add the frame num for the gap
// Parameter   : None
// Return      : None
// *********************************************************************
/*static void _vFillFrameNumGap(VDEC_AVC_DecPrm *prDecPrm)
{
    UINT32 u4CurrFrameNum;
    UINT32 u4UnusedShortTermFrameNum;
    INT32 tmp1, tmp2;

    ASSERT(prDecPrm!=NULL);

    tmp1 = prDecPrm->prSliceHdr->ai4DeltaPicOrderCnt[0];
    tmp2 = prDecPrm->prSliceHdr->ai4DeltaPicOrderCnt[1];
    
    prDecPrm->prSliceHdr->ai4DeltaPicOrderCnt[0] = 0;
    prDecPrm->prSliceHdr->ai4DeltaPicOrderCnt[1] = 0;

    u4UnusedShortTermFrameNum = (prDecPrm->rLastInfo.u4LastFrameNum + 1) % 
                                prDecPrm->prSPS->u4MaxFrameNum;
    u4CurrFrameNum = prDecPrm->prSliceHdr->u4FrameNum;

    if((prDecPrm->prSliceHdr->u4FrameNum != prDecPrm->rLastInfo.u4LastFrameNum) 
    && (u4CurrFrameNum != u4UnusedShortTermFrameNum))
    {
        LOG(6, "!!!!  Fill frame num gap works  !!!!!\n");
        while (u4CurrFrameNum != u4UnusedShortTermFrameNum)
        {
            // Create a new frame pic
            _vAllocateFBuf(prDecPrm);
            if(prDecPrm->fgAllocFbFail)
            {
                return;
            }
            prDecPrm->prCurrFBufInfo->u1FBufStatus = FRAME;
            prDecPrm->prCurrFBufInfo->i4PicNum = (INT32)u4UnusedShortTermFrameNum;
            prDecPrm->prCurrFBufInfo->u4FrameNum = u4UnusedShortTermFrameNum;
            prDecPrm->prCurrFBufInfo->fgNonExisting = TRUE;

            prDecPrm->prSliceHdr->fgAdaptiveRefPicMarkingModeFlag = 0;

            if(prDecPrm->prSPS->u4PicOrderCntType == 0)
            {
                prDecPrm->prCurrFBufInfo->i4POC = prDecPrm->rLastInfo.i4LastPOC;
                prDecPrm->prCurrFBufInfo->i4TFldPOC = prDecPrm->rLastInfo.i4LastTFldPOC;
                prDecPrm->prCurrFBufInfo->i4BFldPOC = prDecPrm->rLastInfo.i4LastBFldPOC;
            }
            else
            {
                _VDEC_SetCurrPOC(prDecPrm);
            }
            // Check if out of the Ref Frames
            _VDEC_SlidingWindowProce(prDecPrm);
            prDecPrm->prCurrFBufInfo->u1FBufRefType = SREF_PIC;
            prDecPrm->prCurrFBufInfo->u1TFldRefType = SREF_PIC;
            prDecPrm->prCurrFBufInfo->u1BFldRefType = SREF_PIC;
            prDecPrm->rLastInfo.u4LastFrameNum = u4UnusedShortTermFrameNum;
            u4UnusedShortTermFrameNum = (u4UnusedShortTermFrameNum + 1) % prDecPrm->prSPS->u4MaxFrameNum;
        }    
    }

    prDecPrm->prSliceHdr->ai4DeltaPicOrderCnt[0] = tmp1;
    prDecPrm->prSliceHdr->ai4DeltaPicOrderCnt[1] = tmp2;
    prDecPrm->prSliceHdr->u4FrameNum = u4CurrFrameNum;
}*/


// *********************************************************************
// Function    : void   vPrepareRefPiclist(TMpvDecPrm *tMpvDecPrm)
// Description : check pic type to send P_0, B_0, B_1
// Parameter   : None
// Return      : None
// *********************************************************************
static void _vPrepareFBufInfo(VDEC_AVC_DecPrm *prDecPrm)
{
    #ifdef FBM_DPB_CTRL
    UCHAR ucFbgType;
    FBM_SEQ_HDR_T *prFbmSeqHdr;
    UINT32 u4fps = 0, u4FbWidth, u4FbHeight;    
    #endif
    UINT32 u4CurrFrameNum = 0, u4UnusedShortTermFrameNum, u4DAR = 0, u4SAR = 0;
    BOOL fgAfdChg = FALSE;
    BOOL fgRefChg = FALSE;
    VDEC_INFO_T* prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T* prVdecEsInfo = _VDEC_GetEsInfo(ES0);
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ES0);
    
    ASSERT(prDecPrm!=NULL);

    prDecPrm->u4PicW = (prDecPrm->prSPS->u4PicWidthInMbsMinus1 + 1) << 4;
    prDecPrm->u4PicH = ((2 -prDecPrm->prSPS->fgFrameMbsOnlyFlag)*(prDecPrm->prSPS->u4PicHeightInMapUnitsMinus1 + 1)) << 4; //32x
    prDecPrm->u4RealPicH = (prDecPrm->prSPS->u4PicHeightInMapUnitsMinus1 + 1) << 4;  // original real size

    if(((prDecPrm->rLastInfo.u4LastPicW != prDecPrm->u4PicW) || 
        (prDecPrm->rLastInfo.u4LastPicH != prDecPrm->u4PicH)) && 
        prVdecEsInfo->fgMMPlayback && prVdecEsInfo->fgIsTPT &&
        (prDecPrm->ucFbgId != FBM_FBG_ID_UNKNOWN))
    {
        if((prDecPrm->rLastInfo.u4LastPicW > prDecPrm->u4PicW) || 
        (prDecPrm->rLastInfo.u4LastPicH > prDecPrm->u4PicH))
        {
            prDecPrm->fgSeamlessDisp = TRUE;
        }
        else
        {
            prDecPrm->fgSeamlessDisp = FALSE;
        }
    }
    else if((prDecPrm->rLastInfo.u4LastPicW != prDecPrm->u4PicW) || 
        (prDecPrm->rLastInfo.u4LastPicH != prDecPrm->u4PicH) ||
        (prDecPrm->ucFbgId == FBM_FBG_ID_UNKNOWN))
    {
        fgAfdChg = TRUE;
        prDecPrm->fgIsResChg = TRUE;
        #ifdef FBM_DPB_CTRL
        _VDEC_FlushDPB(prDecPrm);
        FBM_ReleaseGroup(prDecPrm->ucFbgId);
        
        prDecPrm->rLastInfo.u4LastSPos = 0;
        prDecPrm->i4LastPOC = -99999;        
        
        ucFbgType = FBM_SelectGroupType(prDecPrm->u4PicW, prDecPrm->u4PicH);
        prDecPrm->ucFbgId = FBM_CreateGroup(ucFbgType, FBM_VDEC_H264, prDecPrm->u4PicW, prDecPrm->u4PicH);
        prVdecEsInfo->ucFbgId = prDecPrm->ucFbgId;
        if(prDecPrm->ucFbgId == FBM_FBG_ID_UNKNOWN)
        {
            prDecPrm->fgAllocFbFail = TRUE;
            LOG(2, "CreateFbg Fail!!\n");
            //ASSERT(0);
            return;
        }
        if(prVdecEsInfo->fgMMPlayback)
        {
            FBM_SetPlayMode(prDecPrm->ucFbgId, FBM_FBG_MM_MODE);
        }        
        FBM_FbgChgNotify(prDecPrm->ucFbgId, 0);
        //FBM_SetSyncStc(prDecPrm->ucFbgId, prDecPrm->ucSyncMode, prDecPrm->ucStcId);
        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prDecPrm->ucFbgId);
        if(prFbmSeqHdr == NULL)
        {
            prDecPrm->fgAllocFbFail = TRUE;
            LOG(2, "GetFbgSeqHdr Fail!!\n");
            //ASSERT(0);
            return;
        }
        if (FBM_ChkFrameBufferFlag(prDecPrm->ucFbgId, FBM_FLAG_RESET))
        {
            FBM_ResetGroup(prDecPrm->ucFbgId);
            FBM_ClrFrameBufferFlag(prDecPrm->ucFbgId, FBM_FLAG_RESET);
            FBM_ClrFrameBufferFlag(prDecPrm->ucFbgId, FBM_FLAG_NOT_READY);
            //FBM_SetFrameBufferFlag(prDecPrm->ucFbgId, FBM_FLAG_CLEAR_FRC_PRM);
        }        
        //set afd default value
        prFbmSeqHdr->ucActFmt = ACT_FMT_DEFAULT;
        
        //RR mode setting
        if(prDecPrm->fgRRModeYEn || prDecPrm->fgRRModeCEn)
        {
            FBM_SetFrameBufferFlag(prDecPrm->ucFbgId, FBM_FLAG_REDUCE_REF);
        }
        else if(FBM_ChkFrameBufferFlag(prDecPrm->ucFbgId, FBM_FLAG_REDUCE_REF))
        {
            prDecPrm->fgRRModeYEn = TRUE;
            prDecPrm->fgRRModeCEn = TRUE;
            prDecPrm->u1RRModeY = 0;
            prDecPrm->u1RRModeC = 0;
        }
        else
        {
            FBM_ClrFrameBufferFlag(prDecPrm->ucFbgId, FBM_FLAG_REDUCE_REF);
        }
        
        if(prDecPrm->fgRRModeYEn)
        {
            prFbmSeqHdr->fgRRYEn = TRUE;            
            if(prDecPrm->u1RRModeY > 0)
            {
                prFbmSeqHdr->fgYRatio34 = TRUE;
            }
            else
            {
                prFbmSeqHdr->fgYRatio34 = FALSE;
            }
        }
        else
        {
            prFbmSeqHdr->fgRRYEn = FALSE;
        }
        if(prDecPrm->fgRRModeCEn)
        {
            prFbmSeqHdr->fgRRCEn = TRUE;
            if(prDecPrm->u1RRModeC > 0)
            {
                prFbmSeqHdr->fgCRatio34 = TRUE;
            }
            else
            {
                prFbmSeqHdr->fgCRatio34 = FALSE;
            }
        }
        else
        {
            prFbmSeqHdr->fgRRCEn = FALSE;
        }
        FBM_GetFrameBufferSize(prDecPrm->ucFbgId, &u4FbWidth, &u4FbHeight);
        
        //Display parameter setting
        prFbmSeqHdr->u2LineSize = (UINT16)u4FbWidth;//prDecPrm->u4PicW;
        UNUSED(u4FbHeight);
        
        if((prVdecEsInfo->fgMMSeqInfoValid && prVdecEsInfo->fgMMPlayback) &&
            ((prVdecEsInfo->rMMSeqInfo.u2_width > 0) &&
            (prVdecEsInfo->rMMSeqInfo.u2_height > 0)))
        {
            prFbmSeqHdr->u2HSize = prVdecEsInfo->rMMSeqInfo.u2_width;
            prFbmSeqHdr->u2VSize = prVdecEsInfo->rMMSeqInfo.u2_height;
        }
        else
        {
            prFbmSeqHdr->u2HSize = 
                (UINT16)(prDecPrm->u4PicW - prDecPrm->prSPS->u4FrameCropRightOffset);
            prFbmSeqHdr->u2VSize = 
                (UINT16)(prDecPrm->u4PicH - 
                ((prDecPrm->prSPS->fgFrameMbsOnlyFlag) ? 
                prDecPrm->prSPS->u4FrameCropBottomOffset :
                (prDecPrm->prSPS->u4FrameCropBottomOffset << 1)));
        }
        prFbmSeqHdr->u2OrgHSize = prFbmSeqHdr->u2HSize;
        prFbmSeqHdr->u2OrgVSize = prFbmSeqHdr->u2VSize;
        prVdecEsInfo->u2LineSize = prFbmSeqHdr->u2LineSize;
#ifdef CC_RESIZE_SMALL_IMAGE
        prVdecEsInfo->u2OrgHSize = prFbmSeqHdr->u2OrgHSize;        
        prVdecEsInfo->u2OrgVSize = prFbmSeqHdr->u2OrgVSize;

        if((prFbmSeqHdr->u2HSize < FBM_IMG_RESIZE_LIMITATION) ||
           (prFbmSeqHdr->u2VSize < FBM_IMG_RESIZE_LIMITATION))
        {
            prFbmSeqHdr->fgResizeSmallPic = TRUE;
            if(prFbmSeqHdr->u2HSize < FBM_IMG_RESIZE_LIMITATION)
            {
                prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
            }
            else if(prFbmSeqHdr->u2HSize > FBM_IMG_RESIZE_LIMITATION_MAX)
            {
                prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
            }
            else
            {
                prFbmSeqHdr->u2LineSize = AVC_ALIGN_MASK(prFbmSeqHdr->u2HSize, 
                                                            AVC_BLK_ALIGMENT);
                prFbmSeqHdr->u2HSize = prFbmSeqHdr->u2LineSize;
                prFbmSeqHdr->u2OrgHSize = prFbmSeqHdr->u2LineSize;
            }
            if(prFbmSeqHdr->u2VSize < FBM_IMG_RESIZE_LIMITATION)
            {
                prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
            }
            else if(prFbmSeqHdr->u2VSize > FBM_IMG_RESIZE_LIMITATION_MAX)
            {
                prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
            }
            else
            {
                prFbmSeqHdr->u2VSize = AVC_ALIGN_MASK(prFbmSeqHdr->u2VSize, 
                                                        AVC_BLK_ALIGMENT);
                prFbmSeqHdr->u2OrgVSize = prFbmSeqHdr->u2VSize;
            }

            LOG(6, "-> 0 OrgHSize %d, OrgVSize %d, HSize %d, VSize %d, LineSize %d\n",
                       prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize, 
                       prFbmSeqHdr->u2HSize, prFbmSeqHdr->u2VSize, prFbmSeqHdr->u2LineSize);
        }
#endif

        prDecPrm->rLastInfo.u4SARWidth = 0;
        prDecPrm->rLastInfo.u4SARHeight = 0;        
        if(prDecPrm->prSPS->rVUI.u4AspectRatioIdc != 0xFF)   //extended_SAR
        {
            prFbmSeqHdr->ucAspRatInf = (UCHAR)prDecPrm->prSPS->rVUI.u4AspectRatioIdc;    // 16:9, 4:3
        }

        if(prDecPrm->prSPS->fgVuiParametersPresentFlag)
        {
            if(prDecPrm->prSPS->rVUI.u4NumUnitsInTick > 0)
            {
                u4fps = (prDecPrm->prSPS->rVUI.u4TimeScale/prDecPrm->prSPS->rVUI.u4NumUnitsInTick) >> 1;
            }

            switch(u4fps)
            {
                case 24:
                    prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_24;
                    break;
                case 23:
                    prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_24_;
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
                    if((prVdecEsInfo->fgMMSeqInfoValid && prVdecEsInfo->fgMMPlayback) &&
                        (prVdecEsInfo->rMMSeqInfo.e_frame_rate != VDEC_SRC_FRAME_RATE_UNKNOWN))
                    {
                        switch(prVdecEsInfo->rMMSeqInfo.e_frame_rate)
                        {
                            case VDEC_SRC_FRAME_RATE_23_976: /* 24000/1001 (23.976...) */
                                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_24_;
                                break;
                            case VDEC_SRC_FRAME_RATE_24:
                                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_24;
                                break;
                            case VDEC_SRC_FRAME_RATE_25:
                                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_25;
                                break;
                            case VDEC_SRC_FRAME_RATE_29_97: /* 30000/1001 (29.97...) */
                                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_30_;
                                break;
                            case VDEC_SRC_FRAME_RATE_30:
                                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_30;
                                break;
                            case VDEC_SRC_FRAME_RATE_50:
                                prFbmSeqHdr->ucFrmRatCod = 
                                    (!prDecPrm->prSliceHdr->fgFieldPicFlag) ? MPEG_FRAME_RATE_50 : MPEG_FRAME_RATE_25;
                                break;
                            case VDEC_SRC_FRAME_RATE_59_94: /* 60000/1001 (59.94...) */
                                prFbmSeqHdr->ucFrmRatCod = 
                                    (!prDecPrm->prSliceHdr->fgFieldPicFlag) ? MPEG_FRAME_RATE_60_ : MPEG_FRAME_RATE_30_;
                                break;
                            case VDEC_SRC_FRAME_RATE_60:
                                prFbmSeqHdr->ucFrmRatCod = 
                                    (!prDecPrm->prSliceHdr->fgFieldPicFlag) ? MPEG_FRAME_RATE_60 : MPEG_FRAME_RATE_30;
                                break;
                            default:
                                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_UNKNOWN;
                                break;
                        }
                    }
                    else
                    {
                        prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_UNKNOWN;
                    }
                    break;
            }
            //OverScan
            if(prDecPrm->prSPS->rVUI.fgOverscanInfoPresentFlag &&
                (!prDecPrm->prSPS->rVUI.fgOverscanAppropriateFlag))
            {
                prFbmSeqHdr->fgNoOverScan = TRUE;
            }
            else
            {
                prFbmSeqHdr->fgNoOverScan = FALSE;
            }
            //Color Description              
            if(prDecPrm->prSPS->rVUI.fgVideoSignalTypePresentFlag &&
                prDecPrm->prSPS->rVUI.fgColourDescriptionPresentFlag)
            {
                prFbmSeqHdr->fgColorDescrip = TRUE;
                prFbmSeqHdr->ucColourPrimaries = (UCHAR)prDecPrm->prSPS->rVUI.u4ColourPrimaries;
                prFbmSeqHdr->ucTransCharacter = (UCHAR)prDecPrm->prSPS->rVUI.u4TransferCharacteristics;
                prFbmSeqHdr->ucMatrixCoeff = (UCHAR)prDecPrm->prSPS->rVUI.u4MatrixCoefficients;
            }
            else
            {
                prFbmSeqHdr->fgColorDescrip = FALSE;
            }
            //Frame Crop
            if(prDecPrm->prSPS->fgFrameCroppingFlag)
            {
                if(prDecPrm->prSPS->fgFrameMbsOnlyFlag)
                {
                    prFbmSeqHdr->u4FrmCropTop = prDecPrm->prSPS->u4FrameCropTopOffset;
                    prFbmSeqHdr->u4FrmCropBtm = prDecPrm->prSPS->u4FrameCropBottomOffset;
                }
                else
                {
                    prFbmSeqHdr->u4FrmCropTop = prDecPrm->prSPS->u4FrameCropTopOffset << 1;
                    prFbmSeqHdr->u4FrmCropBtm = prDecPrm->prSPS->u4FrameCropBottomOffset << 1;
                }
            }
            else
            {
                prFbmSeqHdr->u4FrmCropTop = 0;
                prFbmSeqHdr->u4FrmCropBtm = 0;
            }
        }
        else
        {
            prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_UNKNOWN;
            prFbmSeqHdr->fgNoOverScan = FALSE;
            prFbmSeqHdr->fgColorDescrip = FALSE;
            prFbmSeqHdr->u4FrmCropTop = 0;
            prFbmSeqHdr->u4FrmCropBtm = 0;            
        }

        //progressive/interlace display
        if(prDecPrm->prSPS->rVUI.fgPicStructPresentFlag)
        {
            prFbmSeqHdr->fgProgressiveSeq = 
                ((prDecPrm->rSEI.u1PicStruct == (UINT8)SEI_FRAME) || 
                (prDecPrm->rSEI.u1PicStruct == (UINT8)SEI_FRAME_DOUBLING) ||
                (prDecPrm->rSEI.u1PicStruct == (UINT8)SEI_FRAME_TRIPLING))?TRUE:FALSE;
        }
        else  // progressive/interlace display detect
        {
            if(prDecPrm->prSliceHdr->fgFieldPicFlag)
            {
                prFbmSeqHdr->fgProgressiveSeq = FALSE;
            }
            else
            {
                if(prVdecEsInfo->fgMMPlayback)
                {
                    prFbmSeqHdr->fgProgressiveSeq = TRUE;
                }
                else
                {
                    if((prFbmSeqHdr->ucFrmRatCod == (UCHAR)MPEG_FRAME_RATE_24) ||
                        (prFbmSeqHdr->ucFrmRatCod == (UCHAR)MPEG_FRAME_RATE_24_) ||
                        ((prDecPrm->u4PicW == 1280) && (prDecPrm->u4PicH == 720)) ||
                        ((prDecPrm->u4PicW == 1280) && (prDecPrm->u4PicH == 736)))
                    {
                        prFbmSeqHdr->fgProgressiveSeq = TRUE;
                    }
                    else
                    {
                        prFbmSeqHdr->fgProgressiveSeq = FALSE;
                    }
                }
            }
        }
        
        #endif
        _vPartitionDPB(prDecPrm);
        if(prDecPrm->fgNotSupport)  // memory not enough
        {
            return;
        }
    }
    else
    {
        prDecPrm->fgIsResChg = FALSE;
    }

    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prDecPrm->ucFbgId);
    // I frame case
    if (prVdecInfo && (prVdecInfo->eCurState == VDEC_ST_PLAY_I_FRAME))
    {
        if (prVdecEsInfo && (!prVdecEsInfo->fgIFrameDec))   // header parsing
        {
            if (prVdecEsInfoKeep && (prVdecEsInfoKeep->pfDecNfyFct))
            {
                prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag, VDEC_COND_I_FRAME_SET_BUFF_DONE, 0, 0);                
            }
            prDecPrm->fgAllocFbFail = TRUE; //ignore decoding stage
            return;
        }
        else    //decoding
        {
            prDecPrm->prSPS->rVUI.fgPicStructPresentFlag = TRUE;
            prDecPrm->rSEI.u1PicStruct = (UINT8)SEI_FRAME;
            if(prFbmSeqHdr)
            {
                prFbmSeqHdr->fgProgressiveSeq = TRUE;
            }
        }
    }

    // set default pts scale/rate
    if(prDecPrm->u4Rate == 0)
    {
        if(prFbmSeqHdr == NULL)
        {
            prDecPrm->fgAllocFbFail = TRUE;
            LOG(2, "GetFbgSeqHdr Fail!!\n");
            //ASSERT(0);
            return;
        }    
        if((prFbmSeqHdr->ucFrmRatCod != MPEG_FRAME_RATE_UNKNOWN) &&
            ((UINT32)prFbmSeqHdr->ucFrmRatCod < FRAME_RATE_MAX))
        {
            prDecPrm->u8PtsScale = au8Scale[prFbmSeqHdr->ucFrmRatCod];
            prDecPrm->u4Rate = au4Rate[prFbmSeqHdr->ucFrmRatCod];
        }
        else
        {   
            prDecPrm->u8PtsScale = au8Scale[MPEG_FRAME_RATE_30];
            prDecPrm->u4Rate = au4Rate[MPEG_FRAME_RATE_30];
        }
    }    
    //aspect ratio change notify
    #ifdef FBM_DPB_CTRL
    if(prFbmSeqHdr != NULL)
    {
        if(prDecPrm->prSPS->rVUI.u4AspectRatioIdc == 0xFF)   //extended_SAR
        {
            if((prDecPrm->rLastInfo.u4SARWidth != prDecPrm->prSPS->rVUI.u4SarWidth)||
            (prDecPrm->rLastInfo.u4SARHeight != prDecPrm->prSPS->rVUI.u4SarHeight)||
            fgAfdChg)   //resolution change
            {
                u4DAR = ((prDecPrm->u4PicW * prDecPrm->prSPS->rVUI.u4SarWidth)*100) /
                            (prDecPrm->u4PicH * prDecPrm->prSPS->rVUI.u4SarHeight);
                prDecPrm->rLastInfo.u4SARWidth = prDecPrm->prSPS->rVUI.u4SarWidth;
                prDecPrm->rLastInfo.u4SARHeight = prDecPrm->prSPS->rVUI.u4SarHeight;
                prDecPrm->rLastInfo.u4AspRatInf = 0xFF;
                fgAfdChg = TRUE;
            }
        }
        else if((prDecPrm->rLastInfo.u4AspRatInf != 
        prDecPrm->prSPS->rVUI.u4AspectRatioIdc) || fgAfdChg)    
        //aspRatio change or resolution chage
        {
            switch (prDecPrm->prSPS->rVUI.u4AspectRatioIdc)
            {
                case 1:
                    u4SAR = 100;
                    break;
                case 2:
                    u4SAR = 1200/11;
                    break;
                case 3:
                    u4SAR = 1000/11;
                    break;
                case 4:
                    u4SAR = 1600/11;
                    break;
                case 5:
                    u4SAR = 4000/33;
                    break;
                case 6:
                    u4SAR = 2400/11;
                    break;
                case 7:
                    u4SAR = 2000/11;
                    break;
                case 8:
                    u4SAR = 3200/11;
                    break;
                case 9:
                    u4SAR = 8000/33;
                    break;
                case 10:
                    u4SAR = 1800/11;
                    break;
                case 11:
                    u4SAR = 1500/11;
                    break;
                case 12:
                    u4SAR = 6400/33;
                    break;
                case 13:
                    u4SAR = 16000/99;
                    break;
                case 14:
                    u4SAR = 400/3;
                    break;
                case 15:
                    u4SAR = 300/2;
                    break;
                case 16:
                    u4SAR = 200;
                    break;                    
                default:
                    u4SAR = 1200/11;
            }
            u4DAR = (prDecPrm->u4PicW * u4SAR) / prDecPrm->u4PicH;
            fgAfdChg = TRUE;        
            prDecPrm->rLastInfo.u4SARWidth = 0;
            prDecPrm->rLastInfo.u4SARHeight = 0;
            prDecPrm->rLastInfo.u4AspRatInf = prDecPrm->prSPS->rVUI.u4AspectRatioIdc;
        }
        if(fgAfdChg)
        {
            if(((u4DAR *3) >>2) < 110)
            {
                prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_4_3;
            }
            else if(((u4DAR *9) >>4) < 110)
            {
                prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_16_9;
            }
            else if(((u4DAR *100) /221) < 110)
            {
                prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_221_1;
            }
            else
            {
                prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_4_3;
            }
            LOG(2, "AFD %d, AspRatio Chg %d\n", prFbmSeqHdr->ucActFmt, prFbmSeqHdr->ucAspRatInf);
            FBM_SetFrameBufferFlag(prDecPrm->ucFbgId, FBM_FLAG_SEQ_CHG);
        }
    }
    
    //set AV sync mode
    FBM_SetSyncStc(prDecPrm->ucFbgId, prDecPrm->ucSyncMode, prDecPrm->ucStcId);
    #endif
    
    //trick mode with only 1 field sending from swdmx
    if(prDecPrm->fgIsMMPlay && (prDecPrm->ucSkipMode == IMode) && 
        (prDecPrm->prCurrFBufInfo->u1FBufStatus != FRAME) && 
        (prDecPrm->u1PicStruct == prDecPrm->prCurrFBufInfo->u1FBufStatus))
    {
        if(prDecPrm->u1PicStruct == BOTTOM_FIELD)
        {
            LOG(3, "Need to rpt bottom fld!\n");
        }
        if(FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId) ==
            FBM_FB_STATUS_DECODE)
        {
            FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId, 
            FBM_FB_STATUS_DISPLAYQ);
        }
    }
    
    //if(tMpvDecPrm->tSliceHdr->fgNoOutputOfPriorPicsFlag) // clear all prior pic in DPB
    if(fgIsIDRPic(prDecPrm->u1NalUnitType))
    {
        prDecPrm->rLastInfo.u4LastFrameNum = 0xffffffff;
        //vFlushBufRefInfo();
        _VDEC_FlushDPB(prDecPrm);
    }

    u4UnusedShortTermFrameNum = (prDecPrm->rLastInfo.u4LastFrameNum + 1) % 
        prDecPrm->prSPS->u4MaxFrameNum;
    u4CurrFrameNum = prDecPrm->prSliceHdr->u4FrameNum;

    if(((u4CurrFrameNum != prDecPrm->rLastInfo.u4LastFrameNum) 
    && (u4CurrFrameNum != u4UnusedShortTermFrameNum))
    && (!prDecPrm->fgIsMMPlay))
    {
        if(prDecPrm->rLastInfo.u4LastFrameNum < (UINT32)0xffffffff)
        {
            LOG(2, "FrmGap!%d\n", 
                (u4CurrFrameNum > u4UnusedShortTermFrameNum)?
                (u4CurrFrameNum - u4UnusedShortTermFrameNum):
                ((prDecPrm->prSPS->u4MaxFrameNum - u4UnusedShortTermFrameNum) + u4CurrFrameNum));
                if(prDecPrm->prSPS->fgGapsInFrameNumValueAllowedFlag)
                {
                    LOG(2, "FillFrmNumGap?\n");
                    /*_vFillFrameNumGap(prDecPrm);
                    if(prDecPrm->fgAllocFbFail)
                    {
                        return;
                    }*/
                }
                //else
                {
                    #ifndef __MODEL_slt__
                    _VDEC_FlushDPB(prDecPrm);
                    _vPartitionDPB(prDecPrm);   //reset DPB and MV buffer
                    prDecPrm->prSPS->fgSPSValid = FALSE;
                    prDecPrm->fgAllocFbFail = TRUE;
                    prDecPrm->rLastInfo.u4LastFrameNum = 0xffffffff;

                    prDecPrm->i4LastPOC = -99999;
                    prDecPrm->fgPOCWrap = FALSE;
                    prDecPrm->rSEI.u1ExtMatchFlg = FALSE;
                    prDecPrm->rSEI.u1WaitRAPCnt = 0;
                    prDecPrm->rSEI.u1WaitRAPRefCnt = 0;
                    prDecPrm->rSEI.u1WaitRAPICnt = 0;            
                    prDecPrm->fgIsFirstFrm = FALSE;                    
                    return;
                    #endif
                }
        }
    }
    
    // Find an empty fbuf
    if((prDecPrm->rLastInfo.fgLastIsRef && (!fgIsRefPic(prDecPrm->u1NalRefIdc))) ||
        (!prDecPrm->rLastInfo.fgLastIsRef && fgIsRefPic(prDecPrm->u1NalRefIdc)))
    {
        fgRefChg = TRUE;
    }
    if((prDecPrm->prCurrFBufInfo->u1FBufStatus == NO_PIC)
        || (prDecPrm->prCurrFBufInfo->u1FBufStatus & prDecPrm->u1PicStruct)
        || (u4CurrFrameNum != prDecPrm->rLastInfo.u4LastFrameNum)
        || fgRefChg)
    {
        if((prDecPrm->prCurrFBufInfo->u1FBufStatus == TOP_FIELD) ||
            (prDecPrm->prCurrFBufInfo->u1FBufStatus == BOTTOM_FIELD))
        {
            LOG(2, "NPF!Prev field IsRef %d\n", prDecPrm->prCurrFBufInfo->fgIsBufRef);
            /*if(prDecPrm->prCurrFBufInfo->fgIsBufRef)    //wait util next SPS
            {
                _VDEC_FlushDPB(prDecPrm);
                _vPartitionDPB(prDecPrm);   //reset DPB and MV buffer
                prDecPrm->prSPS->fgSPSValid = FALSE;
                prDecPrm->fgAllocFbFail = TRUE;
                prDecPrm->rLastInfo.u4LastFrameNum = 0xffffffff;
                return;
            }
            prDecPrm->prCurrFBufInfo->u1FBufStatus = NO_PIC;*/
        }
        //else
        {
            _vAllocateFBuf(prDecPrm);    //find new prDecPrm->prCurrFBufInfo
            if(prDecPrm->fgAllocFbFail)
            {
                return;
            }
            //_u4MaxDecCycle = 0; //Init the count for decoding cycle
        }
    }
    if(fgIsRefPic(prDecPrm->u1NalRefIdc))
    {
        prDecPrm->prCurrFBufInfo->fgIsBufRef = TRUE;
    }
    prDecPrm->prCurrFBufInfo->u1FBufStatus |= prDecPrm->u1PicStruct;

    if(prDecPrm->u1PicStruct & TOP_FIELD)
    {
        prDecPrm->prCurrFBufInfo->u4TFldPara = (((UINT32)fgIsFrmPic(prDecPrm->u1PicStruct)? 0 : 1) << 19) + 
        ((UINT32)(fgIsFrmPic(prDecPrm->u1PicStruct) && prDecPrm->prSPS->fgMbAdaptiveFrameFieldFlag) << 18);
    }
    if(prDecPrm->u1PicStruct & BOTTOM_FIELD)
    {
        prDecPrm->prCurrFBufInfo->u4BFldPara = (((UINT32)fgIsFrmPic(prDecPrm->u1PicStruct)? 0 : 1) << 19) + 
        ((UINT32)(fgIsFrmPic(prDecPrm->u1PicStruct) && prDecPrm->prSPS->fgMbAdaptiveFrameFieldFlag) << 18);
    }
  
}


// *********************************************************************
// Function    : BOOL fgChkRefInfo(DWRD dwFBufIdx, DWRD dwRefType)
// Description : Check if reference picture should be insered to ref pic list
// Parameter   : None
// Return      : None
// *********************************************************************
static BOOL _fgChkRefInfo(const VDEC_AVC_DecPrm* prDecPrm, UINT32 u4FBufIdx, UINT32 u4RefType)
{
    ASSERT(prDecPrm!=NULL);
    
    if(fgIsFrmPic(prDecPrm->u1PicStruct))
    {
         // According to spec 8.2.4.2.1 
        // NOTE: A non-pared reference fiedl is not used for inter prediction for decoding a frame.
        if((prDecPrm->arFBufInfo[u4FBufIdx].u1TFldRefType == (UINT8)u4RefType) && 
            (prDecPrm->arFBufInfo[u4FBufIdx].u1BFldRefType == (UINT8)u4RefType))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        if((prDecPrm->arFBufInfo[u4FBufIdx].u1TFldRefType == (UINT8)u4RefType) || 
            (prDecPrm->arFBufInfo[u4FBufIdx].u1BFldRefType == (UINT8)u4RefType))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }    
    }
}


// *********************************************************************
// Function    : void vInsertRefPicList(TRefPicList *ptRefPicList, INT iCurrPOC, DWRD dwRefPicListInfo)
// Description : Instert Short ref pic list item
// Parameter   : None
// Return      : None
// *********************************************************************
static void _vInsertRefPicList(const VDEC_AVC_DecPrm* prDecPrm, VDEC_REF_PIC_LIST_T* prRefPicList, INT32 i4CurrPOC, UINT32 u4RefPicListInfo)
{
    INT32 j;
    UINT8 u1RefType; // 1-> Short 2-> Long
    UINT8 u1ListType; // 0-> P, 1-> B_0, 2->B_1
    UINT8 u1FBufIdx;
    INT32 i4Comp0 = 0, i4Comp1 = 0;
    UINT32 u4Temp;
    BOOL fgSwitch;

    ASSERT(prRefPicList!=NULL);
    ASSERT(prDecPrm!=NULL);
    
    u1RefType = (UINT8)(u4RefPicListInfo & 0xf);   
    // 0:P_T, 1:P_B, 2:B0_T, 3:B0_B, 4:B1_T, 5:B1_B, 6:P_T_L, 7:P_B_L, 8:B_T_L, 9:B_B_L,  
    u1ListType = (UINT8)((u4RefPicListInfo >> 8) & 0xf);  
    u1FBufIdx = (UINT8)((u4RefPicListInfo >> 16) & 0xff);  

    if(u1RefType == SREF_PIC)
    {
        // 1st: Insert the current to the last idx
        prRefPicList->au4FBufIdx[prRefPicList->u4RefPicCnt] = u1FBufIdx;
        // 2nd: shift Shortterm ref pic        
        for(j=(INT32)prRefPicList->u4RefPicCnt - 1; j>=0; j--)
        {
            fgSwitch = FALSE;
            if(u1ListType == 0)
            {
                if(prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j+1]].i4TFldPicNum > 
                    prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j]].i4TFldPicNum)
                {
                    fgSwitch = TRUE;
                }
            }
            else if(u1ListType == 1)
            {
                if(prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j+1]].i4BFldPicNum > 
                    prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j]].i4BFldPicNum)
                {
                    fgSwitch = TRUE;
                }
            }
            else if((u1ListType == 2) || (u1ListType == 3))
            {
                i4Comp0 = (u1ListType == 2)? prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j]].i4TFldPOC : 
                        prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j]].i4BFldPOC;
                i4Comp1 = (u1ListType == 2)? prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j+1]].i4TFldPOC : 
                        prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j+1]].i4BFldPOC;
                if((fgIsFrmPic(prDecPrm->u1PicStruct)? 
                        (((i4Comp0 < i4CurrPOC) &&
                        (i4Comp1 < i4CurrPOC) && (i4Comp1 > i4Comp0)) ||
                        ((i4Comp0 >= i4CurrPOC) && (i4Comp1 < i4Comp0)) ||
                        ((i4Comp0 >= i4CurrPOC) && (i4Comp1 < i4CurrPOC))):
                        (((i4Comp0 <= i4CurrPOC) && (i4Comp1 <= i4CurrPOC) && (i4Comp1 > i4Comp0)) ||
                          ((i4Comp0 > i4CurrPOC) && (i4Comp1 < i4Comp0)) ||
                          ((i4Comp0 > i4CurrPOC) && (i4Comp1 <= i4CurrPOC))
                        )) )
                {
                    fgSwitch = TRUE;
                }
            }
            else if((u1ListType == 4) || (u1ListType == 5))
            {
                i4Comp0 = (u1ListType == 4)? prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j]].i4TFldPOC : 
                        prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j]].i4BFldPOC;
                i4Comp1 = (u1ListType == 4)? prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j+1]].i4TFldPOC : 
                        prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j+1]].i4BFldPOC;
                if(((i4Comp0 <= i4CurrPOC) && (i4Comp1 > i4Comp0)) ||
                    ((i4Comp0 > i4CurrPOC) && (i4Comp1 > i4CurrPOC) && (i4Comp1 < i4Comp0)) ||
                    ((i4Comp0 <= i4CurrPOC) && (i4Comp1 > i4CurrPOC)))
                {
                    fgSwitch = TRUE;
                }
            }
            if(fgSwitch)
            {
                u4Temp = prRefPicList->au4FBufIdx[j+1];
                prRefPicList->au4FBufIdx[j+1] = prRefPicList->au4FBufIdx[j];
                prRefPicList->au4FBufIdx[j] = u4Temp;
            }
        }
        prRefPicList->u4RefPicCnt ++;  
    }
    else if(u1RefType == LREF_PIC)
    {
        prRefPicList->au4FBufIdx[prRefPicList->u4RefPicCnt] = u1FBufIdx;        
        for(j=(INT32)(prRefPicList->u4RefPicCnt - 1); j>=0; j--)
        {
            if(u1ListType == 6)
            {
                i4Comp0 = prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j]].i4TFldLongTermPicNum;
                i4Comp1 = prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j+1]].i4TFldLongTermPicNum;
            }
            else if(u1ListType == 7)
            {
                i4Comp0 = prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j]].i4BFldLongTermPicNum;
                i4Comp1 = prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j+1]].i4BFldLongTermPicNum;
            }
            else if(u1ListType == 8)
            {
                i4Comp0 = (INT32)prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j]].u4TFldLongTermFrameIdx;
                i4Comp1 = (INT32)prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j+1]].u4TFldLongTermFrameIdx;
            }
            else if(u1ListType == 9)
            {
                i4Comp0 = (INT32)prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j]].u4BFldLongTermFrameIdx;
                i4Comp1 = (INT32)prDecPrm->arFBufInfo[prRefPicList->au4FBufIdx[j+1]].u4BFldLongTermFrameIdx;
            }            
            if(i4Comp1 < i4Comp0)
            {
                u4Temp = prRefPicList->au4FBufIdx[j+1];
                prRefPicList->au4FBufIdx[j+1] = prRefPicList->au4FBufIdx[j];
                prRefPicList->au4FBufIdx[j] = u4Temp;
            }
        }
        prRefPicList->u4RefPicCnt ++;
    }
}


// *********************************************************************
// Function    : void vSetupPRefPicList(DWRD *dwRefIdx, TRefPicList *ptTFldRefPicList, TRefPicList *ptBFldRefPicList)
// Description : Setup Ref Pic List
// Parameter   : None
// Return      : None
// *********************************************************************
static void _vSetupPRefPicList(const VDEC_REF_PIC_LIST_T* prRefPicList, 
    const VDEC_FBUF_INFO_T* prFBufInfo, UINT8 u1PicStruct, UINT32 *pu4RefIdx, 
    UINT32 u4TFldListIdx, UINT32 u4BFldListIdx)
{
    UINT32 i ;
    UINT32 u4TotalFBuf;  

    ASSERT(pu4RefIdx!=NULL);
    ASSERT(prRefPicList!=NULL);
    ASSERT(prFBufInfo!=NULL);
    
    u4TotalFBuf = (prRefPicList[u4TFldListIdx].u4RefPicCnt >= prRefPicList[u4BFldListIdx].u4RefPicCnt)?
                         prRefPicList[u4TFldListIdx].u4RefPicCnt : prRefPicList[u4BFldListIdx].u4RefPicCnt;
    if(fgIsFrmPic(u1PicStruct))
    {
        u4TFldListIdx = (prRefPicList[u4TFldListIdx].u4RefPicCnt >= prRefPicList[u4BFldListIdx].u4RefPicCnt)?
                               u4TFldListIdx : u4BFldListIdx;
    }
    for(i=0; i<u4TotalFBuf; i++)
    {
        if(fgIsFrmPic(u1PicStruct))
        {
            _VDEC_SetPRefPicListReg(prRefPicList[u4TFldListIdx], prFBufInfo, 
                FRAME + (i << 8) + (pu4RefIdx[0] << 16), u4TFldListIdx);
            pu4RefIdx[0] ++;
        }
        else if(u1PicStruct == TOP_FIELD)
        {
            if(i < prRefPicList[u4TFldListIdx].u4RefPicCnt)
            {
                _VDEC_SetPRefPicListReg(prRefPicList[u4TFldListIdx], prFBufInfo, 
                    TOP_FIELD + (i << 8) + (pu4RefIdx[0] << 16), u4TFldListIdx);
                pu4RefIdx[0] ++;
            }
            if(i < prRefPicList[u4BFldListIdx].u4RefPicCnt)
            {
                _VDEC_SetPRefPicListReg(prRefPicList[u4BFldListIdx], prFBufInfo,
                    BOTTOM_FIELD + (i << 8)  + (pu4RefIdx[0] << 16), u4BFldListIdx);
                pu4RefIdx[0] ++;
            } 
        }
        else if(u1PicStruct == BOTTOM_FIELD)
        {
            if(i < prRefPicList[u4BFldListIdx].u4RefPicCnt)
            {
                _VDEC_SetPRefPicListReg(prRefPicList[u4BFldListIdx], prFBufInfo, 
                    BOTTOM_FIELD + (i << 8)  + (pu4RefIdx[0] << 16), u4BFldListIdx);
                pu4RefIdx[0] ++;
            }
            if(i < prRefPicList[u4TFldListIdx].u4RefPicCnt)
            {
                _VDEC_SetPRefPicListReg(prRefPicList[u4TFldListIdx], prFBufInfo,
                    TOP_FIELD + (i << 8) + (pu4RefIdx[0] << 16), u4TFldListIdx);
                pu4RefIdx[0] ++;
            } 
        }
    }  
}

// *********************************************************************
// Function    : void vVDecSetPRefPicList(void)
// Description : Set P ref pic list by Pic Num
// Parameter   : None
// Return      : None
// *********************************************************************
static void _vSetPRefPicList(VDEC_AVC_DecPrm* prDecPrm)
{
    UINT32 i;
    UINT32 u4AddTop;
    UINT32 u4AddBot;
    UINT32 u4Temp;
    UINT32 u4CurrPicNum;

       ASSERT(prDecPrm!=NULL);

    prDecPrm->prCurrFBufInfo->u4FrameNum = prDecPrm->prSliceHdr->u4FrameNum;
    u4CurrPicNum = (fgIsFrmPic(prDecPrm->u1PicStruct)) ? 
                prDecPrm->prCurrFBufInfo->u4FrameNum : 
                ((prDecPrm->prCurrFBufInfo->u4FrameNum << 1) +1);
    prDecPrm->prCurrFBufInfo->i4PicNum = (INT32)u4CurrPicNum;
    prDecPrm->arRefPicList[0].u4RefPicCnt = 0;
    prDecPrm->arRefPicList[1].u4RefPicCnt = 0;  
    prDecPrm->arRefPicList[4].u4RefPicCnt = 0;
    prDecPrm->arRefPicList[5].u4RefPicCnt = 0;

    //if(fgIsFrmPic())
    {
        ASSERT(prDecPrm->u4MaxFBufNum <= 18);
        for(i=0; (i<prDecPrm->u4MaxFBufNum) && (i<18); i++)
        {
            if(prDecPrm->u1PicStruct == TOP_FIELD)
            {
                u4AddTop = 1;
                u4AddBot = 0;
            }
            else if(prDecPrm->u1PicStruct == BOTTOM_FIELD)
            {
                u4AddTop = 0;
                u4AddBot = 1;
            }
            else
            {
                u4AddTop = 0;
                u4AddBot = 0;
            }
      
            if(_fgChkRefInfo(prDecPrm, i, SREF_PIC))
            {        
                if(prDecPrm->arFBufInfo[i].u4FrameNum > prDecPrm->prCurrFBufInfo->u4FrameNum)
                {
                    prDecPrm->arFBufInfo[i].i4FrameNumWrap = (INT32)(prDecPrm->arFBufInfo[i].u4FrameNum - 
                                                            prDecPrm->prSPS->u4MaxFrameNum);
                }
                else
                {
                    prDecPrm->arFBufInfo[i].i4FrameNumWrap = (INT32)prDecPrm->arFBufInfo[i].u4FrameNum;
                }
                if(fgIsFrmPic(prDecPrm->u1PicStruct))
                {
                    prDecPrm->arFBufInfo[i].i4PicNum = prDecPrm->arFBufInfo[i].i4FrameNumWrap;
                }
                else
                {
                    prDecPrm->arFBufInfo[i].i4PicNum = (prDecPrm->arFBufInfo[i].i4FrameNumWrap *2) + 1;
                }
        
                if(prDecPrm->arFBufInfo[i].u1TFldRefType == SREF_PIC)
                {
                    prDecPrm->arFBufInfo[i].i4TFldPicNum = (prDecPrm->arFBufInfo[i].i4FrameNumWrap *2) + (INT32)u4AddTop;
                    _vInsertRefPicList(prDecPrm, &prDecPrm->arRefPicList[0], (INT32)i, SREF_PIC + (0 <<8) + ( i<<16));          
                }
                if(prDecPrm->arFBufInfo[i].u1BFldRefType == SREF_PIC)
                {
                    prDecPrm->arFBufInfo[i].i4BFldPicNum = 
                        (prDecPrm->arFBufInfo[i].i4FrameNumWrap *2) + (INT32)u4AddBot;
                    _vInsertRefPicList(prDecPrm, &prDecPrm->arRefPicList[1], (INT32)i, SREF_PIC + (1 <<8) + ( i<<16));          
                }
                // set display flag according to reference frame error type
                if(prDecPrm->arFBufInfo[i].fgIsErr)
                {
                    prDecPrm->prCurrFBufInfo->fgIsNotDisp = TRUE;
                }
            }
            else if(_fgChkRefInfo(prDecPrm, i, LREF_PIC))
            {        
                prDecPrm->arFBufInfo[i].i4LongTermPicNum = (INT32)prDecPrm->arFBufInfo[i].u4LongTermFrameIdx;
        
                if(prDecPrm->arFBufInfo[i].u1TFldRefType == LREF_PIC)
                {
                    prDecPrm->arFBufInfo[i].i4TFldLongTermPicNum = 
                        (INT32)((prDecPrm->arFBufInfo[i].u4TFldLongTermFrameIdx << 1) + u4AddTop);
                    _vInsertRefPicList(prDecPrm, &prDecPrm->arRefPicList[4], (INT32)i, LREF_PIC + (6 <<8) + ( i<<16));          
                } 
                if(prDecPrm->arFBufInfo[i].u1BFldRefType == LREF_PIC)
                {
                    prDecPrm->arFBufInfo[i].i4BFldLongTermPicNum = 
                        (INT32)((prDecPrm->arFBufInfo[i].u4BFldLongTermFrameIdx << 1) + u4AddBot);        
                    _vInsertRefPicList(prDecPrm, &prDecPrm->arRefPicList[5], (INT32)i, LREF_PIC + (7 <<8) + ( i<<16));          
                }
                // set display flag according to reference frame error type
                if(prDecPrm->arFBufInfo[i].fgIsErr)
                {
                    prDecPrm->prCurrFBufInfo->fgIsNotDisp = TRUE;
                }                
            }      
        }
    }
  
    MC_WRITE32(RW_MC_P_LIST0_FLD, 0);
    AVC_VLD_WRITE32(RW_AVLD_RESET_PIC_NUM, RESET_PIC_NUM);
    AVC_VLD_WRITE32(RW_AVLD_MAX_PIC_NUM, fgIsFrmPic(prDecPrm->u1PicStruct)?
        prDecPrm->prSPS->u4MaxFrameNum : (prDecPrm->prSPS->u4MaxFrameNum << 1));
    AVC_VLD_WRITE32(RW_AVLD_CUR_PIC_NUM, u4CurrPicNum);
    u4Temp = 0;
    _vSetupPRefPicList(prDecPrm->arRefPicList, prDecPrm->arFBufInfo, prDecPrm->u1PicStruct, &u4Temp, 0, 1);
    _vSetupPRefPicList(prDecPrm->arRefPicList, prDecPrm->arFBufInfo, prDecPrm->u1PicStruct, &u4Temp, 4, 5);
}


// *********************************************************************
// Function    : void vSetupPRefPicList(DWRD *dwRefIdx, DWRD dwTFldListIdx, DWRD dwBFldListIdx)
// Description : Setup Ref Pic List
// Parameter   : None
// Return      : None
// *********************************************************************
static void _vSetupBRefPicList(const VDEC_REF_PIC_LIST_T* prRefPicList, 
    const VDEC_FBUF_INFO_T* prFBufInfo, UINT8 u1PicStruct, UINT32 *pu4RefIdx, 
    UINT32 u4TFldListIdx, UINT32 u4BFldListIdx, BOOL *pfgDiff)
{
    UINT32 i;
    UINT32 u4TotalFBuf;

    ASSERT(pu4RefIdx!=NULL);
    ASSERT(pfgDiff!=NULL);
    ASSERT(prRefPicList!=NULL);
    ASSERT(prFBufInfo!=NULL);
    ASSERT(u4TFldListIdx < 6);
    ASSERT(u4BFldListIdx < 6);
    
    u4TotalFBuf = (prRefPicList[u4TFldListIdx].u4RefPicCnt >= prRefPicList[u4BFldListIdx].u4RefPicCnt)?
                         prRefPicList[u4TFldListIdx].u4RefPicCnt : prRefPicList[u4BFldListIdx].u4RefPicCnt;
    if(fgIsFrmPic(u1PicStruct))
    {
        u4TFldListIdx = (prRefPicList[u4TFldListIdx].u4RefPicCnt >= prRefPicList[u4BFldListIdx].u4RefPicCnt)?
                               u4TFldListIdx : u4BFldListIdx;
    }  

    for(i=0; i<u4TotalFBuf; i++)
    {
        if(fgIsFrmPic(u1PicStruct))
        {
            _VDEC_SetBRefPicListReg(prRefPicList[u4TFldListIdx], prRefPicList[u4TFldListIdx+2], prFBufInfo, 
                FRAME + (i << 8) + (pu4RefIdx[0] << 16), u4TFldListIdx);
            if((!pfgDiff[0]) && (AVC_MV_READ32(RW_AMV_B0_REF_PARA + (pu4RefIdx[0]<<2)) != 
                AVC_MV_READ32(RW_AMV_B1_REF_PARA + (pu4RefIdx[0]<<2))))
            {
                pfgDiff[0] = TRUE;
            }
            pu4RefIdx[0] ++;
        }
        else if(u1PicStruct == TOP_FIELD)
        {
            if(i < prRefPicList[u4TFldListIdx].u4RefPicCnt)
            {
                _VDEC_SetBRefPicListReg(prRefPicList[u4TFldListIdx], prRefPicList[u4TFldListIdx+2], prFBufInfo, 
                    TOP_FIELD + (i << 8) + (pu4RefIdx[0] << 16), u4TFldListIdx);
                if(u4TFldListIdx < 4) // Short-term only
                {
                    _VDEC_SetBRefPicListReg(prRefPicList[u4TFldListIdx+2], prRefPicList[u4TFldListIdx+4], prFBufInfo, 
                        TOP_FIELD + (i << 8) + (pu4RefIdx[0] << 16), u4TFldListIdx+2);
                }
                if((!pfgDiff[0]) && (AVC_MV_READ32(RW_AMV_B0_REF_PARA + (pu4RefIdx[0]<<2)) != 
                    AVC_MV_READ32(RW_AMV_B1_REF_PARA + (pu4RefIdx[0]<<2))))
                {
                    pfgDiff[0] = TRUE;
                }
                pu4RefIdx[0] ++;
            }
            if(i < prRefPicList[u4BFldListIdx].u4RefPicCnt)
            {
                _VDEC_SetBRefPicListReg(prRefPicList[u4BFldListIdx], prRefPicList[u4BFldListIdx+2], prFBufInfo, 
                    BOTTOM_FIELD + (i << 8) + (pu4RefIdx[0] << 16), u4BFldListIdx);
                if(u4BFldListIdx < 4) // Short-term only
                {
                    _VDEC_SetBRefPicListReg(prRefPicList[u4BFldListIdx+2], prRefPicList[u4BFldListIdx+4], prFBufInfo, 
                        BOTTOM_FIELD + (i << 8) + (pu4RefIdx[0] << 16), u4BFldListIdx+2);
                }
                if((!pfgDiff[0]) && (AVC_MV_READ32(RW_AMV_B0_REF_PARA + (pu4RefIdx[0]<<2)) != 
                    AVC_MV_READ32(RW_AMV_B1_REF_PARA + (pu4RefIdx[0]<<2))))
                {
                    pfgDiff[0] = TRUE;
                }
                pu4RefIdx[0] ++;
            }
        }
        else if(u1PicStruct == BOTTOM_FIELD)
        {
            if(i < prRefPicList[u4BFldListIdx].u4RefPicCnt)
            {
                _VDEC_SetBRefPicListReg(prRefPicList[u4BFldListIdx], prRefPicList[u4BFldListIdx+2], prFBufInfo, 
                    BOTTOM_FIELD + (i << 8) + (pu4RefIdx[0] << 16), u4BFldListIdx);
                if(u4BFldListIdx < 4) // Short-term only
                {
                    _VDEC_SetBRefPicListReg(prRefPicList[u4BFldListIdx+2], prRefPicList[u4BFldListIdx+4], prFBufInfo, 
                        BOTTOM_FIELD + (i << 8) + (pu4RefIdx[0] << 16), u4BFldListIdx+2);
                }
                if((!pfgDiff[0]) && (AVC_MV_READ32(RW_AMV_B0_REF_PARA + (pu4RefIdx[0]<<2)) != 
                    AVC_MV_READ32(RW_AMV_B1_REF_PARA + (pu4RefIdx[0]<<2))))
                {
                    pfgDiff[0] = TRUE;
                }
                pu4RefIdx[0] ++;
            }      
            if(i < prRefPicList[u4TFldListIdx].u4RefPicCnt)
            {
                _VDEC_SetBRefPicListReg(prRefPicList[u4TFldListIdx], prRefPicList[u4TFldListIdx+2], prFBufInfo, 
                    TOP_FIELD + (i << 8) + (pu4RefIdx[0] << 16), u4TFldListIdx);
                if(u4TFldListIdx < 4) // Short-term only
                {
                    _VDEC_SetBRefPicListReg(prRefPicList[u4TFldListIdx+2], prRefPicList[u4TFldListIdx+4], prFBufInfo, 
                        TOP_FIELD + (i << 8) + (pu4RefIdx[0] << 16), u4TFldListIdx+2);
                }
                if((!pfgDiff[0]) && (AVC_MV_READ32(RW_AMV_B0_REF_PARA + (pu4RefIdx[0]<<2)) != 
                    AVC_MV_READ32(RW_AMV_B1_REF_PARA + (pu4RefIdx[0]<<2))))
                {
                    pfgDiff[0] = TRUE;
                }
                pu4RefIdx[0] ++;
            }
        }
    } 
}


// *********************************************************************
// Function    : void vVDecSetBRefPicList(void)
// Description : Set Ref Pic List for P slice
// Parameter   : None
// Return      : None
// *********************************************************************
static void _vSetBRefPicList(VDEC_AVC_DecPrm* prDecPrm)
{
    UINT32 i;
    UINT32 u4Temp;
    INT32 i4CurrPOC;
    UINT32 u4TotalRPIdx;
    BOOL fgDiff;

    ASSERT(prDecPrm!=NULL);

    _VDEC_SetCurrPOC(prDecPrm);   
    if(fgIsFrmPic(prDecPrm->u1PicStruct))
    {
        i4CurrPOC = prDecPrm->prCurrFBufInfo->i4POC;
        AVC_MV_WRITE32(RW_AMV_CURR_TFLD_POC, (UINT32)(prDecPrm->prCurrFBufInfo->i4TFldPOC & 0x3ffff));
        AVC_MV_WRITE32(RW_AMV_CURR_BFLD_POC, (UINT32)(prDecPrm->prCurrFBufInfo->i4BFldPOC & 0x3ffff));  
        AVC_MV_WRITE32(RW_AMV_CURR_POC, (UINT32)(prDecPrm->prCurrFBufInfo->i4POC & 0x3ffff));
    }
    else
    {
        i4CurrPOC = (prDecPrm->u1PicStruct == TOP_FIELD)? 
                    prDecPrm->prCurrFBufInfo->i4TFldPOC : prDecPrm->prCurrFBufInfo->i4BFldPOC;
        AVC_MV_WRITE32(RW_AMV_CURR_POC, (UINT32)(i4CurrPOC & 0x3ffff));
    }

    prDecPrm->arRefPicList[0].u4RefPicCnt = 0;  
    prDecPrm->arRefPicList[1].u4RefPicCnt = 0;  
    prDecPrm->arRefPicList[2].u4RefPicCnt = 0;  
    prDecPrm->arRefPicList[3].u4RefPicCnt = 0;  
    prDecPrm->arRefPicList[4].u4RefPicCnt = 0;  
    prDecPrm->arRefPicList[5].u4RefPicCnt = 0;  

    //if(fgIsFrmPic())
    {
        ASSERT(prDecPrm->u4MaxFBufNum <= 18);
        for(i=0; (i<prDecPrm->u4MaxFBufNum) && (i<18); i++)
        {     
            if(_fgChkRefInfo(prDecPrm, (UINT32)i, SREF_PIC))
            {
                // Avoid non-existing pic into ref pic list when POC type = 0
                if((prDecPrm->arFBufInfo[i].u1TFldRefType == SREF_PIC)) 
                //            && !((_tMpvDecPrm.tSPS->dwPicOrderCntType == 0) && (_ptFBufInfo[i].fgNonExisting)))
                {
                    // B0
                    _vInsertRefPicList(prDecPrm, &prDecPrm->arRefPicList[0], i4CurrPOC, 
                                    SREF_PIC + (2 <<8) + ((UINT32)i<<16));
                    // B1
                    _vInsertRefPicList(prDecPrm, &prDecPrm->arRefPicList[2], i4CurrPOC, 
                                    SREF_PIC + (4 <<8) + ((UINT32)i<<16));      
                }
                if((prDecPrm->arFBufInfo[i].u1BFldRefType == SREF_PIC) 
                    && !((prDecPrm->prSPS->u4PicOrderCntType == 0) 
                    && (prDecPrm->arFBufInfo[i].fgNonExisting)))
                {
                    // B0
                    _vInsertRefPicList(prDecPrm, &prDecPrm->arRefPicList[1], i4CurrPOC, 
                                    SREF_PIC + (3 <<8) + ((UINT32)i <<16));
                    // B1
                    _vInsertRefPicList(prDecPrm, &prDecPrm->arRefPicList[3], i4CurrPOC, 
                                    SREF_PIC + (5 <<8) + ((UINT32)i <<16));    
                }
                // set display flag according to reference frame error type
                if(prDecPrm->arFBufInfo[i].fgIsErr)
                {
                    prDecPrm->prCurrFBufInfo->fgIsNotDisp = TRUE;
                }
            }
            else if(_fgChkRefInfo(prDecPrm, (UINT32)i, LREF_PIC))
            {
                if(prDecPrm->arFBufInfo[i].u1TFldRefType == LREF_PIC)
                {
                    _vInsertRefPicList(prDecPrm, &prDecPrm->arRefPicList[4], i4CurrPOC, 
                                    LREF_PIC + (8 <<8) + ((UINT32)i <<16));
                }
                if(prDecPrm->arFBufInfo[i].u1BFldRefType == LREF_PIC)
                {
                    _vInsertRefPicList(prDecPrm, &prDecPrm->arRefPicList[5], i4CurrPOC, 
                                    LREF_PIC + (9 <<8) + ((UINT32)i <<16)); 
                }
                // set display flag according to reference frame error type
                if(prDecPrm->arFBufInfo[i].fgIsErr)
                {
                    prDecPrm->prCurrFBufInfo->fgIsNotDisp = TRUE;
                }
            }      
        }
    }

    MC_WRITE32(RW_MC_B_LIST0_FLD, 0);
    MC_WRITE32(RW_MC_B_LIST1_FLD, 0); 
    //vWriteAVCVLD(RW_AVLD_RESET_PIC_NUM, RESET_PIC_NUM);

    //LOG(8, "img->poc, 0x%.8x, 0x%.8x, 0x%.8x\n", prDecPrm->prCurrFBufInfo->i4TFldPOC, 
    //                                        prDecPrm->prCurrFBufInfo->i4BFldPOC,
    //                                        prDecPrm->prCurrFBufInfo->i4POC);
    u4Temp = 0;
    fgDiff = FALSE;
    _vSetupBRefPicList(prDecPrm->arRefPicList, prDecPrm->arFBufInfo, prDecPrm->u1PicStruct, 
        &u4Temp, 0, 1, &fgDiff);    //B0
    _vSetupBRefPicList(prDecPrm->arRefPicList, prDecPrm->arFBufInfo, prDecPrm->u1PicStruct, 
        &u4Temp, 4, 5, &fgDiff);    //long term

    if(fgIsFrmPic(prDecPrm->u1PicStruct))
    {
        u4TotalRPIdx = (prDecPrm->arRefPicList[0].u4RefPicCnt < prDecPrm->arRefPicList[1].u4RefPicCnt)? 
                    prDecPrm->arRefPicList[1].u4RefPicCnt : prDecPrm->arRefPicList[0].u4RefPicCnt;
        u4TotalRPIdx += (prDecPrm->arRefPicList[4].u4RefPicCnt < prDecPrm->arRefPicList[5].u4RefPicCnt)? 
                    prDecPrm->arRefPicList[5].u4RefPicCnt : prDecPrm->arRefPicList[4].u4RefPicCnt;
    }
    else
    {
        u4TotalRPIdx = prDecPrm->arRefPicList[0].u4RefPicCnt + prDecPrm->arRefPicList[1].u4RefPicCnt + 
                    prDecPrm->arRefPicList[4].u4RefPicCnt + prDecPrm->arRefPicList[5].u4RefPicCnt;
    }

    // in field pic, if B0 & B1 identical, switch the 1st 2 items
    if((u4TotalRPIdx>1) && (!fgDiff))
    {
        LOG(9, "!!! Switch 1st items for B0 & B1 !!!!\n");

        if(fgIsFrmPic(prDecPrm->u1PicStruct))
        {
            u4Temp = MC_READ32(RW_MC_B_LIST1);
            MC_WRITE32(RW_MC_B_LIST1, MC_READ32(RW_MC_B_LIST1 + 4));
            #ifdef REF_TRACE
            LOG(2, "BList1 RegIdx=0, YAddr=0x%x\n", MC_READ32(RW_MC_B_LIST1 + 4));
            #endif
            
            MC_WRITE32(RW_MC_B_LIST1 + 4, u4Temp);
            #ifdef REF_TRACE
            LOG(2, "BList1 RegIdx=1, YAddr=0x%x\n", u4Temp);
            #endif
            
            u4Temp = AVC_VLD_READ32(RW_AVLD_REORD_B1_RPL);    
            AVC_VLD_WRITE32(RW_AVLD_REORD_B1_RPL, AVC_VLD_READ32(RW_AVLD_REORD_B1_RPL + 4));
            AVC_VLD_WRITE32(RW_AVLD_REORD_B1_RPL + 4, u4Temp);        
      
            u4Temp = AVC_MV_READ32(RW_AMV_B1_REF_PARA);
            AVC_MV_WRITE32(RW_AMV_B1_REF_PARA, AVC_MV_READ32(RW_AMV_B1_REF_PARA + 8));
            AVC_MV_WRITE32(RW_AMV_B1_REF_PARA + 8, u4Temp);        
      
            u4Temp = AVC_MV_READ32(RW_AMV_B1_REF_PARA + 4);
            AVC_MV_WRITE32(RW_AMV_B1_REF_PARA + 4, AVC_MV_READ32(RW_AMV_B1_REF_PARA + 12));
            AVC_MV_WRITE32(RW_AMV_B1_REF_PARA + 12, u4Temp); 
      
            u4Temp = AVC_MV_READ32(RW_AMV_MV_BUF_ADDR);
            AVC_MV_WRITE32(RW_AMV_MV_BUF_ADDR, AVC_MV_READ32(RW_AMV_MV_BUF_ADDR + 8));
            AVC_MV_WRITE32(RW_AMV_MV_BUF_ADDR + 8, u4Temp);
      
            u4Temp = AVC_MV_READ32(RW_AMV_MV_BUF_ADDR + 4);
            AVC_MV_WRITE32(RW_AMV_MV_BUF_ADDR + 4, AVC_MV_READ32(RW_AMV_MV_BUF_ADDR + 12));
            AVC_MV_WRITE32(RW_AMV_MV_BUF_ADDR + 12, u4Temp); 
        }
        else
        {
            u4Temp = MC_READ32(RW_MC_B_LIST1);
            MC_WRITE32(RW_MC_B_LIST1, MC_READ32(RW_MC_B_LIST1 + 4));
            #ifdef REF_TRACE
            LOG(2, "BList1 RegIdx=0, YAddr=0x%x\n", MC_READ32(RW_MC_B_LIST1 + 4));
            #endif
            
            MC_WRITE32(RW_MC_B_LIST1 + 4, u4Temp);
            #ifdef REF_TRACE
                  LOG(2, "BList1 RegIdx=1, YAddr=0x%x\n", u4Temp);
            #endif
            
            u4Temp = AVC_VLD_READ32(RW_AVLD_REORD_B1_RPL);    
            AVC_VLD_WRITE32(RW_AVLD_REORD_B1_RPL, AVC_VLD_READ32(RW_AVLD_REORD_B1_RPL + 4));
            AVC_VLD_WRITE32(RW_AVLD_REORD_B1_RPL + 4, u4Temp);        
      
            u4Temp = AVC_MV_READ32(RW_AMV_B1_REF_PARA);
            AVC_MV_WRITE32(RW_AMV_B1_REF_PARA, AVC_MV_READ32(RW_AMV_B1_REF_PARA + 4));
            AVC_MV_WRITE32(RW_AMV_B1_REF_PARA + 4, u4Temp);        

            u4Temp = AVC_MV_READ32(RW_AMV_MV_BUF_ADDR);
            AVC_MV_WRITE32(RW_AMV_MV_BUF_ADDR, AVC_MV_READ32(RW_AMV_MV_BUF_ADDR + 4));
            AVC_MV_WRITE32(RW_AMV_MV_BUF_ADDR + 4, u4Temp);        
 
            u4Temp = MC_READ32(RW_MC_B_LIST1_FLD) & 1; // bit 0
            // Write bit 1 to bit 0
            MC_WRITE32(RW_MC_B_LIST1_FLD,  (MC_READ32(RW_MC_B_LIST1_FLD) & (~1)) | ((MC_READ32(RW_MC_B_LIST1_FLD)  >> 1) & 1)); 
            MC_WRITE32(RW_MC_B_LIST1_FLD,  (MC_READ32(RW_MC_B_LIST1_FLD) & (~2)) | (u4Temp << 1)); 
        }
    }
}


// *********************************************************************
// Function    : void vSetPicRefType(BYTE bPicStruct, bPicStruct bRefType)
// Description : set pic ref type
// Parameter   : BYTE bPicType: pic struct : FRAME, TOP_FIELD, BOTTOM_FIELD
//                     BYTE bRefType: pic ref type: NREF_PIC, SREF_PIC, LREF_PIC
// Return      : None
// *********************************************************************
static void _vSetPicRefType(VDEC_FBUF_INFO_T* prFBufInfo, UINT8 u1PicStruct, UINT8 u1RefType)
{
      ASSERT(prFBufInfo!=NULL);
      
    if(u1PicStruct & TOP_FIELD)
    {
        prFBufInfo->u1TFldRefType = u1RefType;
    }
    if(u1PicStruct & BOTTOM_FIELD)
    {
        prFBufInfo->u1BFldRefType = u1RefType;
    }
    //if(bPicStruct == FRAME)
    {
        prFBufInfo->u1FBufRefType = u1RefType;
    }
    //else
    {
        //_ptCurrFBufInfo->bFBufRefType = NREF_PIC;
    }
}

// *********************************************************************
// Function    : BYTE bGetPicRefType(BYTE bPicStruct)
// Description : get pic ref type
// Parameter   : BYTE bPicType: pic struct : FRAME, TOP_FIELD, BOTTOM_FIELD
// Return      : BYTE bRefType: pic ref type: NREF_PIC, SREF_PIC, LREF_PIC
// *********************************************************************
static UINT8 _bGetPicRefType(VDEC_FBUF_INFO_T rFBufInfo, UINT8 u1PicStruct)
{
    
    if(u1PicStruct == TOP_FIELD)
    {
        return rFBufInfo.u1TFldRefType;
    }
    else if(u1PicStruct == BOTTOM_FIELD)
    {
        return rFBufInfo.u1BFldRefType;
    }
    else//if(bPicStruct == FRAME)
    {
        return rFBufInfo.u1FBufRefType;
    }
}

// *********************************************************************
// Function    : void vClrPicRefInfo(BYTE bPicType, BYTE bFBufIdx)
// Description : Clear picture info in frame buffer
// Parameter   : None
// Return      : None
// *********************************************************************
static void _vClrPicRefInfo(VDEC_AVC_DecPrm* prDecPrm, UINT8 u1PicType, UINT8 u1FBufIdx)
{
    ASSERT(prDecPrm!=NULL);
    ASSERT(u1FBufIdx < 18);

    if(u1FBufIdx >= 18)
    {
        return;
    }
    if(u1PicType & TOP_FIELD)
    {
        prDecPrm->arFBufInfo[u1FBufIdx].u1TFldRefType = NREF_PIC;
    }
    if(u1PicType & BOTTOM_FIELD)
    {
        prDecPrm->arFBufInfo[u1FBufIdx].u1BFldRefType = NREF_PIC;
    }
    prDecPrm->arFBufInfo[u1FBufIdx].u1FBufRefType = NREF_PIC;
    
    if(fgIsNonRefFBuf(prDecPrm->arFBufInfo[u1FBufIdx].u1FBufRefType, 
        prDecPrm->arFBufInfo[u1FBufIdx].u1TFldRefType, 
        prDecPrm->arFBufInfo[u1FBufIdx].u1BFldRefType) &&
        (prDecPrm->arFBufInfo[u1FBufIdx].ucMvBufIdx < 0xff))
    {
        if(prDecPrm->ucEmpMVBufMaxNum == (UCHAR)prDecPrm->u4MaxFBufNum)
        {
            LOG(2, "MV buf duplicate\n");
        }
        else
        {
            if(prDecPrm->ucEmpMVBufMaxNum > 17)
            {
                ASSERT(prDecPrm->ucEmpMVBufMaxNum <= 17);
                return;
            }
            prDecPrm->aucEmpMVBufIdxMap[prDecPrm->ucEmpMVBufMaxNum] = prDecPrm->arFBufInfo[u1FBufIdx].ucMvBufIdx;
            //LOG(2, "free mvbufIdx %d, maxNum %d\n", prDecPrm->arFBufInfo[u1FBufIdx].ucMvBufIdx, prDecPrm->ucEmpMVBufMaxNum);

            prDecPrm->ucEmpMVBufMaxNum++;
            prDecPrm->arFBufInfo[u1FBufIdx].ucMvBufIdx = 0xff;
        }
    }
}

// *********************************************************************
// Function    : void vAdapRefPicmarkingProce(void)
// Description : marking the decoded ref pic with adaptive method
// Parameter   : None
// Return      : None
// *********************************************************************
static void _vAdapRefPicmarkingProce(VDEC_AVC_DecPrm* prDecPrm)
{
    INT32 i4PicNumX;
    UINT32 u4Cnt;
    UINT32 i;

      ASSERT(prDecPrm!=NULL);
      
    u4Cnt = 0;
    while(prDecPrm->prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] != 0)
    {
        switch(prDecPrm->prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] & 0xff)
        {
            case 0:    //end loop
                break;
            case 1:    //mark a short-term ref pic as "unused for reference"
                // picNumX
                prDecPrm->prSliceHdr->u4DifferencOfPicNumsMinus1 = 
                    prDecPrm->prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] >> 8;
                //if(fgIsFrmPic())
                {
                    i4PicNumX = (prDecPrm->prCurrFBufInfo->i4PicNum - 
                            (INT32)prDecPrm->prSliceHdr->u4DifferencOfPicNumsMinus1) - 1;
                }

                for(i=0; i < prDecPrm->u4MaxFBufNum; i++)
                {
                    if(fgIsFrmPic(prDecPrm->u1PicStruct) && 
                        (prDecPrm->arFBufInfo[i].i4PicNum == i4PicNumX) && 
                        (prDecPrm->arFBufInfo[i].u1FBufRefType == SREF_PIC))
                    {
                        _vClrPicRefInfo(prDecPrm, FRAME, (UINT8)i);            
                        i = prDecPrm->u4MaxFBufNum; // break
                    }
                    else if((!fgIsFrmPic(prDecPrm->u1PicStruct)) && 
                        (((prDecPrm->arFBufInfo[i].i4TFldPicNum == i4PicNumX) && 
                        (prDecPrm->arFBufInfo[i].u1TFldRefType == SREF_PIC)) ||
                        ((prDecPrm->arFBufInfo[i].i4BFldPicNum == i4PicNumX) && 
                        (prDecPrm->arFBufInfo[i].u1BFldRefType == SREF_PIC))))
                    {
                        if((prDecPrm->arFBufInfo[i].i4TFldPicNum == i4PicNumX) && 
                            (prDecPrm->arFBufInfo[i].u1TFldRefType == SREF_PIC))
                        {
                            _vClrPicRefInfo(prDecPrm, TOP_FIELD, (UINT8)i);            
                        }
                        if((prDecPrm->arFBufInfo[i].i4BFldPicNum == i4PicNumX) && 
                            (prDecPrm->arFBufInfo[i].u1BFldRefType == SREF_PIC))
                        {
                            _vClrPicRefInfo(prDecPrm, BOTTOM_FIELD, (UINT8)i);            
                        }
                        i = prDecPrm->u4MaxFBufNum; // break
                    }
                }
                break;
            case 2:    //mark a long term ref pic as "unused for reference"
                prDecPrm->prSliceHdr->u4LongTermPicNum = 
                    prDecPrm->prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] >> 8;
                i4PicNumX = (INT32)prDecPrm->prSliceHdr->u4LongTermPicNum;
                for(i=0; i < prDecPrm->u4MaxFBufNum; i++)
                {
                    if(fgIsFrmPic(prDecPrm->u1PicStruct) && 
                        (prDecPrm->arFBufInfo[i].i4LongTermPicNum == i4PicNumX) && 
                        (prDecPrm->arFBufInfo[i].u1FBufRefType == LREF_PIC))
                    {
                        _vClrPicRefInfo(prDecPrm, FRAME, (UINT8)i);            
                        i = prDecPrm->u4MaxFBufNum; // break
                    }
                    else if((!fgIsFrmPic(prDecPrm->u1PicStruct)) && 
                        (((prDecPrm->arFBufInfo[i].i4TFldLongTermPicNum == i4PicNumX) && 
                        (prDecPrm->arFBufInfo[i].u1TFldRefType == LREF_PIC)) ||
                        ((prDecPrm->arFBufInfo[i].i4BFldLongTermPicNum == i4PicNumX) && 
                        (prDecPrm->arFBufInfo[i].u1BFldRefType == LREF_PIC))))
                    {
                        if((prDecPrm->arFBufInfo[i].i4TFldLongTermPicNum == i4PicNumX) && 
                            (prDecPrm->arFBufInfo[i].u1TFldRefType == LREF_PIC))
                        {
                            _vClrPicRefInfo(prDecPrm, TOP_FIELD, (UINT8)i);            
                        }
                        if((prDecPrm->arFBufInfo[i].i4BFldLongTermPicNum == i4PicNumX) && 
                            (prDecPrm->arFBufInfo[i].u1BFldRefType == LREF_PIC))
                        {
                            _vClrPicRefInfo(prDecPrm, BOTTOM_FIELD, (UINT8)i);            
                        }
                        i = prDecPrm->u4MaxFBufNum; // break
                    }
                }
                break;
            case 3:    //mark a short term ref pic as "used for long-term reference" and assign a long-term index
                prDecPrm->prSliceHdr->u4DifferencOfPicNumsMinus1 = 
                    (prDecPrm->prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] >> 8) & 0xff;        
                prDecPrm->prSliceHdr->u4LongTermFrameIdx = 
                    prDecPrm->prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] >> 16;
                i4PicNumX = (INT32)prDecPrm->prSliceHdr->u4LongTermFrameIdx;
                for(i=0; i < prDecPrm->u4MaxFBufNum; i++)
                {
                    if(i != prDecPrm->u1DecFBufIdx)
                    {
                        if((prDecPrm->arFBufInfo[i].u1FBufStatus == FRAME) && 
                            (prDecPrm->arFBufInfo[i].u4LongTermFrameIdx == (UINT32)i4PicNumX) && 
                            (prDecPrm->arFBufInfo[i].u1FBufRefType == LREF_PIC))
                        {
                            _vClrPicRefInfo(prDecPrm, FRAME, (UINT8)i);            
                            i = prDecPrm->u4MaxFBufNum; // break
                        }
                        else if((prDecPrm->arFBufInfo[i].u1FBufStatus != FRAME) && 
                            (((prDecPrm->arFBufInfo[i].u4TFldLongTermFrameIdx == (UINT32)i4PicNumX) && 
                            (prDecPrm->arFBufInfo[i].u1TFldRefType == LREF_PIC)) ||
                            ((prDecPrm->arFBufInfo[i].u4BFldLongTermFrameIdx == (UINT32)i4PicNumX) && 
                            (prDecPrm->arFBufInfo[i].u1BFldRefType == LREF_PIC))))
                        {
                            if((prDecPrm->arFBufInfo[i].u4TFldLongTermFrameIdx == (UINT32)i4PicNumX) && 
                                (prDecPrm->arFBufInfo[i].u1TFldRefType == LREF_PIC))
                            {
                                _vClrPicRefInfo(prDecPrm, TOP_FIELD, (UINT8)i);            
                            }
                            if((prDecPrm->arFBufInfo[i].u4BFldLongTermFrameIdx == (UINT32)i4PicNumX) && 
                                (prDecPrm->arFBufInfo[i].u1BFldRefType == LREF_PIC))
                            {
                                _vClrPicRefInfo(prDecPrm, BOTTOM_FIELD, (UINT8)i);            
                            }
                            i = prDecPrm->u4MaxFBufNum; // break
                        }
                    }
                }
                // picNumX
                //if(fgIsFrmPic())
                {
                    i4PicNumX = (prDecPrm->prCurrFBufInfo->i4PicNum - 
                        (INT32)prDecPrm->prSliceHdr->u4DifferencOfPicNumsMinus1) - 1;
                }
 
                for(i=0; i < prDecPrm->u4MaxFBufNum; i++)
                {
                    if(fgIsFrmPic(prDecPrm->u1PicStruct) && 
                        (prDecPrm->arFBufInfo[i].i4PicNum == i4PicNumX) &&
                        (prDecPrm->arFBufInfo[i].u1FBufRefType == SREF_PIC) && 
                        (!prDecPrm->arFBufInfo[i].fgNonExisting))
                    {
                        prDecPrm->arFBufInfo[i].u1FBufRefType = LREF_PIC;
                        prDecPrm->arFBufInfo[i].u1TFldRefType = LREF_PIC;
                        prDecPrm->arFBufInfo[i].u1BFldRefType = LREF_PIC;          
                        prDecPrm->arFBufInfo[i].u4LongTermFrameIdx = 
                            prDecPrm->prSliceHdr->u4LongTermFrameIdx;            
                        prDecPrm->arFBufInfo[i].u4TFldLongTermFrameIdx = 
                            prDecPrm->prSliceHdr->u4LongTermFrameIdx;            
                        prDecPrm->arFBufInfo[i].u4BFldLongTermFrameIdx = 
                            prDecPrm->prSliceHdr->u4LongTermFrameIdx;            
                        prDecPrm->arFBufInfo[i].i4LongTermPicNum = 
                            (INT32)prDecPrm->prSliceHdr->u4LongTermFrameIdx;            
                        prDecPrm->arFBufInfo[i].i4TFldLongTermPicNum = 
                            (INT32)prDecPrm->prSliceHdr->u4LongTermFrameIdx;            
                        prDecPrm->arFBufInfo[i].i4BFldLongTermPicNum = 
                            (INT32)prDecPrm->prSliceHdr->u4LongTermFrameIdx;            
                        i = prDecPrm->u4MaxFBufNum; // break
                    }
                    else if((!fgIsFrmPic(prDecPrm->u1PicStruct)) &&
                        (((prDecPrm->arFBufInfo[i].i4TFldPicNum == i4PicNumX) && 
                        (prDecPrm->arFBufInfo[i].u1TFldRefType == SREF_PIC) && 
                        (!prDecPrm->arFBufInfo[i].fgNonExisting)) || 
                        ((prDecPrm->arFBufInfo[i].i4BFldPicNum == i4PicNumX) && 
                        (prDecPrm->arFBufInfo[i].u1BFldRefType == SREF_PIC) &&
                        (!prDecPrm->arFBufInfo[i].fgNonExisting))))
                    {
                        if((prDecPrm->arFBufInfo[i].i4TFldPicNum == i4PicNumX) && 
                            (prDecPrm->arFBufInfo[i].u1TFldRefType == SREF_PIC) && 
                            (!prDecPrm->arFBufInfo[i].fgNonExisting))
                        {
                            prDecPrm->arFBufInfo[i].u1TFldRefType = LREF_PIC;
                            prDecPrm->arFBufInfo[i].u4TFldLongTermFrameIdx = 
                                prDecPrm->prSliceHdr->u4LongTermFrameIdx;
                            prDecPrm->arFBufInfo[i].i4TFldLongTermPicNum = 
                                (INT32)((prDecPrm->prSliceHdr->u4LongTermFrameIdx << 1) + 
                                ((prDecPrm->u1PicStruct == TOP_FIELD)? 1: 0));
                            
                            if(prDecPrm->arFBufInfo[i].u1BFldRefType == LREF_PIC)
                            {
                                prDecPrm->arFBufInfo[i].u1FBufRefType = LREF_PIC;
                                prDecPrm->arFBufInfo[i].u4LongTermFrameIdx = 
                                    prDecPrm->prSliceHdr->u4LongTermFrameIdx;
                                prDecPrm->arFBufInfo[i].i4LongTermPicNum = 
                                    (INT32)prDecPrm->arFBufInfo[i].u4LongTermFrameIdx;
                            }
                        }
                        if((prDecPrm->arFBufInfo[i].i4BFldPicNum == i4PicNumX) && 
                            (prDecPrm->arFBufInfo[i].u1BFldRefType == SREF_PIC) && 
                            (!prDecPrm->arFBufInfo[i].fgNonExisting))
                        {
                            prDecPrm->arFBufInfo[i].u1BFldRefType = LREF_PIC;
                            prDecPrm->arFBufInfo[i].u4BFldLongTermFrameIdx = 
                                prDecPrm->prSliceHdr->u4LongTermFrameIdx;
                            prDecPrm->arFBufInfo[i].i4BFldLongTermPicNum = 
                                (INT32)((prDecPrm->prSliceHdr->u4LongTermFrameIdx << 1) + 
                                ((prDecPrm->u1PicStruct == BOTTOM_FIELD)? 1: 0));
                            if(prDecPrm->arFBufInfo[i].u1TFldRefType == LREF_PIC)
                            {
                                prDecPrm->arFBufInfo[i].u1FBufRefType = LREF_PIC;
                                prDecPrm->arFBufInfo[i].u4LongTermFrameIdx = 
                                    prDecPrm->prSliceHdr->u4LongTermFrameIdx;    
                                prDecPrm->arFBufInfo[i].i4LongTermPicNum = 
                                    (INT32)prDecPrm->arFBufInfo[i].u4LongTermFrameIdx;
                            }
                        }
                        i = prDecPrm->u4MaxFBufNum; // break
                    }
                }
                break;      
            case 4:    //specify the max long-term index and 
                //mark all long-term ref pic having indices greater than the value as "unused for reference"
                prDecPrm->prSliceHdr->u4MaxLongTermFrameIdxPlus1 = 
                    prDecPrm->prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] >> 8;
                if(prDecPrm->prSliceHdr->u4MaxLongTermFrameIdxPlus1 == 0)
                {
                    prDecPrm->u1MaxLongTermFrameIdx = 0xff;
                    i4PicNumX = 0;
                }
                else
                {
                    i4PicNumX = (INT32)prDecPrm->prSliceHdr->u4MaxLongTermFrameIdxPlus1;
                }
                ASSERT(prDecPrm->u4MaxFBufNum <= 18);
                for(i=0; i < (prDecPrm->u4MaxFBufNum) && (i<18); i++)
                {
                    if((prDecPrm->arFBufInfo[i].u4LongTermFrameIdx >= (UINT32)i4PicNumX) && 
                        ((prDecPrm->arFBufInfo[i].u1TFldRefType == LREF_PIC) || 
                        (prDecPrm->arFBufInfo[i].u1BFldRefType == LREF_PIC)))
                    {
                        _vClrPicRefInfo(prDecPrm, FRAME, (UINT8)i);
                        //prDecPrm->arFBufInfo[i].u1TFldRefType = NREF_PIC;
                        //prDecPrm->arFBufInfo[i].u1BFldRefType = NREF_PIC;
                        //prDecPrm->arFBufInfo[i].u1FBufRefType = NREF_PIC;
                    }
                }
                break;
            case 5:    //mark all ref pic as "unused for reference" and set MaxLongTermFrameIdx to "no long-term frame indices" 
                prDecPrm->prCurrFBufInfo->u4FrameNum = 0;
                prDecPrm->prCurrFBufInfo->i4PicNum = 0;
                prDecPrm->prCurrFBufInfo->i4TFldPicNum = 0;        
                prDecPrm->prCurrFBufInfo->i4BFldPicNum = 0;         
                if(prDecPrm->u1PicStruct == TOP_FIELD)
                {
                    prDecPrm->prCurrFBufInfo->i4TFldPOC = 0;
                }
                else if(prDecPrm->u1PicStruct == BOTTOM_FIELD)
                {
                    prDecPrm->prCurrFBufInfo->i4BFldPOC = 0;
                }
                else if(prDecPrm->u1PicStruct == FRAME)
                {
                    prDecPrm->prCurrFBufInfo->i4TFldPOC -= prDecPrm->prCurrFBufInfo->i4POC;
                    prDecPrm->prCurrFBufInfo->i4BFldPOC -= prDecPrm->prCurrFBufInfo->i4POC;
                    prDecPrm->prCurrFBufInfo->i4POC = 
                        (prDecPrm->prCurrFBufInfo->i4TFldPOC < prDecPrm->prCurrFBufInfo->i4BFldPOC)?
                        prDecPrm->prCurrFBufInfo->i4TFldPOC: prDecPrm->prCurrFBufInfo->i4BFldPOC;
                }
                _VDEC_FlushBufRefInfo(prDecPrm);
                break;      
            case 6:    //mark current pic as "used for long-term reference" and assign a long term frame index
                prDecPrm->prSliceHdr->u4LongTermFrameIdx = 
                    prDecPrm->prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] >> 8;
                i4PicNumX = (INT32)prDecPrm->prSliceHdr->u4LongTermFrameIdx;
                for(i=0; i < prDecPrm->u4MaxFBufNum; i++)
                {
                    if(i != prDecPrm->u1DecFBufIdx)
                    {
                        if((prDecPrm->arFBufInfo[i].u1FBufStatus == FRAME) && 
                            (prDecPrm->arFBufInfo[i].u4LongTermFrameIdx == (UINT32)i4PicNumX) && 
                            (prDecPrm->arFBufInfo[i].u1FBufRefType == LREF_PIC))
                        {
                            _vClrPicRefInfo(prDecPrm, FRAME, (UINT8)i);
                            i = prDecPrm->u4MaxFBufNum; // break
                        }
                        else if((prDecPrm->arFBufInfo[i].u1FBufStatus != FRAME) &&
                            (((prDecPrm->arFBufInfo[i].u4TFldLongTermFrameIdx == (UINT32)i4PicNumX) && 
                            (prDecPrm->arFBufInfo[i].u1TFldRefType == LREF_PIC)) ||
                            ((prDecPrm->arFBufInfo[i].u4BFldLongTermFrameIdx == (UINT32)i4PicNumX) && 
                            (prDecPrm->arFBufInfo[i].u1BFldRefType == LREF_PIC))))
                        {
                            if((prDecPrm->arFBufInfo[i].u4TFldLongTermFrameIdx == (UINT32)i4PicNumX) && 
                                (prDecPrm->arFBufInfo[i].u1TFldRefType == LREF_PIC))
                            {
                                _vClrPicRefInfo(prDecPrm, TOP_FIELD, (UINT8)i);
                            }
                            if((prDecPrm->arFBufInfo[i].u4BFldLongTermFrameIdx == (UINT32)i4PicNumX) && 
                                (prDecPrm->arFBufInfo[i].u1BFldRefType == LREF_PIC))
                            {
                                _vClrPicRefInfo(prDecPrm, BOTTOM_FIELD, (UINT8)i);
                            }
                            i = prDecPrm->u4MaxFBufNum; // break
                        }
                    }
                }

                if(fgIsFrmPic(prDecPrm->u1PicStruct)) // 2 flds decoded
                {
                    prDecPrm->prCurrFBufInfo->u1FBufRefType = LREF_PIC;          
                    prDecPrm->prCurrFBufInfo->u1TFldRefType = LREF_PIC;  
                    prDecPrm->prCurrFBufInfo->u1BFldRefType = LREF_PIC;        
                    prDecPrm->prCurrFBufInfo->u4LongTermFrameIdx = 
                        prDecPrm->prSliceHdr->u4LongTermFrameIdx;
                    prDecPrm->prCurrFBufInfo->u4TFldLongTermFrameIdx = 
                        prDecPrm->prSliceHdr->u4LongTermFrameIdx;            
                    prDecPrm->prCurrFBufInfo->u4BFldLongTermFrameIdx = 
                        prDecPrm->prSliceHdr->u4LongTermFrameIdx;            
                }
                else if(prDecPrm->u1PicStruct & TOP_FIELD) // 1 fld decoded
                {
                    prDecPrm->prCurrFBufInfo->u1TFldRefType = LREF_PIC;
                    prDecPrm->prCurrFBufInfo->u4LongTermFrameIdx = 
                        prDecPrm->prSliceHdr->u4LongTermFrameIdx;
                    prDecPrm->prCurrFBufInfo->u4TFldLongTermFrameIdx = 
                        prDecPrm->prSliceHdr->u4LongTermFrameIdx;
                    if(prDecPrm->prCurrFBufInfo->u1BFldRefType == LREF_PIC)
                    {
                        prDecPrm->prCurrFBufInfo->u1FBufRefType = LREF_PIC;
                    }
                }
                else if(prDecPrm->u1PicStruct & BOTTOM_FIELD) // 1 fld decoded
                {
                    prDecPrm->prCurrFBufInfo->u1BFldRefType = LREF_PIC;
                    prDecPrm->prCurrFBufInfo->u4LongTermFrameIdx = 
                        prDecPrm->prSliceHdr->u4LongTermFrameIdx;        
                    prDecPrm->prCurrFBufInfo->u4BFldLongTermFrameIdx = 
                        prDecPrm->prSliceHdr->u4LongTermFrameIdx;
                    if(prDecPrm->prCurrFBufInfo->u1TFldRefType == LREF_PIC)
                    {
                        prDecPrm->prCurrFBufInfo->u1FBufRefType = LREF_PIC;
                    }
                }
                break;    
            default:
                break;
        }
        u4Cnt ++;
    }
}                 

// *********************************************************************
// Function    : BOOL fgIsVDecComplete(void)
// Description : Check if VDec complete with interrupt
// Parameter   : None
// Return      : None
// *********************************************************************
static BOOL _vECFindNeighborFrm(const VDEC_AVC_DecPrm* prDecPrm, 
    UINT32 u4YAddr, UINT32* pu4Addr)
{
    UINT32 u4Idx, u4POCDiff = 0xFFFF, u4Diff = 0;
    UINT32 u4TmpAddr = 0;
    INT32 i4FBufPOC = 0;
    
    ASSERT(prDecPrm!=NULL);
    ASSERT(pu4Addr!=NULL);
    
    for(u4Idx = 0; u4Idx < prDecPrm->u4MaxFBufNum; u4Idx++)
    {
        if((prDecPrm->arFBufInfo[u4Idx].u1FBufStatus==FRAME) &&
            (prDecPrm->arFBufInfo[u4Idx].u4YAddr == u4YAddr))
        {
            i4FBufPOC = prDecPrm->arFBufInfo[u4Idx].i4POC;
        }
    }

    for(u4Idx = 0; u4Idx < prDecPrm->u4MaxFBufNum; u4Idx++)
    {
        u4TmpAddr = prDecPrm->arFBufInfo[u4Idx].u4YAddr;
        if((prDecPrm->arFBufInfo[u4Idx].u1FBufStatus == FRAME) &&
            ((u4TmpAddr & FBUF_ERR_MASK) == 0))
        {
            u4Diff = (UINT32)POC_abs(prDecPrm->arFBufInfo[u4Idx].i4POC - i4FBufPOC);
            if(u4Diff < u4POCDiff)
            {
                u4POCDiff = u4Diff;
                *pu4Addr = u4TmpAddr;
            }
        }
    }

    if(u4POCDiff < 0xFFFF)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


// *********************************************************************
// Function    : BOOL fgIsVDecComplete(void)
// Description : Check if VDec complete with interrupt
// Parameter   : None
// Return      : None
// *********************************************************************
static void _vECBufSetting(const VDEC_AVC_DecPrm* prDecPrm, UINT32 u4RefCnt, UINT32 u4RefEntryCnt)
{
    UINT32 u4CurAddr, /*u4NextAddr, */u4NeighborAddr = 0;
    UINT8 u1RegIdx, u1PrevIdx;
    static UINT32 u4P0Addr = 0, u4B0Addr = 0, u4B1Addr = 0, u4MVAddr = 0;
    static UINT32 u4P0Para = 0, u4B0Para = 0, u4B1Para = 0;

    //P0 first entry
    u4CurAddr = MC_READ32(RW_MC_P_LIST0);
    if((u4CurAddr & FBUF_ERR_MASK) > 0)
    {
        if(_vECFindNeighborFrm(prDecPrm, u4CurAddr, &u4NeighborAddr))
        {
            MC_WRITE32(RW_MC_P_LIST0, u4NeighborAddr);
        }
        else
        {
            MC_WRITE32(RW_MC_P_LIST0, MC_READ32(RW_MC_P_LIST0) & 0x0FFFFFFF);
        }
        /*u4NextAddr = MC_READ32(RW_MC_P_LIST0 + (1 <<2));
        //find next entry buffer with no error
        if((u4RefCnt == 1) || ((u4NextAddr & FBUF_ERR_MASK) > 0))
        {
            MC_WRITE32(RW_MC_P_LIST0, MC_READ32(RW_MC_P_LIST0) & 0x0FFFFFFF);
        }
        else
        {
            MC_WRITE32(RW_MC_P_LIST0, MC_READ32(RW_MC_P_LIST0 + (1 <<2)));
        }*/
    }
    else if(u4CurAddr == 0) //for I frame error concealment
    {
        MC_WRITE32(RW_MC_P_LIST0, u4P0Addr);
        AVC_MV_WRITE32(RW_AMV_P_REF_PARA, u4P0Para);
        if(AVC_MV_READ32(RW_AMV_MV_BUF_ADDR) == 0)
        {
            AVC_MV_WRITE32(RW_AMV_MV_BUF_ADDR, u4MVAddr);
        }
    }
    //B0 first entry
    u4CurAddr = MC_READ32(RW_MC_B_LIST0);
    if((u4CurAddr & FBUF_ERR_MASK) > 0)
    {
        if(_vECFindNeighborFrm(prDecPrm, u4CurAddr, &u4NeighborAddr))
        {
            MC_WRITE32(RW_MC_B_LIST0, u4NeighborAddr);
        }
        else
        {
            MC_WRITE32(RW_MC_B_LIST0, MC_READ32(RW_MC_B_LIST0) & 0x0FFFFFFF);
        }
        /* 
        u4NextAddr = MC_READ32(RW_MC_B_LIST0 + (1 <<2));
        //find next entry buffer with no error
        if((u4RefCnt == 1) || ((u4NextAddr & FBUF_ERR_MASK) > 0))
        {
            MC_WRITE32(RW_MC_B_LIST0, MC_READ32(RW_MC_B_LIST0) & 0x0FFFFFFF);
        }
        else
        {
            MC_WRITE32(RW_MC_B_LIST0, MC_READ32(RW_MC_B_LIST0 + (1 <<2)));
        }*/
    }
    else if(u4CurAddr == 0)
    {
        MC_WRITE32(RW_MC_B_LIST0, u4B0Addr);
        AVC_MV_WRITE32(RW_AMV_B0_REF_PARA, u4B0Para);
    }
    //B1 first entry
    u4CurAddr = MC_READ32(RW_MC_B_LIST1);    
    if((u4CurAddr & FBUF_ERR_MASK) > 0)
    {
        if(_vECFindNeighborFrm(prDecPrm, u4CurAddr, &u4NeighborAddr))
        {
            MC_WRITE32(RW_MC_B_LIST1, u4NeighborAddr);
        }
        else
        {
            MC_WRITE32(RW_MC_B_LIST1, MC_READ32(RW_MC_B_LIST1) & 0x0FFFFFFF);
        }
        
        /*u4NextAddr = MC_READ32(RW_MC_B_LIST1 + (1 <<2));
        //find next entry buffer with no error
        if((u4RefCnt == 1) || ((u4NextAddr & FBUF_ERR_MASK) > 0))
        {
            MC_WRITE32(RW_MC_B_LIST1, MC_READ32(RW_MC_B_LIST1) & 0x0FFFFFFF);
        }
        else
        {
            MC_WRITE32(RW_MC_B_LIST1, MC_READ32(RW_MC_B_LIST1 + (1 <<2)));
        }*/
    }
    else if(u4CurAddr == 0)
    {
        MC_WRITE32(RW_MC_B_LIST1, u4B1Addr);
        AVC_MV_WRITE32(RW_AMV_B1_REF_PARA, u4B1Para);
    }
    
    //------- frame gap handling, ref frame may missing, need to set reg (default=0)
    for(u1RegIdx = 1; u1RegIdx < (UINT8)u4RefEntryCnt; u1RegIdx++)
    {
        u1PrevIdx = (UINT8)(u1RegIdx - 1);
        //p list
        u4CurAddr = MC_READ32((UINT32)(RW_MC_P_LIST0 + (u1RegIdx <<2)));
        if(u4CurAddr == 0)
        {
            MC_WRITE32((UINT32)(RW_MC_P_LIST0 + (u1RegIdx <<2)), MC_READ32((UINT32)(RW_MC_P_LIST0 + (u1PrevIdx <<2))));
            AVC_MV_WRITE32((UINT32)(RW_AMV_P_REF_PARA + (u1RegIdx<<3)), 
                AVC_MV_READ32((UINT32)(RW_AMV_P_REF_PARA + (u1PrevIdx<<3))));
            AVC_MV_WRITE32((UINT32)(RW_AMV_P_REF_PARA + (u1RegIdx<<3) + 4), 
                AVC_MV_READ32((UINT32)(RW_AMV_P_REF_PARA + (u1PrevIdx<<3) + 4)));
        }
        else if((u4CurAddr & FBUF_ERR_MASK) > 0)
        {
            if(_vECFindNeighborFrm(prDecPrm, u4CurAddr, &u4NeighborAddr))
            {
                MC_WRITE32(RW_MC_P_LIST0, u4NeighborAddr);
            }
            else
            {
                MC_WRITE32(RW_MC_P_LIST0, MC_READ32(RW_MC_P_LIST0) & 0x0FFFFFFF);
            }
        }
        
        //b list 0
        u4CurAddr = MC_READ32((UINT32)(RW_MC_B_LIST0 + (u1RegIdx <<2)));
        if(u4CurAddr == 0)
        {
            MC_WRITE32((UINT32)(RW_MC_B_LIST0 + (u1RegIdx<<2)), MC_READ32((UINT32)(RW_MC_B_LIST0 + (u1PrevIdx<<2))));
            AVC_MV_WRITE32((UINT32)(RW_AMV_B0_REF_PARA + (u1RegIdx<<3)), 
                AVC_MV_READ32((UINT32)(RW_AMV_B0_REF_PARA + (u1PrevIdx<<3))));
            AVC_MV_WRITE32((UINT32)(RW_AMV_B0_REF_PARA + (u1RegIdx<<3) + 4), 
                AVC_MV_READ32((UINT32)(RW_AMV_B0_REF_PARA + (u1PrevIdx<<3) + 4)));
        }
        else if((u4CurAddr & FBUF_ERR_MASK) > 0)
        {
            if(_vECFindNeighborFrm(prDecPrm, u4CurAddr, &u4NeighborAddr))
            {
                MC_WRITE32(RW_MC_B_LIST0, u4NeighborAddr);
            }
            else
            {
                MC_WRITE32(RW_MC_B_LIST0, MC_READ32(RW_MC_B_LIST0) & 0x0FFFFFFF);
            }
        }        
        //b list 1
        u4CurAddr = MC_READ32((UINT32)(RW_MC_B_LIST1 + (u1RegIdx <<2)));
        if(u4CurAddr == 0)
        {
            MC_WRITE32((UINT32)(RW_MC_B_LIST1 + (u1RegIdx<<2)), MC_READ32((UINT32)(RW_MC_B_LIST1 + (u1PrevIdx<<2))));
            AVC_MV_WRITE32((UINT32)(RW_AMV_B1_REF_PARA + (u1RegIdx<<3)), 
                AVC_MV_READ32((UINT32)(RW_AMV_B1_REF_PARA + (u1PrevIdx<<3))));
            AVC_MV_WRITE32((UINT32)(RW_AMV_B1_REF_PARA + (u1RegIdx<<3) + 4), 
                AVC_MV_READ32((UINT32)(RW_AMV_B1_REF_PARA + (u1PrevIdx<<3) + 4)));
        }
        else if((u4CurAddr & FBUF_ERR_MASK) > 0)
        {
            if(_vECFindNeighborFrm(prDecPrm, u4CurAddr, &u4NeighborAddr))
            {
                MC_WRITE32(RW_MC_B_LIST1, u4NeighborAddr);
            }
            else
            {
                MC_WRITE32(RW_MC_B_LIST1, MC_READ32(RW_MC_B_LIST1) & 0x0FFFFFFF);
            }
        }
        
        /*if(AVC_MV_READ32(RW_AMV_MV_BUF_ADDR + (u1RegIdx<<3)) == 0)
        {
            AVC_MV_WRITE32(RW_AMV_MV_BUF_ADDR + (u1RegIdx<<3), 
                AVC_MV_READ32(RW_AMV_MV_BUF_ADDR + (u1PrevIdx<<3)));
            AVC_MV_WRITE32(RW_AMV_MV_BUF_ADDR + (u1RegIdx<<3) + 4, 
                AVC_MV_READ32(RW_AMV_MV_BUF_ADDR + (u1PrevIdx<<3) + 4));
        }*/
    }

    //save last info for I frame error reconcealment
    u4P0Addr = MC_READ32(RW_MC_P_LIST0);
    u4B0Addr = MC_READ32(RW_MC_B_LIST0);
    u4B1Addr = MC_READ32(RW_MC_B_LIST1);
    u4MVAddr = AVC_MV_READ32(RW_AMV_MV_BUF_ADDR);
    u4P0Para = AVC_MV_READ32(RW_AMV_P_REF_PARA);
    u4B0Para = AVC_MV_READ32(RW_AMV_B0_REF_PARA);
    u4B1Para = AVC_MV_READ32(RW_AMV_B1_REF_PARA);
    UNUSED(u4RefCnt);
}



// *********************************************************************
// Function    : BOOL fgIsVDecComplete(void)
// Description : Check if VDec complete with interrupt
// Parameter   : None
// Return      : None
// *********************************************************************
/*static void _vSaveChkSum(void)
{
    UINT32 u4Temp;
    UINT32 i;
    
    //Checksum dump binary
    u4Temp = 0;
    u4DumpChksum[u4Temp++] = MC_READ32(0x5f4);
    u4DumpChksum[u4Temp++] = MC_READ32(0x5f8);
    u4DumpChksum[u4Temp++] = MC_READ32(0x608);
    u4DumpChksum[u4Temp++] = MC_READ32(0x60c);

    //MC  378~397  
    for(i=378; i<=397; i++)
    {
        u4DumpChksum[u4Temp++] = MC_READ32((UINT32)(i<<2));    
    }

    //AVC VLD  165~179
    for(i=165; i<=179; i++)
    {
        u4DumpChksum[u4Temp++] = AVC_VLD_READ32((UINT32)(i<<2));
    }

    //MV  147~151
    for(i=147; i<=151; i++)
    {
        u4DumpChksum[u4Temp++] = AVC_MV_READ32((UINT32)(i<<2));      
    }

    //IP  212    
    u4DumpChksum[u4Temp++] = AVC_MV_READ32((UINT32)(212 << 2));       
    
    //IQ  235~239
    for(i=241; i<=245; i++)
    {
        u4DumpChksum[u4Temp++] = AVC_MV_READ32((UINT32)(i<<2));                     
    }    

    //IS  241~245
    for(i=241; i<=245; i++)
    {
        u4DumpChksum[u4Temp++] = AVC_MV_READ32((UINT32)(i<<2));       
    }
    while(u4Temp < 80)
    {
        u4DumpChksum[u4Temp++] = 0;           
    }        
}*/

// *********************************************************************
// Function    : void vDumpReg()
// Description : Dump decoder registers when decoding error
// Parameter   None
// Return      : None
// *********************************************************************
void _VDEC_DumpReg(void)
{
    UINT32 u4Temp;
    UINT32 i,j;
        
    LOG(0, "AVC_VLD 274(157) Dec complete 0x%x\n", AVC_VLD_READ32(RO_AVLD_COMPLETE));
    LOG(0, "AVC_VLD 284(161) Error acc 0x%x\n", AVC_VLD_READ32(RO_AVLD_ERR_ACC) & ERR_MASK_OBSV);            
    LOG(0, "AVC_VLD 270(156) Error msg 0x%x\n", AVC_VLD_READ32(RO_AVLD_ERR_MESSAGE) & ERR_MASK_OBSV);
    LOG(0, "AVC_VLD 280(160) Error mask 0x%x\n", AVC_VLD_READ32(RW_AVLD_ERR_MASK));    
    u4Temp = AVC_VLD_READ32(RO_AVLD_SMKR);
    LOG(0, "AVC_VLD 250(148) 0x%x, x %d, y %d\n", u4Temp, u4Temp>>24, (u4Temp<<8)>>24);            
    LOG(0, "AVC_VLD 268(154) timeout threshold 0x%x\n", AVC_VLD_READ32(RW_AVLD_TIMEOUT_THD));
    LOG(0, "AVC_VLD 288(162) decode cycle 0x%x\n", AVC_VLD_READ32(RO_AVLD_DEC_CYCLE));
    LOG(0, "AVC_VLD 290(164) mc busy threshold 0x%x\n\n", AVC_VLD_READ32(RW_AVLD_MC_BUSY_THD));

    LOG(0, "VLD F8(62) Wp 0x%x\n", VLD_READ32(RO_VLD_VWPTR));
    LOG(0, "VLD FC(63) Rp 0x%x\n", VLD_READ32(RO_VLD_VRPTR));
    LOG(0, "VLD B4(45) Vstart 0x%x\n", VLD_READ32(RW_VLD_VSTART)<<6);
    LOG(0, "VLD B8(46) VEnd 0x%x\n\n", VLD_READ32(RW_VLD_VEND)<<6);
    LOG(0, "AVC_VLD 29C(167) Residual checksum 0x%x\n", AVC_VLD_READ32(RO_AVLD_RESISUAL_SUM));

    LOG(0, "AVC_VLD 2A0(168) Bs-in checksum 0x%x\n", AVC_VLD_READ32(RO_AVLD_BS_SUM));
    LOG(0, "AVC_VLD 2A4(169) 0x%x\n", AVC_VLD_READ32(0x2A4));    
    LOG(0, "AVC_VLD 2d0(180) ERC control 0x%x\n", AVC_VLD_READ32(RW_AVLD_ERC_CTRL));
    LOG(0, "AVC_VLD 248(146) slice hdr state 0x%x\n", AVC_VLD_READ32(RO_AVLD_SLICE_HDR_STATE));
    LOG(0, "AVC_VLD 254(149) slice data state 0x%x\n", AVC_VLD_READ32(RO_AVLD_SLICE_STATE));
    LOG(0, "AVC_VLD 258(150) mb state 0x%x\n", AVC_VLD_READ32(RO_AVLD_MB_STATE));
    LOG(0, "AVC_VLD 25C(151) mb pred state 0x%x\n", AVC_VLD_READ32(RO_AVLD_MB_PRED_STATE));
    LOG(0, "AVC_VLD 260(152) residual state 0x%x\n", AVC_VLD_READ32(RO_AVLD_RESIDUAL_STATE));
    LOG(0, "AVC_VLD 264(153) residual blk state 0x%x\n", AVC_VLD_READ32(RO_AVLD_RESIDUAL_BLK_STATE));            
    LOG(0, "AVC_VLD 24C(147) weighted pred state 0x%x\n", AVC_VLD_READ32(RO_AVLD_WEIGHTED_PRED_STATE));
    LOG(0, "MC 5c4(369) PP status 0x%x\n", MC_READ32(RO_MC_PP_STATUS));
    
    //Checksum log
    LOG(0, "<VLD Checksum>\n");
    LOG(0, "Reg Addr %d=0x%x\n", (RW_VLD_MB_STOP_POS>>2), VLD_READ32(RW_VLD_MB_STOP_POS));
    LOG(0, "Reg Addr %d=0x%x\n", (RO_VLD_CHKSUM_ALL>>2), VLD_READ32(RO_VLD_CHKSUM_ALL));
    LOG(0, "Reg Addr %d=0x%x\n", (RO_VLD_CHKSUM_JPG>>2), VLD_READ32(RO_VLD_CHKSUM_JPG));
    for(i = RO_VLD_CHKSUM_DCAC_OUT; i<=RO_VLD_CHKSUM_IS_DATA; i+=4)
    {
        LOG(0, "Reg Addr %d=0x%x\n", (i>>2), VLD_READ32(i));
    }

    x_thread_delay(100);

    LOG(0, "\n<AVC VLD Checksum>\n");
    LOG(0, "Reg Addr %d=0x%x\n", (RO_AVLD_CHKSUM_DRAMI>>2), AVC_VLD_READ32(RO_AVLD_CHKSUM_DRAMI));
    LOG(0, "Reg Addr %d=0x%x\n", (RO_AVLD_CHKSUM_DRAMO>>2), AVC_VLD_READ32(RO_AVLD_CHKSUM_DRAMO));
    LOG(0, "Reg Addr %d=0x%x\n", (RO_AVLD_CHKSUM_RESIDUAL>>2), AVC_VLD_READ32(RO_AVLD_CHKSUM_RESIDUAL));
    LOG(0, "Reg Addr %d=0x%x\n", (RO_AVLD_CHKSUM_BS>>2), AVC_VLD_READ32(RO_AVLD_CHKSUM_BS));
    LOG(0, "Reg Addr %d=0x%x\n", (RO_AVLD_CHKSUM_MIX1>>2), AVC_VLD_READ32(RO_AVLD_CHKSUM_MIX1));
    LOG(0, "Reg Addr %d=0x%x\n", (RO_AVLD_CHKSUM_MIX2>>2), AVC_VLD_READ32(RO_AVLD_CHKSUM_MIX2));
    LOG(0, "Reg Addr %d=0x%x\n", (RO_AVLD_CHKSUM_MIX3>>2), AVC_VLD_READ32(RO_AVLD_CHKSUM_MIX3));
    LOG(0, "Reg Addr %d=0x%x\n", (RO_AVLD_CHKSUM_MIX4>>2), AVC_VLD_READ32(RO_AVLD_CHKSUM_MIX4));
    LOG(0, "Reg Addr %d=0x%x\n", (RO_AVLD_CHKSUM_W_ADDR>>2), AVC_VLD_READ32(RO_AVLD_CHKSUM_W_ADDR));
    LOG(0, "Reg Addr %d=0x%x\n", (RO_AVLD_CHKSUM_R_ADDR>>2), AVC_VLD_READ32(RO_AVLD_CHKSUM_R_ADDR));
    LOG(0, "Reg Addr %d=0x%x\n", (RO_AVLD_CHKSUM_MIX_START>>2), AVC_VLD_READ32(RO_AVLD_CHKSUM_MIX_START));

    LOG(0, "\n<AVC MV Checksum>\n");
    for(i = RO_AMV_CHKSUM_TYPE; i<=RO_AMV_CHKSUM_DRAMO; i+=4)
    {
        LOG(0, "Reg Addr %d=0x%x\n", (i>>2), AVC_MV_READ32(i));
    }
    for(i = RO_AMV_CHKSUM_IS_BUSY_CNT; i<=RO_AMV_CHKSUM_IS_DATA; i+=4)
    {
        LOG(0, "Reg Addr %d=0x%x\n", (i>>2), AVC_MV_READ32(i));
    }
    for(i = RO_AMV_CHKSUM_QP; i<=RO_AMV_CHKSUM_SCL_DATA; i+=4)
    {
        LOG(0, "Reg Addr %d=0x%x\n", (i>>2), AVC_MV_READ32(i));
    }
    for(i = RO_AMV_CHKSUM_ACC_CBP; i<=RO_AMV_CHKSUM_BLK_C_AVAL; i+=4)
    {
        LOG(0, "Reg Addr %d=0x%x\n", (i>>2), AVC_MV_READ32(i));
    }

    LOG(0, "\n<MC Checksum>\n");
    for(i = RO_MC_CHKSUM_IDCT_Y0Y1; i<=RO_MC_CHKSUM_DS_MIX4; i+=4)
    {
        LOG(0, "Reg Addr %d=0x%x\n", (i>>2), MC_READ32(i));
    }
    x_thread_delay(100);
    
    LOG(0, "<Scaling list>\n");    
    for(j=0; j<8; j++)
    {
        if(j < 6)
        {
            u4Temp = (j << 4);
            for(i=0; i<4; i++)
            {
                // add 16 for every list
                VLD_WRITE32(RW_VLD_SCL_ADDR, 0x100 + u4Temp + (i << 2));
                LOG(0, "%4d [0x%.8x] 0x%.8x, \n", 0x100 + u4Temp + (i << 2), 
                    0x100 + u4Temp + (i << 2), VLD_READ32(RW_VLD_SCL_DATA));
            }
            LOG(0, "\n");      
        }
        else
        {
            u4Temp = (j == 6)? (j << 4) : ((j + 3) << 4); // 6=>16*6   7=>16*6+64(equal to 16*7+48)
            for(i=0; i<16; i++)
            {
                // add 64 for every list
                VLD_WRITE32(RW_VLD_SCL_ADDR, 0x100 + u4Temp + (i << 2));
                LOG(0, "%4d [0x%.8x] 0x%.8x, \n", 0x100 + u4Temp + (i << 2), 
                    0x100 + u4Temp + (i << 2), VLD_READ32(RW_VLD_SCL_DATA));
            }
            LOG(0, "\n");     
        }
    }
    x_thread_delay(100);

    LOG(0, "<AVC VLD setting>\n");     

    u4Temp = 33;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 34;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 35;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 36;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 37;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 38;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 42;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 74;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 106;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 39;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));

    LOG(0, "== Deblocking \n");
    u4Temp = 136;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 137;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 138;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 148;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 142;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 152;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 153;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 139;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));

    LOG(0, "== Motion Compensation \n");  
    u4Temp = 130;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 131;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 343;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 344;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));

    u4Temp = 345;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 346;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 347;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));

    LOG(0, "==> Debug recording (MC)\n");
    u4Temp = 10;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 11;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 37;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 27;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 245;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 354;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 355;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 356;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 357;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 358;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 359;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 360;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 361;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 362;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    u4Temp = 363;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));

    for(i=378;i<=393;i++)
    {
        u4Temp = i;
        LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    }
    x_thread_delay(100);
    
    LOG(0, "==> Debug recording (PP)\n");
    for(i=369;i<=374;i++)
    {
        u4Temp = i;
        LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, MC_BASE+(u4Temp << 2), MC_READ32((u4Temp << 2)));
    }

    x_thread_delay(100);

    LOG(0, "==> Debug recording (VLD)\n");
    u4Temp = 44;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, VLD_BASE+(u4Temp << 2), VLD_READ32((u4Temp << 2)));
    u4Temp = 45;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, VLD_BASE+(u4Temp << 2), VLD_READ32((u4Temp << 2)));  
    u4Temp = 46;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, VLD_BASE+(u4Temp << 2), VLD_READ32((u4Temp << 2)));  
    u4Temp = 59;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, VLD_BASE+(u4Temp << 2), VLD_READ32((u4Temp << 2)));
    u4Temp = 62;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, VLD_BASE+(u4Temp << 2), VLD_READ32((u4Temp << 2)));
    u4Temp = 63;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, VLD_BASE+(u4Temp << 2), VLD_READ32((u4Temp << 2)));
    u4Temp = 68;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, VLD_BASE+(u4Temp << 2), VLD_READ32((u4Temp << 2)));

    LOG(0, "==> Debug recording (AVC_VLD)\n");  
    u4Temp = 146;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 147;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 148;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 149;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 150;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 151;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 152;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 153;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 156;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 157;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
    u4Temp = 158;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));

    u4Temp = 165;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));

    u4Temp = 166;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));

    u4Temp = 167;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));

    u4Temp = 168;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));

    u4Temp = 169;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));

    u4Temp = 174;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));

    u4Temp = 175;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));

    u4Temp = 176;
    LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_VLD_BASE+(u4Temp << 2), AVC_VLD_READ32((u4Temp << 2)));
  
    LOG(0, "==> Debug recording (AVC_MV)\n");
    for(i=140;i <= 151;i++)
    {
        u4Temp = i;
        LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_MV_BASE+(u4Temp << 2), AVC_MV_READ32((u4Temp << 2)));
    }

    x_thread_delay(100);

    LOG(0, "==> Debug recording (AVC_IP)\n");
    for(i=208;i <= 223;i++)
    {
        u4Temp = i;
        LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_MV_BASE+(u4Temp << 2), AVC_MV_READ32((u4Temp << 2)));
    }

    LOG(0, "==> Debug recording (AVC_IT)\n");
    for(i=224;i <= 231;i++)
    {
        u4Temp = i;
        LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_MV_BASE+(u4Temp << 2), AVC_MV_READ32((u4Temp << 2)));
    }

    LOG(0, "==> Debug recording (AVC_IQ)\n");
    for(i=232;i <= 239;i++)
    {
        u4Temp = i;
        LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_MV_BASE+(u4Temp << 2), AVC_MV_READ32((u4Temp << 2)));
    }

    LOG(0, "==> Debug recording (AVC_IS)\n");
    for(i=240;i <= 255;i++)
    {
        u4Temp = i;
        LOG(0, "%d (0x%.4x) = 0x%.8x\n", u4Temp, AVC_MV_BASE+(u4Temp << 2), AVC_MV_READ32((u4Temp << 2)));
    }

    x_thread_delay(100);

    //MC p_list0 setting
    LOG(0, "// MC p_list0 setting \n");
    for(i=0;i<32;i++)
    {
        LOG(0, "RISCWrite(%d, 32'h%.8x);\n", ((RW_MC_P_LIST0 >>2) + i), MC_READ32(RW_MC_P_LIST0  + (4*i)));
    }
    LOG(0, "\n");
    //MC b_list0 setting
    LOG(0, "// MC b_list0 setting \n");  
    for(i=0;i<32;i++)
    {
        LOG(0, "RISCWrite(%d, 32'h%.8x);\n", ((RW_MC_B_LIST0 >>2) + i), MC_READ32(RW_MC_B_LIST0  + (4*i)));
    }
    LOG(0, "\n");
    //MC b_list1 setting
    LOG(0, "// MC b_list1 setting \n");    
    for(i=0;i<32;i++)
    {
        LOG(0, "RISCWrite(%d, 32'h%.8x);\n", ((RW_MC_B_LIST1 >>2) + i), MC_READ32(RW_MC_B_LIST1  + (4*i)));
    }

    LOG(0, "\n");
    //MV p_list0 setting
    LOG(0, "// MV p_list0 setting \n");    
    for(i=0;i<32;i++)
    {
        LOG(0, "RISCWrite_MV(%d, 32'h%.8x);\n", ((RW_AMV_P_REF_PARA/* >>2*/) + i),
            AVC_MV_READ32(RW_AMV_P_REF_PARA  + (4*i)));
    }
    LOG(0, "\n");

    x_thread_delay(100);
    
    //MV b_list0 setting
    LOG(0, "// MV b_list0 setting \n");    
    for(i=0;i<32;i++)
    {
        LOG(0, "RISCWrite_MV(%d, 32'h%.8x);\n", ((RW_AMV_B0_REF_PARA >>2) + i),
            AVC_MV_READ32(RW_AMV_B0_REF_PARA  + (4*i)));
    }
  
    LOG(0, "\n");
    //MV b_list1 setting
    LOG(0, "// MV b_list1 setting \n");    
    for(i=0;i<32;i++)
    {
        LOG(0, "RISCWrite_MV(%d, 32'h%.8x);\n", ((RW_AMV_B1_REF_PARA >>2) + i),
            AVC_MV_READ32(RW_AMV_B1_REF_PARA  + (4*i)));
    }
  
    LOG(0, "\n");
    //MV drt_adr setting
    LOG(0, "// MV drt_adr setting \n");    
    for(i=0;i<32;i++)
    {
        LOG(0, "RISCWrite_MV(%d, 32'h%.8x);\n", ((RW_AMV_MV_BUF_ADDR >>2) + i),
            AVC_MV_READ32(RW_AMV_MV_BUF_ADDR  + (4*i)));
    }
    LOG(0, "\n");
    LOG(0, "// MV POC setting \n");    
    LOG(0, "RISCWrite_MV(%d, 32'h%.8x);\n", (RW_AMV_CURR_POC >>2),AVC_MV_READ32(RW_AMV_CURR_POC ));
    LOG(0, "RISCWrite_MV(%d, 32'h%.8x);\n", (RW_AMV_CURR_TFLD_POC >>2),
        AVC_MV_READ32(RW_AMV_CURR_TFLD_POC ));
    LOG(0, "RISCWrite_MV(%d, 32'h%.8x);\n", (RW_AMV_CURR_BFLD_POC >>2),
        AVC_MV_READ32(RW_AMV_CURR_BFLD_POC ));

    x_thread_delay(100);
    // end : write out test bench for FPGA RTL simulation  
}



//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

// **************************************************************************
// Function : void vInitVDecHW(DWRD dwRDPtrAddr);
// Description : Init Video Decoder
// Parameter : None
// Return      : TRUE: init OK.
//                   FALSE: init failed
// **************************************************************************
void _VDEC_InitVDecHW(BOOL bIsInit)
{
  //DWRD u4Temp;
 
    //vIrqOff(0xffffffff);
    #ifndef AVC_IRQ_DISABLE
    x_os_isr_fct pfnOldIsr;        
    if(_hVdecFinSema == (HANDLE_T)NULL)
    {
        VERIFY(x_sema_create(&_hVdecFinSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
    }
    // reg ISR        
    if (x_reg_isr(AVLD_VECTOR, _AVLDIsr, &pfnOldIsr) != OSR_OK)
    {
        ASSERT(0);
    }
    //vIrqOn(INT_VLD);
    #endif

    if(bIsInit)
    {
        _VDEC_VLDHWReset(bIsInit);        
        #ifdef BARREL2_SUPPORT
        _VDEC_VLDHWReset2(bIsInit);
        #endif
    }
}

// *********************************************************************
// Function : void _VDEC_SetBsInfo
// Description : Set Bs fifo start addr, end addr, and current read ptr
// Parameter : u4RDPtrAddr, u4BsFifoS, u4BsFifoE
// Return    : None
// *********************************************************************
void _VDEC_SetBsInfo(UINT8 u1Num, UINT32 u4RDPtrAddr, UINT32 u4WPtrAddr,
    UINT32 u4BsFifoStart, UINT32 u4BsFifoEnd, UINT32 u4PredSA, UINT8 u1ECCMode, UINT32 u4ECType)
{    
    #ifdef WAIT_SRAM_STABLE
    UINT32 u4Cnt = 0;
    #endif
    
    // Set for VLD boundary check 0x5000000
    #ifdef BARREL2_SUPPORT
    if(u1Num == BARREL2)
    {
        _VDEC_VLDHWReset2(FALSE);
        while(!_VDEC_ResetVLD2(PHYSICAL(u4RDPtrAddr), PHYSICAL(u4WPtrAddr), 
            PHYSICAL(u4BsFifoStart), PHYSICAL(u4BsFifoEnd)))
        {
            _VDEC_VLDHWReset2(TRUE);
        }
    }
    else
    #else
    UNUSED(u1Num);
    #endif
    {
        #ifdef WAIT_SRAM_STABLE
        _VDEC_VLDHWReset(FALSE);
        while(!_VDEC_ResetVLD1(PHYSICAL(u4RDPtrAddr), PHYSICAL(u4WPtrAddr),
            PHYSICAL(u4BsFifoStart), PHYSICAL(u4BsFifoEnd), PHYSICAL(u4PredSA), u1ECCMode, u4ECType))
        {
            LOG(2, "SwRst %d\n", u4Cnt++);
            _VDEC_VLDHWReset(TRUE);
        }
        UNUSED(u4Cnt);
        #else
        _VDEC_VLDSetVFifo(PHYSICAL(u4BsFifoStart), PHYSICAL(u4BsFifoEnd));
    
        if(!_VDEC_VLDInitBarrelShifter(u4RDPtrAddr))
        {
            LOG(2, "Barrel Shifter1 Init failed");
        }
        #endif
    }
}


void _VDEC_ClrFBufInfo(VDEC_AVC_DecPrm *prDecPrm, UINT32 u4FBufIdx)
{
    VDEC_FBUF_INFO_T* prFBufInfo;
    
    ASSERT(prDecPrm!=NULL);
    ASSERT(u4FBufIdx < 18);

    if(u4FBufIdx >= 18)
    {
        return;
    }
    prFBufInfo = &prDecPrm->arFBufInfo[u4FBufIdx];
        
    prFBufInfo->fgNonExisting = FALSE;    
    prFBufInfo->fgIsBufRef = FALSE;
    prFBufInfo->fgIsErr = FALSE;
    prFBufInfo->fgIsNotDisp = FALSE;
    prFBufInfo->fgFirstFrmTag = FALSE;
    
    prFBufInfo->u1FBufStatus = NO_PIC;    
    
    _vClrPicRefInfo(prDecPrm, FRAME, (UINT8)u4FBufIdx);
    //prFBufInfo->u1BFldRefType = NREF_PIC;    
    //prFBufInfo->u1FBufRefType = NREF_PIC;
    //prFBufInfo->u1TFldRefType = NREF_PIC;
  
    prFBufInfo->u4FrameNum = (UINT32)0xffffffff;
    prFBufInfo->i4FrameNumWrap = (INT32)0xefffffff;
    prFBufInfo->i4PicNum = (INT32)0xefffffff;
    prFBufInfo->i4TFldPicNum = (INT32)0xefffffff;
    prFBufInfo->i4BFldPicNum = (INT32)0xefffffff;
    prFBufInfo->u4LongTermFrameIdx = (UINT32)0xff;
    prFBufInfo->u4TFldLongTermFrameIdx = (UINT32)0xff;
    prFBufInfo->u4BFldLongTermFrameIdx = (UINT32)0xff;
    prFBufInfo->i4LongTermPicNum = (INT32)0xefffffff;
    prFBufInfo->i4TFldLongTermPicNum =(INT32) 0xefffffff;
    prFBufInfo->i4BFldLongTermPicNum = (INT32)0xefffffff;    

    prFBufInfo->u4TFldPara = 0;
    prFBufInfo->u4BFldPara = 0;
  
    prFBufInfo->i4POC = (INT32)0x7fffffff;
    prFBufInfo->i4TFldPOC = (INT32)0x7fffffff;
    prFBufInfo->i4BFldPOC = (INT32)0x7fffffff;

    prFBufInfo->u4UserDataFrmWptr = 0;
    prFBufInfo->u4UserDataPESWptr = 0;
    prFBufInfo->u4SkipFrmCnt = 0;
    #ifdef FBM_DPB_CTRL
    FBM_UpdateReferenceList(prDecPrm->ucFbgId, prFBufInfo->ucFbId, FALSE);
    #endif
}


// *********************************************************************
// Function    : void vFlushDPB(tMpvDecPrm)
// Description : Flush pictures in DPB
// Parameter   : None
// Return      : None
// *********************************************************************
void _VDEC_FlushDPB(VDEC_AVC_DecPrm *prDecPrm)
{
    UINT32 i, FbCnt;
    INT32 i4MinPOC;
    UINT32 u4MinPOCFBufIdx, u4YAddr, u4CAddr, u4SaveInfoIdx = 0xff;
    //VDEC_DISP_PIC_INFO_T rDispInfo;
    DRV_DBASE_T* _prDbase = NULL;
    CRIT_STATE_T _rCritState;
    PARAM_MPV_T* prInfo;
    STC_CLOCK rStcClk;    
    #ifdef FBM_DPB_CTRL
    FBM_PIC_HDR_T * prFbmPicHdr;
    #endif
    VDEC_ES_INFO_T* prVdecEsInfo = _VDEC_GetEsInfo(ES0);
    MPV_EXT_INFO_T *prExtInfo;
    MPV_USERDATA_CB_T rUsrData;
    
    prExtInfo = (MPV_EXT_INFO_T*)_MPV_GetExtInfo();

    _prDbase = DBS_Lock(&_rCritState);
    ASSERT(_prDbase);
    VERIFY(DBS_Unlock(_prDbase, _rCritState));        
    prInfo = &(_prDbase->rMpv[0]);

    ASSERT(prDecPrm!=NULL);

    //do
    for(FbCnt=0; FbCnt<prDecPrm->u4MaxFBufNum; FbCnt++)
    {
        // Check if DPB full
        i4MinPOC = 0x7fffffff;
        u4MinPOCFBufIdx = 0xff;
        for(i=0; i<prDecPrm->u4MaxFBufNum; i++)
        {
            if((i4MinPOC > prDecPrm->arFBufInfo[i].i4POC) && (prDecPrm->arFBufInfo[i].u1FBufStatus != NO_PIC) &&
                (i != u4SaveInfoIdx))
            {
                i4MinPOC = prDecPrm->arFBufInfo[i].i4POC;
                u4MinPOCFBufIdx = i;
            }
        }
        if(u4MinPOCFBufIdx != 0xff)
        {
            #ifdef FBM_DPB_CTRL
            //calculate pts in decoder if AVI file
            prFbmPicHdr = FBM_GetFrameBufferPicHdr(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId);
            if(prFbmPicHdr == NULL)
            {
                LOG(2, "prFbmPicHdr NULL in FlushDpb\n");
                break;
            }

            
            if(FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId) 
                == FBM_FB_STATUS_READY)
            {

                if(prDecPrm->fgIsMMPlay)
                {
                    if(!prDecPrm->arFBufInfo[u4MinPOCFBufIdx].fgPTSValid)
                    {
                        prFbmPicHdr->u4PTS = prDecPrm->u4DispPts;
                    }
                    else
                    {
                        prDecPrm->u4DispPts = prFbmPicHdr->u4PTS;
                    }
                    if(prDecPrm->u4Rate > 0)
                    {
                        prDecPrm->u4DispPts += ((UINT32)u8Div6432((prDecPrm->u8PtsScale + prDecPrm->u8VRemainder), 
                            prDecPrm->u4Rate, &prDecPrm->u8VRemainder) * (prDecPrm->arFBufInfo[u4MinPOCFBufIdx].u4SkipFrmCnt + 1));
                    }
                }
                             
                FBM_UpdateReferenceList(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId, FALSE);
                #ifdef VDP_RACE_LOG
                VDP_FrcRacingLog('Q');
                VDP_FrcRacingLog('0' + prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId);
                #endif

                if((prDecPrm->arFBufInfo[u4MinPOCFBufIdx].u1FBufStatus != FRAME) ||
                    (prDecPrm->arFBufInfo[u4MinPOCFBufIdx].fgIsNotDisp))  //field empty
                {
                    FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId, 
                        FBM_FB_STATUS_EMPTY);
                    prInfo->u4VpsrDropNs++;
                }
                else
                {
                    if (STC_GetSrc(0, &rStcClk) == STC_VALID)
                    {
                        LOG(6, "FlushQ PTS(0x%x), STC(0x%x), POC %d\n", 
                            prFbmPicHdr->u4PTS, rStcClk.u4Base, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].i4POC);
                    }
                    
                    if(!prDecPrm->fgStartPlay)
                    {
                        if(prDecPrm->fgIsMMPlay)
                        {
                            prDecPrm->fgStartPlay = TRUE;
                        }
                        else
                        {
                            if(_bNotifyAudStartPlay(prDecPrm->ucStcId, prFbmPicHdr->u4PTS))
                            {
                                prDecPrm->fgStartPlay = TRUE;
                            }
                        }
                    }
                    // mm checking
                    if(prDecPrm->fgIsMMPlay && VDEC_IsNotDisplay(ES0, prFbmPicHdr->u4PTS,
                        prFbmPicHdr->i4TemporalRef, prFbmPicHdr->u8Offset))
                    {
                        FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId, 
                            FBM_FB_STATUS_EMPTY);            
                    }
                    else
                    {
                        FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId, 
                            FBM_FB_STATUS_DISPLAYQ);
                        prVdecEsInfo->u4DisplayQPicCnt++;
                        
                        if(prDecPrm->arFBufInfo[u4MinPOCFBufIdx].fgFirstFrmTag)   //continuous display from 1st frame
                        {
                            for(i=0; i<prDecPrm->u4MaxFBufNum; i++)
                            {
                                if(prDecPrm->arFBufInfo[i].u1FBufStatus != NO_PIC)
                                {
                                    prDecPrm->arFBufInfo[i].fgIsNotDisp = FALSE;
                                }
                            }
                        }
                        prDecPrm->arFBufInfo[u4MinPOCFBufIdx].fgFirstFrmTag = FALSE;

                        //notify userdata
                        if(prExtInfo->pfUserDataCb &&
                            ((prExtInfo->fgUserData[ES0] && (prDecPrm->arFBufInfo[u4MinPOCFBufIdx].u4UserDataFrmWptr > 0)) ||
                            (prExtInfo->fgPesUserData[ES0] && (prDecPrm->arFBufInfo[u4MinPOCFBufIdx].u4UserDataPESWptr > 0))))
                        {
                            rUsrData.fgProgressiveFrm = prFbmPicHdr->fgProgressiveFrm;
                            rUsrData.u2TemporalRef = (UINT16)prFbmPicHdr->i4TemporalRef;
                            rUsrData.u4Pts = prFbmPicHdr->u4PTS;
                            rUsrData.ucEsId = ES0;
                            rUsrData.ucPicStruct = prFbmPicHdr->ucPicStruct;
                            rUsrData.ucPicType = prFbmPicHdr->ucPicCdTp;
                            rUsrData.ucRepFirstFld = prFbmPicHdr->fgRepFirstFld;
                            rUsrData.ucTopFldFirst = prFbmPicHdr->fgTopFldFirst;
                            
                            #ifdef CC_SEI_USER_DATA_ENABLE
                            if(prExtInfo->fgUserData[ES0] && (prDecPrm->arFBufInfo[u4MinPOCFBufIdx].u4UserDataFrmWptr > 0))
                            {
                                rUsrData.u4Rptr = _VDEC_SaveUserData(prDecPrm->arFBufInfo[u4MinPOCFBufIdx].au1UsrDataFrm, 
                                prDecPrm->arFBufInfo[u4MinPOCFBufIdx].u4UserDataFrmWptr);
                                rUsrData.ucDataLen = (UCHAR)prDecPrm->arFBufInfo[u4MinPOCFBufIdx].u4UserDataFrmWptr;                                
                                prExtInfo->pfUserDataCb(&rUsrData);
                            }
                            #endif
                            #ifdef CC_DMX_PES_EXT_BUFFER
                            if(prExtInfo->fgPesUserData[ES0] && (prDecPrm->arFBufInfo[u4MinPOCFBufIdx].u4UserDataPESWptr > 0))
                            {
                                rUsrData.u4Rptr = _VDEC_SaveUserData(prDecPrm->arFBufInfo[u4MinPOCFBufIdx].au1UsrDataPES, 
                                prDecPrm->arFBufInfo[u4MinPOCFBufIdx].u4UserDataPESWptr);
                                rUsrData.ucDataLen = (UCHAR)prDecPrm->arFBufInfo[u4MinPOCFBufIdx].u4UserDataPESWptr;                            
                                prExtInfo->pfUserDataCb(&rUsrData);
                            }
                            #endif
                            UNUSED(rUsrData);
                        }
                    }
                    if(prDecPrm->fgIsMMPlay)
                    {
                        prVdecEsInfo->u4PTS = prFbmPicHdr->u4PTS;
                        UNUSED(VDEC_CheckStopStatus(ES0, prFbmPicHdr->u4PTS, prFbmPicHdr->i4TemporalRef));
                    }
                    prDecPrm->i4LastPOC = i4MinPOC;
                }
                FBM_GetFrameBufferAddr(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId,
                &u4YAddr, &u4CAddr);
            }
            else if(FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId) 
                == FBM_FB_STATUS_DECODE)
            {
                //if((prDecPrm->arFBufInfo[u4MinPOCFBufIdx].u1FBufStatus != FRAME))
                {
                    //FBM_UpdateReferenceList(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId, FALSE);
                    FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId, 
                        FBM_FB_STATUS_EMPTY);
                    prInfo->u4VpsrDropNs++;
                }
                //else
                //{
                //    u4SaveInfoIdx = u4MinPOCFBufIdx;
                //    LOG(2, "SaveInfo in FlushDPB\n");
                //}
            }
            #endif            
            _VDEC_ClrFBufInfo(prDecPrm, u4MinPOCFBufIdx);
        }
    }
    //while(u4MinPOCFBufIdx != 0xff);
}


void _VDEC_ClrDPB(VDEC_AVC_DecPrm *prDecPrm)
{
    UINT32 FbCnt;
    UCHAR ucFbStatus;

    ASSERT(prDecPrm!=NULL);
    
    //do
    ASSERT(prDecPrm->u4MaxFBufNum <= 18);
    for(FbCnt = 0; (FbCnt < prDecPrm->u4MaxFBufNum) && (FbCnt < 18); FbCnt++)
    {
        #ifdef FBM_DPB_CTRL    
        ucFbStatus = FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[FbCnt].ucFbId);
        if((ucFbStatus == FBM_FB_STATUS_READY) || (ucFbStatus == FBM_FB_STATUS_DECODE))
        {
            FBM_UpdateReferenceList(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[FbCnt].ucFbId, FALSE);

            FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[FbCnt].ucFbId, 
            FBM_FB_STATUS_EMPTY);
            _VDEC_ClrFBufInfo(prDecPrm, FbCnt);
        }
        #endif            
    }
    prDecPrm->i4LastPOC = -99999;
    LOG(2, "ClrDPB done.\n");
}


void _VDEC_SetPicInfo(VDEC_AVC_DecPrm* prDecPrm)
{
    UINT32 u4SPSInfo, u4PPSInfo1, u4PPSInfo2, u4SHDRInfo, u4Idx;
    
    ASSERT(prDecPrm!=NULL);
    
    //prepare reference picture list
    _vPrepareFBufInfo(prDecPrm);
    if(prDecPrm->fgAllocFbFail || prDecPrm->fgNotSupport)
    {
        return;
    }
    _vSetPRefPicList(prDecPrm);
    _vSetBRefPicList(prDecPrm);

    _vAssignQuantParam(prDecPrm);
    //SPS data prepare ===========================================
    u4SPSInfo = (prDecPrm->prSPS->u4ChromaFormatIdc & 0x3); // 1~0
    u4SPSInfo |= ((prDecPrm->prSPS->u4Log2MaxFrameNumMinus4 & 0xf)<< 2); //5~2
    u4SPSInfo |= ((prDecPrm->prSPS->u4PicOrderCntType & 0x3) << 6); //7~6
    u4SPSInfo |= ((prDecPrm->prSPS->u4Log2MaxPicOrderCntLsbMinus4 & 0xf) << 8);
    u4SPSInfo |= (prDecPrm->prSPS->fgDeltaPicOrderAlwaysZeroFlag << 12);
    u4SPSInfo |= ((prDecPrm->prSPS->u4NumRefFrames & 0x1f) << 13);
    u4SPSInfo |= (prDecPrm->prSPS->fgFrameMbsOnlyFlag << 18);
    u4SPSInfo |= (prDecPrm->prSPS->fgMbAdaptiveFrameFieldFlag << 19);  
    u4SPSInfo |= (prDecPrm->prSPS->fgDirect8x8InferenceFlag << 20);    
    u4SPSInfo |= (1 << 21);
    
    _VDEC_SetSPSAVLD(u4SPSInfo, prDecPrm->prSPS->u4PicWidthInMbsMinus1, 
        prDecPrm->prSPS->u4PicHeightInMapUnitsMinus1);

    //PPS data prepare ============================================
    u4PPSInfo1 = prDecPrm->prPPS->fgEntropyCodingModeFlag;
    u4PPSInfo1 |= (prDecPrm->prPPS->fgPicOrderPresentFlag << 1);
    u4PPSInfo1 |= (prDecPrm->prPPS->fgWeightedPredFlag << 2);
    u4PPSInfo1 |= ((prDecPrm->prPPS->u4WeightedBipredIdc & 0x03) << 3);  
    u4PPSInfo1 |= ((prDecPrm->prPPS->i4PicInitQpMinus26 & 0x3f) << 5);  
    u4PPSInfo1 |= ((prDecPrm->prPPS->i4ChromaQpIndexOffset & 0x1f)<< 11);    
    u4PPSInfo1 |= (prDecPrm->prPPS->fgDeblockingFilterControlPresentFlag << 16);
    u4PPSInfo1 |= (prDecPrm->prPPS->fgConstrainedIntraPredFlag << 17);
    u4PPSInfo1 |= (prDecPrm->prPPS->fgTransform8x8ModeFlag << 18);  
    u4PPSInfo1 |= ((prDecPrm->prPPS->i4SecondChromaQpIndexOffset & 0x1f) << 19);

    u4PPSInfo2 = prDecPrm->prPPS->u4NumRefIdxL0ActiveMinus1;
    u4PPSInfo2 |= (prDecPrm->prPPS->u4NumRefIdxL1ActiveMinus1 << 5);
    for(u4Idx = 0; u4Idx < 8; u4Idx++)
    {
        u4PPSInfo2 |= (prDecPrm->afgUserScalingListPresentFlag[u4Idx] << (10 + u4Idx));  
    }
    u4PPSInfo2 |= (prDecPrm->fgUserScalingMatrixPresentFlag << 18);
    //for(i=0; i<8; i++)
    {
        //dwPPSInfo |= (tMpvDecPrm->tPPS->fgPicScalingListPresentFlag[i] << (10 + i));  
    }
    //dwPPSInfo |= (tMpvDecPrm->tPPS->fgPicScalingMatrixPresentFlag << 18);  
    
    _VDEC_SetPPSAVLD(u4PPSInfo1, u4PPSInfo2);

    //slice header info ===============================================
    u4SHDRInfo = prDecPrm->prSliceHdr->u4FirstMbInSlice & 0x1fff;
    u4SHDRInfo |= ((prDecPrm->prSliceHdr->u4SliceType & 0xf) << 13);
    u4SHDRInfo |= (prDecPrm->prSliceHdr->fgFieldPicFlag << 17);
    u4SHDRInfo |= (prDecPrm->prSliceHdr->fgBottomFieldFlag << 18);
    u4SHDRInfo |= (prDecPrm->prSliceHdr->fgDirectSpatialMvPredFlag << 19);
    u4SHDRInfo |= ((prDecPrm->prSliceHdr->u4NumRefIdxL0ActiveMinus1 & 0x1f) << 20);
    u4SHDRInfo |= ((prDecPrm->prSliceHdr->u4NumRefIdxL1ActiveMinus1 & 0x1f) << 25);
    _VDEC_SetSHDRAVLD1(u4SHDRInfo);     

}

void _VDEC_RefPicListReordering()
{
    _VDEC_AVCReordering();    //hw function
}

// *********************************************************************
// Function    : void vVDecSetCurrPOC(void)
// Description : Set POC : Picture order count for display order
// Parameter   : None
// Return      : None
// Note  :JM decode_POC
// *********************************************************************
void _VDEC_SetCurrPOC(VDEC_AVC_DecPrm *prDecPrm)
{
    INT32 i4PrevPOCMsb;
    INT32 i4PrevPOCLsb;    
    INT32 i4MaxPicOrderCntLsb;
    INT32 i4PrevFrameNumOffset;
    INT32 i4AbsFrameNum;
    INT32 i4PicOrderCntCycleCnt = 0;
    INT32 i4FrameNumInPicOrderCntCycle = 0;
    INT32 i4ExpectedDeltaPerPicOrderCnt;
    INT32 i4ExpectedDeltaPerPicOrderCntCycle;
    INT32 i;
    INT32 i4Temp;

    ASSERT(prDecPrm!=NULL);
    i4MaxPicOrderCntLsb = (INT32)(1 << (prDecPrm->prSPS->u4Log2MaxPicOrderCntLsbMinus4 + 4));
    //ptSliceHdr = &_tSliceHdr;
  
    switch(prDecPrm->prSPS->u4PicOrderCntType)
    {
        case 0:
            if(fgIsIDRPic(prDecPrm->u1NalUnitType))
            {
                i4PrevPOCMsb = 0;
                i4PrevPOCLsb = 0;        
            }
            else
            {
                if(prDecPrm->rLastInfo.fgLastMmco5)
                {
                    if(prDecPrm->rLastInfo.u1LastPicStruct != BOTTOM_FIELD)
                    {
                        i4PrevPOCMsb = 0;
                        i4PrevPOCLsb = prDecPrm->rLastInfo.i4LastRefTFldPOC;        
                    }
                    else
                    {
                        i4PrevPOCMsb = 0;
                        i4PrevPOCLsb = 0;        
                    }
                }
                else
                {
                    i4PrevPOCMsb = prDecPrm->rLastInfo.i4LastRefPOCMsb;        
                    i4PrevPOCLsb = prDecPrm->rLastInfo.i4LastRefPOCLsb;        
                }
            }

            // Calculate POCMsb
            if((prDecPrm->prSliceHdr->i4PicOrderCntLsb < i4PrevPOCLsb) && 
            ((i4PrevPOCLsb - prDecPrm->prSliceHdr->i4PicOrderCntLsb) >= (i4MaxPicOrderCntLsb /2)))
            {
                prDecPrm->prSliceHdr->i4PicOrderCntMsb = i4PrevPOCMsb + i4MaxPicOrderCntLsb;
            }
            else if((prDecPrm->prSliceHdr->i4PicOrderCntLsb > i4PrevPOCLsb) && 
            ((prDecPrm->prSliceHdr->i4PicOrderCntLsb - i4PrevPOCLsb) > (i4MaxPicOrderCntLsb /2)))
            {
                prDecPrm->prSliceHdr->i4PicOrderCntMsb = i4PrevPOCMsb - i4MaxPicOrderCntLsb;
            }
            else
            {
                prDecPrm->prSliceHdr->i4PicOrderCntMsb = i4PrevPOCMsb;
            }

            if((!prDecPrm->prSliceHdr->fgFieldPicFlag) || (!prDecPrm->prSliceHdr->fgBottomFieldFlag))
            {
                prDecPrm->prCurrFBufInfo->i4TFldPOC = prDecPrm->prSliceHdr->i4PicOrderCntMsb + 
                                                    prDecPrm->prSliceHdr->i4PicOrderCntLsb;
            }

            if((!prDecPrm->prSliceHdr->fgFieldPicFlag))
            {
                prDecPrm->prCurrFBufInfo->i4BFldPOC = prDecPrm->prCurrFBufInfo->i4TFldPOC + 
                                                    prDecPrm->prSliceHdr->i4DeltaPicOrderCntBottom;
            }
            else if(prDecPrm->prSliceHdr->fgBottomFieldFlag)
            {
                prDecPrm->prCurrFBufInfo->i4BFldPOC = prDecPrm->prSliceHdr->i4PicOrderCntMsb + 
                                                    prDecPrm->prSliceHdr->i4PicOrderCntLsb;
            }
            break;
            
        case 1:
            if(prDecPrm->rLastInfo.fgLastMmco5)
            {
                i4PrevFrameNumOffset = 0;
            }
            else
            {
                i4PrevFrameNumOffset = prDecPrm->rLastInfo.i4LastFrameNumOffset;
            }
            if(fgIsIDRPic(prDecPrm->u1NalUnitType))
            {
                prDecPrm->prSliceHdr->ai4DeltaPicOrderCnt[0] = 0;
                prDecPrm->i4FrmNumOffset = 0;
            }
            else if(prDecPrm->rLastInfo.u4LastFrameNum > prDecPrm->prSliceHdr->u4FrameNum)
            {
                prDecPrm->i4FrmNumOffset = i4PrevFrameNumOffset + (INT32)prDecPrm->prSPS->u4MaxFrameNum;
            }
            else
            {
                prDecPrm->i4FrmNumOffset = i4PrevFrameNumOffset;
            }

            if(prDecPrm->prSPS->u4NumRefFramesInPicOrderCntCycle != 0)
            {
                i4AbsFrameNum = prDecPrm->i4FrmNumOffset + (INT32)prDecPrm->prSliceHdr->u4FrameNum;
            }
            else
            {
                i4AbsFrameNum = 0; 
            }
      
            if((prDecPrm->u1NalRefIdc == 0) && (i4AbsFrameNum > 0))
            {
                i4AbsFrameNum --;
            }

            if(i4AbsFrameNum > 0)
            {
                i4PicOrderCntCycleCnt = (i4AbsFrameNum - 1)/(INT32)prDecPrm->prSPS->u4NumRefFramesInPicOrderCntCycle;
                i4FrameNumInPicOrderCntCycle = (i4AbsFrameNum - 1) %
                                            (INT32)prDecPrm->prSPS->u4NumRefFramesInPicOrderCntCycle;        
            }

            i4ExpectedDeltaPerPicOrderCntCycle = 0;
            for(i=0; i<(INT32)prDecPrm->prSPS->u4NumRefFramesInPicOrderCntCycle; i++)
            {
                i4ExpectedDeltaPerPicOrderCntCycle += prDecPrm->prSPS->ai4OffsetForRefFrame[i];
            }
            if(i4AbsFrameNum > 0)
            {
                i4ExpectedDeltaPerPicOrderCnt = i4PicOrderCntCycleCnt * i4ExpectedDeltaPerPicOrderCntCycle;
                for(i=0; i<=i4FrameNumInPicOrderCntCycle; i++)
                {
                    i4ExpectedDeltaPerPicOrderCnt = i4ExpectedDeltaPerPicOrderCnt + 
                                                prDecPrm->prSPS->ai4OffsetForRefFrame[i];
                }
            }
            else
            {
                i4ExpectedDeltaPerPicOrderCnt = 0;
            }
            if(prDecPrm->u1NalRefIdc == 0)
            {
                i4ExpectedDeltaPerPicOrderCnt = i4ExpectedDeltaPerPicOrderCnt + 
                                            prDecPrm->prSPS->i4OffsetForNonRefPic;
             }
            if(!prDecPrm->prSliceHdr->fgFieldPicFlag)
            {
                prDecPrm->prCurrFBufInfo->i4TFldPOC = i4ExpectedDeltaPerPicOrderCnt + 
                                                    prDecPrm->prSliceHdr->ai4DeltaPicOrderCnt[0];
                i4Temp = prDecPrm->prCurrFBufInfo->i4TFldPOC + prDecPrm->prSPS->i4OffsetForTopToBottomField;
                prDecPrm->prCurrFBufInfo->i4BFldPOC = i4Temp + prDecPrm->prSliceHdr->ai4DeltaPicOrderCnt[1];
                prDecPrm->prCurrFBufInfo->i4BFldPOC = prDecPrm->prCurrFBufInfo->i4TFldPOC + 
                                                    prDecPrm->prSPS->i4OffsetForTopToBottomField + 
                                                    prDecPrm->prSliceHdr->ai4DeltaPicOrderCnt[1];
            }
            else if(!prDecPrm->prSliceHdr->fgBottomFieldFlag)
            {
                prDecPrm->prCurrFBufInfo->i4TFldPOC = i4ExpectedDeltaPerPicOrderCnt + 
                                                    prDecPrm->prSliceHdr->ai4DeltaPicOrderCnt[0];
            }
            else
            {
                prDecPrm->prCurrFBufInfo->i4BFldPOC = i4ExpectedDeltaPerPicOrderCnt + 
                                                    prDecPrm->prSPS->i4OffsetForTopToBottomField + 
                                                    prDecPrm->prSliceHdr->ai4DeltaPicOrderCnt[0];      
            }
            break;

        case 2:
            if(prDecPrm->rLastInfo.fgLastMmco5)
            {
                i4PrevFrameNumOffset = 0;
            }
            else
            {
                i4PrevFrameNumOffset = prDecPrm->rLastInfo.i4LastFrameNumOffset;
            }      
            if(fgIsIDRPic(prDecPrm->u1NalUnitType))
            {
                prDecPrm->i4FrmNumOffset = 0;
            }
            else if(prDecPrm->rLastInfo.u4LastFrameNum > prDecPrm->prSliceHdr->u4FrameNum)
            {
                prDecPrm->i4FrmNumOffset = i4PrevFrameNumOffset + (INT32)prDecPrm->prSPS->u4MaxFrameNum;
            }
            else
            {
                prDecPrm->i4FrmNumOffset = i4PrevFrameNumOffset;
            }
            if(fgIsIDRPic(prDecPrm->u1NalUnitType))
            {
                // Use iAbsFrameNum as tempPicOrderCnt
                i4AbsFrameNum = 0;
            }
            else if(prDecPrm->u1NalRefIdc == 0)
            {
                i4AbsFrameNum = ((prDecPrm->i4FrmNumOffset + (INT32)prDecPrm->prSliceHdr->u4FrameNum) *2) - 1;
            }
            else
            {
                i4AbsFrameNum = ((prDecPrm->i4FrmNumOffset + (INT32)prDecPrm->prSliceHdr->u4FrameNum) *2);        
            }
      
            if(!prDecPrm->prSliceHdr->fgFieldPicFlag)
            {
                prDecPrm->prCurrFBufInfo->i4TFldPOC = i4AbsFrameNum;
                prDecPrm->prCurrFBufInfo->i4BFldPOC = i4AbsFrameNum;
            }
            else if(!prDecPrm->prSliceHdr->fgBottomFieldFlag)
            {
                prDecPrm->prCurrFBufInfo->i4TFldPOC = i4AbsFrameNum;
            }
            else
            {
                prDecPrm->prCurrFBufInfo->i4BFldPOC = i4AbsFrameNum;
            }     
            break;
            
        default:
            break;
    }
    prDecPrm->prCurrFBufInfo->i4POC = 
        (prDecPrm->prCurrFBufInfo->i4TFldPOC < prDecPrm->prCurrFBufInfo->i4BFldPOC)?
        prDecPrm->prCurrFBufInfo->i4TFldPOC : prDecPrm->prCurrFBufInfo->i4BFldPOC;
}


// *********************************************************************
// Function    : void vSlidingWindowProce(void)
// Description : marking the decoded ref pic with sliding window method
// Parameter   : None
// Return      : None
// *********************************************************************
void _VDEC_SlidingWindowProce(VDEC_AVC_DecPrm *prDecPrm)
{
    UINT32 i;
    INT32 i4MinFrameNumWrap;
    INT32 i4FrameNumWrap;
    UINT32 u4NumShortTerm;
    UINT32 u4NumLongTerm;
    UINT32 u4MinFBufIdx = 0;
    UINT32 u4MinPOCFBufIdx = 0;
    INT32 i4MinPOC = 0x7fffffff;
    
    ASSERT(prDecPrm!=NULL);
    
    // If the curr pic is the 2nd field, follow the 1st field's ref info
    if((prDecPrm->prCurrFBufInfo->u1FBufStatus == FRAME) && (prDecPrm->u1PicStruct == BOTTOM_FIELD) && 
        (prDecPrm->prCurrFBufInfo->u1TFldRefType == SREF_PIC))
    {
        prDecPrm->prCurrFBufInfo->u1BFldRefType = SREF_PIC;
    }
    else if((prDecPrm->prCurrFBufInfo->u1FBufStatus == FRAME) && (prDecPrm->u1PicStruct == TOP_FIELD) && 
        (prDecPrm->prCurrFBufInfo->u1BFldRefType == SREF_PIC))
    {
        prDecPrm->prCurrFBufInfo->u1TFldRefType = SREF_PIC;
    }
    else
    {
        i = 0;
        i4MinFrameNumWrap = 0xfffffff;
        u4NumShortTerm = 0;
        u4NumLongTerm = 0;  
        // Remove 1 SREF pic for a new ref pic
        for(i=0; i<prDecPrm->u4MaxFBufNum; i++)
        {
            if((prDecPrm->arFBufInfo[i].u1TFldRefType == SREF_PIC) || 
                (prDecPrm->arFBufInfo[i].u1BFldRefType == SREF_PIC))
            {
                i4FrameNumWrap = (INT32)((prDecPrm->arFBufInfo[i].u4FrameNum > prDecPrm->prCurrFBufInfo->u4FrameNum)? 
                    (prDecPrm->arFBufInfo[i].u4FrameNum - prDecPrm->prSPS->u4MaxFrameNum) : 
                    prDecPrm->arFBufInfo[i].u4FrameNum);
                if(i4MinFrameNumWrap > i4FrameNumWrap)
                {
                    i4MinFrameNumWrap =  i4FrameNumWrap;
                    u4MinFBufIdx = (UINT32)i;        
                }
                LOG(9, "SW SREF POC %d, FrameNum %d\n", prDecPrm->arFBufInfo[i].i4POC ,prDecPrm->arFBufInfo[i].u4FrameNum);
                u4NumShortTerm ++;
            }
            if((prDecPrm->arFBufInfo[i].u1TFldRefType == LREF_PIC) || 
                (prDecPrm->arFBufInfo[i].u1BFldRefType == LREF_PIC))
            {
                u4NumLongTerm ++;
            }
            // check wrap around POC
            else if(i4MinPOC > prDecPrm->arFBufInfo[i].i4POC)
            {
                i4MinPOC = prDecPrm->arFBufInfo[i].i4POC;
                u4MinPOCFBufIdx = i;
            }
        }
        // check wrap around POC
        if(prDecPrm->i4LastPOC < prDecPrm->arFBufInfo[u4MinPOCFBufIdx].i4POC)
        {
            prDecPrm->fgPOCWrap = FALSE;
        }
        else if((FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId) 
            == FBM_FB_STATUS_DECODE) && (prDecPrm->prCurrFBufInfo->u1FBufStatus == FRAME))
        {   
            //prDecPrm->fgPOCWrap = TRUE;
            LOG(2, "POC wrap detect!\n");
        }
        // Since current pic should be ref pic, the condition should be modified as "larger" only
        // but the current one not set as ref pic at this time,
        if((u4NumShortTerm + u4NumLongTerm) >= 
            ((prDecPrm->prSPS->u4NumRefFrames > 0) ? prDecPrm->prSPS->u4NumRefFrames : 1))
        {
            if((u4MinFBufIdx != u4MinPOCFBufIdx) || (!prDecPrm->fgPOCWrap))
            {
                // Remove the smallet FrameNumWrap item
                LOG(9, "RM SREF POC %d, FrameNum %d\n", prDecPrm->arFBufInfo[u4MinFBufIdx].i4POC ,prDecPrm->arFBufInfo[u4MinFBufIdx].u4FrameNum);
                _vClrPicRefInfo(prDecPrm, FRAME, (UINT8)u4MinFBufIdx);
            }
            //prDecPrm->arFBufInfo[u4MinFBufIdx].u1FBufRefType = NREF_PIC;
            //prDecPrm->arFBufInfo[u4MinFBufIdx].u1TFldRefType = NREF_PIC;
            //prDecPrm->arFBufInfo[u4MinFBufIdx].u1BFldRefType = NREF_PIC;    
              #if 0    
            _ptFBufInfo[dwMinFBufIdx].dwFrameNum = 0xffffffff;
            _ptFBufInfo[dwMinFBufIdx].iFrameNumWrap = 0xefffffff;
            _ptFBufInfo[dwMinFBufIdx].iPicNum = 0xefffffff;
            _ptFBufInfo[dwMinFBufIdx].iTFldPicNum = 0xefffffff;
             _ptFBufInfo[dwMinFBufIdx].iBFldPicNum = 0xefffffff;
            _ptFBufInfo[dwMinFBufIdx].bLongTermFrameIdx = 0xff;
            _ptFBufInfo[dwMinFBufIdx].bTFldLongTermFrameIdx = 0xff;
            _ptFBufInfo[dwMinFBufIdx].bBFldLongTermFrameIdx = 0xff;
            _ptFBufInfo[dwMinFBufIdx].iLongTermPicNum = 0xefffffff;
            _ptFBufInfo[dwMinFBufIdx].iTFldLongTermPicNum = 0xefffffff;
            _ptFBufInfo[dwMinFBufIdx].iBFldLongTermPicNum = 0xefffffff;    
            #endif    
        }
    }
}


// *********************************************************************
// Function    : void vFlushBufRefInfo()
// Description : flush DPB Ref info
// Parameter   : None
// Return      : None
// *********************************************************************
void _VDEC_FlushBufRefInfo(VDEC_AVC_DecPrm* prDecPrm)
{
    UINT32 i;

    ASSERT(prDecPrm!=NULL);
    prDecPrm->u1MaxLongTermFrameIdx = 0xff;

    for(i=0; i<prDecPrm->u4MaxFBufNum; i++)
    {
        prDecPrm->arFBufInfo[i].fgNonExisting = FALSE;        
    
        prDecPrm->arFBufInfo[i].u1FBufRefType = NREF_PIC;
        prDecPrm->arFBufInfo[i].u1TFldRefType = NREF_PIC;
        prDecPrm->arFBufInfo[i].u1BFldRefType = NREF_PIC;    
        prDecPrm->arFBufInfo[i].u4LongTermFrameIdx = 0xff;
        prDecPrm->arFBufInfo[i].u4TFldLongTermFrameIdx = 0xff;
        prDecPrm->arFBufInfo[i].u4BFldLongTermFrameIdx = 0xff;
    }
    for(i=0; i<3; i++)
    {
        prDecPrm->arRefPicList[i].u4RefPicCnt = 0;
    } 
}


// **************************************************************************
// Function : UINT8 _VDEC_WaitDecDone(VDEC_AVC_DecPrm rDecPrm);
// Description : WaitDecDone
// Parameter : rDecPrm
// Return      : BOOL, if decode successfully
// **************************************************************************
UINT8 _VDEC_WaitDecDone(UINT8 u1PicStruct, UINT32 u4W, UINT32 u4H, BOOL bWaitDecLock)
{
    UINT8 u1Ret = 0;
    UINT32 u4CntTimeChk = 0;
    UINT32 u4MbX, u4MbY, u4Tmp;
    DRV_DBASE_T* _prDbase = NULL;
    CRIT_STATE_T _rCritState;
    PARAM_MPV_T* prInfo;
    VDEC_INFO_T *prVdecInfo;   
    VDEC_STATUS_CB_T rStatusCB;
    
    prVdecInfo = _VDEC_GetInfo();
    
    #ifndef AVC_IRQ_DISABLE
    if(bWaitDecLock)
    {
        VERIFY(x_sema_lock(_hVdecFinSema, X_SEMA_OPTION_WAIT) == OSR_OK);    //wait decode done
    }
    
    u1Ret = (UINT8)(_bTimeOutFlag? 0: 1);
    if(u1PicStruct == FRAME)
    {
        _prDbase = DBS_Lock(&_rCritState);
        ASSERT(_prDbase);
        VERIFY(DBS_Unlock(_prDbase, _rCritState));        
        prInfo = &(_prDbase->rMpv[0]);
    
        if(_bTimeOutFlag == 1)
        {
            prInfo->u4TimeOutNs++;
            if(prVdecInfo->pfnStatusCb)
            {
                rStatusCB.u1EsId = prVdecInfo->rPesInfo.ucEsId;
                rStatusCB.u4Status = (UINT32)VDEC_PIC_DECODING_ERROR;
                prVdecInfo->pfnStatusCb(&rStatusCB);
            }            
        }
        else
        {
            prInfo->u4DecOkNs++;
        }
    }
    if(_bTimeOutFlag)
    {
        LOG(2, "//Error! TimeOut, Error Msg 0x270=0x%x !!!\n", AVC_VLD_READ32(RO_AVLD_ERR_MESSAGE));
        LOG(2, "//AVC_VLD 0x284(161) Error acc 0x%x\n", AVC_VLD_READ32(RO_AVLD_ERR_ACC));
        LOG(2, "//AVC_VLD 288(162) decode cycle 0x%x\n", AVC_VLD_READ32(RO_AVLD_DEC_CYCLE));    
        u4CntTimeChk = DEC_RETRY_NUM;
        _bTimeOutFlag = FALSE;
    }
    else    //wait picture finish
    {
        while(((AVC_VLD_READ32(RO_AVLD_PIC_FINISH) & AVLD_FINISH_MASK) == 0) &&
            (u4CntTimeChk < DEC_RETRY_NUM))
        {
            u4CntTimeChk++;
        }
        #ifdef VDEC_TIME_PROFILE
        #ifdef CC_MT5365
        u4DramCycle = IO_REG32(DRAM_BASE, 0x190);
        #else
        u4DramCycle = IO_REG32(DRAM_BASE, 0x3C8);
        #endif
        //LOG(2, "dram cycle=%d\n", u4DramCycle);
        #endif        
    }

    
    #ifdef VDP_RACE_LOG
    VDP_FrcRacingLog('F');
    #endif
    
    #if 0    //RR mode testing with FBM_DPB_CTRL
    static UINT32 u4IntraIn = 0, u4MC_Y = 0;
    static UINT32 u4TrigCnt = 1, u4CmpFail = 0;
    
    LOG(2, "AVC_VLD 2A0(168) Bs-in checksum 0x%x\n", AVC_VLD_READ32(RO_AVLD_BS_SUM));
    LOG(2, "AVC_VLD 294(165) IntraPred DramIn 0x%x\n", AVC_VLD_READ32(0x294));
    LOG(2, "AVC_VLD 298(166) IntraPred DramOut 0x%x\n", AVC_VLD_READ32(0x298));
    LOG(2, "AVC_VLD 2B8(174) 0x%x\n", AVC_VLD_READ32(0x2B8));
    LOG(2, "AVC_VLD 2BC(175) 0x%x\n", AVC_VLD_READ32(0x2BC));
    LOG(2, "AVC_VLD 2C0(176) 0x%x\n", AVC_VLD_READ32(0x2C0));
    LOG(2, "idct y0y1 0x%x\n", MC_READ32(RO_MC_CHKSUM_IDCT_Y0Y1));
    LOG(2, "idct y2y3 0x%x\n", MC_READ32(RO_MC_CHKSUM_IDCT_Y2Y3));
    LOG(2, "idct cbcr 0x%x\n", MC_READ32(RO_MC_CHKSUM_IDCT_CBCR));
    LOG(2, "MC Y 0x%x\n", MC_READ32(RO_MC_CHKSUM_MC_OUTY));
    LOG(2, "MC C 0x%x\n", MC_READ32(RO_MC_CHKSUM_MC_OUTC));
    LOG(2, "After ILF PP_Y 0x%x\n", MC_READ32(RO_MC_CHKSUM_PP_Y));
    LOG(2, "After ILF PP_C 0x%x\n", MC_READ32(RO_MC_CHKSUM_PP_C));
    LOG(2, "QBI 0x61C 0x%x\n", MC_READ32(RO_MC_CHKSUM_QPEL_QBI));
    LOG(2, "QBO 0x620 0x%x\n", MC_READ32(RO_MC_CHKSUM_QPEL_QBO));
    LOG(2, "MB buf input(IntraPrediction) 0x624 0x%x, 0x574 0x%x\n", MC_READ32(RO_MC_CHKSUM_MBI), MC_READ32(0x574));
    LOG(2, "before RR 0x628 0x%x\n", MC_READ32(RO_MC_CHKSUM_DS_MIX1));
    LOG(2, "before RR 0x62C 0x%x\n", MC_READ32(RO_MC_CHKSUM_DS_MIX2));
    LOG(2, "RR checksum 0x678 0x%x\n", MC_READ32(0x678));

    for(u4Temp = 212; u4Temp <= 220; u4Temp++)
    {
        LOG(2, "AVC_MV 0x%x=0x%x\n", u4Temp<<2, AVC_MV_READ32(u4Temp<<2));
    }
    LOG(2, "before RR 0x630 0x%x\n", MC_READ32(RO_MC_CHKSUM_DS_MIX3));
    LOG(2, "before RR 0x634 0x%x\n", MC_READ32(RO_MC_CHKSUM_DS_MIX4));

    if((u4MC_Y > 0) && (MC_READ32(RO_MC_CHKSUM_MC_OUTY)!=u4MC_Y))
    {
        LOG(2, "Checksum mismatch, u4TrigCnt=%d, u4CmpFailCnt=%d\n", u4TrigCnt, ++u4CmpFail);    
    }
    u4MC_Y = MC_READ32(RO_MC_CHKSUM_MC_OUTY);
    
    VDEC_DISP_PIC_INFO_T rDispInfo;
    rDispInfo.pvYAddr = (void*)MC_READ32(RW_MC_Y_OUT_ADDR);
    rDispInfo.pvCAddr = (void*)(MC_READ32(RW_MC_Y_OUT_ADDR) + MC_READ32(RW_MC_CBCR_OFFSET));
    LOG(2, "Dec 0x%x, 0x%x\n", rDispInfo.pvYAddr, rDispInfo.pvCAddr);
    rDispInfo.u1PicStruct = FRAME;
    rDispInfo.u4ChromaFormatIdc = 1;
    rDispInfo.u4YSize = ((((u4W + 15) >> 4) * ((u4H + 31) >> 5)) << 9);
    rDispInfo.u4CSize = rDispInfo.u4YSize >> 1;
    rDispInfo.u4W = u4W;
    rDispInfo.u4H = u4H;

    _VdecNotify(1, (UINT32)&rDispInfo, 1, 1);
    LOG(2, "Dec Cmp done, u4TrigCnt=%d, u4CmpFailCnt=%d\n", u4TrigCnt++, u4CmpFail);
    #endif
    
    #else
    UINT32 u4MbX_last, u4MbY_last; 
    UINT32 u4Cnt = 0;

    _VDEC_MCGetMbXY(&u4MbX, &u4MbY);
    while(u4CntTimeChk < DEC_RETRY_NUM) //polling retry times
    {
        u4Cnt++;
        if((u4Cnt & 0x3f)== 0x3f)    //delay
        {
            //vIrqProc();
            if(AVC_VLD_READ32(RO_AVLD_COMPLETE))
            {
                //_fgVDecComplete = TRUE;
                u1Ret = 1;
                break;
            }
            /*if(_fgIsVDecComplete(u1PicStruct, u4W, u4H))
            {
                u4CntTimeChk = 0;
                u1Ret = 1;
                break;
            }*/
            else
            {
                u4MbX_last = u4MbX;
                u4MbY_last = u4MbY;
                _VDEC_MCGetMbXY(&u4MbX, &u4MbY);
                if((u4MbX == u4MbX_last) && (u4MbY == u4MbY_last))
                {
                    u4CntTimeChk++;
                }
                else
                {
                    u4CntTimeChk = 0;
                }
            }
            u4Cnt = 0;
        }
    }
    #endif

    //_vSaveChkSum();
    //_VDEC_DumpReg();

    u4Tmp = AVC_VLD_READ32(RO_AVLD_ERR_ACC) & ERR_MASK_OBSV;
    if(u4Tmp != 0)
    {
        LOG(5, "AVC_VLD 284(161) Error acc 0x%x\n", u4Tmp);    //ignore bit21
    }
    u4Tmp = AVC_VLD_READ32(RO_AVLD_ERR_MESSAGE) & ERR_MASK_OBSV;
    if((u4CntTimeChk == DEC_RETRY_NUM) || (u4Tmp != 0))
    {
        if(u4CntTimeChk == DEC_RETRY_NUM)
        {
            LOG(2, "\n!!!!!!!!! Decoding Timeout !!!!!!!\n");
            //dump status register
            LOG(2, "AVC_VLD 274(157) Dec complete 0x%x\n", AVC_VLD_READ32(RO_AVLD_COMPLETE));
            LOG(2, "AVC_VLD 284(161) Error acc 0x%x\n", AVC_VLD_READ32(RO_AVLD_ERR_ACC) & ERR_MASK_OBSV);            
            LOG(2, "AVC_VLD 270(156) Error msg 0x%x\n", AVC_VLD_READ32(RO_AVLD_ERR_MESSAGE) & ERR_MASK_OBSV);
            LOG(2, "AVC_VLD 280(160) Error mask 0x%x\n", AVC_VLD_READ32(RW_AVLD_ERR_MASK));
            u4Tmp = AVC_VLD_READ32(RO_AVLD_SMKR);
            LOG(2, "AVC_VLD 250(148) 0x%x, x %d, y %d\n", u4Tmp, u4Tmp>>24, (u4Tmp<<8)>>24);            
            LOG(2, "AVC_VLD 268(154) timeout threshold 0x%x\n", AVC_VLD_READ32(RW_AVLD_TIMEOUT_THD));
            LOG(2, "AVC_VLD 288(162) decode cycle 0x%x\n", AVC_VLD_READ32(RO_AVLD_DEC_CYCLE));
            LOG(2, "AVC_VLD 290(164) mc busy threshold 0x%x\n\n", AVC_VLD_READ32(RW_AVLD_MC_BUSY_THD));

            LOG(2, "VLD F8(62) Wp 0x%x\n", VLD_READ32(RO_VLD_VWPTR));
            LOG(2, "VLD FC(63) Rp 0x%x\n", VLD_READ32(RO_VLD_VRPTR));
            LOG(2, "VLD B4(45) Vstart 0x%x\n", VLD_READ32(RW_VLD_VSTART)<<6);
            LOG(2, "VLD B8(46) VEnd 0x%x\n\n", VLD_READ32(RW_VLD_VEND)<<6);

            LOG(2, "AVC_VLD 29C(167) Residual checksum 0x%x\n", AVC_VLD_READ32(RO_AVLD_RESISUAL_SUM));
            LOG(2, "AVC_VLD 2A0(168) Bs-in checksum 0x%x\n", AVC_VLD_READ32(RO_AVLD_BS_SUM));
            LOG(2, "AVC_VLD 2d0(180) ERC control 0x%x\n", AVC_VLD_READ32(RW_AVLD_ERC_CTRL));
            LOG(2, "AVC_VLD 248(146) slice hdr state 0x%x\n", AVC_VLD_READ32(RO_AVLD_SLICE_HDR_STATE));
            LOG(2, "AVC_VLD 254(149) slice data state 0x%x\n", AVC_VLD_READ32(RO_AVLD_SLICE_STATE));
            LOG(2, "AVC_VLD 258(150) mb state 0x%x\n", AVC_VLD_READ32(RO_AVLD_MB_STATE));
            LOG(2, "AVC_VLD 25C(151) mb pred state 0x%x\n", AVC_VLD_READ32(RO_AVLD_MB_PRED_STATE));
            LOG(2, "AVC_VLD 260(152) residual state 0x%x\n", AVC_VLD_READ32(RO_AVLD_RESIDUAL_STATE));
            LOG(2, "AVC_VLD 264(153) residual blk state 0x%x\n", AVC_VLD_READ32(RO_AVLD_RESIDUAL_BLK_STATE));            
            LOG(2, "AVC_VLD 24C(147) weighted pred state 0x%x\n", AVC_VLD_READ32(RO_AVLD_WEIGHTED_PRED_STATE));
            LOG(2, "MC 5c4(369) PP status 0x%x\n", MC_READ32(RO_MC_PP_STATUS));
            
            #ifdef STRESS_TESTING
            u1Ret = 2;    //need log slice info
            //_VDEC_DumpReg();
            #else
            u1Ret = 0;
            while(1);
            #endif
        }
        _VDEC_MCGetMbXY(&u4MbX, &u4MbY);
        //LOG(2, "//!!!!!!!!! Error msg 0x%.8x!!!!!!!\n", AVC_VLD_READ32(RO_AVLD_ERR_MESSAGE) & ERR_MASK_OBSV);
        //u4Temp = (1-((UINT32)fgIsFrmPic(u1PicStruct)));
        
        //LOG(2, "//!!!!!!!!! Error at MC (x,y)=(%d/%d, %d/%d)  !!!!!!!\n", 
        //         u4MbX, ((u4W + 15)>> 4) - 1, 
        //         u4MbY, (((u4H >> u4Temp) + 15)>> 4) - 1);
    }
    
    if(u1Ret > 0)
    {
        u4DecCycle = AVC_VLD_READ32(RO_AVLD_DEC_CYCLE);
        if(u4MaxDecCycle < u4DecCycle)
        {
            u4MaxDecCycle = u4DecCycle;
            LOG(8, "//Max Dec Cycle %d\n", u4MaxDecCycle);
            u1Ret = 2;    //need log slice info
        }
    }
    UNUSED(u4W);
    UNUSED(u4H);
    UNUSED(u1PicStruct);
    return u1Ret;
}

// **************************************************************************
// Function : void _VDEC_DecProc(void);
// Description : Init Video Decoder
// Parameter : None
// Return      : 
// **************************************************************************
void _VDEC_DecProc(const VDEC_AVC_DecPrm* prDecPrm, INT32* pi4RetValue)
{
    UINT32 u4Temp,u4SHDRInfo;
    //static UINT32 u4Cnt = 0;
    #if (defined(CC_MT5360B) || defined(CC_MT5392B) || defined(IS_POST_MT5363)) && defined (FBM_DPB_CTRL)
    FBM_PIC_HDR_T * prFbmPicHdr;
    #endif
    #ifdef CC_MT5395
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    #endif
    VDEC_ES_INFO_T* prVdecEsInfo = _VDEC_GetEsInfo(ES0);
    #ifdef CC_MT5365
    UINT32 u4DramType = DRVCUST_InitGet(eDramType);
    #endif
    
    ASSERT(pi4RetValue!=NULL);
    ASSERT(prDecPrm!=NULL);

    // Fld & Frame height same?
    MC_WRITE32(RW_MC_UMV_PIC_WIDTH, prDecPrm->u4PicW);
    MC_WRITE32(RW_MC_UMV_PIC_HEIGHT, prDecPrm->u4PicH);  

    // Only one case needs to turn off deblocking
    /*if(0)//ptMpvDecPrm->tSliceHdr->dwDisableDeblockingFilterIdc == 1)
    {
        //test only
        MC_WRITE32(RW_MC_PP_ENABLE, 0);  
        MC_WRITE32(RW_MC_PP_WB_BY_POST, 0);  
        MC_WRITE32(RW_MC_PP_DBLK_MODE, 0);            
    }
    else*/
    {
        ASSERT(PHYSICAL((UINT32)prDecPrm->pu1DecWorkBufY) > 0);
        ASSERT(PHYSICAL((UINT32)prDecPrm->pu1DecWorkBufC) > 0);
    
        MC_WRITE32(RW_MC_PP_ENABLE, 1);  
        MC_WRITE32(RW_MC_PP_Y_ADDR, PHYSICAL((UINT32)prDecPrm->pu1DecWorkBufY) >> 9);  
        MC_WRITE32(RW_MC_PP_C_ADDR, PHYSICAL((UINT32)prDecPrm->pu1DecWorkBufC) >> 8);  
        MC_WRITE32(RW_MC_PP_WB_BY_POST, 1); 
        MC_WRITE32(RW_MC_PP_DBLK_MODE, (UINT32)(DBLK_Y | DBLK_C));

        MC_WRITE32(RW_MC_PP_X_RANGE, ((prDecPrm->u4PicW + 15)>> 4) - 1);
        u4Temp = (1-(UINT32)(fgIsFrmPic(prDecPrm->u1PicStruct)));
        u4Temp = ((prDecPrm->u4PicH >> u4Temp) + 15);
        MC_WRITE32(RW_MC_PP_Y_RANGE, (u4Temp >> 4) - 1);    
        //MC_WRITE32(RW_MC_PP_MB_WIDTH, (((prDecPrm->u4PicW + 15)>> 4) + 0x3)& (~0x3));
        if(prDecPrm->fgOutDispOrder)
        {
            MC_WRITE32(RW_MC_PP_MB_WIDTH, ((prVdecEsInfo->u2LineSize + 15)>> 4));
        }
        else
        {
            MC_WRITE32(RW_MC_PP_MB_WIDTH, ((prDecPrm->u4PicW + 15)>> 4));
        }
        //should be multiple of 4
    }
    MC_WRITE32(RW_MC_Y_OUT_ADDR, PHYSICAL((UINT32)prDecPrm->pu1DecWorkBufY));
    MC_WRITE32(RW_MC_CBCR_OFFSET, (UINT32)prDecPrm->pu1DecWorkBufC - (UINT32)prDecPrm->pu1DecWorkBufY);

    if(prDecPrm->fgOutDispOrder && (!(prDecPrm->fgRRModeYEn || prDecPrm->fgRRModeCEn)))
    {
        MC_WRITE32(RW_MC_ADDRSWAP, ADDR_MODE_5351_NORMAL);
        #if defined(IS_POST_MT5363)
        MC_WRITE32(RW_MC_ADDRSWAP_NBM, ADDR_MODE_5351_NORMAL);
        PP_WRITE32(RW_PP_ADDRSWAP_SEL, PP_ADDR_MODE_5351_NORMAL);
        #endif
    }
    else
    {
        MC_WRITE32(RW_MC_ADDRSWAP, ADDR_MODE_8520);    //8520 swap mode
        #if defined(IS_POST_MT5363)
        MC_WRITE32(RW_MC_ADDRSWAP_NBM, ADDR_MODE_8520);
        PP_WRITE32(RW_PP_ADDRSWAP_SEL, PP_ADDR_MODE_8520);
        #endif        
    }
     //MC_WRITE32(RW_MC_ADDR_MODE, WADDM_RASTER |RADDM_RASTER);

    #ifdef CC_MT5365
    if(u4DramType == DDR_III_x2)
    {
        MC_WRITE32(RW_MC_32B_MODE_EN1, MC_READ32(RW_MC_32B_MODE_EN1) | 1);
        MC_WRITE32(RW_MC_32B_MODE_EN2, MC_READ32(RW_MC_32B_MODE_EN2) | 1);
        
        if(prDecPrm->prSPS && (prDecPrm->prSPS->u4ChromaFormatIdc > 0)) //0: mono color(Y only)
        {
            MC_WRITE32(RW_MC_PP_OUT_32B_MODE_EN, 
                MC_READ32(RW_MC_PP_OUT_32B_MODE_EN) & 0xFFFFFFFE);
        }
        //force 64x32 block mode
        MC_WRITE32(RW_MC_ADDRSWAP, ADDR_MODE_5351_NORMAL);
        MC_WRITE32(RW_MC_ADDRSWAP_NBM, ADDR_MODE_5351_NORMAL);
        PP_WRITE32(RW_PP_ADDRSWAP_SEL, PP_ADDR_MODE_5351_NORMAL);
    }
    #endif
    
    //RR mode
    if(prDecPrm->fgRRModeYEn | prDecPrm->fgRRModeCEn)
    {
        if(prDecPrm->fgRRWrapper)
        {
            MC_WRITE32(RW_MC_BYPASS_DBLK_WRAP, 0);
        }
        else
        {
            MC_WRITE32(RW_MC_BYPASS_DBLK_WRAP, 1);
        }
        MC_WRITE32(RW_MC_RR_CTRL, (MC_READ32(RW_MC_RR_CTRL) & 0x3F000000) | RR_COMPACT |
            ((UINT32)prDecPrm->fgRRModeYEn << EN_Y_BIT) |
            ((UINT32)prDecPrm->fgRRModeCEn << EN_C_BIT) |
            ((UINT32)prDecPrm->u1RRModeY << Y_RATIO_BIT) |
            ((UINT32)prDecPrm->u1RRModeC << C_RATIO_BIT) |
            ((UINT32)prDecPrm->u1MaxDisHbands << HBAND_BIT));
        MC_WRITE32(RW_MC_PP_RR_CTRL, (MC_READ32(RW_MC_PP_RR_CTRL) & 0x3F000000) | RR_COMPACT |
            ((UINT32)prDecPrm->fgRRModeYEn << EN_Y_BIT) |
            ((UINT32)prDecPrm->fgRRModeCEn << EN_C_BIT) |
            ((UINT32)prDecPrm->u1RRModeY << Y_RATIO_BIT) |
            ((UINT32)prDecPrm->u1RRModeC << C_RATIO_BIT) |
            ((UINT32)prDecPrm->u1MaxDisHbands << HBAND_BIT));

        #if (defined(CC_MT5360B) || defined(CC_MT5392B) || defined(IS_POST_MT5363)) && defined (FBM_DPB_CTRL)   //new field mode
            prFbmPicHdr = FBM_GetFrameBufferPicHdr(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId);
            if(prFbmPicHdr != NULL)
            {
                if(!prFbmPicHdr->fgProgressiveFrm)
                {
                    MC_WRITE32(RW_MC_RR_FMT, RR_FIELD_DISP_MODE);
                }
            }
            //disable MC request connection
            MC_WRITE32(RW_MC_REQ_CONNECT, MC_READ32(RW_MC_REQ_CONNECT) | REQ_CON_DISABLE);
        #endif
    }
    else
    {
        MC_WRITE32(RW_MC_BYPASS_DBLK_WRAP, 0); 
        #if defined(IS_POST_MT5363)
        MC_WRITE32(RW_MC_REQ_CONNECT, MC_READ32(RW_MC_REQ_CONNECT) | REQ_CON_DISABLE);
        #endif
        #if defined(CC_MT5360)
        {//!bypass
            MC_WRITE32(RW_MC_RR_CTRL, (MC_READ32(RW_MC_RR_CTRL) & 0xFFFFFFFE));
            MC_WRITE32(RW_MC_PP_RR_CTRL, (MC_READ32(RW_MC_PP_RR_CTRL) & 0xFFFFFFFE));            
        }
        #else
        {
            MC_WRITE32(RW_MC_RR_CTRL, (MC_READ32(RW_MC_RR_CTRL) & 0xFF000000) | BY_PASS);        
            MC_WRITE32(RW_MC_PP_RR_CTRL, (MC_READ32(RW_MC_PP_RR_CTRL) & 0xFF000000) | BY_PASS);
        }
        #endif
    }

    if(prDecPrm->fgOutDispOrder)
    {
        VLD_WRITE32(RW_MC_BLK_MODE_W, ((prVdecEsInfo->u2LineSize + 15)>> 4));
    }
    else
    {
        VLD_WRITE32(RW_MC_BLK_MODE_W, ((prDecPrm->u4PicW + 15)>> 4));
    }
    //restore 1/4 frame size MV info for each reference frame
    ASSERT(PHYSICAL((UINT32)prDecPrm->pu1DecWorkBufMV) > 0);
    AVC_MV_WRITE32(RW_AMV_WR_ADDR, ((PHYSICAL((UINT32)prDecPrm->pu1DecWorkBufMV))>> 4) + 
                    ((prDecPrm->u1PicStruct == BOTTOM_FIELD)? 4 : 0));
    //prepare slice header info, part II
    u4SHDRInfo = (UINT32)((prDecPrm->prSliceHdr->i4SliceQpDelta) & 0x7f);
    u4SHDRInfo |= ((prDecPrm->prSliceHdr->u4DisableDeblockingFilterIdc & 0x3) << 7);
    u4SHDRInfo |= ((prDecPrm->prSliceHdr->i4SliceAlphaC0OffsetDiv2 & 0xf) << 9);  
    u4SHDRInfo |= ((prDecPrm->prSliceHdr->i4SliceBetaOffsetDiv2 & 0xf) << 13);  
    u4SHDRInfo |= ((prDecPrm->u1NalRefIdc & 0x3) << 17);  
    u4SHDRInfo |= ((UINT32)fgIsIDRPic(prDecPrm->u1NalUnitType) << 19);  
    u4SHDRInfo |= ((prDecPrm->prSliceHdr->u4CabacInitIdc & 0x3) << 20);    
    _VDEC_SetSHDRAVLD2(u4SHDRInfo);  

    if(prDecPrm->prPPS->fgEntropyCodingModeFlag) // CABAC only
    {
        AVC_VLD_WRITE32(RW_AVLD_INIT_CTX_SRAM, 0x1);  
    }

    //MC new wrapper
    #if defined(CC_MT5360B) || defined(CC_MT5392B) || defined(IS_POST_MT5363)
    MC_WRITE32(RW_MC_WRAPPER, ((MC_READ32(RW_MC_WRAPPER) |
        PP_WRAPPER_Y_LINES) | PP_WRAPPER_C_LINES));
    #endif

    #ifdef CC_MT5395
    //input
    if(prVdecInfo)
    {
        HalFlushInvalidateDCacheMultipleLine(prVdecInfo->rPesInfo.u4FifoStart, 
            prVdecInfo->rPesInfo.u4FifoEnd - prVdecInfo->rPesInfo.u4FifoStart);    
    }
    //output
    HalFlushInvalidateDCacheMultipleLine(((UINT32)prDecPrm->pu1DecWorkBufY), 
        ((UINT32)prDecPrm->pu1DecWorkBufC - (UINT32)prDecPrm->pu1DecWorkBufY) << 1);
    #else    
    HalFlushInvalidateDCache();
    #endif
    //VLD_WRITE32(RW_VLD_MB_STOP_POS, (1 << 8) + 0);
    //Checksum dump
    //if(gu1VdecEnableCS == 1)
    //{
    //    VLD_WRITE32(RW_VLD_MB_STOP_POS, (gu1VdecStopMBy << 8) + gu1VdecStopMBx);
    //}
    //if(gu1VdecLogSimPat == 1)
    //{
    //    MC_WRITE32(RW_MC_HALT_MB, (gu1VdecStopMBy << 24) + (gu1VdecStopMBx << 16) + MC_HALT_ENABLE);        
    //}

    //LOG(2, "//InputWin 0x%x\n", AVC_VLD_READ32(RO_AVLD_BARL));
    //LOG(7, "//AVC_VLD 2A0(168) Bs-in checksum 0x%x\n", AVC_VLD_READ32(RO_AVLD_BS_SUM));

    //Dram MC request max len
    if(prDecPrm->u4DramMaxLen > 0)
    {
        MC_WRITE32(RW_MC_REQ_MAX_LEN, MAX_LEN_EN | prDecPrm->u4DramMaxLen);
    }
    //MC_WRITE32(RW_MC_REQ_CONNECT, MC_READ32(RW_MC_REQ_CONNECT) | REQ_CON_DISABLE | FIFO_4BANK);
    MC_WRITE32(RW_MC_REQ_CONNECT, MC_READ32(RW_MC_REQ_CONNECT) 
        | (prDecPrm->u4DramReqConnDis << 12) | (prDecPrm->u4DramFifoBank << 17));

    //profile decoding time
    HAL_GetTime(&rTimeHwS);
    #ifdef VDEC_TIME_PROFILE

    u4DelayClock = (BSP_GetDomainClock(CAL_SRC_DMPLL) >> 1) ; //dram cycle /sec
    if(prDecPrm->u1DramMoniter == 1)  //total bandwidth
    {
        u4DramCycle = SRM_DramMonitor(SRM_DRV_TOTAL, u4DelayClock, 0);
    }
    else if(prDecPrm->u1DramMoniter == 2)  //mpeg
    {
        #if defined(IS_POST_MT5363)
        u4DramCycle = SRM_DramMonitor(SRM_DRV_VDEC_MC, u4DelayClock, 0);
        #else
        u4DramCycle = SRM_DramMonitor(SRM_DRV_MPEG, u4DelayClock, 0);
        #endif
    }
    #endif

    _vECBufSetting(prDecPrm, 
        prDecPrm->arRefPicList[0].u4RefPicCnt, 
        ((prDecPrm->prSPS->u4NumRefFrames > 16) ? 16 : prDecPrm->prSPS->u4NumRefFrames));

    //LOG(2, "Dec Count %d\n", u4Cnt++);
    MC_WRITE32(RW_PP_ECO_SWITCH, MC_READ32(RW_PP_ECO_SWITCH) | PP_ECO_DISABLE);
    
    AVC_VLD_WRITE32(RW_AVLD_PROC, 0x1);  
    *pi4RetValue = 0;
}


// **************************************************************************
// Function : void _VDEC_DoRefPicMarking(void);
// Description : do reference picture marking (adaptive mmco / sliding window)
// Parameter : rDecPrm
// Return      : 
// **************************************************************************

void _VDEC_DoRefPicMarking(VDEC_AVC_DecPrm* prDecPrm)
{
    VDEC_FBUF_INFO_T* prFBufInfo;
    
    ASSERT(prDecPrm!=NULL);

    prFBufInfo = prDecPrm->prCurrFBufInfo;
    ASSERT(prFBufInfo != NULL);
    
    prDecPrm->rLastInfo.u4LastPicW = prDecPrm->u4PicW;
    prDecPrm->rLastInfo.u4LastPicH = prDecPrm->u4PicH;

    //Marking procedure
    if(fgIsRefPic(prDecPrm->u1NalRefIdc))
    {
        if(fgIsIDRPic(prDecPrm->u1NalUnitType)) // IDR pic
        {
            //if(tMpvDecPrm->tSliceHdr->fgNoOutputOfPriorPicsFlag) // clear all prior pic in DPB
            {
                //vFlushBufRefInfo();
            }      
            if(prDecPrm->prSliceHdr->fgLongTermReferenceFlag)
            {
                _vSetPicRefType(prFBufInfo, prDecPrm->u1PicStruct, LREF_PIC);
                prDecPrm->u1MaxLongTermFrameIdx = 0;
                prFBufInfo->u4LongTermFrameIdx = 0;
                prFBufInfo->u4TFldLongTermFrameIdx = 0;
                prFBufInfo->u4BFldLongTermFrameIdx = 0;
            }
            else
            {
                prDecPrm->u1MaxLongTermFrameIdx = 0xff;        
                _vSetPicRefType(prFBufInfo, prDecPrm->u1PicStruct, SREF_PIC);
            }
        }
        else // !IDR pic
        {      
            if(prDecPrm->prSliceHdr->fgAdaptiveRefPicMarkingModeFlag)
            {
                _vAdapRefPicmarkingProce(prDecPrm);
            }
            else
            {              
                _VDEC_SlidingWindowProce(prDecPrm);
            }
            if(_bGetPicRefType(*prFBufInfo, prDecPrm->u1PicStruct) != LREF_PIC)
            {
                _vSetPicRefType(prFBufInfo, prDecPrm->u1PicStruct, SREF_PIC);  
            }      
        }
    }
  
    prDecPrm->rLastInfo.fgLastMmco5 = prDecPrm->prSliceHdr->fgMmco5;  
    prDecPrm->rLastInfo.u1LastPicStruct = prDecPrm->u1PicStruct;
    if(prDecPrm->rLastInfo.fgLastMmco5)
    {
        prDecPrm->rLastInfo.u4LastFrameNum = 0;
        prDecPrm->rLastInfo.i4LastFrameNumOffset = 0;
    }
    else
    {
        prDecPrm->rLastInfo.u4LastFrameNum = prDecPrm->prSliceHdr->u4FrameNum;
        prDecPrm->rLastInfo.i4LastFrameNumOffset = prDecPrm->i4FrmNumOffset;
    }
    prDecPrm->rLastInfo.i4LastPOC =  prFBufInfo->i4POC;      
    prDecPrm->rLastInfo.i4LastTFldPOC = prFBufInfo->i4TFldPOC;
    prDecPrm->rLastInfo.i4LastBFldPOC = prFBufInfo->i4BFldPOC;  

    if(fgIsRefPic(prDecPrm->u1NalRefIdc))
    {
        prDecPrm->rLastInfo.i4LastRefPOC = prFBufInfo->i4POC;  
        //tMpvDecPrm->tLastInfo.iLastRefPOCCntLsb = tMpvDecPrm->tSliceHdr->iPicOrderCntLsb;  
        //tMpvDecPrm->tLastInfo.iLastRefPOCCntMsb = tMpvDecPrm->tSliceHdr->iPicOrderCntMsb;  
        prDecPrm->rLastInfo.i4LastRefPOCLsb = prDecPrm->prSliceHdr->i4PicOrderCntLsb;
        prDecPrm->rLastInfo.i4LastRefPOCMsb = prDecPrm->prSliceHdr->i4PicOrderCntMsb;
    }
}


// **************************************************************************
// Function : void _VDEC_OutputDispPic(void);
// Description : Output 1 frm buff in DPB when DPB full
// Parameter : None
// Return      : 
// **************************************************************************

/*void _VDEC_OutputDispPic(VDEC_AVC_DecPrm* prDecPrm, INT32* pi4RetValue)
{
    UINT32 i, u4FrmCnt = 0;
    INT32 i4MinPOC,i4MinDispPOC, i4DispPOC = 0x7fffffff;
    UINT32 u4MinPOCFBufIdx = 0, u4MinPOCDispFBufIdx = 0, u4DispIdx = 0, u4YAddr, u4CAddr;
    DRV_DBASE_T* _prDbase = NULL;
    CRIT_STATE_T _rCritState;
    PARAM_MPV_T* prInfo;
    STC_CLOCK rStcClk;
    #ifdef FBM_DPB_CTRL
    FBM_PIC_HDR_T * prFbmPicHdr;
    #endif
    
    _prDbase = DBS_Lock(&_rCritState);
    ASSERT(_prDbase);
    VERIFY(DBS_Unlock(_prDbase, _rCritState));        
    prInfo = &(_prDbase->rMpv[0]);

    ASSERT(prDecPrm!=NULL);
    ASSERT(pi4RetValue!=NULL);

    //set if need wait decode done sema in _VDEC_WaitDecDone
    prDecPrm->bWaitDecLock = TRUE;
    
    //change dpb status
    #ifdef FBM_DPB_CTRL
    if(fgIsIDRPic(prDecPrm->u1NalUnitType))
    {
        prDecPrm->i4LastPOC = -99999;
    }    
    #endif
    // Check if DPB full, for frame clr
    i4MinPOC = 0x7fffffff;
    for(i=0; i<prDecPrm->u4MaxFBufNum; i++)
    {
        if(prDecPrm->arFBufInfo[i].u1FBufStatus == NO_PIC)
        {
            i4MinPOC = 0x7fffffff;
            u4MinPOCFBufIdx = i;
            break;
        }
        // miew: need to take care of field empty
        else if((i4MinPOC > prDecPrm->arFBufInfo[i].i4POC) && 
            (fgIsNonRefFBuf(prDecPrm->arFBufInfo[i].u1FBufRefType, prDecPrm->arFBufInfo[i].u1TFldRefType, 
            prDecPrm->arFBufInfo[i].u1BFldRefType) || prDecPrm->fgPOCWrap))
        {
            i4MinPOC = prDecPrm->arFBufInfo[i].i4POC;
            u4MinPOCFBufIdx = i;
        }
    }
    
    //check display order and set current frame as ready
    if(prDecPrm->prCurrFBufInfo->u1FBufStatus == FRAME)
    {
        //wrap around case, POC disorder
        if((i4MinPOC < prDecPrm->i4LastPOC) &&
            (FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId) 
            == FBM_FB_STATUS_DECODE))
        {
            LOG(2, "POC < last POC, FlushDPB\n");
            //_VDEC_FlushDPB(prDecPrm);
            //prDecPrm->i4LastPOC = -99999;
            //prDecPrm->prSPS->fgSPSValid = FALSE;
        }
        //set cur decoded pic from decode state to ready state
        #ifdef VDEC_TIME_PROFILE
        prDecPrm->u4CurWaitFbMicro = 0;
        #endif
        #ifdef FBM_DPB_CTRL
        if(prDecPrm->u1RptFrm > 0)
        {
            FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId, FBM_FB_STATUS_DISPLAYQ);
            *pi4RetValue = (INT32)E_VDEC_OK;
            return;
        }        
        else
        {
            FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId, FBM_FB_STATUS_READY);
            LOG(9, "POC %d, FbId %d ready\n", prDecPrm->prCurrFBufInfo->i4POC, prDecPrm->prCurrFBufInfo->ucFbId);
        }
        #endif        
    }

    //find display frame, may be current decoded frame
    i4MinDispPOC = 0x7fffffff;
    for(i=0; i<prDecPrm->u4MaxFBufNum; i++)
    {
        if(prDecPrm->arFBufInfo[i].u1FBufStatus != NO_PIC)
        {
            u4FrmCnt++;
            if(fgIsNonRefFBuf(prDecPrm->arFBufInfo[i].u1FBufRefType, prDecPrm->arFBufInfo[i].u1TFldRefType, 
                prDecPrm->arFBufInfo[i].u1BFldRefType))
            {
                if((i4MinDispPOC > prDecPrm->arFBufInfo[i].i4POC) &&
                FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[i].ucFbId) 
                == FBM_FB_STATUS_READY)
                {
                    i4MinDispPOC = prDecPrm->arFBufInfo[i].i4POC;
                    u4MinPOCDispFBufIdx = i;
                }
            }
        }
    }

    // output fbuf for display
    if((i4MinDispPOC < 0x7fffffff) &&                                     //display available   
        (u4FrmCnt >= (prDecPrm->prSPS->u4NumRefFrames + 1)) //early display
        #ifdef FBM_DPB_CTRL
        && (prDecPrm->prCurrFBufInfo->u1FBufStatus == FRAME)
        #endif
        )
    {
        prDecPrm->u1MinPOCBufIdx = (UINT8)u4MinPOCDispFBufIdx;

        #ifdef FBM_DPB_CTRL
        LOG(9, "<");
        do
        {
            i4DispPOC = i4MinDispPOC;
            u4DispIdx = u4MinPOCDispFBufIdx;
            for(i=0; i<prDecPrm->u4MaxFBufNum; i++)
            {
                if((i4DispPOC > prDecPrm->arFBufInfo[i].i4POC) &&
                    (FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[i].ucFbId)
                    == FBM_FB_STATUS_READY))
                {
                    i4DispPOC = prDecPrm->arFBufInfo[i].i4POC;
                    u4DispIdx = i;
                }
            }
            if(FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4DispIdx].ucFbId) 
                == FBM_FB_STATUS_READY)
            {
                if(prDecPrm->i4LastPOC > i4DispPOC)    //temp before FBM modification ready
                {
                    LOG(2, "POC %d < last POC %d\n", i4DispPOC, prDecPrm->i4LastPOC);
                    FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4DispIdx].ucFbId, 
                        FBM_FB_STATUS_EMPTY);
                    prDecPrm->i4LastPOC = i4DispPOC;
                    prInfo->u4VpsrDropNs++;
                }
                else if(prDecPrm->arFBufInfo[u4DispIdx].u1FBufStatus != FRAME)  //field empty
                {
                    FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4DispIdx].ucFbId, 
                        FBM_FB_STATUS_EMPTY);
                    prInfo->u4VpsrDropNs++;
                }
                else
                {
                    #ifdef VDP_RACE_LOG
                    VDP_FrcRacingLog('Q');
                    VDP_FrcRacingLog('0' + prDecPrm->arFBufInfo[u4DispIdx].ucFbId);
                    #endif
                    if(prDecPrm->arFBufInfo[u4DispIdx].ucFbId == prDecPrm->prCurrFBufInfo->ucFbId)
                    {
                        VERIFY(x_sema_lock(_hVdecFinSema, X_SEMA_OPTION_WAIT) == OSR_OK);    //wait decode done
                        prDecPrm->bWaitDecLock = FALSE;                        
                    }
                    //can't put to empty Q now, so set reference always
                    FBM_UpdateReferenceList(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4DispIdx].ucFbId, TRUE);
                    
                    //log put dispQ timing
                    if (STC_GetSrc(0, &rStcClk) == STC_VALID)
                    {
                        prFbmPicHdr = FBM_GetFrameBufferPicHdr(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4DispIdx].ucFbId);
                        LOG(8, "PutQ PTS(0x%x), STC(0x%x)\n", prFbmPicHdr->u4PTS, rStcClk.u4Base);
                    }
                    //notify audio start to play
                    if(!prDecPrm->fgStartPlay)
                    {
                        prFbmPicHdr = FBM_GetFrameBufferPicHdr(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4DispIdx].ucFbId);
                        if(_bNotifyAudStartPlay(prDecPrm->ucStcId, prFbmPicHdr->u4PTS))
                        {
                            prDecPrm->fgStartPlay = TRUE;
                        }
                    }  
                    FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4DispIdx].ucFbId, 
                        FBM_FB_STATUS_DISPLAYQ); 
                    prDecPrm->i4LastPOC = i4DispPOC;
                
                    FBM_GetFrameBufferAddr(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4DispIdx].ucFbId,
                        &u4YAddr, &u4CAddr);
                    LOG(9, "Put FbId %d, FrameNum %d, POC %d to DispQ, Y 0x%x, C 0x%x\n", 
                        prDecPrm->arFBufInfo[u4DispIdx].ucFbId, prDecPrm->arFBufInfo[u4DispIdx].u4FrameNum,
                        i4DispPOC, u4YAddr, u4CAddr);
                }
            }
        }while(i4DispPOC != i4MinDispPOC);
        LOG(9, ">\n");
        #endif
        *pi4RetValue = (INT32)E_VDEC_OK;
    }
    else
    {
        if(prDecPrm->fgOutDispOrder)
        {
            *pi4RetValue = (INT32)E_VDEC_DISP_NOT_READY; //display order
        }
        else
        {
            *pi4RetValue = (INT32)E_VDEC_OK;                 //decode order comparison
        }
    }

    //clear frame buffer from DPB if full
    if((prDecPrm->arFBufInfo[u4MinPOCFBufIdx].u1FBufStatus != NO_PIC)
        #ifdef FBM_DPB_CTRL
        && (prDecPrm->prCurrFBufInfo->u1FBufStatus == FRAME)
        #endif
        )
    {
        if(prDecPrm->u1RptFrm > 0)
        {
            _VDEC_ClrFBufInfo(prDecPrm, prDecPrm->u1DecFBufIdx);    
        }
        else
        {
            if(FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId) 
                == FBM_FB_STATUS_READY)
            {
                LOG(3, "PutBuf to Empty at ClrBuf\n");
                FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId, 
                    FBM_FB_STATUS_EMPTY);
            }
            _VDEC_ClrFBufInfo(prDecPrm, u4MinPOCFBufIdx);
        }    
    }

    // clear frame buffer if frameNum is duplicated
    if(prDecPrm->u4DupFrmNumId < 0xFF)
    {    	
        if(FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[prDecPrm->u4DupFrmNumId].ucFbId)!=
            FBM_FB_STATUS_DISPLAYQ)
        {
            FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[prDecPrm->u4DupFrmNumId].ucFbId, 
            FBM_FB_STATUS_EMPTY);
        }
        _VDEC_ClrFBufInfo(prDecPrm, prDecPrm->u4DupFrmNumId);
        prDecPrm->u4DupFrmNumId = 0xFF;
        LOG(3, "Clr Dup Frm_Num\n");
    }
}*/


//old output display and buffer clean at the same time
void _VDEC_OutputDispPic(VDEC_AVC_DecPrm* prDecPrm, INT32* pi4RetValue)
{
    UINT32 i;
    INT32 i4MinPOC, i4DispPOC = 0x7fffffff;
    UINT32 u4MinPOCFBufIdx = 0, u4DispIdx = 0, u4YAddr, u4CAddr;
	UINT32 u4TmpResult;
    DRV_DBASE_T* _prDbase = NULL;
    CRIT_STATE_T _rCritState;
    PARAM_MPV_T* prInfo;
    STC_CLOCK rStcClk;
    #ifdef FBM_DPB_CTRL
    FBM_PIC_HDR_T * prFbmPicHdr;
    #endif
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T* prVdecEsInfo = _VDEC_GetEsInfo(ES0);
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ES0);
    UCHAR ucFbStatus;
    MPV_EXT_INFO_T *prExtInfo;
    MPV_USERDATA_CB_T rUsrData;
    
    prExtInfo = (MPV_EXT_INFO_T*)_MPV_GetExtInfo();
    x_memset((void*)&rUsrData, 0, sizeof(MPV_USERDATA_CB_T));
    
    _prDbase = DBS_Lock(&_rCritState);
    ASSERT(_prDbase);
    VERIFY(DBS_Unlock(_prDbase, _rCritState));        
    prInfo = &(_prDbase->rMpv[0]);

    ASSERT(prDecPrm!=NULL);
    ASSERT(pi4RetValue!=NULL);
    ASSERT(prDecPrm->u4MaxFBufNum <= 18);        

    //set if need wait decode done sema in _VDEC_WaitDecDone
    prDecPrm->bWaitDecLock = TRUE;
    prDecPrm->fgDispCurDecPic = FALSE;
    
    //change dpb status
    #ifdef FBM_DPB_CTRL
    if(fgIsIDRPic(prDecPrm->u1NalUnitType))
    {
        prDecPrm->i4LastPOC = -99999;
    }    
    #endif
    // Check if DPB full
    i4MinPOC = 0x7fffffff;
    for(i=0; (i<prDecPrm->u4MaxFBufNum) && (i<18); i++)
    {
        if(prDecPrm->arFBufInfo[i].u1FBufStatus == NO_PIC)
        {
            i4MinPOC = 0x7fffffff;
            u4MinPOCFBufIdx = i;
            break;
        }
        // miew: need to take care of field empty
        else if((i4MinPOC > prDecPrm->arFBufInfo[i].i4POC) && 
            (fgIsNonRefFBuf(prDecPrm->arFBufInfo[i].u1FBufRefType, prDecPrm->arFBufInfo[i].u1TFldRefType, 
            prDecPrm->arFBufInfo[i].u1BFldRefType) || prDecPrm->fgPOCWrap))
        {
            i4MinPOC = prDecPrm->arFBufInfo[i].i4POC;
            u4MinPOCFBufIdx = i;
        }
    }
    
    if(prDecPrm->prCurrFBufInfo->u1FBufStatus == FRAME)
    {
        //wrap around case, POC disorder
        if(((i4MinPOC < prDecPrm->i4LastPOC) &&
            (FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId) 
            == FBM_FB_STATUS_DECODE)) &&
            (!prDecPrm->fgIsMMPlay))   //start point?
        {
            LOG(2, "Warning! POC %d < last POC %d, FlushDPB\n", i4MinPOC, prDecPrm->i4LastPOC);
            _VDEC_FlushDPB(prDecPrm);
            prDecPrm->i4LastPOC = -99999;
            prDecPrm->prSPS->fgSPSValid = FALSE;
            return;
        }
        //set cur decoded pic from decode state to ready state
        #ifdef VDEC_TIME_PROFILE
        prDecPrm->u4CurWaitFbMicro = 0;
        #endif
        #ifdef FBM_DPB_CTRL
        if(prDecPrm->u1RptFrm > 0)
        {
            FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId, FBM_FB_STATUS_DISPLAYQ);
            *pi4RetValue = (INT32)E_VDEC_OK;
            return;
        }        
        else if(prVdecInfo && (prVdecInfo->eCurState == VDEC_ST_PLAY_I_FRAME))
        {
            FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId, FBM_FB_STATUS_DISPLAYQ);
            _VDEC_ClrFBufInfo(prDecPrm, prDecPrm->u1DecFBufIdx);
            if (prVdecEsInfoKeep && prVdecEsInfoKeep->pfDecNfyFct)
            {
                prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag, VDEC_COND_I_FRAME_DONE, 0, 0);
            }            
            prDecPrm->i4LastPOC = -99999;
            *pi4RetValue = (INT32)E_VDEC_OK;
            return;
        }
        else
        {
            if(FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId) 
                == FBM_FB_STATUS_DECODE)
            {
                prFbmPicHdr = FBM_GetFrameBufferPicHdr(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId);
                if(prFbmPicHdr)
                {
                    prFbmPicHdr->i4TemporalRef = prDecPrm->prCurrFBufInfo->i4POC;
                }
                if(prDecPrm->fgIsMMPlay && (prDecPrm->ucSkipMode == IMode)) //decoding order = display order
                {
                    //prFbmPicHdr = FBM_GetFrameBufferPicHdr(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId);

						    #ifdef ENABLE_MULTIMEDIA
							#ifndef AVC_IRQ_DISABLE
       						 VERIFY(x_sema_lock(_hVdecFinSema,X_SEMA_OPTION_WAIT) == OSR_OK);
							u4TmpResult = AVC_VLD_READ32(RO_AVLD_ERR_ACC) & ERR_MASK_OBSV;
							if((u4TmpResult != 0) && (prVdecEsInfo->eContainerType> SWDMX_FMT_MPEG2_TS)&& (prVdecEsInfo->eContainerType< SWDMX_FMT_VC1_ES))
							{
								prDecPrm->prCurrFBufInfo->fgIsNotDisp = TRUE;
							}
							VERIFY(x_sema_unlock(_hVdecFinSema)== OSR_OK);
							#endif
							#endif
					
                    if(prFbmPicHdr == NULL)
                    {
                        LOG(2, "OutPic prFbmPicHdr NULL\n");
                    }
                    else
                    {
                        prVdecEsInfo->u4PTS = prFbmPicHdr->u4PTS;
                        if(prDecPrm->prCurrFBufInfo->fgIsNotDisp)
                        {
                            FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId, 
                                FBM_FB_STATUS_EMPTY);
                        }
                        else
                        {
                            if(VDEC_IsNotDisplay(ES0, prFbmPicHdr->u4PTS,
                                prFbmPicHdr->i4TemporalRef, prFbmPicHdr->u8Offset))
                            {
                                FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId, 
                                    FBM_FB_STATUS_EMPTY);
                            }
                            else
                            {
                                FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId, 
                                    FBM_FB_STATUS_DISPLAYQ);
                                prVdecEsInfo->u4DisplayQPicCnt++;
                            }
                            UNUSED(VDEC_CheckStopStatus(ES0, prFbmPicHdr->u4PTS, prFbmPicHdr->i4TemporalRef));
                            prDecPrm->i4LastPOC = i4DispPOC;
                         }
                    }
                }
                else
                {
                    FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId, FBM_FB_STATUS_READY);
                    LOG(9, "POC %d, FbId %d ready\n", prDecPrm->prCurrFBufInfo->i4POC, prDecPrm->prCurrFBufInfo->ucFbId);
                }
            }
            else
            {
                LOG(3, "Err!Try to set FB from %d to ready\n", 
                    (UINT32)FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId));
            }
        }
        #endif        
    }
    
    // No free FBuf, output 1 fbuf is needed
    if((prDecPrm->arFBufInfo[u4MinPOCFBufIdx].u1FBufStatus != NO_PIC) 
        #ifdef FBM_DPB_CTRL
        && (prDecPrm->prCurrFBufInfo->u1FBufStatus == FRAME)
        #endif
        )
    {
        prDecPrm->u1MinPOCBufIdx = (UINT8)u4MinPOCFBufIdx;

        #ifdef FBM_DPB_CTRL
        //FBM_UpdateReferenceList(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4MinPOCFBufIdx].ucFbId, FALSE);
        LOG(9, "<");        
        do
        {
            i4DispPOC = i4MinPOC;
            u4DispIdx = u4MinPOCFBufIdx;
            for(i=0; (i<prDecPrm->u4MaxFBufNum) && (i<18); i++)
            {
                if((i4DispPOC > prDecPrm->arFBufInfo[i].i4POC) &&
                    (FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[i].ucFbId)
                    == FBM_FB_STATUS_READY) &&
                    (prDecPrm->arFBufInfo[i].u1FBufRefType != LREF_PIC))
                {
                    i4DispPOC = prDecPrm->arFBufInfo[i].i4POC;
                    u4DispIdx = i;
                }
            }
            if(FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4DispIdx].ucFbId) 
                == FBM_FB_STATUS_READY)
            {
                FBM_UpdateReferenceList(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4DispIdx].ucFbId, 
                    ((i4DispPOC != i4MinPOC)?TRUE:FALSE));

                //wait decoding done if need to display current frame
                if(prDecPrm->arFBufInfo[u4DispIdx].ucFbId == prDecPrm->prCurrFBufInfo->ucFbId)
                {
                    VERIFY(x_sema_lock(_hVdecFinSema, X_SEMA_OPTION_WAIT) == OSR_OK);    //wait decode done
                    prDecPrm->bWaitDecLock = FALSE;
                    prDecPrm->fgDispCurDecPic = TRUE;
                }

                //calculate pts in decoder if AVI file
                prFbmPicHdr = FBM_GetFrameBufferPicHdr(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4DispIdx].ucFbId);
                if(prFbmPicHdr == NULL)
                {
                    LOG(2, "OutPic prFbmPicHdr NULL\n");
                    break;
                }
                if(prDecPrm->fgIsMMPlay)
                {
                    if(!prDecPrm->arFBufInfo[u4DispIdx].fgPTSValid)
                    {
                        prFbmPicHdr->u4PTS = prDecPrm->u4DispPts;
                    }
                    else
                    {
                        prDecPrm->u4DispPts = prFbmPicHdr->u4PTS;
                    }
                    if(prDecPrm->u4Rate > 0)
                    {
                        prDecPrm->u4DispPts += ((UINT32)u8Div6432((prDecPrm->u8PtsScale + prDecPrm->u8VRemainder), 
                            prDecPrm->u4Rate, &prDecPrm->u8VRemainder) * (prDecPrm->arFBufInfo[u4DispIdx].u4SkipFrmCnt + 1));
                    }
                }
                if(prDecPrm->i4LastPOC > i4DispPOC)    //temp before FBM modification ready
                {
                    LOG(2, "Warning! POC %d < last POC %d\n", i4DispPOC, prDecPrm->i4LastPOC);
                    prDecPrm->i4LastPOC = i4DispPOC;
                    FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4DispIdx].ucFbId, 
                        FBM_FB_STATUS_EMPTY);
                    prInfo->u4VpsrDropNs++;
                }
                else if((prDecPrm->arFBufInfo[u4DispIdx].u1FBufStatus != FRAME) || 
                    (prDecPrm->arFBufInfo[u4DispIdx].fgIsNotDisp))  //field empty, or reference error frame
                {
                    FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4DispIdx].ucFbId, 
                        FBM_FB_STATUS_EMPTY);
                    prInfo->u4VpsrDropNs++;
                }
                else
                {
                    #ifdef VDP_RACE_LOG
                    VDP_FrcRacingLog('Q');
                    VDP_FrcRacingLog('0' + prDecPrm->arFBufInfo[u4DispIdx].ucFbId);
                    #endif
                    //log put dispQ timing
                    if (STC_GetSrc(0, &rStcClk) == STC_VALID)
                    {
                        LOG(6, "PutQ PTS(0x%x), STC(0x%x), POC %d\n", 
                            prFbmPicHdr->u4PTS, rStcClk.u4Base, i4DispPOC);
                    }
                    //notify audio start to play
                    if(!prDecPrm->fgStartPlay)
                    {
                        if(prDecPrm->fgIsMMPlay)
                        {
                            prDecPrm->fgStartPlay = TRUE;
                        }
                        else
                        {
                            if(_bNotifyAudStartPlay(prDecPrm->ucStcId, prFbmPicHdr->u4PTS))
                            {
                                prDecPrm->fgStartPlay = TRUE;
                            }
                        }
                    }
                    // mm checking
                    prVdecEsInfo->u4PTS = prFbmPicHdr->u4PTS;
                    if(prDecPrm->fgIsMMPlay && VDEC_IsNotDisplay(ES0, prFbmPicHdr->u4PTS,
                        prFbmPicHdr->i4TemporalRef, prFbmPicHdr->u8Offset))
                    {
                        FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4DispIdx].ucFbId, 
                            FBM_FB_STATUS_EMPTY);
                    }
                    else
                    {
                        FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4DispIdx].ucFbId, 
                            FBM_FB_STATUS_DISPLAYQ);
                        prVdecEsInfo->u4DisplayQPicCnt++;
                        
                        if(prDecPrm->arFBufInfo[u4DispIdx].fgFirstFrmTag)   //continuous display from 1st frame
                        {
                            for(i=0; (i<prDecPrm->u4MaxFBufNum) && (i<18); i++)
                            {
                                if(prDecPrm->arFBufInfo[i].u1FBufStatus != NO_PIC)
                                {
                                    prDecPrm->arFBufInfo[i].fgIsNotDisp = FALSE;
                                }
                            }
                        }
                        prDecPrm->arFBufInfo[u4DispIdx].fgFirstFrmTag = FALSE;

                        //notify userdata
                        if(prExtInfo->pfUserDataCb &&
                            ((prExtInfo->fgUserData[ES0] && (prDecPrm->arFBufInfo[u4DispIdx].u4UserDataFrmWptr > 0)) ||
                            (prExtInfo->fgPesUserData[ES0] && (prDecPrm->arFBufInfo[u4DispIdx].u4UserDataPESWptr > 0))))
                        {
                            rUsrData.fgProgressiveFrm = prFbmPicHdr->fgProgressiveFrm;
                            rUsrData.u2TemporalRef = (UINT16)prFbmPicHdr->i4TemporalRef;
                            rUsrData.u4Pts = prFbmPicHdr->u4PTS;
                            rUsrData.ucEsId = ES0;
                            rUsrData.ucPicStruct = prFbmPicHdr->ucPicStruct;
                            rUsrData.ucPicType = prFbmPicHdr->ucPicCdTp;
                            rUsrData.ucRepFirstFld = prFbmPicHdr->fgRepFirstFld;
                            rUsrData.ucTopFldFirst = prFbmPicHdr->fgTopFldFirst;

                            #ifdef CC_SEI_USER_DATA_ENABLE
                            if(prExtInfo->fgUserData[ES0] && (prDecPrm->arFBufInfo[u4DispIdx].u4UserDataFrmWptr > 0))
                            {
                                rUsrData.u4Rptr = _VDEC_SaveUserData(prDecPrm->arFBufInfo[u4DispIdx].au1UsrDataFrm, 
                                    prDecPrm->arFBufInfo[u4DispIdx].u4UserDataFrmWptr);
                                rUsrData.ucDataLen = (UCHAR)prDecPrm->arFBufInfo[u4DispIdx].u4UserDataFrmWptr;
                                
                            if(!prDecPrm->fgDispCurDecPic)
                            {   //notify user data
                                prExtInfo->pfUserDataCb(&rUsrData);
                            }
                            else    //current frm user data !ready, wait next AU, save to Q
                            {
                                    x_msg_q_send(prDecPrm->hUsrDataQ, (void *)&rUsrData,
                                        sizeof(MPV_USERDATA_CB_T), 255);
                                }
                            }
                            #endif
                            #ifdef CC_DMX_PES_EXT_BUFFER
                            if(prExtInfo->fgPesUserData[ES0] && (prDecPrm->arFBufInfo[u4DispIdx].u4UserDataPESWptr > 0))
                            {
                                rUsrData.u4Rptr = _VDEC_SaveUserData(prDecPrm->arFBufInfo[u4DispIdx].au1UsrDataPES, 
                                    prDecPrm->arFBufInfo[u4DispIdx].u4UserDataPESWptr);
                                rUsrData.ucDataLen = (UCHAR)prDecPrm->arFBufInfo[u4DispIdx].u4UserDataPESWptr;
                                
                                if(!prDecPrm->fgDispCurDecPic)
                                {   //notify user data
                                    prExtInfo->pfUserDataCb(&rUsrData);
                                }
                                else    //current frm user data !ready, wait next AU, save to Q
                                {
                                    x_msg_q_send(prDecPrm->hUsrDataQ, (void *)&rUsrData,
                                        sizeof(MPV_USERDATA_CB_T), 255);
                                }
                            }
                            #endif
                        }
                    }
                    if(prDecPrm->fgIsMMPlay)
                    {
                        prVdecEsInfo->u4PTS = prFbmPicHdr->u4PTS;
                        UNUSED(VDEC_CheckStopStatus(ES0, prFbmPicHdr->u4PTS, prFbmPicHdr->i4TemporalRef));
                    }
                    prDecPrm->i4LastPOC = i4DispPOC;
                
                    FBM_GetFrameBufferAddr(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4DispIdx].ucFbId,
                        &u4YAddr, &u4CAddr);
                    LOG(9, "Put FbId %d, FrameNum %d, POC %d to DispQ, Y 0x%x, C 0x%x, IsRef %d\n", 
                        prDecPrm->arFBufInfo[u4DispIdx].ucFbId, prDecPrm->arFBufInfo[u4DispIdx].u4FrameNum,
                        i4DispPOC, u4YAddr, u4CAddr, ((i4DispPOC != i4MinPOC)?1:0));
                }
            }
        }while(i4DispPOC != i4MinPOC);
        LOG(9, ">\n");
        #endif
        
        if(prDecPrm->u1RptFrm > 0)
        {
            _VDEC_ClrFBufInfo(prDecPrm, prDecPrm->u1DecFBufIdx);    
        }
        else
        {
            _VDEC_ClrFBufInfo(prDecPrm, u4MinPOCFBufIdx);
        }
        *pi4RetValue = (INT32)E_VDEC_OK;
    }
    else
    {
        if(prDecPrm->fgOutDispOrder)
        {
            *pi4RetValue = (INT32)E_VDEC_DISP_NOT_READY; //display order
        }
        else
        {
            *pi4RetValue = (INT32)E_VDEC_OK;                 //decode order comparison
        }
    }
    
    // clear frame buffer if frameNum is duplicated
    if(prDecPrm->u4DupFrmNumId < 18/*0xFF*/)
    {    	
        ucFbStatus = FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[prDecPrm->u4DupFrmNumId].ucFbId);
        if((ucFbStatus!= FBM_FB_STATUS_DISPLAYQ) && (ucFbStatus!= FBM_FB_STATUS_LOCK))
        {
            FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[prDecPrm->u4DupFrmNumId].ucFbId, 
            FBM_FB_STATUS_EMPTY);
        }
        else
        {
            FBM_UpdateReferenceList(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[prDecPrm->u4DupFrmNumId].ucFbId, FALSE);
        }
        if(prDecPrm->fgOutDispOrder)    //skip this in emulation
        {
            _VDEC_ClrFBufInfo(prDecPrm, prDecPrm->u4DupFrmNumId);
        }
        prDecPrm->u4DupFrmNumId = 0xFF;
        LOG(3, "Clr Dup Frm_Num\n");
    }
}


void _VDEC_GetRptr(UINT32* pu4Rptr)
{
    ASSERT(pu4Rptr!=NULL);
    *pu4Rptr = _VDEC_VldRPtr();
}

void _VDEC_SetWptr(UINT32 u4Wptr)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();

    _VDEC_SetVldWPtr(u4Wptr, prVdecInfo->rPesInfo.u4FifoStart, prVdecInfo->rPesInfo.u4FifoEnd);
}

BOOL _VDEC_SkipFrm(const VDEC_AVC_DecPrm* prDecPrm)
{
    DRV_DBASE_T* _prDbase = NULL;
    CRIT_STATE_T _rCritState;
    PARAM_MPV_T* prInfo;
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();

    ASSERT(prDecPrm != NULL);

    _prDbase = DBS_Lock(&_rCritState);
    ASSERT(_prDbase);
    VERIFY(DBS_Unlock(_prDbase, _rCritState));        
    prInfo = &(_prDbase->rMpv[0]);
        
    prInfo->u4EsmSkipNs++;
    #ifdef FBM_DPB_CTRL
    if(((prDecPrm->prCurrFBufInfo == NULL) ||
        (prDecPrm->prSliceHdr == NULL)) ||
        (prVdecInfo && (prVdecInfo->eCurState == VDEC_ST_PLAY_I_FRAME)))
    {
        return FALSE;
    }

    if((prDecPrm->prCurrFBufInfo->u1FBufStatus == TOP_FIELD) ||
        (prDecPrm->prCurrFBufInfo->u1FBufStatus == BOTTOM_FIELD))
    {
        if(fgIsNonRefFBuf(prDecPrm->prCurrFBufInfo->u1FBufRefType, 
        prDecPrm->prCurrFBufInfo->u1TFldRefType, 
        prDecPrm->prCurrFBufInfo->u1BFldRefType))
        {
            LOG(3, "Put Non-Ref Frm to Empty in SkipFrm\n");
            prDecPrm->prCurrFBufInfo->fgIsNotDisp = TRUE;
            prDecPrm->prCurrFBufInfo->u1FBufStatus = FRAME;
            FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, 
                prDecPrm->prCurrFBufInfo->ucFbId, FBM_FB_STATUS_READY);
            LOG(3, "Skip B\n");
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else if(prDecPrm->prSliceHdr->fgFieldPicFlag)   //field encoded
    {
        return FALSE;
    }
    else
    {
        prDecPrm->prCurrFBufInfo->u4SkipFrmCnt++;
        LOG(3, "Skip B\n");
        return TRUE;
    }
        
    #endif
}

void _VDEC_FrmFin(VDEC_AVC_DecPrm* prDecPrm)
{
    UINT32 u4ErrAcc = 0, u4PrevAddr = 0, u4PList0 = 0;
    
    ASSERT(prDecPrm!=NULL);
    
    //save decode time
    prDecPrm->u4CurDecMs += (rTimeHwDt.u4Micros / 1000);
    #ifdef VDEC_TIME_PROFILE
    prDecPrm->u4CurDecCycle += u4DecCycle;
    prDecPrm->u4CurDramCycle += u4DramCycle;
    #endif

    if(prDecPrm->prSliceHdr->fgFieldPicFlag)
    {
        return; //need more test on field encoded bitstream
    }    
    //check error type
    u4ErrAcc = AVC_VLD_READ32(RO_AVLD_ERR_ACC) & ERR_MASK_OBSV;
    if((!prDecPrm->fgIsMMPlay) &&
        (((u4ErrAcc & ERR_DISP_MASK) > 0) && 
        (prDecPrm->prCurrFBufInfo->u1FBufStatus != NO_PIC)))
    {
        prDecPrm->prCurrFBufInfo->fgIsNotDisp = TRUE;

        //copy previous frame if IDR is broken, sw workaround
        if(prDecPrm->u1NalUnitType == IDR_SLICE)
        {
            u4PList0 = MC_READ32(RW_MC_P_LIST0) & 0xFFFFFFF;
            u4PrevAddr = VIRTUAL(u4PList0);
            //check null pointer
            if(((u4PList0 > 0) && (prDecPrm->prCurrFBufInfo->u4YAddr > 0)) &&
                (prDecPrm->prCurrFBufInfo->u4CAddr > 0))
            {
                x_memcpy((void*)VIRTUAL(prDecPrm->prCurrFBufInfo->u4YAddr), 
                    (void*)u4PrevAddr, 
                    prDecPrm->prCurrFBufInfo->u4DramPicSize);
                x_memcpy((void*)VIRTUAL(prDecPrm->prCurrFBufInfo->u4CAddr), 
                    (void*)(u4PrevAddr + MC_READ32(RW_MC_CBCR_OFFSET)), 
                    (prDecPrm->prCurrFBufInfo->u4DramPicSize >> 1));
            }
        }
        /*if((u4ErrAcc & ERR_REF_MASK) > 0)
        {
            prDecPrm->prCurrFBufInfo->fgIsErr = TRUE;
            prDecPrm->prCurrFBufInfo->u4YAddr |= FBUF_ERR_MASK;
        }*/        
    }
}

UINT32 _VDEC_SaveUserData(const UINT8* pu1UserData, UINT32 u4Size)
{
    MPV_EXT_INFO_T *prExtInfo;
    UINT32 u4Sz = 0, u4CpSz = 0, u4Rptr =0;

    if(pu1UserData == NULL)
    {
        LOG(2, "Null UserDataPtr\n");
        return 0;
    }
    prExtInfo = (MPV_EXT_INFO_T*)_MPV_GetExtInfo();

    u4Rptr = prExtInfo->u4UserDataWptr;
    u4Sz = prExtInfo->u4UserDataWptr + u4Size;

    u4CpSz = (u4Sz < MAX_USERDATA_SIZE) ? u4Size : (MAX_USERDATA_SIZE - prExtInfo->u4UserDataWptr);
    x_memcpy((void*)&prExtInfo->aucUserDataBuf[prExtInfo->u4UserDataWptr], 
        (void*)pu1UserData, 
        u4CpSz);
    prExtInfo->u4UserDataWptr += u4CpSz;
    if(prExtInfo->u4UserDataWptr == MAX_USERDATA_SIZE)
    {
        prExtInfo->u4UserDataWptr = 0;
    }

    if(u4CpSz < u4Size) // ring data
    {
        u4Sz = u4Size - u4CpSz;
        x_memcpy((void*)prExtInfo->aucUserDataBuf, 
            (void*)(pu1UserData + u4CpSz), 
            u4Sz);
        prExtInfo->u4UserDataWptr = u4Sz;
    }

    return u4Rptr;
}
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------



