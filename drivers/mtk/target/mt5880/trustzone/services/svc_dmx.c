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

UINT32 SvcEx_DMX_Init(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    // Extend dmx data memroy for non-seucre access
    IO_WRITE32(DEMUX0_BASE, 0x14, 0x3FF0000);
   
    return TZ_SUCCESS;
}

UINT32 SvcEx_DMX_SetDesIV(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    
     Printf("SvcEx_DMX_SetDesIV\n");
     _DMX_SetDesIV((UINT32)prSMCArg->u4R1, (UINT32)prSMCArg->u4R2);

     return TZ_SUCCESS;
}

UINT32 SvcEx_DMX_SetDesIVEx(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
   
    Printf("SvcEx_DMX_SetDesIVEx\n");
    _DMX_SetDesIVEx((UINT32)prSMCArg->u4R1, (UINT32)prSMCArg->u4R2, (BOOL)prSMCArg->u4R3);

    return TZ_SUCCESS;
}

UINT32 SvcEx_DMX_SetDesKey(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    UINT32 *pu4Key = (UINT32 *)prSMCArg->u4R4;

    HalFlushInvalidateDCache();
    
    Printf("SvcEx_DMX_SetDesKey, Key Addr: 0x%x\n", pu4Key);

    Printf("Key: 0x%x 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", *(UINT32 *)(pu4Key), 
        *(UINT32 *)(pu4Key+1), *(UINT32 *)(pu4Key+2), *(UINT32 *)(pu4Key+3), 
        *(UINT32 *)(pu4Key+4), *(UINT32 *)(pu4Key+5));
    
    _DMX_SetDesKey((UINT8)prSMCArg->u4R1, (BOOL)prSMCArg->u4R2, 
        (BOOL)prSMCArg->u4R3, (UINT32 *)prSMCArg->u4R4);

    return TZ_SUCCESS;
}

UINT32 SvcEx_DMX_SetAesIVEx(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    UINT32 *pu4Key = (UINT32 *)prSMCArg->u4R1;

    HalFlushInvalidateDCache();
    
    Printf("SvcEx_DMX_SetAesIVEx, IV Addr: 0x%x\n", pu4Key);

    Printf("IV: 0x%x 0x%x, 0x%x, 0x%x\n", *(UINT32 *)(pu4Key), 
        *(UINT32 *)(pu4Key+1), *(UINT32 *)(pu4Key+2), *(UINT32 *)(pu4Key+3));
    
    _DMX_SetAesIVEx((UINT32 *)prSMCArg->u4R1, (BOOL)prSMCArg->u4R2);

    return TZ_SUCCESS;
}

UINT32 SvcEx_DMX_SetAesIV(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    UINT32 *pu4Key = (UINT32 *)prSMCArg->u4R1;

    HalFlushInvalidateDCache();
    
    Printf("SvcEx_DMX_SetAesIV, IV Addr: 0x%x\n", pu4Key);

    Printf("IV: 0x%x 0x%x, 0x%x, 0x%x\n", *(UINT32 *)(pu4Key), 
        *(UINT32 *)(pu4Key+1), *(UINT32 *)(pu4Key+2), *(UINT32 *)(pu4Key+3));

    pu4Key = pu4Key - 0x80000000;

    Printf("!!!SvcEx_DMX_SetAesIV, IV Addr: 0x%x\n", pu4Key);

    Printf("!!!IV: 0x%x 0x%x, 0x%x, 0x%x\n", *(UINT32 *)(pu4Key), 
        *(UINT32 *)(pu4Key+1), *(UINT32 *)(pu4Key+2), *(UINT32 *)(pu4Key+3));
    
    _DMX_SetAesIV((UINT32 *)prSMCArg->u4R1);
    
    return TZ_SUCCESS;
}

