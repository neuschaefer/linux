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
 * $RCSfile: svl_cb_fct.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file implements SVL callback functions
 *    
 *   
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "os/inc/x_os.h"
#include "cdb/cdb_api.h"
#include "svl/svl.h"
#include "svl/_svl.h"


/*-------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/


/**********************************************************************
   Following are callback functions for the SVL library.
***********************************************************************/

/*-----------------------------------------------------------------------------
 * Name: svl_handle_free_fct
 *
 * Description: This function is called whenever a SVL database client handle
 *              is freed.
 *
 * Inputs:  h_obj         Contains the handle to core db client.
 *          e_type        Contains the core db client handle type and must be 
 *                        set to SVLT_SERVICE_LIST.
 *          pv_obj        References the client object structure.
 *          pv_tag        Ignored.
 *          b_req_handle  Is set to TRUE if the handle free was initiated with
 *                        this handle else FALSE.
 *
 * Outputs: None
 *           
 * Returns: -
 *    TRUE:  handle and its control block is freed.
 *    FALSE: Don't free the handle.
 ----------------------------------------------------------------------------*/
BOOL svl_handle_free_fct (
    HANDLE_T       h_obj,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle
)
{
    SVL_OBJ_T* pt_svl ;
    
    /* If no client database object  has been pass into this
       simply return FALSE . */
    if ( ( pt_svl = ( (SVL_OBJ_T*) pv_obj ) ) != NULL ) 
    {
        /* Abort if this is not the correct type. */
        if (e_type == SVLT_SERVICE_LIST )
        {
            /*
              Free the CDB handle and its control structure
            */
            if ( pt_svl->h_cdb != NULL_HANDLE )
            {
                handle_free(pt_svl->h_cdb, TRUE);
            }
            
            x_mem_free(pt_svl);
            
            return TRUE;
        }
        else
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: svl_nfy_fct
 *
 * Description: This API is registered with the CoreDB database object.
 *              It acts as an intermediate to invoke SVL client's
 *              notification function.
 *
 *              When this function is called by the CoreDB database object,
 *              it needs to get the address of the SVL client object. 
 *              The SVL client object structure contains the address of
 *              the notification function for the SVL client.  Using
 *              that address, this function then call the SVL client
 *              notification fucntion.
 *
 * Inputs:  h_client:  CDB handle
 *          e_cond:    condition SVL object of call back.
 *          ui4_reason:  Reason for the callback.
 *          ui4_db_version: The version id of the CoreDB object.
 *                         
 *
 * Outputs: 
 *
 * Returns: HP_NEXT or HP_BREAK.
 ----------------------------------------------------------------------------*/
VOID svl_nfy_fct(
    HANDLE_T	h_client,            /* handle to the client of CoreDB. */
    DB_COND_T   e_cond,
    UINT32		ui4_reason,
    VOID*       pv_tag,
    UINT32		ui4_db_version
)
{
    HANDLE_TYPE_T     pe_type = INV_HANDLE_TYPE;
    SVL_OBJ_T*        pt_svl;
    SVL_COND_T        e_svl_cond;
    HANDLE_T          h_svl_client;
    UINT32            ui4_svl_reason;
    x_svl_nfy_fct     pf_svl_nfy_fct;
    
    /*
       The private tag value passed in to this function
       must not be a NULL value.
    */
    if (     
         (pv_tag != NULL)
        )
    {
        /* cast pv_tag to the address of SVL client object. */
        pt_svl=(SVL_OBJ_T*) pv_tag;

        /* Check if the client handle passed into this function
           is really CDB client for this SVL control block.
           Note: we can only do this check, because if the notification
           reason is that 'CDB is being deleted', then the handle to the
           CDB is no longer valid by the time we get here, so we can
           not use x_handle_get_type(...h_client...) to check for
           valid handle.
        */
        if ( pt_svl->h_cdb == h_client && pt_svl->pf_nfy_fct != NULL )
        {
            /* Get the handle to the svl object. */
            h_svl_client = pt_svl->h_svl;

            /* Get the private tag for the SVL handle. */
            if ( (h_svl_client != NULL_HANDLE)                           &&
                 (x_handle_get_type( h_svl_client, &pe_type) == HR_OK )  &&
                 (pe_type == SVLT_SERVICE_LIST )                
                )
            {
                /*
                  Get the SVL client tag.
                */
                if (x_handle_get_tag(h_svl_client,((VOID**) &pv_tag))
                    != HR_OK)
                {
                    pv_tag=NULL;
                }
                /* set the condition of the database object
                   for callback. */
                if ( e_cond == DB_UPDATING )
                {
                    e_svl_cond = SVL_UPDATING;
                }
                else if ( e_cond == DB_UPDATED )
                {
                    e_svl_cond = SVL_UPDATED;
                }
                else if ( e_cond == DB_CLOSED )
                {
                    e_svl_cond = SVL_CLOSED;
                }
                else
                {
                    e_svl_cond = SVL_UNKNOWN;
                }
                
        
                ui4_svl_reason = 0;
        
                /* Get the reason for the notification. */
                if ( ui4_reason & DB_REASON_UNK )
                {
                    ui4_svl_reason = ui4_svl_reason | SVL_REASON_UNKNOWN;
                }
                if ( ui4_reason & DB_RECORD_ADD )
                {
                    ui4_svl_reason = ui4_svl_reason | SVL_RECORD_ADD;
                }
                if ( ui4_reason & DB_RECORD_DEL )
                {
                    ui4_svl_reason = ui4_svl_reason | SVL_RECORD_DEL;
                }
                if ( ui4_reason & DB_RECORD_MOD )
                {
                    ui4_svl_reason = ui4_svl_reason | SVL_RECORD_MOD;
                }

                /* Save the notification function pointer */
                pf_svl_nfy_fct=pt_svl->pf_nfy_fct;
                        
                /* If the reason for the notification is that SVL
                   is shutting down, then we need to invalidate all
                   other handles to this SVL
                */
                if ( e_svl_cond == SVL_CLOSED )
                {
                    /* Free the SVL handle. */
                    handle_free(h_svl_client, TRUE);
                }
                
                /* Notify the SVL client that the SVL's records
                   have changed or it has been deleted. */
                pf_svl_nfy_fct(h_svl_client,
                               e_svl_cond,
                               ui4_svl_reason,
                               pv_tag,
                               ui4_db_version);
            }
            else
            {
#ifdef DEBUG
                if ( (h_svl_client != NULL_HANDLE) &&
                     x_handle_valid(h_svl_client)
                    )
                {
                    ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
                }
#endif
                /* Invalid SVL handle or NULL notification function,
                   nothing we can do here, so just return. */
                return;
            }
        }
        else
        {
            /* Invalid CDB handle, nothing we can do here, so just return. */
            return;
        }
    }
    return;
}
