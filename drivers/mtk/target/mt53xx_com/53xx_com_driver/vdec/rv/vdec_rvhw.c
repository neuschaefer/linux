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

/** @file vdec_rvhw.c
 *  This header file declares exported APIs of RV.
 */

#include "x_lint.h"
#include "vdec_rvhw.h"
#include "vdec_rvhw.h"
#include "vdec_rvutil.h"
#include "vdec_rvdec.h"
#ifdef USE_FBM
#include "fbm_drvif.h"
#endif

LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_common.h"
#include "x_typedef.h"
LINT_EXT_HEADER_END

#define DUMP_SIM_PAT


#if 0  // !!!
void WriteRVREG(UINT32 address, UINT32 value)
{
  HAL_WRITE32((address), (value));
}

UINT32 ReadRVREG(UINT32 address)
{
  return (HAL_READ32((address)));
}
#endif

VOID vWriteREG(UINT32 address, UINT32 value)
{
    HAL_WRITE32((address), (value));
}

UINT32 u4ReadREG(UINT32 address)
{
    return (HAL_READ32((address)));
}


VOID vInitRVVDec(void)
{
    //_rRVDecInfo.rOther.u1DecState = VDEC_NORM_INIT_PRM;
    _rRVDecInfo.rOther.u1GetSeqHdr = 0;
    _rRVDecInfo.rOther.u1FbgId = FBM_FBG_ID_UNKNOWN;
    vResetRVVDec();
    // initiate register files
    //vInitRVVdoMem();
    //vInitRVMiscMem();

    //vSetRVMcBufPtr();  !!! meaningless because pointers have not been initialized yet
    // vResetRVVDec();
    vSetRVVldVFifo();
    vSetRVFRefBuf(0);
    vSetRVBRefBuf(1);
}

VOID vInitRVVdoMem(void)
{
    _rRVDecInfo.rDram.pu1VFifo = (UINT8 *)_rRVDecInfo.rPesInfo.u4FifoStart;
    _rRVDecInfo.rDram.pu1FifoEnd = (UINT8 *)_rRVDecInfo.rPesInfo.u4FifoEnd;

#ifndef USE_FBM
    _rRVDecInfo.rDram.pu1Pic0Y = (UINT8 *)PIC0Y_SA; //byte address
    _rRVDecInfo.rDram.pu1Pic0C = (UINT8 *)PIC0C_SA;
    _rRVDecInfo.rDram.pu1Pic1Y = (UINT8 *)PIC1Y_SA;
    _rRVDecInfo.rDram.pu1Pic1C = (UINT8 *)PIC1C_SA;
    _rRVDecInfo.rDram.pu1Pic2Y = (UINT8 *)PIC2Y_SA;
    _rRVDecInfo.rDram.pu1Pic2C = (UINT8 *)PIC2C_SA;
#else
    if (FBM_ChkFrameBufferFlag(_rRVDecInfo.rOther.u1FbgId, FBM_FLAG_RELEASE_FBG))
    {
        FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, 0);
        FBM_WaitUnlockFrameBuffer(_rRVDecInfo.rOther.u1FbgId, 20);    // block until display unlock
        FBM_ReleaseGroup(_rRVDecInfo.rOther.u1FbgId);
        _rRVDecInfo.rOther.u1FbgId = FBM_FBG_ID_UNKNOWN;
    }

    LOG(0,"hello it's the resolution : %dx%d\n",_rRVDecInfo.rRvPicAttr.u4OrgWidth,_rRVDecInfo.rRvPicAttr.u4OrgHeight);

    if (_rRVDecInfo.rOther.u1FbgId == FBM_FBG_ID_UNKNOWN)
    {
        VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(0);
        UINT32 u4FbWidth, u4FbHeight;
        UINT8 u1FbgType;

        // !!! ToDo: initialize FBM after getting the first picture's width and height from V Parser
        u1FbgType = FBM_SelectGroupType(_rRVDecInfo.rRvPicAttr.u4OrgWidth, _rRVDecInfo.rRvPicAttr.u4OrgHeight);
        _rRVDecInfo.rOther.u1FbgId = FBM_CreateGroup(u1FbgType, FBM_VDEC_RV, 0, 0);
        FBM_SetPlayMode(_rRVDecInfo.rOther.u1FbgId,FBM_FBG_MM_MODE);
        ASSERT(_rRVDecInfo.rOther.u1FbgId != FBM_FBG_ID_UNKNOWN);
        prVdecEsInfo->ucFbgId = _rRVDecInfo.rOther.u1FbgId;
        FBM_SetSyncStc(_rRVDecInfo.rOther.u1FbgId, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId);
        FBM_GetFrameBufferSize(prVdecEsInfo->ucFbgId, &u4FbWidth, &u4FbHeight);
        FBM_FbgChgNotify(_rRVDecInfo.rOther.u1FbgId, 0);
        _rRVDecInfo.rRvPicAttr.u4LineSize = u4FbWidth;
        //prDecPrm->u1FbgType = u1FbgType;
    }
    else
    {
    }
