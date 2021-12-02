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
 * $RCSfile: av_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file av_cmd.c
 *  Brief of file av_cmd.c.
 *  Details of file av_cmd.c (optional).
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
 #ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "dmx_if.h"
#include "srm_drvif.h"
#include "drv_video.h"
#include "sv_const.h"
#include "x_stl_lib.h"
#include "x_rand.h"
#include "x_serial.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_hal_5381.h"
#include "x_debug.h"
#include "x_ckgen.h"
#include "x_util.h"
#include "drvcust_if.h"

LINT_EXT_HEADER_END

LINT_SUPPRESS_BRACE(534)        // Warning 534: Ignoring return value of function
LINT_SUPPRESS_BRACE(818)        // Info 818: Pointer parameter 'szArgv'

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//extern UINT8 _VDP_u1DrvDIGetBob (UINT8 u1VdpId);


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static INT32 _AvWorkAroundCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _AvInitCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _AvDtvPlayMainCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _AvDtvPlayH264Cmd (INT32 i4Argc, const CHAR ** szArgv);
#if defined(__linux__)
static INT32 _AvDtvStopMainCmd(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _AvDtvStopPipCmd(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _AvSetTvMode(INT32 i4Argc, const CHAR ** szArgv);
#endif
static INT32 _EnableMainCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _EnablePipCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _DisableMainCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _DisablePipCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SwitchMainCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SwitchPipCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _LogCmd(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _AvPopSizeCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _AvPipSizeCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _AvChangeChannelTestCmd (INT32 i4Argc, const CHAR ** szArgv);

static INT32 _AvDramMeasureCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _AvSramSetCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _AvQueryCmd (INT32 i4Argc, const CHAR ** szArgv);

#ifdef MPV_VIRTUAL_PRS
static INT32 _AvPingTestCmd (INT32 i4Argc, const CHAR ** szArgv);
#endif

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
UINT32 _u8Total  = 0;
UINT32 _u4TotalByte  = 0;
UINT32 _u4MaxValue  = 0;

static volatile UINT32 _u4AvThreadLoop = 0;
static UINT32 _u4AvThreadDelayMs = 30000;

CLI_EXEC_T arAvPlayCmdTbl[] =
{
    DECLARE_G_CMD(_AvDtvPlayMainCmd, main, m,  "1 Video/1 Audio Main"),
    DECLARE_G_CMD(_AvChangeChannelTestCmd, cct, 0, "Change Channel Test"),

    DECLARE_G_CMD(_AvDtvPlayH264Cmd, h264, 0,  "1 Video/1 Audio Main-H.264"),

    DECLARE_END_ITEM(),
};

#if defined(__linux__)
CLI_EXEC_T arAvStopCmdTbl[] =
{
    DECLARE_G_CMD(_AvDtvStopMainCmd, main, m,  "1 Video/1 Audio Main"),
    DECLARE_G_CMD(_AvDtvStopPipCmd, pip, p, "1 Video/1 Audio Pip"),
    DECLARE_END_ITEM(),
};
#endif /* __linux__ */

CLI_EXEC_T arEnablePathCmdTbl[] =
{
    DECLARE_G_CMD(_EnableMainCmd, main, m, "Enable Main Path"),
    DECLARE_G_CMD(_EnablePipCmd, pip, p, "Enable Pip Path"),
    DECLARE_END_ITEM(),
};

CLI_EXEC_T arDisablePathCmdTbl[] =
{
    DECLARE_G_CMD(_DisableMainCmd, main, m, "Diasble Main Path"),
    DECLARE_G_CMD(_DisablePipCmd, pip, p, "Diasble Pip Path"),
    DECLARE_END_ITEM(),
};

CLI_EXEC_T arSwitchPathCmdTbl[] =
{
    DECLARE_G_CMD(_SwitchMainCmd, main, m, "Change Main Input"),
    DECLARE_G_CMD(_SwitchPipCmd, pip, p, "Change Pip Input"),
    DECLARE_END_ITEM(),
};

static CLI_EXEC_T _arAvCmdTbl[] =
{
    DECLARE_G_CMD(_AvInitCmd, init, i, "Av init"),
    DECLARE_G_SUBLIST(EnablePath, enable, e, "Av enable"),
    DECLARE_G_SUBLIST(DisablePath, disable, d, "Av disable"),
    DECLARE_G_SUBLIST(SwitchPath, switch, sw, "Av switch input"),
    DECLARE_G_CMD(_AvWorkAroundCmd, wa, wa, "Av workaround"),
    DECLARE_G_SUBLIST(AvPlay, play, p, "Av play"),
#if defined(__linux__)
    DECLARE_G_SUBLIST(AvStop, stop, s, "Av stop"),
    DECLARE_G_CMD(_AvSetTvMode, tv, tv, "Tv mode"),
#endif /* LINUX */
    DECLARE_G_CMD(_LogCmd, log, l, "Av log"),
    DECLARE_CMD(_AvPopSizeCmd, pop, pop, "POP Mode"),
    DECLARE_CMD(_AvPipSizeCmd, pip, pip, "PIP Mode"),
    DECLARE_G_CMD(_AvDramMeasureCmd, dram, dram, "DRAM Measure"),
    DECLARE_G_CMD(_AvSramSetCmd, sram, sram, "SRAM setting"),
    DECLARE_G_CMD(_AvQueryCmd, query, q, "query AV info"),
#ifdef MPV_VIRTUAL_PRS
    DECLARE_CMD(_AvPingTestCmd, pingtest, pt, "Ping Test"),
#endif
    DECLARE_END_ITEM(),
};

LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(Av)
{
    "av",
    NULL,
    NULL,
    _arAvCmdTbl,
    "Audio/Video command",
    CLI_SUPERVISOR
};
LINT_RESTORE

static UINT32 _u4MainSrc = 0;
static UINT32 _u4PipSrc = 0;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _AvWorkAroundCmd
 */
//-------------------------------------------------------------------------
static INT32 _AvWorkAroundCmd (INT32 i4Argc, const CHAR ** szArgv)
{

    UNUSED(i4Argc);
    UNUSED(szArgv);

    return 0;
}


//-------------------------------------------------------------------------
/** _AvInitCmd
 */
//-------------------------------------------------------------------------
static INT32 _AvInitCmd (INT32 i4Argc, const CHAR ** szArgv)
{

    UINT32 u4PanelId;
    
   if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("Arg: Panel ID\n");
        return 0;
    }

    u4PanelId = StrToInt(szArgv[1]);

    //SRM_Init();
    // CLI_Parser("dbs.i");    

    CLI_Parser_Arg("pmx.s.p %d",u4PanelId);
    CLI_Parser("fbm.i");
    CLI_Parser("vdp.i");    
    CLI_Parser("sif.i");    
    CLI_Parser("n.i");    

    CLI_Parser("aud.i"); 
    CLI_Parser("dmx.i");
    CLI_Parser("vdec.i");


    return 0;
}


//-------------------------------------------------------------------------
/** _AvDtvPlayMainCmd
 */
//-------------------------------------------------------------------------
static INT32 _AvDtvPlayMainCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4VPid;
    UINT32 u4APid;
    UINT32 u4PcrPid;
    UINT32 u4Freq;
    BOOL fgIsDvbt = FALSE;

    if ((i4Argc < 4) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL))
    {
        Printf("Arg: FREQ V-PID A-PID PCR-PID\n");
        return 0;
    }

    u4Freq = StrToInt(szArgv[1]);
    u4Freq = u4Freq * 1000;
    u4VPid = StrToInt(szArgv[2]);
    u4APid = StrToInt(szArgv[3]);
    if ((i4Argc >= 5) && (szArgv[4] != NULL))
    {
        u4PcrPid = StrToInt(szArgv[4]);
    }
    else
    {
        u4PcrPid = u4VPid;
    }

    if((i4Argc >= 6) && (szArgv[5] != NULL))
    {
        if(x_strcmp(szArgv[5], "dvbt") == 0)
        {
            fgIsDvbt = TRUE;
        }
    }

    UNUSED(u4APid);

    CLI_Parser("vdp.e 0");
    CLI_Parser("vdp.s.mode 0 0");
    CLI_Parser("vdp.s.i 0 0 0");
    
    CLI_Parser("vdec.p 0 0");
    
#ifdef MPV_VIRTUAL_PRS
    CLI_Parser("mpv.vp.start 33 0 0x3000000 1115");
#else

    CLI_Parser("mpv.sync 0 1 0");   // video sync
        
#if 1
    CLI_Parser("aud.sync 0 2");     // audio sync

    if(!fgIsDvbt)
    {
        CLI_Parser("aud.s 0 1 2");
    }
    else
    {
        CLI_Parser("aud.s 0 1 1");
    }
#endif

    if(!fgIsDvbt)
    {
        CLI_Parser("dmx.sfe 0");
    }
    else
    {
        CLI_Parser("dmx.sfe 1");    
    }
    CLI_Parser("dmx.svt mpeg");
    
    CLI_Parser("dmx.settodecoder on off");

    CLI_Parser("aud.play");

    CLI_Parser_Arg("dmx.sp 0 %d video", u4VPid);
    CLI_Parser_Arg("dmx.sp 1 %d audio", u4APid);

#if 1
    if (u4PcrPid == u4VPid)
    {
        CLI_Parser("d.spcr 0 0 on");
        CLI_Parser("d.sync 0 0");
    }
    else
    if (u4PcrPid == u4APid)
    {
        CLI_Parser("d.spcr 1 0 on");
        CLI_Parser("d.sync 1 0");
    }
    else
    {
        DMX_PID_T rPid;

        rPid.fgEnable = 1;
        rPid.u1TsIndex = 0;
        rPid.u2Pid = u4PcrPid;
        rPid.ePcrMode = DMX_PCR_MODE_NEW;
        rPid.ePidType = DMX_PID_TYPE_NONE;

        DMX_SetPid(8,
            (DMX_PID_FLAG_VALID | DMX_PID_FLAG_TS_INDEX | DMX_PID_FLAG_PID | DMX_PID_FLAG_PCR),
            &rPid);

        CLI_Parser("d.spcr 8 0 on");
        CLI_Parser("d.sync 8 0");
    }
#endif
#endif /* MPV_VIRTUAL_PRS */

    CLI_Parser_Arg("nim.id 1");

    if(!fgIsDvbt)
    {
        CLI_Parser_Arg("nim.go atsc");
    //CLI_Parser_Arg("nim.go cable256");    
    }
    else
    {
        CLI_Parser_Arg("nim.go dvbt");
    }

    CLI_Parser_Arg("nim.freq %d", u4Freq);


    return 0;
}

