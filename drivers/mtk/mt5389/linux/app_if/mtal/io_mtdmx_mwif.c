/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * $RCSfile: io_mtdmx.c,v $
 * $Revision: #1 $
 * $Author: dtvbm11 $
 * $SWAuthor: wen han $
 * $MD5HEX:  $
 *---------------------------------------------------------------------------*/

/** @file io_mtdmx.c
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "mtdmx.h"
#include "dmx_if.h"
#include "dmx_mm_if.h"
#include "fvr_if.h"
#include "fbm_drvif.h"
#include "mpv_drvif.h"
#include "vdec_drvif.h"
#include "aud_drvif.h"
#include "stc_drvif.h"
#include "cb_data.h"
#include "cb_low.h"
#include <linux/mtal_ioctl.h>
#ifdef CC_TRUSTZONE_SUPPORT
#include "tz_if.h"
#endif
#include "x_assert.h"

#ifdef CC_ENABLE_SKB
#include "mtskb.h"
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define PRINTK(a, ...)
//#define PRINTK printk
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define COPY_FROM_USER_ARG_SIZE(ArgUsr, ArgKernel, size)       \
    if (copy_from_user(ArgKernel, (void __user *)ArgUsr,   \
                       (size)))                             \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error 1\n");      \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error 2\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error 3\n");      \
        return MTR_ERR_INV;                                          \
    }     \

#define USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(arg,size)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   size))              \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error 4\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_TO_USER_ARG_SIZE(ArgUsr, ArgKernel,size)              \
    if (copy_to_user((void __user *)ArgUsr, (void *)ArgKernel,   \
                       size))          \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error 5\n");      \
        return MTR_ERR_INV;                                          \
    } \


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static binstr_t *hci_BinstrAlloc(np_size_t in_size, const u_int8_t *in_buf)
{
    binstr_t *bs = NULL;

    /* check */
    if ( in_size > (UINT_MAX - sizeof(binstr_t)) )
        return NULL;

    /* alloc */
    bs = x_mem_alloc(in_size + sizeof(binstr_t));
    if ( NULL != bs ) {
        bs->len = in_size;
        bs->buf = (u_int8_t*)bs + sizeof(binstr_t);

        /* copy */
        if ( in_buf != NULL ) {
            x_memcpy( bs->buf, in_buf, in_size );
        } else {
            x_memset( bs->buf, 0, in_size );
        }
    }

    return bs;
}


static void hci_BinstrFree(binstr_t *io_str)
{
    if (io_str)
    {
        x_mem_free(io_str);
    }
}


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static BOOL _SKB_GetInstance(MTDMX_MWIF_SKB_ENGINE_GET_INSTANCE_T *prKernParam,
    MTDMX_MWIF_SKB_ENGINE_GET_INSTANCE_T *prUserParam)
{
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    prKernParam->rRet = SKB_Engine_GetInstance(&prKernParam->pEngine);
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_ENGINE_GET_INSTANCE_T));
    fgRet = TZ_SKB_GetInstance(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->pEngine, sizeof(SKB_Engine *));
    COPY_TO_USER_ARG_SIZE(&prUserParam->pEngine, &prKernParam->pEngine, sizeof(SKB_Engine *));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
    COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &prKernParam->rRet, sizeof(SKB_Result));

    PRINTK("-%s.....%d\n", __FUNCTION__, prKernParam->rRet);

    return TRUE;
}

static BOOL _SKB_Engine_Release(MTDMX_MWIF_SKB_ENGINE_RELEASE_T *prKernParam,
    MTDMX_MWIF_SKB_ENGINE_GET_INSTANCE_T *prUserParam)
{
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    prKernParam->rRet = SKB_Engine_Release(prKernParam->pEngine);
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_ENGINE_RELEASE_T));
    fgRet = TZ_SKB_Release(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
    COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &prKernParam->rRet, sizeof(SKB_Result));

    PRINTK("-%s.....%d\n", __FUNCTION__, prKernParam->rRet);

    return TRUE;
}

static BOOL _SKB_GetInfo(MTDMX_MWIF_SKB_ENGINE_GET_INFO_T *prKernParam,
    MTDMX_MWIF_SKB_ENGINE_GET_INFO_T *prUserParam)
{
    SKB_EngineInfo rInfo;
    SKB_Result rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = SKB_Engine_GetInfo(prKernParam->pEngine, &rInfo);
    #else
    prKernParam->pInfo = (SKB_EngineInfo *)PHYSICAL((UINT32)&rInfo);
    HalFlushInvalidateDCacheMultipleLine((UINT32)&rInfo, sizeof(rInfo));
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_ENGINE_GET_INFO_T));
    fgRet = TZ_SKB_GetInfo(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->rRet;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->pInfo, sizeof(SKB_EngineInfo));
    COPY_TO_USER_ARG_SIZE(prUserParam->pInfo, &rInfo, sizeof(SKB_EngineInfo));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
    COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}

static BOOL _SKB_GetProertiesSize(MTDMX_MWIF_SKB_ENGINE_GET_INFO_T *prKernParam,
    MTDMX_MWIF_SKB_ENGINE_GET_INFO_T *prUserParam)
{
    SKB_EngineInfo rInfo;
    SKB_Result rRet;
    UINT32 u4ValueSize;
    UINT32 u4NameSize;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = SKB_Engine_GetInfo(prKernParam->pEngine, &rInfo);
    u4NameSize = x_strlen(rInfo.properties[prKernParam->u4PropertiesIndex].name);
    u4ValueSize = x_strlen(rInfo.properties[prKernParam->u4PropertiesIndex].value);
    #else
    prKernParam->pInfo = (SKB_EngineInfo *)PHYSICAL((UINT32)&rInfo);
    HalFlushInvalidateDCacheMultipleLine((UINT32)&rInfo, sizeof(rInfo));
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_ENGINE_GET_INFO_T));
    fgRet = TZ_SKB_GetProertiesSize(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->rRet;
    u4NameSize = prKernParam->u4PropertiesNameSize;
    u4ValueSize = prKernParam->u4PropertiesValueSize;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4PropertiesNameSize, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4PropertiesNameSize, &u4NameSize, sizeof(UINT32));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4PropertiesValueSize, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4PropertiesValueSize, &u4ValueSize, sizeof(UINT32));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
    COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}

static BOOL _SKB_GetProertiesData(MTDMX_MWIF_SKB_ENGINE_GET_INFO_T *prKernParam,
    MTDMX_MWIF_SKB_ENGINE_GET_INFO_T *prUserParam)
{
    SKB_EngineInfo rInfo;
    SKB_Result rRet;
    UINT32 u4ValueSize;
    UINT32 u4NameSize;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    UINT8 *pName;
    UINT8 *pValue;
    SKB_EngineProperty rProperties;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = SKB_Engine_GetInfo(prKernParam->pEngine, &rInfo);
    u4NameSize = x_strlen(rInfo.properties[prKernParam->u4PropertiesIndex].name);
    u4ValueSize = x_strlen(rInfo.properties[prKernParam->u4PropertiesIndex].value);
    #else
    pName = x_mem_alloc(prKernParam->u4PropertiesNameSize);
    rProperties.name = (const char *)PHYSICAL((UINT32)pName);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pName, prKernParam->u4PropertiesNameSize);
    pValue = x_mem_alloc(prKernParam->u4PropertiesValueSize);
    rProperties.value = (const char *)PHYSICAL((UINT32)pValue);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pValue, prKernParam->u4PropertiesValueSize);
    prKernParam->pProperties = (SKB_EngineProperty *)PHYSICAL((UINT32)&rProperties);
    HalFlushInvalidateDCacheMultipleLine((UINT32)&rProperties, sizeof(rProperties));
    prKernParam->pInfo = (SKB_EngineInfo *)PHYSICAL((UINT32)&rInfo);
    HalFlushInvalidateDCacheMultipleLine((UINT32)&rInfo, sizeof(rInfo));
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_ENGINE_GET_INFO_T));
    fgRet = TZ_SKB_GetProertiesData(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->rRet;
    u4NameSize = prKernParam->u4PropertiesNameSize;
    u4ValueSize = prKernParam->u4PropertiesValueSize;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->pProperties->name, u4NameSize);
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->pProperties->value, u4ValueSize);
    #ifndef CC_TRUSTZONE_SUPPORT
    COPY_TO_USER_ARG_SIZE(prUserParam->pProperties->name,
        rInfo.properties[prKernParam->u4PropertiesIndex].name, u4NameSize);
    COPY_TO_USER_ARG_SIZE(prUserParam->pProperties->value,
        rInfo.properties[prKernParam->u4PropertiesIndex].value, u4ValueSize);
    #else
    COPY_TO_USER_ARG_SIZE(prUserParam->pProperties->name,
        pName, u4NameSize);
    COPY_TO_USER_ARG_SIZE(prUserParam->pProperties->value,
        pValue, u4ValueSize);
    x_mem_free(pName);
    x_mem_free(pValue);
    #endif
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
    COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _SKB_DataFromWrapped(MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_WRAPPED_T *prKernParam,
    MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_WRAPPED_T *prUserParam)
{
    SKB_SecureData* data;
    SKB_Byte *wrapped;
    SKB_Result rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s..... %d\n", __FUNCTION__, (UINT32)prKernParam->wrapped_size);

    wrapped = x_mem_alloc((UINT32)prKernParam->wrapped_size);
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prKernParam->wrapped, prKernParam->wrapped_size);
    COPY_FROM_USER_ARG_SIZE(prKernParam->wrapped, wrapped, prKernParam->wrapped_size);

    ///NOTE: prWrapped->wrapping_parameters should not be used in kernel,
    // we don't know how to handle this pointer and size
    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = SKB_Engine_CreateDataFromWrapped(prKernParam->pEngine,
        wrapped, prKernParam->wrapped_size, prKernParam->wrapped_type, prKernParam->wrapped_format,
        prKernParam->wrapping_algorithm, prKernParam->wrapping_parameters, prKernParam->unwrapping_key,
        &data);
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)wrapped, (UINT32)prKernParam->wrapped_size);
    prKernParam->wrapped = (const SKB_Byte* )PHYSICAL((UINT32)wrapped);
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_WRAPPED_T));
    PRINTK("TZ-%s.....0x%x\n", __FUNCTION__, prKernParam);
    HalFlushInvalidateDCache();
    fgRet = TZ_SKB_DataFromWrapped(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->rRet;
    data = prKernParam->data;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->data, sizeof(SKB_SecureData *));
    COPY_TO_USER_ARG_SIZE(&prUserParam->data, &data, sizeof(SKB_SecureData *));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
    COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

    x_mem_free(wrapped);

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _SKB_DataFromExported(MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_EXPORTED_T *prKernParam,
    MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_EXPORTED_T *prUserParam)
{
    SKB_SecureData* data;
    SKB_Byte *exported;
    SKB_Result rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....%d\n", __FUNCTION__, (UINT32)prKernParam->exported_size);

    exported = x_mem_alloc((UINT32)prKernParam->exported_size);
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prKernParam->exported, prKernParam->exported_size);
    COPY_FROM_USER_ARG_SIZE(prKernParam->exported, exported, prKernParam->exported_size);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = SKB_Engine_CreateDataFromExported(prKernParam->pEngine, exported, prKernParam->exported_size,
        &data);
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)exported, (UINT32)prKernParam->exported_size);
    prKernParam->exported = (const SKB_Byte*)PHYSICAL((UINT32)exported);
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_EXPORTED_T));
    fgRet = TZ_SKB_DataFromExported(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->rRet;
    data = prKernParam->data;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->data, sizeof(SKB_SecureData *));
    COPY_TO_USER_ARG_SIZE(&prUserParam->data, &data, sizeof(SKB_SecureData *));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
    COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

    x_mem_free(exported);

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _SKB_CreateTransform(MTDMX_MWIF_SKB_ENGINE_CREATE_TRANSFORM_T  *prKernParam,
    MTDMX_MWIF_SKB_ENGINE_CREATE_TRANSFORM_T  *prUserParam)
{
    SKB_Transform* pTransform = NULL;
    SKB_DigestTransformParameters *prDigestParam = NULL;
    SKB_VerifyTransformParameters *prVerifyParam = NULL;
    SKB_SignTransformParameters *pSignParam = NULL;
    SKB_Byte* signature = NULL;
    const void* transform_parameters = NULL;
    SKB_Result rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    switch(prKernParam->transform_type)
    {
    case SKB_TRANSFORM_TYPE_DIGEST:
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prKernParam->transform_parameters,
            sizeof(SKB_DigestTransformParameters));
        prDigestParam = x_mem_alloc(sizeof(SKB_DigestTransformParameters));
        COPY_FROM_USER_ARG_SIZE(prKernParam->transform_parameters, prDigestParam,
            sizeof(SKB_DigestTransformParameters));
        transform_parameters = prDigestParam;
        #ifdef CC_TRUSTZONE_SUPPORT
        HalFlushInvalidateDCacheMultipleLine((UINT32)prDigestParam, (UINT32)sizeof(SKB_DigestTransformParameters));
        prKernParam->transform_parameters = (const void* )PHYSICAL((UINT32)prDigestParam);
        #endif
        break;
    case SKB_TRANSFORM_TYPE_VERIFY:
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prKernParam->transform_parameters,
            sizeof(SKB_VerifyTransformParameters));
        prVerifyParam = x_mem_alloc(sizeof(SKB_VerifyTransformParameters));
        COPY_FROM_USER_ARG_SIZE(prKernParam->transform_parameters, prVerifyParam,
            sizeof(SKB_VerifyTransformParameters));
        transform_parameters = prVerifyParam;
        signature = x_mem_alloc(prVerifyParam->signature_size);
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prVerifyParam->signature,
            prVerifyParam->signature_size);
        COPY_FROM_USER_ARG_SIZE(prVerifyParam->signature, signature,
            prVerifyParam->signature_size);
        prVerifyParam->signature = signature;
        #ifdef CC_TRUSTZONE_SUPPORT
        HalFlushInvalidateDCacheMultipleLine((UINT32)signature, (UINT32)prVerifyParam->signature_size);
        prVerifyParam->signature = (const SKB_Byte*)PHYSICAL((UINT32)signature);
        HalFlushInvalidateDCacheMultipleLine((UINT32)prVerifyParam, (UINT32)sizeof(SKB_VerifyTransformParameters));
        prKernParam->transform_parameters = (const void*)PHYSICAL((UINT32)prVerifyParam);
        #endif
        break;
    case SKB_TRANSFORM_TYPE_SIGN:
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prKernParam->transform_parameters,
            sizeof(SKB_SignTransformParameters));
        pSignParam = x_mem_alloc(sizeof(SKB_SignTransformParameters));
        COPY_FROM_USER_ARG_SIZE(prKernParam->transform_parameters, pSignParam,
            sizeof(SKB_SignTransformParameters));
        transform_parameters = pSignParam;
        #ifdef CC_TRUSTZONE_SUPPORT
        HalFlushInvalidateDCacheMultipleLine((UINT32)pSignParam, (UINT32)sizeof(SKB_SignTransformParameters));
        prKernParam->transform_parameters = (const void*)PHYSICAL((UINT32)pSignParam);
        #endif
        break;
    }

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = SKB_Engine_CreateTransform(prKernParam->pEngine, prKernParam->transform_type,
                           transform_parameters, &pTransform);
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_ENGINE_CREATE_TRANSFORM_T));
    fgRet = TZ_SKB_CreateTransform(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->rRet;
    pTransform = prKernParam->transform;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->transform, sizeof(SKB_Transform*));
    COPY_TO_USER_ARG_SIZE(&prUserParam->transform, &pTransform, sizeof(SKB_Transform*));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
    COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

    switch(prKernParam->transform_type)
    {
    case SKB_TRANSFORM_TYPE_DIGEST:
        x_mem_free(prDigestParam);
        break;
    case SKB_TRANSFORM_TYPE_VERIFY:
        x_mem_free(prVerifyParam);
        x_mem_free(signature);
        break;
    case SKB_TRANSFORM_TYPE_SIGN:
        x_mem_free(pSignParam);
        break;
    }

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _SKB_CreateCipher(MTDMX_MWIF_SKB_ENGINE_CREATE_CIPHER_T *prKernParam,
    MTDMX_MWIF_SKB_ENGINE_CREATE_CIPHER_T *prUserParam)
{
    SKB_Cipher* pCipher;
    SKB_Result rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    ///NOTE: prCipher->cipher_parameters should not be used in kernel,
    // we don't know how to handle this pointer and size

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = SKB_Engine_CreateCipher(prKernParam->pEngine, prKernParam->cipher_algorithm,
        prKernParam->cipher_direction, prKernParam->cipher_flags, prKernParam->cipher_parameters,
        prKernParam->cipher_key, &pCipher);
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_ENGINE_CREATE_CIPHER_T));
    fgRet = TZ_SKB_CreateCipher(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->rRet;
    pCipher = prKernParam->cipher;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->cipher, sizeof(SKB_Cipher*));
    COPY_TO_USER_ARG_SIZE(&prUserParam->cipher, &pCipher, sizeof(SKB_Cipher*));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
    COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}

