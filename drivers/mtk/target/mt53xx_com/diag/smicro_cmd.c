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

#include "SMicro/SMicro_if.h"
#include "SMicro/SMicro_prc.h"

#ifndef CC_CLI
#define CC_CLI
#endif

#define ONE_PARAM_CHK(u4Mode, u4Sync, LIMIT)  \
    if (i4Argc < 2)\
    {\
        Printf("Usage: %s <Mode> <sync/async 0/1>\n", szArgv[0]);\
        return 0;\
    }\
    if (i4Argc < 3)\
    {\
        u4Sync = 1;\
    }\
    else\
    {\
        u4Sync = (UINT32)StrToInt(szArgv[2]);\
    }\
    u4Mode = (UINT32)StrToInt(szArgv[1]);\
    if (u4Mode >= LIMIT)\
    {\
            Printf("Invalid parameter\n");\
            return 0;\
    }


#define ZERO_PARAM_CHK(u4Sync)\
    if (i4Argc < 2)\
    {\
        Printf("Usage: %s <sync/async 0/1>\n", szArgv[0]);\
        u4Sync = 1;\
    }\
    else\
    {\
        u4Sync = (UINT32)StrToInt(szArgv[1]);\
    }
    
    
#define ONE_PARAM_CMD(MODULE, CMD, VALUE, FCN)  \
        SMICRO_MSG rMsg;\
        rMsg.u4ModuleId = MODULE;\
        rMsg.u4CommandId = CMD;\
        rMsg.aPrm[0] = (UINT8)VALUE;\
        rMsg.pCallbackFcn = (void*)FCN;\
        vSMicroSendEvent(rMsg);  

#ifdef CC_CLI
static INT32 i4VersionCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4PanelCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4EnableBackendCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4MuteCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4DemoCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4FreqCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4ResolutionCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4FormatCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4MotionFlowModeCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4PictureModeCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4CineModeCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4FilmCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4GraphicCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4DCCCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4NRCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4MaskCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4PowerCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4NotifyBackendCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4PictureExCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4PreFreqCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4NotifyTempCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4InputCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4NotifyTConCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4SendSMicroCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 i4SetRetry(INT32 i4Argc, const CHAR **szArgv);

#endif /*#ifdef CC_CLI*/
/******************************************************************************
* CLI Command Table
******************************************************************************/

//CLIMOD_DEBUG_FUNCTIONS(NPTV)

