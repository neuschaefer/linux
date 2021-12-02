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
 * $RCSfile: hal_926.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file hal_926.c
 *  hal_926.c provides functions of IRQ/FIQ, MMU, and Cache.
 */


//lint --e{950} No ANSI reserved word (__asm)
//lint --e{529} Symbol not subsequently referenced
//lint --e{530} Symbol not initialized
//lint --e{715} Symbol not referenced
//lint --e{506} Constant value Boolean
//lint --e{641} Converting enum to int


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_hal_926.h"
#include "x_assert.h"


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------
// IRQ/FIQ related definitions
//---------------------------------------------------------------------

// PSR bit definitions
#define PSR_IRQ_DISABLE             (1 << 7)
#define PSR_FIQ_DISABLE             (1 << 6)

//---------------------------------------------------------------------
// Cache related definitions
//---------------------------------------------------------------------

// CP15 control register (c1) bit definitions
#define ICACHE_ENABLE               (1 << 12)
#define DCACHE_ENABLE               (1 << 2)
#define MMU_ENABLE                  (1 << 0)

// CP15 cache type register (c0) bit definitions
#define CTYPE_MASK                  0xf
#define CTYPE_SHIFT                 0x25
#define S_MASK                      1
#define S_SHIFT                     24
#define DSIZE_MASK                  0xf
#define DSIZE_SHIFT                 (6 + 12)
#define ISIZE_MASK                  0xf
#define ISIZE_SHIFT                 6

// Translate VA (Virtual Address) to MVA (Modified Virtual Address)
#define MVA(addr)                   ((addr) & 0xffffffe0)

// Cache parameters (in bytes)
#define ICACHE_LINE_SIZE            32
#define DCACHE_LINE_SIZE            32

//---------------------------------------------------------------------
// Page table related definitions
//---------------------------------------------------------------------

// Domain access control definitions of CP15 register r3
enum DOMAIN_ACCESS
{
    NO_ACCESS = 0,
    CLIENT = 1,
    RESERVED = 2,
    MANAGER = 3
};

enum PAGE_TABLE_ENTRY_TYPE
{
    INVALID = 0,
    COARSE_PAGE = 1,
    SECTION = 2,
    FINE_PAGE = 3
};

// Bit definitions of page table
#define C_BIT           (1 << 3)    // Cachable
#define B_BIT           (1 << 2)    // Bufferable
#define U_BIT           (1 << 4)    // Must be 1, for backward compatibility


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

// Macro definition for single level 1 entry of page table
#define L1Entry(type, addr, dom, ucb, acc) \
    ((type == SECTION) \
        ? (((addr) & 0xfff00000) | ((acc) << 10) | ((dom) << 5) | (ucb) | \
            U_BIT | (type)) \
        : ((type == COARSE_PAGE) \
            ? (((addr) &0xfffffc00) | ((dom) << 5) | U_BIT | (type)) \
            : 0))
#pragma arm

#ifndef CC_5391_PRELOADER

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

EXTERN UINT32 BSP_GetMemSize(void);


//===========================================================================
// IRQ/FIQ related functions
//===========================================================================

//-----------------------------------------------------------------------------
/** HalEnableIRQ() Enable IRQ
 */
//-----------------------------------------------------------------------------
void HalEnableIRQ(void)
{
    UINT32 r;

    __asm
    {
        MRS     r, CPSR
        BIC     r, r, #PSR_IRQ_DISABLE
        MSR     CPSR_c, r
    }
}

//-----------------------------------------------------------------------------
/** HalDisableIRQ() Disable IRQ
 */
//-----------------------------------------------------------------------------
void HalDisableIRQ(void)
{
    UINT32 r;

    __asm
    {
        MRS     r, CPSR
        ORR     r, r, #PSR_IRQ_DISABLE
        MSR     CPSR_c, r
    }
}

//-----------------------------------------------------------------------------
/** HalEnableFIQ() Enable FIQ
 */
//-----------------------------------------------------------------------------
void HalEnableFIQ(void)
{
    UINT32 r;

    __asm
    {
        MRS     r, CPSR
        BIC     r, r, #PSR_FIQ_DISABLE
        MSR     CPSR_c, r
    }
}

//-----------------------------------------------------------------------------
/** HalCriticalStart() Enter critical section, disable IRQ and FIQ
 *  @return The current processor status, which must be carried back to \n
 *               HalCritialEnd()
 */
