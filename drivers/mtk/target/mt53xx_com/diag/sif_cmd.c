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
 * $RCSfile: sif_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sif_cmd.c
 *  SIF module CLI test function.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_lint.h"

LINT_SAVE_AND_DISABLE

#include "eeprom_if.h"
#include "sif_if.h"
#include "sif_sw_drvif.h"

#include "sif_nptv_drvif.h"
#include "drvcust_if.h"
#include "uart_if.h"

#ifdef __MW_CLI_DEF__

#include "x_mid_cli.h"
#else

#include "x_drv_cli.h"
#endif

#define DEFINE_IS_LOG      CLI_IsLog
#include "x_debug.h"
#include "x_util.h"
#include "x_hal_5381.h"

#include "x_os.h"
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_pinmux.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_serial.h"
#include "x_timer.h"
#include "drv_hdmi.h"
#include "c_model.h"

#include "util.h"

#if defined(CC_MT5387P1V1)
#include "x_gpio.h"
#endif


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
#define MT5391_IC_VERIFY_STAGE 1
//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define SIF_BASE           BIM_BASE
#define SIF_REG_LENGTH      0

#define SIF_HDCP_SIZE  320

#if defined(CC_MT5387P1V1)

#define HDMI_DDC_WP 402

// HDMI EDID 1 EERPOM GPIO pin configuration.
#ifndef SIF_EDID1_SDA
#define SIF_EDID1_SDA  (GPIO(206))
#endif
#ifndef SIF_EDID1_SCL
#define SIF_EDID1_SCL  (GPIO(207))
#endif

// HDMI EDID 2 EERPOM GPIO pin configuration.
#ifndef SIF_EDID2_SDA
#define SIF_EDID2_SDA  (GPIO(206))
#endif
#ifndef SIF_EDID2_SCL
#define SIF_EDID2_SCL  (GPIO(207))
#endif

// HDMI EDID 3 EERPOM GPIO pin configuration.
#ifndef SIF_EDID3_SDA
#define SIF_EDID3_SDA  (GPIO(206))
#endif
#ifndef SIF_EDID3_SCL
#define SIF_EDID3_SCL  (GPIO(207))
#endif

#else

#ifndef SIF_EDID1_SDA
#define SIF_EDID1_SDA  (GPIO(214))
#endif
#ifndef SIF_EDID1_SCL
#define SIF_EDID1_SCL  (GPIO(219))
#endif

// HDMI EDID 2 EERPOM GPIO pin configuration.
#ifndef SIF_EDID2_SDA
#define SIF_EDID2_SDA  (GPIO(210))
#endif
#ifndef SIF_EDID2_SCL
#define SIF_EDID2_SCL  (GPIO(211))
#endif

// HDMI EDID 3 EERPOM GPIO pin configuration.
#ifndef SIF_EDID3_SDA
#define SIF_EDID3_SDA  (GPIO(206))
#endif
#ifndef SIF_EDID3_SCL
#define SIF_EDID3_SCL  (GPIO(207))
#endif

#endif

#define SIF_EDID_SIZE  256

#define SIF_EEP_DEVICE_ADDR  0xA0

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
#ifdef CC_Support_Sil9285

extern UINT8 vSiI9287EDIDWrite(UINT8*,UINT8*) ;

extern UINT8  vSiI9287EDIDread(UINT8, UINT8*);

extern UINT8  vSiI9287HDCPKSVread(UINT8*); 
extern UINT8 bHDMISwitch5VStatus(UINT8);
extern UINT8 bHDMISwitchCLKStatus(UINT8);
extern UINT8 bHDMISwitchHDCPStatus(UINT8);
extern UINT8 bHDMISwitchHPDStatus(UINT8);


#endif
//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
static INT32 _SifSWWriteEeprom(UINT32 u4SDA, UINT32 u4SCL, UINT8 *pu1Data);
static INT32 _SifSWReadCmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifInitCmd(INT32 i4Argc, const CHAR * *szArgv);

// General sif read/write function.
static INT32 _SifReadCmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifWriteCmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifWriteByteCmd(INT32 i4Argc, const CHAR * *szArgv);

// HDMI HDCP read/write function.
static INT32 _SifHDCPWriteSramCmd(INT32 i4Argc, const CHAR * *szArgv);
//static INT32 _SifHDCPWriteEepromCmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifHDCPReadCmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifHDCPWriteByteCmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifHDCPReadByteCmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifHDCPWrAllByteCmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifHDCPRdAllByteCmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifHDCPWrRdAllByteCmd(INT32 i4Argc, const CHAR * *szArgv);

// HDMI EDID read/write function.
static INT32 _SifEDIDWriteCmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifEDIDReadCmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifEDID1WriteCmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifEDID2WriteCmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifEDID3WriteCmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifEDID1ReadCmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifEDID2ReadCmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifEDID3ReadCmd(INT32 i4Argc, const CHAR * *szArgv);
//static INT32 _SifEDIDWriteByteCmd(INT32 i4Argc, const CHAR * *szArgv);
//static INT32 _SifEDIDReadByteCmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifEDIDWrAllByteCmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifEDIDRdAllByteCmd(INT32 i4Argc, const CHAR * *szArgv);

static INT32 _SifDiagCmd(INT32 i4Argc, const CHAR * *szArgv);

static INT32 _SifTunerReadNoSubCmd(INT32 i4Argc, const CHAR  * *szArgv);
static INT32 _SifTunerWriteNoSubCmd(INT32 i4Argc, const CHAR  * *szArgv);
static INT32 _SifReadMultiCmd(INT32 i4Argc, const CHAR  * *szArgv);
static INT32 _SifWriteMultiCmd(INT32 i4Argc, const CHAR  * *szArgv);
static INT32 _SifRead_X_Cmd(INT32 i4Argc, const CHAR  * *szArgv);
static INT32 _SifWrite_X_Cmd(INT32 i4Argc, const CHAR  * *szArgv);
static INT32 _EDIDReadByteCmd(INT32 i4Argc, const CHAR * *szArgv);

static INT32 _SifSWRead0Cmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifSWWrite0Cmd(INT32 i4Argc, const CHAR * *szArgv);
static INT32 _SifSWWriteCmd(INT32 i4Argc, const CHAR * *szArgv);
//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static UINT8 _au1EEPBuf[SIF_HDCP_SIZE+16] = { 0xff };

