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
 * $RCSfile: divx_drm_adp.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision:  $
 * $SWAuthor:  $
 * $MD5HEX:  $
 *
 * Description:
 *         
 *---------------------------------------------------------------------------*/
/*
 * $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_adp.c#1 $
 * Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
 * All rights reserved.
 *
 * This software is the confidential and proprietary information of DivX
 * Inc. ("Confidential Information").  You shall not disclose such Confidential
 * Information and shall use it only in accordance with the terms of the license
 * agreement you entered into with DivX, Inc.
*/

/*
 * This tool works with drm memory for the Authorized Disconnected Playback (ADP) DRM mode.
*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "libc/memory.h"
#include "os/inc/x_os.h"
#include "dbg/x_dbg.h"
#include "drm/drm_dbg.h"
#include "drm/divx_drm/divx_drm.h"

#ifdef DIVX_DRM_40
#include "drm/divx_drm/divx_drm_40/AdpLib/DrmAdpApi.h"
#include "drm/divx_drm/divx_drm_40/AdpLib/DrmAdpTypes.h"
#include "drm/divx_drm/divx_drm_40/AdpLocalLib/DrmAdpLocal.h"
#elif defined(DIVX_DRM_51)
#include "drm/divx_drm/divx_drm_51/DrmApi.h"
#include "drm/divx_drm/divx_drm_51/DrmTypes.h"
#include "drm/divx_drm/divx_drm_51/local/DrmLocal.h"
#include "drm/divx_drm/divx_drm_51/DrmDataLoad.h"
#include "drm/divx_drm/divx_drm_51/HeaderMem2Structs.h"
#endif

#else

#include "x_os.h"
#include "x_dbg.h"
#include "../drm_dbg.h"
#include "divx_drm.h"

#ifdef DIVX_DRM_40
#include "divx_drm_40/AdpLib/DrmAdpApi.h"
#include "divx_drm_40/AdpLib/DrmAdpTypes.h"
#include "divx_drm_40/AdpLocalLib/DrmAdpLocal.h"
#elif defined(DIVX_DRM_51)
#include "divx_drm_51/DrmApi.h"
#include "divx_drm_51/DrmTypes.h"
#include "divx_drm_51/local/DrmLocal.h"
#include "divx_drm_51/DrmDataLoad.h"
#include "divx_drm_51/HeaderMem2Structs.h"
#endif

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: divxDrmGetUIHelpInfo
 *
 * Description: This API retrives the divx drm UI help info.
 *
 * Inputs: i4_ui_info  Specify pointer to retrive the UI help info.
 *
 * Outputs: i4_ui_info  Specify pointer contains the UI help info.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_INTERNAL_ERROR  Fail.
 *          DRMR_FCT_NOT_SUPPORT  Fail.
 ----------------------------------------------------------------------------*/
INT32 divxDrmGetUIHelpInfo (
 UINT32* pui4_ui_help_info)
{
#ifdef DIVX_DRM_51
    *pui4_ui_help_info = 0;

    /* already activated, and not deactivated yet */
    if (DRM_DEVICE_IS_ACTIVATED == drmIsDeviceActivated())
    {
        *pui4_ui_help_info |= DIVX_DRM_DEACTIVATION_CONFIRMATION;
        *pui4_ui_help_info |= DIVX_DRM_REGISTRATION_CODE_HIDE;        
    }

    DBG_INFO((_INFO_HEADER"%s: %s to confirm for deactivation\n", __FUNCTION__, *pui4_ui_help_info & DIVX_DRM_DEACTIVATION_CONFIRMATION ? "Need" : "Don't need"));
    DBG_INFO((_INFO_HEADER"%s: %s to hide registration code\n", __FUNCTION__, *pui4_ui_help_info & DIVX_DRM_REGISTRATION_CODE_HIDE ? "Need" : "Don't need"));

    return DRMR_OK;
#else
    
    return DRMR_FCT_NOT_SUPPORT;
#endif
}

/*-----------------------------------------------------------------------------
 * Name: divxDrmGetRegistrationCode
 *
 * Description: This API retrives the divx drm registration code.
 *
 * Inputs: ps_registration_code  Specify pointer to retrive the registration code.
 *
 * Outputs: ps_registration_code  Specify pointer contains the registration code.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_INTERNAL_ERROR  Fail.
 ----------------------------------------------------------------------------*/