//-----------------------------------------------------------------------------
UINT32 HalCriticalStart(void)
{
    UINT32 r, s;

    __asm
    {
        MRS     r, CPSR
        ORR     s, r, #(PSR_IRQ_DISABLE | PSR_FIQ_DISABLE)
        MSR     CPSR_c, s
    }

    return r;
}

//-----------------------------------------------------------------------------
/** HalCritialSemiStart() Enter critical section, disable IRQ (FIQ is still \n
 *                      enabled)
 *  @return The current processor status, which must be carried back to \n
 *               HalCritialEnd()
 */
//-----------------------------------------------------------------------------
UINT32 HalCriticalSemiStart(void)
{
    UINT32 r, s;

    __asm
    {
        MRS     r, CPSR
        ORR     s, r, #PSR_IRQ_DISABLE
        MSR     CPSR_c, s
    }

    return r;
}

//-----------------------------------------------------------------------------
/** HalCritialEnd() Leave critical section, restore IRQ and/or FIQ status
 *  @param u4Flags - The return value of the corresponding \n
 *                     HalCriticalStart() or HalCriticalSemiStart()
 */
//-----------------------------------------------------------------------------
void HalCriticalEnd(UINT32 u4Flags)
{
    __asm
    {
        MSR     CPSR_c, u4Flags
    }
}

//-----------------------------------------------------------------------------
/** HalDisableFIQ() Disable FIQ
 */
//-----------------------------------------------------------------------------
void HalDisableFIQ(void)
{
    UINT32 r;

    __asm
    {
        MRS     r, CPSR
        ORR     r, r, #PSR_FIQ_DISABLE
        MSR     CPSR_c, r
    }
}

//===========================================================================
// Cache related functions
//===========================================================================

//-----------------------------------------------------------------------------
/** HalIsICacheEnabled() Check if I-cache enabled
 * @return      : TRUE if I-cache enabled, FALSE if I-cache disabled
 */
//-----------------------------------------------------------------------------
BOOL HalIsICacheEnabled(void)
{
    UINT32 r;

    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
    }

    return ((r & ICACHE_ENABLE) ? TRUE : FALSE);
}

//-----------------------------------------------------------------------------
/** HalIsDCacheEnabled() Check if D-cache enabled
 *  @return      : TRUE if D-cache enabled, FALSE if D-cache disabled
 */
//-----------------------------------------------------------------------------
BOOL HalIsDCacheEnabled(void)
{
    UINT32 r;

    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
    }

    return ((r & DCACHE_ENABLE) ? TRUE : FALSE);
}

//-----------------------------------------------------------------------------
/** HalGetICacheSize() Get I-cache size
 *  @return      : The size in byte of I-cache
 */
//-----------------------------------------------------------------------------
UINT32 HalGetICacheSize(void)
{
    UINT32 r;
    UINT32 u4SizeField, u4Size = 0;

    __asm
    {
        MRC     p15, 0, r, c0, c0, 1
    }

    u4SizeField = (r >> ISIZE_SHIFT) & ISIZE_MASK;

    switch (u4SizeField)
    {
    case 3:
        u4Size = 4;
        break;

    case 4:
        u4Size = 8;
        break;

    case 5:
        u4Size = 16;
        break;

    case 6:
        u4Size = 32;
        break;

    case 7:
        u4Size = 64;
        break;

    case 8:
        u4Size = 128;
        break;

    default:
        ASSERT(0);
        break;
    }

    return u4Size * 1024;
}

//-----------------------------------------------------------------------------
/** HalGetDCacheSize() Get D-cache size
 *  @return      : The size in byte of D-cache
 */
//-----------------------------------------------------------------------------
UINT32 HalGetDCacheSize(void)
{
    UINT32 r;
    UINT32 u4SizeField, u4Size = 0;

    __asm
    {
        MRC     p15, 0, r, c0, c0, 1
    }

    u4SizeField = (r >> DSIZE_SHIFT) & DSIZE_MASK;

    switch (u4SizeField)
    {
    case 3:
        u4Size = 4;
        break;

    case 4:
        u4Size = 8;
        break;

    case 5:
        u4Size = 16;
        break;

    case 6:
        u4Size = 32;
        break;

    case 7:
        u4Size = 64;
        break;

    case 8:
        u4Size = 128;
        break;

    default:
        ASSERT(0);
        break;
    }

    return u4Size * 1024;
}

