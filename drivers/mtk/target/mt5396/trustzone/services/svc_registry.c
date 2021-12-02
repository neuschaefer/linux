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
 * $LastChangedRevision: 2862 $
 * ----------------------------------------------------------------------------
 */

/* ============================================================================
    Includes
============================================================================ */
/* Include the header importing Secure World definitions. */
#include "svc_registry.h"

/* ============================================================================
    The Service Registry
============================================================================ */
TZ_SERVICE_T const _garService[] =
{
    /* My example service. */
    {
        SVC_MTK_ID,
        NULL,                       /* No init function. */
        _garMTKFunction,
    },
    /* End of service. */
    {
        0,
        NULL,                       /* No init function. */
        NULL,
    }
};

TZ_FUNCTION_T const _garMTKFunction[] =
{
    /* Example ROT13 function. */
    {
        SMC_MTK_INIT_ID,
        SvcEx_TZInit
    },
    {
        SMC_MTK_INVALIDATEICACHE_ID,
        SvcEx_InvalidateICache
    },
    {
        SMC_MTK_INVALIDATEDCACHE_ID,
        SvcEx_InvalidateDCache
    },
    {
        SMC_MTK_MEMORY_ACCESS_EX,
        SvcEx_MemoryAccessExample
    },
    {
        SMC_MTK_ENABLE_L2,
        SvcEx_EnableL2
    },
    {
        SMC_MTK_FLUSH_L2,
        SvcEx_FlushL2
    },
    {
        SMC_MTK_DMX_INIT,
        SvcEx_DMX_Init
    },
    {
        SMC_MTK_DMX_SETDESIV,
        SvcEx_DMX_SetDesIV
    },
    {
        SMC_MTK_DMX_SETDESIVEX,
        SvcEx_DMX_SetDesIVEx
    },
    {
        SMC_MTK_DMX_SETDESKEY,
        SvcEx_DMX_SetDesKey
    },
    {
        SMC_MTK_DMX_SETAESIVEX,
        SvcEx_DMX_SetAesIVEx
    },
    {
        SMC_MTK_DMX_SETAESIV,
        SvcEx_DMX_SetAesIV
    },
    {
        SMC_MTK_DMX_SETAESKEYLEN,
        SvcEx_DMX_SetAesKeyLen
    },
    {
        SMC_MTK_DMX_SETAESKEY,
        SvcEx_DMX_SetAesKey
    },
    {
        SMC_MTK_DMX_SETDMEMAESIV,
        SvcEx_DMX_SetDmemAesIV
    },
    {
        SMC_MTK_DMX_SETMULTI2KEY,
        SvcEx_DMX_SetMulti2Key
    },
    {
        SMC_MTK_DMX_SETMULTI2CBCIV,
        SvcEx_DMX_SetMulti2CbcIV
    },
    {
        SMC_MTK_DMX_SetMULTI2OFBIV,
        SvcEx_DMX_SetMulti2OfbIV
    },
    {
        SMC_MTK_DMX_SetMULTI2SYSKEY,
        SvcEx_DMX_SetMulti2SysKey
    },
    {
        SMC_MTK_DMX_SetMULTI2CONFIG,
        SvcEx_DMX_SetMulti2Config
    },
    {
        SMC_MTK_DMX_CPUDESC_KEYIV,
        SvcEx_DMX_SetCpuDescKeyIv
    },
    {
        SMC_MTK_DMX_CPUDESC_INTRCLEAR,
        SvcEx_DMX_SetCpuDescIntrClear
    },
    {
        SMC_MTK_DRM_GETROMBYTES,
        SvcEx_DRM_GetRomBytes
    },
    {
        SMC_MTK_TZCTL,
        SvcEx_TZCTL
    },
    //end of function list
    {
        0,
        NULL
    },
};
