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
 * $Author: jiangbo.wang $
 * $Date: 2013/01/24 $
 * $RCSfile: gfx_if.c,v $
 * $Revision: #5 $
 *
 *---------------------------------------------------------------------------*/
 
/** @file gfx_if.c
 *  main interfaces of gfx driver
 *
 */

/*
    lint inhibition(s)
        506 - Constant value Boolean [MTK Rule 6.1.3]
        534 - Ignoring return value of function
        774 - Boolean within 'right side of && within if' always
              evaluates to True [Reference: file gfx_if.c: lines 919, 935]
        826 - Suspicious pointer-to-pointer conversion (area too small)
        831 - Loss of sign (assignment) (signed char to unsigned long)
*/


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------


#include "gfx_if.h"
#include "gfx_drvif.h"
#include "gfx_cmdque.h"
#include "vdp_if.h"

#include "osd_if.h"
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "gfx_common_if.h"


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//#ifndef CC_MTK_LOADER
#define UI_WIDTH    960
#define UI_HEIGHT    540
#define UI_BPP    4
#define OSD_ALIGN_SIZE 64
//#endif


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
/*typedef struct _GFX_DIGEST_INFO_T
{
    UINT8                     *pcBuffer;
    GFX_COLOR_MODE_T       eColormode;
    UINT32                    u4CanvasWidth;
    UINT32                    u4CanvasHeight;
    UINT32                    u4X;
    UINT32                    u4Y;
    UINT32                    u4Width;
    UINT32                    u4Height;
}   GFX_DIGEST_INFO_T;
*/

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
//EXTERN INT32 _OSD_3D_GetOsd2LShiftEn(UINT32 *pu4Value);
//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static INT32 _i4GfxInDMA = (INT32)FALSE;
static void *_pvDMATag;


//#ifndef CC_MTK_LOADER
static UINT8 *puTmpBase = NULL;
//#endif


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


#if defined(CC_MT5360B) || defined(CC_MT5392B)
void GfxSetBurstReadMode(BOOL fgBurstRead)
{
    _GfxSetBurstReadMode(fgBurstRead);
}

BOOL GfxGetBurstReadMode(VOID)
{
    return _GfxGetBurstReadMode();
}
#endif
//---------------------------------------------------------------------------
//  GFX_Init
//
/** Initialize gfx driver.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Init(void)
{
    _GFX_Init();
}


//---------------------------------------------------------------------------
//  GFX_Reset
//
/** Reset engine.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Reset(void)
{
    _GFX_Reset();
}


//---------------------------------------------------------------------------
//  GFX_Wait
//
/** Wait for flushing engine completely.
 *  
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Wait(void)
{
    _GFX_Wait();
}

void GFX_MMU_UPD_PGT(void)
{
    _GFX_MMU_UPD_PGT();
}

//---------------------------------------------------------------------------
//  GFX_Wait_vdp
//
/** Wait for flushing engine completely.
 *  
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Wait_Vdp(void)
{
    _GFX_Wait_Vdp();
}


//---------------------------------------------------------------------------
//  GFX_Wait
//
/** Wait for flushing engine completely.
 *  
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Wait_Timeout(INT32 i4Timeout)
{
    _GFX_Wait_Timeout(i4Timeout);
}



//---------------------------------------------------------------------------
//  GFX_MwFlush
//
/** Flush cmdque and force engine to do operations (for MW use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_MwFlush(void)
{
    _GFX_MwFlush();
}


//---------------------------------------------------------------------------
//  GFX_Flush
//
/** Flush cmdque and force engine to do operations (for Drv use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Flush(void)
{
    _GFX_Flush();
}


//---------------------------------------------------------------------------
//  GFX_DFB_Flush
//
/** Flush cmdque and force engine to do operations (for Drv use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_DFB_Flush(void)
{
    _GFX_DFB_Flush();
}

//---------------------------------------------------------------------------
//	GFX_Flush_Invalid_DCache
//
/** Take the gfx resource.
 *
 *	@retval void
 */
//---------------------------------------------------------------------------
void GFX_Flush_Invalid_DCache(void)
{
    GFX_FlushInvalidateDCacheMultipleLine();
}

