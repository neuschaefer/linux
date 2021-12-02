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
 * $Author: haitao.xu $
 * $Date: 2012/06/21 $
 * $RCSfile: vdec_h264api.c,v $
 * $Revision: #2 $
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
//for clock setting
#include "x_hal_5381.h"

#include "x_mm_common.h"

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

#define MAX_ES_NUM VDEC_MAX_ES
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

#define MAX_VERIFY_FRAME 16
#define WAIT_PPS_SKIP_FRM_NUM 60
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static VDEC_H264_INFO_T rH264DecInfo[MAX_ES_NUM];
#ifdef VDEC_TIME_PROFILE
static BOOL fgIsFrmStart;
#endif


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
INT32 _VDEC_H264Init(UCHAR ucEsId)
{
    VDEC_H264_INFO_T* prVdec;
    static UINT32 u4PredSA = 0;
    UINT32 i, j;
    
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];
    //reset data structure
    x_memset(prVdec, 0, sizeof(VDEC_H264_INFO_T));
    #ifdef TEST_ONOFF_RUNTIME
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    #endif
    if(u4PredSA == 0)
    {
        u4PredSA = (UINT32)BSP_AllocAlignedDmaMemory(PRED_SA_SZ, 1024);
        ASSERT(u4PredSA);
        VERIFY(x_msg_q_create(&prVdec->rDecParam.hUsrDataQ, "AVC_UserDataQ",
            sizeof(MPV_USERDATA_CB_T), USERDATA_Q_SIZE) == OSR_OK);
    }
    prVdec->bPicQueue = FALSE;
    prVdec->rDecParam.u4MaxFBufNum = 18;
    #if 0
    prVdec->rDecParam.rLastInfo.u4LastFrameNum = 0xffffffff;
    prVdec->rDecParam.rLastInfo.u4LastPicW = 0;
    prVdec->rDecParam.rLastInfo.u4LastPicH = 0;
    prVdec->rDecParam.rLastInfo.u4LastSPos = 0;
    prVdec->rDecParam.rLastInfo.fgLastIsRef = FALSE;
    #endif
    prVdec->rDecParam.fgPOCWrap = FALSE;
    prVdec->rDecParam.fgStartPlay = FALSE;
    prVdec->rDecParam.fgIsFirstFrm = FALSE;
    #ifdef VDEC_TIME_PROFILE
    fgIsFrmStart = FALSE;
    #endif

    //memory layout, should query from FBM
    prVdec->rDecParam.u4PredSA = u4PredSA;    //PRED_SA;   
    prVdec->rDecParam.u4DPBAddr = DPB_ADDR;    //not used if FBM_DPB_CTRL
    prVdec->rDecParam.u4DPBSize = DPB_SIZE;    //not used if FBM_DPB_CTRL
    //prVdec->rPesInfo.u4FifoStart = 0;            //init value, will get from pes queue later
    //prVdec->rPesInfo.u4FifoEnd = 0xFFFF;        //init value, will get from pes queue later
    
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
    //prVdec->rDecParam.i4LastPOC = -99999;
    prVdec->rDecParam.u4Dts = 0;
    
    //swdmx will set the param before vdec init
    if((!prVdec->rDecParam.fgIsMMPlay) || (!prVdec->rDecParam.fgEnCalPTS))
    {
        //prVdec->rDecParam.u4DispPts = 0;
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
    #if defined(VDEC_CRC_TEST) || defined(MULTIMEDIA_AUTOTEST_SUPPORT)
    #if defined(VDEC_CRC_TEST) && !defined(MULTIMEDIA_AUTOTEST_SUPPORT)
    if(VDEC_IsTestCRC(ucEsId))
    #endif
    {
        _VDEC_EnableCRC(TRUE);
    }
    #endif

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
    for(j=0; j<MAX_VIEW_NUM; j++)
    {
        for(i=0; i<18; i++)
        {
            prVdec->rDecParam.arFBufInfo[j][i].ucMvBufIdx = 0xff;
            _VDEC_ClrFBufInfo(&prVdec->rDecParam, i + (j<<8));
        }
        #if defined(CC_MVC_SUPPORT) && !defined(CC_VDEC_MVC_DISABLE)
        prVdec->rDecParam.aprCurrFBufInfo[j] = NULL;
        #endif
        prVdec->rDecParam.rLastInfo[j].u4LastFrameNum = 0xffffffff;
        prVdec->rDecParam.rLastInfo[j].u4LastPicW = 0;
        prVdec->rDecParam.rLastInfo[j].u4LastPicH = 0;
        prVdec->rDecParam.rLastInfo[j].u4LastSPos = 0;
        prVdec->rDecParam.rLastInfo[j].fgLastIsRef = FALSE;
        prVdec->rDecParam.i4LastPOC[j] = -99999;
        prVdec->rDecParam.aucFbgId[j] = FBM_FBG_ID_UNKNOWN;
        if((!prVdec->rDecParam.fgIsMMPlay) || (!prVdec->rDecParam.fgEnCalPTS))
        {
            prVdec->rDecParam.au4DispPts[j] = 0;
        }
        //MV buffer array
        for(i=0; i<MAX_FRAME_NUM; i++)
        {
            prVdec->rDecParam.aucEmpMVBufIdxMap[j][i] = (UCHAR)i;
        }
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
    /*for(i=0; i<(MAX_FRAME_NUM<<1); i++)
    {
        prVdec->rDecParam.aucEmpMVBufIdxMap[i] = (UCHAR)i;
    }*/

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
    prVdec->rDecParam.ucPreAllocFbId = FBM_FB_ID_UNKNOWN;
    prVdec->rDecParam.bPreAllocFBG = FALSE;
    prVdec->rDecParam.u1DecFBufIdx = 0;
    //crc
#ifdef CC_STRESS_TEST_CRC_CHK
    prVdec->rDecParam.fgEnCRC = FALSE;
        x_memset((void*)&prVdec->rDecParam.au4CrcFailCnt, 0, sizeof(prVdec->rDecParam.au4CrcFailCnt));
    prVdec->rDecParam.u4CrcPassCnt = 0;
    prVdec->rDecParam.u4BsFailCnt = 0;
#endif
#if defined(CC_MVC_SUPPORT) && !defined(CC_VDEC_MVC_DISABLE)
    prVdec->rDecParam.fgIsMVC = FALSE;
    prVdec->rDecParam.fgAnchorPic = FALSE;
    prVdec->rDecParam.fgInterView = FALSE;
    prVdec->rDecParam.u4CurViewID = 0;
    prVdec->rDecParam.u1CurViewIdx= 0;
    prVdec->rDecParam.u4WaitClrBufIdx = 0xFF;
#endif 
    prVdec->rDecParam.fgNalNonIdr = TRUE;
        
    //init hw module, fifo address is 0 now
    #ifdef TEST_ONOFF_RUNTIME
    _VDEC_LockVld(ucEsId, prVdecEsInfo->u4VldId);
    #endif
    _VDEC_InitVDecHW(TRUE);
    #ifdef TEST_ONOFF_RUNTIME
    _VDEC_UnlockVld(ucEsId, prVdecEsInfo->u4VldId);
    #endif

#ifdef CC_ENABLE_TRACKING_MODE
    prVdec->rDecParam.ucTrackingFbId = 0xFF;
    prVdec->rDecParam.ucTrackingFbIdPending = 0xFF;
#endif

    return (INT32)E_VDEC_OK;
}

/** unit of es packet is an access unit.
*@param .
 */
 #if 0 //obsolete
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
    }
    while (!((prVdec->u1NalUnitType == NON_IDR_SLICE) || (prVdec->u1NalUnitType == IDR_SLICE)));

    return (INT32)E_VDEC_OK;
}
#endif


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
    #ifdef CC_ENABLE_TRACKING_MODE
    HAL_TIME_T rTimeGetFbS, rTimeGetFbE, rTimeGetFbDt;
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
    #if defined(CC_MVC_SUPPORT) && !defined(CC_VDEC_MVC_DISABLE)
    UINT32 u4NaluExtData = 0, u4ByteIdx = 0;
    UINT8 u1PrefixLen = 0;
    #endif
    UINT8 u1ViewIdx = 0;
    UINT16 i = 0;
    
    prExtInfo = (MPV_EXT_INFO_T*)_MPV_GetExtInfo();
    
    _prDbase = DBS_Lock(&_rCritState);
    ASSERT(_prDbase);
    VERIFY(DBS_Unlock(_prDbase, _rCritState));        
    prInfo = &(_prDbase->rMpv[ucEsId]);

    UNUSED(ucVdecId);
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    ASSERT(prVdecEsInfo != NULL);
    
    prVdec->rDecParam.ucEsId = ucEsId;

    ASSERT(prVdec->rPesInfo.ucEsId < MAX_ES_NUM);
    if(prVdec->rPesInfo.ucEsId >= MAX_ES_NUM)
    {
        LOG(3, "Pes EsId > MAX_NUM\n");
        return (INT32)E_VDEC_PARSE_NOT_READY;
    }
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
    prVdecEsInfo->fgNoFrameBuffer = FALSE;
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

    //PVR, only parse SPS and SEI without decoding, skip other NALU
    if(prVdecEsInfo && (prVdecEsInfo->fgParsingInfo && !prVdecEsInfo->fgThumbMethod2))
    {
        if((prVdec->u1NalUnitType != SPS) && (prVdec->u1NalUnitType != SEI))
        {
            if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, prVdec->rPesInfo.u4VldReadPtr, prVdec->rPesInfo.u4VldReadPtr))                
            {                    
                LOG(1, "H264 ES(%d) DMX_UpdVRPtr Fail\n");
            }
            return (INT32)E_VDEC_PARSE_NOT_READY;
        }
    }

    #if defined(CC_MVC_SUPPORT) && !defined(CC_VDEC_MVC_DISABLE)
    if(prVdec->rDecParam.u4PrefixAddr < 0xFFFFFFFF)
    {
        if(prVdec->rPesInfo.u4VldReadPtr > prVdec->rDecParam.u4PrefixAddr)
        {
            u1PrefixLen = prVdec->rPesInfo.u4VldReadPtr - prVdec->rDecParam.u4PrefixAddr;
        }
        else
        {
            u1PrefixLen = (prVdec->rPesInfo.u4FifoEnd - prVdec->rPesInfo.u4FifoEnd) +
                prVdec->rPesInfo.u4VldReadPtr - prVdec->rDecParam.u4PrefixAddr;
        }
        prVdec->rDecParam.fgIsPrefixLen32Bit = (u1PrefixLen == 7) ? TRUE : FALSE;//7 should be 8?
    }
        
    if(prVdec->u1NalUnitType == PREFIX)
    {
        prVdec->rDecParam.u4PrefixAddr = prVdec->rPesInfo.u4VldReadPtr;
    }
    else
    {
        prVdec->rDecParam.u4PrefixAddr = 0xFFFFFFFF;
    }

    if((prVdec->u1NalUnitType == PREFIX) || (prVdec->u1NalUnitType == CODED_SLICE_EXT))
    {
        HalFlushInvalidateDCacheMultipleLine(VIRTUAL(prVdec->rPesInfo.u4VldReadPtr), NALU_EXT_SZ); 
        
        for(u4ByteIdx = 0; u4ByteIdx < NALU_EXT_SZ; u4ByteIdx++)
        {
            u4NaluExtData <<= 8;
            u4NaluExtData += (UINT32)(*(UINT8*)VIRTUAL(prVdec->rPesInfo.u4VldReadPtr));
            prVdec->rPesInfo.u4VldReadPtr++;
            if(prVdec->rPesInfo.u4VldReadPtr == prVdec->rPesInfo.u4FifoEnd)
            {
                prVdec->rPesInfo.u4VldReadPtr = prVdec->rPesInfo.u4FifoStart;
            }
        }
        prVdec->rDecParam.fgInterView = FALSE;
        if((u4NaluExtData >> 23) == 0)  //mvc
        {
            prVdec->rDecParam.fgIsMVC = TRUE;
			#ifdef CC_SW_MVC_ENABLE
			prVdecEsInfo->e3DType = VDEC_3D_MVC;
			prVdec->rDecParam.fgIsMVC = FALSE;
			#endif
            prVdec->rDecParam.fgNalNonIdr = (u4NaluExtData >> 22);
            prVdec->rDecParam.fgAnchorPic = (u4NaluExtData >> 2) & (0x1);
            prVdec->rDecParam.fgInterView = (u4NaluExtData >> 1) & (0x1);
            prVdec->rDecParam.u4CurViewID = (u4NaluExtData >> 6) & (0x3FF);
            for(u1ViewIdx = 0; u1ViewIdx < MAX_VIEW_NUM; u1ViewIdx++)
            {
                if(prVdec->rDecParam.u4CurViewID == prVdec->rDecParam.au4ViewIDList[u1ViewIdx])
                {
                    prVdec->rDecParam.u1CurViewIdx = u1ViewIdx;
                    if(prVdec->rDecParam.u4CurViewID == 0)
                    {
                        printf("dsfsafasfd\n");
                    }
                    break;
                }
            }
            if(u1ViewIdx == MAX_VIEW_NUM)   //view idx > 2
            {
                prVdec->rDecParam.fgIsMVC = FALSE;
                prVdec->rDecParam.fgAnchorPic = FALSE;
                prVdec->rDecParam.fgInterView = FALSE;
                prVdec->rDecParam.u4CurViewID = 0;
                prVdec->rDecParam.u1CurViewIdx= 0;
                prVdec->rDecParam.fgNalNonIdr = TRUE;
                prVdec->rDecParam.fgIsMVCClkSet = FALSE;
                return (INT32)E_VDEC_PARSE_NOT_READY;
            }
            #if defined(CC_MT5395)
            if(!prVdec->rDecParam.fgIsMVCClkSet)
            {
                UINT32 u4Clk;
                u4Clk = IO_READ32(CKGEN_BASE, 0x224);
                prVdec->rDecParam.u1PrevVLDClk =  u4Clk & 0xF;
                IO_WRITE32(CKGEN_BASE, 0x224, (u4Clk & 0xFFFFFFF0) | 0x9);   //mt5395 MVC 288MHz, sawless_pll/1.5

                u4Clk = IO_READ32(CKGEN_BASE, 0x228);
                prVdec->rDecParam.u1PrevMCClk =  u4Clk & 0xF;
                IO_WRITE32(CKGEN_BASE, 0x228, (u4Clk & 0xFFFFFFF0) | 0x9);   //mt5395 MVC 288MHz, sawless_pll/1.5

                prVdec->rDecParam.fgIsMVCClkSet = TRUE;
            }
            #endif
            prVdec->rDecParam.prCurrFBufInfo = 
                prVdec->rDecParam.aprCurrFBufInfo[prVdec->rDecParam.u1CurViewIdx];
            prVdec->rDecParam.u1DecFBufIdx = prVdec->rDecParam.au1DecFBufIdx[prVdec->rDecParam.u1CurViewIdx];
            if(prVdec->rDecParam.prCurrFBufInfo)
            {
                prVdec->rDecParam.pu1DecWorkBufY = (UINT8*)(prVdec->rDecParam.prCurrFBufInfo->u4YAddr);
                prVdec->rDecParam.pu1DecWorkBufC = (UINT8*)(prVdec->rDecParam.prCurrFBufInfo->u4CAddr);
                prVdec->rDecParam.pu1DecWorkBufMV = (UINT8*)(prVdec->rDecParam.prCurrFBufInfo->u4MVBufAddr);
            }
        }
    }
    #endif

    if(prVdecEsInfo && (prVdecEsInfo->e3DType == VDEC_3D_REALD_DUAL_STREAM))
    {
        prVdec->rDecParam.u1CurViewIdx = prVdec->rPesInfo.ucEsId;
        u1ViewIdx = prVdec->rDecParam.u1CurViewIdx;
        prVdec->rDecParam.u4MaxFBufNum = prVdec->rDecParam.au4MaxFBufNum[prVdec->rDecParam.u1CurViewIdx];
    }
        
    if(u4LastPtr > (prVdec->rPesInfo.u4VldReadPtr - prVdec->rPesInfo.u4FifoStart))
    {
        LOG(2, "--Nalu type %d, offset 0x%x, wptrOffset 0x%x\n", prVdec->u1NalUnitType, 
            prVdec->rPesInfo.u4VldReadPtr - prVdec->rPesInfo.u4FifoStart,
            prVdec->rDecParam.u4WPtr - prVdec->rPesInfo.u4FifoStart);
    }
    //u4LastPtr = (prVdec->rPesInfo.u4VldReadPtr - prVdec->rPesInfo.u4FifoStart);
    if(prVdecEsInfo && prVdecEsInfo->u1PVRWaitSps == 1)
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
    if(prVdecEsInfo && prVdec->u1NalUnitType <= CODED_SLICE_EXT)
    {
        _VDEC_LockVld(ucEsId, prVdecEsInfo->u4VldId);
    }
    if((prVdec->u1NalUnitType <= IDR_SLICE) || (prVdec->u1NalUnitType == CODED_SLICE_EXT))
    {
        //fix erc mode to slice mode
        if(prVdec->rDecParam.u1SliceCnt == prVdec->rPesInfo.u1SliceCount)
        {
            #ifdef ENABLE_MULTIMEDIA
            if(prVdec->rDecParam.fgIsMMPlay)
            {
                if(prVdec->rDecParam.u1SliceCnt > SLICE_CNT_THRD)
                {
                    //prVdec->rDecParam.u1ECMode = ((ERC_ENABLE | ERC_DED_ERR) | (ERC_MODE1 << 2));
                    prVdec->rDecParam.u4ECType = 0x8210;
                }
            }
            else
            {
                if(prVdec->rDecParam.u1SliceCnt > SLICE_CNT_THRD)
                {
                    //prVdec->rDecParam.u1ECMode = ((ERC_ENABLE | ERC_DED_ERR) | (ERC_MODE1 << 2));
                    prVdec->rDecParam.u4ECType = 0x8210;
                }
                else
                {
                   //prVdec->rDecParam.u1ECMode = ((ERC_ENABLE | ERC_DED_ERR) | (ERC_MODE2 << 2));
                   prVdec->rDecParam.u4ECType = 0xC210;
                }
            }
            #else
            if(prVdec->rDecParam.u1SliceCnt > SLICE_CNT_THRD)
            {
                //prVdec->rDecParam.u1ECMode = ((ERC_ENABLE | ERC_DED_ERR) | (ERC_MODE1 << 2));
                prVdec->rDecParam.u4ECType = 0x8210;
            }
            else
            {
               //prVdec->rDecParam.u1ECMode = ((ERC_ENABLE | ERC_DED_ERR) | (ERC_MODE2 << 2));
               prVdec->rDecParam.u4ECType = 0xC210;
            }
            #endif
            
        }
        else    //unknown slice cnt
        {
            prVdec->rDecParam.u1ECMode = (UINT8)((ERC_ENABLE | ERC_DED_ERR) | (ERC_MODE2 << 2));
            prVdec->rDecParam.u1SliceCnt = prVdec->rPesInfo.u1SliceCount;
        }
        _VDEC_SetBsInfo(BARREL1, prVdec->rPesInfo.u4VldReadPtr, rH264DecInfo[prVdec->rPesInfo.ucEsId].rDecParam.u4WPtr, 
            prVdec->rPesInfo.u4FifoStart, prVdec->rPesInfo.u4FifoEnd, prVdec->rDecParam.u4PredSA,
            prVdec->rDecParam.u1ECMode, prVdec->rDecParam.u4ECType, prVdec->rDecParam.fgIsMMPlay);
    }
    else if((prVdec->u1NalUnitType <= END_OF_SEQ) || (prVdec->u1NalUnitType == SUB_SPS))
    {        
        _VDEC_SetBsInfo(BARREL2, prVdec->rPesInfo.u4VldReadPtr, rH264DecInfo[prVdec->rPesInfo.ucEsId].rDecParam.u4WPtr, 
            prVdec->rPesInfo.u4FifoStart, prVdec->rPesInfo.u4FifoEnd, prVdec->rDecParam.u4PredSA,
            0, prVdec->rDecParam.u4ECType, prVdec->rDecParam.fgIsMMPlay);            
    }
    else
    {
        // I frame case
        if (prVdecEsInfoKeep && (prVdecEsInfoKeep->eCurState == VDEC_ST_PLAY_I_FRAME))
        {
            if (prVdecEsInfoKeep && (prVdecEsInfoKeep->pfDecNfyFct))
            {
                prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag, VDEC_COND_ERROR, 0, 0);                
            }
        }
        if(prVdecEsInfo && (prVdec->u1NalUnitType <= CODED_SLICE_EXT))
        {
            _VDEC_UnlockVld(ucEsId, prVdecEsInfo->u4VldId);
        }
        return (INT32)E_VDEC_PARSE_NOT_READY;
    }
    #ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&rTimeRstE);
    HAL_GetDeltaTime(&rTimeRstDt, &rTimeRstS, &rTimeRstE);
    prVdec->rDecParam.u4CurRstMs += (rTimeRstDt.u4Micros / 1000);
    #endif
    prVdec->rDecParam.rLastInfo[u1ViewIdx].u4LastNaluType = prVdec->u1NalUnitType;
    switch(prVdec->u1NalUnitType)
    {
        case NON_IDR_SLICE:
        case IDR_SLICE:
            #if defined(CC_MVC_SUPPORT) && !defined(CC_VDEC_MVC_DISABLE)
            if(prVdec->rDecParam.u4CurViewID != prVdec->rDecParam.au4ViewIDList[0])
            {
                LOG(7, "[warning] prefix missing? Force viewIdx to 0");
                prVdec->rDecParam.fgNalNonIdr = TRUE;
                prVdec->rDecParam.fgAnchorPic = FALSE;
                prVdec->rDecParam.fgInterView = TRUE;
                prVdec->rDecParam.u4CurViewID = prVdec->rDecParam.au4ViewIDList[0];
                prVdec->rDecParam.u1CurViewIdx = 0;
                
                prVdec->rDecParam.prCurrFBufInfo = 
                    prVdec->rDecParam.aprCurrFBufInfo[prVdec->rDecParam.u1CurViewIdx];
                prVdec->rDecParam.u1DecFBufIdx = prVdec->rDecParam.au1DecFBufIdx[prVdec->rDecParam.u1CurViewIdx];
                if(prVdec->rDecParam.prCurrFBufInfo)
                {
                    prVdec->rDecParam.pu1DecWorkBufY = (UINT8*)(prVdec->rDecParam.prCurrFBufInfo->u4YAddr);
                    prVdec->rDecParam.pu1DecWorkBufC = (UINT8*)(prVdec->rDecParam.prCurrFBufInfo->u4CAddr);
                    prVdec->rDecParam.pu1DecWorkBufMV = (UINT8*)(prVdec->rDecParam.prCurrFBufInfo->u4MVBufAddr);
                }
            }
            #endif
		#ifndef CC_SW_MVC_ENABLE
        case CODED_SLICE_EXT:
		#endif
            prVdec->rDecParam.fgSkipCurFrm = FALSE;
            if(prVdec->rDecParam.u1RptFrm == 1)
            {
                prVdec->rDecParam.u1RptFrm = 2;
            }
            if((prVdecEsInfo && (prVdecEsInfo->u4MMSpeed > STC_SPEED_TYPE_FORWARD_2X))&&(prVdec->rDecParam.u1CurViewIdx == 0x1))
            {
                printf("error index 111111111111111111\n");
            }
            _VDEC_SliceNonIDR(prVdec, &i4RetValue, ucEsId);

            if(i4RetValue == E_VDEC_ALLOCATEFB)
            {
                _VDEC_UnlockVld(ucEsId, prVdecEsInfo->u4VldId);
                //error handling
                //clr current frame status if still in the middle of decoding process
                if(prVdec->rDecParam.prCurrFBufInfo)
                {
                    if(FBM_GetFrameBufferStatus(prVdec->rDecParam.aucFbgId[u1ViewIdx], 
                        prVdec->rDecParam.prCurrFBufInfo->ucFbId) == FBM_FB_STATUS_DECODE)
                    {
                        FBM_SetFrameBufferStatus(prVdec->rDecParam.aucFbgId[u1ViewIdx], 
                            prVdec->rDecParam.prCurrFBufInfo->ucFbId, FBM_FB_STATUS_READY);
                    }
                }

                #ifdef CC_ENABLE_TRACKING_MODE
                HAL_GetTime(&rTimeGetFbS);
                #endif
                
                prVdec->rDecParam.ucPreAllocFbId = 
                    FBM_GetEmptyFrameBuffer(prVdec->rDecParam.aucFbgId[u1ViewIdx], prVdec->rDecParam.u4WaitFbTime);

                #ifdef CC_ENABLE_TRACKING_MODE
                HAL_GetTime(&rTimeGetFbE);
                HAL_GetDeltaTime(&rTimeGetFbDt, &rTimeGetFbS, &rTimeGetFbE);
                LOG(1, "GetFb %d, %d ms\n", prVdec->rDecParam.ucPreAllocFbId, (rTimeGetFbDt.u4Seconds * 1000) +
                    (rTimeGetFbDt.u4Micros / 1000));
                #endif
                if(prVdec->rDecParam.ucPreAllocFbId == FBM_FB_ID_UNKNOWN)
                {
                   prVdecEsInfo->fgNoFrameBuffer = TRUE;
                   return E_VDEC_PARSE_NOT_READY;
                }

                if((prVdec->rDecParam.ucPreAllocFbId != FBM_FB_ID_UNKNOWN) &&
                    prVdec->rPesInfo.ucEsId < MAX_ES_NUM)
                {
                    _VDEC_LockVld(ucEsId, prVdecEsInfo->u4VldId);
                    _VDEC_SetBsInfo(BARREL1, prVdec->rPesInfo.u4VldReadPtr, rH264DecInfo[prVdec->rPesInfo.ucEsId].rDecParam.u4WPtr, 
                        prVdec->rPesInfo.u4FifoStart, prVdec->rPesInfo.u4FifoEnd, prVdec->rDecParam.u4PredSA,
                        prVdec->rDecParam.u1ECMode, prVdec->rDecParam.u4ECType, prVdec->rDecParam.fgIsMMPlay);
                    _VDEC_SliceNonIDR(prVdec, &i4RetValue,ucEsId);
                }
            }
            //record slice reference type for new picture detect
            prVdec->rDecParam.rLastInfo[u1ViewIdx].fgLastIsRef = fgIsRefPic(prVdec->rDecParam.u1NalRefIdc);

            //find access unit start 7.4.1.2.3, start dec last picture queue, and restore pes info in queue
            if((i4RetValue != (INT32)E_VDEC_OK) && (i4RetValue != (INT32)E_VDEC_OK_RES_CHG))
            //{
            //    prVdec->rDecParam.rLastInfo[u1ViewIdx].u4LastSPos = prVdec->rPesInfo.u4VldReadPtr;
            //    return i4RetValue;
            //}
            //selse
            {
                prInfo->u4VpsrDropNs++;
                if(prVdecEsInfoKeep->rPesInfo.fgEos &&(prVdec->rDecParam.ucPreAllocFbId != FBM_FB_ID_UNKNOWN))
                {
                    FBM_SetFrameBufferStatus( prVdec->rDecParam.aucFbgId[u1ViewIdx],  prVdec->rDecParam.ucPreAllocFbId,
                                                    FBM_FB_STATUS_EMPTY);

                }
                if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, prVdec->rPesInfo.u4VldReadPtr, prVdec->rPesInfo.u4VldReadPtr))                
                {                    
                    LOG(1, "H264 ES(%d) DMX_UpdVRPtr Fail\n");
                }
                if(i4RetValue == E_VDEC_ALLOCATEFB) //can't get FB
                {
                    return (INT32)E_VDEC_PARSE_NOT_READY;
                }
            }
            break;
        case SEI:
            _VDEC_SEIrbsp(prVdec, &i4RetValue);
            //*pi4RetValue = (INT32)E_VDEC_OK;
            break;
