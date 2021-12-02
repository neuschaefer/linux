/*
 * Copyright:
 * ----------------------------------------------------------------------------
 * This confidential and proprietary software may be used only as authorised
 * by a licensing agreement from ARM Limited.
 *      (C) COPYRIGHT 2008-2009 ARM Limited, ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised copies and
 * copies may only be made to the extent permitted by a licensing agreement
 * from ARM Limited.
 * ----------------------------------------------------------------------------
 * $LastChangedRevision: 2860 $
 * ----------------------------------------------------------------------------
 */

/* ============================================================================
    Includes
============================================================================ */
#include "x_bim.h"
#include "tzst.h"
#include "tgt_platform.h"
#include "cpu_cache.h"

/* ============================================================================
    Globals - exported andused by the monitor code
============================================================================ */
UINT32 guiNWdBootAdxPA = NWD_BOOT_ADX_PA;

UINT32 u4UbootR0 = 0;
UINT32 u4UbootR1 = 0;
UINT32 u4UbootR2 = 0;
UINT32 u4FirstTimeSwitch = 1;
/* ============================================================================
    See header for more details.
============================================================================ */
void Tgt_PostBootInit(void)
{
   // UINT32 * puiL1Table;

    /* This function needs to remove the platform specific bounce-mapping that
     * was stored in the page table when the MMU was initialized . */
    // TODO!!
    //puiL1Table = (UINT32*)VA_TABLE_BASE;
    //puiL1Table[PA_LOAD_ADDRESS / C_SIZE_1MB] = 0;

    /* Invalidate TLB to ensure that old mappings are removed. */
    CPU_InvalidateTLB();

#ifdef CC_CA9
    // GIC,SCU setting
    L2C_WRITE32(0x2054, 0xfff);
    L2C_WRITE32(0x2100, 0x3);
    // single core need these setting in secure world
    L2C_WRITE32(0x2104, 0xff);
    L2C_WRITE32(0x3080, 0xffffffff);
    L2C_WRITE32(0x3084, 0xffffffff);
    L2C_WRITE32(0x3088, 0xffffffff);
    L2C_WRITE32(0x308c, 0xffffffff);
#endif
}