//---------------------------------------------------------------------------
//  GFX_Invalid_DCache
//
/** Take the gfx resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Invalid_DCache(void)
{
    GFX_InvalidateDCacheMultipleLine();
}

//---------------------------------------------------------------------------
//  GFX_MMU_Set_Enable_Fg
//
/** Set gfx mmu disable/enable
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_MMU_Set_Cfg(void* pt_gfx_mmu)
{
    _GFX_MMU_Set_Cfg((GFX_MMU_T*) pt_gfx_mmu);
}

void GFX_MMU_Set_Enable(UINT32 u4_enable)
{
	_GFX_MMU_Set_Enable(u4_enable);
}

//---------------------------------------------------------------------------
//  GFX_Lock
//
/** Take the gfx resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Lock(void)
{
    _GFX_Lock();
}

//---------------------------------------------------------------------------
//  GFX_MMU_Set_Enable_Fg
//
/** Set gfx mmu disable/enable
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_MMU_Set_Cmprss(UINT32  u4_enable)
{
    _GFX_Lock();
    _GFX_MMU_Set_Cmprss(u4_enable);
    _GFX_Flush();
    _GFX_Unlock();
}

//---------------------------------------------------------------------------
//  GFX_TryLock
//
/** Try to take the gfx resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_TryLock(void)
{
    _GFX_TryLock();
}


//---------------------------------------------------------------------------
//  GFX_Unlock
//
/** Release the gfx resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Unlock(void)
{
    _GFX_Unlock();
}


//---------------------------------------------------------------------------
//  GFX_LockCmdque
//
/** Take the gfx-cmdq resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_LockCmdque(void)
{
    _GFX_LockCmdque();
}


//---------------------------------------------------------------------------
//  GFX_UnlockCmdque
//
/** Release the gfx-cmdq resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_UnlockCmdque(void)
{
    _GFX_UnlockCmdque();
}


//---------------------------------------------------------------------------
//  GFX_QueryHwIdle
//
/** Query engine status.
 *
 *  @retval 1   Idle
 *  @retval 0   Busy
 */
//---------------------------------------------------------------------------
INT32 GFX_QueryHwIdle(void)
{
    return _GFX_QueryHwIdle();
}


//---------------------------------------------------------------------------
//  GFX_SetNotify
//
/** Set notify function (for MW use).
 *
 *  @param  pfnNotifyFunc   The callback function pointer
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetNotify(void (*pfnNotifyFunc)(UINT32))
{
    _GFX_SetNotify(pfnNotifyFunc);
}


//---------------------------------------------------------------------------
//  GFX_SetDst
//
/** Set destination region.
 *
 *  @param  pu1Base     The base address of the dst region
 *  @param  u4ColorMode The color mode of the dst region
 *  @param  u4Pitch     The pitch of the dst region
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetDst(UINT8 *pu1Base, UINT32 u4ColorMode, UINT32 u4Pitch)
{
    _GFX_SetDst(pu1Base, u4ColorMode, u4Pitch);
}


//---------------------------------------------------------------------------
//  GFX_SetSrc
//
/** Set source region.
 *
 *  @param  pu1Base     The base address of the src region
 *  @param  u4ColorMode The color mode of the src region
 *  @param  u4Pitch     The pitch of the src region
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetSrc(UINT8 *pu1Base, UINT32 u4ColorMode, UINT32 u4Pitch)
{
    _GFX_SetSrc(pu1Base, u4ColorMode, u4Pitch);
}


//---------------------------------------------------------------------------
//  GFX_SetAlpha
//
/** Set global alpha.
 *
 *  @param  u4Alpha     The global alpha
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetAlpha(UINT32 u4Alpha)
{
    _GFX_SetAlpha(u4Alpha);
}

#if defined(CC_MT5389)||defined(CC_MT5398)||defined(CC_MT5880)

void GFX_SetMaxAlphaEn(UINT32 u4Enable)
{
    _GFX_SetMaxAlphaEn( u4Enable);
}

void GFX_SetRGB0En(UINT32 u4Enable)
{
    _GFX_SetRGB0En(u4Enable);
}

#endif

#if defined(CC_MT5398)||defined(CC_MT5880)
//---------------------------------------------------------------------------
//  GFX_SetXORColor
//
/** Set XOR color.
 *
 *  @param  u4Color     The rectangular color
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetXORColor(UINT32 u4Color)
{
    _GFX_SetXORColor(u4Color);
}
//---------------------------------------------------------------------------
//  GFX_Set_PRE_COLORIZE
//
/** Set PRE_COLORIZE
 *
 *  @param  u4PreColorize     The value of PRE_COLORIZE  [BOOL]
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Set_PRE_COLORIZE(BOOL u4PreColorize)
{
    _GFX_Set_PRE_COLORIZE(u4PreColorize);
}
//---------------------------------------------------------------------------
//  _GFX_Set_FastBitBlt
//
/** Set FastBitBlt MODE
 *
 *  @param  u4FastBitBlt     The value of FastBitBlt  0x3 enalbe
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Set_FastBitBlt(UINT32 u4FastBitBlt)
{
	_GFX_Set_FastBitBlt(u4FastBitBlt);
}
//---------------------------------------------------------------------------
//  _GFX_Set_SrcAlReplace
//
/** Set SrcAlReplace
 *
 *  @param  u4SrcAlRep     The value of SrcAlReplace  [BOOL]
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Set_SrcAlReplace(UINT32 u4SrcAlRep)
{
    _GFX_Set_SrcAlReplace(u4SrcAlRep);
}
//---------------------------------------------------------------------------
//  _GFX_Set_UseRectSrc
//
/** Set UseRectSrc
 *
 *  @param  u4UseRectSrc     The value of UseRectSrc  [BOOL]
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Set_UseRectSrc(UINT32 u4UseRectSrc)
{
    _GFX_Set_UseRectSrc(u4UseRectSrc);
}

//---------------------------------------------------------------------------
//  GFX_Set_FlashColCnvInfo
//
/** Set FlashColCnvInfo
 *
 *  @param  pu1SrcY     The value of src Y Adress  
 *  @param  u4SrcYPitch     The number of src Y bytes  
 *  @param  pu1SrcCbCr     The value of src CbCr Adress  
 *  @param  u4SrcCbCrPitch     The number of src CbCr bytes  
 *  @param  u4DstAdress     The value of u4DstAdress  
 *  @param  pu1SrcY     The number of src Y bytes  
 *  @param  u4SrcWidth     The value of src Y Width   
 *  @param  u4SrcHeight     The value of src Y Width   
 *  @retval void
 */