#if defined(CC_3D_MM_DS_SUPPORT) && !defined(CC_SW_MVC_ENABLE)
        case SUB_SPS:   //todo, error handling in error bitstream
            prVdec->rDecParam.fgIsMVC = TRUE;
            if(prVdecEsInfo)
            {
                prVdecEsInfo->e3DType = VDEC_3D_MVC;
            }
#endif            
        case SPS:
            prVdec->fgisvalid = FALSE;
            _VDEC_SPSrbsp(prVdec, &i4RetValue);
            if((prVdec->fgisvalid == TRUE)&&(i4RetValue != 0)&&(prVdec->u1SPSIDBK < 32))
            {
                x_memcpy((void*)&prVdec->arSPS[prVdec->u1SPSIDBK], (void*)&prVdec->rSPSBK, sizeof(VDEC_AVC_SPS_T));

            }
            break;      
        case PPS:
            prVdec->fgisvalid = FALSE;
            _VDEC_PPSrbsp(prVdec, &i4RetValue);
            if((prVdec->fgisvalid== TRUE)&&(i4RetValue != 0))
            {
                x_memcpy((void*)&prVdec->arPPS[prVdec->u1PPSIDBK], (void*)&prVdec->rPPSBK, sizeof(VDEC_AVC_PPS_T));

            }
            break;
        case AU_DELIMITER:
            //save user data from sei to current frame
            prCurrFBufInfo = prVdec->rDecParam.prCurrFBufInfo;
            if(prCurrFBufInfo && (!prVdec->rDecParam.fgSkipCurFrm))
            {
                if(prVdec->rDecParam.fgDispCurDecPic)
                {
                    // 1. copy data, notify current data in SEI, output ring buffer
                    // 2. get all msg from Q and notify
                    if(prExtInfo->pfUserDataCb &&
                        ((prExtInfo->fgUserData[prVdec->rDecParam.ucEsId] && (prVdec->rDecParam.rSEI.u4UserDataWptr > 0)) ||
                        (prExtInfo->fgPesUserData[prVdec->rDecParam.ucEsId] && (prVdec->rDecParam.u4PESUserDataWptr > 0))))
                    {
                        prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdec->rDecParam.aucFbgId[u1ViewIdx],
                            prCurrFBufInfo->ucFbId);
                        if(prFbmPicHdr != NULL)
                        {
                            rUsrData.fgProgressiveFrm = prFbmPicHdr->fgProgressiveFrm;
                            rUsrData.u2TemporalRef = (UINT16)prFbmPicHdr->i4TemporalRef;
                            rUsrData.u4Pts = prFbmPicHdr->u4PTS;
                            rUsrData.ucEsId = prVdec->rDecParam.ucEsId;
                            rUsrData.ucPicStruct = prFbmPicHdr->ucPicStruct;
                            rUsrData.ucPicType = prFbmPicHdr->ucPicCdTp;
                            rUsrData.ucRepFirstFld = prFbmPicHdr->fgRepFirstFld;
                            rUsrData.ucTopFldFirst = prFbmPicHdr->fgTopFldFirst;
                        }                        
                        
                        #ifdef CC_SEI_USER_DATA_ENABLE
                        if(prExtInfo->fgUserData[prVdec->rDecParam.ucEsId] && (prVdec->rDecParam.rSEI.u4UserDataWptr > 0))
                        {
                            rUsrData.u4Rptr = _VDEC_SaveUserData(prVdec->rDecParam.rSEI.au1UsrData, 
                                prVdec->rDecParam.rSEI.u4UserDataWptr);
                            rUsrData.ucDataLen = (UCHAR)prVdec->rDecParam.rSEI.u4UserDataWptr;                        
                            prExtInfo->pfUserDataCb(&rUsrData);
                        }
                        #endif
                        #ifdef CC_DMX_PES_EXT_BUFFER
                        if(prExtInfo->fgPesUserData[prVdec->rDecParam.ucEsId] && (prVdec->rDecParam.u4PESUserDataWptr > 0))
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
            prVdec->rDecParam.fgSkipCurFrm = FALSE;
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
            if (prVdecEsInfoKeep && (prVdecEsInfoKeep->eCurState == VDEC_ST_PLAY_I_FRAME))
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
    else if((i4RetValue == (INT32)E_VDEC_OK) && 
        ((prVdec->u1NalUnitType > IDR_SLICE) && (prVdec->u1NalUnitType != CODED_SLICE_EXT)))
    {
        i4RetValue = (INT32)E_VDEC_PARSE_NOT_READY;
        //I frame case
        if(prVdecEsInfoKeep && (prVdecEsInfoKeep->eCurState == VDEC_ST_PLAY_I_FRAME))
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

    prVdec->rDecParam.rLastInfo[u1ViewIdx].u4LastSPos = prVdec->rPesInfo.u4VldReadPtr;    
    if(prVdecEsInfo && (i4RetValue != (INT32)E_VDEC_OK) && (i4RetValue != (INT32)E_VDEC_OK_RES_CHG))
    {
        _VDEC_UnlockVld(ucEsId, prVdecEsInfo->u4VldId);
        #if defined(CC_MVC_SUPPORT) && !defined(CC_VDEC_MVC_DISABLE)
        if((i4RetValue == 1)&&(prVdec->u1NalUnitType == CODED_SLICE_EXT))
        {
            prVdec->rDecParam.fgIsMVC = FALSE;
            prVdec->rDecParam.fgAnchorPic = FALSE;
            prVdec->rDecParam.fgInterView = FALSE;
            prVdec->rDecParam.u4CurViewID = 0;
            prVdec->rDecParam.u1CurViewIdx= 0;
            prVdec->rDecParam.fgNalNonIdr = TRUE;
            prVdec->rDecParam.u4WaitClrBufIdx = 0xFF;
            #if defined(CC_MT5395)
            if(prVdec->rDecParam.fgIsMVCClkSet)
            {
                UINT32 u4Clk;
                u4Clk = IO_READ32(CKGEN_BASE, 0x224);
                IO_WRITE32(CKGEN_BASE, 0x224, (u4Clk & 0xFFFFFFF0) | prVdec->rDecParam.u1PrevVLDClk);   //mt5395 MVC 288MHz
                u4Clk = IO_READ32(CKGEN_BASE, 0x228);
                IO_WRITE32(CKGEN_BASE, 0x228, (u4Clk & 0xFFFFFFF0) | prVdec->rDecParam.u1PrevMCClk);   //mt5395 MVC 288MHz
                prVdec->rDecParam.fgIsMVCClkSet = FALSE;
            }
            #endif
            
        }
        #endif
    }
    return i4RetValue;
}

/** .
*@param .
 */
INT32 _VDEC_H264DecStart(UCHAR ucVdecId, UCHAR ucEsId)
{
    VDEC_H264_INFO_T* prVdec;
    INT32 i4RetValue;
    VDEC_ES_INFO_T* prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    
    UNUSED(ucVdecId);
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];
        
    _VDEC_DecProc(&prVdec->rDecParam, &i4RetValue);
    if(i4RetValue != (INT32)E_VDEC_OK)
    {
       if(ucEsId < MAX_ES_NUM)
       {
            _VDEC_H264UpdDmxRptr(ucEsId);
       }
        //Printf("unlock in _VDEC_H264DecStart %d -----\n",ucEsId);
        _VDEC_UnlockVld(ucEsId, prVdecEsInfo->u4VldId);
    }
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
    VDEC_ES_INFO_T* prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    UINT32 u4CurRptr, u4TimeOutCnt = 0;
 
    UNUSED(ucVdecId);
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];
    
    if(prVdec->rDecParam.u1RptFrm == 0)
    {
        _VDEC_DoRefPicMarking(&prVdec->rDecParam);
    }

    #if defined(CC_MVC_SUPPORT) && !defined(CC_VDEC_MVC_DISABLE) && defined(CC_ALLEGRO_MVC)
    //FUNNY_A_S00_5x6_r1.0, frm103, 321
    if((prVdec->rDecParam.u1CurViewIdx > 0) &&
        (!prVdec->rDecParam.fgNalNonIdr))
    {
        //prVdec->rDecParam.rLastInfo[prVdec->rDecParam.u1CurViewIdx].u4LastFrameNum = 0xffffffff;
        //vFlushBufRefInfo();
        _VDEC_FlushDPB(&prVdec->rDecParam);
    }
    #endif
    
    _VDEC_OutputDispPic(&prVdec->rDecParam, &i4RetValue);    //vChkOutputFBuf

    if((u1Ret = _VDEC_WaitDecDone(prVdec->rDecParam.prCurrFBufInfo->u1FBufStatus, 
        prVdec->rDecParam.prCurrFBufInfo->u4W, 
        prVdec->rDecParam.prCurrFBufInfo->u4H,
        prVdec->rDecParam.bWaitDecLock, ucEsId)) > 0)
    {
        if(u1Ret > 1)
        {
            LOG(5, "//DecFin - SliceType %d, NaluOft %d poc %d\n", prVdec->rSliceHdr.u4SliceType, 
            prVdec->rPesInfo.u4VldReadPtr - prVdec->rPesInfo.u4FifoStart,prVdec->rDecParam.prCurrFBufInfo->i4POC);
			if(u1Ret == 3)
			{
				prVdec->rDecParam.prCurrFBufInfo->fgIsNotDisp = TRUE;
			}
        }
        i4RetValue = (INT32)E_VDEC_OK;
    }
    else
    {
        i4RetValue = (INT32)E_VDEC_FAIL; //Timeout, decode error
    }

    _VDEC_FrmFin(&prVdec->rDecParam);

    //I frame case
    if(prVdecEsInfoKeep && (prVdecEsInfoKeep->eCurState == VDEC_ST_PLAY_I_FRAME))
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
        if(prVdec->rDecParam.fgIsMVC)
        {
            prVdec->rDecParam.u1CurViewIdx = 1 - prVdec->rDecParam.u1CurViewIdx;
            _VDEC_FlushDPB(&prVdec->rDecParam);
            prVdec->rDecParam.u1CurViewIdx = 1 - prVdec->rDecParam.u1CurViewIdx;
        }
    }
    if(ucEsId < MAX_ES_NUM)
       {
          _VDEC_H264UpdDmxRptr(ucEsId);
       }
    //Printf("unlock in _VDEC_H264DecFin %d -----\n",ucEsId);
    #ifdef TEST_ONOFF_RUNTIME
    if(i4RetValue != (INT32)E_VDEC_OK)
    {
    #endif
        _VDEC_UnlockVld(ucEsId, prVdecEsInfo->u4VldId);
    #ifdef TEST_ONOFF_RUNTIME
    }
    #endif
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
    VDEC_ES_INFO_T *prVdecEsInfo;
    UINT32 u4Idx;
    
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    #ifdef CC_ENABLE_TRACKING_MODE
    LOG(1, "======== TrackingAcc %d\n", prVdec->rDecParam.u4TrackingAccMax);
    #endif
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
	prVdec->rDecParam.ucBNum = 0;
    
    prVdec->rDecParam.fgNalNonIdr = TRUE;
    
    if(prVdec->rDecParam.prSPS)
    {
        prVdec->rDecParam.prSPS->fgSPSValid = FALSE;
    }
    //_VDEC_FlushDPB(&prVdec->rDecParam);

    for(u4Idx=0; u4Idx<MAX_VIEW_NUM; u4Idx++)
    {
        if(prVdec->rDecParam.aucFbgId[u4Idx] != FBM_FBG_ID_UNKNOWN)
        {
            prVdec->rDecParam.u1CurViewIdx = u4Idx;
            _VDEC_FlushDPB(&prVdec->rDecParam);
            
            if(prVdec->rDecParam.fgIsMVC)
            {
                FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, u4Idx);
            }
            else
            {
                FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, ucEsId);
            }
            FBM_ReleaseGroup(prVdec->rDecParam.aucFbgId[u4Idx]);
            prVdec->rDecParam.aucFbgId[u4Idx] = FBM_FBG_ID_UNKNOWN;
        }
        prVdec->rDecParam.au4DispPts[u4Idx] = 0;
    }
    if(prVdecEsInfo->ucFbgId != FBM_FBG_ID_UNKNOWN)
    {
        FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, ucEsId);         
        FBM_ReleaseGroup(prVdecEsInfo->ucFbgId);
        prVdecEsInfo->ucFbgId = FBM_FBG_ID_UNKNOWN;
    }
    #if defined(CC_MVC_SUPPORT) && !defined(CC_VDEC_MVC_DISABLE)
    //move after _VDEC_FlushDPB
    #if defined(CC_MT5395)
    if(prVdec->rDecParam.fgIsMVCClkSet)
    {
        UINT32 u4Clk;
        u4Clk = IO_READ32(CKGEN_BASE, 0x224);
        IO_WRITE32(CKGEN_BASE, 0x224, (u4Clk & 0xFFFFFFF0) | prVdec->rDecParam.u1PrevVLDClk);   //mt5395 MVC 288MHz

        u4Clk = IO_READ32(CKGEN_BASE, 0x228);
        IO_WRITE32(CKGEN_BASE, 0x228, (u4Clk & 0xFFFFFFF0) | prVdec->rDecParam.u1PrevMCClk);   //mt5395 MVC 288MHz
        prVdec->rDecParam.fgIsMVCClkSet = FALSE;
    }
    #endif
    prVdec->rDecParam.fgAnchorPic = FALSE;
    prVdec->rDecParam.fgInterView = FALSE;
    prVdec->rDecParam.u4CurViewID = 0;
    prVdec->rDecParam.u4WaitClrBufIdx = 0xFF;
    #endif
    prVdec->rDecParam.fgIsMVC = FALSE;
    prVdec->rDecParam.u1CurViewIdx= 0;
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
    //#ifdef TEST_ONOFF_RUNTIME
    #if defined(TEST_ONOFF_RUNTIME) || defined(MULTIMEDIA_AUTOTEST_SUPPORT) || defined(VDEC_CRC_TEST)    
        VDEC_ES_INFO_T* prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);    
    #endif
    UINT8 u1BufIdx = 0;
    #ifdef CC_STRESS_TEST_CRC_CHK
    BOOL fgCRCFail = FALSE;
    #endif
    #ifdef CRCFLOWTEST
    UINT8 i = 0;
    BOOL fgIsEqual = TRUE;
    #endif
    UINT8 u1ViewIdx;
    
    ASSERT(pvInfo !=NULL);
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];
    prDispInfo = (VDEC_DISP_PIC_INFO_T*)pvInfo;

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if(prVdecEsInfoKeep == NULL)
    {
        #ifdef TEST_ONOFF_RUNTIME
         _VDEC_UnlockVld(ucEsId, prVdecEsInfo->u4VldId);
        #endif
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
        #ifdef TEST_ONOFF_RUNTIME
        _VDEC_UnlockVld(ucEsId, prVdecEsInfo->u4VldId);
        #endif
        ASSERT(u1BufIdx < 18);
        return;
    }

    u1ViewIdx = prVdec->rDecParam.u1CurViewIdx;
    if(u1ViewIdx >= MAX_VIEW_NUM)
    {
        #ifdef TEST_ONOFF_RUNTIME
         _VDEC_UnlockVld(ucEsId, prVdecEsInfo->u4VldId);
        #endif
        LOG(1, "ViewIdx Err!\n");
        return;
    }
    prDispInfo->pvYAddr = (void*)prVdec->rDecParam.arFBufInfo[u1ViewIdx][u1BufIdx].u4YAddr;
    #if defined(CC_MVC_SUPPORT) && !defined(CC_VDEC_MVC_DISABLE)
    prDispInfo->u1PicStruct = prVdec->rDecParam.u1PicStructKeep;    //current decoded frame
    //buf status may be clear if display/buf clr right after decode
    prDispInfo->u4H =
        (UINT16)(prVdec->rDecParam.prCurrFBufInfo->u4H -
        ((prVdec->rDecParam.prSPS->fgFrameMbsOnlyFlag) ?
        prVdec->rDecParam.prSPS->u4FrameCropBottomOffset :
        (prVdec->rDecParam.prSPS->u4FrameCropBottomOffset << 1)));
    //mvc, compare by frame
    if((prDispInfo->u1PicStruct != FRAME) && prVdec->rDecParam.fgIsMVC)
    {
        prDispInfo->pvYAddr = NULL;
        //LOG(3, "[MVC]skip field cmp\n");
    }
    #else
    prDispInfo->u1PicStruct = prVdec->rDecParam.u1PicStruct;
    prDispInfo->u4H = prVdec->rDecParam.prCurrFBufInfo->u4H;
    #endif
    prDispInfo->u4YSize = prVdec->rDecParam.arFBufInfo[u1ViewIdx][u1BufIdx].u4DramPicSize;
    prDispInfo->pvCAddr = (void*)prVdec->rDecParam.arFBufInfo[u1ViewIdx][u1BufIdx].u4CAddr;
    prDispInfo->u4CSize = prDispInfo->u4YSize >> 1;
    prDispInfo->pvMVAddr = (void*)prVdec->rDecParam.arFBufInfo[u1ViewIdx][u1BufIdx].u4MVBufAddr;
    
    prDispInfo->u4W = prVdec->rDecParam.prCurrFBufInfo->u4W;
    prDispInfo->u4ChromaFormatIdc = prVdec->rDecParam.prSPS->u4ChromaFormatIdc;
	
    #if defined(CRCFLOWTEST) || defined(MULTIMEDIA_AUTOTEST_SUPPORT) || defined(VDEC_CRC_TEST)
    #if defined(VDEC_CRC_TEST) && !defined(MULTIMEDIA_AUTOTEST_SUPPORT) && !defined(CRCFLOWTEST)
    if(VDEC_IsTestCRC(ucEsId))
    #endif
    {
        _VDEC_AVCYC(prDispInfo);
    }
	#endif
	
    #ifdef TEST_ONOFF_RUNTIME
    _VDEC_UnlockVld(ucEsId, prVdecEsInfo->u4VldId);
    #endif

    #ifdef CC_STRESS_TEST_CRC_CHK
    if(prVdec->rDecParam.u4RecordingCRC > 0)
    {
        if(prVdec->rPesInfo.u4PTS == prVdec->rDecParam.u4ChkPTS)
        {
            prVdec->rDecParam.fgStartChkCRC = TRUE;
        }
        if(prVdec->rDecParam.fgStartChkCRC)
        {
            //PP CRC
            prVdec->rDecParam.u4RecordingCRC--;
            for(u1BufIdx = 0; u1BufIdx < PP_CRC_NUM; u1BufIdx++)
            {
                prVdec->rDecParam.au4RecYcrc[prVdec->rDecParam.u4RecordingCRC][u1BufIdx] = prDispInfo->u4Ycrc[u1BufIdx];
                prVdec->rDecParam.au4RecCcrc[prVdec->rDecParam.u4RecordingCRC][u1BufIdx] = prDispInfo->u4Ccrc[u1BufIdx];
            }
            //Bs input/VLD
            prVdec->rDecParam.u4RecBsCrc[prVdec->rDecParam.u4RecordingCRC] = AVC_VLD_READ32(RO_AVLD_RESISUAL_SUM);
            // 0:3 CRC check, 4:7 MC input check, 8:9 MC check, 10 MC Dram check
            for(u1BufIdx = PP_CRC_NUM; u1BufIdx < CRC_TOTAL_CNT; u1BufIdx++)
            {
                prVdec->rDecParam.au4RecYcrc[prVdec->rDecParam.u4RecordingCRC][u1BufIdx] = prDispInfo->u4Ycrc[u1BufIdx];
            }
            prVdec->rDecParam.au4RecDramcrc[prVdec->rDecParam.u4RecordingCRC][0] = prVdec->rDecParam.u4DramMC_R_CRC;
            prVdec->rDecParam.au4RecDramcrc[prVdec->rDecParam.u4RecordingCRC][1] = prVdec->rDecParam.u4DramPP_W_CRC;

            LOG(5, "CRC Record OK, pts 0x%x~\n", prVdec->rPesInfo.u4PTS);
        }
        if(prVdec->rDecParam.u4RecordingCRC == 0)
        {
            prVdec->rDecParam.fgStartChkCRC = FALSE;
        }
    }

    if(prVdec->rDecParam.fgEnCRC && (!prVdec->rDecParam.fgStartChkCRC))
    {
        if(prVdec->rPesInfo.u4PTS == prVdec->rDecParam.u4ChkPTS)
        {
            prVdec->rDecParam.u4VerifyCRC = CRC_CHK_FRM_NUM;
        }
        if(prVdec->rDecParam.u4VerifyCRC > 0)
        {
            prVdec->rDecParam.u4VerifyCRC--;
            if(fgIsTargetFrm(prVdec->rDecParam.u4VerifyCRC))
            {
                LOG(5, "CRC check, pts 0x%x, ", prVdec->rPesInfo.u4PTS);
                //Bs input/VLD
                if(prVdec->rDecParam.u4RecBsCrc[prVdec->rDecParam.u4VerifyCRC] != AVC_VLD_READ32(RO_AVLD_RESISUAL_SUM))
                {
                    LOG(5, "Bs Input Checksum mismatch! 0x%x\n", AVC_VLD_READ32(RO_AVLD_RESISUAL_SUM));
                    prVdec->rDecParam.u4BsFailCnt++;
                    prVdec->rDecParam.u4VerifyCRC = 0;
                }
                else
                {
                    //PP CRC
                    for(u1BufIdx = 0; u1BufIdx < PP_CRC_NUM; u1BufIdx++)
                    {
                        if(prVdec->rDecParam.au4RecYcrc[prVdec->rDecParam.u4VerifyCRC][u1BufIdx] != prDispInfo->u4Ycrc[u1BufIdx])
                        {
                            LOG(5, "PP CRC Y Fail~\n");
                            prVdec->rDecParam.au4CrcFailCnt[0]++;
                            fgCRCFail = TRUE;
                            break;
                        }
                        if(prVdec->rDecParam.au4RecCcrc[prVdec->rDecParam.u4VerifyCRC][u1BufIdx] != prDispInfo->u4Ccrc[u1BufIdx])
                        {
                            LOG(5, "PP CRC C Fail~\n");
                            prVdec->rDecParam.au4CrcFailCnt[1]++;
                            fgCRCFail = TRUE;
                            break;
                        }
                    }
                    //MC Input
                    for(u1BufIdx = PP_CRC_NUM; u1BufIdx < CRC_TOTAL_CNT; u1BufIdx++)
                    {
                        if(prVdec->rDecParam.au4RecYcrc[prVdec->rDecParam.u4VerifyCRC][u1BufIdx] != prDispInfo->u4Ycrc[u1BufIdx])
                        {
                            LOG(5, "Reg %d mismatch~\n", u1BufIdx);
                            prVdec->rDecParam.au4CrcFailCnt[u1BufIdx]++;
                            fgCRCFail = TRUE;
                        }
                    }
                    //DramC crc
                    if(prVdec->rDecParam.au4RecDramcrc[prVdec->rDecParam.u4VerifyCRC][0] != prVdec->rDecParam.u4DramMC_R_CRC)
                    {
                        LOG(5, "DramC Crc MC R mismatch~\n");
                        fgCRCFail = TRUE;
                    }
                    if(prVdec->rDecParam.au4RecDramcrc[prVdec->rDecParam.u4VerifyCRC][1] != prVdec->rDecParam.u4DramPP_W_CRC)
                    {
                        LOG(5, "DramC Crc PP W mismatch~\n");
                        fgCRCFail = TRUE;
                    }
                    if(!fgCRCFail)
                    {
                        LOG(5, "CRC Pass~\n");
                        prVdec->rDecParam.u4CrcPassCnt++;
                    }
                    else
                    {
                        ASSERT(0);
                    }
                }
            }
        }
    }
    #endif
    
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
    #if defined(VDEC_CRC_TEST) || defined(MULTIMEDIA_AUTOTEST_SUPPORT)
    #if defined(VDEC_CRC_TEST) && !defined(MULTIMEDIA_AUTOTEST_SUPPORT)
    if(VDEC_IsTestCRC(ucEsId))
    #endif
    {
        UINT8 i;
        for(i=0; i<4;i++)
        {
            prVdecEsInfo->u4CrcResult ^= prDispInfo->u4Ycrc[i];
            prVdecEsInfo->u4CrcResult ^= prDispInfo->u4Ccrc[i];
        }
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
#ifdef CC_STRESS_TEST_CRC_CHK
    else if(u4Type==(UINT32)RECORD_CRC)
    {
        prVdec->rDecParam.u4ChkPTS = u4Param1;
        prVdec->rDecParam.u4RecordingCRC = CRC_CHK_FRM_NUM;
        _VDEC_EnableCRC(TRUE);
    }
    else if(u4Type==(UINT32)CRC_CHK)
    {
        prVdec->rDecParam.fgEnCRC = (BOOL)u4Param1;
        _VDEC_EnableCRC(prVdec->rDecParam.fgEnCRC);
        if(!prVdec->rDecParam.fgEnCRC)
        {
            x_memset((void*)&prVdec->rDecParam.au4CrcFailCnt, 0, sizeof(prVdec->rDecParam.au4CrcFailCnt));
            prVdec->rDecParam.u4CrcPassCnt = 0;
            prVdec->rDecParam.u4BsFailCnt = 0;
        }
    }
#endif

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
        if(prVdec->rDecParam.u1CurViewIdx >= MAX_VIEW_NUM)
        {
            LOG(1, "u1CurViewIdx Err!\n");
            return;
        }
        *pu4Param1 = (UINT32)prVdec->rDecParam.aucFbgId[prVdec->rDecParam.u1CurViewIdx];
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
            *pu4Param1 = prVdec->rDecParam.aucFbgId[0];
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
#ifdef CC_STRESS_TEST_CRC_CHK
    else if(u4Type==(UINT32)CRC_CHK)
    {
        *pu4Param1 = (UINT32)&prVdec->rDecParam.au4CrcFailCnt;
        *pu4Param2 = prVdec->rDecParam.u4BsFailCnt;
        *pu4Param3 = prVdec->rDecParam.u4CrcPassCnt;
    }
#endif

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
        #ifndef CC_DRIVER_DEMO
        LOG(2, "H264 ES(%d) DMX_UpdVRPtr Fail CurRptr 0x%x, VS 0x%x, VE 0x%x, PicS 0x%x, DmxWptr 0x%x\n", 
            prVdec->rPesInfo.ucEsId, u4CurRptr, 
            prVdec->rPesInfo.u4FifoStart, prVdec->rPesInfo.u4FifoEnd, 
            prVdec->rPesInfo.u4VldReadPtr, prVdec->rDecParam.u4WPtr);
        #endif
    }
}

