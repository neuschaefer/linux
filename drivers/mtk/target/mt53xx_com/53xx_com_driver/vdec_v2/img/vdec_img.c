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
#include "srm_drvif.h"
#include "img_lib_if.h"
#include "imgrz_if.h"
#include "fbm_drvif.h"
#include "vdec_img.h"
#include "x_assert.h"
#include "x_debug.h"
#include "x_img_dec.h"
#include "gfx_if.h"

/********************************************************************
  MACRO CONSTANT DEFINITIONS
********************************************************************/
#ifdef VDEC_VENC_PATH
#define MJPEG_SUPPORT_VENC
#endif

/********************************************************************
  MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
  TYPE DEFINITION
********************************************************************/
#define MJPEG_DHT_DEFSIZE 1024
#define MJPEG_MAX_ES_NUM    VDEC_MAX_ES
#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
#define MJPEG_MAX_WIDTH     1920
#define MJPEG_MAX_HEIGHT    1080
#elif defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
#define MJPEG_MAX_WIDTH     1280
#define MJPEG_MAX_HEIGHT    720
#else
#define MJPEG_MAX_WIDTH     640
#define MJPEG_MAX_HEIGHT    480  
#endif
#define MJPEG_TRACK_SKIP  5

/********************************************************************
  EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
#ifdef VDEC_TIME_PROFILE
typedef struct
{    
    UINT8 u1DramMoniter;     //0: none, 1: Total, 2: Mpeg

    UINT32 u4CurDecMs;
    UINT32 u4CurDecCycle;
    UINT32 u4CurDramCycle;
}MJPEGOther;
#endif

typedef struct
{
  VDEC_PES_INFO_T rPesInfo;
#ifdef VDEC_TIME_PROFILE
    MJPEGOther rOther;
#endif
  UINT32 u4MJPEGNtyValue;
  UINT32 u4ReadPtr;
  UCHAR ucMpvId;
  UCHAR ucFbgId;
  UCHAR ucFbId;
  BOOL  fgUsed;
}VDEC_IMG_INSTANCE_T;

VDEC_IMG_INSTANCE_T _rVdecImgInfo[MJPEG_MAX_ES_NUM];
static HANDLE_T _hMJPGDecSema = 0;
static BOOL _fgVdecImgLibInited=FALSE;
IMG_REGION_T _rDecOut;

#ifdef VDEC_TIME_PROFILE
static HAL_TIME_T _rTimeHwS, _rTimeHwDt, _rTimeHwE;
static UINT32 u4DelayClock;
static UINT32 u4DramCycle;
static UINT32 u4DecCycle = 0;
#endif

extern VOID _VPUSH_ImgFrmDone(UCHAR ucEsId, VOID *pNfyInfo);

// *********************************************************************
// Function    : void _VDEC_MJPEGCb
// Description : call back handler of decode event
// Parameter   : 
// Return      : None
// *********************************************************************
static VOID _VDEC_MJPEGCb(UINT32 u4ImgID, void *pv_tag, void *pv_data, UINT32 u4State)
{    
    VDEC_IMG_INSTANCE_T *prVdecImgInfo=NULL;
	prVdecImgInfo=&_rVdecImgInfo[u4ImgID];
	prVdecImgInfo->u4MJPEGNtyValue=u4State;
    if((u4State == JPG_HW_NFY_FINISHED) || (u4State == JPG_HW_NFY_ERROR) || 
       (u4State == JPG_HW_NFY_STOP_DONE))
	{		
		VERIFY (x_sema_unlock(_hMJPGDecSema) == OSR_OK);
	}    
	else if(u4State!=JPG_HW_NFY_ALREADY_STOPPED)
	{
	  LOG(0, "_VDEC_MJPEGCb MJPEG ES(0) Error=%d\n", u4State); 			   
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
    UINT32 u4Bits;
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

	u4Bits=(UINT32)(VLD_READ32(RO_VLD_SUM) & 0x3F);
#if !(defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)) 
    if (VLD_READ32(RW_VLD_ABS) & 0x1) // 03 replacement enable!
    {
        u4ByteAddr = (UINT32)(u4DramRptr - (u4SramDataSz + 4) * 4 + (u4Bits / 8));
    }
    else
#endif
    {
        u4ByteAddr = (UINT32)(u4DramRptr - (u4SramDataSz + 3)* 4 + (u4Bits / 8));
    }

    u4Bits &= 0x7;

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

	if(pu4Bits)
	{
      *pu4Bits=u4Bits;
	}
    return u4ByteAddr;
}


// *********************************************************************
// Function    : VOID _VDEC_MJPEGInit
// Description : Initialization procedure of vdec
// Parameter   : 
// Return      : 
// *********************************************************************
INT32 _VDEC_MJPEGInit(UCHAR ucEsId)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if(!_fgVdecImgLibInited)
    {
        if (!_hMJPGDecSema)
        {
            if (x_sema_create(&_hMJPGDecSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
            {
                return E_VDEC_FAIL;
            }
        }

        IMGRZ_Init();
    }

    IMG_Lib_Init();

    if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_PNG)
    {
        IMG_Config(IMAGE_PNG);
    }
    else
    {
        IMG_Config(IMAGE_JPEG);
    }

    _fgVdecImgLibInited=TRUE;

    x_memset((VOID *)(&_rVdecImgInfo[ucEsId]),0,sizeof(VDEC_IMG_INSTANCE_T));
    _rVdecImgInfo[ucEsId].ucFbgId=FBM_FBG_ID_UNKNOWN;

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
        IMG_Init(EXTERNAL_RING_IN_DOUBLE_OUT, NULL, ucEsId, _VDEC_MJPEGCb);
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
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
	VDEC_PES_INFO_T *prPesInfo=NULL;
    FBM_SEQ_HDR_T* prFbmSeqHdr = NULL;
    FBM_PIC_HDR_T* prFbmPicHdr = NULL;
    UINT32 u4YAddr, u4CAddr, u4DstAddr;//,u4FrameWidth=0;
    UCHAR ucFbId = 0xff;
    UCHAR ucFbgId,ucMJPEGTargetType;
    BOOL fgNotDisplay;
    VDEC_IMG_INSTANCE_T *prVdecImgInfo=NULL;
    VDEC_IMG_FRMDONE_DATA rCBData = {0};
    //FBM_POOL_T* prFbmPool;

	prVdecImgInfo=&_rVdecImgInfo[ucEsId];
	prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
	prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    if ((prVdecEsInfo == NULL) || (prVdecEsInfoKeep == NULL))
    {
        return (INT32)FALSE;
    }
    if ((prVdecEsInfo->rMMSeqInfo.u2_width > (UINT16)MJPEG_MAX_WIDTH) ||
        (prVdecEsInfo->rMMSeqInfo.u2_height> (UINT16)MJPEG_MAX_HEIGHT))
    {
        return (INT32)E_VDEC_RES_NOT_SUPPORT;
    }
    prPesInfo=&prVdecImgInfo->rPesInfo;
    
    if(prVdecImgInfo->ucFbgId==FBM_FBG_ID_UNKNOWN)
    {
        if(0/*prVdecEsInfo->rMMSeqInfo.u2_width==0 || prVdecEsInfo->rMMSeqInfo.u2_height==0*/)
        {
            ucMJPEGTargetType=0;
        }
        else
        {
            ucMJPEGTargetType = FBM_SelectGroupType(MJPEG_MAX_WIDTH,MJPEG_MAX_HEIGHT);
        }

        {
            FBM_CREATE_FBG_PAR_T rPar;
            x_memset(&rPar, 0, sizeof(FBM_CREATE_FBG_PAR_T));
            rPar.u1VdecId = (UINT8)ucEsId;
            if((prVdecEsInfoKeep->fgVPush) && (prVdecEsInfoKeep->fgVPushDecodeOnly))
            {
                rPar.u1AppMode = FBM_FBG_APP_OMX;
            }
            else if(prVdecEsInfoKeep->fgVPush)
            {
                rPar.u1AppMode = FBM_FBG_APP_OMX_DISP;
            }
            else if(prVdecEsInfo->e3DType == VDEC_3D_DUAL_STREAM)
            {
                rPar.u1AppMode = FBM_FBG_APP_Frame_Pack_3D;
            }
            else
            {
                rPar.u1AppMode = FBM_FBG_APP_NORMAL;
            }
            ucFbgId =
                FBM_CreateGroupExt(ucMJPEGTargetType,
                    FBM_VDEC_JPEG, 0, 0, &rPar);
        }
        
		if(ucFbgId==FBM_FBG_ID_UNKNOWN)
		{
            LOG(0, "create FBG failed.\n");
            return (INT32)FALSE;
		}

		FBM_SetPlayMode(ucFbgId,FBM_FBG_MM_MODE);
        if(prVdecEsInfoKeep->fgVPush)
        {
            if(prVdecEsInfoKeep->fgVPushDecodeOnly)
            {
                FBM_SetPlayMode(ucFbgId,FBM_FBG_NOTDISPLAY_MODE);
            }
            FBM_RegFbgCbFunc(ucFbgId,
                FBM_CB_FUNC_FB_READY_EX_IND, (UINT32)_VPUSH_DecodeDone, ucEsId);
        }

        FBM_SetSyncStc(ucFbgId, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId);
        FBM_FbgChgNotify(ucFbgId, ucEsId);
		prVdecImgInfo->ucFbgId=ucFbgId;
		prVdecEsInfo->ucFbId = 0xFF;
	}
	else
	{
		ucFbgId=prVdecImgInfo->ucFbgId;
	}

    if (FBM_ChkFrameBufferFlag(ucFbgId, (UINT32)FBM_FLAG_RESET))
    {
        FBM_ResetGroup(ucFbgId);
        FBM_ClrFrameBufferFlag(ucFbgId, (UINT32)FBM_FLAG_RESET);
        FBM_ClrFrameBufferFlag(ucFbgId, (UINT32)FBM_FLAG_NOT_READY);                  
    }
    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
    if (prFbmSeqHdr == NULL)
    {
        LOG(0, "get seq hdr failed.\n");        
        return (INT32)FALSE;
    }