//-------------------------------------------------------------------------
/** _AvDtvPlayMainCmd
 */
//-------------------------------------------------------------------------
static INT32 _AvDtvPlayH264Cmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4VPid;
    UINT32 u4APid;
    UINT32 u4PcrPid;
    UINT32 u4Freq;
    UINT32 u4RRMode = 0;
    
    if ((i4Argc < 4) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL))
    {
        Printf("Arg: FREQ V-PID A-PID PCR-PID RRmode(0:Disable, 1:1/2, 2:3/4)\n");
        return 0;
    }

    u4Freq = StrToInt(szArgv[1]);
    u4Freq = u4Freq * 1000;
    u4VPid = StrToInt(szArgv[2]);
    u4APid = StrToInt(szArgv[3]);
    if ((i4Argc >= 5) && (szArgv[4] != NULL))
    {
        u4PcrPid = StrToInt(szArgv[4]);
    }
    else
    {
        u4PcrPid = u4VPid;
    }

    UNUSED(u4APid);

    CLI_Parser("vdp.e 0");
    CLI_Parser("vdp.s.mode 0 0");
    CLI_Parser("vdp.s.i 0 0 0");
    
    CLI_Parser("vdec.p 0 2");

    if ((i4Argc >= 6) && (szArgv[5] != NULL))
    {
        u4RRMode = StrToInt(szArgv[5]);
    }
    if(u4RRMode == 1)
    {
        CLI_Parser("vdec.srr 1 0 0");
        CLI_Parser("vdec.srr 1 1 0");        
    }
    else if(u4RRMode == 2)
    {
        CLI_Parser("vdec.srr 1 0 1");
        CLI_Parser("vdec.srr 1 1 1");        
    }
    
    CLI_Parser("vdec.ss 0 1 0");   // video sync
    
