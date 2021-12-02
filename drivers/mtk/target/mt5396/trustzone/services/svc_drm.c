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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: dmx_descrambler.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_descrambler.c
 *  Demux driver - descrambler
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "aes.h"

#include "tzst.h"
#include "svc_registry.h"
#include "mtskb.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define WV_KEYBOX_DEVICE_ID_OFFSET          (0)
#define WV_KEYBOX_DEVICE_KEY_OFFSET         (32)
#define WV_KEYBOX_DEVICE_DATA_OFFSET        (48)
#define WV_KEYBOX_SIZE                      (256)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#undef Printf
#define Printf(a, ...)

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static UINT8 g_au1Keybox[256] = {0}; //ecnrypted device key
//static UINT8 g_OEMKey[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x5, 0x06, 0x7,
//                               0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0xf};

static UINT8 g_OEMKey[16] = { 0xC5, 0xC5, 0x93, 0xEB, 0x32, 0x89, 0x0A, 0xDA,
                               0x15, 0x3C, 0x9F, 0xA2, 0x1F, 0x0E, 0xDE, 0x01};

static UINT8 g_au1AssetKey[16] = {0};
static UINT8 g_au1ContorlWord[16] = {0};

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static BOOL _SvcEx_DRM_GetRomBytes(UINT32 u4Offset, UINT8* pu1Buf, UINT32 u4Size)
{
    memcpy(pu1Buf, (void *)(0xf4000000 + u4Offset), u4Size);

    return TRUE;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
BOOL SvcEx_DRM_WV_SetAssetkey(UINT8* pu1EncAssetKey, UINT32 u4KeyLen)
{
    UINT8 au1ClearDeviceKey[16];
    aes_context ctx;
    UINT8 i;

    //Mtk_WVReadKeybox(g_au1Keybox, 256);

    // Decrypt [AES-128-ECB] the device key in the keybox with the MTK root key
    aes_setkey_dec(&ctx, g_OEMKey, (INT32)128);
    aes_crypt_ecb(&ctx, AES_DECRYPT, g_au1Keybox + WV_KEYBOX_DEVICE_KEY_OFFSET, au1ClearDeviceKey);

    //ShowKey("device key", au1ClearDeviceKey, 16);

    Printf("Secure world clear device Key:\n");
    for(i = 0; i < 16; i++)
    {
        if((i) % 16 == 0)
            Printf("\n");

        Printf("0x%x ", *(UINT8 *)(au1ClearDeviceKey + i));
    }
    Printf("\n");

    aes_setkey_dec(&ctx, au1ClearDeviceKey, (INT32)128);
    aes_crypt_ecb(&ctx, AES_DECRYPT, pu1EncAssetKey, g_au1AssetKey);

    Printf("Secure world Asset Key:\n");
    for(i = 0; i < 16; i++)
    {
        if((i) % 16 == 0)
            Printf("\n");

        Printf("0x%x ", *(UINT8 *)(g_au1AssetKey + i));
    }
    Printf("\n");

    return TRUE;
}


BOOL SvcEx_DRM_WV_DeriveCW(UINT8* pu1Ecm, UINT32* pu4Flags)
{
    UINT8 au1ClearEcm[32];
    UINT8 au1ECMIV[16];
    UINT8 i;
    aes_context ctx;

    for(i = 0; i < 16; i++)
    {
        au1ECMIV[i] = 0;
    }

    aes_setkey_dec(&ctx, g_au1AssetKey, 128);
    aes_crypt_cbc(&ctx, AES_DECRYPT, 32, au1ECMIV, pu1Ecm, au1ClearEcm);

    //ShowKey("clear ecm", au1ClearEcm, 32);

    memcpy(pu4Flags, au1ClearEcm, 4);
    memcpy(g_au1ContorlWord, au1ClearEcm + 4, 16);

    memcpy(pu1Ecm, au1ClearEcm, 32); //!!!TODO: leak ecm to normal world

    Printf("Secure world Control word:\n");
    for(i = 0; i < 16; i++)
    {
        if((i) % 16 == 0)
            Printf("\n");

        Printf("0x%x ", *(UINT8 *)(g_au1ContorlWord + i));
    }
    Printf("\n");

    Printf("Secure world ECM: 0x%x\n", *(UINT32 *)pu4Flags);
    //ShowKey("control word", g_au1ContorlWord, 16);

    return TRUE;
}

BOOL SvcEx_DRM_WV_PassKeybox(UINT8* pu1Keybox, UINT32 u4Size)
{
    memcpy(g_au1Keybox, pu1Keybox, u4Size);

    return TRUE;
}

BOOL SvcEx_DRM_GetRomBytes(UINT32 u4Offset, UINT8 *pu1Buf, UINT32 u4Size)
{
    // Rom code is only 16KB
    if(u4Offset > 0x4000 || pu1Buf == NULL)
    {
        return TZ_ERROR_GENERIC;
    }

    if(u4Offset + u4Size > 0x4000)
    {
        return TZ_ERROR_GENERIC;
    }

    _SvcEx_DRM_GetRomBytes(u4Offset, pu1Buf, u4Size);

    HalFlushInvalidateDCacheMultipleLine((UINT32)pu1Buf, u4Size);

    return TRUE;
}

BOOL SvcEx_HCI_Init(MTDMX_MWIF_HCI_INIT_T* pTrustzone)
{
    pTrustzone->u4Ret = hci_Init();
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_INIT_T));
    return TRUE;
}

BOOL SvcEx_HCI_Fin(MTDMX_MWIF_HCI_INIT_T* pTrustzone)
{
    hci_Fin();
    return TRUE;
}