static BOOL _SKB_SecureData_Release(MTDMX_MWIF_SKB_SECUREDATA_T *prKernParam,
    MTDMX_MWIF_SKB_SECUREDATA_T *prUserParam)
{
    SKB_Result rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = SKB_SecureData_Release((SKB_SecureData *)prKernParam->self);
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_SECUREDATA_T));
    fgRet = TZ_SKB_SecureData_Release(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->rRet;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
    COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}

static BOOL _SKB_SecureDataGetInfo(MTDMX_MWIF_SKB_SECUREDATA_T *prKernParam,
    MTDMX_MWIF_SKB_SECUREDATA_T *prUserParam)
{
    SKB_DataInfo rDataInfo;
    SKB_Result rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = SKB_SecureData_GetInfo(prKernParam->self, &rDataInfo);
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)&rDataInfo, sizeof(rDataInfo));
    prKernParam->info = (SKB_DataInfo*)PHYSICAL((UINT32)&rDataInfo);
    HalFlushInvalidateDCacheMultipleLine((UINT32)&rDataInfo, sizeof(rDataInfo));
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_SECUREDATA_T));
    fgRet = TZ_SKB_SecureDataGetInfo(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->rRet;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->info, sizeof(SKB_DataInfo));
    COPY_TO_USER_ARG_SIZE(prUserParam->info, &rDataInfo, sizeof(SKB_DataInfo));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
    COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _SKB_SecureData_Export(MTDMX_MWIF_SKB_SECUREDATA_EXPORT_T *prKernParam,
    MTDMX_MWIF_SKB_SECUREDATA_EXPORT_T *prUserParam)
{
    SKB_Size rSize;
    SKB_Byte *pByte;
    SKB_Result rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....%d\n", __FUNCTION__, *(SKB_Size *)prKernParam->buffer_size);

    if(prKernParam->buffer == NULL)
    {
        // get size first
        #ifndef CC_TRUSTZONE_SUPPORT
        rRet = SKB_SecureData_Export(prKernParam->self, prKernParam->target,
            prKernParam->target_parameters, NULL, &rSize);
        #else
        prKernParam->buffer_size = (SKB_Size*)PHYSICAL((UINT32)&rSize);
        HalFlushInvalidateDCacheMultipleLine((UINT32)&rSize, sizeof(rSize));
        HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_SECUREDATA_EXPORT_T));
        fgRet = TZ_SKB_SecureData_Export(prKernParam);
        if(!fgRet)
        {
            PRINTK("TZ-%s.....\n", __FUNCTION__);
        }
        rRet = prKernParam->rRet;
        #endif

        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->buffer_size, sizeof(rSize));
        COPY_TO_USER_ARG_SIZE(prUserParam->buffer_size, &rSize, sizeof(rSize));
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
        COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

        PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

        return TRUE;
    }

/*
    // check if buffer size is enough
    if(rSize > *(SKB_Size *)prKernParam->buffer_size)
    {
        return FALSE;
    }
*/
    pByte = x_mem_alloc(*(SKB_Size *)prKernParam->buffer_size);
    if(pByte == NULL)
    {
        return FALSE;
    }

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = SKB_SecureData_Export(prKernParam->self, prKernParam->target,
        prKernParam->target_parameters, pByte, &rSize);
    #else
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->buffer_size, sizeof(SKB_Size));
    COPY_FROM_USER_ARG_SIZE(prUserParam->buffer_size, &rSize, sizeof(SKB_Size));
    prKernParam->buffer = (SKB_Byte*)PHYSICAL((UINT32)pByte);
    prKernParam->buffer_size = (SKB_Size*)PHYSICAL((UINT32)&rSize);
    HalFlushInvalidateDCacheMultipleLine((UINT32)&rSize, sizeof(rSize));
    HalFlushInvalidateDCacheMultipleLine((UINT32)pByte, rSize);
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_SECUREDATA_EXPORT_T));
    fgRet = TZ_SKB_SecureData_Export(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->rRet;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->buffer, rSize);
    COPY_TO_USER_ARG_SIZE(prUserParam->buffer, pByte, rSize);
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->buffer_size, sizeof(rSize));
    COPY_TO_USER_ARG_SIZE(prUserParam->buffer_size, &rSize, sizeof(rSize));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
    COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

    x_mem_free(pByte);

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _SKB_SecureData_Derive(MTDMX_MWIF_SKB_SECUREDATA_DERIVE_T *prKernParam,
    MTDMX_MWIF_SKB_SECUREDATA_DERIVE_T *prUserParam)
{
    SKB_SecureData* data;
    SKB_Result rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    SKB_SliceDerivationParameters rParameters;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = SKB_SecureData_Derive(prKernParam->self,
        prKernParam->algorithm, prKernParam->parameters, &data);
    #else
    // parameter is a data from user space, we need to copy it to kernel space. its size is 8.
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prKernParam->parameters, sizeof(SKB_SliceDerivationParameters));
    COPY_FROM_USER_ARG_SIZE(prKernParam->parameters, &rParameters, sizeof(SKB_SliceDerivationParameters));
    prKernParam->parameters = (const void*)PHYSICAL((UINT32)&rParameters);
    HalFlushInvalidateDCacheMultipleLine((UINT32)&rParameters, sizeof(SKB_SliceDerivationParameters));
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_SECUREDATA_DERIVE_T));
    fgRet = TZ_SKB_SecureData_Derive(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->rRet;
    data = prKernParam->data;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->data, sizeof(SKB_SecureData *));
    COPY_TO_USER_ARG_SIZE(&prUserParam->data, &data, sizeof(SKB_SecureData *));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
    COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}

static BOOL _SKB_Transform_Release(MTDMX_MWIF_SKB_TRANSFORM_ADDBYTES_T *prKernParam,
    MTDMX_MWIF_SKB_TRANSFORM_ADDBYTES_T *prUserParam)
{
    SKB_Result rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = SKB_Transform_Release(prKernParam->self);
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_TRANSFORM_ADDBYTES_T));
    fgRet = TZ_SKB_Transform_Release(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->rRet;
    #endif
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
    COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}

static BOOL _SKB_Transform_AddBytes(MTDMX_MWIF_SKB_TRANSFORM_ADDBYTES_T *prKernParam,
    MTDMX_MWIF_SKB_TRANSFORM_ADDBYTES_T *prUserParam)
{
    SKB_Byte *data;
    SKB_Result rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....%d\n", __FUNCTION__, (UINT32)prKernParam->data_size);

    data = x_mem_alloc((UINT32)prKernParam->data_size);
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prKernParam->data, prKernParam->data_size);
    COPY_FROM_USER_ARG_SIZE(prKernParam->data, data, prKernParam->data_size);
    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = SKB_Transform_AddBytes(prKernParam->self, data, prKernParam->data_size);
    #else
    prKernParam->data = (const SKB_Byte*)PHYSICAL((UINT32)data);
    HalFlushInvalidateDCacheMultipleLine((UINT32)data, (UINT32)prKernParam->data_size);
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_TRANSFORM_ADDBYTES_T));
    fgRet = TZ_SKB_Transform_AddBytes(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->rRet;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
    COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

    x_mem_free(data);

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _SKB_Transform_AddSecureData(MTDMX_MWIF_SKB_TRANSFORM_ADDSECUREDATA_T *prKernParam,
    MTDMX_MWIF_SKB_TRANSFORM_ADDSECUREDATA_T *prUserParam)
{
    SKB_Result rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = SKB_Transform_AddSecureData(prKernParam->self, prKernParam->data);
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_TRANSFORM_ADDSECUREDATA_T));
    fgRet = TZ_SKB_Transform_AddSecureData(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->rRet;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
    COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}

static BOOL _SKB_Transform_GetOutput(MTDMX_MWIF_SKB_TRANSFORM_GETOUTPUT_T *prKernParam,
    MTDMX_MWIF_SKB_TRANSFORM_GETOUTPUT_T *prUserParam)
{
    SKB_Size rSize;
    SKB_Byte* data;
    SKB_Result rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    // if buffer is NULL, get size.
    if(prKernParam->output == NULL)
    {
        #ifndef CC_TRUSTZONE_SUPPORT
        rRet = SKB_Transform_GetOutput(prKernParam->self, NULL, &rSize);
        #else
        prKernParam->output_size = (SKB_Size*)PHYSICAL((UINT32)&rSize);
        HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_TRANSFORM_GETOUTPUT_T));
        fgRet = TZ_SKB_Transform_GetOutput(prKernParam);
        if(!fgRet)
        {
            PRINTK("TZ-%s.....\n", __FUNCTION__);
        }
        rRet = prKernParam->rRet;
        #endif

        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->output_size, sizeof(SKB_Size));
        COPY_TO_USER_ARG_SIZE(prUserParam->output_size, &rSize, sizeof(SKB_Size));
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
        COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

        PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

        return TRUE;
    }
    else
    {
        #ifndef CC_TRUSTZONE_SUPPORT
        rRet = SKB_Transform_GetOutput(prKernParam->self, NULL, &rSize);
        #else
        prKernParam->output_size = (SKB_Size*)PHYSICAL((UINT32)&rSize);
        prKernParam->output = NULL;
        HalFlushInvalidateDCacheMultipleLine((UINT32)&rSize, sizeof(rSize));
        HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_TRANSFORM_GETOUTPUT_T));
        fgRet = TZ_SKB_Transform_GetOutput(prKernParam);
        if(!fgRet)
        {
            PRINTK("TZ-%s.....\n", __FUNCTION__);
        }
        rRet = prKernParam->rRet;
        #endif

        //if(rRet != SKB_SUCCESS || *(SKB_Size *)prKernParam->output_size < rSize)
        //{
        //    return FALSE;
        //}

        data = x_mem_alloc((UINT32)rSize);

        #ifndef CC_TRUSTZONE_SUPPORT
        rRet = SKB_Transform_GetOutput(prKernParam->self, data, &rSize);
        #else
        prKernParam->output = (SKB_Byte*)PHYSICAL((UINT32)data);
        HalFlushInvalidateDCacheMultipleLine((UINT32)data, rSize);
        HalFlushInvalidateDCacheMultipleLine((UINT32)&rSize, sizeof(rSize));
        HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_TRANSFORM_GETOUTPUT_T));
        fgRet = TZ_SKB_Transform_GetOutput(prKernParam);
        if(!fgRet)
        {
            PRINTK("TZ-%s.....\n", __FUNCTION__);
        }
        rRet = prKernParam->rRet;
        #endif

        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->output, rSize);
        COPY_TO_USER_ARG_SIZE(prUserParam->output, data, rSize);
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
        COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

        x_mem_free(data);

        PRINTK("-%s.....%d %d\n", __FUNCTION__, rRet, rSize);

        return TRUE;
    }
}