void _VDEC_H264SetDmxWptr(UCHAR ucEsId, UINT32 u4Wptr)
{
    VDEC_H264_INFO_T* prVdec;    

    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];
    
    prVdec->rDecParam.u4WPtr = u4Wptr;
    _VDEC_SetWptr(ucEsId, u4Wptr);
}

//skip ES if return FALSE
BOOL _VDEC_H264EsFlwCtrl(UCHAR ucEsId, UCHAR* pucVldId, INT8 i1EsBufStatus, UCHAR ucSkipMode)
{
    UCHAR ucFbgId;
    VDEC_H264_INFO_T* prVdec;
    VDEC_ES_INFO_T* prVdecEsInfo;
    VDEC_ES_INFO_T* prVdecEsInfoSub;
    UINT8 u1ViewIdx;

    ASSERT(ucEsId < MAX_ES_NUM);
    ASSERT(pucVldId !=NULL);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    u1ViewIdx = prVdec->rDecParam.u1CurViewIdx;
    if(u1ViewIdx >= MAX_VIEW_NUM)
    {
        LOG(1, "u1ViewIdx Err!\n");
        return FALSE;
    }
    ucFbgId = prVdec->rDecParam.aucFbgId[u1ViewIdx];
    //skip depend view when trick mode
    if((prVdecEsInfo->u4MMSpeed != STC_SPEED_TYPE_FORWARD_1X)
        && 
        (((prVdec->rDecParam.fgIsMVC) &&( IS_MVC_DEPEND_PIC_NALU(prVdec->rPesInfo.ucPicType)||((prVdecEsInfo->fgIsDependView)&&(!IS_AVC_PIC_NALU(prVdec->rPesInfo.ucPicType))))) ||
        ((prVdecEsInfo->e3DType == VDEC_3D_REALD_DUAL_STREAM) && (prVdec->rPesInfo.ucEsId > 0))))
    {
        #if defined(CC_MVC_SUPPORT) && !defined(CC_VDEC_MVC_DISABLE)
        //clr MVC base view pending buffer
        if(prVdec->rDecParam.u4WaitClrBufIdx < 0xFF)
        {
            //LOG(3, "clr posponed buf %d\n", prDecPrm->u4WaitClrBufIdx);
            _VDEC_ClrFBufInfo(&prVdec->rDecParam, prVdec->rDecParam.u4WaitClrBufIdx); //should be base view
            prVdec->rDecParam.u4WaitClrBufIdx = 0xFF;
        }
        #endif
        if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, 
            prVdec->rPesInfo.u4VldReadPtr, prVdec->rPesInfo.u4VldReadPtr))                
        {                    
            LOG(2, "EsFlwCtrl H264 ES(%d) DMX_UpdateVideoReadPointer Fail\n", prVdec->rPesInfo.ucEsId);                
        }
        return TRUE;
    }

	 if(prVdecEsInfo->fgSpeedChg)
    {
        LOG(0, "SpeedChg %d->%d\n", prVdec->rDecParam.ucSkipMode, ucSkipMode);
        if((prVdec->rDecParam.fgIsMVC)&&(((prVdec->rDecParam.ucSkipMode == IPMode)&&(ucSkipMode == IPBMode))
			|| ((prVdecEsInfo->u4MMSpeed == STC_SPEED_TYPE_FORWARD_1X)&&(prVdec->rDecParam.ucSkipMode == IPBMode)
			&&(ucSkipMode == IPBMode))))
        {
            SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
            SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_AVC_RAP);
            FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);
			_VDEC_ClrDPB(&prVdec->rDecParam);
            FBM_ReleaseDispQ(prVdecEsInfo->ucFbgId);
            prVdec->rDecParam.u1CurViewIdx = 1 - prVdec->rDecParam.u1CurViewIdx;
            if(prVdecEsInfo->ucSubFbgId != FBM_FBG_ID_UNKNOWN)
            {
                FBM_SetFrameBufferFlag(prVdecEsInfo->ucSubFbgId, FBM_FLAG_SEEK_MODE);
				
				_VDEC_ClrDPB(&prVdec->rDecParam);
                FBM_ReleaseDispQ(prVdecEsInfo->ucSubFbgId);
				
                FBM_ClrFrameBufferFlag(prVdecEsInfo->ucSubFbgId, FBM_FLAG_SEEK_MODE);
            }
            prVdec->rDecParam.u1CurViewIdx = 1 - prVdec->rDecParam.u1CurViewIdx;
            FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);
        }
		prVdecEsInfo->fgSpeedChg = FALSE;
    }
	 prVdec->rDecParam.ucSkipMode = ucSkipMode;

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
        if(prVdecEsInfo->e3DType == VDEC_3D_MVC)
        {
            LOG(3, "DEC_FLG_WAIT_NEXT_I FlushDPB, View%d\n", prVdec->rDecParam.u1CurViewIdx);
            _VDEC_ClrDPB(&prVdec->rDecParam);
            prVdec->rDecParam.u1CurViewIdx = 1 - prVdec->rDecParam.u1CurViewIdx;
            _VDEC_ClrDPB(&prVdec->rDecParam);
            prVdec->rDecParam.u1CurViewIdx = 1 - prVdec->rDecParam.u1CurViewIdx;
        }
        else
        {
            LOG(3, "DEC_FLG_WAIT_NEXT_I FlushDPB, View%d\n", prVdec->rDecParam.u1CurViewIdx);
        _VDEC_FlushDPB(&prVdec->rDecParam);
        }
        CLR_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    }
    //if(prVdec->rDecParam.fgIsMVC)
    if(((prVdecEsInfo->e3DType == VDEC_3D_MVC) || 
        (prVdecEsInfo->e3DType == VDEC_3D_REALD_DUAL_STREAM))
        && 
        (ucEsId != prVdec->rPesInfo.ucEsId))
    {
        prVdecEsInfoSub = _VDEC_GetEsInfo(prVdec->rPesInfo.ucEsId);
        if (IS_DECFLAG_SET(prVdecEsInfoSub, DEC_FLG_WAIT_NEXT_I))
        {
            LOG(3, "DEC_FLG_WAIT_NEXT_I FlushDPB, View%d\n", prVdec->rPesInfo.ucEsId);
            prVdec->rDecParam.u1CurViewIdx = prVdec->rPesInfo.ucEsId;
            #if defined(CC_MVC_SUPPORT) && !defined(CC_VDEC_MVC_DISABLE)
            if(prVdec->rPesInfo.ucEsId < MAX_VIEW_NUM)
            {
                prVdec->rDecParam.u4CurViewID = prVdec->rDecParam.au4ViewIDList[prVdec->rPesInfo.ucEsId];
            }
            #endif
            _VDEC_FlushDPB(&prVdec->rDecParam);
            CLR_DECFLAG(prVdecEsInfoSub, DEC_FLG_WAIT_NEXT_I);
        }
    }

    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_WAIT_AVC_RAP))
    {
        LOG(3, "DEC_FLG_WAIT_AVC_RAP\n");
        prVdec->rDecParam.rSEI.u1ExtMatchFlg = FALSE;
        prVdec->rDecParam.rSEI.u1WaitRAPCnt = 0;
        prVdec->rDecParam.rSEI.u1WaitRAPRefCnt = 0;
        prVdec->rDecParam.rSEI.u1WaitRAPICnt = 0;
        prVdec->rDecParam.fgIsFirstFrm = FALSE;
        prVdec->rDecParam.u1CurViewIdx = 0;
		if(prVdec->rDecParam.fgIsMVC)
        {
            prVdec->rDecParam.fgWaitBaseView = TRUE;
            #if defined(CC_MVC_SUPPORT) && !defined(CC_VDEC_MVC_DISABLE)
            prVdec->rDecParam.u4WaitClrBufIdx = 0xFF;
            #endif
        }
        #ifdef ENABLE_MULTIMEDIA
        if(((prVdecEsInfo->eContainerType >= SWDMX_FMT_MPEG2_TS) &&
            (prVdecEsInfo->eContainerType <= SWDMX_FMT_MPEG2_TS_TIME_SHIFT)) ||
            (prVdecEsInfo->eContainerType == SWDMX_FMT_H264_VIDEO_ES))
        {
            prVdec->rDecParam.fgWaitPPS = TRUE;
			prVdec->rDecParam.u1WaitPPSSkipFrmCnt = WAIT_PPS_SKIP_FRM_NUM;
			prVdec->rDecParam.ucBNum = 0;
        }
		prVdec->rDecParam.i4LastPOC[u1ViewIdx] = -99999;
	    prVdec->rDecParam.rLastInfo[u1ViewIdx].u4LastFrameNum = 0xffffffff;
	    prVdec->rDecParam.rLastInfo[u1ViewIdx].i4LastFrameNumOffset = 0;
	    prVdec->rDecParam.rLastInfo[u1ViewIdx].fgLastIsRef = FALSE;
	    prVdec->rDecParam.rLastInfo[u1ViewIdx].i4LastRefTFldPOC = 0;
	    prVdec->rDecParam.rLastInfo[u1ViewIdx].i4LastRefBFldPOC = 0;
	    prVdec->rDecParam.rLastInfo[u1ViewIdx].i4LastRefPOC = 0;
	    prVdec->rDecParam.rLastInfo[u1ViewIdx].i4LastRefPOCLsb = 0;
	    prVdec->rDecParam.rLastInfo[u1ViewIdx].i4LastRefPOCMsb = 0;
	    prVdec->rDecParam.rLastInfo[u1ViewIdx].i4LastTFldPOC = 0;
	    prVdec->rDecParam.rLastInfo[u1ViewIdx].i4LastBFldPOC = 0;
	    prVdec->rDecParam.rLastInfo[u1ViewIdx].fgLastMmco5 = FALSE;
        if(((prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) ||
            (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS_ENCRYPT_192))
            && prVdec->rDecParam.prSPS) // find new parameter set
        {   
            prVdec->rDecParam.prSPS->fgSPSValid = FALSE;
            
            prVdec->rDecParam.rLastInfo[u1ViewIdx].u4LastFrameNum = 0xffffffff;
            prVdec->rDecParam.i4LastPOC[u1ViewIdx] = -99999;
            prVdec->rDecParam.rLastInfo[u1ViewIdx].i4LastRefPOCMsb = 0;
            prVdec->rDecParam.rLastInfo[u1ViewIdx].i4LastRefPOCLsb = 0;
            prVdec->rDecParam.rLastInfo[u1ViewIdx].i4LastFrameNumOffset = 0;

            prVdec->rDecParam.prSliceHdr->ai4DeltaPicOrderCnt[0] = 0;
            prVdec->rDecParam.i4FrmNumOffset = 0;
        }   
        #endif
        CLR_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_AVC_RAP);
    }
	//start from base view after wait RAP
	   if((prVdec->rDecParam.fgIsMVC) && IS_MVC_DEPEND_PIC_NALU(prVdec->rPesInfo.ucPicType) &&
		   prVdec->rDecParam.fgWaitBaseView)
	   {
		   if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, 
			   prVdec->rPesInfo.u4VldReadPtr, prVdec->rPesInfo.u4VldReadPtr))				 
		   {					
			   LOG(2, "EsFlwCtrl H264 ES(%d) DMX_UpdateVideoReadPointer Fail\n", prVdec->rPesInfo.ucEsId);				  
		   }
		   return TRUE;
	   }
	   else if(prVdec->rDecParam.fgWaitBaseView && IS_MVC_BASE_PIC_NALU(prVdec->rPesInfo.ucPicType))
	   {
		   prVdec->rDecParam.fgWaitBaseView = FALSE;
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
            
            if(u1ViewIdx < MAX_VIEW_NUM)
            {
                prVdec->rDecParam.i4LastPOC[u1ViewIdx] = -99999;
                prVdec->rDecParam.rLastInfo[u1ViewIdx].u4LastFrameNum = 0xffffffff;
            }
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
        #ifndef FBM_VIRTUAL_DISPLAY
        // Check if display is ready, skip picture if NOT
        if (!FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_DISP_READY))
        {
            LOG(3, "Disp Not Ready\n");
            *pucVldId = VDEC_MAX_VLD;
            return TRUE;    //skip current packet
        }
        #endif
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
#ifndef CC_DRIVER_DEMO
    VDEC_H264_INFO_T* prVdec;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    VDEC_PES_INFO_T rPesInfo;
    VDEC_ES_INFO_T* prVdecEsInfo;

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsmQId);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsmQId];
    
    ASSERT(pvPesInfo!=NULL);
    ASSERT(prVdecEsInfo!=NULL);

    //flush Q
    zMsgSize = sizeof(VDEC_PES_INFO_T);
    while (x_msg_q_receive(&u2MsgQIdx, &rPesInfo, &zMsgSize,
        &hMsgQueue, 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
    }

    if(prVdec->rDecParam.aucFbgId[0] != FBM_FBG_ID_UNKNOWN)
    {
        //flush DPB
        _VDEC_H264Flush(ucEsmQId, FALSE);
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_AVC_RAP);
    }
    