static const UINT8 _au1HDCP[SIF_HDCP_SIZE] = 
{
0x00 ,0x14 ,0x7c ,0xe6 ,0x2c ,0x37 ,0xf3 ,0xe8 ,0xa8 ,0x4d ,0x27 ,0x66 ,0xa8 ,0xd0 ,0x2f ,0x13,
0x55 ,0x79 ,0x97 ,0xe7 ,0x87 ,0x37 ,0x18 ,0x04 ,0xd1 ,0x5f ,0x3b ,0x12 ,0x9f ,0x32 ,0xa4 ,0x6f,
0x58 ,0x1c ,0x8a ,0xbc ,0x8c ,0x7f ,0xe3 ,0xcb ,0x81 ,0x9e ,0xb4 ,0x45 ,0x7c ,0x66 ,0x6a ,0xcc,
0xdd ,0x5c ,0xc8 ,0x17 ,0xa3 ,0x90 ,0x43 ,0x91 ,0x8b ,0x01 ,0xf0 ,0x1d ,0xff ,0x8a ,0x1b ,0xde,
0x94 ,0xd6 ,0xb6 ,0xf6 ,0x6d ,0x15 ,0x7b ,0x42 ,0x13 ,0xa2 ,0x1b ,0x04 ,0xb5 ,0xdd ,0x11 ,0xcc,
0x42 ,0x28 ,0x66 ,0x65 ,0xf5 ,0x77 ,0xf1 ,0x65 ,0xc3 ,0x8c ,0x9b ,0x2c ,0xad ,0xb4 ,0xe9 ,0x7c,
0xd1 ,0xbc ,0xd6 ,0x4a ,0x5d ,0xf7 ,0x45 ,0x3e ,0x2a ,0x22 ,0xec ,0xa8 ,0xdf ,0x68 ,0x54 ,0x57,
0x5a ,0x10 ,0xc8 ,0x38 ,0x9f ,0x94 ,0xa0 ,0xa7 ,0xa0 ,0x71 ,0xa2 ,0x67 ,0x8e ,0x23 ,0xbd ,0x8d,
0x63 ,0x89 ,0x0d ,0x01 ,0x91 ,0x97 ,0x4c ,0xba ,0x5c ,0x4d ,0x94 ,0x73 ,0x36 ,0x68 ,0x12 ,0x6c,
0xe8 ,0xfa ,0xb1 ,0x51 ,0xc1 ,0x93 ,0xc6 ,0xce ,0x72 ,0x90 ,0xc1 ,0x6b ,0x4d ,0xf6 ,0x63 ,0x02,
0xd3 ,0xa6 ,0x9b ,0x80 ,0x35 ,0xb6 ,0xa9 ,0xff ,0x8e ,0xfd ,0xd9 ,0x6f ,0x24 ,0xa6 ,0xdb ,0x4c,
0xd2 ,0x0c ,0x0f ,0xcf ,0xcd ,0x1a ,0x19 ,0xe4 ,0x62 ,0x9c ,0x6d ,0x17 ,0x6b ,0x57 ,0x39 ,0xcb,
0x6a ,0x0d ,0x80 ,0x75 ,0xfa ,0xf3 ,0x69 ,0x7d ,0x9f ,0x79 ,0xe3 ,0xc0 ,0x8b ,0x5a ,0xd2 ,0xa8,
0xc9 ,0xd9 ,0x90 ,0x93 ,0xaa ,0xe6 ,0x1a ,0x1e ,0x17 ,0x93 ,0x03 ,0x2d ,0x43 ,0xc0 ,0xaf ,0x33,
0x94 ,0x66 ,0xa9 ,0x18 ,0x55 ,0xcc ,0x22 ,0xf5 ,0x23 ,0xc8 ,0xc5 ,0x37 ,0xf1 ,0x81 ,0xd2 ,0x96,
0xaf ,0x0a ,0x5a ,0xe5 ,0x8a ,0x13 ,0xef ,0x63 ,0x19 ,0x4c ,0xc6 ,0x3f ,0x6c ,0x9a ,0x7a ,0xb2,
0xa6 ,0xd4 ,0x31 ,0x5a ,0x30 ,0x01 ,0x21 ,0xcd ,0xa2 ,0x86 ,0x74 ,0x04 ,0x46 ,0x3e ,0x38 ,0x08,
0xe7 ,0x57 ,0xa7 ,0xb4 ,0x41 ,0x73 ,0x02 ,0x78 ,0x93 ,0x34 ,0x93 ,0xce ,0x93 ,0x5a ,0x46 ,0xd3,
0x0a ,0x14 ,0xf7 ,0x61 ,0x03 ,0xb7 ,0x8f ,0x22 ,0xd2 ,0xf1 ,0x3a ,0x98 ,0xba ,0x28 ,0x62 ,0x53,
0xca ,0x4c ,0x1d ,0x19 ,0xce ,0x49 ,0x87 ,0xf7 ,0xb5 ,0x26 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
};
/*
static UINT8 _au1EDID[SIF_EDID_SIZE] = 
{
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x36, 0x8B, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00
, 0x01, 0x0F, 0x01, 0x03, 0x80, 0x3C, 0x22, 0x78, 0x0A, 0x0D, 0xC9, 0xA0, 0x57, 0x47, 0x98, 0x27
, 0x12, 0x48, 0x4C, 0x20, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01
, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20, 0x6E, 0x28
, 0x55, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16, 0x20
, 0x58, 0x2C, 0x25, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x9E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x4D
, 0x54, 0x4B, 0x20, 0x4C, 0x43, 0x44, 0x54, 0x56, 0x0A, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD
, 0x00, 0x31, 0x4C, 0x0F, 0x50, 0x0E, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0xE7
, 0x02, 0x03, 0x1C, 0x74, 0x49, 0x84, 0x05, 0x13, 0x14, 0x01, 0x02, 0x11, 0x06, 0x15, 0x23, 0x09
, 0x57, 0x03, 0x83, 0x01, 0x00, 0x00, 0x65, 0x03, 0x0C, 0x00, 0x10, 0x00, 0x01, 0x1D, 0x00, 0xBC
, 0x52, 0xD0, 0x1E, 0x20, 0xB8, 0x28, 0x55, 0x40, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D
, 0x80, 0xD0, 0x72, 0x1C, 0x16, 0x20, 0x10, 0x2C, 0x25, 0x80, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x9E
, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00, 0x13, 0x8E, 0x21, 0x00
, 0x00, 0x18, 0x8C, 0x0A, 0xD0, 0x90, 0x20, 0x40, 0x31, 0x20, 0x0C, 0x40, 0x55, 0x00, 0x13, 0x8E
, 0x21, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44
}; */ 
//#ifdef CC_AUD_DVBT_SUPPORT
#ifdef CC_MT8223
static UINT8 _au1EDID_CEC_48k_DeepColor12B_P1[SIF_EDID_SIZE] = 
{
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x36, 0x8B, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x0F, 0x01, 0x03, 0x80, 0x3C, 0x22, 0x78, 0x0A, 0x0D, 0xC9, 0xA0, 0x57, 0x47, 0x98, 0x27, 
	0x12, 0x48, 0x4C, 0xBF, 0xEF, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20, 0x6E, 0x28, 
	0x55, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16, 0x20, 
	0x58, 0x2C, 0x25, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x9E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x4D, 
	0x54, 0x4B, 0x20, 0x4C, 0x43, 0x44, 0x54, 0x56, 0x0A, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD, 
	0x00, 0x31, 0x4C, 0x0F, 0x50, 0x0E, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x5A, 
	0x02, 0x03, 0x20, 0x74, 0x4B, 0x84, 0x10, 0x1F, 0x05, 0x13, 0x14, 0x01, 0x02, 0x11, 0x06, 0x15, 
	0x23, 0x09, 0x07, 0x03, 0x83, 0x01, 0x00, 0x00, 0x67, 0x03, 0x0C, 0x00, 0x10, 0x00, 0xB8, 0x2D, 
	0x01, 0x1D, 0x00, 0xBC, 0x52, 0xD0, 0x1E, 0x20, 0xB8, 0x28, 0x55, 0x40, 0xC4, 0x8E, 0x21, 0x00, 
	0x00, 0x1E, 0x01, 0x1D, 0x80, 0xD0, 0x72, 0x1C, 0x16, 0x20, 0x10, 0x2C, 0x25, 0x80, 0xC4, 0x8E, 
	0x21, 0x00, 0x00, 0x9E, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00, 
	0x13, 0x8E, 0x21, 0x00, 0x00, 0x18, 0x8C, 0x0A, 0xD0, 0x90, 0x20, 0x40, 0x31, 0x20, 0x0C, 0x40, 
	0x55, 0x00, 0x13, 0x8E, 0x21, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78   
};
static UINT8 _au1EDID_CEC_48k_DeepColor12B_P2[SIF_EDID_SIZE] = 
{
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x36, 0x8B, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x0F, 0x01, 0x03, 0x80, 0x3C, 0x22, 0x78, 0x0A, 0x0D, 0xC9, 0xA0, 0x57, 0x47, 0x98, 0x27,
	0x12, 0x48, 0x4C, 0xBF, 0xEF, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20, 0x6E, 0x28,
	0x55, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16, 0x20,
	0x58, 0x2C, 0x25, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x9E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x4D,
	0x54, 0x4B, 0x20, 0x4C, 0x43, 0x44, 0x54, 0x56, 0x0A, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
	0x00, 0x31, 0x4C, 0x0F, 0x50, 0x0E, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x5A,
	0x02, 0x03, 0x20, 0x74, 0x4B, 0x84, 0x10, 0x1F, 0x05, 0x13, 0x14, 0x01, 0x02, 0x11, 0x06, 0x15,
	0x23, 0x09, 0x07, 0x03, 0x83, 0x01, 0x00, 0x00, 0x67, 0x03, 0x0C, 0x00, 0x20, 0x00, 0xB8, 0x2D,
	0x01, 0x1D, 0x00, 0xBC, 0x52, 0xD0, 0x1E, 0x20, 0xB8, 0x28, 0x55, 0x40, 0xC4, 0x8E, 0x21, 0x00,
	0x00, 0x1E, 0x01, 0x1D, 0x80, 0xD0, 0x72, 0x1C, 0x16, 0x20, 0x10, 0x2C, 0x25, 0x80, 0xC4, 0x8E,
	0x21, 0x00, 0x00, 0x9E, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00,
	0x13, 0x8E, 0x21, 0x00, 0x00, 0x18, 0x8C, 0x0A, 0xD0, 0x90, 0x20, 0x40, 0x31, 0x20, 0x0C, 0x40,
	0x55, 0x00, 0x13, 0x8E, 0x21, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68
}; 
static UINT8 _au1EDID_CEC_48k_DeepColor12B_P3[SIF_EDID_SIZE] = 
{
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x36, 0x8B, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x0F, 0x01, 0x03, 0x80, 0x3C, 0x22, 0x78, 0x0A, 0x0D, 0xC9, 0xA0, 0x57, 0x47, 0x98, 0x27, 
	0x12, 0x48, 0x4C, 0xBF, 0xEF, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20, 0x6E, 0x28, 
	0x55, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16, 0x20, 
	0x58, 0x2C, 0x25, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x9E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x4D, 
	0x54, 0x4B, 0x20, 0x4C, 0x43, 0x44, 0x54, 0x56, 0x0A, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD, 
	0x00, 0x31, 0x4C, 0x0F, 0x50, 0x0E, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x5A, 
	0x02, 0x03, 0x20, 0x74, 0x4B, 0x84, 0x10, 0x1F, 0x05, 0x13, 0x14, 0x01, 0x02, 0x11, 0x06, 0x15, 
	0x23, 0x09, 0x07, 0x03, 0x83, 0x01, 0x00, 0x00, 0x67, 0x03, 0x0C, 0x00, 0x30, 0x00, 0xB8, 0x2D, 
	0x01, 0x1D, 0x00, 0xBC, 0x52, 0xD0, 0x1E, 0x20, 0xB8, 0x28, 0x55, 0x40, 0xC4, 0x8E, 0x21, 0x00, 
	0x00, 0x1E, 0x01, 0x1D, 0x80, 0xD0, 0x72, 0x1C, 0x16, 0x20, 0x10, 0x2C, 0x25, 0x80, 0xC4, 0x8E, 
	0x21, 0x00, 0x00, 0x9E, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00, 
	0x13, 0x8E, 0x21, 0x00, 0x00, 0x18, 0x8C, 0x0A, 0xD0, 0x90, 0x20, 0x40, 0x31, 0x20, 0x0C, 0x40, 
	0x55, 0x00, 0x13, 0x8E, 0x21, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58
}; 
#else
static UINT8 _au1EDID_CEC_192k_DeepColor12B_P1[SIF_EDID_SIZE] = 
{
	0x00, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0x00,   0x36, 0x8B, 0x01, 0x00,  0x01, 0x01, 0x01, 0x01, 
	0x01, 0x0F, 0x01, 0x03,  0x80, 0x3C, 0x22, 0x78,   0x0A, 0x0D, 0xC9, 0xA0,  0x57, 0x47, 0x98, 0x27, 
	0x12, 0x48, 0x4C, 0xBF,  0xEF, 0x00, 0x01, 0x01,   0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x01, 0x01, 
	0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x01, 0x1D,   0x00, 0x72, 0x51, 0xD0,  0x1E, 0x20, 0x6E, 0x28, 
	0x55, 0x00, 0xC4, 0x8E,  0x21, 0x00, 0x00, 0x1E,   0x01, 0x1D, 0x80, 0x18,  0x71, 0x1C, 0x16, 0x20, 
	0x58, 0x2C, 0x25, 0x00,  0xC4, 0x8E, 0x21, 0x00,   0x00, 0x9E, 0x00, 0x00,  0x00, 0xFC, 0x00, 0x4D, 
	0x54, 0x4B, 0x20, 0x4C,  0x43, 0x44, 0x54, 0x56,   0x0A, 0x20, 0x20, 0x20,  0x00, 0x00, 0x00, 0xFD, 
	0x00, 0x31, 0x4C, 0x0F,  0x50, 0x0E, 0x00, 0x0A,   0x20, 0x20, 0x20, 0x20,  0x20, 0x20, 0x01, 0x56, 
	0x02, 0x03, 0x23, 0x74,  0x4B, 0x84, 0x10, 0x1F,   0x05, 0x13, 0x14, 0x01,  0x02, 0x11, 0x06, 0x15, 
	0x26, 0x09, 0x7F, 0x03,  0x11, 0x7F, 0x18, 0x83,   0x01, 0x00, 0x00, 0x67,  0x03, 0x0C, 0x00, 0x10, 
	0x00, 0xB8, 0x2D, 0x01,  0x1D, 0x00, 0xBC, 0x52,   0xD0, 0x1E, 0x20, 0xB8,  0x28, 0x55, 0x40, 0xC4, 
	0x8E, 0x21, 0x00, 0x00,  0x1E, 0x01, 0x1D, 0x80,   0xD0, 0x72, 0x1C, 0x16,  0x20, 0x10, 0x2C, 0x25, 
	0x80, 0xC4, 0x8E, 0x21,  0x00, 0x00, 0x9E, 0x8C,   0x0A, 0xD0, 0x8A, 0x20,  0xE0, 0x2D, 0x10, 0x10, 
	0x3E, 0x96, 0x00, 0x13,  0x8E, 0x21, 0x00, 0x00,   0x18, 0x8C, 0x0A, 0xD0,  0x90, 0x20, 0x40, 0x31, 
	0x20, 0x0C, 0x40, 0x55,  0x00, 0x13, 0x8E, 0x21,   0x00, 0x00, 0x18, 0x00,  0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x52
}; 
static UINT8 _au1EDID_CEC_192k_DeepColor12B_P2[SIF_EDID_SIZE] = 
{
	0x00, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0x00,   0x36, 0x8B, 0x01, 0x00,  0x01, 0x01, 0x01, 0x01, 
	0x01, 0x0F, 0x01, 0x03,  0x80, 0x3C, 0x22, 0x78,   0x0A, 0x0D, 0xC9, 0xA0,  0x57, 0x47, 0x98, 0x27, 
	0x12, 0x48, 0x4C, 0xBF,  0xEF, 0x00, 0x01, 0x01,   0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x01, 0x01, 
	0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x01, 0x1D,   0x00, 0x72, 0x51, 0xD0,  0x1E, 0x20, 0x6E, 0x28, 
	0x55, 0x00, 0xC4, 0x8E,  0x21, 0x00, 0x00, 0x1E,   0x01, 0x1D, 0x80, 0x18,  0x71, 0x1C, 0x16, 0x20, 
	0x58, 0x2C, 0x25, 0x00,  0xC4, 0x8E, 0x21, 0x00,   0x00, 0x9E, 0x00, 0x00,  0x00, 0xFC, 0x00, 0x4D, 
	0x54, 0x4B, 0x20, 0x4C,  0x43, 0x44, 0x54, 0x56,   0x0A, 0x20, 0x20, 0x20,  0x00, 0x00, 0x00, 0xFD, 
	0x00, 0x31, 0x4C, 0x0F,  0x50, 0x0E, 0x00, 0x0A,   0x20, 0x20, 0x20, 0x20,  0x20, 0x20, 0x01, 0x56, 
	0x02, 0x03, 0x23, 0x74,  0x4B, 0x84, 0x10, 0x1F,   0x05, 0x13, 0x14, 0x01,  0x02, 0x11, 0x06, 0x15, 
	0x26, 0x09, 0x7F, 0x03,  0x11, 0x7F, 0x18, 0x83,   0x01, 0x00, 0x00, 0x67,  0x03, 0x0C, 0x00, 0x20, 
	0x00, 0xB8, 0x2D, 0x01,  0x1D, 0x00, 0xBC, 0x52,   0xD0, 0x1E, 0x20, 0xB8,  0x28, 0x55, 0x40, 0xC4, 
	0x8E, 0x21, 0x00, 0x00,  0x1E, 0x01, 0x1D, 0x80,   0xD0, 0x72, 0x1C, 0x16,  0x20, 0x10, 0x2C, 0x25, 
	0x80, 0xC4, 0x8E, 0x21,  0x00, 0x00, 0x9E, 0x8C,   0x0A, 0xD0, 0x8A, 0x20,  0xE0, 0x2D, 0x10, 0x10, 
	0x3E, 0x96, 0x00, 0x13,  0x8E, 0x21, 0x00, 0x00,   0x18, 0x8C, 0x0A, 0xD0,  0x90, 0x20, 0x40, 0x31, 
	0x20, 0x0C, 0x40, 0x55,  0x00, 0x13, 0x8E, 0x21,   0x00, 0x00, 0x18, 0x00,  0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x42
}; 
static UINT8 _au1EDID_CEC_192k_DeepColor12B_P3[SIF_EDID_SIZE] = 
{
	0x00, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0x00,   0x36, 0x8B, 0x01, 0x00,  0x01, 0x01, 0x01, 0x01, 
	0x01, 0x0F, 0x01, 0x03,  0x80, 0x3C, 0x22, 0x78,   0x0A, 0x0D, 0xC9, 0xA0,  0x57, 0x47, 0x98, 0x27, 
	0x12, 0x48, 0x4C, 0xBF,  0xEF, 0x00, 0x01, 0x01,   0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x01, 0x01, 
	0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x01, 0x1D,   0x00, 0x72, 0x51, 0xD0,  0x1E, 0x20, 0x6E, 0x28, 
	0x55, 0x00, 0xC4, 0x8E,  0x21, 0x00, 0x00, 0x1E,   0x01, 0x1D, 0x80, 0x18,  0x71, 0x1C, 0x16, 0x20, 
	0x58, 0x2C, 0x25, 0x00,  0xC4, 0x8E, 0x21, 0x00,   0x00, 0x9E, 0x00, 0x00,  0x00, 0xFC, 0x00, 0x4D, 
	0x54, 0x4B, 0x20, 0x4C,  0x43, 0x44, 0x54, 0x56,   0x0A, 0x20, 0x20, 0x20,  0x00, 0x00, 0x00, 0xFD, 
	0x00, 0x31, 0x4C, 0x0F,  0x50, 0x0E, 0x00, 0x0A,   0x20, 0x20, 0x20, 0x20,  0x20, 0x20, 0x01, 0x56, 
	0x02, 0x03, 0x23, 0x74,  0x4B, 0x84, 0x10, 0x1F,   0x05, 0x13, 0x14, 0x01,  0x02, 0x11, 0x06, 0x15, 
	0x26, 0x09, 0x7F, 0x03,  0x11, 0x7F, 0x18, 0x83,   0x01, 0x00, 0x00, 0x67,  0x03, 0x0C, 0x00, 0x30, 
	0x00, 0xB8, 0x2D, 0x01,  0x1D, 0x00, 0xBC, 0x52,   0xD0, 0x1E, 0x20, 0xB8,  0x28, 0x55, 0x40, 0xC4, 
	0x8E, 0x21, 0x00, 0x00,  0x1E, 0x01, 0x1D, 0x80,   0xD0, 0x72, 0x1C, 0x16,  0x20, 0x10, 0x2C, 0x25, 
	0x80, 0xC4, 0x8E, 0x21,  0x00, 0x00, 0x9E, 0x8C,   0x0A, 0xD0, 0x8A, 0x20,  0xE0, 0x2D, 0x10, 0x10, 
	0x3E, 0x96, 0x00, 0x13,  0x8E, 0x21, 0x00, 0x00,   0x18, 0x8C, 0x0A, 0xD0,  0x90, 0x20, 0x40, 0x31, 
	0x20, 0x0C, 0x40, 0x55,  0x00, 0x13, 0x8E, 0x21,   0x00, 0x00, 0x18, 0x00,  0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x32
}; 
#endif // CC_AUD_DVBT_SUPPORT
#if 0
// SVD05L1 & SVD05L2
static const UINT8 _au1EDID[SIF_EDID_SIZE] = 
{
 0x00, 0xff ,0xff, 0xff ,0xff, 0xff ,0xff, 0x00 ,0x41, 0x0c ,0x07, 0xd0 ,0x01, 0x01 ,0x01, 0x01
,0x01, 0x11 ,0x01, 0x03 ,0x80, 0x40 ,0x24, 0x78 ,0x0a, 0xbe ,0x1d, 0xa1 ,0x57, 0x47 ,0x99, 0x25
,0x0f, 0x45 ,0x47, 0x23 ,0x08, 0x00 ,0x81, 0x80 ,0x01, 0x01 ,0x01, 0x01 ,0x01, 0x01 ,0x01, 0x01
,0x01, 0x01 ,0x01, 0x01 ,0x01, 0x01 ,0x02, 0x3a ,0x80, 0x18 ,0x71, 0x38 ,0x2d, 0x40 ,0x58, 0x2c
,0x45, 0x00 ,0x80, 0x68 ,0x21, 0x00 ,0x00, 0x1e ,0x01, 0x1d ,0x80, 0x18 ,0x71, 0x1c ,0x16, 0x20
,0x58, 0x2c ,0x25, 0x00 ,0x80, 0x68 ,0x21, 0x00 ,0x00, 0x9e ,0x00, 0x00 ,0x00, 0xfc ,0x00, 0x4d
,0x41, 0x47 ,0x4e, 0x41 ,0x56, 0x4f ,0x58, 0x20 ,0x54, 0x56 ,0x0a, 0x20 ,0x00, 0x00 ,0x00, 0xfd
,0x00, 0x38 ,0x3e, 0x1e ,0x41, 0x0f ,0x00, 0x0a ,0x20, 0x20 ,0x20, 0x20 ,0x20, 0x20 ,0x01, 0x15
,0x02, 0x03 ,0x1b, 0x70 ,0x48, 0x01 ,0x02, 0x03 ,0x04, 0x05 ,0x06, 0x07 ,0x10, 0x23 ,0x89, 0xd7
,0xfb, 0x83 ,0x01, 0x00 ,0x00, 0x65 ,0x03, 0x0c ,0x00, 0x10 ,0x00, 0x01 ,0x1d, 0x00 ,0x72, 0x51
,0xd0, 0x1e ,0x20, 0x6e ,0x28, 0x55 ,0x00, 0x80 ,0x68, 0x21 ,0x00, 0x00 ,0x1e, 0x8c ,0x0a, 0xd0
,0x8a, 0x20 ,0xe0, 0x2d ,0x10, 0x10 ,0x3e, 0x96 ,0x00, 0x80 ,0x68, 0x21 ,0x00, 0x00 ,0x18, 0x8c
,0x0a, 0xa0 ,0x14, 0x51 ,0xf0, 0x16 ,0x00, 0x26 ,0x7c, 0x43 ,0x00, 0x80 ,0x68, 0x21 ,0x00, 0x00
,0x98, 0x8c ,0x0a, 0xd0 ,0x8a, 0x20 ,0xe0, 0x2d ,0x10, 0x10 ,0x3e, 0x96 ,0x00, 0x90 ,0x2c, 0x11
,0x00, 0x00 ,0x18, 0x8c ,0x0a, 0xa0 ,0x14, 0x51 ,0xf0, 0x16 ,0x00, 0x26 ,0x7c, 0x43 ,0x00, 0x90
,0x2c, 0x11 ,0x00, 0x00 ,0x98, 0x00 ,0x00, 0x00 ,0x00, 0x00 ,0x00, 0x00 ,0x00, 0x00 ,0x00, 0x3b
};
#endif
#if 0
static UINT8 _EDID_TEST_TABLE[SIF_EDID_SIZE] = 
{
    0x32, 0x33, 0x34, 0x35, 0xFF, 0xFF, 0xFF, 0x00, 0x36, 0x8B, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x11, 0x0F, 0x01, 0x03, 0x80, 0x3C, 0x22, 0x78, 0x0A, 0x0D, 0xC9, 0xA0, 0x57, 0x47, 0x98, 0x1a, 
    0x22, 0x48, 0x4C, 0xBF, 0xEF, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x2b, 
    0x33, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20, 0x6E, 0x3c, 
    0x44, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16, 0x4d, 
    0x55, 0x2C, 0x25, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x9E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x5e, 
    0x66, 0x4B, 0x20, 0x4C, 0x43, 0x44, 0x54, 0x56, 0x0A, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x6f, 
    0x77, 0x31, 0x4C, 0x0F, 0x50, 0x0E, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x70, 
    0x88, 0x03, 0x20, 0x74, 0x4B, 0x84, 0x10, 0x1F, 0x05, 0x13, 0x14, 0x01, 0x02, 0x11, 0x06, 0x81, 
    0x99, 0x09, 0x57, 0x03, 0x83, 0x01, 0x00, 0x00, 0x67, 0x03, 0x0C, 0x00, 0x10, 0x00, 0xB8, 0x92, 
    0xaa, 0x1D, 0x00, 0xBC, 0x52, 0xD0, 0x1E, 0x20, 0xB8, 0x28, 0x55, 0x40, 0xC4, 0x8E, 0x21, 0xa3, 
    0xbb, 0x1E, 0x01, 0x1D, 0x80, 0xD0, 0x72, 0x1C, 0x16, 0x20, 0x10, 0x2C, 0x25, 0x80, 0xC4, 0xb4, 
    0xcc, 0x00, 0x00, 0x9E, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0xc5, 
    0xdd, 0x8E, 0x21, 0x00, 0x00, 0x18, 0x8C, 0x0A, 0xD0, 0x90, 0x20, 0x40, 0x31, 0x20, 0x0C, 0xd6, 
    0xee, 0x00, 0x13, 0x8E, 0x21, 0x00, 0x00, 0x18, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 
    0xff, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x30, 0x31, 0x88, 0x88, 0x18, 0x50
}; 
#endif
/* Declare the debug on/off/level and RegTest functions */
CLIMOD_DEBUG_FUNCTIONS(SIF)
#if 0
#ifdef CC_CLI   
static REG_TEST_T arSIFRgtList [] =
{
    { 0x0240, eRD_WR, 4, 0x0fff071e, 1, 0x10 },
    { 0x0244, eRD_ONLY, 4, 0x03ff0707, 1, 0 },
    { 0x0248, eRD_WR, 2, 0x000000fe, 0, 0 },
    { 0x024C, eRD_WR, 2, 0x000000ff, 0, 0 },
    { 0x0250, eRD_WR, 4, 0xffffffff, 0, 0 },
    { 0x0254, eRD_WR, 4, 0xffffffff, 0, 0 },

    // End.
    { 0xffff, eNO_TYPE, -1 }
};

