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
#define DEFINE_IS_LOG(level, fmt...) Printf(fmt)
#include "x_debug.h"
#include "x_util.h"
#include "x_timer.h"
#include "nand_if.h"
#include "storage_if.h"

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define NAND_TESTBUF1_ADDR    0x1000000
#define NAND_TESTBUF2_ADDR    0x2000000
#define NAND_TESTBUF3_ADDR    0x3000000

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
        if (szArgv[i][0]!='-')
        {
            continue;
        }
        
        switch (szArgv[i][1])
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
static INT32 _NANDCLI_Read(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8  *ptr, u1Spare[NAND_MAX_OOB_SIZE];
    INT32  i, i4Ret;
    UINT32 u4Len, u4MemPtr, u4Offset;
    NANDDEV_T rNandDev;

    if (i4Argc < 4)
    {
        Printf("%s [memptr] [nand_offset] [bytenum]\n", szArgv[0]);
        return 1;
    }
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("NAND_GetInfo failed\n");
        return 1;
    }
    
    u4MemPtr = StrToInt(szArgv[1]);
    u4Offset = StrToInt(szArgv[2]);
    u4Len    = StrToInt(szArgv[3]);

    if ((u4Len % rNandDev.u4PgSz) != 0)
    {
        Printf("Read length must be page alignment!\n");
        return 1;
    }
    
    x_memset(u1Spare, 0xFF, NAND_MAX_OOB_SIZE);
    if (NAND_ReadAttr((UINT64)u4Offset, u4MemPtr, (UINT32)u1Spare, u4Len) != 0)
    {
        i4Ret = 1;
        Printf("nand.read operates failed!\n");
    }
    else
    {
        i4Ret = 0;
        Printf("nand.read operates successfully.\n");
    }

    Printf("Main data:");
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

    Printf("Spare data:");
    ptr = (UINT8 *)u1Spare;
    for (i = 0; i < NAND_MAX_OOB_SIZE; i++) 
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
static INT32 _NANDCLI_Write(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8  *ptr;
    INT32  i, i4Ret;
    UINT32 u4Len, u4MemPtr, u4Offset;
    NANDDEV_T rNandDev;

    if (i4Argc < 4)
    {
        Printf("%s [memptr] [nand_offset] [bytenum]\n", szArgv[0]);
        return 1;
    }
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("NAND_GetInfo failed\n");
        return 1;
    }

    u4MemPtr = StrToInt(szArgv[1]);
    u4Offset = StrToInt(szArgv[2]);
    u4Len    = StrToInt(szArgv[3]);

    if ((u4Len % rNandDev.u4PgSz) != 0)
    {
        Printf("Write length must be page alignment!\n");
        return 1;
    }
    
    if (NAND_Write((UINT64)u4Offset, u4MemPtr, u4Len) != 0)
    {
        i4Ret = 1;
        Printf("nand.write operates failed\n");
    }
    else
    {
        i4Ret = 0;
        Printf("nand.write operates successfully.\n");
    }

    Printf("Main data:");
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
    return i4Ret;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_Erase(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Offset, u4Len;
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
    u4Len    = StrToInt(szArgv[2]);

    if (u4Len == 0)
    {
        Printf("Erase size is zero!\n");
        return 0;
    }

    u4PgStart = u4Offset / rNandDev.u4PgSz;
    u4PgEnd = (u4Offset + u4Len - 1) / rNandDev.u4PgSz;
    u4PgCount = u4PgEnd - u4PgStart + 1;

    u4BlkStart = u4Offset / rNandDev.u4BlkSz;
    u4BlkEnd = (u4Offset + u4Len - 1) / rNandDev.u4BlkSz;

    Printf("Starting Erase from block: %d ~ %d.\n", u4BlkStart, u4BlkEnd);
    NAND_Erase(u4PgStart, u4PgCount);
    Printf("nand.e operates Successfully.\n");
    
    return 0;
}

//-------------------------------------------------------------------------
static INT32 _NANDCLI_EraseForce(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Offset, u4Len;
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
    u4Len    = StrToInt(szArgv[2]);
    
    if (u4Len == 0)
    {
        Printf("Erase size is zero!\n");
        return 0;
    }
    
    u4PgStart = u4Offset / rNandDev.u4PgSz;
    u4PgEnd = (u4Offset + u4Len - 1) / rNandDev.u4PgSz;
    u4PgCount = u4PgEnd - u4PgStart + 1;
    
    u4BlkStart = u4Offset / rNandDev.u4BlkSz;
    u4BlkEnd = (u4Offset + u4Len - 1) / rNandDev.u4BlkSz;
    
    Printf("Starting EraseForce from block: %d ~ %d.\n", u4BlkStart, u4BlkEnd);
    if (NAND_EraseForce(u4PgStart, u4PgCount) != 0)
    {
        Printf("nand.ef operates failed!\n");
        return 1;
    }

    Printf("nand.ef operates successfully.\n");
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
    UINT32 i, j, p;
    UINT8 *pu1TestBuf = NULL, *pu1GoldenBuf = NULL;

    if (i4Argc != 3)
    {
        Printf("%s [nand_offset] [size]\n", szArgv[0]);
        return -1;
    }

    u4TestAddr = StrToInt(szArgv[1]);
    u4Len      = StrToInt(szArgv[2]);

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("NAND_GetInfo failed\n");
        return -1;
    }

    pu1TestBuf = (UINT8 *)NAND_TESTBUF1_ADDR;
    pu1GoldenBuf = (UINT8 *)NAND_TESTBUF2_ADDR;

    u4PgIdx = u4TestAddr / rNandDev.u4PgSz;
    u4PgCount = u4Len / rNandDev.u4PgSz;
    
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
        
        // Erase time
        Printf("Measure erase time:\n");
        HAL_GetTime(&time1);
        if (NAND_Erase(u4PgIdx, u4PgCount))
        {
            Printf("Erase NAND flash error!\n");
            return -1;
        }
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
        if (NAND_IsBadBlk(i))
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
static INT32 _NANDCLI_PartInfo(INT32 i4Argc, const CHAR **szArgv)
{
    NANDPART_ShowPartition();
    return 0;
}

