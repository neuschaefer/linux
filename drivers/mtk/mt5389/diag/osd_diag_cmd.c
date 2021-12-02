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
 * $RCSfile: osd_diag_cmd.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_diag_cmd.c
 *  This header file includes CLI diagnostic function definitions of OSD driver.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "osd_cmd.h"
#include "osd_drvif.h"
#if 0
#include "osdc_if.h"
#endif
#ifdef __MODEL_slt__
#include "fbm_drvif.h"
#endif
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_rand.h"
#include "x_assert.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"

#define DEFINE_IS_LOG OSD_IsLog
#include "x_debug.h"
#include "x_serial.h"


#ifdef CC_DEBUG
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define CAP_SRC_REGION 0
#define CAP_VIEW_REGION 1
#define CAP_VIEW_LIST 1
#define CAP_ACTIVE_W 640
#define CAP_ACTIVE_H 360
// need 9MB bytes
#define BMP_ADDRESS 0x3500000


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define _DMA_R_INDEX(x, y, base_address) \
    base_address + ((x * 3 + 2) & ~15) + (15 - ((x * 3 + 2) % 16))

#define _DMA_G_INDEX(x, y, base_address) \
    base_address + ((x * 3) & ~15) + (15 - (x * 3 % 16))

#define _DMA_B_INDEX(x, y, base_address) \
    base_address + ((x * 3 + 1) & ~15) + (15 - ((x * 3 + 1) % 16))


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

