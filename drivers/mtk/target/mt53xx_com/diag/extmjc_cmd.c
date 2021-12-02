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
 * $RCSfile: extmjc_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file extmjc_cmd.c
 *  EXTMJC CLI commands
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------


#include "x_drv_cli.h"
#include "x_stl_lib.h"
#include "x_printf.h"

#include "extmjc_if.h"
//#include "u_drv_cust.h"
#include "drv_lvds.h"
#include "osd_drvif.h"
#include "pmx_drvif.h"
#include "sif_if.h"
#include "uart_if.h"
#include "x_timer.h"
#include "x_gpio.h"
/*----------------------------------------------------------------------------*/

#ifdef DRV_SUPPORT_EXTMJC
//---------------------------------------------------------------------
// LOG macro related

// #define DEFINE_IS_LOG	CLI_IsLog
#define DEFINE_IS_LOG CLI_IsLog
#include "x_debug.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#if 0
CLIMOD_DEBUG_FUNCTIONS(EXTMJC)
#endif
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define BYTE3(arg)        (*((UINT8 *)&(arg) + 3))
#define BYTE2(arg)        (*((UINT8 *)&(arg) + 2))
#define BYTE1(arg)        (*((UINT8 *)&(arg) + 1))
#define BYTE0(arg)        (* (UINT8 *)&(arg))

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
extern INT32 drv_extmjc_sram_write(
    UINT16                                  ui2_sram_addr,
    UINT32                                  ui4_sram_data,
    UINT8*                                  pui1_msg_error
    );
extern UINT8 u1ExtmjcReadRegByI2C(
    UINT16 addr,
    UINT8* pdata
    );

extern UINT8 u1ExtmjcWriteRegByI2C(
    UINT16 u2addr,
    UINT32 u1data
    );
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifndef DRV_SUPPORT_MT8283
static INT32 i4CmdSetMJCMode(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 ui1_err;
    if(i4Argc != 4)
    {
        return -1024;
    }

    d_custom_extmjc_set_mjc_setting(StrToInt(szArgv[1]), StrToInt(szArgv[2]), StrToInt(szArgv[3]), &ui1_err);

    return 0;
}
#endif

static INT32 i4CmdSetPing(INT32 i4Argc, const CHAR** szArgv)
{
    //UINT8 ui1_err;
    //if(i4Argc != 1)
    if(i4Argc != 2)
    {
        return -1024;
    }

    //drv_extmjc_set_ping(&ui1_err);
    d_custom_extmjc_enable_alive_check(StrToInt(szArgv[1]));
    Printf( "Ping=%d\r\n",StrToInt(szArgv[1] ));

    return 0;
}

#ifndef DRV_SUPPORT_MT8283
static INT32 i4CmdSetColorTempEx(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8   ui1_idx;
    UINT16   ui2_value;

    if(i4Argc != 3)
    {
        return -1024;
    }

    ui1_idx = StrToInt(szArgv[1]);
    ui2_value = StrToInt(szArgv[2]);

    drv_extmjc_set_color_temperature_ex(0, ui1_idx, ui2_value);

    return 0;
}
#endif

static INT32 i4CmdSetGamma(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 ui1_err;
    if(i4Argc != 2)
    {
        return -1024;
    }

    drv_extmjc_set_gamma(StrToInt(szArgv[1]), &ui1_err);

    return 0;
}

#ifdef DRV_SUPPORT_MT8283
static INT32 i4CmdSetVideoFmt(INT32 i4Argc, const CHAR** szArgv)
{
       UINT8 ui1_err;
	UINT8 u1LvdsMode;
	UINT8 u1FrameRate;
       UINT16 u2HTotal;
       UINT16 u2VTotal;
       UINT16 u2Width;
       UINT16 u2Height;
       UINT8 u1MjcOnOff;
	//UINT8 ui1_data;

    if(i4Argc != 6)
    {
        return -1024;
    }

	u1LvdsMode = StrToInt(szArgv[1]);
	u1FrameRate = StrToInt(szArgv[2]);
	u2HTotal = StrToInt(szArgv[3]);
	u2VTotal = StrToInt(szArgv[4]);
	u1MjcOnOff = StrToInt(szArgv[5]);
	u2Width = StrToInt(szArgv[6]);
	u2Height = StrToInt(szArgv[7]);

	// Change 538x LVDS mode
	vDrvSetCurLvdsMode(u1LvdsMode);
//	vDrvLVDSOff();
//	vDrvLVDSConfig();
//	vDrvLVDSOn();

	// Change 8280 LVDS mode

	drv_extmjc_set_video_fmt(u1LvdsMode,u1FrameRate, u2HTotal, u2VTotal, u1MjcOnOff,u2Width,u2Height,&ui1_err);

#if 0
	if (u1LvdsMode == 1)
	{
		ui1_data = 0x11;
		SIF_WriteMultipleSubAddr(0x100, 0xc, 2, 0x0055, &ui1_data, 1);
	}
	else
	{
		ui1_data = 0x22;
		SIF_WriteMultipleSubAddr(0x100, 0xc, 2, 0x0055, &ui1_data, 1);
	}
#endif

	x_thread_delay(10);
	OSD_DMA_SetMode(OSD_OP_DMA);
	x_thread_delay(10);
	OSD_DMA_SetMode(OSD_OP_NORMAL);

#if 0
	// set 8280 osd mode
	ui1_data = 0x3;
	SIF_WriteMultipleSubAddr(0x100, 0xc, 2, 0x70c0, &ui1_data, 1);
	x_thread_delay(10);

	ui1_data = 0x2;
	SIF_WriteMultipleSubAddr(0x100, 0xc, 2, 0x70c0, &ui1_data, 1);
	x_thread_delay(10);

	ui1_data = 0x3;
	SIF_WriteMultipleSubAddr(0x100, 0xc, 2, 0x70c0, &ui1_data, 1);
	x_thread_delay(10);

	ui1_data = 0xf7;
	SIF_WriteMultipleSubAddr(0x100, 0xc, 2, 0x70c4, &ui1_data, 1);
	x_thread_delay(10);

	ui1_data = 0xf6;
	SIF_WriteMultipleSubAddr(0x100, 0xc, 2, 0x70c4, &ui1_data, 1);
	x_thread_delay(10);

	ui1_data = 0x1;
	SIF_WriteMultipleSubAddr(0x100, 0xc, 2, 0x7200, &ui1_data, 1);
	x_thread_delay(10);

	ui1_data = 0x1;
	SIF_WriteMultipleSubAddr(0x100, 0xc, 2, 0x7000, &ui1_data, 1);
	x_thread_delay(40);
#endif

	// Change 538x and 8280 OSD to DMA mode
	drv_extmjc_set_osd_mode(DRV_EXTMJC_OSD_MODE_DMA, &ui1_err);

	OSD_DMA_SetMode(OSD_OP_DMA);


	//x_thread_delay(10);
//	OSD_DMA_SetMode(OSD_OP_NORMAL);
//	OSD_DMA_SetMode(OSD_OP_DMA);

	// Enable 8280 lvds
	//drv_extmjc_set_lvds_enable(TRUE, &ui1_err);

    return 0;
}
#else
static INT32 i4CmdSetVideoFmt(INT32 i4Argc, const CHAR** szArgv)
{
       UINT8 ui1_err;
	UINT8 u1LvdsMode;
	UINT8 u1FrameRate;
       UINT16 u2HTotal;
       UINT16 u2VTotal;
       UINT8 u1MjcOnOff;
	//UINT8 ui1_data;

    if(i4Argc != 6)
    {
        return -1024;
    }

	u1LvdsMode = StrToInt(szArgv[1]);
	u1FrameRate = StrToInt(szArgv[2]);
	u2HTotal = StrToInt(szArgv[3]);
	u2VTotal = StrToInt(szArgv[4]);
	u1MjcOnOff = StrToInt(szArgv[5]);

	// Change 538x LVDS mode
	vDrvSetCurLvdsMode(u1LvdsMode);
//	vDrvLVDSOff();
//	vDrvLVDSConfig();
//	vDrvLVDSOn();

	// Change 8280 LVDS mode

	drv_extmjc_set_video_fmt(u1LvdsMode,u1FrameRate, u2HTotal, u2VTotal, u1MjcOnOff,&ui1_err);

#if 0
	if (u1LvdsMode == 1)
	{
		ui1_data = 0x11;
		SIF_WriteMultipleSubAddr(0x100, 0xc, 2, 0x0055, &ui1_data, 1);
	}
	else
	{
		ui1_data = 0x22;
		SIF_WriteMultipleSubAddr(0x100, 0xc, 2, 0x0055, &ui1_data, 1);
	}
#endif

	x_thread_delay(10);
	OSD_DMA_SetMode(OSD_OP_DMA);
	x_thread_delay(10);
	OSD_DMA_SetMode(OSD_OP_NORMAL);

#if 0
	// set 8280 osd mode
	ui1_data = 0x3;
	SIF_WriteMultipleSubAddr(0x100, 0xc, 2, 0x70c0, &ui1_data, 1);
	x_thread_delay(10);

	ui1_data = 0x2;
	SIF_WriteMultipleSubAddr(0x100, 0xc, 2, 0x70c0, &ui1_data, 1);
	x_thread_delay(10);

	ui1_data = 0x3;
	SIF_WriteMultipleSubAddr(0x100, 0xc, 2, 0x70c0, &ui1_data, 1);
	x_thread_delay(10);

	ui1_data = 0xf7;
	SIF_WriteMultipleSubAddr(0x100, 0xc, 2, 0x70c4, &ui1_data, 1);
	x_thread_delay(10);

	ui1_data = 0xf6;
	SIF_WriteMultipleSubAddr(0x100, 0xc, 2, 0x70c4, &ui1_data, 1);
	x_thread_delay(10);

	ui1_data = 0x1;
	SIF_WriteMultipleSubAddr(0x100, 0xc, 2, 0x7200, &ui1_data, 1);
	x_thread_delay(10);

	ui1_data = 0x1;
	SIF_WriteMultipleSubAddr(0x100, 0xc, 2, 0x7000, &ui1_data, 1);
	x_thread_delay(40);
#endif

	// Change 538x and 8280 OSD to DMA mode
	drv_extmjc_set_osd_mode(DRV_EXTMJC_OSD_MODE_DMA, &ui1_err);

	OSD_DMA_SetMode(OSD_OP_DMA);


	//x_thread_delay(10);
//	OSD_DMA_SetMode(OSD_OP_NORMAL);
//	OSD_DMA_SetMode(OSD_OP_DMA);

	// Enable 8280 lvds
	//drv_extmjc_set_lvds_enable(TRUE, &ui1_err);

    return 0;
}
#endif
/*
static INT32 i4CmdSetPip(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 ui1_err;
    DRV_EXTMJC_PIP_T t_pip;

    if (i4Argc == 2)
    {
        t_pip.ui1_pip_mode = StrToInt(szArgv[1]);
    }
    else if (i4Argc == 7)
    {
        t_pip.ui1_pip_mode = StrToInt(szArgv[1]);

        if (0 == StrToInt(szArgv[2]))
        {
            t_pip.b_main = TRUE;
        }
        else
        {
            t_pip.b_main = FALSE;
        }

        t_pip.ui2_vid_left_top_x = StrToInt(szArgv[3]);
        t_pip.ui2_vid_left_top_y = StrToInt(szArgv[4]);
        t_pip.ui2_vid_right_bottom_x = StrToInt(szArgv[5]);
        t_pip.ui2_vid_right_bottom_y = StrToInt(szArgv[6]);
    }
    else
    {
        return -1024;
    }

    drv_extmjc_set_pip(&t_pip, &ui1_err);

    return 0;
}
*/

