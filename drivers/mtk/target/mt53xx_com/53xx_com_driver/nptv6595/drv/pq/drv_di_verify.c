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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: drv_di_auto.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#include "hw_di.h"
#include "hw_di_int.h"
#include "hw_scpos.h"
#include "hw_ycproc.h"
#include "drv_di.h"
#include "drv_di_int.h"
#include "drv_meter.h"
#include "di_debug.h"
#include "vdo_misc.h"

#ifdef PSCAN_AUTO_VERIFY
#define INK_LEVEL 16
#define BLOCK_SIZE 1
#define FRAME_SIZE 1000
#define SCALE_SIZE 1000
#define LUMA_AVG_DIFF 5

typedef struct 
{
    UINT16 u2Width;
    UINT16 u2ScaleFactor;
    UINT32 u4OmuxCRC[FRAME_SIZE];
    UINT32 u4PreProcCRC[FRAME_SIZE];
    UINT32 u4SrcRCRC[FRAME_SIZE];
    UINT32 u4SrcWCRC[FRAME_SIZE];
    UINT32 u4OutCRC[FRAME_SIZE];
} OUTPUT_CRC_TBL;

typedef struct 
{
    UINT16 u2HistInk[INK_LEVEL];
} INK_LABEL_TBL;

typedef struct 
{
    INK_LABEL_TBL InkStack[FRAME_SIZE];
} INK_BLOCK_TBL;

typedef struct 
{
    UINT16 u2Width;
    UINT16 u2ScaleFactor;
    INK_BLOCK_TBL InkBlockStk[BLOCK_SIZE];
} INK_SCALE_TBL;

typedef struct 
{
    UINT16 u2XStart;
    UINT16 u2XEnd;
    UINT16 u2YStart;
    UINT16 u2YEnd;
} INK_RANGE_TBL;

enum
{
    ST_WAIT_FOR_KEYFRAME  = 0,
    ST_RECORDING = 1,
    ST_FINISH = 2
};

enum
{
    ST_WAIT_FOR_SCENE_CHG = 0,
    ST_FIRST_SCENE_CHG = 1,
    ST_SCENE_REPEAT = 2
};

static INK_SCALE_TBL _InkScaleStk[SCALE_SIZE];
static INK_RANGE_TBL _InkRangeStk[BLOCK_SIZE];
static OUTPUT_CRC_TBL _OutputCrcStk[SCALE_SIZE];
static UINT8 _u1BlockIdx = 0;
static UINT8 _u1CurState = ST_WAIT_FOR_KEYFRAME;
static UINT16 _u2StackIdx = 0;
static UINT32 _u4OmuxCRC = 0;
UINT8 u1InkOnOff[VDP_NS] = {0, 0};
UINT8 u1CrcOnOff[VDP_NS] = {0, 0};
UINT8 u1GetLength[VDP_NS] = {0, 0};
UINT16 u2FactorIdx = 0;
UINT16 u2LumaAvgPre = 0;
UINT16 u2LumaAvgCur = 0;
UINT16 u2FactorSize = SCALE_SIZE;
UINT16 u2FrameSize = FRAME_SIZE;

