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
 * $RCSfile: fvr_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_pvr_cmd.c
 *  Demux CLI commands
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN


#include "x_drv_cli.h"
#include "x_stl_lib.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_timer.h"
#include "x_rand.h"
#include "x_bim.h"
#include "x_os.h"

#include "dmx_mm_if.h"
#include "fvr_if.h"


//---------------------------------------------------------------------
// LOG macro related

#define DEFINE_IS_LOG	FVR_IsLog
#include "x_debug.h"

LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
LINT_SAVE_AND_DISABLE
CLIMOD_DEBUG_FUNCTIONS(FVR)
LINT_RESTORE

//#define PVR_LOOPBACK_TEST

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define PICTURE_FIND_RESULT_COUNT       0x40000

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
//
// Emulation
//
static UINT32 *_pu4PicFindData = NULL;
static UINT32 _u4PicIdx = 0;

static UINT32 _u1TsIndex = 0;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _PicSearchHandler
 */
//-----------------------------------------------------------------------------
static void _PicSearchHandler(UINT8 u1Pidx, UINT8 u1Type, UINT32 u4PktIdx, 
                              const void* pvTag)
{
    UINT8 u1Pattern;
    
    switch(u1Type)
    {
    case FVR_PIC_I_TYPE:
        Printf("Find I picture, pidx: %d, PktCount: %d\n", u1Pidx, u4PktIdx);
        u1Pattern = 0;
        break;

    case FVR_PIC_P_TYPE:
        Printf("Find P picture, pidx: %d, PktCount: %d\n", u1Pidx, u4PktIdx);
        u1Pattern = 1;
        break;

    case FVR_PIC_B_TYPE:
        Printf("Find B picture, pidx: %d, PktCount: %d\n", u1Pidx, u4PktIdx);
        u1Pattern = 2;
        break;

    default:
        Printf("Unknown picture!\n");
        u1Pattern = 3;
        break;
    }

    // Save result into memory
    _pu4PicFindData[0] = _u4PicIdx;
    _pu4PicFindData[_u4PicIdx] = ((u1Pattern & 0x7) << 29) | (u4PktIdx & 0x1FFFFFFF);
    _u4PicIdx++;
    if(_u4PicIdx >= PICTURE_FIND_RESULT_COUNT)
    {
        Printf("Picture data exceed 0x%x\n", PICTURE_FIND_RESULT_COUNT);
        _u4PicIdx = 1;      // array 0 -> total count
    }

    UNUSED(pvTag);
}


//-----------------------------------------------------------------------------
/** _PktSwapHandler
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _PktSwapHandler(UINT8 u1Pidx, UINT32 u4Addr)
{
}

//-----------------------------------------------------------------------------
/** _PVRCmdInit
 *  Command: init
 */
//-----------------------------------------------------------------------------
static INT32 _PVRCmdInit(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PVR_PLAY_T rPlay;
    FVR_GBL_T rGblPid;
    UINT32 u4Flags;

    //------------------------------------
    // Record
    //------------------------------------
    if (!FVR_Init())
    {
        return -1;
    }

    u4Flags = FVR_GBL_FLAG_ALL;

    x_memset((VOID*)&rGblPid, 0x0, sizeof(FVR_GBL_T));
    rGblPid.fgAllocateBuffer = TRUE;
    rGblPid.u2TargetPktCount = 0x200;
    rGblPid.u4BufSize = 0x100000;
    rGblPid.pfnNotifyFunc = NULL;
    rGblPid.pvNotifyTag = NULL;
    
    if(!FVR_SetGBL(u4Flags, &rGblPid))
    {
        return -1;
    }

    //------------------------------------
    // Play
    //------------------------------------    
    if(!DMX_PVRPlay_FreeBuf())
    {
        return -1;
    }

    rPlay.fgAllocBuf = TRUE;
    rPlay.u4BufSize = 0x100000;
    rPlay.u4ThresholdSize = 0x20000;
    rPlay.pfnPVRNotify = NULL;

    if(!DMX_PVRPlay_Set(PVRPLAY_FLAGS_BUFFER| PVRPLAY_FLAGS_THRESHOLD | PVRPLAY_FLAGS_CALLBACK, 
                    &rPlay))
    {
        return -1;
    }    
    
    UNUSED(i4Argc);
    UNUSED(aszArgv);

    return 0;
}


