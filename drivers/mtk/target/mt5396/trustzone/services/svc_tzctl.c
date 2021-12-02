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
#include "mtskb.h"
/* ============================================================================
    Internal Functions
============================================================================ */

// this function is mainly to test c-lib
static void _tzReverseArray(UINT8* pu1Array, UINT32 u4Size)
{
    #define SIZE 512*1024
    #define CACHE_LINE_ALIGN 31
    UINT32 i;
    UINT8* pu1Buf;
    //UINT8* pu1BufAlign;

    // we allocate a big array to check memory pool size
    pu1Buf = (UINT8*)malloc(SIZE);
    if (!pu1Buf)
    {
        Printf("malloc error!\n");
        return;
    }
    Printf("pu1Buf=0x%x, size=%d\n", pu1Buf, SIZE);
    memset((VOID*)pu1Buf, 0, SIZE);
    //pu1BufAlign = (UINT8*)(((UINT32)pu1Buf + CACHE_LINE_ALIGN) & ~CACHE_LINE_ALIGN);

    for (i=0; i<(u4Size); i++)
    {
        pu1Buf[i] = pu1Array[u4Size-1-i];
        //pu1BufAlign[i] = pu1Array[u4Size-1-i];
    }
    memcpy((VOID*)pu1Array, (VOID*)pu1Buf, u4Size);
    //memcpy((VOID*)pu1Array, (VOID*)pu1BufAlign, u4Size);
    HalFlushInvalidateDCacheMultipleLine(pu1Array, u4Size);
    free(pu1Buf);
}

static void SvcEx_TZ_IO_WRITE32(UINT32 u4Base, UINT32 u4Offset, UINT32 u4Val)
{
    (*((volatile UINT32*)(u4Base + u4Offset)) = (u4Val));
}

static void SvcEx_TZ_IO_WRITE16(UINT32 u4Base, UINT32 u4Offset, UINT16 u2Val)
{
    (*((volatile UINT16*)(u4Base + u4Offset)) = (u2Val));
}

static void SvcEx_TZ_IO_WRITE8(UINT32 u4Base, UINT32 u4Offset, UINT8 u1Val)
{
    (*((volatile UINT8*)(u4Base + u4Offset)) = (u1Val));
}

static void SvcEx_TZ_IO_READ32(UINT32 u4Base, UINT32 u4Offset, UINT32 *pu4Val)
{
    *(UINT32 *)pu4Val = *((volatile UINT32*)(u4Base + u4Offset));
}

static void SvcEx_TZ_IO_READ16(UINT32 u4Base, UINT32 u4Offset, UINT16 *pu2Val)
{
    *(UINT16 *)pu2Val = *((volatile UINT16*)(u4Base + u4Offset));
}

static void SvcEx_TZ_IO_READ8(UINT32 u4Base, UINT32 u4Offset, UINT8 *pu1Val)
{
    *(UINT8 *)pu1Val = *((volatile UINT8*)(u4Base + u4Offset));
}