#if 1
    CLI_Parser("aud.sync 0 2");     // audio sync

    CLI_Parser("aud.s 0 1 1");

    CLI_Parser("dmx.sfe 1");
    CLI_Parser("dmx.svt h264");
    CLI_Parser("dmx.settodecoder on off");

    CLI_Parser("aud.play");

    CLI_Parser_Arg("dmx.sp 0 %d video", u4VPid);
    CLI_Parser_Arg("dmx.sp 1 %d audio", u4APid);

#if 1
    if (u4PcrPid == u4VPid)
    {
        CLI_Parser("d.spcr 0 0 on");
        CLI_Parser("d.sync 0 0");
    }
    else
    if (u4PcrPid == u4APid)
    {
        CLI_Parser("d.spcr 1 0 on");
        CLI_Parser("d.sync 1 0");
    }
    else
    {
        DMX_PID_T rPid;

        rPid.fgEnable = 1;
        rPid.u1TsIndex = 0;
        rPid.u2Pid = u4PcrPid;
        rPid.ePcrMode = DMX_PCR_MODE_NEW;
        rPid.ePidType = DMX_PID_TYPE_NONE;

        DMX_SetPid(8,
            (DMX_PID_FLAG_VALID | DMX_PID_FLAG_TS_INDEX | DMX_PID_FLAG_PID | DMX_PID_FLAG_PCR),
            &rPid);

        CLI_Parser("d.spcr 8 0 on");
        CLI_Parser("d.sync 8 0");
    }
#endif
#endif

    IO_WRITE32(BIM_BASE, 0x234, 0xFF8000A1);
    IO_WRITE32(CKGEN_BASE, 0x400, 0x04000000);

    CLI_Parser_Arg("nim.go dvbt");
    CLI_Parser_Arg("nim.id 1");
    CLI_Parser_Arg("nim.dl 2");
    CLI_Parser_Arg("nim.sbw 3");
    CLI_Parser_Arg("nim.freq %d", u4Freq);

    return 0;
}

#if defined(__linux__)
//-------------------------------------------------------------------------
/** _AvStopMainCmd
 */
//-------------------------------------------------------------------------
static INT32 _AvDtvStopMainCmd(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4VPid;
    UINT32 u4APid;
    UINT32 u4PcrPid;

    if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        Printf("Arg: V-PID A-PID PCR-PID\n");
        return 0;
    }

    u4VPid = StrToInt(szArgv[1]);
    u4APid = StrToInt(szArgv[2]);
    if ((i4Argc >= 4) && (szArgv[3] != NULL))
    {
        u4PcrPid = StrToInt(szArgv[3]);
    }
    else
    {
        u4PcrPid = u4VPid;
    }
    CLI_Parser("dmx.ep 0 off");
    CLI_Parser("aud.stop");
    CLI_Parser("dmx.ep 1 off");
    CLI_Parser("dmx.fp 1");
    CLI_Parser("vdp.d 0");
    CLI_Parser("vdec.stop 0");
    CLI_Parser("dmx.fp 0");

    if (u4PcrPid == u4VPid)
    {
        CLI_Parser("d.sync 0 2");
    }
    else if (u4PcrPid == u4APid)
    {
        CLI_Parser("d.sync 1 2");
    }
    else
    {
        CLI_Parser("dmx.ep 8 off");
        CLI_Parser("d.sync 8 2");
        CLI_Parser("dmx.fp 8");
    }

    return 0;
}

//-------------------------------------------------------------------------
/** _AvStopPipCmd
 */
//-------------------------------------------------------------------------
static INT32 _AvDtvStopPipCmd(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4VPid;
    UINT32 u4APid;
    UINT32 u4PcrPid;

    if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        Printf("Arg: V-PID A-PID PCR-PID\n");
        return 0;
    }

    u4VPid = StrToInt(szArgv[1]);
    u4APid = StrToInt(szArgv[2]);
    if ((i4Argc >= 4) && (szArgv[3] != NULL))
    {
        u4PcrPid = StrToInt(szArgv[3]);
    }
    else
    {
        u4PcrPid = u4VPid;
    }
    CLI_Parser("dmx.ep 0 off");
    CLI_Parser("vdp.d 1");
    CLI_Parser("mpv.stop 0");
    CLI_Parser("mpv.flush 0");
    CLI_Parser("dmx.fp 0");

    if (u4PcrPid == u4VPid)
    {
        CLI_Parser("d.sync 0 2");
    }
    else if (u4PcrPid == u4APid)
    {
        CLI_Parser("d.sync 1 2");
    }
    else
    {
        CLI_Parser("dmx.ep 8 off");
        CLI_Parser("d.sync 8 2");
        CLI_Parser("dmx.fp 8");
    }

    return 0;
}

//-------------------------------------------------------------------------
/** _AvSetTVMode
 */
//-------------------------------------------------------------------------
static INT32 _AvSetTvMode(INT32 i4Argc, const CHAR ** szArgv)
{
    if(i4Argc < 2)
    {
        return -1024;
    }
    SRM_SetTvMode(StrToInt(szArgv[1]));
    return 0;
}

#endif

//-------------------------------------------------------------------------
/** _EnableMainCmd
 */
//-------------------------------------------------------------------------
static INT32 _EnableMainCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    CLI_Parser("vdp.e 0");
    CLI_Parser("vdp.s.m 0 0");

    return 0;
}

//-------------------------------------------------------------------------
/** _EnablePipCmd
 */