static void _vDrvDISaveINKRecord(UINT8 bBlockIdx, UINT32 bStackIdx)
{   
    _InkScaleStk[u2FactorIdx].InkBlockStk[bBlockIdx].InkStack[bStackIdx].u2HistInk[0] = MDDI_READ_FLD(VDP_1, MDDI_DARE_18, DAR_INK_HIST_0);
    _InkScaleStk[u2FactorIdx].InkBlockStk[bBlockIdx].InkStack[bStackIdx].u2HistInk[1] = MDDI_READ_FLD(VDP_1, MDDI_DARE_18, DAR_INK_HIST_1);
    _InkScaleStk[u2FactorIdx].InkBlockStk[bBlockIdx].InkStack[bStackIdx].u2HistInk[2] = MDDI_READ_FLD(VDP_1, MDDI_DARE_19, DAR_INK_HIST_2);
    _InkScaleStk[u2FactorIdx].InkBlockStk[bBlockIdx].InkStack[bStackIdx].u2HistInk[3] = MDDI_READ_FLD(VDP_1, MDDI_DARE_19, DAR_INK_HIST_3);
    _InkScaleStk[u2FactorIdx].InkBlockStk[bBlockIdx].InkStack[bStackIdx].u2HistInk[4] = MDDI_READ_FLD(VDP_1, MDDI_DARE_1A, DAR_INK_HIST_4);
    _InkScaleStk[u2FactorIdx].InkBlockStk[bBlockIdx].InkStack[bStackIdx].u2HistInk[5] = MDDI_READ_FLD(VDP_1, MDDI_DARE_1A, DAR_INK_HIST_5);
    _InkScaleStk[u2FactorIdx].InkBlockStk[bBlockIdx].InkStack[bStackIdx].u2HistInk[6] = MDDI_READ_FLD(VDP_1, MDDI_DARE_1B, DAR_INK_HIST_6);
    _InkScaleStk[u2FactorIdx].InkBlockStk[bBlockIdx].InkStack[bStackIdx].u2HistInk[7] = MDDI_READ_FLD(VDP_1, MDDI_DARE_1B, DAR_INK_HIST_7);
    _InkScaleStk[u2FactorIdx].InkBlockStk[bBlockIdx].InkStack[bStackIdx].u2HistInk[8] = MDDI_READ_FLD(VDP_1, MDDI_DARE_1C, DAR_INK_HIST_8);
    _InkScaleStk[u2FactorIdx].InkBlockStk[bBlockIdx].InkStack[bStackIdx].u2HistInk[9] = MDDI_READ_FLD(VDP_1, MDDI_DARE_1C, DAR_INK_HIST_9);
    _InkScaleStk[u2FactorIdx].InkBlockStk[bBlockIdx].InkStack[bStackIdx].u2HistInk[10] = MDDI_READ_FLD(VDP_1, MDDI_DARE_1D, DAR_INK_HIST_A);
    _InkScaleStk[u2FactorIdx].InkBlockStk[bBlockIdx].InkStack[bStackIdx].u2HistInk[11] = MDDI_READ_FLD(VDP_1, MDDI_DARE_1D, DAR_INK_HIST_B);
    _InkScaleStk[u2FactorIdx].InkBlockStk[bBlockIdx].InkStack[bStackIdx].u2HistInk[12] = MDDI_READ_FLD(VDP_1, MDDI_DARE_1E, DAR_INK_HIST_C);
    _InkScaleStk[u2FactorIdx].InkBlockStk[bBlockIdx].InkStack[bStackIdx].u2HistInk[13] = MDDI_READ_FLD(VDP_1, MDDI_DARE_1E, DAR_INK_HIST_D);
    _InkScaleStk[u2FactorIdx].InkBlockStk[bBlockIdx].InkStack[bStackIdx].u2HistInk[14] = MDDI_READ_FLD(VDP_1, MDDI_DARE_1F, DAR_INK_HIST_E);
    _InkScaleStk[u2FactorIdx].InkBlockStk[bBlockIdx].InkStack[bStackIdx].u2HistInk[15] = MDDI_READ_FLD(VDP_1, MDDI_DARE_1F, DAR_INK_HIST_F);
}

static UINT8 _u1DrvDIDetectKeyFrame(UINT8 u1VdpId)
{
    return ((_u4OmuxCRC == u4DrvDIGetOmuxCRC()) ? SV_TRUE : SV_FALSE);
}

static UINT16 _vDrvDICheckBoundLimit(UINT32 u4Value)
{
    if (u4Value > 65535)
    {
        return 65535;
    }
    return u4Value;
}

static UINT32 _u4DrvDIGetPreProcCRC(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, STA_CRC_MAIN_02, STA_CRC_V_OUT);
}

static UINT32 _u4DrvDIGetSrcRCRC(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, MDDI_KC_0A, SRCR_CRC_OUT_V);
}

