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
/*****************************************************************************
*  TVE: Diagnostic command
*****************************************************************************/

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_printf.h"
#include "x_stl_lib.h"

#define DEFINE_IS_LOG	CLI_IsLog
#include "x_debug.h"

#include "tve_if.h"
#include "source_table.h"
#include "vdp_if.h"
#include "video_def.h"
#include "nptv_if.h"
#include "vdp_drvif.h"
#include "source_select.h"
#include "b2r_if.h"

/******************************************************************************
* Function prototype
******************************************************************************/
static INT32 _TveInitCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveUnInitCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveResetCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveQueryCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveEnableCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveDisableCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveSetCmdMode (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveSetCmdFmt (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveSetCmdImpedence (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveSetCmdColorBar (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveCcEnableCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveCcDisableCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveCcSendCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveSetCgmsaCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveWssEnableCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveWssDisableCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveSetAspectRatio (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveTTXEnableCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveTTXDisableCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveTTXSendCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TvePowerOnCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TvePowerDownCmd (INT32 i4Argc, const CHAR ** szArgv);
#if defined(CC_MT5363)||defined(CC_TVE6595)
static INT32 _TvePIPMixerCmd (INT32 i4Argc, const CHAR ** szArgv);
#endif
static INT32 _TveCtlSetSourceCmd(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveCtlSetMuteDelayCmd(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _TveHalSetCurrentCmd(INT32 i4Argc, const CHAR ** szArgv);


/******************************************************************************
* CLI Command Table
******************************************************************************/
CLIMOD_DEBUG_FUNCTIONS(TVE)

static CLI_EXEC_T _arTveSetCmdTbl[] =
{
 	{"fmt",			"f",		_TveSetCmdFmt, NULL,		"Tve format", CLI_SUPERVISOR},
   	{"imp",			NULL,		_TveSetCmdImpedence, NULL,	"Tve Impedence (0/1 => Low/High)", CLI_SUPERVISOR},
 	{"mode",		"m",		_TveSetCmdMode, NULL,		"Tve mode", CLI_SUPERVISOR},
 	{"colorbar",    "cb",		_TveSetCmdColorBar, NULL,	"Tve colorbar", CLI_SUPERVISOR},

 	{NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arTveCcCmdTbl[] =
{
 	{"enable",		"e",	_TveCcEnableCmd, NULL,		"CC enable", CLI_SUPERVISOR},
 	{"disable",		"d",	_TveCcDisableCmd, NULL,		"CC disable", CLI_SUPERVISOR},
 	{"send",		"s",	_TveCcSendCmd, NULL,		"CC send", CLI_SUPERVISOR},

 	{NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arTveCgmsaCmdTbl[] =
{
 	{"set",	    "s",	_TveSetCgmsaCmd,       NULL,	"CGMS-A set value", CLI_SUPERVISOR},

 	{NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arTveWssCmdTbl[] =
{
 	{"enable",	"e",	_TveWssEnableCmd,       NULL,	"WSS enable", CLI_SUPERVISOR},
 	{"disable",	"d",	_TveWssDisableCmd,      NULL,	"WSS disable", CLI_SUPERVISOR},
 	{"aspect",  "asr",  _TveSetAspectRatio,  NULL,   "Aspect ratio", CLI_SUPERVISOR},

 	{NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arTveTTXCmdTbl[] =
{
 	{"enable",		"e",	_TveTTXEnableCmd, NULL,		"TTX enable", CLI_SUPERVISOR},
 	{"disable",		"d",	_TveTTXDisableCmd, NULL,		"TTX disable", CLI_SUPERVISOR},
 	{"send",		"s",	_TveTTXSendCmd, NULL,		"TTX send", CLI_SUPERVISOR},

 	{NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arTveCmdTbl[] =
{
 	{"init",		"i",	_TveInitCmd, NULL,			"Tve init", CLI_SUPERVISOR},
 	{"enable",		"e",	_TveEnableCmd, NULL,		"Tve enable", CLI_SUPERVISOR},
 	{"disable",		"d",	_TveDisableCmd, NULL,		"Tve disable", CLI_SUPERVISOR},
 	{"reset",		"r",	_TveResetCmd, NULL,			"Tve reset", CLI_SUPERVISOR},
 	{"query",		"q",	_TveQueryCmd, NULL,			"Tve status query", CLI_GUEST},
 	{"stop",		NULL,	_TveUnInitCmd, NULL,		"Tve stop", CLI_SUPERVISOR},
 	{"set",			"s",	NULL, _arTveSetCmdTbl,		"\tTve parameter set", CLI_SUPERVISOR},
 	{"cc",			NULL,	NULL, _arTveCcCmdTbl,		"Tve Close Caption", CLI_SUPERVISOR},	
   	{"cgms",		NULL,	NULL, _arTveCgmsaCmdTbl,	"Tve CGMS-A", CLI_SUPERVISOR},
  	{"wss",			NULL,	NULL, _arTveWssCmdTbl,		"Tve WideScreen Signalling", CLI_SUPERVISOR},
 	{"ttx",			NULL,	NULL, _arTveTTXCmdTbl,		"Tve Teletext", CLI_SUPERVISOR},	
 	{"poweron",	    "po",	_TvePowerOnCmd, NULL,		"DAC Power On", CLI_GUEST},
 	{"powerdown",	"pd",	_TvePowerDownCmd, NULL,		"DAC Power Down", CLI_GUEST},
#if defined(CC_MT5363)||defined(CC_TVE6595)
 	{"pipmixer",	"pm",	_TvePIPMixerCmd, NULL,		"PIP Mixer On/Off", CLI_SUPERVISOR},
#endif
	{"ctrlsource",  "cs",  _TveCtlSetSourceCmd, NULL,               "TVE Control SetSource", CLI_SUPERVISOR}, 	
       {"ctrlsource",  "delay",  _TveCtlSetMuteDelayCmd, NULL,               "TVE Control SetMuteDelay", CLI_SUPERVISOR}, 	
       {"SetCurrent",  "current",  _TveHalSetCurrentCmd, NULL,               "TVE Hal SetCurrent", CLI_SUPERVISOR}, 	
	CLIMOD_DEBUG_CLIENTRY(TVE),
 	
 	{NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


CLI_MAIN_COMMAND_ITEM(tve)
{
	"tve",
	NULL,
	NULL,
	_arTveCmdTbl,
	"Tve command",
	CLI_GUEST
};


/******************************************************************************
* Local Function
******************************************************************************/
static INT32 _TveInitCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

	TVE_Init();
	Printf("Start System OK\n");
	return 0;
}

//===============================================================
static INT32 _TveUnInitCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

	TVE_UnInit();	
	return 0;
}

//===============================================================
static INT32 _TveResetCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

	TVE_Reset(TVE_1);
	TVE_Reset(TVE_2);
	return 0;
}

//===============================================================
static INT32 _TveQueryCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

	TVE_QueryStatus();	
#ifdef CC_SUPPORT_TVE
    vApiTVEQueryStatus();
#endif
 
	return 0;
}

//===============================================================
static INT32 _TveEnableCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucTveId)
	UCHAR ucTveId;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: ucTveId\n");
		return 0;
	}

	ucTveId = (UCHAR) StrToInt(szArgv[1]);

	if (TVE_SetEnable(ucTveId, 1) == TVE_SET_OK)
	{
		return 0;
	}

	return -1;
}

//===============================================================
static INT32 _TveDisableCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucTveId)
	UCHAR ucTveId;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: ucTveId\n");
		return 0;
	}

	ucTveId = (UCHAR) StrToInt(szArgv[1]);

	if (TVE_SetEnable(ucTveId, 0) == TVE_SET_OK)
	{
		return 0;
	}

	return -1;
}

//===============================================================
static INT32 _TveSetCmdMode (INT32 i4Argc, const CHAR ** szArgv)
{
	UCHAR ucTveId;
	UCHAR ucMode;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucTveId ucMode\n");
		return 0;
	}

	ucTveId = (UCHAR) StrToInt(szArgv[1]);
	ucMode = (UCHAR) StrToInt(szArgv[2]);

	if (TVE_SetMode(ucTveId, ucMode) == TVE_SET_OK)
	{
	    if (ucTveId == 0)
	    {
	        if (ucMode == 0)
	        {
	            Printf(" (Main) CVBS/S-Video output \n\n");
	        }
	        else if (ucMode == 1)
	        {
	            Printf(" (Main) YUV output \n\n");
	        }
	        else if (ucMode == 2)
	        {
	            Printf(" (Main) RGB output \n\n");
	        }
	    }
	    else if (ucTveId == 1)
	    {
	        if (ucMode == 0)
	        {
	            Printf(" (Aux) CVBS/S-Video output \n\n");
	        }
	    }
		return 0;
	}

	return -1;
}

//===============================================================
static INT32 _TveSetCmdFmt (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucTveId, UCHAR ucFmt)
	UCHAR ucTveId;
	UCHAR ucFmt;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucTveId ucFmt\n");
		return 0;
	}

	ucTveId = (UCHAR) StrToInt(szArgv[1]);
	ucFmt = (UCHAR) StrToInt(szArgv[2]);

	if (TVE_SetFmt(ucTveId, ucFmt) == TVE_SET_OK)
	{
		return 0;
	}

	return -1;
}

//===============================================================
static INT32 _TveSetCmdImpedence (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucTveId, UCHAR ucImpedence)
	UCHAR ucTveId;
	UCHAR ucImpedence;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucTveId ucImpedence\n");
		return 0;
	}

	ucTveId = (UCHAR) StrToInt(szArgv[1]);
	ucImpedence = (UCHAR) StrToInt(szArgv[2]);

	if (TVE_SetImpedence(ucTveId, ucImpedence) == TVE_SET_OK)
	{
		return 0;
	}

	return -1;
}

//===============================================================
static INT32 _TveSetCmdColorBar (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucTveId, UCHAR ucEnable)
	UCHAR ucTveId;
	UCHAR ucEnable;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucTveId ucEnable\n");
		return 0;
	}

	ucTveId = (UCHAR) StrToInt(szArgv[1]);
	ucEnable = (UCHAR) StrToInt(szArgv[2]);

	if (TVE_SelectInput(1) == TVE_SET_OK)//Vdoin
	{
		if (TVE_SetColorBar(ucTveId, ucEnable) == TVE_SET_OK)
		{
			return 0;
		}
	}

	return -1;
}

//===============================================================
static INT32 _TveCcEnableCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucTveId)
	UCHAR ucTveId;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: ucTveId\n");
		return 0;
	}

	ucTveId = (UCHAR) StrToInt(szArgv[1]);

	if (TVE_SetCcEnable(ucTveId, 1) == TVE_SET_OK)
	{
		return 0;
	}

	return -1;
}

