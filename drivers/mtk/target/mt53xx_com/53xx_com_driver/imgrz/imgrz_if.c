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
 * $RCSfile: imgrz_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
 
/** @file imgrz_if.c
 *  main interfaces of image resizer driver
 *
 */

/*
    lint inhibition(s)
        506 - Constant value Boolean [MTK Rule 6.1.3]
        534 - Ignoring return value of function
        774 - Boolean within 'right side of && within if' always
              evaluates to True [Reference: file imgrz_if.c: lines 919, 935]
        826 - Suspicious pointer-to-pointer conversion (area too small)
        831 - Loss of sign (assignment) (signed char to unsigned long)
*/


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------


#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "vdp_if.h"
#include "b2r_if.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  IMGRZ_Init
//
/** Initialize image resizer driver.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Init(void)
{
    _IMGRZ_Init();
}


//---------------------------------------------------------------------------
//  IMGRZ_Reset
//
/** Reset engine.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Reset(void)
{
    _IMGRZ_Reset();
}


//---------------------------------------------------------------------------
//  IMGRZ_Wait
//
/** Wait for flushing engine completely.
 *  
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Wait(void)
{
    _IMGRZ_Wait();
}


//---------------------------------------------------------------------------
//  IMGRZ_MwFlush
//
/** Flush cmdque and force engine to do operations (for MW use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_MwFlush(void)
{
    _IMGRZ_MwFlush();
}


//---------------------------------------------------------------------------
//  IMGRZ_Flush
//
/** Flush cmdque and force engine to do operations (for Drv use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Flush(void)
{
    _IMGRZ_Flush();
}


//---------------------------------------------------------------------------
//  IMGRZ_Lock
//
/** Take the imgrz resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Lock(void)
{
    _IMGRZ_Lock();
}


//---------------------------------------------------------------------------
//  IMGRZ_TryLock
//
/** Try to take the imgrz resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_TryLock(void)
{
    _IMGRZ_TryLock();
}


//---------------------------------------------------------------------------
//  IMGRZ_Unlock
//
/** Release the imgrz resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Unlock(void)
{
    _IMGRZ_Unlock();
}


//---------------------------------------------------------------------------
//  IMGRZ_LockCmdque
//
/** Take the imgrz-cmdq resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_LockCmdque(void)
{
    _IMGRZ_LockCmdque();
}


//---------------------------------------------------------------------------
//  IMGRZ_UnlockCmdque
//
/** Release the imgrz-cmdq resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_UnlockCmdque(void)
{
   //LOG(0, "IMGRZ_UnlockCmdque.....\n");

    _IMGRZ_UnlockCmdque();
}

//---------------------------------------------------------------------------
//  IMGRZ_LockWaitIsr
//
/** Take the imgrz- waitIsr cmd resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_LockWaitIsr(void)
{
    _IMGRZ_LockWaitIsr();
}


//---------------------------------------------------------------------------
//  IMGRZ_UnlockWaitIsr
//
/** Release the imgrz-waitIsr cmd resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_UnlockWaitIsr(void)
{
    _IMGRZ_UnlockWaitIsr();
}

//---------------------------------------------------------------------------
//  IMGRZ_QueryHwIdle
//
/** Query engine status.
 *
 *  @retval 1   Idle
 *  @retval 0   Busy
 */
//---------------------------------------------------------------------------
INT32 IMGRZ_QueryHwIdle(void)
{
    return _IMGRZ_QueryHwIdle();
}


//---------------------------------------------------------------------------
//  IMGRZ_SetNotify
//
/** Set notify function (for MW use).
 *
 *  @param  pfnNotifyFunc   The callback function pointer
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_SetNotify(void (*pfnNotifyFunc)(UINT32))
{
    _IMGRZ_SetNotify(pfnNotifyFunc);
}


//---------------------------------------------------------------------------
//  IMGRZ_ReInit
//
/** Re initial imgrz.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_ReInit(void)
{
    _IMGRZ_ReInit();
}


//---------------------------------------------------------------------------
//  IMGRZ_SetScaleOpt
//
/** Set scale option.
 *
 *  @param  eInOutType   Scale option
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_T eInOutType)
{
    _IMGRZ_SetScaleOpt(eInOutType);
}


//---------------------------------------------------------------------------
//  IMGRZ_Scale
//
/** Execute scale operation.
 *
 *  @param  pvSclParam  scale parameters
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Scale(void *pvSclParam)
{
    _IMGRZ_Scale(pvSclParam);
}


//---------------------------------------------------------------------------
//  IMGRZ_GetScaleOpt
//
/** get scale option.
 *  @retval void
 */
//---------------------------------------------------------------------------
E_RZ_INOUT_TYPE_T IMGRZ_GetScaleOpt(void)
{
    return _IMGRZ_GetScaleOpt();
}


