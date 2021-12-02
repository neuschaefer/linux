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
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif
#define DEFINE_IS_LOG   NOR_IsLog
#include "x_debug.h"
#include "x_util.h"
// #include "x_chr_dev.h"

#include "x_timer.h"
#include "drv_common.h"
#include "nor_if.h"
// #include "storage_if.h"

#define CC_SMALL_STORAGE        1

#ifndef CC_SMALL_STORAGE

#ifndef __MW_CLI_DEF__
#define CC_SMALL_STORAGE        0
#else   /* __MW_CLI_DEF__ */
#define CC_SMALL_STORAGE        1
#endif  /* __MW_CLI_DEF__ */

#endif  /* CC_SMALL_STORAGE */


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define SFLASH_TESTBUF_SIZE     1024

#define NOR_BASE                BIM_BASE
#define NOR_FLASH_BASE_ADDR     (IO_VIRT + 0x8000000)


#define NOR_GOLDEN_ADDR         0x3000000
#define NOR_TEST_ADDR           0x3800000

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
//__align(16) static UINT8 _au1GoldenBuf[SFLASH_TESTBUF_SIZE];
//__align(16) static UINT8 _au1TestBuf[SFLASH_TESTBUF_SIZE];


//-------------------------------------------------------------------------
/** _NORCLI_Init
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_Init(INT32 i4Argc, const CHAR ** szArgv)
{
    Printf("Nor init\n");
    NOR_Init();

    return 0;
}

//-------------------------------------------------------------------------
/** _NORCLI_Stop
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_Stop(INT32 i4Argc, const CHAR ** szArgv)
{
    Printf("NORCLI_Stop do nothing\n");

    return 0;
}

//-------------------------------------------------------------------------
/** _NORCLI_Reset
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_Reset(INT32 i4Argc, const CHAR ** szArgv)
{
    return 0;
}

//-------------------------------------------------------------------------
/** _NORCLI_Diag
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_Diag(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = NOR_Diag();

    Printf( "\n==================\n"
            " Nor Diag %s!"
            "\n==================\n", ((i4Ret==0) ? "Success" : "Fail"));
    return i4Ret;
}


//-------------------------------------------------------------------------
/** _DoPatternTest
 */
//-------------------------------------------------------------------------
static INT32 _DoPatternTest(UINT32 u4FlashAddr, UINT32 u4GoldenAddr,
                            UINT32 u4TestAddr, UINT32 u4ByteCount)
{
    INT32 i4Loop;
    UINT8 *pu1Tmp1, *pu1Tmp2;

    pu1Tmp1 = (UINT8*)u4GoldenAddr;
    pu1Tmp2 = (UINT8*)u4TestAddr;

    Printf("Erase.....\n");
    if(NOR_EraseAddr(u4FlashAddr, u4ByteCount) == 1)
    {
        Printf(" Error!!!\n");
        return 1;
    }
    if(NOR_Read(u4FlashAddr, u4TestAddr, u4ByteCount) == 1)
    {
        Printf(" Error!!!\n");
        return 1;
    }
    for(i4Loop=0; i4Loop<u4ByteCount; i4Loop++)
    {
        if(pu1Tmp2[i4Loop] != 0xFF)
        {
            Printf(" Error!!!\n");
            return 1;
        }
    }
    Printf(" OK!\n");

    Printf("Write test pattern.....\n");
    if(NOR_Write(u4FlashAddr, u4GoldenAddr, u4ByteCount) == 1)
    {
        Printf(" Error!!!\n");
        return 1;
    }
    Printf(" OK!\n");

    Printf("Read back pattern.....\n");
    if(NOR_Read(u4FlashAddr, u4TestAddr, u4ByteCount) == 1)
    {
        Printf(" Error!!!\n");
        return 1;
    }
    Printf(" OK!\n");

    for(i4Loop=0; i4Loop<u4ByteCount; i4Loop++)
    {
        if(pu1Tmp1[i4Loop] != pu1Tmp2[i4Loop])
        {
            Printf("Serial flash compare error %d!!!\n", i4Loop);
            return 1;
        }
    }

    Printf("Serial flash 0x%x ~ 0x%x test OK!\n", u4FlashAddr, u4FlashAddr + u4ByteCount - 1);

    return 0;
}


//-------------------------------------------------------------------------
/** _PatternTest
 */
