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
 * $RCSfile: dmx_diag.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_diag.c
 *  Demux diagnostic routines
 */


// This module exists only in SLT mode
#ifdef __MODEL_slt__


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx.h"
#include "dmx_debug.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_printf.h"
#include "x_os.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct
{
    UINT32 u4Addr;
    UINT32 u4Size;
} MEM_BLOCK_T;

typedef enum
{
    DMX_I_MEM,
    DMX_D_MEM
} DMX_MEM_TYPE_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define CHECK_MEM(addr, seed, temp) \
    if ((temp) != (seed)) \
    { \
        Printf("\n    Memory failed at 0x%08x, write: 0x%08x, read: 0x%08x\n", \
            (addr), (seed), (temp)); \
        return FALSE; \
    }


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static MEM_BLOCK_T _arSramBlocks[] =
{
    { 0x1000,   0x1d00},    // 0x20018000, 8K, instruction and data memory
    { 0x5c00,   0x80},      // 0x2001cc00, 128B, PID index table
//    { 0x6c00,   0x40},      // 0x2001dc00, 64B, PES filter control words
//    { 0x6e00,   0x100},     // 0x2001de00, 256B, PES filter bank memory
/*
    { 0x4800,     0x200}        // 0.5K
*/
};


static MEM_BLOCK_T _arSramBlocks2[] =
{
    { 0x5000,   0x200},     // 0x2001c000, 512B, even descrambling key
    { 0x6000,   0x200},     // 0x2001d000, 512B, odd descrambling key
/*
    { 0x5000,   0x400},     // 0x2001c000, 1K, even descrambling key
    { 0x5400,   0x400},     // 0x2001c400, 1K, even descrambling key
    { 0x5800,   0x400},     // 0x2001c800, 1K, even descrambling key
    { 0x6000,   0x400},     // 0x2001d000, 1K, odd descrambling key
    { 0x6400,   0x400},     // 0x2001d400, 1K, odd descrambling key
    { 0x6800,   0x400},     // 0x2001d800, 1K, odd descrambling key
*/
};


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#if 0

