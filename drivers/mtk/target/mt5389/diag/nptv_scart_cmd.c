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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: nptv_scart_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/

#include "x_os.h"
#include "x_stl_lib.h"
#include "video_def.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_assert.h"
#include "x_pinmux.h"
#include "x_debug.h"

#include "source_table.h"
#include "sv_const.h"
#include "drv_scart.h" /* for SCART_COLOR_x macro */
#include "drv_adcset.h" 

#ifndef CC_CLI
#define CC_CLI
#endif

#ifdef CC_CLI

/* Functions for SCART */
static INT32 _ScartInit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScartGetMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4ScartMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScartGetFsSrvAdc(INT32 i4Argc, const CHAR **szArgv);

#endif /*#ifdef CC_CLI*/
/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for SCART command entry
CLI_EXEC_T arScartCmdTbl[] = {
#ifdef CC_CLI
  {"mode",	"mode",	_i4ScartMode,	NULL,	"SCART mode selet",			CLI_SUPERVISOR},
  {"ScartInit", "i", _ScartInit, NULL, "Scart VFE init", CLI_SUPERVISOR},
  {"ScartGetMode", "gm", _ScartGetMode, NULL, "SCART GetMode", CLI_SUPERVISOR},
  {"ScartGetFsSrvAdc", "gfs", _ScartGetFsSrvAdc, NULL, "SCART ServoAd status", CLI_SUPERVISOR},
#endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/


static INT32 _ScartInit(INT32 i4Argc, const CHAR **szArgv)
{
	vScartVfeConnect();
	return 0;
}

static INT32 _ScartGetMode(INT32 i4Argc, const CHAR **szArgv)
{
	switch(bDrvGetScartInputMode())
	{
		case SCART_COLOR_CVBS:
			Printf("CVBS\n");
			break;
		case SCART_COLOR_SV:
			Printf("SV\n");
			break;
		case SCART_COLOR_RGB:
			Printf("RGB\n");
			break;
		case SCART_COLOR_MIX:
			Printf("MIX\n");
			break;
	}
	return 0;
}

void vScartUIChangeInputMode(UINT8 bScartMode);
static INT32 _i4ScartMode(INT32 i4Argc, const CHAR **szArgv)
{

	UINT8  mode=StrToInt(szArgv[1]);
	
	
	Printf("0:Auto 1:CVBS+RGB 2:CVBS 3:RGB 4:SV\n");
	
	if(i4Argc==2)
	{
		if (mode>4)
		{	
			mode=0;
		}	
		Printf("Setting SCART mode to %d\n",mode);
		vScartUIChangeInputMode(mode);
	}
	return 0;
}

extern void vScartReadFsSrvAdc(void);
static INT32 _ScartGetFsSrvAdc(INT32 i4Argc, const CHAR **szArgv)
{

	vScartReadFsSrvAdc();

	return 0;
}

#endif /*#ifdef CC_CLI*/

