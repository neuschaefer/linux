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
 * $RCSfile: vdec_mpeg4psr.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_mpeg4psr.c
 *  This file contains implementation of exported APIs of VDEC.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
#include "vdec_mpeg4psr.h"
#include "vdec_mpeg4dec.h"
#include "vdec_mpeg4hw.h"
#include "fbm_drvif.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_common.h"
#include "x_typedef.h"
#include "x_util.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define u4Div2Slash(v1, v2) (((v1)+(v2)/2)/(v2))

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

#ifdef IC_MPEG4_VERIFICATION
extern UINT32 _u4Dx311PicWidth;
extern UINT32 _u4Dx311PicHeight;
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

// M4V Part
static void _PrsM4vHdr(VDEC_HDLR hDecPrm, INT32* pi4RetValue);
static void _PrsVOLHdr(VDEC_MPEG4_DEC_PARAM_INFO_T * prDecPrm, INT32 *pi4RetValue);
static void _PrsUsrData(VDEC_MPEG4_DEC_PARAM_INFO_T * prDecPrm, INT32 *pi4RetValue);
static void _PrsVOPHdr(VDEC_MPEG4_DEC_PARAM_INFO_T * prDecPrm, INT32* pi4RetValue);
static void _SpriteTrajetory(VDEC_MPEG4_DEC_PARAM_INFO_T * prDecPrm, INT32* pi4RetValue);
static void _LoadM4vQMat(VDEC_MPEG4_DEC_PARAM_INFO_T * prDecPrm, INT32* pi4RetValue, BOOL fgIntra);
static INT32 _WarpingMvCode(UINT32* pu4BitCount);
static UINT32 _DmvCodeLength(UINT32* pBitCount);
static void _CalGmcMv(VDEC_MPEG4_DEC_PARAM_INFO_T * prDecPrm, INT32* pi4RetValue);

// H263 Part
static void _PrsH263Hdr(VDEC_HDLR hDecPrm, INT32* pi4RetValue);
static void _PrsH263PicHdr(VDEC_MPEG4_DEC_PARAM_INFO_T * prDecPrm, INT32 * pi4RetValue);

#ifdef CC_IS_MPEG4_POST_MT5363
// Flv Part
static void _PrsFlvHdr(VDEC_HDLR hDecPrm, INT32* pi4RetValue);
static void _PrsFlvPicHdr(VDEC_MPEG4_DEC_PARAM_INFO_T * prDecPrm, INT32 * pi4RetValue);
#endif

// Divx311 Part
static void _PrsDx311Hdr(VDEC_HDLR hDecPrm, INT32* pi4RetValue);

// Miscs
static UINT32 _GetDivXVersion(UINT8 *u1Buf);
static UINT32 _LsbMask(UINT32 u4ShiftBits);
static void _Align2Byte(UINT32* pu4BitCount);
static BOOL _SrcFmtTbl(VDEC_MPEG4_DEC_PARAM_INFO_T* prDecPrm);
#ifdef CC_IS_MPEG4_POST_MT5363
static BOOL _FlvSrcFmtTbl(VDEC_MPEG4_DEC_PARAM_INFO_T* prDecPrm);
#endif

static void _PrsM4vHdr(VDEC_HDLR hDecPrm, INT32* pi4RetValue)
{
    #ifdef IC_MPEG4_VERIFICATION
    UINT32 u4Bits = 0;
	#endif
	UINT32 u4Temp = 0;
	VDEC_MPEG4_DEC_PARAM_INFO_T *prDecPrm = (VDEC_MPEG4_DEC_PARAM_INFO_T*)hDecPrm;

	*pi4RetValue = E_VDEC_FAIL;

#ifdef IC_MPEG4_VERIFICATION
	while (_VDEC_VldMpeg4RPtr(&u4Bits) <= prDecPrm->rPesInfo.u4FifoEnd)
#endif
	{
		u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
		//LOG(5, "_PrsM4vHdr Start code = %08x\n", u4Temp);
		if ((u4Temp >> 8) == M4V_START_CODE)
		{
		    *pi4RetValue = E_VDEC_PARSE_NOT_READY;
		    if ((u4Temp <= M4V_VOL_START_CODE_MAX) &&
		        u4Temp >= M4V_VOL_START_CODE_MIN)
		    {
		        _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 32);
		        _PrsVOLHdr(prDecPrm, pi4RetValue);
		        _Align2Byte(&prDecPrm->u4BitCount);
		        if (*pi4RetValue == E_VDEC_OK)
		        {
		            *pi4RetValue = E_VDEC_PARSE_NOT_READY;
		        }
#ifdef IC_MPEG4_VERIFICATION
            continue;
#endif
		    }
	        else if (u4Temp == M4V_USER_DATA_START_CODE)
	        {
	            _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 32);
	            _PrsUsrData(prDecPrm, pi4RetValue);
	#ifdef IC_MPEG4_VERIFICATION
	            continue;
	#endif        
	        }
	        else if (u4Temp == M4V_VOP_START_CODE)
	        {
	            _PrsVOPHdr(prDecPrm, pi4RetValue);
	#ifdef IC_MPEG4_VERIFICATION
	            //*pi4RetValue = E_VDEC_OK;
	            break;
	#endif
	        }
		}
	#ifdef IC_MPEG4_VERIFICATION
		u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 8);
	#endif
	}

	
}

static void _PrsVOLHdr(VDEC_MPEG4_DEC_PARAM_INFO_T * prDecPrm, INT32 *pi4RetValue)
{
    UINT8 	i;
    UINT8 	u1FbgType;
    UINT32 	u4Temp;
    UINT32 u4FbWidth, u4FbHeight;
    FBM_SEQ_HDR_T* prFbmSeqHdr = NULL;    
    VDEC_M4V_VOL_INFO_T  *prVolInfo;
    UINT32 u4WorkingSize;
    UINT32 u4WorkingBuffer;
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;

    LOG(5, "Parse VOL header\n");
    
    prDecPrm->u4BitCount = 0;

    *pi4RetValue = E_VDEC_OK;
    prVolInfo = &prDecPrm->rM4vParams.rVol;

    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 9);
    _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    if (u4Temp & 0x80000000) // is_object_layer_identifier
    {
        u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
        prVolInfo->u1VideoObjectLayerVerid = (UINT8)(u4Temp >> 28);
        _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 7);
    }
    else
    {
        prVolInfo->u1VideoObjectLayerVerid = 0x01;
    }

    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0); 
    prVolInfo->u1AspectRatioInfo = (UINT8)(u4Temp >> 28);
    _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 4);
    if (prVolInfo->u1AspectRatioInfo == 0x0f)
    {
        _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 16);
    }

    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0) >> 31;
    _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    if(u4Temp)  // vol control parameters
    {
        u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 3) >> 31;
        _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
        if(u4Temp) // vbv_parameters
        {
            _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 32); // first_half_bit_rate, marker_bit, latter_half_bit_rate, marker_bit
            _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 19); // first_half_vbv_buffer_size, marker_bit, latter_half_vbv_buffer_size
            _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 28); // first_half_vbv_occupancy, marker_bit, latter_half_vbv_occupancy, marker_bit
        }
    }

    if(((_VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0) >> 30) & 0x03) != 0) // Video Object Layer Shape
    {
        *pi4RetValue = E_VDEC_FAIL;
        return;
    }

    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 3);
    prVolInfo->u4TimeIncrementResolution = (u4Temp >> 16) & 0xffff;
    for(i = 15; i > 0; i--)
    {
        if((prVolInfo->u4TimeIncrementResolution - 1) & (0x1 << i))
        {
            break;
        }
    }
    prVolInfo->u1VopTimeIncrementResolutionBits = (UINT8)(i + 1);

    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 17);
    _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    if(u4Temp & 0x80000000) // fixed_vop_rate
    {
        u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
        prVolInfo->u2FixedVopTimeIncrement  = 
        	(UINT16)(u4Temp >> (32 -  prVolInfo->u1VopTimeIncrementResolutionBits));
        _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, prVolInfo->u1VopTimeIncrementResolutionBits);
    }

    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    prDecPrm->u2Width = (UINT16)(u4Temp >> 19);
    prDecPrm->u2Height = (UINT16)((u4Temp >> 5) & 0x1fff);

    LOG(5, "Video Width = 0x%08x Height = 0x%08x\n", prDecPrm->u2Width, prDecPrm->u2Height);


    prDecPrm->u4DramWidth = ((prDecPrm->u2Width % 16) != 0) ? 
    	(UINT16)(((prDecPrm->u2Width / 16) + 1) * 16) : (prDecPrm->u2Width);

    prDecPrm->u4DramHeight = ((prDecPrm->u2Height % 32) != 0) ? 
    	(UINT16)(((prDecPrm->u2Height / 32) + 1) * 32) : (prDecPrm->u2Height);


    if (FBM_ChkFrameBufferFlag(prDecPrm->u1FbgId, (UINT32)FBM_FLAG_RELEASE_FBG))        
    {
        FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, 0);    
        FBM_WaitUnlockFrameBuffer(prDecPrm->u1FbgId, 20);    // block until display unlock                    
        FBM_ReleaseGroup(prDecPrm->u1FbgId);
        prDecPrm->u1FbgId = FBM_FBG_ID_UNKNOWN;
    }


    u1FbgType = FBM_SelectGroupType(prDecPrm->u4DramWidth, prDecPrm->u4DramHeight);

    prVdecEsInfo = _VDEC_GetEsInfo(0);

    if (prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN)
    {
        prDecPrm->u1FbgId = FBM_CreateGroup(u1FbgType, FBM_VDEC_MPEG4, 0, 0);
        FBM_SetPlayMode(prDecPrm->u1FbgId,FBM_FBG_MM_MODE);
        //ASSERT(prDecPrm->u1FbgId != FBM_FBG_ID_UNKNOWN);  
        if (prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN)
        {
            LOG(3,  "prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN\n");
            *pi4RetValue = E_VDEC_FAIL;
            return;
        }
        prVdecEsInfo->ucFbgId = prDecPrm->u1FbgId;
        FBM_SetSyncStc(prDecPrm->u1FbgId, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId);
        	FBM_GetFrameBufferSize(prDecPrm->u1FbgId, &u4FbWidth, &u4FbHeight);
        FBM_FbgChgNotify(prDecPrm->u1FbgId, 0);
         prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prDecPrm->u1FbgId);
         prDecPrm->u4LineSize = u4FbWidth;
         prDecPrm->u1FbgType = u1FbgType;
    }
    else
    {
        if (u1FbgType != prDecPrm->u1FbgType)
        {
            LOG(3, "Fbg Type Change %d\n", u1FbgType);
            FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, 0);

            FBM_WaitUnlockFrameBuffer(prDecPrm->u1FbgId, 
            	20);    // block until display unlock            

            FBM_ReleaseGroup(prDecPrm->u1FbgId);
            prDecPrm->u1FbgId = FBM_FBG_ID_UNKNOWN;         

            prDecPrm->u1FbgId = FBM_CreateGroup(u1FbgType, FBM_VDEC_MPEG4, 0, 0);
            FBM_SetPlayMode(prDecPrm->u1FbgId,FBM_FBG_MM_MODE);
            //ASSERT(prDecPrm->u1FbgId != FBM_FBG_ID_UNKNOWN);  
            if (prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN)
            {
                LOG(3,  "prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN\n");
                *pi4RetValue = E_VDEC_FAIL;
                return;
            }
            prVdecEsInfo->ucFbgId = prDecPrm->u1FbgId;
            FBM_SetSyncStc(prDecPrm->u1FbgId, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId);
            FBM_GetFrameBufferSize(prDecPrm->u1FbgId, &u4FbWidth, &u4FbHeight);
            FBM_FbgChgNotify(prDecPrm->u1FbgId, 0);
            prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prDecPrm->u1FbgId);
            prDecPrm->u4LineSize = u4FbWidth;
        }
    }

    prDecPrm->u4UmvPicW = (UINT32)(((prDecPrm->u2Width + 15) / 16) * 16);
    prDecPrm->u4UmvPicH = (UINT32)(((prDecPrm->u2Height + 15) / 16) * 16);

    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 28);
    prVolInfo->fgInterlaced = (BOOL)(u4Temp >> 31) & 0x1;
    prVolInfo->fgObmcDisable = (BOOL)(u4Temp >> 30) & 0x1;
    _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 2);

    if (prFbmSeqHdr != NULL)
    {
        prFbmSeqHdr->ucActFmt = ACT_FMT_DEFAULT;
        
        if (prVdecEsInfo != 0)
        {
            //prFbmSeqHdr->ucAspRatInf = 1;        // 16:9
            switch (prVdecEsInfo->rMMSeqInfo.e_frame_rate)
            {
                case VDEC_SRC_FRAME_RATE_23_976: case VDEC_SRC_FRAME_RATE_24:
                case VDEC_SRC_FRAME_RATE_25: case VDEC_SRC_FRAME_RATE_29_97:
                case VDEC_SRC_FRAME_RATE_30: case VDEC_SRC_FRAME_RATE_50:
                case VDEC_SRC_FRAME_RATE_59_94: case VDEC_SRC_FRAME_RATE_60:
                    prFbmSeqHdr->ucFrmRatCod = prVdecEsInfo->rMMSeqInfo.e_frame_rate;
                    break;
                default:
                    prFbmSeqHdr->ucFrmRatCod = VDEC_SRC_FRAME_RATE_29_97;
                    LOG(1, "Upper layer did not set the frame rate\n");
                    break;
            }
            prFbmSeqHdr->fgProgressiveSeq = (prVolInfo->fgInterlaced) ? 0 : 1;
#if 0
            if((prDecPrm->u2Width < FBM_IMG_RESIZE_LIMITATION) ||
                (prDecPrm->u2Height < FBM_IMG_RESIZE_LIMITATION))
            {
                prFbmSeqHdr->fgResizeSmallPic = TRUE;

                if (prDecPrm->u2Width < FBM_IMG_RESIZE_LIMITATION)
                {
                    prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                    prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                    prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                }
                else if(prDecPrm->u2Width > FBM_IMG_RESIZE_LIMITATION_MAX)
                {
                    prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                    prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                    prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                }
                else
                {
                    prFbmSeqHdr->u2LineSize = prDecPrm->u4LineSize;
                    prFbmSeqHdr->u2HSize = prDecPrm->u2Width;
                    prFbmSeqHdr->u2OrgHSize = prDecPrm->u2Width;
                }

                if (prDecPrm->u2Height < FBM_IMG_RESIZE_LIMITATION)
                {
                    prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                    prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                    prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                }
                else if(prDecPrm->u2Height > FBM_IMG_RESIZE_LIMITATION_MAX)
                {
                    prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                    prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                    prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                }
                else
                {
                    prFbmSeqHdr->u2VSize = prDecPrm->u2Height;
                    prFbmSeqHdr->u2OrgVSize = prDecPrm->u2Height;
                }
            }
            else
            {
                prFbmSeqHdr->u2HSize = prDecPrm->u2Width;
                prFbmSeqHdr->u2VSize = prDecPrm->u2Height;
                prFbmSeqHdr->u2OrgHSize = prDecPrm->u2Width;
                prFbmSeqHdr->u2OrgVSize = prDecPrm->u2Height;
                prFbmSeqHdr->u2LineSize = (UINT16)prDecPrm->u4LineSize;
            }
#else
            prFbmSeqHdr->u2HSize = prDecPrm->u2Width;
            prFbmSeqHdr->u2VSize = prDecPrm->u2Height;
            prFbmSeqHdr->u2OrgHSize = prDecPrm->u2Width;
            prFbmSeqHdr->u2OrgVSize = prDecPrm->u2Height;
            prFbmSeqHdr->u2LineSize = (UINT16)prDecPrm->u4LineSize;
#endif
        }
        else
        {
            // Did not parse the frame rate from ES so far
            //FBM_SetFrameBufferFlag(prDecPrm->u1FbgId, FBM_FLAG_SEQ_CHG);   
            //prFbmSeqHdr->ucAspRatInf = 1;        // 16:9
            prFbmSeqHdr->ucFrmRatCod = 5;        // 30 fps
            prFbmSeqHdr->fgProgressiveSeq = (prVolInfo->fgInterlaced) ? 0 : 1;
            prFbmSeqHdr->u2HSize = prDecPrm->u2Width;
            prFbmSeqHdr->u2VSize = prDecPrm->u2Height;
            prFbmSeqHdr->u2OrgHSize = prDecPrm->u2Width;
            prFbmSeqHdr->u2OrgVSize = prDecPrm->u2Height;
            prFbmSeqHdr->u2LineSize = (UINT16)prDecPrm->u4LineSize;
            //prFbmSeqHdr->u2LineSize = prDecPrm->u4LineSize;
            //FBM_PicStructChgNotify(0, MPEG_PIC_FRAME);
        }

#ifdef IC_MPEG4_VERIFICATION
    prDecPrm->u4LineSize = prDecPrm->u2Width;
#endif

        LOG(7, "prVolInfo->u1AspectRatioInfo (%X)\n", prVolInfo->u1AspectRatioInfo);
        switch (prVolInfo->u1AspectRatioInfo)
        {
            case 0x1: // 1:1 Square
                prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_1_1;
                break;
            case 0x2: // 12:11 625-type for 4:3 picture
                prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_12_11;
                break;
            case 0x3: // 10:11 525-type for 4:3 picture
                prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_10_11;
                break;                
            case 0x4: // 16:11 625-type stretched for 16:9 picture
                prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_16_11;
                break;
            case 0x5: // 40:33 525-type stretched for 16:9 picture
                prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_40_33;
                break;
            case 0x0: // forbidden
            case 0x6: // Reserved
            case 0x7: // Reserved
            case 0x8: // Reserved
            case 0x9: // Reserved
            case 0xA: // Reserved
            case 0xB: // Reserved
            case 0xC: // Reserved
            case 0xD: // Reserved
            case 0xE: // Reserved
            case 0xF: // extended PAR
                prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_1_1;
                break;
            default:
                prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_4_3;
                break;
        }
    }


