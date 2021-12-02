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
 * $RCSfile: lzhs_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file serialflash_cmd.c
 *  This file is the implementation of serial flash CLI definition.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_bim.h"

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_stl_lib.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_timer.h"

#define DEFINE_IS_LOG        LZHS_IsLog
#include "x_debug.h"
#include "x_util.h"

LINT_EXT_HEADER_END


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

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** _LZHS_SetIsrCmd
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  0 	SUCCESS.
 *  @retval  1	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _LZHS_SetIsrCmd(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgSet;
    
    if(i4Argc != 2)
    {
        Printf("lzhs.setisr [TRUE/FALSE]");
        return 0;
    }

    fgSet = FALSE;
    if((x_strcmp("TRUE", szArgv[1])==0) || (x_strcmp("true", szArgv[1])==0))
    {
        fgSet = TRUE;
    }
    else if((x_strcmp("FALSE", szArgv[1])==0) || (x_strcmp("false", szArgv[1])==0))
    {
        fgSet = FALSE;
    }
    else
    {
        Printf("lzhs.setisr [TRUE/FALSE]");
        return 0;
    }

    LZHS_SetIsrEnable(fgSet);

    return 0;
}


//-------------------------------------------------------------------------
/** _LZHS_LoopCmd
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  0 	SUCCESS.
 *  @retval  1	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _LZHS_LoopCmd(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 i, u4Src, u4Dest, u4Len, u4Times;
	UINT8 u1CheckSum, u1CheckSumGold;
	
	if(i4Argc != 5)
	{
		Printf("lzhs.l [times] [src_addr] [dest_addr] [len]\n");
		return -1;
	}

	u4Times = StrToInt(szArgv[1]);
    u4Src = StrToInt(szArgv[2]);
    u4Dest = StrToInt(szArgv[3]);
    u4Len = StrToInt(szArgv[4]);

	if(LZHS_Dec(u4Src, u4Dest, u4Len, &u1CheckSumGold) == 1)
	{
		Printf("LZHS hardware decode fail at loop 0!!\n");
		return -1;
	}

    for(i=0; i<(u4Times-1); i++)
    {
        Printf("LZHS decode loop: %d\n", i + 1);
        
    	if(LZHS_Dec(u4Src, u4Dest, u4Len, &u1CheckSum) == 1)
    	{
    		Printf("Fail!!\n");
    		return -1;
    	}

    	if(u1CheckSum != u1CheckSumGold)
    	{
    	    Printf("Check compare fail! 0x%X != 0x%X\n",u1CheckSum, u1CheckSumGold);
            return -1;
    	}
    }

    Printf("Loop test OK!\n");
    

    return 0;
}



//-------------------------------------------------------------------------
/** _LZHS_DecodeCmd
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  0 	SUCCESS.
 *  @retval  1	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _LZHS_DecodeCmd(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4Src, u4Dest, u4Len;
	UINT8 u1CheckSum;
	
	if(i4Argc != 4)
	{
		Printf("lzhs.hd [src_addr] [dest_addr] [len]\n");
		return -1;
	}
	
    u4Src = StrToInt(szArgv[1]);
    u4Dest = StrToInt(szArgv[2]);
    u4Len = StrToInt(szArgv[3]);
    
	if(LZHS_Dec(u4Src, u4Dest, u4Len, &u1CheckSum) == 1)
	{
		Printf("LZHS hardware decode fail!!\n");
		return -1;
	}

	Printf("LZHS hardware decode OK!!\n");

	return 0;
}


//-------------------------------------------------------------------------
/** _LZHS_Diag
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  0 	SUCCESS.
 *  @retval  1	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _LZHS_Diag(INT32 i4Argc, const CHAR ** szArgv)
{
	// Register test
	
	return 0;
}



/******************************************************************************
* Declare the debug on/off/level functions
******************************************************************************/
//CLIMOD_DEBUG_FUNCTIONS(LZHS)

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
static CLI_EXEC_T arLZHSCmdTbl [] =
{
    //DECLARE_CMD(_LZHS_Init, init, init, "lzhs.init"),

//	CLIMOD_DEBUG_CLIENTRY(SFLASH),

	DECLARE_CMD(_LZHS_DecodeCmd, decode, d, "lzhs.d [src_addr] [dest_addr] [len]"),
	DECLARE_CMD(_LZHS_SetIsrCmd, setisr, setisr, "lzhs.setisr [TRUE/FALSE]"),
	DECLARE_CMD(_LZHS_LoopCmd, loop, l, "lzhs.l [times] [checksum]"),
	
	DECLARE_CMD(_LZHS_Diag, diag, diag, "lzhs.diag"),
	
    DECLARE_END_ITEM(),
};


/// Serial Flash CLI command structure.
CLI_MAIN_COMMAND_ITEM(LZHS)
{
    "lzhs", NULL, NULL, arLZHSCmdTbl, "LZHS decode command", CLI_SUPERVISOR
};


//-------------------------------------------------------------------------
/** GetLZHSCmdTbl
 *  get LZHS CLI function list.
 *  @param  VOID
 *  @return  CLI_EXEC_T pointer.
 */
//-------------------------------------------------------------------------
#if 0
#ifdef __MW_CLI_DEF__
CLI_EXEC_T *GetLZHSMwCmdTbl(VOID)
#else
CLI_EXEC_T *GetLZHSCmdTbl(VOID)
#endif
{
    return &_rLZHSModCmdTbl;
}
#endif