//===============================================================
static INT32 _TveCcDisableCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucTveId)
	UCHAR ucTveId;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: ucTveId\n");
		return 0;
	}

	ucTveId = (UCHAR) StrToInt(szArgv[1]);

	if (TVE_SetCcEnable(ucTveId, 0) == TVE_SET_OK)
	{
		return 0;
	}

	return -1;
}

//===============================================================
static INT32 _TveCcSendCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucTveId)
	UCHAR ucTveId;
	UINT32 u4Addr;
	UINT32 u4Len;
	UINT32 u4Count;

	if ((i4Argc < 4) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL))
	{
		Printf("Arg: ucTveId Address Length\n");
		return 0;
	}

	ucTveId = (UCHAR) StrToInt(szArgv[1]);
	u4Addr = StrToInt(szArgv[2]);
	u4Len = StrToInt(szArgv[3]);

	while (u4Len)
	{
		u4Count = (u4Len > 2) ? 2 : u4Len;
		if (TVE_SendCc(ucTveId, TVE_FIELD_BOTH, (UCHAR*) u4Addr, u4Count) == TVE_SET_OK)
		{
			u4Len -= u4Count;
			Printf("%c ", *((UCHAR *)(u4Addr + 0)));
			Printf("%c\n", *((UCHAR *)(u4Addr + 1)));			
			u4Addr += u4Count;
		}
		else
		{
			x_thread_delay(1000);
		}
	}

	return 0;
}