UINT32 SvcEx_DMX_SetAesKeyLen(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{ 
    _DMX_SetAesKeyLen((UINT32)prSMCArg->u4R1);
    
    return TZ_SUCCESS;
}

UINT32 SvcEx_DMX_SetAesKey(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{    
    UINT32 *pu4Key = (UINT32 *)prSMCArg->u4R3;

    HalFlushInvalidateDCache();
    
    Printf("SvcEx_DMX_SetAesKey, Key Addr: 0x%x\n", pu4Key);

    Printf("Key: 0x%x, 0x%x, 0x%x, 0x%x\n", *(UINT32 *)(pu4Key), 
        *(UINT32 *)(pu4Key+1), *(UINT32 *)(pu4Key+2), *(UINT32 *)(pu4Key+3));
    
    Printf("0x%x, 0x%x, 0x%x, 0x%x\n", *(UINT32 *)(pu4Key+4), 
        *(UINT32 *)(pu4Key+5), *(UINT32 *)(pu4Key+6), *(UINT32 *)(pu4Key+7));
    
    _DMX_SetAesKey((UINT8)prSMCArg->u4R1, (BOOL)prSMCArg->u4R2, (UINT32 *)prSMCArg->u4R3);
    
    return TZ_SUCCESS;
}

UINT32 SvcEx_DMX_SetDmemAesIV(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    UINT32 *pu4Key = (UINT32 *)prSMCArg->u4R1;

    HalFlushInvalidateDCache();
    
    Printf("SvcEx_DMX_SetDmemAesIV, IV Addr: 0x%x\n", pu4Key);

    Printf("IV: 0x%x 0x%x, 0x%x, 0x%x\n", *(UINT32 *)(pu4Key), 
        *(UINT32 *)(pu4Key+1), *(UINT32 *)(pu4Key+2), *(UINT32 *)(pu4Key+3));
    
    _DMX_SetDmemAesIV((UINT32 *)prSMCArg->u4R1);

    return TZ_SUCCESS;
}

UINT32 SvcEx_DMX_SetMulti2Key(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    UINT32 *pu4Key = (UINT32 *)prSMCArg->u4R3;
    
    HalFlushInvalidateDCache();    

    Printf("SvcEx_DMX_SetMulti2Key, Key Addr: 0x%x\n", pu4Key);

    Printf("Key: 0x%x 0x%x\n", *(UINT32 *)(pu4Key), 
        *(UINT32 *)(pu4Key+1));
    
    _DMX_SetMulti2Key((UINT8)prSMCArg->u4R1, (BOOL)prSMCArg->u4R2, (UINT32 *)prSMCArg->u4R3);
    
    return TZ_SUCCESS;
}

UINT32 SvcEx_DMX_SetMulti2CbcIV(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    _DMX_SetMulti2CbcIV((UINT32)prSMCArg->u4R1, (UINT32)prSMCArg->u4R2);

    return TZ_SUCCESS;
}
UINT32 SvcEx_DMX_SetMulti2OfbIV(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    _DMX_SetMulti2OfbIV((UINT32)prSMCArg->u4R1, (UINT32)prSMCArg->u4R2);

    return TZ_SUCCESS;
}

UINT32 SvcEx_DMX_SetMulti2SysKey(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    UINT32 *pu4Key = (UINT32 *)prSMCArg->u4R1;

    HalFlushInvalidateDCache();
    
    Printf("SvcEx_DMX_SetMulti2Key, Key Addr: 0x%x\n", pu4Key);

    Printf("Key: 0x%x, 0x%x, 0x%x, 0x%x\n", *(UINT32 *)(pu4Key), 
        *(UINT32 *)(pu4Key+1), *(UINT32 *)(pu4Key+2), *(UINT32 *)(pu4Key+3));
    
    Printf("0x%x, 0x%x, 0x%x, 0x%x\n", *(UINT32 *)(pu4Key+4), 
        *(UINT32 *)(pu4Key+5), *(UINT32 *)(pu4Key+6), *(UINT32 *)(pu4Key+7));
    
    _DMX_SetMulti2SysKey((UINT32 *)prSMCArg->u4R1);

    return TZ_SUCCESS;
}

UINT32 SvcEx_DMX_SetMulti2Config(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    _DMX_SetMulti2Config((BOOL)prSMCArg->u4R1, (UINT16)prSMCArg->u4R2);
    
    return TZ_SUCCESS;
}    

UINT32 SvcEx_DMX_SetCpuDescKeyIv(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM) 
{
    _DMX_SetCpuDescKeyIv((UINT32 *)prSMCArg->u4R1, (UINT32 *)prSMCArg->u4R2);
    
    return TZ_SUCCESS;    
}

UINT32 SvcEx_DMX_SetCpuDescIntrClear(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    _DMX_SetCpuDescIntrClear();

    return TZ_SUCCESS;    
}

