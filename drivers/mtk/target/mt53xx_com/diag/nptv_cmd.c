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
 * $RCSfile: nptv_cmd.c,v $
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
#include "x_pinmux.h"
#include "nptv_debug.h"
#include "x_debug.h"

#include "source_table.h"
#include "sv_const.h"
#include "video_def.h"
#include "nptv_cmd.h"
#include "drv_scpos.h"
#include "drv_vdoclk.h"
#include "drv_video.h"
#include "vdo_if.h"


#include "drv_scpos.h"
#include "drv_ycproc.h"
#include "mute_if.h"
#include "api_eep.h"
#include "eepdef.h"


#ifdef CC_FLIP_MIRROR_SUPPORT
#include "eeprom_if.h"
#endif


#ifndef CC_CLI
#define CC_CLI
#endif
#ifdef CC_CLI
static INT32 _NptvInitCmd (INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvMainSrcSelCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvPipSrcSelCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvFixColorSpaceCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvSetScanModeCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvMuteCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvMuteQuery(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvMuteOnOff(INT32 i4Argc, const CHAR **szArgv);
#ifdef CC_BOOT_MUTE_LOG
static INT32 _NptvMuteBootLog(INT32 i4Argc, const CHAR **szArgv);
#endif
static INT32 _NptvDemoModeCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvFwOnOff (INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvStatusCmd(INT32 i4Argc, const CHAR **szArgv);

#ifndef NDEBUG 
#ifndef CC_DRIVER_PROGRAM
static INT32 _NptvMwDebug(INT32 i4Argc, const CHAR **szArgv);
#endif
#ifdef CC_FLIP_MIRROR_SUPPORT
static INT32 _NptvSetFlipMirror(INT32 i4Argc, const CHAR **szArgv);
#endif
#endif //NDEBUG

static INT32 _NptvDevDumpCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvDevVideoDumpCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvFWUpgradeCmd(INT32 i4Argc, const CHAR **szArgv);

#endif /*#ifdef CC_CLI*/

EXTERN void vSetUSBDump(BOOL bOnOff);
EXTERN void NptvScalerVideoDump(INT32 i4Argc, const CHAR **szArgv);

EXTERN UINT32 FlashGetBlkSize(void);
EXTERN INT8 FlashWrite(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4Size);

/******************************************************************************
* CLI Command Table
******************************************************************************/

CLIMOD_DEBUG_FUNCTIONS(NPTV)
CLIMOD_DEBUG_FUNCTIONS(MUTE)

CLI_EXEC_T arMuteCmdTbl[] = {
  #ifdef CC_CLI
  {"MuteOnOff",  "oo",   _NptvMuteCmd,   NULL,           "Main/PIP Mute/Unmute",    CLI_GUEST},
  {"MuteQurey",  "q",   _NptvMuteQuery,   NULL,           "Main/PIP Mute query",    CLI_GUEST},
  #ifdef CC_BOOT_MUTE_LOG
  {"MuteBootLog",  "boot",   _NptvMuteBootLog,   NULL,           "Enable/Disable mute log when system boot up",    CLI_GUEST},
  #endif
  {"MuteForceOff",  "forceoff",   _NptvMuteOnOff,   NULL,           "Main/PIP Mute Force OnOff",    CLI_SUPERVISOR},
  CLIMOD_DEBUG_CLIENTRY(MUTE),
  #endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

// Table for Nptv command entry
static CLI_EXEC_T arNptvCmdTbl[] = {
#ifdef CC_CLI
  {"Init",       "i",    _NptvInitCmd,     NULL,           "Initialize Nptv",  CLI_SUPERVISOR},
  {"MainSrc",  "ms",   _NptvMainSrcSelCmd,   NULL,           "Main Source Select",    CLI_SUPERVISOR},
  {"PipSrc",  "ps",   _NptvPipSrcSelCmd,   NULL,           "Pip Source Select",    CLI_SUPERVISOR},
  {"FwOnoff",       "fw",    _NptvFwOnOff,     NULL,           "Nptv fimware on/off",  CLI_SUPERVISOR},
  {"FixColorSpace",  "fixcolor",   _NptvFixColorSpaceCmd,   NULL, 	  "Fix color space",	 CLI_SUPERVISOR},
  {"SetScanMode",  "scan",   _NptvSetScanModeCmd,   NULL, 	  "Set Scan Mode",	 CLI_SUPERVISOR},
  {"SetDemoMode",  "demo",   _NptvDemoModeCmd,   NULL, 	  "Set Demo Mode",	 CLI_GUEST},

  #ifdef CC_DRV_SUPPORT_SMICRO
  {"SMicro",  "sm",   NULL, arSMicroCmdTbl,   "SMicro",	 CLI_GUEST},
  #endif
  {"Scpos",      "scl",  NULL,             arScposCmdTbl,  "Scpos command",    CLI_GUEST},
  {"Decoder",    "dec",  NULL,             arDecCmdTbl,    "Decoder Command",  CLI_SUPERVISOR},
  {"HDMI",    "hdmi",  NULL,             arHDMICmdTbl,    "HDMI Command",  CLI_SUPERVISOR},
  {"HVPos",    "hv",  NULL,             arHVCmdTbl,    "H/V position Command",  CLI_SUPERVISOR},
  {"DVI",    "dvi",  NULL,             arDVICmdTbl,    "DVI Command",  CLI_SUPERVISOR},
  {"Ycproc",    "ycproc",  NULL,             arYcprocCmdTbl,    "YCPROC Command",  CLI_GUEST},
  {"MDDi",    "di",  NULL,             arYcprocDiTbl,    "MDDi Command",  CLI_SUPERVISOR},
  {"MJC",    "mjc",  NULL,             arYcprocMjcTbl,    "MJC Command",  CLI_SUPERVISOR},
  {"vga",    "vga",  NULL,             arVgaCmdTbl,    "VGA Command", CLI_GUEST}, /* add vga*/
  {"Teletext",	"ttx", NULL,	arTTXCmdTbl,"Teletext Command", CLI_SUPERVISOR}, /* Teletext added by Pibben at 2006 5 30 */
  {"VBI",    "vbi",  NULL,             arVbiCmdTbl,    "VBI Command", CLI_GUEST},
  {"TVD",    "tvd",  NULL,             arTvdCmdTbl,    "TVD Command", CLI_GUEST},
  {"TDC",    "tdc",  NULL,             arTdcCmdTbl,    "TDC Command", CLI_GUEST},
  {"NR",    "nr",  NULL,             arNRCmdTbl,    "NR Command", CLI_GUEST},
  {"SCART",    "scart",  NULL,	arScartCmdTbl,    "SCART Command", CLI_GUEST},
  {"Mute",    "mute",  NULL, 	arMuteCmdTbl,    "Mute Command", CLI_GUEST},
  {"PatternGen",    "pt",  NULL, 	arPatgenCmdTbl,    "Video pattern generator Command", CLI_GUEST},
  {"Bypass",    "byp",  NULL, 	arBypassCmdTbl,    "Video module bypass Command", CLI_GUEST},
  CLIMOD_DEBUG_CLIENTRY(NPTV),
  #endif
  {"VFE",    "vfe",  NULL,             arVfeCmdTbl,    "VFE Command",  CLI_SUPERVISOR},
  {"dram_dump", "dd", _NptvDevDumpCmd, NULL, "NPTV dram dump", CLI_GUEST },
  {"scaler_dd", "scaler_dd", _NptvDevVideoDumpCmd, NULL, "NPTV scaler video dram dump", CLI_GUEST },  
  {"fw_upgrade", "fw_upgrade", _NptvFWUpgradeCmd, NULL, "Firmware Upgrade", CLI_GUEST },  
  {"status", "ss", _NptvStatusCmd, NULL, "NPTV Status", CLI_SUPERVISOR },
#ifndef NDEBUG 
#ifdef CC_FLIP_MIRROR_SUPPORT
  {"flip_mirror", "fm", _NptvSetFlipMirror, NULL, "Set Flip Mirror", CLI_SUPERVISOR },
#endif
#ifndef CC_DRIVER_PROGRAM
  {"mw_dbg", "mw_dbg", _NptvMwDebug, NULL, "Set Mw if debug", CLI_SUPERVISOR },
#endif
#endif //NDEBUG
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

// Table for top level nptv cli interface
CLI_MAIN_COMMAND_ITEM(Nptv)
{
	"nptv",
	"n",
	NULL,
	arNptvCmdTbl,
	"Nptv command",
	CLI_GUEST
};

#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/

extern BOOL VdoMLInit(void);

static INT32 _NptvInitCmd (INT32 i4Argc, const CHAR **szArgv)
{
  UNUSED(i4Argc);
  UNUSED(szArgv);

  VdoMLInit();

  Printf("Nptv Initialized!");

  return 0;
}


struct VideoInputNameMap {
	char * shortname;
	UINT8 vss_id;
};
static  struct VideoInputNameMap  cli_vdoinputmap[]=
{
	{ "tv",SV_VS_TUNER1 },
	{ "atd1",SV_VS_ATD1},
	{ "av1",SV_VS_CVBS1 },
	{ "av2",SV_VS_CVBS2 },
	{ "av3",SV_VS_CVBS3 },
	{ "av4",SV_VS_CVBS4 },
	{ "sv1",SV_VS_S1 },
	{ "sv2",SV_VS_S2 },
	{ "sv3",SV_VS_S3},
	{ "dvd",SV_VS_YPbPr3}, /* alias to SV_VS_YPbPr3 */
	{ "ypbpr1",SV_VS_YPbPr1},
	{ "ypbpr2",SV_VS_YPbPr2},
	{ "ypbpr3",SV_VS_YPbPr3},	/*DVD */
	{ "ypbpr4",SV_VS_YPbPr4},	/*vga componet */
	{ "vgacomp",SV_VS_VGACOMP1},
	{ "vga",SV_VS_VGA},
	{ "dtd1",SV_VS_DTD1},
	{ "dtv1",SV_VS_DT1},
	{ "dtv2",SV_VS_DT2},
	{ "hdmi1",SV_VS_HDMI1},
	{ "hdmi2",SV_VS_HDMI2},
	{ "hdmi3",SV_VS_HDMI3},
	{ "hdmi4",SV_VS_HDMI4},
	{ "scart0",SV_VS_SCART},
	{ "scart1",SV_VS_SCART1},
	{ "scart2",SV_VS_SCART2},
	{ "off",SV_VS_MAX}
};

#define VDOSHORTNAME_NR (sizeof(cli_vdoinputmap)/sizeof(struct VideoInputNameMap))


static INT32 _NptvSrcSel(UINT8 path, INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 i;
	UINT8 u1Ret = 0;
	char * pathname;
	UINT8 srcnew;

	if(!path)
	{
		pathname="main";
		srcnew=_bSrcMainNew;
	}
	else
	{
		pathname="pip";
		srcnew=_bSrcSubNew;
		path=1;
	}

	//Printf("%d Arguments", i4Argc);
	if(i4Argc <= 2)
	{
		if(i4Argc==1)
		{
			for (i=0;i<VDOSHORTNAME_NR;i++)
			{
				if( srcnew == cli_vdoinputmap[i].vss_id)
					break;
			}
			if(i >= VDOSHORTNAME_NR)
            {
				i = VDOSHORTNAME_NR - 1;
            }
			Printf("Current %s input: [%s]\n", pathname, cli_vdoinputmap[i].shortname);
			return 0;
		}

		for (i=0;i<VDOSHORTNAME_NR;i++)
		{
			if(x_strcmp(szArgv[1], cli_vdoinputmap[i].shortname) == 0)
			{
				u1Ret = bApiVideoSetVideoSrc(path, cli_vdoinputmap[i].vss_id);
				break;
			}
		}
		if(u1Ret)
		{
			Printf("Successful to select %s to [%s]\n", pathname, szArgv[1]);
			return 0;
		}
		else
		{
			Printf("Fail to select %s to [%s]\n", pathname, szArgv[1]);
		}
	}

	Printf("Usage: %s <", szArgv[0]);
	for (i=0;i<VDOSHORTNAME_NR;i++)
	{
		Printf("%s ",cli_vdoinputmap[i].shortname);
		if((i&0xf)==0xf) Printf("\n\t");
	}
		Printf(">\n");
	return 0;

}


static INT32 _NptvMainSrcSelCmd(INT32 i4Argc, const CHAR **szArgv)
{
	return _NptvSrcSel(0,i4Argc,szArgv);
}

static INT32 _NptvPipSrcSelCmd(INT32 i4Argc, const CHAR **szArgv)
{
	return _NptvSrcSel(1,i4Argc,szArgv);
}

#if 0
static void _NptvCheckMuteMask(UINT32 u4MuteMask)
{
    if ((u4MuteMask & CC_MUTE_MASK_FORCE))
    {
        Printf("Mutemask: CC_MUTE_MASK_FORCE => Mute by AP\n");
    }
    if ((u4MuteMask & CC_MUTE_MASK_BLUE) >> 1)
    {
        Printf("Mutemask: CC_MUTE_MASK_BLUE => Mute by AP\n");
    }
    if ((u4MuteMask & CC_MUTE_MASK_BLACK) >> 2)
    {
        Printf("Mutemask: CC_MUTE_MASK_BLACK => Mute by AP\n");
    }
    if ((u4MuteMask & CC_MUTE_MASK_MDDI01) >> 3)
    {
        Printf("Mutemask: CC_MUTE_MASK_MDDI01 => Mute by DI\n");
    }
    if ((u4MuteMask & CC_MUTE_MASK_SCPOS01) >> 4)
    {
        Printf("Mutemask: CC_MUTE_MASK_SCPOS01 => Mute by SCPOS\n");
    }
    if ((u4MuteMask & CC_MUTE_MASK_SCPOS02) >> 5)
    {
        Printf("Mutemask: CC_MUTE_MASK_SCPOS02 => Mute by SCPOS\n");
    }
    if ((u4MuteMask & CC_MUTE_MASK_SCPOS03) >> 6)
    {
        Printf("Mutemask: CC_MUTE_MASK_SCPOS03 => Mute by SCPOS\n");
    }
    if ((u4MuteMask & CC_MUTE_MASK_SCPOS04) >> 7)
    {
        Printf("Mutemask: CC_MUTE_MASK_SCPOS04 => Mute by SCPOS\n");
    }
    if ((u4MuteMask & CC_MUTE_MASK_MODECHG) >> 8)
    {
        Printf("Mutemask: CC_MUTE_MASK_MODECHG => Mute by Nptv driver\n");
    }
    if ((u4MuteMask & CC_MUTE_MASK_SRM) >> 9)
    {
        Printf("Mutemask: CC_MUTE_MASK_SRM => Mute by SRM\n");
    }
    if ((u4MuteMask & CC_MUTE_MASK_DTV) >> 10)
    {
        Printf("Mutemask: CC_MUTE_MASK_DTV => Mute by DTV\n");
    }
    if ((u4MuteMask & CC_MUTE_MASK_HDMI) >> 11)
    {
        Printf("Mutemask: CC_MUTE_MASK_HDMI => Mute by HDMI\n");
    }
}
#endif

EXTERN UINT32 _u4MainMuteMask;
EXTERN UINT32 _u4PipMuteMask;
static INT32 _NptvMuteQuery(INT32 i4Argc, const CHAR **szArgv)
{
	UINT32 u4VdpId;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: ucVdpId\n");
		return 0;
	}
	u4VdpId = (UINT32)StrToInt(szArgv[1]);

	if ((u4VdpId != SV_VP_MAIN) && (u4VdpId != SV_VP_PIP))
	{
		Printf("Invalid ucVdpId\n");
		return 0;
	}

    _vDrvDumpVideoMuteModule(u4VdpId, 1);
    return 0;
}

static INT32 _NptvMuteCmd(INT32 i4Argc, const CHAR **szArgv)
{
	UCHAR ucVdpId;
	UCHAR ucEn;
	UINT8 MuteR;
	UINT8 MuteG;
	UINT8 MuteB;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucVdpId u4En\n");
		return 0;
	}
	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
	ucEn = (UCHAR) StrToInt(szArgv[2]);

        if (i4Argc >= 4)
        {
        	MuteR = (UCHAR) StrToInt(szArgv[3]);
         	MuteG = (UCHAR) StrToInt(szArgv[4]);
        	MuteB = (UCHAR) StrToInt(szArgv[5]);
        	if (!ucVdpId)
        	{
                        vRegWriteFldAlign(OUTSTG_OS_53, MuteR, OS_53_R_MUTE_MAIN);
                        vRegWriteFldAlign(OUTSTG_OS_52, MuteG, OS_52_G_MUTE_MAIN);
                        vRegWriteFldAlign(OUTSTG_OS_52, MuteB, OS_52_B_MUTE_MAIN);
        	}
        	else
        	{
        	          #if SUPPORT_POP
                        vRegWriteFldAlign(OUTSTG_OS_54, MuteR, OS_54_R_MUTE_PIP);
                        vRegWriteFldAlign(OUTSTG_OS_54, MuteG, OS_54_G_MUTE_PIP);
                        vRegWriteFldAlign(OUTSTG_OS_53, MuteB, OS_53_B_MUTE_PIP);
                        #endif
        	}
       }

	if (!ucVdpId)
	{
	    vDrvMainMute(ucEn);
	}
	else
	{
	    vDrvPIPMute(ucEn);
	}

	if (!ucEn)
	{
        	if (!ucVdpId)
        	{
                    	    vRegWriteFldAlign(OUTSTG_OS_53, 0, OS_53_R_MUTE_MAIN);
                    	    vRegWriteFldAlign(OUTSTG_OS_52, 0, OS_52_G_MUTE_MAIN);
                    	    vRegWriteFldAlign(OUTSTG_OS_52, 0, OS_52_B_MUTE_MAIN);
                	_u4MainMuteMask = 0x0;
               }
               else
               {
        	          #if SUPPORT_POP
                    	    vRegWriteFldAlign(OUTSTG_OS_54, 0, OS_54_R_MUTE_PIP);
                    	    vRegWriteFldAlign(OUTSTG_OS_54, 0, OS_54_G_MUTE_PIP);
                    	    vRegWriteFldAlign(OUTSTG_OS_53, 0, OS_53_B_MUTE_PIP);
                	    _u4PipMuteMask = 0x0;
                    	    #endif
               }
	}
	return 0;
}

static INT32 _NptvMuteOnOff(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 ucVdpId;
	UINT32 u4En;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucVdpId u4En\n");
		return 0;
	}
	ucVdpId = (UINT8) StrToInt(szArgv[1]);
	u4En = (UINT32) StrToInt(szArgv[2]);

	//vScposMuteMaskOnOff(ucVdpId, u4En);
	_vDrvVideoForceUnMute((UINT32)ucVdpId, (BOOL)u4En);

	return 0;
}

#ifdef CC_BOOT_MUTE_LOG
static INT32 _NptvMuteBootLog(INT32 i4Argc, const CHAR **szArgv)
{
	UINT32 u4En;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL) )
	{
		Printf("Arg: u4Enable\n");
		return 0;
	}
	u4En = (UINT32) StrToInt(szArgv[1]);
	u4En = (u4En>0) ? 1 : 0;

	if(fgApiEepromWriteByte(EEP_BOOT_MUTE_LOG, u4En))
	{
	    Printf("Success : Change mute log when system boot up\n");
	}
	else
	{
	    Printf("Fail : Cannot change mute log when system boot up\n");
	}
	return 0;
}
#endif

