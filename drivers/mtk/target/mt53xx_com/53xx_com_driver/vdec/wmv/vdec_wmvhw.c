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
 * $RCSfile: vdec_wmvhw.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_wmvhw.c
 *  This header file declares exported APIs of wmv.
 */

#include "x_lint.h"
#include "vdec_wmvpsr.h"
#include "vdec_wmvhw.h"
#include "vdec_wmvhw.h"
#include "vdec_wmvutil.h"
#include "imgrz_if.h"
#include "imgrz_drvif.h"

#ifdef USE_FBM
#include "fbm_drvif.h"
#endif

LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_common.h"
#include "x_typedef.h"
LINT_EXT_HEADER_END


static int write_enable = 1;

EXTERN void FBM_SetPlayMode(UCHAR ucFbgId, UCHAR ucPlayMode);

void WriteREG(UINT32 address, UINT32 value)
{
    if (FALSE)
    {
        UINT32 u4Offset;
        u4Offset = (address & 0xFFF);
        switch (address & 0xFFFFF000)
        {
        case MC_REG_OFST:
            LOG(0,"MC_Write(%08x,%08x)\n", (u4Offset), (value));
            break;
        case VLD_REG_OFST:
            LOG(0,"VLD_Write(%08x,%08x)\n", u4Offset, value);
            break;
        default:
            break;
        }
    }
    HAL_WRITE32((address), (value));

}

UINT32 ReadREG(UINT32 address)
{
    if (rWMVDecInfo.rOther.fgDumpSimPat && FALSE)
    {
        UINT32 u4Offset;

        u4Offset = (address & 0xFFF)>>2;
        switch (address & 0xFFFFF000)
        {
        case MC_REG_OFST:
            LOG(0, "RISCRead_MC(%d);\n", u4Offset);
            break;
        case VLD_REG_OFST:
            LOG(0, "RISCRead_VLD(%d);\n", u4Offset);
            break;
        default:
            break;
        }
    }
    return (HAL_READ32((address)));
}

VOID vInitVDec(void)
{
    rWMVDecInfo.rOther.u1DecState = VDEC_NORM_INIT_PRM;
    rWMVDecInfo.rOther.u1GetSeqHdr = 0;
    rWMVDecInfo.rOther.u1FbgId = FBM_FBG_ID_UNKNOWN;
    vResetVDec();
    // initiate register files

    //vInitVdoMem();

    //vInitWMVMiscMem();
    vSetMcBufPtr();
    // vResetVDec();
    vSetVldVFifo();
    vSetFRefBuf(0);
    vSetBRefBuf(1);
}

VOID vInitVdoMem(void)
{
    UINT8 u1FbgType;
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(0);
    //rWMVDecInfo.rDram.pu1VFifo = (UINT8 *)V_FIFO_SA;
    rWMVDecInfo.rDram.pu1VFifo = (UINT8 *)rWMVDecInfo.rPesInfo.u4FifoStart;
#ifndef USE_FBM
    rWMVDecInfo.rDram.pu1Pic0Y = (UINT8 *)PIC0Y_SA; //byte address
    rWMVDecInfo.rDram.pu1Pic0C = (UINT8 *)PIC0C_SA;
    rWMVDecInfo.rDram.pu1Pic1Y = (UINT8 *)PIC1Y_SA;
    rWMVDecInfo.rDram.pu1Pic1C = (UINT8 *)PIC1C_SA;
    rWMVDecInfo.rDram.pu1Pic2Y = (UINT8 *)PIC2Y_SA;
    rWMVDecInfo.rDram.pu1Pic2C = (UINT8 *)PIC2C_SA;
#else
    //FBM_ReleaseGroup(rWMVDecInfo.rOther.u1FbgId);
    //UCHAR ucFbgType = FBM_SelectGroupType(rWMVDecInfo.rSeqHdrR.u4PicWidthSrc, rWMVDecInfo.rSeqHdrR.u4PicHeightSrc);
    if (FBM_ChkFrameBufferFlag(rWMVDecInfo.rOther.u1FbgId, FBM_FLAG_RELEASE_FBG))
    {
        FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, 0);
        FBM_WaitUnlockFrameBuffer(rWMVDecInfo.rOther.u1FbgId, 20);    // block until display unlock
        FBM_ReleaseGroup(rWMVDecInfo.rOther.u1FbgId);
        rWMVDecInfo.rOther.u1FbgId = FBM_FBG_ID_UNKNOWN;
    }

    //u1FbgType = FBM_SelectGroupType(prDecPrm->u4DramWidth, prDecPrm->u4DramHeight);
    //LOG(0,"hello it's the resolution : %dx%d\n",rWMVDecInfo.rSeqHdrR.u4PicWidthSrc,rWMVDecInfo.rSeqHdrR.u4PicHeightSrc);

    if (prVdecEsInfo->eMMSrcType != SWDMX_SRC_TYPE_NETWORK_NETFLIX)
    {
        u1FbgType = FBM_SelectGroupType(rWMVDecInfo.rSeqHdrR.u4PicWidthSrc, rWMVDecInfo.rSeqHdrR.u4PicHeightSrc);
    }
    else
    {
        u1FbgType = FBM_SelectGroupType(1920,1088);
    }

    rWMVDecInfo.rSeqHdrR.u4PrevPicWidthSrc = rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
    rWMVDecInfo.rSeqHdrR.u4PrevPicHeightSrc = rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;

    if (rWMVDecInfo.rOther.u1FbgId == FBM_FBG_ID_UNKNOWN)
    {
        rWMVDecInfo.rOther.u1FbgId = FBM_CreateGroup(u1FbgType, FBM_VDEC_VC1, 0, 0);
        FBM_SetPlayMode(rWMVDecInfo.rOther.u1FbgId,FBM_FBG_MM_MODE);
        ASSERT(rWMVDecInfo.rOther.u1FbgId != FBM_FBG_ID_UNKNOWN);
        prVdecEsInfo->ucFbgId = rWMVDecInfo.rOther.u1FbgId;
        FBM_SetSyncStc(rWMVDecInfo.rOther.u1FbgId, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId);
        //FBM_GetFrameBufferSize(prDecPrm->u1FbgId, &u4FbWidth, &u4FbHeight);
        FBM_FbgChgNotify(rWMVDecInfo.rOther.u1FbgId, 0);
        //prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prDecPrm->u1FbgId);
        //prDecPrm->u4LineSize = u4FbWidth;
        //prDecPrm->u1FbgType = u1FbgType;
    }
    else
    {
        /*
        if (u1FbgType != prDecPrm->u1FbgType)
        {
            LOG(3, "Fbg Type Change %d\n", u1FbgType);
            FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, 0);

            FBM_WaitUnlockFrameBuffer(prDecPrm->u1FbgId,20);    // block until display unlock

            FBM_ReleaseGroup(prDecPrm->u1FbgId);
            prDecPrm->u1FbgId = FBM_FBG_ID_UNKNOWN;

            prDecPrm->u1FbgId = FBM_CreateGroup(u1FbgType, FBM_VDEC_MPEG4, 0, 0);
            ASSERT(prDecPrm->u1FbgId != FBM_FBG_ID_UNKNOWN);
            prVdecEsInfo->ucFbgId = prDecPrm->u1FbgId;
            FBM_SetSyncStc(prDecPrm->u1FbgId, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId);
            FBM_GetFrameBufferSize(prDecPrm->u1FbgId, &u4FbWidth, &u4FbHeight);
              FBM_FbgChgNotify(prDecPrm->u1FbgId, 0);
              prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prDecPrm->u1FbgId);
              prDecPrm->u4LineSize = u4FbWidth;
        }
        */
    }
    //ucFbgType = FBM_SelectGroupType(1280, 720);
    //rWMVDecInfo.rOther.u1FbgId = FBM_CreateGroup(ucFbgType, 2,0,0);
    //FBM_FbgChgNotify(rWMVDecInfo.rOther.u1FbgId, 0);
#endif
    vInitWMVMiscMem();
}

VOID vInitWMVMiscMem(void)
{
    UINT32 u4PPSize = (((1920/16)*9*8)*2);
    UINT32 u4DCACSize = 7*16*(1920/16);  //(1920*4);  new mode uses larger!!!
    UINT32 u4MV1Size = (1920/16)*(1088/16)*16;
    UINT32 u4MV23Size = (1920);
    UINT32 u4BitplaneSize = (1088);
    UINT32 u4WorkingSize;
    UINT32 u4WorkingBuffer;

#ifdef USE_FBM
    FBM_GetWorkingBuffer(rWMVDecInfo.rOther.u1FbgId,&u4WorkingBuffer,&u4WorkingSize);
    ASSERT(u4WorkingSize > 0);
    //u4WorkingBuffer = DCAC_SA;  5365/5395 emulation test code, only for reference
    rWMVDecInfo.rDram.pu1Dcac = (UINT8*)u4WorkingBuffer;
    rWMVDecInfo.rDram.pu1Mv_1 = rWMVDecInfo.rDram.pu1Dcac + u4DCACSize;
    rWMVDecInfo.rDram.pu1Mv_2 = rWMVDecInfo.rDram.pu1Mv_1 + u4MV1Size;
    rWMVDecInfo.rDram.pu1Bp_1 = rWMVDecInfo.rDram.pu1Mv_2 + u4MV23Size;
    rWMVDecInfo.rDram.pu1Bp_2 = rWMVDecInfo.rDram.pu1Bp_1 + u4BitplaneSize;
    rWMVDecInfo.rDram.pu1Bp_3 = rWMVDecInfo.rDram.pu1Bp_2 + u4BitplaneSize;
    rWMVDecInfo.rDram.pu1Bp_4 = rWMVDecInfo.rDram.pu1Bp_3 + u4BitplaneSize;
    rWMVDecInfo.rDram.pu1Mv_3 = rWMVDecInfo.rDram.pu1Bp_4 + u4BitplaneSize;
    rWMVDecInfo.rDram.pu1Mv_1_2 = rWMVDecInfo.rDram.pu1Mv_3 + u4MV23Size;
    rWMVDecInfo.rDram.pu1Dcac_2 = rWMVDecInfo.rDram.pu1Mv_1_2 + u4MV1Size;
    rWMVDecInfo.rDram.pu1Pp_1 = rWMVDecInfo.rDram.pu1Dcac_2 + u4DCACSize;
    rWMVDecInfo.rDram.pu1Pp_2 = rWMVDecInfo.rDram.pu1Pp_1 + u4PPSize;
    if (u4WorkingSize > 0)
    {
        ASSERT((UINT32)rWMVDecInfo.rDram.pu1Pp_2 - u4WorkingBuffer + u4PPSize <= u4WorkingSize);
    }
#else
#ifdef CC_WMV_EMULATION
    u4WorkingBuffer = DCAC_SA;
    rWMVDecInfo.rDram.pu1Dcac = (UINT8*)u4WorkingBuffer;
    rWMVDecInfo.rDram.pu1Mv_1 = rWMVDecInfo.rDram.pu1Dcac + u4DCACSize;
    rWMVDecInfo.rDram.pu1Mv_2 = rWMVDecInfo.rDram.pu1Mv_1 + u4MV1Size;
    rWMVDecInfo.rDram.pu1Bp_1 = rWMVDecInfo.rDram.pu1Mv_2 + u4MV23Size;
    rWMVDecInfo.rDram.pu1Bp_2 = rWMVDecInfo.rDram.pu1Bp_1 + u4BitplaneSize;
    rWMVDecInfo.rDram.pu1Bp_3 = rWMVDecInfo.rDram.pu1Bp_2 + u4BitplaneSize;
    rWMVDecInfo.rDram.pu1Bp_4 = rWMVDecInfo.rDram.pu1Bp_3 + u4BitplaneSize;
    rWMVDecInfo.rDram.pu1Mv_3 = rWMVDecInfo.rDram.pu1Bp_4 + u4BitplaneSize;
    rWMVDecInfo.rDram.pu1Mv_1_2 = rWMVDecInfo.rDram.pu1Mv_3 + u4MV23Size;
    rWMVDecInfo.rDram.pu1Dcac_2 = rWMVDecInfo.rDram.pu1Mv_1_2 + u4MV1Size;
    rWMVDecInfo.rDram.pu1Pp_1 = rWMVDecInfo.rDram.pu1Dcac_2 + u4DCACSize;
    rWMVDecInfo.rDram.pu1Pp_2 = rWMVDecInfo.rDram.pu1Pp_1 + u4PPSize;
    //rWMVDecInfo.rDram.pu1Dcac = (UINT8 *)DCAC_TEST_SA;
#else
    rWMVDecInfo.rDram.pu1Dcac = (UINT8 *) BSP_AllocAlignedDmaMemory(u4DCACSize,16);//BSP_AllocAlignedDmaMemory(0x1f00,16);
    rWMVDecInfo.rDram.pu1Mv_1 = (UINT8 *) BSP_AllocAlignedDmaMemory(u4MV1Size,16);
    rWMVDecInfo.rDram.pu1Mv_2 = (UINT8 *) BSP_AllocAlignedDmaMemory(u4MV23Size,16);
    rWMVDecInfo.rDram.pu1Bp_1 = (UINT8 *) BSP_AllocAlignedDmaMemory(u4BitplaneSize,16);
    rWMVDecInfo.rDram.pu1Bp_2 = (UINT8 *) BSP_AllocAlignedDmaMemory(u4BitplaneSize,16);
    rWMVDecInfo.rDram.pu1Bp_3 = (UINT8 *) BSP_AllocAlignedDmaMemory(u4BitplaneSize,16);
    rWMVDecInfo.rDram.pu1Bp_4 = (UINT8 *) BSP_AllocAlignedDmaMemory(u4BitplaneSize,16);
    rWMVDecInfo.rDram.pu1Mv_3 = (UINT8 *) BSP_AllocAlignedDmaMemory(u4MV23Size,16);
    rWMVDecInfo.rDram.pu1Mv_1_2 = (UINT8 *) BSP_AllocAlignedDmaMemory(u4MV23Size,16);
    rWMVDecInfo.rDram.pu1Dcac_2 = (UINT8 *) BSP_AllocAlignedDmaMemory(u4DCACSize,16);
    rWMVDecInfo.rDram.pu1Pp_1 = (UINT8*) BSP_AllocAlignedDmaMemory(u4PPSize,16);
    rWMVDecInfo.rDram.pu1Pp_2 = (UINT8*) BSP_AllocAlignedDmaMemory(u4PPSize,16);
#endif
#endif

    //rWMVDecInfo.rDram.pu1Pp_1 = (UINT8*) BSP_AllocAlignedDmaMemory(u4PPSize,16);
    //rWMVDecInfo.rDram.pu1Pp_2 = (UINT8*) BSP_AllocAlignedDmaMemory(u4PPSize,16);

    vInitPred();
}

VOID vInitVld(void)
{
    if (rWMVDecInfo.rOther.i4CodecVersion == WMVA)
    {
        //Robert: Enable 0x03 replancement before initializing barrel shifter.
        WriteREG(RW_VLD_WMV_ABS, 0x1);
    }
    else
    {
        WriteREG(RW_VLD_WMV_ABS, 0x0);
    }
    WriteREG(RW_VLD_VSTART, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1VFifo)>>6); // (128-bit address >> 6)
    //spork
    WriteREG(RW_VLD_VEND, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1FifoEnd)>>6);
    fgVldInitBarrelShifter((UINT32)rWMVDecInfo.rDram.pu1VFifo);

    // ilii, clean register
    WriteREG(RW_VLD_DECSTART, 1<<8);
    WriteREG(RW_VLD_DECSTART, 0);
    WriteREG(RW_VLD_VDOUFM, 1);
}

VOID vInitPred(void)
{
    UINT32 a194, a198, a201, a202;

    WriteREG(RW_VLD_DCACSA, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Dcac) >> 2); //192 DCAC
    a194 = (((UINT32)rWMVDecInfo.rDram.pu1Mv_1 >> 2 ) & 0x003FFFFF);
    WriteREG(RW_VLD_MVSA, PHYSICAL(a194)); //194 Mv_1#1(frame/top field)
    a202 = (((UINT32)rWMVDecInfo.rDram.pu1Mv_1 >> 24) & 0x3F);
    WriteREG(VLD_REG_202, PHYSICAL(a202)); //202
    // ilii

    //MPEG-4 use    WriteREG(RW_VLD_BMB1, 0x2d124f80); //196 set once
    //MPEG-4 use    WriteREG(RW_VLD_BMB2, 0x2d13d620); //197 set once
    a198 = (((UINT32)rWMVDecInfo.rDram.pu1Bp_1 >> 2) & 0x00FFFFFF)
           + (((UINT32)rWMVDecInfo.rDram.pu1Bp_1 << 2) & 0xF0000000)
           + (rWMVDecInfo.rOther.i4pred_use_wdle << 25) + //wdle
           ( 1 << 26);
    WriteREG(RW_VLD_BCODE_SA, a198); //(UINT32)rWMVDecInfo.rDram.pu1Bp_1 >> 3); //198 Bp_1
    // ilii
    //MPEG-4 use    WriteREG(RW_VLD_DIRE_MD_IL, 0x00000000); //199 set once
    a201 =0x00900002 + (1 << 16) + //ming add for test
          (((rWMVDecInfo.rSeqHdrR.u4NumMBX == 1) ? 1 : 0) << 22);/* 0x00100000 +
        (rWMVDecInfo.rOther.i4pred_use_wdle << 20)+
        (rWMVDecInfo.rOther.i4pred_use_wdle << 1 );*/
    if (rWMVDecInfo.rOther.i4CodecVersion == WMV1
            || rWMVDecInfo.rOther.i4CodecVersion == WMV2)
    {
        LOG(6, "setting a201 for wmv7 and wmv8\n");
        a201 += ((1<<28) + (1<<29)); //turn of the wmv7,wmv8 timeout bug
    }
    WriteREG(RW_VLD_MBDRAM_SEL, a201); //201 set once
    WriteREG(RW_VLD_MV3_ADDR, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Mv_3 >> 2)); //214 Mv_3 for new RTL
    WriteREG(RW_VLD_BP2_ADDR, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Bp_2 >> 2)); //203 Bp_2
    WriteREG(RW_VLD_BP3_ADDR, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Bp_3 >> 2)); //204 Bp_3
    WriteREG(RW_VLD_BP4_ADDR, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Bp_4 >> 2)); //205 Bp_4
    WriteREG(RW_VLD_MV2_ADDR, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Mv_2 >> 2)); //211 Mv_2
    WriteREG(RW_VLD_DCAC2_ADDR, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Dcac_2 >> 2)); //213 Dcac2
    //WriteREG(RW_VLD_MV2_ADDR, 0x00011170); //211 Mv_2
    //WriteREG(RW_VLD_MVSA, 0x00013880); //194 Mv_1#2(bottom field)

    //MC post process
    WriteREG(RW_MC_PP_DBLK_Y_ADDR, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Pp_1 >> 4));
    WriteREG(RW_MC_PP_DBLK_C_ADDR, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Pp_2 >> 4));
}

VOID vResetVDec(void)
{
#ifdef WAIT_SRAM_STABLE
    UINT32 u4Tmp = 0;
    while ((ReadREG(RO_VLD_SRAMCTRL) & AA_FIT_TARGET_D)==0)
    {
        u4Tmp++;
        if (u4Tmp >= 0x1000)
        {
            //indicate that this is first time hardware reset
            WriteREG(WO_VLD_SRST, 1);
            WriteREG(WO_VLD_SRST, 0);
            //ASSERT(0);
        }
    }
#endif
    WriteREG(WO_VLD_SRST, 1);
    WriteREG(WO_VLD_SRST, 0);
}

VOID vSetVldVFifo(void)
{
    if (rWMVDecInfo.rOther.i4CodecVersion == WMVA)
    {
        //Robert: Enable 0x03 replancement before initializing barrel shifter.
        WriteREG(RW_VLD_WMV_ABS, 0x1);
        //cbZeroThree->Checked = true;
    }
    else
    {
        WriteREG(RW_VLD_WMV_ABS, 0x0);
        //cbZeroThree->Checked = false;
    }
    WriteREG(RW_VLD_VSTART, (UINT32)rWMVDecInfo.rDram.pu1VFifo >> 6);
    //  WriteREG(RW_VLD_VEND, (UINT32)(rWMVDecInfo.rDram.pu1VFifo + V_FIFO_SZ) >> 6);
    //WriteREG(RW_VLD_VEND, (UINT32)0xffffffff);
    //spork
    WriteREG(RW_VLD_VEND, (UINT32)rWMVDecInfo.rDram.pu1FifoEnd>>6);

    //ilii, refer to Video Fifo setting, fifo should be ring, should be implement in driver implement.
    //rWMVDecInfo.rOther.u4WPtr = 0xFFFFFFFF;
    //WriteREG(WO_VLD_VWPTR, (UINT32)(0xFFFFFFFF)); // max wptr
    //rWMVDecInfo.rOther.u4Datain = ReadREG(RO_VLD_VWPTR); // max wptr
}

VOID vSetMcBufPtr(void)
{
    WriteREG(RW_MC_R1Y, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Pic0Y) >> 9); // div 512
    WriteREG(RW_MC_R1C, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Pic0C) >> 8); // div 256
    WriteREG(RW_MC_R2Y, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Pic1Y) >> 9); // div 512
    WriteREG(RW_MC_R2C, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Pic1C) >> 8); // div 256
    WriteREG(RW_MC_BY, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Pic2Y)  >> 8); // div 256
    WriteREG(RW_MC_BC, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Pic2C)  >> 7); // div 128
    WriteREG(RW_MC_BY1, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Pic2Y) >> 9); // div 256
    WriteREG(RW_MC_BC1, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Pic2C) >> 8); // div 128
}

