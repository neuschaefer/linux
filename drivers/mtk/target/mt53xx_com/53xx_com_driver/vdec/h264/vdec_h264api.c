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
 * $RCSfile: vdec_h264api.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_h264api.c
 *  This file contains implementation of exported APIs of VDEC.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_timer.h"
#include "x_lint.h"
#include "vdec_h264psr.h"
#include "vdec_h264dec.h"
#include "vdec_h264util.h"
#include "vdec_h264api.h"
#include "vdec_h264hw.h"
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
#ifdef CC_DRIVER_DEMO
#include "vdec_h264hw.h"
#endif
#include "vdec_drvif.h"
#include "drv_dbase.h"
//for user data
#include "mpv_drvif.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#ifdef MEM_TEST
#define DPB_ADDR 0x679c000
#else
#define DPB_ADDR 0x179c000
#endif

#define DPB_SIZE 0xC00000//0x1200000
//actual used DPB_SIZE 0x01680000;    //size (12288*1024)=0xC00000, + cur dec 3M, + cur disp 3M,  *5/4=0x01680000(MV buffer)
#define PRED_SA 0x7ff0000//(DPB_ADDR + ((DPB_SIZE * 5)>>2))         //buffer for VLD, size 32*1024
#define PRED_SA_SZ 0x8000    //size 32*1024

#define MAX_ES_NUM 1
#define ES_ID_0 0

#define IS_NREF_NALU(x) (((x >> 5) & 0x3) == 0)
#define IS_PIC_NALU(x) (((x & 0x1f) <= IDR_SLICE))
#define IPBMode 0x0
#define IPMode 0x1
#define IMode 0x2

#define ATSC_IDENTIFIER_LE  0x34394147
#define EIA_CC    0x03
#define MPEG2_USER_DATA_SC 0x000001b2   // start code

#define FIND_START_CODE_THRD 32
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static VDEC_H264_INFO_T rH264DecInfo[MAX_ES_NUM];

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
INT32 _VDEC_H264Init()
{
    VDEC_H264_INFO_T* prVdec;
    static UINT32 u4PredSA = 0;
    UINT32 i;
    
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ES_ID_0];

    if(u4PredSA == 0)
    {
        u4PredSA = (UINT32)BSP_AllocAlignedDmaMemory(PRED_SA_SZ, 1024);
        VERIFY(x_msg_q_create(&prVdec->rDecParam.hUsrDataQ, "AVC_UserDataQ",
            sizeof(MPV_USERDATA_CB_T), USERDATA_Q_SIZE) == OSR_OK);
    }
    prVdec->bPicQueue = FALSE;
    prVdec->rDecParam.u4MaxFBufNum = 18;
    prVdec->rDecParam.rLastInfo.u4LastFrameNum = 0xffffffff;
    prVdec->rDecParam.rLastInfo.u4LastPicW = 0;
    prVdec->rDecParam.rLastInfo.u4LastPicH = 0;
    prVdec->rDecParam.rLastInfo.u4LastSPos = 0;
    prVdec->rDecParam.rLastInfo.fgLastIsRef = FALSE;
    prVdec->rDecParam.fgPOCWrap = FALSE;
    prVdec->rDecParam.fgStartPlay = FALSE;
    prVdec->rDecParam.fgIsFirstFrm = FALSE;

    //memory layout, should query from FBM
    prVdec->rDecParam.u4PredSA = u4PredSA;    //PRED_SA;   
    prVdec->rDecParam.u4DPBAddr = DPB_ADDR;    //not used if FBM_DPB_CTRL
    prVdec->rDecParam.u4DPBSize = DPB_SIZE;    //not used if FBM_DPB_CTRL
    //prVdec->rPesInfo.u4FifoStart = 0;            //init value, will get from pes queue later
    //prVdec->rPesInfo.u4FifoEnd = 0xFFFF;        //init value, will get from pes queue later
    prVdec->rDecParam.ucFbgId = FBM_FBG_ID_UNKNOWN;
    
    //wait fb counter
    prVdec->rDecParam.u4WaitFbCnt = 0;
    prVdec->rDecParam.fgAllocFbFail = FALSE;
    
    //RR mode
    prVdec->rDecParam.fgRRModeYEn = FALSE;
    prVdec->rDecParam.fgRRModeCEn = FALSE;
    prVdec->rDecParam.u1RRModeY = 0;
    prVdec->rDecParam.u1RRModeC = 0;
    prVdec->rDecParam.u1MaxDisHbands = 3;

    //Display order
    prVdec->rDecParam.fgOutDispOrder = TRUE;
    prVdec->rDecParam.i4LastPOC = -99999;
    prVdec->rDecParam.u4Dts = 0;
    
    //swdmx will set the param before vdec init
    if((!prVdec->rDecParam.fgIsMMPlay) || (!prVdec->rDecParam.fgEnCalPTS))
    {
        prVdec->rDecParam.u4DispPts = 0;
        prVdec->rDecParam.u8VRemainder = 0;
        prVdec->rDecParam.u8PtsScale = 0;
        prVdec->rDecParam.u4Rate = 0;
        prVdec->rDecParam.fgEnCalPTS = FALSE;
    }
    
    //Reset Wptr
    prVdec->rDecParam.u4WPtr = 0xF000000;

    //emulation, repeat frame
    prVdec->rDecParam.u1RptFrm = 0;

    //error cocealment mode
    prVdec->rDecParam.u1ECMode = (UINT8)((ERC_ENABLE | ERC_DED_ERR) | (ERC_MODE1 << 2));   //EC to slice start pos
    prVdec->rDecParam.u4ECType = 0xC210;//0x4200;
    
    //dram bandwidth simulation
    prVdec->rDecParam.u4DramMaxLen = 0;
    prVdec->rDecParam.u4DramReqConnDis = 0;
    prVdec->rDecParam.u4DramFifoBank = 0;   //16 bank
    prVdec->rDecParam.fgLogSimPat = FALSE;
    prVdec->rDecParam.u1DramMoniter = 0;

    //waiting for 1st IDR
    #ifdef EARLY_DISP
    prVdec->rDecParam.fgWaitFirstIDR = TRUE;
    #endif

    //mm playback
    prVdec->rDecParam.fgIsMMPlay = FALSE;
    prVdec->rDecParam.fgNotSupport = FALSE;
    prVdec->rDecParam.ucSkipMode = IPBMode;
    
    //flush buf info    
    prVdec->rDecParam.u1MaxLongTermFrameIdx = 0xff;
    for(i=0; i<18; i++)
    {
        prVdec->rDecParam.arFBufInfo[i].ucMvBufIdx = 0xff;
        _VDEC_ClrFBufInfo(&prVdec->rDecParam, i);
    }
    for(i=0; i<6; i++)
    {        
            prVdec->rDecParam.arRefPicList[i].u4RefPicCnt = 0;
    }

    //flush set data
    for(i=0; i<32; i++)
    {
        prVdec->arSPS[i].fgSPSValid = FALSE;
    }

    for(i=0; i<256; i++)
    {
        prVdec->arPPS[i].fgPPSValid = FALSE;    
    }
    
    //MV buffer array
    for(i=0; i<17; i++)
    {
        prVdec->rDecParam.aucEmpMVBufIdxMap[i] = (UCHAR)i;
    }

    prVdec->rDecParam.prSPS = NULL;
    prVdec->rDecParam.prPPS = NULL;
    prVdec->rDecParam.prSliceHdr = NULL;
    prVdec->rDecParam.prCurrFBufInfo = NULL;
    prVdec->rDecParam.rSEI.u1ExtMatchFlg = FALSE;
    prVdec->rDecParam.rSEI.u1WaitRAPCnt = 0;
    prVdec->rDecParam.rSEI.u1WaitRAPRefCnt = 0;
    prVdec->rDecParam.rSEI.u1WaitRAPICnt = 0;
    prVdec->rDecParam.rSEI.u4UserDataWptr = 0;
    prVdec->rDecParam.u4PESUserDataWptr = 0;
    prVdec->rDecParam.rSEI.u1PicStruct = (UINT8)SEI_FRAME_UNKNOWN;
    prVdec->rDecParam.fgDispCurDecPic = FALSE;
        
    //init hw module, fifo address is 0 now
    _VDEC_InitVDecHW(TRUE);

    return (INT32)E_VDEC_OK;
}