//-------------------------------------------------------------------------
static INT32 _EnablePipCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    CLI_Parser("vdp.e 1");
    CLI_Parser("vdp.s.m 1 0");

    return 0;
}

//-------------------------------------------------------------------------
/** _DisableMainCmd
 */
//-------------------------------------------------------------------------
static INT32 _DisableMainCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    CLI_Parser("vdp.d 0");
    CLI_Parser("vdp.s.m 0 1");

    return 0;
}

//-------------------------------------------------------------------------
/** _DisablePipCmd
 */
//-------------------------------------------------------------------------
static INT32 _DisablePipCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    CLI_Parser("vdp.d 1");
    CLI_Parser("vdp.s.m 1 1");

    return 0;
}

//-------------------------------------------------------------------------
/** _SwitchMainCmd
 */
//-------------------------------------------------------------------------
static INT32 _SwitchMainCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    if ((i4Argc >= 2) && (szArgv != NULL) && (szArgv[1] != NULL))
    {
        INT32 i4ReturnValue;

        if(x_strcmp(szArgv[1], "tv") ==0 )
        {
            _u4MainSrc = 1;
        }
        else if(x_strcmp(szArgv[1], "av1") == 0)
        {
            _u4MainSrc = 2;
        }
        else if(x_strcmp(szArgv[1], "sv1") == 0)
        {
            _u4MainSrc = 3;
        }
        else if(x_strcmp(szArgv[1], "sv2") == 0)
        {
            _u4MainSrc = 4;
        }
        else if(x_strcmp(szArgv[1], "sv3") == 0)
        {
            _u4MainSrc = 5;
        }
        else if(x_strcmp(szArgv[1], "av2") == 0)
        {
            _u4MainSrc = 6;
        }
        else if(x_strcmp(szArgv[1], "av3") == 0)
        {
            _u4MainSrc = 7;
        }
        else if(x_strcmp(szArgv[1], "ypbpr1") == 0)
        {
            _u4MainSrc = 8;
        }
        else if(x_strcmp(szArgv[1], "vga") == 0)
        {
            _u4MainSrc = 9;
        }
        else if(x_strcmp(szArgv[1], "dtv1") == 0)
        {
            _u4MainSrc = 10;
        }
        else if(x_strcmp(szArgv[1], "dtv2") == 0)
        {
            _u4MainSrc = 11;
        }
        else if(x_strcmp(szArgv[1], "hdmi") == 0)
        {
            _u4MainSrc = 12;
        }
        else if(x_strcmp(szArgv[1], "off") == 0)
        {
            _u4MainSrc = 0;
        }

        i4ReturnValue = CLI_Parser_Arg("n.ms %s", szArgv[1]);

        if (_u4PipSrc == _u4MainSrc)
        {
            _u4PipSrc = 0;
            CLI_Parser_Arg("n.ps off");
        }

        return i4ReturnValue;
    }

    return 0;
}

//-------------------------------------------------------------------------
/** _SwitchPipCmd
 */
//-------------------------------------------------------------------------
static INT32 _SwitchPipCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    if ((i4Argc >= 2) && (szArgv != NULL) && (szArgv[1] != NULL))
    {
        INT32 i4ReturnValue;

        if(x_strcmp(szArgv[1], "tv") ==0 )
        {
            _u4PipSrc = 1;
        }
        else if(x_strcmp(szArgv[1], "av1") == 0)
        {
            _u4PipSrc = 2;
        }
        else if(x_strcmp(szArgv[1], "sv1") == 0)
        {
            _u4PipSrc = 3;
        }
        else if(x_strcmp(szArgv[1], "sv2") == 0)
        {
            _u4PipSrc = 4;
        }
        else if(x_strcmp(szArgv[1], "sv3") == 0)
        {
            _u4PipSrc = 5;
        }
        else if(x_strcmp(szArgv[1], "av2") == 0)
        {
            _u4PipSrc = 6;
        }
        else if(x_strcmp(szArgv[1], "av3") == 0)
        {
            _u4PipSrc = 7;
        }
        else if(x_strcmp(szArgv[1], "ypbpr1") == 0)
        {
            _u4PipSrc = 8;
        }
        else if(x_strcmp(szArgv[1], "vga") == 0)
        {
            _u4PipSrc = 9;
        }
        else if(x_strcmp(szArgv[1], "dtv1") == 0)
        {
            _u4PipSrc = 10;
        }
        else if(x_strcmp(szArgv[1], "dtv2") == 0)
        {
            _u4PipSrc = 11;
        }
        else if(x_strcmp(szArgv[1], "hdmi") == 0)
        {
            _u4PipSrc = 12;
        }
        else if(x_strcmp(szArgv[1], "off") == 0)
        {
            _u4PipSrc = 0;
        }

        i4ReturnValue = CLI_Parser_Arg("n.ps %s", szArgv[1]);

        if (_u4MainSrc == _u4PipSrc)
        {
            _u4MainSrc = 0;
            CLI_Parser_Arg("n.ms off");
        }

        return i4ReturnValue;
    }

    return 0;
}

//-------------------------------------------------------------------------
/** _LogCmd
 */