#ifdef USE_FBM_BUF
    FBM_GetWorkingBuffer(prDecPrm->u1FbgId, &u4WorkingBuffer, &u4WorkingSize);    

    prDecPrm->u4DCACBufAddr = u4WorkingBuffer;
    prDecPrm->u4MVECBufAddr = u4WorkingBuffer + MP4_DCAC_SZ;
    prDecPrm->u4BMB1BufAddr = prDecPrm->u4MVECBufAddr + MP4_MVEC_SZ;
    prDecPrm->u4BMB2BufAddr = prDecPrm->u4BMB1BufAddr + MP4_BMB1_SZ;
    prDecPrm->u4BCODEBufAddr = prDecPrm->u4BMB2BufAddr + MP4_BMB2_SZ; 
#else 
    if (prDecPrm->u4DCACBufAddr == 0)
    {
    	prDecPrm->u4DCACBufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_DCAC_SZ, 16);
    }
    
    if (prDecPrm->u4MVECBufAddr == 0)
    {
    	prDecPrm->u4MVECBufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_MVEC_SZ, 16);
    }
    
    if (prDecPrm->u4BMB1BufAddr == 0)
    {
    	prDecPrm->u4BMB1BufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_BMB1_SZ, 16);
    }
    
    if (prDecPrm->u4BMB2BufAddr == 0)
    {
    	prDecPrm->u4BMB2BufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_BMB2_SZ, 16);
    }
    
    if (prDecPrm->u4BCODEBufAddr == 0)
    {
    	prDecPrm->u4BCODEBufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_BCODE_SZ, 16);
    }
#endif
	  
    if(prVolInfo->u1VideoObjectLayerVerid == 1)
    {
        u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
        prVolInfo->u1SpriteEnable = (UINT8)(u4Temp >> 31) & 0x1;
        _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    }
    else
    {
        u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
        prVolInfo->u1SpriteEnable = (UINT8)(u4Temp >> 30) & 0x3;
        _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 2);
    }

    if(prVolInfo->u1SpriteEnable != SPRITE_NOT_USED && prVolInfo->u1SpriteEnable != SPRITE_GMC)
    {
        *pi4RetValue = E_VDEC_FAIL;
        return;
    }

    if(prVolInfo->u1SpriteEnable == SPRITE_GMC)
    {
        u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
        prVolInfo->u1NoOfSpriteWarpingPoints = (UINT8)(u4Temp >> 26) & 0x3f;
        prVolInfo->u1SpriteWarpingAccuracy = (UINT8)(u4Temp >> 24) & 0x03;
        if((u4Temp >> 22) & 0x01) // Sprite Brightness Change
        {
            *pi4RetValue = E_VDEC_FAIL;
            return;
        }
        _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 9);
    }

    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
    if (u4Temp >> 31)   // not_8_bit, we just support 8-bits pixel
    {
        *pi4RetValue = E_VDEC_FAIL;
        return;
    }

    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    prVolInfo->u1QuantType = (UINT8)(u4Temp >> 31);
    _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    if (prVolInfo->u1QuantType)
    {
        u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
        prVolInfo->fgLoadIntraQuantMat = (UINT8)(u4Temp >> 31) & 0x01;
        _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
        if (prVolInfo->fgLoadIntraQuantMat)
        {
            _LoadM4vQMat(prDecPrm, pi4RetValue, TRUE);
            if(*pi4RetValue == E_VDEC_FAIL)
            {
            	return;
            }
        }
        u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
        prVolInfo->fgLoadNonIntraQuantMat = u4Temp >> 31;
        _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
        if(prVolInfo->fgLoadNonIntraQuantMat)
        {
          	_LoadM4vQMat(prDecPrm, pi4RetValue, FALSE);
          	if(*pi4RetValue == E_VDEC_FAIL)
          	{
            	return;
          	}
        }
    }
	  
    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
    if (prVolInfo->u1VideoObjectLayerVerid != 0x01)
    {
        prVolInfo->fgQuarterSample = u4Temp >> 31;
        _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    }

    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
    _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    if (!(u4Temp >> 31)) // Complexity Estimation Disable
    {
        *pi4RetValue = E_VDEC_FAIL;
        return;
    }

    prVolInfo->fgResyncMarkerDisable = (BOOL)(u4Temp >> 30) & 0x01;
    prVolInfo->fgDataPartitioned = (BOOL)(u4Temp >> 29) & 0x01;
    _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 2);
    if (prVolInfo->fgDataPartitioned)
    {
        prVolInfo->fgReversibleVlc = (u4Temp >> 28) & 0x01;
        _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    }

    u4Temp =  _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
    _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    if (prVolInfo->u1VideoObjectLayerVerid != 0x01)
    {
        if (u4Temp >> 31) // newpred_enable
        {
             *pi4RetValue = E_VDEC_FAIL;
             return;
        }

        prVolInfo->fgReducedResolutionVopEnable = (u4Temp >> 30) & 0x01;
        _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    }
}

static void _PrsUsrData(VDEC_MPEG4_DEC_PARAM_INFO_T * prDecPrm, INT32 *pi4RetValue)
{
	UINT8	u1aBuf[64];
	UINT32 	u4BufPos = 0, u4Temp, i;
	VDEC_M4V_VOL_INFO_T  *prVolInfo;

	prVolInfo = &prDecPrm->rM4vParams.rVol;

	u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
	while((u4Temp >> 8) != M4V_START_CODE && u4BufPos < 64)
	{
		u1aBuf[u4BufPos++] = (UINT8)(u4Temp >> 24);
		_VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 8);
		u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
	}

	// Mainly parse user data for DIVX
	if(x_strncmp((const CHAR*)u1aBuf, "DivX", 4))
	{
		//*pi4RetValue = E_VDEC_FAIL;
		return;
	}
	else 
	{
		prDecPrm->u1VideoType = VIDEO_TYPE_DIVX_MPEG4;
	}

	prVolInfo->u4DivxCodecVersion = _GetDivXVersion(&u1aBuf[4]);

	if (!x_strncmp((const CHAR*)&u1aBuf[7], "Build", 5))
	{
		prVolInfo->u4UserDataBuildNumber = _GetDivXVersion(&u1aBuf[12]);
		return;
	}

	for(i = 0; i < u4BufPos; i++)
  	{
    		if(u1aBuf[i] == 'b')
    		{
      			prVolInfo->u4UserDataBuildNumber = _GetDivXVersion(&u1aBuf[i+1]);
      			break;
    		}
  	}
}

static void _LoadM4vQMat(VDEC_MPEG4_DEC_PARAM_INFO_T * prDecPrm, INT32* pi4RetValue
							, BOOL fgIntra)
{
    // Should use software to load Q Matrix
    // instead of hardware
#if 0
    UINT32	u4OldAddr, u4Bit, u4OldBitCount;
    UINT32 	u4QLength;
    UINT32	i, u4Idx = 0;
    UINT8 u1QMat[64];
    UINT32 u4QMat[16];
    UINT32 u4Val;

    while ((VLD_READ32(0xF4) & 0x20000) != 0x20000)
    {
    };


    u4OldAddr = _VDEC_VldMpeg4RPtr(&u4Bit);

    // Trigger Hardware to load Q Matrix
    if (fgIntra) 
    {
        VLD_READ32(RW_VLD_TABLIM);
    }
    else
    {
        VLD_READ32(RW_VLD_TABLNIM);
    }

    u4OldBitCount = prDecPrm->u4BitCount;

    while ((VLD_READ32(0xF4) & 0x20000) != 0x20000)
    {
    };

    /* 040329: Workaround for HW bug:
             If quantization matrix is smaller than 64 bytes,
             HW may load more than necessary bytes,
             causing successive vparser reading to be wrong.
             To cover it, after loading a matrix,
             set vld rd ptr back to the matrix,
             and shift the correct byte number by FW.
             One issue is not covered yet:
             The remaining values should be equal to the value before the 0,
             but if HW loads the values after the 0, it will use those values.
             To really solve this issue we have to prepare a 64-byte buffer with
             correct trailing values for HW to load. */
    if(!_VDEC_VLDInitMpeg4BarrelShifter(u4OldAddr, (VDEC_HDLR)prDecPrm))
    {
    	*pi4RetValue = E_VDEC_FAIL;
    	return;
    }

    _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, u4Bit);

    prDecPrm->u4BitCount = u4OldBitCount;
#endif 
    UINT32 	u4QLength;
    UINT32	i, u4Idx = 0;
    UINT8 u1QMat[64];
    UINT32 u4Val;

    u4QLength = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
    for(i = 0; i < 64; i++)
    {	
    	u1QMat[i]= (UINT8)(u4QLength >> 24);
    	if((*((UINT8 *)&(u4QLength) + 3)) == 0)
    	{
    		_VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 8);
    		break;
    	}
    	_VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 8);
    	u4QLength = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
    }
    
    if (i < 64)
    {
        if (i >= 1)
        {
            UINT8 u1Temp = u1QMat[i - 1];
            for (;i < 64; i++)
            {
                u1QMat[i] = u1Temp;
            }
        }
    }
    
    if (fgIntra)
    {
        for (u4Idx = 0; u4Idx <= 15; u4Idx++)
        {
            u4Val = (UINT32)(VLD_WRITE_QMATRIX |(u4Idx << 2));
            VLD_WRITE32(RW_VLD_RISC_QMATRIX, u4Val);
            VLD_WRITE32(RW_VLD_QMATRIX_RW, (UINT32)((u1QMat[u4Idx*4] << 24) |
                          (u1QMat[u4Idx*4 + 1] << 16) |
                          (u1QMat[u4Idx*4 + 2] << 8) |
                          (u1QMat[u4Idx*4 + 3])));
        }

    }
    else
    {
        for (u4Idx = 0; u4Idx <= 15; u4Idx++)
        {
            u4Val = (UINT32)(VLD_WRITE_QMATRIX | VLD_NONINTRA |(u4Idx << 2));
            VLD_WRITE32(RW_VLD_RISC_QMATRIX, u4Val);
            VLD_WRITE32(RW_VLD_QMATRIX_RW, (UINT32)((u1QMat[u4Idx*4] << 24) |
                          (u1QMat[u4Idx*4 + 1] << 16) |
                          (u1QMat[u4Idx*4 + 2] << 8) |
                          (u1QMat[u4Idx*4 + 3])));
        }
    }

    *pi4RetValue = E_VDEC_OK;

}

