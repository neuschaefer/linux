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
 * $Author: cheng.chen $
 * $Date: 2012/07/06 $
 * $RCSfile: gfx_if.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/
 
/** @file gfx_if.h
 *  gfx driver main interfaces
 *  
 */

#ifndef GFX_IF_H
#define GFX_IF_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------


/*lint -save -e961 */
#include "x_common.h"
#include "x_typedef.h"
#include "x_timer.h"
#include "x_assert.h"
#include "x_os.h"
//#include "x_bim.h"
#include "x_clksrc.h"
#include "x_hal_5381.h"
#include "x_hal_926.h"
/*lint -restore */
#include "vgfx_drvif.h"

#include "gfx_common_if.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  GFX_Init
//
/** Initialize gfx driver.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_Init(void);


//---------------------------------------------------------------------------
//  GFX_Reset
//
/** Reset engine.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_Reset(void);


//---------------------------------------------------------------------------
//  GFX_Wait
//
/** Wait for flushing engine completely.
 *  
 *  @retval void
 */
//---------------------------------------------------------------------------

extern void GFX_Wait(void);

extern void GFX_Wait_Vdp(void);


extern void GFX_Wait_Timeout(INT32 i4Timeout);


//---------------------------------------------------------------------------
//  GFX_MwFlush
//
/** Flush cmdque and force engine to do operations (for MW use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_MwFlush(void);


//---------------------------------------------------------------------------
//  GFX_Flush
//
/** Flush cmdque and force engine to do operations (for Drv use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_Flush(void);

extern void GFX_DFB_Flush(void);



//---------------------------------------------------------------------------
//  GFX_Lock
//
/** Take the gfx resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_Lock(void);


//---------------------------------------------------------------------------
//  GFX_TryLock
//
/** Try to take the gfx resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_TryLock(void);


//---------------------------------------------------------------------------
//  GFX_Unlock
//
/** Release the gfx resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_Unlock(void);


//---------------------------------------------------------------------------
//  GFX_LockCmdque
//
/** Take the gfx-cmdq resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_LockCmdque(void);


//---------------------------------------------------------------------------
//  GFX_UnlockCmdque
//
/** Release the gfx-cmdq resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_UnlockCmdque(void);


//---------------------------------------------------------------------------
//  GFX_QueryHwIdle
//
/** Query engine status.
 *
 *  @retval 1   Idle
 *  @retval 0   Busy
 */
//---------------------------------------------------------------------------
extern INT32 GFX_QueryHwIdle(void);


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
extern void GFX_SetNotify(void (*pfnNotifyFunc)(UINT32));


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
extern void GFX_SetDst(UINT8 *pu1Base, UINT32 u4ColorMode, UINT32 u4Pitch);


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
extern void GFX_SetSrc(UINT8 *pu1Base, UINT32 u4ColorMode, UINT32 u4Pitch);


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
extern void GFX_SetAlpha(UINT32 u4Alpha);

#if defined(CC_MT5389)||defined(CC_MT5398)||defined(CC_MT5880)

extern void GFX_SetMaxAlphaEn(UINT32 u4Enable);

extern void GFX_SetRGB0En(UINT32 u4Enable);

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
extern void GFX_SetColor(UINT32 u4Color);


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
extern void GFX_Fill(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, 
    UINT32 u4Height);


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
extern void GFX_HLine(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width);


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
extern void GFX_VLine(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Height);


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
extern void GFX_SetBltOpt(UINT32 u4Switch, UINT32 u4ColorMin, 
    UINT32 u4ColorMax);


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
extern void GFX_BitBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height);


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
extern void GFX_Blend(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height);


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
extern void GFX_AlphaComposePass(UINT32 u4SrcX, UINT32 u4SrcY, 
    UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, 
    UINT32 u4Pass, UINT32 u4Param);


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
extern void GFX_SetColCnvFmt(UINT32 u4YCFmt, UINT32 u4SwapMode, 
    UINT32 u4VidStd, UINT32 u4VidSys);


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
extern void GFX_SetColCnvSrc(UINT8 *pu1SrcLuma, UINT32 u4SrcLumaPitch, 
    UINT8 *pu1SrcChroma, UINT32 u4SrcChromaPitch, UINT32 u4FieldPic);


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
extern void GFX_ColConv(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, 
    UINT32 u4Height);



#if defined(CC_MT5368)||defined(CC_MT5396)|| defined(CC_MT5389)||defined(CC_MT5398)||defined(CC_MT5880)
extern void GFX_SetDstWrRgb24Bit(UINT32 u4Dst);
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
extern void GFX_StretchBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW, 
    UINT32 u4SrcH, UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW, UINT32 u4DstH);


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
extern void GFX_AlphaMapBitBlt(UINT32 u4SrcX, UINT32 u4SrcY, 
    UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height);


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
extern void GFX_SetRopOpt(UINT32 u4RopCode);


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
extern void GFX_RopBitBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height);


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
extern void GFX_ComposeLoop(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar, 
    UINT32 u4OpCode, UINT32 u4RectSrc);


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
extern void GFX_SetIdx2DirSrc(UINT8 *pu1Base, UINT32 u4SrcCM);


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
extern void GFX_SetIdx2DirOpt(UINT8 *pu1PaletteBase, UINT32 u4MsbLeft, 
    UINT32 u4StartByteAligned);


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
extern void GFX_Idx2DirBitBlt(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, 
    UINT32 u4Height);


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
extern void GFX_2DMemCompare(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height);