//-------------------------------------------------------------------------
static INT32 _LogCmd(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4LogLevel;

    if ((i4Argc >= 2) && (szArgv != NULL) && (szArgv[1] != NULL))
    {
        u4LogLevel = (UINT32)StrToInt(szArgv[1]);

        if (u4LogLevel > 0)
        {
            CLI_Parser_Arg("dmx.d_l +X +M %d", u4LogLevel);
            CLI_Parser_Arg("fbm.d_l +X +M %d", u4LogLevel);
            CLI_Parser_Arg("vdp.d_l +X +M %d", u4LogLevel);
            CLI_Parser_Arg("mpv.d_l +X +M %d", u4LogLevel);
            CLI_Parser_Arg("vdec.d_l +X +M %d", u4LogLevel);
            CLI_Parser_Arg("aud.d_l +X +M %d", u4LogLevel);
            CLI_Parser_Arg("nptv.d_l +X +M %d", u4LogLevel);
            #ifdef ENABLE_MULTIMEDIA
            CLI_Parser_Arg("swdmx.d_l +X +M %d", u4LogLevel);
            #endif
        }
        else
        {
            CLI_Parser("dmx.d_l 0");
            CLI_Parser("fbm.d_l 0");
            CLI_Parser("vdp.d_l 0");
            CLI_Parser("mpv.d_l 0");
            CLI_Parser("vdec.d_l 0");
            CLI_Parser("aud.d_l 0");
            CLI_Parser("nptv.d_l 0");
            CLI_Parser("n.scl.d_l 0");
            CLI_Parser("n.di.d_l 0");
            CLI_Parser("n.nr.d_l 0");
            CLI_Parser("n.mute.d_l 0 ");           
            CLI_Parser("dbs.s");
            #ifdef ENABLE_MULTIMEDIA
            CLI_Parser("swdmx.d_l 0");
            #endif
        }
    }

    return 0;
}

//-------------------------------------------------------------------------
/** _AvPopSizeCmd
 */
//-------------------------------------------------------------------------
static INT32 _AvPopSizeCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Mode;

    if ((i4Argc >= 2) && (szArgv != NULL) && (szArgv[1] != NULL))
    {
        u4Mode = (UINT32)StrToInt(szArgv[1]);
    }
    else
    {
        u4Mode = 0;
    }

    if (u4Mode == 0)
    {
        CLI_Parser("vdp.s.outr 0 0 0 500 1000");
        CLI_Parser("vdp.s.outr 1 500 0 500 1000");
    }
    else
    {
        CLI_Parser("vdp.s.outr 1 0 0 500 1000");
        CLI_Parser("vdp.s.outr 0 500 0 500 1000");
    }

    return 0;
}

//-------------------------------------------------------------------------
/** _AvPopSizeCmd
 */
//-------------------------------------------------------------------------
static INT32 _AvPipSizeCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Mode;

    if ((i4Argc >= 2) && (szArgv != NULL) && (szArgv[1] != NULL))
    {
        u4Mode = (UINT32)StrToInt(szArgv[1]);
    }
    else
    {
        u4Mode = 0;
    }

    if (u4Mode == 0)
    {
        CLI_Parser("vdp.s.outr 0 1");
        CLI_Parser("vdp.s.outr 1 550 500 400 400");
    }
    else
    if (u4Mode == 1)
    {
        CLI_Parser("vdp.s.outr 0 1");
        CLI_Parser("vdp.s.outr 1 550 50 400 400");
    }
    else
    if (u4Mode == 2)
    {
        CLI_Parser("vdp.s.outr 0 1");
        CLI_Parser("vdp.s.outr 1 50 50 400 400");
    }
    else
    {
        CLI_Parser("vdp.s.outr 0 1");
        CLI_Parser("vdp.s.outr 1 50 550 400 400");
    }

    CLI_Parser("pmx.s.poa 4 3 1 0");

    return 0;
}

//-------------------------------------------------------------------------
/** _DramMesaure
 */
