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
 * $RCSfile: vdec_rmapi.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_RVapi.c
 *  This file contains implementation of exported APIs of RealVideo VDEC.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
//#include "vdec_rvpsr.h"
#include "vdec_rvdec.h"
#include "vdec_rvutil.h"
#include "vdec_rvapi.h"
#include "vdec_rvhw.h"
#include "vdec_rvpsr.h"
#if 1 // swdmx multi-instance
#include "swdmx_drvif.h"
#endif
LINT_EXT_HEADER_BEGIN
#include "x_timer.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_chip_id.h"
#include "x_common.h"
#include "x_typedef.h"
#include "dmx_if.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define RV_MAX_ES_NUM 2
#define ES_ID_0 0


//global variable 
VDEC_RV_INFO_T _rRVDecInfo[VDEC_MAX_ES];


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


static VOID _VDEC_RVPrsPushDataHdr(VDEC_RV_INFO_T* prVdec)
{
    UINT8 *pu1Data = (UINT8 *)VIRTUAL(prVdec->rPesInfo.u4VldReadPtr);
    UINT8 *pu1FifoStart = (UINT8 *)VIRTUAL(prVdec->rPesInfo.u4FifoStart);
    UINT8 *pu1FifoEnd = (UINT8 *)VIRTUAL(prVdec->rPesInfo.u4FifoEnd);
    UINT8 *pu1TgtSa = (UINT8 *)prVdec->rRvPicAttr.au4SliceSize;
    UINT8 *pu1SrcEa;
    UINT32 u4FifoSize = prVdec->rPesInfo.u4FifoEnd - prVdec->rPesInfo.u4FifoStart;
    UINT32 u4SliceNum, u4CpySz;

    u4SliceNum = (UINT32)*pu1Data;
    ASSERT(u4SliceNum < 128);

    pu1Data += 4;
    if (pu1Data >= pu1FifoEnd)
    {
        pu1Data -= u4FifoSize;
    }

    u4CpySz = u4SliceNum * 4;
    pu1SrcEa = pu1Data + u4CpySz;
    if (pu1SrcEa <= pu1FifoEnd)
    {
        x_memcpy(pu1TgtSa, pu1Data, u4CpySz);
    }
    else
    {
        UINT32 u4CpySz2 = (UINT32)pu1SrcEa - (UINT32)pu1FifoEnd;
        UINT32 u4CpySz1 = u4CpySz - u4CpySz2;
        x_memcpy(pu1TgtSa, pu1Data, u4CpySz1);
        x_memcpy(pu1TgtSa + u4CpySz1, pu1FifoStart, u4CpySz2);
    }

    prVdec->rPesInfo.u4VldReadPtr += u4SliceNum * 8 + 1;
    if (prVdec->rPesInfo.u4VldReadPtr >= prVdec->rPesInfo.u4FifoEnd)
    {
        prVdec->rPesInfo.u4VldReadPtr -= u4FifoSize;
    }

    prVdec->rRvPicAttr.u4NumSlice = u4SliceNum;
}


//-----------------------------------------------------------------------------
// Typedef
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

BOOL _VDEC_IsSupportRV(VOID)
{
    return IS_SupportRM();
}


INT32 _VDEC_RVInit(UCHAR ucEsId)
{
    VDEC_RV_INFO_T* prVdec;
    
    prVdec = (VDEC_RV_INFO_T*)&_rRVDecInfo[ucEsId];
    x_memset((void*)prVdec, 0, sizeof(VDEC_RV_INFO_T));

    return (INT32)E_VDEC_OK;
}


INT32 _VDEC_RVHdrPrsPic(UCHAR ucVdecId, UCHAR ucEsId)
{
    return (INT32)E_VDEC_OK;
}


INT32 _VDEC_RVHdrPrsNalu(UCHAR ucVdecId, UCHAR ucEsId)
{
    //VDEC_RV_INFO_T* prVdec;
    
    return (INT32)E_VDEC_OK;
}


/** .
*@param .
 */
INT32 _VDEC_RVHdrPrs(UCHAR ucVdecId, UCHAR ucEsId)
{
  INT32 i4RetValue;

  _RVHdrPrs(ucEsId, &i4RetValue);
  
  return i4RetValue;
}


INT32 _VDEC_RVDecStart(UCHAR ucVdecId, UCHAR ucEsId)
{
  INT32 i4RetValue;
  
  _RVDecStart(ucEsId, &i4RetValue);
  
  return i4RetValue;
}