//---------------------------------------------------------------------------
//  GFX_GetMemCompareResult
//
/** Get memory compare result.
 *
 *  @retval 1   With difference
 *  @retval 0   No difference
 */
//---------------------------------------------------------------------------
extern INT32 GFX_GetMemCompareResult(void);



#ifdef CC_GFX_COMPOSITION_IMPROVE
extern void GFX_SetCharCM(BOOL flag,UINT32 u4SrcCM);
extern void GFX_ComposeLoopImprove(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar, 
    UINT32 u4OpCode, UINT32 u4RectSrc);

#endif


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
extern void GFX_SetHoriToVertLineOpt(UINT32 u4Is90dCcw);


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
extern void GFX_HoriToVertLine(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4HoriLineWidth);


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
extern void GFX_SetExtCmdQue(volatile GFX_CMDQUE_T  *prGfxCmdQue,
                     volatile UINT64       *pu8CmdQueBuf, UINT32 u4Config);

extern void GFX_SetDirectFBMem(UINT32 u4Addr, UINT32 u4Size);

extern void GfxSetBurstReadMode(BOOL fgBurstRead);
extern BOOL GfxGetBurstReadMode(VOID);


extern BOOL GFX_ColorConvert(void* pvDstInfo, UINT32 u4SrcAddr, UINT32 u4DstCM, UINT8 u1Alpha);
extern void GFX_NewCompressBlt(UINT8 *pu1SrcBase, UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcCM, 
    UINT32 u4SrcPitch, UINT8 *pu1DstBase, UINT32 u4Width, UINT32 u4Height, UINT32 u4BufHeight,
    BOOL u4RollBackEn, UINT32 u4QualityMode, UINT32 u4BpCompAddrEnd,BOOL fgMirrorFlip,BOOL fgBurstRead,BOOL fgLineSeparate);

extern void GFX_StretchAlphaCom(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW, 
    UINT32 u4SrcH, UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW, UINT32 u4DstH, 
    UINT32 u4Ar, UINT32 u4OpCode);

extern void GFX_SetPremult(UINT32 u4PremultSrcR, UINT32 u4PremultDstR, UINT32 u4PremultDstW, UINT32 u4PremultOvrflw);
	
extern void GFX_GeneralAlphaCom(UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar, 
    UINT32 u4OpCode, UINT32 u4RectSrc,UINT32 u4ThirdEnable,BOOL fgAlcomNormal,BOOL fgOnePassCompress);
extern void (*pfnGFX_DMANotify)(UINT32, void*);

extern void GFX_SetDMANotify(void (*pfnNotifyFunc)(UINT32, void*));

extern void GFX_SetDMAStatus(INT32 i4Status);

extern INT32 GFX_IsDMA(void);

extern void GFX_SetDMATag(void *pvTag);

extern void* GFX_GetDMATag(void);

extern void GFX_MMU_Set_Cfg(void* pt_gfx_mmu);
extern void GFX_MMU_Set_Enable(UINT32 u4_enable);

extern void GFX_MMU_Set_Cmprss(UINT32  u4_enable);
extern void GFX_MMU_UPD_PGT(void);
extern void _GFX_MMU_Set_Enable(UINT32 u4_enable);

extern UINT32 GFX_Get_Cm_Size(GFX_COLOR_MODE_T e_cm);
extern void GFX_Flush_Invalid_DCache(void);
extern void GFX_Invalid_DCache(void);

extern BOOL GFX_ColorConvert_YC(void* pvDstInfo,UINT32 u4SrcAddrY,UINT32 u4SrcAddrC, UINT32 u4DstCM, UINT8 u1Alpha);

#ifdef CC_TDTV_3D_OSD_ENABLE
extern int  _mt53fb_get_tmpbufaddr(void);
#endif
#if defined(CC_MT5398)||defined(CC_MT5880)
extern void GFX_SetXORColor(UINT32 u4Color);
extern void GFX_Set_PRE_COLORIZE(BOOL u4PreColorize);
extern void GFX_Set_FastBitBlt(UINT32 u4FastBitBlt);
extern void GFX_Set_SrcAlReplace(UINT32 u4SrcAlRep);
extern void GFX_Set_UseRectSrc(UINT32 u4UseRectSrc);
extern void GFX_Set_FlashColCnvInfo(UINT8 *pu1SrcY, UINT32 u4SrcYPitch, UINT8 *pu1SrcCbCr, 
	UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,UINT32 u4SrcWidth,UINT32 u4SrcHeight);
extern void GFX_YCbCrBlockToRaster_2Buf(UINT8 *pu1SrcY, UINT32 u4SrcYPitch, UINT8 *pu1SrcCbCr, 
	UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,UINT32 u4SrcWidth,UINT32 u4SrcHeight);
extern void GFX_YCbCrBlockToRaster_3Buf(UINT8 *pu1SrcY, UINT32 u4SrcYPitch, UINT8 *pu1SrcCbCr, 
	UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,UINT32 u4SrcWidth,UINT32 u4SrcHeight);
extern void GFX_YCbCr420ToARGB32(UINT8 *pu1SrcY, UINT32 u4SrcYPitch, UINT8 *pu1SrcCbCr, 
	UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,UINT32 u4SrcWidth,UINT32 u4SrcHeight,UINT32 u4AlphaValue,UINT32 u4FieldPic);
extern void GFX_Memset(UINT8 *pu1Addr, UINT32 u4Width, UINT32 Height, 
    UINT32 u4Color);



#endif

extern void GFX_Flush_Invalid_DCache(void);

#endif // GFX_IF_H



