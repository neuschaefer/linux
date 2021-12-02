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
#ifdef IC_MPEG4_VERIFICATION
#include "drv_t32.h"
#endif
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define VLD_VECTOR    	      	24
//#define MAX_ES_NUM 4

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifdef IC_MPEG4_VERIFICATION
static CHAR ucFimeName[256] = {0};
//static UINT32 gu4FrameNum = 0;
#endif
static VDEC_MPEG4_DEC_PARAM_INFO_T _rMpeg4DecInfo[VDEC_MAX_ES];

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
VDEC_HDLR _VDEC_Mpeg4GetHdlr(UCHAR ucEsId)
{
   return (VDEC_HDLR)&_rMpeg4DecInfo[ucEsId];
}

// *********************************************************************
// Function : _VDEC_Mpeg4Init(VDEC_HDLR hVdec, INT32* pi4RetValue)
// Description : Get Handler for MPEG4
// Parameter : 
// Return    : None
// *********************************************************************
INT32 _VDEC_Mpeg4Init(UCHAR ucEsId)
{
    VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec;

    prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo[ucEsId];

    x_memset((void *)prVdec, 0, sizeof(VDEC_MPEG4_DEC_PARAM_INFO_T));

    // Set some default values
    prVdec->fgDecError = FALSE;
    
    prVdec->u1VideoType = VIDEO_TYPE_ISO_MPEG4;
    prVdec->rM4vParams.rVol.u1QuantPrecision = 5;

    prVdec->fgPicQueue = FALSE;
    prVdec->fgPesQueue = FALSE;
    prVdec->u1FbgId = FBM_FBG_ID_UNKNOWN;
#ifdef IC_MPEG4_VERIFICATION
#ifdef CC_MPEG4_EMULATION_DDR3
    prVdec->u4AddrMode = MPEG4_ADDR_MODE_5351_NORMAL;
#else
    prVdec->u4AddrMode = MPEG4_ADDR_MODE_8520_NORMAL; 
#endif
    prVdec->u4WPtr = 0xFFFFFFFF;
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

#ifdef CC_MPEG4_VIRTUAL_P
    prVdec->fgEnVirtualP = TRUE;
#endif


    // Jack's test code
    //init hw module, fifo address is 0 now
    
    _VDEC_LockVld(prVdec->rPesInfo.ucEsId, prVdec->rPesInfo.ucMpvId);  
     
    _VDEC_InitMpeg4VDecHW(prVdec->rPesInfo.u4FifoStart,  
                                             prVdec->rPesInfo.u4FifoStart, 
                                             prVdec->rPesInfo.u4FifoEnd, 
                                             FALSE, 
                                             (VDEC_HDLR)&_rMpeg4DecInfo[ucEsId]);
   

    Mpeg4IsrInit();
    //set MC and VLD clock
    /*UINT32 u4ClockSetting;
    u4ClockSetting = ReadREG(0x2000D21C);
    u4ClockSetting &= 0xFFFFF0F0;
    u4ClockSetting |= 7 + 1<<8;
    WriteREG(u4ClockSetting);*/
    _VDEC_UnlockVld(prVdec->rPesInfo.ucEsId, prVdec->rPesInfo.ucMpvId);

#ifdef MPEG4_TRACKING_MODE
    prVdec->fgFirstB = TRUE;
#endif

    return E_VDEC_OK;
}


/** .
*@param .
 */
void _VDEC_Mpeg4SWRst(UCHAR ucVdecId, UCHAR ucEsId)
{    
    //VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec;
    //prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo;
    _VDEC_LockVld(ucEsId, ucVdecId); 

    
    // reset
    _VDEC_VLDMpeg4WaitForSramStable();
    _VDEC_VLDMpeg4HWReset();

    _VDEC_UnlockVld(ucEsId, ucVdecId);
    UNUSED(ucVdecId);
    UNUSED(ucEsId);
}


/** unit of es packet is a vop.
*@param .
 */