CLI_EXEC_T arSMicroCmdTbl[] = {
  #ifdef CC_CLI
  {"version",  "ver",   i4VersionCmd,   NULL,           "Get Version Info",    CLI_GUEST},
  {"panel",  "panel",   i4PanelCmd,   NULL,           "Get Panel Info",    CLI_GUEST},
  {"enablebackend",  "eb",   i4EnableBackendCmd,   NULL,           "Enable Backend",    CLI_GUEST},
  {"lvdsmute",  "mute",   i4MuteCmd,   NULL,           "LVDS mute",    CLI_GUEST},
  {"demo",  "demo",   i4DemoCmd,   NULL,           "Demo Mode",    CLI_GUEST},
  {"freq",  "freq",   i4FreqCmd,   NULL,           "Vertical Freq",    CLI_GUEST},
  {"resolution",  "resolution",   i4ResolutionCmd,   NULL,           "Signal Resolution",    CLI_GUEST},
  {"path",  "path",   i4FormatCmd,   NULL,           "Signal Format",    CLI_GUEST},
  {"motionflow",  "mf",   i4MotionFlowModeCmd,   NULL,           "Motion Flow Mode",    CLI_GUEST},
  {"picture",  "pic",   i4PictureModeCmd,   NULL,           "Picture Mode",    CLI_GUEST},
  {"cinemotion",  "cine",   i4CineModeCmd,   NULL,           "Cine Motion Mode",    CLI_GUEST},
  {"graphic",  "graphic",   i4GraphicCmd,   NULL,           "Graphic area",    CLI_GUEST},
  {"film",  "film",   i4FilmCmd,   NULL,           "Film Mode",    CLI_GUEST},
  {"dcc",  "dcc",   i4DCCCmd,   NULL,           "DCC",    CLI_GUEST},
  {"nr",  "nr",   i4NRCmd,   NULL,           "NR Level",    CLI_GUEST},
  {"mask",  "mask",   i4MaskCmd,   NULL,           "Mask Area",    CLI_GUEST},
  {"power",  "power",   i4PowerCmd,   NULL,           "Power Status",    CLI_GUEST},
  {"notifybackend",  "nb",   i4NotifyBackendCmd,   NULL,           "Notify Backend",    CLI_GUEST},
  {"pictureex",  "picex",   i4PictureExCmd,   NULL,           "Pixture Ex",    CLI_GUEST},
  {"prefreq",  "prefreq",   i4PreFreqCmd,   NULL,           "Pre-Vertical Freq",    CLI_GUEST},
  {"notifytemp",  "temp",   i4NotifyTempCmd,   NULL,           "Notify Temperature",    CLI_GUEST},
  {"input",  "input",   i4InputCmd,   NULL,           "Input",    CLI_GUEST},
  {"notifytcon",  "tcon",   i4NotifyTConCmd,   NULL,           "Notify T-Con",    CLI_GUEST},
  {"SendCmd",  "cmd",   i4SendSMicroCmd,   NULL,           "Send SMicro Cmd",    CLI_GUEST},
  {"SetRetry",  "retry",   i4SetRetry,   NULL,           "Enable SMicro Re-send if fail",    CLI_GUEST},
  #endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_CLI


void vPrintCallbackInfo(SMICRO_CALLBACK_DATA rData)
{
    UINT32 i;

    Printf("SMicro Callback (%d, %d):\n", rData.u4ModuleId, rData.u4CommandId);

    Printf("\tData (%d): ", rData.u4DataNum);
    for (i = 0; i < rData.u4DataNum; i++)
    {
        Printf("0x%02X ", rData.aPrm[i]);
    }
    Printf("\n");

}

static INT32 i4VersionCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    UINT32 u4Sync;
    ONE_PARAM_CHK(u4Mode, u4Sync, 0xffffffff);
    if (u4Sync)
    {
        SMICRO_VER rVerInfo;
        
        if (u4SMicroGetVersion(u4Mode, &rVerInfo) == SM_OK)
        {
            Printf("SWVer: %s\n", rVerInfo.au1SWVer);
            Printf("BootVer: %s\n", rVerInfo.au1BootVer);
            Printf("NVMVer: %s\n", rVerInfo.au1NVMVer);
            Printf("PacketVer: %s\n", rVerInfo.au1PacketVer);
            Printf("ModelName: %s\n", rVerInfo.au1ModelName);
            Printf("SerialNum: %s\n", rVerInfo.au1SerialNum);
            Printf("ModelId: %s\n", rVerInfo.au1ModelId);
           // Printf("RFVer: %s\n", rVerInfo.au1RFVer);
        }
    }
    else
    {
        SMICRO_MSG rMsg;
        rMsg.u4ModuleId = MODULEID_GENERIC;
        rMsg.u4CommandId = CMD0_GET_VERSION;
        rMsg.aPrm[3] = (UINT8)(u4Mode & 0xff);
        rMsg.aPrm[2] = (UINT8)((u4Mode>>8) & 0xff);
        rMsg.aPrm[1] = (UINT8)((u4Mode>>16) & 0xff);
        rMsg.aPrm[0] = (UINT8)((u4Mode>>24) & 0xff);
        rMsg.pCallbackFcn = (void*)vPrintCallbackInfo;
        vSMicroSendEvent(rMsg);  
    }
    return 0;
}

static INT32 i4PanelCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Sync;
    
    ZERO_PARAM_CHK(u4Sync);
    
    if (u4Sync)
    {
         UINT8 au1Id[16];
         if (u4SMicroGetPanelId((UINT8 *)&au1Id) == SM_OK)
         {
             Printf("PanelID: %s\n", au1Id);
         }
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_GENERIC, CMD0_GET_PANEL_ID, 0, vPrintCallbackInfo);
    }
    return 0;
}