#endif

    vInitRVMiscMem();
}

VOID vInitRVMiscMem(void)
{
    UINT32 u4WorkingBuffer;
    UINT32 u4WorkingSize;

    FBM_GetWorkingBuffer(_rRVDecInfo.rOther.u1FbgId, &u4WorkingBuffer, &u4WorkingSize);
    _rRVDecInfo.rDram.pu1VldPred = (UINT8*)PHYSICAL(u4WorkingBuffer);
    _rRVDecInfo.rDram.pu1MvDirect = _rRVDecInfo.rDram.pu1VldPred + VLD_PRED_OFFSET_SZ;
    ASSERT((UINT32)(_rRVDecInfo.rDram.pu1MvDirect - _rRVDecInfo.rDram.pu1VldPred) + MV_DIRECT_SZ <= u4WorkingSize);
}

VOID vInitRVVld(void)
{
#if 0  // !!!
    if (rWMVDecInfo.rOther.i4CodecVersion == WMVA)
    {
        //Robert: Enable 0x03 replancement before initializing barrel shifter.
        vWriteREG(RW_VLD_WMV_ABS, 0x1);
    }
    else
    {
        vWriteREG(RW_VLD_WMV_ABS, 0x0);
    }
    vWriteREG(RW_VLD_VSTART, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1VFifo)>>6); // (128-bit address >> 6)
    //spork
    vWriteREG(RW_VLD_VEND, PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1FifoEnd)>>6);
    fgVldInitBarrelShifter((UINT32)rWMVDecInfo.rDram.pu1VFifo);

    // ilii, clean register
    vWriteREG(RW_VLD_DECSTART, 1<<8);
    vWriteREG(RW_VLD_DECSTART, 0);
    vWriteREG(RW_VLD_VDOUFM, 1);
#endif
}

VOID vInitRVPred(void)
{
}

VOID vResetRVVDec(void)
{
    #ifdef WAIT_SRAM_STABLE
        UINT32 u4Tmp = 0;
        while((u4ReadREG(RO_VLD_SRAMCTRL) & AA_FIT_TARGET_D)==0)
        {
            u4Tmp++;
            if(u4Tmp >= 0x10000)
            {
                ASSERT(0);
            }
        }
    #endif
    vWriteREG(WO_VLD_SRST, 1);
    vWriteREG(WO_VLD_SRST, 0);
}

VOID vSetRVVldVFifo(void)
{
  vWriteREG(RW_VLD_VSTART, (UINT32)_rRVDecInfo.rDram.pu1VFifo >> 6);
  vWriteREG(RW_VLD_VEND, (UINT32)_rRVDecInfo.rDram.pu1FifoEnd>>6);
}

