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
#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_os.h"
#include "x_drv_cli.h"
#ifdef CC_MTK_LOADER
#define DEFINE_IS_LOG(level, fmt...) Printf(fmt)
#else /* CC_MTK_LOADER */
#define DEFINE_IS_LOG	CLI_IsLog
#endif /* CC_MTK_LOADER */
#include "x_debug.h"
#include "x_util.h"

#include "x_timer.h"
#include "nand_if.h"
#include "storage_if.h"


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define NAND_TESTBUF_SIZE	0x4000	// 16K bytes -> block size
#define NAND_TESTBUF1_ADDR	0x1000000
#define NAND_TESTBUF2_ADDR	0x2000000
#define NAND_TESTBUF3_ADDR	0x3000000


#define VERBOSE
#ifdef DECLARE_CMD
#undef DECLARE_CMD
#endif
#define DECLARE_CMD(func, cmd, ab, msg)     \
    {                                       \
        #cmd,           /* cmd name */      \
        #ab,            /* cmd abname */    \
        func,           /* function */      \
        NULL,           /* sublist */       \
        msg,            /* msg */           \
        CLI_GUEST  /* permission */    \
    }

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
#if 0
//-------------------------------------------------------------------------
static INT32 _NANDCLI_Init(INT32 i4Argc, const CHAR ** szArgv)
{
    NAND_Init();

    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_Diag(INT32 i4Argc, const CHAR ** szArgv)
{
	INT32 i4Ret;

	i4Ret = NAND_Diag();
	Printf(	"\n==================\n"
			" Nand Diag %s!"
			"\n==================\n", ((i4Ret==0) ? "Success" : "Fail"));
	return i4Ret;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_Stop(INT32 i4Argc, const CHAR ** szArgv)
{
	NAND_Stop();
	return 0;
}


//-------------------------------------------------------------------------
static INT32 _NANDCLI_Reset(INT32 i4Argc, const CHAR ** szArgv)
{
	NAND_Reset();
	return 0;
}

#if 0
//-------------------------------------------------------------------------
static INT32 _NANDCLI_Enable(INT32 i4Argc, const CHAR ** szArgv)
{
	return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_Disable(INT32 i4Argc, const CHAR ** szArgv)
{
	return 0;
}
#endif

//-------------------------------------------------------------------------
static void _NAND_STATE_Usage(const CHAR *pu1Cmd)
{
    Printf("%s [-n/-h/-s] [-D/-d] [-I/-i]\n", pu1Cmd);
    Printf("   -n: No ECC\n");
    Printf("   -h: Hardware ECC\n");
    Printf("   -s: Software ECC\n");
    Printf("   -D: Enable DMA\n");
    Printf("   -d: Disable DMA\n");
    Printf("   -I: Enable interrupt\n");
    Printf("   -i: Disable interrupt\n");
}

//-------------------------------------------------------------------------
static void _NAND_Ctrl_Status(NAND_CONTROL_T* prCtrl)
{
    Printf("%s Isr, %s DMA",
            prCtrl->fgIsr ? "Enable" : "Disable",
            prCtrl->fgDMA ? "Enable" : "Disable");

    switch(prCtrl->rEccType)
    {
    case NAND_ECC_NONE:
        printf(" no ECC\n");
        break;
    case NAND_ECC_SOFT:
        printf(" soft ECC\n");
        break;
    case NAND_ECC_HARD:
        printf(" hard ECC\n");
        break;
    }
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_State(INT32 i4Argc, const CHAR ** szArgv)
{
    NAND_CONTROL_T rCtrl;
	UINT32 i, u4Flag;

    NAND_GetControl(NAND_CTRL_FLAGS_ALL, &rCtrl);
	if (i4Argc <= 1)
	{
	    _NAND_Ctrl_Status(&rCtrl);
	    Printf("\n");
    	_NAND_STATE_Usage(szArgv[0]);
    	return 0;
	}

    u4Flag = 0;
	for (i=1; i<i4Argc; i++)
	{
		if (szArgv[i][0]!='-')
		{
		    continue;
		}
		switch(szArgv[i][1])
		{
		case 'n':
		    u4Flag |= NAND_CTRL_FLAGS_ECC;
		    rCtrl.rEccType = NAND_ECC_NONE;
			break;
		case 'h':
		    u4Flag |= NAND_CTRL_FLAGS_ECC;
		    rCtrl.rEccType = NAND_ECC_HARD;
		    break;
		case 's':
		    u4Flag |= NAND_CTRL_FLAGS_ECC;
		    rCtrl.rEccType = NAND_ECC_SOFT;
		    break;
		case 'D':
		    u4Flag |= NAND_CTRL_FLAGS_DMA;
		    rCtrl.fgDMA = TRUE;
			break;
	    case 'd':
		    u4Flag |= NAND_CTRL_FLAGS_DMA;
		    rCtrl.fgDMA = FALSE;
	        break;
	    case 'I':
		    u4Flag |= NAND_CTRL_FLAGS_ISR;
		    rCtrl.fgIsr = TRUE;
	        break;
	    case 'i':
		    u4Flag |= NAND_CTRL_FLAGS_ISR;
		    rCtrl.fgIsr = FALSE;
	        break;
        default:
            _NAND_STATE_Usage(szArgv[0]);
            break;
		}
	}

    NAND_SetControl(u4Flag, &rCtrl);
    _NAND_Ctrl_Status(&rCtrl);

	return 0;
}


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
static INT32 _DoPatternTest(UINT32 u4Start, UINT32 u4Size)
{
	UINT32 loop, u4Addr, u4SecId;
	NANDDEV_T nanddev;
	UINT8 *TestBuf, *CompBuf;
	UINT32 u4BlkCount, u4BlkStart;

	TestBuf = (UINT8*)NAND_TESTBUF1_ADDR;
	CompBuf = (UINT8*)NAND_TESTBUF2_ADDR;

	if(NAND_GetInfo(&nanddev) != 0)
	{
		Printf("Get flash info error!!\n");
		return 1;
	}

    // Use block unit
	u4BlkStart = (u4Start & ~(nanddev.u4BlkSz - 1)) / nanddev.u4BlkSz;
	u4BlkCount = (u4Size + nanddev.u4BlkSz - 1) / nanddev.u4BlkSz;
	u4Addr = u4Start & ~(nanddev.u4BlkSz - 1);
	u4SecId = u4BlkStart * (nanddev.u4BlkSz / nanddev.u4PgSz);
	for(loop=0; loop < u4BlkCount; loop++)
	{
		if(NAND_Erase(u4SecId, 1) != 0)
		{
			Printf("Erase error!!!\n");
//			return 1;
		}

		if(NAND_Write((UINT64)u4Addr, (UINT32)TestBuf, nanddev.u4BlkSz) != 0)
		{
			Printf("Write error!!!\n");
//			return 1;
		}

		x_memset((void*)CompBuf, 0xFF, nanddev.u4BlkSz);
		if(NAND_Read((UINT64)u4Addr, (UINT32)CompBuf, nanddev.u4BlkSz) != 0)
		{
			Printf("Read error!!!\n");
//			return 1;
		}

		if(x_memcmp((void*)TestBuf, (void*)CompBuf, nanddev.u4BlkSz) != 0)
		{
			Printf("Compare error!!\n");
//			return 1;
		}

		u4Addr += nanddev.u4BlkSz;
		u4SecId += (nanddev.u4BlkSz / nanddev.u4PgSz);

		if((((u4BlkStart+loop+1) * nanddev.u4BlkSz) % 0x100000) == 0)
		{
			Printf("NAND flash %03d MB test OK\n",
			            ((u4BlkStart+loop+1)*nanddev.u4BlkSz) / 0x100000);
		}
	}

	return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_Pattern(UINT32 u4Start, UINT32 u4Size)
{
	UINT8 *TestBuf;
	UINT32 i, *TestBuf2, u4RandomSeed;
	HAL_RAW_TIME_T H_RawTime;

	TestBuf = (UINT8*)NAND_TESTBUF1_ADDR;
	TestBuf2 = (UINT32*)NAND_TESTBUF1_ADDR;

	Printf("Pattern: 1\n");
	for(i=0; i<NAND_TESTBUF_SIZE; i++)
	{
	    TestBuf[i] = 0x5A;
	}
	if( _DoPatternTest(u4Start, u4Size) != 0)
	{
		return -1;
	}

	Printf("Pattern: 2\n");
	for(i=0; i<NAND_TESTBUF_SIZE; i++)
	{
	    TestBuf[i] = 0xA5;
	}
	if( _DoPatternTest(u4Start, u4Size) != 0)
	{
		return -1;
	}

	Printf("Pattern: increase address\n");
	for(i=0; i<NAND_TESTBUF_SIZE; i++)
	{
	    TestBuf[i] = (UINT8)(i&0xFF);
	}
	if( _DoPatternTest(u4Start, u4Size) != 0)
	{
		return -1;
    }
	Printf("Pattern: random\n");
    HAL_GetRawTime(&H_RawTime) ;
    u4RandomSeed = H_RawTime.u4Cycles ;
    srand(u4RandomSeed );
	for(i=0; i<NAND_TESTBUF_SIZE; i++)
	{
	    TestBuf2[i] = (UINT32)rand();
	}
	if( _DoPatternTest(u4Start, u4Size) != 0)
	{
		return -1;
	}

	return 0;
}

//-------------------------------------------------------------------------
static void _NAND_PT_Usage(const CHAR* pu1Cmd)
{
	Printf("%s [burst/dmap/dmai] [addr] [size]\n", pu1Cmd);
	Printf("   burst: Burst mode test\n");
	Printf("   dmap:  DMA polling mode test\n");
	Printf("   dmai:  DMA interrupt mode test\n");
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PatternTest(INT32 i4Argc, const CHAR ** szArgv)
{
    NAND_CONTROL_T rOrgCtrl, rNewCtrl;
	UINT32 u4Start, u4Size;

	if(i4Argc != 4)
	{
    	_NAND_PT_Usage(szArgv[0]);
		return 0;
	}

	u4Start = 0x0;
	u4Size = 0x2000000;
	NAND_GetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtrl);

    if(x_strncmp(szArgv[1], "burst", sizeof("burst")) == 0)
    {
        rNewCtrl.fgDMA = FALSE;
        rNewCtrl.fgIsr = FALSE;
    }
    else if(x_strncmp(szArgv[1], "dmai", sizeof("dmai")) == 0)
    {
        rNewCtrl.fgDMA = TRUE;
        rNewCtrl.fgIsr = TRUE;
    }
    else if(x_strncmp(szArgv[1], "dmap", sizeof("dmap")) == 0)
    {
        rNewCtrl.fgDMA = TRUE;
        rNewCtrl.fgIsr = FALSE;
    }
    else
    {
        _NAND_PT_Usage(szArgv[0]);
        return -1;
    }

    u4Start = StrToInt(szArgv[2]);
    u4Size = StrToInt(szArgv[3]);

	NAND_SetControl(NAND_CTRL_FLAGS_ISR | NAND_CTRL_FLAGS_DMA, &rNewCtrl);
	if(_NANDCLI_Pattern(u4Start, u4Size) != 0)
	{
		Printf("Test fail!!\n");
		NAND_SetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtrl);
		return -1;
	}
	Printf("Test OK!!\n");

	NAND_SetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtrl);
	Printf("NAND pattern test OK!!\n");

	return 0;
}


//-------------------------------------------------------------------------
static INT32 _NAND_DoAttrTest(UINT32 u4Offset, UINT32 u4GoldenMemAddr, UINT32 u4TestMemAddr)
{
    UINT32 i, *pu4MemPtr, *pu4GoldenPtr;
	NANDDEV_T rNandDev;

    pu4GoldenPtr = (UINT32*)u4GoldenMemAddr;
    pu4MemPtr = (UINT32*)u4TestMemAddr;
    NAND_GetInfo(&rNandDev);

    if(NAND_Erase(u4Offset/rNandDev.u4PgSz, 1) != 0)
    {
        Printf("Erase fail !\n");
        return 1;
    }

    x_memset(pu4MemPtr, 0x0, rNandDev.u4OOBSz);
    if(NAND_RdAttr(u4Offset, (UINT32)pu4MemPtr, (UINT32)rNandDev.u4OOBSz) != 0)
    {
        Printf("Erase read back fail!\n");
        return 1;
    }
    for(i=0; i<(rNandDev.u4OOBSz/4); i++)
    {
        if(pu4MemPtr[i] != 0xFFFFFFFF)
        {
            Printf("Erase check fail!\n");
            return 1;
        }
    }

    if(NAND_WrAttr(u4Offset, (UINT32)pu4GoldenPtr, rNandDev.u4OOBSz) != 0)
    {
        Printf("Write attr fail!\n");
        return 1;
    }
    x_memset(pu4MemPtr, 0x0, rNandDev.u4OOBSz);
    if(NAND_RdAttr(u4Offset, (UINT32)pu4MemPtr, rNandDev.u4OOBSz) != 0)
    {
        Printf("Read attr fail!\n");
        return 1;
    }

    if(x_memcmp(pu4MemPtr, pu4GoldenPtr, rNandDev.u4OOBSz) != 0)
    {
        Printf("Compare fail!\n");
        return 1;
    }

    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_AttrTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 i, *pu4GoldenPtr;

    pu4GoldenPtr = (UINT32*)0x3100000;

    Printf("Pattern 1...\n");
    for(i=0; i<(64/4); i++)
    {
        pu4GoldenPtr[i] = 0x5A5A5A5A;
    }
    if(_NAND_DoAttrTest(0, 0x3100000, 0x300000)!= 0)
    {
        Printf("Attr test fail!\n");
        return 1;
    }
    Printf("OK\n");

    Printf("Pattern 2...\n");
    for(i=0; i<(64/4); i++)
    {
        pu4GoldenPtr[i] = 0xA5A5A5A5;
    }
    if(_NAND_DoAttrTest(0x100000, 0x3100000, 0x300000)!= 0)
    {
        Printf("Attr test fail!\n");
        return 1;
    }
    Printf("OK\n");

    Printf("Pattern 3...\n");
    for(i=0; i<(64/4); i++)
    {
        pu4GoldenPtr[i] = 0x300000 + i<<4;
    }
    if(_NAND_DoAttrTest(0x200000, 0x3100000, 0x300000)!= 0)
    {
        Printf("Attr test fail!\n");
        return 1;
    }
    Printf("OK\n");

    Printf("Attr test OK!\n");

    return 0;
}


//-------------------------------------------------------------------------
static INT32 _NANDCLI_SetECC(INT32 i4Argc, const CHAR ** szArgv)
{
    NAND_CONTROL_T rCtrl;

    NAND_GetControl(NAND_CTRL_FLAGS_ECC, &rCtrl);
    if(i4Argc != 2)
    {
        Printf("ECC: ");
        if(rCtrl.rEccType == NAND_ECC_NONE)
        {
            Printf("off\n");
        }
        else if(rCtrl.rEccType == NAND_ECC_SOFT)
        {
            Printf("sw\n");
        }
        else if(rCtrl.rEccType == NAND_ECC_HARD)
        {
            Printf("hw\n");
        }
        Printf("%s [hw/sw/off]\n", szArgv[0]);
    return 0;
}

    if(x_strcmp("off", szArgv[1]) == 0)
    {
        rCtrl.rEccType = NAND_ECC_NONE;
    }
    else if(x_strcmp("sw", szArgv[1]) == 0)
    {
        rCtrl.rEccType = NAND_ECC_SOFT;
    }
    else if(x_strcmp("hw", szArgv[1]) == 0)
    {
        rCtrl.rEccType = NAND_ECC_HARD;
    }
    else
    {
        return -1;
    }

    NAND_SetControl(NAND_CTRL_FLAGS_ECC, &rCtrl);
    return 0;
}
#endif
//-------------------------------------------------------------------------
static INT32 _NANDCLI_Read(INT32 i4Argc, const CHAR ** szArgv)
{
#ifdef VERBOSE
	UINT8 *ptr;
	INT32 i;
#endif
	UINT32 u4ByteCount, u4MemPtr, u4Offset;

	if (i4Argc < 4)
	{
		Printf("%s [memptr] [nand_offset] [bytenum]\n", szArgv[0]);
		return 1;
	}
	u4MemPtr = StrToInt(szArgv[1]);
	u4Offset = StrToInt(szArgv[2]);
	u4ByteCount = StrToInt(szArgv[3]);

	if(NAND_Read((UINT64)u4Offset, u4MemPtr, u4ByteCount) != 0)
    {
		Printf("nand.read operates failed\n");
		return 1;
	}

	Printf("nand.read operates successfully.\n");

#ifdef VERBOSE
	ptr = (UINT8 *)u4MemPtr;
	for (i=0; i<0x20; i++) {
		if (i%16==0) Printf("\n%03x: %02x ", i, ptr[i]);
		else Printf("%02x ", ptr[i]);
	}
	Printf("\n");
#endif
	return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_Write(INT32 i4Argc, const CHAR ** szArgv)
{
#ifdef VERBOSE
	UINT8 *ptr;
	INT32 i;
#endif
	UINT32 u4ByteCount, u4MemPtr, u4Offset;

	if (i4Argc < 4)
	{
		Printf("%s [memptr] [nand_offset] [bytenum]\n", szArgv[0]);
		return 1;
	}

	u4MemPtr = StrToInt(szArgv[1]);
	u4Offset = StrToInt(szArgv[2]);
	u4ByteCount = StrToInt(szArgv[3]);


	if(NAND_Write((UINT64)u4Offset, u4MemPtr, u4ByteCount) != 0)
    {
		Printf("nand.write operates failed\n");
		return 1;
	}

	Printf("nand.write operates successfully.\n");

#ifdef VERBOSE
	ptr = (UINT8 *)u4MemPtr;
	for (i=0; i<0x20; i++) {
		if (i%16==0) Printf("\n%03x: %02x ", i, ptr[i]);
		else Printf("%02x ", ptr[i]);
	}
	Printf("\n");
#endif
	return 0;
}

#if 0
//-------------------------------------------------------------------------
static INT32 _NANDCLI_RdAttr(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT8 *ptr;
	INT32 i4Ret;
	UINT32 i, u4Offset, u4MemPtr;
	NANDDEV_T rNandDev;

	if (i4Argc < 3)
	{
		Printf("%s [memptr] [nand_offset]\n", szArgv[0]);
		return 1;
	}
    u4MemPtr = StrToInt(szArgv[1]);
    u4Offset = StrToInt(szArgv[2]);

	NAND_GetInfo(&rNandDev);
    i4Ret = NAND_RdAttr((UINT64)u4Offset, u4MemPtr, (UINT32)rNandDev.u4OOBSz);
    if (i4Ret)
    {
        return i4Ret;
    }
	ptr = (UINT8 *)u4MemPtr;
	for (i=0; i<rNandDev.u4OOBSz; i++) {
		if (i%16==0) Printf("\n%03x: %02x ", i, ptr[i]);
		else Printf("%02x ", ptr[i]);
	}
	Printf("\n");
    return i4Ret;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_WrAttr(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT8 *ptr;
	INT32 i4Ret;
	UINT32 i, u4Offset, u4MemPtr;
	NANDDEV_T rNandDev;

	if (i4Argc < 3)
	{
		Printf("%s [memptr] [nand_offset]\n", szArgv[0]);
		return 1;
	}
	u4MemPtr = StrToInt(szArgv[1]);
	u4Offset = StrToInt(szArgv[2]);

	NAND_GetInfo(&rNandDev);
    i4Ret = NAND_WrAttr((UINT64)u4Offset, u4MemPtr, rNandDev.u4OOBSz);
    if (i4Ret)
    {
        return i4Ret;
    }
	ptr = (UINT8 *)u4MemPtr;
	for (i=0; i<rNandDev.u4OOBSz; i++) {
		if (i%16==0) Printf("\n%03x: %02x ", i, ptr[i]);
		else Printf("%02x ", ptr[i]);
	}
	Printf("\n");
    return i4Ret;
}
#endif
//-------------------------------------------------------------------------
static INT32 _NANDCLI_Erase(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4Offset, u4ByteCount, u4PgStart, u4PgEnd, u4PgCount;
	NANDDEV_T rNandDev;

	if (i4Argc < 3)
	{
		Printf("%s [nand_offset] [bytelen]\n", szArgv[0]);
		return 1;
	}

	if(NAND_GetInfo(&rNandDev) != 0)
    {
		Printf("NAND_Identify failed\n");
		return 1;
	}

    u4Offset = StrToInt(szArgv[1]);
    u4ByteCount = StrToInt(szArgv[2]);

    u4PgStart = u4Offset / rNandDev.u4PgSz;
    u4PgEnd = (u4Offset + u4ByteCount + rNandDev.u4PgSz - 1) / rNandDev.u4PgSz;
    u4PgCount = u4PgEnd - u4PgStart;

    Printf("Erase from sect: %d ~ %d\n", u4PgStart, u4PgEnd);
	NAND_Erase(u4PgStart, u4PgCount);
	Printf("Erase from sect: %d ~ %d Successfully\n", u4PgStart, u4PgEnd);
	return 0;
}
//-------------------------------------------------------------------------
static INT32 _NANDCLI_EraseForce(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4Offset, u4ByteCount, u4PgStart, u4PgEnd, u4PgCount;
	NANDDEV_T rNandDev;
	if (i4Argc < 3)
	{
		Printf("%s [nand_offset] [bytelen]\n", szArgv[0]);
		return 1;
	}
	if(NAND_GetInfo(&rNandDev) != 0)
    {
		Printf("NAND_Identify failed\n");
		return 1;
	}
    u4Offset = StrToInt(szArgv[1]);
    u4ByteCount = StrToInt(szArgv[2]);
    u4PgStart = u4Offset / rNandDev.u4PgSz;
    u4PgEnd = (u4Offset + u4ByteCount + rNandDev.u4PgSz - 1) / rNandDev.u4PgSz;
    u4PgCount = u4PgEnd - u4PgStart;
    Printf("Starting Erase from sect: %d ~ %d\n", u4PgStart, u4PgEnd);
	NAND_EraseForce(u4PgStart, u4PgCount);
	Printf("Erase sect: %d ~ %d\n done\n", u4PgStart, u4PgEnd);
	return 0;
}


#if 0
//-------------------------------------------------------------------------
static INT32 _NANDCLI_EraseForce(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4Offset, u4ByteCount, u4PgStart, u4PgEnd, u4PgCount;
	NANDDEV_T rNandDev;
	if (i4Argc < 3)
	{
		Printf("%s [nand_offset] [bytelen]\n", szArgv[0]);
		return 1;
	}
	if(NAND_GetInfo(&rNandDev) != 0)
    {
		Printf("NAND_Identify failed\n");
		return 1;
	}
    u4Offset = StrToInt(szArgv[1]);
    u4ByteCount = StrToInt(szArgv[2]);
    u4PgStart = u4Offset / rNandDev.u4PgSz;
    u4PgEnd = (u4Offset + u4ByteCount + rNandDev.u4PgSz - 1) / rNandDev.u4PgSz;
    u4PgCount = u4PgEnd - u4PgStart;
    Printf("Erase from sect: %d ~ %d\n", u4PgStart, u4PgEnd);
	if (NAND_EraseForce(u4PgStart, u4PgCount) != 0)
	{
		Printf("nand.erase operates failed\n");
		return 1;
	}

	Printf("nand.erase operates successfully.\n");

	return 0;
}


//-------------------------------------------------------------------------
static INT32 _NANDCLI_BlankCheck(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Offset, u4PageCount;

    if(i4Argc != 3)
    {
        Printf("%s [nand_offset] [bytenum]\n", szArgv[0]);
        return 1;
    }

    u4Offset = StrToInt(szArgv[1]);
    u4PageCount = StrToInt(szArgv[2]);

    if(NAND_BlankCheck((UINT64)u4Offset, u4PageCount) != 0)
    {
        Printf("NAND blank check fail!\n");
        return 1;
    }

    Printf("Blank check OK!\n");

    return 0;
}


//-------------------------------------------------------------------------
static INT32 _NANDCLI_BlankCheckAttr(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Offset, u4PageCnt;

    if(i4Argc != 3)
    {
        Printf("%s [nand_offset] [pagecount]\n", szArgv[0]);
        return 1;
    }

    u4Offset = StrToInt(szArgv[1]);
    u4PageCnt = StrToInt(szArgv[2]);

    if(NAND_BlankCheckAttr((UINT64)u4Offset, u4PageCnt) != 0)
    {
        Printf("NAND blank check fail!\n");
        return 1;
    }

    Printf("Blank check OK!\n");

    return 0;
}


//-------------------------------------------------------------------------
static INT32 _NANDCLI_IdDevice(INT32 i4Argc, const CHAR ** szArgv)
{
	INT32 i4Ret;
	UINT32 u4ID;

	i4Ret = NAND_ReadID(&u4ID);

	if (i4Ret)
	{
		Printf("NAND_Identify failed\n");
		return 1;
	}

	Printf("NAND Device ID: 0x%X\n", u4ID);

	return 0;
}
#endif
//-------------------------------------------------------------------------
static INT32 _NANDCLI_Measure(INT32 i4Argc, const CHAR ** szArgv)
{
	HAL_TIME_T time1, time2, result_time;
	UINT32 u4Micros1, u4Micros2, u4Micros3;
	//UINT32 u4Micros4;
	UINT32 u4TestAddr, u4Len;
	UINT32 u4PgIdx, u4PgCount;
	NANDDEV_T rNandDev;
	UINT32 i, j;
	UINT8 *pu1TestBuf, *pu1GoldenBuf;
	NAND_CONTROL_T rOrgCtrl, rNewCtrl;

	if(i4Argc != 3)
	{
	    Printf("%s [nand_offset] [size]\n", szArgv[0]);
	    return -1;
	}

    u4TestAddr = StrToInt(szArgv[1]);
    u4Len = StrToInt(szArgv[2]);

	if (NAND_GetInfo(&rNandDev) != 0)
	{
		Printf("NAND_Identify failed\n");
		return 1;
	}

	pu1TestBuf = (UINT8*)NAND_TESTBUF1_ADDR;
	pu1GoldenBuf = (UINT8*)NAND_TESTBUF2_ADDR;

    // Backup org control
	NAND_GetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtrl);

	// Set new control
	rNewCtrl.fgIsr = FALSE;
	rNewCtrl.fgDMA = TRUE;
	NAND_SetControl((UINT32)(NAND_CTRL_FLAGS_DMA | NAND_CTRL_FLAGS_ISR), &rNewCtrl);

	u4PgIdx = u4TestAddr / rNandDev.u4PgSz;
	u4PgCount = u4Len / rNandDev.u4PgSz;

	// Erase time
	Printf("Measure erase time:\n");
	HAL_GetTime(&time1);
	NAND_Erase(u4PgIdx, u4PgCount);
	HAL_GetTime(&time2);
	HAL_GetDeltaTime(&result_time, &time1, &time2);
	u4Micros1 = result_time.u4Seconds * 1000000 + result_time.u4Micros;

	for(i=0; i<u4Len; i+=0x200)
      {
          if(i < 0x200000)
          {
            x_memset(&pu1GoldenBuf[i], 0xAA, 0x200);
          }
          else if(i < 0x400000)
          {
            x_memset(&pu1GoldenBuf[i], 0x55, 0x200);
          }
          else if(i < 0x600000)
          {
            x_memset(&pu1GoldenBuf[i], 0x0F, 0x200);
          }
          else
          {
              for(j = i; j < (i+0x200) ; j++)
              {
                 pu1GoldenBuf[j] = (UINT8)(j&0xFF);
              }
          }
          pu1GoldenBuf[i+ (((i&0x1FFFFF)/0x200)>>3)] ^= (1<<(((i&0x1FFFFF)/0x200)&0x7));
	}
	// Write time, test 1MBytes
	Printf("Measure write time:\n");

	HAL_GetTime(&time1);
	if(NAND_Write((UINT64)u4TestAddr, (UINT32)pu1GoldenBuf, u4Len) != 0)
	{
		Printf("Write NAND flash error!\n");
		NAND_SetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtrl);
		return -1;
	}
	HAL_GetTime(&time2);
	HAL_GetDeltaTime(&result_time, &time1, &time2);
	u4Micros2 = result_time.u4Seconds * 1000000 + result_time.u4Micros;

	// Read time
	Printf("Measure read time:\n");
	x_memset(pu1TestBuf, 0xFF, u4Len);
	HAL_GetTime(&time1);
	if(NAND_Read((UINT64)u4TestAddr, (UINT32)pu1TestBuf, u4Len) != 0)
	{
		Printf("Read NAND flash error!\n");
		NAND_SetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtrl);
		return -1;
	}
	HAL_GetTime(&time2);
	HAL_GetDeltaTime(&result_time, &time1, &time2);
	u4Micros3 = result_time.u4Seconds * 1000000 + result_time.u4Micros;

#if 1
	for(i=0; i<u4Len; i++)
	{
	    if(pu1GoldenBuf[i] != pu1TestBuf[i])
	    {
    		Printf("Read compare fail at offset 0x%X !\n", i);
    		NAND_SetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtrl);
    		return -1;
    	}
	}
#endif

	Printf("Erase 0x%X elapse time: %d us\n", u4Len, u4Micros1);
	Printf("Write 0x%X elapse time: %d us\n", u4Len, u4Micros2);
	Printf("Read  0x%X elapse time: %d us\n", u4Len, u4Micros3);
#if 0
    if(rNandDev.u4PgSz == 512)
    {
        // Block read time
    	Printf("Measure block read time:\n");
    	x_memset(pu1TestBuf, 0xFF, u4Len);
    	HAL_GetTime(&time1);
    	if(NAND_ReadBlock((UINT64)u4TestAddr, (UINT32)pu1TestBuf, u4Len) != 0)
    	{
    		Printf("Block Read NAND flash error!\n");
    		NAND_SetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtrl);
    		return -1;
    	}
    	HAL_GetTime(&time2);
    	HAL_GetDeltaTime(&result_time, &time1, &time2);
    	u4Micros4 = result_time.u4Seconds * 1000000 + result_time.u4Micros;
#if 1
    	for(i=0; i<u4Len; i++)
    	{
    	    if(pu1GoldenBuf[i] != pu1TestBuf[i])
    	    {
        		Printf("Block read compare fail at 0x%X !\n", i);
        		NAND_SetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtrl);
        		return -1;
        	}
    	}
#endif
    	Printf("Block Read  1 MB elapse time: %d us\n", u4Micros4);
    }