//===============================================================
static INT32 _TveSetCgmsaCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucTveId)
	UCHAR ucTveId;
	UCHAR ucValue;

	if ((i4Argc < 3) || (szArgv == NULL) || 
	    (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucTveId ucCgmsaValue\n");
		return 0;
	}

	ucTveId = (UCHAR) StrToInt(szArgv[1]);
	ucValue = (UCHAR) StrToInt(szArgv[2]);

	if (TVE_SetCgmsaValue(ucTveId, ucValue) == TVE_SET_OK)
	{
		return 0;
	}

	return -1;
}

//===============================================================
static INT32 _TveWssEnableCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (ucTveId)
	UCHAR ucTveId;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: ucTveId\n");
		return 0;
	}

	ucTveId = (UCHAR) StrToInt(szArgv[1]);

	if (TVE_SetWssEnable(ucTveId, 1) == TVE_SET_OK)
	{
		return 0;
	}

	return -1;
}

//===============================================================
static INT32 _TveWssDisableCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (ucTveId)
	UCHAR ucTveId;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: ucTveId\n");
		return 0;
	}

	ucTveId = (UCHAR) StrToInt(szArgv[1]);

	if (TVE_SetWssEnable(ucTveId, 0) == TVE_SET_OK)
	{
		return 0;
	}

	return -1;
}