INT32 _VDEC_RVDecFin(UCHAR ucVdecId, UCHAR ucEsId)
{
    INT32 i4RetValue;
    
    _RVDecFin(ucEsId, &i4RetValue);
    
    return i4RetValue;
}


VOID _VDEC_RVSWRst(UCHAR ucVdecId, UCHAR ucEsId)
{
#if 0 // do nothing at common part
    INT32 i4RetValue;
    _VDEC_LockVld(ucEsId, ucVdecId);
    _RVSWRst(ucEsId, &i4RetValue);
    _VDEC_UnlockVld(ucEsId, ucVdecId);
#endif
}


VOID _VDEC_RVStop(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_RV_INFO_T *prVdec;

    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
#ifdef USE_FBM
    if(prVdec->rOther.u1FbgId != FBM_FBG_ID_UNKNOWN)
    {
        FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, ucEsId);
        FBM_WaitUnlockFrameBuffer(prVdec->rOther.u1FbgId, 20);    // block until display unlock            
        FBM_ReleaseGroup(prVdec->rOther.u1FbgId);
        prVdec->rOther.u1FbgId = FBM_FBG_ID_UNKNOWN;
    }
#endif

    _VDEC_LockFlushMutex(ucEsId);

    if (prVdecEsInfo->fgFlushEsmQ)
    {
        _VDEC_FlushEs(ucEsId);
        prVdecEsInfo->fgFlushEsmQ = FALSE;
        //Printf("_MPV_StopDecES prVdecEsInfo->fgFlushEsmQ = FALSE\n");
        _VDEC_UnlockFlushSema(ucEsId);        
    }
    _VDEC_ClearEsInfo(ucEsId);

    _VDEC_UnlockFlushMutex(ucEsId);
}


VOID _VDEC_RVGetPesStruct(UCHAR ucEsId, void **ppvPesInfo)
{
    VDEC_RV_INFO_T* prVdec;
    
    ASSERT(ppvPesInfo !=NULL);    
    ASSERT(ucEsId < VDEC_MAX_ES);
    prVdec = (VDEC_RV_INFO_T*)&_rRVDecInfo[ucEsId];
    *ppvPesInfo = (void*)(&prVdec->rPesInfo);
}

// !!!
VOID _VDEC_RVGetWholePesStruct(UCHAR ucEsId, void **ppvPesInfo)
{
    ASSERT(ppvPesInfo !=NULL);    
    ASSERT(ucEsId < VDEC_MAX_ES);

    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(0, "_VDEC_RVGetWholePesStruct ucEsId(%d) > VDEC_MAX_ES, oops...\n", ucEsId);
        //ASSERT(0);
        ucEsId = (VDEC_MAX_ES - 1);
    }
    *ppvPesInfo = (void*)(&_rRVDecInfo[ucEsId]);
}

VOID _VDEC_RVGetDispPic(UCHAR ucEsId, void* pvInfo)
{
  VDEC_RV_INFO_T* prVdec;
  VDEC_DISP_PIC_INFO_T* prDispInfo;

  _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);
  
  prDispInfo = (VDEC_DISP_PIC_INFO_T*)pvInfo;
  
  prDispInfo->u4YSize = prVdec->rRvPicAttr.u4Width * prVdec->rRvPicAttr.u4Height;
  prDispInfo->u4CSize = prDispInfo->u4YSize >> 1;
    
  prDispInfo->u4W = prVdec->rRvPicAttr.u4OrgWidth;
  prDispInfo->u4H = prVdec->rRvPicAttr.u4OrgHeight;

  prDispInfo->u4W_Cmp = prVdec->rRvPicAttr.u4Width;  //rWMVDecInfo.rSeqHdrR.u4PicWidthCmp;  !!!
  prDispInfo->u4H_Cmp = prVdec->rRvPicAttr.u4Height;  //rWMVDecInfo.rSeqHdrR.u4PicHeightCmp;  !!!
  
  prDispInfo->u4CurrentPicNo = prVdec->rOther.u4CurrPicNo;

  prDispInfo->pvYAddr = (void*)prVdec->rOther.rVSyncPrm.dwPtrY;
  prDispInfo->pvCAddr = (void*)prVdec->rOther.rVSyncPrm.dwPtrC;
}

VOID _VDEC_RVSetParam(UCHAR ucEsId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3)
{
#ifdef VDEC_TIME_PROFILE
    VDEC_RV_INFO_T *prVdec;
    RVOther *prOther;

    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);
    prOther = &prVdec->rOther;
    if(u4Type == (UINT32)RV_TIME_PROFILE2)
    {
        prOther->u1DramMoniter = (UINT8)u4Param1;
    }
