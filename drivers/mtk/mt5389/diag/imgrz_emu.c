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
 * $RCSfile: imgrz_emu.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file imgrz_emu.c
 *  This file contains implementation of CLI CMD for IMGRZ emulation.
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

#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "drvcust_if.h"

#include <stdarg.h>

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

#if defined(IMGRZ_ENABLE_SW_MODE)
#include "drv_t32.h"
#ifndef JPG_SEMI_HOST
#define JPG_SEMI_HOST
#endif

//#include "gfx_emu_ui_tex.c"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define D_PAUSE_DELAY   1000
#define D_FRAME_DELAY   33
#define D_RZ_TOTAL_DIR_CM   4

#define RZ_EMU_OSD_SCREEN_BUF   (_u4RzEmuBuf[0])       // OnScreen
#define RZ_EMU_COMPARE_BUF      (_u4RzEmuBuf[1])       // for C-model
#define RZ_EMU_OSD_SRC_BUF      (_u4RzEmuBuf[2])

//#define IMGRZ_TIME_PROFILE
//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

#ifdef IMGRZ_AUTO_TEST
HAL_TIME_T rRZStart, rRZEnd, rRZdif;

typedef enum
{
 E_IMGRZ_VERIFY_MULTI_SET,
 E_IMGRZ_VERIFY_OFFSET,
 E_IMGRZ_VERIFY_BURST_LIMIT,
 E_IMGRZ_VERIFY_H_FILTER,
 E_IMGRZ_VERIFY_V_FILTER,
 E_IMGRZ_VERIFY_TIME_PROFILE,
 E_IMGRZ_VERIFY_BREAK_RESUME,
 E_IMGRZ_VERIFY_NEWBUF_CFG,
 E_IMGRZ_VERIFY_BUFFER_CFG,
 E_IMGRZ_VERIFY_MULTI_INIT=100,
 E_IMGRZ_VERIFY_SHOW_HELP=200
}E_IMGRZ_VERIFY_PARAM;

typedef struct
{
  INT32 arImgrzOffSet[3][2];
  INT32 i4ImgrzBustLimit;
  INT32 i4ImgRzFilterH;
  INT32 i4ImgRzFilterV;
  INT32 i4GetRzTimeProfile;
  INT32 i4RzBreakResumeTest;
  INT32 i4RZNewBufCfg;
}T_IMGRZ_VERIFY_PARAM;

typedef struct
{
  UINT32 u4InputAddr1;
  UINT32 u4InputAddr2;
  UINT32 u4OutPutAddr1;
  UINT32 u4OutPutAddr2;
}T_BUF_CFG;

static T_IMGRZ_VERIFY_PARAM _ImgrzVerParam;
static T_BUF_CFG _rImgBufCfg;
static BOOL _fgErrHappen=FALSE;
typedef struct _AutoTestCase
{
  UCHAR *szName;
  UCHAR *szCmd;
  BOOL fg_load;
  UCHAR *szLoadPath;
  UCHAR *szLoadFile;
  UINT32 u4LoadAddr;
  BOOL fg_save;
  UCHAR *szSavePath;
  UCHAR *szSaveFile;
  UINT32 u4SaveStartAddr;
  UINT32 u4SaveSize;
}T_AUTO_TEST_CASE,*P_AUTO_TEST_CASE;

typedef struct _AutoTestSet
{
  UINT32 id;
  UCHAR *name;
  P_AUTO_TEST_CASE *pCaseSet;
  UINT32 arData[8];
}T_AUTO_TEST_SET,*P_AUTO_TEST_SET;

