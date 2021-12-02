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
 * $RCSfile: vdec_wmvapi.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_wmvapi.c
 *  This file contains implementation of exported APIs of VDEC.
 */

#include "x_lint.h"
#include "vdec_drvif.h"
#include "vdec_wmvpsr.h"
#include "vdec_wmvdec.h"
#include "vdec_wmvutil.h"
#include "vdec_wmvhw.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_chip_id.h"
#include "x_common.h"
#include "x_typedef.h"
LINT_EXT_HEADER_END


#ifndef AVC_IRQ_DISABLE
#define WMVVLD_VECTOR 24
extern HANDLE_T hVdecWMVFinSema;
#endif

//global variable
#define MAX_ES_NUM 1
#define ES_ID_0 0

static UINT8 _u1vopFirstFieldType[8] =
{ IVOP, IVOP, PVOP, PVOP, BVOP, BVOP, BIVOP, BIVOP };
static UINT8 _u1vopSecondFieldType[8] =
{ IVOP, PVOP, IVOP, PVOP, BVOP, BIVOP, BVOP, BIVOP };
#define WMV_SC_FRAME        0x10D
#define WMV_SC_ENTRY        0x10E
#define WMV_SC_SEQ          0x10F
#define WMV_SC_FIELD        0x10C
extern void reset_pic_hdr_bits(void);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
VDEC_WMV_INFO_T rWMVDecInfo;


/** .
*@param .
 */
VDEC_HDLR _VDEC_WMVGetHdlr(void)
{
    return (VDEC_HDLR)&rWMVDecInfo;
}


/** .
*@param .
 */
INT32 _VDEC_WMVInit()
{
    VDEC_WMV_INFO_T* prVdec;

    prVdec = (VDEC_WMV_INFO_T*)&rWMVDecInfo;

    x_memset((void*)prVdec, 0, sizeof(VDEC_WMV_INFO_T));
    rWMVDecInfo.rOther.fgFirstFrame = TRUE;
    //_WMVInit(hVdec, pi4RetValue); we need wmv type, move it to header parsing
#if defined(CC_MT5365) || defined(CC_MT5395)
    rWMVDecInfo.rOther.fgNewMode = TRUE;
#else
    rWMVDecInfo.rOther.fgNewMode = FALSE;
#endif
    return E_VDEC_OK;
}

/** .
*@param .
 */
INT32 _VDEC_WMVHdrPrs(UCHAR ucVdecId, UCHAR ucEsId)
{
    INT32 i4RetValue;
    _WMVHdrPrs(&i4RetValue);
    if (i4RetValue == E_VDEC_SKIPFRAME) {
        INT32 i4RetValue2;
#ifdef CC_WMV_EMULATION
        LOG(0,"Frame %d : Skip Frame .........................................\n", rWMVDecInfo.rOther.u4CurrPicNo);
#endif
        rWMVDecInfo.rOther.u4CurrPicNo++;
        _WMVSWRst(&i4RetValue2);
    }
    return i4RetValue;
}

/** .
*@param .
 */
INT32 _VDEC_WMVDecStart(UCHAR ucVdecId, UCHAR ucEsId)
{
    INT32 i4RetValue;
    _WMVDecStart(&i4RetValue);
    return i4RetValue;
}

/** .
*@param .
 */
INT32 _VDEC_WMVDecFin(UCHAR ucVdecId, UCHAR ucEsId)
{
    INT32 i4RetValue;
    _WMVDecFin(&i4RetValue);
    if (i4RetValue == E_VDEC_DISP_NOT_READY) {
        INT32 i4RetValue2;
        _WMVSWRst(&i4RetValue2);
    }
    return i4RetValue;
}


VOID _VDEC_WMVSWRst(UCHAR ucVdecId, UCHAR ucEsId)
{
    INT32 i4RetValue;
    _WMVSWRst(&i4RetValue);
}