INT32 divxDrmGetRegistrationCode (
    CHAR* ps_registration_code)
{
#ifdef DIVX_DRM_40
    UINT8 ui1_result;
    CHAR s_registrationCodeString[DRM_REGISTRATION_CODE_BYTES] = {0};
    
    ui1_result = (UINT8)drmGetRegistrationCodeString(s_registrationCodeString);
    if (ADP_SUCCESS != ui1_result)
    {
        DBG_ERROR((_ERROR_HEADER"%s: Get registration code failed, Code %d\n", __FUNCTION__, drmAdpGetLastError()));
        printk("Get registration code fail\n");
        return DRMR_INTERNAL_ERROR;
    }

    DBG_INFO((_INFO_HEADER"%s: Registration Code %s\n", __FUNCTION__, s_registrationCodeString));

    /* fill data, registration code */
    if (NULL != ps_registration_code)
    {
        x_strncpy(ps_registration_code, s_registrationCodeString, DRM_REGISTRATION_CODE_BYTES);
    }

    return DRMR_OK;
#elif defined(DIVX_DRM_51)

    CHAR s_registrationCodeString[DRM_REGISTRATION_CODE_BYTES] = {0};
    drmErrorCodes_t e_errorCode;
    UINT32 ui4_drmContextLength = 0;
    UINT8*pui1_drmContext = NULL;

    if (DRM_DEVICE_IS_ACTIVATED == drmIsDeviceActivated())
    {
        DBG_INFO((_INFO_HEADER"%s: You must deactivate your device before you may re-register.\n", __FUNCTION__));
        
	    return DRMR_OK;
    }

    e_errorCode = drmInitSystem(NULL, (uint32_t*)&ui4_drmContextLength);
    if (DRM_SUCCESS != e_errorCode)
    {
        return DRMR_INTERNAL_ERROR;        
    }

    pui1_drmContext = (UINT8*) x_mem_alloc(ui4_drmContextLength);
    if (NULL == pui1_drmContext)
    {
        return DRMR_INTERNAL_ERROR;
    }

    e_errorCode = drmInitSystem(pui1_drmContext, (uint32_t*)&ui4_drmContextLength);
    if (DRM_SUCCESS != e_errorCode)
    {
        x_mem_free(pui1_drmContext);
        
        return DRMR_INTERNAL_ERROR;        
    }

    e_errorCode = drmGetRegistrationCodeString(pui1_drmContext, s_registrationCodeString);
    if (DRM_SUCCESS != e_errorCode)
    {     
        DBG_ERROR((_ERROR_HEADER"%s: Get registration code failed, Code %d\n", __FUNCTION__, drmGetLastError(pui1_drmContext)));
        printk("Get registration code fail\n");
        
        x_mem_free(pui1_drmContext);

        return DRMR_INTERNAL_ERROR;
    }

    DBG_INFO((_INFO_HEADER"%s: Registration Code %s\n", __FUNCTION__, s_registrationCodeString));

    /* fill data, registration code */
    if (NULL != ps_registration_code)
    {
        x_strncpy(ps_registration_code, s_registrationCodeString, DRM_REGISTRATION_CODE_BYTES);
    }

    x_mem_free(pui1_drmContext);

    return DRMR_OK;    
#endif
}

/*-----------------------------------------------------------------------------
 * Name: divxDrmGetDeactivationCode
 *
 * Description: This API retrives the divx drm deactivation code.
 *
 * Inputs: ps_registration_code  Specify pointer to retrive the deactivation code.
 *
 * Outputs: ps_registration_code  Specify pointer contains the deactivation code.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_INTERNAL_ERROR  Fail.
 *          DRMR_FCT_NOT_SUPPORT  Fail.  
 ----------------------------------------------------------------------------*/