//===============================================================
static INT32 _TveSetAspectRatio (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucTveId, UCHAR ucAspectRatio)
	UCHAR ucTveId;
	UCHAR ucAspectRatio;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucTveId ucAspectRatio\n");
		return 0;
	}

	ucTveId = (UCHAR) StrToInt(szArgv[1]);
	ucAspectRatio = (UCHAR) StrToInt(szArgv[2]);

	if (TVE_SetAspectRatio(ucTveId, ucAspectRatio) == TVE_SET_OK)
	{
		return 0;
	}

	return -1;
}

//===============================================================
static INT32 _TveTTXEnableCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucTveId)
	UCHAR ucTveId;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: ucTveId\n");
		return 0;
	}

	ucTveId = (UCHAR) StrToInt(szArgv[1]);

	if (TVE_SetTTXEnable(ucTveId, 1) == TVE_SET_OK)
	{
		return 0;
	}

	return -1;
}

//===============================================================
static INT32 _TveTTXDisableCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucTveId)
	UCHAR ucTveId;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: ucTveId\n");
		return 0;
	}

	ucTveId = (UCHAR) StrToInt(szArgv[1]);

	if (TVE_SetTTXEnable(ucTveId, 0) == TVE_SET_OK)
	{
		return 0;
	}

	return -1;
}

//===============================================================
static INT32 _TveTTXSendCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucTveId)
	UCHAR ucTveId;
	UINT32 u4Addr;
	UINT32 u4Len;
	//UINT32 u4Count;
	//UINT8 testdata[18][45];
	//UINT8 i, j;
	
	if ((i4Argc < 4) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL))
	{
		Printf("Arg: ucTveId Address Length\n");
		return 0;
	}

	ucTveId = (UCHAR) StrToInt(szArgv[1]);
	u4Addr = StrToInt(szArgv[2]);
	u4Len = StrToInt(szArgv[3]);

	TVE_SendTTX(ucTveId, (UCHAR*) u4Addr, u4Len);
	
	return 0;
}

//===============================================================
static INT32 _TvePowerOnCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (DACId)
	UCHAR ucDACId;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: DACId\n");
		return 0;
	}

	ucDACId = (UCHAR) StrToInt(szArgv[1]);

	if (TVE_DACPower(TVE_1, ucDACId, 1) == TVE_SET_OK)
	{
		return 0;
	}

	return -1;
} 

//===============================================================
static INT32 _TvePowerDownCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (DACId)
	UCHAR DACId;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: DACId\n");
		return 0;
	}

	DACId = (UCHAR) StrToInt(szArgv[1]);

	if (TVE_DACPower(TVE_1, DACId, 0) == TVE_SET_OK)
	{
		return 0;
	}

	return -1;
} 

#if defined(CC_MT5363)||defined(CC_TVE6595)
//===============================================================
static INT32 _TvePIPMixerCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (ucTveId)
	UCHAR ucTveId;
	UCHAR ucOnOff;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) ||(szArgv[2] == NULL))
	{
		Printf("Arg: ucTveId ucOnOff\n");
		return 0;
	}

	ucTveId = (UCHAR) StrToInt(szArgv[1]);
	ucOnOff = (UCHAR) StrToInt(szArgv[2]);

	if (TVE_PIPMixer(ucTveId, ucOnOff) == TVE_SET_OK)
	{
		return 0;
	}

	return -1;
} 
#endif

