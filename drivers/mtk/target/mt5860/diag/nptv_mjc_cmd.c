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
 * $RCSfile: nptv_mjc_cmd.c $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "x_drv_cli.h"
#include "x_stl_lib.h"
#include "drv_mjc_if.h"
#include "drv_mjc.h"
#include "drv_mfd.h"
#include "drv_ttd.h"

/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/
static INT32 _i4MjcInit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcInitISR(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcPROnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcQuery(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcPRQuery(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetDispMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetDramMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSet3DMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetPRMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcChkDramOvfUdf(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcCheck3DFSLR(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcCheckCRC(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetFrmSchMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetFlipMirror(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetEffect(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcStatusPolling(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetSPReg(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetMute(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcMbistGo(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSPEna(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetEffectMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetEffectDFT(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcChkFrmDly(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetFreeze(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetFB1Frm(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetTTDFWMODE(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetTTDUIDepth(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetProgMEMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetDemo(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetMJC1RPR1R(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetMJCDbgInk(INT32 i4Argc, const CHAR **szArgv);
#if 1
static INT32 _i4MjcSetTTDOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetTTDMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcSetTTDPicSize(INT32 i4Argc, const CHAR **szArgv);

#endif

//CLIMOD_DEBUG_FUNCTIONS(MJC)

// Table for MJC command entry
CLI_EXEC_T arMjcCmdTbl[] = {
    {"Init MJC",        "i",     _i4MjcInit,          NULL, "Initialize MJC",              CLI_GUEST},
    {"Init ISR",        "isr",   _i4MjcInitISR,       NULL, "Initialize MJC ISR",          CLI_GUEST},
    {"OnOff",           "oo",    _i4MjcOnOff,         NULL, "Enable MJC",                  CLI_GUEST},
    {"PROnOff",         "proo",  _i4MjcPROnOff,       NULL, "Enable MJC PR OnOff",         CLI_GUEST},
    {"Query MJC Info",  "q",     _i4MjcQuery,         NULL, "Query MJC Information",       CLI_GUEST},
    {"Query MJCPR Info","prq",   _i4MjcPRQuery,       NULL, "Query MJC PR Information",    CLI_GUEST},
    {"Set TTD Adap FW", "ttdfw", _i4MjcSetTTDFWMODE,  NULL, "Set TTD Adap FW",             CLI_GUEST},
    {"set TTD UI Depth","ttdcd", _i4MjcSetTTDUIDepth, NULL, "Set TTD UI Depth",            CLI_GUEST},
    {"Set DISP Mode",   "disp",  _i4MjcSetDispMode,   NULL, "Set MJC DISP Mode",           CLI_GUEST},
    {"Set DRAM Mode",   "dm",    _i4MjcSetDramMode,   NULL, "Set MJC DRAM Mode",           CLI_GUEST},
    {"Set 3D Mode",     "3d",    _i4MjcSet3DMode,     NULL, "Set MJC 3D Mode",             CLI_GUEST},
    {"Set PR Mode",     "pm",    _i4MjcSetPRMode,     NULL, "Set MJC PR Mode",             CLI_GUEST},
    {"Set Flip/Mirror", "fm",    _i4MjcSetFlipMirror, NULL, "Set MJC Flip/Mirror",         CLI_GUEST},
    {"Set FrmSch Mode", "frmsh", _i4MjcSetFrmSchMode, NULL, "Set MJC Frame Schedule Mode", CLI_GUEST},
    {"Set Effect",      "eft",   _i4MjcSetEffect,     NULL, "Set Effect",                  CLI_GUEST},
    {"Set Effect Mode", "rmp",   _i4MjcSetEffectMode, NULL, "Set Effect Remap Mode",       CLI_GUEST},
    {"Set Effect DFT",  "dft",   _i4MjcSetEffectDFT,  NULL, "Set Effect Remap DFT",        CLI_GUEST},
    {"Set Demo Mode",   "demo",  _i4MjcSetDemo,       NULL, "Set MJC Demo Mode",           CLI_GUEST},
    {"Set SP Register", "sspr",  _i4MjcSetSPReg,      NULL, "Set Status Polling Register", CLI_GUEST},
    {"Set MJC Mute",    "mute",  _i4MjcSetMute,       NULL, "Set MJC Mute",                CLI_GUEST},
    {"Status Polling",  "sp",    _i4MjcStatusPolling, NULL, "Polling Status",              CLI_GUEST},
	{"Set MJC1W_PR1R",  "mwpr",  _i4MjcSetMJC1RPR1R,  NULL, "Set MJC1RPR1R Mode ",         CLI_GUEST},
    {"SP OnOff",        "spen",  _i4MjcSPEna,         NULL, "Status Polling Enable",       CLI_GUEST},
    {"Check BW Status", "chkdm", _i4MjcChkDramOvfUdf, NULL, "Verify Bandwidth Status",     CLI_GUEST},
    {"Check Frm Delay", "chkfd", _i4MjcChkFrmDly,     NULL, "MJC Frame Delay Check",       CLI_GUEST},
    {"Check Mbist",     "chkmb", _i4MjcMbistGo,       NULL, "MJC Mbist Proc",              CLI_GUEST},
    {"Frame Freeze",    "frz",   _i4MjcSetFreeze,     NULL, "MJC Frame Freeze",            CLI_GUEST},
    {"MJC FB 1 Frm",    "fb1",   _i4MjcSetFB1Frm,     NULL, "MJC FB 1 Frm",                CLI_GUEST},
    {"MJC Prog ME",     "pme",   _i4MjcSetProgMEMode, NULL, "MJC Programmable ME Setting", CLI_GUEST},
    {"Set MJC dbg ink", "ink",   _i4MjcSetMJCDbgInk,  NULL, "Set MJC debug ink",           CLI_GUEST},
    {"Check 3d fs LR",  "chklr", _i4MjcCheck3DFSLR,   NULL, "Check 3d fs LR",              CLI_GUEST},
    {"Check IN/OUT CRC","crc",   _i4MjcCheckCRC,      NULL, "Check MJC INPUT/OUTPUT CRC",  CLI_GUEST},
#if 1
    {"Set TTD On/Off",  "ttden", _i4MjcSetTTDOnOff,   NULL, "Set TTD On/Off Control",      CLI_GUEST},
    {"Set TTD Mode",    "ttddm", _i4MjcSetTTDMode,    NULL, "Set TTD Mode Control",        CLI_GUEST},
    {"Set TTD Size",    "ttdsz", _i4MjcSetTTDPicSize, NULL, "Set TTD Picture Size",        CLI_GUEST},
#endif
    //CLIMOD_DEBUG_CLIENTRY(MJC),
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

/*----------------------------------------------------------------------------*
 * Static functions
 *----------------------------------------------------------------------------*/
static INT32 _i4MjcInit(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 1;
	}

	vDrvMJCInit(FALSE);

	return 0;
}

static INT32 _i4MjcInitISR(INT32 i4Argc, const CHAR **szArgv)
{	
    if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 1;
	}

	vDrvMJCInitISR();

	return 0;
}


static INT32 _i4MjcOnOff(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32  fgOnOff;
    
	if (i4Argc != 2)
	{
		Printf("Current Status: %s\n", u1DrvMJCIsOnOff() ? "On" : "Off");
		Printf("Usage: %s OnOff\n", szArgv[0]);
		Printf("<OnOff>\n");
		Printf("off: Disable\n");
		Printf("on: Enable\n");

		return 1;
	}

    fgOnOff = (x_strcmp(szArgv[1], "on") == 0) ? E_MJC_ON : E_MJC_BYPASS;
	MJC_SetOnOff(fgOnOff);

	return 0;
}

static INT32 _i4MjcPROnOff(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32  fgPROnOff;
    
	if (i4Argc != 2)
	{
		Printf("Current Status: %s\n", u1DrvMJCGetPROnOffInfo() ? "On" : "Off");
		Printf("Usage: %s PROnOff\n", szArgv[0]);
		Printf("<OnOff>\n");
		Printf("off: Disable PR\n");
		Printf("on: Enable PR\n");

		return 1;
	}

    fgPROnOff = (x_strcmp(szArgv[1], "on") == 0) ? SV_ON : SV_OFF;
	vDrvMJCSetPRModeOnOff(fgPROnOff);

	return 0;
}


static INT32 _i4MjcQuery(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 1)
    {
        Printf("Usage: %s\n", szArgv[0]);
        return 1;
    }

    vDrvMJCQueryInOutCtrl();
    vDrvMJCQueryFrmSchCtrl();
    vDrvMJCQueryDramCtrl();
    vDrvMJCQuery3DCtrl();

    return 0;
}

static INT32 _i4MjcPRQuery(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 1)
    {
        Printf("Usage: %s\n", szArgv[0]);
        return 1;
    }

    vDrvMJCPRQueryCtrl();
    return 0;
}


static INT32 _i4MjcSetDispMode(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 2)
    {
        Printf("Current Status: %s\n",
            fgDrvMJCGetDispMode() == E_MJC_DISP_MODE ? "MJC_DISP_MODE" : "MJC_SELF_MODE");
        Printf("Usage: %s OnOff\n", szArgv[0]);
        Printf("<OnOff>\n");
        Printf("0: Off\n");
        Printf("1: On\n");

        return 1;
    }

    vDrvMJCSetDispMode(StrToInt(szArgv[1]));

    return 0;
}

static INT32 _i4MjcSetDramMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4DramModeIdx;
    const MJC_DRAM_MODE_T * prDramMode;

    if (i4Argc != 2)
    {
        prDramMode = rDrvMJCGetDramMode(MJC_DRAM_MODE_QUERY);
        Printf("Current Status: %s(0x%02X)\n", prDramMode->strDramMode, prDramMode->u2DramMode);
        Printf("Usage: %s DramMode\n", szArgv[0]);
        Printf("<DramMode>\n");

        for (u4DramModeIdx = 0; u4DramModeIdx < MJC_DRAM_ALL_MODE_NUM; u4DramModeIdx++)
        {
            prDramMode = rDrvMJCGetDramMode(u4DramModeIdx);
            Printf("%d: %s(0x%02X)\n", u4DramModeIdx, prDramMode->strDramMode, prDramMode->u2DramMode);
        }
        Printf("%d: Restore(0x%02X)\n", MJC_DRAM_ALL_MODE_NUM, arMJCDramAlloc.u2Mode);

        return 1;
    }

    u4DramModeIdx = StrToInt(szArgv[1]);

    MJC_SetDramModeDbg(u4DramModeIdx);

    return 0;
}

static INT32 _i4MjcSet3DMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u13DModeIdx;
    if (i4Argc != 2 )
    {
		Printf("Usage: %s u1Idx 3DMode\n", szArgv[0]);               
		Printf("<3DMode>\n");
		Printf("0: 2D\n");
		Printf("1: 3D FS\n");
		Printf("2: 3D SBS\n");
		Printf("3: 3D TAB\n");
        Printf("4: 3D TTD\n");
        Printf("5: 3D SS\n");  
		Printf(">=6: 3D QUERRY FROM FW");
        return 1;
    }
    u13DModeIdx = StrToInt(szArgv[1]);
    MJC_Set3DModeDbg(u13DModeIdx);

	return 0;
}


static INT32 _i4MjcSetPRMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1PRModeIdx ,u1FHPRModeIdx;
	UINT8  u1PRMode , u1HPRMode ,u1FPRMode;
    if (i4Argc != 3)
    {
        u1PRMode = u1DrvMJCGetPRMode();
		u1HPRMode = u1DrvMJCGetHPRMode();
		u1FPRMode = u1DrvMJCGetFPRMode();
		if(BSP_GetIcVersion() == IC_VER_5396_AA)
        {
            Printf("Current Status: PRMode:%d  HPRMode:%d  HPRMode:%d\n", u1PRMode , u1HPRMode ,u1FPRMode);
            Printf("Usage: %s PRMode\n", szArgv[0]);
            Printf("<PRMode>\n");
            Printf(" 0: E_MJC_NoPR\n 1: E_MJC_HPR\n 2: E_MJC_PR1\n 3: E_MJC_PR2\n");
		    Printf("<HFPRMode>\n");
            Printf(" 0:  E_MJC_NOHPR \n  1: HPR_120\n 2: HPR_60\n 3: HPR_240\n 4: HPR_2160\n");
		}
		else
		{
            Printf("Current Status: PRMode:%d  HPRMode:%d  HPRMode:%d\n", u1PRMode , u1HPRMode ,u1FPRMode);
            Printf("Usage: %s PRMode\n", szArgv[0]);
            Printf("<PRMode>\n");
            Printf(" 0: E_MJC_NO_HALF_PR\n 1: E_MJC_HPR\n");
		    Printf("<HFPRMode> or <PRVn> when pr mode = 0\n");
            Printf("<HFPRMode> 0:  E_MJC_NOHPR \n  1: HPR_120\n 2: HPR_60\n 3: HPR_240\n 4: HPR_2160\n");
			Printf("<PRVn> 0:  PRV0 \n 1: PRV1\n 2: PRV2\n 3: PRV3\n ");
		}
		return 1;
    }

    u1PRModeIdx = StrToInt(szArgv[1]);
	u1FHPRModeIdx = StrToInt(szArgv[2]);
    vDrvMJCSetPRMode(u1PRModeIdx , u1FHPRModeIdx);
   
    return 0;
}

static INT32 _i4MjcSetMJC1RPR1R(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 fgOnOff;
    if (i4Argc != 2 )
    {
		Printf("Usage: %s u1Idx Set MJC1RPR1R_Mode\n", szArgv[0]);               
		Printf("<MJC1RPR1R_Mode>\n");
		Printf("0: OFF\n");
		Printf("1: ON \n");
        return 1;
    }
    fgOnOff = StrToInt(szArgv[1]);
    vDrvMJCSet1W1Rmode(fgOnOff);

	return 0;
}

static INT32 _i4MjcSetMJCDbgInk(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1INKIdex;
    if (i4Argc != 2 )
    {
		Printf("Usage: %s u1Idx Set debug ink type\n", szArgv[0]);               
		Printf("<Debug INK>\n");
		Printf("0: Off debug ink\n");
		Printf("1: Set MJC Film Detect ink \n");
		Printf("2: Set MJC and DI Film Detect ink  \n");
		Printf("3: Set MJC APL_DBG ink \n");
		Printf("4: Set MJC SCN_DBG ink \n");
        return 1;
    }
    u1INKIdex = StrToInt(szArgv[1]);
    vDrvMJCSetDbgInk(u1INKIdex);

	return 0;
}

static INT32 _i4MjcCheck3DFSLR(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4NumOfFunc = 0;

    if (i4Argc != 2)
    {
		Printf("Usage: %s LRCheckOnOff\n", szArgv[0]);
		Printf("<OnOff>\n");
		Printf("0: Turn Off LR Check\n");
		Printf("1: Turn On LR Check\n\n");
        return 1;
    }
    
    while (MJCSPThread[u4NumOfFunc].pThreadFunc != NULL)
    {
        ++u4NumOfFunc;
    }

    u1MJCStatusCheck = (StrToInt(szArgv[1])? 3 : 6);
    Printf("vDrvMJC3DFSLRCheck:\n");
    (*(MJCSPVsyncIsr[2].ptrPfnVSyncFunc)) = MJCSPThread[u4NumOfFunc + 2].pThreadFunc;
 
    return 0;
}
static INT32 _i4MjcCheckCRC(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4NumOfFunc = 0;

    if (i4Argc != 2)
    {
		Printf("Usage: %s CRCCheckOnOff\n", szArgv[0]);
		Printf("<OnOff>\n");
		Printf("0: Turn Off CRC Check\n");
		Printf("1: Turn On CRC Check\n\n");
        return 1;
    }
    
    while (MJCSPThread[u4NumOfFunc].pThreadFunc != NULL)
    {
        ++u4NumOfFunc;
    }

    u1MJCStatusCheck = (StrToInt(szArgv[1])? 3 : 6);
    Printf("vDrvMJCCRCCheck:\n");
    (*(MJCSPVsyncIsr[1].ptrPfnVSyncFunc)) = MJCSPThread[u4NumOfFunc + 3].pThreadFunc;
    
    return 0;
}


static INT32 _i4MjcChkDramOvfUdf(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4NumOfFunc = 0;

    if (i4Argc != 2)
    {
		Printf("Usage: %s MonitorCount\n", szArgv[0]);
		Printf("<MonitorCount>\n");
		Printf("0: Turn Off DRAM Efficiency Monitor\n");
		Printf("0 ~ 0xFFFFFFFF: Enable DRAM Efficiency Monitor and Set Monitor Count\n");
        return 1;
    }
    
    while (MJCSPThread[u4NumOfFunc].pThreadFunc != NULL)
    {
        ++u4NumOfFunc;
    }

    u4MJCStatusPollingCount = StrToInt(szArgv[1]);
    if (u4MJCStatusPollingCount)
    {
        Printf("vDrvMJCDramOvfUdfPolling:\n");
        vDrvMJCClearDramStatus();
        vDrvMJCSetMVSortOnOff(SV_OFF);
        (*(MJCSPVsyncIsr[1].ptrPfnVSyncFunc)) = MJCSPThread[u4NumOfFunc + 1].pThreadFunc;
    }
    else
    {
        vDrvMJCSetMVSortOnOff(SV_ON);
    }

    return 0;
}

static INT32 _i4MjcSetFrmSchMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4FrmSchMode;
	
    if (i4Argc != 2)
    {
        Printf("Current Status: %d\n", arMJCPrm.arFRCConfig.u1FrmSchMode);
        Printf("Usage: %s FrmSchMode\n", szArgv[0]);
        Printf("<FrmSchMode>\n");
        Printf("0: MJC_FRAME_NO_DROP\n");
        Printf("1: MJC_PSEUDO_DROP_ALL\n");
        Printf("2: MJC_PSEUDO_DROP_ON_SCN_CHG\n");
        Printf("3: MJC_FRAME_DROP_LIKE\n");
        Printf("4: Restore\n");

        return 1;
    }

    u4FrmSchMode = StrToInt(szArgv[1]);

    MJC_SetFrmSchDbg(u4FrmSchMode);

    return 0;

}

static INT32 _i4MjcSetFlipMirror(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4FlipMirror = fgDrvMJCGetFlipMirror();

    if ((i4Argc != 2) || (StrToInt(szArgv[1]) > 3))
	{
		Printf("Current Status: flip = %d, mirror = %d\n",
		    (u4FlipMirror >> 1),
		    (u4FlipMirror &  1));
		Printf("Usage: %s FlipMirror\n", szArgv[0]);
		Printf("<FlipMirror>\n");
        Printf("0: no flip and mirror.\n");
        Printf("1: flip = 0, mirror = 1.\n");
        Printf("2: flip = 1, mirror = 0.\n");
        Printf("3: flip = 1, mirror = 1.\n");

		return 1;
	}

    u4FlipMirror = StrToInt(szArgv[1]);
    vDrvMJCSetFlipMirror((u4FlipMirror & 0x2) >> 1, (u4FlipMirror & 0x1));
    
    return 0;
}

static INT32 _i4MjcSetEffect(INT32 i4Argc, const CHAR **szArgv)
{
	UINT32 u4Effect;

	if ((i4Argc != 2) || ((i4Argc == 2) && (StrToInt(szArgv[1]) > 255)))
	{
        Printf("Current Status: %d\n", arMJCPrm.u1EffectParam);
		Printf("Usage: %s Effect\n", szArgv[0]);
		Printf("<Effect>\n");
		Printf("0: Off\n");
		Printf("1 ~ 255: Low ~ High\n");
		return 1;
	}

	u4Effect = StrToInt(szArgv[1]);
	
    MJC_SetEffect(u4Effect);

	return 0;
}

static INT32 _i4MjcStatusPolling(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4NumOfFunc, u4NumOfVsync;
    UINT32 u4SPIndex, u4VsyncIdx;
    
    u4NumOfFunc = 0;
    u4NumOfVsync = 0;
    while (MJCSPThread[u4NumOfFunc].pThreadFunc != NULL)
    {
        ++u4NumOfFunc;
    }

    while (MJCSPVsyncIsr[u4NumOfVsync].ptrPfnVSyncFunc != NULL)
    {
        ++u4NumOfVsync;
    }

	if (i4Argc < 4)
	{
		Printf("Usage: %s <index> <Vsync> <count>\n", szArgv[0]);
        for (u4SPIndex = 0; u4SPIndex < u4NumOfFunc; u4SPIndex++)
        {
            Printf("(%d) %s\n", u4SPIndex, MJCSPThread[u4SPIndex].szThreadDescription);
        }
        Printf("Vsync: (if not specified, default is 0)\n");
        for (u4SPIndex = 0; u4SPIndex < u4NumOfVsync; u4SPIndex++)
        {
            Printf("(%d) %s\n", u4SPIndex, MJCSPVsyncIsr[u4SPIndex].szVsyncDesc);
        }
		return 1;
	}

    u4VsyncIdx = 0;
    if (i4Argc == 4)
    {
        u4VsyncIdx = StrToInt(szArgv[2]);
        u4VsyncIdx = (u4VsyncIdx >= u4NumOfVsync) ? 0 : u4VsyncIdx;
    }

    u4SPIndex = StrToInt(szArgv[1]);
    if (u4SPIndex >= u4NumOfFunc)
    {
        for (u4SPIndex = 0; u4SPIndex < u4NumOfFunc; u4SPIndex++)
        {
            Printf("(%d) %s\n", u4SPIndex, MJCSPThread[u4SPIndex].szThreadDescription);
        }
        return 1;
    }

    u4MJCStatusPollingCount = StrToInt(szArgv[3]);
    if (u4MJCStatusPollingCount)
    {
        (*(MJCSPVsyncIsr[u4VsyncIdx].ptrPfnVSyncFunc)) = MJCSPThread[u4SPIndex].pThreadFunc;
        //if (u4SPIndex == 6)
        //{
        //}

        Printf("Status Polling Function is launched\n");
    }
    
	return 0;
}

static INT32 _i4MjcSetSPReg(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Index, u4Addr, u4Msb, u4Lsb;
    if (i4Argc != 5)
    {
		Printf("Usage: %s <index> <addr> <msb> <lsb>\n", szArgv[0]);
		vDrvMJCGetSPReg();
		return 1;
    }

    u4Index = StrToInt(szArgv[1]);
    u4Addr  = StrToInt(szArgv[2]);
    u4Msb   = StrToInt(szArgv[3]);
    u4Lsb   = StrToInt(szArgv[4]);
    if (u4DrvMJCSetSPReg(u4Index, u4Addr, u4Msb, u4Lsb) == SV_TRUE)
    {
        Printf("Set SP Register Successful\n");
    }
    else
    {
        Printf("Set SP Register Failed\n");
    }
    return 0;
}

static INT32 _i4MjcSetMute(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
        Printf("Current MJC mute delay: %d\n", u1DrvMJCGetMuteCount());
        Printf("Usage: %s <mute delay: 0(off)/1~255(on)>\n", szArgv[0]);
	}
    else
    {
        vDrvMJCSetMuteCount(StrToInt(szArgv[1]));
    }

	return 0;
}

static INT32 _i4MjcMbistGo(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 1;
	}

    vDrvMJCRunMbist();

	return 0;
}

static INT32 _i4MjcSPEna(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 3)
	{
        Printf("Usage: %s <bit (0~31)> <count>\n", szArgv[0]);
	}
    vDrvMJCSetSpEn((1 << StrToInt(szArgv[1])), StrToInt(szArgv[2]));

    return 0;
}

static INT32 _i4MjcSetEffectMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4RemapMode;

    u4RemapMode = StrToInt(szArgv[1]);

	if ((i4Argc != 2) || (u4RemapMode > E_MJC_EFFECT_MODE_2SEGMENT))
	{
        Printf("Usage: %s EffectMode\n", szArgv[0]);
        Printf("<EffectMode>\n");
        Printf("0: MJC_EFFECT_MODE_NO_REMAP\n");
        Printf("1: MJC_EFFECT_MODE_DFT\n");
        Printf("2: MJC_EFFECT_MODE_2SEGMENT\n");
        return 1;
	}
    else
    {
        switch (u4RemapMode)
        {
        default:
        case 0:
            MJC_SetEffectRemapMode(E_MJC_EFFECT_MODE_NO_REMAP);
            break;
        case 1:
            MJC_SetEffectRemapMode(E_MJC_EFFECT_MODE_DFT);
            break;
        case 2:
            MJC_SetEffectRemapMode(E_MJC_EFFECT_MODE_2SEGMENT);
            break;
        }
    }

    return 0;
}

static INT32 _i4MjcSetEffectDFT(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 4)
	{
        Printf("Usage: %s Coring Offset Weighting\n", szArgv[0]);
        Printf("<Coring>\n");
        Printf("0 ~ 64\n");
        Printf("<Offset>\n");
        Printf("0 ~ 64\n");
        Printf("<Weighting>\n");
        Printf("0 ~ 255\n");
        return 1;
	}
	else
	{
	    MJC_SetRemapDFTDbg(StrToInt(szArgv[1]), StrToInt(szArgv[2]), StrToInt(szArgv[3]));
	}

	return 0;
}

static INT32 _i4MjcSetDemo(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc == 3)
    {
        MJC_SetDemoNew((E_MJC_NEW_DEMO_MODE)StrToInt(szArgv[1]), StrToInt(szArgv[2]));
    }
    else if (i4Argc == 4)
    {
        MJC_SetDemo(StrToInt(szArgv[1]), StrToInt(szArgv[2]), StrToInt(szArgv[3]));
    }
    else
    {
        Printf("Usage(new): %s <mode> <param>\n", szArgv[0]);
        Printf("Usage(old): %s <mode> <param1> <param2>\n", szArgv[0]);
    }

    return 0;
}

static INT32 _i4MjcChkFrmDly(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
        Printf("Usage: %s OnOff\n", szArgv[0]);
        Printf("<OnOff>\n");
        Printf("0: Off\n");
        Printf("1: On\n");
        return 1;
	}
	
    vDrvMJCFrmDlyChkOnOff(StrToInt(szArgv[1]));

	return 0;
}

static INT32 _i4MjcSetFreeze(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 2)
    {
        Printf("Usage: %s OnOff\n", szArgv[0]);
        Printf("<OnOff>\n");
        Printf("0: Off\n");
        Printf("1: On\n");
        return 1;
    }

    vDrvMJCSetFrmFreeze(StrToInt(szArgv[1]));

    return 0;
}

static INT32 _i4MjcSetFB1Frm(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 2)
    {
        Printf("Usage: %s OnOff\n", szArgv[0]);
        Printf("<OnOff>\n");
        Printf("0: Off\n");
        Printf("1: On\n");
        return 1;
    }

    vDrvMJCSetFBOneFrmOnOff(StrToInt(szArgv[1]));

    return 0;
}

static INT32 _i4MjcSetTTDUIDepth(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 2)
    {
        Printf("Usage: %s convergenceDepth\n", szArgv[0]);
        return 1;
    }

    //vDrvTTDSetConvergenceDepthUI(StrToInt(szArgv[1]));

    return 0;
}

static INT32 _i4MjcSetTTDFWMODE(INT32 i4Argc, const CHAR **szArgv)
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

static INT32 _i4MjcSetProgMEMode(INT32 i4Argc, const CHAR **szArgv)
{
   if (i4Argc != 3)
    {
        Printf("Usage: %s <0(240Hz)/1(120Hz)> <0(64)/1(55P2)>\n", szArgv[0]);

        return 1;
    }

    vDrvCliCallPME(StrToInt(szArgv[1]),StrToInt(szArgv[2]));

    return 0;
}


#if 1
static INT32 _i4MjcSetTTDOnOff(INT32 i4Argc, const CHAR **szArgv)
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

    vDrvMJCSetTTDOnOff(u1OnOff);
    return 0;
}

static INT32 _i4MjcSetTTDMode(INT32 i4Argc, const CHAR **szArgv)
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

static INT32 _i4MjcSetTTDPicSize(INT32 i4Argc, const CHAR **szArgv)
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


