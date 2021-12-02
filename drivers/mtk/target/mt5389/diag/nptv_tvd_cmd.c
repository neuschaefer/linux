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
 * $RCSfile: nptv_tvd_cmd.c,v $
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
#include "nptv_drvif.h"
#include "sv_const.h"
#include "drv_tvd.h"
#include "eepdef.h"
#include "eeprom_if.h"
#include "api_eep.h"
#include "drv_vbi.h"

#ifndef CC_CLI
#define CC_CLI
#endif

#ifdef CC_CLI
static INT32 _vDRVTVDBlankLevel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDRVDFEAGCLevel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDRVCAGCLevel(INT32 i4Argc, const CHAR **szArgv);
#if 0
static INT32 _TdcStart(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdcLength(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NRHeight(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NRMeter(INT32 i4Argc, const CHAR **szArgv);
#endif
static INT32 _vDRVTVDHW(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDRVTVDAVdelay(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDRVTVDAGCPedEn(INT32 i4Argc, const CHAR **szArgv);
#if TVD_AUTO_CALIB
static INT32 _vDRVTVDAdcCalib(INT32 i4Argc, const CHAR **szArgv);
#endif
static INT32 _vDRVTVDSetFixAgc(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDRVTVDSetAGain(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDRVTVDSetDGain(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDRVTVDSetCAGC(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDRVTVDQueryCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDRVTVDSetColorSystem(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDRVTVDGetColorSystem(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDRVTVDSetEnabledColorSystem(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDRVTVDSignalStrengthLevel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDRVTVDCtrl(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDRVTVDOverSampleEnable(INT32 i4Argc, const CHAR **szArgv);
extern void vDrvTVDBlankLevel(UINT16 wTARGET_BLANK);
extern void vTvdDFEAGCTarget(UINT16 bTarget);
extern void vTVDCAGCTARGET(INT8 bOffset);
static INT32 _vDrvTvdFastDump(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDrvTVDQtest(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDrvTVDSetHNChChgMCnt(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDrvTVDEnablePhaltWa(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDrvSVFBypass(INT32 i4Argc, const CHAR **szArgv);
#if TVD_NR_AVG
static INT32 _vDrvTVDSetNRTable(INT32 i4Argc, const CHAR **szArgv);
static INT32 _vDrvTVDEnableDumpNR(INT32 i4Argc, const CHAR **szArgv);
#endif

//extern void vTVDAVDelay(UINT16 bDelay);

#endif /*#ifdef CC_CLI*/
/******************************************************************************
* CLI Command Table
******************************************************************************/
#ifdef CC_CLI
CLIMOD_DEBUG_FUNCTIONS(TVD)
#endif


// Table for TVD command entry Mark 0906
CLI_EXEC_T arTvdCmdTbl[] = {
#ifdef CC_CLI
  {"TVD_Blank_Level",  "blank",   _vDRVTVDBlankLevel,   NULL,  "TVD Blank Level",    CLI_SUPERVISOR},
  {"VGA_Target",  "vga_level",   _vDRVDFEAGCLevel,   NULL,  "Front End VGA target Level",    CLI_SUPERVISOR},
  {"CAGC_Target", "cagc_level",  _vDRVCAGCLevel,   NULL,  "CAGC range(0xB7(-0x49)~0x89)",    CLI_SUPERVISOR},
  {"TVD_HeightWidth", "tvd_hw",  _vDRVTVDHW,   NULL,  "TVD output height and width",    CLI_SUPERVISOR},
  {"TVD_AVDelay", "av_delay",  _vDRVTVDAVdelay,   NULL,  "TVD AV Delay (HPorch)",    CLI_GUEST},
  {"TVD_AGCPedEn", "p",  _vDRVTVDAGCPedEn,   NULL,  "AGCPedEn",    CLI_GUEST},
#if TVD_AUTO_CALIB
  {"TVD_AdcCalib", "cal",  _vDRVTVDAdcCalib,   NULL,  "AdcCalib",    CLI_GUEST},
#endif
  {"TVD_AdcFixAgc", "f",  _vDRVTVDSetFixAgc,   NULL,  "AdcFixAgc",    CLI_GUEST},
  {"TVD_AdcAGain", "again",  _vDRVTVDSetAGain,   NULL,  "AdcAGain",    CLI_SUPERVISOR},
  {"TVD_AdcDGain", "dgain",  _vDRVTVDSetDGain,   NULL,  "AdcDGain",    CLI_SUPERVISOR},
  {"TVD_AdcSetCagc", "cagc",  _vDRVTVDSetCAGC,   NULL,  "AdcSetCagc",    CLI_SUPERVISOR},
  {"TVD_query",		"q",	_vDRVTVDQueryCmd, NULL,			"TVD status query", CLI_GUEST},
  {"TVD_SetColorSystem",	"sc",	_vDRVTVDSetColorSystem, NULL,"TVD set color system", CLI_GUEST},
  {"TVD_GetColorSystem",	"gc",	_vDRVTVDGetColorSystem, NULL,"TVD get color system", CLI_GUEST},
  {"TVD_StrengthLevel",	"sl",	_vDRVTVDSignalStrengthLevel, NULL,"TVD Signal Strength Level", CLI_GUEST},
  {"TVD_OversampleEnable",	"os",	_vDRVTVDOverSampleEnable, NULL,"TVD Over Sample Enable", CLI_SUPERVISOR},
  {"ctl","c",_vDRVTVDCtrl, NULL,"Tvd Ctrl", CLI_GUEST},
  {"Tvd_FastDump","fdump",_vDrvTvdFastDump, NULL,"Tvd Fast Dump", CLI_GUEST},
  {"Tvd_QlabTest","qtest",_vDrvTVDQtest, NULL,"Qlat Auto Test", CLI_GUEST},
  {"Tvd_SetHNChChgCnt","hncnt",_vDrvTVDSetHNChChgMCnt, NULL,"HighNoise MCnt", CLI_GUEST},
  {"Tvd_EnablePhaltWa","phaltwa",_vDrvTVDEnablePhaltWa, NULL,"Enable Phalt WA", CLI_GUEST},
  {"Tvd_SVFbypass","svf",_vDrvSVFBypass, NULL,"S-First Function Bypass", CLI_GUEST},
#if TVD_NR_AVG  
  {"Tvd_SetNRTable","NrTable",_vDrvTVDSetNRTable, NULL,"Set NR Table", CLI_GUEST},
  {"Tvd_StartNR","StartNR",_vDrvTVDEnableDumpNR, NULL,"Start to Dump NR", CLI_GUEST},
#endif  
  {"TVD_SetEnabledColorSystem",	"encs",	_vDRVTVDSetEnabledColorSystem, NULL,"TVD set enabled color system", CLI_GUEST},
   CLIMOD_DEBUG_CLIENTRY(TVD),

#endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/

static INT32 _vDRVTVDBlankLevel(INT32 i4Argc, const CHAR **szArgv)
{
  if (i4Argc != 2)
  {
    Printf("Usage: blank blanklevel(default 256)\n");
    return 0;
  }

//  vDrvTVDBlankLevel((UINT16)StrToInt(szArgv[1]));

  return 0;
}

static INT32 _vDRVDFEAGCLevel(INT32 i4Argc, const CHAR **szArgv)
{
  if (i4Argc != 2)
  {
    Printf("Usage: VGA Target level(default 0x33)\n");
    return 0;
  }

//  vTvdDFEAGCTarget((UINT16)StrToInt(szArgv[1]));

  return 0;
}

static INT32 _vDRVCAGCLevel(INT32 i4Argc, const CHAR **szArgv)
{
  if (i4Argc != 2)
  {
    Printf("Usage:CAGC Target level(Range:0xB7(-0x49)~0x89)\n");
    return 0;
  }

//  vTVDCAGCTARGET((INT8)StrToInt(szArgv[1]));

  return 0;
}

static INT32 _vDRVTVDHW(INT32 i4Argc, const CHAR **szArgv)
{
  Printf("Height = %d\n", wTvd3dHeight() );
  Printf("Width = %d\n", 720);

  return 0;
}

static INT32 _vDRVTVDAGCPedEn(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: AGCPedEn?\n");
		return 0;
	}

	vTVD3dAGCPedEn((UINT8)StrToInt(szArgv[1]));

	Printf("Set AGC Ped_En to %d\n", (UINT8)StrToInt(szArgv[1]));
	return 0;
}
#if TVD_AUTO_CALIB
static INT32 _vDRVTVDAdcCalib(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 bmode;
	UINT8 btmp;
	UINT16 wYlevel;
	if ((i4Argc != 2)&&(i4Argc != 3))
	{
		Printf("Usage: AdcCalib?\n");
		Printf("1: Adc Calib + Fix Agc\n");
		Printf("2: Adc Calib + not Fix Agc\n");
		Printf("3: Clean calibration flag\n");
		Printf("4: Check calibration flag\n");
		Printf("5: read Max Ylevel\n");
		Printf("6: Bypass E-Fuse ADC Gain \n");
		Printf("   0 => Allow  E-Fuse ADC gain \n");
		Printf("   1 => Bypass E-Fuse ADC gain \n");
//		Printf("7: Set Y level support 16~216\n");
		return 0;
	}

	bmode = (UINT8) StrToInt(szArgv[1]);
	btmp = (UINT8) StrToInt(szArgv[2]);

	if(bmode == 1)
	{
		Printf("Adc Calib Start\n");
		vDrvTVDAutoCalibStart();
	}
	else if(bmode == 2)
	{
		if(bDrvTVDAdcCalib4Auto())
		{
			Printf("Adc Calib Success\n");
		}
		else
		{
			Printf("Adc Calib Fail\n");
		}
	}
	else if(bmode == 3)
	{
		Printf("TV address = %lx\n", EEPROM_NPTV_DRIVER_OFFSET + EEP_VIDEO_AUTO_CALIB_TV_DONE);
		Printf("AV address = %lx\n", EEPROM_NPTV_DRIVER_OFFSET + EEP_VIDEO_AUTO_CALIB_AV_DONE);
		UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_DONE, 0x0));
		UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_DONE, 0x0));
	}
	else if(bmode == 4)
	{
		if(bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_TV_DONE)==0xaa)
		{
			Printf("TV ADC calibration done! \n");
		}
		else
		{
			Printf("TV not do ADC calibration yet! \n");
		}
			if(bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_AV_DONE)==0xaa)
		{
			Printf("AV ADC calibration done! \n");
		}
		else
		{
			Printf("AV not do ADC calibration yet! \n");
		}
		if(((IO32ReadFldAlign(0xf000868c, AC_FULLDW)>>29)==0x7)&&(bApiEepromReadByte(EEP_VIDEO_BYPASS_AV_EFUSE)!=0xaa))
		{
			Printf("AV use E-Fuse ADC result \n");
		}
		else
		{
			Printf("AV not E-Fuse ADC result \n");
		}
	}
	else if(bmode == 5)
	{
		wYlevel = wDrvGetTVDYLvl();
/*		switch (_rTvd3dStatus.bTvdMode)
		{
			case SV_CS_SECAM:
			case SV_CS_PAL:
			case SV_CS_PAL_60:
			case SV_CS_PAL_N:
				wYlevel = ((wYlevel+40)*1.5996)/4;
				break;
			default:
				wYlevel = ((wYlevel+41)*1.5645)/4;
				break;
		}
*/
		Printf("MAX Y level = %d \n",wYlevel);
	}
	else if(bmode == 6)
	{
		if(btmp == 1)
		{
			UNUSED(fgApiEepromWriteByte(EEP_VIDEO_BYPASS_AV_EFUSE, 0xAA));
			Printf("Bypass E-Fuse ADC gain \n");
		}
		else
		{
			UNUSED(fgApiEepromWriteByte(EEP_VIDEO_BYPASS_AV_EFUSE, 0xFF));
			Printf("Allow E-Fuse ADC gain \n");
		}
	}
/*	else if(bmode == 7)
	{
		UNUSED(fgApiEepromWriteByte(EEP_VIDEO_SUPPORT_216, btmp));
		if(btmp == 0xAA)
			Printf("Y Level support 16~216 \n");
		else
			Printf("Y Level support 16~235 \n");
	}
*/

	return 0;
}

#endif

static INT32 _vDRVTVDSetFixAgc(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 bOnOff;
	if (i4Argc != 2)
	{
		Printf("Usage: TvdFixAgc?\n");
		return 0;
	}

	bOnOff = (UINT8) StrToInt(szArgv[1]);
	vTvd3dSetFixAgc(bOnOff);
	
	return 0;
}

static INT32 _vDRVTVDSetAGain(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 TVD_AGain;
	if (i4Argc != 2)
	{
		Printf("Usage: Set Tvd Analog Gain?\n");
		return 0;
	}

	TVD_AGain = (UINT8) StrToInt(szArgv[1]);
	vDrvSetTVDADCGain(TVD_AGain);
	
	return 0;
}

static INT32 _vDRVTVDSetDGain(INT32 i4Argc, const CHAR **szArgv)
{
	UINT16 TVD_DGain;
	if (i4Argc != 2)
	{
		Printf("Usage: Set Tvd Digital Gain?\n");
		return 0;
	}

	TVD_DGain = (UINT16) StrToInt(szArgv[1]);
	vDrvSetTVDADCDGain(TVD_DGain);
	
	return 0;
}
static INT32 _vDRVTVDSetCAGC(INT32 i4Argc, const CHAR **szArgv)
{
	UINT32 u4temp;

	if (i4Argc != 9)
	{
		Printf("Usage: Set Tvd CAGC parameter?\n");
		Printf("OnOff MCAGC MID_B MID_A TARGET_B TARGET_A L_Step S_Step\n");
		return 0;
	}
	
	u4temp = (StrToInt(szArgv[3])<<24) | (StrToInt(szArgv[4])<<16) | (StrToInt(szArgv[5])<<8) |(StrToInt(szArgv[6]));  
	_rTvd3dCAGCInfo.MCAGC_EN = (UINT8) StrToInt(szArgv[1]);
	_rTvd3dCAGCInfo.MCAGC = (UINT8) StrToInt(szArgv[2]);
	_rTvd3dCAGCInfo.Bound = u4temp;
	_rTvd3dCAGCInfo.L_Step = (UINT8) StrToInt(szArgv[7]);
	_rTvd3dCAGCInfo.S_Step = (UINT8) StrToInt(szArgv[8]);
	
	vTvd3dSetCAGCbyAP();
	
	return 0;
}

static INT32 _vDRVTVDAVdelay(INT32 i4Argc, const CHAR **szArgv)
{
  if (i4Argc != 2)
  {
    Printf("Usage:TVD AV Delay (BYTE) default 0x2E \n");
    return 0;
  }

  vTVDAVDelay((UINT8)StrToInt(szArgv[1]));

  return 0;
}

static INT32 _vDRVTVDQueryCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

 vDrvTvdQueryStatus();   

  return 0;
}

static INT32 _vDRVTVDOverSampleEnable(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc==2)
	{
	    UINT8 bmode;

	    bmode = (UINT8) StrToInt(szArgv[1]);
	    vTvdSetOverSampleEnable(bmode);
	}
	if (vTvdGetOverSampleEnable())
	{
            Printf("TVD OverSample Enable\n");
	}
	else
	{
            Printf("TVD OverSample Disable\n");
	}
    return 0;
}

static struct  RTvdCtrlInfo
{
	char *name;
	char *param_range;
} rTvdCtrlCLIInfo[]=
{
	{"tvd_reset",0},
	{"reset_default",0},
	{"reconnect",0},
	{"inputtype","0:TV,1:AV,2:SV"},
	{"country","1:US,2:EU"},
	{"dftmode","0:PAL_N,1:PAL,2:PAL_M,3:NTSC,4:SECAM,5:PAL60,7:NTSC443,8:AUTO/Off"},
	{"mdstbprot","0:off,1:on"},
	{"ntsconly","0:off,1:on"},
	{"colorsystem",0},
	{"noiselvl","1:low,2:high,3:superhigh"},
	{"vcr","0:off,1:on"},
	{"coch","0:off,1:on"},
	{"ckill","0:off,1:on"},
	{"serrtype","0:off,1:on"},
	{"serrcnt","U8"},
	{"acode","U7"},
	{"dcode","U16/save MSB8"},
	{"cagc","U8"},
	{"bp_isr","0:off,1:on"},
	{"bp_modechg","0:off,1:on"},
	{"bp_mloop","0:off,1:on"},
	{"bp_tc_init","0:off,1:on"},
	{"reg_save_restore","0:restore,1:save"},
	{"peden","0:off,1:on"},
	{"mcnt","U8"},
	{"list",0},
};
static void _vDRVTVDCtr_Query(UINT8 bItem)
{
	Printf("%3d[%c]%2x:%s {%s}\n",bItem,
		bTvdCtrl(bItem,TC_GETEN,0)?'X':' ',
		bTvdCtrl(bItem,TC_GETVAL,0),
		rTvdCtrlCLIInfo[bItem].name,
		rTvdCtrlCLIInfo[bItem].param_range?rTvdCtrlCLIInfo[bItem].param_range:""
	);
}

static INT32 _vDRVTVDCtrl(INT32 i4Argc, const CHAR **szArgv)
{
	int i;
	int charcnt=0;
	if (i4Argc <= 2)
	{
		if(x_strcmp(szArgv[1], "all"))
			return 0;
		Printf("Usage: ctl item cmd <val>\n item:\n\t");
		for(i=0;i<TCTL_MAX;i++)
		{
			int itemstrlen=x_strlen(rTvdCtrlCLIInfo[i].name)+1;
			if((charcnt+itemstrlen)>=62)
			{	
				Printf("\n\t");
				charcnt=0;
			}
			Printf("%s,",rTvdCtrlCLIInfo[i].name);
			charcnt+=itemstrlen;
		}
		Printf("\n\t");
		Printf("%s\n",rTvdCtrlCLIInfo[TCTL_MAX].name);
		Printf(" cmd: 0(off) 1(on) 2(query)\n");
		return 0;
	}
	
	for(i=0;i<=TCTL_MAX;i++)
	{
		if(x_strcmp(szArgv[1],rTvdCtrlCLIInfo[i].name)==0)
		{
			if(i<TCTL_MAX)
			{	
				UINT32 CtrlArg=0;
				if(i4Argc>=3)
				{
					if(i4Argc>=4)
						CtrlArg=(UINT32)StrToInt(szArgv[3]);
					if (x_strcmp(szArgv[2],"0")==0)
						bTvdCtrl(i,TC_DISABLE,0);
					else if (x_strcmp(szArgv[2],"1")==0)
						bTvdCtrl(i,TC_ENABLE,CtrlArg);
				}
				/*
				else if (!rTvdCtrlCLIInfo[i].param_range)
					bTvdCtrl(i,TC_ENABLE,1);
                            */
				_vDRVTVDCtr_Query(i);
			}
			else
			{
				int j;
				for(j=0;j<TCTL_MAX;j++)
					_vDRVTVDCtr_Query(j);
			}
			break;
		}
	}
    return 0;
}

static INT32 _vDRVTVDSetColorSystem(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 TVD_ColorSys;
	if (i4Argc != 2)
	{
		Printf("Usage: Set Tvd Color System?\n");
		return 0;
	}
	TVD_ColorSys = (UINT8) StrToInt(szArgv[1]);
	fgApiVideoColorSystem(TVD_ColorSys);
	
    return 0;
}
static INT32 _vDRVTVDGetColorSystem(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 TVD_ColorSys,Tvd_MMode;
	if (i4Argc != 2)
	{
		Printf("Usage: Get Tvd Color System?\n");
		return 0;
	}
	TVD_ColorSys=bApiVideoGetColorSystem();
	Tvd_MMode=fgHwTvdIsMMode();
	if(Tvd_MMode)
		Printf("TVD is manual setting color system\n");
	else
		Printf("TVD is auto detect color system\n");
	Printf("TVD Color System is %d\n",TVD_ColorSys);

	
    return 0;
}

static INT32 _vDRVTVDSetEnabledColorSystem(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 TVD_ColorSys;
	if (i4Argc != 2)
	{
		Printf("Usage: Set Tvd Enabled Color System?\n");
		return 0;
	}
	TVD_ColorSys = (UINT8) StrToInt(szArgv[1]);
	vDrvTvd3dSetEnabledColorSystem(TVD_ColorSys);
	
    return 0;
}


static INT32 _vDRVTVDSignalStrengthLevel(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 bLevel;
	bLevel = bTvdSignalStrengthLevel(0);
	Printf("TVD Signal Strength Level is %d\n",bLevel);

	bLevel = bTvdSignalStrengthLevel(1);
	Printf("TVD Signal Level is %ddB\n",bLevel);
    return 0;
}

static UINT8  _fgEnableLog = 0;
static INT32 _i4LogItem = 0;
static UINT16 _u2Cnt=0;
static UINT32 _u4PreMicros=0;
static UINT32 * _pFifoStarAddr = 0;
static UINT32 _FifoSize;

/**
 * @brief Add value to Internal Array. 
 * @param  u2Value : u2Value 
 * @retval: Noon.
 */
static void vAddValue( UINT32 u4Value)
{
    UINT16 u2i;
    
    *(_pFifoStarAddr+_u2Cnt) = u4Value;    
    if (_u2Cnt == (_i4LogItem-1))
    {
        _u2Cnt = 0;
        _fgEnableLog = 0;
        _u4PreMicros = 0;
        for (u2i = 0; u2i <_i4LogItem; u2i++)
        {
            Printf("%ld\n", *(_pFifoStarAddr+u2i)); 
        }
    }
    else
    {
        _u2Cnt= _u2Cnt+1;
    }
}



/**
 * @brief Check Time Period. 
 * @param  u4TimeOut : Uint Micor Sec 
 * @retval: Ture: if Time Period over u4TimeOut.
 */
static BOOL fgTimeTriger(UINT32 u4TimeOut)
{

    UINT32 u4Micros, u4Secs;
    static UINT32 u4Diff = 0;
    INT32  i4temp;

    x_os_drv_get_time(&u4Secs, &u4Micros);

    if (_u4PreMicros == 0)
    {
        _u4PreMicros = u4Micros; 
        return 0;
    }
    else
    {
        if (u4Micros >= _u4PreMicros)
        {
            u4Diff += (u4Micros - _u4PreMicros);
        }
        else
        {
            u4Diff += (0xFFFFFFFF-_u4PreMicros) + u4Micros;
        }
        
        _u4PreMicros = u4Micros;
        if (u4Diff >= u4TimeOut)
        {
            i4temp =(u4Diff - u4TimeOut);
            while(i4temp > u4TimeOut)
            {
                if (_fgEnableLog)
                {
                    vAddValue(0x0);
                }
                i4temp -= u4TimeOut;
            }
            u4Diff = i4temp; 
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

static INT32 _vDrvTvdFastDump(INT32 i4Argc, const CHAR **szArgv)
{

    UINT8 u1starbit, u1stopbit;
    UINT32 u4DumpAddr, u4RegValue;
    INT8 i1bitragne;
    INT32 i4TimePeriod;
    static UINT32 u4PreVBI2Mode;

	if (i4Argc != 6)
	{
	    goto EXIT;
	}

    u4DumpAddr = StrToInt(szArgv[1]);
    u1starbit = StrToInt(szArgv[2]);
    u1stopbit = StrToInt(szArgv[3]);
    _i4LogItem = StrToInt(szArgv[4]);
    i4TimePeriod = StrToInt(szArgv[5]);
    _pFifoStarAddr = (UINT32 *)(RegReadFldAlign(VBI2_02, VBI2_MBASE0) << 4); 
    _FifoSize = RegReadFldAlign(VBI2_04, VBI_MOFFSET) << 4;        
    i1bitragne = (u1stopbit - u1starbit)+1; 

    if ((i1bitragne <= 0) || (_i4LogItem <=0) || (i4TimePeriod < 0) )
    {
        goto EXIT;
    }

    if (_FifoSize < _i4LogItem*4)
    {
        Printf ("Fifo Size [%ld] not enough!\n",_FifoSize );
        goto EXIT;
    }

    _fgEnableLog = TRUE;

    u4PreVBI2Mode = VBI_GetOutputMode(VBI2);
    VBI_SetOutputMode(VBI2, VBI_OMODE_NO);

    while (_fgEnableLog)
    {
            if (fgTimeTriger(64))
            {
                u4RegValue = (*(volatile UINT32 *)(u4DumpAddr) >> u1starbit)&((1<<i1bitragne)-1) ; 
                vAddValue(u4RegValue);
            }
    }

    VBI_SetOutputMode(VBI2, u4PreVBI2Mode);
    return 0;
EXIT:
    
	Printf("n.tvd.fdump [Addr] [Star Bit] [End Bit] [LogItems] [TimePeriod]\n");
	Printf("EX: n.tvd.fdump 0x20022044 0 9 1024 64 --> dump 0x20022044[0:9] 1024 items per 64MS\n");
	return 0;

}

//#define QTest_Delay 900
#define QTest_Delay 50
#define FAIL_THR    2

#define EN_STDTEST  0x8000
#define EN_CAGCTOGGLE   0x0400
#define EN_CFLICKER 0x0200
#define EN_VBOUNCING    0x0100
#define EN_HJITTER  0x0080
#define EN_HLOCK    0x0040
#define EN_MODECHG  0x0020
#define EN_VPRES    0x0010
#define EN_BLOCK    0x0008
#define EN_VLOCK    0x0004
#define EN_HSYNC    0x0002
#define EN_LINEERR  0x0001
#define ALL_QTEST    0x003F
#define ALL_STDTEST    0x03FF


#define DFT_VPRESOFF 2
#define DFT_MODECHG  2
#define DFT_BUNLCOK  8
#define DFT_VUNLOCK  8
#define DFT_HUNLOCK  8
#define DFT_LINEERR  8
#define DFT_HJITTER 0xb0
#define DFT_VBOUNCING 0x10

typedef struct RTvdIndicator
{
	UINT8 bVPresoff;
	UINT8 bModeChg;
	UINT8 bBunlock;
	UINT8 bHsyncLoss;
	UINT8 bHunlock;
	UINT8 bVunlock;
	UINT8 bHjitter;
	UINT8 bVbouncing;
	UINT8 bCFlicker;
	UINT8 bCAGCToggle;
} RTvdIndicator;


//-----------Fail Threshold for each items-----------------
static UINT8 u1ModeChgThr  = DFT_MODECHG;
static UINT8 u1VpresOffThr = DFT_VPRESOFF;
static UINT8 u1BunLockThr  = DFT_BUNLCOK;
static UINT8 u1VunLockThr  = DFT_VUNLOCK;
static UINT8 u1HsyncLossThr = DFT_HUNLOCK;
static UINT8 u1LineErrThr   = DFT_LINEERR;
static UINT16 u2ChkTimeOut = QTest_Delay;
static UINT8 u1HunLockThr = DFT_HUNLOCK;
static UINT32 u4HjitterThr = DFT_HJITTER;
static UINT32 u4VbouncingThr = DFT_VBOUNCING;

static UINT16 bTestMask = ALL_QTEST;
static UINT16 u2TvdTestCnt = 1;
static UINT8 bVpresOffCnt = 0, bVunlockCnt = 0, bHyncUnlockCnt = 0, bBunlockCnt = 0, bLineErrCnt = 0;
static UINT8 bModeChgCnt;
static UINT8 bPreMode = 0;
static UINT8 bHunlockCnt = 0;
static UINT8 bHjitterCnt=0;
static UINT32 u4TvdHErrorSum=0;
static UINT8 bVbouncingCnt=0;
static UINT32 u4TvdVErrorSum=0;
static UINT32 bCFlickerCnt=0;
static UINT32 u4TvdUDiffSum=0;
static UINT32 u4TvdVDiffSum=0;
static UINT32 u4TvdUDiffTmp=0;
static UINT32 u4TvdVDiffTmp=0;

RTvdIndicator _rTvdIndicator;

static void _sTvdRestCnt(void)
{
    bVpresOffCnt = 0;
    bVunlockCnt = 0;
    bHyncUnlockCnt = 0;
    bBunlockCnt = 0;
    bLineErrCnt = 0;
    bModeChgCnt = 0;
    bPreMode = 0;
    u2TvdTestCnt = 1;
    bHunlockCnt = 0;
    bHjitterCnt=0; u4TvdHErrorSum=0;
    bVbouncingCnt=0; u4TvdVErrorSum=0;
    bCFlickerCnt=0; u4TvdUDiffSum=0; u4TvdVDiffSum=0; u4TvdUDiffTmp=0; u4TvdVDiffTmp=0;
}

static void _sTvdQtestInit(UINT32 u4FailThres, UINT16 u2ChekTime)
{
    u1ModeChgThr   = (u4FailThres>>20) & 0xF;
    u1VpresOffThr  = (u4FailThres>>16) & 0xF;
    u1BunLockThr   = (u4FailThres>>12) & 0xF;
    u1VunLockThr   = (u4FailThres>>8)  & 0xF;
    u1HsyncLossThr = (u4FailThres>>4)  & 0xF;
    u1LineErrThr   = (u4FailThres)     & 0xF;
    u2ChkTimeOut   = u2ChekTime;
    _sTvdRestCnt();
    x_memset(&_rTvdIndicator, 0, sizeof(RTvdIndicator));
}


static void _sTvdQTestNfy (void *pInput)
{
    RTvdStatus *pTvd3dStatus =  (RTvdStatus *)pInput;
    UINT8 u1temp;
    UINT16 bPass = 0;
    

    if ((u2TvdTestCnt % u2ChkTimeOut) == 0)
    {
        if(!(bTestMask & EN_STDTEST))//For Qlab test result
        {
            if ((bVpresOffCnt >= u1VpresOffThr)&&(bTestMask & EN_VPRES))
            {
                Printf("##VPRESOFF[%d]", bVpresOffCnt);
            }
            else if((bModeChgCnt >= u1ModeChgThr)&&(bTestMask & EN_MODECHG))
            {
                Printf("##MODECHG[%d]", bModeChgCnt);
            }
            else if ((_rTvdIndicator.bBunlock==TRUE)&&(bTestMask & EN_BLOCK))
            {
                Printf("##BUNLOCK[%d]", bBunlockCnt);
            }
                else if ((_rTvdIndicator.bVunlock==TRUE)&&(bTestMask & EN_VLOCK))
            {
                Printf("##VUNLOCK[%d]",bVunlockCnt);
            }
                else if ((_rTvdIndicator.bHsyncLoss==TRUE)&&(bTestMask & EN_HSYNC))
            {
                Printf("##HSYNCUNLOCK[%d]",bHyncUnlockCnt);
            }
            else if ((bLineErrCnt >= u1LineErrThr)&&(bTestMask & EN_LINEERR))
            {
                Printf("##LINEERR[%d]",bLineErrCnt);
            }
            else
            {
                if ((bVpresOffCnt == 0) || ((bTestMask & EN_VPRES) == 0))
                {
                        bPass |= EN_VPRES;
                }
                if ((bModeChgCnt < DFT_MODECHG) || ((bTestMask & EN_MODECHG) == 0))
                {
                        bPass |= EN_MODECHG;
                }
                    if ((_rTvdIndicator.bVunlock==FALSE) || ((bTestMask & EN_VLOCK) == 0))
                {
                        bPass |= EN_VLOCK;
                }
                    if ((_rTvdIndicator.bBunlock==FALSE) || ((bTestMask & EN_BLOCK) == 0))
                {
                        bPass |= EN_BLOCK;
                }
                    if ((_rTvdIndicator.bHsyncLoss==FALSE) || ((bTestMask & EN_HSYNC) == 0))
                {
                        bPass |= EN_HSYNC;
                }
                if ((bLineErrCnt == 0) || ((bTestMask & EN_LINEERR) == 0))
                {
                        bPass |= EN_LINEERR;
                }
                    if (bPass == ALL_QTEST)
                {
                    Printf("##TVDPASS");
                }
                else
                {
                    Printf("##TVDTEST_FINISH");
                    Printf("\nodecnt,Vprescnt,Bcnt,Vcnt,Hcnt,Lcnt] = [%d,%d,%d,%d,%d,%d]\n",
                        bModeChgCnt,bVpresOffCnt,bBunlockCnt,bVunlockCnt,bHyncUnlockCnt,bLineErrCnt);   
                }
            }        
            _sTvdRestCnt();
            u2TvdTestCnt = 1;
        }
        else
        {
            if ((bVpresOffCnt >= u1VpresOffThr)&&(bTestMask & EN_VPRES))
            {
                Printf("##VPRESOFF[%d]\n", bVpresOffCnt);
            }
            if((bModeChgCnt >= u1ModeChgThr)&&(bTestMask & EN_MODECHG))
            {
                Printf("##MODECHG[%d]\n", bModeChgCnt);
            }
            if ((_rTvdIndicator.bBunlock==TRUE)&&(bTestMask & EN_BLOCK))
            {
                Printf("##BUNLOCK[%d]\n", bBunlockCnt);
            }
            if ((_rTvdIndicator.bCFlicker==TRUE)&&(bTestMask & EN_CFLICKER))
            {
                Printf("##CFLICKER[%ld]\n", bCFlickerCnt);
            }
            if ((_rTvdIndicator.bVunlock==TRUE)&&(bTestMask & EN_VLOCK))
            {
                Printf("##VUNLOCK[%d]\n",bVunlockCnt);
            }
            if ((_rTvdIndicator.bVbouncing==TRUE)&&(bTestMask & EN_VBOUNCING))
            {
                Printf("##VBOUNCING\n");
            }
            if ((_rTvdIndicator.bHsyncLoss==TRUE)&&(bTestMask & EN_HSYNC))
            {
                Printf("##HSYNCUNLOCK[%d]\n",bHyncUnlockCnt);
            }
            if ((_rTvdIndicator.bHunlock==TRUE)&&(bTestMask & EN_HLOCK))
            {
                Printf("##HUNLOCK[%d]\n",bHunlockCnt);
            }
            if ((bLineErrCnt >= u1LineErrThr)&&(bTestMask & EN_LINEERR))
            {
                Printf("##LINEERR[%d]\n",bLineErrCnt);
            }
            if ((_rTvdIndicator.bHjitter==TRUE)&&(bTestMask & EN_HJITTER))
            {
                Printf("##HJITTER\n");
            }
            else
            {
                if ((bVpresOffCnt == 0) || ((bTestMask & EN_VPRES) == 0))
                {
                        bPass |= EN_VPRES;
                }
                if ((bModeChgCnt < DFT_MODECHG) || ((bTestMask & EN_MODECHG) == 0))
                {
                        bPass |= EN_MODECHG;
                }
                if ((_rTvdIndicator.bVunlock==FALSE) || ((bTestMask & EN_VLOCK) == 0))
                {
                        bPass |= EN_VLOCK;
                }
                if ((_rTvdIndicator.bVbouncing==FALSE) || ((bTestMask & EN_VBOUNCING) == 0))
                {
                        bPass |= EN_VBOUNCING;
                }
                if ((_rTvdIndicator.bBunlock==FALSE) || ((bTestMask & EN_BLOCK) == 0))
                {
                        bPass |= EN_BLOCK;
                }
                if ((_rTvdIndicator.bHsyncLoss==FALSE) || ((bTestMask & EN_HSYNC) == 0))
                {
                        bPass |= EN_HSYNC;
                }
                if ((bLineErrCnt == 0) || ((bTestMask & EN_LINEERR) == 0))
                {
                        bPass |= EN_LINEERR;
                }
                if ((_rTvdIndicator.bHjitter==FALSE) || ((bTestMask & EN_HJITTER) == 0))
                {
                        bPass |= EN_HJITTER;
                }
                if ((_rTvdIndicator.bCFlicker==FALSE) || ((bTestMask & EN_CFLICKER) == 0))
                {
                        bPass |= EN_CFLICKER;
                }
                if ((_rTvdIndicator.bCAGCToggle==FALSE) || ((bTestMask & EN_CAGCTOGGLE) == 0))
                {
                        bPass |= EN_CAGCTOGGLE;
                }
                if (bPass == ALL_STDTEST)
                {
                    Printf("##TVDPASS\n");
                }
            }        
            _sTvdRestCnt();
            u2TvdTestCnt = 1;
        }
    }
    else
    {
        //HSYNC Lock
        if (pTvd3dStatus->fgHSYNC_LOCK == FALSE)
        {
            bHyncUnlockCnt ++;
            if(bHyncUnlockCnt >= u1HsyncLossThr)
            {
                bHyncUnlockCnt = u1HsyncLossThr;
                _rTvdIndicator.bHsyncLoss= TRUE;
            }    
        }
        else
        {
            if(bHyncUnlockCnt > 0)
                bHyncUnlockCnt--;
            if(bHyncUnlockCnt==0)
                _rTvdIndicator.bHsyncLoss = FALSE;
        }

        //H Lock
        if (pTvd3dStatus->fgHLOCK == FALSE)
        {
            bHunlockCnt ++;
            if(bHunlockCnt >= u1HunLockThr)
            {
                bHunlockCnt = u1HunLockThr;
                _rTvdIndicator.bHunlock = TRUE;
            }    
        }
        else
        {
            if(bHunlockCnt > 0)
                bHunlockCnt--;
            if(bHunlockCnt==0)
            {
                _rTvdIndicator.bHunlock = FALSE;
            }
        }
        
        //V Lock
        if (pTvd3dStatus->fgVLOCK == FALSE)
        {
            bVunlockCnt ++;
            if(bVunlockCnt>=u1VunLockThr)
            {
                bVunlockCnt = u1VunLockThr;
                _rTvdIndicator.bVunlock = TRUE;
            }
        }
        else
        {
            if(bVunlockCnt>0)
                bVunlockCnt--;
            if(bVunlockCnt==0)
            {
                _rTvdIndicator.bVunlock = FALSE;
            }
        }

        //VPRES
        if (pTvd3dStatus->fgVPres == FALSE)
            bVpresOffCnt ++;

        //CKILL or Burst Unlock
        if ((pTvd3dStatus->fgIsCkill==TRUE)
            || (pTvd3dStatus->fgIsBurstLock== FALSE && pTvd3dStatus->bTvdMode!=SV_CS_SECAM)
            || (pTvd3dStatus->u4U==512 && pTvd3dStatus->u4V==512))
        {
            bBunlockCnt++;
            if(bBunlockCnt>=u1BunLockThr)
            {
                bBunlockCnt = u1BunLockThr;
                _rTvdIndicator.bBunlock = TRUE;
            }    
        }
        else
        {
            if(bBunlockCnt>0)
                bBunlockCnt--;
            if(bBunlockCnt==0)
            {
                _rTvdIndicator.bBunlock = FALSE;
            }
        }
        
        //Mode Change
        if (pTvd3dStatus->bTvdMode != bPreMode)
        {
            bPreMode = pTvd3dStatus->bTvdMode;
            bModeChgCnt++;
        }

        //Line Err
        if (pTvd3dStatus->wLine_err < 0)
        {
            u1temp = 0x10000 - pTvd3dStatus->wLine_err;
        }
        else
        {   
            u1temp = pTvd3dStatus->wLine_err;
        }
        if (u1temp > FAIL_THR)
        {
            bLineErrCnt++;
        }

        //HJitter
        if(bHjitterCnt < 16)
        {
            u4TvdHErrorSum += pTvd3dStatus->u4LLockStatus;
            bHjitterCnt++;
        }
        else
        {
            LOG(1,"[TVD_DBG_MSG] QTest: HError=%d\n", u4TvdHErrorSum);
            if(u4TvdHErrorSum > u4HjitterThr)//42dB
                _rTvdIndicator.bHjitter = TRUE;
            else
                _rTvdIndicator.bHjitter = FALSE;
            u4TvdHErrorSum = 0;
            bHjitterCnt = 0;
        }

        //check V-BOUNCING
        if(bVbouncingCnt < 16)
        {
            if(pTvd3dStatus->LineInfo.wERR_VLEN >= pTvd3dStatus->LineInfo.wLOCK_VLEN)
                u4TvdVErrorSum += (pTvd3dStatus->LineInfo.wERR_VLEN - pTvd3dStatus->LineInfo.wLOCK_VLEN);
            else
                u4TvdVErrorSum += pTvd3dStatus->LineInfo.wERR_VLEN;
            bVbouncingCnt++;
        }
        else
        {
            LOG(1,"[TVD_DBG_MSG] QTest: VError=%d\n", u4TvdVErrorSum);
            if(u4TvdVErrorSum > u4VbouncingThr)
                _rTvdIndicator.bVbouncing = TRUE;
            else
                _rTvdIndicator.bVbouncing = FALSE;
            u4TvdVErrorSum = 0;
            bVbouncingCnt = 0;
        }

        //check C-FLICKER
        if(bCFlickerCnt < 16)
        {
            if(bCFlickerCnt)
            {
                if(pTvd3dStatus->u4U >= u4TvdUDiffTmp)
                {
                    u4TvdUDiffSum += pTvd3dStatus->u4U - u4TvdUDiffTmp;
                }
                else
                {
                    u4TvdUDiffSum += u4TvdUDiffTmp - pTvd3dStatus->u4U;
                }

                if(pTvd3dStatus->u4V >= u4TvdVDiffTmp)
                {
                    u4TvdVDiffSum += pTvd3dStatus->u4V - u4TvdVDiffTmp;
                }
                else
                {
                    u4TvdVDiffSum += u4TvdVDiffTmp - pTvd3dStatus->u4V;
                }
            }
            u4TvdUDiffTmp = pTvd3dStatus->u4U;
            u4TvdVDiffTmp = pTvd3dStatus->u4V;
            bCFlickerCnt ++;
        }
        else
        {
            UINT32 TvdUDiff_Thr, TvdVDiff_Thr;
            if((pTvd3dStatus->bTvdMode == SV_CS_PAL)
            ||(pTvd3dStatus->bTvdMode == SV_CS_PAL_M)
            ||(pTvd3dStatus->bTvdMode == SV_CS_PAL_N))
            {
                TvdUDiff_Thr = 0x80;
                TvdVDiff_Thr = 0x70;
            }
            else
            {
                TvdUDiff_Thr = 0x90;
                TvdVDiff_Thr = 0x80;
            }
            if((u4TvdUDiffSum > TvdUDiff_Thr) || (u4TvdVDiffSum > TvdVDiff_Thr))
                _rTvdIndicator.bCFlicker = TRUE;
            else
                _rTvdIndicator.bCFlicker = FALSE;

            LOG(1,"[TVD_DBG_MSG] QTest: CFlicker_U_Diff=%d\n", u4TvdUDiffSum);
            LOG(1,"[TVD_DBG_MSG] QTest: CFlicker_V_Diff=%d\n", u4TvdVDiffSum);
            
            u4TvdUDiffSum = 0;
            u4TvdVDiffSum = 0;
            bCFlickerCnt = 0;
        }

    }
    
    u2TvdTestCnt ++;

    return;
}




static INT32 _vDrvTVDQtest(INT32 i4Argc, const CHAR **szArgv)
{
    BOOL fgEnQtest;
    UINT32 u4FailThres = ((DFT_MODECHG<<20)|(DFT_VPRESOFF<<16)|(DFT_BUNLCOK<<12)|
                            (DFT_VUNLOCK<<8)|(DFT_HUNLOCK<<4)|(DFT_LINEERR));
    UINT16 u2ChkTimeOut = QTest_Delay;

    if (i4Argc == 2)
    {
        fgEnQtest = StrToInt(szArgv[1]);
        bTestMask = ALL_QTEST;
    }
    else if (i4Argc == 3)
    {
        fgEnQtest = StrToInt(szArgv[1]);
        bTestMask = StrToInt(szArgv[2]);        
    }
    else if (i4Argc == 4)
    {
        fgEnQtest = StrToInt(szArgv[1]);
        bTestMask = StrToInt(szArgv[2]);        
        u4FailThres = StrToInt(szArgv[3]); 
    }
    else if (i4Argc == 5)
    {
        fgEnQtest = StrToInt(szArgv[1]);
        bTestMask = StrToInt(szArgv[2]);        
        u4FailThres = StrToInt(szArgv[3]); 
        u2ChkTimeOut = StrToInt(szArgv[4]);
    }
    else
    {
	    goto EXIT;
    }

    if (fgEnQtest)
    {
        _sTvdQtestInit(u4FailThres, u2ChkTimeOut);
        vTvdSetCLINfy(_sTvdQTestNfy);
    }
    else
    {
        vTvdSetCLINfy(NULL);
    }

    return 1;
EXIT:
	Printf("---------CMD with one Parameter----------\n");
	Printf("n.tvd.qtest [En-Test]\n");
	Printf("EX: n.tvd.qtest 1 --> Enable Qlab TVD ALL Test\n");
	Printf("EX: n.tvd.qtest 0 --> Disable Qlab TVD ALL Test\n");
	Printf("---------CMD with two Parameter----------\n");
	Printf("n.tvd.qtest [En-Test] [B5-On/Off][B4-On/Off][B3-On/Off][B2-On/Off][B1-On/Off][B0-On/Off]\n");
	Printf("n.tvd.qtest [On/Off]  [MODECHG]  [VPRESOFF] [BUNLOCK]  [VUNLOCK]  [HUNLCOK]  [LINEERR]\n");
	Printf("EX: n.tvd.qtest 1 0x37 --> Enable Qtest but Disable BLOCK Check\n");
	Printf("---------CMD with three Parameter----------\n");
	Printf("n.tvd.qtest [En-Test] [En-Type] [B24-B20][B20-B16] [B16-B12] [B12-B08] [B08-B04] [B04-B00] \n");
	Printf("n.tvd.qtest [B0]      [B5-B0]   [MODE_TH][BLOCK_TH][VLOCK_TH][HSYNC_TH][LINE_TH] \n");
	Printf("EX: n.tvd.qtest 1 0x3F 0x222888 --> Enable Qtest and MODE_TH=2,BLOCK_TH=2,VLOCK_TH=8,HSYNC_TH=8,LINE_TH=8\n");
	Printf("---------CMD with four Parameter----------\n");
	Printf("n.tvd.qtest [En-Test]  [En-Type] [Type-Th] [Test Time] \n");
	Printf("EX: n.tvd.qtest 1 0x3F 0x222888  900 --> Enable Qtest with 900Vsyncs one Logs\n");
	Printf("---------Qtest DFT Value----------\n");
	Printf("Type Th[B24-B00]= [0x%x], Test Time = [%d]\n",
	    ((DFT_MODECHG<<20)|(DFT_VPRESOFF<<16)|(DFT_BUNLCOK<<12)|(DFT_VUNLOCK<<8)|(DFT_HUNLOCK<<4)|(DFT_LINEERR)), QTest_Delay );
    return 0;
}

static INT32 _vDrvTVDSetHNChChgMCnt(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 bTh;
	if (i4Argc != 2)
	{
		Printf("Usage: hncnt\n");
		return 0;
	}

	bTh = (UINT8) StrToInt(szArgv[1]);
	vTvd3dSetHNChChgMCntTh(bTh);
	
	return 0;
}

static INT32 _vDrvTVDEnablePhaltWa(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 bOnOff;
	if (i4Argc != 2)
	{
		Printf("Usage: phaltwa\n");
		return 0;
	}

	bOnOff = (UINT8) StrToInt(szArgv[1]);
	vTvd3dSetPhaltWaOnOff(bOnOff);
	return 0;
}

static INT32 _vDrvSVFBypass(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 TVD_SVFBypass;
	if (i4Argc != 2)
	{
		Printf("Usage: S-First function bypass, 1->bypass 0->enable\n");
		return 0;
	}

	TVD_SVFBypass = (UINT8) StrToInt(szArgv[1]);
	vDrvSetSVFBypass(TVD_SVFBypass);
	
	return 0;

}

#if TVD_NR_AVG
extern UINT32 _bGetNRTh;
extern UINT32 _bGetNRCnt;
extern UINT32 wNoiselevel_new2;
extern void vSetNRTable(UINT32 u1dB, UINT32 u4Val, UINT32 u4Count);

static INT32 _vDrvTVDSetNRTable(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 4)
	{
		Printf("Usage: setNR db val th\n");
		return 0;
	}

	vSetNRTable(StrToInt(szArgv[1]),StrToInt(szArgv[2]),StrToInt(szArgv[3]));
	return 0;
}
static INT32 _vDrvTVDEnableDumpNR(INT32 i4Argc, const CHAR **szArgv)
{
    Printf("Set _bGetNRCnt=%ld to ZERO\n", _bGetNRCnt);
    _bGetNRCnt = wNoiselevel_new2 = 0;
    return 0;
}
#endif

#endif //#ifdef CC_CLI