T_AUTO_TEST_CASE pAutoSetOsdAuto[]=
{
  {"loadSourcefile",NULL,TRUE,"D:\\jpg_test\\dump","lena_252x252.8888",0x29fc000,FALSE,NULL,NULL,0,0},
  {"osdatuotest","imgrzemu.auto 0x29fc000 252 251 0x5612000",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {NULL,NULL,NULL,FALSE,NULL,0,FALSE,0,0}
};

T_AUTO_TEST_CASE pAutoSetOsdSrcARGB8888[]=
{
	{"loadSourcefile",NULL,TRUE,"D:\\jpg_test\\dump","lena_252x252.8888",0x29fc000,FALSE,NULL,NULL,0,0},		
	
	{"osdmanargb88881","imgrzemu.osdman1  0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 400    0   0   100 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd1_argb8888_100_100.bin",0x5612000,40000},
	{"osdmanargb88882","imgrzemu.osdman1  0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 2000   0   0   500 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd2_argb8888_500_400.bin",0x5612000,800000},
	{"osdmanargb88883","imgrzemu.osdman1  0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 400    0   0   100 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd3_argb8888_100_400.bin",0x5612000,160000},
	{"osdmanargb88884","imgrzemu.osdman1  0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 2000   0   0   500 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd4_argb8888_500_100.bin",0x5612000,200000},
	{"osdmanargb88885","imgrzemu.osdman1  0x29fc000 1 3 1008 100  50   125 201 0x5612000 3 2000   0   0   500 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd5_argb8888_src_100_50_500_400.bin",0x5612000,800000},
	{"osdmanargb88886","imgrzemu.osdman1  0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 2000   200 100 300 300 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd6_argb8888_dst_200_100_500_400.bin",0x5612000,800000},
	{"osdmanargb88887","imgrzemu.osdman1  0x29fc000 1 3 1008 100  50   125 201 0x5612000 3 2000   200 100 300 300 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd7_argb8888_src_100_50_dst_200_100_500_400.bin",0x5612000,800000},
	{"osdmanargb88888","imgrzemu.osdman1  0x29fc000 1 3 1008 0    0    252 251 0x5612000 0 1024   0   0   512 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd8_argb8888_to_rgb565_512_400.bin",0x5612000,409600},
	{"osdmanargb88889","imgrzemu.osdman1  0x29fc000 1 3 1008 0    0    252 251 0x5612000 1 1024   0   0   512 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd9_argb8888_to_argb1555_512_400.bin",0x5612000,409600},
	{"osdmanargb888810","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 2 1024   0   0   512 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd10_argb8888_to_argb4444_512_400.bin",0x5612000,409600},
	{"osdmanargb888811","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008   0   0   252 251 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd11_argb8888_bypass_252_251.bin",0x5612000,252288},
	{"osdmanargb888812","imgrzemu.filter 1 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
	{"osdmanargb888813","imgrzemu.osdman1  0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 400    0   0   100 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd13_argb8888_H_acc_V_acc_100_100.bin",0x5612000,40000},
	{"osdmanargb888814","imgrzemu.filter 1 2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
	{"osdmanargb888815","imgrzemu.osdman1  0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 400   0   0    100 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd15_argb8888_H_acc_v_4tap_100_400.bin",0x5612000,160000},
    {"osdmanargb888816","imgrzemu.filter 2 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
	{"osdmanargb888817","imgrzemu.osdman1  0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 2000   0   0   500 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd17_argb8888_H_8tap_V_acc_500_400.bin",0x5612000,800000},
    {"osdmanargb888818","imgrzemu.filter 2 2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
	{"osdmanargb888819","imgrzemu.osdman1  0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 2000   0   0   500 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd19_argb8888_H_8tap_v_4tap_500_400.bin",0x5612000,800000},
    {"osdmanargb888820","imgrzemu.filter 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},

	#ifdef IMGRZ_HW_V4
	 //CSC test(Color space conversion), add 16 17 18 more three parameter 1. CSC enable 2. input format  3. output format (0 means ARGB8888, 1 means AYUV8888)
    {"osdmanargb888821","imgrzemu.osdman1  0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008    0   0   252 251 1 0 1 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd21_argb8888_to_ayuv8888_252_251.bin",0x5612000,253008},
	{"loadSourcefile",NULL,TRUE,"D:\\jpg_test\\dump","lena_252x251_AYUV.8888",0x29fc000,FALSE,NULL,NULL,0,0},
	//AYUV8888 to ARGB8888
	{"osdmanargb888822","imgrzemu.osdman1  0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008    0   0   252 251 1 1 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:/jpg_test/dump/upload","osd22_ayuv8888_to_argb8888_252_251.bin",0x5612000,253008},	
	#endif

	#ifdef IMGRZ_HW_V4
	//ARGB and RGBA conversion, add 19 parameter (0 means ARGB8888, 1 means RGBA8888)
	{"loadSourcefile",NULL,TRUE,"D:\\jpg_test\\dump","lena_252x252.8888",0x29fc000,FALSE,NULL,NULL,0,0},
	{"osdmanargb888823","imgrzemu.osdman1  0x29fc000 1 3 1008 0    0    252 251 0x5612000 6 1008    0   0   252 251 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd23_rgba8888_252_251.bin",0x5612000,253008},		
	{"osdmanargb888824","imgrzemu.osdman1  0x29fc000 1 3 1008 0    0    252 251 0x5612000 6 400    0   0   100 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd24_rgba8888_100_100.bin",0x5612000,40000},
	{"osdmanargb888825","imgrzemu.osdman1  0x29fc000 1 3 1008 0    0    252 251 0x5612000 6 2000   0   0   500 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd25_rgba8888_500_400.bin",0x5612000,800000},	
	#endif

	#if 0//def IMGRZ_HW_A_COMPOSITION, only when using 2 resizer for testing
	//add turbo mode enable, alpha composition and mode type
	{"loadSourcefile",NULL,TRUE,"D:\\jpg_test\\dump","chiling_252x251_alpha_ff.8888",0x29fc000,FALSE,NULL,NULL,0,0},
	{"loadSourcefile",NULL,TRUE,"D:\\jpg_test\\dump","lena_252x251_alpha_ff.8888",0xc000000,FALSE,NULL,NULL,0,0},	
	{"osdmanargb888827","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008   0   0   252 251 0 0 0 1 1 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd27_argb8888_bypass_OSDComp_Mode0_252_251.bin",0x5612000,253008},
	{"osdmanargb888828","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008   0   0   252 251 0 0 0 1 1 1",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd28_argb8888_bypass_OSDComp_Mode1_252_251.bin",0x5612000,253008},
	{"osdmanargb888829","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008   0   0   252 251 0 0 0 1 1 2",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd29_argb8888_bypass_OSDComp_Mode2_252_251.bin",0x5612000,253008},
	{"osdmanargb888830","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008   0   0   252 251 0 0 0 1 1 3",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd30_argb8888_bypass_OSDComp_Mode3_252_251.bin",0x5612000,253008},
	{"osdmanargb888831","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008   0   0   252 251 0 0 0 1 1 4",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd31_argb8888_bypass_OSDComp_Mode4_252_251.bin",0x5612000,253008},
	{"osdmanargb888832","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008   0   0   252 251 0 0 0 1 1 5",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd32_argb8888_bypass_OSDComp_Mode5_252_251.bin",0x5612000,253008},
	{"osdmanargb888833","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008   0   0   252 251 0 0 0 1 1 6",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd33_argb8888_bypass_OSDComp_Mode6_252_251.bin",0x5612000,253008},
	{"osdmanargb888834","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008   0   0   252 251 0 0 0 1 1 7",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd34_argb8888_bypass_OSDComp_Mode7_252_251.bin",0x5612000,253008},
	{"osdmanargb888835","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008   0   0   252 251 0 0 0 1 1 8",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd35_argb8888_bypass_OSDComp_Mode8_252_251.bin",0x5612000,253008},
	{"osdmanargb888836","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008   0   0   252 251 0 0 0 1 1 9",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd36_argb8888_bypass_OSDComp_Mode9_252_251.bin",0x5612000,253008},
	{"osdmanargb888837","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008   0   0   252 251 0 0 0 1 1 10",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd37_argb8888_bypass_OSDComp_Mode10_252_251.bin",0x5612000,253008},
	{"osdmanargb888838","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008   0   0   252 251 0 0 0 1 1 11",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd38_argb8888_bypass_OSDComp_Mode11_252_251.bin",0x5612000,253008},
	{"osdmanargb888839","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008   0   0   252 251 0 0 0 1 1 12",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd39_argb8888_bypass_OSDComp_Mode12_252_251.bin",0x5612000,253008},
	{"osdmanargb888840","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008   0   0   252 251 0 0 0 1 1 13",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd40_argb8888_bypass_OSDComp_Mode13_252_251.bin",0x5612000,253008},		

	{"loadSourcefile",NULL,TRUE,"D:\\jpg_test\\dump","chiling_252x251_alpha_random.8888",0x29fc000,FALSE,NULL,NULL,0,0},
	{"loadSourcefile",NULL,TRUE,"D:\\jpg_test\\dump","lena_252x251_alpha_random.8888",0xc000000,FALSE,NULL,NULL,0,0},
	
	{"osdmanargb888841","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008   0   0   252 251 0 0 0 1 1 7",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd41_argb8888_bypass_OSDComp_Mode7_252_251.bin",0x5612000,253008},
	{"osdmanargb888842","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 1008   0   0   252 251 0 0 0 1 1 8",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd42_argb8888_bypass_OSDComp_Mode8_252_251.bin",0x5612000,253008},
    //{"osdmanargb888843","imgrzemu.osdman1 0x29fc000 1 3 1008 0    0    252 251 0x5612000 3 7680   0   0 1920 1080 0 0 0 1 1 8",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd43_argb8888_bypass_OSDComp_Mode8_1920_1080.bin",0x5612000,8294400},
	#endif		
	{NULL,NULL,NULL,FALSE,NULL,0,FALSE,0,0}
};

T_AUTO_TEST_CASE pAutoSetOsdSrcARGB1555[]=
{
    {"loadSourcefile",NULL,TRUE,"D:\\jpg_test\\dump","osd_argb1555_320_300.bin",0x29fc000,FALSE,NULL,NULL,0,0},
	  {"osdmanargb15551","imgrzemu.osdman1  0x29fc000 1 1 640 0   0  320 300 0x5612000 1 256    0    0  128 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd1_argb1555_128_100.bin",0x5612000,25600},
	  {"osdmanargb15552","imgrzemu.osdman1  0x29fc000 1 1 640 0   0  320 300 0x5612000 1 1024   0    0  512 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd2_argb1555_512_400.bin",0x5612000,409600},
	  {"osdmanargb15553","imgrzemu.osdman1  0x29fc000 1 1 640 0   0  320 300 0x5612000 1 256    0    0  128 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd3_argb1555_128_400.bin",0x5612000,102400},
	  {"osdmanargb15554","imgrzemu.osdman1  0x29fc000 1 1 640 0   0  320 300 0x5612000 1 1024   0    0  512 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd4_argb1555_512_100.bin",0x5612000,102400},
	  {"osdmanargb15555","imgrzemu.osdman1  0x29fc000 1 1 640 100 50 220 250 0x5612000 1 1024   0    0  512 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd5_argb1555_src_100_50_512_400.bin",0x5612000,409600},
	  {"osdmanargb15556","imgrzemu.osdman1  0x29fc000 1 1 640 0   0  320 300 0x5612000 1 1024   200 100 312 300 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd6_argb1555_dst_200_100_512_400.bin",0x5612000,409600},
	  {"osdmanargb15557","imgrzemu.osdman1  0x29fc000 1 1 640 100 50 220 250 0x5612000 1 1024   200 100 312 300 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd7_argb1555_src_100_50_dst_200_100_512_400.bin",0x5612000,409600},
	  {"osdmanargb15558","imgrzemu.osdman1  0x29fc000 1 1 640 0   0  320 300 0x5612000 0 1024   0    0  512 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd8_argb1555_to_rgb565_512_400.bin",0x5612000,409600},
	  {"osdmanargb15559","imgrzemu.osdman1  0x29fc000 1 1 640 0   0  320 300 0x5612000 2 1024   0    0  512 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd9_argb1555_to_argb4444_512_400.bin",0x5612000,409600},
	  {"osdmanargb155510","imgrzemu.osdman1 0x29fc000 1 1 640 0   0  320 300 0x5612000 3 2048   0    0  512 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd10_argb1555_to_argb8888_512_400.bin",0x5612000,819200},
	  {"osdmanargb155511","imgrzemu.osdman1 0x29fc000 1 1 640 0   0  320 300 0x5612000 1 640    0    0  320 300 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd11_argb1555_bypass_320_300.bin",0x5612000,192000},
  
    {"osdmanargb155512","imgrzemu.filter 1 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
    {"osdmanargb155513","imgrzemu.osdman1  0x29fc000 1 1 640 0   0  320 300 0x5612000 1 256    0    0  128 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd13_argb1555_H_acc_V_acc_128_100.bin",0x5612000,25600},
    {"osdmanargb155514","imgrzemu.filter 1 2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
    {"osdmanargb155515","imgrzemu.osdman1  0x29fc000 1 1 640 0   0  320 300 0x5612000 1 256    0    0  128 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd15_argb1555_H_acc_V_4tap_128_400.bin",0x5612000,102400},
    {"osdmanargb155516","imgrzemu.filter 2 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
    {"osdmanargb155517","imgrzemu.osdman1  0x29fc000 1 1 640 0   0  320 300 0x5612000 1 800    0    0  400 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd17_argb1555_H_8tap_V_acc_400_400.bin",0x5612000,320000},
    {"osdmanargb155518","imgrzemu.filter 2 2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
    {"osdmanargb155519","imgrzemu.osdman1  0x29fc000 1 1 640 0   0  320 300 0x5612000 1 800    0    0  400 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd19_argb1555_H_8tap_V_4tap_400_400.bin",0x5612000,320000},
    {"osdmanargb155520","imgrzemu.filter 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
	  {NULL,NULL,NULL,FALSE,NULL,0,FALSE,0,0}
};

T_AUTO_TEST_CASE pAutoSetOsdSrcRGB565[]=
{
      {"loadSourcefile",NULL,TRUE,"D:\\jpg_test\\dump","osd_rgb565_400_200.bin",0x29fc000,FALSE,NULL,NULL,0,0},
	  {"osdmanrgb5651","imgrzemu.osdman1  0x29fc000 1 0 800 0   0  400 200 0x5612000 0 256    0    0  128 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd1_rgb565_128_100.bin",0x5612000,25600},
	  {"osdmanrgb5652","imgrzemu.osdman1  0x29fc000 1 0 800 0   0  400 200 0x5612000 0 1024   0    0  512 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd2_rgb565_512_400.bin",0x5612000,409600},
	  {"osdmanrgb5653","imgrzemu.osdman1  0x29fc000 1 0 800 0   0  400 200 0x5612000 0 256    0    0  128 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd3_rgb565_128_400.bin",0x5612000,102400},
	  {"osdmanrgb5654","imgrzemu.osdman1  0x29fc000 1 0 800 0   0  400 200 0x5612000 0 1024   0    0  512 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd4_rgb565_512_100.bin",0x5612000,102400},
	  {"osdmanrgb5655","imgrzemu.osdman1  0x29fc000 1 0 800 100 50 300 150 0x5612000 0 1024   0    0  512 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd5_rgb565_src_100_50_512_400.bin",0x5612000,409600},
	  {"osdmanrgb5656","imgrzemu.osdman1  0x29fc000 1 0 800 0   0  400 200 0x5612000 0 1024   200 100 312 300 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd6_rgb565_dst_200_100_512_400.bin",0x5612000,409600},
	  {"osdmanrgb5657","imgrzemu.osdman1  0x29fc000 1 0 800 100 50 300 150 0x5612000 0 1024   200 100 312 300 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd7_rgb565_src_100_50_dst_200_100_512_400.bin",0x5612000,409600},
	  {"osdmanrgb5658","imgrzemu.osdman1  0x29fc000 1 0 800 0   0  400 200 0x5612000 1 1024   0    0  512 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd8_rgb565_to_argb1555_512_400.bin",0x5612000,409600},
	  {"osdmanrgb5659","imgrzemu.osdman1  0x29fc000 1 0 800 0   0  400 200 0x5612000 2 1024   0    0  512 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd9_rgb565_to_argb4444_512_400.bin",0x5612000,409600},
	  {"osdmanrgb56510","imgrzemu.osdman1 0x29fc000 1 0 800 0   0  400 200 0x5612000 3 2048   0    0  512 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd10_rgb565_to_argb8888_512_400.bin",0x5612000,819200},
      {"osdmanrgb56511","imgrzemu.osdman1 0x29fc000 1 0 800 0   0  400 200 0x5612000 0 800    0    0  400 200 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd11_rgb565_bypass_400_200.bin",0x5612000,160000},
	
    {"osdmanrgb56512","imgrzemu.filter 1 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
	  {"osdmanrgb56513","imgrzemu.osdman1  0x29fc000 1 0 800 0   0  400 200 0x5612000 0 256    0    0  128 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd13_rgb565_H_acc_V_acc_128_100.bin",0x5612000,25600},
    {"osdmanrgb56514","imgrzemu.filter 1 2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
	  {"osdmanrgb56515","imgrzemu.osdman1  0x29fc000 1 0 800 0   0  400 200 0x5612000 0 256    0    0  128 200 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd15_rgb565_H_acc_V_4tap_128_200.bin",0x5612000,51200},
    {"osdmanrgb56516","imgrzemu.filter 2 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
	  {"osdmanrgb56517","imgrzemu.osdman1  0x29fc000 1 0 800 0   0  400 200 0x5612000 0 1200   0    0  600 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd17_rgb565_H_8tap_V_acc_600_400.bin",0x5612000,480000},
    {"osdmanrgb56518","imgrzemu.filter 2 2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
	  {"osdmanrgb56519","imgrzemu.osdman1  0x29fc000 1 0 800 0   0  400 200 0x5612000 0 1200   0    0  600 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd19_rgb565_H_8tap_V_4tap_600_400.bin",0x5612000,480000},
    {"osdmanrgb56520","imgrzemu.filter 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
	  {NULL,NULL,NULL,FALSE,NULL,0,FALSE,0,0}
};

T_AUTO_TEST_CASE pAutoSetOsdSrcARGB4444[]=
{
    {"loadSourcefile",NULL,TRUE,"D:\\jpg_test\\dump","osd_argb4444_400_200.bin",0x29fc000,FALSE,NULL,NULL,0,0},
	  {"osdmanargb44441","imgrzemu.osdman1   0x29fc000 1 2 800 0   0  400 200 0x5612000 2 256    0    0  128 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd1_argb4444_128_100.bin",0x5612000,25600},
	  {"osdmanargb44442","imgrzemu.osdman1   0x29fc000 1 2 800 0   0  400 200 0x5612000 2 1024   0    0  512 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd2_argb4444_512_400.bin",0x5612000,409600},
	  {"osdmanargb44443","imgrzemu.osdman1   0x29fc000 1 2 800 0   0  400 200 0x5612000 2 256    0    0  128 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd3_argb4444_128_400.bin",0x5612000,102400},
	  {"osdmanargb44444","imgrzemu.osdman1   0x29fc000 1 2 800 0   0  400 200 0x5612000 2 1024   0    0  512 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd4_argb4444_512_100.bin",0x5612000,102400},
	  {"osdmanargb44445","imgrzemu.osdman1   0x29fc000 1 2 800 100 50 300 150 0x5612000 2 1024   0    0  512 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd5_argb4444_src_100_50_512_400.bin",0x5612000,409600},
	  {"osdmanargb44446","imgrzemu.osdman1   0x29fc000 1 2 800 0   0  400 200 0x5612000 2 1024   200 100 312 300 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd6_argb4444_dst_200_100_512_400.bin",0x5612000,409600},
	  {"osdmanargb44447","imgrzemu.osdman1   0x29fc000 1 2 800 100 50 300 150 0x5612000 2 1024   200 100 312 300 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd7_argb4444_src_100_50_dst_200_100_512_400.bin",0x5612000,409600},
	  {"osdmanargb44448","imgrzemu.osdman1   0x29fc000 1 2 800 0   0  400 200 0x5612000 0 1024   0    0  512 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd8_argb4444_to_rgb565_512_400.bin",0x5612000,409600},
	  {"osdmanargb44449","imgrzemu.osdman1   0x29fc000 1 2 800 0   0  400 200 0x5612000 1 1024   0    0  512 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd9_argb4444_to_argb1555_512_400.bin",0x5612000,409600},
	  {"osdmanargb444410","imgrzemu.osdman1  0x29fc000 1 2 800 0   0  400 200 0x5612000 3 2048   0    0  512 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd10_argb4444_to_argb8888_512_400.bin",0x5612000,819200},
	  {"osdmanargb444411","imgrzemu.osdman1  0x29fc000 1 2 800 0   0  400 200 0x5612000 2 800    0    0  400 200 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd11_argb4444_bypass_400_200.bin",0x5612000,160000},

    {"osdmanargb444412","imgrzemu.filter 1 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
	  {"osdmanargb444413","imgrzemu.osdman1   0x29fc000 1 2 800 0   0  400 200 0x5612000 2 256    0    0  128 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd13_argb4444_H_acc_V_acc_128_100.bin",0x5612000,25600},
    {"osdmanargb444414","imgrzemu.filter 1 2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
    {"osdmanargb444415","imgrzemu.osdman1   0x29fc000 1 2 800 0   0  400 200 0x5612000 2 256    0    0  128 200 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd15_argb4444_H_acc_V_4tap_128_200.bin",0x5612000,51200},
	  {"osdmanargb444416","imgrzemu.filter 2 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
	  {"osdmanargb444417","imgrzemu.osdman1   0x29fc000 1 2 800 0   0  400 200 0x5612000 2 1200   0    0  600 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd17_argb4444_H_8tap_V_acc_600_400.bin",0x5612000,480000},
    {"osdmanargb444418","imgrzemu.filter 2 2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
    {"osdmanargb444419","imgrzemu.osdman1   0x29fc000 1 2 800 0   0  400 200 0x5612000 2 1200   0    0  600 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd19_argb4444_H_8tap_V_4tap_600_400.bin",0x5612000,480000},
    {"osdmanargb444420","imgrzemu.filter 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
	  {NULL,NULL,NULL,FALSE,NULL,0,FALSE,0,0}
};


T_AUTO_TEST_CASE pAutoSetOsd[]=
{
  {"loadSourcefile",NULL,TRUE,"D:\\jpg_test\\dump","lena_252x252.8888",0x29fc000,FALSE,NULL,NULL,0,0},

  {"osdman1","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000 3 400 0 0 100 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"osdsave1",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd1_argb8888_100_100.bin",0x5612000,40000},

  {"osdman1.x1","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000 3 400 1 0 99 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"osdsave1.x1",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd1_argb8888_100_100_x1.bin",0x5612000,40000},
  {"osdman1.x2","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000 3 400 2 0 98 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"osdsave1.x2",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd1_argb8888_100_100_x2.bin",0x5612000,40000},
  {"osdman1.x3","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000 3 400 3 0 97 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"osdsave1.x3",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd1_argb8888_100_100_x3.bin",0x5612000,40000},
  {"osdman1.x4","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000 3 400 4 0 96 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"osdsave1.x4",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd1_argb8888_100_100_x4.bin",0x5612000,40000},
  {"osdman1.x15","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000 3 400 15 0 85 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"osdsave1.x15",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd1_argb8888_100_100_x15.bin",0x5612000,40000},

  {"osdman2","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000 3 2000 0 0 500 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"osdsave2",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd2_argb8888_500_400.bin",0x5612000,800000},

  {"osdman3","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000 3 96 0 0 24 24 0 0 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"osdsave3",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd3_argb8888_24_24.bin",0x5612000,2304},

  {"osdman4","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000 3 400 0 0 100 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"osdsave4",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd4_argb8888_100_400.bin",0x5612000,160000},

  {"osdman5","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000 3 1600 0 0 400 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"osdsave5",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd5_argb8888_400_100.bin",0x5612000,160000},

  {"osdman6","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000 2 800 0 0 400 200 0 0 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"osdsave6",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd6_argb4444_400_200.bin",0x5612000,160000},

  {"osdman7","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000 1 800 0 0 400 200 0 0 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"osdsave7",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd7_argb1555_400_200.bin",0x5612000,160000},

  {"osdman8","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000 0 800 0 0 400 200 0 0 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"osdsave8",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd8_rgb565_400_200.bin",0x5612000,160000},

  {"loadfile9",NULL,TRUE,"D:\\jpg_test\\dump\\upload","set1_osd8_rgb565_400_200.bin",0x29fc000,FALSE,NULL,NULL,0,0},
  {"osdman9","imgrzemu.osdman1 0x29fc000 1 0 800 0 0 400 200 0x5612000 3 1600 0 0 400 100 0 0 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"osdsave9",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd9_argba888_400_100.bin",0x5612000,160000},

  {"loadfile10",NULL,TRUE,"D:\\jpg_test\\dump\\upload","set1_osd6_argb4444_400_200.bin",0x29fc000,FALSE,NULL,NULL,0,0},
  {"osdman10","imgrzemu.osdman1 0x29fc000 1 2 800 0 0 400 200 0x5612000 2 640 0 0 320 300 0 0 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"osdsave10",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd10_argb4444_320_300.bin",0x5612000,192000},

  {"loadfile11",NULL,TRUE,"D:\\jpg_test\\dump\\upload","set1_osd10_argb4444_320_300.bin",0x29fc000,FALSE,NULL,NULL,0,0},
  {"osdman11","imgrzemu.osdman1 0x29fc000 1 2 640 0 0 320 300 0x5612000 3 1600 0 0 400 400 0 0 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"osdsave11",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","osd11_argb8888_400_400.bin",0x5612000,640000},
  {NULL,NULL,NULL,FALSE,NULL,0,FALSE,0,0}
};



T_AUTO_TEST_CASE pAutoSetVdo422RasterSrc[]=
{
	
 {"loadfileY",NULL,TRUE,"D:\\jpg_test\\dump","luma_rs_422_720x480.bin",0x29fc000,FALSE,NULL,NULL,0,0},
 {"loadfileC",NULL,TRUE,"D:\\jpg_test\\dump","chroma_rs_422_720x480.bin",0x2c7c000,FALSE,NULL,NULL,0,0},

 {"vdomanraster4221","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 0 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 240 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile1Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo1Y_r_420_720_480.bin",0x5612000,345600},
 {"savefile1C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo1C_r_420_720_480.bin",0x5f0e000,172800},

 {"vdomanraster4222","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 0 0 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 240 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile2Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo2Y_b_420_720_480.bin",0x5612000,345600},
 {"savefile2C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo2C_b_420_720_480.bin",0x5f0e000,172800},

 {"vdomanraster4223","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 1 0 0x5612000 0 256 0 0 256 400 0x5f0e000 128 400 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile3Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo3Y_r_422_256_400.bin",0x5612000,102400},
 {"savefile3C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo3C_r_422_256_400.bin",0x5f0e000,102400},

 {"vdomanraster4223","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 1 0 0x5612000 0 1024 0 0 1024 768 0x5f0e000 512 768 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile4Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo4Y_r_422_1024_768.bin",0x5612000,786432},
 {"savefile4C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo4C_r_422_1024_768.bin",0x5f0e000,786432},
 
 {"vdomanraster4225","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 1 0 0x5612000 0 1024 0 0 1024 768 0x5f0e000 512 768 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile5Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo5Y_r_422_1024_768.bin",0x5612000,245760},
 {"savefile5C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo5C_r_422_1024_768.bin",0x5f0e000,245760},
 
 {"vdomanraster4226","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 1 0 0x5612000 0 1024 0 0 1024 240 0x5f0e000 512 240 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile6Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo6Y_r_422_1024_240.bin",0x5612000,245760},
 {"savefile6C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo6C_r_422_1024_240.bin",0x5f0e000,245760},
 
 {"vdomanraster4227","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 1 0 0x5612000 0 256 0 0 256 1024 0x5f0e000 128 1024 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile7Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo7Y_r_422_256_1024.bin",0x5612000,262144},
 {"savefile7C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo7C_r_422_256_1024.bin",0x5f0e000,262144},

 {"vdomanraster4228","imgrzemu.filter 1 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanraster4229","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 0 0 0x5612000 0 256 0 0 256 512 0x5f0e000 128 256 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile9Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo9Y_r_Hacc_Vacc_420_256_512.bin",0x5612000,131072},
 {"savefile9C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo9C_r_Hacc_Vacc_420_256_512.bin",0x5f0e000,65536},

 {"vdomanraster42210","imgrzemu.filter 1 2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanraster42211","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 0 1 0 0x5612000 0 256 0 0 256 512 0x5f0e000 128 512 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile11Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo11Y_b_Hacc_V4tap_422_256_512.bin",0x5612000,131072},
 {"savefile11C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo11C_b_Hacc_V4tap_422_256_512.bin",0x5f0e000,131072},

 {"vdomanraster42212","imgrzemu.filter 2 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanraster42213","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 2 1 0x5612000 3 4096 0 0 1024 768 0x5f0e000 1024 768 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile13Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo13Y_osd_aycbcr8888_H8tap_Vacc_1024_768.bin",0x5612000,3145728},

 {"vdomanraster42214","imgrzemu.filter 2 2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanraster42215","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 2 1 0x5612000 3 2048 0 0 512 512 0x5f0e000 512 512 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile15Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo15Y_osd_aycbcr8888_H8tap_V4tap512_512.bin",0x5612000,1048576},

 {"vdomanraster42216","imgrzemu.filter 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanraster42217","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 1 1 0x5612000 4 1440 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile17Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo17Y_osd_ycbycr8888_720_480.bin",0x5612000,691200},

 {"vdomanraster42218","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 1 1 0x5612000 5 1440 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile18Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo18Y_osd_cbycry8888_720_480.bin",0x5612000,691200},

 {"vdomanraster42219","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile19Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo19Y_r_422_720_480.bin",0x5612000,345600},
 {"savefile19C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo19C_r_422_720_480.bin",0x5f0e000,345600},

 {"vdomanraster42220","imgrzemu.vdoman1 0 1 0x29fc000 720 200 50 520 430 0x2c7c000 100 50 260 430 0 1 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile20Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo20Y_r_422_src_200_50_720_480.bin",0x5612000,345600},
 {"savefile20C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo20C_r_422_src_200_50_720_480.bin",0x5f0e000,345600},

 {"vdomanraster42221","imgrzemu.vdoman1 0 1 0x29fc000 720 0  0 720 480 0x2c7c000 0 0 360 480 0 1 1 0 0x5612000 0 720 100 200 620 280 0x5f0e000 310 280 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile21Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo21Y_r_422_dst_100_200_720_480.bin",0x5612000,345600},
 {"savefile21C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo21C_r_422_dst_100_200_720_480.bin",0x5f0e000,345600},

 {"vdomanraster42222","imgrzemu.vdoman1 0 1 0x29fc000 720 200 50 520 430 0x2c7c000 100 50 260 430 0 1 1 0 0x5612000 0 720 100 200 620 280 0x5f0e000 310 280 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile22Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo22Y_r_422_src_200_50_dst_100_200_720_480.bin",0x5612000,345600},
 {"savefile22C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo22C_r_422_src_200_50_dst_100_200_720_480.bin",0x5f0e000,345600},
 
 {"vdomanraster42223","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 6 0 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile23Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo23Y_b_422_720_480_normal.bin",0x5612000,345600},
 {"savefile23C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo23C_b_422_720_480_normal.bin",0x5f0e000,345600},

 {"vdomanraster42224","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 6 0 0 0 0x5612000 0 1024 0 0 1024 768 0x5f0e000 512 384 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile24Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo24Y_b_420_1024_768_normal.bin",0x5612000,786432},
 {"savefile24C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo24C_b_420_1024_768_normal.bin",0x5f0e000,393216},
 

 {NULL,NULL,NULL,FALSE,NULL,0,FALSE,0,0}
};


T_AUTO_TEST_CASE pAutoSetVdo422BlockSrc[]=
{
	
 {"loadfileY",NULL,TRUE,"D:\\jpg_test\\dump","luma_mb_422_720x480.bin",0x29fc000,FALSE,NULL,NULL,0,0},
 {"loadfileC",NULL,TRUE,"D:\\jpg_test\\dump","chroma_mb_422_720x480.bin",0x2c7c000,FALSE,NULL,NULL,0,0},

 {"vdomanblock4221","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 0 0 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 240 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile1Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo1Y_b_420_720_480.bin",0x5612000,345600},
 {"savefile1C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo1C_b_420_720_480.bin",0x5f0e000,172800},

 {"vdomanblock4222","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 0 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 240 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile2Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo2Y_r_420_720_480.bin",0x5612000,345600},
 {"savefile2C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo2C_r_420_720_480.bin",0x5f0e000,172800},

 {"vdomanblock4223","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 0 1 0 0x5612000 0 256 0 0 256 400 0x5f0e000 128 400 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile3Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo3Y_b_422_256_400.bin",0x5612000,102400},
 {"savefile3C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo3C_b_422_256_400.bin",0x5f0e000,102400},

 {"vdomanblock4224","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 0 1 0 0x5612000 0 1024 0 0 1024 768 0x5f0e000 512 768 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile4Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo4Y_b_422_1024_768.bin",0x5612000,786432},
 {"savefile4C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo4C_b_422_1024_768.bin",0x5f0e000,786432},
 
 {"vdomanblock4225","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 0 1 0 0x5612000 0 1024 0 0 1024 768 0x5f0e000 512 768 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile5Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo5Y_b_422_1024_768.bin",0x5612000,245760},
 {"savefile5C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo5C_b_422_1024_768.bin",0x5f0e000,245760},
 
 {"vdomanblock4226","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 0 1 0 0x5612000 0 1024 0 0 1024 240 0x5f0e000 512 240 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile6Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo6Y_b_422_1024_240.bin",0x5612000,245760},
 {"savefile6C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo6C_b_422_1024_240.bin",0x5f0e000,245760},
 
 {"vdomanblock4227","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 0 1 0 0x5612000 0 256 0 0 256 1024 0x5f0e000 128 1024 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile7Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo7Y_b_422_256_1024.bin",0x5612000,262144},
 {"savefile7C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo7C_b_422_256_1024.bin",0x5f0e000,262144},

 {"vdomanblock4228","imgrzemu.filter 1 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanblock4229","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 0 0 0 0x5612000 0 256 0 0 256 512 0x5f0e000 128 256 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile9Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo9Y_b_Hacc_Vacc_420_256_512.bin",0x5612000,131072},
 {"savefile9C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo9C_b_Hacc_Vacc_420_256_512.bin",0x5f0e000,65536},

 {"vdomanblock42210","imgrzemu.filter 1 2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanblock42211","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 0 1 0 0x5612000 0 256 0 0 256 512 0x5f0e000 128 512 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile11Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo11Y_b_Hacc_V4tap_422_256_512.bin",0x5612000,131072},
 {"savefile11C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo11C_b_Hacc_V4tap_422_256_512.bin",0x5f0e000,131072},

 {"vdomanblock42212","imgrzemu.filter 2 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanblock42213","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 2 1 0x5612000 3 4096 0 0 1024 768 0x5f0e000 1024 768 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile13Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo13Y_osd_aycbcr8888_H8tap_Vacc_1024_768.bin",0x5612000,3145728},

 {"vdomanblock42214","imgrzemu.filter 2 2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanblock42215","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 2 1 0x5612000 3 2048 0 0 512 512 0x5f0e000 512 512 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile15Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo15Y_osd_aycbcr8888_H8tap_V4tap512_512.bin",0x5612000,1048576},

 {"vdomanblock42216","imgrzemu.filter 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanblock42217","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 1 1 0x5612000 4 1440 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile17Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo17Y_osd_ycbycr8888_720_480.bin",0x5612000,691200},

 {"vdomanblock42218","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 1 1 0x5612000 5 1440 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile18Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo18Y_osd_cbycry8888_720_480.bin",0x5612000,691200},

 {"vdomanblock42219","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 0 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile19Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo19Y_b_422_720_480.bin",0x5612000,345600},
 {"savefile19C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo19C_b_422_720_480.bin",0x5f0e000,345600},

 {"vdomanblock42220","imgrzemu.vdoman1 0 0 0x29fc000 720 200 50 520 430 0x2c7c000 100 50 260 430 0 0 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile20Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo20Y_b_422_src_200_50_720_480.bin",0x5612000,345600},
 {"savefile20C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo20C_b_422_src_200_50_720_480.bin",0x5f0e000,345600},

 {"vdomanblock42221","imgrzemu.vdoman1 0 0 0x29fc000 720 0  0 720 480 0x2c7c000 0 0 360 480 0 0 1 0 0x5612000 0 720 100 200 620 280 0x5f0e000 310 280 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile21Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo21Y_b_422_dst_100_200_720_480.bin",0x5612000,345600},
 {"savefile21C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo21C_b_422_dst_100_200_720_480.bin",0x5f0e000,345600},


 {"vdomanblock42222","imgrzemu.vdoman1 0 0 0x29fc000 720 200 50 520 430 0x2c7c000 100 50 260 430 0 0 1 0 0x5612000 0 720 100 200 620 280 0x5f0e000 310 280 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile22Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo22Y_b_422_src_200_50_dst_100_200_720_480.bin",0x5612000,345600},
 {"savefile22C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo22C_b_422_src_200_50_dst_100_200_720_480.bin",0x5f0e000,345600},

 {"vdomanblock42223","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 6 0 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile23Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo23Y_b_422_720_480_normal.bin",0x5612000,345600},
 {"savefile23C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo23C_b_422_720_480_normal.bin",0x5f0e000,345600},

 {"vdomanblock42224","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 6 0 0 0 0x5612000 0 1024 0 0 1024 768 0x5f0e000 512 384 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile24Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo24Y_b_420_1024_768_normal.bin",0x5612000,786432},
 {"savefile24C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo24C_b_420_1024_768_normal.bin",0x5f0e000,393216},
 
 {NULL,NULL,NULL,FALSE,NULL,0,FALSE,0,0}
};


T_AUTO_TEST_CASE pAutoSetVdo420RasterSrc[]=
{
	
 {"loadfileY",NULL,TRUE,"D:\\jpg_test\\dump","luma_rs_720x480.bin",0x29fc000,FALSE,NULL,NULL,0,0},
 {"loadfileC",NULL,TRUE,"D:\\jpg_test\\dump","chroma_rs_720x240.bin",0x2c7c000,FALSE,NULL,NULL,0,0},

 {"vdomanraster420","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile1Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo1Y_b_422_720_480.bin",0x5612000,345600},
 {"savefile1C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo1C_b_422_720_480.bin",0x5f0e000,345600},

 {"vdomanraster4202","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile2Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo2Y_r_422_720_480.bin",0x5612000,345600},
 {"savefile2C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo2C_r_422_720_480.bin",0x5f0e000,345600},

 {"vdomanraster4203","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 0 0 0x5612000 0 256 0 0 256 400 0x5f0e000 128 200 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile3Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo3Y_r_420_256_400.bin",0x5612000,102400},
 {"savefile3C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo3C_r_420_256_400.bin",0x5f0e000,51200},

 {"vdomanraster4204","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 0 0 0x5612000 0 1024 0 0 1024 768 0x5f0e000 512 384 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile4Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo4Y_r_420_1024_768.bin",0x5612000,786432},
 {"savefile4C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo4C_r_420_1024_768.bin",0x5f0e000,393216},

 {"vdomanraster4205","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 0 0 0x5612000 0 1024 0 0 1024 768 0x5f0e000 512 384 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile5Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo5Y_b_420_1024_768.bin",0x5612000,786432},
 {"savefile5C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo5C_b_420_1024_768.bin",0x5f0e000,393216},
 
 {"vdomanraster4206","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 0 0 0x5612000 0 1024 0 0 1024 240 0x5f0e000 512 120 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile6Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo6Y_r_420_1024_240.bin",0x5612000,245760},
 {"savefile6C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo6C_r_420_1024_240.bin",0x5f0e000,122880},
 
 {"vdomanraster4207","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 0 0 0x5612000 0 256 0 0 256 1024 0x5f0e000 128 512 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile7Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo7Y_r_420_256_1024.bin",0x5612000,262144},
 {"savefile7C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo7C_r_420_256_1024.bin",0x5f0e000,131072},

 {"vdomanraster4208","imgrzemu.filter 1 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanraster4209","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 1 0 0x5612000 0 256 0 0 256 512 0x5f0e000 128 512 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile9Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo9Y_r_Hacc_Vacc_422_256_512.bin",0x5612000,131072},
 {"savefile9C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo9C_r_Hacc_Vacc_422_256_512.bin",0x5f0e000,131072},

 {"vdomanraster42010","imgrzemu.filter 1 2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanraster42011","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 0 0 0x5612000 0 256 0 0 256 512 0x5f0e000 128 256 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile11Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo11Y_b_Hacc_V4tap_420_256_512.bin",0x5612000,131072},
 {"savefile11C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo11C_b_Hacc_V4tap_420_256_512.bin",0x5f0e000,65536},

 {"vdomanraster42012","imgrzemu.filter 2 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanraster42013","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 2 1 0x5612000 3 4096 0 0 1024 768 0x5f0e000 1024 768 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile13Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo13Y_osd_aycbcr8888_H8tap_Vacc_1024_768.bin",0x5612000,3145728},

 {"vdomanraster42014","imgrzemu.filter 2 2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanraster42015","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 2 1 0x5612000 3 2048 0 0 512 512 0x5f0e000 512 512 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile15Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo15Y_osd_aycbcr8888_H8tap_V4tap512_512.bin",0x5612000,1048576},

 {"vdomanraster42016","imgrzemu.filter 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanraster42017","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 1 1 0x5612000 4 1440 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile17Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo17Y_osd_ycbycr8888_720_480.bin",0x5612000,691200},

 {"vdomanraster42018","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 1 1 0x5612000 5 1440 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile18Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo18Y_osd_cbycry8888_720_480.bin",0x5612000,691200},

 {"vdomanraster42019","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 0 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 240 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile19Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo19Y_r_420_720_480.bin",0x5612000,345600},
 {"savefile19C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo19C_r_420_720_480.bin",0x5f0e000,172800},

 {"vdomanraster42020","imgrzemu.vdoman1 0 1 0x29fc000 720 200 50 520 430 0x2c7c000 100 25 260 215 0 1 0 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 240 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile19Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo20Y_r_420_src_200_50_720_480.bin",0x5612000,345600},
 {"savefile19C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo20C_r_420_src_200_50_720_480.bin",0x5f0e000,172800},

 {"vdomanraster42021","imgrzemu.vdoman1 0 1 0x29fc000 720 0  0   720 480 0x2c7c000 0 0 360 240 0 1 0 0 0x5612000 0 720 100 200 620 280 0x5f0e000 310 140 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile21Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo21Y_r_420_dst_100_200_720_480.bin",0x5612000,345600},
 {"savefile21C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo21C_r_420_dst_100_200_720_480.bin",0x5f0e000,172800},

 {"vdomanraster42022","imgrzemu.vdoman1 0 1 0x29fc000 720 200 50 520 430 0x2c7c000 100 25 260 215 0 1 0 0 0x5612000 0 720 100 200 620 280 0x5f0e000 310 140 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile22Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo22Y_r_420_src_200_50_dst_100_200_720_480.bin",0x5612000,345600},
 {"savefile22C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo22C_r_420_src_200_50_dst_100_200_720_480.bin",0x5f0e000,172800},

 {"vdomanmaster42023","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 6 0 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile23Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo23Y_b_422_720_480_normal.bin",0x5612000,345600},
 {"savefile23C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo23C_b_422_720_480_normal.bin",0x5f0e000,345600},

 {"vdomanmaster42024","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 6 0 0 0 0x5612000 0 1024 0 0 1024 768 0x5f0e000 512 384 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile24Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo24Y_b_420_1024_768_normal.bin",0x5612000,786432},
 {"savefile24C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo24C_b_420_1024_768_normal.bin",0x5f0e000,393216},
 

 {NULL,NULL,NULL,FALSE,NULL,0,FALSE,0,0}
};

T_AUTO_TEST_CASE pAutoSetVdo420BlockSrc[]=
{
 {"loadfile1Y",NULL,TRUE,"D:\\jpg_test\\dump","luma_mb_720x480.bin",0x29fc000,FALSE,NULL,NULL,0,0},
 {"loadfile1C",NULL,TRUE,"D:\\jpg_test\\dump","chroma_mb_720x240.bin",0x2c7c000,FALSE,NULL,NULL,0,0},
 
 {"vdomanblock1","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile1Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo1Y_r_422_720_480.bin",0x5612000,345600},
 {"savefile1C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo1C_r_422_720_480.bin",0x5f0e000,345600},

 {"vdomanblock2","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile2Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo2Y_b_422_720_480.bin",0x5612000,345600},
 {"savefile2C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo2C_b_422_720_480.bin",0x5f0e000,345600},

 {"vdomanblock3","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 0 0 0x5612000 0 256 0 0 256 400 0x5f0e000 128 200 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile3Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo3Y_b_420_256_400.bin",0x5612000,102400},
 {"savefile3C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo3C_b_420_256_400.bin",0x5f0e000,51200},

 {"vdomanblock4","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 0 0 0x5612000 0 1024 0 0 1024 768 0x5f0e000 512 384 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile4Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo4Y_b_420_1024_768.bin",0x5612000,786432},
 {"savefile4C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo4C_b_420_1024_768.bin",0x5f0e000,393216},
 
 {"vdomanblock5","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 0 0 0x5612000 0 1024 0 0 1024 768 0x5f0e000 512 384 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile5Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo5Y_b_420_1024_768.bin",0x5612000,245760},
 {"savefile5C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo5C_b_420_1024_768.bin",0x5f0e000,122880},
 
 {"vdomanblock6","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 0 0 0x5612000 0 1024 0 0 1024 240 0x5f0e000 512 120 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile6Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo6Y_b_420_1024_240.bin",0x5612000,245760},
 {"savefile6C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo6C_b_420_1024_240.bin",0x5f0e000,122880},
 
 {"vdomanblock7","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 0 0 0x5612000 0 256 0 0 256 1024 0x5f0e000 128 512 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile7Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo7Y_b_420_256_1024.bin",0x5612000,262144},
 {"savefile7C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo7C_b_420_256_1024.bin",0x5f0e000,131072},

 {"vdomanblock8","imgrzemu.filter 1 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanblock9","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 1 0 0x5612000 0 256 0 0 256 512 0x5f0e000 128 512 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile9Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo9Y_b_H_acc_V_acc_422_256_512.bin",0x5612000,131072},
 {"savefile9C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo9C_b_H_acc_V_acc_422_256_512.bin",0x5f0e000,131072},

 {"vdomanblock10","imgrzemu.filter 1 2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanblock11","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 0 0 0x5612000 0 256 0 0 256 512 0x5f0e000 128 256 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile11Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo11Y_r_H_acc_V_4tap_420_256_512.bin",0x5612000,131072},
 {"savefile11C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo11C_r_H_acc_V_4tap_420_256_512.bin",0x5f0e000,65536},

 {"vdomanblock12","imgrzemu.filter 2 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanblock13","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 2 1 0x5612000 3 4096 0 0 1024 768 0x5f0e000 1024 768 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile13Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo13Y_osd_aycbcr8888_1024_768.bin",0x5612000,3145728},

 {"vdomanblock14","imgrzemu.filter 2 2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanblock15","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 2 1 0x5612000 3 2048 0 0 512 512 0x5f0e000 512 512 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile15Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo15Y_osd_aycbcr8888_512_512.bin",0x5612000,1048576},

 {"vdomanblock16","imgrzemu.filter 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanblock17","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 1 1 0x5612000 4 1440 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile17Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo17Y_osd_ycbycr8888_720_480.bin",0x5612000,691200},

 {"vdomanblock18","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 1 1 0x5612000 5 1440 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile18Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo18Y_osd_cbycry8888_720_480.bin",0x5612000,691200},

 {"vdomanblock19","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 0 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 240 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile19Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo19Y_b_420_720_480.bin",0x5612000,345600},
 {"savefile19C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo19C_b_420_720_480.bin",0x5f0e000,172800},

 {"vdomanblock20","imgrzemu.vdoman1 0 0 0x29fc000 720 200 50 520 430 0x2c7c000 100 25 260 215 0 0 0 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 240 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile19Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo20Y_b_420_src_200_50_720_480.bin",0x5612000,345600},
 {"savefile19C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo20C_b_420_src_200_50_720_480.bin",0x5f0e000,172800},

 {"vdomanblock21","imgrzemu.vdoman1 0 0 0x29fc000 720 0  0   720 480 0x2c7c000 0 0 360 240 0 0 0 0 0x5612000 0 720 100 200 620 280 0x5f0e000 310 140 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile21Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo21Y_b_420_dst_100_200_720_480.bin",0x5612000,345600},
 {"savefile21C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo21C_b_420_dst_100_200_720_480.bin",0x5f0e000,172800},

 {"vdomanblock22","imgrzemu.vdoman1 0 0 0x29fc000 720 200 50 520 430 0x2c7c000 100 25 260 215 0 0 0 0 0x5612000 0 720 100 200 620 280 0x5f0e000 310 140 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile22Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo22Y_b_420_src_200_50_dst_100_200_720_480.bin",0x5612000,345600},
 {"savefile22C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo22C_b_420_src_200_50_dst_100_200_720_480.bin",0x5f0e000,172800},

 {"vdomanblock23","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 6 0 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile23Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo23Y_b_422_720_480_normal.bin",0x5612000,345600},
 {"savefile23C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo23C_b_422_720_480_normal.bin",0x5f0e000,345600},

 {"vdomanblock24","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 6 0 0 0 0x5612000 0 1024 0 0 1024 768 0x5f0e000 512 384 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile24Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo24Y_b_420_1024_768_normal.bin",0x5612000,786432},
 {"savefile24C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo24C_b_420_1024_768_normal.bin",0x5f0e000,393216},
 
 
 {NULL,NULL,NULL,FALSE,NULL,0,FALSE,0,0}
};


T_AUTO_TEST_CASE pAutoSetVdo420BlockNormalSrc[]=
{
 {"loadfile1Y",NULL,TRUE,"D:\\jpg_test\\dump","luma_mb_420_1024x768_normal.bin",0x29fc000,FALSE,NULL,NULL,0,0},
 {"loadfile1C",NULL,TRUE,"D:\\jpg_test\\dump","chroma_mb_420_1024x768_normal.bin",0x2c7c000,FALSE,NULL,NULL,0,0},
 
 {"vdomanblock1","imgrzemu.vdoman1 6 0 0x29fc000 1024 0 0 1024 768 0x2c7c000 0 0 512 384 0 1 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile1Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo1Y_r_422_720_480.bin",0x5612000,345600},
 {"savefile1C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo1C_r_422_720_480.bin",0x5f0e000,345600},

 {"vdomanblock2","imgrzemu.vdoman1 6 0 0x29fc000 1024 0 0 1024 768 0x2c7c000 0 0 512 384 6 0 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile2Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo2Y_b_422_720_480_normal.bin",0x5612000,345600},
 {"savefile2C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo2C_b_422_720_480_normal.bin",0x5f0e000,345600},

 {"vdomanblock3","imgrzemu.vdoman1 6 0 0x29fc000 1024 0 0 1024 768 0x2c7c000 0 0 512 384 6 0 0 0 0x5612000 0 1280 0 0 1280 960 0x5f0e000 640 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile3Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo3Y_b_420_1280_960_normal.bin",0x5612000,1228800},
 {"savefile3C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo3C_b_420_1280_960_normal.bin",0x5f0e000,614400},

 {"vdomanblock4","imgrzemu.vdoman1 6 0 0x29fc000 1024 0 0 1024 768 0x2c7c000 0 0 512 384 0 0 0 0 0x5612000 0 1024 0 0 1024 768 0x5f0e000 512 384 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile4Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo4Y_b_420_1024_768.bin",0x5612000,786432},
 {"savefile4C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo4C_b_420_1024_768.bin",0x5f0e000,393216},
 
 {"vdomanblock5","imgrzemu.vdoman1 6 0 0x29fc000 1024 0 0 1024 768 0x2c7c000 0 0 512 384 6 0 0 0 0x5612000 0 1024 0 0 1024 768 0x5f0e000 512 384 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile5Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo5Y_b_420_1024_768_normal.bin",0x5612000,786432},
 {"savefile5C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo5C_b_420_1024_768_normal.bin",0x5f0e000,393216},
 
 {"vdomanblock6","imgrzemu.vdoman1 6 0 0x29fc000 1024 0 0 1024 768 0x2c7c000 0 0 512 384 6 0 0 0 0x5612000 0 1024 0 0 1024 240 0x5f0e000 512 120 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile6Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo6Y_b_420_1024_240_normal.bin",0x5612000,245760},
 {"savefile6C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo6C_b_420_1024_240_normal.bin",0x5f0e000,122880},
 
 {"vdomanblock7","imgrzemu.vdoman1 6 0 0x29fc000 1024 0 0 1024 768 0x2c7c000 0 0 512 384 0 0 0 0 0x5612000 0 256 0 0 256 1024 0x5f0e000 128 512 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile7Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo7Y_b_420_256_1024.bin",0x5612000,262144},
 {"savefile7C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo7C_b_420_256_1024.bin",0x5f0e000,131072},

 {"vdomanblock8","imgrzemu.filter 1 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanblock9","imgrzemu.vdoman1 6 0 0x29fc000 1024 0 0 1024 768 0x2c7c000 0 0 512 384 6 0 1 0 0x5612000 0 256 0 0 256 512 0x5f0e000 128 512 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile9Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo9Y_b_H_acc_V_acc_422_256_512_normal.bin",0x5612000,131072},
 {"savefile9C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo9C_b_H_acc_V_acc_422_256_512_normal.bin",0x5f0e000,131072},

 {"vdomanblock10","imgrzemu.filter 1 2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanblock11","imgrzemu.vdoman1 6 0 0x29fc000 1024 0 0 1024 768 0x2c7c000 0 0 512 384 0 1 0 0 0x5612000 0 256 0 0 256 512 0x5f0e000 128 256 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile11Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo11Y_r_H_acc_V_4tap_420_256_512.bin",0x5612000,131072},
 {"savefile11C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo11C_r_H_acc_V_4tap_420_256_512.bin",0x5f0e000,65536},

 {"vdomanblock12","imgrzemu.filter 2 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanblock13","imgrzemu.vdoman1 6 0 0x29fc000 1024 0 0 1024 768 0x2c7c000 0 0 512 384 0 1 2 1 0x5612000 3 4096 0 0 1024 768 0x5f0e000 1024 768 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile13Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo13_osd_aycbcr8888_1024_768.bin",0x5612000,3145728},

 {"vdomanblock14","imgrzemu.filter 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanblock15","imgrzemu.vdoman1 6 0 0x29fc000 1024 0 0 1024 768 0x2c7c000 0 0 512 384 0 1 2 1 0x5612000 3 2048 0 0 512 512 0x5f0e000 512 512 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile15Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo15_osd_aycbcr8888_512_512.bin",0x5612000,1048576},

 {"vdomanblock16","imgrzemu.filter 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"vdomanblock17","imgrzemu.vdoman1 6 0 0x29fc000 1024 0 0 1024 768 0x2c7c000 0 0 512 384 0 1 1 1 0x5612000 4 1440 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile17Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo17_osd_ycbycr8888_720_480.bin",0x5612000,691200},

 {"vdomanblock18","imgrzemu.vdoman1 6 0 0x29fc000 1024 0 0 1024 768 0x2c7c000 0 0 512 384 0 1 1 1 0x5612000 5 1440 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile18Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo18_osd_cbycry8888_720_480.bin",0x5612000,691200},

 {"vdomanblock19","imgrzemu.vdoman1 6 0 0x29fc000 1024 0 0 1024 768 0x2c7c000 0 0 512 384 6 0 0 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 240 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile19Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo19Y_b_420_720_480_normal.bin",0x5612000,345600},
 {"savefile19C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo19C_b_420_720_480_normal.bin",0x5f0e000,172800},

 {"vdomanblock20","imgrzemu.vdoman1 6 0 0x29fc000 1024 200 50 824 718 0x2c7c000 100 25 412 359 0 1 0 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 240 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile19Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo20Y_r_420_src_200_50_720_480.bin",0x5612000,345600},
 {"savefile19C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo20C_r_420_src_200_50_720_480.bin",0x5f0e000,172800},

 {"vdomanblock21","imgrzemu.vdoman1 6 0 0x29fc000 1024 0 0 1024 768 0x2c7c000 0 0 512 384 0 1 1 0 0x5612000 0 1920 100 200 1820 880 0x5f0e000 910 880 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile21Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo21Y_r_422_dst_100_200_1920_1080.bin",0x5612000,2073600},
 {"savefile21C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo21C_r_422_dst_100_200_1920_1080.bin",0x5f0e000,2073600},

 {"vdomanblock22","imgrzemu.vdoman1 6 0 0x29fc000 1024 200 50 824 718 0x2c7c000 100 25 412 359 6 0 1 0 0x5612000 0 1920 100 200 1820 880 0x5f0e000 910 880 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile22Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo22Y_b_422_src_200_50_dst_100_200_1920_1080_normal.bin",0x5612000,2073600},
 {"savefile22C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","vdo22C_b_422_src_200_50_dst_100_200_1920_1080_normal.bin",0x5f0e000,2073600},
 
 {NULL,NULL,NULL,FALSE,NULL,0,FALSE,0,0}
};

T_AUTO_TEST_CASE pAutoSetVdo[]=
{

 {"loadfile1Y",NULL,TRUE,"D:\\jpg_test\\dump","luma_mb_720x480.bin",0x29fc000,FALSE,NULL,NULL,0,0},
 {"loadfile1C",NULL,TRUE,"D:\\jpg_test\\dump","chroma_mb_720x240.bin",0x2c7c000,FALSE,NULL,NULL,0,0},
 {"vdoman1","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 0 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 240 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile1Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test1Y_r_420_720_480.bin",0x5612000,0x54600},
 {"savefile1C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test1C_r_420_720_480.bin",0x5f0e000,0x2a300},
 {"vdoman2","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 0 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 240 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile2Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test2Y_b_420_720_480.bin",0x5612000,0x54600},
 {"savefile2C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test2C_b_420_720_480.bin",0x5f0e000,0x2a300},

 {"loadfile3Y",NULL,TRUE,"D:\\jpg_test\\dump","luma_rs_720x480.bin",0x29fc000,FALSE,NULL,NULL,0,0},
 {"loadfile3C",NULL,TRUE,"D:\\jpg_test\\dump","chroma_rs_720x240.bin",0x2c7c000,FALSE,NULL,NULL,0,0},
 {"vdoman3","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 0 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 240 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile3Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test3Y_r_420_720_480.bin",0x5612000,0x54600},
 {"savefile3C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test3C_r_420_720_480.bin",0x5f0e000,0x2a300},

 {"vdoman4","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 0 0 0x5612000 0 368 0 0 368 224 0x5f0e000 184 112 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile4Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test4Y_r_420_368_224.bin",0x5612000,0x14200},
 {"savefile4C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test4C_r_420_368_224.bin",0x5f0e000,0xa100},

 {"vdoman5","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 0 0 0x5612000 0 768 0 0 768 512 0x5f0e000 384 256 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile5Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test5Y_r_420_768_512.bin",0x5612000,0x60000},
 {"savefile5C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test5C_r_420_768_512.bin",0x5f0e000,0x30000},

 {"vdoman6","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 1 1 0x5612000 4 736 0 0 368 224 0x5f0e000 184 224 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile6Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test6Y_osd_ycbycr422_368_224.bin",0x5612000,0x28400},

 {"vdoman7","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 1 1 0x5612000 5 736 0 0 368 224 0x5f0e000 184 224 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile7Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test7Y_osd_cbycry422_368_224.bin",0x5612000,0x28400},

 {"vdoman9","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 0 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 240 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile9Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test9Y_b_420_720_480.bin",0x5612000,0x54600},
 {"savefile9C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test9C_b_420_720_480.bin",0x5f0e000,0x2a300},
 
 {"vdoman8","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 2 1 0x5612000 3 1472 0 0 368 224 0x5f0e000 368 224 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile8Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test8Y_osd_aycbcr444_368_224.bin",0x5612000,0x50800},

 {"loadfile10Y",NULL,TRUE,"D:\\jpg_test\\dump","luma_mb_720x480.bin",0x29fc000,FALSE,NULL,NULL,0,0},
 {"loadfile10C",NULL,TRUE,"D:\\jpg_test\\dump","chroma_mb_720x240.bin",0x2c7c000,FALSE,NULL,NULL,0,0},
 {"vdoman10","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 0 0 0x5612000 0 368 0 0 368 224 0x5f0e000 184 112 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile10Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test10Y_b_420_368_224.bin",0x5612000,0x14200},
 {"savefile10C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test10C_b_420_368_224.bin",0x5f0e000,0xa100},

 {"vdoman11","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 0 0 0x5612000 0 768 0 0 768 512 0x5f0e000 384 256 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile11Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test11Y_b_420_768_512.bin",0x5612000,0x60000},
 {"savefile11C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test11C_b_420_768_512.bin",0x5f0e000,0x30000},

 {"vdoman12","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 0 0 0x5612000 0 368 0 0 368 224 0x5f0e000 184 112 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile12Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test12Y_r_420_368_224.bin",0x5612000,0x14200},
 {"savefile12Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test12C_r_420_368_224.bin",0x5f0e000,0xA100},

 {"vdoman13","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 1 1 0x5612000 4 736 0 0 368 224 0x5f0e000 184 224 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile13Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test13Y_osd_ycbycr422_368_224.bin",0x5612000,0x28400},

 {"vdoman14","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 1 1 0x5612000 5 736 0 0 368 224 0x5f0e000 184 224 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile14Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test14Y_osd_cbycry422_368_224.bin",0x5612000,0x28400},

 {"vdoman15","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 2 1 0x5612000 3 1472 0 0 368 224 0x5f0e000 368 224 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile15Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test15Y_osd_aycbcr444_368_224.bin",0x5612000,0x50800},

 {"vdoman16","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile16Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test16Y_r_422_720_480.bin",0x5612000,0x54600},
 {"savefile16Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test16C_r_422_720_480.bin",0x5f0e000,0x54600},

  {"loadfile17Y",NULL,TRUE,"D:\\jpg_test\\dump\\upload","set6_test16Y_r_422_720_480.bin",0x29fc000,FALSE,NULL,NULL,0,0},
  {"loadfile17C",NULL,TRUE,"D:\\jpg_test\\dump\\upload","set6_test16C_r_422_720_480.bin",0x2c7c000,FALSE,NULL,NULL,0,0},
  {"vdoman17","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 1 0 0x5612000 0 368 0 0 368 224 0x5f0e000 184 224 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"vdoman17","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 1 0 0x5612000 0 768 0 0 768 512 0x5f0e000 384 512 0 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},


#ifdef IMGRZ_HW_V4

  //	CbCr change for MB 720x480 420 vdo format
 {"loadfile18Y",NULL,TRUE,"D:\\jpg_test\\dump","luma_mb_720x480.bin",0x29fc000,FALSE,NULL,NULL,0,0},
 {"loadfile18C",NULL,TRUE,"D:\\jpg_test\\dump","chroma_mb_720x240.bin",0x2c7c000,FALSE,NULL,NULL,0,0},
 {"vdoman18","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 0 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 240 1 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile18Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test18Y_b_420_720_480.bin",0x5612000,0x54600},
 {"savefile18C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test18C_b_420_720_480.bin",0x5f0e000,0x2a300},

 {"vdoman19","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 0 0 0x5612000 0 368 0 0 368 224 0x5f0e000 184 112 1 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile19Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test19Y_b_420_368_224.bin",0x5612000,0x14200},
 {"savefile19C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test19C_b_420_368_224.bin",0x5f0e000,0xa100},

 {"vdoman20","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 0 0 0x5612000 0 768 0 0 768 512 0x5f0e000 384 256 1 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile20Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test20Y_b_420_768_512.bin",0x5612000,0x60000},
 {"savefile20C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test20C_b_420_768_512.bin",0x5f0e000,0x30000},

  //	CbCr change for RS 720x480 420 vdo format
 {"loadfile21Y",NULL,TRUE,"D:\\jpg_test\\dump","luma_rs_720x480.bin",0x29fc000,FALSE,NULL,NULL,0,0},
 {"loadfile21C",NULL,TRUE,"D:\\jpg_test\\dump","chroma_rs_720x240.bin",0x2c7c000,FALSE,NULL,NULL,0,0},
 {"vdoman21","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 0 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 240 1 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile21Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test21Y_r_420_720_480.bin",0x5612000,0x54600},
 {"savefile21C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test21C_r_420_720_480.bin",0x5f0e000,0x2a300},

 {"vdoman22","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 0 0 0x5612000 0 368 0 0 368 224 0x5f0e000 184 112 1 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile22Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test22Y_r_420_368_224.bin",0x5612000,0x14200},
 {"savefile22C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test22C_r_420_368_224.bin",0x5f0e000,0xa100},

 {"vdoman23","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 1 0 0 0x5612000 0 768 0 0 768 512 0x5f0e000 384 256 1 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile23Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test23Y_r_420_768_512.bin",0x5612000,0x60000},
 {"savefile23C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test23C_r_420_768_512.bin",0x5f0e000,0x30000},
 
  //	CbCr change for MB 720x480 422 vdo format
 {"loadfile24Y",NULL,TRUE,"D:\\jpg_test\\dump","luma_mb_422_720x480.bin",0x29fc000,FALSE,NULL,NULL,0,0},
 {"loadfile24C",NULL,TRUE,"D:\\jpg_test\\dump","chroma_mb_422_720x480.bin",0x2c7c000,FALSE,NULL,NULL,0,0},
 {"vdoman24","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 0 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 1 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile24Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test24Y_b_422_720_480.bin",0x5612000,0x54600},
 {"savefile24C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test24C_b_422_720_480.bin",0x5f0e000,0x54600},

 {"vdoman25","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 0 1 0 0x5612000 0 368 0 0 368 224 0x5f0e000 184 224 1 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile25Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test25Y_b_422_368_224.bin",0x5612000,0x14200},
 {"savefile25C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test25C_b_422_368_224.bin",0x5f0e000,0x14200},

 {"vdoman26","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 0 1 0 0x5612000 0 768 0 0 768 512 0x5f0e000 384 512 1 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile26Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test26Y_b_422_768_512.bin",0x5612000,0x60000},
 {"savefile26C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test26C_b_422_768_512.bin",0x5f0e000,0x60000}, 

  //	CbCr change for RS 720x480 422 vdo format
 {"loadfile27Y",NULL,TRUE,"D:\\jpg_test\\dump","luma_rs_422_720x480.bin",0x29fc000,FALSE,NULL,NULL,0,0},
 {"loadfile27C",NULL,TRUE,"D:\\jpg_test\\dump","chroma_rs_422_720x480.bin",0x2c7c000,FALSE,NULL,NULL,0,0},
 {"vdoman27","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 1 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile27Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test27Y_r_422_720_480.bin",0x5612000,0x54600},
 {"savefile27C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test27C_r_422_720_480.bin",0x5f0e000,0x54600},

 {"vdoman28","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 1 0 0x5612000 0 368 0 0 368 224 0x5f0e000 184 224 1 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile28Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test28Y_r_422_368_224.bin",0x5612000,0x14200},
 {"savefile28C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test28C_r_422_368_224.bin",0x5f0e000,0x14200},

 {"vdoman29","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 1 0 0x5612000 0 768 0 0 768 512 0x5f0e000 384 512 1 0x80",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile29Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test29Y_r_422_768_512.bin",0x5612000,0x60000},
 {"savefile29C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test29C_r_422_768_512.bin",0x5f0e000,0x60000},

//alpha blending 
 {"vdoman30","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 0 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 240 1 0x60",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile30Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test30Y_r_420_720_480.bin",0x5612000,0x54600},
 {"savefile30C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test30C_r_420_720_480.bin",0x5f0e000,0x2a300},

 {"vdoman31","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 1 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 1 0x20",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile31Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test31Y_r_422_720_480.bin",0x5612000,0x54600},
 {"savefile31C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test31C_r_422_720_480.bin",0x5f0e000,0x54600},

 {"vdoman32","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 0 0 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 240 1 0x40",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile32Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test32Y_b_420_720_480.bin",0x5612000,0x54600},
 {"savefile32C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test32C_b_420_720_480.bin",0x5f0e000,0x2a300},

 {"vdoman33","imgrzemu.vdoman1 0 1 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 0 1 0 0x5612000 0 720 0 0 720 480 0x5f0e000 360 480 1 0x10",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
 {"savefile33Y",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test33Y_b_422_720_480.bin",0x5612000,0x54600},
 {"savefile33C",NULL,FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump\\upload","test33C_b_422_720_480.bin",0x5f0e000,0x54600},

#endif
 //{"osdman1-2","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000 3 2000 0 0 500 400",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump","osdman1-2_500_400.bin",0x5612000,0x4b11c00},
 //{"osdman1-2","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000 3 2000 0 0 500 400",FALSE,NULL,NULL,0,TRUE,"D:\\jpg_test\\dump","osdman1-2_500_400.bin",0x5612000,0x4b11c00},

 {NULL,NULL,NULL,FALSE,NULL,0,FALSE,0,0}
};

T_AUTO_TEST_CASE pAutoSetJpg420PicSrc[]=
{
  {"Jpg.init","jpg.jset 100",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"imgrz.init","imgrzemu.rzset 100",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"Jpg.d_off","jpg.d_off",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"Jpg.load 420:512x768","jpg.load imgrz_512x768_420_77599.jpg 77599",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},

  {"jdz1","imgrzemu.jdz 4201 256  416 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz2","imgrzemu.jdz 4202 1024 800 0 1 1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz3","imgrzemu.jdz 4203 256  800 0 0 1 6",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz4","imgrzemu.jdz 4204 1024 512 0 0 0 6",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz5","imgrzemu.jdz 4205 16    16 0 0 1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz6","imgrzemu.jdz 4206 1920 1080 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz7","imgrzemu.jdz 4207 1024  960 1 0 -1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz8","imgrzemu.jdz 4208 256   256 1 1 -1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz9","imgrzemu.jdz 4209 512   768 1 2 -1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz10","imgrzemu.jdz 42010 512 768 0 1 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz11","imgrzemu.jdz 42011 4 4 1 0 -1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz12","imgrzemu.jdz 42012 1024 800 1 0 -1 0 200 100 4",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz13","imgrzemu.jdz 42013 256  256 0 0  1 6 30  40  2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz14","imgrzemu.jdz 42014 768  600 0 0  0 0 500 400 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz_x1","imgrzemu.jdz 420901 1024 768 1 0  0 6 1  0  2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz_x2","imgrzemu.jdz 420902 1024 768 1 0  0 6 2  0  2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz_x3","imgrzemu.jdz 420903 1024 768 1 0  0 6 3  0  2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz_x4","imgrzemu.jdz 420904 1024 768 1 0  0 6 4  0  2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz_x39","imgrzemu.jdz 4209039 1024 768 1 0  0 6 39  0  2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},

  {"init","jpg.jset 8 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},

  {NULL,NULL,NULL,FALSE,NULL,0,FALSE,0,0}
};


T_AUTO_TEST_CASE pAutoSetJpg422PicSrc[]=
{
  {"Jpg.init","jpg.jset 100",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"imgrz.init","imgrzemu.rzset 100",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"Jpg.d_off","jpg.d_off",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"Jpg.load 422:1280x960","jpg.load imgrz_1280x960_422_619781.jpg 619781",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},

  {"jdz1","imgrzemu.jdz 4221 512 768 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz2","imgrzemu.jdz 4222 1920 1024 0 1 1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz3","imgrzemu.jdz 4223 768 1024 0 0 1 6",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz4","imgrzemu.jdz 4224 1536 256 0 0 0 6",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz5","imgrzemu.jdz 4225 16 16 0 0 1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz6","imgrzemu.jdz 4226 1920 1080 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz7","imgrzemu.jdz 4227 1024 960 1 0 -1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz8","imgrzemu.jdz 4228 256 256 1 1 -1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz9","imgrzemu.jdz 4229 512 768 1 2 -1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz10","imgrzemu.jdz 42210 1280 960 0 1 1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz11","imgrzemu.jdz 42211 4 4 1 0 -1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz12","imgrzemu.jdz 42212 1024 800  1 0 -1 0 200 100 4",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz13","imgrzemu.jdz 42213 256  256  0 0  1 6 30  40  2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz14","imgrzemu.jdz 42214 1920 1080 0 0  0 0 500 400 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},

  {"init","jpg.jset 8 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {NULL,NULL,NULL,FALSE,NULL,0,FALSE,0,0}
};


T_AUTO_TEST_CASE pAutoSetJpg444PicSrc[]=
{
  {"Jpg.init","jpg.jset 100",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"imgrz.init","imgrzemu.rzset 100",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"Jpg.d_off","jpg.d_off",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"Jpg.load 444:1280x960","jpg.load imgrz_1280x960_444_265204.jpg 265204",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},

  {"jdz1","imgrzemu.jdz 4441 512 768 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz2","imgrzemu.jdz 4442 1920 1024 0 1 1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz3","imgrzemu.jdz 4443 768 1024 0 0 1 6",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz4","imgrzemu.jdz 4444 1536 256 0 0 0 6",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz5","imgrzemu.jdz 4445 16 16 0 0 1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz6","imgrzemu.jdz 4446 1920 1080 0 0 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz7","imgrzemu.jdz 4447 1024 960 1 0 -1 0 ",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz8","imgrzemu.jdz 4448 256 256 1 1 -1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz9","imgrzemu.jdz 4449 512 768 1 2 -1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz10","imgrzemu.jdz 44410 1280 960 0 1 0 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz11","imgrzemu.jdz 44411 4 4 1 0 -1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz12","imgrzemu.jdz 44412 1024 800 1 0 -1 0 200 100 4",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz13","imgrzemu.jdz 44413 256  256 0 0  1 6 30  40  2",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"jdz14","imgrzemu.jdz 44414 1920 1080 0 0 0 0 500 400 1",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},

  {"init","jpg.jset 100",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {NULL,NULL,NULL,FALSE,NULL,0,FALSE,0,0}
};

T_AUTO_TEST_CASE pAutoSetTimeProfile[]=
{
  {"loadSourcefile",NULL,TRUE,"D:\\jpg_test\\dump","lena_252x252.8888",0x29fc000,FALSE,NULL,NULL,0,0},
  {"ProfileOSD-->1/8(32x32)","imgrzemu.osdman1	0x29fc000 1 3 1008 0 0 252 251 0x5612000 3 128 0 0 32  32 0 0 0",FALSE,NULL,NULL,0,FALSE,FALSE,FALSE,0,0},
  {"ProfileOSD-->1/4(64x64)","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000  3 256 0 0 64  64 0 0 0",FALSE,NULL,NULL,0,FALSE,FALSE,FALSE,0,0},
  {"ProfileOSD-->1/2(128x128)","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000  3 512 0 0 128 128 0 0 0",FALSE,NULL,NULL,0,FALSE,FALSE,FALSE,0,0},
  {"ProfileOSD-->1/1(252x251)","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000  3 1024 0 0 256 251 0 0 0",FALSE,NULL,NULL,0,FALSE,FALSE,FALSE,0,0},
  {"ProfileOSD-->x2(512x512)","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000  3 2048 0 0 512 512 0 0 0",FALSE,NULL,NULL,0,FALSE,FALSE,FALSE,0,0},
  {"ProfileOSD-->x4(1024x1024)","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000  3 4096 0 0 1024 1024 0 0 0",FALSE,NULL,NULL,0,FALSE,FALSE,FALSE,0,0},
  {"ProfileOSD-->x8(2048x2048)","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 252 251 0x5612000  3 8192 0 0 2048 2048 0 0 0",FALSE,NULL,NULL,0,FALSE,FALSE,FALSE,0,0},
  {"ProfileOSD-->x2048(1x1_2048x2048)","imgrzemu.osdman1 0x29fc000 1 3 1008 0 0 1 1 0x5612000  3 8192 0 0 2048 2048 0 0 0",FALSE,NULL,NULL,0,FALSE,FALSE,FALSE,0,0},
  {"ProfileOSD-->1/2048(2048x2048_1x1)","imgrzemu.osdman1 0x29fc000 1 3 8192 0 0 2048 2048 0x5612000  3 1008 0 0 1 1 0 0 0",FALSE,NULL,NULL,0,FALSE,FALSE,FALSE,0,0},
  {"ProfileOSD-->W_2048(4x4_8192x4)","imgrzemu.osdman1 0x29fc000 1 3 8192 0 0 4 4 0x5612000  3 1024 0 0 8192 4 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:/jpg_test/dump/upload","osd15_argb8888_W_2048.bin",0x5612000,0x20000},
  {"ProfileOSD-->W_1/2048(8192x4_4x4)","imgrzemu.osdman1 0x29fc000 1 3 8192 0 0 8192 4 0x5612000  3 1024 0 0 4 4 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:/jpg_test/dump/upload","osd15_argb8888_W_1_2048.bin",0x5612000,0x40},
  {"ProfileOSD-->H_2048(4x4_4x8192)","imgrzemu.osdman1 0x29fc000 1 3 8192 0 0 4 4 0x5612000  3 1024 0 0 4 8192 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:/jpg_test/dump/upload","osd15_argb8888_H_2048.bin",0x5612000,0x20000},
  {"ProfileOSD-->H_1/2048(4x8192_4x4)","imgrzemu.osdman1 0x29fc000 1 3 8192 0 0 4 8192 0x5612000  3 1024 0 0 4 4 0 0 0",FALSE,NULL,NULL,0,TRUE,"D:/jpg_test/dump/upload","osd15_argb8888_H_1_2048.bin",0x5612000,0x40},  
  {NULL,NULL,NULL,FALSE,NULL,0,FALSE,0,0}
};

T_AUTO_TEST_CASE pAutoSetTimeProfile2[]=
{
  //OSD ODE
  {"loadSourcefile",NULL,TRUE,"D:\\jpg_test\\dump","lena_252x252.8888",0x29fc000,FALSE,NULL,NULL,0,0},
  {"ProfileOSD11920x1080","imgrzemu.osdman1	0x29fc000 1 3 1008 0 0 252 251 0x5612000 3 7680 0 0 1920 1080",FALSE,NULL,NULL,0,FALSE,FALSE,FALSE,0,0},
  {"ProfileOSD14x4","imgrzemu.osdman1	0x29fc000 1 3 1008 0 0 252 251 0x5612000 3 16 0 0 4 4",FALSE,NULL,NULL,0,FALSE,FALSE,FALSE,0,0},

  //VDO MODE
  {"loadVDO mb420_720x480Y",NULL,TRUE,"D:\\jpg_test\\dump","luma_mb_720x480.bin",0x29fc000,FALSE,NULL,NULL,0,0},
  {"loadVDO mb420_720x480C",NULL,TRUE,"D:\\jpg_test\\dump","chroma_mb_720x240.bin",0x2c7c000,FALSE,NULL,NULL,0,0},
  {"ProfileVDO1-->1920x1080","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 0 0 0x5612000 0 1920 0 0 1920 1080 0x5f0e000 960 540",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"ProfileVDO2-->16x16",    "imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 240 0 0 0 0 0x5612000 0 16   0 0 16   16   0x5f0e000 8   8",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},

  
  {"loadVDO mb422_720x480Y",NULL,TRUE,"D:\\jpg_test\\dump","luma_mb_422_720x480.bin",0x29fc000,FALSE,NULL,NULL,0,0},
  {"loadVDO mb422_720x480C",NULL,TRUE,"D:\\jpg_test\\dump","chroma_mb_422_720x480.bin",0x2c7c000,FALSE,NULL,NULL,0,0},
  {"ProfileVDO3-->1920x1080","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 0 1 0 0x5612000 0 1920 0 0 1920 1080 0x5f0e000 960 1080",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"ProfileVDO4-->16x16","imgrzemu.vdoman1 0 0 0x29fc000 720 0 0 720 480 0x2c7c000 0 0 360 480 0 0 1 0 0x5612000 0 16 0 0 16 16 0x5f0e000 8 16",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},

  //JPEG MODE
  
  {"Jpg.init","jpg.jset 100",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"Jpg.load 444:1280x960","jpg.load imgrz_1280x960_444_265204.jpg 265204",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"ProfileJPG1-->422 1920x1080 Rst","imgrzemu.jdz 1 1920 1080 0 1 1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"ProfileJPG2-->422 16x16 Rst","imgrzemu.jdz 2    16   16   0 1 1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"ProfileJPG3-->ARGB8888 1920x1080","imgrzemu.jdz 3	1920 1080 1 0 -1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},
  {"ProfileJPG4-->ARGB8888 4x4","imgrzemu.jdz 4	4 4 1 0 -1 0",FALSE,NULL,NULL,0,FALSE,NULL,NULL,0,0},

  {NULL,NULL,NULL,FALSE,NULL,0,FALSE,0,0}
};

#define MAX_OUT_TEST_SET 16
T_AUTO_TEST_CASE *pAutoSet[MAX_OUT_TEST_SET]=
{
                                                                                                                             //6                                                                                                                                               10                                                                                 12
   pAutoSetOsdAuto,pAutoSetOsd,pAutoSetOsdSrcARGB8888,pAutoSetOsdSrcARGB4444,pAutoSetOsdSrcARGB1555,
   pAutoSetOsdSrcRGB565,pAutoSetVdo,pAutoSetVdo420RasterSrc,pAutoSetVdo420BlockSrc,pAutoSetVdo422RasterSrc,
   pAutoSetVdo422BlockSrc,pAutoSetVdo420BlockNormalSrc,pAutoSetJpg420PicSrc,pAutoSetJpg422PicSrc,
   pAutoSetJpg444PicSrc,pAutoSetTimeProfile
};

UCHAR *pucTestCaseInfo[MAX_OUT_TEST_SET]=
{
  " 0:Auto Stress test",
  " 1:OSD Orginal Imgrz",
  " 2:OSD SrcArgb8888",
  " 3:OSD SrcArgb4444",
  " 4:OSD SrcArgb1555",
  " 5:OSD SrcArgb565",
  " 6:Vdo Orginal Imgrz",
  " 7:Vdo Src420Raster",
  " 8:Vdo Src420Block",
  " 9:Vdo Src422Raster",
  "10:Vdo Src422Block",
  "11:Vdo SrcNormal",
  "12:Jpg Src420",
  "13:Jpg Src422",
  "14:Jpg Src444",
  "15:Test Time"
};
#endif

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define _PAUSE(X)       x_thread_delay(X)


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

static UINT32 _u4RzEmuBuf[3];


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

// imgrz_dif.h
extern void IMGRZ_DifSetMode(UINT32 u4Mode);

// for HW/SW compare use
extern void HalFlushInvalidateDCache(void);
extern BOOL HalIsDCacheEnabled(void);
extern void HalDisableDCache(void);

// extern from jpg_cmd.c
#ifdef IMGRZ_DRAM_ACCESS_BYTE_MEASUREMENT
extern void _JPGDramAccessBtye_start(void);
extern UINT32 _JPGDramAccessBtye_end(UINT32 u4GroupId);
#endif

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
INT32 RzEmuSetParam(INT32 i4ParamType,INT32 i4Param);
static INT32 _RzEmuCmdInit          (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _RzEmuCmdFast5391Test  (INT32 i4Argc, const CHAR ** szArgv);
#ifdef IMGRZ_AUTO_TEST
static INT32 _RzEmuCmdAuto2(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _RzEmuCmdJpegModeTest(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _RzEmuSet(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _RzEmuCmdAutoFilter(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _RzEmuCmdAutoCaseInfo(INT32 i4Argc, const CHAR ** szArgv);

#endif
static INT32 _RzEmuCmdAuto(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _RzEmuCmdEnDCache      (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _RzEmuCmdDiDCache      (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _RzEmuCmdCrashTest1    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _RzEmuCmdWrapTest1     (INT32 i4Argc, const CHAR ** szArgv);

// VDO
static INT32 _RzEmuVdoCmdManTest1   (INT32 i4Argc, const CHAR ** szArgv);

// JPG
static INT32 _RzEmuJpgCmdManTestS1   (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _RzEmuJpgCmdManTestD1   (INT32 i4Argc, const CHAR ** szArgv);

// OSD
static INT32 _RzEmuOsdCmdManTest1   (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _RzEmuOsdCmdManTest2   (INT32 i4Argc, const CHAR ** szArgv);

static INT32 _RzEmuOsdCmdSetMode    (INT32 i4Argc, const CHAR ** szArgv);

static INT32 _RzEmuOsdCmdDmaTest1   (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _RzEmuOsdCmdDmaTest2   (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _RzEmuOsdCmdDmaTest3   (INT32 i4Argc, const CHAR ** szArgv);


static INT32 _RzBufCompare(UINT32 *pu4Cm, UINT32 *pu4Hw, UINT32 u4Num);
//static INT32 _RzHwBufCompare(UINT8 *pu1Cm, UINT8 *pu1Hw, UINT32 u4Num);
static void _RzEmuCreateBuffers(void);
static INT32 _CLI_CMD(const CHAR* szCmd, ...);
static INT32 _RzEmuCmdAutoSave(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _RzEmuCmdAutoCase(INT32 i4Argc, const CHAR ** szArgv);


//static void _RzSetMidWriteRegion(void);


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static UINT32 _u4RzFbmStartAddr;
static UINT32 _u4RzFbmTotalSize;
static CHAR * _szRzCurDirCM = "argb8888";
static UINT32 _u4RzCurDirCMIdx = 3;
static UINT32 _u4RzScreenWidth = 720;
static UINT32 _u4RzScreenHeight = 480;
static UINT32 _u4RzSrcBufLen = 2880;    // (_u4RzScreenWidth * 4)
static RZ_JPG_SCL_PARAM_SET_T _rJpgSclParam;

#endif // #if defined(IMGRZ_ENABLE_SW_MODE)


// cli command table
static CLI_EXEC_T _arRzEmuTbl[] =
{
#if defined(IMGRZ_ENABLE_SW_MODE)
    {"init",        "i",    _RzEmuCmdInit,          NULL,   "Init whole system (when boot)", CLI_SUPERVISOR},
    {"5391",        NULL,   _RzEmuCmdFast5391Test,  NULL,   "5391 fast test all items", CLI_SUPERVISOR},
    {"auto",        "go",   _RzEmuCmdAuto,          NULL,   "Auto test all items", CLI_SUPERVISOR},
#ifdef IMGRZ_AUTO_TEST
    {"docmd",       "do",    _RzEmuCmdAuto2,         NULL,  "Auto test command set", CLI_SUPERVISOR},
    {"docmd",       "case",    _RzEmuCmdAutoCase,         NULL,  "Run One case in a test set", CLI_SUPERVISOR},
    {"setparam",        "rzset",_RzEmuSet,        NULL,  "Set Imgrz Param", CLI_SUPERVISOR},
    {"jpegdecrz",       "jdz",_RzEmuCmdJpegModeTest,        NULL,  "Set Param to Decode and Resize Picture", CLI_SUPERVISOR},
    {"jpegderzfilter",  "filter",_RzEmuCmdAutoFilter,        NULL,  "Set IMGRZ Filter Type", CLI_SUPERVISOR},
    {"AutoCaseInfo",    "csinfo",_RzEmuCmdAutoCaseInfo,        NULL,  "Display Auto test case info", CLI_SUPERVISOR},
    {"SaveData",        "save",_RzEmuCmdAutoSave,        NULL,  "Save data: filename startaddr endaddr", CLI_SUPERVISOR},
#endif
    {"endcache",    NULL,   _RzEmuCmdEnDCache,      NULL,   "Enable D-Cache", CLI_SUPERVISOR},
    {"didcache",    NULL,   _RzEmuCmdDiDCache,      NULL,   "Disable D-Cache", CLI_SUPERVISOR},
    {"crash1",      NULL,   _RzEmuCmdCrashTest1,    NULL,   "Crash Test 1", CLI_SUPERVISOR},
    {"wrap1",        "w1",  _RzEmuCmdWrapTest1,     NULL,   "CmdQ Wrap test 1", CLI_SUPERVISOR},

    {"osdmode",     NULL,   _RzEmuOsdCmdSetMode,    NULL,   "set sw/hw and direct-cm", CLI_SUPERVISOR},
    {"vdoman1",     NULL,   _RzEmuVdoCmdManTest1,   NULL,   "VDO manual test 1", CLI_SUPERVISOR},
    {"jpgmans1",    NULL,   _RzEmuJpgCmdManTestS1,  NULL,   "JPG manual test src 1", CLI_SUPERVISOR},
    {"jpgmand1",    NULL,   _RzEmuJpgCmdManTestD1,  NULL,   "JPG manual test dst 1", CLI_SUPERVISOR},
    {"osdman1",     NULL,   _RzEmuOsdCmdManTest1,   NULL,   "OSD manual test 1", CLI_SUPERVISOR},
    {"osdman2",     NULL,   _RzEmuOsdCmdManTest2,   NULL,   "OSD manual test 2", CLI_SUPERVISOR},

    {"osddma1",     NULL,   _RzEmuOsdCmdDmaTest1,   NULL,  "OSD dma test 1 (basic)", CLI_SUPERVISOR},
    {"osddma2",     NULL,   _RzEmuOsdCmdDmaTest2,   NULL,  "OSD dma test 2 (single)", CLI_SUPERVISOR},
    {"osddma3",     NULL,   _RzEmuOsdCmdDmaTest3,   NULL,  "OSD dma test 3 (random)", CLI_SUPERVISOR},
    
#endif // #if defined(IMGRZ_ENABLE_SW_MODE)

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


#if defined(IMGRZ_ENABLE_SW_MODE)

// direct color mode table
static CHAR * _aszRzDirCMTable[] =
{
    "rgb565",
    "argb1555",
    "argb4444",
    "argb8888",
    NULL
};

// screen width table
static UINT32 _au4RzScreenWidthTable[] =
{
    720,
    720,
    720,
    720
};

// screen height table
static UINT32 _au4RzScreenHeightTable[] =
{
    480,
    480,
    480,
    480
};

// default buffer length table
static UINT32 _au4RzDefBufLenTable[] =
{
    1440,
    1440,
    1440,
    2880
};


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
static void _IMGRZ_CheckSum(void *pvParam, UINT32 u4Type);

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
/** _RzEmuOsdCmdSetMode
 *  HW/SW mode
 *  direct color mode selection
 */
//-------------------------------------------------------------------------
static INT32 _RzEmuOsdCmdSetMode(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc == 1)
    {
        //Printf("In Hardware Mode\n");
        IMGRZ_DifSetMode(E_RZ_HW_MOD);
    }
    else if (i4Argc == 2 && x_strncmp("sw", szArgv[1], 2) == 0)
    {
        //Printf("In Software Mode\n");
        IMGRZ_DifSetMode(E_RZ_SW_MOD);        
    }
    else if (i4Argc == 3 && x_strncmp("cm", szArgv[1], 2) == 0)
    {
        INT32 i4Index;

        for (i4Index = 0; _aszRzDirCMTable[i4Index] != (CHAR *)NULL; i4Index++)
        {
            if (x_strcmp(_aszRzDirCMTable[i4Index], szArgv[2]) == 0 ||
                ((szArgv[2][0] >= '0' && szArgv[2][0] <= '9') && (i4Index == StrToInt(szArgv[2]))))
            {
                if (i4Index >= D_RZ_TOTAL_DIR_CM)
                {
                    Printf("Currently direct colormode %s is not supported\n", 
                        _aszRzDirCMTable[i4Index]);
                    return -1;
                }
                _u4RzCurDirCMIdx   = (UINT32)i4Index;
                _szRzCurDirCM      = _aszRzDirCMTable[i4Index];
                _u4RzSrcBufLen     = _au4RzDefBufLenTable[i4Index];
                _u4RzScreenWidth   = _au4RzScreenWidthTable[i4Index];
                _u4RzScreenHeight  = _au4RzScreenHeightTable[i4Index];

                Printf("turn off osd plane 2\n");
                
                _CLI_CMD("osd.plane.enable 2 0");
                
                Printf("flip to list and region %d (mode = %s, buflen = %u)\n",
                    i4Index, _szRzCurDirCM, _u4RzSrcBufLen);
                
                _CLI_CMD("osd.plane.flip 2 %d", i4Index);
                
                Printf("turn on osd plane 2\n");
                
                _CLI_CMD("osd.plane.enable 2 1");

                // only ARGB8888 is big_endian
                _CLI_CMD("osd.region.set %d big_endian 0", i4Index);
                _CLI_CMD("osd.region.set %d mix_sel 1", i4Index);
                
                Printf("set source and target to default\n");
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
/** _RzEmuCmdInit
 *  initialization for emulation
 */
//-------------------------------------------------------------------------
static INT32 _RzEmuCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    //==========================================
    //...
    //==========================================
    Printf("...\n");
    Printf("name=%s\n",szArgv[0]);
    // create buffers for emulation use
    _RzEmuCreateBuffers();
    _CLI_CMD("fbm.i");
    //_RzSetMidWriteRegion();

#if 0
    //==========================================
    //init display flow
    //==========================================
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

    _CLI_CMD("osd.region.new 720 480 %u 11 1440 0 0 720 480", RZ_EMU_OSD_SCREEN_BUF); // 0 - RGB565 720x480
    _CLI_CMD("osd.region.new 720 480 %u 12 1440 0 0 720 480", RZ_EMU_OSD_SCREEN_BUF); // 1 - ARGB1555 720x480
    _CLI_CMD("osd.region.new 720 480 %u 13 1440 0 0 720 480", RZ_EMU_OSD_SCREEN_BUF); // 2 - ARGB4444 720x480
    _CLI_CMD("osd.region.new 720 480 %u 14 2880 0 0 720 480", RZ_EMU_OSD_SCREEN_BUF); // 3 - ARGB8888 720x480
    _CLI_CMD("osd.region.insert 0 0");
    _CLI_CMD("osd.region.insert 1 1");
    _CLI_CMD("osd.region.insert 2 2");
    _CLI_CMD("osd.region.insert 3 3");

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
    // init 
    //==========================================
#ifdef IMGRZ_AUTO_TEST
    x_memset(&_ImgrzVerParam,0,sizeof(T_IMGRZ_VERIFY_PARAM));
    RzEmuSetParam(E_IMGRZ_VERIFY_MULTI_INIT,0);
    #ifdef IMGRZ_HW_A_BLENDING
    x_memset((void *)IMGRZ_HW_A_BLENDING_PRELOAD_ADDR,0,0x1000000);
    #endif
#endif

    Printf("IMGRZ Emu init ...\n");
    _CLI_CMD("imgrz.init");
    _CLI_CMD("imgrz.flush off");
    Printf("IMGRZ Emu init ... done\n");

    //==========================================
    //init emu cases - kodak pictures
    //==========================================
    Printf("    cases init ...\n");
    Printf("    cases init ... kodak pictures loaded\n");

    //==========================================
    //init emu cases - yuv source (7 frames)
    //==========================================
    Printf("    cases init ... yuv source loaded\n");

    Printf("IMGRZ emulation system ready.\n");

    #ifdef IMGRZ_HW_V4
    	#ifdef IMGRZ_HW_IO_MMU
	 		_IMGRZ_MMU_int();
    	#endif
    #endif
 
    srand(29871827);

    return 0;
}

//-------------------------------------------------------------------------
/** _RzEmuCmdFast5391Test
 *  Test all 5391 functions (fast)
 */
//-------------------------------------------------------------------------
static INT32 _RzEmuCmdFast5391Test(INT32 i4Argc, const CHAR ** szArgv)
{
    _CLI_CMD("imgrzemu.mode cm %d", 0);
    Printf("\n");
    Printf("====================\n");
    Printf("5391 IMGRZ Fast Test -> Auto Test OK !\n");
    Printf("====================\n");
    return 0;
}

//-------------------------------------------------------------------------
/** _RzEmuCmdAuto
 *  Auto Test All Functions
 */
//-------------------------------------------------------------------------

#ifdef IMGRZ_AUTO_TEST
extern INT32 _JpgVerifyCmdGetParam(INT32 i4ParamType, INT32 Param);
static INT32 _RzEmuSet(INT32 i4Argc, const CHAR ** szArgv)
{
	E_IMGRZ_VERIFY_PARAM e_ImgrzParam=E_IMGRZ_VERIFY_MULTI_INIT;
	BOOL bIsCLI=TRUE;
	
	if(i4Argc&0xffff0000)
	{
		bIsCLI=FALSE;
		e_ImgrzParam=(E_IMGRZ_VERIFY_PARAM)(0xffff&i4Argc);
	}
	else
	{
	  if(i4Argc>1 )
	  {
	     e_ImgrzParam=(E_IMGRZ_VERIFY_PARAM)(StrToInt(szArgv[1]));
	  }
	  else
	  {
	     e_ImgrzParam=E_IMGRZ_VERIFY_SHOW_HELP;
	  }
	}

    switch(e_ImgrzParam)
    {
		case  E_IMGRZ_VERIFY_BURST_LIMIT:
		case  E_IMGRZ_VERIFY_H_FILTER:
		case  E_IMGRZ_VERIFY_V_FILTER:
		case  E_IMGRZ_VERIFY_TIME_PROFILE:
		case  E_IMGRZ_VERIFY_BREAK_RESUME:
	    case  E_IMGRZ_VERIFY_NEWBUF_CFG:
        {
			INT32 *pShiftBase=NULL,i4ParamShift=0,i4ParamValue=0;
			pShiftBase= &_ImgrzVerParam.i4ImgrzBustLimit;
			i4ParamShift=(e_ImgrzParam-E_IMGRZ_VERIFY_BURST_LIMIT);
			i4ParamValue=bIsCLI ? (INT32)StrToInt(szArgv[2]) : (INT32)szArgv;
			pShiftBase[i4ParamShift]=i4ParamValue;
			break;
		}
		case E_IMGRZ_VERIFY_OFFSET:
		{
		   INT32 i4PicFormant=0,i4Hfactor,i4Vfactor;
		   INT32 *Param=(INT32 *)szArgv;
           if(bIsCLI && i4Argc<7)
           	{
           	   Printf("arg:Picformat(0:420,1:422,2:444) SrcOffsetH SrcOffsetV TargetoffsetH TargetOffsetV\n");
			   Printf("SrcYOffsetH=%d,ScrYOffsetV=%d\n",_ImgrzVerParam.arImgrzOffSet[0][0],_ImgrzVerParam.arImgrzOffSet[0][1]);
			   Printf("SrcCOffsetH=%d,ScrCOffsetV=%d\n",_ImgrzVerParam.arImgrzOffSet[1][0],_ImgrzVerParam.arImgrzOffSet[1][1]);
			   Printf("TargetOffsetH=%d,TargetOffsetV=%d\n",_ImgrzVerParam.arImgrzOffSet[2][0],_ImgrzVerParam.arImgrzOffSet[2][1]);
			   break;
           	}

		   i4PicFormant= bIsCLI ? (INT32)StrToInt(szArgv[2]) : Param[0];
		   i4Hfactor= i4PicFormant==2 ? 1 : 2;
		   i4Vfactor= i4PicFormant==0 ? 2 : 1;
		   	
		   _ImgrzVerParam.arImgrzOffSet[0][0]=bIsCLI ? (INT32)StrToInt(szArgv[3]) : Param[1];
		   _ImgrzVerParam.arImgrzOffSet[0][1]=bIsCLI ? (INT32)StrToInt(szArgv[4]) : Param[2];

		   _ImgrzVerParam.arImgrzOffSet[1][0]=_ImgrzVerParam.arImgrzOffSet[0][0]/i4Hfactor;
		   _ImgrzVerParam.arImgrzOffSet[1][1]=_ImgrzVerParam.arImgrzOffSet[0][1]/i4Vfactor;

		   _ImgrzVerParam.arImgrzOffSet[2][0]=bIsCLI ? (INT32)StrToInt(szArgv[5]) : Param[3];
		   _ImgrzVerParam.arImgrzOffSet[2][1]=bIsCLI ? (INT32)StrToInt(szArgv[6]) : Param[4];
           break;
		}
		case E_IMGRZ_VERIFY_BUFFER_CFG:
		{
		   INT32 *Param=(INT32 *)szArgv;
		   if(bIsCLI && i4Argc<6)
		   {
			  Printf("Now Cfg:Input1=0x%x Input2=0x%x Output1=0x%x Output2=0x%x\n",_rImgBufCfg.u4InputAddr1,_rImgBufCfg.u4InputAddr2,_rImgBufCfg.u4OutPutAddr1,_rImgBufCfg.u4OutPutAddr2);
              return 0;
		   }
		   _rImgBufCfg.u4InputAddr1= bIsCLI ? (UINT32)StrToInt(szArgv[2]) : (UINT32)Param[0];
		   _rImgBufCfg.u4InputAddr2= bIsCLI ? (UINT32)StrToInt(szArgv[3]) : (UINT32)Param[1];
		   _rImgBufCfg.u4OutPutAddr1= bIsCLI ? (UINT32)StrToInt(szArgv[4]) : (UINT32)Param[2];
		   _rImgBufCfg.u4OutPutAddr2= bIsCLI ? (UINT32)StrToInt(szArgv[5]) : (UINT32)Param[3];
           _ImgrzVerParam.i4RZNewBufCfg=TRUE;
		   break;
		}
        case E_IMGRZ_VERIFY_MULTI_SET:
		{
		   INT32 i4ParamNum,i=0,i4ParamValue=0;
		   INT32 *Param=(INT32 *)szArgv;
		   i4ParamNum=bIsCLI ? i4Argc-2 : Param[0];

		   if(bIsCLI && !i4ParamNum)
		   {
             Printf("arg:BurstLimit(1,2,4,8) H_Filter(0,1,2) V_Filter(0,1,2)\n");
			 Printf("    i4GetRzTimeProfile(0,1)  i4RzBreakResumeTest(0,1)\n");
		   }

           for(i=0;i<i4ParamNum;i++)
           {
              i4ParamValue=bIsCLI ? (INT32)StrToInt(szArgv[2+i]) :Param[1+i];
			  if(i==0)
			    _ImgrzVerParam.i4ImgrzBustLimit=i4ParamValue;
			  else if(i==1)
			  	_ImgrzVerParam.i4ImgRzFilterH=i4ParamValue;
			  else if(i==2)
			  	_ImgrzVerParam.i4ImgRzFilterV=i4ParamValue;
			  else if(i==3)
			  	_ImgrzVerParam.i4GetRzTimeProfile=i4ParamValue;
			  else if(i==4)
			  	_ImgrzVerParam.i4RzBreakResumeTest=i4ParamValue;
           }

		   if(bIsCLI)
		   	{
			   Printf("--------------------------------------------------\n");
			   Printf("BustLimt=%d,FilterH=%d,FilterV=%d\n",_ImgrzVerParam.i4ImgrzBustLimit,_ImgrzVerParam.i4ImgRzFilterH,_ImgrzVerParam.i4ImgRzFilterV);
			   Printf("TimeProfile=%d,BreakResum=%d\n",_ImgrzVerParam.i4GetRzTimeProfile,_ImgrzVerParam.i4RzBreakResumeTest);
			   Printf("--------------------------------------------------\n");
		   	}
		}
		case E_IMGRZ_VERIFY_MULTI_INIT:
		{
           _ImgrzVerParam.i4ImgrzBustLimit=8;
		   _ImgrzVerParam.i4ImgRzFilterH=0;
		   _ImgrzVerParam.i4ImgRzFilterV=0;
		   _ImgrzVerParam.i4GetRzTimeProfile=0;
		   _ImgrzVerParam.i4RzBreakResumeTest=0;
		   _ImgrzVerParam.i4RZNewBufCfg=1;
		   x_memset(_ImgrzVerParam.arImgrzOffSet,0,sizeof(INT32)*3*2);
		   _rImgBufCfg.u4InputAddr1=_u4RzFbmStartAddr;
		   _rImgBufCfg.u4InputAddr2=_rImgBufCfg.u4InputAddr1+1024*1024*4;

		   #ifdef IMGRZ_HW_IO_MMU
		   _rImgBufCfg.u4OutPutAddr1=0x1f600800;
		   _rImgBufCfg.u4OutPutAddr2=0x1fb00000;		   
		   #else
		   _rImgBufCfg.u4OutPutAddr1=_rImgBufCfg.u4InputAddr2+1024*1024*4;
		   _rImgBufCfg.u4OutPutAddr2=_rImgBufCfg.u4OutPutAddr1+1024*1024*6;
		   #endif
           break;
		}
		case E_IMGRZ_VERIFY_SHOW_HELP:
		{
		  if(bIsCLI )
		  {
			  Printf("--------------------------------------------------\n");
			  Printf("Cmd:imgrzemu.rzset type value\n");
			  Printf(" 0-->MultiSet BurstValue H_Filter V_Filter GetRZTimeProfile DoBreakAndResumeTest\n");
			  Printf(" 1-->RzOffset(%d %d %d %d) PictureFmt(0:420 1:422 2:444) SrcOffsetX SrcOffsetY TargetOffsetX TargetOffsetY\n",_ImgrzVerParam.arImgrzOffSet[0][0],_ImgrzVerParam.arImgrzOffSet[0][1],_ImgrzVerParam.arImgrzOffSet[2][0],_ImgrzVerParam.arImgrzOffSet[2][1]);
			  Printf(" 2-->BustLimt(%d) (1,2,4,8)\n",_ImgrzVerParam.i4ImgrzBustLimit);
			  Printf(" 3-->HFilter(%d) (0:Default 1:HSA Filter 2:8Tap Filter)\n",_ImgrzVerParam.i4ImgRzFilterH);
			  Printf(" 4-->VFilter(%d) (0:Default 1:SCL Filter 2:4Tap Filter)\n",_ImgrzVerParam.i4ImgRzFilterV);
			  Printf(" 5-->TimeProfile(%d) (if set,get time used by resize)\n",_ImgrzVerParam.i4GetRzTimeProfile);
			  Printf(" 6-->i4RzBreakResumeTest(%d) (Use in JPEG Mode,if set,Jpeg MCU-Row+IMGRZ mode will do stop and resume)\n",_ImgrzVerParam.i4RzBreakResumeTest);
			  Printf(" 7-->UseNewBufferCfg(%d) (if set,use _rImgBufCfg,else use address in cli command)\n",_ImgrzVerParam.i4RZNewBufCfg);
			  Printf(" 8-->SetBufferCfg(0x%x 0x%x 0x%x 0x%x) InputAddr1 InputAddr2 OutputAddr1 OutputAddr2\n",_rImgBufCfg.u4InputAddr1,_rImgBufCfg.u4InputAddr2,_rImgBufCfg.u4OutPutAddr1,_rImgBufCfg.u4OutPutAddr2);
			  Printf("100-->Init\n200->Help\n");
			  Printf("--------------------------------------------------\n");
 		  }
          break;
		}
		
		default:
		{
           Printf("Param %d not support\n",e_ImgrzParam);
		}

    }

	return 0;
}

INT32 RzEmuGetParam(INT32 i4ParamType,INT32 i4Param)
{
  switch(i4ParamType)
  {
    	case  E_IMGRZ_VERIFY_BURST_LIMIT:
		case  E_IMGRZ_VERIFY_H_FILTER:
		case  E_IMGRZ_VERIFY_V_FILTER:
		case  E_IMGRZ_VERIFY_TIME_PROFILE:
		case  E_IMGRZ_VERIFY_BREAK_RESUME: 
     	{
		   INT32 *pShiftBase=NULL,i4ParamShift=0;
		   pShiftBase= &_ImgrzVerParam.i4ImgrzBustLimit;
		   i4ParamShift=(i4ParamType-E_IMGRZ_VERIFY_BURST_LIMIT);
		   if(i4Param)
		   *((INT32 *)i4Param)=pShiftBase[i4ParamShift];
		   return pShiftBase[i4ParamShift];
   	    }

		case E_IMGRZ_VERIFY_OFFSET:
		{
           if(i4Param)
		   *((INT32 *)i4Param)=(INT32)_ImgrzVerParam.arImgrzOffSet;
		   break;
		}
		
		default:
		{
		   Printf("Not Support %d,Please Eidt This Functin\n",i4ParamType);
		   break;
		}
  }
  return 0;
}

INT32 RzEmuSetParam(INT32 i4ParamType,INT32 i4Param)
{
   _RzEmuSet(0xffff0000|i4ParamType,(const CHAR **)i4Param);
   return 0;
}

static INT32 _RzEmuCmdJpegModeTest(INT32 i4Argc, const CHAR ** szArgv)
{
   INT32 i4index=0,i4TarW=0,i4TarH=0,i4Rst_CM=0,i4IsVdo2Osd=0,i4OutMode=0,i4OutSwapMode=0;
   INT32 i4OffsetH=0,i4OffsetV=0,i4BurstLimit=8;
   CHAR szUpLoadName[2][64]={{0},{0}};

   if(i4Argc<8)
   {
     Printf("Index,TarWidth,TarHeight,isVdo2Osd,i4Rst_CM,OutputMode,OutSwapMode\n");
	 return 0;
   }
   
   i4index=StrToInt(szArgv[1]);
   i4TarW=StrToInt(szArgv[2]);
   i4TarH=StrToInt(szArgv[3]);
   i4IsVdo2Osd=StrToInt(szArgv[4]);
   i4Rst_CM=StrToInt(szArgv[5]);
   i4OutMode=StrToInt(szArgv[6]);
   i4OutSwapMode=StrToInt(szArgv[7]);
   
   if(i4Argc==11)
   {
	   i4OffsetH=(INT32)StrToInt(szArgv[8]);
	   i4OffsetV=(INT32)StrToInt(szArgv[9]);
	   i4BurstLimit=(INT32)StrToInt(szArgv[10]);
	   _ImgrzVerParam.i4ImgrzBustLimit=i4BurstLimit;
	   _ImgrzVerParam.arImgrzOffSet[0][0]=_ImgrzVerParam.arImgrzOffSet[0][1]=0;
	   _ImgrzVerParam.arImgrzOffSet[1][0]=_ImgrzVerParam.arImgrzOffSet[1][1]=0;
	   _ImgrzVerParam.arImgrzOffSet[2][0]=i4OffsetH;
	   _ImgrzVerParam.arImgrzOffSet[2][1]=i4OffsetV;
   }

   _CLI_CMD("jpg.wh %d %d",i4TarW,i4TarH);
   _CLI_CMD("jpg.jset 0 3 %d %d %d %d",i4IsVdo2Osd,i4Rst_CM,i4OutMode,i4OutSwapMode);
   _CLI_CMD("jpg.de");
   
   sprintf(szUpLoadName[0],"%d%s_%s_%dx%d_%s.bin",i4index,i4IsVdo2Osd ? "osd" : "Y", \
   	      i4IsVdo2Osd ? (i4Rst_CM==0 ? "argb8888" : (i4Rst_CM==1 ? "ycbycr4444" : "cbycry4444")):(i4Rst_CM ? (i4OutMode==0 ? "r_420" : (i4OutMode==1 ? "r_422" : "r_444")) : (i4OutMode==0 ? "b_420" : (i4OutMode==1 ? "b_422" : "b_444"))), \
   	      i4TarW,i4TarH,i4OutSwapMode==6 ? "normal" : "1389");
   sprintf(szUpLoadName[1],"%d%s_%s_%dx%d_%s.bin",i4index,i4IsVdo2Osd ? "osd" : "C", \
   	      i4IsVdo2Osd ? (i4Rst_CM==0 ? "argb8888" : (i4Rst_CM==1 ? "ycbycr4444" : "cbycry4444")):(i4Rst_CM ? (i4OutMode==0 ? "r_420" : (i4OutMode==1 ? "r_422" : "r_444")) : (i4OutMode==0 ? "b_420" : (i4OutMode==1 ? "b_422" : "b_444"))), \
   	      i4TarW,i4TarH,i4OutSwapMode==6 ? "normal" : "1389");
   
   if(i4IsVdo2Osd)
   	{
   	   UINT32 u4Size,u4SizeFactor=0,u4I=0,u4Sum=0;
	   UCHAR *pu1Ptr=NULL;
	   u4SizeFactor= (i4Rst_CM==0 ? 2 : 1);
	   u4Size = (i4TarW * i4TarH) << u4SizeFactor;
       _JpgVerifyCmdGetParam(6,(UINT32)&pu1Ptr);

       if(pu1Ptr==NULL)
       {
         Printf("Y Target Address is Null\n");
       }
	   else
	   {
		   for (u4I = 0; u4I < u4Size; u4I++)
		   {
			   u4Sum += ((UINT32)(*(pu1Ptr + u4I)));
		   }
		   Printf("checksum: %x\n", u4Sum);
	   }
	   _CLI_CMD("jpg.save %s",szUpLoadName[0]);
   	}
    else
   	{
		UINT32 u4Size,u4I=0,u4Sum=0,u4SizeFactor=0;
		UCHAR *pu1Ptr=NULL;
		u4Size = (i4TarW * i4TarH);
		_JpgVerifyCmdGetParam(6,(UINT32)&pu1Ptr);
		if(pu1Ptr==NULL)
		{
		   Printf("Y Target Address is Null\n");
		}
		else
		{
		  for (u4I = 0; u4I < u4Size; u4I++)
		  {
			 u4Sum += ((UINT32)(*(pu1Ptr + u4I)));
		  }
		  
		  Printf("Y checksum: %x\n", u4Sum);
		}

        pu1Ptr=NULL;
		u4Sum=0;
		u4SizeFactor= i4OutMode==0 ? 1 : 0;
		u4Size = (i4TarW * i4TarH)>>u4SizeFactor;
		_JpgVerifyCmdGetParam(7,(UINT32)&pu1Ptr);
		if(pu1Ptr==NULL)
		{
		   Printf("C Target Address is Null\n");
		}
		else
		{
		  for (u4I = 0; u4I < u4Size; u4I++)
		  {
			 u4Sum += ((UINT32)(*(pu1Ptr + u4I)));
		  }
		  
		  Printf("C checksum: %x\n", u4Sum);
		}
	   _CLI_CMD("jpg.save %s %s",szUpLoadName[0],szUpLoadName[1]);
   	}

   return 0;
}

/*
VOID _RzEmuCmdFilterType(UINT8 uHorV,INT32 *i4FilterType)
{
  if(!i4FilterType)
  	return;

  if(uHorV)
  	*i4FilterType=_i4ImgRzFilterH;
  else
	*i4FilterType=_i4ImgRzFilterV;
  return;
}
*/

#ifdef IMGRZ_SEMI_HOST_BY_USB
#include "x_fm.h"
BOOL _RZLoadUsbFile(CHAR *pcFileName, UINT32 u4Addr)
{
    UINT64 u8FileSize, u8Tmp;
    UINT32 u4Request, u4Read;
    HANDLE_T  h_file;

    VERIFY(FMR_OK == x_fm_open(FM_ROOT_HANDLE, pcFileName, FM_READ_ONLY, 0777, FALSE, &h_file));
    VERIFY(FMR_OK == x_fm_lseek(h_file, 0, FM_SEEK_END, &u8FileSize));
    VERIFY(FMR_OK == x_fm_lseek(h_file, 0, FM_SEEK_BGN, &u8Tmp));

    while (u8FileSize > 0)
    {
        if (u8FileSize >> 32)
        {
            u4Request = 0xffffffff;
        }
        else
        {
            u4Request = (UINT32)u8FileSize;
        }
        x_fm_read(h_file, (VOID *)u4Addr, u4Request, &u4Read);
        ASSERT(u4Request == u4Read);
        u4Addr += u4Read;
        u8FileSize -= u4Read;
    }

    VERIFY(FMR_OK == x_fm_close(h_file));

    return TRUE;
}
#endif

static INT32 _RzEmuCmdAutoSave(INT32 i4Argc, const CHAR ** szArgv)
{
  const CHAR *pucPashName=szArgv[1];
  UINT32 u4StartAddr=0,u4EndAddr=0;

  if(i4Argc<4)
  {
     Printf("Parameter: filename startaddr endaddr\n");
	 return 0;
  }
  u4StartAddr=StrToInt(szArgv[2]);
  u4EndAddr=StrToInt(szArgv[3]);
  
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396)
    HalFlushInvalidateDCacheMultipleLine((UINT32)(u4StartAddr), (UINT32)(u4EndAddr- u4StartAddr));
#else
    HalFlushInvalidateDCache();
#endif
  UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s\" 0x%08x--0x%08x",pucPashName,u4StartAddr,u4EndAddr));

  // sync ram and cache
#if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)
    //HalFlushInvalidateDCacheMultipleLine((UINT32)(u4StartAddr), (UINT32)(u4EndAddr- u4StartAddr));
#else
    //HalFlushInvalidateDCache();
#endif
  
  return 0;
}
static INT32 _RzEmuCmdAutoFilter(INT32 i4Argc, const CHAR ** szArgv)
{
   if(i4Argc<3)
   {
     Printf("H(0:default,1:Acc,2:8-tap)  V(0:default,1:Acc,2:4-tap)\n");
	 return 0;
   }

   _ImgrzVerParam.i4ImgRzFilterH=StrToInt(szArgv[1]);
   _ImgrzVerParam.i4ImgRzFilterV=StrToInt(szArgv[2]);
  return 0;
}

static VOID _RzRunAutoTest(P_AUTO_TEST_CASE pAutoSet,INT32 i4SetId)
{
   INT32 i4Case=0;
   UINT32 u4InAddr,u4OutAddr;
#ifdef IMGRZ_SEMI_HOST_BY_USB
   CHAR *pcImgrzFileName;
#endif
   
   while(pAutoSet[i4Case].szName)
   {   
       Printf("In Set %d,Start Case %d->%s\n",i4SetId,i4Case,pAutoSet[i4Case].szName);
       if(pAutoSet[i4Case].fg_load)
       	{
       	   if(_ImgrzVerParam.i4RZNewBufCfg)
       	   {
			   if(pAutoSet[i4Case].u4LoadAddr==0x29fc000)
			   {
				  u4InAddr=_rImgBufCfg.u4InputAddr1;
			   }
			   else if(pAutoSet[i4Case].u4LoadAddr==0x2c7c000)
			   {
				  u4InAddr=_rImgBufCfg.u4InputAddr2;
			   }
			   #ifdef IMGRZ_HW_A_COMPOSITION
			   else if(pAutoSet[i4Case].u4LoadAddr==0xc000000)
			   {
				  u4InAddr=IMGRZ_HW_A_COMPOSITION_PRELOAD_ADDR;
			   }			   
			   #endif
			   else
			   {
				  Printf("!!!!!!!!Input Addr Error %s,0x%x\n",pAutoSet[i4Case].szName,pAutoSet[i4Case].u4LoadAddr);
				  break;
			   }  
		   }
		   else
		   {
               u4InAddr=pAutoSet[i4Case].u4LoadAddr;
		   }
 		
		   //LOG(0, "d.load.binary \"%s/%s\" 0x%08x /ny",pAutoSet[i4Case].szLoadPath,pAutoSet[i4Case].szLoadFile,pAutoSet[i4Case].u4LoadAddr);
		   
            #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)
                //HalFlushInvalidateDCacheMultipleLine((UINT32)(u4InAddr), (UINT32)(pAutoSet[i4Case].u4SaveSize));
            #else
                //HalFlushInvalidateDCache();
            #endif

#ifdef IMGRZ_SEMI_HOST_BY_USB
	     x_sprintf((CHAR *)pAutoSet[i4Case].szLoadPath , "/mnt/usb_0/Jpg_test/dump/");		
	     pcImgrzFileName = (CHAR *)pAutoSet[i4Case].szLoadPath;	
            //Printf("115 imgrz load %s \n", pcImgrzFileName);    	     
   	     x_strcat(pcImgrzFileName, (CHAR *)pAutoSet[i4Case].szLoadFile);
            //Printf("116 imgrz load %s \n", pcImgrzFileName);              
            //Printf("117 load addr %x \n", u4InAddr);
            _RZLoadUsbFile(pcImgrzFileName, u4InAddr);
#else
	//Printf("11 imgrz load %s\\%s  \n", pAutoSet[i4Case].szLoadPath, pAutoSet[i4Case].szLoadFile);
           UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\\%s\" 0x%08x /ny",pAutoSet[i4Case].szLoadPath,pAutoSet[i4Case].szLoadFile,u4InAddr));
#endif  


            // sync ram and cache
            #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)
                HalFlushInvalidateDCacheMultipleLine((UINT32)(u4InAddr), (UINT32)(pAutoSet[i4Case].u4SaveSize));
            #else
                HalFlushInvalidateDCache();
            #endif
		
	    }
	
       if(pAutoSet[i4Case].szCmd)
       	{
           CLI_Parser((CHAR *)pAutoSet[i4Case].szCmd);
		   //x_thread_delay(100);
       	}

	    if(_fgErrHappen)
	    {
			Printf("!!!!!!!!!!!Error Error!!!!!!!!!!!!!!!!!\n");
			break;
	    }

	if(pAutoSet[i4Case].fg_save)
	{

	    if(_ImgrzVerParam.i4RZNewBufCfg)
	    {
			if(pAutoSet[i4Case].u4SaveStartAddr==0x5612000)
			{
			   u4OutAddr=_rImgBufCfg.u4OutPutAddr1;
			}
			else if(pAutoSet[i4Case].u4SaveStartAddr==0x5f0e000)
			{
			   u4OutAddr=_rImgBufCfg.u4OutPutAddr2;
			}
			else
			{
			   Printf("!!!!!!!!Output Addr Error %s,0x%x\n",pAutoSet[i4Case].szName,pAutoSet[i4Case].u4SaveStartAddr);
			   break;
			}
	    }
        else
        {
            u4OutAddr=pAutoSet[i4Case].u4SaveStartAddr;
        }
#if 1	
	    _CLI_CMD("imgrzemu.save %s/set%d_%s %d %d",pAutoSet[i4Case].szSavePath,i4SetId,pAutoSet[i4Case].szSaveFile,u4OutAddr,u4OutAddr+pAutoSet[i4Case].u4SaveSize-1);
#else
	    //LOG(0, "d.save.binary \"%s/%s\" 0x%08x--0x%08x \n",pAutoSet[i4Case].szSavePath,pAutoSet[i4Case].szSaveFile,pAutoSet[i4Case].u4SaveStartAddr,pAutoSet[i4Case].u4SaveStartAddr+pAutoSet[i4Case].u4SaveSize-1);
            HalFlushInvalidateDCache();
	    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s\\%s\" 0x%08x--0x%08x",pAutoSet[i4Case].szSavePath,pAutoSet[i4Case].szSaveFile,u4OutAddr,u4OutAddr+pAutoSet[i4Case].u4SaveSize-1));
		// sync ram and cache
            HalFlushInvalidateDCache();
	   //x_thread_delay(50000);
#endif
	}
       i4Case++;
   };
   
}

static INT32 _RzEmuCmdAutoCaseInfo(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Index=0;
	
	Printf("================================================\n");
	Printf("Auto Test Case Count is %d\n",MAX_OUT_TEST_SET);
	for(i4Index=0;i4Index<MAX_OUT_TEST_SET;i4Index++)
	{
       Printf("%s\n",pucTestCaseInfo[i4Index]);
	}
	Printf("================================================\n");
	return 0;
}

static INT32 _RzEmuCmdAutoCase(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4SetId,i4CaseId;
	UINT32 u4InAddr,u4OutAddr;
	T_AUTO_TEST_CASE *pDoCase,*pDoSet;
    if (i4Argc < 3)
    {
        Printf("Args: SetId CaseId\n");
        return -1;
    }

	i4SetId=(INT32) StrToInt(szArgv[1]);
	i4CaseId=(INT32) StrToInt(szArgv[2]);

	pDoSet=pAutoSet[i4SetId];
	pDoCase=pDoSet+i4CaseId;
    Printf("In Set %d,Start Case %d->%s\n",i4SetId,i4CaseId,pDoCase->szName);
    if(pDoCase->fg_load)
    {
	   if(_ImgrzVerParam.i4RZNewBufCfg)
	   {
		  if(pDoCase->u4LoadAddr==0x29fc000)
		  {
		    u4InAddr=_rImgBufCfg.u4InputAddr1;
		  }
		  else if(pDoCase->u4LoadAddr==0x2c7c000)
		  {
			u4InAddr=_rImgBufCfg.u4InputAddr2;
		  }
		  else
		  {
			Printf("!!!!!!!!Input Addr Error %s,0x%x\n",pDoCase->szName,pDoCase->u4LoadAddr);
			return 0;
		  }  
	   }
	   else
	   {
		  u4InAddr=pDoCase->u4LoadAddr;
	   }
			
			   //LOG(0, "d.load.binary \"%s/%s\" 0x%08x /ny",pAutoSet[i4Case].szLoadPath,pAutoSet[i4Case].szLoadFile,pAutoSet[i4Case].u4LoadAddr);
            #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)
            //HalFlushInvalidateDCacheMultipleLine((UINT32)(u4InAddr), (UINT32)(pDoCase->u4SaveSize));
            #else
            //HalFlushInvalidateDCache();
            #endif
		//Printf("22 imgrz load %s\\%s\\  \n", pDoCase->szLoadPath, pDoCase->szLoadFile);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\\%s\" 0x%08x /ny",pDoCase->szLoadPath,pDoCase->szLoadFile,u4InAddr));
            
            // sync ram and cache
            #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)
            HalFlushInvalidateDCacheMultipleLine((UINT32)(u4InAddr), (UINT32)(pDoCase->u4SaveSize));
            #else
            HalFlushInvalidateDCache();
            #endif

	}
		
	if(pDoCase->szCmd)
	{
	   CLI_Parser((CHAR *)pDoCase->szCmd);
	}
	
	if(_fgErrHappen)
	{
	   Printf("!!!!!!!!!!!Error Error!!!!!!!!!!!!!!!!!\n");
	   return 0;
	}
	
	if(pDoCase->fg_save)
	{
	   if(_ImgrzVerParam.i4RZNewBufCfg)
	   {
		  if(pDoCase->u4SaveStartAddr==0x5612000)
		  {
			u4OutAddr=_rImgBufCfg.u4OutPutAddr1;
		  }
		  else if(pDoCase->u4SaveStartAddr==0x5f0e000)
		  {
		     u4OutAddr=_rImgBufCfg.u4OutPutAddr2;
		  }
		  else
		  {
			 Printf("!!!!!!!!Output Addr Error %s,0x%x\n",pDoCase->szName,pDoCase->u4SaveStartAddr);
			 return 0;
		  }
		}
		else
		{
		   u4OutAddr=pDoCase->u4SaveStartAddr;
		}
#if 1	
	    _CLI_CMD("imgrzemu.save %s/set%d_%s %d %d",pDoCase->szSavePath,i4SetId,pDoCase->szSaveFile,u4OutAddr,u4OutAddr+pDoCase->u4SaveSize-1);
#else
		//LOG(0, "d.save.binary \"%s/%s\" 0x%08x--0x%08x \n",pAutoSet[i4Case].szSavePath,pAutoSet[i4Case].szSaveFile,pAutoSet[i4Case].u4SaveStartAddr,pAutoSet[i4Case].u4SaveStartAddr+pAutoSet[i4Case].u4SaveSize-1);
                HalFlushInvalidateDCache();
              UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s\\%s\" 0x%08x--0x%08x",pAutoSet[i4Case].szSavePath,pAutoSet[i4Case].szSaveFile,u4OutAddr,u4OutAddr+pAutoSet[i4Case].u4SaveSize-1));
		// sync ram and cache
                HalFlushInvalidateDCache();
		//x_thread_delay(50000);
#endif
    }
	return 0;
}

static INT32 _RzEmuCmdAuto2(INT32 i4Argc, const CHAR ** szArgv)
{
   INT32 i4SetID=0,i4BstLimt=8,i4Loop=0,i4AllCase=0,i=0;
   BOOL bBst=FALSE;
    if (i4Argc < 3)
    {
        Printf("Args: SetId,DoAllFlag,[Loop,BstLimit(1,2,4,8)]\n");
        return -1;
    }

    i4SetID= (INT32) StrToInt(szArgv[1]);
	i4AllCase=(INT32) StrToInt(szArgv[2]);
	if(i4Argc>=4)
	{
	   i4Loop=(INT32) StrToInt(szArgv[3]);
	}
	else
	{
       i4Loop=1;
	}

	if(i4Argc>=5)
	{
		_ImgrzVerParam.i4ImgrzBustLimit=(INT32) StrToInt(szArgv[4]);
	}
	
	if(i4SetID>=MAX_OUT_TEST_SET)
	{
      Printf("No This Set\n");
	  return 0;
	}


    if(i4SetID==0 && i4Loop==0)
    {
       i4Loop=4;
	   _ImgrzVerParam.i4ImgrzBustLimit=1;
	   bBst=TRUE;
    }
	_fgErrHappen=FALSE;


    for(i=0;i<i4Loop;i++)
    {
      if(bBst)
      {
        i4BstLimt=_ImgrzVerParam.i4ImgrzBustLimit;
		_CLI_CMD("imgrzemu.rzset 2 %d",i4BstLimt);
		_ImgrzVerParam.i4ImgrzBustLimit=_ImgrzVerParam.i4ImgrzBustLimit<<1;
      }
	  
      if(i4AllCase)
      {
        INT32 i4Case=i4SetID;
		for(;i4Case<MAX_OUT_TEST_SET;i4Case++)
		{
		    Printf(" Imgrz Auto Test!\n");
			_RzRunAutoTest(pAutoSet[i4Case],i4Case);
		}
      }
	  else
	  {
		  _RzRunAutoTest(pAutoSet[i4SetID],i4SetID);
	  }
    }
	
    Printf("====================\n");
    Printf(" Auto Test OK !\n");
    Printf("====================\n");
   return 0;
}

#endif
static INT32 _RzEmuCmdAuto(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4SrcBase, u4SrcW, u4SrcH, u4TgBase, u4I;
    UINT32 u4Max = 2460, u4Min = 4, u4Time;
    INT32 i4TgW, i4TgH ;
    
    if (i4Argc < 5)
    {
        Printf("Args: u4SrcBase, u4SrcW, u4SrcH, u4TgBase\n");
        return -1;
    }

    u4SrcBase     = (UINT32) StrToInt(szArgv[1]);
    u4SrcW        = (UINT32) StrToInt(szArgv[2]);
    u4SrcH        = (UINT32) StrToInt(szArgv[3]);
    u4TgBase      = (UINT32) StrToInt(szArgv[4]);
    u4Time = ((u4Max - u4SrcW) / 32) + 1;
    for (u4I = 1; u4I < u4Time; u4I++)
    {
        //u4TgW = rand() % 2500;
        //u4TgH = rand() % 2500;
        i4TgW = u4SrcW + (u4I * 32);
        i4TgH = u4SrcH + (u4I * 32);
        if (i4TgW < u4SrcW)
            i4TgW = u4SrcW;
        if (i4TgH < u4SrcH)
            i4TgH = u4SrcH;
        if (i4TgW % 4)
            i4TgW = i4TgW - (i4TgW % 4) + 4;
        if (i4TgW > u4Max)
            i4TgW = u4Max;
        if (i4TgH > u4Max)
            i4TgH = u4Max;
        Printf("Do Resize:TargetW=%d,TargetH=%d\n",i4TgW,i4TgH);
        _CLI_CMD("imgrzemu.osdman1 %d 1 3 %d 0 0 %d %d %d 3 %d 0 0 %d %d",
                u4SrcBase, u4SrcW * 4, u4SrcW, u4SrcH, u4TgBase, i4TgW * 4, i4TgW, i4TgH);
    }    
    u4Time = ((u4SrcW - u4Min) / 8) + 1;
    for (u4I = 1; u4I < u4Time; u4I++)
        {
        //u4TgW = rand() % u4SrcW;
        //u4TgH = rand() % u4SrcH;
        i4TgW = u4SrcW - (u4I * 8);
        i4TgH = u4SrcH - (u4I * 8);
        if (i4TgW < 4)
            i4TgW = 4;
        if (i4TgH < 4)
            i4TgH = 4;
        if (i4TgW % 4)
            i4TgW = i4TgW - (i4TgW % 4) + 4;
        Printf("Do Resize:TargetW=%d,TargetH=%d\n",i4TgW,i4TgH);
        _CLI_CMD("imgrzemu.osdman1 %d 1 3 %d 0 0 %d %d %d 3 %d 0 0 %d %d",
                u4SrcBase, u4SrcW * 4, u4SrcW, u4SrcH, u4TgBase, i4TgW * 4, i4TgW, i4TgH);
    }

    Printf("====================\n");
    Printf(" Auto Test OK !\n");
    Printf("====================\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _RzEmuCmdEnDCache
 *  Enable D-Cache
 */
//-------------------------------------------------------------------------
static INT32 _RzEmuCmdEnDCache(INT32 i4Argc, const CHAR ** szArgv)
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
/** _RzEmuCmdDiDCache
 *  Disable D-Cache
 */
//-------------------------------------------------------------------------
static INT32 _RzEmuCmdDiDCache(INT32 i4Argc, const CHAR ** szArgv)
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
/** _RzEmuCmdCrashTest1
 *  Crash Test 01
 *  only run hw to do crash test
 */
//-------------------------------------------------------------------------
static INT32 _RzEmuCmdCrashTest1(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Loops, i4RetValue;

    if (i4Argc < 2)
    {
        Printf("Args: i4Loops\n");
        return -1;
    }

    i4Loops = (INT32) StrToInt(szArgv[1]);

    i4RetValue = _CLI_CMD("imgrzemu.osddma3 %d", i4Loops);
    if (i4RetValue == -1)   return -1;

    return 0;
}

//-------------------------------------------------------------------------
/** _RzEmuCmdWrapTest1
 *  cmdque wrap Test 01 (for cmdque buffer size = 2 KB)
 */
//-------------------------------------------------------------------------
static INT32 _RzEmuCmdWrapTest1(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size, loop;
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    
    Printf("Cmdque Wrap Test 01 (basic) \n");

	x_memset((void *)&rSclParam, 0, sizeof(RZ_OSD_SCL_PARAM_SET_T));
	
    // set scaling parameters
    rSclParam.u4SrcBase     = RZ_EMU_OSD_SRC_BUF;
    rSclParam.u4IsSrcDirCM  = IMGRZ_TRUE;
    rSclParam.u4SrcCM       = _u4RzCurDirCMIdx;
    rSclParam.u4SrcBufLen   = _u4RzSrcBufLen;
    rSclParam.u4SrcHOffset  = 0;
    rSclParam.u4SrcVOffset  = 0;
    rSclParam.u4SrcW        = _u4RzScreenWidth;
    rSclParam.u4SrcH        = _u4RzScreenHeight;

    rSclParam.u4TgBase      = RZ_EMU_OSD_SCREEN_BUF;
    rSclParam.u4TgCM        = _u4RzCurDirCMIdx;
    rSclParam.u4TgBufLen    = _u4RzSrcBufLen;
    rSclParam.u4TgHOffset   = 0;
    rSclParam.u4TgVOffset   = 0;
    rSclParam.u4TgW         = _u4RzScreenWidth;
    rSclParam.u4TgH         = _u4RzScreenHeight;
    
    // HW
    _CLI_CMD("imgrz.reset");
    _CLI_CMD("imgrzemu.osdmode");

    // Action
    _IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_OSDMD);
    for (loop = 0; loop < 30; loop++)
    {
        _IMGRZ_Scale(&rSclParam);
    }
    _CLI_CMD("imgrz.flush");

    // Compare HW and SW
    u4Size = (_u4RzScreenWidth * _u4RzScreenHeight);
    if (_RzBufCompare(
        (UINT32 *)(RZ_EMU_OSD_SRC_BUF), 
        (UINT32 *)(RZ_EMU_OSD_SCREEN_BUF), u4Size) == -1)
    {
        Printf("Cmdque Wrap Test 01 (basic) Error\n");
        return -1;
    }
        
    Printf("Cmdque Wrap Test 01 (basic) ... 1 runs OK !!\n");
    return 0;
}

//-------------------------------------------------------------------------
/** _RzEmuVdoCmdManualTest1
 *  VDO Manual Test 01 (single)
 *  manual input
 */
//-------------------------------------------------------------------------

static INT32 _RzEmuVdoCmdManTest1(INT32 i4Argc, const CHAR ** szArgv)
{
    RZ_VDO_SCL_PARAM_SET_T rSclParam;
    //Printf("VDO Manual Test 01 (single) \n");

    UINT32 u4HCOffset=0,u4VCOffset=0;
    
    if (i4Argc != 30)
    {
        Printf("Arg: SrcSwapMode,IsRsIn, YSrcBase, YSrcBufLen, \n");
        Printf("     YSrcHOffset, YSrcVOffset, YSrcW, YSrcH, \n");
        Printf("     CSrcBase, CSrcHOffset, CSrcVOffset, CSrcW, CSrcH, \n");
        Printf("     TgSwapMode,IsRsOut, OutMode, IsVdo2Osd, \n");
        Printf("     YTgBase, YTgCM, TgBufLen, \n");
        Printf("     YTgHOffset, YTgVOffset, YTgW, YTgH, \n");
        Printf("     CTgBase, CTgW, CTgH \n");
        Printf(" Argument num isn't correct \n");
        return -1;
    }

	x_memset((void *)&rSclParam, 0, sizeof(RZ_VDO_SCL_PARAM_SET_T));
	
    // set scaling parameters
    rSclParam.u4SrcSwap     = (UINT32) StrToInt(szArgv[1]);
    rSclParam.u4IsRsIn      = (UINT32) StrToInt(szArgv[2]);
    rSclParam.u4YSrcBase    = (UINT32) StrToInt(szArgv[3]);
    rSclParam.u4YSrcBufLen  = (UINT32) StrToInt(szArgv[4]);
    rSclParam.u4YSrcHOffset = (UINT32) StrToInt(szArgv[5]);
    rSclParam.u4YSrcVOffset = (UINT32) StrToInt(szArgv[6]);
    rSclParam.u4YSrcW       = (UINT32) StrToInt(szArgv[7]);
    rSclParam.u4YSrcH       = (UINT32) StrToInt(szArgv[8]);
    rSclParam.u4CSrcBase    = (UINT32) StrToInt(szArgv[9]);
    rSclParam.u4CSrcHOffset = (UINT32) StrToInt(szArgv[10]);
    rSclParam.u4CSrcVOffset = (UINT32) StrToInt(szArgv[11]);
    rSclParam.u4CSrcW       = (UINT32) StrToInt(szArgv[12]);
    rSclParam.u4CSrcH       = (UINT32) StrToInt(szArgv[13]);

	rSclParam.u4OutSwap     = (UINT32) StrToInt(szArgv[14]);
    rSclParam.u4IsRsOut     = (UINT32) StrToInt(szArgv[15]);
    rSclParam.u4OutMode     = (UINT32) StrToInt(szArgv[16]);
    rSclParam.u4IsVdo2Osd   = (UINT32) StrToInt(szArgv[17]);
    rSclParam.u4YTgBase     = (UINT32) StrToInt(szArgv[18]);
    rSclParam.u4YTgCM       = (UINT32) StrToInt(szArgv[19]);
    rSclParam.u4YTgBufLen   = (UINT32) StrToInt(szArgv[20]);
    rSclParam.u4YTgHOffset  = (UINT32) StrToInt(szArgv[21]);
    rSclParam.u4YTgVOffset  = (UINT32) StrToInt(szArgv[22]);
    rSclParam.u4YTgW        = (UINT32) StrToInt(szArgv[23]);
    rSclParam.u4YTgH        = (UINT32) StrToInt(szArgv[24]);
    rSclParam.u4CTgBase     = (UINT32) StrToInt(szArgv[25]);
    rSclParam.u4CTgW        = (UINT32) StrToInt(szArgv[26]);
    rSclParam.u4CTgH        = (UINT32) StrToInt(szArgv[27]);
    rSclParam.u4VdoCbCrSwap        = (UINT32) StrToInt(szArgv[28]);
    rSclParam.u4ABlend = (UINT32) StrToInt(szArgv[29]);
    #ifdef IMGRZ_HW_A_BLENDING
    rSclParam.u4PreLoadYAddr = IMGRZ_HW_A_BLENDING_PRELOAD_ADDR;
    rSclParam.u4PreLoadCAddr = IMGRZ_HW_A_BLENDING_PRELOAD_ADDR+0x800000;
    #endif

#ifdef IMGRZ_AUTO_TEST
        if(_ImgrzVerParam.i4RZNewBufCfg)
        {
			if(rSclParam.u4YSrcBase==0x29fc000)
			{
				rSclParam.u4YSrcBase=_rImgBufCfg.u4InputAddr1;
			}
			else if(rSclParam.u4YSrcBase==0x2c7c000)
			{
				rSclParam.u4YSrcBase=_rImgBufCfg.u4InputAddr2;
			}
			else 
			{
			
		        _fgErrHappen=TRUE;
				Printf("!!!!VdoMan1 YSrcAddr Error 0x%x\n",rSclParam.u4YSrcBase);
				return 0;
			}
			
			if(rSclParam.u4CSrcBase==0x29fc000)
			{
				rSclParam.u4CSrcBase=_rImgBufCfg.u4InputAddr1;
			}
			else if(rSclParam.u4CSrcBase==0x2c7c000)
			{
				rSclParam.u4CSrcBase=_rImgBufCfg.u4InputAddr2;
			}
			else 
			{
			    _fgErrHappen=TRUE;
				Printf("!!!!VdoMan1 CSrcAddr Error 0x%x\n",rSclParam.u4CSrcBase);
				return 0;
			}

            if(rSclParam.u4YTgBase==0x5612000)
		    {
		        rSclParam.u4YTgBase=_rImgBufCfg.u4OutPutAddr1;
		    }
		    else if(rSclParam.u4YTgBase==0x5f0e000)
		    {
		        rSclParam.u4YTgBase=_rImgBufCfg.u4OutPutAddr2;
		    }
		    else
		    {
			    _fgErrHappen=TRUE;
		        Printf("!!!!OsdMan1 TagAddr Error 0x%x\n",rSclParam.u4YTgBase);
			    return 0;
		    }
		
		    if(rSclParam.u4CTgBase==0x5612000)
		    {
		        rSclParam.u4CTgBase=_rImgBufCfg.u4OutPutAddr1;
		    }
		    else if(rSclParam.u4CTgBase==0x5f0e000)
		    {
			   rSclParam.u4CTgBase=_rImgBufCfg.u4OutPutAddr2;
		    }
		    else
		    {
		    
		      _fgErrHappen=TRUE;
			  Printf("!!!!OsdMan1 TagAddr Error 0x%x\n",rSclParam.u4CTgBase);
			  return 0;
	        }	
        }	
#endif
/*
    Printf(" u4SrcSwap = %u \n", rSclParam.u4SrcSwap);
    Printf(" IsRsIn = %u \n", rSclParam.u4IsRsIn);
    Printf(" YSrcBase = 0x%08x \n", rSclParam.u4YSrcBase);
    Printf(" YSrcBufLen = %u \n", rSclParam.u4YSrcBufLen);
    Printf(" YSrcHOffset = %u, YSrcVOffset = %u, YSrcW = %u, YSrcH = %u \n", 
        rSclParam.u4YSrcHOffset, rSclParam.u4YSrcVOffset,
        rSclParam.u4YSrcW, rSclParam.u4YSrcH);
    Printf(" CSrcBase = 0x%08x \n", rSclParam.u4CSrcBase);
    Printf(" CSrcHOffset = %u, CSrcVOffset = %u, CSrcW = %u, CSrcH = %u \n", 
        rSclParam.u4CSrcHOffset, rSclParam.u4CSrcVOffset,
        rSclParam.u4CSrcW, rSclParam.u4CSrcH);

    Printf(" u4OutSwap = %u \n", rSclParam.u4OutSwap);
    Printf(" IsRsOut = %u, OutMode = %u, Vdo2Osd = %u \n", 
        rSclParam.u4IsRsOut, rSclParam.u4OutMode, rSclParam.u4IsVdo2Osd);
    Printf(" YTgBase = 0x%08x \n", rSclParam.u4YTgBase);
    Printf(" YTgCM = %u, YTgBufLen = %u \n", 
        rSclParam.u4YTgCM, rSclParam.u4YTgBufLen);
    Printf(" YTgHOffset = %u, YTgVOffset = %u, YTgW = %u, YTgH = %u \n", 
        rSclParam.u4YTgHOffset, rSclParam.u4YTgVOffset, 
        rSclParam.u4YTgW, rSclParam.u4YTgH);
    Printf(" CTgBase = 0x%08x \n", rSclParam.u4CTgBase);
    Printf(" CTgW = %u, CTgH = %u \n", 
        rSclParam.u4CTgW, rSclParam.u4CTgH);
*/

#ifdef IMGRZ_HW_A_BLENDING
	if(rSclParam.u4ABlend!=0x80)	//will do alpha blending operation
	{
		x_memset((void *)(rSclParam.u4PreLoadYAddr),0,rSclParam.u4YTgBufLen*(rSclParam.u4YTgH+rSclParam.u4YTgHOffset+16)+1024);
		x_memset((void *)(rSclParam.u4PreLoadCAddr),0x80,rSclParam.u4YTgBufLen*(rSclParam.u4YTgH+rSclParam.u4YTgHOffset+16)+1024);

	    #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)
	    HalFlushInvalidateDCacheMultipleLine((UINT32)(rSclParam.u4PreLoadYAddr), (UINT32)(rSclParam.u4YTgBufLen*(rSclParam.u4YTgH+rSclParam.u4YTgHOffset+16)+1024));
	    HalFlushInvalidateDCacheMultipleLine((UINT32)(rSclParam.u4PreLoadCAddr), (UINT32)((rSclParam.u4CTgH+u4VCOffset+16)*(rSclParam.u4CTgW+u4HCOffset+16)*2+1024));
	    #else
	    HalFlushInvalidateDCache();
	    #endif
	}
#endif

   x_memset((void *)(rSclParam.u4YTgBase),0,rSclParam.u4YTgBufLen*(rSclParam.u4YTgH+rSclParam.u4YTgHOffset+16)+1024);
   if(!rSclParam.u4IsVdo2Osd)
   {     
	 u4HCOffset= rSclParam.u4OutMode==2 ? rSclParam.u4YTgHOffset : rSclParam.u4YTgHOffset>>1;
	 u4VCOffset= rSclParam.u4OutMode==0 ? rSclParam.u4YTgVOffset>>1 : rSclParam.u4YTgVOffset;
	 x_memset((void *)(rSclParam.u4CTgBase),0,(rSclParam.u4CTgH+u4VCOffset+16)*(rSclParam.u4CTgW+u4HCOffset+16)*2+1024);
   }
   
    #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)
    HalFlushInvalidateDCacheMultipleLine((UINT32)(rSclParam.u4YTgBase), (UINT32)(rSclParam.u4YTgBufLen*(rSclParam.u4YTgH+rSclParam.u4YTgHOffset+16)+1024));
    HalFlushInvalidateDCacheMultipleLine((UINT32)(rSclParam.u4CTgBase), (UINT32)((rSclParam.u4CTgH+u4VCOffset+16)*(rSclParam.u4CTgW+u4HCOffset+16)*2+1024));
    #else
    HalFlushInvalidateDCache();
    #endif

   

    _CLI_CMD("imgrz.i");
    // HW
    _CLI_CMD("imgrz.reset");
    _CLI_CMD("imgrzemu.osdmode");

    // Action
    if(_ImgrzVerParam.i4GetRzTimeProfile)
        HAL_GetTime(&rRZStart);
    _IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
    _IMGRZ_Scale(&rSclParam);
    _CLI_CMD("imgrz.flush");
    if(_ImgrzVerParam.i4GetRzTimeProfile)
    {
		HAL_GetTime(&rRZEnd);
	    HAL_GetDeltaTime(&rRZdif, &rRZStart, &rRZEnd);
	    Printf("RZ time %u.%06u\n", rRZdif.u4Seconds, rRZdif.u4Micros);
    }

    Printf("VDO Manual Test 01 (single) ... Done !! \n");
    _IMGRZ_CheckSum(&rSclParam, 1);
    
    return 0;
}

//-------------------------------------------------------------------------
/** _RzEmuJpgCmdManualTestS1
 *  JPG Manual Test Src 01 (single)
 *  manual input
 */
//-------------------------------------------------------------------------
static INT32 _RzEmuJpgCmdManTestS1(INT32 i4Argc, const CHAR ** szArgv)
{
    Printf("JPG Manual Test Src 01 (single) \n");
    
    if (i4Argc < 21)
    {
        Printf("Arg: IsRsIn, YSrcBase1, YSrcBase2, YSrcBufLen, \n");
        Printf("     YSrcHOffset, YSrcVOffset, YSrcW, YSrcH, \n");
        Printf("     CbSrcBase1, CbSrcBase2, CbSrcHOffset, CbSrcVOffset, CbSrcW, CbSrcH, \n");
        Printf("     CrSrcBase1, CrSrcBase2, CrSrcHOffset, CrSrcVOffset, CrSrcW, CrSrcH \n");
        return -1;
    }

	x_memset((void *)&_rJpgSclParam, 0, sizeof(RZ_JPG_SCL_PARAM_SET_T));
	
    // set scaling parameters
    _rJpgSclParam.u4IsRsIn       = (UINT32) StrToInt(szArgv[1]);
    _rJpgSclParam.u4YSrcBase1    = (UINT32) StrToInt(szArgv[2]);
    _rJpgSclParam.u4YSrcBase2    = (UINT32) StrToInt(szArgv[3]);
    _rJpgSclParam.u4YSrcBufLen   = (UINT32) StrToInt(szArgv[4]);
    _rJpgSclParam.u4YSrcHOffset  = (UINT32) StrToInt(szArgv[5]);
    _rJpgSclParam.u4YSrcVOffset  = (UINT32) StrToInt(szArgv[6]);
    _rJpgSclParam.u4YSrcW        = (UINT32) StrToInt(szArgv[7]);
    _rJpgSclParam.u4YSrcH        = (UINT32) StrToInt(szArgv[8]);
    _rJpgSclParam.u4CbSrcBase1   = (UINT32) StrToInt(szArgv[9]);
    _rJpgSclParam.u4CbSrcBase2   = (UINT32) StrToInt(szArgv[10]);
    _rJpgSclParam.u4CbSrcHOffset = (UINT32) StrToInt(szArgv[11]);
    _rJpgSclParam.u4CbSrcVOffset = (UINT32) StrToInt(szArgv[12]);
    _rJpgSclParam.u4CbSrcW       = (UINT32) StrToInt(szArgv[13]);
    _rJpgSclParam.u4CbSrcH       = (UINT32) StrToInt(szArgv[14]);
    _rJpgSclParam.u4CrSrcBase1   = (UINT32) StrToInt(szArgv[15]);
    _rJpgSclParam.u4CrSrcBase2   = (UINT32) StrToInt(szArgv[16]);
    _rJpgSclParam.u4CrSrcHOffset = (UINT32) StrToInt(szArgv[17]);
    _rJpgSclParam.u4CrSrcVOffset = (UINT32) StrToInt(szArgv[18]);
    _rJpgSclParam.u4CrSrcW       = (UINT32) StrToInt(szArgv[19]);
    _rJpgSclParam.u4CrSrcH       = (UINT32) StrToInt(szArgv[20]);

    Printf(" IsRsIn = %u \n", _rJpgSclParam.u4IsRsIn);
    Printf(" YSrcBase1 = 0x%08x \n", _rJpgSclParam.u4YSrcBase1);
    Printf(" YSrcBase2 = 0x%08x \n", _rJpgSclParam.u4YSrcBase2);
    Printf(" YSrcBufLen = %u \n", _rJpgSclParam.u4YSrcBufLen);
    Printf(" YSrcHOffset = %u, YSrcVOffset = %u, YSrcW = %u, YSrcH = %u \n", 
        _rJpgSclParam.u4YSrcHOffset, _rJpgSclParam.u4YSrcVOffset,
        _rJpgSclParam.u4YSrcW, _rJpgSclParam.u4YSrcH);
    Printf(" CbSrcBase1 = 0x%08x \n", _rJpgSclParam.u4CbSrcBase1);
    Printf(" CbSrcBase2 = 0x%08x \n", _rJpgSclParam.u4CbSrcBase2);
    Printf(" CbSrcHOffset = %u, CbSrcVOffset = %u, CbSrcW = %u, CbSrcH = %u \n", 
        _rJpgSclParam.u4CbSrcHOffset, _rJpgSclParam.u4CbSrcVOffset,
        _rJpgSclParam.u4CbSrcW, _rJpgSclParam.u4CbSrcH);
    Printf(" CrSrcBase1 = 0x%08x \n", _rJpgSclParam.u4CrSrcBase1);
    Printf(" CrSrcBase2 = 0x%08x \n", _rJpgSclParam.u4CrSrcBase2);
    Printf(" CrSrcHOffset = %u, CrSrcVOffset = %u, CrSrcW = %u, CrSrcH = %u \n", 
        _rJpgSclParam.u4CrSrcHOffset, _rJpgSclParam.u4CrSrcVOffset,
        _rJpgSclParam.u4CrSrcW, _rJpgSclParam.u4CrSrcH);

    Printf("JPG Manual Test Src 01 (single) ... Done !! \n");
    return 0;
}

//-------------------------------------------------------------------------
/** _RzEmuJpgCmdManualTestD1
 *  JPG Manual Test Dst 01 (single)
 *  manual input
 */
//-------------------------------------------------------------------------
static INT32 _RzEmuJpgCmdManTestD1(INT32 i4Argc, const CHAR ** szArgv)
{
    Printf("JPG Manual Test Dst 01 (single) \n");
    
    if (i4Argc < 20)
    {
        Printf("Arg: IsRsOut, OutMode, IsVdo2Osd, \n");
        Printf("     YTgBase, YTgCM, TgBufLen, \n");
        Printf("     YTgHOffset, YTgVOffset, YTgW, YTgH, \n");
        Printf("     CTgBase, CTgW, CTgH, \n");
        Printf("     IsFstB1, IsLstB1, JpgVfacY, JpgVfacCb, JpgVfacCr, \n");
        Printf("     IsJpgRzOn \n");
        return -1;
    }

    // set scaling parameters
    _rJpgSclParam.u4IsRsOut     = (UINT32) StrToInt(szArgv[1]);
    _rJpgSclParam.u4OutMode     = (UINT32) StrToInt(szArgv[2]);
    _rJpgSclParam.u4IsVdo2Osd   = (UINT32) StrToInt(szArgv[3]);
    _rJpgSclParam.u4YTgBase     = (UINT32) StrToInt(szArgv[4]);
    _rJpgSclParam.u4YTgCM       = (UINT32) StrToInt(szArgv[5]);
    _rJpgSclParam.u4YTgBufLen   = (UINT32) StrToInt(szArgv[6]);
    _rJpgSclParam.u4YTgHOffset  = (UINT32) StrToInt(szArgv[7]);
    _rJpgSclParam.u4YTgVOffset  = (UINT32) StrToInt(szArgv[8]);
    _rJpgSclParam.u4YTgW        = (UINT32) StrToInt(szArgv[9]);
    _rJpgSclParam.u4YTgH        = (UINT32) StrToInt(szArgv[10]);
    _rJpgSclParam.u4CTgBase     = (UINT32) StrToInt(szArgv[11]);
    _rJpgSclParam.u4CTgW        = (UINT32) StrToInt(szArgv[12]);
    _rJpgSclParam.u4CTgH        = (UINT32) StrToInt(szArgv[13]);
    _rJpgSclParam.u4IsFstBl     = (UINT32) StrToInt(szArgv[14]);
    _rJpgSclParam.u4IsLstBl     = (UINT32) StrToInt(szArgv[15]);
    _rJpgSclParam.u4JpgVfacY    = (UINT32) StrToInt(szArgv[16]);
    _rJpgSclParam.u4JpgVfacCb   = (UINT32) StrToInt(szArgv[17]);
    _rJpgSclParam.u4JpgVfacCr   = (UINT32) StrToInt(szArgv[18]);
    _rJpgSclParam.u4IsJpgRzOn   = (UINT32) StrToInt(szArgv[19]);

    Printf(" IsRsOut = %u, OutMode = %u, Vdo2Osd = %u \n", 
        _rJpgSclParam.u4IsRsOut, _rJpgSclParam.u4OutMode, _rJpgSclParam.u4IsVdo2Osd);
    Printf(" YTgBase = 0x%08x \n", _rJpgSclParam.u4YTgBase);
    Printf(" YTgCM = %u, YTgBufLen = %u \n", 
        _rJpgSclParam.u4YTgCM, _rJpgSclParam.u4YTgBufLen);
    Printf(" YTgHOffset = %u, YTgVOffset = %u, YTgW = %u, YTgH = %u \n", 
        _rJpgSclParam.u4YTgHOffset, _rJpgSclParam.u4YTgVOffset, 
        _rJpgSclParam.u4YTgW, _rJpgSclParam.u4YTgH);
    Printf(" CTgBase = 0x%08x \n", _rJpgSclParam.u4CTgBase);
    Printf(" CTgW = %u, CTgH = %u \n", 
        _rJpgSclParam.u4CTgW, _rJpgSclParam.u4CTgH);

    Printf("IsFstBl= %u, IsLstBl = %u \n", 
        _rJpgSclParam.u4IsFstBl, _rJpgSclParam.u4IsLstBl);
    Printf("JpgVfacY = %u, JpgVfacCb = %u, JpgVfacCr = %u \n", 
        _rJpgSclParam.u4JpgVfacY, _rJpgSclParam.u4JpgVfacCb, _rJpgSclParam.u4JpgVfacCr);
    Printf("IsJpgRzOn= %u \n", _rJpgSclParam.u4IsJpgRzOn);

    // HW
    _CLI_CMD("imgrz.reset");
    _CLI_CMD("imgrzemu.osdmode");

    // Action
    _IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_JPGMD);
    _IMGRZ_Scale(&_rJpgSclParam);
    _CLI_CMD("imgrz.flush");

    Printf("JPG Manual Test Dst 01 (single) ... Done !! \n");
    return 0;
}

//-------------------------------------------------------------------------
/** _RzEmuOsdCmdManualTest1
 *  OSD Manual Test 01 (single)
 *  manual input for direct color mode
 */
//-------------------------------------------------------------------------
static INT32 _RzEmuOsdCmdManTest1(INT32 i4Argc, const CHAR ** szArgv)
{
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
   // Printf("OSD Manual Test 01 (single for DirCM) \n");
   
   #ifdef IMGRZ_DRAM_ACCESS_BYTE_MEASUREMENT
    _JPGDramAccessBtye_start();
    #endif     

    if (i4Argc < 16)
    {
        Printf("Arg: SrcBase, IsSrcDirCM, SrcCM, SrcBufLen, \n");
        Printf("     SrcHOffset, SrcVOffset, SrcW, SrcH, \n");
        Printf("     TgBase, TgCM, TgBufLen, \n");
        Printf("     TgHOffset, TgVOffset, TgW, TgH, \n");
        return -1;
    }

	x_memset((void *)&rSclParam, 0, sizeof(RZ_OSD_SCL_PARAM_SET_T));
	
    // set scaling parameters
    rSclParam.u4SrcBase     = (UINT32) StrToInt(szArgv[1]);
    rSclParam.u4IsSrcDirCM  = (UINT32) StrToInt(szArgv[2]);
    rSclParam.u4SrcCM       = (UINT32) StrToInt(szArgv[3]);
    rSclParam.u4SrcBufLen   = (UINT32) StrToInt(szArgv[4]);
    rSclParam.u4SrcHOffset  = (UINT32) StrToInt(szArgv[5]);
    rSclParam.u4SrcVOffset  = (UINT32) StrToInt(szArgv[6]);
    rSclParam.u4SrcW        = (UINT32) StrToInt(szArgv[7]);
    rSclParam.u4SrcH        = (UINT32) StrToInt(szArgv[8]);

    rSclParam.u4TgBase      = (UINT32) StrToInt(szArgv[9]);
    rSclParam.u4TgCM        = (UINT32) StrToInt(szArgv[10]);
    rSclParam.u4TgBufLen    = (UINT32) StrToInt(szArgv[11]);
    rSclParam.u4TgHOffset   = (UINT32) StrToInt(szArgv[12]);
    rSclParam.u4TgVOffset   = (UINT32) StrToInt(szArgv[13]);
    rSclParam.u4TgW         = (UINT32) StrToInt(szArgv[14]);
    rSclParam.u4TgH         = (UINT32) StrToInt(szArgv[15]);

	rSclParam.u4Csc_enable      = (UINT32) StrToInt(szArgv[16]);
	rSclParam.csc_src_format      = (E_RZ_CSC_FORMAT_T) StrToInt(szArgv[17]);
	rSclParam.csc_des_format      = (E_RZ_CSC_FORMAT_T) StrToInt(szArgv[18]);	
	
	rSclParam.fgOSDTurboMode      = (BOOL) StrToInt(szArgv[19]);		
	rSclParam.fgAlphaComposition      = (BOOL) StrToInt(szArgv[20]);
	rSclParam.u4AlphaComposition_Mode      = (UINT32) StrToInt(szArgv[21]);

	#ifdef IMGRZ_HW_A_COMPOSITION
	if((rSclParam.fgOSDTurboMode==TRUE)&&(rSclParam.fgAlphaComposition==TRUE))
	{
		rSclParam.u4AlphaComposition_PreloadAddr = IMGRZ_HW_A_COMPOSITION_PRELOAD_ADDR;
	}
    #endif
    
#ifdef IMGRZ_AUTO_TEST
	if(_ImgrzVerParam.i4RZNewBufCfg)
	{
		if(rSclParam.u4SrcBase==0x29fc000)
		{
			rSclParam.u4SrcBase=_rImgBufCfg.u4InputAddr1;
		}
		else if(rSclParam.u4SrcBase==0x2c7c000)
		{
			rSclParam.u4SrcBase=_rImgBufCfg.u4InputAddr2;
		}
		else 
		{  
		    _fgErrHappen=TRUE;
			Printf("!!!!OsdMan1 SrcAddr Error 0x%x\n",rSclParam.u4SrcBase);
			return 0;
		}
		
		if(rSclParam.u4TgBase==0x5612000)
		{
			rSclParam.u4TgBase=_rImgBufCfg.u4OutPutAddr1;
		}
		else if(rSclParam.u4TgBase==0x5f0e000)
		{
			rSclParam.u4TgBase=_rImgBufCfg.u4OutPutAddr2;
		}
		else
		{
		    _fgErrHappen=TRUE;
			Printf("!!!!OsdMan1 TagAddr Error 0x%x\n",rSclParam.u4TgBase);
			return 0;
		}
	}
#endif
	
/*
    Printf(" SrcBase = 0x%08x \n", rSclParam.u4SrcBase);
    Printf(" IsSrcDirCM = %u, SrcCM = %u, SrcBufLen = %u \n", 
        rSclParam.u4IsSrcDirCM, rSclParam.u4SrcCM, rSclParam.u4SrcBufLen);
    Printf(" SrcHOffset = %u, SrcVOffset = %u, SrcW = %u, SrcH = %u \n", 
        rSclParam.u4SrcHOffset, rSclParam.u4SrcVOffset,
        rSclParam.u4SrcW, rSclParam.u4SrcH);
    Printf(" TgBase = 0x%08x \n", rSclParam.u4TgBase);
    Printf(" TgCM = %u, TgBufLen = %u \n", 
        rSclParam.u4TgCM, rSclParam.u4TgBufLen);
    Printf(" TgHOffset = %u, TgVOffset = %u, TgW = %u, TgH = %u \n", 
        rSclParam.u4TgHOffset, rSclParam.u4TgVOffset, 
        rSclParam.u4TgW, rSclParam.u4TgH);
 */
    //x_memset((void *)(rSclParam.u4TgBase),0,rSclParam.u4TgBufLen*(rSclParam.u4TgH+rSclParam.u4TgHOffset+16)+1024);

    #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)
    HalFlushInvalidateDCacheMultipleLine((UINT32)(rSclParam.u4TgBase), (UINT32)(rSclParam.u4TgBufLen*(rSclParam.u4TgH+rSclParam.u4TgHOffset+16)+1024));
    #else
    HalFlushInvalidateDCache();
    #endif
    
    _CLI_CMD("imgrz.i");
    // HW
    _CLI_CMD("imgrz.reset");
    _CLI_CMD("imgrzemu.osdmode");

    // Action
    if(_ImgrzVerParam.i4GetRzTimeProfile)
    {
        //u4DelayClock = (BSP_GetDomainClock(CAL_SRC_DMPLL) >> 1) ; //dram cycle /sec
        //u4DramCycle = SRM_DramMonitor(0, SRM_DRV_CPU, u4DelayClock, 0);
        HAL_GetTime(&rRZStart);
    }
    x_memset((void*)rSclParam.u4TgBase, 0, rSclParam.u4TgBufLen*rSclParam.u4TgH);
    _IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_OSDMD);
    _IMGRZ_Scale(&rSclParam);
    _CLI_CMD("imgrz.flush");
	
    if(_ImgrzVerParam.i4GetRzTimeProfile)
    {
        HAL_GetTime(&rRZEnd);
        HAL_GetDeltaTime(&rRZdif, &rRZStart, &rRZEnd);
        Printf("RZ time %u.%06u\n", rRZdif.u4Seconds, rRZdif.u4Micros);  

        #ifdef IMGRZ_DRAM_ACCESS_BYTE_MEASUREMENT
        Printf("Total BW bytes %d\n", _JPGDramAccessBtye_end(1));  
        #endif
    }

    _IMGRZ_CheckSum(&rSclParam, 0);

    Printf("OSD Manual Test 01 (single for DirCM) ... Done !! \n");
    Printf("------------------------------------------\n");
    return 0;
}

//-------------------------------------------------------------------------
/** _RzEmuOsdCmdManualTest2
 *  OSD Manual Test 02 (single)
 *  manual input for index color mode
 */
//-------------------------------------------------------------------------
static INT32 _RzEmuOsdCmdManTest2(INT32 i4Argc, const CHAR ** szArgv)
{
#if 0
    UINT32 i, u4PaleSize, u4A, u4R, u4G, u4B, u4Val, u4Idx, u4ImgSize;
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    Printf("OSD Manual Test 02 (single for IdxCM) \n");
   
    if (i4Argc < 3)
    {
        Printf("Arg: TgCM, TgBufLen \n");
        return -1;
    }

    rSclParam.u4SrcBase  = NULL;
    rSclParam.pu4CPTData = NULL;

    // set scaling parameters
    u4ImgSize = sizeof(_u1gfx_pt_img_bits);
    
    rSclParam.u4SrcBase = (UINT32)BSP_AllocAlignedDmaMemory(u4ImgSize, 16);
    if (rSclParam.u4SrcBase == NULL)
    {
        Printf("rSclParam.u4SrcBase == NULL \n");
        return -1;
    }
    x_memcpy((void *)rSclParam.u4SrcBase, 
             (void *)_u1gfx_pt_img_bits, u4ImgSize);
    HalFlushDCache();
    
    rSclParam.u4IsSrcDirCM  = 0;
    rSclParam.u4SrcCM       = (UINT32)E_RZ_OSD_IDX_CM_8BPP;
    rSclParam.u4SrcBufLen   = 1024;
    rSclParam.u4SrcHOffset  = 0;
    rSclParam.u4SrcVOffset  = 0;
    rSclParam.u4SrcW        = 1024;
    rSclParam.u4SrcH        = 768;

    rSclParam.u4TgCM        = (UINT32) StrToInt(szArgv[1]);
    rSclParam.u4TgBufLen    = (UINT32) StrToInt(szArgv[2]);
    rSclParam.u4TgBase      = (UINT32) StrToInt(szArgv[3]);//RZ_EMU_OSD_SCREEN_BUF;
    rSclParam.u4TgHOffset   = 0;
    rSclParam.u4TgVOffset   = 0;
    rSclParam.u4TgW         = 1024;
    rSclParam.u4TgH         = 768;

    rSclParam.pu4CPTData = (UINT32 *)BSP_AllocAlignedDmaMemory((256 * 4), 16);
    if (rSclParam.pu4CPTData == NULL)
    {
        Printf("rSclParam.pu4CPTData == NULL \n");
        return -1;
    }

    u4PaleSize = sizeof(_au1PaleTable_256Entry_32BPP);
    Printf("u4PaleSize = %u \n\n", u4PaleSize);
    u4Idx = 0;
    for (i = 0; i < u4PaleSize; i += 4)
    {
        u4A = _au1PaleTable_256Entry_32BPP[i];
        u4R = _au1PaleTable_256Entry_32BPP[i+1];
        u4G = _au1PaleTable_256Entry_32BPP[i+2];
        u4B = _au1PaleTable_256Entry_32BPP[i+3];

        u4Val = ((u4A << 24) | (u4R << 16) | (u4G << 8) | (u4B));

        rSclParam.pu4CPTData[u4Idx] = u4Val;
        u4Idx++;
    }
    rSclParam.u4CPTDataCount = u4Idx;
    
    Printf(" SrcBase = 0x%08x \n", rSclParam.u4SrcBase);
    Printf(" IsSrcDirCM = %u, SrcCM = %u, SrcBufLen = %u \n", 
        rSclParam.u4IsSrcDirCM, rSclParam.u4SrcCM, rSclParam.u4SrcBufLen);
    Printf(" SrcHOffset = %u, SrcVOffset = %u, SrcW = %u, SrcH = %u \n", 
        rSclParam.u4SrcHOffset, rSclParam.u4SrcVOffset,
        rSclParam.u4SrcW, rSclParam.u4SrcH);
    Printf(" TgBase = 0x%08x \n", rSclParam.u4TgBase);
    Printf(" TgCM = %u, TgBufLen = %u \n", 
        rSclParam.u4TgCM, rSclParam.u4TgBufLen);
    Printf(" TgHOffset = %u, TgVOffset = %u, TgW = %u, TgH = %u, \n", 
        rSclParam.u4TgHOffset, rSclParam.u4TgVOffset, 
        rSclParam.u4TgW, rSclParam.u4TgH);
    Printf("CPTDataCount = %u, CPTData = 0x%08x \n",
        rSclParam.u4CPTDataCount, rSclParam.pu4CPTData);

    // HW
    _CLI_CMD("imgrz.reset");
    _CLI_CMD("imgrzemu.osdmode");

    // Action
    _IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_OSDMD);
    _IMGRZ_Scale(&rSclParam);
    _CLI_CMD("imgrz.flush");

    Printf("OSD Manual Test 02 (single for IdxCM) ... Done !! \n");
    _IMGRZ_CheckSum(&rSclParam, 0);

    BSP_FreeAlignedDmaMemory(rSclParam.u4SrcBase);
    BSP_FreeAlignedDmaMemory(rSclParam.pu4CPTData);
#endif    
    return 0;
}

//-------------------------------------------------------------------------
/** _RzEmuOsdCmdDmaTest1
 *  OSD DMA Test 01 (basic)
 *  Just show a picture
 */
//-------------------------------------------------------------------------
static INT32 _RzEmuOsdCmdDmaTest1(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size;
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    
    Printf("OSD DMA Test 01 (basic) \n");

	x_memset((void *)&rSclParam, 0, sizeof(RZ_OSD_SCL_PARAM_SET_T));
	
    // set scaling parameters
    rSclParam.u4SrcBase     = RZ_EMU_OSD_SRC_BUF;
    rSclParam.u4IsSrcDirCM  = IMGRZ_TRUE;
    rSclParam.u4SrcCM       = _u4RzCurDirCMIdx;
    rSclParam.u4SrcBufLen   = _u4RzSrcBufLen;
    rSclParam.u4SrcHOffset  = 0;
    rSclParam.u4SrcVOffset  = 0;
    rSclParam.u4SrcW        = _u4RzScreenWidth;
    rSclParam.u4SrcH        = _u4RzScreenHeight;

    rSclParam.u4TgBase      = RZ_EMU_OSD_SCREEN_BUF;
    rSclParam.u4TgCM        = _u4RzCurDirCMIdx;
    rSclParam.u4TgBufLen    = _u4RzSrcBufLen;
    rSclParam.u4TgHOffset   = 0;
    rSclParam.u4TgVOffset   = 0;
    rSclParam.u4TgW         = _u4RzScreenWidth;
    rSclParam.u4TgH         = _u4RzScreenHeight;
    
    // HW
    _CLI_CMD("imgrz.reset");
    _CLI_CMD("imgrzemu.osdmode");

    // Action
    _IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_OSDMD);
    _IMGRZ_Scale(&rSclParam);
    _CLI_CMD("imgrz.flush");


    // SW
    _CLI_CMD("imgrz.reset");
    _CLI_CMD("imgrzemu.osdmode sw");

    // Action
    rSclParam.u4TgBase = RZ_EMU_COMPARE_BUF;    // compare buffer
    _IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_OSDMD);
    _IMGRZ_Scale(&rSclParam);
    _CLI_CMD("imgrz.flush");

    // Compare HW and SW
    u4Size = (_u4RzScreenWidth * _u4RzScreenHeight);
    if (_RzBufCompare(
        (UINT32 *)(RZ_EMU_COMPARE_BUF), 
        (UINT32 *)(RZ_EMU_OSD_SCREEN_BUF), u4Size) == -1)
    {
        Printf("OSD DMA Test 01 (basic) Error\n");
        return -1;
    }

    Printf("OSD DMA Test 01 (basic) ... 1 runs OK !!\n");
    return 0;
}

//-------------------------------------------------------------------------
/** _RzEmuOsdCmdDmaTest2
 *  OSD DMA Test 02 (random)
 */
//-------------------------------------------------------------------------
static INT32 _RzEmuOsdCmdDmaTest2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size;
    INT32 i4Left, i4Right, i4Top, i4Bottom;
    INT32 i4Width, i4Height, i4Count;
    INT32 i4CountNum = 1000;

    Printf("OSD DMA Test 02 (random) \n");
    
    if (i4Argc < 2)
    {
        Printf("arg: loops\n");
        return -1;
    }

    i4CountNum = (INT32) StrToInt(szArgv[1]);

    for (i4Count = 0; i4Count < i4CountNum; i4Count++)
    {
        i4Left    = ((UINT32)rand()) % _u4RzScreenWidth;
        i4Right   = ((UINT32)rand()) % _u4RzScreenWidth;
        i4Top     = ((UINT32)rand()) % _u4RzScreenHeight;
        i4Bottom  = ((UINT32)rand()) % _u4RzScreenHeight;

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
        _CLI_CMD("imgrz.reset");
        _CLI_CMD("imgrzemu.osdmode");

        // Action


        // SW
        _CLI_CMD("imgrz.reset");
        _CLI_CMD("imgrzemu.osdmode sw");
        
        // Action


        // Compare HW and SW
        u4Size = (_u4RzScreenWidth * _u4RzScreenHeight);
        if (_RzBufCompare(
            (UINT32 *)(RZ_EMU_COMPARE_BUF), 
            (UINT32 *)(RZ_EMU_OSD_SCREEN_BUF), u4Size) == -1)
        {
            Printf("OSD DMA Test 02 (random) Error\n");
            return -1;
        }
    }

    Printf("OSD DMA Test 02 (random) ... %u runs OK !!\n", i4Count);
    return 0;
}

//-------------------------------------------------------------------------
/** _RzEmuOsdCmdDmaTest3
 *  OSD DMA Test 03 (random)
 */
//-------------------------------------------------------------------------
static INT32 _RzEmuOsdCmdDmaTest3(INT32 i4Argc, const CHAR ** szArgv)
{
   return 0;
}

//-------------------------------------------------------------------------
/** _RzBufCompare
 *  compare C-model & engine
 */
//-------------------------------------------------------------------------
#if 0
static INT32 _RzBufCompare(UINT32 *pu4Cm, UINT32 *pu4Hw, UINT32 u4Num)
{
    _CLI_CMD("gfx.reset");
    _CLI_CMD("imgrzemu.mode");
    return _RzHwBufCompare((UINT8 *)pu4Cm, (UINT8 *)pu4Hw, u4Num);
}
#else
static INT32 _RzBufCompare(UINT32 *pu4Cm, UINT32 *pu4Hw, UINT32 u4Num)
{
    INT32 i4Error = 0;
    UINT32 u4TotalPixels = u4Num;
    UINT32 u4MaxDiff = 0, u4Temp;
    
    if (u4Num <= 0)
    {
        Printf("Error: u4Num <= 0\n");
        return -1;
    }

    if ((0 == x_strcmp(_szRzCurDirCM, "argb4444")) ||
        (0 == x_strcmp(_szRzCurDirCM, "argb1555")) ||
        (0 == x_strcmp(_szRzCurDirCM, "rgb565")))
    {
        u4Num = u4Num / 2;
    }
    else if (0 == x_strcmp(_szRzCurDirCM, "rgb8"))
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
            u4TotalPixels, _szRzCurDirCM);
        Printf("[CPU] HW/SW Comparison Fail => Error = %d (MaxDiff = %u)\n", 
            i4Error, u4MaxDiff);
        return -1;
    }
    else
    {
        Printf("Color Mode = %s \n", _szRzCurDirCM);
        Printf("[CPU] HW/SW Comparison OK ! \n");
    }
    return 0;
}
#endif
/*
// use gfx hw to do memory compare
static INT32 _RzHwBufCompare(UINT8 *pu1Cm, UINT8 *pu1Hw, UINT32 u4Num)
{
    INT32 i4Ret;
    UINT32 u4TotalPixels = u4Num;
    
    if (u4Num <= 0)
    {
        Printf("Error: u4Num <= 0\n");
        return -1;
    }

    if (0 == x_strcmp(_szRzCurDirCM, "rgb8"))
    {
        u4Num = u4Num;
    }
    else if ((0 == x_strcmp(_szRzCurDirCM, "argb4444")) ||
             (0 == x_strcmp(_szRzCurDirCM, "argb1555")) ||
             (0 == x_strcmp(_szRzCurDirCM, "rgb565")))
    {
        u4Num = u4Num * 2;
    }
    else
    {
        u4Num = u4Num * 4;
    }

    Printf("\n");

    i4Ret = GFX_MemCompare(pu1Cm, pu1Hw, u4Num);

    Printf("Total Pixels = %u, Color Mode = %s \n", u4TotalPixels, _szRzCurDirCM);
            
    if (i4Ret != (INT32)E_RZ_OK)
    {
        Printf("[IMGRZ_HW] HW/SW Comparison Fail ! \n");
        return -1;
    }
    else
    {
        Printf("[IMGRZ_HW] HW/SW Comparison OK ! \n");
    }
    return 0;
}
*/
static void _RzEmuCreateBuffers(void)
{
    UINT32 u4Size = (_u4RzScreenWidth * _u4RzScreenHeight * 4);
    UINT32 i, u4BufNum = 3;
    
     #ifdef JPEG_AUTO_TEST_PERFORMANCE
    _u4RzFbmStartAddr = 0x0623e000;
    _u4RzFbmTotalSize = DRVCUST_OptGet(eFbmMemSize);
     #else
    _u4RzFbmStartAddr = DRVCUST_OptGet(eFbmMemAddr);
    _u4RzFbmTotalSize = DRVCUST_OptGet(eFbmMemSize);
    #endif
    
    Printf("----------------------------------------------------------\n");
    Printf("FBM start addr = 0x%08x\n", _u4RzFbmStartAddr);
    Printf("FBM total size = %u (MB)\n", (_u4RzFbmTotalSize / 1048576));

    for (i = 0; i < u4BufNum; i++)
    {
        _u4RzEmuBuf[i] = _u4RzFbmStartAddr + (i * u4Size);
    }

    Printf("osd screen buf addr = 0x%08x\n", RZ_EMU_OSD_SCREEN_BUF);

    Printf("compare buf addr = 0x%08x\n", RZ_EMU_COMPARE_BUF);

    Printf("src buffer addr 1 = 0x%08x\n", RZ_EMU_OSD_SRC_BUF);


    Printf("----------------------------------------------------------\n");

    Printf("d.load.binary D:/MT5391_Emu/kodak14.8888 0x%08x\n", RZ_EMU_OSD_SRC_BUF);

    Printf("----------------------------------------------------------\n");
}


static void _IMGRZ_CheckSum(void *pvParam, UINT32 u4Type)
{
    RZ_OSD_SCL_PARAM_SET_T *pOsdParam;
    RZ_VDO_SCL_PARAM_SET_T *pVdoParam;
    UINT32 u4SizeFactor, u4Width, u4Height, u4Size;
    UINT32 u4Sum = 0, u4I;
    UINT8 *pu1Ptr;
    
    if (!u4Type)        //OSD
    {
        pOsdParam = (RZ_OSD_SCL_PARAM_SET_T *)pvParam;
        u4Width = pOsdParam->u4TgW+pOsdParam->u4TgHOffset;
        u4Height = pOsdParam->u4TgH+pOsdParam->u4TgVOffset;
        if (pOsdParam->u4TgCM == E_RZ_OSD_DIR_CM_AYCbCr8888)
        {
            u4SizeFactor = 2;
        }
        else
        {
            u4SizeFactor = 1;
        }       
        pu1Ptr = (UINT8 *)pOsdParam->u4TgBase;
    }
    else            //VDO
    {
        pVdoParam = (RZ_VDO_SCL_PARAM_SET_T *)pvParam;
        u4Width = pVdoParam->u4YTgW+pVdoParam->u4YTgHOffset;
        u4Height = pVdoParam->u4YTgH+pVdoParam->u4YTgVOffset;
        if (!pVdoParam->u4YTgCM)                 //VDO output
        {
            u4SizeFactor = 0;
        }
        else if (pVdoParam->u4YTgCM == E_RZ_OSD_DIR_CM_AYCbCr8888)   //OSD 32bpp output
        {
            u4SizeFactor = 2;
        }
        else                                    //OSD 16 bpp output
        {        
            u4SizeFactor = 1;
        }
        pu1Ptr = (UINT8 *)pVdoParam->u4YTgBase;
    }

    u4Size = (u4Width * u4Height) << u4SizeFactor;

    for (u4I = 0; u4I < u4Size; u4I++)
    {
        u4Sum += ((UINT32)(*(pu1Ptr + u4I)));
    }
    Printf("checksum: %x\n", u4Sum);

    if (u4Type && !pVdoParam->u4YTgCM)     //VDO check chroma sum
    {
        u4Sum = 0;
        u4Width = pVdoParam->u4CTgW+(pVdoParam->u4OutMode==2 ? pVdoParam->u4YTgHOffset : pVdoParam->u4YTgHOffset>>1);
        u4Height = pVdoParam->u4CTgH+(pVdoParam->u4OutMode==1 ? pVdoParam->u4YTgVOffset : pVdoParam->u4YTgVOffset>>1);
        pu1Ptr = (UINT8 *)pVdoParam->u4CTgBase;
        u4Size = (u4Width * u4Height) << 1;
        for (u4I = 0; u4I < u4Size; u4I++)
        {
            u4Sum += ((UINT32)(*(pu1Ptr + u4I)));
        }
        Printf("checksum (C): %x\n", u4Sum);
    }
    
}

#endif // #if defined(IMGRZ_ENABLE_SW_MODE)


// for debug use
/*
static void _RzSetMidWriteRegion(void)
{
    _CLI_CMD("mid.i");
    MID_ResetRegion(0);
    MID_SetRegion(0, MID_AGENT_CPU, 0x00000000, 0x0FFFFFFF);
    MID_EnableRegionProtect(0);    

    CLI_Parser("mid.h on");
//    CLI_Parser("mid.rl");
}
*/

CLI_MAIN_COMMAND_ITEM(IMGRZEMU)
{
    "imgrzemu",
    NULL,
    NULL,
    _arRzEmuTbl,
    "Imgrz emulation suit",
    CLI_SUPERVISOR
};

/*lint -restore */