/** unit of es packet is an access unit.
*@param .
 */
INT32 _VDEC_H264HdrPrsPic(UCHAR ucVdecId, UCHAR ucEsId)
{
    VDEC_H264_INFO_T* prVdec;
    UINT8 u1NaluInfo;
    INT32 i4RetValue = (INT32)E_VDEC_OK;
    BOOL fgForbidenZeroBits;
    
    UNUSED(ucVdecId);    
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];

    do
    {
        if(prVdec->u1NalUnitType > IDR_SLICE)
        {
            _VDEC_SetBsInfo(BARREL2, prVdec->rPesInfo.u4VldReadPtr, prVdec->rDecParam.u4WPtr,
                prVdec->rPesInfo.u4FifoStart, prVdec->rPesInfo.u4FifoEnd, prVdec->rDecParam.u4PredSA,
                prVdec->rDecParam.u1ECMode, prVdec->rDecParam.u4ECType);
            u1NaluInfo = _VDEC_AVCGetStartCode(BARREL2);        //find start code
        }
        else
        {        
            _VDEC_SetBsInfo(BARREL1, prVdec->rPesInfo.u4VldReadPtr, prVdec->rDecParam.u4WPtr, 
                prVdec->rPesInfo.u4FifoStart, prVdec->rPesInfo.u4FifoEnd, prVdec->rDecParam.u4PredSA,
                prVdec->rDecParam.u1ECMode, prVdec->rDecParam.u4ECType);            
            u1NaluInfo = _VDEC_AVCGetStartCode(BARREL1);        //find start code
        }

        fgForbidenZeroBits = ((u1NaluInfo >> 7) & 0x01);    // bit 31
        if(fgForbidenZeroBits != 0)
        {
            //vErrInfo(FORBIDEN_ERR);
        }
        prVdec->u1NalRefIdc = ((u1NaluInfo >> 5) & 0x3);    // bit 30,29
        prVdec->u1NalUnitType = (u1NaluInfo & 0x1f);        // bit 28,27,26,25,24
        prVdec->rDecParam.u1NalRefIdc = prVdec->u1NalRefIdc;
        prVdec->rDecParam.u1NalUnitType = prVdec->u1NalUnitType;
        
        switch(prVdec->u1NalUnitType)
        {
            case NON_IDR_SLICE:
            case IDR_SLICE:
                _VDEC_SliceNonIDR(prVdec, &i4RetValue);
                //record slice reference type for new picture detect
                prVdec->rDecParam.rLastInfo.fgLastIsRef = fgIsRefPic(prVdec->rDecParam.u1NalRefIdc);                
                break;
            case SEI:
                //_VDEC_SEIrbsp(prVdec, &i4RetValue);
                i4RetValue = (INT32)E_VDEC_OK;
                break;
            case SPS:
                _VDEC_SPSrbsp(prVdec, &i4RetValue);
                break;      
            case PPS:
                _VDEC_PPSrbsp(prVdec, &i4RetValue);
                break;
            case AU_DELIMITER:
                break;
            default:
                break;
        }

        if(i4RetValue != (INT32)E_VDEC_OK)
        {
            return i4RetValue;
        }
    }while(!((prVdec->u1NalUnitType == NON_IDR_SLICE) || (prVdec->u1NalUnitType == IDR_SLICE)));

    return (INT32)E_VDEC_OK;
}


/** unit of es packet is a nalu.
*@param .
 */
