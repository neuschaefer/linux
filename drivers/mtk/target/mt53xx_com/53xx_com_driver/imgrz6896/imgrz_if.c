/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *---------------------------------------------------------------------------
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
    _IMGRZ_Init(E_FIRST_RESIZER);
}
void IMGRZ_Init_Ex(UINT32 u4Which)
{
    _IMGRZ_Init(u4Which);
}

void IMGRZ_Cfg_MMU_IO(UINT32 u4Which, BOOL MMU_Read, BOOL MMU_Write)
{
    _IMGRZ_Cfg_MMU_IO(u4Which, MMU_Read, MMU_Write);
}

//---------------------------------------------------------------------------
//  IMGRZ_Reset
//
/** Reset engine.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Reset(UINT32 u4Which)
{
    _IMGRZ_Reset(u4Which);
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
    _IMGRZ_Wait(E_FIRST_RESIZER);
}
void IMGRZ_Wait_Ex(UINT32 u4Which)
{
    _IMGRZ_Wait(u4Which);
}


//---------------------------------------------------------------------------
//  IMGRZ_MwFlush
//
/** Flush cmdque and force engine to do operations (for MW use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_MwFlush(UINT32 u4Which)
{
    _IMGRZ_MwFlush(u4Which);
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
    _IMGRZ_Flush(E_FIRST_RESIZER);
}
void IMGRZ_Flush_Ex(UINT32 u4Which)
{
    _IMGRZ_Flush(u4Which);
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
    _IMGRZ_Lock(E_FIRST_RESIZER);
}
void IMGRZ_Lock_Ex(UINT32 u4Which)
{
    _IMGRZ_Lock(u4Which);
}


//---------------------------------------------------------------------------
//  IMGRZ_TryLock
//
/** Try to take the imgrz resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_TryLock(UINT32 u4Which)
{
    _IMGRZ_TryLock(u4Which);
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
    _IMGRZ_Unlock(E_FIRST_RESIZER);
}
void IMGRZ_Unlock_Ex(UINT32 u4Which)
{
    _IMGRZ_Unlock(u4Which);
}


//---------------------------------------------------------------------------
//  IMGRZ_LockCmdque
//
/** Take the imgrz-cmdq resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_LockCmdque(UINT32 u4Which)
{
    _IMGRZ_LockCmdque(u4Which);
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

    _IMGRZ_UnlockCmdque(E_FIRST_RESIZER);
}
void IMGRZ_UnlockCmdque_Ex(UINT32 u4Which)
{
    _IMGRZ_UnlockCmdque(u4Which);
}

//---------------------------------------------------------------------------
//  IMGRZ_LockWaitIsr
//
/** Take the imgrz- waitIsr cmd resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_LockWaitIsr(UINT32 u4Which)
{
    _IMGRZ_LockWaitIsr(u4Which);
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
    _IMGRZ_UnlockWaitIsr(E_FIRST_RESIZER);
}
void IMGRZ_UnlockWaitIsr_Ex(UINT32 u4Which)
{
    _IMGRZ_UnlockWaitIsr(u4Which);
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
INT32 IMGRZ_QueryHwIdle(UINT32 u4Which)
{
    return _IMGRZ_QueryHwIdle(u4Which);
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
void IMGRZ_SetNotify(UINT32 u4Which, void (*pfnNotifyFunc)(UINT32))
{
    _IMGRZ_SetNotify(u4Which, pfnNotifyFunc);
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
    _IMGRZ_ReInit(E_FIRST_RESIZER);
}
void IMGRZ_ReInit_Ex(UINT32 u4Which)
{
    _IMGRZ_ReInit(u4Which);
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
    _IMGRZ_SetScaleOpt(E_FIRST_RESIZER, eInOutType);
}
void IMGRZ_SetScaleOpt_Ex(UINT32 u4Which, E_RZ_INOUT_TYPE_T eInOutType)
{
    _IMGRZ_SetScaleOpt(u4Which, eInOutType);
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
    _IMGRZ_Scale(E_FIRST_RESIZER, pvSclParam);
}
void IMGRZ_Scale_Ex(UINT32 u4Which, void *pvSclParam)
{
    _IMGRZ_Scale(u4Which, pvSclParam);
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
    return _IMGRZ_GetScaleOpt(E_FIRST_RESIZER);
}
E_RZ_INOUT_TYPE_T IMGRZ_GetScaleOpt_Ex(UINT32 u4Which)
{
    return _IMGRZ_GetScaleOpt(u4Which);
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
    UINT32 u4Offset=0;
    x_memset((void *)&rSclParam, 0, sizeof(RZ_VDO_SCL_PARAM_SET_T));
    rSclParam.u4IsRsIn      = pSrc->u4IsRsIn;
    rSclParam.u4YSrcBase    = pSrc->u4AddrY;
    rSclParam.u4YSrcBufLen  = D_RZ_ALIGN16(pSrc->u4MemHSize);
    rSclParam.u4CSrcBufLen  = D_RZ_ALIGN16(pSrc->u4MemHSize);
    rSclParam.u4YSrcHOffset = 0;
	if(pSrc->u4VSize>pDst->u4ThumbnailHeight)
	{
		u4Offset=pSrc->u4VSize/pDst->u4ThumbnailHeight;
		u4Offset-=u4Offset&1;
	}
    rSclParam.u4YSrcVOffset = u4Offset>>1;
    rSclParam.u4YSrcW       = pSrc->u4HSize;
    rSclParam.u4YSrcH       = pSrc->u4VSize;
    rSclParam.u4CSrcBase    = pSrc->u4AddrC;
    rSclParam.u4CSrcHOffset = 0;
    rSclParam.u4CSrcVOffset = rSclParam.u4YSrcVOffset>>1;
    rSclParam.u4CSrcW       = pSrc->u4HSize >> 1;
    rSclParam.u4CSrcH       = pSrc->u4VSize >> 1;
    rSclParam.u4IsRsOut     = 1;

    if (fgOnePass)
    {
        //method 1: Y/C --> AYCbCr8888, 1 pass
        rSclParam.u4OutMode     = (UINT32) E_RZ_VDO_OUTMD_444;
        rSclParam.u4IsVdo2Osd   = 1;
        rSclParam.u4YTgBase     = (UINT32) pDst->pucCanvasBuffer;
        rSclParam.u4YTgCM       = (UINT32) E_RZ_OSD_DIR_CM_AYCbCr8888;
        rSclParam.u4YTgBufLen   = D_RZ_ALIGN16(pDst->u4CanvasPitch);
        rSclParam.u4CTgBufLen   = D_RZ_ALIGN16(pDst->u4CanvasPitch);        
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
        rSclParam.u4CTgBufLen   = D_RZ_ALIGN16(pDst->u4ThumbnailWidth);        
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
    rSclParam.u4ABlend = 0x80;
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
void IMGRZ_Break(UINT32 u4Which)
{
    _IMGRZ_Break(u4Which);
}

//---------------------------------------------------------------------------
//  IMGRZ_Resume
//
/** Resume the process of imgrz of jpg mcu row mode
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Resume(UINT32 u4Which)
{
    _IMGRZ_Resume(u4Which);
}

//---------------------------------------------------------------------------
//  IMGRZ_OFF
//
/** disable clock of imgrz
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_OFF(UINT32 u4Which)
{
    _IMGRZ_OFF(u4Which);
}

//---------------------------------------------------------------------------
//  IMGRZ_RegInfo
//
/** get imgrz register address and size for 
 *  register dump
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_RegInfo(UINT32 u4Which, UINT32 *pu4Addr,UINT32 *pu4Size)
{
  return _IMGRZ_RegInfo(u4Which, pu4Addr,pu4Size);
}


BOOL IMGRZ_CkgenOnOff(UINT32 u4Id, BOOL fgEnable)
{
#define BLOCK_RST_CFG0 0x270
#define IMGRZ0_RST ((UINT32)1 << 3)
#define IMGRZ1_RST ((UINT32)1 << 0)
#define BLOCK_CKEN_CFG0 0x278
#define IMGRZ0_CKEN ((UINT32)1 << 3)
#define IMGRZ1_CKEN ((UINT32)1 << 0)
    if (u4Id == 0)
    {
        if (fgEnable)
        {
            IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) & ~IMGRZ0_RST);
            IO_WRITE32(CKGEN_BASE, BLOCK_CKEN_CFG0, IO_READ32(CKGEN_BASE, BLOCK_CKEN_CFG0) | IMGRZ0_CKEN);
            IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) | IMGRZ0_RST);
        }
        else
        {
            IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) & ~IMGRZ0_RST);
            IO_WRITE32(CKGEN_BASE, BLOCK_CKEN_CFG0, IO_READ32(CKGEN_BASE, BLOCK_CKEN_CFG0) & ~IMGRZ0_CKEN);
        }
    }
    else if (u4Id == 1)
    {
        if (fgEnable)
        {
            IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) & ~IMGRZ1_RST);
            IO_WRITE32(CKGEN_BASE, BLOCK_CKEN_CFG0, IO_READ32(CKGEN_BASE, BLOCK_CKEN_CFG0) | IMGRZ1_CKEN);
            IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) | IMGRZ1_RST);
        }
        else
        {
            IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) & ~IMGRZ1_RST);
            IO_WRITE32(CKGEN_BASE, BLOCK_CKEN_CFG0, IO_READ32(CKGEN_BASE, BLOCK_CKEN_CFG0) & ~IMGRZ1_CKEN);
        }
    }

    UNUSED(fgEnable);
    return TRUE;
}

void IMGRZ_AYUV_ARGB_Conv(UINT32  u4TgBase,UINT32 u4BufLen,UINT32 u4Width, UINT32 u4Height,
    UINT32 u4OffsetX,  UINT32 u4OffsetY, BOOL fgAYUV2ARGB)
{
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    x_memset((void *)&rSclParam, 0, sizeof(RZ_OSD_SCL_PARAM_SET_T));

    // set scaling parameters
    rSclParam.u4SrcBase     =  u4TgBase;
    rSclParam.u4IsSrcDirCM  = 1;
    rSclParam.u4SrcCM       = 3;
    rSclParam.u4SrcBufLen   = u4BufLen;
    rSclParam.u4SrcHOffset  = u4OffsetX;
    rSclParam.u4SrcVOffset  = u4OffsetY;
    rSclParam.u4SrcW        = u4Width;
    rSclParam.u4SrcH        = u4Height;

    rSclParam.u4TgBase      = u4TgBase;
    rSclParam.u4TgCM        = 3;
    rSclParam.u4TgBufLen    = u4BufLen;
    rSclParam.u4TgHOffset   = u4OffsetX;
    rSclParam.u4TgVOffset   = u4OffsetY;
    rSclParam.u4TgW         = u4Width;
    rSclParam.u4TgH         = u4Height;

    rSclParam.u4Csc_enable      = 1;
    rSclParam.csc_src_format      = (E_RZ_CSC_FORMAT_T)(fgAYUV2ARGB);
    rSclParam.csc_des_format      = (E_RZ_CSC_FORMAT_T) (!fgAYUV2ARGB);	
    
    IMGRZ_Lock();
    IMGRZ_ReInit();
    IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_OSDMD);
    IMGRZ_Scale(&rSclParam);
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();

    return;
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
    IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_OSDMD);
    IMGRZ_Scale(&rSclParam);
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();
    return;
}