#define SV_LEFT     1
#define SV_RIGHT    0
static INT32 _NptvDemoModeCmd(INT32 i4Argc, const CHAR **szArgv)
{
    EXTERN void vApiVideoDemoMode(UINT8 bmode, Region DemoRec, UINT8 bDEMOModules);
    Region Test;
    UINT8 bModules;   
    if(i4Argc != 6)
	{
        Printf("Usage: %s <mode> <region HStart> <region HEnd> <region VStart> <region VEnd> <Demo Modules>\n", szArgv[0]);
		return (0);
	}
    Test.u1OnOff = SV_ON;
    Test.wHEnd = (UINT16) StrToInt(szArgv[3]);
    Test.wHStart = (UINT16) StrToInt(szArgv[2]);
    Test.wVEnd = (UINT16) StrToInt(szArgv[5]);
    Test.wVStart = (UINT16) StrToInt(szArgv[4]);   
    bModules = (UINT8) StrToInt(szArgv[6]);  
    vApiVideoDemoMode((UINT16)StrToInt(szArgv[1]), Test, bModules);
    return 0;
}


void vApiVideoFixColorSpace(UINT8 bPath,UINT8 bMode);
extern UINT8 _bFixColorSpace[2];

static INT32 _NptvFixColorSpaceCmd(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc != 3)
	{
	  Printf("Usage: %s <main/pip 0/1> <color mode >\n", szArgv[0]);
	  Printf("mode: (0)auto (1)RGB_444 (2)RGB_422(3)YCbCr(4)YCBCRtoRGB(5)RGBtoYCBCR\n"
	           "     (17)RGB 444 once (18)RGB 422 once (19)YCbCr(20)YCBCRtoRGB once(21)RGBtoYCBCRonce\n");
	}
	else
	{
		vApiVideoFixColorSpace((UINT16)StrToInt(szArgv[1]),(UINT16)StrToInt(szArgv[2]));
		Printf("fix(force) %s to colorspace mode to %d\n", (UINT16)StrToInt(szArgv[1]),(UINT16)StrToInt(szArgv[2]));
	}
	Printf("_bFixColorSpace[2]={0x%02x,0x%02x}\n",_bFixColorSpace[0],_bFixColorSpace[1]);
	return (0);
}

