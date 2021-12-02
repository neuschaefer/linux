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
 * $RCSfile: nptv_scpos_cmd.c,v $
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

#include "source_table.h"
#include "sv_const.h"
#include "drv_scpos.h"
#include "drv_upscaler.h"
#include "vdp_display.h"
#include "drv_scaler.h"
#include "video_def.h"
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396) || defined(CC_MT5368)
#include "drv_scaler_drvif.h"
#include "frametrack_drvif.h"
#include "hw_scpos.h"
#endif

#ifndef CC_CLI
#define CC_CLI
#endif

INT32 _ScposSetDSPtnCmd(INT32 i4Argc, const CHAR **szArgv);
INT32 _ScposSetUSPtnCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScposSetFrameTrackCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScposGetFrameTrackCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScposSetSWFrameTrackCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScposSetHPorchCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScposSetVPorchCmd(INT32 i4Argc, const CHAR **szArgv);


#ifdef CC_CLI
static INT32 _ScposSetDispSizePosCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScposSetSrcSizePosCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvImportProtectionOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScalerTestCmd(INT32 i4Argc, const CHAR **szArgv);
#if defined(CC_MT5365) || defined(CC_MT5395)
static INT32 _ScpipSetDramBurstCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipNegPhaseOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipCheckFifoOverCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipQueryCmd(INT32 i4Argc, const CHAR **szArgv);
#ifdef CC_SCPOS_3DTV_SUPPORT
static INT32 _ScpipTv3dShift(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipTv3dQuery(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipTv3dSetFlg(INT32 i4Argc, const CHAR **szArgv);
#endif
#if 0   
static INT32 _ScpipTv3dStatusCmd(INT32 i4Argc, const CHAR **szArgv);
#endif
#endif
static INT32 _ScpipSetMirrorCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipSetFlipCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipSetMirrorFlipAllCmd(INT32 i4Argc, const CHAR **szArgv);

#endif /*#ifdef CC_CLI*/
/******************************************************************************
* CLI Command Table
******************************************************************************/

CLIMOD_DEBUG_FUNCTIONS(SCPOS)

// Table for scpos command entry
CLI_EXEC_T arScposCmdTbl[] = {
  {"dspattern", "dspt", _ScposSetDSPtnCmd, NULL, "Down Scalar Pattern", CLI_SUPERVISOR},
  {"uspattern", "uspt", _ScposSetUSPtnCmd, NULL, "Up Scalar Pattern", CLI_SUPERVISOR},
  {"frametrackoff",  "ft",  _ScposSetFrameTrackCmd,  NULL,  "Frame track froce on/off",  CLI_SUPERVISOR},  
  {"frametrackQuery",  "ftq",  _ScposGetFrameTrackCmd,  NULL,  "Query Frame track status",  CLI_SUPERVISOR},  
  {"setswframetrack",  "swft",  _ScposSetSWFrameTrackCmd,  NULL,  "Set SW Frame track parameters",  CLI_SUPERVISOR},  
  {"sethporch",  "hpos",  _ScposSetHPorchCmd,  NULL,  "Set H Porch",  CLI_SUPERVISOR},  
  {"setvporch",  "vpos",  _ScposSetVPorchCmd,  NULL,  "Set V Porch",  CLI_SUPERVISOR},  
  CLIMOD_DEBUG_CLIENTRY(SCPOS),
#ifdef CC_CLI
  {"Disp", "d", _ScposSetDispSizePosCmd, NULL, "Set Display Position/Size", CLI_SUPERVISOR},
  {"Src", "s", _ScposSetSrcSizePosCmd, NULL, "Set Source Position/Size", CLI_SUPERVISOR},
  {"ImportProtectionOnOff",  "ipoo",  _NptvImportProtectionOnOff,  NULL,  "Nptv Import Protection on/off",  CLI_SUPERVISOR},  
  {"ScalerTest",    "st",  _ScalerTestCmd,             NULL,    "Scaler Table Command",  CLI_SUPERVISOR},
#if defined(CC_MT5365) || defined(CC_MT5395)
  {"dramburst", "db", _ScpipSetDramBurstCmd, NULL, "Set Scaler DRAM burst read mode on/off", CLI_SUPERVISOR},
  {"negphaseonoff", "npoo", _ScpipNegPhaseOnOff, NULL, "Set Scaler negative initial phase on/off", CLI_SUPERVISOR},    
  {"fifoover", "fover", _ScpipCheckFifoOverCmd, NULL, "Check scaler dram fifo overflow status", CLI_SUPERVISOR},
  {"query", "q", _ScpipQueryCmd, NULL, "Query scaler status", CLI_SUPERVISOR},    
#ifdef CC_SCPOS_3DTV_SUPPORT
  {"3dshift", "3dshft", _ScpipTv3dShift, NULL, "3d L/R print", CLI_SUPERVISOR},    
  {"3dquery", "3dq", _ScpipTv3dQuery, NULL, "3d setting print", CLI_SUPERVISOR},    
  {"3dflg", "3dflg", _ScpipTv3dSetFlg, NULL, "3d flg set", CLI_SUPERVISOR},    
    
#endif    
#if 0  
  {"3dstatus", "3ds", _ScpipTv3dStatusCmd, NULL, "3d status print", CLI_SUPERVISOR},
#endif    
#endif
  {"Mirror", "mir", _ScpipSetMirrorCmd, NULL, "Set Scaler Mirror in H dir on/off", CLI_SUPERVISOR},
  {"Flip", "flip", _ScpipSetFlipCmd, NULL, "Set Scaler Flip in V dir on/off", CLI_SUPERVISOR},
  {"MirrorFlip", "mf", _ScpipSetMirrorFlipAllCmd, NULL, "Set Scaler Mirror and Flip in both H/V for main and sub", CLI_SUPERVISOR},
#endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

/******************************************************************************
* Local Function
******************************************************************************/

static INT32 _ScpipSetMirrorCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bPath, bOnOff;
    
    if (i4Argc < 2)
    {
        Printf("Usage: <main:0/sub:1> <on:1/off:0>\n");
        return 0;
    }

    bPath = (UINT8)StrToInt(szArgv[1]);
    bOnOff= (UINT8)StrToInt(szArgv[2]);

    vScpipSetMirror(bPath,bOnOff);
    if(bPath==VDP_1)
    {
        vSetScposFlg(MAIN_DISP_PRM_CHG);
    }
    else
    {
        vSetScposFlg(PIP_DISP_PRM_CHG);
    }

    printf("[SetMirror]  path [%d], onoff[%d]\n", (INT32)bPath,(INT32)bOnOff);
    printf("Please change timing to trigger mode change\n");
    
    return 0;
}


static INT32 _ScpipSetFlipCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bPath, bOnOff;
    
    if (i4Argc < 2)
    {
        Printf("Usage: <main:0/sub:1> <on:1/off:0>\n");
        return 0;
    }

    bPath = (UINT8)StrToInt(szArgv[1]);
    bOnOff= (UINT8)StrToInt(szArgv[2]);

    vScpipSetFlip(bPath,bOnOff);
    if(bPath==VDP_1)
    {
        vSetScposFlg(MAIN_DISP_PRM_CHG);
    }
    else
    {
        vSetScposFlg(PIP_DISP_PRM_CHG);
    }

    printf("[SetFlip]  path [%d], onoff[%d]\n",(INT32)bPath,(INT32)bOnOff);
    printf("Please change timing to trigger mode change\n");
    
    return 0;
}


static INT32 _ScpipSetMirrorFlipAllCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8  bOnOff;
    
    if (i4Argc < 2)
    {
        Printf("Usage: <on:1/off:0>\n");
        return 0;
    }

    bOnOff= (UINT8)StrToInt(szArgv[1]);

    vScpipSetMirrorFlipAll(bOnOff);
    vSetScposFlg(MAIN_DISP_PRM_CHG);
    vSetScposFlg(PIP_DISP_PRM_CHG);

    printf("[SetMirrorFlipAll] onoff[%d]\n",(INT32)bOnOff);
    printf("Please change timing to trigger mode change\n");
 
    return 0;
}

