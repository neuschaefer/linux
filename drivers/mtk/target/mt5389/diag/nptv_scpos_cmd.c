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
#include "drv_scaler_drvif.h"
#include "frametrack_drvif.h"
#include "hw_scpos.h"

#ifndef CC_CLI
#define CC_CLI
#endif

INT32 _ScposSetDSPtnCmd(INT32 i4Argc, const CHAR **szArgv);
INT32 _ScposSetUSPtnCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScposSetPanelVOfst(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScposSetFrameTrackTarget(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScposSetFrameTrackCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScposGetFrameTrackCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScposSetSWFrameTrackCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScposSetHPorchCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScposSetVPorchCmd(INT32 i4Argc, const CHAR **szArgv);


#ifdef CC_CLI
static INT32 _ScposSetDispSizePosCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScposSetSrcSizePosCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScposSetImportOffCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvImportProtectionOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScalerTestCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipNegPhaseOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipCheckFifoOverCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipQueryCmd(INT32 i4Argc, const CHAR **szArgv);
#ifdef CC_SCPOS_3DTV_SUPPORT
static INT32 _ScpipTv3dQuery(INT32 i4Argc, const CHAR **szArgv);
#endif
#if 0   
static INT32 _ScpipTv3dStatusCmd(INT32 i4Argc, const CHAR **szArgv);
#endif
static INT32 _ScpipSetMirrorCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipSetFlipCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipSetMirrorFlipAllCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipSetTdtvInOutCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipSetTdtvResolutionCmd(INT32 i4Argc, const CHAR **szArgv);
#ifdef IS_SUPPORT_3D_PHOTO
static INT32 _ScpipSet3DPhoto(INT32 i4Argc, const CHAR **szArgv);
#endif
#endif /*#ifdef CC_CLI*/
/******************************************************************************
* CLI Command Table
******************************************************************************/

CLIMOD_DEBUG_FUNCTIONS(SCPOS)

// Table for scpos command entry
CLI_EXEC_T arScposCmdTbl[] = {
  {"dspattern", "dspt", _ScposSetDSPtnCmd, NULL, "Down Scalar Pattern", CLI_SUPERVISOR},    //porting done
  {"uspattern", "uspt", _ScposSetUSPtnCmd, NULL, "Up Scalar Pattern", CLI_SUPERVISOR},      //porting done
  {"PANEL VOFST",  "spv",  _ScposSetPanelVOfst,  NULL,  "Set Panel VOFST value",  CLI_SUPERVISOR},  
  {"frametrackTarget",  "ftt",  _ScposSetFrameTrackTarget,  NULL,  "Set Frame Track TARGET value",  CLI_SUPERVISOR},  
  {"frametrackoff",  "ft",  _ScposSetFrameTrackCmd,  NULL,  "Frame track froce on/off",  CLI_SUPERVISOR},  
  {"frametrackQuery",  "ftq",  _ScposGetFrameTrackCmd,  NULL,  "Query Frame track status",  CLI_SUPERVISOR},  
  {"setswframetrack",  "swft",  _ScposSetSWFrameTrackCmd,  NULL,  "Set SW Frame track parameters",  CLI_SUPERVISOR},  
  {"sethporch",  "hpos",  _ScposSetHPorchCmd,  NULL,  "Set H Porch",  CLI_SUPERVISOR},  
  {"setvporch",  "vpos",  _ScposSetVPorchCmd,  NULL,  "Set V Porch",  CLI_SUPERVISOR},  
  CLIMOD_DEBUG_CLIENTRY(SCPOS),
#ifdef CC_CLI
  {"Disp", "d", _ScposSetDispSizePosCmd, NULL, "Set Display Position/Size", CLI_SUPERVISOR},
  {"Src", "s", _ScposSetSrcSizePosCmd, NULL, "Set Source Position/Size", CLI_SUPERVISOR},
  {"Scaler Import Off", "ipof", _ScposSetImportOffCmd, NULL, "Set Scaler Import off", CLI_SUPERVISOR},  
  {"ImportProtectionOnOff",  "ipoo",  _NptvImportProtectionOnOff,  NULL,  "Nptv Import Protection on/off",  CLI_SUPERVISOR},  
  {"ScalerTest",    "st",  _ScalerTestCmd,             NULL,    "Scaler Table Command",  CLI_SUPERVISOR},
  {"negphaseonoff", "npoo", _ScpipNegPhaseOnOff, NULL, "Set Scaler negative initial phase on/off", CLI_SUPERVISOR},    
  {"fifoover", "fover", _ScpipCheckFifoOverCmd, NULL, "Check scaler dram fifo overflow status", CLI_SUPERVISOR},
  {"query", "q", _ScpipQueryCmd, NULL, "Query scaler status", CLI_SUPERVISOR},    
#ifdef CC_SCPOS_3DTV_SUPPORT
  {"query3d", "q3d", _ScpipTv3dQuery, NULL, "3d setting print", CLI_SUPERVISOR},    
#endif    
#if 0  
  {"3dstatus", "3ds", _ScpipTv3dStatusCmd, NULL, "3d status print", CLI_SUPERVISOR},
#endif    
  {"Mirror", "mir", _ScpipSetMirrorCmd, NULL, "Set Scaler Mirror in H dir on/off", CLI_SUPERVISOR},
  {"Flip", "flip", _ScpipSetFlipCmd, NULL, "Set Scaler Flip in V dir on/off", CLI_SUPERVISOR},
  {"MirrorFlip", "mf", _ScpipSetMirrorFlipAllCmd, NULL, "Set Scaler Mirror and Flip in both H/V for main and sub", CLI_SUPERVISOR},  
  {"3D input/output", "3dio", _ScpipSetTdtvInOutCmd, NULL, "Set 3D input/ output format", CLI_SUPERVISOR},  
  {"3D resolution", "3dres", _ScpipSetTdtvResolutionCmd, NULL, "Set 3D pre/pst resolution", CLI_SUPERVISOR},    
  #ifdef IS_SUPPORT_3D_PHOTO
  {"3D Photo", "3dpht", _ScpipSet3DPhoto, NULL, "Set 3D photo", CLI_SUPERVISOR},
  #endif
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

    vScpipReconfig(getScalerVDP(bPath));
    
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
    vScpipReconfig(getScalerVDP(bPath));

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

static INT32 _ScpipSetTdtvResolutionCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;
    E_TDTV_RES_OUTPUT preH, preV, pstH, pstV;

    if (i4Argc < 6 || StrToInt(szArgv[1])> 1 || 
        StrToInt(szArgv[2])>3 || StrToInt(szArgv[3])>3 ||
        StrToInt(szArgv[4])>3 || StrToInt(szArgv[5])>3)
    {
        Printf("Usage: <main:0/sub:1> <PreH> <PreV> <PstH> <PstV>\n");
        Printf("===============================\n");
        Printf("<PreH> <PreV> <PstH> <PstV>\n");
        Printf("0: E_TD_RES_NORMAL\n");
        Printf("1: E_TD_RES_HALF\n");
        Printf("2: E_TD_RES_DOUBLE\n");
        Printf("===============================\n");
        return 0;
    }    

    u4VdpId = StrToInt(szArgv[1]);
    preH = (E_TDTV_RES_OUTPUT)StrToInt(szArgv[2]);
    preV = (E_TDTV_RES_OUTPUT)StrToInt(szArgv[3]);
    pstH = (E_TDTV_RES_OUTPUT)StrToInt(szArgv[4]);
    pstV = (E_TDTV_RES_OUTPUT)StrToInt(szArgv[5]);

    vScpipSetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PRE_H, preH);
    vScpipSetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PRE_V, preV);
    vScpipSetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PST_H, pstH);
    vScpipSetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PST_V, pstV);

    return 0;
}
#ifdef IS_SUPPORT_3D_PHOTO
static INT32 _ScpipSet3DPhoto(INT32 i4Argc, const CHAR **szArgv)
{   
    BOOL bFix;
    TDTV_3D_CROP_INTO_T prCropInfo;
    

    if (i4Argc < 8 || (szArgv == NULL) || (szArgv[1] == NULL) 
        ||(szArgv[2] == NULL) || (szArgv[3] == NULL) || (szArgv[4] == NULL)
        ||(szArgv[5] == NULL) || (szArgv[6] == NULL) || (szArgv[7] == NULL))
    {
        Printf("Arg: <fgIsFullResolution> <fgIsFixSrcAspectRatio><u4CropTopOffset> <u4CropBottomOffset><u4CropLeftOffset><u4CropRightOffset><bFix>\n"); 
        return 0;
    }    
    prCropInfo.fgIsFullResolution = (BOOL)StrToInt(szArgv[1]);
    prCropInfo.fgIsFixSrcAspectRatio= (BOOL)StrToInt(szArgv[2]);
    prCropInfo.u4CropTopOffset= (UINT32)StrToInt(szArgv[3]);
    prCropInfo.u4CropBottomOffset= (UINT32)StrToInt(szArgv[4]);
    prCropInfo.u4CropLeftOffset= (UINT32)StrToInt(szArgv[5]);
    prCropInfo.u4CropRightOffset= (UINT32)StrToInt(szArgv[6]);
    bFix = (BOOL)StrToInt(szArgv[7]);


    Set3DPhotoRegion(&prCropInfo,bFix);


    return 0;
}
#endif

