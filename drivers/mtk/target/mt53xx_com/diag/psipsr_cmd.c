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
 * $RCSfile: psipsr_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file psipsr_cmd.c
 *  PSI Parser module CLI test function.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

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

#include "psipsr_if.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define PSI_TIMEOUT 3000
//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define MA_CHK_NUM_MIN_PARAM(N) \
	do \
	{ \
		if (i4Argc < ((N)+1)) \
		{ \
			Printf("too few arguments, at least "###N##"\n"); \
			return E_PSIPSR_ERROR; \
		} \
	} while (0)

#define MA_GET_PARAMETER_ONE_DIGIT(X_VAR, MIN, MAX) \
	do \
	{ \
		if ((szArgv[1][0] < (CHAR)'0'+MIN) || (szArgv[1][0] > (CHAR)'0'+MAX)) \
		{ \
			Printf("parameter range ("###MIN##")~("###MAX##"), input=%c\n", szArgv[1][0]); \
			return E_PSIPSR_ERROR; \
		} \
		else \
		{ \
			X_VAR = (INT32)szArgv[1][0] - (INT32)'0'; \
		} \
	} while (0)
//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
EXTERN BOOL PSIPSR_UtcToRtc(PSIPSR_RTC_T* prRtc, UINT64 u8Utc);
EXTERN INT32 PSIPSR_ParseECM(PSIPSR_TBL_ECM_T* prEcmTbl, UINT32* pu4NumTblEle, UINT32 u4Timeout, UINT32 u4TsIndex);

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static INT32 _PSIPSRCmdParsePSI(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PSIPSRCmdParseVCT(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PSIPSRCmdParseMGT(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PSIPSRCmdParseSTT(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PSIPSRCmdParseECM(INT32 i4Argc, const CHAR ** szArgv);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

/* Declare the debug on/off/level and RegTest functions */
CLIMOD_DEBUG_FUNCTIONS(PSIPSR)

/* End of Declare */

static CLI_EXEC_T _arPsipsrCmdTbl [] =
{
    { "parsePsi", "psi", _PSIPSRCmdParsePSI, NULL, "Parse PAT and PMT [0:tuner 0 / 1:tuner1]", CLI_SUPERVISOR },
    { "parseVct", "vct", _PSIPSRCmdParseVCT, NULL, "Parse VCT [0: tuner 0 / 1:tuner 1] [0: tvct 1: cvct]", CLI_SUPERVISOR },
    { "parseMgt", "mgt", _PSIPSRCmdParseMGT, NULL, "Parse MGT [0: tuner 0 / 1: tuner 1]", CLI_SUPERVISOR },
    { "parseStt", "stt", _PSIPSRCmdParseSTT, NULL, "Parse STT [0: tuner 0 / 1: tuner 1]", CLI_SUPERVISOR },
    { "parseEcm", "ecm", _PSIPSRCmdParseECM, NULL, "Parse ECM [PID]", CLI_SUPERVISOR },          
    CLIMOD_DEBUG_CLIENTRY(PSIPSR),
    { NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR }
};

CLI_MAIN_COMMAND_ITEM(Psipsr)
{
    "psipsr", NULL, NULL, _arPsipsrCmdTbl, "PSI Parser Command", CLI_SUPERVISOR
};


static INT32 _PSIPSRCmdParsePSI(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4Ret, u4Src, u4Num =16;
	UINT32 u4Timeout = PSI_TIMEOUT;
	PSIPSR_TBL_PMT_T arPMTTbl[16];

	MA_CHK_NUM_MIN_PARAM(1);
	u4Src = (UINT32)StrToInt(szArgv[1]);
	//CLI_Parser_Arg("dmx.settuner %d", u4Src);
	if (i4Argc==3)
		u4Timeout = (UINT32)StrToInt(szArgv[2]);

	x_memset(arPMTTbl, 0, sizeof(arPMTTbl));
	
	u4Ret = PSIPSR_ParsePSI(arPMTTbl, &u4Num, u4Timeout, u4Src);
	
	if (u4Ret==E_PSIPSR_OK)
	{
		INT32 i;
		LOG(1, "PSI parsing done---------------------\n");
		
		for (i=0; i<u4Num; i++)
		{
			LOG(1, "prog=0x%x, pcr=0x%x, v=0x%x(0x%x), a1=0x%x(0x%x), a2=0x%x(0x%x), a3=0x%x(0x%x)\n",
				arPMTTbl[i].u4ProgNum, arPMTTbl[i].u4PCR, arPMTTbl[i].u4VPID, arPMTTbl[i].u4VPIDType, 
				arPMTTbl[i].u4APID1, arPMTTbl[i].u4APID1Type,
				arPMTTbl[i].u4APID2, arPMTTbl[i].u4APID2Type,
				arPMTTbl[i].u4APID3, arPMTTbl[i].u4APID3Type);
		}
	}
	else
		LOG(1, "PSI parse timeout\n");
	return (INT32)E_PSIPSR_OK;
}



static INT32 _PSIPSRCmdParseVCT(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4Ret, u4System, u4Src, u4Timeout = PSI_TIMEOUT;
	PSIPSR_TBL_VCT_T arVctTbl[64];
	PSIPSR_TBL_TS2FREQ_T arTsTbl[1];
	UINT32 u4ChNum = 64;

	x_memset(arVctTbl, 0, sizeof(arVctTbl));
	x_memset(arTsTbl, 0, sizeof(arTsTbl));

	MA_CHK_NUM_MIN_PARAM(2);
	u4System = (UINT32)StrToInt(szArgv[1]);
	u4Src = (UINT32)StrToInt(szArgv[2]);
	//CLI_Parser_Arg("dmx.settuner %d", u4Src);
	
	if (i4Argc==4)
		u4Timeout = (UINT32)StrToInt(szArgv[3]);
	if(u4System == 0)
		u4Ret = PSIPSR_ParseTVCT(arVctTbl, &u4ChNum, arTsTbl, u4Timeout, u4Src);
	else 
		u4Ret = PSIPSR_ParseCVCT(arVctTbl, &u4ChNum, arTsTbl, u4Timeout, u4Src);
	if (u4Ret==E_PSIPSR_OK || u4Ret==E_PSIPSR_SLD_NOT_MATCH)
	{
		INT32 i4I;
		LOG(1, "VCT parse done---------------------\n");
		LOG(1, "tsId=0x%02x, version=%d, chNum=%d\n", arTsTbl->u2TsId, arTsTbl->u1Version, u4ChNum);
		for (i4I=0; i4I<u4ChNum; i4I++)
		{
			PSIPSR_TBL_VCT_T  *prVctTbl = &arVctTbl[i4I];
			LOG(1, "[%2d.%d %7s] tsId=0x%02x, prog=%d, pcr=0x%02x, v=0x%02x, a1=0x%02x, a2=0x%02x\n",
				   prVctTbl->u2MajorChNum, prVctTbl->u2MinorChNum, prVctTbl->szShortName,
				   prVctTbl->u2ChTsId, prVctTbl->u2ProgNum, prVctTbl->u2PCR,
				   prVctTbl->u2VPID, prVctTbl->u2APID1, prVctTbl->u2APID2);
		}
	}
	else
		LOG(1, "VCT parse timeout\n");
	return (INT32)E_PSIPSR_OK;
}




static INT32 _PSIPSRCmdParseMGT(INT32 i4Argc, const CHAR ** szArgv)
{
	INT32 i4Ret=0, i=0;
	PSIPSR_TBL_MGT_T arMgtTbl[128];
	UINT32 u4Num = 128;
	UINT32 u4Src = (UINT32)StrToInt(szArgv[1]);
	UINT32 u4Timeout = PSI_TIMEOUT;

	x_memset(arMgtTbl, 0, sizeof(arMgtTbl));

	MA_CHK_NUM_MIN_PARAM(1);
	//CLI_Parser_Arg("dmx.settuner %d", u4Src);
	if (i4Argc==3)
		u4Timeout = (UINT32)StrToInt(szArgv[2]);
	

	i4Ret = PSIPSR_ParseMGT(arMgtTbl, &u4Num, u4Timeout, u4Src);
	if (i4Ret==0)
	{
		
		LOG(1, "MGT parse done---------------------\n");
		LOG(1, "u4Num=%d\n", u4Num);
		for (i=0; i<u4Num; i++)
		{
			LOG(1, "[%d] u4Type=0x%04x, u4Pid=0x%04x, u4Version=0x%x, u4Length=0x%x\n",
				i+1, arMgtTbl[i].u4Type, arMgtTbl[i].u4Pid, arMgtTbl[i].u4Version,
				arMgtTbl[i].u4Length);
		}
	}
	else
		LOG(1, "MGT parse timeout\n");

	return (INT32)E_PSIPSR_OK;
}



static INT32 _PSIPSRCmdParseSTT(INT32 i4Argc, const CHAR ** szArgv)
{
	INT32 i4Ret=0;
	PSIPSR_TBL_STT_T rSttTbl;
	UINT32 u4Num = 128;
	UINT32 u4Src = (UINT32)StrToInt(szArgv[1]);
	UINT32 u4Timeout = PSI_TIMEOUT;
	PSIPSR_RTC_T rRtc;
		
	x_memset(&rSttTbl, 0, sizeof(PSIPSR_TBL_STT_T));

	MA_CHK_NUM_MIN_PARAM(1);
	//CLI_Parser_Arg("dmx.settuner %d", u4Src);
	if (i4Argc==3)
		u4Timeout = (UINT32)StrToInt(szArgv[2]);
	

	i4Ret = PSIPSR_ParseSTT(&rSttTbl, &u4Num, u4Timeout, u4Src);
	if (i4Ret==0)
	{
		
		LOG(1, "STT parse done---------------------\n");
		LOG(1, "u4Num=%d\n", u4Num);

		LOG(1, "System Time: %d, GPS to UTC Offset: %d, Daylight saving: %d\n", 
			rSttTbl.u4SysTime, rSttTbl.u1GPSUTCOffset, rSttTbl.u2DayLightSavings);
		PSIPSR_UtcToRtc(&rRtc, (UINT64)rSttTbl.u4SysTime-rSttTbl.u1GPSUTCOffset);
		LOG(1, "RTC(Pacific Standard): %2d/%2d/%2d %2d:%2d:%2d\n",
			rRtc.u1Year,rRtc.u1Month, rRtc.u1Day,rRtc.u1Hour,rRtc.u1Minute,rRtc.u1Second);
	}
	else
		LOG(1, "STT parse timeout\n");

	return (INT32)E_PSIPSR_OK;
}

static INT32 _PSIPSRCmdParseECM(INT32 i4Argc, const CHAR ** szArgv)
{
	INT32 i4Ret=0;
	PSIPSR_TBL_ECM_T rEcmTbl;
	UINT32 u4Num = 128;
	UINT32 u4Src = (UINT32)StrToInt(szArgv[1]);
	UINT32 u4Timeout = PSI_TIMEOUT;
		
	x_memset(&rEcmTbl, 0, sizeof(PSIPSR_TBL_ECM_T));

	MA_CHK_NUM_MIN_PARAM(1);
	//CLI_Parser_Arg("dmx.settuner %d", u4Src);
	if (i4Argc==3)
		u4Timeout = (UINT32)StrToInt(szArgv[2]);
	
       rEcmTbl.u2Pid = StrToInt(szArgv[1]);

	i4Ret = PSIPSR_ParseECM(&rEcmTbl, &u4Num, u4Timeout, u4Src);
	if (i4Ret==0)
	{
		
		LOG(1, "ECM parse done---------------------\n");

	}
	else
		LOG(1, "ECM parse timeout\n");

	return (INT32)E_PSIPSR_OK;
}


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------



