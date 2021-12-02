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
 * $RCSfile: vdec_mpeg4api.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_mpeg4api.c
 *  This file contains implementation of exported APIs of mpeg4 video decoder.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
#include "vdec_mpeg4psr.h"
#include "vdec_mpeg4dec.h"
#include "vdec_mpeg4util.h"
#include "vdec_mpeg4hw.h"
#include "vdec_mpeg4api.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_common.h"
#include "x_typedef.h"
#include "dmx_if.h"
#include "x_util.h"
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define VLD_VECTOR    	      	24
#define MAX_ES_NUM 1

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static VDEC_MPEG4_DEC_PARAM_INFO_T _rMpeg4DecInfo;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
//static void _TestMCRegister(void);

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

// *********************************************************************
// Function : _VDEC_Mpeg4GetHdlr(void)
// Description : Get Handler for MPEG4
// Parameter : 
// Return    : The address of this handler
// *********************************************************************
VDEC_HDLR _VDEC_Mpeg4GetHdlr(void)
{
  return (VDEC_HDLR)&_rMpeg4DecInfo;
}

// *********************************************************************
// Function : _VDEC_Mpeg4Init(VDEC_HDLR hVdec, INT32* pi4RetValue)
// Description : Get Handler for MPEG4
// Parameter : 
// Return    : None
// *********************************************************************
INT32 _VDEC_Mpeg4Init(void)
{
    VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec;

    prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo;

    x_memset((void *)prVdec, 0, sizeof(VDEC_MPEG4_DEC_PARAM_INFO_T));

    // Set some default values
    prVdec->fgDecError = FALSE;
    
    prVdec->u1VideoType = VIDEO_TYPE_ISO_MPEG4;
    prVdec->rM4vParams.rVol.u1QuantPrecision = 5;

    prVdec->fgPicQueue = FALSE;
    prVdec->fgPesQueue = FALSE;
    prVdec->u1FbgId = FBM_FBG_ID_UNKNOWN;
#ifdef IC_MPEG4_VERIFICATION
    prVdec->u4AddrMode = MPEG4_ADDR_MODE_8520_NORMAL; 
#else
    prVdec->u4AddrMode = MPEG4_ADDR_MODE_5351_NORMAL;
	#endif 

    prVdec->rM4vParams.rVop.rDirectMd.u4TFrm = 0xffffffff;

    prVdec->rPesInfo.u4FifoStart = 0;
    prVdec->rPesInfo.u4FifoEnd = 0xFFFFFF;	//init value, will get from pes queue later

#ifdef CC_MPEG4_DEBLOCKING
    prVdec->fgDbkEngrMode = FALSE;
#endif
    //_TestMCRegister();


    // Jack's test code
    //init hw module, fifo address is 0 now
    _VDEC_InitMpeg4VDecHW(prVdec->rPesInfo.u4FifoStart,  
                                             prVdec->rPesInfo.u4FifoStart, 
                                             prVdec->rPesInfo.u4FifoEnd, 
                                             FALSE, 
                                             (VDEC_HDLR)&_rMpeg4DecInfo);

    Mpeg4IsrInit();
    //set MC and VLD clock
    /*UINT32 u4ClockSetting;
    u4ClockSetting = ReadREG(0x2000D21C);
    u4ClockSetting &= 0xFFFFF0F0;
    u4ClockSetting |= 7 + 1<<8;
    WriteREG(u4ClockSetting);*/


    return E_VDEC_OK;
}


/** .
*@param .
 */
void _VDEC_Mpeg4SWRst(UCHAR ucVdecId, UCHAR ucEsId)
{    
	//VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec;
	//prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo;

	// reset
	_VDEC_VLDMpeg4WaitForSramStable();
	_VDEC_VLDMpeg4HWReset();

	UNUSED(ucVdecId);
	UNUSED(ucEsId);
}


/** unit of es packet is a vop.
*@param .
 */