static INT32 _ScpipSetTdtvInOutCmd(INT32 i4Argc, const CHAR **szArgv)
{
    SCALER_TDTV_IN_TYPE in;
    SCALER_TDTV_OUT_TYPE out;
    UINT32 path;
    if (i4Argc < 4 || StrToInt(szArgv[1])> 1 || StrToInt(szArgv[2])>4 || StrToInt(szArgv[3])>5)
    {
        Printf("Usage: <main:0/sub:1> <3D in> <3D out>\n");
        Printf("===============================\n");
        Printf("<3D in>\n");
        Printf("0: SCALER_TDTV_IN_TYPE_2D\n");
        Printf("1: SCALER_TDTV_IN_TYPE_FS\n");
        Printf("2: SCALER_TDTV_IN_TYPE_SBS\n");
        Printf("3: SCALER_TDTV_IN_TYPE_TAB\n");        
        Printf("4: SCALER_TDTV_IN_TYPE_LI\n");
        Printf("===============================\n");
        Printf("<3D out>\n");
        Printf("0: SCALER_TDTV_OUT_TYPE_2D\n");
        Printf("1: SCALER_TDTV_OUT_TYPE_PR\n");
        Printf("2: SCALER_TDTV_OUT_TYPE_FPR\n");
        Printf("3: SCALER_TDTV_OUT_TYPE_SBS\n");        
        Printf("4: SCALER_TDTV_OUT_TYPE_SG\n");        
        Printf("5: SCALER_TDTV_OUT_TYPE_3D_TO_2D\n");        
        Printf("===============================\n");
        return 0;
    }
    path = (UINT32)StrToInt(szArgv[1]);
    in = (SCALER_TDTV_IN_TYPE)StrToInt(szArgv[2]);
    out = (SCALER_TDTV_OUT_TYPE)StrToInt(szArgv[3]);

    vScpipSetTdtvIO(path,in,out);

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
    printf("[Frame Track]  u4ForceFrameTrackCtrl= 0x%x, Mode(%d), OnOff[%d]\n",(UINT32)u4ForceFrameTrackCtrl, (UINT32)(u4GetFrameTrackMode()), (UINT32)u4OnOff);
    printf("SCPIP_FRAME_TRACK_AUTO 0\n");
    printf("SCPIP_FRAME_TRACK_ON_CLI ((UINT8)1 << 1)\n");
    printf("SCPIP_FRAME_TRACK_OFF_CLI ((UINT8)1 << 2)\n");
    printf("SCPIP_FRAME_TRACK_OFF_DS_PATTERN ((UINT8)1 << 3)\n");
    printf("SCPIP_FRAME_TRACK_TARGET %d\n", u4ScpipGetFrameTrackTargetVar());
    #endif
    return 0;
}