#if 1
// for vdec to venc support, we always use FBM_GetEmptyFrameBuffer
// This will simply modification of FBM
    if((prVdecEsInfoKeep->eCurFMT == VDEC_FMT_JPG)||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_PNG))
    {
        if((prVdecEsInfo->ucFbId == 0xFF) || !prVdecEsInfo->fgNotClearBuffer)
        {
            if((prVdecEsInfo->ucFbId != 0xFF))
            {
                FBM_UpdateReferenceList(ucFbgId, ucFbId, TRUE);
            }
            ucFbId = FBM_GetEmptyFrameBuffer(ucFbgId, (UINT32)1500);
        }
        else
        {
            ucFbId = prVdecEsInfo->ucFbId;
        }
    }
    else
    {
        ucFbId = FBM_GetEmptyFrameBuffer(ucFbgId, (UINT32)1500);
    }
#else
    ucFbId = FBM_GetEmptyRefFrameBuffer(ucFbgId, (UINT32)1500);
#endif
    if (ucFbId == (UCHAR)FBM_FB_ID_UNKNOWN)
    {
        LOG(0, "get empty frame buffer failed.\n");        
        return (INT32)FALSE;
    }
    FBM_GetFrameBufferAddr(ucFbgId, ucFbId, &u4YAddr, &u4CAddr);    

    prVdecEsInfo->ucFbgId = ucFbgId;
    prVdecEsInfo->ucFbId = ucFbId;
    
    prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucFbId);
    if (prFbmPicHdr == NULL)
    {
        LOG(0, "get pic hdr failed.\n");
        FBM_SetFrameBufferStatus(ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
        return (INT32)FALSE;
    }

    _VDEC_LockVld(ucEsId, ucMpvId);
    
    _VDEC_MJPEGSWRst(ucMpvId, ucEsId);

    if (IMG_GetDecState() != IMG_INIT)
    {
        FBM_SetFrameBufferStatus(ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
        _VDEC_UnlockVld(ucEsId, ucMpvId);
        return (INT32)FALSE;
    }

    
    x_memset((VOID *)&rDecOut, 0x0, sizeof(IMG_REGION_T));
    x_memset((VOID *)&rInfo, 0x0, sizeof(IMG_PARAM_T));
    rInfo.pu1InBuffer = (UINT8 *)prPesInfo->u4FifoStart;
    rInfo.u8BufSize = (UINT32)(prPesInfo->u4FifoEnd - prPesInfo->u4FifoStart);
    rInfo.rDstRegion.u4StartX = (UINT32)0;
    rInfo.rDstRegion.u4StartY = (UINT32)0;

    // you don't get width/height here
    // width = height = 0 now.
    /*
    if(prVdecEsInfo->rMMSeqInfo.u2_width<192)
    {
		u4FrameWidth=192;
    }
	else
    {
        u4FrameWidth=(UINT32)prVdecEsInfo->rMMSeqInfo.u2_width;
#ifdef CC_MT5365
        if(u4FrameWidth&0x3f)
        {
           u4FrameWidth=(u4FrameWidth+0x40)&0xffffff40;
        }
#endif
    }
	
    rInfo.rDstRegion.u4Width = u4FrameWidth;
    rInfo.rDstRegion.u4Height = u4FrameWidth;
    rInfo.rDstRegion.u4Pitch = u4FrameWidth;
    */
    rInfo.rDstRegion.u4Width = (UINT32)prVdecEsInfo->rMMSeqInfo.u2_width;
    rInfo.rDstRegion.u4Height = (UINT32)prVdecEsInfo->rMMSeqInfo.u2_height;
    rInfo.rDstRegion.u4Pitch = (UINT32)((prVdecEsInfo->rMMSeqInfo.u2_width + 0x3F)&(~0x3F));

    if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_PNG)
    {
        IMG_Set_ImgSize(prVdecEsInfo->u4Imagefilesize);
        rInfo.u4DecColorMode = GFX_COLORMODE_ARGB_D8888;
        rInfo.eDispColorMode = AYCBCR8888_DIRECT32;
    }            
    else
    {
        rInfo.eDispColorMode = Y_CBCR420_SEP16;
    }

    rInfo.eRotateDegree = CLOCKWISE_0;
    rInfo.bKeepRatio = TRUE;
    rInfo.prDecOut = &rDecOut;
    
    rInfo.rDstRegion.pu1Addr = (UINT8 *)u4YAddr;
    rInfo.rDstRegion.u4CbCrOffset = u4CAddr - u4YAddr;
    //TO DO
    rInfo.rMjpg.u4RingOffset = prPesInfo->u4VldReadPtr - prPesInfo->u4FifoStart;
    rInfo.rMjpg.ucFbgId = ucFbgId;

#ifdef VDEC_TIME_PROFILE

    prVdecImgInfo->rOther.u4CurDecMs = 0;
    prVdecImgInfo->rOther.u4CurDecCycle = 0;
    prVdecImgInfo->rOther.u4CurDramCycle = 0;
    prVdecImgInfo->rOther.u1DramMoniter = 2;

    HAL_GetTime(&_rTimeHwS);

    u4DelayClock = (BSP_GetDomainClock(CAL_SRC_DMPLL) >> 1) ; //dram cycle /sec
#if 0    
    if (prVdecImgInfo->rOther.u1DramMoniter == 1) //total bandwidth
    {
#ifdef CC_MT5395
        u4DramCycle = SRM_DramMonitor(0, SRM_DRV_TOTAL, u4DelayClock, 0);
#else
        u4DramCycle = SRM_DramMonitor(SRM_DRV_TOTAL, u4DelayClock, 0);
#endif
    }
    else if (prVdecImgInfo->rOther.u1DramMoniter == 2) //mpeg
#endif        
    {
#ifdef VDEC_TIME_PROFILE
#if defined(CC_MT5395) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
        u4DramCycle = SRM_DramMonitor(0, SRM_DRV_JPEGDEC, u4DelayClock, 0);
#else
        u4DramCycle = SRM_DramMonitor(SRM_DRV_JPEGDEC, u4DelayClock, 0);
#endif
#endif
    }

#endif

    if((prVdecEsInfoKeep->eCurFMT == VDEC_FMT_JPG)||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_PNG))
    {
        IMG_SetUseMw(TRUE); //block for initial buffer filling.
        if(prVdecEsInfo->fgIsThumbnail)
        {
            rInfo.rDstRegion.u4Width = prVdecEsInfo->u4OutputPicW;
            rInfo.rDstRegion.u4Height = prVdecEsInfo->u4OutputPicH;
            rInfo.bKeepRatio = FALSE;
            if((prVdecEsInfo->u4OutputPicW < 128) || (prVdecEsInfo->u4OutputPicH < 128))
            {
                rInfo.fgThumbnailDec = TRUE;
            }
        }
        else
        {
            rInfo.rDstRegion.u4Width = (UINT32)MJPEG_MAX_WIDTH;
            rInfo.rDstRegion.u4Height = (UINT32)MJPEG_MAX_HEIGHT;
        }
        rInfo.rDstRegion.u4Pitch = (UINT32)MJPEG_MAX_WIDTH;
    }
    else
    {
        IMG_SetFillBsBufEnd((UINT32)rInfo.u8BufSize, 0, TRUE);
    }
    IMG_Decode(rInfo);        

    VERIFY (x_sema_lock(_hMJPGDecSema, X_SEMA_OPTION_WAIT) == OSR_OK);