static BOOL _SKB_Cipher_ProcessBuffer(MTDMX_MWIF_SKB_CIPHER_PROCESSBUFFER_T *prKernParam,
    MTDMX_MWIF_SKB_CIPHER_PROCESSBUFFER_T *prUserParam)
{
    SKB_Byte* in_buffer = NULL;
    SKB_Byte* out_buffer = NULL;
    SKB_Byte* iv = NULL;
    SKB_Result rRet;
    SKB_Size rSize;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s..... in: %d out: %d\n", __FUNCTION__, prKernParam->in_buffer_size, *(SKB_Size *)prKernParam->out_buffer_size);

    // Assume in buffer is equal to out buffer
    if(prKernParam->out_buffer == NULL)
    {
        #ifndef CC_TRUSTZONE_SUPPORT
        rRet = SKB_Cipher_ProcessBuffer(prKernParam->self, in_buffer, prKernParam->in_buffer_size,
        NULL, &rSize, iv, prKernParam->iv_size);
        #else
        prKernParam->out_buffer_size = (SKB_Size*)PHYSICAL((UINT32)&rSize);
        HalFlushInvalidateDCacheMultipleLine((UINT32)&rSize, sizeof(rSize));
        HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_CIPHER_PROCESSBUFFER_T));
        fgRet = TZ_SKB_Cipher_ProcessBuffer(prKernParam);
        if(!fgRet)
        {
            PRINTK("TZ-%s.....\n", __FUNCTION__);
        }
        rRet = prKernParam->rRet;
        #endif

        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->out_buffer_size, sizeof(SKB_Size));
        COPY_TO_USER_ARG_SIZE(prUserParam->out_buffer_size, &rSize, sizeof(SKB_Size));
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
        COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

        PRINTK("-%s.....\n", __FUNCTION__);

        return TRUE;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->out_buffer_size, sizeof(SKB_Size));
    COPY_FROM_USER_ARG_SIZE(prUserParam->out_buffer_size, &rSize, sizeof(SKB_Size));
    in_buffer = x_mem_alloc((UINT32)prKernParam->in_buffer_size);
    out_buffer = x_mem_alloc(*(SKB_Size *)prKernParam->out_buffer_size);
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prKernParam->in_buffer, prKernParam->in_buffer_size);
    COPY_FROM_USER_ARG_SIZE(prKernParam->in_buffer, in_buffer, prKernParam->in_buffer_size);

    if(prKernParam->iv_size)
    {
        iv = x_mem_alloc((UINT32)prKernParam->iv_size);
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prKernParam->iv, prKernParam->iv_size);
        COPY_FROM_USER_ARG_SIZE(prKernParam->iv, iv, prKernParam->iv_size);
    }
    else
    {
        iv = NULL;
    }

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = SKB_Cipher_ProcessBuffer(prKernParam->self, in_buffer, prKernParam->in_buffer_size,
        out_buffer, &rSize, iv, prKernParam->iv_size);
    #else
    prKernParam->in_buffer = (const SKB_Byte*)PHYSICAL((UINT32)in_buffer);
    prKernParam->out_buffer = (SKB_Byte*)PHYSICAL((UINT32)out_buffer);
    prKernParam->iv = (const SKB_Byte*)PHYSICAL((UINT32)iv);
    prKernParam->out_buffer_size = (SKB_Size*)PHYSICAL((UINT32)&rSize);

    HalFlushInvalidateDCacheMultipleLine((UINT32)in_buffer, prKernParam->in_buffer_size);
    HalFlushInvalidateDCacheMultipleLine((UINT32)out_buffer, rSize);
    if(iv != NULL)
    {
        HalFlushInvalidateDCacheMultipleLine((UINT32)iv, prKernParam->iv_size);
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)&rSize, sizeof(rSize));
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_CIPHER_PROCESSBUFFER_T));
    fgRet = TZ_SKB_Cipher_ProcessBuffer(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->rRet;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->out_buffer, rSize);
    COPY_TO_USER_ARG_SIZE(prUserParam->out_buffer, out_buffer, rSize);
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->out_buffer_size, sizeof(SKB_Size));
    COPY_TO_USER_ARG_SIZE(prUserParam->out_buffer_size, &rSize, sizeof(SKB_Size));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
    COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

    x_mem_free(in_buffer);
    x_mem_free(out_buffer);
    if(prKernParam->iv_size)
    {
        x_mem_free(iv);
    }

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _SKB_Cipher_Release(MTDMX_MWIF_SKB_CIPHER_PROCESSBUFFER_T *prKernParam,
    MTDMX_MWIF_SKB_CIPHER_PROCESSBUFFER_T *prUserParam)
{
    SKB_Result rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = SKB_Cipher_Release(prKernParam->self);
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_SKB_CIPHER_PROCESSBUFFER_T));
    fgRet = TZ_SKB_Cipher_Release(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->rRet;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->rRet, sizeof(SKB_Result));
    COPY_TO_USER_ARG_SIZE(&prUserParam->rRet, &rRet, sizeof(SKB_Result));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_Init(MTDMX_MWIF_HCI_INIT_T *prKernParam,
    MTDMX_MWIF_HCI_INIT_T *prUserParam)
{
    retcode_t rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_Init();
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_HCI_INIT_T));
    fgRet = TZ_HCI_Init(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_Fin(MTDMX_MWIF_HCI_INIT_T *prKernParam,
    MTDMX_MWIF_HCI_INIT_T *prUserParam)
{
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    hci_Fin();
    #else
    fgRet = TZ_HCI_Fin();
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    #endif

    PRINTK("-%s.....\n", __FUNCTION__);

    return TRUE;
}


static BOOL _HCI_KeyRefCreate(MTDMX_MWIF_HCI_KEY_REF_T *prKernParam,
    MTDMX_MWIF_HCI_KEY_REF_T *prUserParam)
{
    retcode_t rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....0x%x\n", __FUNCTION__, (UINT32)prKernParam->pKeyRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_KeyRefCreate((hci_key_type_t)prKernParam->u4KeyType,
        (hci_key_ref_t *)&prKernParam->pKeyRef);
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_HCI_KEY_REF_T));
    fgRet = TZ_HCI_KeyRefCreate(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->pKeyRef, sizeof(hci_key_ref_t *));
    COPY_TO_USER_ARG_SIZE(&prUserParam->pKeyRef, &prKernParam->pKeyRef, sizeof(hci_key_ref_t *));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d 0x%x\n", __FUNCTION__, rRet, (UINT32)prKernParam->pKeyRef);

    return TRUE;
}


static BOOL _HCI_KeyRefCheck(MTDMX_MWIF_HCI_KEY_REF_T *prKernParam,
    MTDMX_MWIF_HCI_KEY_REF_T *prUserParam)
{
    retcode_t rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....%d\n", __FUNCTION__, (UINT32)prKernParam->pKeyRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_KeyRefCheck((hci_key_ref_t)prKernParam->pKeyRef);
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_HCI_KEY_REF_T));
    fgRet = TZ_HCI_KeyRefCheck(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d 0x%x\n", __FUNCTION__, rRet, (UINT32)prKernParam->pKeyRef);

    return TRUE;
}


static BOOL _HCI_KeyRefDestroy(MTDMX_MWIF_HCI_KEY_REF_T *prKernParam,
    MTDMX_MWIF_HCI_KEY_REF_T *prUserParam)
{
    BOOL rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....0x%x\n", __FUNCTION__, (UINT32)prKernParam->pKeyRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_KeyRefDestroy((hci_key_ref_t)prKernParam->pKeyRef);
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_HCI_KEY_REF_T));
    fgRet = TZ_HCI_KeyRefDestroy(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d 0x%x\n", __FUNCTION__, rRet, (UINT32)prKernParam->pKeyRef);

    return TRUE;
}


static BOOL _HCI_DevIdGetNum(MTDMX_MWIF_HCI_ID_T *prKernParam,
    MTDMX_MWIF_HCI_ID_T *prUserParam)
{
    retcode_t rRet;
    UINT32 u4Num;
    #ifndef CC_TRUSTZONE_SUPPORT
    binstr_t **out_ids;
    #else
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_DevIdsGet(&out_ids, &u4Num);
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_HCI_ID_T));
    fgRet = TZ_HCI_DevIdsGetNum(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    u4Num = prKernParam->u4IdNum;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4IdNum, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4IdNum, &u4Num, sizeof(UINT32));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_DevIdGetSize(MTDMX_MWIF_HCI_ID_T *prKernParam,
    MTDMX_MWIF_HCI_ID_T *prUserParam)
{
    retcode_t rRet;
    UINT32 u4IdSize;
    #ifndef CC_TRUSTZONE_SUPPORT
    UINT32 u4Num;
    binstr_t **out_ids;    
    #else
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_DevIdsGet(&out_ids, &u4Num);
    u4IdSize = out_ids[prKernParam->u4IdIdx]->len;
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_HCI_ID_T));
    fgRet = TZ_HCI_DevIdsGetSize(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    u4IdSize = prKernParam->u4IdSize;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4IdSize, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4IdSize, &u4IdSize, sizeof(UINT32));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_DevIdGetName(MTDMX_MWIF_HCI_ID_T *prKernParam,
    MTDMX_MWIF_HCI_ID_T *prUserParam)
{
    retcode_t rRet;
    UINT32 u4IdSize;
    #ifndef CC_TRUSTZONE_SUPPORT
    UINT32 u4Num;
    binstr_t **out_ids;    
    #else
    BOOL fgRet;
    binstr_t *pSubBinstr;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_DevIdsGet(&out_ids, &u4Num);
    u4IdSize = out_ids[prKernParam->u4IdIdx]->len;
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->pBuf, u4IdSize);
    COPY_TO_USER_ARG_SIZE(prUserParam->pBuf, out_ids[prKernParam->u4IdIdx]->buf, u4IdSize);
    #else
    pSubBinstr = hci_BinstrAlloc(prKernParam->u4IdSize, NULL);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pSubBinstr->buf, prKernParam->u4IdSize);
    u4IdSize = prKernParam->u4IdSize;
    prKernParam->pBuf = (void*)PHYSICAL((UINT32)pSubBinstr->buf);
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_HCI_ID_T));
    fgRet = TZ_HCI_DevIdsGetName(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->pBuf, u4IdSize);
    COPY_TO_USER_ARG_SIZE(prUserParam->pBuf, (void *)VIRTUAL((UINT32)prKernParam->pBuf), u4IdSize);
    hci_BinstrFree(pSubBinstr);
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_ImportKey(MTDMX_MWIF_HCI_IMPORT_KEY_T *prKernParam,
    MTDMX_MWIF_HCI_IMPORT_KEY_T *prUserParam)
{
    retcode_t rRet;
    hci_key_ref_t rKeyRef;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    binstr_t *pOAEPParam = NULL;
    binstr_t *pKeyData = NULL;
    #endif

    PRINTK("+%s.....0x%x\n", __FUNCTION__, prKernParam->pKeyInRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_ImportKey(prKernParam->pKeyInRef, prKernParam->u4EncAlg, prKernParam->pOAEPParam,
        prKernParam->pKeyData, prKernParam->u4KeyType, &rKeyRef);
    #else
    if(prKernParam->pOAEPParam != NULL)
    {
        pOAEPParam = hci_BinstrAlloc(((binstr_t *)prKernParam->pOAEPParam)->len,
            ((binstr_t *)prKernParam->pOAEPParam)->buf);
        pOAEPParam->buf = (u_int8_t*)PHYSICAL((UINT32)pOAEPParam->buf);
        prKernParam->pOAEPParam = (void*)PHYSICAL((UINT32)pOAEPParam);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pOAEPParam,
            pOAEPParam->len + sizeof(binstr_t));
    }
    if(prKernParam->pKeyData != NULL)
    {
        pKeyData = hci_BinstrAlloc(((binstr_t *)prKernParam->pKeyData)->len,
            ((binstr_t *)prKernParam->pKeyData)->buf);
        pKeyData->buf = (u_int8_t*)PHYSICAL((UINT32)pKeyData->buf);
        prKernParam->pKeyData = (void*)PHYSICAL((UINT32)pKeyData);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pKeyData,
            pKeyData->len + sizeof(binstr_t));
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam, sizeof(MTDMX_MWIF_HCI_IMPORT_KEY_T));
    fgRet = TZ_HCI_ImportKey(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    rKeyRef = prKernParam->pKeyOutRef;
    if(pOAEPParam != NULL)
    {
        hci_BinstrFree(pOAEPParam);
    }
    if(pKeyData != NULL)
    {
        hci_BinstrFree(pKeyData);
    }
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->pKeyOutRef, sizeof(hci_key_ref_t));
    COPY_TO_USER_ARG_SIZE(&prUserParam->pKeyOutRef, &rKeyRef, sizeof(hci_key_ref_t));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d 0x%x\n", __FUNCTION__, rRet, prUserParam->pKeyOutRef);

    return TRUE;
}