// for FPGA verify use
void vDrvSetRelatedParam(UINT8 u1VdpId, UINT32 u4Factor, UINT32 u4Width)
{
    _arMDDiPrm[u1VdpId].u2Width = u4Width;
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_00, u4Width, HDEW);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MC_00, MPY_AND_CEIL(u4Width, 1, 16), H_PITCH);
    vRegWriteFldAlign(((u1VdpId == VDP_1) ? SCPIP_HDS_00 : SCPIP_HDS_01), u4Factor, ((u1VdpId == VDP_1) ? SCPIP_HDS_MAIN_SCALER : SCPIP_HDS_PIP_SCALER));
    vDrvDISetROIPixel(u1VdpId, MPY_AND_CEIL(_arMDDiPrm[u1VdpId].u2Width, _arMDDiPrm[u1VdpId].u2Height, 16));
}

void vDrvDISetDataSize(UINT16 u2Factor, UINT16 u2Frame)
{
    Printf("Factor Size:  %d -> %d\n", u2FactorSize, u2Factor);
    Printf("Frame Size: %d -> %d\n", u2FrameSize, u2Frame);
    u2FactorSize = u2Factor;
    u2FrameSize = u2Frame;
}

void vDrvDIInitInkRange(UINT16 u2Factor, UINT16 u2Width, UINT16 u2Height)
{
    UINT16 u2Idx = 0;
    UINT16 u2Idx1;
    UINT16 u2Idx2;

    _InkScaleStk[u2FactorIdx].u2Width = u2Width;
    _InkScaleStk[u2FactorIdx].u2ScaleFactor = u2Factor;

    Printf("Index: %d Width: %d Factor: 0x%x\n", u2FactorIdx, u2Width, u2Factor);
    
    // whole range
    _InkRangeStk[u2Idx].u2XStart = 0;
    _InkRangeStk[u2Idx].u2XEnd = u2Width - 1;
    _InkRangeStk[u2Idx].u2YStart = 0;
    _InkRangeStk[u2Idx++].u2YEnd = u2Height - 1;

    // sub range
    for(u2Idx1 = 0; u2Idx1 < u2Width; u2Idx1 += (u2Width/4))
    {
        for(u2Idx2 = 0; u2Idx2 < u2Height; u2Idx2 += (u2Height/2))
        {
            _InkRangeStk[u2Idx].u2XStart = u2Idx1;
            _InkRangeStk[u2Idx].u2XEnd = u2Idx1 + (u2Width/4) - 1;
            _InkRangeStk[u2Idx].u2YStart = u2Idx2;
            _InkRangeStk[u2Idx++].u2YEnd = u2Idx2 + (u2Height/2) - 1;
        }
    }
}

void vDrvDISetInkRange(UINT16 u2X_Start, UINT16 u2X_End, UINT16 u2Y_Start, UINT16 u2Y_End)
{
    MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0E, u2X_Start, DAR_INK_HIST_X_ST);
    MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0E, u2X_End, DAR_INK_HIST_X_END);
    MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0F, u2Y_Start, DAR_INK_HIST_Y_ST);
    MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0F, u2Y_End, DAR_INK_HIST_Y_END);
}

void vDrvDISetCurrentState(void)
{
    _u1CurState = ST_WAIT_FOR_KEYFRAME;
}

void vDrvDISetInkBlock(UINT8 u1VdpId, UINT8 u1Block)
{
    vDrvDISetCurrentState();
    _u2StackIdx = 0;
    _u1BlockIdx = u1Block;
    
    vDrvDISetInkRange(_InkRangeStk[u1Block].u2XStart, _InkRangeStk[u1Block].u2XEnd,
        _InkRangeStk[u1Block].u2YStart, _InkRangeStk[u1Block].u2YEnd);                                                   
}