// CLIMOD_REGTST_FUNCTIONS(SIF)
#endif
#endif
/* End of Declare */

static INT32 _Sif_Monitor_Cmd(INT32 i4Argc, const CHAR * *szArgv)
{
    char * pstr;
    INT32 i4PinNum, i4PinFunc, i4Ret, i4PinGrp, i4PinOldFunc;
    
    if (i4Argc<2)
    {
        Printf("Arg: [Option=ps,...] [Option Parameter]\n");
        return 0;
    }

    pstr = (char *)szArgv[1] ;

    if ( x_strcmp(pstr, "ps")==0 )
    {
        if( i4Argc ==2 )
        {
            Printf("... ps gpio#   pin_func \n");
            return -1;
        }
        else if( i4Argc ==3 )   //sif.m ps xxx
        {
            i4PinNum = StrToInt( szArgv[2] );
            i4PinFunc =BSP_PinGpioGet( i4PinNum , &i4PinGrp );
            Printf("Gpio %d 's pin_func= %d\n", i4PinNum, i4PinFunc );
        }
        else    //sif.m ps xxx func
        {
            i4PinNum = StrToInt( szArgv[2] );
            i4PinFunc = StrToInt( szArgv[3] );
            i4PinOldFunc = BSP_PinGpioGet( i4PinNum , &i4PinGrp ) ;
            i4Ret = BSP_PinSet(i4PinGrp, i4PinFunc);
            Printf("Gpio %d 's old pin_func %d -->set to %d %s\n", i4PinNum, i4PinOldFunc ,i4PinFunc, i4Ret==0?"OK":"Fail" );
        }
    }
    
    return 0;
}

static INT32 _SifScan(INT32 i4Argc, const CHAR  * *szArgv)
{
    UINT8 u1Data[128];
    INT32   i4Ret, i4Index;
    UINT8 u1BusId;

    SIF_Init();

    Printf("Scan SIF device(s) on all buses...\n");
    for(u1BusId= 0; u1BusId< SIF_BUS_MAX ; u1BusId++)
    {
        switch (u1BusId)
        {
            case SIF_BUS_SYSTEM_I2C:
                Printf("\rSystem BUS:                                      \n");
                break;
            case SIF_BUS_TUNER_I2C:
                Printf("\rTuner BUS:                                      \n");
                break;
            case SIF_BUS_SECOND_I2C:
                Printf("\rSecond BUS:                                      \n");
                break;
            case SIF_BUS_PDWNC_I2C:
                Printf("\rPdwnc BUS:                                      \n");
                break;
			case SIF_BUS_8295B_I2C_0:
				Printf("\r8295B BUS0:                                      \n");
                break;
			case SIF_BUS_8295B_I2C_1:
				Printf("\r8295B BUS1:                                      \n");
                break;
            default:
                Printf("\rBUS %d:                                      \n", u1BusId);
                break;
        }

        for( i4Index = 0x00;  i4Index <= 0xFF ; i4Index+=2)
        {
            Printf("\rScan (read) device addr 0x%02X", i4Index );
            //        i4Ret = SIF_TunerReadNoSubAddr(0x100, i4Index, u1Data, 1);
            i4Ret = SIF_X_Read(u1BusId ,0x100, i4Index, 0 , 0 ,u1Data, 1);
            if ( i4Ret > 0 )
                Printf("\rFound readable device at addr 0x%02X\n", i4Index);
        };
        Printf("\r                                           \n");
    }

    Printf("\rDone.                                      \n");
    return 0;
}

#ifdef CC_Support_Sil9285
UINT8 NVRAMboottable[64] = 
{
#if 1
/*  00    01    02   03     04    05    06    07   08     09    0A    0B    0C    0D    0E   0F */
 0xAA,0x55 ,0x04 ,0x1F ,0x0F ,0x00 ,0x00 ,0xFF ,0xFF ,0xFF ,0x9D ,0xFF ,0x40 ,0x00 ,0x10 ,0x00,
 0x30,0x00 ,0x20 ,0x00 ,0x50 ,0x00 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0x10 ,0x40 ,0x20 ,0x30,
 0x00,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
	0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00

#else

0xAA,0x55,0x04, 0x1F, 0x0F,   0x00, 0x00,  0xFF, 0xFF, 0xFF,  0x9D,  0xFF,  0x10,   0x00,  0x20, 0x00, 
0x30,0x00,0x40, 0x00, 0x50,   0x00, 0xFF,  0xFF, 0xFF, 0xFF,  0xFF,  0xFF,  0x38,   0x28,  0x18, 0x08, 
0xF8,0x00,0x00,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
0x00,0x00,0x00,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
#endif
};

 UINT8 NVRAMtable[256] = 
{
#if 1
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x4D, 0xD9, 0x01, 0xEB, 0x01, 0x01, 0x01, 0x01,
 0x01,0x14 ,0x01 ,0x03 ,0x80 ,0xA0 ,0x5A ,0x78 ,0x0A ,0x83 ,0xAD ,0xA2 ,0x56 ,0x49 ,0x9B ,0x25,
    0x0F, 0x47, 0x4A, 0x21, 0x08, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x66, 0x21, 0x50, 0xB0, 0x51, 0x00, 0x1B, 0x30, 0x40, 0x70,
    0x36, 0x00, 0x40, 0x84, 0x63, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20,
	0x6E,0x28 ,0x55 ,0x00 ,0x40 ,0x84 ,0x63 ,0x00 ,0x00 ,0x1E ,0x00 ,0x00 ,0x00 ,0xFD ,0x00 ,0x3A,
    0x3E, 0x0F, 0x44, 0x0F, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFC,
    0x00, 0x53, 0x4F, 0x4E, 0x59, 0x20, 0x54, 0x56, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x09,
	0x02,0x03 ,0x20 ,0x70 ,0x49 ,0x10 ,0x04 ,0x05 ,0x03 ,0x02 ,0x07 ,0x06 ,0x20 ,0x01 ,0x26 ,0x09,
    0x07, 0x07, 0x15, 0x07, 0x50, 0x83, 0x01, 0x00, 0x00, 0x66, 0x03, 0x0C, 0x00, 0x10, 0x00, 0x80,
	0x01,0x1D ,0x80 ,0x18 ,0x71 ,0x1C ,0x16 ,0x20 ,0x58 ,0x2C ,0x25 ,0x00 ,0x40 ,0x84 ,0x63 ,0x00,
    0x00, 0x9E, 0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C, 0x45, 0x00, 0x40, 0x84,
    0x63, 0x00, 0x00, 0x1E, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00,
    0x40, 0x84, 0x63, 0x00, 0x00, 0x18, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E,
    0x96, 0x00, 0xB0, 0x84, 0x43, 0x00, 0x00, 0x18, 0x8C, 0x0A, 0xA0, 0x14, 0x51, 0xF0, 0x16, 0x00,
    0x26, 0x7C, 0x43, 0x00, 0xB0, 0x84, 0x43, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40

#else
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x4D, 0xD9, 0x01, 0xEA, 0x01, 0x01, 0x01, 0x01,
    	0x01, 0x14, 0x01, 0x03, 0x80, 0xA0, 0x5A, 0x78, 0x0A, 0x83, 0xAD, 0xA2, 0x56, 0x49, 0x9B, 0x25,
    	0x0F, 0x47, 0x4A, 0x21, 0x08, 0x00, 0x81, 0x80, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,
   	0x45, 0x00, 0x40, 0x84, 0x63, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20,
    	0x6E, 0x28, 0x55, 0x00, 0x40, 0x84, 0x63, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x3A,
    	0x3E, 0x0F, 0x46, 0x0F, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFC,
    	0x00, 0x53, 0x4F, 0x4E, 0x59, 0x20, 0x54, 0x56, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x5F,
    	0x02, 0x03, 0x20, 0x70, 0x49, 0x10, 0x04, 0x05, 0x03, 0x02, 0x07, 0x06, 0x20, 0x01, 0x26, 0x09,
    	0x07, 0x07, 0x15, 0x07, 0x50, 0x83, 0x01, 0x00, 0x00, 0x66, 0x03, 0x0C, 0x00, 0x10, 0x00, 0x80,
    	0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16, 0x20, 0x58, 0x2C, 0x25, 0x00, 0x40, 0x84, 0x63, 0x00,
    	0x00, 0x9E, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00, 0x40, 0x84,
    	0x63, 0x00, 0x00, 0x18, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00,
    	0xB0, 0x84, 0x43, 0x00, 0x00, 0x18, 0x8C, 0x0A, 0xA0, 0x14, 0x51, 0xF0, 0x16, 0x00, 0x26, 0x7C,
    	0x43, 0x00, 0xB0, 0x84, 0x43, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38
#endif
};

extern UINT8 _bHDMI9287switchstopflag;
static INT32 _SifHDMISwitchI2CCtrl(INT32 i4Argc, const CHAR  * *szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s 1 is stop/ 0 is resume\n", szArgv[0]);
		return 0;
	}
	_bHDMI9287switchstopflag=(UINT8)StrToInt(szArgv[1]);
	if(_bHDMI9287switchstopflag)
	Printf("Stop HDMI 9287 I2C access\n");
	else
	Printf("Resume HDMI 9287 I2C access\n");
    return 0;
}
static INT32 _SifHDMI9287SwitchEDIDRead(INT32 i4Argc, const CHAR  * *szArgv)
{
	UINT32 ret;
	UINT8 bEDID[256];
	if (i4Argc != 2)
	{
		Printf("Usage: %s  is port ,port 5 is NVRAMboottable and port 6 is NVRAMtable\n", szArgv[0]);
		return 0;
	}
	ret=vSiI9287EDIDread((UINT8)StrToInt(szArgv[1]) ,bEDID);
	if(ret)
	{
		Printf(" HDMI 9287 I2C EDID read OK\n");

	}
	else
	{
		Printf(" HDMI 9287 I2C EDID read fail\n");
	}
    return 0;
}
static INT32 _SifHDMI9287SwitchKSVRead(INT32 i4Argc, const CHAR  * *szArgv)
{
	UINT8 ret;
	UINT8 bKSV[5];
	ret=vSiI9287HDCPKSVread(bKSV);
	if(ret)
	{
		Printf(" HDMI 9287 I2C KSV read OK\n");

	}
	else
	{
		Printf(" HDMI 9287 I2C KSV read fail\n");
	}
    return 0;
}