INT32 _VDEC_Mpeg4PrsHdr(UCHAR ucVdecId, UCHAR ucEsId)
{
    INT32   i4RetValue = 0;
    VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec;

    prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo[ucEsId];

    LOG(6, "_VDEC_Mpeg4PrsHdr ReadPtr = 0x%08x, Pts = 0x%08x\n", prVdec->rPesInfo.u4VldReadPtr, prVdec->rPesInfo.u4PTS);
#ifdef IC_MPEG4_VERIFICATION
    if ( prVdec->rPesInfo.fgEos)
    {         
         UINT32 *pcCrcCheckY = NULL;
         UINT32 *pcCrcCheckC = NULL;
         UINT32 u4YNumber = 0;
         UINT32 u4CNumber = 0;
         
         pcCrcCheckY  = VDEC_GetCrcY();
         u4YNumber = VDEC_GetCrcYNum();
         pcCrcCheckC  = VDEC_GetCrcC();
         u4CNumber = VDEC_GetCrcCNum();

         
        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_Ycrc.bin\" 0x%08x--0x%08x", 
				"d:\\LOW_CLK_CRC\\", ucFimeName, (UINT32)pcCrcCheckY, (UINT32)(pcCrcCheckY + u4YNumber) - 1));
        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_Ccrc.bin\" 0x%08x--0x%08x", 
				"d:\\LOW_CLK_CRC\\", ucFimeName, (UINT32)pcCrcCheckC, (UINT32)(pcCrcCheckC + u4CNumber) - 1));
         VDEC_ResetCrcPam();

        return E_VDEC_BS_END;
    }

     if (prVdec->rPesInfo.u4VldReadPtr == prVdec->u4PrevPrsPtr)
    {
        return E_VDEC_NOT_CODED_FRAME;
    }

    prVdec->u4PrevPrsPtr = prVdec->rPesInfo.u4VldReadPtr;

#endif

    _VDEC_LockVld(prVdec->rPesInfo.ucEsId, prVdec->rPesInfo.ucMpvId); 

	
    if (prVdec->rPesInfo.u4VldReadPtr == (UINT32) NULL || prVdec->rPesInfo.fgEos)
    {
        LOG(2, "u4VldReadPtr\n");
        return E_VDEC_FAIL;
    }
    
    //Printf("Prs lock %d\n",prVdec->rPesInfo.ucEsId );
    //LOG(0, "_VDEC_Mpeg4PrsHdr prVdec->rPesInfo.u4VldReadPtr = 0x%8x\n", prVdec->rPesInfo.u4VldReadPtr);
	
    prVdec->u4DecReadBits = 0;
    _VDEC_InitMpeg4VDecHW(prVdec->rPesInfo.u4VldReadPtr, 
        prVdec->rPesInfo.u4FifoStart, 
        prVdec->rPesInfo.u4FifoEnd,
        TRUE,
        (VDEC_HDLR)&_rMpeg4DecInfo[ucEsId]);

    if (!prVdec->pfnPrsHdr)
    {
        _VDEC_PrsVideoType(prVdec, ucEsId, &i4RetValue);
    }

    ASSERT(prVdec->u1VideoType != VIDEO_TYPE_UNKNOWN);

    prVdec->pfnPrsHdr(prVdec, &i4RetValue,ucEsId);

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
    if((!((i4RetValue == E_VDEC_OK) || (i4RetValue == E_VDEC_FAIL)))&&
        (!(prVdec->rPesInfo.fgEos)))
    {
         //Printf("prs  ID %d fgEos %d\n", prVdec->rPesInfo.ucEsId,prVdec->rPesInfo.fgEos);
         _VDEC_UnlockVld(prVdec->rPesInfo.ucEsId, prVdec->rPesInfo.ucMpvId);
    }

    return i4RetValue;
}

/** .
*@param .
 */