#ifdef VDEC_TIME_PROFILE

    _rTimeHwDt.u4Micros = 0;
    HAL_GetTime(&_rTimeHwE);
    HAL_GetDeltaTime(&_rTimeHwDt, &_rTimeHwS, &_rTimeHwE);

    u4DecCycle = MC_READ32(0x778);
    u4DramCycle = IO_REG32(DRAM_BASE, 0x190);

    prVdecImgInfo->rOther.u4CurDecMs += (_rTimeHwDt.u4Micros / 1000);
    prVdecImgInfo->rOther.u4CurDecCycle += u4DecCycle;
    prVdecImgInfo->rOther.u4CurDramCycle += u4DramCycle;

#endif

	//feedback real width&height, when MW don't pass the sequence info by raw data info (0xfb) in avi container
	if((prVdecEsInfo->rMMSeqInfo.u2_width==0)||(prVdecEsInfo->rMMSeqInfo.u2_height==0))
	{
		prVdecEsInfo->rMMSeqInfo.u2_width =rDecOut.u4Width;
		prVdecEsInfo->rMMSeqInfo.u2_height =rDecOut.u4Height;		
	}


        if(prVdecEsInfo->rMMSeqInfo.e_frame_rate==VDEC_SRC_FRAME_RATE_UNKNOWN)
        {
            //set a default frame rate
            prVdecEsInfo->rMMSeqInfo.e_frame_rate = VDEC_SRC_FRAME_RATE_29_97;
        }

	
    prFbmSeqHdr->ucAspRatInf = (UCHAR)prVdecEsInfo->rMMSeqInfo.e_src_asp;

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
            prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_50;
            break;
        case VDEC_SRC_FRAME_RATE_59_94: /* 60000/1001 (59.94...) */
            prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_60_;
            break;
        case VDEC_SRC_FRAME_RATE_60:
            prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_60;
            break;
        default:
            prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_UNKNOWN;
            break;
    }
    
    //prFbmSeqHdr->ucFrmRatCod = (UCHAR)prVdecEsInfo->rMMSeqInfo.e_frame_rate;
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

    if((prVdecEsInfoKeep->eCurFMT == VDEC_FMT_JPG)||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_PNG))
    {
        prFbmSeqHdr->u2HSize = MJPEG_MAX_WIDTH;
        prFbmSeqHdr->u2VSize = MJPEG_MAX_HEIGHT;
        prFbmSeqHdr->u2DHS = MJPEG_MAX_WIDTH;
        prFbmSeqHdr->u2DVS = MJPEG_MAX_HEIGHT;
        prFbmSeqHdr->u2OrgHSize = MJPEG_MAX_WIDTH;
        prFbmSeqHdr->u2OrgVSize = MJPEG_MAX_HEIGHT;
    }
