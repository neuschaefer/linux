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
 * $RCSfile: vdec_avspsr.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_avspsr.c
 *  This file contains implementation of exported APIs of VDEC.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
#include "vdec_avsdec.h"
#include "vdec_avsutil.h"
#include "vdec_avshw.h"
#include "fbm_drvif.h"
#include "x_timer.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_common.h"
#include "x_typedef.h"
#ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
#endif
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//Error handling
#define CHKPARAM(u4Param, u4HBound)\
    if(u4Param > u4HBound)\
    {\
        if(prDecPrm->prCurrFBufInfo)\
        {\
            if(FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId) == FBM_FB_STATUS_DECODE)\
            {\
                FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId, FBM_FB_STATUS_EMPTY);\
                _VDEC_ClrAvsFBufInfo(prDecPrm, prDecPrm->u1DecFBufIdx, FALSE);\
            }\
        }\
        LOG(2, "Hdr err\n");\
        *pi4RetValue = E_VDEC_FAIL;\
        return;\
    }

#define CHKPARAM_R(i4Param, i4LBound, i4HBound)\
    if((i4Param < i4LBound) || (i4Param > i4HBound))\
    {\
        if(prDecPrm->prCurrFBufInfo)\
        {\
            if(FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId) == FBM_FB_STATUS_DECODE)\
            {\
                FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId, FBM_FB_STATUS_EMPTY);\
                _VDEC_ClrAvsFBufInfo(prDecPrm, prDecPrm->u1DecFBufIdx, FALSE);\
            }\
        }\
        LOG(2, "Hdr err\n");\
        *pi4RetValue = E_VDEC_FAIL;\
        return;\
    }
    
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
extern void FBM_UpdateReferenceList(UCHAR ucFbgId, UCHAR ucFbId, BOOL fgReference);


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


// *********************************************************************
// Function    : void _VDEC_AvsSeqHdr(void)
// Description : Handle sequence header
// Parameter   : None
// Return      : None
// *********************************************************************
void _VDEC_AvsSeqHdr(VDEC_AVS_INFO_T* prVdec, INT32* pi4RetValue)
{
    VDEC_AVS_SEQ_T rSeqHdr = {0};
    VDEC_AVS_SEQ_T* prSeqHdr;
    VDEC_AVS_DECPRM_T* prDecPrm;
    
    ASSERT(prVdec!=NULL);
    ASSERT(pi4RetValue!=NULL);

    prSeqHdr = &prVdec->rSeqHdr;
    prDecPrm = &prVdec->rDecParam;
    //x_memset((void*)prSeqHdr, 0, sizeof(prSeqHdr));

    prDecPrm->u4BitCnt = 0;

    LOG(5, "==== Avs Seq Hdr ====\n");
    // Shift 0x000001 for read pointer
    _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 32);
    
    rSeqHdr.u4ProfileID = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 8);
    rSeqHdr.u4LevelID = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 8);
#if 0
    if((!fgChkProfile((UINT8)prSeqHdr->u4ProfileID)) || (!fgChkLevel((UINT8)prSeqHdr->u4LevelID)))
    {
        *pi4RetValue = (INT32)E_VDEC_FAIL;
        LOG(2, "err at SeqHdr Profile/Level\n");
        return;
    }  
