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
 * $RCSfile: osd_pattern.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_pattern.c
 *  This file includes osd pattern generation function.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "osd_drvif.h"
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#ifdef __MODEL_slt__
#include "gfx_if.h"
#endif
#include "x_stl_lib.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_printf.h"
LINT_EXT_HEADER_END


#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif


#include "gfx_if.h"
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


#define LOG(lvl, fmt...)        Printf(fmt)


#define CLI(X, ARG...) \
    do \
    { \
        LOG(5, X"\n", ARG); \
        CLI_Parser_Arg(X, ARG); \
    } \
    while (0)


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static UINT32 _OsdRgb2PaleIdx(UINT32 u4Red, UINT32 u4Green, UINT32 u4Blue);


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static const UINT8 _au1Cos[256] =
{
    255, 254, 254, 254, 254, 254, 253, 253,
    252, 251, 251, 250, 249, 248, 247, 246,
    245, 244, 242, 241, 239, 238, 236, 235,
    233, 231, 229, 228, 226, 224, 221, 219,
    217, 215, 213, 210, 208, 205, 203, 200,
    198, 195, 193, 190, 187, 184, 182, 179,
    176, 173, 170, 167, 164, 161, 158, 155,
    152, 149, 146, 143, 139, 136, 133, 130,
    127, 124, 121, 118, 115, 111, 108, 105,
    102, 99, 96, 93, 90, 87, 84, 81,
    78, 75, 72, 70, 67, 64, 61, 59,
    56, 54, 51, 49, 46, 44, 41, 39,
    37, 35, 33, 30, 28, 26, 25, 23,
    21, 19, 18, 16, 15, 13, 12, 10,
    9, 8, 7, 6, 5, 4, 3, 3,
    2, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1,
    2, 3, 3, 4, 5, 6, 7, 8,
    9, 10, 12, 13, 15, 16, 18, 19,
    21, 23, 25, 26, 28, 30, 33, 35,
    37, 39, 41, 44, 46, 49, 51, 54,
    56, 59, 61, 64, 67, 70, 72, 75,
    78, 81, 84, 87, 90, 93, 96, 99,
    102, 105, 108, 111, 115, 118, 121, 124,
    127, 130, 133, 136, 139, 143, 146, 149,
    152, 155, 158, 161, 164, 167, 170, 173,
    176, 179, 182, 184, 187, 190, 193, 195,
    198, 200, 203, 205, 208, 210, 213, 215,
    217, 219, 221, 224, 226, 228, 229, 231,
    233, 235, 236, 238, 239, 241, 242, 244,
    245, 246, 247, 248, 249, 250, 251, 251,
    252, 253, 253, 254, 254, 254, 254, 254,
};


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define CHECK_COLOR_MODE(CM)                                                   \
    do                                                                         \
    {                                                                          \
        if (((CM) != (UINT32)OSD_CM_ARGB4444_DIRECT16) &&                      \
            ((CM) != (UINT32)OSD_CM_RGB565_DIRECT16) &&                        \
            ((CM) != (UINT32)OSD_CM_ARGB1555_DIRECT16) &&                      \
            ((CM) != (UINT32)OSD_CM_ARGB8888_DIRECT32) &&                      \
            ((CM) != (UINT32)OSD_CM_RGB_CLUT8) &&                              \
            ((CM) != (UINT32)OSD_CM_ALPHA_BIT))                                \
        {                                                                      \
            Printf("color mode %d no support\n", (CM));                        \
            return;                                                            \
        }                                                                      \
    }                                                                          \
    while (0)


// OSD_CM_ARGB8888_DIRECT32 -> big endian
// OSD_CM_ARGB4444_DIRECT16 -> little endian
#define WRITE_PIXEL(PTR, CM, A, R, G, B)                                       \
    do                                                                         \
    {                                                                          \
        if ((PTR) == NULL)                                                     \
        {                                                                      \
            break;                                                             \
        }                                                                      \
        switch ((CM))                                                          \
        {                                                                      \
        case OSD_CM_ARGB8888_DIRECT32:                                         \
            *(UINT32*)((UINT32)(PTR)) = (UINT32)((A) | ((UINT32)(R) << 8)      \
                                                      | ((UINT32)(G) << 16)    \
                                                      | ((UINT32)(B) << 24));  \
            (PTR) += 4;                                                        \
            break;                                                             \
        case OSD_CM_ARGB4444_DIRECT16:                                         \
            *(UINT16*)((UINT32)(PTR)) = (UINT16)(0xf000 | (((R) & 0xf0) << 4)  \
                                                        | ((G) & 0xf0)         \
                                                        | (((B) & 0xf0) >> 4));\
            (PTR) += 2;                                                        \
            break;                                                             \
        case OSD_CM_RGB565_DIRECT16:                                           \
            *(UINT16*)((UINT32)(PTR)) = (UINT16)((((R) & 0xf8) << 8)           \
                                                 | (((G) & 0xfc) << 3)         \
                                                 | (((B) & 0xf8) >> 3));       \
            (PTR) += 2;                                                        \
            break;                                                             \
        case OSD_CM_ARGB1555_DIRECT16:                                         \
            *(UINT16*)((UINT32)(PTR)) = (UINT16)(0x8000 | (((R) & 0xf8) << 7)  \
                                                        | (((G) & 0xf8) << 2)  \
                                                        | (((B) & 0xf8) >> 3));\
            (PTR) += 2;                                                        \
            break;                                                             \
        case OSD_CM_RGB_CLUT8:                                                 \
            *(UINT8*)((UINT32)(PTR)) = (UINT8)_OsdRgb2PaleIdx((R), (G), (B));  \
            (PTR) += 1;                                                        \
            break;                                                             \
        case OSD_CM_ALPHA_BIT:                                                 \
            if (((UINT32)PTR % 4) == 0)                                        \
            {                                                                  \
                *(UINT8*)((UINT32)(PTR)) = (UINT8)(A);                         \
            }                                                                  \
            else if (((UINT32)PTR % 4) == 1)                                   \
            {                                                                  \
                *(UINT8*)((UINT32)(PTR)) = (UINT8)(R);                         \
            }                                                                  \
            else if (((UINT32)PTR % 4) == 2)                                   \
            {                                                                  \
                *(UINT8*)((UINT32)(PTR)) = (UINT8)(G);                         \
            }                                                                  \
            else if (((UINT32)PTR % 4) == 3)                                   \
            {                                                                  \
                *(UINT8*)((UINT32)(PTR)) = (UINT8)(B);                         \
            }                                                                  \
            (PTR) += 1;                                                        \
            break;                                                             \
        default:                                                               \
            break;                                                             \
        }                                                                      \
    }                                                                          \
    while (0)