static INT32 _TveCtlSetSourceCmd(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1ScartIdx, u1TmpIdx, u1TmpMinor;
    static UINT8 _au1TveCliOutSource[2];
    static TveUsage_t _rTveUsed = {0xf, 0x0};

    char *strSrcMajName[] = {"atv", "atd", "dtv", "av", "sv", "ypbpr", "vga", "hdmi", "scart", "off"};
    UINT8 u1SrcMajor[] = { VSS_ATV, VSS_ATD, VSS_DTV, VSS_CVBS, VSS_SV, VSS_YPBPR, VSS_VGA, VSS_HDMI, VSS_SCART, VSS_MAX };

    if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("Arg: <TVE idx> <source>\n<source> can be atv/atd/dtv/av/sv/ypbpr/vga/hdmi/scart/off");
        return 0;
    }

    u1ScartIdx = (UCHAR) StrToInt(szArgv[1]);

    if(u1ScartIdx > 1) return -1;

    for(u1TmpIdx=0;u1TmpIdx<sizeof(u1SrcMajor);u1TmpIdx++)
    {
        if(x_strncmp(strSrcMajName[u1TmpIdx], szArgv[2], x_strlen(strSrcMajName[u1TmpIdx])) == 0)
        {
            /* match */
            if(szArgv[2][x_strlen(szArgv[2])-1] >= '1' &&
               szArgv[2][x_strlen(szArgv[2])-1] <= '9')
            {
                u1TmpMinor = szArgv[2][x_strlen(szArgv[2])-1] -'1'; 
            }
            else
            {
                u1TmpMinor = 0;
            }
            _au1TveCliOutSource[u1ScartIdx] = VSS_MKVSRC(u1SrcMajor[u1TmpIdx], u1TmpMinor);
            Printf("SetTVESource as %s\n", szArgv[2]);  
            break;
        }
    }

    if(u1TmpIdx >= sizeof(u1SrcMajor)) return -1;

    if(_rTveUsed.TveIsUsedBy == u1ScartIdx)
    {
        TVE_SetEnable(TVE_1, FALSE);
        VDP_SetEnable(VDP_2, FALSE);
        if(VSS_MAJOR(_au1TveCliOutSource[u1ScartIdx]) == VSS_DTV)
        {
            VDP_SetInput(VDP_2, 0xff, 0);
        }
        bApiVideoSetVideoSrc(SV_VP_PIP, SV_VS_MAX);
        vApiTVESetOutputFormat(SV_TVE_OFMT_NONE);
        _rTveUsed.TveIsUsedBy = 0xf;
    }

    vApiTVESetDacMuteMask(u1ScartIdx, TVE_DAC_MUTE_MASK_DRV, TRUE);
    vApiTVESetScartOutCtrl(u1ScartIdx, &_au1TveCliOutSource[u1ScartIdx], &_rTveUsed);

    if(u1ScartIdx == _rTveUsed.TveIsUsedBy)
    {
        VDP_SetEnable(VDP_2, TRUE);
        TVE_SetEnable(TVE_1, TRUE);
    }
    vApiTVESetDacMuteMask(u1ScartIdx, TVE_DAC_MUTE_MASK_MW, FALSE);

    return 0;
}

extern UINT8 _gMuteDelay;
static INT32 _TveCtlSetMuteDelayCmd(INT32 i4Argc, const CHAR ** szArgv)
{
    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("Arg: delay (10ms)\n");
        return 0;
    }

    _gMuteDelay = (UCHAR) StrToInt(szArgv[1]);
    Printf("MuteDelay= %d (ms)\n", _gMuteDelay*10);
    return 0;
}

static INT32 _TveHalSetCurrentCmd(INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR u1ScartIdx;
    UCHAR u1Current;
    UCHAR u1InputVal;

    if ((i4Argc != 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        goto EXIT;
    }
    u1ScartIdx = (UCHAR) StrToInt(szArgv[1]);    
    u1InputVal = (UCHAR) StrToInt(szArgv[2]);

    if(u1ScartIdx>1 || u1InputVal >3)
        goto EXIT;

    if(u1InputVal==0)
        u1Current = DAC_1_4_CURRENT;
    else if(u1InputVal==1)
        u1Current = DAC_1_2_CURRENT;
    else if(u1InputVal==2)
        u1Current = DAC_3_4_CURRENT;
    else
        u1Current = DAC_FULL_CURRENT;

    TVE_SetCurrent(u1ScartIdx, u1Current);
    return 0;

EXIT:
    Printf("Arg: DAC_ID(0/1)   Current(0/1/2/3)\n");
    Printf("Current(0) : 1/4 Current\n");
    Printf("Current(1) : 1/2 Current\n");
    Printf("Current(2) : 3/4 Current\n");
    Printf("Current(3) : Full Current\n");
    return 0;
}

/******************************************************************************
* Global Function
******************************************************************************/
#if (0)
#ifdef __MW_CLI_DEF__
CLI_EXEC_T* GetTveMwCmdTbl(void)
#else
CLI_EXEC_T* GetTveCmdTbl(void)
#endif
{
	return &_rTveModCmdTbl;
}
#endif