#endif
    NAND_SetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtrl);

	return 0;
}


//-------------------------------------------------------------------------
static INT32 _NANDCLI_CheckBadBlk(INT32 i4Argc, const CHAR ** szArgv)
{
    NANDDEV_T rNandDev;
    UINT32 i;

    if(NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("NAND_Identify fail!\n");
        return 1;
    }

    Printf("Bad block:\n");
    for(i=0; i<rNandDev.u4TotalBlk; i++)
    {
        if(NAND_IsBadBlk(i))
        {
            Printf(" blk idx:%d, offset:0x%x\n", i, i * rNandDev.u4BlkSz);
        }
    }

    return 0;
}


//-------------------------------------------------------------------------
static INT32 _NANDCLI_MarkBadBlk(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Blk;
    NANDDEV_T rNandDev;

    if(i4Argc != 2)
    {
        Printf("%s [blk_idx]\n", szArgv[0]);
        return -1;
    }
    if(NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("NAND_Identify fail!\n");
        return 1;
    }

    u4Blk = StrToInt(szArgv[1]);

    if(!NAND_MarkBadBlk(u4Blk))
    {
        Printf("Fail\n");
        return -1;
    }

    Printf("OK\n");

    return 0;
}
#if 0
//-------------------------------------------------------------------------
static INT32 _NANDCLI_ShowDevList(INT32 i4Argc, const CHAR ** szArgv)
{
    NAND_ShowDevList();
    return 0;
}
//-------------------------------------------------------------------------
/** _NORPinShareThread
 */
