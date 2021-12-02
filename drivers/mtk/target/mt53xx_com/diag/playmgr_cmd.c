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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: playmgr_cmd.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file playmgr_cmd.c
 *  This file contains implementation of exported APIs of play manager's utility.
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_drv_cli.h"
#include "x_stl_lib.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_os.h"

#include "playmgr_if.h"

#define DEFINE_IS_LOG	PLAYMGR_IsLog
#include "x_debug.h"

//#define __MODEL_slt__
#ifdef __MODEL_slt__
#include "srm_drvif.h"
#include "vdp_if.h"
#include "x_timer.h"
#if defined(CC_MT5387) || defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
#include "b2r_drvif.h"
#include "b2r_if.h"
#endif
#endif


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

CLIMOD_DEBUG_FUNCTIONS(PLAYMGR)
#define PLAYMGR_CMD_PLAYMGR_0 (0)

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
// t32 host command

static INT32 _PlaymgrCmdInit(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PlaymgrCmdPlay(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PlaymgrCmdPause(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PlaymgrCmdStop(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PlaymgrCmdSpeed(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PlaymgrCmdStep(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PlaymgrCmdSeekTime(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PlaymgrCmdABRepeat(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PlaymgrCmdSeekPercent(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PlaymgrCmdSeekPos(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PlaymgrCmdTest(INT32 i4Argc, const CHAR ** szArgv);

#ifdef __MODEL_slt__
static INT32 _PlaymgrCmdSltPlay(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PlaymgrCmdSltPlayStop(INT32 i4Argc, const CHAR ** szArgv);

static VOID _PlaymgrCmdVdpNfy(UINT32 u4VdpId, VDP_COND_T eCond);
#endif


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
#ifdef __MODEL_slt__
HAL_TIME_T _rTimeRstS, _rTimeRstE, _rTimeRstDt; 
#endif

// cmd table
static CLI_EXEC_T _arPlaymgrCmdTbl[] =
{
    {
        "i", NULL, _PlaymgrCmdInit, NULL, 
        "Initialize the play manager", CLI_SUPERVISOR
    },
    {
        "play", NULL, _PlaymgrCmdPlay, NULL, 
        "Run the play manager to play AV", CLI_SUPERVISOR
    },
    {
        "pause", NULL, _PlaymgrCmdPause, NULL, 
        "Pause the play manager", CLI_SUPERVISOR
    },
    {
        "stop", NULL, _PlaymgrCmdStop, NULL, 
        "Stop the play manager", CLI_SUPERVISOR
    },
    {
        "speed", NULL, _PlaymgrCmdSpeed, NULL, 
        "Speed the play manager", CLI_SUPERVISOR
    },
    {
        "seektime", "seekt", _PlaymgrCmdSeekTime, NULL, 
        "Seek time", CLI_SUPERVISOR
    },
    {
        "abRepeat", "ab", _PlaymgrCmdABRepeat, NULL, 
        "abRepeat", CLI_SUPERVISOR
    },
    {
        "seekpercent", "seekp", _PlaymgrCmdSeekPercent, NULL, 
        "Seek percent", CLI_SUPERVISOR
    },
    {
        "seekposition", "seekb", _PlaymgrCmdSeekPos, NULL, 
        "Seek bytes", CLI_SUPERVISOR
    },
    {
        "step", "step", _PlaymgrCmdStep, NULL, 
        "Step video frame", CLI_SUPERVISOR
    },
    {
        "test", "test", _PlaymgrCmdTest, NULL, 
        "Test MM playback", CLI_SUPERVISOR
    },
#ifdef __MODEL_slt__
    {
        "sltplayback", "sltplay", _PlaymgrCmdSltPlay, NULL,
        "Slt Play", CLI_SUPERVISOR
    },
    {
        "sltstop", "sltstop", _PlaymgrCmdSltPlayStop, NULL,
        "Slt Stop", CLI_SUPERVISOR
    },
#endif
    CLIMOD_DEBUG_CLIENTRY(PLAYMGR),     // for LOG use

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(Playmgr)
{
    "playmgr",
    NULL,
    NULL,
    _arPlaymgrCmdTbl,
    "play manager command",
    CLI_SUPERVISOR
};
LINT_RESTORE


static INT32 _PlaymgrCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }

    UNUSED(i4Argc);
    UNUSED(szArgv);

    PLAYMGR_Init();
    
    return 0;
}


static INT32 _PlaymgrCmdPlay(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }

    UNUSED(i4Argc);
    UNUSED(szArgv);

#ifdef CC_53XX_SWDMX_V2
    PLAYMGR_Start(PLAYMGR_CMD_PLAYMGR_0);
#else
    PLAYMGR_Start();
#endif
    
    return 0;
}


static INT32 _PlaymgrCmdPause(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }

    UNUSED(i4Argc);
    UNUSED(szArgv);

#ifdef CC_53XX_SWDMX_V2
    PLAYMGR_Pause(PLAYMGR_CMD_PLAYMGR_0);
#else
    PLAYMGR_Pause();
#endif
    
    return 0;
}


static INT32 _PlaymgrCmdStop(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }

    UNUSED(i4Argc);
    UNUSED(szArgv);

#ifdef CC_53XX_SWDMX_V2
    PLAYMGR_Stop(PLAYMGR_CMD_PLAYMGR_0);
#else
    PLAYMGR_Stop();
#endif
    
    return 0;
}


static INT32 _PlaymgrCmdSpeed(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Speed;
    if (szArgv == NULL || i4Argc < 2)
    {
        LOG(0, "Arg: 0/1 (0:forward, 1:backward) i4Speed (1x:1000, -1x:-1000)");
        return 0;
    }
    
    UNUSED(i4Argc);
    UNUSED(szArgv);
    
    i4Speed = (INT32)StrToInt(szArgv[1]);
    if(i4Speed == 1)
    {
        i4Speed = -(INT32)StrToInt(szArgv[2]);
    }
    else
    {
        i4Speed = (INT32)StrToInt(szArgv[2]);
    }
    
#ifdef CC_53XX_SWDMX_V2
    PLAYMGR_Speed(PLAYMGR_CMD_PLAYMGR_0, i4Speed);
#else
    PLAYMGR_Speed(i4Speed);
#endif
    
    return 0;
}


static INT32 _PlaymgrCmdSeekTime(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Time;
    if (szArgv == NULL || i4Argc < 2)
    {
        LOG(0, "Arg: u4Time\n");
        return -1;
    }
    
    UNUSED(u4Time);
    UNUSED(szArgv);
    
    u4Time = (INT32)StrToInt(szArgv[1]);
    
#ifdef CC_53XX_SWDMX_V2
    PLAYMGR_SeekTime(PLAYMGR_CMD_PLAYMGR_0, u4Time);
#else
    PLAYMGR_SeekTime(u4Time);
#endif
    
    return 0;
}

static INT32 _PlaymgrCmdABRepeat(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Mode;
    if (szArgv == NULL || i4Argc < 2)
    {
        LOG(0, "Arg: [0/1/2]\n");
        LOG(0, "0: SetA position; 1: SetB position; 2: Cancel\n");
        return -1;
    }
    
    UNUSED(u4Mode);
    UNUSED(szArgv);
    
    u4Mode = (INT32)StrToInt(szArgv[1]);
    
    if (u4Mode > 2)
    {
        LOG(0, "Arg: [0/1/2]\n");
        LOG(0, "0: SetA position; 1: SetB position; 2: Cancel\n");
        return -1;
    }
    
#ifdef CC_53XX_SWDMX_V2
    PLAYMGR_ABRepeat(PLAYMGR_CMD_PLAYMGR_0, (PLAYMGR_ABREPEAT_TYPE_T)u4Mode);
#endif
    
    return 0;
}

static INT32 _PlaymgrCmdSeekPos(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Pos;
    if (szArgv == NULL || i4Argc < 2)
    {
        LOG(0, "Arg: u4Pos (only 32 bits in cli)\n");
        return -1;
    }
    
    UNUSED(u4Pos);
    UNUSED(szArgv);
    
    u4Pos = (INT32)StrToInt(szArgv[1]);
    
#ifdef CC_53XX_SWDMX_V2
    PLAYMGR_SeekPos(PLAYMGR_CMD_PLAYMGR_0, u4Pos);
#else
    PLAYMGR_SeekPos(u4Pos);
#endif
    
    return 0;
}

static INT32 _PlaymgrCmdSeekPercent(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Percent;
    if (szArgv == NULL || i4Argc < 2)
    {
        LOG(0, "Arg: u4Percent (0~100 /100)");
        return -1;
    }
    
    UNUSED(u4Percent);
    UNUSED(szArgv);
    
    u4Percent = (INT32)StrToInt(szArgv[1]);
    
#ifdef CC_53XX_SWDMX_V2
    PLAYMGR_SeekPercent(PLAYMGR_CMD_PLAYMGR_0, u4Percent);
#else
    PLAYMGR_SeekPercent(u4Percent);
#endif
    
    return 0;
}

static INT32 _PlaymgrCmdStep(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Amount;
    if (szArgv == NULL || i4Argc < 2)
    {
        LOG(0, "Arg: 0/1 (0:forward, 1:backward) 1~x (amount)");
        return 0;
    }
    
    UNUSED(i4Argc);
    UNUSED(szArgv);
    
    i4Amount = (INT32)StrToInt(szArgv[1]);
    if(i4Amount == 1)
    {
        i4Amount = -(INT32)StrToInt(szArgv[2]);
    }
    else
    {
        i4Amount = (INT32)StrToInt(szArgv[2]);
    }
    
#ifdef CC_53XX_SWDMX_V2
    PLAYMGR_Step(PLAYMGR_CMD_PLAYMGR_0, i4Amount);
#else
    PLAYMGR_Step(i4Amount);
#endif
    
    return 0;
}

#ifdef CC_53XX_SWDMX_V2
//#include "playmgr_if.h"
#include "swdmx_drvif.h"
#include "vdec_drvif.h"
//extern PLAYMGR_INFO_T _arPlayerInfo[PLAYMGR_MAX_INPUT_SRC];  // change it to non-static, tmp solution
extern SWDMX_INFO_T _arSwdmxInfo[SWDMX_SOURCE_MAX];
extern VDEC_INFO_T _rVdecInfo;
#endif
static INT32 _PlaymgrCmdTest(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Item;
    INT32 i4Item2;
    if (szArgv == NULL || i4Argc < 2)
    {
        LOG(0, "Arg: Clean playmgr/swdmx/vdec (0, 1, 2) u1SrcId (0, 1...)");
        return 0;
    }
    
    UNUSED(i4Argc);
    UNUSED(szArgv);
    
    i4Item = (INT32)StrToInt(szArgv[1]);
    i4Item2 = (INT32)StrToInt(szArgv[2]);
    UNUSED(i4Item2);
    if(i4Item > 3)
    {
        LOG(0, "Do nothing");
        return 0;
    }
    else
    {
        #ifdef CC_53XX_SWDMX_V2
        switch(i4Item)
        {
            case 0:
                /*
                if(i4Item2 < PLAYMGR_MAX_INPUT_SRC)
                {
                    x_memset(&_arPlayerInfo[i4Item2], 0xFF, sizeof(PLAYMGR_INFO_T));
                }
                */
                break;
            case 1:
                if(i4Item2 < SWDMX_SOURCE_MAX)
                {
                    x_memset(&_arSwdmxInfo[i4Item2], 0xFF, sizeof(SWDMX_INFO_T));
                }
                break;
            case 2:
                if(i4Item2 < VDEC_MAX_ES)
                {
                    x_memset(&_rVdecInfo.arVdecEsInfo[i4Item2], 0xFF, sizeof(VDEC_ES_INFO_T));
                }
                break;
            default:
                break;
        }
        #endif
    }
        
    return 0;
}

#ifdef __MODEL_slt__
#include "x_drv_if.h"
static INT32 _PlaymgrCmdSltPlay(INT32 i4Argc, const CHAR ** szArgv)
{
    PLAYMGR_STATUS_T rPlayStatus;
    PLAYMGR_SETTING_T rSettings;

    x_memset(&rPlayStatus, 0, sizeof(PLAYMGR_STATUS_T));
    x_memset(&rSettings, 0, sizeof(PLAYMGR_SETTING_T));
    
    HAL_GetTime(&_rTimeRstS);

#if 0
    CLI_Parser("n.ms dtv1");
    CLI_Parser("vdp.d 0");
    CLI_Parser("vdp.s.srcr 0 1");
    CLI_Parser("vdp.s.outr 0 1");
    CLI_Parser("n.ms dtv1");
    CLI_Parser("n.ps off");
#endif 
    DRV_TVE_CTRL_Enable(FALSE);

    CLI_Parser("n.ms dtv1");

    SRM_SetMmMode(SRM_MM_MODE_TYPE_VIDEO);


    VDP_RegCbFunc(VDP_CB_FUNC_MM_COND_IND, (UINT32)_PlaymgrCmdVdpNfy, 0, 0);
    VDP_SetEnable(0, 1);
    VDP_SetMode(0, VDP_MODE_NORMAL);
    VDP_SetInput(0, 0, 0);

#if 1
    if (!PLAYMGR_Init())
    {
        Printf("Playmgr Init failed\n");
        return -1;
    }
#endif

#if 1
    // slt doesn't stop vdec
    // so I add this before vdec_play
    PLAYMGR_VdecFlushCtrl();
    VDEC_Stop(0);
    PLAYMGR_VdecWaitCtrlDone();
#endif 

    if (szArgv == NULL || i4Argc != 4)
    {
        Printf("Argc fail in_PlaymgrCmdSltPlay\n");
        return -1;
    }

    PLAYMGR_SetContainerType((UINT32)StrToInt(szArgv[2]));
    
    PLAYMGR_SetSubType((UINT32)StrToInt(szArgv[3]));

    if (((UINT32)StrToInt(szArgv[2])) == 4)
    {
        if (((UINT32)StrToInt(szArgv[3])) == 1)
    {
#ifdef CC_53XX_SWDMX_V2
    	PLAYMGR_Open(PLAYMGR_CMD_PLAYMGR_0, "//mnt//usb_0//b05.avi");
#else
    	PLAYMGR_Open("//mnt//usb_0//b05.avi");
#endif
    } 
        else if (((UINT32)StrToInt(szArgv[3])) == 5)
    {
#ifdef CC_53XX_SWDMX_V2
    	PLAYMGR_Open(PLAYMGR_CMD_PLAYMGR_0, "//mnt//usb_0//slt.rm");
#else
    	PLAYMGR_Open("//mnt//usb_0//slt.rm");
#endif
    }     
        else if (((UINT32)StrToInt(szArgv[3])) == 2)
    {
#ifdef CC_53XX_SWDMX_V2
    	PLAYMGR_Open(PLAYMGR_CMD_PLAYMGR_0, "//mnt//usb_0//slt.wmv");
#else
    	PLAYMGR_Open("//mnt//usb_0//slt.wmv");
#endif
    }     
    else if (((UINT32)StrToInt(szArgv[2])) == 6)    // vp6
    {
#ifdef CC_53XX_SWDMX_V2
    	PLAYMGR_Open(PLAYMGR_CMD_PLAYMGR_0, "//mnt//usb_0//slt.flv");
#else
    	PLAYMGR_Open("//mnt//usb_0//slt.flv");
#endif
    }
    else if (((UINT32)StrToInt(szArgv[2])) == 4)    // vp8
    {
#ifdef CC_53XX_SWDMX_V2
    	PLAYMGR_Open(PLAYMGR_CMD_PLAYMGR_0, "//mnt//usb_0//slt.webm");
#else
    	PLAYMGR_Open("//mnt//usb_0//slt.webm");
#endif
    }
}
    else if (((UINT32)StrToInt(szArgv[2])) == 2)
    {
#ifdef CC_53XX_SWDMX_V2
    	      PLAYMGR_Open(PLAYMGR_CMD_PLAYMGR_0, "//mnt//usb_0//slt.avs");
#else
    	      PLAYMGR_Open("//mnt//usb_0//slt.avs");
#endif        
    }
    else
    {
    	return -1;
    }
    

#ifdef CC_53XX_SWDMX_V2
    if(!PLAYMGR_GetInfo(PLAYMGR_CMD_PLAYMGR_0, &rPlayStatus))
#else
    if(!PLAYMGR_GetInfo(&rPlayStatus))
#endif
    {
        Printf("Fail to get file in_PlaymgrCmdSltPlay\n");
        return -1;
    }

    rSettings.u4InfoMask |= PLAYMGR_INFO_MASK_VIDEO;
    rSettings.fgVidEnable = TRUE;
#ifdef CC_53XX_SWDMX_V2
    PLAYMGR_Set(PLAYMGR_CMD_PLAYMGR_0, &rSettings);
#else
    PLAYMGR_Set(&rSettings);
#endif
       
#if 0
    CLI_Parser("mid.i");
    CLI_Parser("mid.r");
    CLI_Parser("mid.sr 0x05b6a000 0x003a398c cpu 0 -c");
    CLI_Parser("mid.e 0");
#endif

#ifdef CC_53XX_SWDMX_V2
    PLAYMGR_Start(PLAYMGR_CMD_PLAYMGR_0);   
#else
    PLAYMGR_Start();   
#endif   


    return 0;
}

static INT32 _PlaymgrCmdSltPlayStop(INT32 i4Argc, const CHAR ** szArgv)
{
#ifdef CC_53XX_SWDMX_V2
    PLAYMGR_Stop(PLAYMGR_CMD_PLAYMGR_0);
    PLAYMGR_Close(PLAYMGR_CMD_PLAYMGR_0);
#else
    PLAYMGR_Stop();
    PLAYMGR_Close();
#endif

    DRV_TVE_CTRL_Enable(TRUE);
    
    return 0;
}

static VOID _PlaymgrCmdVdpNfy(UINT32 u4VdpId, VDP_COND_T eCond)
{
    //Printf("MMOK!!\n");
    HAL_GetTime(&_rTimeRstE);
    HAL_GetDeltaTime(&_rTimeRstDt, &_rTimeRstS, &_rTimeRstE);
    Printf("parsing and deliver sec = %d micro sec = (%ld) \n", _rTimeRstDt.u4Seconds, _rTimeRstDt.u4Micros);
}
#endif