#ifndef DRV_SUPPORT_MT8283
static INT32 i4CmdGetVer(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 ui1_err;
    DRV_CUSTOM_EXTMJC_VERSION_T t_ver;
    UINT8 aui1_ver[20];

    if (i4Argc != 1)
    {
        return -1024;
    }

    t_ver.ui1_size = 20;
    t_ver.pui1_version = aui1_ver;
    d_custom_extmjc_get_version(&ui1_err, &t_ver);

    return 0;
}

static INT32 i4CmdSetWinPos(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 ui1_err;
    DRV_CUSTOM_EXTMJC_WINDOW_POSITION_T t_win_pos;

    if (i4Argc != 5)
    {
        return -1024;
    }

    t_win_pos.ui2_left_top_x = StrToInt(szArgv[1]);
    t_win_pos.ui2_left_top_y = StrToInt(szArgv[2]);
    t_win_pos.ui2_right_bottom_x = StrToInt(szArgv[3]);
    t_win_pos.ui2_right_bottom_y = StrToInt(szArgv[4]);
    d_custom_extmjc_set_window_position(&t_win_pos, 1, &ui1_err);

    return 0;
}

static INT32 i4CmdSetLVDS(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 ui1_err;
    if(i4Argc != 2)
    {
        return -1024;
    }

    drv_extmjc_set_lvds_enable(StrToInt(szArgv[1]), &ui1_err);

    return 0;
}
#endif

static INT32 i4CmdSetOSDEnable(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 ui1_err;
    if(i4Argc != 2)
    {
        return -1024;
    }

    d_custom_extmjc_set_osd_mode(StrToInt(szArgv[1]), &ui1_err);

    return 0;
}

static INT32 i4CmdSetOSDMode(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 ui1_err;
    if(i4Argc != 2)
    {
        return -1024;
    }

	d_custom_extmjc_set_osd_mode(StrToInt(szArgv[1]), &ui1_err);

    return 0;
}

#ifndef DRV_SUPPORT_MT8283
static INT32 i4CmdSetOSDInfo(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 ui1_err;
    if(i4Argc != 4)
    {
        return -1024;
    }

    drv_extmjc_set_osd_info(StrToInt(szArgv[1]),
                            StrToInt(szArgv[2]),
                            StrToInt(szArgv[3]),
                            &ui1_err);

    return 0;
}

static INT32 i4CmdSetGPIOTest(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 ui1_err;
    if(i4Argc != 3)
    {
        return -1024;
    }

    d_custom_extmjc_set_gpio(StrToInt(szArgv[1]), StrToInt(szArgv[2]), &ui1_err);

    return 0;
}

/*
static INT32 i4CmdGetGPIOTest(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 ui1_err;
	BOOL  b_value;
    if(i4Argc != 2)
    {
        return -1024;
    }

    d_custom_extmjc_get_gpio(StrToInt(szArgv[1]), &b_value, &ui1_err);
	Printf("Gpio value=%d", b_value);

    return 0;
}
*/

static INT32 i4CmdSetColorPatternTest(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 ui1_err;
    if(i4Argc != 2)
    {
        return -1024;
    }

    d_custom_extmjc_set_color_pattern((DRV_CUSTOM_EXTMJC_COLOR_PATTERN_T)StrToInt(szArgv[1]),  &ui1_err);

    return 0;
}

/*
static INT32 i4CmdSetDemoMode(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 ui1_err;

	if(i4Argc != 3)
    {
        return -1024;
    }

	d_custom_extmjc_set_demo_mode(StrToInt(szArgv[1]), StrToInt(szArgv[1]), &ui1_err);

    return 0;
}
*/

static INT32 i4CmdReadSram(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 ui1_err;

	if(i4Argc != 2)
    {
        return -1024;
    }

	drv_extmjc_sram_read(StrToInt(szArgv[1]), &ui1_err);

    return 0;
}


static INT32 i4CmdWriteSram(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 ui1_err;

	if(i4Argc != 3)
    {
        return -1024;
    }

	drv_extmjc_sram_write(StrToInt(szArgv[1]), StrToInt(szArgv[2]), &ui1_err);

    return 0;
}

static INT32 i4CmdSetDramTest(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 ui1_err;
    if(i4Argc != 3)
    {
        return -1024;
    }

    d_custom_extmjc_set_dram(StrToInt(szArgv[1]), StrToInt(szArgv[2]), &ui1_err);
    printf("write extmjc dram addr=0x%08x, write dram data (hex)=0x%08x\n",
            StrToInt(szArgv[1]),StrToInt(szArgv[2]));

    return 0;
}

static INT32 i4CmdGetDramTest(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 ui1_err;
	UINT32  ui4_data;
    if(i4Argc != 2)
    {
        return -1024;
    }

    d_custom_extmjc_get_dram(StrToInt(szArgv[1]), &ui4_data, &ui1_err);
    printf("read extmjc dram addr=0x%08x, read dram data=0x%08x\n", StrToInt(szArgv[1]),ui4_data);

    return 0;
}
#endif