//-----------------------------------------------------------------------------
/** HalGetICacheLineSize() Get I-cache line size
 *  @return      : The size in byte
 */
//-----------------------------------------------------------------------------
UINT32 HalGetICacheLineSize(void)
{
    return ICACHE_LINE_SIZE;
}

//-----------------------------------------------------------------------------
/** HalGetDCacheLineSize() Get D-cache line size
 *  @return      : The size in byte
 */
//-----------------------------------------------------------------------------
UINT32 HalGetDCacheLineSize(void)
{
    return DCACHE_LINE_SIZE;
}

//-----------------------------------------------------------------------------
/** HalDisableICache() Disable I-cache
 */
//-----------------------------------------------------------------------------
void HalDisableICache(void)
{
    UINT32 r;

    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
        BIC     r, r, #ICACHE_ENABLE
        MCR     p15, 0, r, c1, c0, 0
    }
}

//-----------------------------------------------------------------------------
/** HalEnableDCache() Enable D-cache
 */
//-----------------------------------------------------------------------------
void HalEnableDCache(void)
{
    UINT32 r;

    HalInvalidateDCache();

    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
        ORR     r, r, #DCACHE_ENABLE
        MCR     p15, 0, r, c1, c0, 0
        NOP
        NOP
        NOP
        NOP
        NOP
    }
}

//-----------------------------------------------------------------------------
/** HalDisableDCache() Disable D-cache
 */
//-----------------------------------------------------------------------------
void HalDisableDCache(void)
{
    UINT32 r;

    // D-cache must be cleaned of dirty data before it is disabled
    HalFlushDCache();

    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
        BIC     r, r, #DCACHE_ENABLE
        MCR     p15, 0, r, c1, c0, 0
    }
}

//-----------------------------------------------------------------------------
/** HalEnableCaches() Enable both I-cache and D-cache
 */
//-----------------------------------------------------------------------------
void HalEnableCaches(void)
{
    UINT32 r;

    HalInvalidateICache();
    HalInvalidateDCache();

    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
        ORR     r, r, #ICACHE_ENABLE
        ORR     r, r, #DCACHE_ENABLE
        MCR     p15, 0, r, c1, c0, 0
        NOP
        NOP
        NOP
        NOP
        NOP
    }
}

//-----------------------------------------------------------------------------
/** HalDisableCaches() Disable both I-cache and D-cache
 */
//-----------------------------------------------------------------------------
void HalDisableCaches(void)
{
    UINT32 r;

    // D-cache must be cleaned of dirty data before it is disabled
    HalFlushDCache();

    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
        BIC     r, r, #ICACHE_ENABLE
        BIC     r, r, #DCACHE_ENABLE
        MCR     p15, 0, r, c1, c0, 0
    }
}

//-----------------------------------------------------------------------------
/** HalInvalidateAllCaches() Invalidate I-cache and D-cache
 */
//-----------------------------------------------------------------------------
void HalInvalidateAllCaches(void)
{
    UINT32 r = 0;

    __asm
    {
        MCR     p15, 0, r, c7, c7, 0
    }
}

//-----------------------------------------------------------------------------
/** HalInvalidateICacheSingleLine() Invalidate single line (8 words or 32 \n
 *                                  bytes) of I-cache
 *  @param u4Addr  [in] - The address to be invalidated
 */
//-----------------------------------------------------------------------------
void HalInvalidateICacheSingleLine(UINT32 u4Addr)
{
    u4Addr = MVA(u4Addr);

    __asm
    {
        MCR     p15, 0, u4Addr, c7, c5, 1
    }
}

//-----------------------------------------------------------------------------
/** HalInvalidateDCache() Invalidate entire D-cache
 */
//-----------------------------------------------------------------------------
void HalInvalidateDCache(void)
{
    UINT32 r = 0;

    __asm
    {
        MCR     p15, 0, r, c7, c6, 0
    }
}

//-----------------------------------------------------------------------------
/** HalInvalidateDCacheSingleLine() Invalidate single line of D-cache
 *  @param u4Addr  [in] - The address to be invalidated
 */
//-----------------------------------------------------------------------------
void HalInvalidateDCacheSingleLine(UINT32 u4Addr)
{
    u4Addr = MVA(u4Addr);

    __asm
    {
        MCR     p15, 0, u4Addr, c7, c6, 1
    }
}