void vDrvDIQueryInkStatus(void)
{
    Printf("INK_HIST_0: %d\n", MDDI_READ_FLD(VDP_1, MDDI_DARE_18, DAR_INK_HIST_0));
    Printf("INK_HIST_1: %d\n", MDDI_READ_FLD(VDP_1, MDDI_DARE_18, DAR_INK_HIST_1));
    Printf("INK_HIST_2: %d\n", MDDI_READ_FLD(VDP_1, MDDI_DARE_19, DAR_INK_HIST_2));
    Printf("INK_HIST_3: %d\n", MDDI_READ_FLD(VDP_1, MDDI_DARE_19, DAR_INK_HIST_3));
    Printf("INK_HIST_4: %d\n", MDDI_READ_FLD(VDP_1, MDDI_DARE_1A, DAR_INK_HIST_4));
    Printf("INK_HIST_5: %d\n", MDDI_READ_FLD(VDP_1, MDDI_DARE_1A, DAR_INK_HIST_5));
    Printf("INK_HIST_6: %d\n", MDDI_READ_FLD(VDP_1, MDDI_DARE_1B, DAR_INK_HIST_6));
    Printf("INK_HIST_7: %d\n", MDDI_READ_FLD(VDP_1, MDDI_DARE_1B, DAR_INK_HIST_7));
    Printf("INK_HIST_8: %d\n", MDDI_READ_FLD(VDP_1, MDDI_DARE_1C, DAR_INK_HIST_8));
    Printf("INK_HIST_9: %d\n", MDDI_READ_FLD(VDP_1, MDDI_DARE_1C, DAR_INK_HIST_9));
    Printf("INK_HIST_10: %d\n", MDDI_READ_FLD(VDP_1, MDDI_DARE_1D, DAR_INK_HIST_A));
    Printf("INK_HIST_11: %d\n", MDDI_READ_FLD(VDP_1, MDDI_DARE_1D, DAR_INK_HIST_B));
    Printf("INK_HIST_12: %d\n", MDDI_READ_FLD(VDP_1, MDDI_DARE_1E, DAR_INK_HIST_C));
    Printf("INK_HIST_13: %d\n", MDDI_READ_FLD(VDP_1, MDDI_DARE_1E, DAR_INK_HIST_D));
    Printf("INK_HIST_14: %d\n", MDDI_READ_FLD(VDP_1, MDDI_DARE_1F, DAR_INK_HIST_E));
    Printf("INK_HIST_15: %d\n", MDDI_READ_FLD(VDP_1, MDDI_DARE_1F, DAR_INK_HIST_F));
    Printf("\n");
}

void vDrvDIQueryInkLevel(UINT16 u2Factor, UINT8 u1Block, UINT16 u2Frame)
{
    if ((u1Block >= BLOCK_SIZE) || (u2Factor > u2FactorSize))
        return;

    UINT16 u2Idx1;
    
    Printf("Scaling Factor: 0x%x\n", _InkScaleStk[u2Factor].u2ScaleFactor);
    for (u2Idx1 = 0; u2Idx1 < INK_LEVEL; u2Idx1++)
    {
        Printf("INK_HIST_%d: %d\n", u2Idx1, _InkScaleStk[u2Factor].InkBlockStk[u1Block].InkStack[u2Frame].u2HistInk[u2Idx1]);
    }
}

void vDrvDIDumpAllHistogram(void)
{
    UINT16 u2Idx1;
    UINT16 u2Idx2;
    UINT16 u2Idx3;
    UINT16 u2Idx4;

    // Factor_No, Block_No, Ink_Level, Time_Frame
    Printf("%d, %d, %d, %d\n", u2FactorSize, BLOCK_SIZE, INK_LEVEL, u2FrameSize);
    
    for (u2Idx1 = 0; u2Idx1 < u2FactorSize; u2Idx1++)
    {
        for (u2Idx2 = 0; u2Idx2 < BLOCK_SIZE; u2Idx2++)
        {
            for (u2Idx3 = 0; u2Idx3 < INK_LEVEL; u2Idx3++)
            {
                Printf("%d, %d, ", u2Idx2, u2Idx3);
                Printf("%d, %d, ", _InkScaleStk[u2Idx1].u2Width, _InkScaleStk[u2Idx1].u2ScaleFactor);                
                for (u2Idx4 = 0; u2Idx4 < (u2FrameSize - 1); u2Idx4++)
                {
                    Printf("%d, ", _InkScaleStk[u2Idx1].InkBlockStk[u2Idx2].InkStack[u2Idx4].u2HistInk[u2Idx3]);
                }
                Printf("%d\n", _InkScaleStk[u2Idx1].InkBlockStk[u2Idx2].InkStack[u2FrameSize - 1].u2HistInk[u2Idx3]);
            }
        }
    }
}

