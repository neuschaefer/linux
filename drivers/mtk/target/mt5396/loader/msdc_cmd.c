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
#include "x_bim.h"
#include "x_timer.h"

#include "msdc_if.h"

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define MSDC_TESTBUF1_ADDR    0x1000000
#define MSDC_TESTBUF2_ADDR    0x2000000
#define MSDC_TESTBUF3_ADDR    0x3000000

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
    INT32 i4Ret = 0;
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

static INT32 _MSDCCLI_SetDataMode(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;
	UINT32 u4fgDataMode;

	if (i4Argc != 2)
	{
        LOG(0, "setdatamode [1/2/3/4]\n");
		LOG(0, "     1 - PIO Mode\n");
		LOG(0, "     2 - Basic DMA Mode\n");
		LOG(0, "     3 - Descriptor DMA Mode\n");
		LOG(0, "     4 - Enhanced DMA Mode\n");
		return 0;
	}

	u4fgDataMode = StrToInt(szArgv[1]);

	i4Ret = MsdcSetDataMode(u4fgDataMode);

	LOG(0, "_MSDCCLI_SetDataMode, i4Ret = 0x%X.\n", i4Ret);
	return 0;
}

static INT32 _MSDCCLI_SetDataBusWidth(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;
	UINT32 u4BusWidth;

	if (i4Argc != 2)
	{
        LOG(0, "setbuswidth [1/4/8]\n");
		LOG(0, "     1 - 1bit Mode\n");
		LOG(0, "     4 - 4bit Mode\n");
		LOG(0, "     8 - 8bit Mode\n");
		return 0;
	}

	u4BusWidth = StrToInt(szArgv[1]);

	i4Ret = MsdcSetDataBusWidth(u4BusWidth);

    LOG(0, "_MSDCCLI_SetDataBusWidth, i4Ret = 0x%X.\n", i4Ret);
    return 0;
}

static INT32 _MSDCCLI_SetBusClock(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;
	UINT32 u4Clock;

	if (i4Argc != 2)
	{
        LOG(0, "setbusclock [clock(MHz)]: 54 48 43 40 36 30 27 24 18 13 0\n");
		return 0;
	}

	u4Clock = StrToInt(szArgv[1]);

	i4Ret = MsdcSetBusClock(u4Clock);

    LOG(0, "_MSDCCLI_SetBusClock, i4Ret = 0x%X.\n", i4Ret);
    return 0;

}

static INT32 _MSDCCLI_PrintConfigInfo(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;

	i4Ret = MsdcPrintConfigInfo();
	
	LOG(0, "_MSDCCLI_PrintConfigInfo, i4Ret = 0x%X.\n", i4Ret);
	return 0;
}

static INT32 _MSDCCLI_SetContinueClock(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;
	UINT32 fgContinueClock;

	if (i4Argc != 2)
	{
        LOG(0, "setcontinueclock [0/1]\n");
		return 0;
	}

	fgContinueClock = StrToInt(szArgv[1]);

	i4Ret = MsdcSetContinueClock(fgContinueClock);
	
	LOG(0, "_MSDCCLI_SetContinueClock, i4Ret = 0x%X.\n", i4Ret);
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
    INT32 i4Ret = 0;
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
    INT32 i4Ret = 0;
    UINT32 u4Offset, u4Len, u4SrcPtr;

	if (i4Argc < 4)
	{
        LOG(0, "%s [memptr] [offset] [size]\n", szArgv[0]);
		return 0;
	}

    u4SrcPtr = StrToInt(szArgv[1]);
    u4Offset = StrToInt(szArgv[2]);
    u4Len    = StrToInt(szArgv[3]);
    	
	LOG(0, "_MSDCCLI_Write, SrcAddr:0x%X, Offset:0x%08X, Len:0x%08X\n", 
                            u4SrcPtr, u4Offset, u4Len);
	
	i4Ret = MsdcWriteCard((UINT32 *)u4SrcPtr, u4Offset, u4Len);
	
	LOG(0, "_MSDCCLI_Write, i4Ret = 0x%X.\n", i4Ret);
	return 0;
}

static INT32 _MSDCCLI_Erase(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;
    UINT32 u4Offset, u4Len;
    BOOL fgEraseZero;

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

	LOG(0, "_MSDCCLI_Erase, i4Ret = 0x%X.\n", i4Ret);
    return 0;
}

