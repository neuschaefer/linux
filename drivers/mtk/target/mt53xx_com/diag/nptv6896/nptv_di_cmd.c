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
#include "hw_vdoin.h"
#include "source_table.h"
#include "sv_const.h"
#include "drv_di.h"
#include "drv_di_int.h"

#ifndef CC_CLI
#define CC_CLI
#endif

#ifdef CC_CLI

/* extern variale */
EXTERN UINT8 bFPGADiMainLoop;
EXTERN UINT8 fgDIStatusPolling;

/* extern function */
EXTERN void vDrvDIOnOff(UINT8 u1VdpId, UINT8 bOnOff);
EXTERN void vDrvDISetHParam(UINT8 u1VdpId, UINT16 u2HTotal, UINT16 u2InputWidth, UINT16 u2Clip);
EXTERN void vDrvDISetVParam(UINT8 u1VdpId, UINT16 u2VTotal, UINT16 u2Height, UINT16 u2Clip);
EXTERN void vDrvDISetWritePrecision(UINT8 u1VdpId, UINT8 b8bit);
EXTERN void vDrvDIProgModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff);
EXTERN void vDrvDIForceRead8bit(UINT8 u1VdpId, UINT8 b8bit);
EXTERN void vDrvDIFilmStatusPolling(void *pvArgv);
EXTERN void vDrvDIMotionStatusPolling(void *pvArgv);
EXTERN void vDrvDICMotionStatusPolling(void *pvArgv);
EXTERN void vDrvDIEventStatusPolling(void *pvArgv);
EXTERN void vDrvDIEventProfileStatusPolling(void *pvArgv);
EXTERN void vDrvDISetEvent(UINT8 bNum,UINT8 bMode,UINT32 u4Value);
EXTERN void vDrvDISetDebugDataMode(UINT8 bPath, E_DI_DMODE bMode);
EXTERN UINT8 u1DrvDIGetDebugDataMode(UINT8 u1VdpId);
EXTERN void vDrvDISetDebugPSCANDisplay(UINT8 u1VdpId, UINT8 u1OnOff);
EXTERN UINT8 u1DrvDIGetDebugPSCANDisplay(UINT8 u1VdpId);
EXTERN UINT8 vDrvDIGetInfo(UINT8 u1VdpId, UINT8 u1DbgIdx);
EXTERN UINT8 vDrvDISetOSD(UINT8 u1Item, UINT8 u1OnOff);
EXTERN UINT8 vDrvDIGetBufAddr(UINT8 u1VdpId, UINT32 *u4AddrStart, UINT32 *u4AddrEnd);
EXTERN void vDrvDiFPGAMloop(void);
EXTERN UINT16 wDrvVideoGetPorch(UINT8 bPath, UINT8 bPorchType);
EXTERN UINT16 wDrvVideoSetPorch(UINT8 bPath, UINT8 bPorchType, UINT16 wValue);
EXTERN void MDDI_PRINT_REG_LOG(void);
EXTERN void vDrvDISetPattern(UINT8 u1VdpId, UINT8 u1OnOff);
EXTERN void vDrvDISetInputBorder(UINT8 u1VdpId, UINT8 u1OnOff);
EXTERN void vDrvDISetFlipMirror(UINT8 u1VdpId, UINT8 u1Mode);
EXTERN void vDrvDISetDeAdap(UINT8 u1Mode, UINT8 u1Value);