INT32 _VDEC_Mpeg4PrsHdr(UCHAR ucVdecId, UCHAR ucEsId)
{
    INT32   i4RetValue;
    VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec;

    prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo;

    LOG(6, "_VDEC_Mpeg4PrsHdr ReadPtr = 0x%08x, Pts = 0x%08x\n", prVdec->rPesInfo.u4VldReadPtr, prVdec->rPesInfo.u4PTS);

#ifdef IC_MPEG4_VERIFICATION
    if ( prVdec->rPesInfo.fgEos)
    {
        return E_VDEC_BS_END;
    }

     if (prVdec->rPesInfo.u4VldReadPtr == prVdec->u4PrevPrsPtr)
    {
        Printf("duplicate pointer\n");
        return E_VDEC_NOT_CODED_FRAME;
    }

    prVdec->u4PrevPrsPtr = prVdec->rPesInfo.u4VldReadPtr;

#endif

    if (prVdec->rPesInfo.u4VldReadPtr == NULL /*|| prVdec->rPesInfo.fgEos*/)
    {
        return E_VDEC_FAIL;
    }

    //LOG(0, "_VDEC_Mpeg4PrsHdr prVdec->rPesInfo.u4VldReadPtr = 0x%8x\n", prVdec->rPesInfo.u4VldReadPtr);

    _VDEC_InitMpeg4VDecHW(prVdec->rPesInfo.u4VldReadPtr, 
        prVdec->rPesInfo.u4FifoStart, 
        prVdec->rPesInfo.u4FifoEnd,
        TRUE,
        (VDEC_HDLR)&_rMpeg4DecInfo);

    if (!prVdec->pfnPrsHdr)
    {
        _VDEC_PrsVideoType(prVdec, &i4RetValue);
    }

    ASSERT(prVdec->u1VideoType != VIDEO_TYPE_UNKNOWN);

    prVdec->pfnPrsHdr(prVdec, &i4RetValue);

#ifdef IC_MPEG4_VERIFICATION
    if (i4RetValue == (INT32)E_VDEC_FAIL)
    {
        LOG(5, "Enter _VDEC_Mpeg4PrsHdr Fail");
        return E_VDEC_FAIL;
    }
#else
    if ((i4RetValue != (INT32)E_VDEC_OK) &&
        (prVdec->rPesInfo.fgEos))
    {
        LOG(5, "Enter _VDEC_Mpeg4PrsHdr Fail");
        return E_VDEC_FAIL;
    }
#endif
    return i4RetValue;
}

/** .
*@param .
 */
INT32 _VDEC_Mpeg4DecStart(UCHAR ucVdecId, UCHAR ucEsId)
{
  INT32 i4RetValue = E_VDEC_OK;

  VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo;

  prVdec->pfnDecInit(prVdec, &i4RetValue);
  //_VMpeg4DEC_DecProc((VDEC_MPEG4_DEC_PARAM_INFO_T*)hVdec, pi4RetValue);
  //_VTestMpeg4DEC_DecProc((VDEC_MPEG4_DEC_PARAM_INFO_T*)hVdec, pi4RetValue);
  return i4RetValue;
}

/** .
*@param .
 */
INT32 _VDEC_Mpeg4DecFin(UCHAR ucVdecId, UCHAR ucEsId)
{
	UINT32 u4Temp;
 	INT32 i4RetValue = E_VDEC_OK;
 	VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo;
  
	if(_VDEC_WaitMpeg4DecDone(prVdec))
	{
		if (prVdec->u1VideoType == VIDEO_TYPE_DX311 && 
		    prVdec->u1VopCodingType == VOP_CODING_TYPE_I)
		{
			UINT32 u4RealAddr = 0, u4Bits = 0;
			
			_VDEC_VLDVdec2Barl();
			u4RealAddr = (UINT32)_VDEC_VldMpeg4RPtr(&u4Bits);

			if (((prVdec->rPesInfo.u4VldReadPtr * 8) - ((u4RealAddr * 8) + u4Bits)) >= 17)
			{
				u4Temp = _VDEC_Mpeg4GetBitStream(&prVdec->u4BitCount, 0);
				LOG(7, "Rounding Control = 0x%8x\n", u4Temp);
				prVdec->rDx3Params.fgSwitchRounding = (u4Temp >> 15) & 0x01;
			}
		}
	}
	else
	{
		LOG(3, "Fail in _VDEC_WaitMpeg4DecDone");  
		 i4RetValue = (INT32)E_VDEC_FAIL; //decode error
	}

	return i4RetValue;
}

/** .
*@param .
 */
