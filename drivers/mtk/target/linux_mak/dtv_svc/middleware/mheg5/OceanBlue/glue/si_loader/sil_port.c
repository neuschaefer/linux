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
#include "inc/u_common.h"
#include "os/inc/x_os.h"

#include "mheg5/OceanBlue/glue/si_loader/sil.h"

/* MHEG-5 stack header files */
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_types.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_errors.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_si.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_tune.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define MHEG5_DBG_APP_SI(x)    \
    do {                        \
        if (_b_dbg_msg_si)     \
        {                       \
            MHEG5_DBG_APP(x);   \
        }                       \
    } while (0)
    

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
static BOOL                           _b_dbg_msg_si                  = TRUE;
static x_mheg5_tune_svc_fct           _pf_tune_svc_fct               = NULL;
static x_mheg5_tune_method_update_nfy _pf_tune_method_update_nfy     = NULL;
static VOID*                          _pv_tune_svc_fct_tag           = NULL;
static VOID*                          _pv_tune_method_update_nfy_tag = NULL;

/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
tmMHEG5Err_t tmMHEG5AckData(U8BIT* pData)
{
    tmMHEG5Err_t t_err;
    
    MHEG5_DBG_APP_SI((
        "{MHEG5 Stack} %s: pData = %d\r\n",
        __FUNCTION__,
        (UINT32) pData));
    
    t_err = MHEG5_ERR_BAD_PARAMETER;

    if (pData)
    {
        x_mem_free(pData);
        t_err = MHEG5_OK;
    }

    return (t_err);
}

tmMHEG5Err_t tmMHEG5GetBootInfo (
    U8BIT** pBootInfo, 
    U32BIT* pBootInfoLength)
{
    #define MAX_NB_INFO_LEN     253
    
    tmMHEG5Err_t    t_err;
    
    MHEG5_DBG_APP_SI((
        "{MHEG5 Stack} %s: pBootInfo = %d, pBootInfoLength = %d\r\n",
        __FUNCTION__,
        (UINT32) pBootInfo,
        (UINT32) pBootInfoLength));
    
    t_err = MHEG5_ERR_BAD_PARAMETER;

    if (pBootInfo && pBootInfoLength)
    {
        VOID* pv_buf = x_mem_alloc(MAX_NB_INFO_LEN);
        
        t_err = MHEG5_ERR_OTHER;
        
        if (pv_buf)
        {
            if (MHEG5R_OK == sil_get_boot_info(
                MAX_NB_INFO_LEN,
                (UINT8*) pv_buf,
                (UINT32*) pBootInfoLength))
            {
                *pBootInfo = (U8BIT*) pv_buf;
                t_err = MHEG5_OK;
            }
            else
            {
                x_mem_free(pv_buf);
            }
        }
    }
    
    return (t_err);
}