#endif    
    rSeqHdr.u4IsProgSeq = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 1);
    rSeqHdr.u4HSize = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 14);
    rSeqHdr.u4VSize = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 14);
    LOG(5, "prSeqHdr->u4IsProgSeq = %d, H size = %d, V size = %d\n", 
        rSeqHdr.u4IsProgSeq,
        rSeqHdr.u4HSize,
        rSeqHdr.u4VSize);
    if(((rSeqHdr.u4HSize == 0) || (rSeqHdr.u4VSize == 0)) ||
        ((rSeqHdr.u4HSize > 1920) || (rSeqHdr.u4VSize > 1088)))
    {
        *pi4RetValue = (INT32)E_VDEC_FAIL;
        LOG(2, "err at SeqHdr HSize/VSize\n");
        return;
    }            
    rSeqHdr.u4ChromaFmt = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 2);
    rSeqHdr.u4SamplePrec = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 3);
    CHKPARAM_R((INT32)rSeqHdr.u4SamplePrec, 0, 1);
    rSeqHdr.u4AspRatio = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 4);
    CHKPARAM_R((INT32)rSeqHdr.u4AspRatio, 0, 4);
    rSeqHdr.u4FrmRate = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 4);
    CHKPARAM_R((INT32)rSeqHdr.u4FrmRate, 0, 8);
    LOG(5, "prSeqHdr->u4AspRatio = %d, prSeqHdr->u4FrmRate = %d\n", 
        rSeqHdr.u4AspRatio,
        rSeqHdr.u4FrmRate);
    rSeqHdr.u4BitRateL = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 18);
    rSeqHdr.u4MarketBitBR = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 1);    
    rSeqHdr.u4BitRateU = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 12);
    rSeqHdr.u4LowDelay = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 1);
    rSeqHdr.u4MarkerBit = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 1);
    rSeqHdr.u4BBVSize = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 18);
    rSeqHdr.u4RsvBits = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 3);

    rSeqHdr.u4IsValid = 1;

    x_memcpy((void*)prSeqHdr, &rSeqHdr, sizeof(VDEC_AVS_SEQ_T));

    *pi4RetValue = (INT32)E_VDEC_PARSE_NOT_READY;
}


// *********************************************************************
// Function    : void _VDEC_AvsPicHdr(void)
// Description : Handle picture header
// Parameter   : None
// Return      : None
// *********************************************************************
void _VDEC_AvsPicHdr(VDEC_AVS_INFO_T* prVdec, INT32* pi4RetValue)
{
    BOOL bIsIFrm;
    UINT32 u4Temp = 0xFFFFFFFF;

    UINT32 u4Rptr, u4Wptr;

    VDEC_AVS_SEQ_T* prSeqHdr;
    VDEC_AVS_PIC_T* prPicHdr;
    VDEC_AVS_DECPRM_T* prDecPrm;
    
    ASSERT(prVdec!=NULL);
    ASSERT(pi4RetValue!=NULL);

    // Turn On Bit Stuffing Check
    AVS_VLD_WRITE32(RW_AVS_VLD_BITSTUFFING_SWITCH, 1);

    prSeqHdr = &prVdec->rSeqHdr;
    if(!prSeqHdr->u4IsValid)
    {
        *pi4RetValue = (INT32)E_VDEC_FAIL;
        LOG(2, "SeqHdr !valid in PicHdr\n");
        return;
    }
    prPicHdr = &prVdec->rPicHdr;
    x_memset((void*)prPicHdr, 0, sizeof(prPicHdr));

    prDecPrm = &prVdec->rDecParam;

    // Reset bit count
    prDecPrm->u4BitCnt = 0;

    // Shift 0x000001 for read pointer
    _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 32);

    bIsIFrm = (prVdec->rPesInfo.ucPicType == I_PICTURE_SC) ? TRUE : FALSE;
    
    prPicHdr->u4BBVDelay = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 16);
    if(bIsIFrm)
    {
        prPicHdr->u4TimeCodeFg = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 1);
        if(prPicHdr->u4TimeCodeFg)
        {
            prPicHdr->u4TimeCode = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 24);
        }
        prPicHdr->u4MarkerBit = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 1);
        prPicHdr->u4PicCodingType = I_IMG;
    }
    else
    {
        prPicHdr->u4PicCodingType = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 2);
        CHKPARAM_R(prPicHdr->u4PicCodingType, 1, 2);
    }

    LOG(6, "AVS coding type = %d\n", prPicHdr->u4PicCodingType);

    if (!prDecPrm->fgFirstI)
    {
        if (prPicHdr->u4PicCodingType != I_IMG)
        {
            LOG(3, "Wait for the first I frame\n");
            *pi4RetValue = (INT32)E_VDEC_FAIL;
            return;
        }
        else
        {
            prDecPrm->fgFirstI = TRUE;
        }
    }

    if ((prPicHdr->u4PicCodingType == B_IMG) &&
        (prDecPrm->u1LastRefIdx < 1))
    {
        LOG(3, "Not enough ref frame buf for B\n");
        //*pi4RetValue = (INT32)E_VDEC_SKIPFRAME;
        *pi4RetValue = (INT32)E_VDEC_FAIL;
        return;
    }

   
    prPicHdr->u4PicDistance = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 8);
    if(prSeqHdr->u4LowDelay)
    {
        //bbv_check_times
        prPicHdr->u4BBVCheckTimes = _VDEC_AvsUeCodeNum();
    }
    prPicHdr->u4ProgFrm = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 1);
    if(!prPicHdr->u4ProgFrm)
    {
        prPicHdr->u4PicStruct = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 1);
        if(!prPicHdr->u4PicStruct && !bIsIFrm)
        {
            prPicHdr->u4AdvPredModeDisable = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 1);
        }
        else
        {
            prPicHdr->u4AdvPredModeDisable = 1;
        }
    }
    else
    {
        prPicHdr->u4PicStruct = FRM_ENCODE;
    }
    prPicHdr->u4TFT = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 1);
    prPicHdr->u4RFF = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 1);
    prPicHdr->u4FixedPicQP = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 1);
    prPicHdr->u4PicQP = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 6);

    if(!bIsIFrm)
    {
        if(!((prPicHdr->u4PicCodingType == B_IMG) && 
            (prPicHdr->u4PicStruct == FRM_ENCODE) && 
            (prPicHdr->u4AdvPredModeDisable)))
        {
            prPicHdr->u4PicRefFg = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 1);
        }
        prPicHdr->u4NoForwardRefFg = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 1);
        prPicHdr->u4RsvBits = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 3);
    }

    if((!prPicHdr->u4ProgFrm && !prPicHdr->u4PicStruct) || (!bIsIFrm))
    {
        prPicHdr->u4SkipModeFg = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 1);
    }
    if(bIsIFrm)
    {
        prPicHdr->u4RsvBits = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 4);
    }


    
    // loop filter
    prPicHdr->u4LoopFilterDisable = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 1);