static INT32 _NptvSetScanModeCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Path = 0;
    UINT32 u4Mode = 0;
    
    if (i4Argc < 3)
    {
        Printf("Usage: %s <main/pip 0/1> <scan mode >\n", szArgv[0]);    
        if (i4Argc > 1)
        {
            Printf("Current Mode: %d\n", u4ApiVideoGetScanMode((UINT32)StrToInt(szArgv[1])));            
        }
        return 0;
    }
    u4Path = (UINT32)StrToInt(szArgv[1]);
    u4Mode = (UINT32)StrToInt(szArgv[2]);

    vApiVideoSetScanMode(u4Path, u4Mode);    

    return 0;
}

//UINT8 bApiVideoSetDispPosSize(UINT8 bPath, UINT16 wXOff, UINT16 wYOff, UINT16 wWidth, UINT16 wHeight)
#if 0
static INT32 _ScposSetDispSizePosCmd(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 u1Path;

  if(i4Argc != 6)
  {
    Printf("Usage: %s <main/pip> <XOffset> <YOffset> <Width> <Height>\n", szArgv[0]);
    return (0);
  }

  if((x_strcmp(szArgv[1], "m") == 0) || (x_strcmp(szArgv[1], "main") == 0))
  {
    u1Path = SV_VP_MAIN;
  }
  else
  {
    u1Path = SV_VP_PIP;
  }

  bApiVideoSetDispPosSize(u1Path, (UINT16)StrToInt(szArgv[2])
                                , (UINT16)StrToInt(szArgv[3])
                                , (UINT16)StrToInt(szArgv[4])
                                , (UINT16)StrToInt(szArgv[5]));

  bApiVideoSetPathDispInfo(u1Path, (UINT16)StrToInt(szArgv[2])
                                , (UINT16)StrToInt(szArgv[3])
                                , (UINT16)StrToInt(szArgv[4])
                                , (UINT16)StrToInt(szArgv[5]));



  return 0;
}