static void _PrsVOPHdr(VDEC_MPEG4_DEC_PARAM_INFO_T * prDecPrm, INT32* pi4RetValue)
{
    UCHAR ucBRefFbId;
    UCHAR ucFRefFbId;              

    UINT32 u4Temp;
    FBM_SEQ_HDR_T* prFbmSeqHdr = NULL;    
    FBM_PIC_HDR_T * prFbmPicHdr;
    VDEC_M4V_VOL_INFO_T 	*prVolPrm;
    VDEC_M4V_PIC_INFO_T 	*prVopPrm;
    VDEC_M4V_GMC_INFO_T 	*prGmcPrm;

    VDEC_ES_INFO_T *prVdecEsInfo = NULL;

    prVdecEsInfo = _VDEC_GetEsInfo(0);
    if ((prVdecEsInfo != NULL) &&
        (prVdecEsInfo->fgOnlyGotSeqHeader))
    {
        LOG(5, "Get Seq Header Only, Don't need to parse pic\n");
        *pi4RetValue = E_VDEC_OK;
        return;
    }

    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prDecPrm->u1FbgId);
    if (!prFbmSeqHdr)
    {
        LOG(5, "FBM sequece header is null");
        *pi4RetValue = E_VDEC_FAIL;
        return;
    }

	
    prVolPrm = &prDecPrm->rM4vParams.rVol;
    prVopPrm = &prDecPrm->rM4vParams.rVop;
    prGmcPrm = &prVopPrm->rGmcParam;
    
    LOG(7, "This is the %ld frame\n", prDecPrm->u4FrameCount);
    prDecPrm->u4FrameCount++;
    
    u4Temp = VLD_READ32(0xF0);
    LOG(7, "Parser Input Window: 0x%8x\n", u4Temp);
    
    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 3);
    prDecPrm->u1VopCodingType = (UINT8)((u4Temp >> 1) & 0x03); // Vop Coding Type

    *pi4RetValue = E_VDEC_OK;

    if (prDecPrm->fgDecError == TRUE && 
        prDecPrm->u1VopCodingType != VOP_CODING_TYPE_I)
    {
        *pi4RetValue = E_VDEC_FAIL;
        return;
    }
    else 
    {
        prDecPrm->fgDecError = FALSE;
    }
     
    if (prDecPrm->u1VopCodingType != VOP_CODING_TYPE_B)
    {
        prVopPrm->u4RefPicTimeBase = prVopPrm->u4TimeBase;
        prVopPrm->u4CurDispTime = prVopPrm->u4TimeBase;
        prDecPrm->u1BackwardRefVopType = prDecPrm->u1VopCodingType;
    }
    else
    {
        prVopPrm->u4CurDispTime = prVopPrm->u4RefPicTimeBase;
    }

    while (u4Temp & 0x01)
    {
        if (prDecPrm->u1VopCodingType != VOP_CODING_TYPE_B)
        {
            prVopPrm->u4TimeBase++;
        }
        prVopPrm->u4CurDispTime++;
        
        u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    } 
    
    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, (UINT32)(prVolPrm->u1VopTimeIncrementResolutionBits+1));
    prVopPrm->u2VopTimeIncrement = 
    	(UINT16)(u4Temp & _LsbMask(prVolPrm->u1VopTimeIncrementResolutionBits)); 

    LOG(6, "Video coding type = %d Time Increment = %d\n", prDecPrm->u1VopCodingType, prVopPrm->u2VopTimeIncrement);
    
    prVopPrm->u4CurDispTime = (prVopPrm->u4CurDispTime * prVolPrm->u4TimeIncrementResolution)
    						+ prVopPrm->u2VopTimeIncrement;
    
    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 2);
    if ((u4Temp & 0x01) == 0) // vop is not coded
    {
        LOG(6, "Not coded video frame\n");
        //prDecPrm->fgVopNotCoded = TRUE;
        prDecPrm->u4RefPts = prDecPrm->rPesInfo.u4PTS;
        prDecPrm->u8RecOffset = prDecPrm->rPesInfo.u8Offset;
        prDecPrm->u4VopNotCoded++;
        if(prDecPrm->rPesInfo.u2SkipVidCount > 0)
        {
            if ((prDecPrm->u8PtsScale != 0) &&
                        (prDecPrm->u4Rate != 0))
            {
                UINT64 u8Numerator = 0, u8Denominator = 0;

                u8Numerator = prDecPrm->u8PtsScale + prDecPrm->u8Remainder;
                u8Denominator = (UINT64)prDecPrm->u4Rate;

                prDecPrm->u4DispPts += 
                    u8Div6432(u8Numerator, u8Denominator, 
                    &prDecPrm->u8Remainder)*
                    (prDecPrm->rPesInfo.u2SkipVidCount);     
            }
        }
        *pi4RetValue = E_VDEC_NOT_CODED_FRAME;
        return;
    }
    
    if (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_P || 
       (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_S && 
       prVolPrm->u1SpriteEnable == SPRITE_GMC))	
    {
    	prDecPrm->u1VopRoundingType =  (UINT8)_VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1) & 0x01;
    	LOG(7, "Rounding Type = %ld\n", prDecPrm->u1VopRoundingType);
    }
    else
    {
    	prDecPrm->u1VopRoundingType = 0;
    	LOG(7, "Rounding Type = 0\n");
    }
    
    if (prVolPrm->fgReducedResolutionVopEnable && 
    	(prDecPrm->u1VopCodingType == VOP_CODING_TYPE_I || 
       	 prDecPrm->u1VopCodingType == VOP_CODING_TYPE_P))
    {
    	prVopPrm->fgVopReducedResolution = (BOOL)(_VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1) & 0x01);
    }
    
    prVopPrm->u1IntraDcVlcThr = (UINT8)(_VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 3) & 0x07);
    
    if (prVolPrm->fgInterlaced)
    {
        u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 2);
        prVopPrm->fgTopFldFirst = (u4Temp >> 1) & 0x01;
        if (prVopPrm->fgTopFldFirst)
        {
            prDecPrm->u1PicStructure = MP4_TOP_FLD_PIC;
        }
        else
        {
            prDecPrm->u1PicStructure = MP4_BTM_FLD_PIC;
        }

        prVopPrm->fgAlternateVerticalScanFlag = u4Temp & 0x01;
        if ((prVolPrm->fgInterlaced) &&
            (!prVopPrm->fgAlternateVerticalScanFlag))
        {
            prFbmSeqHdr->fgProgressiveSeq = TRUE;
        }
    }
    else
    {
    	prDecPrm->u1PicStructure = MP4_FRM_PIC;
    }
    
    if ((prVolPrm->u1SpriteEnable == SPRITE_STATIC ||prVolPrm->u1SpriteEnable == SPRITE_GMC) &&
    	prDecPrm->u1VopCodingType == VOP_CODING_TYPE_S)
    {
    	if (prVolPrm->u1NoOfSpriteWarpingPoints > 0)
    	{
            LOG(3, "Error The num of Warpping points is larger than 1\n");
            _SpriteTrajetory(prDecPrm, pi4RetValue);
            if (*pi4RetValue != E_VDEC_OK)
            {
                prDecPrm->fgDecError = TRUE;
                return;
            }
    	}
    }
    else
    {
    	prVopPrm->u1EffectiveWarpingPoints = 0;
    }
    
    // Process GMC Parameters
    if(prVopPrm->u1EffectiveWarpingPoints == 0)
    	{
    		prGmcPrm->i4GmcYMvX = 0;
    		prGmcPrm->i4GmcYMvY = 0;
    		prGmcPrm->i4GmcCMvX = 0;
    		prGmcPrm->i4GmcCMvY = 0;
    	}
    	else
    	{
    		_CalGmcMv(prDecPrm, pi4RetValue);
    	}  
    
    prDecPrm->u1VopQuant = (UINT8)(_VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, prVolPrm->u1QuantPrecision) & 0x1f);
    if(prDecPrm->u1VopCodingType != VOP_CODING_TYPE_I)
    {
    	prVopPrm->u1ForwardFCode = (UINT8)(_VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 3) & 0x07);
    }
    if(prDecPrm->u1VopCodingType == VOP_CODING_TYPE_B)
    {
    	prVopPrm->u1BackwardFCode = (UINT8)(_VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 3) & 0x07);
    }
    _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 32);

    if (prDecPrm->u1FbgId != FBM_FBG_ID_UNKNOWN)
    {
        // Check if reset is required
        if (FBM_ChkFrameBufferFlag(prDecPrm->u1FbgId, (UINT32)FBM_FLAG_RESET))
        {
            FBM_ResetGroup(prDecPrm->u1FbgId);
            FBM_ClrFrameBufferFlag(prDecPrm->u1FbgId, (UINT32)FBM_FLAG_RESET);
            FBM_ClrFrameBufferFlag(prDecPrm->u1FbgId, (UINT32)FBM_FLAG_NOT_READY);
//            FBM_SetFrameBufferFlag(prDecPrm->u1FbgId, FBM_FLAG_CLEAR_FRC_PRM);

            /*if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC))
            {
                _MPV_SkipPic(*pucMpvId, prVdecEsInfo->u4EsId);
                *pucMpvId = VDEC_MAX_VLD;
            }*/                        
        }
    }
    else
    {
        LOG(3, "_PrsVOPHdr FBM group is not created\n");
        *pi4RetValue = E_VDEC_FAIL;
        return;
    }

    if (prDecPrm->u1VopCodingType != VOP_CODING_TYPE_B)
    {
        BOOL fgNotDisplay;

        VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(0);

        FBM_GetRefFrameBuffer(prDecPrm->u1FbgId, &ucFRefFbId, &ucBRefFbId);

        LOG(6, "ucFRefFbId = 0x%08x ucBRefFbId = 0x%08x\n", ucFRefFbId, ucBRefFbId);

        if (ucBRefFbId != FBM_FBG_ID_UNKNOWN)
        {
            prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucBRefFbId);
            if (prFbmPicHdr)
            {
                if (prDecPrm->fgEnCalPts)
                {
                    if (prDecPrm->fgValidPts)
                    {
                        LOG(7, "Valid PTS\n");
                        prFbmPicHdr->u4PTS = prDecPrm->u4RefPts;
                        prDecPrm->u4DispPts = prDecPrm->u4RefPts;
                         prDecPrm->u4VopNotCoded = 0;
                        prDecPrm->fgValidPts = FALSE;
                    }
                    else
                    {
                        prFbmPicHdr->u4PTS = prDecPrm->u4DispPts;
                    }

                    LOG(7, "Enable PTS calculation Cur PTS = 0x%08x\n", prFbmPicHdr->u4PTS);
                    if ((prDecPrm->u8PtsScale != 0) &&
                        (prDecPrm->u4Rate != 0)&&(!prDecPrm->rPesInfo.fgNoVdecCalPts))
                    {
                        UINT64 u8Numerator = 0, u8Denominator = 0;

                        u8Numerator = prDecPrm->u8PtsScale + prDecPrm->u8Remainder;
                        u8Denominator = (UINT64)prDecPrm->u4Rate;

                        prDecPrm->u4DispPts += 
                            u8Div6432(u8Numerator, u8Denominator, 
                            &prDecPrm->u8Remainder)*
                            (prDecPrm->rPesInfo.u2SkipVidCount + 1 +  prDecPrm->u4VopNotCoded);
                       
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
                            u8Div6432(u8Numerator, u8Denominator, &prDecPrm->u8Remainder);
                        LOG(7, "Use ES PTS = 0x%08x\n", prFbmPicHdr->u4PTS);
                    }
                }
                
                prFbmPicHdr->u2DecodingOrder = prVdecEsInfo->u2DecodingOrder;
                prFbmPicHdr->u4TotlaTimeOffset = prVdecEsInfo->u4TotlaTimeOffset;

                prFbmPicHdr->u8Offset = prDecPrm->u8RecOffset;
                prFbmPicHdr->u8OffsetDisp = prVdecEsInfo->u8OffsetLast;
                prFbmPicHdr->u8OffsetI = prDecPrm->u8RecOffsetI;
                prFbmPicHdr->fgTopFldFirst = prVopPrm->fgTopFldFirst;
                prFbmPicHdr->ucTrueZeroPTS = FBM_USE_ZERO_PTS_IF_ZERO;
                prFbmPicHdr->i4TemporalRef = prDecPrm->i4TemporalRef;

                prDecPrm->i4TemporalRef++;

#if 0
                prFbmPicHdr->u4PicWidth = prDecPrm->u2Width;
                prFbmPicHdr->u4PicHeight = prDecPrm->u2Height;
                prFbmPicHdr->u4PicWidthPitch = prDecPrm->u4LineSize;
#endif

                fgNotDisplay = VDEC_IsNotDisplay(0, prFbmPicHdr->u4PTS,
                    prFbmPicHdr->i4TemporalRef, prFbmPicHdr->u8Offset);
                if (fgNotDisplay)
                {
                    /*FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, 
                        ucBRefFbId, 
                        FBM_MM_NOT_DISPLAY_FLAG);

                    LOG(6, "_PrsVOPHdr fgNotDisplay ucFbId %d\n", ucBRefFbId);*/
                    UCHAR ucFbStatus;
                    ucFbStatus = FBM_GetFrameBufferStatus(prDecPrm->u1FbgId, ucBRefFbId);
                    if ((ucFbStatus != FBM_FB_STATUS_DISPLAYQ) ||
                        (ucFbStatus != FBM_FB_STATUS_LOCK))
                    {
                        FBM_SetFrameBufferStatus(prDecPrm->u1FbgId,
                           ucBRefFbId, FBM_FB_STATUS_EMPTY);
                    }
                    else
                    {
                        LOG(6, "Frame buffer status = %x\n", ucFbStatus);
                    }
                }
                else
                {
                    //Printf("prFbmPicHdr->u4PTS = 0x%08x\n", prFbmPicHdr->u4PTS);
                    //Printf("ucFRefFbId = 0x%08x\n", ucFRefFbId);
                    //Printf("ucBRefFbId = 0x%08x\n", ucBRefFbId);
                    UCHAR ucFbStatus;
                    ucFbStatus = FBM_GetFrameBufferStatus(prDecPrm->u1FbgId, ucBRefFbId);
                    if ((ucFbStatus == FBM_FB_STATUS_DECODE) || (ucFbStatus == FBM_FB_STATUS_FIELD_READY) ||
			      (ucFbStatus == FBM_FB_STATUS_READY))
                    {
                        LOG(5, "I/P Pic Temporal Ref = %d\n", prFbmPicHdr->i4TemporalRef);
                        FBM_SetFrameBufferStatus(prDecPrm->u1FbgId,
                               ucBRefFbId, FBM_FB_STATUS_DISPLAYQ);

                        prVdecEsInfo->u4DisplayQPicCnt++;
                    }
                    else
                    {
                        LOG(6, "Frame buffer status = %x\n", ucFbStatus);
                    }
                }
                VDEC_CheckStopStatus(0, prFbmPicHdr->u4PTS, prFbmPicHdr->i4TemporalRef);
            }
            else
            {
                LOG(3, "_PrsVOPHdr prFbmPicHdr == NULL\n");
                *pi4RetValue = E_VDEC_FAIL;
                return;
            }
        }
        // FBM error
        else if (prDecPrm->u1VopCodingType != VOP_CODING_TYPE_I)
        {
            LOG(3, "Wrong reference for P VOP\n");
            *pi4RetValue = E_VDEC_FAIL;
            return;
        }
        
        prDecPrm->fgValidPts = prDecPrm->rPesInfo.fgDtsValid;
        prDecPrm->u4RefPts = prDecPrm->rPesInfo.u4PTS;
        prDecPrm->u8RecOffset = prDecPrm->rPesInfo.u8Offset;
        prDecPrm->u8RecOffsetI = prDecPrm->rPesInfo.u8OffsetI;
        prDecPrm->u1FbId = FBM_GetEmptyRefFrameBuffer(prDecPrm->u1FbgId, MPEG4_WAIT_DISP_TIME);

        prVopPrm->u4PrevDispTime = prVopPrm->u4NextDispTime;
        prVopPrm->u4NextDispTime = prVopPrm->u4CurDispTime;
        prVopPrm->rDirectMd.u4Trd = prVopPrm->u4NextDispTime - prVopPrm->u4PrevDispTime;
    }
    else
    {
        FBM_GetRefFrameBuffer(prDecPrm->u1FbgId, &ucFRefFbId, &ucBRefFbId);

        if (ucFRefFbId == FBM_FBG_ID_UNKNOWN ||
            ucBRefFbId == FBM_FBG_ID_UNKNOWN)
        {
            LOG(3, "Wrong reference for B VOP\n");
            *pi4RetValue = E_VDEC_FAIL;
            return;
        }

        prDecPrm->u1FbId = FBM_GetEmptyBFrameBuffer(prDecPrm->u1FbgId, MPEG4_WAIT_DISP_TIME);
        prVopPrm->rDirectMd.u4Trb = prVopPrm->u4CurDispTime- prVopPrm->u4PrevDispTime;
    
        if (prVolPrm->fgInterlaced)
        {
            if (prVopPrm->rDirectMd.u4TFrm == (UINT32)0xffffffff)
            {
                prVopPrm->rDirectMd.u4TFrm = prVopPrm->u4NextDispTime - prVopPrm->u4CurDispTime;
            }
        	
            u4Temp = u4Div2Slash(prVopPrm->u4PrevDispTime, prVopPrm->rDirectMd.u4TFrm);
            		prVopPrm->rDirectMd.u4Trbi = 2 * (u4Div2Slash(prVopPrm->u4CurDispTime
            									, prVopPrm->rDirectMd.u4TFrm) - u4Temp);
            		prVopPrm->rDirectMd.u4Trdi = 2 * (u4Div2Slash(prVopPrm->u4NextDispTime
            									, prVopPrm->rDirectMd.u4TFrm) - u4Temp);
        }
    }

    if (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_I)
    {
        prDecPrm->i4TemporalRef = 0;
    }

    prVopPrm->u2VopPrevTimeIncrement = prVopPrm->u2VopTimeIncrement;
}