void _VDEC_Mpeg4Stop(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo;

    if(prVdec->u1FbgId != FBM_FBG_ID_UNKNOWN)
    {
        FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, ucEsId);
        FBM_WaitUnlockFrameBuffer(prVdec->u1FbgId, 
        	20);    // block until display unlock            
        FBM_ReleaseGroup(prVdec->u1FbgId);
        prVdec->u1FbgId = FBM_FBG_ID_UNKNOWN;
    }
    if(prVdecEsInfo->ucFbgId != FBM_FBG_ID_UNKNOWN)
    {
        FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, ucEsId);
        FBM_WaitUnlockFrameBuffer(prVdecEsInfo->ucFbgId, 
        	20);    // block until display unlock            
        FBM_ReleaseGroup(prVdecEsInfo->ucFbgId);
        prVdecEsInfo->ucFbgId = FBM_FBG_ID_UNKNOWN;
    }
    _VDEC_LockFlushMutex(ucEsId);

    if (prVdecEsInfo->fgFlushEsmQ)
    {
        _VDEC_FlushEs(ucEsId);
        prVdecEsInfo->fgFlushEsmQ = FALSE;
        _VDEC_UnlockFlushSema(ucEsId);        
    }
    _VDEC_ClearEsInfo(ucEsId);

    _VDEC_UnlockFlushMutex(ucEsId);
}

/** .
*@param .
 */
void _VDEC_GetMpeg4PesStruct(UCHAR ucEsId, void **ppvPesInfo)
{
  VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo;

  if(prVdec->fgPesQueue)
  {
    *ppvPesInfo = NULL;
  }
  else
  {
    *ppvPesInfo = (void*)(&prVdec->rPesInfo);
  }
  
}

void _VDEC_MPEG4DropFrame(UCHAR ucEsId)
{
    VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec;
    UINT32 u4CurRptr;
    
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo;

    u4CurRptr = prVdec->rPesInfo.u4VldReadPtr;
    
    if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, u4CurRptr, u4CurRptr))                
    {                    
        LOG(1, "Mpeg4 ES(%d) DMX_UpdateVideoReadPointer Fail\n", prVdec->rPesInfo.ucEsId);                
    }
}

void _VDEC_MPEG4UpdDmxRptr(UCHAR ucEsId)
{
    VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec;
    UINT32 u4CurRptr, u4CurRptrBits;
    
    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo;

    u4CurRptr = _VDEC_VldMpeg4RPtr(&u4CurRptrBits);
    
#ifndef IC_MPEG4_VERIFICATION
    //LOG(0, "UpdRptr 0x%x\n", u4CurRptr);
    //update dmx pointer
    if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, u4CurRptr, u4CurRptr))                
    {                    
        LOG(0, "Mpeg4 ES(%d) DMX_UpdateVideoReadPointer Fail\n", prVdec->rPesInfo.ucEsId);                
    }
#endif    
}

void _VDEC_MPEG4SetDmxWptr(UCHAR ucEsId, UINT32 u4Wptr)
{
    VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec;

    ASSERT(ucEsId < MAX_ES_NUM);
    prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo;

    //LOG(0, "UpdWptr 0x%x\n", u4Wptr);
    prVdec->u4WPtr = (u4Wptr & (~0xf));
    _VDEC_UpdateVldMpeg4WPtr(prVdec->u4WPtr);
}


/** for emulation
*@param .
 */
void _VDEC_GetMpeg4DispPic(UCHAR ucEsId, void* pvInfo)
{
	VDEC_DISP_PIC_INFO_T* prDispInfo;
  	VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec;
	
	prDispInfo = (VDEC_DISP_PIC_INFO_T*)pvInfo;
	prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo;
	
	prDispInfo->pvYAddr = (void*)prVdec->ucDecYPtr;
	prDispInfo->pvCAddr = (void*)prVdec->ucDecCbCrPtr;
	//pu1DecWorkBuf
	prDispInfo->u4YSize = prVdec->u4DramWidth * prVdec->u2Height;
	prDispInfo->u4CSize = prDispInfo->u4YSize >> 1;

	//LOG(0, "prDispInfo->u4YSize = %ld\n", prDispInfo->u4YSize);
	//LOG(0, "prVdec->u4DramWidth = %ld prVdec->u4DramHeight = %ld\n", prVdec->u4DramWidth, prVdec->u4DramHeight);
	
	prDispInfo->u1PicStruct = prVdec->u1PicStructure;
	prDispInfo->u4W = prVdec->u2Width;
	prDispInfo->u4H = prVdec->u2Height;
	//prDispInfo->u4DramW = prVdec->u4DramWidth;
	//prDispInfo->u4DramH = prVdec->u4DramHeight;

}