//UINT8 bApiVideoSetSrcPosSize(UINT8 bPath, UINT16 wXOff, UINT16 wYOff, UINT16 wWidth, UINT16 wHeight)
static INT32 _ScposSetSrcSizePosCmd(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 u1Path;

  if(i4Argc != 6)
  {
    Printf("Usage: %s <main/pip> <XOffset> <YOffset> <Width> <Height>\n", szArgv[0]);

  }

  if((x_strcmp(szArgv[1], "m") == 0) || (x_strcmp(szArgv[1], "main") == 0))
  {
    u1Path = SV_VP_MAIN;
  }
  else
  {
    u1Path = SV_VP_PIP;
  }

  bApiVideoSetSrcPosSize(u1Path, (UINT16)StrToInt(szArgv[2])
                                , (UINT16)StrToInt(szArgv[3])
                                , (UINT16)StrToInt(szArgv[4])
                                , (UINT16)StrToInt(szArgv[5]));


  return 0;
}
#endif
extern UINT32 _u4BypassNptvMloop ;
static INT32 _NptvFwOnOff (INT32 i4Argc, const CHAR **szArgv)
{
		if(i4Argc==2)
		{
			_u4BypassNptvMloop = StrToInt(szArgv[1]);
		}
		else
		{
			Printf("BYPASS_NPTV_All		(1 << 0)\n");
			Printf("BYPASS_NPTV_NR		(1 << 1) 0x2\n");
			Printf("BYPASS_NPTV_HDMI	(1 << 2)0x4\n");
			Printf("BYPASS_NPTV_SCPOS	(1 << 3)0x8\n");
			Printf("BYPASS_NPTV_PSCAN	(1 << 4)0x10\n");
			Printf("BYPASS_NPTV_TVD		(1 << 5)0x20\n");
			Printf("BYPASS_NPTV_DDDS	(1 << 6)0x40\n");
			Printf("BYPASS_NPTV_CEC		(1 << 7)0x80\n");
			Printf("BYPASS_NPTV_TVE		(1 << 8)0x100\n");
			Printf("BYPASS_NPTV_SCART	(1 << 9)0x200\n");
			Printf("BYPASS_OUTPUT_ISR	(1 << 10)0x400\n");	
			Printf("BYPASS_INPUT_ISR	(1 << 11)0x800\n");	
			Printf("BYPASS_COLOR_PROC	(1 << 12)0x1000 \n");	
			Printf("BYPASS_NSD_PROC		(1 << 13)0x2000\n");	
			Printf("\nBypassNptvMloop: 0x%X\n", _u4BypassNptvMloop);
		}

    return 0;
}