static void _SpriteTrajetory(VDEC_MPEG4_DEC_PARAM_INFO_T * prDecPrm, INT32* pi4RetValue)
{
    BOOL	fgBadDivx;
    INT32	i;
    VDEC_M4V_VOL_INFO_T *prVolPrm;
    VDEC_M4V_PIC_INFO_T *prVopPrm;

    prVolPrm = &prDecPrm->rM4vParams.rVol;
    prVopPrm = &prDecPrm->rM4vParams.rVop;

    fgBadDivx = ((prVolPrm->u4DivxCodecVersion == 500) ||
                    (prVolPrm->u4DivxCodecVersion == 501)) &&
                   (prVolPrm->u4UserDataBuildNumber >= 370) &&
                   (prVolPrm->u4UserDataBuildNumber <= 413);

    for(i = 0; (i < prVolPrm->u1NoOfSpriteWarpingPoints) && (i < 4); i++)
    {
        prVopPrm->ppi4WarpingMv[i][0] = _WarpingMvCode(&prDecPrm->u4BitCount);
        if(!fgBadDivx)
        {
            _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);  // marker_bit
        }
        prVopPrm->ppi4WarpingMv[i][1] = _WarpingMvCode(&prDecPrm->u4BitCount);
        _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);  // marker_bit
    }

    prVopPrm->u1EffectiveWarpingPoints= prVolPrm->u1NoOfSpriteWarpingPoints;
    while ((prVopPrm->u1EffectiveWarpingPoints > 0) &&
         (prVopPrm->u1EffectiveWarpingPoints <= 4) &&
         (prVopPrm->ppi4WarpingMv[prVopPrm->u1EffectiveWarpingPoints - 1][0] == 0) &&
         (prVopPrm->ppi4WarpingMv[prVopPrm->u1EffectiveWarpingPoints - 1][1] == 0))
    {
        prVopPrm->u1EffectiveWarpingPoints--;
    }

    if(prVopPrm->u1EffectiveWarpingPoints > 1)
    {
        *pi4RetValue = E_VDEC_FAIL;
        return;
    }

    *pi4RetValue = E_VDEC_OK;
}

static INT32 _WarpingMvCode(UINT32 *pu4BitCount)
{
	UINT32 	u4Length;
	INT32 	i4Code;

	u4Length = _DmvCodeLength(pu4BitCount);

	if(u4Length == 0)
	{
		return 0;
	}
	if(_VDEC_Mpeg4GetBitStream(pu4BitCount, 1) & 0x1)
	{
		i4Code = (INT32)(1 << (u4Length - 1));
	}
	else
	{
		i4Code = (INT32)(-(1 << u4Length) + 1);
	}
	
	i4Code += (INT32)(_VDEC_Mpeg4GetBitStream(pu4BitCount, u4Length - 1) 
				& _LsbMask(u4Length - 1));

	return i4Code;

}

static UINT32 _DmvCodeLength(UINT32* pi4BitCount)
{
	UINT32 u4DmvLengthMsb;
	UINT32 u4DmvCodeLength;

	u4DmvLengthMsb = _VDEC_Mpeg4GetBitStream(pi4BitCount, 2) & 0x03;
	switch(u4DmvLengthMsb)
	{
		case 0:
	  		u4DmvCodeLength = 0;
	  		break;
		case 1:
			u4DmvCodeLength = 1 + (_VDEC_Mpeg4GetBitStream(pi4BitCount, 1) & 0x01);
			break;
		case 2:
			u4DmvCodeLength = 3 + (_VDEC_Mpeg4GetBitStream(pi4BitCount, 1) & 0x01);
	  		break;
		case 3:
		default:
	  		u4DmvCodeLength = 3;
	  		while(_VDEC_Mpeg4GetBitStream(pi4BitCount, 1) & 0x1)
	  		{
	    			u4DmvCodeLength++;
	  		}
	  		u4DmvCodeLength += 2;
	  		break;
	}

	return u4DmvCodeLength;

}

static void _CalGmcMv(VDEC_MPEG4_DEC_PARAM_INFO_T * prDecPrm, INT32* pi4RetValue)
{
	BOOL	fgBadDivx = FALSE;
	INT32 	i4X, i4Y;
	INT32 	i4SpriteEnable;
	INT32 	i0_pla, j0_pla;

	VDEC_M4V_VOL_INFO_T *prVolPrm;
	VDEC_M4V_PIC_INFO_T *prVopPrm;
	VDEC_M4V_GMC_INFO_T *prGmcPrm;

	prVolPrm = &prDecPrm->rM4vParams.rVol;
	prVopPrm = &prDecPrm->rM4vParams.rVop;
	prGmcPrm = &prVopPrm->rGmcParam;
	
	fgBadDivx = ((prVolPrm->u4DivxCodecVersion == 500) ||
                    (prVolPrm->u4DivxCodecVersion == 501)) &&
                   (prVolPrm->u4UserDataBuildNumber >= 370) &&
                   (prVolPrm->u4UserDataBuildNumber <= 413);

	i4SpriteEnable = 2 << prDecPrm->rM4vParams.rVol.u1SpriteEnable;

	if (fgBadDivx)
	{
		i4X = prVopPrm->ppi4WarpingMv[0][0];
		i4Y = prVopPrm->ppi4WarpingMv[0][1];
	}
	else
	{
		i4X = prVopPrm->ppi4WarpingMv[0][0] * i4SpriteEnable / 2;
		i4Y = prVopPrm->ppi4WarpingMv[0][1] * i4SpriteEnable / 2;
	}

	i0_pla = i4X * 4 / i4SpriteEnable;  /* multiply by two to get q-pel precision */
	j0_pla = i4Y * 4 / i4SpriteEnable;  /* multiply by two to get q-pel precision */
	prGmcPrm->i4GmcYMvX = i0_pla;
	prGmcPrm->i4GmcYMvY = j0_pla;

	i0_pla = (i4X >> 1) | (i4X & 0x01);
	i0_pla = i0_pla * 4 / i4SpriteEnable;   /* multiply by two to get q-pel precision */
	j0_pla = (i4Y >> 1) | (i4Y & 0x01);
	j0_pla = j0_pla * 4 / i4SpriteEnable;   /* multiply by two to get q-pel precision */
	prGmcPrm->i4GmcCMvX = i0_pla;
	prGmcPrm->i4GmcCMvY = j0_pla;

}

static void _PrsH263Hdr(VDEC_HDLR hDecPrm, INT32* pi4RetValue)
{
    UINT32 u4Temp;
    VDEC_MPEG4_DEC_PARAM_INFO_T *prDecPrm = (VDEC_MPEG4_DEC_PARAM_INFO_T*)hDecPrm;

    *pi4RetValue = E_VDEC_OK;


    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);

#if 0
    while ((u4Temp & H263_VIDEO_START_MASK) != H263_VIDEO_START_CODE)
    {
        u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 8);
    }
#endif 	

    if ((u4Temp & H263_VIDEO_START_MASK) == H263_VIDEO_START_CODE)
    {
        _PrsH263PicHdr(prDecPrm, pi4RetValue);
    }
}

