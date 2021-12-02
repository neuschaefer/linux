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
#include "x_pinmux.h"
#include "x_debug.h"
#include "x_mid.h"
#include "source_table.h"
#include "sv_const.h"
#include "drv_di.h"
#include "drv_di_int.h"
#include "vdo_misc.h"
#include "video_def.h"

#include "fbm_drvif.h"

#include "drv_scaler.h"
#include "vdp_if.h"


#ifndef CC_CLI
#define CC_CLI
#endif

#ifdef CC_CLI
/* extern variale */
#define GET_PS_WIDTH(dividend, divisor)        (((dividend) + ((divisor) - 1)) / (divisor))

/* static function */
static INT32 _i4DSetBitRes(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiFWOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiInit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiDemo(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiTrickModeOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiSetColorMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiSetEvent(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiHPSTest(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetDebugDataMode(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiGetInfo(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiStatus(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetInk(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetOSD(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetMid(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetFreeze(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetFieldInverse(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiForceRead8bit(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetWritePrecision(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetPorchTest(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiLogReg(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiLogAll(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiSetFlipMirror(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiSetCrcIntSelect(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetCrcLog(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiInitCrc(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiChkSta(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiClearStatus(INT32 i4Argc, const CHAR ** szArgv);	

#ifdef DYNAMIC_RES_VERIFY
static INT32 _i4DiForcePscanOverscan(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiDynRes(INT32 i4Argc, const CHAR ** szArgv);
#endif

#ifdef PSCAN_AUTO_VERIFY

static INT32 _i4DiQueryINKLevel(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiCheckInkResult(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiDumpAllHistogram(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetDataSize(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiGetClipLength(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiQueryINKStatus(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiAutoMationLoop(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiAutoCrcLoop(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiDumpAllCrc(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetForceMotion(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetDetectCRC(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiGetVdoinCRC(INT32 i4Argc, const CHAR ** szArgv);
#endif

INT32 _iDiSetOutputPattern(INT32 i4Argc, const CHAR **szArgv);
INT32 _iDiSetInputBorder(INT32 i4Argc, const CHAR **szArgv);
INT32 _i4DiOnOff(INT32 i4Argc, const CHAR **szArgv);
#ifdef CC_SUPPORT_H264
static INT32 _i4DiSetH264WriteMode(INT32 i4Argc, const CHAR ** szArgv);
#endif
CLIMOD_DEBUG_FUNCTIONS(DI)

#endif /*#ifdef CC_CLI*/

/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for DI command entry
CLI_EXEC_T arYcprocDiTbl[] = {
#ifdef CC_CLI
    {"SetBitRes", "sbr", _i4DSetBitRes, NULL, "Set bit resolution", CLI_SUPERVISOR},
    {"FWOnOff", "fwoo", _i4DiFWOnOff, NULL, "Enable/disable MDDi FW", CLI_SUPERVISOR},
    {"OnOff", "oo", _i4DiOnOff, NULL, "Enable/disable MDDi/DRAM controller", CLI_SUPERVISOR},
    {"Init", "i", _i4DiInit, NULL, "Initialize MDDi register values", CLI_SUPERVISOR},
    {"DemoParam", "dp", _i4DiDemo, NULL, "Set demo mode active region", CLI_SUPERVISOR},
    {"TrickModeOnOff", "tmoo", _i4DiTrickModeOnOff, NULL, "Turn on/off trick mode", CLI_SUPERVISOR},
    {"SetColorMode", "scm", _i4DiSetColorMode, NULL, "Switch between 420/422 color mode", CLI_SUPERVISOR},
    {"Polling", "p", _i4DiStatus, NULL, "DI QTY Status Polling", CLI_SUPERVISOR},    
    {"SetInk", "ink", _i4DiSetInk, NULL, "Set Ink", CLI_SUPERVISOR},
    {"SetEvent", "se", _i4DiSetEvent, NULL, "DI Set Event", CLI_SUPERVISOR},
    {"HPSTest", "hpst", _i4DiHPSTest, NULL, "HPS test", CLI_SUPERVISOR},
    {"SetDebugDataMode", "sddm", _i4DiSetDebugDataMode, NULL, "Set debug data mode", CLI_SUPERVISOR},
    {"GetInfo", "g", _i4DiGetInfo, NULL, "Get PSCAN debug Information", CLI_SUPERVISOR},
    {"SetInk", "osd", _i4DiSetOSD, NULL, "Set PSCAN ink control", CLI_SUPERVISOR},
    {"SetMid", "smid", _i4DiSetMid, NULL, "Set PSCAN Dram Agent", CLI_SUPERVISOR},
    {"SetFreeze", "f", _i4DiSetFreeze, NULL, "Set PSCAN Freeze", CLI_SUPERVISOR},
    {"SetFieldInverse", "sfld", _i4DiSetFieldInverse, NULL, "Set Field Inverse", CLI_SUPERVISOR},
    {"ForceRead8bit", "srbit", _i4DiForceRead8bit, NULL, "Force Read Precision Selection", CLI_SUPERVISOR},
    {"SetWritePrecision", "swbit", _i4DiSetWritePrecision, NULL, "Write Precision Selection", CLI_SUPERVISOR},
    {"PorchTest", "spt", _i4DiSetPorchTest, NULL, "Set Porch test", CLI_SUPERVISOR},
    {"LogReg", "la", _i4DiLogReg, NULL, "Log Register access", CLI_SUPERVISOR},
    {"LogAll", "logall", _i4DiLogAll, NULL, "Log all information", CLI_SUPERVISOR},
    {"SetOutputPattern", "ptgn", _iDiSetOutputPattern, NULL, "Set output pattern", CLI_SUPERVISOR},
    {"SetInputBorder", "ibor", _iDiSetInputBorder, NULL, "Set input border", CLI_SUPERVISOR},
    {"SetFlipMirror", "fm", _i4DiSetFlipMirror, NULL, "Set flip mirror mode", CLI_SUPERVISOR},
    {"CRC Test", "crclog", _i4DiSetCrcLog, NULL, "crc", CLI_SUPERVISOR},
    {"CRC interrupt edge", "crcint", _i4DiSetCrcIntSelect, NULL, "crc", CLI_SUPERVISOR},
    {"Initial CRC", "crc", _i4DiInitCrc, NULL, "Initialize crc test", CLI_SUPERVISOR},
    {"Check status", "chksta", _i4DiChkSta, NULL, "chksta", CLI_SUPERVISOR},
    {"Clear status", "clrsta", _i4DiClearStatus, NULL, "clrsta", CLI_SUPERVISOR},
#ifdef DYNAMIC_RES_VERIFY
    {"InitDynRes", "dynres", _i4DiDynRes, NULL, "Init Dyn res", CLI_SUPERVISOR},
    {"OverscanAtMIB", "mibovs", _i4DiForcePscanOverscan, NULL, "Overscan at MIB", CLI_SUPERVISOR},
#endif    
#ifdef PSCAN_AUTO_VERIFY
    {"QueryInkLevel", "query", _i4DiQueryINKLevel, NULL, "Query Ink Histogrm of Specific scaling factor/block/frame", CLI_SUPERVISOR},
    {"QueryInkStatus", "qis", _i4DiQueryINKStatus, NULL, "Query Ink Status of Current Block", CLI_SUPERVISOR},
    {"CheckInkResult", "cir", _i4DiCheckInkResult, NULL, "Check Ink Result", CLI_SUPERVISOR},
    {"DumpAllHistogram", "dah", _i4DiDumpAllHistogram, NULL, "Dump All Histogram", CLI_SUPERVISOR},
    {"SetDataSize", "sds", _i4DiSetDataSize, NULL, "Set Data Stack Size", CLI_SUPERVISOR},
    {"GetClipLength", "gcl", _i4DiGetClipLength, NULL, "GetClipLength", CLI_SUPERVISOR},
    {"HPSAutomation", "aml", _i4DiAutoMationLoop, NULL, "HPS automation test", CLI_SUPERVISOR},
    {"HPSCrcAuto", "acl", _i4DiAutoCrcLoop, NULL, "HPS crc automation test", CLI_SUPERVISOR},
    {"DumpAllCrcResult", "dac", _i4DiDumpAllCrc, NULL, "Dump All Crc Result", CLI_SUPERVISOR},
    {"ForceMotionLevel", "sfm", _i4DiSetForceMotion, NULL, "Set Force Motion Level", CLI_SUPERVISOR},
    {"SetDetectCRC", "sdc", _i4DiSetDetectCRC, NULL, "Set Detect CRC", CLI_SUPERVISOR},
    {"GetVdoinCRC", "gvc", _i4DiGetVdoinCRC, NULL, "Get Vdoin CRC", CLI_SUPERVISOR},
#endif
#ifdef CC_SUPPORT_H264
    {"SetH264WriteMode", "swm", _i4DiSetH264WriteMode, NULL, "Set Write Mode for H264", CLI_SUPERVISOR},
#endif
CLIMOD_DEBUG_CLIENTRY(DI),
#endif /*#ifdef CC_CLI*/

  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/
static INT32 _i4DSetBitRes(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> bit-resolution\n", szArgv[0]);
    	return 1;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    vDrvDISetBitRes(u1Path, ((UINT32)StrToInt(szArgv[2])));

    return 0;
}

static INT32 _i4DiFWOnOff(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc < 3)
    {
    	Printf("Usage: %s <0:Main Ctrl / 1:Sub Ctrl / 2:Quality> <0:off / 1:on>\n", szArgv[0]);
    	return 1;
    }

    vDrvDIFWOnOff(StrToInt(szArgv[1]), StrToInt(szArgv[2]));	

    return 0;
}

INT32 _i4DiOnOff(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;
    UINT8 u1OnOff;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> <on/off> \n  or\n   %s <main:0/sub:1>  <On:0/Bypass:1)\n", szArgv[0], szArgv[0]);
    	return 1;
    }
        
    u1Path = (((x_strcmp(szArgv[1], "m") == 0) ||(StrToInt(szArgv[1])==0)) ? SV_VP_MAIN : SV_VP_PIP);
    u1OnOff = (((x_strcmp(szArgv[2], "on") == 0) ||(StrToInt(szArgv[2])==1)) ? SV_ON : SV_OFF);
    vDrvDIOnOff(u1Path, u1OnOff);

    return 0;
}

static INT32 _i4DiInit(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;

    if (i4Argc < 2)
    {
    	Printf("Usage: %s <m/p>\n", szArgv[0]);
    	return 1;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    vDrvDIInit(u1Path);

    return 0;
}

static INT32 _i4DiDemo(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1OnOff;
    UINT8 u1Region;

    if (i4Argc < 5)
    {
    	Printf("Usage: %s <m/p> <on/off> <l/r> center\n", szArgv[0]);
    	return 1;
    }

    u1OnOff = ((x_strcmp(szArgv[2], "on") == 0) ? SV_ON : SV_OFF);
    u1Region = ((x_strcmp(szArgv[3], "l") == 0) ? SV_LEFT : SV_RIGHT);

    vDrvDIDemo(u1OnOff, u1Region, ((UINT32)StrToInt(szArgv[4])));

    return 0;
}

static INT32 _i4DiTrickModeOnOff(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;
    UINT8 u1OnOff;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> <on/off>\n", szArgv[0]);
    	return 1;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    u1OnOff = ((x_strcmp(szArgv[2], "on") == 0) ? SV_ON : SV_OFF);

    vDrvDITrickModeOnOff(u1Path, u1OnOff);

    return 0;
}

static INT32 _i4DiSetColorMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;
    UINT8 u1420Mode;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> <420/422>\n", szArgv[0]);
    	return 1;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    u1420Mode = ((x_strcmp(szArgv[2], "420") == 0) ? SV_TRUE : SV_FALSE);

    vDrvDISetColorMode(u1Path, u1420Mode);

    return 0;
}

static INT32 _i4DiStatus(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    INT32 bPara1;
    INT32 i,u4NumOfFunc=0, iTestLoop = 0xFFFFFFFF;
    
    static HANDLE_T _hThread;

    while(DIPQThread[u4NumOfFunc].pThreadFunc !=NULL)
    {
        ++u4NumOfFunc;
    }

    if (i4Argc < 3)
    {
        for(i=0;i<u4NumOfFunc;++i)
        {
            Printf("\n %d)%s",(int)i,DIPQThread[i].szThreadDescription);
        }
        
        bPara1=0;
        fgDIStatusPolling=0;
        return 1;       
    }
    else
    {
        bPara1=StrToInt(szArgv[1]);
        fgDIStatusPolling = (x_strcmp(szArgv[2], "on") == 0);

        if (i4Argc == 4)
        {
            iTestLoop = StrToInt(szArgv[3]);
        }
    }

    if(bPara1 >=u4NumOfFunc)
    {
        for(i=0;i<u4NumOfFunc;++i)
        {
            Printf("\n %d)%s",(int)i,DIPQThread[i].szThreadDescription);
        }        
        return 0;
    }

    if (fgDIStatusPolling)
    {
        i4Ret = x_thread_create(&_hThread, "DI STATUS POLLING", 4096, 100,
            DIPQThread[bPara1].pThreadFunc, sizeof(iTestLoop), (void*)&iTestLoop);

        if (i4Ret != OSR_OK)
        {
            Printf("Status Polling Thread cannot be created\n");
            return 1;
        }
    }
    return 0;
}

static INT32 _i4DiSetInk(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc == 1)
    {
        vDrvDISetInk(0, 0);
    }
    else if(i4Argc == 2)
    {
        vDrvDISetInk(StrToInt(szArgv[1]), 0);        
    }
    else if(i4Argc == 3)
    {
        vDrvDISetInk(StrToInt(szArgv[1]), StrToInt(szArgv[2]));
    }

    return 1;
}

static INT32 _i4DiSetEvent(INT32 i4Argc, const CHAR ** szArgv)
{
    #if defined(CC_MT5387)
    UINT32 u4Param1,u4Param2,u4Param3;
    if (i4Argc <4)
    {
        u4Param1=0;
        u4Param2=0xFF;
        u4Param3=0;
    }
    else
    {
        u4Param1=StrToInt(szArgv[1]);
        u4Param2=StrToInt(szArgv[2]);
        u4Param3=StrToInt(szArgv[3]);    
    }

    vDrvDISetEvent(u4Param1,u4Param2,u4Param3);
    #endif
    return 0;
}

static INT32 _i4DiHPSTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Path;
    UINT16 u2Cnt, u2Delay, u2Step = 1;
    UINT16 u2High, u2Low;
    UINT16 u2Width, u2WidthPre=0;
    
    if (i4Argc < 5)
    {
    	Printf("Usage: %s <m/p> high low delay (option:step) \n", szArgv[0]);
    	return 1;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    u2High = StrToInt(szArgv[2]);
    u2Low = StrToInt(szArgv[3]);
    u2Delay = StrToInt(szArgv[4]);
    
    if (i4Argc == 6)
    {
        u2Step = StrToInt(szArgv[5]);
    }
    
    for (u2Cnt = u2High; u2Cnt >= u2Low; u2Cnt--)
    {
        u2Width = GET_PS_WIDTH(((UINT32)wDrvVideoInputWidth(u1Path) * u2Cnt), 0x8000);

        if (u2WidthPre != u2Width)
        {       
            if ((u2Width%u2Step) == 0)
            {
                u2WidthPre = u2Width;
                
                Printf("\nTest Width = %d  ratio = 0x%x\n", u2Width, u2Cnt);
                
            	CLI_Parser_Arg("vdp.s.hps %d %d", u1Path, u2Cnt);
            	x_thread_delay(u2Delay*1000);
            }
        }
    }

    return 0;
}

static INT32 _i4DiSetDebugDataMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 i,sep=0xff;
    UINT8 u1Path, u1DataModeIdx;
    EXTERN UINT32 VDP_SetDispmode(UCHAR ucVdpId, UCHAR ucMode);
    
    if (i4Argc < 3)
    {
        
        Printf("\nSelect Data Mode\n");
        for(i=0;aDataMode[i].eMIBMode!= E_MIB_DMODE_END;++i)
        {
            if(sep!= (((UINT32)aDataMode[i].eMIBMode)&0xF))
            {
                sep = (((UINT32)aDataMode[i].eMIBMode)&0xF);
                Printf("-------------------------------------------------\n");
            }
            Printf("%2d :Data Mode --%s\n",i,aDataMode[i].szModeName);            
        }
        Printf("-------------------------------------------------\n");            
        Printf("Current debug data mode, main %s\n",szDrvDataModeToString(SV_VP_MAIN));
        Printf("Current debug data mode, sub  %s\n",szDrvDataModeToString(SV_VP_PIP));
        return 1;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    u1DataModeIdx = StrToInt(szArgv[2]);    
    vDrvDISetDebugDataMode(u1Path, (aDataMode[u1DataModeIdx].eMIBMode));

    if(aDataMode[u1DataModeIdx].eMIBMode != E_MIB_DMODE_UNKNOWN)
    {
        VDP_SetDispmode(u1Path, ((((UINT32)aDataMode[u1DataModeIdx].eMIBMode)&0xF) ==E_MIB_FRC_MIB) ? 3 : 5 );
    }
    else
    {
        if (_arMDDiPrm[u1Path].rFbmPool.u4Mode & FBM_POOL_MODE_MDDI_DISP)
        {
            VDP_SetDispmode(u1Path, 3);//FRC_MIB
        }
        else
        {
            VDP_SetDispmode(u1Path, 5);// FRC_SCL  96
        }
    }
    
#ifdef CC_FPGA    
    vDrvMIBProc();
#endif

    return 0;
}

static INT32 _i4DiGetInfo(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> <0:DI Info><1: NR Info><2: Overflow/Underflow Flag Info>\n", szArgv[0]);
    	return 1;
    }

    vDrvDIGetInfo(((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP), 
        StrToInt(szArgv[2]));	

    return 0;
}

static INT32 _i4DiSetOSD(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc < 3)
    {
    	Printf("Usage: %s inkmode <0:off / 1:on>\n", szArgv[0]);
     	Printf("<0:Probe,    1:Film,     2:CS,    3:MEMA,>\n");
        Printf("<4:KENERL,   5:Input Border,   6:Output Border>\n");
    	return 1;
    }
    vDrvDISetOSD(StrToInt(szArgv[1]), StrToInt(szArgv[2]));	

    return 0;
}

static INT32 _i4DiSetMid(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 fgOk;
    UINT8 bPath;
    UINT32 u4RegID, u4LowAddr, u4HighAddr;
    MID_AGENT_ID_T eAgentId;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> <0:off / 1:on>\n", szArgv[0]);
    	return 1;
    }

    bPath = (x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP;    
    u4RegID = (x_strcmp(szArgv[1], "m") == 0) ? 3 : 4;
    eAgentId = MID_StringToAgentId("pscan_nr");

    if (StrToInt(szArgv[2]))
    {
        vDrvDIGetBufAddr(bPath, &u4LowAddr, &u4HighAddr);

        MID_ResetRegion(u4RegID);
        
        fgOk = MID_SetRegionEx(u4RegID, eAgentId, u4LowAddr, u4HighAddr, 0);
        
        Printf("%s %s Set Mid Region %d (0x%08x - 0x%08x)  %s\n",
            MID_AgentIdToString(eAgentId),
    	    (bPath == SV_VP_MAIN) ? "Main" : "Sub", u4RegID,
            u4LowAddr, u4HighAddr, fgOk ? "Succeeded" : "Failed");
        
        MID_EnableRegionProtect(u4RegID);
    }
    else
    {
        MID_DisableRegionProtect(u4RegID);         
    }

    return 0;
}

static INT32 _i4DiSetFreeze(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> <on: 1/off: 0>\n", szArgv[0]);
    	return 1;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    vDrvDIFreezeOnOff(u1Path, ((UINT16)StrToInt(szArgv[2])));

    return 0;
}

static INT32 _i4DiSetFieldInverse(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> <inverse:1/normal:0>\n", szArgv[0]);
    	return 1;
    }
    
    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    vDrvDISetFieldInverse(u1Path, ((UINT16)StrToInt(szArgv[2])));
    
    return 0;
}

static INT32 _i4DiForceRead8bit(INT32 i4Argc, const CHAR **szArgv)
{
    //UINT8 u1Path;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> <8bit :1/10bit :0>\n", szArgv[0]);
    	return 1;
    }
    
    //u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    //vDrvDIForceRead8bit(u1Path, ((UINT16)StrToInt(szArgv[2])));
    
    return 0;
}

static INT32 _i4DiSetWritePrecision(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> <8bit :1/10bit :0>\n", szArgv[0]);
    	return 1;
    }
    
    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    vDrvDISetWritePrecision(u1Path, ((UINT16)StrToInt(szArgv[2])));
    
    return 0;
}

static INT32 _i4DiSetPorchTest(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;
    int i;
    int iStart, iEnd, idelay;

    if (i4Argc < 5)
    {
    	Printf("Usage: %s <main:0/ sub:1> <Start_Porch> <End_Porch> <Delay>\n\n", szArgv[0]);

        Printf("Main Path:\n %6s %6s %6s %6s %6s %6s %6s %6s\n",
            "0)hcurr","1)hdef","2)hmin","3)hmax","4)vcurr","5)vdef","6)vmin","7)vmax");
        for (i=0; i<=SV_VPORCH_MAX; i++)
        {
            Printf("%6d ",wDrvVideoGetPorch(SV_VP_MAIN,i));
        }
        Printf("\n");
        
        Printf("Sub Path:\n %6s %6s %6s %6s %6s %6s %6s %6s\n",
            "0)hcurr","1)hdef","2)hmin","3)hmax","4)vcurr","5)vdef","6)vmin","7)vmax");
        for (i=0; i<=SV_VPORCH_MAX; i++)
        {
            Printf("%6d ",wDrvVideoGetPorch(SV_VP_PIP,i));
        }
        Printf("\n");
    	return 1;
    }
    else
    {
        Printf("Set porch test from %d to %d\n", StrToInt(szArgv[2]), StrToInt(szArgv[3]));
        u1Path = StrToInt(szArgv[1]);
        iStart = StrToInt(szArgv[2]);
        iEnd = StrToInt(szArgv[3]);
        idelay = StrToInt(szArgv[4]);

        for (i = iStart; i <= iEnd; i++)
        {
            Printf("Set porch %d \n", i);
            wDrvVideoSetPorch(u1Path, 4, i);
            x_thread_delay(idelay*1000);
        }
    }
    return 0;
}

static INT32 _i4DiLogReg(INT32 i4Argc, const CHAR **szArgv)
{
#ifdef LOG_MDDI_REG
    MDDI_PRINT_REG_LOG();
#endif
    return 0;
}

static INT32 _i4DiLogAll(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc < 2)
    {
    	Printf("Usage: %s <m/p>\n", szArgv[0]);
    	return 1;
    }

    Printf("5365 PSCAN Memory dump\n");
    if (x_strcmp(szArgv[1], "m") == 0)
        CLI_Parser_Arg("r 0xf0024000 0x800");
    else
        CLI_Parser_Arg("r 0xf0024800 0x800");
    
    Printf("\n\n");
    CLI_Parser_Arg("n.di.g %d %d", ((x_strcmp(szArgv[1], "m") == 0) ? "m" : "p"), 0);
    Printf("\n\n");
    CLI_Parser_Arg("fbm.d_l 3");
    Printf("\n\n");
    CLI_Parser_Arg("fbm.q");
    Printf("\n\n");
    CLI_Parser_Arg("vdp.d_l 2");  
    Printf("\n\n");
    CLI_Parser_Arg("vdp.q");    
    
    return 0;
}

INT32 _iDiSetOutputPattern(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <main:0/ sub:1> <On :1/Off :0>\n", szArgv[0]);
    	return 1;
    }
    
    //u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    u1Path = StrToInt(szArgv[1]);
    vDrvDISetPattern(u1Path, ((UINT16)StrToInt(szArgv[2])));
    
    return 0;
}

INT32 _iDiSetInputBorder(INT32 i4Argc, const CHAR **szArgv)
{
    //UINT8 u1Path;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <main:0/ sub:1> <On :1/Off :0>\n", szArgv[0]);
    	return 1;
    }
    
    //u1Path = StrToInt(szArgv[1]);
    //vDrvDISetInputBorder(u1Path, ((UINT16)StrToInt(szArgv[2])));
    
    return 0;
}

static INT32 _i4DiSetFlipMirror(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> <Mode:\n", szArgv[0]); 	
    	Printf("0:DI_NORMAL_MODE\n"); 
        Printf("1:DI_MIRROR_ON_MODE\n"); 
        Printf("2:DI_FLIP_ON_MODE\n"); 
        Printf("3:DI_FLIP_MIRROR_ON_MODE\n"); 
        Printf("4:DI_FLIP_MIRROR_UNKOWN\n"); 
    	return 1;
    }
    
    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    vDrvDISetDbgFlipMirror(u1Path, ((UINT16)StrToInt(szArgv[2])));
    
    return 0;
}

static INT32 _i4DiSetCrcLog(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        Printf("Usage: %s <1:SRC CRC 2:OUT CRC>", szArgv[0]);
        return 1;
    }
    
    //vDrvDISetCrc(0, ((UINT32)StrToInt(szArgv[1])));
    return 0;
}

static INT32 _i4DiSetCrcIntSelect(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        Printf("Usage: %s <0:Output R 1:Output F 2:Input R 3:Input F>", szArgv[0]);
        return 1;
    }
    
    //vDrvDISetCrc(1, ((UINT32)StrToInt(szArgv[1])));
    return 0;
}


#ifdef PSCAN_AUTO_VERIFY

static INT32 _i4DiQueryINKLevel(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Block;
    UINT16 u2Frame;
    UINT16 u2Factor;
    
    if (i4Argc != 4)
    {
        Printf("Usage: %s factor_idx block frame\n", szArgv[0]);
        return 1;
    }

    u2Factor = StrToInt(szArgv[2]);
    u1Block = StrToInt(szArgv[3]);
    u2Frame = StrToInt(szArgv[4]);
    vDrvDIQueryInkLevel(u2Factor, u1Block, u2Frame);
    return 0;
}

static INT32 _i4DiCheckInkResult(INT32 i4Argc, const CHAR ** szArgv)
{        
    UINT32 u4Idx;
    if (i4Argc != 2)
    {
        Printf("Usage: %s Scale_idx\n", szArgv[0]);
        return 1;
    }
    
    u4Idx = StrToInt(szArgv[1]);
    if (u1DrvDICheckResult(u4Idx))
    {
        Printf("Hardware Function Check is OK!\n");
    }
    else
    {
        Printf("Hardware Function Check is NG!\n");
    }
    return 0;
}

static INT32 _i4DiDumpAllHistogram(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Path;
    
    if (i4Argc != 2)
    {
        Printf("Usage: %s <m/p>\n", szArgv[0]);
        return 1;
    }
    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    
    if (u1InkOnOff[u1Path])
    {
        Printf("Automation is running!\n");
    }
    else
    {
        vDrvDIDumpAllHistogram();
    }
    return 0;
}

static INT32 _i4DiSetDataSize(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2FactorSize;
    UINT16 u2FrameSize;

    if (i4Argc != 3)
    {
        Printf("Usage: %s factor_size frame_size\n", szArgv[0]);
        return 1;
    }

    u2FactorSize = StrToInt(szArgv[1]);
    u2FrameSize = StrToInt(szArgv[2]);
    vDrvDISetDataSize(u2FactorSize, u2FrameSize);
    return 0;
}

static INT32 _i4DiGetClipLength(INT32 i4Argc, const CHAR ** szArgv)
{            
    UINT8 u1Path;
    
    if (i4Argc != 2)
    {
        Printf("Usage: %s <m/p>\n", szArgv[0]);
        return 1;
    }
    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);

    vDrvDISetCurrentState();
    u1GetLength[u1Path] = 1;
    return 0;
}

static INT32 _i4DiQueryINKStatus(INT32 i4Argc, const CHAR ** szArgv)
{
    vDrvDIQueryInkStatus();
    return 0;
}

static INT32 _i4DiAutoMationLoop(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Path;
    UINT8 u1Auto;
    UINT16 u2High;
    UINT16 u2Low;
    UINT16 u2Cnt;
    UINT16 u2Width, u2WidthPre=0;
    BOOL fgIsMain;

    if (i4Argc < 5)
    {
    	Printf("Usage: %s <m/p> high low auto\n", szArgv[0]);
    	return 1;
    }
     
    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    u2High = StrToInt(szArgv[2]);
    u2Low = StrToInt(szArgv[3]);
    u1Auto= StrToInt(szArgv[4]);
    fgIsMain = (u1Path == VDP_1);
    
    u2FactorIdx = 0;
    u2Cnt = u2High;
    do
    {
        u2Width = GET_PS_WIDTH(((UINT32)wDrvVideoInputWidth(u1Path) * u2Cnt), 0x8000);

        if (u1Auto)
        {
            if (u1DrvDIOutOfBound())
            {
                Printf("u2FactorIdx is larger than SCALE_SIZE\nStop recording\n");
                u1InkOnOff[u1Path]= 0;
                break;
            }
        }
        
        if (u2WidthPre != u2Width)
        {       
            if ((u2Width%2) == 0) // only support even
            {
                u2WidthPre = u2Width;
                #if 0
                vDrvSetRelatedParam(u1Path, u2Cnt, u2Width); // for FPGA verify use
                #else
                CLI_Parser_Arg("vdp.s.hps %d %d", u1Path, u2Cnt); // for IC verify use
                while (fgIsScposFlgSet((fgIsMain) ? MAIN_DISP_PRM_CHG : PIP_DISP_PRM_CHG)); // for IC verify use
                #endif
                x_thread_delay(1000);
                
                if (u1Auto)
                {
                    vDrvDIInitInkRange(u2Cnt, u2Width, wDrvVideoInputHeight(u1Path));
                    vDrvDISetInkBlock(u1Path, 0);
                    u1InkOnOff[u1Path] = 1;
                }
            }
        }
        if (u1Auto)
        {
            while (u1InkOnOff[u1Path]); // wait until ink histogram polling
        }
        u2Cnt--;
    } while (u2Cnt >= u2Low);
    return 0;
}

static INT32 _i4DiAutoCrcLoop(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgIsMain;
    UINT8 u1Path;
    UINT16 u2High;
    UINT16 u2Low;
    UINT16 u2Cnt;
    UINT16 u2Width, u2WidthPre=0;
    
    if (i4Argc < 4)
    {
    	Printf("Usage: %s <m/p> high low <0: input/1: output>\n", szArgv[0]);
    	return 1;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    u2High = StrToInt(szArgv[2]);
    u2Low = StrToInt(szArgv[3]);
    fgIsMain = (u1Path == VDP_1);

    u2FactorIdx = 0;
    u2LumaAvgPre = 0;
    u2LumaAvgCur = 0;
    u2Cnt = u2High;

    do
    {
        u2Width = GET_PS_WIDTH(((UINT32)wDrvVideoInputWidth(u1Path) * u2Cnt), 0x8000);        
        
        if (u1DrvDIOutOfBound())
        {
            Printf("u2FactorIdx is larger than SCALE_SIZE\nStop recording\n");
            u1CrcOnOff[u1Path] = 0;
            break;
        }
        
        if (u2WidthPre != u2Width)
        {       
            if ((u2Width%2) == 0) // only support even
            {
                u2WidthPre = u2Width;
                #if 0
                vDrvSetRelatedParam(u1Path, u2Cnt, u2Width); // for FPGA verify use
                #else
                CLI_Parser_Arg("vdp.s.hps %d %d", u1Path, u2Cnt); // for IC verify use
                while (fgIsScposFlgSet((fgIsMain) ? MAIN_DISP_PRM_CHG : PIP_DISP_PRM_CHG)); // for IC verify use
                #endif
                x_thread_delay(1000);
                vDrvDISetCrcRange(u1Path, u2Cnt, u2Width);
            }
        }
        while (u1CrcOnOff[u1Path]); // wait until crc polling
        u2Cnt--;
    } while (u2Cnt >= u2Low);
    return 0;
}

static INT32 _i4DiDumpAllCrc(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Mode;

    if (i4Argc != 2)
    {
        Printf("Usage: %s <0: SRCR only/ 1: SRCW only/ 2: OUT only/ 3: PSCAN All/ 4: PreProc & PSCAN ALL>\n", szArgv[0]);
        return 1;
    }
    
    u1Mode = StrToInt(szArgv[1]);
    vDrvDIDumpAllCrc(u1Mode);
    return 0;
}



static INT32 _i4DiSetForceMotion(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Path;
    UINT8 u1YOnOff;
    UINT8 u1YLevel;
    UINT8 u1COnOff;
    UINT8 u1CLevel;

    if (i4Argc != 6)
    {
        Printf("Usage: %s <m/p> Y<on/off> C<on/off> Y_Level<0/8> C_Level<0/8>\n", szArgv[0]);
        return 1;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    u1YOnOff = ((x_strcmp(szArgv[2], "on") == 0) ? SV_ON : SV_OFF);
    u1COnOff = ((x_strcmp(szArgv[3], "on") == 0) ? SV_ON : SV_OFF);
    u1YLevel = StrToInt(szArgv[4]);
    u1CLevel = StrToInt(szArgv[5]);
    vDrvDISetMotionLevel(u1Path, u1YOnOff, u1COnOff, u1YLevel, u1CLevel);
    return 0;
}

static INT32 _i4DiSetDetectCRC(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Value;

    if (i4Argc != 2)
    {
        Printf("Usage: %s crc_value\n", szArgv[0]);
        return 1;
    }

    u4Value = StrToInt(szArgv[1]);
    vDrvDISetDetectCRC(u4Value);
    return 0;
}

static INT32 _i4DiGetVdoinCRC(INT32 i4Argc, const CHAR ** szArgv)
{
    Printf("Vdoin CRC of current frame: 0x%x\n", u4DrvDIGetVdoinCRC());
    return 0;
}
#endif

#ifdef CC_SUPPORT_H264
static INT32 _i4DiSetH264WriteMode(INT32 i4Argc, const CHAR ** szArgv)
{            
    UINT8 u1Mode;
    
    if (i4Argc != 2)
    {
        Printf("Usage: %s Mode [0: normal, 1: for H264, 2: for OSD alpha blending\n", szArgv[0]);
        return 1;
    }

    u1Mode = StrToInt(szArgv[1]);
    vDrvDISetH264WriteMode(u1Mode);
    return 0;
}
#endif

static INT32 _i4DiInitCrc(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Path;
    
    if (i4Argc != 2)
    {
        Printf("Usage: %s m/p\n", szArgv[0]);
        return 1;
    }
    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    vDrvDIInitCRC(u1Path);
    return 0;
}



/****victor test***/
static INT32 _i4DiChkSta(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Path; 
    UINT32 u1Result;
    
    if (i4Argc != 2)
    {
        Printf("Usage: %s m/p\n", szArgv[0]);
        return 1;
    }
    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    u1Result = u4DrvDIChkAllStatus(u1Path);
    Printf("Result: %d \n", u1Result);
    return 0;
}


static INT32 _i4DiClearStatus(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Path;
    
    if (i4Argc != 2)
    {
        Printf("Usage: %s m/p\n", szArgv[0]);
        return 1;
    }
    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    vDrvDIRstStatusCnt(u1Path);
    return 0;
}
#endif /*#ifdef CC_CLI*/

#ifdef DYNAMIC_RES_VERIFY
static INT32 _i4DiForcePscanOverscan(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1OnOff;

    if (i4Argc != 2)
    {
        Printf("Usage: %s 0/1 \n", szArgv[0]);
        return 1;
    }

    u1OnOff = StrToInt(szArgv[1]);
    vForcePscanOverscan(u1OnOff);
    vDiForceClipEn(u1OnOff);
    return 0;
}
static INT32 _i4DiDynRes(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1OnOff;
  
    if (i4Argc != 2)
    {
        Printf("Usage: %s 0/1 \n", szArgv[0]);
        return 1;
    }

    u1OnOff = StrToInt(szArgv[1]);
    vSetSCDynRes(u1OnOff);
    vSetDiDynRes(u1OnOff);
    return 0;
}
#endif