static INT32 _NptvStatusCmd(INT32 i4Argc, const CHAR **szArgv)
{
#ifdef COUNTRY_FROM_EEPROM 
	Printf("EEPROM_COUNTRY_OFFSET1 0x%x\n", DRVCUST_OptGet(eCountryEEPROMOffset1));
	Printf("EEPROM_COUNTRY_OFFSET2 0x%x\n", DRVCUST_OptGet(eCountryEEPROMOffset2));
	Printf("EEPROM_COUNTRY_OFFSET3 0x%x\n", DRVCUST_OptGet(eCountryEEPROMOffset3));
#endif
	Printf("nptv size =%d\n", DRVCUST_OptGet(eEepromNptvDriverSize));
	Printf("nptv start =%d\n", DRVCUST_OptGet(eEepromNptvDriverOffset));
	Printf("EEP_ADC_START=%d\n", EEP_ADC_START);
	Printf("EEP_VIDEO_AUTO_COLOR_START=%d\n", EEP_VIDEO_AUTO_COLOR_START);
	Printf("EEP_VGA_USR_START=%d\n", EEP_VGA_USR_START);
	Printf("EEP_CEC_LOG_ADDR=%d\n", EEP_CEC_LOG_ADDR);
#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT
	Printf("EEP_VGA_AMBIGUOUS_DEFAULT_START=%d\n", EEP_VGA_AMBIGUOUS_DEFAULT_START);
#endif
	Printf("EEP_GEN_SET_END=%d\n", EEP_GEN_SET_END);
	return 0;
}