static BOOL _HCI_ExportKeySize(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T *prKernParam,
    MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T *prUserParam)
{
    retcode_t rRet;
    UINT32 u4IdSize;
    #ifndef CC_TRUSTZONE_SUPPORT    
    binstr_t *out_data = NULL;    
    #else
    BOOL fgRet;
    #endif

    PRINTK("+%s.....0x%x\n", __FUNCTION__, prKernParam->pKeyRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_ExportKey(prKernParam->pKeyRef, &out_data);
    if(rRet == ERR_OK && out_data !=NULL)
    {
        u4IdSize = out_data->len;
    }
    else
    {
        u4IdSize = 0;
    }
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T));
    fgRet = TZ_HCI_ExportKeySize(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    u4IdSize = prKernParam->u4IdSize;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4IdSize, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4IdSize, &u4IdSize, sizeof(UINT32));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_ExportKey(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T *prKernParam,
    MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T *prUserParam)
{
    retcode_t rRet;
    binstr_t *out_data = NULL;
    UINT32 u4IdSize;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....0x%x\n", __FUNCTION__, prKernParam->pKeyRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_ExportKey(prKernParam->pKeyRef, &out_data);
    #else
    out_data = hci_BinstrAlloc(prKernParam->u4IdSize, NULL);
    prKernParam->pBuf = (void*)PHYSICAL((UINT32)out_data->buf);
    HalFlushInvalidateDCacheMultipleLine((UINT32)out_data->buf,
        prKernParam->u4IdSize);
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T));
    fgRet = TZ_HCI_ExportKey(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    #endif

    if(rRet == ERR_OK && out_data !=NULL)
    {
        u4IdSize = out_data->len;
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->pBuf, u4IdSize);
        COPY_TO_USER_ARG_SIZE(prUserParam->pBuf, out_data->buf, u4IdSize);
        #ifdef CC_TRUSTZONE_SUPPORT
        hci_BinstrFree(out_data);
        #endif
    }

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_StarfishIdGetSize(MTDMX_MWIF_HCI_ID_T *prKernParam,
    MTDMX_MWIF_HCI_ID_T *prUserParam)
{
    retcode_t rRet;
    UINT32 u4IdSize;
    #ifndef CC_TRUSTZONE_SUPPORT    
    binstr_t *out_id;    
    #else
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_StarfishIdGet(&out_id);
    u4IdSize = out_id->len;
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_ID_T));
    fgRet = TZ_HCI_StarfishIdGetSize(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    u4IdSize = prKernParam->u4IdSize;
    rRet = prKernParam->u4Ret;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4IdSize, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4IdSize, &u4IdSize, sizeof(UINT32));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_StarfishIdGet(MTDMX_MWIF_HCI_ID_T *prKernParam,
    MTDMX_MWIF_HCI_ID_T *prUserParam)
{
    retcode_t rRet;
    binstr_t *out_id;
    UINT32 u4IdSize;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_StarfishIdGet(&out_id);
    u4IdSize = out_id->len;
    #else
    out_id = hci_BinstrAlloc(prKernParam->u4IdSize, NULL);
    prKernParam->pBuf = (void*)PHYSICAL((UINT32)out_id->buf);
    HalFlushInvalidateDCacheMultipleLine((UINT32)out_id->buf,
        prKernParam->u4IdSize);
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_ID_T));
    TZ_HCI_StarfishIdGet(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    u4IdSize = prKernParam->u4IdSize;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->pBuf, u4IdSize);
    COPY_TO_USER_ARG_SIZE(prUserParam->pBuf, out_id->buf, u4IdSize);
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));
    #ifdef CC_TRUSTZONE_SUPPORT
    if(out_id != NULL)
    {
        hci_BinstrFree(out_id);
    }
    #endif
    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_KeyDataGetIfStarfishSize(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T *prKernParam,
    MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T *prUserParam)
{
    retcode_t rRet;

    UINT32 u4IdSize;
    #ifndef CC_TRUSTZONE_SUPPORT    
    binstr_t *out_data = NULL;    
    #else
    BOOL fgRet;
    #endif

    PRINTK("+%s.....0x%x\n", __FUNCTION__, prKernParam->pKeyRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_KeyDataGetIfStarfish(prKernParam->pKeyRef, &out_data);
    if(rRet == ERR_OK && out_data !=NULL)
    {
        u4IdSize = out_data->len;
    }
    else
    {
        u4IdSize = 0;
    }
    #else
    PRINTK("%s.....Addr: 0x%x\n", __FUNCTION__, prKernParam);
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T));
    fgRet = TZ_HCI_KeyDataGetIfStarfishSize(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    u4IdSize = prKernParam->u4IdSize;
    rRet = prKernParam->u4Ret;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4IdSize, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4IdSize, &u4IdSize, sizeof(UINT32));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_KeyDataGetIfStarfish(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T *prKernParam,
    MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T *prUserParam)
{
    retcode_t rRet;
    binstr_t *out_data = NULL;
    UINT32 u4IdSize;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....0x%x\n", __FUNCTION__, prKernParam->pKeyRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_KeyDataGetIfStarfish(prKernParam->pKeyRef, &out_data);
    #else
    out_data = hci_BinstrAlloc(prKernParam->u4IdSize, NULL);
    prKernParam->pBuf = (void*)PHYSICAL((UINT32)out_data->buf);
    HalFlushInvalidateDCacheMultipleLine((UINT32)out_data->buf, prKernParam->u4IdSize);
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T));
    TZ_HCI_KeyDataGetIfStarfish(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    #endif

    if(rRet == ERR_OK && out_data !=NULL)
    {
        u4IdSize = out_data->len;
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->pBuf, u4IdSize);
        COPY_TO_USER_ARG_SIZE(prUserParam->pBuf, out_data->buf, u4IdSize);
        #ifdef CC_TRUSTZONE_SUPPORT
        hci_BinstrFree(out_data);
        #endif
    }
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_ImportStarfishKey(MTDMX_MWIF_HCI_IMPORT_STARFISH_KEY_T *prKernParam,
    MTDMX_MWIF_HCI_IMPORT_STARFISH_KEY_T *prUserParam)
{
    retcode_t rRet;
    hci_key_ref_t rKeyRef;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    binstr_t *pEncBK = NULL;
    binstr_t *pBKHash = NULL;
    binstr_t *pIv = NULL;
    binstr_t *pEncCK = NULL;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_ImportStarfishKey(prKernParam->u4IterationNum, prKernParam->u4KeyIndex,
        prKernParam->pEncBK, prKernParam->pBKHash, prKernParam->pIv, prKernParam->pEncCK,
        &rKeyRef);
    #else
    if(prKernParam->pEncBK != NULL)
    {
        pEncBK = hci_BinstrAlloc(((binstr_t *)prKernParam->pEncBK)->len,
            ((binstr_t *)prKernParam->pEncBK)->buf);
        pEncBK->buf = (u_int8_t*)PHYSICAL((UINT32)pEncBK->buf);
        prKernParam->pEncBK = (void*)PHYSICAL((UINT32)pEncBK);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pEncBK,
            pEncBK->len + sizeof(binstr_t));
    }
    if(prKernParam->pBKHash != NULL)
    {
        pBKHash = hci_BinstrAlloc(((binstr_t *)prKernParam->pBKHash)->len,
            ((binstr_t *)prKernParam->pBKHash)->buf);
        pBKHash->buf = (u_int8_t*)PHYSICAL((UINT32)pBKHash->buf);
        prKernParam->pBKHash = (void*)PHYSICAL((UINT32)pBKHash);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pBKHash,
            pBKHash->len + sizeof(binstr_t));
    }
    if(prKernParam->pIv != NULL)
    {
        pIv = hci_BinstrAlloc(((binstr_t *)prKernParam->pIv)->len,
            ((binstr_t *)prKernParam->pIv)->buf);
        pIv->buf = (u_int8_t*)PHYSICAL((UINT32)pIv->buf);
        prKernParam->pIv = (void*)PHYSICAL((UINT32)pIv);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pIv,
            pIv->len + sizeof(binstr_t));
    }
    if(prKernParam->pEncCK != NULL)
    {
        pEncCK = hci_BinstrAlloc(((binstr_t *)prKernParam->pEncCK)->len,
            ((binstr_t *)prKernParam->pEncCK)->buf);
        pEncCK->buf = (u_int8_t*)PHYSICAL((UINT32)pEncCK->buf);
        prKernParam->pEncCK = (void*)PHYSICAL((UINT32)pEncCK);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pEncCK,
            pEncCK->len + sizeof(binstr_t));
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_IMPORT_STARFISH_KEY_T));
    fgRet = TZ_HCI_ImportStarfishKey(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    rKeyRef = prKernParam->pKeyOutRef;
    if(pEncBK != NULL)
    {
        hci_BinstrFree(pEncBK);
    }
    if(pBKHash != NULL)
    {
        hci_BinstrFree(pBKHash);
    }
    if(pIv != NULL)
    {
        hci_BinstrFree(pIv);
    }
    if(pEncCK != NULL)
    {
        hci_BinstrFree(pEncCK);
    }
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->pKeyOutRef, sizeof(hci_key_ref_t));
    COPY_TO_USER_ARG_SIZE(&prUserParam->pKeyOutRef, &rKeyRef, sizeof(hci_key_ref_t));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d 0x%x\n", __FUNCTION__, rRet, rKeyRef);

    return TRUE;
}


static BOOL _HCI_DataSignSize(MTDMX_MWIF_HCI_DATA_OP_T *prKernParam,
    MTDMX_MWIF_HCI_DATA_OP_T *prUserParam)
{
    retcode_t rRet;
    UINT32 u4Size;
    #ifndef CC_TRUSTZONE_SUPPORT    
    binstr_t *out_data;    
    #else
    binstr_t *pInData = NULL;
    BOOL fgRet;
    #endif

    PRINTK("+%s.....0x%x\n", __FUNCTION__, prKernParam->pKeyInRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_DataSign(prKernParam->pKeyInRef, prKernParam->u4Alg,
        prKernParam->pInData, &out_data);
    u4Size = out_data->len;
    #else
    if(prKernParam->pInData != NULL)
    {
        pInData = hci_BinstrAlloc(((binstr_t *)prKernParam->pInData)->len,
            ((binstr_t *)prKernParam->pInData)->buf);
        pInData->buf = (u_int8_t*)PHYSICAL((UINT32)pInData->buf);
        prKernParam->pInData = (void*)PHYSICAL((UINT32)pInData);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInData,
            pInData->len + sizeof(binstr_t));
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_DATA_OP_T));
    fgRet = TZ_HCI_DataSignSize(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    u4Size = prKernParam->u4Size;
    if(pInData != NULL)
    {
        hci_BinstrFree(pInData);
    }
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Size, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Size, &u4Size, sizeof(UINT32));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_DataSign(MTDMX_MWIF_HCI_DATA_OP_T *prKernParam,
    MTDMX_MWIF_HCI_DATA_OP_T *prUserParam)
{
    retcode_t rRet;
    binstr_t *out_data;
    UINT32 u4Size;
    #ifdef CC_TRUSTZONE_SUPPORT
    binstr_t *pInData = NULL;
    BOOL fgRet;
    #endif

    PRINTK("+%s.....0x%x\n", __FUNCTION__, prKernParam->pKeyInRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_DataSign(prKernParam->pKeyInRef, prKernParam->u4Alg,
        prKernParam->pInData, &out_data);
    u4Size = out_data->len;
    #else
    if(prKernParam->pInData != NULL)
    {
        pInData = hci_BinstrAlloc(((binstr_t *)prKernParam->pInData)->len,
            ((binstr_t *)prKernParam->pInData)->buf);
        pInData->buf = (u_int8_t*)PHYSICAL((UINT32)pInData->buf);
        prKernParam->pInData = (void*)PHYSICAL((UINT32)pInData);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInData,
            pInData->len + sizeof(binstr_t));
    }
    out_data = hci_BinstrAlloc(prKernParam->u4Size, NULL);
    prKernParam->pOutData = (void*)PHYSICAL((UINT32)out_data->buf);
    HalFlushInvalidateDCacheMultipleLine((UINT32)out_data->buf,
        prKernParam->u4Size);
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_DATA_OP_T));
    fgRet = TZ_HCI_DataSign(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    u4Size = prKernParam->u4Size;
    if(pInData != NULL)
    {
        hci_BinstrFree(pInData);
    }
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->pOutData, u4Size);
    COPY_TO_USER_ARG_SIZE(prUserParam->pOutData, out_data->buf, u4Size);
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));
    #ifdef CC_TRUSTZONE_SUPPORT
    hci_BinstrFree(out_data);
    #endif
    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_DataDecryptSize(MTDMX_MWIF_HCI_DATA_OP_T *prKernParam,
    MTDMX_MWIF_HCI_DATA_OP_T *prUserParam)
{
    retcode_t rRet;
    UINT32 u4Size;
    #ifndef CC_TRUSTZONE_SUPPORT    
    binstr_t *out_data;    
    #else
    binstr_t *pInOAEPParam = NULL;
    binstr_t *pInData = NULL;
    BOOL fgRet;
    #endif

    PRINTK("+%s.....0x%x\n", __FUNCTION__, prKernParam->pKeyInRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_DataDecrypt(prKernParam->pKeyInRef, prKernParam->u4Alg, prKernParam->u4Pad,
        prKernParam->pInSaltBuf, prKernParam->pInOAEPParam, prKernParam->pInData, &out_data);
    if(rRet == ERR_OK)
    {
        u4Size = out_data->len;
    }
    else
    {
        u4Size = 0;
    }
    #else
    if(prKernParam->pInOAEPParam != NULL)
    {
        pInOAEPParam = hci_BinstrAlloc(((binstr_t *)prKernParam->pInOAEPParam)->len,
            ((binstr_t *)prKernParam->pInOAEPParam)->buf);
        pInOAEPParam->buf = (u_int8_t*)PHYSICAL((UINT32)pInOAEPParam->buf);
        prKernParam->pInOAEPParam = (void*)PHYSICAL((UINT32)pInOAEPParam);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInOAEPParam,
            pInOAEPParam->len + sizeof(binstr_t));
    }
    if(prKernParam->pInData != NULL)
    {
        pInData = hci_BinstrAlloc(((binstr_t *)prKernParam->pInData)->len,
            ((binstr_t *)prKernParam->pInData)->buf);
        pInData->buf = (u_int8_t*)PHYSICAL((UINT32)pInData->buf);
        prKernParam->pInData = (void*)PHYSICAL((UINT32)pInData);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInData,
            pInData->len + sizeof(binstr_t));
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_DATA_OP_T));
    fgRet = TZ_HCI_DataDecryptSize(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    u4Size = prKernParam->u4Size;
    if(pInOAEPParam != NULL)
    {
        hci_BinstrFree(pInOAEPParam);
    }
    if(pInData != NULL)
    {
        hci_BinstrFree(pInData);
    }
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Size, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Size, &u4Size, sizeof(UINT32));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_DataDecrypt(MTDMX_MWIF_HCI_DATA_OP_T *prKernParam,
    MTDMX_MWIF_HCI_DATA_OP_T *prUserParam)
{
    retcode_t rRet;
    binstr_t *out_data;
    UINT32 u4Size;
    #ifdef CC_TRUSTZONE_SUPPORT
    binstr_t *pInOAEPParam = NULL;
    binstr_t *pInData = NULL;
    BOOL fgRet;
    #endif

    PRINTK("+%s.....0x%x\n", __FUNCTION__, prKernParam->pKeyInRef);
    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_DataDecrypt(prKernParam->pKeyInRef, prKernParam->u4Alg, prKernParam->u4Pad,
        prKernParam->pInSaltBuf, prKernParam->pInOAEPParam, prKernParam->pInData, &out_data);
    if(rRet == ERR_OK)
    {
        u4Size = out_data->len;
    }
    else
    {
        u4Size = 0;
    }
    #else
    if(prKernParam->pInOAEPParam != NULL)
    {
        pInOAEPParam = hci_BinstrAlloc(((binstr_t *)prKernParam->pInOAEPParam)->len,
            ((binstr_t *)prKernParam->pInOAEPParam)->buf);
        pInOAEPParam->buf = (u_int8_t*)PHYSICAL((UINT32)pInOAEPParam->buf);
        prKernParam->pInOAEPParam = (void*)PHYSICAL((UINT32)pInOAEPParam);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInOAEPParam,
            pInOAEPParam->len + sizeof(binstr_t));
    }
    if(prKernParam->pInData != NULL)
    {
        pInData = hci_BinstrAlloc(((binstr_t *)prKernParam->pInData)->len,
            ((binstr_t *)prKernParam->pInData)->buf);
        pInData->buf = (u_int8_t*)PHYSICAL((UINT32)pInData->buf);
        prKernParam->pInData = (void*)PHYSICAL((UINT32)pInData);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInData,
            pInData->len + sizeof(binstr_t));
    }
    out_data = hci_BinstrAlloc(prKernParam->u4Size, NULL);
    prKernParam->pOutData = (void*)PHYSICAL((UINT32)out_data->buf);
    HalFlushInvalidateDCacheMultipleLine((UINT32)out_data->buf,
        prKernParam->u4Size);
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_DATA_OP_T));
    fgRet = TZ_HCI_DataDecrypt(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    u4Size = prKernParam->u4Size;
    if(pInOAEPParam != NULL)
    {
        hci_BinstrFree(pInOAEPParam);
    }
    if(pInData != NULL)
    {
        hci_BinstrFree(pInData);
    }
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->pOutData, u4Size);
    COPY_TO_USER_ARG_SIZE(prUserParam->pOutData, out_data->buf, u4Size);
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));
    #ifdef CC_TRUSTZONE_SUPPORT
    hci_BinstrFree(out_data);
    #endif

    PRINTK("-%s.....%d 0x%x\n", __FUNCTION__, rRet, prKernParam->pKeyInRef);

    return TRUE;
}