INT32 _VDEC_Mpeg4DecStart(UCHAR ucVdecId, UCHAR ucEsId)
{
  INT32 i4RetValue = E_VDEC_OK;

  VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo[ucEsId];
  _VDEC_LockVld(ucEsId, prVdec->rPesInfo.ucMpvId);
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
 	VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo[ucEsId];
  
	if(_VDEC_WaitMpeg4DecDone(prVdec,ucEsId))
	{
		if (prVdec->u1VideoType == VIDEO_TYPE_DX311 && 
		    prVdec->u1VopCodingType == VOP_CODING_TYPE_I)
		{
			UINT32 u4RealAddr = 0, u4Bits = 0;
#ifndef CC_MT5398
        		_VDEC_VLDVdec2Barl();
#endif
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
    
    _VDEC_UnlockVld(ucEsId, prVdec->rPesInfo.ucMpvId);
	return i4RetValue;
}

/** .
*@param .
 */
void _VDEC_Mpeg4Stop(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo[ucEsId];

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
    //_VDEC_ClearEsInfo(ucEsId);

    _VDEC_UnlockFlushMutex(ucEsId);
}

/** .
*@param .
 */
void _VDEC_GetMpeg4PesStruct(UCHAR ucEsId, void **ppvPesInfo)
{
  VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo[ucEsId];

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
    VDEC_MPEG4_DEC_PARAM_INFO_T* prDecPrm;

    VDEC_M4V_VOL_INFO_T 	*prVolPrm;
    VDEC_M4V_PIC_INFO_T 	*prVopPrm;
    
//  FBM_SEQ_HDR_T *prFbmSeqHdr;
    FBM_PIC_HDR_T * prFbmPicHdr;
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    UINT32 u4CurRptr;
    BOOL   fgNotDisplay = FALSE;
//  UINT32 u4AddrY = 0;
//  UINT32 u4AddrC = 0;
    UCHAR ucFRefFbId   = 0;
    UCHAR ucBRefFbId   = 0;
    
    ASSERT(ucEsId < VDEC_MAX_ES);
    prDecPrm = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo[ucEsId];
    prVolPrm = &prDecPrm->rM4vParams.rVol;
    prVopPrm = &prDecPrm->rM4vParams.rVop;
    u4CurRptr = prDecPrm->rPesInfo.u4VldReadPtr;
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if(prDecPrm->u1FbgId != FBM_FBG_ID_UNKNOWN)
    {
         prVdecEsInfo->u4FailCnt = 0;
    }
    
    if (!DMX_UpdateVideoReadPointer(prDecPrm->rPesInfo.ucEsId, u4CurRptr, u4CurRptr))                
    {                    
        LOG(3, "Mpeg4 ES(%d) DMX_UpdateVideoReadPointer Fail\n", prDecPrm->rPesInfo.ucEsId);                
    }
    
    if (prDecPrm->fgInvalidSequence)
    {
        prDecPrm->fgInvalidSequence = FALSE;
        _VDEC_UnlockVld(prDecPrm->rPesInfo.ucEsId, prDecPrm->rPesInfo.ucMpvId);
        return;
    }
    
    if (prDecPrm->u1FbgId != FBM_FBG_ID_UNKNOWN)
    {
        FBM_GetRefFrameBuffer(prDecPrm->u1FbgId, &ucFRefFbId, &ucBRefFbId);
    }
    else
    {
        LOG(3, "MPEG4 FBG is Unknown\n");
        _VDEC_UnlockVld(prDecPrm->rPesInfo.ucEsId, prDecPrm->rPesInfo.ucMpvId);
        return;
    }
    
    if ((ucBRefFbId != FBM_FBG_ID_UNKNOWN)&&
        (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_B))
    {
        prDecPrm->u1FbId = FBM_GetEmptyBFrameBuffer(prDecPrm->u1FbgId, MPEG4_WAIT_DISP_TIME);
        if( prDecPrm->u1FbId == FBM_FBG_ID_UNKNOWN)
        {
            LOG(3, "MPEG4 FB ID is Unknown\n");
            _VDEC_UnlockVld(prDecPrm->rPesInfo.ucEsId, prDecPrm->rPesInfo.ucMpvId);
            return;
        }
        prFbmPicHdr = FBM_GetFrameBufferPicHdr(prDecPrm ->u1FbgId, prDecPrm->u1FbId);
        if(prFbmPicHdr)
        {
            if (prDecPrm->fgEnCalPts)
            {
                if (prDecPrm->fgValidPts)
                {
                    LOG(7, "Valid PTS\n");
                    prFbmPicHdr->u4PTS = prDecPrm->u4RefPts;
                    prDecPrm->u4DispPts = prDecPrm->u4RefPts;
                    prDecPrm->fgValidPts = FALSE;
                    prDecPrm->fgCheckVP = FALSE;
                }
                else
                {
                    prFbmPicHdr->u4PTS =  prDecPrm->u4DispPts;
                }

                LOG(7, "Enable PTS calculation Cur PTS = 0x%08x\n", prFbmPicHdr->u4PTS);
                if ((prDecPrm->u8PtsScale != 0) &&
                    (prDecPrm->u4Rate != 0))
                {
                    UINT64 u8Numerator = 0, u8Denominator = 0;
                    
                    u8Numerator = prDecPrm->u8PtsScale + prDecPrm->u8Remainder;
                    u8Denominator = (UINT64)prDecPrm->u4Rate;

                    prDecPrm->u4DispPts += 
                        u8Div6432(u8Numerator, u8Denominator, 
                        &prDecPrm->u8Remainder)*(prDecPrm->rPesInfo.u2SkipVidCount + 1 + prDecPrm->u4VopNotCoded);
                    prDecPrm->u4VopNotCoded = 0;    
                }
            }
            else
            {
                if (!prDecPrm->fgValidPts)
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

                    prDecPrm->u4DispPts += 
                        u8Div6432(u8Numerator, u8Denominator, &prDecPrm->u8Remainder);
                }
            }
        }

        fgNotDisplay =  VDEC_IsNotDisplay(0, prDecPrm->u4DispPts, 0, 0);
        if(!fgNotDisplay)
        {   
#if 1
            if (prFbmPicHdr)
            {
                prFbmPicHdr->fgDummyFrame = 1;
                FBM_SetFrameBufferStatus(prDecPrm->u1FbgId, prDecPrm->u1FbId, FBM_FB_STATUS_DISPLAYQ);
            }
#else
            prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prDecPrm->u1FbgId);
            if(prFbmSeqHdr != NULL)
            {
	            FBM_GetFrameBufferAddr(prDecPrm->u1FbgId, prDecPrm->u1FbId, &u4AddrY, &u4AddrC);
	            if((u4AddrY) && (u4AddrC) && prFbmSeqHdr && prFbmPicHdr)
	            {
#if 1
					prFbmPicHdr->fgDummyFrame = 1;
					FBM_SetFrameBufferStatus(prDecPrm->u1FbgId, prDecPrm->u1FbId, FBM_FB_STATUS_DISPLAYQ);
#else
	                x_memset((void*)VIRTUAL(u4AddrY), 0, (prFbmSeqHdr->u2LineSize*prFbmSeqHdr->u2VSize));
	                x_memset((void*)VIRTUAL(u4AddrC), 128, (prFbmSeqHdr->u2LineSize*prFbmSeqHdr->u2VSize)/2);
	                FBM_SetFrameBufferStatus(prDecPrm->u1FbgId, prDecPrm->u1FbId, FBM_FB_STATUS_DISPLAYQ);
#endif
	            }
	            else
	            {
	                 LOG(3, "get frame buffer fail \n");
	            }
	          }
	          else
	          {
	          	LOG(3,"prFbmSeqHdr Is NUll\n");
	          }
#endif
        }
        else
        {
            FBM_SetFrameBufferStatus(prDecPrm->u1FbgId, prDecPrm->u1FbId, FBM_FB_STATUS_EMPTY);
        }
    }
    else
    {
        prDecPrm->fgValidPts = prDecPrm->rPesInfo.fgDtsValid;
        prDecPrm->u4RefPts = prDecPrm->rPesInfo.u4PTS;
        prDecPrm->u8RecOffset = prDecPrm->rPesInfo.u8Offset;
        prDecPrm->u8RecOffsetI = prDecPrm->rPesInfo.u8OffsetI;
        prDecPrm->u1FbId = FBM_GetEmptyRefFrameBuffer(prDecPrm->u1FbgId, MPEG4_WAIT_DISP_TIME);
        if( prDecPrm->u1FbId == FBM_FBG_ID_UNKNOWN)
        {
            _VDEC_UnlockVld(prDecPrm->rPesInfo.ucEsId, prDecPrm->rPesInfo.ucMpvId);
            return;
        }
        prVopPrm->u4PrevDispTime = prVopPrm->u4NextDispTime;
        prVopPrm->u4NextDispTime = prVopPrm->u4CurDispTime;
        prVopPrm->rDirectMd.u4Trd = prVopPrm->u4NextDispTime - prVopPrm->u4PrevDispTime;
        prDecPrm->fgDecError = TRUE;   

        fgNotDisplay =  VDEC_IsNotDisplay(0, prDecPrm->u4DispPts, 0, 0);
        if(!fgNotDisplay)
        {   
#if 1
            _VDEC_MPEG4CalPts(ucEsId, ucBRefFbId);
            prFbmPicHdr = FBM_GetFrameBufferPicHdr(prDecPrm->u1FbgId, ucBRefFbId);
            if (prFbmPicHdr)
            {
                prFbmPicHdr->u4PicWidth = prDecPrm->u2Width;
                prFbmPicHdr->u4PicHeight = prDecPrm->u2Height;
                prFbmPicHdr->u4PicWidthPitch = prDecPrm->u4LineSize;
            }
            FBM_SetFrameBufferStatus(prDecPrm->u1FbgId, ucBRefFbId, FBM_FB_STATUS_DISPLAYQ);
            prFbmPicHdr = FBM_GetFrameBufferPicHdr(prDecPrm ->u1FbgId, prDecPrm->u1FbId);
            if (prFbmPicHdr)
            {
                prFbmPicHdr->fgDummyFrame = 1;
                FBM_SetFrameBufferStatus(prDecPrm->u1FbgId, prDecPrm->u1FbId, FBM_FB_STATUS_READY);
            }
#else
            prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prDecPrm->u1FbgId);
            if((ucBRefFbId != FBM_FBG_ID_UNKNOWN)&&(prFbmSeqHdr!=NULL))
            {
                prFbmPicHdr = FBM_GetFrameBufferPicHdr(prDecPrm ->u1FbgId, ucBRefFbId);
                if(prFbmPicHdr != NULL)
                {
                    prFbmPicHdr->u4PTS = prDecPrm->u4RefPts;
                    FBM_GetFrameBufferAddr(prDecPrm->u1FbgId, ucBRefFbId, &u4AddrY, &u4AddrC);
                    if((u4AddrY > 0) && (u4AddrC > 0) && prFbmSeqHdr && prFbmPicHdr)
                    {
#if 1
						prFbmPicHdr->fgDummyFrame = 1;
						FBM_SetFrameBufferStatus(prDecPrm->u1FbgId,ucBRefFbId,FBM_FB_STATUS_DISPLAYQ);
#else
                        x_memset((void*)VIRTUAL(u4AddrY), 0, (prFbmSeqHdr->u2LineSize*prFbmSeqHdr->u2VSize));
                        x_memset((void*)VIRTUAL(u4AddrC), 128, (prFbmSeqHdr->u2LineSize*prFbmSeqHdr->u2VSize)/2);
                       FBM_SetFrameBufferStatus(prDecPrm->u1FbgId,ucBRefFbId,FBM_FB_STATUS_DISPLAYQ);
#endif
                    }
                }
                else
                {
                	LOG(3, "prFbmPicHdr is NUll\n");
                } 
            }
            FBM_SetFrameBufferStatus(prDecPrm->u1FbgId,prDecPrm->u1FbId,FBM_FB_STATUS_READY);
#endif
        }
        else
        {
            FBM_SetFrameBufferStatus(prDecPrm->u1FbgId,prDecPrm->u1FbId,FBM_FB_STATUS_EMPTY);
        }
    }

    if(prDecPrm->u1VopCodingType == VOP_CODING_TYPE_I)
    {
        prDecPrm->fgValidPts = TRUE;
        prDecPrm->u4RefPts = prDecPrm->rPesInfo.u4PTS;
    }
     _VDEC_UnlockVld(prDecPrm->rPesInfo.ucEsId, prDecPrm->rPesInfo.ucMpvId);
}