VOID _VDEC_WMVStop(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec;
    //VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
#ifdef USE_FBM
   // if (rWMVDecInfo.rOther.u1FbgId != FBM_FBG_ID_UNKNOWN)//Cr 81570
    {
        FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, 0);
        FBM_WaitUnlockFrameBuffer(rWMVDecInfo.rOther.u1FbgId,20);    // block until display unlock
        FBM_ReleaseGroup(rWMVDecInfo.rOther.u1FbgId);
        rWMVDecInfo.rOther.u1FbgId = FBM_FBG_ID_UNKNOWN;
    }
#endif

//clear decoder information
    prVdec = (VDEC_WMV_INFO_T*)&rWMVDecInfo;

    x_memset((void*)prVdec, 0, sizeof(VDEC_WMV_INFO_T));
    rWMVDecInfo.rOther.fgFirstFrame = TRUE;

#if 0
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
#endif

}


VOID _VDEC_WMVGetPesStruct(UCHAR ucEsId, void **ppvPesInfo)
{
    VDEC_WMV_INFO_T* prVdec;
    prVdec = (VDEC_WMV_INFO_T*)&rWMVDecInfo;
    *ppvPesInfo = (void*)(&prVdec->rPesInfo);
    //return E_VDEC_OK;
}


VOID _VDEC_WMVGetDispPic(UCHAR ucEsId, void* pvInfo)
{
    VDEC_WMV_INFO_T* prVdec;
    VDEC_DISP_PIC_INFO_T* prDispInfo;

    prVdec = (VDEC_WMV_INFO_T*)&rWMVDecInfo;
    prDispInfo = (VDEC_DISP_PIC_INFO_T*)pvInfo;

    prDispInfo->u4YSize = prVdec->rSeqHdrR.u4PicWidthDec * prVdec->rSeqHdrR.u4PicHeightDec;
    prDispInfo->u4CSize = prDispInfo->u4YSize >> 1;

    prDispInfo->u4W = rWMVDecInfo.rOther.rPicInfo.dwW;
    prDispInfo->u4H = rWMVDecInfo.rOther.rPicInfo.dwH;

    prDispInfo->u4W_Cmp = rWMVDecInfo.rSeqHdrR.u4PicWidthCmp;
    prDispInfo->u4H_Cmp = rWMVDecInfo.rSeqHdrR.u4PicHeightCmp;

    prDispInfo->u4CurrentPicNo = rWMVDecInfo.rOther.u4CurrPicNo;

    prDispInfo->pvYAddr = (void*)rWMVDecInfo.rOther.rVSyncPrm.dwPtrY;
    prDispInfo->pvCAddr = (void*)rWMVDecInfo.rOther.rVSyncPrm.dwPtrC;

    prDispInfo->i4CurrentTemporalField = rWMVDecInfo.rPicLayer.i4CurrentTemporalField;
    prDispInfo->u1FrameCodingMode = rWMVDecInfo.rPicLayer.u1FrameCodingMode;

}

VOID _VDEC_WMVSetParam(UCHAR ucEsId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3)
{
    if (u4Type == 0) //set resolution
    {
        rWMVDecInfo.rSeqHdrR.u4PicWidthSrc = (UINT32)u4Param1;
        rWMVDecInfo.rSeqHdrR.u4PicHeightSrc = (UINT32)u4Param2;
    }
    else if (u4Type ==1) //set sequence header
    {
        rWMVDecInfo.rOther.i4SeqHdrDataLen = u4Param1;
        rWMVDecInfo.rOther.i4SeqHdrData1 = u4Param2;
    }
    else
    {
        //do nothing
    }
}


BOOL _VDEC_IsSupportVc1()
{
#ifdef CC_WMV_EMULATION
    return TRUE;
#else
    return IS_SupportVC1();
#endif
}