//---------------------------------------------------------------------------

void GFX_Set_FlashColCnvInfo(UINT8 *pu1SrcY, UINT32 u4SrcYPitch, UINT8 *pu1SrcCbCr, 
	UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,UINT32 u4SrcWidth,UINT32 u4SrcHeight)
{
 	_GFX_Set_FlashColCnvInfo(pu1SrcY, u4SrcYPitch,pu1SrcCbCr, 
		u4SrcCbCrPitch,u4DstAdress,u4SrcWidth,u4SrcHeight);
}

//---------------------------------------------------------------------------
//  GFX_YCbCr420ToARGB32
//
/** Convert YCbCr to ARGB8888
 *
 *  @param  u4AlphaValue     The value of Alpha  
 *  @param  u4FieldPic     The choose of field  picture  ( 0 frame pic / 1 field pic )
 */
//---------------------------------------------------------------------------

void GFX_YCbCr420ToARGB32(UINT8 *pu1SrcY, UINT32 u4SrcYPitch, UINT8 *pu1SrcCbCr, 
	UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,UINT32 u4SrcWidth,UINT32 u4SrcHeight,UINT32 u4AlphaValue,UINT32 u4FieldPic)
{
	GFX_Set_FlashColCnvInfo(pu1SrcY,u4SrcYPitch,pu1SrcCbCr,u4SrcCbCrPitch,u4DstAdress,u4SrcWidth,u4SrcHeight);
	_GFX_Set_YCbCr420ToARGB32(u4AlphaValue,u4FieldPic);
	_GFX_Flush();
	_GFX_Set_WriteNoPitch(FALSE);
}

//---------------------------------------------------------------------------
//  GFX_YCbCrBlockToRaster_2Buf
//
/** make YCbCr to Y / CbCr
 *
 *  @param  pu1SrcY     The value of src Y Adress  
 *  @param  u4SrcYPitch     The number of src Y bytes  
 *  @param  pu1SrcCbCr     The value of src CbCr Adress  
 *  @param  u4SrcCbCrPitch     The number of src CbCr bytes  
 *  @param  u4DstAdress     The value of u4DstAdress  
 *  @param  pu1SrcY     The number of src Y bytes  
 *  @param  u4SrcWidth     The value of src Y Width   
 *  @param  u4SrcHeight     The value of src Y Width   
 *  @retval void
 */
//---------------------------------------------------------------------------

void GFX_YCbCrBlockToRaster_2Buf(UINT8 *pu1SrcY, UINT32 u4SrcYPitch, UINT8 *pu1SrcCbCr, 
	UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,UINT32 u4SrcWidth,UINT32 u4SrcHeight)
{	
	UINT8 *puTempY;
	UINT8 *puTempWrite;
	//pass1
	GFX_Set_FlashColCnvInfo(pu1SrcY,u4SrcYPitch,pu1SrcCbCr,u4SrcCbCrPitch,u4DstAdress,u4SrcWidth,u4SrcHeight);
	_GFX_Set_YCbCrBlockToRaster(OP_YBLOCK_TO_YRASTER);
	_GFX_Flush();

	//pass2
	puTempY = u4DstAdress + u4SrcWidth*u4SrcHeight*5/4;
	puTempWrite = u4DstAdress + u4SrcWidth*u4SrcHeight;
	GFX_Set_FlashColCnvInfo(puTempY,u4SrcYPitch,pu1SrcCbCr,u4SrcCbCrPitch,puTempWrite,u4SrcWidth,u4SrcHeight);
	_GFX_Set_YCbCrBlockToRaster(OP_CBCRBLOCK_TO_1CBCRRASTER);
	_GFX_Flush();

	_GFX_Set_WriteNoPitch(FALSE);
}