//-------------------------------------------------------------------------
static INT32 _PatternTest(UINT32 u4FlashOffset, UINT32 u4GoldenAddr,
                          UINT32 u4TestAddr, UINT32 u4Size)
{
    UINT32 i;
    UINT8 *pu1GoldenBuf;

    pu1GoldenBuf = (UINT8*)u4GoldenAddr;

    //-----------------------------------------------
    Printf("Pattern 1\n");
    for(i=0; i<SFLASH_TESTBUF_SIZE; i++)
    {
        if((i%2) == 0)
            pu1GoldenBuf[i] = 0x55;
        else
            pu1GoldenBuf[i] = 0xAA;
    }
    if( _DoPatternTest(u4FlashOffset, u4GoldenAddr, u4TestAddr, u4Size) != 0)
    {
        return 1;
    }

    //-----------------------------------------------
    Printf("Pattern 2\n");
    for(i=0; i<SFLASH_TESTBUF_SIZE; i++)
    {
        if((i%2) == 0)
            pu1GoldenBuf[i] = 0xAA;
        else
            pu1GoldenBuf[i] = 0x55;
    }
    if( _DoPatternTest(u4FlashOffset, u4GoldenAddr, u4TestAddr, u4Size) != 0)
    {
        return 1;
    }

    //-----------------------------------------------
    Printf("Pattern increase address\n");
    for(i=0; i<SFLASH_TESTBUF_SIZE; i++)
    {
        pu1GoldenBuf[i] = (UINT8)(i&0xFF);
    }
    if( _DoPatternTest(u4FlashOffset, u4GoldenAddr, u4TestAddr, u4Size) != 0)
    {
        return 1;
    }

    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_FlashPattern
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_FlashPattern(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr;

    if(i4Argc != 2)
    {
        Printf("nor.pattern [sflash_offset]\n");
        return 1;
    }

    u4Addr = StrToInt(szArgv[1]);

    if(_PatternTest(u4Addr, NOR_GOLDEN_ADDR, NOR_TEST_ADDR, SFLASH_TESTBUF_SIZE) != 0)
    {
        Printf("Pattern test fail!\n");
        return 1;
    }

    Printf("Patter test OK!\n");

    return 0;
}


//-------------------------------------------------------------------------
static INT32 _Nor_AlignTest(UINT32 u4FlashAddr, UINT32 u4GoldenAddr,
                            UINT32 u4TestAddr, UINT32 u4ByteCount)
{
    UINT32 i, u4Align;
    UINT8 *pu1TestBuf;

    if(_DoPatternTest(u4FlashAddr, u4GoldenAddr, u4TestAddr, u4ByteCount) != 0)
    {
        return 1;
    }

    u4Align = u4FlashAddr % 128;
    if(u4Align != 0)
    {
        pu1TestBuf = (UINT8*)(NOR_FLASH_BASE_ADDR + u4FlashAddr - u4Align);
        for(i=0; i<u4Align; i++)
        {
            if(pu1TestBuf[i] != 0xFF)
                return 1;
        }
    }

    u4Align = (u4FlashAddr + u4ByteCount) % 128;
    if(u4Align != 0)
    {
        pu1TestBuf = (UINT8*)(NOR_FLASH_BASE_ADDR + u4FlashAddr + u4ByteCount);
        for(i=0; i<(128 - u4Align); i++)
        {
            if(pu1TestBuf[i] != 0xFF)
                return 1;
        }
    }

    return 0;
}

//-------------------------------------------------------------------------
/** _NORCLI_AlignTest
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_AlignTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr, i;
    UINT8  *pu1GoldenBuf;

    if(i4Argc != 2)
    {
        Printf("nor.aligntest [sflash_offset]\n");
        return 1;
    }

    u4Addr = StrToInt(szArgv[1]);

    pu1GoldenBuf = (UINT8*)NOR_GOLDEN_ADDR;

    for(i=0; i<SFLASH_TESTBUF_SIZE; i++)
    {
        pu1GoldenBuf[i] = (UINT8)(i&0xFF);
    }

    if(_Nor_AlignTest(u4Addr, NOR_GOLDEN_ADDR, NOR_TEST_ADDR, 511) != 0)
    {
        Printf("Pattern test fail!\n");
        return 1;
    }

    if(_Nor_AlignTest(u4Addr + 1, NOR_GOLDEN_ADDR, NOR_TEST_ADDR, 509) != 0)
    {
        Printf("Pattern test fail!\n");
        return 1;
    }

    if(_Nor_AlignTest(u4Addr + 1, NOR_GOLDEN_ADDR, NOR_TEST_ADDR, 512) != 0)
    {
        Printf("Pattern test fail!\n");
        return 1;
    }

    Printf("Pattern test OK!\n");

    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_Enable
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_Enable(INT32 i4Argc, const CHAR ** szArgv)
{
    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_Disable
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_Disable(INT32 i4Argc, const CHAR ** szArgv)
{
    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_State
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_State(INT32 i4Argc, const CHAR ** szArgv)
{
    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_Part
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_Part(INT32 i4Argc, const CHAR ** szArgv)
{
#if 0
    UINT32 u4ParSize;
    UINT32 u4Pid;
    UINT32 i, u4Loop;
    UINT8  *pu1SrcMemPtr = (UINT8 *)0x1000;
    UINT8  *pu1DstMemPtr;

    if (i4Argc != 3) {
    Printf("nor.pt [Part Num] [Loop times]\n");
        return 0;
    }

    u4Pid = StrToInt(szArgv[1]);
    u4Loop = StrToInt(szArgv[2]);

    u4ParSize = NORPART_GetPartSize(u4Pid);
    if (u4ParSize == 0)
    {
        Printf("Part Size = 0 !!\n");
        return 0;
    }

    pu1DstMemPtr = x_mem_alloc(u4ParSize);
    if (!pu1DstMemPtr)
    {
        Printf("allocate fail !!\n");
        return 0;
    }

    for (i=0; i<u4Loop; i++)
    {
        NORSTG_Erase(u4Pid);
        NORSTG_Write(u4Pid, 0, (UINT32)pu1SrcMemPtr, u4ParSize);
        NORSTG_Read(u4Pid, 0, (UINT32)pu1DstMemPtr, u4ParSize);

        // compare data.
        if (x_memcmp((const VOID *)pu1SrcMemPtr, (const VOID *)pu1DstMemPtr, u4ParSize) != 0)
        {
            Printf("Compare fail.\n");
            break;
        }
    }

    x_mem_free((VOID *)pu1DstMemPtr);
#endif
    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_Read
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_Read(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Offset, u4MemPtr, u4MemLen;
    INT32 i4Ret;

    if (i4Argc != 4) {
        Printf("nor.r [nor offset] [memptr] [memlen]\n");
        return 1;
    }
    u4Offset = StrToInt(szArgv[1]);
    u4MemPtr = StrToInt(szArgv[2]);
    u4MemLen = StrToInt(szArgv[3]);
    i4Ret = NOR_Read((UINT64) u4Offset, u4MemPtr, u4MemLen);
    if (i4Ret) {
        Printf("NorFlash read failed\n");
        return 1;
    }
    Printf("NorFlash read successfully.\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_WriteByte
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_WriteByte(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Offset, u4Data;
    INT32 i4Ret;

    if (i4Argc != 3) {
        Printf("nor.wb [nor offset] [byteval]\n");
        return 1;
    }

    u4Offset = StrToInt(szArgv[1]);
    u4Data = StrToInt(szArgv[2]);

    i4Ret = NOR_WriteByte(u4Offset, u4Data);
    if (i4Ret) {
        Printf("NorFlash write failed\n");
        return 1;
    }
    Printf("NorFlash write successfully.\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_Write
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_Write(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Offset, u4MemPtr, u4MemLen;
    INT32 i4Ret;

    if (i4Argc != 4) {
        Printf("nor.wf [nor offset] [memptr] [memlen]\n");
        return 1;
    }

    u4Offset = StrToInt(szArgv[1]);
    u4MemPtr = StrToInt(szArgv[2]);
    u4MemLen = StrToInt(szArgv[3]);

    i4Ret = NOR_Write(u4Offset, u4MemPtr, u4MemLen);
    if (i4Ret) {
        Printf("NorFlash write failed\n");
        return 1;
    }
    Printf("NorFlash write successfully.\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_WrNoE
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_WrNoE(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Offset, u4MemPtr, u4MemLen, u4TmpLen;
    INT32 i4Ret;
    HAL_TIME_T rTime;

    if (i4Argc != 4) {
        Printf("nor.we [nor offset] [memptr] [memlen]\n");
        return 1;
    }
    u4Offset = StrToInt(szArgv[1]);
    u4MemPtr = StrToInt(szArgv[2]);
    u4MemLen = StrToInt(szArgv[3]);

    HAL_GetTime(&rTime);
    Printf("Start %d.%06d\n", rTime.u4Seconds, rTime.u4Micros);
    while (u4MemLen > 0) {
        if (u4MemLen > 0x10000) {
            u4TmpLen = 0x10000;
        } else {
            u4TmpLen = u4MemLen;
        }

        // i4Ret = NOR_WrNoE(u4Offset, u4MemPtr, u4MemLen);
        i4Ret = NOR_WrNoE(u4Offset, u4MemPtr, u4TmpLen);
        if (i4Ret)
        {
            Printf("NorFlash wrnoe failed\n");
            return 1;
        }

        Printf("0x%08x - 0x%08x ... writing done\n", u4MemPtr, u4MemPtr + u4TmpLen - 1);
        u4MemLen -= u4TmpLen;
        u4MemPtr += u4TmpLen;
        u4Offset += u4TmpLen;
    }
    Printf("Start %d.%06d\n", rTime.u4Seconds, rTime.u4Micros);
    HAL_GetTime(&rTime);
    Printf("End %d.%06d\n", rTime.u4Seconds, rTime.u4Micros);
    Printf("NorFlash wrnoe successfully.\n");

    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_ErChp
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_ErChp(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Chip;
    INT32 i4Ret;
    HAL_TIME_T rTime;

    if (i4Argc != 2) {
        Printf("nor.ec [chip id 0~2]\n");
        return 1;
    }

    u4Chip = StrToInt(szArgv[1]);
    HAL_GetTime(&rTime);
    Printf("Start %d.%06d\n", rTime.u4Seconds, rTime.u4Micros);
    i4Ret = NOR_EraseChip(u4Chip);
    HAL_GetTime(&rTime);
    Printf("End %d.%06d\n", rTime.u4Seconds, rTime.u4Micros);
    if (i4Ret) {
        Printf("Chip %d erase failed.\n", u4Chip);
        return 1;
    }
    Printf("Chip %d erase successfully.\n", u4Chip);
    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_EraseIdx
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_EraseIdx(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT32 u4BlkIdx, u4BlkNum;

    if (i4Argc != 3) {
        Printf("nor.ed [blkidx] [blknum]\n");
        return 1;
    }

    u4BlkIdx = StrToInt(szArgv[1]);
    u4BlkNum = StrToInt(szArgv[2]);
    i4Ret = NOR_Erase(u4BlkIdx, u4BlkNum);
    if (i4Ret) {
        Printf("Erase failed\n");
        return 1;
    }
    Printf("Erase OK\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_Erase
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_Erase(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Offset, u4ByteLen;

    if (i4Argc != 3) {
        Printf("nor.e [nor offset] [byte-len]\n");
        return 1;
    }
    u4Offset = StrToInt(szArgv[1]);
    u4ByteLen = StrToInt(szArgv[2]);

    if(NOR_EraseAddr(u4Offset, u4ByteLen) != 0)
    {
        Printf("Erase flash fail !\n");
        return -1;
    }
	
	Printf("Erase flash Start Add:%x, ByteLen: %x!\n",u4Offset,u4ByteLen);
    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_ErMap
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_ErMap(INT32 i4Argc, const CHAR ** szArgv)
{
    SECTTBL_T rNorMap;
    INT32 i4Ret, i;
    UINT32 u4Seg, u4Sect, u4Size;

    i4Ret = NOR_EraseMap(&rNorMap);
    if (i4Ret) {
        Printf("No NorFlash Erase block map\n");
        return 1;
    }

    i = u4Seg = u4Sect = u4Size = 0;
    for (u4Seg = 0; u4Seg < rNorMap.u4SegNum; u4Seg++) {
        for (u4Sect = 0; u4Sect < rNorMap.prSegInfo[u4Seg].u4SectNum; u4Sect++) {
            Printf("%02d: 0x%08x - 0x%08x\n", i, u4Size, u4Size + rNorMap.prSegInfo[u4Seg].u4SectSize - 1);
            u4Size += rNorMap.prSegInfo[u4Seg].u4SectSize;
            i++;
        }
    }

    i = u4Seg = u4Sect = u4Size = 0;
    for (u4Seg = 0; u4Seg < rNorMap.u4SegNum; u4Seg++) {
        Printf("0x%08x - 0x%08x num:%d unit:0x%08x\n", u4Size,
                u4Size + (rNorMap.prSegInfo[u4Seg].u4SectSize * rNorMap.prSegInfo[u4Seg].u4SectNum),
                rNorMap.prSegInfo[u4Seg].u4SectNum, rNorMap.prSegInfo[u4Seg].u4SectSize);
        u4Size += (rNorMap.prSegInfo[u4Seg].u4SectSize * rNorMap.prSegInfo[u4Seg].u4SectNum);
    }

    Printf("Total NorFlash size is 0x%08x(%d MB)\n", u4Size, u4Size >> 20);
    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_Identify
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_Identify(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Idx,u4Size;
    UINT8 u1MenuID, u1DevID1, u1DevID2;
    CHAR *pStr;

    u4Idx = 0;
    if(i4Argc > 1)
    {
        u4Idx = StrToInt(szArgv[1]);
    }

    if(SFLASH_GetID(u4Idx, &u1MenuID, &u1DevID1, &u1DevID2) != 0)
    {
        Printf("Read serial flash ID fail!\n");
        return -1;
    }

    Printf("MenuID: 0x%X, DeviceID1: 0x%X, DeviceID2: 0x%X\n",
            u1MenuID, u1DevID1, u1DevID2);
	
    u4Size = SFLASH_GetFlashSize(u1MenuID, u1DevID1, u1DevID2);
    pStr = SFLASH_GetFlashName(u1MenuID, u1DevID1, u1DevID2);
    if(pStr != NULL)
    {
        Printf("Nor flash name:%s,Size:%d MB(0x%x)\n", pStr,u4Size >> 20,u4Size);
    }

    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_PartErase
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_PartErase(INT32 i4Argc, const CHAR ** szArgv)
{
#if 0
    UINT32 u4Pid;
    UINT32 i;
    UINT32 u4PartSize;

    Printf("Nor flash partition information.\n");
    for (i = PART_MIDD_0; i < (UINT32)PART_MIDD_END; i++)
    {
        u4PartSize = NORPART_GetPartSize(i);
        if (u4PartSize > 0)
        {
            Printf("Part(%d), size = 0x%08X bytes, start addr = 0x%08X.\n", i,
                u4PartSize, NOR_FLASH_BASE_ADDR + NORPART_PartStartOffset(i));
        }
        else
        {
            Printf("Part(%d), Not used yet.\n", i);
        }
    }

    if (i4Argc != 2) {
    Printf("\nUsage: nor.pe [erase partition]\n");
        return 0;
    }

    u4Pid = StrToInt(szArgv[1]);

    if (u4Pid >= (UINT32)PART_MIDD_END) {
    Printf("partition number error.\n");
        return 0;
    }

    NORSTG_Erase(u4Pid);

    Printf("erase partition %d ok.\n", u4Pid);
#endif
    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_Calibrate
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_Calibrate(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 au1Buf[2048];
    UINT32 i;

    // Write test pattern
    for(i=0; i<512; i++)
        au1Buf[i] = (i & 0xFF);

    if(SFLASH_EraseAddr(0x400000, 512) != 0)
    {
        Printf("Erase flash fail !\n");
        return 1;
    }

    if(SFLASH_Write(0x400000, (UINT32)au1Buf, 2048) != 0)
    {
        Printf("Write flash fail !\n");
        return 1;
    }

    // Check
    SFLASH_Calibrate(0x400000, 2048, au1Buf);

    return 0;
}

//-------------------------------------------------------------------------
static INT32 _pow(INT32 i4Base, INT32 i4Exp)
{
    INT32 val, i;

    if(i4Exp == 0)
        return 1;

    val= i4Base;
    for(i=1; i<i4Exp; i++)
    {
        val *= i4Base;
    }

    return val;
}

//-------------------------------------------------------------------------
#define   INIT_STATE 0
static INT32 _RISCCheckSum(UINT32 u4Offset, UINT32 u4Len, UINT32* pu4CheckSum)
{
    int coeff[32] = { 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0,
                      1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0 };

    int state[32], i, j, in, state_h = 0;
    UINT8 *pu1Buf;
    UINT32 u4Loop;

    //------------------------------------------------------------------
    // Init CRC
    //------------------------------------------------------------------
    for(i=0; i<32; i++)
        state[i] = (INIT_STATE >> i) & 0x1;

    for(j=0, i=0; i<32; i++)
    {
        j += (coeff[i] == 1) ? (int)_pow(2, i): 0;
    }

    //------------------------------------------------------------------
    // Calculate CRC
    //------------------------------------------------------------------
    pu1Buf = (UINT8*)(u4Offset + NOR_FLASH_BASE_ADDR);
    for(u4Loop=0; u4Loop<u4Len; u4Loop++)
    {
        for(i=0; i<8; i++)
        {
            in = ((pu1Buf[u4Loop]>>i) & 0x01) ^ state[31];
            for(j=31; j>=1; j--)
                state[j] = coeff[j] ? state[j-1] ^ in : state[j-1];
            state[0] = in;
        }
    }

    state_h = 0 ;
    for(j=0; j<32; j++)
    {
        if( j==0 )
            state_h += (state[j]==1) ? (int)(1) : 0;
        else
            state_h += (state[j]==1) ? (int)(((long)2)<<(j-1)) : 0;
    }

    *pu4CheckSum = state_h;

    return 0;
}

//-------------------------------------------------------------------------
/** _NORCLI_CRC
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_CRC(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Addr, u4Len, u4Checksum, u4Checksum2;

    if (i4Argc != 3)
    {
        Printf("nor.c [offset] [byte-len]\n");
        return 1;
    }

    u4Addr = StrToInt(szArgv[1]);
    u4Len = StrToInt(szArgv[2]);

    if(NOR_CalCRC(u4Addr, u4Len, &u4Checksum) == 1)
    {
        Printf("Serial flash calculate checksum FAIL!!\n");
    }
	Printf("HW calculate checksum:%x!\n",u4Checksum);
	
    _RISCCheckSum(u4Addr, u4Len, &u4Checksum2);

	Printf("RISC calculate checksum:%x!\n",u4Checksum2);

    if(u4Checksum != u4Checksum2)
    {
        Printf("Serial flash calculate checksum error!!\n");
        return 1;
    }

    Printf("Serial flash calculate checksum OK!!\n");

    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_Loop
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_Loop(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 i, u4Loop, u4NorOffset, u4MemAddr, u4Size, u4Checksum, u4Checksum2;

	if (i4Argc != 5)
	{
        Printf("nor.l [times] [nor-offset] [memptr] [size]\n");
        return 1;
	}

    u4Loop = StrToInt(szArgv[1]);
    u4NorOffset = StrToInt(szArgv[2]);
    if(u4NorOffset >= NOR_FLASH_BASE_ADDR)
        u4NorOffset -= NOR_FLASH_BASE_ADDR;
    u4MemAddr = StrToInt(szArgv[3]);
    u4Size = StrToInt(szArgv[4]);

    if(NOR_CalCRC(u4NorOffset, u4Size, &u4Checksum) == 1)
    {
        Printf("Get checksum error!\n");
        return 1;
    }

    for(i=0; i<u4Loop; i++)
    {
        Printf("Loop: %d\n", i);
        x_memset((VOID*)u4MemAddr, 0x0, u4Size);
        NOR_Read((UINT64) u4NorOffset, u4MemAddr, u4Size);
        if(x_memcmp((VOID*)u4MemAddr, (VOID*)(u4NorOffset + NOR_FLASH_BASE_ADDR), u4Size) != 0)
        {
            Printf("Memory compare error !\n");
            return 1;
        }

        if(NOR_CalCRC(u4NorOffset, u4Size, &u4Checksum2) == 1)
        {
            Printf("Get checksum error!\n");
            return 1;
        }
        if(u4Checksum != u4Checksum2)
        {
            Printf("Checksum compare error 0x%X != 0x%X\n", u4Checksum, u4Checksum2);
            return 1;
        }

        Printf("OK!\n");
    }

    Printf("Nor flash test OK\n");

    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_Measure
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_Measure(INT32 i4Argc, const CHAR ** szArgv)
{
    HAL_TIME_T time1, time2, result_time;
    UINT32 i, u4Offset, u4MemPtr, u4ByteLen;
    UINT8 *pu1Buf;

    if(i4Argc != 3)
    {
        Printf("nor.m [nor_offset] [byte_count]\n");
        return 1;
    }
    u4Offset = StrToInt(szArgv[1]);
    u4ByteLen = StrToInt(szArgv[2]);

    u4MemPtr = (UINT32)x_mem_alloc(u4ByteLen);
    if (u4MemPtr == 0)
    {
        Printf("nor measure mem alloc fail\n");
        return 1;
    }
    pu1Buf = (UINT8*)u4MemPtr;

    //-------------------------------
    // Erase
    //-------------------------------
    Printf("Measre erase time...\n");
    HAL_GetTime(&time1);
    if(NOR_EraseAddr(u4Offset, u4ByteLen) != 0)
    {
        Printf("Erase fail!\n");
        x_mem_free((VOID*)u4MemPtr);
        return -1;
    }
    HAL_GetTime(&time2);
    if(NOR_Read((UINT64) u4Offset, u4MemPtr, u4ByteLen) != 0)
    {
        Printf("Erase check fail!\n");
        x_mem_free((VOID*)u4MemPtr);
        return -1;
    }
    for(i=0; i<u4ByteLen; i++)
    {
        if(pu1Buf[i] != 0xFF)
        {
            Printf("Erase check fail at 0x%X!\n", i);
            x_mem_free((VOID*)u4MemPtr);
            return -1;
        }
    }
    HAL_GetDeltaTime(&result_time, &time1, &time2);
    Printf("Erase 0x%XBytes elapse time: %d sec, %d usec\n", u4ByteLen,
                                    result_time.u4Seconds, result_time.u4Micros);

    //-------------------------------
    // Write
    //-------------------------------
    Printf("Measre write time...\n");
    for(i=0; i<u4ByteLen; i++)
    {
        pu1Buf[i] = (UINT8)(i&0xFF);
    }

    HAL_GetTime(&time1);
    if(NOR_Write(u4Offset, u4MemPtr, u4ByteLen) != 0)
    {
        Printf("Write fail!\n");
        x_mem_free((VOID*)u4MemPtr);
        return -1;
    }
    HAL_GetTime(&time2);
#if 1
    if(x_memcmp((VOID*)u4MemPtr, (VOID*)(u4Offset + NOR_FLASH_BASE_ADDR), u4ByteLen) != 0)
    {
        Printf("Write check fail!\n");
        x_mem_free((VOID*)u4MemPtr);
        return -1;
    }
#endif
    HAL_GetDeltaTime(&result_time, &time1, &time2);
    Printf("Write 0x%XBytes elapse time: %d sec, %d usec\n", u4ByteLen,
                                    result_time.u4Seconds, result_time.u4Micros);


    //-------------------------------
    // Read
    //-------------------------------
    Printf("Measre read time...\n");
    x_memset((VOID*)u4MemPtr, 0x0, u4ByteLen);
    HAL_GetTime(&time1);
    if(NOR_Read(u4Offset, u4MemPtr, u4ByteLen) != 0)
    {
        Printf("Read fail!\n");
        x_mem_free((VOID*)u4MemPtr);
        return -1;
    }
    HAL_GetTime(&time2);
    if(x_memcmp((VOID*)u4MemPtr, (VOID*)(u4Offset + NOR_FLASH_BASE_ADDR), u4ByteLen) != 0)
    {
        Printf("Read check fail!\n");
        x_mem_free((VOID*)u4MemPtr);
        return -1;
    }
    HAL_GetDeltaTime(&result_time, &time1, &time2);
    Printf("Read 0x%XBytes elapse time: %d sec, %d usec\n", u4ByteLen,
                                    result_time.u4Seconds, result_time.u4Micros);

    x_mem_free((VOID*)u4MemPtr);
    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_ReadStatus
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_ReadStatus(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Val;
    UINT32 u4Idx;

    u4Idx = 0;
    if(i4Argc > 1)
    {
	u4Idx = StrToInt(szArgv[1]);
    }

    if(SFLASH_ReadFlashStatus(u4Idx, &u1Val) != 0)
    {
        Printf("Read flash status fail!\n");
        return -1;
    }

    Printf("Flash index:%d, status: 0x%X\n",u4Idx,u1Val);

    return 0;
}


//-------------------------------------------------------------------------
/** _NORCLI_WriteProtect
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
EXTERN BOOL _fgWriteProtect;
static INT32 _NORCLI_WriteProtect(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgWriteProtect;
    UINT32 u4Idx;

    if(i4Argc < 2)
    {
        Printf("wp [enable/disable]\n");
        return 0;
    }

    u4Idx = 0;
    if(i4Argc > 2)
    {
        u4Idx = StrToInt(szArgv[2]);
    }

    fgWriteProtect = TRUE;
    _fgWriteProtect = TRUE;
	
    if(x_strncmp(szArgv[1], "enable", sizeof("enable")) == 0)
    {
        fgWriteProtect = TRUE;
	if(SFLASH_WriteProtect(u4Idx, fgWriteProtect) != 0)
    	{
          Printf("Write protect fail!\n");
          return -1;
    	}
	 _fgWriteProtect = TRUE;
    }
    else
    {
        fgWriteProtect = FALSE;
	if(SFLASH_WriteProtect(u4Idx, fgWriteProtect) != 0)
    {
        Printf("Write protect fail!\n");
        return -1;
    }
	_fgWriteProtect = FALSE;
    }

   

    Printf("Flash index %d, %s write protect OK",u4Idx,fgWriteProtect? "Enable" : "Disable");

    return 0;
}
//-------------------------------------------------------------------------
/** _NORCLI_Stress
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _NORCLI_Stress(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4TestLoop;

	if(i4Argc != 4)
	{
	   Printf("nor.stress [nor_offset] [byte_count] [test loop]\n");
	   return 1;
	}
	u4TestLoop = StrToInt(szArgv[3]);
	if (u4TestLoop == 0)
	{
	   Printf("test loop must be > 0");
	   return 1;
	}
	while(u4TestLoop)
	{
	  printf("\n");
	  printf("-----------------TestLoop: %d----------------------\n",(StrToInt(szArgv[3])- u4TestLoop));
	  if (_NORCLI_Measure( (i4Argc-1), szArgv) )
	   {
		return 1;
	   }
	   u4TestLoop--;
}

	 Printf("\nStressOK!! %d times, from 0x%x, len:0x%x\n", StrToInt(szArgv[3]),
						 StrToInt(szArgv[1]), StrToInt(szArgv[2]) );
    return 0;
}

/******************************************************************************
* Command table
******************************************************************************/
static CLI_EXEC_T arNorCmdTbl[] = {
    DECLARE_CMD(_NORCLI_Init, init, init, "nor.init"),
    DECLARE_CMD(_NORCLI_Stop, stop, stop, "nor.stop"),
    DECLARE_CMD(_NORCLI_Reset, reset, reset, "nor.reset"),
    DECLARE_CMD(_NORCLI_Diag, diag, diag, "nor.diag"),
    DECLARE_CMD(_NORCLI_FlashPattern, flashpattern, fp, "nor.flashpattern"),
    DECLARE_CMD(_NORCLI_AlignTest, aligntest, at, "nor.aligntest [sflash_offset]"),

    DECLARE_CMD(_NORCLI_Enable, enable, en, "nor.en"),
    DECLARE_CMD(_NORCLI_Disable, disable, dis, "nor.dis"),

    DECLARE_CMD(_NORCLI_State, state, st, "nor.st"),
    DECLARE_CMD(_NORCLI_Part, part, pt, "nor.pt [loop times]"),

/*  CLIMOD_REGTST_CLIENTRY(NOR), */

    DECLARE_CMD(_NORCLI_Read, read, r, "nor.r [nor-offset] [memptr] [memlen]"),
    DECLARE_CMD(_NORCLI_Write, write, wr, "nor.wr [nor-offset] [memptr] [memlen]"),
    DECLARE_CMD(_NORCLI_WriteByte, writebyte, wb, "nor.wb [nor-offset] [byteval]"),
    DECLARE_CMD(_NORCLI_WrNoE, wrnoe, we, "nor.we [nor-offset] [memptr] [memlen], "
                                            "write with auto-erasing, no necessary erase."),
    DECLARE_CMD(_NORCLI_Erase, erase, e, "nor.e [nor-offset] [byte-length]"),
    DECLARE_CMD(_NORCLI_EraseIdx, eraseidx, ed, "nor.ed [blkidx] [blknum]"),
    DECLARE_CMD(_NORCLI_ErChp, erchp, ec, "nor.ec [chip id 0~1]"),
    DECLARE_CMD(_NORCLI_ErMap, ermap, em, "nor.em , to display the erase block map."),
    DECLARE_CMD(_NORCLI_Identify, identify, id, "nor.id"),
    //CLIMOD_DEBUG_CLIENTRY(NOR),

    DECLARE_CMD(_NORCLI_PartErase, parterase, pe, "nor.pe [erase partition]"),

    DECLARE_CMD(_NORCLI_Calibrate, calibrate, cal, "nor.cal"),
    DECLARE_CMD(_NORCLI_CRC, crc, c, "nor.c"),
    DECLARE_CMD(_NORCLI_Loop, loop, l, "nor.l [times] [nor-offset] [memptr] [size]"),
    DECLARE_CMD(_NORCLI_Measure, measure, m, "nor.m [nor_offset] [dram_addr] [byte_count]"),
    DECLARE_CMD(_NORCLI_ReadStatus, readstatus, rs, "nor.rs"),
    DECLARE_CMD(_NORCLI_WriteProtect, writeprotect, wp, "nor.wp [enable/disable]"),
    DECLARE_CMD(_NORCLI_Stress, stress, str, "nor.str [nor_offset] [byte_count ][stress loop]"),

    DECLARE_END_ITEM(),
};

/*
 * Do not set static and Add "CLI_MAIN_COMMAND" to add command dir into cli.
 */
CLI_MAIN_COMMAND_ITEM(Nor)
{
    "nor", NULL, NULL, arNorCmdTbl, "Nor command", CLI_SUPERVISOR
};

#if 0
/******************************************************************************
* Global Function
******************************************************************************/

#ifdef __MW_CLI_DEF__
CLI_EXEC_T* GetNorMwCmdTbl(void)
#else
CLI_EXEC_T* GetNorCmdTbl(void)
#endif
{
    return _arNorModCmdTbl;
}
#endif