/* ============================================================================
    Exported Entry Points
============================================================================ */
UINT32 SvcEx_TZCTL(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM)
{
    BOOL fgRet = TRUE;
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
    case TZCTL_SYSTEM_IO_WRITE32:
        SvcEx_TZ_IO_WRITE32(*(UINT32 *)prSMCArg->u4R2, *(UINT32 *)prSMCArg->u4R3, *(UINT32 *)prSMCArg->u4R4);
        break;
    case TZCTL_SYSTEM_IO_WRITE16:
        SvcEx_TZ_IO_WRITE16(*(UINT32 *)prSMCArg->u4R2, *(UINT32 *)prSMCArg->u4R3, *(UINT16 *)prSMCArg->u4R4);
        break;
    case TZCTL_SYSTEM_IO_WRITE8:
        SvcEx_TZ_IO_WRITE8(*(UINT32 *)prSMCArg->u4R2, *(UINT32 *)prSMCArg->u4R3, *(UINT8 *)prSMCArg->u4R4);
        break;
    case TZCTL_SYSTEM_IO_READ32:
        SvcEx_TZ_IO_READ32(*(UINT32 *)prSMCArg->u4R2, *(UINT32 *)prSMCArg->u4R3, (UINT32 *)prSMCArg->u4R4);
        break;
    case TZCTL_SYSTEM_IO_READ16:
        SvcEx_TZ_IO_READ16(*(UINT32 *)prSMCArg->u4R2, *(UINT32 *)prSMCArg->u4R3, (UINT16 *)prSMCArg->u4R4);
        break;
    case TZCTL_SYSTEM_IO_READ8:
        SvcEx_TZ_IO_READ8(*(UINT32 *)prSMCArg->u4R2, *(UINT32 *)prSMCArg->u4R3, (UINT8 *)prSMCArg->u4R4);
        break;
    case TZCTL_WV_SET_ASSET_KEY:
        fgRet = SvcEx_DRM_WV_SetAssetkey((UINT8*)prSMCArg->u4R2, *(UINT32 *)prSMCArg->u4R3);
        break;
    case TZCTL_WV_SET_DERIVE_CW:
        fgRet = SvcEx_DRM_WV_DeriveCW((UINT8*)prSMCArg->u4R2, (UINT32 *)prSMCArg->u4R3);
        break;
    case TZCTL_WV_PASS_KEYBOX:
        fgRet = SvcEx_DRM_WV_PassKeybox((UINT8*)prSMCArg->u4R2, *(UINT32 *)prSMCArg->u4R3);
        break;
    case TZCTL_BIM_GETROMBYTES:
        fgRet = SvcEx_DRM_GetRomBytes(*(UINT32 *)prSMCArg->u4R2, (UINT8*)prSMCArg->u4R3, *(UINT32 *)prSMCArg->u4R4);
        break;                
    case TZCTL_HCI_INIT:
        fgRet = SvcEx_HCI_Init((MTDMX_MWIF_HCI_INIT_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_FIN:
        fgRet = SvcEx_HCI_Fin((MTDMX_MWIF_HCI_INIT_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_KEYREF_CREATE:
        fgRet = SvcEx_HCI_KeyRefCreate((MTDMX_MWIF_HCI_KEY_REF_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_KEYREF_CHECK:
        fgRet = SvcEx_HCI_KeyRefCheck((MTDMX_MWIF_HCI_KEY_REF_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_KEYREF_DESTROY:
        fgRet = SvcEx_HCI_KeyRefDestroy((MTDMX_MWIF_HCI_KEY_REF_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_DEVICE_GET_NUM:
        fgRet = SvcEx_HCI_DevIdGetNum((MTDMX_MWIF_HCI_ID_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_DEVICE_GET_SIZE:
        fgRet = SvcEx_HCI_DevIdGetSize((MTDMX_MWIF_HCI_ID_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_DEVICE_GET_NAME:
        fgRet = SvcEx_HCI_DevIdGetName((MTDMX_MWIF_HCI_ID_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_IMPORT_KEY:
        fgRet = SvcEx_HCI_ImportKey((MTDMX_MWIF_HCI_IMPORT_KEY_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_EXPORT_KEY_SIZE:
        fgRet = SvcEx_HCI_ExportKeySize((MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_EXPORT_KEY:
        fgRet = SvcEx_HCI_ExportKey((MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_GET_START_FISH_ID_SIZE:
        fgRet = SvcEx_HCI_StarfishIdGetSize((MTDMX_MWIF_HCI_ID_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_GET_START_FISH_ID:
        fgRet = SvcEx_HCI_StarfishIdGet((MTDMX_MWIF_HCI_ID_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_GET_START_FISH_KEYDATA_SIZE:
        fgRet = SvcEx_HCI_KeyDataGetIfStarfishSize((MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_GET_START_FISH_KEYDATA:
        fgRet = SvcEx_HCI_KeyDataGetIfStarfish((MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_IMPORT_START_FISH_KEY:
        fgRet = SvcEx_HCI_ImportStarfishKey((MTDMX_MWIF_HCI_IMPORT_STARFISH_KEY_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_DATA_SIGN_SIZE:
        fgRet = SvcEx_HCI_DataSignSize((MTDMX_MWIF_HCI_DATA_OP_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_DATA_SIGN:
        fgRet = SvcEx_HCI_DataSign((MTDMX_MWIF_HCI_DATA_OP_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_DATA_DECRYPT_SIZE:
        fgRet = SvcEx_HCI_DataDecryptSize((MTDMX_MWIF_HCI_DATA_OP_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_DATA_DECRYPT:
        fgRet = SvcEx_HCI_DataDecrypt((MTDMX_MWIF_HCI_DATA_OP_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_STREAM_DECRYPT_SIZE:
        fgRet = SvcEx_HCI_StreamDecryptSize((MTDMX_MWIF_HCI_DATA_OP_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_STREAM_DECRYP:
        fgRet = SvcEx_HCI_StreamDecrypt((MTDMX_MWIF_HCI_DATA_OP_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_KEYDATA_ENCRYPT_SIZE:
        fgRet = SvcEx_HCI_KeyDataEncryptSize((MTDMX_MWIF_HCI_DATA_OP_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_KEYDATA_ENCRYPT:
        fgRet = SvcEx_HCI_KeyDataEncrypt((MTDMX_MWIF_HCI_DATA_OP_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_HMAC_CHECK:
        fgRet = SvcEx_HCI_HmacCheck((MTDMX_MWIF_HCI_HASH_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_HASH_CHECK:
        fgRet = SvcEx_HCI_HashCheck((MTDMX_MWIF_HCI_HASH_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_MOVE_KEY_SIZE:
        fgRet = SvcEx_HCI_MoveKeySize((MTDMX_MWIF_HCI_MOVEKEY_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_MOVE_KEY:
        fgRet = SvcEx_HCI_MoveKey((MTDMX_MWIF_HCI_MOVEKEY_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_PERSONALITYKEY_LOAD:
        fgRet = SvcEx_HCI_PersonalityKeyLoad((MTDMX_MWIF_HCI_PERSONALITY_KEY_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_PERSONALITYKEY_CHECK:
        fgRet = SvcEx_HCI_PersonalityKeyCheck((MTDMX_MWIF_HCI_PERSONALITY_KEY_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_IMPORT_SAS_SIZE:
        fgRet = SvcEx_HCI_ImportSasSize((MTDMX_MWIF_HCI_IMPORT_SAS_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_IMPORT_SAS:
        fgRet = SvcEx_HCI_ImportSas((MTDMX_MWIF_HCI_IMPORT_SAS_T *)prSMCArg->u4R2);
        break;
    case TZCTL_HCI_IMPORT_SAS_IDS:
        fgRet = SvcEx_HCI_ImportSasIds((MTDMX_MWIF_HCI_IMPORT_SAS_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_GET_INSTANCE:
        fgRet = SvcEx_SKB_GetInstance((MTDMX_MWIF_SKB_ENGINE_GET_INSTANCE_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_RELEASE:
        fgRet = SvcEx_SKB_Engine_Release((MTDMX_MWIF_SKB_ENGINE_RELEASE_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_GET_INFO:
        fgRet = SvcEx_SKB_GetInfo((MTDMX_MWIF_SKB_ENGINE_GET_INFO_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_GET_PROPERTIES_SIZE:
        fgRet = SvcEx_SKB_GetProertiesSize((MTDMX_MWIF_SKB_ENGINE_GET_INFO_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_GET_PROPERTIES_DATA:
        fgRet = SvcEx_SKB_GetProertiesData((MTDMX_MWIF_SKB_ENGINE_GET_INFO_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_CREATE_DATA_FROM_WRAPPED:
        fgRet = SvcEx_SKB_DataFromWrapped((MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_WRAPPED_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_CREATE_DATA_FROM_EXPORTED:
        fgRet = SvcEx_SKB_DataFromExported((MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_EXPORTED_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_CREATE_TRANSFORM:
        fgRet = SvcEx_SKB_CreateTransform((MTDMX_MWIF_SKB_ENGINE_CREATE_TRANSFORM_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_CREATE_CIPHER:
        fgRet = SvcEx_SKB_CreateCipher((MTDMX_MWIF_SKB_ENGINE_CREATE_CIPHER_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_SECUREDATA_RELEASE:
        fgRet = SvcEx_SKB_SecureData_Release((MTDMX_MWIF_SKB_SECUREDATA_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_SECUREDATA_GETINFO:
        fgRet = SvcEx_SKB_SecureDataGetInfo((MTDMX_MWIF_SKB_SECUREDATA_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_SECUREDATA_EXPORT:
        fgRet = SvcEx_SKB_SecureData_Export((MTDMX_MWIF_SKB_SECUREDATA_EXPORT_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_SECUREDATA_DERIVE:
        fgRet = SvcEx_SKB_SecureData_Derive((MTDMX_MWIF_SKB_SECUREDATA_DERIVE_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_TRANSFORM_RELEASE:
        fgRet = SvcEx_SKB_Transform_Release((MTDMX_MWIF_SKB_TRANSFORM_ADDBYTES_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_TRANSFORM_ADDBYTES:
        fgRet = SvcEx_SKB_Transform_AddBytes((MTDMX_MWIF_SKB_TRANSFORM_ADDBYTES_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_TRANSFORM_ADDSECUREDATA:
        fgRet = SvcEx_SKB_Transform_AddSecureData((MTDMX_MWIF_SKB_TRANSFORM_ADDSECUREDATA_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_TRANSFORM_GETOUTPUT:
        fgRet = SvcEx_SKB_Transform_GetOutput((MTDMX_MWIF_SKB_TRANSFORM_GETOUTPUT_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_CIPHER_PROCESSBUFFER:
        fgRet = SvcEx_SKB_Cipher_ProcessBuffer((MTDMX_MWIF_SKB_CIPHER_PROCESSBUFFER_T *)prSMCArg->u4R2);
        break;
    case TZCTL_SKB_CIPHER_RELEASE:
        fgRet = SvcEx_SKB_Cipher_Release((MTDMX_MWIF_SKB_CIPHER_PROCESSBUFFER_T *)prSMCArg->u4R2);
        break;
    case TZCTL_DRM_TZ_SETSECRETDATATYPE:
        fgRet = SvcEx_DRM_TZ_SetSecretDataType((MTDMX_MWIF_DRM_TZ_SETSECRETDATATYPE_T *)prSMCArg->u4R2);
        break;       
    case TZCTL_DRM_TZ_OPEN:
        fgRet = SvcEx_DRM_TZ_Open((MTDMX_MWIF_DRM_TZ_OPEN_T *)prSMCArg->u4R2);
        break;          
    case TZCTL_DRM_TZ_CLOSE:
        fgRet = SvcEx_DRM_TZ_Close((MTDMX_MWIF_DRM_TZ_OPEN_T *)prSMCArg->u4R2);
        break;             
    case TZCTL_DRM_TZ_DECODEPACKETSVOD:
        fgRet = SvcEx_DRM_TZ_DecodePacketsVOD((MTDMX_MWIF_DRM_TZ_DECODEPACKETS_VOD_T *)prSMCArg->u4R2);
        break;            
    }

    return fgRet?TZ_SUCCESS:TZ_ERROR_GENERIC;
}