void _VDEC_MPEG4UpdDmxRptr(UCHAR ucEsId)
{
    VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec;
    
#ifndef IC_MPEG4_VERIFICATION
    //UINT32 u4CurRptr, u4CurRptrBits;
#endif

    ASSERT(ucEsId < VDEC_MAX_ES);
    prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo[ucEsId];

    if (prVdec->rPesInfo.fgNotUpdateDmxRp)
    {
        _VDEC_UnlockVld(prVdec->rPesInfo.ucEsId, prVdec->rPesInfo.ucMpvId);
        return;
    }

#ifndef IC_MPEG4_VERIFICATION
    //u4CurRptr = _VDEC_VldMpeg4RPtr(&u4CurRptrBits);
#endif    

#ifndef IC_MPEG4_VERIFICATION

    //update dmx pointer
    //if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, u4CurRptr, u4CurRptr))                
    if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, prVdec->rPesInfo.u4VldReadPtr, prVdec->rPesInfo.u4VldReadPtr))
    {                    
        LOG(0, "Mpeg4 ES(%d) DMX_UpdateVideoReadPointer Fail\n", prVdec->rPesInfo.ucEsId);                
    }
#endif    
     //Printf("Update Unlock %d\n",prVdec->rPesInfo.ucEsId );
    //_VDEC_UnlockVld(prVdec->rPesInfo.ucEsId, prVdec->rPesInfo.ucMpvId);
}