VOID vSetRVMcBufPtr(VOID)
{
#if 1
    if (_rRVDecInfo.rDram.u4FRefBufIdx == 0 && _rRVDecInfo.rDram.u4BRefBufIdx == 1)
    {
        vWriteREG(RW_MC_R1Y, PHYSICAL((UINT32)_rRVDecInfo.rDram.pu1Pic0Y) >> 9); // div 512
        vWriteREG(RW_MC_R1C, PHYSICAL((UINT32)_rRVDecInfo.rDram.pu1Pic0C) >> 8); // div 256
        vWriteREG(RW_MC_R2Y, PHYSICAL((UINT32)_rRVDecInfo.rDram.pu1Pic1Y) >> 9); // div 512
        vWriteREG(RW_MC_R2C, PHYSICAL((UINT32)_rRVDecInfo.rDram.pu1Pic1C) >> 8); // div 256
    }
    else if (_rRVDecInfo.rDram.u4FRefBufIdx == 1 && _rRVDecInfo.rDram.u4BRefBufIdx == 0)
    {
        vWriteREG(RW_MC_R1Y, PHYSICAL((UINT32)_rRVDecInfo.rDram.pu1Pic1Y) >> 9); // div 512
        vWriteREG(RW_MC_R1C, PHYSICAL((UINT32)_rRVDecInfo.rDram.pu1Pic1C) >> 8); // div 256
        vWriteREG(RW_MC_R2Y, PHYSICAL((UINT32)_rRVDecInfo.rDram.pu1Pic0Y) >> 9); // div 512
        vWriteREG(RW_MC_R2C, PHYSICAL((UINT32)_rRVDecInfo.rDram.pu1Pic0C) >> 8); // div 256
    }
    else
    {
        ASSERT(0);
    }
#else
    vWriteREG(RW_MC_R1Y, PHYSICAL((UINT32)_rRVDecInfo.rDram.pu1Pic0Y) >> 9); // div 512
    vWriteREG(RW_MC_R1C, PHYSICAL((UINT32)_rRVDecInfo.rDram.pu1Pic0C) >> 8); // div 256
    vWriteREG(RW_MC_R2Y, PHYSICAL((UINT32)_rRVDecInfo.rDram.pu1Pic1Y) >> 9); // div 512
    vWriteREG(RW_MC_R2C, PHYSICAL((UINT32)_rRVDecInfo.rDram.pu1Pic1C) >> 8); // div 256
#endif
    vWriteREG(RW_MC_BY, PHYSICAL((UINT32)_rRVDecInfo.rDram.pu1Pic2Y)  >> 8); // div 256
    vWriteREG(RW_MC_BC, PHYSICAL((UINT32)_rRVDecInfo.rDram.pu1Pic2C)  >> 7); // div 128
    vWriteREG(RW_MC_BY1, PHYSICAL((UINT32)_rRVDecInfo.rDram.pu1Pic2Y) >> 9); // div 512
    vWriteREG(RW_MC_BC1, PHYSICAL((UINT32)_rRVDecInfo.rDram.pu1Pic2C) >> 8); // div 256
    vWriteREG(RW_MC_PP_Y_ADDR, PHYSICAL((UINT32)_rRVDecInfo.rDram.pu1Pic2Y) >> 9); // div 512  // !!!
    vWriteREG(RW_MC_PP_C_ADDR, PHYSICAL((UINT32)_rRVDecInfo.rDram.pu1Pic2C) >> 8); // div 256  // !!!
}