static INT32 _ScposSetFrameTrackTarget(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4FrameTrackTarget;
    
    if(i4Argc < 2)
    {
        Printf("Usage:<frame track target value>\n");
    }

    u4FrameTrackTarget = StrToInt(szArgv[1]);
    vScpipSetFrameTrackTargetVar(u4FrameTrackTarget);

    vScpipReconfig(SV_VP_MAIN);
    vScpipReconfig(SV_VP_PIP);

    return 0;
}


static INT32 _ScposSetPanelVOfst(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4PanelVOfst;
    if(i4Argc < 2)
    {
        Printf("Usage:<panel vofst value>\n");
    }

    u4PanelVOfst = StrToInt(szArgv[1]);

    vScpipSetPanelVOfstVar(u4PanelVOfst);

    vScpipReconfig(SV_VP_MAIN);
    vScpipReconfig(SV_VP_PIP);

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
/**
 *  @6896 porting done
 */ 
INT32 _ScposSetDSPtnCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Path;
    UINT32 u4Mode;
    UINT32 u4Invalid = 0;

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
        vScpipSetDSPattern(u4Path, VDP_DS_PTN_MANUAL, StrToInt(szArgv[3]), StrToInt(szArgv[4]), StrToInt(szArgv[5]), StrToInt(szArgv[6]));
    }
    else
    {
        vScpipSetDSPattern(u4Path, u4Mode, 0, 0, 0, 0);
    }

    return 0;
}
/**
 *  @6896 porting done
 */
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
  
  bApiVideoSetDispRegion(u1Path, (UINT16)StrToInt(szArgv[2])
                                , (UINT16)StrToInt(szArgv[3])
                                , (UINT16)StrToInt(szArgv[4])
                                , (UINT16)StrToInt(szArgv[5]));

  return 0;
}


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

  bApiVideoSetSrcRegion(u1Path, (UINT16)StrToInt(szArgv[2])
                                , (UINT16)StrToInt(szArgv[3])
                                , (UINT16)StrToInt(szArgv[4])
                                , (UINT16)StrToInt(szArgv[5]));


  return 0;
}