#ifdef CC_3D_MM_DS_SUPPORT  
    if( prVdecEsInfo->e3DType == VDEC_3D_DUAL_STREAM)
     {
        prFbmSeqHdr->u1B2R3DTrickModeType =  prVdecEsInfo->u13DTrickModeType;
     }
#endif
#if defined(CC_MT5395) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    prFbmSeqHdr->fgRasterOrder = TRUE;
#else
    prFbmSeqHdr->fgRasterOrder = FALSE;
#endif
    prFbmPicHdr->ucPicCdTp = (UCHAR)I_TYPE;
    prFbmPicHdr->ucPicStruct = (UCHAR)3;
    prFbmPicHdr->fgProgressiveFrm = TRUE; 
    prFbmPicHdr->fgRepFirstFld = FALSE;
    prFbmPicHdr->fgTopFldFirst = FALSE;
    prFbmPicHdr->u4PTS = prVdecEsInfo->u4PTS;       

    //add for A-B repeart precisely
    prFbmPicHdr->u8Offset = prVdecEsInfo->u8Offset;   
    prFbmPicHdr->u8OffsetI = prVdecEsInfo->u8OffsetI;
    prFbmPicHdr->u4Timestap = prVdecEsInfo->u4Timestap;
    x_memcpy(&prFbmPicHdr->rExtra, &prVdecEsInfo->rExtra, sizeof(FBM_PIC_EXTRA_INFO_T));