//-----------------------------------------------------------------------------
/** _FVRCmdSetTuner
 *  Command: 
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdSetTuner(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1TsIndex;
    
    if(i4Argc != 2)
    {
        Printf("%s [tuner_id]\n", aszArgv[0]);
        Printf("Current tuner id is %d\n", _u1TsIndex);
        return 0;
    }

    u1TsIndex = StrToInt(aszArgv[1]);
    if(u1TsIndex > DMX_MAX_TUNER_INDEX)
    {
        return -1;
    }

    _u1TsIndex = u1TsIndex;

    return 0;
}


//-----------------------------------------------------------------------------
/** _FVRCmdTimeStamp
 *  Command: 
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdTimeStamp(INT32 i4Argc, const CHAR** aszArgv)
{
    FVR_TIMESTAMP_T rTimeStamp;
    UINT32 u4Flag;

    u4Flag = FVR_TIMESTAMP_FLAG_NONE;
    
    if(i4Argc < 2)
    {
        FVR_GetTimestamp(FVR_TIMESTAMP_FLAG_ENABLE, &rTimeStamp);
        Printf("Insert time stamp %s\n", rTimeStamp.fgEnable ? "enable" : "disable");
        Printf("%s [on/off] [clkdiv] [mask] [init_counter]\n", aszArgv[0]);
        return 0;
    }

    if((x_strncmp(aszArgv[1], "on", sizeof("on"))) == 0)
    {
        rTimeStamp.fgEnable = TRUE;
        u4Flag |= FVR_TIMESTAMP_FLAG_ENABLE;
    }
    else if((x_strncmp(aszArgv[1], "off", sizeof("off"))) == 0)
    {
        rTimeStamp.fgEnable = FALSE;
        u4Flag |= FVR_TIMESTAMP_FLAG_ENABLE;
    }
    else
    {
        Printf("%s [on/off] [clkdiv] [mask] [init_counter]\n", aszArgv[0]);
        return -1;
    }

    if(i4Argc >= 3)
    {
        rTimeStamp.u2ClkDiv = StrToInt(aszArgv[2]);
        u4Flag |= FVR_TIMESTAMP_FLAG_CLKDIV;
    }

    if(i4Argc >= 4)
    {
        rTimeStamp.u4Mask = StrToInt(aszArgv[3]);
        u4Flag |= FVR_TIMESTAMP_FLAG_MASK;    
    }

    if(i4Argc >= 5)
    {
        rTimeStamp.u4Counter= StrToInt(aszArgv[4]);
        u4Flag |= FVR_TIMESTAMP_FLAG_COUNTER;    
    }
    
    FVR_SetTimestamp(u4Flag, &rTimeStamp);

    return 0;
}


//-----------------------------------------------------------------------------
/** _FVRCmdByPass
 *  Command: 
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdByPass(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1TsIndex;
    UINT32 u4Val;

    u1TsIndex = 0x0;
    u4Val = 0x0;
    if(i4Argc > 1)
    {
        u1TsIndex = StrToInt(aszArgv[1]);
    }

    if(i4Argc > 2)
    {
        u4Val = StrToInt(aszArgv[2]);
    }
    
    FVR_SetByPass(u1TsIndex, u4Val, TRUE);

    return 0;
}


//-----------------------------------------------------------------------------
/** _FVRCmdStart
 *  Command: 
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdStart(INT32 i4Argc, const CHAR** aszArgv)
{
    if(!FVR_Start())
    {
        return -1;
    }

    //_fgCaptureStart = TRUE;
    Printf("Starting...\n");
    
    return 0;
}


//-----------------------------------------------------------------------------
/** _FVRCmdPause
 *  Command: 
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdPause(INT32 i4Argc, const CHAR** aszArgv)
{
    FVR_Stop();

    Printf("Pause\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** _FVRCmdStop
 *  Command: 
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdStop(INT32 i4Argc, const CHAR** aszArgv)
{

    FVR_Stop();
    FVR_FlushBuffer();

    Printf("Stop\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** _FVRCmdSetVideoType
 *  Command: 
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdSetVideoType(INT32 i4Argc, const CHAR** aszArgv)
{
    if(i4Argc != 2)
    {
        Printf("%s [mpeg|h264]\n", aszArgv[0]);
        Printf("Current type: ");
        switch(FVR_GetVideoType())
        {
        case FVR_VIDEO_MPEG:
            Printf(" mpeg\n");
            break;
        default:
            Printf(" unknown\n");
            break;
        }
        return 0;
    }

    if(x_strncmp(aszArgv[1], "mpeg", sizeof("mpeg")) == 0)
    {
        if(!FVR_SetVideoType(FVR_VIDEO_MPEG))
        {
            return -1;
        }
    }
    else
    {
        Printf("Unknown type!\n");
        return -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------
static void _FVRShowSetPidUsage(const CHAR* str)
{
    Printf("%s [fvr_pidx] [pid_num] [pics] [pkts] [enc] [v_idx]\n", str);
    Printf("    pid_num: pid number\n");
    Printf("    fvr_pidx: fvr pid index\n");
    Printf("    pics: pic search [on|off]\n");
    Printf("    pkts: packet swap [on|off]\n");
    Printf("    enc: enc [on|off]\n");
    Printf("    v_idx: [0 ~ 7]\n");
}


//-----------------------------------------------------------------------------
/** _FVRCmdSetPid
 *  Command: 
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdSetPid(INT32 i4Argc, const CHAR** aszArgv)
{
    FVR_PID_T rFVRPid;
    UINT32 u4Flags;        
    UINT16 u2Pid;
    UINT8 u1FVRPidx;    

    if(i4Argc != 7)
    {
        _FVRShowSetPidUsage(aszArgv[0]);
        return -1;
    }

    u1FVRPidx = (UINT8)StrToInt(aszArgv[1]);
    u2Pid = (UINT16)StrToInt(aszArgv[2]);

    // Set FVR PID
    u4Flags = FVR_PID_FLAG_ALL & (~FVR_PID_FLAG_KEY_LEN);

    x_memset((void*)&rFVRPid, 0, sizeof(FVR_PID_T));
    rFVRPid.u1TsIndex = _u1TsIndex;
    rFVRPid.fgEnable = TRUE;
    rFVRPid.eKeyType= FVR_KEY_EVEN;
    rFVRPid.fgPktSwap = FALSE;
    rFVRPid.fgScramble = FALSE;
    rFVRPid.fgPicSearch = FALSE;
    rFVRPid.u2Pid = u2Pid;

    // Pic search
    if(x_strncmp(aszArgv[3], "on", sizeof("on")) == 0)
    {
        rFVRPid.fgPicSearch = TRUE;
        rFVRPid.u1PicSearchMask = FVR_PIC_I_MASK | FVR_PIC_P_MASK | FVR_PIC_B_MASK;
        rFVRPid.pfnPicSearchNotify = _PicSearchHandler;            
        rFVRPid.u1PerPidIdx = StrToInt(aszArgv[6]);
    }
    else if(x_strncmp(aszArgv[3], "off", sizeof("off")) == 0)
    {
        rFVRPid.fgPicSearch = FALSE;
        rFVRPid.u1PicSearchMask = 0x0;
        rFVRPid.pfnPicSearchNotify = NULL;
        rFVRPid.u1PerPidIdx = 0;
    }
    else
    {
        _FVRShowSetPidUsage(aszArgv[0]);
        return -1;        
    }

    // Packet swap
    if(x_strncmp(aszArgv[4], "on", sizeof("on")) == 0)
    {
        rFVRPid.fgPktSwap = TRUE;
        rFVRPid.pfnSwapNotify = _PktSwapHandler;            
    }
    else if(x_strncmp(aszArgv[4], "off", sizeof("off")) == 0)
    {
        rFVRPid.fgPktSwap = FALSE;
        rFVRPid.pfnSwapNotify = NULL;
    }
    else
    {
        _FVRShowSetPidUsage(aszArgv[0]);
        return -1;        
    }

    // Scramble    
    if(x_strncmp(aszArgv[5], "on", sizeof("on")) == 0)
    {
        rFVRPid.fgScramble = TRUE;
    }
    else if(x_strncmp(aszArgv[5], "off", sizeof("off")) == 0)
    {
        rFVRPid.fgScramble = FALSE;
    }
    else
    {
        _FVRShowSetPidUsage(aszArgv[0]);
        return -1;        
    }    
    
    FVR_SetPid(u1FVRPidx, u4Flags, &rFVRPid);

    return 0;
}


//-----------------------------------------------------------------------------
/** _FVRCmdGetPid
 *  Command: 
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdGetPid(INT32 i4Argc, const CHAR** aszArgv)
{
    FVR_PID_T rPid;
    UINT8 u1Pidx;
    
    if(i4Argc != 2)
    {
        Printf("%s [pidx]\n", aszArgv[0]);
        return -1;
    }

    u1Pidx = StrToInt(aszArgv[1]);

    FVR_GetPid(u1Pidx, FVR_PID_FLAG_ALL, &rPid);

    Printf("Pidx %d:\n", u1Pidx);
    Printf("  Pid: 0x%x\n", rPid.u2Pid);
    Printf("  Packet Swap: %s\n", rPid.fgPktSwap ? "Enable" : "Disable");
    Printf("  Picture search: %s\n", rPid.fgPicSearch ? "Enable" : "Disable");
    Printf("  Scramble: %s, %s\n", 
                    rPid.fgScramble ? "Enable" : "Disable",
                    rPid.fgCBCMode ? "CBC" : "EBC");

    switch(rPid.eKeyType)
    {
    }
    
    return 0;
}


//-----------------------------------------------------------------------------
/** _FVRCmdFreeFVRPid
 *  Command: 
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdFreeFVRPid(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1Pidx;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 2)
    {
        Printf("Usage: %s pid_index(0-%u)\n", aszArgv[0],
            DMX_NUM_PID_INDEX - 1);
        return -1;
    }

    u1Pidx = (UINT8)StrToInt(aszArgv[1]);
#if 0    
    if (!DMX_FreePid(u1Pidx))
    {
        return -1;
    }
#endif
    if(!FVR_FreePid(u1Pidx))
    {
        return -1;
    }

    Printf("Free PID OK\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** _FVRCmdPktCount
 *  Command: 
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdPktCount(INT32 i4Argc, const CHAR** aszArgv)
{
    FVR_GBL_T rGblPid;
    UINT32 u4Count;

    if(i4Argc!=2)
    {
        Printf("%s [count]\n", aszArgv[0]);
        return -1;
    }

    u4Count = StrToInt(aszArgv[1]);

    rGblPid.u2TargetPktCount = u4Count;
    
    FVR_SetGBL(FVR_GBL_FLAG_TARGET_PKT, &rGblPid);
    
    return 0;
}


//-----------------------------------------------------------------------------
/** _FVRCmdSetAESMode
 *  Command: 
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdSetAESMode(INT32 i4Argc, const CHAR** aszArgv)
{
    FVR_PID_T rPid;
    UINT32 u4Flags;
    UINT8 u1Pidx;
    
    if(i4Argc != 4)
    {
        Printf("%s [pidx] [cbc/ebc] [e/o/b]\n", aszArgv[0]);
        return -1;
    }

    u4Flags = FVR_PID_FLAG_NONE;
    u1Pidx = StrToInt(aszArgv[1]);

    // Set CBC mode
    if(x_strncmp(aszArgv[2], "cbc", sizeof("cbc")) == 0)
    {
        rPid.fgCBCMode = TRUE;
    }
    else if(x_strncmp(aszArgv[2], "ebc", sizeof("ebc")) == 0)
    {
        rPid.fgCBCMode = FALSE;
    }
    else
    {
        Printf("%s [pidx] [cbc/ebc] [e/o/b]\n", aszArgv[0]);
        return -1;
    }
    u4Flags |= FVR_PID_FLAG_CBC_MODE;

    // Set key type
    if(x_strncmp(aszArgv[3], "e", sizeof("e")) == 0)
    {
        rPid.eKeyType= FVR_KEY_EVEN;
    }
    else if(x_strncmp(aszArgv[3], "o", sizeof("o")) == 0)
    {
        rPid.eKeyType = FVR_KEY_ODD;
    }
    else if(x_strncmp(aszArgv[3], "b", sizeof("b")) == 0)
    {
        rPid.eKeyType = FVR_KEY_BOTH;
    }
    else
    {
        Printf("%s [pidx] [cbc/ebc] [e/o/b]\n", aszArgv[0]);
        return -1;
    }
    u4Flags |= FVR_PID_FLAG_KEY_TYPE;

    FVR_SetPid(u1Pidx, u4Flags, &rPid);

    Printf("Set AES mode OK\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** _FVRCmdPicSearchTest
 *  Command: 
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdPicSearchTest(INT32 i4Argc, const CHAR** aszArgv)
{
    if(_pu4PicFindData != NULL)
    {
        x_mem_free((void*)_pu4PicFindData);
    }
    
    _pu4PicFindData = (UINT32*)x_mem_alloc(PICTURE_FIND_RESULT_COUNT * 4);
    x_memset((void*)_pu4PicFindData, 0, PICTURE_FIND_RESULT_COUNT * 4);
    _u4PicIdx = 0x1;
    Printf("Picture find data locates at 0x%x\n", (UINT32)_pu4PicFindData);

    return 0;
}


//-----------------------------------------------------------------------------
/** _FVRCmdQuery
 *  Command: 
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdQuery(INT32 i4Argc, const CHAR** aszArgv)
{
    FVR_QUERY_INFO_T rInfo;
    FVR_PID_T rFVRPid;
    UINT8 i;

    //------------------------------------------------------------
    // FVR buffer
    //------------------------------------------------------------
    if(FVR_IsRecording())
    {
        Printf("FVR is recording\n");
    }
    else
    {
        Printf("FVR stop record\n");
    }

    FVR_QueryStatus(&rInfo);
    Printf("Receive pkt count: %d\n", rInfo.u4PktCount);
    Printf("Buf: 0x%08x ~ 0x%08x, WP: 0x%08x, RP: 0x%08x\n", 
                        rInfo.u4BufStart, rInfo.u4BufEnd, rInfo.u4Wp, rInfo.u4Rp);
    Printf("Buf peak: 0x%x\n", rInfo.u4PktCount);
    Printf("Overflow count: %d\n", rInfo.u4OverflowCount);

    //------------------------------------------------------------
    // Reocrd PID
    //------------------------------------------------------------
    for(i=0; i<FVR_NUM_PID_INDEX; i++)
    {
        if(!FVR_GetPid(i, FVR_PID_FLAG_ENABLE | FVR_PID_FLAG_PID | FVR_PID_FLAG_PICSEARCH |
                          FVR_PID_FLAG_PKTSWAP | FVR_PID_FLAG_SCRAMBLE_ENABLE, &rFVRPid))
        {
            return -1;
        }

        if(rFVRPid.fgEnable)
        {
            Printf("FVR, %02d, Pid: 0x%x enable, pic_serach:%d, swap:%d, encry:%d\n", 
                        i, rFVRPid.u2Pid, rFVRPid.fgPicSearch, rFVRPid.fgPktSwap,
                        rFVRPid.fgScramble);
        }
    }

    Printf("\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** _FVRCmdSetKeyIdx
 *  Command: 
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdSetKeyIdx(INT32 i4Argc, const CHAR** aszArgv)
{
    FVR_PID_T rPid;
    UINT8 u1Pidx;

    if(i4Argc != 4)
    {
        Printf("%s [pidx] [key_idx] [key_len]\n", aszArgv[0]);
        return -1;
    }

    u1Pidx = StrToInt(aszArgv[1]);
    rPid.u1KeyIdx = StrToInt(aszArgv[2]);
    rPid.u4KeyLen = StrToInt(aszArgv[3]);

    if(!FVR_SetPid(u1Pidx, FVR_PID_FLAG_KEY_LEN | FVR_PID_FLAG_KEY_INDEX, &rPid))
    {
        Printf("Fail\n");
        return -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** _FVRCmdSetAESKey
 *  Command: 
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdSetAESKey(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4KeyLen, u4Key;
    UINT8 u1KeyIdx;
    BOOL fgEvenKey;
    UINT8 au1Key[32];
    
    if(i4Argc < 8)
    {
        Printf("%s [k_idx] [k_len] [e/o] [key...]\n", aszArgv[0]);
        return -1;
    }

    u1KeyIdx = StrToInt(aszArgv[1]);
    u4KeyLen = StrToInt(aszArgv[2]);

    if((u4KeyLen != 128) && (u4KeyLen != 192) && (u4KeyLen != 256))
    {
        Printf("Error key length, %d\n", u4KeyLen);
        return -1;
    }

    if(x_strncmp(aszArgv[3], "e", sizeof("e")) == 0)
    {
        fgEvenKey = TRUE;
    }
    else if(x_strncmp(aszArgv[3], "o", sizeof("o")) == 0)
    {
        fgEvenKey = FALSE;
    }
    else
    {
        Printf("%s [pidx] [k_len] [e/o] [key...]\n", aszArgv[0]);
        return -1;
    }

    for(i=0; i<32; i++)
    {
        au1Key[i] = 0;
    }

    if(u4KeyLen == 128)
    {
        for(i=0; i<4; i++)
        {
            u4Key = StrToInt(aszArgv[4 + i]);
            // Following decrypt byte sequence
            au1Key[(i * 4) + 0] = GET_BYTE(u4Key, 3);
            au1Key[(i * 4) + 1] = GET_BYTE(u4Key, 2);
            au1Key[(i * 4) + 2] = GET_BYTE(u4Key, 1);
            au1Key[(i * 4) + 3] = GET_BYTE(u4Key, 0);
        }
    }
    else if(u4KeyLen == 192)
    {
        if(i4Argc < 10)
        {
            Printf("Not enough key len\n");
            return -1;
        }
        
        for(i=0; i<6; i++)
        {
            u4Key = StrToInt(aszArgv[4 + i]);
            au1Key[(i * 4) + 0] = GET_BYTE(u4Key, 3);
            au1Key[(i * 4) + 1] = GET_BYTE(u4Key, 2);
            au1Key[(i * 4) + 2] = GET_BYTE(u4Key, 1);
            au1Key[(i * 4) + 3] = GET_BYTE(u4Key, 0);
        }
    }
    else if(u4KeyLen == 256)
    {
        if(i4Argc < 12)
        {
            Printf("Not enough key len\n");
            return -1;
        }

        for(i=0; i<8; i++)
        {
            u4Key = StrToInt(aszArgv[4 + i]);
            au1Key[(i * 4) + 0] = GET_BYTE(u4Key, 3);
            au1Key[(i * 4) + 1] = GET_BYTE(u4Key, 2);
            au1Key[(i * 4) + 2] = GET_BYTE(u4Key, 1);
            au1Key[(i * 4) + 3] = GET_BYTE(u4Key, 0);
        }
    }

    if(!FVR_SetAesKey(u1KeyIdx, fgEvenKey, au1Key))
    {
        Printf("Fail\n");
        return -1;
    }

    Printf("Set AES key OK\n");

    return 0;
}


//-----------------------------------------------------------------------------
static void _FVRTimeCallback(FVR_GBL_NOTIFY_CODE_T eCode, 
                             UINT32 u4Data, const void* pvTag)
{
    FVR_GBL_TIMENOTIFY_INFO_T *prTimeInfo;
    UINT32 u4RetSize;
    UINT8 au1Buf[12];
    
    if(eCode != FVR_GBL_NFY_TIME)
    {
        return;
    }

    prTimeInfo = (FVR_GBL_TIMENOTIFY_INFO_T*)u4Data;
    
    Printf("%s, addr: 0x%x, pktcount: 0x%x, size: 0x%x\n", 
                        __FUNCTION__, prTimeInfo->u4Addr, prTimeInfo->u4PktCount, prTimeInfo->u4Size);

    if(!FVR_GetBuffer(prTimeInfo->u4Addr, 8, au1Buf, &u4RetSize))
    {
        Printf("%s:%d, Fail\n", __FUNCTION__, __LINE__);
        return;
    }

    Printf("%s, %02x %02x %02x %02x\n", __FUNCTION__, au1Buf[4], au1Buf[5], au1Buf[6], au1Buf[7]);

    if(!FVR_UnlockBuffer(prTimeInfo->u4Addr, prTimeInfo->u4Size))
    {
        Printf("%s:%d, Fail\n", __FUNCTION__, __LINE__);
        return;
    }

    UNUSED(pvTag);
}

//-----------------------------------------------------------------------------
/** _FVRCmdSetTime
 *  Command: 
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdSetTime(INT32 i4Argc, const CHAR** aszArgv)
{
    FVR_GBL_T rGblPid, rGblPid2;
    BOOL fgEnable;
    UINT32 u4Time = 0;

    if(i4Argc < 2)
    {
        Printf("%s [on|off] [msec]\n", aszArgv[0]);
        return -1;
    }

    if(x_strcmp(aszArgv[1], "on") == 0)
    {
        fgEnable = TRUE;
    }
    else if(x_strcmp(aszArgv[1], "off") == 0)
    {
        fgEnable = FALSE;
    }
    else
    {
        Printf("%s [on|off] [msec]\n", aszArgv[0]);
        return -1;    
    }

    if(i4Argc > 2)
    {
        u4Time = StrToInt(aszArgv[2]);
    }

    if(fgEnable)
    {
        if(!FVR_GetGBL(FVR_GBL_FLAG_TARGET_PKT, &rGblPid2))
        {
            return -1;
        }

        if(rGblPid2.u2TargetPktCount != 0)
        {
            Printf("FVR packet count not 0\n");
            return -1;
        }
        
        rGblPid.u4TimeToNotify = u4Time;
        rGblPid.pfnNotifyFunc = _FVRTimeCallback;
    }
    else
    {
        rGblPid.u4TimeToNotify = 0;
        rGblPid.pfnNotifyFunc = NULL;
    }
    
    if(!FVR_SetGBL(FVR_GBL_FLAG_TIME_INFO | FVR_GBL_FLAG_NOTIFY, &rGblPid))
    {
        return -1;
    }
   
    return 0;
}


//-----------------------------------------------------------------------------
/** _FVRCmdSetAesIV
 *  Command: 
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdSetAesIV(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 au4IV[4];
        
    if(i4Argc != 5)
    {
        Printf("%s [iv_0] [iv_1] [iv_2] [iv_3]\n", aszArgv[0]);
        return -1;
    }

    au4IV[0] = StrToInt(aszArgv[1]);
    au4IV[1] = StrToInt(aszArgv[2]);
    au4IV[2] = StrToInt(aszArgv[3]);
    au4IV[3] = StrToInt(aszArgv[4]);

    // not to use it
    /*if(!FVR_SetAesIV(au4IV))
    {
        Printf("Fail\n");
        return -1;
     }

    Printf("OK\n"); */

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _PVRPlayCmdStart(INT32 i4Argc, const CHAR** aszArgv)
{
    if(!DMX_PVRPlay_Start())
    {
        Printf("Start play fail!\n");
        return -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _PVRPlayCmdStop(INT32 i4Argc, const CHAR** aszArgv)
{
    if(!DMX_PVRPlay_Stop(TRUE))
    {
        Printf("Stop play fail!\n");
        return -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _PVRPlayCmdPause(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PVRPLAY_PTR_T rPtr;
    
    if(!DMX_PVRPlay_Pause(&rPtr))
    {
        Printf("Fail\n");
        return -1;
    }

    Printf("OK\n");

    UNUSED(rPtr);

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _PVRPlayCmdResume(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PVRPLAY_PTR_T rPtr;
    
    if(!DMX_PVRPlay_Resume(&rPtr))
    {
        Printf("Fail\n");
        return -1;
    }

    Printf("OK\n");    

    UNUSED(rPtr);
    
    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _PVRPlayCmdState(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PVR_PLAYSTATE_T rState;
    
    rState = DMX_PVRPlay_GetState();

    if(rState == DMX_PVRPLAY_STATE_PLAYING)
    {
        Printf("Playing...\n");
    }
    else if(rState == DMX_PVRPLAY_STATE_STOP)
    {
        Printf("Stop\n");
    }
    
    Printf("\n");
    
    return 0;
}

//-----------------------------------------------------------------------------
static void _PrintTimeStampUsage(const CHAR* pstr)
{
    Printf("%s [contain] [use] [div]\n", pstr);
    Printf("    contain: on/off\n");
    Printf("    use: on/off\n");
    Printf("    div: 0 ~ 255 (0 means 256)\n");
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _PVRPlayCmdTimeStamp(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PVR_PLAY_T rPlay;
    BOOL fgContain, fgIgnore;
    UINT32 u4Div;
    
    if(i4Argc != 4)
    {
        _PrintTimeStampUsage(aszArgv[0]);
        return -1;
    }

    if(DMX_PVRPlay_GetState() == DMX_PVRPLAY_STATE_PLAYING)
    {
        Printf("Stop playing first!\n");
        return -1;
    }

    if(x_strncmp("on", aszArgv[1], sizeof("on")) == 0)
    {
        fgContain = TRUE;
    }
    else if(x_strncmp("off", aszArgv[1], sizeof("off")) == 0)
    {
        fgContain = FALSE;
    }
    else
    {
        _PrintTimeStampUsage(aszArgv[0]);
        return -1;    
    }

    if(x_strncmp("on", aszArgv[2], sizeof("on")) == 0)
    {
        fgIgnore = FALSE;
    }
    else if(x_strncmp("off", aszArgv[2], sizeof("off")) == 0)
    {
        fgIgnore = TRUE;
    }
    else
    {
        _PrintTimeStampUsage(aszArgv[0]);
        return -1;    
    }

    u4Div = StrToInt(aszArgv[3]);

    rPlay.fgContainTimeStamp = fgContain;
    rPlay.fgIgnoreTimeStamp = fgIgnore;
    rPlay.u2TimeStampFreqDiv = (UINT16)(u4Div & 0xFFFF);
    
    DMX_PVRPlay_Set(PVRPLAY_FLAGS_TIMESTAMP, &rPlay);
    
    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _PVRPlayCmdSetPid(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PID_T rPid;
    UINT8 u1Pidx;

    if(i4Argc != 2)
    {
        Printf("%s [pidx]\n", aszArgv[0]);
        return -1;
    }

    u1Pidx = StrToInt(aszArgv[1]);

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    rPid.u1TsIndex = 2;     // For PVR playback stream

    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_TS_INDEX, &rPid))
    {
        return FALSE;
    }

    Printf("OK\n");    

    return 0;
}

#ifdef PVR_LOOPBACK_TEST
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdLoopbackInit(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4BufSize, u4RecPktCount, u4PlayThreshold;

    if(i4Argc !=  4)
    {
        Printf("%s [bufsize] [r_pktcount] [p_threshold]\n", aszArgv[0]);
        return 0;        
    }

    u4BufSize = StrToInt(aszArgv[1]);
    u4RecPktCount = StrToInt(aszArgv[2]);
    u4PlayThreshold = StrToInt(aszArgv[3]);
    
    if(!PVRM_Loopback_Init(u4BufSize, u4RecPktCount, u4PlayThreshold))
    {
        Printf("Fail!\n");
        return -1;
    }

    Printf("OK\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdLoopbackStart(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Distance;

    if(i4Argc != 2)
    {
        Printf("%s [distance]\n", aszArgv[0]);
        return 0;
    }

    u4Distance = StrToInt(aszArgv[1]);

    if(!PVRM_Loopback_Start(u4Distance))
    {
        Printf("Fail!\n");
        return -1;
    }

    Printf("OK\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdLoopbackStop(INT32 i4Argc, const CHAR** aszArgv)
{
    if(!PVRM_Loopback_Stop())
    {
        Printf("Fail\n");
        return -1;
    }

    Printf("OK\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
/*
static INT32 _FVRCmdLoopbackLiveTest(INT32 i4Argc, const CHAR** aszArgv)
{
    if(!PVRM_Loopback_Live(_u1TsIndex))
    {
        Printf("Fail\n");
        return -1;
    }

    Printf("OK\n");

    return 0;
}
*/

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdLoopbackQuery(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4DataSize, u4Max, u4Min;
    
    PVRM_Loopback_GetDataSize(&u4DataSize, &u4Max, &u4Min);
    
    Printf("DataSize: 0x%x, max:0x%x, min:0x%x\n", u4DataSize, u4Max, u4Min);

    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdLoopbackCurrentChannel(INT32 i4Argc, const CHAR** aszArgv)
{
    // Show all counters of active PIDs
    DMX_PID_T rPid;
    UINT32 i;
    
    INT32 i4VideoPid = -1;    
    INT32 i4AudioPid = -1;
    INT32 i4PcrPid = -1;
    INT32 i4VideoPidIdx = -1;
    INT32 i4AudioPidIdx = -1;
    INT32 i4PcrPidIdx = -1;
    UINT8 u1AvaliablePidIdx[3];
    UINT8 u1AvaliableCount = 0;
    static CHAR szMsg[256];

    // Get current channel PID
    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        if (DMX_GetPid(i, DMX_PID_FLAG_ALL, &rPid))
        {
            if (rPid.fgEnable)
            {
                // PID type
                switch (rPid.ePidType)
                {
                case DMX_PID_TYPE_NONE:
                    i4PcrPid = rPid.u2Pid;
                    i4PcrPidIdx = i;
                    break;

                case DMX_PID_TYPE_ES_VIDEO:
                    i4VideoPid = rPid.u2Pid;
                    i4VideoPidIdx = i;
                    break;

                case DMX_PID_TYPE_ES_AUDIO:
                    i4AudioPid = rPid.u2Pid;
                    i4AudioPidIdx = i;
                    break;

                default:
                    break;
                }                
            }
            else 
            {
                if(u1AvaliableCount < 3) 
                {
                    u1AvaliablePidIdx[u1AvaliableCount] = i;
                    u1AvaliableCount++;
                }
            }
        }
    }

    if( (i4VideoPidIdx == -1) &&
        (i4AudioPidIdx == -1) &&
        (i4PcrPidIdx == -1)) 
    {
        Printf("Play a DTV channel first!\n");
        return -1;
    }
    else 
    {
        Printf("Current Channel: V(%d): %d, A(%d): %d, PCR(%d): %d\n",
            i4VideoPidIdx, i4VideoPid, i4AudioPidIdx, i4AudioPid, 
            i4PcrPidIdx, i4PcrPid);
    }
    
    CLI_Parser("d.st 2");
    CLI_Parser("pvr.i");
    CLI_Parser("pvr.p.ts on on 8");

    if(i4VideoPidIdx != -1) 
    {
        x_sprintf(szMsg, "pvr.p.sp %d", i4VideoPidIdx);
        CLI_Parser(szMsg);
        x_sprintf(szMsg, "pvr.r.sp %d %d off off off 0", u1AvaliablePidIdx[0], i4VideoPid);
        CLI_Parser(szMsg);
    }

    if(i4AudioPidIdx != -1) 
    {
        x_sprintf(szMsg, "pvr.p.sp %d", i4AudioPidIdx);
        CLI_Parser(szMsg);
        x_sprintf(szMsg, "pvr.r.sp %d %d off off off 0", u1AvaliablePidIdx[1], i4AudioPid);
        CLI_Parser(szMsg);
    }

    if(i4PcrPidIdx != -1) 
    {
        x_sprintf(szMsg, "pvr.p.sp %d", i4PcrPidIdx);
        CLI_Parser(szMsg);
        x_sprintf(szMsg, "pvr.r.sp %d %d off off off 0", u1AvaliablePidIdx[2], i4PcrPid);
        CLI_Parser(szMsg);        
    }
       
    CLI_Parser("pvr.r.ts on 8");  
    CLI_Parser("pvr.lp.init 0x500000 0x200 0x2000");      
    CLI_Parser("pvr.lp.start 0x100000");  

    Printf("Finish PVR loopback current channel setup\n");
    return 0;
}
    
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdLoopbackRecTest(INT32 i4Argc, const CHAR** aszArgv)
{
    FVR_GBL_T rGBL;
    UINT32 u4Addr, u4Size, u4BufSize;

    if(i4Argc != 2)
    {
        Printf("%s [bufsize]\n", aszArgv[0]);
        return -1;
    }

    u4BufSize = StrToInt(aszArgv[1]);
    
    if(!PVRM_Loopback_Record(u4BufSize))
    {
        Printf("Fail\n");
        return -1;
    }
    
    if(!FVR_GetGBL(FVR_GBL_FLAG_BUFFER, &rGBL))
    {
        return FALSE;
    }

    u4Addr = rGBL.u4Rp;
    u4Size = rGBL.u4Wp - rGBL.u4Rp;

    Printf("Addr:0x%x, size:0x%x\n", u4Addr, u4Size);
    Printf("OK\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdLoopbackPlayTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Addr, u4Size;
    BOOL fgSingle;

    if(i4Argc != 4)
    {
        Printf("%s [addr] [size] [single]\n", aszArgv[0]);
        Printf("   single: on | off\n");
        return -1;
    }

    u4Addr = StrToInt(aszArgv[1]);
    u4Size = StrToInt(aszArgv[2]);

    if(x_strcmp(aszArgv[3], "on") == 0)
    {
        fgSingle = TRUE;
    }
    else if(x_strcmp(aszArgv[3], "off") == 0)
    {
        fgSingle = FALSE;
    }
    else
    {
        Printf("%s [addr] [size] [single]\n", aszArgv[0]);
        Printf("   single: on | off\n");
        return -1;
    }

    if(fgSingle)
    {
        if(!PVRM_Loopback_SinglePlay(u4Addr, u4Size))
        {
            return FALSE;
        }
    }
    else
    {
        if(!PVRM_Loopback_StreamPlay(u4Addr, u4Size))
        {
            return FALSE;
        }
    }
    
    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdLoopbackErrTest(INT32 i4Argc, const CHAR** aszArgv)
{
    if(!PVRM_Loopback_ErrorTest())
    {
        Printf("Fail\n");
        return -1;
    }

    Printf("OK\n");
    
    return 0;
}

EXTERN BOOL PVRM_Loopback_RecTimerTest(UINT32 u4BufSize);
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdLoopbackTimerTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4BufSize;
    
    if(i4Argc != 2)
    {
        Printf("%s [bufsize]\n", aszArgv[0]);
        return -1;
    }

    u4BufSize = StrToInt(aszArgv[1]);
    
    if(!PVRM_Loopback_RecTimerTest(u4BufSize))
    {
        Printf("Fail\n");
        return -1;
    }

    Printf("OK\n");
    
    return 0;
}


EXTERN BOOL PVRM_Loopback_PlayResetTest(UINT32 u4Loop, UINT32 u4BufAddr, UINT32 u4BufSize);
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdLoopbackResetTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Loop, u4Addr, u4Size;

    if(i4Argc != 4)
    {
        Printf("%s loop addr size\n", aszArgv[0]);
        return -1;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u4Addr = StrToInt(aszArgv[2]);
    u4Size = StrToInt(aszArgv[3]);

    if(!PVRM_Loopback_PlayResetTest(u4Loop, u4Addr, u4Size))
    {
        Printf("Test fail\n");
        return -1;
    }

    Printf("Test OK\n");
    
    return 0;
}


EXTERN BOOL PVRM_Loopback_IgnoreTimeStampTest(UINT32 u4BufAddr, UINT32 u4BufSize);
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _FVRCmdLoopbackIgnoreTimeStampTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Loop, u4Addr, u4Size;
    UINT32 i;

    if(i4Argc != 4)
    {
        Printf("%s loop addr size\n", aszArgv[0]);
        return -1;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u4Addr = StrToInt(aszArgv[2]);
    u4Size = StrToInt(aszArgv[3]);

    for(i=0; i<u4Loop; i++)
    {
        if(!PVRM_Loopback_IgnoreTimeStampTest(u4Addr, u4Size))
        {
            Printf("Test fail\n");
            return -1;
        }
    }

    Printf("Test OK\n");

    return 0;
}

#endif

//=====================================================================
// CLI command table
//=====================================================================

static CLI_EXEC_T _arPVRPlayCmdTbl[] =
{
    // Playback property
    {
        "state", "state", _PVRPlayCmdState, NULL, "state", CLI_SUPERVISOR
    },

    {
        "timestamp", "ts", _PVRPlayCmdTimeStamp, NULL, "ts [contain] [ignore] [div]", CLI_SUPERVISOR
    },
    
    {
        "setpid", "sp", _PVRPlayCmdSetPid, NULL, "sp [pidx]", CLI_SUPERVISOR
    },

    {
        "start", "start", _PVRPlayCmdStart, NULL, "start", CLI_SUPERVISOR
    },

    {
        "stop", "stop", _PVRPlayCmdStop, NULL, "stop", CLI_SUPERVISOR
    },

    {
        "pause", "pause", _PVRPlayCmdPause, NULL, "pause", CLI_SUPERVISOR
    },

    {
        "resume", "resume", _PVRPlayCmdResume, NULL, "resume", CLI_SUPERVISOR
    },

    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }
    
};


//=====================================================================
// CLI command table
//=====================================================================
static CLI_EXEC_T _arPVRRecordCmdTbl[] =
{
    {
        "setvideotype", "svt", _FVRCmdSetVideoType, NULL, "", CLI_SUPERVISOR
    },
    
    {
        "fvrpid", "sp", _FVRCmdSetPid, NULL, "sp [fvr_pidx] [pid_num] ...", CLI_SUPERVISOR
    },

    {
        "getfvrpid", "gp", _FVRCmdGetPid, NULL, "gp [pidx]", CLI_SUPERVISOR
    },

    {
        "freefvrpid", "fpp", _FVRCmdFreeFVRPid, NULL, "fpp [pidx]", CLI_SUPERVISOR
    },

    {
        "timestamp", "ts", _FVRCmdTimeStamp, NULL, "ts [on/off] [clkdiv] [mask] [init_counter]", CLI_SUPERVISOR
    },

    {
        "pktcount", "pc", _FVRCmdPktCount, NULL, "pc [count]", CLI_SUPERVISOR
    },

    {
        "bypass", "bypass", _FVRCmdByPass, NULL, "bypass [ts_idx] [val]", CLI_SUPERVISOR
    },

    {
        "setaesmode", "saesm", _FVRCmdSetAESMode, NULL, "saesm [pidx] [cbc/ebc] [e/o/b]", CLI_SUPERVISOR
    },

    {
        "setkeyidx", "skidx", _FVRCmdSetKeyIdx, NULL, "skidx [pidx] [key_idx] [key_len]", CLI_SUPERVISOR
    },

    {
        "setaesiv", "saesiv", _FVRCmdSetAesIV, NULL, "saesiv [iv_0] [iv_1] [iv_2] [iv_3]", CLI_SUPERVISOR
    },
        
    {
        "setaeskey", "saesk", _FVRCmdSetAESKey, NULL, "saesk", CLI_SUPERVISOR
    },

    {
        "settimehandler", "sth", _FVRCmdSetTime, NULL, "sth [on|off] [msec]", CLI_SUPERVISOR
    },
    
    {
        "picsearchtest", "pct", _FVRCmdPicSearchTest, NULL, "pct", CLI_SUPERVISOR
    },
    
    {
        "start", "start", _FVRCmdStart, NULL, "start", CLI_SUPERVISOR
    },

    {
        "pause", "pause", _FVRCmdPause, NULL, "pause", CLI_SUPERVISOR
    },

    {
        "stop", "stop", _FVRCmdStop, NULL, "stop", CLI_SUPERVISOR
    },

    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }

};


//=====================================================================
// CLI command table
//=====================================================================
#ifdef PVR_LOOPBACK_TEST
static CLI_EXEC_T _arPVRLoopbackCmdTbl[] =
{
    {
        "init", "i", _FVRCmdLoopbackInit, NULL, "init", CLI_SUPERVISOR
    },
    
    {
        "start", "start", _FVRCmdLoopbackStart, NULL, "start", CLI_SUPERVISOR
    },

    {
        "stop", "stop", _FVRCmdLoopbackStop, NULL, "stop", CLI_SUPERVISOR
    },

//    {
//        "test", "test", _FVRCmdLoopbackLiveTest, NULL, "test", CLI_SUPERVISOR
//    },

    {
        "rectest", "rtest", _FVRCmdLoopbackRecTest, NULL, "rtest", CLI_SUPERVISOR
    },

    {
        "playtest", "ptest", _FVRCmdLoopbackPlayTest, NULL, "ptest", CLI_SUPERVISOR
    },

    {
        "errtest", "errtest", _FVRCmdLoopbackErrTest, NULL, "errtest", CLI_SUPERVISOR
    },

    {
        "timertest", "ttest", _FVRCmdLoopbackTimerTest, NULL, "ttest", CLI_SUPERVISOR
    },

    {
        "resettest", "resettest", _FVRCmdLoopbackResetTest, NULL, "resettest", CLI_SUPERVISOR
    },

    {
        "ignoretest", "igtest", _FVRCmdLoopbackIgnoreTimeStampTest, NULL, "igtest", CLI_SUPERVISOR
    },

    {
        "query", "q", _FVRCmdLoopbackQuery, NULL, "query", CLI_SUPERVISOR
    },

    {
        "currentchannel", "c", _FVRCmdLoopbackCurrentChannel, NULL, "currentchannel", CLI_SUPERVISOR
    },
    
    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }    
};
#endif

//=====================================================================
// CLI command table
//=====================================================================
static CLI_EXEC_T _arDmxPVRCmdTbl[] =
{
    {
        "init", "i", _PVRCmdInit, NULL, "init", CLI_SUPERVISOR
    },

    {
        "query", "q", _FVRCmdQuery, NULL, "query", CLI_SUPERVISOR
    },

    {
        "settuner", "st", _FVRCmdSetTuner, NULL, "st [tuner_idx]", CLI_SUPERVISOR
    },

    {
        "record", "r", NULL, _arPVRRecordCmdTbl, "Record driver commands", CLI_SUPERVISOR
    },

    {
        "play", "p", NULL, _arPVRPlayCmdTbl, "Play driver commands", CLI_SUPERVISOR
    },
#ifdef PVR_LOOPBACK_TEST
    {
        "loopback", "lp", NULL, _arPVRLoopbackCmdTbl, "Loopback commands", CLI_SUPERVISOR
    },
#endif

LINT_SAVE_AND_DISABLE
    CLIMOD_DEBUG_CLIENTRY(FVR),
LINT_RESTORE


    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }

};


LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(Fvr)
{
    "pvr", "pvr", NULL, _arDmxPVRCmdTbl, "PVR commands", CLI_GUEST

};
LINT_RESTORE
