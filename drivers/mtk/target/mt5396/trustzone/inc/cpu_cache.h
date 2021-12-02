/*
 * Copyright:
 * ---------------------------------------------------------------------------
 * This confidential and proprietary software may be used only as authorised 
 * by a licensing agreement from ARM Limited.
 *      (C) COPYRIGHT 2008-2009 ARM Limited, ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised copies and 
 * copies may only be made to the extent permitted by a licensing agreement
 * from ARM Limited.
 * ---------------------------------------------------------------------------
 * $LastChangedRevision: 2839 $
 * ---------------------------------------------------------------------------
 */

#ifndef CPU_CACHE_H
#define CPU_CACHE_H

/* ============================================================================
    Includes
============================================================================ */
#include "tzst.h"

/* ============================================================================
    Function Prototypes
============================================================================ */

/* ----------------------------------------------------------------------------
 * Description:
 *     This function enables the I cache on the platform, including performing
 *     any required invalidations.
 *
 * Arguments:
 *     None.
 *
 * Return:
 *     None.
 * ------------------------------------------------------------------------- */
void CPU_EnableICache(void);

/* ----------------------------------------------------------------------------
 * Description:
 *     This function disables the I cache on the platform, including performing
 *     any required invalidations.
 *
 * Arguments:
 *     None.
 *
 * Return:
 *     None.
 * ------------------------------------------------------------------------- */
void CPU_DisableICache(void);

/* ----------------------------------------------------------------------------
 * Description:
 *     This function enables the D cache on the platform, including performing
 *     any required invalidations.
 *
 * Arguments:
 *     None.
 *
 * Return:
 *     None.
 * ------------------------------------------------------------------------- */
void CPU_EnableDCache(void);

/* ----------------------------------------------------------------------------
 * Description:
 *     This function disables the D cache on the platform, including performing
 *     any required invalidations.
 *
 * Arguments:
 *     None.
 *
 * Return:
 *     None.
 * ------------------------------------------------------------------------- */
void CPU_DisableDCache(void);

/* ----------------------------------------------------------------------------
 * Description:
 *     This function enables CPU program flow options such as branch 
 *     prediction, and bus speculation.
 *
 * Arguments:
 *     None.
 *
 * Return:
 *     None.
 * ------------------------------------------------------------------------- */
void CPU_EnableProgramFlow(void);

/* ----------------------------------------------------------------------------
 * Description:
 *     This function disables CPU program flow options such as branch
 *     prediction, and bus speculation.
 *
 * Arguments:
 *     None.
 *
 * Return:
 *     None.
 * ------------------------------------------------------------------------- */
void CPU_DisableProgramFlow(void);

/* ----------------------------------------------------------------------------
 * Description:
 *     This function invalidates the entire TLB.
 *
 * Arguments:
 *     None.
 *
 * Return:
 *     None.
 * ------------------------------------------------------------------------- */
void CPU_InvalidateTLB(void);

#endif  /* CPU_CACHE_H */
