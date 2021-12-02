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
 * $RCSfile: mpv_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of DV.
 *---------------------------------------------------------------------------*/

/** @file mpv_cmd.c
 *  CLI command implementation of MPV module.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mpv_drvif.h"
#include "mpv_if.h"
#include "vdec_drvif.h"
#include "vdec_if.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

//for emulation
#ifdef CC_DRIVER_DEMO
#include "drv_t32.h"
#endif

#define DEFINE_IS_LOG	CLI_IsLog
#include "x_debug.h"

#include "x_printf.h"
#include "x_stl_lib.h"


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

CLIMOD_DEBUG_FUNCTIONS(MPV)


//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

static INT32 _MpvEcCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvDbkRegionCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvDbkCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvDbkQueryCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvDbkSetCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvMcModeCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvReduceRefCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvInitCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvTermintCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvResetCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvPlayCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvStopCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvPauseCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvSyncStcCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvSetECCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvQueryCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvDbkThrsdCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvDbkPara1Cmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvDbkPara2Cmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvClrRRDbk (INT32 i4Argc, const CHAR ** szArgv);
#ifdef CC_BOOT_VDEC_LOGO
static INT32 _MpvShowLogo (INT32 i4Argc, const CHAR ** szArgv);
#endif

#ifdef MPV_VIRTUAL_PRS
static INT32 _MpvVirtualPrsStart(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvVirtualPrsStop(INT32 i4Argc, const CHAR ** szArgv);
#endif

#ifdef MPV_SEMI_HOST
static INT32 _MpvSetSaveNo (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvSetSaveFilePath (INT32 i4Argc, const CHAR ** szArgv);
#endif

#ifdef MPV_STATISTICS
static INT32 _MpvResetTime (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MpvPrintTime (INT32 i4Argc, const CHAR ** szArgv);
#endif

#ifdef MPV_138184
static INT32 _MpvTestCmd (INT32 i4Argc, const CHAR ** szArgv);
#endif

#ifdef MPV_VIRTUAL_CC
static INT32 _MpvVirtualCcCmd (INT32 i4Argc, const CHAR ** szArgv);
#endif

#if defined(__linux__)
static INT32 _MpvFlushCmd (INT32 i4Argc, const CHAR ** szArgv);
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

#ifdef MPV_VIRTUAL_PRS
static CLI_EXEC_T _arMpvVirtualPrsCmdTbl[] =
{
 	{"start",		NULL,	_MpvVirtualPrsStart, NULL, "start", CLI_SUPERVISOR},
 	{"stop",		NULL,	_MpvVirtualPrsStop, NULL,	"stop", CLI_SUPERVISOR},
 	{NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif


static CLI_EXEC_T arMpvCmdTbl[] = {
#ifdef MPV_138184
 	{
		"138184",				//pszCmdStr
		"138184",
		_MpvTestCmd,		//execution function
		NULL,
		"13818-4 Test",
		CLI_SUPERVISOR
	},
#endif

 	{
		"dbkthrsd",				//pszCmdStr
		"dbkt",
		_MpvDbkThrsdCmd,		//execution function
		NULL,
		"DBK Test",
		CLI_SUPERVISOR
	},

 	{
		"dbkp1",				//pszCmdStr
		"dbkp1",
		_MpvDbkPara1Cmd,		//execution function
		NULL,
		"DBK Para1",
		CLI_SUPERVISOR
	},

 	{
		"dbkp2",				//pszCmdStr
		"dbkp2",
		_MpvDbkPara2Cmd,		//execution function
		NULL,
		"DBK Para2",
		CLI_SUPERVISOR
	},


 	{
		"ecs",				//pszCmdStr
		"ecs",
		_MpvEcCmd,		//execution function
		NULL,
		"EC mode",
		CLI_SUPERVISOR
	},


 	{
		"dbkr",				//pszCmdStr
		"dbkr",
		_MpvDbkRegionCmd,		//execution function
		NULL,
		"DBK Interest Region",
		CLI_SUPERVISOR
	},


 	{
		"dbk",				//pszCmdStr
		"dbk",
		_MpvDbkCmd,		//execution function
		NULL,
		"DBK Mode",
		CLI_SUPERVISOR
	},

 	{
		"dbkq",				//pszCmdStr
		"dbkquery",
		_MpvDbkQueryCmd,		//execution function
		NULL,
		"DBK Query",
		CLI_SUPERVISOR
	},

 	{
		"dbks",				//pszCmdStr
		"dbkset",
		_MpvDbkSetCmd,		//execution function
		NULL,
		"DBK Setting",
		CLI_SUPERVISOR
	},

 	{
		"mcmode",				//pszCmdStr
		"mm",
		_MpvMcModeCmd,		//execution function
		NULL,
		"MC Mode",
		CLI_SUPERVISOR
	},


 	{
		"rr",				//pszCmdStr
		"rr",
		_MpvReduceRefCmd,		//execution function
		NULL,
		"mpv reduce-reference Mode",
		CLI_SUPERVISOR
	},

 	{
		"init",
		"i",
		_MpvInitCmd,
		NULL,
		"Init : Create Thread, sema, INT",
		CLI_SUPERVISOR
	},

 	{
		"termint",
		"t",
		_MpvTermintCmd,
		NULL,
		"Termint : Terminate Thread, sema, INT",
		CLI_SUPERVISOR
	},

 	{
		"reset",
		"r",
		_MpvResetCmd,
		NULL,
		"Module reset",
		CLI_SUPERVISOR
	},

 	{
		"play",
		"p",
		_MpvPlayCmd,
		NULL,
		"Start to decode",
		CLI_SUPERVISOR
	},

	{
		"stop",
		"s",
		_MpvStopCmd,
		NULL,
		"Stop decoding",
		CLI_SUPERVISOR
	},

	{
		"pause",
		"ps",
		_MpvPauseCmd,
		NULL,
		"Pause decoding",
		CLI_SUPERVISOR
	},

	{
		"sync",
		"sy",
		_MpvSyncStcCmd,
		NULL,
		"Sync STC",
		CLI_SUPERVISOR
	},

	{
		"ec",
		"ec",
		_MpvSetECCmd,
		NULL,
		"Set EC",
		CLI_SUPERVISOR
	},

	{
		"query",
		"q",
		_MpvQueryCmd,
		NULL,
		"Query Status",
		CLI_SUPERVISOR
	},

	{
		"clearrrdbk",
		"clr",
		_MpvClrRRDbk,
		NULL,
		"Clear RR Dbk",
		CLI_SUPERVISOR
	},

#ifdef CC_BOOT_VDEC_LOGO
	{
		"showlogo",
		"logo",
		_MpvShowLogo,
		NULL,
		"Show Vdec logo",
		CLI_SUPERVISOR
	},
#endif

	#ifdef MPV_VIRTUAL_PRS
 	{"vp",			NULL,	NULL, _arMpvVirtualPrsCmdTbl, "Virtual Parser", CLI_SUPERVISOR},
       #endif

       #ifdef MPV_SEMI_HOST
	{
		"save",
		"sv",
		_MpvSetSaveNo,
		NULL,
		"Set StartSaveNo EndSaveNo",
		CLI_SUPERVISOR
	},

	{
		"savefilepath",
		"sf",
		_MpvSetSaveFilePath,
		NULL,
		"Set Save File Path",
		CLI_SUPERVISOR
	},

       #endif

       #ifdef MPV_STATISTICS
	{
		"retime",
		"rt",
		_MpvResetTime,
		NULL,
		"Reset Decoding Time Measurement",
		CLI_SUPERVISOR
	},

	{
		"prtime",
		"pt",
		_MpvPrintTime,
		NULL,
		"Print Decoding Time",
		CLI_SUPERVISOR
	},

       #endif

        #ifdef MPV_VIRTUAL_CC
        {
		"vc",				//pszCmdStr
		"vc",
		_MpvVirtualCcCmd,		//execution function
		NULL,
		"mpv Virtual Closed Caption Mode",
		CLI_SUPERVISOR
        },
        #endif

#if defined(__linux__)
	{
		"flush",
		"f",
		_MpvFlushCmd,
		NULL,
		"Flush decoding",
		CLI_SUPERVISOR
	},
#endif /* __LINUX__ */

	CLIMOD_DEBUG_CLIENTRY(MPV),

	// last cli command record, NULL
 	{
		NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
	}
};


LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(Mpv)
{
	"mpv",
	NULL,
	NULL,
	arMpvCmdTbl,
	"MPEG Video Decoder command",
	CLI_SUPERVISOR
};
LINT_RESTORE


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


static INT32 _MpvDbkThrsdCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgDisable;
    UINT32 u4QpAvgThrsd;
    UINT32 u4DbkRatioThrsd;
    UINT32 u4QpAvgThrsdP;
    UINT32 u4DbkRatioThrsdP;
    UINT32 u4MvThrsd;


    if ((i4Argc < 7) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL)
        ||(szArgv[4] == NULL) || (szArgv[5] == NULL) || (szArgv[6] == NULL))
    {
        Printf("Arg: Disable QPThrsd DbkRatioThrsd QPThrsdP DbkRatioThrsdP u4MvThrsd)\n");
        return 0;
    }

    fgDisable = (UCHAR)StrToInt(szArgv[1]);
    u4QpAvgThrsd = (UINT16)StrToInt(szArgv[2]);
    u4DbkRatioThrsd = (UINT16)StrToInt(szArgv[3]);
    u4QpAvgThrsdP = (UINT16)StrToInt(szArgv[4]);
    u4DbkRatioThrsdP = (UINT16)StrToInt(szArgv[5]);
    u4MvThrsd = (UINT16)StrToInt(szArgv[6]);

#ifdef CC_53XX_SWDMX_V2
    MPV_SetDBkThrsd(ES0, fgDisable, u4QpAvgThrsd, u4DbkRatioThrsd, u4QpAvgThrsdP, u4DbkRatioThrsdP,u4MvThrsd);
#else
    MPV_SetDBkThrsd(fgDisable, u4QpAvgThrsd, u4DbkRatioThrsd, u4QpAvgThrsdP, u4DbkRatioThrsdP,u4MvThrsd);
#endif

    return 0;
}