static INT32 _SifHDMI9287SwitchEDIDWrite(INT32 i4Argc, const CHAR  * *szArgv)
{
	UINT8 ret;
	ret=vSiI9287EDIDWrite(NVRAMboottable,NVRAMtable);
	if(ret)
	{
		Printf(" HDMI 9287 I2C EDID write success \n");
	}
	else
	{
		Printf(" HDMI 9287 I2C EDID write fail\n");
	}
    return 0;
}
static INT32 _SifHDMISwitchPWRStatus(INT32 i4Argc, const CHAR  * *szArgv)
{
	
	if (i4Argc != 2)
	{
		Printf("Usage: %s port \n", szArgv[0]);
		return 0;
	}
	Printf("HDMI 9287 port %s PWR5V = %d",szArgv[1],bHDMISwitch5VStatus((UINT8)StrToInt(szArgv[1])));
    return 0;
}
static INT32 _SifHDMISwitchCLKStatus(INT32 i4Argc, const CHAR  * *szArgv)
{
	
	if (i4Argc != 2)
	{
		Printf("Usage: %s port \n", szArgv[0]);
		return 0;
	}
	Printf("HDMI 9287 port %s CLK status = %d",szArgv[1],bHDMISwitchCLKStatus((UINT8)StrToInt(szArgv[1])));
    return 0;
}
static INT32 _SifHDMI9287SwitchHDCPStatusRead(INT32 i4Argc, const CHAR  * *szArgv)
{
	UINT8 ret;
	ret=bHDMISwitchHDCPStatus(1);
	Printf(" HDMI 9287 I2C HDCP status =%x\n",ret);
    return 0;
}
static INT32 _SifHDMISwitchHPDStatus(INT32 i4Argc, const CHAR  * *szArgv)
{
	
	if (i4Argc != 2)
	{
		Printf("Usage: %s port \n", szArgv[0]);
		return 0;
	}
	Printf("HDMI 9287 port %s HPD status = %d",szArgv[1],bHDMISwitchHPDStatus((UINT8)StrToInt(szArgv[1])));
    return 0;
}

#endif

static CLI_EXEC_T _arSifCmdTbl [] =
{
#if !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE)
    // General sif diagnostic function.
    { "init", "i", _SifInitCmd, NULL, "Sif init", CLI_GUEST },
    { "diag", NULL, _SifDiagCmd, NULL, "Sif diagnostic", CLI_SUPERVISOR },

    // General sif read/write function.
    { "read", "r", _SifReadCmd, NULL, "Sif read", CLI_GUEST },
    { "write", "w", _SifWriteCmd, NULL, "Sif write", CLI_GUEST },
    { "writebyte", "wb", _SifWriteByteCmd, NULL, "Sif write byte", CLI_GUEST },    
    { "softread", "sr", _SifSWReadCmd, NULL, "Sif S/W Read", CLI_SUPERVISOR },    
    { "softwrite", "sw", _SifSWWriteCmd, NULL, "Sif S/W Write", CLI_SUPERVISOR }, 
        
    { "softread(0-SubAddr)", "sr0", _SifSWRead0Cmd, NULL, "Sif S/W Read(0-SubAddr)", CLI_SUPERVISOR },    
    { "softwrite(0-SubAddr)", "sw0", _SifSWWrite0Cmd, NULL, "Sif S/W Write(0-SubAddr)", CLI_SUPERVISOR },    

     // HDMI HDCP read/write function.
     { "hdcp", NULL, _SifHDCPWriteSramCmd, NULL, "Sif write HDCP SRAM default value", CLI_GUEST },    
     { "rhdcp", NULL, _SifHDCPReadCmd, NULL, "Sif read HDCP", CLI_GUEST },    
     { "wbhdcp", NULL, _SifHDCPWriteByteCmd, NULL, "Sif write byte to HDCP", CLI_GUEST },    
     { "rbhdcp", NULL, _SifHDCPReadByteCmd, NULL, "Sif read byte from HDCP", CLI_GUEST },    
#endif /* !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE) */
     { "wallhdcp", "wah", _SifHDCPWrAllByteCmd, NULL, "Sif write all 320 bytes to HDCP", CLI_GUEST },
     { "rallhdcp", "rah", _SifHDCPRdAllByteCmd, NULL, "Sif read all 320 bytes from HDCP", CLI_GUEST },
     { "wrallhdcp", "wrah", _SifHDCPWrRdAllByteCmd, NULL, "Sif write all 320 bytes to HDCP", CLI_GUEST },
#if !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE)
     // HDMI EDID read/write function.
     { "edid", NULL, _SifEDIDWriteCmd, NULL, "Sif write EDID default value", CLI_GUEST },    
     { "redid", NULL, _SifEDIDReadCmd, NULL, "Sif read EDID", CLI_GUEST },    
     { "edid1", NULL, _SifEDID1WriteCmd, NULL, "Sif write EDID port 1 default value", CLI_GUEST },  
     { "edid2", NULL, _SifEDID2WriteCmd, NULL, "Sif write EDID port 2 default value", CLI_GUEST },  
     { "edid3", NULL, _SifEDID3WriteCmd, NULL, "Sif write EDID port 3 default value", CLI_GUEST },  
     { "redid1", NULL, _SifEDID1ReadCmd, NULL, "Sif read EDID port 1", CLI_GUEST }, 
     { "redid2", NULL, _SifEDID2ReadCmd, NULL, "Sif read EDID port 2", CLI_GUEST },
     { "redid3", NULL, _SifEDID3ReadCmd, NULL, "Sif read EDID port 3", CLI_GUEST },
     //{ "wbedid", NULL, _SifEDIDWriteByteCmd, NULL, "Sif write byte to EDID", CLI_GUEST },    
     //{ "rbedid", NULL, _SifEDIDReadByteCmd, NULL, "Sif read byte from EDID", CLI_GUEST },    
#endif /* !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE) */
     { "walledid", "wae", _SifEDIDWrAllByteCmd, NULL, "Sif write all 256 bytes to EDID", CLI_GUEST },
     { "ralledid", "rae", _SifEDIDRdAllByteCmd, NULL, "Sif read all 256 bytes from EDID", CLI_GUEST },
#if !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE)
     //TUNER I2C Function
     { "tunerread", "tr", _SifTunerReadNoSubCmd, NULL, "Tuner I2C No-sub-addr read", CLI_GUEST },
     { "tunerwrite", "tw", _SifTunerWriteNoSubCmd, NULL, "Tuner I2C No-sub-addr write", CLI_GUEST },

    //Multiple-sub-addr read /write
     { "multipleread", "mr", _SifReadMultiCmd, NULL, "Multiple sub-addr I2C read", CLI_GUEST },
     { "multiplewrite", "mw", _SifWriteMultiCmd, NULL, "Multiple sub-addr I2C write", CLI_GUEST },

    //scan bus functions
     { "scan", "s", _SifScan, NULL, "Scan all SIF Buses", CLI_GUEST },
     #ifdef CC_Support_Sil9285
     { "swstop", "sws", _SifHDMISwitchI2CCtrl, NULL, "Silicon image 9287 switch I2C access 1 is stop / 0 is resume", CLI_GUEST }, 
     { "swedidr", "swedidr", _SifHDMI9287SwitchEDIDRead, NULL, "Silicon image 9287 switch EDID read", CLI_GUEST },
     { "swedidw", "swedidw", _SifHDMI9287SwitchEDIDWrite, NULL, "Silicon image 9287 switch EDID write", CLI_GUEST },
     { "swksvr", "swksvr", _SifHDMI9287SwitchKSVRead, NULL, "Silicon image 9287 switch KSV read", CLI_GUEST },
     { "sw5vr", "sw5vr", _SifHDMISwitchPWRStatus, NULL, "Silicon image 9287 switch 5V status  read", CLI_GUEST },
     { "swclkr", "swclkr", _SifHDMISwitchCLKStatus, NULL, "Silicon image 9287 switch clock status  read", CLI_GUEST },
     { "swhdcpr", "swhdcpr", _SifHDMI9287SwitchHDCPStatusRead, NULL, "Silicon image 9287 switch hdcp status  read", CLI_GUEST },
     { "swhpdr", "swhpdr", _SifHDMISwitchHPDStatus, NULL, "Silicon image 9287 switch HPD status  read", CLI_GUEST },
    #endif
    //sif_x read write
     { "sif_x_read", "xr", _SifRead_X_Cmd, NULL, "fully functional sif read", CLI_GUEST },
     { "sif_x_write", "xw", _SifWrite_X_Cmd, NULL, "fully functional sif write", CLI_GUEST },
    
    //edid read byte
     { "edidreadbyte", "edidrb", _EDIDReadByteCmd, NULL, "edid read byte", CLI_SUPERVISOR },
     { "monitor", "m", _Sif_Monitor_Cmd, NULL, "Montor CLI", CLI_SUPERVISOR },
//#ifdef CC_CLI       
//    CLIMOD_REGTST_CLIENTRY(SIF),
//#endif
#endif /* !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE) */
    // Register test.  
    CLIMOD_DEBUG_CLIENTRY(SIF),

    { NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR }
};


CLI_MAIN_COMMAND_ITEM(Sif)
{
    "sif", NULL, NULL, _arSifCmdTbl, "Sif command", CLI_GUEST
};

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

static INT32 _SifInitCmd(INT32 i4Argc, const CHAR * *szArgv)
{
    UNUSED(i4Argc);

    UNUSED(szArgv);

    SIF_Init();
#ifndef MT5391_IC_VERIFY_STAGE
    // initail HDCP SRAM.
    if (EEPHDCP_Init() != 0)
    {
        Printf("HDCP Init failed\n");
    }
#endif
    Printf("SIF_Init OK !\n");
    return 0;
}

static INT32 _SifDiagCmd(INT32 i4Argc, const CHAR * *szArgv)
{
#ifdef __MODEL_slt__
    return _SIF_Diag();
#else
    _SIF_Diag();

    return 0;
#endif
}

