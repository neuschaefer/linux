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
 * $LastChangedRevision: 2843 $
 * ----------------------------------------------------------------------------
 */

#ifndef CPU_MMU_H
#define CPU_MMU_H

/* ============================================================================
    Includes
============================================================================ */
#include "tzst.h"

/* ============================================================================
    Page Table Macros
============================================================================ */

/* L1 entries */

#define L1_FLT  0x0         /* Fault section. */
#define L1_SCN  0x1         /* 1MB section. */
#define L1_CRS  0x2         /* L2 coarse page table. */

/* Properties for 1MB section (L1_SCN) entries. */
#define L1_SCN_B(x)         (x << 2)
#define L1_SCN_X(x)         (x << 3)
#define L1_SCN_XN(x)        (x << 4)
#define L1_SCN_DOMAIN(x)    (x << 5)
#define L1_SCN_P(x)         (x << 9)
#define L1_SCN_AP(x)        (x << 10)
#define L1_SCN_TEX(x)       (x << 12)
#define L1_SCN_APX(x)       (x << 15)
#define L1_SCN_S(s)         (x << 16)
#define L1_SCN_NG(x)        (x << 17)
#define L1_SCN_NS(x)        (x << 19)
#define L1_SCN_PA(x)        (x & 0xFFF00000)

/* Properties for coarse page table (L1_CRS) entries. */
#define L1_CRS_NS(x)        (x << 3)
#define L1_CRS_DOMAIN(x)    (x << 5)
#define L1_CRS_P(x)         (x << 9)
#define L1_CRS_PA(x)        (x & 0xFFFFFC00)

/* L2 entries. */

#define L2_FLT  0x0         /* Fault page. */
#define L2_SML  0x2         /* 4KB extended small page. */

/* Properties for 4KB pages. */
#define L2_SML_XN(x)        (x << 0)
#define L2_SML_B(x)         (x << 2)
#define L2_SML_C(x)         (x << 3)
#define L2_SML_AP(x)        (x << 4)
#define L2_SML_TEX(x)       (x << 6)
#define L2_SML_APX(x)       (x << 9)
#define L2_SML_S(x)         (x << 10)
#define L2_SML_NG(x)        (x << 11)
#define L2_SML_PA(x)        (x & 0xFFFFF000)

#endif /* CPU_MMU_H */