BOOL SvcEx_HCI_KeyRefCreate(MTDMX_MWIF_HCI_KEY_REF_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_KEY_REF_T));
    pTrustzone->u4Ret = hci_KeyRefCreate((hci_key_type_t)pTrustzone->u4KeyType,
        (hci_key_ref_t *)&pTrustzone->pKeyRef);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_KEY_REF_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->pKeyRef);
    return TRUE;
}

BOOL SvcEx_HCI_KeyRefCheck(MTDMX_MWIF_HCI_KEY_REF_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_KEY_REF_T));
    pTrustzone->u4Ret = hci_KeyRefCheck((hci_key_ref_t)pTrustzone->pKeyRef);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_KEY_REF_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->pKeyRef);
    return TRUE;
}

BOOL SvcEx_HCI_KeyRefDestroy(MTDMX_MWIF_HCI_KEY_REF_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_KEY_REF_T));
    pTrustzone->u4Ret = hci_KeyRefDestroy((hci_key_ref_t)pTrustzone->pKeyRef);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_KEY_REF_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->pKeyRef);
    return TRUE;
}

BOOL SvcEx_HCI_DevIdGetNum(MTDMX_MWIF_HCI_ID_T* pTrustzone)
{
    binstr_t **out_ids = NULL;
    UINT32 u4Num, i;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_ID_T));
    pTrustzone->u4Ret = hci_DevIdsGet(&out_ids, &u4Num);
    pTrustzone->u4IdNum = u4Num;
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_ID_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4IdNum);

    if (out_ids != NULL) {
        for (i = 0; i < u4Num; i++)
        {
            hci_BinstrFree(out_ids[i]);
        }
        tom_free(out_ids);
    }

    return TRUE;
}

BOOL SvcEx_HCI_DevIdGetSize(MTDMX_MWIF_HCI_ID_T* pTrustzone)
{
    binstr_t **out_ids = NULL;
    UINT32 u4Num, i;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_ID_T));
    pTrustzone->u4Ret = hci_DevIdsGet(&out_ids, &u4Num);
    pTrustzone->u4IdSize = out_ids[pTrustzone->u4IdIdx]->len;
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_ID_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4IdSize);

    if (out_ids != NULL) {
        for (i = 0; i < u4Num; i++)
        {
            hci_BinstrFree(out_ids[i]);
        }
        tom_free(out_ids);
    }

    return TRUE;
}

BOOL SvcEx_HCI_DevIdGetName(MTDMX_MWIF_HCI_ID_T* pTrustzone)
{
    binstr_t **out_ids = NULL;
    UINT32 u4Num, i;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_ID_T));
    pTrustzone->u4Ret = hci_DevIdsGet(&out_ids, &u4Num);
    memcpy(pTrustzone->pBuf, out_ids[pTrustzone->u4IdIdx]->buf, out_ids[pTrustzone->u4IdIdx]->len);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pBuf, out_ids[pTrustzone->u4IdIdx]->len);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_ID_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Ret);

    if (out_ids != NULL) {
        for (i = 0; i < u4Num; i++)
        {
            hci_BinstrFree(out_ids[i]);
        }
        tom_free(out_ids);
    }

    return TRUE;
}

BOOL SvcEx_HCI_ImportKey(MTDMX_MWIF_HCI_IMPORT_KEY_T* pTrustzone)
{
    hci_key_ref_t rKeyRef;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_IMPORT_KEY_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pOAEPParam, ((binstr_t *)pTrustzone->pOAEPParam)->len + sizeof(binstr_t));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pKeyData, ((binstr_t *)pTrustzone->pKeyData)->len + sizeof(binstr_t));
    pTrustzone->u4Ret = hci_ImportKey(pTrustzone->pKeyInRef, pTrustzone->u4EncAlg, pTrustzone->pOAEPParam,
        pTrustzone->pKeyData, pTrustzone->u4KeyType, &rKeyRef);
    pTrustzone->pKeyOutRef = rKeyRef;
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_IMPORT_KEY_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->pKeyOutRef);
    return TRUE;
}

BOOL SvcEx_HCI_ExportKeySize(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T* pTrustzone)
{
    binstr_t *out_data = NULL;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T));
    pTrustzone->u4Ret = hci_ExportKey(pTrustzone->pKeyRef, &out_data);
    if((pTrustzone->u4Ret == ERR_OK) && (out_data !=NULL))
    {
        pTrustzone->u4IdSize = out_data->len;
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4IdSize);

    if(out_data != NULL)
    {
        hci_l_FreeKey(out_data);
    }

    return TRUE;
}

BOOL SvcEx_HCI_ExportKey(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T* pTrustzone)
{
    binstr_t *out_data = NULL;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T));
    pTrustzone->u4Ret = hci_ExportKey(pTrustzone->pKeyRef, &out_data);
    if((pTrustzone->u4Ret == ERR_OK) && (out_data !=NULL))
    {
        memcpy(pTrustzone->pBuf, out_data->buf, out_data->len);
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pBuf, out_data->len);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Ret);

    if(out_data != NULL)
    {
        hci_l_FreeKey(out_data);
    }

    return TRUE;
}

BOOL SvcEx_HCI_StarfishIdGetSize(MTDMX_MWIF_HCI_ID_T* pTrustzone)
{
    binstr_t *out_id = NULL;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_ID_T));
    pTrustzone->u4Ret = hci_StarfishIdGet(&out_id);
    if((pTrustzone->u4Ret == ERR_OK) && (out_id !=NULL))
    {
        pTrustzone->u4IdSize = out_id->len;
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_ID_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4IdSize);

    if(out_id !=NULL)
    {
        hci_BinstrFree(out_id);
    }

    return TRUE;
}