#ifdef SUPPORT_SW_FRAME_CHECK
extern UINT32 _u4VDistUpperBound;
extern UINT32 _u4VDistLowerBound;  
extern UINT32 _u4VDistTrackMaxStep;          
#endif
static INT32 _ScposSetSWFrameTrackCmd(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc < 4)
    {
        Printf("Usage: <Step> <UpperBound> <LowerBound>\n");
        #ifdef SUPPORT_SW_FRAME_CHECK
        Printf("Current: <%d> <0x%X> <0x%X>\n", (INT32)_u4VDistTrackMaxStep, (INT32)_u4VDistUpperBound, (INT32)_u4VDistLowerBound);
        #endif
        return 0;
    }

    #ifdef SUPPORT_SW_FRAME_CHECK
    _u4VDistTrackMaxStep = StrToInt(szArgv[1]);
    _u4VDistUpperBound = StrToInt(szArgv[2]);
    _u4VDistLowerBound = StrToInt(szArgv[3]);
    #endif

    return 0;
}

static INT32 _ScposGetFrameTrackCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4OnOff= u4GetFrameTrackOnOff();

    #if 0
    switch(u4ForceFrameTrackCtrl)
    {
        case SCPIP_FRAME_TRACK_OFF_CLI:
            printf("[Frame Track] Force OFF [%d]\n",u4OnOff);
            break;
        case SCPIP_FRAME_TRACK_ON_CLI:
            printf("[Frame Track]  Force ON [%d]\n",u4OnOff);
            break;
        case SCPIP_FRAME_TRACK_OFF_DS_PATTERN:
            printf("[Frame Track]  Force OFF because of Down Scaler Pattern enable  [%d]\n",u4OnOff);
            break;
        case SCPIP_FRAME_TRACK_AUTO:
            printf("[Frame Track]  Noamal mode [%d]\n",u4OnOff);
            break;
    }
    #else
    printf("[Frame Track]  u4ForceFrameTrackCtrl= 0x%x, Mode(%d), OnOff[%d]\n",(INT32)u4ForceFrameTrackCtrl, (INT32)(u4GetFrameTrackMode()), (INT32)u4OnOff);
    printf("SCPIP_FRAME_TRACK_AUTO 0\n");
    printf("SCPIP_FRAME_TRACK_ON_CLI ((UINT8)1 << 1)\n");
    printf("SCPIP_FRAME_TRACK_OFF_CLI ((UINT8)1 << 2)\n");
    printf("SCPIP_FRAME_TRACK_OFF_DS_PATTERN ((UINT8)1 << 3)\n");
    printf("SCPIP_FRAME_TRACK_OFF_V_PORCH ((UINT8)1 << 4)\n");
    #endif
    return 0;
}

