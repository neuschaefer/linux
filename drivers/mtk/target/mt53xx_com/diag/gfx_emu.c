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
 * $RCSfile: gfx_emu.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file gfx_emu.c
 *  This file contains implementation of CLI CMD for GFX emulation.
 *  
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

/*lint -save -e534 -e715 -e786 -e818 -e826 -e830 -e950 -e957 -e960 -e961 -e970 */
#include "x_typedef.h"
#include "x_os.h"
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_bim.h"
#include "x_timer.h"
#include "x_assert.h"
#include "x_rand.h"
#include "x_debug.h"
#include "x_mid.h"
#include "x_hal_926.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "gfx_if.h"
#include "gfx_drvif.h"
#include "drvcust_if.h"

#include <stdarg.h>
#include <string.h>


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

#if defined(GFX_ENABLE_SW_MODE)

#include "gfx_emu_ui_tex_1.c"

#include "gfx_emu_jkaraoke_sp_mem.c"

#if defined(CC_MT5368)||defined(CC_MT5396)|| defined(CC_MT5389)||defined(CC_MT5398)
#include "vgfx_drvif.h"
#endif

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define GRAPH_BASE					(IO_VIRT + 0x04000)
#define GFX_ADDR                (GRAPH_BASE)

#define GFX_HW_RESET // test  gfx function not HW reset  every time

#define ADR_DST         FALSE
#define ADR_SRC         TRUE
#define D_PAUSE_DELAY   1000
#define D_FRAME_DELAY   33
#define D_GFX_TEST_AVAILABLE_MODE       6

#define BUF_SCREEN      (_u4GfxEmuBuf[0])
#define COMPARE_BUF     (_u4GfxEmuBuf[1])

#define BUF_ADDR1       (_u4GfxEmuBuf[2])
#define BUF_ADDR2       (_u4GfxEmuBuf[3])
#define BUF_ADDR3       (_u4GfxEmuBuf[4])
#define BUF_ADDR4       (_u4GfxEmuBuf[5])
#define BUF_TEMP        (_u4GfxEmuBuf[6])

#define BUF_ALPHAMAP    (_u4GfxEmuBufAlphaMap)  // Alpha Map
#define BUF_LUMA        (_u4GfxEmuBufLuma)
#define BUF_DST_LUMA    (_u4GfxEmuBufDstLuma)
#define BUF_CHROMA      (_u4GfxEmuBufChroma)
#define BUF_DST_CHROMA  (_u4GfxEmuBufDstChroma)

#define COLOR_COMP_BLUE         0
#define COLOR_COMP_GREEN        1
#define COLOR_COMP_RED          2
#define COLOR_COMP_ALPHA        3
#define MAX_SRC_WIDTH           720
#define MAX_SRC_HEIGHT          480
#define MIN_SRC_WIDTH           19
#define MIN_SRC_HEIGHT          5
#define D_SCALE_UP              64
#define D_SCALE_DN              64
#define D_X_SCALE_UP            D_SCALE_UP
#define D_Y_SCALE_UP            D_SCALE_UP
#define D_X_SCALE_DN            D_SCALE_DN
#define D_Y_SCALE_DN            D_SCALE_DN
#define MAX_DST_WIDTH           (_u4ScreenWidth - 2)
#define MAX_DST_HEIGHT          (_u4ScreenHeight - 2)
#define MIN_DST_WIDTH           (MIN_SRC_WIDTH)
#define MIN_DST_HEIGHT          (MIN_SRC_HEIGHT)

#define GFX_BURST_READ_ALIGN_SIZE 63

#define GFX_BURST_READ_ALIGN(X)  (((X) + GFX_BURST_READ_ALIGN_SIZE) & (~GFX_BURST_READ_ALIGN_SIZE))

#if defined(CC_MT5368)||defined(CC_MT5396)|| defined(CC_MT5389)||defined(CC_MT5398)
#define VG_FLOAT_SUPPORT  1
#define GFX_AND_VGFX 0   // test one cmdque include many fire
#define VGFX_AND_GFX 0
#define VGFX_AND_VGFX 0
#define VGFX_MMU_ENABLE 0
#define VGFX_WRITE_ARGBORDER 0
#define VGFX_DUALCHANNEL 0
#define VGFX_TEXTURE_ARGBORDER 0

//#define 5396_IC_DUAL_CH 1
#endif

#if defined(CC_MT5389)||defined(CC_MT5398)
#define VGFX_RTXT_ARGBORDER 0
#endif

#if defined(CC_MT5398)
#define GFX_BURST_READ
#endif
//#define GFX_BURST_READ
//
//
//xxxx_def
//
#define  PIXEL_SUB(a, b) ((a)>=(b) ? ((a) - (b)) : ((b) - (a)))
//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

typedef struct _NAMED_COLOR_T
{
    CHAR  *szName;
    UINT32 u4Color;
} NAMED_COLOR_T;

typedef union
{
	UINT32 u4Color;
	struct _CM14
	{
		UINT16 b:8;
		UINT16 g:8;
		UINT16 r:8;
		UINT16 a:8;
	}cm14;
	struct _CM13
	{
		UINT16 b:4;
		UINT16 g:4;
		UINT16 r:4;
		UINT16 a:4;
	}cm13[2];
	struct _CM12
	{
		UINT16 b:5;
		UINT16 g:5;
		UINT16 r:5;
		UINT16 a:1;
	}cm12[2];
	struct _CM11
	{
		UINT16 b:5;
		UINT16 g:6;
		UINT16 r:5;
	}cm11[2];
}XolorValue;

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define _PAUSE(X)       x_thread_delay(X)


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

static UINT32 _u4GfxEmuBuf[10];
static UINT32 _u4GfxEmuBufAlphaMap;
static UINT32 _u4GfxEmuBufLuma;
static UINT32 _u4GfxEmuBufDstLuma;
static UINT32 _u4GfxEmuBufChroma;
static UINT32 _u4GfxEmuBufDstChroma;

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
//t32 host command
enum
{
	E_T32_NOP,
	E_T32_EXEC,
	E_T32_RESTART,
	E_T32_EXIT,
	E_T32_EXEC_RELOAD_SYM,
	E_T32_DETECT_HOST,
	E_T32_END
};

#if defined(CC_MT5398)
extern void GFX_SetXORColor(UINT32 u4Color);
extern void GFX_Set_PRE_COLORIZE(BOOL u4PreColorize);
extern void GFX_Set_FastBitBlt(UINT32 u4FastBitBlt);
extern void GFX_Set_SrcAlReplace(UINT32 u4SrcAlRep);
#endif
extern INT32 _GfxColorValueCompare(UINT32 u4ClrVal1, UINT32 u4ClrVal2, UINT32 u4CM);
extern INT32 _GfxMemCompare(UINT8 *pu1HwSa, UINT8 *pu1SwSa, UINT32 u4Num,UINT32 u4ColorModeIndex);

extern void HalFlushInvalidateDCache(void);

extern INT32 T32_HostExec(INT32 i4TargetFlag, const CHAR *szFormat, ...);
// gfx_dif.h
extern void GFX_DifSetMode(UINT32 u4GfxMode);

// gfx_hw.h
extern void GFX_HwSetReqInterval(UINT32 u4ReqInterval);
extern void GFX_HwSetSDFifoThreshold(UINT32 u4Value);
extern void GFX_HwSetCMDFifoThreshold(UINT32 u4Value);
extern void GFX_HwSetPOSTFifoThreshold(UINT32 u4Value);

// gfx_sw.h
extern INT32 GFX_SwBlock2Linear(UINT8 *pu1SrcBase, UINT8 *pu1DstBase,
    INT32 i4Width, INT32 i4Height, INT32 i4MBWidth, INT32 i4MBHeight);
extern INT32 GFX_SwBlock2Swap(UINT8 *pu1SrcBase, UINT8 *pu1DstBase,
    INT32 i4Width, INT32 i4Height, INT32 i4MBWidth, INT32 i4MBHeight);
extern INT32 GFX_SwBlock2Mergetop(UINT8 *pu1SrcBase, UINT8 *pu1DstBase,
    INT32 i4Width, INT32 i4Height, INT32 i4MBWidth, INT32 i4MBHeight);
extern INT32 GFX_SwGetMemCompareResult(void);

// gfx_drvif.h
extern void _GFX_Compose(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar, UINT32 u4Mode);

// for HW/SW compare use
extern void HalFlushInvalidateDCache(void);
extern BOOL HalIsDCacheEnabled(void);
extern void HalDisableDCache(void);


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static INT32 _GfxEmuCmdInit         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdMode         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdColor        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmd5391         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmd536x         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdAuto         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdEnDCache     (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdDiDCache     (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdFillRed      (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdFillGreen    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdFillBlue     (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdFillYellow   (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdFillPurple   (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdFillCyan     (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdFillBlack    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdFillWhite    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmd5391CrashTest1(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmd5391CrashTest2(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmd5391CrashTest3(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmd536xCrashTest1(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmd536xCrashTest2(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdFill1        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdFill2        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdFill3        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdFill4        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdFill5        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdFill6        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdFill7        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdLine1        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdLine2        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdLine3        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdLine4        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdLine5        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdLine6        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdLine7        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdGrad1        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdGrad2        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdGrad3        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdBlt1         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdBlt2         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdBlt3         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdBlt4         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdBlt5         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdBlt6         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdBlt7         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdBlt8         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdBlt9         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdBlt10        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdBlt11        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdBlt12        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdBlt13        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdBlt14        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdDMA1         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdBlend1       (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdBlend2       (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdBlend3       (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCompPass0    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCompPass1    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCompPass2    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCompPass3    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCompPass45   (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCompose1     (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCompose2     (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCompose3     (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCompose4     (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCompose5     (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCcnv1        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCcnv2        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCcnv3        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCcnv4        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCcnv5        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCcnv6        (INT32 i4Argc, const CHAR ** szArgv);
#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)||defined(CC_MT5398)
static INT32 _GfxEmuCmdCcnv7        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdReadArgbOrder(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdWriteArgbOrder(INT32 i4Argc, const CHAR ** szArgv);

#endif
static INT32 _GfxEmuCmdStrBlt1      (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdStrBlt2      (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdAlphaMap1    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdAlphaMap2    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdAlphaMap3    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdAlphaMap4    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdRop1         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdRop2         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdRop3         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmd2DMemComp1   (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmd2DMemComp2   (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdComposeLoop1 (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdComposeLoop2 (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdComposeLoop3 (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdComposeLoop4 (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdComposeLoop5 (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdComposeLoop6 (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdIdx2DirBlt1  (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdIdx2DirBlt2  (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdIdx2DirBlt3  (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdIdx2DirBlt4  (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdHor2VerLin1  (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdHor2VerLin2  (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdHor2VerLin3  (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdHor2VerLin4  (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdSubPicDec1   (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCompress1    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdOnePassCompression (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdGeneralAlcom    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmd5387CrashTest1    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCompress2(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCompress3(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdWrap1        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdWorkBuf01    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdOsdBitblt    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdPerf1        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdPerf2        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdPerf3(INT32 i4Argc, const CHAR ** szArgv);



static INT32 _GfxEmuCmdSDFifoThrs   (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCMDFifoThrs  (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdPOSTFifoThrs (INT32 i4Argc, const CHAR ** szArgv);

static INT32 _GfxBufferCompare(UINT32 *pu4Cm, UINT32 *pu4Hw, UINT32 u4Num);
static void _GfxEmuCreateBuffers(void);
static INT32 _CLI_CMD(const CHAR* szCmd, ...);
//static INT32 _GfxTestText(INT32 i4Argc, const CHAR ** szArgv);

//static void _GfxSetMidWriteRegion(void);
static INT32 _GfxEmuCmdVFull(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuArgb1555(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdComposeLoopTest(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdOverlapBlt(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdPremultAlcom(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdStretchAlcom(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdCompressMF(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdStress(INT32 i4Argc, const CHAR ** szArgv);
#if defined(CC_MT5368)||defined(CC_MT5396)
static INT32 _GfxEmuCmdRop4(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdScaler(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdScalerRandom(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdScalerAlphaEdge(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuDualChannelGeneralAlcom(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuIOMMUFill(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmddualFill2(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdScalerSeamLessClip(INT32 i4Argc, const CHAR ** szArgv);
#endif

#if defined(CC_MT5389)
static INT32 _GfxEmuCmd25dScaler(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmd25dScalerRandom(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmd25dScalerSeamLessClip(INT32 i4Argc, const CHAR ** szArgv);
#endif
#if defined(CC_MT5389)||defined(CC_MT5398)
static INT32 _GfxEmuCmdRGB0(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdRop5(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdGeneralAlcomMaxAlpha(INT32 i4Argc, const CHAR ** szArgv);
#endif

#if defined(CC_MT5389)||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5398)
static INT32 _GfxEmuCmdRop4(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuIOMMUAlcomCompress(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuIOMMU(INT32 i4Argc, const CHAR ** szArgv);
INT32 _VGfxSkipCfgData(UINT32 *pu4CurrOfst, char *pu4FileCfg);
INT32 _VGfxGetCfgFileName(UINT32 *pu4CurrOfst, BYTE arbFileName[], char *pu4FileCfg);
INT32 _VGfxGetVfgFileName(BYTE cfgFileName[], BYTE vfgFileName[]);
UINT32 _VGfxGetVal(UINT32 *pu4CurrOfst, char *pu4FileCfg);
void _VGfxGetCoeff(UINT32 *pu4GetData, UINT32 *pu4Ofst, char *pu4FileCfg);
INT32 _VGfxGetFileName(UINT32 *pu4CurrOfst, BYTE arbFileName[], char *pu4FileCfg);
static INT32 _GfxEmuCmdIdx2DirBlt5(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdIdx2DirAlcom(INT32 i4Argc, const CHAR ** szArgv);
#endif

#if defined(CC_MT5398)
static INT32 _GfxEmuCmdRop6(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdClrRzAlcom(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdSrcAlphaReplace(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdSrcRectColor(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdFlashFormatConv(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuIOMMUAlcomCompress2(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuIOMMUAlcomCompress3(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdConvMMU(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxEmuCmdFlashConv3BMMU(INT32 i4Argc, const CHAR ** szArgv);

#endif

static INT32 _GfxEmuCmdAutoRunning(INT32 i4Argc, const CHAR ** szArgv);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static UINT32 _u4FbmStartAddr;
static UINT32 _u4FbmTotalSize;
static CHAR * _szCurColorMode = "argb8888";

static UINT32 _szDstModeIndex = 14;

static UINT32 _u4CurColorModeIndex = 0;
static UINT32 _u4CurColor = 0xffffffff;
static UINT32 _u4GfxPitch = 2880;

static UINT32 _u4CanvasWidth = 720;
static UINT32 _u4ScreenWidth = 720;
static UINT32 _u4ScreenHeight = 480;
static BOOL _fgTimerInited = FALSE;

#endif // #if defined(GFX_ENABLE_SW_MODE)

// cli "gfx.emu" command table
static CLI_EXEC_T _arGfxEmuTbl[] =
{
#if defined(GFX_ENABLE_SW_MODE)
    {"init",        "i",    _GfxEmuCmdInit,         NULL,   "Init whole system (when boot)", CLI_SUPERVISOR},
    {"mode",        "m",    _GfxEmuCmdMode,         NULL,   "Mode {sw | hw} to set engine mode", CLI_SUPERVISOR},
    {"color",       "c",    _GfxEmuCmdColor,        NULL,   "Set default color", CLI_SUPERVISOR},
    {"5391",        NULL,   _GfxEmuCmd5391,         NULL,   "5391 fast test all items", CLI_SUPERVISOR},
    {"536x",        NULL,   _GfxEmuCmd536x,         NULL,   "536x fast test all items", CLI_SUPERVISOR},
    {"auto",        "go",   _GfxEmuCmdAuto,         NULL,   "Auto test all items", CLI_SUPERVISOR},
    {"endcache",    NULL,   _GfxEmuCmdEnDCache,     NULL,   "Enable D-Cache", CLI_SUPERVISOR},
    {"didcache",    NULL,   _GfxEmuCmdDiDCache,     NULL,   "Disable D-Cache", CLI_SUPERVISOR},
    {"5391crash1",  NULL,   _GfxEmuCmd5391CrashTest1, NULL, "5391 HW Crash Test 1", CLI_SUPERVISOR},
    {"5391crash2",  NULL,   _GfxEmuCmd5391CrashTest2, NULL, "5391 HW Crash Test 2 (random)", CLI_SUPERVISOR},
    {"5391crash3",  NULL,   _GfxEmuCmd5391CrashTest3, NULL, "5391 HW Crash Test 3 (random)", CLI_SUPERVISOR},
    {"536xcrash1",  NULL,   _GfxEmuCmd536xCrashTest1, NULL, "536x HW Crash Test 1", CLI_SUPERVISOR},
    {"536xcrash2",  NULL,   _GfxEmuCmd536xCrashTest2, NULL, "536x HW Crash Test 2 (random)", CLI_SUPERVISOR},
    {"fill1",       "f1",   _GfxEmuCmdFill1,        NULL,   "Rectangle fill pattern 1", CLI_SUPERVISOR},
    {"fill2",       "f2",   _GfxEmuCmdFill2,        NULL,   "Rectangle fill pattern 2", CLI_SUPERVISOR},
    {"fill3",       "f3",   _GfxEmuCmdFill3,        NULL,   "Rectangle fill pattern 3 (random)", CLI_SUPERVISOR},
    {"fill4",       "f4",   _GfxEmuCmdFill4,        NULL,   "Rectangle fill pattern 4 (random, hw & sw reset)", CLI_SUPERVISOR},
    {"fill5",       "f5",   _GfxEmuCmdFill5,        NULL,   "Rectangle fill pattern 5 (random, only one hw reset)", CLI_SUPERVISOR},
    {"fill6",       "f6",   _GfxEmuCmdFill6,        NULL,   "Rectangle fill pattern 6 (req_interval)", CLI_SUPERVISOR},
    {"fill7",       "f7",   _GfxEmuCmdFill7,        NULL,   "Rectangle fill pattern 7 (random, only run hw)", CLI_SUPERVISOR},
    {"red",         "fr",   _GfxEmuCmdFillRed,      NULL,   "Red screen", CLI_SUPERVISOR},
    {"green",       "fg",   _GfxEmuCmdFillGreen,    NULL,   "Green screen", CLI_SUPERVISOR},
    {"blue",        "fb",   _GfxEmuCmdFillBlue,     NULL,   "Blue screen", CLI_SUPERVISOR},
    {"yellow",      "fy",   _GfxEmuCmdFillYellow,   NULL,   "Yellow screen", CLI_SUPERVISOR},
    {"purple",      "fp",   _GfxEmuCmdFillPurple,   NULL,   "Purple screen", CLI_SUPERVISOR},
    {"cyan",        "fc",   _GfxEmuCmdFillCyan,     NULL,   "Cyan screen", CLI_SUPERVISOR},
    {"black",       "cls",  _GfxEmuCmdFillBlack,    NULL,   "Black screen", CLI_SUPERVISOR},
    {"white",       "fw",   _GfxEmuCmdFillWhite,    NULL,   "White screen", CLI_SUPERVISOR},
    {"line1",       "l1",   _GfxEmuCmdLine1,        NULL,   "Draw line pattern 1", CLI_SUPERVISOR},
    {"line2",       "l2",   _GfxEmuCmdLine2,        NULL,   "Draw line pattern 2 (random)", CLI_SUPERVISOR},
    {"line3",       "l3",   _GfxEmuCmdLine3,        NULL,   "Draw line pattern 3", CLI_SUPERVISOR},
    {"line4",       "l4",   _GfxEmuCmdLine4,        NULL,   "Draw line pattern 4", CLI_SUPERVISOR},
    {"line5",       "l5",   _GfxEmuCmdLine5,        NULL,   "Draw line pattern 5 (random)", CLI_SUPERVISOR},
    {"line6",       "l6",   _GfxEmuCmdLine6,        NULL,   "Draw line pattern 6 (random)", CLI_SUPERVISOR},
    {"line7",       "l7",   _GfxEmuCmdLine7,        NULL,   "Draw line pattern 7 (random, only run hw)", CLI_SUPERVISOR},
    {"grad1",       "g1",   _GfxEmuCmdGrad1,        NULL,   "Gradient fill 1", CLI_SUPERVISOR},
    {"grad2",       "g2",   _GfxEmuCmdGrad2,        NULL,   "Gradient fill 2", CLI_SUPERVISOR},
    {"grad3",       "g3",   _GfxEmuCmdGrad3,        NULL,   "Gradient fill 3", CLI_SUPERVISOR},
    {"bitblt1",     "b1",   _GfxEmuCmdBlt1,         NULL,   "Bitblt test 1", CLI_SUPERVISOR},
    {"bitblt2",     "b2",   _GfxEmuCmdBlt2,         NULL,   "Bitblt test 2", CLI_SUPERVISOR},
    {"bitblt3",     "b3",   _GfxEmuCmdBlt3,         NULL,   "Bitblt test 3", CLI_SUPERVISOR},
    {"bitblt4",     "b4",   _GfxEmuCmdBlt4,         NULL,   "Bitblt test 4", CLI_SUPERVISOR},
    {"bitblt5",     "b5",   _GfxEmuCmdBlt5,         NULL,   "Bitblt test 5", CLI_SUPERVISOR},
    {"bitblt6",     "b6",   _GfxEmuCmdBlt6,         NULL,   "Bitblt test 6 (random)", CLI_SUPERVISOR},
    {"bitblt7",     "b7",   _GfxEmuCmdBlt7,         NULL,   "Bitblt test 7", CLI_SUPERVISOR},
    {"bitblt8",     "b8",   _GfxEmuCmdBlt8,         NULL,   "Bitblt test 8 (random)", CLI_SUPERVISOR},
    {"bitblt9",     "b9",   _GfxEmuCmdBlt9,         NULL,   "Bitblt test 9 (pix-by-pix)", CLI_SUPERVISOR},
    {"bitblt10",    "b10",  _GfxEmuCmdBlt10,        NULL,   "Bitblt test 10 (random, only run hw)", CLI_SUPERVISOR},
    {"bitblt11",    "b11",  _GfxEmuCmdBlt11,        NULL,   "Bitblt test 11 (basic flip & mirror)", CLI_SUPERVISOR},
    {"bitblt12",    "b12",  _GfxEmuCmdBlt12,        NULL,   "Bitblt test 12 (random, flip | mirror)", CLI_SUPERVISOR},
    {"bitblt13",    "b13",  _GfxEmuCmdBlt13,        NULL,   "Bitblt test 13 (only run hw, flip/mirr)", CLI_SUPERVISOR},
    {"bitblt14",    "b14",  _GfxEmuCmdBlt14,        NULL,   "Bitblt test 14 (single, flip | mirror)", CLI_SUPERVISOR},
    {"blend1",      "bl1",  _GfxEmuCmdBlend1,       NULL,   "Alpha blend test 1", CLI_SUPERVISOR},
    {"blend2",      "bl2",  _GfxEmuCmdBlend2,       NULL,   "Alpha blend test 2 (random)", CLI_SUPERVISOR},
    {"blend3",      "bl3",  _GfxEmuCmdBlend3,       NULL,   "Alpha blend test 3 (random, only run hw)", CLI_SUPERVISOR},
    {"ccnv1",       "cn1",  _GfxEmuCmdCcnv1,        NULL,   "YCbCr2RGB test 1 (420mb block)", CLI_SUPERVISOR},
    {"ccnv2",       "cn2",  _GfxEmuCmdCcnv2,        NULL,   "YCbCr2RGB test 2 (420 linear scan)", CLI_SUPERVISOR},
    {"ccnv3",       "cn3",  _GfxEmuCmdCcnv3,        NULL,   "YCbCr2RGB test 3 (420mb swap)", CLI_SUPERVISOR},
    {"ccnv4",       "cn4",  _GfxEmuCmdCcnv4,        NULL,   "YCbCr2RGB test 4 (compare 420mb block with 420lin)", CLI_SUPERVISOR},
    {"ccnv5",       "cn5",  _GfxEmuCmdCcnv5,        NULL,   "YCbCr2RGB test 5 (420mb mergetop)", CLI_SUPERVISOR},
    {"ccnv6",       "cn6",  _GfxEmuCmdCcnv6,        NULL,   "YCbCr2RGB test 6 (422 linear scan)", CLI_SUPERVISOR},
    {"strblt1",     "sb1",  _GfxEmuCmdStrBlt1,      NULL,   "Stretch test 1", CLI_SUPERVISOR},
    {"strblt2",     "sb2",  _GfxEmuCmdStrBlt2,      NULL,   "Stretch test 2", CLI_SUPERVISOR},
    {"dma1",         "d1",  _GfxEmuCmdDMA1,         NULL,   "DMA test 1", CLI_SUPERVISOR},
    {"alphamap1",   "am1",  _GfxEmuCmdAlphaMap1,    NULL,   "Alpha map test 1", CLI_SUPERVISOR},
    {"alphamap2",   "am2",  _GfxEmuCmdAlphaMap2,    NULL,   "Alpha map test 2", CLI_SUPERVISOR},
    {"alphamap3",   "am3",  _GfxEmuCmdAlphaMap3,    NULL,   "Alpha map test 3 (random)", CLI_SUPERVISOR},
    {"alphamap4",   "am4",  _GfxEmuCmdAlphaMap4,    NULL,   "Alpha map test 4 (random, only run hw)", CLI_SUPERVISOR},
    {"rop1",         "r1",  _GfxEmuCmdRop1,         NULL,   "ROP test 1", CLI_SUPERVISOR},
    {"rop2",         "r2",  _GfxEmuCmdRop2,         NULL,   "ROP test 2 (random)", CLI_SUPERVISOR},
    {"rop3",         "r3",  _GfxEmuCmdRop3,         NULL,   "ROP test 3 (random, only run hw)", CLI_SUPERVISOR},
    {"2dmemcomp1",   NULL,  _GfxEmuCmd2DMemComp1,   NULL,   "2D mem compare test 1 (random)", CLI_SUPERVISOR},
    {"2dmemcomp2",   NULL,  _GfxEmuCmd2DMemComp2,   NULL,   "2D mem compare test 2 (random, only run hw)", CLI_SUPERVISOR},
    {"composelp1", "cpl1",  _GfxEmuCmdComposeLoop1, NULL,   "Alpha compose loop test 1", CLI_SUPERVISOR},
    {"composelp2", "cpl2",  _GfxEmuCmdComposeLoop2, NULL,   "Alpha compose loop test 2 (pix-by-pix)", CLI_SUPERVISOR},
    {"composelp3", "cpl3",  _GfxEmuCmdComposeLoop3, NULL,   "Alpha compose loop test 3 (random)", CLI_SUPERVISOR},
    {"composelp4", "cpl4",  _GfxEmuCmdComposeLoop4, NULL,   "Alpha compose loop test 4 (random)", CLI_SUPERVISOR},
    {"composelp5", "cpl5",  _GfxEmuCmdComposeLoop5, NULL,   "Alpha compose loop test 5", CLI_SUPERVISOR},
    {"composelp6", "cpl6",  _GfxEmuCmdComposeLoop6, NULL,   "Alpha compose loop test 6 (random, only run hw)", CLI_SUPERVISOR},
    {"idx2dir1",     NULL,  _GfxEmuCmdIdx2DirBlt1,  NULL,   "Idx to direct color bitblt test 1", CLI_SUPERVISOR},
    {"idx2dir2",     NULL,  _GfxEmuCmdIdx2DirBlt2,  NULL,   "Idx to direct color bitblt test 2 (random)", CLI_SUPERVISOR},
    {"idx2dir3",     NULL,  _GfxEmuCmdIdx2DirBlt3,  NULL,   "Idx to direct color bitblt test 3", CLI_SUPERVISOR},
    {"idx2dir4",     NULL,  _GfxEmuCmdIdx2DirBlt4,  NULL,   "Idx to direct color bitblt test 4 (random, only run hw)", CLI_SUPERVISOR},
    {"hor2verlin1",  NULL,  _GfxEmuCmdHor2VerLin1,  NULL,   "Hori to Vert line test 1", CLI_SUPERVISOR},
    {"hor2verlin2",  NULL,  _GfxEmuCmdHor2VerLin2,  NULL,   "Hori to Vert line test 2", CLI_SUPERVISOR},
    {"hor2verlin3",  NULL,  _GfxEmuCmdHor2VerLin3,  NULL,   "Hori to Vert line test 3 (random)", CLI_SUPERVISOR},
    {"hor2verlin4",  NULL,  _GfxEmuCmdHor2VerLin4,  NULL,   "Hori to Vert line test 4 (random, only run hw)", CLI_SUPERVISOR},
    {"subpic1",      NULL,  _GfxEmuCmdSubPicDec1,   NULL,   "Sub-pic decode test 1 (basic)", CLI_SUPERVISOR},
    {"5387crash",   NULL,  _GfxEmuCmd5387CrashTest1, NULL,   "mt5387 crash test", CLI_SUPERVISOR},
    {"onepasscomp",   NULL,  _GfxEmuCmdOnePassCompression, NULL,   "One pass Compression Test", CLI_SUPERVISOR},
    {"gencompose",   NULL,  _GfxEmuCmdGeneralAlcom, NULL,   "General Alpha composition", CLI_SUPERVISOR},
    {"compress1",    NULL,  _GfxEmuCmdCompress1,    NULL,   "Compress bitblt test 1 (basic)", CLI_SUPERVISOR},
    {"compress2",    NULL,  _GfxEmuCmdCompress2,    NULL,   "New Compress bitblt test 2 (basic)", CLI_SUPERVISOR},
    {"compress3",    NULL,  _GfxEmuCmdCompress3,    NULL,   "New Compress bitblt test 2 (basic)", CLI_SUPERVISOR},
    {"wrap1",        "w1",  _GfxEmuCmdWrap1,        NULL,   "Wrap test 1", CLI_SUPERVISOR},
    {"workbuf1",   "wbf1",  _GfxEmuCmdWorkBuf01,    NULL,   "Work buf test 1", CLI_SUPERVISOR},
    {"compass0",    "cmp0", _GfxEmuCmdCompPass0,    NULL,   "Composition pass 0", CLI_SUPERVISOR},
    {"compass1",    "cmp1", _GfxEmuCmdCompPass1,    NULL,   "Composition pass 1", CLI_SUPERVISOR},
    {"compass2",    "cmp2", _GfxEmuCmdCompPass2,    NULL,   "Composition pass 2", CLI_SUPERVISOR},
    {"compass3",    "cmp3", _GfxEmuCmdCompPass3,    NULL,   "Composition pass 3", CLI_SUPERVISOR},
    {"compass45",   "cmp45",_GfxEmuCmdCompPass45,   NULL,   "Composition pass 4 & 5", CLI_SUPERVISOR},
    {"compose1",    "cp1",  _GfxEmuCmdCompose1,     NULL,   "Alpha compose test 1", CLI_SUPERVISOR},
    {"compose2",    "cp2",  _GfxEmuCmdCompose2,     NULL,   "Alpha compose test 2", CLI_SUPERVISOR},
    {"compose3",    "cp3",  _GfxEmuCmdCompose3,     NULL,   "Alpha compose test 3", CLI_SUPERVISOR},
    {"compose4",    "cp4",  _GfxEmuCmdCompose4,     NULL,   "Alpha compose test 4", CLI_SUPERVISOR},
    {"compose5",    "cp5",  _GfxEmuCmdCompose5,     NULL,   "Alpha compose test 5", CLI_SUPERVISOR},
    {"osdtest",     "ot",   _GfxEmuCmdOsdBitblt,    NULL,   "Osd bitblt test 1", CLI_SUPERVISOR},
    {"perf1",       "pf1",  _GfxEmuCmdPerf1,        NULL,   "Performance test 1", CLI_SUPERVISOR},
    {"perf2",       "pf2",  _GfxEmuCmdPerf2,        NULL,   "Performance test 2", CLI_SUPERVISOR},
    {"perf3",       "pf3",  _GfxEmuCmdPerf3,        NULL,   "Performance test 3", CLI_SUPERVISOR},
    {"sdfifo",      "sdf",  _GfxEmuCmdSDFifoThrs,   NULL,   "SD fifo threshold", CLI_SUPERVISOR},
    {"cmdfifo",     "cmdf", _GfxEmuCmdCMDFifoThrs,  NULL,   "CMD fifo threshold", CLI_SUPERVISOR},
    {"postfifo",    "postf",_GfxEmuCmdPOSTFifoThrs, NULL,   "POST fifo threshold", CLI_SUPERVISOR},
    {"vfull",    "vf", _GfxEmuCmdVFull, NULL,   "full-range ccnv ", CLI_SUPERVISOR},    
    {"argb1555",    "1555", _GfxEmuArgb1555, NULL,   "ARGB 1555", CLI_SUPERVISOR},
    {"clt",    "clt", _GfxEmuCmdComposeLoopTest, NULL,   "SLT Scaler Dram", CLI_SUPERVISOR},    
    {"overlapblt",    "olblt", _GfxEmuCmdOverlapBlt, NULL,   "overlap bitblt", CLI_SUPERVISOR},
    {"premultalcom",    "premult", _GfxEmuCmdPremultAlcom, NULL,   "Premultiplied Alpha Composition", CLI_SUPERVISOR},
    {"stretchalcom",    "strcom", _GfxEmuCmdStretchAlcom, NULL,   "Stretch Alpha Composition", CLI_SUPERVISOR},
    {"compmf",    "cmf", _GfxEmuCmdCompressMF, NULL,   "Compression Mirror/Flip", CLI_SUPERVISOR},
    {"stress",    "stress",_GfxEmuCmdStress, NULL,   "Stress", CLI_SUPERVISOR},    
#if defined(CC_MT5368)||defined(CC_MT5396)
    {"mmu",    "mmu", _GfxEmuIOMMU, NULL,   "2D graphic iommu test ", CLI_SUPERVISOR},
    {"ccnv7",       "cn7",  _GfxEmuCmdCcnv7,        NULL,   "YCbCr2RGB test 7 YCbCr2RGB24bit test", CLI_SUPERVISOR},
    {"readargborder",       "rorder",  _GfxEmuCmdReadArgbOrder,        NULL,   "Read Argb Order  test", CLI_SUPERVISOR},
    {"writeargborder",       "wrorder",  _GfxEmuCmdWriteArgbOrder,        NULL,   "Read Argb Order  test", CLI_SUPERVISOR},
    {"idx2dir5",	 NULL,	_GfxEmuCmdIdx2DirBlt5,	NULL,	"Idx to direct color bitblt test 5 (src pitch enable)", CLI_SUPERVISOR},
    {"idx2diralcom",	"i2dcomp", _GfxEmuCmdIdx2DirAlcom, NULL,	"idx2dir Alpha Composition", CLI_SUPERVISOR},
    {"ropmutimode",	 "rop4",	_GfxEmuCmdRop4,	NULL,	"rop multiply mode test  (for mt5368 feature)", CLI_SUPERVISOR},
    {"scaler",  "sc",	_GfxEmuCmdScaler, NULL,	"gfx scaler test  (for mt5368 feature)", CLI_SUPERVISOR},
    {"scaler_go",  "sc_go",	_GfxEmuCmdScalerRandom, NULL,	"gfx scaler test random patten  (for mt5368 feature)", CLI_SUPERVISOR},
    {"scaler_alpha_edge",  "sc_ae",	_GfxEmuCmdScalerAlphaEdge, NULL,	"gfx scaler test alpha edge  (for mt5368 feature)", CLI_SUPERVISOR},
    {"scaler_seamlessclip",  "sc_clip",	_GfxEmuCmdScalerSeamLessClip, NULL,	"gfx scaler test seamless clip  (for mt5368 feature)", CLI_SUPERVISOR},
    {"dualchannel",  "dualch",	_GfxEmuDualChannelGeneralAlcom, NULL,	"test dual channel and gfx group1,3  (for mt5396 feature)", CLI_SUPERVISOR},
    {"mmu_cmdque",  "mcq",	_GfxEmuIOMMUAlcomCompress, NULL,	"gfx mmu test include alpha comp and compression in one cmdque", CLI_SUPERVISOR},
    {"mmu_fill",  "mfill",	_GfxEmuIOMMUFill, NULL,	"gfx mmu test fill rect", CLI_SUPERVISOR},
    {"dual_fill",  "dualfill",	_GfxEmuCmddualFill2, NULL,	"gfx dual channel fill rect test" , CLI_SUPERVISOR},
    {"scaler_seamlessclip",  "sc_clip",	_GfxEmuCmdScalerSeamLessClip, NULL,	"gfx scaler test seamless clip  (for mt5368 feature)", CLI_SUPERVISOR},
	#endif
	#if defined(CC_MT5389)	
    {"ropmutimode",	 "rop4",	_GfxEmuCmdRop4,	NULL,	"rop multiply mode test  (for mt5368 feature)", CLI_SUPERVISOR},
    {"25dscaler",    "25dsc", _GfxEmuCmd25dScaler, NULL,   "2.5 D graphic basic test ", CLI_SUPERVISOR},
    {"25dscaler_go",    "25dsc_go", _GfxEmuCmd25dScalerRandom, NULL,   "2.5 D graphic random pattern auto test ", CLI_SUPERVISOR},
    {"gencompmaxalpha",   "compmaxalpha", _GfxEmuCmdGeneralAlcomMaxAlpha, NULL,   "2.5 D graphic random pattern auto test ", CLI_SUPERVISOR},
    {"rop5",	 "r5", _GfxEmuCmdRop5, NULL,   "gfx test rop mode MAX and MIN ", CLI_SUPERVISOR},
    {"alpha0rgb0",   "argb0", _GfxEmuCmdRGB0, NULL,   "gfx test rgb0 when alpha is 0 ", CLI_SUPERVISOR},
    {"25dscaler_seamlessclip",  "25dsc_clip",	_GfxEmuCmd25dScalerSeamLessClip, NULL,	"gfx scaler test seamless clip  (for mt5368 feature)", CLI_SUPERVISOR},
    {"idx2dir5",	 NULL,	_GfxEmuCmdIdx2DirBlt5,	NULL,	"Idx to direct color bitblt test 5 (src pitch enable)", CLI_SUPERVISOR},
    {"readargborder",       "rorder",  _GfxEmuCmdReadArgbOrder,        NULL,   "Read Argb Order  test", CLI_SUPERVISOR},
    {"ccnv7",       "cn7",  _GfxEmuCmdCcnv7,        NULL,   "YCbCr2RGB test 7 YCbCr2RGB24bit test", CLI_SUPERVISOR},
    {"idx2diralcom",	"i2dcomp", _GfxEmuCmdIdx2DirAlcom, NULL,	"idx2dir Alpha Composition", CLI_SUPERVISOR},
    {"writeargborder",       "wrorder",  _GfxEmuCmdWriteArgbOrder,        NULL,   "Read Argb Order  test", CLI_SUPERVISOR},
    {"mmu",    "mmu", _GfxEmuIOMMU, NULL,   "2D graphic iommu test ", CLI_SUPERVISOR},
    {"mmu_cmdque",  "mcq",	_GfxEmuIOMMUAlcomCompress, NULL,	"gfx mmu test include alpha comp and compression in one cmdque", CLI_SUPERVISOR},
#endif
	#if defined(CC_MT5398)
	{"rop6",	 "r6", _GfxEmuCmdRop6, NULL,   "gfx test rop mode for colorize with XOR_COLOR", CLI_SUPERVISOR},
	{"clrrzalcom",	 "cralc", _GfxEmuCmdClrRzAlcom, NULL,   "test onepass colorize + alpha composition", CLI_SUPERVISOR},
	{"alphareplace",	 "alrep", _GfxEmuCmdSrcAlphaReplace, NULL,	"test Blit Blend ColorAlpha(using specific alpha to replace src alpha)", CLI_SUPERVISOR},
	{"srcrectcolor",	 "srcrc", _GfxEmuCmdSrcRectColor, NULL,	"test one pass to using RectColor as rect src", CLI_SUPERVISOR},
	{"flashconv",	 "fconv", _GfxEmuCmdFlashFormatConv, NULL, "test Andriod/Flush format convertion", CLI_SUPERVISOR},
	{"mmu_ac2",	 "mmu_ac2", _GfxEmuIOMMUAlcomCompress2, NULL, "test different mid in same cmdque", CLI_SUPERVISOR},
	{"mmu_ac3",  "mmu_ac3", _GfxEmuIOMMUAlcomCompress3, NULL, "test different state agent (disable/enable) in same cmdque", CLI_SUPERVISOR},
	{"mmu_Convargb",  "mmu_Convargb", _GfxEmuCmdConvMMU, NULL, "test MMU YCbCr to ARGB8888", CLI_SUPERVISOR},
	{"mmu_Conv3b",  "mmu_Conv3b", _GfxEmuCmdFlashConv3BMMU, NULL, "test MMU YCbCr to ARGB8888", CLI_SUPERVISOR},
	#endif	
	#if defined(CC_MT5398)||defined(CC_MT5389)
    {"alpha0rgb0",   "argb0", _GfxEmuCmdRGB0, NULL,   "gfx test rgb0 when alpha is 0 ", CLI_SUPERVISOR},
	{"gencompmaxalpha",   "compmaxalpha", _GfxEmuCmdGeneralAlcomMaxAlpha, NULL,   "2.5 D graphic random pattern auto test ", CLI_SUPERVISOR},
	{"rop5",	 "r5", _GfxEmuCmdRop5, NULL,   "gfx test rop mode MAX and MIN ", CLI_SUPERVISOR},
	#endif
	
	{"autorunning",	 "autorun", _GfxEmuCmdAutoRunning, NULL,   "2.5 D graphic basic test ", CLI_SUPERVISOR},

#endif // #if defined(GFX_ENABLE_SW_MODE)

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


#if defined(GFX_ENABLE_SW_MODE)
// 147 named color
static NAMED_COLOR_T _arNamedColor[] =
{
    {"aliceblue",               0xFFF0F8FF},
    {"antiquewhite",            0xFFFAEBD7},
    {"aqua",                    0xFF00FFFF},
    {"aquamarine",              0xFF7FFFD4},
    {"azure",                   0xFFF0FFFF},
    {"beige",                   0xFFF5F5DC},
    {"bisque",                  0xFFFFE4C4},
    {"black",                   0xFF000001},
    {"blanchedalmond",          0xFFFFEBCD},
    {"blue",                    0xFF0000FF},
    {"blueviolet",              0xFF8A2BE2},
    {"brown",                   0xFFA52A2A},
    {"burlywood",               0xFFDEB887},
    {"cadetblue",               0xFF5F9EA0},
    {"chartreuse",              0xFF7FFF00},
    {"chocolate",               0xFFD2691E},
    {"coral",                   0xFFFF7F50},
    {"cornflowerblue",          0xFF6495ED},
    {"cornsilk",                0xFFFFF8DC},
    {"crimson",                 0xFFDC143C},
    {"cyan",                    0xFF00FFFF},
    {"darkblue",                0xFF00008B},
    {"darkcyan",                0xFF008B8B},
    {"darkgoldenrod",           0xFFB8860B},
    {"darkgray",                0xFFA9A9A9},
    {"darkgreen",               0xFF006400},
    {"darkgrey",                0xFFA9A9A9},
    {"darkkhaki",               0xFFBDB76B},
    {"darkmagenta",             0xFF8B008B},
    {"darkolivegreen",          0xFF556B2F},
    {"darkorange",              0xFFFF8C00},
    {"darkorchid",              0xFF9932CC},
    {"darkred",                 0xFF8B0000},
    {"darksalmon",              0xFFE9967A},
    {"darkseagreen",            0xFF8FBC8F},
    {"darkslateblue",           0xFF483D8B},
    {"darkslategray",           0xFF2F4F4F},
    {"darkslategrey",           0xFF2F4F4F},
    {"darkturquoise",           0xFF00CED1},
    {"darkviolet",              0xFF9400D3},
    {"deeppink",                0xFFFF1493},
    {"deepskyblue",             0xFF00BFFF},
    {"dimgray",                 0xFF696969},
    {"dimgrey",                 0xFF696969},
    {"dodgerblue",              0xFF1E90FF},
    {"firebrick",               0xFFB22222},
    {"floralwhite",             0xFFFFFAF0},
    {"forestgreen",             0xFF228B22},
    {"fuchsia",                 0xFFFF00FF},
    {"gainsboro",               0xFFDCDCDC},
    {"ghostwhite",              0xFFF8F8FF},
    {"gold",                    0xFFFFD700},
    {"goldenrod",               0xFFDAA520},
    {"gray",                    0xFF808080},
    {"green",                   0xFF008000},
    {"greenyellow",             0xFFADFF2F},
    {"grey",                    0xFF808080},
    {"honeydew",                0xFFF0FFF0},
    {"hotpink",                 0xFFFF69B4},
    {"indianred",               0xFFCD5C5C},
    {"indigo",                  0xFF4B0082},
    {"ivory",                   0xFFFFFFF0},
    {"khaki",                   0xFFF0E68C},
    {"lavender",                0xFFE6E6FA},
    {"lavenderblush",           0xFFFFF0F5},
    {"lawngreen",               0xFF7CFC00},
    {"lemonchiffon",            0xFFFFFACD},
    {"lightblue",               0xFFADD8E6},
    {"lightcoral",              0xFFF08080},
    {"lightcyan",               0xFFE0FFFF},
    {"lightgoldenrodyellow",    0xFFFAFAD2},
    {"lightgray",               0xFFD3D3D3},
    {"lightgreen",              0xFF90EE90},
    {"lightgrey",               0xFFD3D3D3},
    {"lightpink",               0xFFFFB6C1},
    {"lightsalmon",             0xFFFFA07A},
    {"lightseagreen",           0xFF20B2AA},
    {"lightskyblue",            0xFF87CEFA},
    {"lightslategray",          0xFF778899},
    {"lightslategrey",          0xFF778899},
    {"lightsteelblue",          0xFFB0C4DE},
    {"lightyellow",             0xFFFFFFE0},
    {"lime",                    0xFF00FF00},
    {"limegreen",               0xFF32CD32},
    {"linen",                   0xFFFAF0E6},
    {"magenta",                 0xFFFF00FF},
    {"maroon",                  0xFF800000},
    {"mediumaquamarine",        0xFF66CDAA},
    {"mediumblue",              0xFF0000CD},
    {"mediumorchid",            0xFFBA55D3},
    {"mediumpurple",            0xFF9370DB},
    {"mediumseagreen",          0xFF3CB371},
    {"mediumslateblue",         0xFF7B68EE},
    {"mediumspringgreen",       0xFF00FA9A},
    {"mediumturquoise",         0xFF48D1CC},
    {"mediumvioletred",         0xFFC71585},
    {"midnightblue",            0xFF191970},
    {"mintcream",               0xFFF5FFFA},
    {"mistyrose",               0xFFFFE4E1},
    {"moccasin",                0xFFFFE4B5},
    {"navajowhite",             0xFFFFDEAD},
    {"navy",                    0xFF000080},
    {"oldlace",                 0xFFFDF5E6},
    {"olive",                   0xFF808000},
    {"olivedrab",               0xFF6B8E23},
    {"orange",                  0xFFFFA500},
    {"orangered",               0xFFFF4500},
    {"orchid",                  0xFFDA70D6},
    {"palegoldenrod",           0xFFEEE8AA},
    {"palegreen",               0xFF98FB98},
    {"paleturquoise",           0xFFAFEEEE},
    {"palevioletred",           0xFFDB7093},
    {"papayawhip",              0xFFFFEFD5},
    {"peachpuff",               0xFFFFDAB9},
    {"peru",                    0xFFCD853F},
    {"pink",                    0xFFFFC0CB},
    {"plum",                    0xFFDDA0DD},
    {"powderblue",              0xFFB0E0E6},
    {"purple",                  0xFF800080},
    {"red",                     0xFFFF0000},
    {"rosybrown",               0xFFBC8F8F},
    {"royalblue",               0xFF4169E1},
    {"saddlebrown",             0xFF8B4513},
    {"salmon",                  0xFFFA8072},
    {"sandybrown",              0xFFF4A460},
    {"seagreen",                0xFF2E8B57},
    {"seashell",                0xFFFFF5EE},
    {"sienna",                  0xFFA0522D},
    {"silver",                  0xFFC0C0C0},
    {"skyblue",                 0xFF87CEEB},
    {"slateblue",               0xFF6A5ACD},
    {"slategray",               0xFF708090},
    {"slategrey",               0xFF708090},
    {"snow",                    0xFFFFFAFA},
    {"springgreen",             0xFF00FF7F},
    {"steelblue",               0xFF4682B4},
    {"tan",                     0xFFD2B48C},
    {"teal",                    0xFF008080},
    {"thistle",                 0xFFD8BFD8},
    {"tomato",                  0xFFFF6347},
    {"turquoise",               0xFF40E0D0},
    {"violet",                  0xFFEE82EE},
    {"wheat",                   0xFFF5DEB3},
    {"white",                   0xFFFFFFFF},
    {"whitesmoke",              0xFFF5F5F5},
    {"yellow",                  0xFFFFFF00},
    {"yellowgreen",             0xFF9ACD32}
};


static UINT32 _au4ScreenWidth[] =
{
    720,
    720,
    720,
    720,
    720,
};


static UINT32 _au4ScreenHeight[] =
{
    480,
    480,
    480,
    480,
    480,
};


static CHAR * _aszColorMode[] =
{
    "argb8888",
    "argb4444",
    "argb1555",
    "rgb565",
    "rgb8",
};


static UINT32 _au4DefPitch[] =
{
    2880,
    1440,
    1440,
    1440,
    720,
};


static CHAR * _aszRopOpCode[] =
{
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "~Src",
    "~Dst",
    "Src ^ Dst",
    "~(Src ^ Dst)",
    "Src & Dst",
    "~Src & Dst",
    "Src & ~Dst",
    "~Src & ~Dst",
    "Src | Dst",
    "~Src | Dst",
    "Src | ~Dst",
    "~Src | ~Dst",
};


static CHAR * _aszAlphaComposeLoopOpCode[] =
{
    "0_Clear",
    "1_DstIn",
    "2_DstOut",
    "3_DstOver",
    "4_Src",
    "5_SrcIn",
    "6_SrcOut",
    "7_SrcOver",
    "8_Dst",
    "9_SrcAtop",
    "10_DstAtop",
    "11_Xor",
};


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** _CLI_CMD
 *  
 */
//-------------------------------------------------------------------------
static INT32 _CLI_CMD(const CHAR* szCmd, ...)
{
    CHAR szBuf[256];
    VA_LIST t_ap;

    VA_START(t_ap, szCmd);
    x_vsprintf(szBuf, szCmd, t_ap);
    VA_END(t_ap);

    return CLI_Parser(szBuf);
}


//-------------------------------------------------------------------------
/** _GfxSetDefMode
 *  
 */
//-------------------------------------------------------------------------
static INT32 _GfxSetDefMode(UINT32 u4BuffAddr, BOOL fgSrc)
{
    UINT32 u4Pitch;
    #ifdef GFX_BURST_READ
        u4Pitch = GFX_BURST_READ_ALIGN(_u4GfxPitch);
        LOG(6,"_GfxSetDefMode pitch : %d %s\n",u4Pitch,_szCurColorMode);
    #else
        u4Pitch = _u4GfxPitch;
    #endif
    _CLI_CMD("gfx.set.%s 0x%08x %s %u", 
        fgSrc ? "src" : "dst", u4BuffAddr, _szCurColorMode, u4Pitch);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxSetSdMode
 *
 */
//-------------------------------------------------------------------------
static INT32 _GfxSetSdMode(UINT32 u4BuffAddr, BOOL fgSrc)
{
    _CLI_CMD("gfx.set.%s 0x%08x %s 2880", 
        fgSrc ? "src" : "dst", u4BuffAddr, _szCurColorMode);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdColor
 *  Set Rect Color
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdColor(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Color;
    UINT8 u1A, u1R, u1G, u1B;
    UINT32 u4Color4444, u4Color1555, u4Color565;

    if (i4Argc < 2)
    {
        Printf("gfxemu.color {color}\n");
        Printf("current color is 0x%08x\n", _u4CurColor);
        return 0;
    }
    u4Color = StrToInt(szArgv[1]);

    u1A = u4Color >> 24;
    u1R = (u4Color >> 16) & 0xff;
    u1G = (u4Color >> 8) & 0xff;
    u1B = u4Color & 0xff;
    u4Color4444 = ((u1A << 8) & 0xf000) | ((u1R << 4) & 0x0f00) | (u1G & 0x00f0) | (u1B >> 4);
    u4Color1555 = (u1A == 0 ? 0 : 0x8000) | ((u1R << 7) & 0x7c00) | ((u1G << 2) & 0x03e0) | (u1B >> 3);
    u4Color565 = ((u1R << 8) & 0xf800) | ((u1G << 3) & 0x07e0) | (u1B >> 3);

    u4Color4444 |= u4Color4444 << 16;
    u4Color1555 |= u4Color1555 << 16;
    u4Color565  |= u4Color565  << 16;

    switch (_u4CurColorModeIndex)
    {
    case 1:     // argb4444
        _CLI_CMD("gfx.set.color 0x%08x", u4Color4444);
        break;
    case 2:     // argb1555
        _CLI_CMD("gfx.set.color 0x%08x", u4Color1555);
        break;
    case 3:     // rgb565
        _CLI_CMD("gfx.set.color 0x%08x", u4Color565);
        break;
    case 4:     // rgb8
        _CLI_CMD("gfx.set.color 0x%02x%02x%02x%02x", u4Color & 0xff, u4Color & 0xff, u4Color & 0xff, u4Color & 0xff);
        break;
    default:
        _CLI_CMD("gfx.set.color 0x%08x", u4Color);
        break;
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdMode
 *  Set HW or SW mode
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdMode(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc == 1)
    {
        //Printf("In Hardware Mode\n");
        GFX_DifSetMode(1);
    }
    else if (i4Argc == 2 && x_strncmp("sw", szArgv[1], 2) == 0)
    {
        //Printf("In Software Mode\n");
        GFX_DifSetMode(0);        
    }
    else if (i4Argc == 3 && x_strncmp("cm", szArgv[1], 2) == 0)
    {
        INT32 i4Index;

        for (i4Index = 0; _aszColorMode[i4Index] != (CHAR *)NULL; i4Index++)
        {
            if (x_strcmp(_aszColorMode[i4Index], szArgv[2]) == 0 ||
                ((szArgv[2][0] >= '0' && szArgv[2][0] <= '9') && (i4Index == StrToInt(szArgv[2]))))
            {
                if (i4Index > D_GFX_TEST_AVAILABLE_MODE)
                {
                    Printf("Currently mode %s is not supported in test mode\n", _aszColorMode[i4Index]);
                    return -1;
                }
                _u4CurColorModeIndex = (UINT32)i4Index;
                _szCurColorMode = _aszColorMode[i4Index];
				switch(_u4CurColorModeIndex)
				{
					case 0:
						_szDstModeIndex = 14;
						break;
					case 1:
						_szDstModeIndex = 13;
						break;
					case 2:
						_szDstModeIndex = 12;
						break;
					case 3:
						_szDstModeIndex = 11;
						break;
					case 4:
						_szDstModeIndex = 10;
						break;
					default:
						break;					
				}
                #ifdef GFX_BURST_READ
                    _u4GfxPitch = GFX_BURST_READ_ALIGN(_au4DefPitch[i4Index]);
                #else
                    _u4GfxPitch     = _au4DefPitch[i4Index];
                #endif
                _u4ScreenWidth  = _au4ScreenWidth[i4Index];
                _u4ScreenHeight = _au4ScreenHeight[i4Index];

                Printf("turn off osd plane 2\n");
                
                _CLI_CMD("osd.plane.enable 2 0");
                
                Printf("flip to list and region %d (mode %s pitch %u)\n",
                    i4Index, _szCurColorMode, _u4GfxPitch);
                
                _CLI_CMD("osd.plane.flip 2 %d", i4Index);
                
                Printf("turn on osd plane 2\n");
                
                _CLI_CMD("osd.plane.enable 2 1");

                // only ARGB8888 is big_endian
                _CLI_CMD("osd.region.set %d big_endian 0", i4Index);
                _CLI_CMD("osd.region.set %d mix_sel 1", i4Index);
                
                Printf("set gfx source and destination to default\n");
                Printf("loading data ...\n");
                Printf("color mode selection done\n");
                return 0;
            }
        }
        return -1;
    }
    else
    {
        return -1;
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdInit
 *  initialization for gfx emulation
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    //==========================================
    //...
    //==========================================
    Printf("...\n");

    // create buffers for emulation use
    _GfxEmuCreateBuffers();
    
    //_GfxSetMidWriteRegion();

#if 1
    //==========================================
    //init display flow
    //==========================================
#if !(defined(CC_MT5368)||defined(CC_MT5396))   //for 5368 FPGA run error
    _CLI_CMD("fbm.i");
#endif
    //_CLI_CMD("nptv.i");
    
    Printf("Plane mixer init ...\n");
    _CLI_CMD("pmx.i");
    Printf("Plane mixer init ... done\n");
    
    Printf("OSD init ...\n");
    _CLI_CMD("osd.init");

    // default create 7 lists
    _CLI_CMD("osd.list.create");    // 0
    _CLI_CMD("osd.list.create");    // 1
    _CLI_CMD("osd.list.create");    // 2
    _CLI_CMD("osd.list.create");    // 3
    _CLI_CMD("osd.list.create");    // 4

    _CLI_CMD("osd.region.new 720 480 %u 14 2880 0 0 720 480", BUF_SCREEN); // 0 - ARGB8888 720x480
    _CLI_CMD("osd.region.new 720 480 %u 13 1440 0 0 720 480", BUF_SCREEN); // 1 - ARGB4444 720x480
    _CLI_CMD("osd.region.new 720 480 %u 12 1440 0 0 720 480", BUF_SCREEN); // 2 - ARGB1555 720x480
    _CLI_CMD("osd.region.new 720 480 %u 11 1440 0 0 720 480", BUF_SCREEN); // 3 - RGB565 720x480
    _CLI_CMD("osd.region.new 720 480 %u 10  720 0 0 720 480", BUF_SCREEN); // 4 - ARGB_CLUT8 720x480

    _CLI_CMD("osd.region.insert 0 0");
    _CLI_CMD("osd.region.insert 1 1");
    _CLI_CMD("osd.region.insert 2 2");
    _CLI_CMD("osd.region.insert 3 3");
    _CLI_CMD("osd.region.insert 4 4");

    _CLI_CMD("osd.plane.flip 2 0"); // select region list 0 (region 0 - argb8888) as default
    _CLI_CMD("osd.sc.s 0 720 480 720 480");
    _CLI_CMD("osd.plane.enable 2 1");
        
    _CLI_CMD("osd.region.set 0 big_endian 0");  // region 0: little_endian (0)
    _CLI_CMD("osd.region.set 1 big_endian 0");  // region 1: little_endian (0) 

    _CLI_CMD("osd.region.set 0 mix_sel 1"); // region 0: pixel mode (1)
    _CLI_CMD("osd.region.set 1 mix_sel 1"); // region 1: pixel mode (1)

    Printf("OSD init ... done\n");
#endif // #if 0

    //==========================================
    //init gfx
    //==========================================
    Printf("GFX Emu init ...\n");
    _CLI_CMD("gfx.init");
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.set.alpha 255");
    _CLI_CMD("gfxemu.color 0xffffffff");
    _CLI_CMD("gfx.flush off");
    Printf("GFX Emu init ... done\n");

    //==========================================
    //init gfx emu cases - kodak pictures
    //==========================================
    Printf("GFX emu cases init ...\n");
    Printf("GFX emu cases init ... kodak pictures loaded\n");

    //==========================================
    //init gfx emu cases - yuv source (7 frames)
    //==========================================
    Printf("GFX emu cases init ... yuv source loaded\n");

    //==========================================
    //init gfx test cases - alpha map source (1 frame)
    // 720x480x8 bpp = 345600 bytes = 0x54600
    //==========================================
    Printf("GFX emu cases init ... alpha map source loaded\n");
    Printf("GFX emulation system ready.\n");

    //==========================================
    //init gfx
    //==========================================
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
 
    srand(1827364);

    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmd5391
 *  Fast Test 5391 All Functions
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmd5391(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Num = 10;
    INT32 i4ModeCount = 5;  // argb8888/4444/1555, rgb565, rgb8
    INT32 i4Mode = 0;
    //INT32 i4ArStart = 239;    // Ar: global alpha
   // INT32 i4ArEnd = 241;
    INT32 i4RetValue = 0;

    if (i4Argc >= 2)
    {
        i4Num = (INT32) StrToInt(szArgv[1]);
        Printf("Args: i4Num = %d\n", i4Num);
    }

    // Init
    _CLI_CMD("mid.r");
    _CLI_CMD("nvgt.log 0");
    _CLI_CMD("gfxemu.i");
    _PAUSE(D_PAUSE_DELAY);

    // Only run hw for crash test
    
    i4RetValue = _CLI_CMD("gfxemu.5391crash1 %d", i4Num);
    if (i4RetValue == -1)   return -1;

    // Only run hw for crash test
    i4RetValue = _CLI_CMD("gfxemu.5391crash2 %d", i4Num);
    if (i4RetValue == -1)   return -1;
    
    
    // Special Operations
    // 2D Memory Compare (ROP Bitblt with OP_CODE = XOR and NO_WR = 1)
    i4RetValue = _CLI_CMD("gfxemu.2dmemcomp1 %d", i4Num);
    if (i4RetValue == -1)   return -1;

    // OP = 15 (Index To Direct Color Bitblt)
    i4RetValue = _CLI_CMD("gfxemu.idx2dir2 %d", i4Num);
    if (i4RetValue == -1)   return -1;
    
    // argb8888 test
    for (i4Mode = 0; i4Mode < 1; i4Mode++)
    {
        _CLI_CMD("gfxemu.mode cm %d", i4Mode);
        
        // OP = 2 (Rect Fill)
        i4RetValue = _CLI_CMD("gfxemu.fill3 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.fill7 %d %d", i4Num, 0);
        if (i4RetValue == -1)   return -1;

        // OP = 3 & 4 (H & V Line)
        i4RetValue = _CLI_CMD("gfxemu.line6 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.line7 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        // OP = 6 (BitBlt)
        i4RetValue = _CLI_CMD("gfxemu.bitblt6 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt8 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt10 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt12 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt13 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        
        // OP = 8 (Alpha Blending)
        i4RetValue = _CLI_CMD("gfxemu.blend2 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.blend3 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        // OP = 10 (YCbCr to RGB)
        //  Linear Scan
        i4RetValue = _CLI_CMD("gfxemu.ccnv2 3");
        if (i4RetValue == -1)   return -1;

        //  MB: Swap Mode
        i4RetValue = _CLI_CMD("gfxemu.ccnv3 3");
        if (i4RetValue == -1)   return -1;

        //  MB: Block Mode
        i4RetValue = _CLI_CMD("gfxemu.ccnv4 3");
        if (i4RetValue == -1)   return -1;

        //  MB: MergeTop Mode
        i4RetValue = _CLI_CMD("gfxemu.ccnv5 3");
        if (i4RetValue == -1)   return -1;

        // OP = 11 (Stretch)
        i4RetValue = _CLI_CMD("gfxemu.sb2 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        // OP = 12 (Alpha Map)
        i4RetValue = _CLI_CMD("gfxemu.am3 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.am4 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        // OP = 13 (Alpha Composition Loop Mode, argb8888/4444)
        if ((i4Mode == 0) || (i4Mode == 1))
        {
            i4RetValue = _CLI_CMD("gfxemu.cpl1 255 255 7 11 0");
            if (i4RetValue == -1)   return -1;
            i4RetValue = _CLI_CMD("gfxemu.cpl4 %d", i4Num);
            if (i4RetValue == -1)   return -1;
            i4RetValue = _CLI_CMD("gfxemu.cpl6 %d", i4Num);
            if (i4RetValue == -1)   return -1;
        } // ~if

        // OP = 14 (ROP Bitblt)
        i4RetValue = _CLI_CMD("gfxemu.rop2 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.rop3 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        // OP = 16 (Horizontal Line to Vertical Line)
        i4RetValue = _CLI_CMD("gfxemu.hor2verlin3 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.hor2verlin4 %d", i4Num);
        if (i4RetValue == -1)   return -1;
    }

    // rgb8 test
    for (i4Mode = 4; i4Mode < i4ModeCount; i4Mode++)
    {
        _CLI_CMD("gfxemu.mode cm %d", i4Mode);
        
        // OP = 2 (Rect Fill)
        i4RetValue = _CLI_CMD("gfxemu.fill3 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.fill7 %d %d", i4Num, 0);
        if (i4RetValue == -1)   return -1;

        // OP = 3 & 4 (H & V Line)
        i4RetValue = _CLI_CMD("gfxemu.line6 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.line7 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        // OP = 6 (BitBlt)
        i4RetValue = _CLI_CMD("gfxemu.bitblt6 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt8 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt10 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt12 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt13 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        
        // OP = 12 (Stretch)
        i4RetValue = _CLI_CMD("gfxemu.sb2 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        // OP = 14 (ROP Bitblt)
        i4RetValue = _CLI_CMD("gfxemu.rop2 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.rop3 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        
        // OP = 16 (Horizontal Line to Vertical Line)
        i4RetValue = _CLI_CMD("gfxemu.hor2verlin3 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        // OP = 16 (Horizontal Line to Vertical Line)
        i4RetValue = _CLI_CMD("gfxemu.hor2verlin4 %d", i4Num);
        if (i4RetValue == -1)   return -1;
    }

    _CLI_CMD("gfxemu.mode cm %d", 0);
    Printf("\n");
    Printf("====================\n");
    Printf("5391 GFX Fast Test -> Auto Test OK !\n");
    Printf("====================\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmd536x / mt5387
 *  Fast Test 536x All Functions
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmd536x(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Num = 10;
    INT32 i4ModeCount = 5;  // argb8888/4444/1555, rgb565, rgb8
    INT32 i4Mode = 0;
    //INT32 i4ArStart = 239;    // Ar: global alpha
    //INT32 i4ArEnd = 241;
    INT32 i4RetValue = 0;

    if (i4Argc >= 2)
    {
        i4Num = (INT32) StrToInt(szArgv[1]);
        Printf("Args: i4Num = %d\n", i4Num);
    }

    // Init
    _CLI_CMD("mid.r");
    _CLI_CMD("nvgt.log 0");
    _CLI_CMD("gfxemu.i");
    _PAUSE(D_PAUSE_DELAY);

    
    // Only run hw for crash test
    i4RetValue = _CLI_CMD("gfxemu.536xcrash1 %d", i4Num);
    if (i4RetValue == -1)   return -1;

    // Only run hw for crash test
    i4RetValue = _CLI_CMD("gfxemu.536xcrash2 %d", i4Num);
    if (i4RetValue == -1)   return -1;

    // Special Operations
    // 2D Memory Compare (ROP Bitblt with OP_CODE = XOR and NO_WR = 1)
    //i4RetValue = _CLI_CMD("gfxemu.2dmemcomp1 %d", i4Num);
    //if (i4RetValue == -1)   return -1;

    // OP = 15 (Index To Direct Color Bitblt)
    i4RetValue = _CLI_CMD("gfxemu.idx2dir2 %d", i4Num);
    if (i4RetValue == -1)   return -1;

    
    // argb8888 test
    for (i4Mode = 0; i4Mode < 2; i4Mode++)
    {
        //_CLI_CMD("gfxemu.mode cm 0");
        //i4RetValue = _CLI_CMD("gfxemu.fill3 %d", i4Num);
        
        _CLI_CMD("gfxemu.mode cm %d", i4Mode);
        
        // OP = 2 (Rect Fill)
        i4RetValue = _CLI_CMD("gfxemu.fill3 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.fill7 %d %d", i4Num, 0);
        if (i4RetValue == -1)   return -1;

        
        // OP = 3 & 4 (H & V Line)
        i4RetValue = _CLI_CMD("gfxemu.line6 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.line7 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        
        // OP = 6 (BitBlt)
        i4RetValue = _CLI_CMD("gfxemu.bitblt6 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt8 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt10 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt12 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt13 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        
        // OP = 8 (Alpha Blending)
        i4RetValue = _CLI_CMD("gfxemu.blend2 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.blend3 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        // OP = 10 (YCbCr to RGB)
        //  Linear Scan
        i4RetValue = _CLI_CMD("gfxemu.ccnv2 3");
        if (i4RetValue == -1)   return -1;

        //  MB: Swap Mode
        i4RetValue = _CLI_CMD("gfxemu.ccnv3 3");
        if (i4RetValue == -1)   return -1;

        //  MB: Block Mode
        i4RetValue = _CLI_CMD("gfxemu.ccnv4 3");
        if (i4RetValue == -1)   return -1;

        //  MB: MergeTop Mode
        i4RetValue = _CLI_CMD("gfxemu.ccnv5 3");
        if (i4RetValue == -1)   return -1;

        // OP = 11 (Stretch)
        i4RetValue = _CLI_CMD("gfxemu.sb2 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        // OP = 12 (Alpha Map)
        i4RetValue = _CLI_CMD("gfxemu.am3 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.am4 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        // OP = 13 (Alpha Composition Loop Mode, argb8888/4444)
        if ((i4Mode == 0) || (i4Mode == 1))
        {
            i4RetValue = _CLI_CMD("gfxemu.cpl1 255 255 7 11 0");
            if (i4RetValue == -1)   return -1;
            i4RetValue = _CLI_CMD("gfxemu.cpl4 %d", i4Num);
            if (i4RetValue == -1)   return -1;
            i4RetValue = _CLI_CMD("gfxemu.cpl6 %d", i4Num);
            if (i4RetValue == -1)   return -1;
        } // ~if

        // OP = 14 (ROP Bitblt)
        i4RetValue = _CLI_CMD("gfxemu.rop2 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.rop3 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        // OP = 16 (Horizontal Line to Vertical Line)
        i4RetValue = _CLI_CMD("gfxemu.hor2verlin3 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.hor2verlin4 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        
    }

    
    // rgb8 test
    for (i4Mode = 4; i4Mode < i4ModeCount; i4Mode++)
    {
        //_CLI_CMD("gfxemu.mode cm 0");
        //i4RetValue = _CLI_CMD("gfxemu.fill3 %d", i4Num);

        _CLI_CMD("gfxemu.mode cm %d", i4Mode);
        
        // OP = 2 (Rect Fill)
        i4RetValue = _CLI_CMD("gfxemu.fill3 %d 0", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.fill7 %d %d", i4Num, 0);
        if (i4RetValue == -1)   return -1;

        // OP = 3 & 4 (H & V Line)
        i4RetValue = _CLI_CMD("gfxemu.line6 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.line7 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        // OP = 6 (BitBlt)
        
        i4RetValue = _CLI_CMD("gfxemu.bitblt6 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt8 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt10 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt12 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt13 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        
        // OP = 12 (Stretch)
        i4RetValue = _CLI_CMD("gfxemu.sb2 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        // OP = 14 (ROP Bitblt)
        i4RetValue = _CLI_CMD("gfxemu.rop2 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.rop3 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        
        // OP = 16 (Horizontal Line to Vertical Line)
        i4RetValue = _CLI_CMD("gfxemu.hor2verlin3 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        // OP = 16 (Horizontal Line to Vertical Line)
        i4RetValue = _CLI_CMD("gfxemu.hor2verlin4 %d", i4Num);
        if (i4RetValue == -1)   return -1;
    }
    
    _CLI_CMD("gfxemu.mode cm %d", 0);
    Printf("\n");
    Printf("====================\n");
    Printf("536x GFX Fast Test -> Auto Test OK !\n");
    Printf("====================\n");
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdAuto
 *  Auto Test All Functions
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdAuto(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Num = 2000;
    INT32 i4ModeCount = 5;  // argb8888/4444/1555, rgb565, rgb8
    INT32 i4Mode = 0;
    INT32 i4ArStart = 0;    // Ar: global alpha
    INT32 i4ArEnd = 256;
    INT32 i4RetValue = 0;
    
    if (i4Argc < 4)
    {
        Printf("Args: i4Num, i4ArStart, i4ArEnd\n");
        return -1;
    }

    i4Num = (INT32) StrToInt(szArgv[1]);
    i4ArStart = (INT32) StrToInt(szArgv[2]);
    i4ArEnd = (INT32) StrToInt(szArgv[3]);

    // Init
    _CLI_CMD("mid.r");
    _CLI_CMD("nvgt.log 0");
    _CLI_CMD("gfxemu.i");
    _PAUSE(D_PAUSE_DELAY);

    // Only run hw for crash test
    i4RetValue = _CLI_CMD("gfxemu.536xcrash1 %d", i4Num);
    if (i4RetValue == -1)   return -1;

    // Special Operations
    // 2D Memory Compare (ROP Bitblt with OP_CODE = XOR and NO_WR = 1)

    //i4RetValue = _CLI_CMD("gfxemu.2dmemcomp1 %d", i4Num);
    //if (i4RetValue == -1)   return -1;

    // OP = 15 (Index To Direct Color Bitblt)
    i4RetValue = _CLI_CMD("gfxemu.idx2dir2 %d", i4Num);
    if (i4RetValue == -1)   return -1;

    // argb8888/4444/1555 test
    for (i4Mode = 0; i4Mode < 3; i4Mode++)
    {
        //_CLI_CMD("gfxemu.mode cm 0");
        //i4RetValue = _CLI_CMD("gfxemu.fill3 %d", i4Num);
        //if (i4RetValue == -1)   return -1;

        _CLI_CMD("gfxemu.mode cm %d", i4Mode);
        
        // OP = 2 (Rect Fill)
        i4RetValue = _CLI_CMD("gfxemu.fill1");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.fill2");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.fill3 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.fill7 %d %d", i4Num, 0);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.red");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.green");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.blue");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.yellow");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.purple");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.cyan");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.black");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.white");
        if (i4RetValue == -1)   return -1;

        // OP = 3 & 4 (H & V Line)
        i4RetValue = _CLI_CMD("gfxemu.line1");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.line2 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.line3");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.line4");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.line5 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.line6 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.line7 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        
        // OP = 5 (Gradient)
        i4RetValue = _CLI_CMD("gfxemu.grad1");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.grad2");
        if (i4RetValue == -1)   return -1;

        // OP = 6 (BitBlt)
        i4RetValue = _CLI_CMD("gfxemu.bitblt1");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt2 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt6 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt8 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt10 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        
        // OP = 8 (Alpha Blending)
        i4RetValue = _CLI_CMD("gfxemu.blend1 %d %d", i4ArStart, i4ArEnd);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.blend2 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.blend3 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        // OP = 9 (Alpha Composition)

        /*
        i4RetValue = _CLI_CMD("gfxemu.compass0 %d %d", i4ArStart, i4ArEnd);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.compass1 %d %d", i4ArStart, i4ArEnd);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.compass2 0 7");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.compass3 %d %d", i4ArStart, i4ArEnd);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.compass45");
        if (i4RetValue == -1)   return -1;
        */
        
        // OP = 10 (YCbCr to RGB)
        //  Linear Scan
        i4RetValue = _CLI_CMD("gfxemu.ccnv2 0");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.ccnv2 1");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.ccnv2 2");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.ccnv2 3");
        if (i4RetValue == -1)   return -1;

        //  MB: Swap Mode
        i4RetValue = _CLI_CMD("gfxemu.ccnv3 0");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.ccnv3 1");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.ccnv3 2");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.ccnv3 3");
        if (i4RetValue == -1)   return -1;

        //  MB: Block Mode
        i4RetValue = _CLI_CMD("gfxemu.ccnv4 0");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.ccnv4 1");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.ccnv4 2");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.ccnv4 3");
        if (i4RetValue == -1)   return -1;

        //  MB: MergeTop Mode
        i4RetValue = _CLI_CMD("gfxemu.ccnv5 0");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.ccnv5 1");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.ccnv5 2");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.ccnv5 3");
        if (i4RetValue == -1)   return -1;

        // OP = 11 (Stretch)
        i4RetValue = _CLI_CMD("gfxemu.sb1");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.sb2");
        if (i4RetValue == -1)   return -1;

        // OP = 12 (Alpha Map)

        //_CLI_CMD("gfxemu.mode cm 0");
        //i4RetValue = _CLI_CMD("gfxemu.fill3 %d", i4Num);
        //if (i4RetValue == -1)   return -1;
        
        _CLI_CMD("gfxemu.mode cm %d", i4Mode);
        
        i4RetValue = _CLI_CMD("gfxemu.am1");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.am2");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.am3 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.am4 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        // OP = 13 (Alpha Composition Loop Mode, argb8888/4444)
        if ((i4Mode == 0) || (i4Mode == 1))
        {
            i4RetValue = _CLI_CMD("gfxemu.cpl1 255 255 7 7 0"); 
            if (i4RetValue == -1)   return -1;
            i4RetValue = _CLI_CMD("gfxemu.cpl4 %d", i4Num);
            if (i4RetValue == -1)   return -1;
            i4RetValue = _CLI_CMD("gfxemu.cpl6 %d", i4Num);
            if (i4RetValue == -1)   return -1;
        } // ~if

        // OP = 14 (ROP Bitblt)
        i4RetValue = _CLI_CMD("gfxemu.rop1 4 15");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.rop2 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.rop3 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        // OP = 16 (Horizontal Line to Vertical Line)
        i4RetValue = _CLI_CMD("gfxemu.hor2verlin3 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.hor2verlin4 %d", i4Num);
        if (i4RetValue == -1)   return -1;
    }

    // rgb565 and rgb8 test
    for (i4Mode = 3; i4Mode < i4ModeCount; i4Mode++)
    {
        //_CLI_CMD("gfxemu.mode cm 0");
        //i4RetValue = _CLI_CMD("gfxemu.fill3 %d", i4Num);

        _CLI_CMD("gfxemu.mode cm %d", i4Mode);
        
        // OP = 2 (Rect Fill)
        i4RetValue = _CLI_CMD("gfxemu.fill1");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.fill2");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.fill3 %d 0", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.fill4 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.fill7 %d %d", i4Num, 0);
        if (i4RetValue == -1)   return -1;

        i4RetValue = _CLI_CMD("gfxemu.red");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.green");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.blue");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.yellow");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.purple");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.cyan");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.black");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.white");
        if (i4RetValue == -1)   return -1;

        // OP = 3 & 4 (H & V Line)
        i4RetValue = _CLI_CMD("gfxemu.line1");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.line2 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.line3");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.line4");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.line5 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.line6 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.line7 %d", i4Num);
        if (i4RetValue == -1)   return -1;

        // OP = 5 (Gradient)
        i4RetValue = _CLI_CMD("gfxemu.grad1");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.grad2");
        if (i4RetValue == -1)   return -1;

        // OP = 6 (BitBlt)
        i4RetValue = _CLI_CMD("gfxemu.bitblt1");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt2 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt6 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt8 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.bitblt10 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        
        // OP = 12 (Stretch)
        i4RetValue = _CLI_CMD("gfxemu.sb1");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.sb2");
        if (i4RetValue == -1)   return -1;

        // OP = 14 (ROP Bitblt)
        //_CLI_CMD("gfxemu.mode cm 0");
        //i4RetValue = _CLI_CMD("gfxemu.fill3 %d", i4Num);
        //_CLI_CMD("gfxemu.mode cm %d", i4Mode);
        
        i4RetValue = _CLI_CMD("gfxemu.rop1 4 15");
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.rop2 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        i4RetValue = _CLI_CMD("gfxemu.rop3 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        
        // OP = 16 (Horizontal Line to Vertical Line)
        i4RetValue = _CLI_CMD("gfxemu.hor2verlin3 %d", i4Num);
        if (i4RetValue == -1)   return -1;
        // OP = 16 (Horizontal Line to Vertical Line)
        i4RetValue = _CLI_CMD("gfxemu.hor2verlin4 %d", i4Num);
        if (i4RetValue == -1)   return -1;
    }

    _CLI_CMD("gfxemu.mode cm %d", 0);
    Printf("\n");
    Printf("====================\n");
    Printf(" Auto Test OK !\n");
    Printf("====================\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdEnDCache
 *  Enable D-Cache
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdEnDCache(INT32 i4Argc, const CHAR ** szArgv)
{
    // enable ARM926 D-cache
    if (HalIsDCacheEnabled() == FALSE)
    {
        HalFlushInvalidateDCache();
        HalEnableDCache();
        Printf("=========================\n");
        Printf("Enable ARM926 D-Cache\n");
        Printf("=========================\n");
    }

    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdDiDCache
 *  Disable D-Cache
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdDiDCache(INT32 i4Argc, const CHAR ** szArgv)
{
    // disable ARM926 D-cache
    if (HalIsDCacheEnabled() == TRUE)
    {
        HalFlushInvalidateDCache();
        HalDisableDCache();
        Printf("=========================\n");
        Printf("Disable ARM926 D-Cache\n");
        Printf("=========================\n");
    }

    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdFillRed
 *  Rectangle Fill Test (Red)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdFillRed(INT32 i4Argc, const CHAR ** szArgv)
{
    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
    
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.color 0xFFFF0000");
    _CLI_CMD("gfx.op.rect 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.flush");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdFillGreen
 *  Rectangle Fill Test (Green)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdFillGreen(INT32 i4Argc, const CHAR ** szArgv)
{
    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
    
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.color 0xFF00FF00");
    _CLI_CMD("gfx.op.rect 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.flush");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdFillBlue
 *  Rectangle Fill Test (Blue)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdFillBlue(INT32 i4Argc, const CHAR ** szArgv)
{
    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
    
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.color 0xFF0000FF");
    _CLI_CMD("gfx.op.rect 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.flush");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdFillYellow
 *  Rectangle Fill Test (Yellow)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdFillYellow(INT32 i4Argc, const CHAR ** szArgv)
{
    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
    
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.color 0xFFFFFF00");
    _CLI_CMD("gfx.op.rect 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.flush");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdFillPurple
 *  Rectangle Fill Test (Purple)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdFillPurple(INT32 i4Argc, const CHAR ** szArgv)
{
    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
    
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.color 0xFFFF00FF");
    _CLI_CMD("gfx.op.rect 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.flush");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdFillCyan
 *  Rectangle Fill Test (Cyan)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdFillCyan(INT32 i4Argc, const CHAR ** szArgv)
{
    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
    
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.color 0xFF00FFFF");
    _CLI_CMD("gfx.op.rect 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.flush");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdFillBlack
 *  Rectangle Fill Test (Black)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdFillBlack(INT32 i4Argc, const CHAR ** szArgv)
{
    _CLI_CMD("gfxemu.color 0xFF000000");
    _CLI_CMD("gfx.op.rect 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.flush");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdFillWhite
 *  Rectangle Fill Test (White)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdFillWhite(INT32 i4Argc, const CHAR ** szArgv)
{
    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
    
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.color 0xFFFFFFFF");
    _CLI_CMD("gfx.op.rect 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.flush");
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmd5391CrashTest1
 *  5391 Crash Test 01
 *  only run hw to do crash test
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmd5387CrashTest1(INT32 i4Argc, const CHAR ** szArgv)
{
     INT32 i, i4Run, i4Loops=0, i4RetValue=0, i4Index;
    INT32 i4TotalOps = 5;

    if (i4Argc < 2)
    {
        Printf("Args: i4Run\n");
        return -1;
    }

    i4Run = (INT32) StrToInt(szArgv[1]);

    Printf("5391 HW Crash Test 02, %d times \n", i4Run);

    for (i = 0; i < i4Run; i++)
    {
        i4Index = (((INT32)rand()) % i4TotalOps) +1;
        i4Loops = ((INT32)rand()) % i4TotalOps;
        ++i4Loops;

        switch (i4Index)
        {
        case 1:
            i4RetValue = _CLI_CMD("gfxemu.fill7 %d 1", i4Loops);
            break;
                   
        case 2:
            i4RetValue = _CLI_CMD("gfxemu.cpl6 %d", i4Loops);
            break;

        case 3:
            i4RetValue = _CLI_CMD("gfxemu.onepasscomp 87 88 99");
            break;
            
        case 4:
            i4RetValue = _CLI_CMD("gfxemu.gencompose 9 10 44");
            break;
            
        case 5:
            i4RetValue = _CLI_CMD("gfxemu.compress2 2 3 99");
            break;
            
        default:
            break;
        } // ~switch

        if (i4RetValue == -1)
        {
            return -1;
        }
    } // ~for

    Printf("5387 HW Crash Test 01, %d times ..... done \n", i4Loops);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmd5391CrashTest1
 *  5391 Crash Test 01
 *  only run hw to do crash test
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmd5391CrashTest1(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Loops, i4RetValue;

    if (i4Argc < 2)
    {
        Printf("Args: i4Loops\n");
        return -1;
    }

    i4Loops = (INT32) StrToInt(szArgv[1]);

    Printf("5391 HW Crash Test 01, %d times \n", i4Loops);
    
    i4RetValue = _CLI_CMD("gfxemu.fill7 %d 1", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.line7 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.bitblt10 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.blend3 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.alphamap4 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.cpl6 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.rop3 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.2dmemcomp2 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.idx2dir4 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.hor2verlin4 %d", i4Loops);
    //if (i4RetValue == -1)   return -1;
    //i4RetValue = _CLI_CMD("gfxemu.compress1 0x058ed800 2880 0x05b90800 720 480 0");
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.bitblt13 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.subpic1 640 468 14");
    if (i4RetValue == -1)   return -1;

    Printf("5391 HW Crash Test 01, %d times ..... done \n", i4Loops);
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdCrash5391Test2
 *  5391 Crash Test 02
 *  only run hw to do crash test (random)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmd5391CrashTest2(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i, i4Run, i4Loops=0, i4RetValue=0, i4Index=0;
    INT32 i4TotalOps = 15;

    if (i4Argc < 2)
    {
        Printf("Args: i4Run\n");
        return -1;
    }

    i4Run = (INT32) StrToInt(szArgv[1]);

    Printf("5391 HW Crash Test 02, %d times \n", i4Run);

    for (i = 0; i < i4Run; i++)
    {
        i4Index = ((INT32)rand()) % i4TotalOps+1;
        i4Loops = ((INT32)rand()) % i4TotalOps;
        ++i4Loops;

        switch (i4Index)
        {
        case 1:
            i4RetValue = _CLI_CMD("gfxemu.fill7 %d 1", i4Loops);
            break;
            
        case 2:
            i4RetValue = _CLI_CMD("gfxemu.line7 %d", i4Loops);
            break;
            
        case 3:
            i4RetValue = _CLI_CMD("gfxemu.bitblt10 %d", i4Loops);
            break;
            
        case 4:
            i4RetValue = _CLI_CMD("gfxemu.blend3 %d", i4Loops);
            break;
            
        case 5:
            i4RetValue = _CLI_CMD("gfxemu.alphamap4 %d", i4Loops);
            break;
            
        case 6:
            i4RetValue = _CLI_CMD("gfxemu.cpl6 %d", i4Loops);
            break;
            
        case 7:
            i4RetValue = _CLI_CMD("gfxemu.rop3 %d", i4Loops);
            break;
            
        case 8:
            i4RetValue = _CLI_CMD("gfxemu.2dmemcomp2 %d", i4Loops);
            break;

        case 9:
            i4RetValue = _CLI_CMD("gfxemu.idx2dir4 %d", i4Loops);
            break;
            
        case 10:
            i4RetValue = _CLI_CMD("gfxemu.hor2verlin4 %d", i4Loops);
            break;
            
        //case 11:
        //    i4RetValue = _CLI_CMD("gfxemu.compress1 0x058ed800 2880 0x05b90800 720 480 0");
        //    break;

        case 12:
            i4RetValue = _CLI_CMD("gfxemu.bitblt13 %d", i4Loops);
            break;

        case 13:
            i4RetValue = _CLI_CMD("gfxemu.subpic1 640 468 14");
            break;
            
        default:
            break;
        } // ~switch

        if (i4RetValue == -1)
        {
            return -1;
        }
    } // ~for

    Printf("5391 HW Crash Test 02, %d times ...... done \n", i4Run);
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmd5391CrashTest3
 *  5391 Crash Test 03
 *  only run hw to check whether crash recovery can work or not (random)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmd5391CrashTest3(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i, i4Loops;

    if (i4Argc < 2)
    {
        Printf("Args: i4Loops\n");
        return -1;
    }

    i4Loops = (INT32) StrToInt(szArgv[1]);

    Printf("5391 HW Crash Test 03, %d times \n", i4Loops);

    for (i = 0; i < i4Loops; i++)
    {
        GFX_Lock();
        GFX_SetSrc((UINT8*)BUF_SCREEN, CM_Reserved0, 16);
        GFX_SetDst((UINT8*)BUF_SCREEN, CM_Reserved0, 0);
        GFX_SetColor(0x12344321);
        GFX_SetRopOpt(0xFF);
        GFX_RopBitBlt(0, 0, 0, 0, 100, 200);
        GFX_SetSrc((UINT8*)BUF_SCREEN, CM_Reserved0, 16);
        GFX_SetDst((UINT8*)BUF_SCREEN, CM_ARGB8888_DIRECT32, 16);
        GFX_SetBltOpt(0xFFFFFFFB, 0x0, 0xFFFFFFFF);
        GFX_BitBlt(0, 0, 0, 0, 100, 200);
        GFX_SetSrc((UINT8*)BUF_SCREEN, CM_Reserved3, 0);
        GFX_SetDst((UINT8*)BUF_SCREEN, CM_Reserved3, 16);
        GFX_SetHoriToVertLineOpt(1);
        GFX_HoriToVertLine(0, 0, 0, 0, 1);
        GFX_StretchBlt(0, 0, 1, 1, 0, 0, 10000, 10000);
        GFX_StretchBlt(0, 0, 1, 1, 0, 0, 10000, 10000);
        GFX_StretchBlt(0, 0, 0, 1, 0, 0, 10000, 10000);
        GFX_StretchBlt(0, 0, 0, 0, 0, 0, 10000, 10000);
        GFX_StretchBlt(0, 0, 1, 1, 0, 0, 10000, 10000);
        GFX_StretchBlt(0, 0, 1, 0, 0, 0, 10000, 10000);
        GFX_StretchBlt(0, 0, 1000, 1000, 0, 0, 1, 1);
        GFX_Flush();
        GFX_Reset();
        GFX_Wait();
        GFX_Unlock();
    } // ~for

    Printf("5391 HW Crash Test 03, %d times ...... done \n", i4Loops);
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmd536xCrashTest1
 *  536x Crash Test 01
 *  only run hw to do crash test
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmd536xCrashTest1(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Loops, i4RetValue;

    if (i4Argc < 2)
    {
        Printf("Args: i4Loops\n");
        return -1;
    }

    i4Loops = (INT32) StrToInt(szArgv[1]);

    Printf("536x HW Crash Test 01, %d times \n", i4Loops);
    
    i4RetValue = _CLI_CMD("gfxemu.fill7 %d 1", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.line7 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.bitblt10 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.blend3 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.alphamap4 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.cpl6 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.rop3 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.2dmemcomp2 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.idx2dir4 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.hor2verlin4 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    i4RetValue = _CLI_CMD("gfxemu.bitblt13 %d", i4Loops);
    if (i4RetValue == -1)   return -1;
    //i4RetValue = _CLI_CMD("gfxemu.subpic1 640 468 14");
    //if (i4RetValue == -1)   return -1;

    Printf("536x HW Crash Test 01, %d times ..... done \n", i4Loops);
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdCrash536xTest2
 *  536x Crash Test 02
 *  only run hw to do crash test (random)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmd536xCrashTest2(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i, i4Run, i4Loops=0, i4RetValue=0, i4Index=0;
    INT32 i4TotalOps = 15;

    if (i4Argc < 2)
    {
        Printf("Args: i4Run\n");
        return -1;
    }

    i4Run = (INT32) StrToInt(szArgv[1]);

    Printf("536x HW Crash Test 02, %d times \n", i4Run);

    for (i = 0; i < i4Run; i++)
    {
        i4Index = ((INT32)rand()) % i4TotalOps+1;
        i4Loops = ((INT32)rand()) % i4TotalOps;
        ++i4Loops;

        switch (i4Index)
        {
        case 1:
            i4RetValue = _CLI_CMD("gfxemu.fill7 %d 1", i4Loops);
            break;
            
        case 2:
            i4RetValue = _CLI_CMD("gfxemu.line7 %d", i4Loops);
            break;
            
        case 3:
            i4RetValue = _CLI_CMD("gfxemu.bitblt10 %d", i4Loops);
            break;
            
        case 4:
            i4RetValue = _CLI_CMD("gfxemu.blend3 %d", i4Loops);
            break;
            
        case 5:
            i4RetValue = _CLI_CMD("gfxemu.alphamap4 %d", i4Loops);
            break;
            
        case 6:
            i4RetValue = _CLI_CMD("gfxemu.cpl6 %d", i4Loops);
            break;
            
        case 7:
            i4RetValue = _CLI_CMD("gfxemu.rop3 %d", i4Loops);
            break;
            
        case 8:
            i4RetValue = _CLI_CMD("gfxemu.2dmemcomp2 %d", i4Loops);
            break;

        case 9:
            i4RetValue = _CLI_CMD("gfxemu.idx2dir4 %d", i4Loops);
            break;
            
        case 10:
            i4RetValue = _CLI_CMD("gfxemu.hor2verlin4 %d", i4Loops);
            break;

        case 12:
            i4RetValue = _CLI_CMD("gfxemu.bitblt13 %d", i4Loops);
            break;

        //case 13:
        //    i4RetValue = _CLI_CMD("gfxemu.subpic1 640 468 14");
        //    break;
            
        default:
            break;
        } // ~switch

        if (i4RetValue == -1)
        {
            return -1;
        }
    } // ~for

    Printf("536x HW Crash Test 02, %d times ...... done \n", i4Run);
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdFill1
 *  Rect Fill Test 01
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdFill1(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;

    Printf("Rect Fill Test 01 ");

    x_memset((UINT32*)BUF_SCREEN,0,_u4ScreenWidth * _u4ScreenHeight*4);
    x_memset((UINT32*)COMPARE_BUF,0,_u4ScreenWidth * _u4ScreenHeight*4);

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    _CLI_CMD("gfxemu.color 0xffff0000");
    _CLI_CMD("gfx.op.rect 0 0 200 480");
    _CLI_CMD("gfxemu.color 0xff00ff00");
    _CLI_CMD("gfx.op.rect 200 0 200 480");
    _CLI_CMD("gfxemu.color 0xff0000ff");
    _CLI_CMD("gfx.op.rect 400 0 320 480");
    _CLI_CMD("gfxemu.color 0xffffffff");
    _CLI_CMD("gfx.op.rect 0 0 720 240");
    _CLI_CMD("gfx.flush");


    // SW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");

    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    _CLI_CMD("gfxemu.color 0xffff0000");
    _CLI_CMD("gfx.op.rect 0 0 200 480");
    _CLI_CMD("gfxemu.color 0xff00ff00");
    _CLI_CMD("gfx.op.rect 200 0 200 480");
    _CLI_CMD("gfxemu.color 0xff0000ff");
    _CLI_CMD("gfx.op.rect 400 0 320 480");
    _CLI_CMD("gfxemu.color 0xffffffff");
    _CLI_CMD("gfx.op.rect 0 0 720 240");
    _CLI_CMD("gfx.flush");

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    
    return _GfxBufferCompare(
            (UINT32 *)COMPARE_BUF, 
            (UINT32 *)(BUF_SCREEN), u4Size);

    // HW
//    _CLI_CMD("gfx.reset");
//    _CLI_CMD("gfxemu.mode");
//    return _GfxHwBufferCompare((UINT8 *)COMPARE_BUF, (UINT8 *)BUF_SCREEN, u4Size);
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdFill2
 *  Rect Fill Test 02
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdFill2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;

    Printf("Rect Fill Test 02 ");

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");
    
    _CLI_CMD("gfxemu.color 0xFFF0F8FF");    //aliceblue
    _CLI_CMD("gfx.op.rect  20  20 140 120");
    _CLI_CMD("gfxemu.color 0xFFFAEBD7");    //antiquewhite
    _CLI_CMD("gfx.op.rect 200  20 140 120");
    _CLI_CMD("gfxemu.color 0xFF00FFFF");    //aqua
    _CLI_CMD("gfx.op.rect 380  20 140 120");
    _CLI_CMD("gfxemu.color 0xFF7FFFD4");    //aquamarine
    _CLI_CMD("gfx.op.rect 560  20 140 120");
    _CLI_CMD("gfxemu.color 0xFFF0FFFF");    //azure
    _CLI_CMD("gfx.op.rect  20 180 140 120");
    _CLI_CMD("gfxemu.color 0xFFF5F5DC");    //beige
    _CLI_CMD("gfx.op.rect 200 180 140 120");
    _CLI_CMD("gfxemu.color 0xFFFFE4C4");    //bisque
    _CLI_CMD("gfx.op.rect 380 180 140 120");
    _CLI_CMD("gfxemu.color 0xFFFFEBCD");    //blanchedalmond
    _CLI_CMD("gfx.op.rect 560 180 140 120");
    _CLI_CMD("gfxemu.color 0xFF0000FF");    //blue
    _CLI_CMD("gfx.op.rect  20 340 140 120");
    _CLI_CMD("gfxemu.color 0xFF8A2BE2");    //blueviolet
    _CLI_CMD("gfx.op.rect 200 340 140 120");
    _CLI_CMD("gfxemu.color 0xFFA52A2A");    //brown
    _CLI_CMD("gfx.op.rect 380 340 140 120");
    _CLI_CMD("gfxemu.color 0xFFDEB887");    //burlywood
    _CLI_CMD("gfx.op.rect 560 340 140 120");
    _CLI_CMD("gfxemu.color 0xFFFFFFFF");
    _CLI_CMD("gfx.op.rect 0   0 720 2");
    _CLI_CMD("gfx.op.rect 0 159 720 3");
    _CLI_CMD("gfx.op.rect 0 319 720 3");
    _CLI_CMD("gfx.op.rect 0 478 720 2");
    _CLI_CMD("gfx.op.rect   0 0 2 480");
    _CLI_CMD("gfx.op.rect 179 0 3 480");
    _CLI_CMD("gfx.op.rect 359 0 3 480");
    _CLI_CMD("gfx.op.rect 539 0 3 480");
    _CLI_CMD("gfx.op.rect 718 0 2 480");
    _CLI_CMD("gfx.flush");


    // SW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");

    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    _CLI_CMD("gfxemu.color 0xFFF0F8FF");    //aliceblue
    _CLI_CMD("gfx.op.rect  20  20 140 120");
    _CLI_CMD("gfxemu.color 0xFFFAEBD7");    //antiquewhite
    _CLI_CMD("gfx.op.rect 200  20 140 120");
    _CLI_CMD("gfxemu.color 0xFF00FFFF");    //aqua
    _CLI_CMD("gfx.op.rect 380  20 140 120");
    _CLI_CMD("gfxemu.color 0xFF7FFFD4");    //aquamarine
    _CLI_CMD("gfx.op.rect 560  20 140 120");
    _CLI_CMD("gfxemu.color 0xFFF0FFFF");    //azure
    _CLI_CMD("gfx.op.rect  20 180 140 120");
    _CLI_CMD("gfxemu.color 0xFFF5F5DC");    //beige
    _CLI_CMD("gfx.op.rect 200 180 140 120");
    _CLI_CMD("gfxemu.color 0xFFFFE4C4");    //bisque
    _CLI_CMD("gfx.op.rect 380 180 140 120");
    _CLI_CMD("gfxemu.color 0xFFFFEBCD");    //blanchedalmond
    _CLI_CMD("gfx.op.rect 560 180 140 120");
    _CLI_CMD("gfxemu.color 0xFF0000FF");    //blue
    _CLI_CMD("gfx.op.rect  20 340 140 120");
    _CLI_CMD("gfxemu.color 0xFF8A2BE2");    //blueviolet
    _CLI_CMD("gfx.op.rect 200 340 140 120");
    _CLI_CMD("gfxemu.color 0xFFA52A2A");    //brown
    _CLI_CMD("gfx.op.rect 380 340 140 120");
    _CLI_CMD("gfxemu.color 0xFFDEB887");    //burlywood
    _CLI_CMD("gfx.op.rect 560 340 140 120");
    _CLI_CMD("gfxemu.color 0xFFFFFFFF");
    _CLI_CMD("gfx.op.rect 0   0 720 2");
    _CLI_CMD("gfx.op.rect 0 159 720 3");
    _CLI_CMD("gfx.op.rect 0 319 720 3");
    _CLI_CMD("gfx.op.rect 0 478 720 2");
    _CLI_CMD("gfx.op.rect   0 0 2 480");
    _CLI_CMD("gfx.op.rect 179 0 3 480");
    _CLI_CMD("gfx.op.rect 359 0 3 480");
    _CLI_CMD("gfx.op.rect 539 0 3 480");
    _CLI_CMD("gfx.op.rect 718 0 2 480");
    _CLI_CMD("gfx.flush");
    
    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    return _GfxBufferCompare(
                (UINT32 *)COMPARE_BUF,
                (UINT32 *)(BUF_SCREEN),
                u4Size);
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdFill3
 *  Rect Fill Test 03 (random)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdFill3(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4PreCount = 0;
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count, i4ColIndex;
    INT32 i4CountNum = 5000;
    INT32 i4Error = 0;

    Printf("Rect Fill Test 03 ");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        i4ColIndex = ((UINT32)rand()) % 147;
        i4Left     = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right    = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top      = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom   = ((UINT32)rand()) % _u4ScreenHeight;
        i4Width    = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -i4Height;
            i4Top = i4Bottom;
        }

        i4Width++;
        i4Height++;

        Printf("%d Run : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d, ColIndex = %d\n",
            i4Left, i4Top, i4Width, i4Height, i4ColIndex);
        
        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        x_memset((UINT32*)BUF_SCREEN,0,_u4ScreenWidth * _u4ScreenHeight*4);
        x_memset((UINT32*)COMPARE_BUF,0,_u4ScreenWidth * _u4ScreenHeight*4);
        
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        //_CLI_CMD("gfxemu.color 0x%0");
        //_CLI_CMD("gfx.op.rect %d %d %d %d", 0, 0, _u4ScreenWidth, _u4ScreenHeight);
        
        // test min cmd size = 2
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _CLI_CMD("gfx.flush");
        
        _CLI_CMD("gfx.op.rect %d %d %d %d", i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");
        
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        //_CLI_CMD("gfxemu.color 0x%0");
        //_CLI_CMD("gfx.op.rect %d %d %d %d", 0, 0, _u4ScreenWidth, _u4ScreenHeight);

        // test min cmd size = 2
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _CLI_CMD("gfx.flush");
        
        _CLI_CMD("gfx.op.rect %d %d %d %d", i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Rect Fill Test 03 ");
            Printf("%d Run, Diff = %d, (Error = %d)\n",
                i4Count, (i4Count-i4PreCount), ++i4Error);
            i4PreCount = i4Count;
            return -1;
        }
    }
    Printf("# of Error Run = %d, # of Total Run = %d\n", i4Error, i4CountNum);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdFill4
 *  Rect Fill Test 04 (random, hw reset & sw reset)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdFill4(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 *pu4Addr;
    INT32 i4Left, i4Top;
    INT32 i4Width, i4Height, i4Count, i4ColIndex;
    INT32 i4CountNum = 5000;

    Printf("Rect Fill Test 04 ");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _CLI_CMD("gfxemu.cls");
        
    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _PAUSE(200);    // delay 0.2 sec

        i4ColIndex = ((UINT32)rand()) % 147;

        if ((i4Count % 3) == 0)
        {
            i4Left = 249;
            i4Top = 100;
            i4Width = 8;
            i4Height = 15;
            pu4Addr = (UINT32 *)((BUF_SCREEN + 0x468e0));
        }
        else if ((i4Count % 3) == 1)
        {
            i4Left  = 251;
            i4Top = 260;
            i4Width = 9;
            i4Height = 13;
            pu4Addr = (UINT32 *)((BUF_SCREEN + 0xb70e0));
        }
        else
        {
            i4Left  = 250;
            i4Top = 160;
            i4Width = 10;
            i4Height = 7;
            pu4Addr = (UINT32 *)((BUF_SCREEN + 0x70be0));
        }

        Printf("(%6d) : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d, ColIndex = %d\n",
            i4Left, i4Top, i4Width, i4Height, i4ColIndex);


        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _CLI_CMD("gfx.op.rect %d %d %d %d", i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");

        // check first pixel
        if ((*pu4Addr) == _arNamedColor[i4ColIndex].u4Color)
        {
            Printf("Rect Fill Test 04 Error : ");
            Printf("[0x%08x] = 0x%08x\n", pu4Addr, (*pu4Addr));
            return -1;
        }

        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");
    }
    Printf("# of Total Run = (%6d)\n", i4CountNum);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdFill5
 *  Rect Fill Test 05 (random, only one hw reset)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdFill5(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 *pu4Addr;
    INT32 i4Left, i4Top;
    INT32 i4Width, i4Height, i4Count, i4ColIndex;
    INT32 i4CountNum = 5000;

    Printf("Rect Fill Test 05 ");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _CLI_CMD("gfxemu.cls");
        
    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        _PAUSE(200);    // delay 0.2 sec

        i4ColIndex = ((UINT32)rand()) % 147;

        if ((i4Count % 3) == 0)
        {
            i4Left = 249;
            i4Top = 100;
            i4Width = 8;
            i4Height = 15;
            pu4Addr = (UINT32 *)((BUF_SCREEN + 0x468e0));
        }
        else if ((i4Count % 3) == 1)
        {
            i4Left  = 251;
            i4Top = 260;
            i4Width = 9;
            i4Height = 13;
            pu4Addr = (UINT32 *)((BUF_SCREEN + 0xb70e0));
        }
        else
        {
            i4Left  = 250;
            i4Top = 160;
            i4Width = 10;
            i4Height = 7;
            pu4Addr = (UINT32 *)((BUF_SCREEN + 0x70be0));
        }

        Printf("(%6d) : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d, ColIndex = %d\n",
            i4Left, i4Top, i4Width, i4Height, i4ColIndex);


        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _CLI_CMD("gfx.op.rect %d %d %d %d", i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");

        // check first pixel
        if ((*pu4Addr) == _arNamedColor[i4ColIndex].u4Color)
        {
            Printf("Rect Fill Test 05 Error : ");
            Printf("[0x%08x] = 0x%08x\n", pu4Addr, (*pu4Addr));
            return -1;
        }

    }
    Printf("# of Total Run = (%6d)\n", i4CountNum);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdFill6
 *  Rect Fill Test 06
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdFill6(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4Loops, i4ReqVal, i;

    Printf("Rect Fill Test 06 ");

    if (i4Argc < 3)
    {
        Printf("arg: loops, req_interval\n");
        return -1;
    }

    i4Loops  = (INT32) StrToInt(szArgv[1]);
    i4ReqVal = (INT32) StrToInt(szArgv[2]);

    for (i = 0; i < i4Loops; i++)
    {
        Printf("(%5d) : ", i);
        
        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        GFX_HwSetReqInterval(i4ReqVal);

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        _CLI_CMD("gfxemu.color 0xFFF0F8FF");    //aliceblue
        _CLI_CMD("gfx.op.rect  20  20 140 120");
        _CLI_CMD("gfxemu.color 0xFFFAEBD7");    //antiquewhite
        _CLI_CMD("gfx.op.rect 200  20 140 120");
        _CLI_CMD("gfxemu.color 0xFF00FFFF");    //aqua
        _CLI_CMD("gfx.op.rect 380  20 140 120");
        _CLI_CMD("gfxemu.color 0xFF7FFFD4");    //aquamarine
        _CLI_CMD("gfx.op.rect 560  20 140 120");
        _CLI_CMD("gfxemu.color 0xFFF0FFFF");    //azure
        _CLI_CMD("gfx.op.rect  20 180 140 120");
        _CLI_CMD("gfxemu.color 0xFFF5F5DC");    //beige
        _CLI_CMD("gfx.op.rect 200 180 140 120");
        _CLI_CMD("gfxemu.color 0xFFFFE4C4");    //bisque
        _CLI_CMD("gfx.op.rect 380 180 140 120");
        _CLI_CMD("gfxemu.color 0xFFFFEBCD");    //blanchedalmond
        _CLI_CMD("gfx.op.rect 560 180 140 120");
        _CLI_CMD("gfxemu.color 0xFF0000FF");    //blue
        _CLI_CMD("gfx.op.rect  20 340 140 120");
        _CLI_CMD("gfxemu.color 0xFF8A2BE2");    //blueviolet
        _CLI_CMD("gfx.op.rect 200 340 140 120");
        _CLI_CMD("gfxemu.color 0xFFA52A2A");    //brown
        _CLI_CMD("gfx.op.rect 380 340 140 120");
        _CLI_CMD("gfxemu.color 0xFFDEB887");    //burlywood
        _CLI_CMD("gfx.op.rect 560 340 140 120");
        _CLI_CMD("gfxemu.color 0xFFFFFFFF");
        _CLI_CMD("gfx.op.rect 0   0 720 2");
        _CLI_CMD("gfx.op.rect 0 159 720 3");
        _CLI_CMD("gfx.op.rect 0 319 720 3");
        _CLI_CMD("gfx.op.rect 0 478 720 2");
        _CLI_CMD("gfx.op.rect   0 0 2 480");
        _CLI_CMD("gfx.op.rect 179 0 3 480");
        _CLI_CMD("gfx.op.rect 359 0 3 480");
        _CLI_CMD("gfx.op.rect 539 0 3 480");
        _CLI_CMD("gfx.op.rect 718 0 2 480");
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        _CLI_CMD("gfxemu.color 0xFFF0F8FF");    //aliceblue
        _CLI_CMD("gfx.op.rect  20  20 140 120");
        _CLI_CMD("gfxemu.color 0xFFFAEBD7");    //antiquewhite
        _CLI_CMD("gfx.op.rect 200  20 140 120");
        _CLI_CMD("gfxemu.color 0xFF00FFFF");    //aqua
        _CLI_CMD("gfx.op.rect 380  20 140 120");
        _CLI_CMD("gfxemu.color 0xFF7FFFD4");    //aquamarine
        _CLI_CMD("gfx.op.rect 560  20 140 120");
        _CLI_CMD("gfxemu.color 0xFFF0FFFF");    //azure
        _CLI_CMD("gfx.op.rect  20 180 140 120");
        _CLI_CMD("gfxemu.color 0xFFF5F5DC");    //beige
        _CLI_CMD("gfx.op.rect 200 180 140 120");
        _CLI_CMD("gfxemu.color 0xFFFFE4C4");    //bisque
        _CLI_CMD("gfx.op.rect 380 180 140 120");
        _CLI_CMD("gfxemu.color 0xFFFFEBCD");    //blanchedalmond
        _CLI_CMD("gfx.op.rect 560 180 140 120");
        _CLI_CMD("gfxemu.color 0xFF0000FF");    //blue
        _CLI_CMD("gfx.op.rect  20 340 140 120");
        _CLI_CMD("gfxemu.color 0xFF8A2BE2");    //blueviolet
        _CLI_CMD("gfx.op.rect 200 340 140 120");
        _CLI_CMD("gfxemu.color 0xFFA52A2A");    //brown
        _CLI_CMD("gfx.op.rect 380 340 140 120");
        _CLI_CMD("gfxemu.color 0xFFDEB887");    //burlywood
        _CLI_CMD("gfx.op.rect 560 340 140 120");
        _CLI_CMD("gfxemu.color 0xFFFFFFFF");
        _CLI_CMD("gfx.op.rect 0   0 720 2");
        _CLI_CMD("gfx.op.rect 0 159 720 3");
        _CLI_CMD("gfx.op.rect 0 319 720 3");
        _CLI_CMD("gfx.op.rect 0 478 720 2");
        _CLI_CMD("gfx.op.rect   0 0 2 480");
        _CLI_CMD("gfx.op.rect 179 0 3 480");
        _CLI_CMD("gfx.op.rect 359 0 3 480");
        _CLI_CMD("gfx.op.rect 539 0 3 480");
        _CLI_CMD("gfx.op.rect 718 0 2 480");
        _CLI_CMD("gfx.flush");
        
        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Rect Fill Test 06 ... FAIL !!");
            return -1;
        }
    } // ~for
    Printf("Rect Fill Test 06 ... OK !!\n");
    Printf("# of Total Run = (%6d)\n", i4Loops);
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdFill7
 *  Rect Fill Test 07 (random, only run hw)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdFill7(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Left, i4Right, i4Top, i4Bottom, i4ReqVal;
    INT32 i4Width, i4Height, i4Count, i4ColIndex;
    INT32 i4CountNum = 5000, i4ReqRandOn = 0;

    Printf("Rect Fill Test 07 (only hw) ... ");
    
    if (i4Argc < 3)
    {
        Printf("arg: loops, req_random_on\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    i4ReqRandOn = (INT32) StrToInt(szArgv[2]); 
    if (i4ReqRandOn)
    {
        // set req_interval arbitrarily
        i4ReqVal = ((UINT32)rand()) % 3;
        i4ReqVal++;
        GFX_HwSetReqInterval(i4ReqVal);
        Printf("ReqInterval = %d\n", i4ReqVal);
    }
        
    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        i4ColIndex = ((UINT32)rand()) % 147;
        i4Left     = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right    = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top      = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom   = ((UINT32)rand()) % _u4ScreenHeight;
        i4Width    = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -i4Height;
            i4Top = i4Bottom;
        }

        i4Width++;
        i4Height++;

        Printf("%d Run : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d, ColIndex = %d\n",
            i4Left, i4Top, i4Width, i4Height, i4ColIndex);

        // test min cmd size = 2
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _CLI_CMD("gfx.flush");

        _CLI_CMD("gfx.op.rect %d %d %d %d", i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");
    }
    Printf("Rect Fill Test 07 (only hw) ... OK !!\n");
    Printf("# of Total Run = %d\n", i4CountNum);
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdLine1
 *  Draw H/V Line Test 01
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdLine1(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4X, i4Y, i4ColIdx = 0;

    Printf("Line Test 01 ");
    
    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    i4ColIdx = 0;
    for (i4X = 0; i4X < 720; i4X += 7)
    {
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIdx].u4Color);
        i4ColIdx++; if (i4ColIdx >= 147) i4ColIdx %= 147;
        _CLI_CMD("gfx.op.vline %d 0 240", i4X);
    }
    _CLI_CMD("gfx.flush");

    for (i4X = 0; i4X < 717; i4X += 7)
    {
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIdx].u4Color);
        i4ColIdx++; if (i4ColIdx >= 147) i4ColIdx %= 147;
        _CLI_CMD("gfx.op.vline %d 240 240", i4X + 3);
    }
    _CLI_CMD("gfx.flush");

    for (i4Y = 0; i4Y < 477; i4Y += 7)
    {
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIdx].u4Color);
        i4ColIdx++; if (i4ColIdx >= 147) i4ColIdx %= 147;
        _CLI_CMD("gfx.op.hline 0 %d 360", i4Y);
    }
    _CLI_CMD("gfx.flush");

    for (i4Y = 0; i4Y < 477; i4Y += 7)
    {
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIdx].u4Color);
        i4ColIdx++; if (i4ColIdx >= 147) i4ColIdx %= 147;
        _CLI_CMD("gfx.op.hline 360 %d 360", i4Y + 3);
    }
    _CLI_CMD("gfx.flush");

//==========================================================================


    // SW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");
    
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    i4ColIdx = 0;
    for (i4X = 0; i4X < 720; i4X += 7)
    {
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIdx].u4Color);
        i4ColIdx++; if (i4ColIdx >= 147) i4ColIdx %= 147;
        _CLI_CMD("gfx.op.vline %d 0 240", i4X);
    }
    _CLI_CMD("gfx.flush");

    for (i4X = 0; i4X < 717; i4X += 7)
    {
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIdx].u4Color);
        i4ColIdx++; if (i4ColIdx >= 147) i4ColIdx %= 147;
        _CLI_CMD("gfx.op.vline %d 240 240", i4X + 3);
    }
    _CLI_CMD("gfx.flush");

    for (i4Y = 0; i4Y < 477; i4Y += 7)
    {
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIdx].u4Color);
        i4ColIdx++; if (i4ColIdx >= 147) i4ColIdx %= 147;
        _CLI_CMD("gfx.op.hline 0 %d 360", i4Y);
    }
    _CLI_CMD("gfx.flush");

    for (i4Y = 0; i4Y < 477; i4Y += 7)
    {
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIdx].u4Color);
        i4ColIdx++; if (i4ColIdx >= 147) i4ColIdx %= 147;
        _CLI_CMD("gfx.op.hline 360 %d 360", i4Y + 3);
    }
    _CLI_CMD("gfx.flush");


    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    return _GfxBufferCompare(
                (UINT32 *)(COMPARE_BUF),
                (UINT32 *)(BUF_SCREEN),
                u4Size);
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdLine2
 *  Draw H/V Line Test 02 (Random)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdLine2(INT32 i4Argc, const CHAR ** szArgv)
{

    UINT32 u4Size = 0;
    INT32 i4PreCount = 0;
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count;
    INT32 i4ColIndex;
    INT32 i4CountNum = 5000;
    INT32 i4Error = 0;

    Printf("Line Test 02 ");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        i4ColIndex = ((UINT32)rand()) % 147;
        i4Left = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom = ((UINT32)rand()) % _u4ScreenHeight;
        i4Width = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -i4Height;
            i4Top = i4Bottom;
        }
        i4Width++;
        i4Height++;

        Printf("%d Run : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d, ColIndex = %d\n",
            i4Left, i4Top, i4Width, i4Height, i4ColIndex);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _CLI_CMD("gfx.op.hline %d %d %d", i4Left, i4Top, i4Width);
        _CLI_CMD("gfx.op.hline %d %d %d", i4Left, i4Top + i4Height - 1, i4Width);
        _CLI_CMD("gfx.op.vline %d %d %d", i4Left, i4Top, i4Height);
        _CLI_CMD("gfx.op.vline %d %d %d", i4Left + i4Width - 1, i4Top, i4Height);
        _CLI_CMD("gfx.flush");
        

        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");
        
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _CLI_CMD("gfx.op.hline %d %d %d", i4Left, i4Top, i4Width);
        _CLI_CMD("gfx.op.hline %d %d %d", i4Left, i4Top + i4Height - 1, i4Width);
        _CLI_CMD("gfx.op.vline %d %d %d", i4Left, i4Top, i4Height);
        _CLI_CMD("gfx.op.vline %d %d %d", i4Left + i4Width - 1, i4Top, i4Height);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Line Test 02 ");
            Printf("%d Run, Diff = %d, (Error = %d)\n",
                i4Count, (i4Count-i4PreCount), ++i4Error);
            i4PreCount = i4Count;

            return -1;
        }
    }
    Printf("# of Error Run = %d, # of Total Run = %d\n", i4Error, i4CountNum);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdLine3
 *  Draw H/V Line Test 03
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdLine3(INT32 i4Argc, const CHAR ** szArgv)
{

    UINT32 u4Size = 0;
    INT32 i4X, i4Y, i4ColIdx;

    Printf("Line Test 03 ");
    
    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    i4ColIdx = 0;
    for (i4X = 0, i4Y = 0; i4Y < _u4ScreenHeight; i4Y += 4, i4X += 4)
    {
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIdx].u4Color);
        i4ColIdx++; if (i4ColIdx >= 147) i4ColIdx %= 147;
        _CLI_CMD("gfx.op.hline %d %d %d", i4X, i4Y, _u4ScreenWidth - i4X);
    }
    _CLI_CMD("gfx.flush");

    for (i4X = 719, i4Y = 0; i4Y < _u4ScreenHeight; i4Y += 4, i4X -= 4)
    {
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIdx].u4Color);
        i4ColIdx++; if (i4ColIdx >= 147) i4ColIdx %= 147;
        _CLI_CMD("gfx.op.vline %d %d %d", i4X, i4Y, _u4ScreenHeight - i4Y);
    }
    _CLI_CMD("gfx.flush");


    // SW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");
    
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfxemu.cls");
    
    // Action
    i4ColIdx = 0;
    for (i4X = 0, i4Y = 0; i4Y < _u4ScreenHeight; i4Y += 4, i4X += 4)
    {
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIdx].u4Color);
        i4ColIdx++; if (i4ColIdx >= 147) i4ColIdx %= 147;
        _CLI_CMD("gfx.op.hline %d %d %d", i4X, i4Y, _u4ScreenWidth - i4X);
    }
    _CLI_CMD("gfx.flush");

    for (i4X = 719, i4Y = 0; i4Y < _u4ScreenHeight; i4Y += 4, i4X -= 4)
    {
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIdx].u4Color);
        i4ColIdx++; if (i4ColIdx >= 147) i4ColIdx %= 147;
        _CLI_CMD("gfx.op.vline %d %d %d", i4X, i4Y, _u4ScreenHeight - i4Y);
    }
    _CLI_CMD("gfx.flush");

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    return _GfxBufferCompare(
                (UINT32 *)COMPARE_BUF,
                (UINT32 *)(BUF_SCREEN),
                u4Size);
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdLine4
 *  Draw H/V Line Test 04
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdLine4(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size;
    INT32 i4X, i4ColIdx = 0;

    Printf("Line Test 04 ");
    
    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    i4ColIdx = 0;
    for (i4X = 0; i4X < _u4ScreenWidth; i4X++)
    {
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIdx].u4Color);
        _CLI_CMD("gfx.flush");
        i4ColIdx++; if (i4ColIdx >= 147) i4ColIdx %= 147;
        _CLI_CMD("gfx.op.vline %d 0 %d", i4X, _u4ScreenHeight);
    }
    _CLI_CMD("gfx.flush");


    // SW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");
    
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    i4ColIdx = 0;
    for (i4X = 0; i4X < _u4ScreenWidth; i4X++)
    {
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIdx].u4Color);
        _CLI_CMD("gfx.flush");
        i4ColIdx++; if (i4ColIdx >= 147) i4ColIdx %= 147;
        _CLI_CMD("gfx.op.vline %d 0 %d", i4X, _u4ScreenHeight);
    }
    _CLI_CMD("gfx.flush");

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    return _GfxBufferCompare(
                (UINT32 *)COMPARE_BUF,
                (UINT32 *)(BUF_SCREEN),
                u4Size);
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdLine5
 *  Draw H/V Line Test 05 (Random)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdLine5(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4PreCount = 0;
    UINT32 u4X, u4Y, u4Len, u4ColIdx;
    INT32 i4Count;
    INT32 i4CountNum = 5000;
    INT32 i4Error = 0;

    Printf("Line Test 05 ");

    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        u4X = (UINT32)rand() % _u4ScreenWidth;
        u4Y = (UINT32)rand() % _u4ScreenHeight;
        u4Len = (UINT32)rand() % (_u4ScreenWidth - u4X);
        u4ColIdx = (UINT32)rand() % 147;

        if (u4Len)
        {
            Printf("%d Run : ", i4Count);
            Printf("X = %u, Y = %u, LEN = %u\n", u4X, u4Y, u4Len);

            // HW
#ifdef GFX_HW_RESET
            _CLI_CMD("gfx.reset");
#endif
            _CLI_CMD("gfxemu.mode");

            _GfxSetDefMode(BUF_SCREEN, ADR_DST);
            _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
            _CLI_CMD("gfxemu.cls");

            // Action
            _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[u4ColIdx].u4Color);
            _CLI_CMD("gfx.op.hline %u %u %u", u4X, u4Y, u4Len);
            _CLI_CMD("gfx.flush");
            

            // SW
#ifdef GFX_HW_RESET
            _CLI_CMD("gfx.reset");
#endif
            _CLI_CMD("gfxemu.mode sw");
            
            _GfxSetDefMode(COMPARE_BUF, ADR_DST);
            _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
            _CLI_CMD("gfxemu.cls");

            // Action
            _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[u4ColIdx].u4Color);
            _CLI_CMD("gfx.op.hline %u %u %u", u4X, u4Y, u4Len);
            _CLI_CMD("gfx.flush");

            // Compare HW and SW
            u4Size = _u4ScreenWidth * _u4ScreenHeight;
            if (_GfxBufferCompare(
                    (UINT32 *)COMPARE_BUF, 
                    (UINT32 *)(BUF_SCREEN), u4Size) == -1)
            {
                Printf("Line Test 05 ");
                Printf("%d Run, Diff = %d, (Error = %d)\n",
                    i4Count, (i4Count-i4PreCount), ++i4Error);
                i4PreCount = i4Count;

                return -1;
            }
        }
    }
    Printf("# of Error Run = %d, # of Total Run = %d\n", i4Error, i4CountNum);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdLine6
 *  Draw H/V Line Test 06 (Random)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdLine6(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4PreCount = 0;
    UINT32 u4X, u4Y, u4Len, u4ColIdx;
    INT32 i4Count;
    INT32 i4CountNum = 5000;
    INT32 i4Error = 0;

    Printf("Line Test 06 ");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        u4X = (UINT32)rand() % _u4ScreenWidth;
        u4Y = (UINT32)rand() % _u4ScreenHeight;
        u4Len = (UINT32)rand() % (_u4ScreenHeight - u4Y);
        u4ColIdx = (UINT32)rand() % 147;

        if (u4Len)
        {
            Printf("%d Run : ", i4Count);
            Printf("X = %u, Y = %u, LEN = %u\n", u4X, u4Y, u4Len);

            // HW
#ifdef GFX_HW_RESET
            _CLI_CMD("gfx.reset");
#endif
            _CLI_CMD("gfxemu.mode");

            _GfxSetDefMode(BUF_SCREEN, ADR_DST);
            _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
            _CLI_CMD("gfxemu.cls");

            // Action
            _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[u4ColIdx].u4Color);
            _CLI_CMD("gfx.op.vline %u %u %u", u4X, u4Y, u4Len);
            _CLI_CMD("gfx.flush");


            // SW
#ifdef GFX_HW_RESET
            _CLI_CMD("gfx.reset");
#endif
            _CLI_CMD("gfxemu.mode sw");
            
            _GfxSetDefMode(COMPARE_BUF, ADR_DST);
            _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
            _CLI_CMD("gfxemu.cls");

            // Action
            _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[u4ColIdx].u4Color);
            _CLI_CMD("gfx.op.vline %u %u %u", u4X, u4Y, u4Len);
            _CLI_CMD("gfx.flush");

            // Compare HW and SW
            u4Size = _u4ScreenWidth * _u4ScreenHeight;
            if (_GfxBufferCompare(
                    (UINT32 *)COMPARE_BUF, 
                    (UINT32 *)(BUF_SCREEN), u4Size) == -1)
            {
                Printf("Line Test 06 ");
                Printf("%d Run, Diff = %d, (Error = %d)\n",
                    i4Count, (i4Count-i4PreCount), ++i4Error);
                i4PreCount = i4Count;

                return -1;
            }
        }
    }
    Printf("# of Error Run = %d, # of Total Run = %d\n", i4Error, i4CountNum);
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdLine7
 *  Draw H/V Line Test 07 (random, only run hw)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdLine7(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4X, u4Y, u4Len, u4ColIdx;
    INT32 i4Count;
    INT32 i4CountNum = 5000;

    Printf("Line Test 07 (random, only run hw) ... ");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        u4X = (UINT32)rand() % _u4ScreenWidth;
        u4Y = (UINT32)rand() % _u4ScreenHeight;
        u4Len = (UINT32)rand() % (_u4ScreenWidth - u4X);
        u4ColIdx = (UINT32)rand() % 147;

        if (u4Len)
        {
            Printf("%d Run : ", i4Count);
            Printf("X = %u, Y = %u, LEN = %u\n", u4X, u4Y, u4Len);

            // Action
            _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[u4ColIdx].u4Color);
            _CLI_CMD("gfx.op.vline %u %u %u", u4X, u4Y, u4Len);
            _CLI_CMD("gfx.op.hline %u %u %u", u4X, u4Y, u4Len);
            _CLI_CMD("gfx.flush");
        }
    }
    Printf("Line Test 07 (random, only run hw) ... OK !!");
    Printf("# of Total Run = %d\n", i4CountNum);
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdGrad1
 *  Gradient Test 01
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdGrad1(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;

    Printf("Gradient Test 01 ");

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    _CLI_CMD("gfxemu.color 0xFFFFFF00");
    _CLI_CMD("gfx.set.gradopt 3 3 0 -2 0 7 -7 -3 -4 3");
    _CLI_CMD("gfx.op.grad  20  20 200 200 both");   //from yellow to green and blue
    _CLI_CMD("gfx.op.grad 240  20 200 200 h");      //from yellow to green
    _CLI_CMD("gfx.op.grad  20 240 200 200 v");      //from yellow to purple
    _CLI_CMD("gfx.flush");


    // SW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");
        
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    _CLI_CMD("gfxemu.color 0xFFFFFF00");
    _CLI_CMD("gfx.set.gradopt 3 3 0 -2 0 7 -7 -3 -4 3");
    _CLI_CMD("gfx.op.grad  20  20 200 200 both");   //from yellow to green and blue
    _CLI_CMD("gfx.op.grad 240  20 200 200 h");      //from yellow to green
    _CLI_CMD("gfx.op.grad  20 240 200 200 v");      //from yellow to purple
    _CLI_CMD("gfx.flush");

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    return _GfxBufferCompare(
                (UINT32 *)COMPARE_BUF,
                (UINT32 *)(BUF_SCREEN),
                u4Size);
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdGrad2
 *  Gradient Test 02
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdGrad2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;

    Printf("Gradient Test 02 ");

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    _CLI_CMD("gfxemu.color 0xFFFFFF00");
    _CLI_CMD("gfx.set.gradopt 3 3 -5 2 -7 5 2 -2 5 -3");
    _CLI_CMD("gfx.op.grad  20  20 200 200 both");   //from yellow to green and blue
    _CLI_CMD("gfx.op.grad 240  20 200 200 h");      //from yellow to green
    _CLI_CMD("gfx.op.grad  20 240 200 200 v");      //from yellow to purple
    _CLI_CMD("gfx.flush");


    // SW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");
        
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    _CLI_CMD("gfxemu.color 0xFFFFFF00");
    _CLI_CMD("gfx.set.gradopt 3 3 -5 2 -7 5 2 -2 5 -3");
    _CLI_CMD("gfx.op.grad  20  20 200 200 both");   //from yellow to green and blue
    _CLI_CMD("gfx.op.grad 240  20 200 200 h");      //from yellow to green
    _CLI_CMD("gfx.op.grad  20 240 200 200 v");      //from yellow to purple
    _CLI_CMD("gfx.flush");

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    return _GfxBufferCompare(
                (UINT32 *)COMPARE_BUF,
                (UINT32 *)(BUF_SCREEN),
                u4Size);
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdGrad3
 *  Gradient Test 03
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdGrad3(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;

    Printf("Gradient Test 03 ");

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    _CLI_CMD("gfxemu.color 0xFFFFFF00");
    _CLI_CMD("gfx.set.gradopt 3 3 -1 2 -1 2 -3 -2 3 -2");
    _CLI_CMD("gfx.op.grad  20  20 200 200 both");   //from yellow to green and blue
    _CLI_CMD("gfx.op.grad 240  20 200 200 h");      //from yellow to green
    _CLI_CMD("gfx.op.grad  20 240 200 200 v");      //from yellow to purple
    _CLI_CMD("gfx.flush");


    // SW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");
        
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    _CLI_CMD("gfxemu.color 0xFFFFFF00");
    _CLI_CMD("gfx.set.gradopt 3 3 -1 2 -1 2 -3 -2 3 -2");
    _CLI_CMD("gfx.op.grad  20  20 200 200 both");   //from yellow to green and blue
    _CLI_CMD("gfx.op.grad 240  20 200 200 h");      //from yellow to green
    _CLI_CMD("gfx.op.grad  20 240 200 200 v");      //from yellow to purple
    _CLI_CMD("gfx.flush");

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    return _GfxBufferCompare(
                (UINT32 *)COMPARE_BUF,
                (UINT32 *)(BUF_SCREEN),
                u4Size);
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdBlt1
 *  Normal BitBlt Test 01
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdBlt1(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;

    Printf("Normal BitBlt Test 01 ");

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    _CLI_CMD("gfxemu.color 0xFFFF7F00");
    _CLI_CMD("gfx.set.gradopt 1 2 0 -5 2 -1 0 -3 -2 3");
    _CLI_CMD("gfx.op.grad  20  20 100 100 both");   //from yellow to green and blue
    _CLI_CMD("gfxemu.color 0xFFFFFFFF");
    _CLI_CMD("gfx.op.rect  69  20   2 100");
    _CLI_CMD("gfxemu.color 0xFF000000");
    _CLI_CMD("gfx.op.rect  20  69 100   2");

    _CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.op.bitblt  20  20 140  20 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 260  20 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 380  20 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 500  20 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 620  20 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20  20 140 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 140 140 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 260 140 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 380 140 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 500 140 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 620 140 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20  20 260 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 140 260 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 260 260 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 380 260 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 500 260 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 620 260 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20  20 380 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 140 380 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 260 380 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 380 380 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 500 380 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 620 380 100 100");
    //GFX_SetAlpha(0xC9);
    _CLI_CMD("gfx.flush");


    // SW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");
        
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    _CLI_CMD("gfxemu.color 0xFFFF7F00");
    _CLI_CMD("gfx.set.gradopt 1 2 0 -5 2 -1 0 -3 -2 3");
    _CLI_CMD("gfx.op.grad  20  20 100 100 both");   //from yellow to green and blue
    _CLI_CMD("gfxemu.color 0xFFFFFFFF");
    _CLI_CMD("gfx.op.rect  69  20   2 100");
    _CLI_CMD("gfxemu.color 0xFF000000");
    _CLI_CMD("gfx.op.rect  20  69 100   2");

    _CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.op.bitblt  20  20 140  20 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 260  20 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 380  20 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 500  20 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 620  20 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20  20 140 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 140 140 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 260 140 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 380 140 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 500 140 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 620 140 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20  20 260 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 140 260 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 260 260 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 380 260 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 500 260 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 620 260 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20  20 380 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 140 380 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 260 380 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 380 380 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 500 380 100 100");
    _CLI_CMD("gfx.op.bitblt  20  20 620 380 100 100");
    _CLI_CMD("gfx.flush");

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    return _GfxBufferCompare(
                (UINT32 *)COMPARE_BUF,
                (UINT32 *)(BUF_SCREEN),
                u4Size);
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdBlt2
 *  Normal BitBlt Test 02 (random)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdBlt2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count;
    INT32 i4SrcX=0, i4SrcY=0;
    INT32 i4ColIndex;
    INT32 i4PicSel;
    INT32 i4CountNum = 2000;

    Printf("Normal BitBlt Test 02 ");

    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        i4ColIndex  = ((UINT32)rand()) % 147;
        i4Left      = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right     = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top       = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom    = ((UINT32)rand()) % _u4ScreenHeight;
        i4PicSel    = ((UINT32)rand()) % 3;
        
        i4Width = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -i4Height;
            i4Top = i4Bottom;
        }
        i4Width++;
        i4Height++;

        if (i4Width >= 3 && i4Height >= 3)
        {
            i4Width -= 2;
            i4Height -= 2;
            i4Left++;
            i4Top++;
            
            if ((_u4ScreenWidth - i4Width) != 0)
            {
                i4SrcX = ((UINT32)rand()) % (_u4ScreenWidth - i4Width);
            }
            else
            {
                i4SrcX = 0;
            }

            if ((_u4ScreenHeight - i4Height) != 0)
            {
                i4SrcY = ((UINT32)rand()) % (_u4ScreenHeight - i4Height);
            }
            else
            {
                i4SrcY = 0;
            }
        }

        Printf("%d Run : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d, ColIndex = %d\n",
            i4Left, i4Top, i4Width, i4Height, i4ColIndex);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // HW Action
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _CLI_CMD("gfx.op.vline %d %d %d", i4Left, i4Top, i4Height);
        _CLI_CMD("gfx.op.vline %d %d %d", i4Left + i4Width - 1, i4Top, i4Height);
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                i4SrcX, i4SrcY, i4Left, i4Top, i4Width, i4Height);
        //_CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
        //        0, 0, 0, 0, 720, 480);
        //GFX_SetAlpha(0xB7);
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");
            
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // SW Action
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _CLI_CMD("gfx.op.vline %d %d %d", i4Left, i4Top, i4Height);
        _CLI_CMD("gfx.op.vline %d %d %d", i4Left + i4Width - 1, i4Top, i4Height);
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                i4SrcX, i4SrcY, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Normal BitBlt Test 02 Error\n");
            return -1;
        }
    }
    Printf("Total Run = %d\n", i4CountNum);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdBlt3
 *  Normal BitBlt Test 03
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdBlt3(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;

    // load 3 test bitmap first (@ 0x01151800, 0x012A3000, 0x013F4800)
    INT32 i4Iter;

    Printf("Normal BitBlt Test 03 ");
    
    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // HW Action
    // 1. transparent bitblt kodak16 over kodak14
    for (i4Iter = 0; i4Iter < 256; i4Iter++)
    {
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x transparent",
            0, 0, 0, i4Iter, i4Iter, i4Iter);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
    }

    // 2. transparent kodak17 over kodak16
    for (i4Iter = 0; i4Iter < 256; i4Iter++)
    {
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x transparent",
            0, 0, 0, i4Iter, i4Iter, i4Iter);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
    }

    // 3. transparent kodak14 over kodak17
    for (i4Iter = 0; i4Iter < 256; i4Iter++)
    {
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x transparent",
            0, 0, 0, i4Iter, i4Iter, i4Iter);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
    }


    // SW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");
            
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // SW Action
    // 1. transparent bitblt kodak16 over kodak14
    for (i4Iter = 0; i4Iter < 256; i4Iter++)
    {
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x transparent",
            0, 0, 0, i4Iter, i4Iter, i4Iter);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
    }

    // 2. transparent kodak17 over kodak16
    for (i4Iter = 0; i4Iter < 256; i4Iter++)
    {
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x transparent",
            0, 0, 0, i4Iter, i4Iter, i4Iter);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
    }

    // 3. transparent kodak14 over kodak17
    for (i4Iter = 0; i4Iter < 256; i4Iter++)
    {
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x transparent",
            0, 0, 0, i4Iter, i4Iter, i4Iter);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
    }

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    if (_GfxBufferCompare(
        (UINT32 *)COMPARE_BUF, 
        (UINT32 *)(BUF_SCREEN), u4Size) == -1)
    {
        return -1;
    }

    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdBlt4
 *  Normal BitBlt Test 04
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdBlt4(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size;

    // load 3 test bitmap first (@ 0x01151800, 0x012A3000, 0x013F4800)
    INT32 i4Iter;

    Printf("Normal BitBlt Test 04 ");
    
    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // HW Action
    // 1. transparent keynot bitblt kodak16 over kodak14
    for (i4Iter = 0; i4Iter < 256; i4Iter++)
    {
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x transparent keynot",
            0, 0, 0, i4Iter, i4Iter, i4Iter);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
    }

    // 2. transparent keynot kodak17 over kodak16
    for (i4Iter = 0; i4Iter < 256; i4Iter++)
    {
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x transparent keynot",
            0, 0, 0, i4Iter, i4Iter, i4Iter);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
    }

    // 3. transparent keynot kodak14 over kodak17
    for (i4Iter = 0; i4Iter < 256; i4Iter++)
    {
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x transparent keynot",
            0, 0, 0, i4Iter, i4Iter, i4Iter);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
    }


    // SW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");
            
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // SW Action
    // 1. transparent keynot bitblt kodak16 over kodak14
    for (i4Iter = 0; i4Iter < 256; i4Iter++)
    {
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x transparent keynot",
            0, 0, 0, i4Iter, i4Iter, i4Iter);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
    }

    // 2. transparent keynot kodak17 over kodak16
    for (i4Iter = 0; i4Iter < 256; i4Iter++)
    {
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x transparent keynot",
            0, 0, 0, i4Iter, i4Iter, i4Iter);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
    }

    // 3. transparent keynot kodak14 over kodak17
    for (i4Iter = 0; i4Iter < 256; i4Iter++)
    {
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x transparent keynot",
            0, 0, 0, i4Iter, i4Iter, i4Iter);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
    }

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    if (_GfxBufferCompare(
        (UINT32 *)COMPARE_BUF, 
        (UINT32 *)(BUF_SCREEN), u4Size) == -1)
    {
        return -1;
    }

    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdBlt5
 *  Normal BitBlt Test 05
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdBlt5(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;

    // load 3 test bitmap first (@ 0x01151800, 0x012A3000, 0x013F4800)
    INT32 i4Iter;

    Printf("Normal BitBlt Test 05 ");
    
    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // HW Action
    // 1. colorchange bitblt kodak16 over kodak14
    _CLI_CMD("gfxemu.color 0x00000000");
    for (i4Iter = 0; i4Iter < 256; i4Iter++)
    {
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x colorchange",
            0, 0, 0, i4Iter, i4Iter, i4Iter);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
    }

    // 2. transparent kodak17 over kodak16
    for (i4Iter = 0; i4Iter < 256; i4Iter++)
    {
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x colorchange",
            0, 0, 0, i4Iter, i4Iter, i4Iter);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
    }

    // 3. transparent kodak14 over kodak17
    for (i4Iter = 0; i4Iter < 256; i4Iter++)
    {
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x colorchange",
            0, 0, 0, i4Iter, i4Iter, i4Iter);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
    }

    // SW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");
            
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // SW Action
    // 1. colorchange bitblt kodak16 over kodak14
    _CLI_CMD("gfxemu.color 0x00000000");
    for (i4Iter = 0; i4Iter < 256; i4Iter++)
    {
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x colorchange",
            0, 0, 0, i4Iter, i4Iter, i4Iter);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
    }

    // 2. transparent kodak17 over kodak16
    for (i4Iter = 0; i4Iter < 256; i4Iter++)
    {
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x colorchange",
            0, 0, 0, i4Iter, i4Iter, i4Iter);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
    }

    // 3. transparent kodak14 over kodak17
    for (i4Iter = 0; i4Iter < 256; i4Iter++)
    {
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x colorchange",
            0, 0, 0, i4Iter, i4Iter, i4Iter);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
    }

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    if (_GfxBufferCompare(
        (UINT32 *)COMPARE_BUF, 
        (UINT32 *)(BUF_SCREEN), u4Size) == -1)
    {
        return -1;
    }

    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdBlt6
 *  Normal BitBlt Test 06 (Random)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdBlt6(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count;
    INT32 i4SrcX, i4SrcY;
    UINT32 u4Min, u4Max;
    UINT32 u4Chg, u4Keynot;
    INT32 i4PicSel, i4ColIndex;
    INT32 i4CountNum = 2000;

    Printf("Normal BitBlt Test 06 ");

    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);
   
    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        u4Min = 0;
        u4Max = 0xff7f7f7f;
        u4Chg = ((UINT32)rand()) & 1;
        u4Keynot = ((UINT32)rand()) & 1;
        i4Left = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom = ((UINT32)rand()) % _u4ScreenHeight;
        i4ColIndex = ((UINT32)rand()) % 147;

        i4Width = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -i4Height;
            i4Top = i4Bottom;
        }
        i4Width++;
        i4Height++;

        if ((_u4ScreenWidth - i4Width) != 0)
        {
            i4SrcX = ((UINT32)rand()) % (_u4ScreenWidth - i4Width);
        }
        else
        {
            i4SrcX = 0;
        }

        if ((_u4ScreenHeight - i4Height) != 0)
        {
            i4SrcY = ((UINT32)rand()) % (_u4ScreenHeight - i4Height);
        }
        else
        {
            i4SrcY = 0;
        }

        i4PicSel = ((UINT32)rand()) % 3;

        Printf("%d Run : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d, ColIndex = %d\n",
            i4Left, i4Top, i4Width, i4Height, i4ColIndex);
        Printf("gfx.set.bltopt 0x%08x 0x%08x %s %s\n",
                u4Min, u4Max,
                u4Chg ? "colorchange" : "transparent",
                u4Keynot ? "keynot" : " ");
        
        if (i4Width >= 3 && i4Height >= 3)
        {
            i4Width -= 2;
            i4Height -= 2;
            i4Left++;
            i4Top++;
        }
        else
        {
            Printf("(i4Width = %d, i4Height = %d\n", i4Width, i4Height);
            continue;
        }

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        #if defined(CC_MT5398)
    	GFX_SetXORColor(_arNamedColor[i4ColIndex].u4Color);
		#endif
		
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _CLI_CMD("gfx.op.vline %d %d %d", i4Left, i4Top, i4Height);
        _CLI_CMD("gfx.op.vline %d %d %d", i4Left + i4Width - 1, i4Top, i4Height);
        _CLI_CMD("gfx.flush");
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x%08x 0x%08x %s %s",
                u4Min, u4Max,
                u4Chg ? "colorchange" : "transparent",
                u4Keynot ? "keynot" : " ");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                    i4SrcX, i4SrcY, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");


        // SW
        
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");
            
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        #if defined(CC_MT5398)
    	GFX_SetXORColor(_arNamedColor[i4ColIndex].u4Color);
		#endif
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _CLI_CMD("gfx.op.vline %d %d %d", i4Left, i4Top, i4Height);
        _CLI_CMD("gfx.op.vline %d %d %d", i4Left + i4Width - 1, i4Top, i4Height);
        _CLI_CMD("gfx.flush");
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0x%08x 0x%08x %s %s",
                u4Min, u4Max,
                u4Chg ? "colorchange" : "transparent",
                u4Keynot ? "keynot" : " ");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                    i4SrcX, i4SrcY, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Normal BitBlt Test 06 Error ");
            return -1;
        }
        
    }
    Printf("Total Run = %d\n", i4CountNum);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdBlt7
 *  Normal BitBlt Test 07
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdBlt7(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;

    Printf("Normal BitBlt Test 07 ");

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    // 1. Draw rectangles
    _GfxSetDefMode(BUF_TEMP, ADR_DST);
    _CLI_CMD("gfxemu.color 0x693845ac");
    _CLI_CMD("gfx.op.rect 0 0 85 480");
    _CLI_CMD("gfxemu.color 0x87368736");
    _CLI_CMD("gfx.op.rect 85 0 85 480");
    _CLI_CMD("gfxemu.color 0x5a8a8a5a");
    _CLI_CMD("gfx.op.rect 170 0 86 480");
    _CLI_CMD("gfxemu.color 0xebfbabdb");
    _CLI_CMD("gfx.op.rect 0 0 256 240");

    // 2. do normal bitblt
    _GfxSetDefMode(BUF_TEMP, ADR_SRC);
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.op.bitblt 0 0 0 0 256 480");
    _CLI_CMD("gfx.flush");

    // 3. do transparent bitblt
    _GfxSetDefMode(BUF_TEMP, ADR_SRC);
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _CLI_CMD("gfx.set.bltopt 0xdb 0xffffffff transparent");
    _CLI_CMD("gfx.op.bitblt 0 0 300 0 256 480");
    _CLI_CMD("gfx.flush");


    // SW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");
        
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    // 1. Draw rectangles
    _GfxSetDefMode(BUF_TEMP, ADR_DST);
    _CLI_CMD("gfxemu.color 0x693845ac");
    _CLI_CMD("gfx.op.rect 0 0 85 480");
    _CLI_CMD("gfxemu.color 0x87368736");
    _CLI_CMD("gfx.op.rect 85 0 85 480");
    _CLI_CMD("gfxemu.color 0x5a8a8a5a");
    _CLI_CMD("gfx.op.rect 170 0 86 480");
    _CLI_CMD("gfxemu.color 0xebfbabdb");
    _CLI_CMD("gfx.op.rect 0 0 256 240");

    // 2. do normal bitblt
    _GfxSetDefMode(BUF_TEMP, ADR_SRC);
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.op.bitblt 0 0 0 0 256 480");
    _CLI_CMD("gfx.flush");

    // 3. do transparent bitblt
    _GfxSetDefMode(BUF_TEMP, ADR_SRC);
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _CLI_CMD("gfx.set.bltopt 0xdb 0xffffffff transparent");
    _CLI_CMD("gfx.op.bitblt 0 0 300 0 256 480");
    _CLI_CMD("gfx.flush");

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    return _GfxBufferCompare(
                (UINT32 *)COMPARE_BUF,
                (UINT32 *)(BUF_SCREEN),
                u4Size);
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdBlt8
 *  Normal BitBlt Test 08 (Random)
 *  SRC color key and DST color key
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdBlt8(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count;
    INT32 i4SrcX, i4SrcY;
    UINT32 u4Min, u4Max;
    UINT32 u4Chg, u4Keynot, u4KeySdSel;
    INT32 i4PicSel, i4ColIndex;
    INT32 i4CountNum = 2000;

    Printf("Normal BitBlt Test 08 (src/dst color key random)");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);
   
    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        u4Min = 0;
        u4Max = 0xff7f7f7f;
        u4Chg = ((UINT32)rand()) & 1;
        u4Keynot = ((UINT32)rand()) & 1;
        u4KeySdSel = ((UINT32)rand()) & 1;
        i4Left = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom = ((UINT32)rand()) % _u4ScreenHeight;
        i4ColIndex = ((UINT32)rand()) % 147;

        i4Width = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -i4Height;
            i4Top = i4Bottom;
        }
        i4Width++;
        i4Height++;

        if ((_u4ScreenWidth - i4Width) != 0)
        {
            i4SrcX = ((UINT32)rand()) % (_u4ScreenWidth - i4Width);
        }
        else
        {
            i4SrcX = 0;
        }

        if ((_u4ScreenHeight - i4Height) != 0)
        {
            i4SrcY = ((UINT32)rand()) % (_u4ScreenHeight - i4Height);
        }
        else
        {
            i4SrcY = 0;
        }

        i4PicSel = ((UINT32)rand()) % 3;

        Printf("%d Run : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d, ColIndex = %d\n",
            i4Left, i4Top, i4Width, i4Height, i4ColIndex);

        Printf("gfx.set.bltopt 0x%08x 0x%08x %s %s %s\n",
                u4Min, u4Max,
                u4Chg ? "colorchange" : "transparent",
                u4Keynot ? "keynot" : "",
                u4KeySdSel ? "keysdsel" : "");
        
        if (i4Width >= 3 && i4Height >= 3)
        {
            i4Width -= 2;
            i4Height -= 2;
            i4Left++;
            i4Top++;
        }
        else
        {
            Printf("(i4Width = %d, i4Height = %d\n", i4Width, i4Height);
            continue;
        }

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        #if defined(CC_MT5398)
    	GFX_SetXORColor(_arNamedColor[i4ColIndex].u4Color);
		#endif
		
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                    i4SrcX, i4SrcY, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.set.bltopt 0x%08x 0x%08x %s %s %s",
                u4Min, u4Max,
                u4Chg ? "colorchange" : "transparent",
                u4Keynot ? "keynot" : "",
                u4KeySdSel ? "keysdsel" : "");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                    i4SrcX, i4SrcY, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");

        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");
            
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                    i4SrcX, i4SrcY, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.set.bltopt 0x%08x 0x%08x %s %s %s",
                u4Min, u4Max,
                u4Chg ? "colorchange" : "transparent",
                u4Keynot ? "keynot" : "",
                u4KeySdSel ? "keysdsel" : "");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                    i4SrcX, i4SrcY, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Normal BitBlt Test 08 (src/dst color key random) Error\n");
            return -1;
        }
    }
    Printf("Total Run = %d\n", i4CountNum);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdBlt9
 *  Normal BitBlt Test 09
 *  Bitblt pixel-by-pixel
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdBlt9(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4Width, i4Height, i4Count;
    INT32 i4SrcX, i4SrcY;
    INT32 i4PicSel;
    INT32 i4CountNum = 2000;

    Printf("Normal BitBlt Test 09 ");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);
    i4Width  = (INT32)_u4ScreenWidth;
    i4Height = (INT32)_u4ScreenHeight;

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        Printf("Loop = %d : ", i4Count);

        i4PicSel = ((UINT32)rand()) % 3;

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // HW Action
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        for (i4SrcY = 0; i4SrcY < i4Height; i4SrcY++)
        {
            for (i4SrcX = 0; i4SrcX < i4Width; i4SrcX++)
            {
                _CLI_CMD("gfx.set.bltopt 0 0 0");
                _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                        i4SrcX, i4SrcY, i4SrcX, i4SrcY, 1, 1);
            }
        }
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");
            
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // SW Action
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        for (i4SrcY = 0; i4SrcY < i4Height; i4SrcY++)
        {
            for (i4SrcX = 0; i4SrcX < i4Width; i4SrcX++)
            {
                _CLI_CMD("gfx.set.bltopt 0 0 0");
                _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                        i4SrcX, i4SrcY, i4SrcX, i4SrcY, 1, 1);
            }
        }
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Normal BitBlt Test 09 Error\n");
            return -1;
        }
    } // ~for

    Printf("Total Run = %d\n", i4CountNum);
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdBlt10
 *  Normal BitBlt Test 10 (random, only run hw)
 *  SRC color key and DST color key
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdBlt10(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count;
    INT32 i4SrcX, i4SrcY;
    UINT32 u4Min, u4Max;
    UINT32 u4Chg, u4Keynot, u4KeySdSel;
    INT32 i4PicSel, i4ColIndex;
    INT32 i4CountNum = 2000;
    //UINT32 u4Size;

    Printf("Normal BitBlt Test 10 (src/dst color key random, only run hw) ...");



    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);
#ifdef GFX_BURST_READ
    return 0;
#endif


    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        u4Min = 0;
        u4Max = 0xff7f7f7f;
        u4Chg = ((UINT32)rand()) & 1;
        u4Keynot = ((UINT32)rand()) & 1;
        u4KeySdSel = ((UINT32)rand()) & 1;
        i4Left = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom = ((UINT32)rand()) % _u4ScreenHeight;
        i4ColIndex = ((UINT32)rand()) % 147;

        i4Width = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -i4Height;
            i4Top = i4Bottom;
        }
        i4Width++;
        i4Height++;

        if ((_u4ScreenWidth - i4Width) != 0)
        {
            i4SrcX = ((UINT32)rand()) % (_u4ScreenWidth - i4Width);
        }
        else
        {
            i4SrcX = 0;
        }

        if ((_u4ScreenHeight - i4Height) != 0)
        {
            i4SrcY = ((UINT32)rand()) % (_u4ScreenHeight - i4Height);
        }
        else
        {
            i4SrcY = 0;
        }

        i4PicSel = ((UINT32)rand()) % 3;

        Printf("%d Run : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d, ColIndex = %d\n",
            i4Left, i4Top, i4Width, i4Height, i4ColIndex);

        Printf("gfx.set.bltopt 0x%08x 0x%08x %s %s %s\n",
                u4Min, u4Max,
                u4Chg ? "colorchange" : "transparent",
                u4Keynot ? "keynot" : "",
                u4KeySdSel ? "keysdsel" : "");
        
        if (i4Width >= 3 && i4Height >= 3)
        {
            i4Width -= 2;
            i4Height -= 2;
            i4Left++;
            i4Top++;
        }
        else
        {
            Printf("(i4Width = %d, i4Height = %d\n", i4Width, i4Height);
            continue;
        }
        //HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        #if defined(CC_MT5398)
    	GFX_SetXORColor(_arNamedColor[i4ColIndex].u4Color);
		#endif
		
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                    i4SrcX, i4SrcY, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.set.bltopt 0x%08x 0x%08x %s %s %s",
                u4Min, u4Max,
                u4Chg ? "colorchange" : "transparent",
                u4Keynot ? "keynot" : "",
                u4KeySdSel ? "keysdsel" : "");
        
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                    i4SrcX, i4SrcY, i4Left, i4Top, i4Width, i4Height);
        
    }
    _CLI_CMD("gfx.flush");
    Printf("Normal BitBlt Test 10 (src/dst color key random, only run hw) ... OK!!");
    Printf("Total Run = %d\n", i4CountNum);
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdBlt11
 *  Normal BitBlt Test 11 (flip test)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdBlt11(INT32 i4Argc, const CHAR ** szArgv)
{
#ifdef GFX_BURST_READ
    return 0;
#else

    UINT32 u4Size = 0;

    Printf("Normal BitBlt Test 11 (basic flip & mirror) ");



    // HW
    _CLI_CMD("gfx.reset");
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _CLI_CMD("gfx.set.bltopt 0x0 0xffffffff flip mirror");
    _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
    _CLI_CMD("gfx.flush");


    // SW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");
        
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _CLI_CMD("gfx.set.bltopt 0x0 0xffffffff flip mirror");
    _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
    _CLI_CMD("gfx.flush");

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    if (_GfxBufferCompare(
            (UINT32 *)COMPARE_BUF, 
            (UINT32 *)(BUF_SCREEN), u4Size) == -1)
    {
        Printf("Normal BitBlt Test 11 (basic flip & mirror) Error\n");
        return -1;
    }

    Printf("Normal BitBlt Test 11 (basic flip & mirror) ... Done OK !!\n");
    return 0;
#endif
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdBlt12
 *  Normal BitBlt Test 12 (random flip | mirror)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdBlt12(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count;
    UINT32 u4Min, u4Max;
    UINT32 u4Flip, u4Mirror;
    INT32 i4PicSel, i4ColIndex;
    INT32 i4CountNum = 2000;

    Printf("Normal BitBlt Test 12 (random flip | mirror)");
    
#ifdef GFX_BURST_READ
    //return 0;
#endif

    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);
   
    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        //do {
        u4Min = 0;
        u4Max = 0xffffffff;
        u4Flip = ((UINT32)rand()) & 1;
        u4Mirror = ((UINT32)rand()) & 1;
        i4Left = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom = ((UINT32)rand()) % _u4ScreenHeight;
        i4ColIndex = ((UINT32)rand()) % 147;


        i4Width = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -i4Height;
            i4Top = i4Bottom;
        }
        i4Width++;
        i4Height++;
        //}while(i4Width >4);
        
        i4PicSel = 0;//((UINT32)rand()) % 3;

        Printf("%d Run : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d, ColIndex = %d\n",
            i4Left, i4Top, i4Width, i4Height, i4ColIndex);

        Printf("gfx.set.bltopt 0x%08x 0x%08x %s %s\n",
                u4Min, u4Max,
                u4Flip ? "flip" : "",
                u4Mirror ? "mirror" : "");

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        _GfxSetDefMode(BUF_TEMP, ADR_DST);
        _GfxSetDefMode(BUF_TEMP, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _GfxSetDefMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                    i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _GfxSetDefMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0x%08x 0x%08x %s %s",
                u4Min, u4Max,
                u4Flip ? "flip" : "",
                u4Mirror ? "mirror" : "");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                    0, 0, 0, 0, _u4ScreenWidth, _u4ScreenHeight);
        _CLI_CMD("gfx.flush");

        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");
            
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        _GfxSetDefMode(BUF_TEMP, ADR_DST);
        _GfxSetDefMode(BUF_TEMP, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _GfxSetDefMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                    i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _GfxSetDefMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0x%08x 0x%08x %s %s",
                u4Min, u4Max,
                u4Flip ? "flip" : "",
                u4Mirror ? "mirror" : "");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                    0, 0, 0, 0, _u4ScreenWidth, _u4ScreenHeight);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Normal BitBlt Test 12 (random flip | mirror) Error\n");
            return -1;
        }
    }
    Printf("Total Run = %d\n", i4CountNum);
    Printf("Normal BitBlt Test 12 (random flip | mirror)...OK!!\n");
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdBlt13
 *  Normal BitBlt Test 13 (only run hw, flip/mirr)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdBlt13(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count;
    INT32 i4SrcX, i4SrcY;
    UINT32 u4Min, u4Max;
    UINT32 u4Flip, u4Mirror;
    INT32 i4PicSel, i4ColIndex;
    INT32 i4CountNum = 2000;

    Printf("Normal BitBlt Test 13 (only run hw, flip/mirr) ...");



    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);
#ifdef GFX_BURST_READ
    return 0;
#endif

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        u4Min = 0;
        u4Max = 0xff7f7f7f;
        u4Flip = ((UINT32)rand()) & 1;
        u4Mirror = ((UINT32)rand()) & 1;
        i4Left = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom = ((UINT32)rand()) % _u4ScreenHeight;
        i4ColIndex = ((UINT32)rand()) % 147;

        i4Width = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -i4Height;
            i4Top = i4Bottom;
        }
        i4Width++;
        i4Height++;

        if ((_u4ScreenWidth - i4Width) != 0)
        {
            i4SrcX = ((UINT32)rand()) % (_u4ScreenWidth - i4Width);
        }
        else
        {
            i4SrcX = 0;
        }

        if ((_u4ScreenHeight - i4Height) != 0)
        {
            i4SrcY = ((UINT32)rand()) % (_u4ScreenHeight - i4Height);
        }
        else
        {
            i4SrcY = 0;
        }

        i4PicSel = ((UINT32)rand()) % 3;

        Printf("%d Run : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d, ColIndex = %d\n",
            i4Left, i4Top, i4Width, i4Height, i4ColIndex);

        Printf("gfx.set.bltopt 0x%08x 0x%08x %s %s %s\n",
                u4Min, u4Max,
                u4Flip ? "flip" : "",
                u4Mirror ? "mirror" : "");
        
        if (i4Width >= 3 && i4Height >= 3)
        {
            i4Width -= 2;
            i4Height -= 2;
            i4Left++;
            i4Top++;
        }
        else
        {
            Printf("i4Width = %d, i4Height = %d\n", i4Width, i4Height);
            continue;
        }

        // Action
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                    i4SrcX, i4SrcY, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.set.bltopt 0x%08x 0x%08x %s %s %s",
                u4Min, u4Max,
                u4Flip ? "flip" : "",
                u4Mirror ? "mirror" : "");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                    i4SrcX, i4SrcY, i4Left, i4Top, i4Width, i4Height);
    }
    _CLI_CMD("gfx.flush");
    Printf("Normal BitBlt Test 13 (only run hw, flip/mirr) ... OK!!");
    Printf("Total Run = %d\n", i4CountNum);
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdBlt14
 *  Normal BitBlt Test 14 (single flip | mirror)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdBlt14(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    //INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Left, i4Top;
    INT32 i4Width, i4Height, i4Count;
    UINT32 u4Min, u4Max;
    UINT32 u4Flip, u4Mirror;
    INT32 i4PicSel, i4ColIndex;
    INT32 i4CountNum = 2000;

    Printf("Normal BitBlt Test 14 (single flip | mirror)");



    if (i4Argc < 9)
    {
        Printf("arg: loops, flipon, mirron, L, T, W, H, ColIdx\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);
    u4Min = 0;
    u4Max = 0xffffffff;
    u4Flip   = (INT32) StrToInt(szArgv[2]);
    u4Mirror = (INT32) StrToInt(szArgv[3]);
    i4Left   = (INT32) StrToInt(szArgv[4]);
    i4Top    = (INT32) StrToInt(szArgv[5]);
    i4Width  = (INT32) StrToInt(szArgv[6]);
    i4Height = (INT32) StrToInt(szArgv[7]);
    i4ColIndex = (INT32) StrToInt(szArgv[8]);
#ifdef GFX_BURST_READ
    return 0;
#endif

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        i4PicSel = ((UINT32)rand()) % 3;

        Printf("%d Run : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d, ColIndex = %d\n",
            i4Left, i4Top, i4Width, i4Height, i4ColIndex);
        Printf("gfx.set.bltopt 0x%08x 0x%08x %s %s\n",
                u4Min, u4Max,
                u4Flip ? "flip" : "",
                u4Mirror ? "mirror" : "");

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        _GfxSetDefMode(BUF_TEMP, ADR_DST);
        _GfxSetDefMode(BUF_TEMP, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _GfxSetDefMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                    i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _GfxSetDefMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0x%08x 0x%08x %s %s",
                u4Min, u4Max,
                u4Flip ? "flip" : "",
                u4Mirror ? "mirror" : "");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                    0, 0, 0, 0, _u4ScreenWidth, _u4ScreenHeight);
        _CLI_CMD("gfx.flush");

        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");
            
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        _GfxSetDefMode(BUF_TEMP, ADR_DST);
        _GfxSetDefMode(BUF_TEMP, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _GfxSetDefMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                    i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _GfxSetDefMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0x%08x 0x%08x %s %s",
                u4Min, u4Max,
                u4Flip ? "flip" : "",
                u4Mirror ? "mirror" : "");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                    0, 0, 0, 0, _u4ScreenWidth, _u4ScreenHeight);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Normal BitBlt Test 14 (single flip | mirror) Error\n");
            return -1;
        }
    }
    Printf("Total Run = %d\n", i4CountNum);
    Printf("Normal BitBlt Test 14 (single flip | mirror)...OK!!\n");
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdDMA1
 *  1-D BitBlt Test 01 (only for ARGB8888 test)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdDMA1(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 au4Buf[3];
    au4Buf[0] = BUF_ADDR1;
    au4Buf[1] = BUF_ADDR2;
    au4Buf[2] = BUF_ADDR3;

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
    
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");
        
    /* 1. copy kodak14 to addr4 */
    _GfxSetSdMode(BUF_TEMP, ADR_DST);
    _GfxSetSdMode(au4Buf[((UINT32)rand()) % 3], ADR_SRC);
    _CLI_CMD("gfx.op.bitblt 0 0 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.flush");
    
    /* 2. Copy to display buffer */
    _CLI_CMD("gfx.s.dst 0x%08x argb8888 %d", BUF_SCREEN, _u4GfxPitch);
    _CLI_CMD("gfx.op.dma 0x%08x 0x%08x %d", BUF_SCREEN, BUF_TEMP, 
            (_u4ScreenWidth * _u4ScreenHeight));
    _CLI_CMD("gfx.flush");

    Printf("1-D BitBlt OK !!\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdBlend1
 *  Alpha Blending Test 01 (all alpha values 0-255)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdBlend1(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;

    // load 3 test bitmap first (@ 0x01151800, 0x012A3000, 0x013F4800)
    INT32 i4Iter, i4Step;
    INT32 i4ArStart = 0, i4ArEnd = 255;

    Printf("Alpha Blending Test 01 ");
    
    if (i4Argc < 3)
    {
        Printf("args: ArStart, ArEnd\n");
        return -1;
    }

    i4ArStart = (INT32) StrToInt(szArgv[1]);
    i4ArEnd   = (INT32) StrToInt(szArgv[2]);

    // 1. blend kodak16 over kodak14
    i4Step = 1;
    for (i4Iter = i4ArStart; i4Iter <= i4ArEnd; i4Iter++)
    {
        Printf("%d Step, %3d Run : ", i4Step, i4Iter);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _CLI_CMD("gfx.set.alpha %d", i4Iter);
        _CLI_CMD("gfx.op.blend 0 0 0 0 720 480");

        // debug use
        _CLI_CMD("gfx.flush");

        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _CLI_CMD("gfx.set.alpha %d", i4Iter);
        _CLI_CMD("gfx.op.blend 0 0 0 0 720 480");
        
        // debug use
        _CLI_CMD("gfx.flush");

        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("%d Step, Alpha Value = %d\n", i4Step, i4Iter);
            return -1;
        }
    }

    // 2. blend kodak17 over kodak16
    i4Step = 2;
    for (i4Iter = i4ArStart; i4Iter <= i4ArEnd; i4Iter++)
    {
        Printf("%d Step, %3d Run : ", i4Step, i4Iter);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _CLI_CMD("gfx.set.alpha %d", i4Iter);
        _CLI_CMD("gfx.op.blend 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");
        
        // Action
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _CLI_CMD("gfx.set.alpha %d", i4Iter);
        _CLI_CMD("gfx.op.blend 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("%d Step, Alpha Value = %d\n", i4Step, i4Iter);
            return -1;
        }
    }

    // 3. blend kodak14 over kodak17
    i4Step = 3;
    for (i4Iter = i4ArStart; i4Iter <= i4ArEnd; i4Iter++)
    {
        Printf("%d Step, %3d Run : ", i4Step, i4Iter);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.set.alpha %d", i4Iter);
        _CLI_CMD("gfx.op.blend 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.set.alpha %d", i4Iter);
        _CLI_CMD("gfx.op.blend 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("%d Step, Alpha Value = %d\n", i4Step, i4Iter);
            return -1;
        }
    }

    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdBlend2
 *  Alpha Blending Test 02 (Random)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdBlend2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count;
    INT32 i4SrcX, i4SrcY;
    INT32 i4Alpha;
    INT32 i4PicSel;
    INT32 i4CountNum = 5000;
    UINT32 u4Color;

    Printf("Alpha Blending Test 02 (random) ");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        i4Alpha = ((UINT32)rand()) % 256;
        i4Left = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom = ((UINT32)rand()) % _u4ScreenHeight;
        i4PicSel = ((UINT32)rand()) % 3;
        u4Color = _arNamedColor[((UINT32)rand()) % 147].u4Color;

        i4Width = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -i4Height;
            i4Top = i4Bottom;
        }
        i4Width++;
        i4Height++;
        
        if (i4Width >= 3 && i4Height >= 3)
        {
            i4Width -= 2;
            i4Height -= 2;
        }
        i4Left++;
        i4Top++;

        if ((_u4ScreenWidth - i4Width) != 0)
        {
            i4SrcX = ((UINT32)rand()) % (_u4ScreenWidth - i4Width);
        }
        else
        {
            i4SrcX = 0;
        }

        if ((_u4ScreenHeight - i4Height) != 0)
        {
            i4SrcY = ((UINT32)rand()) % (_u4ScreenHeight - i4Height);
        }
        else
        {
            i4SrcY = 0;
        }

        Printf("%d Run : ", i4Count);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _CLI_CMD("gfxemu.color 0x%08x", u4Color);
        _CLI_CMD("gfx.op.vline %d %d %d", i4Left, i4Top, i4Height);
        _CLI_CMD("gfx.op.vline %d %d %d", i4Left + i4Width - 1, i4Top, i4Height);
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _CLI_CMD("gfx.set.alpha %d", i4Alpha);
        _CLI_CMD("gfx.op.blend %d %d %d %d %d %d",
                i4SrcX, i4SrcY, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _CLI_CMD("gfxemu.color 0x%08x", u4Color);
        _CLI_CMD("gfx.op.vline %d %d %d", i4Left, i4Top, i4Height);
        _CLI_CMD("gfx.op.vline %d %d %d", i4Left + i4Width - 1, i4Top, i4Height);
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _CLI_CMD("gfx.set.alpha %d", i4Alpha);
        _CLI_CMD("gfx.op.blend %d %d %d %d %d %d",
                i4SrcX, i4SrcY, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);

        /* Compare HW and SW */
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Alpha Blending Test 02 Error\n");
            Printf("PS = %d, Sx = %d, Sy = %d, L = %d, T = %d, W = %d, H = %d, Ar = %d\n", 
                i4PicSel, i4SrcX, i4SrcY, i4Left, i4Top, i4Width, i4Height, i4Alpha);
            return -1;
        }
    }
    
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdBlend3
 *  Alpha Blending Test 03 (random, only run hw)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdBlend3(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count;
    INT32 i4SrcX, i4SrcY;
    INT32 i4Alpha;
    INT32 i4PicSel;
    INT32 i4CountNum = 5000;
    UINT32 u4Color;

    Printf("Alpha Blending Test 03 (random, only run hw) ... ");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        i4Alpha = ((UINT32)rand()) % 256;
        i4Left = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom = ((UINT32)rand()) % _u4ScreenHeight;
        i4PicSel = ((UINT32)rand()) % 3;
        u4Color = _arNamedColor[((UINT32)rand()) % 147].u4Color;

        i4Width = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -i4Height;
            i4Top = i4Bottom;
        }
        i4Width++;
        i4Height++;
        
        if (i4Width >= 3 && i4Height >= 3)
        {
            i4Width -= 2;
            i4Height -= 2;
        }
        i4Left++;
        i4Top++;

        if ((_u4ScreenWidth - i4Width) != 0)
        {
            i4SrcX = ((UINT32)rand()) % (_u4ScreenWidth - i4Width);
        }
        else
        {
            i4SrcX = 0;
        }

        if ((_u4ScreenHeight - i4Height) != 0)
        {
            i4SrcY = ((UINT32)rand()) % (_u4ScreenHeight - i4Height);
        }
        else
        {
            i4SrcY = 0;
        }

        Printf("%d Run : ", i4Count);

        // Action
        _CLI_CMD("gfxemu.color 0x%08x", u4Color);
        _CLI_CMD("gfx.op.vline %d %d %d", i4Left, i4Top, i4Height);
        _CLI_CMD("gfx.op.vline %d %d %d", i4Left + i4Width - 1, i4Top, i4Height);
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _CLI_CMD("gfx.set.alpha %d", i4Alpha);
        _CLI_CMD("gfx.op.blend %d %d %d %d %d %d",
                i4SrcX, i4SrcY, i4Left, i4Top, i4Width, i4Height);
    }
    _CLI_CMD("gfx.flush");
    Printf("Alpha Blending Test 03 (random, only run hw) ... OK!!");
    Printf("Total Run = %d\n", i4CountNum);
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdCompPass0
 *  Alpha Composition Pass 0
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCompPass0(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    UINT32 u4Start = 0, u4End = 255, i;

    Printf("Alpha Composition Pass 0 Test\n");

    x_memset((UINT32*)BUF_SCREEN,0,_u4ScreenWidth * _u4ScreenHeight*4);
    x_memset((UINT32*)COMPARE_BUF,0,_u4ScreenWidth * _u4ScreenHeight*4);
    
    if (i4Argc < 3)
    {
        Printf("args: ArStart, ArEnd\n");
        return -1;
    }

    u4Start = StrToInt(szArgv[1]);
    u4End   = StrToInt(szArgv[2]);
    
    for (i = u4Start; i <= u4End; i++)
    {
        Printf("%u Run : ", i);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.op.cp 0 0 0 0 %d %d 0 %d", 
            _u4ScreenWidth, _u4ScreenHeight, i);
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");
            
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.op.cp 0 0 0 0 %d %d 0 %d", 
            _u4ScreenWidth, _u4ScreenHeight, i);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;        
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Alpha Composition Pass 0 Test\n");
            Printf("Alpha = %u\n", i);
            return -1;
        }
    }   // ~for (i = u4Start; i <= u4End; i++)
    Printf("Alpha Composition Pass 0 (%u to %u) OK !\n", u4Start, u4End);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdCompPass1
 *  Alpha Composition Pass 1
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCompPass1(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    UINT32 u4Start = 0, u4End = 255, i;

    Printf("Alpha Composition Pass 1 Test\n");

    if (i4Argc < 3)
    {
        Printf("args: ArStart, ArEnd\n");
        return -1;
    }

    u4Start = StrToInt(szArgv[1]);
    u4End   = StrToInt(szArgv[2]);

    for (i = u4Start; i <= u4End; i++)
    {
        Printf("%u Run : ", i);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.op.cp 0 0 0 0 %d %d 1 %d", 
            _u4ScreenWidth, _u4ScreenHeight, i);
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");
            
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.op.cp 0 0 0 0 %d %d 1 %d", 
            _u4ScreenWidth, _u4ScreenHeight, i);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;        
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Alpha Composition Pass 1 Test\n");
            Printf("Alpha = %u\n", i);
            return -1;
        }
    }   // ~for (i = u4Start; i <= u4End; i++)
    Printf("Alpha Composition Pass 1 (%u to %u) OK !\n", u4Start, u4End);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdCompPass2
 *  Alpha Composition Pass 2
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCompPass2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    UINT32 u4Start = (UINT32)E_AC_CLEAR;
    UINT32 u4End = (UINT32)E_AC_SRC_OVER;
    UINT32 i;

    Printf("Alpha Composition Pass 2 Test\n");
    
    if (i4Argc < 3)
    {
        Printf("args: OpStart, OpEnd\n");
        return -1;
    }

    u4Start = StrToInt(szArgv[1]);
    u4End   = StrToInt(szArgv[2]);
    
    for (i = u4Start; i <= u4End; i++)
    {
        Printf("%u Run : ", i);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.op.cp 0 0 0 0 %d %d 2 %d", 
            _u4ScreenWidth, _u4ScreenHeight, i);
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");
            
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.op.cp 0 0 0 0 %d %d 2 %d", 
            _u4ScreenWidth, _u4ScreenHeight, i);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;        
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Alpha Composition Pass 2 Test\n");
            Printf("Alpha = %u\n", i);
            return -1;
        }
    }   // ~for (i = u4Start; i <= u4End; i++)
    Printf("Alpha Composition Pass 2 (%u to %u) OK !\n", u4Start, u4End);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdCompPass3
 *  Alpha Composition Pass 3
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCompPass3(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    UINT32 u4Start = 0, u4End = 255, i;

    Printf("Alpha Composition Pass 3 Test\n");

    if (i4Argc < 3)
    {
        Printf("args: ArStart, ArEnd\n");
        return -1;
    }

    u4Start = StrToInt(szArgv[1]);
    u4End   = StrToInt(szArgv[2]);
    
    for (i = u4Start; i <= u4End; i++)
    {
        Printf("%u Run : ", i);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.op.cp 0 0 0 0 %d %d 3 %d", _u4ScreenWidth, _u4ScreenHeight, i);
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");
            
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.op.cp 0 0 0 0 %d %d 3 %d", _u4ScreenWidth, _u4ScreenHeight, i);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;        
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Alpha Composition Pass 3 Test\n");
            Printf("Alpha = %u\n", i);
            return -1;
        }
    }   // ~for (i = u4Start; i <= u4End; i++)
    Printf("Alpha Composition Pass 3 (%u to %u) OK !\n", u4Start, u4End);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdCompPass45
 *  Alpha Composition Pass 4 & 5
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCompPass45(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    UINT32 u4Start = 0, u4End = 255, u4Ar;   

    Printf("Alpha Composition Pass 4 & 5 Test\n");
    
    for (u4Ar = u4Start; u4Ar <= u4End; u4Ar += 51)
    {
        Printf("[Ar = %u] ", u4Ar);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _CLI_CMD("gfx.set.alpha %d", u4Ar);
        _CLI_CMD("gfx.op.blend 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.op.cp 0 0 0 0 %d %d 4 255", _u4ScreenWidth, _u4ScreenHeight);
        _CLI_CMD("gfx.op.cp 0 0 0 0 %d %d 5 255", _u4ScreenWidth, _u4ScreenHeight);
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");
                
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _CLI_CMD("gfx.set.alpha %d", u4Ar);
        _CLI_CMD("gfx.op.blend 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.op.cp 0 0 0 0 %d %d 4 255", _u4ScreenWidth, _u4ScreenHeight);
        _CLI_CMD("gfx.op.cp 0 0 0 0 %d %d 5 255", _u4ScreenWidth, _u4ScreenHeight);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;        
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Alpha Composition Pass 4 & 5 Test\n");
            Printf("Ar = %u\n", u4Ar);
            return -1;
        }
    }
    Printf("Alpha Composition Pass 4 and 5 OK !\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdCompose1
 *  Alpha Composition Test 01 (CLEAR)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCompose1(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    
    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    // Action 1
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.color 0x000000");
    _CLI_CMD("gfx.op.rect 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.flush");

    // Action 2
    //gfx.op.compose {sx} {sy} {dx} {dy} {w} {h} {ar} {mode}
    _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _CLI_CMD("gfx.set.bltopt 0 0");
    _CLI_CMD("gfx.op.bitblt 0 0 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.op.compose 0 0 0 0 %u %u 255 0", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.flush");

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    if (_GfxBufferCompare(
            (UINT32 *)COMPARE_BUF, (UINT32 *)BUF_SCREEN, u4Size) == -1)
    {
        Printf("Alpha Composition Test 01 FAIL (CLEAR) >_<\n");
        return -1;
    }

    Printf("Alpha Composition Test 01 OK (CLEAR) ^_^\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdCompose2
 *  Alpha Composition Test 02 (SRC)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCompose2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size;
    UINT32 u4Ar = 255;

    if (i4Argc < 2)
    {
        Printf("Arg: urAr (0~255)\n");
        return -1;
    }
    u4Ar = StrToInt(szArgv[1]);
    
    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
        
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    //gfx.op.compose {sx} {sy} {dx} {dy} {w} {h} {ar} {mode}
    _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
    _CLI_CMD("gfx.op.compose 0 0 0 0 %u %u %u 4", 
        _u4ScreenWidth, _u4ScreenHeight, u4Ar);
    _CLI_CMD("gfx.flush");

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    if (_GfxBufferCompare(
            (UINT32 *)BUF_ADDR1, (UINT32 *)BUF_SCREEN, u4Size) == -1)
    {
        Printf("Alpha Composition Test 02 FAIL (SRC, Ar=%u) >_<\n", u4Ar);
        return -1;
    }

    Printf("Alpha Composition Test 02 OK (SRC, Ar=%u) ^_^\n", u4Ar);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdCompose3
 *  Alpha Composition Test 03 (SRC, As=0~255, Ar=0 or 255)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCompose3(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size, u4RectColor, u4Ar, u4As;

    if (i4Argc < 2)
    {
        Printf("Arg: urAr (0 or 255)\n");
        return -1;
    }
    u4Ar = StrToInt(szArgv[1]);

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
        
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    //gfx.op.compose {sx} {sy} {dx} {dy} {w} {h} {ar} {mode}
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _CLI_CMD("gfxemu.color 0xFF000000");
    _CLI_CMD("gfx.op.rect 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
    
    for (u4As = 0; u4As < 256; u4As++)
    {
        if (0 == u4As)
        {
            u4RectColor = ((0 << 24) | 0x00000000);
        }
        else
        {
            u4RectColor = ((u4As << 24) | 0x00FF0000);
        }
        _CLI_CMD("gfxemu.color %u", u4RectColor);
        _CLI_CMD("gfx.op.rect %u 20 2 80", (20 + (u4As * 2)));
    }

    for (u4As = 0; u4As < 256; u4As++)
    {
        if (0 == u4As)
        {
            u4RectColor = ((0 << 24) | 0x00000000);
        }
        else
        {
            u4RectColor = ((u4As << 24) | 0x0000FF00);
        }
        _CLI_CMD("gfxemu.color %u", u4RectColor);
        _CLI_CMD("gfx.op.rect %u 120 2 80", (20 + (u4As * 2)));
    }

    for (u4As = 0; u4As < 256; u4As++)
    {
        if (0 == u4As)
        {
            u4RectColor = ((0 << 24) | 0x00000000);
        }
        else
        {
            u4RectColor = ((u4As << 24) | 0x000000FF);
        }
        _CLI_CMD("gfxemu.color %u", u4RectColor);
        _CLI_CMD("gfx.op.rect %u 220 2 80", (20 + (u4As * 2)));
    }
    
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfx.op.compose 0 0 0 0 %u %u %u 4", 
        _u4ScreenWidth, _u4ScreenHeight, u4Ar);
    _CLI_CMD("gfx.flush");

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    if (_GfxBufferCompare(
            (UINT32 *)COMPARE_BUF, (UINT32 *)BUF_SCREEN, u4Size) == -1)
    {
        Printf("Alpha Composition Test 03 FAIL (SRC, As=0~255, Ar=%u) >_<\n", u4Ar);
        return -1;
    }

    Printf("Alpha Composition Test 03 OK (SRC, As=0~255, Ar=%u) ^_^\n", u4Ar);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdCompose4
 *  Alpha Composition Test 04 
 *  SRC_OVER => 1. Ar=1.0 & As=1.0
 *              2. Ar=0.0
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCompose4(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size;
    UINT32 u4Ar = 255;

    if (i4Argc < 2)
    {
        Printf("Arg: urAr (0 or 255)\n");
        return -1;
    }
    u4Ar = StrToInt(szArgv[1]);
    
    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    if (0 == u4Ar)
    {
        _GfxSetDefMode(BUF_TEMP, ADR_DST);
        _GfxSetDefMode(BUF_TEMP, ADR_SRC);
        _CLI_CMD("gfxemu.color 0x00FFFFFF");
        _CLI_CMD("gfx.op.rect 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);

        // Action
        //gfx.op.compose {sx} {sy} {dx} {dy} {w} {h} {ar} {mode}
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
        _CLI_CMD("gfx.flush");
        _GfxSetDefMode(BUF_TEMP, ADR_SRC);
        _CLI_CMD("gfx.op.compose 0 0 0 0 %u %u %u 7", 
            _u4ScreenWidth, _u4ScreenHeight, u4Ar);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)BUF_ADDR1, (UINT32 *)BUF_SCREEN, u4Size) == -1)
        {
            Printf("Alpha Composition Test 04 FAIL (SRC_OVER, Ar=%u) >_<\n", u4Ar);
            return -1;
        }
        Printf("Alpha Composition Test 04 OK (SRC_OVER, Ar=%u) ^_^\n", u4Ar);
    }
    else
    {
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        //gfx.op.compose {sx} {sy} {dx} {dy} {w} {h} {ar} {mode}
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.op.compose 0 0 0 0 %u %u %u 7", 
            _u4ScreenWidth, _u4ScreenHeight, u4Ar);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)BUF_ADDR1, (UINT32 *)BUF_SCREEN, u4Size) == -1)
        {
            Printf("Alpha Composition Test 04 FAIL (SRC_OVER, Ar=%u) >_<\n", u4Ar);
            return -1;
        }
        Printf("Alpha Composition Test 04 OK (SRC_OVER, Ar=%u) ^_^\n", u4Ar);
    }

    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdCompose5
 *  Alpha Composition Test 05 (SRC_OVER, Ar=1.0 & As=0.0)
 *  the result of (Ar=1.0 & As=0) equals to (Ar=0.0)
 *  special case: Ad=0, dst_color=0
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCompose5(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size, u4RectColor, u4Ad;
    
    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
        
    _GfxSetDefMode(BUF_TEMP, ADR_DST);
    _GfxSetDefMode(BUF_TEMP, ADR_SRC);
    _CLI_CMD("gfxemu.color 0x00000000");
    _CLI_CMD("gfx.op.rect 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);

    // Action
    //gfx.op.compose {sx} {sy} {dx} {dy} {w} {h} {ar} {mode}
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _CLI_CMD("gfxemu.color 0xFF000000");
    _CLI_CMD("gfx.op.rect 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
    
    for (u4Ad = 0; u4Ad < 256; u4Ad++)
    {
        if (0 == u4Ad)
        {
            u4RectColor = ((u4Ad << 24) | 0x00000000);
        }
        else
        {
            u4RectColor = ((u4Ad << 24) | 0x00FF0000);
        }
        _CLI_CMD("gfxemu.color %u", u4RectColor);
        _CLI_CMD("gfx.op.rect %u 20 2 80", (20 + (u4Ad * 2)));
    }

    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfx.op.bitblt 0 0 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);

    _GfxSetDefMode(BUF_TEMP, ADR_SRC);
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);    
    _CLI_CMD("gfx.op.compose 0 0 0 0 %u %u %u 7", 
        _u4ScreenWidth, _u4ScreenHeight, 255);
    _CLI_CMD("gfx.flush");

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    if (_GfxBufferCompare(
            (UINT32 *)COMPARE_BUF, (UINT32 *)BUF_SCREEN, u4Size) == -1)
    {
        Printf("Alpha Composition Test 05 FAIL (SRC_OVER, As=0~255) >_<\n");
        return -1;
    }

    Printf("Alpha Composition Test 05 OK (SRC_OVER, As=0~255) ^_^\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdCcnv1
 *  YCbCr to RGB Test 01 (420mb block bt601 video)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCcnv1(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Count = 1;
    CHAR *szColorConvertScript="gfx.set.cnvsrc 0x03000000 0x6000 0x0305A000 0x3000";

    if (i4Argc > 1)
    {
        i4Count = StrToInt(szArgv[1]);
        if (i4Count <= 0)
        {
            i4Count = 1;
        }
    }

    LOG(5, "YCbCr2RGB Test 1 ");
    
    if (i4Argc > 2)
    {
        szColorConvertScript = "gfx.set.cnvsrc 0x03000000 0x6000 0x0305A000 0x3000 field";
    }
    else
    {
        szColorConvertScript = "gfx.set.cnvsrc 0x03000000 0x6000 0x0305A000 0x3000";
    }

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);

    _CLI_CMD("gfx.set.cnvfmt 420mb block bt601 video");

    while (i4Count-- >= 0)
    {
        _CLI_CMD(szColorConvertScript);
        _CLI_CMD("gfx.op.colorcnv 0 0 720 480");
        _CLI_CMD("gfx.flush");
        _PAUSE(D_FRAME_DELAY);
        _CLI_CMD(szColorConvertScript);
        _CLI_CMD("gfx.op.colorcnv 0 0 720 480");
        _CLI_CMD("gfx.flush");
        _PAUSE(D_FRAME_DELAY);
        _CLI_CMD(szColorConvertScript);
        _CLI_CMD("gfx.op.colorcnv 0 0 720 480");
        _CLI_CMD("gfx.flush");
        _PAUSE(D_FRAME_DELAY);
        _CLI_CMD(szColorConvertScript);
        _CLI_CMD("gfx.op.colorcnv 0 0 720 480");
        _CLI_CMD("gfx.flush");
        _PAUSE(D_FRAME_DELAY);
        _CLI_CMD(szColorConvertScript);
        _CLI_CMD("gfx.op.colorcnv 0 0 720 480");
        _CLI_CMD("gfx.flush");
        _PAUSE(D_FRAME_DELAY);
        _CLI_CMD(szColorConvertScript);
        _CLI_CMD("gfx.op.colorcnv 0 0 720 480");
        _CLI_CMD("gfx.flush");
        _PAUSE(D_FRAME_DELAY);
        _CLI_CMD(szColorConvertScript);
        _CLI_CMD("gfx.op.colorcnv 0 0 720 480");
        _CLI_CMD("gfx.flush");
        _PAUSE(D_FRAME_DELAY);
    }

    Printf("... OK !\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdCcnv2
 *  YCbCr to RGB Test 02 (420 linear scan)
 *  Compare SW linear with HW linear
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCcnv2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4Choice = 0, i4PicCount = 1, i;
    CHAR *szColorConvertScript="gfx.set.cnvfmt 420lin block bt601 video";
    UINT32 u4LumaBase = BUF_LUMA;
    UINT32 u4ChromaBase = BUF_CHROMA;
    UINT32 u4DstLumaBase = BUF_DST_LUMA;
    UINT32 u4DstChromaBase = BUF_DST_CHROMA;
    UINT32 u4LumaPtr, u4ChromaPtr;
    UINT32 u4LumaSize = 0x5A000;    // 768 x 480
    UINT32 u4ChromaSize = 0x2D000;  // 768 x 240

    if (i4Argc > 1)
    {
        i4Choice = StrToInt(szArgv[1]);
    }

    Printf("YCbCr2RGB Test 2 ");
    
    if (i4Choice == 0)
    {
        szColorConvertScript = "gfx.set.cnvfmt 420lin block bt601 video";
        Printf("[420 Linear, BT601, Video]\n");
    }
    else if (i4Choice == 1)
    {
        szColorConvertScript = "gfx.set.cnvfmt 420lin block bt601 computer";
        Printf("[420 Linear, BT601, Computer]\n");
    }
    else if (i4Choice == 2)
    {
        szColorConvertScript = "gfx.set.cnvfmt 420lin block bt709 video";
        Printf("[420 Linear, BT709, Video]\n");
    }
    else if (i4Choice == 3)
    {
        szColorConvertScript = "gfx.set.cnvfmt 420lin block bt709 computer";
        Printf("[420 Linear, BT709, Computer]\n");
    }

    for (i = 0; i < i4PicCount; i++)
    {
        u4LumaPtr = u4LumaBase + (i * (u4LumaSize + u4ChromaSize));
        u4ChromaPtr = u4ChromaBase + (i * (u4LumaSize + u4ChromaSize));

        // for Y data
        GFX_SwBlock2Linear((UINT8 *)u4LumaPtr, (UINT8 *)u4DstLumaBase, 768, 480, 64, 32);

        // for CbCr data
        GFX_SwBlock2Linear((UINT8 *)u4ChromaPtr, (UINT8 *)u4DstChromaBase, 768, 240, 64, 16);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        //_GFX_SetVFull(1);
        _CLI_CMD(szColorConvertScript);
        _CLI_CMD("gfx.set.cnvsrc %u 768 %u 768", u4DstLumaBase, u4DstChromaBase);
        _CLI_CMD("gfx.op.colorcnv 0 0 720 480");
        
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        //_GFX_SetVFull(1);
        _CLI_CMD(szColorConvertScript);
        
        _CLI_CMD("gfx.set.cnvsrc %u 768 %u 768", u4DstLumaBase, u4DstChromaBase);
        _CLI_CMD("gfx.op.colorcnv 0 0 720 480");

        _CLI_CMD("gfx.flush");
        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;

        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("YCbCr2RGB Test 2 Fail\n");
            return -1;
        }

    }
    Printf("... OK !\n");
    
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdCcnv3
 *  YCbCr to RGB Test 03
 *  Compare HW swap with HW block
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCcnv3(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4Choice = 0;
    CHAR *szColorConvertScript1= "gfx.set.cnvfmt 420mb swap bt601 video";
    CHAR *szColorConvertScript2= "gfx.set.cnvfmt 420mb block bt601 video";
    
    if (i4Argc > 1)
    {
        i4Choice = StrToInt(szArgv[1]);
    }

    Printf("YCbCr2RGB Test 3 ");
    
    if (i4Choice == 0)
    {
        szColorConvertScript1 = "gfx.set.cnvfmt 420mb swap bt601 video";
        szColorConvertScript2 = "gfx.set.cnvfmt 420mb block bt601 video";
        Printf("[420MB Swap, BT601, Video]\n");
    }
    else if (i4Choice == 1)
    {
        szColorConvertScript1 = "gfx.set.cnvfmt 420mb swap bt601 computer";
        szColorConvertScript2 = "gfx.set.cnvfmt 420mb block bt601 computer";
        Printf("[420MB Swap, BT601, Computer]\n");
    }
    else if (i4Choice == 2)
    {
        szColorConvertScript1 = "gfx.set.cnvfmt 420mb swap bt709 video";
        szColorConvertScript2 = "gfx.set.cnvfmt 420mb block bt709 video";
        Printf("[420MB Swap, BT709, Video]\n");
    }
    else if (i4Choice == 3)
    {
        szColorConvertScript1 = "gfx.set.cnvfmt 420mb swap bt709 computer";
        szColorConvertScript2 = "gfx.set.cnvfmt 420mb block bt709 computer";
        Printf("[420MB Swap, BT709, Computer]\n");
    }
    
    // for Y data
    GFX_SwBlock2Swap((UINT8 *)BUF_LUMA, (UINT8 *)BUF_DST_LUMA, 768, 480, 64, 32);

    // for CbCr data
    GFX_SwBlock2Swap((UINT8 *)BUF_CHROMA, (UINT8 *)BUF_DST_CHROMA, 768, 240, 64, 16);
    
    // HW1
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
        
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    _CLI_CMD(szColorConvertScript1);
    _CLI_CMD("gfx.set.cnvsrc %u 0x6000 %u 0x3000",
            BUF_DST_LUMA, BUF_DST_CHROMA);
    _CLI_CMD("gfx.op.colorcnv 0 0 720 480");
    _CLI_CMD("gfx.flush");

    // HW2
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    _CLI_CMD(szColorConvertScript2);
    _CLI_CMD("gfx.set.cnvsrc %u 0x6000 %u 0x3000",
            BUF_LUMA, BUF_CHROMA);
    _CLI_CMD("gfx.op.colorcnv 0 0 720 480");
    _CLI_CMD("gfx.flush");

    // Compare HW1 and HW2
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    return _GfxBufferCompare(
                (UINT32 *)COMPARE_BUF,
                (UINT32 *)BUF_SCREEN,
                u4Size);
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdCcnv4
 *  YCbCr to RGB Test 04
 *  Compare HW block with HW linear
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCcnv4(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4Choice = 0;
    CHAR *szColorConvertScript1="gfx.set.cnvfmt 420mb block bt601 video";
    CHAR *szColorConvertScript2= "gfx.set.cnvfmt 420lin block bt601 video";
    
    if (i4Argc > 1)
    {
        i4Choice = StrToInt(szArgv[1]);
    }

    if (i4Choice == 0)
    {
        szColorConvertScript1 = "gfx.set.cnvfmt 420mb block bt601 video";
        szColorConvertScript2 = "gfx.set.cnvfmt 420lin block bt601 video";
        Printf("[420MB Block, BT601, Video]\n");
    }
    else if (i4Choice == 1)
    {
        szColorConvertScript1 = "gfx.set.cnvfmt 420mb block bt601 computer";
        szColorConvertScript2 = "gfx.set.cnvfmt 420lin block bt601 computer";
        Printf("[420MB Block, BT601, Computer]\n");
    }
    else if (i4Choice == 2)
    {
        szColorConvertScript1 = "gfx.set.cnvfmt 420mb block bt709 video";
        szColorConvertScript2 = "gfx.set.cnvfmt 420lin block bt709 video";
        Printf("[420MB Block, BT709, Video]\n");
    }
    else if (i4Choice == 3)
    {
        szColorConvertScript1 = "gfx.set.cnvfmt 420mb block bt709 computer";
        szColorConvertScript2 = "gfx.set.cnvfmt 420lin block bt709 computer";
        Printf("[420MB Block, BT709, Computer]\n");
    }
    
    // for Y data
    GFX_SwBlock2Linear((UINT8 *)BUF_LUMA, (UINT8 *)BUF_DST_LUMA, 768, 480, 64, 32);

    // for CbCr data
    GFX_SwBlock2Linear((UINT8 *)BUF_CHROMA, (UINT8 *)BUF_DST_CHROMA, 768, 240, 64, 16);
    
    // HW1
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
        
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action MB-Block mode
    _CLI_CMD(szColorConvertScript1);
    _CLI_CMD("gfx.set.cnvsrc %u 0x6000 %u 0x3000",
            BUF_LUMA, BUF_CHROMA);
    _CLI_CMD("gfx.op.colorcnv 0 0 720 480");
    _CLI_CMD("gfx.flush");

    // HW2
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action LIN-Linear mode
    _CLI_CMD(szColorConvertScript2);
    _CLI_CMD("gfx.set.cnvsrc %u 768 %u 768",
            BUF_DST_LUMA, BUF_DST_CHROMA); // Note: pitch
    _CLI_CMD("gfx.op.colorcnv 0 0 720 480");
    _CLI_CMD("gfx.flush");

    // Compare HW1 and HW2
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    Printf("YCbCr2RGB Test 04 ");
    return _GfxBufferCompare(
                (UINT32 *)COMPARE_BUF,
                (UINT32 *)BUF_SCREEN,
                u4Size);
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdCcnv5
 *  YCbCr to RGB Test 05
 *  Compare HW mergetop with HW block
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCcnv5(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4Choice = 0;
    CHAR *szColorConvertScript1= "gfx.set.cnvfmt 420mb mergetop bt601 video";
    CHAR *szColorConvertScript2= "gfx.set.cnvfmt 420mb block bt601 video";
    
    if (i4Argc > 1)
    {
        i4Choice = StrToInt(szArgv[1]);
    }

    if (i4Choice == 0)
    {
        szColorConvertScript1 = "gfx.set.cnvfmt 420mb mergetop bt601 video";
        szColorConvertScript2 = "gfx.set.cnvfmt 420mb block bt601 video";
        Printf("[420MB MergeTop, BT601, Video]\n");
    }
    else if (i4Choice == 1)
    {
        szColorConvertScript1 = "gfx.set.cnvfmt 420mb mergetop bt601 computer";
        szColorConvertScript2 = "gfx.set.cnvfmt 420mb block bt601 computer";
        Printf("[420MB MergeTop, BT601, Computer]\n");
    }
    else if (i4Choice == 2)
    {
        szColorConvertScript1 = "gfx.set.cnvfmt 420mb mergetop bt709 video";
        szColorConvertScript2 = "gfx.set.cnvfmt 420mb block bt709 video";
        Printf("[420MB MergeTop, BT709, Video]\n");
    }
    else if (i4Choice == 3)
    {
        szColorConvertScript1 = "gfx.set.cnvfmt 420mb mergetop bt709 computer";
        szColorConvertScript2 = "gfx.set.cnvfmt 420mb block bt709 computer";
        Printf("[420MB MergeTop, BT709, Computer]\n");
    }
    
    // for Y data
    GFX_SwBlock2Mergetop((UINT8 *)BUF_LUMA, (UINT8 *)BUF_DST_LUMA, 768, 480, 64, 32);

    // for CbCr data
    GFX_SwBlock2Mergetop((UINT8 *)BUF_CHROMA, (UINT8 *)BUF_DST_CHROMA, 768, 240, 64, 16);
    
    // HW1
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
        
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    _CLI_CMD(szColorConvertScript1);
    _CLI_CMD("gfx.set.cnvsrc %u 0x6000 %u 0x3000",
            BUF_DST_LUMA, BUF_DST_CHROMA);
    _CLI_CMD("gfx.op.colorcnv 0 0 720 480");
    _CLI_CMD("gfx.flush");


    // HW2
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    // Action
    _CLI_CMD(szColorConvertScript2);
    _CLI_CMD("gfx.set.cnvsrc %u 0x6000 %u 0x3000",
            BUF_LUMA, BUF_CHROMA);
    _CLI_CMD("gfx.op.colorcnv 0 0 720 480");
    _CLI_CMD("gfx.flush");

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    Printf("YCbCr2RGB Test 05 ");
    return _GfxBufferCompare(
                (UINT32 *)COMPARE_BUF,
                (UINT32 *)BUF_SCREEN,
                u4Size);
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdCcnv6 (only for MT5382)
 *  YCbCr to RGB Test 06 (422 linear scan)
 *  Compare SW linear with HW linear
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCcnv6(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4Choice = 0, i4PicCount = 1, i;
    CHAR *szColorConvertScript="gfx.set.cnvfmt 422lin block bt601 video";
    UINT32 u4LumaBase = BUF_LUMA;
    UINT32 u4ChromaBase = BUF_CHROMA;
    UINT32 u4DstLumaBase = BUF_DST_LUMA;
    UINT32 u4DstChromaBase = BUF_DST_CHROMA;
    UINT32 u4LumaPtr, u4ChromaPtr;
    UINT32 u4LumaSize = 0x5A000;    // 768 x 480
    UINT32 u4ChromaSize = 0x5A000;  // 768 x 480

    if (i4Argc > 1)
    {
        i4Choice = StrToInt(szArgv[1]);
    }

    Printf("YCbCr2RGB Test 6 (only for MT5382) ");
    
    if (i4Choice == 0)
    {
        szColorConvertScript = "gfx.set.cnvfmt 422lin block bt601 video";
        Printf("[422 Linear, BT601, Video]\n");
    }
    else if (i4Choice == 1)
    {
        szColorConvertScript = "gfx.set.cnvfmt 422lin block bt601 computer";
        Printf("[422 Linear, BT601, Computer]\n");
    }
    else if (i4Choice == 2)
    {
        szColorConvertScript = "gfx.set.cnvfmt 422lin block bt709 video";
        Printf("[422 Linear, BT709, Video]\n");
    }
    else if (i4Choice == 3)
    {
        szColorConvertScript = "gfx.set.cnvfmt 422lin block bt709 computer";
        Printf("[422 Linear, BT709, Computer]\n");
    }
        
    for (i = 0; i < i4PicCount; i++)
    {
        u4LumaPtr = u4LumaBase + (i * (u4LumaSize + u4ChromaSize));
        u4ChromaPtr = u4ChromaBase + (i * (u4LumaSize + u4ChromaSize));
            
        // for Y data
        GFX_SwBlock2Linear((UINT8 *)u4LumaPtr, (UINT8 *)u4DstLumaBase, 768, 480, 64, 32);

        // for CbCr data
        GFX_SwBlock2Linear((UINT8 *)u4ChromaPtr, (UINT8 *)u4DstChromaBase, 768, 480, 64, 32);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _CLI_CMD(szColorConvertScript);
        _CLI_CMD("gfx.set.cnvsrc %u 768 %u 768", u4DstLumaBase, u4DstChromaBase);
        _CLI_CMD("gfx.op.colorcnv 0 0 720 480");
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _CLI_CMD(szColorConvertScript);
        _CLI_CMD("gfx.set.cnvsrc %u 768 %u 768", u4DstLumaBase, u4DstChromaBase);
        _CLI_CMD("gfx.op.colorcnv 0 0 720 480");
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("YCbCr2RGB Test 6 Fail\n");
            return -1;
        }
    }
    Printf("... OK !\n");
    return 0;
}

#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)||defined(CC_MT5398)
//-------------------------------------------------------------------------
/** _GfxEmuCmdCcnv7 (only for MT5368)
 *  YCbCr to RGB24 Test 07 (422 linear scan)
 *  Compare Golden data with HW linear
 */
//-------------------------------------------------------------------------

static INT32 _GfxEmuCmdCcnv7(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32  i4PicCount = 1, i;
    CHAR *szColorConvertScript = "gfx.set.cnvfmt 422lin block bt601 video";
    CHAR *szColorConvertScript2 = "gfx.set.cnvfmt 420lin block bt601 video";
    UINT32 u4LumaBase = BUF_LUMA;
    UINT32 u4ChromaBase = BUF_CHROMA;
    //UINT32 u4DstLumaBase = BUF_DST_LUMA;
    // UINT32 u4DstChromaBase = BUF_DST_CHROMA;
    // UINT32 u4LumaPtr, u4ChromaPtr;
//    UINT32 u4LumaSize = 0x5A000;    // 768 x 480
//    UINT32 u4ChromaSize = 0x5A000;  // 768 x 480

    if (i4Argc > 1)
    {
        i4PicCount = StrToInt(szArgv[1]);
    }
    Printf("YCbCrToRgb24Bit   test ...... \n");
    Printf("YCbCr2RGB Test 7  (only for MT5368 YcbCrTo24BitRGB) ");

    for (i = 0; i < i4PicCount; i++)
    {
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\0322_y.raw\" 0x%08x /ny\n", BUF_LUMA);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\0322_y.raw\" 0x%08x /ny",  BUF_LUMA));
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\0322_c.raw\" 0x%08x /ny\n", BUF_CHROMA);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\0322_c.raw\" 0x%08x /ny", BUF_CHROMA));

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _CLI_CMD(szColorConvertScript);
        _GFX_SetDstWrRgb24Bit(1);
        //_CLI_CMD("gfx.set.cnvsrc %u 768 %u 768", u4DstLumaBase, u4DstChromaBase);
        _CLI_CMD("gfx.set.cnvsrc %u 112 %u 112", u4LumaBase, u4ChromaBase);
        //_CLI_CMD("gfx.op.colorcnv 0 0 720 480");
        _CLI_CMD("gfx.op.colorcnv 0 0 112 120");

        _CLI_CMD("gfx.flush");
        _GFX_SetDstWrRgb24Bit(0);

        // Compare HW and golden
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\rgb24b_112x120_golden.raw\" 0x%08x /ny\n", COMPARE_BUF);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\rgb24b_112x120_golden.raw\" 0x%08x /ny",
                            COMPARE_BUF));

        //   u4Size = _u4ScreenWidth * _u4ScreenHeight;
        u4Size = 112 * 120 * 3 / 4;
        if (_GfxBufferCompare(
                    (UINT32 *)COMPARE_BUF,
                    (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("YCbCr2RGB Test 7 112x120 raw data  Fail\n");
            return -1;
        }

        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\luma_768x480_linear.bin\" 0x%08x /ny\n", BUF_LUMA);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\luma_768x480_linear.bin\" 0x%08x /ny",	BUF_LUMA));
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\chroma_768x240_linear.bin\" 0x%08x /ny\n", BUF_CHROMA);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\chroma_768x240_linear.bin\" 0x%08x /ny", BUF_CHROMA));

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _CLI_CMD(szColorConvertScript2);
        _GFX_SetDstWrRgb24Bit(1);
        //_CLI_CMD("gfx.set.cnvsrc %u 768 %u 768", u4DstLumaBase, u4DstChromaBase);
        _CLI_CMD("gfx.set.cnvsrc %u 768 %u 768", u4LumaBase, u4ChromaBase);
        //_CLI_CMD("gfx.op.colorcnv 0 0 720 480");
        _CLI_CMD("gfx.op.colorcnv 0 0 720 480");

        _CLI_CMD("gfx.flush");
        _GFX_SetDstWrRgb24Bit(0);

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");
        // Compare HW and SW
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\rgb24b_720x480_golden.raw\" 0x%08x /ny\n", COMPARE_BUF);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\rgb24b_720x480_golden.raw\" 0x%08x /ny",
                            COMPARE_BUF));

        u4Size = _u4ScreenWidth * _u4ScreenHeight * 3 / 4;
        //u4Size = 112 * 120 * 3 / 4;
        if (_GfxBufferCompare(
                    (UINT32 *)COMPARE_BUF,
                    (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("YCbCr2RGB Test 7 720x480 raw data  Fail\n");
            return -1;
        }
    }

    Printf("YCbCr2RGB Test 7 ... OK !\n");
    Printf("YCbCrToRgb24Bit  Test OK\n");
    return 0;
}


static INT32 _GfxEmuCmdReadArgbOrder(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0, u4Width, u4Height, u4DstPitch;
    INT32  i4PicCount = 1, i;
    CHAR *szCharCM;
    CHAR *szDstCM;
    if (i4Argc > 1)
    {
        i4PicCount = StrToInt(szArgv[1]);
    }
    Printf("ARGB read order   test ...... \n");
    for (i = 0; i < i4PicCount; i++)
    {
        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");
        _GfxSetDefMode(BUF_ADDR1, ADR_DST);
        _GfxSetDefMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfxemu.cls");
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\argb_order\\old_test.raw\" 0x%08x /ny\n", BUF_ADDR1);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\argb_order\\old_test.raw\" 0x%08x /ny", BUF_ADDR1));

        // Action
        _GFX_SetWriteArgbOrder( A_ORDER, R_ORDER, G_ORDER, B_ORDER);
        _GFX_SetReadArgbOrder(TRUE, A_ORDER, B_ORDER, G_ORDER, R_ORDER);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
        _GFX_SetReadArgbOrder(TRUE, A_ORDER, R_ORDER, G_ORDER, B_ORDER);

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");
        // Compare HW and golden HW
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\argb_order\\rgb_order.raw\" 0x%08x /ny\n", COMPARE_BUF);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\argb_order\\rgb_order.raw\" 0x%08x /ny", COMPARE_BUF));

        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                    (UINT32 *)COMPARE_BUF,
                    (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {

            Printf("argb8888 direct color mode argb order  Test  Faill\n");
            return -1;
        }
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");
        _GfxSetDefMode(BUF_ADDR1, ADR_DST);
        _GfxSetDefMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfxemu.cls");
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\argb_order\\rgb8_704x480.raw\" 0x%08x /ny\n", BUF_ADDR1);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\argb_order\\rgb8_704x480.raw\" 0x%08x /ny", BUF_ADDR1));

        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\argb_order\\rgb8_704x480_palette.raw\" 0x%08x /ny\n", BUF_TEMP);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\argb_order\\rgb8_704x480_palette.raw\" 0x%08x /ny", BUF_TEMP));
        // Action
        _GFX_SetReadArgbOrder(TRUE, A_ORDER, B_ORDER, G_ORDER, R_ORDER);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        szCharCM = "8bit";
        szDstCM = "argb8888";
        u4Width = 704;
        u4Height = 480;
        u4DstPitch = u4Width * 4;

        // gfx.set.idx2dirsrc {buff_base_address} {char_cm}
        _CLI_CMD("gfx.set.idx2dirsrc 0x%08x %s", BUF_ADDR1, szCharCM);

        // gfx.set.dst {buff_base_address} {color_mode} {pitch_in_bytes}
        _CLI_CMD("gfx.set.dst 0x%08x %s %u", BUF_SCREEN, szDstCM, u4DstPitch);

        // gfx.set.idx2diropt {palet_addr} {msb_left} {start_byte_align}
        _CLI_CMD("gfx.set.idx2diropt 0x%08x %u %u",
                 BUF_TEMP, 0, 0);

        // gfx.op.idx2dirblt {dx} {dy} {w} {h}
        _CLI_CMD("gfx.op.idx2dirblt %u %u %u %u", 0, 0, u4Width, u4Height);

        _CLI_CMD("gfx.flush");
        _GFX_SetReadArgbOrder(TRUE, A_ORDER, R_ORDER, G_ORDER, B_ORDER);

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");
        // Compare HW and golden HW
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\argb_order\\rgb_idxtodir_order.raw\" 0x%08x /ny\n", COMPARE_BUF);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\argb_order\\rgb_idxtodir_order.raw\" 0x%08x /ny", COMPARE_BUF));

        u4Size = u4Width * u4Height;
        if (_GfxBufferCompare(
                    (UINT32 *)COMPARE_BUF,
                    (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {

            Printf(" 8bit index color mode argb order  Test  Faill\n");
            return -1;
        }

    }

    Printf("ARGB read order   test OK ..... !\n");
    return 0;
}

static INT32 _GfxEmuCmdWriteArgbOrder(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32  i4PicCount = 1, i;
    if (i4Argc > 1)
    {
        i4PicCount = StrToInt(szArgv[1]);
    }
    Printf("ARGB write order   test ...... \n");
    for (i = 0; i < i4PicCount; i++)
    {
        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");
        _GfxSetDefMode(BUF_ADDR1, ADR_DST);
        _GfxSetDefMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfxemu.cls");


        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\argb_order\\rgb_order.raw\" 0x%08x /ny\n", BUF_ADDR1);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\argb_order\\rgb_order.raw\" 0x%08x /ny", BUF_ADDR1));
        // Action
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_ADDR1, ADR_SRC);

        _GFX_SetReadArgbOrder(TRUE, A_ORDER, R_ORDER, G_ORDER, B_ORDER);
        _GFX_SetWriteArgbOrder( A_ORDER, B_ORDER, G_ORDER, R_ORDER);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");
        _GFX_SetWriteArgbOrder( A_ORDER, R_ORDER, G_ORDER, B_ORDER);

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");
        // Compare HW and golden HW

        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\argb_order\\old_test.raw\" 0x%08x /ny\n", COMPARE_BUF);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\argb_order\\old_test.raw\" 0x%08x /ny", COMPARE_BUF));
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                    (UINT32 *)COMPARE_BUF,
                    (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {

            Printf("argb8888 direct color mode argb order  Test  Faill\n");
            return -1;
        }

    }

    Printf("ARGB read order   test OK ..... !\n");
    return 0;
}



#endif


//-------------------------------------------------------------------------
/** _GfxEmuCmdStrBlt1
 *  Stretch BitBlt Test 01
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdStrBlt1(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Sx=0, u4Sy=0, u4Sw, u4Sh, u4Dx=0, u4Dy=0, u4Dw, u4Dh;
    INT32 i4Loop, i4CountNum = 10000;
    UINT32 au4Buf[3];
    au4Buf[0] = BUF_ADDR1;
    au4Buf[1] = BUF_ADDR2;
    au4Buf[2] = BUF_ADDR3;

    if (i4Argc > 1)
    {
        i4CountNum = StrToInt(szArgv[1]);
    }
        
    // 1. prepare source bmp
    // 2. select a source size (w,h)
    // 3. use constraint (1/128 ~ x128) to get dest size (w,h)
    // 4. use source size to get source pos (x,y)
    // 5. use dest size to get dest pos (x,y)
    // 6. stretch to buf4, alpha blend to osd
    // 7. every 1000 stretch, blt buf1 to osd

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
    
    // init
    _CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.set.alpha 128");
    _CLI_CMD("gfxemu.black");

    // do many in loop
    for (i4Loop = 0; i4Loop < i4CountNum; i4Loop++)
    {
        UINT32 u4Max, u4Min;

        // select source w,h
        u4Sw = (((UINT32)rand()) % (MAX_SRC_WIDTH - MIN_SRC_WIDTH)) + MIN_SRC_WIDTH;
        u4Sh = (((UINT32)rand()) % (MAX_SRC_HEIGHT - MIN_SRC_HEIGHT)) + MIN_SRC_HEIGHT;
        if (u4Sw == 0)
        {
            u4Sw++;
        }
        if (u4Sh == 0)
        {
            u4Sh++;
        }

        // select dest w
        u4Max = ((u4Sw * D_SCALE_UP) > MAX_DST_WIDTH) ? MAX_DST_WIDTH : (u4Sw * D_X_SCALE_UP);
        u4Min = ((u4Sw / D_SCALE_DN) < MIN_DST_WIDTH) ? MIN_DST_WIDTH : (u4Sw / D_X_SCALE_DN);
        u4Dw = (((UINT32)rand()) % (u4Max - u4Min + 1)) + u4Min;

        // select dest h
        u4Max = ((u4Sh * D_SCALE_UP) > MAX_DST_HEIGHT) ? MAX_DST_HEIGHT : (u4Sh * D_Y_SCALE_UP);
        u4Min = ((u4Sh / D_SCALE_DN) < MIN_DST_HEIGHT) ? MIN_DST_HEIGHT : (u4Sh / D_Y_SCALE_DN);
        u4Dh = (((UINT32)rand()) % (u4Max - u4Min + 1)) + u4Min;

        // select source x,y
        if (u4Sw == MAX_SRC_WIDTH) u4Sw--;
        if (u4Sh == MAX_SRC_HEIGHT) u4Sh--;
        u4Sx = ((UINT32)rand()) % (MAX_SRC_WIDTH - u4Sw);
        u4Sy = ((UINT32)rand()) % (MAX_SRC_HEIGHT - u4Sh);

        // select dest x,y
        if (u4Dw == MAX_DST_WIDTH) u4Dw--;
        if (u4Dh == MAX_DST_HEIGHT) u4Dh--;
        u4Dx = (((UINT32)rand()) % (MAX_DST_WIDTH - u4Dw)) + 2;
        u4Dy = (((UINT32)rand()) % (MAX_DST_HEIGHT - u4Dh)) + 2;

        _GfxSetSdMode(au4Buf[((UINT32)rand()) % 3], ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        
        // draw a frame
        if (i4Argc == 1)
        {
            // vline
            _CLI_CMD("gfx.op.vline %d %d %d", u4Dx - 1, u4Dy - 1, u4Dh + 2);
            _CLI_CMD("gfx.op.vline %d %d %d", u4Dx + u4Dw + 1, u4Dy - 1, u4Dh + 2);
        }

        _CLI_CMD("gfx.op.stretchblt %d %d %d %d %d %d %d %d",
                u4Sx, u4Sy, u4Sw, u4Sh,
                u4Dx, u4Dy, u4Dw, u4Dh);
        _CLI_CMD("gfx.flush");
    }
    Printf("Stretch BitBlt Test 01 (%d times) OK !!", i4CountNum);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdStrBlt2
 *  Stretch BitBlt Test 02 (random, only run hw)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdStrBlt2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Sx, u4Sy, u4Sw, u4Sh, u4Dx, u4Dy, u4Dw, u4Dh;
    INT32 i4Loop, i4CountNum = 10000;

    UINT32 au4Buf[3];
    au4Buf[0] = BUF_ADDR1;
    au4Buf[1] = BUF_ADDR2;
    au4Buf[2] = BUF_ADDR3;

    if (i4Argc > 1)
    {
        i4CountNum = StrToInt(szArgv[1]);
    }

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
    
    // init
    _CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.set.alpha 128");
    _CLI_CMD("gfxemu.black");

    // do many in loop
    for (i4Loop = 0; i4Loop < i4CountNum; i4Loop++)
    {
        UINT32 u4Max, u4Min;
        INT32 i4ColorIndex = 0;

        // select source w,h
        u4Sw = (((UINT32)rand()) % (MAX_SRC_WIDTH - MIN_SRC_WIDTH - 1)) + MIN_SRC_WIDTH;
        u4Sh = (((UINT32)rand()) % (MAX_SRC_HEIGHT - MIN_SRC_HEIGHT - 1)) + MIN_SRC_HEIGHT;
        if (u4Sw == 0)
        {
            u4Sw++;
        }
        if (u4Sh == 0)
        {
            u4Sh++;
        }

        // select dest w
        u4Max = ((u4Sw * D_SCALE_UP) > (MAX_DST_WIDTH - 1)) ? (MAX_DST_WIDTH - 1) : (u4Sw * D_X_SCALE_UP);
        u4Min = ((u4Sw / D_SCALE_DN) < MIN_DST_WIDTH) ? MIN_DST_WIDTH : (u4Sw / D_X_SCALE_DN);
        u4Dw = (((UINT32)rand()) % (u4Max - u4Min + 1)) + u4Min;

        // select dest h
        u4Max = ((u4Sh * D_SCALE_UP) > (MAX_DST_HEIGHT - 1)) ? (MAX_DST_HEIGHT - 1) : (u4Sh * D_Y_SCALE_UP);
        u4Min = ((u4Sh / D_SCALE_DN) < MIN_DST_HEIGHT) ? MIN_DST_HEIGHT : (u4Sh / D_Y_SCALE_DN);
        u4Dh = (((UINT32)rand()) % (u4Max - u4Min + 1)) + u4Min;

        // select source x,y
        u4Sx = ((UINT32)rand()) % (MAX_SRC_WIDTH - u4Sw);
        u4Sy = ((UINT32)rand()) % (MAX_SRC_HEIGHT - u4Sh);

        // select dest x,y
        u4Dx = (((UINT32)rand()) % (MAX_DST_WIDTH - u4Dw)) + 2;
        u4Dy = (((UINT32)rand()) % (MAX_DST_HEIGHT - u4Dh)) + 2;

        _GfxSetSdMode(au4Buf[((UINT32)rand()) % 3], ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.op.stretchblt %d %d %d %d %d %d %d %d",
                u4Sx, u4Sy, u4Sw, u4Sh,
                u4Dx, u4Dy, u4Dw, u4Dh);

        // draw a frame
        i4ColorIndex = ((UINT32)rand()) % 147;
        if (i4Argc == 1)
        {
            _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColorIndex].u4Color);
            _CLI_CMD("gfx.op.vline %d %d %d", u4Dx - 1, u4Dy - 1, u4Dh + 2);
            _CLI_CMD("gfx.op.vline %d %d %d", u4Dx + u4Dw + 1, u4Dy - 1, u4Dh + 2);
        }
        _CLI_CMD("gfx.flush");
    }
    Printf("Stretch BitBlt Test 02 (%d times) OK !!", i4CountNum);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdAlphaMap1
 *  Alpha-Map Test 01 (apply gradient alpha values)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdAlphaMap1(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    UINT32 u4Pitch;

    Printf(" Alpha-Map Test 01 (apply gradient alpha values) \n");    

    x_memset((UINT32*)BUF_SCREEN,0,_u4ScreenWidth * _u4ScreenHeight*4);
    x_memset((UINT32*)COMPARE_BUF,0,_u4ScreenWidth * _u4ScreenHeight*4);

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfxemu.cls");
        
        // 1. copy kodak14 to DST
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
    _CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.op.bitblt 0 0 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.flush");

        // 2. do AlphaMap
    #ifdef GFX_BURST_READ
        u4Pitch = GFX_BURST_READ_ALIGN(_u4ScreenWidth);
        LOG(0,"pitch : %d\n",u4Pitch);
    #else
        u4Pitch = _u4ScreenWidth;
    #endif
    _CLI_CMD("gfx.s.src 0x%08x RGB8 %d", BUF_ALPHAMAP, u4Pitch);
    _CLI_CMD("gfx.op.am 0 0 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.flush");

        // 3. set SRC to kodak16 and do DST_OVER
    _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
    _CLI_CMD("gfx.op.composeloop 0 0 0 0 %d %d 255 3", _u4ScreenWidth, _u4ScreenHeight);

    //_CLI_CMD("gfx.op.compose 0 0 0 0 %d %d 255 3", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.flush");


    // SW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");
        
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
    _CLI_CMD("gfxemu.cls");
        
        // 1. copy kodak14 to DST
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
    _CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.op.bitblt 0 0 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.flush");

        // 2. do AlphaMap
    _CLI_CMD("gfx.s.src 0x%08x RGB8 %d", BUF_ALPHAMAP, u4Pitch);
    _CLI_CMD("gfx.op.am 0 0 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.flush");

        // 3. set SRC to kodak16 and do DST_OVER
    _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
    //_CLI_CMD("gfx.op.compose 0 0 0 0 %d %d 255 3", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.op.composeloop 0 0 0 0 %d %d 255 3", _u4ScreenWidth, _u4ScreenHeight);
    _CLI_CMD("gfx.flush");

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    Printf("Alpha Map Test 01 ");
    return _GfxBufferCompare(
                (UINT32 *)COMPARE_BUF,
                (UINT32 *)(BUF_SCREEN),
                u4Size);
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdAlphaMap2
 *  Alpha-Map Test 02 (apply gradient alpha values)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdAlphaMap2   (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0, i = 0;
    INT32 i4Sx, i4Dx, i4Width, i4Height;
    UINT32 au4Buf[3];    
    UINT32 u4Pitch;
    
    Printf("Alpha Map Test 02 ");
    
    au4Buf[0] = BUF_ADDR1;
    au4Buf[1] = BUF_ADDR2;
    au4Buf[2] = BUF_ADDR3;
    
    i4Sx = 401;
    i4Height = _u4ScreenHeight;
    for (i4Width = 1; i4Width < 33; i4Width++)
    {
        Printf("%d loop : ", i++);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        /* 1. copy kodak17 to DST */
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetSdMode(au4Buf[ (i4Width % 3) ], ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
        _CLI_CMD("gfx.flush");
        
        /* 2. do AlphaMap: pixel width test */
        #ifdef GFX_BURST_READ
            u4Pitch = GFX_BURST_READ_ALIGN(_u4ScreenWidth);
            LOG(0,"pitch : %d\n",u4Pitch);
        #else
            u4Pitch = _u4ScreenWidth;
        #endif
        _CLI_CMD("gfx.s.src 0x%08x RGB8 %d", BUF_ALPHAMAP, u4Pitch);
        for (i4Dx = 0; i4Dx < _u4ScreenWidth; i4Dx += i4Width)
        {
            _CLI_CMD("gfx.op.am %d 0 %d 0 %d %d", i4Sx, i4Dx, i4Width, i4Height);
            _CLI_CMD("gfx.flush");
        }


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");
        
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        /* 1. copy kodak17 to DST */
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetSdMode(au4Buf[ (i4Width % 3) ], ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
        _CLI_CMD("gfx.flush");
        
        /* 2. do AlphaMap: pixel width test */
        _CLI_CMD("gfx.s.src 0x%08x RGB8 %d", BUF_ALPHAMAP, u4Pitch);
        for (i4Dx = 0; i4Dx < _u4ScreenWidth; i4Dx += i4Width)
        {
            _CLI_CMD("gfx.op.am %d 0 %d 0 %d %d", i4Sx, i4Dx, i4Width, i4Height);
            _CLI_CMD("gfx.flush");
        }

        /* Compare HW and SW */
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Alpha Map Test 02 Error ");
            Printf("Pixel Width = %d\n", i4Width);
            return -1;
        }
    }
    Printf("..... OK !!\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdAlphaMap3
 *  Alpha-Map Test 03 (apply random alpha values)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdAlphaMap3   (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4PreCount = 0;
    INT32 i4Sx, i4Sy, i4Dx, i4Dy, i4Width, i4Height;
    INT32 i4Count;
    INT32 i4CountNum = 4000;
    INT32 i4Error = 0;    
    UINT32 u4Pitch;
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        /* gen random values */
        i4Sx = ((INT32)rand()) % _u4ScreenWidth;
        i4Sy = ((INT32)rand()) % _u4ScreenHeight;
        i4Dx = ((INT32)rand()) % _u4ScreenWidth;
        i4Dy = ((INT32)rand()) % _u4ScreenHeight;
        i4Width = ((INT32)rand()) % _u4ScreenWidth;
        i4Height = ((INT32)rand()) % _u4ScreenHeight;

        /* check boundary */
        if ((i4Sx + i4Width) > _u4ScreenWidth)
        {
            i4Width = _u4ScreenWidth - i4Sx;
        }
            
        if ((i4Dx + i4Width) > _u4ScreenWidth)
        {
            i4Width = _u4ScreenWidth - i4Dx;
        }

        if ((i4Sy + i4Height) > _u4ScreenHeight)
        {
            i4Height = _u4ScreenHeight - i4Sy;
        }

        if ((i4Dy + i4Height) > _u4ScreenHeight)
        {
            i4Height = _u4ScreenHeight - i4Dy;
        }

        i4Width  = (i4Width  == 0) ? 1 : i4Width;
        i4Height = (i4Height == 0) ? 1 : i4Height;
           
        Printf("%d Run : ", i4Count);
        Printf("Sx = %d, Sy = %d, Dx = %d, Dy = %d, W = %d, H = %d\n",
            i4Sx, i4Sy, i4Dx, i4Dy, i4Width, i4Height);

        /* HW */
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

            /* 1. copy kodak17 to DST */
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
        _CLI_CMD("gfx.flush");
        
            /* 2. do AlphaMap*/
        #ifdef GFX_BURST_READ
            u4Pitch = GFX_BURST_READ_ALIGN(_u4ScreenWidth);
            LOG(0,"pitch : %d\n",u4Pitch);
        #else
            u4Pitch = _u4ScreenWidth;
        #endif           
        _CLI_CMD("gfx.s.src 0x%08x RGB8 %d", BUF_ALPHAMAP, u4Pitch);
        _CLI_CMD("gfx.op.am %d %d %d %d %d %d", i4Sx, i4Sy, i4Dx, i4Dy, i4Width, i4Height);
        _CLI_CMD("gfx.flush");
        

        /* SW */
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");
            
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

            /* 1. copy kodak17 to DST */
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
        _CLI_CMD("gfx.flush");
        
            /* 2. do AlphaMap*/
        _CLI_CMD("gfx.s.src 0x%08x RGB8 %d", BUF_ALPHAMAP, u4Pitch);
        _CLI_CMD("gfx.op.am %d %d %d %d %d %d", i4Sx, i4Sy, i4Dx, i4Dy, i4Width, i4Height);
        _CLI_CMD("gfx.flush");


        /* Compare HW and SW */
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Alpha Map Test 03 ");
            Printf("%d Run, Diff = %d, (Error = %d)\n",
                i4Count, (i4Count-i4PreCount), ++i4Error);
            i4PreCount = i4Count;

            return -1;
        }
    }
    Printf("Alpha Map Test 03 ");
    Printf("# of Error Run = %d, # of Total Run = %d\n", i4Error, i4CountNum);
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdAlphaMap4
 *  Alpha-Map Test 04 (random, only run hw)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdAlphaMap4(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Sx, i4Sy, i4Dx, i4Dy, i4Width, i4Height;
    INT32 i4Count;
    INT32 i4CountNum = 4000;
    UINT32 u4Pitch =0;
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    Printf("Alpha Map Test 04 (random, only run hw) ... \n");
    
    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        /* gen random values */
        i4Sx = ((INT32)rand()) % _u4ScreenWidth;
        i4Sy = ((INT32)rand()) % _u4ScreenHeight;
        i4Dx = ((INT32)rand()) % _u4ScreenWidth;
        i4Dy = ((INT32)rand()) % _u4ScreenHeight;
        i4Width = ((INT32)rand()) % _u4ScreenWidth;
        i4Height = ((INT32)rand()) % _u4ScreenHeight;

        /* check boundary */
        if ((i4Sx + i4Width) > _u4ScreenWidth)
        {
            i4Width = _u4ScreenWidth - i4Sx;
        }
            
        if ((i4Dx + i4Width) > _u4ScreenWidth)
        {
            i4Width = _u4ScreenWidth - i4Dx;
        }

        if ((i4Sy + i4Height) > _u4ScreenHeight)
        {
            i4Height = _u4ScreenHeight - i4Sy;
        }

        if ((i4Dy + i4Height) > _u4ScreenHeight)
        {
            i4Height = _u4ScreenHeight - i4Dy;
        }

        i4Width  = (i4Width  == 0) ? 1 : i4Width;
        i4Height = (i4Height == 0) ? 1 : i4Height;
           
        Printf("%d Run : ", i4Count);
        Printf("Sx = %d, Sy = %d, Dx = %d, Dy = %d, W = %d, H = %d\n",
            i4Sx, i4Sy, i4Dx, i4Dy, i4Width, i4Height);

            /* 1. copy kodak17 to DST */
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
        _CLI_CMD("gfx.flush");
        
            /* 2. do AlphaMap*/
        #ifdef GFX_BURST_READ
            u4Pitch = GFX_BURST_READ_ALIGN(_u4ScreenWidth);
            //LOG(0,"pitch : %d\n",u4Pitch);
        #else
            u4Pitch = _u4ScreenWidth;
        #endif

        _CLI_CMD("gfx.s.src 0x%08x RGB8 %d", BUF_ALPHAMAP, u4Pitch);
        _CLI_CMD("gfx.op.am %d %d %d %d %d %d", i4Sx, i4Sy, i4Dx, i4Dy, i4Width, i4Height);
    }
    _CLI_CMD("gfx.flush");
    Printf("Alpha Map Test 04 (random, only run hw) ... OK!!\n");
    Printf("# of Total Run = %d\n", i4CountNum);
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdRop1
 *  ROP Test 01
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdRop1(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4RopCode;
    INT32 i4Start = 4, i4CountNum = 15;

    Printf("ROP Test 01\n");

    x_memset((UINT32*)BUF_SCREEN,0,_u4ScreenWidth * _u4ScreenHeight*4);
    x_memset((UINT32*)COMPARE_BUF,0,_u4ScreenWidth * _u4ScreenHeight*4);
        
    if (i4Argc > 1)
    {
        i4Start = (INT32) StrToInt(szArgv[1]);
    }

    if (i4Argc > 2)
    {
        i4CountNum = (INT32) StrToInt(szArgv[2]);
    }

    for (i4RopCode = i4Start; i4RopCode <= i4CountNum; i4RopCode++)
    {
        Printf("Rop OpCode = %d (%s) : ", i4RopCode, _aszRopOpCode[i4RopCode]);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _CLI_CMD("gfx.set.ropcode %d", i4RopCode);
        _CLI_CMD("gfx.op.rop 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");

        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _CLI_CMD("gfx.set.ropcode %d", i4RopCode);
        _CLI_CMD("gfx.op.rop 0 0 0 0 720 480");
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("ROP Test 01 Error\n");
            return -1;
        }
    }

    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdRop2
 *  ROP Test 02 (random)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdRop2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4PreCount = 0;
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count, i4RopCode, i4PicSel;
    INT32 i4CountNum = 5000;
    INT32 i4Error = 0;
    INT32 i4TotalRopCodes = 16;

    Printf("ROP Test 02 ");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        i4PicSel  = ((UINT32)rand()) % 3;
        i4RopCode = ((UINT32)rand()) % i4TotalRopCodes;
        i4Left    = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right   = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top     = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom  = ((UINT32)rand()) % _u4ScreenHeight;

        i4Width   = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -1 * i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -1 * i4Height;
            i4Top = i4Bottom;
        }

        if (i4RopCode < 4)
        {
            i4RopCode = 4;
        }
        
        i4Width++;
        i4Height++;

        Printf("(%4d) : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d, RopCode = %d\n",
            i4Left, i4Top, i4Width, i4Height, i4RopCode);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d", 
            i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _CLI_CMD("gfx.set.ropcode %d", i4RopCode);
        _CLI_CMD("gfx.op.rop %d %d %d %d %d %d", 
            i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");
        
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d", 
            i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _CLI_CMD("gfx.set.ropcode %d", i4RopCode);
        _CLI_CMD("gfx.op.rop %d %d %d %d %d %d", 
            i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("ROP Test 02 Error ");
            Printf("(%4d) : Diff = %d, (Error = %d)\n",
                i4Count, (i4Count-i4PreCount), ++i4Error);
            i4PreCount = i4Count;
            return -1;
        }
    }
    Printf("# of Error Run = %d, # of Total Run = %d\n", i4Error, i4CountNum);
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdRop3
 *  ROP Test 03 (random, only run hw)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdRop3(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count, i4RopCode, i4PicSel;
    INT32 i4CountNum = 5000;
    INT32 i4TotalRopCodes = 16;

    Printf("ROP Test 03 (random, only run hw) ... \n");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        i4PicSel  = ((UINT32)rand()) % 3;
        i4RopCode = ((UINT32)rand()) % i4TotalRopCodes;
        i4Left    = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right   = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top     = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom  = ((UINT32)rand()) % _u4ScreenHeight;

        i4Width   = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -1 * i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -1 * i4Height;
            i4Top = i4Bottom;
        }

        if (i4RopCode < 4)
        {
            i4RopCode = 4;
        }
        
        i4Width++;
        i4Height++;

        Printf("(%4d) : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d, RopCode = %d\n",
            i4Left, i4Top, i4Width, i4Height, i4RopCode);

        // Action
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d", 
            i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _CLI_CMD("gfx.set.ropcode %d", i4RopCode);
        _CLI_CMD("gfx.op.rop %d %d %d %d %d %d", 
            i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
    }
    _CLI_CMD("gfx.flush");
    Printf("ROP Test 03 (random, only run hw) ... OK!!\n");
    Printf("# of Total Run = %d\n", i4CountNum);
    return 0;
}


// _GfxEmuCmdRop4 only test mutiply rop mode for mt5368
#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)||defined(CC_MT5398)
static INT32 _GfxEmuCmdRop4(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4PreCount = 0;
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count, i4RopCode;
    INT32 i4CountNum = 5000;
    INT32 i4Error = 0;

    Printf("ROP Test 02 ");

    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        i4RopCode = 2;
        i4Left    = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right   = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top     = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom  = ((UINT32)rand()) % _u4ScreenHeight;

        i4Width   = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -1 * i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -1 * i4Height;
            i4Top = i4Bottom;
        }

        i4Width++;
        i4Height++;

        Printf("(%4d) : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d, RopCode = %d\n",
               i4Left, i4Top, i4Width, i4Height, i4RopCode);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        _GFX_SetColor(0x12345678);
        // Action
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.ropcode %d", i4RopCode);
        _CLI_CMD("gfx.op.rop %d %d %d %d %d %d",
                 i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");
        _GFX_SetColor(0x12345678);
        // Action
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.set.ropcode %d", i4RopCode);
        _CLI_CMD("gfx.op.rop %d %d %d %d %d %d",
                 i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                    (UINT32 *)COMPARE_BUF,
                    (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("ROP Test 04 Error ");
            Printf("(%4d) : Diff = %d, (Error = %d)\n",
                   i4Count, (i4Count - i4PreCount), ++i4Error);
            i4PreCount = i4Count;
            return -1;
        }
    }
    Printf("# of Error Run = %d, # of Total Run = %d\n", i4Error, i4CountNum);
    return 0;
}

#endif

//-------------------------------------------------------------------------
/** _GfxEmuCmd2DMemComp1
 *  2D Memory Compare Test 01 (random)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmd2DMemComp1(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count;
    INT32 i4PicSel01, i4PicSel02;
    INT32 i4CountNum = 5000;
    INT32 i4HwMemCompFlag, i4SwMemCompFlag;

    Printf("2D Memory Compare Test 01 (random) \n");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        i4PicSel01 = ((UINT32)rand()) % 3;
        i4PicSel02 = ((UINT32)rand()) % 3;
        
        i4Left    = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right   = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top     = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom  = ((UINT32)rand()) % _u4ScreenHeight;

        i4Width   = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -1 * i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -1 * i4Height;
            i4Top = i4Bottom;
        }
        
        i4Width++;
        i4Height++;

        Printf("(%4d) : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d\n",
            i4Left, i4Top, i4Width, i4Height);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        // Action
        _CLI_CMD("gfx.set.src 0x%08x %s 2880", 
            BUF_ADDR1 + (i4PicSel01 * 0x151800), "rgb8");
        _CLI_CMD("gfx.set.dst 0x%08x %s 2880", 
            BUF_ADDR1 + (i4PicSel02 * 0x151800), "rgb8");
        _CLI_CMD("gfx.op.2dmemcomp %d %d %d %d %d %d", 
            i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");

        i4HwMemCompFlag = GFX_GetMemCompareResult();

        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");
        
        // Action
        _CLI_CMD("gfx.set.src 0x%08x %s 2880", 
            BUF_ADDR1 + (i4PicSel01 * 0x151800), "rgb8");
        _CLI_CMD("gfx.set.dst 0x%08x %s 2880", 
            BUF_ADDR1 + (i4PicSel02 * 0x151800), "rgb8");
        _CLI_CMD("gfx.op.2dmemcomp %d %d %d %d %d %d", 
            i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");

        i4SwMemCompFlag = GFX_SwGetMemCompareResult();

        Printf("Src Picture Number = %d\n", i4PicSel01 + 1);
        Printf("Dst Picture Number = %d\n", i4PicSel02 + 1);

        if (i4PicSel01 != i4PicSel02)
        {
            if ((i4HwMemCompFlag != 1) ||
                (i4SwMemCompFlag != 1))
            {
                Printf("... Fail !! \n");
                Printf("Hw Compare Flag = %d\n", i4HwMemCompFlag);
                Printf("Cm Compare Flag = %d\n", i4SwMemCompFlag);
                return -1;
            }
        }
        else
        {
            if ((i4HwMemCompFlag != 0) ||
                (i4SwMemCompFlag != 0))
            {
                Printf("... Fail !! \n");
                Printf("Hw Compare Flag = %d\n", i4HwMemCompFlag);
                Printf("Cm Compare Flag = %d\n", i4SwMemCompFlag);
                return -1;
            }
        }

        Printf("Hw Compare Flag = %d\n", i4HwMemCompFlag);
        Printf("Cm Compare Flag = %d\n", i4SwMemCompFlag);
        Printf("... OK !! \n");
    }

    Printf("2D Memory Compare Test 01 (random) ..... done\n");
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmd2DMemComp2
 *  2D Memory Compare Test 02 (random, only run hw)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmd2DMemComp2(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count;
    INT32 i4PicSel01, i4PicSel02;
    INT32 i4CountNum = 5000;

    Printf("2D Memory Compare Test 02 (random, only run hw) ... \n");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        i4PicSel01 = ((UINT32)rand()) % 3;
        i4PicSel02 = ((UINT32)rand()) % 3;
        
        i4Left    = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right   = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top     = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom  = ((UINT32)rand()) % _u4ScreenHeight;

        i4Width   = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -1 * i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -1 * i4Height;
            i4Top = i4Bottom;
        }
        
        i4Width++;
        i4Height++;

        Printf("(%4d) : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d\n",
            i4Left, i4Top, i4Width, i4Height);

        // Action
        _CLI_CMD("gfx.set.src 0x%08x %s 2880", 
            BUF_ADDR1 + (i4PicSel01 * 0x151800), "rgb8");
        _CLI_CMD("gfx.set.dst 0x%08x %s 2880", 
            BUF_ADDR1 + (i4PicSel02 * 0x151800), "rgb8");
        _CLI_CMD("gfx.op.2dmemcomp %d %d %d %d %d %d", 
            i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
    }
    _CLI_CMD("gfx.flush");
    Printf("2D Memory Compare Test 02 (random, only run hw) ..... done\n");
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdComposeLoop1
 *  Alpha Composition Loop Test 01 (all alpha values 0-255)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdComposeLoop1(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;

    // load 3 test bitmap first (@ 0x01151800, 0x012A3000, 0x013F4800)
    INT32 i4Ar, i4Op, i4ColIndex;
    INT32 i4ArStart = 0, i4ArEnd = 255;
    INT32 i4OpStart = 0, i4OpEnd = 11;
    INT32 i4RectSrc = 0;

    Printf("Alpha Composition Loop Test 01\n");
    
    if (i4Argc < 6)
    {
        Printf("arg: Ar_Start, Ar_End, Op_Start, Op_End, RectSrc\n");
        Printf("Ar = 0 ~ 255, Op = 0 ~ 11, RectSrc = 0/1\n");
        return -1;
    }

    i4ArStart = (INT32) StrToInt(szArgv[1]);
    i4ArEnd   = (INT32) StrToInt(szArgv[2]);
    i4OpStart = (INT32) StrToInt(szArgv[3]);
    i4OpEnd   = (INT32) StrToInt(szArgv[4]);
    i4RectSrc = (INT32) StrToInt(szArgv[5]);
    
    // compose kodak16 to kodak14
    for (i4Op = i4OpEnd; i4Op >= i4OpStart; i4Op--)
    {
        for (i4Ar = i4ArEnd; i4Ar >= i4ArStart; i4Ar--)
        {
            i4ColIndex = ((UINT32)rand()) % 147;
            Printf("OpCode = %s, Ar = %3d, RectSrc = %d (0x%08x) : ", 
                _aszAlphaComposeLoopOpCode[i4Op], i4Ar, i4RectSrc,
                _arNamedColor[i4ColIndex].u4Color);

            // HW
#ifdef GFX_HW_RESET
            _CLI_CMD("gfx.reset");
#endif
            _CLI_CMD("gfxemu.mode");

            _GfxSetDefMode(BUF_SCREEN, ADR_DST);
            _CLI_CMD("gfxemu.cls");
            _GfxSetDefMode(BUF_TEMP, ADR_SRC);
            _CLI_CMD("gfxemu.cls");

            // Action
            _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
            _GfxSetSdMode(BUF_TEMP, ADR_DST);
            _CLI_CMD("gfx.set.bltopt 0 0 0");
            _CLI_CMD("gfx.op.bitblt 0 0 0 0 %u %u", 
                _u4ScreenWidth, _u4ScreenHeight);
            _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
            _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
            _CLI_CMD("gfx.op.composeloop 0 0 0 0 %u %u %d %d %d", 
                _u4ScreenWidth, _u4ScreenHeight, i4Ar, i4Op, i4RectSrc);

            _GfxSetSdMode(BUF_TEMP, ADR_SRC);
            _GfxSetDefMode(BUF_SCREEN, ADR_DST);
            _CLI_CMD("gfx.op.bitblt 0 0 0 0 %u %u", 
                     _u4ScreenWidth, _u4ScreenHeight);
            _CLI_CMD("gfx.flush");


            // SW
#ifdef GFX_HW_RESET
            _CLI_CMD("gfx.reset");
#endif
            _CLI_CMD("gfxemu.mode sw");

            _GfxSetDefMode(COMPARE_BUF, ADR_DST);
            _CLI_CMD("gfxemu.cls");
            _GfxSetDefMode(BUF_TEMP, ADR_SRC);
            _CLI_CMD("gfxemu.cls");
            

            // Action
            _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
            _GfxSetSdMode(BUF_TEMP, ADR_DST);
            _CLI_CMD("gfx.set.bltopt 0 0 0");
            _CLI_CMD("gfx.op.bitblt 0 0 0 0 %u %u", 
                _u4ScreenWidth, _u4ScreenHeight);
            _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
            _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
            _CLI_CMD("gfx.op.composeloop 0 0 0 0 %u %u %d %d %d", 
                _u4ScreenWidth, _u4ScreenHeight, i4Ar, i4Op, i4RectSrc);

            _GfxSetSdMode(BUF_TEMP, ADR_SRC);
            _GfxSetDefMode(COMPARE_BUF, ADR_DST);
            _CLI_CMD("gfx.op.bitblt 0 0 0 0 %u %u", 
                _u4ScreenWidth, _u4ScreenHeight);
            _CLI_CMD("gfx.flush");

            // Compare HW and SW
            u4Size = _u4ScreenWidth * _u4ScreenHeight;
            if (_GfxMemCompare(
                    (UINT8 *)COMPARE_BUF, 
                    (UINT8 *)(BUF_SCREEN), u4Size ,_szDstModeIndex) == -1)
            {
                Printf("Alpha Composition Loop Test 01 Error\n");
                Printf("OpCode = %s, Ar = %3d, RectSrc = %d (0x%08x) : ", 
                    _aszAlphaComposeLoopOpCode[i4Op], i4Ar, i4RectSrc,
                    _arNamedColor[i4ColIndex].u4Color);
                return -1;
            }
        } //~for
    } //~for

    return 0;
}



//-------------------------------------------------------------------------
/** _GfxEmuCmdComposeLoop2
 *  Alpha Composition Loop Test 02 (pixel-by-pixel)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdComposeLoop2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4Width, i4Height, i4Count;
    INT32 i4SrcX, i4SrcY, i4ColIndex;
    INT32 i4PicSel, i4RectSrc, i4Ar, i4Op;
    INT32 i4CountNum = 2000;

    Printf("Alpha Composition Loop Test 02 (pix-by-pix)\n");
    
    if (i4Argc < 4)
    {
        Printf("arg: loops, width, height\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);
    i4Width    = (INT32) StrToInt(szArgv[2]);
    i4Height   = (INT32) StrToInt(szArgv[3]);

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        i4Ar       = ((UINT32)rand()) % 256;
        i4Op       = ((UINT32)rand()) % (((UINT32)E_AC_XOR) + 1);
        i4RectSrc  = ((UINT32)rand()) % 2;
        i4ColIndex = ((UINT32)rand()) % 147;
        i4PicSel   = ((UINT32)rand()) % 3;
                
        Printf("(%4d) : ", i4Count);
        Printf("Width = %d, Height = %d, ", i4Width, i4Height);
        Printf("OpCode = %s, Ar = %3d, RectSrc = %d (0x%08x) : ", 
                _aszAlphaComposeLoopOpCode[i4Op], i4Ar, i4RectSrc,
                _arNamedColor[i4ColIndex].u4Color);

        //======================================================================
        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfxemu.cls");
        _GfxSetDefMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfxemu.cls");

        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 %d %d", i4Width, i4Height);
        _CLI_CMD("gfx.flush");
                
        // HW Action
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        for (i4SrcY = 0; i4SrcY < i4Height; i4SrcY++)
        {
            for (i4SrcX = 0; i4SrcX < i4Width; i4SrcX++)
            {
                _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
                _CLI_CMD("gfx.op.composeloop %d %d %d %d %d %d %d %d %d", 
                    i4SrcX, i4SrcY, i4SrcX, i4SrcY, 1, 1, i4Ar, i4Op, i4RectSrc);
            } // ~for
        } // ~for

        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 %d %d", i4Width, i4Height);
        _CLI_CMD("gfx.flush");

        //======================================================================
        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfxemu.cls");
        _GfxSetDefMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfxemu.cls");

        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 %d %d", i4Width, i4Height);
        _CLI_CMD("gfx.flush");

        // SW Action
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        for (i4SrcY = 0; i4SrcY < i4Height; i4SrcY++)
        {
            for (i4SrcX = 0; i4SrcX < i4Width; i4SrcX++)
            {
                _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
                _CLI_CMD("gfx.op.composeloop %d %d %d %d %d %d %d %d %d", 
                    i4SrcX, i4SrcY, i4SrcX, i4SrcY, 1, 1, i4Ar, i4Op, i4RectSrc);
            } // ~for
        } // ~for

        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.op.bitblt 0 0 0 0 %d %d", i4Width, i4Height);
        _CLI_CMD("gfx.flush");

        //======================================================================

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxMemCompare(
            (UINT8 *)COMPARE_BUF, 
            (UINT8 *)(BUF_SCREEN), u4Size,_szDstModeIndex) == -1)
        {
            Printf("Alpha Composition Loop Test 02 (pix-by-pix) Error\n");
            Printf("(%4d) : ", i4Count);
            Printf("Width = %d, Height = %d, ", i4Width, i4Height);
            Printf("OpCode = %s, Ar = %3d, RectSrc = %d (0x%08x)\n", 
                    _aszAlphaComposeLoopOpCode[i4Op], i4Ar, i4RectSrc,
                    _arNamedColor[i4ColIndex].u4Color);
            return -1;
        }

    } // ~for

    Printf("\nAlpha Composition Loop Test 02 (pix-by-pix) ..... OK !\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdComposeLoop3
 *  Alpha Composition Loop Test 03 (random)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdComposeLoop3(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;

    // load 3 test bitmap first (@ 0x01151800, 0x012A3000, 0x013F4800)
    INT32 i, i4Ar, i4Op, i4ColIndex, i4PicSel, i4Temp;
    INT32 i4ArStart = 0, i4ArEnd = 255;
    INT32 i4OpStart = 0, i4OpEnd = 11;
    INT32 i4RectSrc = 0, i4Loops = 1000;
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height;

    Printf("Alpha Composition Loop Test 03 (random)\n");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4Loops = (INT32) StrToInt(szArgv[1]);
    
    for (i = 0; i < i4Loops; i++)
    {
        i4ArStart = ((UINT32)rand()) % 256;
        i4ArEnd   = ((UINT32)rand()) % 256;
        i4OpStart = ((UINT32)rand()) % (((UINT32)E_AC_XOR) + 1);
        i4OpEnd   = ((UINT32)rand()) % (((UINT32)E_AC_XOR) + 1);
        i4PicSel  = ((UINT32)rand()) % 3;
        i4Left    = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right   = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top     = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom  = ((UINT32)rand()) % _u4ScreenHeight;

        i4Width   = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -1 * i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -1 * i4Height;
            i4Top = i4Bottom;
        }
        
        i4Width++;
        i4Height++;

        if (i4ArStart > i4ArEnd)
        {
            i4Temp = i4ArEnd;
            i4ArEnd = i4ArStart;
            i4ArStart = i4Temp;
        }

        if (i4OpStart > i4OpEnd)
        {
            i4Temp = i4OpEnd;
            i4OpEnd = i4OpStart;
            i4OpStart = i4Temp;
        }

        Printf("(%4d) : ", i);
        Printf("L = %d, T = %d, W = %d, H = %d\n",
            i4Left, i4Top, i4Width, i4Height);
        Printf("ArStart = %d, ArEnd = %d, OpStart = %s, OpEnd = %s\n",
            i4ArStart, i4ArEnd, 
            _aszAlphaComposeLoopOpCode[i4OpStart],
            _aszAlphaComposeLoopOpCode[i4OpEnd]);

        for (i4Op = i4OpEnd; i4Op >= i4OpStart; i4Op--)
        {
            for (i4Ar = i4ArEnd; i4Ar >= i4ArStart; i4Ar--)
            {
                i4ColIndex = ((UINT32)rand()) % 147;
                i4RectSrc  = ((UINT32)rand()) % 2;

                Printf("OpCode = %s, Ar = %3d, RectSrc = %d (0x%08x) : ", 
                    _aszAlphaComposeLoopOpCode[i4Op], i4Ar, i4RectSrc,
                    _arNamedColor[i4ColIndex].u4Color);

                // HW
#ifdef GFX_HW_RESET
                _CLI_CMD("gfx.reset");
#endif
                _CLI_CMD("gfxemu.mode");

                _GfxSetDefMode(BUF_SCREEN, ADR_DST);
                _CLI_CMD("gfxemu.cls");
                _GfxSetDefMode(BUF_TEMP, ADR_DST);
                _CLI_CMD("gfxemu.cls");

                // Action
                _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
                _GfxSetSdMode(BUF_TEMP, ADR_DST);
                _CLI_CMD("gfx.set.bltopt 0 0 0");
                _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d", 
                    i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
                
                _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
                _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
                _CLI_CMD("gfx.op.composeloop %d %d %d %d %d %d %d %d %d", 
                    i4Left, i4Top, i4Left, i4Top, i4Width, i4Height, 
                    i4Ar, i4Op, i4RectSrc);

                _GfxSetSdMode(BUF_TEMP, ADR_SRC);
                _GfxSetDefMode(BUF_SCREEN, ADR_DST);
                _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d", 
                    i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
                _CLI_CMD("gfx.flush");


                // SW
#ifdef GFX_HW_RESET
                _CLI_CMD("gfx.reset");
#endif
                _CLI_CMD("gfxemu.mode sw");

                _GfxSetDefMode(COMPARE_BUF, ADR_DST);
                _CLI_CMD("gfxemu.cls");
                _GfxSetDefMode(BUF_TEMP, ADR_DST);
                _CLI_CMD("gfxemu.cls");
                

                // Action
                _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
                _GfxSetSdMode(BUF_TEMP, ADR_DST);
                _CLI_CMD("gfx.set.bltopt 0 0 0");
                _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d", 
                    i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);

                _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
                _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
                _CLI_CMD("gfx.op.composeloop %d %d %d %d %d %d %d %d %d", 
                    i4Left, i4Top, i4Left, i4Top, i4Width, i4Height, 
                    i4Ar, i4Op, i4RectSrc);

                _GfxSetSdMode(BUF_TEMP, ADR_SRC);
                _GfxSetDefMode(COMPARE_BUF, ADR_DST);
                _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d", 
                    i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
                _CLI_CMD("gfx.flush");

                // Compare HW and SW
                u4Size = _u4ScreenWidth * _u4ScreenHeight;
                if (_GfxMemCompare(
                        (UINT8 *)COMPARE_BUF, 
                        (UINT8 *)(BUF_SCREEN), u4Size,_szDstModeIndex) == -1)
                {
                    Printf("Alpha Composition Loop Test 03 (random) Error\n");
                    Printf("(%4d) : ", i);
                    Printf("L = %d, T = %d, W = %d, H = %d\n",
                        i4Left, i4Top, i4Width, i4Height);
                    Printf("ArStart = %d, ArEnd = %d, OpStart = %s, OpEnd = %s\n",
                        i4ArStart, i4ArEnd, 
                        _aszAlphaComposeLoopOpCode[i4OpStart],
                        _aszAlphaComposeLoopOpCode[i4OpEnd]);
                    Printf("OpCode = %s, Ar = %3d, RectSrc = %d (0x%08x) : ", 
                        _aszAlphaComposeLoopOpCode[i4Op], i4Ar, i4RectSrc,
                        _arNamedColor[i4ColIndex].u4Color);
                    return -1;
                }
            } //~for
        } //~for
    } //~for

    Printf("Alpha Composition Loop Test 03 (random) ..... OK !\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdComposeLoop4
 *  Alpha Composition Loop Test 04 (random)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdComposeLoop4(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;

    // load 3 test bitmap first (@ 0x01151800, 0x012A3000, 0x013F4800)
    INT32 i, i4Ar, i4Op, i4ColIndex, i4PicSel;
    INT32 i4RectSrc = 0, i4Loops = 1000;
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height;

    Printf("Alpha Composition Loop Test 04 (random)\n");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4Loops = (INT32) StrToInt(szArgv[1]);
  
    for (i = 0; i < i4Loops; i++)
    {
        i4Ar       = ((UINT32)rand()) % 256;
        i4Op       = ((UINT32)rand()) % (((UINT32)E_AC_XOR) + 1);
        i4RectSrc  = ((UINT32)rand()) % 2;
        i4PicSel   = ((UINT32)rand()) % 3;
        i4ColIndex = ((UINT32)rand()) % 147;

        i4Left    = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right   = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top     = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom  = ((UINT32)rand()) % _u4ScreenHeight;

        //Printf("%d, %d, %d, %d, %d, %d, %d, %d, %d\n", 
        	//i4Ar, i4Op, i4RectSrc, i4PicSel, i4ColIndex, i4Left, i4Right, i4Top, i4Bottom);

        i4Width   = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -1 * i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -1 * i4Height;
            i4Top = i4Bottom;
        }
        
        i4Width++;
        i4Height++;

        Printf("(%5d) : ", i);
        Printf("L = %d, T = %d, W = %d, H = %d\n", i4Left, i4Top, i4Width, i4Height);
        Printf("OpCode = %s, Ar = %3d, RectSrc = %d (0x%08x) : ", 
                _aszAlphaComposeLoopOpCode[i4Op], i4Ar, i4RectSrc,
                _arNamedColor[i4ColIndex].u4Color);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfxemu.cls");
        _GfxSetDefMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfxemu.cls");

        // Action
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d", 
            i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");
                
        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _CLI_CMD("gfx.op.composeloop %d %d %d %d %d %d %d %d %d", 
            i4Left, i4Top, i4Left, i4Top, i4Width, i4Height, i4Ar, i4Op, i4RectSrc);
        _CLI_CMD("gfx.flush");

        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d", 
            i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfxemu.cls");
        _GfxSetDefMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfxemu.cls");
                

        // Action
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetSdMode(BUF_TEMP, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d", 
            i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");

        _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _CLI_CMD("gfx.op.composeloop %d %d %d %d %d %d %d %d %d", 
            i4Left, i4Top, i4Left, i4Top, i4Width, i4Height, i4Ar, i4Op, i4RectSrc);
        _CLI_CMD("gfx.flush");

        _GfxSetSdMode(BUF_TEMP, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d", 
            i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxMemCompare(
                (UINT8 *)COMPARE_BUF, 
                (UINT8 *)(BUF_SCREEN), u4Size,_szDstModeIndex) == -1)
        {
            Printf("Alpha Composition Loop Test 04 (random) Error\n");
            Printf("(%5d) : ", i);
            Printf("L = %d, T = %d, W = %d, H = %d\n", i4Left, i4Top, i4Width, i4Height);
            Printf("OpCode = %s, Ar = %3d, RectSrc = %d (0x%08x) : ", 
                _aszAlphaComposeLoopOpCode[i4Op], i4Ar, i4RectSrc,
                _arNamedColor[i4ColIndex].u4Color);
            return -1;
        }
    } //~for

    Printf("Alpha Composition Loop Test 04 (random) ..... OK !\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdComposeLoop5
 *  Alpha Composition Loop Test 05
 *    gfx.emu.cpl5 {L} {T} {W} {H} {OP} {Ar} {RectSrc}
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdComposeLoop5(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;

    // load 3 test bitmap first (@ 0x01151800, 0x012A3000, 0x013F4800)
    INT32 i4Ar, i4Op, i4ColIndex, i4PicSel;
    INT32 i4Left, i4Top, i4Width, i4Height, i4RectSrc;

    Printf("Alpha Composition Loop Test 05\n");
    
    if (i4Argc < 8)
    {
        Printf("args: {L} {T} {W} {H} {OP} {Ar} {RectSrc}\n");
        return -1;
    }

    i4Left    = (INT32) StrToInt(szArgv[1]);
    i4Top     = (INT32) StrToInt(szArgv[2]);
    i4Width   = (INT32) StrToInt(szArgv[3]);
    i4Height  = (INT32) StrToInt(szArgv[4]);
    i4Op      = (INT32) StrToInt(szArgv[5]);
    i4Ar      = (INT32) StrToInt(szArgv[6]);
    i4RectSrc = (INT32) StrToInt(szArgv[7]);

    i4PicSel   = ((UINT32)rand()) % 3;
    i4ColIndex = ((UINT32)rand()) % 147;

    Printf("L = %d, T = %d, W = %d, H = %d\n", i4Left, i4Top, i4Width, i4Height);
    Printf("OpCode = %s, Ar = %3d, RectSrc = %d (0x%08x) : ", 
        _aszAlphaComposeLoopOpCode[i4Op], i4Ar, i4RectSrc,
        _arNamedColor[i4ColIndex].u4Color);

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _CLI_CMD("gfxemu.cls");
    _GfxSetDefMode(BUF_TEMP, ADR_DST);
    _CLI_CMD("gfxemu.cls");

    // Action
    _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
    _GfxSetSdMode(BUF_TEMP, ADR_DST);
    _CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d", 
        i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
    _CLI_CMD("gfx.flush");
                
    _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
    _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
    _CLI_CMD("gfx.op.composeloop %d %d %d %d %d %d %d %d %d", 
        i4Left, i4Top, i4Left, i4Top, i4Width, i4Height, i4Ar, i4Op, i4RectSrc);
    _CLI_CMD("gfx.flush");

    _GfxSetSdMode(BUF_TEMP, ADR_SRC);
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d", 
        i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
    _CLI_CMD("gfx.flush");


    // SW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");

    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _CLI_CMD("gfxemu.cls");
    _GfxSetDefMode(BUF_TEMP, ADR_DST);
    _CLI_CMD("gfxemu.cls");
                

    // Action
    _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
    _GfxSetSdMode(BUF_TEMP, ADR_DST);
    _CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d", 
        i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
    _CLI_CMD("gfx.flush");

    _GfxSetSdMode(BUF_ADDR1 + (i4PicSel * 0x151800), ADR_SRC);
    _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
    _CLI_CMD("gfx.op.composeloop %d %d %d %d %d %d %d %d %d", 
        i4Left, i4Top, i4Left, i4Top, i4Width, i4Height, i4Ar, i4Op, i4RectSrc);
    _CLI_CMD("gfx.flush");

    _GfxSetSdMode(BUF_TEMP, ADR_SRC);
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d", 
        i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
    _CLI_CMD("gfx.flush");

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
	//_GfxBufferCompare
    if (_GfxMemCompare(
        (UINT8 *)COMPARE_BUF, 
        (UINT8 *)(BUF_SCREEN), u4Size,_szDstModeIndex) == -1)
    {
        Printf("Alpha Composition Loop Test 05 Error\n");
        Printf("L = %d, T = %d, W = %d, H = %d\n", i4Left, i4Top, i4Width, i4Height);
        Printf("OpCode = %s, Ar = %3d, RectSrc = %d (0x%08x) : ", 
            _aszAlphaComposeLoopOpCode[i4Op], i4Ar, i4RectSrc,
            _arNamedColor[i4ColIndex].u4Color);
        return -1;
    }

    Printf("Alpha Composition Loop Test 05 ..... OK !\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdComposeLoop6
 *  Alpha Composition Loop Test 06 (random, only run hw) 
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdComposeLoop6(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i, i4Ar, i4Op, i4ColIndex;
    INT32 i4RectSrc = 0, i4Loops = 1000;
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height;

    Printf("Alpha Composition Loop Test 06 (random)\n");
    Printf("(Only gfx engine runs several times to check crashed issue)\n");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4Loops = (INT32) StrToInt(szArgv[1]);

    // HW
    //_CLI_CMD("gfx.reset");
    _CLI_CMD("gfxemu.mode");

    for (i = 0; i < i4Loops; i++)
    {
        if ((((UINT32)rand()) % 4) == 0)
        {
            i4Ar = 255;            
        }
        else
        {
        i4Ar       = ((UINT32)rand()) % 256;
        }
        i4Op       = ((UINT32)rand()) % (((UINT32)E_AC_XOR) + 1);
        i4RectSrc  = ((UINT32)rand()) % 2;
        i4ColIndex = ((UINT32)rand()) % 147;

        i4Left    = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right   = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top     = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom  = ((UINT32)rand()) % _u4ScreenHeight;

        i4Width   = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -1 * i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -1 * i4Height;
            i4Top = i4Bottom;
        }
        
        i4Width++;
        i4Height++;
        
        Printf("(%5d) : ", i);
        Printf("L = %d, T = %d, W = %d, H = %d\n", i4Left, i4Top, i4Width, i4Height);
        Printf("OpCode = %s, Ar = %3d, RectSrc = %d (0x%08x) : ", 
                _aszAlphaComposeLoopOpCode[i4Op], i4Ar, i4RectSrc,
                _arNamedColor[i4ColIndex].u4Color);

        // Action
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColIndex].u4Color);
        _CLI_CMD("gfx.op.composeloop %d %d %d %d %d %d %d %d %d", 
            i4Left, i4Top, i4Left, i4Top, i4Width, i4Height, i4Ar, i4Op, i4RectSrc);
    } //~for
    _CLI_CMD("gfx.flush");
    
    Printf("Alpha Composition Loop Test 06 (random) ..... OK !\n");
    Printf("(Only gfx engine runs several times to check crashed issue)\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdIdx2DirBlt1
 *  Index to Direct Color BitBlt Test 01
 */
//-------------------------------------------------------------------------
static UINT16 _au2PaleTable_16Entry_16BPP[] = 
{
    0x0f8f, 
    0xf2ff, 
    0xf000, 
    0xf222, 
    0xf444, 
    0xf777, 
    0xf999, 
    0xfccc, 
    0xfeee, 
    0xf800, 
    0xff0f, 
    0xf808, 
    0xfff0, 
    0xf880, 
    0xffff, 
    0xf888
};

//--------------------------------------------------
// index to direct color bitblt test 01 (for Subtitle data use)
//--------------------------------------------------
static INT32 _GfxEmuCmdIdx2DirBlt1(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height, u4DstPitch, u4MsbLeft, u4StByAl;
    UINT16 *pu2PaleTableAddr = NULL;
    UINT32 i, u4Size, u4Align, u4TableEntryNum;

    Printf("Index to Direct Color BitBlt Test 01\n");

    if (i4Argc < 6)
    {
        Printf("arg: char_cm, width, height, msbleft, start_byte_align\n");
        return -1;
    }

    u4Width   = (UINT32) StrToInt(szArgv[2]);
    u4Height  = (UINT32) StrToInt(szArgv[3]);
    u4MsbLeft = (UINT32) StrToInt(szArgv[4]);
    u4StByAl  = (UINT32) StrToInt(szArgv[5]);

    // Allocate palette table
    {
#ifdef GFX_BURST_READ
        u4Align = 64;   // 16 bytes alignment
#else
        u4Align = 16;   // 16 bytes alignment
#endif

        u4Size = (UINT32) sizeof(_au2PaleTable_16Entry_16BPP);
        pu2PaleTableAddr = (UINT16 *)BSP_AllocAlignedDmaMemory(u4Size, u4Align);
        if (pu2PaleTableAddr == NULL)
        {
            Printf("Error : pu2PaleTableAddr is NULL\n");
            return -1;
        }

        u4TableEntryNum = (u4Size / 2);
        for (i = 0; i < u4TableEntryNum; i++)
        {
            pu2PaleTableAddr[i] = _au2PaleTable_16Entry_16BPP[i];
        }
        
        Printf("src_w = %u, src_h = %u\n", u4Width, u4Height);
        Printf("pale_table_entry_num = %u\n", u4TableEntryNum);
        Printf("pale_table_address = 0x%08x\n", (UINT32)pu2PaleTableAddr);
    }
    

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _CLI_CMD("gfxemu.cls");

    // HW Action
    // 1. set idx2dir source address
    // 2. set destination address
    // 3. set index to direct color bitblt option
    // 4. do index to direct color bitblt
    // 5. flush
    u4DstPitch = (u4Width * 2);

    // gfx.set.idx2dirsrc {buff_base_address} {char_cm}
    _CLI_CMD("gfx.set.idx2dirsrc 0x%08x %s", BUF_ADDR4, szArgv[1]);

    // gfx.set.dst {buff_base_address} {color_mode} {pitch_in_bytes}
    _CLI_CMD("gfx.set.dst 0x%08x %s %u", BUF_SCREEN, "argb4444", u4DstPitch);
    
    // gfx.set.idx2diropt {palet_addr} {msb_left} {start_byte_align}
    _CLI_CMD("gfx.set.idx2diropt 0x%08x %u %u", 
        (UINT32)pu2PaleTableAddr, u4MsbLeft, u4StByAl);

    // gfx.op.idx2dirblt {dx} {dy} {w} {h}
    _CLI_CMD("gfx.op.idx2dirblt 0 0 %u %u", u4Width, u4Height);
    
    _CLI_CMD("gfx.flush");


    // SW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");

    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _CLI_CMD("gfxemu.cls");

    // SW Action
    // 1. set idx2dir source address
    // 2. set destination address
    // 3. set index to direct color bitblt option
    // 4. do index to direct color bitblt
    // 5. flush
    u4DstPitch = (u4Width * 2);

    // gfx.set.idx2dirsrc {buff_base_address} {char_cm}
    _CLI_CMD("gfx.set.idx2dirsrc 0x%08x %s", BUF_ADDR4, szArgv[1]);

    // gfx.set.dst {buff_base_address} {color_mode} {pitch_in_bytes}
    _CLI_CMD("gfx.set.dst 0x%08x %s %u", COMPARE_BUF, "argb4444", u4DstPitch);
    
    // gfx.set.idx2diropt {palet_addr} {msb_left} {start_byte_align}
    _CLI_CMD("gfx.set.idx2diropt 0x%08x %u %u", 
        (UINT32)pu2PaleTableAddr, u4MsbLeft, u4StByAl);

    // gfx.op.idx2dirblt {dx} {dy} {w} {h}
    _CLI_CMD("gfx.op.idx2dirblt 0 0 %u %u", u4Width, u4Height);
    
    _CLI_CMD("gfx.flush");

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    if (_GfxBufferCompare(
        (UINT32 *)COMPARE_BUF, 
        (UINT32 *)(BUF_SCREEN), u4Size) == -1)
    {
        BSP_FreeAlignedDmaMemory((UINT32)pu2PaleTableAddr);
        Printf("Error: Index to Direct Color BitBlt Test 01\n");
        return -1;
    }

    BSP_FreeAlignedDmaMemory((UINT32)pu2PaleTableAddr);
    Printf("Index to Direct Color BitBlt Test 01 .... OK !\n");
    return 0;
}

//--------------------------------------------------
// index to direct color bitblt test 02 (random)
//--------------------------------------------------
static INT32 _GfxEmuCmdIdx2DirBlt2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height, u4DstPitch, u4MsbLeft, u4StByAl;
    UINT8 *pu1PaleTableAddr = NULL;
    UINT32 i, u4Size, u4Align, u4TableEntryNum, u4Dx, u4Dy;
    UINT32 u4CharCM, u4DstCM, u4Loops;
    UINT32 u4BitsPerSrcPix, u4BytesPerDstPix;
    CHAR *szCharCM;
    CHAR *szDstCM;

    Printf("Index to Direct Color BitBlt Test 02 (random)\n");

    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    u4Loops = (UINT32) StrToInt(szArgv[1]);

    // Allocate palette table
    {
#ifdef GFX_BURST_READ
        u4Align = 64;   // 16 bytes alignment
#else
        u4Align = 16;   // 16 bytes alignment
#endif

        u4Size = (UINT32) sizeof(_u1gfxpt_img_1_clut);
        pu1PaleTableAddr = (UINT8 *)BSP_AllocAlignedDmaMemory(u4Size, u4Align);
        if (pu1PaleTableAddr == NULL)
        {
            Printf("Error : pu1PaleTableAddr is NULL\n");
            return -1;
        }

        for (i = 0; i < u4Size; i++)
        {
            pu1PaleTableAddr[i] = _u1gfxpt_img_1_clut[i];
        }

        u4TableEntryNum = (u4Size / 4);

        Printf("pale_table_entry_num = %u\n", u4TableEntryNum);
        Printf("pale_table_address = 0x%08x\n", (UINT32)pu1PaleTableAddr);
    }

    for (i = 0; i < u4Loops; i++)
    {
        // set dst color mode (argb8888/4444/1555, rgb565)
        u4DstCM   = ((UINT32)rand()) % 4;
        if (u4DstCM == 0)
        {
            u4BytesPerDstPix = 4;
            szDstCM = "argb8888";
        }
        else
        {
            u4BytesPerDstPix = 2;
            switch (u4DstCM)
            {
                case 1: szDstCM = "argb4444"; break;
                case 2: szDstCM = "argb1555"; break;
                case 3: szDstCM = "rgb565";   break;
                default: return -1;
            }
        }

        // set char color mode
        u4CharCM  = ((UINT32)rand()) % 4;
        switch (u4CharCM)
        {
            case 0: szCharCM = "1bit"; u4BitsPerSrcPix = 1; break;
            case 1: szCharCM = "2bit"; u4BitsPerSrcPix = 2; break;
            case 2: szCharCM = "4bit"; u4BitsPerSrcPix = 4; break;
            case 3: szCharCM = "8bit"; u4BitsPerSrcPix = 8; break;
            default: return -1;
        }

        u4Dx = ((UINT32)rand()) % (_u4ScreenWidth / 2);
        u4Dy = ((UINT32)rand()) % (_u4ScreenHeight / 2);
        
        while (1)
        {
            u4Width = ((UINT32)rand()) % _u4ScreenWidth;
            if (((u4Dx + u4Width) < _u4ScreenWidth) && (u4Width > 0))
            {
                break;
            }
        }

        while (1)
        {
            u4Height = ((UINT32)rand()) % _u4ScreenHeight;
            if (((u4Dy + u4Height) < _u4ScreenHeight) && (u4Height > 0))
            {
                break;
            }
        }
        

        u4MsbLeft = ((UINT32)rand()) % 2;

        if (((u4BitsPerSrcPix * u4Width) % 8) > 0)
        {
            u4StByAl = 1;
        }
        else
        {
            u4StByAl = 0;
        }

        u4DstPitch = (_u4ScreenWidth * u4BytesPerDstPix);

        #ifdef GFX_BURST_READ
            //_CLI_CMD("gfxemu.mode cm %d",u4DstCM);
            u4DstPitch = GFX_BURST_READ_ALIGN(u4DstPitch);
            //_szCurColorMode = _aszColorMode[u4DstCM];
            //_u4GfxPitch = u4DstPitch;
        #endif

        Printf("(%4d) : ", i);
        Printf("Dx = %u, Dy = %u, W = %u, H = %u\n", u4Dx, u4Dy, u4Width, u4Height);
        Printf("CharCM = %s, DstCM = %s, MsbLeft = %u, SAddrByteAlign = %u, DstPitch = %u\n",
                szCharCM, szDstCM, u4MsbLeft, u4StByAl, u4DstPitch);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfxemu.cls");

        // HW Action
        // 1. set idx2dir source address
        // 2. set destination address
        // 3. set index to direct color bitblt option
        // 4. do index to direct color bitblt
        // 5. flush

        // gfx.set.idx2dirsrc {buff_base_address} {char_cm}
        _CLI_CMD("gfx.set.idx2dirsrc 0x%08x %s", BUF_ADDR2, szCharCM);

        // gfx.set.dst {buff_base_address} {color_mode} {pitch_in_bytes}
        _CLI_CMD("gfx.set.dst 0x%08x %s %u", BUF_SCREEN, szDstCM, u4DstPitch);
        
        // gfx.set.idx2diropt {palet_addr} {msb_left} {start_byte_align}
        _CLI_CMD("gfx.set.idx2diropt 0x%08x %u %u", 
            (UINT32)pu1PaleTableAddr, u4MsbLeft, u4StByAl);

        // gfx.op.idx2dirblt {dx} {dy} {w} {h}
        _CLI_CMD("gfx.op.idx2dirblt %u %u %u %u", u4Dx, u4Dy, u4Width, u4Height);
        
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfxemu.cls");

        // SW Action
        // 1. set idx2dir source address
        // 2. set destination address
        // 3. set index to direct color bitblt option
        // 4. do index to direct color bitblt
        // 5. flush

        // gfx.set.idx2dirsrc {buff_base_address} {char_cm}
        _CLI_CMD("gfx.set.idx2dirsrc 0x%08x %s", BUF_ADDR2, szCharCM);

        // gfx.set.dst {buff_base_address} {color_mode} {pitch_in_bytes}
        _CLI_CMD("gfx.set.dst 0x%08x %s %u", COMPARE_BUF, szDstCM, u4DstPitch);
        
        // gfx.set.idx2diropt {palet_addr} {msb_left} {start_byte_align}
        _CLI_CMD("gfx.set.idx2diropt 0x%08x %u %u", 
            (UINT32)pu1PaleTableAddr, u4MsbLeft, u4StByAl);

        // gfx.op.idx2dirblt {dx} {dy} {w} {h}
        _CLI_CMD("gfx.op.idx2dirblt %u %u %u %u", u4Dx, u4Dy, u4Width, u4Height);
        
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
            (UINT32 *)COMPARE_BUF, 
            (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            BSP_FreeAlignedDmaMemory((UINT32)pu1PaleTableAddr);
            Printf("Error: Index to Direct Color BitBlt Test 02 FAIL !!\n");
            return -1;
        }
    } //~for

    BSP_FreeAlignedDmaMemory((UINT32)pu1PaleTableAddr);
    Printf("Index to Direct Color BitBlt Test 02 (random) .... OK !\n");
    return 0;
}

//--------------------------------------------------
// index to direct color bitblt test 03 (debug use)
//--------------------------------------------------
static INT32 _GfxEmuCmdIdx2DirBlt3(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height, u4DstPitch, u4MsbLeft, u4StByAl;
    UINT8 *pu1PaleTableAddr = NULL;
    UINT32 i, u4Size, u4Align, u4TableEntryNum, u4Dx, u4Dy;
    UINT32 u4CharCM, u4DstCM, u4Loops = 1;
    UINT32 u4BitsPerSrcPix, u4BytesPerDstPix;
    CHAR *szCharCM;
    CHAR *szDstCM;

    Printf("Index to Direct Color BitBlt Test 03 (random)\n");

    if (i4Argc < 10)
    {
        Printf("arg: dx, dy, w, h, char_cm, dst_cm, msbleft, stbyal, loop\n");
        return -1;
    }

    u4Dx      = (UINT32) StrToInt(szArgv[1]);
    u4Dy      = (UINT32) StrToInt(szArgv[2]);
    u4Width   = (UINT32) StrToInt(szArgv[3]);
    u4Height  = (UINT32) StrToInt(szArgv[4]);
    u4CharCM  = (UINT32) StrToInt(szArgv[5]);
    u4DstCM   = (UINT32) StrToInt(szArgv[6]);
    u4MsbLeft = (UINT32) StrToInt(szArgv[7]);
    u4StByAl  = (UINT32) StrToInt(szArgv[8]);
    u4Loops   = (UINT32) StrToInt(szArgv[9]);

    // Allocate palette table
    {
#ifdef GFX_BURST_READ
        u4Align = 64;   // 16 bytes alignment
#else
        u4Align = 16;   // 16 bytes alignment
#endif

        u4Size = (UINT32) sizeof(_u1gfxpt_img_1_clut);
        pu1PaleTableAddr = (UINT8 *)BSP_AllocAlignedDmaMemory(u4Size, u4Align);
        if (pu1PaleTableAddr == NULL)
        {
            Printf("Error : pu1PaleTableAddr is NULL\n");
            return -1;
        }

        for (i = 0; i < u4Size; i++)
        {
            pu1PaleTableAddr[i] = _u1gfxpt_img_1_clut[i];
        }

        u4TableEntryNum = (u4Size / 4);

        Printf("pale_table_entry_num = %u\n", u4TableEntryNum);
        Printf("pale_table_address = 0x%08x\n", (UINT32)pu1PaleTableAddr);
    }

    for (i = 0; i < u4Loops; i++)
    {
        // set dst color mode
        switch (u4DstCM)
        {
            case CM_ARGB8888_DIRECT32:
                u4BytesPerDstPix = 4;
                szDstCM = "argb8888";
                break;

            case CM_ARGB4444_DIRECT16:
                u4BytesPerDstPix = 2;
                szDstCM = "argb4444";
                break;

            case CM_ARGB1555_DIRECT16:
                u4BytesPerDstPix = 2;
                szDstCM = "argb1555";
                break;

            case CM_RGB565_DIRECT16:
                u4BytesPerDstPix = 2;
                szDstCM = "rgb565";
                break;

           default: return -1; 
        }

        // set char color mode
        switch (u4CharCM)
        {
            case 0: szCharCM = "1bit"; u4BitsPerSrcPix = 1; break;
            case 1: szCharCM = "2bit"; u4BitsPerSrcPix = 2; break;
            case 2: szCharCM = "4bit"; u4BitsPerSrcPix = 4; break;
            case 3: szCharCM = "8bit"; u4BitsPerSrcPix = 8; break;
            default: return -1;
        }

	u4BitsPerSrcPix = u4BitsPerSrcPix;//solve warning
	
        u4DstPitch = (_u4ScreenWidth * u4BytesPerDstPix);

        Printf("(%4d) : ", i);
        Printf("Dx = %u, Dy = %u, W = %u, H = %u\n", u4Dx, u4Dy, u4Width, u4Height);
        Printf("CharCM = %s, DstCM = %s, MsbLeft = %u, SAddrByteAlign = %u, DstPitch = %u\n",
                szCharCM, szDstCM, u4MsbLeft, u4StByAl, u4DstPitch);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfxemu.cls");

        // HW Action
        // 1. set idx2dir source address
        // 2. set destination address
        // 3. set index to direct color bitblt option
        // 4. do index to direct color bitblt
        // 5. flush

        // gfx.set.idx2dirsrc {buff_base_address} {char_cm}
        _CLI_CMD("gfx.set.idx2dirsrc 0x%08x %s", BUF_ADDR2, szCharCM);

        // gfx.set.dst {buff_base_address} {color_mode} {pitch_in_bytes}
        _CLI_CMD("gfx.set.dst 0x%08x %s %u", BUF_SCREEN, szDstCM, u4DstPitch);
        
        // gfx.set.idx2diropt {palet_addr} {msb_left} {start_byte_align}
        _CLI_CMD("gfx.set.idx2diropt 0x%08x %u %u", 
            (UINT32)pu1PaleTableAddr, u4MsbLeft, u4StByAl);

        // gfx.op.idx2dirblt {dx} {dy} {w} {h}
        _CLI_CMD("gfx.op.idx2dirblt %u %u %u %u", u4Dx, u4Dy, u4Width, u4Height);
        
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfxemu.cls");

        // SW Action
        // 1. set idx2dir source address
        // 2. set destination address
        // 3. set index to direct color bitblt option
        // 4. do index to direct color bitblt
        // 5. flush

        // gfx.set.idx2dirsrc {buff_base_address} {char_cm}
        _CLI_CMD("gfx.set.idx2dirsrc 0x%08x %s", BUF_ADDR2, szCharCM);

        // gfx.set.dst {buff_base_address} {color_mode} {pitch_in_bytes}
        _CLI_CMD("gfx.set.dst 0x%08x %s %u", COMPARE_BUF, szDstCM, u4DstPitch);
        
        // gfx.set.idx2diropt {palet_addr} {msb_left} {start_byte_align}
        _CLI_CMD("gfx.set.idx2diropt 0x%08x %u %u", 
            (UINT32)pu1PaleTableAddr, u4MsbLeft, u4StByAl);

        // gfx.op.idx2dirblt {dx} {dy} {w} {h}
        _CLI_CMD("gfx.op.idx2dirblt %u %u %u %u", u4Dx, u4Dy, u4Width, u4Height);
        
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
            (UINT32 *)COMPARE_BUF, 
            (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            BSP_FreeAlignedDmaMemory((UINT32)pu1PaleTableAddr);
            Printf("Error: Index to Direct Color BitBlt Test 03 FAIL !!\n");
            return -1;
        }
    } //~for

    BSP_FreeAlignedDmaMemory((UINT32)pu1PaleTableAddr);
    Printf("Index to Direct Color BitBlt Test 03 (debug use) .... OK !\n");
    return 0;
}

//--------------------------------------------------
// index to direct color bitblt test 04 (random, only run hw)
//--------------------------------------------------
static INT32 _GfxEmuCmdIdx2DirBlt4(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height, u4DstPitch, u4MsbLeft, u4StByAl;
    UINT8 *pu1PaleTableAddr = NULL;
    UINT32 i, u4Size, u4Align, u4TableEntryNum, u4Dx, u4Dy;
    UINT32 u4CharCM, u4DstCM, u4Loops;
    UINT32 u4BitsPerSrcPix, u4BytesPerDstPix;
    CHAR *szCharCM;
    CHAR *szDstCM;

    Printf("Index to Direct Color BitBlt Test 04 (random, only run hw)\n");

    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    u4Loops = (UINT32) StrToInt(szArgv[1]);

    // Allocate palette table
    {
        #ifdef GFX_BURST_READ
            u4Align = 64;   // 16 bytes alignment
        #else
            u4Align = 16;   // 16 bytes alignment
        #endif
        u4Size = (UINT32) sizeof(_u1gfxpt_img_1_clut);
        pu1PaleTableAddr = (UINT8 *)BSP_AllocAlignedDmaMemory(u4Size, u4Align);
        if (pu1PaleTableAddr == NULL)
        {
            Printf("Error : pu1PaleTableAddr is NULL\n");
            return -1;
        }

        for (i = 0; i < u4Size; i++)
        {
            pu1PaleTableAddr[i] = _u1gfxpt_img_1_clut[i];
        }

        u4TableEntryNum = (u4Size / 4);

        Printf("pale_table_entry_num = %u\n", u4TableEntryNum);
        Printf("pale_table_address = 0x%08x\n", (UINT32)pu1PaleTableAddr);
    }

    for (i = 0; i < u4Loops; i++)
    {
        // set dst color mode (argb8888/4444/1555, rgb565)
        u4DstCM   = ((UINT32)rand()) % 4;
        if (u4DstCM == 0)
        {
            u4BytesPerDstPix = 4;
            szDstCM = "argb8888";
        }
        else
        {
            u4BytesPerDstPix = 2;
            switch (u4DstCM)
            {
                case 1: szDstCM = "argb4444"; break;
                case 2: szDstCM = "argb1555"; break;
                case 3: szDstCM = "rgb565";   break;
                default: return -1;
            }
        }

        // set char color mode
        u4CharCM  = ((UINT32)rand()) % 4;
        switch (u4CharCM)
        {
            case 0: szCharCM = "1bit"; u4BitsPerSrcPix = 1; break;
            case 1: szCharCM = "2bit"; u4BitsPerSrcPix = 2; break;
            case 2: szCharCM = "4bit"; u4BitsPerSrcPix = 4; break;
            case 3: szCharCM = "8bit"; u4BitsPerSrcPix = 8; break;
            default: return -1;
        }

        u4Dx = ((UINT32)rand()) % (_u4ScreenWidth / 2);
        u4Dy = ((UINT32)rand()) % (_u4ScreenHeight / 2);
        
        while (1)
        {
            u4Width = ((UINT32)rand()) % _u4ScreenWidth;
            if (((u4Dx + u4Width) < _u4ScreenWidth) && (u4Width > 0))
            {
                break;
            }
        }

        while (1)
        {
            u4Height = ((UINT32)rand()) % _u4ScreenHeight;
            if (((u4Dy + u4Height) < _u4ScreenHeight) && (u4Height > 0))
            {
                break;
            }
        }
        

        u4MsbLeft = ((UINT32)rand()) % 2;

        if (((u4BitsPerSrcPix * u4Width) % 8) > 0)
        {
            u4StByAl = 1;
        }
        else
        {
            u4StByAl = 0;
        }

        u4DstPitch = (_u4ScreenWidth * u4BytesPerDstPix);
        #ifdef GFX_BURST_READ
            u4DstPitch = GFX_BURST_READ_ALIGN((_u4ScreenWidth * u4BytesPerDstPix));
        #endif
        Printf("(%4d) : ", i);
        Printf("Dx = %u, Dy = %u, W = %u, H = %u\n", u4Dx, u4Dy, u4Width, u4Height);
        Printf("CharCM = %s, DstCM = %s, MsbLeft = %u, SAddrByteAlign = %u, DstPitch = %u\n",
                szCharCM, szDstCM, u4MsbLeft, u4StByAl, u4DstPitch);

        // HW Action
        // 1. set idx2dir source address
        // 2. set destination address
        // 3. set index to direct color bitblt option
        // 4. do index to direct color bitblt
        // 5. flush

        // gfx.set.idx2dirsrc {buff_base_address} {char_cm}
        _CLI_CMD("gfx.set.idx2dirsrc 0x%08x %s", BUF_ADDR2, szCharCM);

        // gfx.set.dst {buff_base_address} {color_mode} {pitch_in_bytes}
        _CLI_CMD("gfx.set.dst 0x%08x %s %u", BUF_SCREEN, szDstCM, u4DstPitch);
        
        // gfx.set.idx2diropt {palet_addr} {msb_left} {start_byte_align}
        _CLI_CMD("gfx.set.idx2diropt 0x%08x %u %u", 
            (UINT32)pu1PaleTableAddr, u4MsbLeft, u4StByAl);

        // gfx.op.idx2dirblt {dx} {dy} {w} {h}
        _CLI_CMD("gfx.op.idx2dirblt %u %u %u %u", u4Dx, u4Dy, u4Width, u4Height);
    } //~for
    _CLI_CMD("gfx.flush");
    BSP_FreeAlignedDmaMemory((UINT32)pu1PaleTableAddr);
    Printf("Index to Direct Color BitBlt Test 04 (random, only run hw) .... OK !\n");
    return 0;
}

#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)||defined(CC_MT5398)
//--------------------------------------------------
// index to direct color bitblt test 05 (5368 new feature for set src pitch )
//--------------------------------------------------
static INT32 _GfxEmuCmdIdx2DirBlt5(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height, u4DstPitch, u4MsbLeft, u4StByAl, u4SrcPitch;
    UINT8 *pu1PaleTableAddr = NULL;
    UINT32 i, u4Size, u4Align, u4TableEntryNum, u4Dx, u4Dy;
    UINT32 u4CharCM, u4DstCM, u4Loops = 1;
    UINT32 u4BitsPerSrcPix, u4BytesPerDstPix;
    CHAR *szCharCM;
    CHAR *szDstCM;
    CHAR *szSrcCM;
    //UINT8 u4SrcPitchEn = 1;

    Printf("Index to Direct Color BitBlt Test 05 (random)\n");

    if (i4Argc < 1)
    {
        Printf("please input test  loop num \n");
        return -1;
    }


    //u4Dx      = (UINT32) StrToInt(szArgv[1]);
    //u4Dy      = (UINT32) StrToInt(szArgv[2]);
    //u4Width   = (UINT32) StrToInt(szArgv[3]);
    //u4Height  = (UINT32) StrToInt(szArgv[4]);
    //u4CharCM  = (UINT32) StrToInt(szArgv[5]);
    //u4DstCM   = (UINT32) StrToInt(szArgv[6]);
    //u4MsbLeft = (UINT32) StrToInt(szArgv[7]);
    //u4StByAl  = (UINT32) StrToInt(szArgv[8]);
    //u4Loops   = (UINT32) StrToInt(szArgv[9]);
    u4Loops   = (UINT32) StrToInt(szArgv[1]);

#ifdef GFX_BURST_READ
    u4Align = 64;   // 16 bytes alignment
#else
    u4Align = 16;   // 16 bytes alignment
#endif

    u4Size = (UINT32) sizeof(_u1gfxpt_img_1_clut);
    pu1PaleTableAddr = (UINT8 *)BSP_AllocAlignedDmaMemory(u4Size, u4Align);
    if (pu1PaleTableAddr == NULL)
    {
        Printf("Error : pu1PaleTableAddr is NULL\n");
        return -1;
    }

    for (i = 0; i < u4Size; i++)
    {
        pu1PaleTableAddr[i] = _u1gfxpt_img_1_clut[i];
    }

    u4TableEntryNum = (u4Size / 4);

    Printf("pale_table_entry_num = %u\n", u4TableEntryNum);
    Printf("pale_table_address = 0x%08x\n", (UINT32)pu1PaleTableAddr);



    for (i = 0; i < u4Loops; i++)
    {
        u4Width   = (((UINT32)rand()) % (MAX_SRC_WIDTH - MIN_SRC_WIDTH - 1)) + MIN_SRC_WIDTH;
        u4Height = (((UINT32)rand()) % (MAX_SRC_HEIGHT - MIN_SRC_HEIGHT - 1)) + MIN_SRC_HEIGHT;
        u4CharCM  = 3;  //only support 8 bit plaette mode
        u4DstCM   = CM_RGB565_DIRECT16 + ((UINT32)rand()) % 4;
        u4MsbLeft = 0;
        u4StByAl  = 0;
        u4Dx = 0;
        u4Dy = 0;

        // set dst color mode
        switch (u4DstCM)
        {
            case CM_ARGB8888_DIRECT32:
                u4BytesPerDstPix = 4;
                szDstCM = "argb8888";
                break;

            case CM_ARGB4444_DIRECT16:
                u4BytesPerDstPix = 2;
                szDstCM = "argb4444";
                break;

            case CM_ARGB1555_DIRECT16:
                u4BytesPerDstPix = 2;
                szDstCM = "argb1555";
                break;

            case CM_RGB565_DIRECT16:
                u4BytesPerDstPix = 2;
                szDstCM = "rgb565";
                break;

            default: return -1;
        }

        // set char color mode
        switch (u4CharCM)
        {
            case 0: szCharCM = "1bit"; szSrcCM = "rgb1"; u4BitsPerSrcPix = 1; break;
            case 1: szCharCM = "2bit"; szSrcCM = "rgb2"; u4BitsPerSrcPix = 2; break;
            case 2: szCharCM = "4bit"; szSrcCM = "rgb4"; u4BitsPerSrcPix = 4; break;
            case 3: szCharCM = "8bit"; szSrcCM = "rgb8"; u4BitsPerSrcPix = 8; break;
            default: return -1;
        }

        u4BitsPerSrcPix = u4BitsPerSrcPix;//solve warning

        //u4SrcPitch =  (u4Width * u4BitsPerSrcPix/8);
        u4SrcPitch =  (_u4ScreenWidth * u4BitsPerSrcPix / 8);
        u4SrcPitch = (u4SrcPitch + 0xf) & (~0xf);
        u4DstPitch = (_u4ScreenWidth * u4BytesPerDstPix);
        u4DstPitch = (u4DstPitch + 0xf) & (~0xf);
        // if (((u4BitsPerSrcPix * u4Width) % 8) > 0)
        if (((u4SrcPitch) % 8) > 0)
        {
            u4StByAl = 1;
        }
        else
        {
            u4StByAl = 0;
        }


        Printf("(%4d) : ", i);
        Printf("Dx = %u, Dy = %u, W = %u, H = %u\n", u4Dx, u4Dy, u4Width, u4Height);
        Printf("CharCM = %s, DstCM = %s, MsbLeft = %u, SAddrByteAlign = %u, DstPitch = %u\n",
               szCharCM, szDstCM, u4MsbLeft, u4StByAl, u4DstPitch);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfxemu.cls");

        // HW Action
        // 1. set idx2dir source address
        // 2. set destination address
        // 3. set index to direct color bitblt option
        // 4. do index to direct color bitblt
        // 5. flush

        _GFX_Idx2DirSrcPitchEn(1);
        // gfx.set.idx2dirsrc {buff_base_address} {char_cm}
        _CLI_CMD("gfx.set.idx2dirsrc 0x%08x %s", BUF_ADDR2, szCharCM);

        // gfx.set.dst {buff_base_address} {color_mode} {pitch_in_bytes}
        _CLI_CMD("gfx.set.dst 0x%08x %s %u", BUF_SCREEN, szDstCM, u4DstPitch);

        // gfx.set.idx2diropt {palet_addr} {msb_left} {start_byte_align}
        _CLI_CMD("gfx.set.idx2diropt 0x%08x %u %u",
                 (UINT32)pu1PaleTableAddr, u4MsbLeft, u4StByAl);

        //_GFX_SetSrc((UINT8 *)BUF_ADDR2, szCharCM, u4SrcPitch);  // only use src pitch
        //   _CLI_CMD("gfx.set.src 0x%08x %s %u", BUF_ADDR2, szCharCM, u4SrcPitch);
        _CLI_CMD("gfx.set.src 0x%08x %s %u", BUF_ADDR2, szSrcCM, u4SrcPitch);
        // gfx.op.idx2dirblt {dx} {dy} {w} {h}
        // _CLI_CMD("gfx.op.idx2dirblt %u %u %u %u", u4Dx, u4Dy, u4Width, u4Height);
        _CLI_CMD("gfx.op.idx2dirblt %u %u %u %u", u4Dx, u4Dy, u4Width, u4Height);
        _CLI_CMD("gfx.flush");
        _GFX_Idx2DirSrcPitchEn(0);

        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfxemu.cls");

        // SW Action
        // 1. set idx2dir source address
        // 2. set destination address
        // 3. set index to direct color bitblt option
        // 4. do index to direct color bitblt
        // 5. flush
        _GFX_Idx2DirSrcPitchEn(1);

        // gfx.set.idx2dirsrc {buff_base_address} {char_cm}
        _CLI_CMD("gfx.set.idx2dirsrc 0x%08x %s", BUF_ADDR2, szCharCM);

        // gfx.set.dst {buff_base_address} {color_mode} {pitch_in_bytes}
        _CLI_CMD("gfx.set.dst 0x%08x %s %u", COMPARE_BUF, szDstCM, u4DstPitch);

        // gfx.set.idx2diropt {palet_addr} {msb_left} {start_byte_align}
        _CLI_CMD("gfx.set.idx2diropt 0x%08x %u %u",
                 (UINT32)pu1PaleTableAddr, u4MsbLeft, u4StByAl);

        // _GFX_SetSrc((UINT8 *)BUF_ADDR2, szCharCM, u4SrcPitch);  // only use src pitch
        //_CLI_CMD("gfx.set.src 0x%08x %s %u", BUF_ADDR2, szCharCM, u4SrcPitch);
        _CLI_CMD("gfx.set.src 0x%08x %s %u", BUF_ADDR2, szSrcCM, u4SrcPitch);

        // gfx.op.idx2dirblt {dx} {dy} {w} {h}
        _CLI_CMD("gfx.op.idx2dirblt %u %u %u %u", u4Dx, u4Dy, u4Width, u4Height);


        _CLI_CMD("gfx.flush");
        _GFX_Idx2DirSrcPitchEn(0);
        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                    (UINT32 *)COMPARE_BUF,
                    (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            BSP_FreeAlignedDmaMemory((UINT32)pu1PaleTableAddr);
            Printf("Error: Index to Direct Color BitBlt Test 05 FAIL !!\n");
            return -1;
        }
    } //~for

    BSP_FreeAlignedDmaMemory((UINT32)pu1PaleTableAddr);
    Printf("Index to Direct Color BitBlt Test 05 (5368 src pitch use) .... OK !\n");
    return 0;
}

#endif

//--------------------------------------------------
// horizontal line to vertical line test 01
//--------------------------------------------------
static INT32 _GfxEmuCmdHor2VerLin1(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Sx, u4Sy, u4Len, u4Ccw;

    Printf("Horizontal line to vertical line test 01 (one line)\n");

    if (i4Argc < 5)
    {
        Printf("arg: sx, sy, len, ccw\n");
        return -1;
    }

    u4Sx  = (UINT32) StrToInt(szArgv[1]);
    u4Sy  = (UINT32) StrToInt(szArgv[2]);
    u4Len = (UINT32) StrToInt(szArgv[3]);
    u4Ccw = (UINT32) StrToInt(szArgv[4]);

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _CLI_CMD("gfxemu.cls");

    // HW Action
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
    
    _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[0].u4Color);
    _CLI_CMD("gfx.op.hline %u %u %u", u4Sx, u4Sy, u4Len);
    _CLI_CMD("gfx.flush");

    _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _CLI_CMD("gfx.set.h2vlineopt %d", u4Ccw);
    _CLI_CMD("gfx.op.h2vline %d %d %d %d %d", u4Sx, u4Sy, u4Sx, u4Sy, u4Len);
    _CLI_CMD("gfx.flush");

    Printf("Horizontal line to vertical line test 01 .... OK !\n");
    return 0;
}

//--------------------------------------------------
// horizontal line to vertical line test 02
//--------------------------------------------------
static INT32 _GfxEmuCmdHor2VerLin2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Sx, u4Sy, u4Len, u4Ccw, i, u4Run;
    UINT32 u4Size;

    Printf("Horizontal line to vertical line test 02\n");

    if (i4Argc < 6)
    {
        Printf("arg: sx, sy, len, ccw, loops\n");
        return -1;
    }

    u4Sx  = (UINT32) StrToInt(szArgv[1]);
    u4Sy  = (UINT32) StrToInt(szArgv[2]);
    u4Len = (UINT32) StrToInt(szArgv[3]);
    u4Ccw = (UINT32) StrToInt(szArgv[4]);
    u4Run = (UINT32) StrToInt(szArgv[5]);

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _CLI_CMD("gfxemu.cls");

    _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);

    _CLI_CMD("gfx.set.h2vlineopt %d", u4Ccw);
    for (i = 0; i < u4Run; i++)
    {
        _CLI_CMD("gfx.op.h2vline %d %d %d %d %d", 
            u4Sx, u4Sy + (i * 20), u4Sx + (i * 20), u4Sy, u4Len);
    }
    _CLI_CMD("gfx.flush");


    // SW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");

    _GfxSetDefMode(COMPARE_BUF, ADR_DST);
    _CLI_CMD("gfxemu.cls");

    _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
    _GfxSetDefMode(COMPARE_BUF, ADR_DST);

    _CLI_CMD("gfx.set.h2vlineopt %d", u4Ccw);
    for (i = 0; i < u4Run; i++)
    {
        _CLI_CMD("gfx.op.h2vline %d %d %d %d %d", 
            u4Sx, u4Sy + (i * 20), u4Sx + (i * 20), u4Sy, u4Len);
    }
    _CLI_CMD("gfx.flush");

    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    if (_GfxBufferCompare(
            (UINT32 *)COMPARE_BUF, 
            (UINT32 *)(BUF_SCREEN), u4Size) == -1)
    {
        Printf("Horizontal line to vertical line test 02 .. FAIL ");
        return -1;
    }

    Printf("Horizontal line to vertical line test 02 .... OK !\n");
    return 0;
}

//--------------------------------------------------
// horizontal line to vertical line test 03 (random)
//--------------------------------------------------
static INT32 _GfxEmuCmdHor2VerLin3(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Sx, i4Sy, i4Len, i4Ccw, i, i4Loops, i4Dx, i4Dy;
    UINT32 u4Size;

    Printf("Horizontal line to vertical line test 03 (random)\n");

    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4Loops = (INT32) StrToInt(szArgv[1]);


    for (i = 0; i < i4Loops; i++)
    {
        i4Sx  = ((UINT32)rand()) % _u4ScreenWidth;
        i4Sy  = ((UINT32)rand()) % _u4ScreenHeight;
        i4Dx  = ((UINT32)rand()) % _u4ScreenWidth;
        
        i4Ccw = ((UINT32)rand()) % 2;
        if (i4Ccw == 1)
        {
            while (1)
            {
                i4Dy  = ((UINT32)rand()) % _u4ScreenHeight;
                i4Len = ((UINT32)rand()) % _u4ScreenHeight;

                if (((i4Dy - i4Len) >= 0) && (i4Len > 0))
                {
                    break;
                }
            }
        }
        else
        {
            while (1)
            {
                i4Dy  = ((UINT32)rand()) % _u4ScreenHeight;
                i4Len = ((UINT32)rand()) % _u4ScreenHeight;

                if (((i4Dy + i4Len) <= _u4ScreenHeight) && (i4Len > 0))
                {
                    break;
                }
            }
        }

        Printf("(%4d) : ", i);
        Printf("Sx = %d, Sy = %d, Dx = %d, Dy = %d, Length = %d, Ccw = %d\n",
            i4Sx, i4Sy, i4Dx, i4Dy, i4Len, i4Ccw);
            
        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfxemu.cls");

        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);

        _CLI_CMD("gfx.set.h2vlineopt %d", i4Ccw);
        _CLI_CMD("gfx.op.h2vline %d %d %d %d %d", i4Sx, i4Sy, i4Dx, i4Dy, i4Len);
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfxemu.cls");

        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);

        _CLI_CMD("gfx.set.h2vlineopt %d", i4Ccw);
        _CLI_CMD("gfx.op.h2vline %d %d %d %d %d", i4Sx, i4Sy, i4Dx, i4Dy, i4Len);
        _CLI_CMD("gfx.flush");


        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Horizontal line to vertical line test 03 .. FAIL ");
            return -1;
        }
    } //~for

    Printf("Horizontal line to vertical line test 03 .... OK !\n");
    return 0;
}

//--------------------------------------------------
// horizontal line to vertical line test 04 (random, only run hw)
//--------------------------------------------------
static INT32 _GfxEmuCmdHor2VerLin4(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Sx, i4Sy, i4Len, i4Ccw, i, i4Loops, i4Dx, i4Dy;

    Printf("Horizontal line to vertical line test 04 (random, only run hw) .. \n");

    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4Loops = (INT32) StrToInt(szArgv[1]);


    for (i = 0; i < i4Loops; i++)
    {
        i4Sx  = ((UINT32)rand()) % _u4ScreenWidth;
        i4Sy  = ((UINT32)rand()) % _u4ScreenHeight;
        i4Dx  = ((UINT32)rand()) % _u4ScreenWidth;
        
        i4Ccw = ((UINT32)rand()) % 2;
        if (i4Ccw == 1)
        {
            while (1)
            {
                i4Dy  = ((UINT32)rand()) % _u4ScreenHeight;
                i4Len = ((UINT32)rand()) % _u4ScreenHeight;

                if (((i4Dy - i4Len) >= 0) && (i4Len > 0))
                {
                    break;
                }
            }
        }
        else
        {
            while (1)
            {
                i4Dy  = ((UINT32)rand()) % _u4ScreenHeight;
                i4Len = ((UINT32)rand()) % _u4ScreenHeight;

                if (((i4Dy + i4Len) <= _u4ScreenHeight) && (i4Len > 0))
                {
                    break;
                }
            }
        }

        Printf("(%4d) : ", i);
        Printf("Sx = %d, Sy = %d, Dx = %d, Dy = %d, Length = %d, Ccw = %d\n",
            i4Sx, i4Sy, i4Dx, i4Dy, i4Len, i4Ccw);

        // Action
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);

        _CLI_CMD("gfx.set.h2vlineopt %d", i4Ccw);
        _CLI_CMD("gfx.op.h2vline %d %d %d %d %d", i4Sx, i4Sy, i4Dx, i4Dy, i4Len);
        _CLI_CMD("gfx.flush");
    } //~for
    _CLI_CMD("gfx.flush");
    Printf("Horizontal line to vertical line test 04 (random, only run hw) .... OK !\n");
    return 0;
}


//--------------------------------------------------
// sub-picture decode test 01 (for dvd-subtitle data)
//--------------------------------------------------
static INT32 _GfxEmuCmdSubPicDec1(INT32 i4Argc, const CHAR ** szArgv)
{
    //UINT32 u4Width, u4Height, u4DstPitch, u4MsbLeft, u4StByAl;
    UINT32 u4Width, u4Height,u4Size;
   // UINT8 *pu1SubPicTableAddr = NULL;
    //UINT32 i, u4Size, u4Align, u4TableEntryNum;
    UINT32 au4ColorMap[4], u4CM;
    UINT32 u4Pitch;
    
    Printf("Sub-picture Decode Test 01\n");

    if (i4Argc < 4)
    {
        Printf("arg: width, height, dst_cm_idx\n");
        return -1;
    }

    u4Width  = (UINT32) StrToInt(szArgv[1]);
    u4Height = (UINT32) StrToInt(szArgv[2]);
    u4CM     = (UINT32) StrToInt(szArgv[3]);

    // prepare sub-picture & colormap table
    {
        u4Size = (UINT32) sizeof(_u1GfxEmuJkaraokeSpTable);
        Printf("size of sub_pic_table = %u\n", u4Size);
        Printf("src_w = %u, src_h = %u\n", u4Width, u4Height);
        Printf("sub_pic_table_address = 0x%08x\n", 
            (UINT32)_u1GfxEmuJkaraokeSpTable);

        if (u4CM == CM_ARGB8888_DIRECT32)
        {
            au4ColorMap[0] = 0xFF000000;
            au4ColorMap[1] = 0xFFFF0000;
            au4ColorMap[2] = 0xFF00FF00;
            au4ColorMap[3] = 0xFF0000FF;
        }
        else
        {
            au4ColorMap[0] = 0xF000;
            au4ColorMap[1] = 0xFF00;
            au4ColorMap[2] = 0xF0F0;
            au4ColorMap[3] = 0xF00F;
        }
    }

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _CLI_CMD("gfxemu.cls");

    // HW Action
    // 1. set destination address
    // 2. set sub-picture source address
    // 3. set sub-pic-decode flag
    // 4. do txt/bmp mapping
    // 5. flush
    #ifdef GFX_BURST_READ
        u4Pitch = GFX_BURST_READ_ALIGN(_u4GfxPitch);
    #else
        u4Pitch = _u4GfxPitch;
    #endif
    _GFX_SetDst((UINT8*)BUF_SCREEN, u4CM, u4Pitch);

    _GFX_SetBmpSrc(_u1GfxEmuJkaraokeSpTable, E_BMP_CM_2BIT);

    _GFX_SetColorMap(E_BMP_CM_2BIT, u4CM, au4ColorMap);
        
    _GFX_SetBmpOpt(TRUE, TRUE);

    _GFX_BmpBlt(0, 0, u4Width, u4Height);

    _GFX_SetBmpOpt(FALSE, FALSE);
    
    _CLI_CMD("gfx.flush");

    Printf("Sub-picture Decode Test 01 .... OK !\n");
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdCompress1
 *  Compression BitBlt Test 1
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCompress1(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height;
    UINT32 u4SrcAddr, u4DstAddr, u4SrcPitch, u4DstPitch, u4RollBackEn, MirrorFlip, line_separate;

    Printf("Compression BitBlt Test 1 ...");
    
    if (i4Argc < 7)
    {
        Printf("arg: SrcAddr, SrcPitch, DstAddr, Width, Height, aMethod\n");
        return -1;
    }

    u4SrcAddr  = (UINT32) StrToInt(szArgv[1]);
    u4SrcPitch = (UINT32) StrToInt(szArgv[2]);
    u4DstAddr  = (UINT32) StrToInt(szArgv[3]);
    u4Width    = (UINT32) StrToInt(szArgv[4]);
    u4Height   = (UINT32) StrToInt(szArgv[5]);
    u4RollBackEn  = (UINT32) StrToInt(szArgv[6]);
    MirrorFlip   = (UINT32) StrToInt(szArgv[7]);
    line_separate  = (UINT32) StrToInt(szArgv[8]);

    //u4DstPitch = u4SrcPitch / 2;

    Printf("Src = 0x%08x, sPitch = %d, Dst = 0x%08x, dPitch = %d, W = %d, H = %d, AMethod = %d\n",
            u4SrcAddr, u4SrcPitch, u4DstAddr, u4Width, u4Height);

            if(line_separate == 1)
            {
                UINT32 original_line_bit = u4Width * 4 *8;
                
                UINT32 line_bit;
                if(u4RollBackEn)
                    line_bit = original_line_bit+4;
                else
                    line_bit = original_line_bit + 4 + original_line_bit/16;
                    line_bit = (line_bit%128 ==0)? line_bit : (line_bit/128 +1)*128;

                u4DstPitch = line_bit/8;  
                
                printf("width : %d\n",u4Width);                
            }
            else
            {
                u4DstPitch = u4SrcPitch;
            }

    // Action
    _GFX_SetDst((UINT8 *)u4DstAddr, CM_ARGB8888_DIRECT32, u4DstPitch);
    _GFX_SetColor(0x0);
    _GFX_Fill(0, 0, u4Width, u4Height);
    _CLI_CMD("gfx.flush");

    _GFX_NewCompressBlt((UINT8 *)u4SrcAddr, 0, 0, CM_ARGB8888_DIRECT32, u4SrcPitch, 
                (UINT8 *)u4DstAddr, u4Width, u4Height, u4Height, u4RollBackEn, 0, 0xffffffff, MirrorFlip, 0, line_separate,u4DstPitch);

    _CLI_CMD("gfx.flush");

    Printf("Compression BitBlt Test 1 ... OK!!");
    return 0;
}



//-------------------------------------------------------------------------
/** _GfxEmuCmdCompress1
 *  Compression BitBlt Test 2 / new compression operation
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCompress3(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height;
    UINT32 u4RollBackEn, u4QualityMode, u4BpCompAddrEnd, u4ColorMode;
    UINT32 u4SrcAddr, u4DstAddr, u4SrcPitch, u4DstPitch;
    BOOL fgMirrorFlip;
//    char* pcFileInfo;
//    UINT32 i,u4Times;
//    char* pcToken;
//    UINT32 file_size;
    Printf("Compression BitBlt Test 2 ...");
    
    if (i4Argc < 10)
    {
        Printf("arg: SrcAddr, SrcPitch, DstAddr, Width, Height, RollBackEn, QualityMode, BpCompAddrEnd, ColorMode\n");
        return -1;
    }

    u4SrcAddr     = (UINT32) StrToInt(szArgv[1]);
    u4SrcPitch    = (UINT32) StrToInt(szArgv[2]);
    u4DstAddr     = (UINT32) StrToInt(szArgv[3]);
    u4Width       = (UINT32) StrToInt(szArgv[4]);
    u4Height      = (UINT32) StrToInt(szArgv[5]);
    u4RollBackEn  = (UINT32) StrToInt(szArgv[6]);
    u4QualityMode = (UINT32) StrToInt(szArgv[7]);
    u4BpCompAddrEnd = (UINT32) StrToInt(szArgv[8]);
    u4ColorMode = (UINT32) StrToInt(szArgv[9]);
    fgMirrorFlip = FALSE;

    u4DstPitch = u4SrcPitch;

    Printf("Src = 0x%08x, sPitch = %d, Dst = 0x%08x, dPitch = %d, W = %d, H = %d, u4RollBackEn = %d,u4QualityMode = %d, u4BpCompAddrEnd = 0x%08x, u4ColorMode = %d\n",
            u4SrcAddr, u4SrcPitch, u4DstAddr, u4DstPitch, u4Width, u4Height, u4RollBackEn,u4QualityMode,u4BpCompAddrEnd,u4ColorMode);

    if(fgMirrorFlip)
    {
        UINT32 u4BytePixel;
        if(u4ColorMode == 10 || u4ColorMode ==2)
            u4BytePixel = 1;
        else if(u4ColorMode == 13)
            u4BytePixel = 2;
        else if(u4ColorMode==14 || u4ColorMode==6)
            u4BytePixel = 4;
        else
            ASSERT(0);
        u4SrcAddr += u4Width*u4Height*u4BytePixel - 8;
        Printf("u4SrcAddr %08x\n",u4SrcAddr);
    }
    // Action
    _GFX_SetDst((UINT8 *)u4DstAddr, u4ColorMode, u4DstPitch);
    _GFX_SetColor(0x0);
    _GFX_Fill(0, 0, u4Width, u4Height);
        
    _GFX_NewCompressBlt((UINT8 *)u4SrcAddr, 0, 0, u4ColorMode, u4SrcPitch, 
        (UINT8 *)u4DstAddr, u4Width, u4Height, u4Height, u4RollBackEn,u4QualityMode,u4BpCompAddrEnd,fgMirrorFlip,0,0,u4DstPitch);
    _CLI_CMD("gfx.flush");

    Printf("Compression BitBlt Test 2 ... OK!!");
    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdCompress1
 *  Compression BitBlt Test 2 / new compression operation
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCompress2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height;
    UINT32 u4RollBackEn, u4QualityMode, u4BpCompAddrEnd, u4ColorMode;
    UINT32 u4SrcAddr, u4DstAddr, u4SrcPitch, u4DstPitch=0;
    BOOL fgMirrorFlip;
    BOOL fgBurstRead;
    char* pcFileInfo;
    UINT32 i,u4Times,j;
    char* pcToken;
    UINT32 file_size1,file_size2,u4BytesPixel=1;
    UINT32 FileSize;
    UINT32 u4InitValue =0;
    UINT32 index =0;
    //UINT32 u4RepeatFrame =9999;
    UINT8* SrcAddr = NULL;
    UINT8* DstAddr = NULL;
    UINT8* golden_data1 = NULL;
    UINT8* golden_data2 = NULL;
    UINT8* GoldenData = NULL;
    UINT8* hw_address = NULL;
    //BOOL fgFirstTime = TRUE;
    UINT32 u4FrameSize = 1920*1200*4;

    DstAddr = (UINT8*)_u4GfxEmuBuf[0];
    golden_data1 = DstAddr + u4FrameSize;
    SrcAddr = golden_data1 + u4FrameSize;
    golden_data2 = SrcAddr + u4FrameSize;

    hw_address = DstAddr;
    
    Printf("Compression BitBlt Test 2 ...");

    u4InitValue =  (UINT32) StrToInt(szArgv[1]);
    u4Times =  (UINT32) StrToInt(szArgv[2]);
    //u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);

    fgMirrorFlip = FALSE;
    
    pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);

    for(i=u4InitValue ; i<u4Times ; i++)
    {
        int line_separate =0;
        //if(i == (u4RepeatFrame+1))
        //    i--;
        UINT32 file_data[10] = {0};

        index =0;
        
        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_info_%d.txt\" 0x%08x /ny\n",i,pcFileInfo);
	    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_info_%d.txt\" 0x%08x /ny", 
		i,pcFileInfo));

		//Printf("string : %s\n",pcFileInfo);
        //sscanf(pcFileInfo,"%s %s %s %s %s %s",&file_data[0],&file_data[1],&file_data[2],&file_data[3],&file_data[4],&file_data[5]);

        pcToken = strtok(pcFileInfo, " ");
        while ( pcToken != NULL)
        {
            file_data[index++] = StrToInt(pcToken);
            //Printf("bitstream_sz : %d \n",bitstream_sz[u2file_num-1]);
            pcToken = strtok(NULL, " ");
        }

        u4SrcAddr     = (UINT32) SrcAddr;//(UINT32) 0x01ea4000;
        u4DstAddr     = (UINT32) DstAddr;
        u4Width       = (UINT32) file_data[0];
        u4Height      = (UINT32) file_data[1];
        u4ColorMode = (UINT32) file_data[2];
        u4QualityMode = (UINT32) file_data[3];
        u4RollBackEn  = (UINT32) file_data[4];
        file_size1 = (UINT32) file_data[5];
        fgBurstRead = (BOOL) file_data[6];
        file_size2 = (UINT32) file_data[7];
        u4BpCompAddrEnd = 0xffffffff;
        //fgBurstRead = (BOOL)((UINT32)rand()%2);
        u4SrcPitch =0;

        #if 0
        if(u4ColorMode ==11 || u4ColorMode ==12)
            continue;
        #endif
        //if((i>=100) && u4RollBackEn ==1)
        //    continue;
        //Printf("Src = 0x%08x, sPitch = %d, Dst = 0x%08x, dPitch = %d, W = %d, H = %d, u4RollBackEn = %d,u4QualityMode = %d, u4BpCompAddrEnd = 0x%08x, u4ColorMode = %d\n file_size : %d\n",
        //    u4SrcAddr, u4SrcPitch, u4DstAddr, u4DstPitch, u4Width, u4Height, u4RollBackEn,u4QualityMode,u4BpCompAddrEnd,u4ColorMode,file_size);

        //if(u4ColorMode !=10 && u4ColorMode != 14 && u4ColorMode !=13) {
        //    continue;
        //}
        
        if (u4ColorMode == 2 || u4ColorMode == 10)
        {
            u4BytesPixel = 1;
        }
        else if (u4ColorMode == 6 || u4ColorMode == 14)
        {
            u4BytesPixel = 4; 
        }
        else if (u4ColorMode == 13 || u4ColorMode == 12 || u4ColorMode == 11)
        {
            u4BytesPixel = 2;
        }
        else
        {
            ASSERT(0);
        }
        
        u4SrcPitch    = u4Width * u4BytesPixel;

        
        //if(fgFirstTime) {
            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_%d.txt\" 0x%08x /ny\n",i,u4SrcAddr);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_%d.txt\" 0x%08x /ny", 
    		(i),u4SrcAddr));
    		//x_thread_delay(1000);
    		
            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx\\emu_bitstream_line_connect_%d.txt\" 0x%08x /ny\n",(i),golden_data1);
    		UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx\\emu_bitstream_line_connect_%d.txt\" 0x%08x /ny", 
    		(i),golden_data1));

    		LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx\\emu_bitstream_line_separate_%d.txt\" 0x%08x /ny\n",(i),golden_data2);
    		UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx\\emu_bitstream_line_separate_%d.txt\" 0x%08x /ny", 
    		(i),golden_data2));
    		//x_thread_delay(1000);
		//}
        
        Printf("Src = 0x%08x, sPitch = %d, Dst = 0x%08x, dPitch = %d, W = %d, H = %d, u4RollBackEn = %d,u4QualityMode = %d, u4BpCompAddrEnd = 0x%08x, u4ColorMode = %d fgBurstRead = %d\n",
            u4SrcAddr, u4SrcPitch, u4DstAddr, u4DstPitch, u4Width, u4Height, u4RollBackEn,u4QualityMode,u4BpCompAddrEnd,u4ColorMode,fgBurstRead);

#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        //_GFX_SetSrc((UINT8 *)u4SrcAddr, u4ColorMode, u4SrcPitch);



        for(line_separate =0; line_separate<2 ;line_separate++)
        {
            //printf("\nclear dst buffer\n");
            _GFX_SetDst((UINT8 *)u4DstAddr, 14, 1920*4);
            _GFX_SetColor(0x0);
            _GFX_Fill(0, 0, 1920, 1200);
            _CLI_CMD("gfx.flush");
            //printf("\nclear dst buffer\n");
            
            //line-separate mode
            if(line_separate == 1)
            {
                UINT32 original_line_bit = u4Width * u4BytesPixel *8;
                
                UINT32 line_bit;
                if(u4RollBackEn)
                    line_bit = original_line_bit+4;
                else
                    line_bit = original_line_bit + 4 + original_line_bit/16;
                if(fgBurstRead)
                    line_bit = (line_bit%512 ==0)? line_bit : (line_bit/512 +1)*512;
                else
                    line_bit = (line_bit%128 ==0)? line_bit : (line_bit/128 +1)*128;

                u4DstPitch = line_bit/8;
                //u4SrcPitch = line_bit/8;
                //u4Width = line_bit/8;
                //u4Width /= u4BytesPixel;
                
                printf("width : %d\n",u4Width);

                GoldenData = golden_data2;
                FileSize = file_size2;
            }
            else
            {
                u4DstPitch = u4SrcPitch;
                GoldenData = golden_data1;
                FileSize = file_size1;
            }
            //printf("\nnew osd compression\n");
            _GFX_NewCompressBlt((UINT8 *)u4SrcAddr, 0, 0, u4ColorMode, u4SrcPitch, 
                (UINT8 *)u4DstAddr, u4Width, u4Height, u4Height, u4RollBackEn,u4QualityMode,u4BpCompAddrEnd,fgMirrorFlip,fgBurstRead,line_separate,u4DstPitch);
            //printf("\nnew osd compression\n");

            _CLI_CMD("gfx.flush");

            //LOG(0,"file_size : %d\n",file_size);
            HalFlushInvalidateDCache();
            for (j = 0; j < FileSize ; j++)
            {
                if (hw_address[j] != GoldenData[j])
                {
                    printf("hw : %x, golden : %x, j = %d\n",hw_address[j],GoldenData[j],j);
                    printf("compare error in emu_source_%d.txt %d\n",i,line_separate);
                    printf("BPCOMP_CHKSUM : %x\n",IO_READ32(GFX_ADDR, (0xb8)));
                    printf("BPCOMP_NIPPLE : %x\n",IO_READ32(GFX_ADDR, (0xb4)));
                    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\emu_source_%d_error_%d.txt\" 0x%08x--0x%08x", 
        		(i),(line_separate),(UINT32)hw_address,(UINT32)hw_address+FileSize-1));
                    return 1;
                    //break;
                }
                else
                {
                    //LOG(0,"hw : %x , golden : %x\n",hw_address[j],golden_dataY[j]);
                }
            }
        }
        printf("compare ok in emu_source_%d.txt\n\n",i);
        //fgFirstTime = FALSE;
    }

    return 0;
}




//-------------------------------------------------------------------------
/** _GfxEmuCmdOnePassCompression
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdOnePassCompression(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height;
    UINT32 u4SrcAddr, u4DstAddr, u4SrcPitch, u4DstPitch,u4ThirdAddr,u4ThirdPitch;
    UINT32 u4SrcCM,u4DstCM,u4ThirdCM;
    char* pcFileInfo;
    char* CompressionInfo;
    UINT32 i,u4Times,j;
    char* pcToken;
    UINT32 u4InitValue =0;
    UINT32 index =0;
    //UINT32 u4RepeatFrame =9999;

    UINT8* hw_address = NULL;
    UINT8* SrcAddr = NULL;
    UINT8* ThirdAddr = NULL;
    UINT8* SrcPallete = NULL;
    UINT8* ThirdPallete = NULL;
    UINT8* golden_data1 = NULL;
    UINT8* golden_data2 = NULL;
    UINT8* DstAddr = NULL;
    //BOOL fgFirstTime = TRUE;

    UINT32 u4BytesPixel=1,u4RollBackEn,fgBurstRead,FileSize,file_size2,file_size1,u4QualityMode;
    UINT8* GoldenData;
    UINT32 u4FrameSize = (1920 * 1500*4);
    //UINT32 u4FrameSize = (10*1024*1024);
    
    UINT32 u4GlobalAlpha,u4Opcode,u4AlcomAr,u4SrcPallte,u4ThirdPallete,u4GoldenSize;
    BOOL fgAlcomNormal,fgThirdEn,fgAlcomDiffCM;
    //golden_dataY = (UINT8*)BSP_AllocAlignedDmaMemory((1920 * 1088*4), 1024);

    #if 0
    DstAddr = BSP_AllocAlignedDmaMemory((1920 * 1200*4), 1024);
    golden_dataY = BSP_AllocAlignedDmaMemory((1920 * 1200*4), 1024);
    SrcAddr = BSP_AllocAlignedDmaMemory((1920 * 1200*4), 1024);
    ThirdAddr = BSP_AllocAlignedDmaMemory((1920 * 1200*4), 1024);
    #else
    DstAddr = (UINT8*)_u4GfxEmuBuf[0];
    golden_data1 = DstAddr + u4FrameSize;
    SrcAddr = golden_data1 + u4FrameSize;
    ThirdAddr = SrcAddr + u4FrameSize;
    golden_data2 = ThirdAddr + u4FrameSize;
    //SrcPallete = ThirdAddr + 1024;
    //ThirdPallete = SrcPallete + 1024;
    #endif
    
    SrcPallete = (UINT8 *)BSP_AllocAlignedDmaMemory((1024), 1024);
    ThirdPallete = (UINT8 *)BSP_AllocAlignedDmaMemory((1024), 1024);

    if(SrcAddr == NULL || ThirdAddr==NULL || SrcPallete==NULL || ThirdPallete==NULL || DstAddr==NULL || golden_data1==NULL || golden_data2==NULL)
    {
        ASSERT(0);
    }
    
    Printf("General Alpha Composition Stress Test ...");

    u4InitValue =  (UINT32) StrToInt(szArgv[1]);
    u4Times =  (UINT32) StrToInt(szArgv[2]);
    //u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);

    pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);
    CompressionInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);

    for(i=u4InitValue ; i<u4Times ; i++)
    {
        //if(i == (u4RepeatFrame+1))
        //    i--;
        UINT32 line_separate =0;
        UINT32 file_data[21] = {0};
        UINT32 Compression_data[4] ={0};

        index =0;
        
        x_memset((void*)pcFileInfo, 0, 0x40000);
        x_memset((void*)CompressionInfo, 0, 0x40000);
        
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx3\\emu_setup_%d.txt\" 0x%08x /ny\n",i,pcFileInfo);
	    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx3\\emu_setup_%d.txt\" 0x%08x /ny", 
		i,pcFileInfo));
		
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx3\\emu_info_%d.txt\" 0x%08x /ny\n",i,CompressionInfo);
	    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx3\\emu_info_%d.txt\" 0x%08x /ny", 
		i,CompressionInfo));

		//Printf("string : %s\n",pcFileInfo);
        //sscanf(pcFileInfo,"%s %s %s %s %s %s",&file_data[0],&file_data[1],&file_data[2],&file_data[3],&file_data[4],&file_data[5]);

        pcToken = strtok(pcFileInfo, "\r\n");
        while( pcToken != NULL) 
        { 
            file_data[index++] = StrToInt(pcToken);
            pcToken = strtok(NULL, "\r\n");
        }

        u4SrcAddr = (UINT32)SrcAddr;
        u4DstAddr = (UINT32)DstAddr;
        u4ThirdAddr = (UINT32)ThirdAddr;
        u4SrcPitch = (UINT32)file_data[2]*16;
        u4DstPitch = (UINT32)file_data[3]*16;
        u4SrcCM = (UINT32)file_data[4];
        u4Height = (UINT32)file_data[5]+1;
        u4Width = (UINT32)file_data[6];
        u4GlobalAlpha = (UINT32)file_data[7];
        fgAlcomNormal = (BOOL)file_data[8];
        u4Opcode = file_data[9];
        u4AlcomAr = file_data[10];
        u4SrcPallte = (UINT32)SrcPallete;
        u4ThirdAddr = (UINT32)ThirdAddr;
        u4ThirdPallete = (UINT32)ThirdPallete;
        u4ThirdPitch = file_data[14]*16;
        u4ThirdCM = file_data[15];
        fgThirdEn = (BOOL)file_data[16];
        fgAlcomDiffCM = (BOOL)file_data[17];
        u4DstCM = (UINT32)file_data[19];
        u4GoldenSize = file_data[20];

	//wanring solve
	u4SrcPallte =u4SrcPallte;
	u4ThirdPallete = u4ThirdPallete;
	u4GoldenSize=u4GoldenSize;
	fgAlcomDiffCM=fgAlcomDiffCM;
	
        //if(u4DstCM !=12 && u4DstCM !=13 )
        //    continue;

        //can't support not third enable in one pass compression
        if(!fgThirdEn)
            continue;
            
        pcToken = strtok(CompressionInfo, " ");
        index =0;
        while( pcToken != NULL) 
        { 
            Compression_data[index++] = StrToInt(pcToken);
            pcToken = strtok(NULL, " ");
        }

        u4QualityMode = Compression_data[0];
        fgBurstRead = Compression_data[1];
        file_size1 = Compression_data[2];
        file_size2 = Compression_data[3];
        u4RollBackEn = 0;

        if (u4DstCM == 2 || u4DstCM == 10)
        {
            u4BytesPixel = 1;
        }
        else if (u4DstCM == 6 || u4DstCM == 14)
        {
            u4BytesPixel = 4; 
        }
        else if (u4DstCM == 13 || u4DstCM == 12 || u4DstCM == 11)
        {
            u4BytesPixel = 2;
        }
        else
        {
            ASSERT(0);
        }
        
        //if(fgFirstTime) {
            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx3\\random_src_bmp_%d.raw\" 0x%08x /ny\n",i,u4SrcAddr);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx3\\random_src_bmp_%d.raw\" 0x%08x /ny", 
    		(i),u4SrcAddr));

            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx3\\random_src_plt_%d.raw\" 0x%08x /ny\n",i,SrcPallete);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx3\\random_src_plt_%d.raw\" 0x%08x /ny", 
    		(i),SrcPallete));    		

            if(fgThirdEn)
            {
                LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx3\\random_thd_bmp_%d.raw\" 0x%08x /ny\n",i,ThirdAddr);
                UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx3\\random_thd_bmp_%d.raw\" 0x%08x /ny", 
    		    (i),ThirdAddr));

                LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx3\\random_thd_plt_%d.raw\" 0x%08x /ny\n",i,ThirdPallete);
                UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx3\\random_thd_plt_%d.raw\" 0x%08x /ny", 
    		    (i),ThirdPallete));
    		}
    		else
    		{
    		    LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx3\\random_thd_bmp_%d.raw\" 0x%08x /ny\n",i,ThirdAddr);
                UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx3\\random_thd_bmp_%d.raw\" 0x%08x /ny", 
    		    (i),DstAddr));

                //LOG(0, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny\n",i,ThirdPallete);
                //UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny", 
    		    //(i),ThirdPallete));
    		}
    		
            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx3\\emu_bitstream_line_connect_%d.txt\" 0x%08x /ny\n",(i),golden_data1);
    		UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx3\\emu_bitstream_line_connect_%d.txt\" 0x%08x /ny", 
    		(i),golden_data1));

    		LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx3\\emu_bitstream_line_separate_%d.txt\" 0x%08x /ny\n",(i),golden_data2);
    		UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx3\\emu_bitstream_line_separate_%d.txt\" 0x%08x /ny", 
    		(i),golden_data2));

		//}

        //Printf("Src = 0x%08x, sPitch = %d, Dst = 0x%08x, dPitch = %d, W = %d, H = %d, u4RollBackEn = %d,u4QualityMode = %d, u4BpCompAddrEnd = 0x%08x, u4ColorMode = %d fgBurstRead = %d\n",
        //    u4SrcAddr, u4SrcPitch, u4DstAddr, u4DstPitch, u4Width, u4Height, u4RollBackEn,u4QualityMode,u4BpCompAddrEnd,u4ColorMode,fgBurstRead);

        //printf("%d %d %d\n",u4SrcCM,u4DstCM,u4ThirdCM);
        
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        #if 1
        for(line_separate =0; line_separate<2 ;line_separate++)
         {
             //_CLI_CMD("gfx.reset");
             //_CLI_CMD("gfxemu.mode");
             
             //x_memset(u4DstAddr,0,10*1024*1024);
             _GFX_SetDst((UINT8 *)u4DstAddr, 14, 1920*4);
             _GFX_SetColor(0x0);
             _GFX_Fill(0, 0, 1920, 1500);
             _CLI_CMD("gfx.flush");
             
             //line-separate mode
             if(line_separate == 1)
             {
                 UINT32 original_line_bit = u4Width * u4BytesPixel *8;
                 
                 UINT32 line_bit;
                 if(u4RollBackEn)
                     line_bit = original_line_bit+4;
                 else
                     line_bit = original_line_bit + 4 + original_line_bit/16;
                 if(fgBurstRead)
                     line_bit = (line_bit%512 ==0)? line_bit : (line_bit/512 +1)*512;
                 else
                     line_bit = (line_bit%128 ==0)? line_bit : (line_bit/128 +1)*128;
        
                 u4DstPitch = line_bit/8;
                 //u4SrcPitch = line_bit/8;
                 //u4Width = line_bit/8;
                 //u4Width /= u4BytesPixel;
                 
                 printf("width : %d\n",u4Width);
        
                 GoldenData = golden_data2;
                 FileSize = file_size2;
             }
             else
             {
                 //u4DstPitch = u4SrcPitch;
                 GoldenData = golden_data1;
                 FileSize = file_size1;
             }

             _GFX_SetSrc((UINT8 *)u4SrcAddr, u4SrcCM, u4SrcPitch);
             _GFX_SetThird((UINT8 *)u4ThirdAddr, u4ThirdCM, u4ThirdPitch);
             
             _GFX_SetDst((UINT8 *)u4DstAddr, u4DstCM, u4DstPitch);
             
             GFX_SetAlpha(u4GlobalAlpha);
             
             _GFX_SetCharCM(TRUE,E_BMP_CM_8BIT);
             _GFX_SetCharCM(FALSE,E_BMP_CM_8BIT);
             
             _GFX_SetThirdPallete(ThirdPallete,0,0);
             _GFX_SetIdx2DirOpt(SrcPallete,0,0);
             
             _GFX_SetCompressionOpt(line_separate,u4QualityMode,u4RollBackEn);
             
             _GFX_GeneralAlphaCom(u4Width, u4Height,u4AlcomAr, 
                 u4Opcode, 0,fgThirdEn,fgAlcomNormal,TRUE);

             hw_address = (UINT8*)u4DstAddr;
            
             _CLI_CMD("gfx.flush");
        
             //LOG(0,"file_size : %d\n",file_size);
             HalFlushInvalidateDCache();
            for (j = 0; j < FileSize ; j++)
            {
                if (hw_address[j] != GoldenData[j])
                {
                     printf("hw : %x, golden : %x, j = %d\n",hw_address[j],GoldenData[j],j);
                     printf("compare error in emu_source_%d.txt %d\n",i,line_separate);
                     printf("BPCOMP_CHKSUM : %x\n",IO_READ32(GFX_ADDR, (0xb8)));
                     printf("BPCOMP_NIPPLE : %x\n",IO_READ32(GFX_ADDR, (0xb4)));
                     UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\emu_source_onepass_%d_error_%d.txt\" 0x%08x--0x%08x", 
                 (i),(line_separate),(UINT32)hw_address,(UINT32)hw_address+FileSize-1));
                     return 1;
                     //break;
                 }
                else
                {
                     //LOG(0,"hw : %x , golden : %x\n",hw_address[j],golden_dataY[j]);
                 }
             }
         }
        #endif

        printf("New alpha composition comparing ok in emu_source_%d.txt\n\n",i);
        //fgFirstTime = FALSE;
    }

    return 0;
}

//-------------------------------------------------------------------------
/** _GfxEmuCmdGeneralAlcom
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdGeneralAlcom(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height;
    UINT32 u4SrcAddr, u4DstAddr, u4SrcPitch, u4DstPitch,u4ThirdAddr,u4ThirdPitch;
    UINT32 u4SrcCM,u4DstCM,u4ThirdCM;
    char* pcFileInfo;
    UINT32 i,u4Times;
    char* pcToken;
    UINT32 u4InitValue =0;
    UINT32 index =0;
    //UINT32 u4RepeatFrame =9999;
	UINT32 u4Size;
    UINT8* hw_address = NULL;
    UINT8* SrcAddr = NULL;
    UINT8* ThirdAddr = NULL;
    UINT8* SrcPallete = NULL;
    UINT8* ThirdPallete = NULL;
    UINT8* golden_dataY = NULL;
    UINT8* DstAddr = NULL;
    //BOOL fgFirstTime = TRUE;
    UINT32 u4FrameSize = (1920 * 1200*4);
    
    UINT32 u4GlobalAlpha,u4Opcode,u4AlcomAr,u4SrcPallte,u4ThirdPallete;
    BOOL fgAlcomNormal,fgThirdEn,fgAlcomDiffCM;
    //golden_dataY = (UINT8*)BSP_AllocAlignedDmaMemory((1920 * 1088*4), 1024);

    #if 0
    DstAddr = BSP_AllocAlignedDmaMemory((1920 * 1200*4), 1024);
    golden_dataY = BSP_AllocAlignedDmaMemory((1920 * 1200*4), 1024);
    SrcAddr = BSP_AllocAlignedDmaMemory((1920 * 1200*4), 1024);
    ThirdAddr = BSP_AllocAlignedDmaMemory((1920 * 1200*4), 1024);
    #else
    DstAddr = (UINT8*)_u4GfxEmuBuf[0];
    golden_dataY = DstAddr + u4FrameSize;
    SrcAddr = golden_dataY + u4FrameSize;
    ThirdAddr = SrcAddr + u4FrameSize;
    //SrcPallete = ThirdAddr + 1024;
    //ThirdPallete = SrcPallete + 1024;
    #endif
    
    SrcPallete = (UINT8 *)BSP_AllocAlignedDmaMemory((1024), 1024);
    ThirdPallete = (UINT8 *)BSP_AllocAlignedDmaMemory((1024), 1024);

    if(SrcAddr == NULL || ThirdAddr==NULL || SrcPallete==NULL || ThirdPallete==NULL || DstAddr==NULL || golden_dataY==NULL)
    {
        ASSERT(0);
    }
    
    Printf("General Alpha Composition Stress Test ...");

    u4InitValue =  (UINT32) StrToInt(szArgv[1]);
    u4Times =  (UINT32) StrToInt(szArgv[2]);
    //u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);

    pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);

    for(i=u4InitValue ; i<u4Times ; i++)
    {
        //if(i == (u4RepeatFrame+1))
        //    i--;
        UINT32 file_data[21] = {0};

        index =0;
        
        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\emu_setup_%d.txt\" 0x%08x /ny\n",i,pcFileInfo);
		 UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\emu_setup_%d.txt\" 0x%08x /ny", 
		i,pcFileInfo));

		//Printf("string : %s\n",pcFileInfo);
        //sscanf(pcFileInfo,"%s %s %s %s %s %s",&file_data[0],&file_data[1],&file_data[2],&file_data[3],&file_data[4],&file_data[5]);

        pcToken = strtok(pcFileInfo, "\r\n");
        while( pcToken != NULL) 
        { 
            file_data[index++] = StrToInt(pcToken);
            pcToken = strtok(NULL, "\r\n");
        }

        u4SrcAddr = (UINT32)SrcAddr;
        u4DstAddr = (UINT32)DstAddr;
        u4ThirdAddr = (UINT32)ThirdAddr;
        u4SrcPitch = (UINT32)file_data[2]*16;
        u4DstPitch = (UINT32)file_data[3]*16;
        u4SrcCM = (UINT32)file_data[4];
        u4Height = (UINT32)file_data[5]+1;
        u4Width = (UINT32)file_data[6];
        u4GlobalAlpha = (UINT32)file_data[7];
        fgAlcomNormal = (BOOL)file_data[8];
        u4Opcode = file_data[9];
        u4AlcomAr = file_data[10];
        u4SrcPallte = (UINT32)SrcPallete;
        u4ThirdAddr = (UINT32)ThirdAddr;
        u4ThirdPallete = (UINT32)ThirdPallete;
        u4ThirdPitch = file_data[14]*16;
        u4ThirdCM = file_data[15];
        fgThirdEn = (BOOL)file_data[16];
        fgAlcomDiffCM = (BOOL)file_data[17];
        u4DstCM = (UINT32)file_data[19];
        
        //wanring solve
	u4SrcPallte =u4SrcPallte;
	u4ThirdPallete = u4ThirdPallete;
	fgAlcomDiffCM=fgAlcomDiffCM;
	u4Size = u4Height * u4Width;
        //if(fgFirstTime) {
            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_bmp_%d.raw\" 0x%08x /ny\n",i,u4SrcAddr);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_bmp_%d.raw\" 0x%08x /ny", 
    		(i),u4SrcAddr));

            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_plt_%d.raw\" 0x%08x /ny\n",i,SrcPallete);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_plt_%d.raw\" 0x%08x /ny", 
    		(i),SrcPallete));    		

            if(fgThirdEn)
            {
                LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny\n",i,ThirdAddr);
                UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny", 
    		    (i),ThirdAddr));

                LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny\n",i,ThirdPallete);
                UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny", 
    		    (i),ThirdPallete));
    		}
    		else
    		{
    		    LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny\n",i,ThirdAddr);
                UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny", 
    		    (i),DstAddr));

                //LOG(0, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny\n",i,ThirdPallete);
                //UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny", 
    		    //(i),ThirdPallete));
    		}
            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\dram_result_%d.raw\" 0x%08x /ny\n",(i),golden_dataY);
    		UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\dram_result_%d.raw\" 0x%08x /ny", 
    		(i),golden_dataY));
		//}

        //Printf("Src = 0x%08x, sPitch = %d, Dst = 0x%08x, dPitch = %d, W = %d, H = %d, u4RollBackEn = %d,u4QualityMode = %d, u4BpCompAddrEnd = 0x%08x, u4ColorMode = %d fgBurstRead = %d\n",
        //    u4SrcAddr, u4SrcPitch, u4DstAddr, u4DstPitch, u4Width, u4Height, u4RollBackEn,u4QualityMode,u4BpCompAddrEnd,u4ColorMode,fgBurstRead);

        printf("%d %d %d\n",u4SrcCM,u4DstCM,u4ThirdCM);

	HalFlushInvalidateDCache();	
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        //_GFX_SetSrc((UINT8 *)u4SrcAddr, u4ColorMode, u4SrcPitch);
        #if 0
        _GFX_SetDst((UINT8 *)u4DstAddr, u4DstCM, u4DstPitch);
        _GFX_SetColor(0x0);
        _GFX_Fill(0, 0, u4Width, u4Height);
        _CLI_CMD("gfx.flush");
        #else
        //x_memset(u4DstAddr,0,u4FrameSize);
        #endif
        
        _GFX_SetSrc((UINT8 *)u4SrcAddr, u4SrcCM, u4SrcPitch);
        _GFX_SetThird((UINT8 *)u4ThirdAddr, u4ThirdCM, u4ThirdPitch);

        //if(!fgThirdEn)
        //_GFX_SetDst((UINT8 *)u4ThirdAddr, u4ThirdCM, u4ThirdPitch);
        //else


        _GFX_SetDst((UINT8 *)u4DstAddr, u4DstCM, u4DstPitch);

        GFX_SetAlpha(u4GlobalAlpha);
        //GFX_SetAlpha(0xB3);

        _GFX_SetCharCM(TRUE,E_BMP_CM_8BIT);
        _GFX_SetCharCM(FALSE,E_BMP_CM_8BIT);
        
        _GFX_SetThirdPallete(ThirdPallete,0,0);
        _GFX_SetIdx2DirOpt(SrcPallete,0,0);

        #if 0
        //fgAlcomNormal =0;
        //u4AlcomAr = 255;
        #endif

#if 0   //test force to use global alpha
        u4Opcode = E_AC_SRC;
        GFX_SetAlpha(0xBB);
        _GFX_SetForceAlpha(1);
        _GFX_GeneralAlphaCom(u4Width, u4Height, u4AlcomAr,
                             u4Opcode, 0, fgThirdEn, fgAlcomNormal, FALSE);
        hw_address = (UINT8*)u4DstAddr;
        _CLI_CMD("gfx.flush");
        for (j = 3; j < u4GoldenSize - 4; j = j + 4)
        {
            if (hw_address[j] != 0xBB)
            {
                printf("hw : %x, golden : %x, j = %d\n", hw_address[j], golden_dataY[j], j);
                printf("force to use global alpha test error in emu_setup_%d.txt\n\n", i);
                //printf("BPCOMP_CHKSUM : %x\n",IO_READ32(GFX_ADDR, (0xb8)));
                //printf("BPCOMP_NIPPLE : %x\n",IO_READ32(GFX_ADDR, (0xb4)));
                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\emu_setup_%d_error.txt\" 0x%08x--0x%08x",
                                    (i), (UINT32)hw_address, (UINT32)hw_address + u4GoldenSize - 1));
                return 1;
                //break;
            }
            else
            {
                printf("force to use global alpha test  OK\n\n");
                return 0;
            }
        }
#endif
#if 0//test color key
        u4Opcode = E_AC_SRC;//E_AC_DST;//E_AC_SRC;
            _GFX_SetSrcColorKey(TRUE,0x1a1a1a1a);
        //_GFX_SetThirdColorKey(TRUE,0x198c08b8);
        //_GFX_SetThirdColorKey(TRUE, 0x7F20B634);
        _GFX_GeneralAlphaCom(u4Width, u4Height, u4AlcomAr,
                             u4Opcode, 0, fgThirdEn, fgAlcomNormal, FALSE);
        hw_address = (UINT8*)u4DstAddr;
        u4GoldenSize = 32;
        _CLI_CMD("gfx.flush");
        for (j = 0; j < u4GoldenSize; j++)
        {
            if (hw_address[j] != 0)
            {
                printf("hw : %x, golden : %x, j = %d\n", hw_address[j], golden_dataY[j], j);
                printf("src color key test  error in emu_setup_%d.txt\n\n", i);
                //printf("BPCOMP_CHKSUM : %x\n",IO_READ32(GFX_ADDR, (0xb8)));
                //printf("BPCOMP_NIPPLE : %x\n",IO_READ32(GFX_ADDR, (0xb4)));
                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\emu_setup_%d_error.txt\" 0x%08x--0x%08x",
                                    (i), (UINT32)hw_address, (UINT32)hw_address + u4GoldenSize - 1));
                return 1;
                //break;
            }
            else
            {
                printf("src color key test OK\n\n");
                return 0;
            }
        }
        #else
        _GFX_GeneralAlphaCom(u4Width, u4Height,u4AlcomAr, 
            u4Opcode, 0,fgThirdEn,fgAlcomNormal,FALSE);
        #endif
        hw_address = (UINT8*)u4DstAddr;
            
        _CLI_CMD("gfx.flush");

        //LOG(0,"file_size : %d\n",file_size);
        //HalFlushInvalidateDCache();
		

		if (_GfxMemCompare(
			(UINT8 *)hw_address, 
			(UINT8 *)(golden_dataY), u4Size,u4DstCM) == -1)
		{
			printf("New alpha composition comparing Error in emu_source_%d.txt\n\n",i);
			return -1;
		}
	 
	 /*

     for (j = 0; j < u4GoldenSize ; j++)
        {
            if (hw_address[j] != golden_dataY[j])
            {
                printf("hw : %x, golden : %x, j = %d\n",hw_address[j],golden_dataY[j],j);
                printf("compare error in emu_setup_%d.txt\n\n",i);
                //printf("BPCOMP_CHKSUM : %x\n",IO_READ32(GFX_ADDR, (0xb8)));
                //printf("BPCOMP_NIPPLE : %x\n",IO_READ32(GFX_ADDR, (0xb4)));
                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\emu_setup_%d_error.txt\" 0x%08x--0x%08x", 
    		(i),(UINT32)hw_address,(UINT32)hw_address+u4GoldenSize-1));
                return 1;
                //break;
            }
            else
            {
                //LOG(0,"hw : %x , golden : %x\n",hw_address[j],golden_dataY[j]);
            }
        }  */
        printf("New alpha composition comparing ok in emu_source_%d.txt\n\n",i);
        //fgFirstTime = FALSE;
    }

    return 0;
}


#if defined(CC_MT5368)||defined(CC_MT5396)


static INT32 _GfxEmuIOMMUFill(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size, i;
    UINT8* PA_address = NULL;

    Printf("graphic iommu test fill rect start......\n");

    //draw rect
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

//test fill rect  use 1M table
    HalSet1MSectionTable(0xbb00000, 0xca00000);
    HalSet1MSectionTable(0xba00000, 0xcb00000);
    x_memset((void*)0xbb00000, 0, 0x100000);
    x_memset((void*)0xba00000, 0, 0x100000);
    _GFX_MMU_Init();
    Printf("graphic MMU table address is  0x%x\n", HalGetMMUTableAddress());
    _GFX_MMU_Set_TabAddr(HalGetMMUTableAddress());
    _GFX_MMU_Enable(TRUE);
//    _GFX_MMU_SelfFire();

    GFX_SetDst((UINT8*)0xca00000, (UINT32)CM_ARGB8888_DIRECT32, 720*4);
    _CLI_CMD("gfxemu.color 0x12345678");
    _CLI_CMD("gfx.op.rect 0 0 720 480");
    _CLI_CMD("gfx.flush");


    PA_address = (UINT8*)0xbb00000;
    u4Size = 0x100000 - 1;
    for ( i = 0; i < u4Size; i += 4)
    {
        if ((PA_address[i] != (UINT8)0x78) && (PA_address[i+1] != (UINT8)0x56)
                && (PA_address[i+2] != (UINT8)0x34) && (PA_address[i+3] != (UINT8)0x12))
        {
            Printf("graphic iommu fill rect 1M table test fail  1  i = 0x%x  (PA_address[i] = 0x%x, )\n", i, PA_address[i]);
            return -1;
        }
    }
    PA_address = (UINT8*)0xba00000;
    u4Size = 720 * 480 * 4 - 0x100000;
    for ( i = 0; i < u4Size; i += 4)
    {
        if ((PA_address[i] != (UINT8)0x78) && (PA_address[i+1] != (UINT8)0x56)
                && (PA_address[i+2] != (UINT8)0x34) && (PA_address[i+3] != (UINT8)0x12))

        {
            Printf("graphic iommu fill rect 1M table test fail  2  i = 0x%x  (PA_address[i] = 0x%x, )\n", i, PA_address[i]);
            return -1;
        }
    }
    Printf("graphic iommu fill rect 1M table test OK .......\n");

//test fill rect  use 1M table  VA > 0x80000000
    HalSet1MSectionTable(0xbb00000, 0xaca00000);
    HalSet1MSectionTable(0xba00000, 0xacb00000);
    x_memset((void*)0xbb00000, 0, 0x100000);
    x_memset((void*)0xba00000, 0, 0x100000);
    _GFX_MMU_Init();
    Printf("graphic MMU table address is  0x%x\n", HalGetMMUTableAddress());
    _GFX_MMU_Set_TabAddr(HalGetMMUTableAddress());
    _GFX_MMU_Enable(TRUE);
//    _GFX_MMU_SelfFire();

    GFX_SetDst((UINT8*)0xaca00000, (UINT32)CM_ARGB8888_DIRECT32, 720*4);
    _CLI_CMD("gfxemu.color 0x12345678");
    _CLI_CMD("gfx.op.rect 0 0 720 480");
    _CLI_CMD("gfx.flush");


    PA_address = (UINT8*)0xbb00000;
    u4Size = 0x100000 - 1;
    for ( i = 0; i < u4Size; i += 4)
    {
        if ((PA_address[i] != (UINT8)0x78) && (PA_address[i+1] != (UINT8)0x56)
                && (PA_address[i+2] != (UINT8)0x34) && (PA_address[i+3] != (UINT8)0x12))
        {
            Printf("graphic iommu fill rect 1M table test fail  1  i = 0x%x  (PA_address[i] = 0x%x, )\n", i, PA_address[i]);
            return -1;
        }
    }
    PA_address = (UINT8*)0xba00000;
    u4Size = 720 * 480 * 4 - 0x100000;
    for ( i = 0; i < u4Size; i += 4)
    {
        if ((PA_address[i] != (UINT8)0x78) && (PA_address[i+1] != (UINT8)0x56)
                && (PA_address[i+2] != (UINT8)0x34) && (PA_address[i+3] != (UINT8)0x12))

        {
            Printf("graphic iommu fill rect 1M table test fail  2  i = 0x%x  (PA_address[i] = 0x%x, )\n", i, PA_address[i]);
            return -1;
        }
    }
    Printf("graphic iommu fill rect 1M table test (VA out of PA space) OK .......\n");


#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

//test fill rect  use 4K table
    x_memset((void*)0xa000000, 0, 0x400);
    HalSet4KPageTable(0xbb00000, 0xca00000, (UINT32 *)0xa000000);
    for (i = 1; i < 0x100; i++)
    {
        HalSet4KPageTable(0xbb00000 | (i << 12), 0xca00000 | (i << 12), (UINT32 *)0xa000000);
    }
    HalSet4KPageTable(0xbc01000, 0xca01000, (UINT32 *)0xa000000);
    HalSet1MSectionTable(0xba00000, 0xcb00000);
    x_memset((void*)0xbb00000, 0, 0x100000);
    x_memset((void*)0xba00000, 0, 0x100000);
    x_memset((void*)0xbc00000, 0, 0x100000);
    _GFX_MMU_Init();
    // Printf("graphic MMU table address is  0x%x\n",HalGetMMUTableAddress());
    _GFX_MMU_Set_TabAddr(HalGetMMUTableAddress());
    _GFX_MMU_Enable(TRUE);
    //  _GFX_MMU_SelfFire();

    GFX_SetDst((UINT8*)0xca00000, (UINT32)CM_ARGB8888_DIRECT32, 720*4);
    _CLI_CMD("gfxemu.color 0x12345678");
    _CLI_CMD("gfx.op.rect 0 0 720 480");
    _CLI_CMD("gfx.flush");


    PA_address = (UINT8*)0xbb00000;
    u4Size = 0x1000;
    for ( i = 0; i < u4Size; i += 4)  //4K
    {
        if ((PA_address[i] != (UINT8)0x78) && (PA_address[i+1] != (UINT8)0x56)
                && (PA_address[i+2] != (UINT8)0x34) && (PA_address[i+3] != (UINT8)0x12))
        {
            Printf("graphic iommu fill rect 4K table test fail  1  i = 0x%x  (PA_address[i] = 0x%x, )\n", i, PA_address[i]);
            return -1;
        }
    }
    PA_address = (UINT8*)0xbc01000;   //4K
    u4Size = 0x1000;
    for ( i = 0; i < u4Size; i += 4)
    {
        if ((PA_address[i] != (UINT8)0x78) && (PA_address[i+1] != (UINT8)0x56)
                && (PA_address[i+2] != (UINT8)0x34) && (PA_address[i+3] != (UINT8)0x12))
        {
            Printf("graphic iommu fill rect 4K table test fail  2  i = 0x%x  (PA_address[i] = 0x%x, )\n", i, PA_address[i]);
            return -1;
        }
    }
    PA_address = (UINT8*)0xbb02000;
    u4Size = 0x100000 - 0x1000 * 2;   // 1M - 2*4K
    for ( i = 0; i < u4Size; i += 4)
    {
        if ((PA_address[i] != (UINT8)0x78) && (PA_address[i+1] != (UINT8)0x56)
                && (PA_address[i+2] != (UINT8)0x34) && (PA_address[i+3] != (UINT8)0x12))
        {
            Printf("graphic iommu fill rect 4K table test fail  3  i = 0x%x  (PA_address[i] = 0x%x, )\n", i, PA_address[i]);
            return -1;
        }
    }

    PA_address = (UINT8*)0xba00000;
    u4Size = 720 * 480 * 4 - 0x100000;
    for ( i = 0; i < u4Size; i += 4)
    {
        if ((PA_address[i] != (UINT8)0x78) && (PA_address[i+1] != (UINT8)0x56)
                && (PA_address[i+2] != (UINT8)0x34) && (PA_address[i+3] != (UINT8)0x12))

        {
            Printf("graphic iommu fill rect 4K table test fail  4  i = 0x%x  (PA_address[i] = 0x%x, )\n", i, PA_address[i]);
            return -1;
        }
    }
    Printf("graphic iommu fill rect 4K table test OK .......\n");
    return 0;
}

/*
static void GetResVirtStart()
{
    //_u4KernelEndAddr = (num_physpages >> (20 - PAGE_SHIFT)) << 20;
    //_u4AllDramSize = TOTAL_DRAM_SIZE + FBM_CHB_SIZE;
    _u4KernelEndAddr = TOTAL_DRAM_SIZE - FBM_SIZE;
    _u4AllDramSize = TOTAL_DRAM_SIZE;
    _ui4ResVirtStart = (UINT32) BSP_MapReservedMem((void*) _u4KernelEndAddr, _u4AllDramSize - _u4KernelEndAddr);
    _ui4ResVirtEnd = _ui4ResVirtStart + (_u4AllDramSize - _u4KernelEndAddr);
    printk("Mapped physical address 0x%x - 0x%x to virtual begins 0x%x\n", _u4KernelEndAddr, _u4AllDramSize, _ui4ResVirtStart);
}
*/


static INT32 _GfxEmuCmddualFill2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
	UINT32 j,u4CmpAddr, u4DstAddr; 

    Printf("Rect Fill Test 02 ");

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
 
	    u4DstAddr = BUF_SCREEN + 0x20000000; 
		u4CmpAddr = COMPARE_BUF + 0x20000000;
      for (j = 0;j < 9;j++)  // UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
      {
        HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr+ j*0x100000);   //+48M
        HalSet1MSectionTable(u4CmpAddr + j*0x100000, u4CmpAddr+ j*0x100000); 
               // HalSet1MSectionTable(u4ThirdAddr + j*0x100000, u4ThirdAddr + j*0x100000);
               // HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr + j*0x100000);
      }
	  HalFlushInvalidateDCache();
	  HalInvalidateTLB();
	
    _GfxSetDefMode(u4DstAddr, ADR_DST);
    _GfxSetDefMode(u4DstAddr, ADR_SRC);
    _CLI_CMD("gfxemu.cls");
    
    _CLI_CMD("gfxemu.color 0xFFF0F8FF");    //aliceblue
    _CLI_CMD("gfx.op.rect  20  20 140 120");
	_CLI_CMD("gfx.flush");

	#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode sw");

    _GfxSetDefMode(u4CmpAddr, ADR_DST);
    _GfxSetDefMode(u4CmpAddr, ADR_SRC);
    _CLI_CMD("gfxemu.cls");

    _CLI_CMD("gfxemu.color 0xFFF0F8FF");    //aliceblue
    _CLI_CMD("gfx.op.rect  20  20 140 120");
    _CLI_CMD("gfx.flush");
    
    // Compare HW and SW
    u4Size = _u4ScreenWidth * _u4ScreenHeight;
    return _GfxBufferCompare(
                (UINT32 *)u4DstAddr,
                (UINT32 *)(u4CmpAddr),
                u4Size);
}


static INT32 _GfxEmuDualChannelGeneralAlcom(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height;
    UINT32 u4SrcAddr, u4DstAddr, u4SrcPitch, u4DstPitch, u4ThirdAddr, u4ThirdPitch;
    UINT32 u4SrcCM, u4DstCM, u4ThirdCM;
    char* pcFileInfo;
    UINT32 i, u4Times, j;
    char* pcToken;
    UINT32 u4InitValue = 0;
    UINT32 index = 0;
    //UINT32 u4RepeatFrame =9999;
    UINT32 u4VaTrans = 0x3000000;
    UINT8* hw_address = NULL;
    UINT8* SrcAddr = NULL;
    UINT8* ThirdAddr = NULL;
    UINT8* SrcPallete = NULL;
    UINT8* ThirdPallete = NULL;
    UINT8* golden_dataY = NULL;
    UINT8* DstAddr = NULL;
    //BOOL fgFirstTime = TRUE;
    UINT32 u4FrameSize = (1920 * 1200 * 4);
    UINT8* PA_address = NULL;
    UINT8* Golden_address = NULL;
//#if GFX_VERIFY_GROUP3
    UINT32 u4Value, u4TestType, u4MMUEnable = 0;
//#endif

    UINT32 u4GlobalAlpha, u4Opcode, u4AlcomAr, u4SrcPallte, u4ThirdPallete, u4GoldenSize;
    BOOL fgAlcomNormal, fgThirdEn, fgAlcomDiffCM;
    //golden_dataY = (UINT8*)BSP_AllocAlignedDmaMemory((1920 * 1088*4), 1024);

// 5396 FPGA address bit30 = 1?channelB:channelA
#if 0
    DstAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    golden_dataY = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    SrcAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    ThirdAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
#else
    DstAddr = (UINT8*)_u4GfxEmuBuf[0];
    golden_dataY = DstAddr + u4FrameSize;
    SrcAddr = golden_dataY + u4FrameSize;
    ThirdAddr = SrcAddr + u4FrameSize;
    //SrcPallete = ThirdAddr + 1024;
    //ThirdPallete = SrcPallete + 1024;
#endif
    _GFX_MMU_Enable(FALSE);
    x_memset((void*)DstAddr, 0, u4FrameSize);
    x_memset((void*)golden_dataY, 0, u4FrameSize);
    x_memset((void*)SrcAddr, 0, u4FrameSize);
    x_memset((void*)ThirdAddr, 0, u4FrameSize);

    SrcPallete = (UINT8 *)BSP_AllocAlignedDmaMemory((1024), 1024);
    ThirdPallete =(UINT8 *)BSP_AllocAlignedDmaMemory((1024), 1024);

    if (SrcAddr == NULL || ThirdAddr == NULL || SrcPallete == NULL || ThirdPallete == NULL || DstAddr == NULL || golden_dataY == NULL)
    {
        ASSERT(0);
    }

    // Printf("General Alpha Composition Stress  (1 group3, 2 dual channelB) Test ...\n");

    u4InitValue =  (UINT32) StrToInt(szArgv[2]);
    u4Times =  (UINT32) StrToInt(szArgv[3]);
    u4TestType = (UINT32) StrToInt(szArgv[1]);


    //u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);

    pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);

    for (i = u4InitValue ; i < u4Times ; i++)
    {
        //if(i == (u4RepeatFrame+1))
        //    i--;
        UINT32 file_data[21] = {0};

        index = 0;

        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\emu_setup_%d.txt\" 0x%08x /ny\n", i, pcFileInfo);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\emu_setup_%d.txt\" 0x%08x /ny",
                            i, pcFileInfo));

        //Printf("string : %s\n",pcFileInfo);
        //sscanf(pcFileInfo,"%s %s %s %s %s %s",&file_data[0],&file_data[1],&file_data[2],&file_data[3],&file_data[4],&file_data[5]);

        pcToken = strtok(pcFileInfo, "\r\n");
        while ( pcToken != NULL)
        {
            file_data[index++] = StrToInt(pcToken);
            pcToken = strtok(NULL, "\r\n");
        }

        u4SrcAddr = (UINT32)SrcAddr;
        u4DstAddr = (UINT32)DstAddr;
        u4ThirdAddr = (UINT32)ThirdAddr;
        u4SrcPitch = (UINT32)file_data[2] * 16;
        u4DstPitch = (UINT32)file_data[3] * 16;
        u4SrcCM = (UINT32)file_data[4];
        u4Height = (UINT32)file_data[5] + 1;
        u4Width = (UINT32)file_data[6];
        u4GlobalAlpha = (UINT32)file_data[7];
        fgAlcomNormal = (BOOL)file_data[8];
        u4Opcode = file_data[9];
        u4AlcomAr = file_data[10];
        u4SrcPallte = (UINT32)SrcPallete;
        u4ThirdAddr = (UINT32)ThirdAddr;
        u4ThirdPallete = (UINT32)ThirdPallete;
        u4ThirdPitch = file_data[14] * 16;
        u4ThirdCM = file_data[15];
        fgThirdEn = (BOOL)file_data[16];
        fgAlcomDiffCM = (BOOL)file_data[17];
        u4DstCM = (UINT32)file_data[19];
        u4GoldenSize = file_data[20];

        //wanring solve
        u4SrcPallte = u4SrcPallte;
        u4ThirdPallete = u4ThirdPallete;
        fgAlcomDiffCM = fgAlcomDiffCM;
//#ifdef 5396_IC_VERIFICATION   //total DRAM 1G;  <512M,  channelA,  >512M(0x20000000)  channelB 		
#if 1//5396_IC_DUAL_CH 
    
		 if (u4TestType == 1)  //group 3 verify
        {
            Printf("General Alpha Composition Stress  (use  group3) Test ...\n");
            u4Value = IO_READ32(GFX_ADDR, (0x0));
            u4Value = (u4Value & (~(1 << 19)));
            u4Value = (u4Value) | (1 << 19) ;
            IO_WRITE32(GFX_ADDR, (0), u4Value);
        }
        else  if (u4TestType == 2)  //src use channel B
        {
            u4SrcAddr = u4SrcAddr + 0x20000000;
			 Printf("General Alpha Composition Stress Test ...\n", u4SrcAddr);
          
            for (j = 0;j < 9;j++)  // UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
            {
                HalSet1MSectionTable(u4SrcAddr + j*0x100000, u4SrcAddr+ j*0x100000);   //+48M
               // HalSet1MSectionTable(u4ThirdAddr + j*0x100000, u4ThirdAddr + j*0x100000);
               // HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr + j*0x100000);
            }  
			  HalFlushInvalidateDCache();
	          HalInvalidateTLB();
        }
		 else  if (u4TestType == 7)  //src dst use channel B 
        {
             Printf("General Alpha Composition Stress  (src address use  channelB, dst use channelB) Test ...\n");
           u4SrcAddr = u4SrcAddr + 0x20000000;
            u4DstAddr = u4DstAddr + 0x20000000;
			 for (j = 0;j < 9;j++)  // UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
            {
                HalSet1MSectionTable(u4SrcAddr + j*0x100000, u4SrcAddr+ j*0x100000);   //+48M
                HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr + j*0x100000);
               // HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr + j*0x100000);
            }  
			  HalFlushInvalidateDCache();
	          HalInvalidateTLB();
        }
        else  if (u4TestType == 3)  //src   channel A and dst use channel B
        {
            Printf("General Alpha Composition Stress  (src address use  channelA, dst use channelB) Test ...\n");
            //u4SrcAddr = u4SrcAddr + 0x20000000;
            u4DstAddr = u4DstAddr + 0x20000000;
			 for (j = 0;j < 9;j++)  // UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
            {
                //HalSet1MSectionTable(u4SrcAddr + j*0x100000, u4SrcAddr+ j*0x100000);   //+48M
                HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr + j*0x100000);
               // HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr + j*0x100000);
            }  
			  HalFlushInvalidateDCache();
	          HalInvalidateTLB();
        }

        else if (u4TestType == 4)   //enable gfx MMU  use group3
        {
            Printf("General Alpha Composition Stress  (enable MMU, use group 3) Test ...\n");
            u4MMUEnable = 1;
            u4Value = IO_READ32(GFX_ADDR, (0x0));
            u4Value = (u4Value & (~(1 << 19)));
            u4Value = (u4Value) | (1 << 19) ;
            IO_WRITE32(GFX_ADDR, (0), u4Value);
        }
        else   if (u4TestType == 5)  //src use channel B
        {
            Printf("General Alpha Composition Stress  (src address use  channelB, dst use channelA) Test ...\n");
            u4SrcAddr = u4SrcAddr + 0x20000000;
            u4MMUEnable = 1;
			 for (j = 0;j < 9;j++)  // UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
            {
                HalSet1MSectionTable(u4SrcAddr + j*0x100000, u4SrcAddr+ j*0x100000);   //+48M
             }  
			  HalFlushInvalidateDCache();
	          HalInvalidateTLB();
        }
        else   if (u4TestType == 6)  //src  and dst use channel B
        {
            Printf("General Alpha Composition Stress  (src address use  channel3, dst use channelB) Test ...\n");
            u4SrcAddr = u4SrcAddr + 0x20000000;
            u4DstAddr = u4DstAddr + 0x20000000;
            u4MMUEnable = 1;
			 for (j = 0;j < 9;j++)  // UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
            {
                HalSet1MSectionTable(u4SrcAddr + j*0x100000, u4SrcAddr+ j*0x100000);   //+48M
                HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr + j*0x100000);
               // HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr + j*0x100000);
            }  
			  HalFlushInvalidateDCache();
	          HalInvalidateTLB();
        }
#endif
        //if(fgFirstTime) {
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_bmp_%d.raw\" 0x%08x /ny\n", i, u4SrcAddr);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_bmp_%d.raw\" 0x%08x /ny",
                            (i), u4SrcAddr));

        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_plt_%d.raw\" 0x%08x /ny\n", i, SrcPallete);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_plt_%d.raw\" 0x%08x /ny",
                            (i), SrcPallete));
        if (fgThirdEn)
        {
            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny\n", i, ThirdAddr);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny",
                                (i), ThirdAddr));

            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny\n", i, ThirdPallete);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny",
                                (i), ThirdPallete));
        }
        else
        {
            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny\n", i, ThirdAddr);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny",
                                (i), u4DstAddr));
               //LOG(0, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny\n",i,ThirdPallete);
            //UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny",
            //(i),ThirdPallete));
        }
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\dram_result_%d.raw\" 0x%08x /ny\n", (i), golden_dataY);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\dram_result_%d.raw\" 0x%08x /ny",
                            (i), golden_dataY));
        //}

        printf("%d %d %d\n", u4SrcCM, u4DstCM, u4ThirdCM);
		Printf("test by  u4SrcAddr = 0x%08x ,u4DstAddr = 0x%x\n", u4SrcAddr, u4DstAddr);

        _CLI_CMD("gfx.reset");
        _CLI_CMD("gfxemu.mode");
		
//#ifdef 5396_FPGA_EMULATION	// total dram is 256M
#if 0
        if (u4TestType == 1)  //group 3 verify
        {
            Printf("General Alpha Composition Stress  (use  group3) Test ...\n");
            u4Value = IO_READ32(GFX_ADDR, (0x0));
            u4Value = (u4Value & (~(1 << 19)));
            u4Value = (u4Value) | (1 << 19) ;
            IO_WRITE32(GFX_ADDR, (0), u4Value);
        }
        else  if (u4TestType == 2)  //src use channel B
        {
            u4SrcAddr = u4SrcAddr + 0x20000000;  
            for (j = 0;j < 9;j++)  // UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
            {
                HalSet1MSectionTable(u4SrcAddr + j*0x100000, u4SrcAddr+ j*0x100000);   //+48M
               // HalSet1MSectionTable(u4ThirdAddr + j*0x100000, u4ThirdAddr + j*0x100000);
               // HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr + j*0x100000);
            }
            Printf("General Alpha Composition Stress  (src address use  channelB, dst use channelA) Test 0x%x ...\n", u4SrcAddr);
            
        }

        else  if (u4TestType == 3)  //src  and dst use channel B
        {
            Printf("General Alpha Composition Stress  (src address use  channelB, dst use channelB) Test ...\n");
            u4SrcAddr = u4SrcAddr + 0x40000000;
            u4DstAddr = u4DstAddr + 0x40000000;
        }

        else if (u4TestType == 4)   //enable gfx MMU  use group3
        {
            Printf("General Alpha Composition Stress  (enable MMU, use group 3) Test ...\n");
            u4MMUEnable = 1;
            u4Value = IO_READ32(GFX_ADDR, (0x0));
            u4Value = (u4Value & (~(1 << 19)));
            u4Value = (u4Value) | (1 << 19) ;
            IO_WRITE32(GFX_ADDR, (0), u4Value);
        }
        else   if (u4TestType == 5)  //src use channel B
        {
            Printf("General Alpha Composition Stress  (src address use  channel3, dst use channelA) Test ...\n");
            u4SrcAddr = u4SrcAddr + 0x40000000;
            u4MMUEnable = 1;
        }
        else   if (u4TestType == 6)  //src  and dst use channel B
        {
            Printf("General Alpha Composition Stress  (src address use  channel3, dst use channelB) Test ...\n");
            u4SrcAddr = u4SrcAddr + 0x40000000;
            u4DstAddr = u4DstAddr + 0x40000000;
            u4MMUEnable = 1;
        }
#endif
        if (u4MMUEnable)
        {    //test fill rect  use 1M table
            for (j = 0;j < 9;j++)  // UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
            {
                HalSet1MSectionTable(u4SrcAddr + j*0x100000, u4SrcAddr + u4VaTrans + j*0x100000);   //+48M
                HalSet1MSectionTable(u4ThirdAddr + j*0x100000, u4ThirdAddr + u4VaTrans + j*0x100000);
                HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr + u4VaTrans + j*0x100000);
            }
            //should blt dst raw data to PA 4K address
            HalSet1MSectionTable(u4DstAddr , u4DstAddr);
            x_memset((void*)0xa000000, 0, 0x400);
            HalSet4KPageTable(u4DstAddr, u4DstAddr + u4VaTrans, (UINT32 *)0xa000000);
            for (i = 1; i < 0x100; i++)
            {
                HalSet4KPageTable((u4DstAddr&0xfff00000) | (i << 12), ((u4DstAddr + u4VaTrans)&0xfff00000) | (i << 12), (UINT32 *)0xa000000);
            }
            HalSet4KPageTable(0xa801000, ((u4DstAddr + u4VaTrans)&0xfff00000) + 0x1000, (UINT32 *)0xa000000);
            HalSet4KPageTable(0xa805000, ((u4DstAddr + u4VaTrans)&0xfff00000) + 0x2000, (UINT32 *)0xa000000);

            _GFX_MMU_Init();
            _GFX_MMU_Set_TabAddr(HalGetMMUTableAddress());
            _GFX_MMU_Enable(TRUE);
            //_GFX_MMU_SelfFire();
            _GFX_SetSrc((UINT8 *)(u4SrcAddr + u4VaTrans), u4SrcCM, u4SrcPitch);
            _GFX_SetThird((UINT8 *)(u4ThirdAddr + u4VaTrans), u4ThirdCM, u4ThirdPitch);
            _GFX_SetDst((UINT8 *)(u4DstAddr + u4VaTrans), u4DstCM, u4DstPitch);
        }
        else
        {
            _GFX_SetSrc((UINT8 *)(u4SrcAddr ), u4SrcCM, u4SrcPitch);
            _GFX_SetThird((UINT8 *)(u4ThirdAddr ), u4ThirdCM, u4ThirdPitch);
            _GFX_SetDst((UINT8 *)(u4DstAddr ), u4DstCM, u4DstPitch);
        }
        GFX_SetAlpha(u4GlobalAlpha);

        _GFX_SetCharCM(TRUE, E_BMP_CM_8BIT);
        _GFX_SetCharCM(FALSE, E_BMP_CM_8BIT);

        _GFX_SetThirdPallete(ThirdPallete, 0, 0);
        _GFX_SetIdx2DirOpt(SrcPallete, 0, 0);
       
        _GFX_GeneralAlphaCom(u4Width, u4Height, u4AlcomAr,
                             u4Opcode, 0, fgThirdEn, fgAlcomNormal, FALSE);
        hw_address = (UINT8*)(u4DstAddr);

        _CLI_CMD("gfx.flush");

        Golden_address = (UINT8*)golden_dataY;
        PA_address = (UINT8*)hw_address;
	      for ( j = 0; j < 0x1000; j ++)  //4K
        {
            if (PA_address[j] != Golden_address[j])
            {
                Printf("hw : %x, golden : %x, j = %d\n", PA_address[j], Golden_address[j], j);
                Printf("gfx alpha composition enable MMU compare error  0 in emu_setup_%d.txt\n\n", i);
                return 1;
                //break;
            }
        }

        Golden_address = (UINT8*)(golden_dataY + 0x3000);
        PA_address = (UINT8*)(hw_address + 0x3000);
        u4GoldenSize = u4GoldenSize - 3 * 0x1000;

        for (j = 0; j < u4GoldenSize ; j++)
        {
            if (PA_address[j] != Golden_address[j])
            {
                Printf("hw : %x, golden : %x, j = %d\n", PA_address[j], Golden_address[j], j);
                Printf("gfx alpha composition enable MMU  compare error 3 in emu_setup_%d.txt\n\n", i);
                return 1;
                //break;
            }
            else
            {
                //LOG(0,"hw : %x , golden : %x\n",hw_address[j],golden_dataY[j]);
            }
        }
        Printf("New alpha composition MMU open  comparing ok in emu_source_%d.txt\n\n", i);
        //fgFirstTime = FALSE;
    }
    if ((u4TestType == 1) || (u4TestType == 4))
    {
        u4Value = IO_READ32(GFX_ADDR, (0x0));
        u4Value = (u4Value & (~(1 << 19)));
        IO_WRITE32(GFX_ADDR, (0), u4Value);
    }
    return 0;
}

//=========================

#endif

#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)||defined(CC_MT5398)

static INT32 _GfxEmuIOMMUBlt(void)
{
    UINT32 u4Size, i;
    UINT8* PA_address = NULL;
    UINT8* Golden_address = NULL;


    Printf("graphic iommu test blit   start......\n");

    LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\argb8888_720x480.bin\" 0x%08x /ny\n", 0xbb00000);
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\argb8888_720x480.bin\" 0x%08x /ny",
                        0xbb00000));

    //draw rect
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

//test fill rect  use 1M table
    HalSet1MSectionTable(0xbb00000, 0xca00000);
    HalSet1MSectionTable(0xbc00000, 0xcb00000);

    // HalSet1MSectionTable(0xaa00000,0xaa00000);   //reset 0xaa00000
    //HalSet1MSectionTable(0xab00000,0xaab00000);  //reset 0xab00000

    HalSet1MSectionTable(0xaa00000, 0xda00000);
    HalSet1MSectionTable(0xab00000, 0xdb00000);
    x_memset((void*)0xaa00000, 0, 0x100000);
    x_memset((void*)0xab00000, 0, 0x100000);
    _GFX_MMU_Init();
    Printf("graphic MMU table address is  0x%x\n", HalGetMMUTableAddress());
    _GFX_MMU_Set_TabAddr(HalGetMMUTableAddress());
    _GFX_MMU_Enable(TRUE);
    //  _GFX_MMU_SelfFire();

    GFX_SetSrc((UINT8*)0xca00000, (UINT32)CM_ARGB8888_DIRECT32, 720*4);
    GFX_SetDst((UINT8*)0xda00000, (UINT32)CM_ARGB8888_DIRECT32, 720*4);

    _CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
    _CLI_CMD("gfx.flush");

    PA_address = (UINT8*)0xaa00000;
    Golden_address =  (UINT8*)0xbb00000;
    u4Size = 720 * 480 * 4;
    for ( i = 0; i < u4Size; i ++)
    {
        if (PA_address[i] != Golden_address[i])
        {
            Printf("graphic iommu blit 1M table test fail  1  i = 0x%x  (PA_address[i] = 0x%x,  Golden_address[i] = 0x%x)\n", i, PA_address[i], Golden_address[i]);
            return -1;
        }
    }
    Printf("graphic iommu blit 1M table test OK .......\n");


    x_memset((void*)0xa000000, 0, 0x400);
    HalSet1MSectionTable(0xbb00000, 0xca00000);
    HalSet1MSectionTable(0xbc00000, 0xcb00000);
    HalSet4KPageTable(0xaa00000, 0xda00000, (UINT32 *)0xa000000);
    for (i = 1; i < 0x100; i++)
    {
        HalSet4KPageTable(0xaa00000 | (i << 12), 0xda00000 | (i << 12), (UINT32 *)0xa000000);
    }
    HalSet4KPageTable(0xac01000, 0xda01000, (UINT32 *)0xa000000);
    HalSet1MSectionTable(0xab00000, 0xdb00000);

    _GFX_MMU_Init();
    _GFX_MMU_Set_TabAddr(HalGetMMUTableAddress());
    _GFX_MMU_Enable(TRUE);
    // _GFX_MMU_SelfFire();

    GFX_SetSrc((UINT8*)0xca00000, (UINT32)CM_ARGB8888_DIRECT32, 720*4);
    GFX_SetDst((UINT8*)0xda00000, (UINT32)CM_ARGB8888_DIRECT32, 720*4);

    _CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
    _CLI_CMD("gfx.flush");
    Golden_address = (UINT8*)0xbb00000;
    PA_address = (UINT8*)0xaa00000;
    u4Size = 0x1000;
    for ( i = 0; i < u4Size; i ++)  //4K
    {
        if (PA_address[i] != Golden_address[i])
        {
            Printf("graphic iommu blit 4K table test fail  1  i = 0x%x  (PA_address[i] = 0x%x,  Golden_address[i] = 0x%x)\n", i, PA_address[i], Golden_address[i]);
            return -1;
        }
    }
    PA_address = (UINT8*)0xac01000;   //4K
    Golden_address = (UINT8*)0xbb01000;
    u4Size = 0x1000;
    for ( i = 0; i < u4Size; i ++)
    {
        if (PA_address[i] != Golden_address[i])
        {
            Printf("graphic iommu blit 4K table test fail  2 i = 0x%x  (PA_address[i] = 0x%x,  Golden_address[i] = 0x%x)\n", i, PA_address[i], Golden_address[i]);
            return -1;
        }
    }
    Golden_address = (UINT8*)0xbb02000;
    PA_address = (UINT8*)0xaa02000;
    u4Size = 0x100000 - 0x1000 * 2;   // 1M - 2*4K
    for ( i = 0; i < u4Size; i ++)
    {
        if (PA_address[i] != Golden_address[i])
        {
            Printf("graphic iommu blit 4K table test fail  3 i = 0x%x  (PA_address[i] = 0x%x,  Golden_address[i] = 0x%x)\n", i, PA_address[i], Golden_address[i]);
            return -1;
        }
    }
    Golden_address = (UINT8*)0xbc00000;
    PA_address = (UINT8*)0xab00000;
    u4Size = 720 * 480 * 4 - 0x100000;
    for ( i = 0; i < u4Size; i += 4)
    {
        if (PA_address[i] != Golden_address[i])
        {
            Printf("graphic iommu blit 4K table test fail  4 i = 0x%x  (PA_address[i] = 0x%x,  Golden_address[i] = 0x%x)\n", i, PA_address[i], Golden_address[i]);
            return -1;
        }
    }
    Printf("graphic iommu blit 4K table test OK .......\n");

    return 0;
}

static INT32 _GfxEmuIOMMUGeneralAlcom(INT32 u4start, INT32 u4end)
{
    UINT32 u4Width, u4Height,u4Size;
    UINT32 u4SrcAddr, u4DstAddr, u4SrcPitch, u4DstPitch, u4ThirdAddr, u4ThirdPitch;
    UINT32 u4SrcCM, u4DstCM, u4ThirdCM;
    char* pcFileInfo;
    UINT32 i, u4Times, j;
    char* pcToken;
    UINT32 u4InitValue = 0;
    UINT32 index = 0;
    //UINT32 u4RepeatFrame =9999;
    UINT32 u4VaTrans = 0x3000000;
    UINT8* hw_address = NULL;
    UINT8* SrcAddr = NULL;
    UINT8* ThirdAddr = NULL;
    UINT8* SrcPallete = NULL;
    UINT8* ThirdPallete = NULL;
    UINT8* golden_dataY = NULL;
    UINT8* DstAddr = NULL;
    //BOOL fgFirstTime = TRUE;
    UINT32 u4FrameSize = (1920 * 1200 * 4);
    UINT8* PA_address = NULL;
    UINT8* Golden_address = NULL;

    UINT32 u4GlobalAlpha, u4Opcode, u4AlcomAr, u4SrcPallte, u4ThirdPallete, u4GoldenSize;
    BOOL fgAlcomNormal, fgThirdEn, fgAlcomDiffCM;
    //golden_dataY = (UINT8*)BSP_AllocAlignedDmaMemory((1920 * 1088*4), 1024);

#if 0
    DstAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    golden_dataY = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    SrcAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    ThirdAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
#else
    DstAddr = (UINT8*)_u4GfxEmuBuf[0];
    golden_dataY = DstAddr + u4FrameSize;
    SrcAddr = golden_dataY + u4FrameSize;
    ThirdAddr = SrcAddr + u4FrameSize;
    //SrcPallete = ThirdAddr + 1024;
    //ThirdPallete = SrcPallete + 1024;
#endif
    _GFX_MMU_Enable(FALSE);
    x_memset((void*)DstAddr, 0, u4FrameSize);
    x_memset((void*)golden_dataY, 0, u4FrameSize);
    x_memset((void*)SrcAddr, 0, u4FrameSize);
    x_memset((void*)ThirdAddr, 0, u4FrameSize);

    SrcPallete = (UINT8 *)BSP_AllocAlignedDmaMemory((1024), 1024);
    ThirdPallete = (UINT8 *)BSP_AllocAlignedDmaMemory((1024), 1024);

    if (SrcAddr == NULL || ThirdAddr == NULL || SrcPallete == NULL || ThirdPallete == NULL || DstAddr == NULL || golden_dataY == NULL)
    {
        ASSERT(0);
    }

    Printf("General Alpha Composition Stress MMU Test ...");

    u4InitValue =  u4start;
    u4Times =  u4end;
    //u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);

    pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);

    for (i = u4InitValue ; i < u4Times ; i++)
    {
        //if(i == (u4RepeatFrame+1))
        //    i--;
        UINT32 file_data[21] = {0};

        index = 0;

        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\emu_setup_%d.txt\" 0x%08x /ny\n", i, pcFileInfo);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\emu_setup_%d.txt\" 0x%08x /ny",
                            i, pcFileInfo));

        //Printf("string : %s\n",pcFileInfo);
        //sscanf(pcFileInfo,"%s %s %s %s %s %s",&file_data[0],&file_data[1],&file_data[2],&file_data[3],&file_data[4],&file_data[5]);

        pcToken = strtok(pcFileInfo, "\r\n");
        while ( pcToken != NULL)
        {
            file_data[index++] = StrToInt(pcToken);
            pcToken = strtok(NULL, "\r\n");
        }

        u4SrcAddr = (UINT32)SrcAddr;
        u4DstAddr = (UINT32)DstAddr;
        u4ThirdAddr = (UINT32)ThirdAddr;
        u4SrcPitch = (UINT32)file_data[2] * 16;
        u4DstPitch = (UINT32)file_data[3] * 16;
        u4SrcCM = (UINT32)file_data[4];
        u4Height = (UINT32)file_data[5] + 1;
        u4Width = (UINT32)file_data[6];
        u4GlobalAlpha = (UINT32)file_data[7];
        fgAlcomNormal = (BOOL)file_data[8];
        u4Opcode = file_data[9];
        u4AlcomAr = file_data[10];
        u4SrcPallte = (UINT32)SrcPallete;
        u4ThirdAddr = (UINT32)ThirdAddr;
        u4ThirdPallete = (UINT32)ThirdPallete;
        u4ThirdPitch = file_data[14] * 16;
        u4ThirdCM = file_data[15];
        fgThirdEn = (BOOL)file_data[16];
        fgAlcomDiffCM = (BOOL)file_data[17];
        u4DstCM = (UINT32)file_data[19];
        u4GoldenSize = file_data[20];
		u4Size = u4Height * u4Width;
        //wanring solve
        u4SrcPallte = u4SrcPallte;
        u4ThirdPallete = u4ThirdPallete;
        fgAlcomDiffCM = fgAlcomDiffCM;

        //if(fgFirstTime) {
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_bmp_%d.raw\" 0x%08x /ny\n", i, u4SrcAddr);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_bmp_%d.raw\" 0x%08x /ny",
                            (i), u4SrcAddr));

        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_plt_%d.raw\" 0x%08x /ny\n", i, SrcPallete);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_plt_%d.raw\" 0x%08x /ny",
                            (i), SrcPallete));

        if (fgThirdEn)
        {
            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny\n", i, ThirdAddr);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny",
                                (i), ThirdAddr));

            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny\n", i, ThirdPallete);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny",
                                (i), ThirdPallete));
        }
        else
        {
            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny\n", i, ThirdAddr);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny",
                                (i), DstAddr));

            //LOG(0, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny\n",i,ThirdPallete);
            //UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny",
            //(i),ThirdPallete));
        }
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\dram_result_%d.raw\" 0x%08x /ny\n", (i), golden_dataY);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\dram_result_%d.raw\" 0x%08x /ny",
                            (i), golden_dataY));
        //}

        //Printf("Src = 0x%08x, sPitch = %d, Dst = 0x%08x, dPitch = %d, W = %d, H = %d, u4RollBackEn = %d,u4QualityMode = %d, u4BpCompAddrEnd = 0x%08x, u4ColorMode = %d fgBurstRead = %d\n",
        //    u4SrcAddr, u4SrcPitch, u4DstAddr, u4DstPitch, u4Width, u4Height, u4RollBackEn,u4QualityMode,u4BpCompAddrEnd,u4ColorMode,fgBurstRead);

        printf("%d %d %d\n", u4SrcCM, u4DstCM, u4ThirdCM);
		
        _CLI_CMD("gfx.reset");
        _CLI_CMD("gfxemu.mode");

        //test fill rect  use 1M table
        for (j = 0;j < 9;j++)  // UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
        {
            HalSet1MSectionTable(u4SrcAddr + j*0x100000, u4SrcAddr + u4VaTrans + j*0x100000);   //+48M
            HalSet1MSectionTable(u4ThirdAddr + j*0x100000, u4ThirdAddr + u4VaTrans + j*0x100000);
            HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr + u4VaTrans + j*0x100000);
        }
        //should blt dst raw data to PA 4K address
        HalSet1MSectionTable(u4DstAddr , u4DstAddr);
        x_memset((void*)0xa000000, 0, 0x400);
        HalSet4KPageTable(u4DstAddr, u4DstAddr + u4VaTrans, (UINT32 *)0xa000000);
        for (j = 1; j < 0x100; j++)
        {
            HalSet4KPageTable((u4DstAddr&0xfff00000) | (j << 12), ((u4DstAddr + u4VaTrans)&0xfff00000) | (j << 12), (UINT32 *)0xa000000);
        }
        HalSet4KPageTable(0xa801000, ((u4DstAddr + u4VaTrans)&0xfff00000) + 0x1000, (UINT32 *)0xa000000);
        HalSet4KPageTable(0xa805000, ((u4DstAddr + u4VaTrans)&0xfff00000) + 0x2000, (UINT32 *)0xa000000);

        _GFX_MMU_Init();
          _GFX_MMU_Set_TabAddr(HalGetMMUTableAddress());
		  
          HalFlushInvalidateDCache();
          _GFX_SetSrc((UINT8 *)(u4SrcAddr + u4VaTrans), u4SrcCM, u4SrcPitch);
        _GFX_SetThird((UINT8 *)(u4ThirdAddr + u4VaTrans), u4ThirdCM, u4ThirdPitch);

        //if(!fgThirdEn)
        //_GFX_SetDst((UINT8 *)u4ThirdAddr, u4ThirdCM, u4ThirdPitch);
        //else


        _GFX_SetDst((UINT8 *)(u4DstAddr + u4VaTrans), u4DstCM, u4DstPitch);
       
        GFX_SetAlpha(u4GlobalAlpha);
        //GFX_SetAlpha(0xB3);
        
        _GFX_SetCharCM(TRUE, E_BMP_CM_8BIT);
        _GFX_SetCharCM(FALSE, E_BMP_CM_8BIT);
        
        _GFX_SetThirdPallete(ThirdPallete, 0, 0);
        _GFX_SetIdx2DirOpt(SrcPallete, 0, 0);

        
        _GFX_GeneralAlphaCom(u4Width, u4Height, u4AlcomAr,
                             u4Opcode, 0, fgThirdEn, fgAlcomNormal, FALSE);
        hw_address = (UINT8*)(u4DstAddr);
        
        _CLI_CMD("gfx.flush");
        
        //LOG(0,"file_size : %d\n",file_size);
        //HalFlushInvalidateDCache();
        // should blt dst raw data to PA 4K address
      	Golden_address = (UINT8*)golden_dataY;
        PA_address = (UINT8*)hw_address;;
   /*       for ( j = 0; j < 0x1000; j ++)  //4K
        {
            if (PA_address[j] != Golden_address[j])
            {
                printf("hw : %x, golden : %x, j = %d\n", PA_address[j], Golden_address[j], j);
                printf("gfx alpha composition enable MMU compare error  0 in emu_setup_%d.txt\n\n", i);
                //printf("BPCOMP_CHKSUM : %x\n",IO_READ32(GFX_ADDR, (0xb8)));
                //printf("BPCOMP_NIPPLE : %x\n",IO_READ32(GFX_ADDR, (0xb4)));
                return 1;
                //break;
            }
        }  */

		if (_GfxMemCompare(
				(UINT8 *)Golden_address, 
				(UINT8 *)(PA_address), u4Size,u4DstCM) == -1)
		{
				printf("New alpha composition comparing Error in emu_source_%d.txt\n\n",i);
				return -1;
		}

#ifdef GFX_MMU_4KENABLE
        Golden_address = (UINT8*)(golden_dataY + 0x1000);
        PA_address = (UINT8*)0xa801000;
        for ( j = 0;j < 0x1000; j ++)  //4K
        {
            if (PA_address[j] != Golden_address[j])
            {
                printf("hw : %x, golden : %x, j = %d\n", PA_address[j], Golden_address[j], j);
                printf("gfx alpha composition enable MMU  compare error 1 in emu_setup_%d.txt\n\n", i);
                //printf("BPCOMP_CHKSUM : %x\n",IO_READ32(GFX_ADDR, (0xb8)));
                //printf("BPCOMP_NIPPLE : %x\n",IO_READ32(GFX_ADDR, (0xb4)));
                return 1;
                //break;
            }
        }

        Golden_address = (UINT8*)(golden_dataY + 0x2000);
        PA_address = (UINT8*)0xa805000;;
        for ( j = 0; j < 0x1000; j ++)  //4K
        {
            if (PA_address[j] != Golden_address[j])
            {
                printf("hw : %x, golden : %x, j = %d\n", PA_address[j], Golden_address[j], j);
                printf("gfx alpha composition enable MMU  compare error 2 in emu_setup_%d.txt\n\n", i);
                //printf("BPCOMP_CHKSUM : %x\n",IO_READ32(GFX_ADDR, (0xb8)));
                //printf("BPCOMP_NIPPLE : %x\n",IO_READ32(GFX_ADDR, (0xb4)));
                return 1;
                //break;
            }
        }
#endif
        Golden_address = (UINT8*)(golden_dataY + 0x3000);
        PA_address = (UINT8*)(hw_address + 0x3000);
        u4GoldenSize = u4GoldenSize - 3 * 0x1000;

		if (_GfxMemCompare(
				(UINT8 *)Golden_address, 
				(UINT8 *)(PA_address), u4Size,u4DstCM) == -1)
		{
				printf("New alpha composition comparing Error in emu_source_%d.txt\n\n",i);
				return -1;
		}
/*
        for (j = 0; j < u4GoldenSize ; j++)
        {
            if (PA_address[j] != Golden_address[j])
            {
                printf("hw : %x, golden : %x, j = %d\n", PA_address[j], Golden_address[j], j);
                printf("gfx alpha composition enable MMU  compare error 3 in emu_setup_%d.txt\n\n", i);
                return 1;
                //break;
            }
            else
            {
                //LOG(0,"hw : %x , golden : %x\n",hw_address[j],golden_dataY[j]);
            }
        }
        printf("New alpha composition MMU open  comparing ok in emu_source_%d.txt\n\n", i);
        //fgFirstTime = FALSE;   
        */
    }

    return 0;
}

static INT32 _GfxEmuIOMMUCompress2(INT32 u4Start, INT32 u4End)
{
    UINT32 u4Width, u4Height;
    UINT32 u4RollBackEn, u4QualityMode, u4BpCompAddrEnd, u4ColorMode;
    UINT32 u4SrcAddr, u4DstAddr, u4SrcPitch, u4DstPitch = 0;
    BOOL fgMirrorFlip;
    BOOL fgBurstRead;
    char* pcFileInfo;
    UINT32 i, j;
    char* pcToken;
    UINT32 file_size1, file_size2, u4BytesPixel = 1;
    UINT32 FileSize;
    UINT32 index = 0;
    //UINT32 u4RepeatFrame =9999;
    UINT8* SrcAddr = NULL;
    UINT8* DstAddr = NULL;
    UINT8* golden_data1 = NULL;
    UINT8* golden_data2 = NULL;
    UINT8* GoldenData = NULL;
    UINT8* hw_address = NULL;
    //BOOL fgFirstTime = TRUE;
    UINT32 u4FrameSize = 1920 * 1200 * 4;
    UINT32 u4VaTrans = 0x3000000;

    DstAddr = (UINT8*)_u4GfxEmuBuf[0];
    golden_data1 = DstAddr + u4FrameSize;
    SrcAddr = golden_data1 + u4FrameSize;
    golden_data2 = SrcAddr + u4FrameSize;

    hw_address = DstAddr;
    u4SrcAddr     = (UINT32) SrcAddr;//(UINT32) 0x01ea4000;
    u4DstAddr     = (UINT32) DstAddr;

    Printf("Compression BitBlt MMU Enable Test 2 ...");

    //u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);

    fgMirrorFlip = FALSE;

    pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);
       _CLI_CMD("gfx.reset");
        _CLI_CMD("gfxemu.mode");
		
	 _GFX_MMU_Init();
            _GFX_MMU_Set_Agent0(TRUE, 3);  //compress agent0 mid = 3
            for (j = 0;j < 9;j++)  // UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
            {
                HalSet1MSectionTable(u4SrcAddr + j*0x100000, u4SrcAddr + u4VaTrans + j*0x100000); //+48M
                HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr + u4VaTrans + j*0x100000);
            }

            _GFX_MMU_Set_TabAddr(HalGetMMUTableAddress());


    for (i = u4Start ; i < u4End ; i++)
    {
        int line_separate = 0;
        //if(i == (u4RepeatFrame+1))
        //    i--;
        UINT32 file_data[10] = {0};

        index = 0;

        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_info_%d.txt\" 0x%08x /ny\n", i, pcFileInfo);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_info_%d.txt\" 0x%08x /ny",
                            i, pcFileInfo));

        pcToken = strtok(pcFileInfo, " ");
        while ( pcToken != NULL)
        {
            file_data[index++] = StrToInt(pcToken);
            //Printf("bitstream_sz : %d \n",bitstream_sz[u2file_num-1]);
            pcToken = strtok(NULL, " ");
        }


        u4Width       = (UINT32) file_data[0];
        u4Height      = (UINT32) file_data[1];
        u4ColorMode = (UINT32) file_data[2];
        u4QualityMode = (UINT32) file_data[3];
        u4RollBackEn  = (UINT32) file_data[4];
        file_size1 = (UINT32) file_data[5];
        fgBurstRead = (BOOL) file_data[6];
        file_size2 = (UINT32) file_data[7];
        u4BpCompAddrEnd = 0xffffffff;
        //fgBurstRead = (BOOL)((UINT32)rand()%2);
        u4SrcPitch = 0;

#if 0
        if (u4ColorMode == 11 || u4ColorMode == 12)
            continue;
#endif


        if (u4ColorMode == 2 || u4ColorMode == 10)
        {
            u4BytesPixel = 1;
        }
        else if (u4ColorMode == 6 || u4ColorMode == 14)
        {
            u4BytesPixel = 4;
        }
        else if (u4ColorMode == 13 || u4ColorMode == 12 || u4ColorMode == 11)
        {
            u4BytesPixel = 2;
        }
        else
        {
            ASSERT(0);
        }

        u4SrcPitch    = u4Width * u4BytesPixel;


        //if(fgFirstTime) {
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_%d.txt\" 0x%08x /ny\n", i, u4SrcAddr);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_%d.txt\" 0x%08x /ny",
                            (i), u4SrcAddr));
        //x_thread_delay(1000);

        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx\\emu_bitstream_line_connect_%d.txt\" 0x%08x /ny\n", (i), golden_data1);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx\\emu_bitstream_line_connect_%d.txt\" 0x%08x /ny",
                            (i), golden_data1));

        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx\\emu_bitstream_line_separate_%d.txt\" 0x%08x /ny\n", (i), golden_data2);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx\\emu_bitstream_line_separate_%d.txt\" 0x%08x /ny",
                            (i), golden_data2));
        //x_thread_delay(1000);
        //}

        Printf("Src = 0x%08x, sPitch = %d, Dst = 0x%08x, dPitch = %d, W = %d, H = %d, u4RollBackEn = %d,u4QualityMode = %d, u4BpCompAddrEnd = 0x%08x, u4ColorMode = %d fgBurstRead = %d\n",
               u4SrcAddr, u4SrcPitch, u4DstAddr, u4DstPitch, u4Width, u4Height, u4RollBackEn, u4QualityMode, u4BpCompAddrEnd, u4ColorMode, fgBurstRead);

      //  _CLI_CMD("gfx.reset");
       // _CLI_CMD("gfxemu.mode");

        for (line_separate = 0; line_separate < 2 ;line_separate++)
        {
            //printf("\nclear dst buffer\n");
            //_GFX_MMU_Enable(FALSE);
            // _GFX_MMU_SelfFire();
            _GFX_SetDst((UINT8 *)(u4DstAddr+u4VaTrans), 14, 1920*4);
            _GFX_SetColor(0x0);
            _GFX_Fill(0, 0, 1920, 1200);
            _CLI_CMD("gfx.flush");
            //printf("\nclear dst buffer\n");

            //line-separate mode
            if (line_separate == 1)
            {
                UINT32 original_line_bit = u4Width * u4BytesPixel * 8;

                UINT32 line_bit;
                if (u4RollBackEn)
                    line_bit = original_line_bit + 4;
                else
                    line_bit = original_line_bit + 4 + original_line_bit / 16;
                if (fgBurstRead)
                    line_bit = (line_bit % 512 == 0) ? line_bit : (line_bit / 512 + 1) * 512;
                else
                    line_bit = (line_bit % 128 == 0) ? line_bit : (line_bit / 128 + 1) * 128;

                u4DstPitch = line_bit / 8;
                //u4SrcPitch = line_bit/8;
                //u4Width = line_bit/8;
                //u4Width /= u4BytesPixel;

                printf("width : %d\n", u4Width);

                GoldenData = golden_data2;
                FileSize = file_size2;
            }
            else
            {
                u4DstPitch = u4SrcPitch;
                GoldenData = golden_data1;
                FileSize = file_size1;
            }

            //_GFX_SetSrc((UINT8 *)u4SrcAddr, u4ColorMode, u4SrcPitch);
           
          //  _GFX_MMU_Enable(TRUE);
            //_GFX_MMU_SelfFire();

            //printf("\nnew osd compression\n");
            _GFX_NewCompressBlt((UINT8 *)(u4SrcAddr + u4VaTrans), 0, 0, u4ColorMode, u4SrcPitch,
                                (UINT8 *)(u4DstAddr + u4VaTrans), u4Width, u4Height, u4Height, u4RollBackEn, u4QualityMode, u4BpCompAddrEnd, fgMirrorFlip, fgBurstRead, line_separate, u4DstPitch);
            //printf("\nnew osd compression\n");

            _CLI_CMD("gfx.flush");

            //LOG(0,"file_size : %d\n",file_size);
            HalFlushInvalidateDCache();
            for (j = 0; j < FileSize ; j++)
            {
                if (hw_address[j] != GoldenData[j])
                {
                    printf("hw : %x, golden : %x, j = %d\n", hw_address[j], GoldenData[j], j);
                    printf("compare error in emu_source_%d.txt %d\n", i, line_separate);
                    printf("BPCOMP_CHKSUM : %x\n", IO_READ32(GFX_ADDR, (0xb8)));
                    printf("BPCOMP_NIPPLE : %x\n", IO_READ32(GFX_ADDR, (0xb4)));
                    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\emu_source_%d_error_%d.txt\" 0x%08x--0x%08x",
                                        (i), (line_separate), (UINT32)hw_address, (UINT32)hw_address + FileSize - 1));
                    return 1;
                    //break;
                }
                else
                {
                    //LOG(0,"hw : %x , golden : %x\n",hw_address[j],golden_dataY[j]);
                }
            }
        }
        printf("compare ok MMU Enable in emu_source_%d.txt\n\n", i);
        //fgFirstTime = FALSE;
    }
    Printf("Compression BitBlt MMU Enable Test 2  OK...");
    return 0;
}

static INT32 _GfxEmuIOMMUAlcomCompress(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height;
    UINT32 u4SrcAddr, u4DstAddr, u4SrcPitch, u4DstPitch, u4ThirdAddr, u4ThirdPitch;
    UINT32 u4SrcCM, u4DstCM, u4ThirdCM;
    char* pcFileInfo;
    UINT32 i, u4Times, j;
    char* pcToken;
    UINT32 u4InitValue = 0;
    UINT32 index = 0;
    //UINT32 u4RepeatFrame =9999;
    UINT32 u4VaTrans = 0x3000000;
    UINT8* hw_address = NULL;
    UINT8* SrcAddr = NULL;
    UINT8* ThirdAddr = NULL;
    UINT8* SrcPallete = NULL;
    UINT8* ThirdPallete = NULL;
    UINT8* golden_dataY = NULL;
    UINT8* DstAddr = NULL;
    //BOOL fgFirstTime = TRUE;
    UINT32 u4FrameSize = (1920 * 1200 * 4);
    UINT8* PA_address = NULL;
    UINT8* Golden_address = NULL;

    UINT32 u4GlobalAlpha, u4Opcode, u4AlcomAr, u4SrcPallte, u4ThirdPallete, u4GoldenSize;
    BOOL fgAlcomNormal, fgThirdEn, fgAlcomDiffCM;
    UINT32 u4TestType = 0;


	UINT32 u4Width_Com, u4Height_Com;
				UINT32 u4RollBackEn_Com, u4QualityMode_Com, u4BpCompAddrEnd_Com, u4ColorMode_Com;
				UINT32 u4SrcAddr_Com, u4DstAddr_Com, u4SrcPitch_Com, u4DstPitch_Com = 0;
				BOOL fgMirrorFlip_Com;
				BOOL fgBurstRead_Com;
				char* pcFileInfo_Com;
				char* pcToken_Com;
				UINT32 file_size1_Com, file_size2_Com, u4BytesPixel_Com = 1;
				UINT32 FileSize_Com;
				UINT32 index_Com = 0;
				//UINT32 u4RepeatFrame =9999;
				UINT8* SrcAddr_Com = NULL;
				UINT8* DstAddr_Com = NULL;
				UINT8* golden_data1_Com = NULL;
				UINT8* golden_data2_Com = NULL;
				UINT8* GoldenData_Com = NULL;
				UINT8* hw_address_Com = NULL;
				//BOOL fgFirstTime = TRUE;



    //golden_dataY = (UINT8*)BSP_AllocAlignedDmaMemory((1920 * 1088*4), 1024);

#if 0
    DstAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    golden_dataY = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    SrcAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    ThirdAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
#else
    DstAddr = (UINT8*)_u4GfxEmuBuf[0];
    golden_dataY = DstAddr + u4FrameSize;
    SrcAddr = golden_dataY + u4FrameSize;
    ThirdAddr = SrcAddr + u4FrameSize;


    hw_address = DstAddr;
    u4SrcAddr 	= (UINT32) SrcAddr;//(UINT32) 0x01ea4000;
    u4DstAddr 	= (UINT32) DstAddr;


	DstAddr_Com = (UINT8*)_u4GfxEmuBuf[0] - u4FrameSize;
			  golden_data1_Com = DstAddr_Com - u4FrameSize;
			  SrcAddr_Com = golden_data1_Com - u4FrameSize;
			  golden_data2_Com = SrcAddr_Com - u4FrameSize;
	
			  hw_address_Com = DstAddr_Com;
			  u4SrcAddr_Com 	= (UINT32) SrcAddr_Com;//(UINT32) 0x01ea4000;
			  u4DstAddr_Com 	= (UINT32) DstAddr_Com;

	
	     _CLI_CMD("gfx.reset");
        _CLI_CMD("gfxemu.mode");

      for (j = 0;j < 9;j++)  // UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
		   {
			   HalSet1MSectionTable(u4SrcAddr + j*0x100000, u4SrcAddr + u4VaTrans + j*0x100000);   //+48M
			   HalSet1MSectionTable(u4ThirdAddr + j*0x100000, u4ThirdAddr + u4VaTrans + j*0x100000);
			   HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr + u4VaTrans + j*0x100000);
			   HalSet1MSectionTable(u4SrcAddr_Com + j*0x100000, u4SrcAddr_Com - u4VaTrans + j*0x100000); //+48M
			   HalSet1MSectionTable(u4DstAddr_Com + j*0x100000, u4DstAddr_Com - u4VaTrans + j*0x100000);
						
		   }
		   //should blt dst raw data to PA 4K address
		   HalSet1MSectionTable(u4DstAddr , u4DstAddr);
		   x_memset((void*)0xa000000, 0, 0x400);
		   HalSet4KPageTable(u4DstAddr, u4DstAddr + u4VaTrans, (UINT32 *)0xa000000);
		   for (j = 1; j < 0x100; j++)
		   {
			   HalSet4KPageTable((u4DstAddr&0xfff00000) | (j << 12), ((u4DstAddr + u4VaTrans)&0xfff00000) | (j << 12), (UINT32 *)0xa000000);
		   }
		   HalSet4KPageTable(0xa801000, ((u4DstAddr + u4VaTrans)&0xfff00000) + 0x1000, (UINT32 *)0xa000000);
		   HalSet4KPageTable(0xa805000, ((u4DstAddr + u4VaTrans)&0xfff00000) + 0x2000, (UINT32 *)0xa000000);
		   _VGFX_SetTexture(340, 340, 340, 340);
                
		_GFX_MMU_Init();					
		_GFX_MMU_Set_TabAddr(HalGetMMUTableAddress());


    //SrcPallete = ThirdAddr + 1024;
    //ThirdPallete = SrcPallete + 1024;
#endif
   // _GFX_MMU_Enable(FALSE);
    x_memset((void*)DstAddr, 0, u4FrameSize);
    x_memset((void*)golden_dataY, 0, u4FrameSize);
    x_memset((void*)SrcAddr, 0, u4FrameSize);
    x_memset((void*)ThirdAddr, 0, u4FrameSize);

    SrcPallete = (UINT8 *)BSP_AllocAlignedDmaMemory((1024), 1024);
    ThirdPallete = (UINT8 *)BSP_AllocAlignedDmaMemory((1024), 1024);

    if (SrcAddr == NULL || ThirdAddr == NULL || SrcPallete == NULL || ThirdPallete == NULL || DstAddr == NULL || golden_dataY == NULL)
    {
        ASSERT(0);
    }

    Printf("General Alpha Composition Stress MMU Test ...\n");

    u4InitValue =  (UINT32) StrToInt(szArgv[1]);
    u4Times =  (UINT32) StrToInt(szArgv[2]);
    if (szArgv[3] != NULL)  u4TestType = (UINT32) StrToInt(szArgv[3]);

    //u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);

    pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);

    for (i = u4InitValue ; i < u4Times ; i++)
    {
        //if(i == (u4RepeatFrame+1))
        //    i--;
        UINT32 file_data[21] = {0};

        index = 0;


        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\emu_setup_%d.txt\" 0x%08x /ny\n", i, pcFileInfo);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\emu_setup_%d.txt\" 0x%08x /ny",
                            i, pcFileInfo));

        //Printf("string : %s\n",pcFileInfo);
        //sscanf(pcFileInfo,"%s %s %s %s %s %s",&file_data[0],&file_data[1],&file_data[2],&file_data[3],&file_data[4],&file_data[5]);

        pcToken = strtok(pcFileInfo, "\r\n");
        while ( pcToken != NULL)
        {
            file_data[index++] = StrToInt(pcToken);
            pcToken = strtok(NULL, "\r\n");
        }

        u4SrcAddr = (UINT32)SrcAddr;
        u4DstAddr = (UINT32)DstAddr;
        u4ThirdAddr = (UINT32)ThirdAddr;
        u4SrcPitch = (UINT32)file_data[2] * 16;
        u4DstPitch = (UINT32)file_data[3] * 16;
        u4SrcCM = (UINT32)file_data[4];
        u4Height = (UINT32)file_data[5] + 1;
        u4Width = (UINT32)file_data[6];
        u4GlobalAlpha = (UINT32)file_data[7];
        fgAlcomNormal = (BOOL)file_data[8];
        u4Opcode = file_data[9];
        u4AlcomAr = file_data[10];
        u4SrcPallte = (UINT32)SrcPallete;
        u4ThirdAddr = (UINT32)ThirdAddr;
        u4ThirdPallete = (UINT32)ThirdPallete;
        u4ThirdPitch = file_data[14] * 16;
        u4ThirdCM = file_data[15];
        fgThirdEn = (BOOL)file_data[16];
        fgAlcomDiffCM = (BOOL)file_data[17];
        u4DstCM = (UINT32)file_data[19];
        u4GoldenSize = file_data[20];

        //wanring solve
        u4SrcPallte = u4SrcPallte;
        u4ThirdPallete = u4ThirdPallete;
        fgAlcomDiffCM = fgAlcomDiffCM;

        //if(fgFirstTime) {
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_bmp_%d.raw\" 0x%08x /ny\n", i, u4SrcAddr);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_bmp_%d.raw\" 0x%08x /ny",
                            (i), u4SrcAddr));

        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_plt_%d.raw\" 0x%08x /ny\n", i, SrcPallete);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_plt_%d.raw\" 0x%08x /ny",
                            (i), SrcPallete));

        if (fgThirdEn)
        {
            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny\n", i, ThirdAddr);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny",
                                (i), ThirdAddr));

            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny\n", i, ThirdPallete);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny",
                                (i), ThirdPallete));
        }
        else
        {
            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny\n", i, ThirdAddr);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny",
                                (i), DstAddr));

            //LOG(0, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny\n",i,ThirdPallete);
            //UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny",
            //(i),ThirdPallete));
        }
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\dram_result_%d.raw\" 0x%08x /ny\n", (i), golden_dataY);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\dram_result_%d.raw\" 0x%08x /ny",
                            (i), golden_dataY));
        //}

        printf("%d %d %d\n", u4SrcCM, u4DstCM, u4ThirdCM);

   //     _CLI_CMD("gfx.reset");
    //    _CLI_CMD("gfxemu.mode");
		
        




        //test fill rect  use 1M table
   /*     for (j = 0;j < 9;j++)  // UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
        {
            HalSet1MSectionTable(u4SrcAddr + j*0x100000, u4SrcAddr + u4VaTrans + j*0x100000);   //+48M
            HalSet1MSectionTable(u4ThirdAddr + j*0x100000, u4ThirdAddr + u4VaTrans + j*0x100000);
            HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr + u4VaTrans + j*0x100000);
        }
        //should blt dst raw data to PA 4K address
        HalSet1MSectionTable(u4DstAddr , u4DstAddr);
        x_memset((void*)0xa000000, 0, 0x400);
        HalSet4KPageTable(u4DstAddr, u4DstAddr + u4VaTrans, (UINT32 *)0xa000000);
        for (j = 1; j < 0x100; j++)
        {
            HalSet4KPageTable((u4DstAddr&0xff00000) | (j << 12), ((u4DstAddr + u4VaTrans)&0xff00000) | (j << 12), (UINT32 *)0xa000000);
        }
        HalSet4KPageTable(0xa801000, ((u4DstAddr + u4VaTrans)&0xff00000) + 0x1000, (UINT32 *)0xa000000);
        HalSet4KPageTable(0xa805000, ((u4DstAddr + u4VaTrans)&0xff00000) + 0x2000, (UINT32 *)0xa000000);
        _VGFX_SetTexture(340, 340, 340, 340);

        _GFX_MMU_Init();
        _VGFX_SetTexture(350, 350, 350, 350);
        _GFX_MMU_Set_TabAddr(HalGetMMUTableAddress());
        _GFX_MMU_Enable(TRUE);
        _GFX_MMU_SelfFire();
        _VGFX_SetTexture(450, 450, 450, 450);
*/
        //_GFX_SetSrc((UINT8 *)u4SrcAddr, u4ColorMode, u4SrcPitch);
          _GFX_MMU_Set_Agent0(1, 0);
        if (u4TestType != 3)
        {
            _GFX_SetSrc((UINT8 *)(u4SrcAddr + u4VaTrans), u4SrcCM, u4SrcPitch);
            _GFX_SetThird((UINT8 *)(u4ThirdAddr + u4VaTrans), u4ThirdCM, u4ThirdPitch);

            _GFX_SetDst((UINT8 *)(u4DstAddr + u4VaTrans), u4DstCM, u4DstPitch);
        }
        else
            if (u4TestType == 3)
            {
                 _GFX_MMU_Set_Agent0(0, 0);
			_GFX_MMU_Set_Agent1(0, 4);
			_GFX_MMU_Set_Agent2(0, 1);
                _GFX_SetSrc((UINT8 *)(u4SrcAddr ), u4SrcCM, u4SrcPitch);
                _GFX_SetThird((UINT8 *)(u4ThirdAddr ), u4ThirdCM, u4ThirdPitch);

                _GFX_SetDst((UINT8 *)(u4DstAddr ), u4DstCM, u4DstPitch);
            }
        GFX_SetAlpha(u4GlobalAlpha);
        //GFX_SetAlpha(0xB3);

        _GFX_SetCharCM(TRUE, E_BMP_CM_8BIT);
        _GFX_SetCharCM(FALSE, E_BMP_CM_8BIT);

        _GFX_SetThirdPallete(ThirdPallete, 0, 0);
        _GFX_SetIdx2DirOpt(SrcPallete, 0, 0);       


        _GFX_GeneralAlphaCom(u4Width, u4Height, u4AlcomAr,
                             u4Opcode, 0, fgThirdEn, fgAlcomNormal, FALSE);
        hw_address = (UINT8*)(u4DstAddr);
      if (u4TestType == 0)
        {
            _CLI_CMD("gfx.flush");
        }


//******************************************************************
{                    
            Printf("Compression BitBlt MMU Enable Test 2 ...");

            //u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);

            fgMirrorFlip_Com = FALSE;

            pcFileInfo_Com = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);

            {
                int line_separate = 0;
                //if(i == (u4RepeatFrame+1))
                //	  i--;
                UINT32 file_data[10] = {0};

                index_Com = 0;

                x_memset((void*)pcFileInfo_Com, 0, 0x40000);
                LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_info_%d.txt\" 0x%08x /ny\n", i, pcFileInfo_Com);
                UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_info_%d.txt\" 0x%08x /ny",
                                    i, pcFileInfo_Com));

                pcToken_Com = strtok(pcFileInfo_Com, " ");
                while ( pcToken_Com != NULL)
                {
                    file_data[index_Com++] = StrToInt(pcToken_Com);
                    //Printf("bitstream_sz : %d \n",bitstream_sz[u2file_num-1]);
                    pcToken_Com = strtok(NULL, " ");
                }


                u4Width_Com 	  = (UINT32) file_data[0];
                u4Height_Com	  = (UINT32) file_data[1];
                u4ColorMode_Com = (UINT32) file_data[2];
                u4QualityMode_Com = (UINT32) file_data[3];
                u4RollBackEn_Com  = (UINT32) file_data[4];
                file_size1_Com = (UINT32) file_data[5];
                fgBurstRead_Com = (BOOL) file_data[6];
                file_size2_Com = (UINT32) file_data[7];
                u4BpCompAddrEnd_Com = 0xffffffff;
                //fgBurstRead = (BOOL)((UINT32)rand()%2);
                u4SrcPitch_Com = 0;


                if (u4ColorMode_Com == 2 || u4ColorMode_Com == 10)
                {
                    u4BytesPixel_Com = 1;
                }
                else if (u4ColorMode_Com == 6 || u4ColorMode_Com == 14)
                {
                    u4BytesPixel_Com = 4;
                }
                else if (u4ColorMode_Com == 13 || u4ColorMode_Com == 12 || u4ColorMode_Com == 11)
                {
                    u4BytesPixel_Com = 2;
                }
                else
                {
                    ASSERT(0);
                }

                u4SrcPitch_Com	  = u4Width_Com * u4BytesPixel_Com;


                //if(fgFirstTime) {
                LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_%d.txt\" 0x%08x /ny\n", i, u4SrcAddr_Com);
                UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_%d.txt\" 0x%08x /ny",
                                    (i), u4SrcAddr_Com));
                //x_thread_delay(1000);

                LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx\\emu_bitstream_line_connect_%d.txt\" 0x%08x /ny\n", (i), golden_data1_Com);
                UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx\\emu_bitstream_line_connect_%d.txt\" 0x%08x /ny",
                                    (i), golden_data1_Com));

                LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx\\emu_bitstream_line_separate_%d.txt\" 0x%08x /ny\n", (i), golden_data2_Com);
                UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx\\emu_bitstream_line_separate_%d.txt\" 0x%08x /ny",
                                    (i), golden_data2_Com));
                //x_thread_delay(1000);
                //}

                if (u4TestType == 0)
                {
                   // _CLI_CMD("gfx.reset");
                   // _CLI_CMD("gfxemu.mode");
                }
                for (line_separate = 0; line_separate < 2 ;line_separate++)
                {
                    //printf("\nclear dst buffer\n");
                  //  _GFX_MMU_Enable(FALSE);
                    // _GFX_MMU_SelfFire();
                    _GFX_SetDst((UINT8 *)(u4DstAddr_Com - u4VaTrans), 14, 1920*4);
                    _GFX_SetColor(0x0);
                    _GFX_Fill(0, 0, 1920, 1200);
                    if (u4TestType == 0)
                    {
                        _CLI_CMD("gfx.flush");
                    }
                    //printf("\nclear dst buffer\n");

                    //line-separate mode
                    if (line_separate == 1)
                    {
                        UINT32 original_line_bit = u4Width_Com * u4BytesPixel_Com * 8;

                        UINT32 line_bit;
                        if (u4RollBackEn_Com)
                            line_bit = original_line_bit + 4;
                        else
                            line_bit = original_line_bit + 4 + original_line_bit / 16;
                        if (fgBurstRead_Com)
                            line_bit = (line_bit % 512 == 0) ? line_bit : (line_bit / 512 + 1) * 512;
                        else
                            line_bit = (line_bit % 128 == 0) ? line_bit : (line_bit / 128 + 1) * 128;

                        u4DstPitch_Com = line_bit / 8;
                        //u4SrcPitch = line_bit/8;
                        //u4Width = line_bit/8;
                        //u4Width /= u4BytesPixel;

                        printf("width : %d\n", u4Width_Com);

                        GoldenData_Com = golden_data2_Com;
                        FileSize_Com = file_size2_Com;
                    }
                    else
                    {
                        u4DstPitch_Com = u4SrcPitch_Com;
                        GoldenData_Com = golden_data1_Com;
                        FileSize_Com = file_size1_Com;
                    }
                     _GFX_MMU_Set_Agent0(TRUE, 3); 
         
                    if (u4TestType != 3)
                    {
                        _GFX_NewCompressBlt((UINT8 *)(u4SrcAddr_Com - u4VaTrans), 0, 0, u4ColorMode_Com, u4SrcPitch_Com,
                                            (UINT8 *)(u4DstAddr_Com - u4VaTrans), u4Width_Com, u4Height_Com, u4Height_Com, u4RollBackEn_Com,
                                            u4QualityMode_Com, u4BpCompAddrEnd_Com, fgMirrorFlip_Com, fgBurstRead_Com, line_separate, u4DstPitch_Com);
                    }
                    else if (u4TestType == 3)
                    {
                        //_GFX_MMU_Enable(FALSE);
                        //_GFX_MMU_SelfFire();
						_GFX_MMU_Set_Agent0(0, 3);
						  _GFX_MMU_Set_Agent1(0, 4);
						  _GFX_MMU_Set_Agent2(0, 1);

                        //printf("\nnew osd compression\n");
                        _GFX_NewCompressBlt((UINT8 *)(u4SrcAddr_Com), 0, 0, u4ColorMode_Com, u4SrcPitch_Com,
                                            (UINT8 *)(u4DstAddr_Com ), u4Width_Com, u4Height_Com, u4Height_Com, u4RollBackEn_Com,
                                            u4QualityMode_Com, u4BpCompAddrEnd_Com, fgMirrorFlip_Com, fgBurstRead_Com, line_separate, u4DstPitch_Com);
                    }
                    //printf("\nnew osd compression\n");
           
                        _CLI_CMD("gfx.flush");
                  
                    //LOG(0,"file_size : %d\n",file_size);
                    HalFlushInvalidateDCache();
                    for (j = 0; j < FileSize_Com ; j++)
                    {
                        if (hw_address_Com[j] != GoldenData_Com[j])
                        {
                            printf("hw : %x, golden : %x, j = %d\n", hw_address_Com[j], GoldenData_Com[j], j);
                            printf("compare error in emu_source_%d.txt %d\n", i, line_separate);
                            printf("BPCOMP_CHKSUM : %x\n", IO_READ32(GFX_ADDR, (0xb8)));
                            printf("BPCOMP_NIPPLE : %x\n", IO_READ32(GFX_ADDR, (0xb4)));
                            return 1;
                            //break;
                        }
                        else
                        {
                            //LOG(0,"hw : %x , golden : %x\n",hw_address[j],golden_dataY[j]);
                        }
                    }
                }
                printf("compare ok MMU Enable in emu_source_%d.txt\n\n", i);
                //fgFirstTime = FALSE;
            }
            Printf("Compression BitBlt MMU Enable Test 2  OK... \n");

        }

//******************************************************************


        //LOG(0,"file_size : %d\n",file_size);
        //HalFlushInvalidateDCache();
        // should blt dst raw data to PA 4K address
        Golden_address = (UINT8*)golden_dataY;
        PA_address = (UINT8*)hw_address;;
        for ( j = 0; j < 0x1000; j ++)  //4K
        {
            if (PA_address[j] != Golden_address[j])
            {
                printf("hw : %x, golden : %x, j = %d\n", PA_address[j], Golden_address[j], j);
                printf("gfx alpha composition enable MMU compare error  0 in emu_setup_%d.txt\n\n", i);
                //printf("BPCOMP_CHKSUM : %x\n",IO_READ32(GFX_ADDR, (0xb8)));
                //printf("BPCOMP_NIPPLE : %x\n",IO_READ32(GFX_ADDR, (0xb4)));
                return 1;
                //break;
            }
        }
        Golden_address = (UINT8*)(golden_dataY + 0x3000);
        PA_address = (UINT8*)(hw_address + 0x3000);
        u4GoldenSize = u4GoldenSize - 3 * 0x1000;

        for (j = 0; j < u4GoldenSize ; j++)
        {
            if (PA_address[j] != Golden_address[j])
            {
                printf("hw : %x, golden : %x, j = %d\n", PA_address[j], Golden_address[j], j);
                printf("gfx alpha composition enable MMU  compare error 3 in emu_setup_%d.txt\n\n", i);
                return 1;
                //break;
            }
            else
            {
                //LOG(0,"hw : %x , golden : %x\n",hw_address[j],golden_dataY[j]);
            }
        }
        printf("New alpha composition MMU open  comparing ok in emu_source_%d.txt\n\n", i);
        //fgFirstTime = FALSE;
    }



    return 0;
}


static INT32 _GfxEmuIOMMU(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4InitValue = 0, u4Times = 1;
    u4InitValue =  (UINT32) StrToInt(szArgv[1]);
    u4Times =  (UINT32) StrToInt(szArgv[2]);

    if ((u4InitValue == 0) || (u4Times == 0))
    {
        u4InitValue = 1;
        u4Times = 2;
    }

    Printf("graphic iommu test start......\n");
	 _CLI_CMD("gfxemu.mmu_fiil");
    //_GfxEmuIOMMUFill();
    _GfxEmuIOMMUBlt();
    _GfxEmuIOMMUGeneralAlcom(u4InitValue, u4Times);
    _GfxEmuIOMMUCompress2(u4InitValue, u4Times);
    _GFX_MMU_Enable(FALSE);

    // Printf("graphic iommu test OK ......\n");

    return 0;
}

#endif
//-----------------------------------------------------------------------------
/** _GfxGenColor
 *
 */
//-----------------------------------------------------------------------------
static UINT32 _GfxGenColor(UINT32 u4Idx, UINT32 u4ColorMode)
{
    UINT8 i, j, u1R = 0, u1G = 0, u1B = 0;
    UINT32 u4Color = 0;

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

    if (u4ColorMode == (UINT32)CM_ARGB4444_DIRECT16)
    {
        //4444
        u4Color = 0xf000 | ((u1R << 4) & 0x0f00) | (u1G & 0x00f0) | (u1B >> 4);
        u4Color |= u4Color << 16;
    }
    else if (u4ColorMode == (UINT32)CM_ARGB8888_DIRECT32)
    {
        //8888...big endian
        u4Color = 0xff | (u1R << 8) | (u1G << 16 ) | (u1B << 24);
    }
    return u4Color;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdWrap1
 *  Gfx cmdque wrap Test 01 (for cmdque buffer size = 32 KB)
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdWrap1(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Loop, i;
    UINT32 u4Idx, u4Size;
    UINT32 u4Width1 = 1360;
    UINT32 u4Height1 = 768;
    UINT32 u4ColorMode = 14;
    UINT32 u4GfxWidth, u4GfxPitch;
    UINT32 u4OsdPitch, u4OsdPitch1;
    UINT32 u4Width, u4Height, u4Start = 0;

    if (i4Argc < 5)
    {
        Printf("args: loop, w, h1, h2");
        return -1;
    }

    Printf("Cmdque Wrap Test 01 ");

    i4Loop   = (INT32) StrToInt(szArgv[1]);
    u4Width  =         StrToInt(szArgv[2]);
    u4Start  =         StrToInt(szArgv[3]);
    u4Height =         StrToInt(szArgv[4]);

    // allocate memory for gfx canvas
    u4GfxWidth = u4Width + u4Height;
    //OSD_BYTE_PER_PIXEL(u4ColorMode, u4GfxPitch);
    u4GfxPitch = u4GfxWidth * 4;
    //_AllocateMemory(u4GfxPitch, 0xf, &pu1OrigGfxBuf, &pu1GfxBuf);

    // bitblt from gfx canvas to osd canvas
    //OSD_BYTE_PER_PIXEL(u4ColorMode, u4OsdPitch);
    u4OsdPitch = u4Width * 4;
    u4OsdPitch1 = u4Width1 * 4;

    for (i = 0; i < i4Loop; i++)
    {
        Printf("%d Run : ", i);
        
        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        GFX_SetDst((UINT8 *)BUF_SCREEN, u4ColorMode, u4OsdPitch1);
        _CLI_CMD("gfxemu.color 0xFF000000");
        _CLI_CMD("gfx.op.rect 0 0 %d %d", u4Width1, u4Height1);
        _CLI_CMD("gfx.flush");

        // Action        
        // draw gfx canvas
        GFX_SetDst((UINT8 *)BUF_TEMP, u4ColorMode, u4GfxPitch);
        for (u4Idx = 0; u4Idx < u4GfxWidth; u4Idx++)
        {
            GFX_SetColor(_GfxGenColor(u4Idx, u4ColorMode));
            GFX_VLine(u4Idx, 0, 1);
        }
        GFX_Flush();
        GFX_Wait();

        GFX_SetSrc((UINT8 *)BUF_TEMP, u4ColorMode, u4GfxPitch);
        GFX_SetDst((UINT8 *)BUF_SCREEN, u4ColorMode, u4OsdPitch);
        for (u4Idx = u4Start; u4Idx < u4Height; u4Idx++)
        {
            GFX_BitBlt(u4Idx, 0, 0, u4Idx, u4Width, 1);
        }
        GFX_Flush();
        GFX_Wait();


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        GFX_SetDst((UINT8 *)COMPARE_BUF, u4ColorMode, u4OsdPitch1);
        _CLI_CMD("gfxemu.color 0xFF000000");
        _CLI_CMD("gfx.op.rect 0 0 %d %d", u4Width1, u4Height1);
        _CLI_CMD("gfx.flush");

        // Action        
        // draw gfx canvas
        GFX_SetDst((UINT8 *)BUF_TEMP, u4ColorMode, u4GfxPitch);
        for (u4Idx = 0; u4Idx < u4GfxWidth; u4Idx++)
        {
            GFX_SetColor(_GfxGenColor(u4Idx, u4ColorMode));
            GFX_VLine(u4Idx, 0, 1);
        }
        GFX_Flush();
        GFX_Wait();

        GFX_SetSrc((UINT8 *)BUF_TEMP, u4ColorMode, u4GfxPitch);
        GFX_SetDst((UINT8 *)COMPARE_BUF, u4ColorMode, u4OsdPitch);
        for (u4Idx = u4Start; u4Idx < u4Height; u4Idx++)
        {
            GFX_BitBlt(u4Idx, 0, 0, u4Idx, u4Width, 1);
        }
        GFX_Flush();
        GFX_Wait();

        // Compare HW and SW
        u4Size = u4Width * u4Height;
        if (_GfxBufferCompare(
                (UINT32 *)COMPARE_BUF, 
                (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("Error \n", i);
            return -1;
        }
    } // ~for

    return 0;
}


//-------------------------------------------------------------------------
/** _GfxEmuCmdWorkBuf01
 *  WorkBuf Test 01 (random)
 *      Check whether gfx hw writes beyond end of _au1WorkingBuf or not ?
 *      Use mid to protect memory
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdWorkBuf01(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Count, i, i4CountNum = 5000;
    UINT32 u4SrcX, u4SrcY, u4DstX, u4DstY;
    UINT32 u4Width, u4Height, u4SrcPitch, u4DstPitch;
    UINT32 u4BitbltCount, u4ComposeCount;
    UINT32 u4Ar, u4Mode;


    Printf("Alpha Composition : Working Buffer Test 01 (random) ");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
        
    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        Printf("(%4d run) : \n", i4Count);

        u4BitbltCount  = ((UINT32)rand()) % 1000;
        u4ComposeCount = ((UINT32)rand()) % 1000;

        Printf("  Do Bitblt  Runs = %4u\n", u4BitbltCount);
        Printf("  Do Compose Runs = %4u\n", u4ComposeCount);

        // do several bitblt
        for (i = 0; i < u4BitbltCount; i++)
        {
            u4SrcX = ((UINT32)rand()) % _u4ScreenWidth;
            u4SrcY = ((UINT32)rand()) % _u4ScreenHeight;
            u4DstX = ((UINT32)rand()) % _u4ScreenWidth;
            u4DstY = ((UINT32)rand()) % _u4ScreenHeight;

            u4Width  = ((UINT32)rand()) % 2048;
            u4Width++;
            
            u4Height = ((UINT32)rand()) % _u4ScreenHeight;
            u4Height++;

            u4SrcPitch = ((u4Width * 4) + 0xf) & (~0xf);
            u4DstPitch = u4SrcPitch;

            Printf("  Dst Pitch = %u\n", u4DstPitch);
            
            GFX_SetSrc((UINT8*)BUF_ADDR1,  CM_ARGB8888_DIRECT32, u4SrcPitch);
            GFX_SetDst((UINT8*)BUF_SCREEN, CM_ARGB8888_DIRECT32, u4DstPitch);
            _CLI_CMD("gfx.set.bltopt 0 0 0");
            _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height);
        } // ~for

        // do several composition
        for (i = 0; i < u4ComposeCount; i++)
        {
            u4SrcX = ((UINT32)rand()) % _u4ScreenWidth;
            u4SrcY = ((UINT32)rand()) % _u4ScreenHeight;
            u4DstX = ((UINT32)rand()) % _u4ScreenWidth;
            u4DstY = ((UINT32)rand()) % _u4ScreenHeight;

            u4Ar   = ((UINT32)rand()) % 256;
            u4Mode = ((UINT32)rand()) % 8;

            u4Width  = ((UINT32)rand()) % 2048;
            u4Width++;
            
            u4Height = ((UINT32)rand()) % 100;
            u4Height++;

            u4SrcPitch = ((u4Width * 4) + 0xf) & (~0xf);
            u4DstPitch = u4SrcPitch;

            Printf("  Dst Pitch = %u\n", u4DstPitch);

            // do several composition
            GFX_SetSrc((UINT8*)BUF_ADDR1,  CM_ARGB8888_DIRECT32, u4SrcPitch);
            GFX_SetDst((UINT8*)BUF_SCREEN, CM_ARGB8888_DIRECT32, u4DstPitch);
            GFX_SetAlpha(u4Ar);
            _GFX_Compose(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height, 
                u4Ar, u4Mode);
            
        } // ~for

        _CLI_CMD("gfx.flush");

    } // ~for
    Printf("Alpha Composition : ");
    Printf("Working Buffer Test 01 (%d runs) ... OK !\n", i4CountNum);
    return 0;
}

static INT32 _GfxEmuCmdSDFifoThrs   (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4SdFifoValue;

    if (i4Argc < 2)
    {
        Printf("argv: sd_fifo_value (0~3)\n");
        return -1;
    }

    u4SdFifoValue = StrToInt(szArgv[1]);
    GFX_HwSetSDFifoThreshold(u4SdFifoValue);
    Printf("Set SD Fifo Thres = %u\n", u4SdFifoValue);
    return 0;
}


static INT32 _GfxEmuCmdCMDFifoThrs   (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4CmdFifoValue;

    if (i4Argc < 2)
    {
        Printf("argv: cmd_fifo_value (0~3)\n");
        return -1;
    }

    u4CmdFifoValue = StrToInt(szArgv[1]);
    GFX_HwSetCMDFifoThreshold(u4CmdFifoValue);
    Printf("Set CMD Fifo Thres = %u\n", u4CmdFifoValue);
    return 0;
}


static INT32 _GfxEmuCmdPOSTFifoThrs   (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4PostFifoValue;

    if (i4Argc < 2)
    {
        Printf("argv: post_fifo_value (0~3)\n");
        return -1;
    }

    u4PostFifoValue = StrToInt(szArgv[1]);
    GFX_HwSetPOSTFifoThreshold(u4PostFifoValue);
    Printf("Set POST Fifo Thres = %u\n", u4PostFifoValue);
    return 0;
}

// the time of filling cmds by CPU
static void _GetGfxCmdsTime(HAL_TIME_T rStart, HAL_TIME_T rEnd)
{
    HAL_TIME_T rDiffTime;
    UINT32 u4MilliSec;

    // get time difference
    HAL_GetDeltaTime(&rDiffTime, &rStart, &rEnd);
    Printf("----- [CPU Time, Fill Cmds] -----\n");
    Printf("\tdiff time: %u (s) %u (us)\n", rDiffTime.u4Seconds, rDiffTime.u4Micros);
    
    u4MilliSec = (rDiffTime.u4Seconds * 1000) + (rDiffTime.u4Micros / 1000);
    Printf("\t     time: %u (ms)\n", u4MilliSec);
}

static UINT64 _GetGfxOpTime(void)
{
    HAL_TIME_T rTime1, rTime2, rDiffTime;
    UINT32 u4MilliSec;

    HAL_GetTime(&rTime1);   // get start time
    GFX_Flush();
    GFX_Wait();
    HAL_GetTime(&rTime2);   // get end time

    // get time difference
    HAL_GetDeltaTime(&rDiffTime, &rTime1, &rTime2);
    Printf("\n----- [Engine Time] -----\n");
    Printf("\tdiff time: %u (s) %u (us)\n", rDiffTime.u4Seconds, rDiffTime.u4Micros);

    u4MilliSec = (rDiffTime.u4Seconds * 1000) + (rDiffTime.u4Micros / 1000);
    return u4MilliSec;
}


static void _GfxPerfShow(UINT64 u8Pixel)
{
    UINT32 u4MilliSec, u4PixelPerSec, u4High, u4Low;
    //UINT32 u4Part1, u4Part2, u4Part3;
    UINT64 u8MilliSec;

    u4MilliSec = _GetGfxOpTime();

    if (u8Pixel > 0xffffffff)
    {
        u4High = (UINT32)(u8Pixel >> 32);
        u4Low  = (UINT32)(u8Pixel & 0xffffffff);
        Printf("\ttotal pixel : 0x%x%x\n", u4High, u4Low);
    }
    else
    {
        u4Low = (UINT32)(u8Pixel & 0xffffffff);
        Printf("\ttotal pixel : 0x%x\n", u4Low);
    }
    Printf("\ttotal time  : %u (ms)\n", u4MilliSec);

    u8MilliSec = (UINT64)u4MilliSec;
    u4PixelPerSec = (UINT32)((u8Pixel * 1000) / u8MilliSec);
    Printf("\tpixels/sec   : %u\n\n", u4PixelPerSec);
    return;
}


static UINT32 _GfxPerfShow2(UINT64 u8Pixel)
{
    UINT32 u4MilliSec;
    UINT32 u4PixelPerSec;

    u4MilliSec = _GetGfxOpTime();
    u4PixelPerSec = (UINT32)((u8Pixel * 1000) / u4MilliSec);
    Printf("\tpixels/sec   : %u\n", u4PixelPerSec);
    return u4PixelPerSec;
}


// For Samsang
// OSD Bitblt speed test
static INT32 _GfxEmuCmdOsdBitblt(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 OSD_BUFFER_SIZE = (960 * 540 * sizeof(UINT32));
    HAL_TIME_T rTimeStart;
    HAL_TIME_T rTimeEnd;
    HAL_TIME_T rTimeDiff;

    void *pOsdBuf1;
    void *pOsdBuf2;
    INT32 i;
    INT32 i4Loops = 2000;

    if (i4Argc < 2)
    {
        Printf("argv: loops\n");
        return -1;
    }

    i4Loops = (INT32) StrToInt(szArgv[1]);
    
    pOsdBuf1 = x_mem_alloc(OSD_BUFFER_SIZE);
    pOsdBuf2 = x_mem_alloc(OSD_BUFFER_SIZE);

    HAL_GetTime(&rTimeStart);
    for (i = 0; i < i4Loops; i++)
    {
        x_memcpy(pOsdBuf1, pOsdBuf2, OSD_BUFFER_SIZE);
    }
    HAL_GetTime(&rTimeEnd);
    HAL_GetDeltaTime(&rTimeDiff, &rTimeStart, &rTimeEnd);
    Printf("x_memcpy : %u.%06u\n", rTimeDiff.u4Seconds, rTimeDiff.u4Micros);

    HAL_GetTime(&rTimeStart);
    for (i = 0; i < i4Loops; i++)
    {
        memcpy(pOsdBuf1, pOsdBuf2, OSD_BUFFER_SIZE);
    }
    HAL_GetTime(&rTimeEnd);
    HAL_GetDeltaTime(&rTimeDiff, &rTimeStart, &rTimeEnd);
    Printf("memcpy : %u.%06u\n", rTimeDiff.u4Seconds, rTimeDiff.u4Micros);


    GFX_SetSrc((UINT8*)pOsdBuf2, CM_ARGB8888_DIRECT32, 960*4);
    GFX_SetDst((UINT8*)pOsdBuf1, CM_ARGB8888_DIRECT32, 960*4);

    HAL_GetTime(&rTimeStart);
    for (i = 0; i < i4Loops; i++)
    {
        //memcpy(pOsdBuf1, pOsdBuf2, OSD_BUFFER_SIZE);
        GFX_BitBlt(0, 0, 0, 0, 960, 540);
    }
    GFX_Flush();
    GFX_Wait();
    HAL_GetTime(&rTimeEnd);
    HAL_GetDeltaTime(&rTimeDiff, &rTimeStart, &rTimeEnd);
    Printf("GFX_BitBlt : %u.%06u\n", rTimeDiff.u4Seconds, rTimeDiff.u4Micros);

    x_mem_free(pOsdBuf1);
    x_mem_free(pOsdBuf2);
    return 0;
}


static INT32 _GfxEmuCmdPerf1(INT32 i4Argc, const CHAR ** szArgv)
{
    HAL_TIME_T rStartTime, rEndTime;//, rDeltaTime;
    //INT32 i4ColorIndex, i4Iter, i4Loop, i4TotalRuns;
    INT32 i4Loop, i4TotalRuns;
    UINT64 u8Pixel;
    UINT32 u4OpMask = 0;
    UINT32 u4TestWidth  = _u4ScreenWidth;
    UINT32 u4TestHeight = _u4ScreenHeight;
    UINT32 u4TestPitch  = u4TestWidth * 4;
    UINT32 u4TestColorMode=(UINT32)CM_ARGB8888_DIRECT32;


    Printf("GFX performance test 01 (fifo thres is default)\n\n");
    if (!_fgTimerInited)
    {
        HAL_InitTimer();
        _fgTimerInited = TRUE;
    }

    if (i4Argc < 2)
    {
        Printf("Arg: num\n");
        Printf("\t 1: Rect Fill\n");
        Printf("\t 2: Normal BitBlt\n");
        Printf("\t 3: Alpha Blending\n");
        Printf("\t 4: Alpha Composition (src_over)\n");
        Printf("\t 5: YCbCr to RGB (420-linear)\n");
        Printf("\t 6: YCbCr to RGB (422-linear)\n");
        Printf("\t 7: Alpha Map\n");
        Printf("\t 8: Alpha Composition Loop (src_over)\n");
        Printf("\t 9: Rop BitBlt\n");
        Printf("\t10: Horizontal Line to Vertical Line\n");
        return -1;
    }
    
    if (i4Argc > 1)
    {
        INT32 i4N = 1;
        Printf("Selected perf op list :");
        while(i4N < i4Argc)
        {
            INT32 i4Op;
            i4Op = StrToInt(szArgv[i4N]);
            Printf(" %s{%d}", szArgv[i4N], i4Op);
            i4N++;
            if (i4Op < 0)
            {
                break;
            }
            u4OpMask |= 1 << i4Op;
        }
        Printf("\n");
    }

    if (u4OpMask == 0 || u4OpMask == 1)
    {
        u4OpMask = ~0;
    }

    if (x_strcmp(_szCurColorMode, "argb8888") == 0)
    {
        u4TestColorMode = (UINT32)CM_ARGB8888_DIRECT32;
    }

    if (x_strcmp(_szCurColorMode, "argb4444") == 0)
    {
        u4TestColorMode = (UINT32)CM_ARGB4444_DIRECT16;
    }
    
    #define MA_GFX_PERF_BEGIN(X, BIT)   \
        if ((X) & (1 << BIT))           \
        {                               \
            _CLI_CMD("gfx.reset");      \
            _CLI_CMD("gfxemu.mode");    \


    #define MA_GFX_PERF_END()           \
        }                               \


    // 1. Rect Fill
    MA_GFX_PERF_BEGIN(u4OpMask, 1)
        Printf("1. Rect Fill -\n");
        Printf("\tprepare cmd q ... ");
        i4TotalRuns = 200;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * u4TestHeight);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetColor(_arNamedColor[0].u4Color);
            GFX_Fill(0, 0, u4TestWidth, u4TestHeight);
        }
        HAL_GetTime(&rEndTime);   // get end time
        Printf("done\n");
        _GetGfxCmdsTime(rStartTime, rEndTime);
        _GfxPerfShow(u8Pixel);
    MA_GFX_PERF_END()

    // 2. Normal BitBlt
    MA_GFX_PERF_BEGIN(u4OpMask, 2)
        Printf("2. Normal Bitblt -\n");
        Printf("\tprepare cmd q ... ");
        i4TotalRuns = 110;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * u4TestHeight);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetSrc((UINT8*)BUF_ADDR1,  u4TestColorMode, u4TestPitch);
            GFX_SetBltOpt(0, 0x00000000, 0xffffffff);
            GFX_BitBlt(0, 0, 0, 0, u4TestWidth, u4TestHeight);
        }
        HAL_GetTime(&rEndTime);   // get end time
        Printf("done\n");
        _GetGfxCmdsTime(rStartTime, rEndTime);
        _GfxPerfShow(u8Pixel);
    MA_GFX_PERF_END()

    // 3. Alpha Blending
    MA_GFX_PERF_BEGIN(u4OpMask, 3)
        Printf("3. Alpha Blending -\n");
        Printf("\tprepare cmd q...");
        i4TotalRuns = 125;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * u4TestHeight);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetSrc((UINT8*)BUF_ADDR1,  u4TestColorMode, u4TestPitch);
            GFX_SetAlpha(239);
            GFX_Blend(0, 0, 0, 0, u4TestWidth, u4TestHeight);
        }
        HAL_GetTime(&rEndTime);   // get end time
        Printf("done\n");
        _GetGfxCmdsTime(rStartTime, rEndTime);
        _GfxPerfShow(u8Pixel);
    MA_GFX_PERF_END()

    // 4. Alpha Composition (SRC_OVER)
    MA_GFX_PERF_BEGIN(u4OpMask, 4)
        Printf("4. Alpha Composition (src_over) -\n");
        Printf("\tprepare cmd q...");
        i4TotalRuns = 1;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * 300);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetSrc((UINT8*)BUF_ADDR1,  u4TestColorMode, u4TestPitch);
            _GFX_Compose(0, 0, 0, 0, u4TestWidth, 300, 239, 7);
        }
        HAL_GetTime(&rEndTime);   // get end time
        Printf("done\n");
        _GetGfxCmdsTime(rStartTime, rEndTime);
        _GfxPerfShow(u8Pixel);
    MA_GFX_PERF_END()
    
    // 5. YCbCr to RGB (420 linear mode)
    MA_GFX_PERF_BEGIN(u4OpMask, 5)
        Printf("5. YCbCr2RGB (420-linear)-\n");
        Printf("\tprepare cmd q...");
        i4TotalRuns = 100;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * u4TestHeight);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetColCnvFmt((UINT32)E_YCFMT_420LINEAR, (UINT32)E_SWAP_BLOCK, 
                (UINT32)E_VSTD_BT709, (UINT32)E_VSYS_COMP);
            GFX_SetColCnvSrc((UINT8*)BUF_ADDR1, u4TestPitch, (UINT8*)BUF_ADDR2, u4TestPitch, 0);
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetAlpha(239);
            GFX_ColConv(0, 0, u4TestWidth, u4TestHeight);
        }
        HAL_GetTime(&rEndTime);   // get end time
        Printf("done\n");
        _GetGfxCmdsTime(rStartTime, rEndTime);
        _GfxPerfShow(u8Pixel);
    MA_GFX_PERF_END()

    // 6. YCbCr to RGB (422 linear mode)
    MA_GFX_PERF_BEGIN(u4OpMask, 6)
        Printf("6. YCbCr2RGB (422-linear)-\n");
        Printf("\tprepare cmd q...");
        i4TotalRuns = 100;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * u4TestHeight);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetColCnvFmt((UINT32)E_YCFMT_422LINEAR, (UINT32)E_SWAP_BLOCK, 
                (UINT32)E_VSTD_BT709, (UINT32)E_VSYS_COMP);
            GFX_SetColCnvSrc((UINT8*)BUF_ADDR1, u4TestPitch, (UINT8*)BUF_ADDR2, u4TestPitch, 0);
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetAlpha(239);
            GFX_ColConv(0, 0, u4TestWidth, u4TestHeight);
        }
        HAL_GetTime(&rEndTime);   // get end time
        Printf("done\n");
        _GetGfxCmdsTime(rStartTime, rEndTime);
        _GfxPerfShow(u8Pixel);
    MA_GFX_PERF_END()

    // 7. Alpha Map
    MA_GFX_PERF_BEGIN(u4OpMask, 7)
        Printf("7. Alpha Map -\n");
        Printf("\tprepare cmd q...");
        i4TotalRuns = 170;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * u4TestHeight);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetSrc((UINT8*)BUF_ALPHAMAP, (UINT32)CM_RGB_CLUT8, _u4ScreenWidth);
            GFX_AlphaMapBitBlt(0, 0, 0, 0, u4TestWidth, u4TestHeight);
        }
        HAL_GetTime(&rEndTime);   // get end time
        Printf("done\n");
        _GetGfxCmdsTime(rStartTime, rEndTime);
        _GfxPerfShow(u8Pixel);
    MA_GFX_PERF_END()

    // 8. Alpha Composition Loop (SRC_OVER)
    MA_GFX_PERF_BEGIN(u4OpMask, 8)
        Printf("8. Alpha Composition Loop (src_over) -\n");
        Printf("\tprepare cmd q...");
        i4TotalRuns = 145;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * u4TestHeight);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetSrc((UINT8*)BUF_ADDR1,  u4TestColorMode, u4TestPitch);
            GFX_ComposeLoop(0, 0, 0, 0, u4TestWidth, u4TestHeight, i4Loop, 7, 0);
        }
        HAL_GetTime(&rEndTime);   // get end time
        Printf("done\n");
        _GetGfxCmdsTime(rStartTime, rEndTime);
        _GfxPerfShow(u8Pixel);
    MA_GFX_PERF_END()

    // 9. Rop BitBlt
    MA_GFX_PERF_BEGIN(u4OpMask, 9)
        Printf("9. Rop BitBlt -\n");
        Printf("\tprepare cmd q...");
        i4TotalRuns = 145;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * u4TestHeight);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetSrc((UINT8*)BUF_ADDR1,  u4TestColorMode, u4TestPitch);
            GFX_SetRopOpt(6);
            GFX_RopBitBlt(0, 0, 0, 0, u4TestWidth, u4TestHeight);
        }
        HAL_GetTime(&rEndTime);   // get end time
        Printf("done\n");
        _GetGfxCmdsTime(rStartTime, rEndTime);
        _GfxPerfShow(u8Pixel);
    MA_GFX_PERF_END()

    // 10. Horizontal Line to Vertical Line
    MA_GFX_PERF_BEGIN(u4OpMask, 10)
        Printf("10. Horizontal Line to Vertical Line -\n");
        Printf("\tprepare cmd q...");
        i4TotalRuns = 125;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * u4TestHeight);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetSrc((UINT8*)BUF_ADDR1,  u4TestColorMode, u4TestPitch);
            GFX_SetHoriToVertLineOpt(0);
            GFX_HoriToVertLine(0, 0, 0, 0, u4TestHeight);
        }
        HAL_GetTime(&rEndTime);   // get end time
        Printf("done\n");
        _GetGfxCmdsTime(rStartTime, rEndTime);
        _GfxPerfShow(u8Pixel);
    MA_GFX_PERF_END()
    return 0;
}


static INT32 _GfxEmuCmdPerf2    (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4ColorIndex;
    INT32 i4Loop, i4TotalRuns;
    UINT64 u8Pixel;
    INT32 i4Iter;
    UINT32 u4OpMask = 0;
    UINT32 ui, uj, uk;
    UINT32 u4PixPerSec, u4Count;
    UINT32 u4BestPixPerSecIndex, u4BestPixPerSec;
    UINT32 u4WorstPixPerSecIndex, u4WorstPixPerSec;

    Printf("GFX performance test 02 (all fifo thres combination)\n\n");
    if (!_fgTimerInited)
    {
        HAL_InitTimer();
        _fgTimerInited = TRUE;
    }

    if (i4Argc < 2)
    {
        Printf("Arg: num\n");
        Printf("\t1: Rect Fill\n");
        Printf("\t2: Normal BitBlt\n");
        Printf("\t3: Transparent BitBlt\n");
        Printf("\t4: Alpha Blending\n");
        Printf("\t5: Alpha Composition (src_over)\n");
        Printf("\t6: YCbCr to RGB (block)\n");
        Printf("\t7: YCbCr to RGB (linear)\n");
        Printf("\t8: Alpha Map\n");
        return -1;
    }
    
    if (i4Argc > 1)
    {
        INT32 i4N = 1;
        Printf("Selected perf op list :");
        while(i4N < i4Argc)
        {
            INT32 i4Op;
            i4Op = StrToInt(szArgv[i4N]);
            Printf(" %s{%d}", szArgv[i4N], i4Op);
            i4N++;
            if (i4Op < 0)
            {
                break;
            }
            u4OpMask |= 1 << i4Op;
        }
        Printf("\n");
    }

    if (u4OpMask == 0 || u4OpMask == 1)
    {
        u4OpMask = ~0;
    }

    #define MA_GFX_PERF_2_BEGIN(X, BIT)                 \
        if ((X) & (1 << BIT))                           \
        {                                               \
            u4Count = 0;                                \
            u4BestPixPerSec = 0;                        \
            u4BestPixPerSecIndex = 0;                   \
            u4WorstPixPerSec = 0xffffffff;              \
            u4WorstPixPerSecIndex = 0;                  \
            for (ui = 0; ui <= 3; ui++)                 \
            {                                           \
                GFX_HwSetSDFifoThreshold(ui);           \
                for (uj = 0; uj <= 3; uj++)             \
                {                                       \
                    GFX_HwSetCMDFifoThreshold(uj);      \
                    for (uk = 0; uk <= 3; uk++)         \
                    {                                   \
                        GFX_HwSetPOSTFifoThreshold(uk); \
                        u4Count++;                      \


    #define MA_GFX_PERF_2_END()                                 \
                        u4PixPerSec = _GfxPerfShow2(u8Pixel);   \
                        if (u4PixPerSec > u4BestPixPerSec)      \
                        {                                       \
                            u4BestPixPerSec = u4PixPerSec;      \
                            u4BestPixPerSecIndex = u4Count;     \
                        }                                       \
                        if (u4PixPerSec < u4WorstPixPerSec)     \
                        {                                       \
                            u4WorstPixPerSec = u4PixPerSec;     \
                            u4WorstPixPerSecIndex = u4Count;    \
                        }                                       \
                    }                                           \
                }                                               \
            }                                                   \
            Printf("\n(%2u) %u = best pix per sec\n",           \
                u4BestPixPerSecIndex, u4BestPixPerSec);         \
            Printf("\n(%2u) %u = worst pix per sec\n",          \
                u4WorstPixPerSecIndex, u4WorstPixPerSec);       \
        }                                                       \


    // back to HW mode
    _CLI_CMD("gfx.reset");
    _CLI_CMD("gfxemu.mode");
        
    // 1. Rect Fill
    MA_GFX_PERF_2_BEGIN(u4OpMask, 1)
        Printf("(%2u) Rect Fill - ", u4Count);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        i4TotalRuns = 20;
        u8Pixel = (UINT64)i4TotalRuns * (UINT64)147 * 
                  (UINT64)_u4ScreenWidth * (UINT64)_u4ScreenHeight;
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            for (i4ColorIndex = 0; i4ColorIndex < 147; i4ColorIndex++)
            {
                _CLI_CMD("gfxemu.color 0x%08x", _arNamedColor[i4ColorIndex].u4Color);
                _CLI_CMD("gfx.op.rect 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
            }
        }
    MA_GFX_PERF_2_END()

    // 2. Normal BitBlt
    MA_GFX_PERF_2_BEGIN(u4OpMask, 2)
        Printf("(%2u) Normal Bitblt - ", u4Count);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        i4TotalRuns = 500;
        u8Pixel = (UINT64)i4TotalRuns * (UINT64)3 * 
                  (UINT64)_u4ScreenWidth * (UINT64)_u4ScreenHeight;
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
            _CLI_CMD("gfx.op.bitblt 0 0 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
            _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
            _CLI_CMD("gfx.op.bitblt 0 0 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
            _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
            _CLI_CMD("gfx.op.bitblt 0 0 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);

        }
    MA_GFX_PERF_2_END()

    // 3. Transparent BitBlt
    MA_GFX_PERF_2_BEGIN(u4OpMask, 3)
        Printf("(%2u) Transparent Bitblt - ", u4Count);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        i4TotalRuns = 2;
        u8Pixel = (UINT64)i4TotalRuns * (UINT64)256 * (UINT64)3 * 
                  (UINT64)_u4ScreenWidth * (UINT64)_u4ScreenHeight;
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            for (i4Iter = 0; i4Iter < 256; i4Iter++)
            {
                _CLI_CMD("gfx.set.bltopt 0x00%02x%02x%02x 0xff%02x%02x%02x transparent",
                    0, 0, 0, i4Iter, i4Iter, i4Iter);
                _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
                _CLI_CMD("gfx.op.bitblt 0 0 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
                _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
                _CLI_CMD("gfx.op.bitblt 0 0 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
                _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
                _CLI_CMD("gfx.op.bitblt 0 0 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
            }
        }
    MA_GFX_PERF_2_END()

    // 4. Alpha Blending
    MA_GFX_PERF_2_BEGIN(u4OpMask, 4)
        Printf("(%2u) Alpha Blending - ", u4Count);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        i4TotalRuns = 10;
        u8Pixel = (UINT64)i4TotalRuns * (UINT64)256 * 
                  (UINT64)_u4ScreenWidth * (UINT64)_u4ScreenHeight;
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            for (i4Iter = 0; i4Iter < 256; i4Iter++)
            {
                switch((i4Loop + i4Iter) % 3)
                {
                case 0:
                    _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
                    break;
                case 1:
                    _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
                    break;
                case 2:
                    _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
                    break;
                default:
                    break;
                }
                _CLI_CMD("gfx.set.alpha %d", i4Iter);
                _CLI_CMD("gfx.op.blend 0 0 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
            }
        }
    MA_GFX_PERF_2_END()

    // 5. Alpha Composition (SRC_OVER)
    MA_GFX_PERF_2_BEGIN(u4OpMask, 5)
        Printf("(%2u) Alpha Composition (src_over) - ", u4Count);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        i4TotalRuns = 1;
        u8Pixel = (UINT64)i4TotalRuns * (UINT64)16 * 
                  (UINT64)_u4ScreenWidth * (UINT64)_u4ScreenHeight;
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            for (i4Iter = 0; i4Iter < 256; i4Iter+=16)
            {
                switch((i4Loop + i4Iter) % 3)
                {
                case 0:
                    _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
                    break;
                case 1:
                    _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
                    break;
                case 2:
                    _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
                    break;
                default:
                    break;
                }
                _CLI_CMD("gfx.op.compose 0 0 0 0 %u %u %d 7", 
                    _u4ScreenWidth, _u4ScreenHeight, i4Iter);
            }
        }
    MA_GFX_PERF_2_END()
    
    // 6. YCbCr to RGB (Block mode)
    MA_GFX_PERF_2_BEGIN(u4OpMask, 6)
        Printf("(%2u) YCbCr2RGB (block) - ", u4Count);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.cnvfmt 420mb block bt601 video");
        _CLI_CMD("gfxemu.black");
        i4TotalRuns = 200;
        u8Pixel = (UINT64)i4TotalRuns * (UINT64)7 * 
                  (UINT64)_u4ScreenWidth * (UINT64)_u4ScreenHeight;
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            _CLI_CMD("gfx.set.cnvsrc 0x03000000 0x6000 0x0305A000 0x3000");
            _CLI_CMD("gfx.op.colorcnv 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
            _CLI_CMD("gfx.set.cnvsrc 0x03087000 0x6000 0x030E1000 0x3000");
            _CLI_CMD("gfx.op.colorcnv 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
            _CLI_CMD("gfx.set.cnvsrc 0x0310E000 0x6000 0x03168000 0x3000");
            _CLI_CMD("gfx.op.colorcnv 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
            _CLI_CMD("gfx.set.cnvsrc 0x03195000 0x6000 0x031EF000 0x3000");
            _CLI_CMD("gfx.op.colorcnv 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
            _CLI_CMD("gfx.set.cnvsrc 0x0321C000 0x6000 0x03276000 0x3000");
            _CLI_CMD("gfx.op.colorcnv 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
            _CLI_CMD("gfx.set.cnvsrc 0x032A3000 0x6000 0x032FD000 0x3000");
            _CLI_CMD("gfx.op.colorcnv 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
            _CLI_CMD("gfx.set.cnvsrc 0x0332A000 0x6000 0x03384000 0x3000");
            _CLI_CMD("gfx.op.colorcnv 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
        }
    MA_GFX_PERF_2_END()

    // 7. YCbCr to RGB (Linear mode)
    MA_GFX_PERF_2_BEGIN(u4OpMask, 7)
        Printf("(%2u) YCbCr2RGB (linear)- ", u4Count);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.cnvfmt 420lin block bt601 video");
        _CLI_CMD("gfxemu.black");
        i4TotalRuns = 200;
        u8Pixel = (UINT64)i4TotalRuns * (UINT64)7 * 
                  (UINT64)_u4ScreenWidth * (UINT64)_u4ScreenHeight;
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            _CLI_CMD("gfx.set.cnvsrc 0x03000000 0x6000 0x0305A000 0x3000");
            _CLI_CMD("gfx.op.colorcnv 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
            _CLI_CMD("gfx.set.cnvsrc 0x03087000 0x6000 0x030E1000 0x3000");
            _CLI_CMD("gfx.op.colorcnv 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
            _CLI_CMD("gfx.set.cnvsrc 0x0310E000 0x6000 0x03168000 0x3000");
            _CLI_CMD("gfx.op.colorcnv 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
            _CLI_CMD("gfx.set.cnvsrc 0x03195000 0x6000 0x031EF000 0x3000");
            _CLI_CMD("gfx.op.colorcnv 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
            _CLI_CMD("gfx.set.cnvsrc 0x0321C000 0x6000 0x03276000 0x3000");
            _CLI_CMD("gfx.op.colorcnv 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
            _CLI_CMD("gfx.set.cnvsrc 0x032A3000 0x6000 0x032FD000 0x3000");
            _CLI_CMD("gfx.op.colorcnv 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
            _CLI_CMD("gfx.set.cnvsrc 0x0332A000 0x6000 0x03384000 0x3000");
            _CLI_CMD("gfx.op.colorcnv 0 0 %u %u", _u4ScreenWidth, _u4ScreenHeight);
        }
    MA_GFX_PERF_2_END()

    // 8. Alpha Map
    MA_GFX_PERF_2_BEGIN(u4OpMask, 8)
        Printf("(%2u) Alpha Map - ", u4Count);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.s.src 0x%08x RGB8 %d", BUF_ALPHAMAP, _u4ScreenWidth);
        i4TotalRuns = 4000;
        u8Pixel = (UINT64)i4TotalRuns * (UINT64)_u4ScreenWidth * (UINT64)_u4ScreenHeight;
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            _CLI_CMD("gfx.op.am 0 0 0 0 %d %d", _u4ScreenWidth, _u4ScreenHeight);
        }
    MA_GFX_PERF_2_END()

    return 0;
}



static INT32 _GfxEmuCmdPerf3(INT32 i4Argc, const CHAR ** szArgv)
{
    HAL_TIME_T rStartTime, rEndTime;//, rDeltaTime;
    //INT32 i4ColorIndex, i4Iter, i4Loop, i4TotalRuns;
    INT32 i4Loop, i4TotalRuns;
    UINT64 u8Pixel;
    UINT32 u4OpMask = 0;
    UINT32 u4TestWidth  = _u4ScreenWidth;
    UINT32 u4TestHeight = _u4ScreenHeight;
    UINT32 u4TestPitch  = u4TestWidth * 4;
    UINT32 u4TestColorMode=(UINT32)CM_ARGB8888_DIRECT32;
	HAL_TIME_T rDiffTime;
    UINT32 u4MilliSec;
	UINT32 u4PixelPerSec;


    Printf("GFX performance test 01 (fifo thres is default)\n\n");
    if (!_fgTimerInited)
    {
        HAL_InitTimer();
        _fgTimerInited = TRUE;
    }

    if (i4Argc < 2)
    {
        Printf("Arg: num\n");
        Printf("\t 1: Rect Fill\n");
        Printf("\t 2: Normal BitBlt\n");
        Printf("\t 3: Alpha Blending\n");
        Printf("\t 4: Alpha Composition (src_over)\n");
        Printf("\t 5: YCbCr to RGB (420-linear)\n");
        Printf("\t 6: YCbCr to RGB (422-linear)\n");
        Printf("\t 7: Alpha Map\n");
        Printf("\t 8: Alpha Composition Loop (src_over)\n");
        Printf("\t 9: Rop BitBlt\n");
        Printf("\t10: Horizontal Line to Vertical Line\n");
        return -1;
    }
    
    if (i4Argc > 1)
    {
        INT32 i4N = 1;
        Printf("Selected perf op list :");
        while(i4N < i4Argc)
        {
            INT32 i4Op;
            i4Op = StrToInt(szArgv[i4N]);
            Printf(" %s{%d}", szArgv[i4N], i4Op);
            i4N++;
            if (i4Op < 0)
            {
                break;
            }
            u4OpMask |= 1 << i4Op;
        }
        Printf("\n");
    }

    if (u4OpMask == 0 || u4OpMask == 1)
    {
        u4OpMask = ~0;
    }

    if (x_strcmp(_szCurColorMode, "argb8888") == 0)
    {
        u4TestColorMode = (UINT32)CM_ARGB8888_DIRECT32;
    }

    if (x_strcmp(_szCurColorMode, "argb4444") == 0)
    {
        u4TestColorMode = (UINT32)CM_ARGB4444_DIRECT16;
    }
    
    #define MA_GFX_PERF_BEGIN(X, BIT)   \
        if ((X) & (1 << BIT))           \
        {                               \
            _CLI_CMD("gfx.reset");      \
            _CLI_CMD("gfxemu.mode");    \


    #define MA_GFX_PERF_END()           \
        }                               \


    // 1. Rect Fill
    MA_GFX_PERF_BEGIN(u4OpMask, 1)
        Printf("1. Rect Fill -\n");
        i4TotalRuns = 200;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * u4TestHeight);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetColor(_arNamedColor[0].u4Color);
            GFX_Fill(0, 0, u4TestWidth, u4TestHeight);
			GFX_Flush();
    		GFX_Wait();
        }
        HAL_GetTime(&rEndTime);   // get end time
        HAL_GetDeltaTime(&rDiffTime, &rStartTime, &rEndTime);
        u4MilliSec = (rDiffTime.u4Seconds * 1000) + (rDiffTime.u4Micros / 1000);
		u4PixelPerSec = (UINT32)((u8Pixel * 1000) / u4MilliSec/1024/1024);
		Printf("\t Rect Fill rate = %uMpixels/sec  u8Pixel =%u, u4MilliSec=%ld\n", u4PixelPerSec,u8Pixel,u4MilliSec);
		
    MA_GFX_PERF_END()

    // 2. Normal BitBlt
    MA_GFX_PERF_BEGIN(u4OpMask, 2)
        Printf("2. Normal Bitblt -\n");
        i4TotalRuns = 110;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * u4TestHeight);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetSrc((UINT8*)BUF_ADDR1,  u4TestColorMode, u4TestPitch);
            GFX_SetBltOpt(0, 0x00000000, 0xffffffff);
            GFX_BitBlt(0, 0, 0, 0, u4TestWidth, u4TestHeight);
			GFX_Flush();
    		GFX_Wait();
        }
        HAL_GetTime(&rEndTime);   // get end time
        HAL_GetDeltaTime(&rDiffTime, &rStartTime, &rEndTime);
        u4MilliSec = (rDiffTime.u4Seconds * 1000) + (rDiffTime.u4Micros / 1000);
		u4PixelPerSec = (UINT32)((u8Pixel * 1000) / u4MilliSec/1024/1024);
		Printf("\t Normal BitBlt rate = %uMpixels/sec  u8Pixel =%u, u4MilliSec=%ld\n", u4PixelPerSec,u8Pixel,u4MilliSec);
    MA_GFX_PERF_END()

    // 3. Alpha Blending
    MA_GFX_PERF_BEGIN(u4OpMask, 3)
        Printf("3. Alpha Blending -\n");
        Printf("\tprepare cmd q...");
        i4TotalRuns = 125;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * u4TestHeight);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetSrc((UINT8*)BUF_ADDR1,  u4TestColorMode, u4TestPitch);
            GFX_SetAlpha(239);
            GFX_Blend(0, 0, 0, 0, u4TestWidth, u4TestHeight);
        }
        HAL_GetTime(&rEndTime);   // get end time
        Printf("done\n");
        _GetGfxCmdsTime(rStartTime, rEndTime);
        _GfxPerfShow(u8Pixel);
    MA_GFX_PERF_END()

    // 4. Alpha Composition (SRC_OVER)
    MA_GFX_PERF_BEGIN(u4OpMask, 4)
        Printf("4. Alpha Composition (src_over) -\n");
        Printf("\tprepare cmd q...");
        i4TotalRuns = 1;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * 300);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetSrc((UINT8*)BUF_ADDR1,  u4TestColorMode, u4TestPitch);
            _GFX_Compose(0, 0, 0, 0, u4TestWidth, 300, 239, 7);
        }
        HAL_GetTime(&rEndTime);   // get end time
        Printf("done\n");
        _GetGfxCmdsTime(rStartTime, rEndTime);
        _GfxPerfShow(u8Pixel);
    MA_GFX_PERF_END()
    
    // 5. YCbCr to RGB (420 linear mode)
    MA_GFX_PERF_BEGIN(u4OpMask, 5)
        Printf("5. YCbCr2RGB (420-linear)-\n");
        Printf("\tprepare cmd q...");
        i4TotalRuns = 100;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * u4TestHeight);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetColCnvFmt((UINT32)E_YCFMT_420LINEAR, (UINT32)E_SWAP_BLOCK, 
                (UINT32)E_VSTD_BT709, (UINT32)E_VSYS_COMP);
            GFX_SetColCnvSrc((UINT8*)BUF_ADDR1, u4TestPitch, (UINT8*)BUF_ADDR2, u4TestPitch, 0);
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetAlpha(239);
            GFX_ColConv(0, 0, u4TestWidth, u4TestHeight);
        }
        HAL_GetTime(&rEndTime);   // get end time
        Printf("done\n");
        _GetGfxCmdsTime(rStartTime, rEndTime);
        _GfxPerfShow(u8Pixel);
    MA_GFX_PERF_END()

    // 6. YCbCr to RGB (422 linear mode)
    MA_GFX_PERF_BEGIN(u4OpMask, 6)
        Printf("6. YCbCr2RGB (422-linear)-\n");
        Printf("\tprepare cmd q...");
        i4TotalRuns = 100;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * u4TestHeight);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetColCnvFmt((UINT32)E_YCFMT_422LINEAR, (UINT32)E_SWAP_BLOCK, 
                (UINT32)E_VSTD_BT709, (UINT32)E_VSYS_COMP);
            GFX_SetColCnvSrc((UINT8*)BUF_ADDR1, u4TestPitch, (UINT8*)BUF_ADDR2, u4TestPitch, 0);
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetAlpha(239);
            GFX_ColConv(0, 0, u4TestWidth, u4TestHeight);
        }
        HAL_GetTime(&rEndTime);   // get end time
        Printf("done\n");
        _GetGfxCmdsTime(rStartTime, rEndTime);
        _GfxPerfShow(u8Pixel);
    MA_GFX_PERF_END()

    // 7. Alpha Map
    MA_GFX_PERF_BEGIN(u4OpMask, 7)
        Printf("7. Alpha Map -\n");
        Printf("\tprepare cmd q...");
        i4TotalRuns = 170;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * u4TestHeight);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetSrc((UINT8*)BUF_ALPHAMAP, (UINT32)CM_RGB_CLUT8, _u4ScreenWidth);
            GFX_AlphaMapBitBlt(0, 0, 0, 0, u4TestWidth, u4TestHeight);
        }
        HAL_GetTime(&rEndTime);   // get end time
        Printf("done\n");
        _GetGfxCmdsTime(rStartTime, rEndTime);
        _GfxPerfShow(u8Pixel);
    MA_GFX_PERF_END()

    // 8. Alpha Composition Loop (SRC_OVER)
    MA_GFX_PERF_BEGIN(u4OpMask, 8)
        Printf("8. Alpha Composition Loop (src_over) -\n");
        i4TotalRuns = 145;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * u4TestHeight);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetSrc((UINT8*)BUF_ADDR1,  u4TestColorMode, u4TestPitch);
            GFX_ComposeLoop(0, 0, 0, 0, u4TestWidth, u4TestHeight, i4Loop, 7, 0);
			GFX_Flush();
    		GFX_Wait();

        }
        HAL_GetTime(&rEndTime);   // get end time
        HAL_GetDeltaTime(&rDiffTime, &rStartTime, &rEndTime);
        u4MilliSec = (rDiffTime.u4Seconds * 1000) + (rDiffTime.u4Micros / 1000);
		u4PixelPerSec = (UINT32)((u8Pixel * 1000) / u4MilliSec/1024/1024);
		Printf("\t Alpha Composition Loop (SRC_OVER) = %uMpixels/sec u8Pixel =%u, u4MilliSec=%ld\n", u4PixelPerSec,u8Pixel, u4MilliSec);
    MA_GFX_PERF_END()

    // 9. Rop BitBlt
    MA_GFX_PERF_BEGIN(u4OpMask, 9)
        Printf("9. Rop BitBlt -\n");
        Printf("\tprepare cmd q...");
        i4TotalRuns = 145;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * u4TestHeight);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetSrc((UINT8*)BUF_ADDR1,  u4TestColorMode, u4TestPitch);
            GFX_SetRopOpt(6);
            GFX_RopBitBlt(0, 0, 0, 0, u4TestWidth, u4TestHeight);
        }
        HAL_GetTime(&rEndTime);   // get end time
        Printf("done\n");
        _GetGfxCmdsTime(rStartTime, rEndTime);
        _GfxPerfShow(u8Pixel);
    MA_GFX_PERF_END()

    // 10. Horizontal Line to Vertical Line
    MA_GFX_PERF_BEGIN(u4OpMask, 10)
        Printf("10. Horizontal Line to Vertical Line -\n");
        Printf("\tprepare cmd q...");
        i4TotalRuns = 125;
        u8Pixel = ((UINT64)i4TotalRuns * u4TestWidth * u4TestHeight);
        HAL_GetTime(&rStartTime);   // get start time
        for (i4Loop = 0; i4Loop < i4TotalRuns; i4Loop++)
        {
            GFX_SetDst((UINT8*)BUF_SCREEN, u4TestColorMode, u4TestPitch);
            GFX_SetSrc((UINT8*)BUF_ADDR1,  u4TestColorMode, u4TestPitch);
            GFX_SetHoriToVertLineOpt(0);
            GFX_HoriToVertLine(0, 0, 0, 0, u4TestHeight);
        }
        HAL_GetTime(&rEndTime);   // get end time
        Printf("done\n");
        _GetGfxCmdsTime(rStartTime, rEndTime);
        _GfxPerfShow(u8Pixel);
    MA_GFX_PERF_END()
    return 0;
}



#if 0
static INT32 _GfxTestText(INT32 i4Argc, const CHAR ** szArgv)
{
    unsigned char bitmap [] = 
    {
                        0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00,
                        0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00,
                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                        0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00,
                        0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00,
    };
    /*unsigned char bitmap [] =  {0x00,0x00,0x00,0x00,0x00,
                                0x00,0xff,0x00,0xff,0x00,
                                0x00,0x00,0x00,0x00,0x00,
                                0x00,0xff,0x00,0xff,0x00,
                                0x00,0x00,0x00,0x00,0x00,};*/
    //UINT32 u4Width, u4Height, u4DstPitch, u4MsbLeft, u4StByAl;
    UINT32 u4Width, u4Height,u4Size;
    //UINT8 *pu1SubPicTableAddr = NULL;
    //UINT32 i, u4Size, u4Align, u4TableEntryNum;
    UINT32 au4ColorMap[4], u4CM;
    
    Printf("Sub-picture Decode Test 01\n");

    if (i4Argc < 4)
    {
        Printf("arg: width, height, dst_cm_idx\n");
        return -1;
    }

    u4Width  = (UINT32) StrToInt(szArgv[1]);
    u4Height = (UINT32) StrToInt(szArgv[2]);
    u4CM     = (UINT32) StrToInt(szArgv[3]);

    // prepare sub-picture & colormap table
    {
        u4Size = (UINT32) sizeof(bitmap);
        Printf("size of sub_pic_table = %u\n", u4Size);
        Printf("src_w = %u, src_h = %u\n", u4Width, u4Height);
        Printf("sub_pic_table_address = 0x%08x\n", 
            (UINT32)bitmap);

        if (u4CM == CM_ARGB8888_DIRECT32)
        {
            au4ColorMap[0] = 0xFFFF0000; //red
            au4ColorMap[1] = 0x0;
            au4ColorMap[2] = 0x0;
            au4ColorMap[3] = 0xFFFFFFFF;//white
        }
        else
        {
            au4ColorMap[0] = 0xFF00;
            au4ColorMap[1] = 0x0;
            au4ColorMap[2] = 0x0;
            au4ColorMap[3] = 0xFFFF;
        }
    }

    // HW
    _CLI_CMD("gfx.reset");
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _CLI_CMD("gfxemu.cls");

    // HW Action
    // 1. set destination address
    // 2. set sub-picture source address
    // 3. set sub-pic-decode flag
    // 4. do txt/bmp mapping
    // 5. flush
    _GFX_SetDst((UINT8*)BUF_SCREEN, u4CM, _u4GfxPitch);

    _GFX_SetBmpSrc(bitmap, E_BMP_CM_2BIT);

    _GFX_SetColorMap(E_BMP_CM_2BIT, u4CM, au4ColorMap);
        
    _GFX_SetBmpOpt(FALSE, FALSE);

    _GFX_BmpBlt(100, 100, u4Width, u4Height);

    _GFX_SetBmpOpt(FALSE, FALSE);
    
    _CLI_CMD("gfx.flush");

    Printf("draw Text .... OK !\n");
    return 0;

}
#endif



#if 0
// use gfx hw to do memory compare
static INT32 _GfxHwBufferCompare(UINT8 *pu1Cm, UINT8 *pu1Hw, UINT32 u4Num)
{
    INT32 i4Ret;
    UINT32 u4TotalPixels = u4Num;
    
    if (u4Num <= 0)
    {
        Printf("Error: u4Num <= 0\n");
        return -1;
    }

    if (0 == x_strcmp(_szCurColorMode, "rgb8"))
    {
        u4Num = u4Num;
    }
    else if ((0 == x_strcmp(_szCurColorMode, "argb4444")) ||
             (0 == x_strcmp(_szCurColorMode, "argb1555")) ||
             (0 == x_strcmp(_szCurColorMode, "rgb565")))
    {
        u4Num = u4Num * 2;
    }
    else
    {
        u4Num = u4Num * 4;
    }

    Printf("\n");

    i4Ret = GFX_MemCompare(pu1Cm, pu1Hw, u4Num);

    Printf("Total Pixels = %u, Color Mode = %s \n", u4TotalPixels, _szCurColorMode);
            
    if (i4Ret != (INT32)E_GFX_OK)
    {
        Printf("[GFX_HW] HW/SW Comparison Fail ! \n");
        return -1;
    }
    else
    {
        Printf("[GFX_HW] HW/SW Comparison OK ! \n");
    }
    return 0;
}


static INT32 _GfxBufferCompare(UINT32 *pu4Cm, UINT32 *pu4Hw, UINT32 u4Num)
{
    _CLI_CMD("gfx.reset");
    _CLI_CMD("gfxemu.mode");
    return _GfxHwBufferCompare((UINT8 *)pu4Cm, (UINT8 *)pu4Hw, u4Num);
}
#else
static INT32 _GfxBufferCompare(UINT32 *pu4Cm, UINT32 *pu4Hw, UINT32 u4Num)
{
    INT32 i4Error = 0;
    UINT32 u4TotalPixels = u4Num;
    UINT32 u4MaxDiff = 0, u4Temp;
    
    if (u4Num <= 0)
    {
        Printf("Error: u4Num <= 0\n");
        return -1;
    }

    if ((0 == x_strcmp(_szCurColorMode, "argb4444")) ||
        (0 == x_strcmp(_szCurColorMode, "argb1555")) ||
        (0 == x_strcmp(_szCurColorMode, "rgb565")))
    {
        u4Num = u4Num / 2;
    }
    else if (0 == x_strcmp(_szCurColorMode, "rgb8"))
    {
        u4Num = u4Num / 4;
    }

    Printf("\n");
    while (u4Num--)
    {
        if (*pu4Cm != *pu4Hw)
        {
            ++i4Error;
            Printf("Compare error (%d) :\n", i4Error);
            Printf("    HW --> [0x%08x] = 0x%08x\n", pu4Hw, *pu4Hw);
            Printf("    CM --> [0x%08x] = 0x%08x\n", pu4Cm, *pu4Cm);

            if (*pu4Cm > *pu4Hw)
            {
                u4Temp = *pu4Cm - *pu4Hw;
            }
            else
            {
                u4Temp = *pu4Hw - *pu4Cm;
            }

            if (u4Temp > u4MaxDiff)
            {
                u4MaxDiff = u4Temp;
            }
            return -1;
        }
        pu4Cm++;
        pu4Hw++;
    }

    if (i4Error)
    {
        Printf("Total Pixels = %u, Color Mode = %s \n", 
            u4TotalPixels, _szCurColorMode);
        Printf("[CPU] HW/SW Comparison Fail => Error = %d (MaxDiff = %u)\n", 
            i4Error, u4MaxDiff);
        return -1;
    }
    else
    {
        Printf("Color Mode = %s \n", _szCurColorMode);
        Printf("[CPU] HW/SW Comparison OK ! \n");
    }
    return 0;
}
#endif


static INT32 _GfxEmuCmdStress(INT32 i4Argc, const CHAR ** szArgv)
{
    
    VERIFY(_CLI_CMD("gfxemu.premult 1 5000") == 0);
    VERIFY(_CLI_CMD("gfxemu.gencompose 1 500 0") == 0);     
    VERIFY(_CLI_CMD("gfxemu.compress2 2 3000 0") == 0);             
    VERIFY(_CLI_CMD("gfxemu.onepasscomp 1 2000 0") == 0);  
    VERIFY(_CLI_CMD("gfxemu.cmf 2 1000 0") == 0);       
    VERIFY(_CLI_CMD("gfxemu.go 678 0 255") == 0);            
       
    return 0;
}


static INT32 _GfxEmuCmdVFull(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret1, i4Ret2;

    _GFX_SetVFull(1);
    i4Ret1 = _CLI_CMD("gfxemu.ccnv2 0");
    i4Ret2 = _CLI_CMD("gfxemu.ccnv2 2");        

    if (i4Ret1 || i4Ret2)
    {
        Printf("YCbCr2RGB Full-Range Test Fail\n"); 
        return -1;
    }

    Printf("YCbCr2RGB Full-Range Test OK\n");     
     _GFX_SetVFull(0);

     return 0;
}


static INT32 _GfxEmuArgb1555(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4GlobalAlpha, u4ForceAlpha;
    UINT8* hw_address = NULL;
    UINT32 u4Size, i;
    

    Printf("ARGB1555 test\n");
    
    if (i4Argc < 3)
    {
        Printf("argv: global alpha value (0~255), ForceAlpha\n");
        return -1;
    }

    u4GlobalAlpha =  (UINT32) StrToInt(szArgv[1]);
    u4ForceAlpha =  (UINT32) StrToInt(szArgv[2]);   
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
    _CLI_CMD("gfxemu.cls");   

    Printf("(Src,Dst)=(0x%x, 0x%x)\n", BUF_ADDR1, BUF_SCREEN);   

    GFX_SetDst((UINT8*)BUF_ADDR1, (UINT32)CM_ARGB1555_DIRECT16, 1440);
    _CLI_CMD("gfxemu.color 0x1234");
    _CLI_CMD("gfx.op.rect 0 0 720 240");
    _CLI_CMD("gfxemu.color 0x8765");
    _CLI_CMD("gfx.op.rect 0 240 720 240");
    _CLI_CMD("gfx.flush");
  
    GFX_SetSrc((UINT8*)BUF_ADDR1, (UINT32)CM_ARGB1555_DIRECT16, 1440);
    GFX_SetDst((UINT8*)BUF_SCREEN, (UINT32)CM_ARGB8888_DIRECT32, 2880);
    _GFX_SetAlpha(u4GlobalAlpha);  
    _GFX_SetARGB1555(1);
    
    if (u4ForceAlpha)
    {
         _GFX_SetForceAlpha(1);    
    }
    
    GFX_SetBltOpt(0, 0, 0);
    GFX_BitBlt(0, 0, 0, 0, 720, 480);
    GFX_Flush();
    GFX_Wait();

    _GFX_SetForceAlpha(0);
    _GFX_SetARGB1555(0);


    u4Size = 720*480*4;
    
    hw_address = (UINT8*)BUF_SCREEN;

    if (u4ForceAlpha)
    {
	    for ( i = 3; i < u4Size; i+=4)
	    {
	        if (hw_address[i] != (UINT8)u4GlobalAlpha)   
	        {
	            Printf("ARGB1555 with Force Global Alpha Test Fail (0x%x, 0x%x)\n", hw_address[i], u4GlobalAlpha); 
	            return -1;
	        }
	    }    

	    Printf("ARGB1555 with Force Global Alpha Test OK\n"); 	    
    }
    else
    {
	    for ( i = 3; i < (u4Size/2); i+=4)
	    {
	        if (hw_address[i] != (UINT8)u4GlobalAlpha)   
	        {
	            Printf("ARGB1555 Test Fail 1 (0x%x, 0x%x)\n", hw_address[i], u4GlobalAlpha); 
	            return -1;	            
	        }
	    }

	    for ( i = ((u4Size/2)+3); i <u4Size ; i+=4)
	    {
	        if (hw_address[i] == (UINT8)u4GlobalAlpha)
	        {
	            Printf("ARGB1555 Test Fail 2 (0x%x, 0x%x)\n", hw_address[i], u4GlobalAlpha); 
	            return -1;	            
	        }
	    }
	    
	    Printf("ARGB1555  Test OK\n"); 	    	    
    	}
   

    return 0;
    
      
}


static INT32 _GfxEmuCmdComposeLoopTest(INT32 i4Argc, const CHAR ** szArgv)
{   
    UINT32 u4GlobalAlpha;
    UINT32 u4SrcKey;    
    UINT8* hw_address = NULL;
    UINT32 u4Size, i;    

    Printf("Alpha Composition Loop Test for Global Alpha/Color Keying\n");
    
    if (i4Argc < 2)
    {
        Printf("argv: global alpha value (0~255)\n");
        return -1;
    }
    Printf("(Src,Dst)=(0x%x, 0x%x)\n", BUF_ADDR1, BUF_SCREEN);   
    
    u4GlobalAlpha =  (UINT32) StrToInt(szArgv[1]);
    u4SrcKey =  (UINT32) StrToInt(szArgv[2]);

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _CLI_CMD("gfxemu.cls");

    // Action
    _GfxSetSdMode(BUF_ADDR1, ADR_SRC);

    _GFX_SetSrcColorKey(TRUE, u4SrcKey);
    GFX_SetAlpha(u4GlobalAlpha);
    _GFX_SetForceAlpha(1);
 
    
    _CLI_CMD("gfx.op.composeloop 0 0 0 0 %u %u %d %d %d", 
        _u4ScreenWidth, _u4ScreenHeight, 38, 4, 0);

    _CLI_CMD("gfx.flush");

    _GFX_SetForceAlpha(0);
    
    u4Size = _u4ScreenWidth*_u4ScreenHeight*4;
    
    hw_address = (UINT8*)BUF_SCREEN;


    for ( i = 3; i < u4Size; i+=4)
    {
        if (hw_address[i] != (UINT8)u4GlobalAlpha)   
        {
            Printf("Force to use Global Alpha Test Fail (0x%x, 0x%x)\n", hw_address[i], u4GlobalAlpha); 
            return -1;
        }
    }

    Printf("Alpha Composition Loop Test with Global Alpha OK\n");

    return 0;
}


static INT32 _GfxEmuCmdOverlapBlt(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size;
    UINT32 u4OverlapAddr;
   

    Printf("OverlapBlt Test ");

    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");

    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _CLI_CMD("gfxemu.cls");

    // Action 
    _GfxSetDefMode(BUF_TEMP, ADR_SRC);
    _GfxSetDefMode(BUF_SCREEN, ADR_DST);
    _CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
    _CLI_CMD("gfx.flush");

    u4Size = _u4ScreenWidth * _u4ScreenHeight;

    u4OverlapAddr = (BUF_TEMP-4);
    _GfxSetDefMode(u4OverlapAddr, ADR_DST);
    _CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
    _CLI_CMD("gfx.flush");


    if (_GfxBufferCompare(
            (UINT32 *)BUF_SCREEN, 
            (UINT32 *)(u4OverlapAddr), u4Size) == -1)
    {
        Printf("OverlapBlt- Test Error\n");
        return -1;
    }

    _GfxSetDefMode(u4OverlapAddr, ADR_SRC);
    u4OverlapAddr = (u4OverlapAddr + 4);
    _GfxSetDefMode(u4OverlapAddr, ADR_DST);
    _CLI_CMD("gfx.set.bltopt 0 0 0 overlap");
    _CLI_CMD("gfx.op.bitblt 0 0 0 0 720 480");
    _CLI_CMD("gfx.flush");

    if (_GfxBufferCompare(
            (UINT32 *)BUF_SCREEN, 
            (UINT32 *)(u4OverlapAddr), u4Size) == -1)
    {
        Printf("OverlapBlt+ Test Error\n");
        return -1;
    }

    Printf("Range(0x%x -- 0x%x)\n", BUF_SCREEN, (BUF_TEMP+0x151800-1));
    return 0;

}



//-------------------------------------------------------------------------
/** _GfxEmuCmdPremultAlcom
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdPremultAlcom(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height;
    UINT32 u4SrcAddr, u4DstAddr, u4SrcPitch, u4DstPitch;
    UINT32 u4SrcCM,u4DstCM;
    char* pcFileInfo;
    UINT32 i,u4Times,j;
    char* pcToken;
    UINT32 u4InitValue =0;
    UINT32 index =0;
    //UINT32 u4RepeatFrame =9999;

    UINT8* hw_address = NULL;
    UINT8* SrcAddr = NULL;  
    UINT8* golden_dataY = NULL;
    UINT8* DstAddr = NULL;
    //BOOL fgFirstTime = TRUE;
    UINT32 u4FrameSize = (1920 * 1200*4);
    
    UINT32 u4GlobalAlpha,u4Opcode,u4AlcomAr,u4GoldenSize, u4PremultSrcR, u4PremultDstR, u4PremultDstW, u4PremultOvrflw;
    BOOL fgAlcomNormal;
    //golden_dataY = (UINT8*)BSP_AllocAlignedDmaMemory((1920 * 1088*4), 1024);

    DstAddr = (UINT8*)_u4GfxEmuBuf[0];
    golden_dataY = DstAddr + u4FrameSize;
    SrcAddr = golden_dataY + u4FrameSize;
    
    if(SrcAddr == NULL ||  DstAddr==NULL || golden_dataY==NULL)
    {
        ASSERT(0);
    }
    
    Printf("Premultipliedl Alpha Composition Stress Test ...");

    u4InitValue =  (UINT32) StrToInt(szArgv[1]);
    u4Times =  (UINT32) StrToInt(szArgv[2]);
    //u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);

    pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);

    for(i=u4InitValue ; i<u4Times ; i++)
    {
        //if(i == (u4RepeatFrame+1))
        //    i--;
        UINT32 file_data[21] = {0};

        index =0;
        
        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"d:\\5365\\premult\\emu_setup_%d.txt\" 0x%08x /ny\n",i,pcFileInfo);
	    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\5365\\premult\\emu_setup_%d.txt\" 0x%08x /ny", 
		i,pcFileInfo));

        pcToken = strtok(pcFileInfo, "\r\n");
        while( pcToken != NULL) 
        { 
            file_data[index++] = StrToInt(pcToken);
            pcToken = strtok(NULL, "\r\n");
        }

        u4SrcAddr = (UINT32)SrcAddr;
        u4DstAddr = (UINT32)DstAddr;
        u4SrcPitch = (UINT32)file_data[2]*16;
        u4DstPitch = (UINT32)file_data[3]*16;
        u4SrcCM = (UINT32)file_data[4];
        u4Height = (UINT32)file_data[5]+1;
        u4Width = (UINT32)file_data[6];
        u4GlobalAlpha = (UINT32)file_data[7];
        fgAlcomNormal = (BOOL)file_data[8];
        u4Opcode = file_data[9];
        u4AlcomAr = file_data[10];
        u4DstCM = (UINT32)file_data[12];
        u4PremultOvrflw = (UINT32)file_data[13];
        u4PremultSrcR = (UINT32)file_data[14];
        u4PremultDstR = (UINT32)file_data[15];
        u4PremultDstW = (UINT32)file_data[16];        
        u4GoldenSize = file_data[17];
        
	
        //if(fgFirstTime) {
        LOG(10, "//d.load.binary \"d:\\5365\\premult\\random_src_%d.raw\" 0x%08x /ny\n",i,u4SrcAddr);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\5365\\premult\\random_src_%d.raw\" 0x%08x /ny", 
    		(i),u4SrcAddr));

        LOG(10, "//d.load.binary \"d:\\5365\\premult\\random_dst_%d.raw\" 0x%08x /ny\n",i,u4DstAddr);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\5365\\premult\\random_dst_%d.raw\" 0x%08x /ny", 
		(i),DstAddr));
    

        LOG(10, "//d.load.binary \"d:\\5365\\premult\\dram_result_%d.raw\" 0x%08x /ny\n",(i),golden_dataY);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\5365\\premult\\dram_result_%d.raw\" 0x%08x /ny", 
    		(i),golden_dataY));
	//}

        //Printf("Src = 0x%08x, sPitch = %d, Dst = 0x%08x, dPitch = %d, W = %d, H = %d, u4RollBackEn = %d,u4QualityMode = %d, u4BpCompAddrEnd = 0x%08x, u4ColorMode = %d fgBurstRead = %d\n",
        //    u4SrcAddr, u4SrcPitch, u4DstAddr, u4DstPitch, u4Width, u4Height, u4RollBackEn,u4QualityMode,u4BpCompAddrEnd,u4ColorMode,fgBurstRead);
        
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");
       
        _GFX_SetSrc((UINT8 *)u4SrcAddr, u4SrcCM, u4SrcPitch);
        _GFX_SetDst((UINT8 *)u4DstAddr, u4DstCM, u4DstPitch);

        GFX_SetAlpha(u4GlobalAlpha);        

        _GFX_SetCharCM(TRUE,E_BMP_CM_8BIT);
        _GFX_SetCharCM(FALSE,E_BMP_CM_8BIT);
        

        _GFX_SetPremult(u4PremultSrcR, u4PremultDstR, u4PremultDstW, u4PremultOvrflw);
        _GFX_GeneralAlphaCom(u4Width, u4Height,u4AlcomAr, 
            u4Opcode, 0, 0, fgAlcomNormal,FALSE);   

        hw_address = (UINT8*)u4DstAddr;
            
        _CLI_CMD("gfx.flush");

        //LOG(0,"file_size : %d\n",file_size);
        //HalFlushInvalidateDCache();

        for (j = 0; j < u4GoldenSize ; j++)
        {
            if (hw_address[j] != golden_dataY[j])
            {
                printf("hw : %x, golden : %x, j = %d\n",hw_address[j],golden_dataY[j],j);
                printf("compare error in emu_setup_%d.txt\n\n",i);
                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\emu_setup_%d_error.txt\" 0x%08x--0x%08x", 
    		(i),(UINT32)hw_address,(UINT32)hw_address+u4GoldenSize-1));
                return 1;
                //break;
            }
            else
            {
                //LOG(0,"hw : %x , golden : %x\n",hw_address[j],golden_dataY[j]);
            }
        }
        printf("Premultiplied alpha composition comparing ok in emu_setup_%d.txt\n\n",i);
        //fgFirstTime = FALSE;

        Printf("0x%x\n", IO_READ32(0xf0004000, 0xe4));
        
    }

    return 0;
}


static INT32 _GfxEmuCmdStretchAlcom(INT32 i4Argc, const CHAR ** szArgv)
{

    UINT32 u4Sx, u4Sy, u4Sw, u4Sh, u4Dx, u4Dy, u4Dw, u4Dh, u4Size;
    UINT32 u4Loop, u4CountNum, u4Opcode, u4PremultSrcR, u4PremultDstW, j, u4Ar;
    UINT8*  pucTwoPass;
    UINT8* pucOnePass;
       
    Printf("d.load.binary D:/MT5381_Emu/kodak16.8888 0x%08x /ny\n", BUF_ADDR1);
    Printf("d.load.binary D:/MT5381_Emu/kodak16.8888 0x%08x /ny\n", BUF_ADDR3);
    Printf("d.load.binary D:/MT5381_Emu/kodak16.8888 0x%08x /ny\n", BUF_SCREEN);
    LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny\n", BUF_ADDR1);
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny", BUF_ADDR1));
    LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny\n", BUF_ADDR3);
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny", BUF_ADDR3));
    LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny\n", BUF_SCREEN);
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny", BUF_SCREEN));


    if (i4Argc > 2) //stretch + premultiplied
    {
        u4CountNum = StrToInt(szArgv[1]);
        u4PremultSrcR = StrToInt(szArgv[2]);
        u4PremultDstW = StrToInt(szArgv[3]);
        u4Opcode = 0x4;        
        u4Ar = 255;                
    }
    else if (i4Argc > 1) // stretch + alpha composition
    {
        u4CountNum = StrToInt(szArgv[1]);
        u4PremultSrcR = 0;
        u4PremultDstW = 0;        
        u4Opcode = ((UINT32)rand()) % (((UINT32)E_AC_XOR) + 1); 
        if ((((UINT32)rand()) % 4) == 0)
        {
            u4Ar = 255;            
        }
        else
        {
            u4Ar = ((UINT32)rand()) % 256;
        }
    }
    else
    {
        u4CountNum = 1;
        u4Opcode = 0x7;
        u4PremultSrcR = 0;
        u4PremultDstW = 0;        
    }
       
    // HW
#ifdef GFX_HW_RESET
    _CLI_CMD("gfx.reset");
#endif
    _CLI_CMD("gfxemu.mode");
    
    // init
    _CLI_CMD("gfx.set.bltopt 0 0 0");

    // do many in loop
    for (u4Loop = 0; u4Loop < u4CountNum; u4Loop++)
    {
        UINT32 u4Max, u4Min;

        // select source w,h
        u4Sw = (((UINT32)rand()) % (MAX_SRC_WIDTH - MIN_SRC_WIDTH - 1)) + MIN_SRC_WIDTH;
        u4Sh = (((UINT32)rand()) % (MAX_SRC_HEIGHT - MIN_SRC_HEIGHT - 1)) + MIN_SRC_HEIGHT;
        if (u4Sw == 0)
        {
            u4Sw++;
        }
        if (u4Sh == 0)
        {
            u4Sh++;
        }

        // select dest w
        u4Max = ((u4Sw * D_SCALE_UP) > (MAX_DST_WIDTH - 1)) ? (MAX_DST_WIDTH - 1) : (u4Sw * D_X_SCALE_UP);
        u4Min = ((u4Sw / D_SCALE_DN) < MIN_DST_WIDTH) ? MIN_DST_WIDTH : (u4Sw / D_X_SCALE_DN);
        u4Dw = (((UINT32)rand()) % (u4Max - u4Min + 1)) + u4Min;

        // select dest h
        u4Max = ((u4Sh * D_SCALE_UP) > (MAX_DST_HEIGHT - 1)) ? (MAX_DST_HEIGHT - 1) : (u4Sh * D_Y_SCALE_UP);
        u4Min = ((u4Sh / D_SCALE_DN) < MIN_DST_HEIGHT) ? MIN_DST_HEIGHT : (u4Sh / D_Y_SCALE_DN);
        u4Dh = (((UINT32)rand()) % (u4Max - u4Min + 1)) + u4Min;

        // select source x,y
        u4Sx = 0;
        u4Sy = 0;
        
        // select dest x,y
        u4Dx = 0;
        u4Dy = 0;

#if 0
       //for test
       u4Sw = 149;
       u4Sh = 167;
       u4Dw = 516;
       u4Dh = 91;
#endif       

        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetDefMode(BUF_ADDR2, ADR_DST);
        _CLI_CMD("gfx.op.stretchblt %d %d %d %d %d %d %d %d",
                u4Sx, u4Sy, u4Sw, u4Sh,
                u4Dx, u4Dy, u4Dw, u4Dh);
        Printf("(%d, %d, %d, %d)\n", u4Sw, u4Sh, u4Dw, u4Dh);

        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _GfxSetDefMode(BUF_ADDR3, ADR_DST);        

        if ((u4PremultSrcR == 1) || (u4PremultDstW == 1))
        {
            _GFX_SetPremult(u4PremultSrcR, 0, u4PremultDstW, 1);
        }

	_GFX_GeneralAlphaCom(u4Dw, u4Dh, u4Ar, u4Opcode, 0, 0, 1, FALSE);   

        _CLI_CMD("gfx.flush");


        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        
        if ((u4PremultSrcR == 1) || (u4PremultDstW == 1))
        {
            _GFX_SetPremult(u4PremultSrcR, 0, u4PremultDstW, 1);
        }
        
	    _GFX_StretchAlphaCom(u4Sx, u4Sy, u4Sw, u4Sh,
                u4Dx, u4Dy, u4Dw, u4Dh, u4Ar, u4Opcode);           

        _CLI_CMD("gfx.flush");

        pucOnePass = (UINT8*)BUF_SCREEN;
        pucTwoPass = (UINT8*)BUF_ADDR3;
        u4Size = (u4Dw*u4Dh);
        
        for(j=0; j<u4Size ; j++) 
        {
            if (pucOnePass[j] != pucTwoPass[j] )
            {
                printf("1Pass : %x, 2Pass : %x, j = %d\n", pucOnePass[j], pucTwoPass[j], j);
                printf("Stretch alpha composition comparing error  (%d) \n");
                return 1;
            }
        }
        printf("Stretch alpha composition comparing ok (%d) \n", u4Loop);          
    }   

    return 0;
}

#if defined(CC_MT5368)||defined(CC_MT5396)

static INT32 _GfxEmuCmdScaler(INT32 i4Argc, const CHAR ** szArgv)
{
    // UINT32 u4SrcCM, u4DstCM, u4ThirdCM;
    char* pcFileInfo;
    char* vfgFileInfo;
    UINT32 i, u4Times, j;
    UINT32 u4VfgFileCount = 0;
    UINT32 u4InitValue = 0;

    //UINT32 u4RepeatFrame =9999;
    UINT32 u4GoldenSize;
    UINT8* golden_dataY = NULL;
    UINT8* DstAddr = NULL;
    //BOOL fgFirstTime = TRUE;
    UINT32 u4FrameSize = (1920 * 1200 * 4);

    UINT32 u4CfgCount = 0;
    UINT32 u4ImagX0, u4ImagY0, u4ImagX1, u4ImagY1, u4ImagX2, u4ImagY2, u4ImagX3, u4ImagY3;
    UINT32 u4WbX0, u4WbY0, u4WbX1, u4WbY1, u4WbX2, u4WbY2, u4WbX3, u4WbY3;
    UINT32 u4TransImg = FALSE, u4TransWb = FALSE;
    BYTE bImgFileName[256] = {0}, bWbFileName[256] = {0}, bCfgFileName[256] = {0};
    BYTE bVfgFileName[256] = {0};
    UINT32 u4ValTmp, u4WbColorWidth;
    //  UINT32 u4Matrix00,u4Matrix01,u4Matrix02,u4Matrix10,u4Matrix11,u4Matrix12;
//   UINT32 u4Matrix20,u4Matrix21,u4Matrix22;
    UINT32 u4ImgAddress, u4ImgCm, u4ImgGlAlpha, u4ImgWidth, u4ImgHeight;
    INT32 u4ImgPitch, u4WbPitch;
    UINT32 u4TextStartX, u4TextStartY, u4TextEndX, u4TextEndY;
    UINT32 u4WbAddress, u4WbCm, u4WbGlAlpha, u4WbWidth, u4WbHeight;
    UINT32 u4BBStartX, u4BBStartY, u4BBWidth, u4BBHeight;
    UINT32 u4FilterEn, u4AntiAliasEn, u4AlphaEdge;
    UINT32 u4AlcomEn, u4AlcomOpMode, u4AlcomNormal, u4AlcomAr;
    UINT32 u4PreMultiSrcRdEn, u4PreMultiDstRdEn, u4PreMultiDstWrEn;
    UINT32 u4ImgeRdExp, u4WbRdExp, u4WbWrRounding;
    UINT32 u4DumpEn;

    UINT32 u4PostWriteThreshold = 0xf, u4WdleEn = TRUE;

    UINT8* ImgAddr = NULL;

#if (GFX_AND_VGFX||VGFX_AND_GFX  || VGFX_AND_VGFX)
    UINT8 *ThirdAddr = NULL;
#endif

#if VGFX_MMU_ENABLE
    UINT32 u4VaTrasAddr = 0x3000000;
    UINT32 u4OverReadAddr;
#endif

    UINT32 u4Matrix[9] = {0};
#if 0//VG_FLOAT_SUPPORT
    float fSrcX[4], fSrcY[4], fDstX[4], fDstY[4], fIMtrix[3][3];
#endif

    //golden_dataY = (UINT8*)BSP_AllocAlignedDmaMemory((1920 * 1088*4), 1024);

#if 0
    DstAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    golden_dataY = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    SrcAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    ThirdAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
#else
    DstAddr = (UINT8*)_u4GfxEmuBuf[0];
    golden_dataY = DstAddr + u4FrameSize;
    ImgAddr = golden_dataY + u4FrameSize;
#if (GFX_AND_VGFX||VGFX_AND_GFX|| VGFX_AND_VGFX)
    ThirdAddr = ImgAddr + u4FrameSize;
#endif

    //SrcPallete = ThirdAddr + 1024;
    //ThirdPallete = SrcPallete + 1024;
#endif

    if (i4Argc != 3)
    {
        Printf(" please input gfxemu.sc start and end \n ");
        return -1;
    }


    if (DstAddr == NULL || golden_dataY == NULL)
    {
        ASSERT(0);
    }

    Printf("2d graphic scaler Stress Test ...\n");


    u4InitValue =  (UINT32) StrToInt(szArgv[1]);
    u4Times =  (UINT32) StrToInt(szArgv[2]);
    //u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);
#if 1
    Printf("Plane mixer init ...\n");
    _CLI_CMD("pmx.i");
    Printf("Plane mixer init ... done\n");

    Printf("OSD init ...\n");
    _CLI_CMD("osd.init");

    // default create 7 lists
    _CLI_CMD("osd.list.create");    // 0

    _CLI_CMD("osd.region.new 640 480 %u 14 2560 0 0 640 480", BUF_SCREEN); // 0 - ARGB8888 720x480
    _CLI_CMD("osd.region.insert 0 0");

    _CLI_CMD("osd.plane.flip 2 0"); // select region list 0 (region 0 - argb8888) as default
    _CLI_CMD("osd.sc.s 0 640 480 640 480");
    _CLI_CMD("osd.plane.enable 2 1");

    _CLI_CMD("osd.region.set 0 big_endian 0");  // region 0: little_endian (0)

    _CLI_CMD("osd.region.set 0 mix_sel 1"); // region 0: pixel mode (1)
    Printf("OSD init ... done\n");
#endif

    if (u4Times > 80)
        Printf(" The Max pattern is 80 \n ");

    pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);
    vfgFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);

    x_memset((void*)vfgFileInfo, 0, 0x40000);
    LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\scaler\\vfg_list.raw\" 0x%08x /ny\n",  vfgFileInfo);
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\scaler\\vfg_list.raw\" 0x%08x /ny", vfgFileInfo));
    u4VfgFileCount = 0;
    for (i = 0;i < u4InitValue;i++)
    {
        _VGfxGetCfgFileName(&u4VfgFileCount, bCfgFileName, vfgFileInfo) ;  //bVfgFileName file name
        //Printf("test  bVfgFileName[%d]= %s\n",i,bVfgFileName);
    }

    for (i = u4InitValue ; i < u4Times ; i++)
    {
        u4CfgCount = 0;
        _VGfxGetCfgFileName(&u4VfgFileCount, bCfgFileName, vfgFileInfo);
        LOG(10, "test  bCfgFileName[%d]= %s\n", i, bCfgFileName);
        _VGfxGetVfgFileName(bCfgFileName, bVfgFileName);
        LOG(10, "test  bVfgFileName[%d]= %s\n", i, bVfgFileName);
        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\%s\" 0x%08x /ny\n",  bVfgFileName, bCfgFileName, pcFileInfo);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bCfgFileName, pcFileInfo));
        /***********************************************
        parse cfg file
        ************************************************/
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);    // input format
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);
        u4ImagX0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);   //x0 y0
        u4ImagY0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //x1 y1
        u4ImagX1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4ImagY1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //x2 y2
        u4ImagX2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4ImagY2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //x3 y3
        u4ImagX3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);

        u4ImagY3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, "test  u4ImagX0 =%d, u4ImagY0 =%d, u4ImagX1 =%d, u4ImagY1 =%d, u4ImagX2 =%d, u4ImagY2 =%d, u4ImagX3 =%d, u4ImagY3 =%d\n ", u4ImagX0, \
            u4ImagY0, u4ImagX1, u4ImagY1, u4ImagX2, u4ImagY2, u4ImagX3, u4ImagY3);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //wb x0 y0
        u4WbX0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);    //wb x1 y1
        u4WbX1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);    //wb x2 y2
        u4WbX2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);     //wb x3 y3
        u4WbX3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " %d  %d  %d %d %d %d %d %d \n", u4WbX0, u4WbY0, u4WbX1, u4WbY1, u4WbX2, u4WbY2, u4WbX3, u4WbY3);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //matrix row 0
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //matrix row 1
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //matrix row 2
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //img file
        if (_VGfxGetFileName(&u4CfgCount, bImgFileName, pcFileInfo) == 1)    //img  file name
        {
            u4TransImg = TRUE;
        }
        else
        {
            u4TransImg = FALSE;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //img base address
        u4ImgAddress = (UINT32)ImgAddr;//_GfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //img color mode
        u4ImgCm = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        switch (u4ImgCm)     //image color mode
        {
            case 11:
                u4ValTmp = 2;
                break;
            case 12:
                u4ValTmp = 2;
                break;
            case 13:
                u4ValTmp = 2;
                break;
            case 14:
                u4ValTmp = 4;
                break;
            default:
                return -1;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //img global alpha
        u4ImgGlAlpha = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //img width
        u4ImgWidth = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4ImgPitch = u4ImgWidth * u4ValTmp;
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //img height
        u4ImgHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4ImgCm = %d  %d  %d %d %d  \n", u4ImgAddress, u4ImgCm, u4ImgGlAlpha, u4ImgWidth, u4ImgHeight);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //texture min x y
        u4TextStartX = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4TextStartY = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //texture max x y
        u4TextEndX = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4TextEndY = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4TextStartX = %d  %d  %d %d  \n", u4TextStartX, u4TextStartY, u4TextEndX, u4TextEndY);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //Wb file
        if (_VGfxGetFileName(&u4CfgCount, bWbFileName, pcFileInfo) == 1)    //img  file name
        {
            u4TransWb = TRUE;
        }
        else
        {
            u4TransWb = FALSE;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //Wb base address
#if VGFX_AND_VGFX
        if (i % 2 == 0)
        {
            u4WbAddress = (UINT32)ThirdAddr;
        }
        else
        {
            u4WbAddress = (UINT32)DstAddr;
        }
#else
        u4WbAddress = (UINT32)DstAddr;//_GfxGetVal(&u4CfgCount, pcFileInfo);
#endif
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb color mode
        u4WbCm = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        switch (u4WbCm)     //image color mode
        {
            case 11:
            case 12:
            case 13:
                u4ValTmp = 2;
                u4WbColorWidth = 2;
                break;
            case 14:
                u4ValTmp = 4;
                u4WbColorWidth = 4;
                break;
            default:
                return -1;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb global alpha
        u4WbGlAlpha = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb width
        u4WbWidth = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbPitch = u4WbWidth * u4ValTmp;
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb height
        u4WbHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4WbAddress = %d  u4WbCm = %d  u4WbGlAlpha = %d u4WbWidth=%d  u4WbHeight =%d \n", u4WbAddress, u4WbCm, u4WbGlAlpha, u4WbWidth, u4WbHeight);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //BB  MIN x y
        u4BBStartX = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4BBStartY = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // BB width
        u4BBWidth = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // BB height
        u4BBHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4BBStartX =%d  u4BBStartY=%d  u4BBWidth=%d  u4BBHeight =%d \n", u4BBStartX, u4BBStartY, u4BBWidth, u4BBHeight);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // filter enable
        u4FilterEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // anti aliasing enable
        u4AntiAliasEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //alpha edge mode
        u4AlphaEdge = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4FilterEn = %d  u4AntiAliasEn=%d \n", u4FilterEn, u4AntiAliasEn);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //  after alpha edge mode
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // alpha composition enable
        u4AlcomEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, "test  u4AlcomEn = %d, u4AlphaEdge= %d\n", u4AlcomEn, u4AlphaEdge);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // alpha com op mode
        u4AlcomOpMode = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // alpha com normal
        u4AlcomNormal = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // Ar
        u4AlcomAr = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // pre mult src read enable
        u4PreMultiSrcRdEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // pre mult dst read enable
        u4PreMultiDstRdEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // pre mult dst write enable
        u4PreMultiDstWrEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // img read exp
        u4ImgeRdExp = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb read exp
        u4WbRdExp = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb write rounding
        u4WbWrRounding = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //dump enable
        u4DumpEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
//#ifdef VGFX_DEBUG
        LOG(10, "test  u4AlcomEn =%d  %d  %d  %d  %d %d %d %d %d %d %d %d %d %d %d \n", u4AlcomEn, \
            u4AlcomOpMode, u4AlcomNormal, u4AlcomAr, u4PreMultiSrcRdEn, u4PreMultiDstRdEn, \
            u4PreMultiDstWrEn, u4ImgeRdExp, u4WbRdExp, u4WbWrRounding, u4DumpEn,  u4TransImg, u4TransWb, u4ImgPitch, u4WbPitch);
// #endif
        /***********************************************
        load raw data
        ************************************************/

        x_memset((void*)ImgAddr, 0, u4FrameSize);  // load img file to u4ImgAddress
        x_memset((void*)DstAddr, 0, u4FrameSize); // load wb file to u4WbAddress
        x_memset((void*)golden_dataY, 0, u4FrameSize);
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bImgFileName,  u4ImgAddress);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bImgFileName, ImgAddr));
#if VGFX_AND_VGFX
        if (i % 2 == 0)
        {
            x_memset((void*)ThirdAddr, 0, u4FrameSize);
            LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bWbFileName,  (UINT32)ThirdAddr);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bWbFileName, ThirdAddr));
        }
        else
        {
            LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bWbFileName,  u4WbAddress);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bWbFileName, DstAddr));
        }
#else
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bWbFileName,  u4WbAddress);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bWbFileName, DstAddr));
#endif

        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\white_board_o.raw\" 0x%08x /ny\n", bVfgFileName, 	golden_dataY);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\white_board_o.raw\" 0x%08x /ny", bVfgFileName, golden_dataY));

#if GFX_AND_VGFX
        x_memset((void*)ImgAddr, 0, u4FrameSize);
        x_memset((void*)ThirdAddr, 0, u4FrameSize);

        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bImgFileName,  (UINT32)ThirdAddr);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bImgFileName, ThirdAddr));
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");
        _GFX_SetDst(ImgAddr, u4WbCm, 1920*4);
        _GFX_SetSrc(ThirdAddr, u4WbCm, 1920*4);
        GFX_SetBltOpt(0, 0, 0);
        GFX_BitBlt(0, 0, 0, 0, 1920, 1080);
        // _CLI_CMD("gfx.flush");
#endif

#if 0//VG_FLOAT_SUPPORT
        fSrcX[0] = (float)(u4TextStartX);
        fSrcX[1] = (float)(u4TextEndX);
        fSrcX[2] = (float)(u4TextEndX);
        fSrcX[3] = (float)(u4TextStartX);
        fSrcY[0] = (float)(u4TextStartY);
        fSrcY[1] = (float)(u4TextStartY);
        fSrcY[2] = (float)(u4TextEndY);
        fSrcY[3] = (float)(u4TextEndY);
        fDstX[0] = (float)(u4WbX0);
        fDstX[1] = (float)(u4WbX1);
        fDstX[2] = (float)(u4WbX2);
        fDstX[3] = (float)(u4WbX3);
        fDstY[0] = (float)(u4WbY0);
        fDstY[1] = (float)(u4WbY1);
        fDstY[2] = (float)(u4WbY2);
        fDstY[3] = (float)(u4WbY3);

        derive_inverse_transform(fSrcX, fSrcY, fDstX, fDstY, fIMtrix);

        u4ITMatrixA = fnum_converter(fIMtrix[0][0]) >> 9;
        u4ITMatrixB = fnum_converter(fIMtrix[0][1]) >> 9;
        u4ITMatrixC = fnum_converter(fIMtrix[0][2]) >> 9;
        u4ITMatrixD = fnum_converter(fIMtrix[1][0]) >> 9;
        u4ITMatrixE = fnum_converter(fIMtrix[1][1]) >> 9;
        u4ITMatrixF = fnum_converter(fIMtrix[1][2]) >> 9;
        u4ITMatrixG = fnum_converter(fIMtrix[2][0]) >> 9;
        u4ITMatrixH = fnum_converter(fIMtrix[2][1]) >> 9;
        u4ITMatrixI = fnum_converter(fIMtrix[2][2]) >> 9;

#endif
        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\xform_i_coeff.txt\" 0x%08x /ny\n", bVfgFileName, pcFileInfo);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\xform_i_coeff.txt\" 0x%08x /ny", bVfgFileName, pcFileInfo));
        u4CfgCount = 0;
        for (j = 0;j < 9; j++)
        {
            _VGfxGetCoeff(&u4Matrix[j], &u4CfgCount, pcFileInfo);
            //Printf("debug ,u4Matrix[%d]= 0x%08x \n",i ,u4Matrix[i]);
        }
#if GFX_AND_VGFX

#else
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");
#endif

#if VGFX_MMU_ENABLE
#if VGFX_DUALCHANNEL   //only 5396 FPGA  use
		{
			u4ImgAddress = u4ImgAddress+0x40000000;
		}
#endif
        _GFX_MMU_Init();
        for (j = 0;j < 9;j++)  // UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
        {
            HalSet1MSectionTable(u4ImgAddress + j*0x100000, u4ImgAddress + u4VaTrasAddr + j*0x100000); //+48M
            HalSet1MSectionTable(u4WbAddress + j*0x100000, u4WbAddress + u4VaTrasAddr + j*0x100000);
        }
        u4OverReadAddr = u4ImgAddress + u4VaTrasAddr + u4WbWidth * u4WbHeight * u4WbColorWidth;
        _GFX_MMU_Set_OverRead(TRUE, TRUE, u4OverReadAddr >> 12);
        _GFX_MMU_Set_TabAddr(HalGetMMUTableAddress());
        _GFX_MMU_Enable(TRUE);
        //_GFX_MMU_SelfFire();
#endif

#if VGFX_WRITE_ARGBORDER
        _VGFX_SetWriteArgbOrder( B_ORDER, A_ORDER, R_ORDER, G_ORDER);
#endif

        _VGFX_SetPostWrEnv(u4PostWriteThreshold, u4WdleEn, u4WbWrRounding);
        _VGFX_SetQualityEnv(u4WbRdExp, u4ImgeRdExp, u4AntiAliasEn, u4FilterEn, u4AlphaEdge);
        //set img info
#if VGFX_MMU_ENABLE
        _VGFX_SetImgage(u4ImgAddress + u4VaTrasAddr, u4ImgCm, u4ImgGlAlpha, u4ImgPitch);
#else
        _VGFX_SetImgage(u4ImgAddress, u4ImgCm, u4ImgGlAlpha, u4ImgPitch);
#endif
        _VGFX_SetTexture(u4TextStartX, u4TextStartY, u4TextEndX, u4TextEndY);
        // set wb info
        {
            UINT32 x1, y1, w, h;
            if (u4BBStartX >= (u4WbPitch / u4WbColorWidth))
            {
                Printf("the BB startx is not correctly\n");
                return -1;
            }
            else
            {
                x1 = u4BBStartX;
                if ((u4BBStartX + u4BBWidth) >= (u4WbPitch / u4WbColorWidth))
                {
                    w = u4WbPitch / u4WbColorWidth - x1;
                }
                else
                {
                    w = u4BBWidth;
                }
            }
            if (u4BBStartY >= u4WbHeight)
            {
                Printf("the BB starty is not correctly u4BBStartY = %d, u4WbHeight = %d\n", u4BBStartY, u4WbHeight);
                return -1;
            }
            else
            {
                y1 = u4BBStartY;
                if ((u4BBStartY + u4BBHeight) >= u4WbHeight)
                {
                    h = u4WbHeight - y1;
                }
                else
                {
                    h = u4BBHeight;
                }
            }
            _VGFX_SetBb(x1, y1, w, h);
        }
#if VGFX_MMU_ENABLE
        _VGFX_SetWb(u4WbAddress + u4VaTrasAddr, u4WbCm, u4WbGlAlpha, u4WbPitch);
#else
        _VGFX_SetWb(u4WbAddress, u4WbCm, u4WbGlAlpha, u4WbPitch);
#endif
        _VGFX_SetMatrixCoef_Emu(u4Matrix[0], u4Matrix[1], u4Matrix[2], u4Matrix[3], u4Matrix[4], u4Matrix[5], u4Matrix[6], u4Matrix[7], u4Matrix[8]);
        _VGFX_SetAlcom(u4AlcomEn);
        if (u4AlcomEn == TRUE)
        {
            //_GFX_SetPremult(u4PreMultiSrcRdEn, u4PreMultiDstRdEn, u4PreMultiDstWrEn, 1);
            _VGFX_GeneralAlphaComSet(u4ImgCm, u4WbCm, u4AlcomAr, u4AlcomOpMode, u4AlcomNormal, u4PreMultiSrcRdEn, u4PreMultiDstRdEn, u4PreMultiDstWrEn, 1);
        }
        _VGFX_Enable();

#if VGFX_AND_GFX
        x_memset((void*)ThirdAddr, 0, u4FrameSize);

        _GFX_SetDst(ThirdAddr, u4WbCm, 1920*4);
        _GFX_SetSrc(DstAddr, u4WbCm, 1920*4);
        GFX_SetBltOpt(0, 0, 0);
        GFX_BitBlt(0, 0, 0, 0, 1920, 1080);
        DstAddr = ThirdAddr;
        // _CLI_CMD("gfx.flush");
#endif
#if VGFX_AND_VGFX
        if (i % 2 == 0)
        {
            GFX_Flush();
            GFX_Wait();
        }
#else
        GFX_Flush();
        GFX_Wait();
#endif



        u4GoldenSize = u4WbWidth * u4WbHeight * u4WbColorWidth;
#if VGFX_WRITE_ARGBORDER
        for (j = 0; j < u4GoldenSize ; j += 4)  //only BB is RGBA
        {
            if (((DstAddr[j] != golden_dataY[j]) && (DstAddr[j+1] != golden_dataY[j+1])
                    && (DstAddr[j+2] != golden_dataY[j+2]) && (DstAddr[j+3] != golden_dataY[j+3]))
                    && ((DstAddr[j] != golden_dataY[j+3]) && (DstAddr[j+1] != golden_dataY[j])
                        && (DstAddr[j+2] != golden_dataY[j+1]) && (DstAddr[j+3] != golden_dataY[j+2])))
            {
                Printf("hw : %x, golden : %x, j = %d\n", DstAddr[j], golden_dataY[j], j);
                Printf("compare error in %d times file_name=%s\n", i, bVfgFileName);
                return 1;
                //break;
            }
            else
            {
                //Printf("compare OK in %d times file_name=%s\n", i, bVfgFileName);
            }
        }
        _VGFX_SetWriteArgbOrder( A_ORDER, R_ORDER, G_ORDER, B_ORDER); //reset

        Printf("gfx scaler write RGBA order comparing OK......\n");
        return 0;
#endif
#if VGFX_AND_VGFX
        if (i % 2 == 0)
        {
            DstAddr = ThirdAddr;
            for (j = 0; j < u4GoldenSize ; j++)
            {
                if (DstAddr[j] != golden_dataY[j])
                {
                    Printf("hw : %x, golden : %x, j = %d\n", DstAddr[j], golden_dataY[j], j);
                    Printf("compare error in %d times file_name=%s\n", i, bVfgFileName);
                    return 1;
                    //break;
                }
                else
                {
                    //Printf("compare OK in %d times file_name=%s\n", i, bVfgFileName);
                }
            }
        }
#else
        for (j = 0; j < u4GoldenSize ; j++)
        {
            if (DstAddr[j] != golden_dataY[j])
            {
                Printf("hw : %x, golden : %x, j = %d\n", DstAddr[j], golden_dataY[j], j);
                Printf("compare error in %d times file_name=%s\n", i, bVfgFileName);
                return 1;
                //break;
            }
            else
            {
                //Printf("compare OK in %d times file_name=%s\n", i, bVfgFileName);
            }
        }
#endif
        Printf("compare OK in %d times file_name=%s\n", i, bVfgFileName);

    }
    Printf("gfx scaler 80 pattern comparing OK......\n");
#if VGFX_MMU_ENABLE
    Printf("gfx scaler enable MMU  comparing OK......\n");
#endif

    return 0;

}

// test random pattern
static INT32 _GfxEmuCmdScalerRandom(INT32 i4Argc, const CHAR ** szArgv)
{
    // UINT32 u4SrcCM, u4DstCM, u4ThirdCM;
    char* pcFileInfo;
    UINT32 i, u4Times, j;
    UINT32 u4InitValue = 1;

    //UINT32 u4RepeatFrame =9999;
    UINT32 u4GoldenSize;
    UINT8* golden_dataY = NULL;
    UINT8* DstAddr = NULL;
    //BOOL fgFirstTime = TRUE;
    UINT32 u4FrameSize = (1920 * 1200 * 4);

    UINT32 u4CfgCount = 0;
    UINT32 u4ImagX0, u4ImagY0, u4ImagX1, u4ImagY1, u4ImagX2, u4ImagY2, u4ImagX3, u4ImagY3;
    UINT32 u4WbX0, u4WbY0, u4WbX1, u4WbY1, u4WbX2, u4WbY2, u4WbX3, u4WbY3;
    UINT32 u4TransImg = FALSE, u4TransWb = FALSE;
    BYTE bImgFileName[256] = {0}, bWbFileName[256] = {0};
    BYTE bVfgFileName[256] = {0};
    UINT32 u4ValTmp, u4WbColorWidth;
    //  UINT32 u4Matrix00,u4Matrix01,u4Matrix02,u4Matrix10,u4Matrix11,u4Matrix12;
//   UINT32 u4Matrix20,u4Matrix21,u4Matrix22;
    UINT32 u4ImgAddress, u4ImgCm, u4ImgGlAlpha, u4ImgWidth, u4ImgHeight;
    INT32 u4ImgPitch, u4WbPitch;
    UINT32 u4TextStartX, u4TextStartY, u4TextEndX, u4TextEndY;
    UINT32 u4WbAddress, u4WbCm, u4WbGlAlpha, u4WbWidth, u4WbHeight;
    UINT32 u4BBStartX, u4BBStartY, u4BBWidth, u4BBHeight;
    UINT32 u4FilterEn, u4AntiAliasEn, u4AlphaEdge;
    UINT32 u4AlcomEn, u4AlcomOpMode, u4AlcomNormal, u4AlcomAr;
    UINT32 u4PreMultiSrcRdEn, u4PreMultiDstRdEn, u4PreMultiDstWrEn;
    UINT32 u4ImgeRdExp, u4WbRdExp, u4WbWrRounding;
    UINT32 u4DumpEn;

    UINT32 u4PostWriteThreshold = 0xf, u4WdleEn = TRUE;

    UINT8* ImgAddr = NULL;

#if (GFX_AND_VGFX||VGFX_AND_GFX  || VGFX_AND_VGFX)
    UINT8 *ThirdAddr = NULL;
#endif
    UINT32 u4Matrix[9] = {0};
#if 0//VG_FLOAT_SUPPORT
    float fSrcX[4], fSrcY[4], fDstX[4], fDstY[4], fIMtrix[3][3];
#endif

    //golden_dataY = (UINT8*)BSP_AllocAlignedDmaMemory((1920 * 1088*4), 1024);

#if 0
    DstAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    golden_dataY = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    SrcAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    ThirdAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
#else
    DstAddr = (UINT8*)_u4GfxEmuBuf[0];
    golden_dataY = DstAddr + u4FrameSize;
    ImgAddr = golden_dataY + u4FrameSize;
#if (GFX_AND_VGFX||VGFX_AND_GFX|| VGFX_AND_VGFX)
    ThirdAddr = ImgAddr + u4FrameSize;
#endif

    //SrcPallete = ThirdAddr + 1024;
    //ThirdPallete = SrcPallete + 1024;
#endif

    if ((szArgv == NULL) || (szArgv[1] == NULL) )
    {
        Printf(" please input auto test times \n ");
        return -1;
    }


    if (DstAddr == NULL || golden_dataY == NULL)
    {
        ASSERT(0);
    }

    Printf("2d graphic scaler auto  Test ...\n");

    //u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);
    u4InitValue =  (UINT32) StrToInt(szArgv[1]);
    u4Times =	(UINT32) StrToInt(szArgv[2]);


    pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);


    for (i = u4InitValue ; i < u4Times ; i++)
    {
        u4CfgCount = 0;
        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"f:\\scaler\\random_vgfx_config\\random_%d.cfg\" 0x%08x /ny\n",  i, pcFileInfo);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\scaler\\random_vgfx_config\\random_%d.cfg\" 0x%08x /ny", i, pcFileInfo));
        /***********************************************
        parse cfg file
        ************************************************/
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);    // input format
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);
        u4ImagX0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);   //x0 y0
        u4ImagY0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //x1 y1
        u4ImagX1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4ImagY1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //x2 y2
        u4ImagX2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4ImagY2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //x3 y3
        u4ImagX3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);

        u4ImagY3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, "test  u4ImagX0 =%d, u4ImagY0 =%d, u4ImagX1 =%d, u4ImagY1 =%d, u4ImagX2 =%d, u4ImagY2 =%d, u4ImagX3 =%d, u4ImagY3 =%d\n ", u4ImagX0, \
            u4ImagY0, u4ImagX1, u4ImagY1, u4ImagX2, u4ImagY2, u4ImagX3, u4ImagY3);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //wb x0 y0
        u4WbX0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);    //wb x1 y1
        u4WbX1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);    //wb x2 y2
        u4WbX2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);     //wb x3 y3
        u4WbX3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " %d  %d  %d %d %d %d %d %d \n", u4WbX0, u4WbY0, u4WbX1, u4WbY1, u4WbX2, u4WbY2, u4WbX3, u4WbY3);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //matrix row 0
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //matrix row 1
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //matrix row 2
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //img file
        if (_VGfxGetFileName(&u4CfgCount, bImgFileName, pcFileInfo) == 1)    //img  file name
        {
            u4TransImg = TRUE;
        }
        else
        {
            u4TransImg = FALSE;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //img base address
        u4ImgAddress = (UINT32)ImgAddr;//_GfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //img color mode
        u4ImgCm = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        switch (u4ImgCm)     //image color mode
        {
            case 11:
                u4ValTmp = 2;
                break;
            case 12:
                u4ValTmp = 2;
                break;
            case 13:
                u4ValTmp = 2;
                break;
            case 14:
                u4ValTmp = 4;
                break;
            default:
                return -1;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //img global alpha
        u4ImgGlAlpha = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //img width
        u4ImgWidth = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4ImgPitch = u4ImgWidth * u4ValTmp;
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //img height
        u4ImgHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4ImgCm = %d  %d  %d %d %d  \n", u4ImgAddress, u4ImgCm, u4ImgGlAlpha, u4ImgWidth, u4ImgHeight);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //texture min x y
        u4TextStartX = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4TextStartY = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //texture max x y
        u4TextEndX = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4TextEndY = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4TextStartX = %d  %d  %d %d  \n", u4TextStartX, u4TextStartY, u4TextEndX, u4TextEndY);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //Wb file
        if (_VGfxGetFileName(&u4CfgCount, bWbFileName, pcFileInfo) == 1)    //img  file name
        {
            u4TransWb = TRUE;
        }
        else
        {
            u4TransWb = FALSE;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //Wb base address
#if VGFX_AND_VGFX
        if (i % 2 == 0)
        {
            u4WbAddress = (UINT32)ThirdAddr;
        }
        else
        {
            u4WbAddress = (UINT32)DstAddr;
        }
#else
        u4WbAddress = (UINT32)DstAddr;//_GfxGetVal(&u4CfgCount, pcFileInfo);
#endif
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb color mode
        u4WbCm = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        switch (u4WbCm)     //image color mode
        {
            case 11:
            case 12:
            case 13:
                u4ValTmp = 2;
                u4WbColorWidth = 2;
                break;
            case 14:
                u4ValTmp = 4;
                u4WbColorWidth = 4;
                break;
            default:
                return -1;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb global alpha
        u4WbGlAlpha = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb width
        u4WbWidth = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbPitch = u4WbWidth * u4ValTmp;
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb height
        u4WbHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4WbAddress = %d  u4WbCm = %d  u4WbGlAlpha = %d u4WbWidth=%d  u4WbHeight =%d \n", u4WbAddress, u4WbCm, u4WbGlAlpha, u4WbWidth, u4WbHeight);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //BB  MIN x y
        u4BBStartX = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4BBStartY = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // BB width
        u4BBWidth = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // BB height
        u4BBHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4BBStartX =%d  u4BBStartY=%d  u4BBWidth=%d  u4BBHeight =%d \n", u4BBStartX, u4BBStartY, u4BBWidth, u4BBHeight);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // filter enable
        u4FilterEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // anti aliasing enable
        u4AntiAliasEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //alpha edge mode
        u4AlphaEdge = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4FilterEn = %d  u4AntiAliasEn=%d \n", u4FilterEn, u4AntiAliasEn);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //  after alpha edge mode
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // alpha composition enable
        u4AlcomEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, "test  u4AlcomEn = %d, u4AlphaEdge= %d\n", u4AlcomEn, u4AlphaEdge);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // alpha com op mode
        u4AlcomOpMode = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // alpha com normal
        u4AlcomNormal = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // Ar
        u4AlcomAr = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // pre mult src read enable
        u4PreMultiSrcRdEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // pre mult dst read enable
        u4PreMultiDstRdEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // pre mult dst write enable
        u4PreMultiDstWrEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // img read exp
        u4ImgeRdExp = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb read exp
        u4WbRdExp = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb write rounding
        u4WbWrRounding = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //dump enable
        u4DumpEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
//#ifdef VGFX_DEBUG
        LOG(10, "test  u4AlcomEn =%d  %d  %d  %d  %d %d %d %d %d %d %d %d %d %d %d \n", u4AlcomEn, \
            u4AlcomOpMode, u4AlcomNormal, u4AlcomAr, u4PreMultiSrcRdEn, u4PreMultiDstRdEn, \
            u4PreMultiDstWrEn, u4ImgeRdExp, u4WbRdExp, u4WbWrRounding, u4DumpEn,  u4TransImg, u4TransWb, u4ImgPitch, u4WbPitch);
// #endif
        /***********************************************
        load raw data
        ************************************************/

        x_memset((void*)ImgAddr, 0, u4FrameSize);  // load img file to u4ImgAddress
        x_memset((void*)DstAddr, 0, u4FrameSize); // load wb file to u4WbAddress
        x_memset((void*)golden_dataY, 0, u4FrameSize);
        LOG(10, "//d.load.binary \"f:\\scaler\\random_vgfx_config\\random_image_%d.raw\" 0x%08x /ny\n", i,  u4ImgAddress);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\scaler\\random_vgfx_config\\random_image_%d.raw\" 0x%08x /ny", i, ImgAddr));
        LOG(10, "//d.load.binary \"f:\\scaler\\random_vgfx_config\\random_white_board_%d.raw\" 0x%08x /ny\n", i,  u4WbAddress);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\scaler\\random_vgfx_config\\random_white_board_%d.raw\" 0x%08x /ny", i, DstAddr));

        LOG(10, "//d.load.binary \"f:\\scaler\\random_vgfx_config\\white_board_o_%d.raw\" 0x%08x /ny\n", i, 	golden_dataY);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\scaler\\random_vgfx_config\\white_board_o_%d.raw\" 0x%08x /ny", i, golden_dataY));


        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"f:\\scaler\\random_vgfx_config\\xform_i_coeff_%d.txt\" 0x%08x /ny\n", i, pcFileInfo);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\scaler\\random_vgfx_config\\xform_i_coeff_%d.txt\" 0x%08x /ny", i, pcFileInfo));
        u4CfgCount = 0;
        for (j = 0;j < 9; j++)
        {
            _VGfxGetCoeff(&u4Matrix[j], &u4CfgCount, pcFileInfo);
            //Printf("debug ,u4Matrix[%d]= 0x%08x \n",i ,u4Matrix[i]);
        }

#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");
        _VGFX_SetPostWrEnv(u4PostWriteThreshold, u4WdleEn, u4WbWrRounding);
        _VGFX_SetQualityEnv(u4WbRdExp, u4ImgeRdExp, u4AntiAliasEn, u4FilterEn, u4AlphaEdge);
        //set img info
        _VGFX_SetImgage(u4ImgAddress, u4ImgCm, u4ImgGlAlpha, u4ImgPitch);

        _VGFX_SetTexture(u4TextStartX, u4TextStartY, u4TextEndX, u4TextEndY);
        // set wb info
        {
            UINT32 x1, y1, w, h;
            if (u4BBStartX >= (u4WbPitch / u4WbColorWidth))
            {
                Printf("the BB startx is not correctly\n");
                return -1;
            }
            else
            {
                x1 = u4BBStartX;
                if ((u4BBStartX + u4BBWidth) >= (u4WbPitch / u4WbColorWidth))
                {
                    w = u4WbPitch / u4WbColorWidth - x1;
                }
                else
                {
                    w = u4BBWidth;
                }
            }
            if (u4BBStartY >= u4WbHeight)
            {
                Printf("the BB starty is not correctly u4BBStartY = %d, u4WbHeight = %d\n", u4BBStartY, u4WbHeight);
                return -1;
            }
            else
            {
                y1 = u4BBStartY;
                if ((u4BBStartY + u4BBHeight) >= u4WbHeight)
                {
                    h = u4WbHeight - y1;
                }
                else
                {
                    h = u4BBHeight;
                }
            }
            _VGFX_SetBb(x1, y1, w, h);
        }
        _VGFX_SetWb(u4WbAddress, u4WbCm, u4WbGlAlpha, u4WbPitch);
        _VGFX_SetMatrixCoef_Emu(u4Matrix[0], u4Matrix[1], u4Matrix[2], u4Matrix[3], u4Matrix[4], u4Matrix[5], u4Matrix[6], u4Matrix[7], u4Matrix[8]);
        _VGFX_SetAlcom(u4AlcomEn);
        if (u4AlcomEn == TRUE)
        {
            //_GFX_SetPremult(u4PreMultiSrcRdEn, u4PreMultiDstRdEn, u4PreMultiDstWrEn, 1);
            _VGFX_GeneralAlphaComSet(u4ImgCm, u4WbCm, u4AlcomAr, u4AlcomOpMode, u4AlcomNormal, u4PreMultiSrcRdEn, u4PreMultiDstRdEn, u4PreMultiDstWrEn, 1);
        }
        _VGFX_Enable();
        GFX_Flush();
        GFX_Wait();

        u4GoldenSize = u4WbWidth * u4WbHeight * u4WbColorWidth;

        for (j = 0; j < u4GoldenSize ; j++)
        {
            if (DstAddr[j] != golden_dataY[j])
            {
                Printf("hw : %x, golden : %x, j = %d\n", DstAddr[j], golden_dataY[j], j);
                Printf("compare error in %d times file_name=%s\n", i, bVfgFileName);
                return 1;
                //break;
            }
            else
            {
                //Printf("compare OK in %d times file_name=%s\n", i, bVfgFileName);
            }
        }
        Printf("compare OK in %d times file_name=%s\n", i, bVfgFileName);

    }
    Printf("gfx scaler random pattern auto test OK......");
    return 0;

}

static INT32 _GfxEmuCmdScalerAlphaEdge(INT32 i4Argc, const CHAR ** szArgv)
{
    // UINT32 u4SrcCM, u4DstCM, u4ThirdCM;
    VG_PARAM_SET_T prVgParam;

    Printf("Plane mixer init ...\n");
    _CLI_CMD("pmx.i");
    Printf("Plane mixer init ... done\n");

    Printf("OSD init ...\n");
    _CLI_CMD("osd.init");

    // default create 7 lists
    _CLI_CMD("osd.list.create");    // 0
    _CLI_CMD("osd.list.create");

    _CLI_CMD("osd.region.new 720 480 %u 14 2880 0 0 720 480", COMPARE_BUF); // 0 - ARGB8888 720x480
    _CLI_CMD("osd.region.insert 0 0");
    _CLI_CMD("osd.plane.flip 2 0"); // select region list 0 (region 0 - argb8888) as default
    _CLI_CMD("osd.sc.s 1 720 480 720 480");
    //_CLI_CMD("osd.plane.enable 2 1");
    _CLI_CMD("osd.region.set 0 big_endian 0");  // region 0: little_endian (0)
    _CLI_CMD("osd.region.set 0 mix_sel 1"); // region 0: pixel mode (1)


    _CLI_CMD("osd.list.create");    // 0
    _CLI_CMD("osd.region.new 720 480 %u 14 2880 0 0 720 480", BUF_SCREEN); // 0 - ARGB8888 720x480
    _CLI_CMD("osd.region.insert 1 1");
    _CLI_CMD("osd.plane.flip 3 1"); // select region list 0 (region 0 - argb8888) as default
    _CLI_CMD("osd.sc.s 1 720 480 720 480");
    _CLI_CMD("osd.plane.enable 3 1");
    _CLI_CMD("osd.region.set 1 big_endian 0");  // region 1: little_endian (0)
    _CLI_CMD("osd.region.set 1 mix_sel 1"); // region 1: pixel mode (1)

    Printf("OSD init ... done\n");



    _CLI_CMD("gfx.reset");
    _CLI_CMD("gfxemu.mode");


    GFX_SetDst((UINT8*)BUF_SCREEN, (UINT32)CM_ARGB8888_DIRECT32, 720*4);
    _CLI_CMD("gfxemu.cls");
    GFX_SetDst((UINT8*)COMPARE_BUF, (UINT32)CM_ARGB8888_DIRECT32, 64*4);
    _CLI_CMD("gfxemu.cls");

    _CLI_CMD("gfxemu.color 0xFFFFFFFFF");    //
    _CLI_CMD("gfx.op.rect   10 0 10 64");
    _CLI_CMD("gfxemu.color 0x0000FF00");     //
    _CLI_CMD("gfx.op.rect   20 0 5 64");
    _CLI_CMD("gfxemu.color 0xFFFF00FF");     //
    _CLI_CMD("gfx.op.rect   25 0 5 64");
    _CLI_CMD("gfxemu.color 0x00FFFFFF");     //
    _CLI_CMD("gfx.op.rect   30 0 5 64");
    _CLI_CMD("gfxemu.color 0xFFFF0000");     //
    _CLI_CMD("gfx.op.rect   35 0 10 64");
    _CLI_CMD("gfxemu.color 0x00000000");     //
    _CLI_CMD("gfx.op.rect   45 0 5 64");
    _CLI_CMD("gfxemu.color 0xFF0000FF");     //
    _CLI_CMD("gfx.op.rect   50 0 5 64");
    _CLI_CMD("gfxemu.color 0x0000FFFF");     //
    _CLI_CMD("gfx.op.rect   55 0 5 64");
    _CLI_CMD("gfxemu.color 0xFF00FFFF");     //
    _CLI_CMD("gfx.op.rect   60 0 4 64");
    _CLI_CMD("gfx.flush");


    prVgParam.u4ImgAddr = COMPARE_BUF;
    prVgParam.u4ImgPitch = 256;
    prVgParam.u4ImgClrMode = 14;
    prVgParam.u4ImgGlobalAlpha = 0;

    prVgParam.u4TextureHStart = 0;
    prVgParam.u4TextureVStart = 0;
    prVgParam.u4TextureWidth = 63;
    prVgParam.u4TextureHeight = 63;

    prVgParam.u4WBAddr = BUF_SCREEN;
    prVgParam.u4WBPitch = 2880;
    prVgParam.u4WBHeight = 480;
    prVgParam.u4WBClrMode = 14;
    prVgParam.u4WBGlobalAlpha = 0;

    prVgParam.u4BBXStart = 0;
    prVgParam.u4BBYStart = 0;
    prVgParam.u4BBWidth = 720;
    prVgParam.u4BBHeight = 480;

    prVgParam.u4AlphaCompAr = 0;
    prVgParam.fgAlphaCompEn = 0;
    prVgParam.u4AlphaCompOpCode  = 0;
    prVgParam.fgAlphaCompNormal = 0;

    prVgParam.u4AlphaEdgeMode = VG_ALPHA_EDGE_NORMAL;
    prVgParam.fgAntiAliasing = 0;             //anti-alising          0x300_OK//en
    prVgParam.fgFilterEnable = 1;
    Printf("alpha edge mode is VG_ALPHA_EDGE_NORMAL \n");
   _VGFX_Scaler_Emu(&prVgParam);
   GFX_Flush();
   GFX_Wait();

   x_thread_delay(10000);
   Printf("alpha edge mode is VG_ALPHA_EDGE_NEAREST \n ");
   prVgParam.u4AlphaEdgeMode = VG_ALPHA_EDGE_NEAREST;
   _VGFX_Scaler_Emu(&prVgParam);
   GFX_Flush();
   GFX_Wait();
    return 0;

}


static INT32 _GfxEmuCmdScalerSeamLessClip(INT32 i4Argc, const CHAR ** szArgv)
{
    // UINT32 u4SrcCM, u4DstCM, u4ThirdCM;
    char* pcFileInfo;
    char* vfgFileInfo;
    UINT32 i, u4Times, j;
    UINT32 u4VfgFileCount = 0;
    UINT32 u4InitValue = 0;

    //UINT32 u4RepeatFrame =9999;
    UINT32 u4GoldenSize;
    UINT8* golden_dataY = NULL;
    UINT8* DstAddr = NULL;
    //BOOL fgFirstTime = TRUE;
    UINT32 u4FrameSize = (1920 * 1200 * 4);

    UINT32 u4CfgCount = 0;
    UINT32 u4ImagX0, u4ImagY0, u4ImagX1, u4ImagY1, u4ImagX2, u4ImagY2, u4ImagX3, u4ImagY3;
    UINT32 u4WbX0, u4WbY0, u4WbX1, u4WbY1, u4WbX2, u4WbY2, u4WbX3, u4WbY3;
    UINT32 u4TransImg = FALSE, u4TransWb = FALSE;
    BYTE bImgFileName[256] = {0}, bWbFileName[256] = {0}, bCfgFileName[256] = {0};
    BYTE bVfgFileName[256] = {0};
    UINT32 u4ValTmp, u4WbColorWidth;
    //	UINT32 u4Matrix00,u4Matrix01,u4Matrix02,u4Matrix10,u4Matrix11,u4Matrix12;
    //	 UINT32 u4Matrix20,u4Matrix21,u4Matrix22;
    UINT32 u4ImgAddress, u4ImgCm, u4ImgGlAlpha, u4ImgWidth, u4ImgHeight;
    INT32 u4ImgPitch, u4WbPitch;
    UINT32 u4TextStartX, u4TextStartY, u4TextEndX, u4TextEndY;
    UINT32 u4WbAddress, u4WbCm, u4WbGlAlpha, u4WbWidth, u4WbHeight;
    UINT32 u4BBStartX, u4BBStartY, u4BBWidth, u4BBHeight;
    UINT32 u4FilterEn, u4AntiAliasEn, u4AlphaEdge;
    UINT32 u4AlcomEn, u4AlcomOpMode, u4AlcomNormal, u4AlcomAr;
    UINT32 u4PreMultiSrcRdEn, u4PreMultiDstRdEn, u4PreMultiDstWrEn;
    UINT32 u4ImgeRdExp, u4WbRdExp, u4WbWrRounding;
    UINT32 u4DumpEn;

    UINT32 u4PostWriteThreshold = 0xf, u4WdleEn = TRUE;

    UINT8* ImgAddr = NULL;



    UINT32 u4Matrix[9] = {0};
#if 0//VG_FLOAT_SUPPORT
    float fSrcX[4], fSrcY[4], fDstX[4], fDstY[4], fIMtrix[3][3];
#endif

    //golden_dataY = (UINT8*)BSP_AllocAlignedDmaMemory((1920 * 1088*4), 1024);

#if 0
    DstAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    golden_dataY = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    SrcAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    ThirdAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
#else
    DstAddr = (UINT8*)_u4GfxEmuBuf[0];
    golden_dataY = DstAddr + u4FrameSize;
    ImgAddr = golden_dataY + u4FrameSize;

    //SrcPallete = ThirdAddr + 1024;
    //ThirdPallete = SrcPallete + 1024;
#endif

    if (i4Argc != 3)
    {
        Printf(" please input gfxemu.sc start and end \n ");
        return -1;
    }


    if (DstAddr == NULL || golden_dataY == NULL)
    {
        ASSERT(0);
    }

    Printf("2d graphic scaler Stress Test ...\n");


    u4InitValue =  (UINT32) StrToInt(szArgv[1]);
    u4Times =  (UINT32) StrToInt(szArgv[2]);
    //u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);
#if 1
    Printf("Plane mixer init ...\n");
    _CLI_CMD("pmx.i");
    Printf("Plane mixer init ... done\n");

    Printf("OSD init ...\n");
    _CLI_CMD("osd.init");

    // default create 7 lists
    _CLI_CMD("osd.list.create");	// 0

    _CLI_CMD("osd.region.new 640 480 %u 14 2560 0 0 640 480", BUF_SCREEN); // 0 - ARGB8888 720x480

    _CLI_CMD("osd.region.insert 0 0");

    _CLI_CMD("osd.plane.flip 2 0"); // select region list 0 (region 0 - argb8888) as default
    _CLI_CMD("osd.sc.s 0 640 480 640 480");
    _CLI_CMD("osd.plane.enable 2 1");

    _CLI_CMD("osd.region.set 0 big_endian 0");	// region 0: little_endian (0)

    _CLI_CMD("osd.region.set 0 mix_sel 1"); // region 0: pixel mode (1)

    Printf("OSD init ... done\n");
#endif

    if (u4Times > 1)
        Printf(" The  pattern only 1 time use from 0 to 1 \n ");

    pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);
    vfgFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);

    x_memset((void*)vfgFileInfo, 0, 0x40000);
    LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\scaler\\vfg_list.raw\" 0x%08x /ny\n",  vfgFileInfo);
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\scaler\\vfg_list_seamless.raw\" 0x%08x /ny", vfgFileInfo));
    u4VfgFileCount = 0;
    for (i = 0;i < u4InitValue;i++)
    {
        _VGfxGetCfgFileName(&u4VfgFileCount, bCfgFileName, vfgFileInfo) ;  //bVfgFileName file name
        //Printf("test	bVfgFileName[%d]= %s\n",i,bVfgFileName);
    }

    for (i = u4InitValue ; i < u4Times ; i++)
    {
        u4CfgCount = 0;
        _VGfxGetCfgFileName(&u4VfgFileCount, bCfgFileName, vfgFileInfo);
        LOG(10, "test  bCfgFileName[%d]= %s\n", i, bCfgFileName);
        _VGfxGetVfgFileName(bCfgFileName, bVfgFileName);
        LOG(10, "test  bVfgFileName[%d]= %s\n", i, bVfgFileName);
        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\%s\" 0x%08x /ny\n",	bVfgFileName, bCfgFileName, pcFileInfo);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bCfgFileName, pcFileInfo));
        /***********************************************
        parse cfg file
        ************************************************/
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	  // input format
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);
        u4ImagX0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);   //x0 y0
        u4ImagY0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //x1 y1
        u4ImagX1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4ImagY1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //x2 y2
        u4ImagX2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4ImagY2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//x3 y3
        u4ImagX3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);

        u4ImagY3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, "test  u4ImagX0 =%d, u4ImagY0 =%d, u4ImagX1 =%d, u4ImagY1 =%d, u4ImagX2 =%d, u4ImagY2 =%d, u4ImagX3 =%d, u4ImagY3 =%d\n ", u4ImagX0, \
            u4ImagY0, u4ImagX1, u4ImagY1, u4ImagX2, u4ImagY2, u4ImagX3, u4ImagY3);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //wb x0 y0
        u4WbX0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	  //wb x1 y1
        u4WbX1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	  //wb x2 y2
        u4WbX2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	   //wb x3 y3
        u4WbX3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " %d  %d  %d %d %d %d %d %d \n", u4WbX0, u4WbY0, u4WbX1, u4WbY1, u4WbX2, u4WbY2, u4WbX3, u4WbY3);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//matrix row 0
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//matrix row 1
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//matrix row 2
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //img file
        if (_VGfxGetFileName(&u4CfgCount, bImgFileName, pcFileInfo) == 1)	 //img	file name
        {
            u4TransImg = TRUE;
        }
        else
        {
            u4TransImg = FALSE;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //img base address
        u4ImgAddress = (UINT32)ImgAddr;//_GfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//img color mode
        u4ImgCm = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        switch (u4ImgCm)	 //image color mode
        {
            case 11:
                u4ValTmp = 2;
                break;
            case 12:
                u4ValTmp = 2;
                break;
            case 13:
                u4ValTmp = 2;
                break;
            case 14:
                u4ValTmp = 4;
                break;
            default:
                return -1;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//img global alpha
        u4ImgGlAlpha = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//img width
        u4ImgWidth = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4ImgPitch = u4ImgWidth * u4ValTmp;
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//img height
        u4ImgHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4ImgCm = %d	%d	%d %d %d  \n", u4ImgAddress, u4ImgCm, u4ImgGlAlpha, u4ImgWidth, u4ImgHeight);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//texture min x y
        u4TextStartX = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4TextStartY = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//texture max x y
        u4TextEndX = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4TextEndY = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4TextStartX = %d  %d  %d %d	\n", u4TextStartX, u4TextStartY, u4TextEndX, u4TextEndY);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//Wb file
        if (_VGfxGetFileName(&u4CfgCount, bWbFileName, pcFileInfo) == 1)	//img  file name
        {
            u4TransWb = TRUE;
        }
        else
        {
            u4TransWb = FALSE;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//Wb base address
        u4WbAddress = (UINT32)DstAddr;//_GfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//wb color mode
        u4WbCm = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        switch (u4WbCm) 	//image color mode
        {
            case 11:
            case 12:
            case 13:
                u4ValTmp = 2;
                u4WbColorWidth = 2;
                break;
            case 14:
                u4ValTmp = 4;
                u4WbColorWidth = 4;
                break;
            default:
                return -1;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//wb global alpha
        u4WbGlAlpha = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//wb width
        u4WbWidth = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbPitch = u4WbWidth * u4ValTmp;
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//wb height
        u4WbHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4WbAddress = %d	u4WbCm = %d  u4WbGlAlpha = %d u4WbWidth=%d	u4WbHeight =%d \n", u4WbAddress, u4WbCm, u4WbGlAlpha, u4WbWidth, u4WbHeight);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//BB  MIN x y
        u4BBStartX = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4BBStartY = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// BB width
        u4BBWidth = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// BB height
        u4BBHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4BBStartX =%d  u4BBStartY=%d  u4BBWidth=%d  u4BBHeight =%d \n", u4BBStartX, u4BBStartY, u4BBWidth, u4BBHeight);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// filter enable
        u4FilterEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// anti aliasing enable
        u4AntiAliasEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //alpha edge mode
        u4AlphaEdge = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4FilterEn = %d  u4AntiAliasEn=%d \n", u4FilterEn, u4AntiAliasEn);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //  after alpha edge mode
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// alpha composition enable
        u4AlcomEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, "test  u4AlcomEn = %d, u4AlphaEdge= %d\n", u4AlcomEn, u4AlphaEdge);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// alpha com op mode
        u4AlcomOpMode = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// alpha com normal
        u4AlcomNormal = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// Ar
        u4AlcomAr = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// pre mult src read enable
        u4PreMultiSrcRdEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// pre mult dst read enable
        u4PreMultiDstRdEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// pre mult dst write enable
        u4PreMultiDstWrEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// img read exp
        u4ImgeRdExp = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//wb read exp
        u4WbRdExp = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//wb write rounding
        u4WbWrRounding = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//dump enable
        u4DumpEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        //#ifdef VGFX_DEBUG
        LOG(10, "test  u4AlcomEn =%d  %d  %d  %d  %d %d %d %d %d %d %d %d %d %d %d \n", u4AlcomEn, \
            u4AlcomOpMode, u4AlcomNormal, u4AlcomAr, u4PreMultiSrcRdEn, u4PreMultiDstRdEn, \
            u4PreMultiDstWrEn, u4ImgeRdExp, u4WbRdExp, u4WbWrRounding, u4DumpEn,  u4TransImg, u4TransWb, u4ImgPitch, u4WbPitch);
        // #endif
        /***********************************************
        load raw data
        ************************************************/
		LOG(10,"*********************** load raw data************************\n");
		
        x_memset((void*)ImgAddr, 0, u4FrameSize);  // load img file to u4ImgAddress
        x_memset((void*)DstAddr, 0, u4FrameSize); // load wb file to u4WbAddress
        x_memset((void*)golden_dataY, 0, u4FrameSize);
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bImgFileName,	u4ImgAddress);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bImgFileName, ImgAddr));

        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bWbFileName,  u4WbAddress);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bWbFileName, DstAddr));

        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\white_board_o.raw\" 0x%08x /ny\n", bVfgFileName, 	golden_dataY);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\white_board_o.raw\" 0x%08x /ny", bVfgFileName, golden_dataY));


        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\xform_i_coeff.txt\" 0x%08x /ny\n", bVfgFileName, pcFileInfo);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\scaler\\%s\\xform_i_coeff.txt\" 0x%08x /ny", bVfgFileName, pcFileInfo));
        u4CfgCount = 0;
        for (j = 0;j < 9; j++)
        {
            _VGfxGetCoeff(&u4Matrix[j], &u4CfgCount, pcFileInfo);
            //Printf("debug ,u4Matrix[%d]= 0x%08x \n",i ,u4Matrix[i]);
        }

#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _VGFX_SetPostWrEnv(u4PostWriteThreshold, u4WdleEn, u4WbWrRounding);
        _VGFX_SetQualityEnv(u4WbRdExp, u4ImgeRdExp, u4AntiAliasEn, u4FilterEn, u4AlphaEdge);
        //set img info

        _VGFX_SetImgage(u4ImgAddress, u4ImgCm, u4ImgGlAlpha, u4ImgPitch);
        _VGFX_SetTexture(u4TextStartX, u4TextStartY, u4TextEndX, u4TextEndY);
        // set wb info
        {
            UINT32 x1, y1, w, h;
            if (u4BBStartX >= (u4WbPitch / u4WbColorWidth))
            {
                Printf("the BB startx is not correctly\n");
                return -1;
            }
            else
            {
                x1 = u4BBStartX;
                if ((u4BBStartX + u4BBWidth) >= (u4WbPitch / u4WbColorWidth))
                {
                    w = u4WbPitch / u4WbColorWidth - x1;
                }
                else
                {
                    w = u4BBWidth;
                }
            }
            if (u4BBStartY >= u4WbHeight)
            {
                Printf("the BB starty is not correctly u4BBStartY = %d, u4WbHeight = %d\n", u4BBStartY, u4WbHeight);
                return -1;
            }
            else
            {
                y1 = u4BBStartY;
                if ((u4BBStartY + u4BBHeight) >= u4WbHeight)
                {
                    h = u4WbHeight - y1;
                }
                else
                {
                    h = u4BBHeight;
                }
            }
            _VGFX_SetBb(x1, y1, w, h);
        }

        _VGFX_SetWb(u4WbAddress, u4WbCm, u4WbGlAlpha, u4WbPitch);
        _VGFX_SetMatrixCoef_Emu(u4Matrix[0], u4Matrix[1], u4Matrix[2], u4Matrix[3], u4Matrix[4], u4Matrix[5], u4Matrix[6], u4Matrix[7], u4Matrix[8]);
        _VGFX_SetAlcom(u4AlcomEn);
        if (u4AlcomEn == TRUE)
        {
            //_GFX_SetPremult(u4PreMultiSrcRdEn, u4PreMultiDstRdEn, u4PreMultiDstWrEn, 1);
            _VGFX_GeneralAlphaComSet(u4ImgCm, u4WbCm, u4AlcomAr, u4AlcomOpMode, u4AlcomNormal, u4PreMultiSrcRdEn, u4PreMultiDstRdEn, u4PreMultiDstWrEn, 1);
        }
        _VGFX_Enable();

        GFX_Flush();
        GFX_Wait();

        x_thread_delay(1000);
        _VGFX_SetBb(u4BBWidth , u4BBStartY, u4WbWidth - u4BBWidth, u4BBHeight);
        _VGFX_Enable();
        GFX_Flush();
        GFX_Wait();

        x_thread_delay(1000);
        _VGFX_SetBb(u4BBStartX , u4BBHeight, u4BBWidth, u4WbHeight - u4BBHeight);
        _VGFX_Enable();
        GFX_Flush();
        GFX_Wait();

        x_thread_delay(1000);
        _VGFX_SetBb(u4BBWidth , u4BBHeight, u4WbWidth - u4BBWidth,  u4WbHeight - u4BBHeight);
        _VGFX_Enable();
        GFX_Flush();
        GFX_Wait();


        u4GoldenSize = u4WbWidth * u4WbHeight * u4WbColorWidth;

        for (j = 0; j < u4GoldenSize ; j++)
        {
            if (DstAddr[j] != golden_dataY[j])
            {
                Printf("hw : %x, golden : %x, j = %d\n", DstAddr[j], golden_dataY[j], j);
                Printf("compare 4times scaler error in %d times file_name=%s\n", i, bVfgFileName);
                return 1;
                //break;
            }
            else
            {
                //Printf("compare OK in %d times file_name=%s\n", i, bVfgFileName);
            }
        }
        Printf("compare 4times scaler  OK in %d times file_name=%s\n", i, bVfgFileName);
        x_thread_delay(1000);
        //_VGFX_SetTexture(49, 49, 100, 100);
        _VGFX_SetBb(133 , 75,  268, 150);
        _VGFX_Enable();
        GFX_Flush();
        GFX_Wait();

        for (j = 0; j < u4GoldenSize ; j++)
        {
            if (DstAddr[j] != golden_dataY[j])
            {
                Printf("hw : %x, golden : %x, j = %d\n", DstAddr[j], golden_dataY[j], j);
                Printf("compare clip scaler error in %d times file_name=%s\n", i, bVfgFileName);
                return 1;
                //break;
            }
            else
            {
                //Printf("compare OK in %d times file_name=%s\n", i, bVfgFileName);
            }
        }
        Printf("compare clip scaler  OK in %d times file_name=%s\n", i, bVfgFileName);

    }
    Printf("gfx scaler seamless clip  comparing OK......\n");

    return 0;

}

#endif

#if defined(CC_MT5389)||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5398)


static INT32 _GfxEmuCmdIdx2DirAlcom(INT32 i4Argc, const CHAR ** szArgv)
{
    //UINT32 u4Sx, u4Sy;
    UINT32 u4Sw, u4Sh, u4Dx, u4Dy, u4Dw, u4Dh, u4Size, u4MsbLeft, u4StByAl;
    UINT32 u4Loop, u4CountNum, u4Opcode, u4PremultSrcR, u4PremultDstW, j, u4Ar;
    UINT8*  pucTwoPass;
    UINT8* pucOnePass;

    //UINT32 u4Width, u4Height, u4DstPitch, u4MsbLeft, u4StByAl,u4SrcPitch;
    UINT32  u4DstPitch,  u4SrcPitch, u4DstCM, u4CharCM;
    UINT8 *pu1PaleTableAddr = NULL;
    UINT32 i, u4Align, u4TableEntryNum;
    // UINT32 u4CharCM, u4DstCM, u4Loops = 1;
    UINT32 u4BitsPerSrcPix, u4BytesPerDstPix;
    CHAR *szCharCM;
    CHAR *szDstCM;
    CHAR *szSrcCM;

    Printf("d.load.binary D:/MT5381_Emu/kodak16.8888 0x%08x /ny\n", BUF_ADDR1);
    Printf("d.load.binary D:/MT5381_Emu/kodak16.8888 0x%08x /ny\n", BUF_ADDR3);
    Printf("d.load.binary D:/MT5381_Emu/kodak16.8888 0x%08x /ny\n", BUF_SCREEN);

    LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny\n", BUF_ADDR1);
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny", BUF_ADDR1));
    LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny\n", BUF_ADDR3);
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny", BUF_ADDR3));
    LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny\n", BUF_SCREEN);
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny", BUF_SCREEN));

    if (i4Argc > 2) //stretch + premultiplied
    {
        u4CountNum = StrToInt(szArgv[1]);
        u4PremultSrcR = StrToInt(szArgv[2]);
        u4PremultDstW = StrToInt(szArgv[3]);
        u4Opcode = 0x4;
        u4Ar = 255;
    }
    else if (i4Argc > 1) // stretch + alpha composition
    {
        u4CountNum = StrToInt(szArgv[1]);
        u4PremultSrcR = 0;
        u4PremultDstW = 0;
        u4Opcode = E_AC_SRC;//((UINT32)rand()) % (((UINT32)E_AC_XOR) + 1);
        if ((((UINT32)rand()) % 4) == 0)
        {
            u4Ar = 255;
        }
        else
        {
            u4Ar = ((UINT32)rand()) % 256;
        }
    }
    else
    {
        u4CountNum = 1;
        u4Opcode = 0x7;
        u4PremultSrcR = 0;
        u4PremultDstW = 0;
    }

#ifdef GFX_BURST_READ
    u4Align = 64;   // 16 bytes alignment
#else
    u4Align = 16;   // 16 bytes alignment
#endif

    u4Size = (UINT32) sizeof(_u1gfxpt_img_1_clut);
    pu1PaleTableAddr = (UINT8 *)BSP_AllocAlignedDmaMemory(u4Size, u4Align);
    if (pu1PaleTableAddr == NULL)
    {
        Printf("Error : pu1PaleTableAddr is NULL\n");
        return -1;
    }

    for (i = 0; i < u4Size; i++)
    {
        pu1PaleTableAddr[i] = _u1gfxpt_img_1_clut[i];
    }

    u4TableEntryNum = (u4Size / 4);

    Printf("pale_table_entry_num = %u\n", u4TableEntryNum);
    Printf("pale_table_address = 0x%08x\n", (UINT32)pu1PaleTableAddr);

    // do many in loop
    for (u4Loop = 0; u4Loop < u4CountNum; u4Loop++)
    {
        UINT32 u4Max, u4Min;
        // select source w,h
        u4Sw = (((UINT32)rand()) % (MAX_SRC_WIDTH - MIN_SRC_WIDTH - 1)) + MIN_SRC_WIDTH;
        u4Sh = (((UINT32)rand()) % (MAX_SRC_HEIGHT - MIN_SRC_HEIGHT - 1)) + MIN_SRC_HEIGHT;
        if (u4Sw == 0)
        {
            u4Sw++;
        }
        if (u4Sh == 0)
        {
            u4Sh++;
        }

        // select dest w
        u4Max = ((u4Sw * D_SCALE_UP) > (MAX_DST_WIDTH - 1)) ? (MAX_DST_WIDTH - 1) : (u4Sw * D_X_SCALE_UP);
        u4Min = ((u4Sw / D_SCALE_DN) < MIN_DST_WIDTH) ? MIN_DST_WIDTH : (u4Sw / D_X_SCALE_DN);
        u4Dw = (((UINT32)rand()) % (u4Max - u4Min + 1)) + u4Min;

        // select dest h
        u4Max = ((u4Sh * D_SCALE_UP) > (MAX_DST_HEIGHT - 1)) ? (MAX_DST_HEIGHT - 1) : (u4Sh * D_Y_SCALE_UP);
        u4Min = ((u4Sh / D_SCALE_DN) < MIN_DST_HEIGHT) ? MIN_DST_HEIGHT : (u4Sh / D_Y_SCALE_DN);
        u4Dh = (((UINT32)rand()) % (u4Max - u4Min + 1)) + u4Min;

        // select source x,y
        // u4Sx = 0;
        //u4Sy = 0;

        // select dest x,y
        u4Dx = 0;
        u4Dy = 0;
        u4CharCM  = 3;  //only support 8bit palette mode
        u4DstCM   = CM_ARGB8888_DIRECT32 ;	//only support argb 8888 color mode
        u4MsbLeft = 0;
        u4StByAl  = 0;

        // set dst color mode
        switch (u4DstCM)
        {
            case CM_ARGB8888_DIRECT32:
                u4BytesPerDstPix = 4;
                szDstCM = "argb8888";
                break;

            case CM_ARGB4444_DIRECT16:
                u4BytesPerDstPix = 2;
                szDstCM = "argb4444";
                break;

            case CM_ARGB1555_DIRECT16:
                u4BytesPerDstPix = 2;
                szDstCM = "argb1555";
                break;

            case CM_RGB565_DIRECT16:
                u4BytesPerDstPix = 2;
                szDstCM = "rgb565";
                break;

            default: return -1;
        }

        // set char color mode
        switch (u4CharCM)
        {
            case 0: szCharCM = "1bit"; szSrcCM = "rgb1"; u4BitsPerSrcPix = 1; break;
            case 1: szCharCM = "2bit"; szSrcCM = "rgb2"; u4BitsPerSrcPix = 2; break;
            case 2: szCharCM = "4bit"; szSrcCM = "rgb4"; u4BitsPerSrcPix = 4; break;
            case 3: szCharCM = "8bit"; szSrcCM = "rgb8"; u4BitsPerSrcPix = 8; break;
            default: return -1;
        }

        u4BitsPerSrcPix = u4BitsPerSrcPix;//solve warning
        //u4SrcPitch =  (u4Width * u4BitsPerSrcPix/8);
        u4SrcPitch =  (_u4ScreenWidth * u4BitsPerSrcPix / 8);
        u4SrcPitch = (u4SrcPitch + 0xf) & (~0xf);
        u4DstPitch = (_u4ScreenWidth * u4BytesPerDstPix);
        u4DstPitch = (u4DstPitch + 0xf) & (~0xf);
        // if (((u4BitsPerSrcPix * u4Width) % 8) > 0)
        if (((u4SrcPitch) % 8) > 0)
        {
            u4StByAl = 1;
        }
        else
        {
            u4StByAl = 0;
        }

        Printf("(%d, %d, %d, %d ) u4Opcode = %d, szDstCM = %s\n", u4Dx, u4Dy, u4Dw, u4Dh, u4Opcode, szDstCM);
        // 2-pass
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        // HW Action
        // 1. set idx2dir source address
        // 2. set destination address
        // 3. set index to direct color bitblt option
        // 4. do index to direct color bitblt
        // 5. flush
        _GFX_Idx2DirSrcPitchEn(1);
        // gfx.set.idx2dirsrc {buff_base_address} {char_cm}
        _CLI_CMD("gfx.set.idx2dirsrc 0x%08x %s", BUF_ADDR1, szCharCM);

        // gfx.set.dst {buff_base_address} {color_mode} {pitch_in_bytes}
        _CLI_CMD("gfx.set.dst 0x%08x %s %u", BUF_ADDR2, szDstCM, u4DstPitch);

        // gfx.set.idx2diropt {palet_addr} {msb_left} {start_byte_align}
        _CLI_CMD("gfx.set.idx2diropt 0x%08x %u %u",
                 (UINT32)pu1PaleTableAddr, u4MsbLeft, u4StByAl);

        _CLI_CMD("gfx.set.src 0x%08x %s %u", BUF_ADDR1, szSrcCM, u4SrcPitch);
        //_GFX_SetSrc((UINT8 *)BUF_ADDR1, szCharCM, u4SrcPitch);  // only use src pitch

        // gfx.op.idx2dirblt {dx} {dy} {w} {h}
        _CLI_CMD("gfx.op.idx2dirblt %u %u %u %u", u4Dx, u4Dy, u4Dw, u4Dh);

        _CLI_CMD("gfx.flush");
        _GFX_Idx2DirSrcPitchEn(0);


        //   _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        //  _GfxSetDefMode(BUF_ADDR3, ADR_DST);

        _CLI_CMD("gfx.set.src 0x%08x %s %u", BUF_ADDR2, szDstCM, u4DstPitch); //set alpha com src pitch
        _CLI_CMD("gfx.set.dst 0x%08x %s %u", BUF_ADDR3, szDstCM, u4DstPitch);

        if ((u4PremultSrcR == 1) || (u4PremultDstW == 1))
        {
            _GFX_SetPremult(u4PremultSrcR, 0, u4PremultDstW, 1);
        }

        _GFX_GeneralAlphaCom(u4Dw, u4Dh, u4Ar, u4Opcode, 0, 0, 1, FALSE);

        _CLI_CMD("gfx.flush");

        //1-pass
        _GFX_Idx2DirSrcPitchEn(1);
        _CLI_CMD("gfx.set.idx2dirsrc 0x%08x %s", BUF_ADDR1, szCharCM);
        // _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _CLI_CMD("gfx.set.src 0x%08x %s %u", BUF_ADDR1, szSrcCM, u4SrcPitch);
        // _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.dst 0x%08x %s %u", BUF_SCREEN, szDstCM, u4DstPitch);
        //_GFX_SetSrc((UINT8 *)BUF_ADDR1, szCharCM, u4SrcPitch);  // only use src pitch
        _CLI_CMD("gfx.set.idx2diropt 0x%08x %u %u",
                 (UINT32)pu1PaleTableAddr, u4MsbLeft, u4StByAl);

        if ((u4PremultSrcR == 1) || (u4PremultDstW == 1))
        {
            _GFX_SetPremult(u4PremultSrcR, 0, u4PremultDstW, 1);
        }

        _GFX_GeneralAlphaCom(u4Dw, u4Dh, u4Ar, u4Opcode, 0, 0, 1, FALSE);
        //  _GFX_StretchAlphaCom(u4Sx, u4Sy, u4Sw, u4Sh,
        //     u4Dx, u4Dy, u4Dw, u4Dh, u4Ar, u4Opcode);

        _CLI_CMD("gfx.flush");
        _GFX_Idx2DirSrcPitchEn(0);

        pucOnePass = (UINT8*)BUF_SCREEN;
        pucTwoPass = (UINT8*)BUF_ADDR3;
        u4Size = (u4Dw * u4Dh);

        for (j = 0; j < u4Size ; j++)
        {
            if (pucOnePass[j] != pucTwoPass[j] )
            {
                printf("1Pass : %x, 2Pass : %x, j = %d\n", pucOnePass[j], pucTwoPass[j], j);
                printf("Stretch alpha composition comparing  (%d) error \n", u4Loop);
                return 1;
            }
        }
        printf("Stretch alpha composition comparing  (%d)  ok \n", u4Loop);
    }

    return 0;
}

INT32 _VGfxSkipCfgData(UINT32 *pu4CurrOfst, char *pu4FileCfg)
{
    BYTE* pbCfg = (BYTE *)pu4FileCfg;
    UINT32 u4Ofst = *pu4CurrOfst;

    while (pbCfg[u4Ofst] != '=')
    {
        u4Ofst++;
    }
    u4Ofst++;
    while ((pbCfg[u4Ofst] < 0x30) || (pbCfg[u4Ofst] > 0x7A))
    {
        u4Ofst++;
    }
    *pu4CurrOfst = u4Ofst;
    return 0;
}

INT32 _VGfxGetCfgFileName(UINT32 *pu4CurrOfst, BYTE arbFileName[], char *pu4FileCfg)
{
    BYTE* pbCfg = (BYTE *)pu4FileCfg;
    UINT32 u4ValCount = 0;
    UINT32 u4Ofst = *pu4CurrOfst;

    while ((pbCfg[u4Ofst] != 0xA))
    {
        if (((pbCfg[u4Ofst] == 0xD) && (pbCfg[u4Ofst+1] == 0xA)) || (pbCfg[u4Ofst] == 0x20) || (pbCfg[u4Ofst] == 0x09))
        {
            u4Ofst++;
            u4Ofst++;
            *pu4CurrOfst = u4Ofst;
            return 1;
        }
        arbFileName[u4ValCount] = pbCfg[u4Ofst];
        u4ValCount++;
        u4Ofst++;
    }
    u4Ofst++;
    *pu4CurrOfst = u4Ofst;
    return 1;
}


INT32 _VGfxGetVfgFileName(BYTE cfgFileName[], BYTE vfgFileName[])
{
    UINT32 u4ValCount = 0;

    while (cfgFileName[u4ValCount] != 0x2E)  // "."
    {
        vfgFileName[u4ValCount] = cfgFileName[u4ValCount];
        u4ValCount++;
        if (u4ValCount > 5)break;
    }
    return 1;
}




UINT32 _VGfxGetVal(UINT32 *pu4CurrOfst, char *pu4FileCfg)
{
    BYTE* pbCfg = (BYTE *)pu4FileCfg;
    CHAR arcVfVal[16];
    UINT32 u4ValCount = 0;
    UINT32 u4Val;
    UINT32 u4Ofst = *pu4CurrOfst;

    while ((pbCfg[u4Ofst] < 0x30) || (pbCfg[u4Ofst] > 0x7A))
    {
        u4Ofst++;
    }
    while ((pbCfg[u4Ofst] >= 0x30) && (pbCfg[u4Ofst] <= 0x7A))
    {
        arcVfVal[u4ValCount] = pbCfg[u4Ofst];
        u4ValCount++;
        u4Ofst++;
    }
    arcVfVal[u4ValCount] = 0;
    u4Val = (UINT32)StrToInt(arcVfVal);
    *pu4CurrOfst = u4Ofst;
    return u4Val;
}

void _VGfxGetCoeff(UINT32 *pu4GetData, UINT32 *pu4Ofst, char *pu4FileCfg)
{
    UINT8 *pu1FillCoeff = (UINT8*)pu4FileCfg;
    UINT32 u4Ofst = *pu4Ofst;
    UINT32 u4Val = 0;
    INT32 i;

    for (i = 0; i < 8; i++)
    {
        if ((pu1FillCoeff[u4Ofst + i] >= 0x30) &&
                (pu1FillCoeff[u4Ofst + i] <= 0x39))
        {
            u4Val |= ((pu1FillCoeff[u4Ofst + i] - 0x30) << ((7 - i) * 4));
        }
        else if ((pu1FillCoeff[u4Ofst + i] >= 0x61) &&
                 (pu1FillCoeff[u4Ofst + i] <= 0x66))
        {
            u4Val |= ((pu1FillCoeff[u4Ofst + i] - 0x57) << ((7 - i) * 4));
        }
    }
    *pu4GetData = (u4Val >> 9);
    *pu4Ofst = u4Ofst + 10;
}


INT32 _VGfxGetFileName(UINT32 *pu4CurrOfst, BYTE arbFileName[], char *pu4FileCfg)
{
    BYTE* pbCfg = (BYTE *)pu4FileCfg;
    UINT32 u4ValCount = 0;
    UINT32 u4Ofst = *pu4CurrOfst;

    while (pbCfg[u4Ofst] == 0x20)
    {
        u4Ofst++;
    }
    while (pbCfg[u4Ofst] != 0x20)
    {
        if (((pbCfg[u4Ofst] == 0xD) && (pbCfg[u4Ofst+1] == 0xA)) || (pbCfg[u4Ofst] == 0x20) || (pbCfg[u4Ofst] == 0x09))
        {
            break;
        }
        arbFileName[u4ValCount] = pbCfg[u4Ofst];
        u4ValCount++;
        u4Ofst++;
    }
    arbFileName[u4ValCount] = 0;
    *pu4CurrOfst = u4Ofst;
    if ((arbFileName[0] == 'T') &&
            (arbFileName[1] == 'R'))
    {
        return 1;
    }
    return 0;
}

#endif

#if defined(CC_MT5389)||defined(CC_MT5398)
static INT32 _GfxEmuCmdRGB0(INT32 i4Argc, const CHAR ** szArgv)
{
//    UINT32 u4Size = 0;
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count;
    INT32 i4SrcX=0, i4SrcY=0;
    INT32 i4ColIndex;
  //  INT32 i4PicSel;
    INT32 i4CountNum = 2000;
    UINT32 u4GoldenSize;
     UINT8* hw_address = NULL;
   UINT32 u4LineSize = 0, u4Line = 0;
   UINT32 j;

    Printf("alpha 0 rgb0 Test 02 ");

    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        i4ColIndex  = ((UINT32)rand()) % 147;
        i4Left      = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right     = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top       = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom    = ((UINT32)rand()) % _u4ScreenHeight;
      //  i4PicSel    = ((UINT32)rand()) % 3;
        
        i4Width = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -i4Height;
            i4Top = i4Bottom;
        }
        i4Width++;
        i4Height++;

        if (i4Width >= 3 && i4Height >= 3)
        {
            i4Width -= 2;
            i4Height -= 2;
            i4Left++;
            i4Top++;
            
            if ((_u4ScreenWidth - i4Width) != 0)
            {
                i4SrcX = ((UINT32)rand()) % (_u4ScreenWidth - i4Width);
            }
            else
            {
                i4SrcX = 0;
            }

            if ((_u4ScreenHeight - i4Height) != 0)
            {
                i4SrcY = ((UINT32)rand()) % (_u4ScreenHeight - i4Height);
            }
            else
            {
                i4SrcY = 0;
            }
        }

        Printf("%d Run : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d, ColIndex = %d\n",
            i4Left, i4Top, i4Width, i4Height, i4ColIndex);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

	i4SrcX = 0;
	i4SrcY = 0;
	  
        _GfxSetDefMode(BUF_ADDR1, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        _CLI_CMD("gfxemu.color 0x00123456");    //aliceblue
        _CLI_CMD("gfx.op.rect  %d  %d %d %d", i4SrcX, i4SrcY, i4Width, i4Height );

	 _GfxSetDefMode(BUF_ADDR1, ADR_SRC);
	 _GfxSetDefMode(BUF_SCREEN, ADR_DST);
	 _GFX_SetRGB0En(TRUE);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
                i4SrcX, i4SrcX, i4SrcX, i4SrcX, i4Width, i4Height);
        //_CLI_CMD("gfx.op.bitblt %d %d %d %d %d %d",
        //        0, 0, 0, 0, 720, 480);
        //GFX_SetAlpha(0xB7);
        _CLI_CMD("gfx.flush");
       _GFX_SetRGB0En(FALSE);

	 u4GoldenSize = i4Width*i4Height*4;  //
	 u4LineSize = i4Width*4;
	 u4Line = 0;
	 hw_address = (UINT8 *)BUF_SCREEN;
        for (j = 0; j < u4GoldenSize; j++)
        {
           if(j%u4LineSize == 0)
	    {
	      u4Line += _u4ScreenWidth*4;
	       j = u4Line;
		break;
           }
		
            if (hw_address[j] != 0)
            {
                printf("hw : %x, , j = %d\n", hw_address[j], j);
                printf("RGB0 when alphais 0 test  error in \n\n", j);
                //printf("BPCOMP_CHKSUM : %x\n",IO_READ32(GFX_ADDR, (0xb8)));
                //printf("BPCOMP_NIPPLE : %x\n",IO_READ32(GFX_ADDR, (0xb4)));
                  return 1;
                //break;
            }
        }
       Printf("RGB0 when alphais 0 test OK\n");
    }
        

    return 0;
}


/*for 5389 new feature rA = max(sA, dA), need third buf,   compare Max alpha(ThirdAddr ,src addr) 
width golden */

static INT32 _GfxEmuCmdGeneralAlcomMaxAlpha(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height;
    UINT32 u4SrcAddr, u4DstAddr, u4SrcPitch, u4DstPitch,u4ThirdAddr,u4ThirdPitch;
    UINT32 u4SrcCM,u4DstCM,u4ThirdCM;
    char* pcFileInfo;
    UINT32 i,u4Times,j;
    char* pcToken;
    UINT32 u4InitValue =0;
    UINT32 index =0;
    //UINT32 u4RepeatFrame =9999;

    UINT8* hw_address = NULL;
    UINT8* SrcAddr = NULL;
    UINT8* ThirdAddr = NULL;
    UINT8* SrcPallete = NULL;
    UINT8* ThirdPallete = NULL;
    UINT8* golden_dataY = NULL;
    UINT8* DstAddr = NULL;
    //BOOL fgFirstTime = TRUE;
    UINT32 u4FrameSize = (1920 * 1200*4);
    
    UINT32 u4GlobalAlpha,u4Opcode,u4AlcomAr,u4SrcPallte,u4ThirdPallete,u4GoldenSize;
    BOOL fgAlcomNormal,fgThirdEn,fgAlcomDiffCM;
    //golden_dataY = (UINT8*)BSP_AllocAlignedDmaMemory((1920 * 1088*4), 1024);

    #if 0
    DstAddr = BSP_AllocAlignedDmaMemory((1920 * 1200*4), 1024);
    golden_dataY = BSP_AllocAlignedDmaMemory((1920 * 1200*4), 1024);
    SrcAddr = BSP_AllocAlignedDmaMemory((1920 * 1200*4), 1024);
    ThirdAddr = BSP_AllocAlignedDmaMemory((1920 * 1200*4), 1024);
    #else
    DstAddr = (UINT8*)_u4GfxEmuBuf[0];
    golden_dataY = DstAddr + u4FrameSize;
    SrcAddr = golden_dataY + u4FrameSize;
    ThirdAddr = SrcAddr + u4FrameSize;
    //SrcPallete = ThirdAddr + 1024;
    //ThirdPallete = SrcPallete + 1024;
    #endif
    
    SrcPallete = (UINT8 *)BSP_AllocAlignedDmaMemory((1024), 1024);
    ThirdPallete = (UINT8 *)BSP_AllocAlignedDmaMemory((1024), 1024);

    if(SrcAddr == NULL || ThirdAddr==NULL || SrcPallete==NULL || ThirdPallete==NULL || DstAddr==NULL || golden_dataY==NULL)
    {
        ASSERT(0);
    }
    
    Printf("General Alpha Composition Stress Test ...");

   // u4InitValue =  (UINT32) StrToInt(szArgv[1]);
   // u4Times =  (UINT32) StrToInt(szArgv[2]);
    //u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);
     u4InitValue =  2;
    u4Times =  3;

    pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);

    for(i=u4InitValue ; i<u4Times ; i++)
    {
        //if(i == (u4RepeatFrame+1))
        //    i--;
        UINT32 file_data[21] = {0};

        index =0;
        
        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\emu_setup_%d.txt\" 0x%08x /ny\n",i,pcFileInfo);
		 UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2_maxalpha\\emu_setup_%d.txt\" 0x%08x /ny", 
		i,pcFileInfo));

		//Printf("string : %s\n",pcFileInfo);
        //sscanf(pcFileInfo,"%s %s %s %s %s %s",&file_data[0],&file_data[1],&file_data[2],&file_data[3],&file_data[4],&file_data[5]);

        pcToken = strtok(pcFileInfo, "\r\n");
        while( pcToken != NULL) 
        { 
            file_data[index++] = StrToInt(pcToken);
            pcToken = strtok(NULL, "\r\n");
        }

        u4SrcAddr = (UINT32)SrcAddr;
        u4DstAddr = (UINT32)DstAddr;
        u4ThirdAddr = (UINT32)ThirdAddr;
        u4SrcPitch = (UINT32)file_data[2]*16;
        u4DstPitch = (UINT32)file_data[3]*16;
        u4SrcCM = (UINT32)file_data[4];
        u4Height = (UINT32)file_data[5]+1;
        u4Width = (UINT32)file_data[6];
        u4GlobalAlpha = (UINT32)file_data[7];
        fgAlcomNormal = (BOOL)file_data[8];
        u4Opcode = file_data[9];
        u4AlcomAr = file_data[10];
        u4SrcPallte = (UINT32)SrcPallete;
        u4ThirdAddr = (UINT32)ThirdAddr;
        u4ThirdPallete = (UINT32)ThirdPallete;
        u4ThirdPitch = file_data[14]*16;
        u4ThirdCM = file_data[15];
        fgThirdEn = (BOOL)file_data[16];
        fgAlcomDiffCM = (BOOL)file_data[17];
        u4DstCM = (UINT32)file_data[19];
        u4GoldenSize = file_data[20];
        
        //wanring solve
	u4SrcPallte =u4SrcPallte;
	u4ThirdPallete = u4ThirdPallete;
	fgAlcomDiffCM=fgAlcomDiffCM;
	
        //if(fgFirstTime) {
            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2_maxalpha\\random_src_bmp_%d.raw\" 0x%08x /ny\n",i,u4SrcAddr);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2_maxalpha\\random_src_bmp_%d.raw\" 0x%08x /ny", 
    		(i),u4SrcAddr));

            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2_maxalpha\\random_src_plt_%d.raw\" 0x%08x /ny\n",i,SrcPallete);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2_maxalpha\\random_src_plt_%d.raw\" 0x%08x /ny", 
    		(i),SrcPallete));    		

            if(fgThirdEn)
            {
                LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2_maxalpha\\random_thd_bmp_%d.raw\" 0x%08x /ny\n",i,ThirdAddr);
                UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2_maxalpha\\random_thd_bmp_%d.raw\" 0x%08x /ny", 
    		    (i),ThirdAddr));

                LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2_maxalpha\\random_thd_plt_%d.raw\" 0x%08x /ny\n",i,ThirdPallete);
                UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2_maxalpha\\random_thd_plt_%d.raw\" 0x%08x /ny", 
    		    (i),ThirdPallete));
    		}
    		else
    		{
    		    LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2_maxalpha\\random_thd_bmp_%d.raw\" 0x%08x /ny\n",i,ThirdAddr);
                UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2_maxalpha\\random_thd_bmp_%d.raw\" 0x%08x /ny", 
    		    (i),DstAddr));

                //LOG(0, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny\n",i,ThirdPallete);
                //UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny", 
    		    //(i),ThirdPallete));
    		}
            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2_maxalpha\\dram_result_%d.raw\" 0x%08x /ny\n",(i),golden_dataY);
    		UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2_maxalpha\\dram_result_%d.raw\" 0x%08x /ny", 
    		(i),golden_dataY));
		//}

        //Printf("Src = 0x%08x, sPitch = %d, Dst = 0x%08x, dPitch = %d, W = %d, H = %d, u4RollBackEn = %d,u4QualityMode = %d, u4BpCompAddrEnd = 0x%08x, u4ColorMode = %d fgBurstRead = %d\n",
        //    u4SrcAddr, u4SrcPitch, u4DstAddr, u4DstPitch, u4Width, u4Height, u4RollBackEn,u4QualityMode,u4BpCompAddrEnd,u4ColorMode,fgBurstRead);

        printf("%d %d %d\n",u4SrcCM,u4DstCM,u4ThirdCM);
        
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GFX_SetSrc((UINT8 *)u4SrcAddr, u4SrcCM, u4SrcPitch);
        _GFX_SetThird((UINT8 *)u4ThirdAddr, u4ThirdCM, u4ThirdPitch);

        _GFX_SetDst((UINT8 *)u4DstAddr, u4DstCM, u4DstPitch);

        GFX_SetAlpha(u4GlobalAlpha);
        //GFX_SetAlpha(0xB3);
         _GFX_SetMaxAlphaEn(1);
	
        _GFX_SetCharCM(TRUE,E_BMP_CM_8BIT);
        _GFX_SetCharCM(FALSE,E_BMP_CM_8BIT);
        
        _GFX_SetThirdPallete(ThirdPallete,0,0);
        _GFX_SetIdx2DirOpt(SrcPallete,0,0);
        
#if 1      
        _GFX_GeneralAlphaCom(u4Width, u4Height,u4AlcomAr, 
            u4Opcode, 0,fgThirdEn,fgAlcomNormal,FALSE);
  #endif	
#if 0
   
	u4AlcomAr = 255;
        _GFX_GeneralAlphaCom(u4Width, u4Height,u4AlcomAr, 
            u4Opcode, 0,fgThirdEn,fgAlcomNormal,FALSE);
	  hw_address = (UINT8*)u4DstAddr;
            
        _CLI_CMD("gfx.flush");
		
	 for (j = 0; j < u4GoldenSize ; j++)
        {
            if ((hw_address[3] != 0x94)&&(hw_address[7] != 0x67)&&(hw_address[11] != 0x84)
			&&(hw_address[15] != 0xC0)&&(hw_address[19] != 0xEA)&&(hw_address[23] != 0xCC))
            {
                printf("compare error in emu_setup_%d.txt\n\n",i);
		 Printf("alpha composition alcomNormal Enable use max alpha as dst alpha error");
                //printf("BPCOMP_CHKSUM : %x\n",IO_READ32(GFX_ADDR, (0xb8)));
                //printf("BPCOMP_NIPPLE : %x\n",IO_READ32(GFX_ADDR, (0xb4)));
                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\emu_setup_%d_error.txt\" 0x%08x--0x%08x", 
    		(i),(UINT32)hw_address,(UINT32)hw_address+u4GoldenSize-1));
                return 1;
                //break;
            }
            else
            {
                //LOG(0,"hw : %x , golden : %x\n",hw_address[j],golden_dataY[j]);
            }
        }
        printf("alpha composition alcomNormal Enable use max alpha as dst alpha ok in emu_source_%d.txt\n\n",i);
	return 0;
#endif	
        hw_address = (UINT8*)u4DstAddr;
            
        _CLI_CMD("gfx.flush");

        //LOG(0,"file_size : %d\n",file_size);
        //HalFlushInvalidateDCache();

        for (j = 0; j < u4GoldenSize ; j++)
        {
            if ((hw_address[3] != 0x11)&&(hw_address[7] != 0x4e)&&(hw_address[11] != 0x24) 
			&&(hw_address[15] != 0xC0)&&(hw_address[19] != 0x1b)&&(hw_address[23] != 0xCC))
            {
                printf("compare error in emu_setup_%d.txt\n\n",i);
		 Printf("alpha composition use max alpha as dst alpha error");
                //printf("BPCOMP_CHKSUM : %x\n",IO_READ32(GFX_ADDR, (0xb8)));
                //printf("BPCOMP_NIPPLE : %x\n",IO_READ32(GFX_ADDR, (0xb4)));
                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\emu_setup_%d_error.txt\" 0x%08x--0x%08x", 
    		(i),(UINT32)hw_address,(UINT32)hw_address+u4GoldenSize-1));
                return 1;
                //break;
            }
            else
            {
                //LOG(0,"hw : %x , golden : %x\n",hw_address[j],golden_dataY[j]);
            }
        }
        _GFX_SetMaxAlphaEn(0);
        printf("alpha composition use max alpha as dst alpha ok in emu_source_%d.txt\n\n",i);
        //fgFirstTime = FALSE;
    }

    return 0;
}


// _GfxEmuCmdRop5 test 5389 new frature max and min (src, dst)

static INT32 _GfxEmuCmdRop5(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4PreCount = 0;
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count, i4RopCode;
    INT32 i4CountNum = 5000;
    INT32 i4Error = 0;

    Printf("ROP Test 05 ");

    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);
    i4RopCode = (INT32) StrToInt(szArgv[2]);   //0: MAX A R G Bcomponent
                                                                         //1: MAX A R G Bcomponent
     for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        //i4RopCode = 2;
        i4Left    = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right   = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top     = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom  = ((UINT32)rand()) % _u4ScreenHeight;

        i4Width   = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -1 * i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -1 * i4Height;
            i4Top = i4Bottom;
        }

        i4Width++;
        i4Height++;

        Printf("(%4d) : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d, RopCode = %d\n",
               i4Left, i4Top, i4Width, i4Height, i4RopCode);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        _GFX_SetColor(0x12345678);
        // Action
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.ropcode %d", i4RopCode);
        _CLI_CMD("gfx.op.rop %d %d %d %d %d %d",
                 i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");
        _GFX_SetColor(0x12345678);
        // Action
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.set.ropcode %d", i4RopCode);
        _CLI_CMD("gfx.op.rop %d %d %d %d %d %d",
                 i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                    (UINT32 *)COMPARE_BUF,
                    (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("ROP Test 04 Error ");
            Printf("(%4d) : Diff = %d, (Error = %d)\n",
                   i4Count, (i4Count - i4PreCount), ++i4Error);
            i4PreCount = i4Count;
            return -1;
        }
    }
    Printf("# of Error Run = %d, # of Total Run = %d\n", i4Error, i4CountNum);
    return 0;
}

#endif

#if defined(CC_MT5389)
static INT32 _GfxEmuCmd25dScalerSeamLessClip(INT32 i4Argc, const CHAR ** szArgv)
{
		// UINT32 u4SrcCM, u4DstCM, u4ThirdCM;
		char* pcFileInfo;
		char* vfgFileInfo;
		UINT32 i, u4Times, j;
		UINT32 u4VfgFileCount = 0;
		UINT32 u4InitValue = 0;
	
		//UINT32 u4RepeatFrame =9999;
		UINT32 u4GoldenSize;
		UINT8* golden_dataY = NULL;
		UINT8* DstAddr = NULL;
		//BOOL fgFirstTime = TRUE;
		UINT32 u4FrameSize = (1920 * 1200 * 4);
	
		UINT32 u4CfgCount = 0;
		UINT32 u4ImagX0, u4ImagY0, u4ImagX1, u4ImagY1, u4ImagX2, u4ImagY2, u4ImagX3, u4ImagY3;
		UINT32 u4WbX0, u4WbY0, u4WbX1, u4WbY1, u4WbX2, u4WbY2, u4WbX3, u4WbY3;
		UINT32 u4TransImg = FALSE, u4TransWb = FALSE;
		BYTE bImgFileName[256] = {0}, bWbFileName[256] = {0}, bCfgFileName[256] = {0};
		BYTE bVfgFileName[256] = {0};
		UINT32 u4ValTmp, u4WbColorWidth;
		//	UINT32 u4Matrix00,u4Matrix01,u4Matrix02,u4Matrix10,u4Matrix11,u4Matrix12;
	//	 UINT32 u4Matrix20,u4Matrix21,u4Matrix22;
		UINT32 u4ImgAddress, u4ImgCm, u4ImgGlAlpha, u4ImgWidth, u4ImgHeight;
		INT32 u4ImgPitch, u4WbPitch;
		UINT32 u4TextStartX, u4TextStartY, u4TextEndX, u4TextEndY;
		UINT32 u4WbAddress, u4WbCm, u4WbGlAlpha, u4WbWidth, u4WbHeight;
		UINT32 u4BBStartX, u4BBStartY, u4BBWidth, u4BBHeight;
		UINT32 u4FilterEn, u4AntiAliasEn, u4AlphaEdge;
		UINT32 u4AlcomEn, u4AlcomOpMode, u4AlcomNormal, u4AlcomAr;
		UINT32 u4PreMultiSrcRdEn, u4PreMultiDstRdEn, u4PreMultiDstWrEn;
		UINT32 u4ImgeRdExp, u4WbRdExp, u4WbWrRounding;
		UINT32 u4DumpEn;
	
   #if defined(CC_MT5389)
		UINT32 u4FilterType, u4AlEdgeRGB0, u4AlEdgeThresHold;
		UINT32 u4RefEn, u4RefOnly, u4RefTextHeight, u4RefRatioIntital, u4RefRatioStep, u4RefAAEn;
   #endif
	
		UINT32 u4PostWriteThreshold = 0xf, u4WdleEn = TRUE;
	
		UINT8* ImgAddr = NULL;
	
#if (GFX_AND_VGFX||VGFX_AND_GFX  || VGFX_AND_VGFX)
		UINT8 *ThirdAddr = NULL;
#endif
	
#if VGFX_MMU_ENABLE
		UINT32 u4VaTrasAddr = 0x3000000;
		UINT32 u4OverReadAddr;
#endif
	
		UINT32 u4Matrix[9] = {0};
#if 0//VG_FLOAT_SUPPORT
		float fSrcX[4], fSrcY[4], fDstX[4], fDstY[4], fIMtrix[3][3];
#endif
	
		//golden_dataY = (UINT8*)BSP_AllocAlignedDmaMemory((1920 * 1088*4), 1024);
	
#if 0
		DstAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
		golden_dataY = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
		SrcAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
		ThirdAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
#else
		DstAddr = (UINT8*)_u4GfxEmuBuf[0];
		golden_dataY = DstAddr + u4FrameSize;
		ImgAddr = golden_dataY + u4FrameSize;
#if (GFX_AND_VGFX||VGFX_AND_GFX|| VGFX_AND_VGFX)
		ThirdAddr = ImgAddr + u4FrameSize;
#endif
	
		//SrcPallete = ThirdAddr + 1024;
		//ThirdPallete = SrcPallete + 1024;
#endif
	
		if (i4Argc != 3)
		{
			Printf(" please input 2.5D gfxemu.25dsc start and end \n ");
			return -1;
		}
	
	
		if (DstAddr == NULL || golden_dataY == NULL)
		{
			ASSERT(0);
		}
	
		Printf("2.5d graphic scaler Stress Test ...\n");
	
	
		u4InitValue =  (UINT32) StrToInt(szArgv[1]);
		u4Times =  (UINT32) StrToInt(szArgv[2]);
		//u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);
#if 1
		Printf("Plane mixer init ...\n");
		_CLI_CMD("pmx.i");
		Printf("Plane mixer init ... done\n");
	
		Printf("OSD init ...\n");
		_CLI_CMD("osd.init");
	
		// default create 7 lists
		_CLI_CMD("osd.list.create");	// 0
	
		_CLI_CMD("osd.region.new 640 480 %u 14 2560 0 0 640 480", BUF_SCREEN); // 0 - ARGB8888 720x480
		_CLI_CMD("osd.region.insert 0 0");
	
		_CLI_CMD("osd.plane.flip 2 0"); // select region list 0 (region 0 - argb8888) as default
		_CLI_CMD("osd.sc.s 0 640 480 640 480");
		_CLI_CMD("osd.plane.enable 2 1");
	
		_CLI_CMD("osd.region.set 0 big_endian 0");	// region 0: little_endian (0)
	
		_CLI_CMD("osd.region.set 0 mix_sel 1"); // region 0: pixel mode (1)
		Printf("OSD init ... done\n");
#endif
	
		if (u4Times > 101)
			Printf(" The Max pattern is 101 \n ");
	
		pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);
		vfgFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);
	
		x_memset((void*)vfgFileInfo, 0, 0x40000);
		LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\vfg_list.raw\" 0x%08x /ny\n",  vfgFileInfo);
		UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\vfg_list.raw\" 0x%08x /ny", vfgFileInfo));
		u4VfgFileCount = 0;
		for (i = 0;i < u4InitValue;i++)
		{
			_VGfxGetCfgFileName(&u4VfgFileCount, bCfgFileName, vfgFileInfo) ;  //bVfgFileName file name
			//Printf("test	bVfgFileName[%d]= %s\n",i,bVfgFileName);
		}
	
		for (i = u4InitValue ; i < u4Times ; i++)
		{
       #if VGFX_AND_GFX
			DstAddr = (UINT8*)_u4GfxEmuBuf[0];
       #endif
		  
			u4CfgCount = 0;
			_VGfxGetCfgFileName(&u4VfgFileCount, bCfgFileName, vfgFileInfo);
			LOG(10, "test  bCfgFileName[%d]= %s\n", i, bCfgFileName);
			_VGfxGetVfgFileName(bCfgFileName, bVfgFileName);
			LOG(10, "test  bVfgFileName[%d]= %s\n", i, bVfgFileName);
			x_memset((void*)pcFileInfo, 0, 0x40000);
			LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny\n",  bVfgFileName, bCfgFileName, pcFileInfo);
			UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bCfgFileName, pcFileInfo));
			/***********************************************
			parse cfg file
			************************************************/
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	  // input format
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);
			u4ImagX0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);   //x0 y0
			u4ImagY0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //x1 y1
			u4ImagX1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			u4ImagY1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //x2 y2
			u4ImagX2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			u4ImagY2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//x3 y3
			u4ImagX3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	
			u4ImagY3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			LOG(10, "test  u4ImagX0 =%d, u4ImagY0 =%d, u4ImagX1 =%d, u4ImagY1 =%d, u4ImagX2 =%d, u4ImagY2 =%d, u4ImagX3 =%d, u4ImagY3 =%d\n ", u4ImagX0, \
				u4ImagY0, u4ImagX1, u4ImagY1, u4ImagX2, u4ImagY2, u4ImagX3, u4ImagY3);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //wb x0 y0
			u4WbX0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			u4WbY0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	  //wb x1 y1
			u4WbX1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			u4WbY1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	  //wb x2 y2
			u4WbX2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			u4WbY2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	   //wb x3 y3
			u4WbX3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			u4WbY3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			LOG(10, " %d  %d  %d %d %d %d %d %d \n", u4WbX0, u4WbY0, u4WbX1, u4WbY1, u4WbX2, u4WbY2, u4WbX3, u4WbY3);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//matrix row 0
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//matrix row 1
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//matrix row 2
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //img file
			if (_VGfxGetFileName(&u4CfgCount, bImgFileName, pcFileInfo) == 1)	 //img	file name
			{
				u4TransImg = TRUE;
			}
			else
			{
				u4TransImg = FALSE;
			}
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //img base address
			u4ImgAddress = (UINT32)ImgAddr;//_GfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//img color mode
			u4ImgCm = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			switch (u4ImgCm)	 //image color mode
			{
				case 11:
					u4ValTmp = 2;
					break;
				case 12:
					u4ValTmp = 2;
					break;
				case 13:
					u4ValTmp = 2;
					break;
				case 14:
					u4ValTmp = 4;
					break;
				default:
					return -1;
			}
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//img global alpha
			u4ImgGlAlpha = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//img width
			u4ImgWidth = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			u4ImgPitch = u4ImgWidth * u4ValTmp;
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//img height
			u4ImgHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			LOG(10, " u4ImgCm = %d	%d	%d %d %d  \n", u4ImgAddress, u4ImgCm, u4ImgGlAlpha, u4ImgWidth, u4ImgHeight);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//texture min x y
			u4TextStartX = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			u4TextStartY = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//texture max x y
			u4TextEndX = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			u4TextEndY = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			LOG(10, " u4TextStartX = %d  %d  %d %d	\n", u4TextStartX, u4TextStartY, u4TextEndX, u4TextEndY);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//Wb file
			if (_VGfxGetFileName(&u4CfgCount, bWbFileName, pcFileInfo) == 1)	//img  file name
			{
				u4TransWb = TRUE;
			}
			else
			{
				u4TransWb = FALSE;
			}
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//Wb base address
#if VGFX_AND_VGFX
			if (i % 2 == 0)
			{
				u4WbAddress = (UINT32)ThirdAddr;
			}
			else
			{
				u4WbAddress = (UINT32)DstAddr;
			}
#else
			u4WbAddress = (UINT32)DstAddr;//_GfxGetVal(&u4CfgCount, pcFileInfo);
#endif
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//wb color mode
			u4WbCm = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			switch (u4WbCm) 	//image color mode
			{
				case 11:
				case 12:
				case 13:
					u4ValTmp = 2;
					u4WbColorWidth = 2;
					break;
				case 14:
					u4ValTmp = 4;
					u4WbColorWidth = 4;
					break;
				default:
					return -1;
			}
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//wb global alpha
			u4WbGlAlpha = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//wb width
			u4WbWidth = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			u4WbPitch = u4WbWidth * u4ValTmp;
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//wb height
			u4WbHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			LOG(10, " u4WbAddress = %d	u4WbCm = %d  u4WbGlAlpha = %d u4WbWidth=%d	u4WbHeight =%d \n", u4WbAddress, u4WbCm, u4WbGlAlpha, u4WbWidth, u4WbHeight);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//BB  MIN x y
			u4BBStartX = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			u4BBStartY = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// BB width
			u4BBWidth = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// BB height
			u4BBHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			LOG(10, " u4BBStartX =%d  u4BBStartY=%d  u4BBWidth=%d  u4BBHeight =%d \n", u4BBStartX, u4BBStartY, u4BBWidth, u4BBHeight);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// filter enable
			u4FilterEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// filter type
			u4FilterType = _VGfxGetVal(&u4CfgCount, pcFileInfo);	 
	
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// anti aliasing enable
			u4AntiAliasEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //alpha edge mode	  
			u4AlphaEdge = _VGfxGetVal(&u4CfgCount, pcFileInfo);
		 _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //  after alpha edge mode
		 
			LOG(10, " u4FilterEn = %d  u4AntiAliasEn=%d \n", u4FilterEn, u4AntiAliasEn);
		_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// alpha edge rgb0
			u4AlEdgeRGB0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
		_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// alpha edge threshold
			u4AlEdgeThresHold = _VGfxGetVal(&u4CfgCount, pcFileInfo);
		_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//reflection enable
		u4RefEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
		_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//reflection only
		u4RefOnly = _VGfxGetVal(&u4CfgCount, pcFileInfo);
		_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//reflection texture height 
		u4RefTextHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
		_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//reflection ratio initial 
		u4RefRatioIntital = _VGfxGetVal(&u4CfgCount, pcFileInfo);
		_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//reflection ratio line step 
		u4RefRatioStep = _VGfxGetVal(&u4CfgCount, pcFileInfo);
		_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//reflection anti alias step 
		u4RefAAEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
		Printf( " u4FilterType = %d, u4AlEdgeRGB0 = %d, u4AlEdgeThresHold  = %d \n", 
			  u4FilterType, u4AlEdgeRGB0, u4AlEdgeThresHold);
		  
		 Printf( " u4RefEn= %d, u4RefOnly= %d, u4RefTextHeight= %d, u4RefRatioIntital= %d, u4RefRatioStep= %d, u4RefAAEn = %d \n", 
			  u4RefEn, u4RefOnly, u4RefTextHeight, u4RefRatioIntital, u4RefRatioStep, u4RefAAEn);
		   
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// alpha composition enable
			u4AlcomEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			LOG(10, "test  u4AlcomEn = %d, u4AlphaEdge= %d\n", u4AlcomEn, u4AlphaEdge);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// alpha com op mode
			u4AlcomOpMode = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// alpha com normal
			u4AlcomNormal = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// Ar
			u4AlcomAr = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// pre mult src read enable
			u4PreMultiSrcRdEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// pre mult dst read enable
			u4PreMultiDstRdEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// pre mult dst write enable
			u4PreMultiDstWrEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	// img read exp
			u4ImgeRdExp = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//wb read exp
			u4WbRdExp = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//wb write rounding
			u4WbWrRounding = _VGfxGetVal(&u4CfgCount, pcFileInfo);
			_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	//dump enable
			u4DumpEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	//#ifdef VGFX_DEBUG
			LOG(10, "test  u4AlcomEn =%d  %d  %d  %d  %d %d %d %d %d %d %d %d %d %d %d \n", u4AlcomEn, \
				u4AlcomOpMode, u4AlcomNormal, u4AlcomAr, u4PreMultiSrcRdEn, u4PreMultiDstRdEn, \
				u4PreMultiDstWrEn, u4ImgeRdExp, u4WbRdExp, u4WbWrRounding, u4DumpEn,  u4TransImg, u4TransWb, u4ImgPitch, u4WbPitch);
	// #endif
			/***********************************************
			load raw data
			************************************************/
	
			x_memset((void*)ImgAddr, 0, u4FrameSize);  // load img file to u4ImgAddress
			x_memset((void*)DstAddr, 0, u4FrameSize); // load wb file to u4WbAddress
			x_memset((void*)golden_dataY, 0, u4FrameSize);
#if VGFX_TEXTURE_ARGBORDER  //only test alu_00,  alu_00 src raw have reorder GBRA     
			 LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bImgFileName,	u4ImgAddress);
		  UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bImgFileName, ImgAddr));
#else
			LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bImgFileName,  u4ImgAddress);
			UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bImgFileName, ImgAddr));
#endif		
#if VGFX_AND_VGFX
			if (i % 2 == 0)
			{
				x_memset((void*)ThirdAddr, 0, u4FrameSize);
				LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bWbFileName,  (UINT32)ThirdAddr);
				UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bWbFileName, ThirdAddr));
			}
			else
			{
				LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bWbFileName,  u4WbAddress);
				UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bWbFileName, DstAddr));
			}
#else
			LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bWbFileName,  u4WbAddress);
			UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bWbFileName, DstAddr));
#endif
	
			LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\white_board_o.raw\" 0x%08x /ny\n", bVfgFileName,	golden_dataY);
			UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\white_board_o.raw\" 0x%08x /ny", bVfgFileName, golden_dataY));
	
#if GFX_AND_VGFX
			x_memset((void*)ImgAddr, 0, u4FrameSize);
			x_memset((void*)ThirdAddr, 0, u4FrameSize);
	
			LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bImgFileName,  (UINT32)ThirdAddr);
			UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bImgFileName, ThirdAddr));
#ifdef GFX_HW_RESET
			_CLI_CMD("gfx.reset");
#endif
			_CLI_CMD("gfxemu.mode");
			_GFX_SetDst(ImgAddr, u4WbCm, 1920*4);
			_GFX_SetSrc(ThirdAddr, u4WbCm, 1920*4);
			GFX_SetBltOpt(0, 0, 0);
			GFX_BitBlt(0, 0, 0, 0, 1920, 1080);
			// _CLI_CMD("gfx.flush");
#endif
	
#if 0//VG_FLOAT_SUPPORT
			fSrcX[0] = (float)(u4TextStartX);
			fSrcX[1] = (float)(u4TextEndX);
			fSrcX[2] = (float)(u4TextEndX);
			fSrcX[3] = (float)(u4TextStartX);
			fSrcY[0] = (float)(u4TextStartY);
			fSrcY[1] = (float)(u4TextStartY);
			fSrcY[2] = (float)(u4TextEndY);
			fSrcY[3] = (float)(u4TextEndY);
			fDstX[0] = (float)(u4WbX0);
			fDstX[1] = (float)(u4WbX1);
			fDstX[2] = (float)(u4WbX2);
			fDstX[3] = (float)(u4WbX3);
			fDstY[0] = (float)(u4WbY0);
			fDstY[1] = (float)(u4WbY1);
			fDstY[2] = (float)(u4WbY2);
			fDstY[3] = (float)(u4WbY3);
	
			derive_inverse_transform(fSrcX, fSrcY, fDstX, fDstY, fIMtrix);
	
			u4ITMatrixA = fnum_converter(fIMtrix[0][0]) >> 9;
			u4ITMatrixB = fnum_converter(fIMtrix[0][1]) >> 9;
			u4ITMatrixC = fnum_converter(fIMtrix[0][2]) >> 9;
			u4ITMatrixD = fnum_converter(fIMtrix[1][0]) >> 9;
			u4ITMatrixE = fnum_converter(fIMtrix[1][1]) >> 9;
			u4ITMatrixF = fnum_converter(fIMtrix[1][2]) >> 9;
			u4ITMatrixG = fnum_converter(fIMtrix[2][0]) >> 9;
			u4ITMatrixH = fnum_converter(fIMtrix[2][1]) >> 9;
			u4ITMatrixI = fnum_converter(fIMtrix[2][2]) >> 9;
	
#endif
			x_memset((void*)pcFileInfo, 0, 0x40000);
			LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\xform_i_coeff.txt\" 0x%08x /ny\n", bVfgFileName, pcFileInfo);
			UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\xform_i_coeff.txt\" 0x%08x /ny", bVfgFileName, pcFileInfo));
			u4CfgCount = 0;
			for (j = 0;j < 9; j++)
			{
				_VGfxGetCoeff(&u4Matrix[j], &u4CfgCount, pcFileInfo);
				//Printf("debug ,u4Matrix[%d]= 0x%08x \n",i ,u4Matrix[i]);
			}
#if GFX_AND_VGFX
	
#else
#ifdef GFX_HW_RESET
			_CLI_CMD("gfx.reset");
#endif
			_CLI_CMD("gfxemu.mode");
#endif
	
#if VGFX_MMU_ENABLE
#if VGFX_DUALCHANNEL   //only 5396 FPGA  use
			{
				u4ImgAddress = u4ImgAddress+0x40000000;
			}
#endif
			_GFX_MMU_Init();
			for (j = 0;j < 9;j++)  // UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
			{
				HalSet1MSectionTable(u4ImgAddress + j*0x100000, u4ImgAddress + u4VaTrasAddr + j*0x100000); //+48M
				HalSet1MSectionTable(u4WbAddress + j*0x100000, u4WbAddress + u4VaTrasAddr + j*0x100000);
			}
			u4OverReadAddr = u4ImgAddress + u4VaTrasAddr + u4WbWidth * u4WbHeight * u4WbColorWidth;
			_GFX_MMU_Set_OverRead(TRUE, TRUE, u4OverReadAddr >> 12);
			_GFX_MMU_Set_TabAddr(HalGetMMUTableAddress());
			_GFX_MMU_Enable(TRUE);
			//_GFX_MMU_SelfFire();
#endif
	
#if VGFX_WRITE_ARGBORDER
			_VGFX_SetWriteArgbOrder( B_ORDER, A_ORDER, R_ORDER, G_ORDER);
#endif
	
#if VGFX_TEXTURE_ARGBORDER  //only test alu_00,  alu_00 src raw have reorder GBRA  
			_VGFX_SetWriteArgbOrder( A_ORDER, R_ORDER, B_ORDER, G_ORDER);
#endif
	
	
			_VGFX_SetPostWrEnv(u4PostWriteThreshold, u4WdleEn, u4WbWrRounding);
			_VGFX_SetQualityEnv(u4WbRdExp, u4ImgeRdExp, u4AntiAliasEn, u4FilterEn, u4AlphaEdge);
			//set img info
#if VGFX_MMU_ENABLE
			_VGFX_SetImgage(u4ImgAddress + u4VaTrasAddr, u4ImgCm, u4ImgGlAlpha, u4ImgPitch);
#else
			_VGFX_SetImgage(u4ImgAddress, u4ImgCm, u4ImgGlAlpha, u4ImgPitch);
#endif
			_VGFX_SetTexture(u4TextStartX, u4TextStartY, u4TextEndX, u4TextEndY);
			// set wb info
			{
				UINT32 x1, y1, w, h;
				if (u4BBStartX >= (u4WbPitch / u4WbColorWidth))
				{
					Printf("the BB startx is not correctly\n");
					return -1;
				}
				else
				{
					x1 = u4BBStartX;
					if ((u4BBStartX + u4BBWidth) >= (u4WbPitch / u4WbColorWidth))
					{
						w = u4WbPitch / u4WbColorWidth - x1;
					}
					else
					{
						w = u4BBWidth;
					}
				}
				if (u4BBStartY >= u4WbHeight)
				{
					Printf("the BB starty is not correctly u4BBStartY = %d, u4WbHeight = %d\n", u4BBStartY, u4WbHeight);
					return -1;
				}
				else
				{
					y1 = u4BBStartY;
					if ((u4BBStartY + u4BBHeight) >= u4WbHeight)
					{
						h = u4WbHeight - y1;
					}
					else
					{
						h = u4BBHeight;
					}
				}
				_VGFX_SetBb(x1, y1, w, h);
			}
#if VGFX_MMU_ENABLE
			_VGFX_SetWb(u4WbAddress + u4VaTrasAddr, u4WbCm, u4WbGlAlpha, u4WbPitch);
#else
			_VGFX_SetWb(u4WbAddress, u4WbCm, u4WbGlAlpha, u4WbPitch);
#endif
			_VGFX_SetMatrixCoef_Emu(u4Matrix[0], u4Matrix[1], u4Matrix[2], u4Matrix[3], u4Matrix[4], u4Matrix[5], u4Matrix[6], u4Matrix[7], u4Matrix[8]);
			_VGFX_SetAlcom(u4AlcomEn);
		 //   if (u4AlcomEn == TRUE)	  //2.5d scaler need use premulty ,since u4AlcomEn is disable
			{
				//_GFX_SetPremult(u4PreMultiSrcRdEn, u4PreMultiDstRdEn, u4PreMultiDstWrEn, 1);
				_VGFX_GeneralAlphaComSet(u4ImgCm, u4WbCm, u4AlcomAr, u4AlcomOpMode, u4AlcomNormal, u4PreMultiSrcRdEn, u4PreMultiDstRdEn, u4PreMultiDstWrEn, 1);
			}
			
		   _VGFX_SetFilterType(u4FilterType);
		_VGFX_SetAlphaEdgeRGB0(u4AlEdgeRGB0, u4AlEdgeThresHold);
		_VGFX_SetReflect( u4RefEn,	u4RefOnly, u4RefTextHeight, u4RefRatioIntital, u4RefRatioStep,	u4RefAAEn);
		
			_VGFX_Enable();
	/*
	
	GFX_Flush();
	GFX_Wait();
	
	x_thread_delay(1000);
	_VGFX_SetBb(u4BBWidth , u4BBStartY, u4WbWidth - u4BBWidth, u4BBHeight);
	_VGFX_Enable();
	GFX_Flush();
	GFX_Wait();
	
	x_thread_delay(1000);
	_VGFX_SetBb(u4BBStartX , u4BBHeight, u4BBWidth, u4WbHeight - u4BBHeight);
	_VGFX_Enable();
	GFX_Flush();
	GFX_Wait();
	
	x_thread_delay(1000);
	_VGFX_SetBb(u4BBWidth , u4BBHeight, u4WbWidth - u4BBWidth,	u4WbHeight - u4BBHeight);
	_VGFX_Enable();
	GFX_Flush();
	GFX_Wait();
	
	
	u4GoldenSize = u4WbWidth * u4WbHeight * u4WbColorWidth;


	*/
#if VGFX_AND_GFX
			
			x_memset((void*)ThirdAddr, 0, u4FrameSize);
	
			_GFX_SetDst(ThirdAddr, u4WbCm, 1920*4);
			_GFX_SetSrc(DstAddr, u4WbCm, 1920*4);
			GFX_SetBltOpt(0, 0, 0);
			GFX_BitBlt(0, 0, 0, 0, 1920, 1080);
			DstAddr = ThirdAddr;
	 Printf("test by zijie 0000000000 DstAddr = 0x%x \n", DstAddr);
			// _CLI_CMD("gfx.flush");
#endif
	
#if VGFX_AND_VGFX
			if (i % 2 == 0)
			{
				GFX_Flush();
				GFX_Wait();
			}
#else
			GFX_Flush();
			GFX_Wait();
#endif
	
#if VGFX_TEXTURE_ARGBORDER  //only test alu_00,  alu_00 src raw have reorder GBRA  
		_VGFX_SetWriteArgbOrder( A_ORDER, R_ORDER, G_ORDER, B_ORDER);
#endif
	
	
			u4GoldenSize = u4WbWidth * u4WbHeight * u4WbColorWidth;
#if VGFX_WRITE_ARGBORDER
			for (j = 0; j < u4GoldenSize ; j += 4)	//only BB is RGBA
			{
				if (((DstAddr[j] != golden_dataY[j]) && (DstAddr[j+1] != golden_dataY[j+1])
						&& (DstAddr[j+2] != golden_dataY[j+2]) && (DstAddr[j+3] != golden_dataY[j+3]))
						&& ((DstAddr[j] != golden_dataY[j+3]) && (DstAddr[j+1] != golden_dataY[j])
							&& (DstAddr[j+2] != golden_dataY[j+1]) && (DstAddr[j+3] != golden_dataY[j+2])))
				{
					Printf("hw : %x, golden : %x, j = %d\n", DstAddr[j], golden_dataY[j], j);
					Printf("compare error in %d times file_name=%s\n", i, bVfgFileName);
					return 1;
					//break;
				}
				else
				{
					//Printf("compare OK in %d times file_name=%s\n", i, bVfgFileName);
				}
			}
			_VGFX_SetWriteArgbOrder( A_ORDER, R_ORDER, G_ORDER, B_ORDER); //reset
	
			Printf("gfx scaler write RGBA order comparing OK......\n");
			return 0;
#endif
#if VGFX_AND_VGFX
			if (i % 2 == 0)
			{
				DstAddr = ThirdAddr;
				for (j = 0; j < u4GoldenSize ; j++)
				{
					if (DstAddr[j] != golden_dataY[j])
					{
						Printf("hw : %x, golden : %x, j = %d\n", DstAddr[j], golden_dataY[j], j);
						Printf("compare error in %d times file_name=%s\n", i, bVfgFileName);
						return 1;
						//break;
					}
					else
					{
						//Printf("compare OK in %d times file_name=%s\n", i, bVfgFileName);
					}
				}
			}
#else
			for (j = 0; j < u4GoldenSize ; j++)
			{
				if (DstAddr[j] != golden_dataY[j])
				{
					Printf("hw : %x, golden : %x, j = %d\n", DstAddr[j], golden_dataY[j], j);
					Printf("compare error in %d times file_name=%s\n", i, bVfgFileName);
					return 1;
					//break;
				}
				else
				{
					//Printf("compare OK in %d times file_name=%s\n", i, bVfgFileName);
				}
			}
#endif
			Printf("compare OK in %d times file_name=%s\n", i, bVfgFileName);
	
		}
		Printf("gfx 25d scaler 101 pattern comparing OK......\n");
#if VGFX_MMU_ENABLE
		Printf("gfx 25d scaler enable MMU  comparing OK......\n");
#endif
	
	
		return 0;

}


static INT32 _GfxEmuCmd25dScaler(INT32 i4Argc, const CHAR ** szArgv)
{
    // UINT32 u4SrcCM, u4DstCM, u4ThirdCM;
    char* pcFileInfo;
    char* vfgFileInfo;
    UINT32 i, u4Times, j;
    UINT32 u4VfgFileCount = 0;
    UINT32 u4InitValue = 0;

    //UINT32 u4RepeatFrame =9999;
    UINT32 u4GoldenSize;
    UINT8* golden_dataY = NULL;
    UINT8* DstAddr = NULL;
    //BOOL fgFirstTime = TRUE;
    UINT32 u4FrameSize = (1920 * 1200 * 4);

    UINT32 u4CfgCount = 0;
    UINT32 u4ImagX0, u4ImagY0, u4ImagX1, u4ImagY1, u4ImagX2, u4ImagY2, u4ImagX3, u4ImagY3;
    UINT32 u4WbX0, u4WbY0, u4WbX1, u4WbY1, u4WbX2, u4WbY2, u4WbX3, u4WbY3;
    UINT32 u4TransImg = FALSE, u4TransWb = FALSE;
    BYTE bImgFileName[256] = {0}, bWbFileName[256] = {0}, bCfgFileName[256] = {0};
    BYTE bVfgFileName[256] = {0};
    UINT32 u4ValTmp, u4WbColorWidth;
    //  UINT32 u4Matrix00,u4Matrix01,u4Matrix02,u4Matrix10,u4Matrix11,u4Matrix12;
//   UINT32 u4Matrix20,u4Matrix21,u4Matrix22;
    UINT32 u4ImgAddress, u4ImgCm, u4ImgGlAlpha, u4ImgWidth, u4ImgHeight;
    INT32 u4ImgPitch, u4WbPitch;
    UINT32 u4TextStartX, u4TextStartY, u4TextEndX, u4TextEndY;
    UINT32 u4WbAddress, u4WbCm, u4WbGlAlpha, u4WbWidth, u4WbHeight;
    UINT32 u4BBStartX, u4BBStartY, u4BBWidth, u4BBHeight;
    UINT32 u4FilterEn, u4AntiAliasEn, u4AlphaEdge;
    UINT32 u4AlcomEn, u4AlcomOpMode, u4AlcomNormal, u4AlcomAr;
    UINT32 u4PreMultiSrcRdEn, u4PreMultiDstRdEn, u4PreMultiDstWrEn;
    UINT32 u4ImgeRdExp, u4WbRdExp, u4WbWrRounding;
    UINT32 u4DumpEn;

   #if defined(CC_MT5389)
    UINT32 u4FilterType, u4AlEdgeRGB0, u4AlEdgeThresHold;
    UINT32 u4RefEn, u4RefOnly, u4RefTextHeight, u4RefRatioIntital, u4RefRatioStep, u4RefAAEn;
   #endif

    UINT32 u4PostWriteThreshold = 0xf, u4WdleEn = TRUE;

    UINT8* ImgAddr = NULL;

#if (GFX_AND_VGFX||VGFX_AND_GFX  || VGFX_AND_VGFX)
    UINT8 *ThirdAddr = NULL;
#endif

#if VGFX_MMU_ENABLE
    UINT32 u4VaTrasAddr = 0x3000000;
    UINT32 u4OverReadAddr;
#endif

    UINT32 u4Matrix[9] = {0};
#if 0//VG_FLOAT_SUPPORT
    float fSrcX[4], fSrcY[4], fDstX[4], fDstY[4], fIMtrix[3][3];
#endif

    //golden_dataY = (UINT8*)BSP_AllocAlignedDmaMemory((1920 * 1088*4), 1024);

#if 0
    DstAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    golden_dataY = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    SrcAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    ThirdAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
#else
    DstAddr = (UINT8*)_u4GfxEmuBuf[0];
    golden_dataY = DstAddr + u4FrameSize;
    ImgAddr = golden_dataY + u4FrameSize;
#if (GFX_AND_VGFX||VGFX_AND_GFX|| VGFX_AND_VGFX)
    ThirdAddr = ImgAddr + u4FrameSize;
#endif

    //SrcPallete = ThirdAddr + 1024;
    //ThirdPallete = SrcPallete + 1024;
#endif

    if (i4Argc != 3)
    {
        Printf(" please input 2.5D gfxemu.25dsc start and end \n ");
        return -1;
    }


    if (DstAddr == NULL || golden_dataY == NULL)
    {
        ASSERT(0);
    }

    Printf("2.5d graphic scaler Stress Test ...\n");


    u4InitValue =  (UINT32) StrToInt(szArgv[1]);
    u4Times =  (UINT32) StrToInt(szArgv[2]);
    //u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);
#if 1
    Printf("Plane mixer init ...\n");
    _CLI_CMD("pmx.i");
    Printf("Plane mixer init ... done\n");

    Printf("OSD init ...\n");
    _CLI_CMD("osd.init");

    // default create 7 lists
    _CLI_CMD("osd.list.create");    // 0

    _CLI_CMD("osd.region.new 640 480 %u 14 2560 0 0 640 480", BUF_SCREEN); // 0 - ARGB8888 720x480
    _CLI_CMD("osd.region.insert 0 0");

    _CLI_CMD("osd.plane.flip 2 0"); // select region list 0 (region 0 - argb8888) as default
    _CLI_CMD("osd.sc.s 0 640 480 640 480");
    _CLI_CMD("osd.plane.enable 2 1");

    _CLI_CMD("osd.region.set 0 big_endian 0");  // region 0: little_endian (0)

    _CLI_CMD("osd.region.set 0 mix_sel 1"); // region 0: pixel mode (1)
    Printf("OSD init ... done\n");
#endif

    if (u4Times > 101)
        Printf(" The Max pattern is 101 \n ");

    pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);
    vfgFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);

    x_memset((void*)vfgFileInfo, 0, 0x40000);
    LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\vfg_list.raw\" 0x%08x /ny\n",  vfgFileInfo);
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\vfg_list.raw\" 0x%08x /ny", vfgFileInfo));
    u4VfgFileCount = 0;
    for (i = 0;i < u4InitValue;i++)
    {
        _VGfxGetCfgFileName(&u4VfgFileCount, bCfgFileName, vfgFileInfo) ;  //bVfgFileName file name
        //Printf("test  bVfgFileName[%d]= %s\n",i,bVfgFileName);
    }

    for (i = u4InitValue ; i < u4Times ; i++)
    {
       #if VGFX_AND_GFX
	    DstAddr = (UINT8*)_u4GfxEmuBuf[0];
       #endif
      
        u4CfgCount = 0;
        _VGfxGetCfgFileName(&u4VfgFileCount, bCfgFileName, vfgFileInfo);
        LOG(10, "test  bCfgFileName[%d]= %s\n", i, bCfgFileName);
        _VGfxGetVfgFileName(bCfgFileName, bVfgFileName);
        LOG(10, "test  bVfgFileName[%d]= %s\n", i, bVfgFileName);
        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny\n",  bVfgFileName, bCfgFileName, pcFileInfo);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bCfgFileName, pcFileInfo));
        /***********************************************
        parse cfg file
        ************************************************/
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);    // input format
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);
        u4ImagX0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);   //x0 y0
        u4ImagY0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //x1 y1
        u4ImagX1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4ImagY1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //x2 y2
        u4ImagX2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4ImagY2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //x3 y3
        u4ImagX3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);

        u4ImagY3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, "test  u4ImagX0 =%d, u4ImagY0 =%d, u4ImagX1 =%d, u4ImagY1 =%d, u4ImagX2 =%d, u4ImagY2 =%d, u4ImagX3 =%d, u4ImagY3 =%d\n ", u4ImagX0, \
            u4ImagY0, u4ImagX1, u4ImagY1, u4ImagX2, u4ImagY2, u4ImagX3, u4ImagY3);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //wb x0 y0
        u4WbX0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);    //wb x1 y1
        u4WbX1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);    //wb x2 y2
        u4WbX2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);     //wb x3 y3
        u4WbX3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " %d  %d  %d %d %d %d %d %d \n", u4WbX0, u4WbY0, u4WbX1, u4WbY1, u4WbX2, u4WbY2, u4WbX3, u4WbY3);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //matrix row 0
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //matrix row 1
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //matrix row 2
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //img file
        if (_VGfxGetFileName(&u4CfgCount, bImgFileName, pcFileInfo) == 1)    //img  file name
        {
            u4TransImg = TRUE;
        }
        else
        {
            u4TransImg = FALSE;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //img base address
        u4ImgAddress = (UINT32)ImgAddr;//_GfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //img color mode
        u4ImgCm = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        switch (u4ImgCm)     //image color mode
        {
            case 11:
                u4ValTmp = 2;
                break;
            case 12:
                u4ValTmp = 2;
                break;
            case 13:
                u4ValTmp = 2;
                break;
            case 14:
                u4ValTmp = 4;
                break;
            default:
                return -1;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //img global alpha
        u4ImgGlAlpha = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //img width
        u4ImgWidth = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4ImgPitch = u4ImgWidth * u4ValTmp;
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //img height
        u4ImgHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4ImgCm = %d  %d  %d %d %d  \n", u4ImgAddress, u4ImgCm, u4ImgGlAlpha, u4ImgWidth, u4ImgHeight);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //texture min x y
        u4TextStartX = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4TextStartY = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //texture max x y
        u4TextEndX = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4TextEndY = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4TextStartX = %d  %d  %d %d  \n", u4TextStartX, u4TextStartY, u4TextEndX, u4TextEndY);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //Wb file
        if (_VGfxGetFileName(&u4CfgCount, bWbFileName, pcFileInfo) == 1)    //img  file name
        {
            u4TransWb = TRUE;
        }
        else
        {
            u4TransWb = FALSE;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //Wb base address
#if VGFX_AND_VGFX
        if (i % 2 == 0)
        {
            u4WbAddress = (UINT32)ThirdAddr;
        }
        else
        {
            u4WbAddress = (UINT32)DstAddr;
        }
#else
        u4WbAddress = (UINT32)DstAddr;//_GfxGetVal(&u4CfgCount, pcFileInfo);
#endif
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb color mode
        u4WbCm = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        switch (u4WbCm)     //image color mode
        {
            case 11:
            case 12:
            case 13:
                u4ValTmp = 2;
                u4WbColorWidth = 2;
                break;
            case 14:
                u4ValTmp = 4;
                u4WbColorWidth = 4;
                break;
            default:
                return -1;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb global alpha
        u4WbGlAlpha = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb width
        u4WbWidth = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbPitch = u4WbWidth * u4ValTmp;
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb height
        u4WbHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4WbAddress = %d  u4WbCm = %d  u4WbGlAlpha = %d u4WbWidth=%d  u4WbHeight =%d \n", u4WbAddress, u4WbCm, u4WbGlAlpha, u4WbWidth, u4WbHeight);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //BB  MIN x y
        u4BBStartX = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4BBStartY = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // BB width
        u4BBWidth = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // BB height
        u4BBHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4BBStartX =%d  u4BBStartY=%d  u4BBWidth=%d  u4BBHeight =%d \n", u4BBStartX, u4BBStartY, u4BBWidth, u4BBHeight);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // filter enable
        u4FilterEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // filter type
        u4FilterType = _VGfxGetVal(&u4CfgCount, pcFileInfo);	 

        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // anti aliasing enable
        u4AntiAliasEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //alpha edge mode    
        u4AlphaEdge = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	 _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //  after alpha edge mode
	 
        LOG(10, " u4FilterEn = %d  u4AntiAliasEn=%d \n", u4FilterEn, u4AntiAliasEn);
	_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // alpha edge rgb0
        u4AlEdgeRGB0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // alpha edge threshold
        u4AlEdgeThresHold = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //reflection enable
	u4RefEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //reflection only
	u4RefOnly = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //reflection texture height 
	u4RefTextHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //reflection ratio initial 
	u4RefRatioIntital = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //reflection ratio line step 
	u4RefRatioStep = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //reflection anti alias step 
	u4RefAAEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	Printf( " u4FilterType = %d, u4AlEdgeRGB0 = %d, u4AlEdgeThresHold  = %d \n", 
	 	  u4FilterType, u4AlEdgeRGB0, u4AlEdgeThresHold);
      
	 Printf( " u4RefEn= %d, u4RefOnly= %d, u4RefTextHeight= %d, u4RefRatioIntital= %d, u4RefRatioStep= %d, u4RefAAEn = %d \n", 
	 	  u4RefEn, u4RefOnly, u4RefTextHeight, u4RefRatioIntital, u4RefRatioStep, u4RefAAEn);
       
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // alpha composition enable
        u4AlcomEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, "test  u4AlcomEn = %d, u4AlphaEdge= %d\n", u4AlcomEn, u4AlphaEdge);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // alpha com op mode
        u4AlcomOpMode = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // alpha com normal
        u4AlcomNormal = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // Ar
        u4AlcomAr = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // pre mult src read enable
        u4PreMultiSrcRdEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // pre mult dst read enable
        u4PreMultiDstRdEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // pre mult dst write enable
        u4PreMultiDstWrEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // img read exp
        u4ImgeRdExp = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb read exp
        u4WbRdExp = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb write rounding
        u4WbWrRounding = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //dump enable
        u4DumpEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
//#ifdef VGFX_DEBUG
        LOG(10, "test  u4AlcomEn =%d  %d  %d  %d  %d %d %d %d %d %d %d %d %d %d %d \n", u4AlcomEn, \
            u4AlcomOpMode, u4AlcomNormal, u4AlcomAr, u4PreMultiSrcRdEn, u4PreMultiDstRdEn, \
            u4PreMultiDstWrEn, u4ImgeRdExp, u4WbRdExp, u4WbWrRounding, u4DumpEn,  u4TransImg, u4TransWb, u4ImgPitch, u4WbPitch);
// #endif
        /***********************************************
        load raw data
        ************************************************/

        x_memset((void*)ImgAddr, 0, u4FrameSize);  // load img file to u4ImgAddress
        x_memset((void*)DstAddr, 0, u4FrameSize); // load wb file to u4WbAddress
        x_memset((void*)golden_dataY, 0, u4FrameSize);
#if VGFX_TEXTURE_ARGBORDER  //only test alu_00,  alu_00 src raw have reorder GBRA     
         LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bImgFileName,  u4ImgAddress);
	  UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bImgFileName, ImgAddr));
#else
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bImgFileName,  u4ImgAddress);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bImgFileName, ImgAddr));
#endif		
#if VGFX_AND_VGFX
        if (i % 2 == 0)
        {
            x_memset((void*)ThirdAddr, 0, u4FrameSize);
            LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bWbFileName,  (UINT32)ThirdAddr);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bWbFileName, ThirdAddr));
        }
        else
        {
            LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bWbFileName,  u4WbAddress);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bWbFileName, DstAddr));
        }
#else
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bWbFileName,  u4WbAddress);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bWbFileName, DstAddr));
#endif

        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\white_board_o.raw\" 0x%08x /ny\n", bVfgFileName, 	golden_dataY);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\white_board_o.raw\" 0x%08x /ny", bVfgFileName, golden_dataY));

#if GFX_AND_VGFX
        x_memset((void*)ImgAddr, 0, u4FrameSize);
        x_memset((void*)ThirdAddr, 0, u4FrameSize);

        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny\n", bVfgFileName, bImgFileName,  (UINT32)ThirdAddr);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\%s\" 0x%08x /ny", bVfgFileName, bImgFileName, ThirdAddr));
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");
        _GFX_SetDst(ImgAddr, u4WbCm, 1920*4);
        _GFX_SetSrc(ThirdAddr, u4WbCm, 1920*4);
        GFX_SetBltOpt(0, 0, 0);
        GFX_BitBlt(0, 0, 0, 0, 1920, 1080);
        // _CLI_CMD("gfx.flush");
#endif

#if 0//VG_FLOAT_SUPPORT
        fSrcX[0] = (float)(u4TextStartX);
        fSrcX[1] = (float)(u4TextEndX);
        fSrcX[2] = (float)(u4TextEndX);
        fSrcX[3] = (float)(u4TextStartX);
        fSrcY[0] = (float)(u4TextStartY);
        fSrcY[1] = (float)(u4TextStartY);
        fSrcY[2] = (float)(u4TextEndY);
        fSrcY[3] = (float)(u4TextEndY);
        fDstX[0] = (float)(u4WbX0);
        fDstX[1] = (float)(u4WbX1);
        fDstX[2] = (float)(u4WbX2);
        fDstX[3] = (float)(u4WbX3);
        fDstY[0] = (float)(u4WbY0);
        fDstY[1] = (float)(u4WbY1);
        fDstY[2] = (float)(u4WbY2);
        fDstY[3] = (float)(u4WbY3);

        derive_inverse_transform(fSrcX, fSrcY, fDstX, fDstY, fIMtrix);

        u4ITMatrixA = fnum_converter(fIMtrix[0][0]) >> 9;
        u4ITMatrixB = fnum_converter(fIMtrix[0][1]) >> 9;
        u4ITMatrixC = fnum_converter(fIMtrix[0][2]) >> 9;
        u4ITMatrixD = fnum_converter(fIMtrix[1][0]) >> 9;
        u4ITMatrixE = fnum_converter(fIMtrix[1][1]) >> 9;
        u4ITMatrixF = fnum_converter(fIMtrix[1][2]) >> 9;
        u4ITMatrixG = fnum_converter(fIMtrix[2][0]) >> 9;
        u4ITMatrixH = fnum_converter(fIMtrix[2][1]) >> 9;
        u4ITMatrixI = fnum_converter(fIMtrix[2][2]) >> 9;

#endif
        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\xform_i_coeff.txt\" 0x%08x /ny\n", bVfgFileName, pcFileInfo);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\25dscaler\\%s\\xform_i_coeff.txt\" 0x%08x /ny", bVfgFileName, pcFileInfo));
        u4CfgCount = 0;
        for (j = 0;j < 9; j++)
        {
            _VGfxGetCoeff(&u4Matrix[j], &u4CfgCount, pcFileInfo);
            //Printf("debug ,u4Matrix[%d]= 0x%08x \n",i ,u4Matrix[i]);
        }
#if GFX_AND_VGFX

#else
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");
#endif

#if VGFX_MMU_ENABLE
#if VGFX_DUALCHANNEL   //only 5396 FPGA  use
		{
			u4ImgAddress = u4ImgAddress+0x40000000;
		}
#endif
        _GFX_MMU_Init();
        for (j = 0;j < 9;j++)  // UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
        {
            HalSet1MSectionTable(u4ImgAddress + j*0x100000, u4ImgAddress + u4VaTrasAddr + j*0x100000); //+48M
            HalSet1MSectionTable(u4WbAddress + j*0x100000, u4WbAddress + u4VaTrasAddr + j*0x100000);
        }
        u4OverReadAddr = u4ImgAddress + u4VaTrasAddr + u4WbWidth * u4WbHeight * u4WbColorWidth;
        _GFX_MMU_Set_OverRead(TRUE, TRUE, u4OverReadAddr >> 12);
        _GFX_MMU_Set_TabAddr(HalGetMMUTableAddress());
        _GFX_MMU_Enable(TRUE);
        //_GFX_MMU_SelfFire();
#endif

#if VGFX_WRITE_ARGBORDER
        _VGFX_SetWriteArgbOrder( B_ORDER, A_ORDER, R_ORDER, G_ORDER);
#endif

#if VGFX_TEXTURE_ARGBORDER  //only test alu_00,  alu_00 src raw have reorder GBRA  
        _VGFX_SetWriteArgbOrder( A_ORDER, R_ORDER, B_ORDER, G_ORDER);
#endif


        _VGFX_SetPostWrEnv(u4PostWriteThreshold, u4WdleEn, u4WbWrRounding);
        _VGFX_SetQualityEnv(u4WbRdExp, u4ImgeRdExp, u4AntiAliasEn, u4FilterEn, u4AlphaEdge);
        //set img info
#if VGFX_MMU_ENABLE
        _VGFX_SetImgage(u4ImgAddress + u4VaTrasAddr, u4ImgCm, u4ImgGlAlpha, u4ImgPitch);
#else
        _VGFX_SetImgage(u4ImgAddress, u4ImgCm, u4ImgGlAlpha, u4ImgPitch);
#endif
        _VGFX_SetTexture(u4TextStartX, u4TextStartY, u4TextEndX, u4TextEndY);
        // set wb info
        {
            UINT32 x1, y1, w, h;
            if (u4BBStartX >= (u4WbPitch / u4WbColorWidth))
            {
                Printf("the BB startx is not correctly\n");
                return -1;
            }
            else
            {
                x1 = u4BBStartX;
                if ((u4BBStartX + u4BBWidth) >= (u4WbPitch / u4WbColorWidth))
                {
                    w = u4WbPitch / u4WbColorWidth - x1;
                }
                else
                {
                    w = u4BBWidth;
                }
            }
            if (u4BBStartY >= u4WbHeight)
            {
                Printf("the BB starty is not correctly u4BBStartY = %d, u4WbHeight = %d\n", u4BBStartY, u4WbHeight);
                return -1;
            }
            else
            {
                y1 = u4BBStartY;
                if ((u4BBStartY + u4BBHeight) >= u4WbHeight)
                {
                    h = u4WbHeight - y1;
                }
                else
                {
                    h = u4BBHeight;
                }
            }
            _VGFX_SetBb(x1, y1, w, h);
        }
#if VGFX_MMU_ENABLE
        _VGFX_SetWb(u4WbAddress + u4VaTrasAddr, u4WbCm, u4WbGlAlpha, u4WbPitch);
#else
        _VGFX_SetWb(u4WbAddress, u4WbCm, u4WbGlAlpha, u4WbPitch);
#endif
        _VGFX_SetMatrixCoef_Emu(u4Matrix[0], u4Matrix[1], u4Matrix[2], u4Matrix[3], u4Matrix[4], u4Matrix[5], u4Matrix[6], u4Matrix[7], u4Matrix[8]);
        _VGFX_SetAlcom(u4AlcomEn);
     //   if (u4AlcomEn == TRUE)      //2.5d scaler need use premulty ,since u4AlcomEn is disable
        {
            //_GFX_SetPremult(u4PreMultiSrcRdEn, u4PreMultiDstRdEn, u4PreMultiDstWrEn, 1);
            _VGFX_GeneralAlphaComSet(u4ImgCm, u4WbCm, u4AlcomAr, u4AlcomOpMode, u4AlcomNormal, u4PreMultiSrcRdEn, u4PreMultiDstRdEn, u4PreMultiDstWrEn, 1);
        }
		
       _VGFX_SetFilterType(u4FilterType);
	_VGFX_SetAlphaEdgeRGB0(u4AlEdgeRGB0, u4AlEdgeThresHold);
	_VGFX_SetReflect( u4RefEn,  u4RefOnly, u4RefTextHeight, u4RefRatioIntital, u4RefRatioStep,  u4RefAAEn);
	
        _VGFX_Enable();

#if VGFX_AND_GFX
		
        x_memset((void*)ThirdAddr, 0, u4FrameSize);

        _GFX_SetDst(ThirdAddr, u4WbCm, 1920*4);
        _GFX_SetSrc(DstAddr, u4WbCm, 1920*4);
        GFX_SetBltOpt(0, 0, 0);
        GFX_BitBlt(0, 0, 0, 0, 1920, 1080);
        DstAddr = ThirdAddr;
 Printf("test by zijie 0000000000 DstAddr = 0x%x \n", DstAddr);
        // _CLI_CMD("gfx.flush");
#endif

#if VGFX_AND_VGFX
        if (i % 2 == 0)
        {
            GFX_Flush();
            GFX_Wait();
        }
#else
        GFX_Flush();
        GFX_Wait();
#endif

#if VGFX_TEXTURE_ARGBORDER  //only test alu_00,  alu_00 src raw have reorder GBRA  
	_VGFX_SetWriteArgbOrder( A_ORDER, R_ORDER, G_ORDER, B_ORDER);
#endif


        u4GoldenSize = u4WbWidth * u4WbHeight * u4WbColorWidth;
#if VGFX_WRITE_ARGBORDER
        for (j = 0; j < u4GoldenSize ; j += 4)  //only BB is RGBA
        {
            if (((DstAddr[j] != golden_dataY[j]) && (DstAddr[j+1] != golden_dataY[j+1])
                    && (DstAddr[j+2] != golden_dataY[j+2]) && (DstAddr[j+3] != golden_dataY[j+3]))
                    && ((DstAddr[j] != golden_dataY[j+3]) && (DstAddr[j+1] != golden_dataY[j])
                        && (DstAddr[j+2] != golden_dataY[j+1]) && (DstAddr[j+3] != golden_dataY[j+2])))
            {
                Printf("hw : %x, golden : %x, j = %d\n", DstAddr[j], golden_dataY[j], j);
                Printf("compare error in %d times file_name=%s\n", i, bVfgFileName);
                return 1;
                //break;
            }
            else
            {
                //Printf("compare OK in %d times file_name=%s\n", i, bVfgFileName);
            }
        }
        _VGFX_SetWriteArgbOrder( A_ORDER, R_ORDER, G_ORDER, B_ORDER); //reset

        Printf("gfx scaler write RGBA order comparing OK......\n");
        return 0;
#endif
#if VGFX_AND_VGFX
        if (i % 2 == 0)
        {
            DstAddr = ThirdAddr;
            for (j = 0; j < u4GoldenSize ; j++)
            {
                if (DstAddr[j] != golden_dataY[j])
                {
                    Printf("hw : %x, golden : %x, j = %d\n", DstAddr[j], golden_dataY[j], j);
                    Printf("compare error in %d times file_name=%s\n", i, bVfgFileName);
                    return 1;
                    //break;
                }
                else
                {
                    //Printf("compare OK in %d times file_name=%s\n", i, bVfgFileName);
                }
            }
        }
#else
        for (j = 0; j < u4GoldenSize ; j++)
        {
            if (DstAddr[j] != golden_dataY[j])
            {
                Printf("hw : %x, golden : %x, j = %d\n", DstAddr[j], golden_dataY[j], j);
                Printf("compare error in %d times file_name=%s\n", i, bVfgFileName);
                return 1;
                //break;
            }
            else
            {
                //Printf("compare OK in %d times file_name=%s\n", i, bVfgFileName);
            }
        }
#endif
        Printf("compare OK in %d times file_name=%s\n", i, bVfgFileName);

    }
    Printf("gfx 25d scaler 101 pattern comparing OK......\n");
#if VGFX_MMU_ENABLE
    Printf("gfx 25d scaler enable MMU  comparing OK......\n");
#endif


    return 0;

}

static INT32 _GfxEmuCmd25dScalerRandom(INT32 i4Argc, const CHAR ** szArgv)
{
    // UINT32 u4SrcCM, u4DstCM, u4ThirdCM;
    char* pcFileInfo;
    UINT32 i, u4Times, j;
    UINT32 u4InitValue = 1;

    //UINT32 u4RepeatFrame =9999;
    UINT32 u4GoldenSize;
    UINT8* golden_dataY = NULL;
    UINT8* DstAddr = NULL;
    //BOOL fgFirstTime = TRUE;
    UINT32 u4FrameSize = (1920 * 1200 * 4);

    UINT32 u4CfgCount = 0;
    UINT32 u4ImagX0, u4ImagY0, u4ImagX1, u4ImagY1, u4ImagX2, u4ImagY2, u4ImagX3, u4ImagY3;
    UINT32 u4WbX0, u4WbY0, u4WbX1, u4WbY1, u4WbX2, u4WbY2, u4WbX3, u4WbY3;
    UINT32 u4TransImg = FALSE, u4TransWb = FALSE;
    BYTE bImgFileName[256] = {0}, bWbFileName[256] = {0};
    BYTE bVfgFileName[256] = {0};
    UINT32 u4ValTmp, u4WbColorWidth;
    //  UINT32 u4Matrix00,u4Matrix01,u4Matrix02,u4Matrix10,u4Matrix11,u4Matrix12;
//   UINT32 u4Matrix20,u4Matrix21,u4Matrix22;
    UINT32 u4ImgAddress, u4ImgCm, u4ImgGlAlpha, u4ImgWidth, u4ImgHeight;
    INT32 u4ImgPitch, u4WbPitch;
    UINT32 u4TextStartX, u4TextStartY, u4TextEndX, u4TextEndY;
    UINT32 u4WbAddress, u4WbCm, u4WbGlAlpha, u4WbWidth, u4WbHeight;
    UINT32 u4BBStartX, u4BBStartY, u4BBWidth, u4BBHeight;
    UINT32 u4FilterEn, u4AntiAliasEn, u4AlphaEdge;
    UINT32 u4AlcomEn, u4AlcomOpMode, u4AlcomNormal, u4AlcomAr;
    UINT32 u4PreMultiSrcRdEn, u4PreMultiDstRdEn, u4PreMultiDstWrEn;
    UINT32 u4ImgeRdExp, u4WbRdExp, u4WbWrRounding;
    UINT32 u4DumpEn;
#if defined(CC_MT5389)
	 UINT32 u4FilterType, u4AlEdgeRGB0, u4AlEdgeThresHold;
	 UINT32 u4RefEn, u4RefOnly, u4RefTextHeight, u4RefRatioIntital, u4RefRatioStep, u4RefAAEn;
#endif

    UINT32 u4PostWriteThreshold = 0xf, u4WdleEn = TRUE;

    UINT8* ImgAddr = NULL;

    UINT32 u4Matrix[9] = {0};
#if 0//VG_FLOAT_SUPPORT
    float fSrcX[4], fSrcY[4], fDstX[4], fDstY[4], fIMtrix[3][3];
#endif

    //golden_dataY = (UINT8*)BSP_AllocAlignedDmaMemory((1920 * 1088*4), 1024);

#if 0
    DstAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    golden_dataY = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    SrcAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
    ThirdAddr = BSP_AllocAlignedDmaMemory((1920 * 1200 * 4), 1024);
#else
    DstAddr = (UINT8*)_u4GfxEmuBuf[0];
    golden_dataY = DstAddr + u4FrameSize;
    ImgAddr = golden_dataY + u4FrameSize;

    //SrcPallete = ThirdAddr + 1024;
    //ThirdPallete = SrcPallete + 1024;
#endif

    if ((szArgv == NULL) || (szArgv[1] == NULL) )
    {
        Printf(" please input auto test times \n ");
        return -1;
    }


    if (DstAddr == NULL || golden_dataY == NULL)
    {
        ASSERT(0);
    }

    Printf("2.5 d graphic scaler auto  Test ...\n");

    //u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);
    u4InitValue =  (UINT32) StrToInt(szArgv[1]);
    u4Times =	(UINT32) StrToInt(szArgv[2]);


    pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);


    for (i = u4InitValue ; i < u4Times ; i++)
    {
        u4CfgCount = 0;
        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"f:\\25dscaler\\random_vgfx_config\\random_%d.cfg\" 0x%08x /ny\n",  i, pcFileInfo);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\25dscaler\\random_vgfx_config\\random_%d.cfg\" 0x%08x /ny", i, pcFileInfo));
        /***********************************************
        parse cfg file
        ************************************************/
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);    // input format
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);
        u4ImagX0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);   //x0 y0
        u4ImagY0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //x1 y1
        u4ImagX1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4ImagY1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //x2 y2
        u4ImagX2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4ImagY2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //x3 y3
        u4ImagX3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);

        u4ImagY3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, "test  u4ImagX0 =%d, u4ImagY0 =%d, u4ImagX1 =%d, u4ImagY1 =%d, u4ImagX2 =%d, u4ImagY2 =%d, u4ImagX3 =%d, u4ImagY3 =%d\n ", u4ImagX0, \
            u4ImagY0, u4ImagX1, u4ImagY1, u4ImagX2, u4ImagY2, u4ImagX3, u4ImagY3);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //wb x0 y0
        u4WbX0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);    //wb x1 y1
        u4WbX1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY1 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);    //wb x2 y2
        u4WbX2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY2 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);     //wb x3 y3
        u4WbX3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbY3 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " %d  %d  %d %d %d %d %d %d \n", u4WbX0, u4WbY0, u4WbX1, u4WbY1, u4WbX2, u4WbY2, u4WbX3, u4WbY3);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //matrix row 0
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //matrix row 1
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //matrix row 2
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //img file
        if (_VGfxGetFileName(&u4CfgCount, bImgFileName, pcFileInfo) == 1)    //img  file name
        {
            u4TransImg = TRUE;
        }
        else
        {
            u4TransImg = FALSE;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);	 //img base address
        u4ImgAddress = (UINT32)ImgAddr;//_GfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //img color mode
        u4ImgCm = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        switch (u4ImgCm)     //image color mode
        {
            case 11:
                u4ValTmp = 2;
                break;
            case 12:
                u4ValTmp = 2;
                break;
            case 13:
                u4ValTmp = 2;
                break;
            case 14:
                u4ValTmp = 4;
                break;
            default:
                return -1;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //img global alpha
        u4ImgGlAlpha = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //img width
        u4ImgWidth = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4ImgPitch = u4ImgWidth * u4ValTmp;
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //img height
        u4ImgHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4ImgCm = %d  %d  %d %d %d  \n", u4ImgAddress, u4ImgCm, u4ImgGlAlpha, u4ImgWidth, u4ImgHeight);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //texture min x y
        u4TextStartX = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4TextStartY = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //texture max x y
        u4TextEndX = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4TextEndY = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4TextStartX = %d  %d  %d %d  \n", u4TextStartX, u4TextStartY, u4TextEndX, u4TextEndY);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //Wb file
        if (_VGfxGetFileName(&u4CfgCount, bWbFileName, pcFileInfo) == 1)    //img  file name
        {
            u4TransWb = TRUE;
        }
        else
        {
            u4TransWb = FALSE;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //Wb base address

        u4WbAddress = (UINT32)DstAddr;//_GfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb color mode
        u4WbCm = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        switch (u4WbCm)     //image color mode
        {
            case 11:
            case 12:
            case 13:
                u4ValTmp = 2;
                u4WbColorWidth = 2;
                break;
            case 14:
                u4ValTmp = 4;
                u4WbColorWidth = 4;
                break;
            default:
                return -1;
        }
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb global alpha
        u4WbGlAlpha = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb width
        u4WbWidth = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4WbPitch = u4WbWidth * u4ValTmp;
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb height
        u4WbHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4WbAddress = %d  u4WbCm = %d  u4WbGlAlpha = %d u4WbWidth=%d  u4WbHeight =%d \n", u4WbAddress, u4WbCm, u4WbGlAlpha, u4WbWidth, u4WbHeight);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //BB  MIN x y
        u4BBStartX = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        u4BBStartY = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // BB width
        u4BBWidth = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // BB height
        u4BBHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4BBStartX =%d  u4BBStartY=%d  u4BBWidth=%d  u4BBHeight =%d \n", u4BBStartX, u4BBStartY, u4BBWidth, u4BBHeight);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // filter enable
        u4FilterEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	 _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // filter type
        u4FilterType = _VGfxGetVal(&u4CfgCount, pcFileInfo);	
		
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // anti aliasing enable
        u4AntiAliasEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //alpha edge mode
        u4AlphaEdge = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, " u4FilterEn = %d  u4AntiAliasEn=%d \n", u4FilterEn, u4AntiAliasEn);
         _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);   //  after alpha edge mode
	 
        LOG(10, " u4FilterEn = %d  u4AntiAliasEn=%d \n", u4FilterEn, u4AntiAliasEn);
	_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // alpha edge rgb0
        u4AlEdgeRGB0 = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // alpha edge threshold
        u4AlEdgeThresHold = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //reflection enable
	u4RefEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //reflection only
	u4RefOnly = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //reflection texture height 
	u4RefTextHeight = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //reflection ratio initial 
	u4RefRatioIntital = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //reflection ratio line step 
	u4RefRatioStep = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	_VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //reflection anti alias step 
	u4RefAAEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
	Printf( " u4FilterType = %d, u4AlEdgeRGB0 = %d, u4AlEdgeThresHold  = %d \n", 
	 	  u4FilterType, u4AlEdgeRGB0, u4AlEdgeThresHold);
      
	 Printf( " u4RefEn= %d, u4RefOnly= %d, u4RefTextHeight= %d, u4RefRatioIntital= %d, u4RefRatioStep= %d, u4RefAAEn = %d \n", 
	 	  u4RefEn, u4RefOnly, u4RefTextHeight, u4RefRatioIntital, u4RefRatioStep, u4RefAAEn);
       
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // alpha composition enable
        u4AlcomEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        LOG(10, "test  u4AlcomEn = %d, u4AlphaEdge= %d\n", u4AlcomEn, u4AlphaEdge);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // alpha com op mode
        u4AlcomOpMode = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // alpha com normal
        u4AlcomNormal = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // Ar
        u4AlcomAr = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // pre mult src read enable
        u4PreMultiSrcRdEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // pre mult dst read enable
        u4PreMultiDstRdEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // pre mult dst write enable
        u4PreMultiDstWrEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  // img read exp
        u4ImgeRdExp = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb read exp
        u4WbRdExp = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //wb write rounding
        u4WbWrRounding = _VGfxGetVal(&u4CfgCount, pcFileInfo);
        _VGfxSkipCfgData(&u4CfgCount, pcFileInfo);  //dump enable
        u4DumpEn = _VGfxGetVal(&u4CfgCount, pcFileInfo);
//#ifdef VGFX_DEBUG
        LOG(10, "test  u4AlcomEn =%d  %d  %d  %d  %d %d %d %d %d %d %d %d %d %d %d \n", u4AlcomEn, \
            u4AlcomOpMode, u4AlcomNormal, u4AlcomAr, u4PreMultiSrcRdEn, u4PreMultiDstRdEn, \
            u4PreMultiDstWrEn, u4ImgeRdExp, u4WbRdExp, u4WbWrRounding, u4DumpEn,  u4TransImg, u4TransWb, u4ImgPitch, u4WbPitch);
// #endif
        /***********************************************
        load raw data
        ************************************************/

        x_memset((void*)ImgAddr, 0, u4FrameSize);  // load img file to u4ImgAddress
        x_memset((void*)DstAddr, 0, u4FrameSize); // load wb file to u4WbAddress
        x_memset((void*)golden_dataY, 0, u4FrameSize);
        LOG(10, "//d.load.binary \"f:\\25dscaler\\random_vgfx_config\\random_image_%d.raw\" 0x%08x /ny\n", i,  u4ImgAddress);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\25dscaler\\random_vgfx_config\\random_image_%d.raw\" 0x%08x /ny", i, ImgAddr));
        LOG(10, "//d.load.binary \"f:\\25dscaler\\random_vgfx_config\\random_white_board_%d.raw\" 0x%08x /ny\n", i,  u4WbAddress);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\25dscaler\\random_vgfx_config\\random_white_board_%d.raw\" 0x%08x /ny", i, DstAddr));

        LOG(10, "//d.load.binary \"f:\\25dscaler\\random_vgfx_config\\white_board_o_%d.raw\" 0x%08x /ny\n", i, 	golden_dataY);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\25dscaler\\random_vgfx_config\\white_board_o_%d.raw\" 0x%08x /ny", i, golden_dataY));


        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"f:\\25dscaler\\random_vgfx_config\\xform_i_coeff_%d.txt\" 0x%08x /ny\n", i, pcFileInfo);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\25dscaler\\random_vgfx_config\\xform_i_coeff_%d.txt\" 0x%08x /ny", i, pcFileInfo));
        u4CfgCount = 0;
        for (j = 0;j < 9; j++)
        {
            _VGfxGetCoeff(&u4Matrix[j], &u4CfgCount, pcFileInfo);
            //Printf("debug ,u4Matrix[%d]= 0x%08x \n",i ,u4Matrix[i]);
        }

#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");
        _VGFX_SetPostWrEnv(u4PostWriteThreshold, u4WdleEn, u4WbWrRounding);
        _VGFX_SetQualityEnv(u4WbRdExp, u4ImgeRdExp, u4AntiAliasEn, u4FilterEn, u4AlphaEdge);
        //set img info
        _VGFX_SetImgage(u4ImgAddress, u4ImgCm, u4ImgGlAlpha, u4ImgPitch);

        _VGFX_SetTexture(u4TextStartX, u4TextStartY, u4TextEndX, u4TextEndY);
        // set wb info
        {
            UINT32 x1, y1, w, h;
            if (u4BBStartX >= (u4WbPitch / u4WbColorWidth))
            {
                Printf("the BB startx is not correctly\n");
                return -1;
            }
            else
            {
                x1 = u4BBStartX;
                if ((u4BBStartX + u4BBWidth) >= (u4WbPitch / u4WbColorWidth))
                {
                    w = u4WbPitch / u4WbColorWidth - x1;
                }
                else
                {
                    w = u4BBWidth;
                }
            }
            if (u4BBStartY >= u4WbHeight)
            {
                Printf("the BB starty is not correctly u4BBStartY = %d, u4WbHeight = %d\n", u4BBStartY, u4WbHeight);
                return -1;
            }
            else
            {
                y1 = u4BBStartY;
                if ((u4BBStartY + u4BBHeight) >= u4WbHeight)
                {
                    h = u4WbHeight - y1;
                }
                else
                {
                    h = u4BBHeight;
                }
            }
            _VGFX_SetBb(x1, y1, w, h);
        }
        _VGFX_SetWb(u4WbAddress, u4WbCm, u4WbGlAlpha, u4WbPitch);
        _VGFX_SetMatrixCoef_Emu(u4Matrix[0], u4Matrix[1], u4Matrix[2], u4Matrix[3], u4Matrix[4], u4Matrix[5], u4Matrix[6], u4Matrix[7], u4Matrix[8]);
        _VGFX_SetAlcom(u4AlcomEn);
      //   if (u4AlcomEn == TRUE)      //2.5d scaler need use premulty ,since u4AlcomEn is disable
        {
            //_GFX_SetPremult(u4PreMultiSrcRdEn, u4PreMultiDstRdEn, u4PreMultiDstWrEn, 1);
            _VGFX_GeneralAlphaComSet(u4ImgCm, u4WbCm, u4AlcomAr, u4AlcomOpMode, u4AlcomNormal, u4PreMultiSrcRdEn, u4PreMultiDstRdEn, u4PreMultiDstWrEn, 1);
        }

       _VGFX_SetFilterType(u4FilterType);
	_VGFX_SetAlphaEdgeRGB0(u4AlEdgeRGB0, u4AlEdgeThresHold);
	_VGFX_SetReflect( u4RefEn,  u4RefOnly, u4RefTextHeight, u4RefRatioIntital, u4RefRatioStep,  u4RefAAEn);
	
        _VGFX_Enable();
        GFX_Flush();
        GFX_Wait();

        u4GoldenSize = u4WbWidth * u4WbHeight * u4WbColorWidth;

        for (j = 0; j < u4GoldenSize ; j++)
        {
            if (DstAddr[j] != golden_dataY[j])
            {
                Printf("hw : %x, golden : %x, j = %d\n", DstAddr[j], golden_dataY[j], j);
                Printf("compare error in %d times file_name=%s\n", i, bVfgFileName);
                return 1;
                //break;
            }
            else
            {
                //Printf("compare OK in %d times file_name=%s\n", i, bVfgFileName);
            }
        }
        Printf("compare OK in %d times file_name=%s\n", i, bVfgFileName);

    }
    Printf("2.5d gfx scaler random pattern auto test OK......");
    return 0;

}


#endif

//-------------------------------------------------------------------------
/** _GfxEmuCmdCompress1
 *  Compression BitBlt Test 2 / new compression operation
 */
//-------------------------------------------------------------------------
static INT32 _GfxEmuCmdCompressMF(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height;
    UINT32 u4RollBackEn, u4QualityMode, u4BpCompAddrEnd, u4ColorMode;
    UINT32 u4SrcAddr, u4DstAddr, u4DstAddr1, u4SrcPitch, u4DstPitch=0;
    BOOL fgBurstRead;
    char* pcFileInfo;
    UINT32 i,u4Times,j;
    char* pcToken;
    UINT32 file_size1,file_size2,u4BytesPixel=1;
    UINT32 FileSize;
    UINT32 u4InitValue =0;
    UINT32 index =0;
    //UINT32 u4RepeatFrame =9999;
    UINT8* SrcAddr = NULL;
    UINT8* DstAddr = NULL;
    UINT8* DstAddr1 = NULL;
    UINT8* TmpAddr = NULL;

    UINT8* hw_address = NULL;
    UINT8* hw_address1 = NULL;
    //BOOL fgFirstTime = TRUE;
    UINT32 u4FrameSize = 1920*1200*4;

    DstAddr = (UINT8*)_u4GfxEmuBuf[0];
    DstAddr1 = DstAddr + u4FrameSize;
    SrcAddr = DstAddr1 + u4FrameSize;
    TmpAddr = SrcAddr + u4FrameSize;
Printf("(0x%x, 0x%x, 0x%x, 0x%x)\n", SrcAddr, DstAddr, TmpAddr,DstAddr1);

    hw_address = DstAddr;
    hw_address1 = DstAddr1;
    
    Printf("Compression BitBlt Test 2 ...");

    u4InitValue =  (UINT32) StrToInt(szArgv[1]);
    u4Times =  (UINT32) StrToInt(szArgv[2]);
    //u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);
    
    pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);

    for(i=u4InitValue ; i<u4Times ; i++)
    {
        int line_separate =0;
        //if(i == (u4RepeatFrame+1))
        //    i--;
        UINT32 file_data[10] = {0};

        index =0;
        
        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(0, "//d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_info_%d.txt\" 0x%08x /ny\n",i,pcFileInfo);
	    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_info_%d.txt\" 0x%08x /ny", 
		i,pcFileInfo));

        pcToken = strtok(pcFileInfo, " ");
        while ( pcToken != NULL)
        {
            file_data[index++] = StrToInt(pcToken);
            pcToken = strtok(NULL, " ");
        }

        u4SrcAddr     = (UINT32) SrcAddr;//(UINT32) 0x01ea4000;
        u4DstAddr     = (UINT32) DstAddr;
        u4DstAddr1     = (UINT32) DstAddr1;        
        u4Width       = (UINT32) file_data[0];
        u4Height      = (UINT32) file_data[1];
        u4ColorMode = (UINT32) file_data[2];
        u4QualityMode = (UINT32) file_data[3];
        u4RollBackEn  = (UINT32) file_data[4];
        file_size1 = (UINT32) file_data[5];
        fgBurstRead = (BOOL) file_data[6];
        file_size2 = (UINT32) file_data[7];
        u4BpCompAddrEnd = 0xffffffff;
        u4SrcPitch =0;

        
        if (u4ColorMode == 2 || u4ColorMode == 10)
        {
            u4BytesPixel = 1;
        }
        else if (u4ColorMode == 6 || u4ColorMode == 14)
        {
            u4BytesPixel = 4; 
        }
        else if (u4ColorMode == 13 || u4ColorMode == 12 || u4ColorMode == 11)
        {
            u4BytesPixel = 2;
        }
        else
        {
            ASSERT(0);
        }
        
        u4SrcPitch    = u4Width * u4BytesPixel;

        
        //if(fgFirstTime) {
            LOG(0, "//d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_%d.txt\" 0x%08x /ny\n",i,u4SrcAddr);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_%d.txt\" 0x%08x /ny", 
    		(i),u4SrcAddr));
 
        Printf("Src = 0x%08x, sPitch = %d, Dst = 0x%08x, dPitch = %d, W = %d, H = %d, u4RollBackEn = %d,u4QualityMode = %d, u4BpCompAddrEnd = 0x%08x, u4ColorMode = %d fgBurstRead = %d\n",
            u4SrcAddr, u4SrcPitch, u4DstAddr, u4DstPitch, u4Width, u4Height, u4RollBackEn,u4QualityMode,u4BpCompAddrEnd,u4ColorMode,fgBurstRead);

#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");


        for(line_separate =1; line_separate<2 ;line_separate++)
        {
            printf("\nclear dst buffer\n");
            _GFX_SetDst((UINT8 *)u4DstAddr, 14, 1920*4);
            _GFX_SetColor(0x0);
            _GFX_Fill(0, 0, 1920, 1200);
            _CLI_CMD("gfx.flush");
            _GFX_SetDst((UINT8 *)u4DstAddr1, 14, 1920*4);
            _GFX_SetColor(0x0);
            _GFX_Fill(0, 0, 1920, 1200);
            _CLI_CMD("gfx.flush");
            
            printf("\nclear dst buffer\n");
            
            //line-separate mode
            if(line_separate == 1)
            {
                UINT32 original_line_bit = u4Width * u4BytesPixel *8;
                
                UINT32 line_bit;
                if(u4RollBackEn)
                    line_bit = original_line_bit+4;
                else
                    line_bit = original_line_bit + 4 + original_line_bit/16;
                if(fgBurstRead)
                    line_bit = (line_bit%512 ==0)? line_bit : (line_bit/512 +1)*512;
                else
                    line_bit = (line_bit%128 ==0)? line_bit : (line_bit/128 +1)*128;

                u4DstPitch = line_bit/8;
  
                
                printf("width : %d\n",u4Width);
                
                FileSize = file_size2;
            }
            else
            {
                u4DstPitch = u4SrcPitch;
                FileSize = file_size1;
            }

#if 1
            _GFX_SetSrc((UINT8 *)u4SrcAddr, u4ColorMode, u4SrcPitch);
            _GFX_SetDst((UINT8 *)TmpAddr, u4ColorMode, u4SrcPitch);
             _CLI_CMD("gfx.set.bltopt 0x0 0xffffffff flip mirror");
             GFX_BitBlt(0, 0, 0, 0, u4Width, u4Height);
            
            _CLI_CMD("gfx.flush");

            _GFX_NewCompressBlt((UINT8 *)TmpAddr, 0, 0, u4ColorMode, u4SrcPitch, 
                (UINT8 *)u4DstAddr1, u4Width, u4Height, u4Height,  u4RollBackEn,u4QualityMode,u4BpCompAddrEnd, FALSE ,fgBurstRead,line_separate,u4DstPitch);

            _CLI_CMD("gfx.flush");

#endif            

            _GFX_NewCompressBlt((UINT8 *)u4SrcAddr, 0, 0,  u4ColorMode, u4SrcPitch, 
                (UINT8 *)u4DstAddr, u4Width, u4Height, u4Height, u4RollBackEn,u4QualityMode,u4BpCompAddrEnd,TRUE,fgBurstRead,line_separate,u4DstPitch);

            _CLI_CMD("gfx.flush");


            //LOG(0,"file_size : %d\n",file_size);
            HalFlushInvalidateDCache();
            for (j = 0; j < FileSize ; j++)
            {
                if (hw_address[j] != hw_address1[j])
                {
                    printf("hw : %x, golden : %x, j = %d\n",hw_address[j],hw_address1[j],j);
                    printf("compare error in emu_source_%d.txt %d\n",i,line_separate);
                    printf("BPCOMP_CHKSUM : %x\n",IO_READ32(GFX_ADDR, (0xb8)));
                    printf("BPCOMP_NIPPLE : %x\n",IO_READ32(GFX_ADDR, (0xb4)));
                    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\emu_source_%d_error_%d.txt\" 0x%08x--0x%08x", 
        		(i),(line_separate),(UINT32)hw_address,(UINT32)hw_address+FileSize-1));
                    return 1;
                    //break;
                }
                else
                {
                    //LOG(0,"hw : %x , golden : %x\n",hw_address[j],golden_dataY[j]);
                }
            }
        }
        printf("compare ok in emu_source_%d.txt\n\n",i);
        //fgFirstTime = FALSE;
    }

    return 0;
}


static void _GfxEmuCreateBuffers(void)
{
    UINT32 u4Size = (_u4CanvasWidth * _u4ScreenHeight * 4);
    UINT32 i, u4BufNum = 10;
    
    _u4FbmStartAddr = DRVCUST_OptGet(eFbmMemAddr);
    _u4FbmTotalSize = DRVCUST_OptGet(eFbmMemSize);

    _u4FbmStartAddr = ((_u4FbmStartAddr >> 12) + 1) << 12;    

    Printf("----------------------------------------------------------\n");
    Printf("FBM start addr = 0x%08x\n", _u4FbmStartAddr);
    Printf("FBM total size = %u (MB)\n", (_u4FbmTotalSize / 1048576));

    for (i = 0; i < u4BufNum; i++)
    {
        #ifdef GFX_BURST_READ
            _u4GfxEmuBuf[i] = GFX_BURST_READ_ALIGN(_u4FbmStartAddr + (i * u4Size));
        #else
            _u4GfxEmuBuf[i] = _u4FbmStartAddr + (i * u4Size);
        #endif
    }
    #ifdef GFX_BURST_READ
        _u4GfxEmuBufAlphaMap  = GFX_BURST_READ_ALIGN(_u4FbmStartAddr + ((i)   * u4Size));
        _u4GfxEmuBufLuma      = GFX_BURST_READ_ALIGN(_u4FbmStartAddr + ((i+1) * u4Size));
        _u4GfxEmuBufDstLuma   = GFX_BURST_READ_ALIGN(_u4FbmStartAddr + ((i+2) * u4Size));
        _u4GfxEmuBufChroma    = GFX_BURST_READ_ALIGN(_u4FbmStartAddr + ((i+3) * u4Size));
        _u4GfxEmuBufDstChroma = GFX_BURST_READ_ALIGN(_u4FbmStartAddr + ((i+4) * u4Size));
    #else
        _u4GfxEmuBufAlphaMap  = _u4FbmStartAddr + ((i)   * u4Size);
        _u4GfxEmuBufLuma      = _u4FbmStartAddr + ((i+1) * u4Size);
        _u4GfxEmuBufDstLuma   = _u4FbmStartAddr + ((i+2) * u4Size);
        _u4GfxEmuBufChroma    = _u4FbmStartAddr + ((i+3) * u4Size);
        _u4GfxEmuBufDstChroma = _u4FbmStartAddr + ((i+4) * u4Size);
    #endif


    Printf("Screen buf addr = 0x%08x\n", BUF_SCREEN);

    Printf("Compare buf addr = 0x%08x\n", COMPARE_BUF);

    Printf("Gfx buffer addr 1 = 0x%08x\n", BUF_ADDR1);

    Printf("Gfx buffer addr 2 = 0x%08x\n", BUF_ADDR2);

    Printf("Gfx buffer addr 3 = 0x%08x\n", BUF_ADDR3);

    Printf("Gfx buffer addr 4 = 0x%08x\n", BUF_ADDR4);

    Printf("Gfx buffer temp = 0x%08x\n", BUF_TEMP);

    Printf("Gfx buffer src-temp = 0x%08x\n", _u4GfxEmuBuf[7]);

    Printf("Gfx buffer src-cutted = 0x%08x\n", _u4GfxEmuBuf[8]);
    
    Printf("Gfx buffer dst-temp = 0x%08x\n", _u4GfxEmuBuf[9]);

    Printf("Gfx buffer alpha-map = 0x%08x\n", _u4GfxEmuBufAlphaMap);
    
    Printf("Gfx buffer luma = 0x%08x\n", _u4GfxEmuBufLuma);

    Printf("Gfx buffer dst-luma = 0x%08x\n", _u4GfxEmuBufDstLuma);

    Printf("Gfx buffer chroma = 0x%08x\n", _u4GfxEmuBufChroma);

    Printf("Gfx buffer dst-chroma = 0x%08x\n", _u4GfxEmuBufDstChroma);

    Printf("----------------------------------------------------------\n");

Printf("d.load.binary D:/MT5381_Emu/BBC_MUX_603.584784468_0.src 0x%08x\n", BUF_ADDR4);

Printf("d.load.binary D:/MT5381_Emu/kodak14.8888 0x%08x\n", BUF_ADDR1);

Printf("d.load.binary D:/MT5381_Emu/kodak16.8888 0x%08x\n", BUF_ADDR2);

Printf("d.load.binary D:/MT5381_Emu/kodak17.8888 0x%08x\n", BUF_ADDR3);

Printf("d.load.binary D:/MT5381_Emu/luma_768x480_mb.bin 0x%08x\n", BUF_LUMA);

Printf("d.load.binary D:/MT5381_Emu/chroma_768x240_mb.bin 0x%08x\n", BUF_CHROMA);

Printf("d.load.binary D:/MT5381_Emu/alphamap720x480.bin 0x%08x\n", BUF_ALPHAMAP);

    Printf("----------------------------------------------------------\n");
}


static INT32 _GfxEmuCmdAutoRunning(INT32 i4Argc, const CHAR ** szArgv)
{

	//_CLI_CMD("gfxemu.gencompose 1 500 0"); //new alpha composition

	_CLI_CMD("gfxemu.gfxemu.idx2dir5 50"); //premultiplied operations  ok
	_CLI_CMD("gfxemu.gfxemu.idx2diralcom 50"); //1-pass rectangle index to direct bitblt with pitch+alpha composition

_CLI_CMD("gfxemu.onepasscomp 1 2000 0"); //one pass compression
_CLI_CMD("gfxemu.compress2 2 28 0"); //osd compression

return 0;
}


#if defined(CC_MT5398)
//ROP MODE 0x2 TEST for 5398
static INT32 _GfxEmuCmdRop6(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size = 0;
    INT32 i4PreCount = 0;
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count, i4RopCode;
    INT32 i4CountNum = 5000;
    INT32 i4Error = 0;

    Printf("ROP Test 0x2 ");

    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        i4RopCode = 2;
        i4Left    = ((UINT32)rand()) % _u4ScreenWidth;
        i4Right   = ((UINT32)rand()) % _u4ScreenWidth;
        i4Top     = ((UINT32)rand()) % _u4ScreenHeight;
        i4Bottom  = ((UINT32)rand()) % _u4ScreenHeight;

        i4Width   = i4Right - i4Left;
        if (i4Width < 0)
        {
            i4Width = -1 * i4Width;
            i4Left = i4Right;
        }

        i4Height = i4Bottom - i4Top;
        if (i4Height < 0)
        {
            i4Height = -1 * i4Height;
            i4Top = i4Bottom;
        }

        i4Width++;
        i4Height++;

        Printf("(%4d) : ", i4Count);
        Printf("L = %d, T = %d, W = %d, H = %d, RopCode = %d\n",
               i4Left, i4Top, i4Width, i4Height, i4RopCode);

        // HW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode");

        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _GfxSetDefMode(BUF_SCREEN, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

        _GFX_SetXORColor(0x12345678);
		_CLI_CMD("gfx.flush");
        // Action
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetDefMode(BUF_SCREEN, ADR_DST);
        _CLI_CMD("gfx.set.ropcode %d", i4RopCode);
        _CLI_CMD("gfx.op.rop %d %d %d %d %d %d",
                 i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");


        // SW
#ifdef GFX_HW_RESET
        _CLI_CMD("gfx.reset");
#endif
        _CLI_CMD("gfxemu.mode sw");

        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _GfxSetDefMode(COMPARE_BUF, ADR_SRC);
        _CLI_CMD("gfxemu.cls");
        _GFX_SetXORColor(0x12345678);
		_CLI_CMD("gfx.flush");
        // Action
        _GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);
        _CLI_CMD("gfx.set.ropcode %d", i4RopCode);
        _CLI_CMD("gfx.op.rop %d %d %d %d %d %d",
                 i4Left, i4Top, i4Left, i4Top, i4Width, i4Height);
        _CLI_CMD("gfx.flush");

        // Compare HW and SW
        u4Size = _u4ScreenWidth * _u4ScreenHeight;
        if (_GfxBufferCompare(
                    (UINT32 *)COMPARE_BUF,
                    (UINT32 *)(BUF_SCREEN), u4Size) == -1)
        {
            Printf("ROP Test 04 Error ");
            Printf("(%4d) : Diff = %d, (Error = %d)\n",
                   i4Count, (i4Count - i4PreCount), ++i4Error);
            i4PreCount = i4Count;
            return -1;
        }
    }
    Printf("# of Error Run = %d, # of Total Run = %d\n", i4Error, i4CountNum);
    return 0;
}

//one pass colorize + alpha composition
static INT32 _GfxEmuCmdClrRzAlcom(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Sx, u4Sy, u4Sw, u4Sh, u4Dx, u4Dy, u4Dw, u4Dh, u4Size;
    UINT32 u4Loop, u4CountNum, u4Opcode,j,u4Ar;
    UINT8* pucTwoPass;
    UINT8* pucOnePass;
	UINT32 u4XORColor;
 
 	Printf("d.load.binary D:/MT5381_Emu/kodak16.8888 0x%08x /ny\n", BUF_ADDR1);
 	Printf("d.load.binary D:/MT5381_Emu/kodak16.8888 0x%08x /ny\n", BUF_ADDR3);
 	Printf("d.load.binary D:/MT5381_Emu/kodak16.8888 0x%08x /ny\n", BUF_SCREEN);
 	LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny\n", BUF_ADDR1);
 	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny", BUF_ADDR1));
 	LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny\n", BUF_ADDR3);
 	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny", BUF_ADDR3));
 	LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny\n", BUF_SCREEN);
 	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny", BUF_SCREEN));


    if (i4Argc > 1)
    {
        u4CountNum = StrToInt(szArgv[1]);  
        u4Opcode = ( (UINT32)rand() ) % ( ( (UINT32)E_AC_XOR ) + 1); 
        if ((((UINT32)rand()) % 4) == 0)
        {
            u4Ar = 255;            
        }
        else
        {
            u4Ar = ((UINT32)rand()) % 256;
        }
    }
    else
    {
        u4CountNum = 1;
        u4Opcode = 0x7;    
    }
		   
	
	#ifdef GFX_HW_RESET
	_CLI_CMD("gfx.reset");
	#endif
	_CLI_CMD("gfxemu.mode");
		
	// init
	_CLI_CMD("gfx.set.bltopt 0 0 0");
	
	u4XORColor = 0xff123456;
	GFX_SetXORColor(u4XORColor);
	
	// do many in loop
	for (u4Loop = 0; u4Loop < u4CountNum; u4Loop++)
	{
		UINT32 u4Max, u4Min;
	
		// select source w,h
		u4Sw = (((UINT32)rand()) % (MAX_SRC_WIDTH - MIN_SRC_WIDTH - 1)) + MIN_SRC_WIDTH;
		u4Sh = (((UINT32)rand()) % (MAX_SRC_HEIGHT - MIN_SRC_HEIGHT - 1)) + MIN_SRC_HEIGHT;
		if (u4Sw == 0)
		{
			u4Sw++;
		}
		if (u4Sh == 0)
		{
			u4Sh++;
		}
	
		// select dest w
		u4Max = ((u4Sw * D_SCALE_UP) > (MAX_DST_WIDTH - 1)) ? (MAX_DST_WIDTH - 1) : (u4Sw * D_X_SCALE_UP);
		u4Min = ((u4Sw / D_SCALE_DN) < MIN_DST_WIDTH) ? MIN_DST_WIDTH : (u4Sw / D_X_SCALE_DN);
		u4Dw = (((UINT32)rand()) % (u4Max - u4Min + 1)) + u4Min;
	
		// select dest h
		u4Max = ((u4Sh * D_SCALE_UP) > (MAX_DST_HEIGHT - 1)) ? (MAX_DST_HEIGHT - 1) : (u4Sh * D_Y_SCALE_UP);
		u4Min = ((u4Sh / D_SCALE_DN) < MIN_DST_HEIGHT) ? MIN_DST_HEIGHT : (u4Sh / D_Y_SCALE_DN);
		u4Dh = (((UINT32)rand()) % (u4Max - u4Min + 1)) + u4Min;
	
		// select source x,y
		u4Sx = 0;
		u4Sy = 0;
			
		// select dest x,y
		u4Dx = 0;
		u4Dy = 0;
	
#if 0
		   //for test
		   u4Sw = 149;
		   u4Sh = 167;
		   u4Dw = 516;
		   u4Dh = 91;
#endif       
		//multipass/////////////////////////////////////////////////////////////
		_GfxSetDefMode(BUF_ADDR2, ADR_DST);
        _GfxSetDefMode(BUF_ADDR2, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

		_GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetDefMode(BUF_ADDR2, ADR_DST);
        _CLI_CMD("gfx.set.ropcode %d", 0x2);
        _CLI_CMD("gfx.op.rop %d %d %d %d %d %d",
                 u4Sx, u4Sy, u4Dx, u4Dy, u4Dw, u4Dh);
        _CLI_CMD("gfx.flush");
		
        _GfxSetSdMode(BUF_ADDR2, ADR_SRC);
        _GfxSetDefMode(BUF_ADDR3, ADR_DST);        

		GFX_Set_PRE_COLORIZE(FALSE);
		_GFX_GeneralAlphaCom(u4Dw, u4Dh, u4Ar, u4Opcode, 0, 0, 1, FALSE);	
		_CLI_CMD("gfx.flush");
		///////////////////////////////////////////////////////////////////
		
		//one pass////////////////////////////////////////////////////////////
		_GfxSetSdMode(BUF_ADDR1, ADR_SRC);
		_GfxSetDefMode(BUF_SCREEN, ADR_DST);
			
		GFX_Set_PRE_COLORIZE(TRUE);
		_GFX_GeneralAlphaCom(u4Dw, u4Dh, u4Ar, u4Opcode, 0, 0, 1, FALSE);		   
	
		_CLI_CMD("gfx.flush");
		////////////////////////////////////////////////////////////////////
		pucOnePass = (UINT8*)BUF_SCREEN;
		pucTwoPass = (UINT8*)BUF_ADDR3;
		u4Size = (u4Dw*u4Dh);
			
		for(j=0; j<u4Size ; j++) 
		{
			if (pucOnePass[j] != pucTwoPass[j] )
			{
				printf("1Pass : %x, 2Pass : %x, j = %d\n", pucOnePass[j], pucTwoPass[j], j);
				printf("Stretch alpha composition comparing error  (%d) \n");
				return 1;
			}
		}  
		/*

		if (_GfxMemCompare(
				(UINT8 *)pucOnePass,
						(UINT8 *)pucTwoPass, u4Size) == -1)
		{
				printf("Colorize alpha composition comparing error (%d) \n", u4Loop);	
				return -1;
		}*/

		printf("Colorize alpha composition comparing ok (%d) \n", u4Loop);		   
	}	
	
		return 0;

}



static INT32 _GfxEmuCmdSrcAlphaReplace(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Sw, u4Sh, u4Dw, u4Dh, u4Size;
    UINT32 u4Loop, u4CountNum, u4Opcode,j,u4Ar;
    UINT8* pucTwoPass;
    UINT8* pucOnePass;
	UINT32 u4XORColor;
 
 	Printf("d.load.binary D:/MT5381_Emu/kodak16.8888 0x%08x /ny\n", BUF_ADDR1);
 	Printf("d.load.binary D:/MT5381_Emu/kodak16.8888 0x%08x /ny\n", COMPARE_BUF);
 	Printf("d.load.binary D:/MT5381_Emu/kodak16.8888 0x%08x /ny\n", BUF_SCREEN);
 	LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny\n", BUF_ADDR1);
 	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny", BUF_ADDR1));
 	LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny\n", COMPARE_BUF);
 	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny", COMPARE_BUF));
 	LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny\n", BUF_SCREEN);
 	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny", BUF_SCREEN));


    if (i4Argc > 1)
    {
        u4CountNum = StrToInt(szArgv[1]);  
        u4Opcode = ( (UINT32)rand() ) % ( ( (UINT32)E_AC_XOR ) + 1); 
    }
    else
    {
        u4CountNum = 1;
        u4Opcode = 0x7;    
    }
	u4Ar = ((UINT32)rand()) % 256;	   
	
	#ifdef GFX_HW_RESET
	_CLI_CMD("gfx.reset");
	#endif
	_CLI_CMD("gfxemu.mode");
		
	// init
	_CLI_CMD("gfx.set.bltopt 0 0 0");
	
	u4XORColor = 0xff123456;
	GFX_SetXORColor(u4XORColor);
	_CLI_CMD("gfx.flush");
	// do many in loop
	for (u4Loop = 0; u4Loop < u4CountNum; u4Loop++)
	{
	
	
		// select source w,h
		u4Sw = (((UINT32)rand()) % (MAX_SRC_WIDTH - MIN_SRC_WIDTH - 1)) + MIN_SRC_WIDTH;
		u4Sh = (((UINT32)rand()) % (MAX_SRC_HEIGHT - MIN_SRC_HEIGHT - 1)) + MIN_SRC_HEIGHT;
		if (u4Sw == 0)
		{
			u4Sw++;
		}
		if (u4Sh == 0)
		{
			u4Sh++;
		}
	
		// select dest w
		u4Dw = u4Sw;
	
		// select dest h
		u4Dh = u4Sh;
	
		// select source x,y
		//u4Sx = 0;
		//u4Sy = 0;
			
		// select dest x,y
		//u4Dx = 0;
		//u4Dy = 0;
	
#if 0
		   //for test
		   u4Sw = 149;
		   u4Sh = 167;
		   u4Dw = 516;
		   u4Dh = 91;
#endif       
		//multipass/////////////////////////////////////////////////////
		//step1:create alpha map
		_CLI_CMD("gfxemu.mode cm rgb8");
		_GfxSetDefMode(BUF_ADDR2, ADR_DST);
        _GfxSetDefMode(BUF_ADDR2, ADR_SRC);
        _CLI_CMD("gfxemu.cls");
		_CLI_CMD("gfx.set.color 0x%08x", u4Ar);
		_CLI_CMD("gfx.op.rect  0  0 %d %d",_u4ScreenWidth,_u4ScreenHeight);
		_CLI_CMD("gfx.flush");

		//step2:move src to temp
		_CLI_CMD("gfxemu.mode cm argb8888");
		_GfxSetDefMode(BUF_ADDR3, ADR_DST);
        _GfxSetDefMode(BUF_ADDR3, ADR_SRC);
        _CLI_CMD("gfxemu.cls");

		_CLI_CMD("gfxemu.mode cm argb8888");
		_GfxSetSdMode(BUF_ADDR1, ADR_SRC);
        _GfxSetDefMode(BUF_ADDR3, ADR_DST);
        _CLI_CMD("gfx.set.bltopt 0 0 0");
    	_CLI_CMD("gfx.op.bitblt 0 0 0 0 %d %d",_u4ScreenWidth,_u4ScreenHeight);
    	_CLI_CMD("gfx.flush");

		//step3:AlphaMap act
		_CLI_CMD("gfxemu.mode cm rgb8");
        _GfxSetDefMode(BUF_ADDR2, ADR_SRC);
		_CLI_CMD("gfxemu.mode cm argb8888");
		_GfxSetDefMode(BUF_ADDR3, ADR_DST);
		_CLI_CMD("gfx.op.am %d %d %d %d %d %d", 0, 0, 0, 0, _u4ScreenWidth, _u4ScreenHeight);
        _CLI_CMD("gfx.flush");

		//setp4:alpha composition
		_CLI_CMD("gfxemu.mode cm argb8888");
        _GfxSetSdMode(BUF_ADDR3, ADR_SRC);
        _GfxSetDefMode(COMPARE_BUF, ADR_DST);        
		_GFX_GeneralAlphaCom(u4Dw, u4Dh, u4Ar, u4Opcode, 0, 0, 0, FALSE);	
		_CLI_CMD("gfx.flush");
		///////////////////////////////////////////////////////////////////////////
		
		//one pass//////////////////////////////////////////////////////////////
		_CLI_CMD("gfxemu.mode cm argb8888");
		_GfxSetSdMode(BUF_ADDR1, ADR_SRC);
		_GfxSetDefMode(BUF_SCREEN, ADR_DST);
			
		GFX_Set_SrcAlReplace((UINT32) TRUE);
		_GFX_GeneralAlphaCom(u4Dw, u4Dh, u4Ar, u4Opcode, 0, 0, 0, FALSE);		   
	
		_CLI_CMD("gfx.flush");
		////////////////////////////////////////////////////////////////////////
		pucOnePass = (UINT8*)BUF_SCREEN;
		pucTwoPass = (UINT8*)COMPARE_BUF;
		u4Size = (u4Dw*u4Dh);
		
		for(j=0; j<u4Size ; j++) 
		{
			if (pucOnePass[j] != pucTwoPass[j] )
			{
				printf("1Pass : %x, 2Pass : %x, j = %d\n", pucOnePass[j], pucTwoPass[j], j);
				printf("Stretch alpha composition comparing error  (%d) \n");
				return 1;
			}
		}
		printf("Stretch alpha composition comparing ok (%d) \n", u4Loop);		   
	}	
	
		return 0;

}

//one pass to using RectColor as rect src
static INT32 _GfxEmuCmdSrcRectColor(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Sx, u4Sy, u4Sw, u4Sh, u4Dx, u4Dy, u4Dw, u4Dh, u4Size;
    UINT32 i4RopCode, j;
    UINT8* pucTwoPass;
    UINT8* pucOnePass;
	UINT32 u4RectColor;

	u4Sx = 0;
	u4Sy = 0;
	u4Dx = 0;
	u4Dy = 0;
/*
	u4Sw = _u4ScreenWidth;
	u4Sh = _u4ScreenHeight;
	u4Dw = _u4ScreenWidth;
	u4Dh = _u4ScreenHeight;
*/
	u4Sw = (((UINT32)rand()) % (MAX_SRC_WIDTH - MIN_SRC_WIDTH - 1)) + MIN_SRC_WIDTH;
	u4Sh = (((UINT32)rand()) % (MAX_SRC_HEIGHT - MIN_SRC_HEIGHT - 1)) + MIN_SRC_HEIGHT;
	if (u4Sw == 0)
	{
		u4Sw++;
	}
	if (u4Sh == 0)
	{
		u4Sh++;
	}
	
	// select dest w
	u4Dw = u4Sw;
	
	// select dest h
	u4Dh = u4Sh;

	i4RopCode = 0x6;
 	//create test pattern
	_CLI_CMD("gfxemu.mode cm argb8888");
	_GfxSetDefMode(BUF_SCREEN, ADR_DST);
	_GfxSetDefMode(BUF_SCREEN, ADR_SRC);
	_CLI_CMD("gfxemu.cls");
	_GfxSetDefMode(COMPARE_BUF, ADR_DST);
	_GfxSetDefMode(COMPARE_BUF, ADR_SRC);
	_CLI_CMD("gfxemu.cls");
	_CLI_CMD("gfx.flush");
	
 	Printf("d.load.binary D:/MT5381_Emu/kodak16.8888 0x%08x /ny\n", COMPARE_BUF);
 	Printf("d.load.binary D:/MT5381_Emu/kodak16.8888 0x%08x /ny\n", BUF_SCREEN);

 	LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny\n", COMPARE_BUF);
 	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny", COMPARE_BUF));
 	LOG(10, "//d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny\n", BUF_SCREEN);
 	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\MT5381_Emu\\kodak16.8888\" 0x%08x /ny", BUF_SCREEN));

	
	_GfxSetDefMode(BUF_TEMP, ADR_DST);
	_GfxSetDefMode(BUF_TEMP, ADR_SRC);
	_CLI_CMD("gfxemu.color 0xFF4f7b3c");
    _CLI_CMD("gfx.op.rect  %d %d %d %d",u4Sx,u4Sy,u4Sw,u4Sh);
    _CLI_CMD("gfx.flush");
	
	_GfxSetDefMode(BUF_ADDR1, ADR_DST);
	_GfxSetDefMode(BUF_ADDR1, ADR_SRC);
	_CLI_CMD("gfxemu.color 0xFFFFFFFF");
    _CLI_CMD("gfx.op.rect  %d %d %d %d",u4Sx,u4Sy,u4Sw,u4Sh);
    _CLI_CMD("gfx.flush");
	

	u4RectColor = 0xFF4f7b3c;

	//one pass////////////////////////////////////////////////////////
	_GfxSetSdMode(BUF_ADDR1, ADR_SRC);
	_GfxSetDefMode(BUF_SCREEN, ADR_DST);
	_GFX_SetColor(u4RectColor);
	_CLI_CMD("gfx.set.ropcode %d", i4RopCode);
	_GFX_Set_UseRectSrc(TRUE);
    _CLI_CMD("gfx.op.rop %d %d %d %d %d %d", 
            u4Sx, u4Sy, u4Dx, u4Dy, u4Dw, u4Dh);
    _CLI_CMD("gfx.flush");	
	///////////////////////////////////////////////////////
	_GfxSetSdMode(BUF_TEMP, ADR_SRC);
	_GfxSetDefMode(COMPARE_BUF, ADR_DST);

	_GFX_Set_UseRectSrc(FALSE);
	_CLI_CMD("gfx.set.ropcode %d", i4RopCode);
    _CLI_CMD("gfx.op.rop %d %d %d %d %d %d", 
            u4Sx, u4Sy, u4Dx, u4Dy, u4Dw, u4Dh);
    _CLI_CMD("gfx.flush");	
	pucOnePass = (UINT8*)BUF_SCREEN;
	pucTwoPass = (UINT8*)COMPARE_BUF;
		u4Size = (u4Dw*u4Dh);
			
		for(j=0; j<u4Size ; j++) 
		{
			if (pucOnePass[j] != pucTwoPass[j] )
			{
				printf("1Pass : %x, 2Pass : %x, j = %d\n", pucOnePass[j], pucTwoPass[j], j);
				printf("SrcRectColor comparing error  \n");
				return 1;
			}
		}
		printf("SrcRectColor comparing ok \n");	
		return 0;

}

static INT32 _GfxEmuCmdFlashFormatConv(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4Type,u4Width,u4Height,u4loop,u4Pitch,u4Size,u4CompSize;
	UINT8 * puDstAress = (UINT8 *)BUF_TEMP;
	UINT8 * puGolden = (UINT8 *)COMPARE_BUF;
	
    Printf("FlashFormatConv test\n");
	UINT32 ToARGBSize[8] = {37120,37584,38048,38512,38976,39440,39904,40268};
	UINT32 ToRasterSize[8] = {13920,14094,14268,14442,14616,14790,14964,15138};
    
    if (i4Argc < 2)
    {
        Printf("argv: u4Type 1 - 3, FlashFormatConv\n");
        return -1;
    }

    u4Type =  (UINT32) StrToInt(szArgv[1]);
	u4Height = 58;
	u4Pitch = 768;

 	Printf("d.load.binary D:/gfx_emulation/Cobra_Android_pattern/read_Y.raw 0x%08x /ny\n", BUF_SCREEN);
	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\Cobra_Android_pattern\\read_Y.raw\" 0x%08x /ny", BUF_SCREEN));
	Printf("d.load.binary D:/gfx_emulation/Cobra_Android_pattern/read_CbCr.raw 0x%08x /ny\n", BUF_ADDR1);
	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\Cobra_Android_pattern\\read_CbCr.raw\" 0x%08x /ny",BUF_ADDR1));

	for(u4loop = 0 ; u4loop < 8 ; u4loop++)
	{
		u4Width = 160+u4loop*2;
		puDstAress = (UINT8 *)BUF_TEMP;
		puGolden = (UINT8 *)COMPARE_BUF;		
		
		Printf("d.load.binary D:/gfx_emulation/Cobra_Android_pattern/android_type%d_width%d_golden.raw 0x%08x /ny\n",(u4Type),(u4loop*2), COMPARE_BUF);
		UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\Cobra_Android_pattern\\android_type%d_width%d_golden.raw\" 0x%08x /ny",(u4Type),(u4loop*2), COMPARE_BUF));

		switch(u4Type)
		{
			case 1:
				GFX_Set_FlashColCnvInfo((UINT8 *)BUF_SCREEN, u4Pitch, (UINT8 *)BUF_ADDR1, 
						u4Pitch,( UINT8 * )BUF_TEMP,u4Width,u4Height);
				GFX_YCbCr420ToARGB32(0xFF,0);
				u4CompSize = ToARGBSize[u4loop];
				break;
			case 2:
				GFX_YCbCrBlockToRaster_2Buf((UINT8 *)BUF_SCREEN, u4Pitch, (UINT8 *)BUF_ADDR1, 
						u4Pitch,( UINT8 * )BUF_TEMP,u4Width,u4Height);
				u4CompSize = ToRasterSize[u4loop];
				break;
			case 3:
				GFX_YCbCrBlockToRaster_3Buf((UINT8 *)BUF_SCREEN, u4Pitch, (UINT8 *)BUF_ADDR1, 
						u4Pitch,( UINT8 * )BUF_TEMP,u4Width,u4Height);
				u4CompSize = ToRasterSize[u4loop];
				break;
			default:
				printf("u4Type is not support\n");
				return -1;
		}

		for(u4Size = 0; u4Size < u4CompSize ; u4Size++) 
		{
			if (puDstAress[u4Size] != puGolden[u4Size] )
			{
				printf("puDstAress : %x, puGolden : %x, u4Size = %d\n", puDstAress[u4Size], puGolden[u4Size], u4Size);
				printf("FlashFormatConv test error : type [%d] width [%d] \n",(u4Type),(u4loop*2));
				return 1;
			}
		}  
	}
	printf("FlashFormatConv test ok \n");
	return 0;
}

//test different mid in same cmdque
static INT32 _GfxEmuIOMMUAlcomCompress2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height;
    UINT32 u4SrcAddr, u4DstAddr, u4SrcPitch, u4DstPitch, u4ThirdAddr, u4ThirdPitch;
    UINT32 u4SrcCM, u4DstCM, u4ThirdCM;
    char* pcFileInfo;
    UINT32 i, u4Times, j;
    char* pcToken;
    UINT32 u4InitValue = 0;
    UINT32 index = 0;
    //UINT32 u4RepeatFrame =9999;
    UINT32 u4VaTrans = 0x3000000;
    UINT8* hw_address = NULL;
    UINT8* SrcAddr = NULL;
    UINT8* ThirdAddr = NULL;
    UINT8* SrcPallete = NULL;
    UINT8* ThirdPallete = NULL;
    UINT8* golden_dataY = NULL;
    UINT8* DstAddr = NULL;
    //BOOL fgFirstTime = TRUE;
    UINT32 u4FrameSize = (1920 * 1200 * 4);
    UINT8* PA_address = NULL;
    UINT8* Golden_address = NULL;

    UINT32 u4GlobalAlpha, u4Opcode, u4AlcomAr, u4SrcPallte, u4ThirdPallete, u4GoldenSize;
    BOOL fgAlcomNormal, fgThirdEn, fgAlcomDiffCM;


	UINT32 u4Width_Com, u4Height_Com;
	UINT32 u4RollBackEn_Com, u4QualityMode_Com, u4BpCompAddrEnd_Com, u4ColorMode_Com;
	UINT32 u4SrcAddr_Com, u4DstAddr_Com, u4SrcPitch_Com, u4DstPitch_Com = 0;
	BOOL fgMirrorFlip_Com;
	BOOL fgBurstRead_Com;
	char* pcFileInfo_Com;
	char* pcToken_Com;
	UINT32 file_size1_Com, file_size2_Com, u4BytesPixel_Com = 1;
	UINT32 FileSize_Com;
	UINT32 index_Com = 0;
	//UINT32 u4RepeatFrame =9999;
	UINT8* SrcAddr_Com = NULL;
	UINT8* DstAddr_Com = NULL;
	UINT8* golden_data1_Com = NULL;
	UINT8* golden_data2_Com = NULL;
	UINT8* GoldenData_Com = NULL;
	UINT8* hw_address_Com = NULL;
	//BOOL fgFirstTime = TRUE;

    //golden_dataY = (UINT8*)BSP_AllocAlignedDmaMemory((1920 * 1088*4), 1024);


    DstAddr = (UINT8*)_u4GfxEmuBuf[0];
    golden_dataY = DstAddr + u4FrameSize;
    SrcAddr = golden_dataY + u4FrameSize;
    ThirdAddr = SrcAddr + u4FrameSize;


    hw_address = DstAddr;
    u4SrcAddr 	= (UINT32) SrcAddr;//(UINT32) 0x01ea4000;
    u4DstAddr 	= (UINT32) DstAddr;
	u4ThirdAddr = (UINT32) ThirdAddr;

	DstAddr_Com = (UINT8*)_u4GfxEmuBuf[0] - u4FrameSize;
	golden_data1_Com = DstAddr_Com - u4FrameSize;
	SrcAddr_Com = golden_data1_Com - u4FrameSize;
	golden_data2_Com = SrcAddr_Com - u4FrameSize;
	
	hw_address_Com = DstAddr_Com;
	u4SrcAddr_Com 	= (UINT32) SrcAddr_Com;//(UINT32) 0x01ea4000;
	u4DstAddr_Com 	= (UINT32) DstAddr_Com;

	
	_CLI_CMD("gfx.reset");
    _CLI_CMD("gfxemu.mode");

    for (j = 0;j < 9;j++)  // UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
	{
		HalSet1MSectionTable(u4SrcAddr + j*0x100000, u4SrcAddr + u4VaTrans + j*0x100000);   //+48M
		HalSet1MSectionTable(u4ThirdAddr + j*0x100000, u4ThirdAddr + u4VaTrans + j*0x100000);
		HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr + u4VaTrans + j*0x100000);
		HalSet1MSectionTable(u4SrcAddr_Com + j*0x100000, u4SrcAddr_Com - u4VaTrans + j*0x100000); //+48M
		HalSet1MSectionTable(u4DstAddr_Com + j*0x100000, u4DstAddr_Com - u4VaTrans + j*0x100000);			
	}
		   //should blt dst raw data to PA 4K address
	HalSet1MSectionTable(u4DstAddr , u4DstAddr);
	x_memset((void*)0xa000000, 0, 0x400);
	HalSet4KPageTable(u4DstAddr, u4DstAddr + u4VaTrans, (UINT32 *)0xa000000);
	for (j = 1; j < 0x100; j++)
	{
		HalSet4KPageTable((u4DstAddr&0xfff00000) | (j << 12), ((u4DstAddr + u4VaTrans)&0xfff00000) | (j << 12), (UINT32 *)0xa000000);
	}
	HalSet4KPageTable(0xa801000, ((u4DstAddr + u4VaTrans)&0xfff00000) + 0x1000, (UINT32 *)0xa000000);
	HalSet4KPageTable(0xa805000, ((u4DstAddr + u4VaTrans)&0xfff00000) + 0x2000, (UINT32 *)0xa000000);
	_VGFX_SetTexture(340, 340, 340, 340);
                
	_GFX_MMU_Init();					
	_GFX_MMU_Set_TabAddr(HalGetMMUTableAddress());

   // _GFX_MMU_Enable(FALSE);
    x_memset((void*)DstAddr, 0, u4FrameSize);
    x_memset((void*)golden_dataY, 0, u4FrameSize);
    x_memset((void*)SrcAddr, 0, u4FrameSize);
    x_memset((void*)ThirdAddr, 0, u4FrameSize);

    SrcPallete = (UINT8 *)BSP_AllocAlignedDmaMemory((1024), 1024);
    ThirdPallete = (UINT8 *)BSP_AllocAlignedDmaMemory((1024), 1024);

    if (SrcAddr == NULL || ThirdAddr == NULL || SrcPallete == NULL || ThirdPallete == NULL || DstAddr == NULL || golden_dataY == NULL)
    {
        ASSERT(0);
    }

    Printf("General Alpha Composition Stress MMU Test ...\n");

    u4InitValue =  (UINT32) StrToInt(szArgv[1]);
    u4Times =  (UINT32) StrToInt(szArgv[2]);

    //u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);

    pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);

    for (i = u4InitValue ; i < u4Times ; i++)
    {
        //if(i == (u4RepeatFrame+1))
        //    i--;
        UINT32 file_data[21] = {0};

        index = 0;


        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\emu_setup_%d.txt\" 0x%08x /ny\n", i, pcFileInfo);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\emu_setup_%d.txt\" 0x%08x /ny",
                            i, pcFileInfo));

        //Printf("string : %s\n",pcFileInfo);
        //sscanf(pcFileInfo,"%s %s %s %s %s %s",&file_data[0],&file_data[1],&file_data[2],&file_data[3],&file_data[4],&file_data[5]);

        pcToken = strtok(pcFileInfo, "\r\n");
        while ( pcToken != NULL)
        {
            file_data[index++] = StrToInt(pcToken);
            pcToken = strtok(NULL, "\r\n");
        }

        u4SrcAddr = (UINT32)SrcAddr;
        u4DstAddr = (UINT32)DstAddr;
        u4ThirdAddr = (UINT32)ThirdAddr;
        u4SrcPitch = (UINT32)file_data[2] * 16;
        u4DstPitch = (UINT32)file_data[3] * 16;
        u4SrcCM = (UINT32)file_data[4];
        u4Height = (UINT32)file_data[5] + 1;
        u4Width = (UINT32)file_data[6];
        u4GlobalAlpha = (UINT32)file_data[7];
        fgAlcomNormal = (BOOL)file_data[8];
        u4Opcode = file_data[9];
        u4AlcomAr = file_data[10];
        u4SrcPallte = (UINT32)SrcPallete;
        u4ThirdAddr = (UINT32)ThirdAddr;
        u4ThirdPallete = (UINT32)ThirdPallete;
        u4ThirdPitch = file_data[14] * 16;
        u4ThirdCM = file_data[15];
        fgThirdEn = (BOOL)file_data[16];
        fgAlcomDiffCM = (BOOL)file_data[17];
        u4DstCM = (UINT32)file_data[19];
        u4GoldenSize = file_data[20];

        //wanring solve
        u4SrcPallte = u4SrcPallte;
        u4ThirdPallete = u4ThirdPallete;
        fgAlcomDiffCM = fgAlcomDiffCM;

        //if(fgFirstTime) {
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_bmp_%d.raw\" 0x%08x /ny\n", i, u4SrcAddr);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_bmp_%d.raw\" 0x%08x /ny",
                            (i), u4SrcAddr));

        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_plt_%d.raw\" 0x%08x /ny\n", i, SrcPallete);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_plt_%d.raw\" 0x%08x /ny",
                            (i), SrcPallete));

        if (fgThirdEn)
        {
            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny\n", i, ThirdAddr);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny",
                                (i), ThirdAddr));

            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny\n", i, ThirdPallete);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny",
                                (i), ThirdPallete));
        }
        else
        {
            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny\n", i, ThirdAddr);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny",
                                (i), DstAddr));

            //LOG(0, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny\n",i,ThirdPallete);
            //UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny",
            //(i),ThirdPallete));
        }
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\dram_result_%d.raw\" 0x%08x /ny\n", (i), golden_dataY);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\dram_result_%d.raw\" 0x%08x /ny",
                            (i), golden_dataY));
        //}

        printf("%d %d %d\n", u4SrcCM, u4DstCM, u4ThirdCM);

   //     _CLI_CMD("gfx.reset");
    //    _CLI_CMD("gfxemu.mode");
		
        _GFX_MMU_Set_Agent0(1, 0);

        _GFX_SetSrc((UINT8 *)(u4SrcAddr + u4VaTrans), u4SrcCM, u4SrcPitch);
        _GFX_SetThird((UINT8 *)(u4ThirdAddr + u4VaTrans), u4ThirdCM, u4ThirdPitch);
        _GFX_SetDst((UINT8 *)(u4DstAddr + u4VaTrans), u4DstCM, u4DstPitch);
       
        GFX_SetAlpha(u4GlobalAlpha);
        //GFX_SetAlpha(0xB3);

        _GFX_SetCharCM(TRUE, E_BMP_CM_8BIT);
        _GFX_SetCharCM(FALSE, E_BMP_CM_8BIT);

        _GFX_SetThirdPallete(ThirdPallete, 0, 0);
        _GFX_SetIdx2DirOpt(SrcPallete, 0, 0);       


        _GFX_GeneralAlphaCom(u4Width, u4Height, u4AlcomAr,
                             u4Opcode, 0, fgThirdEn, fgAlcomNormal, FALSE);
        hw_address = (UINT8*)(u4DstAddr);

		////////////////////////////////////////////////////////////////////
{
        Printf("Compression BitBlt MMU Enable Test 2 ...");


		//u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);

        fgMirrorFlip_Com = FALSE;

        pcFileInfo_Com = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);
	{
        int line_separate = 0;
        
        UINT32 file_data[10] = {0};

        index_Com = 0;

        x_memset((void*)pcFileInfo_Com, 0, 0x40000);
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_info_%d.txt\" 0x%08x /ny\n", i, pcFileInfo_Com);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_info_%d.txt\" 0x%08x /ny",
                                    i, pcFileInfo_Com));
		pcToken_Com = strtok(pcFileInfo_Com, " ");
        while ( pcToken_Com != NULL)
        {
                    file_data[index_Com++] = StrToInt(pcToken_Com);
                    //Printf("bitstream_sz : %d \n",bitstream_sz[u2file_num-1]);
                    pcToken_Com = strtok(NULL, " ");
        }


        u4Width_Com 	  = (UINT32) file_data[0];
        u4Height_Com	  = (UINT32) file_data[1];
        u4ColorMode_Com = (UINT32) file_data[2];
        u4QualityMode_Com = (UINT32) file_data[3];
        u4RollBackEn_Com  = (UINT32) file_data[4];
        file_size1_Com = (UINT32) file_data[5];
        fgBurstRead_Com = (BOOL) file_data[6];
        file_size2_Com = (UINT32) file_data[7];
        u4BpCompAddrEnd_Com = 0xffffffff;

        u4SrcPitch_Com = 0;


        if (u4ColorMode_Com == 2 || u4ColorMode_Com == 10)
        {
             u4BytesPixel_Com = 1;
        }
        else if (u4ColorMode_Com == 6 || u4ColorMode_Com == 14)
        {
             u4BytesPixel_Com = 4;
        }
        else if (u4ColorMode_Com == 13 || u4ColorMode_Com == 12 || u4ColorMode_Com == 11)
        {
             u4BytesPixel_Com = 2;
        }
        else
        {
             ASSERT(0);
        }

        u4SrcPitch_Com	  = u4Width_Com * u4BytesPixel_Com;


        //if(fgFirstTime) {
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_%d.txt\" 0x%08x /ny\n", i, u4SrcAddr_Com);
                UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx\\emu_source_%d.txt\" 0x%08x /ny",
                                    (i), u4SrcAddr_Com));
                //x_thread_delay(1000);

        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx\\emu_bitstream_line_connect_%d.txt\" 0x%08x /ny\n", (i), golden_data1_Com);
                UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx\\emu_bitstream_line_connect_%d.txt\" 0x%08x /ny",
                                    (i), golden_data1_Com));

        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx\\emu_bitstream_line_separate_%d.txt\" 0x%08x /ny\n", (i), golden_data2_Com);
                UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx\\emu_bitstream_line_separate_%d.txt\" 0x%08x /ny",
                                    (i), golden_data2_Com));

        for (line_separate = 0; line_separate < 2 ;line_separate++)
        {
              _GFX_SetDst((UINT8 *)(u4DstAddr_Com - u4VaTrans), 14, 1920*4);
              _GFX_SetColor(0x0);
              _GFX_Fill(0, 0, 1920, 1200);
              
              if (line_separate == 1)
              {
                   UINT32 original_line_bit = u4Width_Com * u4BytesPixel_Com * 8;

                   UINT32 line_bit;
                   if (u4RollBackEn_Com)
                       line_bit = original_line_bit + 4;
                   else
                       line_bit = original_line_bit + 4 + original_line_bit / 16;
				   
                   if (fgBurstRead_Com)
                       line_bit = (line_bit % 512 == 0) ? line_bit : (line_bit / 512 + 1) * 512;
                   else
                       line_bit = (line_bit % 128 == 0) ? line_bit : (line_bit / 128 + 1) * 128;

                   u4DstPitch_Com = line_bit / 8;
				   
                   printf("width : %d\n", u4Width_Com);

                   GoldenData_Com = golden_data2_Com;
                   FileSize_Com = file_size2_Com;
                }
                else
                {
                   u4DstPitch_Com = u4SrcPitch_Com;
                   GoldenData_Com = golden_data1_Com;
                   FileSize_Com = file_size1_Com;
                }    
                _GFX_MMU_Set_Agent0(TRUE, 3); 
         
                
                _GFX_NewCompressBlt((UINT8 *)(u4SrcAddr_Com - u4VaTrans), 0, 0, u4ColorMode_Com, u4SrcPitch_Com,
                         (UINT8 *)(u4DstAddr_Com - u4VaTrans), u4Width_Com, u4Height_Com, u4Height_Com, u4RollBackEn_Com,
                                            u4QualityMode_Com, u4BpCompAddrEnd_Com, fgMirrorFlip_Com, fgBurstRead_Com, line_separate, u4DstPitch_Com);
            
                _CLI_CMD("gfx.flush");
                
                HalFlushInvalidateDCache();
                for (j = 0; j < FileSize_Com ; j++)
                {
                     if (hw_address_Com[j] != GoldenData_Com[j])
                     {
                            printf("hw : %x, golden : %x, j = %d\n", hw_address_Com[j], GoldenData_Com[j], j);
                            printf("compare error in emu_source_%d.txt %d\n", i, line_separate);
                            printf("BPCOMP_CHKSUM : %x\n", IO_READ32(GFX_ADDR, (0xb8)));
                            printf("BPCOMP_NIPPLE : %x\n", IO_READ32(GFX_ADDR, (0xb4)));
                            //return 1;
                            break;
                      }
                      else
                      {
                            //LOG(0,"hw : %x , golden : %x\n",hw_address[j],golden_dataY[j]);
                      }
                 }
          }
          printf("compare ok MMU Enable in emu_source_%d.txt\n\n", i);
            //fgFirstTime = FALSE;
	}
       Printf("Compression BitBlt MMU Enable Test 2  OK... \n");
}
		//////////////////////////////////////////////////////////////

        Golden_address = (UINT8*)golden_dataY;
        PA_address = (UINT8*)hw_address;;
        for ( j = 0; j < 0x1000; j ++)  //4K
        {
            if (PA_address[j] != Golden_address[j])
            {
                printf("hw : %x, golden : %x, j = %d\n", PA_address[j], Golden_address[j], j);
                printf("gfx alpha composition enable MMU compare error  0 in emu_setup_%d.txt\n\n", i);
                //printf("BPCOMP_CHKSUM : %x\n",IO_READ32(GFX_ADDR, (0xb8)));
                //printf("BPCOMP_NIPPLE : %x\n",IO_READ32(GFX_ADDR, (0xb4)));
                //return 1;
                break;
            }
        }
        Golden_address = (UINT8*)(golden_dataY + 0x3000);
        PA_address = (UINT8*)(hw_address + 0x3000);
        u4GoldenSize = u4GoldenSize - 3 * 0x1000;

        for (j = 0; j < u4GoldenSize ; j++)
        {
            if (PA_address[j] != Golden_address[j])
            {
                printf("hw : %x, golden : %x, j = %d\n", PA_address[j], Golden_address[j], j);
                printf("gfx alpha composition enable MMU  compare error 3 in emu_setup_%d.txt\n\n", i);
                //return 1;
                break;
            }
            else
            {
                //LOG(0,"hw : %x , golden : %x\n",hw_address[j],golden_dataY[j]);
        	}
       }
        printf("New alpha composition MMU open  comparing ok in emu_source_%d.txt\n\n", i);
        //fgFirstTime = FALSE;
	}
    return 0;
}


//test different state agent (disable/enable) in same cmdque
static INT32 _GfxEmuIOMMUAlcomCompress3(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height;
    UINT32 u4SrcAddr, u4DstAddr, u4SrcPitch, u4DstPitch, u4ThirdAddr, u4ThirdPitch;
    UINT32 u4SrcCM, u4DstCM, u4ThirdCM;
    char* pcFileInfo;
    UINT32 i, u4Times, j;
    char* pcToken;
    UINT32 u4InitValue = 0;
    UINT32 index = 0;
    //UINT32 u4RepeatFrame =9999;
    UINT32 u4VaTrans = 0x3000000;
    UINT8* hw_address = NULL;
    UINT8* SrcAddr = NULL;
    UINT8* ThirdAddr = NULL;
    UINT8* SrcPallete = NULL;
    UINT8* ThirdPallete = NULL;
    UINT8* golden_dataY = NULL;
    UINT8* DstAddr = NULL;
    //BOOL fgFirstTime = TRUE;
    UINT32 u4FrameSize = (1920 * 1200 * 4);
    UINT8* PA_address = NULL;
    UINT8* Golden_address = NULL;

    UINT32 u4GlobalAlpha, u4Opcode, u4AlcomAr, u4SrcPallte, u4ThirdPallete, u4GoldenSize;
    BOOL fgAlcomNormal, fgThirdEn, fgAlcomDiffCM;


	UINT32 u4SrcAddr_Com, u4DstAddr_Com;

	//UINT32 u4RepeatFrame =9999;
	UINT8* SrcAddr_Com = NULL;
	UINT8* DstAddr_Com = NULL;
	UINT8* golden_data1_Com = NULL;

	//BOOL fgFirstTime = TRUE;

    //golden_dataY = (UINT8*)BSP_AllocAlignedDmaMemory((1920 * 1088*4), 1024);


    DstAddr = (UINT8*)_u4GfxEmuBuf[0];
    golden_dataY = DstAddr + u4FrameSize;
    SrcAddr = golden_dataY + u4FrameSize;
    ThirdAddr = SrcAddr + u4FrameSize;


    hw_address  = DstAddr;
    u4SrcAddr 	= (UINT32) SrcAddr;//(UINT32) 0x01ea4000;
    u4DstAddr 	= (UINT32) DstAddr;
	u4ThirdAddr = (UINT32) ThirdAddr;


	DstAddr_Com = (UINT8*)_u4GfxEmuBuf[0] - u4FrameSize;
	golden_data1_Com = DstAddr_Com - u4FrameSize;
	SrcAddr_Com = golden_data1_Com - u4FrameSize;
	
	u4SrcAddr_Com 	= (UINT32) SrcAddr_Com;//(UINT32) 0x01ea4000;
	u4DstAddr_Com 	= (UINT32) DstAddr_Com;

	
	_CLI_CMD("gfx.reset");
    _CLI_CMD("gfxemu.mode");

    for (j = 0;j < 9;j++)  // UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
	{
		HalSet1MSectionTable(u4SrcAddr + j*0x100000, u4SrcAddr + u4VaTrans + j*0x100000);   //+48M
		HalSet1MSectionTable(u4ThirdAddr + j*0x100000, u4ThirdAddr + u4VaTrans + j*0x100000);
		HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr + u4VaTrans + j*0x100000);
		HalSet1MSectionTable(u4SrcAddr_Com + j*0x100000, u4SrcAddr_Com - u4VaTrans + j*0x100000); //+48M
		HalSet1MSectionTable(u4DstAddr_Com + j*0x100000, u4DstAddr_Com - u4VaTrans + j*0x100000);			
	}
		   //should blt dst raw data to PA 4K address
	HalSet1MSectionTable(u4DstAddr , u4DstAddr);
	x_memset((void*)0xa000000, 0, 0x400);
	HalSet4KPageTable(u4DstAddr, u4DstAddr + u4VaTrans, (UINT32 *)0xa000000);
	for (j = 1; j < 0x100; j++)
	{
		HalSet4KPageTable((u4DstAddr&0xfff00000) | (j << 12), ((u4DstAddr + u4VaTrans)&0xfff00000) | (j << 12), (UINT32 *)0xa000000);
	}
	HalSet4KPageTable(0xa801000, ((u4DstAddr + u4VaTrans)&0xfff00000) + 0x1000, (UINT32 *)0xa000000);
	HalSet4KPageTable(0xa805000, ((u4DstAddr + u4VaTrans)&0xfff00000) + 0x2000, (UINT32 *)0xa000000);
	_VGFX_SetTexture(340, 340, 340, 340);
                
	_GFX_MMU_Init();					
	_GFX_MMU_Set_TabAddr(HalGetMMUTableAddress());

   // _GFX_MMU_Enable(FALSE);
    x_memset((void*)DstAddr, 0, u4FrameSize);
    x_memset((void*)golden_dataY, 0, u4FrameSize);
    x_memset((void*)SrcAddr, 0, u4FrameSize);
    x_memset((void*)ThirdAddr, 0, u4FrameSize);

    SrcPallete = (UINT8 *)BSP_AllocAlignedDmaMemory((1024), 1024);
    ThirdPallete = (UINT8 *)BSP_AllocAlignedDmaMemory((1024), 1024);

    if (SrcAddr == NULL || ThirdAddr == NULL || SrcPallete == NULL || ThirdPallete == NULL || DstAddr == NULL || golden_dataY == NULL)
    {
        ASSERT(0);
    }

    Printf("General Alpha Composition Stress MMU Test ...\n");

    u4InitValue =  (UINT32) StrToInt(szArgv[1]);
    u4Times =  (UINT32) StrToInt(szArgv[2]);



    //u4RepeatFrame = (UINT32)StrToInt(szArgv[3]);

    pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(0x40000, 1024);

    for (i = u4InitValue ; i < u4Times ; i++)
    {
        //if(i == (u4RepeatFrame+1))
        //    i--;
        UINT32 file_data[21] = {0};

        index = 0;


        x_memset((void*)pcFileInfo, 0, 0x40000);
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\emu_setup_%d.txt\" 0x%08x /ny\n", i, pcFileInfo);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\emu_setup_%d.txt\" 0x%08x /ny",
                            i, pcFileInfo));

        //Printf("string : %s\n",pcFileInfo);
        //sscanf(pcFileInfo,"%s %s %s %s %s %s",&file_data[0],&file_data[1],&file_data[2],&file_data[3],&file_data[4],&file_data[5]);

        pcToken = strtok(pcFileInfo, "\r\n");
        while ( pcToken != NULL)
        {
            file_data[index++] = StrToInt(pcToken);
            pcToken = strtok(NULL, "\r\n");
        }

        u4SrcAddr = (UINT32)SrcAddr;
        u4DstAddr = (UINT32)DstAddr;
        u4ThirdAddr = (UINT32)ThirdAddr;
        u4SrcPitch = (UINT32)file_data[2] * 16;
        u4DstPitch = (UINT32)file_data[3] * 16;
        u4SrcCM = (UINT32)file_data[4];
        u4Height = (UINT32)file_data[5] + 1;
        u4Width = (UINT32)file_data[6];
        u4GlobalAlpha = (UINT32)file_data[7];
        fgAlcomNormal = (BOOL)file_data[8];
        u4Opcode = file_data[9];
        u4AlcomAr = file_data[10];
        u4SrcPallte = (UINT32)SrcPallete;
        u4ThirdAddr = (UINT32)ThirdAddr;
        u4ThirdPallete = (UINT32)ThirdPallete;
        u4ThirdPitch = file_data[14] * 16;
        u4ThirdCM = file_data[15];
        fgThirdEn = (BOOL)file_data[16];
        fgAlcomDiffCM = (BOOL)file_data[17];
        u4DstCM = (UINT32)file_data[19];
        u4GoldenSize = file_data[20];

        //wanring solve
        u4SrcPallte = u4SrcPallte;
        u4ThirdPallete = u4ThirdPallete;
        fgAlcomDiffCM = fgAlcomDiffCM;

        //if(fgFirstTime) {
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_bmp_%d.raw\" 0x%08x /ny\n", i, u4SrcAddr);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_bmp_%d.raw\" 0x%08x /ny",
                            (i), u4SrcAddr));

        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_plt_%d.raw\" 0x%08x /ny\n", i, SrcPallete);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_src_plt_%d.raw\" 0x%08x /ny",
                            (i), SrcPallete));

        if (fgThirdEn)
        {
            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny\n", i, ThirdAddr);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny",
                                (i), ThirdAddr));

            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny\n", i, ThirdPallete);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny",
                                (i), ThirdPallete));
        }
        else
        {
            LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny\n", i, ThirdAddr);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_bmp_%d.raw\" 0x%08x /ny",
                                (i), DstAddr));

            //LOG(0, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny\n",i,ThirdPallete);
            //UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\random_thd_plt_%d.raw\" 0x%08x /ny",
            //(i),ThirdPallete));
        }
        LOG(10, "//d.load.binary \"d:\\gfx_emulation\\gfx2\\dram_result_%d.raw\" 0x%08x /ny\n", (i), golden_dataY);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\gfx2\\dram_result_%d.raw\" 0x%08x /ny",
                            (i), golden_dataY));
        //}

        printf("%d %d %d\n", u4SrcCM, u4DstCM, u4ThirdCM);

   //     _CLI_CMD("gfx.reset");
    //    _CLI_CMD("gfxemu.mode");
		
        _GFX_MMU_Set_Agent0(1, 0);
   		_GFX_MMU_Set_Agent1(1, 4);
   		_GFX_MMU_Set_Agent2(0, 1);

		_GFX_SetSrc((UINT8 *)(u4SrcAddr + u4VaTrans), u4SrcCM, u4SrcPitch);
		_GFX_SetThird((UINT8 *)(u4ThirdAddr + u4VaTrans), u4ThirdCM, u4ThirdPitch);
   		_GFX_SetDst((UINT8 *)(u4DstAddr ), u4DstCM, u4DstPitch);
       
        GFX_SetAlpha(u4GlobalAlpha);
        //GFX_SetAlpha(0xB3);

        _GFX_SetCharCM(TRUE, E_BMP_CM_8BIT);
        _GFX_SetCharCM(FALSE, E_BMP_CM_8BIT);

        _GFX_SetThirdPallete(ThirdPallete, 0, 0);
        _GFX_SetIdx2DirOpt(SrcPallete, 0, 0);       


        _GFX_GeneralAlphaCom(u4Width, u4Height, u4AlcomAr,
                             u4Opcode, 0, fgThirdEn, fgAlcomNormal, FALSE);
        hw_address = (UINT8*)(u4DstAddr);

        Golden_address = (UINT8*)golden_dataY;
        PA_address = (UINT8*)hw_address;;
        for ( j = 0; j < 0x1000; j ++)  //4K
        {
            if (PA_address[j] != Golden_address[j])
            {
                printf("hw : %x, golden : %x, j = %d\n", PA_address[j], Golden_address[j], j);
                printf("gfx alpha composition enable MMU compare error  0 in emu_setup_%d.txt\n\n", i);
                //printf("BPCOMP_CHKSUM : %x\n",IO_READ32(GFX_ADDR, (0xb8)));
                //printf("BPCOMP_NIPPLE : %x\n",IO_READ32(GFX_ADDR, (0xb4)));
                return 1;
                //break;
            }
        }
        Golden_address = (UINT8*)(golden_dataY + 0x3000);
        PA_address = (UINT8*)(hw_address + 0x3000);
        u4GoldenSize = u4GoldenSize - 3 * 0x1000;

        for (j = 0; j < u4GoldenSize ; j++)
        {
            if (PA_address[j] != Golden_address[j])
            {
                printf("hw : %x, golden : %x, j = %d\n", PA_address[j], Golden_address[j], j);
                printf("gfx alpha composition enable MMU  compare error 3 in emu_setup_%d.txt\n\n", i);
                return 1;
                //break;
            }
            else
            {
                //LOG(0,"hw : %x , golden : %x\n",hw_address[j],golden_dataY[j]);
        	}
       }
        printf("New alpha composition MMU open  comparing ok in emu_source_%d.txt\n\n", i);
        //fgFirstTime = FALSE;
	}
    return 0;
}

//test MMU YCbCr to ARGB8888
static INT32 _GfxEmuCmdConvMMU(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4SrcAddr, u4DstAddr,u4ThirdAddr,j;
	
	UINT32 u4Type,u4Width,u4Height,u4loop,u4Pitch,u4Size,u4CompSize;
	//UINT8 * puDstAress = BUF_TEMP;
	//UINT8 * puGolden = COMPARE_BUF;

 	UINT32 u4VaTrans = 0x3000000;
 	UINT8* hw_address = NULL;
 	UINT8* SrcAddr = NULL;
 	UINT8* ThirdAddr = NULL;
 	//UINT8* SrcPallete = NULL;
 	//UINT8* ThirdPallete = NULL;
 	UINT8* golden_dataY = NULL;
 	UINT8* DstAddr = NULL;
 //BOOL fgFirstTime = TRUE;
 	UINT32 u4FrameSize = (1920 * 1200 * 4);
 	UINT8* PA_address = NULL;
 	UINT8* Golden_address = NULL;

 	//UINT32 u4SrcAddr_Com, u4DstAddr_Com
 	//UINT8* SrcAddr_Com = NULL;
 	//UINT8* DstAddr_Com = NULL;
 	//UINT8* golden_data1_Com = NULL;
 	//UINT8* golden_data2_Com = NULL;
 	//UINT8* GoldenData_Com = NULL;
 	//UINT8* hw_address_Com = NULL;

	u4Type = 1;
	UINT32 ToARGBSize[8] = {37120,37584,38048,38512,38976,39440,39904,40268};

 	Printf("FlashFormatConv test\n");

 	DstAddr = (UINT8*)_u4GfxEmuBuf[0];
 	golden_dataY = DstAddr + u4FrameSize;
 	SrcAddr = golden_dataY + u4FrameSize;
 	ThirdAddr = SrcAddr + u4FrameSize;


 	hw_address = DstAddr;
 	u4SrcAddr	 = (UINT32) SrcAddr;//(UINT32) 0x01ea4000;
 	u4DstAddr	 = (UINT32) DstAddr;
	u4ThirdAddr = (UINT32) ThirdAddr;

 	//DstAddr_Com = (UINT8*)_u4GfxEmuBuf[0] - u4FrameSize;
 	//golden_data1_Com = DstAddr_Com - u4FrameSize;
 	//SrcAddr_Com = golden_data1_Com - u4FrameSize;
 	//golden_data2_Com = SrcAddr_Com - u4FrameSize;
 
 	//hw_address_Com = DstAddr_Com;
 	//u4SrcAddr_Com	 = (UINT32) SrcAddr_Com;//(UINT32) 0x01ea4000;
 	//u4DstAddr_Com	 = (UINT32) DstAddr_Com;

 
 	_CLI_CMD("gfx.reset");
 	_CLI_CMD("gfxemu.mode");

 	for (j = 0;j < 9;j++)	// UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
 	{
	 	HalSet1MSectionTable(u4SrcAddr + j*0x100000, u4SrcAddr + u4VaTrans + j*0x100000);	 //+48M
	 	HalSet1MSectionTable(u4ThirdAddr + j*0x100000, u4ThirdAddr + u4VaTrans + j*0x100000);
	 	HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr + u4VaTrans + j*0x100000);
	 	//HalSet1MSectionTable(u4SrcAddr_Com + j*0x100000, u4SrcAddr_Com - u4VaTrans + j*0x100000); //+48M
	 	//HalSet1MSectionTable(u4DstAddr_Com + j*0x100000, u4DstAddr_Com - u4VaTrans + j*0x100000);			 
 	}
		//should blt dst raw data to PA 4K address
 	HalSet1MSectionTable(u4DstAddr , u4DstAddr);
 	x_memset((void*)0xa000000, 0, 0x400);
 	HalSet4KPageTable(u4DstAddr, u4DstAddr + u4VaTrans, (UINT32 *)0xa000000);
 	for (j = 1; j < 0x100; j++)
 	{
	 	HalSet4KPageTable((u4DstAddr&0xfff00000) | (j << 12), ((u4DstAddr + u4VaTrans)&0xfff00000) | (j << 12), (UINT32 *)0xa000000);
 	}
 	HalSet4KPageTable(0xa801000, ((u4DstAddr + u4VaTrans)&0xfff00000) + 0x1000, (UINT32 *)0xa000000);
 	HalSet4KPageTable(0xa805000, ((u4DstAddr + u4VaTrans)&0xfff00000) + 0x2000, (UINT32 *)0xa000000);
 	//_VGFX_SetTexture(340, 340, 340, 340);
			 
 	_GFX_MMU_Init();					 
 	_GFX_MMU_Set_TabAddr(HalGetMMUTableAddress());

	// _GFX_MMU_Enable(FALSE);
 	x_memset((void*)DstAddr, 0, u4FrameSize);
 	x_memset((void*)golden_dataY, 0, u4FrameSize);
 	x_memset((void*)SrcAddr, 0, u4FrameSize);
 	x_memset((void*)ThirdAddr, 0, u4FrameSize);


 	if (SrcAddr == NULL || ThirdAddr == NULL || DstAddr == NULL || golden_dataY == NULL)
 	{
	 	ASSERT(0);
 	}

 	Printf("General Alpha Composition Stress MMU Test ...\n");
	
 	u4Height = 58;
	u4Pitch = 768;
 	Printf("d.load.binary D:/gfx_emulation/Cobra_Android_pattern/read_Y.raw 0x%08x /ny\n", SrcAddr);
	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\Cobra_Android_pattern\\read_Y.raw\" 0x%08x /ny", SrcAddr));
	Printf("d.load.binary D:/gfx_emulation/Cobra_Android_pattern/read_CbCr.raw 0x%08x /ny\n", ThirdAddr);
	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\Cobra_Android_pattern\\read_CbCr.raw\" 0x%08x /ny",ThirdAddr));

	for(u4loop = 0 ; u4loop < 8 ; u4loop++)
	{
		u4Width = 160+u4loop*2;
		//puDstAress = BUF_TEMP;
		//puGolden = COMPARE_BUF; 	
		_GFX_MMU_Set_Agent0(1, 0);
		_GFX_MMU_Set_Agent1(1, 4);
		_GFX_MMU_Set_Agent2(0, 1);
		
		Printf("d.load.binary D:/gfx_emulation/Cobra_Android_pattern/android_type%d_width%d_golden.raw 0x%08x /ny\n",(u4Type),(u4loop*2), golden_dataY);
		UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\Cobra_Android_pattern\\android_type%d_width%d_golden.raw\" 0x%08x /ny",(u4Type),(u4loop*2), golden_dataY));

		GFX_Set_FlashColCnvInfo((UINT8 *)(u4SrcAddr + u4VaTrans), u4Pitch, (UINT8 *)(u4ThirdAddr + u4VaTrans), 
					u4Pitch,( UINT8 * )u4DstAddr,u4Width,u4Height);
		GFX_YCbCr420ToARGB32(0xFF,0);
		u4CompSize = ToARGBSize[u4loop];
/* 
		hw_address = (UINT8*)(u4DstAddr);

        Golden_address = (UINT8*)golden_dataY;
        PA_address = (UINT8*)hw_address;
        for ( u4Size = 0; u4Size < u4CompSize; u4Size ++)  //4K
       {
            if (PA_address[u4Size] != Golden_address[u4Size])
            {
                printf("hw : %x, golden : %x, u4Size = %d\n", PA_address[u4Size], Golden_address[u4Size], u4Size);
                printf("gfx alpha composition enable MMU compare error  0 in emu_setup_%d.txt\n\n", i);
                //printf("BPCOMP_CHKSUM : %x\n",IO_READ32(GFX_ADDR, (0xb8)));
                //printf("BPCOMP_NIPPLE : %x\n",IO_READ32(GFX_ADDR, (0xb4)));
                //return 1;
                break;
            }
        }
        Golden_address = (UINT8*)(golden_dataY + 0x3000);
        PA_address = (UINT8*)(hw_address + 0x3000);
        u4GoldenSize = u4CompSize - 3 * 0x1000;

        for (u4Size = 0; u4Size < u4CompSize ; u4Size++)
        {
            if (PA_address[u4Size] != Golden_address[u4Size])
            {
                printf("hw : %x, golden : %x, u4Size = %d\n", PA_address[u4Size], Golden_address[u4Size], u4Size);
                printf("gfx alpha composition enable MMU  compare error 3 in emu_setup_%d.txt\n\n", i);
                //return 1;
                break;
            }
            else
            {
                //LOG(0,"hw : %x , golden : %x\n",hw_address[j],golden_dataY[j]);
        	}
       }

*/
		Golden_address = (UINT8*)golden_dataY;
		PA_address = (UINT8*)hw_address;

		for(u4Size = 0; u4Size < u4CompSize ; u4Size++) 
		{
			if (PA_address[u4Size] != Golden_address[u4Size] )
			{
				printf("puDstAress : %x, puGolden : %x, u4Size = %d\n", PA_address[u4Size], Golden_address[u4Size], u4Size);
				printf("FlashFormatConv test error : type [%d] width [%d] \n",(u4Type),(u4loop*2));
				return 1;
			}
		}  
	}
	printf("[MMU]YCbCr to ARGB8888 test ok \n"); 
	return 0;
}



//test Flash Formate Conv  (3Buf)
static INT32 _GfxEmuCmdFlashConv3BMMU(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4SrcAddr, u4DstAddr,u4ThirdAddr,j;
	
	UINT32 u4Type,u4Width,u4Height,u4loop,u4Pitch,u4Size,u4CompSize;
	//UINT8 * puDstAress = BUF_TEMP;
	//UINT8 * puGolden = COMPARE_BUF;

 	UINT32 u4VaTrans = 0x3000000;
 	UINT8* hw_address = NULL;
 	UINT8* SrcAddr = NULL;
 	UINT8* ThirdAddr = NULL;
 	//UINT8* SrcPallete = NULL;
 	//UINT8* ThirdPallete = NULL;
 	UINT8* golden_dataY = NULL;
 	UINT8* DstAddr = NULL;
 //BOOL fgFirstTime = TRUE;
 	UINT32 u4FrameSize = (1920 * 1200 * 4);
 	UINT8* PA_address = NULL;
 	UINT8* Golden_address = NULL;
	UINT8 *puTempY = NULL;
	UINT8 *puTempWrite = NULL;

 	//UINT32 u4SrcAddr_Com, u4DstAddr_Com
 	//UINT8* SrcAddr_Com = NULL;
 	//UINT8* DstAddr_Com = NULL;
 	//UINT8* golden_data1_Com = NULL;
 	//UINT8* golden_data2_Com = NULL;
 	//UINT8* GoldenData_Com = NULL;
 	//UINT8* hw_address_Com = NULL;

	u4Type = 3;

	UINT32 ToRasterSize[8] = {13920,14094,14268,14442,14616,14790,14964,15138};
 	Printf("FlashFormatConv test\n");

 	DstAddr = (UINT8*)_u4GfxEmuBuf[0];
 	golden_dataY = DstAddr + u4FrameSize;
 	SrcAddr = golden_dataY + u4FrameSize;
 	ThirdAddr = SrcAddr + u4FrameSize;


 	hw_address = DstAddr;
 	u4SrcAddr	 = (UINT32) SrcAddr;//(UINT32) 0x01ea4000;
 	u4DstAddr	 = (UINT32) DstAddr;
	u4ThirdAddr = (UINT32) ThirdAddr;

 	//DstAddr_Com = (UINT8*)_u4GfxEmuBuf[0] - u4FrameSize;
 	//golden_data1_Com = DstAddr_Com - u4FrameSize;
 	//SrcAddr_Com = golden_data1_Com - u4FrameSize;
 	//golden_data2_Com = SrcAddr_Com - u4FrameSize;
 
 	//hw_address_Com = DstAddr_Com;
 	//u4SrcAddr_Com	 = (UINT32) SrcAddr_Com;//(UINT32) 0x01ea4000;
 	//u4DstAddr_Com	 = (UINT32) DstAddr_Com;

 
 	_CLI_CMD("gfx.reset");
 	_CLI_CMD("gfxemu.mode");

 	for (j = 0;j < 9;j++)	// UINT32 u4FrameSize = (1920 * 1200 * 4) = 0x8CA000;
 	{
	 	HalSet1MSectionTable(u4SrcAddr + j*0x100000, u4SrcAddr + u4VaTrans + j*0x100000);	 //+48M
	 	HalSet1MSectionTable(u4ThirdAddr + j*0x100000, u4ThirdAddr + u4VaTrans + j*0x100000);
	 	HalSet1MSectionTable(u4DstAddr + j*0x100000, u4DstAddr + u4VaTrans + j*0x100000);
	 	//HalSet1MSectionTable(u4SrcAddr_Com + j*0x100000, u4SrcAddr_Com - u4VaTrans + j*0x100000); //+48M
	 	//HalSet1MSectionTable(u4DstAddr_Com + j*0x100000, u4DstAddr_Com - u4VaTrans + j*0x100000);			 
 	}
/*		//should blt dst raw data to PA 4K address
 	HalSet1MSectionTable(u4DstAddr , u4DstAddr);
 	x_memset((void*)0xa000000, 0, 0x400);
 	HalSet4KPageTable(u4DstAddr, u4DstAddr + u4VaTrans, (UINT32 *)0xa000000);
 	for (j = 1; j < 0x100; j++)
 	{
	 	HalSet4KPageTable((u4DstAddr&0xfff00000) | (j << 12), ((u4DstAddr + u4VaTrans)&0xfff00000) | (j << 12), (UINT32 *)0xa000000);
 	}
 	HalSet4KPageTable(0xa801000, ((u4DstAddr + u4VaTrans)&0xfff00000) + 0x1000, (UINT32 *)0xa000000);
 	HalSet4KPageTable(0xa805000, ((u4DstAddr + u4VaTrans)&0xfff00000) + 0x2000, (UINT32 *)0xa000000);
 	//_VGFX_SetTexture(340, 340, 340, 340);
			 
 */
 	_GFX_MMU_Init();					 
 	_GFX_MMU_Set_TabAddr(HalGetMMUTableAddress());

	// _GFX_MMU_Enable(FALSE);
 	x_memset((void*)DstAddr, 0, u4FrameSize);
 	x_memset((void*)golden_dataY, 0, u4FrameSize);
 	x_memset((void*)SrcAddr, 0, u4FrameSize);
 	x_memset((void*)ThirdAddr, 0, u4FrameSize);


 	if (SrcAddr == NULL || ThirdAddr == NULL || DstAddr == NULL || golden_dataY == NULL)
 	{
	 	ASSERT(0);
 	}

 	Printf("General Alpha Composition Stress MMU Test ...\n");
	
 	u4Height = 58;
	u4Pitch = 768;
 	Printf("d.load.binary D:/gfx_emulation/Cobra_Android_pattern/read_Y.raw 0x%08x /ny\n", SrcAddr);
	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\Cobra_Android_pattern\\read_Y.raw\" 0x%08x /ny", SrcAddr));
	Printf("d.load.binary D:/gfx_emulation/Cobra_Android_pattern/read_CbCr.raw 0x%08x /ny\n", ThirdAddr);
	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\Cobra_Android_pattern\\read_CbCr.raw\" 0x%08x /ny",ThirdAddr));


	for(u4loop = 0 ; u4loop < 8 ; u4loop++)
	{
		u4Width = 160+u4loop*2;
		//puDstAress = BUF_TEMP;
		//puGolden = COMPARE_BUF; 	
	
		Printf("d.load.binary D:/gfx_emulation/Cobra_Android_pattern/android_type%d_width%d_golden.raw 0x%08x /ny\n",(u4Type),(u4loop*2), golden_dataY);
		UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\gfx_emulation\\Cobra_Android_pattern\\android_type%d_width%d_golden.raw\" 0x%08x /ny",(u4Type),(u4loop*2), golden_dataY));

		_GFX_MMU_Set_Agent0(1, 0);
		_GFX_MMU_Set_Agent1(1, 4);
		_GFX_MMU_Set_Agent2(1, 1);

		//pass1
		GFX_Set_FlashColCnvInfo((UINT8 *)(u4SrcAddr + u4VaTrans), u4Pitch, (UINT8 *)(u4ThirdAddr + u4VaTrans), 
					u4Pitch,( UINT8 * )(u4DstAddr + u4VaTrans),u4Width,u4Height);
		_GFX_Set_YCbCrBlockToRaster(OP_YBLOCK_TO_YRASTER);
		_GFX_Flush();

		//pass2
		_GFX_MMU_Set_Agent0(1, 6);
		_GFX_MMU_Set_Agent1(1, 4);
		_GFX_MMU_Set_Agent2(1, 1);

		puTempY =(UINT8 *)( (u4DstAddr + u4VaTrans) + u4Width*u4Height*5/4 );
		puTempWrite = (UINT8 *) ((u4DstAddr + u4VaTrans) + u4Width*u4Height);
		GFX_Set_FlashColCnvInfo((UINT8 *)puTempY, u4Pitch, (UINT8 *)(u4ThirdAddr + u4VaTrans), 
					u4Pitch,( UINT8 * )puTempWrite,u4Width,u4Height);
		_GFX_Set_YCbCrBlockToRaster(OP_CBCRBLOCK_TO_1CBCRRASTER);
		_GFX_Flush();

		_GFX_Set_WriteNoPitch(FALSE);
		
		u4CompSize = ToRasterSize[u4loop];
/* 
		hw_address = (UINT8*)(u4DstAddr);

        Golden_address = (UINT8*)golden_dataY;
        PA_address = (UINT8*)hw_address;
        for ( u4Size = 0; u4Size < u4CompSize; u4Size ++)  //4K
       {
            if (PA_address[u4Size] != Golden_address[u4Size])
            {
                printf("hw : %x, golden : %x, u4Size = %d\n", PA_address[u4Size], Golden_address[u4Size], u4Size);
                printf("gfx alpha composition enable MMU compare error  0 in emu_setup_%d.txt\n\n", i);
                //return 1;
                break;
            }
        }
        Golden_address = (UINT8*)(golden_dataY + 0x3000);
        PA_address = (UINT8*)(hw_address + 0x3000);
        u4GoldenSize = u4CompSize - 3 * 0x1000;

        for (u4Size = 0; u4Size < u4CompSize ; u4Size++)
        {
            if (PA_address[u4Size] != Golden_address[u4Size])
            {
                printf("hw : %x, golden : %x, u4Size = %d\n", PA_address[u4Size], Golden_address[u4Size], u4Size);
                printf("gfx alpha composition enable MMU  compare error 3 in emu_setup_%d.txt\n\n", i);
                //return 1;
                break;
            }
       }

*/
		Golden_address = (UINT8*)golden_dataY;
		PA_address = (UINT8*)hw_address;

		for(u4Size = 0; u4Size < u4CompSize ; u4Size++) 
		{
			if (PA_address[u4Size] != Golden_address[u4Size] )
			{
				printf("puDstAress : %x, puGolden : %x, u4Size = %d\n", PA_address[u4Size], Golden_address[u4Size], u4Size);
				printf("FlashFormatConv test error : type [%d] width [%d] \n",(u4Type),(u4loop*2));
				return 1;
			}
		}  
	}
	printf("[MMU]YCbCr to ARGB8888 test ok \n"); 
	return 0;
}

#endif //#if  defined(CC_MT5398)

INT32 _GfxColorValueCompare(UINT32 u4ClrVal1, UINT32 u4ClrVal2, UINT32 u4CM)
{
    INT32 i4Rst = 0;
    XolorValue cv1, cv2;

    cv1.u4Color = u4ClrVal1;
    cv2.u4Color = u4ClrVal2;

    if (CM_ARGB4444_DIRECT16 == u4CM)
    {
        if (PIXEL_SUB(cv1.cm13[0].a, cv2.cm13[0].a) > 2 ||
            PIXEL_SUB(cv1.cm13[0].r, cv2.cm13[0].r) > 2 ||
            PIXEL_SUB(cv1.cm13[0].g, cv2.cm13[0].g) > 2 ||
            PIXEL_SUB(cv1.cm13[0].b, cv2.cm13[0].b) > 2 ||
            PIXEL_SUB(cv1.cm13[1].a, cv2.cm13[1].a) > 2 ||
            PIXEL_SUB(cv1.cm13[1].r, cv2.cm13[1].r) > 2 ||
            PIXEL_SUB(cv1.cm13[1].g, cv2.cm13[1].g) > 2 ||
            PIXEL_SUB(cv1.cm13[1].b, cv2.cm13[1].b) > 2)
        {
            i4Rst = -1;
        }
    }
    else if(CM_ARGB1555_DIRECT16 == u4CM)
    {
        if (/*PIXEL_SUB(cv1.cm12[0].a, cv2.cm12[0].a) > 1 || */
            PIXEL_SUB(cv1.cm12[0].r, cv2.cm12[0].r) > 2 ||
            PIXEL_SUB(cv1.cm12[0].g, cv2.cm12[0].g) > 2 ||
            PIXEL_SUB(cv1.cm12[0].b, cv2.cm12[0].b) > 2 ||
            /*PIXEL_SUB(cv1.cm12[1].a, cv2.cm12[1].a) > 1 || */
            PIXEL_SUB(cv1.cm12[1].r, cv2.cm12[1].r) > 2 ||
            PIXEL_SUB(cv1.cm12[1].g, cv2.cm12[1].g) > 2 ||
            PIXEL_SUB(cv1.cm12[1].b, cv2.cm12[1].b) > 2)
        {
            i4Rst = -1;
        }
    }
    else if(CM_ARGB8888_DIRECT32 == u4CM)
    {
        if (PIXEL_SUB(cv1.cm14.a, cv2.cm14.a) > 2 || 
            PIXEL_SUB(cv1.cm14.r, cv2.cm14.r) > 2 ||
            PIXEL_SUB(cv1.cm14.g, cv2.cm14.g) > 2 ||
            PIXEL_SUB(cv1.cm14.b, cv2.cm14.b) > 2 )
        {
            i4Rst = -1;
        }
    }
    else if (CM_RGB565_DIRECT16 == u4CM)
    {
        if (
            PIXEL_SUB(cv1.cm11[0].r, cv2.cm11[0].r) > 2 ||
            PIXEL_SUB(cv1.cm11[0].g, cv2.cm11[0].g) > 2 ||
            PIXEL_SUB(cv1.cm11[0].b, cv2.cm11[0].b) > 2 ||
            PIXEL_SUB(cv1.cm11[1].r, cv2.cm11[1].r) > 2 ||
            PIXEL_SUB(cv1.cm11[1].g, cv2.cm11[1].g) > 2 ||
            PIXEL_SUB(cv1.cm11[1].b, cv2.cm11[1].b) > 2)
       {
            i4Rst = -1;
        }
    }
    else
    {
        i4Rst = -1;
    }

    return (i4Rst);
}

INT32 _GfxMemCompare(UINT8 *pu1HwSa, UINT8 *pu1SwSa, UINT32 u4Num,UINT32 u4ColorModeIndex)
{
    INT32  i4Error = 0;
    UINT32 u4Temp  = 0;
    UINT32 u4TotalPixels = u4Num;
    UINT32 u4MaxDiff = 0;
    UINT32 u4BytePerPixel = 0, u4SrcBpp = 0;
    UINT32  *pu4Sw = (UINT32 *) pu1SwSa;
    UINT32  *pu4Hw = (UINT32 *) pu1HwSa;

    x_thread_delay(10);

    if (u4TotalPixels <= 0)
    {
        Printf("[Gfx][Ev] Error: u4TotalPixels <= 0");
        return 0;
    }
	
    switch (u4ColorModeIndex)
    {
    case CM_RGB_CLUT8:
        u4TotalPixels = u4TotalPixels / 4;
        u4BytePerPixel = 1;
        break;

    case CM_RGB565_DIRECT16:
    case CM_ARGB1555_DIRECT16:
    case CM_ARGB4444_DIRECT16:
        u4TotalPixels = u4TotalPixels / 2;
        u4BytePerPixel = 2;
        break;

    case CM_ARGB8888_DIRECT32:
    default:
        u4BytePerPixel = 4;
        break;
    }
    
    switch (u4ColorModeIndex)
    {
    case CM_RGB_CLUT8:
        u4SrcBpp = 1;
        break;

    case CM_RGB565_DIRECT16:
    case CM_ARGB1555_DIRECT16:
    case CM_ARGB4444_DIRECT16:
        u4SrcBpp = 2;
        break;

    case CM_ARGB8888_DIRECT32:
    default:
        u4SrcBpp = 4;
        break;
    }

    //u4TotalPixels = EV_GFX_MEM_SLOT_SIZE / 4;
    while (u4TotalPixels--)
    {
        if (*pu4Sw != *pu4Hw)
        {
            //if (0 ==fgEvTestWT())
            {
                //if ((CM_ARGB1555_DIRECT16 == g_GfxVrfCfg.u4DstCM) || (0 == g_GfxVrfCfg.u4HwId))
                {
                    if (_GfxColorValueCompare(*pu4Sw, *pu4Hw,u4ColorModeIndex) == 0)
                    {
                        pu4Sw++;
                        pu4Hw++;
                        continue;
                    }
                }
            }
            ++i4Error;
            
            if (*pu4Sw > *pu4Hw)
            {
                u4Temp = *pu4Sw - *pu4Hw;
            }
            else
            {
                u4Temp = *pu4Hw - *pu4Sw;
            }

            if (u4Temp > u4MaxDiff)
            {
                u4MaxDiff = u4Temp;
            }
			
        }
        pu4Sw++;
        pu4Hw++;
    }

    //BIM_WAIT_WALE();

    if (i4Error)
    {
        Printf("[Gfx][Ev] HW/SW Comparison Fail => Error Cnt = %d (MaxDiff = 0x%x)\n", i4Error, u4MaxDiff);

        return -1;
    }
    /*
    else
    {
    EvOKPrintf("[Gfx][Ev] Mem Comparison OK! \n");
    }
    */
	UNUSED(u4BytePerPixel);
	UNUSED(u4SrcBpp);
	
    return 1;
}

#endif // #if defined(GFX_ENABLE_SW_MODE)


// for debug use
/*
static void _GfxSetMidWriteRegion(void)
{
    _CLI_CMD("mid.i");
    MID_ResetRegion(0);
    MID_SetRegion(0, MID_AGENT_CPU, 0x00000000, 0x0FFFFFFF);
    MID_EnableRegionProtect(0);    

    CLI_Parser("mid.h on");
//    CLI_Parser("mid.rl");
}
*/

CLI_MAIN_COMMAND_ITEM(GfxEmu)
{
    "gfxemu",
    NULL,
    NULL,
    _arGfxEmuTbl,
    "Gfx emulation suit",
    CLI_SUPERVISOR
};

/*lint -restore */