BOOL _VDEC_Mpeg4EsFlwCtrl(UCHAR ucEsId, UCHAR* pucVldId, INT8 i1EsBufStatus, UCHAR ucSkipMode)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec;    

    ASSERT(ucEsId < MAX_ES_NUM);
    ASSERT(pucVldId !=NULL);

    prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo;
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    LOG(9, "Es flw control\n");
    LOG(9, "Skip mode = %ld\n", ucSkipMode);

    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I))
    {
        FBM_InvalidateRefFrameBuffer(prVdecEsInfo->ucFbgId);
        CLR_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);

        /*if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC))
        {
            TOGGLE_DECFLAG(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC);
        }*/            
    }

    
    // ask mpeg decode to skip one picture due to repeat
    // if ESM-Q Ns > Underflow Threshold, otherwise, ignore
    if (ucSkipMode == IPMode)
    {
        if (prVdec->rPesInfo.ucPicType == B_TYPE) 
        {
            if (_rMpeg4DecInfo.fgEnCalPts)
            {
                if ((_rMpeg4DecInfo.u8PtsScale != 0) &&
                    (_rMpeg4DecInfo.u4Rate != 0))
                {
                    UINT64 u8Numerator = 0, u8Denominator = 0;

                    u8Numerator = _rMpeg4DecInfo.u8PtsScale + _rMpeg4DecInfo.u8Remainder;
                    u8Denominator = (UINT64)_rMpeg4DecInfo.u4Rate;

                    _rMpeg4DecInfo.u4DispPts += 
                        u8Div6432(u8Numerator, u8Denominator, &_rMpeg4DecInfo.u8Remainder);
                }
            }

            // Temporaily return True for skip b frame
            return TRUE;
        }
    }
    else if (ucSkipMode == IMode)
    {
        if ((prVdec->rPesInfo.ucPicType != I_TYPE) && (!prVdec->rPesInfo.fgSeqHdr)) 
        {
            return TRUE;
        }
    }

    return FALSE;
}


void _VDEC_Mpeg4SetParam(UCHAR ucEsId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3)
{
    VDEC_SEQUENCE_DATA_T* pSeqData = NULL;
    
    if (u4Type == VDEC_MM_SEQ_INFO)
    {
        pSeqData = (VDEC_SEQUENCE_DATA_T*)u4Param1;

        _rMpeg4DecInfo.u2Width = (UINT16)pSeqData->u2_width;
        _rMpeg4DecInfo.u2Height = (UINT16)pSeqData->u2_height;
    }    
}


VOID _VDEC_MPEG4StartPTS(UCHAR ucEsId, BOOL fgEnCalPTS, UINT32 u4Pts, UINT32 u4PtsDiff)
{
    VDEC_PTS_INFO_T *prPTSInfo = NULL;
    VDEC_MPEG4_DEC_PARAM_INFO_T *prVdec = NULL;

    if(ucEsId >= MAX_ES_NUM)
    {
        ASSERT(ucEsId < MAX_ES_NUM);
        return;
    }
    
    prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo;
    prVdec->fgEnCalPts = fgEnCalPTS;
    if(fgEnCalPTS)
    {            
        prVdec->u4DispPts = u4Pts;
        prVdec->u8Remainder = 0;
        prPTSInfo = (VDEC_PTS_INFO_T*)u4PtsDiff;
        if(prPTSInfo)
        {
            prVdec->u8PtsScale = prPTSInfo->u8Scale;
            prVdec->u4Rate = prPTSInfo->u4Rate;
        }
    }
}