BOOL SvcEx_HCI_StarfishIdGet(MTDMX_MWIF_HCI_ID_T* pTrustzone)
{
    binstr_t *out_id;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_ID_T));
    pTrustzone->u4Ret = hci_StarfishIdGet(&out_id);
    if((pTrustzone->u4Ret == ERR_OK) && (out_id !=NULL))
    {
        memcpy(pTrustzone->pBuf, out_id->buf, out_id->len);
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pBuf, out_id->len);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_ID_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Ret);

    if(out_id !=NULL)
    {
        hci_BinstrFree(out_id);
    }

    return TRUE;
}

BOOL SvcEx_HCI_KeyDataGetIfStarfishSize(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T* pTrustzone)
{
    binstr_t *out_data = NULL;

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T));
    pTrustzone->u4Ret = hci_KeyDataGetIfStarfish(pTrustzone->pKeyRef, &out_data);
    if((pTrustzone->u4Ret == ERR_OK) && (out_data !=NULL))
    {
        pTrustzone->u4IdSize = out_data->len;
    }
    else
    {
        pTrustzone->u4IdSize = 0;
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T));

    Printf("%s: %d 0x%x\n", __FUNCTION__, pTrustzone->u4IdSize, pTrustzone->u4Ret);

    if(out_data !=NULL)
    {
        hci_l_FreeKey(out_data);
    }

    return TRUE;
}

BOOL SvcEx_HCI_KeyDataGetIfStarfish(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T* pTrustzone)
{
    binstr_t *out_data = NULL;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T));
    pTrustzone->u4Ret = hci_KeyDataGetIfStarfish(pTrustzone->pKeyRef, &out_data);
    if((pTrustzone->u4Ret == ERR_OK) && (out_data !=NULL))
    {
        memcpy(pTrustzone->pBuf, out_data->buf, out_data->len);
    }
    else
    {
        pTrustzone->u4IdSize = 0;
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pBuf, out_data->len);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Ret);

    if(out_data !=NULL)
    {
        hci_l_FreeKey(out_data);
    }

    return TRUE;
}

BOOL SvcEx_HCI_ImportStarfishKey(MTDMX_MWIF_HCI_IMPORT_STARFISH_KEY_T* pTrustzone)
{
    hci_key_ref_t rKeyRef;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_IMPORT_STARFISH_KEY_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pEncBK, ((binstr_t *)pTrustzone->pEncBK)->len + sizeof(binstr_t));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pBKHash, ((binstr_t *)pTrustzone->pBKHash)->len + sizeof(binstr_t));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pIv, ((binstr_t *)pTrustzone->pIv)->len + sizeof(binstr_t));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pEncCK, ((binstr_t *)pTrustzone->pEncCK)->len + sizeof(binstr_t));
    pTrustzone->u4Ret = hci_ImportStarfishKey(pTrustzone->u4IterationNum, pTrustzone->u4KeyIndex,
        pTrustzone->pEncBK, pTrustzone->pBKHash, pTrustzone->pIv, pTrustzone->pEncCK,
        &rKeyRef);
    pTrustzone->pKeyOutRef = rKeyRef;
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_IMPORT_STARFISH_KEY_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->pKeyOutRef);
    return TRUE;
}

BOOL SvcEx_HCI_DataSignSize(MTDMX_MWIF_HCI_DATA_OP_T* pTrustzone)
{
    binstr_t *out_data = NULL;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_DATA_OP_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInData, ((binstr_t *)pTrustzone->pInData)->len + sizeof(binstr_t));
    pTrustzone->u4Ret = hci_DataSign(pTrustzone->pKeyInRef, pTrustzone->u4Alg,
        pTrustzone->pInData, &out_data);
    if(pTrustzone->u4Ret == ERR_OK)
    {
        pTrustzone->u4Size = out_data->len;
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_DATA_OP_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Size);

    if(out_data != NULL)
    {
        hci_BinstrFree(out_data);
    }

    return TRUE;
}

BOOL SvcEx_HCI_DataSign(MTDMX_MWIF_HCI_DATA_OP_T* pTrustzone)
{
    binstr_t *out_data = NULL;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_DATA_OP_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInData, ((binstr_t *)pTrustzone->pInData)->len + sizeof(binstr_t));
    pTrustzone->u4Ret = hci_DataSign(pTrustzone->pKeyInRef, pTrustzone->u4Alg,
        pTrustzone->pInData, &out_data);
    if(pTrustzone->u4Ret == ERR_OK)
    {
        memcpy(pTrustzone->pOutData, out_data->buf, out_data->len);
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_DATA_OP_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Ret);

    if(out_data != NULL)
    {
        hci_BinstrFree(out_data);
    }

    return TRUE;
}

BOOL SvcEx_HCI_DataDecryptSize(MTDMX_MWIF_HCI_DATA_OP_T* pTrustzone)
{
    binstr_t *out_data = NULL;
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_DATA_OP_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInOAEPParam, ((binstr_t *)pTrustzone->pInOAEPParam)->len + sizeof(binstr_t));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInData, ((binstr_t *)pTrustzone->pInData)->len + sizeof(binstr_t));
    pTrustzone->u4Ret = hci_DataDecrypt(pTrustzone->pKeyInRef, pTrustzone->u4Alg, pTrustzone->u4Pad,
        pTrustzone->pInSaltBuf, pTrustzone->pInOAEPParam, pTrustzone->pInData, &out_data);
    if(pTrustzone->u4Ret == ERR_OK)
    {
        pTrustzone->u4Size = out_data->len;
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_DATA_OP_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Size);

    if(out_data != NULL)
    {
        hci_BinstrFree(out_data);
    }

    return TRUE;
}

