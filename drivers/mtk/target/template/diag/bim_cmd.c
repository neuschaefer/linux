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
 * Description:
 *      Memory test commands
 *
 *---------------------------------------------------------------------------*/

#include "x_printf.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif
#include "x_stl_lib.h"
#include "x_assert.h"
#include "x_ckgen.h"
#include "x_bim.h"

//=====================================================================
// extern functions
//=====================================================================
EXTERN INT32 CacheMeterQuery(UINT32 *pu4IReq, UINT32 *pu4IMiss, UINT32 *pu4IPenalty, 
        UINT32 *pu4DReq, UINT32 *pu4DMiss, UINT32 *pu4DPenalty);
EXTERN INT32 CmdEfusePowerPinOn(INT32 i4Argc, const CHAR** aszArgv);
EXTERN INT32 CmdEfusePowerPinOff(INT32 i4Argc, const CHAR** aszArgv);
EXTERN INT32 CmdEfuseSecureID(INT32 i4Argc, const CHAR** aszArgv);
EXTERN INT32 CmdEfuseSecureData(INT32 i4Argc, const CHAR** aszArgv);
EXTERN INT32 CmdEfuseDump(INT32 i4Argc, const CHAR** aszArgv);
/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Succeed
 *      FALSE: Fail
 *---------------------------------------------------------------------------*/
static void _ShowCacheMeterUsage(void)
{
    Printf("cm");
}


/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Succeed
 *      FALSE: Fail
 *---------------------------------------------------------------------------*/
static INT32 _CmdCacheMeter(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4IReq, u4IMiss, u4IPenalty, u4DReq, u4DMiss, u4DPenalty;
        
    if (i4Argc != 1)
    {
        _ShowCacheMeterUsage();
        return 0;
    }

    Printf("Please wait for calculation....\n\n");
           
    CacheMeterQuery(&u4IReq, &u4IMiss, &u4IPenalty, &u4DReq, &u4DMiss, &u4DPenalty);

    Printf("I-Cache Request = 0x%x\nI-Cache Miss    = 0x%x\nI-Cache Penalty = 0x%x\n", 
            u4IReq, u4IMiss, u4IPenalty );
    Printf("D-Cache Request = 0x%x\nD-Cache Miss    = 0x%x\nD-Cache Penalty = 0x%x\n", 
            u4DReq, u4DMiss, u4DPenalty);

    return 0;
}

void _CmdDisplayTimeItem(UINT32 u4Timer, CHAR *szString)
{
    UINT32 u4Val;

    u4Val = ((~u4Timer)/(GET_XTAL_CLK()/1000000));  // us time.
    Printf("0x%08x | %6d.%03d ms - %s\n", u4Timer, (u4Val / 1000), (u4Val % 1000), szString);
}

static INT32 _CmdBootTime(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 *pu4Time;
    UINT32 i, u4Size;
    TIME_STAMP_T *prTimeStamp;

    pu4Time = (UINT32 *)0x200080e0;
    _CmdDisplayTimeItem(*pu4Time, "Before loader decompress");
    pu4Time++;
    _CmdDisplayTimeItem(*pu4Time, "After loader decompress");
    pu4Time++;
    _CmdDisplayTimeItem(*pu4Time, "Before image decompress");
    pu4Time++;
    _CmdDisplayTimeItem(*pu4Time, "After image decompress");
    pu4Time++;
    _CmdDisplayTimeItem(*pu4Time, "Before x_drv_init()");
    pu4Time++;
    _CmdDisplayTimeItem(*pu4Time, "After x_drv_init()");

    prTimeStamp = x_os_drv_get_timestamp(&u4Size);
    for (i=0; i<u4Size; i++)
    {
        _CmdDisplayTimeItem(prTimeStamp[i].u4TimeStamp, prTimeStamp[i].szString);
    }
    return 0;
}

static INT32 _CmdLzhsDec(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4SrcAddr, u4DestAddr, u4Len;
    UINT8 u1Checksum;

    u4SrcAddr = StrToInt(aszArgv[1]);
    u4DestAddr = StrToInt(aszArgv[2]);
    u4Len = StrToInt(aszArgv[3]);
    u1Checksum = StrToInt(aszArgv[4]);
    if (LZHS_Dec(u4SrcAddr, u4DestAddr, u4Len, &u1Checksum)!=0)
    {
        Printf("LZHS decode failed\n");
        return 1;
    }
    Printf("LZHS decode ok\n");
    return 0;
}

//=====================================================================

/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Succeed
 *      FALSE: Fail
 *---------------------------------------------------------------------------*/

static CLI_EXEC_T _arEFuseCmdTbl[] =
{
    { "PowerPinOn", "pon", CmdEfusePowerPinOn, NULL, "Efuse Power Pin On", CLI_SUPERVISOR },
    { "PowerPinOff", "poff", CmdEfusePowerPinOff, NULL, "Efuse Power Pin Off", CLI_SUPERVISOR },
    { "ReadSID", "rsid", CmdEfuseSecureID, NULL, "Read Security ID", CLI_SUPERVISOR },
    { "ReadSData", "rsda", CmdEfuseSecureData, NULL, "Read Security Data", CLI_SUPERVISOR }, 
    { "DumpData", "dump", CmdEfuseDump, NULL, "Dump Efuse", CLI_SUPERVISOR }, 
    { NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR }
};

static CLI_EXEC_T _arBIMCmdTbl[] =
{
    {
        "CacheMeter",
        "cm",
        _CmdCacheMeter,
        NULL,
        "Cache Meter",
        CLI_GUEST
    },
    {
        "BootTime",
        "bt",
        _CmdBootTime,
        NULL,
        "Boot time parsing",
        CLI_GUEST
    },
    { 
        "lzhs", 
        "lzhs", 
        _CmdLzhsDec,
        NULL,
        "lzhs [image src] [target mem] [dest size] [checksum]",
        CLI_GUEST
    },    
    { 
        "efuse", 
        "ef", 
        NULL, 
        _arEFuseCmdTbl, 
        "Program stream commands", 
        CLI_SUPERVISOR 
    },    
 	// last cli command record, NULL
 	{
		NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
	}
};

/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Succeed
 *      FALSE: Fail
 *---------------------------------------------------------------------------*/
CLI_MAIN_COMMAND_ITEM(BIM)
{
	"bim",
	NULL,
	NULL,
	_arBIMCmdTbl,
	"BIM module test",
	CLI_GUEST
};

