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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_img.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_img.c
 *  This file contains implementation of exported APIs of sending image data to jpeg driver.
 */
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "vdec_drvif.h"
#include "img_lib_if.h"
#include "fbm_drvif.h"
#include "vdec_img.h"
#include "x_assert.h"
#include "x_debug.h"

/********************************************************************
  MACRO CONSTANT DEFINITIONS
********************************************************************/

/********************************************************************
  MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
  TYPE DEFINITION
********************************************************************/
#define MJPEG_DHT_DEFSIZE 1024
#define MJPEG_MAX_ES_NUM    1
#if defined(CC_MT5363) || defined(CC_MT5365)
#define MJPEG_MAX_WIDTH     1024
#define MJPEG_MAX_HEIGHT    768    
#else
#define MJPEG_MAX_WIDTH     640
#define MJPEG_MAX_HEIGHT    480    
#endif
#define MJPEG_TRACK_SKIP  5
/********************************************************************
  EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
static VDEC_PES_INFO_T _rPesInfo;
static HANDLE_T _hMJPGDecSema = 0;
static UINT32 _u4MJPEGNtyValue = 0;
static UCHAR _ucMJPEGFbgId;

// *********************************************************************
// Function    : void _VDEC_MJPEGCb
// Description : call back handler of decode event
// Parameter   : 
// Return      : None
// *********************************************************************
static VOID _VDEC_MJPEGCb(UINT32 u4ImgID, void *pv_tag, void *pv_data, UINT32 u4State)
{    
    _u4MJPEGNtyValue = u4State;
    if((u4State == JPG_HW_NFY_FINISHED) || (u4State == JPG_HW_NFY_ERROR) || 
       (u4State == JPG_HW_NFY_STOP_DONE))
	{		
		VERIFY (x_sema_unlock(_hMJPGDecSema) == OSR_OK);
	}    
	else if(u4State!=JPG_HW_NFY_ALREADY_STOPPED)
	{
	  LOG(0, "_VDEC_MJPEGCb MJPEG ES(0) Error=%d\n", _u4MJPEGNtyValue); 			   
	}
}

// *********************************************************************
// Function    : UINT32 _VDEC_VldMJPEGRPtr
// Description : get read pointer of ring buf
// Parameter   : 
// Return      : read ptr
// *********************************************************************
UINT32 _VDEC_VldMJPEGRPtr(UINT32* pu4Bits)
{
    UINT32 u4Cnt = 0;
    UINT32 u4DramRptr;
    UINT32 u4SramRptr, u4SramWptr;
    UINT32 u4SramDataSz;
    UINT32 u4ByteAddr;
    UINT32 u4VStart, u4VEnd;
    
    /*while(_WaitMpeg4VldFetchOk() != TRUE)
    {
    }*/

    while((VLD_READ32(RO_VLD_SRAMCTRL) & AA_FIT_TARGET_D)==0)
    {
        u4Cnt++;
        if(u4Cnt >= (UINT32)0x100000)
        {
            LOG(1, "Rptr-WaitSramStable Fail\n");
            break;
        }        
    }
    
    u4DramRptr = (UINT32)VLD_READ32(RO_VLD_VRPTR);
    u4SramRptr = (UINT32)(VLD_READ32(RO_VLD_VBAR) & 0xff);
    u4SramWptr = (UINT32)((VLD_READ32(RO_VLD_VBAR) >> 16) & 0xff);
    if(u4SramWptr > u4SramRptr)
    {
        u4SramDataSz = u4SramWptr - u4SramRptr;
    }
    else
    {
        u4SramDataSz = (UINT32)(64 - (u4SramRptr - u4SramWptr));
    }

    *pu4Bits = (UINT32)(VLD_READ32(RO_VLD_SUM) & 0x3F);
#if !(defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395))
    if (VLD_READ32(RW_VLD_ABS) & 0x1) // 03 replacement enable!
    {
        u4ByteAddr = (UINT32)(u4DramRptr - (u4SramDataSz + 4) * 4 + (*pu4Bits / 8));
    }
    else