//-------------------------------------------------------------------------
static void _NANDPinShareThread(void* pvArg)
{
    UINT32 i, j;
    UINT32 u4MemPtr1;
    UINT32 u4MemPtr2;
    UINT8 *pu1MemBuf1;
    UINT8 *pu1MemBuf2;
    NANDDEV_T rNandDev;
	NAND_CONTROL_T rNewCtrl;

    UNUSED(pvArg);
    if(NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("NAND_Identify fail!\n");
        return;
    }
    u4MemPtr1 = (UINT32)x_mem_alloc(rNandDev.u4BlkSz);
    if (u4MemPtr1 == 0)
    {
        Printf("mem alloc fail\n");
        return;
    }
    pu1MemBuf1 = (UINT8 *)u4MemPtr1;
    u4MemPtr2 = (UINT32)x_mem_alloc(rNandDev.u4BlkSz);
    if (u4MemPtr2 == 0)
    {
        Printf("mem alloc fail\n");
        return;
    }
    pu1MemBuf2 = (UINT8 *)u4MemPtr2;
	rNewCtrl.fgIsr = FALSE;
	rNewCtrl.fgDMA = FALSE;
	NAND_SetControl(NAND_CTRL_FLAGS_DMA | NAND_CTRL_FLAGS_ISR, &rNewCtrl);

    Printf("_NANDPinShareTest start\n");
    while(1)
    {
        for (i = (rNandDev.u4TotalBlk-100); i < rNandDev.u4TotalBlk; i++)
        {
            if ((_u4PinShareOption&3) == 3)
            {
                NOR_SemaLock();
                if (NAND_Erase( (i*rNandDev.u4PgPerBlk), rNandDev.u4PgPerBlk) )
                {
                    Printf("nand erase fail at:0x%x\n", i);
	}
                NOR_SemaUnLock();
            }
            if ((_u4PinShareOption&2))
            {
                for (j = 0; j < (rNandDev.u4BlkSz); j++)
                {
                    pu1MemBuf1[j] = (i+j)&0xFF;
}
                NOR_SemaLock();
                if (NAND_Write((i*rNandDev.u4BlkSz), u4MemPtr1,(rNandDev.u4BlkSz)) )
{
                    Printf("nand write fail at:0x%x\n", i);
                }
                NOR_SemaUnLock();
            }
            if ((_u4PinShareOption &1))
            {
                NOR_SemaLock();
                if (NAND_Read((i*rNandDev.u4BlkSz), u4MemPtr2,(rNandDev.u4BlkSz)))
	{
                    Printf("nand read fail at:0x%x\n", i);
                }
                NOR_SemaUnLock();
	}
            if ((_u4PinShareOption &3)==3)
            {
                for(j = 0; j < (rNandDev.u4BlkSz); j++)
                {
                    if (pu1MemBuf1[j] != pu1MemBuf2[j])
    {
                        Printf("nand cmp err at:0x%x, wrdata:0x%x, redata:0x%x\n",
                        j , pu1MemBuf1[j], pu1MemBuf2[j]);
                        break;
	}
                }
            }
	}
        if (_u4PinSharePeriod)
        {
//            Printf("nand sleep\n");
            x_thread_delay(_u4PinSharePeriod);
        }
    }
}
//-------------------------------------------------------------------------
/** _NANDCLI_PinShareStressTest
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NANDCLI_PinShareStressTest(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 1)
	{
        Printf("nand.pst \n");
		return 1;
	}
    if (x_thread_create(&_hNandPinShare, "NandPinShare", (16*1024), 255,
        _NANDPinShareThread, 0, NULL) != OSR_OK)
    {
        Printf("\nError: Create nand Pin share test thread fail\n");
		return 1;
	}
    return 0;
}
//-------------------------------------------------------------------------
/** _NORCLI_PinShareTestOption
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NANDCLI_PinShareTestOption(INT32 i4Argc, const CHAR ** szArgv)
	{
	if (i4Argc != 3)
	{
        Printf("nand.pso [read/write/both/null][delay(ms)]\n");
		return 1;
	}
    _u4PinShareOption = 0;
    if(x_strncmp(szArgv[1], "read", sizeof("read")) == 0)
    {
        _u4PinShareOption = 1;
    }
    if(x_strncmp(szArgv[1], "write", sizeof("write")) == 0)
    {
        _u4PinShareOption = 2;
    }
    if(x_strncmp(szArgv[1], "both", sizeof("both")) == 0)
    {
        _u4PinShareOption = 3;
    }
    _u4PinSharePeriod = StrToInt(szArgv[2]);
	return 0;
}
#endif
#if defined(CC_NAND_BOOT)
void CHIP_NANDCopyToDRAM(UINT32 *pu4DRAMBuf, UINT32 u4StartOffset, UINT32 u4Len);
void CHIP_NANDCopyToDRAMLZHS(UINT32 *pu4DstBuf, UINT32 u4ByteOffset , UINT32 u4Len);
//------------------------------------------------------------------------------
/** read fifo mode
 *
 */