static INT32 _MpvDbkPara1Cmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucEsId;
    UINT32 u4P1;
    UINT32 u4P2;
    UINT32 u4P3;
    UINT32 u4P4;
    UINT32 u4P5;
    UINT32 u4P6;
    UINT32 u4P7;
    UINT32 u4P8;
    UINT32 u4P9;
    UINT32 u4P10;


    if ((i4Argc < 12) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) ||
                            (szArgv[3] == NULL) || (szArgv[4] == NULL) || (szArgv[5] == NULL) ||(szArgv[6] == NULL) || (szArgv[7] == NULL) || (szArgv[8] == NULL)
                            ||(szArgv[9] == NULL) || (szArgv[10] == NULL) || (szArgv[11] == NULL))
    {
        Printf("Arg: EsId u4P1 u4P2 u4P3 u4P4 u4P5 u4P6 u4P7 u4P8 u4P9 u4P10\n");
        return 0;
    }

    ucEsId = (UCHAR)StrToInt(szArgv[1]);
    u4P1 = (UINT32)StrToInt(szArgv[2]);
    u4P2 = (UINT32)StrToInt(szArgv[3]);
    u4P3 = (UINT32)StrToInt(szArgv[4]);
    u4P4 = (UINT32)StrToInt(szArgv[5]);
    u4P5 = (UINT32)StrToInt(szArgv[6]);
    u4P6 = (UINT32)StrToInt(szArgv[7]);
    u4P7 = (UINT32)StrToInt(szArgv[8]);
    u4P8 = (UINT32)StrToInt(szArgv[9]);
    u4P9 = (UINT32)StrToInt(szArgv[10]);
    u4P10 = (UINT32)StrToInt(szArgv[11]);


    MPV_SetDbkPara1(ucEsId, u4P1, u4P2, u4P3, u4P4, u4P5, u4P6, u4P7, u4P8, u4P9, u4P10);

    return 0;
}


static INT32 _MpvDbkPara2Cmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucEsId;
    UINT32 u4P1;
    UINT32 u4P2;
    UINT32 u4P3;
    UINT32 u4P4;
    UINT32 u4P5;
    UINT32 u4P6;
    UINT32 u4P7;
    UINT32 u4P8;
    UINT32 u4P9;
    UINT32 u4P10;


    if ((i4Argc < 12) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) ||
                            (szArgv[3] == NULL) || (szArgv[4] == NULL) || (szArgv[5] == NULL) ||(szArgv[6] == NULL) || (szArgv[7] == NULL) || (szArgv[8] == NULL)
                            ||(szArgv[9] == NULL) || (szArgv[10] == NULL) || (szArgv[11] == NULL))
    {
        Printf("Arg: EsId u4P1 u4P2 u4P3 u4P4 u4P5 u4P6 u4P7 u4P8 u4P9 u4P10\n");
        return 0;
    }

    ucEsId = (UCHAR)StrToInt(szArgv[1]);
    u4P1 = (UINT32)StrToInt(szArgv[2]);
    u4P2 = (UINT32)StrToInt(szArgv[3]);
    u4P3 = (UINT32)StrToInt(szArgv[4]);
    u4P4 = (UINT32)StrToInt(szArgv[5]);
    u4P5 = (UINT32)StrToInt(szArgv[6]);
    u4P6 = (UINT32)StrToInt(szArgv[7]);
    u4P7 = (UINT32)StrToInt(szArgv[8]);
    u4P8 = (UINT32)StrToInt(szArgv[9]);
    u4P9 = (UINT32)StrToInt(szArgv[10]);
    u4P10 = (UINT32)StrToInt(szArgv[11]);


    MPV_SetDbkPara2(ucEsId, u4P1, u4P2, u4P3, u4P4, u4P5, u4P6, u4P7, u4P8, u4P9, u4P10);

    return 0;
}


static INT32 _MpvEcCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucEsId;
    UINT32 u4Mode;

    if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        Printf("Arg: EsId Mode)\n");
        return 0;
    }

    ucEsId = (UCHAR)StrToInt(szArgv[1]);
    u4Mode = (UINT16)StrToInt(szArgv[2]);

    MPV_SetEcSetting(ucEsId, u4Mode);

    return 0;
}


static INT32 _MpvDbkRegionCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucEsId;
    UINT32 u4XStart;
    UINT32 u4XEnd;
    UINT32 u4YStart;
    UINT32 u4YEnd;

    if ((i4Argc < 6) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) ||
                            (szArgv[3] == NULL) || (szArgv[4] == NULL) || (szArgv[5] == NULL))
    {
        Printf("Arg: EsId u4XStart u4XEnd u4YStart u4YEnc\n");
        return 0;
    }

    ucEsId = (UCHAR)StrToInt(szArgv[1]);
    u4XStart = (UINT32)StrToInt(szArgv[2]);
    u4XEnd = (UINT32)StrToInt(szArgv[3]);
    u4YStart = (UINT32)StrToInt(szArgv[4]);
    u4YEnd = (UINT32)StrToInt(szArgv[5]);

    MPV_SetDbkRegion(ucEsId, u4XStart, u4XEnd, u4YStart, u4YEnd);

    return 0;
}


static INT32 _MpvDbkCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgEnable;
    BOOL fgEngrMode;

    if ((i4Argc < 3) || (szArgv == NULL) ||
        (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        Printf("Arg: fgEnable fgEngrMode\n");
        return 0;
    }

    fgEnable = (UCHAR)StrToInt(szArgv[1]);
    fgEngrMode = (UCHAR)StrToInt(szArgv[2]);

    MPV_SetDbk(fgEnable, fgEngrMode);

    return 0;
}


static INT32 _MpvDbkQueryCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgDbkEnable;
    UINT16 u2XStart;
    UINT16 u2YStart;
    UINT16 u2Width;
    UINT16 u2Height;
    UINT16 u2Level;

    MPV_GetDbkLevel(ES0, &u2Level);
    MPV_GetDbkInfo(ES0, &fgDbkEnable, &u2XStart, &u2YStart, &u2Width, &u2Height);

    Printf("OnOff: %d, Level: %d\n",
        fgDbkEnable, u2Level);

    return 0;
}