#ifdef CC_DUMP_AVS_RISC_SETTING
    if (prDecPrm->fgDumpSetting)
    {
        Printf("RISCWrite(`AVS_VLD_BASE + 49*4, 32'h1);\n");
        Printf("AVS_VLD_SHIFT_BIT(%d);\n", prDecPrm->u4BitCnt);
    }
#endif
    if(!prPicHdr->u4LoopFilterDisable)
    {
        prPicHdr->u4LoopFilterParamFg = _VDEC_AVSGetRealShtBitStream(&prDecPrm->u4BitCnt, 1);
#ifdef CC_DUMP_AVS_RISC_SETTING
        if (prDecPrm->fgDumpSetting)
        {
            Printf("AVS_VLD_SHIFT_BIT(1);\n");
        }
#endif
        if(prPicHdr->u4LoopFilterParamFg)
        {
            UINT32 u4Temp = 0;
            u4Temp = _VDEC_AvsSeCodeNum();
            if (u4Temp & 0x10000)
            {
                prPicHdr->i4AlphaCOffset = (INT32)(u4Temp & 0xFFFF);
                prPicHdr->i4AlphaCOffset = ((((UINT32)(prPicHdr->i4AlphaCOffset) ^ (0xFFFFFFFF)) + 1) & 0xFFFF) * (-1); 
            }
            else
            {
                prPicHdr->i4AlphaCOffset = (INT32)(u4Temp & 0xFFFF);
            }
            //prPicHdr->i4AlphaCOffset = _VDEC_AvsSeCodeNum();
            CHKPARAM_R(prPicHdr->i4AlphaCOffset, -8, 8);
            u4Temp = _VDEC_AvsSeCodeNum();
            if (u4Temp & 0x10000)
            {
                prPicHdr->i4BetaOffset = (INT32)(u4Temp & 0xFFFF);
                prPicHdr->i4BetaOffset = ((((UINT32)(prPicHdr->i4BetaOffset) ^ (0xFFFFFFFF)) + 1) & 0xFFFF) * (-1); 
            }
            else
            {
                prPicHdr->i4BetaOffset = (INT32)(u4Temp & 0xFFFF);
            }
            //prPicHdr->i4BetaOffset = _VDEC_AvsSeCodeNum();
 #ifdef CC_DUMP_AVS_RISC_SETTING
        if (prDecPrm->fgDumpSetting)
        {
            Printf("RISCRead_VDEC(`AVS_VLD_BASE + %d*4);\n", RO_AVS_VLD_SE >> 2);
            Printf("RISCRead_VDEC(`AVS_VLD_BASE + %d*4);\n", RO_AVS_VLD_SE >> 2);
        }
#endif
           CHKPARAM_R(prPicHdr->i4BetaOffset, -8, 8);
        }
    }

    //_VDEC_AVSAlign2Byte(&prDecPrm->u4BitCnt);
    AVS_VLD_WRITE32(RW_AVS_FIND_STARTCODE, 1);
