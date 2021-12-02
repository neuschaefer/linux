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
 * $RCSfile: nptv_di_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/
#include "x_os.h"
#include "x_stl_lib.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_assert.h"
#include "x_debug.h"
#include "x_mid.h"
#include "source_table.h"
#include "sv_const.h"
#include "video_def.h"

#ifdef CC_SCPOS_3DTV_SUPPORT
#include "drv_tdtv_drvif.h"
#ifdef CC_MT5396
#include "drv_mjc.h"
#endif
#endif
#include "drv_ttd.h"

#ifdef CC_SCPOS_3DTV_SUPPORT

static INT32 _TdtvSet3DGlassesDelay(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvSetLRSwitch(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvTv3dPack(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvTv3d22D(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvFPR(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvQuary(INT32 i4Argc, const CHAR **szArgv);

#endif
#if 1
static INT32 _i4TTDSetFwMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4TTDSetConvDepth(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4TTDSetOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4TTDSetMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4TTDSetPicSize(INT32 i4Argc, const CHAR **szArgv);
#endif

/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for DI command entry
CLI_EXEC_T arTDTVCmdTbl[] = {
#ifdef CC_SCPOS_3DTV_SUPPORT
    {"pk"           ,"pk"         ,_TdtvTv3dPack          ,NULL, "3D packing Mode"           , CLI_SUPERVISOR},
    {"322"          ,"322"        ,_TdtvTv3d22D           ,NULL, "3D to 2D"                  , CLI_SUPERVISOR},
    {"FPR Mode"     ,"fpr"        ,_TdtvFPR               ,NULL, "FPR Mode"                  , CLI_SUPERVISOR},       
    {"Config Query" ,"q"          ,_TdtvQuary             ,NULL, "Config Query"              , CLI_SUPERVISOR},           
    {"SG Delay"     ,"sgd"        ,_TdtvSet3DGlassesDelay ,NULL, "Set Shutter Glasses Delay" , CLI_GUEST},
    {"L/R switch"   ,"lrs"        ,_TdtvSetLRSwitch       ,NULL, "Set L/R Indicator switch"  , CLI_GUEST},
#endif
#if 1
    {"Set TTD Adap FW", "ttdfw", _i4TTDSetFwMode,  NULL, "Set TTD Adap FW",             CLI_GUEST},
    {"set TTD UI Depth","ttdcd", _i4TTDSetConvDepth, NULL, "Set TTD UI Depth",            CLI_GUEST},
    {"Set TTD On/Off",  "ttden", _i4TTDSetOnOff,   NULL, "Set TTD On/Off Control",      CLI_GUEST},
    {"Set TTD Mode",    "ttddm", _i4TTDSetMode,    NULL, "Set TTD Mode Control",        CLI_GUEST},
    {"Set TTD Size",    "ttdsz", _i4TTDSetPicSize, NULL, "Set TTD Picture Size",        CLI_GUEST},
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

/******************************************************************************
* Local Function
******************************************************************************/
#ifdef CC_SCPOS_3DTV_SUPPORT

static INT32 _TdtvTv3dPack(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Pack;
    if (i4Argc < 2)
    {
        Printf("Set Packing Mode\n");
        Printf("E_TDTV_UI_3D_MODE_OFF  =0\n");
        Printf("E_TDTV_UI_3D_MODE_AUTO =1\n");
        Printf("E_TDTV_UI_3D_MODE_TTD  =2\n");
        Printf("E_TDTV_UI_3D_MODE_FS   =3\n");
        Printf("E_TDTV_UI_3D_MODE_SBS  =4\n");         
        Printf("E_TDTV_UI_3D_MODE_TB   =5\n");
        Printf("E_TDTV_UI_3D_MODE_RD   =6\n");
        Printf("E_TDTV_UI_3D_MODE_SS   =7\n"); 
        Printf("E_TDTV_UI_3D_MODE_LI   =8\n");
        Printf("E_TDTV_UI_3D_MODE_DA   =9\n");         
        Printf("E_TDTV_UI_3D_MODE_CB   =10\n");
        return 0;
    }
    u4Pack=StrToInt(szArgv[1]);
    vDrvTDTVPackingDbg((E_TDTV_UI_3D_PACKING)u4Pack);

    return 0;
}


static INT32 _TdtvTv3d22D(INT32 i4Argc, const CHAR **szArgv)
{
    
    UINT32 u4322;
    if (i4Argc < 2)
    {
        Printf("E_TDTV_UI_3D_2_2D_OFF  =0\n");
        Printf("E_TDTV_UI_3D_2_2D_ON   =1\n");
        return 0;
    }
    u4322=StrToInt(szArgv[1]);
    vDrvTDTV3D22Ddbg((E_TDTV_UI_3D_2_2D)u4322);
    return 0;
}

static INT32 _TdtvFPR(INT32 i4Argc, const CHAR **szArgv)
{    
    UINT32 utFPR;
    if (i4Argc < 2)
    {
        Printf("E_TDTV_UI_3D_FPR_OFF  =0");
        Printf("E_TDTV_UI_3D_FPR_V1   =1");
        Printf("E_TDTV_UI_3D_FPR_V2   =2");        
        return 0;
    }
    utFPR=StrToInt(szArgv[1]);    
    vDrvTDTV3DFPRdbg((E_TDTV_UI_3D_FPR)utFPR);
    return 0;
}


static INT32 _TdtvSet3DGlassesDelay(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Count;

	if (i4Argc != 2)
	{
		Printf("Usage: %s <2uTimes>\n", szArgv[0]);
		Printf("2uTimes = 100 => 200us => 27 line\n");
		return 1;
	}

    u4Count = StrToInt(szArgv[1]);
    //u2Delay2usTimes = u4Count;

	return 0;
}

static INT32 _TdtvSetLRSwitch(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Count;

	if (i4Argc != 2)
	{
		Printf("Usage: %s <0: Off, 1: On>\n", szArgv[0]);
		return 1;
	}

    u4Count = StrToInt(szArgv[1]);
    //u1LRSwitch = u4Count;

	return 0;
}

static INT32 _TdtvQuary(INT32 i4Argc, const CHAR **szArgv)
{
    vDrvTDTVQueryConfig();
    return 0;
}

#endif

#if 1
static INT32 _i4TTDSetConvDepth(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 2)
    {
        Printf("Usage: %s convergenceDepth\n", szArgv[0]);
        return 1;
    }

    vDrvTTDSetConvergenceDepthUI(StrToInt(szArgv[1]));

    return 0;
}

static INT32 _i4TTDSetFwMode(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 2)
    {
        Printf("Usage: %s OnOff\n", szArgv[0]);
        Printf("<OnOff>\n");
        Printf("0: Off\n");
        Printf("1: On\n");
        return 1;
    }

    vDrvTTDSetFWOnOff(StrToInt(szArgv[1]));
    return 0;
}

static INT32 _i4TTDSetOnOff(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1OnOff;
	
    if (i4Argc != 2)
    {
        Printf("Usage: %s OnOff\n", szArgv[0]);
        Printf("<OnOff>\n");
        Printf("0: Off\n");
        Printf("1: On\n");
        return 1;
    }

    u1OnOff = (UINT8)(StrToInt(szArgv[1]));
	u1OnOff = (u1OnOff > 0) ? 1 : 0;

    vDrvTTDSetEnable(u1OnOff);
    return 0;
}

static INT32 _i4TTDSetMode(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Mode;

    if (i4Argc != 2)
    {
        Printf("Usage: %s Mode\n", szArgv[0]);
        Printf("<Mode>\n");
        Printf("0: SCL_2D_TO_PR (MT5389, MT5368MP, MT5368ES)\n");
        Printf("1: SCL_2D_TO_FS (MT5389, MT5368MP)\n");
        Printf("2: SCL_PR_TO_PR (MT5389, MT5368MP)\n");
        Printf("3: MJC_2D_TO_FS (MT5396ES, MT5396MP)\n");
        Printf("4: MJC_FS_TO_FS (MT5396ES, MT5396MP)\n");
        return 1;
    }

    u1Mode = (UINT8)(StrToInt(szArgv[1]));
	u1Mode = (u1Mode > 4) ? 4 : u1Mode;

    vDrvTTDSetMode(u1Mode);
    return 0;
}

static INT32 _i4TTDSetPicSize(INT32 i4Argc, const CHAR **szArgv)
{
	UINT16 u2Width, u2Height;

    if (i4Argc != 3)
    {
        Printf("Usage: %s PicSize <Width> <Height>\n", szArgv[0]);
        return 1;
    }

    u2Width  = (UINT16)(StrToInt(szArgv[1]));
    u2Height = (UINT16)(StrToInt(szArgv[2]));

    vDrvTTDSetPicSize(u2Width, u2Height);
    return 0;
}
#endif