//------------------------------------------------------------------------------
static INT32 _NANDCLI_ReadFIFOMode(INT32 i4Argc, const CHAR ** szArgv)
{

    UINT32 *pu4DRAMBuf = NULL;
    UINT32 u4StartOffset = 0;
    UINT32 u4Len = 0;

    if (i4Argc != 4)
    {
        LOG(3, "UINT32 *pu4DRAMBuf, UINT32 u4StartOffset, UINT32 u4Len\n");
    }
    pu4DRAMBuf = (UINT32 *)StrToInt(szArgv[1]);
    u4StartOffset = (UINT32)StrToInt(szArgv[2]);
    u4Len = (UINT32)StrToInt(szArgv[3]);
    CHIP_NANDCopyToDRAM(pu4DRAMBuf, u4StartOffset, u4Len);
    return 0;
}

//------------------------------------------------------------------------------
/** read fifo mode with bad block skip
 *
 */
//------------------------------------------------------------------------------
static INT32 _NANDCLI_ReadFIFOWithBadSkip(INT32 i4Argc, const CHAR ** szArgv)
    {
    UINT32 *pu4DstBuf = NULL;
    UINT32 u4ByteOffset = 0;
    UINT32 u4Len = 0;
    if (i4Argc != 4)
    {
        LOG(3, "UINT32 *pu4DRAMBuf, UINT32 u4StartOffset, UINT32 u4Len\n");
    }
    pu4DstBuf = (UINT32 *)StrToInt(szArgv[1]);
    u4ByteOffset = (UINT32)StrToInt(szArgv[2]);
    u4Len = (UINT32)StrToInt(szArgv[3]);
    CHIP_NANDCopyToDRAMLZHS(pu4DstBuf, u4ByteOffset , u4Len);
    return 0;
}
#endif  // #if defined(CC_NAND_BOOT)
#if !defined(CC_MTK_LOADER)
//-------------------------------------------------------------------------
static INT32 _NANDCLI_PodInit(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Timing;

    u4Timing = 0x8FFFFF;

    if(i4Argc == 2)
        u4Timing = StrToInt(szArgv[1]);

    if(NAND_PodInit(u4Timing) != 0)
    {
        Printf("Init fail!\n");
        return 1;
    }

    Printf("Init OK\n");

    return 0;
}