INT32 _VDEC_H264HdrPrsNalu(UCHAR ucVdecId, UCHAR ucEsId)
{
    VDEC_H264_INFO_T* prVdec;
    VDEC_ES_INFO_T *prVdecEsInfo;
    UINT8 u1NaluInfo, u1NalRefIdc, u1NalUnitType;
    BOOL fgForbidenZeroBits;
    INT32 i4RetValue = (INT32)E_VDEC_OK;
    static UINT32 u4LastPtr = 0;
    #ifdef VDEC_TIME_PROFILE
    HAL_TIME_T rTimeRstS, rTimeRstE, rTimeRstDt;    
    #endif
    DRV_DBASE_T* _prDbase = NULL;
    CRIT_STATE_T _rCritState;
    PARAM_MPV_T* prInfo;
    VDEC_FBUF_INFO_T* prCurrFBufInfo;
    MPV_EXT_INFO_T *prExtInfo;
    MPV_USERDATA_CB_T rUsrData;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize = sizeof(MPV_USERDATA_CB_T);
    FBM_PIC_HDR_T * prFbmPicHdr;
    VDEC_INFO_T* prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    UINT32 u4CurRptr, u4TimeOutCnt = 0;
    #ifdef CC_DMX_PES_EXT_BUFFER
    UINT32 u4DataLen1 = 0, u4DataLen2 = 0, u4Wptr = 0, u4Rptr = 0;
    UINT32 u4DataIn = 0;
    #endif
    
    prExtInfo = (MPV_EXT_INFO_T*)_MPV_GetExtInfo();
    
    _prDbase = DBS_Lock(&_rCritState);
    ASSERT(_prDbase);
    VERIFY(DBS_Unlock(_prDbase, _rCritState));        
    prInfo = &(_prDbase->rMpv[0]);

    UNUSED(ucVdecId);
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    UINT16 i = 0;

    //wrap around handle
    /*u4PtsDiff = (prVdec->rDecParam.u4Pts > prVdec->rPesInfo.u4PTS)?
                    (prVdec->rDecParam.u4Pts - prVdec->rPesInfo.u4PTS):
                    (prVdec->rPesInfo.u4PTS - prVdec->rDecParam.u4Pts);
    if(u4PtsDiff > 0x16800)
    {
        LOG(2, "Pts Gap > 90K\n");
        _VDEC_FlushDPB(&prVdec->rDecParam);
        
        prVdec->rDecParam.rLastInfo.u4LastSPos = 0;
        prVdec->rDecParam.i4LastPOC = -99999;
        
        for(u2Idx=0; u2Idx<17; u2Idx++)
        {
            prVdec->rDecParam.aucEmpMVBufIdxMap[u2Idx] = u2Idx;
        }
        prVdec->rDecParam.ucEmpMVBufMaxNum = prVdec->rDecParam.u4MaxFBufNum;
        //flush set data
        for(u2Idx=0; u2Idx<32; u2Idx++)
        {
            prVdec->arSPS[u2Idx].fgSPSValid = FALSE;
        }
    
        for(u2Idx=0; u2Idx<256; u2Idx++)
        {
            prVdec->arPPS[u2Idx].fgPPSValid = FALSE;    
        }      
    }*/
    u1NaluInfo = prVdec->rPesInfo.ucPicType;            //find start code
    if(u1NaluInfo == 0)
    {
        if(prVdec->rDecParam.fgIsMMPlay)
        {
            return (INT32)E_VDEC_PARSE_NOT_READY;
        }
        else
        {
            _VDEC_FlushDPB(&prVdec->rDecParam);
            return (INT32)E_VDEC_BS_END;
        }
    }
    fgForbidenZeroBits = ((u1NaluInfo >> 7) & 0x01);    // bit 31
    if(fgForbidenZeroBits != 0)
    {
        //vErrInfo(FORBIDEN_ERR);
    }
    u1NalRefIdc = ((u1NaluInfo >> 5) & 0x3);    // bit 30,29
    u1NalUnitType = (u1NaluInfo & 0x1f);        // bit 28,27,26,25,24
    
    prVdec->u1NalRefIdc = u1NalRefIdc;
    prVdec->u1NalUnitType = u1NalUnitType;
    prVdec->rDecParam.u1NalRefIdc = prVdec->u1NalRefIdc;
    prVdec->rDecParam.u1NalUnitType = prVdec->u1NalUnitType;

    if(u4LastPtr > (prVdec->rPesInfo.u4VldReadPtr - prVdec->rPesInfo.u4FifoStart))
    {
        LOG(2, "--Nalu type %d, offset 0x%x, wptrOffset 0x%x\n", prVdec->u1NalUnitType, 
            prVdec->rPesInfo.u4VldReadPtr - prVdec->rPesInfo.u4FifoStart,
            prVdec->rDecParam.u4WPtr - prVdec->rPesInfo.u4FifoStart);
    }
    //u4LastPtr = (prVdec->rPesInfo.u4VldReadPtr - prVdec->rPesInfo.u4FifoStart);
    if(prVdecEsInfo->u1PVRWaitSps == 1)
    {         
        for(i=0; i<32; i++)
        {
            prVdec->arSPS[i].fgSPSValid = FALSE;
        }

        for(i=0; i<256; i++)
        {
            prVdec->arPPS[i].fgPPSValid = FALSE;    
        }
        prVdecEsInfo->u1PVRWaitSps = 2;
    }
    
    #ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&rTimeRstS);
    #endif
    if(prVdec->u1NalUnitType <= IDR_SLICE)
    {
        //fix erc mode to slice mode
        if(prVdec->rDecParam.u1SliceCnt == prVdec->rPesInfo.u1SliceCount)
        {
            //if(prVdec->rDecParam.u1SliceCnt > SLICE_CNT_THRD)
            //{
                //prVdec->rDecParam.u1ECMode = ((ERC_ENABLE | ERC_DED_ERR) | (ERC_MODE1 << 2));
                prVdec->rDecParam.u4ECType = 0x8210;
           // }
           // else
            //{
               //prVdec->rDecParam.u1ECMode = ((ERC_ENABLE | ERC_DED_ERR) | (ERC_MODE2 << 2));
              // prVdec->rDecParam.u4ECType = 0xC210;
           // }
        }
        else    //unknown slice cnt
        {
            prVdec->rDecParam.u1ECMode = (UINT8)((ERC_ENABLE | ERC_DED_ERR) | (ERC_MODE2 << 2));
            prVdec->rDecParam.u1SliceCnt = prVdec->rPesInfo.u1SliceCount;
        }
        _VDEC_SetBsInfo(BARREL1, prVdec->rPesInfo.u4VldReadPtr, prVdec->rDecParam.u4WPtr, 
            prVdec->rPesInfo.u4FifoStart, prVdec->rPesInfo.u4FifoEnd, prVdec->rDecParam.u4PredSA,
            prVdec->rDecParam.u1ECMode, prVdec->rDecParam.u4ECType);
    }
    else if(prVdec->u1NalUnitType <= END_OF_SEQ)
    {        
        _VDEC_SetBsInfo(BARREL2, prVdec->rPesInfo.u4VldReadPtr, prVdec->rDecParam.u4WPtr, 
            prVdec->rPesInfo.u4FifoStart, prVdec->rPesInfo.u4FifoEnd, prVdec->rDecParam.u4PredSA,
            0, prVdec->rDecParam.u4ECType);            
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
    prVdec->rDecParam.rLastInfo.u4LastNaluType = prVdec->u1NalUnitType;
    switch(prVdec->u1NalUnitType)
    {
        case NON_IDR_SLICE:
        case IDR_SLICE:
            if(prVdec->rDecParam.u1RptFrm == 1)
            {
                prVdec->rDecParam.u1RptFrm = 2;
            }
            _VDEC_SliceNonIDR(prVdec, &i4RetValue);

            //record slice reference type for new picture detect
            prVdec->rDecParam.rLastInfo.fgLastIsRef = fgIsRefPic(prVdec->rDecParam.u1NalRefIdc);

            //find access unit start 7.4.1.2.3, start dec last picture queue, and restore pes info in queue
            if((i4RetValue != (INT32)E_VDEC_OK) && (i4RetValue != (INT32)E_VDEC_OK_RES_CHG))
            //{
            //    prVdec->rDecParam.rLastInfo.u4LastSPos = prVdec->rPesInfo.u4VldReadPtr;
            //    return i4RetValue;
            //}
            //selse
            {
                prInfo->u4VpsrDropNs++;
                if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, prVdec->rPesInfo.u4VldReadPtr, prVdec->rPesInfo.u4VldReadPtr))                
                {                    
                    LOG(1, "H264 ES(%d) DMX_UpdVRPtr Fail\n");
                }
            }
            break;
        case SEI:
            _VDEC_SEIrbsp(prVdec, &i4RetValue);
            //*pi4RetValue = (INT32)E_VDEC_OK;
            break;
        case SPS:
            _VDEC_SPSrbsp(prVdec, &i4RetValue);
            break;      
        case PPS:
            _VDEC_PPSrbsp(prVdec, &i4RetValue);
            break;
        case AU_DELIMITER:
            //save user data from sei to current frame
            prCurrFBufInfo = prVdec->rDecParam.prCurrFBufInfo;
            if(prCurrFBufInfo)
            {
                if(prVdec->rDecParam.fgDispCurDecPic)
                {
                    // 1. copy data, notify current data in SEI, output ring buffer
                    // 2. get all msg from Q and notify
                    if(prExtInfo->pfUserDataCb &&
                        ((prExtInfo->fgUserData[ES0] && (prVdec->rDecParam.rSEI.u4UserDataWptr > 0)) ||
                        (prExtInfo->fgPesUserData[ES0] && (prVdec->rDecParam.u4PESUserDataWptr > 0))))
                    {
                        prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdec->rDecParam.ucFbgId,
                            prCurrFBufInfo->ucFbId);
                        if(prFbmPicHdr != NULL)
                        {
                            rUsrData.fgProgressiveFrm = prFbmPicHdr->fgProgressiveFrm;
                            rUsrData.u2TemporalRef = (UINT16)prFbmPicHdr->i4TemporalRef;
                            rUsrData.u4Pts = prFbmPicHdr->u4PTS;
                            rUsrData.ucEsId = ES0;
                            rUsrData.ucPicStruct = prFbmPicHdr->ucPicStruct;
                            rUsrData.ucPicType = prFbmPicHdr->ucPicCdTp;
                            rUsrData.ucRepFirstFld = prFbmPicHdr->fgRepFirstFld;
                            rUsrData.ucTopFldFirst = prFbmPicHdr->fgTopFldFirst;
                        }                        
                        
                        #ifdef CC_SEI_USER_DATA_ENABLE
                        if(prExtInfo->fgUserData[ES0] && (prVdec->rDecParam.rSEI.u4UserDataWptr > 0))
                        {
                            rUsrData.u4Rptr = _VDEC_SaveUserData(prVdec->rDecParam.rSEI.au1UsrData, 
                                prVdec->rDecParam.rSEI.u4UserDataWptr);
                            rUsrData.ucDataLen = (UCHAR)prVdec->rDecParam.rSEI.u4UserDataWptr;                        
                            prExtInfo->pfUserDataCb(&rUsrData);
                        }
                        #endif
                        #ifdef CC_DMX_PES_EXT_BUFFER
                        if(prExtInfo->fgPesUserData[ES0] && (prVdec->rDecParam.u4PESUserDataWptr > 0))
                        {
                            rUsrData.u4Rptr = _VDEC_SaveUserData(prVdec->rDecParam.au1PESUsrData, 
                                prVdec->rDecParam.u4PESUserDataWptr);
                            rUsrData.ucDataLen = (UCHAR)prVdec->rDecParam.u4PESUserDataWptr;                        
                            prExtInfo->pfUserDataCb(&rUsrData);
                        }
                        #endif
                        
                        while (x_msg_q_receive(&u2MsgQIdx, &rUsrData, &zMsgSize,
                            &(prVdec->rDecParam.hUsrDataQ), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
                        {
                            prExtInfo->pfUserDataCb(&rUsrData);
                        }
                    }
                }
                else
                {
                    if((prCurrFBufInfo->u1FBufStatus != NO_PIC) && 
                        ((prVdec->rDecParam.rSEI.u4UserDataWptr > 0) ||
                        (prVdec->rDecParam.u4PESUserDataWptr > 0)))
                    {
                        #ifdef CC_SEI_USER_DATA_ENABLE
                        if(prVdec->rDecParam.rSEI.u4UserDataWptr > 0)
                        {
                            if((prCurrFBufInfo->u4UserDataFrmWptr + prVdec->rDecParam.rSEI.u4UserDataWptr) < MAX_USERDATA_SIZE)
                    {
                        x_memcpy((void*)&prCurrFBufInfo->au1UsrDataFrm[prCurrFBufInfo->u4UserDataFrmWptr], 
                                (void*)prVdec->rDecParam.rSEI.au1UsrData, 
                                prVdec->rDecParam.rSEI.u4UserDataWptr);
                        prCurrFBufInfo->u4UserDataFrmWptr += prVdec->rDecParam.rSEI.u4UserDataWptr; // for field encoded
                            }
                            else
                            {
                                LOG(3, "Frame-based SEI_CC_Len > %d", MAX_USERDATA_SIZE);
                            }                            
                        }
                        #endif
                        #ifdef CC_DMX_PES_EXT_BUFFER
                        if(prVdec->rDecParam.u4PESUserDataWptr > 0)
                        {
                            if((prCurrFBufInfo->u4UserDataPESWptr + prVdec->rDecParam.u4PESUserDataWptr) < MAX_USERDATA_SIZE)
                            {                        
                                x_memcpy((void*)&prCurrFBufInfo->au1UsrDataPES[prCurrFBufInfo->u4UserDataPESWptr], 
                                        (void*)prVdec->rDecParam.au1PESUsrData, 
                                        prVdec->rDecParam.u4PESUserDataWptr);
                                prCurrFBufInfo->u4UserDataPESWptr += prVdec->rDecParam.u4PESUserDataWptr; // for field encoded
                            }
                            else
                            {
                                LOG(3, "Frame-based PES_EXT_Len > %d", MAX_USERDATA_SIZE);
                            }                            
                        }
                        #endif                        
                    }
                }
            }
            prVdec->rDecParam.rSEI.u4UserDataWptr = 0;
            prVdec->rDecParam.u4PESUserDataWptr = 0;
            i4RetValue = (INT32)E_VDEC_OK;
            break;
        case END_OF_SEQ:
            LOG(6, "//Nalu END_OF_SEQ\n");
            _VDEC_FlushDPB(&prVdec->rDecParam);
            break;            
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
    
    #ifdef CC_DMX_PES_EXT_BUFFER
    u4Wptr = prVdec->rPesInfo.u4PesExtBufCurrentWp;
    u4Rptr = prVdec->rPesInfo.u4PesExtBufCurrentRp;
    
    ASSERT((u4Rptr >= prVdec->rPesInfo.u4PesExtBufStart) &&
        (u4Rptr <= prVdec->rPesInfo.u4PesExtBufEnd));
    ASSERT((u4Wptr >= prVdec->rPesInfo.u4PesExtBufStart) &&
        (u4Wptr <= prVdec->rPesInfo.u4PesExtBufEnd));
    
    if(u4Rptr > 0)
    {
        u4Rptr += 4;    // skip MPEG2 SC
        if(u4Rptr > prVdec->rPesInfo.u4PesExtBufEnd)
        {
            u4Rptr -= (prVdec->rPesInfo.u4PesExtBufEnd - prVdec->rPesInfo.u4PesExtBufStart);
        }
    
        if(u4Wptr > u4Rptr)
        {
            u4DataLen1 = u4Wptr - u4Rptr;
        }
        else
        {
            u4DataLen1 = prVdec->rPesInfo.u4PesExtBufEnd - u4Rptr;
            u4DataLen2 = u4Wptr - prVdec->rPesInfo.u4PesExtBufStart;
        }

        if((prVdec->rDecParam.u4PESUserDataWptr + (u4DataLen1 + u4DataLen2)) >= MAX_USERDATA_SIZE)
        {
            LOG(1, "UsrData in PES len %d > max %d\n", prVdec->rDecParam.u4PESUserDataWptr, MAX_USERDATA_SIZE);
            prVdec->rDecParam.u4PESUserDataWptr = 0;
        }
        else if((u4DataLen1 + u4DataLen2) > 5)  // ATSC_IDENT + EIA_CC
        {
            if(u4DataLen2 == 0)
            {
                x_memcpy((void*)VIRTUAL((UINT32)&prVdec->rDecParam.au1PESUsrData[prVdec->rDecParam.u4PESUserDataWptr]),
                    (void*)VIRTUAL(u4Rptr),
                    u4DataLen1);
            }
            else
            {
                x_memcpy((void*)VIRTUAL((UINT32)&prVdec->rDecParam.au1PESUsrData[prVdec->rDecParam.u4PESUserDataWptr]),
                    (void*)VIRTUAL(u4Rptr),
                    u4DataLen1);
                x_memcpy((void*)VIRTUAL((UINT32)&prVdec->rDecParam.au1PESUsrData[prVdec->rDecParam.u4PESUserDataWptr + u4DataLen1]),
                    (void*)VIRTUAL(prVdec->rPesInfo.u4PesExtBufStart),
                    u4DataLen2);
            }
            //verify if user data
            u4DataIn = *((UINT32*)&prVdec->rDecParam.au1PESUsrData[prVdec->rDecParam.u4PESUserDataWptr]);
            if((u4DataIn == ATSC_IDENTIFIER_LE) &&
                (prVdec->rDecParam.au1PESUsrData[prVdec->rDecParam.u4PESUserDataWptr + 4] == EIA_CC))
            {
                prVdec->rDecParam.u4PESUserDataWptr += (u4DataLen1 + u4DataLen2);
            }
        }
        DMX_UpdateExtBufferReadPointer(prVdec->rPesInfo.ucEsId, 
            u4Wptr,
            u4Rptr);
    }
    #endif

    if(i4RetValue != (INT32)E_VDEC_OK)
    {
        prVdecInfo->arVdecEsInfo[ucEsId].arRcvPesCnt[VDEC_FMT_H264]--;
    }
    else if((i4RetValue == (INT32)E_VDEC_OK) && (prVdec->u1NalUnitType > IDR_SLICE))
    {
        i4RetValue = (INT32)E_VDEC_PARSE_NOT_READY;
        //I frame case
        if(prVdecInfo && (prVdecInfo->eCurState == VDEC_ST_PLAY_I_FRAME))
        {
            _VDEC_GetRptr(&u4CurRptr);
            //find start code position
            while(!((*((UINT8*)VIRTUAL(u4CurRptr - 2)) == 0) &&
                (*((UINT8*)VIRTUAL(u4CurRptr - 1)) == 0) &&
                (*((UINT8*)VIRTUAL(u4CurRptr)) == 0x1)))
            {
                u4CurRptr++;
                u4TimeOutCnt++;
                if(u4TimeOutCnt >= FIND_START_CODE_THRD)
                {
                    break;
                }
            }
            if(u4TimeOutCnt < FIND_START_CODE_THRD)
            {
                prVdec->rPesInfo.ucPicType = *((UINT8*)VIRTUAL(u4CurRptr + 1));
                prVdec->rPesInfo.u4VldReadPtr = u4CurRptr + 2;
                VDEC_SendEs(&prVdec->rPesInfo);
            }
        }
    }

    prVdec->rDecParam.rLastInfo.u4LastSPos = prVdec->rPesInfo.u4VldReadPtr;    
    return i4RetValue;
}

/** .
*@param .
 */
INT32 _VDEC_H264DecStart(UCHAR ucVdecId, UCHAR ucEsId)
{
    VDEC_H264_INFO_T* prVdec;
    INT32 i4RetValue;
    
    UNUSED(ucVdecId);
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];
        
    _VDEC_DecProc(&prVdec->rDecParam, &i4RetValue);
    return i4RetValue;
}

