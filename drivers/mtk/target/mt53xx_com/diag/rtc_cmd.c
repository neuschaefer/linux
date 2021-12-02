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
 * $RCSfile: rtc_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_cmd.c
 *  Demux CLI commands
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "rtc_if.h"

#include "x_drv_cli.h"
#include "x_stl_lib.h"
#include "x_printf.h"
#include "x_pdwnc.h"


//---------------------------------------------------------------------
// LOG macro related

// #define DEFINE_IS_LOG	CLI_IsLog
#define DEFINE_IS_LOG CLI_IsLog
#include "x_debug.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

CLIMOD_DEBUG_FUNCTIONS(RTC)

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
EXTERN void PDWNC_SetupPowerDown(void); 

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static INT32 i4CmdInit(INT32 i4Argc, const CHAR** szArgv)
{
    RTC_Init();

    return 0;
}


static INT32 i4CmdSetTimeDate(INT32 i4Argc, const CHAR** szArgv)
{
    UINT64 u8Utc;
    RTC_T rRtc;
    if(i4Argc < 7)
    {
        return -1024;
    }
    rRtc.u1Year = StrToInt(szArgv[1]);    
    rRtc.u1Month = StrToInt(szArgv[2]);
    rRtc.u1Day = StrToInt(szArgv[3]);
    rRtc.u1Hour = StrToInt(szArgv[4]);
    rRtc.u1Minute = StrToInt(szArgv[5]);    
    rRtc.u1Second = StrToInt(szArgv[6]);
    RTC_RtcToUtc(&u8Utc, &rRtc);
    RTC_SetTimeDate(&u8Utc);

    return 0;
}

static INT32 i4CmdGetTimeDate(INT32 i4Argc, const CHAR** szArgv)
{
    UINT64 u8Utc;
    RTC_T rRtc;
    RTC_GetTimeDate(&u8Utc);
    RTC_UtcToRtc(&rRtc, u8Utc);
    Printf("[RTC] %2d/%2d/%2d %2d:%2d:%2d\n", 
    	rRtc.u1Year,
    	rRtc.u1Month,
   	rRtc.u1Day,
    	rRtc.u1Hour,
    	rRtc.u1Minute,
    	rRtc.u1Second
    	);

    return 0;
}

static void _RTC_TestISR(UINT16 u2Val)
{

       UINT64 u8Utc;
    RTC_T rRtc;
    UNUSED(u2Val);
    RTC_GetTimeDate(&u8Utc);
    RTC_UtcToRtc(&rRtc, u8Utc);
    Printf("[RTC Alert] %2d/%2d/%2d %2d:%2d:%2d\n", 
    	rRtc.u1Year,
    	rRtc.u1Month,
   	rRtc.u1Day,
    	rRtc.u1Hour,
    	rRtc.u1Minute,
    	rRtc.u1Second
    	);
}
    

static INT32 i4CmdSetAlertTime(INT32 i4Argc, const CHAR** szArgv)
{
    UINT64 u8Utc;
    RTC_T rRtc;
  //INT32 i4Val;
    void (* pfnOldIsr)(UINT16);
    
    if(i4Argc < 7)
    {
        return -1024;
    }
    rRtc.u1Year = StrToInt(szArgv[1]);
    rRtc.u1Month = StrToInt(szArgv[2]);
    rRtc.u1Day = StrToInt(szArgv[3]);
    rRtc.u1Hour = StrToInt(szArgv[4]);
    rRtc.u1Minute = StrToInt(szArgv[5]);
    rRtc.u1Second = StrToInt(szArgv[6]);
    RTC_RtcToUtc(&u8Utc, &rRtc);
    PDWNC_RegIsr(PINTNO_RTC, _RTC_TestISR,  &pfnOldIsr);
    RTC_SetAlertTime(u8Utc);
    return 0;
}


static INT32 i4CmdSetWakeupTime(INT32 i4Argc, const CHAR** szArgv)
{
    UINT64 u8Utc;
    RTC_T rRtc;
  //INT32 i4Val;
    //void (* pfnOldIsr)(UINT16);
    
    if(i4Argc < 7)
    {
        return -1024;
    }
    rRtc.u1Year = StrToInt(szArgv[1]);
    rRtc.u1Month = StrToInt(szArgv[2]);
    rRtc.u1Day = StrToInt(szArgv[3]);
    rRtc.u1Hour = StrToInt(szArgv[4]);
    rRtc.u1Minute = StrToInt(szArgv[5]);
    rRtc.u1Second = StrToInt(szArgv[6]);
    RTC_RtcToUtc(&u8Utc, &rRtc);
    RTC_SetAlertTime(u8Utc);
    vIO32Write4B(PDWNC_WAKEN,  WAK_RTC);//   PDWNC_WRITE32(REG_RW_PDWAKE_EN, u4PowerDownEn);   
    PDWNC_SetupPowerDown();    

    return 0;
}


static INT32 i4CmdGetAlertTime(INT32 i4Argc, const CHAR** szArgv)
{

    UINT64 u8Utc;
    RTC_T rRtc;
    RTC_GetAlertTime(&u8Utc);
    RTC_UtcToRtc(&rRtc, u8Utc);
    Printf("[RTC] %2d/%2d/%2d %2d:%2d:%2d\n", 
    	rRtc.u1Year,
    	rRtc.u1Month,
   	rRtc.u1Day,
    	rRtc.u1Hour,
    	rRtc.u1Minute,
    	rRtc.u1Second
    	);

    return 0;
}

/*
static INT32 i4CmdWriteReg(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 u1Addr;
    UINT8 u1Val;

    if(i4Argc != 3)
    {
        return -1024;
    }
    u1Addr = StrToInt(szArgv[1]);
    u1Val = StrToInt(szArgv[2]);    
    RTC_ISL1208_Write(u1Addr, u1Val);
    return 0;
}
*/

//=====================================================================
// CLI command table
//=====================================================================

static CLI_EXEC_T _arRtcCmdTbl[] =
{
    // LOG macro support    
	CLIMOD_DEBUG_CLIENTRY(RTC),
	{
		"init", "i", i4CmdInit, NULL, "", CLI_SUPERVISOR
	},
	{
		"settime", "st", i4CmdSetTimeDate, NULL, "", CLI_SUPERVISOR
	},
	{
		"gettime", "gt", i4CmdGetTimeDate, NULL, "", CLI_SUPERVISOR
	},
	{
		"setAlert", "sa", i4CmdSetAlertTime, NULL, "", CLI_SUPERVISOR
	},
	{
		"setWakeup", "sw", i4CmdSetWakeupTime, NULL, "", CLI_SUPERVISOR
	},
	{
		"getAlert", "ga", i4CmdGetAlertTime, NULL, "", CLI_SUPERVISOR
	},
	/*
	{
		"writeReg", "wr", i4CmdWriteReg, NULL, "", CLI_SUPERVISOR
	},
	*/
	{
		NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
	}   
};


CLI_MAIN_COMMAND_ITEM(Rtc)
{

    "rtc", "rtc", NULL, _arRtcCmdTbl, "RTC commands", CLI_GUEST


};