#define ALIGN_MASK(VAL, MSK) (((VAL) + (MSK)) & (~(MSK)))


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
#ifdef __MODEL_slt__
static void _AllocateMemory(UINT32 u4BufSize, UINT32 u4Align,
                            UINT8** _ppu1OrigBuf, UINT8** _ppu1Buf)
{
    if ((_ppu1OrigBuf == NULL) || (_ppu1Buf == NULL))
    {
        return;
    }
    *_ppu1OrigBuf = BSP_AllocDmaMemory(u4BufSize + u4Align);
    ASSERT(*_ppu1OrigBuf != NULL);
    *_ppu1Buf = (UINT8*)ALIGN_MASK((UINT32)*_ppu1OrigBuf, u4Align);
    HalFlushInvalidateDCache();
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static UINT32 _GenColor(UINT32 u4Idx, UINT32 u4ColorMode)
{
    UINT8 i, j, u1R = 0, u1G = 0, u1B = 0;
    UINT32 u4Color = 0;

//    i = (u4Idx>>6) & 0x7;
//    j = (u4Idx&0x3f) << 2;
    i = (u4Idx >> 8) & 0x3;
    j = u4Idx & 0xff;

    if (i == 0)
    {
        u1R = j;
        u1G = j;
        u1B = j;
    }
    else if (i == 1)
    {
        u1R = j;
    }
    else if (i == 2)
    {
        u1G = j;
    }
    else if (i == 3)
    {
        u1B = j;
    }

    if (u4ColorMode == (UINT32)OSD_CM_ARGB4444_DIRECT16)
    {
        //4444
        u4Color = 0xf000 | ((u1R << 4) & 0x0f00) | (u1G & 0x00f0) | (u1B >> 4);
        u4Color |= u4Color << 16;
    }
    else if (u4ColorMode == (UINT32)OSD_CM_ARGB8888_DIRECT32)
    {
        //8888...big endian
        u4Color = 0xff | (u1R << 8) | (u1G << 16 ) | (u1B << 24);
    }
    return u4Color;
}
#endif


//-----------------------------------------------------------------------------
/** RGB value to Palette Index
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static UINT32 _OsdRgb2PaleIdx(UINT32 u4Red, UINT32 u4Green, UINT32 u4Blue)
{
    UINT32 u4PaleIdx;
    UINT32 u4RBaseIdx, u4GBaseIdx, u4BBaseIdx, u4EBaseIdx;

    u4RBaseIdx = 0x0;       // for Red level  use
    u4GBaseIdx = 0x40;      // for Green level use
    u4BBaseIdx = 0x80;      // for Blue level use
    u4EBaseIdx = 0xC0;      // for grEy level use

    if ((u4Red != 0) && (u4Green != 0) && (u4Blue != 0))
    {
        // grEy level
        u4PaleIdx = u4EBaseIdx +
                    (((u4Red / 4) + (u4Green / 4) + (u4Blue / 4)) / 3);
    }
    else if (u4Red != 0)
    {
        // Red level
        u4PaleIdx = u4RBaseIdx + (u4Red / 4);
    }
    else if (u4Green != 0)
    {
        // Green level
        u4PaleIdx = u4GBaseIdx + (u4Green / 4);
    }
    else
    {
        // Blue level
        u4PaleIdx = u4BBaseIdx + (u4Blue / 4);
    }

    return u4PaleIdx;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Create a palette table (256 entries)
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void OSD_CreatePaleTable(OSD_ARGB_T *prPaleTable)
{
    INT32 i, j, i4RBaseIdx, i4GBaseIdx, i4BBaseIdx, i4EBaseIdx;

    if (prPaleTable == NULL)
    {
        return;
    }
    //VERIFY(prPaleTable != NULL);

    i4RBaseIdx = 0x0;       // for Red level  use
    i4GBaseIdx = 0x40;      // for Green level use
    i4BBaseIdx = 0x80;      // for Blue level use
    i4EBaseIdx = 0xC0;      // for grEy level use

    // generate Red level
    i = i4RBaseIdx;
    prPaleTable[i].u1Alpha = 0xff;
    prPaleTable[i].u1Red   = 0x3;
    prPaleTable[i].u1Green = 0x0;
    prPaleTable[i].u1Blue  = 0x0;
    j = 1;
    for (i = (i4RBaseIdx + 1); i < i4GBaseIdx; i++, j++)
    {
        prPaleTable[i].u1Alpha = 0xff;
        prPaleTable[i].u1Red   = 0x3 + (j * 4);
        prPaleTable[i].u1Green = 0x0;
        prPaleTable[i].u1Blue  = 0x0;
    }

    // generate Green level
    prPaleTable[i].u1Alpha = 0xff;
    prPaleTable[i].u1Red   = 0x0;
    prPaleTable[i].u1Green = 0x3;
    prPaleTable[i].u1Blue  = 0x0;
    j = 1;
    for (i = (i4GBaseIdx + 1); i < i4BBaseIdx; i++, j++)
    {
        prPaleTable[i].u1Alpha = 0xff;
        prPaleTable[i].u1Red   = 0x0;
        prPaleTable[i].u1Green = 0x3 + (j * 4);
        prPaleTable[i].u1Blue  = 0x0;
    }

    // generate Blue level
    prPaleTable[i].u1Alpha = 0xff;
    prPaleTable[i].u1Red   = 0x0;
    prPaleTable[i].u1Green = 0x0;
    prPaleTable[i].u1Blue  = 0x3;
    j = 1;
    for (i = (i4BBaseIdx + 1); i < i4EBaseIdx; i++, j++)
    {
        prPaleTable[i].u1Alpha = 0xff;
        prPaleTable[i].u1Red   = 0x0;
        prPaleTable[i].u1Green = 0x0;
        prPaleTable[i].u1Blue  = 0x3 + (j * 4);
    }

    // generate grEy level
    j = 0;
    for (i = i4EBaseIdx; i < 255; i++, j++)
    {
        prPaleTable[i].u1Alpha = 0xff;
        prPaleTable[i].u1Red   = (j * 4);
        prPaleTable[i].u1Green = (j * 4);
        prPaleTable[i].u1Blue  = (j * 4);
    }
    prPaleTable[i].u1Alpha = 0xff;
    prPaleTable[i].u1Red   = 0xff;
    prPaleTable[i].u1Green = 0xff;
    prPaleTable[i].u1Blue  = 0xff;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void OSD_DrawVLine(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                   UINT32 u4ColorMode)
{
    UINT32 x, y, tmp, pitch;
    UINT8* ptr = pu1Canvas;

    CHECK_COLOR_MODE(u4ColorMode);

    for (x = 0; x < u4Width; x++)
    {
        tmp = 128 >> (x / 256);
        tmp = _au1Cos[(x * tmp) % 256];
        WRITE_PIXEL(ptr, u4ColorMode, 0xff, tmp, tmp, tmp);
    }

    // for lint
    if (ptr == NULL)
    {
        return;
    }

    OSD_BYTE_PER_PIXEL(u4ColorMode, pitch);
    pitch *= u4Width;
    for (y = 1; y < u4Height; y++)
    {
        x_memcpy(ptr, pu1Canvas, pitch);
        ptr += pitch;
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void OSD_DrawHLine(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                   UINT32 u4ColorMode)
{
    UINT32 x, y, tmp;

    CHECK_COLOR_MODE(u4ColorMode);

    for (y = 0; y < u4Height; y++)
    {
        tmp = 128 >> (y / 128);
        tmp = _au1Cos[(y * tmp) % 256];
        for (x = 0; x < u4Width; x++)
        {
            WRITE_PIXEL(pu1Canvas, u4ColorMode, 0xff, tmp, tmp, tmp);
        }
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void OSD_DrawBorder(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                    UINT32 u4ColorMode)
{
    UINT32 x, y;

    CHECK_COLOR_MODE(u4ColorMode);

    // first line
    for (x = 0; x < u4Width; x++)
    {
        //WRITE_PIXEL(pu1Canvas, u4ColorMode, 0xff, 0xfe, 0x43, 0x21);
        WRITE_PIXEL(pu1Canvas, u4ColorMode, 0xff, x+1, x+17, x+33);
    }

    for (y = 1; y < (u4Height - 1); y++)
    {
        WRITE_PIXEL(pu1Canvas, u4ColorMode, 0xff, 0xff, y, y);
        for (x = 1; x < (u4Width - 1); x++)
        {
            WRITE_PIXEL(pu1Canvas, u4ColorMode, 0xff, 0, 0, 0);
        }
        WRITE_PIXEL(pu1Canvas, u4ColorMode, 0xff, 0xfe, 0x43, 0x21);
    }

    // last line
    for (x = 0; x < u4Width; x++)
    {
        WRITE_PIXEL(pu1Canvas, u4ColorMode, 0xff, 0xfe, 0x43, 0x21);
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void OSD_DrawColorbar(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode)
{
    UINT32 x, y, band, pitch;
    UINT8* ptr = pu1Canvas;

    CHECK_COLOR_MODE(u4ColorMode);

    for (x = 0; x < u4Width; x++)
    {
        band = ((x * 8) / u4Width);
        switch (band)
        {
        case 0:
            WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0xff, 0xff, 0xff);
            break;
        case 1:
            WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0xff, 0xff, 0);
            break;
        case 2:
            WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0, 0xff, 0xff);
            break;
        case 3:
            WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0, 0xff, 0);
            break;
        case 4:
            WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0xff, 0, 0xff);
            break;
        case 5:
            WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0xff, 0, 0);
            break;
        case 6:
            WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0, 0, 0xff);
            break;
        case 7:
            WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0, 0, 0);
            break;
        default:
            break;
        }
    }

    // for lint
    if (ptr == NULL)
    {
        return;
    }

    OSD_BYTE_PER_PIXEL(u4ColorMode, pitch);
    pitch *= u4Width;
    for (y = 1; y < u4Height; y++)
    {
        x_memcpy(ptr, pu1Canvas, pitch);
        ptr += pitch;
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void OSD_DrawColorbar2(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode)
{
    UINT32 x, y, band, pitch;
    UINT8* ptr = pu1Canvas;

    CHECK_COLOR_MODE(u4ColorMode);

    for (x = 0; x < u4Width; x++)
    {
        band = ((x * 7) / u4Width);
        switch (band)
        {
        case 0:
            WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0xff, 0xff, 0xff);
            break;
        case 1:
            WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0xff, 0xff, 0);
            break;
        case 2:
            WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0, 0xff, 0xff);
            break;
        case 3:
            WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0, 0xff, 0);
            break;
        case 4:
            WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0xff, 0, 0xff);
            break;
        case 5:
            WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0xff, 0, 0);
            break;
        case 6:
            WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0, 0, 0xff);
            break;
        case 7:
            WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0, 0, 0);
            break;
        default:
            break;
        }
    }

    // for lint
    if (ptr == NULL)
    {
        return;
    }

    OSD_BYTE_PER_PIXEL(u4ColorMode, pitch);
    pitch *= u4Width;
    for (y = 1; y < u4Height; y++)
    {
        x_memcpy(ptr, pu1Canvas, pitch);
        ptr += pitch;
    }
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void OSD_DrawSlt(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                 UINT32 u4ColorMode)
{
#ifdef __MODEL_slt__
    UINT8 *pu1OrigGfxBuf, *pu1GfxBuf;
    UINT32 u4Idx, u4GfxPitch, u4GfxWidth, u4OsdPitch;

    CHECK_COLOR_MODE(u4ColorMode);
    if (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT8)
    {
        Printf("slt pattern not support OSD_CM_RGB_CLUT8");
        return;
    }

    // allocate memory for gfx canvas
    u4GfxWidth = u4Width + u4Height;
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4GfxPitch);
    u4GfxPitch *= u4GfxWidth;
    _AllocateMemory(u4GfxPitch, 0xf, &pu1OrigGfxBuf, &pu1GfxBuf);
    HalFlushInvalidateDCache();
    // draw gfx canvas
    GFX_SetDst(pu1GfxBuf, u4ColorMode, u4GfxPitch);
    for (u4Idx = 0; u4Idx < u4GfxWidth; u4Idx++)
    {
        GFX_SetColor(_GenColor(u4Idx, u4ColorMode));
        GFX_VLine(u4Idx, 0, 1);
    }
    GFX_Flush();
    GFX_Wait();

    // bitblt from gfx canvas to osd canvas
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4OsdPitch);
    u4OsdPitch *= u4Width;
    GFX_SetSrc(pu1GfxBuf, u4ColorMode, u4GfxPitch);
    GFX_SetDst(pu1Canvas, u4ColorMode, u4OsdPitch);
    for (u4Idx = 0; u4Idx < u4Height; u4Idx++)
    {
        GFX_BitBlt(u4Idx, 0, 0, u4Idx, u4Width, 1);
        GFX_Flush();
        GFX_Wait();
    }

    // free memory for gfx canvas
    IGNORE_RET(BSP_FreeDmaMemory((UINT32)pu1OrigGfxBuf));
#else
    Printf("this pattern not support in SLT\n");
#endif // __MODEL_slt__
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void OSD_DrawRamp(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                  UINT32 u4ColorMode)
{
    UINT32 x, y, tmp, band, pitch, ramp_height, ramp_type;
    UINT8 *ptr, *pulOrigCanvas;

    CHECK_COLOR_MODE(u4ColorMode);

    if (pu1Canvas == NULL)
    {
        return;
    }
    pulOrigCanvas = pu1Canvas;

    OSD_BYTE_PER_PIXEL(u4ColorMode, tmp);
    pitch = u4Width * tmp;
    ramp_height = (u4Height / 10);

    // draw horizontal multi-burst
    OSD_DrawVLine(pu1Canvas, u4Width, ramp_height, u4ColorMode);
    pu1Canvas += (pitch * ramp_height);
    ptr = pu1Canvas;

    // draw 16 and 255 level RGB ramp
    for (ramp_type = 0; ramp_type < 8; ramp_type++)
    {
        if (ramp_type < 4)
        {
            band = (u4Width / 16);
        }
        else
        {
            band = (u4Width / 256);
        }

        for (x = 0; x < u4Width; x++)
        {
            if (ramp_type < 4)
            {
                tmp = (x / band) * 0x11;
            }
            else
            {
                tmp = (x / band);
            }
            if (tmp > 0xff)
            {
                tmp = 0xff;
            }
            switch (ramp_type)
            {
            case 0:
            case 4:
                WRITE_PIXEL(ptr, u4ColorMode, 0xff, tmp, tmp, tmp);
                break;
            case 1:
            case 5:
                WRITE_PIXEL(ptr, u4ColorMode, 0xff, tmp, 0, 0);
                break;
            case 2:
            case 6:
                WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0, tmp, 0);
                break;
            case 3:
            case 7:
                WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0, 0, tmp);
                break;
            default:
                WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0, 0, 0);
                break;
            }
        }

        // for lint
        if (ptr == NULL)
        {
            return;
        }

        for (y = 1; y < ramp_height; y++)
        {
            x_memcpy(ptr, pu1Canvas, pitch);
            ptr += pitch;
        }
        pu1Canvas = ptr;
    }

    // draw color bar
    OSD_DrawColorbar(pu1Canvas, u4Width, u4Height - (ramp_height * 9),
                     u4ColorMode);

    // draw border
    // first line
    for (x = 0; x < u4Width; x++)
    {
        WRITE_PIXEL(pulOrigCanvas, u4ColorMode, 0xff, 0xff, 0, 0);
    }
    OSD_BYTE_PER_PIXEL(u4ColorMode, tmp);
    pitch = tmp * (u4Width - 2);
    for (y = 1; y < (u4Height - 1); y++)
    {
        WRITE_PIXEL(pulOrigCanvas, u4ColorMode, 0xff, 0xff, 0, 0);
        // for lint
        if (pulOrigCanvas == NULL)
        {
            return;
        }
        pulOrigCanvas += pitch; // skip non-border area
        WRITE_PIXEL(pulOrigCanvas, u4ColorMode, 0xff, 0xff, 0, 0);
    }
    // last line
    for (x = 0; x < u4Width; x++)
    {
        WRITE_PIXEL(pulOrigCanvas, u4ColorMode, 0xff, 0xff, 0, 0);
    }
}

void OSD_DrawColor(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode, UINT32 u4Color)
{
    UINT32 x, y,  pitch;
    UINT8* ptr = pu1Canvas;

    CHECK_COLOR_MODE(u4ColorMode);

    for (x = 0; x < u4Width; x++)
    {
            WRITE_PIXEL(ptr, u4ColorMode, (u4Color>>24)&0xff, (u4Color>>16)&0xff, (u4Color>>8)&0xff, u4Color&0xff);  
    }

    // for lint
    if (ptr == NULL)
    {
        return;
    }

    OSD_BYTE_PER_PIXEL(u4ColorMode, pitch);
    pitch *= u4Width;
    for (y = 1; y < u4Height; y++)
    {
        x_memcpy(ptr, pu1Canvas, pitch);
        ptr += pitch;
    }
}


void OSD_DrawMute(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                  UINT32 u4ColorMode, UINT32 u4MuteColor)
{
    UINT32 x, y, pitch;
    UINT8* ptr = pu1Canvas;

    CHECK_COLOR_MODE(u4ColorMode);

    for (x = 0; x < u4Width; x++)
    {
        WRITE_PIXEL(ptr, u4ColorMode,
                    (u4MuteColor & 0xff000000) >> 24,
                    (u4MuteColor & 0xff0000) >> 16,
                    (u4MuteColor & 0xff00) >> 8,
                    u4MuteColor & 0xff);
    }

    // for lint
    if (ptr == NULL)
    {
        return;
    }

    OSD_BYTE_PER_PIXEL(u4ColorMode, pitch);
    pitch *= u4Width;
    for (y = 1; y < u4Height; y++)
    {
        x_memcpy(ptr, pu1Canvas, pitch);
        ptr += pitch;
    }
}

void OSD_DrawPattern0(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode,UINT32 u4BgColor)
{

    UINT32  u4OsdPitch, u4Size, u4Bpp;

    CHECK_COLOR_MODE(u4ColorMode);
    if (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT8)
    {
        Printf("draw pattern not support OSD_CM_RGB_CLUT8");
        return;
    }
    // allocate memory for gfx canvas
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
    u4Size = u4Width * u4Height * u4Bpp;
    u4OsdPitch = u4Bpp * u4Width;
    u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;

    // _AllocateMemory(u4GfxPitch, 0xf, &pu1OrigGfxBuf, &pu1GfxBuf);

    x_memset((void*)pu1Canvas, 0, u4Size);
    // draw gfx canvas
    GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);
    {
        GFX_SetColor(0xff000000);
        GFX_VLine(0, 0, u4Height);
        GFX_VLine(u4Width - 1, 0, u4Height);
        GFX_HLine(0, 0, u4Width);
        GFX_HLine(0, u4Height - 1, u4Width);
        GFX_SetColor(u4BgColor);
        GFX_Fill(1, 1, u4Width - 2, u4Height - 2);
    }
    GFX_Flush();
    GFX_Wait();
}

void OSD_DrawPattern1(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode, UINT32 u4Pattern )
{

    UINT32  u4OsdPitch, u4Size, u4Bpp, u4Color;

    CHECK_COLOR_MODE(u4ColorMode);
    if (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT8)
    {
        Printf("draw pattern not support OSD_CM_RGB_CLUT8");
        return;
    }
    // allocate memory for gfx canvas
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
    u4Size = u4Width * u4Height * u4Bpp;
    u4OsdPitch = u4Bpp * u4Width;
    u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;

    // _AllocateMemory(u4GfxPitch, 0xf, &pu1OrigGfxBuf, &pu1GfxBuf);
    switch (u4Pattern)
    {
        case 1:
            u4Color = 0xffff00ff;
            break;
        case 2:
            u4Color = 0xff000000;
            break;
        case 3:
            u4Color = 0xffffff00;
            break;
        case 4:
            u4Color = 0xffff0000;
            break;
        case 6:
            u4Color = 0xff00ff00;
            break;
        case 8:
            u4Color = 0xff0000ff;
            break;
        case 10:
            u4Color = 0xffc0c0c0;
            break;
        case 12:
            u4Color = 0xff00ffff;
            break;
        case 20:
            u4Color = 0xffffffff;
            break;
	    case 22:
	    case 24:
            u4Color = 0xff808080;
            break;
		case 52:
			u4Color = 0xff818181;
			break;
        default:
            u4Color = 0xffff0000;
            break;

    }
    x_memset((void*)pu1Canvas, 0, u4Size);
    GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);
    {
        GFX_SetColor(0xff000000);
        GFX_VLine(0, 0, u4Height);
        GFX_VLine(u4Width - 1, 0, u4Height);
        GFX_HLine(0, 0, u4Width);
        GFX_HLine(0, u4Height - 1, u4Width);
        GFX_SetColor(u4Color);
        GFX_Fill(1, 1, u4Width - 2, u4Height - 2);
    }
    GFX_Flush();
    GFX_Wait();
}

void OSD_DrawPattern2(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode, UINT32 u4Pattern)
{

    UINT32  u4OsdPitch, u4Size, u4Bpp;
    UINT32 u4Color0 = 0xff000000, u4Color1 = 0xffffffff;
    CHECK_COLOR_MODE(u4ColorMode);
    if (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT8)
    {
        Printf("draw pattern not support OSD_CM_RGB_CLUT8");
        return;
    }
    // allocate memory for gfx canvas
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
    u4Size = u4Width * u4Height * u4Bpp;
    u4OsdPitch = u4Bpp * u4Width;
    u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;
    // _AllocateMemory(u4GfxPitch, 0xf, &pu1OrigGfxBuf, &pu1GfxBuf);
    switch (u4Pattern)
    {
        case 5:
            u4Color0 = 0xff000000;
            u4Color1 = 0xffffffff;
            break;
        case 7:
            u4Color0 = 0xffffffff;
            u4Color1 = 0xff000000;
            break;
	    case 19:
            x_memset((void*)pu1Canvas, 0, u4Size);
            GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);
            GFX_SetColor(0xff000000);
            GFX_VLine(0, 0, u4Height);
            GFX_VLine(u4Width - 1, 0, u4Height);
            GFX_HLine(0, 0, u4Width);
            GFX_HLine(0, u4Height - 1, u4Width);
            GFX_SetColor(0xffff0000);
            GFX_VLine(1, 1, u4Height - 2);
            GFX_SetColor(0xff0000ff);
            GFX_VLine(u4Width - 2 -2, 1, u4Height - 2);
            GFX_SetColor(0xffffffff);
            GFX_HLine(2, 1, u4Width - 4 - 2);
            GFX_HLine(2, u4Height - 2, u4Width - 4 - 2);
            GFX_Flush();
            GFX_Wait();
            return;
        case 21:
            x_memset((void*)pu1Canvas, 0, u4Size);
            GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);
            GFX_SetColor(0xff000000);
            GFX_Fill(0,0, u4Width, u4Height);
            GFX_SetColor(0xffff0000);
            GFX_Fill(u4Width/4, u4Height*3/4, 10, 10);
            GFX_SetColor(0xff00ff00);
           GFX_Fill(u4Width*2/4, u4Height*3/4, 10, 10);
            GFX_SetColor(0xff0000ff);
            GFX_Fill(u4Width*3/4, u4Height*3/4, 10, 10);
            GFX_Flush();
            GFX_Wait();
            return;
        default:
            break;
    }
    x_memset((void*)pu1Canvas, 0, u4Size);
    // draw gfx canvas
    GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);
    {
        GFX_SetColor(0xff000000);
        GFX_VLine(0, 0, u4Height);
        GFX_VLine(u4Width - 1, 0, u4Height);
        GFX_HLine(0, 0, u4Width);
        GFX_HLine(0, u4Height - 1, u4Width);
        GFX_SetColor(u4Color0);
        GFX_Fill(1, 1, (u4Width - 2) / 2, (u4Height - 2) / 2);
        GFX_Fill((u4Width - 2) / 2 + 1, (u4Height - 2) / 2 + 1, (u4Width - 2) / 2, (u4Height - 2) / 2);
        GFX_SetColor(u4Color1);
        GFX_Fill((u4Width - 2) / 2 + 1, 1, (u4Width - 2) / 2, (u4Height - 2) / 2);
        GFX_Fill(1, (u4Height - 2) / 2 + 1, (u4Width - 2) / 2, (u4Height - 2) / 2);
    }
    GFX_Flush();
    GFX_Wait();
}

void OSD_DrawPattern4(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode, UINT32 u4Pattern )
{

    UINT32  u4OsdPitch, u4Size, u4Bpp;

    INT32 i4Stepx = 3, i4Stepy = 0,i4GradX[4] = {0, 0, 0, 0}, i4GradY[4]  = {0, 0, 0, 0};

    CHECK_COLOR_MODE(u4ColorMode);
    if (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT8)
    {
        Printf("draw pattern not support OSD_CM_RGB_CLUT8 %d", u4Pattern);
        return;
    }
    // allocate memory for gfx canvas
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
    u4Size = u4Width * u4Height * u4Bpp;
    u4OsdPitch = u4Bpp * u4Width;
    u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;

	switch (u4Pattern)
		{
    	case 15:
			x_memset((void*)pu1Canvas, 0, u4Size);
    		// draw gfx canvas
    		GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);
    		
        	GFX_SetColor(0xff000000);
        	GFX_VLine(0, 0, u4Height);
        	GFX_VLine(u4Width - 1, 0, u4Height);
        	GFX_HLine(0, 0, u4Width);
        	GFX_HLine(0, u4Height - 1, u4Width);
        	GFX_SetColor(0xffffffff);
        	GFX_Fill(1, 1, u4Width - 2, (u4Height - 2) / 8);
        	GFX_SetColor(0xffffff00);
        	GFX_Fill(1, (u4Height - 2) / 8, u4Width - 2, (u4Height - 2) / 8);
        	GFX_SetColor(0xff00ffff);
        	GFX_Fill(1, 2*(u4Height - 2) / 8, u4Width - 2, (u4Height - 2) / 8);
        	GFX_SetColor(0xff00ff00);
        	GFX_Fill(1, 3*(u4Height - 2) / 8, u4Width - 2, (u4Height - 2) / 8);
        	GFX_SetColor(0xffff00ff);
        	GFX_Fill(1, 4*(u4Height - 2) / 8, u4Width - 2, (u4Height - 2) / 8);
        	GFX_SetColor(0xffff0000);
        	GFX_Fill(1, 5*(u4Height - 2) / 8, u4Width - 2, (u4Height - 2) / 8);
        	GFX_SetColor(0xff0000ff);
        	GFX_Fill(1, 6*(u4Height - 2) / 8, u4Width - 2, (u4Height - 2) / 8);
        	GFX_SetColor(0xff000000);
        	GFX_Fill(1, 7*(u4Height - 2) / 8, u4Width - 2, (u4Height - 2) / 8);
    
    		GFX_Flush();
    		GFX_Wait();

			return;
		case 48:
			i4Stepx = u4Width / (0xFF + 0x1);
			i4Stepy = 0;
			
			x_memset((void*)pu1Canvas, 0, u4Size);
    		// draw gfx canvas
    		GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);

        	GFX_SetColor(0xffffffff);
			//i4GradX[4] = {0, -1, -1, -1};
			i4GradX[0]=0;
			i4GradX[1]=-1;
			i4GradX[2]=-1;
			i4GradX[3]=-1;
        	CLI("gfx.set.gradopt %d %d %d %d %d %d %d %d %d %d", i4Stepx, i4Stepy, i4GradX[0],i4GradX[1], i4GradX[2], i4GradX[3], i4GradY[0], i4GradY[1], i4GradY[2], i4GradY[3]);
        	CLI("gfx.op.grad %d  %d %d %d h", 0 , 0, u4Width , u4Height  / 8); 	
    		GFX_Flush();
    		GFX_Wait();


        	GFX_SetColor(0xffffff00);
			//i4GradX[4] = {0, -1, -1, 0};
			i4GradX[0]=0;
			i4GradX[1]=-1;
			i4GradX[2]=-1;
			i4GradX[3]=0;
        	CLI("gfx.set.gradopt %d %d %d %d %d %d %d %d %d %d", i4Stepx, i4Stepy, i4GradX[0],i4GradX[1], i4GradX[2], i4GradX[3], i4GradY[0], i4GradY[1], i4GradY[2], i4GradY[3]);
        	CLI("gfx.op.grad %d  %d %d %d h", 0 , (u4Height ) / 8, u4Width , (u4Height ) / 8); 	
    		GFX_Flush();
    		GFX_Wait();


        	GFX_SetColor(0xff00ffff);
			//i4GradX[4] = {0, 0, -1, -1};
			i4GradX[0]=0;
			i4GradX[1]=0;
			i4GradX[2]=-1;
			i4GradX[3]=-1;
        	CLI("gfx.set.gradopt %d %d %d %d %d %d %d %d %d %d", i4Stepx, i4Stepy, i4GradX[0],i4GradX[1], i4GradX[2], i4GradX[3], i4GradY[0], i4GradY[1], i4GradY[2], i4GradY[3]);
        	CLI("gfx.op.grad %d  %d %d %d h", 0 , (u4Height ) / 8*2, u4Width , (u4Height ) / 8); 	
    		GFX_Flush();
    		GFX_Wait();
			

			GFX_SetColor(0xff00ff00);
			//i4GradX[4] = {0, 0, -1, 0};
			i4GradX[0]=0;
			i4GradX[1]=0;
			i4GradX[2]=-1;
			i4GradX[3]=0;
        	CLI("gfx.set.gradopt %d %d %d %d %d %d %d %d %d %d", i4Stepx, i4Stepy, i4GradX[0],i4GradX[1], i4GradX[2], i4GradX[3], i4GradY[0], i4GradY[1], i4GradY[2], i4GradY[3]);
        	CLI("gfx.op.grad %d  %d %d %d h", 0 , (u4Height ) / 8*3, u4Width , (u4Height ) / 8); 	
    		GFX_Flush();
    		GFX_Wait();


			GFX_SetColor(0xffff00ff);
			//i4GradX[4] = {0, -1, 0, -1};
			i4GradX[0]=0;
			i4GradX[1]=-1;
			i4GradX[2]=0;
			i4GradX[3]=-1;
        	CLI("gfx.set.gradopt %d %d %d %d %d %d %d %d %d %d", i4Stepx, i4Stepy, i4GradX[0],i4GradX[1], i4GradX[2], i4GradX[3], i4GradY[0], i4GradY[1], i4GradY[2], i4GradY[3]);
        	CLI("gfx.op.grad %d  %d %d %d h", 0 , (u4Height ) / 8*4, u4Width , (u4Height ) / 8); 		
    		GFX_Flush();
    		GFX_Wait();

			GFX_SetColor(0xffff0000);
			//i4GradX[4] = {0, -1, 0, 0};
			i4GradX[0]=0;
			i4GradX[1]=-1;
			i4GradX[2]=0;
			i4GradX[3]=0;
        	CLI("gfx.set.gradopt %d %d %d %d %d %d %d %d %d %d", i4Stepx, i4Stepy, i4GradX[0],i4GradX[1], i4GradX[2], i4GradX[3], i4GradY[0], i4GradY[1], i4GradY[2], i4GradY[3]);
        	CLI("gfx.op.grad %d  %d %d %d h", 0 , (u4Height ) / 8 *5, u4Width , (u4Height ) / 8); 
    		GFX_Flush();
    		GFX_Wait();			

			GFX_SetColor(0xff0000ff);
			//i4GradX[4] = {0, 0, 0, -1};
			i4GradX[0]=0;
			i4GradX[1]=0;
			i4GradX[2]=0;
			i4GradX[3]=-1;
        	CLI("gfx.set.gradopt %d %d %d %d %d %d %d %d %d %d", i4Stepx, i4Stepy, i4GradX[0],i4GradX[1], i4GradX[2], i4GradX[3], i4GradY[0], i4GradY[1], i4GradY[2], i4GradY[3]);
        	CLI("gfx.op.grad %d  %d %d %d h", 0 , (u4Height ) / 8 *6, u4Width , (u4Height ) / 8); 	

        	GFX_SetColor(0xff000000);
        	GFX_Fill(0, (u4Height ) / 8 *7, u4Width , (u4Height) / 8);   
    		GFX_Flush();
    		GFX_Wait();

			return;
		
		case 51:
					i4Stepx = u4Width / (0xFF + 0x1);
					i4Stepy = 0;
					x_memset((void*)pu1Canvas, 0, u4Size);
					GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);
					GFX_SetColor(0xff000000);
					i4GradX[0]=0;
					i4GradX[1]=1;
					i4GradX[2]=1;
					i4GradX[3]=1;
					CLI("gfx.set.gradopt %d %d %d %d %d %d %d %d %d %d", i4Stepx, i4Stepy, i4GradX[0],i4GradX[1], i4GradX[2], i4GradX[3], i4GradY[0], i4GradY[1], i4GradY[2], i4GradY[3]);
					CLI("gfx.op.grad %d  %d %d %d h", 0 , 1 , u4Width - 1, (u4Height - 2) / 4); 	
					GFX_Flush();
					GFX_Wait();
		
					GFX_SetColor(0xff000000);
					i4GradX[0]=0;
					i4GradX[1]=1;
					i4GradX[2]=0;
					i4GradX[3]=0;
					CLI("gfx.set.gradopt %d %d %d %d %d %d %d %d %d %d", i4Stepx, i4Stepy, i4GradX[0],i4GradX[1], i4GradX[2], i4GradX[3], i4GradY[0], i4GradY[1], i4GradY[2], i4GradY[3]);
					CLI("gfx.op.grad %d  %d %d %d h", 0 , 1+(u4Height - 2) / 4 , u4Width - 1, (u4Height - 2) / 4);	
					GFX_Flush();
					GFX_Wait();
		
					GFX_SetColor(0xff000000);
					i4GradX[0]=0;
					i4GradX[1]=0;
					i4GradX[2]=1;
					i4GradX[3]=0;
					CLI("gfx.set.gradopt %d %d %d %d %d %d %d %d %d %d", i4Stepx, i4Stepy, i4GradX[0],i4GradX[1], i4GradX[2], i4GradX[3], i4GradY[0], i4GradY[1], i4GradY[2], i4GradY[3]);
					CLI("gfx.op.grad %d  %d %d %d h", 0 , 1+(u4Height - 2) / 4 * 2 , u4Width - 1, (u4Height - 2) / 4);	
					GFX_Flush();
					GFX_Wait();
		
					GFX_SetColor(0xff000000);
					i4GradX[0]=0;
					i4GradX[1]=0;
					i4GradX[2]=0;
					i4GradX[3]=1;
					CLI("gfx.set.gradopt %d %d %d %d %d %d %d %d %d %d", i4Stepx, i4Stepy, i4GradX[0],i4GradX[1], i4GradX[2], i4GradX[3], i4GradY[0], i4GradY[1], i4GradY[2], i4GradY[3]);
					CLI("gfx.op.grad %d  %d %d %d h", 0 , 1+(u4Height - 2) / 4 * 3, u4Width - 1, (u4Height - 2) / 4);	
					GFX_Flush();
					GFX_Wait();
		
		
					return;

		default :
			break;
		}
	return;
}

void OSD_DrawPattern5(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode)
{
    UINT32 tmp, ramp_type,u4Idx;
    UINT8 *ptr;
    UINT32  u4OsdPitch, u4Size, u4Bpp;
	
    CHECK_COLOR_MODE(u4ColorMode);

    if (pu1Canvas == NULL)
    {
        return;
    }
	OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
	u4Size = u4Width * u4Height * u4Bpp;
	u4OsdPitch = u4Bpp * u4Width;
	u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;
	x_memset((void*)pu1Canvas, 0, u4Size);

    ptr = pu1Canvas;
    for (tmp = 0; tmp < 16; tmp++)
    {
       switch (tmp)
       {
       case 0:
	case 1:
	case 2:
	case 13:
	case 14:
	case 15:
        for (ramp_type = 0; ramp_type < 10; ramp_type++)
        {
            WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x00, 0x00, 0xff);
        }
        ptr += u4OsdPitch -4*10;
	break;
	case 3:
	case 4:
	case 5:
	case 6:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12: 
        {
            WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x00, 0x00, 0xff);
	     WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0xff, 0xff, 0xff);
		for (ramp_type = 0; ramp_type < 6; ramp_type++)	
			{		  
			 WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x00, 0x00, 0xff);
			}
	      WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0xff, 0xff, 0xff);
	     WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x00, 0x00, 0xff);
        }
        ptr += u4OsdPitch -4*10;
	break;
	case 7: 
        {
            WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x00, 0x00, 0xff);
		for (ramp_type = 0; ramp_type < 8; ramp_type++)	
			{		  
			 WRITE_PIXEL(ptr, u4ColorMode, 0xff, 0xff, 0xff, 0xff);
			}
	     WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x00, 0x00, 0xff);
        }
	  ptr += u4OsdPitch -4*10;
	break;
	default:
		break;
       }
    }
    // for lint
    if (ptr == NULL)
    {
        return;
    }
		
    GFX_SetSrc(pu1Canvas, u4ColorMode, u4OsdPitch);
    GFX_SetDst(pu1Canvas, u4ColorMode, u4OsdPitch);
    for (u4Idx = 0; u4Idx < (u4Width -10) /10; u4Idx++)
    {
        GFX_BitBlt(0, 0, u4Idx*10, 0, 10, 16);
        GFX_Flush();
        GFX_Wait();
    }
   for (u4Idx = 0; u4Idx < (u4Height -16) /16; u4Idx++)
	   {
		   GFX_BitBlt(0, 0, 0, u4Idx*16, u4Width, 16);
		   GFX_Flush();
		   GFX_Wait();
	   }


}


void OSD_DrawPattern6(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode)
{
    UINT32 tmp, ramp_type;
    UINT8 *ptr;
    UINT8 *ptrTemp;
    UINT32  u4OsdPitch, u4Size, u4Bpp;

    CHECK_COLOR_MODE(u4ColorMode);

    if (pu1Canvas == NULL)
    {
        return;
    }
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
    u4Size = u4Width * u4Height * u4Bpp;
    u4OsdPitch = u4Bpp * u4Width;
    u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;
    x_memset((void*)pu1Canvas, 0, u4Size);

    ptrTemp = pu1Canvas;
    ptr = pu1Canvas;
    for (tmp = 0; tmp < u4Height; tmp++)
    {
        ptr = ptrTemp;
        if ((tmp == 0) || (tmp == u4Height - 1))
        {
            for (ramp_type = 0; ramp_type < u4Width; ramp_type++)
            {
                if (ramp_type % 2 == 0)
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x80, 0x00, 0xff);
                }
                else
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x80, 0x00, 0x80, 0xff);
                }
            }
        }
        else  if ((tmp % 4 == 1) || (tmp % 4 == 2))
        {
            for (ramp_type = 0; ramp_type < u4Width; ramp_type++)
            {
                if (ramp_type % 2 == 0)
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x80, 0x00, 0x80, 0xff);
                }
                else
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x80, 0x00, 0xff);
                }
            }
        }
        else  if ((tmp % 4 == 3) || (tmp % 4 == 0))
        {
            for (ramp_type = 0; ramp_type < u4Width; ramp_type ++)
            {
                if (ramp_type % 2 == 0)
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x80, 0x00, 0xff);
                }
                else
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x80, 0x00, 0x80, 0xff);
                }
            }
        }
        ptrTemp += u4OsdPitch;
    }
}

void OSD_DrawPattern7(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode)
{
    UINT32 tmp, ramp_type;
    UINT8 *ptr;
    UINT8 *ptrTemp;
    UINT32  u4OsdPitch, u4Size, u4Bpp;

    CHECK_COLOR_MODE(u4ColorMode);

    if (pu1Canvas == NULL)
    {
        return;
    }
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
    u4Size = u4Width * u4Height * u4Bpp;
    u4OsdPitch = u4Bpp * u4Width;
    u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;
    x_memset((void*)pu1Canvas, 0, u4Size);

    ptrTemp = pu1Canvas;
    ptr = pu1Canvas;
    for (tmp = 0; tmp < u4Height; tmp++)
    {
        ptr = ptrTemp;
        if (tmp % 2 == 0)
    {
        for (ramp_type = 0; ramp_type < u4Width; ramp_type++)
            {
                if (ramp_type % 2 == 0)
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x80, 0x00, 0xff);
                }
                else
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x80, 0x00, 0x80, 0xff);
                }
            }
        }
        else
        {
            for (ramp_type = 0; ramp_type < u4Width; ramp_type++)
            {
                if (ramp_type % 2 == 0)
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x80, 0x00, 0x80, 0xff);
                }
                else
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x80, 0x00, 0xff);
                }
            }
        }
        ptrTemp += u4OsdPitch;
    }
}


void OSD_DrawPattern8(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode, UINT32 u4Pattern )
{
    UINT32 tmp, ramp_type;
    UINT8 *ptr;
    UINT8 *ptrTemp;
    UINT32  u4OsdPitch, u4Size, u4Bpp;
	UINT32 ramp_width,ramp_Heidht;

    CHECK_COLOR_MODE(u4ColorMode);

    if (pu1Canvas == NULL)
    {
        return;
    }
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
    u4Size = u4Width * u4Height * u4Bpp;
    u4OsdPitch = u4Bpp * u4Width;
    u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;

	switch( u4Pattern )
		{
		case 27:
    		x_memset((void*)pu1Canvas, 0, u4Size);

    		ptrTemp = pu1Canvas;
    		ptr = pu1Canvas;
    		for (tmp = 0; tmp < u4Height; tmp++)
    			{
        		ptr = ptrTemp;
        		for (ramp_type = 0; ramp_type < u4Width; ramp_type++)
            		{
                	if (ramp_type % 2 == 0)
                		{
                    	WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x80, 0x00, 0xff);
                		}
                	else
                		{
                    	WRITE_PIXEL(ptr, u4ColorMode, 0x80, 0x00, 0x80, 0xff);
                		}
            		}
        		ptrTemp += u4OsdPitch;
    			}
			return;
			
		case 45:
			x_memset((void*)pu1Canvas, 0, u4Size);
			GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);	

			for(ramp_type = 0; ramp_type < u4Width; ramp_type++)
				{
				ramp_width = ramp_type;
				if(ramp_width % 2 == 0)
					{
 					GFX_SetColor(0xffffffff);
 					GFX_VLine(ramp_width, 0, u4Height );				
					}
				else{
 					GFX_SetColor(0xff000000);
 					GFX_VLine(ramp_width, 0, u4Height );					
					}
				}
			 GFX_Flush();
			 GFX_Wait();
			return;

			
		case 46:
			x_memset((void*)pu1Canvas, 0, u4Size);
			GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);	

			tmp = 0;
			ramp_type = 0;
			
			for(ramp_width = 0; ramp_width < u4Width; ramp_width++)
				{
				if( 2 == tmp)
					{
					ramp_type ^= 0xffffffff;
					tmp = 0;
					}
				if(ramp_type == 0)
					{
					GFX_SetColor(0xffffffff);
					GFX_VLine(ramp_width, 0, u4Height ); 			
					}
				else{
					GFX_SetColor(0xff000000);
					GFX_VLine(ramp_width, 0, u4Height ); 				
					}
				tmp++;
				}
			GFX_Flush();
			GFX_Wait();
			return;

		case 47:
			x_memset((void*)pu1Canvas, 0, u4Size);
			GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);	

			ramp_Heidht = 0;
			for(ramp_type = 0; ramp_type < u4Height; ramp_type++)
				{
				ramp_Heidht = ramp_type;
				
				if(ramp_type % 2 == 0)
					{
 					GFX_SetColor(0xffffffff);
 					GFX_HLine(0, ramp_Heidht, u4Width );				
					}
				else{
 					GFX_SetColor(0xff000000);
 					GFX_HLine(0, ramp_Heidht, u4Width );					
					}

				//ramp_Heidht += u4OsdPitch;
				}
			 GFX_Flush();
			 GFX_Wait();
			return;

		default:
			break;
		}
	return;
}


void OSD_DrawPattern9(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode)
{
    UINT32 tmp, ramp_type;
    UINT8 *ptr;
    UINT8 *ptrTemp;
    UINT32  u4OsdPitch, u4Size, u4Bpp;

    CHECK_COLOR_MODE(u4ColorMode);

    if (pu1Canvas == NULL)
    {
        return;
    }
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
    u4Size = u4Width * u4Height * u4Bpp;
    u4OsdPitch = u4Bpp * u4Width;
    u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;
    x_memset((void*)pu1Canvas, 0, u4Size);

    ptrTemp = pu1Canvas;
    ptr = pu1Canvas;
    for (tmp = 0; tmp < u4Height; tmp++)
    {
        ptr = ptrTemp;
        if ((tmp % 4 == 0) ||(tmp % 4 == 1))
    {
        for (ramp_type = 0; ramp_type < u4Width; ramp_type++)
            {
                if (ramp_type % 2 == 0)
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x80, 0x00, 0xff);
                }
                else
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x80, 0x00, 0x80, 0xff);
                }
            }
        }
        else
        {
            for (ramp_type = 0; ramp_type < u4Width; ramp_type++)
            {
                if (ramp_type % 2 == 0)
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x80, 0x00, 0x80, 0xff);
                }
                else
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x80, 0x00, 0xff);
                }
            }
        }
        ptrTemp += u4OsdPitch;
    }
}

void OSD_DrawPattern10(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode)
{
    UINT32 tmp, ramp_type;
    UINT8 *ptr;
    UINT8 *ptrTemp;
    UINT32  u4OsdPitch, u4Size, u4Bpp;

    CHECK_COLOR_MODE(u4ColorMode);

    if (pu1Canvas == NULL)
    {
        return;
    }
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
    u4Size = u4Width * u4Height * u4Bpp;
    u4OsdPitch = u4Bpp * u4Width;
    u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;
    x_memset((void*)pu1Canvas, 0, u4Size);

    ptrTemp = pu1Canvas;
    ptr = pu1Canvas;
    for (tmp = 0; tmp < u4Height; tmp++)
    {
        ptr = ptrTemp;
   
        for (ramp_type = 0; ramp_type < u4Width; ramp_type++)
            {
                if (ramp_type % 2 == 0)
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x00, 0x00, 0xff);
                }
                else
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x80, 0x00, 0xff);
                }
            }
        
       
        ptrTemp += u4OsdPitch;
    }
}


void OSD_DrawPattern11(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode)
{
    UINT32 tmp, ramp_type;
    UINT8 *ptr;
    UINT8 *ptrTemp;
    UINT32  u4OsdPitch, u4Size, u4Bpp;

    CHECK_COLOR_MODE(u4ColorMode);

    if (pu1Canvas == NULL)
    {
        return;
    }
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
    u4Size = u4Width * u4Height * u4Bpp;
    u4OsdPitch = u4Bpp * u4Width;
    u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;
    x_memset((void*)pu1Canvas, 0, u4Size);

    ptrTemp = pu1Canvas;
    ptr = pu1Canvas;
    for (tmp = 0; tmp < u4Height; tmp++)
    {
        ptr = ptrTemp;
         if (tmp % 2 == 0) 
           {
             for (ramp_type = 0; ramp_type < u4Width; ramp_type++)
             {
                if (ramp_type % 2 == 0)
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x00, 0x00, 0xff);
                }
                else
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x80, 0x00, 0xff);
                }
             }
         	}
		 else
           {
             for (ramp_type = 0; ramp_type < u4Width; ramp_type++)
             {
                if (ramp_type % 2 == 0)
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x80, 0x00, 0xff);
                }
                else
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x00, 0x00, 0xff);
                }
             }
         	}
        
       
        ptrTemp += u4OsdPitch;
    }
}


void OSD_DrawPattern12(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode)
{
    UINT32 tmp, ramp_type;
    UINT8 *ptr;
    UINT8 *ptrTemp;
    UINT32  u4OsdPitch, u4Size, u4Bpp;

    CHECK_COLOR_MODE(u4ColorMode);

    if (pu1Canvas == NULL)
    {
        return;
    }
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
    u4Size = u4Width * u4Height * u4Bpp;
    u4OsdPitch = u4Bpp * u4Width;
    u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;
    x_memset((void*)pu1Canvas, 0, u4Size);

    ptrTemp = pu1Canvas;
    ptr = pu1Canvas;
    for (tmp = 0; tmp < u4Height; tmp++)
    {
        ptr = ptrTemp;
         if ((tmp % 4 == 0) ||(tmp % 4 == 1))
           {
             for (ramp_type = 0; ramp_type < u4Width; ramp_type++)
             {
                if (ramp_type % 2 == 0)
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x00, 0x00, 0xff);
                }
                else
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x80, 0x00, 0xff);
                }
             }
         	}
		 else
           {
             for (ramp_type = 0; ramp_type < u4Width; ramp_type++)
             {
                if (ramp_type % 2 == 0)
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x80, 0x00, 0xff);
                }
                else
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x00, 0x00, 0xff);
                }
             }	
        }
       
        ptrTemp += u4OsdPitch;
    }
}


void OSD_DrawPattern13(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode)
{
    UINT32 tmp, ramp_type;
    UINT8 *ptr;
    UINT8 *ptrTemp;
    UINT32  u4OsdPitch, u4Size, u4Bpp;
	BOOL inv_type;

    CHECK_COLOR_MODE(u4ColorMode);

    if (pu1Canvas == NULL)
    {
        return;
    }
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
    u4Size = u4Width * u4Height * u4Bpp;
    u4OsdPitch = u4Bpp * u4Width;
    u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;
    x_memset((void*)pu1Canvas, 0, u4Size);

    ptrTemp = pu1Canvas;
    ptr = pu1Canvas;
	inv_type = TRUE;
    for (tmp = 0; tmp < u4Height; tmp++)
    {
        ptr = ptrTemp;
		if( (tmp % 2) )
		{
			inv_type = (!inv_type)
		}
		if (inv_type)
        {
             for (ramp_type = 0; ramp_type < u4Width; ramp_type++)
             {
                if (ramp_type % 2 == 0)
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x80, 0x00, 0xff);
                }
                else
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x00, 0x00, 0xff);
                }
             }
         	}
		 else
           {
             for (ramp_type = 0; ramp_type < u4Width; ramp_type++)
             {
                if (ramp_type % 2 == 0)
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x00, 0x00, 0xff);
                }
                else
                {
                    WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x80, 0x00, 0xff);
                }
             }	
        }
       
        ptrTemp += u4OsdPitch;
    }
}

void OSD_DrawPattern15(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode)

{

	UINT32	u4OsdPitch, u4Size, u4Bpp;

	CHECK_COLOR_MODE(u4ColorMode);
	if (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT8)
		{
		Printf("draw pattern not support OSD_CM_RGB_CLUT8");
		return;
		}
	if( u4Width < 1116 || u4Height < 611)
		{
		Printf("Width or Height not enough \n");
		return;		
		}
	
	// allocate memory for gfx canvas
	OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
	u4Size = u4Width * u4Height * u4Bpp;
	u4OsdPitch = u4Bpp * u4Width;
	u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;

	x_memset((void*)pu1Canvas, 0, u4Size);
	GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);
	
	GFX_SetColor(0xff7e0000);
	GFX_Fill(275,600, 1, 1);
	GFX_Flush();
	GFX_Wait();

	GFX_SetColor(0xff007e00);
	GFX_Fill(290,600, 1, 1);
	GFX_Flush();
	GFX_Wait();

	GFX_SetColor(0xff00007e);
	GFX_Fill(305,600, 1, 1);
	GFX_Flush();
	GFX_Wait();

	GFX_SetColor(0xff5d0000);
	GFX_Fill(660,600, 1, 1);
	GFX_Flush();
	GFX_Wait();

	GFX_SetColor(0xff004900);
	GFX_Fill(675,600, 1, 1);
	GFX_Flush();
	GFX_Wait();

	GFX_SetColor(0xff00007d);
	GFX_Fill(690,600, 1, 1);
	GFX_Flush();
	GFX_Wait();

	GFX_SetColor(0xff500000);
	GFX_Fill(1055,600, 1, 1);
	GFX_Flush();
	GFX_Wait();

	GFX_SetColor(0xff003c00);
	GFX_Fill(1075,600, 1, 1);
	GFX_Flush();
	GFX_Wait();

	GFX_SetColor(0xff000075);
	GFX_Fill(1095,600, 1, 1);
	
	GFX_Flush();
	GFX_Wait();
		return;


}

void OSD_DrawPatternL92(
    UINT8*              pu1Canvas,
    UINT32              u4Width,
    UINT32              u4Height,
    UINT32              u4ColorMode,
    UINT8               ui1_grid_width,
    UINT8               ui1_grid_heigh,
    UINT32              u4BgColor)
{
    UINT32  u4Size          = 0x0;
    UINT32  u4Bpp           = 0x0;
    UINT32  u4OsdPitch      = 0x0;
    //UINT8   ui1_grid_width  = 0x30;
    //UINT8   ui1_grid_heigh  = 0x20;
    UINT8   ui1_grid_row    = 0x3;
    UINT8   ui1_grid_col    = 0x3;
    UINT8   ui1_div_num     = 0x6;
    UINT32  u4_div_h        = 0x0;
    UINT32  u4_div_w        = 0x0;
    UINT32  u4_start_x      = 0x0;
    UINT32  u4_start_y      = 0x0;
    UINT32  u4_x            = 0x0;
    UINT32  u4_y            = 0x0;
    UINT32  u4_row_idx      = 0x0;
    UINT32  u4_col_idx      = 0x0;
    UINT32  u4Color         = u4BgColor;

    if ( (0x0 == u4Width) || (0x0 == u4Height) )
    {
        Printf("region is null \n");
        return;
    }

    CHECK_COLOR_MODE(u4ColorMode);
    if (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT8)
    {
        Printf("draw pattern not support OSD_CM_RGB_CLUT8");
        return;
    }

    if (0x0 == ui1_grid_width)
    {
        ui1_grid_width = 0x30;
    }

    if (0x0 == ui1_grid_heigh)
    {
        ui1_grid_heigh = 0x30;
    }

    u4_div_h    = (u4Height/ui1_div_num);
    u4_div_w    = (u4Width/ui1_div_num);
    u4_start_x  = (u4_div_w - ui1_grid_width);
    u4_start_y  = (u4_div_h - ui1_grid_heigh);
    u4_x        = u4_start_x;
    u4_y        = u4_start_y;

    Printf("PatternL92(w=%d,h=%d),BgColor=0x%x,grid(w=%d,h=%d),start(x=%d,y=%d) \n",
           u4Width,u4Height,u4Color,ui1_grid_width,ui1_grid_heigh,u4_start_x,u4_start_y);


    // allocate memory for gfx canvas
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
    u4Size = u4Width * u4Height * u4Bpp;
    u4OsdPitch = u4Bpp * u4Width;

    u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;

    x_memset((void*)pu1Canvas, 0, u4Size);
    // draw gfx canvas
    GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);

    GFX_SetColor(u4Color);
    GFX_Fill(0, 0, u4Width, u4Height);
    /* draw gride */
    GFX_SetColor(0xffffffff);

    do
    {
        for (u4_row_idx = 0x0; u4_row_idx < ui1_grid_row; u4_row_idx++)
        {

            GFX_HLine(u4_x, u4_y, ui1_grid_width);
            GFX_HLine(u4_x, (u4_y + ui1_grid_heigh), ui1_grid_width);
            GFX_VLine(u4_x, u4_y, ui1_grid_heigh);
            GFX_VLine((u4_x + ui1_grid_width), u4_y, ui1_grid_heigh);
            u4_y = u4_y + (2*u4_div_h);
        }

        u4_x        = u4_x + (2*u4_div_w);
        u4_y        = u4_start_y;
    }
    while (u4_col_idx++ < ui1_grid_col);

    GFX_Flush();
    GFX_Wait();
}