//-------------------------------------------------------------------------
static void _DramMesaure(const CHAR* szAgent, UINT32 u4Channel, UINT32 u4DelaySecond)
{
    UINT32 u4Mod;
    UINT32 u4DramDomainClk;
    UINT32 u4DelayClock;
    UINT32 u4Result;
    UINT32 u4Byte;
    UINT64 u8Result;
    BOOL fg32Bit, fgHalfFreqMode;

    if (u4Channel == 0)
    {
        fg32Bit = ((DRVCUST_InitGet(eDramType)==DDR_III_x1) || (DRVCUST_InitGet(eDramType)==DDR_II_x1)) ? 0 : 1;
        fgHalfFreqMode = (((IO_REG32(DRAM_BASE, 0x18) & 0x80) == 1) ? 1 : 0);
    }
    else
    {
        fg32Bit = 0;
        fgHalfFreqMode = (((IO_REG32(DRAM_CHB_BASE, 0x18) & 0x80) == 1) ? 1 : 0);
    }

    if (x_strcmp(szAgent, "audio") == 0)
    {
        u4Mod = SRM_DRV_AUD;
    }
    else
    if (x_strcmp(szAgent, "demux") == 0)
    {
        u4Mod = SRM_DRV_DMX;
    }
    else
    if (x_strcmp(szAgent, "vdoin") == 0)
    {
        u4Mod = SRM_DRV_VDOIN;
    }
    else
    if (x_strcmp(szAgent, "osd") == 0)
    {
        u4Mod = SRM_DRV_OSD;
    }
    else
    if ((x_strcmp(szAgent, "nr") == 0) || (x_strcmp(szAgent, "pscan") == 0))
    {
        u4Mod = SRM_DRV_MDDI;
    }
    else
    if (x_strcmp(szAgent, "b2r") == 0)
    {
        u4Mod = SRM_DRV_B2R;
    }
    else
    if (x_strcmp(szAgent, "cpu") == 0)
    {
        u4Mod = SRM_DRV_CPU;
    }
    else
    if (x_strcmp(szAgent, "scpos") == 0)
    {
        u4Mod = SRM_DRV_SCPOS;
    }
    else
    if (x_strcmp(szAgent, "vdec_mc") == 0)
    {
        u4Mod = SRM_DRV_VDEC_MC;
    }
    else
    if (x_strcmp(szAgent, "vld_jpeg") == 0)
    {
        u4Mod = SRM_DRV_VLD1_JPEG;
    }
    else
    if (x_strcmp(szAgent, "jpegdec") == 0)
    {
        u4Mod = SRM_DRV_JPEGDEC;
    }
    else
    if (x_strcmp(szAgent, "imgrz") == 0)
    {
        u4Mod = SRM_DRV_IMGRZ;
    }
    else
    if (x_strcmp(szAgent, "ethernet") == 0)
    {
        u4Mod = SRM_DRV_ETH;
    }
    else
    if (x_strcmp(szAgent, "usb") == 0)
    {
        u4Mod = SRM_DRV_USB;
    }
    else
    if (x_strcmp(szAgent, "gfx_h") == 0)
    {
        u4Mod = SRM_DRV_GFX_HI;
    }
    else
    if (x_strcmp(szAgent, "gfx_l") == 0)
    {
        u4Mod = SRM_DRV_GFX_LO;
    }
    else
    if (x_strcmp(szAgent, "ddi") == 0)
    {
        u4Mod = SRM_DRV_DDI;
    }
    else
    if (x_strcmp(szAgent, "dsp1") == 0)
    {
        u4Mod = SRM_DRV_DSP_1;
    }
    else
    if (x_strcmp(szAgent, "dsp2") == 0)
    {
        u4Mod = SRM_DRV_DSP_2;
    }
    else
    if (x_strcmp(szAgent, "fci") == 0)
    {
        u4Mod = SRM_DRV_FCI;
    }
    else
    if (x_strcmp(szAgent, "tcm") == 0)
    {
        u4Mod = SRM_DRV_TCM;
    }
    else
    if (x_strcmp(szAgent, "idetest") == 0)
    {
        u4Mod = SRM_DRV_IDETEST;
    }
    else
    if (x_strcmp(szAgent, "uart") == 0)
    {
        u4Mod = SRM_DRV_UART;
    }
    else
    if (x_strcmp(szAgent, "gdma") == 0)
    {
        u4Mod = SRM_DRV_GDMA;
    }
    else
    if (x_strcmp(szAgent, "pod") == 0)
    {
        u4Mod = SRM_DRV_POD;
    }
    else
    if (x_strcmp(szAgent, "venc") == 0)
    {
        u4Mod = SRM_DRV_VENC;
    }
    else
    if (x_strcmp(szAgent, "mjc_in") == 0)
    {
        u4Mod = SRM_DRV_MJC;
    }
    else
    if (x_strcmp(szAgent, "mjc_out") == 0)
    {
        u4Mod = SRM_DRV_MJC_OUT;
    }
    else
    if (x_strcmp(szAgent, "pr_gen") == 0)
    {
        u4Mod = SRM_DRV_PR_GEN;
    }
    else
    if (x_strcmp(szAgent, "gfx_3D") == 0)
    {
        u4Mod = SRM_DRV_3D_GFX;
    }
    else
    if (x_strcmp(szAgent, "total") == 0)
    {
        u4Mod = SRM_DRV_TOTAL;
    }
    else
    {
        Printf("Unknown agent [%s]\n", szAgent);
        return;
    }

    if (fgHalfFreqMode)
    {
        Printf("Agent(%s) not measured.\n", szAgent);
        return;
    }

    if (u4DelaySecond == 0)
    {
        u4DelaySecond = 1;
    }

    u4DramDomainClk = BSP_GetDomainClock(SRC_MEM_CLK);
    //u4DelayClock = (u4DramDomainClk >> ((DRVCUST_InitGet(eDramType)==DDR_III_x1) ? 1 : 0)) * u4DelaySecond;
    u4DelayClock = (u4DramDomainClk >> (fgHalfFreqMode ? 1 : 0)) * u4DelaySecond;
    u4Result = SRM_DramMonitor(u4Channel, u4Mod, u4DelayClock, u4DelaySecond * 1000);
    u4Byte = u4Result*((fg32Bit)?16:8)/u4DelaySecond;

    u8Result = (UINT64)u4Result;
    _u8Total += u8Result;
    u8Result *= 1000;
    u8Result = u8Div6432(u8Result, (UINT64)u4DramDomainClk, NULL);
    _u4TotalByte += (UINT64)u4Byte;

    if (u8Result > _u4MaxValue)
    {
        _u4MaxValue = (UINT32) u8Result;
    }

    Printf("Agent(%s) = %d o/oo (%d/%d) %d Byte/s\n", szAgent, (UINT32)u8Result,
           u4Result, (u4DelayClock << (fgHalfFreqMode ? 1 : 0)), u4Byte);
    //_u8Total += (UINT32)u8Result;
}

//-------------------------------------------------------------------------
/** _AvDramMeasureCmd
 */