BOOL SvcEx_HCI_DataDecrypt(MTDMX_MWIF_HCI_DATA_OP_T* pTrustzone)
{
    binstr_t *out_data = NULL;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_DATA_OP_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInOAEPParam, ((binstr_t *)pTrustzone->pInOAEPParam)->len + sizeof(binstr_t));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInData, ((binstr_t *)pTrustzone->pInData)->len + sizeof(binstr_t));
    pTrustzone->u4Ret = hci_DataDecrypt(pTrustzone->pKeyInRef, pTrustzone->u4Alg, pTrustzone->u4Pad,
        pTrustzone->pInSaltBuf, pTrustzone->pInOAEPParam, pTrustzone->pInData, &out_data);
    if(pTrustzone->u4Ret == ERR_OK)
    {
        memcpy(pTrustzone->pOutData, out_data->buf, out_data->len);
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pOutData, out_data->len);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_DATA_OP_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Ret);

    if(out_data != NULL)
    {
        hci_BinstrFree(out_data);
    }

    return TRUE;
}

BOOL SvcEx_HCI_StreamDecryptSize(MTDMX_MWIF_HCI_DATA_OP_T* pTrustzone)
{
    binstr_t *out_data = NULL;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_DATA_OP_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInOAEPParam, ((binstr_t *)pTrustzone->pInOAEPParam)->len + sizeof(binstr_t));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInData, ((binstr_t *)pTrustzone->pInData)->len + sizeof(binstr_t));
    pTrustzone->u4Ret = hci_StreamDecrypt(pTrustzone->pKeyInRef, pTrustzone->u4Alg, pTrustzone->u4Pad,
        pTrustzone->pInSaltBuf, pTrustzone->pInOAEPParam, pTrustzone->pInData, &out_data);
    if(pTrustzone->u4Ret == ERR_OK)
    {
        pTrustzone->u4Size = out_data->len;
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_DATA_OP_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Size);

    if(out_data != NULL)
    {
        hci_BinstrFree(out_data);
    }

    return TRUE;
}

BOOL SvcEx_HCI_StreamDecrypt(MTDMX_MWIF_HCI_DATA_OP_T* pTrustzone)
{
    binstr_t *out_data = NULL;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_DATA_OP_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInOAEPParam, ((binstr_t *)pTrustzone->pInOAEPParam)->len + sizeof(binstr_t));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInData, ((binstr_t *)pTrustzone->pInData)->len + sizeof(binstr_t));
    pTrustzone->u4Ret = hci_StreamDecrypt(pTrustzone->pKeyInRef, pTrustzone->u4Alg, pTrustzone->u4Pad,
        pTrustzone->pInSaltBuf, pTrustzone->pInOAEPParam, pTrustzone->pInData, &out_data);
    if(pTrustzone->u4Ret == ERR_OK)
    {
        memcpy(pTrustzone->pOutData, out_data->buf, out_data->len);
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pOutData, out_data->len);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_DATA_OP_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Ret);

    if(out_data != NULL)
    {
        hci_BinstrFree(out_data);
    }

    return TRUE;
}

BOOL SvcEx_HCI_KeyDataEncryptSize(MTDMX_MWIF_HCI_DATA_OP_T* pTrustzone)
{
    binstr_t *out_data = NULL;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_DATA_OP_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInOAEPParam, ((binstr_t *)pTrustzone->pInOAEPParam)->len + sizeof(binstr_t));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInData, ((binstr_t *)pTrustzone->pInData)->len + sizeof(binstr_t));
    pTrustzone->u4Ret = hci_KeyDataEncrypt(pTrustzone->pKeyInRef, pTrustzone->pInOAEPParam,
        pTrustzone->pInData, &out_data);
    if( pTrustzone->u4Ret == ERR_OK)
    {
        pTrustzone->u4Size = out_data->len;
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_DATA_OP_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Size);

    if(out_data != NULL)
    {
        hci_BinstrFree(out_data);
    }

    return TRUE;
}

BOOL SvcEx_HCI_KeyDataEncrypt(MTDMX_MWIF_HCI_DATA_OP_T* pTrustzone)
{
    binstr_t *out_data = NULL;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_DATA_OP_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInOAEPParam, ((binstr_t *)pTrustzone->pInOAEPParam)->len + sizeof(binstr_t));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInData, ((binstr_t *)pTrustzone->pInData)->len + sizeof(binstr_t));
    pTrustzone->u4Ret = hci_KeyDataEncrypt(pTrustzone->pKeyInRef, pTrustzone->pInOAEPParam,
        pTrustzone->pInData, &out_data);
    if( pTrustzone->u4Ret == ERR_OK)
    {
        memcpy(pTrustzone->pOutData, out_data->buf, out_data->len);
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pOutData, out_data->len);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_DATA_OP_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Ret);

    if(out_data != NULL)
    {
        hci_BinstrFree(out_data);
    }

    return TRUE;
}

BOOL SvcEx_HCI_HmacCheck(MTDMX_MWIF_HCI_HASH_T* pTrustzone)
{
    UINT32 u4Result;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_HASH_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInData, ((binstr_t *)pTrustzone->pInData)->len + sizeof(binstr_t));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInExpected, ((binstr_t *)pTrustzone->pInExpected)->len + sizeof(binstr_t));
    pTrustzone->u4Ret = hci_HmacCheck(pTrustzone->pKeyInRef, pTrustzone->u4Alg, pTrustzone->pInData,
        pTrustzone->pInExpected, &u4Result);
    pTrustzone->u4OutResult = u4Result;
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_HASH_T));

    Printf("%s: %d\n", __FUNCTION__, u4Result);
    return TRUE;
}