EXTERN INT32 _OSD_BASE_UpdateHwReg(void);
EXTERN INT32 _OSD_BASE_GetScrnHSize(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetScrnVSize(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetPrgs(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetOsd1VStart(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_SetScrnHStartOsd1(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetScrnHStartOsd2(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_SetScrnHStartOsd3(UINT32 u4Value);
EXTERN INT32 _OSD_BASE_GetScrnHStartOsd1(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetScrnHStartOsd2(UINT32 *pu4Value);
EXTERN INT32 _OSD_BASE_GetScrnHStartOsd3(UINT32 *pu4Value);

EXTERN INT32 _OSD_PLA_SetHFilter(INT32 i4OsdPlane, UINT32 u4Value);
EXTERN INT32 _OSD_PLA_GetReg(UINT32 u4Plane, UINT32 *pOsdPlaneReg);
EXTERN INT32 _OSD_PLA_SetReg(UINT32 u4Plane, const UINT32 *pOsdPlaneReg);
EXTERN INT32 _OSD_PLA_UpdateHwReg(UINT32 u4Plane);
EXTERN INT32 _OSD_RGN_GetAddress(INT32 i4OsdRegion , UINT32 *pu4Addr);
EXTERN INT32 _OSD_SC_GetReg(UINT32 u4Scaler, UINT32 *pOsdScalerReg);
EXTERN INT32 OSD_SwScale2(OSD_SC_UNION_T * scaler_cfg, UINT32 bmp_width,
                          UINT8* src_address, UINT8* dst_address, UINT32 field);
#ifdef CURSOR_SUPPORT
EXTERN INT32 _OSD_CSR_UpdateHwReg(void);
EXTERN INT32 _OSD_CSR_SetVStart(UINT32 u4Value);
EXTERN INT32 _OSD_CSR_SetHStart(UINT32 u4Value);
EXTERN INT32 _OSD_CSR_SetV2x(UINT32 u4Value);
EXTERN INT32 _OSD_CSR_SetH2x(UINT32 u4Value);
EXTERN INT32 _OSD_CSR_SetV4Bit(UINT32 u4Value);
EXTERN INT32 _OSD_CSR_SetH4Bit(UINT32 u4Value);
EXTERN INT32 _OSD_CSR_SetEn(UINT32 u4Value);
EXTERN void _OSD_CSR_SetColor(UINT32 u4Index, UINT32 u4Value);
#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
EXTERN void _OSD_CSR_SetHActive(UINT32 u4Value);
#endif
#endif

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

static UINT8 _OsdGetSerialChar(void);
static INT32 _OsdTestPixelAlpha(INT32 region, INT32 delay);
static INT32 _OsdGenerateRegion(INT32* region);
static void _OsdSort(UINT32* array, UINT32 size);

static INT32 _OsdDiagCmdTest(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdDiagCmdLinklist(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdDiagCmdAlpha(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdDiagCmdColorkey(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdDiagCmdScale(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdDiagCmdRandomScale(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdDiagCmdStep(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdDiagCmdScrnHStart(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdDiagCmdOsdStart(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdDiagCmdHFilter(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdDiagCmdBatch(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdDiagCmdRandom(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdDiagCmdMix(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdDiagCmdInvertEndian(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdDiagCmdCapture(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdDiagCmdBurn(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdDiagCmdPattern(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdDiagCmdMalloc(INT32 i4Argc, const CHAR ** szArgv);
#ifdef CURSOR_SUPPORT
static INT32 _OsdDiagCmdCursor(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdDiagCmdCursorPt(INT32 i4Argc, const CHAR ** szArgv);
#endif


#endif /* CC_DEBUG */
#if defined(CC_MT5368)||defined(CC_MT5396)
#ifdef CC_3D_OSD_SUPPORT
static INT32 _OsdDiagCmd3DLineShift(INT32 i4Argc, const CHAR ** szArgv);

EXTERN INT32 _OSD_3D_SetReg(const UINT32 *p3DBaseReg);
EXTERN INT32 _OSD_3D_UpdateHwReg(void);


EXTERN INT32 _OSD_3D_SetOsd2LShiftEn(UINT32 u4Value);
EXTERN INT32 _OSD_3D_SetPlaneSwitchEn(UINT32 u4Value);
EXTERN INT32 _OSD_3D_SetShutterShiftEn(UINT32 u4Value);
EXTERN INT32 _OSD_3D_SetOsd1LShiftEn(UINT32 u4Value);
EXTERN INT32 _OSD_3D_SetOsd3LShiftEn(UINT32 u4Value);
EXTERN INT32 _OSD_3D_SetCsrLShiftEn(UINT32 u4Value);
EXTERN INT32 _OSD_3D_SetOsd1ShiftValue(UINT32 u4Value);
EXTERN INT32 _OSD_3D_SetOsd2ShiftValue(UINT32 u4Value);
EXTERN INT32 _OSD_3D_SetOsd3ShiftValue(UINT32 u4Value);
EXTERN INT32 _OSD_3D_SetCsrShiftValue(UINT32 u4Value);


EXTERN INT32 _OSD_3D_GetOsd2LShiftEn(UINT32 *pu4Value);
EXTERN INT32 _OSD_3D_GetPlaneSwitchEn(UINT32 *pu4Value);
EXTERN INT32 _OSD_3D_GetShutterShiftEn(UINT32 *pu4Value);
EXTERN INT32 _OSD_3D_GetOsd1LShiftEn(UINT32 *pu4Value);
EXTERN INT32 _OSD_3D_GetOsd3LShiftEn(UINT32 *pu4Value);
EXTERN INT32 _OSD_3D_GetCsrLShiftEn(UINT32 *pu4Value);
EXTERN INT32 _OSD_3D_GetOsd1ShiftValue(UINT32 *pu4Value);
EXTERN INT32 _OSD_3D_GetOsd2ShiftValue(UINT32 *pu4Value);
EXTERN INT32 _OSD_3D_GetOsd3ShiftValue(UINT32 *pu4Value);
EXTERN INT32 _OSD_3D_GetCsrShiftValue(UINT32 *pu4Value);
#endif


#ifdef CURSOR_SUPPORT
EXTERN INT32 _OSD_CSR_UpdateHwReg(void);
EXTERN INT32 _OSD_CSR_SetVStart(UINT32 u4Value);
EXTERN INT32 _OSD_CSR_SetHStart(UINT32 u4Value);
EXTERN INT32 _OSD_CSR_SetV2x(UINT32 u4Value);
EXTERN INT32 _OSD_CSR_SetH2x(UINT32 u4Value);
EXTERN INT32 _OSD_CSR_SetV4Bit(UINT32 u4Value);
EXTERN INT32 _OSD_CSR_SetH4Bit(UINT32 u4Value);
EXTERN INT32 _OSD_CSR_SetEn(UINT32 u4Value);
EXTERN void _OSD_CSR_SetColor(UINT32 u4Index, UINT32 u4Value);
#endif
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static CLI_EXEC_T _arOsdDiagCmdTbl[] =
{
#ifdef CC_DEBUG
    {
        "test", "t", _OsdDiagCmdTest, NULL,
        "test property {rgn#, cmd, start, end, step, delay, [restore]}",
        CLI_GUEST
    },
    {
        "linklist", "l", _OsdDiagCmdLinklist, NULL,
        "multiple region {plane#, rgn#, list_size, delay}", CLI_GUEST
    },
    {
        "alpha", "a", _OsdDiagCmdAlpha, NULL,
        "blending test {plane#, rgn#, delay}", CLI_GUEST
    },
    {
        "colorkey", "c", _OsdDiagCmdColorkey, NULL,
        "colorkey test {rgn#, pixal_step, delay}", CLI_GUEST
    },
    {
        "scale", "s", _OsdDiagCmdScale, NULL,
        "scaling test {scaler#, src_w, src_h, start_w, start_h, end_w, end_h, "
        "delay (dma_address, bmp_width)}", CLI_GUEST
    },
    {
        "random scale", "rs", _OsdDiagCmdRandomScale, NULL,
        "scaling test {scaler#, start_w, start_h, range_w, range_h, delay, "
        "run (dma_address, bmp_width)}", CLI_GUEST
    },
    {
        "scrn_start", "ss", _OsdDiagCmdScrnHStart, NULL,
        "test scrn H start{plane#, rgn#, delay}", CLI_GUEST
    },
    {
        "osd_start", "os", _OsdDiagCmdOsdStart, NULL,
        "test osd H/V start{plane#, rgn#, delay}", CLI_GUEST
    },
    {
        "hfilter", "h", _OsdDiagCmdHFilter, NULL,
        "test interpolation and fifo{plane#, rgn#}", CLI_GUEST
    },
    {
        "batch", "b", _OsdDiagCmdBatch, NULL,
        "batch test all properties{plane#, rgn#}", CLI_GUEST
    },
    {
        "random", "r", _OsdDiagCmdRandom, NULL,
        "random header generator(multi-color+multi region){plane#}",
        CLI_GUEST
    },
    {
        "mix", "m", _OsdDiagCmdMix, NULL,
        "batch test all properties for all color mode, all plane"
        "[plane(1~2)], [colormde(0~14)] (init osd first!)",
        CLI_GUEST
    },
    {
        "endian", "e", _OsdDiagCmdInvertEndian, NULL,
        "invert endian{bmp_address, pixel_num}", CLI_GUEST
    },
    {
        "capture", "cap",  _OsdDiagCmdCapture, NULL,
        "capture osd output {plane#, address, [1:view | 2:compare]}",
        CLI_GUEST
    },
    {
        "step", "step", _OsdDiagCmdStep, NULL,
        "test scaler step {scaler#, type(0:HD, 1:HU, 2:VD, 3:VU), src_w/h, "
        "start_step, end_step, delay}", CLI_GUEST
    },
    {
        "burn", "burn", _OsdDiagCmdBurn, NULL,
        "overnight burn test {scaler#}", CLI_GUEST
    },
    {
        "pattern", "pt", _OsdDiagCmdPattern, NULL,
        "Draw pattern {plane#, pattern(0:V line, 1:H line, 2:border, "
        "3:colorbar), width, height, [scale_width], [scale_height]}",
        CLI_GUEST
    },
    {
        "malloc", "malloc", _OsdDiagCmdMalloc, NULL, "size in bytes",
        CLI_GUEST
    },
#if 0
    {
        "cmodel", "cm", OSDC_Main, NULL,
        "c model", CLI_GUEST
    },
#endif
#ifdef CURSOR_SUPPORT
    {
        "cursor", "csr", _OsdDiagCmdCursor, NULL,
        "test cursor {delay}", CLI_GUEST
    },
    {
        "cursorpt", "csrpt", _OsdDiagCmdCursorPt, NULL,
        "test cursor {delay}", CLI_GUEST
    },
#endif
#if defined(CC_MT5368) || defined(CC_MT5396) 
    #ifdef CC_3D_OSD_SUPPORT
     {
        "3dlshift", "3dshift", _OsdDiagCmd3DLineShift, NULL,
        "test 3d line shift {delay}", CLI_GUEST
    },
    #endif
#endif

#endif /* CC_DEBUG */
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

CLI_MAIN_COMMAND_ITEM(OsdDiag)
{
    "osdt", NULL, NULL, _arOsdDiagCmdTbl,
    "Osd diag", CLI_GUEST
};

#ifdef CC_DEBUG
static BMP_INFO_T _arOsdBmpInfo[] =
{
    {704, 480, BMP_ADDRESS, 0},
    {704, 480, BMP_ADDRESS + 0x20000, 1},
    {704, 480, BMP_ADDRESS + 0x60000, 2},
    {720, 480, BMP_ADDRESS + 0x100000, 4},
    {720, 480, BMP_ADDRESS + 0x1b0000, 5},
    {720, 480, BMP_ADDRESS + 0x260000, 6},
    {704, 480, BMP_ADDRESS + 0x400000, 8},
    {704, 480, BMP_ADDRESS + 0x420000, 9},
    {704, 480, BMP_ADDRESS + 0x460000, 10},
    {720, 480, BMP_ADDRESS + 0x500000, 11},
    {720, 480, BMP_ADDRESS + 0x5b0000, 12},
    {720, 480, BMP_ADDRESS + 0x660000, 13},
    {720, 480, BMP_ADDRESS + 0x710000, 14},
    {0, 0, 0, 0},
};


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDiagCmdTest(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(5);
    INT32 region = StrToInt(szArgv[1]);
    INT32 field = OSD_FindField(szArgv[2]);
    INT32 value = StrToInt(szArgv[3]);
    INT32 limit = StrToInt(szArgv[4]);
    INT32 step = StrToInt(szArgv[5]);
    INT32 delay = StrToInt(szArgv[6]);
    UINT32 orig_rgn;
    OSD_RGN_UNION_T rgn_reg;

    if (field < 0)
    {
        return field;
    }

    if (i4Argc == 8) // store original setting
    {
        _OSD_RGN_GetAddress(region, &orig_rgn);
        x_memcpy((void*)&rgn_reg, (void*)orig_rgn, sizeof(OSD_RGN_UNION_T));
    }

    while (value <= limit)
    {
        if (field == OSD_RGN_POS_X)
        {
            UINT32 out_w, max_w;
            OSD_RGN_Get(region, OSD_RGN_OUT_W, &out_w);
            _OSD_BASE_GetScrnHSize(&max_w);
            if (out_w + value >= max_w)
            {
                OSD_RGN_Set(region, OSD_RGN_OUT_W, max_w - value);
            }
        }
        else if (field == OSD_RGN_POS_Y)
        {
            UINT32 out_h, max_h;
            OSD_RGN_Get(region, OSD_RGN_OUT_H, &out_h);
            _OSD_BASE_GetScrnVSize(&max_h);
            if (out_h + value >= max_h)
            {
                OSD_RGN_Set(region, OSD_RGN_OUT_H, max_h - value);
            }
        }
        else if (field == OSD_RGN_CLIP_H)
        {
/*
            UINT32 bmp_w, out_w;
            OSD_RGN_Get(region, OSD_RGN_BMP_W, &bmp_w);
            OSD_RGN_Get(region, OSD_RGN_OUT_W, &out_w);
            if (bmp_w - value < out_w)
            {
                OSD_RGN_Set(region, OSD_RGN_OUT_W, bmp_w - value);
            }
*/
        }
        else if (field == OSD_RGN_BMP_W)
        {
            UINT32 clip_h, max_w;
            OSD_RGN_Get(region, OSD_RGN_CLIP_H, &clip_h);
            _OSD_BASE_GetScrnHSize(&max_w);
            if (value - clip_h > max_w)
            {
                OSD_RGN_Set(region, OSD_RGN_OUT_W, max_w);
            }
            else
            {
                OSD_RGN_Set(region, OSD_RGN_OUT_W, value - clip_h);
            }
        }
        else if (field == OSD_RGN_BMP_H)
        {
            UINT32 clip_v, max_h;
            OSD_RGN_Get(region, OSD_RGN_CLIP_H, &clip_v);
            _OSD_BASE_GetScrnHSize(&max_h);
            if (value - clip_v > max_h)
            {
                OSD_RGN_Set(region, OSD_RGN_OUT_H, max_h);
            }
            else
            {
                OSD_RGN_Set(region, OSD_RGN_OUT_H, value - clip_v);
            }
        }

        OSD_RGN_Set(region, field, value);

		HalFlushDCache();
        DELAY(delay, value);

        value = value + step;
    }

    if (i4Argc == 8) // restore original setting
    {
        x_memcpy((void*)orig_rgn, (void*)&rgn_reg, sizeof(OSD_RGN_UNION_T));
    }
    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDiagCmdLinklist(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(4);
    INT32 plane;
    GET_PLANE(plane);
    INT32 region = StrToInt(szArgv[2]);
    INT32 list_size = StrToInt(szArgv[3]);
    INT32 delay = StrToInt(szArgv[4]);
    UINT32 address, tmp, rgn_h, i, j;
    UINT32 com_enable, com_alpha;
    const UINT32 mem_step = sizeof(OSD_RGN_UNION_T) + 0x10;
    OSD_RGN_UNION_T* hdr = NULL;
    OSD_PLA_UNION_T plane_reg;
    UINT32 buf, align_buf;

    OSD_RGN_Get(region, OSD_RGN_BMP_H, &tmp);
    rgn_h = tmp;
    buf = (UINT32)x_mem_alloc(list_size * mem_step + 0xf);
    align_buf = (buf + 0xf) & ~0xf;

    _OSD_RGN_GetAddress(region, &address);
    for (i = 0; i < list_size; i++)
    {
        hdr = (OSD_RGN_UNION_T*)(align_buf+mem_step*i);
        x_memcpy((void*)hdr, (void*)address, mem_step);
        hdr->rField.u4NextOsdAddr = ((UINT32)hdr + mem_step) >> 4;
        //hdr->rField.fgNewPalette = (i & 1)?0:1;
        //hdr->rField.u4PaletteAddr = hdr->rField.u4PaletteAddr + i;
        hdr->rField.fgNewPalette = 1;
        hdr->rField.u4PaletteAddr = hdr->rField.u4PaletteAddr;
    }

    // flip plane to the forged region list
    _OSD_PLA_GetReg(plane, (UINT32*)&plane_reg);
    plane_reg.rField.u4HeaderAddr = align_buf >> 4;
    _OSD_PLA_SetReg(plane, (UINT32*)&plane_reg);
    _OSD_PLA_UpdateHwReg(plane);

    for (i = 1; i < list_size; i++)
    {
        OSD_PLA_Enable(plane, FALSE);
        //x_thread_delay(100);
        tmp = rgn_h / (i+1);
        if (tmp<3)
        {
            break;
        }
        // update src/dst height of all active region
        for (j=0; j<=i; j++)
        {
            hdr = (OSD_RGN_UNION_T*)(align_buf+mem_step*j);
            //hdr->rField.u4Ivw = tmp-1;
            hdr->rField.u4Ovw = tmp;
            hdr->rField.u4Ovs = tmp*j;
            //hdr->rField.u4VClip = hdr->rField.u4Ovs;
        }
        // enable previous region
        hdr = (OSD_RGN_UNION_T*)((UINT32)hdr - mem_step);
        hdr->rField.fgNextOsdEn = 1;
        HalFlushInvalidateDCache();
        if (_OsdGetSerialChar() == 'q')
        {
            Printf("value=%d\n", i);
            goto EXIT;
        }
        OSD_PLA_Enable(plane, TRUE);
        x_thread_delay(delay*i);
    }

    // multiple region with different blending mode
    delay = 5;
    for (i = 0; i < list_size; i++)
    {
        hdr = (OSD_RGN_UNION_T*)(align_buf+mem_step*i);
        hdr->rField.u4MixSel = i & 0x3;
    }
    HalFlushInvalidateDCache();

    OSD_PLA_GetCompressMode(plane, &com_enable, &com_alpha);
    if (!com_enable)
    {
        // pixel mode
        if (_OsdTestPixelAlpha(region, delay)!=OSD_CLI_RET_OK)
        {
            goto EXIT;
        }
    }
    // region mode
    for (i = 0; i < 0xff; i++)
    {
        for (j=0; j<list_size; j++)
        {
            hdr = (OSD_RGN_UNION_T*)(align_buf+mem_step*j);
            hdr->rField.u4MixWeight = i;
        }
        HalFlushInvalidateDCache();
        x_thread_delay(delay);
    }
    // plane mode
    for (i = 0; i < 0xff; i++)
    {
        OSD_PLA_SetBlendLevel(plane, i);
        x_thread_delay(delay);
    }
    // global fading
    for (i = 0; i < 0xff; i++)
    {
        OSD_PLA_SetFading(plane, i);
        x_thread_delay(delay);
    }

     // test scaler
    OSD_RGN_Get(region, OSD_RGN_COLORMODE, &tmp);
    if ((plane != OSD_PLANE_1) &&
        ((tmp == OSD_CM_ARGB8888_DIRECT32) ||
         (tmp == OSD_CM_ARGB4444_DIRECT16)))
    {
        UINT32 w, h, max_h;
        tmp = OSD_PLANE_TO_SCALER(plane);
        OSD_RGN_Get(region, OSD_RGN_BMP_W, &w);
        OSD_RGN_Get(region, OSD_RGN_BMP_H, &h);
        _OSD_BASE_GetScrnVSize(&max_h);

        // HV scale down
        //CLI("osdt.s %d %d %d %d %d %d %d 10", tmp, w, h, w, h, 10, h);
        //CLI("osdt.s %d %d %d %d %d %d %d 10", tmp, w, h, w, h, w, max_h/2);
        // HV scale up
        //CLI("osdt.s %d %d %d %d %d %d %d 5", tmp, w, h, w, h, 4096, h);
        //CLI("osdt.s %d %d %d %d %d %d %d 5", tmp, w, h, w, h, w, 4096);
        // H up, V up
        CLI("osdt.s %d %d %d %d %d %d %d 5", tmp, w, h, w, h, 2048, 2048);
        // H down, V up
        //CLI("osdt.s %d %d %d %d %d %d %d 5", tmp, w, h, w, h, w>>1, 4096);

        OSD_SC_Scale(tmp, FALSE, 0, 0, 0, 0);
    }

EXIT:
    // restore plane to original region
    plane_reg.rField.u4HeaderAddr = address >> 4;
    _OSD_PLA_SetReg(plane, (UINT32*)&plane_reg);
    _OSD_PLA_UpdateHwReg(plane);
    x_mem_free((void*)buf);
    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDiagCmdAlpha(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 alpha, com_enable, com_alpha;

    CHK_NUM_MIN_PARAM(3);
    INT32 plane;
    GET_PLANE(plane);
    INT32 region = StrToInt(szArgv[2]);
    INT32 delay = StrToInt(szArgv[3]);

    // plane mode
    OSD_RGN_Set(region, OSD_RGN_MIX_SEL, 3);
    alpha = 0;
    while (alpha <= 0xff)
    {
        OSD_PLA_SetBlendLevel(plane, alpha);
        DELAY(delay, alpha);
        alpha++;
    }
    alpha = 0;
    while (alpha <= 0xff)
    {
        OSD_PLA_SetFading(plane, alpha);
        DELAY(delay, alpha);
        alpha++;
    }

    // region mode
    OSD_RGN_Set(region, OSD_RGN_MIX_SEL, 2);
    alpha = 0;
    while (alpha <= 0xff)
    {
        OSD_RGN_Set(region, OSD_RGN_ALPHA, alpha);
        DELAY(delay, alpha);
        alpha++;
    }
    alpha = 0;
    while (alpha <= 0xff)
    {
        OSD_PLA_SetFading(plane, alpha);
        DELAY(delay, alpha);
        alpha++;
    }

    OSD_PLA_GetCompressMode(plane, &com_enable, &com_alpha);
    if (!com_enable)
    {
        // pixel mode
        if (_OsdTestPixelAlpha(region, delay) == OSD_CLI_RET_OK)
        {
            alpha = 0;
            while (alpha <= 0xff)
            {
                OSD_PLA_SetFading(plane, alpha);
                DELAY(delay, alpha);
                alpha++;
            }
        }
    }

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDiagCmdColorkey(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 color_mode, bmp_address, key=0;

    CHK_NUM_MIN_PARAM(3);
    INT32 region = StrToInt(szArgv[1]);
    INT32 iteration = StrToInt(szArgv[2]);
    INT32 delay = StrToInt(szArgv[3]);

    OSD_RGN_Set(region, OSD_RGN_COLOR_KEY_EN, 1);
    OSD_RGN_Get(region, OSD_RGN_COLORMODE, &color_mode);

    // direct mode
    if ((color_mode >= OSD_CM_CBYCRY422_DIRECT16 &&
        color_mode <= OSD_CM_AYCBCR8888_DIRECT32) ||
        (color_mode >= OSD_CM_RGB565_DIRECT16 &&
        color_mode <= OSD_CM_ARGB8888_DIRECT32))
    {
        UINT32 x, y, w, h, bpp;

        OSD_RGN_Get(region, OSD_RGN_BMP_W, &w);
        OSD_RGN_Get(region, OSD_RGN_BMP_H, &h);
        OSD_RGN_Get(region, OSD_RGN_BMP_ADDR, &bmp_address);
        // argb8888,aybr8888
        if (color_mode == OSD_CM_AYCBCR8888_DIRECT32 ||
            color_mode == OSD_CM_ARGB8888_DIRECT32)
        {
            bpp = 4;
        }
        else
        {
            bpp = 2;
        }
#ifdef CC_MT5381
        for (y = 0; y < h; y += iteration)
#else // 5371
        for (y = (h>>3)*3; y < (h>>3)*5; y += iteration)
#endif
        {
            for (x = iteration; x < w; x += iteration)
            {
                if (bpp == 4)
                {
                    key = *(UINT32*)(bmp_address + (y * w + x) * bpp);
                }
                else
                {
                    if (color_mode == OSD_CM_CBYCRY422_DIRECT16)
                    {
                        //b0y0r0y1 -> y1r0y0b0, bcuz source is BE
                        key = *(UINT32*)(bmp_address + ((y * w + x) >> 1) * 4);
                        if ((x&1)==0) // y0b0r0
                        {
                            key = ((key & 0xffff) << 8) |
                                  ((key & 0xff0000) >> 16);
                        }
                        else // y1b0r0
                        {
                            key = ((key & 0xff000000) >> 8) |
                                  ((key & 0xff) << 8) |
                                  ((key & 0xff0000) >> 16);
                        }
                    }
                    else if (color_mode == OSD_CM_YCBYCR422_DIRECT16)
                    {
                        //y0b0y1r0 -> r0y1b0y0
                        key = *(UINT32*)(bmp_address + ((y * w + x) >> 1) * 4);
                        if ((x & 1) == 0) // y0b0r0
                        {
                            key = ((key & 0xff) << 16) | (key & 0xff00) |
                                  ((key & 0xff000000) >> 24);
                        }
                        else // y1b0r0
                        {
                            key = ((key & 0xff0000)) | (key & 0xff00) |
                                  ((key & 0xff000000) >> 24);
                        }
                    }
                    else
                    {
                        key = *(UINT16*)(bmp_address + (y * w + x) * bpp);
                    }
                }
                OSD_RGN_Set(region, OSD_RGN_COLOR_KEY, key);
                DELAY(delay, key);
            }
        }
    }
    // palette mode, BE
    else if (color_mode <= OSD_CM_YCBCR_CLUT8 ||
             (color_mode >= OSD_CM_RGB_CLUT2 && color_mode<=OSD_CM_RGB_CLUT8))
    {
        UINT32 tmp;
        OSD_RGN_Get(region, OSD_RGN_PAL_LEN, &tmp);
        switch (tmp)
        {
        case 0:
            key = 4;
            break;
        case 1:
            key = 16;
            break;
        case 2:
            key = 256;
            break;
        default: //do nothing
            break;
        }
        do
        {
            OSD_RGN_Set(region, OSD_RGN_COLOR_KEY, key);
            DELAY(delay, key);
        }
        while (--key != 0);
    }

    OSD_RGN_Set(region, OSD_RGN_COLOR_KEY_EN, 0);

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDiagCmdScale(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(8);
    INT32 sc = StrToInt(szArgv[1]);
    INT32 sw = StrToInt(szArgv[2]);
    INT32 sh = StrToInt(szArgv[3]);
    INT32 start_w = StrToInt(szArgv[4]);
    INT32 start_h = StrToInt(szArgv[5]);
    INT32 end_w = StrToInt(szArgv[6]);
    INT32 end_h = StrToInt(szArgv[7]);
    INT32 delay = StrToInt(szArgv[8]);
    INT32 x, y, fail = 0;
    INT32 step = (i4Argc >= 11) ? delay : 1;
    BOOL fgReverse = szArgv[i4Argc - 1][0] == 'r';

    for (x = start_w, y = start_h; x != end_w || y != end_h; )
    {
        if (x < end_w)
        {
            x += step;
        }
        else if (x > end_w)
        {
            x -= step;
        }
        if (y < end_h)
        {
            y += step;
        }
        else if (y > end_h)
        {
            y -= step;
        }
        if (fgReverse)
        {
            OSD_SC_Scale(sc, TRUE, x, y, sw, sh);
        }
        else
        {
            OSD_SC_Scale(sc, TRUE, sw, sh, x, y);
        }
        if (i4Argc >= 11) // capture and compare
        {
            UINT32 address = StrToInt(szArgv[9]);
            UINT32 bmp_width = StrToInt(szArgv[10]);
            if (CLI_Parser_Arg("osdt.cap %d 0x%x 2 %d",
                               sc + 2, address, bmp_width))
            {
                fail++;
            }
        }
        DELAY(delay, x);
    }

    if (i4Argc >= 11)
    {
        if (fail)
        {
            LOG(5, "_OsdDiagCmdScale FAIL=%d\n", fail);
        }
        else
        {
            LOG(5, "_OsdDiagCmdScale PASS\n");
        }
        return fail;
    }

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDiagCmdRandomScale(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(7);
    INT32 sc = StrToInt(szArgv[1]);
    INT32 start_w = StrToInt(szArgv[2]);
    INT32 start_h = StrToInt(szArgv[3]);
    INT32 range_w = StrToInt(szArgv[4]);
    INT32 range_h = StrToInt(szArgv[5]);
    INT32 delay = StrToInt(szArgv[6]);
    INT32 run = StrToInt(szArgv[7]);
    INT32 src_w, src_h, dst_w, dst_h, fail = 0;

    do
    {
        // random generate x and y
        if (range_w == 0)
        {
            src_w = start_w;
            dst_w = start_w;
        }
        else
        {
            src_w = ((UINT32)rand() % range_w) + start_w;
            dst_w = ((UINT32)rand() % range_w) + start_w;
        }
        if (range_h == 0)
        {
            src_h = start_h;
            dst_h = start_h;
        }
        else
        {
            src_h = ((UINT32)rand() % range_h) + start_h;
            dst_h = ((UINT32)rand() % range_h) + start_h;
        }

        CLI("osd.sc.s %d %d %d %d %d", sc, src_w, src_h, dst_w, dst_h);
        if (i4Argc >= 9) // capture and compare
        {
            UINT32 address = StrToInt(szArgv[8]);
            UINT32 bmp_width = StrToInt(szArgv[9]);
            if (CLI_Parser_Arg("osdt.cap %d 0x%x 2 %d",
                               sc + 2, address, bmp_width))
            {
                fail++;
            }
        }
        DELAY(delay, run);
    } while (--run);

    if (i4Argc >= 11)
    {
        if (fail)
        {
            LOG(5, "_OsdDiagCmdRandomScale FAIL=%d\n", fail);
        }
        else
        {
            LOG(5, "_OsdDiagCmdRandomScale PASS\n");
        }
        return fail;
    }

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  only support progressive mode
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDiagCmdStep(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(6);
    INT32 sc = StrToInt(szArgv[1]);
    INT32 type = StrToInt(szArgv[2]);
    INT32 src = StrToInt(szArgv[3]);
    INT32 start_step = StrToInt(szArgv[4]);
    INT32 end_step = StrToInt(szArgv[5]);
    INT32 delay = StrToInt(szArgv[6]);
    INT32 step = (i4Argc>=8)?delay:1;
    INT32 fail = 0;

    for (; start_step >= end_step; start_step -= step)
    {
        switch (type)
        {
        case 0:
            OSD_SC_HDown(sc, src, start_step);
            break;
        case 1:
            OSD_SC_HUp(sc, src, start_step);
            break;
        case 2:
            OSD_SC_VDown(sc, src, start_step);
            break;
        case 3:
            OSD_SC_VUp(sc, src, start_step);
            break;
        default:
            break; // do nothing
        }
        if (i4Argc >= 8) // capture and compare
        {
            UINT32 address = StrToInt(szArgv[7]);
            UINT32 bmp_width = StrToInt(szArgv[8]);
            if (CLI_Parser_Arg("osdt.cap %d 0x%x 2 %d", sc + 2, address,
                               bmp_width))
            {
                fail++;
            }
        }
        DELAY(delay, start_step);
    }

    if (i4Argc>=8)
    {
        if (fail)
        {
            LOG(5, "_OsdDiagCmdStep FAIL=%d\n", fail);
        }
        else
        {
            LOG(5, "_OsdDiagCmdStep PASS\n");
        }
        return fail;
    }

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDiagCmdScrnHStart(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(1);
    INT32 plane;
    GET_PLANE(plane);
    //INT32 region = StrToInt(szArgv[2]);
    INT32 delay = StrToInt(szArgv[3]);
    UINT32 i, hstart;

    if (plane == OSD_PLANE_1)
    {
        _OSD_BASE_GetScrnHStartOsd1(&hstart);
    }
    else if (plane == OSD_PLANE_2)
    {
        _OSD_BASE_GetScrnHStartOsd2(&hstart);
    }

    for (i = 1; i < 0x1ff; i++)
    {
        if (plane == OSD_PLANE_1)
        {
            _OSD_BASE_SetScrnHStartOsd1(i);
        }
        else if (plane == OSD_PLANE_2)
        {
            _OSD_BASE_SetScrnHStartOsd2(i);
        }

        _OSD_BASE_UpdateHwReg();
        DELAY(delay, i);
    }

    if (plane == OSD_PLANE_1)
    {
        _OSD_BASE_SetScrnHStartOsd1(hstart);
    }
    else if (plane == OSD_PLANE_2)
    {
        _OSD_BASE_SetScrnHStartOsd2(hstart);
    }

    _OSD_BASE_UpdateHwReg();

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDiagCmdOsdStart(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(3);
    INT32 plane;
    GET_PLANE(plane);
    INT32 region = StrToInt(szArgv[2]);
    INT32 delay = StrToInt(szArgv[3]);
    UINT32 x, y, end_w, end_h, rgn_w, rgn_h, prgs;

    _OSD_BASE_GetScrnHSize(&end_w);
    _OSD_BASE_GetScrnVSize(&end_h);
    _OSD_BASE_GetPrgs(&prgs);
    if (!prgs)
    {
        end_h <<= 1;
    }
    OSD_RGN_Get(region, OSD_RGN_OUT_W, &rgn_w);
    OSD_RGN_Get(region, OSD_RGN_OUT_H, &rgn_h);

    for (x = 0, y = 0; x != end_w || y != end_h; )
    {
        if (x < end_w)
        {
            x++;
        }
        else if (x > end_w)
        {
            x--;
        }
        if (y < end_h)
        {
            y++;
        }
        else if (y > end_h)
        {
            y--;
        }

        OSD_BASE_SetOsdPosition(plane, x, y);
        if (plane == OSD_PLANE_1)
        {
            OSD_RGN_Set(region, OSD_RGN_OUT_W, MIN(rgn_w, end_w - x));
            OSD_RGN_Set(region, OSD_RGN_OUT_H, MIN(rgn_h, end_h - y));
        }
/*        else
        {
            OSD_SC_Scale(OSD_PLANE_TO_SCALER(plane), TRUE, end_w,
                         (end_h - y) >> 1, end_w - x, end_h - y);
        }*/

        DELAY(delay, x);
    }

    // restore config
    OSD_BASE_SetOsdPosition(plane, 0, 0);
    if (plane == OSD_PLANE_1)
    {
        OSD_RGN_Set(region, OSD_RGN_OUT_W, rgn_w);
        OSD_RGN_Set(region, OSD_RGN_OUT_H, rgn_h);
    }
    else
    {
        OSD_SC_Scale(OSD_PLANE_TO_SCALER(plane), FALSE, 0, 0, 0, 0);
    }

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDiagCmdHFilter(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(2);
    INT32 plane;
    GET_PLANE(plane);
    INT32 region = StrToInt(szArgv[2]);

    UINT32 out_w, i, color_mode;
    // 1/4, 3/8, 1/2, 2/3, 3/4, 1.5, 2, 3, 4, 8
    UINT32 step_array[] = {0x4000, 0x2aaa, 0x2000, 0x1800, 0x1555, 0xaaa, 0x800,
                           0x555, 0x400, 0x200};
    UINT32 num = sizeof(step_array)/sizeof(INT32);

    OSD_RGN_Get(region, OSD_RGN_COLORMODE, &color_mode);
    OSD_RGN_Get(region, OSD_RGN_DISP_W, &out_w);
    for (i = 0; i < num; i++)
    {
        UINT32 step = step_array[i] ;
        Printf("step=0x%x\n", step);

        if (step > 0x1000) // scale down
        {
            OSD_RGN_Set(region, OSD_RGN_DISP_W, out_w * 0x1000 / step);
        }
        else
        {
            OSD_RGN_Set(region, OSD_RGN_OUT_W, out_w);
            OSD_RGN_Set(region, OSD_RGN_STEP_H, step);
        }

        Printf("\tdisable H filter\n");
        OSD_PLA_SetHFilter(plane, FALSE);
        DELAY(1000, step);
        Printf("\tenable H filter\n");
        OSD_PLA_SetHFilter(plane, TRUE);


        DELAY(1000, step);

        if (plane != OSD_PLANE_1) // test scaler
        {
            UINT32 scaler = OSD_PLANE_TO_SCALER(plane);
            // restore original size
            OSD_RGN_Set(region, OSD_RGN_DISP_W, out_w);
            Printf("\tuse scaler\n");
            OSD_SC_Scale(scaler, TRUE, out_w, 0, out_w * 0x1000 / step, 0);
            DELAY(1000, step);
            Printf("\tuse scaler + lpf\n");
            OSD_SC_SetLpf(scaler, 1);
            DELAY(1000, step);
            OSD_SC_Scale(scaler, FALSE, 0, 0, 0, 0);
            OSD_SC_SetLpf(scaler, 0);
        }
    }

    OSD_RGN_Set(region, OSD_RGN_DISP_W, out_w);

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDiagCmdBatch(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(2);
    INT32 plane;
    GET_PLANE(plane);
    INT32 region = StrToInt(szArgv[2]);
    INT32 ret;
    UINT32 tmp, w, h, max_w, max_h, color_mode, com_enable, com_alpha;

    OSD_RGN_Get(region, OSD_RGN_BMP_W, &w);
    OSD_RGN_Get(region, OSD_RGN_BMP_H, &h);
    OSD_RGN_Get(region, OSD_RGN_COLORMODE, &color_mode);
    OSD_PLA_GetCompressMode(plane, &com_enable, &com_alpha);

    if (i4Argc == 3)
    {
        _OSD_BASE_GetScrnHSize(&max_w);
        _OSD_BASE_GetScrnVSize(&max_h);

        // dst size
        CLI_RET("osdt.test %d pos_x 0 %d 1 20 1", region,
                (w > max_w)?max_w:w);
        CLI_RET("osdt.test %d pos_y 0 %d 1 20 1", region,
                (h > max_h)?max_h:h);
        CLI_RET("osdt.test %d out_w 0 %d 1 20 1", region,
                (w > max_w)?max_w:w);
        CLI_RET("osdt.test %d out_h 0 %d 1 20 1", region,
                (w > max_h)?max_h:h);

        // src_size
        CLI_RET("osdt.test %d clip_h 0 %d 1 20 1", region, w - 1);

        CLI_RET("osdt.test %d clip_v 0 %d 1 20 1", region, h - 1);
        CLI_RET("osdt.test %d src_w 0 %d 1 20 1", region, w);
        CLI_RET("osdt.test %d src_h 0 %d 1 20 1", region, h);

        // compare differents w/o hfilter
        CLI_RET("osdt.h %d %d", plane + 1, region);

        if (plane == OSD_PLANE_1) // test old scaler
        {
            // H-up
            OSD_PLA_SetHFilter(plane, FALSE);
            Printf("scale up without H filter\n");
            CLI_RET("osdt.test %d step_h 0 0x1000 0xf 20 1", region);
            OSD_PLA_SetHFilter(plane, TRUE);
            Printf("scale up with H filter\n");
            CLI_RET("osdt.test %d step_h 0 0x1000 0xf 20 1", region);

            // H-down
            OSD_PLA_SetHFilter(plane, FALSE);
            Printf("scale down without H filter\n");
            CLI_RET("osdt.test %d disp_w %d %d 1 20 1", region, w>>3, w);
            OSD_PLA_SetHFilter(plane, TRUE);
            Printf("scale down with H filter\n");
            CLI_RET("osdt.test %d disp_w %d %d 1 20 1", region, w>>3, w);

            // V-up
            OSD_PLA_SetVFilter(plane, FALSE);
            Printf("scale up without V filter\n");
            CLI_RET("osdt.test %d step_v 0 0x1000 0xf 20 1", region);
            OSD_PLA_SetVFilter(plane, TRUE);
            Printf("scale up with V filter\n");
            CLI_RET("osdt.test %d step_v 0 0x1000 0xf 20 1", region);

            // V-down
            OSD_PLA_SetVFilter(plane, FALSE);
            Printf("scale down without V filter\n");
            CLI_RET("osdt.test %d disp_h %d %d 1 20 1", region, h>>2, h);
            OSD_PLA_SetVFilter(plane, TRUE);
            Printf("scale down with V filter\n");
            CLI_RET("osdt.test %d disp_h %d %d 1 20 1", region, h>>2, h);
        }

        if (plane != OSD_PLANE_1)
        {
            // blending, colorkey, linklist
            CLI_RET("osdt.a %d %d 5", plane + 1, region);
            if (!com_enable)
            {
                CLI_RET("osdt.c %d 16 25", region);
            }
            CLI_RET("osdt.l %d %d 8 25", plane + 1, region);
        }

        // osd start
        if ((color_mode == OSD_CM_ARGB8888_DIRECT32) ||
            (color_mode == OSD_CM_ARGB4444_DIRECT16))
        {
            CLI_RET("osdt.os %d %d 20", plane + 1, region);
        }

        // bmp address
        OSD_RGN_Get(region, OSD_RGN_BMP_ADDR, &tmp);
        CLI_RET("osdt.test %d src_addr 0x%x 0x%x 16 250 1",
                region, tmp, tmp + 0xf0);
        OSD_RGN_Set(region, OSD_RGN_STEP_V, 0x200);
        OSD_RGN_Set(region, OSD_RGN_STEP_H, 0x200);
        if (plane == OSD_PLANE_1)
        {
            CLI_RET("osdt.test %d int_v 0 8 1 1000 1", region);
            CLI_RET("osdt.test %d int_h 0 8 1 1000 1", region);
            CLI_RET("osdt.test %d hpx_ext 0 2 1 1000 1", region);
            CLI_RET("osdt.test %d endian 0 2 1 1000 1", region);
            CLI_RET("osdt.test %d step_ho 0 0xff 0x1 20 1", region);
            CLI_RET("osdt.test %d step_vo 0 0xfff 0xf 20 1", region);
        }
        CLI_RET("osdt.test %d src_addr 0x%x 0x%x 16 250 1",
                region, tmp, tmp + 0xf0);
        OSD_RGN_Set(region, OSD_RGN_STEP_V, 0x1000);
        OSD_RGN_Set(region, OSD_RGN_STEP_H, 0x1000);
        CLI_RET("osd.r.set %d vflip 1", region);
        x_thread_delay(1000);
        CLI_RET("osd.r.set %d hmirror 1", region);
        x_thread_delay(1000);
        CLI_RET("osd.r.set %d vflip 0", region);
        x_thread_delay(1000);
        CLI_RET("osd.r.set %d hmirror 0", region);

        // palette address
        OSD_RGN_Get(region, OSD_RGN_COLORMODE, &tmp);
        if ((color_mode <= OSD_CM_YCBCR_CLUT8) ||
            ((color_mode >= OSD_CM_RGB_CLUT2) &&
             (color_mode <= OSD_CM_RGB_CLUT8))) // palette mode, BE
        {
            OSD_RGN_Get(region, OSD_RGN_PAL_ADDR, &tmp);
            CLI_RET("osdt.test %d pal_addr 0x%x 0x%x 16 250 1",
                    region, tmp, tmp + 0xf0);
        }
    }
    if (plane != OSD_PLANE_1) // test scaler
    {
        // only do scaling test for argb8888 and argb4444 mode
        UINT32 state = 2;
        tmp = OSD_PLANE_TO_SCALER(plane);

        if (color_mode == OSD_CM_ARGB8888_DIRECT32) // argb8888
        {
            state = 1;
        }
        else if (color_mode == OSD_CM_ARGB4444_DIRECT16) // argb4444
        {
            state = 1;
        }

        while (TRUE)
        {
            if (state == 0)
            {
                CLI("osd.sc.l %d 0", tmp); //disable lpf
                state = 1;
            }
            else if (state == 1)
            {
                CLI("osd.sc.l %d 1", tmp); //enable lpf
                state = 2;
            }
            else
            {
                CLI("osd.sc.l %d 0", tmp); //disable lpf
                break;
            }

            if (color_mode == OSD_CM_ARGB8888_DIRECT32) // argb8888
            {
                // HV scale down
                CLI_RET("osdt.s %d %d %d %d %d %d %d 20",
                        tmp, w, h, w, h, max_w >> 2, h);
                CLI_RET("osdt.s %d %d %d %d %d %d %d 20",
                          tmp, w, h, w, h, w, max_h >> 1);
                CLI_RET("osdt.s %d %d %d %d %d %d %d 20",
                          tmp, w, h, w, h, 10, max_h >> 1);
            }
            // HV scale up
            CLI_RET("osdt.s %d %d %d %d %d %d %d 20",
                    tmp, w, h, w, h, 4096, h);
            CLI_RET("osdt.s %d %d %d %d %d %d %d 20",
                    tmp, w, h, w, h, w, 2048);
            CLI_RET("osdt.s %d %d %d %d %d %d %d 20",
                    tmp, w, h, w, h, 2048, 2048);
/*
            // V down, H up
            CLI_RET("osdt.s %d %d %d %d %d %d %d 20",
                      tmp, w, h, w, h, 4096, max_h >> 1);
            // H down, V up
            CLI_RET("osdt.s %d %d %d %d %d %d %d 20",
                    tmp, w, h, w, h, w >> 1, 4096);
*/
            OSD_SC_Scale(tmp, FALSE, 0, 0, 0, 0);
        }

        if (color_mode == OSD_CM_ARGB8888_DIRECT32)
        {
/*
            // H down
            OSD_SC_Scale(tmp, FALSE, 0, 0, 0, 0);
            CLI_RET("osdt.step %d 0 %d 0x3fff 0x10 5", tmp, w);
*/
            // H up
            OSD_SC_Scale(tmp, TRUE, w >> 2, 0, w >> 2, 0);
            CLI_RET("osdt.step %d 1 %d 0x3fff 0x10 5", tmp, w >> 2);
/*
            // V down
            OSD_SC_Scale(tmp, FALSE, 0, 0, 0, 0);
            OSD_BASE_SetClock(OSD_CK_SYS);
            CLI_RET("osdt.step %d 2 %d 0x3fff 0x2000 5", tmp, h);
            OSD_BASE_SetClock(OSD_CK_OCLK);
*/
            // V up
            OSD_SC_Scale(tmp, TRUE, 0, h >> 2, 0, h >> 2);
            CLI_RET("osdt.step %d 3 %d 0x3fff 0x10 5", tmp, h >> 2);
        }

        OSD_SC_Scale(tmp, FALSE, 0, 0, 0, 0);
    }

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdGenerateRegion(INT32* region)
{
    UINT32 color_mode, ret, x, w, max_w;
    BMP_INFO_T* ptr_bmp_info = _arOsdBmpInfo;

    color_mode = OSD_CM_RESERVED_0;
    while (color_mode==OSD_CM_RESERVED_0 || color_mode==OSD_CM_RESERVED_1)
        color_mode = (UINT32)rand() % OSD_CM_ALPHA_BIT;
    while (ptr_bmp_info->u4Width > 0 && ptr_bmp_info->u4ColorMode!=color_mode)
        ptr_bmp_info++;
    ASSERT(ptr_bmp_info->u4Width > 0);

    ret = OSD_RGN_Create((UINT32*)region, ptr_bmp_info->u4Width,
                         ptr_bmp_info->u4Height, (void*)ptr_bmp_info->u4Address,
                         color_mode, 0, 0, 0, ptr_bmp_info->u4Width,
                         ptr_bmp_info->u4Height);

    if (ret != OSD_RET_OK)
        return ret;

    if (color_mode <= OSD_CM_RGB_CLUT8 ||
        color_mode == OSD_CM_ARGB8888_DIRECT32)
        OSD_RGN_SetBigEndian(*region, 1);

    _OSD_BASE_GetScrnHSize(&max_w);
    //max_w = ptr_bmp_info->u4Width;
    x = (UINT32)rand() % max_w;
    w = 1 + (UINT32)rand() % ptr_bmp_info->u4Width;
    if (x+w>max_w)
        w = max_w - x;
    OSD_RGN_Set(*region, OSD_RGN_POS_X, x);
    if (ptr_bmp_info->u4Width > w)
    {
        OSD_RGN_Set(*region, OSD_RGN_CLIP_H,
                    (UINT32)rand() % (ptr_bmp_info->u4Width - w));
    }
    OSD_RGN_Set(*region, OSD_RGN_OUT_W, w);
    OSD_RGN_Set(*region, OSD_RGN_STEP_H, ((UINT32)rand() % 0x2000) + 1); // prevent divide by 0
    OSD_RGN_Set(*region, OSD_RGN_ALPHA, (UINT32)rand() % 0xc0 + 0x40);
    OSD_RGN_Set(*region, OSD_RGN_MIX_SEL, (UINT32)rand() % 4);
    if (((UINT32)rand() % 5) == 0)
    {
        OSD_RGN_Set(*region, OSD_RGN_COLOR_KEY_EN, TRUE);
    }

    if (((UINT32)rand() % 5) == 0)
    {
        OSD_RGN_Set(*region, OSD_RGN_V_FLIP, TRUE);
    }
    if (((UINT32)rand() % 5) == 0)
    {
        OSD_RGN_Set(*region, OSD_RGN_H_MIRROR, TRUE);
    }

    return OSD_RET_OK;
}

static INT32 _OsdDiagCmdRandom(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(1);
    INT32 plane, region;
    INT32 list = 0;
    GET_PLANE(plane);
    UINT32 max_h, total_rgn_height = 0, y, h, i, ret;
    UINT32 region_v_start[OSD_MAX_NUM_RGN];

    CLI_Parser("osd.init");

    _OSD_BASE_GetScrnVSize(&max_h);
    if (i4Argc == 3) // fix region number
    {
        UINT32 region_num = StrToInt(szArgv[2]);
        if (region_num > OSD_MAX_NUM_RGN)
            return OSD_CLI_RET_ERR_REGION;
        x_memset(region_v_start, 0, sizeof(region_v_start));
        for (i = 0; i < region_num; i++)
            region_v_start[i] = (UINT32)rand() % max_h;
        _OsdSort(region_v_start, region_num);
        for (i = 0; i < region_num; i++)
        {
            if ((ret = _OsdGenerateRegion(&region)) != OSD_RET_OK)
            {
                LOG(1, "_OsdGenerateRegion fail, ret=%d\n", ret);
                return OSD_CLI_RET_ERR_GENERAL;
            }
            y = region_v_start[i];

            // method1: get larger height
            h = 1 + (UINT32)rand() % max_h;
            if ((i != region_num - 1) && (y + h > region_v_start[i+1]))
            {
                h = region_v_start[i+1] - y;
            }
            else if ((i == region_num - 1) && (y + h > max_h))
            {
                h = DIFF(max_h, y); // y could exceed max_h
            }

            // method2: pure random, height is often small, not easy to observe
/*            if (i!=region_num-1) // not the last one
                h = 1 + (UINT32)rand() % (region_v_start[i+1] - y);
            else
                h = 1 + (UINT32)rand() % (max_h - y);*/

            OSD_RGN_Set(region, OSD_RGN_POS_Y, y);
            OSD_RGN_Set(region, OSD_RGN_CLIP_V, y);
            OSD_RGN_Set(region, OSD_RGN_OUT_H, h);
            OSD_RGN_Set(region, OSD_RGN_STEP_V, ((UINT32)rand() % 0x2000) + 1); // prevent divide by 0
            OSD_RGN_Dump(region);
            OSD_RGN_Insert(region, list);
        }
    }
    else // random region number
    {
        while (total_rgn_height < (max_h - 1))
        {
            if ((ret = _OsdGenerateRegion(&region)) != OSD_RET_OK)
            {
                LOG(1, "_OsdGenerateRegion fail, ret=%d\n", ret);
                return OSD_CLI_RET_ERR_GENERAL;
            }

            y = (UINT32)rand() % (max_h >> 3);
            if (y < (total_rgn_height + 1))
            {
                y = total_rgn_height + 1;
            }
            h = 1 + (UINT32)rand() % max_h;
            if (y + h > max_h) // cross bottom line
            {
                h = max_h - y;
            }
            total_rgn_height = y + h;

            OSD_RGN_Set(region, OSD_RGN_POS_Y, y);
            OSD_RGN_Set(region, OSD_RGN_CLIP_V, y);
            OSD_RGN_Set(region, OSD_RGN_OUT_H, h);
            OSD_RGN_Set(region, OSD_RGN_STEP_V, (UINT32)rand() % 0x4000);
            OSD_RGN_Dump(region);
            OSD_RGN_Insert(region, list);
        }
    }
    OSD_PLA_FlipTo(plane, list);
    OSD_PLA_SetBlendLevel(plane, (UINT32)rand() % 256);
    OSD_PLA_Enable(plane, TRUE);

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDiagCmdMix(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 i, j, ret, plane_start = 1, plane_end = 3;
    UINT32 color_mode_start = OSD_CM_YCBCR_CLUT2;
    UINT32 color_mode_end = OSD_CM_ARGB8888_DIRECT32;
    BMP_INFO_T* ptr_bmp_info = _arOsdBmpInfo;
    if (i4Argc >= 3)
    {
        plane_start = StrToInt(szArgv[1]);
        plane_end = StrToInt(szArgv[2]);
    }
    if (i4Argc >= 5)
    {
        color_mode_start = StrToInt(szArgv[3]);
        color_mode_end = StrToInt(szArgv[4]);
    }

    CLI_Parser("osd.init");
    for (i = color_mode_start; i <= color_mode_end; i++)
    {
        if (i == OSD_CM_RESERVED_0 || i == OSD_CM_RESERVED_1)
        {
            continue;
        }
        ptr_bmp_info = _arOsdBmpInfo;
        while (ptr_bmp_info->u4Width > 0 && ptr_bmp_info->u4ColorMode!=i)
        {
            ptr_bmp_info++;
        }
        ASSERT(ptr_bmp_info->u4Width > 0);

        CLI_Parser("osd.region.delete 0");
        CLI_RET("osd.region.new %d %d 0x%x %d 0 0 0 %d %d",
                ptr_bmp_info->u4Width, ptr_bmp_info->u4Height,
                ptr_bmp_info->u4Address, ptr_bmp_info->u4ColorMode,
                ptr_bmp_info->u4Width, ptr_bmp_info->u4Height);

        CLI_Parser("osd.region.insert 0 0");

        for (j = plane_start; j <= plane_end; j++)
        {
            CLI_RET("osd.plane.flip %d 0", j);
            CLI_RET("osd.plane.enable %d 1", j);
            CLI_RET("osdt.batch %d 0", j);
            CLI_RET("osd.plane.enable %d 0", j);
        }
    }

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDiagCmdInvertEndian(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(2);

    UINT32* bmp_start = (UINT32*)StrToInt(szArgv[1]);
    INT32 pixel_num = StrToInt(szArgv[2]);
    INT32 i;
    UINT32 pixel;

    for (i = 0; i < pixel_num; i++)
    {
        pixel = *bmp_start;
        pixel = ((pixel & 0xff000000) >> 24) | ((pixel & 0xff0000) >> 8) |
                ((pixel & 0xff00) << 8) | ((pixel & 0xff) << 24);
        *bmp_start++ = pixel;
    }

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static UINT8 _OsdGetSerialChar(void)
{
    UCHAR ucByte;

    if (SerInByte(&ucByte))
    {
        return ucByte;
    }
    else
    {
        return 0xFF;
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdTestPixelAlpha(INT32 region, INT32 delay)
{
    UINT32 alpha, color_mode, bmp_size, pixel, tmp, count;
    UINT16* bmp_address_16;
    UINT8* bmp_address_8;

    //pixel...
    OSD_RGN_Set(region, OSD_RGN_MIX_SEL, 1);
    OSD_RGN_Get(region, OSD_RGN_COLORMODE, &color_mode);
    OSD_RGN_Get(region, OSD_RGN_BMP_W, &tmp);
    bmp_size = tmp;
    OSD_RGN_Get(region, OSD_RGN_BMP_H, &tmp);
    bmp_size *= tmp;
    alpha = 0;
    // argb8888,aybr8888,big-endian
    if (color_mode == OSD_CM_ARGB8888_DIRECT32 ||
        color_mode == OSD_CM_AYCBCR8888_DIRECT32)
    {
        while (alpha <= 0xff)
        {
            OSD_RGN_Get(region, OSD_RGN_BMP_ADDR, &tmp);
            bmp_address_8 = (UINT8*)tmp;
            count = bmp_size;
            do
            {
                *(bmp_address_8) = alpha;
                bmp_address_8 += 4;
            }
            while (--count != 0);
            HalFlushInvalidateDCache();
            DELAY(delay, alpha);
            alpha += 11;
        }
    }
    else if (color_mode == OSD_CM_ARGB4444_DIRECT16) // argb4444,little-endian
    {
        while (alpha <= 0xf)
        {
            OSD_RGN_Get(region, OSD_RGN_BMP_ADDR, &tmp);
            bmp_address_16 = (UINT16*)tmp;
            count = bmp_size;
            do
            {
                pixel = *bmp_address_16;
                pixel = (alpha << 12) | (pixel & 0xfff);
                *bmp_address_16++ = pixel;
            }
            while (--count != 0);
            HalFlushInvalidateDCache();
            DELAY(delay, alpha);
            alpha++;
        }
    }
    else if (color_mode == OSD_CM_ARGB1555_DIRECT16) // argb1555,little-endian
    {
        while (alpha <= 0x1)
        {
            OSD_RGN_Get(region, OSD_RGN_BMP_ADDR, &tmp);
            bmp_address_16 = (UINT16*)tmp;
            count = bmp_size;
            do
            {
                pixel = *bmp_address_16;
                pixel = (alpha << 15) | (pixel & 0x7fff);
                *bmp_address_16++ = pixel;
            }
            while (--count!=0);
            HalFlushInvalidateDCache();
            DELAY(delay, alpha);
            alpha++;
        }
    }
    else if (color_mode <= OSD_CM_YCBCR_CLUT8 ||
             (color_mode >= OSD_CM_RGB_CLUT2 &&
             color_mode<=OSD_CM_RGB_CLUT8)) // palette mode, BE
    {
        OSD_RGN_Get(region, OSD_RGN_PAL_LEN, &tmp);
        switch (tmp)
        {
            case 0:
                bmp_size = 4;
                break;
            case 1:
                bmp_size = 16;
                break;
            case 2:
                bmp_size = 256;
                break;
            default:
                break; // do nothing
        }
        while (alpha <= 0xff)
        {
            OSD_RGN_Get(region, OSD_RGN_PAL_ADDR, &tmp);
            bmp_address_8 = (UINT8*)tmp;
            count = bmp_size;
            do
            {
                *(bmp_address_8) = alpha;
                bmp_address_8 += 4;
            }
            while (--count != 0);
            HalFlushInvalidateDCache();
            DELAY(delay, alpha);
            alpha++;
        }
    }
    // ybr422 & rgb565 has no alpha
    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static void _OsdSort(UINT32* array, UINT32 size)
{
    UINT32 i, j, idx, swap;

    for (i = 0; i < size; i++)
    {
        Printf("%d ", array[i]);
    }
    Printf("\n");

    for (i = 0; i < size; i++)
    {
        idx = i;
        for (j = i; j < size; j++)
        {
            if (idx != j && array[idx] == array[j])
                array[j]++;
            else if (array[idx] > array[j])
                idx = j;
        }
        if (i != idx)
        {
            swap = array[i];
            array[i] = array[idx];
            array[idx] = swap;
        }
    }

    for (i = 0; i < size; i++)
    {
        Printf("%d ", array[i]);
    }
    Printf("\n");
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDiagCmdCapture(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(2);
    INT32 plane;
    GET_PLANE(plane);
    OSD_SC_UNION_T scaler_cfg;
    UINT32 dma_w, dma_h, x, y, action = 0, dst_h, dst_w;
    UINT32 dma_address = (UINT32)StrToInt(szArgv[2]);

    if (i4Argc>=4)
        action = StrToInt(szArgv[3]);

    _OSD_BASE_GetScrnHSize(&dma_w);
    _OSD_BASE_GetScrnVSize(&dma_h);
    if (plane == OSD_PLANE_1)
    {
        dst_w = dma_w;
        dst_h = dma_h;
    }
    else
    {
        _OSD_SC_GetReg(OSD_PLANE_TO_SCALER(plane), (UINT32*)&scaler_cfg);
        dst_w = scaler_cfg.rField.u4DstHSize;
        dst_h = scaler_cfg.rField.u4DstVSize;
    }

    // do sw scaling, note:do this function before any other CLI invocation
    if (action == 2)
    {
        UINT32 bmp_address, bmp_width;
        OSD_RGN_Get(CAP_SRC_REGION, OSD_RGN_BMP_ADDR, &bmp_address);
        bmp_width = StrToInt(szArgv[4]);
        if (plane == OSD_PLANE_1)
        {
            CLI_Parser_Arg("osdt.cm 1 0x%x", dma_address+dma_w*dma_h*4);
        }
        else
        {
            CLI_Parser_Arg("osdt.cm 0x12 0x%x 0x%x %d",
                           dma_address+dma_w*dma_h*4, bmp_address, bmp_width);
        }
    }

    // enable writeback
    CLI_Parser_Arg("pmx.diag.wb 0x%x", dma_address);

    if (action <= 1)
    {
        // back to 480p mode to view result
        CLI_Parser_Arg("osd.list.det %d all", CAP_VIEW_LIST);
        CLI_Parser_Arg("osd.region.new %d %d 0x%x 0xe %d %d %d %d %d",
                       dma_w, dma_h, dma_address, dma_w*4, 10, 10,
                       CAP_ACTIVE_W, CAP_ACTIVE_H);
        CLI_Parser_Arg("osd.region.insert %d %d",
                       CAP_VIEW_REGION, CAP_VIEW_LIST);
        CLI_Parser_Arg("osd.plane.en %d 0", plane + 1);
        x_thread_delay(1000);
        CLI_Parser_Arg("osd.plane.flip %d %d", plane + 1, CAP_VIEW_LIST);
        CLI_Parser_Arg("osd.plane.en %d 1", plane+1);
    }

    if (action == 1) // view original size
    {
        OSD_RGN_Set(CAP_VIEW_REGION, OSD_RGN_STEP_H, 0x1000);
        OSD_RGN_Set(CAP_VIEW_REGION, OSD_RGN_STEP_V, 0x1000);
        for (y = 0; y < dma_h; y += CAP_ACTIVE_H)
        {
            if (y + CAP_ACTIVE_H > dma_h)
            {
                y = dma_h - CAP_ACTIVE_H;
            }
            OSD_RGN_Set(CAP_VIEW_REGION, OSD_RGN_CLIP_V, y);
            //CLI_RET("osd.region.set %d clip_v %d\n", CAP_VIEW_REGION, y);
            for (x = 0; x < dma_w; x += CAP_ACTIVE_W)
            {
                if (x + CAP_ACTIVE_W > dma_w)
                {
                    x = dma_w - CAP_ACTIVE_W;
                }
                OSD_RGN_Set(CAP_VIEW_REGION, OSD_RGN_CLIP_H, x);
                //CLI_RET("osd.region.set %d clip_h %d\n", CAP_VIEW_REGION, x);
                x_thread_delay(1500);
            }
        }
        OSD_RGN_Set(CAP_VIEW_REGION, OSD_RGN_CLIP_H, 0);
        OSD_RGN_Set(CAP_VIEW_REGION, OSD_RGN_CLIP_V, 0);
    }
    else if (action >= 2) // compare
    {
        UINT32 fail=0;

        UINT32* sws_top = (UINT32*)(dma_address+dma_w*dma_h*4);
        UINT8* dmaR = (UINT8*)(dma_address+dma_w*dma_h);
        UINT8* dmaG = (UINT8*)(dma_address+dma_w*dma_h*3);
        UINT8* dmaB = (UINT8*)dma_address;

        for (y = 0; y < dst_h; y++)
        {
            for (x = 0; x < dst_w; x++)
            {
                UINT8 r, g, b, a;
                UINT32 bgr, index, cmodel_bgra;

                // extract dma value
                index = y*dma_w + x;
                r = dmaR[index];
                g = dmaG[index];
                b = dmaB[index];
                bgr = (b << 24) | (g << 16) | (r << 8);

                // extract cmodel value
                cmodel_bgra = *sws_top;
                a = cmodel_bgra & 0xff;
                if (a < 0xff)
                {
                    r = (cmodel_bgra & 0xff00) >> 8;
                    g = (cmodel_bgra & 0xff0000) >> 16;
                    b = (cmodel_bgra & 0xff000000) >> 24;
                    r = ((r * a * 4) + 128) / 1024;
                    if (r >= 256)
                    {
                        r = 255;
                    }
                    g = ((g * a * 4) + 128) / 1024;
                    if (g >= 256)
                    {
                        g = 255;
                    }
                    b = ((b * a * 4) + 128) / 1024;
                    if (b >= 256)
                    {
                        b = 255;
                    }
                    cmodel_bgra = (b << 24) | (g << 16) | (r << 8);
                }

                if ((cmodel_bgra & 0xffffff00) != bgr)
                {
                    LOG(11, "diff@(y=%04d, pixel=%04d), "
                        "sws_top(0x%08x)=0x%08x, dma=0x%08x\n",
                        y, x, sws_top, *sws_top, bgr);
                    fail++;
                }
                sws_top++;
            }
        }

        if (fail>0)
        {
            LOG(1, "compare finish(size=%dx%d), FAIL=%020d\n",
                dst_w, dst_h, fail);
            //ASSERT(0);
        }
        else
        {
            LOG(1, "compare finish(size=%dx%d), PASS\n",
                dst_w, dst_h);
        }
        if (fail > 0)
        {
            return fail;
        }
    }

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDiagCmdBurn(INT32 i4Argc, const CHAR ** szArgv)
{
    //CHK_NUM_MIN_PARAM(1);
    INT32 sc = StrToInt(szArgv[1]);
    INT32 plane = sc + 2;
    INT32 coe;

    // do random test for osd plane1
    // NOTE: set mixer alpha correctly first, call "pmx.i"
    if (i4Argc == 1)
    {
        INT32 i;
        CLI("osd.mode %d", OSD_DM_480P);
        for (i = 0; i < 10000; i++)
        {
            LOG(1, "[TEST LOOP %d]\n", i);
            CLI_Parser("osdt.r 1");
            CLI_Parser("osdt.cap 1 0x3000000 2");
        }
        return OSD_CLI_RET_OK;
    }

    LOG(5, "32 bit color mode\n");
    CLI("osd.sc.f %d 32", sc); //32bit

    // 480p lpf scan----------------------------------------------------
    CLI("osd.mode %d", OSD_DM_480P);
    CLI("osdt.pt %d 5 256 40 256 40", plane);
    for (coe = -31; coe <= 31; coe++)
    {
        OSD_SC_SetLpfInfo(sc, TRUE, coe, -63, 0, 0, 0);
        CLI("osdt.cap %d 0x3000000 2 256", plane);
    }
    for (coe = -31; coe <= 31; coe++)
    {
        OSD_SC_SetLpfInfo(sc, TRUE, coe, 63, 127, 0, 0);
        CLI("osdt.cap %d 0x3000000 2 256", plane);
    }
    for (coe = -63; coe <= 63; coe++)
    {
        OSD_SC_SetLpfInfo(sc, TRUE, -31, coe, 0, 0, 0);
        CLI("osdt.cap %d 0x3000000 2 256", plane);
    }
    for (coe = -63; coe <= 63; coe++)
    {
        OSD_SC_SetLpfInfo(sc, TRUE, 31, coe, 127, 0, 0);
        CLI("osdt.cap %d 0x3000000 2 256", plane);
    }
    for (coe = 0; coe <= 127; coe++)
    {
        OSD_SC_SetLpfInfo(sc, TRUE, -31, -63, coe, 0, 0);
        CLI("osdt.cap %d 0x3000000 2 256", plane);
    }
    for (coe = 0; coe <= 127; coe++)
    {
        OSD_SC_SetLpfInfo(sc, TRUE, 31, 63, coe, 0, 0);
        CLI("osdt.cap %d 0x3000000 2 256", plane);
    }
    OSD_SC_SetLpf(sc, FALSE);


    // 1080p vertical+horizontal test----------------------------------
    // to load a 1080p picture first

    CLI("osd.mode %d", OSD_DM_1080P);
    //CLI("osdt.pt %d 5 1920 1080 1920 1080", plane);
    OSD_SC_Scale(sc, TRUE, 1280, 720, 1366, 768);
    CLI("osdt.cap %d 0x3000000 2 1920", plane);
    OSD_SC_Scale(sc, TRUE, 1280, 720, 1440, 900);
    CLI("osdt.cap %d 0x3000000 2 1920", plane);
    OSD_SC_Scale(sc, TRUE, 1280, 720, 1680, 1050);
    CLI("osdt.cap %d 0x3000000 2 1920", plane);
    OSD_SC_Scale(sc, TRUE, 1280, 720, 1920, 1080);
    CLI("osdt.cap %d 0x3000000 2 1920", plane);

    // H up
    //CLI("osdt.pt %d 5 256 40 256 40", plane);
    CLI("osdt.s %d 480 40 480 40 1920 40 1 0x3000000 1920", sc);
    //CLI("osdt.s %d 256 40 256 40 257 40 1 0x3000000 256", sc);
    // V up
    //CLI("osdt.pt %d 5 256 40 256 40", plane);
    CLI("osdt.s %d 256 40 256 40 256 1080 1 0x3000000 1920", sc);
    // H down
    //CLI("osdt.pt %d 5 1920 40 1920 40", plane);
    CLI("osdt.s %d 1920 40 1920 40 480 40 1 0x3000000 1920", sc);
    // V down
    //CLI("osdt.pt %d 5 256 1080 256 1080", plane);
    CLI("osdt.s %d 256 1080 256 1080 256 270 1 0x3000000 1920", sc);


    // 480p scaler scan step-------------------------------------------
    // to load a 720p picture first

    CLI("osd.mode %d", OSD_DM_480P);

    // H up
    //CLI("osdt.pt %d 5 256 40 256 40", plane);
    CLI("osd.sc.s %d 64 32 64 32", sc);
    CLI("osdt.step %d 1 64 0x3fff 0x1000 1 0x3000000 1280", sc);
    //CLI("osdt.step %d 1 256 0x3fff 0x3fff 1 0x3000000 1920", sc);
    // V up
    //CLI("osdt.pt %d 5 256 40 256 40", plane);
    CLI("osd.sc.s %d 64 32 64 32", sc);
    CLI("osdt.step %d 3 32 0x3fff 0x1000 0x1 0x3000000 1280", sc);
    //CLI("osdt.step %d 3 40 0x3fff 0x3fff 0x1 0x3000000 1920", sc);
    // H down
    //CLI("osdt.pt %d 5 720 40 720 40", plane);
    CLI("osd.sc.s %d 720 32 720 32", sc);
    CLI("osdt.step %d 0 720 0x3fff 0x1000 1 0x3000000 1280", sc);
    //CLI("osdt.step %d 0 720 0x3fff 0x3fff 1 0x3000000 1920", sc);
    // V down
    //CLI("osdt.pt %d 5 256 480 256 480", plane);
    CLI("osd.sc.s %d 64 480 64 480", sc);
    CLI("osdt.step %d 2 480 0x3fff 0x1000 0x1 0x3000000 1280", sc);
    //CLI("osdt.step %d 2 480 0x3fff 0x3fff 0x1 0x3000000 1920", sc);

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDiagCmdPattern(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(4);
    INT32 plane = StrToInt(szArgv[1]);
    UINT32 pattern = (UINT32)StrToInt(szArgv[2]);
    INT32 w = StrToInt(szArgv[3]);
    INT32 h = StrToInt(szArgv[4]);
    INT32 bpp = StrToInt(szArgv[7]);
    UINT32 region, sw=0, sh=0, fgScale = FALSE, cm, pos_x = 0, pos_y = 0;
    static UINT32 osd_buf[3] = {0, 0, 0};
    UINT32 align_buf, buf, palette_addr;
    UINT32 u4Color = 0xffff0000;
      if (i4Argc > 5)
    {
        fgScale = TRUE;
        sw = (UINT32)StrToInt(szArgv[5]);
        sh = (UINT32)StrToInt(szArgv[6]);
    }
    if (i4Argc > 7)
    {
        if (bpp == 16)
        {
            bpp = 2;
            cm = OSD_CM_ARGB4444_DIRECT16;
        }
        else if (bpp == 1555)
        {
            bpp = 2;
            cm = OSD_CM_ARGB1555_DIRECT16;
        }
        else if (bpp == 565)
        {
            bpp = 2;
            cm = OSD_CM_RGB565_DIRECT16;
        }
        else if (bpp == 8)
        {
            bpp = 1;
            cm = OSD_CM_RGB_CLUT8;
        }
        else if (bpp == 1)
        {
            bpp = 1;
            cm = OSD_CM_ALPHA_BIT;
        }
        else
        {
            bpp = 4;
            cm = OSD_CM_ARGB8888_DIRECT32;
        }
    }
    else
    {
        bpp = 4;
        cm = OSD_CM_ARGB8888_DIRECT32;
    }

    if (i4Argc > 8)
    {
         u4Color = StrToInt(szArgv[8]);
    }
 
   if (i4Argc > 9)
   	{
	  pos_x = StrToInt(szArgv[9]);
          pos_y = StrToInt(szArgv[10]);
   	}
   else
    {
        CLI_Parser("osd.init");
        CLI_Parser("osd.d_l -l");
    }
    buf = osd_buf[plane - 1];
#if 1 //ndef __MODEL_slt__
    if (buf != 0)
    {
        BSP_FreeDmaMemory((UINT32)buf);
    }
    buf = (UINT32)BSP_AllocDmaMemory(OSD_DMA_ALIGN(w * h * bpp)
                                     + OSD_DMA_ALIGN_SIZE + OSD_PALETTE_SIZE);
    osd_buf[plane - 1] = buf;

    align_buf = OSD_DMA_ALIGN(buf);
#else
    {
        FBM_POOL_T* prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG);
        buf = prFbmPool->u4Addr;
        osd_buf[plane - 1] = buf;
        align_buf = buf;
    }
#endif
    palette_addr = align_buf + OSD_DMA_ALIGN(w * h * bpp);

    LOG(5, "bmp_addr=0x%x, pale_addr=0x%x\n", align_buf, palette_addr);
    if (fgScale && (plane == 1))
    {
        OSD_RGN_Create(&region, w, h, (void*)align_buf,
                       cm, 0, 0, 0, sw, sh);
    }
    else
    {
        OSD_RGN_Create(&region, w, h, (void*)align_buf,
                       cm, 0, 0, 0, w, h);
    }
    OSD_RGN_SetBigEndian(region, 1);

    // create palette here
    if (cm == OSD_CM_RGB_CLUT8)
    {
        OSD_CreatePaleTable((OSD_ARGB_T *)palette_addr);
        OSD_RGN_Set(region, OSD_RGN_PAL_ADDR, palette_addr);
    }
if( pattern == 6 )
	 {
	   CLI("osd.region.pt %d 0x%x  0x%x", region, pattern, u4Color);
        }
else
   {
    CLI("osd.region.pt %d 0x%x", region, pattern);
    }
    CLI("osd.region.insert %d %d", region, plane);
    CLI("osd.plane.flip %d %d", plane, plane);
    if (fgScale && plane > 1)
    {
        CLI("osd.sc.s %d %d %d %d %d", plane - 2, w, h, sw, sh);
    }
	else  if (fgScale && plane == 1)
	{
	   CLI("osd.sc.s %d %d %d %d %d", 3, w, h, sw, sh);
	}
    OSD_BASE_SetOsdPosition(plane - 1, pos_x, pos_y);
	
  if(plane == 1)  //for 5396 IC bug
  	{
  	 IO_WRITE32(OSD_BASE, 0x300, 0x00000001);
 	}
    CLI("osd.plane.enable %d 1", plane);

  #ifdef __MODEL_slt__
  #if defined(CC_MT5396)
    CLI("osd.region.insert %d %d", region, 1);
    CLI("osd.plane.flip %d %d", 1, 1);
     CLI("osd.region.insert %d %d", region,2);
    CLI("osd.plane.flip %d %d", 2, 2);
	 CLI("osd.region.insert %d %d", region,3);
    CLI("osd.plane.flip %d %d", 3, 3);
	IO_WRITE32(OSD_BASE, 0x100, 0x00000001);
	IO_WRITE32(OSD_BASE, 0x200, 0x00000001);
	IO_WRITE32(OSD_BASE, 0x300, 0x00000001);
	IO_WRITE32(OSD_BASE, 0x024, 0x01000100);
	IO_WRITE32(OSD_BASE, 0x028, 0x02000200);
	IO_WRITE32(OSD_BASE, 0x0, 0x00000001);
	#else
     CLI("osd.region.insert %d %d", region,2);
    CLI("osd.plane.flip %d %d", 2, 2);
	 CLI("osd.region.insert %d %d", region,3);
    CLI("osd.plane.flip %d %d", 3, 3);
	IO_WRITE32(OSD_BASE, 0x200, 0x00000001);
	IO_WRITE32(OSD_BASE, 0x300, 0x00000001);
	IO_WRITE32(OSD_BASE, 0x024, 0x01000100);
	IO_WRITE32(OSD_BASE, 0x028, 0x02000200);
	IO_WRITE32(OSD_BASE, 0x0, 0x00000001);
	#endif
  #endif

    return OSD_CLI_RET_OK;
}


static INT32 _OsdDiagCmdMalloc(INT32 i4Argc, const CHAR ** szArgv)
{
    static UINT32 buf = 0, size = 0;
    if (i4Argc == 1)
    {
        Printf("buf=0x%x(~0x%0x)\n", buf, buf+size);
        return OSD_CLI_RET_OK;
    }

    if (buf != 0)
    {
        BSP_FreeDmaMemory((UINT32)buf);
    }

    size = PARAM(0);
    buf = (UINT32)BSP_AllocDmaMemory(size);
    Printf("buf=0x%x(~0x%0x)\n", buf, buf+size);
//    x_memset((void*)buf, 0, size);
//    HalFlushInvalidateDCache();
    return OSD_CLI_RET_OK;
}

#ifdef CURSOR_SUPPORT
static INT32 _OsdDiagCmdCursorPt(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(1);
    INT32 x, y, delay = StrToInt(szArgv[1]);
    UINT32 scrn_w, scrn_h, tmp, stage;//, v_start_top, v_start_bot;
#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
   UINT32 startX = 0;
  UINT32 startY = 0;
  #endif
    _OSD_BASE_GetScrnHSize(&tmp);
if(tmp != 0)
    scrn_w = tmp - 8;
else scrn_w = 720;
    _OSD_BASE_GetScrnVSize(&tmp);
if(tmp != 0)
    scrn_h = tmp - 8;
else
   scrn_h = 480;

    /*_OSD_BASE_GetScrnVStartTop(&tmp);
    v_start_top = tmp;
    _OSD_BASE_GetScrnVStartBot(&tmp);
    v_start_bot = tmp;
    _OSD_BASE_SetScrnVStartTop(v_start_top+20);
    _OSD_BASE_SetScrnVStartBot(v_start_bot+20);
    _OSD_BASE_UpdateHwReg();*/

#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
		scrn_w = 640;
		scrn_h = 480;
		_OSD_CSR_SetHActive(0x2d0);
		startX = 100;
		startY = 100;
	#endif
    
    _OSD_CSR_SetColor(0x0, 0xff0000);
    _OSD_CSR_SetColor(0x1, 0x00ff00);
    _OSD_CSR_SetColor(0x2, 0x0000ff);
    _OSD_CSR_SetColor(0x3, 0xffff00);
    _OSD_CSR_SetColor(0x4, 0xff00ff);
    _OSD_CSR_SetColor(0x5, 0x00ffff);
    _OSD_CSR_SetColor(0x6, 0xffffff);
    _OSD_CSR_SetColor(0x7, 0xcccccc);
    _OSD_CSR_SetColor(0x8, 0xaaaaaa);
    _OSD_CSR_SetColor(0x9, 0x888888);
    _OSD_CSR_SetColor(0xa, 0x666666);
    _OSD_CSR_SetColor(0xb, 0x444444);
    _OSD_CSR_SetColor(0xc, 0x222222);
    _OSD_CSR_SetColor(0xd, 0x111111);
    _OSD_CSR_SetColor(0xe, 0x000000);

    _OSD_CSR_SetHStart(0);
    _OSD_CSR_SetVStart(0);
    _OSD_CSR_SetEn(1);
    _OSD_CSR_UpdateHwReg();
    

    stage = 0;
    while (1)
    {
        if (stage == 0)
        {
            for (x = 0; x < 4; x++)
            {
                x_memset((void*)(OSD_CSR_DATA_REG + (OSD_CSR_DATA_SIZE * x / 4)),
                         (x << 6) | (x << 4) | (x << 2) | x,
                         OSD_CSR_DATA_SIZE / 4);
            }
            x_memset((void*)(OSD_CSR_DATA_REG), 0x55, 8);
        }
        else if (stage == 1)
        {
            _OSD_CSR_SetH4Bit(1);
            _OSD_CSR_SetV4Bit(0);
            _OSD_CSR_UpdateHwReg();
            for (x = 0; x < 16; x++)
            {
                x_memset((void*)(OSD_CSR_DATA_REG + (OSD_CSR_DATA_SIZE * x / 16)),
                         (x << 4) | x, OSD_CSR_DATA_SIZE / 16);
            }
            x_memset((void*)(OSD_CSR_DATA_REG), 0x11, 8);
        }
        else if (stage == 2)
        {
            _OSD_CSR_SetH4Bit(0);
            _OSD_CSR_SetV4Bit(1);
            _OSD_CSR_UpdateHwReg();
        }
        else if (stage == 3)
        {
            break;
        }

        for (tmp = 0; tmp <= 1; tmp++)
        {
            _OSD_CSR_SetH2x(tmp);
            _OSD_CSR_SetV2x(tmp);
            _OSD_CSR_UpdateHwReg();
            for (x = startX; x <= scrn_w; x++)
            {
                _OSD_CSR_SetHStart(x);
                _OSD_CSR_UpdateHwReg();
                DELAY(delay, x);
            }
            for (y = startY; y <= scrn_h; y++)
            {
                _OSD_CSR_SetVStart(y);
                _OSD_CSR_UpdateHwReg();
                DELAY(delay, y);
            }
            for (; x >= startX; x--)
            {
                _OSD_CSR_SetHStart(x);
                _OSD_CSR_UpdateHwReg();
                DELAY(delay, x);
            }
            for (; y >= startY; y--)
            {
                _OSD_CSR_SetVStart(y);
                _OSD_CSR_UpdateHwReg();
                DELAY(delay, y);
            }
        }
        stage++;
    }

    //_OSD_CSR_SetEn(0);
    //_OSD_CSR_UpdateHwReg();

    /*_OSD_BASE_SetScrnVStartTop(v_start_top);
    _OSD_BASE_SetScrnVStartBot(v_start_bot);
    _OSD_BASE_UpdateHwReg();*/

    return OSD_CLI_RET_OK;
}

static INT32 _OsdDiagCmdCursor(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(1);
    INT32 x, y, delay = StrToInt(szArgv[1]);
    UINT32 scrn_w, scrn_h, tmp, stage;//, v_start_top, v_start_bot;

    _OSD_BASE_GetScrnHSize(&tmp);
    scrn_w = tmp - 8;
    _OSD_BASE_GetScrnVSize(&tmp);
    scrn_h = tmp - 8;

    /*_OSD_BASE_GetScrnVStartTop(&tmp);
    v_start_top = tmp;
    _OSD_BASE_GetScrnVStartBot(&tmp);
    v_start_bot = tmp;
    _OSD_BASE_SetScrnVStartTop(v_start_top+20);
    _OSD_BASE_SetScrnVStartBot(v_start_bot+20);
    _OSD_BASE_UpdateHwReg();*/

    
    _OSD_CSR_SetColor(0x0, 0xff0000);
    _OSD_CSR_SetColor(0x1, 0x00ff00);
    _OSD_CSR_SetColor(0x2, 0x0000ff);
    _OSD_CSR_SetColor(0x3, 0xffff00);
    _OSD_CSR_SetColor(0x4, 0xff00ff);
    _OSD_CSR_SetColor(0x5, 0x00ffff);
    _OSD_CSR_SetColor(0x6, 0xffffff);
    _OSD_CSR_SetColor(0x7, 0xcccccc);
    _OSD_CSR_SetColor(0x8, 0xaaaaaa);
    _OSD_CSR_SetColor(0x9, 0x888888);
    _OSD_CSR_SetColor(0xa, 0x666666);
    _OSD_CSR_SetColor(0xb, 0x444444);
    _OSD_CSR_SetColor(0xc, 0x222222);
    _OSD_CSR_SetColor(0xd, 0x111111);
    _OSD_CSR_SetColor(0xe, 0x000000);

    _OSD_CSR_SetHStart(0);
    _OSD_CSR_SetVStart(0);
    _OSD_CSR_SetEn(1);
    _OSD_CSR_UpdateHwReg();
    

    stage = 0;
    while (1)
    {
        if (stage == 0)
        {
            for (x = 0; x < 4; x++)
            {
                x_memset((void*)(OSD_CSR_DATA_REG + (OSD_CSR_DATA_SIZE * x / 4)),
                         (x << 6) | (x << 4) | (x << 2) | x,
                         OSD_CSR_DATA_SIZE / 4);
            }
            x_memset((void*)(OSD_CSR_DATA_REG), 0x55, 8);
        }
        else if (stage == 1)
        {
            _OSD_CSR_SetH4Bit(1);
            _OSD_CSR_SetV4Bit(0);
            _OSD_CSR_UpdateHwReg();
            for (x = 0; x < 16; x++)
            {
                x_memset((void*)(OSD_CSR_DATA_REG + (OSD_CSR_DATA_SIZE * x / 16)),
                         (x << 4) | x, OSD_CSR_DATA_SIZE / 16);
            }
            x_memset((void*)(OSD_CSR_DATA_REG), 0x11, 8);
        }
        else if (stage == 2)
        {
            _OSD_CSR_SetH4Bit(0);
            _OSD_CSR_SetV4Bit(1);
            _OSD_CSR_UpdateHwReg();
        }
        else if (stage == 3)
        {
            break;
        }

        for (tmp = 0; tmp <= 1; tmp++)
        {
            _OSD_CSR_SetH2x(tmp);
            _OSD_CSR_SetV2x(tmp);
            _OSD_CSR_UpdateHwReg();
            for (x = 0; x <= scrn_w; x++)
            {
                _OSD_CSR_SetHStart(x);
                _OSD_CSR_UpdateHwReg();
                DELAY(delay, x);
            }
            for (y = 0; y <= scrn_h; y++)
            {
                _OSD_CSR_SetVStart(y);
                _OSD_CSR_UpdateHwReg();
                DELAY(delay, y);
            }
            for (; x >= 0; x--)
            {
                _OSD_CSR_SetHStart(x);
                _OSD_CSR_UpdateHwReg();
                DELAY(delay, x);
            }
            for (; y >= 0; y--)
            {
                _OSD_CSR_SetVStart(y);
                _OSD_CSR_UpdateHwReg();
                DELAY(delay, y);
            }
        }
        stage++;
    }

    //_OSD_CSR_SetEn(0);
    //_OSD_CSR_UpdateHwReg();

    /*_OSD_BASE_SetScrnVStartTop(v_start_top);
    _OSD_BASE_SetScrnVStartBot(v_start_bot);
    _OSD_BASE_UpdateHwReg();*/

    return OSD_CLI_RET_OK;
}

#endif

#if defined(CC_MT5368) || defined(CC_MT5396)
#ifdef CC_3D_OSD_SUPPORT

static INT32 _OsdDiagCmd3DLineShift(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(1);
    INT32 delay = StrToInt(szArgv[1]);
    INT32 plane = StrToInt(szArgv[2]);
    INT32 pattern = StrToInt(szArgv[3]);
    UINT32  tmp;//, v_start_top, v_start_bot;
	CLI("osdt.pt  %d %d  720 480 720 480 32  ", plane,pattern);
    switch (plane)
        {
        case 1:
		_OSD_3D_SetOsd1LShiftEn(TRUE);
		 for (tmp = 0; tmp <= 0xff; tmp++)
		 	{
		 	 _OSD_3D_SetOsd1ShiftValue(tmp);
			 _OSD_3D_UpdateHwReg();
			 x_thread_delay(delay);
		 	}
		break;
        case 2:
		_OSD_3D_SetOsd2LShiftEn(TRUE);
		 for (tmp = 0; tmp <= 0xff; tmp++)
		 	{
		 	 _OSD_3D_SetOsd2ShiftValue(tmp);
			 _OSD_3D_UpdateHwReg();
			 x_thread_delay(delay);
		 	}
		 break;
        case 3:
		_OSD_3D_SetOsd3LShiftEn(TRUE);
		 for (tmp = 0; tmp <= 0xff; tmp++)
		 	{
		 	 _OSD_3D_SetOsd3ShiftValue(tmp);
			 _OSD_3D_UpdateHwReg();
			  x_thread_delay(delay);
		 	}
		 break;
        case 4:
		_OSD_3D_SetCsrLShiftEn(TRUE);
		 for (tmp = 0; tmp <= 0xff; tmp++)
		 	{
		 	 _OSD_3D_SetCsrShiftValue(tmp);
			 _OSD_3D_UpdateHwReg();
			  x_thread_delay(delay);
		 	}
		 break;
		default:
			break;
    	}    
    return OSD_CLI_RET_OK;
}
#endif
#endif
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
CLI_EXEC_T* GetOsdDiagCmdTbl(void)
{
    return _arOsdDiagCmdTbl;
}
#endif