static INT32 _ScposSetFrameTrackCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4FTMode;
    UINT32 u4FTCtrl;
    
    if (i4Argc < 3)
    {
        Printf("Usage: <ForceCtrl> <Mode> \n<ForceCtrl>  0:Force Off, 1:Force On, 2:Auto(normal) operation\n<Mode> 0: By default, 1: HW FT, 2: SW FT\n");
        return 0;
    }

    u4FTCtrl = StrToInt(szArgv[1]);
    u4FTMode = StrToInt(szArgv[2]);

    if(u4FTCtrl ==0) // force off 
    {
        u4ForceFrameTrackCtrl|=SCPIP_FRAME_TRACK_OFF_CLI;
        u4ForceFrameTrackCtrl&=(~SCPIP_FRAME_TRACK_ON_CLI);
    }
    else  if(u4FTCtrl ==1) // force on
    {
        u4ForceFrameTrackCtrl|=SCPIP_FRAME_TRACK_ON_CLI;
        u4ForceFrameTrackCtrl&=(~SCPIP_FRAME_TRACK_OFF_CLI);
    }
    else // auto
        u4ForceFrameTrackCtrl =SCPIP_FRAME_TRACK_AUTO;

    u4ForceFrameTrackMode = (u4FTMode > 2)? 0: u4FTMode;

    printf("[Frame Track]  Set mode [%d, %d]\n", (INT32)u4FTCtrl, (INT32)u4FTMode);

    return 0;
}


static INT32 _ScposSetHPorchCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Path = 0;
    UINT16 u2Current = 0xffff;
    UINT16 u2Value;

    if (i4Argc > 1)
    {
        u4Path = StrToInt(szArgv[1]);
        u2Current = _VDP_ScposGetPorch((UINT8)u4Path, SV_HPORCH_MIN);        
    }
        
    if (i4Argc < 3)
    {
        Printf("Usage: <u4VdpId> <value>\n");
        if (u2Current != 0xffff)
        {
            Printf("VDP(%d) Current H Porch = %d\n", (INT32)u4Path, (INT32)(1000 - u2Current));
        }
        return 0;
    }

    u2Value = (UINT16)StrToInt(szArgv[2]);
    u2Value += u2Current;
    _VDP_ScposSetPorch((UINT8)u4Path, SV_HPORCH_CURRENT, u2Value); 

    return 0;
}