//---------------------------------------------------------------------------
//  GFX_YCbCrBlockToRaster_3Buf
//
/** make YCbCr to Y / Cb / Cr
 *
 *  @param  pu1SrcY     The value of src Y Adress  
 *  @param  u4SrcYPitch     The number of src Y bytes  
 *  @param  pu1SrcCbCr     The value of src CbCr Adress  
 *  @param  u4SrcCbCrPitch     The number of src CbCr bytes  
 *  @param  u4DstAdress     The value of u4DstAdress  
 *  @param  pu1SrcY     The number of src Y bytes  
 *  @param  u4SrcWidth     The value of src Y Width   
 *  @param  u4SrcHeight     The value of src Y Width   
 *  @retval void
 */
//---------------------------------------------------------------------------

void GFX_YCbCrBlockToRaster_3Buf(UINT8 *pu1SrcY, UINT32 u4SrcYPitch, UINT8 *pu1SrcCbCr, 
	UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,UINT32 u4SrcWidth,UINT32 u4SrcHeight)
{	
	UINT8 *puTempY;
	UINT8 *puTempWrite;
	//pass1
	GFX_Set_FlashColCnvInfo(pu1SrcY,u4SrcYPitch,pu1SrcCbCr,u4SrcCbCrPitch,u4DstAdress,u4SrcWidth,u4SrcHeight);
	_GFX_Set_YCbCrBlockToRaster(OP_YBLOCK_TO_YRASTER);
	_GFX_Flush();

	//pass2
	puTempY = u4DstAdress + u4SrcWidth*u4SrcHeight*5/4;
	puTempWrite = u4DstAdress + u4SrcWidth*u4SrcHeight;
	GFX_Set_FlashColCnvInfo(puTempY,u4SrcYPitch,pu1SrcCbCr,u4SrcCbCrPitch,puTempWrite,u4SrcWidth,u4SrcHeight);
	_GFX_Set_YCbCrBlockToRaster(OP_CBCRBLOCK_TO_2CBCRRASTER);
	_GFX_Flush();

	_GFX_Set_WriteNoPitch(FALSE);
}

#endif