BOOL SvcEx_HCI_HashCheck(MTDMX_MWIF_HCI_HASH_T* pTrustzone)
{
    UINT32 u4Result;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_HASH_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInData, ((binstr_t *)pTrustzone->pInData)->len + sizeof(binstr_t));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInExpected, ((binstr_t *)pTrustzone->pInExpected)->len + sizeof(binstr_t));
    pTrustzone->u4Ret = hci_HashCheck(pTrustzone->pKeyInRef, pTrustzone->u4Alg, pTrustzone->pInData,
        pTrustzone->pInExpected, &u4Result);
    pTrustzone->u4OutResult = u4Result;
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_HASH_T));

    Printf("%s: %d\n", __FUNCTION__, u4Result);
    return TRUE;
}

BOOL SvcEx_HCI_MoveKeySize(MTDMX_MWIF_HCI_MOVEKEY_T* pTrustzone)
{
    binstr_t *out_data = NULL;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_MOVEKEY_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInData, ((binstr_t *)pTrustzone->pInData)->len + sizeof(binstr_t));
    pTrustzone->u4Ret = hci_MoveKeySize(pTrustzone->pKeyRef, pTrustzone->pInData, &out_data);
    if(pTrustzone->u4Ret == ERR_OK)
    {
        pTrustzone->u4Size = out_data->len;
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_MOVEKEY_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Size);

    if(out_data != NULL)
    {
        hci_l_FreeKey(out_data);
    }

    return TRUE;
}

BOOL SvcEx_HCI_MoveKey(MTDMX_MWIF_HCI_MOVEKEY_T* pTrustzone)
{
    binstr_t *out_data = NULL;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_MOVEKEY_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInData, ((binstr_t *)pTrustzone->pInData)->len + sizeof(binstr_t));
    pTrustzone->u4Ret = hci_MoveKey(pTrustzone->pKeyRef, pTrustzone->pInData, &out_data);
    if(pTrustzone->u4Ret == ERR_OK)
    {
        memcpy(pTrustzone->pOutData, out_data->buf, out_data->len);
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pOutData, out_data->len);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_MOVEKEY_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Ret);

    if(out_data != NULL)
    {
        hci_l_FreeKey(out_data);
    }

    return TRUE;
}

BOOL SvcEx_HCI_PersonalityKeyLoad(MTDMX_MWIF_HCI_PERSONALITY_KEY_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_PERSONALITY_KEY_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInData, pTrustzone->u4InSize);
    pTrustzone->u4Ret = hci_PersonalityKeyLoad((hci_pers_key_type_t)pTrustzone->u4KeyType,
        pTrustzone->u4InSize, pTrustzone->pInData);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_PERSONALITY_KEY_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Ret);
    return TRUE;
}

BOOL SvcEx_HCI_PersonalityKeyCheck(MTDMX_MWIF_HCI_PERSONALITY_KEY_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_PERSONALITY_KEY_T));
    pTrustzone->u4Ret = hci_PersonalityKeyCheck((hci_pers_key_type_t)pTrustzone->u4KeyType);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_PERSONALITY_KEY_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Ret);
    return TRUE;
}

BOOL SvcEx_HCI_ImportSasSize(MTDMX_MWIF_HCI_IMPORT_SAS_T* pTrustzone)
{
    binstr_t *out_public_sas = NULL;
    hci_sas_keys_t *out_sas_keys = NULL;
    binstr_t **pp_out_public_sas;
    hci_sas_keys_t **pp_out_sas_keys;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_IMPORT_SAS_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInSAS, ((binstr_t *)pTrustzone->pInSAS)->len + sizeof(binstr_t));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInApp, ((binstr_t *)pTrustzone->pInApp)->len + sizeof(binstr_t));
    pp_out_public_sas = &out_public_sas;
    if(pTrustzone->pOutSAS == NULL)
    {
        pp_out_public_sas = NULL;
    }

    pp_out_sas_keys = &out_sas_keys;
    if(pTrustzone->pOutKeys == NULL)
    {
        pp_out_sas_keys = NULL;
    }

    pTrustzone->u4Ret = hci_ImportSAS(pTrustzone->pInSAS, pTrustzone->pInApp,
        pp_out_public_sas, pp_out_sas_keys);
    if(pTrustzone->u4Ret == ERR_OK)
    {
        memcpy(pTrustzone->pOutSAS, out_public_sas, sizeof(binstr_t));
        memcpy(pTrustzone->pOutKeys, out_sas_keys, sizeof(hci_sas_keys_t));
        HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pOutSAS, sizeof(binstr_t));
        HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pOutKeys, sizeof(hci_sas_keys_t));
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_IMPORT_SAS_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Ret);

    if(out_public_sas != NULL)
    {
        hci_BinstrFree(out_public_sas);
    }
    if(out_sas_keys != NULL)
    {
        hci_FreeSASKeys(out_sas_keys);
    }

    return TRUE;
}