static BOOL _HCI_StreamDecryptSize(MTDMX_MWIF_HCI_DATA_OP_T *prKernParam,
    MTDMX_MWIF_HCI_DATA_OP_T *prUserParam)
{
    retcode_t rRet;
    UINT32 u4Size;
    #ifndef CC_TRUSTZONE_SUPPORT    
    binstr_t *out_data;    
    #else
    binstr_t *pInOAEPParam = NULL;
    binstr_t *pInData = NULL;
    BOOL fgRet;
    #endif

    PRINTK("+%s.....0x%x\n", __FUNCTION__, prKernParam->pKeyInRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_StreamDecrypt(prKernParam->pKeyInRef, prKernParam->u4Alg, prKernParam->u4Pad,
        prKernParam->pInSaltBuf, prKernParam->pInOAEPParam, prKernParam->pInData, &out_data);
    if(rRet == ERR_OK)
    {
        u4Size = out_data->len;
    }
    #else
    if(prKernParam->pInOAEPParam != NULL)
    {
        pInOAEPParam = hci_BinstrAlloc(((binstr_t *)prKernParam->pInOAEPParam)->len,
            ((binstr_t *)prKernParam->pInOAEPParam)->buf);
        pInOAEPParam->buf = (u_int8_t*)PHYSICAL((UINT32)pInOAEPParam->buf);
        prKernParam->pInOAEPParam = (void*)PHYSICAL((UINT32)pInOAEPParam);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInOAEPParam,
            pInOAEPParam->len + sizeof(binstr_t));
    }
    if(prKernParam->pInData != NULL)
    {
        pInData = hci_BinstrAlloc(((binstr_t *)prKernParam->pInData)->len,
            ((binstr_t *)prKernParam->pInData)->buf);
        pInData->buf = (u_int8_t*)PHYSICAL((UINT32)pInData->buf);
        prKernParam->pInData = (void*)PHYSICAL((UINT32)pInData);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInData,
            pInData->len + sizeof(binstr_t));
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_DATA_OP_T));
    fgRet = TZ_HCI_StreamDecryptSize(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    u4Size = prKernParam->u4Size;
    if(pInOAEPParam != NULL)
    {
        hci_BinstrFree(pInOAEPParam);
    }
    if(pInData != NULL)
    {
        hci_BinstrFree(pInData);
    }
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Size, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Size, &u4Size, sizeof(UINT32));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_StreamDecrypt(MTDMX_MWIF_HCI_DATA_OP_T *prKernParam,
    MTDMX_MWIF_HCI_DATA_OP_T *prUserParam)
{
    retcode_t rRet;
    binstr_t *out_data;
    UINT32 u4Size;
    #ifdef CC_TRUSTZONE_SUPPORT
    binstr_t *pInOAEPParam = NULL;
    binstr_t *pInData = NULL;
    BOOL fgRet;
    #endif

    PRINTK("+%s.....0x%x\n", __FUNCTION__, prKernParam->pKeyInRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_StreamDecrypt(prKernParam->pKeyInRef, prKernParam->u4Alg, prKernParam->u4Pad,
        prKernParam->pInSaltBuf, prKernParam->pInOAEPParam, prKernParam->pInData, &out_data);
    if(rRet == ERR_OK)
    {
        u4Size = out_data->len;
    }
    else
    {
        u4Size = 0;
    }
    #else
    if(prKernParam->pInOAEPParam != NULL)
    {
        pInOAEPParam = hci_BinstrAlloc(((binstr_t *)prKernParam->pInOAEPParam)->len,
            ((binstr_t *)prKernParam->pInOAEPParam)->buf);
        pInOAEPParam->buf = (u_int8_t*)PHYSICAL((UINT32)pInOAEPParam->buf);
        prKernParam->pInOAEPParam = (void*)PHYSICAL((UINT32)pInOAEPParam);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInOAEPParam,
            pInOAEPParam->len + sizeof(binstr_t));
    }
    if(prKernParam->pInData != NULL)
    {
        pInData = hci_BinstrAlloc(((binstr_t *)prKernParam->pInData)->len,
            ((binstr_t *)prKernParam->pInData)->buf);
        pInData->buf = (u_int8_t*)PHYSICAL((UINT32)pInData->buf);
        prKernParam->pInData = (void*)PHYSICAL((UINT32)pInData);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInData,
            pInData->len + sizeof(binstr_t));
    }
    out_data = hci_BinstrAlloc(prKernParam->u4Size, NULL);
    prKernParam->pOutData = (void*)PHYSICAL((UINT32)out_data->buf);
    HalFlushInvalidateDCacheMultipleLine((UINT32)out_data->buf,
        prKernParam->u4Size);
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_DATA_OP_T));
    fgRet = TZ_HCI_StreamDecrypt(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    u4Size = prKernParam->u4Size;
    if(pInOAEPParam != NULL)
    {
        hci_BinstrFree(pInOAEPParam);
    }
    if(pInData != NULL)
    {
        hci_BinstrFree(pInData);
    }
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->pOutData, u4Size);
    COPY_TO_USER_ARG_SIZE(prUserParam->pOutData, out_data->buf, u4Size);
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));
    #ifdef CC_TRUSTZONE_SUPPORT
    hci_BinstrFree(out_data);
    #endif

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_KeyDataEncryptSize(MTDMX_MWIF_HCI_DATA_OP_T *prKernParam,
    MTDMX_MWIF_HCI_DATA_OP_T *prUserParam)
{
    retcode_t rRet;
    UINT32 u4Size;
    #ifndef CC_TRUSTZONE_SUPPORT    
    binstr_t *out_data;    
    #else
    binstr_t *pInOAEPParam = NULL;
    binstr_t *pInData = NULL;
    BOOL fgRet;
    #endif

    PRINTK("+%s.....0x%x\n", __FUNCTION__, prKernParam->pKeyInRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_KeyDataEncrypt(prKernParam->pKeyInRef, prKernParam->pInOAEPParam,
        prKernParam->pInData, &out_data);
    if(rRet == ERR_OK)
    {
        u4Size = out_data->len;
    }
    #else
    if(prKernParam->pInOAEPParam != NULL)
    {
        pInOAEPParam = hci_BinstrAlloc(((binstr_t *)prKernParam->pInOAEPParam)->len,
            ((binstr_t *)prKernParam->pInOAEPParam)->buf);
        pInOAEPParam->buf = (u_int8_t*)PHYSICAL((UINT32)pInOAEPParam->buf);
        prKernParam->pInOAEPParam = (void*)PHYSICAL((UINT32)pInOAEPParam);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInOAEPParam,
            pInOAEPParam->len + sizeof(binstr_t));
    }
    if(prKernParam->pInData != NULL)
    {
        pInData = hci_BinstrAlloc(((binstr_t *)prKernParam->pInData)->len,
            ((binstr_t *)prKernParam->pInData)->buf);
        pInData->buf = (u_int8_t*)PHYSICAL((UINT32)pInData->buf);
        prKernParam->pInData = (void*)PHYSICAL((UINT32)pInData);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInData,
            pInData->len + sizeof(binstr_t));
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_DATA_OP_T));
    fgRet = TZ_HCI_KeyDataEncryptSize(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    u4Size = prKernParam->u4Size;
    if(pInOAEPParam != NULL)
    {
        hci_BinstrFree(pInOAEPParam);
    }
    if(pInData != NULL)
    {
        hci_BinstrFree(pInData);
    }
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Size, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Size, &u4Size, sizeof(UINT32));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_KeyDataEncrypt(MTDMX_MWIF_HCI_DATA_OP_T *prKernParam,
    MTDMX_MWIF_HCI_DATA_OP_T *prUserParam)
{
    retcode_t rRet;
    binstr_t *out_data;
    UINT32 u4Size;
    #ifdef CC_TRUSTZONE_SUPPORT
    binstr_t *pInOAEPParam = NULL;
    binstr_t *pInData = NULL;
    BOOL fgRet;
    #endif

    PRINTK("+%s.....0x%x\n", __FUNCTION__, prKernParam->pKeyInRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_KeyDataEncrypt(prKernParam->pKeyInRef, prKernParam->pInOAEPParam,
        prKernParam->pInData, &out_data);
    if(rRet == ERR_OK)
    {
        u4Size = out_data->len;
    }
    #else
    if(prKernParam->pInOAEPParam != NULL)
    {
        pInOAEPParam = hci_BinstrAlloc(((binstr_t *)prKernParam->pInOAEPParam)->len,
            ((binstr_t *)prKernParam->pInOAEPParam)->buf);
        pInOAEPParam->buf = (u_int8_t*)PHYSICAL((UINT32)pInOAEPParam->buf);
        prKernParam->pInOAEPParam = (void*)PHYSICAL((UINT32)pInOAEPParam);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInOAEPParam,
            pInOAEPParam->len + sizeof(binstr_t));
    }
    if(prKernParam->pInData != NULL)
    {
        pInData = hci_BinstrAlloc(((binstr_t *)prKernParam->pInData)->len,
            ((binstr_t *)prKernParam->pInData)->buf);
        pInData->buf = (u_int8_t*)PHYSICAL((UINT32)pInData->buf);
        prKernParam->pInData = (void*)PHYSICAL((UINT32)pInData);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInData,
            pInData->len + sizeof(binstr_t));
    }
    out_data = hci_BinstrAlloc(prKernParam->u4Size, NULL);
    prKernParam->pOutData = (void*)PHYSICAL((UINT32)out_data->buf);
    HalFlushInvalidateDCacheMultipleLine((UINT32)out_data->buf,
        prKernParam->u4Size);
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_DATA_OP_T));
    fgRet = TZ_HCI_KeyDataEncrypt(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    u4Size = prKernParam->u4Size;
    if(pInOAEPParam != NULL)
    {
        hci_BinstrFree(pInOAEPParam);
    }
    if(pInData != NULL)
    {
        hci_BinstrFree(pInData);
    }
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->pOutData, u4Size);
    COPY_TO_USER_ARG_SIZE(prUserParam->pOutData, out_data->buf, u4Size);
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));
    #ifdef CC_TRUSTZONE_SUPPORT
    hci_BinstrFree(out_data);
    #endif

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_HmacCheck(MTDMX_MWIF_HCI_HASH_T *prKernParam,
    MTDMX_MWIF_HCI_HASH_T *prUserParam)
{
    retcode_t rRet;
    UINT32 u4Result;
    #ifdef CC_TRUSTZONE_SUPPORT
    binstr_t *pInData = NULL;
    binstr_t *pInExpected = NULL;
    BOOL fgRet;
    #endif

    PRINTK("+%s.....0x%x\n", __FUNCTION__, prKernParam->pKeyInRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_HmacCheck(prKernParam->pKeyInRef, prKernParam->u4Alg, prKernParam->pInData,
        prKernParam->pInExpected, &u4Result);
    #else
    if(prKernParam->pInData != NULL)
    {
        pInData = hci_BinstrAlloc(((binstr_t *)prKernParam->pInData)->len,
            ((binstr_t *)prKernParam->pInData)->buf);
        pInData->buf = (u_int8_t*)PHYSICAL((UINT32)pInData->buf);
        prKernParam->pInData = (void*)PHYSICAL((UINT32)pInData);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInData,
            pInData->len + sizeof(binstr_t));
    }
    if(prKernParam->pInExpected != NULL)
    {
        pInExpected = hci_BinstrAlloc(((binstr_t *)prKernParam->pInExpected)->len,
            ((binstr_t *)prKernParam->pInExpected)->buf);
        pInExpected->buf = (u_int8_t*)PHYSICAL((UINT32)pInExpected->buf);
        prKernParam->pInExpected = (void*)PHYSICAL((UINT32)pInExpected);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInExpected,
            pInExpected->len + sizeof(binstr_t));
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_HASH_T));
    fgRet = TZ_HCI_HmacCheck(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    u4Result = prKernParam->u4OutResult;
    if(pInData != NULL)
    {
        hci_BinstrFree(pInData);
    }
    if(pInExpected != NULL)
    {
        hci_BinstrFree(pInExpected);
    }
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4OutResult, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4OutResult, &u4Result, sizeof(UINT32));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_HashCheck(MTDMX_MWIF_HCI_HASH_T *prKernParam,
    MTDMX_MWIF_HCI_HASH_T *prUserParam)
{
    retcode_t rRet;
    UINT32 u4Result;
    #ifdef CC_TRUSTZONE_SUPPORT
    binstr_t *pInData = NULL;
    binstr_t *pInExpected = NULL;
    BOOL fgRet;
    #endif

    PRINTK("+%s.....0x%x\n", __FUNCTION__, prKernParam->pKeyInRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_HashCheck(prKernParam->pKeyInRef, prKernParam->u4Alg, prKernParam->pInData,
        prKernParam->pInExpected, &u4Result);
    #else
    if(prKernParam->pInData != NULL)
    {
        pInData = hci_BinstrAlloc(((binstr_t *)prKernParam->pInData)->len,
            ((binstr_t *)prKernParam->pInData)->buf);
        pInData->buf = (u_int8_t*)PHYSICAL((UINT32)pInData->buf);
        prKernParam->pInData = (void*)PHYSICAL((UINT32)pInData);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInData,
            pInData->len + sizeof(binstr_t));
    }
    if(prKernParam->pInExpected != NULL)
    {
        pInExpected = hci_BinstrAlloc(((binstr_t *)prKernParam->pInExpected)->len,
            ((binstr_t *)prKernParam->pInExpected)->buf);
        pInExpected->buf = (u_int8_t*)PHYSICAL((UINT32)pInExpected->buf);
        prKernParam->pInExpected = (void*)PHYSICAL((UINT32)pInExpected);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInExpected,
            pInExpected->len + sizeof(binstr_t));
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_HASH_T));
    fgRet = TZ_HCI_HashCheck(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    u4Result = prKernParam->u4OutResult;
    if(pInData != NULL)
    {
        hci_BinstrFree(pInData);
    }
    if(pInExpected != NULL)
    {
        hci_BinstrFree(pInExpected);
    }
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4OutResult, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4OutResult, &u4Result, sizeof(UINT32));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_MoveKeySize(MTDMX_MWIF_HCI_MOVEKEY_T *prKernParam,
    MTDMX_MWIF_HCI_MOVEKEY_T *prUserParam)
{
    retcode_t rRet;
    UINT32 u4Size;
    #ifndef CC_TRUSTZONE_SUPPORT    
    binstr_t *out_data = NULL;
    #else
    binstr_t *pInData = NULL;
    BOOL fgRet;
    #endif

    PRINTK("+%s.....%x\n", __FUNCTION__, prKernParam->pKeyRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_MoveKeySize(prKernParam->pKeyRef, prKernParam->pInData, &out_data);
    if(rRet == ERR_OK)
    {
        u4Size = out_data->len;
    }
    #else
    if(prKernParam->pInData != NULL)
    {
        pInData = hci_BinstrAlloc(((binstr_t *)prKernParam->pInData)->len,
            ((binstr_t *)prKernParam->pInData)->buf);
        pInData->buf = (u_int8_t*)PHYSICAL((UINT32)pInData->buf);
        prKernParam->pInData = (void*)PHYSICAL((UINT32)pInData);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInData,
            pInData->len + sizeof(binstr_t));
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_MOVEKEY_T));
    fgRet = TZ_HCI_MoveKeySize(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    u4Size = prKernParam->u4Size;
    if(pInData != NULL)
    {
        hci_BinstrFree(pInData);
    }
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Size, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Size, &u4Size, sizeof(UINT32));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d %d\n", __FUNCTION__, rRet, u4Size);

    return TRUE;
}