// Not Finished
static void _PrsH263PicHdr(VDEC_MPEG4_DEC_PARAM_INFO_T * prDecPrm, INT32 * pi4RetValue)
{
    BOOL fgNotDisplay;
    UCHAR ucBRefFbId;
    UCHAR ucFRefFbId;                
    UINT8   u1FbgType;
    UINT32 u4Temp;
    VDEC_M4V_VOL_INFO_T	*prVolPrm;
    VDEC_M4V_PIC_INFO_T 	*prVopPrm;
    FBM_SEQ_HDR_T* prFbmSeqHdr = NULL;    
    FBM_PIC_HDR_T * prFbmPicHdr = NULL;
    UINT32 u4FbWidth, u4FbHeight;
    UINT32 u4WorkingSize;
    UINT32 u4WorkingBuffer;

    VDEC_ES_INFO_T *prVdecEsInfo = NULL;


    prVopPrm = &prDecPrm->rM4vParams.rVop;
    prVolPrm  = &prDecPrm->rM4vParams.rVol;

    prVolPrm->fgShortHeader = TRUE;
    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 18);
    prVopPrm->u1SplitScreenIndicator = (UINT8)((u4Temp >> 17) & 0x01);
    prVopPrm->u1DocCameraIndicator = (UINT8)((u4Temp >> 16) & 0x01);
    prVopPrm->u1SourceFormat = (UINT8)((u4Temp >> 12) & 0x07);
    prDecPrm->u1VopCodingType = (UINT8)((u4Temp >> 11) & 0x01);
    prDecPrm->u1VopQuant = (UINT8)((u4Temp >> 2) &0x1F);

    if (!_SrcFmtTbl(prDecPrm))
    {
    	*pi4RetValue = E_VDEC_FAIL;
    	return;
    }

    if (prDecPrm->u4FrameCount == 0)
    {
        prVdecEsInfo = _VDEC_GetEsInfo(0);

        if (FBM_ChkFrameBufferFlag(prDecPrm->u1FbgId, (UINT32)FBM_FLAG_RELEASE_FBG))        
        {
            FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, 0);    
            FBM_WaitUnlockFrameBuffer(prDecPrm->u1FbgId, 20);    // block until display unlock                    
            FBM_ReleaseGroup(prDecPrm->u1FbgId);
            prDecPrm->u1FbgId = FBM_FBG_ID_UNKNOWN;
        }


        if (prVdecEsInfo->fgMMSeqInfoValid == TRUE)
        {
            prDecPrm->u2Width = prVdecEsInfo->rMMSeqInfo.u2_width;
            prDecPrm->u2Height = prVdecEsInfo->rMMSeqInfo.u2_height;
        }
        else
        {
            if (prDecPrm->u2Width == 0 || prDecPrm->u2Height == 0)
            {
                *pi4RetValue =  E_VDEC_FAIL;
                return;
            }
        }

        u1FbgType = FBM_SelectGroupType(prDecPrm->u2Width, prDecPrm->u2Height);
        if (prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN)
        {
            prDecPrm->u1FbgId = FBM_CreateGroup(u1FbgType, FBM_VDEC_MPEG4, 0, 0);
            FBM_SetPlayMode(prDecPrm->u1FbgId,FBM_FBG_MM_MODE);
            //ASSERT(prDecPrm->u1FbgId != FBM_FBG_ID_UNKNOWN);  
            if (prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN)
            {
                LOG(3,  "prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN\n");
                *pi4RetValue = E_VDEC_FAIL;
                return;
            }

            prVdecEsInfo->ucFbgId = prDecPrm->u1FbgId;
            FBM_SetSyncStc(prDecPrm->u1FbgId, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId);
            FBM_GetFrameBufferSize(prDecPrm->u1FbgId, &u4FbWidth, &u4FbHeight);
            FBM_FbgChgNotify(prDecPrm->u1FbgId, 0);
            prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prDecPrm->u1FbgId);
            prDecPrm->u4LineSize = u4FbWidth;
            prDecPrm->u1FbgType = u1FbgType;
        }
        else
        {
            if (u1FbgType != prDecPrm->u1FbgType)
            {
                LOG(3, "Fbg Type Change %d\n", u1FbgType);
                FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, 0);

                FBM_WaitUnlockFrameBuffer(prDecPrm->u1FbgId, 
                	20);    // block until display unlock            

                FBM_ReleaseGroup(prDecPrm->u1FbgId);
                prDecPrm->u1FbgId = FBM_FBG_ID_UNKNOWN;         

                prDecPrm->u1FbgId = FBM_CreateGroup(u1FbgType, FBM_VDEC_MPEG4, 0, 0);
                FBM_SetPlayMode(prDecPrm->u1FbgId,FBM_FBG_MM_MODE);
                //ASSERT(prDecPrm->u1FbgId != FBM_FBG_ID_UNKNOWN);  
                if (prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN)
                {
                    LOG(3,  "prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN\n");
                    *pi4RetValue = E_VDEC_FAIL;
                    return;
                }
                prVdecEsInfo->ucFbgId = prDecPrm->u1FbgId;
                FBM_SetSyncStc(prDecPrm->u1FbgId, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId);
                FBM_GetFrameBufferSize(prDecPrm->u1FbgId, &u4FbWidth, &u4FbHeight);
                FBM_FbgChgNotify(prDecPrm->u1FbgId, 0);
                prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prDecPrm->u1FbgId);
                prDecPrm->u4LineSize = u4FbWidth;
            }
        }

        if (prFbmSeqHdr != NULL)
        {
            prFbmSeqHdr->ucActFmt = ACT_FMT_DEFAULT;
            prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_4_3;        // 16:9
            
            if (prVdecEsInfo != 0)
            {
                switch (prVdecEsInfo->rMMSeqInfo.e_frame_rate)
                {
                    case VDEC_SRC_FRAME_RATE_23_976: case VDEC_SRC_FRAME_RATE_24:
                    case VDEC_SRC_FRAME_RATE_25: case VDEC_SRC_FRAME_RATE_29_97:
                    case VDEC_SRC_FRAME_RATE_30: case VDEC_SRC_FRAME_RATE_50:
                    case VDEC_SRC_FRAME_RATE_59_94: case VDEC_SRC_FRAME_RATE_60:
                        prFbmSeqHdr->ucFrmRatCod = prVdecEsInfo->rMMSeqInfo.e_frame_rate;
                        break;
                    default:
                        prFbmSeqHdr->ucFrmRatCod = VDEC_SRC_FRAME_RATE_29_97;
                        LOG(1, "Upper layer did not set the frame rate\n");
                        break;
                }
                prFbmSeqHdr->fgProgressiveSeq = 1;
                prFbmSeqHdr->u2HSize = prDecPrm->u2Width;
                prFbmSeqHdr->u2VSize = prDecPrm->u2Height;
                prFbmSeqHdr->u2OrgHSize = prDecPrm->u2Width;
                prFbmSeqHdr->u2OrgVSize = prDecPrm->u2Height;
                prFbmSeqHdr->u2LineSize = (UINT16)prDecPrm->u4LineSize;
            }
            else
            {
                    // Did not parse the frame rate from ES so far
            	  //FBM_SetFrameBufferFlag(prDecPrm->u1FbgId, FBM_FLAG_SEQ_CHG);   
              	  prFbmSeqHdr->ucFrmRatCod = 5;        // 30 fps
            	  prFbmSeqHdr->fgProgressiveSeq = 1;
            	  prFbmSeqHdr->u2HSize = prDecPrm->u2Width;
            	  prFbmSeqHdr->u2VSize = prDecPrm->u2Height;
            	  prFbmSeqHdr->u2OrgHSize = prDecPrm->u2Width;
            	  prFbmSeqHdr->u2OrgVSize = prDecPrm->u2Height;
            	  prFbmSeqHdr->u2LineSize = (UINT16)prDecPrm->u4LineSize;
            	  //prFbmSeqHdr->u2LineSize = prDecPrm->u4LineSize;
            	  //FBM_PicStructChgNotify(0, MPEG_PIC_FRAME);
            }
        }
#ifdef IC_MPEG4_VERIFICATION
        prDecPrm->u4LineSize = prDecPrm->u2Width;
#endif
    }

    if (prDecPrm->u1FbgId != FBM_FBG_ID_UNKNOWN)
    {
        // Check if reset is required
        if (FBM_ChkFrameBufferFlag(prDecPrm->u1FbgId, (UINT32)FBM_FLAG_RESET))
        {
            FBM_ResetGroup(prDecPrm->u1FbgId);
            FBM_ClrFrameBufferFlag(prDecPrm->u1FbgId, (UINT32)FBM_FLAG_RESET);
            FBM_ClrFrameBufferFlag(prDecPrm->u1FbgId, (UINT32)FBM_FLAG_NOT_READY);
    //            FBM_SetFrameBufferFlag(prDecPrm->u1FbgId, FBM_FLAG_CLEAR_FRC_PRM);

            /*if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC))
            {
                _MPV_SkipPic(*pucMpvId, prVdecEsInfo->u4EsId);
                *pucMpvId = VDEC_MAX_VLD;
            }*/                        
        }
    }


    FBM_GetRefFrameBuffer(prDecPrm->u1FbgId, &ucFRefFbId, &ucBRefFbId);

    // Video's PTS
    if (ucBRefFbId != FBM_FBG_ID_UNKNOWN)
    {
        prVdecEsInfo = _VDEC_GetEsInfo(0);
        if (!prVdecEsInfo)
        {
            *pi4RetValue = E_VDEC_FAIL;
            return;
        }

        prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucBRefFbId);
        if (prFbmPicHdr)
        {
            prFbmPicHdr->u4PTS = prDecPrm->u4RefPts;
        }
        else
        {
            LOG(3, "_PrsVOPHdr prFbmPicHdr == NULL\n");
            *pi4RetValue = E_VDEC_FAIL;
            return;
        }

        fgNotDisplay = VDEC_IsNotDisplay(0, prDecPrm->u4RefPts, 0, 0);
        if (fgNotDisplay)
        {
            /*FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, 
                ucBRefFbId, 
                FBM_MM_NOT_DISPLAY_FLAG);

            LOG(6, "_PrsVOPHdr fgNotDisplay ucFbId %d\n", ucBRefFbId);*/
            UCHAR ucFbStatus;
            ucFbStatus = FBM_GetFrameBufferStatus(prDecPrm->u1FbgId, ucBRefFbId);
            if ((ucFbStatus != FBM_FB_STATUS_DISPLAYQ) ||
                (ucFbStatus != FBM_FB_STATUS_LOCK))
            {
                FBM_SetFrameBufferStatus(prDecPrm->u1FbgId,
                   ucBRefFbId, FBM_FB_STATUS_EMPTY);
            }
            else
            {
                LOG(6, "Frame buffer status = %x\n", ucFbStatus);
            }
        }
        else
        {
            LOG(6, "_PrsH263Hdr prFbmPicHdr->u4PTS = %ld\n", prFbmPicHdr->u4PTS);

            UCHAR ucFbStatus;
            ucFbStatus = FBM_GetFrameBufferStatus(prDecPrm->u1FbgId, ucBRefFbId);
             if ((ucFbStatus == FBM_FB_STATUS_DECODE) || (ucFbStatus == FBM_FB_STATUS_FIELD_READY) ||
			 (ucFbStatus == FBM_FB_STATUS_READY))
            {
                FBM_SetFrameBufferStatus(prDecPrm->u1FbgId,
                       ucBRefFbId, FBM_FB_STATUS_DISPLAYQ);

                prVdecEsInfo->u4DisplayQPicCnt++;
            }
            else
            {
                LOG(6, "Frame buffer status = %x\n", ucFbStatus);
            }
        }
        UNUSED(VDEC_CheckStopStatus(0, prDecPrm->u4RefPts, 0));
    }
    // FBM error
    else if (prDecPrm->u1VopCodingType != VOP_CODING_TYPE_I)
    {
        LOG(3, "Wrong reference for P VOP\n");
        *pi4RetValue = E_VDEC_FAIL;
        return;
    }

    prDecPrm->u4RefPts = prDecPrm->rPesInfo.u4PTS;
    prDecPrm->fgValidPts = prDecPrm->rPesInfo.fgDtsValid;

    LOG(6, "ucFRefFbId = 0x%08x\n", ucFRefFbId);
    LOG(6, "ucBRefFbId = 0x%08x\n", ucBRefFbId);

    LOG(6, "_PrsH263PicHdr prDecPrm->u4RefPts = %ld\n", prDecPrm->u4RefPts);

    prDecPrm->u1FbId = FBM_GetEmptyRefFrameBuffer(prDecPrm->u1FbgId, MPEG4_WAIT_DISP_TIME);

#ifdef USE_FBM_BUF
    FBM_GetWorkingBuffer(prDecPrm->u1FbgId, &u4WorkingBuffer, &u4WorkingSize);    

    prDecPrm->u4DCACBufAddr = u4WorkingBuffer;
    prDecPrm->u4MVECBufAddr = u4WorkingBuffer + MP4_DCAC_SZ;
    prDecPrm->u4BMB1BufAddr = prDecPrm->u4MVECBufAddr + MP4_MVEC_SZ;
    prDecPrm->u4BMB2BufAddr = prDecPrm->u4BMB1BufAddr + MP4_BMB1_SZ;
    prDecPrm->u4BCODEBufAddr = prDecPrm->u4BMB2BufAddr + MP4_BMB2_SZ; 
#else 
    if (prDecPrm->u4DCACBufAddr == 0)
    {
    	prDecPrm->u4DCACBufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_DCAC_SZ, 16);
    }
    
    if (prDecPrm->u4MVECBufAddr == 0)
    {
    	prDecPrm->u4MVECBufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_MVEC_SZ, 16);
    }
    
    if (prDecPrm->u4BMB1BufAddr == 0)
    {
    	prDecPrm->u4BMB1BufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_BMB1_SZ, 16);
    }
    
    if (prDecPrm->u4BMB2BufAddr == 0)
    {
    	prDecPrm->u4BMB2BufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_BMB2_SZ, 16);
    }
    
    if (prDecPrm->u4BCODEBufAddr == 0)
    {
    	prDecPrm->u4BCODEBufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_BCODE_SZ, 16);
    }
#endif

    while(u4Temp & 0x01)  // pei
    {
    	_VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 9);
    	u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
    }

    _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 32);

    // Set some default values for short headers
    prDecPrm->u1VopRoundingType = 0;

    prVolPrm->u1QuantType = FALSE;
    prVolPrm->fgObmcDisable = TRUE;
    prVolPrm->fgResyncMarkerDisable = TRUE;
    prVolPrm->fgDataPartitioned = FALSE;
    prVolPrm->fgReversibleVlc = FALSE;
    prVolPrm->fgInterlaced = FALSE;
    prVopPrm->fgTopFldFirst = FALSE;
    prVopPrm->u1ForwardFCode = 1;
    prDecPrm->u1PicStructure = MP4_FRM_PIC;

    /*if (prDecPrm->rPesInfo.fgEos)
    {
        FBM_SetFrameBufferPicFlag(prDecPrm->u1FbgId, prDecPrm->u1FbId, FBM_MM_EOS_FLAG);   
    }*/
}