#ifdef CC_RESIZE_SMALL_IMAGE
    if(((prFbmSeqHdr->u2HSize < FBM_IMG_RESIZE_LIMITATION) ||
        (prFbmSeqHdr->u2VSize < FBM_IMG_RESIZE_LIMITATION)) &&
        (!((prVdecEsInfoKeep->fgVPush) && (prVdecEsInfoKeep->fgVPushDecodeOnly))))
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
            prFbmSeqHdr->u2LineSize = ((prFbmSeqHdr->u2HSize + 0x3F) & (~0x3F));
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
            prFbmSeqHdr->u2VSize = ((prFbmSeqHdr->u2VSize + 0x3F) & (~0x3F));
            prFbmSeqHdr->u2OrgVSize = prFbmSeqHdr->u2VSize;
        }
        prFbmSeqHdr->u2DHS = prFbmSeqHdr->u2HSize;
        prFbmSeqHdr->u2DVS = prFbmSeqHdr->u2VSize;

        LOG(6, "-> 0 OrgHSize %d, OrgVSize %d, HSize %d, VSize %d, LineSize %d\n",
                   prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize,
                   prFbmSeqHdr->u2HSize, prFbmSeqHdr->u2VSize, prFbmSeqHdr->u2LineSize);
    }
    else
    {
        prFbmSeqHdr->fgResizeSmallPic = FALSE;
    }
    
    prFbmPicHdr->u4PicWidth = rDecOut.u4Width;
    prFbmPicHdr->u4PicWidthPitch = rDecOut.u4Pitch;
    prFbmPicHdr->u4PicHeight = rDecOut.u4Height;