#ifndef DRV_SUPPORT_MT8283
#if 1
static INT32 _u18280Init(INT32 i4Argc, const CHAR **szArgv)
{

#if 1//BL154
UINT8 u1ret;
	//vDrvSYSPLLPWUPIni
	u1ExtmjcWriteRegByI2C(0x840, 0x10);
	u1ExtmjcWriteRegByI2C(0x841, 0x0C);	//0x8C->0x0C		//0x80 is strange
	u1ExtmjcWriteRegByI2C(0x842, 0x18);
	u1ExtmjcWriteRegByI2C(0x843, 0x04);
	u1ExtmjcWriteRegByI2C(0x844, 0x0);

	//need delay
	u1ExtmjcWriteRegByI2C(0x845, 0x0);
	u1ExtmjcWriteRegByI2C(0x846, 0x0);
	u1ExtmjcWriteRegByI2C(0x847, 0x0);
	u1ExtmjcWriteRegByI2C(0x848, 0x0);
	u1ExtmjcWriteRegByI2C(0x849, 0x18);
	u1ExtmjcWriteRegByI2C(0x84a, 0x0);
	u1ExtmjcWriteRegByI2C(0x84b, 0x10);
	u1ExtmjcWriteRegByI2C(0x84c, 0x20);

	//need delay
	u1ExtmjcWriteRegByI2C(0x80B, 0xF0);
	u1ExtmjcWriteRegByI2C(0x80C, 0x69);
	u1ExtmjcWriteRegByI2C(0x80D, 0x40);
	u1ExtmjcWriteRegByI2C(0x80E, 0x2A);
	u1ExtmjcWriteRegByI2C(0x80F, 0x01);

	//vDrvDMPLL
	u1ExtmjcWriteRegByI2C(0x850, 0x10);
	u1ExtmjcWriteRegByI2C(0x851, 0x0A);
	u1ExtmjcWriteRegByI2C(0x852, 0x66);
	u1ExtmjcWriteRegByI2C(0x853, 0x4);
	u1ExtmjcWriteRegByI2C(0x854, 0x0);
	u1ExtmjcWriteRegByI2C(0x855, 0x10);

	//MEMPLL
	u1ExtmjcWriteRegByI2C(0x80b, 0xF0);
	u1ExtmjcWriteRegByI2C(0x80c, 0x69);
	u1ExtmjcWriteRegByI2C(0x80d, 0x60);
	u1ExtmjcWriteRegByI2C(0x80e, 0x1);
	u1ExtmjcWriteRegByI2C(0x80f, 0xF1);


	// dram clk 324
	u1ExtmjcWriteRegByI2C(0x851, 0x0C);
	// if dram clk is 275 , remove this line
	u1ExtmjcReadRegByI2C(0x80d,&u1ret);
	u1ExtmjcWriteRegByI2C(0x80d,u1ret&0x3f);

	//vSetDramType
	// set DDR agent PRIORITY
	u1ExtmjcWriteRegByI2C(0x104c, 0x21);
	u1ExtmjcWriteRegByI2C(0x104d, 0x43);
	u1ExtmjcWriteRegByI2C(0x104e, 0x65);
	u1ExtmjcWriteRegByI2C(0x104f, 0x87);
	u1ExtmjcWriteRegByI2C(0x1050, 0xa9);
	u1ExtmjcWriteRegByI2C(0x1051, 0x00);
	u1ExtmjcWriteRegByI2C(0x1052, 0x50);
	u1ExtmjcWriteRegByI2C(0x1053, 0xf4);
	//set DDR Burst Length
	u1ExtmjcWriteRegByI2C(0x10cc, 0xff);
	u1ExtmjcWriteRegByI2C(0x10cd, 0xff);
	u1ExtmjcWriteRegByI2C(0x10ce, 0xff);
	u1ExtmjcWriteRegByI2C(0x10cf, 0xff);
	u1ExtmjcWriteRegByI2C(0x10d0, 0xff);
	u1ExtmjcWriteRegByI2C(0x10d1, 0xff);
	u1ExtmjcWriteRegByI2C(0x10d2, 0xff);
	u1ExtmjcWriteRegByI2C(0x10d3, 0xff);

	//_DDR_SetDramBeforeAutoInit
	// COLADDR < 10
	u1ExtmjcWriteRegByI2C(0x1000, 0x65);
	u1ExtmjcWriteRegByI2C(0x1001, 0x44);
	u1ExtmjcWriteRegByI2C(0x1002, 0x97);
	u1ExtmjcWriteRegByI2C(0x1003, 0x22);
	u1ExtmjcWriteRegByI2C(0x1080, 0x00);
	u1ExtmjcWriteRegByI2C(0x1081, 0x00);
	u1ExtmjcWriteRegByI2C(0x1082, 0x00);
	u1ExtmjcWriteRegByI2C(0x1083, 0x00);
	u1ExtmjcWriteRegByI2C(0x1090, 0x00);
	u1ExtmjcWriteRegByI2C(0x1091, 0x00);
	u1ExtmjcWriteRegByI2C(0x1092, 0x00);
	u1ExtmjcWriteRegByI2C(0x1093, 0x00);

	u1ExtmjcWriteRegByI2C(0x1098, 0x80);
	u1ExtmjcWriteRegByI2C(0x1099, 0x80);
	u1ExtmjcWriteRegByI2C(0x109a, 0x80);
	u1ExtmjcWriteRegByI2C(0x109b, 0x80);
	u1ExtmjcWriteRegByI2C(0x109c, 0x80);
	u1ExtmjcWriteRegByI2C(0x109d, 0x80);
	u1ExtmjcWriteRegByI2C(0x109e, 0x80);
	u1ExtmjcWriteRegByI2C(0x109f, 0x80);
	u1ExtmjcWriteRegByI2C(0x10a0, 0x80);
	u1ExtmjcWriteRegByI2C(0x10a1, 0x80);
	u1ExtmjcWriteRegByI2C(0x10a2, 0x80);
	u1ExtmjcWriteRegByI2C(0x10a3, 0x80);
	u1ExtmjcWriteRegByI2C(0x10a4, 0x80);
	u1ExtmjcWriteRegByI2C(0x10a5, 0x80);
	u1ExtmjcWriteRegByI2C(0x10a6, 0x80);
	u1ExtmjcWriteRegByI2C(0x10a7, 0x80);
	u1ExtmjcWriteRegByI2C(0x10a8, 0x80);
	u1ExtmjcWriteRegByI2C(0x10a9, 0x80);
	u1ExtmjcWriteRegByI2C(0x10aa, 0x80);
	u1ExtmjcWriteRegByI2C(0x10ab, 0x80);
	u1ExtmjcWriteRegByI2C(0x10ac, 0x80);
	u1ExtmjcWriteRegByI2C(0x10ad, 0x80);
	u1ExtmjcWriteRegByI2C(0x10ae, 0x80);
	u1ExtmjcWriteRegByI2C(0x10af, 0x80);
	u1ExtmjcWriteRegByI2C(0x10b0, 0x80);
	u1ExtmjcWriteRegByI2C(0x10b1, 0x80);
	u1ExtmjcWriteRegByI2C(0x10b2, 0x80);
	u1ExtmjcWriteRegByI2C(0x10b3, 0x80);
	u1ExtmjcWriteRegByI2C(0x10b4, 0x80);
	u1ExtmjcWriteRegByI2C(0x10b5, 0x80);
	u1ExtmjcWriteRegByI2C(0x10b6, 0x80);
	u1ExtmjcWriteRegByI2C(0x10b7, 0x80);

	u1ExtmjcWriteRegByI2C(0x10b8, 0x00);
	u1ExtmjcWriteRegByI2C(0x10b9, 0x66);
	u1ExtmjcWriteRegByI2C(0x10ba, 0x00);
	u1ExtmjcWriteRegByI2C(0x10bb, 0x66);
	u1ExtmjcWriteRegByI2C(0x10bc, 0x00);
	u1ExtmjcWriteRegByI2C(0x10bd, 0x66);
	u1ExtmjcWriteRegByI2C(0x10be, 0x00);
	u1ExtmjcWriteRegByI2C(0x10bf, 0x66);

	u1ExtmjcWriteRegByI2C(0x1028, 0x0d);
	u1ExtmjcWriteRegByI2C(0x1029, 0x0f);
	u1ExtmjcWriteRegByI2C(0x102a, 0xa5);
	u1ExtmjcWriteRegByI2C(0x102b, 0xe1);

	u1ExtmjcWriteRegByI2C(0x10d8, 0x00);
	u1ExtmjcWriteRegByI2C(0x10d9, 0x60);
	u1ExtmjcWriteRegByI2C(0x10da, 0x00);
	u1ExtmjcWriteRegByI2C(0x10db, 0x00);

	u1ExtmjcWriteRegByI2C(0x1008, 0x00);
	u1ExtmjcWriteRegByI2C(0x1009, 0x53);
	u1ExtmjcWriteRegByI2C(0x100a, 0x44);
	u1ExtmjcWriteRegByI2C(0x100b, 0x00);

	u1ExtmjcWriteRegByI2C(0x100c, 0x00);
	u1ExtmjcWriteRegByI2C(0x100d, 0x00);
	u1ExtmjcWriteRegByI2C(0x100e, 0x00);
	u1ExtmjcWriteRegByI2C(0x100f, 0x58);
	u1ExtmjcWriteRegByI2C(0x1010, 0x88);
	u1ExtmjcWriteRegByI2C(0x1011, 0x33);
	u1ExtmjcWriteRegByI2C(0x1012, 0x00);
	u1ExtmjcWriteRegByI2C(0x1013, 0x00);
	u1ExtmjcWriteRegByI2C(0x1014, 0x88);
	u1ExtmjcWriteRegByI2C(0x1015, 0x33);
	u1ExtmjcWriteRegByI2C(0x1016, 0x00);
	u1ExtmjcWriteRegByI2C(0x1017, 0x00);
	u1ExtmjcWriteRegByI2C(0x108c, 0x01);
	u1ExtmjcWriteRegByI2C(0x108d, 0x00);
	u1ExtmjcWriteRegByI2C(0x108e, 0x03);
	u1ExtmjcWriteRegByI2C(0x108f, 0x00);

	u1ExtmjcWriteRegByI2C(0x1098, 0x83);
	u1ExtmjcWriteRegByI2C(0x1099, 0x83);
	u1ExtmjcWriteRegByI2C(0x109a, 0x80);
	u1ExtmjcWriteRegByI2C(0x109b, 0x80);
	u1ExtmjcWriteRegByI2C(0x109c, 0x83);
	u1ExtmjcWriteRegByI2C(0x109d, 0x83);
	u1ExtmjcWriteRegByI2C(0x109e, 0x80);
	u1ExtmjcWriteRegByI2C(0x109f, 0x80);
	u1ExtmjcWriteRegByI2C(0x10a0, 0x83);
	u1ExtmjcWriteRegByI2C(0x10a1, 0x83);
	u1ExtmjcWriteRegByI2C(0x10a2, 0x80);
	u1ExtmjcWriteRegByI2C(0x10a3, 0x80);
	u1ExtmjcWriteRegByI2C(0x10a4, 0x83);
	u1ExtmjcWriteRegByI2C(0x10a5, 0x83);
	u1ExtmjcWriteRegByI2C(0x10a6, 0x80);
	u1ExtmjcWriteRegByI2C(0x10a7, 0x80);

	u1ExtmjcWriteRegByI2C(0x10b8, 0xFF);
	u1ExtmjcWriteRegByI2C(0x10b9, 0xCC);
	u1ExtmjcWriteRegByI2C(0x10ba, 0xFF);
	u1ExtmjcWriteRegByI2C(0x10bb, 0xCC);

	u1ExtmjcWriteRegByI2C(0x10bc, 0x00);
	u1ExtmjcWriteRegByI2C(0x10bd, 0x66);
	u1ExtmjcWriteRegByI2C(0x10be, 0x00);
	u1ExtmjcWriteRegByI2C(0x10bf, 0x22);

	u1ExtmjcWriteRegByI2C(0x10d8, 0x00);
	u1ExtmjcWriteRegByI2C(0x10d9, 0x02);
	u1ExtmjcWriteRegByI2C(0x10da, 0x60);
	u1ExtmjcWriteRegByI2C(0x10db, 0x00);


	u1ExtmjcWriteRegByI2C(0x10dc, 0x40);
	u1ExtmjcWriteRegByI2C(0x10dd, 0x00);
	u1ExtmjcWriteRegByI2C(0x10de, 0x04);
	u1ExtmjcWriteRegByI2C(0x10df, 0x00);

	u1ExtmjcWriteRegByI2C(0x10e0, 0x40);
	u1ExtmjcWriteRegByI2C(0x10e1, 0x00);
	u1ExtmjcWriteRegByI2C(0x10e2, 0x04);
	u1ExtmjcWriteRegByI2C(0x10e3, 0x00);

	u1ExtmjcWriteRegByI2C(0x107c, 0xCF);
	u1ExtmjcWriteRegByI2C(0x107d, 0x23);
	u1ExtmjcWriteRegByI2C(0x107e, 0xFF);
	u1ExtmjcWriteRegByI2C(0x107f, 0xBA);
	u1ExtmjcWriteRegByI2C(0x1094, 0x3B);
	u1ExtmjcWriteRegByI2C(0x1095, 0x3B);
	u1ExtmjcWriteRegByI2C(0x1096, 0x3B);
	u1ExtmjcWriteRegByI2C(0x1097, 0xBB);

	u1ExtmjcWriteRegByI2C(0x1000, 0x65);
	u1ExtmjcWriteRegByI2C(0x1001, 0x44);
	u1ExtmjcWriteRegByI2C(0x1002, 0x97);
	u1ExtmjcWriteRegByI2C(0x1003, 0x22);

        // Trigger auto initialization.
	u1ExtmjcWriteRegByI2C(0x1004, 0x00);
	u1ExtmjcWriteRegByI2C(0x1005, 0x00);
	u1ExtmjcWriteRegByI2C(0x1006, 0x01);
	u1ExtmjcWriteRegByI2C(0x1007, 0x00);

        //WAIT_DRAM_STABLE();
	//_DDR_SetDramAfterAutoInit(0x0);
	u1ExtmjcWriteRegByI2C(0x1004, 0x41);
	u1ExtmjcWriteRegByI2C(0x1005, 0x06);
	u1ExtmjcWriteRegByI2C(0x1006, 0x00);
	u1ExtmjcWriteRegByI2C(0x1007, 0x00);
	u1ExtmjcWriteRegByI2C(0x1008, 0x00);
	u1ExtmjcWriteRegByI2C(0x1009, 0x00);
	u1ExtmjcWriteRegByI2C(0x100a, 0x00);
	u1ExtmjcWriteRegByI2C(0x100b, 0x03);

	//_DDR_SetRtt(DDR_RTT_50OHM);
	u1ExtmjcWriteRegByI2C(0x10c2, 0x30);
	u1ExtmjcWriteRegByI2C(0x1008, 0x48);

	//DDR_CalibrateDqs
	u1ExtmjcReadRegByI2C(0x102b,&u1ret);
	u1ExtmjcWriteRegByI2C(0x102b,u1ret|0x10);
	//for 650MHz DQSDELAY  6 ~ 18 DLLCNT 26
	u1ExtmjcWriteRegByI2C(0x1018, 0x0a);
	u1ExtmjcWriteRegByI2C(0x1019, 0x0a);
	u1ExtmjcWriteRegByI2C(0x101a, 0x0a);
	u1ExtmjcWriteRegByI2C(0x101b, 0x0a);
	u1ExtmjcWriteRegByI2C(0x1020, 0x72);
	u1ExtmjcWriteRegByI2C(0x1021, 0x72);
	u1ExtmjcWriteRegByI2C(0x1022, 0x72);
	u1ExtmjcWriteRegByI2C(0x1023, 0x72);
	u1ExtmjcReadRegByI2C(0x102b,&u1ret);
	u1ExtmjcWriteRegByI2C(0x102b,u1ret&0xef);

	// 0x92000631: power down mode, tRRD
	u1ExtmjcWriteRegByI2C(0x1004, 0x71);
	u1ExtmjcWriteRegByI2C(0x1005, 0x05);
	u1ExtmjcWriteRegByI2C(0x1006, 0x00);
	u1ExtmjcWriteRegByI2C(0x1007, 0x90);
#endif //BL154
    return 0;
}
#endif
#endif