#endif

    UNUSED(ucEsId);
    UNUSED(u4Type);
    UNUSED(u4Param1);
    UNUSED(u4Param2);
    UNUSED(u4Param3);
}


VOID _VDEC_RVGetParam(UCHAR ucEsId, UINT32 u4Type, UINT32* pu4Param1, UINT32* pu4Param2, UINT32* pu4Param3)
{
#ifdef VDEC_TIME_PROFILE
    VDEC_RV_INFO_T *prVdec;
    RVOther *prOther;
    RvPicAttr *prRvPicAttr;

    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);
    prOther = &prVdec->rOther;
    prRvPicAttr = &prVdec->rRvPicAttr;
    
    ASSERT(ucEsId < VDEC_MAX_ES);
    ASSERT(pu4Param1 !=NULL);
    ASSERT(pu4Param2 !=NULL);
    ASSERT(pu4Param3 !=NULL);

    if(u4Type == (UINT32)RV_TIME_PROFILE)
    {
        *pu4Param1 = prOther->u1FbgId;
        *pu4Param2 = prOther->u4CurDecMs;       //hw dec
    }
    else if(u4Type == (UINT32)RV_TIME_PROFILE2)
    {
        switch (prRvPicAttr->ePtype)
        {
        case RV_INTRAPIC:
        case RV_FORCED_INTRAPIC:
            *pu4Param2 = 2;
            break;

        case RV_INTERPIC:
            *pu4Param2 = 0;
            break;

        case RV_TRUEBPIC:
            *pu4Param2 = 1;
            break;
        }
         *pu4Param3 = 1;
    }
    else if(u4Type == (UINT32)RV_TIME_PROFILE3)
    {
        *pu4Param1 = prOther->u4CurSwMicro;
        *pu4Param2 = prOther->u4CurDecCycle;
        *pu4Param3 = prOther->u4CurDramCycle;
    }
    else if (u4Type == (UINT32)RV_DISP_IDX)
    {
        *pu4Param1 = prOther->u1FbgId;
//        *pu4Param2 = prOther->u1FbId;
        *pu4Param3 = 0;
    }
#endif

    UNUSED(ucEsId);
    UNUSED(u4Type);
    UNUSED(pu4Param1);
    UNUSED(pu4Param2);
    UNUSED(pu4Param3);
}

#ifdef VDEC_TIME_PROFILE
BOOL _VDEC_RvIsPic(UCHAR ucEsId, VDEC_PES_INFO_T *prPes)
{
    return TRUE;
}
#endif

VOID _VDEC_RVUpdDmxRptr(UCHAR ucEsId)
{
#ifndef CC_RV_EMULATION
    VDEC_RV_INFO_T* prVdec;
    UINT32 u4CurRptr;
    //UINT32 u4CurRptrBits;
    ASSERT(ucEsId < VDEC_MAX_ES);
    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);

    u4CurRptr = prVdec->rOther.u4LastVldReadPtr;//dwRVVldRPtr(&u4CurRptrBits);
    
    //u4CurRptr += prVdec->rPesInfo.u4FifoStart;
    LOG(6,"current read pointer : %x %x fifo start : %x\n",(u4CurRptr-prVdec->rPesInfo.u4FifoStart),u4CurRptr,prVdec->rPesInfo.u4FifoStart);
    
    //update dmx pointer
    if (!DMX_UpdateVideoReadPointer(ucEsId, (u4CurRptr) , (u4CurRptr)))                
    {                    
        LOG(0, "RV ES(%d) DMX_UpdateVideoReadPointer Fail\n", prVdec->rPesInfo.ucEsId);
    }
#endif
}


VOID _VDEC_RVSetDmxWptr(UCHAR ucEsId, UINT32 u4Wptr)
{
}