VOID _VDEC_MPEG4CalPts(UCHAR ucEsId, UCHAR ucFbId)
{
    FBM_PIC_HDR_T *prFbmPicHdr = 0;
    VDEC_MPEG4_DEC_PARAM_INFO_T *prDecPrm = &_rMpeg4DecInfo;
    VDEC_M4V_VOL_INFO_T 	*prVolPrm;
    VDEC_M4V_PIC_INFO_T 	*prVopPrm;
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;

    prVolPrm = &prDecPrm->rM4vParams.rVol;
    prVopPrm = &prDecPrm->rM4vParams.rVop;

    prVdecEsInfo = _VDEC_GetEsInfo(0);

    prFbmPicHdr = FBM_GetFrameBufferPicHdr(prDecPrm ->u1FbgId, ucFbId);
    if(prFbmPicHdr == NULL)
    {
        LOG(3, "_MPV_CalcPts prFbmPicHdr null\n");
        return;
    }

     if (prDecPrm->fgEnCalPts)
    {
        if (prDecPrm->fgValidPts)
        {
            LOG(7, "Valid PTS\n");
            prFbmPicHdr->u4PTS = prDecPrm->u4RefPts;
            prDecPrm->u4DispPts = prDecPrm->u4RefPts;
            prDecPrm->fgValidPts = FALSE;
        }
        else
        {
            prFbmPicHdr->u4PTS = prDecPrm->u4DispPts;
        }

        LOG(7, "Enable PTS calculation Cur PTS = 0x%08x\n", prFbmPicHdr->u4PTS);
        if ((prDecPrm->u8PtsScale != 0) &&
            (prDecPrm->u4Rate != 0))
        {
            UINT64 u8Numerator = 0, u8Denominator = 0;

            u8Numerator = prDecPrm->u8PtsScale + prDecPrm->u8Remainder;
            u8Denominator = (UINT64)prDecPrm->u4Rate;

            prDecPrm->u4DispPts += 
                u8Div6432(u8Numerator, u8Denominator, &prDecPrm->u8Remainder)*
                (prDecPrm->rPesInfo.u2SkipVidCount + 1);
        }
    }
    else
    {
        if (prDecPrm->fgValidPts)
        {
            prFbmPicHdr->u4PTS = prDecPrm->u4RefPts;
            LOG(7, "Use container's PTS = 0x%08x\n", prFbmPicHdr->u4PTS);
        }
        else
        {
            UINT64 u8Numerator = 0, u8Denominator = 0;

            if (prVopPrm->u2VopTimeIncrement >= prVopPrm->u2VopPrevTimeIncrement)
            {
                u8Numerator = 
                    (prVopPrm->u2VopTimeIncrement - prVopPrm->u2VopPrevTimeIncrement) * MPEG4_TIME_SCALE;
            }
            else
            {
                u8Numerator =  
                    (prVolPrm->u4TimeIncrementResolution - prVopPrm->u2VopPrevTimeIncrement + prVopPrm->u2VopTimeIncrement) * MPEG4_TIME_SCALE;
            }

            u8Numerator += prDecPrm->u8Remainder;
            u8Denominator = (UINT64)prVolPrm->u4TimeIncrementResolution;

            prFbmPicHdr->u4PTS = prDecPrm->u4DispPts;

            prDecPrm->u4DispPts += 
                u8Div6432(u8Numerator, u8Denominator, &prDecPrm->u8Remainder)*
                (prDecPrm->rPesInfo.u2SkipVidCount + 1);
            LOG(7, "Use ES PTS = 0x%08x\n", prFbmPicHdr->u4PTS);
        }
    }

    prFbmPicHdr->u2DecodingOrder = prVdecEsInfo->u2DecodingOrder;
    prFbmPicHdr->u4TotlaTimeOffset = prVdecEsInfo->u4TotlaTimeOffset;

    prFbmPicHdr->u8Offset = prDecPrm->u8RecOffset;
    prFbmPicHdr->u8OffsetI = prDecPrm->u8RecOffsetI;
    prFbmPicHdr->fgTopFldFirst = prVopPrm->fgTopFldFirst;
    prFbmPicHdr->ucTrueZeroPTS = FBM_USE_ZERO_PTS_IF_ZERO;
    prFbmPicHdr->i4TemporalRef = prDecPrm->i4TemporalRef;
    prDecPrm->i4TemporalRef++;
}