#ifndef DRV_SUPPORT_MT8283
#define CODE_SIZE 0x10000
UINT8 pcodebuffer1[CODE_SIZE];
static INT32 _u45382AllocDram(INT32 i4Argc, const CHAR **szArgv)
{
	Printf("code buffer address: 0x%x\n", &pcodebuffer1);

	return 0;
}

static INT32 _u45382DumpDram(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 i1i ;
	UINT8* tmpp;

	Printf("0x0000: ");
	tmpp = pcodebuffer1;
	for (i1i = 0; i1i < 10; i1i++)
	{
		Printf("0x%x ", *tmpp); tmpp++;
	}
	Printf("\n");

	Printf("0x8000: ");
	tmpp = pcodebuffer1 + 0x8000;
	for (i1i = 0; i1i < 10; i1i++)
	{
		Printf("0x%x ", *tmpp); tmpp++;
	}
	Printf("\n");
	return 0;
}
#define MT8280_ICU_DRAM_BASE (0x8000*0x0)
#define REAL_8280_CODE_SIZE 0x10000
//#define REAL_8280_CODE_SIZE 0x0032
static INT32 _u45382DownloadDram(INT32 i4Argc, const CHAR **szArgv)
{
#if 0
	UINT32 i4i;

	for (i4i = 0x0000; i4i <= REAL_8280_CODE_SIZE; i4i++)
	{
		u18280WriteDrambyI2C(MT8280_ICU_DRAM_BASE+i4i, pcodebuffer1[i4i]);
		if (u18280ReadDrambyI2C(MT8280_ICU_DRAM_BASE+i4i) != pcodebuffer1[i4i])
		{
			Printf("write fail: address 0x%x value 0x%x\n", MT8280_ICU_DRAM_BASE+i4i, u18280ReadDrambyI2C(MT8280_ICU_DRAM_BASE+i4i));
			Printf("correct value: 0x%x\n",  pcodebuffer1[i4i]);
			i4i--; // retry
		}
		if (((MT8280_ICU_DRAM_BASE+i4i) % 0x1000) == 0x0)
		{
			Printf("address: 0x%x\n", MT8280_ICU_DRAM_BASE+i4i);
		}
	}

	Printf("download finish\n");
#endif
	return 0;
}
static INT32 _u45382DownloadFlash(INT32 i4Argc, const CHAR **szArgv)
{
	UINT32 i4i;
	UINT8 u1data, u1tmp;

	//sflash write enable
	u1ExtmjcWriteRegByI2C(0x60d, 0x6);
	u1ExtmjcWriteRegByI2C(0x601, 0x8);
	u1ExtmjcWriteRegByI2C(0x600, 0x4);

	//polling ready
	do
	{
		u1ExtmjcWriteRegByI2C(0x600, 0x2);
		u1ExtmjcReadRegByI2C(0x602, &u1data);
	}
	while(u1data&0x1);

	//erase bank0
	u1ExtmjcWriteRegByI2C(0x60d, 0xd8);
	u1ExtmjcWriteRegByI2C(0x60c, 0x0);
	u1ExtmjcWriteRegByI2C(0x60b, 0x0);
	u1ExtmjcWriteRegByI2C(0x60a, 0x0);
	u1ExtmjcWriteRegByI2C(0x601, 0x20);
	u1ExtmjcWriteRegByI2C(0x600, 0x4);

	do
	{
		u1ExtmjcWriteRegByI2C(0x600, 0x2);
		u1ExtmjcReadRegByI2C(0x602, &u1data);
	}
	while(u1data&0x1);

	//program sflash "32bytes one time"
	for (i4i = 0x0000; i4i <= 0xffff; i4i=i4i+0x20)
	{
		u1ExtmjcWriteRegByI2C(0x615, 0x1);

		for (u1tmp = 0; u1tmp < 0x20; u1tmp++)
		{
			#if 0
			if(_bExtmjcICVersion==MT8280){
				u1ExtmjcWriteRegByI2C(0x625, pcodebuffer1[i4i+u1tmp]);
			}
				u1ExtmjcWriteRegByI2C(0x9981, pcodebuffer1[i4i+u1tmp]);
			{
			#else
				u1ExtmjcWriteRegByI2C(0x625, pcodebuffer1[i4i+u1tmp]);
			#endif
		}

		u1ExtmjcWriteRegByI2C(0x606, BYTE2(i4i));
		u1ExtmjcWriteRegByI2C(0x605, BYTE1(i4i));
		u1ExtmjcWriteRegByI2C(0x604, BYTE0(i4i));
		u1ExtmjcWriteRegByI2C(0x600, 0x10);

		do
  		{
		  u1ExtmjcWriteRegByI2C(0x600, 0x2);
		  u1ExtmjcReadRegByI2C(0x602, &u1data);
	  	}
  		while(u1data&0x1);

		if ((i4i % 0x1000) == 0x0)
		{
			Printf("address: 0x%x\n", i4i);
		}
	}
	Printf("sflash download finish\n");

	return 0;

}

static INT32 _u45382CompareDram(INT32 i4Argc, const CHAR **szArgv)
{
#if 0
	UINT32 i4i;

	for (i4i = 0x0000; i4i <= REAL_8280_CODE_SIZE; i4i++)
	{
		if (u18280ReadDrambyI2C(MT8280_ICU_DRAM_BASE+i4i) != pcodebuffer1[i4i])
		{
			Printf("write fail: address 0x%x value 0x%x\n", i4i, u18280ReadDrambyI2C(MT8280_ICU_DRAM_BASE+i4i));
			Printf("expect value: 0x%x\n", pcodebuffer1[i4i]);
			break;
		}
		if ((i4i % 0x1000) == 0x0)
		{
			Printf("address: 0x%x\n", i4i);
		}
	}
#endif

	return 0;
}
static INT32 _u4Trigger8280(INT32 i4Argc, const CHAR **szArgv)
{
	u1ExtmjcWriteRegByI2C(0x0807, 0x00);
	u1ExtmjcWriteRegByI2C(0x0808, 0x00);
	// u1ExtmjcWriteRegByI2C(0x0804, 0x10);
	u1ExtmjcWriteRegByI2C(0x0125, 0x01);
	u1ExtmjcWriteRegByI2C(0x0809, 0x00);

	return 0;
}

static INT32 _u45382DownloadDramDma(INT32 i4Argc, const CHAR **szArgv)
{
	UINT32 i4i;

	u1ExtmjcWriteRegByI2C(0x70c8, 0x00);
	u1ExtmjcWriteRegByI2C(0x70c9, 0x00);
	u1ExtmjcWriteRegByI2C(0x70ca, 0x00);
	u1ExtmjcWriteRegByI2C(0x70cb, 0x00);

	u1ExtmjcWriteRegByI2C(0x70c5, 0x04);

	u1ExtmjcWriteRegByI2C(0x70c0, 0x03);
	u1ExtmjcWriteRegByI2C(0x70c0, 0x02);
	u1ExtmjcWriteRegByI2C(0x70c0, 0x03);
	u1ExtmjcWriteRegByI2C(0x70c4, 0xf0);
	u1ExtmjcWriteRegByI2C(0x70c5, 0x7);
	u1ExtmjcWriteRegByI2C(0x70c2, 0x0);
	u1ExtmjcWriteRegByI2C(0x70c2, 0x1);

	for (i4i = 0x0000; i4i < REAL_8280_CODE_SIZE; i4i=i4i+256)
	{
    		//SIF_WriteMultipleSubAddr(0x80, 0xc, 0x2, 0x2534, pcodebuffer1+i4i, 256); //8280
		SIF_WriteMultipleSubAddr(0x80, 0xc, 0x2, 0x9981, pcodebuffer1+i4i, 256);	//8281

		if (((MT8280_ICU_DRAM_BASE+i4i) % 0x1000) == 0x0)
		{
			Printf("address: 0x%x\n", MT8280_ICU_DRAM_BASE+i4i);
		}
	}
	Printf("download finish\n");
	return 0;
}

static INT32 _u4I2CDma8280(INT32 i4Argc, const CHAR **szArgv)
{

    UINT8 u1Data;
    UINT8 u1Data1,u1Data2,u1Data3,u1Data4;

    u1Data1 = StrToInt(szArgv[1]);
    u1Data2 = StrToInt(szArgv[2]);
    u1Data3 = StrToInt(szArgv[3]);
    u1Data4 = StrToInt(szArgv[4]);

    //Dram start address
    u1Data = 0x00;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x70c8, (UINT8 *)&u1Data, 0x1);
    u1Data = 0x60;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x70c9, (UINT8 *)&u1Data, 0x1);
    u1Data = 0xc6;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x70ca, (UINT8 *)&u1Data, 0x1);
    u1Data = 0x01;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x70cb, (UINT8 *)&u1Data, 0x1);

    u1Data = 0x04;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x70c5, (UINT8 *)&u1Data, 0x1);

    u1Data = 0x3;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x70c0, (UINT8 *)&u1Data, 0x1);
    u1Data = 0x2;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x70c0, (UINT8 *)&u1Data, 0x1);
    u1Data = 0x3;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x70c0, (UINT8 *)&u1Data, 0x1);
    u1Data = 0xf1;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x70c4, (UINT8 *)&u1Data, 0x1);
    u1Data = 0xf0;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x70c4, (UINT8 *)&u1Data, 0x1);

    u1Data = u1Data1;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x2534, (UINT8 *)&u1Data, 0x1);
    u1Data = u1Data2;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x2534, (UINT8 *)&u1Data, 0x1);
    u1Data = u1Data3;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x2534, (UINT8 *)&u1Data, 0x1);
    u1Data = u1Data4;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x2534, (UINT8 *)&u1Data, 0x1);
    u1Data = 0;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x2534, (UINT8 *)&u1Data, 0x1);
    u1Data = 0;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x2534, (UINT8 *)&u1Data, 0x1);
    u1Data = 0;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x2534, (UINT8 *)&u1Data, 0x1);
    u1Data = 0;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x2534, (UINT8 *)&u1Data, 0x1);
    u1Data = 0;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x2534, (UINT8 *)&u1Data, 0x1);
    u1Data = 0;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x2534, (UINT8 *)&u1Data, 0x1);
    u1Data = 0;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x2534, (UINT8 *)&u1Data, 0x1);
    u1Data = 0;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x2534, (UINT8 *)&u1Data, 0x1);
    u1Data = 0;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x2534, (UINT8 *)&u1Data, 0x1);
    u1Data = 0;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x2534, (UINT8 *)&u1Data, 0x1);
    u1Data = 0;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x2534, (UINT8 *)&u1Data, 0x1);
    u1Data = 0;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x2534, (UINT8 *)&u1Data, 0x1);
    u1Data = 0x10;
    SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0xff, (UINT8 *)&u1Data, 0x1);

   LOG(2, "Set mjc setting effect=%d, 60/120=%d demo mode=%d\n", u1Data2, u1Data3, u1Data4);

	return 0;
}
#endif