void _VDEC_WMVUpdDmxRptr(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec;
    UINT32 u4CurRptr;
    UINT32 u4CurRptrBits;

    ASSERT(ucEsId < MAX_ES_NUM);

    prVdec = &rWMVDecInfo;
    u4CurRptr = dwVldRPtr(&u4CurRptrBits);

    if (rWMVDecInfo.rPesInfo.u1SliceCount)
    {
        return;
    }
    /*u4CurRptr -= (WPTR_OFFSET + RPTR_ALIGN);

    u4CurRptr += prVdec->rPesInfo.u4FifoStart;

    if(u4CurRptr < prVdec->rPesInfo.u4FifoStart)
    {
        u4CurRptr += (prVdec->rPesInfo.u4FifoEnd - prVdec->rPesInfo.u4FifoStart);
    }
    */
    u4CurRptr += prVdec->rPesInfo.u4FifoStart;
    LOG(6,"current read pointer : %x %x fifo start : %x\n",(u4CurRptr-rWMVDecInfo.rPesInfo.u4FifoStart),u4CurRptr,rWMVDecInfo.rPesInfo.u4FifoStart);

    //update dmx pointer
    if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, prVdec->rPesInfo.u4VldReadPtr, prVdec->rPesInfo.u4VldReadPtr))
        //if (!DMX_UpdateVideoReadPointer(0, (u4CurRptr) , (u4CurRptr)))
    {
        LOG(0, "WMV ES(%d) DMX_UpdateVideoReadPointer Fail\n", rWMVDecInfo.rPesInfo.ucEsId);
    }
}

void _VDEC_WMVSetDmxWptr(UCHAR ucEsId, UINT32 u4Wptr)
{
    //VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    ASSERT(ucEsId < MAX_ES_NUM);
    //LOG(0, "UpdWptr 0x%x\n", u4Wptr);
    //prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(0);

    /*if (!prVdecEsInfoKeep->fgDmxPartialPes)
    {
        rWMVDecInfo.rOther.u4WPtr = (u4Wptr + 0xf) & (~0xf);
        rWMVDecInfo.rOther.u4WPtr += WPTR_OFFSET;
        //LOG(5,"W: 0x%x\n", prEsInfo->u4VldWptr);
    }
    else
    {
        rWMVDecInfo.rOther.u4WPtr = (u4Wptr & (~0xf));
    }*/

    rWMVDecInfo.rOther.u4WPtr = (u4Wptr & (~0xf));
    _VDEC_UpdateVldWMVWPtr(rWMVDecInfo.rOther.u4WPtr);

}

//skip ES if return TRUE
BOOL _VDEC_WMVEsFlwCtrl(UCHAR ucEsId, UCHAR* pucVldId, INT8 i1EsBufStatus, UCHAR ucSkipMode)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(0);
    UINT8 u1PicType = 0;
    //static UINT32 u4IframeCount =0;
    //static UINT32 u4BIframeCount =0;

    UNUSED(u1PicType);
    ASSERT(ucEsId < MAX_ES_NUM);
    ASSERT(pucVldId !=NULL);
    ASSERT(prVdecEsInfo != NULL);

    //for trick mode
    /*if(rWMVDecInfo.rPesInfo.fgGop)
    {
        vSetFRefBuf(0);
        vSetBRefBuf(1);
    }
    */

    rWMVDecInfo.rOther.i4SetPos = (rWMVDecInfo.rPesInfo.u4VldReadPtr - rWMVDecInfo.rPesInfo.u4FifoStart);
    rWMVDecInfo.rDram.pu1VFifo = (void*)rWMVDecInfo.rPesInfo.u4FifoStart;
    rWMVDecInfo.rDram.pu1FifoEnd = (void*)rWMVDecInfo.rPesInfo.u4FifoEnd;