INT32 divxDrmGetDeactivationCode (
    CHAR* ps_deactivation_code)
{
#ifdef DIVX_DRM_51
    CHAR s_deactivationCode[9];
    drmErrorCodes_t e_errorCode;
    UINT32 ui4_drmContextLength;
    UINT8* pui1_drmContext = NULL;

    e_errorCode = drmInitSystem(NULL, (uint32_t *)&ui4_drmContextLength);
    if (DRM_SUCCESS != e_errorCode)
    {
        return DRMR_INTERNAL_ERROR;        
    }

    pui1_drmContext = (UINT8*) x_mem_alloc(ui4_drmContextLength);
    if (NULL == pui1_drmContext)
    {
        return DRMR_INTERNAL_ERROR;
    }

    e_errorCode = drmInitSystem(pui1_drmContext, (uint32_t *)&ui4_drmContextLength);
    if (DRM_SUCCESS != e_errorCode)
    {
        x_mem_free(pui1_drmContext);

        return DRMR_INTERNAL_ERROR;        
    }

    e_errorCode = drmGetDeactivationCodeString(pui1_drmContext, s_deactivationCode);
    if ( DRM_SUCCESS != e_errorCode )
    {
        DBG_ERROR((_ERROR_HEADER"%s: Get deactivation code failed, Code %d\n", __FUNCTION__, drmGetLastError(pui1_drmContext)));
        printk("Get deactivation code fail\n");
        
        x_mem_free(pui1_drmContext);

        return DRMR_INTERNAL_ERROR;
    }

    DBG_INFO((_INFO_HEADER"%s: Deactivation Code %s\n", __FUNCTION__, s_deactivationCode));

    e_errorCode = drmFinalizePlayback(pui1_drmContext);
    if (DRM_SUCCESS != e_errorCode)
    {
        x_mem_free(pui1_drmContext);

        return DRMR_INTERNAL_ERROR;        
    }

    /* fill data, deactivation code */
    if (NULL != ps_deactivation_code)
    {
        x_strncpy(ps_deactivation_code, s_deactivationCode, 9);
    }

    x_mem_free(pui1_drmContext);

    return DRMR_OK;
#else

    return DRMR_FCT_NOT_SUPPORT;
#endif
}

/*-----------------------------------------------------------------------------
 * Name: divxDrmDecryptNInit
 *
 * Description: This API initializes divx drm for playback and retrives the output signal protection information.
 *
 * Inputs: pt_divx_drm_obj  Specify divx drm object pointer to retrive the divx drm info.
 *         pui1_drm_chunk  Specify the strd chunk.
 *
 * Outputs: pt_divx_drm_obj  Specify divx drm object pointer contains the divx drm info.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_INTERNAL_ERROR  Fail.
 ----------------------------------------------------------------------------*/