#endif

    if (prVdecImgInfo->u4MJPEGNtyValue!= (UINT32)JPG_HW_NFY_FINISHED)
    {
    	//return (INT32)FALSE; 
    	LOG(1, "one frame broken\n");
    }
    CLR_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    fgNotDisplay = VDEC_IsNotDisplay((UCHAR)ucEsId, prFbmPicHdr->u4PTS, 0, 0);
    if (fgNotDisplay || (prVdecImgInfo->u4MJPEGNtyValue != (UINT32)JPG_HW_NFY_FINISHED))
    {
        FBM_UpdateReferenceList(ucFbgId, ucFbId, FALSE);
        FBM_SetFrameBufferStatus(ucFbgId, ucFbId, (UCHAR)FBM_FB_STATUS_EMPTY);
    }
    else
    {
#ifdef MJPEG_SUPPORT_VENC
        if (_VDEC_fg2VencFlowOn(ucEsId))
        {
            VDEC_VENC_SRC_INFO_T rVdec2VencSrcInfo;
            rVdec2VencSrcInfo.u4YAddr = u4YAddr;
            rVdec2VencSrcInfo.u4CAddr = u4CAddr;
            rVdec2VencSrcInfo.u4Pitch = prFbmSeqHdr->u2LineSize;
            rVdec2VencSrcInfo.u4Width = prFbmSeqHdr->u2HSize;
            rVdec2VencSrcInfo.u4Height = prFbmSeqHdr->u2VSize;
            rVdec2VencSrcInfo.u4Pts = prFbmPicHdr->u4PTS;
            if (!_VDEC_PrepareVEncSourse(ucEsId, &rVdec2VencSrcInfo))
            {
            }
        }
#endif
        FBM_UpdateReferenceList(ucFbgId, ucFbId, FALSE);
        #ifdef CC_REALD_3D_SUPPORT
        _VDEC_Check2D3DSwitch(ucEsId, ucFbId);
        #endif
        if((prVdecEsInfoKeep->eCurFMT == VDEC_FMT_JPG)||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_PNG))
        {
            x_memcpy((void*)&_rDecOut, (void*)&rDecOut, sizeof(_rDecOut));
            FBM_UpdateReferenceList(ucFbgId, ucFbId, TRUE);
            rCBData.u2OriginalWidth = rDecOut.u4Width;
            rCBData.u2OriginalHeight = rDecOut.u4Height;
            if((prVdecEsInfo->u4RotationDegree == 0) && (!prVdecEsInfo->fgIsThumbnail))
            {
                FBM_SetFrameBufferStatus(ucFbgId, ucFbId, (UCHAR)FBM_FB_STATUS_DISPLAYQ);
                rCBData.u2OutputWidth = MJPEG_MAX_WIDTH;
                rCBData.u2OutputHeight = MJPEG_MAX_HEIGHT;
                _VPUSH_ImgFrmDone(ucEsId, (void*)&rCBData);
            }
            else
            {
                //prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG);
                //check MPEG partition
                #if 0
                u4DstAddr = prFbmPool->u4Addr;
                if(u4YAddr < (prFbmPool->u4Addr + (prFbmPool->u4Size >> 1)))
                {
                    u4DstAddr += (prFbmPool->u4Size >> 1);
                }
                #endif
                u4DstAddr = u4CAddr + (u4CAddr - u4YAddr);
                FBM_SetFrameBufferStatus(ucFbgId, ucFbId, (UCHAR)FBM_FB_STATUS_EMPTY);
                if(prVdecEsInfo->fgIsThumbnail)
                {
                    //color convert
                    GFX_Lock();

                    GFX_SetColCnvFmt(E_YCFMT_420LINEAR, (UINT32)E_SWAP_BLOCK, (UINT32)E_VSTD_BT709, 
                        (UINT32)E_VSYS_COMP);
                    GFX_SetColCnvSrc((UINT8*)u4YAddr, prFbmSeqHdr->u2LineSize,
                        (UINT8*)u4CAddr, prFbmSeqHdr->u2LineSize, 0);
                    GFX_SetDst((UINT8*)u4DstAddr, CM_ARGB8888_DIRECT32, prVdecEsInfo->u4OutputPicW<<2);
                    GFX_SetAlpha(0xFF);
                    GFX_ColConv(0, 0, prVdecEsInfo->u4OutputPicW, prVdecEsInfo->u4OutputPicH);
                    
                    GFX_Flush();
                    GFX_Wait();
                    GFX_Unlock();
                    rCBData.u2OutputWidth = prVdecEsInfo->u4OutputPicW;
                    rCBData.u2OutputHeight = prVdecEsInfo->u4OutputPicH;
                    rCBData.pBuffer = (UINT8*)(VIRTUAL(u4DstAddr));
                    _VPUSH_ImgFrmDone(ucEsId, (void*)&rCBData);
                }
                else    //rotation
                {
                    _VDEC_MJPEGSetParam(ucEsId, 0, prVdecEsInfo->u4RotationDegree/90, 0, 0);
                }
            }
        }
        else
        {
            FBM_UpdateReferenceList(ucFbgId, ucFbId, FALSE);
            FBM_SetFrameBufferStatus(ucFbgId, ucFbId, (UCHAR)FBM_FB_STATUS_DISPLAYQ);
        }
        prVdecEsInfo->u4DisplayQPicCnt++;
        VDEC_CheckStopStatus(ucEsId, prFbmPicHdr->u4PTS, 0);
    }    
    prVdecImgInfo->u4ReadPtr=_VDEC_VldMJPEGRPtr(NULL);
    IMG_Stop();
    IMG_Release();
    
    _VDEC_UnlockVld(ucEsId, ucMpvId);
    
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
    VDEC_IMG_INSTANCE_T *prVdecImgInfo=NULL;
	prVdecImgInfo=&_rVdecImgInfo[ucEsId];
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    if (prVdecImgInfo->ucFbgId!= (UCHAR)FBM_FBG_ID_UNKNOWN)
    {
        FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, ucEsId);
        FBM_WaitUnlockFrameBuffer(prVdecImgInfo->ucFbgId, (UINT32)20);    // block until display unlock   
	    FBM_ReleaseGroup(prVdecImgInfo->ucFbgId);
	    prVdecImgInfo->ucFbgId = (UCHAR)FBM_FBG_ID_UNKNOWN;
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
    VDEC_IMG_INSTANCE_T *prVdecImgInfo=NULL;
	prVdecImgInfo=&_rVdecImgInfo[ucEsId];
    *ppvPesInfo = (VOID*)(&prVdecImgInfo->rPesInfo);
}

