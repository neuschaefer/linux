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
 * $RCSfile: venc_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file venc_if.c
 *  Video Encoder - public interface
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "venc_if.h"
#include "venc_drvif.h"


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
VENC_HANDLE VENC_Open(UINT32 u4Type, VENC_PROFILE_T eProfile)
{
    VENC_HANDLE hVenc = VENC_NULL_HANDLE;
    _VENC_Open(&hVenc, u4Type, eProfile);
    return hVenc;
}

UINT32 VENC_Close(VENC_HANDLE hEnc)
{
    return _VENC_Close(hEnc);
}

UINT32 VENC_Init(VENC_HANDLE hEnc)
{
    return _VENC_Init(hEnc);
}

UINT32 VENC_Run(VENC_HANDLE hEnc)
{
    return _VENC_Run(hEnc);
}

UINT32 VENC_Stop(VENC_HANDLE hEnc)
{
    return _VENC_Stop(hEnc);
}

UINT32 VENC_SetParam(VENC_HANDLE hEnc, UINT32 u4Type, VOID *pvArg)
{
    return _VENC_SetParam(hEnc, u4Type, pvArg);
}

UINT32 VENC_GetParam(VENC_HANDLE hEnc, UINT32 u4Type, VOID *pvArg)
{
    return _VENC_GetParam(hEnc, u4Type, pvArg);
}

UINT32 VENC_SetCtrl(VENC_HANDLE hEnc, UINT32 u4Type, VOID *pvArg)
{
    return _VENC_SetCtrl(hEnc, u4Type, pvArg);
}

UINT32 VENC_GetCtrl(VENC_HANDLE hEnc, UINT32 u4Type, VOID *pvArg)
{
    return _VENC_GetCtrl(hEnc, u4Type, pvArg);
}

UINT32 VENC_EncFrameSync(VENC_HANDLE hEnc, VENC_IN_PIC_T *ptIn, VENC_OUT_PIC_T *ptOut)
{
    return _VENC_EncFrameSync(hEnc, ptIn, ptOut);
}

UINT32 VENC_EncFrameAsync(VENC_HANDLE hEnc, VENC_IN_PIC_T *ptIn, VENC_OUT_PIC_T *ptOut)
{
    return _VENC_EncFrameAsync(hEnc, ptIn, ptOut);
}

UINT32 VENC_SetRtParamSync(VENC_HANDLE hEnc, VENC_RT_PARM_T *ptParm)
{
    return _VENC_SetRtParamSync(hEnc, ptParm);
}

UINT32 VENC_SetRtParamAsync(VENC_HANDLE hEnc, VENC_RT_PARM_T *ptParm)
{
    return _VENC_SetRtParamAsync(hEnc, ptParm);
}

UINT32 VENC_SetRtCtrlSync(VENC_HANDLE hEnc, VENC_RT_CTRL_T *ptCtrl)
{
    return _VENC_SetRtCtrlSync(hEnc, ptCtrl);
}

UINT32 VENC_SetRtCtrlAsync(VENC_HANDLE hEnc, VENC_RT_CTRL_T *ptCtrl)
{
    return _VENC_SetRtCtrlAsync(hEnc, ptCtrl);
}


BOOL VENC_CkgenOnOff(BOOL fgEnable)
{
#define BLOCK_RST_CFG0 0x270
#define VENC_RST ((UINT32)1 << 19)
#define BLOCK_CKEN_CFG0 0x278
#define VENC_CKEN ((UINT32)1 << 19)
    if (fgEnable)
    {
        IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) & ~VENC_RST);
        IO_WRITE32(CKGEN_BASE, BLOCK_CKEN_CFG0, IO_READ32(CKGEN_BASE, BLOCK_CKEN_CFG0) | VENC_CKEN);
        IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) | VENC_RST);
    }
    else
    {
        IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) & ~VENC_RST);
        IO_WRITE32(CKGEN_BASE, BLOCK_CKEN_CFG0, IO_READ32(CKGEN_BASE, BLOCK_CKEN_CFG0) & ~VENC_CKEN);
    }

    UNUSED(fgEnable);
    return TRUE;
}