/** .
*@param .
 */
INT32 _VDEC_H264DecFin(UCHAR ucVdecId, UCHAR ucEsId)
{
    VDEC_H264_INFO_T* prVdec;
    UINT8 u1Ret;
    INT32 i4RetValue;
    VDEC_INFO_T* prVdecInfo = _VDEC_GetInfo();
    UINT32 u4CurRptr, u4TimeOutCnt = 0;
 
	UNUSED(ucVdecId);
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];
    
    if(prVdec->rDecParam.u1RptFrm == 0)
    {
        _VDEC_DoRefPicMarking(&prVdec->rDecParam);
    }
	
    _VDEC_OutputDispPic(&prVdec->rDecParam, &i4RetValue);    //vChkOutputFBuf

    if((u1Ret = _VDEC_WaitDecDone(prVdec->rDecParam.prCurrFBufInfo->u1FBufStatus, 
        prVdec->rDecParam.prCurrFBufInfo->u4W, 
        prVdec->rDecParam.prCurrFBufInfo->u4H,
        prVdec->rDecParam.bWaitDecLock)) > 0)
    {
        if(u1Ret > 1)
        {
            LOG(8, "//DecFin - SliceType %d, NaluOft %d\n", prVdec->rSliceHdr.u4SliceType, 
            prVdec->rPesInfo.u4VldReadPtr - prVdec->rPesInfo.u4FifoStart);
        }
        i4RetValue = (INT32)E_VDEC_OK;
    }
    else
    {
        i4RetValue = (INT32)E_VDEC_FAIL; //Timeout, decode error
    }

    _VDEC_FrmFin(&prVdec->rDecParam);

    //I frame case
    if(prVdecInfo && (prVdecInfo->eCurState == VDEC_ST_PLAY_I_FRAME))
    {
        if((prVdec->rDecParam.prCurrFBufInfo->u1FBufStatus == TOP_FIELD) ||
            (prVdec->rDecParam.prCurrFBufInfo->u1FBufStatus == BOTTOM_FIELD))
        {
            _VDEC_GetRptr(&u4CurRptr);
            //find start code position
            while(!((*((UINT8*)VIRTUAL(u4CurRptr - 2)) == 0) &&
                (*((UINT8*)VIRTUAL(u4CurRptr - 1)) == 0) &&
                (*((UINT8*)VIRTUAL(u4CurRptr)) == 0x1)))
            {
                u4CurRptr++;
                u4TimeOutCnt++;
                if(u4TimeOutCnt >= FIND_START_CODE_THRD)
                {
                    break;
                }
            }
            if(u4TimeOutCnt < FIND_START_CODE_THRD)
            {
                prVdec->rPesInfo.ucPicType = *((UINT8*)VIRTUAL(u4CurRptr + 1));
                prVdec->rPesInfo.u4VldReadPtr = u4CurRptr + 2;
                VDEC_SendEs(&prVdec->rPesInfo);
            }
        }
    }
    
    if(prVdec->rPesInfo.fgEos)
    {
        _VDEC_FlushDPB(&prVdec->rDecParam);
    }
    return i4RetValue;
}