static INT32 i4EnableBackendCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    UINT32 u4Sync;
    ONE_PARAM_CHK(u4Mode, u4Sync, 2);
    if (u4Sync)
    {
        UINT8 u1Rtn;
        if (u4SMicroEnableBackend((UINT8)u4Mode, &u1Rtn) == SM_OK)
        {
             Printf("Backend: %d\n", (UINT32)u1Rtn);
        }
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_ENABLE_BACKEND, u4Mode, vPrintCallbackInfo);
    }
    return 0;
}

static INT32 i4MuteCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    UINT32 u4Sync;
    ONE_PARAM_CHK(u4Mode, u4Sync, 2);
    if (u4Sync)
    {
        vSMicroSetLvdsMute((UINT8)u4Mode);
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_SET_LVDS_MUTE, u4Mode, NULL);
    }
    return 0;
}

static INT32 i4DemoCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    UINT32 u4Sync;
    ONE_PARAM_CHK(u4Mode, u4Sync, NUM_OF_DEMO_MODE);
    if (u4Sync)
    {
        vSMicroSetDemoMode((SMICRO_DEMO_MODE)u4Mode);
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_SET_DEMO_MODE, u4Mode, NULL);
    }
    return 0;
}

static INT32 i4FreqCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    UINT32 u4Sync;
    ONE_PARAM_CHK(u4Mode, u4Sync, NUM_OF_VERTICAL_FREQ);
    if (u4Sync)
    {
        vSMicroSetFreq((SMICRO_VERTICAL_FREQ)u4Mode);
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_SET_VERTICAL_FREQ, u4Mode, NULL);
    }
    return 0;
}

static INT32 i4ResolutionCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    UINT32 u4Sync;
    ONE_PARAM_CHK(u4Mode, u4Sync, NUM_OF_RESOLUTION);
    if (u4Sync)
    {
        vSMicroSetSignalResolution((SMICRO_SIGNAL_RESOLUTION)u4Mode);
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_SET_RESOLUTION, u4Mode, NULL);
    }
    return 0;
}

static INT32 i4FormatCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    UINT32 u4Sync;
    ONE_PARAM_CHK(u4Mode, u4Sync, NUM_OF_SIGNAL_FORMAT);
    if (u4Sync)
    {
        vSMicroSetSignalFormat((SMICRO_SIGNAL_FORMAT)u4Mode);
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_SET_PATH, u4Mode, NULL);
    }
    return 0;
}

static INT32 i4MotionFlowModeCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    UINT32 u4Sync;
    ONE_PARAM_CHK(u4Mode, u4Sync, NUM_OF_MOTIONFLOW_MODE);
    if (u4Sync)
    {
        vSMicroSetMotionFlowMode((SMICRO_MOTIONFLOW_MODE)u4Mode);
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_SET_MOTIONFLOW_MODE, u4Mode, NULL);
    }
    return 0;
}

static INT32 i4PictureModeCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    UINT32 u4Sync;
    ONE_PARAM_CHK(u4Mode, u4Sync, NUM_OF_PICTURE_MODE);
    if (u4Sync)
    {
        vSMicroSetPictureMode((SMICRO_PICTURE_MODE)u4Mode);
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_SET_PICTURE_MODE, u4Mode, NULL);
    }
    return 0;
}

static INT32 i4CineModeCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    UINT32 u4Sync;    
    ONE_PARAM_CHK(u4Mode, u4Sync, NUM_OF_CINE_MOTION);
    if (u4Sync)
    {
        vSMicroSetCineMotionMode((SMICRO_CINE_MOTION_MODE)u4Mode);
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_SET_CINEMA_MOTION_MODE, u4Mode, NULL);
    }
    return 0;
}

static INT32 i4GraphicCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    UINT32 u4Sync;
    ONE_PARAM_CHK(u4Mode, u4Sync, 2);
    if (u4Sync)
    {
        vSMicroSetGraphic((UINT8)u4Mode);
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_SET_GRAPHIC_AREA, u4Mode, NULL);
    }
    return 0;
}

