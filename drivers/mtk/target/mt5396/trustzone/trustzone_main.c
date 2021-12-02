/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * Description:
 *         The main routine of boot preloader
 *
 *---------------------------------------------------------------------------*/


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
 * $LastChangedRevision: 2769 $
 * ----------------------------------------------------------------------------
 */

/* ============================================================================
    Includes
============================================================================ */
#include "tzst.h"

#include "cpu_cache.h"
#include "tgt_platform.h"

/* ============================================================================
    Declarations
============================================================================ */
extern void sttzsMonitorInit(void);
extern void TZST_EnterNWd(void);
/* ============================================================================
    Function Implementation
============================================================================ */

/* ----------------------------------------------------------------------------
 * Description:
 *     The C entry point for the Secure World which is called after the boot
 *     code has initialized the MMU and the C library stack and heap.
 *
 * Arguments:
 *     None.
 *
 * Return:
 *     None - Note that this function should never return; INT32 is
 *     specified to be compatible with the C standard.
 * ------------------------------------------------------------------------- */

int main()
{
    //Printf("[%s] %s() line%d\n", __FILE__, __FUNCTION__, __LINE__);
    /* Perform any platform-specific initialization that may be required. */
    Tgt_PostBootInit();
    //x_mem_init(PA_MEMORY_POOL_BASE, PA_MEMORY_POOL_END);

    /* Initialize the Monitor. */
    sttzsMonitorInit();

    /* Perform service-specific initialization (where applicable). */
    SvcMgr_InitServices();

    /* Switch to Normal World to allow the NWd OS to boot. This call will not
     * return. */
    TZST_EnterNWd();

    /* This point should never be reached. */
    while(1) { };
}

void __rt_raise(INT32 signal, INT32 type)
{
}

void __rt_SIGFPE()
{
}

