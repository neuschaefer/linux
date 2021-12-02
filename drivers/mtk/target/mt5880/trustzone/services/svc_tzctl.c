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
#include <stdlib.h>

/* ============================================================================
    Internal Functions
============================================================================ */

// this function is mainly to test c-lib
static void _tzReverseArray(UINT8* pu1Array, UINT32 u4Size)
{
    #define SIZE 512*1024
    UINT32 i;
    UINT8* pu1Buf;

    // we allocate a big array to check memory pool size
    pu1Buf = (UINT8*)malloc(SIZE);
    if (!pu1Buf)
    {
        Printf("malloc error!\n");
        return;
    }
    Printf("pu1Buf=0x%x, size=%d\n", pu1Buf, SIZE);
    memset((VOID*)pu1Buf, 0, SIZE);

    for (i=0; i<(u4Size); i++)
    {
        pu1Buf[i] = pu1Array[u4Size-1-i];
    }
    memcpy((VOID*)pu1Array, (VOID*)pu1Buf, u4Size);
    free(pu1Buf);
}

/* ============================================================================
    Exported Entry Points
============================================================================ */
UINT32 SvcEx_TZCTL(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    DMSG("smcid=0x%x, cmd=0x%x, arg2~7=(0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x)\n",
        prSMCArg->u4SMCID, prSMCArg->u4R1, prSMCArg->u4R2,
        prSMCArg->u4R3, prSMCArg->u4R4, prSMCArg->u4R5,
        prSMCArg->u4R6, prSMCArg->u4R7);

    switch (prSMCArg->u4R1)
    {
    case TZCTL_SYSTEM_ENABLE_DEBUG:
        enableDebug(*(BOOL*)prSMCArg->u4R2);
        break;
    case TZCTL_SYSTEM_REVERSE_ARRAY:
        _tzReverseArray((UINT8*)prSMCArg->u4R5, prSMCArg->u4R6);
        break;
    
    }

    return TZ_SUCCESS;
}