static void _VDEC_MPEG4ForceUpdDmxRptr(UCHAR ucEsId)
{
    VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec;
    
    ASSERT(ucEsId < VDEC_MAX_ES);
    prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo[ucEsId];

    if (!prVdec)
    {
        LOG(3, "NULL pointer in _VDEC_MPEG4ForceUpdDmxRptr\n");
        return;
    }
    
    //update dmx pointer
    if (!DMX_UpdateVideoReadPointer(prVdec->rPesInfo.ucEsId, prVdec->rPesInfo.u4VldReadPtr, 
        prVdec->rPesInfo.u4VldReadPtr))                
    {                    
        LOG(0, "Mpeg4 ES(%d) DMX_UpdateVideoReadPointer Fail\n", prVdec->rPesInfo.ucEsId);                
    }
}


void _VDEC_MPEG4SetDmxWptr(UCHAR ucEsId, UINT32 u4Wptr)
{
    VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec;

    ASSERT(ucEsId < VDEC_MAX_ES);
    prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo[ucEsId];
    
   // LOG(0, "UpdWptr 0x%x\n", u4Wptr);
    prVdec->u4WPtr = (u4Wptr & (~0xf));
   //LOG(3, "u4WPtr = %x\n", prVdec->u4WPtr );
  //  _VDEC_UpdateVldMpeg4WPtr(prVdec->u4WPtr);
}


/** for emulation
*@param .
 */