#ifdef CC_IS_MPEG4_POST_MT5363
static void _PrsFlvHdr(VDEC_HDLR hDecPrm, INT32* pi4RetValue)
{
	UINT32 u4Temp;
	VDEC_MPEG4_DEC_PARAM_INFO_T *prDecPrm = (VDEC_MPEG4_DEC_PARAM_INFO_T*)hDecPrm;

	*pi4RetValue = E_VDEC_OK;

	u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);

	while ((u4Temp & H263_VIDEO_START_MASK) != FLV_VIDEO_START_CODE)
	{
		u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 8);
	}

	if ((u4Temp & H263_VIDEO_START_MASK) == FLV_VIDEO_START_CODE)
	{
		_PrsFlvPicHdr(prDecPrm, pi4RetValue);
	}
	
}

// Not Finished
static void _PrsFlvPicHdr(VDEC_MPEG4_DEC_PARAM_INFO_T * prDecPrm, INT32 * pi4RetValue)
{
    //BOOL fgNotDisplay;
    UCHAR ucBRefFbId;
    UCHAR ucFRefFbId;                
    UINT8   u1FbgType;
    UINT32 u4Temp;
    VDEC_M4V_VOL_INFO_T	*prVolPrm;
    VDEC_M4V_PIC_INFO_T 	*prVopPrm;
    FBM_SEQ_HDR_T* prFbmSeqHdr = NULL;    
    FBM_PIC_HDR_T * prFbmPicHdr = NULL;
    UINT32 u4FbWidth, u4FbHeight;
    UINT32 u4WorkingSize;
    UINT32 u4WorkingBuffer;

    VDEC_ES_INFO_T *prVdecEsInfo = NULL;


    prVopPrm = &prDecPrm->rM4vParams.rVop;
    prVolPrm  = &prDecPrm->rM4vParams.rVol;

    prVolPrm->fgShortHeader = TRUE;
    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    prVopPrm->u1SourceFormat = (UINT8)(u4Temp & 0x07);

    if (!_FlvSrcFmtTbl(prDecPrm))
    {
        //Printf("format is not support\n");
    	*pi4RetValue = E_VDEC_FAIL;
    	return;
    }

    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 9);

    prDecPrm->u1VopCodingType = (UINT8)((u4Temp >> 7) & 0x03);
    prDecPrm->u1VopQuant = (UINT8)((u4Temp >> 1) &0x1F);

    if (prDecPrm->u4FrameCount == 0)
    {
        prVdecEsInfo = _VDEC_GetEsInfo(0);

        if (FBM_ChkFrameBufferFlag(prDecPrm->u1FbgId, (UINT32)FBM_FLAG_RELEASE_FBG))        
        {
            FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, 0);    
            FBM_WaitUnlockFrameBuffer(prDecPrm->u1FbgId, 20);    // block until display unlock                    
            FBM_ReleaseGroup(prDecPrm->u1FbgId);
            prDecPrm->u1FbgId = FBM_FBG_ID_UNKNOWN;
        }

/*
        if (prVdecEsInfo->fgMMSeqInfoValid == TRUE)
        {
            prDecPrm->u2Width = prVdecEsInfo->rMMSeqInfo.u2_width;
            prDecPrm->u2Height = prVdecEsInfo->rMMSeqInfo.u2_height;
        }
        else
        {
            if (prDecPrm->u2Width == 0 || prDecPrm->u2Height == 0)
            {
                *pi4RetValue =  E_VDEC_FAIL;
                return;
            }
        }
*/
        prDecPrm->u4DramWidth = ((prDecPrm->u2Width % 16) != 0) ? 
    	    (UINT16)(((prDecPrm->u2Width / 16) + 1) * 16) : (prDecPrm->u2Width);

        prDecPrm->u4DramHeight = ((prDecPrm->u2Height % 32) != 0) ? 
    	    (UINT16)(((prDecPrm->u2Height / 32) + 1) * 32) : (prDecPrm->u2Height);


        u1FbgType = FBM_SelectGroupType(prDecPrm->u2Width, prDecPrm->u2Height);
        if (prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN)
        {
            prDecPrm->u1FbgId = FBM_CreateGroup(u1FbgType, FBM_VDEC_MPEG4, 0, 0);
            FBM_SetPlayMode(prDecPrm->u1FbgId,FBM_FBG_MM_MODE);
            //ASSERT(prDecPrm->u1FbgId != FBM_FBG_ID_UNKNOWN);  
            if (prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN)
            {
                LOG(3,  "prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN\n");
                *pi4RetValue = E_VDEC_FAIL;
                return;
            }

            prVdecEsInfo->ucFbgId = prDecPrm->u1FbgId;
            FBM_SetSyncStc(prDecPrm->u1FbgId, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId);
            FBM_GetFrameBufferSize(prDecPrm->u1FbgId, &u4FbWidth, &u4FbHeight);
            FBM_FbgChgNotify(prDecPrm->u1FbgId, 0);
            prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prDecPrm->u1FbgId);
            prDecPrm->u4LineSize = u4FbWidth;
            prDecPrm->u1FbgType = u1FbgType;

#ifdef CC_MPEG4_DEBLOCKING
            FBM_SetFrameBufferFlag(prDecPrm->u1FbgId, FBM_FLAG_DEBLOCKING);
            LOG(5, "DBK Enable\n");
#endif
        }
        else
        {
            if (u1FbgType != prDecPrm->u1FbgType)
            {
                LOG(3, "Fbg Type Change %d\n", u1FbgType);
                FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, 0);

                FBM_WaitUnlockFrameBuffer(prDecPrm->u1FbgId, 
                	20);    // block until display unlock            

                FBM_ReleaseGroup(prDecPrm->u1FbgId);
                prDecPrm->u1FbgId = FBM_FBG_ID_UNKNOWN;         

                prDecPrm->u1FbgId = FBM_CreateGroup(u1FbgType, FBM_VDEC_MPEG4, 0, 0);
                FBM_SetPlayMode(prDecPrm->u1FbgId,FBM_FBG_MM_MODE);
                //ASSERT(prDecPrm->u1FbgId != FBM_FBG_ID_UNKNOWN);  
                if (prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN)
                {
                    LOG(3,  "prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN\n");
                    *pi4RetValue = E_VDEC_FAIL;
                    return;
                }
                prVdecEsInfo->ucFbgId = prDecPrm->u1FbgId;
                FBM_SetSyncStc(prDecPrm->u1FbgId, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId);
                FBM_GetFrameBufferSize(prDecPrm->u1FbgId, &u4FbWidth, &u4FbHeight);
                FBM_FbgChgNotify(prDecPrm->u1FbgId, 0);
                prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prDecPrm->u1FbgId);
                prDecPrm->u4LineSize = u4FbWidth;
#ifdef CC_MPEG4_DEBLOCKING
                FBM_SetFrameBufferFlag(prDecPrm->u1FbgId, FBM_FLAG_DEBLOCKING);
                LOG(5, "DBK Enable\n");
#endif
            }
        }

        if (prFbmSeqHdr != NULL)
        {
            if (prVdecEsInfo != 0)
            {
                prFbmSeqHdr->ucAspRatInf = 1;        // 16:9
                switch (prVdecEsInfo->rMMSeqInfo.e_frame_rate)
                {
                    case VDEC_SRC_FRAME_RATE_23_976: case VDEC_SRC_FRAME_RATE_24:
                    case VDEC_SRC_FRAME_RATE_25: case VDEC_SRC_FRAME_RATE_29_97:
                    case VDEC_SRC_FRAME_RATE_30: case VDEC_SRC_FRAME_RATE_50:
                    case VDEC_SRC_FRAME_RATE_59_94: case VDEC_SRC_FRAME_RATE_60:
                        prFbmSeqHdr->ucFrmRatCod = prVdecEsInfo->rMMSeqInfo.e_frame_rate;
                        break;
                    default:
                        prFbmSeqHdr->ucFrmRatCod = VDEC_SRC_FRAME_RATE_29_97;
                        LOG(1, "Upper layer did not set the frame rate\n");
                        break;
                }
                prFbmSeqHdr->fgProgressiveSeq = 1;
                prFbmSeqHdr->u2HSize = prDecPrm->u2Width;
                prFbmSeqHdr->u2VSize = prDecPrm->u2Height;
                prFbmSeqHdr->u2OrgHSize = prDecPrm->u2Width;
                prFbmSeqHdr->u2OrgVSize = prDecPrm->u2Height;
                prFbmSeqHdr->u2LineSize = (UINT16)prDecPrm->u4LineSize;
            }
            else
            {
                    // Did not parse the frame rate from ES so far
            	  //FBM_SetFrameBufferFlag(prDecPrm->u1FbgId, FBM_FLAG_SEQ_CHG);   
              	  prFbmSeqHdr->ucAspRatInf = 1;        // 16:9
              	  prFbmSeqHdr->ucFrmRatCod = 5;        // 30 fps
            	  prFbmSeqHdr->fgProgressiveSeq = 1;
            	  prFbmSeqHdr->u2HSize = prDecPrm->u2Width;
            	  prFbmSeqHdr->u2VSize = prDecPrm->u2Height;
            	  prFbmSeqHdr->u2OrgHSize = prDecPrm->u2Width;
            	  prFbmSeqHdr->u2OrgVSize = prDecPrm->u2Height;
            	  prFbmSeqHdr->u2LineSize = (UINT16)prDecPrm->u4LineSize;
            	  //prFbmSeqHdr->u2LineSize = prDecPrm->u4LineSize;
            	  //FBM_PicStructChgNotify(0, MPEG_PIC_FRAME);
            }
        }
    }

    if (prDecPrm->u1FbgId != FBM_FBG_ID_UNKNOWN)
    {
        // Check if reset is required
        if (FBM_ChkFrameBufferFlag(prDecPrm->u1FbgId, (UINT32)FBM_FLAG_RESET))
        {
            FBM_ResetGroup(prDecPrm->u1FbgId);
            FBM_ClrFrameBufferFlag(prDecPrm->u1FbgId, (UINT32)FBM_FLAG_RESET);
            FBM_ClrFrameBufferFlag(prDecPrm->u1FbgId, (UINT32)FBM_FLAG_NOT_READY);
    //            FBM_SetFrameBufferFlag(prDecPrm->u1FbgId, FBM_FLAG_CLEAR_FRC_PRM);

            /*if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC))
            {
                _MPV_SkipPic(*pucMpvId, prVdecEsInfo->u4EsId);
                *pucMpvId = VDEC_MAX_VLD;
            }*/                        
        }
    }


    FBM_GetRefFrameBuffer(prDecPrm->u1FbgId, &ucFRefFbId, &ucBRefFbId);

    // Video's PTS
    if (ucBRefFbId != FBM_FBG_ID_UNKNOWN)
    {
        prVdecEsInfo = _VDEC_GetEsInfo(0);
        if (!prVdecEsInfo)
        {
            *pi4RetValue = E_VDEC_FAIL;
            return;
        }

        prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucBRefFbId);
        if (prFbmPicHdr)
        {
            prFbmPicHdr->u4PTS = prDecPrm->u4RefPts;
        }
        else
        {
            LOG(3, "_PrsVOPHdr prFbmPicHdr == NULL\n");
            *pi4RetValue = E_VDEC_FAIL;
            return;
        }

#if 0
        fgNotDisplay = VDEC_IsNotDisplay(0, prDecPrm->u4RefPts, 0);
        if (fgNotDisplay)
        {
            /*FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, 
                ucBRefFbId, 
                FBM_MM_NOT_DISPLAY_FLAG);

            LOG(6, "_PrsVOPHdr fgNotDisplay ucFbId %d\n", ucBRefFbId);*/
            FBM_SetFrameBufferStatus(prDecPrm->u1FbgId,
    		ucBRefFbId, FBM_FB_STATUS_EMPTY);

        }
        else
#endif
        {
            LOG(6, "_PrsH263Hdr prFbmPicHdr->u4PTS = %ld\n", prFbmPicHdr->u4PTS);

#ifdef CC_MPEG4_DEBLOCKING
            FBM_SetFrameBufferStatus(prDecPrm->u1FbgId,
                                prDecPrm->u1DbkFbId, FBM_FB_STATUS_DISPLAYQ);
#else
            FBM_SetFrameBufferStatus(prDecPrm->u1FbgId,
            							ucBRefFbId, FBM_FB_STATUS_DISPLAYQ);
#endif
            //Printf("_PrsH263Hdr prFbmPicHdr->u4PTS = %ld\n", prFbmPicHdr->u4PTS);
            prVdecEsInfo->u4DisplayQPicCnt++;            
            VDEC_CheckStopStatus(0, prDecPrm->u4RefPts, 0);
        }
    }
    // FBM error
    else if (prDecPrm->u1VopCodingType != VOP_CODING_TYPE_I)
    {
        LOG(3, "Wrong reference for P VOP\n");
        *pi4RetValue = E_VDEC_FAIL;
        return;
    }

    prDecPrm->u4RefPts = prDecPrm->rPesInfo.u4PTS;

    LOG(6, "ucFRefFbId = 0x%08x\n", ucFRefFbId);
    LOG(6, "ucBRefFbId = 0x%08x\n", ucBRefFbId);

    LOG(6, "_PrsH263PicHdr prDecPrm->u4RefPts = %ld\n", prDecPrm->u4RefPts);

    prDecPrm->u1FbId = FBM_GetEmptyRefFrameBuffer(prDecPrm->u1FbgId, MPEG4_WAIT_DISP_TIME);
#ifdef CC_MPEG4_DEBLOCKING
    prDecPrm->u1DbkFbId = FBM_GetDbkEmptyRefFrameBuffer(prDecPrm->u1FbgId, MPEG4_WAIT_DISP_TIME);
#endif