void _VDEC_Mpeg4DumpCheckSum(void)
{
	UINT32 u4Temp;
       UINT32 u4Cnt = 0;

	/*LOG(0, " MBX: 0x%x, MBY: 0x%x \n",
        	MC_READ32(RO_MC_MBX), MC_READ32(RO_MC_MBY));

	u4Temp = VLD_READ32(0x2F4);
	LOG(0, "DC AC Out Dram1 (Register %ld) =  0x%8x\n", (0x2F4>>2), u4Temp);

	u4Temp = VLD_READ32(0x3AC);
	LOG(0, "DC AC Out (Register %ld) =  0x%8x\n", (0x3AC >> 2), u4Temp);

	u4Temp = VLD_READ32(0x3B0);
	LOG(0, "DC AC Out Dram1 (Register %ld) =  0x%8x\n", (0x3B0 >> 2), u4Temp);

	u4Temp = VLD_READ32(0x3B4);
	LOG(0, "DC AC Out Dram2 (Register %ld) =  0x%8x\n", (0x3B4 >> 2), u4Temp);

	u4Temp = VLD_READ32(0x3B8);
	LOG(0, "is_data (Register %ld) =  0x%8x\n", (0x3B8 >> 2), u4Temp);

		
	u4Temp = MC_READ32(0x5E8);
	LOG(0, "idct_in_y0_y1_checksum_reg (Register %ld) =  0x%8x\n", (0x5E8 >> 2), u4Temp);

	u4Temp = MC_READ32(0x5EC);
	LOG(0, "idct_in_y2_y3_checksum_reg (Register %ld) =  0x%8x\n", (0x5EC>>2), u4Temp);

	u4Temp = MC_READ32(0x5F0);
	LOG(0, "idct_in_cb_cr_checksum_reg (Register %ld) =  0x%8x\n", (0x5F0>>2), u4Temp);

	u4Temp = MC_READ32(0x5F4);
	LOG(0, "mc_out_y_checksum_reg (Register %ld) =  0x%8x\n", (0x5F4>>2), u4Temp);

	u4Temp = MC_READ32(0x5F8);
	LOG(0, "mc_out_c_checksum_reg (Register %ld) =  0x%8x\n", (0x5F8>>2), u4Temp);

	u4Temp = MC_READ32(0x5FC);
	LOG(0, "mv_checksum_reg (Register %ld) =  0x%8x\n", (0x5FC>>2), u4Temp);

	u4Temp = MC_READ32(0x600);
	LOG(0, "mc_para_1_checksum_reg (Register %ld) =  0x%8x\n",  (0x600>>2), u4Temp);

	u4Temp = MC_READ32(0x604);
	LOG(0, "mc_para_2_checksum_reg (Register %ld) =  0x%8x\n", (0x604>>2), u4Temp);

	u4Temp = MC_READ32(0x608);
	LOG(0, "pp_y_chk_sum_reg (Register %ld) =  0x%8x\n", (0x608>>2), u4Temp);

	u4Temp = MC_READ32(0x60C);
	LOG(0, "pp_c_chk_sum_reg (Register %ld) =  0x%8x\n",  (0x60C>>2), u4Temp);

	u4Temp = MC_READ32(0x610);
	LOG(0, "hiu_rbus_checksum_reg (Register %ld) =  0x%8x\n", (0x610>>2), u4Temp);

	u4Temp = MC_READ32(0x614);
	LOG(0, "wp_tbl_checksum_reg (Register %ld) =  0x%8x\n", (0x614>>2), u4Temp);

	u4Temp = MC_READ32(0x618);
	LOG(0, "dram_rbus_checksum_reg (Register %ld) =  0x%8x\n", (0x618>>2), u4Temp);

	u4Temp = MC_READ32(0x61C);
	LOG(0, "qpel_qbi_checksum_reg (Register %ld) =  0x%8x\n", (0x61C>>2), u4Temp);

	u4Temp = MC_READ32(0x620);
	LOG(0, "qpel_qbo_checksum_reg (Register %ld) =  0x%8x\n", (0x620>>2), u4Temp);
	
	u4Temp = MC_READ32(0x624);
	LOG(0, "mbi_checksum_reg (Register %ld) =  0x%8x\n", (0x624>>2), u4Temp);

	u4Temp = VLD_READ32(0xFC);
	LOG(0, "Dram RP =  0x%8x\n", u4Temp);

	u4Temp = (VLD_READ32(0xEc) & 0x3f);
	LOG(0, "Sram RP =  0x%8x\n", u4Temp);

	u4Temp = ((VLD_READ32(0xEC) >> 16) & 0x3f);
	LOG(0, "Sram WP =  0x%8x\n", u4Temp);

	u4Temp = (VLD_READ32(0xEc) & 0x3f);
	LOG(0, "Sram RP =  0x%8x\n", u4Temp);

	u4Temp = VLD_READ32(0xE0);
	LOG(0, "RO_VLD_FSBI (%d) = 0x%x\n", (0xE0>>2), u4Temp);

	u4Temp = VLD_READ32(0xE4);
	LOG(0, "RO_VLD_ERRFG (%d) = 0x%x\n", (0xE4>>2), u4Temp);

	u4Temp = VLD_READ32(0xE8);
	LOG(0, "RO_VLD_FETCHOK (%d) = 0x%x\n", (0xE8>>2), u4Temp);

	u4Temp = VLD_READ32(0xEC);
	LOG(0, "RO_VLD_VBAR (%d) = 0x%x\n", (0xEC>>2), u4Temp);

	u4Temp = VLD_READ32(0xF4);
	LOG(0, "RO_VLD_SRAMCTRL (%d) = 0x%x\n", (0xF4>>2), u4Temp);

	u4Temp = VLD_READ32(0xF8);
	LOG(0, "RO_VLD_VWPTR (%d) = 0x%x\n", (0xF8>>2), u4Temp);*/

	Printf("==============================================================================\n");

	for (u4Cnt = 0x84; u4Cnt <= 0x9c; u4Cnt += 4)
	{
		u4Temp = VLD_READ32(u4Cnt);
		Printf("0x%8x (vld Register %ld) =  0x%8x\n", u4Cnt, (u4Cnt>>2), u4Temp);
	}
	
       for (u4Cnt = 0xa8; u4Cnt <= 0x3ec; u4Cnt += 4)
	{
		u4Temp = VLD_READ32(u4Cnt);
		Printf("0x%8x (vld Register %ld) =  0x%8x\n", u4Cnt, (u4Cnt>>2), u4Temp);
	}
	
	for (u4Cnt = 0x0; u4Cnt <= 0x800; u4Cnt += 4)
	{
		u4Temp = MC_READ32(u4Cnt);
		Printf("0x%8x (mc Register %ld) =  0x%8x\n", u4Cnt, (u4Cnt>>2), u4Temp);
	}

	Printf("==============================================================================\n");


}