static INT32 _ScposSetVPorchCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Path = 0;
    UINT16 u2Current = 0xffff;
    UINT16 u2Value;

    if (i4Argc > 1)
    {
        u4Path = StrToInt(szArgv[1]);
        u2Current = _VDP_ScposGetPorch((UINT8)u4Path, SV_VPORCH_MIN);        
    }
        
    if (i4Argc < 3)
    {
        Printf("Usage: <u4VdpId> <value>\n");
        if (u2Current != 0xffff)
        {
            Printf("VDP(%d) Current V Porch = %d\n", (INT32)u4Path, (INT32)(1000 - u2Current));
        }
        return 0;
    }

    u2Value = (UINT16)StrToInt(szArgv[2]);
    u2Value += u2Current;
    _VDP_ScposSetPorch((UINT8)u4Path, SV_VPORCH_CURRENT, u2Value); 

    return 0;
}

INT32 _ScposSetDSPtnCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Path;
    UINT32 u4Mode;
    UINT32 u4Invalid = 0;
  //  UINT32 u4HTotal, u4Width, u4VTotal, u4Height;

    if (i4Argc < 3)
    {
        u4Invalid = 1;
    }
    else if (i4Argc < 7)
    {
        u4Path = StrToInt(szArgv[1]);
        u4Mode = StrToInt(szArgv[2]);

        if ((u4Mode != VDP_DS_PTN_OFF) && (u4Mode !=  VDP_DS_PTN_AUTO))
        {
            u4Invalid = 2;
        }
    }

    if (u4Invalid)
    {
        Printf("Usage: %s <u4VdpId> <OnOff> <HTotal> <Width> <VTotal> <Height>\n", szArgv[0]);
        return 0;
    }

    u4Path = StrToInt(szArgv[1]);
    u4Mode = StrToInt(szArgv[2]);

    if ((u4Mode != VDP_DS_PTN_OFF) && (u4Mode !=  VDP_DS_PTN_AUTO))
    {
#if defined(CC_MT5363) || defined(CC_MT5387)
        vDrvSetDSPattern(u4Path, VDP_DS_PTN_MANUAL, StrToInt(szArgv[3]), StrToInt(szArgv[4]), StrToInt(szArgv[5]), StrToInt(szArgv[6]));            
#else
        vScpipSetDSPattern(u4Path, VDP_DS_PTN_MANUAL, StrToInt(szArgv[3]), StrToInt(szArgv[4]), StrToInt(szArgv[5]), StrToInt(szArgv[6]));            
#endif
    }
    else
    {
#if defined(CC_MT5363) || defined(CC_MT5387)
        vDrvSetDSPattern(u4Path, u4Mode, 0, 0, 0, 0);            
#else
        vScpipSetDSPattern(u4Path, u4Mode, 0, 0, 0, 0);            
#endif
    }

    return 0;
}

INT32 _ScposSetUSPtnCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Path;
    UINT32 u4Mode;
    UINT32 u4Invalid = 0;
  //  UINT32 u4HTotal, u4Width, u4VTotal, u4Height;

    if (i4Argc < 3)
    {
        u4Invalid = 1;
    }
    else if (i4Argc < 5)
    {
        u4Path = StrToInt(szArgv[1]);
        u4Mode = StrToInt(szArgv[2]);

        if ((u4Mode != VDP_US_PTN_OFF) && (u4Mode !=  VDP_US_PTN_AUTO))
        {
            u4Invalid = 2;
        }
    }

    if (u4Invalid)
    {
        Printf("Usage: %s <u4VdpId> <OnOff> <Width> <Height>\n", szArgv[0]);
        return 0;
    }

    u4Path = StrToInt(szArgv[1]);
    u4Mode = StrToInt(szArgv[2]);

    if ((u4Mode != VDP_US_PTN_OFF) && (u4Mode !=  VDP_US_PTN_AUTO))
    {
#if defined(CC_MT5363) || defined(CC_MT5387)
        vDrvSetUSPattern(u4Path, VDP_US_PTN_MANUAL, StrToInt(szArgv[3]), StrToInt(szArgv[4]));            
#else
        vScpipSetUSPattern(u4Path, VDP_US_PTN_MANUAL, StrToInt(szArgv[3]), StrToInt(szArgv[4]));            
#endif
    }
    else
    {
#if defined(CC_MT5363) || defined(CC_MT5387)
        vDrvSetUSPattern(u4Path, u4Mode, 0, 0);            
#else
        vScpipSetUSPattern(u4Path, u4Mode, 0, 0);            
#endif
    }

    
    return 0;
}

#ifdef CC_CLI