void OSD_DrawPatternGomi(
    UINT8*              pu1Canvas,
    UINT32              u4Width,
    UINT32              u4Height,
    UINT32              u4ColorMode,
    UINT32              u4BgColor,
    UINT8               ui1_mode)
{

    UINT32  u4OsdPitch, u4Size, u4Bpp;

    CHECK_COLOR_MODE(u4ColorMode);
    if (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT8)
    {
        Printf("draw pattern not support OSD_CM_RGB_CLUT8");
        return;
    }

    // allocate memory for gfx canvas
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
    u4Size = u4Width * u4Height * u4Bpp;
    u4OsdPitch = u4Bpp * u4Width;
    u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;

    x_memset((void*)pu1Canvas, 0, u4Size);
    // draw gfx canvas
    GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);

    GFX_SetColor(u4BgColor);
    GFX_Fill(0, 0, u4Width, u4Height);

    Printf("Gomi(w=%d,h=%d),BgColor=0x%x \n",
           u4Width,u4Height,u4BgColor);

    GFX_SetColor(0xff000000);
    if (0x0 == ui1_mode)
    {
        GFX_Fill(0, 0, u4Width, (u4Height/4));
        GFX_Fill(0, (u4Height/2), u4Width, (u4Height/4));
    }
    else
    {
        GFX_Fill(0, (u4Height/4), u4Width, (u4Height/4));
        GFX_Fill(0, (u4Height - (u4Height/4)), u4Width, (u4Height/4));
    }

    GFX_Flush();
    GFX_Wait();
}
void OSD_DrawPatternCrosstalk(
    UINT8*              pu1Canvas,
    UINT32              u4Width,
    UINT32              u4Height,
    UINT32              u4ColorMode,
    UINT32              u4BgColor)
{
    UINT32  u4OsdPitch, u4Size, u4Bpp;

    CHECK_COLOR_MODE(u4ColorMode);
    if (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT8)
    {
        Printf("draw pattern not support OSD_CM_RGB_CLUT8");
        return;
    }

    // allocate memory for gfx canvas
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
    u4Size = u4Width * u4Height * u4Bpp;
    u4OsdPitch = u4Bpp * u4Width;
    u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;

    x_memset((void*)pu1Canvas, 0, u4Size);
    // draw gfx canvas
    GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);

    GFX_SetColor(u4BgColor);
    GFX_Fill(0, 0, u4Width, u4Height);

    Printf("Gomi(w=%d,h=%d),BgColor=0x%x \n",
           u4Width,u4Height,u4BgColor);

    GFX_SetColor(0xffffffff);
    GFX_Fill((u4Width/4), (u4Height/3), (u4Width/2), (u4Height/3));

    GFX_Flush();
    GFX_Wait();
}