tmMHEG5Err_t tmMHEG5SiQueryStart(
    tmMHEG5SiQuery_t        siQuery,
    tmMHEG5SiQueryRef_t     queryRef,
    void*                   siUserData,
    tmMHEG5SiStatus_t*      pQueryStatus,
    tmMHEG5SiQueryResult_t* pQueryResult,
    void**                  pQueryHandle)
{
    INT32           i4_ret;
    HANDLE_T        h_query;
    tmMHEG5Err_t    t_err;
    
    MHEG5_DBG_APP_SI((
        "{MHEG5 Stack} %s: siQuery.kind = %d, siQuery.service_id = %d, queryRef = %d, siUserData = %d, pQueryStatus = %d, pQueryResult = %d, pQueryHandle = %d\r\n",
        __FUNCTION__,
        (UINT32) siQuery.kind,
        (UINT32) siQuery.service_id,
        (UINT32) queryRef,
        (UINT32) siUserData,
        (UINT32) pQueryStatus,
        (UINT32) pQueryResult,
        (UINT32) pQueryHandle));
    
    t_err = MHEG5_ERR_BAD_PARAMETER;

    if (pQueryStatus && pQueryResult && pQueryHandle)
    {
#ifdef DEBUG
        switch (siQuery.kind)
        {
            case MHEG5_SI_PID:
            {
                MHEG5_DBG_APP_SI((
                    "New SI query: SiQuery {kind=MHEG5_SI_PID, service_id=%d, association_tag=%d}\r\n",
                    siQuery.service_id,
                    siQuery.data.association_tag));
                
                break;
            }
            
            case MHEG5_SI_BOOT_CAROUSEL:
            {
                MHEG5_DBG_APP_SI((
                    "New SI query: SiQuery {kind=MHEG5_SI_BOOT_CAROUSEL, service_id=%d}\r\n",
                    siQuery.service_id));
                                
                break;
            }
    
            case MHEG5_SI_CAROUSEL:
            {
                MHEG5_DBG_APP_SI((
                    "New SI query: SiQuery {kind=MHEG5_SI_CAROUSEL, service_id=%d, carousel_id = %d}\r\n",
                    siQuery.service_id,
                    siQuery.data.carousel_id));
                                
                break;
            }
    
            case MHEG5_SI_PIDLIST:
            {
                MHEG5_DBG_APP_SI((
                    "New SI query: SiQuery {kind=MHEG5_SI_PIDLIST, service_id=%d}\r\n",
                    siQuery.service_id));
    
                break;
            }
    
            case MHEG5_SI_DEFERRED_SERVICE:
            {
                MHEG5_DBG_APP_SI((
                    "New SI query: SiQuery {kind=MHEG5_SI_DEFERRED_SERVICE, service_id=%d, association_tag = %d}\r\n",
                    siQuery.service_id,
                    siQuery.data.association_tag));
                
                
                break;
            }
            
            default:
                break;
        }
#endif
        
        i4_ret = sil_query(
            &siQuery,
            queryRef,
            (VOID*) siUserData,
            pQueryResult,
            &h_query);

    #ifdef DEBUG
        switch (siQuery.kind)
        {
            case MHEG5_SI_PID:
            {
                if (MHEG5R_OK == i4_ret)
                {
                    MHEG5_DBG_APP_SI((
                        "SiQueryResult {kind=%s, PID=%d}\r\n",
                        "MHEG5_SI_PID",
                        pQueryResult->data.PID));
                }
                
                break;
            }
            
            case MHEG5_SI_BOOT_CAROUSEL:
            {
                if (MHEG5R_OK == i4_ret)
                {
                    MHEG5_DBG_APP_SI((
                        "SiQueryResult {kind=%s, PID=%d, component_tag=%d, life_cycle_association_tag=%d}\r\n",
                        "MHEG5_SI_BOOT_CAROUSEL",
                        pQueryResult->data.carouselBootInfo.PID,
                        pQueryResult->data.carouselBootInfo.component_tag,
                        pQueryResult->data.carouselBootInfo.life_cycle_association_tag));
                }
                
                break;
            }

            case MHEG5_SI_CAROUSEL:
            {
                if (MHEG5R_OK == i4_ret)
                {
                    MHEG5_DBG_APP_SI((
                        "SiQueryResult {kind=%s, PID=%d, component_tag=%d, life_cycle_association_tag=%d}\r\n",
                        "MHEG5_SI_CAROUSEL",
                        pQueryResult->data.carouselBootInfo.PID,
                        pQueryResult->data.carouselBootInfo.component_tag,
                        pQueryResult->data.carouselBootInfo.life_cycle_association_tag));
                }
                
                break;
            }

            case MHEG5_SI_PIDLIST:
            {
                if (MHEG5R_OK == i4_ret)
                {
                    INT32 i = 0;

                    for (i=0;i<pQueryResult->data.PIDList.numPIDs;i++)
                    {
                        MHEG5_DBG_APP_SI((
                        "SiQueryResult {kind=%s, PID=%d, component_tag=%d}\r\n",
                        "MHEG5_SI_PIDLIST",
                        pQueryResult->data.PIDList.PIDList[i],
                        pQueryResult->data.PIDList.component_tag[i]));
                    }
                }
                break;
            }

            case MHEG5_SI_DEFERRED_SERVICE:
            {
                if (MHEG5R_OK == i4_ret)
                {
                    MHEG5_DBG_APP_SI((
                        "SiQueryResult {kind=%s, transport_stream_id=%d, service_id=%d, original_network_id=%d}\r\n",
                        "MHEG5_SI_DEFERRED_SERVICE",
                        pQueryResult->data.deferredService.transport_stream_id,
                        pQueryResult->data.deferredService.service_id,
                        pQueryResult->data.deferredService.original_network_id));
                }
                
                break;
            }
            
            default:
                break;
        }
    #endif
    
        if (MHEG5R_OK == i4_ret)
        {
            *pQueryStatus = MHEG5_SI_SUCCESS;
            *pQueryHandle = NULL;

            t_err = MHEG5_OK;
        }
        else if (MHEG5R_ASYNC_RESPONSE == i4_ret)
        {
            *pQueryStatus = MHEG5_SI_PENDING;
            *pQueryHandle = (void*) h_query;

            t_err = MHEG5_OK;
        }
        else
        {
            *pQueryStatus = MHEG5_SI_FAILURE;
            *pQueryHandle = NULL;

            t_err = MHEG5_OK;
        }
    }
    
    return (t_err);
}

