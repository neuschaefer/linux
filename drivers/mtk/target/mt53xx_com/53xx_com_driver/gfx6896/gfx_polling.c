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
 * $RCSfile: gfx_polling.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/
 
/** @file gfx_polling.c
 *  polling interfaces of gfx driver
 *
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_hal_5381.h"


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define GREG_OFFSET_G_CONFIG        0x00
#define GREG_OFFSET_G_STATUS        0x04
#define GREG_OFFSET_G_MODE          0x10
#define GREG_OFFSET_RECT_COLOR      0x14
#define GREG_OFFSET_SRC_BSAD        0x18
#define GREG_OFFSET_DST_BSAD        0x1C
#define GREG_OFFSET_SRC_SIZE        0x28
#define GREG_OFFSET_S_OSD_WIDTH     0x2C
#define GREG_OFFSET_BITBLT_MODE     0x44

#define GREG_OFFSET_CLK_CFG         0x0224      // GRA_CK Configuration

#define GOP_FIRE                    1
#define GOP_RECT_FILL               2
#define GOP_BITBLT                  6
#define GCLK_DTDPLL_D3              4


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


//-------------------------------------------------------------------------
/** GFX_ResetEngine
 *  reset hw engine
 *  
 */
//-------------------------------------------------------------------------
void GFX_ResetEngine(void)
{
    volatile UINT32 u4Value;

    // set gra_clk to 200 MHz
    IO_WRITE32(CKGEN_BASE, GREG_OFFSET_CLK_CFG, GCLK_DTDPLL_D3);
    
    // enable engine clock (set non-zero value)
    IO_WRITE32(GRAPH_BASE, GREG_OFFSET_G_CONFIG, 0xA0000000);
    IO_WRITE32(GRAPH_BASE, GREG_OFFSET_G_MODE, 0x000001F0);        // fg_OP_MODE = 0x1F;
    
    // reset engine and cmdque :
    // 1. set fg_G_RST to 0x3 and fg_CQ_RST to 0x3
    // 2. set fg_G_RST to 0x0 and fg_CQ_RST to 0x0
    IO_WRITE32(GRAPH_BASE, GREG_OFFSET_G_CONFIG, 0xF0000000);
    IO_WRITE32(GRAPH_BASE, GREG_OFFSET_G_CONFIG, 0x00000000);

    // wait gfx hw reset ready
    for ( ; ; )
    {
        u4Value = IO_READ32(GRAPH_BASE, GREG_OFFSET_G_CONFIG);

        if ((u4Value & 0xF0000000) == 0xA0000000)
        {
            break;
        }
    }

    // set sdfifo, cmdfifo, postthrs and enable gfx interrupt
    u4Value = u4Value | (0x1 << 9) | (0x1 << 4) | (0x3 << 2) | (0x1 << 1);
    IO_WRITE32(GRAPH_BASE, GREG_OFFSET_G_CONFIG, u4Value);
}


//-------------------------------------------------------------------------
/** GFX_FillPolling
 *  fill rectangle (polling mode).
 *  
 *  @param  u4DstAddr   dst address
 *  @param  u4DstCM     dst color mode
 *  @param  u4Pitch     osd pitch
 *  @param  u4DstX      x offset 
 *  @param  u4DstY      y offset
 *  @param  u4Width     width
 *  @param  u4Height    height
 *  @param  u4Color     rect color
 */
//-------------------------------------------------------------------------
void GFX_FillPolling(UINT32 u4DstAddr, UINT32 u4DstCM, UINT32 u4Pitch, 
    UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, 
    UINT32 u4Color)
{
    volatile UINT32 u4DstShift, u4Val;

    switch (u4DstCM)
    {
    // CM_RGB_CLUT8
    case 10: u4DstShift = 0; break;

    // CM_RGB565_DIRECT16
    case 11: u4DstShift = 1; break;

    // CM_ARGB1555_DIRECT16
    case 12: u4DstShift = 1; break;

    // CM_ARGB4444_DIRECT16
    case 13: u4DstShift = 1; break;

    // CM_ARGB8888_DIRECT32
    case 14: u4DstShift = 2; break;

    default: u4DstShift = 2; break;
    }

    // RECT_COLOR
    IO_WRITE32(GRAPH_BASE, GREG_OFFSET_RECT_COLOR, u4Color);
    
    // DST_BSAD
    u4Val = (u4DstAddr) + (u4DstY * u4Pitch) + (u4DstX << u4DstShift);
    IO_WRITE32(GRAPH_BASE, GREG_OFFSET_DST_BSAD, u4Val);

    // SRC_SIZE
    u4Val = ((u4Height - 1) << 16) | (u4Width);
    IO_WRITE32(GRAPH_BASE, GREG_OFFSET_SRC_SIZE, u4Val);
    
    // S_OSD_WIDTH
    u4Val = (u4Pitch >> 4);
    IO_WRITE32(GRAPH_BASE, GREG_OFFSET_S_OSD_WIDTH, u4Val);

    // G_MODE
    u4Val = (GOP_FIRE << 11) | (GOP_RECT_FILL << 4) | (u4DstCM);
    IO_WRITE32(GRAPH_BASE, GREG_OFFSET_G_MODE, u4Val);

    // busy polling
    for ( ; ; )
    {
        u4Val = IO_READ32(GRAPH_BASE, GREG_OFFSET_G_STATUS);

        // check engine idle
        if (u4Val & 0x1)
        {
            break;
        }
    }
}


//-------------------------------------------------------------------------
/** GFX_DmaPolling
 *  DMA (polling mode).
 *  
 *  @param  u4DstAddr   dst address
 *  @param  u4SrcAddr   src address
 *  @param  u4Size      # bytes (max = 16383)
 */
//-------------------------------------------------------------------------
void GFX_DmaPolling(UINT32 u4DstAddr, UINT32 u4SrcAddr, UINT32 u4Size)
{
    volatile UINT32 u4Pitch, u4Val;

    // SRC_BSAD
    IO_WRITE32(GRAPH_BASE, GREG_OFFSET_SRC_BSAD, u4SrcAddr);
  
    // DST_BSAD
    IO_WRITE32(GRAPH_BASE, GREG_OFFSET_DST_BSAD, u4DstAddr);

    // SRC_SIZE
    u4Val = ((0xA << 27) | (u4Size));
    IO_WRITE32(GRAPH_BASE, GREG_OFFSET_SRC_SIZE, u4Val);
    
    // S_OSD_WIDTH
    u4Pitch = (((u4Size + 15) & (~15)) >> 4);
    u4Pitch = ((u4Pitch << 16) | (u4Pitch));
    IO_WRITE32(GRAPH_BASE, GREG_OFFSET_S_OSD_WIDTH, u4Pitch);

    // BITBLT_MODE
    IO_WRITE32(GRAPH_BASE, GREG_OFFSET_BITBLT_MODE, 0);
    
    // G_MODE
    u4Val = ((GOP_FIRE << 11) | (GOP_BITBLT << 4) | (0xA));
    IO_WRITE32(GRAPH_BASE, GREG_OFFSET_G_MODE, u4Val);

    // busy polling
    for ( ; ; )
    {
        u4Val = IO_READ32(GRAPH_BASE, GREG_OFFSET_G_STATUS);

        // check engine idle
        if (u4Val & 0x1)
        {
            break;
        }
    }
}