UINT8 u1DrvDICheckResult(UINT32 u4RatioIdx)
{
    UINT8 u1InkLevel, u1Check = 0;
    UINT32 u4BlockIdx, u4FrameIdx;
    UINT32 u4SumAllBlock[INK_LEVEL];
    UINT32 u4FullBlock[INK_LEVEL];

    // initilize
    for (u1InkLevel = 0; u1InkLevel < INK_LEVEL; u1InkLevel++)
    {
        u4SumAllBlock[u1InkLevel] = 0;
        u4FullBlock[u1InkLevel] = 0;
    }

    // sumation of 8 sub block
    for (u4BlockIdx = 1; u4BlockIdx < BLOCK_SIZE; u4BlockIdx++)
    {
        for (u1InkLevel = 0; u1InkLevel < INK_LEVEL; u1InkLevel++)
        {
            for (u4FrameIdx = 0; u4FrameIdx < u2FrameSize; u4FrameIdx++)
            {
                u4SumAllBlock[u1InkLevel] += _InkScaleStk[u4RatioIdx].InkBlockStk[u4BlockIdx].InkStack[u4FrameIdx].u2HistInk[u1InkLevel];
            }
        }
    }

    // full frame
    u4BlockIdx = 0;
    for (u1InkLevel = 0; u1InkLevel < INK_LEVEL; u1InkLevel++)
    {
        for (u4FrameIdx = 0; u4FrameIdx < u2FrameSize; u4FrameIdx++)
        {
            u4FullBlock[u1InkLevel] += _InkScaleStk[u4RatioIdx].InkBlockStk[u4BlockIdx].InkStack[u4FrameIdx].u2HistInk[u1InkLevel];
        }

        Printf("[INK_HIST_%d] Whole Frame: %d\tSum of Sub Frame: %d\n", u1InkLevel, 
                   _vDrvDICheckBoundLimit(u4FullBlock[u1InkLevel]), _vDrvDICheckBoundLimit(u4SumAllBlock[u1InkLevel]));
        if (_vDrvDICheckBoundLimit(u4FullBlock[u1InkLevel]) == _vDrvDICheckBoundLimit(u4SumAllBlock[u1InkLevel]))
        {
            u1Check++;
        }
    }

    return ((u1Check == INK_LEVEL) ? SV_TRUE : SV_FALSE);
}

UINT8 u1DrvDIOutOfBound(void)
{
    if ((u2FactorIdx >= u2FactorSize) || 
        (u2FactorSize > SCALE_SIZE))
    {
        return SV_TRUE;
    }
    else
    {
        return SV_FALSE;
    }
}

UINT32 u4DrvDIGetOmuxCRC(void)
{
    return RegReadFldAlign(STA_PQCRC_01, PQCRC_OUT_V);
}

void vDrvDIGetClipLength(UINT8 u1VdpId)
{
    static UINT8 _u1NextState;
    static UINT16 _u2ClipLength = 0;
    
    if (u1GetLength[u1VdpId])
    {
        switch (_u1CurState)
        {
            case ST_WAIT_FOR_KEYFRAME:
                _u1NextState = (_u1DrvDIDetectKeyFrame(u1VdpId) ? ST_RECORDING : ST_WAIT_FOR_KEYFRAME);
                break;
                
            case ST_RECORDING:
                _u2ClipLength++;
                _u1NextState = (_u1DrvDIDetectKeyFrame(u1VdpId) ? ST_FINISH : ST_RECORDING);
                break;
                
            case ST_FINISH:
                Printf("Clip Length is : %d\n", _u2ClipLength);
                _u1NextState = ST_WAIT_FOR_KEYFRAME;
                _u2ClipLength = 0;
                _u1CurState = ST_WAIT_FOR_KEYFRAME;
                u1GetLength[u1VdpId] = 0;
                break;
        }
        if (_u1NextState != _u1CurState)
        {
            _u1CurState = _u1NextState;
        }
    }
}

