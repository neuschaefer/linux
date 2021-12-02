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
 *         The CLI commands for dhrystone
 *
 *---------------------------------------------------------------------------*/

#include "x_drv_cli.h"
#include "x_serial.h"
#include "x_printf.h"
#include "x_os.h"
#include "x_stl_lib.h"
#include "x_assert.h"
#include "drv_common.h"

//===========================================================================


//---------------------------------------------------------------------

extern void DhrystoneTest(INT32 i4Runs);

static VOID _DhrystoneThread(VOID* pvArg)
{
    INT32 i4Runs = *(INT32*)pvArg;

    DhrystoneTest(i4Runs);
}

static INT32 _CmdDhrystone(INT32 i4Argc, const CHAR** aszArgv)
{
    HANDLE_T hThread;
    UINT8 u1Priority;
    INT32 i4Runs;

    // Check arguments
    if (i4Argc != 3)
    {
        Printf("Usage: dhrystone [thread_priority] [number_of_runs]\n");
        return -1;
    }

    // Parser arguments
    u1Priority = (UINT8)StrToInt(aszArgv[1]);
    i4Runs = (INT32)StrToInt(aszArgv[2]);

    // Create thread
    if (x_thread_create(&hThread, DHRYSTONE_THREAD_NAME, DHRYSTONE_STACK_SIZE,
        u1Priority, _DhrystoneThread, sizeof(i4Runs), &i4Runs) != OSR_OK)
    {
        Printf("Fail to create dhrystone thread\n");
        return -1;
    }

    return 0;
}


static INT32 _CmdDhrystoneLoop(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i = 0;
    
    while(1)
    {
        Printf("-----------------------------------\n");
        Printf("Loop: %d\n\n", i);
        DhrystoneTest(100000);
        i++;
    }
    
    //return 0;
}


//===========================================================================

static CLI_EXEC_T _arDhrystoneCmdTbl[] =
{
    {
        "dhrystone",
        "d",
        _CmdDhrystone,
        NULL,
        "Dhrystone test",
        CLI_GUEST
    },

    {
        "looptest",
        "l",
        _CmdDhrystoneLoop,
        NULL,
        "Dhrystone loop test",
        CLI_GUEST
    },
    

 	// last cli command record, NULL
 	{
		NULL, NULL, NULL, NULL, NULL, CLI_GUEST
	}
};


//------------------------------------------------------------------------------
/** CLI entry point
 */
//------------------------------------------------------------------------------
CLI_MAIN_COMMAND_ITEM(dhrystone)
{
	"dhrystone",
	NULL,
	NULL,
	_arDhrystoneCmdTbl,
	"dhrystone test",
	CLI_GUEST
};