static INT32 i4FilmCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    UINT32 u4Sync;
    ONE_PARAM_CHK(u4Mode, u4Sync, NUM_OF_FILM_MODE);
    if (u4Sync)
    {
        vSMicroSetFilmMode((SMICRO_FILM_MODE)u4Mode);
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_SET_FILM_MODE, u4Mode, NULL);
    }
    return 0;
}

static INT32 i4DCCCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    UINT32 u4Sync;
    ONE_PARAM_CHK(u4Mode, u4Sync, 2);
    if (u4Sync)
    {
        vSMicroSetDCC((UINT8)u4Mode);
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_SET_DCC, u4Mode, NULL);
    }
    return 0;
}

static INT32 i4NRCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    UINT32 u4Sync;
    ONE_PARAM_CHK(u4Mode, u4Sync, 8);
    if (u4Sync)
    {
        vSMicroSetNR((UINT8)u4Mode);
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_SET_SNNR_LEVEL, u4Mode, NULL);
    }
    return 0;
}

static INT32 i4MaskCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Sync;
    UINT8 u1Idx;
    UINT16 u2X, u2Y, u2W, u2H;

    if (i4Argc < 6)
    {
        Printf("Usage: %s <Idx> <X> <Y> <Width> <Height> <sync/async 0/1>\n", szArgv[0]);
        return 0;
    }
    if (i4Argc < 7)
    {
        u4Sync = 1;
    }
    else
    {
        u4Sync = (UINT32)StrToInt(szArgv[6]);
    }
    u1Idx = (UINT8)StrToInt(szArgv[1]);
    u2X = (UINT16)StrToInt(szArgv[2]);
    u2Y = (UINT16)StrToInt(szArgv[3]);
    u2W = (UINT16)StrToInt(szArgv[4]);
    u2H = (UINT16)StrToInt(szArgv[5]);

    if (u4Sync)
    {
        vSMicroSetMask(u1Idx, u2X, u2Y, u2W, u2H);
    }
    else
    {
        SMICRO_MSG rMsg;
        rMsg.u4ModuleId = MODULEID_PANEL_PARAM;
        rMsg.u4CommandId = CMD17_SET_MASK_AREA;
        rMsg.aPrm[0] = u1Idx;
        rMsg.aPrm[6] = (UINT8)(u2X & 0xff);
        rMsg.aPrm[5] = (UINT8)((u2X>>8) & 0xff);
        rMsg.aPrm[8] = (UINT8)(u2Y & 0xff);
        rMsg.aPrm[7] = (UINT8)((u2Y>>8) & 0xff);
        rMsg.aPrm[2] = (UINT8)(u2W & 0xff);
        rMsg.aPrm[1] = (UINT8)((u2W>>8) & 0xff);        
        rMsg.aPrm[4] = (UINT8)(u2H & 0xff);
        rMsg.aPrm[3] = (UINT8)((u2H>>8) & 0xff);   
        rMsg.pCallbackFcn = NULL;
        vSMicroSendEvent(rMsg);  
    }
    return 0;
}

static INT32 i4PowerCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    UINT32 u4Sync;
    ONE_PARAM_CHK(u4Mode, u4Sync, 2);
    if (u4Sync)
    {
        vSMicroSetPower((UINT8)u4Mode);
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_SET_POWER_STATUS, u4Mode, NULL);
    }
    return 0;
}

static INT32 i4NotifyBackendCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Sync;
    
    ZERO_PARAM_CHK(u4Sync);
    
    if (u4Sync)
    {
        vSMicroNotifyBackendRdy();
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_NOTIFY_BACKEND_READY, 0, NULL);
    }
    return 0;
}

static INT32 i4PictureExCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    UINT32 u4Sync;
    ONE_PARAM_CHK(u4Mode, u4Sync, 2);
    if (u4Sync)
    {
        vSMicroSetPictureEx((UINT8)u4Mode);
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_SET_PICTURE_EX, u4Mode, NULL);
    }
    return 0;
}