VOID vSetVSyncPrmBufPtr(UINT32 dwBufIdx)
{
    UCHAR ucFbId =0 ;
    FBM_PIC_HDR_T * prFbmPicHdr = NULL;
    switch (dwBufIdx)
    {
    case 0:
        rWMVDecInfo.rOther.rVSyncPrm.dwPtrY = (UINT32)rWMVDecInfo.rDram.pu1Pic0Y;
        rWMVDecInfo.rOther.rVSyncPrm.dwPtrC = (UINT32)rWMVDecInfo.rDram.pu1Pic0C;
        ucFbId = rWMVDecInfo.rOther.u1FbId0;
        break;
    case 1:
        rWMVDecInfo.rOther.rVSyncPrm.dwPtrY = (UINT32)rWMVDecInfo.rDram.pu1Pic1Y;
        rWMVDecInfo.rOther.rVSyncPrm.dwPtrC = (UINT32)rWMVDecInfo.rDram.pu1Pic1C;
        ucFbId = rWMVDecInfo.rOther.u1FbId1;
        break;
    case 2:
        rWMVDecInfo.rOther.rVSyncPrm.dwPtrY = (UINT32)rWMVDecInfo.rDram.pu1Pic2Y;
        rWMVDecInfo.rOther.rVSyncPrm.dwPtrC = (UINT32)rWMVDecInfo.rDram.pu1Pic2C;
        ucFbId = rWMVDecInfo.rOther.u1FbId2;
        break;
    }

//#ifdef USE_FBM  // megaa 20100113 ToDo: check the matching
    if ((rWMVDecInfo.rPicLayer.u1FrameCodingMode != INTERLACEFIELD) || (rWMVDecInfo.rPicLayer.i4CurrentTemporalField == 1)) {
        if ((rWMVDecInfo.rPicLayer.u1PicType == BVOP) || (rWMVDecInfo.rPicLayer.u1PicType == BIVOP)) //prog_B
        {
            BOOL fgNotDisplay;
            //FBM_PIC_HDR_T * prFbmPicHdr;
            VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(0);

#ifdef USE_FBM  // megaa 20100113 ToDo: check the matching
            prFbmPicHdr = FBM_GetFrameBufferPicHdr(rWMVDecInfo.rOther.u1FbgId, ucFbId);


            if (prFbmPicHdr)
            {
                if (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_FORWARD_1X)
                {
                    if (rWMVDecInfo.rOther.fgFirstDispFrame)
                    {
                        rWMVDecInfo.rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                        rWMVDecInfo.rOther.fgFirstDispFrame = FALSE;
                    }
                    else
                    {
                        if (rWMVDecInfo.rOther.i4CodecVersion != WMVA)
                        {
                            rWMVDecInfo.rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                        }
                        else
                        {
                            if (prVdecEsInfo->eContainerType != SWDMX_FMT_MPEG2_TS
                                    && prVdecEsInfo->eContainerType != SWDMX_FMT_MKV
                                    && prVdecEsInfo->eContainerType != SWDMX_FMT_VC1_ES
                                    && prVdecEsInfo->eContainerType != SWDMX_FMT_MPEG2_TS_192
                                    && prVdecEsInfo->eContainerType != SWDMX_FMT_MPEG2_TS_ZERO_192
                                    && prVdecEsInfo->eContainerType != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                            {
                                UINT32 u4HalfDeltaPTS = rWMVDecInfo.rOther.u4DeltaPTS>>1;
                                if ((prFbmPicHdr->u4PTS <= (rWMVDecInfo.rOther.u4LastPTS+rWMVDecInfo.rOther.u4DeltaPTS+u4HalfDeltaPTS))
                                        && prFbmPicHdr->u4PTS >= (rWMVDecInfo.rOther.u4LastPTS + u4HalfDeltaPTS))
                                {
                                    rWMVDecInfo.rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                                }
                                else
                                {
                                    prFbmPicHdr->u4PTS = rWMVDecInfo.rOther.u4LastPTS + rWMVDecInfo.rOther.u4DeltaPTS;
                                    rWMVDecInfo.rOther.u4LastPTS += rWMVDecInfo.rOther.u4DeltaPTS;
                                }
                            }
                            else
                            {
                                if (!rWMVDecInfo.rPesInfo.fgDtsValid)
                                {
                                    prFbmPicHdr->u4PTS = rWMVDecInfo.rOther.u4LastPTS + rWMVDecInfo.rOther.u4DeltaPTS;
                                    rWMVDecInfo.rOther.u4LastPTS += rWMVDecInfo.rOther.u4DeltaPTS;
                                }
                                else
                                {
                                    rWMVDecInfo.rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                                }
                            }
                        }
                    }
                    prFbmPicHdr->i4TemporalRef = rWMVDecInfo.rPicLayer.i4TemporalRef;
                    LOG(5,"Vdec WMV B Frame PTS : %8x, temporal ref : %d\n",prFbmPicHdr->u4PTS,prFbmPicHdr->i4TemporalRef);
                    rWMVDecInfo.rPicLayer.i4TemporalRef++;
                }
//#endif  // megaa 20100113 ToDo: check the matching
                fgNotDisplay = VDEC_IsNotDisplay(0, prFbmPicHdr->u4PTS,
                                                 prFbmPicHdr->i4TemporalRef, prFbmPicHdr->u8Offset);
            }
            else
#endif  // megaa 20100113 ToDo: check the matching
            {
                fgNotDisplay = VDEC_IsNotDisplay(0, 0, 0, 0);
            }

            //fgNotDisplay = VDEC_IsNotDisplay(0, 0, 0, 0);

            if (fgNotDisplay || rWMVDecInfo.rOther.fgSkipFrame)
            {
#ifdef WMV_PRINT_FBM_STATUS
                UCHAR u1state;
                u1state = FBM_GetFrameBufferStatus(rWMVDecInfo.rOther.u1FbgId,ucFbId);
                LOG(0,"WMV set B frame FBM status : %d %d EMPTY\n",ucFbId,u1state);
#endif
                rWMVDecInfo.rOther.fgSkipFrame = FALSE;
                FBM_SetFrameBufferStatus(rWMVDecInfo.rOther.u1FbgId, ucFbId, FBM_FB_STATUS_EMPTY);
            }
            else
            {
#ifdef WMV_PRINT_FBM_STATUS
                UCHAR u1state;
                u1state = FBM_GetFrameBufferStatus(rWMVDecInfo.rOther.u1FbgId,ucFbId);
                LOG(0,"WMV set B frame FBM status : %d %d DISPALY_Q\n",ucFbId,u1state);
#endif
                FBM_SetFrameBufferStatus(rWMVDecInfo.rOther.u1FbgId, ucFbId, FBM_FB_STATUS_DISPLAYQ);
                prVdecEsInfo->u4DisplayQPicCnt++;
            }
            if (prFbmPicHdr)
            {
                VDEC_CheckStopStatus(0, prFbmPicHdr->u4PTS, prFbmPicHdr->i4TemporalRef );
            }
            else
            {
                VDEC_CheckStopStatus(0, 0, 0);
            }

        }
        /*else if (rWMVDecInfo.rPicLayer.u1PicType == SKIPFRAME)
        {
            FBM_SetFrameBufferStatus(rWMVDecInfo.rOther.u1FbgId, ucFbId,
                                     FBM_FB_STATUS_EMPTY);
        }*/
        else
        {
            FBM_SetFrameBufferStatus(rWMVDecInfo.rOther.u1FbgId, ucFbId,
                                     FBM_FB_STATUS_READY);
        }
    }
}

VOID vVSyncWriteVdoReg(void)
{
    WriteREG(RW_VDO_PTRY, PHYSICAL(rWMVDecInfo.rOther.rVSyncPrm.dwPtrY >> 3));
    WriteREG(RW_VDO_PTRC, PHYSICAL(rWMVDecInfo.rOther.rVSyncPrm.dwPtrC >> 3));
}

VOID _VDEC_UpdateVldWMVWPtr(UINT32 u4WPtr)
{
    //UINT32 u4RegVal;

    UINT32 u4WPtrAlign, u4VStart, u4VEnd;

    u4WPtrAlign = ((u4WPtr + RPTR_ALIGN) & (~((UINT32)RPTR_ALIGN - 1))) + WPTR_OFFSET;

    u4VStart = ReadREG(RW_VLD_VSTART)<<6;
    u4VEnd = ReadREG(RW_VLD_VEND)<<6;

    if (u4WPtrAlign < u4VStart)
    {
        u4WPtrAlign = u4VEnd - (u4VStart - u4WPtrAlign);
    }
    if (u4WPtrAlign > u4VEnd)
    {
        u4WPtrAlign = u4VStart + (u4WPtrAlign - u4VEnd);
    }
#if defined(CC_MT5392B) || defined(CC_MT5365) || defined(CC_MT5395)
    WriteREG(WO_VLD_VWPTR,u4WPtrAlign);
#else
    //u4RegVal = (ReadREG(WO_VLD_VWPTR) & 0x7)|(u4WPtr << 3);
    WriteREG(WO_VLD_VWPTR, (u4WPtrAlign << 3) | 7);    //set wptr infinity
#endif


}

UINT32 dwVldRPtr(UINT32 *pdwBits)
{

    UINT32 dwDramRptr;
    UINT32 dwSramRptr, dwSramWptr;
    UINT32 dwSramDataSz;
    UINT32 dwByteAddr;

#ifdef WAIT_SRAM_STABLE
    UINT32 u4Tmp = 0;
    while ((ReadREG(RO_VLD_SRAMCTRL) & AA_FIT_TARGET_D)==0)
    {
        u4Tmp++;
        if (u4Tmp >= 0x10000)
        {
            LOG(0, "//===SRAM !Stable 0xF4=0x%x, Rptr 0xB0=0x%x, 0xFC=0x%x, 0xEC=0x%x, 0xE8=0x%x\n",
                ReadREG(RO_VLD_SRAMCTRL), ReadREG(RW_VLD_RPTR), ReadREG(RO_VLD_VRPTR),
                ReadREG(RO_VLD_VBAR), ReadREG(RO_VLD_FETCHOK));
            return FALSE;
        }
    }
#endif

    dwDramRptr = ReadREG(RO_VLD_VRPTR);// & 0xffffff;
#if defined(CC_MT5365) || defined(CC_MT5395)
    dwSramRptr = dwSramWptr = ReadREG(RO_VLD_VBAR);
    dwSramRptr = (UINT32)((((dwSramRptr & 0xf) * 4) + ((dwSramRptr >> 24) & 0x3)));
    dwSramWptr = (UINT32)((((dwSramWptr >> 8) & 0xf) * 4));
#else
    dwSramRptr = ReadREG(RO_VLD_VBAR)&0x3f;// & 0xff;
    dwSramWptr = (ReadREG(RO_VLD_VBAR) >> 16)&0x3f;// & 0xff;
#endif

    /*if(!fgWaitVldFetchOk())
     {
     LOG(3,"VLD RPtr wait vld not ok\n");
     }*/

    //dwDramRptr = ReadREG(RO_VLD_VRPTR);// & 0xffffff;
    //dwSramRptr = ReadREG(RO_VLD_VBAR)&0x3f;// & 0xff;
    //dwSramWptr = (ReadREG(RO_VLD_VBAR) >> 16)&0x3f;// & 0xff;

    if (dwSramWptr > dwSramRptr)
    {
        dwSramDataSz = dwSramWptr - dwSramRptr;
    }
    else
    {
        dwSramDataSz = 64 - (dwSramRptr - dwSramWptr);
    }

    *pdwBits = ReadREG(RW_VLD_BITCOUNT) & 0x3f;
    //*pdwBits = (ReadREG(RW_VLD_BITCOUNT)>>16) & 0x3f;

    if (ReadREG(RW_VLD_WMV_ABS) & 0x1) // 03 replacement enable!
    {
        dwByteAddr = dwDramRptr - (dwSramDataSz + 4) * 4 + (*pdwBits / 8);
    }
    else
    {
        dwByteAddr = dwDramRptr - (dwSramDataSz + 3)* 4 + (*pdwBits / 8);
    }
    //spork
    *pdwBits &= 0x7;
    dwByteAddr -= ((UINT32)rWMVDecInfo.rDram.pu1VFifo);
    return (dwByteAddr);
}

void WriteDram64b(UINT32 dwAddr, UINT32 dw64HiDW, UINT32 dw64LoDW)
{
    //UINT32 *pAddr = (UINT32*)dwAddr;
    //VIRTUAL(pAddr[0]) = dw64LoDW;
    x_memcpy((VOID*)(VIRTUAL(dwAddr)),(VOID*)(VIRTUAL(&dw64LoDW)),4);
    //x_memset(VIRTUAL(pAddr[0]),)
}
void WriteDram8b(UINT32 dwAddr, UINT32 dw64HiDW, UINT32 dw64LoDW)
{
    //UINT8 *pAddr = (UINT8*)dwAddr;
    //VIRTUAL(pAddr[0]) = (UINT8)dw64LoDW;
    x_memcpy((VOID*)(VIRTUAL(dwAddr)),(VOID*)(VIRTUAL(&dw64LoDW)),1);
}

//copy from jup.c

/* Intensity compensation */
void cal_icomp(int *iScale, int *iShift, int m_iLuminanceScale,
               int m_iLuminanceShift)
{
    /* derived from interpolate_wmv9.c IntensityCompensation() */
    if (m_iLuminanceShift > 31)
        m_iLuminanceShift -= 64;

    // remap luminance scale and shift
    if (m_iLuminanceScale == 0)
    {
        *iScale = - 64;
        *iShift = 255 * 64 - m_iLuminanceShift * 2 * 64;
    }
    else
    {
        *iScale = m_iLuminanceScale + 32;
        *iShift = m_iLuminanceShift * 64;
    }
}

/*****************************************/
/*  NAME: print_vop_header_parameter     */
/*  Description: to print picture-layer  */
/*           information */
/*****************************************/

void print_vop_header_parameter(/*int umv_from_mb,*/int vopnum, int fref_ctrl,
        int out_ctrl)
{

    //Printf("entering vop header parameter function\n");
    int iScale, iShift;
    long    a34, a36, a37, a38, a39, a42, a35, a131, a132, a133, a136, a137, a138,
       a140, a142, a195, b_fraction_chk, a193, a198, a200, a206;
    int tmp, TRB, TRD;
    unsigned long high_rate_switch;
    //int dump_size;
    int use_rule_b, ttfrm, use_quarter_pel_mv, use_interpredictor;
    int height; //for interlace_field
    int i;
    int vop_type;
    int iScaleTop, iShiftTop;
    int iScaleBot, iShiftBot;
    static UINT8 _bPreProcessFrameStatus;
    //int u4MC_DBK;
    //ilii
    rWMVDecInfo.rOther.i4pred_use_wdle = 1;//0;
    //_fgPrintWReg = TRUE;
    //_fgRealWReg = (cbWMVFWBP) ? FALSE : TRUE;
    //vop_type = (rWMVDecInfo.rPicLayer.u1PicType == PVOP) ? PVOP: (rWMVDecInfo.rPicLayer.u1PicType == BVOP)? BVOP : IVOP; //VLD view
    if (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
    {
        vop_type = PVOP;
    }
    else if (rWMVDecInfo.rPicLayer.u1PicType == BVOP)
    {
        vop_type = BVOP;
    }
    else
    {
        vop_type = IVOP;
    }
    //LOG(0,"entering print_vop_header_parameter\n\n");
    //LOG(0,"entering print_vop_header_parameter\n\n");
    //LOG(0,"entering print_vop_header_parameter\n\n");
    //LOG(0,"entering print_vop_header_parameter\n\n");
    //Printf("vop type : %d\n",vop_type);
    if (((rWMVDecInfo.rPicLayer.u1PicType == IVOP)
            || (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
            || (rWMVDecInfo.rPicLayer.u1PicType == SKIPFRAME)) && (/* first field picture */
                ((rWMVDecInfo.rPicLayer.fgFieldMode == TRUE)
                 && (rWMVDecInfo.rPicLayer.i4CurrentTemporalField== 0)) ||
                /* frame picture */
                (rWMVDecInfo.rPicLayer.fgFieldMode == FALSE) ))
    {
        rWMVDecInfo.rDram.u4ForwardRefPicType
        = rWMVDecInfo.rDram.u4BackwardRefPicType;

        if ((rWMVDecInfo.rOther.i4CodecVersion == WMVA)
                && (rWMVDecInfo.rPicLayer.fgInterlaceV2))
        {
            if (rWMVDecInfo.rPicLayer.fgFieldMode == TRUE)
                rWMVDecInfo.rDram.u4BackwardRefPicType = INTERLACEFIELD;
            else
                rWMVDecInfo.rDram.u4BackwardRefPicType = INTERLACEFRAME;
        }
        else
        {
            rWMVDecInfo.rDram.u4BackwardRefPicType = PROGRESSIVE;
        }
    }

    if ((rWMVDecInfo.rSMProfile.fgXintra8) && (rWMVDecInfo.rPicLayer.u1PicType
            == IVOP))
    {
        return;
    }

    //print pattern for fpga
    //-------------------------------------------------
    // global setting 只要一開始設一次即可
    //-------------------------------------------------
    /* vSetMcBufPtr() will set the following registers
     WriteREG(RW_MC_R1Y, ((UINT32)(rWMVDecInfo.rDram.pu1Pic0Y) >> _iMemBase) >> 9); //ref_pic1_y_base_addr
     WriteREG(RW_MC_R1C, ((UINT32)(rWMVDecInfo.rDram.pu1Pic0C) >> _iMemBase) >> 8); //ref_pic1_c_base_addr
     WriteREG(RW_MC_R2Y, ((UINT32)(rWMVDecInfo.rDram.pu1Pic1Y) >> _iMemBase) >> 9); //ref_pic2_y_base_addr
     WriteREG(RW_MC_R2C, ((UINT32)(rWMVDecInfo.rDram.pu1Pic1C) >> _iMemBase) >> 8); //ref_pic2_y_base_addr
     WriteREG(RW_MC_BY1, ((UINT32)(rWMVDecInfo.rDram.pu1Pic2Y) >> _iMemBase) >> 9); //prog_y_base_addr
     WriteREG(RW_MC_BC1, ((UINT32)(rWMVDecInfo.rDram.pu1Pic2C) >> _iMemBase) >> 8); //prog_c_base_addr
     */
    //  WriteREG(RW_MC_2FLDMD, 0x00000000);
    WriteREG(RW_MC_QUARTER_SAMPLE, 0x00000001); //always set to 1 for WMV case
    WriteREG(RW_MC_PP_DBLK_THD, 0x000f0606); //always set to this value for WMV case
    //  WriteREG(RW_MC_PP_DBLK_Y_ADDR, DEC_PP_1);
    //  WriteREG(RW_MC_PP_DBLK_C_ADDR, DEC_PP_2);

    //spork
    //WriteREG(RW_VLD_SRAM_TEST_WRITE_ADDR, 200); //ming add for debug 01/03

    //  WriteREG(0x50c, 0xe00); //err_conceal off
    //WriteREG(0xD7c, 0x1); //turn off the ext_sram
    //WriteREG(0x7f8, 0x1); //turn on check sum shift mode
    //WriteREG(VLD_REG_OFST + 0x258, 1000);

    //------------------------------
    //   MC RISC WRITE PATTERN
    //------------------------------
    //----------------------------------------------------------
    // MC_reg_9 : MC output buffer setting
    //            (0:ref_pic1_buf  1:ref_pic2_buf  4:prg_B_pic1)
    //----------------------------------------------------------
    if ((rWMVDecInfo.rPicLayer.u1PicType == BVOP)
            || (rWMVDecInfo.rPicLayer.u1PicType == BIVOP))
    {
        WriteREG(RW_MC_OPBUF, 0x00000004);
        WriteREG(RW_MC_FWDP, fref_ctrl);
    }
    else //I, P
    {
        WriteREG(RW_MC_OPBUF, out_ctrl);
        WriteREG(RW_MC_FWDP, fref_ctrl);
    }

    //----------------------------------------------------------
    // MC_reg_8 : indicate if this is the 2nd field picture
    //        (0:1st field picture or frame picture
    //         1:2nd field picture)
    //----------------------------------------------------------
    WriteREG(RW_MC_2FLDMD, rWMVDecInfo.rPicLayer.i4CurrentTemporalField);

    //----------------------------------------------------------
    // MC_reg_180 : mixed mv resolution (1: turn on)
    //----------------------------------------------------------
    WriteREG(RW_MC_WMV8_MIX_PEL, rWMVDecInfo.rPicLayer.fgMvResolution);

    //----------------------------------------------------------
    // MC_reg_127 : quarter pel type (set 0:TYPE_14496)
    //----------------------------------------------------------
    WriteREG(RW_MC_QPEL_TYPE, 0x00000000); //always set to 0 for WMV

    //----------------------------------------------------------
    //MC_reg_129 : rounding_control (1:turn on)
    //----------------------------------------------------------
    WriteREG(RW_MC_ROUNDING_CTRL, rWMVDecInfo.rPicLayer.i4RndCtrl);

    //--------------------------------------------------------------------
    // MC_reg_130, 131 : UMV Padding boundary (取16的倍數, 或實際pic大小)
    //--------------------------------------------------------------------
    if (rWMVDecInfo.rOther.i4CodecVersion <= WMV3)
    {
        WriteREG(RW_MC_UMV_PIC_WIDTH, rWMVDecInfo.rSeqHdrR.u4PicWidthDec);
        WriteREG(RW_MC_UMV_PIC_HEIGHT, rWMVDecInfo.rSeqHdrR.u4PicHeightDec);
    }
    else /* UMV Padding directly from picture boundary */
    {
        WriteREG(RW_MC_UMV_PIC_WIDTH, rWMVDecInfo.rSeqHdrR.u4PicWidthSrc);
        WriteREG(RW_MC_UMV_PIC_HEIGHT, rWMVDecInfo.rSeqHdrR.u4PicHeightSrc);
    }

    //--------------------------------------------------------------------
    // MC_reg_156 : scale type (0:NO_SCALE, 1:SCALE_DOWN, 2:SCALE_UP)
    //--------------------------------------------------------------------
    if (rWMVDecInfo.rPicLayer.u1PicType == IVOP)
    {
        _bPreProcessFrameStatus = NO_SCALE;
        WriteREG(RW_MC_WMV9_PRE_PROC, NO_SCALE);
    }
    else if ( (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
              || (rWMVDecInfo.rPicLayer.u1PicType == SKIPFRAME)) /* only for PVOP */
    {
        if (rWMVDecInfo.rEntryPoint.i4ReconRangeState == 0)//pWMVDec->mrWMVDecInfo.rEntryPoint.i4ReconRangeState == 0)
        {
            if (rWMVDecInfo.rEntryPoint.i4RangeState == 1)
            {
                // JUP comment
                // Previous not scale range
                // Current scale down range
                // Hence, scan DOWN previous frame to using it for current frame's motion compensation
                //
                // reduce by 2
                _bPreProcessFrameStatus = SCALE_DOWN;
                WriteREG(RW_MC_WMV9_PRE_PROC, SCALE_DOWN);
            }
            else
            {
                _bPreProcessFrameStatus = NO_SCALE;
                WriteREG(RW_MC_WMV9_PRE_PROC, NO_SCALE);
            }
        }
        else if (rWMVDecInfo.rEntryPoint.i4ReconRangeState == 1)//pWMVDec->mrWMVDecInfo.rEntryPoint.i4ReconRangeState == 1)
        {
            if (rWMVDecInfo.rEntryPoint.i4RangeState == 0)
            {
                // JUP comment
                // Previous scale down range
                // Current not scale range
                // Hence, scan UP previous frame to using it for current frame's motion compensation
                //
                // increase by 2
                _bPreProcessFrameStatus = SCALE_UP;
                WriteREG(RW_MC_WMV9_PRE_PROC, SCALE_UP);
            }
            else
            {
                _bPreProcessFrameStatus = NO_SCALE;
                WriteREG(RW_MC_WMV9_PRE_PROC, NO_SCALE);
            }
        }
    }
    else// if (rWMVDecInfo.rPicLayer.u1PicType == BVOP) , Ju said both for BIVOP and BVOP
    {
        /* just follow the setting of last decoded P picture */
        WriteREG(RW_MC_WMV9_PRE_PROC, _bPreProcessFrameStatus);
    }

    //
    // Update Icomp parameters for both IVOP and PVOP
    //
    if (rWMVDecInfo.rOther.i4CodecVersion == WMVA)
    {
        if ( (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
                || (rWMVDecInfo.rPicLayer.u1PicType == IVOP)
                || (rWMVDecInfo.rPicLayer.u1PicType == SKIPFRAME))
        {
            if (rWMVDecInfo.rPicLayer.fgFieldMode == TRUE) //field picture
            {
                if (rWMVDecInfo.rPicLayer.i4CurrentTemporalField== 0) // the first P field picture
                {
                    rWMVDecInfo.rOther.i4BoundaryUMVIcomp = 0;

                    /* Step 1: Update Icomp parameters, Old <= New */
                    //rWMVDecInfo.rOther.rNewTopField.Old = rWMVDecInfo.rOther.rNewTopField.New;
                    //rWMVDecInfo.rOther.rNewBotField.Old = rWMVDecInfo.rOther.rNewBotField.New;

                    if (rWMVDecInfo.rPicLayer.i4CurrentField == 0) //current decode field is top field
                    {
                        /* Step 1: Update Icomp parameters     */
                        /* OLD_BF <= NEW_BF                */

                        rWMVDecInfo.rOther.rOldBotField.Old
                        = rWMVDecInfo.rOther.rNewBotField.Old;
                        rWMVDecInfo.rOther.rOldBotField.New
                        = rWMVDecInfo.rOther.rNewBotField.New;

                        /* reset NEW_BF */
                        rWMVDecInfo.rOther.rNewBotField.Old.iEnable = 0;
                        rWMVDecInfo.rOther.rNewBotField.New.iEnable = 0;

                        /* update NEW_TF, OLD <= NEW */
                        rWMVDecInfo.rOther.rNewTopField.Old
                        = rWMVDecInfo.rOther.rNewTopField.New;

                        if ( /* forward reference picture is frame picture */
                            /* and decode the first field picture now */
                            (rWMVDecInfo.rDram.u4ForwardRefPicType == PROGRESSIVE)
                            || (rWMVDecInfo.rDram.u4ForwardRefPicType
                                == INTERLACEFRAME))
                        {
                            rWMVDecInfo.rOther.rNewTopField.Old.iEnable = 0;
                        }

                    }
                    else // current decode field is bottom field
                    {
                        /* Step 1: Update Icomp parameters     */
                        /* OLD_TF <= NEW_TF                */

                        rWMVDecInfo.rOther.rOldTopField.Old
                        = rWMVDecInfo.rOther.rNewTopField.Old;
                        rWMVDecInfo.rOther.rOldTopField.New
                        = rWMVDecInfo.rOther.rNewTopField.New;

                        /* reset NEW_TF */
                        rWMVDecInfo.rOther.rNewTopField.Old.iEnable = 0;
                        rWMVDecInfo.rOther.rNewTopField.New.iEnable = 0;

                        /* update NEW_BF, OLD <= NEW */
                        rWMVDecInfo.rOther.rNewBotField.Old
                        = rWMVDecInfo.rOther.rNewBotField.New;

                        if ( /* forward reference picture is frame picture */
                            /* and decode the first field picture now */
                            (rWMVDecInfo.rDram.u4ForwardRefPicType == PROGRESSIVE)
                            || (rWMVDecInfo.rDram.u4ForwardRefPicType
                                == INTERLACEFRAME))
                        {
                            rWMVDecInfo.rOther.rNewBotField.Old.iEnable = 0;
                        }
                    }

                    /* Step 2: calculate Icomp parameters, New <= Icomp */
                    if (rWMVDecInfo.rOther.fgLuminanceWarpTop == TRUE)
                    {
                        cal_icomp(&iScaleTop, &iShiftTop,
                                  rWMVDecInfo.rOther.i4LumScaleTop,
                                  rWMVDecInfo.rOther.i4LumShiftTop);
                        rWMVDecInfo.rOther.rNewTopField.New.iShift = iShiftTop;
                        rWMVDecInfo.rOther.rNewTopField.New.iScale = iScaleTop;
                        rWMVDecInfo.rOther.rNewTopField.New.iEnable = 1;
                    }
                    else
                    {
                        rWMVDecInfo.rOther.rNewTopField.New.iEnable = 0;
                    }

                    if (rWMVDecInfo.rOther.fgLuminanceWarpBottom == TRUE)
                    {
                        cal_icomp(&iScaleBot, &iShiftBot,
                                  rWMVDecInfo.rOther.i4LumScaleBottom,
                                  rWMVDecInfo.rOther.i4LumShiftBottom);
                        rWMVDecInfo.rOther.rNewBotField.New.iShift = iShiftBot;
                        rWMVDecInfo.rOther.rNewBotField.New.iScale = iScaleBot;
                        rWMVDecInfo.rOther.rNewBotField.New.iEnable = 1;
                    }
                    else
                    {
                        rWMVDecInfo.rOther.rNewBotField.New.iEnable = 0;
                    }
                }
                else // the second P field picture
                {
                    if ( (rWMVDecInfo.rDram.u4ForwardRefPicType == PROGRESSIVE)
                            && (rWMVDecInfo.rDram.u4BackwardRefPicType
                                == INTERLACEFIELD)
                            /* && this is a second field */
                       )
                    {
                        rWMVDecInfo.rOther.i4BoundaryUMVIcomp = 1;
                    }
                    else
                    {
                        rWMVDecInfo.rOther.i4BoundaryUMVIcomp = 0;
                    }

                    rWMVDecInfo.rOther.i4SecondFieldParity
                    = rWMVDecInfo.rPicLayer.i4CurrentField;

                    if (rWMVDecInfo.rPicLayer.i4CurrentField == 0) //current decode field is top field
                    {
                        /* Step 1: Update Icomp parameters     */
                        /* OLD_BF <= NEW_BF  */
                        rWMVDecInfo.rOther.rOldBotField.Old
                        = rWMVDecInfo.rOther.rNewBotField.Old;
                        rWMVDecInfo.rOther.rOldBotField.New
                        = rWMVDecInfo.rOther.rNewBotField.New;

                        /* reset NEW_BF */
                        rWMVDecInfo.rOther.rNewBotField.Old.iEnable = 0;
                        rWMVDecInfo.rOther.rNewBotField.New.iEnable = 0;

                        /* update NEW_TF: Old <= New   */
                        rWMVDecInfo.rOther.rNewTopField.Old
                        = rWMVDecInfo.rOther.rNewTopField.New;
                    }
                    else // current decode field is bottom field
                    {
                        /* Step 1: Update Icomp parameters     */
                        /* OLD_TF <= NEW_TF                */
                        rWMVDecInfo.rOther.rOldTopField.Old
                        = rWMVDecInfo.rOther.rNewTopField.Old;
                        rWMVDecInfo.rOther.rOldTopField.New
                        = rWMVDecInfo.rOther.rNewTopField.New;

                        /* reset NEW_TF */
                        rWMVDecInfo.rOther.rNewTopField.Old.iEnable = 0;
                        rWMVDecInfo.rOther.rNewTopField.New.iEnable = 0;

                        /* update NEW_BF: Old <= New            */
                        rWMVDecInfo.rOther.rNewBotField.Old
                        = rWMVDecInfo.rOther.rNewBotField.New;
                    }

                    /* Step 2: calculate Icomp parameters, both NEW_BF.New amd NEW_TF.New <= Icomp */
                    if (rWMVDecInfo.rOther.fgLuminanceWarpTop == TRUE)
                    {
                        cal_icomp(&iScaleTop, &iShiftTop,
                                  rWMVDecInfo.rOther.i4LumScaleTop,
                                  rWMVDecInfo.rOther.i4LumShiftTop);
                        rWMVDecInfo.rOther.rNewTopField.New.iShift = iShiftTop;
                        rWMVDecInfo.rOther.rNewTopField.New.iScale = iScaleTop;
                        rWMVDecInfo.rOther.rNewTopField.New.iEnable = 1;
                    }
                    else
                    {
                        rWMVDecInfo.rOther.rNewTopField.New.iEnable = 0;
                    }

                    if (rWMVDecInfo.rOther.fgLuminanceWarpBottom == TRUE)
                    {
                        cal_icomp(&iScaleBot, &iShiftBot,
                                  rWMVDecInfo.rOther.i4LumScaleBottom,
                                  rWMVDecInfo.rOther.i4LumShiftBottom);
                        rWMVDecInfo.rOther.rNewBotField.New.iShift = iShiftBot;
                        rWMVDecInfo.rOther.rNewBotField.New.iScale = iScaleBot;
                        rWMVDecInfo.rOther.rNewBotField.New.iEnable = 1;
                    }
                    else
                    {
                        rWMVDecInfo.rOther.rNewBotField.New.iEnable = 0;
                    }
                }
            } //end : if (rWMVDecInfo.rPicLayer.fgFieldMode == TRUE)
            else // frame picture
            {
                rWMVDecInfo.rOther.i4BoundaryUMVIcomp = 0;

                /* Step 1: Update Icomp parameters, both NEW_BF and NEW_TF, Old <= New */
                rWMVDecInfo.rOther.rNewTopField.Old
                = rWMVDecInfo.rOther.rNewTopField.New;
                rWMVDecInfo.rOther.rNewBotField.Old
                = rWMVDecInfo.rOther.rNewBotField.New;

                if ( /* forward reference picture is frame picture */
                    (rWMVDecInfo.rDram.u4ForwardRefPicType == PROGRESSIVE)
                    || (rWMVDecInfo.rDram.u4ForwardRefPicType
                        == INTERLACEFRAME))
                {
                    rWMVDecInfo.rOther.rNewTopField.Old.iEnable = 0;
                    rWMVDecInfo.rOther.rNewBotField.Old.iEnable = 0;
                }
                else /* rWMVDecInfo.rDram.u4ForwardRefPicType == INTERLACE_FIELD */
                {
                    /* previous reference second field is top field */
                    if (rWMVDecInfo.rOther.i4SecondFieldParity == 0)
                    {
                        rWMVDecInfo.rOther.rNewTopField.Old.iEnable = 0;
                    }
                    /* previous reference second field is bottom field */
                    else
                    {
                        rWMVDecInfo.rOther.rNewBotField.Old.iEnable = 0;
                    }
                }

                /* Step 2: calculate Icomp parameters, both NEW_BF and NEW_TF, New <= Icomp */
                if (rWMVDecInfo.rOther.fgLuminanceWarp == TRUE)
                {
                    cal_icomp(&iScaleTop, &iShiftTop,
                              rWMVDecInfo.rOther.i4LumScale,
                              rWMVDecInfo.rOther.i4LumShift);
                    rWMVDecInfo.rOther.rNewTopField.New.iShift = iShiftTop;
                    rWMVDecInfo.rOther.rNewTopField.New.iScale = iScaleTop;
                    rWMVDecInfo.rOther.rNewTopField.New.iEnable = 1;
                }
                else
                {
                    rWMVDecInfo.rOther.rNewTopField.New.iEnable = 0;
                }

                if (rWMVDecInfo.rOther.fgLuminanceWarp == TRUE)
                {
                    cal_icomp(&iScaleBot, &iShiftBot,
                              rWMVDecInfo.rOther.i4LumScale,
                              rWMVDecInfo.rOther.i4LumShift);
                    rWMVDecInfo.rOther.rNewBotField.New.iShift = iShiftBot;
                    rWMVDecInfo.rOther.rNewBotField.New.iScale = iScaleBot;
                    rWMVDecInfo.rOther.rNewBotField.New.iEnable = 1;
                }
                else
                {
                    rWMVDecInfo.rOther.rNewBotField.New.iEnable = 0;
                }
            } //end : frame picture
        } //end : IVOP PVOP SKIPFRAME
    } //end : if (rWMVDecInfo.rOther.i4CodecVersion == WMVA)


    //-------------------------------------------------------
    // intensity compensation
    //
    //------------------------------------------------------

    if ((rWMVDecInfo.rPicLayer.u1PicType == IVOP)
            && (rWMVDecInfo.rOther.i4CodecVersion == WMVA))
    {
        if (write_enable == 1)
        {
            WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_OFF + ICOMP_Y_OFF);
            WriteREG(RW_MC_ICOMP2_EN, 0);
        }
    }
    else if (( (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
               || (rWMVDecInfo.rPicLayer.u1PicType == SKIPFRAME) )
             && (rWMVDecInfo.rOther.i4CodecVersion == WMVA))
    {
        /* Step 3: output MC parameters */
        // icomp_set1 is forward top field new icomp parameters
        if (rWMVDecInfo.rOther.rNewTopField.New.iEnable == 1)
        {
            if (write_enable == 1)
            {
                WriteREG(RW_MC_ISCALE1_X1, (iScaleTop * 1));
                WriteREG(RW_MC_ISCALE1_X3, (iScaleTop * 3));
                WriteREG(RW_MC_ISCALE1_X5, (iScaleTop * 5));
                WriteREG(RW_MC_ISCALE1_X7, (iScaleTop * 7));
                WriteREG(RW_MC_ISCALE1_X9, (iScaleTop * 9));
                WriteREG(RW_MC_ISCALE1_X11, (iScaleTop * 11));
                WriteREG(RW_MC_ISCALE1_X13, (iScaleTop * 13));
                WriteREG(RW_MC_ISCALE1_X15, (iScaleTop * 15));
                WriteREG(RW_MC_YSHIFT_OFF1, (iShiftTop + 32));
                WriteREG(RW_MC_CSHIFT_OFF1, ((128 * 64) + 32 - (128*iScaleTop)));
            }
        }

        // icomp_set2 is forward bottom field new icomp parameters
        if (rWMVDecInfo.rOther.rNewBotField.New.iEnable == 1)
        {
            if (write_enable == 1)
            {
                WriteREG(RW_MC_ISCALE2_X1, (iScaleBot * 1));
                WriteREG(RW_MC_ISCALE2_X3, (iScaleBot * 3));
                WriteREG(RW_MC_ISCALE2_X5, (iScaleBot * 5));
                WriteREG(RW_MC_ISCALE2_X7, (iScaleBot * 7));
                WriteREG(RW_MC_ISCALE2_X9, (iScaleBot * 9));
                WriteREG(RW_MC_ISCALE2_X11, (iScaleBot * 11));
                WriteREG(RW_MC_ISCALE2_X13, (iScaleBot * 13));
                WriteREG(RW_MC_ISCALE2_X15, (iScaleBot * 15));
                WriteREG(RW_MC_YSHIFT_OFF2, (iShiftBot + 32));
                WriteREG(RW_MC_CSHIFT_OFF2, ((128 * 64) + 32 - (128*iScaleBot)));
            }
        }

        if (rWMVDecInfo.rOther.rNewTopField.Old.iEnable == 1)
        {
            // icomp_set3 is forward top field old icomp parameters
            if (write_enable == 1)
            {
                WriteREG(RW_MC_ISCALE3_X1,
                         (rWMVDecInfo.rOther.rNewTopField.Old.iScale * 1));
                WriteREG(RW_MC_ISCALE3_X3,
                         (rWMVDecInfo.rOther.rNewTopField.Old.iScale * 3));
                WriteREG(RW_MC_ISCALE3_X5,
                         (rWMVDecInfo.rOther.rNewTopField.Old.iScale * 5));
                WriteREG(RW_MC_ISCALE3_X7,
                         (rWMVDecInfo.rOther.rNewTopField.Old.iScale * 7));
                WriteREG(RW_MC_ISCALE3_X9,
                         (rWMVDecInfo.rOther.rNewTopField.Old.iScale * 9));
                WriteREG(RW_MC_ISCALE3_X11,
                         (rWMVDecInfo.rOther.rNewTopField.Old.iScale * 11));
                WriteREG(RW_MC_ISCALE3_X13,
                         (rWMVDecInfo.rOther.rNewTopField.Old.iScale * 13));
                WriteREG(RW_MC_ISCALE3_X15,
                         (rWMVDecInfo.rOther.rNewTopField.Old.iScale * 15));
                WriteREG(RW_MC_YSHIFT_OFF3,
                         (rWMVDecInfo.rOther.rNewTopField.Old.iShift + 32));
                WriteREG(RW_MC_CSHIFT_OFF3, ((128 * 64) + 32 - (128
                                             *rWMVDecInfo.rOther.rNewTopField.Old.iScale)));
            }
        }

        if (rWMVDecInfo.rOther.rNewBotField.Old.iEnable == 1)
        {
            // icomp_set4 is forward bottom field old icomp parameters
            if (write_enable == 1)
            {
                WriteREG(RW_MC_ISCALE4_X1,
                         (rWMVDecInfo.rOther.rNewBotField.Old.iScale * 1));
                WriteREG(RW_MC_ISCALE4_X3,
                         (rWMVDecInfo.rOther.rNewBotField.Old.iScale * 3));
                WriteREG(RW_MC_ISCALE4_X5,
                         (rWMVDecInfo.rOther.rNewBotField.Old.iScale * 5));
                WriteREG(RW_MC_ISCALE4_X7,
                         (rWMVDecInfo.rOther.rNewBotField.Old.iScale * 7));
                WriteREG(RW_MC_ISCALE4_X9,
                         (rWMVDecInfo.rOther.rNewBotField.Old.iScale * 9));
                WriteREG(RW_MC_ISCALE4_X11,
                         (rWMVDecInfo.rOther.rNewBotField.Old.iScale * 11));
                WriteREG(RW_MC_ISCALE4_X13,
                         (rWMVDecInfo.rOther.rNewBotField.Old.iScale * 13));
                WriteREG(RW_MC_ISCALE4_X15,
                         (rWMVDecInfo.rOther.rNewBotField.Old.iScale * 15));
                WriteREG(RW_MC_YSHIFT_OFF4,
                         (rWMVDecInfo.rOther.rNewBotField.Old.iShift + 32));
                WriteREG(RW_MC_CSHIFT_OFF4, ((128 * 64) + 32 - (128
                                             *rWMVDecInfo.rOther.rNewBotField.Old.iScale)));
            }
        }

        rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable = 0;
        if (rWMVDecInfo.rOther.i4BoundaryUMVIcomp == 1)
        {
            /* top field picture */
            if (rWMVDecInfo.rOther.i4SecondFieldParity == 0)
            {
                if (rWMVDecInfo.rOther.rOldBotField.New.iEnable == 1)
                {
                    rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable = 1;
                    // icomp_set6 is forward bottom field old icomp parameters
                    if (write_enable == 1)
                    {
                        WriteREG(
                            RW_MC_ISCALE6_X1,
                            (rWMVDecInfo.rOther.rOldBotField.New.iScale * 1));
                        WriteREG(
                            RW_MC_ISCALE6_X3,
                            (rWMVDecInfo.rOther.rOldBotField.New.iScale * 3));
                        WriteREG(
                            RW_MC_ISCALE6_X5,
                            (rWMVDecInfo.rOther.rOldBotField.New.iScale * 5));
                        WriteREG(
                            RW_MC_ISCALE6_X7,
                            (rWMVDecInfo.rOther.rOldBotField.New.iScale * 7));
                        WriteREG(
                            RW_MC_ISCALE6_X9,
                            (rWMVDecInfo.rOther.rOldBotField.New.iScale * 9));
                        WriteREG(RW_MC_ISCALE6_X11,
                                 (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                  * 11));
                        WriteREG(RW_MC_ISCALE6_X13,
                                 (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                  * 13));
                        WriteREG(RW_MC_ISCALE6_X15,
                                 (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                  * 15));
                        WriteREG(RW_MC_YSHIFT_OFF6,
                                 (rWMVDecInfo.rOther.rOldBotField.New.iShift
                                  + 32));
                        WriteREG(RW_MC_CSHIFT_OFF6, ((128 * 64) + 32 - (128
                                                     *rWMVDecInfo.rOther.rOldBotField.New.iScale)));
                    }
                }
            }
            /* bottom field picture */
            else
            {
                if (rWMVDecInfo.rOther.rOldTopField.New.iEnable == 1)
                {
                    rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable = 1;
                    // icomp_set6 is forward bottom field old icomp parameters
                    if (write_enable == 1)
                    {

                        WriteREG(
                            RW_MC_ISCALE6_X1,
                            (rWMVDecInfo.rOther.rOldTopField.New.iScale * 1));
                        WriteREG(
                            RW_MC_ISCALE6_X3,
                            (rWMVDecInfo.rOther.rOldTopField.New.iScale * 3));
                        WriteREG(
                            RW_MC_ISCALE6_X5,
                            (rWMVDecInfo.rOther.rOldTopField.New.iScale * 5));
                        WriteREG(
                            RW_MC_ISCALE6_X7,
                            (rWMVDecInfo.rOther.rOldTopField.New.iScale * 7));
                        WriteREG(
                            RW_MC_ISCALE6_X9,
                            (rWMVDecInfo.rOther.rOldTopField.New.iScale * 9));
                        WriteREG(RW_MC_ISCALE6_X11,
                                 (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                  * 11));
                        WriteREG(RW_MC_ISCALE6_X13,
                                 (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                  * 13));
                        WriteREG(RW_MC_ISCALE6_X15,
                                 (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                  * 15));
                        WriteREG(RW_MC_YSHIFT_OFF6,
                                 (rWMVDecInfo.rOther.rOldTopField.New.iShift
                                  + 32));
                        WriteREG(RW_MC_CSHIFT_OFF6, ((128 * 64) + 32 - (128
                                                     *rWMVDecInfo.rOther.rOldTopField.New.iScale)));
                    }
                }
            }
        }

        if ( (rWMVDecInfo.rOther.rNewTopField.New.iEnable == 1)
                || (rWMVDecInfo.rOther.rNewBotField.New.iEnable == 1)
                || (rWMVDecInfo.rOther.rNewTopField.Old.iEnable == 1)
                || (rWMVDecInfo.rOther.rNewBotField.Old.iEnable == 1)
                || (rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable == 1))
        {
            // Icomp enable bits
            {
                int b208 = (rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable)+ (0
                           << 4)+ (rWMVDecInfo.rOther.rNewBotField.Old.iEnable
                                   << 8)+ (rWMVDecInfo.rOther.rNewTopField.Old.iEnable
                                           << 12)+ (rWMVDecInfo.rOther.rNewBotField.New.iEnable
                                                    << 16)+ (rWMVDecInfo.rOther.rNewTopField.New.iEnable
                                                            << 20);

                if (write_enable == 1)
                {
                    WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_EN + ICOMP_Y_EN);
                    WriteREG(RW_MC_ICOMP2_EN, b208);
                    WriteREG(RW_MC_SAME_ICOMP, NO_USE_SAME_ICOPM1_FOR_FRAME);
                }
            }
        }
        else
        {
            if (write_enable == 1)
            {
                WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_OFF + ICOMP_Y_OFF);
                WriteREG(RW_MC_ICOMP2_EN, 0);
                WriteREG(RW_MC_SAME_ICOMP, NO_USE_SAME_ICOPM1_FOR_FRAME);
            }
        }
    }//end : PVOP
    else if ((rWMVDecInfo.rPicLayer.u1PicType == BVOP)
             && (rWMVDecInfo.rOther.i4CodecVersion == WMVA))
    {
        if (rWMVDecInfo.rPicLayer.fgFieldMode == TRUE) //field picture
        {
            if (rWMVDecInfo.rPicLayer.i4CurrentTemporalField== 0)// the first B field picture
            {
                rWMVDecInfo.rOther.i4BoundaryUMVIcomp = 0;

                //
                // Step1: No update in the Icomp parameters
                //

                //
                // Step2: output MC parameters
                //

                //
                // Last second P field picture is Top Field
                //
                // (1) Forward TF == NEW_TF
                // (2) Forward BF == OLD_BF
                // (3) Backward TF == NULL
                // (4) Backward BF == NEW_BF

                if (rWMVDecInfo.rOther.i4SecondFieldParity == 0)
                {
                    // icomp_set1 is forward top field new icomp parameters
                    if (rWMVDecInfo.rOther.rNewTopField.New.iEnable == 1)
                    {
                        if (write_enable == 1)
                        {
                            WriteREG(RW_MC_ISCALE1_X1,
                                     (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                      * 1));
                            WriteREG(RW_MC_ISCALE1_X3,
                                     (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                      * 3));
                            WriteREG(RW_MC_ISCALE1_X5,
                                     (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                      * 5));
                            WriteREG(RW_MC_ISCALE1_X7,
                                     (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                      * 7));
                            WriteREG(RW_MC_ISCALE1_X9,
                                     (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                      * 9));
                            WriteREG(RW_MC_ISCALE1_X11,
                                     (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                      * 11));
                            WriteREG(RW_MC_ISCALE1_X13,
                                     (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                      * 13));
                            WriteREG(RW_MC_ISCALE1_X15,
                                     (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                      * 15));
                            WriteREG(RW_MC_YSHIFT_OFF1,
                                     (rWMVDecInfo.rOther.rNewTopField.New.iShift
                                      + 32));
                            WriteREG(
                                RW_MC_CSHIFT_OFF1,
                                ((128 * 64) + 32
                                 - (128
                                    *rWMVDecInfo.rOther.rNewTopField.New.iScale)));
                        }
                    }

                    // icomp_set2 is forward bottom field new icomp parameters
                    if (rWMVDecInfo.rOther.rOldBotField.New.iEnable == 1)
                    {
                        if (write_enable == 1)
                        {
                            WriteREG(RW_MC_ISCALE2_X1,
                                     (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                      * 1));
                            WriteREG(RW_MC_ISCALE2_X3,
                                     (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                      * 3));
                            WriteREG(RW_MC_ISCALE2_X5,
                                     (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                      * 5));
                            WriteREG(RW_MC_ISCALE2_X7,
                                     (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                      * 7));
                            WriteREG(RW_MC_ISCALE2_X9,
                                     (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                      * 9));
                            WriteREG(RW_MC_ISCALE2_X11,
                                     (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                      * 11));
                            WriteREG(RW_MC_ISCALE2_X13,
                                     (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                      * 13));
                            WriteREG(RW_MC_ISCALE2_X15,
                                     (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                      * 15));
                            WriteREG(RW_MC_YSHIFT_OFF2,
                                     (rWMVDecInfo.rOther.rOldBotField.New.iShift
                                      + 32));
                            WriteREG(
                                RW_MC_CSHIFT_OFF2,
                                ((128 * 64) + 32
                                 - (128
                                    *rWMVDecInfo.rOther.rOldBotField.New.iScale)));
                        }
                    }

                    // icomp_set3 is forward top field old icomp parameters
                    if (rWMVDecInfo.rOther.rNewTopField.Old.iEnable == 1)
                    {
                        if (write_enable == 1)
                        {
                            WriteREG(RW_MC_ISCALE3_X1,
                                     (rWMVDecInfo.rOther.rNewTopField.Old.iScale
                                      * 1));
                            WriteREG(RW_MC_ISCALE3_X3,
                                     (rWMVDecInfo.rOther.rNewTopField.Old.iScale
                                      * 3));
                            WriteREG(RW_MC_ISCALE3_X5,
                                     (rWMVDecInfo.rOther.rNewTopField.Old.iScale
                                      * 5));
                            WriteREG(RW_MC_ISCALE3_X7,
                                     (rWMVDecInfo.rOther.rNewTopField.Old.iScale
                                      * 7));
                            WriteREG(RW_MC_ISCALE3_X9,
                                     (rWMVDecInfo.rOther.rNewTopField.Old.iScale
                                      * 9));
                            WriteREG(RW_MC_ISCALE3_X11,
                                     (rWMVDecInfo.rOther.rNewTopField.Old.iScale
                                      * 11));
                            WriteREG(RW_MC_ISCALE3_X13,
                                     (rWMVDecInfo.rOther.rNewTopField.Old.iScale
                                      * 13));
                            WriteREG(RW_MC_ISCALE3_X15,
                                     (rWMVDecInfo.rOther.rNewTopField.Old.iScale
                                      * 15));
                            WriteREG(RW_MC_YSHIFT_OFF3,
                                     (rWMVDecInfo.rOther.rNewTopField.Old.iShift
                                      + 32));
                            WriteREG(
                                RW_MC_CSHIFT_OFF3,
                                ((128 * 64) + 32
                                 - (128
                                    *rWMVDecInfo.rOther.rNewTopField.Old.iScale)));
                        }
                    }

                    // icomp_set4 is forward bottom field old icomp parameters
                    if (rWMVDecInfo.rOther.rOldBotField.Old.iEnable == 1)
                    {
                        if (write_enable == 1)
                        {
                            WriteREG(RW_MC_ISCALE4_X1,
                                     (rWMVDecInfo.rOther.rOldBotField.Old.iScale
                                      * 1));
                            WriteREG(RW_MC_ISCALE4_X3,
                                     (rWMVDecInfo.rOther.rOldBotField.Old.iScale
                                      * 3));
                            WriteREG(RW_MC_ISCALE4_X5,
                                     (rWMVDecInfo.rOther.rOldBotField.Old.iScale
                                      * 5));
                            WriteREG(RW_MC_ISCALE4_X7,
                                     (rWMVDecInfo.rOther.rOldBotField.Old.iScale
                                      * 7));
                            WriteREG(RW_MC_ISCALE4_X9,
                                     (rWMVDecInfo.rOther.rOldBotField.Old.iScale
                                      * 9));
                            WriteREG(RW_MC_ISCALE4_X11,
                                     (rWMVDecInfo.rOther.rOldBotField.Old.iScale
                                      * 11));
                            WriteREG(RW_MC_ISCALE4_X13,
                                     (rWMVDecInfo.rOther.rOldBotField.Old.iScale
                                      * 13));
                            WriteREG(RW_MC_ISCALE4_X15,
                                     (rWMVDecInfo.rOther.rOldBotField.Old.iScale
                                      * 15));
                            WriteREG(RW_MC_YSHIFT_OFF4,
                                     (rWMVDecInfo.rOther.rOldBotField.Old.iShift
                                      + 32));
                            WriteREG(
                                RW_MC_CSHIFT_OFF4,
                                ((128 * 64) + 32
                                 - (128
                                    *rWMVDecInfo.rOther.rOldBotField.Old.iScale)));
                        }
                    }

                    // icomp_set5 is backward top or bottom field icomp parameters
                    if (rWMVDecInfo.rOther.rNewBotField.New.iEnable == 1)
                    {
                        if (write_enable == 1)
                        {
                            WriteREG(RW_MC_ISCALE5_X1,
                                     (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                      * 1));
                            WriteREG(RW_MC_ISCALE5_X3,
                                     (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                      * 3));
                            WriteREG(RW_MC_ISCALE5_X5,
                                     (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                      * 5));
                            WriteREG(RW_MC_ISCALE5_X7,
                                     (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                      * 7));
                            WriteREG(RW_MC_ISCALE5_X9,
                                     (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                      * 9));
                            WriteREG(RW_MC_ISCALE5_X11,
                                     (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                      * 11));
                            WriteREG(RW_MC_ISCALE5_X13,
                                     (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                      * 13));
                            WriteREG(RW_MC_ISCALE5_X15,
                                     (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                      * 15));
                            WriteREG(RW_MC_YSHIFT_OFF5,
                                     (rWMVDecInfo.rOther.rNewBotField.New.iShift
                                      + 32));
                            WriteREG(
                                RW_MC_CSHIFT_OFF5,
                                ((128 * 64) + 32
                                 - (128
                                    *rWMVDecInfo.rOther.rNewBotField.New.iScale)));
                            WriteREG(RW_MC_BWD_ICOMP_FLD, BWD_BTM_FLD);
                        }
                    }

                    if ( (rWMVDecInfo.rOther.rNewTopField.New.iEnable == 1)
                            || (rWMVDecInfo.rOther.rOldBotField.New.iEnable
                                == 1)
                            || (rWMVDecInfo.rOther.rNewTopField.Old.iEnable
                                == 1)
                            || (rWMVDecInfo.rOther.rOldBotField.Old.iEnable
                                == 1)
                            || (rWMVDecInfo.rOther.rNewBotField.New.iEnable
                                == 1))
                    {
                        // Icomp enable bits
                        {
                            int
                            b208 =
                                (0)
                                + (rWMVDecInfo.rOther.rNewBotField.New.iEnable
                                   << 4)
                                + (rWMVDecInfo.rOther.rOldBotField.Old.iEnable
                                   << 8)
                                + (rWMVDecInfo.rOther.rNewTopField.Old.iEnable
                                   << 12)
                                + (rWMVDecInfo.rOther.rOldBotField.New.iEnable
                                   << 16)
                                + (rWMVDecInfo.rOther.rNewTopField.New.iEnable
                                   << 20);

                            if (write_enable == 1)
                            {
                                WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_EN
                                         + ICOMP_Y_EN);
                                WriteREG(RW_MC_ICOMP2_EN, b208);
                                WriteREG(RW_MC_SAME_ICOMP,
                                         NO_USE_SAME_ICOPM1_FOR_FRAME);
                            }
                        }
                    }
                    else
                    {
                        if (write_enable == 1)
                        {
                            WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_OFF
                                     + ICOMP_Y_OFF);
                            WriteREG(RW_MC_ICOMP2_EN, 0);
                            WriteREG(RW_MC_SAME_ICOMP,
                                     NO_USE_SAME_ICOPM1_FOR_FRAME);
                        }
                    }
                }
                else
                {

                    //
                    // Last second P field picture is Bottom Field
                    //
                    // (1) Forward TF == OLD_TF
                    // (2) Forward BF == NEW_BF
                    // (3) Backward TF == NEW_TF
                    // (4) Backward BF == NULL

                    // icomp_set1 is forward top field new icomp parameters
                    if (rWMVDecInfo.rOther.rOldTopField.New.iEnable == 1)
                    {
                        if (write_enable == 1)
                        {
                            WriteREG(RW_MC_ISCALE1_X1,
                                     (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                      * 1));
                            WriteREG(RW_MC_ISCALE1_X3,
                                     (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                      * 3));
                            WriteREG(RW_MC_ISCALE1_X5,
                                     (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                      * 5));
                            WriteREG(RW_MC_ISCALE1_X7,
                                     (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                      * 7));
                            WriteREG(RW_MC_ISCALE1_X9,
                                     (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                      * 9));
                            WriteREG(RW_MC_ISCALE1_X11,
                                     (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                      * 11));
                            WriteREG(RW_MC_ISCALE1_X13,
                                     (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                      * 13));
                            WriteREG(RW_MC_ISCALE1_X15,
                                     (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                      * 15));
                            WriteREG(RW_MC_YSHIFT_OFF1,
                                     (rWMVDecInfo.rOther.rOldTopField.New.iShift
                                      + 32));
                            WriteREG(
                                RW_MC_CSHIFT_OFF1,
                                ((128 * 64) + 32
                                 - (128
                                    *rWMVDecInfo.rOther.rOldTopField.New.iScale)));
                        }
                    }
                    // icomp_set2 is forward bottom field new icomp parameters
                    if (rWMVDecInfo.rOther.rNewBotField.New.iEnable == 1)
                    {
                        if (write_enable == 1)
                        {
                            WriteREG(RW_MC_ISCALE2_X1,
                                     (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                      * 1));
                            WriteREG(RW_MC_ISCALE2_X3,
                                     (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                      * 3));
                            WriteREG(RW_MC_ISCALE2_X5,
                                     (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                      * 5));
                            WriteREG(RW_MC_ISCALE2_X7,
                                     (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                      * 7));
                            WriteREG(RW_MC_ISCALE2_X9,
                                     (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                      * 9));
                            WriteREG(RW_MC_ISCALE2_X11,
                                     (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                      * 11));
                            WriteREG(RW_MC_ISCALE2_X13,
                                     (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                      * 13));
                            WriteREG(RW_MC_ISCALE2_X15,
                                     (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                      * 15));
                            WriteREG(RW_MC_YSHIFT_OFF2,
                                     (rWMVDecInfo.rOther.rNewBotField.New.iShift
                                      + 32));
                            WriteREG(
                                RW_MC_CSHIFT_OFF2,
                                ((128 * 64) + 32
                                 - (128
                                    *rWMVDecInfo.rOther.rNewBotField.New.iScale)));
                        }
                    }

                    // icomp_set3 is forward top field old icomp parameters
                    if (rWMVDecInfo.rOther.rOldTopField.Old.iEnable == 1)
                    {
                        if (write_enable == 1)
                        {
                            WriteREG(RW_MC_ISCALE3_X1,
                                     (rWMVDecInfo.rOther.rOldTopField.Old.iScale
                                      * 1));
                            WriteREG(RW_MC_ISCALE3_X3,
                                     (rWMVDecInfo.rOther.rOldTopField.Old.iScale
                                      * 3));
                            WriteREG(RW_MC_ISCALE3_X5,
                                     (rWMVDecInfo.rOther.rOldTopField.Old.iScale
                                      * 5));
                            WriteREG(RW_MC_ISCALE3_X7,
                                     (rWMVDecInfo.rOther.rOldTopField.Old.iScale
                                      * 7));
                            WriteREG(RW_MC_ISCALE3_X9,
                                     (rWMVDecInfo.rOther.rOldTopField.Old.iScale
                                      * 9));
                            WriteREG(RW_MC_ISCALE3_X11,
                                     (rWMVDecInfo.rOther.rOldTopField.Old.iScale
                                      * 11));
                            WriteREG(RW_MC_ISCALE3_X13,
                                     (rWMVDecInfo.rOther.rOldTopField.Old.iScale
                                      * 13));
                            WriteREG(RW_MC_ISCALE3_X15,
                                     (rWMVDecInfo.rOther.rOldTopField.Old.iScale
                                      * 15));
                            WriteREG(RW_MC_YSHIFT_OFF3,
                                     (rWMVDecInfo.rOther.rOldTopField.Old.iShift
                                      + 32));
                            WriteREG(
                                RW_MC_CSHIFT_OFF3,
                                ((128 * 64) + 32
                                 - (128
                                    *rWMVDecInfo.rOther.rOldTopField.Old.iScale)));
                        }
                    }

                    // icomp_set4 is forward bottom field old icomp parameters
                    if (rWMVDecInfo.rOther.rNewBotField.Old.iEnable == 1)
                    {
                        if (write_enable == 1)
                        {
                            WriteREG(RW_MC_ISCALE4_X1,
                                     (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                      * 1));
                            WriteREG(RW_MC_ISCALE4_X3,
                                     (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                      * 3));
                            WriteREG(RW_MC_ISCALE4_X5,
                                     (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                      * 5));
                            WriteREG(RW_MC_ISCALE4_X7,
                                     (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                      * 7));
                            WriteREG(RW_MC_ISCALE4_X9,
                                     (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                      * 9));
                            WriteREG(RW_MC_ISCALE4_X11,
                                     (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                      * 11));
                            WriteREG(RW_MC_ISCALE4_X13,
                                     (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                      * 13));
                            WriteREG(RW_MC_ISCALE4_X15,
                                     (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                      * 15));
                            WriteREG(RW_MC_YSHIFT_OFF4,
                                     (rWMVDecInfo.rOther.rNewBotField.Old.iShift
                                      + 32));
                            WriteREG(
                                RW_MC_CSHIFT_OFF4,
                                ((128 * 64) + 32
                                 - (128
                                    *rWMVDecInfo.rOther.rNewBotField.Old.iScale)));
                        }
                    }

                    // icomp_set5 is backward top or bottom field icomp parameters
                    if (rWMVDecInfo.rOther.rNewTopField.New.iEnable == 1)
                    {
                        if (write_enable == 1)
                        {
                            WriteREG(RW_MC_ISCALE5_X1,
                                     (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                      * 1));
                            WriteREG(RW_MC_ISCALE5_X3,
                                     (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                      * 3));
                            WriteREG(RW_MC_ISCALE5_X5,
                                     (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                      * 5));
                            WriteREG(RW_MC_ISCALE5_X7,
                                     (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                      * 7));
                            WriteREG(RW_MC_ISCALE5_X9,
                                     (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                      * 9));
                            WriteREG(RW_MC_ISCALE5_X11,
                                     (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                      * 11));
                            WriteREG(RW_MC_ISCALE5_X13,
                                     (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                      * 13));
                            WriteREG(RW_MC_ISCALE5_X15,
                                     (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                      * 15));
                            WriteREG(RW_MC_YSHIFT_OFF5,
                                     (rWMVDecInfo.rOther.rNewTopField.New.iShift
                                      + 32));
                            WriteREG(
                                RW_MC_CSHIFT_OFF5,
                                ((128 * 64) + 32
                                 - (128
                                    *rWMVDecInfo.rOther.rNewTopField.New.iScale)));
                            WriteREG(RW_MC_BWD_ICOMP_FLD, BWD_TOP_FLD);
                        }

                    }

                    if ( (rWMVDecInfo.rOther.rOldTopField.New.iEnable == 1)
                            || (rWMVDecInfo.rOther.rNewBotField.New.iEnable
                                == 1)
                            || (rWMVDecInfo.rOther.rOldTopField.Old.iEnable
                                == 1)
                            || (rWMVDecInfo.rOther.rNewBotField.Old.iEnable
                                == 1)
                            || (rWMVDecInfo.rOther.rNewTopField.New.iEnable
                                == 1))
                    {
                        // Icomp enable bits
                        {
                            int
                            b208 =
                                (0)
                                + (rWMVDecInfo.rOther.rNewTopField.New.iEnable
                                   << 4)
                                + (rWMVDecInfo.rOther.rNewBotField.Old.iEnable
                                   << 8)
                                + (rWMVDecInfo.rOther.rOldTopField.Old.iEnable
                                   << 12)
                                + (rWMVDecInfo.rOther.rNewBotField.New.iEnable
                                   << 16)
                                + (rWMVDecInfo.rOther.rOldTopField.New.iEnable
                                   << 20);

                            if (write_enable == 1)
                            {
                                WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_EN
                                         + ICOMP_Y_EN);
                                WriteREG(RW_MC_ICOMP2_EN, b208);
                                WriteREG(RW_MC_SAME_ICOMP,
                                         NO_USE_SAME_ICOPM1_FOR_FRAME);
                            }
                        }
                    }
                    else
                    {
                        if (write_enable == 1)
                        {
                            WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_OFF
                                     + ICOMP_Y_OFF);
                            WriteREG(RW_MC_ICOMP2_EN, 0);
                            WriteREG(RW_MC_SAME_ICOMP,
                                     NO_USE_SAME_ICOPM1_FOR_FRAME);
                        }
                    }
                }
            } //end : first B field picture
            else // Second B field picture
            {
                if ( (rWMVDecInfo.rDram.u4ForwardRefPicType == PROGRESSIVE)
                        && (rWMVDecInfo.rDram.u4BackwardRefPicType
                            == INTERLACEFIELD)
                        /* && this is a second field */
                   )
                {
                    rWMVDecInfo.rOther.i4BoundaryUMVIcomp = 1;
                }
                else
                {
                    rWMVDecInfo.rOther.i4BoundaryUMVIcomp = 0;
                }

                if (rWMVDecInfo.rPicLayer.i4CurrentField == 0) // current field is top field
                {
                    //
                    // Step1: No update in the Icomp parameters
                    //

                    //
                    // Step2: output MC parameters
                    //

                    //
                    // Last second P field picture is Top Field
                    //
                    // (1) Forward TF == NEW_TF
                    //
                    // (2) Forward BF == NULL
                    //
                    // (3) Backward TF == NULL
                    // (4) Backward BF == NEW_BF

                    if (rWMVDecInfo.rOther.i4SecondFieldParity == 0)
                    {
                        // icomp_set1 is forward top field new icomp parameters
                        if (rWMVDecInfo.rOther.rNewTopField.New.iEnable == 1)
                        {
                            if (write_enable == 1)
                            {
                                WriteREG(
                                    RW_MC_ISCALE1_X1,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 1));
                                WriteREG(
                                    RW_MC_ISCALE1_X3,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 3));
                                WriteREG(
                                    RW_MC_ISCALE1_X5,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 5));
                                WriteREG(
                                    RW_MC_ISCALE1_X7,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 7));
                                WriteREG(
                                    RW_MC_ISCALE1_X9,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 9));
                                WriteREG(
                                    RW_MC_ISCALE1_X11,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 11));
                                WriteREG(
                                    RW_MC_ISCALE1_X13,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 13));
                                WriteREG(
                                    RW_MC_ISCALE1_X15,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 15));
                                WriteREG(
                                    RW_MC_YSHIFT_OFF1,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iShift
                                     + 32));
                                WriteREG(
                                    RW_MC_CSHIFT_OFF1,
                                    ((128 * 64) + 32
                                     - (128
                                        *rWMVDecInfo.rOther.rNewTopField.New.iScale)));
                            }
                        }

                        // icomp_set3 is forward top field old icomp parameters
                        if (rWMVDecInfo.rOther.rNewTopField.Old.iEnable == 1)
                        {
                            if (write_enable == 1)
                            {
                                WriteREG(
                                    RW_MC_ISCALE3_X1,
                                    (rWMVDecInfo.rOther.rNewTopField.Old.iScale
                                     * 1));
                                WriteREG(
                                    RW_MC_ISCALE3_X3,
                                    (rWMVDecInfo.rOther.rNewTopField.Old.iScale
                                     * 3));
                                WriteREG(
                                    RW_MC_ISCALE3_X5,
                                    (rWMVDecInfo.rOther.rNewTopField.Old.iScale
                                     * 5));
                                WriteREG(
                                    RW_MC_ISCALE3_X7,
                                    (rWMVDecInfo.rOther.rNewTopField.Old.iScale
                                     * 7));
                                WriteREG(
                                    RW_MC_ISCALE3_X9,
                                    (rWMVDecInfo.rOther.rNewTopField.Old.iScale
                                     * 9));
                                WriteREG(
                                    RW_MC_ISCALE3_X11,
                                    (rWMVDecInfo.rOther.rNewTopField.Old.iScale
                                     * 11));
                                WriteREG(
                                    RW_MC_ISCALE3_X13,
                                    (rWMVDecInfo.rOther.rNewTopField.Old.iScale
                                     * 13));
                                WriteREG(
                                    RW_MC_ISCALE3_X15,
                                    (rWMVDecInfo.rOther.rNewTopField.Old.iScale
                                     * 15));
                                WriteREG(
                                    RW_MC_YSHIFT_OFF3,
                                    (rWMVDecInfo.rOther.rNewTopField.Old.iShift
                                     + 32));
                                WriteREG(
                                    RW_MC_CSHIFT_OFF3,
                                    ((128 * 64) + 32
                                     - (128
                                        *rWMVDecInfo.rOther.rNewTopField.Old.iScale)));
                            }
                        }

                        // icomp_set5 is backward top or bottom field icomp parameters
                        if (rWMVDecInfo.rOther.rNewBotField.New.iEnable == 1)
                        {
                            if (write_enable == 1)
                            {
                                WriteREG(
                                    RW_MC_ISCALE5_X1,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 1));
                                WriteREG(
                                    RW_MC_ISCALE5_X3,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 3));
                                WriteREG(
                                    RW_MC_ISCALE5_X5,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 5));
                                WriteREG(
                                    RW_MC_ISCALE5_X7,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 7));
                                WriteREG(
                                    RW_MC_ISCALE5_X9,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 9));
                                WriteREG(
                                    RW_MC_ISCALE5_X11,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 11));
                                WriteREG(
                                    RW_MC_ISCALE5_X13,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 13));
                                WriteREG(
                                    RW_MC_ISCALE5_X15,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 15));
                                WriteREG(
                                    RW_MC_YSHIFT_OFF5,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iShift
                                     + 32));
                                WriteREG(
                                    RW_MC_CSHIFT_OFF5,
                                    ((128 * 64) + 32
                                     - (128
                                        *rWMVDecInfo.rOther.rNewBotField.New.iScale)));
                                WriteREG(RW_MC_BWD_ICOMP_FLD, BWD_BTM_FLD);
                            }
                        }

                        rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable = 0;

                        if (rWMVDecInfo.rOther.i4BoundaryUMVIcomp == 1)
                        {
                            /* top field picture */
                            if (rWMVDecInfo.rOther.i4SecondFieldParity == 0)
                            {
                                if (rWMVDecInfo.rOther.rOldBotField.New.iEnable
                                        == 1)
                                {
                                    rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable
                                    = 1;
                                    // icomp_set4 is forward bottom field old icomp parameters
                                    if (write_enable == 1)
                                    {
                                        WriteREG(
                                            RW_MC_ISCALE6_X1,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 1));
                                        WriteREG(
                                            RW_MC_ISCALE6_X3,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 3));
                                        WriteREG(
                                            RW_MC_ISCALE6_X5,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 5));
                                        WriteREG(
                                            RW_MC_ISCALE6_X7,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 7));
                                        WriteREG(
                                            RW_MC_ISCALE6_X9,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 9));
                                        WriteREG(
                                            RW_MC_ISCALE6_X11,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 11));
                                        WriteREG(
                                            RW_MC_ISCALE6_X13,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 13));
                                        WriteREG(
                                            RW_MC_ISCALE6_X15,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 15));
                                        WriteREG(
                                            RW_MC_YSHIFT_OFF6,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iShift
                                             + 32));
                                        WriteREG(
                                            RW_MC_CSHIFT_OFF6,
                                            ((128 * 64) + 32
                                             - (128
                                                *rWMVDecInfo.rOther.rOldBotField.New.iScale)));
                                    }
                                }
                            }
                            /* bottom field picture */
                            else
                            {
                                if (rWMVDecInfo.rOther.rOldTopField.New.iEnable
                                        == 1)
                                {
                                    rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable
                                    = 1;
                                    // icomp_set4 is forward bottom field old icomp parameters
                                    if (write_enable == 1)
                                    {
                                        WriteREG(
                                            RW_MC_ISCALE6_X1,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 1));
                                        WriteREG(
                                            RW_MC_ISCALE6_X3,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 3));
                                        WriteREG(
                                            RW_MC_ISCALE6_X5,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 5));
                                        WriteREG(
                                            RW_MC_ISCALE6_X7,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 7));
                                        WriteREG(
                                            RW_MC_ISCALE6_X9,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 9));
                                        WriteREG(
                                            RW_MC_ISCALE6_X11,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 11));
                                        WriteREG(
                                            RW_MC_ISCALE6_X13,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 13));
                                        WriteREG(
                                            RW_MC_ISCALE6_X15,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 15));
                                        WriteREG(
                                            RW_MC_YSHIFT_OFF6,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iShift
                                             + 32));
                                        WriteREG(
                                            RW_MC_CSHIFT_OFF6,
                                            ((128 * 64) + 32
                                             - (128
                                                *rWMVDecInfo.rOther.rOldTopField.New.iScale)));
                                    }
                                }
                            }
                        }

                        if ( (rWMVDecInfo.rOther.rNewTopField.New.iEnable == 1)
                                || (rWMVDecInfo.rOther.rNewTopField.Old.iEnable
                                    == 1)
                                || (rWMVDecInfo.rOther.rNewBotField.New.iEnable
                                    == 1)
                                || (rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable
                                    == 1))
                        {
                            // Icomp enable bits
                            {
                                int
                                b208 =
                                    (rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable)
                                    + (rWMVDecInfo.rOther.rNewBotField.New.iEnable
                                       << 4)+ (0 << 8)
                                    + (rWMVDecInfo.rOther.rNewTopField.Old.iEnable
                                       << 12)+ (0
                                                << 16)
                                    + (rWMVDecInfo.rOther.rNewTopField.New.iEnable
                                       << 20);

                                if (write_enable == 1)
                                {
                                    WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_EN
                                             + ICOMP_Y_EN);
                                    WriteREG(RW_MC_ICOMP2_EN, b208);
                                    WriteREG(RW_MC_SAME_ICOMP,
                                             NO_USE_SAME_ICOPM1_FOR_FRAME);

                                }
                            }
                        }
                        else
                        {
                            if (write_enable == 1)
                            {
                                WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_OFF
                                         + ICOMP_Y_OFF);
                                WriteREG(RW_MC_ICOMP2_EN, 0);
                                WriteREG(RW_MC_SAME_ICOMP,
                                         NO_USE_SAME_ICOPM1_FOR_FRAME);
                            }
                        }
                    }
                    else
                    {
                        //
                        // Last second P field picture is Bottom Field
                        //
                        //
                        // (1) Forward TF == NULL
                        //
                        // (2) Forward BF == NEW_BF
                        // (3) Backward TF == NEW_TF
                        // (4) Backward BF == NULL

                        // icomp_set2 is forward bottom field new icomp parameters
                        if (rWMVDecInfo.rOther.rNewBotField.New.iEnable == 1)
                        {
                            if (write_enable == 1)
                            {
                                WriteREG(
                                    RW_MC_ISCALE2_X1,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 1));
                                WriteREG(
                                    RW_MC_ISCALE2_X3,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 3));
                                WriteREG(
                                    RW_MC_ISCALE2_X5,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 5));
                                WriteREG(
                                    RW_MC_ISCALE2_X7,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 7));
                                WriteREG(
                                    RW_MC_ISCALE2_X9,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 9));
                                WriteREG(
                                    RW_MC_ISCALE2_X11,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 11));
                                WriteREG(
                                    RW_MC_ISCALE2_X13,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 13));
                                WriteREG(
                                    RW_MC_ISCALE2_X15,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 15));
                                WriteREG(
                                    RW_MC_YSHIFT_OFF2,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iShift
                                     + 32));
                                WriteREG(
                                    RW_MC_CSHIFT_OFF2,
                                    ((128 * 64) + 32
                                     - (128
                                        *rWMVDecInfo.rOther.rNewBotField.New.iScale)));
                            }
                        }

                        // icomp_set4 is forward bottom field old icomp parameters
                        if (rWMVDecInfo.rOther.rNewBotField.Old.iEnable == 1)
                        {
                            if (write_enable == 1)
                            {
                                WriteREG(
                                    RW_MC_ISCALE4_X1,
                                    (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                     * 1));
                                WriteREG(
                                    RW_MC_ISCALE4_X3,
                                    (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                     * 3));
                                WriteREG(
                                    RW_MC_ISCALE4_X5,
                                    (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                     * 5));
                                WriteREG(
                                    RW_MC_ISCALE4_X7,
                                    (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                     * 7));
                                WriteREG(
                                    RW_MC_ISCALE4_X9,
                                    (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                     * 9));
                                WriteREG(
                                    RW_MC_ISCALE4_X11,
                                    (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                     * 11));
                                WriteREG(
                                    RW_MC_ISCALE4_X13,
                                    (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                     * 13));
                                WriteREG(
                                    RW_MC_ISCALE4_X15,
                                    (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                     * 15));
                                WriteREG(
                                    RW_MC_YSHIFT_OFF4,
                                    (rWMVDecInfo.rOther.rNewBotField.Old.iShift
                                     + 32));
                                WriteREG(
                                    RW_MC_CSHIFT_OFF4,
                                    ((128 * 64) + 32
                                     - (128
                                        *rWMVDecInfo.rOther.rNewBotField.Old.iScale)));
                            }
                        }

                        // icomp_set5 is backward top or bottom field icomp parameters

                        if (rWMVDecInfo.rOther.rNewTopField.New.iEnable == 1)
                        {
                            if (write_enable == 1)
                            {
                                WriteREG(
                                    RW_MC_ISCALE5_X1,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 1));
                                WriteREG(
                                    RW_MC_ISCALE5_X3,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 3));
                                WriteREG(
                                    RW_MC_ISCALE5_X5,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 5));
                                WriteREG(
                                    RW_MC_ISCALE5_X7,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 7));
                                WriteREG(
                                    RW_MC_ISCALE5_X9,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 9));
                                WriteREG(
                                    RW_MC_ISCALE5_X11,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 11));
                                WriteREG(
                                    RW_MC_ISCALE5_X13,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 13));
                                WriteREG(
                                    RW_MC_ISCALE5_X15,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 15));
                                WriteREG(
                                    RW_MC_YSHIFT_OFF5,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iShift
                                     + 32));
                                WriteREG(
                                    RW_MC_CSHIFT_OFF5,
                                    ((128 * 64) + 32
                                     - (128
                                        *rWMVDecInfo.rOther.rNewTopField.New.iScale)));
                                WriteREG(RW_MC_BWD_ICOMP_FLD, BWD_TOP_FLD);
                            }
                        }

                        rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable = 0;

                        if (rWMVDecInfo.rOther.i4BoundaryUMVIcomp == 1)
                        {
                            /* top field picture */
                            if (rWMVDecInfo.rOther.i4SecondFieldParity == 0)
                            {
                                if (rWMVDecInfo.rOther.rOldBotField.New.iEnable
                                        == 1)
                                {
                                    rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable
                                    = 1;
                                    // icomp_set4 is forward bottom field old icomp parameters
                                    if (write_enable == 1)
                                    {
                                        WriteREG(
                                            RW_MC_ISCALE6_X1,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 1));
                                        WriteREG(
                                            RW_MC_ISCALE6_X3,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 3));
                                        WriteREG(
                                            RW_MC_ISCALE6_X5,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 5));
                                        WriteREG(
                                            RW_MC_ISCALE6_X7,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 7));
                                        WriteREG(
                                            RW_MC_ISCALE6_X9,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 9));
                                        WriteREG(
                                            RW_MC_ISCALE6_X11,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 11));
                                        WriteREG(
                                            RW_MC_ISCALE6_X13,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 13));
                                        WriteREG(
                                            RW_MC_ISCALE6_X15,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 15));
                                        WriteREG(
                                            RW_MC_YSHIFT_OFF6,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iShift
                                             + 32));
                                        WriteREG(
                                            RW_MC_CSHIFT_OFF6,
                                            ((128 * 64) + 32
                                             - (128
                                                *rWMVDecInfo.rOther.rOldBotField.New.iScale)));
                                    }
                                }
                            }
                            /* bottom field picture */
                            else
                            {
                                if (rWMVDecInfo.rOther.rOldTopField.New.iEnable
                                        == 1)
                                {
                                    rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable
                                    = 1;
                                    // icomp_set4 is forward bottom field old icomp parameters
                                    if (write_enable == 1)
                                    {
                                        WriteREG(
                                            RW_MC_ISCALE6_X1,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 1));
                                        WriteREG(
                                            RW_MC_ISCALE6_X3,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 3));
                                        WriteREG(
                                            RW_MC_ISCALE6_X5,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 5));
                                        WriteREG(
                                            RW_MC_ISCALE6_X7,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 7));
                                        WriteREG(
                                            RW_MC_ISCALE6_X9,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 9));
                                        WriteREG(
                                            RW_MC_ISCALE6_X11,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 11));
                                        WriteREG(
                                            RW_MC_ISCALE6_X13,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 13));
                                        WriteREG(
                                            RW_MC_ISCALE6_X15,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 15));
                                        WriteREG(
                                            RW_MC_YSHIFT_OFF6,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iShift
                                             + 32));
                                        WriteREG(
                                            RW_MC_CSHIFT_OFF6,
                                            ((128 * 64) + 32
                                             - (128
                                                *rWMVDecInfo.rOther.rOldTopField.New.iScale)));
                                    }
                                }
                            }
                        }

                        if ( (rWMVDecInfo.rOther.rNewBotField.New.iEnable == 1)
                                || (rWMVDecInfo.rOther.rNewBotField.Old.iEnable
                                    == 1)
                                || (rWMVDecInfo.rOther.rNewTopField.New.iEnable
                                    == 1)
                                || (rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable
                                    == 1))
                        {
                            // Icomp enable bits
                            {
                                int
                                b208 =
                                    (rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable)
                                    + (rWMVDecInfo.rOther.rNewTopField.New.iEnable
                                       << 4)
                                    + (rWMVDecInfo.rOther.rNewBotField.Old.iEnable
                                       << 8)
                                    + (0 << 12)
                                    + (rWMVDecInfo.rOther.rNewBotField.New.iEnable
                                       << 16)+ (0
                                                << 20);

                                if (write_enable == 1)
                                {
                                    WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_EN
                                             + ICOMP_Y_EN);
                                    WriteREG(RW_MC_ICOMP2_EN, b208);
                                    WriteREG(RW_MC_SAME_ICOMP,
                                             NO_USE_SAME_ICOPM1_FOR_FRAME);
                                }
                            }
                        }
                        else
                        {
                            if (write_enable == 1)
                            {
                                WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_OFF
                                         + ICOMP_Y_OFF);
                                WriteREG(RW_MC_ICOMP2_EN, 0);
                                WriteREG(RW_MC_SAME_ICOMP,
                                         NO_USE_SAME_ICOPM1_FOR_FRAME);
                            }
                        }
                    }
                }
                else// current field is bottom field
                {

                    //
                    // Step1: No update in the Icomp parameters
                    //

                    //
                    // Step2: output MC parameters
                    //

                    //
                    // Last second P field picture is Top Field
                    //
                    //
                    // (1) Forward TF == NULL
                    //
                    // (2) Forward BF == OLD_BF
                    // (3) Backward TF == NULL
                    // (4) Backward BF == NEW_BF

                    if (rWMVDecInfo.rOther.i4SecondFieldParity == 0)
                    {
                        // icomp_set2 is forward bottom field new icomp parameters
                        if (rWMVDecInfo.rOther.rOldBotField.New.iEnable == 1)
                        {
                            if (write_enable == 1)
                            {
                                WriteREG(
                                    RW_MC_ISCALE2_X1,
                                    (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                     * 1));
                                WriteREG(
                                    RW_MC_ISCALE2_X3,
                                    (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                     * 3));
                                WriteREG(
                                    RW_MC_ISCALE2_X5,
                                    (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                     * 5));
                                WriteREG(
                                    RW_MC_ISCALE2_X7,
                                    (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                     * 7));
                                WriteREG(
                                    RW_MC_ISCALE2_X9,
                                    (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                     * 9));
                                WriteREG(
                                    RW_MC_ISCALE2_X11,
                                    (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                     * 11));
                                WriteREG(
                                    RW_MC_ISCALE2_X13,
                                    (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                     * 13));
                                WriteREG(
                                    RW_MC_ISCALE2_X15,
                                    (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                     * 15));
                                WriteREG(
                                    RW_MC_YSHIFT_OFF2,
                                    (rWMVDecInfo.rOther.rOldBotField.New.iShift
                                     + 32));
                                WriteREG(
                                    RW_MC_CSHIFT_OFF2,
                                    ((128 * 64) + 32
                                     - (128
                                        *rWMVDecInfo.rOther.rOldBotField.New.iScale)));
                            }
                        }

                        // icomp_set4 is forward bottom field old icomp parameters
                        if (rWMVDecInfo.rOther.rOldBotField.Old.iEnable == 1)
                        {
                            if (write_enable == 1)
                            {
                                WriteREG(
                                    RW_MC_ISCALE4_X1,
                                    (rWMVDecInfo.rOther.rOldBotField.Old.iScale
                                     * 1));
                                WriteREG(
                                    RW_MC_ISCALE4_X3,
                                    (rWMVDecInfo.rOther.rOldBotField.Old.iScale
                                     * 3));
                                WriteREG(
                                    RW_MC_ISCALE4_X5,
                                    (rWMVDecInfo.rOther.rOldBotField.Old.iScale
                                     * 5));
                                WriteREG(
                                    RW_MC_ISCALE4_X7,
                                    (rWMVDecInfo.rOther.rOldBotField.Old.iScale
                                     * 7));
                                WriteREG(
                                    RW_MC_ISCALE4_X9,
                                    (rWMVDecInfo.rOther.rOldBotField.Old.iScale
                                     * 9));
                                WriteREG(
                                    RW_MC_ISCALE4_X11,
                                    (rWMVDecInfo.rOther.rOldBotField.Old.iScale
                                     * 11));
                                WriteREG(
                                    RW_MC_ISCALE4_X13,
                                    (rWMVDecInfo.rOther.rOldBotField.Old.iScale
                                     * 13));
                                WriteREG(
                                    RW_MC_ISCALE4_X15,
                                    (rWMVDecInfo.rOther.rOldBotField.Old.iScale
                                     * 15));
                                WriteREG(
                                    RW_MC_YSHIFT_OFF4,
                                    (rWMVDecInfo.rOther.rOldBotField.Old.iShift
                                     + 32));
                                WriteREG(
                                    RW_MC_CSHIFT_OFF4,
                                    ((128 * 64) + 32
                                     - (128
                                        *rWMVDecInfo.rOther.rOldBotField.Old.iScale)));
                            }
                        }

                        // icomp_set5 is backward top or bottom field icomp parameters
                        if (rWMVDecInfo.rOther.rNewBotField.New.iEnable == 1)
                        {
                            if (write_enable == 1)
                            {
                                WriteREG(
                                    RW_MC_ISCALE5_X1,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 1));
                                WriteREG(
                                    RW_MC_ISCALE5_X3,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 3));
                                WriteREG(
                                    RW_MC_ISCALE5_X5,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 5));
                                WriteREG(
                                    RW_MC_ISCALE5_X7,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 7));
                                WriteREG(
                                    RW_MC_ISCALE5_X9,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 9));
                                WriteREG(
                                    RW_MC_ISCALE5_X11,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 11));
                                WriteREG(
                                    RW_MC_ISCALE5_X13,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 13));
                                WriteREG(
                                    RW_MC_ISCALE5_X15,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 15));
                                WriteREG(
                                    RW_MC_YSHIFT_OFF5,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iShift
                                     + 32));
                                WriteREG(
                                    RW_MC_CSHIFT_OFF5,
                                    ((128 * 64) + 32
                                     - (128
                                        *rWMVDecInfo.rOther.rNewBotField.New.iScale)));
                                WriteREG(RW_MC_BWD_ICOMP_FLD, BWD_BTM_FLD);
                            }
                        }

                        rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable = 0;

                        if (rWMVDecInfo.rOther.i4BoundaryUMVIcomp == 1)
                        {
                            /* top field picture */
                            if (rWMVDecInfo.rOther.i4SecondFieldParity == 0)
                            {
                                if (rWMVDecInfo.rOther.rOldBotField.New.iEnable
                                        == 1)
                                {
                                    rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable
                                    = 1;
                                    // icomp_set4 is forward bottom field old icomp parameters
                                    if (write_enable == 1)
                                    {
                                        WriteREG(
                                            RW_MC_ISCALE6_X1,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 1));
                                        WriteREG(
                                            RW_MC_ISCALE6_X3,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 3));
                                        WriteREG(
                                            RW_MC_ISCALE6_X5,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 5));
                                        WriteREG(
                                            RW_MC_ISCALE6_X7,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 7));
                                        WriteREG(
                                            RW_MC_ISCALE6_X9,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 9));
                                        WriteREG(
                                            RW_MC_ISCALE6_X11,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 11));
                                        WriteREG(
                                            RW_MC_ISCALE6_X13,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 13));
                                        WriteREG(
                                            RW_MC_ISCALE6_X15,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 15));
                                        WriteREG(
                                            RW_MC_YSHIFT_OFF6,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iShift
                                             + 32));
                                        WriteREG(
                                            RW_MC_CSHIFT_OFF6,
                                            ((128 * 64) + 32
                                             - (128
                                                *rWMVDecInfo.rOther.rOldBotField.New.iScale)));
                                    }
                                }
                            }
                            /* bottom field picture */
                            else
                            {
                                if (rWMVDecInfo.rOther.rOldTopField.New.iEnable
                                        == 1)
                                {
                                    rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable
                                    = 1;
                                    // icomp_set4 is forward bottom field old icomp parameters
                                    if (write_enable == 1)
                                    {
                                        WriteREG(
                                            RW_MC_ISCALE6_X1,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 1));
                                        WriteREG(
                                            RW_MC_ISCALE6_X3,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 3));
                                        WriteREG(
                                            RW_MC_ISCALE6_X5,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 5));
                                        WriteREG(
                                            RW_MC_ISCALE6_X7,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 7));
                                        WriteREG(
                                            RW_MC_ISCALE6_X9,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 9));
                                        WriteREG(
                                            RW_MC_ISCALE6_X11,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 11));
                                        WriteREG(
                                            RW_MC_ISCALE6_X13,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 13));
                                        WriteREG(
                                            RW_MC_ISCALE6_X15,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 15));
                                        WriteREG(
                                            RW_MC_YSHIFT_OFF6,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iShift
                                             + 32));
                                        WriteREG(
                                            RW_MC_CSHIFT_OFF6,
                                            ((128 * 64) + 32
                                             - (128
                                                *rWMVDecInfo.rOther.rOldTopField.New.iScale)));
                                    }
                                }
                            }
                        }

                        if ( (rWMVDecInfo.rOther.rOldBotField.New.iEnable == 1)
                                || (rWMVDecInfo.rOther.rOldBotField.Old.iEnable
                                    == 1)
                                || (rWMVDecInfo.rOther.rNewBotField.New.iEnable
                                    == 1)
                                || (rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable
                                    == 1))
                        {
                            // Icomp enable bits
                            {
                                int
                                b208 =
                                    (rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable)
                                    + (rWMVDecInfo.rOther.rNewBotField.New.iEnable
                                       << 4)
                                    + (rWMVDecInfo.rOther.rOldBotField.Old.iEnable
                                       << 8)
                                    + (0 << 12)
                                    + (rWMVDecInfo.rOther.rOldBotField.New.iEnable
                                       << 16)+ (0
                                                << 20);

                                if (write_enable == 1)
                                {
                                    WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_EN
                                             + ICOMP_Y_EN);
                                    WriteREG(RW_MC_ICOMP2_EN, b208);
                                    WriteREG(RW_MC_SAME_ICOMP,
                                             NO_USE_SAME_ICOPM1_FOR_FRAME);
                                }
                            }
                        }
                        else
                        {
                            if (write_enable == 1)
                            {
                                WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_OFF
                                         + ICOMP_Y_OFF);
                                WriteREG(RW_MC_ICOMP2_EN, 0);
                                WriteREG(RW_MC_SAME_ICOMP,
                                         NO_USE_SAME_ICOPM1_FOR_FRAME);
                            }
                        }
                    }
                    else
                    {
                        //
                        // Last second P field picture is Bottom Field
                        //
                        //
                        // (1) Forward TF == NULL
                        //
                        // (2) Forward BF == NEW_BF
                        // (3) Backward TF == NEW_TF
                        // (4) Backward BF == NULL

                        // icomp_set2 is forward bottom field new icomp parameters
                        if (rWMVDecInfo.rOther.rNewBotField.New.iEnable == 1)
                        {
                            if (write_enable == 1)
                            {
                                WriteREG(
                                    RW_MC_ISCALE2_X1,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 1));
                                WriteREG(
                                    RW_MC_ISCALE2_X3,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 3));
                                WriteREG(
                                    RW_MC_ISCALE2_X5,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 5));
                                WriteREG(
                                    RW_MC_ISCALE2_X7,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 7));
                                WriteREG(
                                    RW_MC_ISCALE2_X9,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 9));
                                WriteREG(
                                    RW_MC_ISCALE2_X11,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 11));
                                WriteREG(
                                    RW_MC_ISCALE2_X13,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 13));
                                WriteREG(
                                    RW_MC_ISCALE2_X15,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iScale
                                     * 15));
                                WriteREG(
                                    RW_MC_YSHIFT_OFF2,
                                    (rWMVDecInfo.rOther.rNewBotField.New.iShift
                                     + 32));
                                WriteREG(
                                    RW_MC_CSHIFT_OFF2,
                                    ((128 * 64) + 32
                                     - (128
                                        *rWMVDecInfo.rOther.rNewBotField.New.iScale)));
                            }
                        }

                        // icomp_set4 is forward bottom field old icomp parameters
                        if (rWMVDecInfo.rOther.rNewBotField.Old.iEnable == 1)
                        {
                            if (write_enable == 1)
                            {
                                WriteREG(
                                    RW_MC_ISCALE4_X1,
                                    (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                     * 1));
                                WriteREG(
                                    RW_MC_ISCALE4_X3,
                                    (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                     * 3));
                                WriteREG(
                                    RW_MC_ISCALE4_X5,
                                    (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                     * 5));
                                WriteREG(
                                    RW_MC_ISCALE4_X7,
                                    (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                     * 7));
                                WriteREG(
                                    RW_MC_ISCALE4_X9,
                                    (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                     * 9));
                                WriteREG(
                                    RW_MC_ISCALE4_X11,
                                    (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                     * 11));
                                WriteREG(
                                    RW_MC_ISCALE4_X13,
                                    (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                     * 13));
                                WriteREG(
                                    RW_MC_ISCALE4_X15,
                                    (rWMVDecInfo.rOther.rNewBotField.Old.iScale
                                     * 15));
                                WriteREG(
                                    RW_MC_YSHIFT_OFF4,
                                    (rWMVDecInfo.rOther.rNewBotField.Old.iShift
                                     + 32));
                                WriteREG(
                                    RW_MC_CSHIFT_OFF4,
                                    ((128 * 64) + 32
                                     - (128
                                        *rWMVDecInfo.rOther.rNewBotField.Old.iScale)));
                            }
                        }

                        // icomp_set5 is backward top or bottom field icomp parameters

                        if (rWMVDecInfo.rOther.rNewTopField.New.iEnable == 1)
                        {
                            if (write_enable == 1)
                            {
                                WriteREG(
                                    RW_MC_ISCALE5_X1,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 1));
                                WriteREG(
                                    RW_MC_ISCALE5_X3,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 3));
                                WriteREG(
                                    RW_MC_ISCALE5_X5,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 5));
                                WriteREG(
                                    RW_MC_ISCALE5_X7,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 7));
                                WriteREG(
                                    RW_MC_ISCALE5_X9,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 9));
                                WriteREG(
                                    RW_MC_ISCALE5_X11,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 11));
                                WriteREG(
                                    RW_MC_ISCALE5_X13,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 13));
                                WriteREG(
                                    RW_MC_ISCALE5_X15,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iScale
                                     * 15));
                                WriteREG(
                                    RW_MC_YSHIFT_OFF5,
                                    (rWMVDecInfo.rOther.rNewTopField.New.iShift
                                     + 32));
                                WriteREG(
                                    RW_MC_CSHIFT_OFF5,
                                    ((128 * 64) + 32
                                     - (128
                                        *rWMVDecInfo.rOther.rNewTopField.New.iScale)));
                                WriteREG(RW_MC_BWD_ICOMP_FLD, BWD_TOP_FLD);
                            }
                        }

                        rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable = 0;

                        if (rWMVDecInfo.rOther.i4BoundaryUMVIcomp == 1)
                        {
                            /* top field picture */
                            if (rWMVDecInfo.rOther.i4SecondFieldParity == 0)
                            {
                                if (rWMVDecInfo.rOther.rOldBotField.New.iEnable
                                        == 1)
                                {
                                    rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable
                                    = 1;
                                    // icomp_set4 is forward bottom field old icomp parameters
                                    if (write_enable == 1)
                                    {
                                        WriteREG(
                                            RW_MC_ISCALE6_X1,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 1));
                                        WriteREG(
                                            RW_MC_ISCALE6_X3,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 3));
                                        WriteREG(
                                            RW_MC_ISCALE6_X5,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 5));
                                        WriteREG(
                                            RW_MC_ISCALE6_X7,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 7));
                                        WriteREG(
                                            RW_MC_ISCALE6_X9,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 9));
                                        WriteREG(
                                            RW_MC_ISCALE6_X11,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 11));
                                        WriteREG(
                                            RW_MC_ISCALE6_X13,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 13));
                                        WriteREG(
                                            RW_MC_ISCALE6_X15,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iScale
                                             * 15));
                                        WriteREG(
                                            RW_MC_YSHIFT_OFF6,
                                            (rWMVDecInfo.rOther.rOldBotField.New.iShift
                                             + 32));
                                        WriteREG(
                                            RW_MC_CSHIFT_OFF6,
                                            ((128 * 64) + 32
                                             - (128
                                                *rWMVDecInfo.rOther.rOldBotField.New.iScale)));
                                    }
                                }
                            }
                            else /* bottom field picture */
                            {
                                if (rWMVDecInfo.rOther.rOldTopField.New.iEnable
                                        == 1)
                                {
                                    rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable
                                    = 1;
                                    // icomp_set4 is forward bottom field old icomp parameters
                                    if (write_enable == 1)
                                    {
                                        WriteREG(
                                            RW_MC_ISCALE6_X1,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 1));
                                        WriteREG(
                                            RW_MC_ISCALE6_X3,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 3));
                                        WriteREG(
                                            RW_MC_ISCALE6_X5,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 5));
                                        WriteREG(
                                            RW_MC_ISCALE6_X7,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 7));
                                        WriteREG(
                                            RW_MC_ISCALE6_X9,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 9));
                                        WriteREG(
                                            RW_MC_ISCALE6_X11,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 11));
                                        WriteREG(
                                            RW_MC_ISCALE6_X13,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 13));
                                        WriteREG(
                                            RW_MC_ISCALE6_X15,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iScale
                                             * 15));
                                        WriteREG(
                                            RW_MC_YSHIFT_OFF6,
                                            (rWMVDecInfo.rOther.rOldTopField.New.iShift
                                             + 32));
                                        WriteREG(
                                            RW_MC_CSHIFT_OFF6,
                                            ((128 * 64) + 32
                                             - (128
                                                *rWMVDecInfo.rOther.rOldTopField.New.iScale)));
                                    }
                                }
                            }
                        }

                        if ( (rWMVDecInfo.rOther.rNewBotField.New.iEnable == 1)
                                || (rWMVDecInfo.rOther.rNewBotField.Old.iEnable
                                    == 1)
                                || (rWMVDecInfo.rOther.rNewTopField.New.iEnable
                                    == 1)
                                || (rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable
                                    == 1))
                        {
                            // Icomp enable bits
                            {
                                int
                                b208 =
                                    (rWMVDecInfo.rOther.i4BoundaryUMVIcompEnable)
                                    + (rWMVDecInfo.rOther.rNewTopField.New.iEnable
                                       << 4)
                                    + (rWMVDecInfo.rOther.rNewBotField.Old.iEnable
                                       << 8) + (0
                                                << 12)
                                    + (rWMVDecInfo.rOther.rNewBotField.New.iEnable
                                       << 16) + (0
                                                 << 20);

                                if (write_enable == 1)
                                {
                                    WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_EN
                                             + ICOMP_Y_EN);
                                    WriteREG(RW_MC_ICOMP2_EN, b208);
                                    WriteREG(RW_MC_SAME_ICOMP,
                                             NO_USE_SAME_ICOPM1_FOR_FRAME);
                                }
                            }
                        }
                        else
                        {
                            if (write_enable == 1)
                            {
                                WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_OFF
                                         + ICOMP_Y_OFF);
                                WriteREG(RW_MC_ICOMP2_EN, 0);
                                WriteREG(RW_MC_SAME_ICOMP,
                                         NO_USE_SAME_ICOPM1_FOR_FRAME);
                            }
                        }
                    }
                }
            }
        }
        else// B frame pciture
        {
            rWMVDecInfo.rOther.i4BoundaryUMVIcomp = 0;

            // (1) Forward TF == NEW_TF
            // (2) Forward BF == NEW_BF
            // (3) Backward TF == NULL
            // (4) Backward BF == NULL

            /* Step 3: output MC parameters */
            // icomp_set1 is forward top field new icomp parameters
            if (rWMVDecInfo.rOther.rNewTopField.New.iEnable == 1)
            {
                if (write_enable == 1)
                {
                    WriteREG(RW_MC_ISCALE1_X1,
                             (rWMVDecInfo.rOther.rNewTopField.New.iScale * 1));
                    WriteREG(RW_MC_ISCALE1_X3,
                             (rWMVDecInfo.rOther.rNewTopField.New.iScale * 3));
                    WriteREG(RW_MC_ISCALE1_X5,
                             (rWMVDecInfo.rOther.rNewTopField.New.iScale * 5));
                    WriteREG(RW_MC_ISCALE1_X7,
                             (rWMVDecInfo.rOther.rNewTopField.New.iScale * 7));
                    WriteREG(RW_MC_ISCALE1_X9,
                             (rWMVDecInfo.rOther.rNewTopField.New.iScale * 9));
                    WriteREG(RW_MC_ISCALE1_X11,
                             (rWMVDecInfo.rOther.rNewTopField.New.iScale * 11));
                    WriteREG(RW_MC_ISCALE1_X13,
                             (rWMVDecInfo.rOther.rNewTopField.New.iScale * 13));
                    WriteREG(RW_MC_ISCALE1_X15,
                             (rWMVDecInfo.rOther.rNewTopField.New.iScale * 15));
                    WriteREG(RW_MC_YSHIFT_OFF1,
                             (rWMVDecInfo.rOther.rNewTopField.New.iShift + 32));
                    WriteREG(RW_MC_CSHIFT_OFF1, ((128 * 64) + 32 - (128
                                                 *rWMVDecInfo.rOther.rNewTopField.New.iScale)));
                }
            }

            //  icomp_set2 is forward bottom field new icomp parameters
            if (rWMVDecInfo.rOther.rNewBotField.New.iEnable == 1)
            {
                if (write_enable == 1)
                {
                    WriteREG(RW_MC_ISCALE2_X1,
                             (rWMVDecInfo.rOther.rNewBotField.New.iScale * 1));
                    WriteREG(RW_MC_ISCALE2_X3,
                             (rWMVDecInfo.rOther.rNewBotField.New.iScale * 3));
                    WriteREG(RW_MC_ISCALE2_X5,
                             (rWMVDecInfo.rOther.rNewBotField.New.iScale * 5));
                    WriteREG(RW_MC_ISCALE2_X7,
                             (rWMVDecInfo.rOther.rNewBotField.New.iScale * 7));
                    WriteREG(RW_MC_ISCALE2_X9,
                             (rWMVDecInfo.rOther.rNewBotField.New.iScale * 9));
                    WriteREG(RW_MC_ISCALE2_X11,
                             (rWMVDecInfo.rOther.rNewBotField.New.iScale * 11));
                    WriteREG(RW_MC_ISCALE2_X13,
                             (rWMVDecInfo.rOther.rNewBotField.New.iScale * 13));
                    WriteREG(RW_MC_ISCALE2_X15,
                             (rWMVDecInfo.rOther.rNewBotField.New.iScale * 15));
                    WriteREG(RW_MC_YSHIFT_OFF2,
                             (rWMVDecInfo.rOther.rNewBotField.New.iShift + 32));
                    WriteREG(RW_MC_CSHIFT_OFF2, ((128 * 64) + 32 - (128
                                                 *rWMVDecInfo.rOther.rNewBotField.New.iScale)));
                }
            }

            if (rWMVDecInfo.rOther.rNewTopField.Old.iEnable == 1)
            {
                // icomp_set3 is forward top field old icomp parameters
                if (write_enable == 1)
                {
                    WriteREG(RW_MC_ISCALE3_X1,
                             (rWMVDecInfo.rOther.rNewTopField.Old.iScale * 1));
                    WriteREG(RW_MC_ISCALE3_X3,
                             (rWMVDecInfo.rOther.rNewTopField.Old.iScale * 3));
                    WriteREG(RW_MC_ISCALE3_X5,
                             (rWMVDecInfo.rOther.rNewTopField.Old.iScale * 5));
                    WriteREG(RW_MC_ISCALE3_X7,
                             (rWMVDecInfo.rOther.rNewTopField.Old.iScale * 7));
                    WriteREG(RW_MC_ISCALE3_X9,
                             (rWMVDecInfo.rOther.rNewTopField.Old.iScale * 9));
                    WriteREG(RW_MC_ISCALE3_X11,
                             (rWMVDecInfo.rOther.rNewTopField.Old.iScale * 11));
                    WriteREG(RW_MC_ISCALE3_X13,
                             (rWMVDecInfo.rOther.rNewTopField.Old.iScale * 13));
                    WriteREG(RW_MC_ISCALE3_X15,
                             (rWMVDecInfo.rOther.rNewTopField.Old.iScale * 15));
                    WriteREG(RW_MC_YSHIFT_OFF3,
                             (rWMVDecInfo.rOther.rNewTopField.Old.iShift + 32));
                    WriteREG(RW_MC_CSHIFT_OFF3, ((128 * 64) + 32 - (128
                                                 *rWMVDecInfo.rOther.rNewTopField.Old.iScale)));
                }
            }

            if (rWMVDecInfo.rOther.rNewBotField.Old.iEnable == 1)
            {
                // icomp_set4 is forward bottom field old icomp parameters
                if (write_enable == 1)
                {
                    WriteREG(RW_MC_ISCALE4_X1,
                             (rWMVDecInfo.rOther.rNewBotField.Old.iScale * 1));
                    WriteREG(RW_MC_ISCALE4_X3,
                             (rWMVDecInfo.rOther.rNewBotField.Old.iScale * 3));
                    WriteREG(RW_MC_ISCALE4_X5,
                             (rWMVDecInfo.rOther.rNewBotField.Old.iScale * 5));
                    WriteREG(RW_MC_ISCALE4_X7,
                             (rWMVDecInfo.rOther.rNewBotField.Old.iScale * 7));
                    WriteREG(RW_MC_ISCALE4_X9,
                             (rWMVDecInfo.rOther.rNewBotField.Old.iScale * 9));
                    WriteREG(RW_MC_ISCALE4_X11,
                             (rWMVDecInfo.rOther.rNewBotField.Old.iScale * 11));
                    WriteREG(RW_MC_ISCALE4_X13,
                             (rWMVDecInfo.rOther.rNewBotField.Old.iScale * 13));
                    WriteREG(RW_MC_ISCALE4_X15,
                             (rWMVDecInfo.rOther.rNewBotField.Old.iScale * 15));
                    WriteREG(RW_MC_YSHIFT_OFF4,
                             (rWMVDecInfo.rOther.rNewBotField.Old.iShift + 32));
                    WriteREG(RW_MC_CSHIFT_OFF4, ((128 * 64) + 32 - (128
                                                 *rWMVDecInfo.rOther.rNewBotField.Old.iScale)));
                }
            }

            if ( (rWMVDecInfo.rOther.rNewTopField.New.iEnable == 1)
                    || (rWMVDecInfo.rOther.rNewBotField.New.iEnable == 1)
                    || (rWMVDecInfo.rOther.rNewTopField.Old.iEnable == 1)
                    || (rWMVDecInfo.rOther.rNewBotField.Old.iEnable == 1))
            {
                // Icomp enable bits
                {
                    int b208 = (rWMVDecInfo.rOther.rNewBotField.Old.iEnable
                                << 8)+ (rWMVDecInfo.rOther.rNewTopField.Old.iEnable
                                        <<12)+ (rWMVDecInfo.rOther.rNewBotField.New.iEnable
                                                <<16)+ (rWMVDecInfo.rOther.rNewTopField.New.iEnable
                                                        <<20);

                    if (write_enable == 1)
                    {
                        WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_EN + ICOMP_Y_EN);
                        WriteREG(RW_MC_ICOMP2_EN, b208);
                        WriteREG(RW_MC_SAME_ICOMP, NO_USE_SAME_ICOPM1_FOR_FRAME);
                    }
                }
            }
            else
            {
                if (write_enable == 1)
                {
                    WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_OFF + ICOMP_Y_OFF);
                    WriteREG(RW_MC_ICOMP2_EN, 0);
                    WriteREG(RW_MC_SAME_ICOMP, NO_USE_SAME_ICOPM1_FOR_FRAME);
                }
            }
        }

    } //}
    else /* pure frame picture case in WMV9 */
    {

        if (write_enable == 1)
        {
            if (rWMVDecInfo.rOther.i4CodecVersion >= WMV3)
            {
                WriteREG(RW_MC_SAME_ICOMP, USE_SAME_ICOMP1_FOR_FRAME);
            }
        }

        if (rWMVDecInfo.rPicLayer.u1PicType == IVOP)
        {
            rWMVDecInfo.rOther.i4FirstFieldIntensityComp = 0;
            if (write_enable == 1)
            {
                WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_OFF + ICOMP_Y_OFF);
                if (rWMVDecInfo.rOther.i4CodecVersion >= WMV3)
                {
                    WriteREG(RW_MC_ICOMP_TYPE, NO_ICOMP);
                }
            }
        }
        else if ( (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
                  || (rWMVDecInfo.rPicLayer.u1PicType == SKIPFRAME))/* only for PVOP */
        {
            if (rWMVDecInfo.rOther.fgLuminanceWarp)
            {
                rWMVDecInfo.rOther.i4FirstFieldIntensityComp = 1;
                if (write_enable == 1)
                {
                    WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_EN + ICOMP_Y_EN);
                    if (rWMVDecInfo.rOther.i4CodecVersion >= WMV3)
                    {
                        WriteREG(RW_MC_ICOMP_TYPE, FRAME_ICOMP);
                    }
                }
            }
            else
            {
                rWMVDecInfo.rOther.i4FirstFieldIntensityComp = 0;
                if (write_enable == 1)
                {
                    WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_OFF + ICOMP_Y_OFF);
                    if (rWMVDecInfo.rOther.i4CodecVersion >= WMV3)
                    {
                        WriteREG(RW_MC_ICOMP_TYPE, NO_ICOMP);
                    }
                }
            }
        }
        else if (rWMVDecInfo.rPicLayer.u1PicType == BVOP)
        {
            /* just follow the setting of last decoded P picture */
            if (rWMVDecInfo.rOther.i4FirstFieldIntensityComp == 1)
            {
                if (write_enable == 1)
                {
                    WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_EN + ICOMP_Y_EN);
                    if (rWMVDecInfo.rOther.i4CodecVersion >= WMV3)
                    {
                        WriteREG(RW_MC_ICOMP_TYPE, FRAME_ICOMP);
                    }
                }
            }
            else
            {
                if (write_enable == 1)
                {
                    WriteREG(RW_MC_WMV9_ICOMP_EN, ICOMP_C_OFF + ICOMP_Y_OFF);
                    if (rWMVDecInfo.rOther.i4CodecVersion >= WMV3)
                    {
                        WriteREG(RW_MC_ICOMP_TYPE, NO_ICOMP);
                    }
                }
            }
        }

        /* derived from interpolate_wmv9.c IntensityCompensation() */
        if (rWMVDecInfo.rOther.i4LumShift > 31)
            rWMVDecInfo.rOther.i4LumShift -= 64;

        // remap luminance scale and shift
        if (rWMVDecInfo.rOther.i4LumScale == 0)
        {
            iScale = - 64;
            iShift = 255 * 64 - rWMVDecInfo.rOther.i4LumShift * 2 * 64;
        }
        else
        {
            iScale = rWMVDecInfo.rOther.i4LumScale + 32;
            iShift = rWMVDecInfo.rOther.i4LumShift * 64;
        }
        if (write_enable == 1)
        {
            int c208;

            if (rWMVDecInfo.rOther.i4FirstFieldIntensityComp == 1)
            {
                c208 = (0) + (0 << 4) + (0 << 8) + (0 << 12) + (0 << 16) + (1
                        << 20);
            }
            else
            {
                c208 = 0;
            }
            WriteREG(RW_MC_ISCALE1_X1, (iScale * 1));
            WriteREG(RW_MC_ISCALE1_X3, (iScale * 3));
            WriteREG(RW_MC_ISCALE1_X5, (iScale * 5));
            WriteREG(RW_MC_ISCALE1_X7, (iScale * 7));
            WriteREG(RW_MC_ISCALE1_X9, (iScale * 9));
            WriteREG(RW_MC_ISCALE1_X11, (iScale * 11));
            WriteREG(RW_MC_ISCALE1_X13, (iScale * 13));
            WriteREG(RW_MC_ISCALE1_X15, (iScale * 15));
            WriteREG(RW_MC_YSHIFT_OFF1, (iShift + 32));
            WriteREG(RW_MC_CSHIFT_OFF1, ((128 * 64) + 32 - (128*iScale)));
            if (rWMVDecInfo.rOther.i4CodecVersion >= WMV3)
                WriteREG(RW_MC_ICOMP2_EN, c208);
        }
    }

    //-------------------------------------------------------
    // MC_reg_168 : MV type (0:BILINEAR, 1:BICUBIC)
    //-------------------------------------------------------
    if (rWMVDecInfo.rOther.i4CodecVersion >= WMV3)
    {
        if (rWMVDecInfo.rOther.i4X9MVMode == ALL_1MV_HALFPEL_BILINEAR)
        {   /* (pWMVDec->m_iFilterType == FILTER_BICUBIC) */
            WriteREG(RW_MC_FILTER_TYPE, (Y_BILINEAR + C_BILINEAR));
        }
        else
        {   /* FILTER_BILINEAR */
            WriteREG(RW_MC_FILTER_TYPE, (Y_BICUBIC + C_BILINEAR));
        }
    }
    else /* rWMVDecInfo.rOther.i4CodecVersion == WMV2  */
    {
        if ( (rWMVDecInfo.rOther.i4CodecVersion == WMV2)
                && (rWMVDecInfo.rPicLayer.fgMvResolution == 1))
        {
            WriteREG(RW_MC_FILTER_TYPE, (Y_BICUBIC + C_BILINEAR));
        }
        else
        {
            WriteREG(RW_MC_FILTER_TYPE, (Y_BILINEAR + C_BILINEAR));
        }
    }

    //-------------------------------------------------------
    // MC_reg_169 : MC clip signal (1:turn on)
    //-------------------------------------------------------
    if (rWMVDecInfo.rPicLayer.i4Overlap & 1)
    {   /* clip operation will be handled by overlap_smooth module */
        WriteREG(RW_MC_WRITE_BUS_TYPE, UNCLIP_TYPE);
    }
    else
    {
        WriteREG(RW_MC_WRITE_BUS_TYPE, CLIP_0_255_TYPE);
    }

    //----------------------------------------------------------------------
    // MC_reg_170 : indicate if the intra_blocks need to add 128 (1:turn on)
    //----------------------------------------------------------------------
    if (rWMVDecInfo.rOther.i4CodecVersion >= WMV3)
    {
        //simple/main I
        if ((rWMVDecInfo.rPicLayer.u1PicType == IVOP
                || rWMVDecInfo.rPicLayer.u1PicType == BIVOP)
                && (rWMVDecInfo.rOther.i4CodecVersion == WMV3))
        {
            WriteREG(RW_MC_INTRA_BLK_ADD128, ADD128_OFF);
        }
        else
        {
            if (rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFRAME)
            {
                WriteREG(RW_MC_INTRA_BLK_ADD128, ADD128_ON);
            }
            else if (rWMVDecInfo.rPicLayer.i4Overlap & 1)
            {   /* add 128 will be handled by overlap_smooth module */
                WriteREG(RW_MC_INTRA_BLK_ADD128, ADD128_OFF);
            }
            else
            {   /* enable add 128 to intra mb */
                WriteREG(RW_MC_INTRA_BLK_ADD128, ADD128_ON);
            }
        }
    }
    else /* rWMVDecInfo.rOther.i4CodecVersion == WMV2 */
    {
        if (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
        {
            WriteREG(RW_MC_INTRA_BLK_ADD128, ADD128_OFF);
        }
        else /* IVOP */
        {
            WriteREG(RW_MC_INTRA_BLK_ADD128, ADD128_OFF);
        }
    }

    //------------------------------------------
    // MC_reg_173 : fast_uvmc (1:turn on)
    //------------------------------------------
    WriteREG(RW_MC_FAST_UVMC, rWMVDecInfo.rEntryPoint.fgUVHpelBilinear);

    //
    // In-loop deblocking parameters
    //

    //------------------------------------------
    // MC_reg_136,148 : post process (1:turn on)
    //------------------------------------------
    if ((rWMVDecInfo.rEntryPoint.fgLoopFilter == 1)
            || (rWMVDecInfo.rPicLayer.i4Overlap & 1))
    {
        WriteREG(RW_MC_PP_ENABLE, 0x00000001);
        WriteREG(RW_MC_PP_WB_BY_POST, 0x00000001);
    }
    else
    {
        WriteREG(RW_MC_PP_ENABLE, 0x00000000);
        WriteREG(RW_MC_PP_WB_BY_POST, 0x00000000);
    }

    //------------------------------------------
    // MC_reg_142 : loopfielter (3:turn on)
    //------------------------------------------
    if (rWMVDecInfo.rEntryPoint.fgLoopFilter == 1)
    {
        WriteREG(RW_MC_PP_DBLK_MODE, 0x00000003);
    }
    else
    {
        WriteREG(RW_MC_PP_DBLK_MODE, 0x00000000);
    }

    //------------------------------------------
    // MC_reg_175 : overlap smoothing (1:turn on)
    //------------------------------------------
    if (rWMVDecInfo.rPicLayer.i4Overlap & 1)
    {
        WriteREG(RW_MC_OVL_SMTH_FILTER, OVL_EN);
    }
    else
    {
        WriteREG(RW_MC_OVL_SMTH_FILTER, OVL_OFF);
    }

    //------------------------------------------
    // MC_reg_152,153 : start_MB # & end_MB #
    //------------------------------------------
    WriteREG(RW_MC_PP_X_RANGE, rWMVDecInfo.rSeqHdrR.u4NumMBX-1);
    WriteREG(RW_MC_PP_Y_RANGE, rWMVDecInfo.rSeqHdrR.u4NumMBY-1);

    //------------------------------------------
    // MC_reg_139 : picture width in MB
    //------------------------------------------
    WriteREG(RW_MC_PP_MB_WIDTH, rWMVDecInfo.rSeqHdrR.u4NumMBX);

    //-----------------------------------------------------
    // MC_reg_137,138 : dram base address for post process
    //-----------------------------------------------------
    if ((rWMVDecInfo.rPicLayer.u1PicType == BVOP)
            || (rWMVDecInfo.rPicLayer.u1PicType == BIVOP)) //prog_B
    {
        WriteREG(RW_MC_PP_Y_ADDR, ((UINT32)(rWMVDecInfo.rDram.pu1Pic2Y)) >> 9);
        WriteREG(RW_MC_PP_C_ADDR, ((UINT32)(rWMVDecInfo.rDram.pu1Pic2C)) >> 8);
        //x_memset(rWMVDecInfo.rDram.pu1Pic2Y, 0x80, PIC_Y_SZ + PIC_C_SZ);
    }
    else if (out_ctrl == 0) //pic1
    {
        WriteREG(RW_MC_PP_Y_ADDR, ((UINT32)(rWMVDecInfo.rDram.pu1Pic0Y)) >> 9);
        WriteREG(RW_MC_PP_C_ADDR, ((UINT32)(rWMVDecInfo.rDram.pu1Pic0C)) >> 8);
        //x_memset(rWMVDecInfo.rDram.pu1Pic0Y, 0x80, PIC_Y_SZ + PIC_C_SZ);

    }
    else //pic2
    {
        WriteREG(RW_MC_PP_Y_ADDR, ((UINT32)(rWMVDecInfo.rDram.pu1Pic1Y)) >> 9);
        WriteREG(RW_MC_PP_C_ADDR, ((UINT32)(rWMVDecInfo.rDram.pu1Pic1C)) >> 8);
        //x_memset(rWMVDecInfo.rDram.pu1Pic1Y, 0x80, PIC_Y_SZ + PIC_C_SZ);

    }

    if (rWMVDecInfo.rOther.i4CodecVersion == WMVA)
    {
        //-----------------------------------------------------
        // MC_reg_176 : conditional overlap flag (1:turn on)
        //-----------------------------------------------------
        WriteREG(RW_MC_COND_OVL_FILTER,
                 (rWMVDecInfo.rPicLayer.i4Overlap & 4) ? COND_OVL_EN
                 : COND_OVL_OFF);

        //-----------------------------------------------------
        // MC_reg_177 : for fix MS ref_code bug (bit[0] [8] [16])
        //-----------------------------------------------------
        WriteREG(RW_MC_PP_DBLK_OPT, VC1_ALL_SET_ONE);

        //-----------------------------------------------------
        // MC_reg_184 : Range Map flag (bit1:y, bit0:c)
        //-----------------------------------------------------

        //-----------------------------------------------------
        // MC_reg_185 : Range Map parameter
        //-----------------------------------------------------
        //Give it to Display module.
        WriteREG(
            RW_MC_RNG_PARA,
            ((rWMVDecInfo.rEntryPoint.i4RangeRedY>0) ? rWMVDecInfo.rEntryPoint.i4RangeRedY
             -1
             : 0)*256
            + ((rWMVDecInfo.rEntryPoint.i4Range_MapUV>0) ? rWMVDecInfo.rEntryPoint.i4Range_MapUV
               -1
               : 0));
    }
    else
    {
        //-----------------------------------------------------
        // MC_reg_177 : for fix MS ref_code bug (bit[0] [8] [16])
        //-----------------------------------------------------
        WriteREG(RW_MC_PP_DBLK_OPT, NOT_VC1_ALL_SET_ZERO);
    }

    //---------------------------------------------------------
    // MC_reg_231 : referece picture
    //---------------------------------------------------------
    if (rWMVDecInfo.rOther.i4CodecVersion >= WMV3)
    {
        if (write_enable == 1)
        {
            int
            b231 =
                ((rWMVDecInfo.rDram.u4BackwardRefPicType == 0) ? BWD_PROG_REF_PIC
                 : BWD_INTLCE_REF_PIC)
                + (((rWMVDecInfo.rDram.u4ForwardRefPicType
                     == 0) ? FWD_PROG_REF_PIC
                    : FWD_INTLCE_REF_PIC));//0: 1) << 16);
            WriteREG(RW_MC_REF_PIC_TYPE, b231);
        }
    }

    //------------------------------
    //   PRED RISC WRITE PATTERN
    //------------------------------

    //-----------------------------------------------------
    // VLD_reg_143 : prediction parameter
    //-----------------------------------------------------

    //-----------------------------------------------------
    // VLD_reg_193 : dcac prediction rule selection
    //-----------------------------------------------------
    //for dc_pred rule selection : Rule_A or Rule_B
    if ((rWMVDecInfo.rOther.i4CodecVersion == WMV1)
            || (rWMVDecInfo.rOther.i4CodecVersion == WMV2))
    {
        use_interpredictor = (rWMVDecInfo.rSeqLayer.i4BitRate
                              <= MAX_BITRATE_DCPred_IMBInPFrame
                              && (rWMVDecInfo.rSeqHdrR.u4PicWidthSrc
                                  * rWMVDecInfo.rSeqHdrR.u4PicHeightSrc < 320 * 240))
                             && (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
                             && (rWMVDecInfo.rOther.i4CodecVersion == WMV1);
        use_rule_b
        = (((rWMVDecInfo.rPicLayer.i4Overlap & 0x1) == 0)
           && (rWMVDecInfo.rPicLayer.u1PicType == IVOP)
           && ((rWMVDecInfo.rSeqLayer.i4WMV3Profile
                == WMV3_SIMPLE_PROFILE)
               || (rWMVDecInfo.rSeqLayer.i4WMV3Profile
                   == WMV3_MAIN_PROFILE)))
          || use_interpredictor
          | (rWMVDecInfo.rOther.i4CodecVersion <= WMV2);
        rWMVDecInfo.rOther.use_rule_b = use_rule_b;
        use_quarter_pel_mv = 0;
        rWMVDecInfo.rOther.use_quarter_pel_mv = use_quarter_pel_mv;

        a193 = //0x0021c0b4 +
            (((rWMVDecInfo.rSeqHdrR.u4PicWidthSrc+15)/16)*4)+
            ((((rWMVDecInfo.rSeqHdrR.u4PicWidthSrc+15)/16)*4*3)<<12)+
            (use_rule_b << 27) + (use_interpredictor << 28)
            + (use_quarter_pel_mv
               /*(pWMVDec->m_pAltTables->m_iHalfPelMV == 0)*/<< 29) + /* quarter-pel */
            ( 0 << 30); //dcac overlap

    }
    else // >= WMV3
    {
        use_interpredictor = 0;
        use_rule_b
        = ((rWMVDecInfo.rPicLayer.i4Overlap & 0x1) == 0)
          && ((rWMVDecInfo.rPicLayer.u1PicType == IVOP)
              || (rWMVDecInfo.rPicLayer.u1PicType == BIVOP))
          && ((rWMVDecInfo.rSeqLayer.i4WMV3Profile
               == WMV3_SIMPLE_PROFILE)
              || (rWMVDecInfo.rSeqLayer.i4WMV3Profile
                  == WMV3_MAIN_PROFILE));
        rWMVDecInfo.rOther.use_rule_b = use_rule_b;
        use_quarter_pel_mv = (rWMVDecInfo.rPicLayer.u1FrameCodingMode
                              == INTERLACEFRAME) ? 1 : ((rWMVDecInfo.rOther.i4X9MVMode
                                      == ALL_1MV_HALFPEL || rWMVDecInfo.rOther.i4X9MVMode
                                      == ALL_1MV_HALFPEL_BILINEAR)) ? 0 : 1;
        rWMVDecInfo.rOther.use_quarter_pel_mv = use_quarter_pel_mv;

        a193 = //0x0021c0b4 +
            (((rWMVDecInfo.rSeqHdrR.u4PicWidthSrc+15)/16)*4)
            + ((((rWMVDecInfo.rSeqHdrR.u4PicWidthSrc+15)/16)*4*3)
               <<12)+ (use_rule_b << 27) + (use_interpredictor
                                            << 28) + (use_quarter_pel_mv
                                                    /*(pWMVDec->m_pAltTables->m_iHalfPelMV == 0)*/<< 29) + /* quarter-pel */
            ( 0 << 30); //dcac overlap
    }

    WriteREG(RW_VLD_DCACPITCH, a193);

    //-----------------------------------------------------
    // VLD_reg_198 : bit-plane mode & address
    //-----------------------------------------------------
    //ilii
    a198 = (((UINT32)rWMVDecInfo.rDram.pu1Bp_1 >> 2) & 0x00FFFFFF)
           + (((UINT32)rWMVDecInfo.rDram.pu1Bp_1 << 2) & 0xF0000000)
           + (rWMVDecInfo.rOther.i4pred_use_wdle << 25) + //wdle
           ( 1 << 26);
    WriteREG(RW_VLD_BCODE_SA, a198); //(UINT32)rWMVDecInfo.rDram.pu1Bp_1 >> 3); //198 Bp_1


    //-----------------------------------------------------
    // VLD_reg_200 : MV resolution (half or full pel)
    //-----------------------------------------------------
    {
        int mv_sram_use_ok, mv_sram_use;
        int my_forward /*my_output*/;
        int bit22;
        if ((rWMVDecInfo.rOther.i4CodecVersion == WMV1)
                || (rWMVDecInfo.rOther.i4CodecVersion == WMV2))
        {
            rWMVDecInfo.rOther.u4ResetMVDram = 0;
            mv_sram_use_ok = 0;
            mv_sram_use = 1;
            //ilii
            // 1:  整張picture有超過一個以上的slice
            // 0: 整張picture只有一個slice
            if (rWMVDecInfo.rOther.i4CodecVersion == WMV2)
                bit22 = (!(rWMVDecInfo.rSeqHdrR.u4NumMBY
                           == (rWMVDecInfo.rSeqHdrR.u4NumMBY
                               / (rWMVDecInfo.rSMProfile.i4SliceCode))));
            else
                bit22 = (!(rWMVDecInfo.rSeqHdrR.u4NumMBY
                           == (rWMVDecInfo.rSeqHdrR.u4NumMBY
                               / (rWMVDecInfo.rSMProfile.i4SliceCode - 22)))); // 5bits for WMV1, -22 to 0~7

            a200 = 0x0020003a + (1<<11)+ (rWMVDecInfo.rOther.i4pred_use_wdle
                                          << 6)+ (rWMVDecInfo.rOther.i4pred_use_wdle << 7)
                   + (rWMVDecInfo.rOther.i4pred_use_wdle << 20)
                   + ((rWMVDecInfo.rOther.i4Panning & 0xf != 0) << 9)
                   + ((rWMVDecInfo.rOther.i4CodecVersion == WMV1
                       || rWMVDecInfo.rOther.i4CodecVersion == WMV2)<<15)
                   + (rWMVDecInfo.rOther.u4ResetMVDram << 18) +
                   //((!(rWMVDecInfo.rSeqHdrR.u4NumMBY == (rWMVDecInfo.rSeqHdrR.u4NumMBY / (rWMVDecInfo.rSMProfile.i4SliceCode - 22)))) << 22) +
                   (bit22 << 22) + (mv_sram_use << 31);

        }
        else // >= WMV3
        {
            mv_sram_use_ok = ( (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
                               || ((rWMVDecInfo.rPicLayer.u1PicType == BVOP)
                                   &&(rWMVDecInfo.rPicLayer.u1FrameCodingMode
                                      != INTERLACEFIELD)) )
                             && (rWMVDecInfo.rSeqHdrR.u4NumMBX < 46);
            mv_sram_use = mv_sram_use_ok;// // simulation use only ? (rand()% 2) : 0;

            my_forward = ReadREG(RW_MC_FWDP);
            //my_output = ReadREG(RW_MC_OPBUF);

            vSetMcBufPtr();

            if ((rWMVDecInfo.rOther.u1FrameTypeLast == SKIPFRAME)
                    && (rWMVDecInfo.rPicLayer.u1PicType == BVOP))
            {
                if (my_forward == 1)
                {
                    WriteREG(RW_MC_R1Y, ((UINT32)(rWMVDecInfo.rDram.pu1Pic0Y) )
                             >> 9); // div 512
                    WriteREG(RW_MC_R1C, ((UINT32)(rWMVDecInfo.rDram.pu1Pic0C) )
                             >> 8); // div 256
                    WriteREG(RW_MC_R2Y, ((UINT32)(rWMVDecInfo.rDram.pu1Pic0Y) )
                             >> 9); // div 512
                    WriteREG(RW_MC_R2C, ((UINT32)(rWMVDecInfo.rDram.pu1Pic0C) )
                             >> 8); // div 256
                }
                else
                {
                    WriteREG(RW_MC_R1Y, ((UINT32)(rWMVDecInfo.rDram.pu1Pic1Y) )
                             >> 9); // div 512
                    WriteREG(RW_MC_R1C, ((UINT32)(rWMVDecInfo.rDram.pu1Pic1C) )
                             >> 8); // div 256
                    WriteREG(RW_MC_R2Y, ((UINT32)(rWMVDecInfo.rDram.pu1Pic1Y) )
                             >> 9); // div 512
                    WriteREG(RW_MC_R2C, ((UINT32)(rWMVDecInfo.rDram.pu1Pic1C) )
                             >> 8); // div 256
                }
            }

            if (rWMVDecInfo.rOther.u1FrameTypeLast == SKIPFRAME)
            {
                if ((rWMVDecInfo.rOther.u4ResetMVDram == 1)
                        && ((rWMVDecInfo.rPicLayer.u1PicType == IVOP)
                            || (rWMVDecInfo.rPicLayer.u1PicType == PVOP)))
                    rWMVDecInfo.rOther.u4ResetMVDram = 0;
                else
                {
                    rWMVDecInfo.rOther.u4ResetMVDram = 1;
                }
            }
            else
            {
                if (((rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
                        && (((rWMVDecInfo.rPicLayer.i4CurrentField == 0)
                             &&(rWMVDecInfo.rOther.u1FrameTypeLastTop
                                == IVOP))
                            || ((rWMVDecInfo.rPicLayer.i4CurrentField == 1)
                                &&(rWMVDecInfo.rOther.u1FrameTypeLastBot
                                   == IVOP))) )
                        || ((rWMVDecInfo.rPicLayer.u1FrameCodingMode
                             != INTERLACEFIELD) && //ming modify @3/14
                            (rWMVDecInfo.rOther.u1FrameTypeLast == IVOP) ))
                {
                    if (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
                        rWMVDecInfo.rOther.u4new_entry_point = 0;
                    rWMVDecInfo.rOther.u4ResetMVDram = 1;
                }
                else if (rWMVDecInfo.rOther.u4new_entry_point == 1)
                {
                    if (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
                        rWMVDecInfo.rOther.u4new_entry_point = 0;
                    rWMVDecInfo.rOther.u4ResetMVDram = 1;
                }
                else
                    rWMVDecInfo.rOther.u4ResetMVDram = 0;
            }

            if ((rWMVDecInfo.rPicLayer.u1PicType == IVOP)
                    || (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
                    || (rWMVDecInfo.rPicLayer.u1PicType == SKIPFRAME))
            {

                /*#ifdef REPEAT_FRAME
                 if(rWMVDecInfo.rOther.u4CurrPicNo == REPEAT_FRAME)
                 {
                 // do nothing
                 }
                 else
                 #endif*/
                {
                    rWMVDecInfo.rOther.u1FrameTypeLast
                    = rWMVDecInfo.rPicLayer.u1PicType;

                    if (rWMVDecInfo.rPicLayer.i4CurrentField == 0) //top
                        rWMVDecInfo.rOther.u1FrameTypeLastTop
                        = rWMVDecInfo.rPicLayer.u1PicType;
                    else
                        //bot
                        rWMVDecInfo.rOther.u1FrameTypeLastBot
                        = rWMVDecInfo.rPicLayer.u1PicType;
                }
            } //ming modify@07/02/10

            a200 = 0x00200032 +
                   //a200 = 0x00200030 +
                   (rWMVDecInfo.rOther.i4pred_use_wdle << 6)
                   + (rWMVDecInfo.rOther.i4pred_use_wdle << 7)
                   + (rWMVDecInfo.rOther.i4pred_use_wdle << 20)
                   + ((rWMVDecInfo.rOther.i4Panning & 0xf != 0) << 9)
                   + ((rWMVDecInfo.rOther.i4CodecVersion == WMV1
                       || rWMVDecInfo.rOther.i4CodecVersion == WMV2)<<15)
                   + (rWMVDecInfo.rOther.u4ResetMVDram << 18) + (mv_sram_use
                           << 31);// +
            //((rWMVDecInfo.rOther.i4X9MVMode == ALL_1MV_HALFPEL_BILINEAR) << 25) +
            //((rWMVDecInfo.rEntryPoint.fgUVHpelBilinear == 1) << 26)+
            //(rWMVDecInfo.rPicLayer.i4CurrentTemporalField << 27) +
            //(rWMVDecInfo.rSeqLayer.fgVC1 << 28) +
            //(pWMVDec->m_bUseOppFieldForRef << 29);
        }
        //a200 = a200 | 0x1400;
        //LOG(0,"a200 %x\n",a200);
        WriteREG(RW_VLD_DCMVSEL, a200);
    }

    //-----------------------------------------------------
    // VLD_reg_206 : MV Range
    //-----------------------------------------------------
    if ((rWMVDecInfo.rOther.i4CodecVersion == WMV1)
            || (rWMVDecInfo.rOther.i4CodecVersion == WMV2))
    {
        a206 = (rWMVDecInfo.rOther.i4CodecVersion <= WMV2) + //wmv7/8
               ((rWMVDecInfo.rOther.i4CodecVersion <= WMV2) << 1)+ //wmv7/8
               ((rWMVDecInfo.rOther.i4CodecVersion == WMV3) << 2)+ //wmv9
               (0 << 3)+ //wmva
               ((rWMVDecInfo.rEntryPoint.i4MVRangeIndex&0xf)<<4)+//mv_range
               (0 << 8)+//interlace_420
               (0 << 9) + //interlace_411
               (((rWMVDecInfo.rOther.i4X9MVMode == ALL_1MV_HALFPEL_BILINEAR)
                 || (rWMVDecInfo.rOther.i4X9MVMode == ALL_1MV_HALFPEL))
                << 10) +//VWMVHalfFilterSel
               ((rWMVDecInfo.rEntryPoint.fgUVHpelBilinear == 1) << 11)+ //VWMVrounding
               (0 << 12) + //VWMVHalfPelBackRef
               (0 << 13) + //VWMVPostCrescent
               (0 << 14) + //VWMVUseOppieldRef
               (0 << 15) + //VWMVinter420Field2ref
               (0 << 16)+ //VWMVinter420FieldCurr
               (0 << 17)+ //VWMVinter420FieldCurrTmp
               (0 << 18)+ //vwmv_420field_postrc1
               (rWMVDecInfo.rSMProfile.fgFrmHybridMVOn << 19) + //vwmv_8hybrid_mv_on
               (rWMVDecInfo.rPicLayer.fgMvResolution << 20) + //vwmv_8mvresolution
               (use_interpredictor << 21)
               + (rWMVDecInfo.rPicLayer.fgInterlaceV2 << 23);
    }
    else // >= WMV3
    {
        a206 = 0 + //wmv7
               (0 << 1)+ //wmv8
               ((rWMVDecInfo.rOther.i4CodecVersion == WMV3)<<2)+ //wmv9
               ((rWMVDecInfo.rOther.i4CodecVersion == WMVA)<<3)+ //wmva
               ((rWMVDecInfo.rEntryPoint.i4MVRangeIndex&0xf)<<4)+//mv_range
               (rWMVDecInfo.rPicLayer.fgInterlaceV2 << 8)+//interlace_420
               (0 << 9) + //interlace_411
               (((rWMVDecInfo.rOther.i4X9MVMode == ALL_1MV_HALFPEL)
                 || (rWMVDecInfo.rOther.i4X9MVMode
                     == ALL_1MV_HALFPEL_BILINEAR)) << 10) +//VWMVHalfFilterSel
               ((rWMVDecInfo.rEntryPoint.fgUVHpelBilinear == 1) << 11)+ //VWMVrounding
               //      (rWMVDecInfo.rPicLayer.i4CurrentTemporalField << 12) + //VWMVHalfPelBackRef
               (rWMVDecInfo.rPicLayer.fgBackRefUsedHalfPel << 12) + //VWMVHalfPelBackRef
               (rWMVDecInfo.rSeqLayer.fgVC1 << 13) + //VWMVPostCrescent
               (rWMVDecInfo.rPicLayer.fgUseOppFieldForRef << 14) + //VWMVUseOppieldRef
               ((rWMVDecInfo.rPicLayer.fgTwoRefPictures
                 && (rWMVDecInfo.rPicLayer.u1FrameCodingMode
                     == INTERLACEFIELD)) << 15) + //VWMVinter420Field2ref
               (((rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
                 &&(rWMVDecInfo.rPicLayer.i4CurrentField == 1)) << 16)+ //VWMVinter420FieldCurr
               ((rWMVDecInfo.rPicLayer.i4CurrentTemporalField) << 17)+ //VWMVinter420FieldCurrTmp
               ((rWMVDecInfo.rPicLayer.fgPostRC1) <<18)
               + ((rWMVDecInfo.rPicLayer.fgInterlaceV2) << 23)
               + ((rWMVDecInfo.rOther.i4CodecVersion == WMVA
                   && rWMVDecInfo.rPicLayer.u1PicType == BVOP ) << 24)
               + (((rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
                   || (rWMVDecInfo.rPicLayer.u1FrameCodingMode
                       == INTERLACEFRAME)) <<25);
        //          ((rWMVDecInfo.rPicLayer.u1FrameCodingMode == PROGRESSIVE && (rWMVDecInfo.rPicLayer.u1PicType == BVOP)) <<26);
    }
    WriteREG(RW_VLD_MVVOP_SEL, a206);

    //-----------------------------------------------------
    // VLD_reg_194 : set top_mv1 or bot_mv1
    //-----------------------------------------------------
    {
        //ilii
        int a194;
        if ((rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
                &&(rWMVDecInfo.rPicLayer.i4CurrentField == 1)
                &&(rWMVDecInfo.rOther.i4CodecVersion >= WMV3)) //bot
        {
            a194 = (((UINT32)rWMVDecInfo.rDram.pu1Mv_1_2 >> 2) & 0x3FFFFF )+ //dram_addr_base
                   ((rWMVDecInfo.rSeqHdrR.u4NumMBX * 4) << 22); //Mv_1_2
            WriteREG(VLD_REG_202, (((UINT32)rWMVDecInfo.rDram.pu1Mv_1_2 >> 24)
                                   & 0x3F ));
        }
        else
        {
            a194 = (((UINT32)rWMVDecInfo.rDram.pu1Mv_1 >> 2) & 0x3FFFFF )+ //dram_addr_base
                   ((rWMVDecInfo.rSeqHdrR.u4NumMBX * 4) << 22); //Mv_1_2
            WriteREG(VLD_REG_202, (((UINT32)rWMVDecInfo.rDram.pu1Mv_1 >> 24)
                                   & 0x3F ));
        }
        WriteREG(RW_VLD_MVSA, a194); //Robert: Temporary Marked! Interlace Field Needs it!!!!
    }

    //-----------------------------------------------------
    // VLD_reg_195 : TRB & TRD
    //-----------------------------------------------------
    TRB=rWMVDecInfo.rPicLayer.i4BNumerator
        * rWMVDecInfo.rPicLayer.i4BFrameReciprocal;
    TRD=rWMVDecInfo.rPicLayer.i4BNumerator
        * rWMVDecInfo.rPicLayer.i4BFrameReciprocal - 256;
    if (rWMVDecInfo.rOther.i4CodecVersion >= WMV3)
    {
        a195 = (TRB << 0) + (TRD << 16);
        WriteREG(RW_VLD_DIRE_MD, a195);
    }
    else
    {
        int wmv78_slice_num;
        if (rWMVDecInfo.rOther.i4CodecVersion == WMV2)
            wmv78_slice_num = rWMVDecInfo.rSeqHdrR.u4NumMBY
                              / rWMVDecInfo.rSMProfile.i4SliceCode;
        else
            wmv78_slice_num = (rWMVDecInfo.rSeqHdrR.u4NumMBY
                               / (rWMVDecInfo.rSMProfile.i4SliceCode - 22));

        int a199;
        if (rWMVDecInfo.rSeqHdrR.u4NumMBY < 33)
        {
            a195 = 0;
            for (i=1; i<rWMVDecInfo.rSeqHdrR.u4NumMBY; i++)
                a195 += ((i%wmv78_slice_num) == 0) << (i-1);
            a199 = 0;
        }
        else
        {
            a195 = 0;
            a199 = 0;
            for (i=1; i<33; i++)
                a195 += ((i%wmv78_slice_num) == 0) << (i-1);

            for (i=33; i<rWMVDecInfo.rSeqHdrR.u4NumMBY; i++)
                a199 += ((i%wmv78_slice_num) == 0) << (i-33);
        }

        WriteREG(RW_VLD_DIRE_MD, a195);
        WriteREG(RW_VLD_DIRE_MD_IL, a199);
    }

    //-----------------------------------------------------
    // VLD_reg_207 : interlace 420
    //-----------------------------------------------------
    if (rWMVDecInfo.rOther.i4CodecVersion >= WMV3)
    {
        int use_half = use_quarter_pel_mv ? 1 : 2;
        int a207;
        a207 = (use_half*rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarMVX & 0x7f)
               + (((use_half*rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarMVY)
                   & 0x1f) << 8)
               + (((use_half*rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarMVX)
                   & 0x7f) << 16)
               + (((use_half*rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarMVY)
                   & 0x1f) << 24);
        WriteREG(RW_VLD_MVF1, a207);
    }

    //-----------------------------------------------------
    // VLD_reg_208 : interlace 420
    //-----------------------------------------------------
    if (rWMVDecInfo.rOther.i4CodecVersion >= WMV3)
    {
        int use_half = 1;//use_quarter_pel_mv ? 1 : 2;
        int a208;
        a208 = ((use_half*rWMVDecInfo.rPicLayer.i4FarFieldScale1) & 0x3fff)
               + (((use_half*rWMVDecInfo.rPicLayer.i4FarFieldScale2) & 0x1ff)
                  << 14) + (((use_half
                              *rWMVDecInfo.rPicLayer.i4NearFieldScale) & 0x1ff) << 23);
        WriteREG(RW_VLD_MVF2, a208);
    }

    //-----------------------------------------------------
    // VLD_reg_209 : interlace 420
    //-----------------------------------------------------
    if (rWMVDecInfo.rOther.i4CodecVersion >= WMV3)
    {
        int use_half = use_quarter_pel_mv ? 1 : 2;
        int a209;
        a209 = ((use_half*rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarBackMVX)
                & 0x7f)
               + (((use_half*rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarBackMVY)
                   & 0x1f) << 8) + (((use_half
                                      *rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarBackMVX) & 0x7f)
                                    << 16) + (((use_half
                                                *rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarBackMVY) & 0x1f)
                                              << 24);
        WriteREG(RW_VLD_MVF3, a209);
    }

    //-----------------------------------------------------
    // VLD_reg_210 : interlace 420
    //-----------------------------------------------------
    if (rWMVDecInfo.rOther.i4CodecVersion >= WMV3)
    {
        int use_half = 1;//use_quarter_pel_mv ? 1 : 2;
        int a210;
        a210 = ((use_half*rWMVDecInfo.rPicLayer.i4FarFieldScaleBack1) & 0x3fff)
               + (((use_half*rWMVDecInfo.rPicLayer.i4FarFieldScaleBack2)
                   & 0x1ff) << 14) + (((use_half
                                        *rWMVDecInfo.rPicLayer.i4NearFieldScaleBack) & 0x1ff) << 23);
        WriteREG(RW_VLD_MVF4, a210);
    }

    //-----------------------------------------------------
    // VLD_reg_212 : interlace 420
    //-----------------------------------------------------
    {
        int a212;
        a212 = (vop_type == 0) + //VopCodingTypeI
               ((vop_type == 1) << 1)+ //VopCodingTypeP
               ((vop_type == 2) << 2)+ //VopCodingTypeB
               (0 << 3)+ //VopCodingTypeS (mp4)
               (0 << 4)+ //MPEG1or2
               (0 << 5)+ // Divx311
               (0 << 6)+ //MP4
               (0 << 7)+ //ShortVideo
               (1 << 8)+ //WMV789A
               ((rWMVDecInfo.rPicLayer.u1FrameCodingMode != INTERLACEFIELD)
                << 9); //WMVFramePic
        WriteREG(RW_VLD_VOP_TYPE, a212);
    }
    //----------------------------------------------
    // VLD_reg_116 : load sum_out_wmv(or mp4) fpga
    //----------------------------------------------
    // No need for emuSoft
    //        WriteREG(RW_VLD_116, 0x00000001);
    //        WriteREG(RW_VLD_116, 0x00000000);

    //------------------------------
    //   VLD RISC WRITE PATTERN
    //------------------------------
    //initialize before load r_ptr and shift_bit
    // HMLin suggests to mark.
    //        WriteREG(RW_VLD_WMV_MODE, 0x00000000);

    //-------------------------------------------
    // VLD_reg_44 : r_ptr (only for simulation)
    //-------------------------------------------

    //-------------------------------------------
    // VLD_reg_45 : v_start address in dram
    //-------------------------------------------

    //-------------------------------------------
    // VLD_reg_46 : v_end address in dram
    //-------------------------------------------

    //-------------------------------------------
    // VLD_reg_35 : load bitstream from dram
    //-------------------------------------------

    //-------------------------------------------
    // read VLD_reg to shift the barrel shifter
    //-------------------------------------------

    //-------------------------------------------
    // VLD_reg_34~39,42 : VLD parameter setting
    //-------------------------------------------
    if (rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
    {
        if (rWMVDecInfo.rPicLayer.i4CurrentField == 0)
            tmp = 1; //top
        else
            tmp = 2; //bot
    }
    else
        tmp = 3; //frame


    a34 = (tmp <<30) + //picture_structure << 30) +
          (0<<29) + //frame_pred_frame_dct << 29) +
          (0<<28) + //concealment_motion_vectors << 28) +
          (0<<27) + //q_scale_type << 27) +
          // (top_field_first << 26) +
          // (full_pel_forward_vector << 25) +
          // (full_pel_backward_vector << 24) +
          // ((mp4_state->hdr.prediction_type) << 21) + //picture_coding_type << 21) +
          (vop_type << 21) + //picture_coding_type << 21) +
          (0<<20)+ //intra_vlc_format << 20) +
          // (intra_dc_precision << 18) +
          // (alternate_scan << 17) +
          // (reset_of_mat << 16) +
          // ((mp4_state->hdr.fcode_for) << 12)+ //f_code[0][0] << 12) +
          // ((pWMVDec->uiFCode) << 12)+ //f_code[0][0] << 12) +
          // it seems that the f_code not exists in WMV
          // ((mp4_state->hdr.fcode_back) << 8); //(f_code[0][1] << 8) +
          // ((mp4_state->hdr.fcode_back)<<4); //f_code[1][0] << 4) +
          // it seems that the f_code not exists in WMV
          (0); //f_code[1][1]);
    WriteREG(RW_VLD_PARA, a34);
    //rWMVDecInfo.rOther.u4Datain = ReadREG(RW_VLD_PARA);

    //  height = (rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFIELD)? rWMVDecInfo.rSeqHdrR.u4PicHeightSrc/2 : rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
    height
    = (rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFIELD) ? rWMVDecInfo.rSeqHdrR.u4PicHeightSrc
      /2
      : rWMVDecInfo.rSeqHdrR.u4PicHeightDec/*Main Profile only*/;
    a36 = (0<<31)+ //MPEG1_flag
          ((height + 15 ) << 16)+ //vertical_size
          (63 << 8)+ //max_mbl_mod/** b_buffer_size_s1 **/ << 8)+
          //          ((rWMVDecInfo.rSeqHdrR.u4PicWidthSrc + 15)>>4); //picture_width_in_MB);
          (rWMVDecInfo.rSeqHdrR.u4NumMBX);
    WriteREG(RW_VLD_PICSZ, a36);

    if ((rWMVDecInfo.rOther.i4CodecVersion == WMV1)
            || (rWMVDecInfo.rOther.i4CodecVersion == WMV2))
    {
        a37 = (0<<24)+ //part_dec_sta_y /* mc_start_mb_row*/ << 24) +
              (63<<16)+ //part_dec_end_y /* mc_end_mb_row*/  << 16) +
              (0<<8); //b_buf_start  /*b_buffer_start_row*/ << 8);
    }
    else
    {
        a37 = (0<<24)+ //part_dec_sta_y /* mc_start_mb_row*/ << 24) +
              (127<<16)+ //part_dec_end_y /* mc_end_mb_row*/  << 16) +
              (0<<8); //b_buf_start  /*b_buffer_start_row*/ << 8);
    }
    WriteREG(RW_VLD_MBROWPRM, a37);

    a38 = (0 /* digest_mb_x */<< 16) + (0 /* digest_mb_y */);
    WriteREG(RW_VLD_DIGMBSA, a38);

    a39 = (0<<24) + //H_scale /*h_scale*/ << 24) +
          (0<<16) + //V_scale /*v_scale*/  << 16) +
          (0<<8); //write_to_digest_buf /*w_to_dig_buf*/ << 8);
    WriteREG(RW_VLD_SCALE, a39);

    a42 = (0 /* digest_MB_y_offset1 */<< 16)
          + (0 /* digest_MB_y_offset1 */<< 8) + (0
                  /* digest_MB_y_offset1 */<< 0);
    WriteREG(RW_VLD_DIGMBYOFF, a42);

    a35 = (1 << 27) + //dec_b_pic_all
          (0 << 8) + //mc_start_mb_addr
          (rWMVDecInfo.rSeqHdrR.u4NumMBX); //mc_end_mb_addr
    WriteREG(RW_VLD_PROC, a35);

    //-------------------------------------------
    // VLD_reg_135 : load_sum_risc (for fpga)
    //-------------------------------------------
    ReadREG(VLD_REG_OFST);
    WriteREG(RW_VLD_WMV_LOAD_SUM, 0x00000001);
    WriteREG(RW_VLD_WMV_LOAD_SUM, 0x00000000);

    //-------------------------------------------
    // VLD_reg_131 : wmv version setting
    //-------------------------------------------
    if ((rWMVDecInfo.rOther.i4CodecVersion == WMV1)
            || (rWMVDecInfo.rOther.i4CodecVersion == WMV2))
    {
        a131 = (rWMVDecInfo.rOther.i4CodecVersion == WMV3) + //wmv9
               ((rWMVDecInfo.rOther.i4CodecVersion == WMV2)<<1)+ //wmv8
               ((rWMVDecInfo.rOther.i4CodecVersion == WMV1)<<2)+ //wmv7
               (1 << 8) + // profile : [main]
               (0 << 9) + // profile : [advance]
               (rWMVDecInfo.rSMProfile.fgYUV411 << 16) + //interlace_411 flag
               (0 << 17)+ //interlace_420 flag
               (1 << 24) + // frame_picture
               (0 << 25); //bottom_field
    }
    else
    {
        a131 = (((rWMVDecInfo.rSeqLayer.i4WMV3Profile & 0x2) == 0) << 0) + // wmv9_flag (this code only can decode the WMV9 bitstream)
               ((rWMVDecInfo.rSeqLayer.i4WMV3Profile & 0x2) << 8) + // profile : [advanced //main]
               (rWMVDecInfo.rSMProfile.fgYUV411 << 16) + //interlace_411 flag
               (rWMVDecInfo.rPicLayer.fgInterlaceV2 << 17)+ //interlace_420 flag
               ((rWMVDecInfo.rPicLayer.u1FrameCodingMode != INTERLACEFIELD)
                << 24) + // frame_picture
               ((tmp == 2) << 25) + //top field:0  bottom_field:1
               (((rWMVDecInfo.rPicLayer.i4CurrentTemporalField == 1)
                 &&(rWMVDecInfo.rPicLayer.u1FrameCodingMode
                    == INTERLACEFIELD)) << 26); //1st field:0, 2nd field:1
    }
    WriteREG(RW_VLD_WMV_MODE, a131);

    //-------------------------------------------------
    // VLD_reg_132 : Quantizer type , half_step, slice
    //-------------------------------------------------
    a132 = ((rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer) << 0)
           + ((rWMVDecInfo.rPicLayer.fgHalfStep) << 8) //+
           //((_fgSliceWMVA) << 16)
           //((rWMVDecInfo.rOther.i4FrameXformMode) << 16) //+
           //// { 0: XFORMMODE_8x8,
           ////   1: XFORMMODE_8x4,
           ////   2: XFORMMODE_4x8,
           ////   3: XFORMMODE_MBSWITCH};
           //((pmbmd->m_chMBMode) << 20)
           + (rWMVDecInfo.rOther.u4BPRawFlag << 16);
    WriteREG(RW_VLD_WMV_PARA1, a132);

    //-------------------------------------------------
    // VLD_reg_133 : TTMB, QP_value, ...
    //-------------------------------------------------
    if ((rWMVDecInfo.rOther.i4CodecVersion == WMV1)
            || (rWMVDecInfo.rOther.i4CodecVersion == WMV2))
    {
        high_rate_switch = (rWMVDecInfo.rPicLayer.i4PicQtIdx <= MAXHIGHRATEQP)
                           && (rWMVDecInfo.rOther.i4CodecVersion == WMV3);
        ttfrm = (rWMVDecInfo.rOther.fgMBXformSwitching == 1) ? 0
                : (rWMVDecInfo.rOther.i4FrameXformMode == 4) ? 3
                : (rWMVDecInfo.rOther.i4FrameXformMode & 0x3);

        a133 = (rWMVDecInfo.rOther.i4CBPTable << 0)
               + ((rWMVDecInfo.rOther.fgDCPred_IMBInPFrame) << 3)
               + ((rWMVDecInfo.rOther.fgMBXformSwitching == 1) << 4) + //TTMBF
               ((ttfrm) << 5) + ((rWMVDecInfo.rPicLayer.fgMvResolution) << 7 )
               + ((rWMVDecInfo.rPicLayer.fgIntraDCTDCTable) << 8)
               + ((rWMVDecInfo.rPicLayer.fgDCTTable_MB) << 9)
               + ((high_rate_switch) << 10)
               + ((rWMVDecInfo.rSMProfile.fgRotatedIdct == 0) << 11)
               + ((rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx) << 12)
               + ((rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx) << 14) + //ming added
               ((rWMVDecInfo.rPicLayer.i4DCStepSize) << 16)
               + ((rWMVDecInfo.rPicLayer.i4StepSize) << 24);
    }
    else
    {
        high_rate_switch = rWMVDecInfo.rPicLayer.i4PicQtIdx <= MAXHIGHRATEQP;// && rWMVDecInfo.rOther.i4CodecVersion == WMV3;
        ttfrm = (rWMVDecInfo.rOther.fgMBXformSwitching == 1) ? 0
                : (rWMVDecInfo.rOther.i4FrameXformMode == 4) ? 3
                : (rWMVDecInfo.rOther.i4FrameXformMode & 0x3);

        a133 = (rWMVDecInfo.rOther.i4CBPTable << 0)
               + ((rWMVDecInfo.rOther.fgMBXformSwitching == 0) << 4) + //TTMBF
               ((ttfrm) << 5) + ((rWMVDecInfo.rPicLayer.fgIntraDCTDCTable)
                                 << 8) + ((rWMVDecInfo.rPicLayer.fgDCTTable_MB) << 9)
               + ((high_rate_switch) << 10)
               + ((rWMVDecInfo.rSMProfile.fgRotatedIdct == 0) << 11)
               + ((rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx) << 12)
               + ((rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx) << 14) + //ming added
               ((rWMVDecInfo.rPicLayer.i4DCStepSize) << 16)
               + ((rWMVDecInfo.rPicLayer.i4StepSize) << 24);
    }
    //Printf("a133 : %08x\n",a133);
    //Printf("rWMVDecInfo.rPicLayer.i4DCStepSize :%d\n",rWMVDecInfo.rPicLayer.i4DCStepSize);
    //Printf("rWMVDecInfo.rPicLayer.i4StepSize :%d\n",rWMVDecInfo.rPicLayer.i4StepSize);

    WriteREG(RW_VLD_WMV_PARA2, a133);

    //-------------------------------------------------
    // VLD_reg_136 : MV parameter setting for VLD
    //-------------------------------------------------
    b_fraction_chk = ((rWMVDecInfo.rPicLayer.i4BNumerator
                       * rWMVDecInfo.rPicLayer.i4BFrameReciprocal) >> 7) ? 1 : 0;

    if ((rWMVDecInfo.rOther.i4CodecVersion == WMV1)
            || (rWMVDecInfo.rOther.i4CodecVersion == WMV2))
    {
        a136 = ((rWMVDecInfo.rEntryPoint.i4MVRangeIndex) << 0)
               + ((rWMVDecInfo.rOther.i4X9MVMode) << 8) + ((b_fraction_chk)
                       << 16) + (0 << 17) + (0 << 18) + (0 << 19) + (0 << 24); //((lookup_mv_tbl_number)     << 24);
    }
    else
    {
        a136 = ((rWMVDecInfo.rEntryPoint.i4MVRangeIndex) << 0)
               + ((rWMVDecInfo.rOther.i4X9MVMode) << 8) + ((b_fraction_chk)
                       << 16) + ((rWMVDecInfo.rPicLayer.fgTwoRefPictures
                                  && (rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFIELD))
                                 << 17) + (rWMVDecInfo.rEntryPoint.i4ExtendedDMVX << 18)
               + (rWMVDecInfo.rEntryPoint.i4ExtendedDMVY << 19)
               + ((rWMVDecInfo.rOther.i4MvTable) << 24);
    }
    WriteREG(RW_VLD_WMV_PARA3, a136);

    if ((rWMVDecInfo.rOther.i4CodecVersion == WMV1)
            || (rWMVDecInfo.rOther.i4CodecVersion == WMV2))
    {
        a137 = (rWMVDecInfo.rSMProfile.i4skip_bit_mode_v87 << 8);
        WriteREG(RW_VLD_WMV_BP, a137);
    }

    //-------------------------------------------------
    // VLD_reg_148 : VLD mode setting
    //-------------------------------------------------
    a138 = 0 + (0 << 25);

    WriteREG(VLD_REG_OFST + 0x0228, a138);
    //-------------------------------------------------
    // VLD_reg_140 : VOPQUANT setting (MBQUANT)
    //-------------------------------------------------
    a140 = ((rWMVDecInfo.rOther.i4Panning & 0xf) << 0)
           + (rWMVDecInfo.rOther.fgDQuantOn << 8)
           + ((rWMVDecInfo.rOther.u1DiffQtProfile == 3) << 9)
           + ((rWMVDecInfo.rOther.fgDQuantBiLevel & 0x1) <<10)
           + ((rWMVDecInfo.rOther.u1DQuantBiLevelStepSize & 0x1f) <<16);
    //Printf("rWMVDecInfo.rOther.u1DQuantBiLevelStepSize : %d\n",(rWMVDecInfo.rOther.u1DQuantBiLevelStepSize & 0x1f));
    WriteREG(RW_VLD_WMV_QM, a140);

    //-------------------------------------------------
    // VLD_reg_142 : MV table index for VLD
    //-------------------------------------------------
    a142 = ((rWMVDecInfo.rOther.i4X9MVMode == MIXED_MV)
            &&((rWMVDecInfo.rPicLayer.u1PicType == PVOP)
               ||(rWMVDecInfo.rPicLayer.u1PicType == BVOP)))
           + (((rWMVDecInfo.rOther.i4CodecVersion <= WMV2)
               && (rWMVDecInfo.rOther.i4MvTable)) << 1)
           + (((rWMVDecInfo.rPicLayer.i4Overlap & 4) != 0) << 8)
           + (rWMVDecInfo.rOther.i4MBModeTable << 16)
           + (rWMVDecInfo.rOther.i42MVBPTable << 24)
           + (rWMVDecInfo.rOther.i44MVBPTable << 28);
    WriteREG(RW_VLD_WMV_TAB, a142);

    //-------------------------------------------------
    // VLD_reg_146 : parse slice picture header (unit:bit)
    //-------------------------------------------------
    if (rWMVDecInfo.rOther.i4CodecVersion == WMVA)
    {
        WriteREG(RW_VLD_WMV_PBN, rWMVDecInfo.rPicLayer.i4SlicePicHeaderNum);
    }
    //spork
    //WriteREG(RW_VLD_SRAM_TEST_WRITE_DATA, 0x101); //ming add for debug 12/15

    // WriteREG(0x4f8, 24); //ming add for debug 12/15

    //  WriteREG(0x04dc, 500);
    //-------------------------------------------------
    // RW_VLD_WMV_LOAD_SUM : load sum_risc (only for simulation)
    //-------------------------------------------------

    //-------------------------------------------------
    // VLD_reg_130 : hw_decode_start flag
    //-------------------------------------------------

    //--------------------------------------------
    // Dump Dram data for comparation with golden
    //--------------------------------------------
    //dump_size = (((rWMVDecInfo.rSeqHdrR.u4PicWidthSrc+15)/16) * 2) * (((rWMVDecInfo.rSeqHdrR.u4PicHeightSrc+31)/32)* 32);

    //_fgPrintWReg = FALSE; //Stop printing WriteREG();

    //test, //ilii, HM comment, 0 -> 1
#if defined(CC_MT5365) || defined(CC_MT5395)
    WriteREG(MC_REG_OFST + (453*4), 0x00010800);
#else
    WriteREG(MC_REG_OFST + (453*4), 0x1);
#endif
}

BOOL fgHWDecBP(UINT32 dwBpNum)
{

    //int i, a = 0;
    int vop_type;
    UINT32 u4Tmp;
    //UINT32 dwInputWindow2;

    //  int height = (rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFIELD)? rWMVDecInfo.rSeqHdrR.u4PicHeightSrc/2 : rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
    int height =
        (rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFIELD) ? rWMVDecInfo.rSeqHdrR.u4PicHeightSrc
        /2
        : rWMVDecInfo.rSeqHdrR.u4PicHeightDec/*Main Profile only*/;
    int a36 = (0<<31)+ //MPEG1_flag
              (((height) + 15 ) << 16)+ //vertical_size
              (63 << 8)+ //max_mbl_mod/** b_buffer_size_s1 **/ << 8)+
              //        ((rWMVDecInfo.rSeqHdrR.u4PicWidthSrc + 15)>>4); //picture_width_in_MB);
              (rWMVDecInfo.rSeqHdrR.u4NumMBX);
    int a37 = (0<<24)+ //part_dec_sta_y /* mc_start_mb_row*/ << 24) +
              (127<<16)+//(63<<16)+ //part_dec_end_y /* mc_end_mb_row*/  << 16) +
              (0<<8); //b_buf_start  /*b_buffer_start_row*/ << 8);
    int a35 = (1 << 27) + //dec_b_pic_all
              (0 << 8) + //mc_start_mb_addr
              (rWMVDecInfo.rSeqHdrR.u4NumMBX); //mc_end_mb_addr

    int a137;

    unsigned long dwTmp = (dwGetBitStream(0) >> 27) & 0xF;

    if (dwTmp == 0)
    {
        switch (dwBpNum)
        {
        case 0: //SKIP
            rWMVDecInfo.rOther.u4BPRawFlag |= 0x1;
            break;
        case 1: //4MV
            rWMVDecInfo.rOther.u4BPRawFlag |= 0x2;
            break;
        case 3: //DIRECT
            rWMVDecInfo.rOther.u4BPRawFlag |= 0x4;
            break;
        case 4: //FIELD
            rWMVDecInfo.rOther.u4BPRawFlag |= 0x8;
            break;
        case 5: //ACPRED
            rWMVDecInfo.rOther.u4BPRawFlag |= 0x10;
            break;
        case 6: //OVERLAP
            rWMVDecInfo.rOther.u4BPRawFlag |= 0x20;
            break;
        case 7: //FORWARD
            rWMVDecInfo.rOther.u4BPRawFlag |= 0x40;
            break;
        default:
            rWMVDecInfo.rOther.u4BPRawFlag = 0;
            break;
        }
    }

    if (rWMVDecInfo.rOther.i4CodecVersion == WMV2)
    {
        a137 = (dwBpNum)+ (1<<16)+ //wmv8
               (rWMVDecInfo.rSMProfile.i4wmv8_bp_mode<<17);
    }
    else
    {
        a137 = dwBpNum;
    }

    before_bp();

    vop_type = (rWMVDecInfo.rPicLayer.u1PicType == PVOP) ? 1
               : (rWMVDecInfo.rPicLayer.u1PicType == BVOP) ? 2 : 0;

    //  WriteREG(RW_VLD_WMV_ABS, (rWMVDecInfo.rOther.i4CodecVersion == WMVA)); // Step 2 : Handle 0x00 0x00 0x03 0x0?
    WriteREG(RW_VLD_PARA, 0x00000000);
    WriteREG(RW_VLD_PROC, a35);
    WriteREG(RW_VLD_PICSZ, a36); // Step 1 : Set Picture Size
    WriteREG(RW_VLD_MBROWPRM, a37);
    WriteREG(RW_VLD_DIGMBSA, 0x00000000);
    WriteREG(RW_VLD_SCALE, 0x00000000);
    WriteREG(RW_VLD_DIGMBYOFF, 0x00000000);

    //***********************************************************************
    //-----------------------------------------------------
    // VLD_reg_212 : interlace 420
    //-----------------------------------------------------
    {
        int a212;
        a212 = (vop_type == 0) + //VopCodingTypeI
               ((vop_type == 1) << 1)+ //VopCodingTypeP
               ((vop_type == 2) << 2)+ //VopCodingTypeB
               (0 << 3)+ //VopCodingTypeS (mp4)
               (0 << 4)+ //MPEG1or2
               (0 << 5)+ // Divx311
               (0 << 6)+ //MP4
               (0 << 7)+ //ShortVideo
               (1 << 8)+ //WMV789A
               ((rWMVDecInfo.rPicLayer.u1FrameCodingMode != INTERLACEFIELD)
                << 9); //WMVFramePic
        WriteREG(RW_VLD_VOP_TYPE, a212);
    }

    //bp mode
    WriteREG(RW_VLD_WMV_BP, a137/*dwBpNum*/); // Step 3 : bit-plane num

    //load sum_risc
    ReadREG(VLD_REG_OFST);
    WriteREG(RW_VLD_WMV_LOAD_SUM, 0x1/*0x00000101*/); // Step 4 : load mpeg2_sum to wmv_sum
    WriteREG(RW_VLD_WMV_LOAD_SUM, 0x00000000); // Step 4

    //change mpeg2_mode to wmv_mode
    if (rWMVDecInfo.rOther.i4CodecVersion == WMV1)
        WriteREG(RW_VLD_WMV_MODE, 0x00000004); // Step 5 : change to WMV mode
    else if (rWMVDecInfo.rOther.i4CodecVersion == WMV2)
        WriteREG(RW_VLD_WMV_MODE, 0x00000002); // Step 5 : change to WMV mode
    else
        WriteREG(RW_VLD_WMV_MODE, 0x00000001); // Step 5 : change to WMV mode

    //start bp_decode
    //dwInputWindow2 = dwGetBitStream(0);
    rWMVDecInfo.rOther.u4VldPosByte = dwVldRPtr(&rWMVDecInfo.rOther.u4VldPosBit);

    //Printf("Rptr 0x%x, input win 0x%x, 60 = 0x%x, 56 = 0x%x\n", ReadREG(RO_VLD_VRPTR), ReadREG(RO_VLD_BARL), ReadREG(RO_VLD_INPUTWND), ReadREG(RO_VLD_FSBI));

    //test, //ilii, HM comment, 0 -> 1
#if defined(CC_MT5365) || defined(CC_MT5395)
    WriteREG(MC_REG_OFST + (453*4), 0x00010800);
#else
    WriteREG(MC_REG_OFST + (453*4), 0x1);
#endif

#ifdef CC_MT5395
    HalFlushDCacheMultipleLine(rWMVDecInfo.rPesInfo.u4FifoStart, rWMVDecInfo.rPesInfo.u4FifoEnd - rWMVDecInfo.rPesInfo.u4FifoStart);
#else
    HalFlushInvalidateDCache();
#endif

#if 0 //defined(CC_MT5365)
    if (rWMVDecInfo.rOther.fgNewMode/* && rWMVDecInfo.rOther.i4CodecVersion != WMV3*/)
    {
        UINT32 a163, a164, a170, a171;
        INT32 i4OwBp;

        // VLD part
        // 163
        a163 = (WMV_NEW_FLG_EN | WMV_PRED_MOD_EN | WMV_PRED_RD_SYNC_DISABLE
                | WMV_DRAM_BURST_MODE_EN  // ???
                | WMV_BP_USE_PRED_RD_EN); // ???
        WriteREG(RW_VLD_WMV_NEW_CTRL, a163);

        // 164
        a164 = PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Dcac);  // ???
        a164 |= (1<<30);
        // how about bit 31???
        WriteREG(RW_VLD_WMV_DCAC_NEW_ADDR, a164);

        // 167, 168, 169
        WriteREG(RW_VLD_WMV_BP0_NEW_ADDR, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Bp_1));  // ???
        WriteREG(RW_VLD_WMV_BP1_NEW_ADDR, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Bp_2));  // ???
        WriteREG(RW_VLD_WMV_BP2_NEW_ADDR, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Bp_3));  // ???

        // 170
        a170 = (rWMVDecInfo.rSeqHdrR.u4PicWidthDec / 16)  * 7  * (16/16);
        WriteREG(RW_VLD_WMV_OW_PRED, a170);

        // 171
        i4OwBp = (rWMVDecInfo.rSeqHdrR.u4PicHeightDec / 16)  * (16/16);
        a171 = ( (i4OwBp << WMV_NUM_OW_BP_0) | (i4OwBp << WMV_NUM_OW_BP_1) | (i4OwBp << WMV_NUM_OW_BP_2));
        WriteREG(RW_VLD_WMV_OW_BP, a171);
    }
#endif

    WriteREG(RW_VLD_WMVDEC, 0x00000100); // Step 6 : start bit-plane decoding
    WriteREG(RW_VLD_WMVDEC, 0x00000000); // Step 6

    while (1)
    {
        //UINT32 static u4Timer = 0;
        //x_thread_delay(1);
        if ((ReadREG(RO_VLD_BP_DEC_END) & 0x100) == 0x100) // Step 7 : Wait for bit-plane decode finish
        {
            break;
        }
    }
#ifdef WAIT_SRAM_STABLE
    u4Tmp = 0;
    while ((ReadREG(RO_VLD_SRAMCTRL) & AA_FIT_TARGET_D)==0)
    {
        u4Tmp++;
        if (u4Tmp >= 0x10000)
        {
            ASSERT(0);
        }
    }
#endif
    //  rWMVDecInfo.rOther.u4BPRawFlag = (ReadREG(RW_VLD_WMV_PARA1) >>16) & 0x7F; //Save BP RAW Mode Flag.


    // VLD_reg_116 : load sum_out_wmv(or mp4)
    WriteREG(RW_VLD_116, 0x00000001); // Step 8 : load wmv_sum to mpeg2_sum
    WriteREG(RW_VLD_116, 0x00000000); // Step 8

    //change wmv_mode to mpeg2_mode
    WriteREG(RW_VLD_WMV_MODE, 0x00000000); // Step 9 : change to mpeg2 mode

    //  WriteREG(RW_VLD_WMV_ABS, 0x00000000);
    after_bp();
    rWMVDecInfo.rOther.u4VldPosByte
    = dwVldRPtr(&rWMVDecInfo.rOther.u4VldPosBit);
    //dwInputWindow2 = dwGetBitStream(0);
    return TRUE;
}


#ifdef CC_WMV_EMULATION
void vDumpWmvMcReg(void)
{
#if 0
    char ucPicType = ' ';

    switch (rWMVDecInfo.rPicLayer.u1PicType)
    {
    case IVOP:
        ucPicType = 'I';
        break;
    case PVOP:
        ucPicType = 'P';
        break;
    case BVOP:
        ucPicType = 'B';
        break;
    case BIVOP:
        ucPicType = 'A';
        break;
    case SKIPFRAME:
        ucPicType = 'S';
        break;
    }
    printf("Frame #%d: %c\n", rWMVDecInfo.rOther.u4CurrPicNo - 1, ucPicType);
#else
    printf("MC 401: 0x%08X\n", ReadREG(MC_REG_OFST + 401 * 4));
    printf("MC 476: 0x%08X\n", ReadREG(MC_REG_OFST + 476 * 4));
    printf("MC 477: 0x%08X\n", ReadREG(MC_REG_OFST + 477 * 4));
    printf("MC 478: 0x%08X\n", ReadREG(MC_REG_OFST + 478 * 4));
    printf("MC 522: 0x%08X\n", ReadREG(MC_REG_OFST + 522 * 4));
#endif
}

void vDumpWmvReg(void)
{
    UINT32 i;

    for (i = 0x84; i < 0x410; i+=4)
    {
        printf("VLD %3d: 0x%08X\n", i/4, ReadREG(VLD_REG_OFST + i));
    }

    for (i = 0; i < 0x770; i+=4)
    {
        printf("MC %3d: 0x%08X\n", i/4, ReadREG(MC_REG_OFST + i));
    }

    if (rWMVDecInfo.rOther.fgNewMode)
    {
        printf("MV %3d: 0x%08X\n", 131, ReadREG(WMV_MV_REG_OFST + 131 * 4));
        printf("MV %3d: 0x%08X\n", 134, ReadREG(WMV_MV_REG_OFST + 134 * 4));
        printf("MV %3d: 0x%08X\n", 135, ReadREG(WMV_MV_REG_OFST + 135 * 4));
        printf("MV %3d: 0x%08X\n", 137, ReadREG(WMV_MV_REG_OFST + 137 * 4));
        printf("MV %3d: 0x%08X\n", 138, ReadREG(WMV_MV_REG_OFST + 138 * 4));
        printf("MV %3d: 0x%08X\n", 139, ReadREG(WMV_MV_REG_OFST + 139 * 4));
        printf("MV %3d: 0x%08X\n", 147, ReadREG(WMV_MV_REG_OFST + 147 * 4));
        printf("MV %3d: 0x%08X\n", 148, ReadREG(WMV_MV_REG_OFST + 148 * 4));
        printf("MV %3d: 0x%08X\n", 149, ReadREG(WMV_MV_REG_OFST + 149 * 4));
        printf("MV %3d: 0x%08X\n", 150, ReadREG(WMV_MV_REG_OFST + 150 * 4));
        printf("MV %3d: 0x%08X\n", 151, ReadREG(WMV_MV_REG_OFST + 151 * 4));
        printf("MV %3d: 0x%08X\n", 152, ReadREG(WMV_MV_REG_OFST + 152 * 4));
        printf("MV %3d: 0x%08X\n", 180, ReadREG(WMV_MV_REG_OFST + 180 * 4));

        //printf("DCAC %3d: 0x%08X\n", 0, ReadREG(WMV_DCAC_REG_OFST + 0 * 4));
        //printf("DCAC %3d: 0x%08X\n", 1, ReadREG(WMV_DCAC_REG_OFST + 1 * 4));
        for (i = 0; i <= 255; i++)
        {
            printf("DCAC %3d: 0x%08X\n", i, ReadREG(WMV_DCAC_REG_OFST + i * 4));
        }
    }
}
#endif  // CC_WMV_EMULATION


void vSetNewMode(void)
{
#if defined(CC_MT5365) || defined(CC_MT5395)
    if (rWMVDecInfo.rOther.fgNewMode/* && rWMVDecInfo.rOther.i4CodecVersion != WMV3*/)
    {
        UINT32 a163, a164, a170, a171;
        INT32 i4OwBp;

        // VLD part
        // 163
        a163 = (WMV_NEW_FLG_EN | WMV_PRED_MOD_EN | WMV_PRED_RD_SYNC_DISABLE
                | WMV_DRAM_BURST_MODE_EN  // ???
                | WMV_BP_USE_PRED_RD_EN); // ???
        WriteREG(RW_VLD_WMV_NEW_CTRL, a163);

        // 164
        a164 = PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Dcac);  // ???
        a164 |= (1<<30);
        // how about bit 31???
        WriteREG(RW_VLD_WMV_DCAC_NEW_ADDR, a164);

        // 167, 168, 169
        WriteREG(RW_VLD_WMV_BP0_NEW_ADDR, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Bp_1));  // ???
        WriteREG(RW_VLD_WMV_BP1_NEW_ADDR, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Bp_2));  // ???
        WriteREG(RW_VLD_WMV_BP2_NEW_ADDR, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Bp_3));  // ???

        // 170
        a170 = (rWMVDecInfo.rSeqHdrR.u4PicWidthDec / 16)  * 7  * (16/16);
        WriteREG(RW_VLD_WMV_OW_PRED, a170);

        // 171
        i4OwBp = (rWMVDecInfo.rSeqHdrR.u4PicHeightDec / 16)  * (16/16);
        a171 = ( (i4OwBp << WMV_NUM_OW_BP_0) | (i4OwBp << WMV_NUM_OW_BP_1) | (i4OwBp << WMV_NUM_OW_BP_2));
        WriteREG(RW_VLD_WMV_OW_BP, a171);
    }
#endif
}