//---------------------------------------------------------------------------
//  IMGRZ_SetDigest
//
/** Set a video output frame to a thumbnail result
 *
 *  @param  pvSrc  source parameters
 *  @param  pvDst  distination parameters
 *  @param  fgOnePass FALSE: 2pass (only video scaling), TRUE: 1 pass (scaling and color conv)
 *  @param  u4WorkingBuf  working buffer address for 2 pass 
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_SetDigest(void *pvSrc, void *pvDst, BOOL fgOnePass, UINT32 u4WorkingBuf)
{
    RZ_VDO_SCL_PARAM_SET_T rSclParam;
    VDP_CAPTURE_INTO_T *pSrc = (VDP_CAPTURE_INTO_T *)pvSrc;
    VDP_THUMBNAIL_INFO_T *pDst = (VDP_THUMBNAIL_INFO_T *)pvDst;
	x_memset(&rSclParam,0,sizeof(RZ_VDO_SCL_PARAM_SET_T));
    rSclParam.u4IsRsIn      = pSrc->u4IsRsIn;
    rSclParam.u4YSrcBase    = pSrc->u4AddrY;
    rSclParam.u4YSrcBufLen  = D_RZ_ALIGN16(pSrc->u4MemHSize);
    rSclParam.u4YSrcHOffset = 0;
    rSclParam.u4YSrcVOffset = 0;
    rSclParam.u4YSrcW       = pSrc->u4HSize;
    rSclParam.u4YSrcH       = pSrc->u4VSize;
    rSclParam.u4CSrcBase    = pSrc->u4AddrC;
    rSclParam.u4CSrcHOffset = 0;
    rSclParam.u4CSrcVOffset = 0;
    rSclParam.u4CSrcW       = pSrc->u4HSize >> 1;
    rSclParam.u4CSrcH       = pSrc->u4VSize >> 1;
    rSclParam.u4IsRsOut     = 1;
    rSclParam.u4HFilterMode=0;
	rSclParam.u4VFilterMode=1;
    if (fgOnePass)
    {
        //method 1: Y/C --> AYCbCr8888, 1 pass
        rSclParam.u4OutMode     = (UINT32) E_RZ_VDO_OUTMD_444;
        rSclParam.u4IsVdo2Osd   = 1;
        rSclParam.u4YTgBase     = (UINT32) pDst->pucCanvasBuffer;
        rSclParam.u4YTgCM       = (UINT32) E_RZ_OSD_DIR_CM_AYCbCr8888;
        rSclParam.u4YTgBufLen   = D_RZ_ALIGN16(pDst->u4CanvasPitch);
        rSclParam.u4CTgBase     = (UINT32) pDst->pucCanvasBuffer;
        rSclParam.u4YTgHOffset  = pDst->u4ThumbnailX;
        rSclParam.u4YTgVOffset  = pDst->u4ThumbnailY;
        rSclParam.u4CTgW        = pDst->u4ThumbnailWidth;
        rSclParam.u4CTgH        = pDst->u4ThumbnailHeight;
    }
    else
    {
        //method 2: Y/C --> RGB, 2 pass
        // 1st pass: VDO scaling
        rSclParam.u4OutMode     = (UINT32) E_RZ_VDO_OUTMD_420;
        rSclParam.u4IsVdo2Osd   = 0;
        rSclParam.u4YTgBase     = u4WorkingBuf;
        rSclParam.u4YTgCM       = (UINT32) E_RZ_VDO_OUTMD_420;
        rSclParam.u4YTgBufLen   = D_RZ_ALIGN16(pDst->u4ThumbnailWidth);
        rSclParam.u4CTgBase     = u4WorkingBuf + (rSclParam.u4YTgBufLen * pDst->u4ThumbnailHeight);  
        rSclParam.u4YTgHOffset  = 0;
        rSclParam.u4YTgVOffset  = 0;
        rSclParam.u4CTgW        = pDst->u4ThumbnailWidth >> 1;
        rSclParam.u4CTgH        = pDst->u4ThumbnailHeight >> 1;
    }

    rSclParam.u4YTgW        = pDst->u4ThumbnailWidth;
    rSclParam.u4YTgH        = pDst->u4ThumbnailHeight;
    rSclParam.u4SrcSwap     = pSrc->u4SrcSwap;//(UINT32)IMGRZ_SWAP;
	rSclParam.u4OutSwap=0;
    IMGRZ_Lock();
    IMGRZ_ReInit();
    IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
    IMGRZ_Scale(&rSclParam);
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();
}
//===================
// OSD mode
//===================

//---------------------------------------------------------------------------
//  IMGRZ_Break
//
/** Save imgrz parameters for breaking
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Break(void)
{
    _IMGRZ_Break();
}

//---------------------------------------------------------------------------
//  IMGRZ_Resume
//
/** Resume the process of imgrz of jpg mcu row mode
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Resume(void)
{
    _IMGRZ_Resume();
}

//---------------------------------------------------------------------------
//  IMGRZ_OFF
//
/** disable clock of imgrz
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_OFF(void)
{
    _IMGRZ_OFF();
}

//-------------------------------------------------------------------------
/**IMGRZ_DMA
 *  Move Dram data form u4SrcAddr to u4DstAddr, size is u4Pitch x u4Height
 *  u4SrcAddr, u4DstAddr, u4Pitch should be 16 byte aligement.
 */
//-------------------------------------------------------------------------
void IMGRZ_DMA(UINT32 u4SrcAddr,UINT32 u4DstAddr, UINT32 u4Pitch,UINT32 u4Height)
{
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    x_memset((void *)&rSclParam, 0, sizeof(RZ_OSD_SCL_PARAM_SET_T));
    // set scaling parameters
    rSclParam.u4SrcBase     = u4SrcAddr;
    rSclParam.u4IsSrcDirCM  = IMGRZ_TRUE;
    rSclParam.u4SrcCM       = 3;
    rSclParam.u4SrcBufLen   = u4Pitch;
    rSclParam.u4SrcHOffset  = 0;
    rSclParam.u4SrcVOffset  = 0;
    rSclParam.u4SrcW        = u4Pitch>>2;
    rSclParam.u4SrcH        = u4Height;

    rSclParam.u4TgBase      = u4DstAddr;
    rSclParam.u4TgCM        = 3;
    rSclParam.u4TgBufLen    = u4Pitch;
    rSclParam.u4TgHOffset   = 0;
    rSclParam.u4TgVOffset   = 0;
    rSclParam.u4TgW         = u4Pitch>>2;
    rSclParam.u4TgH         = u4Height;

    IMGRZ_Lock();
    IMGRZ_ReInit();
    _IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_OSDMD);
    IMGRZ_Scale(&rSclParam);
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();
    return;
}