/** .
*@param .
 */
void _VDEC_H264SWRst(UCHAR ucVdecId, UCHAR ucEsId)
{    
    #ifdef CC_DRIVER_DEMO
    VDEC_H264_INFO_T* prVdec;

    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];

    _VDEC_LogSimPat(prVdec->rDecParam.fgLogSimPat);
    #endif
    
    // reset
    //_VDEC_InitVDecHW(FALSE);

    UNUSED(ucVdecId);
    UNUSED(ucEsId);
}

/** .
*@param .
 */
void _VDEC_H264Stop(UCHAR ucEsId)
{
    VDEC_H264_INFO_T* prVdec;
    
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];

    prVdec->rDecParam.u4DispPts = 0;
    prVdec->rDecParam.u8VRemainder = 0;
    prVdec->rDecParam.u8PtsScale = 0;
    prVdec->rDecParam.u4Rate = 0;
    prVdec->rDecParam.fgEnCalPTS = FALSE;
    prVdec->rDecParam.rSEI.u1ExtMatchFlg = FALSE;
    prVdec->rDecParam.rSEI.u1WaitRAPCnt = 0;
    prVdec->rDecParam.rSEI.u1WaitRAPRefCnt = 0;
    prVdec->rDecParam.rSEI.u1WaitRAPICnt = 0;
    prVdec->rDecParam.rSEI.u4UserDataWptr = 0;
    prVdec->rDecParam.rSEI.u1PicStruct = (UINT8)SEI_FRAME_UNKNOWN;
    prVdec->rDecParam.fgIsFirstFrm = FALSE;
    prVdec->rDecParam.fgIsMMPlay = FALSE;
    prVdec->rDecParam.fgNotSupport = FALSE;
    prVdec->rDecParam.u4PESUserDataWptr = 0;
    prVdec->rDecParam.fgSeamlessDisp = FALSE;
    
    if(prVdec->rDecParam.prSPS)
    {
        prVdec->rDecParam.prSPS->fgSPSValid = FALSE;
    }
    _VDEC_FlushDPB(&prVdec->rDecParam);

    if(prVdec->rDecParam.ucFbgId != FBM_FBG_ID_UNKNOWN)
    {
        FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, ucEsId);
        FBM_ReleaseGroup(prVdec->rDecParam.ucFbgId);
        prVdec->rDecParam.ucFbgId = FBM_FBG_ID_UNKNOWN;
    }
}