//---------------------------------------------------------------------------
//  GFX_SetColor
//
/** Set rectangular color.
 *
 *  @param  u4Color     The rectangular color
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetColor(UINT32 u4Color)
{
    _GFX_SetColor(u4Color);
}


//---------------------------------------------------------------------------
//  GFX_Fill
//
/** Fill rectangle with rectangular color.
 *
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the rectangle
 *  @param  u4Height    The height of the rectangle
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Fill(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, 
    UINT32 u4Height)
{
    _GFX_Fill(u4DstX, u4DstY, u4Width, u4Height);
}


//---------------------------------------------------------------------------
//  GFX_HLine
//
/** Draw horizontal line.
 *
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the line
 * 
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_HLine(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width)
{
    _GFX_HLine(u4DstX, u4DstY, u4Width);
}


//---------------------------------------------------------------------------
//  GFX_VLine
//
/** Draw vertical line.
 *
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Height    The height of the line
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_VLine(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Height)
{
    _GFX_VLine(u4DstX, u4DstY, u4Height);
}


//---------------------------------------------------------------------------
//  GFX_SetBltOpt
//
/** Set bitblt option.
 *
 *  @param  u4Switch    The option flag of BitBlt
 *  @param  u4ColorMin  The dst color key min
 *  @param  u4ColorMax  The dst color key max
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetBltOpt(UINT32 u4Switch, UINT32 u4ColorMin, UINT32 u4ColorMax)
{
    _GFX_SetBltOpt(u4Switch, u4ColorMin, u4ColorMax);
}


//---------------------------------------------------------------------------
//  GFX_BitBlt
//
/** Bit blit operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_BitBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, UINT32 u4DstY, 
    UINT32 u4Width, UINT32 u4Height)
{
    _GFX_BitBlt(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height);
}


//---------------------------------------------------------------------------
//  GFX_Blend
//
/** Alpha blending operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Blend(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, UINT32 u4DstY, 
    UINT32 u4Width, UINT32 u4Height)
{
    _GFX_Blend(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height);
}


//---------------------------------------------------------------------------
//  GFX_AlphaComposePass
//
/** Alpha compose pass operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *  @param  u4Pass      The pass method
 *  @param  u4Param     The global alpha
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_AlphaComposePass(UINT32 u4SrcX, UINT32 u4SrcY, 
    UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, 
    UINT32 u4Pass, UINT32 u4Param)
{
    _GFX_AlphaComposePass(u4SrcX, u4SrcY, u4DstX, u4DstY, 
        u4Width, u4Height, u4Pass, u4Param);
}


//---------------------------------------------------------------------------
//  GFX_SetColCnvFmt
//
/** Set ycbcr to rgb color conversion format.
 *
 *  @param  u4YCFmt     The yc format
 *  @param  u4SwapMode  The swap mode
 *  @param  u4VidStd    The video standard
 *  @param  u4VidSys    The video system
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetColCnvFmt(UINT32 u4YCFmt, UINT32 u4SwapMode, UINT32 u4VidStd, 
    UINT32 u4VidSys)
{
    _GFX_SetColCnvFmt(u4YCFmt, u4SwapMode, u4VidStd, u4VidSys);
}

#ifdef GFX_CC_YC
// 64*32 --> ARGB8888
BOOL GFX_ColorConvert_YC(void* pvDstInfo,UINT32 u4SrcAddrY,UINT32 u4SrcAddrC, UINT32 u4DstCM, UINT8 u1Alpha)
{
    VDP_THUMBNAIL_INFO_T *pDst = (VDP_THUMBNAIL_INFO_T*)pvDstInfo;
    
    GFX_COLOR_CONVERT_T rGfxColorConvert;
    UINT32 u4CbCrAddrAlign = (((pDst->u4ThumbnailWidth) +15) &(~15));
    UINT32 u4CbCrAddr = u4SrcAddrC;
    
    UINT32 u4DstPitch =0;
    //UINT32 u4DstCM = (UINT32)((UINT32)(pDst->eCanvasColormode) +2);

    x_memset(&rGfxColorConvert,0,sizeof(GFX_COLOR_CONVERT_T));
        
    u4DstPitch = pDst->u4CanvasPitch;
	
    rGfxColorConvert.t_mmu.u4_enable = 0x0;
	
    rGfxColorConvert.pu1CbCr = (UINT8*)u4CbCrAddr;
    rGfxColorConvert.pu1DstAddr = pDst->pucCanvasBuffer;
    rGfxColorConvert.pu1SrcAddr = (UINT8*)u4SrcAddrY;
    rGfxColorConvert.u1Alpha = u1Alpha;
    rGfxColorConvert.u4DstCM = u4DstCM;
    rGfxColorConvert.u4DstPitch = u4DstPitch>>2;
    rGfxColorConvert.u4DstX = pDst->u4ThumbnailX;
    rGfxColorConvert.u4DstY = pDst->u4ThumbnailY;
    rGfxColorConvert.u4Height = pDst->u4ThumbnailHeight;
    rGfxColorConvert.u4Width = u4CbCrAddrAlign;
    
    return _GFX_ColorConvert_YC(&rGfxColorConvert);
}
#else
BOOL GFX_ColorConvert_YC(void* pvDstInfo,UINT32 u4SrcAddrY,UINT32 u4SrcAddrC, UINT32 u4DstCM, UINT8 u1Alpha)
{
    return FALSE;
}
#endif

//---------------------------------------------------------------------------
//  GFX_SetColCnvSrc
//
/** Set color conversion source region.
 *
 *  @param  pu1SrcLuma          The base address of the luma region
 *  @param  u4SrcLumaPitch      The pitch of the luma region
 *  @param  pu1SrcChroma        The base address of the chroma region
 *  @param  u4SrcChromaPitch    The pitch of the chroma region
 *  @param  u4FieldPic          The field mode
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetColCnvSrc(UINT8 *pu1SrcLuma, UINT32 u4SrcLumaPitch, 
    UINT8 *pu1SrcChroma, UINT32 u4SrcChromaPitch, UINT32 u4FieldPic)
{
    _GFX_SetColCnvSrc(pu1SrcLuma, u4SrcLumaPitch, 
        pu1SrcChroma, u4SrcChromaPitch, u4FieldPic);
}


//---------------------------------------------------------------------------
//  GFX_ColConv
//
/** Convert ycbcr to rgb.
 *
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_ColConv(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, 
    UINT32 u4Height)
{
    _GFX_ColConv(u4DstX, u4DstY, u4Width, u4Height);
}

#if defined(CC_MT5368)||defined(CC_MT5396)|| defined(CC_MT5389)||defined(CC_MT5398)||defined(CC_MT5880)
void GFX_SetDstWrRgb24Bit(UINT32 u4Enable)
{
    _GFX_SetDstWrRgb24Bit(u4Enable);
}
#endif

//---------------------------------------------------------------------------
//  GFX_StretchBlt
//
/** Stretch bitblt (repeat or drop).
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4SrcW      The width of the src extent
 *  @param  u4SrcH      The height of the src extent
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4DstW      The width of the dst extent
 *  @param  u4DstH      The height of the dst extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_StretchBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW, 
    UINT32 u4SrcH, UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW, UINT32 u4DstH)
{
    _GFX_StretchBlt(u4SrcX, u4SrcY, u4SrcW, u4SrcH, 
                    u4DstX, u4DstY, u4DstW, u4DstH);
}


//---------------------------------------------------------------------------
//  GFX_AlphaMapBitBlt
//
/** Alpha-map bitblt operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_AlphaMapBitBlt(UINT32 u4SrcX, UINT32 u4SrcY, 
    UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height)
{
    _GFX_AlphaMapBitBlt(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height);
}


//---------------------------------------------------------------------------
//  GFX_SetRopOpt
//
/** Set rop bitblt option.
 *
 *  @param  u4RopCode       The rop code
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetRopOpt(UINT32 u4RopCode)
{
    _GFX_SetRopOpt(u4RopCode);
}


//---------------------------------------------------------------------------
//  GFX_RopBitBlt
//
/** Rop bitblt operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_RopBitBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, UINT32 u4DstY, 
    UINT32 u4Width, UINT32 u4Height)
{
    _GFX_RopBitBlt(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height);
}


//---------------------------------------------------------------------------
//  GFX_ComposeLoop
//
/** Loop mode alpha composition.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *  @param  u4Ar        The global alpha
 *  @param  u4OpCode    The PD rules
 *  @param  u4RectSrc   The flag of src extent is filled rectangular color
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_ComposeLoop(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar, 
    UINT32 u4OpCode, UINT32 u4RectSrc)
{
    _GFX_ComposeLoop(u4SrcX, u4SrcY, u4DstX, u4DstY,
        u4Width, u4Height, u4Ar, u4OpCode, u4RectSrc);
}


#ifdef CC_GFX_COMPOSITION_IMPROVE
void GFX_ComposeLoopImprove(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar, 
    UINT32 u4OpCode, UINT32 u4RectSrc)
{
    _GFX_ComposeLoopImprove(u4SrcX, u4SrcY, u4DstX, u4DstY,
        u4Width, u4Height, u4Ar, u4OpCode, u4RectSrc);
}
#endif

//---------------------------------------------------------------------------
//  GFX_SetIdx2DirSrc
//
/** Set index to direct color bitblt source region.
 *
 *  @param  pu1Base     The base address of the src region
 *  @param  u4SrcCM     The color mode of the src region
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetIdx2DirSrc(UINT8 *pu1Base, UINT32 u4SrcCM)
{
    _GFX_SetIdx2DirSrc(pu1Base, u4SrcCM);
}


//---------------------------------------------------------------------------
//  GFX_SetIdx2DirOpt
//
/** Set index to direct color bitblt option.
 *
 *  @param  pu1PaletteBase      The base address of the palette
 *  @param  u4MsbLeft           The flag of left pixel location in one byte
 *  @param  u4StartByteAligned  The flag of src line start byte aligned
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetIdx2DirOpt(UINT8 *pu1PaletteBase, UINT32 u4MsbLeft, 
    UINT32 u4StartByteAligned)
{
    _GFX_SetIdx2DirOpt(pu1PaletteBase, u4MsbLeft, u4StartByteAligned);
}


//---------------------------------------------------------------------------
//  GFX_Idx2DirBitBlt
//
/** index to direct color bitblt operation.
 *
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Idx2DirBitBlt(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, 
    UINT32 u4Height)
{
    _GFX_Idx2DirBitBlt(u4DstX, u4DstY, u4Width, u4Height);
}


//---------------------------------------------------------------------------
//  GFX_2DMemCompare
//
/** 2D memory compare operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_2DMemCompare(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height)
{
    _GFX_2DMemCompare(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height);
}


//---------------------------------------------------------------------------
//  GFX_GetMemCompareResult
//
/** Get memory compare result.
 *
 *  @retval 1   With difference
 *  @retval 0   No difference
 */