static INT32 _NANDCLI_ubifs_Calculate(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 PartitionSize, Sp, P, B, O, overhead, vol_size;
    NANDDEV_T rNandDev;

    if (i4Argc != 2)
    {
        Printf("%s [size (bytes)]\n", szArgv[0]);
        return -1;
    }
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        Printf("NAND_GetInfo fail!\n");
        return 1;
    }
    
    Printf("\nNAND info:\n");
    if (rNandDev.u4PgSz % 0x400 == 0)
    {
        if (rNandDev.u4PgSz % 0x100000 == 0)
        {
            Printf("pagesize: %dM\n", rNandDev.u4PgSz/0x100000);
        }
        else
        {
            Printf("pagesize: %dK\n", rNandDev.u4PgSz/0x400);
        }
    }
    else
    {
        Printf("pagesize: %d\n", rNandDev.u4PgSz);
    }
        
    if (rNandDev.u4BlkSz % 0x400 == 0)
    {
        if (rNandDev.u4BlkSz % 0x100000 == 0)
        {
            Printf("blksize: %dM\n", rNandDev.u4BlkSz/0x100000);
        }
        else
        {
            Printf("blksize: %dK\n", rNandDev.u4BlkSz/0x400);
        }
    }
    else
    {
        Printf("blksize: %d\n", rNandDev.u4PgSz);
    }
    
    if (rNandDev.u4TotalSize % 0x400 == 0)
    {
        if (rNandDev.u4TotalSize % 0x100000 == 0)
        {
            Printf("chipsize: %dM\n", rNandDev.u4TotalSize/0x100000);
        }
        else
        {
            Printf("chipsize: %dK\n", rNandDev.u4TotalSize/0x400);
        }
    }
    else
    {
        Printf("chipsize: %d\n", rNandDev.u4PgSz);
    }
    
    PartitionSize = StrToInt(szArgv[1]);
    if (PartitionSize % rNandDev.u4BlkSz != 0)
    {
        Printf("Input partition size:%d is not blksize align!\n", PartitionSize);
        return 1;
    }
    
    if (PartitionSize < 25*rNandDev.u4BlkSz)
    {
        PartitionSize = 25*rNandDev.u4BlkSz;
        PartitionSize = (PartitionSize + 0x100000-1)& ~(0x100000-1);
        Printf("The smallest ubifs partition size: %dM!\n", PartitionSize/0x100000);
        return 1;
    }
    
    if (PartitionSize % 0x400 == 0)
    {
        if (PartitionSize % 0x100000 == 0)
        {
            Printf("PartitionSize: %dM\n", PartitionSize/0x100000);
        }
        else
        {
            Printf("PartitionSize: %dK\n", PartitionSize/0x400);
        }
    }
    else
    {
        Printf("PartitionSize: %d\n", PartitionSize);    
    }
        
    //output ubifs overhead size and vol_size
    Sp = rNandDev.u4BlkSz;
    P = PartitionSize/rNandDev.u4BlkSz;
    B = P/100 > 5 ? P/100 : 5;
    O = 2*rNandDev.u4PgSz;
    overhead = (B+4)*Sp + O*(P-B-4);
    overhead = (overhead + 0x100000-1)& ~(0x100000-1);
    vol_size = PartitionSize-overhead;
    vol_size = (vol_size + 0x100000-1)& ~(0x100000-1);
    
    Printf("\nubifs overhead size: %dM\n", overhead/0x100000);
    Printf("ubifs max vol_size: %dM\n", vol_size/0x100000);
    Printf("UBI_MAX_LOG_BLK_ERASE_CNT_xxx: %d\n", PartitionSize/rNandDev.u4BlkSz-1);
    Printf("\nNote:volume size can set only larger than image size. ubifs will relarge the volume size dynamicly to max size it can be, "
        "when the the config is: vol_type=dynamic vol_flags=autoresize.\n");
    
    return 0;
}

static CLI_EXEC_T arNandCmdTbl[] =
{
    DECLARE_CMD(_NANDCLI_State, state, st, "nand.st"),
    DECLARE_CMD(_NANDCLI_Read, read, r, "nand.read [memptr] [nand_offset] [bytenum]"),
    DECLARE_CMD(_NANDCLI_Write, write, w, "nand.write [memptr] [nand_offset] [bytenum]"),

    DECLARE_CMD(_NANDCLI_Erase, erase, e, "nand.e [nand_offset] [bytenum]"),
    DECLARE_CMD(_NANDCLI_EraseForce, eraseforce, ef, "erase force, ef [nand_offset] [bytenum]"),

    DECLARE_CMD(_NANDCLI_Measure, measure, m, "nand.m [nand_offset] [size]"),
    DECLARE_CMD(_NANDCLI_CheckBadBlk, bad, bad, "nand.bad"),
    DECLARE_CMD(_NANDCLI_MarkBadBlk, markbad, mbad, "nand.markbad [blk_idx]"),
    DECLARE_CMD(_NANDCLI_PartInfo, partinfo, partinfo, "nand.partinfo"),    
    DECLARE_CMD(_NANDCLI_ubifs_Calculate, ubifs, ubifs, "nand.ubifs [size]"), 
    
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

