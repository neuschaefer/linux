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
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_os.h"
#include "x_drv_cli.h"
#define DEFINE_IS_LOG    CLI_IsLog
#include "x_debug.h"
#include "x_util.h"
#include "x_rand.h"
#include "x_bim.h"
#include "x_timer.h"
#include "nand_if.h"
#include "storage_if.h"

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define NAND_TESTBUF_SIZE       0x4000    // 16K bytes -> block size
#define NAND_TESTBUF1_ADDR      0x1000000
#define NAND_TESTBUF2_ADDR      0x2000000
#define NAND_TESTBUF3_ADDR      0x3000000

#define VERBOSE

//---------------------------------------------------------------------------
EXTERN VOID NOR_SemaLock(VOID);
EXTERN VOID NOR_SemaUnLock(VOID);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static HANDLE_T _hNandPinShare;
static UINT32 _u4PinShareOption = 1;
static UINT32 _u4PinSharePeriod = 500;
static UINT32 _u4MemoryUsage = 0;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
static INT32 _NANDCLI_Init(INT32 i4Argc, const CHAR **szArgv)
{
    NAND_Init();
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_Stop(INT32 i4Argc, const CHAR **szArgv)
{
    NAND_Stop();
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_Reset(INT32 i4Argc, const CHAR **szArgv)
{
    NAND_Reset();
    return 0;
}

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
static void _NAND_Ctrl_Status(NAND_CONTROL_T *prCtrl)
{
    Printf("%s Isr, %s DMA",
            prCtrl->fgIsr ? "Enable" : "Disable",
            prCtrl->fgDMA ? "Enable" : "Disable");

    switch (prCtrl->rEccType)
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
static INT32 _NANDCLI_State(INT32 i4Argc, const CHAR **szArgv)
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
    for (i = 1; i < (UINT32)i4Argc; i++)
    {
        if (szArgv[i][0] != '-')
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
    NAND_GetControl(u4Flag, &rCtrl);
    _NAND_Ctrl_Status(&rCtrl);

    return 0;
}

//-------------------------------------------------------------------------
static INT32 _DoPatternTest(UINT32 u4Start, UINT32 u4Size)
{
    UINT32 loop, u4Addr, u4SecId;
    NANDDEV_T rNandDev;
    UINT8 *TestBuf, *CompBuf;
    UINT32 u4BlkCount, u4BlkStart;

    TestBuf = (UINT8 *)NAND_TESTBUF1_ADDR;
    CompBuf = (UINT8 *)NAND_TESTBUF2_ADDR;

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("Get flash info error!!\n");
        return 1;
    }

    // Use block unit
    u4BlkStart = (u4Start & ~(rNandDev.u4BlkSz - 1)) / rNandDev.u4BlkSz;
    u4BlkCount = (u4Size + rNandDev.u4BlkSz - 1) / rNandDev.u4BlkSz;
    u4Addr = u4Start & ~(rNandDev.u4BlkSz - 1);
    u4SecId = u4BlkStart * (rNandDev.u4BlkSz / rNandDev.u4PgSz);
    for (loop = 0; loop < u4BlkCount; loop++)
    {
        if (NAND_Erase(u4SecId, 1) != 0)
        {
            Printf("Erase error!!!\n");
        }

        if (NAND_Write((UINT64)u4Addr, (UINT32)TestBuf, rNandDev.u4BlkSz) != 0)
        {
            Printf("Write error!!!\n");
        }

        x_memset((void *)CompBuf, 0xFF, rNandDev.u4BlkSz);
        if (NAND_Read((UINT64)u4Addr, (UINT32)CompBuf, rNandDev.u4BlkSz) != 0)
        {
            Printf("Read error!!!\n");
        }

        if (x_memcmp((void *)TestBuf, (void *)CompBuf, rNandDev.u4BlkSz) != 0)
        {
            Printf("Compare error!!\n");
        }

        u4Addr += rNandDev.u4BlkSz;
        u4SecId += (rNandDev.u4BlkSz / rNandDev.u4PgSz);

        if ((((u4BlkStart+loop+1) * rNandDev.u4BlkSz) % 0x100000) == 0)
        {
            Printf("NAND flash %03d MB test OK\n",
                   ((u4BlkStart+loop+1) * rNandDev.u4BlkSz) / 0x100000);
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
    for (i = 0; i < NAND_TESTBUF_SIZE; i++)
    {
        TestBuf[i] = 0x5A;
    }
    
    if( _DoPatternTest(u4Start, u4Size) != 0)
    {
        return -1;
    }

    Printf("Pattern: 2\n");
    for (i = 0; i < NAND_TESTBUF_SIZE; i++)
    {
        TestBuf[i] = 0xA5;
    }
    
    if (_DoPatternTest(u4Start, u4Size) != 0)
    {
        return -1;
    }

    Printf("Pattern: increase address\n");
    for (i = 0; i < NAND_TESTBUF_SIZE; i++)
    {
        TestBuf[i] = (UINT8)(i&0xFF);
    }
    
    if (_DoPatternTest(u4Start, u4Size) != 0)
    {
        return -1;
    }

    Printf("Pattern: random\n");
    HAL_GetRawTime(&H_RawTime) ;
    u4RandomSeed = H_RawTime.u4Cycles ;
    srand((unsigned int)u4RandomSeed );
    for (i = 0; i < NAND_TESTBUF_SIZE; i++)
    {
        TestBuf2[i] = (UINT32)rand();
    }
    
    if (_DoPatternTest(u4Start, u4Size) != 0)
    {
        return -1;
    }

    return 0;
}

//-------------------------------------------------------------------------
static void _NAND_PT_Usage(const char *pu1Cmd)
{
    Printf("%s [burst/dmap/dmai] [addr] [size]\n", pu1Cmd);
    Printf("   burst: Burst mode test\n");
    Printf("   dmap:  DMA polling mode test\n");
    Printf("   dmai:  DMA interrupt mode test\n");
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PatternTest(INT32 i4Argc, const CHAR **szArgv)
{
    NAND_CONTROL_T rOrgCtrl, rNewCtrl;
    UINT32 u4Start, u4Size;

    if (i4Argc != 4)
    {
        _NAND_PT_Usage(szArgv[0]);
        return 0;
    }

    u4Start = 0x0;
    u4Size = 0x2000000;
    NAND_GetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtrl);

    if (x_strncmp(szArgv[1], "burst", sizeof("burst")) == 0)
    {
        rNewCtrl.fgDMA = FALSE;
        rNewCtrl.fgIsr = FALSE;
    }
    else if (x_strncmp(szArgv[1], "dmai", sizeof("dmai")) == 0)
    {
        rNewCtrl.fgDMA = TRUE;
        rNewCtrl.fgIsr = TRUE;
    }
    else if (x_strncmp(szArgv[1], "dmap", sizeof("dmap")) == 0)
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

    NAND_SetControl((UINT32)(NAND_CTRL_FLAGS_ISR | NAND_CTRL_FLAGS_DMA), &rNewCtrl);
    if (_NANDCLI_Pattern(u4Start, u4Size) != 0)
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
    UINT8 *pu1GoldenPtr;
    NANDDEV_T rNandDev;

    pu4GoldenPtr = (UINT32*)u4GoldenMemAddr;
    pu4MemPtr = (UINT32*)u4TestMemAddr;
    pu1GoldenPtr = (UINT8*)u4GoldenMemAddr;
    
    NAND_GetInfo(&rNandDev);
    if (rNandDev.u4PgSz == 2048)
    {
        pu1GoldenPtr[0] = 0xFF;
        pu1GoldenPtr[1] = 0xFF;
    }
    else
    {
        pu1GoldenPtr[5] = 0xFF;
        pu1GoldenPtr[6] = 0xFF;
    }

    if (NAND_Erase(u4Offset/rNandDev.u4PgSz, 1) != 0)
    {
        Printf("Erase fail!\n");
        return 1;
    }

    x_memset(pu4MemPtr, 0x0, rNandDev.u4OOBSz);
    if (NAND_RdAttr(u4Offset, (UINT32)pu4MemPtr, (UINT32)rNandDev.u4OOBSz) != 0)
    {
        Printf("Erase read back fail!\n");
        return 1;
    }
    for (i = 0; i < (rNandDev.u4OOBSz/4); i++)
    {
        if (pu4MemPtr[i] != (UINT32)0xFFFFFFFF)
        {
            Printf("Erase check fail!\n");
            return 1;
        }
    }

    if (NAND_WrAttr(u4Offset, (UINT32)pu4GoldenPtr, rNandDev.u4OOBSz) != 0)
    {
        Printf("Write attr fail!\n");
        return 1;
    }
    
    x_memset(pu4MemPtr, 0x0, rNandDev.u4OOBSz);
    if (NAND_RdAttr(u4Offset, (UINT32)pu4MemPtr, rNandDev.u4OOBSz) != 0)
    {
        Printf("Read attr fail!\n");
        return 1;
    }

    if (x_memcmp(pu4MemPtr, pu4GoldenPtr, rNandDev.u4OOBSz) != 0)
    {
        Printf("Compare fail!\n");
        return 1;
    }

    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_AttrTest(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 i, *pu4GoldenPtr, *pu4TestPtr;

    pu4TestPtr   = (UINT32*)NAND_TESTBUF1_ADDR;
    pu4GoldenPtr = (UINT32*)NAND_TESTBUF2_ADDR;

    Printf("Pattern 1...\n");
    for (i = 0; i < (64/4); i++)
    {
        pu4GoldenPtr[i] = 0x5A5A5A5A;
    }
    if (_NAND_DoAttrTest(0, (UINT32)pu4GoldenPtr, (UINT32)pu4TestPtr)!= 0)
    {
        Printf("Attr test fail!\n");
        return 1;
    }
    Printf("OK\n");

    Printf("Pattern 2...\n");
    for (i = 0; i < (64/4); i++)
    {
        pu4GoldenPtr[i] = 0xA5A5A5A5;
    }
    if (_NAND_DoAttrTest(0x100000, (UINT32)pu4GoldenPtr, (UINT32)pu4TestPtr)!= 0)
    {
        Printf("Attr test fail!\n");
        return 1;
    }
    Printf("OK\n");

    Printf("Pattern 3...\n");
    for (i = 0; i < (64/4); i++)
    {
        pu4GoldenPtr[i] = 0x300000 + i<<4;
    }
    if (_NAND_DoAttrTest(0x200000, (UINT32)pu4GoldenPtr, (UINT32)pu4TestPtr)!= 0)
    {
        Printf("Attr test fail!\n");
        return 1;
    }
    
    Printf("OK\n");
    Printf("Attr test OK!\n");
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_Read(INT32 i4Argc, const CHAR **szArgv)
{
#ifdef VERBOSE
    UINT8 *ptr;
    INT32 i;
#endif
    INT32 i4Ret;
    UINT32 u4ByteCount, u4MemPtr, u4Offset;

    if (i4Argc < 4)
    {
        Printf("%s [memptr] [nand_offset] [bytenum]\n", szArgv[0]);
        return 1;
    }
    u4MemPtr = StrToInt(szArgv[1]);
    u4Offset = StrToInt(szArgv[2]);
    u4ByteCount = StrToInt(szArgv[3]);

    if (NAND_Read((UINT64)u4Offset, u4MemPtr, u4ByteCount) != 0)
    {
        i4Ret = 1;
        Printf("nand.read operates failed!\n");
    }
    else
    {
        i4Ret = 0;
        Printf("nand.read operates successfully.\n");
    }

#ifdef VERBOSE
    ptr = (UINT8 *)u4MemPtr;
    for (i = 0; i < 0x20; i++) 
    {
        if (i%16 == 0) 
        {
            Printf("\n%03x: %02x ", i, ptr[i]);
        }
        else 
        {
            Printf("%02x ", ptr[i]);
        }
    }
    Printf("\n");
#endif

    return i4Ret;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_Write(INT32 i4Argc, const CHAR **szArgv)
{
#ifdef VERBOSE
    UINT8 *ptr;
    INT32 i;
#endif
    INT32 i4Ret;
    UINT32 u4ByteCount, u4MemPtr, u4Offset;

    if (i4Argc < 4)
    {
        Printf("%s [memptr] [nand_offset] [bytenum]\n", szArgv[0]);
        return 1;
    }

    u4MemPtr = StrToInt(szArgv[1]);
    u4Offset = StrToInt(szArgv[2]);
    u4ByteCount = StrToInt(szArgv[3]);

    if (NAND_Write((UINT64)u4Offset, u4MemPtr, u4ByteCount) != 0)
    {
        i4Ret = 1;
        Printf("nand.write operates failed\n");
    }
    else
    {
        i4Ret = 0;
        Printf("nand.write operates successfully.\n");
    }

#ifdef VERBOSE
    ptr = (UINT8 *)u4MemPtr;
    for (i = 0; i < 0x20; i++) 
    {
        if (i%16 == 0) 
        {
            Printf("\n%03x: %02x ", i, ptr[i]);
        }
        else 
        {
            Printf("%02x ", ptr[i]);
        }
    }
    Printf("\n");
#endif

    return i4Ret;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_RdAttr(INT32 i4Argc, const CHAR **szArgv)
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
    for (i = 0; i < rNandDev.u4OOBSz; i++) 
    {
        if (i%16 == 0) 
        {
            Printf("\n%03x: %02x ", i, ptr[i]);
        }
        else 
        {
            Printf("%02x ", ptr[i]);
        }
    }
    
    Printf("\n");
    return i4Ret;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_WrAttr(INT32 i4Argc, const CHAR **szArgv)
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
    for (i = 0; i < rNandDev.u4OOBSz; i++) 
    {
        if (i%16 == 0) 
        {
            Printf("\n%03x: %02x ", i, ptr[i]);
        }
        else 
        {
            Printf("%02x ", ptr[i]);
        }
    }

    Printf("\n");
    return i4Ret;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_Erase(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Offset, u4ByteCount;
    UINT32 u4PgStart, u4PgEnd, u4PgCount;
    UINT32 u4BlkStart, u4BlkEnd;
    NANDDEV_T rNandDev;

    if (i4Argc < 3)
    {
        Printf("%s [nand_offset] [bytelen]\n", szArgv[0]);
        return 1;
    }

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("NAND_GetInfo failed\n");
        return 1;
    }

    u4Offset = StrToInt(szArgv[1]);
    u4ByteCount = StrToInt(szArgv[2]);

    if (u4ByteCount == 0)
    {
        Printf("Erase size is zero!\n");
        return 0;
    }

    u4PgStart = u4Offset / rNandDev.u4PgSz;
    u4PgEnd = (u4Offset + u4ByteCount - 1) / rNandDev.u4PgSz;
    u4PgCount = u4PgEnd - u4PgStart + 1;

    u4BlkStart = u4Offset / rNandDev.u4BlkSz;
    u4BlkEnd = (u4Offset + u4ByteCount - 1) / rNandDev.u4BlkSz;

    Printf("Starting Erase from block: %d ~ %d.\n", u4BlkStart, u4BlkEnd);
    NAND_Erase(u4PgStart, u4PgCount);
    Printf("nand.e operates Successfully.\n");

    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_EraseForce(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Offset, u4ByteCount;
    UINT32 u4PgStart, u4PgEnd, u4PgCount;
    UINT32 u4BlkStart, u4BlkEnd;
    NANDDEV_T rNandDev;
    
    if (i4Argc < 3)
    {
        Printf("%s [nand_offset] [bytelen]\n", szArgv[0]);
        return 1;
    }
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("NAND_GetInfo failed\n");
        return 1;
    }
    
    u4Offset = StrToInt(szArgv[1]);
    u4ByteCount = StrToInt(szArgv[2]);
    
    if (u4ByteCount == 0)
    {
        Printf("Erase size is zero!\n");
        return 0;
    }
    
    u4PgStart = u4Offset / rNandDev.u4PgSz;
    u4PgEnd = (u4Offset + u4ByteCount - 1) / rNandDev.u4PgSz;
    u4PgCount = u4PgEnd - u4PgStart + 1;

    u4BlkStart = u4Offset / rNandDev.u4BlkSz;
    u4BlkEnd = (u4Offset + u4ByteCount - 1) / rNandDev.u4BlkSz;
    
    Printf("Starting EraseForce from block: %d ~ %d.\n", u4BlkStart, u4BlkEnd);
    if (NAND_EraseForce(u4PgStart, u4PgCount) != 0)
    {
        Printf("nand.ef operates failed!\n");
        return 1;
    }

    Printf("nand.ef operates successfully.\n");
    return 0;
}

static INT32 _NANDCLI_SetMemoryUsage(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4MemoryUsage;
    
    if (i4Argc < 2)
    {
        Printf("%s [0|1] 1:Local Memory, 0: Golbal Memory(0x1000000)\n", szArgv[0]);
        return 1;
    }

    u4MemoryUsage = StrToInt(szArgv[1]);
    if ( u4MemoryUsage == 1 || u4MemoryUsage == 0 )
    {
        _u4MemoryUsage = u4MemoryUsage;
    }
    else
    {
        Printf("Value should be 1 or 0\n");
    }

    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_BlankCheck(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Offset, u4PageCount;

    if (i4Argc != 3)
    {
        Printf("%s [nand_offset] [bytenum]\n", szArgv[0]);
        return 1;
    }

    u4Offset = StrToInt(szArgv[1]);
    u4PageCount = StrToInt(szArgv[2]);

    if (NAND_BlankCheck((UINT64)u4Offset, u4PageCount) != 0)
    {
        Printf("NAND blank check fail!\n");
        return 1;
    }

    Printf("Blank check OK!\n");
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_BlankCheckAttr(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Offset, u4PageCnt;

    if (i4Argc != 3)
    {
        Printf("%s [nand_offset] [pagecount]\n", szArgv[0]);
        return 1;
    }

    u4Offset = StrToInt(szArgv[1]);
    u4PageCnt = StrToInt(szArgv[2]);

    if (NAND_BlankCheckAttr((UINT64)u4Offset, u4PageCnt) != 0)
    {
        Printf("NAND blank check fail!\n");
        return 1;
    }

    Printf("Blank check OK!\n");
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_IdDevice(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 i4Ret;
    UINT32 u4ID;

    i4Ret = NAND_ReadID(&u4ID);
    if (i4Ret)
    {
        Printf("NAND_ReadID failed\n");
        return 1;
    }

    Printf("NAND Device ID: 0x%X\n", u4ID);
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_Measure(INT32 i4Argc, const CHAR **szArgv)
{
    HAL_TIME_T time1, time2, result_time;
    UINT32 u4Micros1, u4Micros2, u4Micros3;
    UINT32 u4TestAddr, u4Len;
    UINT32 u4PgIdx, u4PgCount;
    NANDDEV_T rNandDev;
    UINT32 i, j;
    static UINT8 *pu1TestBuf = NULL, *pu1GoldenBuf = NULL;

    if (i4Argc != 3)
    {
        Printf("%s [nand_offset] [size]\n", szArgv[0]);
        return -1;
    }

    u4TestAddr = StrToInt(szArgv[1]);
    u4Len = StrToInt(szArgv[2]);

    if (u4Len > 5*1024*1024)
    {
        Printf("Test size shoule not more than 5M!\n");
        return -1;
    }

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("NAND_GetInfo failed\n");
        return -1;
    }

    if (pu1TestBuf == NULL)
    {
        pu1TestBuf = x_mem_alloc(5*1024*1024);
    }

    if (pu1GoldenBuf == NULL)
    {
        pu1GoldenBuf = x_mem_alloc(5*1024*1024);
    }

    if (pu1TestBuf == NULL || pu1GoldenBuf == NULL)
    {
        ASSERT(0);
        return -1;
    }
    
    for (i = 0; i < u4Len; i += 0x200)
    {
        if (i < 0x200000)
        {
            x_memset(&pu1GoldenBuf[i], 0xAA, 0x200);
        }
        else if (i < 0x400000)
        {
            x_memset(&pu1GoldenBuf[i], 0x55, 0x200);
        }
        else if (i < 0x600000)
        {
            x_memset(&pu1GoldenBuf[i], 0x0F, 0x200);
        }
        else
        {
            for(j = i; j < (i+0x200); j++)
            {
                pu1GoldenBuf[j] = (UINT8)(j&0xFF);
            }
        }
        pu1GoldenBuf[i + (((i & 0x1FFFFF) / 0x200)>>3)] ^= (1<<(((i & 0x1FFFFF) / 0x200) & 0x7));
    }

    u4PgIdx = u4TestAddr / rNandDev.u4PgSz;
    u4PgCount = u4Len / rNandDev.u4PgSz;

    // Erase time
    Printf("Measure erase time:\n");
    HAL_GetTime(&time1);
    NAND_Erase(u4PgIdx, u4PgCount);
    HAL_GetTime(&time2);
    HAL_GetDeltaTime(&result_time, &time1, &time2);
    u4Micros1 = result_time.u4Seconds * 1000000 + result_time.u4Micros;
    Printf("Erase 0x%X elapse time: %d us\n", u4Len, u4Micros1);
    
    // Write time
    Printf("Measure write time:\n");
    HAL_GetTime(&time1);
    if (NAND_Write((UINT64)u4TestAddr, (UINT32)pu1GoldenBuf, u4Len) != 0)
    {
        Printf("Write NAND flash error!\n");
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
    if (NAND_Read((UINT64)u4TestAddr, (UINT32)pu1TestBuf, u4Len) != 0)
    {
        Printf("Read NAND flash error!\n");
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
            Printf("Read compare fail at offset 0x%X!\n", i);
            
#ifdef VERBOSE
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
#endif
            return -1;
        }
    }
    
    return 0;
}

static INT32 _NANDCLI_MeasureAdvanced(INT32 i4Argc, const CHAR **szArgv)
{
    HAL_TIME_T time1, time2, result_time;
    UINT32 u4Micros1, u4Micros2, u4Micros3, u4Micros4;
    UINT32 u4TestAddr, u4Len;
    UINT32 u4PgIdx, u4PgCount;
    NANDDEV_T rNandDev;
    UINT32 i;
    UINT8 *pu1TestBuf, *pu1GoldenBuf;
    UINT8 *pu1TestBufAlign, *pu1GoldenBufAlign;
    NAND_CONTROL_T rOrgCtrl, rNewCtrl;
    UINT32 u4TestBufSize = 128*1024;
    UINT32 u4TestCount, u4TestLen;

    if (i4Argc != 3)
    {
        Printf("%s [nand_offset] [size]\n", szArgv[0]);
        return -1;
    }

    u4TestAddr = StrToInt(szArgv[1]);
    u4Len = StrToInt(szArgv[2]);

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("NAND_GetInfo failed\n");
        return 1;
    }

    if (_u4MemoryUsage == 0)
    {
        pu1GoldenBuf = (UINT8*)NAND_TESTBUF1_ADDR;
        pu1TestBuf = (UINT8*)NAND_TESTBUF2_ADDR;
        pu1GoldenBufAlign = pu1GoldenBuf;
        pu1TestBufAlign = pu1TestBuf;
        u4TestLen = u4Len;
    }
    else
    {
        pu1TestBuf = (UINT8*)x_mem_alloc(u4TestBufSize+128*1024);
        if (pu1TestBuf == 0)
        {
            Printf("Melloc Failed\n");
            return -1;
        }
        pu1GoldenBuf = (UINT8*)x_mem_alloc(u4TestBufSize+128*1024);
        if (pu1GoldenBuf == 0)
        {
            Printf("Melloc Failed\n");
            x_mem_free((VOID*)pu1TestBuf);
            return -1;
        }
        u4TestLen = u4TestBufSize;
        pu1GoldenBufAlign = pu1GoldenBuf;
        pu1TestBufAlign = pu1TestBuf;
        if ((INT32)pu1TestBufAlign % (128*1024) != 0)
        {
            pu1TestBufAlign += (128*1024) - ((INT32)pu1TestBufAlign % (128*1024));
        }
        if ((INT32)pu1GoldenBufAlign % (128*1024) != 0)
        {
            pu1GoldenBufAlign += (128*1024) - ((INT32)pu1GoldenBufAlign % (128*1024));
        }
    }

    // Backup org control
    NAND_GetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtrl);

    // Set new control
    rNewCtrl.fgIsr = FALSE;
    rNewCtrl.fgDMA = TRUE;
    rNewCtrl.rEccType = NAND_ECC_HARD;
    NAND_SetControl(NAND_CTRL_FLAGS_ALL, &rNewCtrl);

    u4PgIdx = u4TestAddr / rNandDev.u4PgSz;
    u4PgCount = u4Len / rNandDev.u4PgSz;
    u4TestCount = u4Len / u4TestBufSize;

    // Erase time
    Printf("Measure erase time:\n");
    HAL_GetTime(&time1);
    NAND_Erase(u4PgIdx, u4PgCount);
    HAL_GetTime(&time2);
    HAL_GetDeltaTime(&result_time, &time1, &time2);
    u4Micros1 = result_time.u4Seconds * 1000000 + result_time.u4Micros;

    // Write time,
    Printf("Measure write time:\n");
    for (i = 0 ; i < u4TestLen ; i++)
    {
        pu1GoldenBufAlign[i] = 0xaa;
    }

    HAL_GetTime(&time1);
    if (_u4MemoryUsage == 0)
    {
        if (NAND_Write((UINT64)u4TestAddr, (UINT32)pu1GoldenBufAlign, u4TestLen) != 0)
        {
            Printf("Write NAND flash error!\n");
            NAND_SetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtrl);
            goto MEASURE_ERROR;
        }
    }
    else
    {
        for (i = 0 ; i < u4TestCount ; i++)
        {
            if (NAND_IsBadBlk(u4TestAddr/rNandDev.u4BlkSz))
            {
                Printf("Skip bad block !\n");
                u4TestAddr+=rNandDev.u4BlkSz;
                i--;
                continue;
            }
            if (NAND_Write((UINT64)u4TestAddr, (UINT32)pu1GoldenBufAlign, u4TestLen) != 0)
            {
                Printf("Write NAND flash error!\n");
                NAND_SetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtrl);
                goto MEASURE_ERROR;
            }
            u4TestAddr+=rNandDev.u4BlkSz;
        }
    }
    HAL_GetTime(&time2);
    HAL_GetDeltaTime(&result_time, &time1, &time2);
    u4Micros2 = result_time.u4Seconds * 1000000 + result_time.u4Micros;

    // Read time
    u4TestAddr = StrToInt(szArgv[1]);
    Printf("Measure read time:\n");
    for (i = 0 ; i < u4TestLen ; i++)
    {
        pu1TestBufAlign[i] = 0xbb;
    }

    HAL_GetTime(&time1);
    if (_u4MemoryUsage == 0)
    {
        if (NAND_Read((UINT64)u4TestAddr, (UINT32)pu1TestBufAlign, u4TestLen) != 0)
        {
            if (NAND_Read((UINT64)u4TestAddr, (UINT32)pu1TestBufAlign, u4TestLen) != 0)
            {
                if (NAND_Read((UINT64)u4TestAddr, (UINT32)pu1TestBufAlign, u4TestLen) != 0)
                {
                    u4TestAddr += rNandDev.u4BlkSz;
                }
            }
        }
    }
    else
    {
        for (i = 0 ; i < u4TestCount ; i++)
        {
            if (NAND_IsBadBlk(u4TestAddr/rNandDev.u4BlkSz))
            {
                Printf("Skip bad block !\n");
                u4TestAddr+=rNandDev.u4BlkSz;
                i--;
                continue;
            }
            
            if (NAND_Read((UINT64)u4TestAddr, (UINT32)pu1TestBufAlign, u4TestLen) != 0)
            {
                if (NAND_Read((UINT64)u4TestAddr, (UINT32)pu1TestBufAlign, u4TestLen) != 0)
                {
                    if (NAND_Read((UINT64)u4TestAddr, (UINT32)pu1TestBufAlign, u4TestLen) != 0)
                    {
                        Printf("Read NAND flash error!\n");
                        u4TestAddr+=rNandDev.u4BlkSz;
                        continue;
                    }
                }
            }
            u4TestAddr+=rNandDev.u4BlkSz;
        }
    }
    
    HAL_GetTime(&time2);
    HAL_GetDeltaTime(&result_time, &time1, &time2);
    u4Micros3 = result_time.u4Seconds * 1000000 + result_time.u4Micros;

    // erase + write + read
    u4TestAddr = StrToInt(szArgv[1]);
    Printf("Measure erase+write+read time:\n");
    for (i = 0 ; i < u4TestLen ; i++)
    {
        pu1TestBufAlign[i] = 0xcc;
        pu1GoldenBufAlign[i] = 0xdd;
    }
    
    HAL_GetTime(&time1);
    if (_u4MemoryUsage == 0)
    {
        u4PgCount = u4Len / rNandDev.u4PgSz;
        NAND_Erase(u4PgIdx, u4PgCount);

        if (NAND_Write((UINT64)u4TestAddr, (UINT32)pu1GoldenBufAlign, u4TestLen) != 0)
        {
            Printf("Write NAND flash error!\n");
            NAND_SetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtrl);
            goto MEASURE_ERROR;
        }

        if (NAND_Read((UINT64)u4TestAddr, (UINT32)pu1TestBufAlign, u4TestLen) != 0)
        {
            if (NAND_Read((UINT64)u4TestAddr, (UINT32)pu1TestBufAlign, u4TestLen) != 0)
            {
                if (NAND_Read((UINT64)u4TestAddr, (UINT32)pu1TestBufAlign, u4TestLen) != 0)
                {
                    u4TestAddr+=rNandDev.u4BlkSz;
                }
            }
        }
    }
    else
    {
        u4PgCount = rNandDev.u4BlkSz / rNandDev.u4PgSz;

        for (i = 0 ; i < u4TestCount ; i++)
        {
            if (NAND_IsBadBlk(u4TestAddr/rNandDev.u4BlkSz))
            {
                Printf("Skip bad block!\n");
                u4TestAddr+=rNandDev.u4BlkSz;
                i--;
                continue;
            }

            u4PgIdx = u4TestAddr / rNandDev.u4PgSz;
            NAND_Erase(u4PgIdx, u4PgCount);

            if (NAND_Write((UINT64)u4TestAddr, (UINT32)pu1GoldenBufAlign, u4TestLen) != 0)
            {
                Printf("Write NAND flash error!\n");
                NAND_SetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtrl);
                goto MEASURE_ERROR;
            }

            if (NAND_Read((UINT64)u4TestAddr, (UINT32)pu1TestBufAlign, u4TestLen) != 0)
            {
                if (NAND_Read((UINT64)u4TestAddr, (UINT32)pu1TestBufAlign, u4TestLen) != 0)
                {
                    if (NAND_Read((UINT64)u4TestAddr, (UINT32)pu1TestBufAlign, u4TestLen) != 0)
                    {
                        Printf("Read NAND flash error!\n");
                        u4TestAddr+=rNandDev.u4BlkSz;
                        continue;
                    }
                }
            }
            u4TestAddr+=rNandDev.u4BlkSz;
        }
    }
    
    HAL_GetTime(&time2);
    HAL_GetDeltaTime(&result_time, &time1, &time2);
    u4Micros4 = result_time.u4Seconds * 1000000 + result_time.u4Micros;

    Printf("Erase %dB elapse time: %d\n", u4Len, u4Micros1);
    Printf("Write %dB elapse time: %d\n", u4Len, u4Micros2);
    Printf("Read  %dB elapse time: %d\n", u4Len, u4Micros3);
    Printf("Erase+Write+Read  %dB elapse time: %d\n", u4Len, u4Micros4);

    for (i = 0 ; i < u4TestLen ; i++)
    {
        if (pu1TestBufAlign[i] != pu1GoldenBufAlign[i])
        {
            Printf("Compare Result Failed!\n");
            break;
        }
    }

    NAND_SetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtrl);

MEASURE_ERROR:
    if (_u4MemoryUsage == 1)
    {
        x_mem_free((VOID*)pu1TestBuf);
        x_mem_free((VOID*)pu1GoldenBuf);
    }
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_CheckBadBlk(INT32 i4Argc, const CHAR **szArgv)
{
    NANDDEV_T rNandDev;
    UINT32 i;

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("NAND_GetInfo fail!\n");
        return 1;
    }

    Printf("Bad block:\n");
    for (i = 0; i < rNandDev.u4TotalBlk; i++)
    {
        if(NAND_IsBadBlk(i))
        {
            Printf(" blk idx:%d, offset:0x%x\n", i, i * rNandDev.u4BlkSz);
        }
    }

    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_MarkBadBlk(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Blk;
    NANDDEV_T rNandDev;

    if (i4Argc != 2)
    {
        Printf("%s [blk_idx]\n", szArgv[0]);
        return -1;
    }
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("NAND_GetInfo fail!\n");
        return 1;
    }

    u4Blk = StrToInt(szArgv[1]);
    if (!NAND_MarkBadBlk(u4Blk))
    {
        Printf("Fail\n");
        return -1;
    }

    Printf("OK\n");
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_ShowDevList(INT32 i4Argc, const CHAR **szArgv)
{
    NAND_ShowDevList();
    return 0;
}

//-------------------------------------------------------------------------
/** _NANDPinShareThread
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
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("NAND_GetInfo fail!\n");
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
    rNewCtrl.rEccType = NAND_ECC_SOFT;
    NAND_SetControl(NAND_CTRL_FLAGS_ALL, &rNewCtrl);
    
    Printf("_NANDPinShareTest start\n");
    while(1)
    {
        for (i = (rNandDev.u4TotalBlk-100); i < rNandDev.u4TotalBlk; i++)
        {
            if ((_u4PinShareOption & 3) == 3)
            {
                NOR_SemaLock();
                if (NAND_Erase((i*rNandDev.u4PgPerBlk), rNandDev.u4PgPerBlk))
                {
                    Printf("nand erase fail at:0x%x\n", i);
                }
                NOR_SemaUnLock();
            }
            
            if ((_u4PinShareOption & 2))
            {
                for (j = 0; j < rNandDev.u4BlkSz; j++)
                {
                    pu1MemBuf1[j] = (UINT8)((i+j)&0xFF);
                }
                NOR_SemaLock();
                if (NAND_Write((i*rNandDev.u4BlkSz), u4MemPtr1, rNandDev.u4BlkSz))
                {
                    Printf("nand write fail at:0x%x\n", i);
                }
                NOR_SemaUnLock();
            }
            
            if ((_u4PinShareOption & 1))
            {
                NOR_SemaLock();
                if (NAND_Read((i*rNandDev.u4BlkSz), u4MemPtr2, rNandDev.u4BlkSz))
                {
                    Printf("nand read fail at:0x%x\n", i);
                }
                NOR_SemaUnLock();
            }
            
            if ((_u4PinShareOption & 3) == 3)
            {
                for (j = 0; j < rNandDev.u4BlkSz; j++)
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
static INT32 _NANDCLI_PinShareStressTest(INT32 i4Argc, const CHAR **szArgv)
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
/** _NANDCLI_PinShareTestOption
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NANDCLI_PinShareTestOption(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 3)
    {
        Printf("nand.pso [read/write/both/null][delay(ms)]\n");
        return 1;
    }

    _u4PinShareOption = 0;
    if (x_strncmp(szArgv[1], "read", sizeof("read")) == 0)
    {
        _u4PinShareOption = 1;
    }
    
    if (x_strncmp(szArgv[1], "write", sizeof("write")) == 0)
    {
        _u4PinShareOption = 2;
    }
    
    if (x_strncmp(szArgv[1], "both", sizeof("both")) == 0)
    {
        _u4PinShareOption = 3;
    }

    _u4PinSharePeriod = StrToInt(szArgv[2]);
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of _NAND_SDM_RWTest.
 *  Details of _NAND_SDM_RWTest (optional).
 */
//-----------------------------------------------------------------------------
static INT32 _NAND_SDM_RWTest(UINT32 u4Count)
{
    UINT32 *pu4TestBuf, *pu4GoldBuf;
    UINT32 i, j, idx, k;
    UINT32 u4RandomSeed;
    HAL_RAW_TIME_T H_RawTime;
    NANDDEV_T rNandDev;
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        return 1;
    }
    
    pu4TestBuf = (UINT32 *)x_mem_alloc_virtual(0x200000);
    pu4GoldBuf = (UINT32 *)x_mem_alloc_virtual(0x200000);
    if (pu4TestBuf == NULL || pu4GoldBuf == NULL)
    {
      return 1;
    }
    
    // Fill all buffer
    srand(REG_RW_TIMER0_LOW);
    for (i = 0; i < (0x200000/4); i++)
    {
        pu4GoldBuf[i] = (UINT32)rand();
    }

    for (i = 0; i < u4Count; i++)
    {
        idx = (UINT32)(rand()%(rNandDev.u4TotalBlk-100));
        Printf("%d: Blk 0x%x\n", i, idx);
        HAL_GetRawTime(&H_RawTime) ;
        u4RandomSeed = H_RawTime.u4Cycles ;
        srand((unsigned int)u4RandomSeed );
        
        for (j = 0; j < (0x200000/4); j++)
        {
            pu4GoldBuf[j] = (UINT32)rand();
        }
        
        if (NANDPART_IORequest(0, idx * rNandDev.u4BlkSz, (UINT32)(pu4GoldBuf) , rNandDev.u4BlkSz, FALSE) != 0)
        {
            LOG(3, "Test write error!\n");
            x_mem_free((void *)pu4TestBuf);
            x_mem_free((void *)pu4GoldBuf);
            return -1;
        }
        
        if (NANDPART_IORequest(0, idx * rNandDev.u4BlkSz, (UINT32)pu4TestBuf, rNandDev.u4BlkSz, TRUE) != 0)
        {
            LOG(3, "Test read error!\n");
            x_mem_free((void *)pu4TestBuf);
            x_mem_free((void *)pu4GoldBuf);
            return -1;
        }
        
        for (k = 0; k < (rNandDev.u4BlkSz/4); k++)
        {
            if (pu4TestBuf[k] != pu4GoldBuf[k])
            {
                LOG(3, "Compare error!\n");
                x_mem_free((void *)pu4TestBuf);
                x_mem_free((void *)pu4GoldBuf);
                return -1;
            }
        }
    }
    
    Printf("Test OK\n");
    x_mem_free((void *)pu4TestBuf);
    x_mem_free((void *)pu4GoldBuf);
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of _NAND_SDM_MountTest.
 *  Details of _NAND_SDM_MountTest (optional).
 */
//-----------------------------------------------------------------------------
static INT32 _NAND_SDM_MountTest(UINT32 u4Count)
{
    UINT32 *pu4TestBuf, *pu4GoldBuf;
    UINT32 i, k;
    
    pu4TestBuf = (UINT32 *)x_mem_alloc_virtual(0x80000);
    pu4GoldBuf = (UINT32 *)x_mem_alloc_virtual(0x80000);
    if (pu4TestBuf == NULL || pu4GoldBuf == NULL)
    {
        return 1;
    }
    
    // Fill all buffer
    srand(REG_RW_TIMER0_LOW);
    for (i = 0; i < (0x80000/4); i++)
    {
        pu4GoldBuf[i] = (UINT32)rand();
    }
    
    if (NANDPART_Mount(0, NANDPART_SDM, 0x1000000, 0x1000000) != 0)
    {
        LOG(3, "Mount fail\n");
        x_mem_free((void *)pu4TestBuf);
        x_mem_free((void *)pu4GoldBuf);
        return -1;
    }
    
    // Fill first golden to NAND
    if (NANDPART_IORequest(0, 0x0, (UINT32)pu4GoldBuf, 0x80000, FALSE) != 0)
    {
        LOG(3, "Test write error!\n");
        return -1;
    }
    
    if (NANDPART_UnMount(0) != 0)
    {
        LOG(3, "Unmount fail\n");
        return -1;
    }
    
    for (i = 0; i < u4Count; i++)
    {
        Printf("loop: %d\n", i);
        if (NANDPART_Mount(0, NANDPART_SDM, 0x1000000, 0x1000000) != 0)
        {
            LOG(3, "Mount fail\n");
            x_mem_free((void *)pu4TestBuf);
            x_mem_free((void *)pu4GoldBuf);
            return -1;
        }
        
        if (NANDPART_IORequest(0, 0x0, (UINT32)pu4TestBuf, 0x80000, TRUE) != 0)
        {
            LOG(3, "Test read error!\n");
            x_mem_free((void *)pu4TestBuf);
            x_mem_free((void *)pu4GoldBuf);
            NANDPART_UnMount(0);
            return -1;
        }
        
        for (k = 0; k < (0x80000/4); k++)
        {
            if(pu4TestBuf[k] != pu4GoldBuf[k])
            {
                LOG(3, "Compare ERROR_READWRITE_TEST!\n");
                x_mem_free((void *)pu4TestBuf);
                x_mem_free((void *)pu4GoldBuf);
                NANDPART_UnMount(0);
                return -1;
            }
        }
        
        if (NANDPART_UnMount(0) != 0)
        {
            LOG(3, "Unmount fail\n");
            return -1;
        }
    }
    
    Printf("Test OK\n");
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartInfo(INT32 i4Argc, const CHAR **szArgv)
{
    NANDPART_ShowPartition();
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartInit(INT32 i4Argc, const CHAR **szArgv)
{
    return NANDPART_Init();
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartMount(INT32 i4Argc, const CHAR **szArgv)
{
    NANDPART_TYPE rType;
    UINT8 u1PartId;
    UINT32 u4Offset, u4Size;

    if (i4Argc != 5)
    {
        Printf("%s [part_id] [type] [offset] [size]\n", szArgv[0]);
        Printf("     type: bdm/sdm\n");
        return -1;
    }

    u1PartId = (UINT8)StrToInt(szArgv[1]);

    if (x_strncmp(szArgv[2], "bdm", sizeof("bdm")) == 0)
    {
        rType = NANDPART_BDM;
    }
    else if (x_strncmp(szArgv[2], "sdm", sizeof("sdm")) == 0)
    {
        rType = NANDPART_SDM;
    }
    else
    {
        Printf("%s [part_id] [type]\n", szArgv[0]);
        Printf("     type: bdm/sdm\n");
        return -1;
    }

    u4Offset = StrToInt(szArgv[3]);
    u4Size = StrToInt(szArgv[4]);

    if (NANDPART_Mount(u1PartId, rType, u4Offset, u4Size) != 0)
    {
        Printf("Fail\n");
        return -1;
    }

    Printf("OK\n");
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartUnMount(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1PartId;

    if (i4Argc != 2)
    {
        Printf("%s [part_id]\n", szArgv[0]);
        return -1;
    }

    u1PartId = (UINT8)StrToInt(szArgv[1]);

    if (NANDPART_UnMount(u1PartId) != 0)
    {
        Printf("Fail\n");
        return -1;
    }

    Printf("OK\n");
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartReadWriteTest(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1PartId;
    UINT8* u4MemPtr1;
    UINT8* u4MemPtr2;
    UINT32 i,j,k;
    UINT32 u4aTestSize[5] = {512, 1024, 1536, 2048, 4096};
    UINT32 u4Counter = 0;
    UINT32 u4Offset = 0;

    if (i4Argc != 2)
    {
        Printf("%s [part_id]\n", szArgv[0]);
        return -1;
    }

    u1PartId = (UINT8)StrToInt(szArgv[1]);
    u4MemPtr1 = (UINT8 *)0x1000000;
    u4MemPtr2 = (UINT8 *)0x2000000;

    // Write + Read + Compare
    for (i = 0; i < 5; i++)
    {
        x_memset((VOID*)u4MemPtr1, (UINT8)(u4Counter%255), u4aTestSize[i]);
        x_memset((VOID*)u4MemPtr2, 0, u4aTestSize[i]);
        if (NANDPART_IORequest(u1PartId, u4Offset, (UINT32)u4MemPtr1, u4aTestSize[i], FALSE) != 0)
        {
            Printf("Test1: Write %dB Failed !\n", u4aTestSize[i]);
            goto ERROR_READWRITE_TEST;
        }

        if (NANDPART_IORequest(u1PartId, u4Offset, (UINT32)u4MemPtr2, u4aTestSize[i], TRUE) != 0)
        {
            Printf("Test1: Read %dB Failed !\n", u4aTestSize[i]);
            goto ERROR_READWRITE_TEST;
        }

        for (j = 0 ; j < u4aTestSize[i] ; j++)
        {
            if (u4MemPtr1[j] != u4MemPtr2[j])
            {
                Printf("Test1: Compare %dB Failed !\n", u4aTestSize[i]);
                goto ERROR_READWRITE_TEST;
            }
        }
        u4Counter++;
        Printf("Test1: Write/Read %dB OK.\n", u4aTestSize[i]);
    }

    // Write multiple times + Read + Compare
    for (i = 0 ; i < 5 ; i++)
    {
        for (j = 1 ; j < 64 ; j++)
        {
            x_memset((VOID*)u4MemPtr1, (UINT8)(u4Counter%255), u4aTestSize[i]);
            x_memset((VOID*)u4MemPtr2, 0, u4aTestSize[i]);

            for  (k = 0; k < j; k++) // write K times, read 1 time, compare 1 time
            {
                if (NANDPART_IORequest(u1PartId, u4Offset, (UINT32)u4MemPtr1, u4aTestSize[i], FALSE) != 0)
                {
                    Printf("Test2: Write %dB at %d:%d times aailed !\n", u4aTestSize[i], j, k);
                    goto ERROR_READWRITE_TEST;
                }
            }

            if (NANDPART_IORequest(u1PartId, u4Offset, (UINT32)u4MemPtr2, u4aTestSize[i], TRUE) != 0)
            {
                Printf("Test2: Read %dB at %d:%d times failed !\n", u4aTestSize[i], j, k);
                goto ERROR_READWRITE_TEST;
            }
            
            for (k = 0 ; k < u4aTestSize[i] ; k++)
            {
                if (u4MemPtr1[k] != u4MemPtr2[k])
                {
                    Printf("Test2: Compare %dB Failed !\n", u4aTestSize[i]);
                    goto ERROR_READWRITE_TEST;
                }
            }
            u4Counter++;
            Printf("Test2: Write %d times, Read %dB OK.\n", j, u4aTestSize[i]);
        }
    }

    Printf("Read/Write Test OK !");

ERROR_READWRITE_TEST:
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartStressTest(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4ByteCount, i;
    UINT8 u1PartId;
    NANDPART_STRUCT_T* arNandPart = 0;
    NANDDEV_T rNandDev;
    UINT32 u4BufSize = 128*1024;
    UINT32 u4RWOffset = 0;
    UINT8* u4MemPtr1;
    UINT8* u4MemPtr2;

    if (i4Argc != 3)
    {
        Printf("%s [part_id] [bytenum]\n", szArgv[0]);
        return -1;
    }
    u1PartId = (UINT8)StrToInt(szArgv[1]);
    u4ByteCount = StrToInt(szArgv[2]);

    arNandPart = NANDPART_GetPartStruct();
    if (arNandPart == 0)
    {
        Printf("Get partition info failed\n", u1PartId);
        return -1;
    }
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("NAND_GetInfo failed\n");
        return -1;
    }
    
    if (u4ByteCount > arNandPart[u1PartId].u4Size)
    {
        Printf("Test size is bigger than partition size !\n");
        return -1;
    }

    u4MemPtr1 = (UINT8*)x_mem_alloc(u4BufSize);
    if (u4MemPtr1 == 0)
    {
        Printf("Allocate memory1 failed !");
        return -1;
    }
    
    u4MemPtr2 = (UINT8*)x_mem_alloc(u4BufSize);
    if (u4MemPtr2 == 0)
    {
        x_mem_free((VOID*)u4MemPtr1);
        Printf("Allocate memory2 failed !");
        return -1;
    }

    for (i = 0; i < u4BufSize; i++)
    {
        u4MemPtr1[i] = (UINT8)(i%255);
    }

    while (u4ByteCount > 0)
    {
        if (NANDPART_IORequest(u1PartId, u4RWOffset, (UINT32)u4MemPtr1, u4BufSize, FALSE) != 0)
        {
            Printf("Write fail 1!\n");
            goto ERROR_PART_STRESS_TEST;
        }
        u4RWOffset+=u4BufSize;
        u4ByteCount-=u4BufSize;
    }
    
    u4ByteCount = StrToInt(szArgv[2]);
    Printf("Write %dB Complete.\n", u4ByteCount);

    u4RWOffset = 0;
    while (u4ByteCount > 0)
    {
        if (NANDPART_IORequest(u1PartId, u4RWOffset, (UINT32)u4MemPtr2, u4BufSize, TRUE) != 0)
        {
            Printf("Write fail 1!\n");
            goto ERROR_PART_STRESS_TEST;
        }
        
        for (i = 0; i < u4BufSize; i++)
        {
            if (u4MemPtr1[i] != u4MemPtr2[i])
            {
                Printf("Compare Failed 0x%X:0x%X !\n", u4MemPtr1[i], u4MemPtr2[i]);
                goto ERROR_PART_STRESS_TEST;
            }
        }
        
        u4RWOffset+=u4BufSize;
        u4ByteCount-=u4BufSize;
    }

    Printf("Stress Test OK \n");

ERROR_PART_STRESS_TEST:
    x_mem_free((VOID*)u4MemPtr1);
    x_mem_free((VOID*)u4MemPtr2);
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartRead(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4MemPtr, u4NandOffset, u4ByteCount, i;
    UINT8 u1PartId;
    UINT8* u1Ptr;

    if (i4Argc != 5)
    {
        Printf("%s [part_id] [memptr] [nand_offset] [bytenum]\n", szArgv[0]);
        return -1;
    }

    u1PartId = (UINT8)StrToInt(szArgv[1]);
    u4MemPtr = StrToInt(szArgv[2]);
    u4NandOffset = StrToInt(szArgv[3]);
    u4ByteCount = StrToInt(szArgv[4]);

    if (NANDPART_IORequest(u1PartId, u4NandOffset, u4MemPtr, u4ByteCount, TRUE) != 0)
    {
        Printf("Fail!\n");
        return -1;
    }

    u1Ptr = (UINT8 *)u4MemPtr;
    for (i = 0; i < 0x20; i++) 
    {
        if (i%16==0)
        {
            Printf("\n%03x: %02x ", i, u1Ptr[i]);
        }
        else 
        {
            Printf("%02x ", u1Ptr[i]);
        }
    }

    Printf("OK!\n");
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartWrite(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4MemPtr, u4NandOffset, u4ByteCount,i;
    UINT8 u1PartId;
    UINT8* u1Ptr;

    if (i4Argc != 5)
    {
        Printf("%s [part_id] [memptr] [nand_offset] [bytenum]\n", szArgv[0]);
        return -1;
    }

    u1PartId = (UINT8)StrToInt(szArgv[1]);
    u4MemPtr = StrToInt(szArgv[2]);
    u4NandOffset = StrToInt(szArgv[3]);
    u4ByteCount = StrToInt(szArgv[4]);

    if(NANDPART_IORequest(u1PartId, u4NandOffset, u4MemPtr, u4ByteCount, FALSE) != 0)
    {
        Printf("Fail!\n");
        return -1;
    }

    u1Ptr = (UINT8 *)u4MemPtr;
    for (i = 0; i < 0x20; i++) 
    {
        if (i%16==0)
        {
            Printf("\n%03x: %02x ", i, u1Ptr[i]);
        }
        else 
        {
            Printf("%02x ", u1Ptr[i]);
        }
    }

    Printf("OK!\n");
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartErase(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1PartId;
    NANDPART_STRUCT_T* arNandPart = 0;
    NANDDEV_T rNandDev;

    UINT32 u4PgStart, u4PgCount, u4PgEnd;

    if (i4Argc != 2)
    {
        Printf("%s [part id]\n", szArgv[0]);
        return -1;
    }

    u1PartId = (UINT8)StrToInt(szArgv[1]);
    arNandPart = NANDPART_GetPartStruct();
    if (arNandPart == 0)
    {
        Printf("Get partition info failed\n", u1PartId);
        return -1;
    }
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("NAND_GetInfo failed\n");
        return -1;
    }

    u4PgStart = ( arNandPart[u1PartId].u4Offset / rNandDev.u4PgSz );
    u4PgCount = ( arNandPart[u1PartId].u4Size / rNandDev.u4PgSz );
    u4PgEnd = u4PgStart + u4PgCount;

    Printf("Erase from offset : 0x%X ~ 0x%X, sect: %d ~ %d\n", arNandPart[u1PartId].u4Offset, arNandPart[u1PartId].u4Offset+arNandPart[u1PartId].u4Size, u4PgStart, u4PgEnd);
    if (NAND_Erase(u4PgStart, u4PgCount))
    {
        Printf("Erase Failed !\n");
        return -1;
    }
    
    Printf("Erase from sect: %d ~ %d Successfully\n", u4PgStart, u4PgEnd);
    Printf("Note ! This partition should be re-mount again !\n");
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartMeasure(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8   u1PartId;
    UINT8*  u4MemPtr;
    UINT8*  u4MemPtrAlign;
    UINT32  u4BufSize = 128*1024;
    UINT32  u4PartitionSize;
    UINT32  u4MeasureSize;
    UINT32  u4WriteCount;
    UINT32  u4ReadCount;
    UINT32  u4CompoundTestCount;
    UINT32 u4RWOffset = 0;
    UINT32 u4PgStart, u4PgCount;
    NANDPART_STRUCT_T* arNandPart = 0;
    NANDDEV_T rNandDev;
    HAL_TIME_T time1, time2, result_time;
    UINT32 u4Micros1, u4Micros2, u4Micros3, u4Micros4;

    if (i4Argc != 2)
    {
        Printf("%s [part_id]\n", szArgv[0]);
        return -1;
    }

    u1PartId = (UINT8)StrToInt(szArgv[1]);
    arNandPart = NANDPART_GetPartStruct();
    if (arNandPart == 0)
    {
        Printf("Get partition info failed\n", u1PartId);
        return -1;
    }

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("NAND_GetInfo failed\n");
        return -1;
    }

    u4PartitionSize = arNandPart[u1PartId].u4Size;
    u4MeasureSize = u4PartitionSize/2;
    u4WriteCount = u4MeasureSize/(u4BufSize);
    u4ReadCount = u4MeasureSize/(u4BufSize);
    u4CompoundTestCount = u4MeasureSize/(u4BufSize);

    u4MemPtr = (UINT8*)x_mem_alloc(u4BufSize+128*1024);
    x_memset((VOID*)u4MemPtr, 0xab, u4BufSize+128*1024);

    u4MemPtrAlign = u4MemPtr;
    if (((INT32)u4MemPtrAlign % (128*1024)) != 0)
    {
        u4MemPtrAlign += (128*1024) - ((INT32)u4MemPtrAlign % (128*1024));
    }

    // Test Erase Performance
    HAL_GetTime(&time1);

    u4PgStart = ( arNandPart[u1PartId].u4Offset / rNandDev.u4PgSz );
    u4PgCount = ( arNandPart[u1PartId].u4Size / rNandDev.u4PgSz );
    if (NAND_Erase(u4PgStart, u4PgCount))
    {
        Printf("Erase Failed 1 !\n");
        return -1;
    }
    
    HAL_GetTime(&time2);
    HAL_GetDeltaTime(&result_time, &time1, &time2);
    u4Micros1 = result_time.u4Seconds * 1000000 + result_time.u4Micros;

    // Test Write Performance
    HAL_GetTime(&time1);
    u4RWOffset = 0;
    while (u4WriteCount > 0)
    {
        if (NANDPART_IORequest(u1PartId, u4RWOffset, (UINT32)u4MemPtrAlign, u4BufSize, FALSE) != 0)
        {
            Printf("Write fail 1!\n");
            goto ERROR_PART_MEASURE;
        }
        u4RWOffset+=u4BufSize;
        u4WriteCount--;
    }
    HAL_GetTime(&time2);
    HAL_GetDeltaTime(&result_time, &time1, &time2);
    u4Micros2 = result_time.u4Seconds * 1000000 + result_time.u4Micros;

    // Test Read Performance
    HAL_GetTime(&time1);
    u4RWOffset = 0;
    while (u4ReadCount > 0)
    {
        if (NANDPART_IORequest(u1PartId, u4RWOffset, (UINT32)u4MemPtrAlign, u4BufSize, TRUE) != 0)
        {
            Printf("Read fail 1!\n");
            goto ERROR_PART_MEASURE;
        }
        
        u4RWOffset+=u4BufSize;
        u4ReadCount--;
    }
    HAL_GetTime(&time2);
    HAL_GetDeltaTime(&result_time, &time1, &time2);
    u4Micros3 = result_time.u4Seconds * 1000000 + result_time.u4Micros;

    // Test Erase+Write+Read Performance
    HAL_GetTime(&time1);
    u4RWOffset = 0;

    while (u4CompoundTestCount > 0)
    {
        if (NANDPART_IORequest(u1PartId, u4RWOffset, (UINT32)u4MemPtrAlign, u4BufSize, FALSE) != 0)
        {
            Printf("Write fail 2!\n");
            goto ERROR_PART_MEASURE;
        }
        
        if (NANDPART_IORequest(u1PartId, u4RWOffset, (UINT32)u4MemPtrAlign, u4BufSize, TRUE) != 0)
        {
            Printf("Read fail 2!\n");
            goto ERROR_PART_MEASURE;
        }
        u4RWOffset+=u4BufSize;
        u4CompoundTestCount--;
    }
    
    HAL_GetTime(&time2);
    HAL_GetDeltaTime(&result_time, &time1, &time2);
    u4Micros4 = result_time.u4Seconds * 1000000 + result_time.u4Micros;

    Printf("Erase Speed : Size=%dB, Time=%d\n", u4MeasureSize, u4Micros1);
    Printf("Write Speed : Size=%dB, Time=%d\n", u4MeasureSize, u4Micros2);
    Printf("Read Speed : Size=%dB, Time=%d\n", u4MeasureSize, u4Micros3);
    Printf("Erase+Write+Read Speed : Size=%dB, Time=%d\n", u4MeasureSize, u4Micros4);

    Printf("Partition Measure OK !\n");

ERROR_PART_MEASURE:
    x_mem_free((VOID*)u4MemPtr);
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NAND_CheckPartitionBlockStatus(
    UINT8 u1PartId, UINT32 u4PrintUsed, UINT32 u4PrintBad, UINT32 u4PrintEmpty)
{
    NANDPART_STRUCT_T* arNandPart = 0;
    NANDDEV_T rNandDev;
    UINT32 u4BlkStartIndex, u4BlkStopIndex;
    UINT32 i;
    UINT32 u4EmptyCount = 0, u4BadCount = 0, u4UsedCount = 0, u4UnknownCount = 0;
    UINT8 spare0[64];

    arNandPart = NANDPART_GetPartStruct();
    if (arNandPart == 0)
    {
        Printf("Get partition info failed\n", u1PartId);
        return -1;
    }

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("NAND_GetInfo failed\n");
        return -1;
    }

    u4BlkStartIndex = arNandPart[u1PartId].u4Offset / rNandDev.u4BlkSz;
    u4BlkStopIndex = u4BlkStartIndex + (arNandPart[u1PartId].u4Size / rNandDev.u4BlkSz);

    Printf("Partition Offset : 0x%X ~ 0x%X\n", arNandPart[u1PartId].u4Offset, arNandPart[u1PartId].u4Offset+arNandPart[u1PartId].u4Size);
    Printf("Block Index : %d ~ %d \n",u4BlkStartIndex, u4BlkStopIndex);

    for (i = u4BlkStartIndex; i < u4BlkStopIndex; i++)
    {
        if (NAND_IsBadBlk(i))
        {
            if (u4PrintBad)
            {
                Printf("  Bad blk idx:%d, offset:0x%x\n", i, i * rNandDev.u4BlkSz);
            }
            u4BadCount++;
            continue;
        }
        x_memset((void *)spare0, 0xff, 64);

        if (rNandDev.u4BlkSz == (128*1024))
        {
            if (NAND_RdAttr(i*rNandDev.u4BlkSz , (UINT32)(&spare0[0]),64))
            {
                Printf("Read Spare Failed : Block Index = %d\n", i);
                continue;
            }
            
            if (arNandPart[u1PartId].rType == NANDPART_BDM)
            {
                // don't know how to check used/empty block.
            }
            else if (arNandPart[u1PartId].rType == NANDPART_SDM)
            {
                if (spare0[1] == 0xFF)    // refer to nand_sdm.c
                {
                    if (u4PrintEmpty)
                    {
                        Printf("  Empty blk idx:%d, offset:0x%x\n", i, i * rNandDev.u4BlkSz);
                    }
                    u4EmptyCount++;
                }
                else if (spare0[1] == 0x5A)
                {
                    if (u4PrintUsed)
                    {
                        Printf("  Used blk idx:%d, offset:0x%x\n", i, i * rNandDev.u4BlkSz);
                    }
                    u4UsedCount++;
                }
                else
                {
                    if (u4PrintBad)
                    {
                        Printf("  Unknown blk idx:%d, offset:0x%x\n", i, i * rNandDev.u4BlkSz);
                    }
                    u4UnknownCount++;
                }
            }
            else
            {
                Printf("Only support SDM\n");
                return 0;
            }
        }
        else if (rNandDev.u4BlkSz == (16*1024))
        {
            if (NAND_RdAttr(i*rNandDev.u4BlkSz, (UINT32)(&spare0[0]),16))
            {
                Printf("Read Spare Failed : Block Index = %d\n", i);
                continue;
            }
            if (arNandPart[u1PartId].rType == NANDPART_BDM)
            {
                // don't know how to check used/empty block.
            }
            else if (arNandPart[u1PartId].rType == NANDPART_SDM)
            {
                if (spare0[4] == 0xFF)
                {
                    if (u4PrintEmpty)
                    {
                        Printf("  Empty blk idx:%d, offset:0x%x\n", i, i * rNandDev.u4BlkSz);
                    }
                    u4EmptyCount++;
                }
                else if (spare0[4] == 0x5A)
                {
                    if (u4PrintUsed)
                    {
                        Printf("  Used blk idx:%d, offset:0x%x\n", i, i * rNandDev.u4BlkSz);
                    }
                    u4UsedCount++;
                }
                else
                {
                    if (u4PrintBad)
                    {
                        Printf("  Unknown blk idx:%d, offset:0x%x\n", i, i * rNandDev.u4BlkSz);
                    }
                    u4UnknownCount++;
                }
            }
            else
            {
                Printf("Only support SDM\n");
                return 0;
            }
        }
    }

    Printf("Total Block : %d\n", arNandPart[u1PartId].u4Size / rNandDev.u4BlkSz);
    Printf("Total Used Block : %d\n", u4UsedCount);
    Printf("Total Empty Block : %d\n", u4EmptyCount);
    Printf("Total Bad Block : %d\n", u4BadCount);
    Printf("Total Unknown Block : %d\n", u4UnknownCount);
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartShowStatus(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1PartId;

    if (i4Argc != 2)
    {
        Printf("%s [part id]\n", szArgv[0]);
        return -1;
    }

    u1PartId = (UINT8)StrToInt(szArgv[1]);

    _NAND_CheckPartitionBlockStatus(u1PartId, FALSE, FALSE, FALSE);
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartShowBad(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1PartId;

    if (i4Argc != 2)
    {
        Printf("%s [part id]\n", szArgv[0]);
        return -1;
    }

    u1PartId = (UINT8)StrToInt(szArgv[1]);

    _NAND_CheckPartitionBlockStatus(u1PartId, FALSE, TRUE, FALSE);
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartShowEmpty(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1PartId;

    if (i4Argc != 2)
    {
        Printf("%s [part id]\n", szArgv[0]);
        return -1;
    }

    u1PartId = (UINT8)StrToInt(szArgv[1]);

    _NAND_CheckPartitionBlockStatus(u1PartId, FALSE, FALSE, TRUE);
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartShowUsed(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1PartId;

    if (i4Argc != 2)
    {
        Printf("%s [part id]\n", szArgv[0]);
        return -1;
    }

    u1PartId = (UINT8)StrToInt(szArgv[1]);

    _NAND_CheckPartitionBlockStatus(u1PartId, TRUE, FALSE, FALSE);
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartSDMDumpStatus(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 i, u4Flags;
    UINT8 u1id;

    if (i4Argc < 3)
    {
        Printf("%s partid [-a -e -b -u -l -d]\n", szArgv[0]);
        Printf("   -a: all, -e: empty, -b: bad, -u: used, -l: logic, -d:dirty \n");
        return -1;
    }

    u1id = (UINT8)StrToInt(szArgv[1]);
    u4Flags = 0;
    for (i = 2; i < (UINT32)i4Argc; i++)
    {
        if (szArgv[i][0]!='-')
        {
            continue;
        }
        
        switch (szArgv[i][1])
        {
        case 'a':
            u4Flags |= NAND_SDMDUMP_ALL;
            break;
        case 'e':
            u4Flags |= NAND_SDMDUMP_EMPTY;
            break;
        case 'd':
            u4Flags |= NAND_SDMDUMP_DIRTY;
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
            Printf("%s [-a -e -b -u -l -d]\n", szArgv[0]);
            Printf("   -a: all, -e: empty, -b: bad, -u: used, -l: logic, -d:dirty \n");
            return -1;
        }
    }

    NAND_SDM_DumpStatus(u1id, u4Flags);
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartSDMSyncCheck(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1ID;
    if (i4Argc != 2)
    {
        Printf("%s partid \n", szArgv[0]);
        return -1;
    }
    
    u1ID = (UINT8)StrToInt(szArgv[1]);
    NAND_SDM_SyncCheck(u1ID);

    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartSDMSyncStatus(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1ID;
    
    if (i4Argc != 2)
    {
        Printf("%s partid \n", szArgv[0]);
        return -1;
    }

    u1ID = (UINT8)StrToInt(szArgv[1]);
    NAND_SDM_SyncStatus(u1ID);

    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_PartSDMTest(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Count;

    if (i4Argc != 3)
    {
        Printf("%s [type] [loop]\n", szArgv[0]);
        Printf("    type: RW - read/write test\n");
        Printf("    type: Mount - mount/umount test\n");
        return -1;
    }

    u4Count = StrToInt(szArgv[2]);

    if (x_strncmp(szArgv[1], "RW", sizeof("RW")) == 0)
    {
        if (NANDPART_Mount(0, NANDPART_SDM, 0, 0x1000000) != 0)
        {
            return -1;
        }
        
        _NAND_SDM_RWTest(u4Count);
        NANDPART_UnMount(0);
    }
    else if (x_strncmp(szArgv[1], "Mount", sizeof("Mount")) == 0)
    {
        _NAND_SDM_MountTest(u4Count);
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

CLIMOD_DEBUG_FUNCTIONS(NAND)

static CLI_EXEC_T _arNandPartCmdTbl[] =
{
    {"info", "info", _NANDCLI_PartInfo, NULL, "info", CLI_GUEST },
    {"init", "init", _NANDCLI_PartInit, NULL, "init", CLI_GUEST },
    {"mount", "mount", _NANDCLI_PartMount, NULL, "mount [part_id] [type] [offset] [size]", CLI_GUEST },
    {"umount", "umount", _NANDCLI_PartUnMount, NULL, "umount [part_id]", CLI_GUEST },
    {"PartRWTest", "prwt", _NANDCLI_PartReadWriteTest, NULL, "prwt [part_id]", CLI_GUEST },
    {"PartStressTest", "pst", _NANDCLI_PartStressTest, NULL, "pst [part_id] [bytenum]", CLI_GUEST },
    {"PartRead", "pr", _NANDCLI_PartRead, NULL, "pr [part_id] [memptr] [nand_offset] [bytenum]", CLI_GUEST },
    {"PartWrite", "pw", _NANDCLI_PartWrite, NULL, "pw [part_id] [memptr] [nand_offset] [bytenum]", CLI_GUEST },
    {"PartErase", "pe", _NANDCLI_PartErase, NULL, "pe [part id] ", CLI_GUEST },
    {"PartMeasure", "pm", _NANDCLI_PartMeasure, NULL, "pm [part id] ", CLI_GUEST },
    {"PartShowStatus", "pss", _NANDCLI_PartShowStatus, NULL, "pss [part id] ", CLI_GUEST },
    {"PartShowBad", "psb", _NANDCLI_PartShowBad, NULL, "psb [part id] ", CLI_GUEST },
    {"PartShowEmpty", "pse", _NANDCLI_PartShowEmpty, NULL, "pse [part id] ", CLI_GUEST },
    {"PartShowUsed", "psu", _NANDCLI_PartShowUsed, NULL, "psu [part id] ", CLI_GUEST },

    {"sdmdumpstatus", "sds", _NANDCLI_PartSDMDumpStatus, NULL, "sds [part_id]", CLI_GUEST },
    {"sdmsynccheck", "sdsc", _NANDCLI_PartSDMSyncCheck, NULL, "sdsc [part_id]", CLI_GUEST },
    {"sdmsyncstatus", "sdss", _NANDCLI_PartSDMSyncStatus, NULL, "sdss [part_id]", CLI_GUEST },
    {"sdmtest", "test", _NANDCLI_PartSDMTest, NULL, "test", CLI_GUEST },

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T arNandCmdTbl[] =
{
    CLIMOD_DEBUG_CLIENTRY(NAND),

    DECLARE_CMD(_NANDCLI_Init, init, init, "Initialization"),

    DECLARE_CMD(_NANDCLI_Stop, stop, stop, "stop"),
    DECLARE_CMD(_NANDCLI_Reset, reset, reset, "reset"),

    DECLARE_CMD(_NANDCLI_PatternTest, patterntest, pt, "patterntest [burst/dmai/dmap] [addr] [size]"),
    DECLARE_CMD(_NANDCLI_AttrTest, attrtest, at, "attrtest"),
    DECLARE_CMD(_NANDCLI_State, state, st, "state"),

    DECLARE_CMD(_NANDCLI_Read, read, r, "read [memptr] [nand_offset] [bytenum]"),
    DECLARE_CMD(_NANDCLI_Write, write, w, "write [memptr] [nand_offset] [bytenum]"),
    
    DECLARE_CMD(_NANDCLI_RdAttr, rdattr, ra, "readattr, ra [memptr] [nand_offset]"),
    DECLARE_CMD(_NANDCLI_WrAttr, wrattr, wa, "writeattr, wa [memptr] [nand_offset]"),
    
    DECLARE_CMD(_NANDCLI_Erase, erase, e, "erase, e [nand_offset] [bytenum]"),
    DECLARE_CMD(_NANDCLI_EraseForce, eraseforce, ef, "erase force, ef [nand_offset] [bytenum]"),

    DECLARE_CMD(_NANDCLI_BlankCheck, blankcheck, bc, "blankcheck, bc [nand_offset] [pagecount]"),
    DECLARE_CMD(_NANDCLI_BlankCheckAttr, blankcheckattr, bca, "blankcheckattr, bca [nand_offset] [pagecount]"),

    DECLARE_CMD(_NANDCLI_IdDevice, id, id, "id"),

    DECLARE_CMD(_NANDCLI_Measure, measure, m, "m [nand_offset] [size]"),
    DECLARE_CMD(_NANDCLI_MeasureAdvanced, measure advanced, ma, "m [nand_offset] [size]"),

    DECLARE_CMD(_NANDCLI_CheckBadBlk, bad, bad, "check bad block"),
    DECLARE_CMD(_NANDCLI_MarkBadBlk, markbad, mbad, "mark bad block, markbad [blk_idx]"),

    DECLARE_CMD(_NANDCLI_ShowDevList, devlist, dlist, "show dev list"),

    DECLARE_CMD(_NANDCLI_PinShareStressTest, PinShareStressTest, pst, "pin share stress test"),
    DECLARE_CMD(_NANDCLI_PinShareTestOption, PinShareTestOption, pso, "pin share test option, pso [read/write/both][delay(ms)]"),

    DECLARE_CMD(_NANDCLI_SetMemoryUsage, setmemoryusage, smu, "smu [1|2] 1:Local Memory(Default), 2: Golbal Memory(0x1000000)"),

    {"partition", "part", NULL, _arNandPartCmdTbl, "partition command", CLI_GUEST },

    DECLARE_END_ITEM(),
};

CLI_MAIN_COMMAND_ITEM(Nand)
{
    "nand",
    NULL,
    NULL,
    arNandCmdTbl,
    "Nand command",
    CLI_SUPERVISOR
};