/** .
*@param .
 */
void _VDEC_H264Flush(UCHAR ucEsId, BOOL bIsDisp)
{
    VDEC_H264_INFO_T* prVdec;
    
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];

    if(bIsDisp)
    {
        _VDEC_FlushDPB(&prVdec->rDecParam);
    }
    else
    {
        _VDEC_ClrDPB(&prVdec->rDecParam);
    }
}

/** .
*@param .
 */
void _VDEC_H264GetPesStruct(UCHAR ucEsId, void **ppvPesInfo)
{
    VDEC_H264_INFO_T* prVdec;
    
    ASSERT(ppvPesInfo !=NULL);    
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];

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
void _VDEC_H264GetDispPic(UCHAR ucEsId, void* pvInfo)
{
    VDEC_H264_INFO_T* prVdec;
    VDEC_DISP_PIC_INFO_T* prDispInfo;
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;
    UINT8 u1BufIdx = 0;
	#ifdef CRCFLOWTEST
    UINT i = 0;
	BOOLEAN fgIsEqual = TRUE;
	#endif
    
    ASSERT(pvInfo !=NULL);
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];
    prDispInfo = (VDEC_DISP_PIC_INFO_T*)pvInfo;

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if(prVdecEsInfoKeep == NULL)
    {
        return;
    }
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
    _VDEC_AVCYC(prDispInfo);

	#ifdef CRCFLOWTEST
    for(i=0;i<4;i++)
    {
		if(prVdecEsInfoKeep->u4Ycrc[i] != prDispInfo->u4Ycrc[i])
		{
			fgIsEqual = FALSE;
			break;
		}
	}

    for(i=0;i<4;i++)
    {
		if(prVdecEsInfoKeep->u4Ccrc[i] != prDispInfo->u4Ccrc[i])
		{
			fgIsEqual = FALSE;
			break;
		}
	}

	if(!fgIsEqual)
	{
		LOG(0,"crc error has happened!!!\n")
	}
	
	#endif
    
}