#ifndef NDEBUG 
#ifdef CC_FLIP_MIRROR_SUPPORT
static INT32 _NptvSetFlipMirror(INT32 i4Argc, const CHAR **szArgv)
{
		DTVCFG_T rDtvCfg;

		if(i4Argc==3)
		{
			if(EEPDTV_GetCfg(&rDtvCfg))
			{
				Printf(" Read Dtv config fail\n");
				return 0 ;
			}

			if(StrToInt(szArgv[1]) == 0)
			{
				if(StrToInt(szArgv[2]))
				{
					rDtvCfg.u1Flags2 |= (DTVCFG_FLAG2_FLIP_ON);	
				}
				else
				{
					rDtvCfg.u1Flags2 &= ~(DTVCFG_FLAG2_FLIP_ON);	
				}
				if(EEPDTV_SetCfg(&rDtvCfg))
				{
					Printf("set Dtv config fail\n");
				}
			}
			else if(StrToInt(szArgv[1]) == 1)
			{
				if(StrToInt(szArgv[2]))
				{
					rDtvCfg.u1Flags2 |= (DTVCFG_FLAG2_MIRROR_ON);	
				}
				else
				{
					rDtvCfg.u1Flags2 &= ~(DTVCFG_FLAG2_MIRROR_ON);	
				}

				if(EEPDTV_SetCfg(&rDtvCfg))
				{
					Printf("set Dtv config fail\n");
				}
			}
			else
			{
				Printf("flip_mirror flip/mirror(0/1)  enable/disable(1/0)\n");
			}
		}
		else
		{
			Printf("flip_mirror flip/mirror(0/1)  enable/disable(1/0)\n");
		}

    return 0;
}
#endif
#ifndef CC_DRIVER_PROGRAM
extern UINT32 _u4MwIfDbgFlags ;
static INT32 _NptvMwDebug(INT32 i4Argc, const CHAR **szArgv)
{
    if(i4Argc==2)
    {
       _u4MwIfDbgFlags = StrToInt(szArgv[1]) ;
        
    }
    else
    {
		Printf("Current Val:0x%x\n", _u4MwIfDbgFlags);
        Printf("VDEC : 0x8\n");
		Printf("VDP : 0x10\n");
		Printf("SRCR : 0x20\n");
		Printf("SRCR : 0x40\n");
    }
    
    return 0;
}
#endif //#ifndef CC_DRIVER_PROGRAM
#endif