static INT32 _MpvDbkSetCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgDbkEnable;
    UINT16 u2XStart;
    UINT16 u2YStart;
    UINT16 u2Width;
    UINT16 u2Height;
    UINT16 u2Level;

    if ((i4Argc < 3) || (szArgv == NULL) ||
        (szArgv[1] == NULL) ||
        (szArgv[2] == NULL))
    {
        Printf("Arg: OnOff Level(0~100)\n");
        Printf(" Ex: 1 50\n");
        return 0;
    }

    fgDbkEnable = (BOOL)StrToInt(szArgv[1]);
    u2Level = (UINT16)StrToInt(szArgv[2]);
    u2XStart = 0;
    u2YStart = 0;
    u2Width = 1000;
    u2Height = 1000;

    MPV_SetDbkLevel(ES0, u2Level);
    MPV_SetDbkInfo(ES0, fgDbkEnable, u2XStart, u2YStart, u2Width, u2Height);

    Printf("OnOff: %d, Level: %d\n",
        fgDbkEnable, u2Level);

    return 0;
}

static INT32 _MpvMcModeCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucEsId;
    BOOL fgMcPipe;
    BOOL fgMcFastAddr;
    BOOL fgMcNewFsm;

    if ((i4Argc < 5) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) ||
                                (szArgv[3] == NULL) || (szArgv[4] == NULL))
    {
        Printf("Arg: EsId fgMcPipe fgMcFastAddr fgMcNewFsm\n");
        return 0;
    }

    ucEsId = (UCHAR)StrToInt(szArgv[1]);
    fgMcPipe = (UINT32)StrToInt(szArgv[2]);
    fgMcFastAddr = (UINT32)StrToInt(szArgv[3]);
    fgMcNewFsm = (UINT32)StrToInt(szArgv[4]);

    MPV_SetMcMode(ucEsId, fgMcPipe, fgMcFastAddr, fgMcNewFsm);

    return 0;
}


static INT32 _MpvReduceRefCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucEsId;
    UINT32 u4Ratio;

    if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) ||
        (szArgv[2] == NULL) )
    {
        Printf("Arg: EsId u4Raio(0: 1/2, 1: 3/4)\n");
        return 0;
    }

    ucEsId = (UCHAR)StrToInt(szArgv[1]);
    u4Ratio = (UINT32)StrToInt(szArgv[2]);

    MPV_SetRR(ucEsId, u4Ratio);

    return 0;
}


static INT32 _MpvInitCmd (INT32 i4Argc, const CHAR ** szArgv)
{

    UNUSED(i4Argc);
    UNUSED(szArgv);

    MPV_Init();

    return 0;
}


static INT32 _MpvTermintCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL) )
    {
        Printf("Arg: 0x128 \n");
        return 0;
    }

    UNUSED(i4Argc);
    UNUSED(szArgv);

    MPV_Termint();

    return 0;
}


static INT32 _MpvResetCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    MPV_Reset();

    return 0;
}


static INT32 _MpvPlayCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucEsId;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL) )
    {
        Printf("Arg: ucEsId \n");
        return 0;
    }

    ucEsId = (UCHAR)StrToInt(szArgv[1]);

    MPV_Play(ucEsId);

    return 0;
}


static INT32 _MpvStopCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucEsId;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("Arg: ucEsId\n");
        return 0;
    }

    ucEsId = (UCHAR)StrToInt(szArgv[1]);

    MPV_Stop(ucEsId);

    return 0;
}


static INT32 _MpvPauseCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucEsId;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("Arg: ucEsId\n");
        return 0;
    }

    ucEsId = (UCHAR)StrToInt(szArgv[1]);

    MPV_Pause(ucEsId);

    return 0;
}


static INT32 _MpvSyncStcCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucEsId;
    UCHAR ucMode;
    UCHAR ucStcId;

    if ((i4Argc < 4) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL))
    {
        Printf("Arg: ucEsId ucMode ucStcId\n");
        return 0;
    }

    ucEsId = (UCHAR)StrToInt(szArgv[1]);
    ucMode = (UCHAR)StrToInt(szArgv[2]);
    ucStcId = (UCHAR)StrToInt(szArgv[3]);

    VDEC_SyncStc(ucEsId, ucMode, ucStcId);

    return 0;
}


static INT32 _MpvSetECCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucEsId;
    UCHAR ucECMethod;
    UINT32 u4ErrThrsd;
    BOOL fgDetectRefLost;

    if ((i4Argc < 5) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL) || (szArgv[4] == NULL))
    {
        Printf("Arg: ucEsId ucECMethod u4ErrThrsd fgDetectRefLost\n");
        return 0;
    }

    ucEsId = (UCHAR)StrToInt(szArgv[1]);
    ucECMethod = (UCHAR)StrToInt(szArgv[2]);
    u4ErrThrsd = (UINT32)StrToInt(szArgv[3]);
    fgDetectRefLost = (BOOL)StrToInt(szArgv[4]);

    MPV_SetEC(ucEsId, ucECMethod, u4ErrThrsd, fgDetectRefLost);

    return 0;
}


static INT32 _MpvQueryCmd (INT32 i4Argc, const CHAR ** szArgv)
{

    UNUSED(i4Argc);
    UNUSED(szArgv);

    MPV_QueryStatus();

    return 0;
}


static INT32 _MpvClrRRDbk (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucEsId;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("Arg: ucEsId\n");
        return 0;
    }

    ucEsId = (UCHAR)StrToInt(szArgv[1]);

    MPV_CleanRRDbk(ucEsId);

    return 0;
}