#ifndef DRV_SUPPORT_MT8283
INT32 i4CmdExtmjcSflashProt(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 ui1enable;
    UINT8 ui1_err;

    if(i4Argc != 2)
    {
        return -1024;
    }

    ui1enable=StrToInt(szArgv[1]);

    if(ui1enable)
    {
       drv_extmjc_sflash_ProtEn(&ui1_err);
    }
    else
    {
	drv_extmjc_sflash_ProtDis(&ui1_err);
    }

    return 0;
}
#endif
INT32 i4CmdExtmjcAccessPasswd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 ui1enable;
    UINT8 ui1_err;

    if(i4Argc != 2)
    {
        return -1024;
    }

    ui1enable=StrToInt(szArgv[1]);

    if(ui1enable)
    {
       drv_extmjc_i2c_access_enable_password(&ui1_err);
    }
    else
    {
	drv_extmjc_i2c_access_disable_password(&ui1_err);
    }

	return 0;
}

INT32 i4CmdExtmjcAccessableGroup(INT32 i4Argc, const CHAR **szArgv)
{

    UINT16 ui2_accessable_group;
    UINT8 ui1_err;

    if(i4Argc != 2)
    {
        return -1024;
    }

#if 0
0128[0]  =>  osd
0128[1]  =>  ckgen
0128[2]  =>  scpos
0128[3]  =>  mjc
0128[4]  =>  io_cntl
0128[5]  =>  dram
0128[6]  =>  pscan
0128[7]  =>  bim
0129[0]  => sflash
0129[1]  => rs232
0129[2:7]  => reserve
#endif

	ui2_accessable_group=StrToInt(szArgv[1]);
	drv_extmjc_i2c_accessable_group(ui2_accessable_group, &ui1_err);

	return 0;
}


#ifdef DRV_SUPPORT_MT8283
INT32 _u4I2C8280Read(INT32 i4Argc, const CHAR **szArgv)
{
    UINT16 u2_address;
    //UINT8 u1_data;
    UINT32 u4_data;


    if(i4Argc != 2)
    {
        return -1024;
    }

    u2_address=StrToInt(szArgv[1]);
    //u1ExtmjcReadRegByI2C(u2_address, &u1_data);
    //Printf("0x%04x:0x%02x\n",u2_address,u1_data);
    u1ExtmjcReadRegByI2C(u2_address, (UINT8*)(&u4_data));
    Printf("0x%04x:0x%08x\n",u2_address,u4_data);
    return 0;
}

INT32 _u4I2C8280Write(INT32 i4Argc, const CHAR **szArgv)
{
    UINT16 u2_address;
    //UINT8 u1_data;
    UINT32 u4_data;

    if(i4Argc != 3)
    {
        return -1024;
    }

    u2_address=StrToInt(szArgv[1]);
    //u1_data=StrToInt(szArgv[2]);
    //u1ExtmjcWriteRegByI2C(u2_address, u1_data);
    u4_data=StrToInt(szArgv[2]);
    u1ExtmjcWriteRegByI2C(u2_address, u4_data);
    return 0;
}
#else
INT32 _u4I2C8280Read(INT32 i4Argc, const CHAR **szArgv)
{
    UINT16 u2_address;
    UINT8 u1_data;

    if(i4Argc != 2)
    {
        return -1024;
    }

    u2_address=StrToInt(szArgv[1]);
    u1ExtmjcReadRegByI2C(u2_address, &u1_data);
    Printf("0x%04x:0x%02x\n",u2_address,u1_data);
    return 0;
}

INT32 _u4I2C8280Write(INT32 i4Argc, const CHAR **szArgv)
{
    UINT16 u2_address;
    UINT8 u1_data;

    if(i4Argc != 3)
    {
        return -1024;
    }

    u2_address=StrToInt(szArgv[1]);
    u1_data=StrToInt(szArgv[2]);
    u1ExtmjcWriteRegByI2C(u2_address, u1_data);
    return 0;
}
#endif