//-------------------------------------------------------------------------
/** _SifReadCmd
 *  SIF CLI read function.
 *  @param  i4Argc              user's CLI input length.
 *  @param  szArgv              user's CLI input string.
 *  @retval  1  SUCCESS.
 *  @retval  0  FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _SifReadCmd(INT32 i4Argc, const CHAR * *szArgv)
{
    UINT8 u1Dev;

    UINT32 u4Idx;
    UINT32 u4ClkDiv;
    UINT32 u4Addr;
    UINT32 u4Count;
    UINT8 *pu1Buf;
    UINT8 *pu2Buf;
    UINT32 u4ReturnValue;

    if (i4Argc<5)
    {
        Printf("Arg: u4ClkDiv u1Dev u4Addr u4Count\n");
        return 0;
    }

    u4ClkDiv = StrToInt(szArgv[1]);
    u1Dev = (UINT8)StrToInt(szArgv[2]);
    u4Addr = StrToInt(szArgv[3]);
    u4Count = StrToInt(szArgv[4]);
    pu1Buf = (UINT8 *)x_mem_alloc(u4Count);
    if (pu1Buf == NULL)
    {
        return -1;
    }
    x_memset((VOID *)pu1Buf, 0, u4Count);

    pu2Buf = pu1Buf;

    while (u4Count>0)
    {
        u4ReturnValue = SIF_Read(u4ClkDiv, u1Dev, u4Addr, pu1Buf, u4Count);

        Printf("u1Dev=0x%X, u4Addr=0x%X, u4Count=%d, u4ReturnValue=%d.\n",
               u1Dev,
               u4Addr,
               u4Count,
               u4ReturnValue);

        if (u4ReturnValue>0)
        {
            // print sif read data content.
            for (u4Idx = 0; u4Idx<u4ReturnValue; u4Idx ++)
            {
                if (!(u4Idx % 8))
                {
                    Printf("\n0x%08X :", u4Idx);                    
                }
                
                Printf(" 0x%02X,", pu1Buf[u4Idx]);
            }
            Printf("\n");

            if (((u4ReturnValue + u4Addr) % 256) == 0)
            {
                u1Dev += 2;
                u4Addr = 0;
            }
            else
            {
                u4Addr += u4ReturnValue;
            }

            u4Count -= u4ReturnValue;

            pu1Buf += u4ReturnValue;
        }
        else
        {
            // fail.
            Printf("sif read fail.\n");
            u4Count = 0;            
        }
    }

    x_mem_free((VOID *)pu2Buf);

    return 0;
}

//-------------------------------------------------------------------------
/** _SifSWReadCmd
 *  SIF CLI SW IIC read function.
 *  @param  i4Argc              user's CLI input length.
 *  @param  szArgv              user's CLI input string.
 *  @retval  1  SUCCESS.
 *  @retval  0  FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _SifSWReadCmd(INT32 i4Argc, const CHAR * *szArgv)
{
    UINT32 u4Dev;
    UINT32 u4SDA;
    UINT32 u4SCL;    

    UINT32 u4Idx;
    UINT32 u4Addr;
    UINT32 u4Count;
    UINT32 u4Count1;    
    UINT8 *pu1Buf;
    UINT8 *pu2Buf;
    UINT32 u4ReturnValue;

    if (i4Argc<6)
    {
        Printf("Arg: SDA_pin SCL_pin u1Dev u4Addr u4Count.\n");
        return 0;
    }

    u4SDA = StrToInt(szArgv[1]);
    u4SCL = StrToInt(szArgv[2]);    
    u4Dev = (UINT8)StrToInt(szArgv[3]);
    u4Addr = StrToInt(szArgv[4]);
    u4Count = StrToInt(szArgv[5]);
    pu1Buf = (UINT8 *)x_mem_alloc(u4Count+8);
    if (pu1Buf == NULL)
    {
        return -1;
    }
    x_memset((VOID *)pu1Buf, 0, u4Count+8);
    
    // initial SIF sda, scl pin.
    SIFSW_Init(u4SDA, u4SCL);

    pu2Buf = pu1Buf;

    while (u4Count>0)
    {
        u4Count1 = (u4Count > 256) ? 256:u4Count;
        
//        u4ReturnValue = SIFSW_ReadData(u4Dev, u4Addr, u4Count1, pu1Buf, 0xFF);
         u4ReturnValue = SIFSW_ReadData((UINT8)u4Dev, (UINT8)u4Addr, (UINT16)u4Count1, pu1Buf, 0xFFU);

        Printf("u4Dev=0x%X, u4Addr=0x%X, u4Count=%d, u4ReturnValue=%d.\n",
               u4Dev,
               u4Addr,
               u4Count1,
               u4ReturnValue);

        if (!u4ReturnValue)
        {
                Printf("Read error !\n");       
                x_mem_free((VOID *)pu2Buf);
                SIFSW_Remove();
                return 0;
        }
        
        if (u4Count1>0)
        {
            for (u4Idx = 0; u4Idx<u4Count1; u4Idx += 8)
            {
                Printf(
                    "0x%08X : 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X.\n",
                    u4Idx,
                    pu1Buf[u4Idx],
                    pu1Buf[u4Idx + 1],
                    pu1Buf[u4Idx + 2],
                    pu1Buf[u4Idx + 3],
                    pu1Buf[u4Idx + 4],
                    pu1Buf[u4Idx + 5],
                    pu1Buf[u4Idx + 6],
                    pu1Buf[u4Idx + 7]);
            }
        }

      if (((u4Count1 + u4Addr) % 256) == 0)
        {
            u4Dev += 2;
            u4Addr = 0;
        }
        else
        {
            u4Addr += u4Count1;
        }
        u4Count -= u4Count1;

        pu1Buf += u4Count1;
    }

    x_mem_free((VOID *)pu2Buf);
    SIFSW_Remove();
    return 0;
}
//-------------------------------------------------------------------------
/** _SifWriteCmd
 *  SIF CLI write function.
 *  @param  i4Argc              user's CLI input length.
 *  @param  szArgv              user's CLI input string.
 *  @retval  1  SUCCESS.
 *  @retval  0  FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _SifWriteCmd(INT32 i4Argc, const CHAR * *szArgv)
{
    UINT8 u1Dev;

    UINT32 u4ClkDiv;
    UINT32 u4Addr;
    UINT32 u4Count;
    UINT32 u4ReturnValue;
    UINT32 u4Memptr;

    if (i4Argc<6)
    {
        Printf("Arg: u4ClkDiv u1Dev u4Addr u4Memptr u4Count.\n");
        return 0;
    }

    u4ClkDiv = StrToInt(szArgv[1]);
    u1Dev = (UINT8)StrToInt(szArgv[2]);
    u4Addr = StrToInt(szArgv[3]);
    u4Memptr = StrToInt(szArgv[4]);
    u4Count = StrToInt(szArgv[5]);

    u4ReturnValue = SIF_Write(u4ClkDiv, u1Dev, u4Addr, (UINT8 *)u4Memptr, u4Count);

    if (u4ReturnValue == 0xFF)
    {
        Printf("write fail.\n");
    }
    else
    {
        Printf("write successfully! byte count = %d\n", u4ReturnValue);
    }

    return 0;
}
EXTERN INT32 SIF_SCLHIGH(void)  ;
EXTERN void SIF_SCLLOW(void);
EXTERN void SIFSW_Init(UINT32 u4SDA, UINT32 u4SCL);

UINT16 _SifSwFreq2Delay(UINT16 u2Freq)
{
    UINT16  u2Delay;
    u2Delay = ((1000/u2Freq-10)/2);
    return u2Delay;
}

static INT32 _SifSWRead0Cmd(INT32 i4Argc, const CHAR * *szArgv)
{
#if 0
    UINT32 u4Dev;
    UINT32 u4SDA;
    UINT32 u4SCL;    

    UINT32 u4Idx;

    UINT32 u4Count;

    UINT8 *pu1Buf;
    UINT8 *pu2Buf;
    UINT32 u4ReturnValue;
    UINT16 u2Option=0x1ff;
    UINT16 u2Freq=100,u2Delay;

    if (i4Argc<5)
    {
        Printf("Arg: SDA_pin SCL_pin u1Dev u4Count [u2Option=0xff].\n");
//        Printf("Arg: SDA_pin SCL_pin u1Dev u4Count [u2Freq=100].\n");
        return 0;
    }

    if(i4Argc>=6)
    {
        u2Option = (UINT16)StrToInt(szArgv[5]);
//        u2Freq = (UINT16)StrToInt(szArgv[5]);
    }

    u2Delay = _SifSwFreq2Delay( u2Freq );

    u4SDA = StrToInt(szArgv[1]);
    u4SCL = StrToInt(szArgv[2]);    
    u4Dev = (UINT8)StrToInt(szArgv[3]);
    u4Count = StrToInt(szArgv[4]);
    pu1Buf = (UINT8 *)x_mem_alloc(u4Count+8);
    if (pu1Buf == NULL)
    {
        return -1;
    }
    x_memset((VOID *)pu1Buf, 0, u4Count+8);
    
    // initial SIF sda, scl pin.
    SIFSW_Init(u4SDA, u4SCL);

    pu2Buf = pu1Buf;

    if (u4Count>0)
    {
        u4Count = (u4Count > 256) ? 256:u4Count;
        
        u4ReturnValue = SIFSW_ReadDataNoSubAddr(u4Dev, u4Count, pu1Buf, u2Option, u2Delay, 1000);
 
        Printf("u4Dev=0x%X, u4Count=%d, u2Option=0x%04X, u2Delay=%d, u4ReturnValue=%d.\n",
               u4Dev,
               u4Count,
               u2Option,
               u2Delay,
               u4ReturnValue);

        if (!u4ReturnValue)
        {
                Printf("Read error !\n");       
                x_mem_free((VOID *)pu2Buf);
                SIFSW_Remove();
                return 0;
        }
        
        if (u4Count>0)
        {
            for (u4Idx = 0; u4Idx<u4Count; u4Idx += 8)
            {
                Printf(
                    "0x%08X : 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X.\n",
                    u4Idx,
                    pu1Buf[u4Idx],
                    pu1Buf[u4Idx + 1],
                    pu1Buf[u4Idx + 2],
                    pu1Buf[u4Idx + 3],
                    pu1Buf[u4Idx + 4],
                    pu1Buf[u4Idx + 5],
                    pu1Buf[u4Idx + 6],
                    pu1Buf[u4Idx + 7]);
            }
        }
   }

    x_mem_free((VOID *)pu2Buf);
    SIFSW_Remove();
#endif
    return 0;
}

static INT32 _SifSWWrite0Cmd(INT32 i4Argc, const CHAR * *szArgv)
{
#if 0
#define DATA_OFFSET 5
    UINT32 u4Dev;
    UINT32 u4SDA;
    UINT32 u4SCL;    

    UINT32 u4Idx;
//    UINT32 u4Addr;
    UINT32 u4Count;
//    UINT32 u4Count1;    
    UINT8 *pu1Buf;
    UINT8 *pu2Buf;
    UINT32 u4ReturnValue;
    UINT16 u2Option=0x1ff;
    UINT16 u2Freq=100,u2Delay;
    
    if (i4Argc<6)
    {
//        Printf("Arg: SDA_pin SCL_pin u1Dev u2Freq u1data0 [u1data1] [u1data2].\n");
        Printf("Arg: SDA_pin SCL_pin u1Dev u2Option u1data0 [u1data1] [u1data2].\n");
        return 0;
    }

    u4SDA = StrToInt(szArgv[1]);
    u4SCL = StrToInt(szArgv[2]);    
    u4Dev = (UINT8)StrToInt(szArgv[3]);
    u2Option = (UINT16)StrToInt(szArgv[4]);
//    u2Freq = (UINT16)StrToInt(szArgv[4]);
    u2Delay = _SifSwFreq2Delay( u2Freq );
    
    if(u2Option==0)
    {
        u2Option=0xff;
    }
    u4Count = (i4Argc-DATA_OFFSET);       
    pu1Buf = (UINT8 *)x_mem_alloc(u4Count);
    if (pu1Buf == NULL)
    {
        return -1;
    }
    x_memset((VOID *)pu1Buf, 0, u4Count);

    //read command line data
    for(u4Idx = 0;u4Idx<u4Count;u4Idx++)
    {
        pu1Buf[u4Idx]= (UINT8)StrToInt(szArgv[u4Idx+DATA_OFFSET]);
    }
    
    // initial SIF sda, scl pin.
    SIFSW_Init(u4SDA, u4SCL);

    pu2Buf = pu1Buf;

    if (u4Count>0)
    {
        u4Count = (u4Count > 256) ? 256:u4Count;
        
        u4ReturnValue = SIFSW_WriteDataNoSubAddr(u4Dev, u4Count, pu1Buf, u2Option, u2Delay,1000);
 
        Printf("u4Dev=0x%X,  u4Count=%d, u2Option=0x%04X, u4ReturnValue=%d.\n",
               u4Dev,
               u4Count,
               u2Option,
               u4ReturnValue);

        if (!u4ReturnValue)
        {
                Printf("Write error !\n");       
                x_mem_free((VOID *)pu2Buf);
                SIFSW_Remove();
                return 0;
        }
    }

    x_mem_free((VOID *)pu2Buf);
    SIFSW_Remove();
#endif
    return 0;
}
static INT32 _SifSWWriteCmd(INT32 i4Argc, const CHAR * *szArgv)
{
#if 1
#define MIN_ARGC 7
    UINT32 u4Dev;
    UINT32 u4SDA;
    UINT32 u4SCL;    

    UINT32 u4Idx;
//    UINT32 u4Addr;
    UINT32 u4Count;
//    UINT32 u4Count1;    
    UINT8 *pu1Buf;
    UINT8 *pu2Buf;
    UINT32 u4ReturnValue;
    UINT16 u2Option=0x1ff;
    UINT16 /*u2Freq=100,*/u2Delay;
    
    if (i4Argc<MIN_ARGC)
    {
//        Printf("Arg: SDA_pin SCL_pin u1Dev u2Freq u1data0 [u1data1] [u1data2].\n");
        Printf("Arg: SDA_pin SCL_pin u1Dev u2Option u2Delay u1data0 [u1data1] [u1data2].\n");
        return 0;
    }

    u4SDA = StrToInt(szArgv[1]);
    u4SCL = StrToInt(szArgv[2]);    
    u4Dev = (UINT8)StrToInt(szArgv[3]);
    u2Option = (UINT16)StrToInt(szArgv[4]);
    u2Delay = (UINT16)StrToInt(szArgv[4]);
    //u2Delay = _SifSwFreq2Delay( u2Freq );
    
    if(u2Option==0)
    {
        u2Option=0x1ff;
    }
    u4Count = (i4Argc-MIN_ARGC+1);       
    pu1Buf = (UINT8 *)x_mem_alloc(u4Count);
    if (pu1Buf == NULL)
    {
        return -1;
    }
    x_memset((VOID *)pu1Buf, 0, u4Count);

    //read command line data
    for(u4Idx = 0;u4Idx<u4Count;u4Idx++)
    {
        pu1Buf[u4Idx]= (UINT8)StrToInt(szArgv[u4Idx+(MIN_ARGC-1)]);
    }
    
    // initial SIF sda, scl pin.
    SIFSW_Init(u4SDA, u4SCL);

    pu2Buf = pu1Buf;

    if (u4Count>0)
    {
        u4Count = (u4Count > 256) ? 256:u4Count;
        
        u4ReturnValue = SIFSW_WriteDataNoSubAddr(u4Dev, u4Count, pu1Buf, u2Option, u2Delay,1000);
 
        Printf("u4Dev=0x%X,  u4Count=%d, u2Option=0x%04X, , u2Delay=0x%04X, u4ReturnValue=%d.\n",
               u4Dev,
               u4Count,
               u2Option,
               u2Delay,
               u4ReturnValue);

        if (!u4ReturnValue)
        {
                Printf("Write error !\n");       
                x_mem_free((VOID *)pu2Buf);
                SIFSW_Remove();
                return 0;
        }
    }

    x_mem_free((VOID *)pu2Buf);
    SIFSW_Remove();
#endif
    return 0;
}

//-------------------------------------------------------------------------
/** _SifWriteByteCmd
 *  SIF CLI write byte function.
 *  @param  i4Argc              user's CLI input length.
 *  @param  szArgv              user's CLI input string.
 *  @retval  1  SUCCESS.
 *  @retval  0  FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _SifWriteByteCmd(INT32 i4Argc, const CHAR * *szArgv)
{
#if 1
    UINT8 u1Dev;

    UINT32 u4ClkDiv;
    UINT32 u4Addr;
    UINT32 u4ReturnValue;
    UINT8 u1ByteValue;

    if (i4Argc<5)
    {
        Printf("Arg: u4ClkDiv u1Dev u4Addr u1ByteValue.\n");
        return 0;
    }

    u4ClkDiv = StrToInt(szArgv[1]);
    u1Dev = (UINT8)StrToInt(szArgv[2]);
    u4Addr = StrToInt(szArgv[3]);
    u1ByteValue = (UINT8)(StrToInt(szArgv[4]) & 0xFF);

    u4ReturnValue = SIF_Write(u4ClkDiv, u1Dev, u4Addr, (UINT8 *)&u1ByteValue, 1);

    if (u4ReturnValue == 0xFF)
    {
        Printf("write fail.\n");
    }
    else
    {
        Printf("write successfully! byte count = %d\n", u4ReturnValue);
    }
#endif
    return 0;
}

//-------------------------------------------------------------------------
/** _SifHDCPWriteSramCmd
 *  SIF CLI write HDMI HDCP in system EEPROM function.
 *  @param  i4Argc              user's CLI input length.
 *  @param  szArgv              user's CLI input string.
 *  @retval  1  SUCCESS.
 *  @retval  0  FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _SifHDCPWriteSramCmd(INT32 i4Argc, const CHAR * *szArgv)
{
    INT32 fgSet;
    INT32 i;

    fgSet = EEPHDCP_Write(0, (UINT32)_au1HDCP, SIF_HDCP_SIZE);
    if (fgSet)
    {
        Printf("EEPHDCP_Write fail.\n");
        return 0;
    }

    fgSet = EEPHDCP_Read(0, (UINT32)_au1EEPBuf, SIF_HDCP_SIZE);
    if (fgSet)
    {
        Printf("read eeprom back fail.\n");
        return 0;
    }

    for (i=0; i < SIF_HDCP_SIZE; i++)
    {
        if (_au1HDCP[i] != _au1EEPBuf[i])
        {
            Printf("write fail !\n");
            return 0;
        }
    }
    
    Printf("write successfully !\n");
    
    fgSet = EEPHDCP_Init();
    if (fgSet)
    {
        Printf("EEPHDCP_Init fail.\n");
        return 0;
    }

    Printf("load to HDCP SRAM successfully !\n");
    
    fgSet = EEPHDCP_SetSramDevice(1, 0xA0);
    if (fgSet)
    {
        Printf("EEPHDCP_SetSramDevice fail.\n");
        return 0;
    }
    
    Printf("Open HDCP SRAM device address = 0xA0 !\n");

    return 0;
}
//-------------------------------------------------------------------------
/** _SifHDCPReadCmd
 *  SIF CLI read HDMI HDCP in system EEPROM function.
 *  @param  i4Argc              user's CLI input length.
 *  @param  szArgv              user's CLI input string.
 *  @retval  1  SUCCESS.
 *  @retval  0  FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _SifHDCPReadCmd(INT32 i4Argc, const CHAR * *szArgv)
{
    UINT32 u4Idx;
    UINT32 u4Data[4];    
    INT32 fgSet,i,BKSV=0;

    fgSet = EEPHDCP_Init();
    if (fgSet)
    {
        Printf("EEPHDCP_Init fail.\n");
        return 0;
    }

    // write to hdcp sram pointer.
    IO_WRITE32(BIM_BASE, REG_RW_HDCP_ADDR, 0);
    
    for (u4Idx = 0; u4Idx<SIF_HDCP_SIZE; u4Idx += 16)
    {
        u4Data[0] = IO_READ32(BIM_BASE, REG_RW_HDCP_DATA);
        u4Data[1] = IO_READ32(BIM_BASE, REG_RW_HDCP_DATA);
        u4Data[2] = IO_READ32(BIM_BASE, REG_RW_HDCP_DATA);
        u4Data[3] = IO_READ32(BIM_BASE, REG_RW_HDCP_DATA);        

         Printf("0x%08X : 0x%08X, 0x%08X, 0x%08X, 0x%08X.\n", u4Idx, u4Data[0], u4Data[1], u4Data[2], u4Data[3]);
         /*check HDCP BKSV key*/
         if(u4Idx ==0)
         {
         	u4Data[0]=u4Data[0]>>8;
         	for(i=0;i<24;i++)
         	{
         		if(u4Data[0]&0x1==0x1)
         		{
         			BKSV++;	
         		}
         		u4Data[0]=u4Data[0]>>1;
         		
         	}
         	 for(i=0;i<16;i++)
         	{
         		if(u4Data[1]&0x1==0x1)
         		{
         			BKSV++;
         		}
         		u4Data[1]=u4Data[1]>>1;
         		
    }
         }
    }

	if(BKSV==20)
		printf("HDCP key BKSV is correct\n");
	else
		printf("HDCP key BKSV is wrong\n");

    return 0;
}