#ifdef CC_BOOT_VDEC_LOGO
static INT32 _MpvShowLogo (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    MPV_ShowLogo();

    return 0;
}
#endif

#ifdef MPV_VIRTUAL_PRS
static INT32 _MpvVirtualPrsStart (INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4Delay;
	INT32 i4Idx;

	if ((i4Argc < 5) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL) || (szArgv[4] == NULL))
	{
		Printf("Arg: u4Delay ucMvpId u4Addr u4PicNs ...\n");
		return 0;
	}

	u4Delay = StrToInt(szArgv[1]);
	i4Idx = 1;

	while ((i4Argc - i4Idx) > 3)
	{
		UINT32 u4PicNs;
		UINT32 u4FrmNs;
		UINT32 u4Addr;
		UCHAR ucMpvId;

		ucMpvId = (UCHAR) StrToInt(szArgv[i4Idx + 1]);
		u4Addr = StrToInt(szArgv[i4Idx + 2]);
		u4PicNs = StrToInt(szArgv[i4Idx + 3]);
		u4FrmNs = StrToInt(szArgv[i4Idx + 4]);

		MPV_VirtualPrsParse(ucMpvId, u4Addr, u4PicNs, u4FrmNs);

		i4Idx += 3;
	}

	MPV_VirtualPrsStart(u4Delay);
	return 0;
}


static INT32 _MpvVirtualPrsStop (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    MPV_VirtualPrsStop();
    return 0;
}
#endif


#ifdef MPV_SEMI_HOST
static INT32 _MpvSetSaveNo (INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4StartSaveNo;
	UINT32 u4EndSaveNo;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) ||(szArgv[2] == NULL) )
	{
		Printf("Arg: u4StartSaveNo u4EndSaveNo\n");
		return 0;
	}

	u4StartSaveNo = (UINT32)StrToInt(szArgv[1]);
	u4EndSaveNo = (UINT32)StrToInt(szArgv[2]);

	_MPV_SetSaveNo(u4StartSaveNo, u4EndSaveNo);

	return 0;
}


static INT32 _MpvSetSaveFilePath (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Flag;

    if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) ||(szArgv[2] == NULL))
	{
        Printf("Arg: fgSave File_Path File_Name_Prefix\n");
		return 0;
	}

    u4Flag = (UINT32)StrToInt(szArgv[1]);
    _MPV_SetFilePath(u4Flag,(char*)szArgv[2]);
	return 0;
}

#endif


#ifdef MPV_STATISTICS
static INT32 _MpvResetTime (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    _MPV_ResetTime();
    return 0;
}


static INT32 _MpvPrintTime (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    _MPV_PrintTime(0);
    return 0;
}

#endif


#ifdef MPV_VIRTUAL_CC
static INT32 _MpvVirtualCcCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucEsId;
    BOOL fgEnable;


    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        Printf("Arg: EsId fgEnable\n");
        return 0;
    }

    ucEsId = (UCHAR)StrToInt(szArgv[1]);
    fgEnable = (UCHAR)StrToInt(szArgv[2]);

    MPV_PlayVirtualCc(ucEsId, fgEnable);

    return 0;
}
#endif


#ifdef MPV_138184
static UCHAR *_aucEs;
//static UCHAR _aucEs[47140000] = {0};
//static UCHAR _aucEs[5*1024*1024] = {0};
//static UCHAR _aucEs[10140000] = {0};

// add in t32 cmm file
//;V (_rVdecInfo).fgSave = 0 
//;V fgMPVDumpBeforeTrigger = 1
//;V fgMPVDumpAfterTrigger = 1
//;V _bVdecLogSimPat = 1
//;V _i4MPVLogLvl = 0x00010001
//;b.s _MPV_DecReset
//;V fgDramBusyTest = 1
//;V _fgMpvEnCrc = 1
//;V _fgDumpMCData = 1