INT32 divxDrmDecryptNInit (
    DIVX_DRM_OBJ_T* pt_divx_drm_obj, 
    UINT8* pui1_drm_chunk)
{
#ifdef DIVX_DRM_40
    UINT8 ui1_result;
    UINT8 ui1_rentalMessageFlag;
    UINT8 ui1_useLimit;
    UINT8 ui1_useCount;
    UINT8 ui1_cgmsaSignal;
    UINT8 ui1_acptbSignal;
    UINT8 ui1_digitalProtectionSignal;
    AudioCryptoInfo t_audioCryptoInfo;    

    /* check parameters */
    if ((NULL == pt_divx_drm_obj) || (NULL == pui1_drm_chunk) || (NULL == pt_divx_drm_obj->pv_handler_obj))
    {
        DBG_ERROR((_ERROR_HEADER"%s: Screen 2 (not authorized) - Step 0.1\n",__FUNCTION__));
        printk("Screen 2 (not authorized)Step 0.1\n");

        return DRMR_INTERNAL_ERROR;
    }

    ui1_result = (UINT8)drmInitPlaybackContext(pui1_drm_chunk, pt_divx_drm_obj->pv_handler_obj);
    if (ADP_SUCCESS != ui1_result)
    {
        DBG_INFO((_INFO_HEADER"%s: Screen 2 (not authorized), code %d - Step 1\n", __FUNCTION__, drmAdpGetLastError()));

        /* fill data, auth error */
        pt_divx_drm_obj->t_divx_drm_basic_info.ui1_flag |= DIVX_DRM_AUTH_ERROR;

        return DRMR_OK;
    }

    ui1_result = (UINT8)drmQueryRentalStatus(pt_divx_drm_obj->pv_handler_obj, &ui1_rentalMessageFlag, &ui1_useLimit, &ui1_useCount);
    if (ADP_SUCCESS != ui1_result)
    {
        if (ADP_RENTAL_EXPIRED == ui1_result)
        {
            DBG_INFO((_INFO_HEADER"%s: Screen 3 (rental expired), code %d - Step 2\n", __FUNCTION__, drmAdpGetLastError()));
            
            /* fill data, rental expired */
            pt_divx_drm_obj->t_divx_drm_basic_info.ui1_flag |= DIVX_DRM_RENTAL_EXPIRED;

            return DRMR_OK;            
        } 
        else if (ADP_NOT_AUTHORIZED == ui1_result)
        {
            DBG_INFO((_INFO_HEADER"%s: Screen 2 (not authorized), code %d - Step 2.1\n", __FUNCTION__, drmAdpGetLastError()));
            
            /* fill data, auth error */
            pt_divx_drm_obj->t_divx_drm_basic_info.ui1_flag |= DIVX_DRM_AUTH_ERROR;

            return DRMR_OK;                    
        }
        else
        {
            DBG_ERROR((_ERROR_HEADER"%s: Screen 2 (not authorized), code %d - Step 2.2\n", __FUNCTION__, drmAdpGetLastError()));
            printk("Screen 2 (not authorized)Step 2.2\n");            
            return DRMR_INTERNAL_ERROR;        
        }
    }

    if (DIVX_TRUE == ui1_rentalMessageFlag)
    {
        /* fill data, rental */
        pt_divx_drm_obj->t_divx_drm_basic_info.ui1_flag |= DIVX_DRM_RENTAL;
        pt_divx_drm_obj->t_divx_drm_basic_info.ui1_use_count = ui1_useCount;
        pt_divx_drm_obj->t_divx_drm_basic_info.ui1_use_limit = ui1_useLimit;        
    
        DBG_INFO((_INFO_HEADER"%s: Screen 4 (rental choice) - Step 3\n", __FUNCTION__));
        DBG_INFO((_INFO_HEADER"%s: You have used %d out of %d views.\n", __FUNCTION__, ui1_useCount, ui1_useLimit));
    }

    ui1_result = (UINT8)drmQueryCgmsa(pt_divx_drm_obj->pv_handler_obj, &ui1_cgmsaSignal);
    if (ADP_SUCCESS != ui1_result)
    {
        DBG_ERROR((_ERROR_HEADER"%s: Screen 2 (not authorized), code %d - Step 4\n", __FUNCTION__, drmAdpGetLastError()));
        printk("Screen 2 (not authorized) Step 4\n");            
        
        return DRMR_INTERNAL_ERROR;
    }
    DBG_INFO((_INFO_HEADER"%s: %d cgmsa signal\n", __FUNCTION__, ui1_cgmsaSignal));

    ui1_result = (UINT8)drmQueryAcptb(pt_divx_drm_obj->pv_handler_obj, &ui1_acptbSignal);
    if (ADP_SUCCESS != ui1_result)
    {
        DBG_ERROR((_ERROR_HEADER"%s: Screen 2 (not authorized), code %d - Step 5\n", __FUNCTION__, drmAdpGetLastError()));
        printk("Screen 2 (not authorized) Step 5\n");            
        
        return DRMR_INTERNAL_ERROR;
    }
    DBG_INFO((_INFO_HEADER"%s: %d acptb signal\n", __FUNCTION__, ui1_acptbSignal));

    ui1_result = (UINT8)drmQueryDigitalProtection(pt_divx_drm_obj->pv_handler_obj, &ui1_digitalProtectionSignal);
    if (ADP_SUCCESS != ui1_result)
    {
        DBG_ERROR((_ERROR_HEADER"%s: Screen 2 (not authorized), code %d - Step 6\n", __FUNCTION__, drmAdpGetLastError()));
         printk("Screen 2 (not authorized) Step 6\n");            
        
        return DRMR_INTERNAL_ERROR;
    }
    DBG_INFO((_INFO_HEADER"%s: %d digital protection signal\n", __FUNCTION__, ui1_digitalProtectionSignal));

    /* fill data, output signal protection */
    pt_divx_drm_obj->t_drm_output_signal_protection_info.ui1_cgmsa_flag = ui1_cgmsaSignal;
    pt_divx_drm_obj->t_drm_output_signal_protection_info.ui1_acptb_flag = ui1_acptbSignal;
    pt_divx_drm_obj->t_drm_output_signal_protection_info.ui1_digital_protection_flag = ui1_digitalProtectionSignal;

    ui1_result = (UINT8)drmCommitPlayback(pt_divx_drm_obj->pv_handler_obj);
    if (ADP_NOT_AUTHORIZED == ui1_result)
    {
        DBG_ERROR((_ERROR_HEADER"%s: Screen 2 (not authorized), code %d - Step 7\n", __FUNCTION__, drmAdpGetLastError()));
        printk("Screen 2 (not authorized) Step 7\n");            
        
        /* fill data, auth error */
        pt_divx_drm_obj->t_divx_drm_basic_info.ui1_flag |= DIVX_DRM_AUTH_ERROR;
        
        return DRMR_OK;
    }
    else if (ADP_RENTAL_EXPIRED == ui1_result)
    {
        DBG_ERROR((_ERROR_HEADER"%s: Screen 3 (rental expired), code %d - Step 7.1\n",__FUNCTION__, drmAdpGetLastError()));
        printk("Screen 3 (rental expired) Step 7.1\n");            
        
        /* fill data, rental expired */
        pt_divx_drm_obj->t_divx_drm_basic_info.ui1_flag |= DIVX_DRM_RENTAL_EXPIRED;       
        
        return DRMR_OK;        
    }

    /* fill data, frame key */
    ui1_result = (UINT8)drmQueryFrameKeys(pt_divx_drm_obj->pv_handler_obj, &(pt_divx_drm_obj->t_divx_drm_crypto_info.ui1_frame_keys_count), (UINT8 *) pt_divx_drm_obj->t_divx_drm_crypto_info.aui1_frame_keys);
    if (ADP_SUCCESS != ui1_result)
    {
        DBG_ERROR((_ERROR_HEADER"%s: Screen 2 (not authorized), code %d - Step 8\n", __FUNCTION__, drmAdpGetLastError()));
        printk("Screen 2 (not authorized) Step 8\n");            

        return DRMR_INTERNAL_ERROR;
    }
    
    ui1_result = (UINT8)drmQueryAudioProtection(pt_divx_drm_obj->pv_handler_obj, &t_audioCryptoInfo);
    if (ADP_SUCCESS != ui1_result)
    {
        DBG_ERROR((_ERROR_HEADER"%s: Screen 2 (not authorized), code %d - Step 9\n", __FUNCTION__, drmAdpGetLastError()));
        printk("Screen 2 (not authorized) Step 9\n");            

        return DRMR_INTERNAL_ERROR;
    }

    /* fill data, audio protection */
    if (PROTECTED_AUDIO == t_audioCryptoInfo.flag)
    {
        pt_divx_drm_obj->t_divx_drm_crypto_info.ui1_flag |= DIVX_DRM_AUDIO_PROTECTION;
        pt_divx_drm_obj->t_divx_drm_crypto_info.ui1_protected_audio_offset = (UINT8)t_audioCryptoInfo.offset;
        pt_divx_drm_obj->t_divx_drm_crypto_info.ui1_protected_audio_crypto_size = (UINT8)t_audioCryptoInfo.size;
    }

    /* auth succeeded */
    return DRMR_OK;
#elif defined(DIVX_DRM_51)    

    drmErrorCodes_t e_errorCode;
    UINT8 ui1_rentalMessageFlag;
    UINT8 ui1_useLimit;
    UINT8 ui1_useCount;
    UINT8 ui1_cgmsaSignal;
    UINT8 ui1_acptbSignal;
    UINT8 ui1_digitalProtectionSignal;
    UINT32 ui4_drmContextLength = 0;
    audioCryptoInfo_t t_audioCryptoInfo;
    UINT8* pui1_drmContext;

    /* check parameters */
    if ((NULL == pt_divx_drm_obj) || (NULL == pui1_drm_chunk) || (NULL == pt_divx_drm_obj->pv_handler_obj))
    {
        DBG_ERROR((_ERROR_HEADER"%s: Screen 2 (not authorized) - Step 0.1\n",__FUNCTION__));
        printk("Screen 2 (not authorized) Step 0.1\n");            

        return DRMR_INTERNAL_ERROR;
    }

    ui4_drmContextLength = drmGetDrmContextSize();
    pui1_drmContext = (UINT8*)(pt_divx_drm_obj->pv_handler_obj);
    e_errorCode = drmInitSystem(pui1_drmContext, (uint32_t *)&ui4_drmContextLength);
    if( DRM_SUCCESS != e_errorCode )
    {
        DBG_ERROR((_ERROR_HEADER"%s: Screen 2 (not authorized), code %d  - Step 0.2\n",__FUNCTION__, drmGetLastError(pui1_drmContext)));
        printk("Screen 2 (not authorized) Step 0.2\n");            

        return DRMR_INTERNAL_ERROR;
    }

    e_errorCode = drmInitPlayback(pui1_drmContext, pui1_drm_chunk);
    if (DRM_SUCCESS != e_errorCode)
    {
        DBG_INFO((_INFO_HEADER"%s: Screen 2 (not authorized), code %d - Step 1\n", __FUNCTION__, drmGetLastError(pui1_drmContext)));
        
        /* fill data, auth error */
        pt_divx_drm_obj->t_divx_drm_basic_info.ui1_flag |= DIVX_DRM_AUTH_ERROR;
        
        return DRMR_OK;
    }

    e_errorCode = drmQueryRentalStatus(pui1_drmContext, &ui1_rentalMessageFlag, &ui1_useLimit, &ui1_useCount);
    if (DRM_SUCCESS != e_errorCode)
    {
        if (DRM_RENTAL_EXPIRED == e_errorCode)
        {
            DBG_INFO((_INFO_HEADER"%s: Screen 3 (rental expired), code %d - Step 2\n", __FUNCTION__, drmGetLastError(pui1_drmContext)));
            
            /* fill data, rental expired */
            pt_divx_drm_obj->t_divx_drm_basic_info.ui1_flag |= DIVX_DRM_RENTAL_EXPIRED;

            return DRMR_OK;            
        } 
        else if (DRM_NOT_AUTHORIZED == e_errorCode)
        {
            DBG_INFO((_INFO_HEADER"%s: Screen 2 (not authorized), code %d - Step 2.1\n", __FUNCTION__, drmGetLastError(pui1_drmContext)));
            
            /* fill data, auth error */
            pt_divx_drm_obj->t_divx_drm_basic_info.ui1_flag |= DIVX_DRM_AUTH_ERROR;

            return DRMR_OK;                    
        }
        else
        {
            DBG_ERROR((_ERROR_HEADER"%s: Screen 2 (not authorized), code %d - Step 2.2\n", __FUNCTION__, drmGetLastError(pui1_drmContext)));
            printk("Screen 2 (not authorized) Step 2.2\n");            
            
            return DRMR_INTERNAL_ERROR;        
        }
    }

    if (DIVX_TRUE == ui1_rentalMessageFlag)
    {
        /* fill data, rental */
        pt_divx_drm_obj->t_divx_drm_basic_info.ui1_flag |= DIVX_DRM_RENTAL;
        pt_divx_drm_obj->t_divx_drm_basic_info.ui1_use_count = ui1_useCount;
        pt_divx_drm_obj->t_divx_drm_basic_info.ui1_use_limit = ui1_useLimit;        

        DBG_INFO((_INFO_HEADER"%s: Screen 4 (rental choice) - Step 3\n", __FUNCTION__));
        DBG_INFO((_INFO_HEADER"%s: You have used %d out of %d views.\n", __FUNCTION__, ui1_useCount, ui1_useLimit));
    }

    e_errorCode = drmQueryCgmsa(pui1_drmContext, &ui1_cgmsaSignal);
    if (DRM_SUCCESS != e_errorCode)
    {
        DBG_ERROR((_ERROR_HEADER"%s: Screen 2 (not authorized), code %d - Step 4\n", __FUNCTION__, drmGetLastError(pui1_drmContext)));
        printk("Screen 2 (not authorized) Step 4\n");            
        
        return DRMR_INTERNAL_ERROR;
    }
    DBG_INFO((_INFO_HEADER"%s: %d cgmsa signal\n", __FUNCTION__, ui1_cgmsaSignal));

    e_errorCode = drmQueryAcptb(pui1_drmContext, &ui1_acptbSignal);
    if (DRM_SUCCESS != e_errorCode)
    {
        DBG_ERROR((_ERROR_HEADER"%s: Screen 2 (not authorized), code %d - Step 5\n", __FUNCTION__, drmGetLastError(pui1_drmContext)));
        printk("Screen 2 (not authorized) Step 5\n");            
        
        return DRMR_INTERNAL_ERROR;
    }
    DBG_INFO((_INFO_HEADER"%s: %d acptb signal\n", __FUNCTION__, ui1_acptbSignal));

    e_errorCode = drmQueryDigitalProtection(pui1_drmContext, &ui1_digitalProtectionSignal);
    if (DRM_SUCCESS != e_errorCode)
    {
        DBG_ERROR((_ERROR_HEADER"%s: Screen 2 (not authorized), code %d - Step 6\n", __FUNCTION__, drmGetLastError(pui1_drmContext)));
        printk("Screen 2 (not authorized) Step 6\n");            
        
        return DRMR_INTERNAL_ERROR;
    }
    DBG_INFO((_INFO_HEADER"%s: %d digital protection signal\n", __FUNCTION__, ui1_digitalProtectionSignal));

    /* fill data, output signal protection */
    pt_divx_drm_obj->t_drm_output_signal_protection_info.ui1_cgmsa_flag = ui1_cgmsaSignal;
    pt_divx_drm_obj->t_drm_output_signal_protection_info.ui1_acptb_flag = ui1_acptbSignal;
    pt_divx_drm_obj->t_drm_output_signal_protection_info.ui1_digital_protection_flag = ui1_digitalProtectionSignal;

    e_errorCode = drmCommitPlayback(pui1_drmContext);
    if (DRM_NOT_AUTHORIZED == e_errorCode)
    {
        DBG_ERROR((_ERROR_HEADER"%s: Screen 2 (not authorized), code %d - Step 7\n", __FUNCTION__, drmGetLastError(pui1_drmContext)));
        printk("Screen 2 (not authorized) code %d  Step 7\n", drmGetLastError(pui1_drmContext));            
        
        /* fill data, auth error */
        pt_divx_drm_obj->t_divx_drm_basic_info.ui1_flag |= DIVX_DRM_AUTH_ERROR;
        
        return DRMR_OK;
    }
    else if (DRM_RENTAL_EXPIRED == e_errorCode)
    {
        DBG_ERROR((_ERROR_HEADER"%s: Screen 3 (rental expired), code %d - Step 7.1\n",__FUNCTION__, drmGetLastError(pui1_drmContext)));
        printk("Screen 2 (not authorized) Step 7.1\n");            

        /* fill data, rental expired */
        pt_divx_drm_obj->t_divx_drm_basic_info.ui1_flag |= DIVX_DRM_RENTAL_EXPIRED;       
        
        return DRMR_OK;        
    }

    /* fill data, frame key */
    e_errorCode = drmQueryFrameKeys(pui1_drmContext, &(pt_divx_drm_obj->t_divx_drm_crypto_info.ui1_frame_keys_count), (UINT8 *) pt_divx_drm_obj->t_divx_drm_crypto_info.aui1_frame_keys);
    if (DRM_SUCCESS != e_errorCode)
    {
        DBG_ERROR((_ERROR_HEADER"%s: Screen 2 (not authorized), code %d - Step 8\n", __FUNCTION__, drmGetLastError(pui1_drmContext)));
         printk("Screen 2 (not authorized) Step 8\n");            
        
        return DRMR_INTERNAL_ERROR;
    }

    e_errorCode = drmQueryAudioProtection(pui1_drmContext, &t_audioCryptoInfo);
    if (DRM_SUCCESS != e_errorCode)
    {
        DBG_ERROR((_ERROR_HEADER"%s: Screen 2 (not authorized), code %d - Step 9\n", __FUNCTION__, drmGetLastError(pui1_drmContext)));
         printk("Screen 2 (not authorized) Step 9\n");            
        
        return DRMR_INTERNAL_ERROR;
    }

    /* fill data, audio protection */
    if (PROTECTED_AUDIO == t_audioCryptoInfo.flag)
    {
        pt_divx_drm_obj->t_divx_drm_crypto_info.ui1_flag |= DIVX_DRM_AUDIO_PROTECTION;
        pt_divx_drm_obj->t_divx_drm_crypto_info.ui1_protected_audio_offset = (UINT8)t_audioCryptoInfo.offset;
        pt_divx_drm_obj->t_divx_drm_crypto_info.ui1_protected_audio_crypto_size = (UINT8)t_audioCryptoInfo.size;
    }

    /* auth succeeded */
    return DRMR_OK;    
#endif
}