//-------------------------------------------------------------------------
static INT32 _NANDCLI_PodWrite(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr, u4Data;

    if(i4Argc != 3)
    {
        Printf("nand.pw [addr] [data]\n");
        return 1;
    }

    u4Addr = StrToInt(szArgv[1]);
    u4Data = StrToInt(szArgv[2]);

    if(NAND_PodWrite(u4Addr, u4Data) != 0)
    {
        Printf("Write data fail!\n");
        return 1;
    }

    Printf("Write data OK\n");

    return 0;
}


//-------------------------------------------------------------------------
static INT32 _NANDCLI_PodRead(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr, u4Data;

    if(i4Argc != 2)
    {
        Printf("nand.pr [addr]\n");
        return 1;
    }

    u4Addr = StrToInt(szArgv[1]);

    if(NAND_PodRead(u4Addr, &u4Data) != 0)
    {
        Printf("Read data fail!\n");
        return 1;
    }

    Printf("Read data: 0x%X\n", u4Data);

    return 0;
}


//-------------------------------------------------------------------------
static INT32 _NANDCLI_PodRegCompare(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 i, u4Times, u4Addr, u4Data, u4Data1;

    if(i4Argc != 4)
    {
        Printf("nand.prcmp [times] [addr] [data]\n");
        return 1;
    }

    u4Times = StrToInt(szArgv[1]);
    u4Addr = StrToInt(szArgv[2]);
    u4Data = StrToInt(szArgv[3]);

    for(i=0; i<u4Times; i++)
    {
        Printf("Loop: %d\n", i);
        if(NAND_PodWrite(u4Addr, u4Data) != 0)
        {
            Printf("Write data fail!\n");
            return 1;
        }

        if(NAND_PodRead(u4Addr, &u4Data1) != 0)
        {
            Printf("Read data fail!\n");
            return 1;
        }

        if(u4Data != u4Data1)
        {
            Printf("Register comapre error 0x%X != 0x%X\n", u4Data, u4Data1);
            return 1;
        }
        Printf("OK\n");
    }

    Printf("Register compare OK!\n");

    return 0;
}