void vDrvDIInkHistogramPolling(UINT8 u1VdpId)
{
    static UINT8 _u1NextState;

    if (u1InkOnOff[u1VdpId])
    {
        switch (_u1CurState)
        {
            case ST_WAIT_FOR_KEYFRAME:
                if (_u2StackIdx == u2FrameSize)
                {
                    _u1BlockIdx++;
                    vDrvDISetInkBlock(u1VdpId, _u1BlockIdx);          
                }
                _u2StackIdx = 0;

                if (_u1DrvDIDetectKeyFrame(u1VdpId)) // first frame
                {
                    _vDrvDISaveINKRecord(_u1BlockIdx, _u2StackIdx++);
                    _u1NextState = ST_RECORDING;
                }
                else
                {
                    _u1NextState = ST_WAIT_FOR_KEYFRAME;
                }
                break;
                
            case ST_RECORDING:
                if (_u2StackIdx < u2FrameSize)
                {
                    _vDrvDISaveINKRecord(_u1BlockIdx, _u2StackIdx++);
                }                
                
                if (_u1BlockIdx < BLOCK_SIZE)
                {
                    _u1NextState = ((_u2StackIdx == u2FrameSize) ? ST_WAIT_FOR_KEYFRAME : ST_RECORDING);
                }
                else
                {
                    _u1NextState = ST_FINISH;
                }
                break;

            case ST_FINISH:
                u1InkOnOff[u1VdpId] = 0;
                u2FactorIdx++;
                _u1CurState = ST_WAIT_FOR_KEYFRAME;
                _u1NextState = ST_WAIT_FOR_KEYFRAME;
                LOG(3, "Finish Ink Histogram Polling!\n");
                break;
        }

        if (_u1NextState != _u1CurState)
        {
            LOG(5, "State Transition: %d -> %d\n", _u1CurState, _u1NextState);
            _u1CurState = _u1NextState;
        }
    }
}

void vDrvDISetDetectCRC(UINT32 u4Value)
{
    _u4OmuxCRC = u4Value;
}

void vDrvDIDumpAllCrc(UINT8 u1Mode)
{
    UINT16 u2Idx1;
    UINT16 u2Idx2;

    Printf("%d, %d\n", u2FactorSize, u2FrameSize);
    for (u2Idx1 = 0; u2Idx1 < u2FactorSize; u2Idx1++)
    {
        for (u2Idx2 = 0; u2Idx2 < u2FrameSize; u2Idx2++)
        {
            switch (u1Mode)
            {
                case 0:
                    Printf("(Width, Factor, SRCR) = (%d, 0x%x, %08X)\n", 
                    _OutputCrcStk[u2Idx1].u2Width, _OutputCrcStk[u2Idx1].u2ScaleFactor,
                    _OutputCrcStk[u2Idx1].u4SrcRCRC[u2Idx2]);
                    break;
                case 1:
                    Printf("(Width, Factor, SRCW) = (%d, 0x%x, %08X)\n", 
                    _OutputCrcStk[u2Idx1].u2Width, _OutputCrcStk[u2Idx1].u2ScaleFactor,
                    _OutputCrcStk[u2Idx1].u4SrcWCRC[u2Idx2]);
                    break;
                case 2:
                    Printf("(Width, Factor, OUT) = (%d, 0x%x, %08X)\n", 
                    _OutputCrcStk[u2Idx1].u2Width, _OutputCrcStk[u2Idx1].u2ScaleFactor,
                    _OutputCrcStk[u2Idx1].u4OutCRC[u2Idx2]);
                    break;
                case 3:
                    Printf("%d, %x, %08X, %08X, %08X\n",
                    _OutputCrcStk[u2Idx1].u2Width, _OutputCrcStk[u2Idx1].u2ScaleFactor,
                    _OutputCrcStk[u2Idx1].u4SrcRCRC[u2Idx2], _OutputCrcStk[u2Idx1].u4SrcWCRC[u2Idx2], 
                    _OutputCrcStk[u2Idx1].u4OutCRC[u2Idx2]);
                    break;
                case 4:
                    Printf("(Width, Factor, OMUX, PREPROC, SRCR, SCRW, OUT) = (%d, %x, %08X, %08X, %08X, %08X, %08X)\n",
                    _OutputCrcStk[u2Idx1].u2Width, _OutputCrcStk[u2Idx1].u2ScaleFactor,
                    _OutputCrcStk[u2Idx1].u4OmuxCRC[u2Idx2],
                    _OutputCrcStk[u2Idx1].u4PreProcCRC[u2Idx2], _OutputCrcStk[u2Idx1].u4SrcRCRC[u2Idx2], 
                    _OutputCrcStk[u2Idx1].u4SrcWCRC[u2Idx2], _OutputCrcStk[u2Idx1].u4OutCRC[u2Idx2]);
                    break;
                default:
                    break;
            }
        }
    }
}