//-----------------------------------------------------------------------------
/** HalFlushDCacheSingleLine() Flush (clean) single line of D-cache
 *  @param u4Addr  [in] - The address to be flushed
 */
//-----------------------------------------------------------------------------
void HalFlushDCacheSingleLine(UINT32 u4Addr)
{
    u4Addr = MVA(u4Addr);

    __asm
    {
        MCR     p15, 0, u4Addr, c7, c10, 1
    }
}

//-----------------------------------------------------------------------------
/** HalFlushInvalidateDCacheSingleLine() Flush (clean) and invalidate single \n
 *                  line of D-cache
 *  @param u4Addr  [in] - The address to be flushed
 */
//-----------------------------------------------------------------------------
void HalFlushInvalidateDCacheSingleLine(UINT32 u4Addr)
{
    u4Addr = MVA(u4Addr);

    __asm
    {
        MCR     p15, 0, u4Addr, c7, c14, 1
    }
}

//-----------------------------------------------------------------------------
/** HalFlushWriteBuffer() Flush write buffer
 */
//-----------------------------------------------------------------------------
void HalFlushWriteBuffer(void)
{
    UINT32 r = 0;

    __asm
    {
        MCR     p15, 0, r, c7, c10, 4
    }
}

//lint -save -e* Don't lint the following two embedded assembly functions

//---------------------------------------------------------------------
/** _FlushDCache Flush (clean) entire D-cache
 */
//-----------------------------------------------------------------------------
inline static void _FlushDCache(void)
// Note: __asm function cannot be static
{
    UINT32 r = 0;
    
    do
    {
	    __asm
	    {
		    MRC     p15, 0, r, c7, c10, 3
	    }
	} while ((r & 0x40000000) == 0);
}

//-----------------------------------------------------------------------------
/** _FlushInvalidateDCache() Flush (clean) and invalidate entire D-cache
 */
//-----------------------------------------------------------------------------
inline static void _FlushInvalidateDCache(void)
// Note: __asm function cannot be static
{
    UINT32 r = 0;
    
    do
    {
	    __asm
	    {
		    MRC     p15, 0, r, c7, c14, 3
	    }
	} while ((r & 0x40000000) == 0);
}

//-----------------------------------------------------------------------------
/** HalFlushDCache() Flush (clean) entire D-cache
 */
//-----------------------------------------------------------------------------
void HalFlushDCache(void)
{
    _FlushDCache();
    HalFlushWriteBuffer();
}

//-----------------------------------------------------------------------------
/** HalFlushInvalidateDCache() Flush (clean) and invalidate entire D-cache
 */
//-----------------------------------------------------------------------------
void HalFlushInvalidateDCache(void)
{
    _FlushInvalidateDCache();
    HalFlushWriteBuffer();
}

//lint -restore Re-enable linting

//-----------------------------------------------------------------------------
/** HalWaitForInterrupt() Put the CPU into a low-power sleep state until an \n
 *                          interrupt
 */
//-----------------------------------------------------------------------------
void HalWaitForInterrupt(void)
{
    UINT32 r = 0;

    __asm
    {
        MCR     p15, 0, r, c7, c0, 4
    }
}

//-----------------------------------------------------------------------------
/** HalInstructionMemoryBarrier() IMB that ensures consistency between the \n
 *                                  data and instruction streams
 */
//-----------------------------------------------------------------------------
void HalInstructionMemoryBarrier(void)
{
    volatile UINT32* p;
    UINT32 i;

    // 1. Clean D-cache and drain write buffer
    HalFlushDCache();

    // 2. Synchronize data and instruction streams in level two AHB subsystems
    // by using a nonbuffered store (STR) or a noncached load (LDR)
    p = (volatile UINT32*)0x20000000;        // DRAM-A, noncached address
    i = *p;
    UNUSED(i);

    // 3. Invalidate the I-cache
    HalInvalidateICache();
}

//===========================================================================
// MMU related functions
//===========================================================================

//-----------------------------------------------------------------------------
/** HalIsMMUEnabled() Check if MMU enabled
 *  @return TRUE if MMU enabled, FALSE if MMU disabled
 */
