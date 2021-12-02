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
#include "x_timer.h"
#include "x_os.h"
#include "x_hal_926.h"
#include "x_util.h"
#include "drv_pwm.h"
#include "gdma_if.h"
#include "x_rand.h"

#define CPU_TEST_ENABLE     0

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
 static INT32 _CMD_GDMA_Init(INT32 i4Argc, const CHAR** aszArgv)
 {
    return GDMA_Init();
 }
 static void vMemInverseSet(VOID *Src, VOID *Dst, UINT32 u4Len)
{
    UINT32 i;

    for(i = 0; i < u4Len; i++)
    {
        *(BYTE *)((UINT32)Dst + i) = ~(*(BYTE *)((UINT32)Src + i));
    }
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
static INT32 GDMA_NonOverlapMemcpy( VOID *dst, const VOID *src, UINT32 n)
{
    if ((NULL == dst)|| (NULL == src))
    {
        //Printf("Address Error, Check please!\n");
        return -1;
    }
    if (0 == n)
    {
        //Printf("Length Error, Check please!\n");
        return -1;
    }
    return GDMA_MemCpy(dst, src, n );
}

static INT32 _CMD_GDMA_Length_OFFSET(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4SrcAddr = 0;
    UINT32 u4DestAddr = 0;
    UINT32 u4Len = 0;
    UINT32 i,j,m;

    u4Len = 0x10000;
    u4SrcAddr =  (UINT32)x_mem_alloc(u4Len);
    u4DestAddr = (UINT32)x_mem_alloc(u4Len);

    for(i = 0; i < 16; i++)
    {
        for(j = 0; j < 16; j++)
        {
            for(m = 1; m <= 16; m++)
            {
                u4Len = m;
                vMemInverseSet((void *)(u4SrcAddr + j), (void *)(u4DestAddr + i), u4Len);
                GDMA_NonOverlapMemcpy((void *)(u4DestAddr + i),(void *)(u4SrcAddr + j),u4Len);
                if( 0 != x_memcmp((void *)(u4DestAddr + i), (void *)(u4SrcAddr + j), u4Len))
                {
                    Printf("[GDMA_Emu][Case][Error]:Source Address = 0x%x, Destination Address = 0x%x, Length = 0x%x!\n", u4SrcAddr + j, u4DestAddr + i, u4Len);
                    return -1;
                }
            }
        }
    }
    Printf("[GDMA_Emu][Length < 16][Address Offset=[0..15]][PASS]\n");
    return 1;
}

#define IOMMU_BASE 0xf0068000
#define _IOMMU_READ32(OFFSET)		IO_READ32(IOMMU_BASE,OFFSET)
#define _IOMMU_WRITE32(OFFSET,VALUE)	IO_WRITE32(IOMMU_BASE,OFFSET,VALUE)
#define _IOMMU_WRITE32MSK(OFFSET,VALUE,MASK)	IO_WRITE32MSK(IOMMU_BASE,OFFSET,VALUE,MASK)

#define MSK_INT_CLR0 (0x1<<0)
#define MSK_INT_EN0  (0x1<<1)
#define MSK_INT_CLR1 (0x1<<2)
#define MSK_INT_EN1  (0x1<<3)
#define MMU_CLEAR_INT_STA()\
    do {\
        _IOMMU_WRITE32MSK(0x110, MSK_INT_CLR0|MSK_INT_CLR1, MSK_INT_CLR0|MSK_INT_CLR1);\
        _IOMMU_WRITE32MSK(0x110, 0, MSK_INT_CLR0|MSK_INT_CLR1);\
    } while (0);

#define MSK_MON_PIN_SEL         (0x7<<8)
#define VAL_MON_PIN_SEL(x)      (((x)&0x7)<<8)
#define MSK_MON_PIN_RD          (0x1<<11)
#define VAL_MON_PIN_RD(x)       (((x)&0x1)<<11)
#define MSK_IOMMU_INT_HALT      (0x1<<20)
#define VAL_IOMMU_INT_HALT(x)   (((x)&0x1)<<20)
#define MSK_SELF_FIRE_TRIG      (UINT32)((UINT32)0x1<<31)
#define VAL_SELF_FIRE_TRIG(x)   (UINT32)(((UINT32)(x)&0x1)<<31)

#define MMU_CLEAR_SELF_FIRE() _IOMMU_WRITE32MSK(0x110, VAL_SELF_FIRE_TRIG(1), MSK_SELF_FIRE_TRIG)

#define IS_1MB_TABLE_ERROR() ((_IOMMU_READ32(0x120) & (0x1<<27)) != 0)
#define IS_4KB_TABLE_ERROR() ((_IOMMU_READ32(0x120) & (0x1<<28)) != 0)
UINT32 mmu_gdma_get_fault_addr(void)
{
    UINT32 u4Ret;
    _IOMMU_WRITE32MSK(0x110, VAL_MON_PIN_SEL(1)|VAL_MON_PIN_RD(1)|VAL_IOMMU_INT_HALT(1), MSK_MON_PIN_SEL|MSK_MON_PIN_RD|MSK_IOMMU_INT_HALT);
    u4Ret = _IOMMU_READ32(0x120);
    _IOMMU_WRITE32MSK(0x110, VAL_MON_PIN_SEL(0)|VAL_MON_PIN_RD(0)|VAL_IOMMU_INT_HALT(0), MSK_MON_PIN_SEL|MSK_MON_PIN_RD|MSK_IOMMU_INT_HALT);

    return u4Ret;
}

/* verification usage only */
UINT32 _get_4KB_buffer(void)
{
    UINT32 u4Ret;

    u4Ret = (UINT32)x_mem_alloc(0x1000*2);
    u4Ret = (u4Ret + (0x1000-1)) & ~(0x1000-1);

    return u4Ret;
}

UINT32 my_memcmp(UINT8* u4DestAddr, UINT8* u4SrcAddr, UINT32 u4Len)
{
    UINT32 u4Ret = 0;
    UINT32 u4TmpLen;
    UINT8 *u4TmpSrc, *u4TmpDest;
    u4TmpLen = u4Len;
    u4TmpDest = u4DestAddr;
    u4TmpSrc = u4SrcAddr;

    while (u4TmpLen != 0)
    {
        if ((*u4TmpDest) != (*u4TmpSrc))
        {
            //Printf("u4SrcAddr 0x%x = 0x%x\n", u4TmpSrc, (*u4TmpSrc));
            //Printf("u4DestAddr 0x%x = 0x%x\n", u4TmpDest, (*u4TmpDest));
        }
        u4TmpDest++;
        u4TmpSrc++;
        u4TmpLen--;
    }

    while (u4Len != 0)
    {
        if ((*u4DestAddr) != (*u4SrcAddr))
        {
            Printf("u4SrcAddr 0x%x = 0x%x\n", u4SrcAddr, (*u4SrcAddr));
            Printf("u4DestAddr 0x%x = 0x%x\n", u4DestAddr, (*u4DestAddr));
            u4Ret = 1;
            break;
        }
        u4DestAddr++;
        u4SrcAddr++;
        u4Len--;
    }
    return u4Ret;
}

void HalSet1MSectionTable(UINT32 u4PhyAddr, UINT32 u4VirtAdr);
void HalSet4KPageTable(UINT32 u4PhyAddr, UINT32 u4VirtAdr, UINT32* pu4PageTableAddress);

#if 0
void HalSetL1Entry(UINT32 u4Value, UINT32 u4VirtAdr)
{
    UINT32 *pu4Table = (UINT32*)_u4MMUTableAddress;
    pu4Table[(u4VirtAdr>>20)] = u4Value;
}
#endif
#define HalSetL1Entry(u4Value, u4VirtAdr)
#if 0
void HalSetL1Entry(UINT32 u4Value, UINT32 u4VirtAdr)
{
    UNUSED(u4Value);
    UNUSED(u4VirtAdr);
    return;
}
#endif

static INT32 _CMD_GDMA_Length_OFFSET_MMU(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4SrcAddr = 0, u4PhySrcAddr=0;
    UINT32 u4DestAddr = 0, u4PhyDestAddr=0;
    UINT32 u4Len = 0x10000;
    UINT32 i,j,m;
    UINT32 u4Mode=0;
    UINT32 u4Table4Src, u4Table4Dst, u4AlignTable4Src, u4AlignTable4Dst, u4Offset=16;

    if (i4Argc >= 2)
    {
        u4Mode = StrToInt(aszArgv[1]);
    }
    if (i4Argc >= 3)
    {
        u4Len = StrToInt(aszArgv[2]);
    }
    if (i4Argc >= 4)
    {
        u4Offset = StrToInt(aszArgv[3]);
    }

    if (u4Mode == 0)
    {
        u4PhySrcAddr = u4SrcAddr = (UINT32)x_mem_alloc(u4Len);
        u4PhyDestAddr = u4DestAddr = (UINT32)x_mem_alloc(u4Len);
    }
    else if (u4Mode == 1)
    {
        // 1M entry, total size = 2MB
        HalSet1MSectionTable(0x8100000, 0x9000000); //phy=129M, virt=144M
        HalSet1MSectionTable(0x8000000, 0x9100000); //phy=128M, virt=145M
        HalSet1MSectionTable(0x8300000, 0x9200000); //phy=131M, virt=146M
        HalSet1MSectionTable(0x8200000, 0x9300000); //phy=130M, virt=147M
        //HalSet1MSectionTable(0x8500000, 0x9400000); //phy=133M, virt=148M
        //HalSet1MSectionTable(0x8400000, 0x9500000); //phy=132M, virt=149M
        u4SrcAddr = 0x9000000; u4PhySrcAddr = 0x8100000;
        u4DestAddr = 0x9200000; u4PhyDestAddr = 0x8000000;

        if (u4Len > 0x200000)
        {
            u4Len = 0x200000;
        }
    }
    else if (u4Mode == 2)
    {
        // 1M entry, total size = 1MB
        HalSet1MSectionTable(0x8500000, 0xb9000000); //phy=133M, virt=2960M
        HalSet1MSectionTable(0x8400000, 0xb9100000); //phy=132M, virt=2961M
        u4SrcAddr = 0xb9000000; u4PhySrcAddr = 0x8500000;
        u4DestAddr = 0xb9100000; u4PhyDestAddr = 0x8400000;

        if (u4Len > 0x100000)
        {
            u4Len = 0x100000;
        }
    }
    else if (u4Mode == 3)
    {
        // 1M entry, total size = 3MB
        HalSet1MSectionTable(0x8100000, 0x9000000); //phy=129M, virt=144M
        HalSet1MSectionTable(0x8000000, 0x9100000); //phy=128M, virt=145M
        HalSet1MSectionTable(0x8200000, 0x9200000); //phy=131M, virt=146M
        HalSet1MSectionTable(0x8300000, 0x9300000); //phy=130M, virt=147M
        HalSet1MSectionTable(0x8500000, 0x9400000); //phy=133M, virt=148M
        HalSet1MSectionTable(0x8400000, 0x9500000); //phy=132M, virt=149M
        u4SrcAddr = 0x9000000; u4PhySrcAddr = 0x8100000;
        u4DestAddr = 0x9300000; u4PhyDestAddr = 0x8300000;

        if (u4Len > 0x300000)
        {
            u4Len = 0x300000;
        }
    }
    else if (u4Mode == 0x21)
    {
        // 1M entry, produce 1M table error
        //HalSet1MSectionTable(0x8500000, 0xb9000000); //phy=133M, virt=2960M
        //HalSet1MSectionTable(0x8400000, 0xb9100000); //phy=132M, virt=2961M
        u4SrcAddr = 0xbb000000; u4PhySrcAddr = 0x8500000;
        u4DestAddr = 0xbb100000; u4PhyDestAddr = 0x8400000;
        HalSet1MSectionTable(0x8500000, 0xbb000000);
        HalSet1MSectionTable(0x8400000, 0xbb100000);
    }
    else if (u4Mode == 0x22)
    {
        // 1M entry, produce 1M table error
        //HalSet1MSectionTable(0x8500000, 0xb9000000); //phy=133M, virt=2960M
        //HalSet1MSectionTable(0x8400000, 0xb9100000); //phy=132M, virt=2961M
        u4SrcAddr = 0xbb000000; u4PhySrcAddr = 0x8500000;
        u4DestAddr = 0xbb100000; u4PhyDestAddr = 0x8400000;
        HalSetL1Entry(0x00, 0xbb000000);
        HalSetL1Entry(0x00, 0xbb100000);
    }
    else if (u4Mode == 0x23)
    {
        // 1M entry, produce 1M table error
        u4SrcAddr = 0xbb000000; u4PhySrcAddr = 0x8500000;
        u4DestAddr = 0xbb100000; u4PhyDestAddr = 0x8400000;
        HalSet1MSectionTable(0x8500000, 0xbb000000);
        HalSetL1Entry(0x00, 0xbb100000);
    }
    else if (u4Mode == 0x24)
    {
        // 1M entry, produce 1M table error
        u4SrcAddr = 0xbb000000; u4PhySrcAddr = 0x8500000;
        u4DestAddr = 0xbb100000; u4PhyDestAddr = 0x8400000;
        HalSetL1Entry(0x00, 0xbb000000);
        HalSet1MSectionTable(0x8400000, 0xbb100000);
    }
    else if (u4Mode == 0x28)
    {
        // 4K entry, produce 1M table error
        u4SrcAddr = 0x9602000; u4PhySrcAddr = 0x8502000;
        u4DestAddr = 0xA640000; u4PhyDestAddr = 0x8803000;
    }
    else if (u4Mode == 0x11)
    {
        // 4K entry, total size: 8K
        u4Table4Src = (UINT32)x_mem_alloc(0x400*3);
        u4Table4Dst = (UINT32)x_mem_alloc(0x400*3);
        x_memset((void*)u4Table4Src, 0, 0x400*3);
        x_memset((void*)u4Table4Dst, 0, 0x400*3);

        u4AlignTable4Src = (u4Table4Src+(0x400-1)) & ~(0x400-1);
        u4AlignTable4Dst = (u4Table4Dst+(0x400-1)) & ~(0x400-1);

        HalSet4KPageTable(0x8500000, 0x9600000, (UINT32*)u4AlignTable4Src); //phy=133M0K, virt=150M0K, page table[0x96], entry[0]
        HalSet4KPageTable(0x8501000, 0x9601000, (UINT32*)u4AlignTable4Src); //phy=133M4K, virt=150M4K, page table[0x96], entry[0]

        HalSet4KPageTable(0x8800000, 0xA600000, (UINT32*)u4AlignTable4Dst); //phy=136M0K, virt=166M0K, page table[0xA6], entry[0]
        HalSet4KPageTable(0x8801000, 0xA601000, (UINT32*)u4AlignTable4Dst); //phy=136M4K, virt=166M4K, page table[0xA6], entry[0]

        u4SrcAddr = 0x9600000; u4PhySrcAddr = 0x8500000;
        u4DestAddr = 0xA600000; u4PhyDestAddr = 0x8800000;

        if (u4Len > 0x2000)
        {
            u4Len = 0x2000;
        }
    }
    else if (u4Mode == 0x12)
    {
        UINT32 u4Tmp, u4Addr, u4PhyAddr;

        // 4K entry, total size: 8K
        u4Table4Src = (UINT32)x_mem_alloc(0x400*3);
        u4Table4Dst = (UINT32)x_mem_alloc(0x400*3);
        x_memset((void*)u4Table4Src, 0, 0x400*3);
        x_memset((void*)u4Table4Dst, 0, 0x400*3);

        u4AlignTable4Src = (u4Table4Src+(0x400-1)) & ~(0x400-1);
        u4AlignTable4Dst = (u4Table4Dst+(0x400-1)) & ~(0x400-1);

        u4SrcAddr = u4PhySrcAddr = 0x9600000;
        u4DestAddr = u4PhyDestAddr = 0xA600000;

        u4Addr = 0x9600000;
        u4PhyAddr = u4PhySrcAddr = 0x8500000;
        for (u4Tmp=0; u4Tmp<u4Len; u4Tmp+=0x1000)
        {
            HalSet4KPageTable(_get_4KB_buffer(), u4Addr, (UINT32*)u4AlignTable4Src); //phy=133M0K, virt=150M0K, page table[0x96], entry[0]
            //HalSet4KPageTable(u4PhyAddr, u4Addr, (UINT32*)u4AlignTable4Src); //phy=133M0K, virt=150M0K, page table[0x96], entry[0]
            u4Addr += 0x1000;
            u4PhyAddr += 0x1000;
        }

        u4Addr = 0xA600000;
        u4PhyAddr = u4PhyDestAddr = 0x8800000;
        for (u4Tmp=0; u4Tmp<u4Len; u4Tmp+=0x1000)
        {
            HalSet4KPageTable(_get_4KB_buffer(), u4Addr, (UINT32*)u4AlignTable4Dst); //phy=133M0K, virt=150M0K, page table[0x96], entry[0]
            //HalSet4KPageTable(u4PhyAddr, u4Addr, (UINT32*)u4AlignTable4Dst); //phy=133M0K, virt=150M0K, page table[0x96], entry[0]
            u4Addr += 0x1000;
            u4PhyAddr += 0x1000;
        }

        Printf("build table from 0x%x to 0x%x, length = 0x%x\n", u4SrcAddr, u4DestAddr, u4Len);

        if (u4Len > 0x200000)
        {
            u4Len = 0x200000;
        }
    }

    for(i = 0; i < u4Offset; i++)
    {
        for(j = 0; j < u4Offset; j++)
        {
            for(m = 1; m <= 16; m++)
            {
                vMemInverseSet((void *)(u4PhySrcAddr + j), (void *)(u4PhyDestAddr + i), m);
                GDMA_NonOverlapMemcpy((void *)(u4DestAddr + i),(void *)(u4SrcAddr + j),m);
                #if 0
                // compare via virtual address
                if( 0 != x_memcmp((void *)(u4DestAddr + i), (void *)(u4SrcAddr + j), m))
                {
                    Printf("[GDMA_Emu][Case][Error]:Source Address = 0x%x, Destination Address = 0x%x, Length = 0x%x!\n", u4SrcAddr + j, u4DestAddr + i, m);
                    return -1;
                }
                #endif
                // compare via physical address
                if( 0 != x_memcmp((void *)(u4PhyDestAddr + i), (void *)(u4PhySrcAddr + j), m))
                {
                    for (;m>0;m--)
                    {
                    Printf("PhyDest=0x%x,PhySrc=0x%x\n", *(BYTE *)(u4PhyDestAddr + i), *(BYTE*)(u4PhySrcAddr + j));
                    }
                    Printf("[GDMA_Emu][Case][Error]:Source Address = 0x%x, Destination Address = 0x%x, Length = 0x%x!\n", u4SrcAddr + j, u4DestAddr + i, m);
                    goto gdma_offset_mmu_exit;
                    //return -1;
                }
            }
        }
    }
    Printf("[GDMA_Emu][Length < 16][Address Offset=[0..15]][PASS]: Source Address = 0x%x, Destination Address = 0x%x\n", u4SrcAddr, u4DestAddr);

    vMemInverseSet((void *)(u4PhySrcAddr), (void *)(u4PhyDestAddr), u4Len);
    GDMA_NonOverlapMemcpy((void *)(u4DestAddr),(void *)(u4SrcAddr), u4Len);
    //HalFlushDCache();
    HalFlushInvalidateDCache();
    HalInvalidateTLB();
    //HalDisableICache();
    //HalDisableDCache();
    //x_thread_delay(8000);
    if (!IS_4KB_TABLE_ERROR() && !IS_1MB_TABLE_ERROR())
    {
        // compare via physical address
        if( 0 != my_memcmp((void *)(u4DestAddr), (void *)(u4SrcAddr), u4Len))
        {
            Printf("[GDMA_Emu][Error]:Source Address = 0x%x, Destination Address = 0x%x, Length = 0x%x!\n", u4SrcAddr, u4DestAddr, u4Len);
            goto gdma_offset_mmu_exit;
            //return -1;
        }
    }
    Printf("[GDMA_Emu][PASS]: Source Address = 0x%x, Destination Address = 0x%x, Length = 0x%x\n", u4SrcAddr, u4DestAddr, u4Len);
    #if 0
    for(i = 0; i < u4Offset; i++)
    {
        for(j = 0; j < u4Offset; j++)
        {
            for(m = 1; m <= 16; m++)
            {
                UINT32 u4RanLen;

                srand(m); // real random value !?
                u4RanLen = (UINT32)random(u4Len);
                vMemInverseSet((void *)(u4PhySrcAddr + j), (void *)(u4PhyDestAddr + i), u4RanLen);
                GDMA_NonOverlapMemcpy((void *)(u4DestAddr + i),(void *)(u4SrcAddr + j),u4RanLen);
                // compare via physical address
                if( 0 != x_memcmp((void *)(u4PhyDestAddr + i), (void *)(u4PhySrcAddr + j), u4RanLen))
                {
                    Printf("[GDMA_Emu][Case][Error]:Source Address = 0x%x, Destination Address = 0x%x, Random Length = 0x%x!\n", u4SrcAddr + j, u4DestAddr + i, u4RanLen);
                    return -1;
                }
            }
        }
    }
    #endif

gdma_offset_mmu_exit:
    if (IS_4KB_TABLE_ERROR())
    {
        Printf("4KB TABLE Error Address = 0x%x\n", mmu_gdma_get_fault_addr());
        MMU_CLEAR_INT_STA();
    }
    else if (IS_1MB_TABLE_ERROR())
    {
        Printf("1MB TABLE Error Address = 0x%x\n", mmu_gdma_get_fault_addr());
        MMU_CLEAR_INT_STA();
    }
    else
    {
        Printf("no TABLE Error\n");
    }
    MMU_CLEAR_SELF_FIRE();

    return 1;
}

static INT32 AUTO_TEST(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 MAXLEN;
    UINT32 u4BufferStart = 0;
    UINT32 u4BufferEnd = 0;
    UINT32 u4SrcAddr;
    UINT32 u4DstAddr;
    UINT32 u4Len;
    UINT32 u4Random = 0;

    if (i4Argc < 2)
    {
        Printf("GDMA_AUTO_TEST Need more arg!\n");
        return -1 ;
    }

    MAXLEN = StrToInt(aszArgv[1]);

	if(MAXLEN < 2)
    {
        Printf("MAXSIZE is too small!\n");
        return -1;
    }

    u4BufferStart = (UINT32)x_mem_alloc(MAXLEN);

    if(!u4BufferStart)
    {
        Printf("MAXSIZE is too large!\n");
        return -1;
    }
    srand(3);
    while(1)
    {
        u4BufferEnd = u4BufferStart + MAXLEN;
        do
        {
            u4SrcAddr = u4BufferStart + (UINT32)random(MAXLEN);
            u4DstAddr = u4BufferStart + (UINT32)random(MAXLEN);

            u4Random = MIN(MIN((u4BufferEnd - u4SrcAddr),(u4BufferEnd - u4DstAddr)),((u4DstAddr > u4SrcAddr)?( u4DstAddr- u4SrcAddr):(u4SrcAddr - u4DstAddr)));

        }while(u4Random == 0);

        u4Len = (UINT32)random(u4Random);

        //Printf("[GDMA_Emu][Case]:Source Address = 0x%x, Destination Address = 0x%x, Length = 0x%x!\n", u4SrcAddr, u4DstAddr, u4Len);

        vMemInverseSet((void *)u4SrcAddr, (void *)u4DstAddr, u4Len);

        GDMA_NonOverlapMemcpy((void *)u4DstAddr, (void *)u4SrcAddr, u4Len);

        if( 0 != x_memcmp((void *)u4DstAddr, (void *)u4SrcAddr, (u4Len & 0xFFFFFF)))
        {
            Printf("Memory Compare Error!\n");
            Printf("[GDMA_Emu][Case]:Source Address = 0x%x, Destination Address = 0x%x, Length = 0x%x!\n", u4SrcAddr, u4DstAddr, (u4Len & 0xFFFFFF));
            VERIFY(0);
        }
        else
        {
            Printf("[GDMA_Emu][Case][PASS]:Source Address = 0x%x, Destination Address = 0x%x, Length = 0x%x!\n", u4SrcAddr, u4DstAddr, (u4Len & 0xFFFFFF));
        }
    }


}

static INT32 AUTO_FILL(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 MAXLEN;
    UINT32 u4BufferStart = 0;
    UINT32 u4Len;
    UINT32 u4DstAddr;
    UINT32 u4Value;
    UINT32 i;
    UINT32 u4RandOff=0;

    if (i4Argc < 2)
    {
        Printf("GDMA_AUTO_FILL Need more arg!\n");
        return -1 ;
    }
    MAXLEN = StrToInt(aszArgv[1]);

    if (i4Argc >=3)
    {
        u4RandOff= StrToInt(aszArgv[2]);
    }

    u4BufferStart = (UINT32)x_mem_alloc(MAXLEN);

    if(!u4BufferStart)
    {
        Printf("MAXSIZE is too large!\n");
        return -1;
    }
    srand(3);
    while(1)
    {
        u4DstAddr = u4BufferStart + (UINT32)random((MAXLEN - 1));
        //u4DstAddr = (u4DstAddr + 3) & ~0x3;// 4 bytes alignment
        if (u4RandOff != 0)
        {
            u4DstAddr = (u4DstAddr + 3) & ~0x3;
            u4DstAddr += (u4RandOff % 4);
        }
        u4Len = (UINT32)random((u4BufferStart + MAXLEN - u4DstAddr));
        if (u4Len == 0)
            u4Len = 1;
        u4Value = (UINT32)rand();
        Printf("[GDMA_Emu][Case]:Destination Address = 0x%x, Length = 0x%x, Value = 0x%x!\n",u4DstAddr, u4Len, u4Value);

        GDMA_MemSet((VOID *)u4DstAddr, u4Value , u4Len);
        Printf("        GDMA_MemSet(0x%x, 0x%x, 0x%x)\n", u4DstAddr, u4Value, u4Len);

        for(i = 0; i < u4Len ; i++)
        {
            if(*(BYTE *)(u4DstAddr + i) != ((u4Value >> (((u4DstAddr + i) % 4) * 8)) & 0xFF))
            {
                Printf("Fail @ %d, Destvalue = %x, Srcvalue= %x \n", i,*(BYTE *)(u4DstAddr + i), ((u4Value >> (((u4DstAddr + i) % 4) * 8)) & 0xFF));
                Printf("Memory Compare Error!\n");
                return -1;
                //VERIFY(0);
            }
        }
        Printf("Memory Compare Pass!\n");

    }
}


#ifdef __MODEL_slt__
static INT32 SLT_TEST(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4SrcAddr = 0, u4PhySrcAddr=0;
    UINT32 u4DestAddr = 0, u4PhyDestAddr=0;
    UINT32 u4Len = 0x10000;
    UINT32 i,j,m;
    UINT32 u4Table4Src, u4Table4Dst, u4AlignTable4Src, u4AlignTable4Dst, u4Offset=16;
    UINT32 fgErrorLog = 0;
    UINT32 fgError = 0;

    if (i4Argc >= 3)
    {
        u4Len = StrToInt(aszArgv[2]);
    }
    if (i4Argc >= 4)
    {
        u4Offset = StrToInt(aszArgv[3]);
    }
    if (i4Argc >= 2)
    {
        fgErrorLog = StrToInt(aszArgv[1]);
    }

    GDMA_Init();

    // 1M entry test
    {
        // 1M entry, total size = 2MB
        HalSet1MSectionTable(0x8100000, 0x9000000); //phy=129M, virt=144M
        HalSet1MSectionTable(0x8000000, 0x9100000); //phy=128M, virt=145M
        //HalSet1MSectionTable(0x8500000, 0x9400000); //phy=133M, virt=148M
        //HalSet1MSectionTable(0x8400000, 0x9500000); //phy=132M, virt=149M
        u4SrcAddr = 0x9000000; u4PhySrcAddr = 0x8100000;
        u4DestAddr = 0x9100000; u4PhyDestAddr = 0x8000000;

        if (u4Len > 0x100000)
        {
            u4Len = 0x100000;
        }
    }
    for(i = 0; i < u4Offset; i++)
    {
        for(j = 0; j < u4Offset; j++)
        {
            for(m = 1; m <= 16; m++)
            {
                vMemInverseSet((void *)(u4PhySrcAddr + j), (void *)(u4PhyDestAddr + i), m);
                GDMA_NonOverlapMemcpy((void *)(u4DestAddr + i),(void *)(u4SrcAddr + j),m);
                // compare via physical address
                if( 0 != x_memcmp((void *)(u4PhyDestAddr + i), (void *)(u4PhySrcAddr + j), m))
                {
                    if (fgErrorLog)
                    {
                        Printf("[GDMA SLT 1M Entry][Case][Error]:Source Address = 0x%x, Destination Address = 0x%x, Length = 0x%x!\n", u4SrcAddr + j, u4DestAddr + i, m);
                        for (;m>0;m--)
                        {
                        Printf("PhyDest=0x%x,PhySrc=0x%x\n", *(BYTE *)(u4PhyDestAddr + i), *(BYTE*)(u4PhySrcAddr + j));
                        }
                    }
                    Printf("[GDMA SLT]:FAIL\n");
                        fgError = 1;

                    goto gdma_offset_mmu_exit;
                    //return -1;
                }
            }
        }
    }
    if (fgErrorLog)
    {
        Printf("[GDMA SLT 1M Entry][PASS]: Source Address = 0x%x, Destination Address = 0x%x, Length = 0x%x\n", u4SrcAddr, u4DestAddr, u4Len);
    }

    // 4k entry test
    {
        // 4K entry, total size: 8K
        u4Table4Src = (UINT32)x_mem_alloc(0x400*3);
        u4Table4Dst = (UINT32)x_mem_alloc(0x400*3);
        x_memset((void*)u4Table4Src, 0, 0x400*3);
        x_memset((void*)u4Table4Dst, 0, 0x400*3);

        u4AlignTable4Src = (u4Table4Src+(0x400-1)) & ~(0x400-1);
        u4AlignTable4Dst = (u4Table4Dst+(0x400-1)) & ~(0x400-1);

        HalSet4KPageTable(0x8500000, 0x9600000, (UINT32*)u4AlignTable4Src); //phy=133M0K, virt=150M0K, page table[0x96], entry[0]
        HalSet4KPageTable(0x8501000, 0x9601000, (UINT32*)u4AlignTable4Src); //phy=133M4K, virt=150M4K, page table[0x96], entry[0]

        HalSet4KPageTable(0x8800000, 0xA600000, (UINT32*)u4AlignTable4Dst); //phy=136M0K, virt=166M0K, page table[0xA6], entry[0]
        HalSet4KPageTable(0x8801000, 0xA601000, (UINT32*)u4AlignTable4Dst); //phy=136M4K, virt=166M4K, page table[0xA6], entry[0]

        u4SrcAddr = 0x9600000; u4PhySrcAddr = 0x8500000;
        u4DestAddr = 0xA600000; u4PhyDestAddr = 0x8800000;

        if (u4Len > 0x2000)
        {
            u4Len = 0x2000;
        }
    }

    for(i = 0; i < u4Offset; i++)
    {
        for(j = 0; j < u4Offset; j++)
        {
            for(m = 1; m <= 16; m++)
            {
                vMemInverseSet((void *)(u4PhySrcAddr + j), (void *)(u4PhyDestAddr + i), m);
                GDMA_NonOverlapMemcpy((void *)(u4DestAddr + i),(void *)(u4SrcAddr + j),m);
                // compare via physical address
                if( 0 != x_memcmp((void *)(u4PhyDestAddr + i), (void *)(u4PhySrcAddr + j), m))
                {
                    if (fgErrorLog)
                    {
                        Printf("[GDMA SLT 4K Entry][Case][Error]:Source Address = 0x%x, Destination Address = 0x%x, Length = 0x%x!\n", u4SrcAddr + j, u4DestAddr + i, m);
                        for (;m>0;m--)
                        {
                        Printf("PhyDest=0x%x,PhySrc=0x%x\n", *(BYTE *)(u4PhyDestAddr + i), *(BYTE*)(u4PhySrcAddr + j));
                        }
                    }
                    Printf("[GDMA SLT]:FAIL\n");
                        fgError = 1;

                    goto gdma_offset_mmu_exit;
                    //return -1;
                }
            }
        }
    }
    if (fgErrorLog)
    {
        Printf("[GDMA SLT 4K Entry][PASS]: Source Address = 0x%x, Destination Address = 0x%x, Length = 0x%x\n", u4SrcAddr, u4DestAddr, u4Len);
    }
    Printf("[GDMA SLT]:PASS\n");

gdma_offset_mmu_exit:
    if (IS_4KB_TABLE_ERROR())
    {
        if (fgErrorLog)
        {
        Printf("4KB TABLE Error Address = 0x%x\n", mmu_gdma_get_fault_addr());
        }
        MMU_CLEAR_INT_STA();
    }
    else if (IS_1MB_TABLE_ERROR())
    {
        if (fgErrorLog)
        {
        Printf("1MB TABLE Error Address = 0x%x\n", mmu_gdma_get_fault_addr());
        }
        MMU_CLEAR_INT_STA();
    }
    else
    {
        if (fgErrorLog)
        {
        Printf("no TABLE Error\n");
        }
    }
    MMU_CLEAR_SELF_FIRE();

    //for slt.misc cmd
    if (fgError)
		return -1;

    return 1;
}
#endif // __MODEL_slt__


static CLI_EXEC_T _arGDMACmdTbl[] =
{
    {
        "GDMA Init",
        "init",
        _CMD_GDMA_Init,
        NULL,
        "gdma init function",
        CLI_GUEST
    },
    {
        "GDMA test",
        "normal",
        _CMD_GDMA_Length_OFFSET,
        NULL,
        "gdma basic test",
        CLI_GUEST
    },
    {
        "GDMA MMU test",
        "mmu",
        _CMD_GDMA_Length_OFFSET_MMU,
        NULL,
        "gdma MMU test",
        CLI_GUEST
    },
    {
        "GDMA auto test",
        "auto",
        AUTO_TEST,
        NULL,
        "gdma auto test",
        CLI_GUEST
    },
    {
        "GDMA Fill Mode",
        "fill",
        AUTO_FILL,
        NULL,
        "gdma fill test",
        CLI_GUEST
    },
#ifdef __MODEL_slt__
    {
        "GDMA MMU slt test",
        "slt",
        SLT_TEST,
        NULL,
        "GDMA MMU slt test",
        CLI_GUEST
    },
#endif
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
CLI_MAIN_COMMAND_ITEM(GDMA)
{
    "gdma",
    NULL,
    NULL,
    _arGDMACmdTbl,
    "gdma module test",
    CLI_GUEST
};