#ifdef CC_DUMP_AVS_RISC_SETTING
    if (prDecPrm->fgDumpSetting)
    {
        Printf("RISCWrite(`AVS_VLD_BASE + 52*4, 32'h1);\n");
    }
#endif
    while (1)
    {
        u4Rptr = VLD_READ32(RO_VLD_VRPTR);
        u4Wptr = VLD_READ32(RO_VLD_VWPTR);
        while (AVAILABLE_SZ(u4Rptr, u4Wptr) <= AVS_MIN_LEN)
        {
            LOG(3, "There is not enough data for start code search\n");
            *pi4RetValue = (INT32)E_VDEC_FAIL;
            break;
        }

        while(AVS_VLD_READ32(RW_AVS_FIND_STARTCODE))
        {
        }
#ifdef CC_DUMP_AVS_RISC_SETTING
        if (prDecPrm->fgDumpSetting)
        {
            Printf("RISCRead_VDEC(`AVS_VLD_BASE + 52*4);\n");
            Printf("while (data_out != 32'h0)\n");
            Printf("begin\n");
            Printf("@(posedge bclk);\n");
            Printf("RISCRead_VDEC(`AVS_VLD_BASE + 52*4);\n");
        }
#endif
        // Shift 0x000001 for read pointer
        u4Temp = _VDEC_AVSGetShtBitStream(&prDecPrm->u4BitCnt, 0);
        if (((u4Temp >> 8) == 0x000001) &&
            ((u4Temp & 0x000000FF) <= 0xAF))
        {
            break;
        }
        AVS_VLD_WRITE32(RW_AVS_FIND_NEXT_STARTCODE, 1);
#ifdef CC_DUMP_AVS_RISC_SETTING
        if (prDecPrm->fgDumpSetting)
        {
            Printf("RISCWrite(`AVS_VLD_BASE + 53*4, 32'h1)\n");
        }
#endif
    }
    
#ifdef CC_DUMP_AVS_RISC_SETTING
    if (prDecPrm->fgDumpSetting)
    {
        Printf("end\n");
    }
#endif
    //Printf("Decoder's Input Window: 0x%8x\n", AVS_VLD_READ32(0));

    prPicHdr->u4IsValid = 1;
    *pi4RetValue = (INT32)E_VDEC_OK;
}


