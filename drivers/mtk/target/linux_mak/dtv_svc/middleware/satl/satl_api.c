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
 * $RCSfile: satl_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file contains implementation of the exported Satellite List library
 *    (SATL) API's.
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "cdb/cdb_api.h"
#include "satl/_satl.h"
#include "satl/satl_rec.h"
#include "satl/satl_rec_id.h"
#include "satl/satl_fs.h"
#include "satl/satl_fs_union.h"
#include "satl/satl_dbg.h"
#include "satl/satl_search_parm.h"
/*--------------------------------------------------------------------------
                  functions implementation
---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: x_satl_create
 *
 * Description: 
 *
 * Inputs: 
 *   ps_satl_name  Name of the SATL object. 
 *   ui2_satl_id   Unique id for the SATL object.  
 *                 The limit for SATL ID is between 1 and 65535.
 *   e_mem_policy  Memory policy: SATL_MEM_STATIC or SATL_MEM_DYNAMIC
 *   ui2_max_nb_rec Specifies the maximum number of SATL record (for
 *                 SATL_MEM_STATIC), or it specifies number of
 *                 SATL records per memory chunk.
 *   pv_tag        Pointer to a private tag value provided by the 
 *                 caller.  This tag value is passed to the 
 *                 caller's notification function.
 *   pf_nfy        Notification function provided by the caller.  
 *                 This function is called to notify the owner of 
 *                 this SATL handle when the SATL object has been 
 *                 modified.  If NULL, then no notification function 
 *                 will be called.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          SATL object is created successfully.
 *   SATLR_INV_ARG     'ui2_satl_id' is 0, or ph_satl is NULL, or ps_satl_name
 *                     is 0..
 *   SATLR_OUT_OF_HANDLE Cannot get a handle.
 *   SATLR_FAIL        The specified 'ps_satl_name' or 'ui2_satl_id' value is not
 *                     unique (e.g., it is currently been used), or limit of
 *                     the number of SATL objects that can be created is
 *                     reached.
 *   SATLR_OUT_OF_MEM  Not enough memory to create SATL object.
 *   SATLR_INV_ARG     'ps_satl_name' or 'ph_satl' is NULL.
 * 
 ----------------------------------------------------------------------------*/