#if defined(SCPIP_SUPPORT_POST_SCALER)
EXTERN void vPscSetImportOff(UINT8 bOnff);
#endif

static INT32 _ScposSetImportOffCmd(INT32 i4Argc, const CHAR **szArgv)
{
    if(i4Argc == 2)
    {
        if(StrToInt(szArgv[1]) == 1)
        {
            vScpipSetFSCImportOff(SV_ON,SV_VP_MAIN);
            vScpipSetFSCImportOff(SV_ON,SV_VP_PIP);            
            #if defined(SCPIP_SUPPORT_POST_SCALER)
            vPscSetImportOff(SV_ON);
            #endif
        }
		else if(StrToInt(szArgv[1]) == 0)
		{
            vScpipSetFSCImportOff(SV_OFF,SV_VP_MAIN);
            vScpipSetFSCImportOff(SV_OFF,SV_VP_PIP);
            #if defined(SCPIP_SUPPORT_POST_SCALER)
            vPscSetImportOff(SV_OFF);
            #endif
		}
		else
		{
		    Printf("Arg: ipof 0:import_off=0, 1:import_off=1\n");
		}
    }
	else
	{
        Printf("Arg: ipof 0:import_off=0, 1:import_off=1\n");
	}

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
    if(u4Ret == 0)	// 0: no overflow
    {
        Printf("No fifo overflow!\n");
    }
    else if(u4Ret == 1)	//1: overflow
    {
        Printf("Fifo overflow!\n");
    }
    else if(u4Ret == 2) //2 : underflow
    {
        Printf("Fifo underflow!\n");
    }
    else if(u4Ret == 3) //3 : overflow & underflow
    {
        Printf("Fifo overflow & underflow!\n");
    }
    else    // not support
    {
        Printf("Fifo overflow/underflow flag not support in this mode\n");
    }
    
    return 0;
}