//---------------------------------------------------------------------------
INT32 GFX_GetMemCompareResult(void)
{
    return _GFX_GetMemCompareResult();
}


//---------------------------------------------------------------------------
//  GFX_SetHoriToVertLineOpt
//
/** Set horizontal line to vertical line option.
 *
 *  @param  u4Is90dCcw      The flag of 90" clockwise or counterclockwise
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetHoriToVertLineOpt(UINT32 u4Is90dCcw)
{
    _GFX_SetHoriToVertLineOpt(u4Is90dCcw);
}


//---------------------------------------------------------------------------
//  GFX_HoriToVertLine
//
/** Horizontal line to vertical line operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the horizontal line
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_HoriToVertLine(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4HoriLineWidth)
{
    _GFX_HoriToVertLine(u4SrcX, u4SrcY, u4DstX, u4DstY, u4HoriLineWidth);
}

#ifdef CC_GFX_COMPOSITION_IMPROVE
void GFX_SetCharCM(BOOL flag,UINT32 u4SrcCM)
{
   _GFX_SetCharCM(flag, u4SrcCM);
}
#endif

//---------------------------------------------------------------------------
//  GFX_SetExtCmdQue
//
/** Horizontal line to vertical line operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetExtCmdQue(volatile GFX_CMDQUE_T  *prGfxCmdQue,
                     volatile UINT64       *pu8CmdQueBuf, UINT32 u4Config)
{
    GFX_CmdQueSetExtBuf(prGfxCmdQue, pu8CmdQueBuf, u4Config);
}

#if 1
BOOL GFX_ColorConvert(void* pvDstInfo, UINT32 u4SrcAddr, UINT32 u4DstCM, UINT8 u1Alpha)
{
    VDP_THUMBNAIL_INFO_T *pDst = (VDP_THUMBNAIL_INFO_T*)pvDstInfo;
    
    GFX_COLOR_CONVERT_T rGfxColorConvert;
    UINT32 u4CbCrAddrAlign = (((pDst->u4ThumbnailWidth) +15) &(~15));
    UINT32 u4CbCrAddr = u4SrcAddr + ((pDst->u4ThumbnailHeight) * u4CbCrAddrAlign);
    
    UINT32 u4DstPitch =0;

    x_memset(&rGfxColorConvert,0,sizeof(GFX_COLOR_CONVERT_T));
    
    u4DstPitch = pDst->u4CanvasPitch;
    rGfxColorConvert.t_mmu.u4_enable = 0x0;
    rGfxColorConvert.pu1CbCr = (UINT8*)u4CbCrAddr;
    rGfxColorConvert.pu1DstAddr = pDst->pucCanvasBuffer;
    rGfxColorConvert.pu1SrcAddr = (UINT8*)u4SrcAddr;
    rGfxColorConvert.u1Alpha = u1Alpha;
    rGfxColorConvert.u4DstCM = u4DstCM;
    rGfxColorConvert.u4DstPitch = u4DstPitch;
    rGfxColorConvert.u4DstX = pDst->u4ThumbnailX;
    rGfxColorConvert.u4DstY = pDst->u4ThumbnailY;
    rGfxColorConvert.u4Height = pDst->u4ThumbnailHeight;
    rGfxColorConvert.u4Width = u4CbCrAddrAlign;
    return _GFX_ColorConvert(&rGfxColorConvert);
}
#endif



void GFX_NewCompressBlt(UINT8 *pu1SrcBase, UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcCM, 
    UINT32 u4SrcPitch, UINT8 *pu1DstBase, UINT32 u4Width, UINT32 u4Height, UINT32 u4BufHeight,
    BOOL u4RollBackEn, UINT32 u4QualityMode, UINT32 u4BpCompAddrEnd,BOOL fgMirrorFlip,BOOL fgBurstRead,BOOL fgLineSeparate)
{
	#ifdef CC_MT8283_OSD_COMPRESS
	UINT32 original_line_bit = u4Width *4*8; 
	#else
	UINT32 original_line_bit = u4SrcPitch *8;   
	#endif
    UINT32 line_bit,u4DstPitch;
	BOOL fgOSD2PrLshift_ENABLE = FALSE;

       if(fgLineSeparate )
       {
       
       if(u4RollBackEn)
           line_bit = original_line_bit+4;
       else
           line_bit = original_line_bit + 4 + original_line_bit/16;

       if(fgBurstRead)
           line_bit = (line_bit%512 ==0)? line_bit : (line_bit/512 +1)*512;
       else
           line_bit = (line_bit%128 ==0)? line_bit : (line_bit/128 +1)*128;


                u4DstPitch = line_bit/8;
       }
       else
       {
       	u4DstPitch = original_line_bit/8;
       }
	   #ifdef CC_3D_OSD_SUPPORT
	   fgOSD2PrLshift_ENABLE = OSD_BASE_Get_3DOSD2PLsEn();
	   if(fgOSD2PrLshift_ENABLE)
	   {
		   if (puTmpBase == 0)
		   {
			   puTmpBase =
		#ifdef CC_TDTV_3D_OSD_ENABLE
				   (UINT8 *)_mt53fb_get_tmpbufaddr();
		#else
				   (UINT8 *)BSP_AllocAlignedDmaMemory((UI_WIDTH*UI_HEIGHT*UI_BPP), (OSD_ALIGN_SIZE));
		#endif	
			   Printf("TmpBuf = 0x%x\n", puTmpBase);   
			   if(!puTmpBase)
			   {
				   printf("[osd/gfx Error] cannot alloc memory\n");
				   return;
			   }
		   }
	   
		   GFX_SetSrc((UINT8 *) pu1SrcBase,u4SrcCM,u4SrcPitch);
		   GFX_SetDst((UINT8 *) puTmpBase,u4SrcCM,u4SrcPitch);
		   GFX_SetBltOpt(0,0,0);
	   
		   GFX_StretchBlt(0,0,u4Width,u4Height,0,0,u4Width,u4Height>>1);
		   GFX_Flush();
		   GFX_Wait();
		   pu1SrcBase = puTmpBase;
	   
	   }
       #endif
 
    _GFX_NewCompressBlt(pu1SrcBase,u4SrcX,u4SrcY,u4SrcCM,u4SrcPitch,pu1DstBase,u4Width,u4Height,u4BufHeight,
       u4RollBackEn,u4QualityMode,u4BpCompAddrEnd,fgMirrorFlip,fgBurstRead,fgLineSeparate,u4DstPitch);
}


void GFX_StretchAlphaCom(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW, 
    UINT32 u4SrcH, UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW, UINT32 u4DstH, 
    UINT32 u4Ar, UINT32 u4OpCode)
{
	_GFX_StretchAlphaCom(u4SrcX, u4SrcY, u4SrcW, u4SrcH, u4DstX, u4DstY, u4DstW, u4DstH, u4Ar, u4OpCode);
}


void GFX_SetPremult(UINT32 u4PremultSrcR, UINT32 u4PremultDstR, UINT32 u4PremultDstW, UINT32 u4PremultOvrflw)
{
	_GFX_SetPremult(u4PremultSrcR, u4PremultDstR, u4PremultDstW, u4PremultOvrflw);
}
void GFX_SetWriteArgbOrder(UINT32 u4Aorder, UINT32 u4Rorder, UINT32 u4Gorder, UINT32 u4Border)
{
	_GFX_SetWriteArgbOrder(u4Aorder,u4Rorder,u4Gorder,u4Border);
}

void GFX_GeneralAlphaCom(UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar, 
    UINT32 u4OpCode, UINT32 u4RectSrc,UINT32 u4ThirdEnable,BOOL fgAlcomNormal,BOOL fgOnePassCompress)
{
	_GFX_GeneralAlphaCom(u4Width,u4Height,u4Ar,u4OpCode,u4RectSrc,u4ThirdEnable,fgAlcomNormal,fgOnePassCompress);
}


void (*pfnGFX_DMANotify)(UINT32, void*);


void GFX_SetDMANotify(void (*pfnNotifyFunc)(UINT32, void*))
{
    pfnGFX_DMANotify = pfnNotifyFunc;
}


void GFX_SetDMAStatus(INT32 i4Status)
{
    _i4GfxInDMA = i4Status;
}


INT32 GFX_IsDMA(void)
{
    return _i4GfxInDMA;
}


void GFX_SetDMATag(void *pvTag)
{
	_pvDMATag = pvTag;

}


void* GFX_GetDMATag(void)
{
	return _pvDMATag;
}

UINT32 GFX_Get_Cm_Size(GFX_COLOR_MODE_T e_cm)
{
    UINT32  ui4_size=0x4;
    
    switch(e_cm)
    {
        case CM_YCbCr_CLUT8:
        case CM_RGB_CLUT8:
            ui4_size=0x1;
            break;
        case CM_CbYCrY422_DIRECT16:
        case CM_YCbYCr422_DIRECT16:
        case CM_RGB565_DIRECT16:
        case CM_ARGB1555_DIRECT16:
        case CM_ARGB4444_DIRECT16:
            ui4_size=0x2;
            break;
        case CM_AYCbCr8888_DIRECT32:
        case CM_ARGB8888_DIRECT32:
            ui4_size=0x4;
            break;
        default:
            break;
    }

    return ui4_size;
}