#ifndef DRV_SUPPORT_MT8283
INT32 _u4I2CSfCrc(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4_size;

    if(i4Argc != 2)
    {
        printf("Flash DMA to DRAM CRC value\n");
        printf("arg [DMA length]\n");
        return -1024;
    }
    printf("Flash DMA to DRAM CRC must in HW trap mode\n");
    u4_size =StrToInt(szArgv[1]);
    printf("\n SFLASH CRC=%08X \n",u4ExtmjcGetSfCrcChecksum(u4_size));
    return 0;
}
INT32 _u4UartPing(INT32 i4Argc, const CHAR **szArgv)
{
#if 0    
    UINT8 u1_port;
    UINT32 u4_ret;
    UINT32 u4_pingtimes,u4_pingcount,ok_times,fail_times;
    UINT8 u1_data;
    static UINT8 u1_uart_passwd[]={0x54,0x8a,0x51,0x37,0x56,0x92,0x1c,0xb2,0x00,0x13,0x01,0x00};//passwd,set mode,sed word length
    static UINT8 u1_uart_readxdata[]={0x50,0x00,0x50,0x02,0x50,0x01};//read xdata[0x0201]
    static UINT8 u1_uart_writexdata[]={0x50,0x80,0x50,0x02,0x50,0x01,0x50,0x00};//write xdata[0x0201]
    UINT8 u1_buffer[20];
    RS_232_SETUP_INFO_T rUartSetupInfo;
    UINT32 time_out;

    time_out = 100000; //100ms
    if (i4Argc < 3)
    {
        Printf("uartping Port PingTimes [timeout]\n");
        return 1;
    }
    if (i4Argc == 4)
    {
        time_out = StrToInt(szArgv[3]);
    }
    u1_port = StrToInt(szArgv[1]);
    u4_pingtimes = StrToInt(szArgv[2]);
    if (u1_port >= UART_PORT_NUM)
    {
        Printf("Arg: wrong port number.\n");
        return 1;
    }

    rUartSetupInfo.e_speed = RS_232_SPEED_115200;
    rUartSetupInfo.e_stop_bit = (RS_232_STOP_BIT_T)RS_232_STOP_BIT_1;
    rUartSetupInfo.e_parity = (RS_232_PARITY_T)RS_232_PARITY_NONE;
    rUartSetupInfo.e_data_len = (RS_232_DATA_LEN_T)RS_232_DATA_LEN_8;
    // set port parameter.
    UART_SetParameter(u1_port, (const RS_232_SETUP_INFO_T *)&rUartSetupInfo);
    u4_ret = UART_Write(u1_port, u1_uart_passwd, sizeof(u1_uart_passwd), TRUE);
    if (u4_ret != sizeof(u1_uart_passwd))
    {
        Printf("Uart PASSWD write failed, write length = %d.\n", u4_ret);
        return -1;
    }
    u1_data = 0;
    ok_times =0;
    fail_times =0;
    for(u4_pingcount=1;u4_pingcount<=u4_pingtimes;u4_pingcount++,u1_data++)
    {
        Printf("UartPing Total=%d,Count=%d ",u4_pingtimes,u4_pingcount);
        u1_uart_writexdata[7] = u1_data;
        u4_ret = UART_Write(u1_port, u1_uart_writexdata, sizeof(u1_uart_writexdata), TRUE);
        if (u4_ret != sizeof (u1_uart_writexdata))
        {
            fail_times++;
            Printf("=>1.MT53xx Write Failed(%d)\n",u4_ret);
            continue;
        }
        HAL_Delay_us(time_out);
        u4_ret = UART_Read(u1_port, u1_buffer, 20, FALSE);
        u4_ret = UART_Write(u1_port, u1_uart_readxdata, sizeof(u1_uart_readxdata), TRUE);
        if (u4_ret != sizeof (u1_uart_readxdata))
        {
            fail_times++;
            Printf("=>2.MT53xx Write Failed(%d)\n",u4_ret);
            continue;
        }
        HAL_Delay_us(time_out);
        u4_ret = UART_Read(u1_port, u1_buffer, 20, FALSE);
        if (u4_ret!=2)
        {
            fail_times++;
            Printf("=>3.MT53xx Read Failed(%d)\n",u4_ret);
            continue;
        }
        if (u1_buffer[0]!=1 && u1_buffer[1]!=u1_data)
        {
            fail_times++;
            Printf("=>4.MT82xx Read Failed([0]=%02x,[1]=%02x,u1_data=%02x\n",u1_buffer[0],u1_buffer[1],u1_data);
            continue;
        }
        ok_times++;
        Printf("Successed \n");
    }
    Printf("ok_times=%d,fail_times=%d\n",ok_times,fail_times);
#endif 
    return 0;
}
INT32 _u4Delayms(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4_delay;
    if (i4Argc != 2)
    {
        Printf("delay [ms]\n");
        return -1024;
    }
    u4_delay = StrToInt(szArgv[1]);
    HAL_Delay_us(u4_delay*1000);
    return 0;
}
INT32 _u4SLTPower(INT32 i4Argc, const CHAR **szArgv)
{
    UINT16 u2_pwr;
    UINT8 ui1_err;
    UINT32 u4_pwr_rst_delay_ms; //power to reset delay
    UINT32 u4_rst_cmd_delay_ms; //reset to send i2c command delay

    u4_pwr_rst_delay_ms = 20; //reset pin need delay 20ms after power on
    u4_rst_cmd_delay_ms = 4000; //wait 4 sec for mt8280 capture ready
    if (i4Argc == 3)
    {
        u4_pwr_rst_delay_ms = StrToInt(szArgv[2]);
    }
    else if (i4Argc == 4)
    {
        u4_pwr_rst_delay_ms = StrToInt(szArgv[2]);
        u4_rst_cmd_delay_ms = StrToInt(szArgv[3]);
    }
    else if (i4Argc != 2)
    {
        Printf("power_on [pwr_rst_delay ms] [rst_cmd_delay ms]\n");
        Printf("    0(Power Off)\n");
        Printf("    1(Power On)\n");
        return -1024;
    }
    PMX_ShowPattern(TRUE); //show mt53xx ostg pattern gen
    u2_pwr = StrToInt(szArgv[1]);
    if (u2_pwr)
    {
        GPIO_SetOut(8,0); //Power On MT8282 5V
        GPIO_SetOut(6,0); //Power On MT8280_0 5V
        GPIO_SetOut(7,0); //Power On MT8280_1 5V
        HAL_Delay_us(u4_pwr_rst_delay_ms*1000);
        GPIO_SetOut(2,1); //Release Reset MT8282
        GPIO_SetOut(0,1); //Release Reset MT8280_0
        GPIO_SetOut(1,1); //Release Reset MT8280_1
        HAL_Delay_us(((u4_rst_cmd_delay_ms*1000)/2));
        drv_extmjc_i2c_access_enable_password(&ui1_err);
        drv_extmjc_i2c_accessable_group(0xffff, &ui1_err);
        u1ExtmjcWriteRegByI2C(0x55,0x02);//cheange to Mode2
        HAL_Delay_us(((u4_rst_cmd_delay_ms*1000)/2));
    }
    else
    {
        GPIO_SetOut(2,0); //Hold Reset MT8282
        GPIO_SetOut(0,0); //Hold Reset MT8280_0
        GPIO_SetOut(1,0); //Hold Reset MT8280_1
        HAL_Delay_us(u4_pwr_rst_delay_ms*1000);
        GPIO_SetOut(8,1); //Power Off MT8282 5V
        GPIO_SetOut(6,1); //Power Off MT8280_0 5V
        GPIO_SetOut(7,1); //Power Off MT8280_1 5V
    }
    return 0;
}
INT32 _u4ResetLoop(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 ui1_err;
    UINT32 loop,max_loop,fail_count,ok_count;

    max_loop=0xffffffff;
    if(i4Argc == 2)
    {
        max_loop = StrToInt(szArgv[1]);
    }
    else if (i4Argc>2)
    {
        Printf("usage:%s [loop count]\n",szArgv[0]);
        return -1024;
    }
    ok_count = 0;
    for(loop=1;loop<=max_loop;loop++)
    {
        GPIO_SetOut(22,0); //Power Off MT8282
        Printf("MT8281 Power Off\n");
        HAL_Delay_us(20000);
        GPIO_SetOut(22,1); //Power On
        Printf("MT8281 Power On\n");
        for(;;) //dummy read,ensure MT8281 is ready
        {
        	if (u1ExtmjcReadRegByI2C(0x55, &ui1_err)==0)
        		break;
            HAL_Delay_us(20000);
        }
        drv_extmjc_i2c_access_enable_password(&ui1_err);
        drv_extmjc_i2c_accessable_group(0xffff, &ui1_err);
        HAL_Delay_us(1000000);
        u1ExtmjcWriteRegByI2C(0x55,0x02);
        HAL_Delay_us(1000000);
        Printf("MT8281 Mode2\n");
        while(1)
        {
            u1ExtmjcReadRegByI2C(0x55, &ui1_err);
            if (ui1_err == 0xff)
                break;
            HAL_Delay_us(10000);
        }
        Printf("MT8281 Checking\n");
        HAL_Delay_us(3000000);
        u1ExtmjcWriteRegByI2C(0x55,0xd0);
        fail_count = 0;
        while(1)
        {
            ui1_err = 0;
            u1ExtmjcReadRegByI2C(0x55, &ui1_err);
            if (ui1_err == 0xd1 || ui1_err==0xd2)
                break;
            HAL_Delay_us(20000);
            fail_count++;
            if (fail_count>=1000)
            {
                Printf("MT8281 fail to read check status\n");
                break;
            }
        }
        if (ui1_err == 0xd1)
        {
            Printf("Check failed\n");
            break;
        }
        else if (ui1_err == 0xd2)
        {
            ok_count++;
            Printf("Check Ok\n");
        }
        Printf("Loop test Count=%d Ok_count=%d\n",loop,ok_count);
    }
    return 0;
}

