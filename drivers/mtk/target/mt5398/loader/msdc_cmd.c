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
 * $RCSfile: msdc_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file msdc_cmd.c
 *  msdc_cmd.c provide cli for msdc module.
 */

#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_stl_lib.h"
#include "x_timer.h"
#include "x_os.h"
#include "x_drv_cli.h"
#include "x_util.h"
#include "x_gpio.h"
#include "x_ckgen.h"
#define DEFINE_IS_LOG(level, fmt...) Printf(fmt)
#include "x_debug.h"
#include "x_rand.h"
#include "x_bim.h"
#include "x_timer.h"

#include "msdc_if.h"

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define TEST_MEM_SIZE           (0x200000)
#define MSDC_TESTBUF1_ADDR      (0x2000000)
#define MSDC_TESTBUF2_ADDR      (0x3000000)

static INT32 _MSDCCLI_Pinmux(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4Ch;

	if (i4Argc != 2)
	{
        LOG(0, "%s pnx [idx]\n", szArgv[0]);
		return 0;
	}
	
	u4Ch = StrToInt(szArgv[1]);	
    
    MSDC_PinMux(u4Ch);
    return 0;
}

static INT32 _MSDCCLI_SysInit(INT32 i4Argc, const CHAR ** szArgv)
{    
    MsdcSysInit();
    return 0;    
}

static INT32 _MSDCCLI_Identify(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4Ret = 0;
    UINT32 u4Force;
	
	if (i4Argc != 2)
	{
        LOG(0, "%s id [0/1]\n", szArgv[0]);
		return 0;
	}	
    
    u4Force = StrToInt(szArgv[1]);
    
    if (u4Force == 0)
    {
        i4Ret = MsdcIdentyCard(FALSE);
    }
    else
    {
        i4Ret = MsdcIdentyCard(TRUE);    
    }
    
    LOG(0, "_MSDCCLI_Identify, i4Ret = 0x%X.\n", i4Ret);
    return 0;
}

#if 0
static INT32 _MSDCCLI_SetAccessRegion(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4Ret = 0;
    UINT32 u4Flag;

	if (i4Argc != 2)
	{
        LOG(0, "%s [usr/boot1/boot2]\n", szArgv[0]);
		return 0;
	}
	
    if (x_strncmp(szArgv[1], "usr", sizeof("usr")) == 0)
    {
        u4Flag = 0;
    }
    else if (x_strncmp(szArgv[1], "boot1", sizeof("boot1")) == 0)
    {
        u4Flag = 1;
    }
    else if (x_strncmp(szArgv[1], "boot2", sizeof("boot2")) == 0)
    {
        u4Flag = 2;    
    }	
    
    i4Ret = MsdcSetAccessRegion(u4Flag);
    
    LOG(0, "_MSDCCLI_SetAccessRegion, i4Ret = 0x%X.\n", i4Ret);
    return 0;
}
#endif

static INT32 _MSDCCLI_Read(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4Ret = 0;
    UINT32 u4Offset, u4Len, u4DstPtr;

	if (i4Argc != 4)
	{
        LOG(0, "%s [memptr] [offset] [size]\n", szArgv[0]);
		return 0;
	}
    
    u4DstPtr = StrToInt(szArgv[1]);
    u4Offset = StrToInt(szArgv[2]);
    u4Len    = StrToInt(szArgv[3]);
	
	LOG(0, "_MSDCCLI_Read, DstPtr:0x%08X, Offset:0x%08X, Len:0x%08X\n", u4DstPtr, u4Offset, u4Len);
	
	i4Ret = MsdcReadCard(u4Offset, (UINT32 *)u4DstPtr, u4Len);
	
	LOG(0, "_MSDCCLI_Read, i4Ret = 0x%X.\n", i4Ret);
	return 0;
}

static INT32 _MSDCCLI_Write(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4Ret = 0, i;
	UINT8  u1Type;
    UINT8 *pu1SrcAddr;
    UINT32 u4Offset, u4Len, u4SrcPtr;

	if (i4Argc < 4)
	{
        LOG(0, "%s [memptr] [offset] [size] [Type]\n", szArgv[0]);
		return 0;
	}

    u4SrcPtr = StrToInt(szArgv[1]);
    u4Offset = StrToInt(szArgv[2]);
    u4Len    = StrToInt(szArgv[3]);

    if (i4Argc == 5)
    {
        u1Type = StrToInt(szArgv[4]);
    }
    else
    {
        u1Type = 0xFF;
    }
    	
	LOG(0, "_MSDCCLI_Write, SrcAddr:0x%X, Offset:0x%08X, Len:0x%08X, Type: 0x%02X\n", 
        u4SrcPtr, u4Offset, u4Len, u1Type);
	
    pu1SrcAddr = (UINT8 *)u4SrcPtr;
	if (u1Type == 0xFF)
	{
        for (i = 0; i < u4Len; i++)
        {
            *(pu1SrcAddr + i) = i; 
        }
	}
	else if (u1Type == 0x00)
	{
        for (i = 0; i < u4Len; i++)
        {
			*(pu1SrcAddr + i) = ((UINT32)rand()) % 0xFF;
        }
	}
	else
	{
        for (i = 0; i < u4Len; i++)
        {
            *(pu1SrcAddr + i) = u1Type; 
        }
	}
	
	i4Ret = MsdcWriteCard((UINT32 *)u4SrcPtr, u4Offset, u4Len);
	
	LOG(0, "_MSDCCLI_Write, i4Ret = 0x%X.\n", i4Ret);
	return 0;
}

static INT32 _MSDCCLI_Erase(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4Ret = 0;
    UINT32 u4Offset, u4Len;
    BOOL   fgEraseZero;

	if (i4Argc < 3)
	{
        LOG(0, "%s [offset] [size] [erasezero]\n", szArgv[0]);
		return 0;
	}

    UNUSED(fgEraseZero);
    u4Offset = StrToInt(szArgv[1]);
    u4Len    = StrToInt(szArgv[2]);

    if (i4Argc == 4)
    {
        fgEraseZero = (BOOL)StrToInt(szArgv[3]);
    }
    else
    {
        fgEraseZero = FALSE;
    }

    i4Ret = MsdcEraseCard(u4Offset, u4Len);
    return 0;
}

static INT32 _MSDCCLI_PartInfo(INT32 i4Argc, const CHAR ** szArgv)
{
    MSDCPART_ShowPartition();
    return 0;
}


/******************************************************************************
* MSDC function list
******************************************************************************/
static CLI_EXEC_T arMsdcCmdTbl[] =
{
    DECLARE_CMD(_MSDCCLI_Pinmux, pinmux, pnx, "msdc.pinmux"),
    DECLARE_CMD(_MSDCCLI_SysInit, init, init, "msdc.init"),
    DECLARE_CMD(_MSDCCLI_Identify, identify, id, "msdc.identify"),
    DECLARE_CMD(_MSDCCLI_Read, read, r, "msdc.read"),	
    DECLARE_CMD(_MSDCCLI_Write, write, w, "msdc.write"),
    DECLARE_CMD(_MSDCCLI_Erase, erase, e, "msdc.erase"),
    DECLARE_CMD(_MSDCCLI_PartInfo, partinfo, partinfo, "msdc.partinfo"),  

    DECLARE_END_ITEM(),
};

CLI_MAIN_COMMAND_ITEM(Msdc)
{
    "msdc",
    NULL,
    NULL,
    arMsdcCmdTbl,
    "Msdc command",
    CLI_GUEST
};