#ifdef USE_FBM_BUF
    FBM_GetWorkingBuffer(prDecPrm->u1FbgId, &u4WorkingBuffer, &u4WorkingSize);    

    prDecPrm->u4DCACBufAddr = u4WorkingBuffer;
    prDecPrm->u4MVECBufAddr = u4WorkingBuffer + MP4_DCAC_SZ;
    prDecPrm->u4BMB1BufAddr = prDecPrm->u4MVECBufAddr + MP4_MVEC_SZ;
    prDecPrm->u4BMB2BufAddr = prDecPrm->u4BMB1BufAddr + MP4_BMB1_SZ;
    prDecPrm->u4BCODEBufAddr = prDecPrm->u4BMB2BufAddr + MP4_BMB2_SZ; 
#else 
    if (prDecPrm->u4DCACBufAddr == 0)
    {
    	prDecPrm->u4DCACBufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_DCAC_SZ, 16);
    }
    
    if (prDecPrm->u4MVECBufAddr == 0)
    {
    	prDecPrm->u4MVECBufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_MVEC_SZ, 16);
    }
    
    if (prDecPrm->u4BMB1BufAddr == 0)
    {
    	prDecPrm->u4BMB1BufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_BMB1_SZ, 16);
    }
    
    if (prDecPrm->u4BMB2BufAddr == 0)
    {
    	prDecPrm->u4BMB2BufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_BMB2_SZ, 16);
    }
    
    if (prDecPrm->u4BCODEBufAddr == 0)
    {
    	prDecPrm->u4BCODEBufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_BCODE_SZ, 16);
    }
#endif

    while(u4Temp & 0x01)  // pei
    {
    	_VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 9);
    	u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
    }

    _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 32);

    // Set some default values for short headers
    prDecPrm->u1VopRoundingType = 0;

    prVolPrm->u1QuantType = FALSE;
    prVolPrm->fgObmcDisable = TRUE;
    prVolPrm->fgResyncMarkerDisable = TRUE;
    prVolPrm->fgDataPartitioned = FALSE;
    prVolPrm->fgReversibleVlc = FALSE;
    prVolPrm->fgInterlaced = FALSE;
    prVopPrm->fgTopFldFirst = FALSE;
    prVopPrm->u1ForwardFCode = 1;
    prDecPrm->u1PicStructure = MP4_FRM_PIC;
    prDecPrm->fgValidPts = prDecPrm->rPesInfo.fgDtsValid;
    prDecPrm->u4FrameCount++;
    /*if (prDecPrm->rPesInfo.fgEos)
    {
        FBM_SetFrameBufferPicFlag(prDecPrm->u1FbgId, prDecPrm->u1FbId, FBM_MM_EOS_FLAG);   
    }*/
}
#endif

static void _PrsDx311Hdr(VDEC_HDLR hDecPrm, INT32* pi4RetValue)
{
    BOOL fgNotDisplay;
    UCHAR ucBRefFbId;
    UCHAR ucFRefFbId;                
    UINT8   u1FbgType;
    UINT32 u4Temp, u4RetValue;
    UINT32 u4FbWidth, u4FbHeight;
    VDEC_MPEG4_DEC_PARAM_INFO_T *prDecPrm = (VDEC_MPEG4_DEC_PARAM_INFO_T*)hDecPrm;
    FBM_SEQ_HDR_T* prFbmSeqHdr = NULL; 
    FBM_PIC_HDR_T * prFbmPicHdr = NULL;
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;

    UINT32 u4WorkingSize;
    UINT32 u4WorkingBuffer;

    prVdecEsInfo = _VDEC_GetEsInfo(0);    
    if(!prVdecEsInfo)
    {
        LOG(1, "prVdecEsInfo null\n");
        return;
    }
    
    #ifdef IC_MPEG4_VERIFICATION
    prDecPrm->u2Width = (UINT16)_u4Dx311PicWidth;
    prDecPrm->u2Height = (UINT16)_u4Dx311PicHeight;
    #endif
    
    prDecPrm->u1PicStructure = MP4_FRM_PIC;
    prDecPrm->u4FrameCount++;

    // skip Compression and SizeImage
    //_VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 64); 

    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);
    u4RetValue = u4Temp >> 30;
    prDecPrm->u1VopCodingType = (UINT8)(u4RetValue & 0x1);
    // Temporally mark
    if(u4RetValue & 0x2)
    {
    	//prDecPrm->fgVopNotCoded = TRUE;
       prDecPrm->u4RefPts = prDecPrm->rPesInfo.u4PTS;
    	prDecPrm->u8RecOffset = prDecPrm->rPesInfo.u8Offset;
    	*pi4RetValue = E_VDEC_NOT_CODED_FRAME;
    	return;
    }

    prDecPrm->u4DramWidth = ((prDecPrm->u2Width % 16) != 0) ? 
      	(UINT16)(((prDecPrm->u2Width / 16) + 1) * 16) : (prDecPrm->u2Width);

    prDecPrm->u4DramHeight = ((prDecPrm->u2Height % 32) != 0) ? 
      	(UINT16)(((prDecPrm->u2Height / 32) + 1) * 32) : (prDecPrm->u2Height);

    if (prDecPrm->u4FrameCount == 1)
    {
        if (FBM_ChkFrameBufferFlag(prDecPrm->u1FbgId, (UINT32)FBM_FLAG_RELEASE_FBG))        
        {
            FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, 0);    
            FBM_WaitUnlockFrameBuffer(prDecPrm->u1FbgId, 20);    // block until display unlock                    
            FBM_ReleaseGroup(prDecPrm->u1FbgId);
            prDecPrm->u1FbgId = FBM_FBG_ID_UNKNOWN;
        }

        if (prVdecEsInfo->fgMMSeqInfoValid == TRUE)
        {
            prDecPrm->u2Width = prVdecEsInfo->rMMSeqInfo.u2_width;
            prDecPrm->u2Height = prVdecEsInfo->rMMSeqInfo.u2_height;
        }
        else
        {
            if (prDecPrm->u2Width == 0 || prDecPrm->u2Height == 0)
            {
                *pi4RetValue =  E_VDEC_FAIL;
                return;
            }
        }

        u1FbgType = FBM_SelectGroupType(prDecPrm->u2Width, prDecPrm->u2Height);
        if (prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN)
        {
            prDecPrm->u1FbgId = FBM_CreateGroup(u1FbgType, FBM_VDEC_MPEG4, 0, 0);
            FBM_SetPlayMode(prDecPrm->u1FbgId,FBM_FBG_MM_MODE);
            //ASSERT(prDecPrm->u1FbgId != FBM_FBG_ID_UNKNOWN);  
            if (prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN)
            {
                LOG(3,  "prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN\n");
                *pi4RetValue = E_VDEC_FAIL;
                return;
            }
            prVdecEsInfo->ucFbgId = prDecPrm->u1FbgId;
            FBM_SetSyncStc(prDecPrm->u1FbgId, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId);
            FBM_GetFrameBufferSize(prDecPrm->u1FbgId, &u4FbWidth, &u4FbHeight);
            FBM_FbgChgNotify(prDecPrm->u1FbgId, 0);
            prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prDecPrm->u1FbgId);
            prDecPrm->u4LineSize = u4FbWidth;
            prDecPrm->u1FbgType = u1FbgType;
        }
        else
        {
            if (u1FbgType != prDecPrm->u1FbgType)
            {
                LOG(3, "Fbg Type Change %d\n", u1FbgType);
                FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, 0);

                FBM_WaitUnlockFrameBuffer(prDecPrm->u1FbgId, 
                	20);    // block until display unlock            

                FBM_ReleaseGroup(prDecPrm->u1FbgId);
                prDecPrm->u1FbgId = FBM_FBG_ID_UNKNOWN;         

                prDecPrm->u1FbgId = FBM_CreateGroup(u1FbgType, FBM_VDEC_MPEG4, 0, 0);
                FBM_SetPlayMode(prDecPrm->u1FbgId,FBM_FBG_MM_MODE);
                //ASSERT(prDecPrm->u1FbgId != FBM_FBG_ID_UNKNOWN);  
                if (prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN)
                {
                    LOG(3,  "prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN\n");
                    *pi4RetValue = E_VDEC_FAIL;
                    return;
                }
                prVdecEsInfo->ucFbgId = prDecPrm->u1FbgId;
                FBM_SetSyncStc(prDecPrm->u1FbgId, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId);
                FBM_GetFrameBufferSize(prDecPrm->u1FbgId, &u4FbWidth, &u4FbHeight);
                FBM_FbgChgNotify(prDecPrm->u1FbgId, 0);
                prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prDecPrm->u1FbgId);
                prDecPrm->u4LineSize = u4FbWidth;
             }
        }

        if (prFbmSeqHdr != NULL)
        {
           prFbmSeqHdr->ucActFmt = ACT_FMT_DEFAULT;
           prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_4_3;        // 16:9
           
            if (prVdecEsInfo != 0)
            {
                switch (prVdecEsInfo->rMMSeqInfo.e_frame_rate)
                {
                    case VDEC_SRC_FRAME_RATE_23_976: case VDEC_SRC_FRAME_RATE_24:
                    case VDEC_SRC_FRAME_RATE_25: case VDEC_SRC_FRAME_RATE_29_97:
                    case VDEC_SRC_FRAME_RATE_30: case VDEC_SRC_FRAME_RATE_50:
                    case VDEC_SRC_FRAME_RATE_59_94: case VDEC_SRC_FRAME_RATE_60:
                        prFbmSeqHdr->ucFrmRatCod = prVdecEsInfo->rMMSeqInfo.e_frame_rate;
                        break;
                    default:
                        prFbmSeqHdr->ucFrmRatCod = VDEC_SRC_FRAME_RATE_29_97;
                        LOG(1, "Upper layer did not set the frame rate\n");
                        break;
                }
                prFbmSeqHdr->fgProgressiveSeq = 1;
                prFbmSeqHdr->u2HSize = prDecPrm->u2Width;
                prFbmSeqHdr->u2VSize = prDecPrm->u2Height;
                prFbmSeqHdr->u2OrgHSize = prDecPrm->u2Width;
                prFbmSeqHdr->u2OrgVSize = prDecPrm->u2Height;
                prFbmSeqHdr->u2LineSize = (UINT16)prDecPrm->u4LineSize;
            }
            else
            {
                    // Did not parse the frame rate from ES so far
    	  	  //FBM_SetFrameBufferFlag(prDecPrm->u1FbgId, FBM_FLAG_SEQ_CHG);   
    	      prFbmSeqHdr->ucFrmRatCod = 5;        // 30 fps
    	  	  prFbmSeqHdr->fgProgressiveSeq = 1;
    	  	  prFbmSeqHdr->u2HSize = prDecPrm->u2Width;
    	  	  prFbmSeqHdr->u2VSize = prDecPrm->u2Height;
    	  	  prFbmSeqHdr->u2OrgHSize = prDecPrm->u2Width;
    	  	  prFbmSeqHdr->u2OrgVSize = prDecPrm->u2Height;
    	  	  prFbmSeqHdr->u2LineSize = (UINT16)prDecPrm->u4LineSize;
    	  	  //prFbmSeqHdr->u2LineSize = prDecPrm->u4LineSize;
    	  	  //FBM_PicStructChgNotify(0, MPEG_PIC_FRAME);
            }

        }
    }

    if (prDecPrm->u1FbgId != FBM_FBG_ID_UNKNOWN)
    {
        // Check if reset is required
        if (FBM_ChkFrameBufferFlag(prDecPrm->u1FbgId, (UINT32)FBM_FLAG_RESET))
        {
            FBM_ResetGroup(prDecPrm->u1FbgId);
            FBM_ClrFrameBufferFlag(prDecPrm->u1FbgId, (UINT32)FBM_FLAG_RESET);
            FBM_ClrFrameBufferFlag(prDecPrm->u1FbgId, (UINT32)FBM_FLAG_NOT_READY);
    //            FBM_SetFrameBufferFlag(prDecPrm->u1FbgId, FBM_FLAG_CLEAR_FRC_PRM);

            /*if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_2ND_FLD_PIC))
            {
                _MPV_SkipPic(*pucMpvId, prVdecEsInfo->u4EsId);
                *pucMpvId = VDEC_MAX_VLD;
            }*/                        
        }
    }

    prVdecEsInfo = _VDEC_GetEsInfo(0);

    FBM_GetRefFrameBuffer(prDecPrm->u1FbgId, &ucFRefFbId, &ucBRefFbId);

    // Video's PTS
    if (ucBRefFbId != FBM_FBG_ID_UNKNOWN)
    {
        prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucBRefFbId);
        if (prFbmPicHdr)
        {
            prFbmPicHdr->u4PTS = prDecPrm->u4RefPts;
            prFbmPicHdr->u8Offset = prDecPrm->u8RecOffset;
            prFbmPicHdr->u8OffsetDisp = prVdecEsInfo->u8OffsetLast;
            prFbmPicHdr->u8OffsetI = prDecPrm->u8RecOffsetI;
            prFbmPicHdr->ucTrueZeroPTS = FBM_USE_ZERO_PTS_IF_ZERO;
        }
        else
        {
            LOG(3, "_PrsVOPHdr prFbmPicHdr == NULL\n");
            *pi4RetValue = E_VDEC_FAIL;
            return;
        }

        fgNotDisplay = VDEC_IsNotDisplay(0, prDecPrm->u4RefPts, 0, 0);
        if (fgNotDisplay)
        {
            /*FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, 
                ucBRefFbId, 
                FBM_MM_NOT_DISPLAY_FLAG);

            LOG(6, "_PrsVOPHdr fgNotDisplay ucFbId %d\n", ucBRefFbId);*/
            UCHAR ucFbStatus;
            ucFbStatus = FBM_GetFrameBufferStatus(prDecPrm->u1FbgId, ucBRefFbId);
            if ((ucFbStatus != FBM_FB_STATUS_DISPLAYQ) ||
                (ucFbStatus != FBM_FB_STATUS_LOCK))
            {
                FBM_SetFrameBufferStatus(prDecPrm->u1FbgId,
                   ucBRefFbId, FBM_FB_STATUS_EMPTY);
            }
            else
            {
                LOG(6, "Frame buffer status = %x\n", ucFbStatus);
            }
        }
        else
        {
            UCHAR ucFbStatus;
            LOG(6, "_PrsDx311Hdr prFbmPicHdr->u4PTS = %ld\n", prFbmPicHdr->u4PTS);

            ucFbStatus = FBM_GetFrameBufferStatus(prDecPrm->u1FbgId, ucBRefFbId);
            if ((ucFbStatus == FBM_FB_STATUS_DECODE) || (ucFbStatus == FBM_FB_STATUS_FIELD_READY) ||
			 (ucFbStatus == FBM_FB_STATUS_READY))
            {
                FBM_SetFrameBufferStatus(prDecPrm->u1FbgId,
                       ucBRefFbId, FBM_FB_STATUS_DISPLAYQ);

                prVdecEsInfo->u4DisplayQPicCnt++;
            }
            else
            {
                LOG(6, "Frame buffer status = %x\n", ucFbStatus);
            }
        }
        VDEC_CheckStopStatus(0, prDecPrm->u4RefPts, 0);
    }
    // FBM error
    else if (prDecPrm->u1VopCodingType != VOP_CODING_TYPE_I)
    {
        LOG(3, "Wrong reference for P VOP\n");
        *pi4RetValue = E_VDEC_FAIL;
        return;
    }
    