// *********************************************************************
// Function    : VOID _VDEC_MJPEGSetParam
// Description : Get param
// Parameter   :
// Return      :
// *********************************************************************
void _VDEC_MJPEGSetParam(UCHAR ucEsId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    UCHAR ucFbId;
    UINT32 u4DstYAddr, u4DstCAddr;
    FBM_PIC_HDR_T *prFbmPicHdrSrc;
    FBM_PIC_HDR_T *prFbmPicHdrDst;
    IMG_BUF_PARAM_T rCmd;
    IMG_REGION_T rOutputRegion = {0};
    VDEC_IMG_FRMDONE_DATA rCBData = {0};
    IMG_REGION_T rOutputDispRegion = {0};

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if (ucEsId >= VDEC_MAX_ES)
    {
        ASSERT(ucEsId < VDEC_MAX_ES);
        return;
    }

    ucFbId = FBM_GetEmptyFrameBuffer(prVdecEsInfo->ucFbgId, (UINT32)1500);
    if (ucFbId == (UCHAR)FBM_FB_ID_UNKNOWN)
    {
        LOG(0, "get empty frame buffer failed.\n");        
        return;
    }
    FBM_UpdateReferenceList(prVdecEsInfo->ucFbgId, ucFbId, FALSE);

    //picture header
    prFbmPicHdrSrc = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucFbId);
    prFbmPicHdrDst = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucFbId);
    if((prFbmPicHdrSrc == NULL) || (prFbmPicHdrDst == NULL))
    {
        LOG(0, "get pic hdr failed.\n");
        FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
        return;
    }
    x_memcpy((void*)prFbmPicHdrDst, (void*)prFbmPicHdrSrc, sizeof(FBM_PIC_HDR_T));

    //rotation
    x_memset((void*)&rCmd, 0, sizeof(rCmd));
    rCmd.bKeepRatio = TRUE;
    rCmd.eDispColorMode = Y_CBCR420_SEP16;
    rCmd.eRotateDegree = (ENUM_IMG_ROTATE_PHASE)u4Param1;

    x_memcpy((void*)&rCmd.rSrcRegion, (void*)&_rDecOut, sizeof(_rDecOut));
    x_memcpy((void*)&rCmd.rDstRegion, (void*)&_rDecOut, sizeof(_rDecOut));
    FBM_GetFrameBufferAddr(prVdecEsInfo->ucFbgId, ucFbId, &u4DstYAddr, &u4DstCAddr);
    rCmd.rDstRegion.pu1Addr = (UINT8*)u4DstYAddr;
    rCmd.rDstRegion.u4Width = rCmd.rDstRegion.u4Pitch;
    rCmd.rDstRegion.u4Height = rCmd.rDstRegion.u4TotalLine;
    rCmd.rDstRegion.u4StartX = 0;
    rCmd.rDstRegion.u4StartY = 0;
    rCmd.prDstRegion = &rOutputRegion;

    IMG_Set(rCmd, &rOutputDispRegion);
    
    //display directly
    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId, FBM_FB_STATUS_DISPLAYQ);

    rCBData.u2OriginalWidth = rCmd.prDstRegion->u4Width;
    rCBData.u2OriginalHeight = rCmd.prDstRegion->u4Height;
    rCBData.u2OutputWidth = MJPEG_MAX_WIDTH;
    rCBData.u2OutputHeight = MJPEG_MAX_HEIGHT;
    _VPUSH_ImgFrmDone(ucEsId, (void*)&rCBData);
}