void vDrvDISceneChangeReset(UINT8 u1VdpId)
{
    if (u1VdpId == VDP_2)
    {
        return;
    }
    
    static UINT16 _u2LumaAvgDiff;
    _u2LumaAvgDiff = (u2LumaAvgCur > u2LumaAvgPre) ? (u2LumaAvgCur - u2LumaAvgPre) : (u2LumaAvgPre - u2LumaAvgCur);
    LOG(2, "(u2LumaAvgPre, u2LumaAvgCur) = (%d, %d)\n", u2LumaAvgPre, u2LumaAvgCur);
    
    if (_u2LumaAvgDiff > LUMA_AVG_DIFF) // scene change -> do iFusion/CS reset
    {
        LOG(2, "Do Scene Change!\n");
        MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_21, 0x3, IF_FORCE_SCENE_CHANGE);
        MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_21, 0x0, IF_FORCE_SCENE_CHANGE);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_13, 0x1, SC_FORCE_ON);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_13, 0x0, SC_FORCE_ON);
    }
}

void vDrvDISetCrcRange(UINT8 u1VdpId, UINT16 u2Factor, UINT16 u2Width)
{
    _u2StackIdx = 0;
    u1CrcOnOff[u1VdpId] = 1;
    _OutputCrcStk[u2FactorIdx].u2Width = u2Width;
    _OutputCrcStk[u2FactorIdx].u2ScaleFactor = u2Factor;
    vDrvDISetCurrentState();
    LOG(1, "Width: %d Factor: 0x%x\n", u2Width, u2Factor);
}

void vDrvDIStartRecord(UINT8 u1VdpId)
{
    u2LumaAvgCur = MDDI_READ_FLD(VDP_1, MDDI_FUSION_16, IF_AVG_Y);
    vDrvDISceneChangeReset(u1VdpId);
    u2LumaAvgPre = u2LumaAvgCur;

    _OutputCrcStk[u2FactorIdx].u4OmuxCRC[_u2StackIdx] = u4DrvDIGetOmuxCRC();
    _OutputCrcStk[u2FactorIdx].u4PreProcCRC[_u2StackIdx] = _u4DrvDIGetPreProcCRC(u1VdpId);
    _OutputCrcStk[u2FactorIdx].u4SrcRCRC[_u2StackIdx] = _u4DrvDIGetSrcRCRC(u1VdpId);
    _OutputCrcStk[u2FactorIdx].u4SrcWCRC[_u2StackIdx] = u4DrvDIGetInputCRC(u1VdpId);
    _OutputCrcStk[u2FactorIdx].u4OutCRC[_u2StackIdx] = u4DrvDIGetOutputCRC(u1VdpId);
    _u2StackIdx++;
}

void vDrvDIGetCRCResult(UINT8 u1VdpId)
{
    static UINT8 _u1NextState;
    
    if (u1CrcOnOff[u1VdpId])
    {
        switch (_u1CurState)
        {
            case ST_WAIT_FOR_KEYFRAME:
                if (_u1DrvDIDetectKeyFrame(u1VdpId)) // first frame
                {
                    _u1NextState = ST_RECORDING;                    
                    vDrvDIStartRecord(u1VdpId);
                }
               break;

            case ST_RECORDING:
               vDrvDIStartRecord(u1VdpId);
                _u1NextState = (_u2StackIdx == u2FrameSize) ? ST_FINISH : ST_RECORDING;
                break;

            case ST_FINISH:
                u1CrcOnOff[u1VdpId] = 0;
                u2FactorIdx++;
                _u1CurState = ST_WAIT_FOR_KEYFRAME;
                _u1NextState = ST_WAIT_FOR_KEYFRAME;
                break;
        }
        if (_u1NextState != _u1CurState)
        {
            LOG(5, "State Transition: %d -> %d\n", _u1CurState, _u1NextState);
            _u1CurState = _u1NextState;
        }
    }
}
#endif