tmMHEG5Err_t tmMHEG5SiQueryStop(
    void*               queryHandle,
    tmMHEG5SiQueryRef_t queryRef)
{
    INT32           i4_ret;
    tmMHEG5Err_t    t_err;

    MHEG5_DBG_APP_SI((
        "{MHEG5 Stack} %s: queryHandle = %d, queryRef = %d\r\n",
        __FUNCTION__,
        (UINT32) queryHandle,
        (UINT32) queryRef));
    
    i4_ret = sil_stop_query(
        (HANDLE_T) queryHandle,
        queryRef);

    t_err = ((MHEG5R_OK == i4_ret) ? MHEG5_OK : MHEG5_ERR_BAD_PARAMETER);

    return (t_err);
}

tmMHEG5Err_t tmMHEG5SiSubscribeServiceInfoChanged(U16BIT service_id)
{
    tmMHEG5Err_t    t_err;
    
    MHEG5_DBG_APP_SI((
        "{MHEG5 Stack} %s: service_id = %d\r\n",
        __FUNCTION__,
        (UINT32) service_id));
    
    if (MHEG5R_OK == sil_subscribe_service(service_id, TRUE))
    {
        t_err = MHEG5_OK;
    }
    else
    {
        t_err = MHEG5_ERR_BAD_PARAMETER;
    }
    
    return (t_err);
}

tmMHEG5Err_t tmMHEG5SiUnsubscribeServiceInfoChanged(U16BIT service_id)
{
    tmMHEG5Err_t    t_err;
    
    MHEG5_DBG_APP_SI((
        "{MHEG5 Stack} %s: service_id = %d\r\n",
        __FUNCTION__,
        (UINT32) service_id));
    
    if (MHEG5R_OK == sil_subscribe_service(service_id, FALSE))
    {
        t_err = MHEG5_OK;
    }
    else
    {
        t_err = MHEG5_ERR_BAD_PARAMETER;
    }
    
    return (t_err);
}