#else
    ASSERT(pvPesInfo!=NULL);

    //temp code
    do
    {
        x_thread_delay(1);
    }
    while (x_msg_q_send(hMsgQueue, pvPesInfo, sizeof(VDEC_PES_INFO_T), 255) != OSR_OK);
    UNUSED(ucEsmQId);
#endif
    
    UNUSED(ucVldId);
    UNUSED(hMutex);
    return 1;
}


#ifdef VDEC_TIME_PROFILE
BOOL  _VDEC_H264IsPic(UCHAR ucEsId, VDEC_PES_INFO_T* prPes)
{
    UINT8 u1NalUnitType;
    
    u1NalUnitType = (prPes->ucPicType& 0x1f);
    if((u1NalUnitType == NON_IDR_SLICE)||(u1NalUnitType == IDR_SLICE)||(u1NalUnitType == CODED_SLICE_EXT))
    {
        if(!fgIsFrmStart)
        {
            fgIsFrmStart = TRUE;
             return FALSE;
        }
        else
        {
            fgIsFrmStart = TRUE;
            return TRUE;
        }
        
    }
    else
    {
        if(fgIsFrmStart)
        {
            fgIsFrmStart = FALSE;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }


}
#endif

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
        if(prVdec->rDecParam.u1CurViewIdx >= MAX_VIEW_NUM)
        {
            LOG(1, "u1CurViewIdx Err!\n");
            return;
        }
        prVdec->rDecParam.au4DispPts[prVdec->rDecParam.u1CurViewIdx] = u4Pts;
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

void _VDEC_H264SetIsr(UCHAR ucEsId)
{
    _VDEC_InitVDecHW(FALSE);
    UNUSED(ucEsId);
}

BOOL _VDEC_MVCFindLRPair(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    HANDLE_T hMsgQ;
    UINT16 u2MsgQIdx;
    UCHAR ucMvcpare = 2;
    UCHAR ucDelayTimes = 200;
    SIZE_T zMsgSize;
    VDEC_PES_INFO_T rPesInfo;
    UINT32 au4BasePTS[MAX_VERIFY_FRAME];
    UINT32 au4DepPTS[MAX_VERIFY_FRAME];
    UINT32 u4QIdx = 0, u4BaseIdx, u4DepIdx, u4CurPTS, u4SkipCnt;
    VDEC_H264_INFO_T* prVdec;
    
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_H264_INFO_T*)&rH264DecInfo[ucEsId];

    x_memset((void*)(&rPesInfo), 0, sizeof(VDEC_PES_INFO_T));
    au4BasePTS[u4QIdx] = prVdecEsInfo->u4BasePTS;
    au4DepPTS[u4QIdx++] = prVdecEsInfoKeep->rPesInfo.u4PTS;
    while(ucMvcpare&&ucDelayTimes)
    {
        ucMvcpare = 2;
        hMsgQ = prVdecEsInfoKeep->hMsgQSub;
        VERIFY(x_msg_q_num_msgs(hMsgQ, &(prVdecEsInfo->u2Count)) == OSR_OK);
        if(prVdecEsInfo->u2Count >30)
        {
            ucMvcpare--;
        }
        else
        {
            x_thread_delay(30);
            ucDelayTimes--;
        }

        hMsgQ = prVdecEsInfoKeep->hMsgQ;
        VERIFY(x_msg_q_num_msgs(hMsgQ, &(prVdecEsInfo->u2Count)) == OSR_OK);
        if(prVdecEsInfo->u2Count >10)
        {
            ucMvcpare--;
        }
        else
        {
            x_thread_delay(10);
            ucDelayTimes--;
        }
    }
    while(u4QIdx < MAX_VERIFY_FRAME)
    {
        //advance depend view
        u4SkipCnt = 0;
        hMsgQ = prVdecEsInfoKeep->hMsgQSub;
        x_memset((void*)(&rPesInfo), 0, sizeof(VDEC_PES_INFO_T));
        while(!IS_MVC_DEPEND_PIC_NALU(rPesInfo.ucPicType))
        {
            if(x_msg_q_receive(&u2MsgQIdx, &rPesInfo, &zMsgSize,
                &(hMsgQ), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
            {
                if(u4SkipCnt++ > MAX_VERIFY_FRAME)
                {
                    return FALSE;
                }
            }
            else    //OSR_NO_MSG
            {
                return FALSE;   
            }
        }
        au4DepPTS[u4QIdx] = rPesInfo.u4PTS;     //dep view advance 1 frame
        u4CurPTS = au4BasePTS[u4QIdx - 1];           //current base frame

        for(u4BaseIdx = 0; u4BaseIdx < u4QIdx; u4BaseIdx++)
        {
            if(au4DepPTS[u4QIdx] == au4BasePTS[u4BaseIdx])
            {
                hMsgQ = prVdecEsInfoKeep->hMsgQSub;
                u4SkipCnt = 0;
                while((rPesInfo.u4PTS != u4CurPTS) || !IS_MVC_DEPEND_PIC_NALU(rPesInfo.ucPicType))
                {
                    if(x_msg_q_receive(&u2MsgQIdx, &rPesInfo, &zMsgSize,
                        &(hMsgQ), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
                    {
                        if(u4SkipCnt++ > MAX_VERIFY_FRAME)
                        {
                            //can't find frame with same pts
                            return FALSE;
                        }
                    }
                    else    //OSR_NO_MSG
                    {
                        return FALSE;   
                    }
                }
                if(0)//u4CurPTS == prVdecEsInfo->u4BasePTS)
                {
                    x_memcpy((void*)&prVdecEsInfoKeep->rPesInfo, (void*)&rPesInfo, sizeof(VDEC_PES_INFO_T));
                    x_memcpy((void*)&prVdec->rPesInfo, (void*)&rPesInfo, sizeof(VDEC_PES_INFO_T));
                }
                else
                {
                    _VDEC_ClrDPB(&prVdec->rDecParam);
                    prVdec->rDecParam.u1CurViewIdx = 1 - prVdec->rDecParam.u1CurViewIdx;
                    _VDEC_ClrDPB(&prVdec->rDecParam);
                    prVdec->rDecParam.u1CurViewIdx = 1 - prVdec->rDecParam.u1CurViewIdx;
                }
                return TRUE;
            }
        }

        // advance base view
        u4SkipCnt = 0;
        hMsgQ = prVdecEsInfoKeep->hMsgQ;
        x_memset((void*)(&rPesInfo), 0, sizeof(VDEC_PES_INFO_T));
        while(!IS_MVC_BASE_PIC_NALU(rPesInfo.ucPicType))
        {
            if(x_msg_q_receive(&u2MsgQIdx, &rPesInfo, &zMsgSize,
                &(hMsgQ), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
            {
                if(u4SkipCnt++ > MAX_VERIFY_FRAME)
                {
                    return FALSE;
                }
            }
            else    //OSR_NO_MSG
            {
                return FALSE;   
            }            
        }        
        au4BasePTS[u4QIdx] = rPesInfo.u4PTS;    //base view advance 1 frame
        u4CurPTS = au4DepPTS[u4QIdx];       //current depend frame

        for(u4DepIdx = 0; u4DepIdx <= u4QIdx; u4DepIdx++)
        {
            if(au4BasePTS[u4QIdx] == au4DepPTS[u4DepIdx])
            {
                hMsgQ = prVdecEsInfoKeep->hMsgQ;
                u4SkipCnt = 0;
                while((rPesInfo.u4PTS != u4CurPTS) || !IS_MVC_BASE_PIC_NALU(rPesInfo.ucPicType))
                {
                    if(x_msg_q_receive(&u2MsgQIdx, &rPesInfo, &zMsgSize,
                        &(hMsgQ), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
                    {
                        if(u4SkipCnt++ > MAX_VERIFY_FRAME)
                        {
                            //can't find frame with same pts
                            return FALSE;
                        }
                    }
                    else    //OSR_NO_MSG
                    {
                        return FALSE;   
                    }
                }
                _VDEC_ClrDPB(&prVdec->rDecParam);
                prVdec->rDecParam.u1CurViewIdx = 1 - prVdec->rDecParam.u1CurViewIdx;
                _VDEC_ClrDPB(&prVdec->rDecParam);
                prVdec->rDecParam.u1CurViewIdx = 1 - prVdec->rDecParam.u1CurViewIdx;
                return TRUE;
            }
        }
        u4QIdx++;
    }    
    return FALSE;
}

UCHAR _VDEC_H264GetFRateFromSPS(UCHAR ucEsId, UINT8* p1Buf, UINT32 u4BufSz)
{
    return _VDEC_GetFRateFromSPS(ucEsId, p1Buf, u4BufSz);
}
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------



