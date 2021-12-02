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
 * $LastChangedRevision: 2875 $
 * ----------------------------------------------------------------------------
 */

#ifndef SVC_REGISTRY_H
#define SVC_REGISTRY_H

/* ============================================================================
    Includes
============================================================================ */
#include "tzst.h"
#include "tz.h"

EXTERN TZ_SERVICE_T const _garService[];
/* Service function table. */
EXTERN TZ_FUNCTION_T const _garMTKFunction[];
/* ============================================================================
   Export the function list to the core Secure World code.
============================================================================ */

EXTERN UINT32 SvcEx_TZInit(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_InvalidateICache(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_InvalidateDCache(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_MemoryAccessExample(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);

EXTERN UINT32 SvcEx_DMX_Init(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_DMX_SetDesIV(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_DMX_SetDesIVEx(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_DMX_SetDesKey(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_DMX_SetAesIVEx(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_DMX_SetAesIV(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_DMX_SetAesKeyLen(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_DMX_SetAesKey(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_DMX_SetDmemAesIV(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_DMX_SetMulti2Key(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_DMX_SetMulti2CbcIV(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_DMX_SetMulti2OfbIV(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_DMX_SetMulti2SysKey(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_DMX_SetMulti2Config(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);

EXTERN UINT32 SvcEx_DMX_SetCpuDescKeyIv(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_DMX_SetCpuDescIntrClear(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);

EXTERN UINT32 SvcEx_DRM_WV_SetAssetKey(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_DRM_WV_DeriveCW(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_DRM_WV_PassKeybox(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_DRM_GetRomBytes(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_EnableL2(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
EXTERN UINT32 SvcEx_FlushL2(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);

EXTERN UINT32 SvcEx_TZCTL(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);


#endif  /* SVC_REGISTRY_H */