/** Get decoding handler
*@param .
 */
 void _VDEC_AvsSliceHdr(VDEC_AVS_INFO_T* prVdec, INT32* pi4RetValue)
{
    VDEC_AVS_SEQ_T* prSeqHdr;
    VDEC_AVS_PIC_T* prPicHdr;    
    VDEC_AVS_SLICE_T* prSliceHdr;
    VDEC_AVS_DECPRM_T* prDecParam;
    UINT8 u1weight_para_num = 0, u1Idx = 0;
    
    ASSERT(prVdec!=NULL);
    ASSERT(pi4RetValue!=NULL);

    prSeqHdr = &prVdec->rSeqHdr;
    prPicHdr = &prVdec->rPicHdr;
    if(!prSeqHdr->u4IsValid || !prPicHdr->u4IsValid)
    {
        *pi4RetValue = (INT32)E_VDEC_FAIL;
        LOG(2, "Seq/Pic Hdr !valid in SliceHdr\n");
        return;
    }

    prDecParam = &prVdec->rDecParam;
    prSliceHdr = &prVdec->rSliceHdr;
    x_memset((void*)prSliceHdr, 0, sizeof(prSliceHdr));

    prDecParam->u4BitCnt = 0;

    if(prSeqHdr->u4HSize > 2800)
    {
        prSliceHdr->u2VPosExt = _VDEC_AVSGetRealShtBitStream(&prDecParam->u4BitCnt, 3);
    }
    if(prPicHdr->u4FixedPicQP == 0)
    {
        prSliceHdr->u2FixedSliceQP = _VDEC_AVSGetRealShtBitStream(&prDecParam->u4BitCnt, 1);
        prSliceHdr->u2SliceQP = _VDEC_AVSGetRealShtBitStream(&prDecParam->u4BitCnt, 6);
    }

    if(prPicHdr->u4PicCodingType != I_IMG /*|| I bottom*/)
    {
        prSliceHdr->u2SliceWeightFg = _VDEC_AVSGetRealShtBitStream(&prDecParam->u4BitCnt, 1);
        if(prSliceHdr->u2SliceWeightFg)
        {
            /*if(((prPicHdr->u4PicCodingType == I_IMG) && !prPicHdr->u4PicStruct) &&
                (mb_index >= (mb_width * mb_height)>>1))  //I bottom
            {
                u1weight_para_num = 1;
            }
            else */if((prPicHdr->u4PicCodingType == P_IMG) && prPicHdr->u4PicStruct)  //P frame coding
            {
                u1weight_para_num = 2;
            }
            else if((prPicHdr->u4PicCodingType == P_IMG) && !prPicHdr->u4PicStruct) //P field coding
            {
                u1weight_para_num = 4;
            }
            else if((prPicHdr->u4PicCodingType == B_IMG) && prPicHdr->u4PicStruct)  //B frame coding
            {
                u1weight_para_num = 2;
            }
            else if((prPicHdr->u4PicCodingType == B_IMG) && !prPicHdr->u4PicStruct) //B field coding
            {
                u1weight_para_num = 4;
            }
		   
            for(u1Idx = 0; u1Idx < u1weight_para_num; u1Idx++)
            {
                prSliceHdr->u1LumaScale[u1Idx] = _VDEC_AVSGetRealShtBitStream(&prDecParam->u4BitCnt, 8);
                prSliceHdr->i1LumaShift[u1Idx] = _VDEC_AVSGetRealShtBitStream(&prDecParam->u4BitCnt, 8);
                _VDEC_AVSGetRealShtBitStream(&prDecParam->u4BitCnt, 1);
                prSliceHdr->u1ChromaScale[u1Idx] = _VDEC_AVSGetRealShtBitStream(&prDecParam->u4BitCnt, 8);
                prSliceHdr->i1ChromaShift[u1Idx] = _VDEC_AVSGetRealShtBitStream(&prDecParam->u4BitCnt, 8);
                _VDEC_AVSGetRealShtBitStream(&prDecParam->u4BitCnt, 1);
            }
            prSliceHdr->u2MBWeightFg = _VDEC_AVSGetRealShtBitStream(&prDecParam->u4BitCnt, 1);
        }
    }
    
    *pi4RetValue = (INT32)E_VDEC_OK;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------