//============================================================================
// NPTV Tool Command
//============================================================================
#define USB_EEPROM_MAGIC 0x73

static INT32 _NptvDevDumpCmd(INT32 i4Argc, const CHAR **szArgv)
{
    if(i4Argc < 2)
    {     	
    	LOG(0, "n.dd [on/off]\n");
    	if(bApiEepromReadByte(EEP_MUSB_ENABLE) == USB_EEPROM_MAGIC)
    	{
    		LOG(0, "USB is in device mode\n");
    	}else
    	{
    		LOG(0, "USB is in host mode\n");
    	}    	
		return 0;
    }else
    {
		if(x_strcmp(szArgv[1], "off") == 0)
		{
			vSetUSBDump(SV_FALSE);
			fgApiEepromWriteByte(EEP_MUSB_ENABLE, 0xFF);	
		}else
		{
			fgApiEepromWriteByte(EEP_MUSB_ENABLE, USB_EEPROM_MAGIC);			
			vSetUSBDump(SV_TRUE);				
		}	
    } 
    return 0;
}

static INT32 _NptvDevVideoDumpCmd(INT32 i4Argc, const CHAR **szArgv)
{	
    if(i4Argc < 2)
    {
    	LOG(0, "scaler_dd on|off|start [#number] [half=0/1]\n");
    	LOG(0, "scaler_dd add address size\n");    	
    }else
    {
		NptvScalerVideoDump(i4Argc, szArgv);  
    }

    return 0;
}