static BOOL _HCI_MoveKey(MTDMX_MWIF_HCI_MOVEKEY_T *prKernParam,
    MTDMX_MWIF_HCI_MOVEKEY_T *prUserParam)
{
    retcode_t rRet;
    binstr_t *out_data;
    UINT32 u4Size;
    #ifdef CC_TRUSTZONE_SUPPORT
    binstr_t *pInData = NULL;
    BOOL fgRet;
    #endif

    PRINTK("+%s.....0x%x\n", __FUNCTION__, prKernParam->pKeyRef);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_MoveKey(prKernParam->pKeyRef, prKernParam->pInData, &out_data);
    if(rRet == ERR_OK)
    {
        u4Size = out_data->len;
    }
    #else
    if(prKernParam->pInData != NULL)
    {
        pInData = hci_BinstrAlloc(((binstr_t *)prKernParam->pInData)->len,
            ((binstr_t *)prKernParam->pInData)->buf);
        pInData->buf = (u_int8_t*)PHYSICAL((UINT32)pInData->buf);
        prKernParam->pInData = (void*)PHYSICAL((UINT32)pInData);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInData,
            pInData->len + sizeof(binstr_t));
    }
    out_data = hci_BinstrAlloc(prKernParam->u4Size, NULL);
    prKernParam->pOutData = (void*)PHYSICAL((UINT32)out_data->buf);
    HalFlushInvalidateDCacheMultipleLine((UINT32)out_data->buf,
        prKernParam->u4Size);
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_MOVEKEY_T));
    fgRet = TZ_HCI_MoveKey(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    u4Size = prKernParam->u4Size;
    if(pInData != NULL)
    {
        hci_BinstrFree(pInData);
    }
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->pKeyRef, sizeof(void *));
    COPY_TO_USER_ARG_SIZE(&prUserParam->pKeyRef, &prKernParam->pKeyRef, sizeof(void *));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->pOutData, u4Size);
    COPY_TO_USER_ARG_SIZE(prUserParam->pOutData, out_data->buf, u4Size);
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));
    #ifdef CC_TRUSTZONE_SUPPORT
    hci_BinstrFree(out_data);
    #endif

    PRINTK("-%s.....%d %d\n", __FUNCTION__, rRet, u4Size);

    return TRUE;
}


static BOOL _HCI_PersonalityKeyLoad(MTDMX_MWIF_HCI_PERSONALITY_KEY_T *prKernParam,
    MTDMX_MWIF_HCI_PERSONALITY_KEY_T *prUserParam)
{
    retcode_t rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    UINT8 *pInData;
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_PersonalityKeyLoad((hci_pers_key_type_t)prKernParam->u4KeyType,
        prKernParam->u4InSize, prKernParam->pInData);
    #else
    pInData = x_mem_alloc(prKernParam->u4InSize);
    x_memcpy(pInData, prKernParam->pInData, prKernParam->u4InSize);
    prKernParam->pInData = (void*)PHYSICAL((UINT32)pInData);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pInData,
        prKernParam->u4InSize);
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_PERSONALITY_KEY_T));
    fgRet = TZ_HCI_PersonalityKeyLoad(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    x_mem_free(pInData);
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}