BOOL SvcEx_HCI_ImportSas(MTDMX_MWIF_HCI_IMPORT_SAS_T* pTrustzone)
{
    binstr_t *out_public_sas = NULL;
    hci_sas_keys_t *out_sas_keys = NULL;
    binstr_t **pp_out_public_sas;
    hci_sas_keys_t **pp_out_sas_keys;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_IMPORT_SAS_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInSAS, ((binstr_t *)pTrustzone->pInSAS)->len + sizeof(binstr_t));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInApp, ((binstr_t *)pTrustzone->pInApp)->len + sizeof(binstr_t));
    pp_out_public_sas = &out_public_sas;
    if(pTrustzone->pOutSAS == NULL)
    {
        pp_out_public_sas = NULL;
    }

    pp_out_sas_keys = &out_sas_keys;
    if(pTrustzone->pOutKeys == NULL)
    {
        pp_out_sas_keys = NULL;
    }

    pTrustzone->u4Ret = hci_ImportSAS(pTrustzone->pInSAS, pTrustzone->pInApp,
        pp_out_public_sas, pp_out_sas_keys);
    if(pTrustzone->u4Ret == ERR_OK)
    {
        memcpy(((binstr_t *)pTrustzone->pOutSAS)->buf, out_public_sas->buf, out_public_sas->len);
        memcpy(((hci_sas_keys_t *)pTrustzone->pOutKeys)->key_refs, out_sas_keys->key_refs, sizeof(hci_key_ref_t) * pTrustzone->u4OutKeysCount);
        HalFlushInvalidateDCacheMultipleLine((UINT32)((binstr_t *)pTrustzone->pOutSAS)->buf, out_public_sas->len);
        HalFlushInvalidateDCacheMultipleLine((UINT32)((hci_sas_keys_t *)pTrustzone->pOutKeys)->key_refs, sizeof(hci_key_ref_t) * pTrustzone->u4OutKeysCount);
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_IMPORT_SAS_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Ret);

    if(out_public_sas != NULL)
    {
        hci_BinstrFree(out_public_sas);
    }
    if(out_sas_keys != NULL)
    {
        hci_FreeSASKeysButKeepKeyRef(out_sas_keys);
    }

    return TRUE;
}

BOOL SvcEx_HCI_ImportSasIds(MTDMX_MWIF_HCI_IMPORT_SAS_T* pTrustzone)
{
    binstr_t *out_public_sas = NULL;
    hci_sas_keys_t *out_sas_keys = NULL;
    binstr_t **pp_out_public_sas;
    hci_sas_keys_t **pp_out_sas_keys;
    UINT32 i;

    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_IMPORT_SAS_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInSAS, ((binstr_t *)pTrustzone->pInSAS)->len + sizeof(binstr_t));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInApp, ((binstr_t *)pTrustzone->pInApp)->len + sizeof(binstr_t));
    pp_out_public_sas = &out_public_sas;
    if(pTrustzone->pOutSAS == NULL)
    {
        pp_out_public_sas = NULL;
    }

    pp_out_sas_keys = &out_sas_keys;
    if(pTrustzone->pOutKeys == NULL)
    {
        pp_out_sas_keys = NULL;
    }

    pTrustzone->u4Ret = hci_ImportSAS(pTrustzone->pInSAS, pTrustzone->pInApp,
        pp_out_public_sas, pp_out_sas_keys);
    if(pTrustzone->u4Ret == ERR_OK)
    {
        for(i = 0; i < pTrustzone->u4OutKeysCount; i++)
        {
            memcpy(((hci_sas_keys_t *)pTrustzone->pOutKeys)->contentids[i]->buf, out_sas_keys->contentids[i]->buf, L_MS3_CONTENT_ID_SIZE);
            HalFlushInvalidateDCacheMultipleLine(((hci_sas_keys_t *)pTrustzone->pOutKeys)->contentids[i]->buf, L_MS3_CONTENT_ID_SIZE);
        }
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_HCI_IMPORT_SAS_T));

    Printf("%s: %d\n", __FUNCTION__, pTrustzone->u4Ret);

    if(out_public_sas != NULL)
    {
        hci_BinstrFree(out_public_sas);
    }
    if(out_sas_keys != NULL)
    {
        hci_FreeSASKeys(out_sas_keys);
    }

    return TRUE;
}

BOOL SvcEx_SKB_GetInstance(MTDMX_MWIF_SKB_ENGINE_GET_INSTANCE_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_ENGINE_GET_INSTANCE_T));
    pTrustzone->rRet = SKB_Engine_GetInstance(&pTrustzone->pEngine);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_ENGINE_GET_INSTANCE_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->pEngine);
    return TRUE;
}

BOOL SvcEx_SKB_Engine_Release(MTDMX_MWIF_SKB_ENGINE_RELEASE_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_ENGINE_RELEASE_T));
    pTrustzone->rRet = SKB_Engine_Release(pTrustzone->pEngine);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_ENGINE_RELEASE_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->rRet);
    return TRUE;
}

BOOL SvcEx_SKB_GetInfo(MTDMX_MWIF_SKB_ENGINE_GET_INFO_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_ENGINE_GET_INFO_T));
    pTrustzone->rRet = SKB_Engine_GetInfo(pTrustzone->pEngine, pTrustzone->pInfo);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_ENGINE_GET_INFO_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->rRet);
    return TRUE;
}

BOOL SvcEx_SKB_GetProertiesSize(MTDMX_MWIF_SKB_ENGINE_GET_INFO_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_ENGINE_GET_INFO_T));
    pTrustzone->rRet = SKB_Engine_GetInfo(pTrustzone->pEngine, pTrustzone->pInfo);
    pTrustzone->u4PropertiesNameSize = x_strlen(pTrustzone->pInfo->properties[pTrustzone->u4PropertiesIndex].name);
    pTrustzone->u4PropertiesValueSize = x_strlen(pTrustzone->pInfo->properties[pTrustzone->u4PropertiesIndex].value);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_ENGINE_GET_INFO_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->rRet);
    return TRUE;
}