void _VDEC_GetMpeg4DispPic(UCHAR ucEsId, void* pvInfo)
{
	VDEC_DISP_PIC_INFO_T* prDispInfo;
  	VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec;
	
	prDispInfo = (VDEC_DISP_PIC_INFO_T*)pvInfo;
	prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo[ucEsId];
	
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

    ASSERT(pucVldId !=NULL);

    if(ucEsId >= VDEC_MAX_ES)
    {
        ASSERT(ucEsId < VDEC_MAX_ES); 
        return FALSE;
    }

    prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo[ucEsId];
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
            if (_rMpeg4DecInfo[ucEsId].fgEnCalPts)
            {
                if ((_rMpeg4DecInfo[ucEsId].u8PtsScale != 0) && (_rMpeg4DecInfo[ucEsId].u4Rate != 0))
                {
                    UINT64 u8Numerator = 0, u8Denominator = 0;

                    u8Numerator = _rMpeg4DecInfo[ucEsId].u8PtsScale + _rMpeg4DecInfo[ucEsId].u8Remainder;
                    u8Denominator = (UINT64)_rMpeg4DecInfo[ucEsId].u4Rate;

                    _rMpeg4DecInfo[ucEsId].u4DispPts += 
                        (u8Div6432(u8Numerator, u8Denominator, &_rMpeg4DecInfo[ucEsId].u8Remainder) *
                        (prVdec->rPesInfo.u2SkipVidCount + 1 +  prVdec->u4VopNotCoded));
                    prVdec->u4VopNotCoded = 0;
                }
            }

            if (prVdec->rPesInfo.fgNoVdecCalPts)
            {
                ASSERT(prVdec->fgCheckVP == FALSE);
                prVdec->fgCheckVP = prVdec->rPesInfo.fgNoVdecCalPts;
            }
            
            // Temporaily return True for skip b frame
            _VDEC_MPEG4ForceUpdDmxRptr(ucEsId);
            return TRUE;
        }
    }
    else if (ucSkipMode == IMode)
    {
        if ((prVdec->rPesInfo.ucPicType != I_TYPE) && (!prVdec->rPesInfo.fgSeqHdr)) 
        {
            _VDEC_MPEG4ForceUpdDmxRptr(ucEsId);
            return TRUE;
        }
    }

    return FALSE;
}


void _VDEC_MPEG4GetParam(UCHAR ucEsId, UINT32 u4Type, UINT32* pu4Param1, UINT32* pu4Param2, UINT32* pu4Param3)
{
#ifdef VDEC_TIME_PROFILE

    VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec;
    
    ASSERT(ucEsId < VDEC_MAX_ES);
    ASSERT(pu4Param1 !=NULL);
    ASSERT(pu4Param2 !=NULL);
    ASSERT(pu4Param3 !=NULL);

    prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo[ucEsId];
    
    if(u4Type == (UINT32)MPEG4_DISP_IDX)
    {
        *pu4Param1 = prVdec->u1FbgId;
        *pu4Param2 = prVdec->u1FbId;
    }
    else if(u4Type == (UINT32)MPEG4_TIME_PROFILE)
    {
        *pu4Param1 = prVdec->u1FbgId;
        *pu4Param2 = prVdec->u4CurDecMs;       //hw dec
    }
    else if(u4Type == (UINT32)MPEG4_TIME_PROFILE2)
    {
         switch (prVdec->u1VopCodingType)
         {
            case VOP_CODING_TYPE_I:
                *pu4Param2 = 2;
                break;
            case VOP_CODING_TYPE_P:
                *pu4Param2 = 0;
                break;
            case VOP_CODING_TYPE_B:
                *pu4Param2 = 1;
                break;
         }
         
         if(prVdec->u1VideoType == VIDEO_TYPE_ISO_MPEG4)
         {
            if (prVdec->rM4vParams.rVol.fgInterlaced)
            {
                if(prVdec->rM4vParams.rVop.fgAlternateVerticalScanFlag)
                {
                    *pu4Param3 = 1;
                }
                else
                {
                    *pu4Param3 = 0;
                }
            }
            else
            {
                *pu4Param3 = 1;
            }
         }
         else
         {
             *pu4Param3 = 1;
         }
    }
    else if(u4Type == (UINT32)MPEG4_TIME_PROFILE3)
    {
        *pu4Param1 = prVdec->u4CurSwMicro;
        *pu4Param2 = prVdec->u4CurDecCycle;
        *pu4Param3 = prVdec->u4CurDramCycle;
    }
    else if (u4Type == (UINT32)MPEG4_DISP_IDX)
    {
        *pu4Param1 = prVdec->u1FbgId;
        *pu4Param2 = prVdec->u1FbId;
        *pu4Param3 = 0;
    }
#endif
}


void _VDEC_Mpeg4SetParam(UCHAR ucEsId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3)
{
    VDEC_SEQUENCE_DATA_T* pSeqData = NULL;
    
    VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo[ucEsId];
    
    if (u4Type == VDEC_MM_SEQ_INFO)
    {
        pSeqData = (VDEC_SEQUENCE_DATA_T*)u4Param1;

        prVdec->u2Width = (UINT16)pSeqData->u2_width;
        prVdec->u2Height = (UINT16)pSeqData->u2_height;
    } 

#ifdef VDEC_TIME_PROFILE
    else if(u4Type == (UINT32)MPEG4_TIME_PROFILE2)
    {
        prVdec->u1DramMoniter = (UINT8)u4Param1;
    }
 #endif
}