#ifndef CC_WMV_EMULATION
    if (prVdecEsInfo != NULL && rWMVDecInfo.rPesInfo.u4VideoType != DMX_VIDEO_VC1)
        //if(prVdecEsInfo != NULL)
    {
        rWMVDecInfo.rSeqHdrR.u4PicWidthSrc =  (UINT32)prVdecEsInfo->rMMSeqInfo.u2_width;
        rWMVDecInfo.rSeqHdrR.u4PicHeightSrc = (UINT32)prVdecEsInfo->rMMSeqInfo.u2_height;
        rWMVDecInfo.rOther.i4SeqHdrDataLen = (UINT32)prVdecEsInfo->rMMSeqInfo.u2SequenceHeaderSize;
        rWMVDecInfo.rOther.i4SeqHdrData1 = (UINT32)prVdecEsInfo->rMMSeqInfo.u4SequenceHeader;
    }
#else
    // vparser use _VdecVParserWMV to set decoder parameters
    if (rWMVDecInfo.rPesInfo.ucPicType == 0xff &&rWMVDecInfo.rPesInfo.fgEos)
    {
        return FALSE;
    }
#endif

    if (rWMVDecInfo.rPesInfo.u4VideoType == DMX_VIDEO_VC1)
    {
        rWMVDecInfo.rSeqLayer.fgVC1 = TRUE;
        rWMVDecInfo.rOther.i4CodecVersion = WMVA;
    }
    else if (rWMVDecInfo.rPesInfo.u4VideoType == DMX_VIDEO_WMV7)
    {
        rWMVDecInfo.rSeqLayer.fgVC1 = FALSE;
        rWMVDecInfo.rOther.i4CodecVersion = WMV1;
    }
    else if (rWMVDecInfo.rPesInfo.u4VideoType == DMX_VIDEO_WMV8)
    {
        rWMVDecInfo.rSeqLayer.fgVC1 = FALSE;
        rWMVDecInfo.rOther.i4CodecVersion = WMV2;
    }
    else if (rWMVDecInfo.rPesInfo.u4VideoType == DMX_VIDEO_WMV9)
    {
        rWMVDecInfo.rSeqLayer.fgVC1 = FALSE;
        rWMVDecInfo.rOther.i4CodecVersion = WMV3;
    }
    else
    {
        ASSERT(0);
        rWMVDecInfo.rSeqLayer.fgVC1 = FALSE;
        rWMVDecInfo.rOther.i4CodecVersion = UNKNOW_TYPE;
    }

    if (rWMVDecInfo.rOther.fgFirstFrame) //init state
    {
        LOG(6,"hello, it's first frame\n");
        _WMVInit();
        vInitVParser();
        vInitVParserWMV();
        vPowerOnVDec();

#ifndef AVC_IRQ_DISABLE
        x_os_isr_fct pfnOldIsr;
        // reg ISR
        if (x_reg_isr(WMVVLD_VECTOR, _WMVVLDIsr, &pfnOldIsr) != OSR_OK)
        {
            ASSERT(0);
        }
        if (hVdecWMVFinSema == NULL)
        {
            VERIFY(x_sema_create(&hVdecWMVFinSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        }
        //vIrqOn(INT_VLD);
#endif

        rWMVDecInfo.rOther.fgDumpSimPat = FALSE;
        rWMVDecInfo.rOther.u4BPRawFlag = 0;
        rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);
        if (rWMVDecInfo.rOther.i4CodecVersion != WMVA)
        {
            fgVldInitBarrelShifter((UINT32)rWMVDecInfo.rDram.pu1VFifo + rWMVDecInfo.rOther.i4SetPos);
            _getESInfo();
            if (rWMVDecInfo.rOther.i4CodecVersion == WMV3)
            {
                SetupMultiResParams();
                rWMVDecInfo.rOther.i4VprErr = decodeVOLHead_WMV3();
            }
            else if ((rWMVDecInfo.rOther.i4CodecVersion == WMV1) || (rWMVDecInfo.rOther.i4CodecVersion == WMV2))
            {
                rWMVDecInfo.rOther.i4VprErr = decodeVOLHead_WMV12();
                //rWMVDecInfo.rPicLayer.u1PicType = dwGetHi(iNumBitsFrameType);
                //u1PicType = rWMVDecInfo.rPicLayer.u1PicType;
            }
            vInitVdoMem();
        }
        rWMVDecInfo.rOther.fgFirstFrame = FALSE;
    }

    if (rWMVDecInfo.rOther.i4CodecVersion == WMV3)
    {
        fgVldInitBarrelShifter((UINT32)rWMVDecInfo.rDram.pu1VFifo + rWMVDecInfo.rOther.i4SetPos);
        if (rWMVDecInfo.rSeqLayer.fgSeqFrameInterpolation)
        {
            dwGetHi(1);
        }
        dwGetHi(2);

        if (rWMVDecInfo.rSMProfile.fgPreProcRange)
        {
            rWMVDecInfo.rEntryPoint.i4RangeState = dwGetHi(1);
        }

        rWMVDecInfo.rPicLayer.u1PrevPicType = rWMVDecInfo.rPicLayer.u1PicType;
        // Picture coding type
        if (1 == dwGetHi(1))
            rWMVDecInfo.rPicLayer.u1PicType = PVOP;
        else
        {
            if (rWMVDecInfo.rSMProfile.i4NumBFrames == 0)
            {
                rWMVDecInfo.rPicLayer.u1PicType = IVOP;
            }
            else
            {
                if (dwGetHi(1) == 1)
                    rWMVDecInfo.rPicLayer.u1PicType = IVOP;
                else
                    rWMVDecInfo.rPicLayer.u1PicType = BVOP;
            }
        }
        u1PicType = rWMVDecInfo.rPicLayer.u1PicType;
    }
    else if ((rWMVDecInfo.rOther.i4CodecVersion == WMV1) || (rWMVDecInfo.rOther.i4CodecVersion == WMV2))
    {
        int iNumBitsFrameType;
        fgVldInitBarrelShifter((UINT32)rWMVDecInfo.rDram.pu1VFifo + rWMVDecInfo.rOther.i4SetPos);
        rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);
        iNumBitsFrameType = (rWMVDecInfo.rOther.i4CodecVersion == WMV2) ? 1 : 2;
        rWMVDecInfo.rPicLayer.u1PrevPicType = rWMVDecInfo.rPicLayer.u1PicType;
        rWMVDecInfo.rPicLayer.u1PicType = dwGetHi(iNumBitsFrameType);
        u1PicType = rWMVDecInfo.rPicLayer.u1PicType;
    }
    else if (rWMVDecInfo.rOther.i4CodecVersion == WMVA)
    {
        UINT32 u4StartCode;
        UINT32 u4Addr;
        UINT8* pucBuf;
        UINT8 u1FrameCodingMode;
        static UINT8 u1SecondFieldType;
        static UINT8 u1CountingIPFrame =0;

        u4Addr = rWMVDecInfo.rPesInfo.u4VldReadPtr + 3;
        if (u4Addr >= rWMVDecInfo.rPesInfo.u4FifoEnd)
        {
            u4Addr -= (rWMVDecInfo.rPesInfo.u4FifoEnd - rWMVDecInfo.rPesInfo.u4FifoStart);
        }
        pucBuf = (UCHAR*)VIRTUAL(u4Addr);
        u4StartCode = pucBuf[0];
        u4StartCode |= 0x100;
        switch (u4StartCode)
        {
        case WMV_SC_SEQ:
            u1CountingIPFrame = 0;
            break;
        case WMV_SC_FRAME:
        case WMV_SC_FIELD:
#ifdef USE_FBM
            if (rWMVDecInfo.rOther.u1FbgId == FBM_FBG_ID_UNKNOWN)
            {
                return TRUE;
            }
#endif
            fgVldInitBarrelShifter(rWMVDecInfo.rPesInfo.u4VldReadPtr+4);
            rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);
            reset_pic_hdr_bits();
            if (rWMVDecInfo.rPicLayer.i4CurrentTemporalField == 1)
            {
                u1FrameCodingMode = INTERLACEFIELD;
            }
            else
            {
                if (rWMVDecInfo.rSeqLayer.fgInterlacedSource)
                {
                    if (0 == dwGetHi(1))
                    {
                        u1FrameCodingMode = PROGRESSIVE;
                    }
                    else
                    {
                        if (0 == dwGetHi(1))
                        {
                            u1FrameCodingMode = INTERLACEFRAME;
                        }
                        else
                        {
                            u1FrameCodingMode = INTERLACEFIELD;
                        }
                    }
                }
                else
                {
                    u1FrameCodingMode = PROGRESSIVE;
                }
            }
            if (u1FrameCodingMode == PROGRESSIVE || u1FrameCodingMode == INTERLACEFRAME)
            {
                if (0 == dwGetHi(1))
                    u1PicType = PVOP;
                else if (0 == dwGetHi(1))
                    u1PicType = BVOP;
                else if (0 == dwGetHi(1))
                    u1PicType = IVOP;
                else if (0 == dwGetHi(1))
                    u1PicType = BIVOP;
                else
                    u1PicType = SKIPFRAME;
            }
            else if (u1FrameCodingMode == INTERLACEFIELD)
            {
                if (rWMVDecInfo.rPicLayer.i4CurrentTemporalField == 0)
                {
                    UINT32 u4Code = dwGetHi(3);
                    u1PicType = _u1vopFirstFieldType[u4Code];
                    u1SecondFieldType = _u1vopSecondFieldType[u4Code];
                }
                else
                {
                    u1PicType = u1SecondFieldType;
                }
            }
            break;
        default:
            return FALSE;
            //break;
        }