//-------------------------------------------------------------------------
/** _SifHDCPWriteByteCmd
 *  SIF CLI SW IIC write byte function.
 *  @param  i4Argc              user's CLI input length.
 *  @param  szArgv              user's CLI input string.
 *  @retval  1  SUCCESS.
 *  @retval  0  FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _SifHDCPWriteByteCmd(INT32 i4Argc, const CHAR * *szArgv)
{
    INT32 fgSet;
    UINT32 u4Addr;
    UINT8 u1ByteValue;
    
    if (i4Argc < 3)
    {
        Printf("Arg: u4Addr u1ByteValue.\n");
        return 0;
    }
    u4Addr = StrToInt(szArgv[1]);    
    u1ByteValue = (UINT8)(StrToInt(szArgv[2]) & 0xFF);    

    if ((u4Addr+1) > SIF_HDCP_SIZE)
    {
        Printf("hdcp size only 320 bytes.\n");
        return 0;    
    }

    fgSet = EEPHDCP_Write((UINT64)u4Addr, (UINT32)&u1ByteValue, 1);
    if (fgSet)
    {
        Printf("write fail.\n");
        return 0;
    }
    Printf("write successfully !\n");
    
    return 0;
}
//-------------------------------------------------------------------------
/** _SifHDCPReadByteCmd
 *  SIF CLI SW IIC read byte function.
 *  @param  i4Argc              user's CLI input length.
 *  @param  szArgv              user's CLI input string.
 *  @retval  1  SUCCESS.
 *  @retval  0  FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _SifHDCPReadByteCmd(INT32 i4Argc, const CHAR * *szArgv)
{
    INT32 fgSet;
    UINT32 u4Addr;
    UINT8 u1ByteValue;

    if (i4Argc < 2)
    {
        Printf("Arg: u4Addr.\n");
        return 0;
    }
    u4Addr = StrToInt(szArgv[1]);    

    if (u4Addr >= SIF_HDCP_SIZE)
    {
        Printf("hdcp size only 320 bytes.\n");
        return 0;    
    }

    fgSet = EEPHDCP_Read((UINT64)u4Addr, (UINT32)&u1ByteValue, 1);
    if (fgSet)
    {
        Printf("read fail.\n");
        return 0;
    }

    // send out data to console.
    SerPollPutChar(u1ByteValue);
    Printf("read successfully !\n");
    return 0;
}

static INT32 _SifHDCPWrAllByteCmd(INT32 i4Argc, const CHAR * *szArgv)
{
    INT32 i, fgSet;
    HAL_TIME_T rOrgTime, rNewTime, rDeltaTime;

    HAL_GetTime(&rOrgTime);
    for (i=0; i<SIF_HDCP_SIZE; i++)
    {
        HAL_GetTime(&rNewTime);
        HAL_GetDeltaTime(&rDeltaTime, &rOrgTime, &rNewTime);
        if (rDeltaTime.u4Seconds > 15)
        {
            SerPollPutChar('F');
            return 1;
        }
        _au1EEPBuf[i] = SerPollGetChar();
    }

    fgSet = EEPHDCP_Write((UINT64)0, (UINT32)_au1EEPBuf, SIF_HDCP_SIZE);
    if (fgSet)
    {
        SerPollPutChar('F');
        return 1;
    }

    SerPollPutChar('S');
    return 0;
}

static INT32 _SifHDCPRdAllByteCmd(INT32 i4Argc, const CHAR * *szArgv)
{
    INT32 i, x, fgSet;

    fgSet = EEPHDCP_Read((UINT64)0, (UINT32)_au1EEPBuf, SIF_HDCP_SIZE);
    if (fgSet)
    {
        x_memset(_au1EEPBuf, 0, SIF_HDCP_SIZE);
    }
    for (i=0; i<SIF_HDCP_SIZE; i++)
    {
        if (_au1EEPBuf[i]==0x18)
        {
            fgSet++;
        }
        if (_au1EEPBuf[i]==0x00)
        {
            fgSet++;
        }
        SerPollPutChar(_au1EEPBuf[i]);
        // delay 1ms.
        for (x=0; x<30000; x++) { }
    }

    return 0;
}

static INT32 _SifHDCPWrRdAllByteCmd(INT32 i4Argc, const CHAR * *szArgv)
{
    INT32 i, fgSet;
    HAL_TIME_T rOrgTime, rNewTime, rDeltaTime;

		UART_SetHdcpTransferFlag(1);
    HAL_GetTime(&rOrgTime);
    for (i=0; i<SIF_HDCP_SIZE; i++)
    {
        HAL_GetTime(&rNewTime);
        HAL_GetDeltaTime(&rDeltaTime, &rOrgTime, &rNewTime);
        if (rDeltaTime.u4Seconds > 15)
        {
            fgSet = 1;
            goto HdcpExit;
        }
        _au1EEPBuf[i] = SerPollGetChar();
    }

    fgSet = EEPHDCP_Write((UINT64)0, (UINT32)_au1EEPBuf, SIF_HDCP_SIZE);
    if (fgSet)
    {
        fgSet = 1;
        goto HdcpExit;
    }

    fgSet = EEPHDCP_Read((UINT64)0, (UINT32)_au1EEPBuf, SIF_HDCP_SIZE);

HdcpExit:
    if (fgSet)
    {
        x_memset(_au1EEPBuf, 0, SIF_HDCP_SIZE);
    }
    for (i=0; i<SIF_HDCP_SIZE; i++)
    {
        SerPollPutChar(_au1EEPBuf[i]);
    }
    
    UART_SetHdcpTransferFlag(0);
    return 0;
}

//-------------------------------------------------------------------------
/** _SifSWWriteEeprom
 *  SIF CLI SW IIC write EEPROM function.
 * @param u4SDA 	GPIO number as SDA.
 * @param u4SCL 	GPIO number as SCL.
 * @param pbData pointer of data to be sent out 
 *  @retval  1  SUCCESS.
 *  @retval  0  FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _SifSWWriteEeprom(UINT32 u4SDA, UINT32 u4SCL, UINT8 *pu1Data)
{
    UINT8 u1Ret = 0;
    INT32 fgSet;
    UINT32 u4Index = 0;  
    UINT32 u4Cnt;   
    UINT32 u4DeviceAddr;   

    UINT32 u4EdidWP, u4EdidWPEnable, u4Flag=0;

    // disable write protect.
    if (0 == DRVCUST_OptQuery(eEdidEepromWPGpio, &u4EdidWP) && 
       (0 == DRVCUST_OptQuery(eEdidEepromWPEnablePolarity, &u4EdidWPEnable)))
    {
        u4Flag = 1;
        fgSet = !(INT32)u4EdidWPEnable;
        VERIFY(GPIO_Output((INT32)u4EdidWP, &fgSet) == fgSet);
    }

    Printf("Write EDID EEPROM SDA = GPIO%d, SCL = GPIO%d.\n", u4SDA, u4SCL);

    // initial SIF sda, scl pin.
    SIFSW_Init(u4SDA, u4SCL);

    u4Index = 0;
    // EEPROM page length = 8 bytes.
    while (u4Index < SIF_EDID_SIZE)
    {
        u4Cnt = ((SIF_EDID_SIZE -  u4Index) > 8) ? 8 : (SIF_EDID_SIZE -  u4Index); 

        // lower device address [b3, b2, b1] are from address [A10, A9, A8].
        u4DeviceAddr = (u4Index >> 7) & 0x0E;        
        u4DeviceAddr |= 0xA0;
        
        u1Ret = SIFSW_WriteData(0xA0, u4Index, u4Cnt, (UINT8 *)&pu1Data[u4Index], 0xFF);

        if (u1Ret == 0)
        {
            Printf("write fail.\n");

            // enable write protect.
            if (u4Flag)
            {
                fgSet = (INT32)u4EdidWPEnable;
                VERIFY(GPIO_Output((INT32)u4EdidWP, &fgSet) == fgSet);
            }

            SIFSW_Remove();
            return 0;
        }
        // Wait EEPROM to program.
        x_thread_delay(20);
        
        u4Index += u4Cnt;
    }

    Printf("Write successfully !\n");

    // enable write protect.
    if (u4Flag)
    {
        fgSet = (INT32)u4EdidWPEnable;
        VERIFY(GPIO_Output((INT32)u4EdidWP, &fgSet) == fgSet);
    }

    SIFSW_Remove();

    return 1;
}

//-------------------------------------------------------------------------
/** _SifEDIDWriteCmd
 *  SIF CLI SW IIC write HDMI EDID EEPROM function.
 *  @param  i4Argc              user's CLI input length.
 *  @param  szArgv              user's CLI input string.
 *  @retval  1  SUCCESS.
 *  @retval  0  FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _SifEDIDWriteCmd(INT32 i4Argc, const CHAR * *szArgv)
{

#if 0                                         //#if (MODEL==SSmt5382pddr2x2)
    Printf("Pinmux &Write 1st EDID EEPROM.\n");  
    Sif_SS_WriteEDIDData(1,(UINT8 *)_au1EDID_CEC_192k_DeepColor12B_P1,256);
//    Sif_SS_WriteEDIDData(1,(UINT8 *)_EDID_TEST_TABLE,256);
    Printf("\nPinmux &Write 2nd EDID EEPROM.\n");
    Sif_SS_WriteEDIDData(1,(UINT8 *)_au1EDID_CEC_192k_DeepColor12B_P2,256);
//    _EDID_TEST_TABLE[0]=0x42;
//    _EDID_TEST_TABLE[255]=0x60;
//    Sif_SS_WriteEDIDData(2,(UINT8 *)_EDID_TEST_TABLE,256);
    Printf("\nPinmux &Write 3rd EDID EEPROM.\n");
    Sif_SS_WriteEDIDData(1,(UINT8 *)_au1EDID_CEC_192k_DeepColor12B_P3,256);
//    _EDID_TEST_TABLE[0]=0x52;
//    _EDID_TEST_TABLE[255]=0x70;
//    Sif_SS_WriteEDIDData(3,(UINT8 *)_EDID_TEST_TABLE,256);

    return 0;    
#else
//#ifdef CC_AUD_DVBT_SUPPORT
#ifdef CC_MT8223  // PCM only

    INT32 dHigh = 1;
    GPIO_Output(10, &dHigh);

    Printf("Write 1st EDID EEPROM.\n");
    vHDMISetSwitch(HDMI_SWITCH_1);
    vUtDelay1ms(20);
    _SifSWWriteEeprom(SIF_EDID1_SDA, SIF_EDID1_SCL, (UINT8 *)_au1EDID_CEC_48k_DeepColor12B_P1);

    Printf("Write 2nd EDID EEPROM.\n");
    vHDMISetSwitch(HDMI_SWITCH_2);
    vUtDelay1ms(20);
    _SifSWWriteEeprom(SIF_EDID2_SDA, SIF_EDID2_SCL, (UINT8 *)_au1EDID_CEC_48k_DeepColor12B_P2);

    vUtDelay1ms(20);
	dHigh = 0;
	GPIO_Output(10, &dHigh);

#else
#if defined(CC_MT5387P1V1)
    INT32 dHigh = 1;
    GPIO_Output(HDMI_DDC_WP, &dHigh);

    Printf("TMDS351, Write 1st EDID EEPROM.\n");
    vHDMISetSwitch(HDMI_SWITCH_1);
    vUtDelay1ms(20);
    _SifSWWriteEeprom(SIF_EDID1_SDA, SIF_EDID1_SCL, (UINT8 *)_au1EDID_CEC_192k_DeepColor12B_P1);

    Printf("\nTMDS351, Write 2nd EDID EEPROM.\n");
    vHDMISetSwitch(HDMI_SWITCH_2);
    vUtDelay1ms(20);
    _SifSWWriteEeprom(SIF_EDID2_SDA, SIF_EDID2_SCL, (UINT8 *)_au1EDID_CEC_192k_DeepColor12B_P2);

    Printf("\nTMDS351, Write 3rd EDID EEPROM.\n");
    vHDMISetSwitch(HDMI_SWITCH_3);
    vUtDelay1ms(20);
    _SifSWWriteEeprom(SIF_EDID3_SDA, SIF_EDID3_SCL, (UINT8 *)_au1EDID_CEC_192k_DeepColor12B_P3);
#else
    //_au1EDID[0x9a] = 0x10; 
    //_au1EDID[0xff] = 0x44;
    Printf("Write 1st EDID EEPROM.\n");    //_SifSWWriteEeprom(SIF_EDID1_SDA, SIF_EDID1_SCL, (UINT8 *)_au1EDID);
    _SifSWWriteEeprom(SIF_EDID1_SDA, SIF_EDID1_SCL, (UINT8 *)_au1EDID_CEC_192k_DeepColor12B_P1);

    //_au1EDID[0x9a] = 0x20; 
    //_au1EDID[0xff] = 0x34;
    Printf("\nWrite 2nd EDID EEPROM.\n");
     //_SifSWWriteEeprom(SIF_EDID2_SDA, SIF_EDID2_SCL, (UINT8 *)_au1EDID);
    _SifSWWriteEeprom(SIF_EDID2_SDA, SIF_EDID2_SCL, (UINT8 *)_au1EDID_CEC_192k_DeepColor12B_P2);

    //_au1EDID[0x9a] = 0x30; 
    //_au1EDID[0xff] = 0x24;
    Printf("\nWrite 3rd EDID EEPROM.\n");
    //_SifSWWriteEeprom(SIF_EDID3_SDA, SIF_EDID3_SCL, (UINT8 *)_au1EDID);
    _SifSWWriteEeprom(SIF_EDID3_SDA, SIF_EDID3_SCL, (UINT8 *)_au1EDID_CEC_192k_DeepColor12B_P3);
#endif	
#endif    
    return 0;
#endif
}
static INT32 _SifEDID1WriteCmd(INT32 i4Argc, const CHAR * *szArgv)
{
#if defined(CC_MT5387P1V1)
    INT32 dHigh = 1;
    GPIO_Output(HDMI_DDC_WP, &dHigh);
    vHDMISetSwitch(HDMI_SWITCH_1);
    vUtDelay1ms(20);
#endif
    Printf("Write 1st EDID EEPROM.\n");
#ifdef CC_MT8223
    _SifSWWriteEeprom(SIF_EDID3_SDA, SIF_EDID3_SCL, (UINT8 *)_au1EDID_CEC_48k_DeepColor12B_P1);
#else
	_SifSWWriteEeprom(SIF_EDID3_SDA, SIF_EDID3_SCL, (UINT8 *)_au1EDID_CEC_192k_DeepColor12B_P1);
#endif

    return 0;
}
static INT32 _SifEDID2WriteCmd(INT32 i4Argc, const CHAR * *szArgv)
{
#if defined(CC_MT5387P1V1)
    INT32 dHigh = 1;
    GPIO_Output(HDMI_DDC_WP, &dHigh);
    vHDMISetSwitch(HDMI_SWITCH_2);
    vUtDelay1ms(20);
#endif

    Printf("\nWrite 2nd EDID EEPROM.\n");
#ifdef CC_MT8223
    _SifSWWriteEeprom(SIF_EDID3_SDA, SIF_EDID3_SCL, (UINT8 *)_au1EDID_CEC_48k_DeepColor12B_P2);
#else
	_SifSWWriteEeprom(SIF_EDID3_SDA, SIF_EDID3_SCL, (UINT8 *)_au1EDID_CEC_192k_DeepColor12B_P2);
#endif

    return 0;
}
static INT32 _SifEDID3WriteCmd(INT32 i4Argc, const CHAR * *szArgv)
{
#if defined(CC_MT5387P1V1)
    INT32 dHigh = 1;
    GPIO_Output(HDMI_DDC_WP, &dHigh);
    vHDMISetSwitch(HDMI_SWITCH_3);
    vUtDelay1ms(20);
#endif

    Printf("\nWrite 3rd EDID EEPROM.\n");
#ifdef CC_MT8223
    _SifSWWriteEeprom(SIF_EDID3_SDA, SIF_EDID3_SCL, (UINT8 *)_au1EDID_CEC_48k_DeepColor12B_P3);
#else
	_SifSWWriteEeprom(SIF_EDID3_SDA, SIF_EDID3_SCL, (UINT8 *)_au1EDID_CEC_192k_DeepColor12B_P3);
#endif
    return 0;
}

/*static _duempBufferBy16( UINT8* u1Buf, UINT32 u4Size)
{
    UINT32 u4Index;
    for( u4Index=0;u4Index<u4Size;u4Index++)
    {
        if( (u4Index %16)==0)
            Printf("\n0x%08X:", u4Index);
        Printf("%02X ", u1Buf[u4Index]);
    }
}*/