VOID extmjc_8280_uart_send(
    UINT8*                                  pui1_data,
    UINT32                                  ui4_data_size)
{
#if 0    
    UINT32 ui4_ttl_send_data_size;
    UINT32 ui4_send_data_size;

    ui4_ttl_send_data_size = 0;

    while (ui4_ttl_send_data_size != ui4_data_size)
    {
        ui4_send_data_size = UART_Write(1,
                                        pui1_data + ui4_ttl_send_data_size,
                                        ui4_data_size - ui4_ttl_send_data_size,
                                        TRUE);

        ui4_ttl_send_data_size += ui4_send_data_size;
    }
#endif
}
INT32 extmjc_8280_uart_read(UINT8 *ui1_data)
{
#if 0    
    HAL_TIME_T  t_org_time, t_new_time, t_time_diff;

    HAL_GetTime(&t_org_time);

    while (TRUE)
    {
        /* Get data */
        if (0 != UART_Read(1, ui1_data, 1, FALSE))
        {
            return 0;
        }

        HAL_GetTime(&t_new_time);
        HAL_GetDeltaTime(&t_time_diff, &t_org_time, &t_new_time);
        if ((t_time_diff.u4Micros > 50000) || (t_time_diff.u4Seconds > 1))
        {
            break;
        }
    }
#endif
    return -1;
}

INT32 _u48281uartwrite(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 i;
    UINT8 u1_data[64];
    if (i4Argc<2)
        return -1024;

    Printf("\n");
    for (i=1;i<i4Argc;i++)
    {
        u1_data[i]=StrToInt(szArgv[i]);
    }
    extmjc_8280_uart_send(&u1_data[1],i4Argc-1);
    while (extmjc_8280_uart_read(&u1_data[0]) == 0)
    {
        Printf(" 0x%02x",u1_data[0]);
    }
    return 0;
}
INT32 _u4I2C8281SfUpgrade(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 offset,datalen;
    INT32 size;

    if (FALSE == drv_extmjc_is_enable())
    {
        Printf("drv_extmjc_is_enable()=%d \n",drv_extmjc_is_enable());
        return -1024;
    }
    Printf("d_custom_extmjc_uart_upgrade_open()=%d\n",d_custom_extmjc_upgrade_open());
    offset = 0;
    size = 0x10000;
#ifndef DRV_SUPPORT_MT8283
    Printf("d_custom_extmjc_uart_upgrade_erase()=%d\n",d_custom_extmjc_upgrade_erase());
#else
    Printf("d_custom_extmjc_uart_upgrade_erase()=%d\n",d_custom_extmjc_upgrade_erase(offset, size));
#endif
    while(size>0)
    {
        datalen = 128;
        if ((offset & 0x00fff)==0)
            Printf("Extmjc Write Offset=0x%08x\n",offset);
        d_custom_extmjc_upgrade_write(offset,pcodebuffer1+offset,datalen);
        offset = offset+datalen;
        size = size - datalen;
    }
    Printf("d_custom_extmjc_uart_upgrade_close()=%d\n",d_custom_extmjc_upgrade_close());
    return 0;
}
#endif

INT32 i4CmdExtmjcInit(INT32 i4Argc, const CHAR **szArgv)
{
    drv_extmjc_init();
    return 0;
}

INT32 i4CmdGetMjcDram(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 ui1_err;
    INT32 i, j, t;
    INT32 I, J;
    INT32 ii, jj;
    UINT32 W, U, W1, U1, BW;
    UINT32 u4Base = 0x69180;
    UINT32 u4BaseCO = 0x243B40;
    UINT32 u4Addr;
    UINT32 u4Data;
    UINT16 Y, C;

    BW = 120;
    U = 5*3*8;
    U1 = 5*8;

    W = U*BW;
    W1 = U1*BW;

    if (i4Argc != 4)
        Printf("usage: %s i j t\n", szArgv[0]);

    i = StrToInt(szArgv[1]);
    j = StrToInt(szArgv[2]);
    t = StrToInt(szArgv[3]);

    I = (i>>1) >> 2;
    J = j >> 4;

    // offset of blocks
    if ((I % 2) == 0)
    {
        u4Base += ((W * I) + (U * (J%BW)));
        if (I == 0)
        {
            u4BaseCO += (U1 * (J%BW));
        }
        else
        {
            u4BaseCO += ((W1 * (I+1)) - (U1 * ((J+1)%BW)));
        }
    }
    else
    {
        u4Base += ((W * (I+1)) - (U * ((J+1)%BW)));
        u4BaseCO += ((W1 * I) + (U1 * (J%BW)));
    }

    u4Base <<= 4;
    u4BaseCO <<= 4;

    ii = (i>>1) % 4;
    jj = j % 16;
    if ((i % 2) == 0) // YE, CE
    {
        // read YE LSB
        u4Addr = u4Base + ((5*3*t)<<4);
        d_custom_extmjc_get_dram(u4Addr, &u4Data, &ui1_err);
        Y = (u4Data >> ((15-jj)*2)) & 0x3;
        // read YE 8bit
        u4Addr = u4Base + ((5*3*t + ii + 1)<<4) + (jj>>2);
        d_custom_extmjc_get_dram(u4Addr, &u4Data, &ui1_err);
        Y |= (((u4Data >> ((3-(jj%4))*8)) & 0xff) << 2);
        // read CE LSB
        u4Addr = u4Base + ((5*3*t + 5)<<4);
        d_custom_extmjc_get_dram(u4Addr, &u4Data, &ui1_err);
        C = (u4Data >> ((15-jj)*2)) & 0x3;
        // read CE 8bit
        u4Addr = u4Base + ((5*3*t + 5 + ii + 1)<<4) + (jj>>2);
        d_custom_extmjc_get_dram(u4Addr, &u4Data, &ui1_err);
        C |= (((u4Data >> ((3-(jj%4))*8)) & 0xff) << 2);
    }
    else
    {
        // read YO LSB
        u4Addr = u4Base + ((5*3*t + 10)<<4);
        d_custom_extmjc_get_dram(u4Addr, &u4Data, &ui1_err);
        Y = (u4Data >> ((15-jj)*2)) & 0x3;
        // read YO 8bit
        u4Addr = u4Base + ((5*3*t + 10 + ii + 1)<<4) + (jj>>2);
        d_custom_extmjc_get_dram(u4Addr, &u4Data, &ui1_err);
        Y |= (((u4Data >> ((3-(jj%4))*8)) & 0xff) << 2);
        // read CO LSB
        u4Addr = u4BaseCO + ((5*1*t)<<4);
        d_custom_extmjc_get_dram(u4Addr, &u4Data, &ui1_err);
        C = (u4Data >> ((15-jj)*2)) & 0x3;
        // read CO 8bit
        u4Addr = u4BaseCO + ((5*1*t + ii + 1)<<4) + (jj>>2);
        d_custom_extmjc_get_dram(u4Addr, &u4Data, &ui1_err);
        C |= (((u4Data >> ((3-(jj%4))*8)) & 0xff) << 2);
    }

    Printf("Y = %d, C = %d\n", Y, C);

    return 0;
}

#ifdef DRV_SUPPORT_MT8283
INT32 i4CmdExtmjcWriteGamma(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4SrcAddr,u4size;
    UINT8 u1Idx;

    if(i4Argc != 4)
    {
        return -1024;
    }

    u4SrcAddr=StrToInt(szArgv[1]);
    u4size=StrToInt(szArgv[2]);
    u1Idx=StrToInt(szArgv[3]);

    Printf("**Write gamma from 0x%x with size %d to %d\n",u4SrcAddr,u4size,u1Idx);
    drv_extmjc_gamma_write((UINT8*)u4SrcAddr,u4size,u1Idx);

	return 0;
}

INT32 i4CmdExtmjcSetPWM(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Src,u1Ratio,u1Start,u1High,u1All,ret;

    if(i4Argc != 6)
    {
        return -1024;
    }

    u1Src=StrToInt(szArgv[1]);
    u1Ratio=StrToInt(szArgv[2]);
    u1Start=StrToInt(szArgv[3]);
    u1High=StrToInt(szArgv[4]);
    u1All=StrToInt(szArgv[5]);

    drv_extmjc_set_PWM(u1Src,u1Ratio,u1Start,u1High,u1All,&ret);

	return 0;
}

INT32 i4CmdExtmjcSetCSHDModeCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode,ret;
    UINT16 u2Overscan;

    if(i4Argc != 3)
    {
        return -1024;
    }

    u1Mode=(UINT8)StrToInt(szArgv[1]);
    u2Overscan=(UINT16)StrToInt(szArgv[2]);
    
    drv_extmjc_set_CSHD_mode(u1Mode,u2Overscan,&ret);

	return 0;
}

INT32 i4CmdExtmjcSetMirrorFlipCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode,ret;

    if(i4Argc != 2)
    {
        return -1024;
    }

    u1Mode=(UINT8)StrToInt(szArgv[1]);
    
    drv_extmjc_set_MJC_MirrorFlip(u1Mode,&ret);

	return 0;
}