static INT32 i4PreFreqCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    UINT32 u4Sync;
    ONE_PARAM_CHK(u4Mode, u4Sync, NUM_OF_VERTICAL_FREQ);
    if (u4Sync)
    {
        vSMicroPreSetFreq((SMICRO_VERTICAL_FREQ)u4Mode);
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_PRE_SET_VERTICAL_FREQ, u4Mode, NULL);
    }
    return 0;
}

static INT32 i4NotifyTempCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Sync;
    
    ZERO_PARAM_CHK(u4Sync);
    
    if (u4Sync)
    {
         UINT16 u2Temp;
         if (u4SMicroNotifyTemp((UINT16 *)&u2Temp) == SM_OK)
         {
             Printf("Temp: %d\n", u2Temp);
         }      
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_NOTIFY_TEMP, 0, vPrintCallbackInfo);
    }
    return 0;
}

static INT32 i4InputCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    UINT32 u4Sync;
    ONE_PARAM_CHK(u4Mode, u4Sync, NUM_OF_INPUT_MODE);
    if (u4Sync)
    {
        vSMicroSetInputMode((SMICRO_INPUT_MODE)u4Mode);
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_SET_INPUT, u4Mode, NULL);
    }
    return 0;
}

static INT32 i4NotifyTConCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Sync;
    
    ZERO_PARAM_CHK(u4Sync);
    
    if (u4Sync)
    {
        vSMicroNotifyTConRdy();
    }
    else
    {
        ONE_PARAM_CMD(MODULEID_PANEL_PARAM, CMD17_NOTIFY_TCON_READY, 0, NULL);
    }
    return 0;
}

static INT32 i4SendSMicroCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4ModuleId, u4CmdId;
    UINT32 u4AckLen = 0, u4DataLen;
    UINT8 *pData, *pAckData = NULL;
    INT32 i, i4Ret;

    if (i4Argc < 5)
    {
        Printf("Usage: %s <Module ID> <Command ID> <Output Len> <Input Len> <Input Data 1> ...\n",
               szArgv[0]);
        return 1;
    }
    
    u4ModuleId = StrToInt(szArgv[1]);    
    u4CmdId = StrToInt(szArgv[2]);    
    u4AckLen = StrToInt(szArgv[3]);    
    u4DataLen = StrToInt(szArgv[4]);
    if (u4DataLen > (i4Argc - 5))
    {
        Printf("Please enter correct input data !\n");
        return 1;
    }
    
    pData = x_mem_alloc(u4DataLen);
    if (pData == NULL)
    {
        Printf("Input data buffer allocation fail !\n");
        return 1;
    }

    if (u4AckLen > 0)
    {
        pAckData = x_mem_alloc(u4AckLen);
        if (pAckData == NULL)
        {
            x_mem_free(pData);
            Printf("Output data buffer allocation fail !\n");
            return 1;
        }
    }

    for (i = 0; i < u4DataLen; i ++)
    {
        pData[i] = StrToInt(szArgv[5 + i]);
    }

    i4Ret = u4SMicroSendCmd(u4ModuleId, u4CmdId, pData, u4DataLen, pAckData, u4AckLen);
    if (i4Ret != 0)
    {
        Printf("u4SMicroSendCmd fail !\n");
    }

    // Print out the output data
    if (u4AckLen > 0)
    {
        Printf("AckData = ");
        for (i = 0; i < u4AckLen; i++)
        {
            Printf("%x", pAckData[i]);
        }
        Printf("\n");
    }
    
    x_mem_free(pData);
    x_mem_free(pAckData);
    return 0;
}

extern UINT32 _u4RetryCnt;
static INT32 i4SetRetry(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Retry; 
    if (i4Argc < 2)
    {
        Printf("Usage: %s <Retry>\n", szArgv[0]);
        Printf("<Retry> 0 : no retry if command sending is failed\n");
        Printf("        1 : retry max. 3 times if command sending is failed\n");
        return 1;
    }
    
    u4Retry = StrToInt(szArgv[1]);    
    _u4RetryCnt = (u4Retry) ? 3 : 1;
    return 0;
}

#endif /* CC_CLI */