//-----------------------------------------------------------------------------
BOOL HalIsMMUEnabled(void)
{
    UINT32 r;

    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
    }

    return ((r & MMU_ENABLE) ? TRUE : FALSE);
}

//-----------------------------------------------------------------------------
/** HalEnableMMU() Enable MMU
 */
//-----------------------------------------------------------------------------
void HalEnableMMU(void)
{
    UINT32 r;

    // TLB content is preserved while enabling MMU, thus a TLB invalidation
    // is necessary.
    HalInvalidateTLB();

    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
        ORR     r, r, #MMU_ENABLE
        MCR     p15, 0, r, c1, c0, 0
        NOP
        NOP
        NOP
        NOP
        NOP
    }
}


//-----------------------------------------------------------------------------
/** HalDisableMMU() Disable MMU
 */
//-----------------------------------------------------------------------------
void HalDisableMMU(void)
{
    UINT32 r;

    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
        BIC     r, r, #MMU_ENABLE
        MCR     p15, 0, r, c1, c0, 0
    }
}

//-----------------------------------------------------------------------------
/** HalInvalidateTLB() Invalidate entire TLB
 */
//-----------------------------------------------------------------------------
void HalInvalidateTLB(void)
{
    UINT32 r = 0;

    __asm
    {
        MCR     p15, 0, r, c8, c7, 0
    }
}

//-----------------------------------------------------------------------------
/** SetDomainAccess() Set access privilege of the given doman
 *  @param u4Domain    [in] - Domain number (0 - 15)
 *  @pram  da          [in] - Access privilege
 *  @return      : TRUE on success, FALSE otherwise
 */