static INT32 _ScposSetDispSizePosCmd(INT32 i4Argc, const CHAR **szArgv)
{
  #if !defined(CC_MT5396)
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
  
#if 0 //5387
  bApiVideoSetDispPosSize(u1Path, (UINT16)StrToInt(szArgv[2])
                                , (UINT16)StrToInt(szArgv[3])
                                , (UINT16)StrToInt(szArgv[4])
                                , (UINT16)StrToInt(szArgv[5]));

  bApiVideoSetPathDispInfo(u1Path, (UINT16)StrToInt(szArgv[2])
                                , (UINT16)StrToInt(szArgv[3])
                                , (UINT16)StrToInt(szArgv[4])
                                , (UINT16)StrToInt(szArgv[5]));
#else
  bVideoSetDispRegion(u1Path, (UINT16)StrToInt(szArgv[2])
                                , (UINT16)StrToInt(szArgv[3])
                                , (UINT16)StrToInt(szArgv[4])
                                , (UINT16)StrToInt(szArgv[5]));
#endif
  #endif

  return 0;
}


static INT32 _ScposSetSrcSizePosCmd(INT32 i4Argc, const CHAR **szArgv)
{
  #if !defined(CC_MT5396)
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


  #endif
  return 0;
}



extern UINT8 _bIsImportProtection;
static INT32 _NptvImportProtectionOnOff(INT32 i4Argc, const CHAR **szArgv)
{
    if(i4Argc == 2)
    {
        if( 1 == StrToInt(szArgv[1]) )
        {
            _bIsImportProtection = 1;
        }
		else if( 0 == StrToInt(szArgv[1]) )
		{
		    _bIsImportProtection = 0;
		}
		else
		{
		    Printf("Arg: ipoo 0/1, ipoo=%d now!\n", _bIsImportProtection);
		}
    }
	else
	{
	    Printf("Arg: ipoo 0/1, ipoo=%d now!\n", _bIsImportProtection);
	}

	return 0;
}

static INT32 _ScalerTestCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT16 TestTimes;
    UINT16 TestCount;

    if (i4Argc != 2)
    {
        Printf("Arg: u2Count.\n");
    }

    TestTimes = StrToInt(szArgv[1]);
    for (TestCount = 1; TestCount <= TestTimes; TestCount++)
    {
        if (vDrvTestUpScalerFilter() == FALSE)
        {
            Printf("The %05dth SRAM test failed.\n", TestCount);
            return 0;
        }
    }
    
    Printf("Upscaler SRAM test successed.\n");
    return 0;
}

#if defined(CC_MT5365) || defined(CC_MT5395)
static INT32 _ScpipSetDramBurstCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bPath, bOnOff;
    
    if (i4Argc < 2)
    {
        Printf("Usage: <main:0/sub:1> <on:1/off:0>\n");
        return 0;
    }

    bPath = (UINT8)StrToInt(szArgv[1]);
    bOnOff= (UINT8)StrToInt(szArgv[2]);

    vScpipSetDramBurst(bPath,bOnOff);
    if(bPath==VDP_1)
    {
        vSetScposFlg(MAIN_DISP_PRM_CHG);
    }
    else
    {
        vSetScposFlg(PIP_DISP_PRM_CHG);
    }

    printf("[SetDramBurst]  path [%d], onoff[%d]\n", (INT32)bPath,(INT32)bOnOff);
    
    return 0;
}

static INT32 _ScpipNegPhaseOnOff(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bHV, bOnOff;
    
    if (i4Argc < 2)
    {
        Printf("Usage: <both:0/horizontal:1/vertical:2> <on:1/off:0>\n");
        return 0;
    }

    bHV = (UINT8)StrToInt(szArgv[1]);
    bOnOff= (UINT8)StrToInt(szArgv[2]);

    if(bHV == 0) // both
    {
        vScpipSetHNegInitPhaseOnOff(bOnOff);
        vScpipSetVNegInitPhaseOnOff(bOnOff);
    }
    else if (bHV == 1) // horizontal 
    {
        vScpipSetHNegInitPhaseOnOff(bOnOff);
    }
    else if (bHV == 2) // vertical
    {
        vScpipSetVNegInitPhaseOnOff(bOnOff);
    }

    vSetScposFlg(MAIN_DISP_PRM_CHG);
    printf("[SetNegativeInitialPhase]  direction [%d], onoff[%d]\n", (INT32)bHV,(INT32)bOnOff);
    
    return 0;    
}

static INT32 _ScpipCheckFifoOverCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bPath;
    UINT32 u4Ret;
    if (i4Argc < 2)
    {
        Printf("Usage: <main:0/sub:1>\n");
        return 0;
    }

    bPath = (UINT8)StrToInt(szArgv[1]);

    vScpipResetFifoOverflow((UINT32)bPath);
    u4Ret = u4ScpipCheckFifoOverflow((UINT32)bPath);
    Printf("[CheckFifoOver]  path [%d], u4Ret[%d]\n", (INT32)bPath,(INT32)u4Ret);    
    if(u4Ret == 0)
    {
        Printf("No fifo overflow!\n");
    }
    else if(u4Ret == 1)
    {
        Printf("Fifo overflow flag is not supported in this mode\n");
    }
    else
    {
        Printf("Fifo overflow!\n");        
    }
    
    return 0;
}
static INT32 _ScpipQueryCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;
    UINT32 pipOffset;
    if (i4Argc < 2)
    {
        Printf("Usage: <main:0/sub:1>\n");
        return 0;
    }

    u4VdpId = (UINT32)StrToInt(szArgv[1]);
    if(u4VdpId == VDP_1)
    {
        Printf("[Main Path]\n");
    }
    else
    {
        Printf("[Sub Path]\n");        
    }
    pipOffset = getPIPOffset((UINT8)u4VdpId);
    Printf("<Input Timing>\n\n");       
    NPTV_REG_PRINT(SCPIP_DS1_RO_00+pipOffset, DS1_RO_00_DS_INHLEN_1);
    NPTV_REG_PRINT(SCPIP_DS1_RO_00+pipOffset, DS1_RO_00_DS_INVLEN_1);    
    NPTV_REG_PRINT(SCPIP_DS1_RO_01+pipOffset, DS1_RO_01_DS_INHACT_DSCALER_H_1);
    NPTV_REG_PRINT(SCPIP_DS1_RO_01+pipOffset, DS1_RO_01_DS_INVACT_DSCALER_H_1);
    Printf("\n<Horizontal Scaling>\n\n");
    if(u4VdpId == VDP_1)
    {
        NPTV_REG_PRINT_HEX(SCPIP_HDS_00, SCPIP_HDS_MAIN_SCALER);
    }
    else
    {
        NPTV_REG_PRINT_HEX(SCPIP_HDS_01, SCPIP_HDS_PIP_SCALER);
    }    
    NPTV_REG_PRINT_HEX(SCPIP_DS1_00+pipOffset, DS1_00_DSCALER_H_1);
    NPTV_REG_PRINT_HEX(SCPIP_US1_00+pipOffset, US1_00_USCALE_H_1);
    NPTV_REG_PRINT(SCPIP_PIP1_01+pipOffset, PIP1_01_HL_1);   
    NPTV_REG_PRINT(SCPIP_PIP1_08+pipOffset, PIP1_08_DATA_LENGTH_1);   
    NPTV_REG_PRINT(SCPIP_PIP1_03+pipOffset, PIP1_03_RHP_1);   
    NPTV_REG_PRINT(SCPIP_PIP1_03+pipOffset, PIP1_03_RHL_1); 
    
    Printf("\n<Vertical Scaling>\n\n");
    NPTV_REG_PRINT_HEX(SCPIP_DS1_00+pipOffset, DS1_00_DSCALER_V_1);
    NPTV_REG_PRINT(SCPIP_DS1_02+pipOffset, DS1_02_VDS_BYPASS_1);
    NPTV_REG_PRINT_HEX(SCPIP_US1_00+pipOffset, US1_00_USCALE_V_1);
    NPTV_REG_PRINT(SCPIP_US1_01+pipOffset, US1_01_VUS_BYPASS_1);
    NPTV_REG_PRINT(SCPIP_US1_04+pipOffset, US1_04_VTAP_SEL_1);
    NPTV_REG_PRINT(SCPIP_PIP1_00+pipOffset, PIP1_00_VL_1);
    NPTV_REG_PRINT(SCPIP_PIP1_02+pipOffset, PIP1_02_RVP_1);   
    NPTV_REG_PRINT(SCPIP_PIP1_02+pipOffset, PIP1_02_RVL_1);   
    
    Printf("\n<Overscan>\n\n");    
    NPTV_REG_PRINT(SCPIP_PIP1_04+pipOffset, PIP1_04_HP_OFST_1);   
    NPTV_REG_PRINT(SCPIP_PIP1_04+pipOffset, PIP1_04_WSTARTP_OFST_1);   

    Printf("\n<Dram Setting>\n\n");    
    NPTV_REG_PRINT_HEX(SCPIP_PIP1_05+pipOffset, PIP1_05_WSTARTP_1);   
    NPTV_REG_PRINT_HEX(SCPIP_PIP1_06+pipOffset, PIP1_06_RSTARTP_1);   
    NPTV_REG_PRINT_HEX(SCPIP_PIP1_07+pipOffset, PIP1_07_WINC_1);   
    NPTV_REG_PRINT(SCPIP_PIP1_09+pipOffset, PIP1_09_W_EN_1);   
    NPTV_REG_PRINT(SCPIP_PIP1_09+pipOffset, PIP1_09_R_EN_1);       
    NPTV_REG_PRINT(SCPIP_PIP1_09+pipOffset, PIP1_09_FSEL_1);  
    NPTV_REG_PRINT(SCPIP_PIP1_12+pipOffset, PIP1_12_FSEL_DUMP_EN);       
    NPTV_REG_PRINT(SCPIP_PIP1_12+pipOffset, PIP1_12_FSEL_DUMP);  
    
    Printf("\n<Color Format>\n\n");    
    NPTV_REG_PRINT(SCPIP_DS1_02+pipOffset, DS1_02_VD10BSEL_1);    
    NPTV_REG_PRINT(SCPIP_DS1_02+pipOffset, DS1_02_SEL444_1);
    NPTV_REG_PRINT(SCPIP_DS1_02+pipOffset, DS1_02_SEL422TO444_1);

    Printf("\n<Output Timing>\n\n");       
    NPTV_REG_PRINT(SCPIP_PIP_OA_00, PIP_OA_00_MOHLEN);
    NPTV_REG_PRINT(SCPIP_PIP_OA_00, PIP_OA_00_MOVLEN); 
    
    return 0;
}
#ifdef CC_SCPOS_3DTV_SUPPORT
static INT32 _ScpipTv3dShift(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1LR;
    INT8 i1Shift; 
    if (i4Argc < 3)
    {
        Printf("Usage: <L:0/R:1> <i1Shift(+-16)>\n");
        return 0;
    }

    u1LR = (UINT8)StrToInt(szArgv[1]);

    if(szArgv[2][0] == '-')
    {
        i1Shift = -(StrToInt(&(szArgv[2][1])));
    }
    else
    {    
        i1Shift = StrToInt(szArgv[2]);
    }
    vScpipSet3DLRShift(VDP_1, u1LR, i1Shift);

    Printf("Set 3D TV L/R Shift to (%d) (%d)\n", u1LR, i1Shift);
    return 0;
}