VOID _VDEC_MPEG4StartPTS(UCHAR ucEsId, BOOL fgEnCalPTS, UINT32 u4Pts, UINT32 u4PtsDiff)
{
    VDEC_PTS_INFO_T *prPTSInfo = NULL;
    VDEC_MPEG4_DEC_PARAM_INFO_T *prVdec = NULL;

    LOG(3, "ucEsId = %x\n",ucEsId);
   
    if(ucEsId >= VDEC_MAX_ES)
    {
        ASSERT(ucEsId < VDEC_MAX_ES); 
        return;
    }
    
    prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)&_rMpeg4DecInfo[ucEsId];
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


#ifdef VDEC_TIME_PROFILE
BOOL _VDEC_MPEG4IsPic(UCHAR ucEsId, VDEC_PES_INFO_T *prPes)
{
    UCHAR *pucBuf;
    UINT32 u4Addr;

    FBM_SEQ_HDR_T* prFbmSeqHdr = NULL;
    VDEC_MPEG4_DEC_PARAM_INFO_T *prDecPrm = &_rMpeg4DecInfo[ucEsId];

    UNUSED(pucBuf);
    UNUSED(u4Addr);
    UNUSED(prFbmSeqHdr);
    UNUSED(prDecPrm);

    // Every Access Unit is one picture except MPEG4
    if (prDecPrm->u1VideoType == VIDEO_TYPE_DX311)
    {
        return TRUE;
    }
    else if (prDecPrm->u1VideoType == VIDEO_TYPE_H263)
    {
        return TRUE;
    }
    else if ((prDecPrm->u1VideoType == VIDEO_TYPE_ISO_MPEG4) ||
             (prDecPrm->u1VideoType == VIDEO_TYPE_DIVX_MPEG4))
    {
        LOG(5, "MPEG4 video\n");
        u4Addr = prPes->u4VldReadPtr + 3;
        if (u4Addr >= prPes->u4FifoEnd)
        {
            u4Addr -= (prPes->u4FifoEnd - prPes->u4FifoStart);
        }
        pucBuf = (UCHAR*)VIRTUAL(u4Addr);

        if (pucBuf[0] == 0xB6)
        {
            LOG(5, "It's a video frame\n");
            return TRUE;
        }
        else
        {
            LOG(5, "It's not a video frame\n");
            return FALSE;
        }
    }
    else if (prDecPrm->u1VideoType == VIDEO_TYPE_FLV)
    {
        return TRUE;
    }

    return FALSE;
}
#endif

VOID _VDEC_MPEG4CalPts(UCHAR ucEsId, UCHAR ucFbId)
{
    FBM_PIC_HDR_T *prFbmPicHdr = 0;
    VDEC_MPEG4_DEC_PARAM_INFO_T *prDecPrm = NULL;
    VDEC_M4V_VOL_INFO_T 	*prVolPrm;
    VDEC_M4V_PIC_INFO_T 	*prVopPrm;
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;

    if(ucEsId >= VDEC_MAX_ES)
    {
	    LOG(3, "_MPV_CalcPts invalid ucEsId\n");
        ASSERT(ucEsId < VDEC_MAX_ES); 
        return;
    }

	prDecPrm = &_rMpeg4DecInfo[ucEsId];
    prVolPrm = &prDecPrm->rM4vParams.rVol;
    prVopPrm = &prDecPrm->rM4vParams.rVop;

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

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
            prDecPrm->fgCheckVP = FALSE;
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
                (prDecPrm->rPesInfo.u2SkipVidCount + 1 + prDecPrm->u4VopNotCoded);
                
            prDecPrm->u4VopNotCoded = 0;
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

    x_memcpy(&prFbmPicHdr->rExtra,
        &prDecPrm->rRecExtra,
        sizeof(FBM_PIC_EXTRA_INFO_T));
    prFbmPicHdr->u8Offset = prDecPrm->u8RecOffset;
    prFbmPicHdr->u8OffsetI = prDecPrm->u8RecOffsetI;
    prFbmPicHdr->u4Timestap = prVdecEsInfo->u4Timestap;
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
		//Printf("0x%8x (vld Register %ld) =  0x%8x\n", u4Cnt, (u4Cnt>>2), u4Temp);
		Printf("%d (0x%x) = 0x%x\n", (u4Cnt>>2), u4Cnt, u4Temp);
	}
	
#if 1
       for (u4Cnt = 0xa8; u4Cnt <= 0x400; u4Cnt += 4)  // 42-256
	{
		u4Temp = VLD_READ32(u4Cnt);
		//Printf("0x%8x (vld Register %ld) =  0x%8x\n", u4Cnt, (u4Cnt>>2), u4Temp);
		Printf("%d (0x%x) = 0x%x\n", (u4Cnt>>2), u4Cnt, u4Temp);
	}

       for (u4Cnt = 0x800; u4Cnt <= 0x900; u4Cnt += 4)  // 512-576
	{
		u4Temp = VLD_READ32(u4Cnt);
		//Printf("0x%8x (vld Register %ld) =  0x%8x\n", u4Cnt, (u4Cnt>>2), u4Temp);
		Printf("%d (0x%x) = 0x%x\n", (u4Cnt>>2), u4Cnt, u4Temp);
	}