//-------------------------------------------------------------------------
static INT32 _NANDCLI_PodBurstWrite(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4DRAMAddr, u4FlashAddr, u4ByteCount;

    if(i4Argc != 4)
    {
        Printf("nand.bw [d_addr] [f_addr] [bcount]\n");
        return 0;
    }

    u4DRAMAddr = StrToInt(szArgv[1]);
    u4FlashAddr = StrToInt(szArgv[2]);
    u4ByteCount = StrToInt(szArgv[3]);

    if(NAND_PodBurstWrite(u4DRAMAddr, u4FlashAddr, u4ByteCount) != 0)
    {
        Printf("POD burst write fail!\n");
        return 1;
    }

    Printf("POD burst write OK\n");

    return 0;
}


//-------------------------------------------------------------------------
static INT32 _NANDCLI_PodBurstRead(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4DRAMAddr, u4FlashAddr, u4ByteCount;

    if(i4Argc != 4)
    {
        Printf("nand.br [d_addr] [f_addr] [bcount]\n");
        return 0;
    }

    u4DRAMAddr = StrToInt(szArgv[1]);
    u4FlashAddr = StrToInt(szArgv[2]);
    u4ByteCount = StrToInt(szArgv[3]);

    if(NAND_PodBurstRead(u4DRAMAddr, u4FlashAddr, u4ByteCount) != 0)
    {
        Printf("POD burst write fail!\n");
        return 1;
    }

    Printf("POD burst read OK\n");

    return 0;
}