tmMHEG5Err_t tmMHEG5GetServiceIndex(
    ptmMHEG5DvbLocator_t    pDvbLocator,
    S32BIT*                 pServiceIndex)
{
    tmMHEG5Err_t    t_err;
    
    MHEG5_DBG_APP_SI((
        "{MHEG5 Stack} %s: pDvbLocator = %d, pServiceIndex = %d\r\n",
        __FUNCTION__,
        (UINT32) pDvbLocator,
        (UINT32) pServiceIndex));
    
    t_err = MHEG5_ERR_BAD_PARAMETER;

    if (pDvbLocator && pServiceIndex)
    {
        UINT32  ui4_svc_index;
        INT32   i4_ret;
        
        MHEG5_DBG_ERROR((
            "{MHEG5 Stack} %s: on_id = %d, ts_id = %d, svc_id = %d\r\n",
            __FUNCTION__,
            (UINT32) pDvbLocator->original_network_id,
            (UINT32) pDvbLocator->transport_stream_id,
            (UINT32) pDvbLocator->service_id));

        i4_ret = sil_get_available_svc_index(
            (UINT16) pDvbLocator->original_network_id,
            (UINT16) pDvbLocator->transport_stream_id,
            (UINT16) pDvbLocator->service_id,
            &ui4_svc_index);
        
        if (MHEG5R_OK == i4_ret)
        {
            *pServiceIndex = (S32BIT) ui4_svc_index;
            t_err = MHEG5_OK;
        }
        else
        {
            *pServiceIndex = -1;
            t_err = MHEG5_OK;
        }
    }
    
    return (t_err);
}

tmMHEG5Err_t tmMHEG5GetBasicSI(
    U32BIT              serviceIndex,
    tmMHEG5BasicSI_t*   pBasicSI)
{
    tmMHEG5Err_t t_err;
    
    MHEG5_DBG_APP_SI((
        "{MHEG5 Stack} %s: serviceIndex = %d, pBasicSI = %d\r\n",
        __FUNCTION__,
        serviceIndex,
        (UINT32) pBasicSI));
    
    t_err = MHEG5_ERR_BAD_PARAMETER;

    if (pBasicSI)
    {
        INT32 i4_ret;
        
        i4_ret = sil_get_basic_si(
            (UINT32) serviceIndex,
            pBasicSI);

        t_err = ((MHEG5R_OK == i4_ret) ? MHEG5_OK : MHEG5_ERR_OTHER);
    }

    return (t_err);
}

tmMHEG5Err_t tmMHEG5GetCurrentService(ptmMHEG5DvbLocator_t locator)
{
    INT32 i4_ret;
    tmMHEG5Err_t t_err;

    MHEG5_DBG_APP_SI((
        "{MHEG5 Stack} %s\r\n",
        __FUNCTION__));

    if (!locator)
    {
        MHEG5_DBG_ERROR(("{MHEG5 Comp} %s: Invalid parameter.\r\n", __FUNCTION__));
        return MHEG5_ERR_BAD_PARAMETER;
    }
    
    i4_ret = sil_get_current_service(locator);
    
    t_err = ((MHEG5R_OK == i4_ret) ? MHEG5_OK : MHEG5_ERR_OTHER);

    return (t_err);
}



tmMHEG5Err_t tmMHEG5TuneIndex(S32BIT serviceIndex, BOOLEAN *pStatus)
{
    INT32 i4_ret;

    MHEG5_DBG_APP_SI((
        "{MHEG5 Stack} %s: serviceIndex = %d\r\n",
        __FUNCTION__,
        serviceIndex));

    if (!pStatus)
    {
        MHEG5_DBG_ERROR(("{MHEG5 Comp} %s: Invalid parameter.\r\n", __FUNCTION__));
        return MHEG5_ERR_BAD_PARAMETER;
    }

    if (_pf_tune_svc_fct != NULL)
    {
        UINT32 ui4_ch_id;

        if (MHEG5R_OK == sil_svc_index_to_ch_id((UINT32) serviceIndex, &ui4_ch_id))
        {        
            i4_ret = _pf_tune_svc_fct((INT32)ui4_ch_id, _pv_tune_svc_fct_tag, (BOOL*)pStatus);

            if (i4_ret == MHEG5R_OK)
            {
                return MHEG5_OK;
            }
        }
    }

    return MHEG5_ERR_OTHER;
}