static INT32 _MSDCCLI_Measure(INT32 i4Argc, const CHAR ** szArgv)
{
    HAL_TIME_T time1, time2, result_time;
    UINT32 /*u4Micros1,*/ u4Micros2, u4Micros3;
    UINT32 u4TestAddr, u4Len;
    UINT32 i, j, p;
    UINT8 *pu1TestBuf = NULL, *pu1GoldenBuf = NULL;

    if (i4Argc != 3)
    {
        Printf("%s [msdc_offset] [size]\n", szArgv[0]);
        return -1;
    }

    u4TestAddr = StrToInt(szArgv[1]);
    u4Len      = StrToInt(szArgv[2]);

    pu1TestBuf = (UINT8 *)MSDC_TESTBUF1_ADDR;
    pu1GoldenBuf = (UINT8 *)MSDC_TESTBUF2_ADDR;
    
    for (p = 0; p < 3; p++)
    {
        if (p == 0)
        {
            Printf("Measure pattern:0x00\n");
            x_memset(pu1GoldenBuf, 0x00, u4Len);
        }
        else if (p == 1)
        {
            Printf("Measure pattern:0x5A\n");
            x_memset(pu1GoldenBuf, 0x5A, u4Len);
        }
        else
        {
            Printf("Measure pattern:0xA5\n");
            x_memset(pu1GoldenBuf, 0xA5, u4Len);
        }

#if 0
        // Erase time
        Printf("Measure erase time:\n");
        HAL_GetTime(&time1);
        if (MsdcEraseCard(u4TestAddr, u4Len))
        {
            Printf("Erase eMMC error!\n");
            return -1;
        }
        HAL_GetTime(&time2);
        HAL_GetDeltaTime(&result_time, &time1, &time2);
        u4Micros1 = result_time.u4Seconds * 1000000 + result_time.u4Micros;
        Printf("Erase 0x%X elapse time: %d us\n", u4Len, u4Micros1);
#endif
        
        // Write time
        Printf("Measure write time:\n");
        HAL_GetTime(&time1);
        if (MsdcWriteCard((UINT32 *)pu1GoldenBuf, u4TestAddr, u4Len) != 0)
        {
            Printf("Write eMMC error!\n");
            return -1;
        }
        HAL_GetTime(&time2);
        HAL_GetDeltaTime(&result_time, &time1, &time2);
        u4Micros2 = result_time.u4Seconds * 1000000 + result_time.u4Micros;
        Printf("Write 0x%X elapse time: %d us\n", u4Len, u4Micros2);
    
        // Read time
        Printf("Measure read time:\n");
        x_memset(pu1TestBuf, 0xFF, u4Len);
        HAL_GetTime(&time1); 
        if (MsdcReadCard(u4TestAddr, (UINT32 *)pu1TestBuf, u4Len) != 0)
        {
            Printf("Read eMMC error!\n");
            return -1;
        }
        HAL_GetTime(&time2);
        HAL_GetDeltaTime(&result_time, &time1, &time2);
        u4Micros3 = result_time.u4Seconds * 1000000 + result_time.u4Micros;
        Printf("Read  0x%X elapse time: %d us\n", u4Len, u4Micros3);
    
        for (i = 0; i < u4Len; i++)
        {
            if (pu1GoldenBuf[i] != pu1TestBuf[i])
            {
                Printf("Read compare fail at offset 0x%X !\n", i);
                Printf("Write buffer: \n");
    
                for (j = 0; j < 0x20; j++)
                {
                    if (j % 16 == 0)
                    {
                        Printf("\n%03x: %02x ", j, pu1GoldenBuf[i + j]);
                    }
                    else
                    {
                        Printf("%02x ", pu1GoldenBuf[i + j]);
                    }
                }
                Printf("\n");
                
                Printf("Read buffer: \n");
                for (j = 0; j < 0x20; j++) 
                {
                    if (j % 16 == 0)
                    {
                        Printf("\n%03x: %02x ", j, pu1TestBuf[i + j]);
                    }
                    else
                    {
                        Printf("%02x ", pu1TestBuf[i + j]);
                    }
                }
    
                Printf("\n");
                return -1;
            }
        }
    }
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
    DECLARE_CMD(_MSDCCLI_SetDataMode, setdatamode, sdm, "msdc.setdatamode"),
    DECLARE_CMD(_MSDCCLI_SetDataBusWidth, setbuswidth, sbw, "msdc.setbuswidth"),
    DECLARE_CMD(_MSDCCLI_SetBusClock, setbusclock, sbc, "msdc.setbusclock"),
    DECLARE_CMD(_MSDCCLI_PrintConfigInfo, configinfo, pci, "msdc.configinfo"),
    DECLARE_CMD(_MSDCCLI_SetContinueClock, setcontinueclock, scc, "msdc.setcontinueclock"),
    DECLARE_CMD(_MSDCCLI_Read, read, r, "msdc.read"),	
    DECLARE_CMD(_MSDCCLI_Write, write, w, "msdc.write"),
    DECLARE_CMD(_MSDCCLI_Erase, erase, e, "msdc.erase"),
    DECLARE_CMD(_MSDCCLI_Measure, measure, m, "msdc.measure"),
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