INT32 x_satl_create
(
    const CHAR*         ps_satl_name,
    UINT16              ui2_satl_id,
    SATL_MEM_POLICY_T   e_mem_policy,
    UINT16              ui2_max_nb_rec,
    VOID*               pv_tag,
    x_satl_nfy_fct      pf_nfy,
    HANDLE_T*           ph_satl
)
{
    INT32   i4_rc=SATLR_FAIL;
    CHAR    s_satl_name[MAX_SATL_NAME_LEN+1];

    if ( ui2_satl_id != 0            &&
         ps_satl_name != NULL        &&
         x_strlen(ps_satl_name) > 0  &&
         ph_satl != NULL )
    {
        *ph_satl=NULL_HANDLE;

        x_strncpy(s_satl_name,ps_satl_name,MAX_SATL_NAME_LEN);
        s_satl_name[MAX_SATL_NAME_LEN]='\0';
        
        satl_sema_lock();
        /* check if the input SATL name and id has not been used. */
        /* Add the name and id to the internal list. */
        if ( satl_check_id_name(ui2_satl_id,s_satl_name) == TRUE &&
             satl_add_id_name(ui2_satl_id,s_satl_name) == SATLR_OK
            )
        {
            i4_rc=satl_create_object(s_satl_name,ui2_satl_id,
                                     e_mem_policy,ui2_max_nb_rec,
                                     pv_tag,pf_nfy,ph_satl);
            if ( i4_rc != SATLR_OK )
            {
                /* Remove the id name pair. */
                satl_del_id(ui2_satl_id);
            }
        }
        satl_sema_unlock();
    }
    else
    {
        i4_rc=SATLR_INV_ARG;
    }
    return  i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_delete
 *
 * Description: This API deletes the specified SATL object associated 
 *              with 'h_satl' handle.    Other clients with a handle to 
 *              the deleted SATL object will receive a notification of 
 *              the delete event.   
 *
 * Inputs: 
 *   h_satl	Handle to the SATL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          Specified SATL object is deleted.
 *   SATLR_INV_HANDLE  Bad handle.
 -----------------------------------------------------------------------*/
INT32 x_satl_delete
(
    HANDLE_T    	h_satl
)
{
    INT32           i4_rc;
    SATL_OBJ_T* 	pt_satl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        satl_sema_lock();
        /* remove satl id/name pairs. */
        satl_del_id(pt_satl->ui2_satl_id);
        satl_sema_unlock();
        
        i4_rc=satl_delete_object(pt_satl);
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return  i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_clean
 *
 * Description: This API removes all the satellite records from the 
 *              specified SATL object pointed to by the 'h_satl' handle.    
 *              All other handles to this SATL object will receive a 
 *notification.   
 *
 * Inputs: 
 *   h_satl     Handle to the SATL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          All records are successfully removed. 
 *   SATLR_INV_HANDLE  Bad handle.
 -----------------------------------------------------------------------*/
INT32 x_satl_clean
(
    HANDLE_T    	h_satl
)
{
    INT32           i4_rc;
    SATL_OBJ_T* 	pt_satl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        i4_rc=satl_clean_object(pt_satl);
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_add_rec
 *
 * Description: This function adds a new service record to the SATL
 *              database object.  The SATL library will make copy of 
 *              the record content from the address location pointed to 
 *              by the 'pt_satl_rec'. The 'ui2_satl_rec_id' field in 
 *              the 'pt_satl_rec' structure specifies the key field of the 
 *              service record to be added to the database.  
 *
 * Inputs: 
 *   h_satl     Handle to the SATL object.
 *   pt_satl_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          Record is successfully added. 
 *   SATLR_INV_HANDLE  Bad SATL handle.
 *   SATLR_OUT_OF_MEM  Can not allocate memory, add record failed,.
 *   SATLR_INV_ARG     'pt_satl_rec' is null.  
 -----------------------------------------------------------------------*/
INT32 x_satl_add_rec
(
    HANDLE_T            h_satl,
    const SATL_REC_T*   pt_satl_rec
)
{
    INT32           i4_rc;
    SATL_OBJ_T* 	pt_satl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        if ( pt_satl_rec != NULL                             &&
             pt_satl_rec->ui2_satl_rec_id != SATL_NULL_REC_ID
           )
        {
            i4_rc=satl_write_rec_to_object(pt_satl,
                                           pt_satl_rec,
                                           SATL_APPEND_RECORD);
        }
        else
        {
            i4_rc=SATLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_del_rec
 *
 * Description: This function deletes a specified service record from 
 *              the SATL. The record to be deleted is specified by the 
 *              satellite record id.
 *
 * Inputs: 
 *   h_satl          Handle to the SATL object.
 *   ui2_satl_rec_id Specifies the satellite record to be deleted. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK           Specified record is deleted. 
 *   SATLR_INV_HANDLE   Bad SATL handle.
 *   SATLR_REC_NOT_FOUND The specified record to be deleted is not in 
 *                      the SATL object.
 *   SATLR_INV_ARG      'ui2_satl_rec_id' is not a valid record id.
 -----------------------------------------------------------------------*/
INT32 x_satl_del_rec
(
    HANDLE_T            h_satl,
    UINT16              ui2_satl_rec_id
)
{
    INT32           i4_rc;
    SATL_OBJ_T* 	pt_satl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        if ( ui2_satl_rec_id != SATL_NULL_REC_ID )
        {
            i4_rc=satl_del_rec_from_object(pt_satl, ui2_satl_rec_id);
        }
        else
        {
            i4_rc = SATLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_update_rec
 *
 * Description: This function updates a Satellite record to the SATL
 *              object. If the key field value (e.g., 'ui2_satl_rec_id' field
 *              in the 'pt_satl_rec' structure) of the input service record 
 *              matches an existing record in the SATL object, then the 
 *              existing record is over-wrote with the new information,  
 *              otherwise a new record is created in the SATL database. 
 * 
 *              If the 'b_must_exist' flag is TRUE, then there must be an 
 *              existing record with the same key field value, else a status 
 *              of 'SATLR_REC_NOT_FOUND' is returned to the caller.  
 *              If 'b_must_exist' flag is FALSE, then a new service record 
 *              is created if no existing record matching the key field value 
 *              is found in the SATL database.
 *
 *              Call to this function must be protected, e.g., the SATL
 *              database object needs to be locked before the call to this 
 *              function and unlock after returning from call to this function.
 *
 * Inputs: 
 *   h_satl     Handle to the SATL object.
 *   pt_satl_rec Pointer to service record structure. 
 *   b_must_exist Boolean flag specifying if the service record to be 
 *                updated should exist in the SATL database
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          Record is successfully added/updated.
 *   SATLR_INV_HANDLE  Bad SATL handle. 
 *   SATLR_OUT_OF_MEM  Not enough memory to create a new SATL record.
 *   SATLR_REC_NOT_FOUND The specified record is not found 
 *                      ('b_must_exist' == TRUE)
 *   SATLR_INV_ARG     'pt_satl_rec' is NULL or the pt_satl_rec
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_update_rec
(
    HANDLE_T            h_satl,
    const SATL_REC_T*   pt_satl_rec,
    BOOL                b_must_exist	
)
{
    INT32           i4_rc;
    SATL_OBJ_T* 	pt_satl;
    HANDLE_TYPE_T   pe_type;
    UINT8           ui1_write_mode;

    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        if ( pt_satl_rec != NULL                              &&
             pt_satl_rec->ui2_satl_rec_id != SATL_NULL_REC_ID
           )
        {
            if ( b_must_exist == TRUE )
            {
                ui1_write_mode=SATL_MODIFY_RECORD;
            }
            else
            {
                ui1_write_mode=SATL_UPDATE_RECORD;
            }
            
            i4_rc=satl_write_rec_to_object(pt_satl,
                                           pt_satl_rec,
                                           ui1_write_mode);            
        }
        else
        {
            i4_rc=SATLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_qry_rec_id
 *
 * Description: This API queries the range (FIRST, LAST, or NEXT) of 
 *              valid satellite record id values used in the SATL
 *              object.  
 *   
 * Inputs: 
 *   h_satl            Handle to the SATL object.
 *   e_satl_qry_option Flag to specifies the record id value to be returned.  
 *                     The values are:
 *                     SATL_FIRST_REC_ID, SATL_LAST_REC_ID, SATL_NEXT_REC_ID.
 *
 * Outputs: None
 *   pui2_rec_id       Contain the satellite record id value.	
 *
 * Returns: 
 *   SATLR_OK          SATL record id return.
 *   SATLR_INV_HANDLE  Bad SATL handle. 
 *   SATLR_INV_ARG     'pui2_rec_id' is NULL.
 *
 -----------------------------------------------------------------------*/
INT32 x_satl_qry_rec_id
(
    HANDLE_T            h_satl,
    SATL_REC_ID_QRY_T   e_satl_qry_option,
    UINT16*             pui2_rec_id	
)
{

    INT32               i4_rc;
    SATL_OBJ_T* 	    pt_satl;
    HANDLE_TYPE_T       pe_type;

    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                                   &&
         (pt_satl != NULL)
        )
    {
        if ( pui2_rec_id != NULL )
        {
            switch (e_satl_qry_option)
            {
                case SATL_FIRST_REC_ID:
                {
                    if ( satl_get_smallest_rec_id(pt_satl, pui2_rec_id) == TRUE )
                    {
                        i4_rc=SATLR_OK;
                    }
                    else
                    {
                        i4_rc=SATLR_REC_NOT_FOUND;
                    }
                }
                break;

                case SATL_LAST_REC_ID:
                {
                    if ( satl_get_largest_rec_id(pt_satl, pui2_rec_id) == TRUE )
                    {
                        i4_rc=SATLR_OK;
                    }
                    else
                    {
                        i4_rc=SATLR_REC_NOT_FOUND;
                    }
                }
                break;

                case SATL_NEXT_REC_ID:
                {
                    satl_get_next_rec_id(pt_satl, pui2_rec_id);
                    i4_rc=SATLR_OK;
                }
                break;

                default:
                {
                    *pui2_rec_id=SATL_NULL_REC_ID;
                    i4_rc=SATLR_INV_ARG;
                }
            }
        }
        else
        {
            i4_rc=SATLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_lock
 *
 * Description: This function locks the SATL object.   The function blocks 
 *              until the lock is acquired.
 *
 * Inputs: 
 *   h_satl	Handle to the SATL object. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          SATL is locked. 
 *   SATLR_INV_HANDLE  Bad SATL handle.
 *
 -----------------------------------------------------------------------*/
INT32 x_satl_lock
(
    HANDLE_T            h_satl
)
{
    INT32           i4_rc;
    SATL_OBJ_T* 	pt_satl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        i4_rc=satl_lock_object(pt_satl);
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_unlock
 *
 * Description: This function unlocks the SATL database. 
 *
 * Inputs: 
 *   h_satl	Handle to the SATL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK         SATL database is unlocked. 
 *   SATLR_INV_HANDLE Bad SATL handle.
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_unlock
(
    HANDLE_T            h_satl
)
{
    INT32           i4_rc;
    SATL_OBJ_T* 	pt_satl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        i4_rc=satl_unlock_object(pt_satl);
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_read_lock
 *
 * Description: This function acquires read-lock on the specified SATL object.
 *    The function blocks until the read-lock is acquired.
 *
 * Inputs: 
 *   h_satl	Handle to the SATL object. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          SATL is locked. 
 *   SATLR_INV_HANDLE  Bad SATL handle.
 *
 -----------------------------------------------------------------------*/
INT32 x_satl_read_lock
(
    HANDLE_T            h_satl
)
{
    INT32           i4_rc;
    SATL_OBJ_T* 	pt_satl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        i4_rc=satl_lock_object_for_read(pt_satl);
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_read_unlock
 *
 * Description: This function releases the read-lock on the specified
 *      SATL database. 
 *
 * Inputs: 
 *   h_satl	Handle to the SATL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK         SATL database is unlocked. 
 *   SATLR_INV_HANDLE Bad SATL handle.
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_read_unlock
(
    HANDLE_T            h_satl
)
{
    INT32           i4_rc;
    SATL_OBJ_T* 	pt_satl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        i4_rc=satl_unlock_object_for_read(pt_satl);
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_fs_load
 *
 * Description: This API loads a specified SATL object from file system
 *              into memory, and returns a handle to the newly created
 *              SATL object.
 * Inputs: 
 *   h_dir	      Handle to the parent directory of the file specified by
 *                the path 'ps_satl_path'. If 'ps_satl_path' is an absolute
 *                path, then this argument should be set to 'FM_ROOT_HANDLE'.
 *   ps_satl_path Specifies the path to the file where the SATL objects
 *                are stored in the file system.
 *   ui2_satl_id  Specifies the SATL id to load from the file system.
 *                The limit for SATL ID is between 1 and 65535.
 *   ps_satl_name Specifies the name for the SATL object.  If null string
 *                is specified, then the default name stored in the file
 *                system is used to create the SATL object.
 *   pv_tag       Pointer to a private tag value provided by the caller.
 *                This tag value is passed to the caller's notification
 *                function.
 *   pf_nfy       Notification function provided by the caller.  This
 *                function is called to notify the owner of this SATL
 *                handle when the SATL object has been modified by another
 *                client.  If NULL, then no notification function will be
 *                called.
 * Outputs:
 *   ph_satl      On return, contain the handle to the SATL object created
 *                from file system.
 * Returns: 
 *   SATLR_OK           SATL object is created. 
 *   SATLR_NOT_FOUND    'ui2_satl_id' is not found or 'ps_satl_path' does
 *                      not exist.
 *   SATLR_OUT_OF_HANDLE Cannot get a handle.
 *   SATLR_OUT_OF_MEM   Not enough memory to create SATL object.
 *   SATLR_FAIL         The specified 'ps_satl_name' or 'ui2_satl_id' value
 *                      is not unique.
 *   SATLR_INV_ARG      'ph_satl' or 'ps_satl_path' is NULL.
 -----------------------------------------------------------------------*/
INT32 x_satl_fs_load
(
    HANDLE_T            h_dir,
    const CHAR*         ps_satl_path,
    UINT16              ui2_satl_id,
    const CHAR*	        ps_satl_name,
    VOID*               pv_tag,
    x_satl_nfy_fct      pf_nfy,
    HANDLE_T*           ph_satl
)
{
    INT32           i4_rc;
    CHAR            s_satl_name[MAX_SATL_NAME_LEN+1];
    
    if ( ps_satl_path != NULL     &&
         ph_satl     != NULL     &&
         ui2_satl_id != 0   
        )
    {
        *ph_satl=NULL_HANDLE;

        if ( ps_satl_name != NULL )
        {
            x_strncpy(s_satl_name,ps_satl_name,MAX_SATL_NAME_LEN);
            s_satl_name[MAX_SATL_NAME_LEN]='\0';
            ps_satl_name=s_satl_name;
        }

        DBG_API(("{SATL} loading SATL data.\n"));
        
        i4_rc=satl_fs_load_object(
            h_dir,
            ps_satl_path,
            ui2_satl_id,
            ps_satl_name,
            pv_tag,
            pf_nfy,
            ph_satl);
        
        DBG_API(("{SATL} done loading SATL data [ret code: %d]\n",i4_rc));
    }
    else
    {
        i4_rc = SATLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_fs_store
 *
 * Description: This API stores the specified SATL object to the file system.
 *
 * Inputs: 
 *   h_dir	      Handle to the parent directory of the file specified by
 *                the path 'ps_satl_path'. If 'ps_satl_path' is an absolute
 *                path, then this argument should be set to 'FM_ROOT_HANDLE'.
 *   ps_satl_path Specifies the file path to the file where the SATL objects
 *                are stored in the file system.
 *   ui2_satl_id  Specifies the  ID of the SATL object to be stored to the
 *                file system.  The limit for SATL ID is between 1 and 65535.
 *
 * Outputs: None 
 *
 * Returns: 
 *   SATLR_OK		SATL object is stored to the file system. 
 *   SATLR_NOT_FOUND	The SATL object specified by the 'ui2_satl_id' does
 *                      not exist.
 *   SATLR_INV_ARG	'ps_satl_path' is NULL.
 -----------------------------------------------------------------------*/
INT32 x_satl_fs_store
(
    HANDLE_T            h_dir,
    const CHAR*         ps_satl_path,
    UINT16              ui2_satl_id
)
{
    INT32   i4_rc;
    
    if ( ps_satl_path != NULL &&
         ui2_satl_id != 0
        )
    {
        DBG_API(("{SATL} saving SATL data.\n"));
        
        i4_rc=satl_fs_store_object(h_dir,ps_satl_path,ui2_satl_id);

        DBG_API(("{SATL} done saving SATL data [ret code: %d]\n",i4_rc));
    }
    else
    {
        i4_rc = SATLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_fs_del
 *
 * Description: This API deletes a specified SATL object on the file system.
 *
 * Inputs: 
 *   h_dir	      Handle to the parent directory of the file specified by the
 *                path 'ps_satl_path'. If 'ps_satl_path' is an absolute path,
 *                then this argument should be set to 'FM_ROOT_HANDLE'.
 *   ps_satl_path Specifies the file path to the file where the SATL objects
 *                are stored in the file system.
 *   ui2_satl_id  Specifies the ID of the SATL object to be deleted from the
 *                file system.  The limit for SATL ID is between 1 and 65535.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK	  SATL object is deleted from the file system. 
 *   SATLR_NOT_FOUND  'ui2_satl_id' is not found or 'ps_satl_path' does not
 *                    exist.
 *   SATLR_INV_ARG    'ps_satl_path' is NULL.
 -----------------------------------------------------------------------*/
INT32 x_satl_fs_del
(
    HANDLE_T            h_dir,
    const CHAR*         ps_satl_path,
    UINT16              ui2_satl_id
)
{
    INT32   i4_rc;
    
    if ( ps_satl_path != NULL &&
         ui2_satl_id != 0
        )
    {   
        i4_rc=satl_fs_del_object(h_dir,ps_satl_path, ui2_satl_id);
    }
    else
    {
        i4_rc = SATLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_fs_qry
 *
 * Description: This API queries the SATL objects stored in file system.
 *
 * Inputs: 
 *   h_dir	      Handle to the parent directory of the file specified
 *                by the path 'ps_satl_path'. If 'ps_satl_path' is an
 *                absolute path, then this argument should be set to
 *                'FM_ROOT_HANDLE'.
 *   ps_satl_path Specifies the file path to the file where the SATL
 *                objects are stored in the file system..
 *   ui2_idx      Index value to get the next SATL object.  0 means the
 *                first SATL, 1 is the second SATL, etc.
 *
 * Outputs: 
 *   pui2_satl_id Pointer to a UINT16 variable. On return, it contains SATL ID.
 *
 *   ps_satl_name Pointer to a string buffer (minimum 17 bytes) .
 *                On return, it contains the default SATL name stored on the
 *                file system. 
 * Returns: 
 *   SATLR_OK         SATL object found. 
 *   SATLR_NOT_FOUND  No SATL object found.
 *   SATLR_INV_ARG    'ps_satl_name' or 'pui2_satl_id' is NULL.
 -----------------------------------------------------------------------*/
INT32 x_satl_fs_qry
(
    HANDLE_T            h_dir,
    const CHAR*         ps_satl_path,
    UINT16              ui2_idx,
    UINT16*             pui2_satl_id,
    CHAR*               ps_satl_name
)
{
    INT32   i4_rc;

    if ( ps_satl_path != NULL     &&
         pui2_satl_id != NULL     &&
         ps_satl_name != NULL
        )
    {
        *pui2_satl_id = SATL_NULL_ID;
        ps_satl_name[0] = '\0';
        
        i4_rc=satl_qry_persistent_objects(
            h_dir,
            ps_satl_path,
            ui2_idx,
            pui2_satl_id,
            ps_satl_name);
    }
    else
    {
        i4_rc = SATLR_INV_ARG;
    }    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_fs_sync
 *
 * Description: This API sync a specified SATL handle with SATL records
 *              from persistent file system.  The existing SATL records
 *              are replaced with new records read from file. 
 *
 * Inputs:
 *
 *   h_satl         Specify the handle to the SATL object.
 *
 *   h_dir          The handle to the directory.  The 'ps_satl_path'
 *                  will be relative to the specified 'h_dir'.
 *
 *   ps_satl_path   Specifies the directory path of the SATL object in the 
 *                  file system. See File Path Specification, 10.1, for the 
 *                  types of file path that can be specified.
 *
 *   ui2_satl_id    Specifies the SATL id to load from the file system.  
 *                  The limit for SATL ID is between 1 and 65535.
 *
 * Outputs: 
 *
 *   None.
 *                 
 * Returns: 
 *   SATLR_OK            SATL records are load from the persistent file
 *                       into the memory of SATL object.
 *
 *   SATLR_NOT_FOUND     'ui2_satl_id' is not found or 'ps_satl_path'
 *                       does not exist.
 *
 *   SATLR_FAIL          internal error
 *
 *   SATLR_INV_ARG       'ps_satl_path' is NULL, or 'h_satl' is NULL_HANDLE,
 *                       or ui2_satl_id is 0.
 *
 *   SATLR_INV_FILE_PATH Specified file path is not a directory or a RAW 
 *                       character file.
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_fs_sync
(
    HANDLE_T		h_satl,
    HANDLE_T        h_dir,
    const CHAR*	    ps_satl_path,
    UINT16		    ui2_satl_id
)
{
    INT32           i4_rc;
    
    if ( ps_satl_path != NULL            &&
         h_satl      != NULL_HANDLE     &&
         ui2_satl_id != 0   
        )
    {
        i4_rc=satl_fs_sync_object(h_satl,
                                  h_dir,
                                  ps_satl_path,
                                  ui2_satl_id);
    }
    else
    {
        i4_rc = SATLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_open
 *
 * Description: This function gets a handle to the specified SATL database 
 *              object by its ID value.    
 *
 * Inputs: 
 *   ui2_satl_id: Specifies the SATL object identifier value.  
 *                The limit for SATL ID is between 1 and 65535.
 *   pv_tag:      A private tag value associated with the calling client.
 *   pf_nfy:      Pointer to the caller provided notification function.  
 *                The notification function will be called in the context of 
 *                the thread that modifies (update, add, delete) the SATL
 *                database.  If NULL, then no notification function will be 
 *                called.
 *
 * Outputs:
 *   ph_satl:     Pointer to the handle created.
 *
 * Returns: 
 *   SATLR_OK              SATL handle is returned. 
 *   SATLR_OUT_OF_HANDLE   No more handle available.
 *   SATLR_OUT_OF_MEM      Not enough memory to create the SATL control 
 *                         structure.
 *   SATLR_NOT_FOUND       The specified SATL database object id (e.g. 
 *                         'ui2_satl_id') is not found.
 *   SATLR_INV_ARG         'ph_satl' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_open
(
    UINT16              ui2_satl_id,
    VOID*               pv_tag,
    x_satl_nfy_fct      pf_nfy,
    HANDLE_T*           ph_satl
)
{
    INT32           i4_rc;
    CHAR            ps_satl_name[MAX_SATL_NAME_LEN+1];

    if ( ph_satl != NULL )
    {
        *ph_satl=NULL_HANDLE;
        satl_sema_lock();
        /* Find the id/name */
        if ( satl_get_name_by_id(ui2_satl_id, ps_satl_name) == TRUE )
        {
            i4_rc=satl_open_object(ui2_satl_id,
                                   ps_satl_name,
                                   pv_tag,
                                   pf_nfy,
                                   ph_satl);
        }
        else
        {
            i4_rc=SATLR_NOT_FOUND;
        }
        satl_sema_unlock();
    }
    else
    {
        i4_rc = SATLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_open_by_name
 *
 * Description: This function gets a handle to the specified SATL database 
 *              object by its name.    
 *
 * Inputs: 
 *   ps_satl_name   Specifies the SATL object name (16 char max).
 *   pv_tag         A private tag value associated with the calling client.
 *   pf_nfy         Pointer to a caller provided notification function.  
 *                  The notification function is called in the context of 
 *                  the thread that modifies (update, add, delete) the SATL
 *                  database.  If NULL, then no notification function will 
 *                  be called.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK            SATL handle is returned. 
 *   SATLR_OUT_OF_HANDLE No more handle available.
 *   SATLR_OUT_OF_MEM    Not enough memory to create the SATL handle structure.
 *   SATLR_NOT_FOUND     The specified SATL database object name (e.g., 
 *                       ps_satl_name) is not found.
 *   SATLR_INV_ARG       'ph_satl' or 'ps_satl_name' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_open_by_name
(
    const CHAR*         ps_satl_name,
    VOID*               pv_tag,
    x_satl_nfy_fct      pf_nfy,
    HANDLE_T*           ph_satl
)
{
    INT32           i4_rc;
    UINT16          ui2_satl_id;
    CHAR            s_satl_name[MAX_SATL_NAME_LEN+1];

    if ( ph_satl != NULL && ps_satl_name != NULL )
    {
        *ph_satl=NULL_HANDLE;

        x_strncpy(s_satl_name,ps_satl_name,MAX_SATL_NAME_LEN);
        s_satl_name[MAX_SATL_NAME_LEN]='\0';
        
        satl_sema_lock();
        /* Find the id/name */
        if ( satl_get_id_by_name(s_satl_name, &ui2_satl_id) == TRUE )
        {
            i4_rc=satl_open_object(ui2_satl_id,
                                   s_satl_name,
                                   pv_tag,
                                   pf_nfy,
                                   ph_satl);
        }
        else
        {
            i4_rc=SATLR_NOT_FOUND;
        }
        satl_sema_unlock();
    }
    else
    {
        i4_rc = SATLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_close
 *
 * Description: This function frees the specified handle to the SATL
 *              database object.    
 *
 * Inputs: 
 *   h_satl	Handle to SATL database object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          SATL handle is freed. 
 *   SATLR_INV_HANDLE  Bad handle.
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_close
(
    HANDLE_T            h_satl
)
{
    INT32           i4_rc;
    SATL_OBJ_T* 	pt_satl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        handle_free(h_satl,TRUE);
        i4_rc=SATLR_OK;
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_get_handle_info
 *
 * Description: This function gets the SATL ID and SATL name for the 
 *              specified handle.    
 *
 * Inputs: 
 *   h_satl	Handle to the SATL object.
 *
 * Outputs: 
 *   pui2_satl_id Pointer to SATL ID variable.  On return it contains 
 *                the SATL ID value for this handle.
 *   ps_satl_name Pointer to a char array variable (the size of this char 
 *                array should be at least MAX_SATL_NAME_LEN+1). On return, 
 *                this array contains the name of the SATL object.
 * Returns: 
 *   SATLR_OK          Information about this handle (satl id and satl name)
 *                     is returned.
 *   SATLR_INV_HANDLE  Bad handle.
 *   SATLR_INV_ARG     'pui2_satl_id' or 'ps_satl_name' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_get_handle_info
(
    HANDLE_T            h_satl,
    UINT16*             pui2_satl_id,
    CHAR*               ps_satl_name
)
{
    INT32           i4_rc;
    SATL_OBJ_T* 	pt_satl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        if ( pui2_satl_id != NULL && ps_satl_name != NULL )
        {
            *pui2_satl_id = pt_satl->ui2_satl_id;
            x_strcpy(ps_satl_name, pt_satl->ps_satl_name);
            i4_rc=SATLR_OK;
        }
        else
        {
            i4_rc=SATLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_get_rec
 *
 * Description: This API gets a specific record stored in the SATL database 
 *              object.    The search parameter is specified by the 
 *              'ui2_satl_rec_id' value.
 *
 * Inputs: 
 *   h_satl          Handle to the SATL object.
 *   ui2_satl_rec_id Specifies the service record id.
 *
 * Outputs: 
 *   pt_satl_rec Pointer to a service record structure.   This structure must 
 *               be allocated by the caller.  On return, it is filled with 
 *               service record information.
 *   pui4_ver_id Pointer to a variable holding version id of the database 
 *               object.   Note: version id of the SATL database object is 
 *               incremented each time when a modification takes place.
 * Returns: 
 *   SATLR_OK   Data from the specified record is returned. 
 *   SATLR_INV_HANDLE   Bad handle.
 *   SATLR_REC_NOT_FOUND No record matching the search parameter 
 *                      ('ui2_satl_rec_id') is found.
 *   SATLR_INV_ARG      'pui4_ver_id' or 'pt_satl_rec' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_get_rec
(
    HANDLE_T            h_satl,
    UINT16              ui2_satl_rec_id,
    SATL_REC_T*         pt_satl_rec,
    UINT32*             pui4_ver_id
)
{
    INT32           i4_rc;
    SATL_OBJ_T* 	pt_satl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        if ( pui4_ver_id    != NULL &&
             pt_satl_rec    != NULL &&
             ui2_satl_rec_id != SATL_NULL_REC_ID  )
        {
            i4_rc=satl_get_rec_from_obj(pt_satl,
                                        ui2_satl_rec_id,
                                        pt_satl_rec,
                                        pui4_ver_id);
        }
        else
        {
            i4_rc=SATLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_get_num_recs
 *
 * Description:    This function returns the number of records for the
 *                 specified SATL.
 *
 * Inputs: 
 *   h_satl	Handle to the SATL object.
 *
 * Outputs: 
 *   pui2_num_recs	Pointer to a variable for the number of records in
 *                  the SATL.	
 *   pui4_ver_id    Pointer to a variable holding version id of the SATL
 *                  database object. 	
 * Returns: 
 *   SATLR_OK		Number of records in the specified SATL is returned. 
 *   SATLR_INV_HANDLE   Bad 'h_satl' handle.
 *   SATLR_INV_ARG      'pui2_num_recs' or 'pui4_ver_id' is a NULL pointer.
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_get_num_recs
(
    HANDLE_T            h_satl,
    UINT16*             pui2_num_rec,
    UINT32*             pui4_ver_id
)
{
    INT32           i4_rc;
    SATL_OBJ_T* 	pt_satl;
    HANDLE_TYPE_T   pe_type;
    
    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        if (  pui4_ver_id  != NULL   &&
              pui2_num_rec != NULL
           )
        {
            i4_rc=satl_get_num_rec(pt_satl,pui2_num_rec,pui4_ver_id);
        }
        else
        {
            i4_rc=SATLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_get_rec_by_rec_idx
 *
 * Description: This API returns satellite list record entry from the
 *              SATL database object by its index. Maximum index is obtained
 *              by calling x_satl_get_num_recs.
 *             
 * Inputs: 
 *    h_satl            Handle to the SATL object.
 *    ui2_idx           an idx to get the next SATL record.
 *                      For example: 0 will the first SATL record, and 1
 *                      will be second, and so on.
 *
 * Outputs:
 *    pt_satl_rec   Ref to a SATL_REC_T structure containing the satellite
 *                  data.
 *    pui4_ver_id   Contains the version id of the SATL database object.   
 *                  
 * Returns: 
 *   SATLR_OK   Data from the specified record is returned. 
 *   SATLR_INV_HANDLE   Bad handle.
 *   SATLR_REC_NOT_FOUND No record matching the search index is found.
 *   SATLR_INV_ARG      'pui4_ver_id' or 'pt_satl_rec' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_get_rec_by_rec_idx
(
    HANDLE_T            h_satl,
    UINT16              ui2_idx,
    SATL_REC_T*         pt_satl_rec,
    UINT32*             pui4_ver_id
)
{
    INT32               i4_rc;
    SATL_OBJ_T* 	    pt_satl;
    HANDLE_TYPE_T       pe_type;
    SATL_SEARCH_PARM_T  t_parm;
 
    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pt_satl_rec    != NULL
            )
        {
            /* Set up the search parameter and address of the key value
               for the search.
            */
            t_parm.ui4_fld_type             = (DB_FIELD_T) SATL_CDB_TYPE_UNKNOWN ;
            
            i4_rc=satl_read_rec_from_sort_list(pt_satl,
                                               &t_parm,
                                               ui2_idx,
                                               pt_satl_rec,
                                               pui4_ver_id);
        }
        else
        {
            i4_rc=SATLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
        	ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
		}
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_rec_init
 *
 * Description: This API pre-sets the SATL_REC_T structure to a 
 *     pre-defined invalid value (e.g., 0xFF).   The pre-defined invalid 
 *     value can be used to detect changes in the SATL_REC_T structure for 
 *     backward compatibility in the future.
 *
 *     For example, if a new field is added to the SATL record structure.  
 *     At the runtime, if the new field in the SATL_REC_T structure passed 
 *     into SATL library contain 0xFF, then we know the caller has not been 
 *     re-compile with the new SATL module.   
 *
 * Inputs: 
 *   Pointer to a SATL record structure. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          SATL library is initialized successfully.
 *   SATLR_INV_ARG     'pt_satl_rec' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 x_satl_rec_init
(
    SATL_REC_T*		pt_satl_rec
)
{
    if ( pt_satl_rec != NULL  )
    {
        x_memset(pt_satl_rec,0,sizeof(SATL_REC_T));
    }
    else
    {
        return SATLR_INV_ARG;
    }
    return SATLR_OK;   
}

/*----------------------------------------------------------------------
 * Name: x_satl_get_info
 *
 * Description: 
 *   Get info (number of records, size of record, etc) for this SATL handle.
 *
 *
 * Inputs: 
 *   h_satl       Handle to the SATL object.
 *
 * Outputs:
 *   None.
 *
 * Returns: 
 *   SATLR_OK        Version id is returned. 
 *   SATLR_INV_HANDLE Bad 'h_satl' handle.
 *   SATLR_INV_ARG   'pf_sort_cmp' or 'pf_search_cmp' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 x_satl_get_info
(
    HANDLE_T               h_satl
)
{
    INT32               i4_rc;
    SATL_OBJ_T* 	    pt_satl;
    HANDLE_TYPE_T       pe_type;
    DB_INFO_T           t_db_info;
    UINT32              ui4_persistent_size;
    UINT32              ui4_ver_id;
    
    if ( (handle_get_type_obj(h_satl, &pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                           &&
         (pt_satl != NULL)
        )
    {
        x_dbg_stmt("<SATL ID:  %d  SATL_NAME: %s>\n",
                   pt_satl->ui2_satl_id, pt_satl->ps_satl_name);
        
        if ( db_get_info(pt_satl->h_cdb, &t_db_info) == DBR_OK )
        {
            x_dbg_stmt("number of SATL records:   %d \n",
                       t_db_info.ui4_record_count);
            
            x_dbg_stmt("size of each record:     %d (bytes)\n",
                       t_db_info.ui2_record_width);

            x_dbg_stmt("Heap memory usage for string buffer:  allocated: %d (bytes) used: %d (bytes)\n",
                       t_db_info.ui4_str_buf_allocated,
                       t_db_info.ui4_str_buf_used);

            x_dbg_stmt("Heap memory usage for string obj array: allocated: %d (bytes) used: %d (bytes)\n",
                       t_db_info.ui2_nb_str_obj_allocated*sizeof(UINT16),
                       t_db_info.ui2_nb_str_obj_used*sizeof(UINT16));
            
            x_dbg_stmt("SATL object filessize [header+metadata+data]:  %d (bytes)\n",
                       satl_persistent_size(pt_satl));

            /* Size of CDB object. */
            i4_rc=db_get_persistent_size(pt_satl->h_cdb,
                                         (db_endian_conv_fct)satl_union_end_conv_to_file,
                                         &ui4_persistent_size,
                                         &ui4_ver_id);
            if ( i4_rc == DBR_OK )
            {
                x_dbg_stmt("[with CDB object using: [%d bytes]\n", ui4_persistent_size);
            }

            i4_rc = SATLR_OK;
        }
        else
        {
            x_dbg_stmt("Error: can get info on this SATL handle.\n");
            i4_rc = SATLR_FAIL;
        }
    }
    else
    {
#ifdef DEBUG
		if(x_handle_valid(h_satl))
		{
        	ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
		}
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: x_satl_set_cursor
 *
 * Description: This API specifies a entry position in SATL records, 
 *   which will read when API x_satl_read_cursor() is called.
 * 
 *
 * Inputs: 
 *   h_satl      Handle to the SATL object.
 *   e_pos       position of the SATL record to read.
 *
 * Outputs: 
 *   None.
 *               
 * Returns: 
 *   SATLR_OK   Data from the specified record is returned. 
 *   SATLR_INV_HANDLE   Bad handle.
 *   SATLR_INV_ARG      'e_pos' is invalid.
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_set_cursor
(
    HANDLE_T            h_satl,
    SATL_ITERATE_T      e_pos
)
{
    INT32           i4_rc;
    SATL_OBJ_T* 	pt_satl;
    HANDLE_TYPE_T   pe_type;
    DB_CTRL_FLAG_T  e_cursor_ctrl;
    

    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        i4_rc = SATLR_OK;
        
        switch (e_pos)
        {
            case SATL_FIRST_REC:
            {
                e_cursor_ctrl = DB_FIRST_RECORD;
            }
            break;

            case SATL_LAST_REC:
            {
                e_cursor_ctrl = DB_LAST_RECORD;
            }
            break;

            case SATL_NEXT_REC:
            {
                e_cursor_ctrl = DB_NEXT_RECORD;
            }
            break;

            case SATL_PREV_REC:
            {
                e_cursor_ctrl = DB_PREV_RECORD;
            }
            break;

            default:
            {
                i4_rc = SATLR_INV_ARG;
            }
            break;
        }

        if ( i4_rc == SATLR_OK )
        {
            i4_rc=db_set_cursor(pt_satl->h_cdb,
                                e_cursor_ctrl,
                                DB_NULL_RECORD);
            /* translate the CDB error code to SATL error code */
            i4_rc=satl_cdb_to_satl_err_code(i4_rc);
        }
    }
    else
    {
#ifdef DEBUG
		if(x_handle_valid(h_satl))
		{
        	ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
		}
#endif
        i4_rc = SATLR_INV_HANDLE;
    }    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_read_cursor
 *
 * Description: This API read the content of a specific record
 *   stored in the SATL database.  The specific record is read
 *   is the record currently pointed by the cursor.   
 *              
 * Inputs: 
 *   h_satl          Handle to the SATL object.
 *
 * Outputs: 
 *   pt_satl_rec Pointer to a service record structure.   This structure must 
 *               be allocated by the caller.  On return, it is filled with 
 *               service record information.
 *
 *   pui4_ver_id Pointer to a variable holding version id of the database 
 *               object.   Note: version id of the SATL database object is 
 *               incremented each time when a modification takes place.
 * Returns: 
 *   SATLR_OK   Data from the specified record is returned. 
 *   SATLR_INV_HANDLE   Bad handle.
 *   SATLR_INV_ARG      'pui4_ver_id' or 'pt_satl_rec' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_read_cursor
(
    HANDLE_T            h_satl,
    SATL_REC_T*         pt_satl_rec,
    UINT32*             pui4_ver_id
)
{
    INT32           i4_rc;
    SATL_OBJ_T* 	pt_satl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        if ( pui4_ver_id    != NULL &&
             pt_satl_rec    != NULL
            )
        {          
            i4_rc=satl_read_rec_at_cursor_pos(pt_satl,
                                              pt_satl_rec,
                                              pui4_ver_id);            
        }
        else
        {
            i4_rc=SATLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_sort_rec
 *
 * Description: This API applies the sort operation (using the
 *    per-handle compare fucntion, or the global compare
 *    function, or default compare function, to the SATL
 *    records.
 *             
 * Inputs: 
 *    h_satl    Handle to the SATL object.
 *
 * Outputs: 
 *    pui4_ver_id   Contains the version id of the SATL database object.   
 *                  
 * Returns: 
 *   SATLR_OK          
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_sort_rec
(
    HANDLE_T			h_satl,
    UINT32*             pui4_ver_id 
)
{
    INT32               i4_rc;
    SATL_OBJ_T* 	    pt_satl;
    HANDLE_TYPE_T       pe_type;
 
    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        if ( pui4_ver_id  != NULL )
        {
            i4_rc=satl_sort_rec_by_ts_id(pt_satl,pui4_ver_id);
        }
        else
        {
            i4_rc=SATLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return i4_rc;
}

#if 0
/*----------------------------------------------------------------------
 * Name: satl_db_union_cmp_fct
 *
 * Description: This API is called by the CDB to compare the LNB
 *    data in SATL_REC_T structure and the specified search value
 *    (in this case, it will be the low frequency.).
 *    
 *             
 * Inputs: 
 *   pv_rec_fld_A:  Pointer to the first field to be compared.
 *                  For search operation, this will pointer to the
 *                  caller specified search value.
 *
 *   pv_rec_fld_B:  Pointer to the second field to be compared. For
 *                  search or sort operation, this will be the pointer
 *                  a field of a record entry in the database.
 *
 *   e_union_tag:   Specifies the UNION TAG value of the UNION field.
 *                  The UNION TAG is defined by the caller when it
 *                  created the database record definition.
 *                  This function use the UNION TAG value to decide how
 *                  to interpret the pointers 'pv_rec_fld_A' and
 *                 'pv_rec_fld_B'.
 *
 *   ui4_size:      Size (in bytes) of the UNION elements
 * 
 *   e_opcode:      Flag for specifying how the two fields should be
 *                  evaluated.
 *
 *   pv_tag:        Private caller data.            
 *
 * Outputs: 
 *       
 *                  
 * Returns: 
 *   SATLR_OK          
 *   
 -----------------------------------------------------------------------*/
INT32 satl_db_union_cmp_fct
(
    const VOID*        pv_rec_fld_A, 
    const VOID*        pv_rec_fld_B,
    DB_UNION_TAG_T     e_union_tag,
    UINT32             ui4_size, 
    DB_CMP_OPCODE_T    e_opcode,
    VOID*              pv_tag
)
{
    TUNER_LNB_INFO_T*  pt_rec_A;
    TUNER_LNB_INFO_T*  pt_rec_B;
    UINT16             ui2_lnb_low_freq_A;
    UINT16             ui2_lnb_low_freq_B;

    /* error checking. */

    /* The union tag value for SATL LNB data is 1. ,
       The size of LNB field should be 16 bytes,
       and the compare operation should EQUAL
    */
    if ( (e_union_tag != 1)      ||
         (ui4_size != SATL_LNB_DATA_PADDING) ||
         (e_opcode != DB_OPS_EQUAL)
        )
    {
        /* something is wrong. Abort. */
        ABORT(DBG_CAT_INV_OP,DBG_ABRT_DB_OPERATION_FAIL);
    }
    
    pt_rec_A = (TUNER_LNB_INFO_T*) pv_rec_fld_A;
    pt_rec_B = (TUNER_LNB_INFO_T*) pv_rec_fld_B;

    
    ui2_lnb_low_freq_A      = pt_rec_A->ui2_lnb_low_mhz_freq ;
    
    ui2_lnb_low_freq_B      = pt_rec_B->ui2_lnb_low_mhz_freq ;

    if ( (ui2_lnb_low_freq_A == 0)                  ||
         (ui2_lnb_low_freq_A == ui2_lnb_low_freq_B)
        )
    {
        return 0;
    }

    return -1;
}

    
/*----------------------------------------------------------------------
 * Name: x_satl_get_num_rec_by_lnb_low_freq
 *
 * Description: This API get the numbers of record matching specific
 *              LNB low frequency and SATL record mask.
 *              Frequency 0 matches any frequency.
 *             
 * Inputs: 
 *    h_satl            Handle to the SATL object.
 *    ui2_lnb_low_freq  Specify the LNB low frequency.
 *    ui4_mask          Specify the mask.
 *
 * Outputs:
 *    pui2_num_recs Contains the number of SATL record with specified
 *                  LNB low freq and mask.
 *    pui4_ver_id   Contains the version id of the SATL database object.   
 *                  
 * Returns: 
 *   SATLR_OK          
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_get_num_rec_by_lnb_low_freq
(
    HANDLE_T            h_satl,
    UINT16              ui2_lnb_low_freq,
    UINT32              ui4_mask,
    UINT16*             pui2_num_recs,
    UINT32*             pui4_ver_id
)
{
    INT32           i4_rc;
    UINT8           ui1_key_count;
    SATL_OBJ_T* 	pt_satl;
    HANDLE_TYPE_T   pe_type;
    DB_FIELD_T      aui4_keyfld_type[3];
    const VOID*     apv_keyfld_addr[3];
    DB_SEARCH_T     ae_cmp_param[3];
    TUNER_LNB_INFO_T    t_lnb;
    
 
    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pui2_num_recs  != NULL
            )
        {
            x_memset(&t_lnb, 0, sizeof(TUNER_LNB_INFO_T));
            
            t_lnb.ui2_lnb_low_mhz_freq = ui2_lnb_low_freq;

            /* Set up the search parameter and address of the key value
               for the search.

               Note: the following paramters: t_lnb, DB_OPS_EQUAL, will
               be pass to 'satl_db_union_cmp_fct' when it is invoked by CDB module
               to compare SATL_REC_T's LNB data with the search value.
               If  you changes these parameters, then you need to check the
               code in 'satl_db_union_cmp_fct', otherwise ABORT might resulted.
            */
            ui1_key_count = 2;
            
            aui4_keyfld_type[0]      = (DB_FIELD_T) SATL_CDB_TYPE_MASK ;
            apv_keyfld_addr[0]       = &ui4_mask;
            ae_cmp_param[0].e_opcode = DB_OPS_BITWISE_AND;
            
            aui4_keyfld_type[1]      = (DB_FIELD_T) SATL_CDB_TYPE_LNB ;
            apv_keyfld_addr[1]       = &t_lnb;
            ae_cmp_param[1].e_opcode = DB_OPS_EQUAL;
            ae_cmp_param[1].pf_fct   = satl_db_union_cmp_fct;
            
            i4_rc=satl_get_num_rec_by_flds(pt_satl,
                                           ui1_key_count,
                                           aui4_keyfld_type,
                                           apv_keyfld_addr,
                                           ae_cmp_param,
                                           pui2_num_recs,
                                           pui4_ver_id);
        }
        else
        {
            i4_rc=SATLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return i4_rc;
}

    
/*----------------------------------------------------------------------
 * Name: x_satl_get_rec_by_lnb_low_freq
 *
 * Description: This API return a SATL record for a specified
 *              LNB low frequency and SATL record mask.
 *              Frequency 0 matches any frequency.
 *             
 * Inputs: 
 *    h_satl            Handle to the SATL object.
 *    ui2_lnb_low_freq  Specify the LNB low frequency.
 *    ui4_mask          Specify the mask.
 *    ui2_idx           an idx to get the next SATL record with specified
 *                      LNB low freq and mask. For example: 0 will
 *                      the first SATL record, and 1 will be second, and so
 *                      on
 *
 * Outputs:
 *    pt_satl_rec   Ref to a SATL_REC_T structure containing the satellite
 *                  data.
 *    pui4_ver_id   Contains the version id of the SATL database object.   
 *                  
 * Returns: 
 *   SATLR_OK          
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_get_rec_by_lnb_low_freq
(
    HANDLE_T            h_satl,
    UINT16              ui2_lnb_low_freq,
    UINT32              ui4_mask,
    UINT16              ui2_idx,
    SATL_REC_T*         pt_satl_rec,
    UINT32*             pui4_ver_id
)
{
    INT32               i4_rc;
    SATL_OBJ_T* 	    pt_satl;
    HANDLE_TYPE_T       pe_type;
    SATL_SEARCH_PARM_T  t_parm;
 
    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                          &&
         (pt_satl != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pt_satl_rec    != NULL
            )
        {
            /* Set up the search parameter and address of the key value
               for the search.
            */
            t_parm.ui4_fld_type             = (DB_FIELD_T) SATL_CDB_TYPE_LNB ;
            t_parm.u.t_lnb.ui2_lnb_low_freq = ui2_lnb_low_freq;
            t_parm.u.t_lnb.ui4_mask         = ui4_mask;
            t_parm.u.t_lnb.ui2_idx          = ui2_idx;
            
            i4_rc=satl_read_rec_from_sort_list(pt_satl,
                                               &t_parm,
                                               ui2_idx,
                                               pt_satl_rec,
                                               pui4_ver_id);
        }
        else
        {
            i4_rc=SATLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return i4_rc;
}
#endif

/*----------------------------------------------------------------------
 * Name: x_satl_init_mem_dump
 *
 * Description: This API initializes the memory dump structure, which 
 *              is used for x_satl_dump_to_mem.
 *
 * Inputs: 
 *   None
 *
 * Outputs: 
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Returns: 
 *   SATLR_OK           SATL database successfully dumped.
 *   SATLR_INV_ARG      'pt_mem_dump' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_init_mem_dump
(
    SATL_MEM_DUMP_T*       pt_mem_dump
)
{
    INT32           i4_rc;

    if ( pt_mem_dump != NULL )
    {          
        x_memset(pt_mem_dump, 0, sizeof(SATL_MEM_DUMP_T));
        i4_rc=SATLR_OK;
    }
    else
    {
        i4_rc=SATLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_free_mem_dump
 *
 * Description: This API deinitializes the memory dump structure. 
 *
 * Inputs: 
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Outputs: 
 *   None
 *
 * Returns: 
 *   SATLR_OK           SATL database successfully dumped.
 *   SATLR_INV_ARG      'pt_mem_dump' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_free_mem_dump
(
    SATL_MEM_DUMP_T*       pt_mem_dump
)
{
    INT32           i4_rc;

    if ( pt_mem_dump != NULL )
    {          
        if (pt_mem_dump->pv_data != NULL)
        {
            x_mem_free(pt_mem_dump->pv_data);
            pt_mem_dump->pv_data = NULL;
        }
        i4_rc=SATLR_OK;
    }
    else
    {
        i4_rc=SATLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_dump_to_mem
 *
 * Description: This API dumps the specified SATL database into the specified
 *              structure, which could be later used for x_satl_load_from_mem.
 *              The memory dump structure should be initialized before dumping.
 *
 * Inputs: 
 *   h_satl          Handle to the SATL object.
 *
 * Outputs: 
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Returns: 
 *   SATLR_OK           SATL database successfully dumped.
 *   SATLR_INV_HANDLE   Bad handle.
 *   SATLR_INV_ARG      'pt_mem_dump' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_dump_to_mem
(
    HANDLE_T               h_satl,
    SATL_MEM_DUMP_T*       pt_mem_dump
)
{
    INT32           i4_rc;
    SATL_OBJ_T*     pt_satl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                                   &&
         (pt_satl != NULL)
        )
    {
        if ( pt_mem_dump != NULL )
        {
            pt_mem_dump->ui2_satl_id = pt_satl->ui2_satl_id;
            x_strcpy(pt_mem_dump->ps_satl_name, pt_satl->ps_satl_name);
            i4_rc = db_dump_to_mem(pt_satl->h_cdb,
                                   TRUE,
                                   &(pt_mem_dump->pv_data),
                                   &(pt_mem_dump->ui4_data_len));
            i4_rc = satl_cdb_to_satl_err_code(i4_rc);
        }
        else
        {
            i4_rc=SATLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_load_from_mem
 *
 * Description: This API loads the SATL database from the specified memory
 *              dump structure, whose data comes from a previous call to
 *              x_satl_load_from_mem.
 *
 * Inputs: 
 *   h_satl          Handle to the SATL object.
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Outputs: 
 *
 * Returns: 
 *   SATLR_OK           SATL database successfully dumped.
 *   SATLR_INV_HANDLE   Bad handle.
 *   SATLR_INV_ARG      'pt_mem_dump' is NULL or SATL metadata not match.
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_load_from_mem
(
    HANDLE_T               h_satl,
    SATL_MEM_DUMP_T*       pt_mem_dump
)
{
    INT32           i4_rc;
    SATL_OBJ_T*     pt_satl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_satl,&pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                                   &&
         (pt_satl != NULL)
        )
    {
        if ( pt_mem_dump != NULL )
        {
            if (pt_mem_dump->ui2_satl_id == pt_satl->ui2_satl_id &&
                x_strcmp(pt_mem_dump->ps_satl_name, pt_satl->ps_satl_name) == 0)
            {
                i4_rc = db_load_from_mem(pt_satl->h_cdb,
                                         pt_mem_dump->pv_data,
                                         pt_mem_dump->ui4_data_len);
                i4_rc = satl_cdb_to_satl_err_code(i4_rc);
            }
            else
            {
                i4_rc = SATLR_INV_ARG;
            }
        }
        else
        {
            i4_rc=SATLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_fs_store_given_file_handle
 *
 * Description: This API stores the specified SATL object to the 
 *              file system.
 *
 * Inputs:
 *   h_dir         The handle to the file.
 *
 *  ui2_satl_id     Specifies the SATL object to be stored to the file system.  
 *                 The limit for SATL ID is between 1 and 65535.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK             SATL object is stored to the file system. 
 *
 *   SATLR_NOT_FOUND      The SATL object specified by the 'ui2_satl_id' 
 *                       does not exist.
 *
 *   SATLR_INV_ARG        ps_satl_path' is NULL.
 *
 *   SATLR_INV_FILE_PATH  Specified file path is not a directory or 
 *                       a RAW character file.
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_fs_store_given_file_handle
(
    HANDLE_T        h_file,
    UINT16          ui2_satl_id  
)
{
    INT32       i4_rc;
    
    if ( ui2_satl_id  != 0 )
    {
        DBG_API(("{SATL} saving SATL data.\n"));
        
        i4_rc=satl_fs_store_object_given_file_handle(h_file,ui2_satl_id);

        DBG_API(("{SATL} done saving SATL data [ret code: %d]\n",i4_rc));
    }
    else
    {
        i4_rc = SATLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_satl_fs_sync_given_file_handle
 *
 * Description: This API sync a specified SATL handle with SATL records
 *              from persistent file system.  The existing SATL records
 *              are replaced with new records read from file. 
 *
 * Inputs:
 *
 *   h_satl          Specify the handle to the SATL object.
 *
 *   h_file         The handle to the file.
 *
 *   ui2_satl_id     Specifies the SATL id to load from the file system.  
 *                  The limit for SATL ID is between 1 and 65535.
 *
 * Outputs: 
 *
 *   None.
 *                 
 * Returns: 
 *   SATLR_OK             SATL records are load from the persistent file
 *                       into the memory of SATL object.
 *
 *   SATLR_NOT_FOUND      'ui2_satl_id' is not found or 'ps_satl_path' 
 *                       does not exist.
 *
 *   SATLR_FAIL           internal error
 *
 *   SATLR_INV_ARG        'ps_satl_path' is NULL, or 'h_satl' is NULL_HANDLE,
 *                       or ui2_satl_id is 0.
 *
 *   SATLR_INV_FILE_PATH  Specified file path is not a directory or a RAW 
 *                       character file.
 *   
 -----------------------------------------------------------------------*/
INT32 x_satl_fs_sync_given_file_handle
(
    HANDLE_T		h_satl,	
    HANDLE_T        h_file,
    UINT16		    ui2_satl_id
)
{
    INT32           i4_rc;
    
    if ( h_satl       != NULL_HANDLE     &&
         ui2_satl_id  != 0   
        )
    {
        i4_rc=satl_fs_sync_object_given_file_handle(h_satl,
                                                   h_file,
                                                   ui2_satl_id);
    }
    else
    {
        i4_rc = SATLR_INV_ARG;
    }
    return i4_rc;
}