static INT32 _MpvTestCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr;
    UINT32 u4TestCase;
    UINT32 u4SubCase;
    UINT32 u4SaveCase;
    UINT32 u4Save;

    _aucEs = (UCHAR*)0x4000000;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    u4Save = 0;
    u4TestCase = (UINT32)StrToInt(szArgv[1]);
    u4SubCase = (UINT32)StrToInt(szArgv[2]);
    u4SaveCase = (UINT32)StrToInt(szArgv[3]);
    u4Addr = ((UINT32)_aucEs + 0xf) & ~0xf;
    Printf("13818-4 SrcAddr: 0x%x\n", u4Addr);
    if(u4SaveCase > 0)
    {
        u4Save = 1;
    }

    CLI_Parser("fbm.i");
    CLI_Parser("fbm.vd.start 10");
    //CLI_Parser("mpv.i");
    CLI_Parser("vdec.i");
    CLI_Parser("mpv.d_l -L");
    CLI_Parser("mpv.d_l 6");
    CLI_Parser("vdec.d_l -L");
    CLI_Parser("vdec.d_l 6");
    //CLI_Parser("mpv.p 0");
    CLI_Parser("vdec.p 0 0");
    CLI_Parser("mpv.dbk 0 0");

    x_thread_delay(1000);
    //for compare test
	//_MPV_SetSaveNo(6910, 6910); // set big to skip compare or save frame
    //_MPV_SetSaveNo(820, 989); // set value to compare or save specified frame

    if(u4TestCase == 1) // testcase A
    {
        Printf("Golden case\n");
        UNUSED(T32_HostLoadData("d:/testdata/mpv/gold.bits", (UCHAR*)u4Addr));
        CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/gold/gold_", u4Save);
        CLI_Parser_Arg("mpv.vp.start 33 0 %d 1114 989", u4Addr); //gold
    }
    else if(u4TestCase == 2) // testcase B
    {
        Printf("Other case\n");
        UNUSED(T32_HostLoadData("d:/testdata/mpv/other.bits", (UCHAR*)u4Addr));
        //UNUSED(T32_HostLoadData("d:/testdata/mpv/other_c.bits", (UCHAR*)u4Addr));
        CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/other/other_", u4Save);
        CLI_Parser_Arg("mpv.vp.start 33 0 %d 965 965", u4Addr); //other
    }
    else if(u4TestCase == 3) // testcase C
    {
        Printf("HD case\n");
        if(u4SubCase == 1)
        {
            Printf("Atsc_06pid49\n");
            UNUSED(T32_HostLoadData("d:/Testdata/mpv/FromNivea/bitstream/Atsc_06pid49.mpg", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/Testdata/mpv/FromNivea/bitstream/Atsc_06pid49/Atsc_06pid49_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 5 5", u4Addr); //Atsc_06pid49 try 5 frames
        }
        else if(u4SubCase == 2)
        {
            Printf("Atsc_08pid49\n");
            UNUSED(T32_HostLoadData("d:/Testdata/mpv/FromNivea/bitstream/Atsc_08pid49.mpg", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/Testdata/mpv/FromNivea/bitstream/Atsc_08pid49/Atsc_08pid49_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 10 10", u4Addr); //Atsc_08pid49 try 10 frames
        }
        else if(u4SubCase == 3)
        {
            Printf("1920x2160_MPEG1\n");
            UNUSED(T32_HostLoadData("d:/Testdata/mpv/high_resolution/1920x2160_MPEG1.bits", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/Testdata/mpv/high_resolution/1920x2160_MPEG1/1920x2160_MPEG1_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 10 10", u4Addr); //try 10 frames
        }
        else if(u4SubCase == 4)
        {
            Printf("1920x2160_MPEG2\n");
            UNUSED(T32_HostLoadData("d:/Testdata/mpv/high_resolution/1920x2160_MPEG2.bits", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/Testdata/mpv/high_resolution/1920x2160_MPEG2/1920x2160_MPEG2_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 10 10", u4Addr); //try 10 frames
        }
        else if(u4SubCase == 5)
        {
            Printf("3840x1088_MPEG1\n");
            UNUSED(T32_HostLoadData("d:/Testdata/mpv/high_resolution/3840x1088_MPEG1.bits", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/Testdata/mpv/high_resolution/3840x1088_MPEG1/3840x1088_MPEG1_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 10 10", u4Addr); //try 10 frames
        }
        else if(u4SubCase == 6)
        {
            Printf("3840x1088_MPEG2\n");
            UNUSED(T32_HostLoadData("d:/Testdata/mpv/high_resolution/3840x1088_MPEG2.bits", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/Testdata/mpv/high_resolution/3840x1088_MPEG2/3840x1088_MPEG2_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 10 10", u4Addr); //try 10 frames
        }
    }
    else if(u4TestCase == 4) // testcase mpeg1
    {
        Printf("mpeg1 case\n");
        if(u4SubCase == 1)
        {
            Printf("ccm1\n");
            UNUSED(T32_HostLoadData("d:/Testdata/mpv/mpeg1/ccm1.bits", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/Testdata/mpv/mpeg1/ccm1/ccm1_16x32_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 129 129", u4Addr); //Atsc_06pid49 try 5 frames
        }
        else if(u4SubCase == 2)
        {
            Printf("tcela-11v2\n");
            UNUSED(T32_HostLoadData("d:/Testdata/mpv/mpeg1/tcela-11v2.bits", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/Testdata/mpv/mpeg1/tcela-11v2/tcela-11v2_16x32_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 15 15", u4Addr); //Atsc_06pid49 try 5 frames
        }
        else if(u4SubCase == 3)
        {
            Printf("tcela-12\n");
            UNUSED(T32_HostLoadData("d:/Testdata/mpv/mpeg1/tcela-12.bits", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/Testdata/mpv/mpeg1/tcela-12/tcela-12_16x32_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 30 30", u4Addr); //Atsc_06pid49 try 5 frames
        }
        else if(u4SubCase == 4)
        {
            Printf("tcela-16-matrices\n");
            UNUSED(T32_HostLoadData("d:/Testdata/mpv/mpeg1/tcela-16-matrices.bits", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/Testdata/mpv/mpeg1/tcela-16-matrices/tcela-16-matrices_16x32_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 31 31", u4Addr); //Atsc_06pid49 try 5 frames
        }
        else if(u4SubCase == 5)
        {
            Printf("tcela-19-wide\n");
            UNUSED(T32_HostLoadData("d:/Testdata/mpv/mpeg1/tcela-19-wide.bits", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/Testdata/mpv/mpeg1/tcela-19-wide/tcela-19-wide_16x32_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 31 31", u4Addr); //Atsc_06pid49 try 5 frames
        }
    }
    else if(u4TestCase == 6) // testcase F, reduce reference
    {
        Printf("Reduce reference mode\n");
        //UNUSED(T32_HostLoadData("d:/testdata/mpv/hhi_burst_long.bits", (UCHAR*)u4Addr));
        //CLI_Parser("mpv.sf 1 d:/testdata/mpv/RR/gi6/dec_");

        if(u4SubCase == 1)
        {
            UNUSED(T32_HostLoadData("d:/testdata/mpv/gold.bits", (UCHAR*)u4Addr));
            Printf("Reduce reference 1/2 mode\n");
            CLI_Parser("mpv.rr 0 0");
            CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/RR/1_2/gold_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 1114 989", u4Addr);
        }
        else if(u4SubCase == 2)
        {
            UNUSED(T32_HostLoadData("d:/testdata/mpv/gold.bits", (UCHAR*)u4Addr));
            Printf("Reduce reference 3/4 mode\n");
            CLI_Parser("mpv.rr 0 1");
            CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/RR/3_4/34gold_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 1114 989", u4Addr);
        }
        else if(u4SubCase == 3)
        {
            UNUSED(T32_HostLoadData("d:/testdata/mpv/gi6.bits", (UCHAR*)u4Addr));
            Printf("Reduce reference gi6 1/2 mode\n");
            CLI_Parser("mpv.rr 0 0");
            CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/RR/gi6/gi6_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 3 3", u4Addr);
        }
        else if(u4SubCase == 4)
        {
            UNUSED(T32_HostLoadData("d:/testdata/mpv/gi6.bits", (UCHAR*)u4Addr));
            Printf("Reduce reference gi6 3/4 mode\n");
            CLI_Parser("mpv.rr 0 1");
            CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/RR/gi6/34gi6_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 3 3", u4Addr);
        }
        else
        {
            Printf("Undefined\n");
        }
        //CLI_Parser_Arg("mpv.vp.start 33 0 %d 65 45", u4Addr);
    }
    else if(u4TestCase == 7) // testcase G, deblocking
    {
        CLI_Parser("mpv.dbk 1 0");
        Printf("Deblocking mode\n");
        if(u4SubCase == 1)
        {
            Printf("gold\n");
            UNUSED(T32_HostLoadData("d:/testdata/mpv/gold.bits", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/dbk/golddbk/golddbk_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 1114 989", u4Addr);
        }
        else if(u4SubCase == 2)
        {
            Printf("football\n");
            UNUSED(T32_HostLoadData("d:/testdata/mpv/dbk/footballpid65_26-30_500k.m2v", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/dbk/football/foot_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 58 58", u4Addr);
        }
        else if(u4SubCase == 3)
        {
            Printf("pid33\n");
            UNUSED(T32_HostLoadData("d:/testdata/mpv/dbk/pid33_60-70_500k.m2v", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/dbk/pid33/pid33_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 187 187", u4Addr);
        }
        else if(u4SubCase == 4)
        {
            Printf("sony-ct1\n");
            UNUSED(T32_HostLoadData("d:/testdata/mpv/dbk/sony-ct1_200k.m2v", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/dbk/sonyct1/sonyct1_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 63 63", u4Addr);
        }
        else if(u4SubCase == 5)
        {
            Printf("sony-ct4\n");
            UNUSED(T32_HostLoadData("d:/testdata/mpv/dbk/sony-ct4_200k.m2v", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/dbk/sonyct4/sonyct4_", u4Save);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 63 63", u4Addr);
        }
        else
        {
            Printf("Undefined\n");
        }
    }
    else if(u4TestCase == 8)
    {
        Printf("err.bits\n");
        UNUSED(T32_HostLoadData("d:/testdata/mpv/err.bits", (UCHAR*)u4Addr));
        CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/err/err_", u4Save | MPV_SEMI_FLAG_IGNORE_ERR);
        CLI_Parser_Arg("mpv.vp.start 33 0 %d 364 354", u4Addr);
    }
    else if(u4TestCase == 9)
    {
        Printf("gi6.bits\n");
        UNUSED(T32_HostLoadData("d:/testdata/mpv/gi6.bits", (UCHAR*)u4Addr));
        CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/gi6/gi6_", u4Save);
        CLI_Parser_Arg("mpv.vp.start 33 0 %d 3 3", u4Addr);
    }
    else if(u4TestCase == 10)
    {
        Printf("att.bits\n");
        UNUSED(T32_HostLoadData("d:/testdata/mpv/att.bits", (UCHAR*)u4Addr));
        CLI_Parser("mpv.sf 0 d:/testdata/mpv/att/att_");
        CLI_Parser_Arg("mpv.vp.start 33 0 %d 3 3", u4Addr);
    }
    else if(u4TestCase == 11)
    {
        Printf("sony-ct1.bits\n");
        UNUSED(T32_HostLoadData("d:/testdata/mpv/sony-ct1.bits", (UCHAR*)u4Addr));
        CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/sony-ct1/sony-ct1_", u4Save);
        CLI_Parser_Arg("mpv.vp.start 33 0 %d 77 60", u4Addr);
    }
    else if(u4TestCase == 12)
    {
        Printf("mpeg1_block_pixel_overflow.mpeg\n");
        UNUSED(T32_HostLoadData("d:/testdata/mpv/err/bitstream/mpeg1_block_pixel_overflow.mpeg", (UCHAR*)u4Addr));
        CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/err/mpeg1_block_pixel_overflow/mpeg1_block_pixel_overflow_", u4Save | MPV_SEMI_FLAG_IGNORE_ERR);
        CLI_Parser_Arg("mpv.vp.start 33 0 %d 1 1", u4Addr);
    }
    else if(u4TestCase == 13)
    {
        Printf("err2.raw\n");
        UNUSED(T32_HostLoadData("d:/testdata/mpv/err/err2/err2.bits", (UCHAR*)u4Addr));
        CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/err/err2/err2_", u4Save | MPV_SEMI_FLAG_IGNORE_ERR);
        CLI_Parser_Arg("mpv.vp.start 33 0 %d 3 3", u4Addr);
    }
    else if(u4TestCase == 14)
    {
        Printf("toshiba.bits\n");
        UNUSED(T32_HostLoadData("d:/testdata/mpv/FromNivea/bitstream/toshiba.bits", (UCHAR*)u4Addr));
        CLI_Parser_Arg("mpv.sf %d d:/temp/toshiba_", u4Save);
        CLI_Parser_Arg("mpv.vp.start 33 0 %d 4 4", u4Addr);
    }
    else if(u4TestCase == 15)
    {
        Printf("Sarnoff\n");
        if(u4SubCase == 1)
        {
            Printf("BURSTY_480i.mpv\n");
            UNUSED(T32_HostLoadData("d:/testdata/mpv/Collect/Sarnoff/480i/BURSTY_480i.mpv", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/Collect/Sarnoff/480i/BURSTY_480i/BURSTY_480i_", u4Save | MPV_SEMI_FLAG_IGNORE_ERR);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 180 180", u4Addr);
        }
        else if(u4SubCase == 2)
        {
            Printf("ERR_BLS.mpv\n");
            UNUSED(T32_HostLoadData("d:/testdata/mpv/Collect/Sarnoff/480i/ERR_BLS.mpv", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/Collect/Sarnoff/480i/ERR_BLS/ERR_BLS_", u4Save | MPV_SEMI_FLAG_IGNORE_ERR);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 120 120", u4Addr);
        }
        else if(u4SubCase == 3)
        {
            Printf("ERR_G1.mpv\n");
            UNUSED(T32_HostLoadData("d:/testdata/mpv/Collect/Sarnoff/480i/ERR_G1.mpv", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/Collect/Sarnoff/480i/ERR_G1/ERR_G1_", u4Save | MPV_SEMI_FLAG_IGNORE_ERR);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 120 120", u4Addr);
        }
        else if(u4SubCase == 4)
        {
            Printf("ERR_GS.mpv\n");
            UNUSED(T32_HostLoadData("d:/testdata/mpv/Collect/Sarnoff/480i/ERR_GS.mpv", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/Collect/Sarnoff/480i/ERR_GS/ERR_GS_", u4Save | MPV_SEMI_FLAG_IGNORE_ERR);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 90 90", u4Addr);
        }
        else if(u4SubCase == 5)
        {
            Printf("ERR_S1.mpv\n");
            UNUSED(T32_HostLoadData("d:/testdata/mpv/Collect/Sarnoff/480i/ERR_S1.mpv", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/Collect/Sarnoff/480i/ERR_S1/ERR_S1_", u4Save | MPV_SEMI_FLAG_IGNORE_ERR);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 120 120", u4Addr);
        }
        else if(u4SubCase == 6)
        {
            Printf("ERR_SL1.mpv\n");
            UNUSED(T32_HostLoadData("d:/testdata/mpv/Collect/Sarnoff/480i/ERR_SL1.mpv", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/Collect/Sarnoff/480i/ERR_SL1/ERR_SL1_", u4Save | MPV_SEMI_FLAG_IGNORE_ERR);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 120 120", u4Addr);
        }
        else if(u4SubCase == 7)
        {
            Printf("ERR_SL2.mpv\n");
            UNUSED(T32_HostLoadData("d:/testdata/mpv/Collect/Sarnoff/480i/ERR_SL2.mpv", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/Collect/Sarnoff/480i/ERR_SL2/ERR_SL2_", u4Save | MPV_SEMI_FLAG_IGNORE_ERR);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 120 120", u4Addr);
        }
        else if(u4SubCase == 8)
        {
            Printf("ERR_SLS.mpv\n");
            UNUSED(T32_HostLoadData("d:/testdata/mpv/Collect/Sarnoff/480i/ERR_SLS.mpv", (UCHAR*)u4Addr));
            CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/Collect/Sarnoff/480i/ERR_SLS/ERR_SLS_", u4Save | MPV_SEMI_FLAG_IGNORE_ERR);
            CLI_Parser_Arg("mpv.vp.start 33 0 %d 120 120", u4Addr);
        }
        else
        {
            Printf("Undefined\n");
        }
    }
    else if(u4TestCase == 16)
    {
        Printf("NO.453_mpeg_352x240.bits\n");
        UNUSED(T32_HostLoadData("d:/testdata/mpv/ErrSony/NO.453_mpeg_352x240.bits", (UCHAR*)u4Addr));
        CLI_Parser_Arg("mpv.sf %d d:/testdata/mpv/ErrSony/NO.453_mpeg_352x240/NO.453_mpeg_352x240_", u4Save | MPV_SEMI_FLAG_IGNORE_ERR);
        CLI_Parser_Arg("mpv.vp.start 33 0 %d 120 120", u4Addr);

    }
    return 0;
}

#endif


#if defined(__linux__)
static INT32 _MpvFlushCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucEsId;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("Arg: ucEsId\n");
        return 0;
    }

    ucEsId = (UCHAR)StrToInt(szArgv[1]);

    MPV_FlushEsmQ(ucEsId, FALSE);

    return 0;
}
#endif /* __LINUX__ */

#if 0
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

#ifdef __MW_CLI_DEF__
CLI_EXEC_T* GetMpvMwCmdTbl(void)
#else
CLI_EXEC_T* GetMpvCmdTbl(void)
#endif
{
	return &_rMpvModCmdTbl;
}
#endif