/**
 *  @6896 porting done
 */
static INT32 _ScpipQueryCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;
    UINT32 u4Mode;
    UINT32 pipOffset;
    if (i4Argc < 2)
    {
        Printf("Usage: <main:0/sub:1> <reg:0/FSC global:1/PSC global:2>\n");
        return 0;
    }

    u4VdpId = (UINT32)StrToInt(szArgv[1]);
    if(i4Argc >2)
    {
        u4Mode = (UINT32)StrToInt(szArgv[2]);
    }
    else
    {
        u4Mode = 0;
    }
    
    if(u4VdpId == VDP_1)
    {
        Printf("[Main Path]\n");
    }
    else
    {
        Printf("[Sub Path]\n");        
    }

    if(u4Mode == 0)
    {
        pipOffset = getPIPOffset((UINT8)u4VdpId);
        Printf("<Input Timing>\n\n");       
        NPTV_REG_PRINT(SCPIP_DS1_RO_00+pipOffset, DS1_RO_00_RD_DS1_INHLEN_1);
        NPTV_REG_PRINT(SCPIP_DS1_RO_00+pipOffset, DS1_RO_00_RD_DS1_INVLEN_1);    
        NPTV_REG_PRINT(SCPIP_DS1_RO_01+pipOffset, DS1_RO_01_RD_DS1_INHACT_1);
        NPTV_REG_PRINT(SCPIP_DS1_RO_01+pipOffset, DS1_RO_01_RD_DS1_INVACT_1);
        Printf("\n<Horizontal Scaling>\n\n");
        if(u4VdpId == VDP_1)
        {
            NPTV_REG_PRINT_HEX(SCPIP_HDS_03, HDS_03_HDS_MAIN_SCALER);
        }
        else
        {
            NPTV_REG_PRINT_HEX(SCPIP_HDS_04, HDS_04_HDS_PIP_SCALER);
        }
        NPTV_REG_PRINT_HEX(SCPIP_DS1_00+pipOffset, DS1_00_DSCALER_H_1);
        NPTV_REG_PRINT_HEX(SCPIP_US1_00+pipOffset, US1_00_USCALE_H_1);
        NPTV_REG_PRINT(SCPIP_PIP1_00+pipOffset, PIP1_00_DRAM_WR_WIDTH_H_1);   
        NPTV_REG_PRINT(SCPIP_PIP1_07+pipOffset, PIP1_07_DISP_WIDTH_H_1);   
        NPTV_REG_PRINT(SCPIP_PIP1_03+pipOffset, PIP1_03_RHP_1);   
        NPTV_REG_PRINT(SCPIP_PIP1_03+pipOffset, PIP1_03_RHL_1); 
        
        Printf("\n<Vertical Scaling>\n\n");
        NPTV_REG_PRINT_HEX(SCPIP_DS1_00+pipOffset, DS1_00_DSCALER_V_1);
        NPTV_REG_PRINT(SCPIP_DS1_02+pipOffset, DS1_02_VDS_BYPASS_1);
        NPTV_REG_PRINT_HEX(SCPIP_US1_00+pipOffset, US1_00_USCALE_V_1);
        NPTV_REG_PRINT(SCPIP_US1_01+pipOffset, US1_01_VUS_BYPASS_1);
        NPTV_REG_PRINT(SCPIP_US1_04+pipOffset, US1_04_VTAP_SEL_1);
        NPTV_REG_PRINT(SCPIP_PIP1_00+pipOffset, PIP1_00_DRAM_WR_WIDTH_V_1);   
        NPTV_REG_PRINT(SCPIP_PIP1_07+pipOffset, PIP1_07_DISP_WIDTH_V_1);    
        NPTV_REG_PRINT(SCPIP_PIP1_02+pipOffset, PIP1_02_RVP_1);   
        NPTV_REG_PRINT(SCPIP_PIP1_02+pipOffset, PIP1_02_RVL_1);   
        
        Printf("\n<Overscan>\n\n");    
        NPTV_REG_PRINT(SCPIP_PIP1_04+pipOffset, PIP1_04_HP_OFST_1);   
        NPTV_REG_PRINT(SCPIP_PIP1_04+pipOffset, PIP1_04_WSTARTP_OFST_1);   

        Printf("\n<Dram Setting>\n\n");
        NPTV_REG_PRINT_HEX(SCPIP_DRAM_M_05+pipOffset, DRAM_M_05_DA_ADDR_BASE_MSB_Y);   
        NPTV_REG_PRINT_HEX(SCPIP_DRAM_M_06+pipOffset, DRAM_M_06_DA_ADDR_BASE_LSB_Y);   
        NPTV_REG_PRINT_HEX(SCPIP_DRAM_M_07+pipOffset, DRAM_M_07_DA_ADDR_BASE_MSB_U);   
        NPTV_REG_PRINT_HEX(SCPIP_DRAM_M_08+pipOffset, DRAM_M_08_DA_ADDR_BASE_LSB_U);   
        NPTV_REG_PRINT_HEX(SCPIP_DRAM_M_09+pipOffset, DRAM_M_09_DA_ADDR_BASE_MSB_V);   
        NPTV_REG_PRINT_HEX(SCPIP_DRAM_M_0A+pipOffset, DRAM_M_0A_DA_ADDR_BASE_LSB_V);
        NPTV_REG_PRINT_HEX(SCPIP_DRAM_M_0C+pipOffset, DRAM_M_0C_DA_WADDR_LO_LIMIT);
        NPTV_REG_PRINT_HEX(SCPIP_DRAM_M_0B+pipOffset, DRAM_M_0B_DA_WADDR_HI_LIMIT);        
        NPTV_REG_PRINT(SCPIP_DRAM_M_00+pipOffset, DRAM_M_00_DA_READ_ENABLE);   
        NPTV_REG_PRINT(SCPIP_DRAM_M_00+pipOffset, DRAM_M_00_DA_WRITE_ENABLE);       
        
        Printf("\n<Color Format>\n\n");    
        NPTV_REG_PRINT(SCPIP_DS1_02+pipOffset, DS1_02_VD10BSEL_1);    
        NPTV_REG_PRINT(SCPIP_DS1_02+pipOffset, DS1_02_SEL444_1);
        NPTV_REG_PRINT(SCPIP_DS1_02+pipOffset, DS1_02_SEL422TO444_1);

        Printf("\n<Output Timing>\n\n");       
        NPTV_REG_PRINT(SCPIP_PIP_OA_00, PIP_OA_00_PANEL_HTOTAL);
        NPTV_REG_PRINT(SCPIP_PIP_OA_01, PIP_OA_01_PANEL_VTOTAL); 
    }
    else if(u4Mode == 1)
    {
        vScpipDumpResInfo(u4VdpId);
    }
    #if 0
    else if(u4Mode == 2)
    {
        vPSCDumpResInfo();    
    }
    #endif
    
    return 0;
}
#ifdef CC_SCPOS_3DTV_SUPPORT
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
#endif

#endif //#ifdef CC_CLI