INT32 i4CmdExtmjcSetPanelSequenceCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;

    if(i4Argc != 2)
    {
        return -1024;
    }

    u1Mode=(UINT8)StrToInt(szArgv[1]);
    
    drv_extmjc_setPanelPowerSequence((BOOL)u1Mode);

	return 0;
}

INT32 i4CmdExtmjcCustomCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Para0=0,u4Para1=0;
    UINT8 ret;

    if(i4Argc != 3)
    {
        return -1024;
    }

    u4Para0=StrToInt(szArgv[1]);
    u4Para1=StrToInt(szArgv[2]);

    //Printf("extmjc custom cmd:para0(%d),para1(%d)\n",u4Para0,u4Para1);
    drv_extmjc_custom_cmd(u4Para0,u4Para1,&ret);

	return 0;
}

#endif
//=====================================================================
// CLI command table
//=====================================================================

static CLI_EXEC_T _arExtmjcCmdTbl[] =
{
    // LOG macro support
#ifdef DRV_SUPPORT_MT8283
 	{"init", "i", i4CmdExtmjcInit, NULL, "i", CLI_GUEST},
	{"setping", "ping", i4CmdSetPing, NULL, "ping", CLI_GUEST},
	{"setgamma", "setga", i4CmdSetGamma, NULL, "setgamma gamma", CLI_GUEST},
	{"setosdenable", "osdenable", i4CmdSetOSDEnable, NULL, "osdenable 0(off)/1(on)", CLI_GUEST},
	{"setosdmode", "osdmode", i4CmdSetOSDMode, NULL, "osdenable 0(DMA)/1(Graphic)", CLI_GUEST},
	{"setvideofmt", "setvf", i4CmdSetVideoFmt, NULL, "setvf colordomain framerate HTotal VTotal", CLI_GUEST},
	{"8281passwd", "8281passwd", i4CmdExtmjcAccessPasswd, NULL, "mjc i2c access enable/disable passwd", CLI_GUEST},
	{"8281accessablegroup", "8281accessablegroup", i4CmdExtmjcAccessableGroup, NULL, "mjc i2c access group control", CLI_GUEST},
	{"sifread", "sifr", _u4I2C8280Read, NULL, "Mt8280 i2c read", CLI_GUEST},
	{"sifwrite", "sifw",_u4I2C8280Write, NULL, "Mt8280 i2c write", CLI_GUEST},
	{"mjcdram", "mjcrd", i4CmdGetMjcDram, NULL, "8281 get mjc dram", CLI_SUPERVISOR},
	{"wrtgamma", "wgamma", i4CmdExtmjcWriteGamma, NULL, "wrtie gamma test", CLI_GUEST},
        {"setpwm", "pwm", i4CmdExtmjcSetPWM, NULL, "Set PWM", CLI_GUEST},
        {"setCSHD", "cshd", i4CmdExtmjcSetCSHDModeCmd, NULL, "Set CSHD mode", CLI_GUEST},
        {"setmf", "setmf", i4CmdExtmjcSetMirrorFlipCmd, NULL, "Set mirror flip", CLI_GUEST},
        {"setpps", "setpps", i4CmdExtmjcSetPanelSequenceCmd, NULL, "Set panel power sequence", CLI_GUEST},
        {"custom", "cust", i4CmdExtmjcCustomCmd, NULL, "custom command", CLI_GUEST},
   #else
	{"init", "i", i4CmdExtmjcInit, NULL, "i", CLI_GUEST},
	{"setping", "ping", i4CmdSetPing, NULL, "ping", CLI_GUEST},
	{"setcolortempex", "setcte", i4CmdSetColorTempEx, NULL, "setct idx(0~15 + 0xFF) value", CLI_GUEST},
	{"setgamma", "setga", i4CmdSetGamma, NULL, "setgamma gamma", CLI_GUEST},
	{"getver", "getver", i4CmdGetVer, NULL, "getver", CLI_GUEST},
	{"setwinpos", "setwp", i4CmdSetWinPos, NULL, "setwp lt=(%d,%d) br=(%d,%d)", CLI_GUEST},
	{"setlvds", "lvds", i4CmdSetLVDS, NULL, "setlvds 0(disable)/1(enable)", CLI_GUEST},
	{"setgpio", "setgpio", i4CmdSetGPIOTest, NULL, "setiot id 0(low)/1(high)", CLI_GUEST},
	{"setcolorpattern", "setctp", i4CmdSetColorPatternTest, NULL, "setctp 0/1/2/3/4/0xff", CLI_GUEST},
	{"setosdenable", "osdenable", i4CmdSetOSDEnable, NULL, "osdenable 0(off)/1(on)", CLI_GUEST},
	{"setosdmode", "osdmode", i4CmdSetOSDMode, NULL, "osdenable 0(DMA)/1(Graphic)", CLI_GUEST},
	{"setosdscale", "osdscale", i4CmdSetOSDInfo, NULL, "osdscale 0(off)/1(on) width height", CLI_GUEST},
	{"read", "r", i4CmdReadSram, NULL, "read ui2_extmjc_reg_addr", CLI_GUEST},
	{"write", "w", i4CmdWriteSram, NULL, "read ui2_extmjc_reg_addr ui4_Data", CLI_GUEST},
	{"readdram", "rd", i4CmdGetDramTest, NULL, "getdram addr", CLI_GUEST},
	{"writedram", "wd", i4CmdSetDramTest, NULL, "setdram addr value", CLI_GUEST},
	{"setmjc", "setmjc", i4CmdSetMJCMode, NULL, "setmode effect 60or120 demo", CLI_GUEST},
	{"setvideofmt", "setvf", i4CmdSetVideoFmt, NULL, "setvf colordomain framerate HTotal VTotal", CLI_GUEST},
	{"8280 init", "8280i", _u18280Init, NULL, "8280 initialization", CLI_GUEST},
	{"alloc dram", "alloc", _u45382AllocDram, NULL, "allocate code buffer in mt5382p", CLI_GUEST},
	{"dump dram", "dump", _u45382DumpDram, NULL, "dump code buffer in mt5382p", CLI_GUEST},
	{"download", "download", _u45382DownloadDram, NULL, "download code buffer to mt8280", CLI_GUEST},
	{"downloaddma", "downloaddma", _u45382DownloadDramDma, NULL, "download code buffer to mt8280 by I2C DMA", CLI_GUEST},
	{"downloadsf", "downloadsf", _u45382DownloadFlash, NULL, "download code buffer to mt8280", CLI_GUEST},
	{"compare", "compare", _u45382CompareDram, NULL, "compare mt5382's code buffer with mt8280's", CLI_GUEST},
	{"trigger", "trigger", _u4Trigger8280, NULL, "trigger mt8280 to run", CLI_GUEST},
	{"idma", "idma", _u4I2CDma8280, NULL, "i2c DMA mt8280", CLI_GUEST},
	{"sflashprot", "sflashprot", i4CmdExtmjcSflashProt, NULL, "mjc sflash protect enable/disable", CLI_GUEST},
	{"8281passwd", "8281passwd", i4CmdExtmjcAccessPasswd, NULL, "mjc i2c access enable/disable passwd", CLI_GUEST},
	{"8281accessablegroup", "8281accessablegroup", i4CmdExtmjcAccessableGroup, NULL, "mjc i2c access group control", CLI_GUEST},
	{"sifread", "sifr", _u4I2C8280Read, NULL, "Mt8280 i2c read", CLI_GUEST},
	{"sifwrite", "sifw",_u4I2C8280Write, NULL, "Mt8280 i2c write", CLI_GUEST},
	{"sfcrc", "sfcrc",_u4I2CSfCrc, NULL, "Must in HW trap mode,sflash DMA to DRAM CRC value", CLI_GUEST},
	{"uartping","uartping",_u4UartPing, NULL, "MT5392b to MT8281 UART ping", CLI_GUEST},
	{"delay","delay",_u4Delayms, NULL, "Delay in ms", CLI_GUEST},
	{"sltpower","sltpwr",_u4SLTPower, NULL, "SLT board MT82xx Power On/Off", CLI_SUPERVISOR},
	{"resetloop","resetloop",_u4ResetLoop, NULL, "Demo board Test", CLI_SUPERVISOR},
	{"sfupgrade","sfupgrade",_u4I2C8281SfUpgrade, NULL, "SF upgrade", CLI_SUPERVISOR},
	{"uartwrite","uartw",_u48281uartwrite, NULL, "8281 uart write", CLI_SUPERVISOR},
	{"mjcdram", "mjcrd", i4CmdGetMjcDram, NULL, "8281 get mjc dram", CLI_SUPERVISOR},
	#endif
	#if 0
	{
		"setpip", "setpip", i4CmdSetPip, NULL, "setpip mode main-lt=(x, y) main-rb=(x, y) sub-lt=(x, y) sub-rb=(x, y)", CLI_GUEST
	},
	{
		"setdemomode", "setdm", i4CmdSetDemoMode, NULL, "setdm 0/1/2/3/4/5/6", CLI_GUEST
	},
	{
		"getgpio", "getiot", i4CmdGetGPIOTest, NULL, "getiot id", CLI_GUEST
	},
	#endif

	{
		NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
	}
};


CLI_MAIN_COMMAND_ITEM(Extmjc)
{
    "extmjc", "extmjc", NULL, _arExtmjcCmdTbl, "EXTMJC commands", CLI_GUEST
};

#else

static CLI_EXEC_T _arExtmjcCmdTbl[] =
{
	{
		NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
	}
};

CLI_MAIN_COMMAND_ITEM(Extmjc)
{
    "extmjc", "extmjc", NULL, _arExtmjcCmdTbl, "EXTMJC commands", CLI_GUEST
};

#endif