//-------------------------------------------------------------------------
static INT32 _AvDramMeasureCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    if ((i4Argc >= 2) && (szArgv != NULL) && (szArgv[1] != NULL))
    {
        UINT32 u4DelaySecond = 1;
        UINT32 u4Loop = 1;
        UINT32 u4Val;
        UINT32 u4DramDomainClk;

        if ((i4Argc >= 3) && (szArgv[2] != NULL))
        {
            u4DelaySecond = (UINT32)StrToInt(szArgv[2]);
        }
        if ((i4Argc >= 4) && (szArgv[3] != NULL))
        {
            u4Loop = (UINT32)StrToInt(szArgv[3]);
        }

        Printf("--------- Channel A ---------\n");
        _u8Total = 0;
        _u4TotalByte = 0;
        _u4MaxValue = 0;

        for (u4Val = 0; u4Val < u4Loop; u4Val++)
        {
            if (x_strcmp(szArgv[1], "av") == 0)
            {
                _DramMesaure("audio", 0, u4DelaySecond);
                _DramMesaure("dsp1", 0, u4DelaySecond);
                _DramMesaure("demux", 0, u4DelaySecond);
                _DramMesaure("ddi", 0, u4DelaySecond);
                _DramMesaure("vdoin", 0, u4DelaySecond);
                _DramMesaure("osd", 0, u4DelaySecond);
                _DramMesaure("pscan", 0, u4DelaySecond);
                _DramMesaure("b2r", 0, u4DelaySecond);
                _DramMesaure("cpu", 0, u4DelaySecond);
                _DramMesaure("scpos", 0, u4DelaySecond);
                _DramMesaure("vdec_mc", 0, u4DelaySecond);
                _DramMesaure("vld_jpeg", 0, u4DelaySecond);
                _DramMesaure("gfx_h", 0, u4DelaySecond);
                _DramMesaure("gfx_l", 0, u4DelaySecond);
                _DramMesaure("venc", 0, u4DelaySecond);
                _DramMesaure("mjc_in", 0, u4DelaySecond);
                _DramMesaure("mjc_out", 0, u4DelaySecond);
            }
            else
            if (x_strcmp(szArgv[1], "all") == 0)
            {
                _DramMesaure("audio", 0, u4DelaySecond);
                _DramMesaure("dsp1", 0, u4DelaySecond);
                _DramMesaure("demux", 0, u4DelaySecond);
                _DramMesaure("ddi", 0, u4DelaySecond);
                _DramMesaure("vdoin", 0, u4DelaySecond);
                _DramMesaure("osd", 0, u4DelaySecond);
                _DramMesaure("pscan", 0, u4DelaySecond);
                _DramMesaure("b2r", 0, u4DelaySecond);
                _DramMesaure("cpu", 0, u4DelaySecond);
                _DramMesaure("scpos", 0, u4DelaySecond);
                _DramMesaure("vdec_mc", 0, u4DelaySecond);
                _DramMesaure("vld_jpeg", 0, u4DelaySecond);
                _DramMesaure("gfx_h", 0, u4DelaySecond);
                _DramMesaure("gfx_l", 0, u4DelaySecond);
                _DramMesaure("venc", 0, u4DelaySecond);
                _DramMesaure("mjc_in", 0, u4DelaySecond);
                _DramMesaure("mjc_out", 0, u4DelaySecond);                
                _DramMesaure("usb", 0, u4DelaySecond);
                _DramMesaure("ethernet", 0, u4DelaySecond);
                _DramMesaure("ddi", 0, u4DelaySecond);
                _DramMesaure("fci", 0, u4DelaySecond);
                _DramMesaure("tcm", 0, u4DelaySecond);
                _DramMesaure("uart", 0, u4DelaySecond);
                _DramMesaure("gfx_3D", 0, u4DelaySecond);
            }
            else
            {
                _DramMesaure(szArgv[1], 0, u4DelaySecond);
            }
        }
        
        u4DramDomainClk = (BSP_GetDomainClock(SRC_MEM_CLK) * u4DelaySecond) / 1000;

        _u8Total = u8Div6432(_u8Total, (UINT64)u4DramDomainClk, NULL);
        Printf("Total = %d, Max = %d, TotalByte = %d\n", _u8Total / u4Loop, _u4MaxValue, _u4TotalByte/u4Loop/1024/1024);


        Printf("------------------------------\n");
        Printf("--------- Channel B ---------\n");
        _u8Total = 0;
        _u4TotalByte = 0;
        _u4MaxValue = 0;

        for (u4Val = 0; u4Val < u4Loop; u4Val++)
        {
            if (x_strcmp(szArgv[1], "av") == 0)
            {
                _DramMesaure("vdoin", 1, u4DelaySecond);
                _DramMesaure("pscan", 1, u4DelaySecond);
                _DramMesaure("cpu", 1, u4DelaySecond);
                _DramMesaure("scpos", 1, u4DelaySecond);
                _DramMesaure("vdec_mc", 1, u4DelaySecond);
                _DramMesaure("vld_jpeg", 1, u4DelaySecond);
                _DramMesaure("gfx_l", 1, u4DelaySecond);
                _DramMesaure("mjc_in", 1, u4DelaySecond);
                _DramMesaure("mjc_out", 1, u4DelaySecond);
            }
            else
            if (x_strcmp(szArgv[1], "all") == 0)
            {
                _DramMesaure("vdoin", 1, u4DelaySecond);
                _DramMesaure("pscan", 1, u4DelaySecond);
                _DramMesaure("cpu", 1, u4DelaySecond);
                _DramMesaure("scpos", 1, u4DelaySecond);
                _DramMesaure("vdec_mc", 1, u4DelaySecond);
                _DramMesaure("vld_jpeg", 1, u4DelaySecond);
                _DramMesaure("gfx_l", 1, u4DelaySecond);
                _DramMesaure("mjc_in", 1, u4DelaySecond);
                _DramMesaure("mjc_out", 1, u4DelaySecond);
                _DramMesaure("pr_gen", 1, u4DelaySecond);
                _DramMesaure("uart", 1, u4DelaySecond);
            }
            else
            {
                _DramMesaure(szArgv[1], 1, u4DelaySecond);
            }
        }
        
        u4DramDomainClk = (BSP_GetDomainClock(SRC_MEM_CLK) * u4DelaySecond) / 1000;

        _u8Total = u8Div6432(_u8Total, (UINT64)u4DramDomainClk, NULL);
        Printf("Total = %d, Max = %d, TotalByte = %d\n", _u8Total / u4Loop, _u4MaxValue, _u4TotalByte/u4Loop/1024/1024);
        Printf("------------------------------\n");

    }
    else
    {
        Printf("av.dram agent [period second] [loop]\n");
        Printf("agent = audio, demux, vdoin, osd, nr, b2r, cpu, scpos, vdec_mc, vld_jpeg, jpegdec, imgrz, ethernet, usb, gdma, ddi, fci, dsp1, dsp2, gfx_h gfx_l, gfx_3D, tcm, idetest, uart, pod\n");
    }

    return 1;
}


static INT32 _AvSramSetCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MainSrc = 0;
    UINT32 u4POPMode = 0;
    UINT32 u4MainScaleUp = 0;
    UINT32 u4SubScaleUp = 0;
    UINT32 u4Mode = 0;

    if (i4Argc == 2)
    {
        u4Mode = (UINT32)StrToInt(szArgv[1]);

        return SRM_SramSetMode(u4Mode);
    }
    else if (i4Argc == 5)
    {
        u4MainSrc = (UINT32)StrToInt(szArgv[1]);
        u4POPMode = (UINT32)StrToInt(szArgv[2]);
        u4MainScaleUp = (UINT32)StrToInt(szArgv[3]);
        u4SubScaleUp = (UINT32)StrToInt(szArgv[4]);

        return SRM_SramSetPool(u4MainSrc, u4POPMode, u4MainScaleUp, u4SubScaleUp);
    }
    else
    {
        Printf("Usage1: av.sram [Mode{0~9}]\n");
        Printf("Usage2: av.sram [MainSrc{0=SD/VGA<960, 1=HD, 2=VGA>960}]\n"
               "\t\t[IsPop{0/1}] [IsMainScaleUp{0/1}] [IsSubScaleUp{0/1}]\n");
        Printf("current status:\n");
        SRM_SramQuery();
    }

    return 1;
}


