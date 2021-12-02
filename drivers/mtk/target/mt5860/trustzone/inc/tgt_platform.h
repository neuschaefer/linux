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
 * $LastChangedRevision: 2820 $
 * ----------------------------------------------------------------------------
 */

#ifndef TGT_PLATFORM_H
#define TGT_PLATFORM_H

/* ============================================================================
    Includes
============================================================================ */
#include "tgt_memory_map.h"

/* ============================================================================
    Normal World Memory Map Configuration
============================================================================ */
//#define NWD_BOOT_ADX_PA             0x30040000
//#define NWD_BOOT_ADX_PA             0x1001000 //small normal
//#define NWD_BOOT_ADX_PA             0x10000   //nucleus
#define NWD_BOOT_ADX_PA             0x8040   //Linux

/* ============================================================================
    Memory Manager Configuration
============================================================================ */
/* PB1176 has caches larger than 16KB, so needs page coloring handled. */
#define MEMMGR_PAGE_COLOURS         4

/* ============================================================================
    TrustZone Protection Controller Boot-time Configuration
============================================================================ */

/* ============================================================================
    Function Prototypes
============================================================================ */

/* ----------------------------------------------------------------------------
 * Description:
 *     This function is called to perform platform specific boot operations 
 *     after the MMU and C library have been initialized.
 *
 * Arguments:
 *     None.
 *
 * Return:
 *     None - Note that this function should never return; INT32 is
 *     specified to be compatible with the C standard.
 * ------------------------------------------------------------------------- */
void Tgt_PostBootInit(void);

#endif   /* TZST_PLATFORM_H */