BOOL SvcEx_SKB_GetProertiesData(MTDMX_MWIF_SKB_ENGINE_GET_INFO_T* pTrustzone)
{
    SKB_EngineInfo rInfo;
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_ENGINE_GET_INFO_T));
    pTrustzone->rRet = SKB_Engine_GetInfo(pTrustzone->pEngine, &rInfo);
    pTrustzone->u4PropertiesNameSize = x_strlen(rInfo.properties[pTrustzone->u4PropertiesIndex].name);
    pTrustzone->u4PropertiesValueSize = x_strlen(rInfo.properties[pTrustzone->u4PropertiesIndex].value);
    memcpy(pTrustzone->pProperties->name,
        rInfo.properties[pTrustzone->u4PropertiesIndex].name, pTrustzone->u4PropertiesNameSize);
    HalFlushInvalidateDCacheMultipleLine(pTrustzone->pProperties->name, pTrustzone->u4PropertiesNameSize);
    memcpy(pTrustzone->pProperties->value,
        rInfo.properties[pTrustzone->u4PropertiesIndex].value, pTrustzone->u4PropertiesValueSize);
    HalFlushInvalidateDCacheMultipleLine(pTrustzone->pProperties->value, pTrustzone->u4PropertiesValueSize);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_ENGINE_GET_INFO_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->rRet);
    return TRUE;
}

BOOL SvcEx_SKB_DataFromWrapped(MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_WRAPPED_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_WRAPPED_T));
    pTrustzone->rRet = SKB_Engine_CreateDataFromWrapped(pTrustzone->pEngine,
        pTrustzone->wrapped, pTrustzone->wrapped_size, pTrustzone->wrapped_type, pTrustzone->wrapped_format,
        pTrustzone->wrapping_algorithm, pTrustzone->wrapping_parameters, pTrustzone->unwrapping_key,
        &pTrustzone->data);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_WRAPPED_T));
    Printf("%s: 0x%x 0x%x\n", __FUNCTION__, pTrustzone->rRet, pTrustzone->data);
    return TRUE;
}

BOOL SvcEx_SKB_DataFromExported(MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_EXPORTED_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_EXPORTED_T));
    pTrustzone->rRet = SKB_Engine_CreateDataFromExported(pTrustzone->pEngine, pTrustzone->exported, pTrustzone->exported_size,
        &pTrustzone->data);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_EXPORTED_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->rRet);
    return TRUE;
}

BOOL SvcEx_SKB_CreateTransform(MTDMX_MWIF_SKB_ENGINE_CREATE_TRANSFORM_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_ENGINE_CREATE_TRANSFORM_T));
    pTrustzone->rRet = SKB_Engine_CreateTransform(pTrustzone->pEngine, pTrustzone->transform_type,
                           pTrustzone->transform_parameters, &pTrustzone->transform);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_ENGINE_CREATE_TRANSFORM_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->rRet);
    return TRUE;
}

BOOL SvcEx_SKB_CreateCipher(MTDMX_MWIF_SKB_ENGINE_CREATE_CIPHER_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_ENGINE_CREATE_CIPHER_T));
    pTrustzone->rRet = SKB_Engine_CreateCipher(pTrustzone->pEngine, pTrustzone->cipher_algorithm,
        pTrustzone->cipher_direction, pTrustzone->cipher_flags, pTrustzone->cipher_parameters,
        pTrustzone->cipher_key, &pTrustzone->cipher);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_ENGINE_CREATE_CIPHER_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->rRet);
    return TRUE;
}

BOOL SvcEx_SKB_SecureData_Release(MTDMX_MWIF_SKB_SECUREDATA_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_SECUREDATA_T));
    pTrustzone->rRet = SKB_SecureData_Release((SKB_SecureData *)pTrustzone->self);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_SECUREDATA_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->rRet);
    return TRUE;
}

BOOL SvcEx_SKB_SecureDataGetInfo(MTDMX_MWIF_SKB_SECUREDATA_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_SECUREDATA_T));
    pTrustzone->rRet = SKB_SecureData_GetInfo(pTrustzone->self, pTrustzone->info);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->info, sizeof(SKB_DataInfo));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_SECUREDATA_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->rRet);
    return TRUE;
}

BOOL SvcEx_SKB_SecureData_Export(MTDMX_MWIF_SKB_SECUREDATA_EXPORT_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_SECUREDATA_EXPORT_T));
    pTrustzone->rRet = SKB_SecureData_Export(pTrustzone->self, pTrustzone->target,
            pTrustzone->target_parameters, pTrustzone->buffer, pTrustzone->buffer_size);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->buffer, *(SKB_Size*)pTrustzone->buffer_size);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_SECUREDATA_EXPORT_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->rRet);
    return TRUE;
}

BOOL SvcEx_SKB_SecureData_Derive(MTDMX_MWIF_SKB_SECUREDATA_DERIVE_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_SECUREDATA_DERIVE_T));
    pTrustzone->rRet = SKB_SecureData_Derive(pTrustzone->self,
        pTrustzone->algorithm, pTrustzone->parameters, &pTrustzone->data);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_SECUREDATA_DERIVE_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->rRet);
    return TRUE;
}

BOOL SvcEx_SKB_Transform_Release(MTDMX_MWIF_SKB_TRANSFORM_ADDBYTES_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_TRANSFORM_ADDBYTES_T));
    pTrustzone->rRet = SKB_Transform_Release(pTrustzone->self);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_TRANSFORM_ADDBYTES_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->rRet);
    return TRUE;
}

BOOL SvcEx_SKB_Transform_AddBytes(MTDMX_MWIF_SKB_TRANSFORM_ADDBYTES_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_TRANSFORM_ADDBYTES_T));
    pTrustzone->rRet = SKB_Transform_AddBytes(pTrustzone->self, pTrustzone->data, pTrustzone->data_size);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_TRANSFORM_ADDBYTES_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->rRet);
    return TRUE;
}