static INT32 _ScpipTv3dQuery(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 i;
    UINT32 u4VdpId;
    
    if (i4Argc < 2)
    {
        Printf("Usage: <main:0/sub:1>\n");
        return 0;
    }

    u4VdpId = (UINT32)StrToInt(szArgv[1]);
    
    if(u4VdpId == VDP_1)
    {
        Printf("[Main Path]\n");
    }
    else
    {
        Printf("[Sub Path]\n");        
    }
    Printf("Query scaler 3d type:\n");
    
    for(i=0; i<SCPIP_TV3D_TYPE_MAX; i++)
    {
        if(fgIsScpipTv3dFlgSet(u4VdpId, 1<<i))
        {
            Printf("%s\n", SCPIP_TV3D_TYPE_NAMES[i]);
        }
    }
    return 0;
}

static INT32 _ScpipTv3dSetFlg(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;
    UINT32 u4Flag;
    
    if (i4Argc < 3)
    {
        Printf("Usage: <main:0/sub:1> <u4Flg>\n");
        return 0;
    }

    u4VdpId = (UINT32)StrToInt(szArgv[1]);
    u4Flag = (UINT32)StrToInt(szArgv[2]);
    vScpipTv3dClrFlg(u4VdpId, SCPIP_TV3D_ALL);
    vScpipTv3dSetFlg(u4VdpId, u4Flag);
    if(u4VdpId==VDP_1)
    {
        vSetScposFlg(MAIN_DISP_PRM_CHG);
    }
    else
    {
        vSetScposFlg(PIP_DISP_PRM_CHG);
    }
    return 0;
}

#endif
#if 0 
static BOOL _3dStatusPrint = FALSE;
static HANDLE_T _h3dThread = 0;

static void _vTv3dStatusArrayPrint(const char* szArrayName, 
    const UINT8 au1Array[], const UINT8 current, const INT8 length)
{
    INT8 i;
    
    Printf(szArrayName);
    for(i = 1; i <= length; i++)
    {
        Printf(" %d", au1Array[(current + i)%length]);
    }
    Printf("\n");
}