//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartInfo(INT32 i4Argc, const CHAR ** szArgv)
{
    NANDPART_ShowPartition();

    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartInit(INT32 i4Argc, const CHAR ** szArgv)
{
    return NANDPART_Init();
}
//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartMount(INT32 i4Argc, const CHAR ** szArgv)
{
    NANDPART_TYPE rType;
    UINT8 u1PartId;
    UINT32 u4Offset, u4Size;

    if(i4Argc != 5)
    {
        Printf("%s [part_id] [type] [offset] [size]\n", szArgv[0]);
        Printf("     type: fdm/bdm/sdm\n");
        return -1;
    }

    u1PartId = StrToInt(szArgv[1]);

    if(x_strncmp(szArgv[2], "fdm", sizeof("fdm")) == 0)
    {
        rType = NANDPART_FDM;
    }
    else if(x_strncmp(szArgv[2], "bdm", sizeof("bdm")) == 0)
    {
        rType = NANDPART_BDM;
    }
    else if(x_strncmp(szArgv[2], "sdm", sizeof("sdm")) == 0)
    {
        rType = NANDPART_SDM;
    }
    else
    {
        Printf("%s [part_id] [type]\n", szArgv[0]);
        Printf("     type: fdm/bdm/sdm\n");
        return -1;
    }

    u4Offset = StrToInt(szArgv[3]);
    u4Size = StrToInt(szArgv[4]);

    if(NANDPART_Mount(u1PartId, rType, u4Offset, u4Size) != 0)
    {
        Printf("Fail\n");
        return -1;
    }

    Printf("OK\n");

    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartUnMount(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1PartId;

    if(i4Argc != 2)
    {
        Printf("%s [part_id]\n", szArgv[0]);
        return -1;
    }

    u1PartId = StrToInt(szArgv[1]);

    if(NANDPART_UnMount(u1PartId) != 0)
    {
        Printf("Fail\n");
        return -1;
    }

    Printf("OK\n");

    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartRead(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MemPtr, u4NandOffset, u4ByteCount;
    UINT8 u1PartId;

    if(i4Argc != 5)
    {
        Printf("%s [part_id] [memptr] [nand_offset] [bytenum]\n", szArgv[0]);
        return -1;
    }

    u1PartId = StrToInt(szArgv[1]);
    u4MemPtr = StrToInt(szArgv[2]);
    u4NandOffset = StrToInt(szArgv[3]);
    u4ByteCount = StrToInt(szArgv[4]);

    if(NANDPART_IORequest(u1PartId, u4NandOffset, u4MemPtr, u4ByteCount, TRUE) != 0)
    {
        Printf("Fail!\n");
        return -1;
    }

    Printf("OK!\n");

    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartWrite(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MemPtr, u4NandOffset, u4ByteCount;
    UINT8 u1PartId;

    if(i4Argc != 5)
    {
        Printf("%s [part_id] [memptr] [nand_offset] [bytenum]\n", szArgv[0]);
        return -1;
    }

    u1PartId = StrToInt(szArgv[1]);
    u4MemPtr = StrToInt(szArgv[2]);
    u4NandOffset = StrToInt(szArgv[3]);
    u4ByteCount = StrToInt(szArgv[4]);

    if(NANDPART_IORequest(u1PartId, u4NandOffset, u4MemPtr, u4ByteCount, FALSE) != 0)
    {
        Printf("Fail!\n");
        return -1;
    }

    Printf("OK!\n");

    return 0;
}
//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartErase(INT32 i4Argc, const CHAR ** szArgv)
{
//    UINT32 u4MemPtr, u4NandOffset, u4ByteCount;
    UINT8 u1PartId;
    if(i4Argc != 2)
    {
        Printf("%s [part id]\n", szArgv[0]);
        return -1;
    }
    u1PartId = StrToInt(szArgv[1]);
    if(NANDPART_Erase(u1PartId) != 0)
    {
        Printf("partition%d erase Fail!\n", u1PartId);
        return -1;
    }
    Printf("part%d erase OK!\n", u1PartId);
    return 0;
}


//EXTERN INT32 NAND_SDM_DumpStatus(UINT8 u1PartId, UINT32 u4Flags);
//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartSDMDumpStatus(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 i, u4Flags;
    UINT8 u1id;

    if(i4Argc < 3)
    {
        Printf("%s partid [-a -e -b -u -l]\n", szArgv[0]);
        Printf("   -a: all, -e: empty, -b: bad, -u: used, -l: logic\n");
        return -1;
    }

    u1id = StrToInt(szArgv[1]);
    u4Flags = 0;
	for (i=2; i<i4Argc; i++)
	{
		if (szArgv[i][0]!='-')
		{
		    continue;
		}
		switch(szArgv[i][1])
		{
		case 'a':
		    u4Flags |= NAND_SDMDUMP_ALL;
    		break;
        case 'e':
            u4Flags |= NAND_SDMDUMP_EMPTY;
            break;
        case 'b':
            u4Flags |= NAND_SDMDUMP_BAD;
            break;
        case 'u':
            u4Flags |= NAND_SDMDUMP_USED;
            break;
        case 'l':
            u4Flags |= NAND_SDMDUMP_LOGIC;
            break;
        default:
            Printf("%s [-a -e -b -u -l]\n", szArgv[0]);
            Printf("   -a: all, -e: empty, -b: bad, -u: used, -l: logic\n");
            return -1;
        }
    }

    NAND_SDM_DumpStatus(u1id, u4Flags);

    return 0;
}


//EXTERN INT32 NAND_SDM_SyncCheck(void);
//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartSDMSyncCheck(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1ID;
    if(i4Argc != 2)
    {
        Printf("%s partid \n", szArgv[0]);
        return -1;
    }
    u1ID = StrToInt(szArgv[1]);
    NAND_SDM_SyncCheck(u1ID);

    return 0;
}


//EXTERN INT32 NAND_SDM_SyncStatus(void);
//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartSDMSyncStatus(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1ID;
    if(i4Argc != 2)
    {
        Printf("%s partid \n", szArgv[0]);
        return -1;
    }
    u1ID = StrToInt(szArgv[1]);
    NAND_SDM_SyncStatus(u1ID);

    return 0;
}

//EXTERN INT32 NAND_SDM_RWTest(UINT32 u4Count);
//EXTERN INT32 NAND_SDM_MountTest(UINT32 u4Count);
//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartSDMTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Count;

    if(i4Argc != 3)
    {
        Printf("%s [type] [loop]\n", szArgv[0]);
        Printf("    type: RW - read/write test\n");
        Printf("    type: Mount - mount/umount test\n");
        return -1;
    }

    u4Count = StrToInt(szArgv[2]);

    if(x_strncmp(szArgv[1], "RW", sizeof("RW")) == 0)
    {
        if(NANDPART_Mount(0, NANDPART_SDM, 0, 0x1000000) != 0)
        {
            return -1;
        }
        NAND_SDM_RWTest(u4Count);
        NANDPART_UnMount(0);
    }
    else if(x_strncmp(szArgv[1], "Mount", sizeof("Mount")) == 0)
    {
        NAND_SDM_MountTest(u4Count);
    }
    else
    {
        Printf("%s [type] [loop]\n", szArgv[0]);
        Printf("    type: RW - read/write test\n");
        Printf("    type: Mount - mount/umount test\n");
        return -1;
    }

    return 0;
}
#endif // #if !defined(CC_MTK_LOADER)