#else
       for (u4Cnt = 0xa8; u4Cnt <= 0x118; u4Cnt += 4)
	{
		u4Temp = VLD_READ32(u4Cnt);
		//Printf("0x%8x (vld Register %ld) =  0x%8x\n", u4Cnt, (u4Cnt>>2), u4Temp);
		Printf("%d (0x%x) = 0x%x\n", (u4Cnt>>2), u4Cnt, u4Temp);
	}

       for (u4Cnt = 0x1c0; u4Cnt <= 0x3fc; u4Cnt += 4)
	{
		u4Temp = VLD_READ32(u4Cnt);
		//Printf("0x%8x (vld Register %ld) =  0x%8x\n", u4Cnt, (u4Cnt>>2), u4Temp);
		Printf("%d (0x%x) = 0x%x\n", (u4Cnt>>2), u4Cnt, u4Temp);
	}
#endif

        Printf("MC Settings\n");
	
	for (u4Cnt = 0x0; u4Cnt <= 0xAF0; u4Cnt += 4)
	{
		u4Temp = MC_READ32(u4Cnt);
		//Printf("0x%8x (mc Register %ld) =  0x%8x\n", u4Cnt, (u4Cnt>>2), u4Temp);
		Printf("%d (0x%x) = 0x%x\n", (u4Cnt>>2), u4Cnt, u4Temp);
	}

        Printf("PP Settings\n");

        for (u4Cnt = 0; u4Cnt <= 0xFC; u4Cnt += 4)
        {
            u4Temp = PP_READ32(u4Cnt);
           Printf("%d (0x%x) = 0x%x\n", (u4Cnt>>2), u4Cnt, u4Temp);
            x_thread_delay(10);
        }

        for (u4Cnt = 0x200; u4Cnt <= 0x2FC; u4Cnt += 4)
        {
            u4Temp = PP_READ32(u4Cnt);
           Printf("%d (0x%x) = 0x%x\n", (u4Cnt>>2), u4Cnt, u4Temp);
            x_thread_delay(10);
        }

        for (u4Cnt = 0x500; u4Cnt <= 0x5FC; u4Cnt += 4)
        {
            u4Temp = PP_READ32(u4Cnt);
           Printf("%d (0x%x) = 0x%x\n", (u4Cnt>>2), u4Cnt, u4Temp);
            x_thread_delay(10);
        }

        for (u4Cnt = 0x900; u4Cnt <= 0x9FC; u4Cnt += 4)
        {
            u4Temp = PP_READ32(u4Cnt);
           Printf("%d (0x%x) = 0x%x\n", (u4Cnt>>2), u4Cnt, u4Temp);
            x_thread_delay(10);
        }

        Printf("VLD Top Settings\n");
        
        for (u4Cnt = 0; u4Cnt <= 0x28; u4Cnt += 4)
        {
            u4Temp = VLD_TOP_READ32(u4Cnt);
           Printf("%d (0x%x) = 0x%x\n", (u4Cnt>>2), u4Cnt, u4Temp);
            x_thread_delay(10);
        }

        Printf("VDMISC Settings\n");
        
        for (u4Cnt = 0; u4Cnt <= 0x100; u4Cnt += 4)
        {
            u4Temp = IO_READ32(IO_VIRT + 0x3000, u4Cnt);
           Printf("%d (0x%x) = 0x%x\n", (u4Cnt>>2), u4Cnt, u4Temp);
            x_thread_delay(10);
        }
#if 0
        u4Temp = VLD_TOP_READ32(RW_VLD_TOP_PIC_WIDTH);
	 Printf("0x%8x (vld top Register %ld) =  0x%8x\n", RW_VLD_TOP_PIC_WIDTH, (RW_VLD_TOP_PIC_WIDTH>>2), u4Temp);

        u4Temp = VLD_TOP_READ32(RW_VLD_TOP_PIC_HEIGHT);
	 Printf("0x%8x (vld top Register %ld) =  0x%8x\n", RW_VLD_TOP_PIC_HEIGHT, (RW_VLD_TOP_PIC_HEIGHT>>2), u4Temp);
#endif
	Printf("==============================================================================\n");


}


VOID _VDEC_MPEG4SetIsr(UCHAR ucEsId)
{
    Mpeg4IsrInit();
    UNUSED(ucEsId);
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
#ifdef IC_MPEG4_VERIFICATION
VOID VDEC_MPEG4SetFilenameAndFrameNum(CHAR* pFilename, UINT32 u4FrameNum)
{
     if(pFilename)
     {
         x_strcpy(ucFimeName, pFilename);
     }
     //gu4FrameNum = u4FrameNum;
}
#endif


