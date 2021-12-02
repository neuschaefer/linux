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

static BOOL _SvcEx_DRM_WV_SetAssetkey(UINT8* pu1EncAssetKey, UINT32 u4KeyLen)
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


static BOOL _SvcEx_DRM_WV_DeriveCW(UINT8* pu1Ecm, UINT32* pu4Flags)
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

static BOOL _SvcEx_DRM_WV_PassKeybox(UINT8* pu1Keybox, UINT32 u4Size)
{
    memcpy(g_au1Keybox, pu1Keybox, u4Size);

    return TRUE;
}

static BOOL _SvcEx_DRM_GetRomBytes(UINT32 u4Offset, UINT8* pu1Buf, UINT32 u4Size)
{
    memcpy(pu1Buf, (void *)(0xf4000000 + u4Offset), u4Size);

    return TRUE;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


UINT32 SvcEx_DRM_WV_SetAssetKey(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM) 
{
    Printf("SvcEx_DRM_WV_SetAssetKey Addr: 0x%x Size: 0x%x\n", prSMCArg->u4R1, prSMCArg->u4R2);

    HalFlushInvalidateDCache();
    
    _SvcEx_DRM_WV_SetAssetkey((UINT8 *)prSMCArg->u4R1, (UINT32)prSMCArg->u4R2);

    HalFlushInvalidateDCache();
    
    return TZ_SUCCESS;
}

UINT32 SvcEx_DRM_WV_DeriveCW(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM) 
{
    Printf("SvcEx_DRM_WV_DeriveCW Addr: 0x%x Addr: 0x%x\n", prSMCArg->u4R1, prSMCArg->u4R2);

    HalFlushInvalidateDCache();    

    _SvcEx_DRM_WV_DeriveCW((UINT8 *)prSMCArg->u4R1, (UINT32 *)prSMCArg->u4R2);

    HalFlushInvalidateDCache();
    
    return TZ_SUCCESS;
}

UINT32 SvcEx_DRM_WV_PassKeybox(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM) 
{
    Printf("SvcEx_DRM_WV_PassKeybox Addr: 0x%x Size: 0x%x\n", prSMCArg->u4R1, prSMCArg->u4R2);

    HalFlushInvalidateDCache();
    
    _SvcEx_DRM_WV_PassKeybox((UINT8 *)prSMCArg->u4R1, (UINT32)prSMCArg->u4R2);

    HalFlushInvalidateDCache();
    
    return TZ_SUCCESS;
}

UINT32 SvcEx_DRM_GetRomBytes(TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM) 
{
    // Rom code is only 16KB
    if(prSMCArg->u4R1 > 0x4000 || prSMCArg->u4R2 == NULL)
    {
        return TZ_ERROR_GENERIC;
    }

    if(prSMCArg->u4R1 + prSMCArg->u4R3 > 0x4000) 
    {
        return TZ_ERROR_GENERIC;
    }
    
    HalFlushInvalidateDCache();
    
    _SvcEx_DRM_GetRomBytes((UINT32)prSMCArg->u4R1, (UINT8 *)prSMCArg->u4R2, (UINT32)prSMCArg->u4R3);

    HalFlushInvalidateDCache();
    
    return TZ_SUCCESS;
}