#if 1
        if (u4StartCode == WMV_SC_FRAME || u4StartCode==WMV_SC_FIELD)
        {
            if ((u1PicType ==IVOP || u1PicType==PVOP) && !rWMVDecInfo.rEntryPoint.fgClosedEntryPoint)
            {
                u1CountingIPFrame++;
                if (u1CountingIPFrame == 2)
                {
                    rWMVDecInfo.rPicLayer.i4TemporalRef = 0;
                    rWMVDecInfo.rEntryPoint.fgClosedEntryPoint = TRUE;
                }
            }
            if (!rWMVDecInfo.rEntryPoint.fgClosedEntryPoint && u1PicType ==BVOP && rWMVDecInfo.rOther.fgChangeResolution)
            {
                rWMVDecInfo.rOther.fgSkipFrame = TRUE;
                //u1PicType = SKIPFRAME;
                //return TRUE;
            }
        }
#endif
    }



    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I))
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
        if (u1PicType!=IVOP)
        {
            return TRUE;
        }
        else
        {
            //LOG(0,"I frame\n");
            return FALSE;
        }
    }
    else if (ucSkipMode == IPMode)
    {
        //LOG(0,"IP mode\n");
        if (u1PicType == BVOP || u1PicType == BIVOP)
            return TRUE;
        else
            return FALSE;
    }
    return FALSE;
}

VOID _VDEC_WMVStartPTS(UCHAR ucEsId, BOOL fgEnCalPTS, UINT32 u4Pts, UINT32 u4PtsDiff)
{
    VDEC_WMV_INFO_T* prVdec;

    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_WMV_INFO_T*)&rWMVDecInfo;

    prVdec->rOther.fgEnCalPTS= fgEnCalPTS;
    prVdec->rOther.fgFirstDispFrame = fgEnCalPTS;
    if (fgEnCalPTS)
    {
        //prVdec->rOther.u4LastPTS = u4Pts;
        //prVdec->rOther.u4DeltaPTS= u4PtsDiff;
    }
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------