static BOOL _HCI_PersonalityKeyCheck(MTDMX_MWIF_HCI_PERSONALITY_KEY_T *prKernParam,
    MTDMX_MWIF_HCI_PERSONALITY_KEY_T *prUserParam)
{
    retcode_t rRet;
    #ifdef CC_TRUSTZONE_SUPPORT
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_PersonalityKeyCheck((hci_pers_key_type_t)prKernParam->u4KeyType);
    #else
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_PERSONALITY_KEY_T));
    fgRet = TZ_HCI_PersonalityKeyCheck(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    #endif

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_ImportSasSize(MTDMX_MWIF_HCI_IMPORT_SAS_T *prKernParam,
    MTDMX_MWIF_HCI_IMPORT_SAS_T *prUserParam)
{
    retcode_t rRet;
    binstr_t *out_public_sas = NULL;
    hci_sas_keys_t *out_sas_keys = NULL;
    #ifndef CC_TRUSTZONE_SUPPORT    
    binstr_t          **pp_out_public_sas;
    hci_sas_keys_t    **pp_out_sas_keys;
    #else
    binstr_t *pInSAS = NULL;
    binstr_t *pInApp = NULL;
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    pp_out_public_sas = &out_public_sas;
    if(prKernParam->pOutSAS == NULL)
    {
        pp_out_public_sas = NULL;
    }

    pp_out_sas_keys = &out_sas_keys;
    if(prKernParam->pOutKeys == NULL)
    {
        pp_out_sas_keys = NULL;
    }

    rRet = hci_ImportSAS(prKernParam->pInSAS, prKernParam->pInApp,
        pp_out_public_sas, pp_out_sas_keys);
    #else
    if(prKernParam->pInSAS != NULL)
    {
        pInSAS = hci_BinstrAlloc(((binstr_t *)prKernParam->pInSAS)->len,
            ((binstr_t *)prKernParam->pInSAS)->buf);
        pInSAS->buf = (u_int8_t*)PHYSICAL((UINT32)pInSAS->buf);
        prKernParam->pInSAS = (void*)PHYSICAL((UINT32)pInSAS);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInSAS,
            pInSAS->len + sizeof(binstr_t));
    }
    if(prKernParam->pInApp != NULL)
    {
        pInApp = hci_BinstrAlloc(((binstr_t *)prKernParam->pInApp)->len,
            ((binstr_t *)prKernParam->pInApp)->buf);
        pInApp->buf = (u_int8_t*)PHYSICAL((UINT32)pInApp->buf);
        prKernParam->pInApp = (void*)PHYSICAL((UINT32)pInApp);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInApp,
            pInApp->len + sizeof(binstr_t));
    }

    if(prKernParam->pOutSAS != NULL)
    {
        out_public_sas = hci_BinstrAlloc(0, NULL);
        prKernParam->pOutSAS = (void*)PHYSICAL((UINT32)out_public_sas);
        HalFlushInvalidateDCacheMultipleLine((UINT32)out_public_sas, sizeof(binstr_t));
    }

    if(prKernParam->pOutKeys != NULL)
    {
        out_sas_keys = x_mem_alloc(sizeof(hci_sas_keys_t));
        memset(out_sas_keys, 0, sizeof(hci_sas_keys_t));
        prKernParam->pOutKeys = (void*)PHYSICAL((UINT32)out_sas_keys);
        HalFlushInvalidateDCacheMultipleLine((UINT32)out_sas_keys, sizeof(hci_sas_keys_t));
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_IMPORT_SAS_T));
    fgRet = TZ_HCI_ImportSasSize(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    if(pInSAS != NULL)
    {
        hci_BinstrFree(pInSAS);
    }
    if(pInApp != NULL)
    {
        hci_BinstrFree(pInApp);
    }
    #endif

    if(rRet == ERR_OK)
    {
        out_public_sas = (binstr_t *)VIRTUAL((UINT32)out_public_sas);
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->pOutSAS, sizeof(binstr_t));
        COPY_TO_USER_ARG_SIZE(prUserParam->pOutSAS, out_public_sas, sizeof(binstr_t));
        out_sas_keys = (hci_sas_keys_t *)VIRTUAL((UINT32)out_sas_keys);
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->pOutKeys, sizeof(hci_sas_keys_t));
        COPY_TO_USER_ARG_SIZE(prUserParam->pOutKeys, out_sas_keys, sizeof(hci_sas_keys_t));
        #ifdef CC_TRUSTZONE_SUPPORT
        if(out_public_sas != NULL)
        {
            hci_BinstrFree(out_public_sas);
        }
        if(out_sas_keys != NULL)
        {
            x_mem_free(out_sas_keys);
        }
        #endif
    }
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_ImportSas(MTDMX_MWIF_HCI_IMPORT_SAS_T *prKernParam,
    MTDMX_MWIF_HCI_IMPORT_SAS_T *prUserParam)
{
    retcode_t rRet;
    binstr_t *out_public_sas = NULL;
    hci_sas_keys_t *out_sas_keys = NULL;
    binstr_t *pUser_out_public_sas;
    hci_sas_keys_t *pUser_out_sas_keys;
    #ifdef CC_TRUSTZONE_SUPPORT
    binstr_t *pInSAS = NULL;
    binstr_t *pInApp = NULL;
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_ImportSAS(prKernParam->pInSAS, prKernParam->pInApp,
        &out_public_sas, &out_sas_keys);
    #else
    if(prKernParam->pInSAS != NULL)
    {
        pInSAS = hci_BinstrAlloc(((binstr_t *)prKernParam->pInSAS)->len,
            ((binstr_t *)prKernParam->pInSAS)->buf);
        pInSAS->buf = (u_int8_t*)PHYSICAL((UINT32)pInSAS->buf);
        prKernParam->pInSAS = (void*)PHYSICAL((UINT32)pInSAS);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInSAS,
            pInSAS->len + sizeof(binstr_t));
    }
    if(prKernParam->pInApp != NULL)
    {
        pInApp = hci_BinstrAlloc(((binstr_t *)prKernParam->pInApp)->len,
            ((binstr_t *)prKernParam->pInApp)->buf);
        pInApp->buf = (u_int8_t*)PHYSICAL((UINT32)pInApp->buf);
        prKernParam->pInApp = (void*)PHYSICAL((UINT32)pInApp);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInApp,
            pInApp->len + sizeof(binstr_t));
    }
    if(prKernParam->pOutSAS != NULL)
    {
        out_public_sas = hci_BinstrAlloc(prKernParam->u4OutSASSize, NULL);
        prKernParam->pOutSAS = (void*)PHYSICAL((UINT32)out_public_sas);
        out_public_sas->buf = (u_int8_t*)PHYSICAL((UINT32)out_public_sas->buf);
        HalFlushInvalidateDCacheMultipleLine((UINT32)out_public_sas, sizeof(binstr_t));
    }

    if(prKernParam->pOutKeys != NULL)
    {
        out_sas_keys = x_mem_alloc(sizeof(hci_sas_keys_t));
        memset(out_sas_keys, 0, sizeof(hci_sas_keys_t));
        prKernParam->pOutKeys = (void*)PHYSICAL((UINT32)out_sas_keys);
        if (prKernParam->u4OutKeysCount)
        {
            out_sas_keys->contentids = x_mem_alloc(sizeof(binstr_t) * prKernParam->u4OutKeysCount);
            out_sas_keys->key_refs = x_mem_alloc(sizeof(hci_key_ref_t) * prKernParam->u4OutKeysCount);
            HalFlushInvalidateDCacheMultipleLine((UINT32)out_sas_keys->contentids,
                sizeof(binstr_t) * prKernParam->u4OutKeysCount);
            HalFlushInvalidateDCacheMultipleLine((UINT32)out_sas_keys->key_refs,
                sizeof(hci_key_ref_t) * prKernParam->u4OutKeysCount);
            out_sas_keys->contentids = (binstr_t**)PHYSICAL((UINT32)out_sas_keys->contentids);
            out_sas_keys->key_refs = (hci_key_ref_t**)PHYSICAL((UINT32)out_sas_keys->key_refs);
        }
        HalFlushInvalidateDCacheMultipleLine((UINT32)out_sas_keys, sizeof(hci_sas_keys_t));
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_IMPORT_SAS_T));
    fgRet = TZ_HCI_ImportSas(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    if(pInSAS != NULL)
    {
        hci_BinstrFree(pInSAS);
    }
    if(pInApp != NULL)
    {
        hci_BinstrFree(pInApp);
    }
    #endif

    if(rRet == ERR_OK)
    {
        pUser_out_public_sas = prUserParam->pOutSAS;
        pUser_out_sas_keys = prUserParam->pOutKeys;
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pUser_out_public_sas->buf, out_public_sas->len);
        COPY_TO_USER_ARG_SIZE(pUser_out_public_sas->buf, (void *)VIRTUAL((UINT32)out_public_sas->buf), out_public_sas->len);
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pUser_out_sas_keys->key_refs,
            sizeof(hci_key_ref_t) * pUser_out_sas_keys->key_count);
        COPY_TO_USER_ARG_SIZE(pUser_out_sas_keys->key_refs, VIRTUAL((UINT32)out_sas_keys->key_refs),
            sizeof(hci_key_ref_t) * pUser_out_sas_keys->key_count);
        #ifdef CC_TRUSTZONE_SUPPORT
        if(out_public_sas != NULL)
        {
            hci_BinstrFree(out_public_sas);
        }
        if(out_sas_keys != NULL)
        {
            if(out_sas_keys->contentids != NULL)
            {
                x_mem_free((void*)VIRTUAL((UINT32)out_sas_keys->contentids));
            }
            if(out_sas_keys->key_refs != NULL)
            {
                x_mem_free((void*)VIRTUAL((UINT32)out_sas_keys->key_refs));
            }
            x_mem_free(out_sas_keys);
        }
        #endif
    }

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _HCI_ImportSasIds(MTDMX_MWIF_HCI_IMPORT_SAS_T *prKernParam,
    MTDMX_MWIF_HCI_IMPORT_SAS_T *prUserParam)
{
    retcode_t rRet;
    hci_sas_keys_t *out_sas_keys = NULL;
    binstr_t *pUser_out_public_sas;
    hci_sas_keys_t *pUser_out_sas_keys;
    UINT32 i;
    #ifdef CC_TRUSTZONE_SUPPORT
    binstr_t *pInSAS = NULL;
    binstr_t *pInApp = NULL;
    BOOL fgRet;
    #endif

    PRINTK("+%s.....\n", __FUNCTION__);

    #ifndef CC_TRUSTZONE_SUPPORT
    rRet = hci_ImportSAS(prKernParam->pInSAS, prKernParam->pInApp,
        &out_public_sas, &out_sas_keys);
    #else
    if(prKernParam->pInSAS != NULL)
    {
        pInSAS = hci_BinstrAlloc(((binstr_t *)prKernParam->pInSAS)->len,
            ((binstr_t *)prKernParam->pInSAS)->buf);
        pInSAS->buf = (u_int8_t*)PHYSICAL((UINT32)pInSAS->buf);
        prKernParam->pInSAS = (void*)PHYSICAL((UINT32)pInSAS);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInSAS,
            pInSAS->len + sizeof(binstr_t));
    }
    if(prKernParam->pInApp != NULL)
    {
        pInApp = hci_BinstrAlloc(((binstr_t *)prKernParam->pInApp)->len,
            ((binstr_t *)prKernParam->pInApp)->buf);
        pInApp->buf = (u_int8_t*)PHYSICAL((UINT32)pInApp->buf);
        prKernParam->pInApp = (void*)PHYSICAL((UINT32)pInApp);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInApp,
            pInApp->len + sizeof(binstr_t));
    }
    if(prKernParam->pOutKeys != NULL)
    {
        out_sas_keys = x_mem_alloc(sizeof(hci_sas_keys_t));
        memset(out_sas_keys, 0, sizeof(hci_sas_keys_t));
        prKernParam->pOutKeys = (void*)PHYSICAL((UINT32)out_sas_keys);
        if (prKernParam->u4OutKeysCount)
        {
            out_sas_keys->contentids = x_mem_alloc(sizeof(binstr_t) * prKernParam->u4OutKeysCount);
            for(i = 0; i < prKernParam->u4OutKeysCount; i++)
            {
                out_sas_keys->contentids[i] = hci_BinstrAlloc(L_MS3_CONTENT_ID_SIZE, NULL);
                out_sas_keys->contentids[i]->buf = (u_int8_t*)PHYSICAL((UINT32)out_sas_keys->contentids[i]->buf);
                HalFlushInvalidateDCacheMultipleLine((UINT32)out_sas_keys->contentids[i],
                    L_MS3_CONTENT_ID_SIZE);
                out_sas_keys->contentids[i] = (binstr_t *)PHYSICAL((UINT32)out_sas_keys->contentids[i]);
            }
            HalFlushInvalidateDCacheMultipleLine((UINT32)out_sas_keys->contentids,
                sizeof(binstr_t) * prKernParam->u4OutKeysCount);
            out_sas_keys->contentids = (binstr_t **)PHYSICAL((UINT32)out_sas_keys->contentids);
        }
        HalFlushInvalidateDCacheMultipleLine((UINT32)out_sas_keys,
            sizeof(hci_sas_keys_t));
    }
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_HCI_IMPORT_SAS_T));
    fgRet = TZ_HCI_ImportSasIds(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    rRet = prKernParam->u4Ret;
    if(pInSAS != NULL)
    {
        hci_BinstrFree(pInSAS);
    }
    if(pInApp != NULL)
    {
        hci_BinstrFree(pInApp);
    }
    #endif


    if(rRet == ERR_OK)
    {
        pUser_out_public_sas = prUserParam->pOutSAS;
        pUser_out_sas_keys = prUserParam->pOutKeys;
        if(pUser_out_sas_keys->key_count != 0)
        {
            out_sas_keys->contentids = (binstr_t**)VIRTUAL((UINT32)out_sas_keys->contentids);
        }
        for(i = 0; i < pUser_out_sas_keys->key_count; i++)
        {
            out_sas_keys->contentids[i] = (binstr_t*)VIRTUAL((UINT32)out_sas_keys->contentids[i]);
            USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pUser_out_sas_keys->contentids[i]->buf,
                L_MS3_CONTENT_ID_SIZE);
            COPY_TO_USER_ARG_SIZE(pUser_out_sas_keys->contentids[i]->buf, VIRTUAL((UINT32)out_sas_keys->contentids[i]->buf),
                L_MS3_CONTENT_ID_SIZE);
        }
        #ifdef CC_TRUSTZONE_SUPPORT
        if(out_sas_keys != NULL)
        {
            for(i = 0; i < prKernParam->u4OutKeysCount; i++)
            {
                if(out_sas_keys->contentids[i] != NULL)
                {
                    hci_BinstrFree((void *)VIRTUAL((UINT32)out_sas_keys->contentids[i]));
                }
            }
            if(out_sas_keys->contentids != NULL)
            {
                x_mem_free((void*)VIRTUAL((UINT32)out_sas_keys->contentids));
            }
            x_mem_free(out_sas_keys);
        }
        #endif
    }
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->u4Ret, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4Ret, &rRet, sizeof(UINT32));

    PRINTK("-%s.....%d\n", __FUNCTION__, rRet);

    return TRUE;
}


static BOOL _DRM_TZ_SetSecretDataType(MTDMX_MWIF_DRM_TZ_SETSECRETDATATYPE_T *prKernParam,
    MTDMX_MWIF_DRM_TZ_SETSECRETDATATYPE_T *prUserParam)
{
    #ifdef CC_TRUSTZONE_SUPPORT 
    BOOL fgRet;
    DRM_ErrorInf rErrorInf;    
    void *pRootCertBuf = NULL;
    void *pCaCertBuf = NULL;
    void *pClientCertBuf = NULL;
    void *pClientKeyBuf = NULL;
    UINT32 u4Size;
    
    if(prKernParam->pRootCertBuf != NULL)
    {
        u4Size = prKernParam->u4RootCertLen;
        pRootCertBuf = x_mem_alloc(u4Size);
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prKernParam->pRootCertBuf, u4Size);
        COPY_FROM_USER_ARG_SIZE(prKernParam->pRootCertBuf, pRootCertBuf, u4Size);        
        prKernParam->pRootCertBuf = (void*)PHYSICAL((UINT32)pRootCertBuf);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pRootCertBuf, u4Size);    
    }
    if(prKernParam->pCaCertBuf != NULL)
    {
        u4Size = prKernParam->u4CaCertLen;        
        pCaCertBuf = x_mem_alloc(u4Size);
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prKernParam->pCaCertBuf, u4Size);
        COPY_FROM_USER_ARG_SIZE(prKernParam->pCaCertBuf, pCaCertBuf, u4Size);          
        prKernParam->pCaCertBuf = (void*)PHYSICAL((UINT32)pCaCertBuf);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pCaCertBuf, u4Size);
    }
    if(prKernParam->pClientCertBuf != NULL)
    {
        u4Size = prKernParam->u4ClientCertLen;          
        pClientCertBuf = x_mem_alloc(u4Size);
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prKernParam->pClientCertBuf, u4Size);
        COPY_FROM_USER_ARG_SIZE(prKernParam->pClientCertBuf, pClientCertBuf, u4Size);          
        prKernParam->pClientCertBuf = (void*)PHYSICAL((UINT32)pClientCertBuf);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pClientCertBuf, u4Size);
    }
    if(prKernParam->pClientKeyBuf != NULL)
    {
        u4Size = prKernParam->u4ClientKeyLen;        
        pClientKeyBuf = x_mem_alloc(u4Size);
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prKernParam->pClientKeyBuf, u4Size);
        COPY_FROM_USER_ARG_SIZE(prKernParam->pClientKeyBuf, pClientKeyBuf, u4Size);          
        prKernParam->pClientKeyBuf = (void*)PHYSICAL((UINT32)pClientKeyBuf);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pClientKeyBuf, u4Size);
    }
    prKernParam->prErrorInf = (DRM_ErrorInf *)PHYSICAL((UINT32)&rErrorInf);
    HalFlushInvalidateDCacheMultipleLine((UINT32)&rErrorInf, sizeof(rErrorInf));
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_DRM_TZ_SETSECRETDATATYPE_T));
    
    fgRet = TZ_DRM_TZ_SetSecretDataType(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->i4Ret, sizeof(INT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->i4Ret, &prKernParam->i4Ret, sizeof(INT32));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->prErrorInf, sizeof(DRM_ErrorInf));
    COPY_TO_USER_ARG_SIZE(prUserParam->prErrorInf, &rErrorInf, sizeof(DRM_ErrorInf));    

    if(pRootCertBuf != NULL)
    {
        x_mem_free(pRootCertBuf);
    }
    if(pCaCertBuf != NULL)
    {
        x_mem_free(pCaCertBuf);
    }
    if(pClientCertBuf != NULL)
    {
        x_mem_free(pClientCertBuf);
    }
    if(pClientKeyBuf != NULL)
    {
        x_mem_free(pClientKeyBuf);
    }    
    #endif
    
    return TRUE;
}