BOOL SvcEx_SKB_Transform_AddSecureData(MTDMX_MWIF_SKB_TRANSFORM_ADDSECUREDATA_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_TRANSFORM_ADDSECUREDATA_T));
    pTrustzone->rRet = SKB_Transform_AddSecureData(pTrustzone->self, pTrustzone->data);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_TRANSFORM_ADDSECUREDATA_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->rRet);
    return TRUE;
}

BOOL SvcEx_SKB_Transform_GetOutput(MTDMX_MWIF_SKB_TRANSFORM_GETOUTPUT_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_TRANSFORM_GETOUTPUT_T));
    pTrustzone->rRet = SKB_Transform_GetOutput(pTrustzone->self, pTrustzone->output, pTrustzone->output_size);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->output, *(SKB_Size*)pTrustzone->output_size);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_TRANSFORM_GETOUTPUT_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->rRet);
    return TRUE;
}

BOOL SvcEx_SKB_Cipher_ProcessBuffer(MTDMX_MWIF_SKB_CIPHER_PROCESSBUFFER_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_CIPHER_PROCESSBUFFER_T));
    pTrustzone->rRet = SKB_Cipher_ProcessBuffer(pTrustzone->self, pTrustzone->in_buffer, pTrustzone->in_buffer_size,
        pTrustzone->out_buffer, pTrustzone->out_buffer_size, pTrustzone->iv, pTrustzone->iv_size);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->out_buffer, *(SKB_Size*)pTrustzone->out_buffer_size);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_CIPHER_PROCESSBUFFER_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->rRet);
    return TRUE;
}

BOOL SvcEx_SKB_Cipher_Release(MTDMX_MWIF_SKB_CIPHER_PROCESSBUFFER_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_CIPHER_PROCESSBUFFER_T));
    pTrustzone->rRet = SKB_Cipher_Release(pTrustzone->self);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_SKB_CIPHER_PROCESSBUFFER_T));

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->rRet);
    return TRUE;
}


				  
BOOL SvcEx_DRM_TZ_SetSecretDataType(MTDMX_MWIF_DRM_TZ_SETSECRETDATATYPE_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_DRM_TZ_SETSECRETDATATYPE_T));
    pTrustzone->i4Ret = MediaDRM_TZ_SetSecretDataType(NULL, pTrustzone->i4SecretDataType, 
        pTrustzone->pRootCertBuf, pTrustzone->u4RootCertBufLen, &pTrustzone->u4RootCertLen,
        pTrustzone->pCaCertBuf, pTrustzone->u4CaCertBufLen, &pTrustzone->u4CaCertLen,
        pTrustzone->pClientCertBuf, pTrustzone->u4ClientCertBufLen, &pTrustzone->u4ClientCertLen,
        pTrustzone->pClientKeyBuf, pTrustzone->u4ClientKeyBufLen, &pTrustzone->u4ClientKeyLen,
        pTrustzone->prErrorInf);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_DRM_TZ_SETSECRETDATATYPE_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->prErrorInf, sizeof(DRM_ErrorInf));
    
    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->i4Ret);
    return TRUE;
}
				  
BOOL SvcEx_DRM_TZ_Open(MTDMX_MWIF_DRM_TZ_OPEN_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_DRM_TZ_OPEN_T));
    pTrustzone->i4Ret = DecoderDRM_TZ_Open(&pTrustzone->i4DecSlot, NULL, pTrustzone->i4SecretDataType,
        pTrustzone->pLicenseObj, pTrustzone->u4LicenseObjLen, pTrustzone->pLid,
        pTrustzone->u4LidLen, pTrustzone->pExtractInfo, pTrustzone->prErrorInf);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_DRM_TZ_OPEN_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pExtractInfo, sizeof(DRM_ExtractInfo));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->prErrorInf, sizeof(DRM_ErrorInf));    

    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->i4Ret);
    return TRUE;
}
		       
BOOL SvcEx_DRM_TZ_Close(MTDMX_MWIF_DRM_TZ_OPEN_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_DRM_TZ_OPEN_T));
    pTrustzone->i4Ret = DecoderDRM_TZ_Close(pTrustzone->i4DecSlot, pTrustzone->prErrorInf);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_DRM_TZ_OPEN_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->prErrorInf, sizeof(DRM_ErrorInf));  
    
    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->i4Ret);
    return TRUE;
}
    
BOOL SvcEx_DRM_TZ_DecodePacketsVOD(MTDMX_MWIF_DRM_TZ_DECODEPACKETS_VOD_T* pTrustzone)
{
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_DRM_TZ_DECODEPACKETS_VOD_T));
    pTrustzone->i4Ret = DecoderDRM_TZ_DecodePackets_VOD(pTrustzone->i4DecSlot, 
        pTrustzone->u4PacketSize, pTrustzone->pInBuf, pTrustzone->u4InNumOfPackets,
		pTrustzone->pOffsets, pTrustzone->pOutBuf, pTrustzone->u4OutBufLen,
		&pTrustzone->u4OutNumOfPackets, pTrustzone->prErrorInf);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone, sizeof(MTDMX_MWIF_DRM_TZ_DECODEPACKETS_VOD_T));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->prErrorInf, sizeof(DRM_ErrorInf));  
    if(pTrustzone->pOutBuf != NULL)
    {
        HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pOutBuf, 
            (pTrustzone->u4PacketSize) * (pTrustzone->u4OutNumOfPackets));       
    }
    else
    {
        HalFlushInvalidateDCacheMultipleLine((UINT32)pTrustzone->pInBuf, 
            (pTrustzone->u4PacketSize) * (pTrustzone->u4OutNumOfPackets));            
    }
    Printf("%s: 0x%x\n", __FUNCTION__, pTrustzone->i4Ret);
    return TRUE;
}
				    