//-------------------------------------------------------------------------
/** _AvChangeChannelTestThread
 */
//-------------------------------------------------------------------------

#ifdef __MW_CLI_DEF__
extern INT32 x_cli_parser(const CHAR* ps_cmd);
static void _AvChangeChannelTestThread(void *pvArgv)
{
    UNUSED(pvArgv);

    x_cli_parser("drv.dbs.i");

    while (_u4AvThreadLoop)
    {
        x_cli_parser("app.nav.i.s");
        x_cli_parser("drv.dbs.p");
        x_cli_parser("drv.ir.rx.send ch_up");

        x_thread_delay(_u4AvThreadDelayMs);
    }
}
#else
static void _AvChangeChannelTestThread(void *pvArgv)
{
    UNUSED(pvArgv);

    CLI_Parser("dbs.i");

    while (_u4AvThreadLoop)
    {
        CLI_Parser("dbs.p");
        CLI_Parser("ir.rx.send ch_up");

        x_thread_delay(_u4AvThreadDelayMs);
    }
}
#endif

//-------------------------------------------------------------------------
/** _AvChangeChannelTestCmd
 */
//-------------------------------------------------------------------------
static INT32 _AvChangeChannelTestCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    HANDLE_T hThread;

    if ((i4Argc >= 2) && (szArgv != NULL) && (szArgv[1] != NULL))
    {
        _u4AvThreadDelayMs =  StrToInt(szArgv[1]) * 1000;
    }

    if (_u4AvThreadLoop == 0)
    {
        INT32 i4Ret;
        _u4AvThreadLoop = 1;

        i4Ret = x_thread_create(&hThread, "CCT", 4096, DBS_THREAD1_PRIORITY, _AvChangeChannelTestThread, 0, NULL);

        if (i4Ret != OSR_OK) {
            Printf("Create thread failed.\n");
            return 1;
        }
    }

    return 0;
}

#ifdef MPV_VIRTUAL_PRS
static INT32 _AvPingTestCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    if (STORGInit() != 0)
    {
        Printf("Init storage pseudo device failed.\n");
        return -1024;
    }
    x_thread_delay(2000);
    CLI_Parser("gfx.i");
    CLI_Parser("osd.diag.pt 3 4 1024 768 1024 768");
    CLI_Parser("av.i");
    CLI_Parser("osd.p.fa 3 0");

    CLI_Parser("av.p.m 0 0");
    CLI_Parser("w 0x20022584 0x30000000");
}
#endif


//-------------------------------------------------------------------------
/**
 */
//-------------------------------------------------------------------------
static INT32 _AvQueryCmd (INT32 i4Argc, const CHAR ** szArgv)
{
#if 0
    UINT32 u4Val[8];
    UCHAR au4Msg[4][10] = {"NO_SIGNAL", "NO_SUPPORT", "UNKNOWN", "STABLE"};
#endif
    CLI_Parser("dbs.i");
    CLI_Parser("osd.q");
#if 0
    u4Val[0] = bDrvVideoSignalStatus(SV_VP_MAIN);
    Printf("[VFE] signal=%s, frame rate=%d, hv_total=(%d, %d), hv_active=(%d, %d)\n",
           au4Msg[u4Val[0]], bDrvVideoGetRefreshRate(SV_VP_MAIN),
           wDrvVideoGetHTotal(SV_VP_MAIN), wDrvVideoGetVTotal(SV_VP_MAIN),
           wDrvVideoInputWidth(SV_VP_MAIN), wDrvVideoInputHeight(SV_VP_MAIN));

    u4Val[0] = IO_READ32MSK_BOOL(VIDEO_IN0_BASE, 0x6e8, 1 << 31);
    Printf("[COMB] en3d=%d, h_lock=%d, v_lock=%d\n",
           u4Val[0], fgIsTvdHLock(), fgIsTvdVLock());

    u4Val[0] = IO_READ32MSK_BOOL(VIDEO_IN0_BASE, 0x8c0, 1 << 29);
    u4Val[1] = IO_READ32MSK_BOOL(VIDEO_IN0_BASE, 0x8c0, 1 << 24);
    u4Val[2] = IO_READ32MSK(VIDEO_IN0_BASE, 0x8c4, 0xfff);
    u4Val[3] = IO_READ32MSK(VIDEO_IN0_BASE, 0x8d0, 0x3fffff);
    u4Val[4] = IO_READ32MSK_BOOL(SCPOS_BASE, 0xdd4, 1 << 25);
    Printf("[NR] bypass=%d, 3dnr=%d, height=%d, addr=0x%07x0, 2dnr=%d\n",
           u4Val[0], u4Val[1], u4Val[2], u4Val[3], u4Val[4]);
#endif
//    u4Val[0] = IO_READ32MSK_BOOL(PSCAN_BASE, 0x4c0, 1);
//    Printf("[MDDI] bypass_pscan=%d, bob=%d\n", u4Val[0], _VDP_u1DrvDIGetBob(0));

    CLI_Parser("pmx.q");
    Printf("[DTV]\n");
    CLI_Parser("dmx.q");
    CLI_Parser("dbs.p");
    Printf("[NIM]\n");
    CLI_Parser("nim.gclk");
    CLI_Parser("nim.gcsc");
    CLI_Parser("nim.gclv");

//    CLI_Parser("aud.d_l 5");
//    CLI_Parser("aud.q");

    return 0;
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