VOID vSetRVVSyncPrmBufPtr(UINT32 dwBufIdx)
{
    UCHAR ucFbId =0 ;
    FBM_PIC_HDR_T * prFbmPicHdr = NULL;
    switch(dwBufIdx)
    {
        case 0:
            _rRVDecInfo.rOther.rVSyncPrm.dwPtrY = (UINT32)_rRVDecInfo.rDram.pu1Pic2Y;  // _rRVDecInfo.rDram.pu1Pic0Y; !!!
            _rRVDecInfo.rOther.rVSyncPrm.dwPtrC = (UINT32)_rRVDecInfo.rDram.pu1Pic2C;  // _rRVDecInfo.rDram.pu1Pic0C; !!!
            ucFbId = _rRVDecInfo.rOther.u1FbId0;
        break;
        case 1:
            _rRVDecInfo.rOther.rVSyncPrm.dwPtrY = (UINT32)_rRVDecInfo.rDram.pu1Pic2Y;  // _rRVDecInfo.rDram.pu1Pic1Y; !!!
            _rRVDecInfo.rOther.rVSyncPrm.dwPtrC = (UINT32)_rRVDecInfo.rDram.pu1Pic2C;  // _rRVDecInfo.rDram.pu1Pic1C; !!!
            ucFbId = _rRVDecInfo.rOther.u1FbId1;
        break;
        case 2:
            _rRVDecInfo.rOther.rVSyncPrm.dwPtrY = (UINT32)_rRVDecInfo.rDram.pu1Pic2Y;
            _rRVDecInfo.rOther.rVSyncPrm.dwPtrC = (UINT32)_rRVDecInfo.rDram.pu1Pic2C;
            ucFbId = _rRVDecInfo.rOther.u1FbId2;
        break;
    }

    #ifdef USE_FBM
            if (_rRVDecInfo.rRvPicAttr.ePtype == RV_TRUEBPIC)
            {
                BOOL fgNotDisplay;
                //FBM_PIC_HDR_T * prFbmPicHdr;
                VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(0);

                prFbmPicHdr = FBM_GetFrameBufferPicHdr(_rRVDecInfo.rOther.u1FbgId, ucFbId);

                if(!_rRVDecInfo.rOther.fgFirstDispFrame) //skip first B frame when no I frame is displayed
                {
                    if(prFbmPicHdr)
                    {
                        if(_rRVDecInfo.rOther.fgFirstDispFrame)
                        {
                             _rRVDecInfo.rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                             _rRVDecInfo.rOther.fgFirstDispFrame = FALSE;
                        }
                        else
                        {
                            if(_rRVDecInfo.rPesInfo.fgDtsValid/* && (prFbmPicHdr->u4PTS >= _rRVDecInfo.rOther.u4LastPTS)*/)
                            {
                                _rRVDecInfo.rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                            }
                            else
                            {
                                prFbmPicHdr->u4PTS = _rRVDecInfo.rOther.u4LastPTS + _rRVDecInfo.rOther.u4DeltaPTS;
                                _rRVDecInfo.rOther.u4LastPTS += _rRVDecInfo.rOther.u4DeltaPTS;
                            }
                        }
                        fgNotDisplay = VDEC_IsNotDisplay(0, prFbmPicHdr->u4PTS, 0, 0);
                    }
                    else
                    {
                fgNotDisplay = VDEC_IsNotDisplay(0, 0, 0, 0);
                    }
                }
                else
                {
                    fgNotDisplay = TRUE;
                }
                //fgNotDisplay = VDEC_IsNotDisplay(0, 0, 0, 0);

                if (fgNotDisplay)
                {
                    #ifdef RV_PRINT_FBM_STATUS
                    UCHAR u1state;
                    u1state = FBM_GetFrameBufferStatus(_rRVDecInfo.rOther.u1FbgId,ucFbId);
                    LOG(0,"RV set B frame FBM status : %d %d EMPTY\n",ucFbId,u1state);
                    #endif
                    FBM_SetFrameBufferStatus(_rRVDecInfo.rOther.u1FbgId, ucFbId, FBM_FB_STATUS_EMPTY);
                }
                else
                {
                    #ifdef RV_PRINT_FBM_STATUS
                    UCHAR u1state;
                    u1state = FBM_GetFrameBufferStatus(_rRVDecInfo.rOther.u1FbgId,ucFbId);
                    LOG(0,"RV set B frame FBM status : %d %d DISPALY_Q\n",ucFbId,u1state);
                    #endif

                    prFbmPicHdr = FBM_GetFrameBufferPicHdr(_rRVDecInfo.rOther.u1FbgId, ucFbId);
                    FBM_SetFrameBufferStatus(_rRVDecInfo.rOther.u1FbgId, ucFbId, FBM_FB_STATUS_DISPLAYQ);
                    prVdecEsInfo->u4DisplayQPicCnt++;
                    if(prFbmPicHdr)
                    {
                        VDEC_CheckStopStatus(0, prFbmPicHdr->u4PTS, 0);
                    }
                    else
                    {
                        VDEC_CheckStopStatus(0, 0, 0);
                    }
                }

            }
            else
            {
                FBM_SetFrameBufferStatus(_rRVDecInfo.rOther.u1FbgId, ucFbId, FBM_FB_STATUS_READY);
            }
    #endif
}

#if 0
VOID vRVVSyncWriteVdoReg(void)
{
    vWriteREG(RW_VDO_PTRY, PHYSICAL(_rRVDecInfo.rOther.rVSyncPrm.dwPtrY >> 3));
    vWriteREG(RW_VDO_PTRC, PHYSICAL(_rRVDecInfo.rOther.rVSyncPrm.dwPtrC >> 3));
}
#endif