static void _vTv3dErrLogPrint(const char* szErrReason, const UINT8 counter, 
    const UINT8 u1LastRpos, const UINT8 u1CurRpos,
    const UINT8 u1LastLR, const UINT8 u1CurLR, const UINT8 u1CurWLR, 
    const UINT8 au1Rpos[], const UINT8 au1Wpos[], const UINT8 au1LR[])
{
    Printf(szErrReason);
    Printf("[ERR]RPOS: %d --> %d\n", u1LastRpos, u1CurRpos);            
    Printf("[ERR]LR: %d --> %d\n", u1LastLR, u1CurLR);
    Printf("[ERR]WLR: %d %d %d %d\n", 
        (u1CurWLR & 0x1)?1:0, 
        (u1CurWLR & 0x2)?1:0, 
        (u1CurWLR & 0x4)?1:0, 
        (u1CurWLR & 0x8)?1:0);

    _vTv3dStatusArrayPrint("[ERR]au1Rpos", au1Rpos, counter, 8);
    _vTv3dStatusArrayPrint("[ERR]au1Wpos", au1Wpos, counter, 8);
    _vTv3dStatusArrayPrint("[ERR]au1LR  ", au1LR, counter, 8);
    
}

static void _Tv3dStatusThread(void* pvArg)
{
    while(_3dStatusPrint)
    {
        static UINT8 u1LastRpos;
        static UINT8 u1LastLR;
        UINT8 u1CurRpos;
        UINT8 u1CurLR;
        UINT8 u1CurWLR;
        static UINT8 au1Rpos[8];
        static UINT8 au1Wpos[8];
        static UINT8 au1LR[8];
        static UINT8 counter = 0;
        static UINT32 time;

        BOOL fgIsSBStoFP = IO32ReadFldAlign(0xf00251c4, Fld(1, 31, AC_MSKB3));
        
        u1CurRpos = RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_R_POS_1);
        
        if(u1CurRpos != u1LastRpos)
        {
            if(counter >= 8)
            {
                break; // for clockwork warning
            }
            u1CurLR = RegReadFldAlign(SCPIP_SYSTEM_RO_00, SYSTEM_RO_00_RD_TV3D_OUT);
            u1CurWLR = RegReadFldAlign(SCPIP_PIP1_RO_05, PIP1_RO_05_W_TV3D_LR);
            au1Rpos[counter] = u1CurRpos;
            au1LR[counter] = u1CurLR;
            au1Wpos[counter] = RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_W_POS_1);
            
            if(u1CurLR == u1LastLR)
            {
                _vTv3dErrLogPrint("[ERR]LR toggle error\n", counter, 
                    u1LastRpos, u1CurRpos, u1LastLR, u1CurLR, u1CurWLR, au1Rpos, au1Wpos, au1LR);
                Printf("time: %08x\n", time);
            }
            else
            {
                //Printf("LR: %d --> %d\n", u1LastLR, u1CurLR);
            }


            if(fgIsSBStoFP && (((u1CurRpos%2) == (u1LastRpos%2)) 
                || ((u1LastRpos%2 == 0) && (u1CurRpos != (u1LastRpos+1)))))
            {
                _vTv3dErrLogPrint("[ERR]R_POS sequence error\n", counter, 
                    u1LastRpos, u1CurRpos, u1LastLR, u1CurLR, u1CurWLR, au1Rpos, au1Wpos, au1LR);
                Printf("time: %08x\n", time);
            }
            else if(!fgIsSBStoFP && ((u1CurRpos&0x1)==(u1LastRpos&0x1)))
            {
                _vTv3dErrLogPrint("[ERR]R_POS sequence error\n", counter, 
                    u1LastRpos, u1CurRpos, u1LastLR, u1CurLR, u1CurWLR, au1Rpos, au1Wpos, au1LR);
                Printf("time: %08x\n", time);            
            }
            u1LastLR = u1CurLR;
            ASSERT(u1LastLR== au1LR[counter]);           
            counter = (counter+1)%8;                
            time = 0;
        }

        u1LastRpos = u1CurRpos;
        time++;        
    }
}
static INT32 _ScpipTv3dStatusCmd(INT32 i4Argc, const CHAR **szArgv)
{
    BOOL fgOnOff;
    if (i4Argc < 2)
    {
        Printf("on/off\n");
        return 0;
    }

    fgOnOff = StrToInt(szArgv[1]);

    if(fgOnOff)
    {
        _3dStatusPrint = TRUE;
        if(_h3dThread == 0)
        {
            VERIFY(x_thread_create(&_h3dThread, "TV3D_STATUS", 2048, 201,
                   _Tv3dStatusThread, 0, NULL) == OSR_OK);
        }
    }
    else
    {
        _3dStatusPrint = FALSE;
        _h3dThread = 0;
    }

    return 0;
}
#endif // NDEBUG
#endif

#endif //#ifdef CC_CLI