static INT32 _NptvFWUpgradeCmd(INT32 i4Argc, const CHAR **szArgv)
{
	static UINT8 *pbData;
	static UINT32 dwBlkSize;
	static UINT32 dwAddr;
	UINT32 dwFlashReadAddr;	
	INT8 iRetVal;
	
    if(i4Argc < 2)
    {
    	LOG(0, "fw_upgrade [on/off/upgrade] [dram_addr]\n");
		return 0;
    }else
    {
		if(x_strcmp(szArgv[1], "on") == 0)
		{
    		dwBlkSize = FlashGetBlkSize();
    		pbData = x_mem_alloc(0x1000000);
    		dwAddr = 0;
    		
			// disable ISR
			vRegWrite4B(0x240c, 0xFFFFFFFF);
			// disable watchdog
			vRegWrite4B(0x8000, 0x0);
			
    		LOG(0, "Address: 0x%x, BlkSize: 0x%x\n", pbData, dwBlkSize);
		}else if(x_strcmp(szArgv[1], "off") == 0)
		{
    		dwAddr = 0;		
			x_mem_free(pbData);
		}else if(i4Argc >= 3 && !x_strcmp(szArgv[1], "upgrade"))
    	{
    		dwFlashReadAddr = StrToInt(szArgv[2]);
			LOG(0, "Flash Write Address 0x%x, MemPtr 0x%x, BlkSize 0x%x\n", 
					dwAddr, dwFlashReadAddr, dwBlkSize);

			iRetVal = FlashWrite(dwAddr, dwFlashReadAddr, dwBlkSize);
			if(iRetVal != 0)
			{    			
				LOG(0, "Flash Write Failed\n");
				return -1;
			}			
			dwAddr += dwBlkSize;
    	}    
    }				
	return 0;
}

//============================================================================
// End of NPTV Tool Command
//============================================================================


#endif /* CC_CLI */