//-------------------------------------------------------------------------
/** _SifEDIDReadCmd
 *  SIF CLI SW IIC write HDMI EDID EEPROM function.
 *  @param  i4Argc              user's CLI input length.
 *  @param  szArgv              user's CLI input string.
 *  @retval  1  SUCCESS.
 *  @retval  0  FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _SifEDIDReadCmd(INT32 i4Argc, const CHAR * *szArgv)
{
#if 0                                         //#if (MODEL==SSmt5382pddr2x2)   
    //INT32 i4index;
    UINT8 u1Buf[256];
    Printf("Pinmux &Read 1st EDID EEPROM \n");
    Sif_SS_ReadEDIDData(1,(UINT8 *)u1Buf,256);
    _duempBufferBy16(u1Buf, 256);

    Printf("\nPinmux &Read 2nd EDID EEPROM \n");
    Sif_SS_ReadEDIDData(2,(UINT8 *)u1Buf,256);
    _duempBufferBy16(u1Buf, 256);


    Printf("\nPinmux &Read 3rd EDID EEPROM \n");
    Sif_SS_ReadEDIDData(3,(UINT8 *)u1Buf,256);
    _duempBufferBy16(u1Buf, 256);

    return 0;    
#else

#if defined(CC_MT8223)

vHDMISetSwitch(HDMI_SWITCH_1);
vUtDelay1ms(20);
Printf("Read 1st EDID EEPROM SDA = GPIO%d, SCL = GPIO%d.\n", SIF_EDID1_SDA, SIF_EDID1_SCL);
CLI_Parser_Arg("sif.sr %d %d 0xA0 0 256", SIF_EDID1_SDA, SIF_EDID1_SCL);

vHDMISetSwitch(HDMI_SWITCH_2);
vUtDelay1ms(20);
Printf("\nRead 2nd EDID EEPROM SDA = GPIO%d, SCL = GPIO%d.\n", SIF_EDID2_SDA, SIF_EDID2_SCL);
CLI_Parser_Arg("sif.sr %d %d 0xA0 0 256", SIF_EDID2_SDA, SIF_EDID2_SCL);

#elif defined(CC_MT5387P1V1)

    vHDMISetSwitch(HDMI_SWITCH_1);
    vUtDelay1ms(20);
    Printf("Read 1st EDID EEPROM SDA = GPIO%d, SCL = GPIO%d.\n", SIF_EDID1_SDA, SIF_EDID1_SCL);
    CLI_Parser_Arg("sif.sr %d %d 0xA0 0 256", SIF_EDID1_SDA, SIF_EDID1_SCL);
	
    vHDMISetSwitch(HDMI_SWITCH_2);
    vUtDelay1ms(20);
    Printf("\nRead 2nd EDID EEPROM SDA = GPIO%d, SCL = GPIO%d.\n", SIF_EDID2_SDA, SIF_EDID2_SCL);
    CLI_Parser_Arg("sif.sr %d %d 0xA0 0 256", SIF_EDID2_SDA, SIF_EDID2_SCL);
	
    vHDMISetSwitch(HDMI_SWITCH_3);
    vUtDelay1ms(20);
    Printf("\nRead 3rd EDID EEPROM SDA = GPIO%d, SCL = GPIO%d.\n", SIF_EDID3_SDA, SIF_EDID3_SCL);
    CLI_Parser_Arg("sif.sr %d %d 0xA0 0 256", SIF_EDID3_SDA, SIF_EDID3_SCL);
#else
    Printf("Read 1st EDID EEPROM SDA = GPIO%d, SCL = GPIO%d.\n", SIF_EDID1_SDA, SIF_EDID1_SCL);
    CLI_Parser_Arg("sif.sr %d %d 0xA0 0 256", SIF_EDID1_SDA, SIF_EDID1_SCL);

    Printf("\nRead 2nd EDID EEPROM SDA = GPIO%d, SCL = GPIO%d.\n", SIF_EDID2_SDA, SIF_EDID2_SCL);
    CLI_Parser_Arg("sif.sr %d %d 0xA0 0 256", SIF_EDID2_SDA, SIF_EDID2_SCL);

    Printf("\nRead 3rd EDID EEPROM SDA = GPIO%d, SCL = GPIO%d.\n", SIF_EDID3_SDA, SIF_EDID3_SCL);
    CLI_Parser_Arg("sif.sr %d %d 0xA0 0 256", SIF_EDID3_SDA, SIF_EDID3_SCL);
#endif
    return 0;   
#endif 
}
static INT32 _SifEDID1ReadCmd(INT32 i4Argc, const CHAR * *szArgv)
{
#if defined(CC_MT5387P1V1)
    vHDMISetSwitch(HDMI_SWITCH_1);
    vUtDelay1ms(20);
#endif
    Printf("Read 1st EDID EEPROM SDA = GPIO%d, SCL = GPIO%d.\n", SIF_EDID1_SDA, SIF_EDID1_SCL);
    CLI_Parser_Arg("sif.sr %d %d 0xA0 0 256", SIF_EDID1_SDA, SIF_EDID1_SCL);
    return 0;   
}
static INT32 _SifEDID2ReadCmd(INT32 i4Argc, const CHAR * *szArgv)
{
#if defined(CC_MT5387P1V1)
    vHDMISetSwitch(HDMI_SWITCH_2);
    vUtDelay1ms(20);
#endif
    Printf("\nRead 2nd EDID EEPROM SDA = GPIO%d, SCL = GPIO%d.\n", SIF_EDID2_SDA, SIF_EDID2_SCL);
    CLI_Parser_Arg("sif.sr %d %d 0xA0 0 256", SIF_EDID2_SDA, SIF_EDID2_SCL);
    return 0;   
}
static INT32 _SifEDID3ReadCmd(INT32 i4Argc, const CHAR * *szArgv)
{
#if defined(CC_MT5387P1V1)
    vHDMISetSwitch(HDMI_SWITCH_3);
    vUtDelay1ms(20);
#endif
    Printf("\nRead 3rd EDID EEPROM SDA = GPIO%d, SCL = GPIO%d.\n", SIF_EDID3_SDA, SIF_EDID3_SCL);
    CLI_Parser_Arg("sif.sr %d %d 0xA0 0 256", SIF_EDID3_SDA, SIF_EDID3_SCL);
    return 0;   
}
//-------------------------------------------------------------------------
/** _SifEDIDWriteByteCmd
 *  SIF CLI SW IIC write function.
 *  @param  i4Argc              user's CLI input length.
 *  @param  szArgv              user's CLI input string.
 *  @retval  1  SUCCESS.
 *  @retval  0  FAIL. 
 */
//-------------------------------------------------------------------------
#if 0
static INT32 _SifEDIDWriteByteCmd(INT32 i4Argc, const CHAR * *szArgv)
{
    INT32 fgSet;
    UINT32 u4Addr;
    UINT32 u4ReturnValue;
    UINT8 u1ByteValue;
    INT32 i4EdidWP;
    INT32 i4EdidWPEnable;
    
    if (i4Argc < 3)
    {
        Printf("Arg: u4Addr u1ByteValue.\n");
        return 0;
    }
    u4Addr = StrToInt(szArgv[1]);    
    u1ByteValue = (UINT8)(StrToInt(szArgv[2]) & 0xFF);    

    if ((u4Addr+1) > SIF_EDID_SIZE)
    {
        Printf("edid size only 256 bytes.\n");
        return 0;    
    }

    i4EdidWP = (INT32)DRVCUST_OptGet(eEdidEepromWPGpio);
    i4EdidWPEnable = (INT32)DRVCUST_OptGet(eEdidEepromWPEnablePolarity);

    Printf("disable write protect gpio%d.\n", i4EdidWP);

    // disable write protect.
    fgSet = !i4EdidWPEnable;
    VERIFY(GPIO_Output(i4EdidWP, &fgSet) == fgSet);

    // initial SIF sda, scl pin.
    SIFSW_Init(SIF_EDID_SDA, SIF_EDID_SCL);

    u4ReturnValue = SIFSW_WriteData(SIF_EEP_DEVICE_ADDR, u4Addr, 1, (UINT8 *)&u1ByteValue, 0xFF);

    if (u4ReturnValue == 0)
    {
        Printf("write fail.\n");
    }
    else
    {
        Printf("write successfully !\n");
    }

    // enable write protect.
    fgSet = i4EdidWPEnable;
    VERIFY(GPIO_Output(i4EdidWP, &fgSet) == fgSet);

    SIFSW_Remove();
    return 0;
}