//-----------------------------------------------------------------------------
/** _DmxMemTestByte
 *  Memory test
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxMemTestByte(UINT32 u4Addr, INT32 i4Len, UINT8 u1Seed)
{
	INT32 i;
	UINT8 u1SeedS, u1Temp;
	volatile UINT8* pu1Addr;

    pu1Addr = (volatile UINT8*)u4Addr;
    u1SeedS = 0xff - u1Seed;

    //---------------------------------------------------------
    // Ascending address
    //---------------------------------------------------------

	// Write seed
	for (i = 0x0; i < i4Len; i++)
	{
	    pu1Addr[i] = u1Seed;
	}

	// Read seed, and write seed's
	for (i = 0x0; i < i4Len; i++)
	{
		u1Temp = pu1Addr[i];
		CHECK_MEM((UINT32)(pu1Addr + i), u1Seed, u1Temp);
        pu1Addr[i] = u1SeedS;
	}

	// Read seed_s, and write seed
	for (i = 0; i < i4Len; i++)
	{
	    u1Temp = pu1Addr[i];
	    CHECK_MEM((UINT32)(pu1Addr + i), u1SeedS, u1Temp);
	    pu1Addr[i] = u1Seed;
	}

	// Read seed
	for (i = 0; i < i4Len; i++)
	{
	    u1Temp = pu1Addr[i];
	    CHECK_MEM((UINT32)(pu1Addr + i), u1Seed, u1Temp);
	}

    //---------------------------------------------------------
    // Descending address
    //---------------------------------------------------------

	// Write seed
	for (i = i4Len - 1; i >= 0; i--)
	{
	    pu1Addr[i] = u1Seed;
	}

	// Read seed, and write seed's
	for (i = i4Len - 1; i >= 0; i--)
	{
	    u1Temp = pu1Addr[i];
	    CHECK_MEM((UINT32)(pu1Addr + i), u1Seed, u1Temp);
	    pu1Addr[i] = u1SeedS;
	}

	// Read seed_s, and write seed
	for (i = i4Len - 1; i >= 0; i--)
	{
	    u1Temp = pu1Addr[i];
	    CHECK_MEM((UINT32)(pu1Addr + i), u1SeedS, u1Temp);
	    pu1Addr[i] = u1Seed;
	}

	// Read seed
	for (i = i4Len - 1; i >= 0; i--)
	{
	    u1Temp = pu1Addr[i];
	    CHECK_MEM((UINT32)(pu1Addr + i), u1Seed, u1Temp);
	}

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxMemTestHalfWord
 *  Memory test
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxMemTestHalfWord(UINT32 u4Addr, INT32 i4Len, UINT16 u2Seed)
{
	INT32 i;
	UINT16 u2SeedS, u2Temp;
	volatile UINT16* pu2Addr;

    pu2Addr = (volatile UINT16*)u4Addr;
    u2SeedS = 0xffff - u2Seed;
    i4Len /= 2;

    //---------------------------------------------------------
    // Ascending address
    //---------------------------------------------------------

	// Write seed
	for (i = 0x0; i < i4Len; i++)
	{
	    pu2Addr[i] = u2Seed;
	}

	// Read seed, and write seed's
	for (i = 0x0; i < i4Len; i++)
	{
		u2Temp = pu2Addr[i];
		CHECK_MEM((UINT32)(pu2Addr + i), u2Seed, u2Temp);
        pu2Addr[i] = u2SeedS;
	}

	// Read seed_s, and write seed
	for (i = 0; i < i4Len; i++)
	{
	    u2Temp = pu2Addr[i];
	    CHECK_MEM((UINT32)(pu2Addr + i), u2SeedS, u2Temp);
	    pu2Addr[i] = u2Seed;
	}

	// Read seed
	for (i = 0; i < i4Len; i++)
	{
	    u2Temp = pu2Addr[i];
	    CHECK_MEM((UINT32)(pu2Addr + i), u2Seed, u2Temp);
	}

    //---------------------------------------------------------
    // Descending address
    //---------------------------------------------------------

	// Write seed
	for (i = i4Len - 1; i >= 0; i--)
	{
	    pu2Addr[i] = u2Seed;
	}

	// Read seed, and write seed's
	for (i = i4Len - 1; i >= 0; i--)
	{
	    u2Temp = pu2Addr[i];
	    CHECK_MEM((UINT32)(pu2Addr + i), u2Seed, u2Temp);
	    pu2Addr[i] = u2SeedS;
	}

	// Read seed_s, and write seed
	for (i = i4Len - 1; i >= 0; i--)
	{
	    u2Temp = pu2Addr[i];
	    CHECK_MEM((UINT32)(pu2Addr + i), u2SeedS, u2Temp);
	    pu2Addr[i] = u2Seed;
	}

	// Read seed
	for (i = i4Len - 1; i >= 0; i--)
	{
	    u2Temp = pu2Addr[i];
	    CHECK_MEM((UINT32)(pu2Addr + i), u2Seed, u2Temp);
	}

    return TRUE;
}

#endif  // 0

//-----------------------------------------------------------------------------
/** _DmxMemTestWord
 *  Memory test
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxMemTestWord(UINT32 u4Addr, INT32 i4Len, UINT32 u4Seed)
{
	INT32 i;
	UINT32 u4SeedS, u4Temp;
	volatile UINT32* pu4Addr;

    pu4Addr = (volatile UINT32*)u4Addr;
    u4SeedS = 0xffffffff - u4Seed;
    i4Len /= 4;

    //---------------------------------------------------------
    // Ascending address
    //---------------------------------------------------------

	// Write seed
	for (i = 0x0; i < i4Len; i++)
	{
	    pu4Addr[i] = u4Seed;
	}

	// Read seed, and write seed's
	for (i = 0x0; i < i4Len; i++)
	{
		u4Temp = pu4Addr[i];
		CHECK_MEM((UINT32)(pu4Addr + i), u4Seed, u4Temp);
        pu4Addr[i] = u4SeedS;
	}

	// Read seed_s, and write seed
	for (i = 0; i < i4Len; i++)
	{
	    u4Temp = pu4Addr[i];
	    CHECK_MEM((UINT32)(pu4Addr + i), u4SeedS, u4Temp);
	    pu4Addr[i] = u4Seed;
	}

	// Read seed
	for (i = 0; i < i4Len; i++)
	{
	    u4Temp = pu4Addr[i];
	    CHECK_MEM((UINT32)(pu4Addr + i), u4Seed, u4Temp);
	}

    //---------------------------------------------------------
    // Descending address
    //---------------------------------------------------------

	// Write seed
	for (i = i4Len - 1; i >= 0; i--)
	{
	    pu4Addr[i] = u4Seed;
	}

	// Read seed, and write seed's
	for (i = i4Len - 1; i >= 0; i--)
	{
	    u4Temp = pu4Addr[i];
	    CHECK_MEM((UINT32)(pu4Addr + i), u4Seed, u4Temp);
	    pu4Addr[i] = u4SeedS;
	}

	// Read seed_s, and write seed
	for (i = i4Len - 1; i >= 0; i--)
	{
	    u4Temp = pu4Addr[i];
	    CHECK_MEM((UINT32)(pu4Addr + i), u4SeedS, u4Temp);
	    pu4Addr[i] = u4Seed;
	}

	// Read seed
	for (i = i4Len - 1; i >= 0; i--)
	{
	    u4Temp = pu4Addr[i];
	    CHECK_MEM((UINT32)(pu4Addr + i), u4Seed, u4Temp);
	}

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxTestSramSeed
 *  Test SRAM with seed
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxTestSramSeed(UINT32 u4Addr, INT32 i4Len, UINT32 u4Seed)
{
/*
    if (!_DmxMemTestByte(u4Addr, i4Len, (UINT8)u4Seed))
    {
        return FALSE;
    }

    if (!_DmxMemTestHalfWord(u4Addr, i4Len, (UINT16)u4Seed))
    {
        return FALSE;
    }
*/
    if (!_DmxMemTestWord(u4Addr, i4Len, u4Seed))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxTestSramSeed2
 *  Test SRAM with seed
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxTestSramSeed2(UINT32 u4Addr, INT32 i4Len, UINT32 u4Seed)
{
    UINT32 u4SeedS, u4Temp;
    INT32 i;
    volatile UINT32* pu4Addr;

    i4Len /= 4;
    pu4Addr = (UINT32*)u4Addr;

    // Write seed
    for (i = 0; i < i4Len; i++)
    {
        pu4Addr[i] = u4Seed + i;
    }

    // Read seed
    for (i = 0; i < i4Len; i++)
    {
	    u4Temp = pu4Addr[i];
	    CHECK_MEM((UINT32)(pu4Addr + i), u4Seed + i, u4Temp);
    }

    u4SeedS = 0xffffffff - u4Seed;

    // Write seed_s
    for (i = 0; i < i4Len; i++)
    {
        pu4Addr[i] = u4SeedS + i;
    }

    // Read seed_s
    for (i = 0; i < i4Len; i++)
    {
	    u4Temp = pu4Addr[i];
	    CHECK_MEM((UINT32)(pu4Addr + i), u4SeedS + i, u4Temp);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
static UINT32 _DmxIReadWord(DMX_MEM_TYPE_T eType, UINT32 u4Offset)
{
    UINT32 u4Ctrl;

    u4Ctrl = ((u4Offset & 0xFFFF) << 16) |          //  Addr
             (1 << 8);                                // Read

    if(eType == DMX_I_MEM)
    {
        u4Ctrl |= 1;
    }
    else if(eType == DMX_D_MEM)
    {
        u4Ctrl |= 2;
    }

    DMXCMD_WRITE32(DMX_REG_MEM_CMD, u4Ctrl);

    while(1)
    {
        if((DMXCMD_READ32(DMX_REG_MEM_CMD) & 0xFFFF) == 0)
        {
            break;
        }
    }

    return DMXCMD_READ32(DMX_REG_MEM_DATA);
}

//-----------------------------------------------------------------------------
static void _DmxIWriteWord(DMX_MEM_TYPE_T eType, UINT32 u4Offset, UINT32 u4Val)
{
    UINT32 u4Ctrl;

    u4Ctrl = ((u4Offset & 0xFFFF) << 16) |          //  Addr
             (2 << 8);                                // Write

    if(eType == DMX_I_MEM)
    {
        u4Ctrl |= 1;
    }
    else if(eType == DMX_D_MEM)
    {
        u4Ctrl |= 2;
    }

    DMXCMD_WRITE32(DMX_REG_MEM_DATA, u4Val);

    DMXCMD_WRITE32(DMX_REG_MEM_CMD, u4Ctrl);

    while(1)
    {
        if((DMXCMD_READ32(DMX_REG_MEM_CMD) & 0xFFFF) == 0)
        {
            break;
        }
    }
}

//-----------------------------------------------------------------------------
/** _DmxTestSram_IndirectWord
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
static BOOL _DmxTestSram_IndirectWord(DMX_MEM_TYPE_T eType, 
                                      UINT32 u4BaseAddr, UINT32 u4ByteCount)
{
    UINT32 i;

    // Special pattern - 0x5A
    for(i=0; i<(u4ByteCount/4); i++)
    {
        if((i%2) == 0)
        {
            _DmxIWriteWord(eType, u4BaseAddr + i, 0x5A5A5A5A);
        }
        else
        {
            _DmxIWriteWord(eType, u4BaseAddr + i, 0xA5A5A5A5);
        }
    }

    for(i=0; i<(u4ByteCount/4); i++)
    {
        if((i%2) == 0)
        {            
            if(_DmxIReadWord(eType, u4BaseAddr + i) != 0x5A5A5A5A)
            {
                return FALSE;
            }
        }
        else
        {
            if(_DmxIReadWord(eType, u4BaseAddr + i) != 0xA5A5A5A5)
            {
                return FALSE;
            }
        }
    }

    // Special pattern - 0xA5
    for(i=0; i<(u4ByteCount/4); i++)
    {
        if((i%2) == 0)
        {
            _DmxIWriteWord(eType, u4BaseAddr + i, 0xA5A5A5A5);
        }
        else
        {
            _DmxIWriteWord(eType, u4BaseAddr + i, 0x5A5A5A5A);
        }
    }

    for(i=0; i<(u4ByteCount/4); i++)
    {
        if((i%2) == 0)
        {
            if(_DmxIReadWord(eType, u4BaseAddr + i) != 0xA5A5A5A5)
            {
                return FALSE;
            }
        }
        else
        {
            if(_DmxIReadWord(eType, u4BaseAddr + i) != 0x5A5A5A5A)
            {
                return FALSE;
            }
        }
    }

    // Increase pattern
    for(i=0; i<(u4ByteCount/4); i++)
    {
        _DmxIWriteWord(eType, u4BaseAddr + i, u4BaseAddr + (i * 4));
    }

    for(i=0; i<(u4ByteCount/4); i++)
    {
        if(_DmxIReadWord(eType, u4BaseAddr + i) != (u4BaseAddr + (i * 4)))
        {
            return FALSE;
        }
    }
        
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxTestSram
 *  Test SRAM
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxTestSram(void)
{
    UINT32 u4Blocks, u4Seed, i;
    BOOL fgRet = TRUE;

    u4Blocks = sizeof (_arSramBlocks) / sizeof (MEM_BLOCK_T);

    for (i = 0; i < u4Blocks; i++)
    {
        UINT32 u4Addr;
        INT32 i4Len;

        u4Addr = _arSramBlocks[i].u4Addr + DEMUX0_BASE;
        i4Len = (INT32)_arSramBlocks[i].u4Size;

        for (u4Seed = 0; u4Seed < 256; u4Seed++)
        {
            if (!_DmxTestSramSeed(u4Addr, i4Len, u4Seed))
            {
                fgRet = FALSE;
                break;
            }
        }
    }

    u4Blocks = sizeof (_arSramBlocks2) / sizeof (MEM_BLOCK_T);

    for (i = 0; i < u4Blocks; i++)
    {
        UINT32 u4Addr;
        INT32 i4Len;

        u4Addr = _arSramBlocks2[i].u4Addr + DEMUX0_BASE;
        i4Len = (INT32)_arSramBlocks2[i].u4Size;

        for (u4Seed = 0; u4Seed < 256; u4Seed++)
        {
            if (!_DmxTestSramSeed2(u4Addr, i4Len, u4Seed))
            {
                fgRet = FALSE;
                break;
            }
        }
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DmxTestReset
 *  Test FTI reset
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxTestReset(void)
{
    BOOL fgRet = FALSE;

    // Reset FTI
    DMXCMD_WRITE32(DMX_REG_CONTROL, 0x80000000);

    // Wait for a while
    x_thread_delay(1);

    // Check if FTI is halted
    if (0x1 == ((DMXCMD_READ32(DMX_REG_CONTROL) >> 28) & 0x3))
    {
        fgRet = TRUE;
    }

#ifdef CC_MT5360
    // Debug, enable section filter hardware clock
    DMXCMD_REG32(DMX_REG_CONFIG2) |= (1 << 29);
#endif

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DmxTestRegisters
 *  Test register access
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxTestRegisters(void)
{
    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_Diag
 *  Demux diagnostic function
 *
 *  @retval 0               Succeed
 *  @retval otherwise       Fail. The return value is error code
 */
//-----------------------------------------------------------------------------
INT32 _DMX_Diag(void)
{
    // Reset test
    if (!_DmxTestReset())
    {
        return -1;
    }

    // Register access test
    if (!_DmxTestRegisters())
    {
        return -2;
    }

    // SRAM memory test
    if (!_DmxTestSram())
    {
        return -3;
    }

    if(!_DmxTestSram_IndirectWord(DMX_I_MEM, 0, 1760 * 4))
    //if(!_DmxTestSram_IndirectWord(DMX_I_MEM, 0, 2052 * 4))
    {
        return -13;
    }

    if(!_DmxTestSram_IndirectWord(DMX_D_MEM, 0, 952 * 4))
    {
        return -14;
    }   

    // uP latch-up test
    if (!_DMX_TestFTuPLatch())
    {
        return -4;
    }

    return 0;
}


#endif  // __MODEL_slt__


