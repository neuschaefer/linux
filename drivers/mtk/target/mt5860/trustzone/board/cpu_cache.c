/*
 * $Copyright:
 * ----------------------------------------------------------------------------
 *  This confidential and proprietary software may be used only as authorised
 *  by a licensing agreement from ARM Limited.
 *          (C) COPYRIGHT 2007-2009 ARM Limited , ALL RIGHTS RESERVED
 *  The entire notice above must be reproduced on all authorised copies and
 *  copies may only be made to the extent permitted by a licensing agreement
 *  from ARM Limited.
 * ----------------------------------------------------------------------------
 *    Revision: $LastChangedRevision: 2839 $
 * ----------------------------------------------------------------------------
 * $
 */

/* ============================================================================
    Includes
============================================================================ */
#include "tzst.h"
#include "cpu_cache.h"
#include "x_hal_5381.h"
#include "x_hal_arm.h"


/* ============================================================================
    See header for more information
============================================================================ */
void CPU_EnableICache(void)
{
    HalEnableICache();
}

/* ============================================================================
    See header for more information
============================================================================ */
void CPU_DisableICache(void)
{
    HalDisableICache();
}

/* ============================================================================
    See header for more information
============================================================================ */
void CPU_EnableDCache(void)
{
    HalEnableDCache();
}

/* ============================================================================
    See header for more information
============================================================================ */
void CPU_DisableDCache(void)
{
    HalDisableDCache();

}

/* ============================================================================
    See header for more information
============================================================================ */
void CPU_EnableProgramFlow(void)
{
    UINT32 uwT0;

    __asm
    {
        /* Invalidate BTAC. */
        MOV     uwT0, #0                    /* Zero T0. */
        MCR     p15, 0, uwT0, c7, c5, 6     /* Invalidate BTAC. */

        /* Enable branch-prediction options. */
        MRC     p15, 0, uwT0, c1, c0, 1     /* Load ACTLR. */
        ORR     uwT0, uwT0, #1 << 2         /* Set SB. */
        ORR     uwT0, uwT0, #1 << 1         /* Set DB. */
        ORR     uwT0, uwT0, #1 << 0         /* Set RS. */
        MCR     p15, 0, uwT0, c1, c0, 1     /* Store ACTLR. */

        /* Globally enable branch-prediction. */
        MRC     p15, 0, uwT0, c1, c0, 0     /* Load SCTLR. */
        ORR     uwT0, uwT0, #1 << 11        /* Set Z. */
        ORR     uwT0, uwT0, #1 << 22        /* Set U. */
        MCR     p15, 0, uwT0, c1, c0, 0     /* Store SCTLR. */
    }
}

/* ============================================================================
    See header for more information
============================================================================ */
void CPU_DisableProgramFlow(void)
{
    UINT32 uwT0;

    __asm
    {
        /* Globally disable branch-prediction. */
        MRC     p15, 0, uwT0, c1, c0, 0     /* Load SCTLR. */
        BIC     uwT0, uwT0, #1 << 11        /* Clear Z. */
        BIC     uwT0, uwT0, #1 << 22        /* Clear U. */
        MCR     p15, 0, uwT0, c1, c0, 0     /* Store SCTLR. */

        /* Invalidate BTAC. */
        MOV     uwT0, #0                    /* Zero T0. */
        MCR     p15, 0, uwT0, c7, c5, 6     /* Invalidate BTAC. */
    }
}

/* ============================================================================
    See header for more information
============================================================================ */
void CPU_InvalidateTLB(void)
{
    HalInvalidateTLB();
}

//---------------------------------------------------------------------
/** _FlushDCache Flush (clean) entire D-cache
 */
//-----------------------------------------------------------------------------


UINT32 SvcEx_InvalidateICache(
    TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    UINT32 r = 0;

    Printf("SvcEx_InvalidateICache\n");

    HalInvalidateICache();

    return TZ_SUCCESS;
}

UINT32 SvcEx_InvalidateDCache(
    TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    UINT32 reg1, reg2, reg3;

    Printf("SvcEx_InvalidateDCache\n");

    HalInvalidateDCache();

    return TZ_SUCCESS;
}

UINT32 SvcEx_EnableL2(
    TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    Printf("SvcEx_EnableL2\n");

    HalEnableCaches();

    return TZ_SUCCESS;
}

UINT32 SvcEx_FlushL2(
    TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    UINT32 reg1, reg2, reg3;

    Printf("SvcEx_FlushL2\n");

    HalFlushInvalidateDCache();

    return TZ_SUCCESS;
}