tmMHEG5Err_t tmMHEG5TuneIndexInfo(U8BIT tuneQuietly)
{
    MHEG5_DBG_APP_SI((
        "{MHEG5 Stack} %s: tuneQuietly = %d\r\n",
        __FUNCTION__,
        tuneQuietly));

    if (tuneQuietly != TUNE_ALL_NORMALLY && tuneQuietly != TUNE_ALL_QUIETLY && tuneQuietly != TUNE_QUIET_THEN_NORMAL && tuneQuietly != TUNE_COMPLETELY_QUIET)
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }

    if (_pf_tune_method_update_nfy != NULL)
    {
        _pf_tune_method_update_nfy((MHEG5_TUNE_METHOD_T)tuneQuietly, _pv_tune_method_update_nfy_tag);
    }

    return MHEG5_OK;
}

tmMHEG5Err_t tmMHEG5GetLCNLocator(
    U32BIT                  lcn, 
    ptmMHEG5DvbLocator_t    pDvbLocator)
{
    tmMHEG5Err_t    t_err;
    
    MHEG5_DBG_APP_SI((
        "{MHEG5 Stack} %s: lcn = %d, pDvbLocator = %d\r\n",
        __FUNCTION__,
        lcn,
        (UINT32) pDvbLocator));
    
    t_err = MHEG5_ERR_BAD_PARAMETER;
    
    if (pDvbLocator)
    {
        if (MHEG5R_OK == sil_get_lcn_locator(
            lcn,
            &(pDvbLocator->original_network_id),
            &(pDvbLocator->transport_stream_id),
            &(pDvbLocator->service_id)))
        {
            t_err = MHEG5_OK;
        }
        else
        {
            t_err = MHEG5_ERR_OTHER;
        }
    }
    
    return (t_err);
}

tmMHEG5Err_t tmMHEG5GetServiceDetails(
    S32BIT               serviceIndex,
    S_ServiceDetails*    details )
{
    x_dbg_stmt("[MHEG5-SIL]INFO: %s(%d,%p)#%d: request current service details\r\n", __FUNCTION__, __LINE__, serviceIndex, details);
    return sil_get_service_details((UINT32)serviceIndex, details);
}

tmMHEG5Err_t tmMHEG5GetEventDetails(
    S32BIT             serviceIndex, 
    BOOLEAN            porf,
    S_EventDetails*    details )
{
    x_dbg_stmt("[MHEG5-SIL]INFO: %s(BOOL %d)#%d: request  event details\r\n", __FUNCTION__, porf, __LINE__);
    return sil_get_event_details((UINT32)serviceIndex, details, (BOOL)porf);
}

void tmMHEG5ReleaseServiceDetails(S_ServiceDetails* details)
{
    x_dbg_stmt("[MHEG5-SIL] %s#%d: release service details\r\n", __FUNCTION__,  __LINE__);
    sil_release_service_details( details );
}

void tmMHEG5ReleaseEventDetails(S_EventDetails* details)
{
    x_dbg_stmt("[MHEG5-SIL] %s#%d: release service details\r\n", __FUNCTION__,  __LINE__);
    sil_release_event_details(details);
}

INT32 sil_reg_tune_method_update_nfy(
    x_mheg5_tune_method_update_nfy pf_nfy, 
    VOID*                          pv_tag)
{
    _pf_tune_method_update_nfy     = pf_nfy;
    _pv_tune_method_update_nfy_tag = pv_tag;
    
    return MHEG5R_OK;
}

INT32 sil_reg_tune_svc_cb_fct(
    x_mheg5_tune_svc_fct pf_fct, 
    VOID*                pv_tag)
{
    _pf_tune_svc_fct     = pf_fct;
    _pv_tune_svc_fct_tag = pv_tag;
    
    return MHEG5R_OK;
}