void _VDEC_H264SetParam(UCHAR ucEsId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3)
{
    VDEC_H264_INFO_T* prVdec;
    
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];

    if(u4Type == (UINT32)RRMODE)
    {
        if(u4Param2 == 0)        //Y
        {
            if(u4Param1 > 0)    //Enable
            {
                prVdec->rDecParam.fgRRModeYEn = TRUE;
                prVdec->rDecParam.u1RRModeY = (UINT8)u4Param3;
                if(u4Param1 == 2)
                {
                    prVdec->rDecParam.fgRRWrapper = FALSE;
                }
                else
                {
                    prVdec->rDecParam.fgRRWrapper = TRUE;
                }
            }
            else
            {
                prVdec->rDecParam.fgRRModeYEn = FALSE;
                prVdec->rDecParam.u1RRModeY = (UINT8)u4Param3;
            }
        }
        else if(u4Param2 == 1)    //C
        {
            if(u4Param1 > 0)        //Enable
            {
                prVdec->rDecParam.fgRRModeCEn = TRUE;
                prVdec->rDecParam.u1RRModeC = (UINT8)u4Param3;
                if(u4Param1 == 2)
                {
                    prVdec->rDecParam.fgRRWrapper = FALSE;
                }
                else
                {
                    prVdec->rDecParam.fgRRWrapper = TRUE;
                }                
            }
            else
            {
                prVdec->rDecParam.fgRRModeCEn = FALSE;
                prVdec->rDecParam.u1RRModeC = (UINT8)u4Param3;
            }
        }
        else if(u4Param2 == 2)      //Max_dis_hbands
        {
            prVdec->rDecParam.u1MaxDisHbands = (UINT8)u4Param3;
        }
        prVdec->rDecParam.fgChgRRMode = TRUE;
    }
    else if(u4Type == (UINT32)OUTPUT_DISP_ORDER)
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
    else if(u4Type == (UINT32)REPEAT_FRM)
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
    else if(u4Type == (UINT32)ECC_MODE)
    {
        //u4Param1: Enable error re-concealment, u4Param2: Mode, u4Param3: Dedicate error
        prVdec->rDecParam.u1ECMode = (UINT8)((u4Param1 | (u4Param2 << 2)) | (u4Param3 << 1));
    }
    else if(u4Type == (UINT32)ECC_TYPE)
    {
        //u4Param1: Enable error re-concealment, u4Param2: Mode, u4Param3: Dedicate error
        prVdec->rDecParam.u4ECType = u4Param1;
    }    
    else if(u4Type == (UINT32)DRAM_BANDWITH_SIMU)
    {
        prVdec->rDecParam.u4DramMaxLen = u4Param1;
        prVdec->rDecParam.u4DramReqConnDis = u4Param2;
        prVdec->rDecParam.u4DramFifoBank = u4Param3;
    }
    else if(u4Type == (UINT32)LOG_SIMU_PAT)
    {
        prVdec->rDecParam.fgLogSimPat = ((u4Param1>0)?TRUE:FALSE);
    }
    else if(u4Type == (UINT32)WAIT_FB_TIME)
    {
        prVdec->rDecParam.u4WaitFbTime = u4Param1;
    }
    else if(u4Type == (UINT32)AV_SYNC_INFO)
    {
        prVdec->rDecParam.ucSyncMode = (UCHAR)u4Param1;
        prVdec->rDecParam.ucStcId = (UCHAR)u4Param2;
    }
    else if(u4Type == (UINT32)TIME_PROFILE2)
    {
        prVdec->rDecParam.u1DramMoniter = (UINT8)u4Param1;
    }
    else if(u4Type==(UINT32)MM_PARAM)
    {
        prVdec->rDecParam.fgIsMMPlay = (u4Param1>0)?TRUE:FALSE;
        if(prVdec->rDecParam.fgIsMMPlay)
        {
            prVdec->rDecParam.u4WaitFbTime = VDEC_AVSYNC_WAIT_DISP_TIME;
        }
    }
}


void _VDEC_H264GetParam(UCHAR ucEsId, UINT32 u4Type, UINT32* pu4Param1, UINT32* pu4Param2, UINT32* pu4Param3)
{
    VDEC_H264_INFO_T* prVdec;
    
    ASSERT(ucEsId < MAX_ES_NUM);
    ASSERT(pu4Param1 !=NULL);
    ASSERT(pu4Param2 !=NULL);
    ASSERT(pu4Param3 !=NULL);

    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];

    if(u4Type == (UINT32)DISP_IDX)
    {
        *pu4Param1 = (UINT32)prVdec->rDecParam.ucFbgId;
        if(prVdec->rDecParam.prCurrFBufInfo)
        {
            *pu4Param2 = (UINT32)prVdec->rDecParam.prCurrFBufInfo->ucFbId;
        }
        if(prVdec->rDecParam.prSPS != NULL)
        {
            if(prVdec->rDecParam.prSPS->rVUI.fgVideoSignalTypePresentFlag)
            {
                *pu4Param3 = prVdec->rDecParam.prSPS->rVUI.u4VideoFormat;
            }
        }
    }
    else if(u4Type == (UINT32)RRMODE)
    {
        if(*pu4Param2 == 0)        //Y
        {
            if(prVdec->rDecParam.fgRRModeYEn)
            {
                if(prVdec->rDecParam.fgRRWrapper)
                {
                    *pu4Param1 = 1;
                }
                else
                {
                    *pu4Param1 = 2;
                }
                *pu4Param3 = (UINT32)prVdec->rDecParam.u1RRModeY;
            }
            else
            {
                *pu4Param1 = 0;
                *pu4Param3 = (UINT32)prVdec->rDecParam.u1RRModeY;
            }
        }
        else if(*pu4Param2 == 1)    //C
        {
            if(prVdec->rDecParam.fgRRModeCEn)
            {
                if(prVdec->rDecParam.fgRRWrapper)
                {
                    *pu4Param1 = 1;
                }
                else
                {
                    *pu4Param1 = 2;
                }
                *pu4Param3 = (UINT32)prVdec->rDecParam.u1RRModeC;
            }
            else
            {
                *pu4Param1 = 0;
                *pu4Param3 = (UINT32)prVdec->rDecParam.u1RRModeC;
            }
        }
    }
    else if(u4Type == (UINT32)OUTPUT_DISP_ORDER)
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
    else if(u4Type == (UINT32)REPEAT_FRM)
    {
        *pu4Param1 = prVdec->rDecParam.u1RptFrm;
    }
    else if(u4Type == (UINT32)ECC_MODE)
    {
        //u4Param1: Enable error re-concealment, u4Param2: Mode, u4Param3: Dedicate error
        *pu4Param1 = (UINT32)(prVdec->rDecParam.u1ECMode & 0x1);
        *pu4Param2 = (UINT32)(prVdec->rDecParam.u1ECMode & 0xFC) >> 2;
        *pu4Param3 = (UINT32)(prVdec->rDecParam.u1ECMode & 0x2) >> 1;
    }
    else if(u4Type == (UINT32)ECC_TYPE)
    {
        *pu4Param1 = AVC_VLD_READ32(RW_AVLD_ERC_DED_ERR_TYPE);
    }    
    else if(u4Type == (UINT32)DRAM_BANDWITH_SIMU)
    {
        *pu4Param1 = prVdec->rDecParam.u4DramMaxLen;
        *pu4Param2 = prVdec->rDecParam.u4DramReqConnDis;
        *pu4Param3 = prVdec->rDecParam.u4DramFifoBank;
    }
    else if(u4Type == (UINT32)TIME_PROFILE)
    {
        if(prVdec->rDecParam.prCurrFBufInfo->u1FBufStatus == FRAME)
        {
            *pu4Param1 = prVdec->rDecParam.ucFbgId;
            *pu4Param2 = prVdec->rDecParam.u4CurDecMs;       //hw dec
            *pu4Param3 = prVdec->rDecParam.u4CurWaitFbMicro/1000;   //wait fb time
        }
        else
        {
            *pu4Param1 = 0xFF;
        }
    }
    else if(u4Type == (UINT32)TIME_PROFILE2)
    {
         *pu4Param1 = prVdec->rDecParam.u4CurRstMs;   //wait fb time
         prVdec->rDecParam.u4CurRstMs = 0;
         *pu4Param2 = prVdec->rDecParam.prSliceHdr->u4SliceType;
         *pu4Param3 = (UINT32)((prVdec->rDecParam.prCurrFBufInfo->u1FBufStatus == FRAME)?1:0);
    }
    else if(u4Type == (UINT32)TIME_PROFILE3)
    {
        *pu4Param1 = prVdec->rDecParam.u4CurSwMicro;
        *pu4Param2 = prVdec->rDecParam.u4CurDecCycle;
        *pu4Param3 = prVdec->rDecParam.u4CurDramCycle;
    }
    else if(u4Type == (UINT32)BS_TYPE)
    {
        *pu4Param1 = prVdec->rDecParam.prSPS->u4ProfileIdc;
        *pu4Param2 = prVdec->rDecParam.prSPS->u4LevelIdc;
        *pu4Param3 = (UINT32)prVdec->rDecParam.prPPS->fgEntropyCodingModeFlag;        
    }
    else if(u4Type == (UINT32)SEQ_HDR_INFO)
    {    
        *pu4Param1 = (prVdec->rDecParam.prSPS->u4PicWidthInMbsMinus1 + 1) << 4;
        *pu4Param2 = ((2 -prVdec->rDecParam.prSPS->fgFrameMbsOnlyFlag)*
            (prVdec->rDecParam.prSPS->u4PicHeightInMapUnitsMinus1 + 1)) << 4; //32
    }
}