//skip ES if return TRUE
BOOL _VDEC_RVEsFlwCtrl(UCHAR ucEsId, UCHAR* pucVldId, INT8 i1EsBufStatus, UCHAR ucSkipMode)
{
    VDEC_RV_INFO_T* prVdec;
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    ASSERT(ucEsId < VDEC_MAX_ES);
    ASSERT(pucVldId !=NULL);

    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);

    if (prVdecEsInfoKeep->fgVPush) // data from ffmpeg
    {
        _VDEC_RVPrsPushDataHdr(prVdec);
    }
    else
    {
        extern void vGetRvSliceSz(UINT8 u1SrcId, UINT32 *pu4SliceSzArr, UINT32 u4Rp, UINT32 u4Cnt);
        prVdec->rRvPicAttr.u4NumSlice = prVdec->rPesInfo.u1SliceCount;
        vGetRvSliceSz(prVdecEsInfo->u1SwdmxSrcId, prVdec->rRvPicAttr.au4SliceSize, prVdec->rPesInfo.u4RenderPts, prVdec->rRvPicAttr.u4NumSlice);
    }

    if(!prVdec->fgInited)  //init state
    {
        LOG(6,"hello, it's first frame\n");
        _RVInit(ucEsId);
        vInitVParserRV(ucEsId);
#if !defined(VDEC_IS_POST_MT5368) // after 6898 this register is not used
        _VDEC_LockVld(ucEsId, prVdecEsInfo->u4VldId);
        vPowerOnVDec();
        _VDEC_UnlockVld(ucEsId, prVdecEsInfo->u4VldId);
#endif

#ifdef RV_TRACKING
        prVdec->rOther.u1TrackFbId = FBM_FB_ID_UNKNOWN;
#endif

#ifndef CC_RV_EMULATION
        if (prVdecEsInfo != NULL) 
        {
            prVdec->rRvPicAttr.u4OrgWidth =  (UINT32)prVdecEsInfo->rMMSeqInfo.u2_width;
            prVdec->rRvPicAttr.u4OrgHeight = (UINT32)prVdecEsInfo->rMMSeqInfo.u2_height;
        }
#else
        {
            extern VOID _VdecRvEmuGetWidthHeight(UINT32 *pu4OrgWidth, UINT32 *pu4OrgHeight);
            _VdecRvEmuGetWidthHeight(&prVdec->rRvPicAttr.u4OrgWidth, &prVdec->rRvPicAttr.u4OrgHeight);
        }
#endif
        vInitRVVdoMem(ucEsId);

        prVdec->fgInited = TRUE;
    }
    
    if ((prVdecEsInfo != NULL) && IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I))
    {
        FBM_InvalidateRefFrameBuffer(prVdecEsInfo->ucFbgId);
        CLR_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);

        /*if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC))
        {
            TOGGLE_DECFLAG(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC);
        }*/            
    }

    if (ucSkipMode == IMode)
    {
        //LOG(0,"I mode\n");        
        if(prVdec->rPesInfo.ucPicType != RV_INTRAPIC && prVdec->rPesInfo.ucPicType != RV_FORCED_INTRAPIC)
        {
            return TRUE;
        }
    }    
    else if (ucSkipMode == IPMode)
    {
        //LOG(0,"IP mode\n");
#if 0  // megaa 20090615: tmp solution for 2X to 1X, non-flush causing frame drop
        if(prVdec->rPesInfo.ucPicType == RV_TRUEBPIC)
            return TRUE;
#endif
    }

    return FALSE;
}


void _VDEC_RVStartPTS(UCHAR ucEsId, BOOL fgEnCalPTS, UINT32 u4Pts, UINT32 u4PtsDiff)
{
    VDEC_RV_INFO_T* prVdec;    

    ASSERT(ucEsId < VDEC_MAX_ES);
    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);

    prVdec->rOther.fgEnCalPTS= fgEnCalPTS;
    prVdec->rOther.fgFirstDispFrame = fgEnCalPTS;
    if(fgEnCalPTS)
    {            
        //prVdec->rOther.u4LastPTS = u4Pts;
        //prVdec->rOther.u4DeltaPTS= u4PtsDiff;
    }
}


VOID _VDEC_RVSetIsr(UCHAR ucEsId)
{
    #ifndef VDEC_COMMON_ISR
    x_os_isr_fct pfnOldIsr;
    #endif
#ifdef RV_IRQ_DISABLE    
    // unreg ISR
    #ifdef VDEC_COMMON_ISR
    _VDEC_RegIsr(RVVLD_VECTOR, NULL);
    #else
    if (x_reg_isr(RVVLD_VECTOR, NULL, &pfnOldIsr) != OSR_OK)
    {
        ASSERT(0);
    }
    #endif
#else 
    #ifdef VDEC_COMMON_ISR
    _VDEC_RegIsr(RVVLD_VECTOR, _RVVLDIsr);
    #else
    // reg ISR      
    if (x_reg_isr(RVVLD_VECTOR, _RVVLDIsr, &pfnOldIsr) != OSR_OK)
    {
        ASSERT(0);
    }
    #endif
#endif
    UNUSED(ucEsId);
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------