/*static void _TestMCRegister(void)
{
	INT32   i;
	UINT32 u4Temp;

	for (i = 136; i <= 184; i += 4)
	{
		VLD_WRITE32( i, 0xFF);
		u4Temp = VLD_READ32(i);
		if (u4Temp != 0xFF)
		{
			LOG(7, "0x%x Register is not right. The value is 0x%x\n", i, u4Temp);
		}
	}

	for (i = 192; i <= 220; i += 4)
	{
		VLD_WRITE32( i, 0xFF);
		u4Temp = VLD_READ32(i);
		if (u4Temp != 0xFF)
		{
			LOG(7, "0x%x Register is not right. The value is 0x%x\n", i, u4Temp);
		}
	}

	VLD_WRITE32( RW_VLD_WAITT, 0xFF);
	u4Temp = VLD_READ32(RW_VLD_WAITT);
	if (u4Temp != 0xFF)
	{
		LOG(7, "0x%x Register is not right. The value is 0x%x\n", RW_VLD_WAITT, u4Temp);
	}

	VLD_WRITE32( RW_VLD_DIGEST_BOUNDARY, 0xFF);
	u4Temp = VLD_READ32(RW_VLD_DIGEST_BOUNDARY);
	if (u4Temp != 0xFF)
	{
		LOG(7, "0x%x Register is not right. The value is 0x%x\n", RW_VLD_DIGEST_BOUNDARY, u4Temp);
	}

	for (i = 0; i <= 456; i += 4)
	{
		MC_WRITE32( i, 0xFF);
		u4Temp = MC_READ32(i);
		if (u4Temp != 0xFF)
		{
			Printf("0x%x Register is not right. The value is 0x%x\n", i, u4Temp);
		}
	}

	for (i = 508; i <= 600; i += 4)
	{
		MC_WRITE32( i, 0xFF);
		u4Temp = MC_READ32(i);
		if (u4Temp != 0xFF)
		{
			Printf("0x%x Register is not right. The value is 0x%x\n", i, u4Temp);
		}
	}

}*/