static BOOL _DRM_TZ_Open(MTDMX_MWIF_DRM_TZ_OPEN_T *prKernParam,
    MTDMX_MWIF_DRM_TZ_OPEN_T *prUserParam)
{
    #ifdef CC_TRUSTZONE_SUPPORT 
    BOOL fgRet;
    DRM_ErrorInf rErrorInf;
    DRM_ExtractInfo rExtractInfo;
    void *pLicenseObj = NULL;
    void *pLid = NULL;
    UINT32 u4Size;
    
    if(prKernParam->pLicenseObj != NULL)
    {
        u4Size = prKernParam->u4LicenseObjLen;
        pLicenseObj = x_mem_alloc(u4Size);
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prKernParam->pLicenseObj, u4Size);
        COPY_FROM_USER_ARG_SIZE(prKernParam->pLicenseObj, pLicenseObj, u4Size);        
        prKernParam->pLicenseObj = (void*)PHYSICAL((UINT32)pLicenseObj);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pLicenseObj, u4Size);    
    }
    if(prKernParam->pLid != NULL)
    {
        u4Size = prKernParam->u4LidLen;        
        pLid = x_mem_alloc(u4Size);
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prKernParam->pLid, u4Size);
        COPY_FROM_USER_ARG_SIZE(prKernParam->pLid, pLid, u4Size);          
        prKernParam->pLid = (void*)PHYSICAL((UINT32)pLid);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pLid, u4Size);
    }
    prKernParam->pExtractInfo = (DRM_ExtractInfo*)PHYSICAL((UINT32)&rExtractInfo);
    prKernParam->prErrorInf = (DRM_ErrorInf*)PHYSICAL((UINT32)&rErrorInf);
    HalFlushInvalidateDCacheMultipleLine((UINT32)&rExtractInfo, sizeof(rExtractInfo));
    HalFlushInvalidateDCacheMultipleLine((UINT32)&rErrorInf, sizeof(rErrorInf));    
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_DRM_TZ_OPEN_T));
    
    fgRet = TZ_DRM_TZ_Open(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->i4Ret, sizeof(INT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->i4Ret, &prKernParam->i4Ret, sizeof(INT32));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->prErrorInf, sizeof(DRM_ErrorInf));
    COPY_TO_USER_ARG_SIZE(prUserParam->prErrorInf, &rErrorInf, sizeof(DRM_ErrorInf)); 
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->pExtractInfo, sizeof(DRM_ExtractInfo));
    COPY_TO_USER_ARG_SIZE(prUserParam->pExtractInfo, &rExtractInfo, sizeof(DRM_ExtractInfo)); 
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->i4DecSlot, sizeof(INT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->i4DecSlot, &prKernParam->i4DecSlot, sizeof(INT32)); 

    if(pLicenseObj != NULL)
    {
        x_mem_free(pLicenseObj);
    }
    if(pLid != NULL)
    {
        x_mem_free(pLid);
    }
    #endif
    
    return TRUE;
}

static BOOL _DRM_TZ_Close(MTDMX_MWIF_DRM_TZ_OPEN_T *prKernParam,
    MTDMX_MWIF_DRM_TZ_OPEN_T *prUserParam)
{
    #ifdef CC_TRUSTZONE_SUPPORT 
    BOOL fgRet;
    DRM_ErrorInf rErrorInf;

    prKernParam->prErrorInf = (DRM_ErrorInf *)PHYSICAL((UINT32)&rErrorInf);
    HalFlushInvalidateDCacheMultipleLine((UINT32)&rErrorInf, sizeof(rErrorInf));  
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_DRM_TZ_OPEN_T));
    
    fgRet = TZ_DRM_TZ_Close(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->i4Ret, sizeof(INT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->i4Ret, &prKernParam->i4Ret, sizeof(INT32));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->prErrorInf, sizeof(DRM_ErrorInf));
    COPY_TO_USER_ARG_SIZE(prUserParam->prErrorInf, &rErrorInf, sizeof(DRM_ErrorInf)); 
    #endif
    
    return TRUE;
}

static BOOL _DRM_TZ_DecodePacketsVOD(MTDMX_MWIF_DRM_TZ_DECODEPACKETS_VOD_T *prKernParam,
    MTDMX_MWIF_DRM_TZ_DECODEPACKETS_VOD_T *prUserParam)
{
    #ifdef CC_TRUSTZONE_SUPPORT 
    BOOL fgRet;
    DRM_ErrorInf rErrorInf;
    DecoderDRM_Offset* pOffsets = NULL;
    void *pInBuf = NULL;
    void *pOutBuf = NULL;
    UINT32 u4Size;
    
    if(prKernParam->pInBuf != NULL)
    {
        u4Size = (prKernParam->u4PacketSize) * (prKernParam->u4InNumOfPackets);
        pInBuf = x_mem_alloc(u4Size);
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prKernParam->pInBuf, u4Size);
        COPY_FROM_USER_ARG_SIZE(prKernParam->pInBuf, pInBuf, u4Size);        
        prKernParam->pInBuf = (void*)PHYSICAL((UINT32)pInBuf);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pInBuf, u4Size);    
    }
    if(prKernParam->pOutBuf != NULL)
    {
        u4Size = prKernParam->u4OutBufLen;
        pOutBuf = x_mem_alloc(u4Size);
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prKernParam->pOutBuf, u4Size);
        COPY_FROM_USER_ARG_SIZE(prKernParam->pOutBuf, pOutBuf, u4Size);          
        prKernParam->pOutBuf = (void*)PHYSICAL((UINT32)pOutBuf);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pOutBuf, u4Size);
    }
    if(prKernParam->pOffsets != NULL)
    {
        u4Size = sizeof(DecoderDRM_Offset) * (prKernParam->u4InNumOfPackets);
        pOffsets = x_mem_alloc(u4Size);
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prKernParam->pOffsets, u4Size);
        COPY_FROM_USER_ARG_SIZE(prKernParam->pOffsets, pOffsets, u4Size);        
        prKernParam->pOffsets = (DecoderDRM_Offset*)PHYSICAL((UINT32)pOffsets);
        HalFlushInvalidateDCacheMultipleLine((UINT32)pOffsets, u4Size);    
    }    
    prKernParam->prErrorInf = (DRM_ErrorInf *)PHYSICAL((UINT32)&rErrorInf);
    HalFlushInvalidateDCacheMultipleLine((UINT32)&rErrorInf, sizeof(rErrorInf));    
    HalFlushInvalidateDCacheMultipleLine((UINT32)prKernParam,
        sizeof(MTDMX_MWIF_DRM_TZ_DECODEPACKETS_VOD_T));

    fgRet = TZ_DRM_TZ_DecodePacketsVOD(prKernParam);
    if(!fgRet)
    {
        PRINTK("TZ-%s.....\n", __FUNCTION__);
    }

    if(prKernParam->pOutBuf != NULL)
    {
        // copy data to pOutBuf
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->pOutBuf, 
            (prKernParam->u4PacketSize) * (prKernParam->u4OutNumOfPackets));
        COPY_TO_USER_ARG_SIZE(prUserParam->pOutBuf, pOutBuf, 
            (prKernParam->u4PacketSize) * (prKernParam->u4OutNumOfPackets));         
    }
    else
    {
        // copy data to pInBuf
        USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->pInBuf, 
            (prKernParam->u4PacketSize) * (prKernParam->u4OutNumOfPackets));
        COPY_TO_USER_ARG_SIZE(prUserParam->pInBuf, pInBuf, 
            (prKernParam->u4PacketSize) * (prKernParam->u4OutNumOfPackets));          
    }
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(&prUserParam->i4Ret, sizeof(INT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->i4Ret, &prKernParam->i4Ret, sizeof(INT32));
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->prErrorInf, sizeof(DRM_ErrorInf));
    COPY_TO_USER_ARG_SIZE(prUserParam->prErrorInf, &rErrorInf, sizeof(DRM_ErrorInf)); 
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(prUserParam->u4OutNumOfPackets, sizeof(UINT32));
    COPY_TO_USER_ARG_SIZE(&prUserParam->u4OutNumOfPackets, &prKernParam->u4OutNumOfPackets, 
        sizeof(UINT32)); 

    if(pInBuf != NULL)
    {
        x_mem_free(pInBuf);
    }
    if(pOutBuf != NULL)
    {
        x_mem_free(pOutBuf);
    }
    if(pOffsets != NULL)
    {
        x_mem_free(pOffsets);
    }    
    #endif
    
    return TRUE;    
}

//-----------------------------------------------------------------------------
/*
 *  Find PID index for a given PID value
 *
 *  @param  u2Pid           PID
 *  @param  pu1PidIndex [out]
 *                          The PID index if found
 */
//-----------------------------------------------------------------------------
MT_RESULT_T MTDMX_MWIF_Call(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    void *pvUserParam;
    void *pvKernParam;
    UINT32 u4Size;
    MT_RESULT_T rRet;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    pvUserParam = (void *)rArg.ai4Arg[0];
    u4Size = (UINT32)rArg.ai4Arg[1];

    pvKernParam = x_mem_alloc(u4Size);

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pvUserParam, u4Size);
    COPY_FROM_USER_ARG_SIZE(pvUserParam, pvKernParam, u4Size);

    switch(*(UINT32 *)pvKernParam) // the first 4 bytes is interface ID
    {
    case MTDMX_MWIF_SKB_ENGINE_GET_INSTANCE:
        rRet = _SKB_GetInstance(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_SKB_ENGINE_RELEASE:
        rRet = _SKB_Engine_Release(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_SKB_ENGINE_GET_INFO:
        rRet = _SKB_GetInfo(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_SKB_ENGINE_GET_PROPERTIES_SIZE:
        rRet = _SKB_GetProertiesSize(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_SKB_ENGINE_GET_PROPERTIES_DATA:
        rRet = _SKB_GetProertiesData(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_WRAPPED:
        rRet = _SKB_DataFromWrapped(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_EXPORTED:
        rRet = _SKB_DataFromExported(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_SKB_ENGINE_CREATE_TRANSFORM:
        rRet = _SKB_CreateTransform(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_SKB_ENGINE_CREATE_CIPHER:
        rRet = _SKB_CreateCipher(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_SKB_SECUREDATA_RELEASE:
        rRet = _SKB_SecureData_Release(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_SKB_SECUREDATA_GETINFO:
        rRet = _SKB_SecureDataGetInfo(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_SKB_SECUREDATA_EXPORT:
        rRet = _SKB_SecureData_Export(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_SKB_SECUREDATA_DERIVE:
        rRet = _SKB_SecureData_Derive(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_SKB_TRANSFORM_RELEASE:
        rRet = _SKB_Transform_Release(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_SKB_TRANSFORM_ADDBYTES:
        rRet = _SKB_Transform_AddBytes(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_SKB_TRANSFORM_ADDSECUREDATA:
        rRet = _SKB_Transform_AddSecureData(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_SKB_TRANSFORM_GETOUTPUT:
        rRet = _SKB_Transform_GetOutput(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_SKB_CIPHER_PROCESSBUFFER:
        rRet = _SKB_Cipher_ProcessBuffer(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_SKB_CIPHER_RELEASE:
        rRet = _SKB_Cipher_Release(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_INIT:
        rRet = _HCI_Init(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_FIN:
        rRet = _HCI_Fin(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_KEYREF_CREATE:
        rRet = _HCI_KeyRefCreate(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_KEYREF_CHECK:
        rRet = _HCI_KeyRefCheck(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_KEYREF_DESTROY:
        rRet = _HCI_KeyRefDestroy(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_DEVICE_GET_NUM:
        rRet = _HCI_DevIdGetNum(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_DEVICE_GET_SIZE:
        rRet = _HCI_DevIdGetSize(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_DEVICE_GET_NAME:
        rRet = _HCI_DevIdGetName(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_IMPORT_KEY:
        rRet = _HCI_ImportKey(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_EXPORT_KEY_SIZE:
        rRet = _HCI_ExportKeySize(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_EXPORT_KEY:
        rRet = _HCI_ExportKey(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_GET_START_FISH_ID_SIZE:
        rRet = _HCI_StarfishIdGetSize(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_GET_START_FISH_ID:
        rRet = _HCI_StarfishIdGet(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_GET_START_FISH_KEYDATA_SIZE:
        rRet = _HCI_KeyDataGetIfStarfishSize(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_GET_START_FISH_KEYDATA:
        rRet = _HCI_KeyDataGetIfStarfish(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_IMPORT_START_FISH_KEY:
        rRet = _HCI_ImportStarfishKey(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_DATA_SIGN_SIZE:
        rRet = _HCI_DataSignSize(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_DATA_SIGN:
        rRet = _HCI_DataSign(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_DATA_DECRYPT_SIZE:
        rRet = _HCI_DataDecryptSize(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_DATA_DECRYPT:
        rRet = _HCI_DataDecrypt(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_STREAM_DECRYPT_SIZE:
        rRet = _HCI_StreamDecryptSize(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_STREAM_DECRYPT:
        rRet = _HCI_StreamDecrypt(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_KEYDATA_ENCRYPT_SIZE:
        rRet = _HCI_KeyDataEncryptSize(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_KEYDATA_ENCRYPT:
        rRet = _HCI_KeyDataEncrypt(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_HMAC_CHECK:
        rRet = _HCI_HmacCheck(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_HASH_CHECK:
        rRet = _HCI_HashCheck(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_MOVE_KEY_SIZE:
        rRet = _HCI_MoveKeySize(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_HCI_MOVE_KEY:
        rRet = _HCI_MoveKey(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_PERSONALITYKEY_LOAD:
        rRet = _HCI_PersonalityKeyLoad(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_PERSONALITYKEY_CHECK:
        rRet = _HCI_PersonalityKeyCheck(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_IMPORT_SAS_SIZE:
        rRet = _HCI_ImportSasSize(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_IMPORT_SAS:
        rRet = _HCI_ImportSas(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_IMPORT_SAS_IDS:
        rRet = _HCI_ImportSasIds(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_FREE_SAS:
        rRet = MTR_OK;
        break;
    case MTDMX_MWIF_DRM_TZ_SETSECRETDATATYPE:
        rRet = _DRM_TZ_SetSecretDataType(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_DRM_TZ_OPEN:
        rRet = _DRM_TZ_Open(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_DRM_TZ_CLOSE:
        rRet = _DRM_TZ_Close(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;
    case MTDMX_MWIF_DRM_TZ_DECODEPACKETS_VOD:
        rRet = _DRM_TZ_DecodePacketsVOD(pvKernParam, pvUserParam)?MTR_OK:MTR_NOT_OK;
        break;        

    default:
        rRet = MTR_NOT_OK;
        break;
    }

    if(rRet == MTR_NOT_OK)
    {
        PRINTK("mtal_ioctl_mtdmx failed.....\n");
    }

    x_mem_free(pvKernParam);

    return rRet;
}

#endif