//-------------------------------------------------------------------------
//CLIMOD_DEBUG_FUNCTIONS(NAND)
////CLIMOD_REGTST_FUNCTIONS(NAND)
#if !defined(CC_MTK_LOADER)
static CLI_EXEC_T _arNandPodCmdTbl[] =
{
    {"podinit", "pi", _NANDCLI_PodInit, NULL, "nand.pi [timing]", CLI_GUEST },
    {"podwrite", "pw", _NANDCLI_PodWrite, NULL, "nand.pw [addr] [data]", CLI_GUEST },
    {"podread", "pr", _NANDCLI_PodRead, NULL, "nand.pr [addr]", CLI_GUEST },
    {"podrcmp", "rcmp", _NANDCLI_PodRegCompare, NULL, "nand.rcmp [times] [addr] [data]", CLI_GUEST },
    {"podbw", "bw", _NANDCLI_PodBurstWrite, NULL, "nand.bw [d_addr] [f_addr] [bcount]", CLI_GUEST },
    {"podbr", "br", _NANDCLI_PodBurstRead, NULL, "nand.br [d_addr] [f_addr] [bcount]", CLI_GUEST },

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arNandPartCmdTbl[] =
{
    {"info", "info", _NANDCLI_PartInfo, NULL, "info", CLI_GUEST },
    {"init", "init", _NANDCLI_PartInit, NULL, "init", CLI_GUEST },
    {"mount", "mount", _NANDCLI_PartMount, NULL, "mount [part_id] [type] [offset] [size]", CLI_GUEST },
    {"umount", "umount", _NANDCLI_PartUnMount, NULL, "umount [part_id]", CLI_GUEST },
    {"pread", "pr", _NANDCLI_PartRead, NULL, "pr [part_id] [memptr] [nand_offset] [bytenum]", CLI_GUEST },
    {"pwrite", "pw", _NANDCLI_PartWrite, NULL, "pw [part_id] [memptr] [nand_offset] [bytenum]", CLI_GUEST },
    {"perase", "pe", _NANDCLI_PartErase, NULL, "pe [part id] ", CLI_GUEST },
    {"sdmdumpstatus", "sds", _NANDCLI_PartSDMDumpStatus, NULL, "sds [part_id]", CLI_GUEST },
    {"sdmsynccheck", "sdsc", _NANDCLI_PartSDMSyncCheck, NULL, "sdsc [part_id]", CLI_GUEST },
    {"sdmsyncstatus", "sdss", _NANDCLI_PartSDMSyncStatus, NULL, "sdss [part_id]", CLI_GUEST },
    {"sdmtest", "test", _NANDCLI_PartSDMTest, NULL, "test", CLI_GUEST },

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif // #if !defined(CC_MTK_LOADER)
static CLI_EXEC_T arNandCmdTbl[] =
{
//	CLIMOD_DEBUG_CLIENTRY(NAND),
	////CLIMOD_REGTST_CLIENTRY(NAND),

#if 0
    DECLARE_CMD(_NANDCLI_Init, init, init, "nand.init"),
    DECLARE_CMD(_NANDCLI_Diag, diag, diag, "nand.diag"),

	DECLARE_CMD(_NANDCLI_Stop, stop, stop, "nand.stop"),
    DECLARE_CMD(_NANDCLI_Reset, reset, reset, "nand.reset"),

#if 0
    DECLARE_CMD(_NANDCLI_Enable, enable, en, "nand.en"),
    DECLARE_CMD(_NANDCLI_Disable, disable, dis, "nand.dis"),
#endif

    DECLARE_CMD(_NANDCLI_PatternTest, patterntest, pt, "nand.pt [burst/dmai/dmap] [addr] [size]"),
    DECLARE_CMD(_NANDCLI_AttrTest, attrtest, at, "nand.at"),
    DECLARE_CMD(_NANDCLI_State, state, st, "nand.st"),

    DECLARE_CMD(_NANDCLI_SetECC, setecc, secc, "nand.setecc [hw/sw/off]"),
#endif
    DECLARE_CMD(_NANDCLI_Read, read, r, "nand.read [memptr] [nand_offset] [bytenum]"),
    DECLARE_CMD(_NANDCLI_Write, write, wecc, "nand.write [memptr] [nand_offset] [bytenum]"),
#if 0
    DECLARE_CMD(_NANDCLI_RdAttr, rdattr, ra, "nand.ra [memptr] [nand_offset]"),
    DECLARE_CMD(_NANDCLI_WrAttr, wrattr, wa, "nand.wa [memptr] [nand_offset]"),
#endif
    DECLARE_CMD(_NANDCLI_Erase, erase, e, "nand.e [nand_offset] [bytenum]"),
    DECLARE_CMD(_NANDCLI_EraseForce, eraseforce, ef, "erase force, ef [nand_offset] [bytenum]"),
#if 0
    DECLARE_CMD(_NANDCLI_BlankCheck, blankcheck, bc, "nand.bc [nand_offset] [pagecount]"),
    DECLARE_CMD(_NANDCLI_BlankCheckAttr, blankcheckattr, bca, "nand.bca [nand_offset] [pagecount]"),
    DECLARE_CMD(_NANDCLI_IdDevice, id, id, "nand.id"),
#endif
    DECLARE_CMD(_NANDCLI_Measure, measure, m, "nand.m [nand_offset] [size]"),

    DECLARE_CMD(_NANDCLI_CheckBadBlk, bad, bad, "nand.bad"),

    DECLARE_CMD(_NANDCLI_MarkBadBlk, markbad, mbad, "nand.markbad [blk_idx]"),
#if 0
    DECLARE_CMD(_NANDCLI_ShowDevList, devlist, dlist, ""),
    DECLARE_CMD(_NANDCLI_PinShareStressTest, PinShareStressTest, pst, "nand.pst"),
    DECLARE_CMD(_NANDCLI_PinShareTestOption, PinShareTestOption, pso, "nand.pso [read/write/both][delay(ms)]"),
    DECLARE_CMD(_NANDCLI_EraseForce, eraseforce, ef, "nand.ef [nand_offset] [bytenum]"),
#endif

#if defined(CC_NAND_BOOT)
    DECLARE_CMD(_NANDCLI_ReadFIFOMode, rfifo, rfifo, "nand.rfifo [blk_idx]"),
    DECLARE_CMD(_NANDCLI_ReadFIFOWithBadSkip, rfifowb, rfwbad, "nand.rfifowb [blk_idx]"),
#endif  // #if defined(CC_NAND_BOOT)
#if !defined(CC_MTK_LOADER)
    {"pod", "pod", NULL, _arNandPodCmdTbl, "pod command", CLI_GUEST },
    {"partition", "part", NULL, _arNandPartCmdTbl, "partition command", CLI_GUEST },
#endif // #if !defined(CC_MTK_LOADER)

    DECLARE_END_ITEM(),
};



CLI_MAIN_COMMAND_ITEM(Nand)
{
	"nand",
	NULL,
	NULL,
	arNandCmdTbl,
	"Nand command",
	CLI_GUEST
};