/* static function */
static INT32 _i4DiSetHParam(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiSetVParam(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DSetBitRes(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiFWOnOff(INT32 i4Argc, const CHAR **szArgv);
INT32 _i4DiOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiInit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiDemo(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiTrickModeOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiFrmEncModeOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiProgModeOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiSetColorMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DiSetEvent(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiHPSTest(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetDebugDataMode(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetDebugDisplayMode(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiGetInfo(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiStatus(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetInk(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetOSD(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetMid(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetFreeze(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetFLDInv(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiForceRead8bit(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _i4DiSetWritePrecision(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _iDiSetPorchTest(INT32 i4Argc, const CHAR **szArgv);
static INT32 _iDiSetCRCTest(INT32 i4Argc, const CHAR **szArgv);
static INT32 _iDiLogReg(INT32 i4Argc, const CHAR **szArgv);
static INT32 _iDiLogAll(INT32 i4Argc, const CHAR **szArgv);
INT32 _iDiSetOutputPattern(INT32 i4Argc, const CHAR **szArgv);
INT32 _iDiSetInputBorder(INT32 i4Argc, const CHAR **szArgv);
static INT32 _iDiSetFlipMirror(INT32 i4Argc, const CHAR **szArgv);
static INT32 _iDiSetDeAdap(INT32 i4Argc, const CHAR **szArgv);

CLIMOD_DEBUG_FUNCTIONS(DI)

#endif /*#ifdef CC_CLI*/


/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for DI command entry
CLI_EXEC_T arYcprocDiTbl[] = {
#ifdef CC_CLI
    {"SetHParam", "shp", _i4DiSetHParam, NULL, "Set horizontal direction related parameters", CLI_SUPERVISOR},
    {"SetVParam", "svp", _i4DiSetVParam, NULL, "Set vertical direction related parameters", CLI_SUPERVISOR},
    {"SetBitRes", "sbr", _i4DSetBitRes, NULL, "Set bit resolution", CLI_SUPERVISOR},
    {"FWOnOff", "fwoo", _i4DiFWOnOff, NULL, "Enable/disable MDDi FW", CLI_SUPERVISOR},
    {"OnOff", "oo", _i4DiOnOff, NULL, "Enable/disable MDDi/DRAM controller", CLI_SUPERVISOR},
    {"Init", "i", _i4DiInit, NULL, "Initialize MDDi register values", CLI_SUPERVISOR},
    {"DemoParam", "dp", _i4DiDemo, NULL, "Set demo mode active region", CLI_SUPERVISOR},
    {"TrickModeOnOff", "tmoo", _i4DiTrickModeOnOff, NULL, "Turn on/off trick mode", CLI_SUPERVISOR},
    {"FrmEncModeOnOff", "femoo", _i4DiFrmEncModeOnOff, NULL, "Turn on/off frame encode mode", CLI_SUPERVISOR},
    {"ProgModeOnOff", "pmoo", _i4DiProgModeOnOff, NULL, "Turn on/off progressive mode (DTV only)", CLI_SUPERVISOR},
    {"SetColorMode", "scm", _i4DiSetColorMode, NULL, "Switch between 420/422 color mode", CLI_SUPERVISOR},
    {"Polling", "p", _i4DiStatus, NULL, "DI QTY Status Polling", CLI_SUPERVISOR},    
    {"SetInk", "ink", _i4DiSetInk, NULL, "Set Ink", CLI_SUPERVISOR},
    {"SetEvent", "se", _i4DiSetEvent, NULL, "DI Set Event", CLI_SUPERVISOR},
    {"HPSTest", "hpst", _i4DiHPSTest, NULL, "HPS test", CLI_SUPERVISOR},
    {"SetDebugDataMode", "sddm", _i4DiSetDebugDataMode, NULL, "Set debug data mode", CLI_SUPERVISOR},
    {"SetDebugDisplayMode", "sdpdm", _i4DiSetDebugDisplayMode, NULL, "Set debug PSCAN display mode", CLI_SUPERVISOR},
    {"GetInfo", "g", _i4DiGetInfo, NULL, "Get PSCAN debug Information", CLI_SUPERVISOR},
    {"SetInk", "osd", _i4DiSetOSD, NULL, "Set PSCAN ink control", CLI_SUPERVISOR},
    {"SetMid", "smid", _i4DiSetMid, NULL, "Set PSCAN Dram Agent", CLI_SUPERVISOR},
    {"SetFreeze", "f", _i4DiSetFreeze, NULL, "Set PSCAN Freeze", CLI_SUPERVISOR},
    {"SetFLDInv", "sfld", _i4DiSetFLDInv, NULL, "Set Field Inverse", CLI_SUPERVISOR},
    {"ForceRead8bit", "srbit", _i4DiForceRead8bit, NULL, "Force Read Precision Selection", CLI_SUPERVISOR},
    {"SetWritePrecision", "swbit", _i4DiSetWritePrecision, NULL, "Write Precision Selection", CLI_SUPERVISOR},
    {"PorchTest", "spt", _iDiSetPorchTest, NULL, "Set Porch test", CLI_SUPERVISOR},
    {"LogReg", "la", _iDiLogReg, NULL, "Log Register access", CLI_SUPERVISOR},
    {"CRCTest", "scrc", _iDiSetCRCTest, NULL, "CRC Test", CLI_SUPERVISOR},
    {"LogAll", "logall", _iDiLogAll, NULL, "Log all information", CLI_SUPERVISOR},
    {"SetOutputPattern", "ptgn", _iDiSetOutputPattern, NULL, "Set output pattern", CLI_SUPERVISOR},
    {"SetInputBorder", "ibor", _iDiSetInputBorder, NULL, "Set input border", CLI_SUPERVISOR},
    {"SetFlipMirror", "fm", _iDiSetFlipMirror, NULL, "Set flip mirror mode", CLI_SUPERVISOR},
    {"SetDeRange", "sde", _iDiSetDeAdap, NULL, "Set De adaptive", CLI_SUPERVISOR},

CLIMOD_DEBUG_CLIENTRY(DI),
#endif /*#ifdef CC_CLI*/

  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/

static INT32 _i4DiSetHParam(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;

    if (i4Argc < 5)
    {
    	Printf("Usage: %s <m/p> htotal width clip\n", szArgv[0]);
    	return 1;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    vDrvDISetHParam(u1Path, ((UINT16)StrToInt(szArgv[2])), ((UINT16)StrToInt(szArgv[3])), ((UINT16)StrToInt(szArgv[4])));

    return 0;
}

static INT32 _i4DiSetVParam(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;

    if (i4Argc < 5)
    {
    	Printf("Usage: %s <m/p> vtotal hieght clip\n", szArgv[0]);
    	return 1;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    vDrvDISetVParam(u1Path, ((UINT16)StrToInt(szArgv[2])), ((UINT16)StrToInt(szArgv[3])), ((UINT16)StrToInt(szArgv[4])));

    return 0;
}

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
    	Printf("Usage: %s <0:MainCtrl / 1:SubCtrl / 2:AdapMA> <0:off / 1:on>\n", szArgv[0]);
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
/*
    UINT8 u1Path;
    UINT8 u1OnOff;
    UINT8 u1Region;

    if (i4Argc < 5)
    {
    	Printf("Usage: %s <m/p> <on/off> <l/r> center\n", szArgv[0]);
    	return 1;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    u1OnOff = ((x_strcmp(szArgv[2], "on") == 0) ? SV_ON : SV_OFF);
    u1Region = ((x_strcmp(szArgv[3], "l") == 0) ? SV_LEFT : SV_RIGHT);
*/
    //vDrvDIDemo(u1Path, u1OnOff, u1Region, ((UINT32)StrToInt(szArgv[4])));

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

static INT32 _i4DiFrmEncModeOnOff(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;
    UINT8 u1OnOff;
    UINT8 u1BtmFirst;

    if (i4Argc < 4)
    {
    	Printf("Usage: %s <m/p> <on/off> <btm/top>\n", szArgv[0]);
    	return 1;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    u1OnOff = ((x_strcmp(szArgv[2], "on") == 0) ? SV_ON : SV_OFF);
    u1BtmFirst = ((x_strcmp(szArgv[3], "btm") == 0) ? SV_TRUE : SV_FALSE);

    vDrvDIFrmEncModeOnOff(u1Path, u1OnOff, u1BtmFirst);

    return 0;
}

static INT32 _i4DiProgModeOnOff(INT32 i4Argc, const CHAR **szArgv)
{
#ifndef CC_MT8223

    UINT8 u1Path;
    UINT8 u1OnOff;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> <on/off>\n", szArgv[0]);
    	return 1;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    u1OnOff = ((x_strcmp(szArgv[2], "on") == 0) ? SV_ON : SV_OFF);
    vDrvDIProgModeOnOff(u1Path, u1OnOff);
#endif
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
    INT32 i,u4NumOfFunc=0;
    
    static HANDLE_T _hThread;
    extern DI_PQ_THREAD DIPQThread[];

    while(DIPQThread[u4NumOfFunc].pThreadFunc !=NULL)
    {
        ++u4NumOfFunc;
    }

    if (i4Argc != 3)
    {
        for(i=0;i<u4NumOfFunc;++i)
        {
            Printf("\n %d)%s",(int)i,DIPQThread[i].szThreadDescription);
        }
        
        bPara1=0;
        fgDIStatusPolling=0;

        return 1;
        
    }else{

        bPara1=StrToInt(szArgv[1]);
        fgDIStatusPolling=x_strcmp(szArgv[2], "on") == 0;

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
            DIPQThread[bPara1].pThreadFunc, 0, NULL);

        if (i4Ret != OSR_OK)
        {
            Printf("Status Polling Thread cannot be created\n");

            return 1;
        }

        Printf("Status Polling Thread created\n");
    }
    return 0;
}


static INT32 _i4DiSetInk(INT32 i4Argc, const CHAR ** szArgv)
{

    EXTERN void vDrvDISetInk(UINT8 bMode,UINT16 u2Type);

    if (i4Argc == 1)
    {
        vDrvDISetInk(0,0);
    }
    else if(i4Argc == 2)
    {
        vDrvDISetInk(StrToInt(szArgv[1]),0);        
    }
    else if(i4Argc == 3)
    {
        vDrvDISetInk(StrToInt(szArgv[1]),StrToInt(szArgv[2]));
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

extern UINT16 wDrvVideoInputWidth(UINT8 bPath);
#define GET_PS_WIDTH(dividend, divisor)        (((dividend) + ((divisor) - 1)) / (divisor))

static INT32 _i4DiHPSTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Path;
    UINT16 u2Delay;
    UINT16 u2High;
    UINT16 u2Low;
    UINT16 u2Cnt;
    UINT16 u2Width, u2WidthPre=0;
    UINT16 u2Step = 1;
    
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
            	//CLI_Parser_Arg("vdp.s.dispmode 0 0");
            	x_thread_delay(u2Delay*1000);
            }
        }
    }

    return 0;
}

static INT32 _i4DiSetDebugDataMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Path;
    UINT8 u1DataMode;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> data_mode\n", szArgv[0]);
    	Printf("0: E_DI_FULL_1W3R,  1: E_DI_FULL_1W4R, \n");
    	Printf("2: E_DI_CBOB_1W3R,  3: E_DI_CBOB_1W4R, \n");
    	Printf("4: E_DI_YCBOB_0W0R, 5: E_DI_YCBOB_1W1R \n");
    	Printf("6: E_DI_FLIP_MIRROR_1W4R\n");
    	Printf("7: E_DI_FLIP_MIRROR_CBOB_1W4R\n");
    	Printf("8: E_DI_FLIP_MIRROR_YCBOB_1W1R\n\n");
    	Printf("Current debug data mode, main(%d), sub(%d)\n",
    		u1DrvDIGetDebugDataMode(SV_VP_MAIN), u1DrvDIGetDebugDataMode(SV_VP_PIP));

    	return 1;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    u1DataMode = (StrToInt(szArgv[2]) < E_DI_UNKNOWN) ? StrToInt(szArgv[2]) : E_DI_UNKNOWN;
    vDrvDISetDebugDataMode(u1Path, ((E_DI_DMODE)u1DataMode));

    return 0;
}

static INT32 _i4DiSetDebugDisplayMode(INT32 i4Argc, const CHAR ** szArgv)
{
    INT8 i1Tmp;

    UINT8 u1Path;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> <0/1/255>\n", szArgv[0]);
    	Printf("0: OFF, 1: ON, 255: system default\n\n");
    	Printf("Current debug PSCAN display mode, main(%d), sub(%d)\n",
    		u1DrvDIGetDebugPSCANDisplay(SV_VP_MAIN), u1DrvDIGetDebugPSCANDisplay(SV_VP_PIP));

    	return 1;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    i1Tmp = StrToInt(szArgv[2]);

    if (i1Tmp == -1)
    {
    	vDrvDISetDebugPSCANDisplay(u1Path, 0xff);
    }
    else
    {
    	vDrvDISetDebugPSCANDisplay(u1Path, i1Tmp);
    }

    return 0;
}

static INT32 _i4DiGetInfo(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> <0:General>\n", szArgv[0]);
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
#ifndef CC_MT8223

    INT32 fgOk;
    UINT32 u4LowAddr, u4HighAddr;
    MID_AGENT_ID_T eAgentId;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> <0:off / 1:on>\n", szArgv[0]);
    	return 1;
    }

    eAgentId = MID_StringToAgentId("pscan_nr");

    if (StrToInt(szArgv[2]))
    {
        vDrvDIGetBufAddr(((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP), 
            &u4LowAddr, &u4HighAddr);
        
        fgOk = MID_SetRegionEx(1, eAgentId, u4LowAddr, u4HighAddr, 0);
        
        Printf("%s %s Set Mid Region 1 (0x%08x - 0x%08x)  %s\n",
            MID_AgentIdToString(eAgentId),
    	    ((x_strcmp(szArgv[1], "m") == 0) ? "Main" : "Sub"),
            u4LowAddr, u4HighAddr,  fgOk ? "Succeeded" : "Failed");

        fgOk = MID_SetRegionEx(2, eAgentId, u4LowAddr, u4HighAddr, 1);

        Printf("%s %s Set Mid Region 2 (0x%08x - 0x%08x)  %s\n",
            MID_AgentIdToString(eAgentId),
    	    ((x_strcmp(szArgv[1], "m") == 0) ? "Main" : "Sub"),
            u4LowAddr, u4HighAddr,  fgOk ? "Succeeded" : "Failed");

        MID_EnableRegionProtect(1);
        MID_EnableRegionProtect(2);
    }
    else
    {
        MID_DisableRegionProtect(1);         
        MID_DisableRegionProtect(2);
    }
#endif
    return 0;
}

static INT32 _i4DiSetFreeze(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> <top field/bot field>\n", szArgv[0]);
    	return 1;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    vDrvDIFreezeOnOff(u1Path, ((UINT16)StrToInt(szArgv[2])));

    return 0;
}

static INT32 _i4DiSetFLDInv(INT32 i4Argc, const CHAR **szArgv)
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
#ifndef CC_MT8223

    UINT8 u1Path;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> <8bit :1/10bit :0>\n", szArgv[0]);
    	return 1;
    }
    
    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    vDrvDIForceRead8bit(u1Path, ((UINT16)StrToInt(szArgv[2])));
#endif    
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


static INT32 _iDiSetPorchTest(INT32 i4Argc, const CHAR **szArgv)
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

static INT32 _iDiLogReg(INT32 i4Argc, const CHAR **szArgv)
{
    MDDI_PRINT_REG_LOG();
    return 0;
}

static INT32 _iDiSetCRCTest(INT32 i4Argc, const CHAR **szArgv)
{
#ifdef __MODEL_slt__
    vDrvDICRCPreProcess();
    Printf("Bypass module for CRC test!!\n");
#else
    Printf("PSCAN_CRC_FLOW is not defined!!\n");
#endif
    return 0;
}

static INT32 _iDiLogAll(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc < 2)
    {
    	Printf("Usage: %s <m/p>\n", szArgv[0]);
    	return 1;
    }

    Printf("5388 PSCAN Memory dump\n");
    if (x_strcmp(szArgv[1], "m") == 0)
        CLI_Parser_Arg("r 0x20024000 0x800");
    else
        CLI_Parser_Arg("r 0x20024800 0x800");
    
    Printf("\n\n");
    CLI_Parser_Arg("n.di.g %d %d", ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP), 0);
    Printf("\n\n");
    CLI_Parser_Arg("fbm.q");
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

    UINT8 u1Path;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <main:0/ sub:1> <On :1/Off :0>\n", szArgv[0]);
    	return 1;
    }
    
    u1Path = StrToInt(szArgv[1]);
    vDrvDISetInputBorder(u1Path, ((UINT16)StrToInt(szArgv[2])));

    return 0;
}

static INT32 _iDiSetFlipMirror(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;

    if (i4Argc < 3)
    {
    	Printf("Usage: %s <m/p> <Mode: (Normal)0/(Mirror)1/(Flip)2/(Mirror&Flip)3>\n", szArgv[0]);
    	return 1;
    }
    
    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    vDrvDISetFlipMirror(u1Path, ((UINT16)StrToInt(szArgv[2])));
    
    return 0;
}

static INT32 _iDiSetDeAdap(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc < 3)
    {
    	Printf("Usage: %s <0:Range, 1:IIR> <Range>\n", szArgv[0]);
    	return 1;
    }
    
    vDrvDISetDeAdap(((UINT16)StrToInt(szArgv[1])), ((UINT16)StrToInt(szArgv[2])));
    
    return 0;
}
#endif /*#ifdef CC_CLI*/