void OSD_DrawPatternFlicker(
    UINT8*              pu1Canvas,
    UINT32              u4Width,
    UINT32              u4Height,
    UINT32              u4ColorMode,
    UINT32              u4BgColor)
{
    UINT32 tmp, ramp_type;
    UINT8 *ptr;
    UINT8 *ptrTemp;
    UINT32  u4OsdPitch, u4Size, u4Bpp;

    UINT8 ui1_a = 0x0;
    UINT8 ui1_r = 0x0;
    UINT8 ui1_g = 0x0;
    UINT8 ui1_b = 0x0;

    CHECK_COLOR_MODE(u4ColorMode);

    if (pu1Canvas == NULL)
    {
        return;
    }

    OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
    u4Size = u4Width * u4Height * u4Bpp;
    u4OsdPitch = u4Bpp * u4Width;
    u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;
    x_memset((void*)pu1Canvas, 0, u4Size);

    ui1_a = (UINT8)((u4BgColor&0xff000000)>>24);
    ui1_r = (UINT8)((u4BgColor&0xff0000)>>16);
    ui1_g = (UINT8)((u4BgColor&0xff00)>>8);
    ui1_b = (UINT8)(u4BgColor&0xff);

    Printf("Flicker(w=%d,h=%d),BgColor=0x%x,(a=%x,r=%x,g=%x,b=%x) \n",
           u4Width,u4Height,u4BgColor,ui1_a,ui1_r,ui1_g,ui1_b);

    ptrTemp = pu1Canvas;
    ptr = pu1Canvas;
    for (tmp =0; tmp < u4Height; tmp++)
    {
        ptr = ptrTemp;
        for (ramp_type = 0; ramp_type < u4Width; ramp_type++)
        {
            if (ramp_type % 2 == 0)
            {
                WRITE_PIXEL(ptr, u4ColorMode, ui1_r, ui1_g, ui1_b, ui1_a);
            }
            else
            {
                WRITE_PIXEL(ptr, u4ColorMode, 0x00, 0x00, 0x00, 0xff);
            }
        }
        ptrTemp += u4OsdPitch;
    }
}