VOID _VDEC_UpdateVldRVWPtr(UINT32 u4WPtr)
{
    //UINT32 u4RegVal;

    UINT32 u4WPtrAlign, u4VStart, u4VEnd;

    u4WPtrAlign = ((u4WPtr + RPTR_ALIGN) & (~((UINT32)RPTR_ALIGN - 1))) + WPTR_OFFSET;

    u4VStart = u4ReadREG(RW_VLD_VSTART)<<6;
    u4VEnd = u4ReadREG(RW_VLD_VEND)<<6;

    if(u4WPtrAlign < u4VStart)
    {
        u4WPtrAlign = u4VEnd - (u4VStart - u4WPtrAlign);
    }
    if(u4WPtrAlign > u4VEnd)
    {
        u4WPtrAlign = u4VStart + (u4WPtrAlign - u4VEnd);
    }
#if defined (CC_MT5392B)
    vWriteREG(WO_VLD_VWPTR,u4WPtrAlign);
#else
    //u4RegVal = (u4ReadREG(WO_VLD_VWPTR) & 0x7)|(u4WPtr << 3);
    vWriteREG(WO_VLD_VWPTR, (u4WPtrAlign << 3) | 7);    //set wptr infinity
#endif


}

UINT32 dwRVVldRPtr(UINT32 *pdwBits)
{

    UINT32 dwDramRptr;
    UINT32 dwSramRptr, dwSramWptr;
    UINT32 dwSramDataSz;
    UINT32 dwByteAddr;

    #ifdef WAIT_SRAM_STABLE
        UINT32 u4Tmp = 0;
        while((u4ReadREG(RO_VLD_SRAMCTRL) & AA_FIT_TARGET_D)==0)
        {
            u4Tmp++;
            if(u4Tmp >= 0x10000)
            {
                LOG(0, "//===SRAM !Stable 0xF4=0x%x, Rptr 0xB0=0x%x, 0xFC=0x%x, 0xEC=0x%x, 0xE8=0x%x\n",
                u4ReadREG(RO_VLD_SRAMCTRL), u4ReadREG(RW_VLD_RPTR), u4ReadREG(RO_VLD_VRPTR),
                u4ReadREG(RO_VLD_VBAR), u4ReadREG(RO_VLD_FETCHOK));
                return FALSE;
            }
        }
    #endif

    dwDramRptr = u4ReadREG(RO_VLD_VRPTR);// & 0xffffff;
    dwSramRptr = u4ReadREG(RO_VLD_VBAR)&0x3f;// & 0xff;
    dwSramWptr = (u4ReadREG(RO_VLD_VBAR) >> 16)&0x3f;// & 0xff;

      /*if(!fgWaitVldFetchOk())
       {
       LOG(3,"VLD RPtr wait vld not ok\n");
       }*/

      //dwDramRptr = u4ReadREG(RO_VLD_VRPTR);// & 0xffffff;
      //dwSramRptr = u4ReadREG(RO_VLD_VBAR)&0x3f;// & 0xff;
      //dwSramWptr = (u4ReadREG(RO_VLD_VBAR) >> 16)&0x3f;// & 0xff;

    if (dwSramWptr > dwSramRptr)
    {
        dwSramDataSz = dwSramWptr - dwSramRptr;
    }
    else
    {
        dwSramDataSz = 64 - (dwSramRptr - dwSramWptr);
    }

    *pdwBits = u4ReadREG(RW_VLD_BITCOUNT) & 0x3f;
    //*pdwBits = (u4ReadREG(RW_VLD_BITCOUNT)>>16) & 0x3f;

    if(u4ReadREG(RW_VLD_WMV_ABS) & 0x1) // 03 replacement enable!
    {
        dwByteAddr = dwDramRptr - (dwSramDataSz + 4) * 4 + (*pdwBits / 8);
    }
    else
    {
        dwByteAddr = dwDramRptr - (dwSramDataSz + 3)* 4 + (*pdwBits / 8);
    }
    //spork
    *pdwBits &= 0x7;
    dwByteAddr -= ((UINT32)_rRVDecInfo.rDram.pu1VFifo);
    return (dwByteAddr);
}