// *********************************************************************
// Function    : VOID _VDEC_MJPEGGetParam
// Description : Get param
// Parameter   :
// Return      :
// *********************************************************************
VOID _VDEC_MJPEGGetParam(UCHAR ucEsId, UINT32 u4Type, UINT32* pu4Param1, UINT32* pu4Param2, UINT32* pu4Param3)
{
#ifdef VDEC_TIME_PROFILE
    VDEC_IMG_INSTANCE_T *prVdecImgInfo = NULL;
    MJPEGOther *prOther;

    prVdecImgInfo = &_rVdecImgInfo[ucEsId];
    prOther = &(prVdecImgInfo->rOther);

    ASSERT(ucEsId < VDEC_MAX_ES);
    ASSERT(pu4Param1 != NULL);
    ASSERT(pu4Param2 != NULL);
    ASSERT(pu4Param3 != NULL);

    if (u4Type == (UINT32)MJPEG_TIME_PROFILE)
    {
        *pu4Param1 = prVdecImgInfo->ucFbgId;
        *pu4Param2 = prOther->u4CurDecMs;       //hw dec
    }
    else if (u4Type == (UINT32)MJPEG_TIME_PROFILE2)
    {
        *pu4Param2 = 2;            
        *pu4Param3 = 1;
    }
    else if (u4Type == (UINT32)MJPEG_TIME_PROFILE3)
    {
        *pu4Param2 = prOther->u4CurDecCycle;
        *pu4Param3 = prOther->u4CurDramCycle;
    }
    else
    {
        ////////
    }
#endif

    UNUSED(ucEsId);
    UNUSED(u4Type);
    UNUSED(pu4Param1);
    UNUSED(pu4Param2);
    UNUSED(pu4Param3);
}

BOOL _VDEC_MJPEGIsPic(UCHAR ucEsId, VDEC_PES_INFO_T *prPes)
{
    UNUSED(ucEsId);
    UNUSED(prPes);
    return TRUE;
}

// *********************************************************************
// Function    : VOID _VDEC_MJPEGUpdDmxRptr
// Description : update dmx read pointer for using vdec
// Parameter   : 
// Return      : 
// *********************************************************************
void _VDEC_MJPEGUpdDmxRptr(UCHAR ucEsId)
{
    UINT32 u4CurRptr;
    VDEC_IMG_INSTANCE_T *prVdecImgInfo=NULL;
    ASSERT(ucEsId < (UCHAR)MJPEG_MAX_ES_NUM);
	prVdecImgInfo=&_rVdecImgInfo[ucEsId];    
    //u4CurRptr = _VDEC_VldMJPEGRPtr(&u4CurRptrBits);
    u4CurRptr=prVdecImgInfo->u4ReadPtr;
    //update dmx pointer
	//_DMX_GetVideoChannel(prVdecImgInfo->rPesInfo.ucEsId*3,&uChannelId);
    if (!DMX_UpdateVideoReadPointer(prVdecImgInfo->rPesInfo.ucEsId, u4CurRptr, u4CurRptr))                
    {                    
        LOG(0, "MJPEG ES(%d) DMX_UpdateVideoReadPointer Fail\n", prVdecImgInfo->rPesInfo.ucEsId);                
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
    VDEC_IMG_INSTANCE_T *prVdecImgInfo=NULL;
	VDEC_PES_INFO_T *prPesInfo;
    VDEC_ES_INFO_T* prVdecEsInfo;
    ASSERT(ucEsId < (UCHAR)MJPEG_MAX_ES_NUM);
	prVdecImgInfo=&_rVdecImgInfo[ucEsId]; 
	prPesInfo=&prVdecImgInfo->rPesInfo;
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    if (prVdecImgInfo->ucFbgId== (UCHAR)FBM_FBG_ID_UNKNOWN)
    {
        return FALSE;
    }
    if( prVdecEsInfo->e3DType != VDEC_3D_DUAL_STREAM)
    {
         if (FBM_ChkFrameBufferFlag(prVdecImgInfo->ucFbgId, (UINT32)FBM_FLAG_SKIP_PIC) && 
            (ucSkipMode != (UCHAR)IMode))
        {
            fgSkip = TRUE;
            if (u1SkipCnt > MJPEG_TRACK_SKIP)
            {
                fgSkip = FALSE;
                u1SkipCnt = 0;
            }
            if (fgSkip)
            {
                //update dmx
		  if (!DMX_UpdateVideoReadPointer(prPesInfo->ucEsId,  prPesInfo->u4VldReadPtr, prPesInfo->u4VldReadPtr))
                {                    
                    LOG(0, "EsFlwCtrl MJPEG ES(%d) DMX_UpdateVideoReadPointer Fail\n", prPesInfo->ucEsId);                
                }

                FBM_ClrFrameBufferFlag(prVdecImgInfo->ucFbgId, (UINT32)FBM_FLAG_SKIP_PIC);
                u1SkipCnt++;
                    
                return TRUE;
            }
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