//-------------------------------------------------------------------------
/** _SifEDIDReadByteCmd
 *  SIF CLI SW IIC read function.
 *  @param  i4Argc              user's CLI input length.
 *  @param  szArgv              user's CLI input string.
 *  @retval  1  SUCCESS.
 *  @retval  0  FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _SifEDIDReadByteCmd(INT32 i4Argc, const CHAR * *szArgv)
{
    UINT32 u4Addr;
    UINT32 u4ReturnValue;
    UINT8 u1ByteValue;
    
    if (i4Argc < 2)
    {
        Printf("Arg: u4Addr.\n");
        return 0;
    }
    
    u4Addr = StrToInt(szArgv[1]);    

    if (u4Addr >= SIF_EDID_SIZE)
    {
        Printf("edid size only 256 bytes.\n");
        return 0;    
    }
    // initial SIF sda, scl pin.
    SIFSW_Init(SIF_EDID_SDA, SIF_EDID_SCL);

    u4ReturnValue = SIFSW_ReadData(SIF_EEP_DEVICE_ADDR, u4Addr, 1, (UINT8 *)&u1ByteValue, 0xFF);

    if (u4ReturnValue != 1)
    {
        Printf("read fail.\n");
    }
    else
    {
        // send out data to console.
        SerPollPutChar(u1ByteValue);
        Printf("read successfully !\n");
    }

    SIFSW_Remove();
    return 0;
}
#endif

static INT32 _SifEDIDWrAllByteCmd(INT32 i4Argc, const CHAR * *szArgv)
{
//    UINT32 u4ReturnValue;       
//    INT32 fgSet;
//    UINT32 u4Index = 0;  
//    UINT32 u4Cnt;   
//    UINT32 u4DeviceAddr;   
//    INT32 i4EdidWP;
//    INT32 i4EdidWPEnable;    
    INT32 i;
    HAL_TIME_T rOrgTime, rNewTime, rDeltaTime;

    HAL_GetTime(&rOrgTime);
    for (i=0; i<SIF_EDID_SIZE; i++)
    {
        HAL_GetTime(&rNewTime);
        HAL_GetDeltaTime(&rDeltaTime, &rOrgTime, &rNewTime);
        if (rDeltaTime.u4Seconds > 15)
        {
            SerPollPutChar('F');
            return 1;
        }
        _au1EEPBuf[i] = SerPollGetChar();
    }

    if (0 == _SifSWWriteEeprom(SIF_EDID1_SDA, SIF_EDID1_SCL, _au1EEPBuf))
    {
        Printf("write 1st EDID fail.\n");
        SerPollPutChar('F');
        return 0;
    }

    if (0 == _SifSWWriteEeprom(SIF_EDID2_SDA, SIF_EDID2_SCL, _au1EEPBuf))
    {
        Printf("write 2nd EDID fail.\n");
        SerPollPutChar('F');
        return 0;
    }

    if (0 == _SifSWWriteEeprom(SIF_EDID3_SDA, SIF_EDID3_SCL, _au1EEPBuf))
    {
        Printf("write 3rd EDID fail.\n");
        SerPollPutChar('F');
        return 0;
    }

    Printf("write successfully !\n");
    SerPollPutChar('S');    
    
    return 0;
}

static INT32 _SifEDIDRdAllByteCmd(INT32 i4Argc, const CHAR * *szArgv)
{
    UINT32 u4Dev;
    UINT32 u4SDA;
    UINT32 u4SCL;    
    UINT32 u4Addr;
    UINT32 u4Count;
    UINT32 u4ReturnValue;
    INT32 i, x;

    u4SDA = SIF_EDID1_SDA;
    u4SCL = SIF_EDID1_SCL;
    u4Dev = 0xa0;
    u4Addr = 0;
    u4Count = 256;

    SIFSW_Init(u4SDA, u4SCL);

    u4ReturnValue = SIFSW_ReadData(u4Dev, u4Addr, u4Count, _au1EEPBuf, 0xFF);
 
    SIFSW_Remove();

    for (i=0; i<SIF_EDID_SIZE; i++)
    {
        if (!u4ReturnValue)
        {
            SerPollPutChar(0xFF);
        }
        else
        {
            SerPollPutChar(_au1EEPBuf[i]);
        }
        // delay 1ms.
        for (x=0; x<30000; x++) { }
    }

    return 0;
}

static INT32 _SifTunerReadNoSubCmd(INT32 i4Argc, const CHAR  * *szArgv)
{
#define	ARGC_BASE	0
    UINT8 u1Dev;
    UINT32 u4Idx;
    UINT32 u4ClkDiv;
//  UINT32 u4Addr;
    UINT32 u4Count;
    UINT8 *pu1Buf;
    UINT8 *pu2Buf;
    INT32 i4ReturnValue;

    if (i4Argc<4)
    {
        Printf("Arg: u4ClkDiv u1Dev  u4Count\n");
        return 0;
    }

    u4ClkDiv = StrToInt(szArgv[ARGC_BASE+1]);
    u1Dev = (UINT8)StrToInt(szArgv[ARGC_BASE+2]);
    u4Count = StrToInt(szArgv[ARGC_BASE+3]);
    pu1Buf = (UINT8 *)x_mem_alloc(u4Count);
    if (pu1Buf == NULL)
    {
        return -1;
    }
    x_memset((VOID *)pu1Buf, 0, u4Count);

    pu2Buf = pu1Buf;

    i4ReturnValue = SIF_TunerReadNoSubAddr(u4ClkDiv, u1Dev, pu1Buf, u4Count);

    Printf("u1Dev=0x%X,  u4Count=%d, i4ReturnValue=%d.\n",
               u1Dev,
               u4Count,
               i4ReturnValue);

     if (i4ReturnValue>0)
     {
            // print sif read data content.
            for (u4Idx = 0; u4Idx<i4ReturnValue; u4Idx ++)
            {
                if (!(u4Idx % 8))
                {
                    Printf("\n0x%08X :", u4Idx);                    
                }
                
                Printf(" 0x%02X,", pu1Buf[u4Idx]);
            }
            Printf("\n");
	} 
        else
        {
            // fail.
            Printf("sif read fail.\n");
         }
 
    x_mem_free((VOID *)pu2Buf);

    return 0;
}

static INT32 _SifTunerWriteNoSubCmd(INT32 i4Argc, const char * *szArgv)
{
    UINT8 u1Dev;

    UINT32 u4ClkDiv;
//    UINT32 u4Addr;
    UINT32 u4Count;
    INT32 i4ReturnValue;
    UINT8 *pu1Buf;
    UINT8 *pu2Buf;
    int i;

    if (i4Argc<5)
    {
        Printf("Arg: u4ClkDiv u1Dev u4Count  [Data] [Data] [Data] ...\n");
        return 0;
    }

    u4ClkDiv = StrToInt(szArgv[1]);
    u1Dev = (UINT8)StrToInt(szArgv[2]);
    u4Count = StrToInt(szArgv[3]);
    pu1Buf = (UINT8 *) x_mem_alloc( u4Count ) ;
    if( pu1Buf == NULL )
    				return -1;    
    
    for(  i=0 ; i< u4Count ; i++)
		pu1Buf[i] = StrToInt( szArgv[i+4] );    
    
    pu2Buf = pu1Buf;
    i4ReturnValue = SIF_TunerWriteNoSubAddr(u4ClkDiv, u1Dev, (UINT8 *)pu1Buf, u4Count);

    Printf("u1Dev=0x%X,  u4Count=%d, i4ReturnValue=%d.\n",
               u1Dev,
               u4Count,
               i4ReturnValue);

    if (i4ReturnValue > 0)
    {
        Printf("write successfully! byte count = %d\n", i4ReturnValue);
    }
    else
    {
        Printf("write fail.\n");
    }

    x_mem_free(pu2Buf);
    return 0;
}

static INT32 _SifReadMultiCmd(INT32 i4Argc, const CHAR  * *szArgv)
{
#define	ARGC_BASE	0
    UINT8 u1Dev;
    UINT32 u4Idx;
    UINT32 u4ClkDiv;
    UINT32 u4Addr;
    UINT32 u4Count;
    UINT8 *pu1Buf;
    UINT8 *pu2Buf;
    INT32 i4ReturnValue;
    UINT8 u1AddrNum;
    
    if (i4Argc<6)
    {
        Printf("Arg: u4ClkDiv u1Dev u1AddrNum u4Addr u4Count\n");
        return 0;
    }

    u4ClkDiv = StrToInt(szArgv[ARGC_BASE+1]);
    u1Dev = (UINT8)StrToInt(szArgv[ARGC_BASE+2]);
    u1AddrNum = StrToInt(szArgv[ARGC_BASE+3]);
    u4Addr = StrToInt(szArgv[ARGC_BASE+4]);
    u4Count = StrToInt(szArgv[ARGC_BASE+5]);
    pu1Buf = (UINT8 *)x_mem_alloc(u4Count);
    if (pu1Buf == NULL)
    {
        return -1;
    }
    x_memset((VOID *)pu1Buf, 0, u4Count);

    pu2Buf = pu1Buf;

    i4ReturnValue = SIF_ReadMultipleSubAddr(u4ClkDiv, u1Dev, u1AddrNum , u4Addr, pu1Buf, u4Count);

    Printf("u1Dev=0x%X, u4Addr=0x%X, u4Count=%d, i4ReturnValue=%d.\n",
               u1Dev,
               u4Addr,
               u4Count,
               i4ReturnValue);

    if (i4ReturnValue>0)
    {
         // print sif read data content.
	for (u4Idx = 0; u4Idx<i4ReturnValue; u4Idx ++)
	{
                if (!(u4Idx % 8))
                {
                    Printf("\n0x%08X :", u4Idx);                    
                }
                
                Printf(" 0x%02X,", pu1Buf[u4Idx]);
	}
	Printf("\n");
    }
    else
    {
            // fail.
            Printf("sif read fail.\n");
    }


    x_mem_free((VOID *)pu2Buf);

    return 0;
}

static INT32 _SifWriteMultiCmd(INT32 i4Argc, const CHAR  * *szArgv)
{
#define	ARGC_BASE	0
    UINT8  u1Dev;
//    UINT32 u4Idx;
    UINT32 u4ClkDiv;
    UINT32 u4Addr;
    UINT32 u4Count;
    UINT8 *pu1Buf;
    UINT8 *pu2Buf;
    INT32 i4ReturnValue;
    UINT8 u1AddrNum;
    int i;
    
    if (i4Argc<7)
    {
        Printf("Arg: u4ClkDiv u1Dev u1AddrNum u4Addr u4Count [Data] [Data] [Data]...\n");
        return 0;
    }

    u4ClkDiv = StrToInt(szArgv[ARGC_BASE+1]);
    u1Dev = (UINT8)StrToInt(szArgv[ARGC_BASE+2]);
    u1AddrNum = StrToInt(szArgv[ARGC_BASE+3]);
    u4Addr = StrToInt(szArgv[ARGC_BASE+4]);
    u4Count = StrToInt(szArgv[ARGC_BASE+5]);
    pu1Buf = (UINT8 *)x_mem_alloc(u4Count);

    x_memset((VOID *)pu1Buf, 0, u4Count);

    pu2Buf = pu1Buf;
    
    if (pu1Buf == NULL)
    {
        return -1;
    }

    for(  i=0 ; i< u4Count ; i++)
		pu1Buf[i] = StrToInt( szArgv[i+6] );
    
    i4ReturnValue = SIF_WriteMultipleSubAddr(u4ClkDiv, u1Dev, u1AddrNum ,u4Addr, (UINT8 *)pu1Buf, u4Count);

    if (i4ReturnValue > 0)
    {
        Printf("write successfully! byte count = %d\n", i4ReturnValue);
    }
    else if (i4ReturnValue==0)
    {
        Printf("Warning: IOCTL should never returns ZERO.\n");
    }
    else
    {
        Printf("write fail.\n");
    }

    x_mem_free((VOID *)pu2Buf);

    return 0;
}

static INT32 _SifRead_X_Cmd(INT32 i4Argc, const CHAR  * *szArgv)
{
#define	ARGC_BASE	0
    UINT8 u1Dev;
    UINT32 u4Idx;
    UINT32 u4ClkDiv;
    UINT32 u4Addr;
    UINT32 u4Count;
    UINT8 *pu1Buf;
    UINT8 *pu2Buf;
    INT32 i4ReturnValue;
    UINT8 u1AddrNum;
    UINT8 u1ChannelId;
    
    if (i4Argc<7)
    {
        Printf("Arg: u1ChannelId, u4ClkDiv u1Dev u1AddrNum u4Addr u4Count\n");
        return 0;
    }

    u1ChannelId = StrToInt(szArgv[ARGC_BASE+1]);
    u4ClkDiv = StrToInt(szArgv[ARGC_BASE+2]);
    u1Dev = (UINT8)StrToInt(szArgv[ARGC_BASE+3]);
    u1AddrNum = StrToInt(szArgv[ARGC_BASE+4]);
    u4Addr = StrToInt(szArgv[ARGC_BASE+5]);
    u4Count = StrToInt(szArgv[ARGC_BASE+6]);
    pu1Buf = (UINT8 *)x_mem_alloc(u4Count);
    if (pu1Buf == NULL)
    {
        return -1;
    }
    x_memset((VOID *)pu1Buf, 0, u4Count);

    pu2Buf = pu1Buf;

    i4ReturnValue = SIF_X_Read(u1ChannelId,u4ClkDiv, u1Dev, u1AddrNum , u4Addr, pu1Buf, u4Count);

    Printf("u1ChannelId = %X, u1Dev=0x%X, u4Addr=0x%X, u4Count=%d, i4ReturnValue=%d.\n",
		u1ChannelId,
               u1Dev,
               u4Addr,
               u4Count,
               i4ReturnValue);

    if (i4ReturnValue>0)
    {
         // print sif read data content.
	for (u4Idx = 0; u4Idx<i4ReturnValue; u4Idx ++)
	{
                if (!(u4Idx % 8))
                {
                    Printf("\n0x%08X :", u4Idx);                    
                }
                
                Printf(" 0x%02X,", pu1Buf[u4Idx]);
	}
	Printf("\n");
    }
    else
    {
            // fail.
            Printf("sif read fail.\n");
    }


    x_mem_free((VOID *)pu2Buf);

    return 0;
}

static INT32 _SifWrite_X_Cmd(INT32 i4Argc, const CHAR  * *szArgv)
{
#define	ARGC_BASE	0
    UINT8  u1Dev;
//  UINT32 u4Idx;
    UINT32 u4ClkDiv;
    UINT32 u4Addr;
    UINT32 u4Count;
    UINT8 *pu1Buf;
    UINT8 *pu2Buf;
    INT32 i4ReturnValue;
    UINT8 u1AddrNum;
    UINT8 u1ChannelId;    
    int i;
    
    if (i4Argc<8)
    {
        Printf("Arg: u1ChannelId u4ClkDiv u1Dev u1AddrNum u4Addr u4Count [Data] [Data] [Data]...\n");
        return 0;
    }
    u1ChannelId = StrToInt(szArgv[ARGC_BASE+1]);
    u4ClkDiv = StrToInt(szArgv[ARGC_BASE+2]);
    u1Dev = (UINT8)StrToInt(szArgv[ARGC_BASE+3]);
    u1AddrNum = StrToInt(szArgv[ARGC_BASE+4]);
    u4Addr = StrToInt(szArgv[ARGC_BASE+5]);
    u4Count = StrToInt(szArgv[ARGC_BASE+6]);
    pu1Buf = (UINT8 *)x_mem_alloc(u4Count);
    if (pu1Buf == NULL)
    {
        return -1;
    }

    for(  i=0 ; i< u4Count ; i++)
		pu1Buf[i] = StrToInt( szArgv[i+7] );

    pu2Buf = pu1Buf ;    
    i4ReturnValue = SIF_X_Write(u1ChannelId, u4ClkDiv, u1Dev, u1AddrNum ,u4Addr, (UINT8 *)pu1Buf, u4Count);

    if (i4ReturnValue > 0)
    {
        Printf("write successfully! byte count = %d\n", i4ReturnValue);
    }
    else if (i4ReturnValue==0)
    {
        Printf("Warning: IOCTL should never returns ZERO.\n");
    }
    else
    {
        Printf("write fail.\n");
    }

    x_mem_free( (VOID *)pu2Buf );
    return 0;
}

static INT32 _EDIDReadByteCmd(INT32 i4Argc, const CHAR * *szArgv)
{
    UINT8 u1EDID_ID;
    UINT8 u1SubAddr;
//    UINT8 *pbBuf;
    INT32 i4Ret;
    UINT8 u1Tmp = 0;

    if (i4Argc<3)
    {
        Printf("Arg: u1EDID_ID u1SubAddr \n");
        return 0;
    }

    u1EDID_ID = StrToInt(szArgv[1]);
    u1SubAddr = StrToInt(szArgv[2]);    
//    pbBuf = (UINT8 *)x_mem_alloc(1);
/*
    if (pbBuf == NULL)
    {
        return 0;
    }*/
//    x_memset((VOID *)pbBuf, 0xA5, 1);
    
    // initial SIF sda, scl pin.
    i4Ret = SifEDIDReadByte( u1EDID_ID, u1SubAddr, &u1Tmp );
         Printf("u1EDID_ID=0x%X, u1SubAddr=0x%X, u1Tmp=0x%X, i4Ret=%d.\n",
               u1EDID_ID,
               u1SubAddr,
               u1Tmp,
               i4Ret);

        if (!i4Ret)
        {
                Printf("Read error !\n");       
        }
     
//    x_mem_free((VOID *)pbBuf);
    return 0;
}



//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


LINT_RESTORE