void _VDEC_H264UpdDmxRptr(UCHAR ucEsId)
{
    VDEC_H264_INFO_T* prVdec;
    UINT32 u4CurRptr, u4UsedSz = 0;
    
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];

    _VDEC_GetRptr(&u4CurRptr);

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
    //update dmx pointer, use current pes start addr, hw rptr may go beyond frame end
    //if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, u4CurRptr, u4CurRptr))
    if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, prVdec->rPesInfo.u4VldReadPtr, prVdec->rPesInfo.u4VldReadPtr))
    {                    
        LOG(2, "H264 ES(%d) DMX_UpdVRPtr Fail CurRptr 0x%x, VS 0x%x, VE 0x%x, PicS 0x%x, DmxWptr 0x%x\n", 
            prVdec->rPesInfo.ucEsId, u4CurRptr, 
            prVdec->rPesInfo.u4FifoStart, prVdec->rPesInfo.u4FifoEnd, 
            prVdec->rPesInfo.u4VldReadPtr, prVdec->rDecParam.u4WPtr);
    }
}

void _VDEC_H264SetDmxWptr(UCHAR ucEsId, UINT32 u4Wptr)
{
    VDEC_H264_INFO_T* prVdec;    

    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];
    
    prVdec->rDecParam.u4WPtr = u4Wptr;
    _VDEC_SetWptr(u4Wptr);
}

//skip ES if return FALSE
BOOL _VDEC_H264EsFlwCtrl(UCHAR ucEsId, UCHAR* pucVldId, INT8 i1EsBufStatus, UCHAR ucSkipMode)
{
    UCHAR ucFbgId;
    VDEC_H264_INFO_T* prVdec;
    VDEC_ES_INFO_T* prVdecEsInfo;

    ASSERT(ucEsId < MAX_ES_NUM);
    ASSERT(pucVldId !=NULL);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    
    ucFbgId = prVdec->rDecParam.ucFbgId;

    if(prVdecEsInfo->rMMTPTSeqInfo.u2_width == 0)
    {
        prVdec->rDecParam.ucSkipMode = ucSkipMode;
    }

    // ask mpeg decode to skip one picture due to repeat
    // if ESM-Q Ns > Underflow Threshold, otherwise, ignore
    if (IS_NREF_NALU(prVdec->rPesInfo.ucPicType) && 
        IS_PIC_NALU(prVdec->rPesInfo.ucPicType)) //nal_ref_idc == 0
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

        #ifdef FBM_DPB_CTRL
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
                        LOG(2, "EsFlwCtrl H264 ES(%d) DMX_UpdateVideoReadPointer Fail\n", prVdec->rPesInfo.ucEsId);                
                    }

                      // [LOG] Video plane ask mpeg to drop 1 picture, set fbm empty if decode partial frame
                    if(!_VDEC_SkipFrm(&prVdec->rDecParam))
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
                            LOG(2, "EsFlwCtrl H264 ES(%d) DMX_UpdateVideoReadPointer Fail\n", prVdec->rPesInfo.ucEsId);                
                        }
                        // [LOG] Video plane ask mpeg to drop 1 picture, set fbm empty if decode partial frame
                        if(!_VDEC_SkipFrm(&prVdec->rDecParam))
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
                LOG(2, "EsFlwCtrl H264 ES(%d) DMX_UpdateVideoReadPointer Fail\n", prVdec->rPesInfo.ucEsId);                
            }  
            // [LOG] Video plane ask mpeg to drop 1 picture, set fbm empty if decode partial frame
            if(!_VDEC_SkipFrm(&prVdec->rDecParam))
            {
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }
    }

    #ifdef FBM_DPB_CTRL
    //prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
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
            (prVdecEsInfo->eContainerType == SWDMX_FMT_H264_VIDEO_ES))
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
            if(prVdec->rDecParam.prSPS)
            {
                prVdec->rDecParam.prSPS->fgSPSValid = FALSE;
            }        
            _VDEC_FlushDPB(&prVdec->rDecParam);
            prVdec->rDecParam.i4LastPOC = -99999;
            prVdec->rDecParam.rLastInfo.u4LastFrameNum = 0xffffffff;
            prVdec->rDecParam.fgPOCWrap = FALSE;
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

BOOL _VDEC_IsSupportH264()
{
    return IS_SupportH264();
}

UCHAR _VDEC_H264HandleOverFlw(UCHAR ucVldId, UCHAR ucEsmQId, HANDLE_T hMutex,
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

void _VDEC_H264StartPTS(UCHAR ucEsId, BOOL fgEnCalPTS, UINT32 u4Pts, UINT32 u4PtsDiff)
{
    VDEC_H264_INFO_T* prVdec;    
    VDEC_PTS_INFO_T* prPTSInfo;
    
    if(ucEsId >= MAX_ES_NUM)
    {
        ASSERT(ucEsId < MAX_ES_NUM);
        return;
    }
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];
    prVdec->rDecParam.fgEnCalPTS = fgEnCalPTS;
    if(fgEnCalPTS)
    {            
        prVdec->rDecParam.u4DispPts = u4Pts;
        prVdec->rDecParam.u8VRemainder = 0;
        prPTSInfo = (VDEC_PTS_INFO_T*)u4PtsDiff;
        if(prPTSInfo)
        {
            prVdec->rDecParam.u8PtsScale = prPTSInfo->u8Scale;
            prVdec->rDecParam.u4Rate = prPTSInfo->u4Rate;
        }
        _VDEC_H264Flush(ucEsId, TRUE);
    }
}
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------