//-----------------------------------------------------------------------------
static BOOL SetDomainAccess(UINT32 u4Domain, UINT32 u4Da)
{
    //lint --e{550} Symbol not accessed

    UINT32 u4Mask;
    UINT32 u4Access;
    UINT32 r;

    // Check if domain out of range
    if (u4Domain > 15)
    {
        return FALSE;
    }

    u4Mask = 3 << (u4Domain * 2);
    u4Access = u4Da << (u4Domain * 2);

    __asm
    {
        MRC     p15, 0, r, c3, c0, 0
        BIC     r, r, u4Mask
        ORR     r, r, u4Access
        MCR     p15, 0, r, c3, c0, 0
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** SetPageTableBase() Set base address of page table
 *  @param u4Address   [in] - The base address of page table
 */
//-----------------------------------------------------------------------------
static void SetPageTableBase(UINT32 u4Address)
{
    __asm
    {
        MCR     p15, 0, u4Address, c2, c0, 0
    }
}

//===========================================================================


/**********************************************************************

Memory mapping of MT5371

00000000 - 0fffffff     C   B       0 - 256M        256M    DRAM-B
10000000 - 117fffff     C   B       256M - 280M     24M     PBI-B (Flash)
11800000 - 1fffffff     -   -       280M - 512M     232M    -
20000000 - 200fffff     -   -       512M - 513M     1M      IO
20100000 - 27ffffff     -   -       513M - 640M     127M    -
28000000 - 297fffff     -   -       640M - 664M     24M     PBI-A (Flash)
29800000 - 2fffffff     -   -       664M - 768M     104M    -
30000000 - 3fffffff     -   -       768M - 1024M    256M    DRAM-A
40000000 - 4fffffff     -   B       1024M - 1280M   256M    DRAM-C
50000000 - 5fffffff     C   -       1280M - 1536M   256M    DRAM-D
60000000 - ffffffff     -   -       1536M - 4096M   2560M   -

**********************************************************************/

//-----------------------------------------------------------------------------
/** CreatePageTable() Create page table
 *  @param u4Addr[in] - Address of page table, shall be aligned to 16K boundary
 */
//-----------------------------------------------------------------------------
static void CreatePageTable(UINT32 u4Addr)
{
    UINT32 i;
    UINT32 *pu4Table = (UINT32*)u4Addr;
    UINT32 u4DramSize = BSP_GetMemSize();

    // Note
    // 1. Unused entries must be reserved.
    // 2. All accessible regions are set to domain 0

    // DRAM B 0-256M, cachable/bufferable
    for (i = 0; i < 256; i++)
    {
        if ((i << 20) < u4DramSize)
        {
            pu4Table[i] = L1Entry(SECTION, i << 20, 0, C_BIT | B_BIT, MANAGER);
        }
        else
        {
            pu4Table[i] = L1Entry(SECTION, i << 20, 0, 0, NO_ACCESS);
        }
    }

    // PBI B 256-280M, cachable/bufferable
    for (i = 256; i < 280; i++)
    {
        pu4Table[i] = L1Entry(SECTION, i << 20, 0, C_BIT | B_BIT, MANAGER);
    }

    // Unused region
    for (i = 280; i < 512; i++)
    {
        pu4Table[i] = L1Entry(SECTION, i << 20, 0, 0, NO_ACCESS);
    }

    // IO 512-528M, non-cachable/non-bufferable, 0x200xxxxx => I/O register address.
    for (i = 512; i < 528; i++)
    {
        pu4Table[i] = L1Entry(SECTION, i << 20, 0, 0, MANAGER);
    }

    // Unused region
    for (i = 528; i < 640; i++)
    {
        pu4Table[i] = L1Entry(SECTION, i << 20, 0, 0, NO_ACCESS);
    }

    // PBI A 640-664M, non-cachable/non-bufferable
    for (i = 640; i < 664; i++)
    {
        pu4Table[i] = L1Entry(SECTION, i << 20, 0, 0, MANAGER);
    }

    // Unused region
    for (i = 664; i < 768; i++)
    {
        pu4Table[i] = L1Entry(SECTION, i << 20, 0, 0, NO_ACCESS);
    }

    // DRAM A 768-1024M, non-cachable/non-bufferable
    for (i = 768; i < 1024; i++)
    {
        if (((i-768) << 20) < u4DramSize)
        {
            pu4Table[i] = L1Entry(SECTION, i << 20, 0, 0, MANAGER);
        }
        else
        {
            pu4Table[i] = L1Entry(SECTION, i << 20, 0, 0, NO_ACCESS);
        }
    }

    // DRAM C 1024-1280M, non-cachable/bufferable
    for (i = 1024; i < 1280; i++)
    {
        pu4Table[i] = L1Entry(SECTION, i << 20, 0, B_BIT, NO_ACCESS);
    }

    // DRAM D 1280-1536M, cachable/non-bufferable
    for (i = 1280; i < 1536; i++)
    {
        pu4Table[i] = L1Entry(SECTION, i << 20, 0, C_BIT, NO_ACCESS);
    }

    // TCM 0x60000000-0x60100000, access enable.
    pu4Table[1536] = L1Entry(SECTION, i << 20, 0, 0, MANAGER);

    // Unused region
    for (i = 1537; i < 4096; i++)
    {
        pu4Table[i] = L1Entry(SECTION, i << 20, 0, 0, NO_ACCESS);
    }
}

//-----------------------------------------------------------------------------
/** HalInitMMU() Initialize MMU
 *  @param u4Addr[in] - Address of page table, shall be aligned to 16K boundary
 *  @return      : TRUE on success, FALSE otherwise
 */
//-----------------------------------------------------------------------------
BOOL HalInitMMU(UINT32 u4Addr)
{
    // Check 16K Boundary
    if(u4Addr & 0x3fff)
    {
        return FALSE;
    }

    // Flush and invalidate caches and TLB
    HalDisableMMU();
    HalInvalidateAllCaches();

    // Setup page table
    CreatePageTable(u4Addr);
    SetPageTableBase(u4Addr);

    // Enable domain 0 only, set it with access permission enable.
    VERIFY(SetDomainAccess(0, CLIENT));

    // Enable MMU
    HalEnableMMU();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** BYTESWAP32() Swap bytes in a word (32-bit)
 *  @param u4Value[in] - The word to be swapped
 *  @return      : The swapped word
 */
//-----------------------------------------------------------------------------
UINT32 BYTESWAP32(UINT32 u4Value)
{
    //lint --e{*} Don't lint this function
    UINT32 r1;

    __asm
    {
                                                // r0 = a  , b  , c  , d
        EOR     r1, u4Value, u4Value, ROR #16   // r1 = a^c, b^d, c^a, d^b
        BIC     r1, r1, #0xff0000               // r1 = a^c, 0  , c^a, d^b
        MOV     u4Value, u4Value, ROR #8        // r0 = d  , a  , b  , c
        EOR     u4Value, u4Value, r1, LSR #8    // r0 = d  , c  , b  , a
    }

    return u4Value;
}

#endif /* CC_5391_PRELOADER */

