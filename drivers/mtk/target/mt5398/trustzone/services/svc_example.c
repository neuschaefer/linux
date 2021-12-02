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
 * $LastChangedRevision: 2907 $
 * ----------------------------------------------------------------------------
 */

/* ============================================================================
    Includes
============================================================================ */
#include "tzst.h"
#include "svc_registry.h"

#include "x_hal_5381.h"
/* ============================================================================
    Internal Functions
============================================================================ */

/* ============================================================================
    Exported Entry Points
============================================================================ */
UINT32 SvcEx_TZInit(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{    
    /*
    Printf("[Trustzone]: ");
    Printf("0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", prSMCArg->u4SMCID, prSMCArg->u4R1, 
        prSMCArg->u4R2, prSMCArg->u4R3, prSMCArg->u4R4, prSMCArg->u4R5,
        prSMCArg->u4R6, prSMCArg->u4R7);
    
    // Extend dmx data memroy for non-seucre access
    */
    IO_WRITE32(DEMUX0_BASE, 0x14, 0x3FF0000);

    return TZ_SUCCESS;
}

UINT32 SvcEx_MemoryAccessExample(
    TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    UINT32 u4DstAddr, i;
    
    u4DstAddr = prSMCArg->u4R1;

    for(i = 0; i < 32; i++) 
    {
        if((i) % 4 == 0)
            Printf("\n");
        
        Printf("0x%x ", *(UINT32 *)(u4DstAddr + i * 4));
    }
    Printf("\n");

    for(i = 0; i < 32; i++) 
    {
        *(UINT32 *)(u4DstAddr + i * 4) = i;
    }

    for(i = 0; i < 32; i++) 
    {
        if((i) % 4 == 0)
            Printf("\n");
        
        Printf("0x%x ", *(UINT32 *)(u4DstAddr + i * 4));
    }
    Printf("\n");

    HalFlushDCache();    
    
    return TZ_SUCCESS;
}