#endif
    {
        u4ByteAddr = (UINT32)(u4DramRptr - (u4SramDataSz + 3)* 4 + (*pu4Bits / 8));
    }
    *pu4Bits &= 0x7;

    u4VStart = (UINT32)(VLD_READ32(RW_VLD_VSTART) << 6);
    u4VEnd = (UINT32)(VLD_READ32(RW_VLD_VEND) << 6);

    if (u4ByteAddr < u4VStart)
    {
        u4ByteAddr = u4ByteAddr + (u4VEnd - u4VStart);
    }
    if(u4ByteAddr > u4VEnd)
    {
        u4ByteAddr = u4VStart + (u4ByteAddr - u4VEnd);
    }

    return u4ByteAddr;
}


// *********************************************************************
// Function    : VOID _VDEC_MJPEGInit
// Description : Initialization procedure of vdec
// Parameter   : 
// Return      : 
// *********************************************************************
INT32 _VDEC_MJPEGInit(VOID)
{
    if (!_hMJPGDecSema)
    {
        if (x_sema_create(&_hMJPGDecSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
        {
            return E_VDEC_FAIL;
        }
    }
    IMG_Lib_Init();
    x_memset((VOID*)&_rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
    _ucMJPEGFbgId = FBM_FBG_ID_UNKNOWN;

    return E_VDEC_OK;
}


// *********************************************************************
// Function    : VOID _VDEC_MJPEGSWRst
// Description : software reset procedure of vdec
// Parameter   : 
// Return      : 
// *********************************************************************
VOID _VDEC_MJPEGSWRst(UCHAR ucMpvId, UCHAR ucEsId)
{
    if(IMG_GetDecState() == (UINT32)IMG_RELEASE)
    {
        IMG_Init(EXTERNAL_RING_IN_DOUBLE_OUT, NULL, 0, _VDEC_MJPEGCb);
    }    
}


// *********************************************************************
// Function    : VOID _VDEC_MJPEGSWRst
// Description : decoding procedure of vdec
// Parameter   : 
// Return      : 
// *********************************************************************
INT32 _VDEC_MJPEGDecode(UCHAR ucMpvId, UCHAR ucEsId)
{
    IMG_PARAM_T rInfo;
    IMG_REGION_T rDecOut;
    VDEC_ES_INFO_T *prVdecEsInfo;
    FBM_SEQ_HDR_T* prFbmSeqHdr = NULL;
    FBM_PIC_HDR_T* prFbmPicHdr = NULL;
    UINT32 u4YAddr, u4CAddr;
    UCHAR ucFbId, ucMJPEGTargetType;
    BOOL fgNotDisplay;

    prVdecEsInfo = _VDEC_GetEsInfo((UCHAR)0);
    if (prVdecEsInfo == NULL)
    {
        return (INT32)FALSE;
    }
    
    
    if ((prVdecEsInfo->rMMSeqInfo.u2_width > (UINT16)MJPEG_MAX_WIDTH) ||(prVdecEsInfo->rMMSeqInfo.u2_height> (UINT16)MJPEG_MAX_HEIGHT))
    {
           return (INT32)E_VDEC_NOT_SUPPORT;
    }

    _VDEC_MJPEGSWRst(ucMpvId, ucEsId);

    if (IMG_GetDecState() != IMG_INIT)
    {
        return (INT32)FALSE;
    }
    
    if (_ucMJPEGFbgId == (UINT32)FBM_FBG_ID_UNKNOWN)
    {
        if(prVdecEsInfo->rMMSeqInfo.u2_width==0 || prVdecEsInfo->rMMSeqInfo.u2_height==0)
        {
            ucMJPEGTargetType=0;
        }
        else
        {
            ucMJPEGTargetType = FBM_SelectGroupType((UINT32)prVdecEsInfo->rMMSeqInfo.u2_width,(UINT32)prVdecEsInfo->rMMSeqInfo.u2_height);
        }
                                                
        _ucMJPEGFbgId = FBM_CreateGroup(ucMJPEGTargetType, (UINT32)FBM_VDEC_MPEG4, (UINT32)0, (UINT32)0);  
        if (_ucMJPEGFbgId == (UINT32)FBM_FBG_ID_UNKNOWN)
        {
            LOG(0, "create FBG failed.\n");
            return (INT32)FALSE;
        }
        FBM_SetPlayMode(_ucMJPEGFbgId, (UCHAR)FBM_FBG_MM_MODE);
        FBM_SetSyncStc(_ucMJPEGFbgId, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId);
        FBM_FbgChgNotify(_ucMJPEGFbgId, ucEsId);
    }    

    if (FBM_ChkFrameBufferFlag(_ucMJPEGFbgId, (UINT32)FBM_FLAG_RESET))
    {
        FBM_ResetGroup(_ucMJPEGFbgId);
        FBM_ClrFrameBufferFlag(_ucMJPEGFbgId, (UINT32)FBM_FLAG_RESET);
        FBM_ClrFrameBufferFlag(_ucMJPEGFbgId, (UINT32)FBM_FLAG_NOT_READY);                  
    }
    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(_ucMJPEGFbgId);
    if (prFbmSeqHdr == NULL)
    {
        LOG(0, "get seq hdr failed.\n");        
        return (INT32)FALSE;
    }
    
    ucFbId = FBM_GetEmptyRefFrameBuffer(_ucMJPEGFbgId, (UINT32)150);
    if (ucFbId == (UCHAR)FBM_FB_ID_UNKNOWN)
    {
        LOG(0, "get empty frame buffer failed.\n");        
        return (INT32)FALSE;
    }
    FBM_GetFrameBufferAddr(_ucMJPEGFbgId, ucFbId, &u4YAddr, &u4CAddr);    

    prVdecEsInfo->ucFbgId = _ucMJPEGFbgId;
    prVdecEsInfo->ucFbId = ucFbId;
    
    prFbmPicHdr = FBM_GetFrameBufferPicHdr(_ucMJPEGFbgId, ucFbId);
    if (prFbmPicHdr == NULL)
    {
        LOG(0, "get pic hdr failed.\n");
        FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
        return (INT32)FALSE;
    }

    x_memset((VOID *)&rDecOut, 0x0, sizeof(IMG_REGION_T));
    rInfo.pu1InBuffer = (UINT8 *)_rPesInfo.u4FifoStart;
    rInfo.u8BufSize = (UINT32)(_rPesInfo.u4FifoEnd - _rPesInfo.u4FifoStart);
    rInfo.rDstRegion.u4StartX = (UINT32)0;
    rInfo.rDstRegion.u4StartY = (UINT32)0;
    
    rInfo.rDstRegion.u4Width = (UINT32)prVdecEsInfo->rMMSeqInfo.u2_width;
    rInfo.rDstRegion.u4Height = (UINT32)prVdecEsInfo->rMMSeqInfo.u2_height;
#ifdef CC_MT5365
	if(prVdecEsInfo->rMMSeqInfo.u2_width&0x3f)
	{
	   UINT32 u4FramePitch=0;
	   u4FramePitch=(UINT32)prVdecEsInfo->rMMSeqInfo.u2_width;
	   u4FramePitch=(u4FramePitch+0x40)&0xffffff40;
	   rInfo.rDstRegion.u4Pitch = u4FramePitch;
	}
#else
    rInfo.rDstRegion.u4Pitch = (UINT32)prVdecEsInfo->rMMSeqInfo.u2_width;
#endif
    rInfo.eDispColorMode = Y_CBCR420_SEP16;
    rInfo.eRotateDegree = CLOCKWISE_0;
    rInfo.bKeepRatio = TRUE;
    rInfo.prDecOut = &rDecOut;
    
    rInfo.rDstRegion.pu1Addr = (UINT8 *)u4YAddr;
    rInfo.rDstRegion.u4CbCrOffset = u4CAddr - u4YAddr;
    //TO DO
    rInfo.rMjpg.u4RingOffset = _rPesInfo.u4VldReadPtr - _rPesInfo.u4FifoStart;
    rInfo.rMjpg.ucFbgId = _ucMJPEGFbgId;

    IMG_SetFillBsBufEnd((UINT32)rInfo.u8BufSize, 0, TRUE);
    IMG_Decode(rInfo);        

    VERIFY (x_sema_lock(_hMJPGDecSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    
    prFbmSeqHdr->ucAspRatInf = (UCHAR)prVdecEsInfo->rMMSeqInfo.e_src_asp;
    prFbmSeqHdr->ucFrmRatCod = (UCHAR)prVdecEsInfo->rMMSeqInfo.e_frame_rate;
    prFbmSeqHdr->fgProgressiveSeq = TRUE;
    prFbmSeqHdr->fgBrokenLink = FALSE;
    prFbmSeqHdr->fgClosedGop = FALSE;
    prFbmSeqHdr->u2LineSize = rDecOut.u4Pitch;
    prFbmSeqHdr->u2HSize = rDecOut.u4Width;
    prFbmSeqHdr->u2VSize = rDecOut.u4Height;
    prFbmSeqHdr->u2DHS = rDecOut.u4Width;
    prFbmSeqHdr->u2DVS = rDecOut.u4Height;
    prFbmSeqHdr->u2OrgHSize = rDecOut.u4Width;
    prFbmSeqHdr->u2OrgVSize = rDecOut.u4Height;
    prFbmPicHdr->ucPicCdTp = (UCHAR)I_TYPE;
    prFbmPicHdr->ucPicStruct = (UCHAR)3;
    prFbmPicHdr->fgProgressiveFrm = TRUE; 
    prFbmPicHdr->fgRepFirstFld = FALSE;
    prFbmPicHdr->fgTopFldFirst = FALSE;
    prFbmPicHdr->u4PTS = prVdecEsInfo->u4PTS;       

    if (_u4MJPEGNtyValue != (UINT32)JPG_HW_NFY_FINISHED)
    {
    	//return (INT32)FALSE; 
    	LOG(1, "one frame broken\n");
    }
    CLR_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    fgNotDisplay = VDEC_IsNotDisplay((UCHAR)0, prFbmPicHdr->u4PTS, 0, 0);
    if (fgNotDisplay || (_u4MJPEGNtyValue != (UINT32)JPG_HW_NFY_FINISHED))
    {
        FBM_SetFrameBufferStatus(_ucMJPEGFbgId, ucFbId, (UCHAR)FBM_FB_STATUS_EMPTY);
    }
    else
    {
        FBM_SetFrameBufferStatus(_ucMJPEGFbgId, ucFbId, (UCHAR)FBM_FB_STATUS_DISPLAYQ);
        prVdecEsInfo->u4DisplayQPicCnt++;
        VDEC_CheckStopStatus(0, prFbmPicHdr->u4PTS, 0);
    }    

    IMG_Stop();
    IMG_Release();

    return (INT32)TRUE;
}


// *********************************************************************
// Function    : VOID _VDEC_MJPEGStop
// Description : stopping procedure of vdec
// Parameter   : 
// Return      : 
// *********************************************************************
VOID _VDEC_MJPEGStop(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    prVdecEsInfo = _VDEC_GetEsInfo(0);
/*
    IMG_Stop();

    IMG_SetFillBsBufEnd(0, 0, TRUE);
	
	IMG_Release();
*/
    if (_ucMJPEGFbgId != (UCHAR)FBM_FBG_ID_UNKNOWN)
    {
        FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, ucEsId);
        FBM_WaitUnlockFrameBuffer(_ucMJPEGFbgId, (UINT32)20);    // block until display unlock   
	    FBM_ReleaseGroup(_ucMJPEGFbgId);
	    _ucMJPEGFbgId = (UCHAR)FBM_FBG_ID_UNKNOWN;
	    prVdecEsInfo->ucFbgId = (UCHAR)FBM_FBG_ID_UNKNOWN;
    }
}


// *********************************************************************
// Function    : VOID _VDEC_MJPEGStop
// Description : Get the Pes Struct for using vdec
// Parameter   : 
// Return      : OK: success
// *********************************************************************
VOID _VDEC_MJPEGGetPesStruct(UCHAR ucEsId, void **ppvPesInfo)
{
  *ppvPesInfo = (VOID*)&_rPesInfo;
}


// *********************************************************************
// Function    : VOID _VDEC_MJPEGUpdDmxRptr
// Description : update dmx read pointer for using vdec
// Parameter   : 
// Return      : 
// *********************************************************************
void _VDEC_MJPEGUpdDmxRptr(UCHAR ucEsId)
{
    UINT32 u4CurRptr, u4CurRptrBits;
    
    ASSERT(ucEsId < (UCHAR)MJPEG_MAX_ES_NUM);

    u4CurRptr = _VDEC_VldMJPEGRPtr(&u4CurRptrBits);
    
    //update dmx pointer
    if (!DMX_UpdateVideoReadPointer((UINT8)_rPesInfo.ucEsId, u4CurRptr, u4CurRptr))                
    {                    
        LOG(0, "MJPEG ES(%d) DMX_UpdateVideoReadPointer Fail\n", _rPesInfo.ucEsId);                
    }
}


// *********************************************************************
// Function    : VOID _VDEC_MJPEGSetDmxWptr
// Description : update dmx write pointer by vdec
// Parameter   : 
// Return      : 
// *********************************************************************
void _VDEC_MJPEGSetDmxWptr(UCHAR ucEsId, UINT32 u4Wptr)
{
    ASSERT(ucEsId < (UCHAR)MJPEG_MAX_ES_NUM);

   // VLD_WRITE32(WO_VLD_WPTR, (u4Wptr & (~0xf)));
}

// *********************************************************************
// Function    : VOID _VDEC_MJPEGEsFlwCtrl
// Description : skip one frame
// Parameter   : 
// Return      : 
// *********************************************************************
BOOL _VDEC_MJPEGEsFlwCtrl(UCHAR ucEsId, UCHAR* pucVldId, INT8 i1EsBufStatus, UCHAR ucSkipMode)
{
    static UINT8 u1SkipCnt = 0;
    BOOL fgSkip = FALSE;
    
    if (_ucMJPEGFbgId == (UCHAR)FBM_FBG_ID_UNKNOWN)
    {
        return FALSE;
    }
    else if (FBM_ChkFrameBufferFlag(_ucMJPEGFbgId, (UINT32)FBM_FLAG_SKIP_PIC) && 
            (ucSkipMode != (UCHAR)IMode))
    {
        fgSkip = TRUE;
        if (u1SkipCnt > (UINT8)MJPEG_TRACK_SKIP)
        {
            fgSkip = FALSE;
            u1SkipCnt = 0;
        }
        if (fgSkip)
        {
            //update dmx
            if (!DMX_UpdateVideoReadPointer((UINT8)_rPesInfo.ucEsId, 
                _rPesInfo.u4VldReadPtr, _rPesInfo.u4VldReadPtr))                
            {                    
                LOG(0, "EsFlwCtrl MJPEG ES(%d) DMX_UpdateVideoReadPointer Fail\n", _rPesInfo.ucEsId);                
            }

            FBM_ClrFrameBufferFlag(_ucMJPEGFbgId, (UINT32)FBM_FLAG_SKIP_PIC);
            u1SkipCnt++;
                
            return TRUE;
        }
    }
    u1SkipCnt = 0;
    
    return FALSE;
}


// *********************************************************************
// Function    : BOOL _VDEC_IsSupportMJPEG
// Description : 
// Parameter   : 
// Return      : 
// *********************************************************************
BOOL _VDEC_IsSupportMJPEG(VOID)
{
    return TRUE;
}