#ifdef IC_MPEG4_VERIFICATION
        prDecPrm->u4LineSize = prDecPrm->u2Width;
#endif

    prDecPrm->fgValidPts = prDecPrm->rPesInfo.fgDtsValid;
    prDecPrm->u4RefPts = prDecPrm->rPesInfo.u4PTS;
    prDecPrm->u8RecOffset = prDecPrm->rPesInfo.u8Offset;
    prDecPrm->u8RecOffsetI = prDecPrm->rPesInfo.u8OffsetI;

    LOG(6, "ucFRefFbId = 0x%08x\n", ucFRefFbId);
    LOG(6, "ucBRefFbId = 0x%08x\n", ucBRefFbId);

    prDecPrm->u1FbId = FBM_GetEmptyRefFrameBuffer(prDecPrm->u1FbgId, MPEG4_WAIT_DISP_TIME);

#ifdef USE_FBM_BUF
    FBM_GetWorkingBuffer(prDecPrm->u1FbgId, &u4WorkingBuffer, &u4WorkingSize);    

    prDecPrm->u4DCACBufAddr = u4WorkingBuffer;
    prDecPrm->u4MVECBufAddr = u4WorkingBuffer + MP4_DCAC_SZ;
    prDecPrm->u4BMB1BufAddr = prDecPrm->u4MVECBufAddr + MP4_MVEC_SZ;
    prDecPrm->u4BMB2BufAddr = prDecPrm->u4BMB1BufAddr + MP4_BMB1_SZ;
    prDecPrm->u4BCODEBufAddr = prDecPrm->u4BMB2BufAddr + MP4_BMB2_SZ; 
#else 
    if (prDecPrm->u4DCACBufAddr == 0)
    {
    	prDecPrm->u4DCACBufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_DCAC_SZ, 16);
    }
    
    if (prDecPrm->u4MVECBufAddr == 0)
    {
    	prDecPrm->u4MVECBufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_MVEC_SZ, 16);
    }
    
    if (prDecPrm->u4BMB1BufAddr == 0)
    {
    	prDecPrm->u4BMB1BufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_BMB1_SZ, 16);
    }
    
    if (prDecPrm->u4BMB2BufAddr == 0)
    {
    	prDecPrm->u4BMB2BufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_BMB2_SZ, 16);
    }
    
    if (prDecPrm->u4BCODEBufAddr == 0)
    {
    	prDecPrm->u4BCODEBufAddr = (UINT32)BSP_AllocAlignedDmaMemory(MP4_BCODE_SZ, 16);
    }
#endif


    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 2);
    prDecPrm->u1VopQuant = (UINT8)((u4Temp >> 27) & 0x1f);
    u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 5);
    if(prDecPrm->u1VopCodingType == VOP_CODING_TYPE_I)
    {
    	prDecPrm->u1VopRoundingType = 1;
    	prDecPrm->rDx3Params.u1FrameMode = (UINT8)((u4Temp >> 27) & 0x1f);
    	u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 5);
    	prDecPrm->rDx3Params.u1AltIAcChromDct = (UINT8)((u4Temp >> 31) & 0x1);
    	u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    	if(prDecPrm->rDx3Params.u1AltIAcChromDct)
    	{
    	  	prDecPrm->rDx3Params.u1AltIAcChromDctIdx = (UINT8)((u4Temp >> 31) & 0x1);
    	  	u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    	}
    	else
    	{
    	 	prDecPrm->rDx3Params.u1AltIAcChromDctIdx = 0;
    	}
    	prDecPrm->rDx3Params.u1AltIAcLumDct = (UINT8)((u4Temp >> 31) & 0x1);
    	u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    	if(prDecPrm->rDx3Params.u1AltIAcLumDct)
    	{
    		 prDecPrm->rDx3Params.u1AltIAcLumDctIdx = (UINT8)((u4Temp >> 31) & 0x1);
    		 u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    	}
    	else
    	{
    	 	 prDecPrm->rDx3Params.u1AltIAcLumDctIdx = 0;
    	}
    	prDecPrm->rDx3Params.u1AltIDcDct = (UINT8)((u4Temp >> 31) & 0x1);
    	u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    	// 050131: has_skip and alternative_P_AC_DCT_index should be 0 in I-VOP,
    	//         or the I will be wrongly decoded.
    	prDecPrm->rDx3Params.u1HasSkip = 0;
    	prDecPrm->rDx3Params.u1AltPAcDct = 0;
    	prDecPrm->rDx3Params.u1AltPAcDctIdx = 0;
    	prDecPrm->rDx3Params.u1AltPDcDct = 0;
    	prDecPrm->rDx3Params.u1AltMv = 0;
    }
    else
    {
    	if(prDecPrm->rDx3Params.fgSwitchRounding)
    	{
    		prDecPrm->u1VopRoundingType = (UINT8)(1 - prDecPrm->u1VopRoundingType);
    	}
    	else
    	{
    	   	prDecPrm->u1VopRoundingType = 0;
    	}

    	prDecPrm->rDx3Params.u1HasSkip = (UINT8)((u4Temp >> 31) & 0x1);
    	u4Temp =  _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    	prDecPrm->rDx3Params.u1AltPAcDct = (UINT8)((u4Temp >> 31) & 0x1);
    	u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    	if(prDecPrm->rDx3Params.u1AltPAcDct)
    	{
    		prDecPrm->rDx3Params.u1AltPAcDctIdx = (UINT8)((u4Temp >> 31) & 0x1);
    		u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 1);
    	}
    	else
    	{
    		prDecPrm->rDx3Params.u1AltPAcDctIdx = 0;
    	}
    	prDecPrm->rDx3Params.u1AltPDcDct = (UINT8)((u4Temp >> 31) & 0x1);
    	prDecPrm->rDx3Params.u1AltMv = (UINT8)((u4Temp >> 30) & 0x1);
    	u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 2);
    }

    /*prFbmPicHdr->u4PTS = prDecPrm->rPesInfo.u4PTS;
    if (prDecPrm->rPesInfo.fgEos)
    {
        FBM_SetFrameBufferPicFlag(prDecPrm->u1FbgId, prDecPrm->u1FbId, FBM_MM_EOS_FLAG);   
    }*/

    *pi4RetValue = E_VDEC_OK;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

void _VDEC_PrsVideoType(VDEC_MPEG4_DEC_PARAM_INFO_T* prDecPrm, INT32* pi4RetValue)
{
	UINT32 u4Temp;

	*pi4RetValue = E_VDEC_OK;

	u4Temp = _VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 0);

	VLD_WRITE32(RW_VLD_MP4_FLG, 1);
	
	if ((u4Temp >> 8) == M4V_START_CODE)
	{
		prDecPrm->pfnPrsHdr = _PrsM4vHdr;
		prDecPrm->pfnDecInit = _VMpeg4DEC_DecProc;
		prDecPrm->u1VideoType = VIDEO_TYPE_ISO_MPEG4;
	}
	else if ((u4Temp  & H263_VIDEO_START_MASK) == H263_VIDEO_START_CODE)
	{
		prDecPrm->pfnPrsHdr = _PrsH263Hdr;
		prDecPrm->pfnDecInit = _VMpeg4DEC_DecProc;
		prDecPrm->u1VideoType = VIDEO_TYPE_H263;
	}
#ifdef CC_IS_MPEG4_POST_MT5363
	else if ((u4Temp  & H263_VIDEO_START_MASK) == FLV_VIDEO_START_CODE)
	{
		prDecPrm->pfnPrsHdr = _PrsFlvHdr;
		prDecPrm->pfnDecInit = _VMpeg4DEC_DecProc;
		prDecPrm->u1VideoType = VIDEO_TYPE_FLV;
	}
#endif
	// Because the biCompression is not fixed for DivX311
	// we can't use any start code to specify the video type for DivX311.
	else //if (u4Temp == DX311_VIDEO_START_CODE)
	{
		prDecPrm->pfnPrsHdr = _PrsDx311Hdr;
		prDecPrm->pfnDecInit = _VDEC_DecDx311Proc;
		prDecPrm->u1VideoType = VIDEO_TYPE_DX311;
		prDecPrm->rM4vParams.rVol.u4DivxCodecVersion = 311;
	}
	/*else
	{
		prDecPrm->u1VideoType = VIDEO_TYPE_UNKNOWN;
		*pi4RetValue = E_VDEC_FAIL;
	}*/
	
}

UINT32 _VDEC_Mpeg4GetStartCode(UINT32* pu4BitCount)
{
	UINT32 u4Temp;

	u4Temp = _VDEC_Mpeg4GetBitStream(pu4BitCount, 0);
	
	while(((u4Temp >> 8) != M4V_START_CODE))
	{
		u4Temp = _VDEC_Mpeg4GetBitStream(pu4BitCount, 8);
	}

	return (UINT8)(u4Temp & 0xff);

}

// Miscs
static UINT32 _GetDivXVersion(UINT8 *u1Buf)
{
	UINT32	u4RetVal;

  	u4RetVal = (UINT32)((u1Buf[0] & 0xf) * 100) +
             		   ((u1Buf[1] & 0xf) * 10) +
             		   (u1Buf[2] & 0xf);
  	return u4RetVal;
}

static UINT32 _LsbMask(UINT32 u4ShiftBits)
{
	UINT32	i;
	UINT32	u4Mask = 0;

	for(i = 0; i < u4ShiftBits; i++)
	{
		u4Mask |= (0x1 << i);
	}
	return u4Mask;
}

static void _Align2Byte(UINT32* pu4BitCount)
{
	if ((*pu4BitCount % 8))
	{
		_VDEC_Mpeg4GetBitStream(pu4BitCount, 8 - (*pu4BitCount % 8));
	}
}

static BOOL _SrcFmtTbl(VDEC_MPEG4_DEC_PARAM_INFO_T* prDecPrm)
{

	UINT32 u4WidthTbl[5] = {128, 176, 352, 704, 1408};
  	UINT32 u4HeightTbl[5] = {96, 144, 288, 576, 1152};
	VDEC_M4V_PIC_INFO_T 	*prVopPrm;

       prVopPrm = &prDecPrm->rM4vParams.rVop;
	if((prVopPrm->u1SourceFormat >= 1) && (prVopPrm->u1SourceFormat <= 5))
	{
		prDecPrm->u2Width = (UINT16)u4WidthTbl[prVopPrm->u1SourceFormat - 1];
		prDecPrm->u2Height = (UINT16)u4HeightTbl[prVopPrm->u1SourceFormat - 1];
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

#ifdef CC_IS_MPEG4_POST_MT5363
static BOOL _FlvSrcFmtTbl(VDEC_MPEG4_DEC_PARAM_INFO_T* prDecPrm)
{

	UINT32 u4WidthTbl[5] = {352, 176, 128, 320,160};
  	UINT32 u4HeightTbl[5] = {288, 144, 96, 240,120};
	VDEC_M4V_PIC_INFO_T 	*prVopPrm;

    prVopPrm = &prDecPrm->rM4vParams.rVop;

    if (prVopPrm->u1SourceFormat == 0)
    {
        prDecPrm->u2Width = (UINT16)(_VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 8) & 0xFF);
        prDecPrm->u2Height = (UINT16)(_VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 8) & 0xFF);
    }
    else if (prVopPrm->u1SourceFormat == 1)
    {
        prDecPrm->u2Width = (UINT16)(_VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 16) & 0xFFFF);
        prDecPrm->u2Height = (UINT16)(_VDEC_Mpeg4GetBitStream(&prDecPrm->u4BitCount, 16) & 0xFFFF);
    }
    else if((prVopPrm->u1SourceFormat >= 2) && (prVopPrm->u1SourceFormat <= 5))
	{
		prDecPrm->u2Width = (UINT16)u4WidthTbl[prVopPrm->u1SourceFormat - 2];
		prDecPrm->u2Height = (UINT16)u4HeightTbl[prVopPrm->u1SourceFormat - 2];
	}
    else if(prVopPrm->u1SourceFormat == 6)
    {
        prDecPrm->u2Width = (UINT16)u4WidthTbl[prVopPrm->u1SourceFormat - 2];
		prDecPrm->u2Height = (UINT16)u4HeightTbl[prVopPrm->u1SourceFormat - 2];
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}
#endif