/*-----------------------------------------------------------------------------
 * Name: divxDrmCommitPlayback
 *
 * Description: This API commits the playback operation and retrives the crypto information.
 *
 * Inputs: pt_divx_drm_obj  Specify the divx drm object.
 *
 * Outputs: NONE.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_INTERNAL_ERROR  Fail.
 *          DRMR_RENTAL_EXPIRED  Fail. 
 ----------------------------------------------------------------------------*/
INT32 divxDrmCommitPlayback (
    DIVX_DRM_OBJ_T* pt_divx_drm_obj)
{
#ifdef DIVX_DRM_40
    UINT8 ui1_result;
    DrmMemory* pt_drm_memory;
    PlaybackContext* pt_playback_context;

    /* check parameters */
    if ((NULL == pt_divx_drm_obj) || (NULL == pt_divx_drm_obj->pv_handler_obj))
    {
        return DRMR_INTERNAL_ERROR;
    }

    pt_playback_context = (PlaybackContext*)(pt_divx_drm_obj->pv_handler_obj);

    pt_drm_memory = &(pt_playback_context->t_drm_memory);

    if ((DIVX_TRUE == pt_playback_context->b_save_drm_memory))
    {
        ui1_result = (UINT8)drmIncreaseRental(pt_playback_context);
        if (ADP_RENTAL_EXPIRED == ui1_result)
        {
            DBG_ERROR((_ERROR_HEADER"%s: Screen 3 (rental expired)\n",__FUNCTION__));
            printk("Screen 3 (rental expired)\n");            
            
            /* fill data, rental expired */
            pt_divx_drm_obj->t_divx_drm_basic_info.ui1_flag |= DIVX_DRM_RENTAL_EXPIRED;

            return DRMR_RENTAL_EXPIRED;
        }

        /* fill data, increase use count */
        ++pt_divx_drm_obj->t_divx_drm_basic_info.ui1_use_count;

        DBG_INFO((_INFO_HEADER"%s: Save DRM memory\n", __FUNCTION__));    

        ui1_result = (UINT8)saveDrmMemory(pt_drm_memory);
        if (ADP_SUCCESS != ui1_result)
        {
            DBG_ERROR((_ERROR_HEADER"%s: Save DRM memory failed\n", __FUNCTION__));
            printk("Save DRM memory failed\n");            

            return DRMR_INTERNAL_ERROR;
        }
    }
    
    return DRMR_OK;
#elif defined(DIVX_DRM_51)    

    drmErrorCodes_t e_errorCode;
    drmMemory_t* pt_drm_memory;
    playbackContext_t* pt_playback_context;

    /* check parameters */
    if ((NULL == pt_divx_drm_obj) || (NULL == pt_divx_drm_obj->pv_handler_obj))
    {
        return DRMR_INTERNAL_ERROR;
    }

    pt_playback_context = (playbackContext_t*)(pt_divx_drm_obj->pv_handler_obj);

    pt_drm_memory = &(pt_playback_context->t_drm_memory);

    if ((DIVX_TRUE == pt_playback_context->b_save_drm_memory))
    {
        e_errorCode = drmIncreaseRental(pt_playback_context);
        if (DRM_RENTAL_EXPIRED == e_errorCode)
        {
            DBG_ERROR((_ERROR_HEADER"%s: Screen 3 (rental expired)\n",__FUNCTION__));
            printk("Screen 3 (rental expired)\n");
            /* fill data, rental expired */
            pt_divx_drm_obj->t_divx_drm_basic_info.ui1_flag |= DIVX_DRM_RENTAL_EXPIRED;

            return DRMR_RENTAL_EXPIRED;
        }

        /* fill data, increase use count */
        ++pt_divx_drm_obj->t_divx_drm_basic_info.ui1_use_count;

        DBG_INFO((_INFO_HEADER"%s: Save DRM memory\n", __FUNCTION__));    

        e_errorCode = s_saveDrmMemory(pt_drm_memory);
        if (0 != e_errorCode)            
        {
            DBG_ERROR((_ERROR_HEADER"%s: Save DRM memory failed\n", __FUNCTION__));
            printk("Save DRM memory failed\n");

            return DRMR_INTERNAL_ERROR;
        }
    }

    return DRMR_OK;
#endif
}

