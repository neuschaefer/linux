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
 * $LastChangedRevision: 2839 $
 * ----------------------------------------------------------------------------
 */

/* ============================================================================
    Includes
============================================================================ */
#include "tzst.h"
#include "cpu_cache.h"
#include "tgt_platform.h"

#include "x_typedef.h"
#include "x_bim.h"
/* ============================================================================
    Definitions
============================================================================ */
extern void __rt_entry(void);

/* ----------------------------------------------------------------------------
 * Description:
 *     Sets the DACR to allow access to domain 0, as a client, only.
 *
 * Arguments:
 *     None.
 *
 * Return:
 *     None.
 * ------------------------------------------------------------------------- */
void Boot_SetDACR( void )
{
    UINT32 uiTmp = 1;
    __asm
    {
        MCR p15, 0, uiTmp, c3, c0, 0
    }
}




/* ----------------------------------------------------------------------------
 * Description:
 *     This function initializes the TrustZone Protection Controller.
 *
 * Arguments:
 *     None.
 *
 * Return:
 *     None.
 * ------------------------------------------------------------------------- */
static void Boot_TZPCInit (void)
{
    /* Perform Development chip TZPC configurations. */
}


void BIM_Trustzone_DRAM_Protection(UINT32 base, UINT32 size)
{
    BIM_WRITE32(0x10, base);   //DRAM_SC_BASE
    BIM_WRITE32(0x8, BIM_READ32(0x8) | (3 << 5) | (1 << 4)); //size 1MB and enable
}

/* ----------------------------------------------------------------------------
 * Description:
 *     This function builds the initial page tables, performs platform setup,
 *     and then triggers the address bounce to switch to the VA != PA memory
 *     mapping.
 *
 * Arguments:
 *     None.
 *
 * Return:
 *     None.
 * ------------------------------------------------------------------------- */
void Boot_PAtoVA(void)
{
    /* Configure the TrustZone Protection Controller. */
    Boot_TZPCInit();

    HalInitMMU(PA_TABLE_BASE);

    //protect trustzone memory
    //BIM_Trustzone_DRAM_Protection(PA_LOAD_ADDRESS, 0x100000);

    /* Enable branch prediction. */
    //CPU_EnableProgramFlow();

    /* Perform the bounce. */ //TODO
    //Boot_FVABounce(VA_STACK_DATA_END, __rt_entry, PA_CODE_BASE, VA_CODE_BASE);
    Boot_FVABounce(VA_STACK_DATA_END, __rt_entry);
}
